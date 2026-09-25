// The Vulkan tower twins' shared instrument - the seat guard, the three-way encode (the exact chain, the CPU q8 chain,
// the device chain) with its counters and bar, the input poison and the exact-lane decline, the audio twins' rel_l2
// bar with its hold and poison, the Vulkan knob's capture and restore - one text the vision and audio families' test
// files call with their own tower, state and encode; no fixture root beside it.

options gen2
options _dasllama_internal = true

module _tower_twin shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/defer
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

//! the CPU-decoder claims pin every whisper ASR-decoder driver off (a hook that serves silently stops the cell measuring the CPU chain)
def set_every_gpu_wdec(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_wdec(on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_wdec(on)
    }
}

//! the CPU-lane claims pin every GPU tower driver off (a hook that serves silently stops the cell measuring the CPU chain)
def set_every_gpu_tower(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(on)
    }
}

//! a front's witness follows its route: on the f16 GEMM feed (cm2 or KHR) the front served every dispatch, off it
//! (DASLLAMA_COOPMAT=sdot4) the driver declined `device` and served none while the blocks served either way
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

struct TwinResult {
    rms : float           //! the reference chain's rms, the unit every distance is read in
    bar : float           //! what the device chain held to; the poison must exceed it
    reference : array<float>   //! the reference chain's encode, the poison's other side
}

//! the knob-off leg never dispatched and counted its decline; the device leg served one encode, every block, the patch grid's rows
def twin_counters(tt : T?; lbl : string; d0, d1, d2 : auto; ncpu, ngpu, n_layer, npos : int64) {
    tt |> equal(ncpu, ngpu, "{lbl}: token count on both chains")
    tt |> equal(d1.encodes - d0.encodes, 0l, "{lbl}: the knob-off leg never dispatched")
    tt |> equal(d1.declines - d0.declines, 1l, "{lbl}: the knob-off leg recorded the knob decline")
    tt |> equal(d2.encodes - d1.encodes, 1l, "{lbl}: device-served (encodes delta)")
    tt |> equal(d2.blocks - d1.blocks, n_layer, "{lbl}: every block ran on the driver (blocks delta)")
    tt |> equal(d2.rows - d1.rows, npos, "{lbl}: the rows delta is the patch grid")
}

def rms_of(a : array<float>) : float {
    var ss = 0.0
    for (v in a) {
        ss += v * v
    }
    return sqrt(ss / float(max(length(a), 1)))
}

//! one canvas three ways - the exact chain, the CPU q8 chain, the device chain - the counters and the bar: the device's
//! distance from the exact chain within `TWIN_SLACK_VK` x the CPU q8 chain's own; `encode` is the family's encode over
//! a tower and its state
def twin_three_way(tt : T?; family, kind : string; w, h : int; patch : int64; extra : string;
                   tw : auto(TW); var st : auto(ST); tw_exact : TW; var st_exact : ST;
                   encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) : TwinResult {
    var r : TwinResult
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let lbl = "{kind} {w}x{h}"
        var cpu : array<float>
        var gpu : array<float>
        var ncpu = 0l
        var ngpu = 0l
        let knob = vulkan_tower_on()
        defer() {
            set_vulkan_tower(knob)
        }
        set_vulkan_tower(false)
        with_job_que() {
            setup_dasllama_jobque_()
            invoke(encode, tw_exact, st_exact, r.reference)
        }
        let d0 = vulkan_tower_stats()
        with_job_que() {
            setup_dasllama_jobque_()
            ncpu = invoke(encode, tw, st, cpu)
        }
        let d1 = vulkan_tower_stats()
        set_vulkan_tower(true)
        with_job_que() {
            setup_dasllama_jobque_()
            ngpu = invoke(encode, tw, st, gpu)
        }
        let d2 = vulkan_tower_stats()
        if (d2.encodes == d1.encodes) {
            to_log(LOG_INFO, "{family} vulkan twin {lbl}: the driver declined - {vulkan_tower_declines()}\n")
        }
        twin_counters(tt, lbl, d0, d1, d2, ncpu, ngpu, tw.n_layer, (int64(w) / patch) * (int64(h) / patch))
        r.rms = rms_of(r.reference)
        let q8_ex = logits_maxdiff(cpu, r.reference)
        let dev_ex = logits_maxdiff(gpu, r.reference)
        let dev_q8 = logits_maxdiff(gpu, cpu)
        r.bar = TWIN_SLACK_VK * q8_ex + TWIN_ABS_VK
        to_log(LOG_INFO, "{family} vulkan twin {lbl} ({tw.n_layer} blocks{extra}): rms {r.rms}; vs exact: cpu q8 {q8_ex} ({q8_ex / r.rms} x rms), device {dev_ex} ({dev_ex / r.rms} x rms); device vs cpu q8 {dev_q8} ({dev_q8 / r.rms} x rms)\n")
        tt |> success(dev_ex <= r.bar, "{lbl}: the device chain sits within {TWIN_SLACK_VK}x the CPU q8 chain's distance from the exact chain ({dev_ex} vs {q8_ex})")
    }
    return <- r
}

//! the input poison: the device chain over a fresh tower with `zero_block` zeroed must EXCEED the bar the clean chains set
def twin_poison(tt : T?; family, kind : string; w, h : int; zero_block : int64; twp : auto(TW); var stp : auto(ST);
                r : TwinResult; encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let lbl = "{kind} {w}x{h}"
        var poisoned : array<float>
        let p0 = vulkan_tower_stats()
        with_job_que() {
            setup_dasllama_jobque_()
            invoke(encode, twp, stp, poisoned)
        }
        tt |> equal(vulkan_tower_stats().encodes - p0.encodes, 1l, "{lbl} poison: device-served (encodes delta)")
        let poison_ex = logits_maxdiff(poisoned, r.reference)
        to_log(LOG_INFO, "{family} vulkan twin {lbl} ({twp.n_layer} blocks) poison: block {zero_block} zeroed on the device lands {poison_ex} from the exact chain ({poison_ex / r.rms} x rms), bar {r.bar}\n")
        tt |> success(poison_ex > r.bar, "{lbl}: a zeroed block through the device chain EXCEEDS the bar ({poison_ex} vs {r.bar})")
    }
}

//! the exact-lane tower reaches the blocks seat with the knob on: the driver declines `quant_mode` once, never dispatches, the CPU chain serves the encode
def exact_lane_declines(tt : T?; tw_exact : auto(TW); var st_exact : auto(ST); expected_len : int64;
                        encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var outx : array<float>
        set_vulkan_tower(true)
        let x0 = vulkan_tower_stats()
        let dq0 = vulkan_tower_declines()?["quant_mode"] ?? 0l
        with_job_que() {
            setup_dasllama_jobque_()
            invoke(encode, tw_exact, st_exact, outx)
        }
        tt |> equal(vulkan_tower_stats().encodes - x0.encodes, 0l, "the exact-lane tower never dispatched")
        tt |> equal((vulkan_tower_declines()?["quant_mode"] ?? 0l) - dq0, 1l, "the exact-lane tower reached the blocks seat and the driver declined quant_mode")
        tt |> equal(long_length(outx), expected_len, "the CPU chain served the exact-lane encode")
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
