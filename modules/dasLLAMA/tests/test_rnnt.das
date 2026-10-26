options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_rnnt
require dasllama/dasllama_plane
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_par    // set_single_thread_ - the decode step's one-lane leg
require _rnnt_oracle   // rnnt_seat_*: the build's device transducer seat, its pin and counters
require daslib/jobque_boost
require math
require strings
require _compares   // nan_wide_diff: a NaN plog reads as outside the bar; sigmoid_d

// The shared transducer decoder (`rnnt_greedy`) over synthetic nets of tiny widths, built in-test, against a scalar
// reference written here: an LSTM stack in the torch gate order or the i, f, o, g one, a ReLU joint, the greedy walk
// with its duration table and its per-frame symbol cap. The weights are multiples of 1/256, so the decoder's half
// copies of them are exact; ids, durations and frames match the reference exactly, log-probabilities within PLOG_BAR.
// Model-free. The device-seat cells (`device_leg`, the zero-frame, decline and shared-weight cells) skip through
// `rnnt_seat_skip`: on a build with neither das_metal nor the Vulkan module, on a Metal build whose box has no Metal
// device, and on a Vulkan build where the tier arms no device (`DASLLAMA_GPU=1` unset, or no device).

let private PLOG_BAR = 1e-5   //!< the decoder sits ~2e-7 off the reference; one joint row mutated moves it past the bar (`test_rnnt_plog_bar_control`)

//! One synthetic net: the shapes and the arms `rnnt_greedy` takes, the joint biases that steer what it emits.
struct private NetSpec {
    nl : int64 = 1l
    pd : int64 = 8l
    jd : int64 = 8l
    n_tokens : int64 = 6l         //!< the vocabulary with its blank, the last id
    n_dur : int = 0               //!< a prefix of DURATIONS; 0 = a plain RNN-T
    max_symbols : int = 3
    zero_start : bool = true
    torch_gates : bool = true
    two_biases : bool = true
    blank_bias : float = 0.0      //!< added to the blank's joint bias
    tok0_bias : float = 0.0       //!< added to token 0's joint bias
    n_frames : int64 = 12l
    seed : uint = 1u
    d_enc : int64 = 0l            //!< the encoder width of the joint's encoder half, laid in the plane; 0 = the projection alone
}

let private DURATIONS <- [0, 1, 2, 4]

//! A net's weight plane and encoder rows with the `RnntNet` laid over them; the planes point into `data` and `durs`.
struct private Fixture {
    data : array<float>
    enc : array<float>
    durs : array<int>
    net : RnntNet = RnntNet()
    x : array<float>      //!< the encoder rows [n_frames][d_enc], where the spec lays the encoder half
    enc_half : RnntEnc    //!< the encoder half's offsets in the plane: `enc` is its projection of `x`, bias included
}

def private next_weight(var seed : uint&) : float {
    seed = seed * 1664525u + 1013904223u
    return float(int((seed >> 16u) & 255u) - 128) / 256.0
}

def private take(var cursor : int64&; n : int64) : int64 {
    let at = cursor
    cursor += n
    return at
}

def private build(spec : NetSpec) : Fixture {
    var f = Fixture()
    let pd = spec.pd
    let jd = spec.jd
    let n_logits = spec.n_tokens + int64(spec.n_dur)
    var net = RnntNet(n_layers = spec.nl, pred_dim = pd, joint_dim = jd, n_tokens = spec.n_tokens,
        blank_id = int(spec.n_tokens) - 1, max_symbols = spec.max_symbols, zero_start = spec.zero_start,
        torch_gates = spec.torch_gates, two_biases = spec.two_biases)
    var at = 0l
    net.embed = take(at, spec.n_tokens * pd)
    for (l in range64(min(spec.nl, 2l))) {
        net.lstm_ih[l] = take(at, 4l * pd * pd)
        net.lstm_hh[l] = take(at, 4l * pd * pd)
        net.lstm_b[l] = take(at, 4l * pd)
        net.lstm_b2[l] = take(at, 4l * pd)
    }
    net.joint_pred_w = take(at, jd * pd)
    net.joint_pred_b = take(at, jd)
    net.joint_net_w = take(at, n_logits * jd)
    net.joint_net_b = take(at, n_logits)
    let de = spec.d_enc
    let ew = de > 0l ? take(at, jd * de) : -1l
    let eb = de > 0l ? take(at, jd) : -1l
    var seed = spec.seed
    f.data |> resize(at)
    for (x in f.data) {
        x = next_weight(seed)
    }
    f.data[net.joint_net_b + int64(net.blank_id)] += spec.blank_bias
    f.data[net.joint_net_b] += spec.tok0_bias
    f.enc |> resize(spec.n_frames * jd)
    if (de > 0l) {   //! the rows and weights on the 1/256 lattice: the projection's every sum is exact in f32, in any order
        f.x |> resize(spec.n_frames * de)
        for (x in f.x) {
            x = next_weight(seed) * 2.0
        }
        for (t in range64(spec.n_frames)) {
            for (j in range64(jd)) {
                var s = f.data[eb + j]
                for (k in range64(de)) {
                    s += f.data[ew + j * de + k] * f.x[t * de + k]
                }
                f.enc[t * jd + j] = s
            }
        }
    } else {
        for (x in f.enc) {
            x = next_weight(seed) * 2.0
        }
    }
    f.enc_half = RnntEnc(d = de, w = ew, b = eb, q8 = de == 0l)
    f.durs <- [for (i in range(spec.n_dur)); DURATIONS[i]]
    unsafe {
        net.blob = PlaneF(p = addr<float const?>(f.data[0]), n = at)
        if (spec.n_dur > 0) {
            net.durations = PlaneI32(p = addr<int const?>(f.durs[0]), n = int64(spec.n_dur))
        }
    }
    f.net = net
    return <- f
}

//! The reference decoder's state: the LSTM's h/c a layer, the joint's prediction and its logit row.
struct private RefState {
    h : array<double>
    c : array<double>
    pred : array<double>
    logits : array<double>
}

//! The predictor over input `x` ([pd]): each layer's four gates (rows i, f, g, o under torch, else i, f, o, g) from
//! ih . x + hh . h + b (+ b2), then c and h, the layer's h the next layer's input; joint.pred over the last h.
def private ref_predict(f : Fixture; var r : RefState; x0 : array<double>) {
    let net = f.net
    let pd = net.pred_dim
    var x := x0
    let gi = 0l
    let gf = 1l
    let gg = net.torch_gates ? 2l : 3l
    let go = net.torch_gates ? 3l : 2l
    for (l in range64(net.n_layers)) {
        var inscope pre : array<double>
        pre |> resize(4l * pd)
        for (row in range64(4l * pd)) {
            var a = double(f.data[net.lstm_b[l] + row])
            if (net.two_biases) {
                a += double(f.data[net.lstm_b2[l] + row])
            }
            for (j in range64(pd)) {
                a += double(f.data[net.lstm_ih[l] + row * pd + j]) * x[j]
                a += double(f.data[net.lstm_hh[l] + row * pd + j]) * r.h[l * pd + j]
            }
            pre[row] = a
        }
        for (j in range64(pd)) {
            let ig = sigmoid_d(pre[gi * pd + j])
            let fg = sigmoid_d(pre[gf * pd + j])
            let cg = tanh(pre[gg * pd + j])
            let og = sigmoid_d(pre[go * pd + j])
            let c = fg * r.c[l * pd + j] + ig * cg
            r.c[l * pd + j] = c
            r.h[l * pd + j] = og * tanh(c)
            x[j] = r.h[l * pd + j]
        }
    }
    for (i in range64(net.joint_dim)) {
        var a = double(f.data[net.joint_pred_b + i])
        for (j in range64(pd)) {
            a += double(f.data[net.joint_pred_w + i * pd + j]) * x[j]
        }
        r.pred[i] = a
    }
}

//! The joint's logits at frame `t`: b + W . relu(enc[t] + pred).
def private ref_joint(f : Fixture; var r : RefState; t : int64) {
    let net = f.net
    let jd = net.joint_dim
    let n_logits = net.n_tokens + long_length(f.durs)
    for (i in range64(n_logits)) {
        var a = double(f.data[net.joint_net_b + i])
        for (j in range64(jd)) {
            let v = double(f.enc[t * jd + j]) + r.pred[j]
            if (v > 0.0lf) {
                a += double(f.data[net.joint_net_w + i * jd + j]) * v
            }
        }
        r.logits[i] = a
    }
}

def private ref_step(f : Fixture; var r : RefState; token : int; zero : bool; t : int64) {
    var x : array<double>
    x |> resize(f.net.pred_dim)
    if (!zero) {
        for (j in range64(f.net.pred_dim)) {
            x[j] = double(f.data[f.net.embed + int64(token) * f.net.pred_dim + j])
        }
    }
    ref_predict(f, r, x)
    ref_joint(f, r, t)
}

//! The greedy transducer walk, written out: the token argmax and the duration argmax a row, a blank advancing by its
//! duration (at least a frame), a token by its own, a duration-0 token staying on the frame up to `max_symbols`.
def private ref_greedy(f : Fixture) : array<RnntToken> {
    let net = f.net
    let n_tok = int(net.n_tokens)
    let n_dur = length(f.durs)
    let n_frames = long_length(f.enc) / net.joint_dim
    var r = RefState()
    r.h |> resize(net.n_layers * net.pred_dim)
    r.c |> resize(net.n_layers * net.pred_dim)
    r.pred |> resize(net.joint_dim)
    r.logits |> resize(n_tok + n_dur)
    var toks : array<RnntToken>
    if (n_frames > 0l) {
        ref_step(f, r, net.blank_id, net.zero_start, 0l)
    }
    var t = 0l
    var emitted = 0
    while (t < n_frames) {
        var best = 0
        for (i in range(1, n_tok)) {
            if (r.logits[i] > r.logits[best]) {
                best = i
            }
        }
        var bdur = 0
        for (i in range(1, n_dur)) {
            if (r.logits[n_tok + i] > r.logits[n_tok + bdur]) {
                bdur = i
            }
        }
        let dur = n_dur == 0 ? 0 : f.durs[bdur]
        if (best == net.blank_id) {
            t += int64(max(dur, 1))
            emitted = 0
            if (t < n_frames) {
                ref_joint(f, r, t)
            }
            continue
        }
        var mx = r.logits[0]
        for (v in r.logits) {
            mx = max(mx, v)
        }
        var lse = 0.0lf
        for (v in r.logits) {
            if (v > mx - 32.0lf) {
                lse += exp(v - mx)
            }
        }
        toks |> push(RnntToken(id = best, dur_idx = bdur, dur = dur, frame = t, plog = float(r.logits[best] - mx - log(lse))))
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
            ref_step(f, r, best, false, t)
        }
    }
    return <- toks
}

def private engine_greedy(f : Fixture) : array<RnntToken> {
    var s = RnntState()
    var toks : array<RnntToken>
    rnnt_greedy(f.net, s, f.enc, long_length(f.enc) / f.net.joint_dim, toks)
    return <- toks
}

//! The decoder's tokens and the reference's of one net, plus the dispatch lanes the decode ran on.
struct private Leg {
    got : array<RnntToken>
    want : array<RnntToken>
    lanes : int
}

[cold_path]
def private run_leg(spec : NetSpec) : Leg {
    var f <- build(spec)
    var leg = Leg(lanes = get_dispatch_lanes())
    leg.got <- engine_greedy(f)
    leg.want <- ref_greedy(f)
    unsafe {
        delete f   // a struct holding planes deletes unsafe: the planes point into its own arrays
    }
    return <- leg
}

def private token_list(toks : array<RnntToken>) : string {
    return build_string() $(var w) {
        for (x in toks) {
            w |> write("{x.id}@{x.frame}+{x.dur} ")
        }
    }
}

//! The decoder's tokens are the reference's: the same ids, frames and durations, the log-probabilities within PLOG_BAR.
def private same_tokens(t : T?; tag : string; leg : Leg) {
    t |> equal(token_list(leg.got), token_list(leg.want), "{tag}: id@frame+duration")
    rnnt_tokens_match(t, "test_rnnt: {tag}", leg.got, leg.want, PLOG_BAR)
    t |> success(!empty(leg.want), "{tag}: the net emits tokens")
}

[test]
def test_rnnt_zero_start(t : T?) {
    t |> run("zero_start reads a zero first input, the other arm the blank's (non-zero) embedding row") @(t : T?) {
        with_dasllama_jobque_() {
            var inscope zero <- run_leg(NetSpec(zero_start = true))
            var inscope blank <- run_leg(NetSpec(zero_start = false))
            same_tokens(t, "zero_start", zero)
            same_tokens(t, "blank start", blank)
            if (token_list(zero.want) == token_list(blank.want)) {
                check_exceeds(t, "the two first inputs decode apart - the arms are distinguishable", plogs_of(zero.want), plogs_of(blank.want),
                    Bar(abs_floor = 10.0 * PLOG_BAR))
            } else {
                to_log(LOG_INFO, "test_rnnt: the two first inputs decode apart: {token_list(zero.want)} / {token_list(blank.want)}\n")
            }
        }
    }
}

[test]
def test_rnnt_plog_bar_control(t : T?) {
    t |> run("PLOG_BAR's control: the decoder over a net with one joint row mutated lands outside the bar of the reference") @(t : T?) {
        with_dasllama_jobque_() {
            let spec = NetSpec(seed = 9u, n_frames = 16l)
            var f <- build(spec)
            var inscope want <- ref_greedy(f)
            let row = f.net.joint_net_w   //! token 0's joint row
            for (j in range64(f.net.joint_dim)) {
                f.data[row + j] += 0.125
            }
            var inscope got <- engine_greedy(f)
            unsafe {
                delete f
            }
            to_log(LOG_INFO, "test_rnnt: the mutated net {token_list(got)} / the reference {token_list(want)}\n")
            check_exceeds(t, "a joint row mutated by 0.125", plogs_of(got), plogs_of(want), Bar(abs_floor = PLOG_BAR))
        }
    }
}

[test]
def test_rnnt_symbol_cap(t : T?) {
    t |> run("a joint that keeps emitting stops at max_symbols a frame") @(t : T?) {
        with_dasllama_jobque_() {
            var inscope leg <- run_leg(NetSpec(tok0_bias = 40.0, max_symbols = 3, n_frames = 4l))
            same_tokens(t, "capped", leg)
            t |> equal(length(leg.got), 12, "three tokens on each of four frames")
            for (x, i in leg.got, count()) {
                t |> equal(x.frame, int64(i / 3), "token {i}'s frame")
            }
        }
    }
}

[test]
def test_rnnt_shapes(t : T?) {
    t |> run("joint_dim != pred_dim") @(t : T?) {
        with_dasllama_jobque_() {
            var inscope leg <- run_leg(NetSpec(pd = 8l, jd = 12l, seed = 7u))
            same_tokens(t, "jd 12, pd 8", leg)
        }
    }
    t |> run("two layers, torch gates and two biases") @(t : T?) {
        with_dasllama_jobque_() {
            var inscope leg <- run_leg(NetSpec(nl = 2l, torch_gates = true, two_biases = true, seed = 3u))
            same_tokens(t, "nl 2 torch", leg)
        }
    }
    t |> run("two layers, i f o g gates and one bias") @(t : T?) {
        with_dasllama_jobque_() {
            var inscope leg <- run_leg(NetSpec(nl = 2l, torch_gates = false, two_biases = false, seed = 5u))
            same_tokens(t, "nl 2 ifog", leg)
        }
    }
    t |> run("a predictor width off the float4 lattice takes the scalar gate tail") @(t : T?) {
        with_dasllama_jobque_() {
            var inscope leg <- run_leg(NetSpec(pd = 6l, jd = 8l, seed = 11u))
            same_tokens(t, "pd 6", leg)
        }
    }
}

[test]
def test_rnnt_durations(t : T?) {
    t |> run("a TDT duration table moves the frame by the picked duration; an empty one is a plain RNN-T") @(t : T?) {
        with_dasllama_jobque_() {
            var inscope tdt <- run_leg(NetSpec(n_dur = 4, seed = 9u, n_frames = 16l, blank_bias = -1.0))
            same_tokens(t, "durations 0 1 2 4", tdt)
            var moved = false
            for (x in tdt.got) {
                moved ||= x.dur > 0
            }
            t |> success(moved, "a token takes a non-zero duration: {token_list(tdt.got)}")
            var inscope plain <- run_leg(NetSpec(n_dur = 0, seed = 9u, n_frames = 16l))
            same_tokens(t, "no durations", plain)
            for (x in plain.got) {
                t |> equal(x.dur, 0, "a plain RNN-T token's duration")
            }
        }
    }
}

[test]
def test_rnnt_one_lane(t : T?) {
    t |> run("the one-lane step decodes what the team publish does") @(t : T?) {
        with_dasllama_jobque_() {
            let was_single = get_single_thread_()
            set_single_thread_(false)
            var inscope team <- run_leg(NetSpec(nl = 2l, pd = 6l, jd = 12l, seed = 13u))
            set_single_thread_(true)
            var inscope one <- run_leg(NetSpec(nl = 2l, pd = 6l, jd = 12l, seed = 13u))
            set_single_thread_(was_single)
            same_tokens(t, "team", team)
            same_tokens(t, "one lane", one)
            t |> success(team.lanes > 1, "the team decode ran on {team.lanes} lanes")
            t |> equal(one.lanes, 1, "the one-lane decode's lanes")
        }
    }
}

[cold_path]
def private layers_refusal(nl : int64) : string {
    var f <- build(NetSpec(nl = nl))
    let why = panic_text() ${
        var inscope toks <- engine_greedy(f)
    }
    unsafe {
        delete f
    }
    return why
}

[test]
def test_rnnt_layer_count(t : T?) {
    t |> run("a predictor of 0 or 3 layers is refused at reset") @(t : T?) {
        for (nl in [0l, 3l]) {
            let why = layers_refusal(nl)
            t |> success(find(why, "{nl} predictor layers; the decode step takes 1 to 2") >= 0, "{nl} layers: {why}")
        }
    }
}

[test]
def test_rnnt_step_lanes(t : T?) {
    t |> run("a decode narrows the pool to its widest stage's work and puts it back") @(t : T?) {
        with_dasllama_jobque_() {
            let lanes = get_dispatch_lanes()
            var f <- build(NetSpec())
            t |> equal(rnnt_step_lanes(f.net), 1, "pd 8: one lane, the step's work is under a lane's share")
            var wide = f.net
            wide.pred_dim = 320l
            wide.joint_dim = 320l
            t |> equal(rnnt_step_lanes(wide), min(6, lanes), "pd 320: GigaAM's predictor takes six lanes")
            wide.pred_dim = 640l
            t |> equal(rnnt_step_lanes(wide), min(25, lanes), "pd 640: parakeet's takes the pool")
            unsafe {
                delete f
            }
            let before = get_jobque_worker_limit()
            var inscope leg <- run_leg(NetSpec(seed = 17u))
            same_tokens(t, "the narrowed decode", leg)
            t |> equal(get_jobque_worker_limit(), before, "the worker limit is back where the decode found it")
            let prev = dispatch_lanes_narrow(1)
            let narrowed = get_jobque_worker_limit()
            dispatch_lanes_restore(prev)
            t |> success(prev < 0 || narrowed == 0, "one lane: every worker dormant ({narrowed}, the pool's was {prev})")
            t |> equal(get_jobque_worker_limit(), before, "restored")
        }
    }
}

//! `f`'s net decoded through `rnnt_decode` on the build's device seat with the state `s`: the tokens, and the seat's counters moved
//! - `why` the declines of that reason, the knob's included.
struct private SeatRun {
    toks : array<RnntToken>
    decodes : int64
    projects : int64
    uploads : int64
    declines : int64
    why : int64
}

def private seat_decode(f : Fixture; var s : RnntState; why : string) : SeatRun {
    let s0 = rnnt_seat_stats()
    let w0 = rnnt_seat_declines(why)
    var encp : array<float>
    var r : SeatRun
    rnnt_decode(f.net, s, f.enc_half, f.x, encp, long_length(f.enc) / f.net.joint_dim, r.toks) $(var e) {
        e := f.enc
    }
    delete encp
    let s1 = rnnt_seat_stats()
    r.decodes = s1.decodes - s0.decodes
    r.projects = s1.projects - s0.projects
    r.uploads = s1.uploads - s0.uploads
    r.declines = s1.declines - s0.declines
    r.why = rnnt_seat_declines(why) - w0
    return <- r
}

//! `spec`'s net decoded on the seat pinned `on` with a fresh state, against the reference
def private seat_leg(spec : NetSpec; on : bool; why : string) : tuple<leg : Leg; run : SeatRun> {
    var f <- build(spec)
    let was = rnnt_seat_pin(on)
    var s = RnntState()
    var run <- seat_decode(f, s, why)
    rnnt_seat_pin(was)
    var leg = Leg(lanes = 1)
    leg.got := run.toks
    leg.want <- ref_greedy(f)
    delete s
    unsafe {
        delete f
    }
    return (leg <- leg, run <- run)
}

//! The decode on the build's device seat: the reference's tokens, the decode counted on the device and, where the net lays its
//! encoder half, projected there.
[cold_path]
def private device_leg(t : T?; spec : NetSpec; tag : string) {
    return if (rnnt_seat_skip(t, tag))
    var inscope r <- seat_leg(spec, true, "")
    same_tokens(t, "{tag} on the {rnnt_seat_name()} seat", r.leg)
    t |> equal(r.run.decodes, 1l, "{tag}: the decode served on the device")
    t |> equal(r.run.projects, spec.d_enc > 0l ? 1l : 0l, "{tag}: the encoder half projected on the device where the net lays it")
    t |> equal(r.run.declines, 0l, "{tag}: no decline")
}

[test]
def test_rnnt_device_twin(t : T?) {
    t |> run("the device seat decodes the synthetic nets as the reference does") @(t : T?) {
        with_dasllama_jobque_() {
            device_leg(t, NetSpec(), "one layer, torch gates, zero start")
            device_leg(t, NetSpec(zero_start = false), "blank start")
            device_leg(t, NetSpec(tok0_bias = 40.0, max_symbols = 3, n_frames = 4l), "symbol cap")
            device_leg(t, NetSpec(nl = 2l, torch_gates = false, two_biases = false, seed = 5u), "two layers, i f o g")
            device_leg(t, NetSpec(n_dur = 4, seed = 9u, n_frames = 16l, blank_bias = -1.0), "durations 0 1 2 4")
            device_leg(t, NetSpec(pd = 6l, jd = 64l, d_enc = 32l, n_frames = 30l, seed = 21u), "the encoder half projected on the device")
            device_leg(t, NetSpec(n_dur = 4, jd = 64l, d_enc = 32l, n_frames = 40l, seed = 23u, blank_bias = 1.0), "durations over a projected half, past a window")
        }
    }
}

[test]
def test_rnnt_device_zero_frames(t : T?) {
    t |> run("a clip of no frames serves as no tokens") @(t : T?) {
        return if (rnnt_seat_skip(t, "zero frames"))
        with_dasllama_jobque_() {
            var inscope r <- seat_leg(NetSpec(n_frames = 0l, seed = 29u), true, "")
            t |> equal(length(r.run.toks), 0, "no tokens")
            t |> equal(length(r.leg.want), 0, "the reference's none")
            t |> equal(r.run.decodes, 1l, "served on the device")
            t |> equal(r.run.declines, 0l, "no decline")
        }
    }
}

//! A decline case: the net, the seat pinned on or off, the failure and the round budget the seat takes, the reason it counts.
struct private DeclineCase {
    tag : string
    spec : NetSpec = NetSpec()
    why : string
    on : bool = true
    fault : RnntDevFault = RnntDevFault.none
    budget : int64 = 0l
}

//! every decline the build's seat counts, each run on its own: the CPU loop decodes the reference's tokens, the device counts
//! no decode and the one decline of its reason
def private decline_cases : array<DeclineCase> {
    var cases <- [
        DeclineCase(tag = "the knob off", spec = NetSpec(seed = 31u), why = "knob", on = false),
        DeclineCase(tag = "a joint past the staged hidden rows", spec = NetSpec(jd = 776l, seed = 33u), why = "shape"),
        DeclineCase(tag = "a device failure", spec = NetSpec(seed = 35u), why = "device", fault = RnntDevFault.device),
        DeclineCase(tag = "the round budget spent", spec = NetSpec(tok0_bias = 40.0, max_symbols = 1, n_frames = 4l, seed = 37u), why = "overrun", budget = 1l)]
    if (rnnt_seat_name() == "metal") {
        cases |> push(DeclineCase(tag = "a failed command buffer", spec = NetSpec(seed = 39u), why = "gpu_error", fault = RnntDevFault.gpu_error))
    } else {
        cases |> push(DeclineCase(tag = "a failed allocation", spec = NetSpec(seed = 39u), why = "memory", fault = RnntDevFault.memory))
    }
    return <- cases
}

[test]
def test_rnnt_device_declines(t : T?) {
    t |> run("each seat decline falls back to the CPU loop and counts its reason") @(t : T?) {
        return if (rnnt_seat_skip(t, "declines"))
        with_dasllama_jobque_() {
            for (c in decline_cases()) {
                let fault_was = set_rnnt_dev_fault(c.fault)
                let budget_was = set_rnnt_dev_round_budget(c.budget)
                var inscope r <- seat_leg(c.spec, c.on, c.why)
                set_rnnt_dev_fault(fault_was)
                set_rnnt_dev_round_budget(budget_was)
                same_tokens(t, c.tag, r.leg)
                t |> equal(r.run.decodes, 0l, "{c.tag}: no device decode")
                t |> equal(r.run.why, 1l, "{c.tag}: one {c.why} decline")
                t |> equal(r.run.declines, c.why == "knob" ? 0l : 1l, "{c.tag}: the declines but the knob's")
            }
            var inscope after <- seat_leg(NetSpec(seed = 31u), true, "")
            same_tokens(t, "after the declines", after.leg)
            t |> equal(after.run.decodes, 1l, "the seat serves again")
        }
    }
}

[test]
def test_rnnt_device_shared_weights(t : T?) {
    t |> run("sessions of one model share the device weights; another net uploads its own") @(t : T?) {
        return if (rnnt_seat_skip(t, "shared weights"))
        with_dasllama_jobque_() {
            var f <- build(NetSpec(nl = 2l, seed = 41u))
            var s1 = RnntState()
            var s2 = RnntState()
            var inscope a <- seat_decode(f, s1, "")
            var inscope b <- seat_decode(f, s2, "")
            var inscope c <- seat_decode(f, s1, "")
            t |> equal(a.decodes + b.decodes + c.decodes, 3l, "three decodes served")
            t |> equal(a.uploads + b.uploads + c.uploads, 1l, "two sessions alternating upload the model once")
            unsafe {
                delete f
            }
            var g <- build(NetSpec(nl = 2l, seed = 41u, tok0_bias = 3.0))
            var inscope d <- seat_decode(g, s1, "")
            t |> equal(d.uploads, 1l, "a net differing in one joint bias uploads its own, at whatever address it lands")
            delete s1
            delete s2
            unsafe {
                delete g
            }
        }
    }
}
