options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_rnnt shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math
require dasllama/dasllama_plane public
require dasllama/dasllama_tower
require dasllama/dasllama_convert
require dasllama/dasllama_repack   // the halves' layouts
require dasllama/dasllama_kv_codec
require dasllama/dasllama_par
require dasllama/dasllama_gpu_kernels_common   // GkRnntArgs and the state's layout: the device seats' host form below

//! One decoded transducer token: vocab id, chosen duration (index + frames, both 0 on a plain RNN-T), the
//! encoder frame it was emitted at, and its (log-)probability under the joint's full-row log-softmax.
struct RnntToken {
    id : int
    dur_idx : int
    dur : int
    frame : int64
    p : float
    plog : float
}

let RNNT_MAX_LAYERS = 2   //!< a step publishes two stages a layer and two behind them, and a team publish takes six

//! A transducer's prediction and joint networks as offsets into the family's f32 weight plane `blob`.
//! The encoder half of the joint is the caller's: `rnnt_greedy` reads its projection precomputed.
struct RnntNet {
    blob : PlaneF = PlaneF()
    n_layers : int64
    pred_dim : int64               //!< the LSTM's width, its embedding's too
    joint_dim : int64              //!< the joint's hidden width
    n_tokens : int64               //!< the vocabulary with its blank
    blank_id : int
    durations : PlaneI32 = PlaneI32()   //!< the TDT duration table, its logits behind the tokens'; empty = a plain RNN-T
    max_symbols : int              //!< the tokens a frame emits before the decode moves on
    zero_start : bool              //!< the first step reads a zero input, not the blank's embedding row
    torch_gates : bool             //!< gate rows packed i, f, g, o (torch) rather than i, f, o, g
    two_biases : bool              //!< a layer carries `lstm_b` and `lstm_b2` (torch's b_ih and b_hh), summed once
    embed : int64                  //!< [n_tokens][pred_dim]
    lstm_ih : int64[RNNT_MAX_LAYERS]   //!< [4 pred_dim][pred_dim]
    lstm_hh : int64[RNNT_MAX_LAYERS]   //!< [4 pred_dim][pred_dim]
    lstm_b : int64[RNNT_MAX_LAYERS]    //!< [4 pred_dim]
    lstm_b2 : int64[RNNT_MAX_LAYERS]
    joint_pred_w : int64           //!< [joint_dim][pred_dim]
    joint_pred_b : int64
    joint_net_w : int64            //!< [n_tokens + durations][joint_dim]
    joint_net_b : int64
    prof_step : string = "rnnt.dec.step"     //!< the asr_prof bucket of a predictor step
    prof_pick : string = "rnnt.dec.pick"     //!< the bucket of a row's pick
    prof_joint : string = "rnnt.dec.joint"   //!< the bucket of a blank step's joint
    dev_only : bool                //!< a GPU-flavored image's net: the CPU never runs it, so a device decline panics
}

//! The decoder's per-session state: the LSTM's h/c, the projected prediction, the halves the session lays
//! once a weight plane, and the joint's logit row.
struct RnntState {
    h : array<float>          //!< [n_layers × pred_dim]
    c : array<float>
    pred : array<float>       //!< joint.pred projection of the last hidden [joint_dim]
    gates : array<float>      //!< scratch [4 × pred_dim]: the ih and hh dots summed
    xin : array<float>        //!< scratch [pred_dim]: layer input
    bias : array<float>       //!< [n_layers][4 pred_dim] each layer's gate bias, packed i, f, o, g
    wh : array<uint16>        //!< the stack's weights as halves: a layer's ih then hh [4 pred_dim][pred_dim], then joint.pred [joint_dim][pred_dim]
    wh_key : uint64           //!< the weight plane `wh` was converted from (0 = none)
    joint_wt : array<uint16>  //!< the joint net's weight as input columns in logit blocks, halves
    joint_wt_key : uint64     //!< the weight plane `joint_wt` was laid from (0 = none)
    @scratch dec_stages : array<int3>   //!< a decode step's stage list, int3(range_begin, range_end, num_chunks)
    logits : array<float>
}

let RNNT_JOINT_BLOCK = 256l   //!< the logits a lane's block of the joint table covers: its columns sit back to back, so a lane walks its block forward
let private RNNT_LSE_CUT = 32.0       //!< a logit this far under the row's maximum is left out of the log-sum; the bound is in the media architecture doc's transducer decode step section
let private RNNT_GATE_CHUNKS = 4      //!< a layer's gate math is a few microseconds whole: more chunks than this cost more in claims than they split
let private RNNT_STEP_JOINT = -1      //!< a blank step: the predictor stands, the joint reads it at a new frame
let private RNNT_STEP_ZERO = -2       //!< a predictor step on a zero input

//! A joint row's logits: the tokens', then the durations'.
def rnnt_n_logits(net : RnntNet) : int64 => net.n_tokens + length(net.durations)

//! The `RNNT_JOINT_BLOCK`-logit blocks a row of `n_logits` takes, the last zero-padded.
def rnnt_joint_blocks(n_logits : int64) : int64 => (n_logits + RNNT_JOINT_BLOCK - 1l) / RNNT_JOINT_BLOCK

//! Lays `s`'s halves and summed biases off `net`'s planes once a weight plane (`dasllama_repack`'s layouts).
def private rnnt_pred_halves(net : RnntNet; var s : RnntState) {
    let pd = net.pred_dim
    let key = intptr(plane_at(net.blob, net.lstm_ih[0]))
    let per_layer = 8l * pd * pd
    let need = net.n_layers * per_layer + net.joint_dim * pd
    return if (s.wh_key == key && long_length(s.wh) >= need)
    ensure_length(s.wh, need)
    ensure_length(s.bias, net.n_layers * 4l * pd)
    unsafe {
        var hp = addr(s.wh[0])
        var bp = addr(s.bias[0])
        for (l in range64(net.n_layers)) {
            let b2 = net.two_biases ? plane_at(net.blob, net.lstm_b2[l]) : null
            repack_lstm_gates_f16(hp + l * per_layer, bp + l * 4l * pd, plane_at(net.blob, net.lstm_ih[l]), plane_at(net.blob, net.lstm_hh[l]),
                plane_at(net.blob, net.lstm_b[l]), b2, pd, net.torch_gates)
        }
        cvt_f32_to_f16_sat(hp + net.n_layers * per_layer, plane_at(net.blob, net.joint_pred_w), net.joint_dim * pd)
    }
    s.wh_key = key
}

//! The joint net's weight as halves [block][joint_dim][RNNT_JOINT_BLOCK], once a weight plane a session; `col` is the caller's scratch of a block of floats.
def private rnnt_joint_cols(net : RnntNet; var s : RnntState; var col : array<float>; jd, n_logits : int64) {
    let key = intptr(plane_at(net.blob, net.joint_net_w))
    let nblk = rnnt_joint_blocks(n_logits)
    return if (s.joint_wt_key == key && long_length(s.joint_wt) >= nblk * jd * RNNT_JOINT_BLOCK)
    ensure_length(s.joint_wt, nblk * jd * RNNT_JOINT_BLOCK)
    unsafe {
        repack_joint_cols_f16(addr(s.joint_wt[0]), addr(col[0]), plane_at(net.blob, net.joint_net_w), jd, n_logits, RNNT_JOINT_BLOCK)
    }
    s.joint_wt_key = key
}

//! Sizes the state for `net`, zeroes the LSTM h/c (an utterance starts from zeros) and lays the halves; the scratch fields stay unzeroed, written before every read.
def private rnnt_reset(net : RnntNet; var s : RnntState) {
    if (net.n_layers < 1l || net.n_layers > int64(RNNT_MAX_LAYERS)) {
        panic("dasLLAMA rnnt: {net.n_layers} predictor layers; the decode step takes 1 to {RNNT_MAX_LAYERS}")
    }
    let pd = net.pred_dim
    let n_logits = rnnt_n_logits(net)
    ensure_length(s.h, net.n_layers * pd)
    ensure_length(s.c, net.n_layers * pd)
    ensure_length(s.pred, net.joint_dim)
    ensure_length(s.gates, 4l * pd)
    ensure_length(s.xin, pd)
    for (i in range64(net.n_layers * pd)) {
        s.h[i] = 0.0
        s.c[i] = 0.0
    }
    rnnt_pred_halves(net, s)
    ensure_length(s.logits, rnnt_joint_blocks(n_logits) * RNNT_JOINT_BLOCK)   // whole blocks: the joint writes zeros past the last logit
    rnnt_joint_cols(net, s, s.logits, net.joint_dim, n_logits)
}

//! One block of the joint's logits from its columns `wt`: y = b + W . relu(enc + pred) over the block's `live` logits, zeros past them.
def private rnnt_joint_block(var yp : float?; bp : float const?; wt : uint16 const?; ep, pp : float const?; jd, live : int64) {
    unsafe {
        copy_floats(yp, bp, live)
        for (i in range64(live, RNNT_JOINT_BLOCK)) {
            yp[i] = 0.0
        }
        for (j in range64(jd)) {
            let v = ep[j] + pp[j]
            if (v > 0.0) {
                axpy_f16(yp, wt + j * RNNT_JOINT_BLOCK, v, RNNT_JOINT_BLOCK)
            }
        }
    }
}

//! Gate math of elements [j0, j1) over the summed gate dots `gp` (packed i, f, o, g, `pd` a gate) plus their bias: c and h advance in place, h lands in `xp`; `j0` sits on a float4 group, a `j1` of `pd` takes the scalar tail.
def private rnnt_lstm_gates(gp, bp : float const?; var cp, hsp, xp : float?; pd, j0, j1 : int64) {
    // float4 gates: sigmoid(a) = 1/(1+exp4(-a)), tanh(a) = 1 - 2/(1+exp4(2a)) (the geglu4 identity)
    unsafe {
        let one = float4(1.0, 1.0, 1.0, 1.0)
        let two = float4(2.0, 2.0, 2.0, 2.0)
        let n4 = min((pd / 4l) * 4l, j1)
        var j = j0
        while (j < n4) {
            let ia = (*reinterpret<float4 const?>(gp + j)) + (*reinterpret<float4 const?>(bp + j))
            let fa = (*reinterpret<float4 const?>(gp + pd + j)) + (*reinterpret<float4 const?>(bp + pd + j))
            let oa = (*reinterpret<float4 const?>(gp + 2l * pd + j)) + (*reinterpret<float4 const?>(bp + 2l * pd + j))
            let ga = (*reinterpret<float4 const?>(gp + 3l * pd + j)) + (*reinterpret<float4 const?>(bp + 3l * pd + j))
            let ig = one / (one + exp4(clamp(-ia, EXP4_LO, EXP4_HI)))
            let fg = one / (one + exp4(clamp(-fa, EXP4_LO, EXP4_HI)))
            let og = one / (one + exp4(clamp(-oa, EXP4_LO, EXP4_HI)))
            let cg = one - two / (one + exp4(clamp(two * ga, EXP4_LO, EXP4_HI)))
            let cn = fg * (*reinterpret<float4 const?>(cp + j)) + ig * cg
            *reinterpret<float4?>(cp + j) = cn
            let hn = og * (one - two / (one + exp4(clamp(two * cn, EXP4_LO, EXP4_HI))))
            *reinterpret<float4?>(hsp + j) = hn
            *reinterpret<float4?>(xp + j) = hn
            j += 4l
        }
        for (jj in range64(max(n4, j0), j1)) {
            let ig = sigmoid_f32(gp[jj] + bp[jj])
            let fg = sigmoid_f32(gp[pd + jj] + bp[pd + jj])
            let og = sigmoid_f32(gp[2l * pd + jj] + bp[2l * pd + jj])
            let cg = tanh(gp[3l * pd + jj] + bp[3l * pd + jj])
            let cn = fg * cp[jj] + ig * cg
            cp[jj] = cn
            let hn = og * tanh(cn)
            hsp[jj] = hn
            xp[jj] = hn
        }
    }
}

//! What a decode step's stages read and write, by pointer: the workers of a team publish hold a copy.
struct private RnntStepArgs {
    xip : float?            //!< [pd] the layer input; each layer's gate math leaves its hidden here
    gp : float?             //!< [4 pd] the gate dots
    hp : float?             //!< [layers][pd]
    cp : float?             //!< [layers][pd]
    pp : float?             //!< [jd] joint.pred's output
    yp : float?             //!< [n_logits]
    whp : uint16 const?     //!< the stack's halves, joint.pred's behind the layers'
    wtp : uint16 const?     //!< the joint net's columns
    bjp : float const?
    lbp : float const?      //!< [layers][4 pd] the gate biases
    ep : float const?       //!< [jd] the frame's encoder projection
    ybp : float const?
    pd : int64
    jd : int64
    n_logits : int64
    nl : int
    ngate : int
}

//! Chunk [jb, je) of stage `sg` of a decode step: a layer's gate dots (even) and gate math (odd), then joint.pred's rows, then the logits.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step")]
def private rnnt_step_stage(var a : RnntStepArgs; sg, jb, je : int) {
    let pd = a.pd
    unsafe {
        if (sg < 2 * a.nl) {
            let l = int64(sg / 2)
            if (sg % 2 == 0) {
                let wih = a.whp + l * 8l * pd * pd
                let whh = wih + 4l * pd * pd
                let hl = a.hp + l * pd
                let xip = a.xip
                var gp = a.gp
                for (i in range(jb, je)) {
                    gp[i] = dot_f16(xip, wih + int64(i) * pd, pd) + dot_f16(hl, whh + int64(i) * pd, pd)
                }
            } else {
                rnnt_lstm_gates(a.gp, a.lbp + l * 4l * pd, a.cp + l * pd, a.hp + l * pd, a.xip, pd, int64(jb) * 4l, je == a.ngate ? pd : int64(je) * 4l)
            }
        } elif (sg == 2 * a.nl) {
            let wjp = a.whp + int64(a.nl) * 8l * pd * pd
            let xip = a.xip
            let bjp = a.bjp
            var pp = a.pp
            for (i in range(jb, je)) {
                pp[i] = dot_f16(xip, wjp + int64(i) * pd, pd) + bjp[i]
            }
        } else {
            for (b in range64(int64(jb), int64(je))) {
                let i0 = b * RNNT_JOINT_BLOCK
                rnnt_joint_block(a.yp + i0, a.ybp + i0, a.wtp + b * a.jd * RNNT_JOINT_BLOCK, a.ep, a.pp, a.jd, min(RNNT_JOINT_BLOCK, a.n_logits - i0))
            }
        }
    }
}

[arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step")]
def private rnnt_step(net : RnntNet; var s : RnntState; encp : array<float>; token : int; t : int64) {
    let pd = net.pred_dim
    let jd = net.joint_dim
    let nl = int(net.n_layers)
    let n_logits = rnnt_n_logits(net)
    let per_layer = 8l * pd * pd
    let ngate = int((pd + 3l) / 4l)
    let nblk = int(rnnt_joint_blocks(n_logits))
    let n_stages = 2 * nl + 2
    let dot_chunks = matmul_chunks_gemv(int(4l * pd), 1, per_layer)
    let proj_chunks = matmul_chunks_gemv(int(jd), 1, jd * pd)
    unsafe {
        var a = RnntStepArgs(
            xip = addr(s.xin[0]), gp = addr(s.gates[0]), hp = addr(s.h[0]), cp = addr(s.c[0]),
            pp = addr(s.pred[0]), yp = addr(s.logits[0]),
            whp = addr<uint16 const?>(s.wh[0]), wtp = addr<uint16 const?>(s.joint_wt[0]),
            bjp = plane_at(net.blob, net.joint_pred_b), lbp = addr<float const?>(s.bias[0]),
            ep = addr<float const?>(encp[t * jd]), ybp = plane_at(net.blob, net.joint_net_b),
            pd = pd, jd = jd, n_logits = n_logits, nl = nl, ngate = ngate)
        if (token >= 0) {
            copy_floats(a.xip, plane_at(net.blob, net.embed + int64(token) * pd), pd)
        } elif (token == RNNT_STEP_ZERO) {
            for (i in range64(pd)) {
                a.xip[i] = 0.0
            }
        }
        let first = token == RNNT_STEP_JOINT ? n_stages - 1 : 0
        if (get_dispatch_lanes() <= 1) {   //! one lane: a team publish would pay its rendezvous and barriers for nothing
            for (sg in range(first, n_stages)) {
                rnnt_step_stage(a, sg, 0, sg == n_stages - 1 ? nblk : sg == n_stages - 2 ? int(jd) : sg % 2 == 0 ? int(4l * pd) : ngate)
            }
            return
        }
        s.dec_stages |> resize(n_stages - first)
        for (sg in range(first, n_stages)) {
            s.dec_stages[sg - first] = (sg == n_stages - 1 ? int3(0, nblk, nblk)   //! a block a chunk: the lanes claim blocks as they free up
                : sg == n_stages - 2 ? int3(0, int(jd), proj_chunks)
                : sg % 2 == 0 ? int3(0, int(4l * pd), dot_chunks) : int3(0, ngate, RNNT_GATE_CHUNKS))
        }
        team_parallel_stages(s.dec_stages) <| @(stage : int; jb : int; je : int) {   // nolint:PERF026,LINT019 — dispatch-API lambda capture, see attention_chain_decode; the hot region enters from the family files
            rnnt_step_stage(a, stage + first, jb, je)
        }
    }
}

//! The lanes a decode step publishes to: `step_lanes` over its widest stage's work.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step")]
def rnnt_step_lanes(net : RnntNet) : int {
    let pd = net.pred_dim
    return step_lanes(max(8l * pd * pd, max(net.joint_dim * pd, rnnt_n_logits(net) * net.joint_dim)))
}

var g_rnnt_trace = false
var private g_rnnt_trace_announced = false

//! The debug rail: every CPU decode prints each frame's pick and duration probabilities; announced once a process.
def set_rnnt_trace(on : bool) {
    g_rnnt_trace = on
}

[cold_path]
def private rnnt_trace_step(s : RnntState; t : int64; n_tok_logits, n_dur, best, bdur : int; mx : float) {
    if (!g_rnnt_trace_announced) {
        g_rnnt_trace_announced = true
        to_log(LOG_WARNING, "dasLLAMA rnnt: set_rnnt_trace(true) prints every frame's pick of the CPU decode; set_rnnt_trace(false) turns it off\n")
    }
    print("  t={t} tok={best} durp=[")
    for (i in range(n_dur)) {
        print("{exp(s.logits[n_tok_logits + i] - mx)} ")
    }
    print("] -> {bdur}\n")
}

//! The row's token argmax and the maximum over the whole row, durations included.
def private rnnt_row_max(lp : float const?; n_tok_logits : int; n_logits : int64) : tuple<best : int; mx : float> {
    var best = 0
    var mx = unsafe(lp[0])
    for (i in range(1, n_tok_logits)) {
        let v = unsafe(lp[i])
        if (v > mx) {
            mx = v
            best = i
        }
    }
    for (i in range64(int64(n_tok_logits), n_logits)) {
        mx = max(mx, unsafe(lp[i]))
    }
    return (best = best, mx = mx)
}

//! The duration index: argmax over fp32 softmax values, not logits - the oracle's probs flush the durations' ~e^-110 mass to 0.0, so ties go to index 0 as the oracle's do; 0 with no durations.
def private rnnt_pick_duration(s : RnntState; n_tok_logits, n_dur : int; mx : float) : int {
    return 0 if (n_dur == 0)
    var bdur = 0
    var bdp = exp(s.logits[n_tok_logits] - mx)
    for (i in range(1, n_dur)) {
        let dp = exp(s.logits[n_tok_logits + i] - mx)
        if (dp > bdp) {
            bdp = dp
            bdur = i
        }
    }
    return bdur
}

//! The emitted token's record: its log-probability under the WHOLE row's log-softmax, durations included (the oracle's plog), and its probability within the token slice, whose mass is this pass's partial sum.
def private rnnt_token(lp : float const?; best, bdur, dur : int; t : int64; n_tok_logits : int; n_logits : int64; mx : float) : RnntToken {
    let cut = mx - RNNT_LSE_CUT
    var lse = 0.0lf
    for (i in range(n_tok_logits)) {
        let v = unsafe(lp[i])
        if (v > cut) {
            lse += double(exp(v - mx))
        }
    }
    let tok_mass = lse
    for (i in range64(int64(n_tok_logits), n_logits)) {
        let v = unsafe(lp[i])
        if (v > cut) {
            lse += double(exp(v - mx))
        }
    }
    let plog = unsafe(lp[best]) - (float(log(lse)) + mx)
    return RnntToken(id = best, dur_idx = bdur, dur = dur, frame = t,
        p = float(double(exp(plog)) / (tok_mass / lse)), plog = plog)
}

//! Greedy transducer decode over `n_frames` rows of `encp`, each frame's joint encoder projection (enc · W + b,
//! [n_frames][joint_dim]): a blank advances by its duration (at least a frame), a token by its own - a 0, every
//! token of a plain RNN-T, stays on the frame up to `max_symbols`. Tokens land in `toks`, cleared first.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step")]
def rnnt_greedy(net : RnntNet; var s : RnntState; encp : array<float>; n_frames : int64; @scratch var toks : array<RnntToken>) {
    toks |> clear()
    let n_logits = rnnt_n_logits(net)
    let n_tok_logits = int(net.n_tokens)
    let n_dur = int(length(net.durations))
    rnnt_reset(net, s)
    let pool = dispatch_lanes_narrow(rnnt_step_lanes(net))

    var t = 0l
    var emitted = 0
    var ts = ref_time_ticks()
    if (n_frames > 0l) {
        rnnt_step(net, s, encp, net.zero_start ? RNNT_STEP_ZERO : net.blank_id, 0l)
    }
    asr_prof_add(net.prof_step, ts)
    while (t < n_frames) {
        ts = ref_time_ticks()
        // explicit n_logits bounds: the scratch arrays are oversized on reuse
        let lp = unsafe(addr<float const?>(s.logits[0]))
        let (best, mx) = rnnt_row_max(lp, n_tok_logits, n_logits)
        let bdur = rnnt_pick_duration(s, n_tok_logits, n_dur, mx)
        var dur = n_dur == 0 ? 0 : net.durations[bdur]
        if (g_rnnt_trace) {
            rnnt_trace_step(s, t, n_tok_logits, n_dur, best, bdur, mx)
        }

        if (best == net.blank_id) {
            if (dur == 0) {
                dur = 1
            }
            t += int64(dur)
            emitted = 0
            asr_prof_add(net.prof_pick, ts)
            if (t < n_frames) {
                ts = ref_time_ticks()
                rnnt_step(net, s, encp, RNNT_STEP_JOINT, t)
                asr_prof_add(net.prof_joint, ts)
            }
            continue
        }

        toks |> push(rnnt_token(lp, best, bdur, dur, t, n_tok_logits, n_logits, mx))
        asr_prof_add(net.prof_pick, ts)
        if (dur > 0) {
            t += int64(dur)
            emitted = 0
        } else {
            emitted++
            if (emitted >= net.max_symbols) {
                t++
                emitted = 0
            }
        }
        if (t < n_frames) {
            ts = ref_time_ticks()
            rnnt_step(net, s, encp, best, t)
            asr_prof_add(net.prof_step, ts)
        }
    }
    dispatch_lanes_restore(pool)
}

//! The joint's encoder half as a family's lane holds it: the projection [joint_dim][d] at `w` - in the f32 plane, or in the
//! lane's q8 plane where `q8` - and its f32 bias row at `b`; `gen` the encode generation the session recorded with its rows (the
//! home's handoff key, 0 for none).
struct RnntEnc {
    d : int64
    w : int64
    b : int64
    q8 : bool
    gen : uint64
}

//! The device decode hook: the whole greedy loop over `n_frames` encoder rows `x` - the encode's own device rows where it left
//! them there - the projection on the device where the home `projects` the lane, else off the family's biased `encp`; false
//! declines to the CPU loop.
typedef RnntGpuGreedyFn = function<(net : RnntNet; var s : RnntState; enc : RnntEnc; x, encp : array<float>; n_frames : int64; var toks : array<RnntToken>) : bool>
//! Whether the device home projects a lane's encoder half itself; where it does not, the family projects ahead of the hook.
typedef RnntGpuProjectsFn = function<(enc : RnntEnc) : bool>
var private g_rnnt_gpu : RnntGpuGreedyFn
var private g_rnnt_gpu_projects : RnntGpuProjectsFn
var private g_rnnt_gpu_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_rnnt_gpu(fn : RnntGpuGreedyFn; projects : RnntGpuProjectsFn) {
    g_rnnt_gpu = fn
    g_rnnt_gpu_projects = projects
    g_rnnt_gpu_set = true
}

//! A device decode seat is registered and projects the encoder half of `enc` itself - a GPU flavor's decode runs whole on it
def rnnt_gpu_projects(enc : RnntEnc) : bool => g_rnnt_gpu_set && invoke(g_rnnt_gpu_projects, enc)

//! The greedy decode over the encoder rows `x`: the device hook first, else `project` lays the joint's encoder projection
//! of every row into `encp` and the CPU loop runs (`rnnt_greedy`). Where the device home does not project the lane, the
//! family projects first and the hook decodes off the result. Tokens land in `toks`.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step")]
def rnnt_decode(net : RnntNet; var s : RnntState; enc : RnntEnc; x : array<float>; @scratch var encp : array<float>; n_frames : int64;
                @scratch var toks : array<RnntToken>; project : block<(var encp : array<float>) : void>) {
    toks |> clear()
    var projected = false
    if (g_rnnt_gpu_set) {
        if (!invoke(g_rnnt_gpu_projects, enc)) {
            invoke(project, encp)
            projected = true
        }
        rnnt_reset(net, s)
        return if (invoke(g_rnnt_gpu, net, s, enc, x, encp, n_frames, toks))
    }
    if (net.dev_only) {
        panic("dasLLAMA rnnt: the device decode declined a GPU-flavored image's net, which never runs on the CPU")
    }
    if (!projected) {
        invoke(project, encp)
    }
    rnnt_greedy(net, s, encp, n_frames, toks)
}

// ===== the device seats' host form: what the Metal and the Vulkan drivers lay, start, drive and land alike =====

let RNNT_DEV_ROUNDS_MIN = 8l    //! a command buffer's fewest rounds: the loop's tail past a short estimate
let RNNT_DEV_ROUNDS_MAX = 256l  //! ... its most: a command buffer stays bounded however long the clip

//! A seat failure a test pins, so each decline arm runs its own release path: a seat takes it where the real failure lands.
enum RnntDevFault {
    none
    device      //! a pipeline or a weight buffer failed
    memory      //! a device allocation failed
    gpu_error   //! a command buffer failed
}

var private g_rnnt_dev_fault = RnntDevFault.none
var private g_rnnt_dev_budget_pin = 0l

//! Test seat: the failure the device seats take until it is pinned back to `none`; answers the pin it replaced.
def set_rnnt_dev_fault(f : RnntDevFault) : RnntDevFault {
    let was = g_rnnt_dev_fault
    g_rnnt_dev_fault = f
    return was
}

//! Whether the seat takes failure `f` here.
def rnnt_dev_faulted(f : RnntDevFault) : bool => g_rnnt_dev_fault == f

//! Test seat: a round budget every device decode takes in place of its own, 0 for its own; answers the pin it replaced.
def set_rnnt_dev_round_budget(n : int64) : int64 {
    let was = g_rnnt_dev_budget_pin
    g_rnnt_dev_budget_pin = max(n, 0l)
    return was
}

//! The rounds a clip of `n_frames` takes at most: a token round emits one of at most `max_symbols` a frame, any other round
//! spends its window or ends the clip.
def rnnt_dev_round_budget(net : RnntNet; n_frames : int64) : int64 {
    return g_rnnt_dev_budget_pin if (g_rnnt_dev_budget_pin > 0l)
    return n_frames * int64(max(net.max_symbols, 1) + 1) + 4l
}

def private rnnt_key_region(h : uint64; net : RnntNet; off, n : int64; whole : bool) : uint64 {
    var k = h
    let p = plane_at(net.blob, off)
    let step = whole ? 1l : max(n / 256l, 1l)
    var i = 0l
    while (i < n) {
        unsafe {
            k = hash_combine64(k, uint64(reinterpret<uint>(p[i])))
        }
        i += step
    }
    return k
}

//! The device copy's identity: the plane, every offset and flag the halves and the slab are laid by, the encoder half, the small
//! regions' values whole and a strided sample of the large ones'. Two sessions of one model share it; a net laid at a freed
//! plane's address with other values does not.
def rnnt_dev_key(net : RnntNet; enc : RnntEnc) : uint64 {
    let pd = net.pred_dim
    let jd = net.joint_dim
    let n_logits = rnnt_n_logits(net)
    var h = hash_combine64(intptr(plane_at(net.blob, 0l)), uint64(length(net.blob)))
    for (v in fixed_array(net.n_layers, pd, jd, net.n_tokens, int64(net.blank_id), net.embed, net.joint_pred_w, net.joint_pred_b,
            net.joint_net_w, net.joint_net_b, enc.d, enc.w, enc.b, net.torch_gates ? 1l : 0l, net.two_biases ? 1l : 0l, enc.q8 ? 1l : 0l)) {
        h = hash_combine64(h, uint64(v))
    }
    for (l in range64(net.n_layers)) {
        h = hash_combine64(hash_combine64(h, uint64(net.lstm_ih[l])), hash_combine64(uint64(net.lstm_hh[l]), uint64(net.lstm_b[l])))
        h = rnnt_key_region(h, net, net.lstm_ih[l], 4l * pd * pd, false)
        h = rnnt_key_region(h, net, net.lstm_hh[l], 4l * pd * pd, false)
        h = rnnt_key_region(h, net, net.lstm_b[l], 4l * pd, true)
        if (net.two_biases) {
            h = rnnt_key_region(hash_combine64(h, uint64(net.lstm_b2[l])), net, net.lstm_b2[l], 4l * pd, true)
        }
    }
    h = rnnt_key_region(h, net, net.embed, net.n_tokens * pd, false)
    h = rnnt_key_region(h, net, net.joint_pred_w, jd * pd, false)
    h = rnnt_key_region(h, net, net.joint_pred_b, jd, true)
    h = rnnt_key_region(h, net, net.joint_net_w, n_logits * jd, false)
    h = rnnt_key_region(h, net, net.joint_net_b, n_logits, true)
    if (!enc.q8) {
        h = rnnt_key_region(h, net, enc.w, jd * enc.d, false)
        h = rnnt_key_region(h, net, enc.b, jd, true)
    }
    return h
}

//! Whether a net fits the stamps' staged widths - the predictor, the joint's window of hidden rows, one or two layers, the
//! duration table - and, where the seat projects the encoder half, the encoder width on the 32 lattice and the joint width on
//! the seat's projection lattice `jd_lattice`: the one admission both seats ask.
def rnnt_dev_shape_ok(net : RnntNet; enc : RnntEnc; jd_lattice : int64) : bool {
    let jd = net.joint_dim
    return false if (net.pred_dim > int64(GK_RNNT_MAX_PD) || jd * int64(GK_RNNT_FRAMES) > int64(GK_RNNT_MAX_HID)
        || net.n_layers < 1l || net.n_layers > int64(RNNT_MAX_LAYERS) || length(net.durations) > int64(GK_RNNT_MAX_DUR))
    return enc.q8 || (jd % jd_lattice == 0l && enc.d % 32l == 0l)
}

//! The f32 slab `wf` a seat uploads, as element offsets - the token table, joint.pred's bias, the joint net's bias, the encoder
//! projection's bias, the summed gate biases - and the halves' counts.
struct RnntDevLayout {
    embed : int64
    jpb : int64
    jnb : int64
    jeb : int64
    lb : int64
    wf_floats : int64
    wh_halves : int64   //! the state's `wh`
    jw_halves : int64   //! the state's `joint_wt`
}

def rnnt_dev_layout(net : RnntNet) : RnntDevLayout {
    let pd = net.pred_dim
    let jd = net.joint_dim
    let n_logits = rnnt_n_logits(net)
    let jpb = net.n_tokens * pd
    let lb = jpb + 2l * jd + n_logits
    return RnntDevLayout(embed = 0l, jpb = jpb, jnb = jpb + jd, jeb = jpb + jd + n_logits, lb = lb, wf_floats = lb + net.n_layers * 4l * pd,
        wh_halves = net.n_layers * 8l * pd * pd + jd * pd, jw_halves = rnnt_joint_blocks(n_logits) * jd * RNNT_JOINT_BLOCK)
}

//! The slab at `lay`'s offsets off the net's plane and the state's summed biases; the encoder projection's bias stays zero where
//! the family hands its projection over biased.
def rnnt_dev_slab(net : RnntNet; s : RnntState; enc : RnntEnc; lay : RnntDevLayout) : array<float> {
    let pd = net.pred_dim
    let jd = net.joint_dim
    var wf : array<float>
    wf |> reserve(lay.wf_floats)
    wf |> resize(lay.wf_floats)
    unsafe {
        memcpy(addr(wf[lay.embed]), plane_at(net.blob, net.embed), net.n_tokens * pd * 4l)
        memcpy(addr(wf[lay.jpb]), plane_at(net.blob, net.joint_pred_b), jd * 4l)
        memcpy(addr(wf[lay.jnb]), plane_at(net.blob, net.joint_net_b), rnnt_n_logits(net) * 4l)
        if (!enc.q8) {
            memcpy(addr(wf[lay.jeb]), plane_at(net.blob, enc.b), jd * 4l)
        }
        memcpy(addr(wf[lay.lb]), addr(s.bias[0]), net.n_layers * 4l * pd * 4l)
    }
    return <- wf
}

//! The token records a clip of `frames` holds at most.
def rnnt_dev_cap(net : RnntNet; frames : int64) : int64 => frames * int64(max(net.max_symbols, 1)) + 1l

//! A decode's kargs: the net's shape, the slab's offsets, the logit row stride; `eb_on` where the seat projects unbiased.
def rnnt_dev_args(net : RnntNet; lay : RnntDevLayout; n_frames : int64; eb_on : bool) : GkRnntArgs {
    return GkRnntArgs(pd = uint(net.pred_dim), jd = uint(net.joint_dim), nl = uint(net.n_layers), layer = 0u, n_tok = uint(net.n_tokens),
        n_dur = uint(length(net.durations)), n_frames = uint(n_frames), blank = uint(net.blank_id), max_sym = uint(net.max_symbols),
        cap = uint(rnnt_dev_cap(net, n_frames)), boff = uint(lay.jpb), embed = uint(lay.embed), jnb = uint(lay.jnb), jeb = uint(lay.jeb),
        eb_on = eb_on ? 1u : 0u)
}

//! The gate stamp's kargs of layer `l`: the layer the stamp reads its row planes off, its gate biases in the slab.
def rnnt_dev_layer_args(ka : GkRnntArgs; lay : RnntDevLayout; l : int64) : GkRnntArgs {
    let lu = uint(l)
    var kl = ka
    kl.layer = lu
    kl.boff = uint(lay.lb) + lu * 4u * ka.pd
    return kl
}

//! The clip's state words at `sp`: t, emitted and done at zero, the first round's predictor step asked, the duration table.
def rnnt_dev_state_words(net : RnntNet; var sp : uint?) {
    unsafe {
        for (i in range(int(GK_RNNT_ST_WORDS))) {
            sp[i] = 0u
        }
        sp[3] = 1u
        for (i in range64(length(net.durations))) {
            sp[int64(GK_RNNT_ST_DUR) + i] = uint(net.durations[i])
        }
    }
}

//! The clip's first predictor input at `xp`: zeros, or the blank's embedding row.
def rnnt_dev_first_input(net : RnntNet; var xp : float?) {
    let pd = net.pred_dim
    unsafe {
        if (net.zero_start) {
            for (i in range64(pd)) {
                xp[i] = 0.0
            }
        } else {
            memcpy(xp, plane_at(net.blob, net.embed + int64(net.blank_id) * pd), pd * 4l)
        }
    }
}

//! What the state words say after a command buffer.
struct RnntDevProgress {
    t : int64
    ntok : int
    rounds : int64
    steps : int64
    done : bool
}

//! A served decode's rounds loop and how it ended: `ok` false where a command buffer failed or, `overrun`, the rounds ran
//! past the budget with the clip not done.
struct RnntDevRun {
    ok : bool
    overrun : bool
    cbs : int64
    p : RnntDevProgress
}

def private rnnt_dev_progress(sp : uint const?) : RnntDevProgress {
    unsafe {
        return RnntDevProgress(t = int64(sp[0]), ntok = int(sp[5]), rounds = int64(sp[7]), steps = int64(sp[6]), done = sp[2] != 0u)
    }
}

//! Command buffers of rounds through `submit` until the state words at `sp` read done: each carries the frames left at the rate
//! the clip has taken so far - `rate`, the last served clip's, before its first - a tenth more, within the round budget; `rate`
//! takes the clip's where it is served.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-device-decode")]
def rnnt_dev_rounds(net : RnntNet; n_frames : int64; sp : uint const?; var rate : double&; submit : block<(rounds : int64; first : bool) : bool>) : RnntDevRun {
    let budget = rnnt_dev_round_budget(net, n_frames)
    var run = RnntDevRun(ok = true)
    var sent = 0l
    while (true) {
        let est = run.p.t > 0l ? double(run.p.rounds) / double(run.p.t) : rate
        let rounds = min(clamp(int64(double(n_frames - run.p.t) * est * 1.1lf) + 4l, RNNT_DEV_ROUNDS_MIN, RNNT_DEV_ROUNDS_MAX), budget - sent)
        run.ok = invoke(submit, rounds, run.cbs == 0l) && !rnnt_dev_faulted(RnntDevFault.gpu_error)
        run.cbs++
        sent += rounds
        break if (!run.ok)
        run.p = rnnt_dev_progress(sp)
        if (run.p.done) {
            rate = double(run.p.rounds) / double(n_frames)
            break
        }
        if (sent >= budget) {
            run.ok = false
            run.overrun = true
            break
        }
    }
    return run
}

//! The served decode's `n` token records at `rp` into `toks`.
def rnnt_dev_land_tokens(rp : uint const?; n : int; @scratch var toks : array<RnntToken>) {
    toks |> reserve(n)
    unsafe {
        for (i in range(n)) {
            let r = rp + i * int(GK_RNNT_TOK_WORDS)
            toks |> push(RnntToken(id = int(r[0]), dur_idx = int(r[1]), dur = int(r[2]), frame = int64(r[3]),
                p = reinterpret<float>(r[4]), plog = reinterpret<float>(r[5])))
        }
    }
}
