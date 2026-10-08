// The Vulkan tower twins' shared instrument - the seat guard, the three-way encode (the exact chain, the CPU q8 chain,
// the device chain) with the family's counters and the bar by metric (rel_l2 wherever a CPU q8 chain is the control,
// qwen25v's maxdiff against the exact chain), the input poison and the exact-lane decline, the vision canvas and dump-poison legs, the in-memory tower mint,
// the GPU knobs' capture and restore (`_gpu_knobs`, re-exported) - one text the vision and audio families' test files call
// with their own tower, state and encode; no fixture root beside it.

options gen2
options _dasllama_internal = true

module _tower_twin shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/defer
require daslib/fio   // nolint:STYLE030,LINT019 — path_join: the twin cells' jfk.wav, inside the GPU arms alone
require _compares   // logits_maxdiff
require _vision_oracle   // the mtmd dump parser and the canvas fixtures the poison legs read
require _model_tier   // tower_parity_stats - the rel_l2 statistic the audio twins' bar reads
require _gpu_knobs public
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_audio_io

let TWIN_SLACK_VK = 1.5   //! 1.5x: the q8 chain's activation quants plus f16 weight scales, the tiles' summation order
let TWIN_ABS_VK = 2e-4    //! the tier-1 dumps' absolute floor, the bar's other term
let TWIN_REL_L2_FLOOR = 1e-4lf   //! the audio twins' absolute floor over rel_l2, a unit-free ratio
let TWIN_EXACT_REL_L2_VK = 2e-2lf   //! the exact lane's bar over rel_l2 from the exact CPU chain: the f32 tile over the f32 rows, the attention's f16 panels and the tiles' order the distance

//! a front's witness follows its route: on the f16 GEMM feed (cm2 or KHR) the front served every dispatch, off it
//! (DASLLAMA_COOPMAT=sdot4) the driver declined `device` and served none while the blocks served either way; `declines_delta` is
//! the DEVICE leg's own (the knob-off legs' declines before it are not its evidence)
def front_served(tt : T?; tag : string; convs_delta, expected, declines_delta : int64) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (vulkan_tower_front_route()) {
            tt |> equal(convs_delta, expected, "{tag}: the front served every dispatch on the device (convs delta = {expected})")
        } else {
            tt |> equal(convs_delta, 0l, "{tag}: off the f16 GEMM feed the front stays on the CPU (convs delta)")
            tt |> success(declines_delta >= 1l && key_exists(vulkan_tower_declines(), "device"), "{tag}: the front recorded the `device` decline off the feed")
        }
    }
}

//! a jfk twin cell's seat, each miss a loud skip - the JIT (when `jit_only`), the model at `model_path` and jfk.wav stocked, the
//! Vulkan twin's seat - then `body` over the clip with the whisper-class block hooks on (every claim here depends on that seat), the
//! Vulkan tower and ASR-decoder knobs put back after it, the block hooks to their default
[unused_argument(tt, model_path, jit_only, body)]
def jfk_twin_cell(tt : T?; model_path : string; jit_only : bool; body : block<(samples : array<float>) : void>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (jit_only && !jit_enabled()) {
            tt |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!model_available(tt, model_path)) {
            return
        }
        let samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            tt |> skip("jfk.wav not present")
            return
        }
        if (!vulkan_twin_seat(tt)) {
            return
        }
        with_gpu_knobs() {
            defer() {
                set_vulkan_audio_blocks(true)
            }
            set_vulkan_audio_blocks(true)
            invoke(body, samples)
        }
    }
}

//! the Vulkan twin's seat, or a loud skip: no seat where das_metal owns the tower hooks, none without the tier's want and a device it serves
def vulkan_twin_seat(tt : T?) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            tt |> skip("das_metal on this build owns the tower hooks - the Vulkan twin has no seat")
            return false
        } else {
            if (!vulkan_tower_arms()) {
                tt |> skip("no Vulkan device (DASLLAMA_GPU=1 and a device the tier serves)")
                return false
            }
            return true
        }
    } else {
        tt |> skip("dasVulkan not present")
        return false
    }
}

typedef VtStats = tuple<encodes : int64; rows : int64; blocks : int64; declines : int64; convs : int64; mels : int64>   //! `vulkan_tower_stats`' counters

//! how a twin reads its distances
enum TwinMetric {
    maxdiff   //! max |a - b|, read in the reference chain's rms; the bar `TWIN_SLACK_VK` x the CPU q8 chain's own plus `TWIN_ABS_VK` (qwen25v's own bar rides the poison leg alone)
    rel_l2    //! `tower_parity_stats`' rel_l2; the bar `TWIN_SLACK_VK` x the CPU q8 chain's own plus `TWIN_REL_L2_FLOOR` (every twin with a CPU q8 chain: a deep tower's worst
              //! element is a tail of one deepstack tap row that reads the same at 18 and 24 blocks while the CPU q8 chain's own worst element and the rms shrink)
}

//! what a twin names and reads: the family and the input (every assert's and log line's tag), the tower's depth, the bar log's suffix
struct TwinSpec {
    family : string
    tag : string
    n_layer : int64
    log_suffix : string
    metric : TwinMetric
}

//! a three-way's driver counters - before the exact leg, after it, after the CPU q8 leg, after the device leg - and each leg's encode count
struct TwinLegs {
    pre : VtStats
    exact : VtStats
    cpu : VtStats
    dev : VtStats
    n_exact : int64
    n_cpu : int64
    n_dev : int64
}

struct TwinResult {
    spec : TwinSpec
    rms : float           //! the reference chain's rms, the unit a maxdiff distance is read in
    bar : double          //! what the device chain held to; the poison must exceed it (0 when a rel_l2 compare could not run)
    reference : array<float>   //! the reference chain's encode, the poison's other side
}

//! the knob-off leg never dispatched and counted its decline; the device leg served one encode, every block, the patch grid's rows
def twin_counters(tt : T?; tag : string; legs : TwinLegs; n_layer, npos : int64) {
    tt |> equal(legs.n_cpu, legs.n_dev, "{tag}: token count on both chains")
    tt |> equal(legs.cpu.encodes - legs.exact.encodes, 0l, "{tag}: the knob-off leg never dispatched")
    tt |> equal(legs.cpu.declines - legs.exact.declines, 1l, "{tag}: the knob-off leg recorded the knob decline")
    device_leg_served(tt, tag, legs.cpu, legs.dev, n_layer, npos)
}

//! a device leg's engage evidence against the driver's counters before it (`s0`) and after (`s1`): one encode, every block,
//! and the patch grid's rows where `npos` is not negative
def device_leg_served(tt : T?; tag : string; s0, s1 : auto; n_layer, npos : int64) {
    tt |> equal(s1.encodes - s0.encodes, 1l, "{tag}: device-served (encodes delta)")
    tt |> equal(s1.blocks - s0.blocks, n_layer, "{tag}: every block ran on the driver (blocks delta)")
    if (npos >= 0l) {
        tt |> equal(s1.rows - s0.rows, npos, "{tag}: the rows delta is the patch grid")
    }
}

//! a q8-lane leg on the serving GPU tower: where `encode`'s gate ran (true), it served one encode and every block with no
//! `quant_mode` decline
def q8_lane_served(tt : T?; tag : string; n_layer : int64; encode : block<() : bool>) {
    let s0 = gpu_tower_counters()
    let dq0 = gpu_tower_quant_declines()
    if (invoke(encode)) {
        device_leg_served(tt, tag, s0, gpu_tower_counters(), n_layer, -1l)
        tt |> equal(gpu_tower_quant_declines() - dq0, 0l, "{tag}: no quant_mode decline")
    }
}

//! one input three ways - the exact chain, the CPU q8 chain (both knob off), the device chain - then `counters` over the legs and
//! the bar by `spec.metric`: the device's distance from the exact chain within `TWIN_SLACK_VK` x the CPU q8 chain's own; `encode`
//! is the family's encode over a tower and its state, returning its token count
def twin_three_way(tt : T?; spec : TwinSpec; tw : auto(TW); var st : auto(ST); tw_exact : TW; var st_exact : ST;
                   encode : block<(t : TW; var s : ST; var out : array<float>) : int64>;
                   counters : block<(legs : TwinLegs) : void>) : TwinResult {
    var r = TwinResult(spec = spec)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var cpu, dev : array<float>
        var legs = TwinLegs()
        with_gpu_knobs() {
            set_vulkan_tower(false)
            legs.pre = vulkan_tower_stats()
            legs.n_exact = queued_encode(tw_exact, st_exact, r.reference, encode)
            legs.exact = vulkan_tower_stats()
            legs.n_cpu = queued_encode(tw, st, cpu, encode)
            legs.cpu = vulkan_tower_stats()
            set_vulkan_tower(true)
            legs.n_dev = queued_encode(tw, st, dev, encode)
            legs.dev = vulkan_tower_stats()
        }
        if (legs.dev.encodes == legs.cpu.encodes) {
            to_log(LOG_INFO, "{spec.family} vulkan twin {spec.tag}: the driver declined - {vulkan_tower_declines()}\n")
        }
        invoke(counters, legs)
        if (spec.metric == TwinMetric.maxdiff) {
            r.rms = rms(r.reference)
            let q8_ex = logits_maxdiff(cpu, r.reference)
            let dev_ex = logits_maxdiff(dev, r.reference)
            let dev_q8 = logits_maxdiff(dev, cpu)
            let bar = TWIN_SLACK_VK * q8_ex + TWIN_ABS_VK
            r.bar = double(bar)
            to_log(LOG_INFO, "{spec.family} vulkan twin {spec.tag} ({spec.n_layer} blocks{spec.log_suffix}): rms {r.rms}; vs exact: cpu q8 {q8_ex} ({q8_ex / r.rms} x rms), device {dev_ex} ({dev_ex / r.rms} x rms); device vs cpu q8 {dev_q8} ({dev_q8 / r.rms} x rms)\n")
            tt |> success(dev_ex <= bar, "{spec.tag}: the device chain sits within {TWIN_SLACK_VK}x the CPU q8 chain's distance from the exact chain ({dev_ex} vs {q8_ex})")
        } else {
            r.bar = tower_twin_bar(tt, spec.tag, r.reference, cpu, dev, spec.n_layer, double(TWIN_SLACK_VK), TWIN_REL_L2_FLOOR)
        }
        delete cpu
        delete dev
    }
    return <- r
}

//! the input poison: the device chain over a fresh tower with `zero_block` zeroed must EXCEED the bar the clean chains set (a
//! rel_l2 bar of 0 - its compare could not run - holds nothing, so the compare is skipped; the device encode is asserted either way)
def twin_poison(tt : T?; r : TwinResult; zero_block : int64; twp : auto(TW); var stp : auto(ST);
                encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var inscope poisoned <- device_poison_leg(tt, r.spec.tag, twp, stp, encode)
        if (r.spec.metric == TwinMetric.maxdiff) {
            let poison_ex = logits_maxdiff(poisoned, r.reference)
            to_log(LOG_INFO, "{r.spec.family} vulkan twin {r.spec.tag} ({r.spec.n_layer} blocks) poison: block {zero_block} zeroed on the device lands {poison_ex} from the exact chain ({poison_ex / r.rms} x rms), bar {r.bar}\n")
            tt |> success(double(poison_ex) > r.bar, "{r.spec.tag}: a zeroed block through the device chain EXCEEDS the bar ({poison_ex} vs {r.bar})")
        } elif (r.bar > 0.0lf) {
            tower_twin_poisoned(tt, r.spec.tag, r.reference, poisoned, r.bar)
        }
    }
}

//! `encode` over a tower and its state on a set-up job queue; the encode's token count
def private queued_encode(tw : auto(TW); var st : auto(ST); var out : array<float>; encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) : int64 {
    var n = 0l
    with_dasllama_jobque_() {
        n = invoke(encode, tw, st, out)
    }
    return n
}

//! a poison's device leg: `encode` over the zeroed tower `twp` with the Vulkan tower knob on, one device encode asserted; the rows it wrote
def private device_poison_leg(tt : T?; tag : string; twp : auto(TW); var stp : auto(ST); encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) : array<float> {
    var inscope out : array<float>
    let e0 = gpu_tower_encodes()
    with_gpu_knobs() {
        set_serving_gpu_tower(true)
        queued_encode(twp, stp, out, encode)
    }
    tt |> equal(gpu_tower_encodes() - e0, 1l, "{tag} poison: device-served (encodes delta)")
    return <- out
}

//! a vision canvas three ways on the patch grid's counters, then - `zero_block` >= 0 - the input poison over the q8 tower
//! `mint_poisoned` returns with that block zeroed; `encode` is the family's encode of the canvas
def twin_canvas(tt : T?; family, kind : string; w, h : int; patch : int64; log_suffix : string; tw : auto(TW); var st : auto(ST);
                tw_exact : TW; var st_exact : ST; zero_block : int64; mint_poisoned : block<(zb : int64) : TW>;
                encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let spec = TwinSpec(family = family, tag = "{kind} {w}x{h}", n_layer = tw.n_layer, log_suffix = log_suffix, metric = TwinMetric.rel_l2)
        let npos = (int64(w) / patch) * (int64(h) / patch)
        let r <- twin_three_way(tt, spec, tw, st, tw_exact, st_exact, encode) $(legs) {
            twin_counters(tt, spec.tag, legs, spec.n_layer, npos)
        }
        if (zero_block >= 0l) {
            var twp <- invoke(mint_poisoned, zero_block)
            var stp = ST()
            twin_poison(tt, r, zero_block, twp, stp, encode)
            delete stp
            delete twp
        }
    }
}

//! the input poison of a device-side dump bar on the serving home: `twp` - a q8 tower with `zero_block` zeroed - served by the driver
//! must EXCEED the bar its dump holds (`bar_rms` x rms), a bar the poison clears measuring nothing; `excess` reads the encode over the dump's bar
def dump_poison(tt : T?; family, tag : string; zero_block : int64; bar_rms : float; twp : auto(TW); var stp : auto(ST);
                encode : block<(t : TW; var s : ST; var out : array<float>) : int64>; excess : block<(out : array<float>) : float>) {
    var inscope out <- device_poison_leg(tt, tag, twp, stp, encode)
    let ex = invoke(excess, out)
    to_log(LOG_INFO, "{family} device poison leg {tag}: block {zero_block} zeroed -> excess {ex} over the {bar_rms} x rms bar\n")
    tt |> success(ex > 0.0, "{tag}: a zeroed block through the device chain reds the bar (excess {ex})")
}

//! `logname`'s mtmd dump under `oracle_dir` beside the models, or the loud skip naming `mint_hint`; false on the skip
def oracle_dump(tt : T?; oracle_dir, logname, mint_hint : string; var o : OracleEncode) : bool {
    if (!load_oracle_encode(path_join(path_join(models_dir(), oracle_dir), logname), o)) {
        tt |> skip("oracle dump {logname} not present/parseable - mint via {mint_hint}")
        return false
    }
    return true
}

//! the input poison of a device-side dump bar: `twp`, the family's tower with block `zero_block` zeroed, served by the driver on
//! the `kind` canvas at w x h must EXCEED the bar (`bar_rms` x rms past `abs_tol`) the dump `o` holds - a bar the poison clears
//! measures nothing; `encode` is the family's encode of the planes, `row_width` the dump's row
def dump_poison_leg(tt : T?; family, kind : string; w, h : int; o : OracleEncode; bar_rms : float; zero_block : int64; row_width : int;
                    abs_tol : float; twp : auto(TW); var stp : auto(ST);
                    encode : block<(t : TW; var s : ST; planes : array<float>; var out : array<float>) : int64>) {
    let planes <- make_planes(kind, w, h)
    dump_poison(tt, family, "{kind} {w}x{h}", zero_block, bar_rms, twp, stp,
        $(t; var s; var out) => invoke(encode, t, s, planes, out)) $(out) => encode_excess(out, row_width, o, abs_tol, bar_rms)
}

//! a tower minted in memory from `stg`: `truncate_and_poison` cuts it to `nl` blocks and zeroes `zero_block`'s planes, `mint`
//! builds it - never the family's load, which bakes a `.dlim`
def mint_staged_tower(var stg : auto(STG); nl, zero_block : int64; mint : function<(var s : STG; var t : auto(TW)) : void>) : TW {
    truncate_and_poison(stg, nl, zero_block)
    var tw = TW()
    invoke(mint, stg, tw)
    return <- tw
}

//! `mint_staged_tower` over the planes `stage` reads from `mm` under the family's lane pin (`pin(q8)`, `unpin` restoring the policy)
def mint_twin_tower(mm : string; q8 : bool; nl, zero_block : int64; pin : function<(on : bool) : void>; unpin : function<() : void>;
                    stage : function<(path : string) : auto(STG)>; mint : function<(var s : STG; var t : auto(TW)) : void>) : TW {
    invoke(pin, q8)
    defer() {
        invoke(unpin)
    }
    var inscope stg <- invoke(stage, mm)
    return <- mint_staged_tower(stg, nl, zero_block, mint)
}

//! the exact-lane tower encoded with the knob on: the driver dispatches nothing - no block, no front - records the `quant_mode`
//! decline, and the CPU chain serves the encode (`encode` returns the output's length); `one_seat`: the tower reaches the driver at
//! the blocks seat alone, so exactly one decline (a vision tower), else at least one (an audio tower declines at its front and at
//! its blocks, a chunked one per chunk)
[unused_argument(tt, tag, expected_len, one_seat, encode)]
def exact_lane_declines(tt : T?; tag : string; expected_len : int64; one_seat : bool; encode : block<() : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(true)
        let x0 = vulkan_tower_stats()
        let dq0 = vulkan_tower_declines()?["quant_mode"] ?? 0l
        var n = 0l
        with_dasllama_jobque_() {
            n = invoke(encode)
        }
        let x1 = vulkan_tower_stats()
        let dq = (vulkan_tower_declines()?["quant_mode"] ?? 0l) - dq0
        tt |> equal(x1.encodes - x0.encodes, 0l, "{tag}: the exact-lane tower never dispatched a block with the knob on")
        tt |> equal(x1.convs - x0.convs, 0l, "{tag}: the exact-lane tower never dispatched a front with the knob on")
        if (one_seat) {
            tt |> equal(dq, 1l, "{tag}: the exact-lane tower reached the blocks seat and the driver declined quant_mode")
        } else {
            tt |> success(dq >= 1l, "{tag}: the exact-lane tower reached the driver and it declined quant_mode ({dq} declines)")
        }
        tt |> equal(n, expected_len, "{tag}: the CPU chain served the exact-lane encode")
    }
}

//! a vision tower's exact lane served on the device: the knob on, one encode, every block, the grid's rows, no `quant_mode`
//! decline, and the device's rel_l2 from the exact CPU chain (the knob off) within `TWIN_EXACT_REL_L2_VK` - the expectation moved
//! past the bar at one element as the compare's control; then the input poison: `twp`, the exact tower with a block zeroed,
//! through the device chain must EXCEED the bar
def exact_lane_served(tt : T?; tag : string; tw_exact : auto(TW); var st_exact : auto(ST); npos : int64; twp : TW; var stp : ST;
                      encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var cpu, dev : array<float>
        var n_cpu = 0l
        var n_dev = 0l
        var s0 = VtStats()
        var s1 = VtStats()
        var dq = 0l
        with_gpu_knobs() {
            set_vulkan_tower(false)
            n_cpu = queued_encode(tw_exact, st_exact, cpu, encode)
            set_vulkan_tower(true)
            s0 = vulkan_tower_stats()
            let dq0 = vulkan_tower_declines()?["quant_mode"] ?? 0l
            n_dev = queued_encode(tw_exact, st_exact, dev, encode)
            s1 = vulkan_tower_stats()
            dq = (vulkan_tower_declines()?["quant_mode"] ?? 0l) - dq0
        }
        if (s1.encodes == s0.encodes) {
            to_log(LOG_INFO, "{tag} exact lane: the driver declined - {vulkan_tower_declines()}\n")
        }
        tt |> equal(n_cpu, n_dev, "{tag} exact lane: token count on both chains")
        device_leg_served(tt, "{tag} exact lane", s0, s1, tw_exact.n_layer, npos)
        tt |> equal(dq, 0l, "{tag} exact lane: no quant_mode decline")
        tower_twin_held(tt, "{tag} exact lane", cpu, dev, TWIN_EXACT_REL_L2_VK)
        var nsq = 0.0lf
        for (v in cpu) {
            nsq += double(v) * double(v)
        }
        var moved := cpu
        moved[0] += float(2.0lf * TWIN_EXACT_REL_L2_VK * sqrt(nsq) + double(abs(dev[0] - cpu[0])) + 1.0lf)
        var stm = TowerParityStats()
        if (tower_parity_stats(tt, "{tag} exact-lane control", moved, dev, stm)) {
            tt |> success(stm.rel_l2 > TWIN_EXACT_REL_L2_VK, "{tag} exact lane control: the expectation moved past the bar at one element reds it ({stm.rel_l2})")
        }
        delete moved
        var inscope poisoned <- device_poison_leg(tt, "{tag} exact lane", twp, stp, encode)
        tower_twin_poisoned(tt, "{tag} exact lane", cpu, poisoned, TWIN_EXACT_REL_L2_VK)
        delete cpu
        delete dev
    }
}

//! the device chain's rel_l2 from the exact chain held within `slack` x the CPU q8 chain's own plus `floor`; returns the bar, 0 when a compare could not run
def tower_twin_bar(t : T?; tag : string; exact, cpu, dev : array<float>; n_layer : int64; slack, floor : double) : double {
    var q8_ex = TowerParityStats()
    var dev_ex = TowerParityStats()
    var dev_q8 = TowerParityStats()
    let ok = (tower_parity_stats(t, "{tag} cpu q8 vs exact", exact, cpu, q8_ex)
        && tower_parity_stats(t, "{tag} vulkan vs exact", exact, dev, dev_ex)
        && tower_parity_stats(t, "{tag} vulkan vs cpu q8", cpu, dev, dev_q8))
    if (!ok) {
        return 0.0lf
    }
    let bar = slack * q8_ex.rel_l2 + floor
    t |> success(dev_ex.rel_l2 <= bar, "{tag} vulkan blocks {n_layer}L: the device chain sits within {slack}x the CPU q8 chain's rel_l2 from the exact chain ({dev_ex.rel_l2} vs {q8_ex.rel_l2})")
    return bar
}

//! a device encode's rel_l2 from the exact chain held within `bar`
def tower_twin_held(t : T?; tag : string; exact, dev : array<float>; bar : double) {
    tower_twin_verdict(t, tag, exact, dev, bar, false)
}

//! the zeroed-plane input poison: a device encode's rel_l2 from the exact chain past `bar`
def tower_twin_poisoned(t : T?; tag : string; exact, dev : array<float>; bar : double) {
    tower_twin_verdict(t, tag, exact, dev, bar, true)
}

def private tower_twin_verdict(t : T?; tag : string; exact, dev : array<float>; bar : double; must_exceed : bool) {
    var st = TowerParityStats()
    return if (!tower_parity_stats(t, must_exceed ? "{tag} poisoned vulkan vs exact" : "{tag} vulkan vs exact", exact, dev, st))
    if (must_exceed) {
        t |> success(st.rel_l2 > bar, "{tag}: a zeroed block through the device chain EXCEEDS the bar ({st.rel_l2} vs {bar})")
    } else {
        t |> success(st.rel_l2 <= bar, "{tag}: the device chain sits within the bar ({st.rel_l2} vs {bar})")
    }
}

//! a staged tower truncated to `nl` blocks (0 = the file's count) with `zero_block`'s GEMM planes zeroed when >= 0 - the
//! input poison of the device-side bars, through every plane the driver reads: a q8 tower's qblob; an exact tower's blob and
//! its halfword plane (the twin a family bakes beside the blob, or the bf16 rows the file holds) wherever the plane spans the block
def truncate_and_poison(var stg : auto; nl, zero_block : int64) {
    if (nl > 0l) {
        stg.t.n_layer = nl
    }
    if (zero_block >= 0l) {
        let base = stg.t.layer_w + zero_block * stg.t.layer_w_stride
        let stride = stg.t.layer_w_stride
        var q8 = false
        static_if (typeinfo has_field<q8>(stg.t)) {
            q8 = stg.t.q8
        }
        if (q8) {
            static_if (typeinfo has_field<qblob>(stg)) {
                for (i in range64(stride)) {
                    stg.qblob[base + i] = int8(0)
                }
            }
        } else {
            for (i in range64(long_length(stg.blob) >= base + stride ? stride : 0l)) {
                stg.blob[base + i] = 0.0
            }
            for (i in range64(long_length(stg.wblob) >= base + stride ? stride : 0l)) {
                stg.wblob[base + i] = uint16(0)
            }
        }
    }
}
