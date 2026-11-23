// The GPU homes' tower twins' shared instrument - the seat guard on the home each family's seats serve on, the three-way encode (the exact chain, the CPU q8 chain,
// the device chain) with the family's counters and the bar by metric (rel_l2 wherever a CPU q8 chain is the control,
// qwen25v's maxdiff against the exact chain), the input poison and the exact lane served on the device - each as legs that keep
// their rows for a compare after a file's block (`TwinKept`, `TwinRows`, `ExactLaneKept`, `ExactFlavorKept`) - the
// dump-poison legs, a minted form's staging, the GPU knobs' capture and restore (`_gpu_knobs`, re-exported) - one text the vision and audio families' test files call with their own tower, state and encode; no fixture
// root beside it.

options gen2
options _dasllama_internal = true

module _tower_twin shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require _f32_tile_pin   // with_put_back: a staging's lane pin put back however the stage leaves
require daslib/rtti   // this_context().last_exception: a knob-off leg's refusal as text
require strings   // rtrim
require daslib/strings_boost   // split: a form's name
require daslib/fio   // nolint:STYLE030,LINT019 — path_join: the oracle dump's path
require _compares   // logits_maxdiff
require _vision_oracle   // the mtmd dump parser and the canvas fixtures the poison legs read
require _model_tier   // tower_parity_stats - the rel_l2 statistic the audio twins' bar reads
require _gpu_knobs public
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_common   // vulkan_tower_tiles_on, g_gpu: the tile-family skip's words
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

//! what the box lacks for the Metal twin's seat, "" where it stands: a das_metal build whose device stands up for family
//! `fam`'s tower
[unused_argument(fam)]
def metal_twin_seat_why(fam : TowerFamily) : string {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return serving_tower_device_up(fam) ? "" : "no Metal device stands up for the tower on this box"
    } else {
        return "das_metal not present"
    }
}

//! what the box lacks for the Metal twin of family `fam` that runs under the JIT alone, "" where it runs: the twin's seat
//! (`metal_twin_seat_why`), then the JIT
def metal_twin_jit_why(fam : TowerFamily) : string {
    let seat = metal_twin_seat_why(fam)
    return seat if (!empty(seat) || jit_enabled())
    return "interpreted (dasLLAMA model tests are JIT-only)"
}

//! the Metal twin's seat, or a loud skip (`metal_twin_seat_why`)
def metal_twin_seat(tt : T?; fam : TowerFamily) : bool => seat_or_skip(tt, metal_twin_seat_why(fam))

//! a seat that stands where `why` is empty, else the loud skip naming it
def private seat_or_skip(tt : T?; why : string) : bool {
    return true if (empty(why))
    tt |> skip(why)
    return false
}

//! what the box lacks for the Vulkan twin's seat, "" where it stands: no seat where das_metal owns the tower hooks, none
//! without the tier's want and a device it serves
def vulkan_twin_seat_why() : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return "das_metal on this build owns the tower hooks - the Vulkan twin has no seat"
        } else {
            return "the CUDA home is asked for (DASLLAMA_CUDA=1 or set_gpu_home) - the Vulkan tower's seats stand down under the CUDA ask; a family the CUDA home seats twins on the CUDA tower" if (cuda_tower_asked())
            return "no Vulkan device (DASLLAMA_GPU=1 and a device the tier serves)" if (!vulkan_tower_arms())
            return "the tower's flash and f16 GEMM stamps need cm2 or the KHR tile at subgroup 32 (subgroup {g_gpu.subgroup_size} here) - the tower declines device" if (!vulkan_tower_tiles_on())
            return ""
        }
    } else {
        return "dasVulkan not present"
    }
}

//! the Vulkan twin's seat, or a loud skip (`vulkan_twin_seat_why`)
def vulkan_twin_seat(tt : T?) : bool => seat_or_skip(tt, vulkan_twin_seat_why())

//! what the box lacks for a tower's CUDA twin seat, "" where it stands: the CUDA home asked for and its device armed
def cuda_twin_seat_why() : string {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return "the CUDA home is not asked for - needs DASLLAMA_CUDA=1 beside DASLLAMA_GPU=1" if (!cuda_tower_asked())
        return "no CUDA device (DASLLAMA_GPU=1 DASLLAMA_CUDA=1 and a device the home serves)" if (!cuda_tower_arms())
        return ""
    } else {
        return "dasCUDA not present"
    }
}

//! a tower's CUDA twin seat, or a loud skip (`cuda_twin_seat_why`)
def cuda_twin_seat(tt : T?) : bool => seat_or_skip(tt, cuda_twin_seat_why())

//! what the box lacks for a vision tower's twin seat on the serving discrete home, "" where it stands: the CUDA tower's
//! under the CUDA ask, else the Vulkan tower's
def discrete_twin_seat_why() : string => cuda_tower_asked() ? cuda_twin_seat_why() : vulkan_twin_seat_why()

//! a vision tower's twin seat on the serving discrete home, or a loud skip (`discrete_twin_seat_why`)
def discrete_twin_seat(tt : T?) : bool => seat_or_skip(tt, discrete_twin_seat_why())

//! what the box lacks for an audio tower's Vulkan twin seat, "" where it stands: the Vulkan twin's, and the f16 GEMM feed
//! its conv front rides - off it (`DASLLAMA_COOPMAT=sdot4`) the load takes the CPU image, so no Vulkan image exists to twin
def vulkan_audio_twin_seat_why() : string {
    let why = vulkan_twin_seat_why()
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return why if (!empty(why) || vulkan_tower_front_route())
        return "off the f16 GEMM feed an audio tower serves on its CPU image - no Vulkan image to twin"
    } else {
        return why
    }
}

//! an audio tower's Vulkan twin seat, or a loud skip (`vulkan_audio_twin_seat_why`)
def vulkan_audio_twin_seat(tt : T?) : bool => seat_or_skip(tt, vulkan_audio_twin_seat_why())

//! what the box lacks for an audio family's twin seat on its seats' home, "" where it stands: the CUDA tower's for a
//! CUDA-home family (`tower_family_cuda`), else the Vulkan twin's, which stands down under the CUDA ask
def discrete_audio_twin_seat_why(fam : TowerFamily) : string => tower_family_cuda(fam) ? cuda_twin_seat_why() : vulkan_audio_twin_seat_why()

//! an audio family's twin seat on its seats' home, or a loud skip (`discrete_audio_twin_seat_why`)
def discrete_audio_twin_seat(tt : T?; fam : TowerFamily) : bool => seat_or_skip(tt, discrete_audio_twin_seat_why(fam))

//! A tower's form, read off its name. `image`, `image_f32` and `image_q8` are the family's own load through the image
//! rail: with no lane pinned (the facade's load), on the exact lane, and on the q8 lane with every GPU tower knob off for
//! the load (the lane's CPU image). Any other name is a tower minted in memory from the file's staging, no `.dlim` baked -
//! `<lane>[_<n>L][_z<block>][@dev|@seat][+<edit>]`: the lane one of the family's own (`f32` or `q8` for a vision tower);
//! cut to `n` blocks; block `block`'s GEMM planes zeroed, the input poison; `+<edit>` a change to the staged planes the
//! family's loader knows by that name. A form with no home is the CPU image the CPU chain's cells read; `@dev` is the
//! serving home's image flavor and `@seat` the CPU image again, each the copy the serving home's device chain reads.
struct TowerForm {
    image : bool
    lane : string   //! one of the family's lanes, or "" for the unpinned image load
    nl : int64
    zero_block : int64 = -1l
    dev : bool    //! minted in the serving home's image flavor
    seat : bool   //! the CPU image, the serving home's device chain reading it as stored
    edit : string
}

//! the staged read the tower form `f` is minted from (`staged_once_at`): none on an image-rail form, else its lane and
//! image flavor, named as the form `<lane>` or `<lane>@dev` - the cut, the poison and the edit are the form's own
def tower_stage_key_of(f : TowerForm) : string {
    return "" if (f.image)
    return f.dev ? "{f.lane}@dev" : f.lane
}

//! the image flavor of family `fam`'s minted tower form `f`: the serving home's under `@dev`, else the CPU image
def tower_form_dev_of(f : TowerForm; fam : TowerFamily) : TowerDev => f.dev ? serving_tower_dev(fam) : TowerDev.none

//! a minted form `f` (named `form`, of `file`) of family `fam` holds the poison and the flavor it names: its zeroed block
//! the middle one of the tower's `n_layer`, and `dev` the image flavor `@dev` names (`tower_form_dev_of`)
def form_zero_and_flavor_is(t : T?; file, form : string; f : TowerForm; fam : TowerFamily; n_layer : int64; dev : TowerDev) {
    if (f.zero_block >= 0l) {
        t |> equal(f.zero_block, n_layer / 2l, "{file}: the {form} form zeroes from the middle block on")
    }
    t |> equal(dev, tower_form_dev_of(f, fam), "{file}: the {form} form is the image flavor it names")
}

//! a minted form `f` (named `form`, of `file`) of family `fam` is the lane, poison and image flavor it names: the tower's
//! lane flag `q8`, then `form_zero_and_flavor_is`
def minted_form_is(t : T?; file, form : string; f : TowerForm; fam : TowerFamily; q8 : bool; n_layer : int64; dev : TowerDev) {
    t |> equal(q8, f.lane == "q8", "{file}: the {form} form serves the lane it names")
    form_zero_and_flavor_is(t, file, form, f, fam, n_layer, dev)
}

//! the form `form` names among the lanes `lanes`; a name of no form panics, naming the `area`'s tests
def tower_form_of(form : string; lanes : array<string>; area : string) : TowerForm {
    var f = TowerForm()
    var inscope edits <- split(form, "+")
    var inscope homes <- split(edits[0], "@")
    var inscope parts <- split(homes[0], "_")
    f.edit = length(edits) == 2 ? edits[1] : ""
    f.dev = length(homes) == 2 && homes[1] == "dev"
    f.seat = length(homes) == 2 && homes[1] == "seat"
    f.image = parts[0] == "image"
    f.lane = f.image ? (length(parts) == 2 ? parts[1] : "") : parts[0]
    var known = (length(edits) <= 2 && (length(homes) == 1 || f.dev || f.seat) && (has_value(lanes, f.lane) || (f.image && length(parts) == 1))
        && (!f.image || (length(parts) <= 2 && length(homes) == 1 && empty(f.edit))))
    for (p, i in parts, count()) {
        continue if (i == 0 || f.image)
        if (p |> ends_with("L")) {
            f.nl = to_int64(slice(p, 0, length(p) - 1), false)
            known = known && f.nl > 0l
        } elif (p |> starts_with("z")) {
            f.zero_block = to_int64(slice(p, 1), false)
        } else {
            known = false
        }
    }
    if (!known) {
        panic("the {area} tests: '{form}' names no tower form")
    }
    return f
}

//! a jfk twin cell's seat, each miss a loud skip (the JIT under `jit_only`, the model and jfk.wav, `fam`'s twin seat), then `body` over the clip, the knobs put back
[unused_argument(tt, model_path, jit_only, fam, body)]
def jfk_twin_cell(tt : T?; model_path : string; jit_only : bool; fam : TowerFamily; body : block<(samples : array<float>) : void>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda) || typeinfo builtin_module_exists(das_metal)) {
        if (jit_only && !jit_enabled()) {
            tt |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!model_available(tt, model_path)) {
            return
        }
        let samples <- asr_samples(tt, "jfk.wav")
        return if (empty(samples) || !discrete_audio_twin_seat(tt, fam))
        with_gpu_knobs() {
            invoke(body, samples)
        }
    }
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

//! a twin's legs by the driver's counters: both chains' token counts equal; the knob-off leg, between `cpu0` and `cpu1`,
//! never dispatched and counted its decline; the device leg, between `dev0` and `dev1`, served one encode, every block, the
//! patch grid's rows
def twin_counted(tt : T?; tag : string; n_cpu, n_dev : int64; cpu0, cpu1, dev0, dev1 : GpuTowerCounters; n_layer, npos : int64) {
    tt |> equal(n_cpu, n_dev, "{tag}: token count on both chains")
    tt |> equal(cpu1.encodes - cpu0.encodes, 0l, "{tag}: the knob-off leg never dispatched")
    tt |> equal(cpu1.declines - cpu0.declines, 1l, "{tag}: the knob-off leg recorded the knob decline")
    device_leg_served(tt, tag, dev0, dev1, n_layer, npos)
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

//! a leg of a twin's input: the exact chain, the CPU q8 chain, the device chain
enum TwinLeg {
    exact
    cpu
    dev
}

//! One input of an audio tower's twin as the forms of its file keep it, a leg a form (`twin_rows_leg`), for the compare
//! after the file's block (`twin_rows_held`): each chain's rows, each leg's encode count and the driver's counters across
//! that leg alone, and the device chain's rows over a tower with `zero_block` zeroed where the twin takes the input poison.
struct TwinRows {
    spec : TwinSpec
    zero_block : int64 = -1l
    exact : array<float>
    cpu : array<float>
    dev : array<float>
    poisoned : array<float>
    n_exact : int64
    n_cpu : int64
    n_dev : int64
    exact_moved : GpuTowerCounters
    cpu_moved : GpuTowerCounters
    dev_moved : GpuTowerCounters
    legs : int   //! the clean legs kept so far
}

//! the counters `after` less the counters `before`
def private counters_moved(after, before : GpuTowerCounters) : GpuTowerCounters =>
    (encodes = after.encodes - before.encodes, rows = after.rows - before.rows, blocks = after.blocks - before.blocks,
        declines = after.declines - before.declines, convs = after.convs - before.convs, mels = after.mels - before.mels)

//! the counters `a` and `b` summed
def private counters_summed(a, b : GpuTowerCounters) : GpuTowerCounters =>
    (encodes = a.encodes + b.encodes, rows = a.rows + b.rows, blocks = a.blocks + b.blocks,
        declines = a.declines + b.declines, convs = a.convs + b.convs, mels = a.mels + b.mels)

//! The leg `leg` of the twin `k`, on the form's tower `tw`: `encode` with the family's tower knob off (the exact chain, the
//! CPU q8 chain) or on (the device chain), the rows, the encode's count and the counters the leg moved kept.
def twin_rows_leg(var k : TwinRows; leg : TwinLeg; tw : auto(TW); var st : auto(ST);
                  encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    let fam = k.spec.seat_family
    var moved : GpuTowerCounters
    with_gpu_knobs() {
        set_serving_gpu_tower(leg == TwinLeg.dev, fam)
        let s0 = gpu_tower_counters(fam)
        if (leg == TwinLeg.exact) {
            k.n_exact = queued_encode(tw, st, k.exact, encode)
        } elif (leg == TwinLeg.cpu) {
            k.n_cpu = queued_encode(tw, st, k.cpu, encode)
        } else {
            k.n_dev = queued_encode(tw, st, k.dev, encode)
        }
        moved = counters_moved(gpu_tower_counters(fam), s0)
    }
    if (leg == TwinLeg.exact) {
        k.exact_moved = moved
    } elif (leg == TwinLeg.cpu) {
        k.cpu_moved = moved
    } else {
        k.dev_moved = moved
        if (moved.encodes == 0l) {
            to_log(LOG_INFO, "{k.spec.family} {serving_tower_dev(fam)} twin {k.spec.tag}: the driver declined - {gpu_tower_declines(fam)}\n")
        }
    }
    k.legs++
}

//! the input poison's leg of the twin `k`, on the form's tower `twp` with `zero_block` zeroed: the device chain's rows kept,
//! one device encode asserted
def twin_rows_poison_leg(tt : T?; var k : TwinRows; zero_block : int64; twp : auto(TW); var stp : auto(ST);
                         encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    k.zero_block = zero_block
    k.poisoned <- device_poison_leg(tt, k.spec.tag, twp, stp, encode, k.spec.seat_family)
}

//! the counters of `k`'s legs as a three-way's `counters` block reads them: each snapshot the legs' own moves summed in
//! the legs' order from zero, so the difference of two neighbours is one leg's counters whatever ran between the forms
def twin_rows_legs(k : TwinRows) : TwinLegs {
    var legs = TwinLegs(n_exact = k.n_exact, n_cpu = k.n_cpu, n_dev = k.n_dev)
    legs.exact = counters_summed(legs.pre, k.exact_moved)
    legs.cpu = counters_summed(legs.exact, k.cpu_moved)
    legs.dev = counters_summed(legs.cpu, k.dev_moved)
    return legs
}

//! The twin `k` held, after its file's block: its three clean legs kept, `counters` over the legs' counters
//! (`twin_rows_legs`), the device chain within the bar the CPU q8 chain sets from the exact chain (`twin_bar`), and the
//! input poison past it where a poison leg ran. Answers the exact chain's rms and the bar, zeros where a leg is missing.
def twin_rows_held(tt : T?; k : TwinRows; counters : block<(legs : TwinLegs) : void>) : tuple<rms : float; bar : double> {
    tt |> equal(k.legs, 3, "{k.spec.tag}: the twin kept its exact, CPU q8 and device legs")
    return (rms = 0.0, bar = 0.0lf) if (k.legs != 3)
    invoke(counters, twin_rows_legs(k))
    let held = twin_bar(tt, k.spec, k.exact, k.cpu, k.dev)
    if (k.zero_block >= 0l) {
        twin_poison_held(tt, k.spec, held.rms, held.bar, k.zero_block, k.exact, k.poisoned)
    }
    return held
}

//! a twin's bar by `spec.metric` over the three chains' rows: the device chain held within `TWIN_SLACK_VK` x the CPU q8
//! chain's distance from the exact chain `reference`; the reference's rms (a maxdiff twin's unit) and the bar it held to
//! (0 where a rel_l2 compare could not run)
def twin_bar(tt : T?; spec : TwinSpec; reference, cpu, dev : array<float>) : tuple<rms : float; bar : double> {
    if (spec.metric == TwinMetric.maxdiff) {
        let home = serving_tower_dev(spec.seat_family)
        let ref_rms = rms(reference)
        let q8_ex = logits_maxdiff(cpu, reference)
        let dev_ex = logits_maxdiff(dev, reference)
        let dev_q8 = logits_maxdiff(dev, cpu)
        let bar = TWIN_SLACK_VK * q8_ex + TWIN_ABS_VK
        to_log(LOG_INFO, "{spec.family} {home} twin {spec.tag} ({spec.n_layer} blocks{spec.log_suffix}): rms {ref_rms}; vs exact: cpu q8 {q8_ex} ({q8_ex / ref_rms} x rms), device {dev_ex} ({dev_ex / ref_rms} x rms); device vs cpu q8 {dev_q8} ({dev_q8 / ref_rms} x rms)\n")
        tt |> success(dev_ex <= bar, "{spec.tag}: the device chain sits within {TWIN_SLACK_VK}x the CPU q8 chain's distance from the exact chain ({dev_ex} vs {q8_ex})")
        return (rms = ref_rms, bar = double(bar))
    }
    return (rms = 0.0, bar = tower_twin_bar(tt, spec.tag, spec.seat_family, reference, cpu, dev, spec.n_layer, double(TWIN_SLACK_VK), TWIN_REL_L2_FLOOR))
}

//! the input poison's verdict: the rows `poisoned` - the device chain's over a tower with `zero_block` zeroed - land past the
//! bar `bar` the clean chains set from `reference` (a rel_l2 bar of 0 holds nothing, so that compare is skipped)
def twin_poison_held(tt : T?; spec : TwinSpec; ref_rms : float; bar : double; zero_block : int64; reference, poisoned : array<float>) {
    if (spec.metric == TwinMetric.maxdiff) {
        let poison_ex = logits_maxdiff(poisoned, reference)
        to_log(LOG_INFO, "{spec.family} {serving_tower_dev(spec.seat_family)} twin {spec.tag} ({spec.n_layer} blocks) poison: block {zero_block} zeroed on the device lands {poison_ex} from the exact chain ({poison_ex / ref_rms} x rms), bar {bar}\n")
        tt |> success(all_finite(reference) && all_finite(poisoned), "{spec.tag}: every element of both encodes is finite before the poison's exceed compare")
        tt |> success(is_finite(poison_ex) && double(poison_ex) > bar, "{spec.tag}: a zeroed block through the device chain EXCEEDS the bar ({poison_ex} vs {bar})")
    } elif (bar > 0.0lf) {
        tower_twin_poisoned(tt, spec.tag, spec.seat_family, reference, poisoned, bar)
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

//! one canvas of a vision tower's twin as the forms of its file keep it - each chain's rows, the token counts and the
//! driver's counters around the knob-off and the device legs - for the compare after the file's block (`twin_kept_held`).
//! The canvas is `kind` at `w` x `h` on a tower cut to `nl` blocks (0 = the file's count); `zero_block` >= 0 adds the input
//! poison's leg
struct TwinKept {
    nl : int64
    kind : string
    w : int
    h : int
    zero_block : int64 = -1l
    n_layer : int64
    exact : array<float>
    cpu : array<float>
    dev : array<float>
    poisoned : array<float>
    n_cpu : int64
    n_dev : int64
    cpu0 : GpuTowerCounters
    cpu1 : GpuTowerCounters
    dev0 : GpuTowerCounters
    dev1 : GpuTowerCounters
    legs : int   //! the legs kept so far
}

//! a twin's canvas, no leg kept yet
def twin_canvas(nl : int64; kind : string; w, h : int; zero_block : int64 = -1l) : TwinKept =>
    TwinKept(nl = nl, kind = kind, w = w, h = h, zero_block = zero_block)

//! the canvas as its asserts and log lines tag it
def twin_tag(k : TwinKept) : string => "{k.kind} {k.w}x{k.h}"

//! The legs one form of a vision tower's file adds to its twin's canvases `kept`: on each canvas cut to the form's `nl`
//! blocks, `encode` of the canvas's planes over the form's tower `tw` - with the tower knob off the exact chain (`q8`
//! false) or the knob-off chain the device answers to (`q8`: a lane family's CPU q8 chain, the counters around it kept);
//! with the knob on the device chain (`dev`, the counters kept), or, on a form with `zero_block` zeroed, the input poison
//! of the canvases that take that block. `n_layer` is the tower's block count.
def twin_keep_legs(tt : T?; var kept : array<TwinKept>; nl, n_layer : int64; q8, dev : bool; zero_block : int64; tw : auto(TW); var st : auto(ST);
                   encode : block<(t : TW; var s : ST; planes : array<float>; w, h : int; var out : array<float>) : int64>) {
    let fam = TowerFamily.vision
    for (k in kept) {
        continue if (k.nl != nl || (zero_block >= 0l && k.zero_block != zero_block))
        let planes <- make_planes(k.kind, k.w, k.h)
        let w = k.w
        let h = k.h
        k.legs++
        if (zero_block >= 0l) {
            k.poisoned <- device_poison_leg(tt, twin_tag(k), tw, st, $(t; var s; var out) => invoke(encode, t, s, planes, w, h, out), fam)
            continue
        }
        with_gpu_knobs() {
            set_serving_gpu_tower(dev, fam)
            let s0 = gpu_tower_counters(fam)
            var inscope rows : array<float>
            let n = queued_encode(tw, st, rows, $(t; var s; var out) => invoke(encode, t, s, planes, w, h, out))
            let s1 = gpu_tower_counters(fam)
            if (dev) {
                if (s1.encodes == s0.encodes) {
                    to_log(LOG_INFO, "{serving_tower_dev(fam)} twin {twin_tag(k)}: the driver declined - {gpu_tower_declines(fam)}\n")
                }
                k.dev <- rows
                k.n_dev = n
                k.dev0 = s0
                k.dev1 = s1
                k.n_layer = n_layer
            } elif (q8) {
                k.cpu <- rows
                k.n_cpu = n
                k.cpu0 = s0
                k.cpu1 = s1
            } else {
                k.exact <- rows
            }
        }
    }
}

//! a kept canvas's verdict, after its file's block: every leg kept, the counters (`twin_counted`, the device leg's rows the
//! `patch` grid's), the device chain within the rel_l2 bar the CPU q8 chain sets from the exact chain (`twin_bar`), and
//! the input poison past it where the canvas takes one
def twin_kept_held(tt : T?; family, log_suffix : string; patch : int64; k : TwinKept) {
    let spec = TwinSpec(family = family, tag = twin_tag(k), n_layer = k.n_layer, log_suffix = log_suffix, metric = TwinMetric.rel_l2, seat_family = TowerFamily.vision)
    let legs = k.zero_block >= 0l ? 4 : 3
    tt |> equal(k.legs, legs, "{spec.tag}: the {k.nl}-block canvas kept its {legs} legs")
    return if (k.legs != legs)
    twin_counted(tt, spec.tag, k.n_cpu, k.n_dev, k.cpu0, k.cpu1, k.dev0, k.dev1, k.n_layer, (int64(k.w) / patch) * (int64(k.h) / patch))
    let held = twin_bar(tt, spec, k.exact, k.cpu, k.dev)
    if (k.zero_block >= 0l) {
        twin_poison_held(tt, spec, held.rms, held.bar, k.zero_block, k.exact, k.poisoned)
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

//! a vision tower's staging for a form minted in memory - never the family's load, which bakes a `.dlim`: the planes `stage`
//! reads from `mm` under the family's lane pin (`pin(q8)`, `unpin` putting the policy back however the stage leaves, a panic
//! included), staged for the image flavor `dev` (`#tower-dev-block`: `none` the CPU image, a GPU flavor row-major for its
//! driver's bake), whole - a form cuts and poisons its own copy (`truncate_and_poison`)
def staged_twin_tower(mm : string; q8 : bool; dev : TowerDev; pin : function<(on : bool) : void>; unpin : function<() : void>;
                      stage : function<(path : string) : auto(STG)>) : STG {
    var stg = STG()
    invoke(pin, q8)
    with_put_back($ {
        invoke(unpin)
    }) {
        delete stg
        stg <- tower_dev_staged(dev) $ { return <- invoke(stage, mm) }
    }
    return <- stg
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

//! a tower's exact lane on `fam`'s device as its legs keep it - `tag` the asserts' name, `bar` the rel_l2 bar from the exact
//! CPU chain, `npos` the patch grid's rows (negative: not asserted): the CPU chain's rows, the device chain's and the input
//! poison's, the token counts, the driver's counters around the device leg and its `quant_mode` declines
struct ExactLaneKept {
    tag : string
    fam : TowerFamily
    bar : double
    npos : int64
    n_layer : int64
    cpu : array<float>
    dev : array<float>
    poisoned : array<float>
    n_cpu : int64
    n_dev : int64
    s0 : GpuTowerCounters
    s1 : GpuTowerCounters
    dq : int64
}

//! the exact lane's CPU leg: `encode` over `tw_cpu` with the tower knob off, its rows kept
def exact_lane_cpu_leg(var k : ExactLaneKept; tw_cpu : auto(TW); var st_cpu : auto(ST);
                       encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    with_gpu_knobs() {
        set_serving_gpu_tower(false, k.fam)
        k.n_cpu = queued_encode(tw_cpu, st_cpu, k.cpu, encode)
    }
}

//! the exact lane's device leg: `encode` over `tw_dev` with the tower knob on, its rows kept with the driver's counters
//! around it and its `quant_mode` declines
def exact_lane_dev_leg(var k : ExactLaneKept; tw_dev : auto(TW); var st_dev : auto(ST);
                       encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    let fam = k.fam
    k.n_layer = twin_facts(tw_dev).n_layer
    with_gpu_knobs() {
        set_serving_gpu_tower(true, fam)
        k.s0 = gpu_tower_counters(fam)
        let dq0 = gpu_tower_quant_declines(fam)
        k.n_dev = queued_encode(tw_dev, st_dev, k.dev, encode)
        k.s1 = gpu_tower_counters(fam)
        k.dq = gpu_tower_quant_declines(fam) - dq0
    }
    if (k.s1.encodes == k.s0.encodes) {
        to_log(LOG_INFO, "{k.tag} exact lane: the driver declined - {gpu_tower_declines(fam)}\n")
    }
}

//! the exact lane's clean legs held: the device leg served every block with no `quant_mode` decline, within the bar of the CPU
//! chain, a moved element the control
def exact_lane_held(tt : T?; k : ExactLaneKept) {
    let tag = k.tag
    let bar = k.bar
    tt |> equal(k.n_cpu, k.n_dev, "{tag} exact lane: token count on both chains")
    device_leg_served(tt, "{tag} exact lane", k.s0, k.s1, k.n_layer, k.npos)
    tt |> equal(k.dq, 0l, "{tag} exact lane: no quant_mode decline")
    tower_twin_held(tt, "{tag} exact lane", k.fam, k.cpu, k.dev, bar)
    var nsq = 0.0lf
    for (v in k.cpu) {
        nsq += double(v) * double(v)
    }
    var moved := k.cpu
    moved[0] += float(2.0lf * bar * sqrt(nsq) + double(abs(k.dev[0] - k.cpu[0])) + 1.0lf)
    var stm = TowerParityStats()
    if (tower_parity_stats(tt, "{tag} exact-lane control", moved, k.dev, stm)) {
        tt |> success(stm.rel_l2 > bar, "{tag} exact lane control: the expectation moved past the bar at one element reds it ({stm.rel_l2})")
    }
    delete moved
}

//! the exact lane's input poison leg: `encode` over the zeroed tower `twp` on the device, its rows kept
def exact_lane_poison_leg(tt : T?; var k : ExactLaneKept; twp : auto(TW); var stp : auto(ST);
                          encode : block<(t : TW; var s : ST; var out : array<float>) : int64>) {
    k.poisoned <- device_poison_leg(tt, "{k.tag} exact lane", twp, stp, encode, k.fam)
}

//! the exact lane's poison held: the zeroed tower's rows past the bar from the clean CPU chain
def exact_lane_poison_held(tt : T?; k : ExactLaneKept) {
    tower_twin_poisoned(tt, "{k.tag} exact lane", k.fam, k.cpu, k.poisoned, k.bar)
}

//! A tower's exact lane on its family's home as the forms of its file keep it: the
//! lane's rows (`ExactLaneKept`), the device and the poison towers' image facts, and the front's dispatches over the two
//! device legs.
struct ExactFlavorKept {
    lane : ExactLaneKept
    dev : TowerDev
    dev_f16 : bool
    poison_f16 : bool
    convs : int64
}

//! the exact lane `k` named: its asserts' tag, its family, its bar over rel_l2 and the patch grid's rows (negative: not asserted)
def exact_flavor_named(var k : ExactFlavorKept; tag : string; fam : TowerFamily; bar : double; npos : int64) {
    k.lane.tag = tag
    k.lane.fam = fam
    k.lane.bar = bar
    k.lane.npos = npos
}

//! the kept exact lane's CPU leg, on the exact form's CPU image `tw_exact`
def exact_flavor_cpu_leg(var k : ExactFlavorKept; tw_exact : auto(TW); var st_exact : auto(ST);
                         encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) {
    exact_lane_cpu_leg(k.lane, tw_exact, st_exact, encode)
}

//! the kept exact lane's device leg, on the exact form's image in the home's flavor `twd`: its facts and the front's dispatches kept
def exact_flavor_dev_leg(var k : ExactFlavorKept; twd : auto(TW); var std : auto(ST);
                         encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) {
    let facts = twin_facts(twd)
    k.dev = facts.dev
    k.dev_f16 = facts.f16
    let c0 = gpu_tower_counters(k.lane.fam).convs
    exact_lane_dev_leg(k.lane, twd, std, encode)
    k.convs += gpu_tower_counters(k.lane.fam).convs - c0
}

//! the kept exact lane's input poison leg, on the zeroed exact form's image in the home's flavor `twp`
def exact_flavor_poison_leg(tt : T?; var k : ExactFlavorKept; twp : auto(TW); var stp : auto(ST);
                            encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) {
    k.poison_f16 = twin_facts(twp).f16
    let c0 = gpu_tower_counters(k.lane.fam).convs
    exact_lane_poison_leg(tt, k.lane, twp, stp, encode)
    k.convs += gpu_tower_counters(k.lane.fam).convs - c0
}

//! The kept exact lane held, after its file's block: the device leg the home's halfword flavor, the lane within its bar of
//! the exact CPU chain with a moved element the control, the poison past it. Answers the front's dispatches over the two
//! device legs.
def exact_flavor_held(tt : T?; k : ExactFlavorKept) : int64 {
    let tag = k.lane.tag
    let home = serving_tower_dev(k.lane.fam)
    let kept = !empty(k.lane.cpu) && !empty(k.lane.dev) && !empty(k.lane.poisoned)
    tt |> success(kept, "{tag}: the exact lane kept its clean legs and its poison leg")
    return k.convs if (!kept)
    tt |> equal(k.dev, home, "{tag}: the exact lane's device leg is the {home} flavor's image")
    tt |> success(k.dev_f16 && k.poison_f16, "{tag}: the exact lane's GEMMs are halfwords on the {home} home")
    exact_lane_held(tt, k.lane)
    exact_lane_poison_held(tt, k.lane)
    return k.convs
}

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
