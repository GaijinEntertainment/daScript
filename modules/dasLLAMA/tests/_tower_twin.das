// The Vulkan tower twins' shared instrument - the seat guard, the three-way encode (the exact chain, the CPU q8 chain,
// the device chain) with the family's counters and the bar by metric (the vision canvases' maxdiff, the audio towers'
// rel_l2), the input poison and the exact-lane decline, the vision canvas and dump-poison legs, the in-memory tower mint,
// the GPU knobs' capture and restore - one text the vision and audio families' test files call with their own tower,
// state and encode; no fixture root beside it.

options gen2
options _dasllama_internal = true

module _tower_twin shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/defer
require daslib/fio   // path_join: the twin cells' jfk.wav
require math
require _compares   // logits_maxdiff
require _model_tier   // tower_parity_stats - the rel_l2 statistic the audio twins' bar reads
require ?das_metal dasllama/dasllama_metal_tower
require ?das_metal dasllama/dasllama_metal_asr_dec
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_asr_dec

let TWIN_SLACK_VK = 1.5   //! 1.5x: the q8 chain's activation quants plus f16 weight scales, the tiles' summation order, the closed-form GELU against the f16 LUT
let TWIN_ABS_VK = 2e-4    //! the tier-1 dumps' absolute floor, the bar's other term
let TWIN_REL_L2_FLOOR = 1e-4lf   //! the audio twins' absolute floor over rel_l2, a unit-free ratio

//! the Vulkan tower knob as the cell found it (false on a build without dasVulkan)
def vulkan_tower_knob() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_tower_on()
    } else {
        return false
    }
}

//! puts back the Vulkan tower knob a cell captured with `vulkan_tower_knob` (a no-op without dasVulkan)
[unused_argument(on)]
def restore_vulkan_tower(on : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(on)
    }
}

//! the Vulkan ASR-decoder knob as the cell found it (false on a build without dasVulkan)
def vulkan_wdec_knob() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_wdec_on()
    } else {
        return false
    }
}

//! undoes `set_every_gpu_tower`: the Metal tower knob back to its default (it has no getter), the Vulkan one to what `vulkan_tower_knob` captured
def restore_every_gpu_tower(vulkan_was_on : bool) => set_every_gpu_tower(true, vulkan_was_on)

//! undoes `set_every_gpu_wdec`: the Metal ASR-decoder knob back to its default (it has no getter), the Vulkan one to what `vulkan_wdec_knob` captured
def restore_every_gpu_wdec(vulkan_was_on : bool) => set_every_gpu_wdec(true, vulkan_was_on)

//! the CPU-decoder claims pin every whisper ASR-decoder driver off (a hook that serves silently stops the cell measuring the CPU chain)
def set_every_gpu_wdec(on : bool) => set_every_gpu_wdec(on, on)

[unused_argument(metal_on, vulkan_on)]
def set_every_gpu_wdec(metal_on, vulkan_on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_wdec(metal_on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_wdec(vulkan_on)
    }
}

//! the CPU-lane claims pin every GPU tower driver off (a hook that serves silently stops the cell measuring the CPU chain)
def set_every_gpu_tower(on : bool) => set_every_gpu_tower(on, on)

[unused_argument(metal_on, vulkan_on)]
def set_every_gpu_tower(metal_on, vulkan_on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(metal_on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(vulkan_on)
    }
}

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
        let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            tt |> skip("jfk.wav not present")
            return
        }
        if (!vulkan_twin_seat(tt)) {
            return
        }
        {
            let tower_was_on = vulkan_tower_knob()
            let wdec_was_on = vulkan_wdec_knob()
            defer() {
                restore_vulkan_tower(tower_was_on)
                set_vulkan_wdec(wdec_was_on)
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
    maxdiff   //! max |a - b|, read in the reference chain's rms; the bar `TWIN_SLACK_VK` x the CPU q8 chain's own plus `TWIN_ABS_VK` (the vision canvases)
    rel_l2    //! `tower_parity_stats`' rel_l2; the bar `TWIN_SLACK_VK` x the CPU q8 chain's own plus `TWIN_REL_L2_FLOOR` (the audio towers)
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
    tt |> equal(legs.dev.encodes - legs.cpu.encodes, 1l, "{tag}: device-served (encodes delta)")
    tt |> equal(legs.dev.blocks - legs.cpu.blocks, n_layer, "{tag}: every block ran on the driver (blocks delta)")
    tt |> equal(legs.dev.rows - legs.cpu.rows, npos, "{tag}: the rows delta is the patch grid")
}

def rms_of(a : array<float>) : float {
    var ss = 0.0
    for (v in a) {
        ss += v * v
    }
    return sqrt(ss / float(max(length(a), 1)))
}

//! one input three ways - the exact chain, the CPU q8 chain (both knob off), the device chain - then `counters` over the legs and
//! the bar by `spec.metric`: the device's distance from the exact chain within `TWIN_SLACK_VK` x the CPU q8 chain's own; `encode`
//! is the family's encode over a tower and its state, returning its token count
def twin_three_way(tt : T?; spec : TwinSpec; tw : auto(TW); var st : auto(ST); tw_exact : TW; var st_exact : ST;
                   encode : block<(t : TW; var s : ST; var out : array<float>) : int64>;
                   counters : block<(legs : TwinLegs) : void>) : TwinResult {
    var r = TwinResult(spec = spec)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var cpu : array<float>
        var dev : array<float>
        var legs = TwinLegs()
        let knob = vulkan_tower_knob()
        defer() {
            restore_vulkan_tower(knob)
        }
        set_vulkan_tower(false)
        legs.pre = vulkan_tower_stats()
        with_job_que() {
            setup_dasllama_jobque_()
            legs.n_exact = invoke(encode, tw_exact, st_exact, r.reference)
        }
        legs.exact = vulkan_tower_stats()
        with_job_que() {
            setup_dasllama_jobque_()
            legs.n_cpu = invoke(encode, tw, st, cpu)
        }
        legs.cpu = vulkan_tower_stats()
        set_vulkan_tower(true)
        with_job_que() {
            setup_dasllama_jobque_()
            legs.n_dev = invoke(encode, tw, st, dev)
        }
        legs.dev = vulkan_tower_stats()
        if (legs.dev.encodes == legs.cpu.encodes) {
            to_log(LOG_INFO, "{spec.family} vulkan twin {spec.tag}: the driver declined - {vulkan_tower_declines()}\n")
        }
        invoke(counters, legs)
        if (spec.metric == TwinMetric.maxdiff) {
            r.rms = rms_of(r.reference)
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
        var poisoned : array<float>
        let knob = vulkan_tower_knob()
        defer() {
            restore_vulkan_tower(knob)
        }
        set_vulkan_tower(true)
        let p0 = vulkan_tower_stats()
        with_job_que() {
            setup_dasllama_jobque_()
            invoke(encode, twp, stp, poisoned)
        }
        tt |> equal(vulkan_tower_stats().encodes - p0.encodes, 1l, "{r.spec.tag} poison: device-served (encodes delta)")
        if (r.spec.metric == TwinMetric.maxdiff) {
            let poison_ex = logits_maxdiff(poisoned, r.reference)
            to_log(LOG_INFO, "{r.spec.family} vulkan twin {r.spec.tag} ({r.spec.n_layer} blocks) poison: block {zero_block} zeroed on the device lands {poison_ex} from the exact chain ({poison_ex / r.rms} x rms), bar {r.bar}\n")
            tt |> success(double(poison_ex) > r.bar, "{r.spec.tag}: a zeroed block through the device chain EXCEEDS the bar ({poison_ex} vs {r.bar})")
        } elif (r.bar > 0.0lf) {
            tower_twin_poison(tt, r.spec.tag, r.reference, poisoned, r.bar)
        }
        delete poisoned
    }
}

//! a vision canvas three ways on the patch grid's counters, then - `zero_block` >= 0 - the input poison over the q8 tower
//! `mint_poisoned` returns with that block zeroed; `encode` is the family's encode of the canvas
def twin_canvas(tt : T?; family, kind : string; w, h : int; patch : int64; extra : string; tw : auto(TW); var st : auto(ST);
                tw_exact : TW; var st_exact : ST; zero_block : int64; mint_poisoned : block<(zb : int64) : TW>;
                encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let spec = TwinSpec(family = family, tag = "{kind} {w}x{h}", n_layer = tw.n_layer, log_suffix = extra, metric = TwinMetric.maxdiff)
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

//! the input poison of a device-side dump bar: `twp` - a q8 tower with `zero_block` zeroed - served by the driver must EXCEED the
//! bar its dump holds (`bar_rms` x rms), a bar the poison clears measuring nothing; `excess` reads the encode over the dump's bar
def dump_poison(tt : T?; family, tag : string; zero_block : int64; bar_rms : float; twp : auto(TW); var stp : auto(ST);
                encode : block<(t : TW; var s : ST; var out : array<float>) : int64>; excess : block<(out : array<float>) : float>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var out : array<float>
        let knob = vulkan_tower_knob()
        defer() {
            restore_vulkan_tower(knob)
        }
        set_vulkan_tower(true)
        let e0 = vulkan_tower_stats().encodes
        with_job_que() {
            setup_dasllama_jobque_()
            invoke(encode, twp, stp, out)
        }
        tt |> equal(vulkan_tower_stats().encodes - e0, 1l, "{tag} poison: device-served (encodes delta)")
        let ex = invoke(excess, out)
        to_log(LOG_INFO, "{family} vulkan poison leg {tag}: block {zero_block} zeroed -> excess {ex} over the {bar_rms} x rms bar\n")
        tt |> success(ex > 0.0, "{tag}: a zeroed block through the device chain reds the bar (excess {ex})")
        delete out
    }
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
        with_job_que() {
            setup_dasllama_jobque_()
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

//! `exact_lane_declines` over a vision tower, whose blocks seat is its one: `encode` writes the family's encode into `out`
def exact_lane_declines(tt : T?; tag : string; tw_exact : auto(TW); var st_exact : auto(ST); expected_len : int64;
                        encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    var outx : array<float>
    exact_lane_declines(tt, tag, expected_len, true) $() : int64 {
        invoke(encode, tw_exact, st_exact, outx)
        return long_length(outx)
    }
    delete outx
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

//! a device encode over the same input held to the bar a clean three-way set (a later encode on the residency, a fresh one after a shutdown)
def tower_twin_held(t : T?; tag : string; exact, dev : array<float>; bar : double) {
    var st = TowerParityStats()
    if (tower_parity_stats(t, "{tag} vulkan vs exact", exact, dev, st)) {
        t |> success(st.rel_l2 <= bar, "{tag}: the device chain sits within the bar the first encode set ({st.rel_l2} vs {bar})")
    }
}

//! the input poison: a device encode over a tower with zeroed GEMM planes must EXCEED the bar the clean chains set
def tower_twin_poison(t : T?; tag : string; exact, poisoned : array<float>; bar : double) {
    var st = TowerParityStats()
    if (tower_parity_stats(t, "{tag} poisoned vulkan vs exact", exact, poisoned, st)) {
        t |> success(st.rel_l2 > bar, "{tag}: a zeroed block through the device chain EXCEEDS the bar ({st.rel_l2} vs {bar})")
    }
}

//! a staged tower truncated to `nl` blocks (0 = the file's count) with `zero_block`'s GEMM planes zeroed when >= 0 - the
//! input poison of the device-side bars, through every plane the driver reads (a q8 family's qblob; the halfword twin
//! beside the blob where the family bakes one)
def truncate_and_poison(var stg : auto; nl, zero_block : int64) {
    if (nl > 0l) {
        stg.t.n_layer = nl
    }
    if (zero_block >= 0l) {
        let base = stg.t.layer_w + zero_block * stg.t.layer_w_stride
        static_if (typeinfo has_field<qblob>(stg)) {
            for (i in range64(stg.t.layer_w_stride)) {
                stg.qblob[base + i] = int8(0)
            }
        } else {
            for (i in range64(stg.t.layer_w_stride)) {
                stg.blob[base + i] = 0.0
            }
            for (i in range64(long_length(stg.wblob) > 0l ? stg.t.layer_w_stride : 0l)) {
                stg.wblob[base + i] = uint16(0)
            }
        }
    }
}
