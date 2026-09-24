// The Vulkan tower twins' shared instrument - the seat guard, the three-way encode (the exact chain, the CPU q8 chain,
// the device chain) with its counters and bar, the input poison and the exact-lane decline - one text the four vision
// families' test files call with their own tower, state and encode; no fixture root beside it.

options gen2
options _dasllama_internal = true

module _tower_twin shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require math
require _compares   // logits_maxdiff
require ?das_metal dasllama/dasllama_metal_tower
require ?vulkan dasllama/dasllama_vulkan_tower

let TWIN_SLACK_VK = 1.5   //! 1.5x: the q8 chain's activation quants plus f16 weight scales, the tiles' summation order, the closed-form GELU against the f16 LUT
let TWIN_ABS_VK = 2e-4    //! the tier-1 dumps' absolute floor, the bar's other term

//! the CPU-lane claims pin every GPU tower driver off (a hook that serves silently stops the cell measuring the CPU chain)
def set_every_gpu_tower(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(on)
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
