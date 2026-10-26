// The transducer decode kernels' CPU references and their synthetic fixture, shared by the Metal and the Vulkan kernel cells:
// the LSTM layer's gates and cell, joint.pred, the joint over a window of frames and the greedy pick, each over the layouts
// `GkRnntArgs` names - the halves the CPU decoder lays (a layer's ih then hh rows, gate rows i, f, o, g, joint.pred behind the
// layers; the joint net's columns in 256-logit blocks) and the f32 rows the drivers lay. The weights are multiples of 1/1024,
// so their halves hold them exactly.

options gen2
options _dasllama_internal = true

module _rnnt_oracle shared public

require dasllama/dasllama_lint public
require dastest/testing_boost
require math
require strings   // build_string: the id lists a twin logs
require daslib/f16_cvt   // f32_to_f16: the halves' bits
require daslib/math_bits   // float_bits_to_uint: the records' p and plog words
require dasllama/dasllama_gpu_kernels_common   // GkRnntArgs, the state's and records' layouts
require _compares   // NAN_SENTINEL_BITS, hash_weight / put_hashed, sigmoid_d, sentinel_off, check_exact, nan_wide_diff
require dasllama/dasllama_rnnt   // RnntToken: the twins' tokens
require ?das_metal dasllama/dasllama_metal_asr_dec   // the Metal transducer seat's pin and counters
require ?das_metal dasllama/dasllama_metal_common    // metal_common_init: whether the box has a Metal device
require ?vulkan dasllama/dasllama_vulkan_asr_dec     // the Vulkan seat's, on a build without das_metal
require ?vulkan dasllama/dasllama_vulkan_tower       // vulkan_tower_arms: whether the tier serves on this box

//! The build's device transducer seat: Metal, else Vulkan, else none.
def rnnt_seat_name : string {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return "metal"
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return "vulkan"
        } else {
            return ""
        }
    }
}

//! whether the build's seat can serve on this box: a Metal build's where the box has a Metal device, a Vulkan build's where the
//! tier arms a device
def rnnt_seat_ready : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_common_init()
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_arms()
        } else {
            return false
        }
    }
}

//! the build's device transducer seat pinned on or off; answers the setting it replaced (false with no seat)
[unused_argument(on)]
def rnnt_seat_pin(on : bool) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return set_metal_rnnt(on)
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return set_vulkan_rnnt(on)
        } else {
            return false
        }
    }
}

//! the build's device transducer seat's counters: decodes, handoffs, projects, weight uploads, declines but the knob's (zeros
//! with no seat)
def rnnt_seat_stats : tuple<decodes : int64; handoffs : int64; projects : int64; uploads : int64; declines : int64> {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let s = metal_rnnt_stats()
        return (decodes = s.decodes, handoffs = s.handoffs, projects = s.projects, uploads = s.uploads, declines = s.declines)
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let s = vulkan_rnnt_stats()
            return (decodes = s.decodes, handoffs = s.handoffs, projects = s.projects, uploads = s.uploads, declines = s.declines)
        } else {
            return (decodes = 0l, handoffs = 0l, projects = 0l, uploads = 0l, declines = 0l)
        }
    }
}

//! the build's device transducer seat's declines of reason `why`, the knob's included (0 with no seat)
[unused_argument(why)]
def rnnt_seat_declines(why : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_rnnt_declines()?[why] ?? 0l
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_rnnt_declines()?[why] ?? 0l
        } else {
            return 0l
        }
    }
}

//! true, with the cell skipped, where the build has no device transducer seat or the box no device for it
def rnnt_seat_skip(t : T?; tag : string) : bool {
    return false if (rnnt_seat_name() != "" && rnnt_seat_ready())
    t |> skip("{tag}: no device transducer seat on this build or box")
    return true
}

//! the ids of `toks`, space-separated: a decode's side of a compare's log
def rnnt_id_list(toks : array<RnntToken>) : string {
    return build_string() $(var w) {
        for (x in toks) {
            w |> write("{x.id} ")
        }
    }
}

//! The device decode's tokens against another decoder's: the same (id, frame, duration index, duration) tuples, the plog within
//! `bar`; the cell's verdicts and both sides' ids logged under `tag`.
def rnnt_tokens_match(t : T?; tag : string; got, want : array<RnntToken>; bar : float) {
    t |> equal(length(got), length(want), "{tag}: the token count")
    var off = 0
    var worst = 0.0
    for (a, b in got, want) {
        if (a.id != b.id || a.frame != b.frame || a.dur_idx != b.dur_idx || a.dur != b.dur) {
            if (off == 0) {
                to_log(LOG_WARNING, "{tag}: first tuple off - got {a.id}@{a.frame}+{a.dur}, want {b.id}@{b.frame}+{b.dur}\n")
            }
            off++
        }
        worst = max(worst, nan_wide_diff(a.plog, b.plog))
    }
    to_log(LOG_INFO, "{tag}: {length(want)} tokens, {off} off, plog off by {worst}\n  got  ids: {rnnt_id_list(got)}\n  want ids: {rnnt_id_list(want)}\n")
    t |> equal(off, 0, "{tag}: the tuples are the reference's")
    t |> success(worst <= bar, "{tag}: plog off by {worst}")
}

//! the plogs of `toks`: a decode's side of a plog compare
def plogs_of(toks : array<RnntToken>) : array<float> => <- [for (x in toks); x.plog]

//! A device twin's two decodes: the CPU loop's tokens and the device's.
struct RnntTwin {
    cpu : array<RnntToken>
    dev : array<RnntToken>
}

//! A family's device-decode twin over one clip's encoder rows `x`: `decode` run with the seat pinned off (the CPU loop - the decode
//! counter must not move, so a knob that pins nothing reds) and pinned on (counted on the device, `projects` / `handoffs` of
//! them as the family's lane expects), the tokens alike within `bar`. The control decodes `x` scaled by 1.05 on the CPU loop and
//! its plogs must land outside `bar` of the device's: a bar the encoder rows cannot move proves nothing. `x` is put back bit for
//! bit. Answers both decodes for the caller's log. The caller skips through `rnnt_seat_skip` first.
def rnnt_device_twin(t : T?; tag : string; bar : float; projects, handoffs : int64; var x : array<float>;
                     decode : block<(var toks : array<RnntToken>) : void>) : RnntTwin {
    var tw : RnntTwin
    let was = rnnt_seat_pin(false)
    let k0 = rnnt_seat_stats()
    invoke(decode, tw.cpu)
    let k1 = rnnt_seat_stats()
    rnnt_seat_pin(true)
    invoke(decode, tw.dev)
    let s1 = rnnt_seat_stats()
    rnnt_seat_pin(false)
    var inscope keep := x
    for (v in x) {
        v *= 1.05
    }
    var inscope moved : array<RnntToken>
    invoke(decode, moved)
    x := keep
    rnnt_seat_pin(was)
    t |> equal(k1.decodes - k0.decodes, 0l, "{tag}: pinned off, the decode stays on the CPU loop")
    t |> equal(s1.decodes - k1.decodes, 1l, "{tag}: the decode served on the device")
    t |> equal(s1.projects - k1.projects, projects, "{tag}: the encoder half projected on the device where the lane's home projects it")
    t |> equal(s1.handoffs - k1.handoffs, handoffs, "{tag}: the projection read the encode's device rows where the device encoded")
    t |> success(!empty(tw.cpu), "{tag}: the clip decodes to tokens")
    rnnt_tokens_match(t, tag, tw.dev, tw.cpu, bar)
    check_exceeds(t, "{tag}: the control - the encoder rows scaled by 1.05 move the plogs past the bar", plogs_of(tw.dev), plogs_of(moved),
        Bar(abs_floor = bar))
    return <- tw
}

let RC_NL = 2
let RC_NDUR = 3
let RC_FRAMES = 20
let RC_DURS = fixed_array(0, 1, 2)

let private RC_LATTICE = 1024.0   //! the weights' grid: their halves hold them exactly

//! A fixture's widths - the predictor's `pd`, the joint's `jd`, the token logits `ntok` with the blank the last - and what they
//! derive: a row's logits `nlog` (the tokens, then the durations), its 256-logit blocks `nblk` and the row stride `lstride`.
struct RcShape {
    pd : int
    jd : int
    ntok : int
    nlog : int
    nblk : int
    lstride : int
}

def rc_shape(pd, jd, ntok : int) : RcShape {
    let nlog = ntok + RC_NDUR
    let nblk = (nlog + 255) / 256
    return RcShape(pd = pd, jd = jd, ntok = ntok, nlog = nlog, nblk = nblk, lstride = nblk * 256)
}

//! The fixture's two shapes. The first sits off every lattice: a predictor of 36 leaves dead groups in the last eight-element
//! workgroup, a joint of 68 is an eighth of 9 and 5 columns - the four-column steps and their tail - and 290 tokens with the
//! durations make the second 256-logit block partial. The second sits on them: 64-wide predictor and joint (whole strides, no
//! tail) and 100 tokens, a row of logits under one 256-thread stride.
def rc_shapes : array<RcShape> => <- [rc_shape(36, 68, 290), rc_shape(64, 64, 100)]

//! the shape's tag in a cell's log
def rc_tag(sh : RcShape) : string => "pd {sh.pd} jd {sh.jd} ntok {sh.ntok}"

//! a weight in [-scale, scale) on the `RC_LATTICE` grid off an integer hash of its index and a salt
def rc_w(i, salt : int; scale : float) : float => hash_weight(i, salt, scale, RC_LATTICE)

//! One synthetic transducer of shape `sh`: the halves `wh` / `jw` with their widened twins, the f32 rows `wf` at the offsets `pa`
//! names, the hidden pair and the cells, the predictor input, joint.pred's output and the encoder projection.
struct RnntFix {
    sh : RcShape
    wh : array<uint16>
    whf : array<float>
    jw : array<uint16>
    jwf : array<float>
    wf : array<float>
    xin : array<float>
    hb : array<float>
    cb : array<float>
    pred : array<float>
    encp : array<float>
    lb : int   //! the gate biases' first row in wf
    pa : GkRnntArgs
}

def private put_halves(var h : array<uint16>; var f : array<float>; n, salt : int; scale : float) {
    let at = length(f)
    h |> resize(at + n)
    f |> resize(at + n)
    for (i in range(n)) {
        let v = rc_w(i, salt, scale)
        f[at + i] = v
        h[at + i] = uint16(f32_to_f16(v))
    }
}

//! the fixture at shape `sh`: two layers, `RC_FRAMES` frames of encoder projection, the hidden pair and cells away from zero
def rnnt_fixture(sh : RcShape) : RnntFix {
    var fx = RnntFix(sh = sh)
    let pd = sh.pd
    let jd = sh.jd
    for (l in range(RC_NL)) {
        put_halves(fx.wh, fx.whf, 8 * pd * pd, 10 + l, 0.25)
    }
    put_halves(fx.wh, fx.whf, jd * pd, 20, 0.25)
    fx.jwf |> reserve(sh.nblk * jd * 256)
    fx.jw |> reserve(sh.nblk * jd * 256)
    for (b in range(sh.nblk)) {
        for (j in range(jd)) {
            for (i in range(256)) {
                let li = b * 256 + i
                let v = li < sh.nlog ? rc_w(j * 256 + li, 30, 0.2) : 0.0
                fx.jwf |> push(v)
                fx.jw |> push(uint16(f32_to_f16(v)))
            }
        }
    }
    let embed = put_hashed(fx.wf, sh.ntok * pd, 40, 0.8, RC_LATTICE)
    let jpb = put_hashed(fx.wf, jd, 41, 0.3, RC_LATTICE)
    let jnb = put_hashed(fx.wf, sh.nlog, 42, 0.5, RC_LATTICE)
    let jeb = put_hashed(fx.wf, jd, 43, 0.3, RC_LATTICE)
    fx.lb = int(put_hashed(fx.wf, RC_NL * 4 * pd, 44, 0.4, RC_LATTICE))
    fx.xin <- [for (i in range(pd)); rc_w(i, 50, 0.9)]
    fx.hb <- [for (i in range(2 * RC_NL * pd)); rc_w(i, 51, 0.6)]
    fx.cb <- [for (i in range(RC_NL * pd)); rc_w(i, 52, 0.8)]
    fx.pred <- [for (i in range(jd)); rc_w(i, 53, 0.5)]
    fx.encp <- [for (i in range(RC_FRAMES * jd)); rc_w(i, 54, 0.6)]
    fx.pa = GkRnntArgs(pd = uint(pd), jd = uint(jd), nl = uint(RC_NL), n_tok = uint(sh.ntok), n_dur = uint(RC_NDUR),
        n_frames = uint(RC_FRAMES), blank = uint(sh.ntok - 1), max_sym = 2u, cap = 64u, boff = jpb, embed = embed, jnb = jnb, jeb = jeb,
        eb_on = 1u)
    return <- fx
}

//! the fixture's kargs for the gates of `layer`: the layer (its two row planes derive from it) and its gate biases
def rnnt_gates_args(fx : RnntFix; layer : int) : GkRnntArgs {
    var pa = fx.pa
    pa.layer = uint(layer)
    pa.boff = uint(fx.lb + layer * 4 * fx.sh.pd)
    return pa
}

//! The state words of a round: t, emitted, done, need, the h parity, the tokens, the predictor steps, the rounds; the duration
//! table behind.
def rnnt_state(t, emitted, done, need, hpar, ntok : int) : array<uint> {
    var st : array<uint>
    st |> resize(int(GK_RNNT_ST_WORDS))
    st[0] = uint(t)
    st[1] = uint(emitted)
    st[2] = uint(done)
    st[3] = uint(need)
    st[4] = uint(hpar)
    st[5] = uint(ntok)
    st[6] = 3u
    st[7] = 5u
    for (i in range(RC_NDUR)) {
        st[int(GK_RNNT_ST_DUR) + i] = uint(RC_DURS[i])
    }
    return <- st
}

//! layer `layer`'s gates at h parity `cur` (`GkRnntPred`, GATES): the hidden pair and the cells as the kernel leaves them - the
//! layer's new hidden in the other half of the pair, its cells moved, every other element as it was
def rnnt_gates_ref(fx : RnntFix; layer, cur : int) : tuple<hb : array<float>; cb : array<float>> {
    let pd = fx.sh.pd
    let nl = RC_NL
    let nxt = 1 - cur
    var hb := fx.hb
    var cb := fx.cb
    let wih = layer * 8 * pd * pd
    let whh = wih + 4 * pd * pd
    let bo = fx.lb + layer * 4 * pd
    for (j in range(pd)) {
        var g : double[4]
        for (k in range(4)) {
            var s = 0.0lf
            for (q in range(pd)) {
                let x = layer == 0 ? fx.xin[q] : fx.hb[(nxt * nl + layer - 1) * pd + q]
                s += double(fx.whf[wih + (k * pd + j) * pd + q]) * double(x)
                s += double(fx.whf[whh + (k * pd + j) * pd + q]) * double(fx.hb[(cur * nl + layer) * pd + q])
            }
            g[k] = s + double(fx.wf[bo + k * pd + j])
        }
        let cn = sigmoid_d(g[1]) * double(fx.cb[layer * pd + j]) + sigmoid_d(g[0]) * tanh(g[3])
        cb[layer * pd + j] = float(cn)
        hb[(nxt * nl + layer) * pd + j] = float(sigmoid_d(g[2]) * tanh(cn))
    }
    return (hb <- hb, cb <- cb)
}

//! joint.pred over the last layer's new hidden at h parity `cur` (`GkRnntPred`, GATES off)
def rnnt_proj_ref(fx : RnntFix; cur : int) : array<float> {
    let pd = fx.sh.pd
    let nxt = 1 - cur
    let wjp = RC_NL * 8 * pd * pd
    return <- [for (i in range(fx.sh.jd)); float(fold_dot(fx, wjp + i * pd, (nxt * RC_NL + RC_NL - 1) * pd) + double(fx.wf[int(fx.pa.boff) + i]))]
}

def private fold_dot(fx : RnntFix; wr, hr : int) : double {
    var s = 0.0lf
    for (k in range(fx.sh.pd)) {
        s += double(fx.whf[wr + k]) * double(fx.hb[hr + k])
    }
    return s
}

//! the joint's window from frame `t0` (`GkRnntJoint`): the frames inside the clip's logit rows, `sh.nlog` live a row
def rnnt_joint_ref(fx : RnntFix; t0 : int; eb_on : bool) : array<float> {
    let jd = fx.sh.jd
    let nlog = fx.sh.nlog
    let nf = min(8, RC_FRAMES - t0)
    var out : array<float>
    out |> resize(nf * nlog)
    for (f in range(nf)) {
        for (i in range(nlog)) {
            var s = double(fx.wf[int(fx.pa.jnb) + i])
            for (j in range(jd)) {
                let e = fx.encp[(t0 + f) * jd + j] + (eb_on ? fx.wf[int(fx.pa.jeb) + j] : 0.0)
                let v = e + fx.pred[j]
                if (v > 0.0) {
                    s += double(v) * double(fx.jwf[((i / 256) * jd + j) * 256 + i % 256])
                }
            }
            out[f * nlog + i] = float(s)
        }
    }
    return <- out
}

//! The window's live logits off a device's rows: frame f's `sh.nlog` logits, the frames inside the clip.
def rnnt_joint_live(lg : array<float>; sh : RcShape; t0 : int) : array<float> {
    let nf = min(8, RC_FRAMES - t0)
    return <- [for (e in range(nf * sh.nlog)); lg[(e / sh.nlog) * sh.lstride + e % sh.nlog]]
}

//! The slots of the window's rows the joint writes no logit into - a frame past the clip, a logit past the row: the first one off
//! the sentinel, -1 where every one keeps it.
def rnnt_joint_kept_off(lg : array<float>; sh : RcShape; t0 : int) : int {
    let nf = min(8, RC_FRAMES - t0)
    for (f in range(8)) {
        for (i in range(sh.lstride)) {
            let at = f * sh.lstride + i
            return at if ((f >= nf || i >= sh.nlog) && float_bits_to_uint(lg[at]) != NAN_SENTINEL_BITS)
        }
    }
    return -1
}

//! What the pick leaves: the state words, the token records (the sentinel bits where none landed) and the predictor input.
struct PickRef {
    st : array<uint>
    toks : array<uint>
    xin : array<float>
    rec : int = -1      //! the record the token landed in, -1 for none
    emitted : bool      //! a token was emitted: its embedding row is the next input
}

//! the duration index as the CPU loop picks it: the argmax of the fp32 exp(logit - max), the first index of a tie
def private ref_dur(lg : array<float>; ntok, row : int; mx : float) : int {
    var b = 0
    var bp = exp(lg[row + ntok] - mx)
    for (d in range(1, RC_NDUR)) {
        let p = exp(lg[row + ntok + d] - mx)
        if (p > bp) {
            bp = p
            b = d
        }
    }
    return b
}

//! a row's token argmax, the first index of a tie (token 0 where no logit beats it: a row of NaN or -inf), its logit and the
//! row's maximum, durations included
def private ref_row(lg : array<float>; ntok, row, n_dur : int) : tuple<best : int; bv : float; mx : float> {
    var best = 0
    var bv = lg[row]
    for (i in range(1, ntok)) {
        if (lg[row + i] > bv) {
            bv = lg[row + i]
            best = i
        }
    }
    var mx = bv
    for (i in range(ntok, ntok + n_dur)) {
        mx = max(mx, lg[row + i])
    }
    return (best = best, bv = bv, mx = mx)
}

//! the token's record at slot `r.rec`: plog under the row's double log-sum, p within the token slice
def private ref_land(var r : PickRef; lg : array<float>; ntok, row, n_dur, best, bdur, dur, t : int; bv, mx : float) {
    var tm = 0.0lf
    var all = 0.0lf
    for (i in range(ntok + n_dur)) {
        let v = lg[row + i]
        if (v > mx - 32.0) {
            let e = exp(double(v - mx))
            all += e
            if (i < ntok) {
                tm += e
            }
        }
    }
    let plog = bv - (float(log(all)) + mx)
    let p = float(double(exp(plog)) / (tm / all))
    let w = r.rec * 8
    r.toks[w] = uint(best)
    r.toks[w + 1] = uint(bdur)
    r.toks[w + 2] = uint(dur)
    r.toks[w + 3] = uint(t)
    r.toks[w + 4] = float_bits_to_uint(p)
    r.toks[w + 5] = float_bits_to_uint(plog)
}

//! The greedy pick over the window `lg` from state `st0` (`GkRnntPick`), the CPU `rnnt_greedy` loop body: `n_dur` 0 reads as a
//! plain RNN-T. The records' p and plog off a double log-sum; the new token's embedding row the next input.
def rnnt_pick_ref(lg : array<float>; st0 : array<uint>; fx : RnntFix; pa : GkRnntArgs) : PickRef {
    let sh = fx.sh
    var r = PickRef(st := st0)
    r.xin <- [for (_i in range(sh.pd)); NAN_SENTINEL]
    r.toks <- [for (_i in range(int(pa.cap) * 8)); NAN_SENTINEL_BITS]
    return <- r if (st0[2] != 0u)
    let n_dur = int(pa.n_dur)
    let t0 = int(st0[0])
    var t = t0
    var emitted = int(st0[1])
    var ntok = int(st0[5])
    var need = 0
    var done = 0
    while (true) {
        if (t >= int(pa.n_frames)) {
            done = 1
            break
        }
        break if (t - t0 >= 8)
        let row = (t - t0) * sh.lstride
        let rp = ref_row(lg, sh.ntok, row, n_dur)
        let bdur = n_dur > 0 ? ref_dur(lg, sh.ntok, row, rp.mx) : 0
        let dur = n_dur > 0 ? RC_DURS[bdur] : 0
        if (rp.best == int(pa.blank)) {
            t += max(dur, 1)
            emitted = 0
            continue
        }
        r.emitted = true
        if (ntok < int(pa.cap)) {
            r.rec = ntok
            ref_land(r, lg, sh.ntok, row, n_dur, rp.best, bdur, dur, t, rp.bv, rp.mx)
        }
        let best = rp.best
        ntok++
        if (dur > 0) {
            t += dur
            emitted = 0
        } else {
            emitted++
            if (emitted >= int(pa.max_sym)) {
                t++
                emitted = 0
            }
        }
        need = 1
        if (t >= int(pa.n_frames)) {
            done = 1
        }
        for (k in range(sh.pd)) {
            r.xin[k] = fx.wf[int(pa.embed) + best * sh.pd + k]
        }
        break
    }
    r.st[0] = uint(t)
    r.st[1] = uint(emitted)
    r.st[2] = uint(done)
    r.st[3] = uint(need)
    if (st0[3] != 0u) {
        r.st[4] = 1u - st0[4]
        r.st[6] = st0[6] + 1u
    }
    r.st[5] = uint(ntok)
    r.st[7] = st0[7] + 1u
    return <- r
}

//! A window of logits a pick case reads: every row's logits small and spread, the cases' winners laid over them by `put`.
def rnnt_pick_rows(sh : RcShape) : array<float> {
    var lg : array<float>
    lg |> resize(8 * sh.lstride)
    for (e in range(8 * sh.lstride)) {
        lg[e] = rc_w(e, 60, 2.0)
    }
    return <- lg
}

//! the hidden pair as a gates leg uploads it: the half the layer writes under the sentinel where the leg runs (`active`)
def rnnt_gates_input(fx : RnntFix; layer, cur : int; active : bool) : array<float> {
    var hb := fx.hb
    if (active) {
        let base = ((1 - cur) * RC_NL + layer) * fx.sh.pd
        for (j in range(fx.sh.pd)) {
            hb[base + j] = NAN_SENTINEL
        }
    }
    return <- hb
}

//! The gates' and the projection's bar: the halves hold the weights exactly, so what the device adds is its f32 dot - at most
//! 2 pd = 128 products a gate, whose rounding walk sits under 1e-6 relative - and its sigmoid / tanh, a few ulp; 1e-5 relative
//! (floored at 1e-6) holds both with margin and is two orders under the 1e-3 a one-weight change moves an element.
let RC_PRED_BAR = Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true)

//! A gates leg's hidden pair and cells against the reference where it runs (the new hidden off its sentinel, the cells moved
//! off their input), or bit for bit its input where the state skips it
def rnnt_gates_verify(t : T?; tag : string; hb, cb : array<float>; fx : RnntFix; layer, cur : int; active : bool) {
    let pd = fx.sh.pd
    if (active) {
        let want = rnnt_gates_ref(fx, layer, cur)
        check(t, "{tag}: the hidden pair", hb, want.hb, RC_PRED_BAR, Shape(row_width = pd))
        check(t, "{tag}: the cells", cb, want.cb, RC_PRED_BAR, Shape(row_width = pd))
        t |> success(moved_off(cb, fx.cb, layer * pd + 5, RC_PRED_BAR), "{tag}: the cells moved off their input")
    } else {
        check(t, "{tag}: the hidden pair as it was", hb, fx.hb, BAR_EXACT, Shape(row_width = pd))
        check(t, "{tag}: the cells as they were", cb, fx.cb, BAR_EXACT, Shape(row_width = pd))
    }
}

//! A projection leg's output against the reference where it runs (`RC_PRED_BAR`), the sentinel kept where it is skipped
def rnnt_proj_verify(t : T?; tag : string; pred : array<float>; fx : RnntFix; cur : int; active : bool) {
    if (active) {
        check(t, "{tag}: joint.pred", pred, rnnt_proj_ref(fx, cur), RC_PRED_BAR, FLAT_SHAPE)
    } else {
        t |> equal(sentinel_off(pred), 0, "{tag}: skipped, the output keeps the sentinel (first off at {first_sentinel_off(pred)})")
    }
}

//! A joint leg's window against the reference where it runs, the slots it writes no logit into kept; every slot kept where the
//! clip is done. The bar: the logits are an f32 sum of jd <= 68 halves-exact products plus the bias, a rounding walk under 1e-6
//! of the row's magnitude; 1e-5 relative, floored at 1e-5 for the near-zero logits a ReLU-cut sum lands on, holds it.
def rnnt_joint_verify(t : T?; tag : string; lg : array<float>; fx : RnntFix; t0 : int; eb_on, active : bool) {
    if (active) {
        check(t, "{tag}: the window's logits", rnnt_joint_live(lg, fx.sh, t0), rnnt_joint_ref(fx, t0, eb_on), Bar(rel = 1e-5, abs_floor = 1e-5, poison_scaled = true),
            Shape(row_width = fx.sh.nlog))
        let kept_off = rnnt_joint_kept_off(lg, fx.sh, t0)
        t |> equal(kept_off, -1, "{tag}: a frame past the clip and a logit past the row keep the sentinel (first off at {kept_off})")
    } else {
        t |> equal(sentinel_off(lg), 0, "{tag}: done, the window keeps the sentinel (first off at {first_sentinel_off(lg)})")
    }
}

//! One pick case: the window's rows, the state the pick starts from and the kargs it reads.
struct PickCase {
    tag : string
    lg : array<float>
    st0 : array<uint>
    pa : GkRnntArgs
    dur_want : int = -1   //! the duration index the case's record must carry, -1 for any: the fixture's own witness
    no_winner : bool      //! no token logit is finite: the record's p and plog carry no value to compare, its id must be token 0
}

def private pick_case(tag : string; var lg : array<float>; st0 : array<uint>; pa : GkRnntArgs; dur_want : int = -1; no_winner : bool = false) : PickCase
    => <- PickCase(tag = tag, lg <- lg, st0 := st0, pa = pa, dur_want = dur_want, no_winner = no_winner)

//! The pick's paths: a TDT blank skipping two frames then a token staying on its frame (a duration of 0); a plain RNN-T window of
//! blanks spent with the predictor standing; a token at the symbol cap moving the frame onto the clip's end; durations whose
//! exp lands in the subnormal range; a full record table counting on; a clip already done; ties; a frame whose token logits are
//! all NaN, and one whose are all -inf - no logit beats the argmax's start, which must still name a token of the table.
def rnnt_pick_cases(fx : RnntFix) : array<PickCase> {
    let sh = fx.sh
    let ntok = sh.ntok
    let blank = ntok - 1
    var cases : array<PickCase>
    cases |> reserve(10)
    let pa = fx.pa
    var lg <- rnnt_pick_rows(sh)
    lg[blank] = 10.0
    lg[ntok] = 1.0
    lg[ntok + 1] = 3.0
    lg[ntok + 2] = 9.0
    lg[2 * sh.lstride + 7] = 10.0
    lg[2 * sh.lstride + ntok] = 9.0
    cases |> emplace(pick_case("tdt blank, token", lg, rnnt_state(0, 0, 0, 1, 0, 0), pa))
    var plain = pa
    plain.n_dur = 0u
    var lgb <- rnnt_pick_rows(sh)
    for (f in range(8)) {
        lgb[f * sh.lstride + blank] = 10.0
    }
    cases |> emplace(pick_case("plain window spent", lgb, rnnt_state(3, 1, 0, 0, 1, 4), plain))
    var capped = plain
    capped.n_frames = 5u
    var lgc <- rnnt_pick_rows(sh)
    lgc[5] = 10.0
    cases |> emplace(pick_case("symbol cap at the clip's end", lgc, rnnt_state(4, 1, 0, 1, 1, 2), capped))
    var lgs <- rnnt_pick_rows(sh)
    lgs[3] = 50.0
    lgs[ntok] = 50.0 - 96.0
    lgs[ntok + 1] = 50.0 - 95.0
    lgs[ntok + 2] = 50.0 - 200.0
    cases |> emplace(pick_case("subnormal durations", lgs, rnnt_state(0, 0, 0, 0, 0, 0), pa, 1))   //! e^-95 above e^-96, both under the smallest normal
    var full = pa
    full.cap = 2u
    var lgf <- rnnt_pick_rows(sh)
    lgf[11] = 10.0
    lgf[ntok + 1] = 9.0
    cases |> emplace(pick_case("record table full", lgf, rnnt_state(0, 0, 0, 1, 0, 2), full))
    var lgd <- rnnt_pick_rows(sh)
    cases |> emplace(pick_case("clip done", lgd, rnnt_state(7, 0, 1, 1, 0, 3), pa))
    var lgt <- rnnt_pick_rows(sh)
    lgt[9] = 10.0
    lgt[2] = 10.0
    cases |> emplace(pick_case("a tie inside a subgroup keeps the lower token", lgt, rnnt_state(0, 0, 0, 0, 0, 0), plain))
    var lgx <- rnnt_pick_rows(sh)
    lgx[min(200, ntok - 10)] = 10.0
    lgx[40] = 10.0
    cases |> emplace(pick_case("a tie across subgroups keeps the lower token", lgx, rnnt_state(0, 0, 0, 0, 0, 0), plain))
    for (bits in fixed_array(0x7fc00000u, 0xff800000u)) {
        var lgn <- rnnt_pick_rows(sh)
        for (i in range(ntok)) {
            lgn[i] = uint_bits_to_float(bits)
        }
        let what = bits == 0x7fc00000u ? "NaN" : "-inf"
        cases |> emplace(pick_case("a frame of {what} token logits picks token 0", lgn, rnnt_state(0, 0, 0, 0, 0, 0), plain, -1, true))
    }
    return <- cases
}

//! A pick's outputs against its reference: the state words exact, the landed records' words exact and their p / plog within
//! 1e-5, the record slots past them still the sentinel, the predictor input the token's embedding row bit for bit or, where no
//! token landed, still the sentinel. The p / plog bar: a float log-sum over the row's `n_logits` exps against the double one,
//! whose rounding walk lands near 1e-6 of a plog of order 1; 1e-5 relative, floored at 1e-6, holds it.
def rnnt_pick_verify(t : T?; tag : string; c : PickCase; st, toks : array<uint>; xin : array<float>; want : PickRef) {
    if (c.dur_want >= 0) {
        t |> success(want.rec >= 0 && int(want.toks[want.rec * 8 + 1]) == c.dur_want, "{tag}: the reference's record carries duration index {c.dur_want}")
    }
    check_exact(t, "{tag}: the state words", st, want.st)
    var off = 0
    var first = -1
    for (i in range(length(toks))) {
        if ((want.rec < 0 || i / 8 != want.rec || i % 8 >= 6) && toks[i] != NAN_SENTINEL_BITS) {
            first = off == 0 ? i : first
            off++
        }
    }
    t |> equal(off, 0, "{tag}: every record slot but the landed record's six words keeps the sentinel (first off at {first})")
    if (want.rec >= 0) {
        let r = want.rec * 8
        check_exact(t, "{tag}: the record's id, duration index, duration and frame", copy_span(toks, r, 4), copy_span(want.toks, r, 4))
        if (c.no_winner) {
            t |> equal(toks[r], 0u, "{tag}: the record names token 0, inside the table of {int(c.pa.n_tok)}")
        } else {
            let gp <- [for (k in range(4, 6)); uint_bits_to_float(toks[r + k])]
            let wp <- [for (k in range(4, 6)); uint_bits_to_float(want.toks[r + k])]
            check(t, "{tag}: the record's p and plog", gp, wp, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), FLAT_SHAPE)
        }
    }
    if (want.emitted) {
        check(t, "{tag}: the next input is the token's embedding row", xin, want.xin, BAR_EXACT, FLAT_SHAPE)
    } else {
        t |> equal(sentinel_off(xin), 0, "{tag}: no token, the predictor input keeps the sentinel (first off at {first_sentinel_off(xin)})")
    }
}
