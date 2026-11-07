// The GPU homes' tower twins' shared instrument - the seat guard on the home each family's seats serve on, the three-way encode (the exact chain, the CPU q8 chain,
// the device chain) with the family's counters and the bar by metric (rel_l2 wherever a CPU q8 chain is the control,
// qwen25v's maxdiff against the exact chain), the input poison and the exact lane served on the device, the vision canvas and dump-poison legs, the in-memory tower mint,
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
require daslib/rtti   // this_context().last_exception: a knob-off leg's refusal as text
require strings   // rtrim
require daslib/fio   // nolint:STYLE030,LINT019 — path_join: the oracle dump's path
require _compares   // logits_maxdiff
require _vision_oracle   // the mtmd dump parser and the canvas fixtures the poison legs read
require _model_tier   // tower_parity_stats - the rel_l2 statistic the audio twins' bar reads
require _gpu_knobs public
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_common   // vulkan_tower_tiles_on, g_gpu: the tile-family skip's words
require dasllama/dasllama_audio_io   // load_audio_16k_mono: the jfk cells' clip
require ?cuda dasllama/dasllama_cuda_tower   // cuda_tower_arms: the CUDA twin seat

let TWIN_SLACK_VK = 1.5   //! 1.5x: the q8 chain's activation quants plus f16 weight scales, the tiles' summation order
let TWIN_ABS_VK = 2e-4    //! the tier-1 dumps' absolute floor, the bar's other term
let TWIN_REL_L2_FLOOR = 1e-4lf   //! the audio twins' absolute floor over rel_l2, a unit-free ratio
let TWIN_EXACT_REL_L2_VK = 2e-2lf   //! the exact lane's bar over rel_l2 from the exact CPU chain: the f32 tile over the f32 rows (a vision tower) or the f16 GEMM over the halfwords (an audio tower), the attention's f16 panels and the tiles' order the distance
//! the whisper f16 lane's bar: the f16-table GELU over turbo's 32 blocks needs headroom past the shared bar (its readings: this
//! constant's row in `PERF_LEDGER.md`'s bar table)
let TWIN_F16_WHISPER_REL_L2 = 3.5e-2lf

//! a front's witness: the front served every dispatch on the device (an audio twin's seat stands on the f16 GEMM feed alone)
[unused_argument(declines_delta)]
def front_served(tt : T?; tag : string; convs_delta, expected, declines_delta : int64) {
    tt |> equal(convs_delta, expected, "{tag}: the front served every dispatch on the device (convs delta = {expected})")
}

//! the Metal twin's seat, or a loud skip: a das_metal build whose device stands up for family `fam`'s tower
[unused_argument(fam)]
def metal_twin_seat(tt : T?; fam : TowerFamily) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return true if (serving_tower_device_up(fam))
        tt |> skip("no Metal device stands up for the tower on this box")
        return false
    } else {
        tt |> skip("das_metal not present")
        return false
    }
}

//! the Vulkan twin's seat, or a loud skip: no seat where das_metal owns the tower hooks, none without the tier's want and a device it serves
def vulkan_twin_seat(tt : T?) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            tt |> skip("das_metal on this build owns the tower hooks - the Vulkan twin has no seat")
            return false
        } else {
            if (cuda_tower_asked()) {
                tt |> skip("the CUDA home is asked for (DASLLAMA_CUDA=1 or set_gpu_home) - the Vulkan tower's seats stand down under the CUDA ask; a family the CUDA home seats twins on the CUDA tower")
                return false
            }
            if (!vulkan_tower_arms()) {
                tt |> skip("no Vulkan device (DASLLAMA_GPU=1 and a device the tier serves)")
                return false
            }
            if (!vulkan_tower_tiles_on()) {
                tt |> skip("the tower's flash and f16 GEMM stamps need cm2 or the KHR tile at subgroup 32 (subgroup {g_gpu.subgroup_size} here) - the tower declines device")
                return false
            }
            return true
        }
    } else {
        tt |> skip("dasVulkan not present")
        return false
    }
}

//! a tower's CUDA twin seat: the CUDA home asked for and its device armed, each miss a loud skip
def cuda_twin_seat(tt : T?) : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (!cuda_tower_asked()) {
            tt |> skip("the CUDA home is not asked for - needs DASLLAMA_CUDA=1 beside DASLLAMA_GPU=1")
            return false
        }
        if (!cuda_tower_arms()) {
            tt |> skip("no CUDA device (DASLLAMA_GPU=1 DASLLAMA_CUDA=1 and a device the home serves)")
            return false
        }
        return true
    } else {
        tt |> skip("dasCUDA not present")
        return false
    }
}

//! a vision tower's twin seat on the serving discrete home: the CUDA tower's under the CUDA ask, else the Vulkan tower's
def discrete_twin_seat(tt : T?) : bool => cuda_tower_asked() ? cuda_twin_seat(tt) : vulkan_twin_seat(tt)

//! an audio tower's Vulkan twin seat: the Vulkan twin's, and the f16 GEMM feed its conv front rides - off it
//! (`DASLLAMA_COOPMAT=sdot4`) the load takes the CPU image, so no Vulkan image exists to twin
def vulkan_audio_twin_seat(tt : T?) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return false if (!vulkan_twin_seat(tt))
        return true if (vulkan_tower_front_route())
        tt |> skip("off the f16 GEMM feed an audio tower serves on its CPU image - no Vulkan image to twin")
        return false
    } else {
        return vulkan_twin_seat(tt)
    }
}

//! an audio family's twin seat on its seats' home: the CUDA tower's for a CUDA-home family (`tower_family_cuda`), else the Vulkan twin's, which stands down under the CUDA ask
def discrete_audio_twin_seat(tt : T?; fam : TowerFamily) : bool => tower_family_cuda(fam) ? cuda_twin_seat(tt) : vulkan_audio_twin_seat(tt)

[unused_argument(tt, model_path, jit_only, fam, metal, body)]
def private jfk_cell_seated(tt : T?; model_path : string; jit_only : bool; fam : TowerFamily; metal : bool; body : block<(samples : array<float>) : void>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda) || typeinfo builtin_module_exists(das_metal)) {
        if (jit_only && !jit_enabled()) {
            tt |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!model_available(tt, model_path)) {
            return
        }
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            tt |> skip("jfk.wav not present")
            return
        }
        if (!(metal ? metal_twin_seat(tt, fam) : discrete_audio_twin_seat(tt, fam))) {
            return
        }
        with_gpu_knobs() {
            invoke(body, samples)
        }
    }
}

//! a jfk twin cell's seat, each miss a loud skip (the JIT under `jit_only`, the model and jfk.wav, `fam`'s twin seat), then `body` over the clip, the knobs put back
def jfk_twin_cell(tt : T?; model_path : string; jit_only : bool; fam : TowerFamily; body : block<(samples : array<float>) : void>) {
    jfk_cell_seated(tt, model_path, jit_only, fam, false, body)
}

//! `jfk_twin_cell` on the Metal tower's seat: its device up
def metal_jfk_twin_cell(tt : T?; model_path : string; jit_only : bool; fam : TowerFamily; body : block<(samples : array<float>) : void>) {
    jfk_cell_seated(tt, model_path, jit_only, fam, true, body)
}


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
    seat_family : TowerFamily   //! its home serves the device leg
}

//! a three-way's driver counters - before the exact leg, after it, after the CPU q8 leg, after the device leg - and each leg's encode count
struct TwinLegs {
    pre : GpuTowerCounters
    exact : GpuTowerCounters
    cpu : GpuTowerCounters
    dev : GpuTowerCounters
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

//! a route counter's control: `encode` with family `fam`'s tower knob off moves no device encode (a flavored image's refusal of
//! the CPU recovered and logged)
def knob_off_unrouted(tt : T?; tag : string; fam : TowerFamily; encode : block<() : void>) {
    let e0 = gpu_tower_encodes(fam)
    var refusal = ""
    with_gpu_knobs() {
        set_serving_gpu_tower(false, fam)
        try {
            invoke(encode)
        } recover {
            refusal = this_context().last_exception |> rtrim
        }
    }
    let how = empty(refusal) ? "the encode ran off the device" : "refused - " + refusal
    to_log(LOG_INFO, "{tag} with the tower knob off: {how}\n")
    tt |> equal(gpu_tower_encodes(fam) - e0, 0l, "{tag}: with the tower knob off no encode reached the device (the route counter's control)")
}

//! a q8-lane leg on family `fam`'s GPU tower: where `encode`'s gate ran (true), one encode served, every block, no `quant_mode` decline
def q8_lane_served(tt : T?; tag : string; fam : TowerFamily; n_layer : int64; encode : block<() : bool>) {
    let s0 = gpu_tower_counters(fam)
    let dq0 = gpu_tower_quant_declines(fam)
    if (invoke(encode)) {
        device_leg_served(tt, tag, s0, gpu_tower_counters(fam), n_layer, -1l)
        tt |> equal(gpu_tower_quant_declines(fam) - dq0, 0l, "{tag}: no quant_mode decline")
    }
}

//! one input three ways (exact, CPU q8, the device over `tw_dev`'s GPU image), then `counters` and the bar: the device within `TWIN_SLACK_VK` x the CPU q8 distance
def twin_three_way(tt : T?; spec : TwinSpec; tw : auto(TW); var st : auto(ST); tw_dev : TW; var st_dev : ST; tw_exact : TW; var st_exact : ST;
                   encode : block<(t : TW; var s : ST; var out : array<float>) : int64>;
                   counters : block<(legs : TwinLegs) : void>) : TwinResult {
    var r = TwinResult(spec = spec)
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda) || typeinfo builtin_module_exists(das_metal)) {
        var cpu, dev : array<float>
        var legs = TwinLegs()
        let home = serving_tower_dev(spec.seat_family)
        with_gpu_knobs() {
            set_serving_gpu_tower(false, spec.seat_family)
            legs.pre = gpu_tower_counters(spec.seat_family)
            legs.n_exact = queued_encode(tw_exact, st_exact, r.reference, encode)
            legs.exact = gpu_tower_counters(spec.seat_family)
            legs.n_cpu = queued_encode(tw, st, cpu, encode)
            legs.cpu = gpu_tower_counters(spec.seat_family)
            set_serving_gpu_tower(true, spec.seat_family)
            legs.n_dev = queued_encode(tw_dev, st_dev, dev, encode)
            legs.dev = gpu_tower_counters(spec.seat_family)
        }
        if (legs.dev.encodes == legs.cpu.encodes) {
            to_log(LOG_INFO, "{spec.family} {home} twin {spec.tag}: the driver declined - {gpu_tower_declines(spec.seat_family)}\n")
        }
        invoke(counters, legs)
        if (spec.metric == TwinMetric.maxdiff) {
            r.rms = rms(r.reference)
            let q8_ex = logits_maxdiff(cpu, r.reference)
            let dev_ex = logits_maxdiff(dev, r.reference)
            let dev_q8 = logits_maxdiff(dev, cpu)
            let bar = TWIN_SLACK_VK * q8_ex + TWIN_ABS_VK
            r.bar = double(bar)
            to_log(LOG_INFO, "{spec.family} {home} twin {spec.tag} ({spec.n_layer} blocks{spec.log_suffix}): rms {r.rms}; vs exact: cpu q8 {q8_ex} ({q8_ex / r.rms} x rms), device {dev_ex} ({dev_ex / r.rms} x rms); device vs cpu q8 {dev_q8} ({dev_q8 / r.rms} x rms)\n")
            tt |> success(dev_ex <= bar, "{spec.tag}: the device chain sits within {TWIN_SLACK_VK}x the CPU q8 chain's distance from the exact chain ({dev_ex} vs {q8_ex})")
        } else {
            r.bar = tower_twin_bar(tt, spec.tag, spec.seat_family, r.reference, cpu, dev, spec.n_layer, double(TWIN_SLACK_VK), TWIN_REL_L2_FLOOR)
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
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda) || typeinfo builtin_module_exists(das_metal)) {
        var inscope poisoned <- device_poison_leg(tt, r.spec.tag, twp, stp, encode, r.spec.seat_family)
        if (r.spec.metric == TwinMetric.maxdiff) {
            let poison_ex = logits_maxdiff(poisoned, r.reference)
            to_log(LOG_INFO, "{r.spec.family} {serving_tower_dev(r.spec.seat_family)} twin {r.spec.tag} ({r.spec.n_layer} blocks) poison: block {zero_block} zeroed on the device lands {poison_ex} from the exact chain ({poison_ex / r.rms} x rms), bar {r.bar}\n")
            tt |> success(double(poison_ex) > r.bar, "{r.spec.tag}: a zeroed block through the device chain EXCEEDS the bar ({poison_ex} vs {r.bar})")
        } elif (r.bar > 0.0lf) {
            tower_twin_poisoned(tt, r.spec.tag, r.spec.seat_family, r.reference, poisoned, r.bar)
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

//! a poison's device leg: `encode` over the zeroed tower `twp` with family `fam`'s tower knob on, one device encode asserted; the rows it wrote
def private device_poison_leg(tt : T?; tag : string; twp : auto(TW); var stp : auto(ST); encode : block<(t : TW; var s : ST; var out : array<float>) : int64>; fam : TowerFamily) : array<float> {
    var inscope out : array<float>
    let e0 = gpu_tower_encodes(fam)
    with_gpu_knobs() {
        set_serving_gpu_tower(true, fam)
        queued_encode(twp, stp, out, encode)
    }
    tt |> equal(gpu_tower_encodes(fam) - e0, 1l, "{tag} poison: device-served (encodes delta)")
    return <- out
}

//! a vision canvas three ways on the patch grid's counters - the device leg over the Vulkan-flavored q8 tower `mint_dev(-1)`
//! returns (`#tower-dev-block`) - then, `zero_block` >= 0, the input poison over the one `mint_dev` returns with that block
//! zeroed; `encode` is the family's encode of the canvas
def twin_canvas(tt : T?; family, kind : string; w, h : int; patch : int64; log_suffix : string; tw : auto(TW); var st : auto(ST);
                tw_exact : TW; var st_exact : ST; zero_block : int64; mint_dev : block<(zb : int64) : TW>;
                encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let spec = TwinSpec(family = family, tag = "{kind} {w}x{h}", n_layer = tw.n_layer, log_suffix = log_suffix, metric = TwinMetric.rel_l2, seat_family = TowerFamily.vision)
        let npos = (int64(w) / patch) * (int64(h) / patch)
        var tw_dev <- invoke(mint_dev, -1l)
        var st_dev = ST()
        tt |> equal(tw_dev.dev_backend, serving_tower_dev(TowerFamily.vision), "{spec.tag}: the device leg's tower is the serving home's image ({serving_tower_dev(TowerFamily.vision)})")
        let r <- twin_three_way(tt, spec, tw, st, tw_dev, st_dev, tw_exact, st_exact, encode) $(legs) {
            twin_counters(tt, spec.tag, legs, spec.n_layer, npos)
        }
        delete st_dev
        delete tw_dev
        if (zero_block >= 0l) {
            var twp <- invoke(mint_dev, zero_block)
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
    var inscope out <- device_poison_leg(tt, tag, twp, stp, encode, TowerFamily.vision)
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

//! a tower minted in memory from the planes `stage` reads from `mm` under the family's lane pin (`pin(q8)`, `unpin` restoring the
//! policy), cut and poisoned as `mint_staged_tower` cuts it, in the image flavor `dev` (`#tower-dev-block`): `none` the CPU image,
//! a GPU flavor staged row-major and baked by the family's registered driver - each backend reads only its own
def mint_twin_tower(mm : string; q8 : bool; nl, zero_block : int64; dev : TowerDev; pin : function<(on : bool) : void>; unpin : function<() : void>;
                    stage : function<(path : string) : auto(STG)>; mint : function<(var s : STG; var t : auto(TW); dev : TowerDev) : void>) : TW {
    invoke(pin, q8)
    defer() {
        invoke(unpin)
    }
    var inscope stg <- tower_dev_staged(dev) $ { return <- invoke(stage, mm) }
    truncate_and_poison(stg, nl, zero_block)
    var tw = TW()
    invoke(mint, stg, tw, dev)
    return <- tw
}

//! a tower's block count, image flavor and halfword exact lane: its own, or - where the blocks are a nested whisper-class tower's
//! (qwen3a) - its core's, the halfword lane there both the core's GEMM set and the front trio's; a tower with no halfword GEMM
//! set (a vision tower: its exact lane is the f32 tile) answers false
def twin_facts(tw) : tuple<n_layer : int64; dev : TowerDev; f16 : bool> {
    static_if (typeinfo safe_has_field < core > (tw)) {
        return (n_layer = tw.core.n_layer, dev = tw.core.dev_backend, f16 = tw.core.gemm_f16 && tw.cf16)
    } static_elif (typeinfo safe_has_field < gemm_f16 > (tw)) {
        return (n_layer = tw.n_layer, dev = tw.dev_backend, f16 = tw.gemm_f16)
    } else {
        return (n_layer = tw.n_layer, dev = tw.dev_backend, f16 = false)
    }
}

//! `exact_lane_served_on` held to `bar` over rel_l2 - a family whose exact lane the measured rows hold to its own bar
def exact_lane_served_at(tt : T?; tag : string; fam : TowerFamily; bar : double; tw_cpu : auto(TW); var st_cpu : auto(ST); tw_dev : TW; var st_dev : ST;
                         npos : int64; twp : TW; var stp : ST; encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda) || typeinfo builtin_module_exists(das_metal)) {
        var cpu, dev : array<float>
        var n_cpu = 0l
        var n_dev = 0l
        var s0 = GpuTowerCounters()
        var s1 = GpuTowerCounters()
        var dq = 0l
        with_gpu_knobs() {
            set_serving_gpu_tower(false, fam)
            n_cpu = queued_encode(tw_cpu, st_cpu, cpu, encode)
            set_serving_gpu_tower(true, fam)
            s0 = gpu_tower_counters(fam)
            let dq0 = gpu_tower_quant_declines(fam)
            n_dev = queued_encode(tw_dev, st_dev, dev, encode)
            s1 = gpu_tower_counters(fam)
            dq = gpu_tower_quant_declines(fam) - dq0
        }
        if (s1.encodes == s0.encodes) {
            to_log(LOG_INFO, "{tag} exact lane: the driver declined - {gpu_tower_declines(fam)}\n")
        }
        tt |> equal(n_cpu, n_dev, "{tag} exact lane: token count on both chains")
        device_leg_served(tt, "{tag} exact lane", s0, s1, twin_facts(tw_dev).n_layer, npos)
        tt |> equal(dq, 0l, "{tag} exact lane: no quant_mode decline")
        tower_twin_held(tt, "{tag} exact lane", fam, cpu, dev, bar)
        var nsq = 0.0lf
        for (v in cpu) {
            nsq += double(v) * double(v)
        }
        var moved := cpu
        moved[0] += float(2.0lf * bar * sqrt(nsq) + double(abs(dev[0] - cpu[0])) + 1.0lf)
        var stm = TowerParityStats()
        if (tower_parity_stats(tt, "{tag} exact-lane control", moved, dev, stm)) {
            tt |> success(stm.rel_l2 > bar, "{tag} exact lane control: the expectation moved past the bar at one element reds it ({stm.rel_l2})")
        }
        delete moved
        var inscope poisoned <- device_poison_leg(tt, "{tag} exact lane", twp, stp, encode, fam)
        tower_twin_poisoned(tt, "{tag} exact lane", fam, cpu, poisoned, bar)
        delete cpu
        delete dev
    }
}

//! a tower's exact lane on `fam`'s device: `tw_cpu` on the CPU vs `tw_dev` within `TWIN_EXACT_REL_L2_VK`, a moved element the control; `twp`'s poison must EXCEED the bar
def exact_lane_served_on(tt : T?; tag : string; fam : TowerFamily; tw_cpu : auto(TW); var st_cpu : auto(ST); tw_dev : TW; var st_dev : ST; npos : int64;
                         twp : TW; var stp : ST; encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    exact_lane_served_at(tt, tag, fam, TWIN_EXACT_REL_L2_VK, tw_cpu, st_cpu, tw_dev, st_dev, npos, twp, stp, encode)
}

//! a vision tower's exact lane served on the device - `exact_lane_served_on` with the exact tower on both legs, the vision home's
def exact_lane_served(tt : T?; tag : string; tw_exact : auto(TW); var st_exact : auto(ST); npos : int64; twp : TW; var stp : ST;
                      encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    exact_lane_served_on(tt, tag, TowerFamily.vision, tw_exact, st_exact, tw_exact, st_exact, npos, twp, stp, encode)
}

//! a tower's exact lane on `fam`'s home at `bar`: `tw_exact`'s CPU chain against `mint(-1)`'s halfword flavor, the poison `mint(zero_block)`; answers the front's dispatches
def exact_flavor_leg(tt : T?; tag : string; fam : TowerFamily; bar : double; tw_exact : auto(TW); var st_exact : auto(ST); npos : int64;
                     mint : block<(zb : int64) : TW>; state : block<(tw : TW) : ST>;
                     encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) : int64 {
    var inscope twd <- invoke(mint, -1l)
    var inscope twp <- invoke(mint, twin_facts(tw_exact).n_layer / 2l)
    let home = serving_tower_dev(fam)
    tt |> equal(twin_facts(twd).dev, home, "{tag}: the exact lane's device leg is the {home} flavor's image")
    tt |> success(twin_facts(twd).f16 && twin_facts(twp).f16, "{tag}: the exact lane's GEMMs are halfwords on the {home} home")
    var inscope std <- invoke(state, twd)
    var inscope stp <- invoke(state, twp)
    let c0 = gpu_tower_counters(fam).convs
    exact_lane_served_at(tt, tag, fam, bar, tw_exact, st_exact, twd, std, npos, twp, stp, encode)
    return gpu_tower_counters(fam).convs - c0
}

//! `exact_flavor_leg` at the shared bar, each state a fresh one
def exact_flavor_leg(tt : T?; tag : string; fam : TowerFamily; tw_exact : auto(TW); var st_exact : auto(ST); npos : int64;
                     mint : block<(zb : int64) : TW>; encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) : int64
    => exact_flavor_leg(tt, tag, fam, TWIN_EXACT_REL_L2_VK, tw_exact, st_exact, npos, mint, $ [unused_argument(tw)] (tw : TW) => ST(), encode)

//! the device's rel_l2 from the exact chain within `slack` x the CPU q8 chain's own plus `floor`; returns the bar, 0 when a compare could not run
def tower_twin_bar(t : T?; tag : string; fam : TowerFamily; exact, cpu, dev : array<float>; n_layer : int64; slack, floor : double) : double {
    var q8_ex = TowerParityStats()
    var dev_ex = TowerParityStats()
    var dev_q8 = TowerParityStats()
    let ok = (tower_parity_stats(t, "{tag} cpu q8 vs exact", exact, cpu, q8_ex)
        && tower_parity_stats(t, "{tag} device vs exact", exact, dev, dev_ex)
        && tower_parity_stats(t, "{tag} device vs cpu q8", cpu, dev, dev_q8))
    if (!ok) {
        return 0.0lf
    }
    let bar = slack * q8_ex.rel_l2 + floor
    t |> success(dev_ex.rel_l2 <= bar, "{tag} device blocks {n_layer}L on {serving_tower_dev(fam)}: the device chain sits within {slack}x the CPU q8 chain's rel_l2 from the exact chain ({dev_ex.rel_l2} vs {q8_ex.rel_l2})")
    return bar
}

//! a device encode's rel_l2 from the exact chain held within `bar`, the encode on family `fam`'s home
def tower_twin_held(t : T?; tag : string; fam : TowerFamily; exact, dev : array<float>; bar : double) {
    tower_twin_verdict(t, tag, fam, exact, dev, bar, false)
}

//! the zeroed-plane input poison: a device encode's rel_l2 from the exact chain past `bar`, the encode on family `fam`'s home
def tower_twin_poisoned(t : T?; tag : string; fam : TowerFamily; exact, dev : array<float>; bar : double) {
    tower_twin_verdict(t, tag, fam, exact, dev, bar, true)
}

def private tower_twin_verdict(t : T?; tag : string; fam : TowerFamily; exact, dev : array<float>; bar : double; must_exceed : bool) {
    var st = TowerParityStats()
    return if (!tower_parity_stats(t, must_exceed ? "{tag} poisoned device vs exact" : "{tag} device vs exact", exact, dev, st))
    let home = serving_tower_dev(fam)
    if (must_exceed) {
        t |> success(all_finite(exact) && all_finite(dev), "{tag}: every element of both encodes is finite before the {home} poison's exceed compare")
        t |> success(is_finite(st.rel_l2) && st.rel_l2 > bar, "{tag}: the zeroed planes through the {home} device chain EXCEED the bar ({st.rel_l2} vs {bar})")
    } else {
        t |> success(st.rel_l2 <= bar, "{tag}: the {home} device chain sits within the bar ({st.rel_l2} vs {bar})")
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
