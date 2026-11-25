options gen2
options _dasllama_internal = true

module _tts_parity shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/fio public
require strings public
require math
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tts   // nolint:STYLE029,LINT019 — the facade synthesis, read only inside the Apple half
require dasllama/dasllama_math   // with_dasllama_jobque_
require ?das_metal dasllama/dasllama_metal_tower
require ?das_metal dasllama/dasllama_metal_tts   // nolint:STYLE030,LINT019 — the Metal home's speech seats register here: a file of this rail alone serves on its home
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 — metal_kernel_coverage: the per-kernel launch census, read only inside the Apple half
require ?vulkan dasllama/dasllama_vulkan_tower   // nolint:STYLE030,LINT019 — the Vulkan tower knob, read only inside the GPU half
require ?vulkan dasllama/dasllama_vulkan_tts   // nolint:STYLE030,LINT019 — the Vulkan home's speech seats register here
require ?cuda dasllama/dasllama_cuda_tts   // nolint:STYLE030,LINT019 — the CUDA home's speech seats register here
require ?cuda dasllama/dasllama_cuda_rail   // nolint:STYLE030,LINT019 - set_cuda_tts_take_fault_: the take's error arm, read only under the CUDA ask
require ?cuda dasllama/dasllama_cuda_common   // nolint:STYLE030,LINT019 — cud_kernel_coverage: the CUDA home's launch census, read only under the CUDA ask
require dasllama/dasllama_tower_tts   // tts_home_stats, tts_home_declines: the serving home's seat counters
require dasllama/dasllama_tower_image   // with_cpu_flavor: the image_cpu form's load
require dasllama/dasllama_gpu_rail   // set_rail_prof_pin, rail_prof_last: the speech seats' profile ledger
require dasllama/dasllama_image   // image_tag_lane, image_save_enabled: a form's image origin
require dasllama/dasllama_env   // g_env_engine.image: the image rail's switch
require daslib/jobque_boost
require daslib/linq
require daslib/strings_boost   // split: a form's name
require _tts_oracle
require _model_tier
require _model_form   // add_served_forms: the forms of a block the box serves
require _gpu_knobs public
require _f32_tile_pin   // with_reference_f32_tile: the parity bars at the tile's reference order; with_put_back: a form's lane unset

// The parity rail both StyleTTS2 families run against their onnxruntime / PyTorch oracle
// dumps (harness/kitten_oracle.py, harness/kokoro_oracle.py): the token ids against the
// reference driver on every case, identical durations on every case, and on the bring-up set
// every stage through the decoder output within BAR of the oracle's peak, the sine source
// within SOURCE_BAR fed the oracle's F0, the generator within BAR fed the oracle's source signal
// and decoder output up to its conv_post seam, which the fp32 batch tile rounds apart from the
// per-token dot (SEAM_CONV_POST_BAR, SEAM_WAVE_BAR). The end-to-end waveform difference is logged, not gated: the source
// integrates F0 into a phase of 1e5 radians, so the GEMM-order rounding the predictor's F0
// carries becomes radians of harmonic phase.

let BAR = 1e-4
let SEAM_CONV_POST_BAR = 2.5e-4   //! of the oracle's peak: the fp32 batch tile rounds conv_post apart from the per-token dot the 1e-4 bar was cut on
let SEAM_WAVE_BAR = 1e-3   //! absolute, the waveform after conv_post: the same rounding, -60 dB
let SOURCE_BAR = 5e-3   //! the harmonic source against the oracle's, absolute: the phase past 1e5 radians turns a ulp of an increment into radians, and the pod's x64 chain reads 2.5e-3 on kitten's ONNX law where the M5 Max reads under 1e-4

//! The Metal driver's bars: rel-l2 against the CPU chain fed the same inputs on the f32 lane (the readings: tests/CLAUDE.md).
let GPU_ALBERT_BAR = 5e-5   //! the f32-exact front end reads 3e-6; the f16-staged route read 2e-3, so the bars sit between
let GPU_TEXT_BAR = 1e-5
let GPU_PRED_BAR = 2e-5
let GPU_DECODE_BAR = 5e-2
let GPU_DECODE_Q8_BAR = 1e-1   //! the served lane: the CPU q8 chain quantizes its activations, the tower reads the dequantized weights at f32
let GPU_DECODE_KQ_BAR = 1.5e-1   //! the recipe lane, the same way: the CPU chain requantizes its rows to Q8_K, one scale a 256 block, against the tower's dequantized K-quant weights - reads 0.053 to 0.115 on kitten nano and mini and 0.034 to 0.048 on kokoro (M5 Max, Metal), the poisoned input 0.37 and up

def private within_twentieth(n, m : int64) : bool => abs(n - m) * 20l < m

//! the launches in `cov` of kernels whose name carries every one of `parts`
def census_launches(cov : table<string; int64>; parts : array<string>) : int64 {
    var n = 0l
    for (name, count in keys(cov), values(cov)) {
        var hit = true
        for (part in parts) {
            hit = hit && find(name, part) >= 0
        }
        n += hit ? count : 0l
    }
    return n
}

//! the serving home's launch census, a count a kernel name; empty with no GPU module
def gpu_census() : table<string; int64> {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return <- metal_kernel_coverage()
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            if (tower_family_cuda(TowerFamily.tts)) {
                return <- cud_kernel_coverage()
            }
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return <- vk_kernel_coverage()
        } else {
            var none : table<string; int64>
            return <- none
        }
    }
}

//! the serving home's launches of kernels whose name carries all of `parts`; 0 with no GPU module
def gpu_launches(parts : array<string>) : int64 {
    var inscope cov <- gpu_census()
    return census_launches(cov, parts)
}

//! the launches a census gained from `before` to `after`, the kernels that launched alone
def census_gain(before, after : table<string; int64>) : table<string; int64> {
    var gain : table<string; int64>
    for (name, count in keys(after), values(after)) {
        let n = count - (before?[name] ?? 0l)
        if (n != 0l) {
            gain |> insert(name, n)
        }
    }
    return <- gain
}

//! every launch a census counts
def census_total(cov : table<string; int64>) : int64 => census_launches(cov, array<string>())

//! a census as a log line: each kernel as `name count`, in name order
def census_text(cov : table<string; int64>) : string {
    var inscope names <- [for (name in keys(cov)); name]
    sort(names)
    return join([for (name in names); "{name} {cov?[name] ?? 0l}"], ", ")
}

//! Pocket's frame loop's row GEMV over f32 rows of the slab, by the head of its stamps' names on every home
let F32_ROW_GEMV = "tts_pk_gemv_"

//! the dense kernels that read f32 rows of a slab: the shared f32 tile, the Metal home's own f32 tiles, the f32 recurrence
//! and Pocket's f32 row GEMV - what a leaf widened into the slab launches in place of its format's stamps
let private F32_ROW_KERNELS = fixed_array("f32_gemm_", "metal_f32_exact_", "metal_st2_conv_",
    "tts_lstm_dir_cls", F32_ROW_GEMV)

//! the launches in `cov` of the kernels that read f32 slab rows (`F32_ROW_KERNELS`, by the head of the name)
def f32_row_launches(cov : table<string; int64>) : int64 {
    var n = 0l
    for (name, count in keys(cov), values(cov)) {
        var hit = false
        for (head in F32_ROW_KERNELS) {
            hit ||= name |> starts_with(head)
        }
        n += hit ? count : 0l
    }
    return n
}

//! the stored members a GPU seat reads, in `StoredMarks`' order
let STORED_MEMBERS = fixed_array("q8", "k4", "k5", "k6", "f16")

//! the launches in `cov` of every stamp that reads a stored leaf of `member`
def stored_member_launches(cov : table<string; int64>; member : string) : int64 {
    var n = 0l
    if (member == "q8") {
        for (part in fixed_array("q8_f32_gemm_cls", "q8_tts_pk", "tts_lstm_dir_q8", "st2_lstm_q8",
            "st2_q8_", "q8_batch_")) {
            n += census_launches(cov, [part])
        }
    } elif (member == "f16") {
        for (part in fixed_array("f16_f32_gemm_cls", "f16_tts_pkst_", "st2_f16_")) {
            n += census_launches(cov, [part])
        }
    } else {
        n = census_launches(cov, ["_{member}_"])
    }
    return n
}

//! every stored member's launches in `cov`, in `STORED_MEMBERS`' order
def stored_marks(cov : table<string; int64>) : int64[5] {
    var m : int64[5]
    for (i, name in count(), STORED_MEMBERS) {
        m[i] = stored_member_launches(cov, name)
    }
    return m
}

//! every stored member's launches on the serving home now
def stored_marks() : int64[5] {
    var inscope cov <- gpu_census()
    return stored_marks(cov)
}

let STORED_HELD = 1      //! `stored_members_held`'s `want`: the model holds a leaf of the member - its stamps must have launched
let STORED_ABSENT = 0    //! ... it holds none - none may have (the control)
let STORED_UNTAGGED = -1 //! ... no loader tag says: logged alone (a half-staged conv a GPU bake mints is no file's format)

//! the launches since `marks` of each member, logged under `tag` and held to `want` (`stored_members_held`)
def stored_members_launched(t : T?; tag : string; marks : int64[5]; want : int[5]) {
    let now = stored_marks()
    var launched : int64[5]
    for (i in range(5)) {
        launched[i] = now[i] - marks[i]
    }
    stored_members_held(t, tag, launched, want)
}

//! each member's launches `launched`, logged under `tag` and held to `want` (a `STORED_*` a member)
def stored_members_held(t : T?; tag : string; launched : int64[5]; want : int[5]) {
    for (i, name in count(), STORED_MEMBERS) {
        let n = launched[i]
        to_log(LOG_INFO, "{tag}: {n} launches of the stamps that read stored {name}\n")
        if (want[i] == STORED_HELD) {
            t |> success(n > 0l, "{tag}: the model holds {name} leaves, and their stamps launched ({n})")
        } elif (want[i] == STORED_ABSENT) {
            t |> equal(n, 0l, "{tag}: the model holds no {name} leaf, and no stamp of that member launched")
        }
    }
}

//! the launches of a stored-plane reader on the Metal home: the home's tile at a hooked site (`tile`) and the shared stamp (`rail`)
def metal_stored_launches(tile, rail : string) : int64 => gpu_launches([tile]) + gpu_launches([rail])

//! The verdict where the tower served nothing: a skip where no GPU device serves, a red where a present device declined. Always false.
def tower_served(t : T?; msg, declines : string) : bool {
    if (find(declines, "device") < 0) {
        t |> failure("{msg}: the tower declined (declines {declines})")
    } else {
        t |> skip("{msg}: no GPU device serves (declines {declines})")
    }
    return false
}

//! seats served since the process started by the home that serves the TTS seats
def gpu_encodes() : int64 => tts_home_stats(serving_tower_dev(TowerFamily.tts)).encodes

//! that home's dense dispatches over rows a stored leaf was widened into, since the process started
def gpu_widened() : int64 => tts_home_stats(serving_tower_dev(TowerFamily.tts)).widened

def conv_off_lattice(c : TtsConv1d) : bool {
    //! A conv no stored member holds: quants or halves whose inputs sit off the 8 lattice, 32 for a
    //! transposed one - the slab holds its rows as floats, each dispatch over them one the home counts.
    return (c.q8 || c.kq_fmt != 0 || conv1d_held_halves(c)) && c.cin % (c.transposed ? 32l : 8l) != 0l
}

def st2_off_lattice_convs(m : StyleTts2Model) : int64 {
    //! The generator's noise convs off the stored members' lattice in `m`, each dispatched once a
    //! chunk: the harmonic source's spectrum is 22 columns wide, which no stored member's gather reads.
    var n = 0l
    for (c in m.decoder.noise_convs) {
        n += conv_off_lattice(c) ? 1l : 0l
    }
    return n
}

//! that home's declines by reason name
def gpu_declines() : table<string; int64> => tts_home_declines(serving_tower_dev(TowerFamily.tts))

//! `body` on a set-up job queue, every GPU knob put back after as the cell found them
def with_gpu_cell(body : block<() : void>) {
    with_dasllama_jobque_() {
        with_gpu_knobs(body)
    }
}

//! both drivers register every StyleTTS2 seat, so a stage no seat serves is a lost registration: a red, never a skip
[arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private seat_present(t : T?; tag, stage : string) : bool {
    return true if (styletts2_gpu_seat_present(stage))
    t |> success(false, "{tag} GPU {stage}: the driver registers no {stage} seat on this backend")
    return false
}

//! whether every front-end pack the voices of the model at `path` ask for sits beside it
def private voice_packs_present(path : string) : bool {
    for (pack in tts_pack_names(path)) {
        return false if (!stat(path_join(dir_name(path), pack)).is_valid)
    }
    return true
}

//! whether this build carries a GPU home the TTS seats serve on
def tts_gpu_build() : bool {
    static_if (typeinfo builtin_module_exists(das_metal) || typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        return true
    } else {
        return false
    }
}

//! what the box lacks for a `@dev` form (a GPU home up with the tower knob on), "" where it serves
def tts_dev_why() : string {
    return "no GPU module in this build" if (!tts_gpu_build())
    var up = false
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.tts)
        up = serving_tower_device_up(TowerFamily.tts)
    }
    return up ? "" : "no GPU device serves the TTS seats on this box"
}

//! "" where the box serves the lane's `@dev` form (`dev_why` empty, `tts_dev_why`), else why its CPU form is skipped too
def tts_cpu_leg_why(dev_why : string) : string
    => empty(dev_why) ? "" : "{dev_why}, and the lane's CPU leg is read only beside its `@dev` form's"

//! What a minted TTS form's name says - `<lane>[@<home>][+<pins>]`: its lane; the image it is minted as - `dev` the serving
//! home's as a load serves it, `cpu` the CPU image, none the family's own (`styletts2_form`, `pocket_form`); and the groups
//! a recipe lane pins (`set_styletts2_recipe`'s spec).
struct TtsForm {
    lane : string
    home : string
    pins : string
}

def tts_form_of(form : string) : TtsForm {
    var inscope edits <- split(form, "+")
    var inscope homes <- split(edits[0], "@")
    return TtsForm(lane = homes[0], home = length(homes) > 1 ? homes[1] : "", pins = length(edits) > 1 ? edits[1] : "")
}

//! the form `form` with the image `home` names (`TtsForm`), "" none
def tts_form_at(form, home : string) : string {
    let f = tts_form_of(form)
    let at = empty(home) ? f.lane : "{f.lane}@{home}"
    return empty(f.pins) ? at : "{at}+{f.pins}"
}

def styletts2_lane_forms(t : T?; file : string; images : array<string>; pinned : bool) : array<string> {
    //! The forms a StyleTTS2 voice's block loads, `file` the tag of its skips: the image forms
    //! `images` its facade cells read; the f32 form; and each lane the tower's cells read - f32,
    //! q8, the recipe lane and, `pinned`, the recipe lane with its upsamplers pinned Q5_K - as its
    //! CPU form and, after it, its `@dev` form. A `@dev` form the box does not serve is a skipped
    //! cell of its own, and so is its lane's CPU form (the f32 form aside), whose leg no cell reads
    //! without it.
    var forms := images
    forms |> push("f32")
    let lacks = tts_dev_why()
    var inscope lanes <- pinned ? ["f32", "q8", "kq", "kq+ups=k5"] : ["f32", "q8", "kq"]
    for (lane in lanes) {
        if (lane != "f32") {
            add_served_forms(t, forms, file, [lane], tts_cpu_leg_why(lacks))
        }
        add_served_forms(t, forms, file, [tts_form_at(lane, "dev")], lacks)
    }
    return <- forms
}

def styletts2_stored_forms(t : T?; file, lane : string) : array<string> {
    //! The forms a published stored voice file's block loads, `file` the tag of its skips and
    //! `lane` the lane the file stores (`q8`, `kq`): the image form its facade cell reads, then
    //! the lane's CPU form and its `@dev` form, for the served synthesis and the cell of the
    //! formats the file stores - each of the two a skipped cell of its own where no device serves.
    var forms <- ["image"]
    let lacks = tts_dev_why()
    add_served_forms(t, forms, file, [lane], tts_cpu_leg_why(lacks))
    add_served_forms(t, forms, file, [tts_form_at(lane, "dev")], lacks)
    return <- forms
}

//! the TTS lane and row pins in force by name, "" with none
def tts_pins_left() : string {
    var inscope pins <- [
        (on = styletts2_q8_pinned(), name = "the StyleTTS2 q8 lane (set_styletts2_q8)"),
        (on = styletts2_serves_recipe(), name = "the StyleTTS2 recipe lane (set_styletts2_recipe)"),
        (on = !pocket_serves_native(), name = "the Pocket lane (set_pocket_q8)"),
        (on = rows_f32_pinned(), name = "the row form (set_rows_f32)")]
    var inscope left <- [for (p in pins); p.name; where p.on]
    return join(left, ", ")
}

//! every TTS pin unset at `who`'s entry, a panic naming the pins in force otherwise: a cell that
//! takes a pin puts it back, so one found here is the leak of the cell before
def tts_pins_unset(who : string) {
    let left = tts_pins_left()
    if (!empty(left)) {
        panic("{who}: {left} pinned on entry - the cell that set it did not put it back")
    }
}

def styletts2_form(path, form : string) : TtsModel {
    //! One form of a StyleTTS2 voice file (KittenTTS, Kokoro) as the facade holds it, its block's
    //! form loader. `image` is the facade's own load, through the image rail on the lane an
    //! unpinned load serves, in the flavor the box's GPU home serves; `image_cpu` is that load
    //! under `with_cpu_flavor`, the CPU image the CPU chain's cells run. Any other form (`TtsForm`)
    //! is minted in memory from the GGUF's staging, no `.dlim` baked: `f32`, `q8` and `kq` the
    //! reference, served and recipe lanes' CPU image, `<lane>@dev` the lane as a load stages it for
    //! the serving home with the tower knob on, `+<pins>` the recipe lane with those groups
    //! pinned - the pin unset and the knobs put back however the mint leaves, a panic included. The
    //! front-end packs load where every pack the file's voices ask for sits beside it; without them
    //! the form carries the voice alone.
    let packs = voice_packs_present(path)
    var inscope m = TtsModel()
    tts_pins_unset("styletts2_form({form})")
    if (form == "image" || form == "image_cpu") {
        let load = $ {
            if (packs) {
                delete m
                m <- load_tts_model(path)
            } else {
                m.model <- load_styletts2(path)
            }
        }
        if (form == "image_cpu") {
            with_cpu_flavor(load)
        } else {
            invoke(load)
        }
        return <- m
    }
    let f = tts_form_of(form)
    let knobs = gpu_knobs()
    if (!empty(f.pins)) {
        set_styletts2_recipe(f.pins)
    }
    with_put_back($ {
        reset_styletts2_recipe()
        set_gpu_knobs(knobs)
    }) {
        if (f.home == "dev") {
            set_serving_gpu_tower(true, TowerFamily.tts)
            var inscope stg <- stage_styletts2_served(path, f.lane != "f32", f.lane == "kq")
            mint_styletts2(stg, m.model)
        } else {
            var inscope stg <- stage_styletts2(path, f.lane != "f32", f.lane == "kq")
            mint_styletts2(stg, m.model)
        }
    }
    if (packs) {
        tts_finish_styletts2(m, path)
    }
    return <- m
}

//! the image a TTS form names is the one its model is (`dev_backend`): the CPU image (`cpu`), or the serving home's
def private form_image_is(t : T?; tag, form : string; cpu : bool; dev : TowerDev) {
    if (cpu) {
        t |> equal(dev, TowerDev.none, "{tag}: the {form} form is the CPU image")
    } else {
        t |> equal(dev, serving_tower_dev(TowerFamily.tts), "{tag}: the {form} form is the serving home's image")
    }
}

//! whether a load through the image rail maps a `.dlim` on this run: the rail on and its mints saved
def private rail_maps() : bool => g_env_engine.image && image_save_enabled()

//! How a TTS form came by its image is what its name says (`tts_image_origin`): a minted form's is built in memory, and
//! an image form's (`image`) is a `.dlim` the image rail mapped - held where this run's rail maps one (`rail_maps`).
def private form_origin_is(t : T?; tag, form : string; image : bool; origin : TtsImageOrigin) {
    if (!image) {
        t |> success(!origin.mapped, "{tag}: the {form} form is minted in memory, no `.dlim` mapped")
    } elif (rail_maps()) {
        t |> success(origin.mapped, "{tag}: the {form} form is a `.dlim` the image rail mapped "
            + "(DASLLAMA_IMAGE and DASLLAMA_IMAGE_SAVE on), not an image minted in memory")
    }
}

def styletts2_form_is(t : T?; m : TtsModel; path, form : string) {
    //! The form is the lane and the image it names: on `m`, a form of the StyleTTS2 voice at `path`
    //! (`styletts2_form`), the model's own lane flags - the f32, q8 or recipe lane of a minted
    //! form, on an image form the lane the file stores or else the q8 lane an unpinned load
    //! serves - its image flavor: the CPU image, a `@dev` form the serving home's; the `image`
    //! form's flavor is the box's to pick - and how it came by its image (`form_origin_is`).
    let image = form == "image" || form == "image_cpu"
    let f = tts_form_of(form)
    let kq = image ? styletts2_stored_lane(path) == "kq" : f.lane == "kq"
    t |> success(m.model.q8 == (image || f.lane != "f32") && m.model.kq == kq, "{base_name(path)}: the {form} form serves the lane it names (q8 {m.model.q8}, kq {m.model.kq})")
    if (form != "image") {
        form_image_is(t, base_name(path), form, f.home != "dev", m.model.dev_backend)
    }
    form_origin_is(t, base_name(path), form, image, tts_image_origin(m.model))
}

//! the lane tag a Pocket form's image is minted under: the unpinned lane's for `image` and `native`, else the pin's
def private pocket_form_lane(lane : string) : string =>
    lane == "q8" ? POCKET_IMAGE_TAG_Q8 : (lane == "f32" ? POCKET_IMAGE_TAG_F32 : POCKET_IMAGE_TAG)

def pocket_form_is(t : T?; m : TtsModel; form : string) {
    //! The form is the lane and the image it names: on `m`, a form of a Pocket file
    //! (`pocket_form`), the model's own lane flag on a pinned form, the image flavor of a `@cpu` or
    //! `@dev` form, how it came by its image (`form_origin_is`), the lane tag that image was minted
    //! under - the unpinned lane's where the form pins none - and no lane left pinned behind the
    //! load.
    let f = tts_form_of(form)
    if (f.lane == "f32" || f.lane == "q8") {
        t |> equal(m.pocket.q8, f.lane == "q8", "the {form} form serves the lane it names")
    }
    if (!empty(f.home)) {
        form_image_is(t, "pocket", form, f.home == "cpu", m.pocket.dev_backend)
    }
    let origin = tts_image_origin(m.pocket)
    form_origin_is(t, "pocket", form, form == "image", origin)
    t |> equal(image_tag_lane(origin.tag), pocket_form_lane(f.lane),
        "the {form} form's image is minted on the lane it names, no other pin in force ({origin.tag})")
    t |> success(pocket_serves_native(), "the {form} form's load leaves no lane pinned")
}

//! what a cell's Pocket mint holds: the image the box's GPU home serves with the weights its device block replaces kept
//! beside it (one model for a compare's CPU leg and its served legs), that image as a load serves it, or the CPU image
enum PkMint {
    both
    device
    cpu
}

def pocket_minted(path : string; mint : PkMint = PkMint.both) : PocketModel {
    //! A Pocket model minted in memory from the GGUF's staging on the lane the pins name, with no
    //! `.dlim` baked beside the stocked file: `both` for the cells that flip the tower knob on one
    //! model, `device` what `load_pocket` serves
    let was = set_tts_host_weights_(mint == PkMint.both)
    var inscope st <- mint == PkMint.cpu ? stage_pocket(path, pocket_serves_q8(), pocket_serves_native()) : stage_pocket_served(path, pocket_serves_q8(), pocket_serves_native())
    set_tts_host_weights_(was)
    var inscope m = PocketModel()
    mint_pocket(st, m)
    return <- m
}

//! `pocket_minted` as the facade holds it.
def pocket_tts_minted(path : string; mint : PkMint = PkMint.both) : TtsModel {
    var inscope m = TtsModel()
    m.kind = TtsKind.pocket
    m.pocket <- pocket_minted(path, mint)
    return <- m
}

//! the mint a Pocket form's home names (`TtsForm`)
def private pocket_form_mint(home : string) : PkMint =>
    home == "cpu" ? PkMint.cpu : (home == "dev" ? PkMint.device : PkMint.both)

def pocket_form(path, form : string) : TtsModel {
    //! One form of a Pocket TTS file as the facade holds it, its block's form loader: `image` is
    //! the facade's own load, through the image rail; `native`, `q8` and `f32` are the lane minted
    //! in memory from the GGUF's staging with no `.dlim` baked - the file's own quants with no lane
    //! pinned, the q8 pin, the f32 pin - as the image the box's GPU home serves with the weights
    //! its device block replaces kept beside it (`PkMint.both`: one model for a compare's CPU leg
    //! and its served legs); `<lane>@dev` is the image a load serves, its weights the device
    //! block's alone - both minted with the tower knob on - and `<lane>@cpu` the lane's CPU image.
    //! The lane is unset and the knobs put back however the mint leaves, a panic included.
    tts_pins_unset("pocket_form({form})")
    return <- load_tts_model(path) if (form == "image")
    let f = tts_form_of(form)
    if (f.lane != "native") {
        set_pocket_q8(f.lane == "q8")
    }
    let knobs = gpu_knobs()
    var inscope m = TtsModel()
    with_put_back($ {
        reset_pocket_q8()
        set_gpu_knobs(knobs)
    }) {
        if (f.home != "cpu") {
            set_serving_gpu_tower(true, TowerFamily.tts)
        }
        delete m
        m <- pocket_tts_minted(path, pocket_form_mint(f.home))
    }
    return <- m
}

//! One family's rig: where its dumps live, how it turns the fixture's phoneme string into token
//! ids, and what it holds the loaded model to before its cases (`prepare`, where it names one).
struct TtsParityRig {
    family : string
    oracle_dir : string
    ids : function<(ps : string) : array<int>>
    prepare : function<(t : T?; m : StyleTts2Model) : void>
}

//! How `bar_diff` reads a distance, and the poisoned expectation that must red its bar
enum BarMetric {
    absolute   //! max |ours - ref| within the bar; the poison moves one element by twice the bar
    of_peak    //! max |ours - ref| within the bar times the reference's peak; the poison moves one element by twice that
    rel_l2     //! `rel_l2(ours, ref)` under the bar; the poison shifts every element by `shift`
}

//! `ours` against `ref` under `bar` read by `metric`, logged either way, with the control: the poisoned expectation must red the bar.
def bar_diff(t : T?; tag : string; ours, ref : array<float>; bar : float; metric : BarMetric; shift : float = 0.0) {
    if (length(ours) != length(ref)) {
        t |> failure("{tag}: {length(ours)} elements against the reference's {length(ref)}")
        return
    }
    if (metric == BarMetric.rel_l2) {
        let rel = rel_l2(ours, ref)
        to_log(LOG_INFO, "{tag}: rel-l2 {rel} over {length(ref)} (bar {bar})\n")
        t |> success(rel < bar, "{tag}: rel-l2 {rel} under {bar}")
        var inscope poisoned <- [for (v in ref); v + shift]
        t |> success(rel_l2(ours, poisoned) >= bar, "{tag}: the bar discriminates an expectation shifted by {shift}")
        return
    }
    let of_peak = metric == BarMetric.of_peak
    check(t, tag, ours, ref, Bar(kind = of_peak ? BarKind.rel_of_max : BarKind.rel_of_larger, rel = of_peak ? bar : 0.0, abs_floor = of_peak ? 0.0 : bar,
        poison = 2.0 * bar, poison_of_max = of_peak, poison_at = 0), FLAT_SHAPE)
}

//! One stage against its dump within BAR (of the oracle's peak, or absolute); a stage the dump lacks is skipped.
def stage_diff(t : T?; tag, name : string; ours : array<float>; dump : table<string; TtsDumpTensor>; metric : BarMetric = BarMetric.of_peak; bar : float = BAR) {
    dump |> get(name) $(tensor) {
        bar_diff(t, "{tag} {name}", ours, tensor.f, bar, metric)
    }
}

let PHASE_BAR = 1e-2   // radians, where the magnitude is above a thousandth of its peak

// the source spectrum against the oracle's: magnitude rows relative to the peak, phase rows
// wrapped and only where the magnitude carries signal
def spectrum_diff(t : T?; tag : string; ours, ref : array<float>; bins, frames : int64) {
    let n = bins * frames
    if (long_length(ours) != 2l * n || long_length(ref) != 2l * n) {
        t |> failure("{tag}: {length(ours)} elements against {length(ref)}, want {2l * n}")
        return
    }
    var peak = 0.0
    for (i in range64(n)) {
        if (!is_nan(ref[i])) {
            peak = max(peak, abs(ref[i]))
        }
    }
    let mag_d = span_maxdiff(ours, ref, 0, 0, int(n)).diff
    var ph_d = 0.0
    var gated = 0l
    for (i in range64(n)) {
        if (ref[i] > 1e-3 * peak) {
            gated++
            var d = abs(ours[n + i] - ref[n + i])
            if (is_nan(d)) {
                d = INFINITE_DIFF
            } elif (d > PI) {
                d = 2.0 * PI - d
            }
            ph_d = max(ph_d, d)
        }
    }
    to_log(LOG_INFO, "{tag}: magnitude maxdiff {mag_d} (peak {peak}), phase maxdiff {ph_d} rad over {gated} of {n} bins with signal\n")
    t |> success(mag_d <= BAR * peak, "{tag}: magnitude maxdiff {mag_d} <= {BAR * peak}")
    t |> success(ph_d <= PHASE_BAR, "{tag}: phase maxdiff {ph_d} rad <= {PHASE_BAR} where the magnitude carries signal")
}

def case_noise(dump : table<string; TtsDumpTensor>) : TtsNoise
    => <- TtsNoise(uniform <- dump_tensor(dump, type<float>, "noise_uniform"), normal <- dump_tensor(dump, type<float>, "noise_normal"), captured = true)

// the generator fed the oracle's own F0 curve, source signal and decoder output: block
// correctness at the seams, with the phase drift the predictor's rounding noise induces taken out
def private seam_check(t : T?; m : StyleTts2Model; tag : string; dump : table<string; TtsDumpTensor>;
                       var cases : array<ParityCase>; keep_stages : bool) {
    var inscope f0 <- dump_tensor(dump, type<float>, "f0")
    var inscope gen_in <- dump_tensor(dump, type<float>, "gen_in")
    var inscope style <- dump_tensor(dump, type<float>, "style")
    var inscope style_dec <- copy_span(style, 0l, m.style_dim / 2l)
    let f2 = long_length(f0)
    let cw = long_length(gen_in) / f2
    var inscope noise <- case_noise(dump)
    var tr = new StyleTts2Trace()
    var inscope sc = St2Scratch()
    var inscope har : array<float>
    let n = styletts2_source(m.decoder, m.source, f0, f2, noise, 0ul, har, sc)
    stage_diff(t, "{tag} SOURCE", "har", har, dump, BarMetric.absolute, SOURCE_BAR)
    var inscope har_ref <- dump_tensor(dump, type<float>, "har")
    var inscope spec : array<float>
    let frames = styletts2_har_spectrum(m.decoder, har_ref, n, tr, spec, sc)
    var inscope spec_ref <- dump_tensor(dump, type<float>, "har_spec")
    if (!empty(spec_ref)) {
        // the reference's FFT and a direct sum agree on the magnitude everywhere and on the phase
        // wherever the magnitude is meaningful; below that the phase is rounding noise the network
        // nonetheless consumes, so the generator is fed the oracle's spectrum from here on
        spectrum_diff(t, "{tag} SPECTRUM", spec, spec_ref, m.decoder.bins, frames)
        spec := spec_ref
    }
    var inscope wave : array<float>
    styletts2_generator(m.decoder, gen_in, cw, f2, style_dec, spec, frames, tr, wave, sc)
    if (keep_stages) {
        with_gpu_knobs() {
            cases |> emplace(cpu_stage_rows(m, tag, dump, noise))
        }
    }
    stage_diff(t, "{tag} SEAM", "stft_re", tr.stft_re, dump)
    stage_diff(t, "{tag} SEAM", "ups0", tr.ups0, dump)
    stage_diff(t, "{tag} SEAM", "src0", tr.src0, dump)
    stage_diff(t, "{tag} SEAM", "nres0", tr.nres0, dump)
    stage_diff(t, "{tag} SEAM", "rb0", tr.rb0, dump)
    stage_diff(t, "{tag} SEAM", "rb1", tr.rb1, dump)
    stage_diff(t, "{tag} SEAM", "rb2", tr.rb2, dump)
    stage_diff(t, "{tag} SEAM", "ups1", tr.ups1, dump)
    stage_diff(t, "{tag} SEAM", "conv_post", tr.conv_post, dump, BarMetric.of_peak, SEAM_CONV_POST_BAR)
    stage_diff(t, "{tag} SEAM", "waveform", wave, dump, BarMetric.absolute, SEAM_WAVE_BAR)
    // the widened bars' control on the route itself: the generator fed gen_in rotated one row lands outside both
    var inscope gen_rot : array<float>
    gen_rot |> resize(long_length(gen_in))
    for (r in range64(f2)) {
        let src = ((r + 1l) % f2) * cw
        for (c in range64(cw)) {
            gen_rot[r * cw + c] = gen_in[src + c]
        }
    }
    var tr2 = new StyleTts2Trace()
    var inscope wave2 : array<float>
    styletts2_generator(m.decoder, gen_rot, cw, f2, style_dec, spec, frames, tr2, wave2, sc)
    dump |> get("conv_post") $(tensor) {
        check_exceeds(t, "{tag} SEAM conv_post, the input rotated", tr2.conv_post, tensor.f, Bar(kind = BarKind.rel_of_max, rel = SEAM_CONV_POST_BAR, abs_floor = 0.0))
    }
    dump |> get("waveform") $(tensor) {
        check_exceeds(t, "{tag} SEAM waveform, the input rotated", wave2, tensor.f, Bar(kind = BarKind.rel_of_larger, rel = 0.0, abs_floor = SEAM_WAVE_BAR))
    }
    unsafe {
        delete tr2
        delete tr
    }
}

//! `run` with the tower knob off: one call and no serve at `stats`' seat, `declines` knob declines, `run` answering CPU-equal; the knob stays off after.
[unused_argument(t, tag, declines, stats, run)]   // off-Apple the gated body is empty
def knob_off_leg(t : T?; tag : string; declines : int64; stats : block<() : tuple<calls : int; served : int>>; run : block<() : bool>) {
    static_if (typeinfo builtin_module_exists(das_metal) || typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        set_serving_gpu_tower(false, TowerFamily.tts)
        let h0 = invoke(stats)
        let d0 <- gpu_declines()
        let same = invoke(run)
        let h1 = invoke(stats)
        let d1 <- gpu_declines()
        t |> equal(h1.calls - h0.calls, 1, "{tag}: the knob-off leg still reaches the hook")
        t |> equal(h1.served - h0.served, 0, "{tag}: the knob-off leg serves nothing")
        t |> equal((d1?["knob"] ?? 0l) - (d0?["knob"] ?? 0l), declines, "{tag}: the knob-off leg records its decline at every seat it reaches")
        t |> success(same, "{tag}: the knob-off leg is the CPU chain")
    }
}

//! `g` with one element moved by twice what the rel-l2 bar tolerates over the whole row: the smallest wrong output the bar must catch.
def one_wrong(g, cpu : array<float>; bar : float) : array<float> {
    var w := g
    let n = long_length(cpu)
    return <- w if (n < 1l)
    var den = 0.0lf
    for (v in cpu) {
        den += double(v) * double(v)
    }
    w[n / 2l] += 2.0 * bar * float(sqrt(den))
    return <- w
}

//! One hook call, one encode, the length, rel-l2 within `bar`, the poisoned and one-wrong legs past it; false where the tower did not serve once.
def gpu_stage_verdict(t : T?; tag, stage : string; g, cpu, gp : array<float>; bar : float; calls, served : int; encodes : int64; declines : string) : bool {
    return tower_served(t, "{tag} GPU {stage}", declines) if (served != 1)
    t |> equal(calls, 1, "{tag} GPU {stage}: one hook call")
    t |> equal(encodes, 1l, "{tag} GPU {stage}: one tower encode served the stage")
    t |> equal(length(g), length(cpu), "{tag} GPU {stage}: the output length")
    t |> equal(length(gp), length(cpu), "{tag} GPU {stage}: the poisoned leg's output length")
    return true if (length(g) != length(cpu) || length(gp) != length(cpu))
    let d = rel_l2(g, cpu)
    let dp = rel_l2(gp, cpu)
    to_log(LOG_INFO, "{tag} GPU {stage}: rel-l2 {d} against the CPU stage over {length(cpu)} (bar {bar}); poisoned input {dp}\n")
    t |> success(d <= bar, "{tag} GPU {stage}: rel-l2 {d} within {bar}")
    t |> success(dp > bar, "{tag} GPU {stage}: the poisoned input reds the bar (the compare's control)")
    var inscope w <- one_wrong(g, cpu, bar)
    t |> success(rel_l2(w, cpu) > bar, "{tag} GPU {stage}: one output element moved by twice the bar reds it (the bar's control)")
    return true
}

//! The three runs a seat leg compares.
enum TowerLeg {
    cpu        //! the knob off: the CPU stage, the reference
    served     //! the knob on: the stage on its seat
    poisoned   //! the knob on over a scaled or reversed input: the compare's control
}

//! The seat record a stage's seats sit in: kitten and kokoro share the StyleTTS2 one.
enum TtsFamily {
    styletts2
    pocket
}

def tts_family(kind : TtsKind) : TtsFamily => kind == TtsKind.pocket ? TtsFamily.pocket : TtsFamily.styletts2

//! `rig`'s oracle manifest; where its dumps are not stocked, a loud skip and no cases
def private rig_manifest(t : T?; rig : TtsParityRig) : TtsOracleManifest {
    var inscope mf <- load_tts_oracle_manifest(rig.oracle_dir)
    if (empty(mf.cases)) {
        t |> skip("{rig.oracle_dir} not stocked")
    }
    return <- mf
}

//! One seat's legs over one bring-up case on one lane: the CPU stage's output off the lane's CPU form, and off its `@dev`
//! form the seat's output over the same inputs and over the poisoned ones, with the seat's counters across the served leg
struct SeatLegs {
    family : TtsFamily   //! the seat record its counters are read from
    stage : string
    tag : string
    bar : float
    cpu : array<float>
    ran : bool   //! the `@dev` form's legs ran
    dev : array<float>
    poisoned : array<float>
    calls : int
    served : int
    encodes : int64
    declines : string
}

let private SEAT_ALBERT = 0
let private SEAT_TEXT = 1
let private SEAT_DUR_ENC = 2
let private SEAT_DURATIONS = 3
let private SEAT_PROSODY = 4
let private SEAT_DECODE = 5   //! the f32 lane's decode; the q8 lane's and the recipe lane's follow it

//! One bring-up case as its voice's forms keep it: the stage inputs the f32 form's CPU chain made, and each seat's legs -
//! the f32 lane's stages in dispatch order, then the q8 and the recipe lanes' decode over the f32 form's inputs
struct ParityCase {
    tag : string
    ids : array<int>
    style_dec : array<float>
    style_pro : array<float>
    n_tok : int64
    dc : int64       //! the duration rows' width
    text_c : int64   //! the text encoder's channels
    dur_c : array<int>
    frames : int64
    n_c : array<float>
    f2 : int64
    noise : TtsNoise
    frames_g : int64     //! the served durations' rounded frame count
    n_g : array<float>   //! the served prosody's N rows
    f2_g : int64         //! ... and its frame count
    seats : array<SeatLegs>
}

//! the decode seat of the lane `lane` in a case's seats
def private decode_seat(lane : string) : int => SEAT_DECODE + (lane == "f32" ? 0 : (lane == "q8" ? 1 : 2))

//! a case with its seats named and barred, no leg kept: the q8 lane's decode against the CPU q8 chain, which quantizes its
//! activations where the tower reads the dequantized weights - the q8 slab writes' only numeric gate; the recipe lane's
//! the K-quant convs and the Q6_K upsamplers through the slab's row reads - their only numeric gate
def private parity_case(tag : string) : ParityCase {
    var c = ParityCase(tag = tag)
    let stages = fixed_array("albert", "text", "dur_enc", "durations", "prosody", "decode", "decode", "decode")
    let bars = fixed_array(GPU_ALBERT_BAR, GPU_TEXT_BAR, GPU_PRED_BAR, GPU_PRED_BAR, GPU_PRED_BAR, GPU_DECODE_BAR, GPU_DECODE_Q8_BAR, GPU_DECODE_KQ_BAR)
    let lanes = fixed_array("", "", "", "", "", "", " q8 lane", " recipe lane")
    c.seats |> reserve(length(stages))
    for (stage, bar, lane in stages, bars, lanes) {
        c.seats |> emplace(SeatLegs(stage = stage, tag = "{tag}{lane}", bar = bar))
    }
    return <- c
}

//! a case's decode on the CPU chain of `m`, a lane's CPU form, over the f32 form's inputs - the CPU leg of the lane's
//! decode seat `seat`. The knob stays off.
def private cpu_decode_leg(m : StyleTts2Model; var c : ParityCase; seat : int) {
    var inscope sc = St2Scratch()
    var inscope asr : array<float>
    align_expand_channels(c.seats[SEAT_TEXT].cpu, c.text_c, c.n_tok, c.dur_c, asr)
    var tm = TtsTimings()
    set_serving_gpu_tower(false, TowerFamily.tts)
    styletts2_decode(m.decoder, c.text_c, m.source, asr, c.frames, c.seats[SEAT_PROSODY].cpu, c.n_c, c.f2, c.style_dec, c.noise, 0ul, null, tm, sc)
    c.seats[seat].cpu := sc.wave
}

//! One bring-up case's stages off the CPU chain of `m`, the f32 form, the tower knob off: PL-BERT, the text encoder, the
//! duration encoder, the durations, prosody and the decode over the oracle's ids, style and captured noise - each seat's CPU
//! leg, and the inputs the `@dev` forms' legs and the q8 and recipe forms' decodes feed. The knob stays off.
def private cpu_stage_rows(m : StyleTts2Model; tag : string; dump : table<string; TtsDumpTensor>; noise : TtsNoise) : ParityCase {
    var c <- parity_case(tag)
    c.ids <- dump_tensor(dump, type<int>, "input_ids")
    var inscope style <- dump_tensor(dump, type<float>, "style")
    let hs = m.style_dim / 2l
    c.style_dec <- copy_span(style, 0l, hs)
    c.style_pro <- copy_span(style, hs, hs)
    c.n_tok = long_length(c.ids)
    c.dc = m.predictor.bert_encoder.nout + hs
    c.text_c = m.text_encoder.c
    c.noise := noise
    var inscope sc = St2Scratch()
    var inscope en : array<float>
    set_serving_gpu_tower(false, TowerFamily.tts)
    styletts2_albert(m.albert, c.ids, c.seats[SEAT_ALBERT].cpu, sc)
    styletts2_text_encoder(m.text_encoder, c.ids, c.seats[SEAT_TEXT].cpu, sc)
    styletts2_duration_encoder(m.predictor, c.seats[SEAT_ALBERT].cpu, c.n_tok, c.style_pro, c.seats[SEAT_DUR_ENC].cpu, sc)
    c.frames = styletts2_durations(m.predictor, c.seats[SEAT_DUR_ENC].cpu, c.n_tok, 1.0, c.dur_c, c.seats[SEAT_DURATIONS].cpu, sc)
    align_expand_rows(c.seats[SEAT_DUR_ENC].cpu, c.n_tok, c.dc, c.dur_c, en)
    c.f2 = styletts2_prosody(m.predictor, en, c.dc, c.frames, c.style_pro, c.seats[SEAT_PROSODY].cpu, c.n_c, sc)
    cpu_decode_leg(m, c, SEAT_DECODE)
    return <- c
}

//! `run` with the scratch carrying the device block of `md`, a `@dev` form: a stage called on its own gets no block from a
//! synthesis
def private on_dev(var sc : St2Scratch; md : StyleTts2Model; run : block<() : void>) {
    sc.blk = tts_dev_block(md.dev, md.devplan)
    invoke(run)
    sc.blk = TtsDevBlock()
}

//! A seat's legs on its `@dev` form, the tower knob on: `run` fills the served leg, the seat's counters read across it,
//! then the poisoned leg; all kept in `s` for the compare after the block. False where the seat did not serve once - the
//! case's later seats then keep no leg. Both drivers register every StyleTTS2 seat: a stage no seat serves is a red.
def private seat_dev_legs(t : T?; var s : SeatLegs; run : block<(poisoned : bool; var out : array<float>) : void>) : bool {
    return false if (s.family == TtsFamily.styletts2 && !seat_present(t, s.tag, s.stage))
    set_serving_gpu_tower(true, TowerFamily.tts)
    let h0 = seat_stats(s.family, s.stage)
    let e0 = gpu_encodes()
    invoke(run, false, s.dev)
    let h1 = seat_stats(s.family, s.stage)
    let e1 = gpu_encodes()
    invoke(run, true, s.poisoned)
    s.calls = h1.calls - h0.calls
    s.served = h1.served - h0.served
    s.encodes = e1 - e0
    s.declines = "{gpu_declines()}"
    s.ran = true
    return s.served == 1
}

//! One stage on the `family`'s seat `stage`: `run` fills each leg (the CPU leg's into `cpu`, which the caller keeps), then the verdict; the knob stays on. A StyleTTS2 stage no seat serves is a red.
[unused_argument(t, family, tag, stage, bar, cpu, run)]   // off-Apple the gated body is empty
def seat_leg(t : T?; family : TtsFamily; tag, stage : string; bar : float; var cpu : array<float>; run : block<(leg : TowerLeg; var out : array<float>) : void>) : bool {
    static_if (typeinfo builtin_module_exists(das_metal) || typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        var inscope s = SeatLegs(family = family, stage = stage, tag = tag, bar = bar)
        set_serving_gpu_tower(false, TowerFamily.tts)
        invoke(run, TowerLeg.cpu, cpu)
        seat_dev_legs(t, s) $(poisoned : bool; var out : array<float>) {
            invoke(run, poisoned ? TowerLeg.poisoned : TowerLeg.served, out)
        }
        return s.ran && gpu_stage_verdict(t, tag, stage, s.dev, cpu, s.poisoned, bar, s.calls, s.served, s.encodes, s.declines)
    } else {
        return false
    }
}

//! a case's decode on the decode seat of `md`, a lane's `@dev` form, over the f32 form's inputs: the served leg and, the
//! aligned rows scaled by three, the poisoned one, kept at the lane's decode seat `seat`
def private dev_decode_legs(t : T?; md : StyleTts2Model; var c : ParityCase; seat : int) {
    var inscope sc = St2Scratch()
    var inscope asr : array<float>
    align_expand_channels(c.seats[SEAT_TEXT].cpu, c.text_c, c.n_tok, c.dur_c, asr)
    var tm = TtsTimings()
    seat_dev_legs(t, c.seats[seat]) $(poisoned : bool; var out : array<float>) {
        on_dev(sc, md) {
            if (poisoned) {
                var inscope asr_x <- scaled_copy(asr, 3.0)
                styletts2_decode(md.decoder, c.text_c, md.source, asr_x, c.frames, c.seats[SEAT_PROSODY].cpu, c.n_c, c.f2, c.style_dec, c.noise, 0ul, null, tm, sc)
            } else {
                styletts2_decode(md.decoder, c.text_c, md.source, asr, c.frames, c.seats[SEAT_PROSODY].cpu, c.n_c, c.f2, c.style_dec, c.noise, 0ul, null, tm, sc)
            }
        }
        out := sc.wave
    }
}

//! One case's stages on the seats of `md`, the f32 lane's `@dev` form, each fed the CPU stage's own inputs
//! (`cpu_stage_rows`): the served leg and the poisoned one - the ids reversed, or the input scaled by three - kept for the
//! compare after the block. A seat that did not serve ends the case's legs.
def private dev_stage_legs(t : T?; md : StyleTts2Model; var c : ParityCase) {
    var inscope sc = St2Scratch()
    var inscope rids <- [for (id in reverse(c.ids)); id]
    let albert_served = seat_dev_legs(t, c.seats[SEAT_ALBERT]) $(poisoned : bool; var out : array<float>) {
        on_dev(sc, md) {
            styletts2_albert(md.albert, poisoned ? rids : c.ids, out, sc)
        }
    }
    return if (!albert_served)
    let text_served = seat_dev_legs(t, c.seats[SEAT_TEXT]) $(poisoned : bool; var out : array<float>) {
        on_dev(sc, md) {
            styletts2_text_encoder(md.text_encoder, poisoned ? rids : c.ids, out, sc)
        }
    }
    return if (!text_served)
    let dur_enc_served = seat_dev_legs(t, c.seats[SEAT_DUR_ENC]) $(poisoned : bool; var out : array<float>) {
        on_dev(sc, md) {
            if (poisoned) {
                var inscope bert_x <- scaled_copy(c.seats[SEAT_ALBERT].cpu, 3.0)
                styletts2_duration_encoder(md.predictor, bert_x, c.n_tok, c.style_pro, out, sc)
            } else {
                styletts2_duration_encoder(md.predictor, c.seats[SEAT_ALBERT].cpu, c.n_tok, c.style_pro, out, sc)
            }
        }
    }
    return if (!dur_enc_served)
    let durations_served = seat_dev_legs(t, c.seats[SEAT_DURATIONS]) $(poisoned : bool; var out : array<float>) {
        var inscope dur : array<int>
        on_dev(sc, md) {
            if (poisoned) {
                var inscope d_x <- scaled_copy(c.seats[SEAT_DUR_ENC].cpu, 3.0)
                styletts2_durations(md.predictor, d_x, c.n_tok, 1.0, dur, out, sc)
            } else {
                c.frames_g = styletts2_durations(md.predictor, c.seats[SEAT_DUR_ENC].cpu, c.n_tok, 1.0, dur, out, sc)
            }
        }
    }
    return if (!durations_served)
    var inscope en : array<float>
    align_expand_rows(c.seats[SEAT_DUR_ENC].cpu, c.n_tok, c.dc, c.dur_c, en)
    let prosody_served = seat_dev_legs(t, c.seats[SEAT_PROSODY]) $(poisoned : bool; var out : array<float>) {
        on_dev(sc, md) {
            if (poisoned) {
                var inscope en_x <- scaled_copy(en, 3.0)
                var inscope n_p : array<float>
                styletts2_prosody(md.predictor, en_x, c.dc, c.frames, c.style_pro, out, n_p, sc)
            } else {
                c.f2_g = styletts2_prosody(md.predictor, en, c.dc, c.frames, c.style_pro, out, c.n_g, sc)
            }
        }
    }
    return if (!prosody_served)
    dev_decode_legs(t, md, c, SEAT_DECODE)
}

//! Each kept seat of each case against its CPU leg, after the voice's block (`gpu_stage_verdict`): one hook call, one
//! encode, rel-l2 within the seat's bar, the poisoned leg and one moved element past it - the durations' rounded frame
//! count and the prosody's N rows and frame count beside their seats.
def private parity_seats_held(t : T?; cases : array<ParityCase>) {
    for (c in cases) {
        for (s, i in c.seats, count()) {
            continue if (!s.ran || !gpu_stage_verdict(t, s.tag, s.stage, s.dev, s.cpu, s.poisoned, s.bar, s.calls, s.served, s.encodes, s.declines))
            if (i == SEAT_DURATIONS) {
                t |> equal(c.frames_g, c.frames, "{c.tag} GPU durations: the rounded frame count")
            } elif (i == SEAT_PROSODY) {
                t |> equal(length(c.n_g), length(c.n_c), "{c.tag} GPU prosody: the N rows' length")
                if (length(c.n_g) == length(c.n_c)) {
                    let dn = rel_l2(c.n_g, c.n_c)
                    t |> success(dn <= GPU_PRED_BAR, "{c.tag} GPU prosody: the N rows within {GPU_PRED_BAR} (rel-l2 {dn})")
                }
                t |> equal(c.f2_g, c.f2, "{c.tag} GPU prosody: the frame count")
            }
        }
    }
}

//! A parity leg's own verdict on the seat `s`: the CPU chain's leg kept finite rows; a tower leg (`on_tower`) ran, served
//! its seat once - a skip where no GPU device serves, a red where a present device declined or the seat is not
//! registered - and kept finite rows. False where the seat did not serve: the case's later seats keep no leg.
def private parity_leg_held(t : T?; s : SeatLegs; on_tower : bool) : bool {
    let name = "{s.tag} {on_tower ? "GPU" : "CPU"} {s.stage}"
    if (!on_tower) {
        t |> success(!empty(s.cpu) && all_finite(s.cpu),
            "{name}: the CPU chain's leg kept {length(s.cpu)} finite rows")
        return true
    }
    if (!s.ran) {
        t |> failure("{name}: the seat kept no tower leg")
        return false
    }
    return tower_served(t, name, s.declines) if (s.served != 1)
    t |> success(!empty(s.dev) && all_finite(s.dev),
        "{name}: the seat served once and kept {length(s.dev)} finite rows")
    return true
}

//! A lane's legs of the parity rail's seats on `m`, the voice's form `form`, over the f32 form's
//! cases: the q8 and recipe forms' decode on their CPU chain, an `@dev` form's seats - each leg
//! held to its own verdict (`parity_leg_held`). Skips where `rig`'s dumps are not stocked; a
//! stocked oracle that left the form no bring-up case is a red.
def private tts_parity_legs(t : T?; rig : TtsParityRig; m : StyleTts2Model; form : string; var cases : array<ParityCase>) {
    var inscope mf <- rig_manifest(t, rig)
    return if (empty(mf.cases))
    if (empty(cases)) {
        t |> failure("{rig.oracle_dir}: no bring-up case reached the {form} form - its seats gate nothing")
        return
    }
    let f = tts_form_of(form)
    let seat = decode_seat(f.lane)
    with_reference_f32_tile(t) {   // the bars are measured at the tile's reference order, whatever this box crowned
        for (c in cases) {
            with_gpu_knobs() {
                if (f.home != "dev") {
                    cpu_decode_leg(m, c, seat)
                } elif (f.lane == "f32") {
                    dev_stage_legs(t, m, c)
                } else {
                    dev_decode_legs(t, m, c, seat)
                }
            }
            let first = f.home == "dev" && f.lane == "f32" ? SEAT_ALBERT : seat
            for (i in range(first, seat + 1)) {
                break if (!parity_leg_held(t, c.seats[i], f.home == "dev"))
            }
        }
    }
}

//! every later StyleTTS2 stage of the GPU image `md`, called on its own with the tower off: each refuses by its own name
def private st2_stage_refusals(t : T?; name : string; md : StyleTts2Model) {
    var inscope sc = St2Scratch()
    var inscope ids <- [1, 2, 3]
    var inscope none, out, aux : array<float>
    var inscope dur : array<int>
    var inscope noise = TtsNoise()
    var tm = TtsTimings()
    let stages = fixed_array("text encoder", "duration encoder", "duration head", "prosody stage", "generator", "decoder")
    for (i, stage in count(), stages) {
        sc.blk = tts_dev_block(md.dev, md.devplan)
        let why = panic_text() $ {
            if (i == 0) {
                styletts2_text_encoder(md.text_encoder, ids, out, sc)
            } elif (i == 1) {
                styletts2_duration_encoder(md.predictor, none, 0l, none, out, sc)
            } elif (i == 2) {
                styletts2_durations(md.predictor, none, 0l, 1.0, dur, out, sc)
            } elif (i == 3) {
                styletts2_prosody(md.predictor, none, 0l, 0l, none, out, aux, sc)
            } elif (i == 4) {
                styletts2_generator_rows(md.decoder, aux, 0l, 0l, none, none, 0l, null, out, sc)
            } else {
                styletts2_decode(md.decoder, 0l, md.source, none, 0l, none, none, 0l, none, noise, 0ul, null, tm, sc)
            }
        }
        sc.blk = TtsDevBlock()
        t |> success(find(why, "the {stage} did not run on the device") >= 0, "{name}: with the tower off the {stage} called on its own refuses by name: '{why}'")
    }
}

//! every Pocket stage of the GPU image `pm`, entered on its own over a scratch of its own: the
//! codec, the codec encoder, the voice state's pass and the prompt's pass each refuse by name with
//! the tower off, and the frame loop with the tower on and its seat withdrawn
def private pocket_stage_refusals(t : T?; name : string; pm : PocketModel) {
    var inscope sc = PocketScratch()
    var inscope none : array<float>
    var inscope lat <- [for (_ in range64(2l * pm.latent_dim)); 0.0]
    let ids <- [1l, 2l, 3l]
    let refused = $(why, stage : string) {
        t |> success(find(why, "the {stage} did not run on the device") >= 0, "{name}: the {stage} refuses by name: '{why}'")
    }
    refused(panic_text() $ {
        var inscope wave : array<float>
        pocket_decode_latents(pm, lat, 2l, sc, wave)
    }, "codec")
    if (pm.cloning) {
        var inscope pcm <- [for (_ in range(4800)); 0.0]
        var inscope got : array<float>
        refused(panic_text() $ {
            pocket_encode_latents(pm, pcm, sc, got)
        }, "codec encoder")
    }
    refused(panic_text() $ {
        var inscope lost <- voice_state_from_latents(pm, lat, 2l, sc)
    }, "voice state's backbone pass")
    set_serving_gpu_tower(true, TowerFamily.tts)
    var inscope vs <- voice_state_from_latents(pm, lat, 2l, sc)   // the tower on: a voice state for the two stages after it
    set_serving_gpu_tower(false, TowerFamily.tts)
    refused(panic_text() $ {
        pocket_prompt_rows(pm, ids, vs, sc)
    }, "prompt's backbone pass")
    set_serving_gpu_tower(true, TowerFamily.tts)
    let drv = pocket_gpu_driver()
    register_pocket_gpu(PocketGpuDriver(encode = drv.encode, voice = drv.voice, codec = drv.codec, prompt = drv.prompt))
    let why = panic_text() $ {
        var tm = TtsTimings()
        pocket_synthesize(pm, ids, vs, 0.7, 2, none, none, tm, sc)
    }
    unregister_pocket_gpu()
    set_serving_gpu_tower(false, TowerFamily.tts)
    refused(why, "frame loop")
}

def tts_knob_flip_refuses(t : T?; var m : TtsModel; name, text, voice : string) {
    //! On `m`, a `@dev` form on the unpinned lane - a GPU home's image as a load serves it - named
    //! `name`, the tower knob going off after the mint: it spoke with the knob on, and its next
    //! synthesis refuses by name and points at the source model: a GPU image never runs on the
    //! CPU; then every other stage, entered on its own over a scratch of its own, refuses by its
    //! own name; and with the knob back on the model speaks again.
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.tts)
        let pieces = m.kind == TtsKind.pocket ? m.pocket.devplan.n : m.model.devplan.n
        t |> success(pieces > 0l, "{name}: with the device up the home's flavor mints a device block ({pieces} pieces)")
        return if (pieces == 0l)
        var inscope on <- synthesize(m, text, voice)
        t |> success(logits_maxabs(on.pcm) > 0.05, "{name}: the home's image speaks with its tower on")
        set_serving_gpu_tower(false, TowerFamily.tts)
        let why = panic_text() $ {
            var inscope off <- synthesize(m, text, voice)
        }
        t |> success(find(why, "never runs on the CPU") >= 0 && find(why, "load the source model") >= 0,
            "{name}: with the tower off the image's next synthesis refuses by name: '{why}'")
        if (m.kind == TtsKind.pocket) {
            pocket_stage_refusals(t, name, m.pocket)
        } else {
            st2_stage_refusals(t, name, m.model)
        }
        set_serving_gpu_tower(true, TowerFamily.tts)
        var inscope again <- synthesize(m, text, voice)
        t |> success(logits_maxabs(again.pcm) > 0.05,
            "{name}: with the tower back on the refused image speaks again")
    }
}

[cold_path]
def tts_unbuilt_seats_take_cpu_image(t : T?; path, text, voice : string; pocket : bool) {
    //! A home whose seats' classes did not build (`set_tts_ready_fault_`, put back) picks no
    //! flavor: the load of the file at `path` mints the CPU image, which speaks on the CPU chain
    //! with no seat served; the same pick with the classes built names the home (the control). The
    //! cell's own load - it asserts on the image flavor the load lands on. Skips where no GPU
    //! device serves the TTS seats.
    let name = base_name(path)
    let lacks = tts_dev_why()
    if (!empty(lacks)) {
        t |> skip("{name}: {lacks}")
        return
    }
    with_gpu_cell() {
        set_serving_gpu_tower(true, TowerFamily.tts)
        tts_pins_unset("{name}: the unbuilt-seats cell")
        let pick = $ => pocket ? pocket_dev_flavor(pocket_serves_native()) : styletts2_dev_flavor(false)
        t |> success(pick() != TowerDev.none, "{name}: with the seats' classes built the home's flavor is picked (the control)")
        let was = set_tts_ready_fault_(true)
        var unbuilt = TowerDev.none
        var inscope m = TtsModel()
        with_put_back($ {
            set_tts_ready_fault_(was)   // a mint that panics leaves no fault on for the file's other cells
        }) {
            unbuilt = pick()
            delete m
            m <- pocket ? pocket_tts_minted(path, PkMint.device) : styletts2_form(path, "q8@dev")
        }
        t |> equal(unbuilt, TowerDev.none, "{name}: with the classes read as not built no flavor is picked")
        let pieces = pocket ? m.pocket.devplan.n : m.model.devplan.n
        t |> equal(pieces, 0l, "{name}: the load mints the CPU image, no device block")
        let e0 = gpu_encodes()
        var inscope a <- synthesize(m, text, voice)
        t |> success(logits_maxabs(a.pcm) > 0.05, "{name}: the CPU image speaks")
        t |> equal(gpu_encodes() - e0, 0l, "{name}: no seat served it")
    }
}

//! The seats one served chunk encodes: every seat of the engine's record.
def private served_seat_count() : int64 => long_length(styletts2_gpu_seats())

//! the serving home's launches of the tiles that read stored Q4_K planes off the exact tile and the row GEMV: a home's own
//! tile, the discrete homes' schedule-fed one
def private k4_tile_launches() : int64 => gpu_launches(["_k4_"]) - gpu_launches(["kq_f32_gemm_k4"]) - gpu_launches(["kq_tts_pkst_k4"])

//! on the Vulkan home the recipe lane's stored Q4_K convs ride the schedule-fed tile exactly where the device serves a
//! cooperative-matrix feed, and the exact tile alone where it serves none; a lane with no Q4_K leaf launches none (the control)
[unused_argument(t, msg, tile, recipe)]   // off the Vulkan home there is no schedule-fed tile to hold
def private vulkan_sched_tile_held(t : T?; msg : string; tile : int64; recipe : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return if (serving_tower_dev(TowerFamily.tts) != TowerDev.vulkan)
        let feed = vt_cm2_feed()
        if (recipe) {
            t |> success(feed ? tile > 0l : tile == 0l, "{msg}: the schedule-fed tile launches over the Q4_K convs exactly where the device has a cooperative-matrix feed ({tile} launches, feed {feed})")
        } else {
            t |> equal(tile, 0l, "{msg}: a lane with no Q4_K leaf launches no Q4_K tile")
        }
    }
}

//! the Metal home's stored-plane launches now - the Q8_0 GEMM's, the Q8_0 recurrence's, the K-quant GEMM's; zeros off an
//! Apple build
def private metal_stored_now() : int64[3] {
    var n : int64[3]
    static_if (typeinfo builtin_module_exists(das_metal)) {
        n[0] = metal_stored_launches("st2_q8_", "q8_f32_gemm_cls")
        n[1] = metal_stored_launches("st2_lstm_q8", "tts_lstm_dir_q8_cls")
        n[2] = metal_stored_launches("st2_k", "kq_f32_gemm_k")
    }
    return n
}

def private seat_stats(family : TtsFamily; seat : string) : tuple<calls : int; served : int>
    => family == TtsFamily.pocket ? pocket_gpu_stats(seat) : styletts2_gpu_stats(seat)

def private seat_counters(family : TtsFamily; seats : array<string>) : array<tuple<calls : int; served : int>>
    => <- [for (s in seats); seat_stats(family, s)]

//! each seat's calls and serves from `before` to `after`
def private counters_moved(before, after : array<tuple<calls : int; served : int>>) : array<tuple<calls : int; served : int>>
    => <- [for (x, y in before, after); (calls = y.calls - x.calls, served = y.served - x.served)]

//! One lane of the served synthesis across the tower knob as its two forms keep it: the CPU form's tower-off leg (`a`, the
//! seats' counters, the knob declines, the loader's tags) and the `@dev` form's tower-on leg (`b`, the counters, the
//! encodes, the stored members' launches), held after the block
struct SynthLane {
    lane : string   //! the lane's CPU form's name
    cpu_ran : bool
    dev_ran : bool
    a : TtsAudio = TtsAudio()
    b : TtsAudio = TtsAudio()
    seats : array<string>
    off : array<tuple<calls : int; served : int>>   //! each seat's calls and serves across the tower-off leg
    on : array<tuple<calls : int; served : int>>    //! ... across the tower-on leg
    knob_declines : int64   //! the knob declines the tower-off leg recorded
    voice_calls : int       //! the voice seat's calls across it: a voice's first line builds its state, the voice seat once
    cpu_encodes : int64     //! the home's encodes across the tower-off leg
    cpu_launches : int64    //! ... and its kernel launches
    encodes : int64         //! ... across the tower-on leg
    declines : string
    launched : table<string; int64>   //! the kernels the tower-on leg launched (`census_gain`)
    widened : int64       //! the home's dense dispatches over widened rows across the tower-on leg
    cpu_widened : int64   //! ... across the tower-off leg, the count's control
    off_lattice_noise_convs : int64   //! the `@dev` form's noise convs off the stored members' lattice
    te_fmt : int   //! the K-quant format the loader tagged the CPU image's text-encoder convs, 0 none
    up_fmt : int   //! ... its upsamplers
    stored : int64[5]   //! the tower-on leg's launches of each stored member's stamps (`STORED_MEMBERS`)
    tile : int64        //! ... of the Q4_K tiles (`k4_tile_launches`)
    metal_on : int64[3]    //! the Metal home's stored-plane launches across the tower-on leg (`metal_stored_now`)
    metal_off : int64[3]   //! ... across the tower-off leg, the launch counts' control
    noise_rows : int64   //! the reference lane's captured-noise rows, sized off the CPU form's untrimmed count
    dec_served : int     //! the reference lane's decodes served
}

//! the lane record of `form`'s lane in `lanes`, -1 where the lane kept none
def private lane_at(lanes : array<SynthLane>; form : string) : int {
    let lane = tts_form_at(form, "")
    for (k, i in lanes, count()) {
        return i if (k.lane == lane)
    }
    return -1
}

//! the serves a leg's seats counted
def private seats_served(moved : array<tuple<calls : int; served : int>>) : int {
    var served = 0
    for (z in moved) {
        served += z.served
    }
    return served
}

//! the lane record of `form`'s lane in `lanes`, added where the lane has none
def synth_lane(var lanes : array<SynthLane>; form : string) : int {
    let at = lane_at(lanes, form)
    return at if (at >= 0)
    lanes |> emplace(SynthLane(lane = tts_form_at(form, "")))
    return length(lanes) - 1
}

def served_lane_cpu_leg(var k : SynthLane; var mc : TtsModel; text, voice : string) {
    //! The tower-off leg of a served lane on `mc`, the lane's CPU image: the knob off, each seat's
    //! calls and serves and the knob declines across one synthesis of `text`. The knob stays off.
    let family = tts_family(mc.kind)
    k.seats <- family == TtsFamily.pocket ? pocket_gpu_seats() : styletts2_gpu_seats()
    set_serving_gpu_tower(false, TowerFamily.tts)
    var inscope h0 <- seat_counters(family, k.seats)
    var inscope d0 <- gpu_declines()
    let v0 = family == TtsFamily.pocket ? pocket_voice_gpu_stats().calls : 0
    let m0 = metal_stored_now()
    let e0 = gpu_encodes()
    let w0 = gpu_widened()
    var inscope before <- gpu_census()
    k.a <- synthesize(mc, text, voice)
    var inscope after <- gpu_census()
    k.cpu_launches = census_total(after) - census_total(before)
    k.cpu_encodes = gpu_encodes() - e0
    k.cpu_widened = gpu_widened() - w0
    var inscope h1 <- seat_counters(family, k.seats)
    var inscope d1 <- gpu_declines()
    let m1 = metal_stored_now()
    k.voice_calls = (family == TtsFamily.pocket ? pocket_voice_gpu_stats().calls : 0) - v0
    k.off <- counters_moved(h0, h1)
    k.knob_declines = (d1?["knob"] ?? 0l) - (d0?["knob"] ?? 0l)
    for (i in range(3)) {
        k.metal_off[i] = m1[i] - m0[i]
    }
    k.cpu_ran = true
}

def served_lane_dev_leg(var k : SynthLane; var m : TtsModel; text, voice : string) {
    //! The tower-on leg of a served lane on `m`, the lane's image as the box's GPU home serves it:
    //! the knob on, each seat's calls and serves, the encodes, the kernels launched and the stored
    //! members' launches among them across one synthesis of `text`. The knob stays on.
    let family = tts_family(m.kind)
    var inscope seats <- family == TtsFamily.pocket ? pocket_gpu_seats() : styletts2_gpu_seats()
    set_serving_gpu_tower(true, TowerFamily.tts)
    var inscope h1 <- seat_counters(family, seats)
    let tile0 = k4_tile_launches()
    let m0 = metal_stored_now()
    let e1 = gpu_encodes()
    let w1 = gpu_widened()
    var inscope before <- gpu_census()
    k.b <- synthesize(m, text, voice)
    var inscope after <- gpu_census()
    k.launched <- census_gain(before, after)
    k.encodes = gpu_encodes() - e1
    k.widened = gpu_widened() - w1
    k.off_lattice_noise_convs = family == TtsFamily.pocket ? 0l : st2_off_lattice_convs(m.model)
    var inscope h2 <- seat_counters(family, seats)
    k.on <- counters_moved(h1, h2)
    k.stored = stored_marks(k.launched)
    k.tile = k4_tile_launches() - tile0
    let m1 = metal_stored_now()
    for (i in range(3)) {
        k.metal_on[i] = m1[i] - m0[i]
    }
    k.declines = "{gpu_declines()}"
    k.dev_ran = true
}

def served_lane_held(t : T?; k : SynthLane; msg : string; extra : block<(a, b : TtsAudio; chunks : int; encodes : int64) : void>) : bool {
    //! A served lane held, after its block, `msg` its name: tower off on the CPU image each seat
    //! reached once a chunk on every lane, none served, one knob decline each and the voice seat
    //! one a state it built;
    //! tower on, the home's image served each seat once a chunk; then `extra`. False where a leg is
    //! missing, or where the tower served nothing - a skip where no GPU device serves, a red where
    //! a present device declined.
    return false if (!k.cpu_ran || !k.dev_ran)
    return tower_served(t, msg, k.declines) if (seats_served(k.on) == 0)
    let chunks = k.on[0].calls
    let off_chunks = k.off[0].calls
    t |> success(chunks >= 1, "{msg}: {chunks} chunk(s) reached the seats with the tower on")
    t |> equal(off_chunks, chunks, "{msg}: every tower-off chunk reached the seats on any lane")
    for (s, x, z in k.seats, k.off, k.on) {
        t |> equal(x.calls, off_chunks, "{msg}: every tower-off chunk that reached a seat reached the {s} seat")
        t |> equal(x.served, 0, "{msg}: the tower-off chunks served nothing at the {s} seat")
        t |> equal(z.served, chunks, "{msg}: every chunk served on the tower at the {s} seat")
    }
    t |> equal(k.knob_declines, int64(off_chunks) * long_length(k.seats) + int64(k.voice_calls),
        "{msg}: every tower-off chunk that reached the seats recorded a knob decline per seat, and the voice seat one a state it built ({k.voice_calls})")
    invoke(extra, k.a, k.b, chunks, k.encodes)
    return true
}

//! the name a lane form's cells give its lane
def private st2_lane_name(lane : string) : string => lane == "f32" ? "f32 reference" : (lane == "kq" ? "recipe" : "served")

//! the groups a form pins, as a lane's name carries them
def private st2_pinned(f : TtsForm) : string => empty(f.pins) ? "" : " pinned {f.pins}"

//! the name a served lane's asserts carry: the voice's file, the lane and its pinned groups
def private st2_lane_msg(file, lane : string) : string {
    let f = tts_form_of(lane)
    return "{file} {st2_lane_name(f.lane)} lane{st2_pinned(f)}"
}

//! A quantized lane of a StyleTTS2 voice's served synthesis held (`served_lane_held`): a tower
//! encode a seat a chunk, an audible leg, samples within a twentieth of the CPU leg's.
def private st2_served_lane_held(t : T?; k : SynthLane; file, voice : string) {
    let msg = st2_lane_msg(file, k.lane)
    served_lane_held(t, k, msg) $(a, b : TtsAudio; chunks : int; encodes : int64) {
        t |> equal(encodes, int64(chunks) * served_seat_count(), "{msg}: one tower encode per seat per chunk")
        let peak = logits_maxabs(b.pcm)
        t |> success(peak > 0.05, "peak {peak}: the tower leg spoke")
        var inscope silent <- scaled_copy(b.pcm, 0.0)
        t |> success(logits_maxabs(silent) <= 0.05, "silence reads under the floor (the floor's control)")
        to_log(LOG_INFO, "{file} {voice}: served lane, tower vs CPU: {length(b.pcm)} / {length(a.pcm)} samples, sample-wise rel-l2 {rel_l2_common(b.pcm, a.pcm)} over the common length (the q8 chain's own rounding, informational); CPU {timings_line(a.timings, a.sample_rate)}; tower {timings_line(b.timings, b.sample_rate)}\n")
        t |> success(within_twentieth(long_length(b.pcm), long_length(a.pcm)), "the sample counts {length(b.pcm)} against {length(a.pcm)} within a twentieth")
        t |> success(!within_twentieth(long_length(b.pcm), long_length(a.pcm) / 2l), "half the count reds the twentieth (the bound's control)")
    }
}

//! the launches in `cov` of the Metal home's own f32 tiles - its dense tile, its conv tiles over slab rows - and the f32
//! recurrence
def private metal_own_f32_launches(cov : table<string; int64>) : int64
    => (census_launches(cov, ["metal_f32_exact_"]) + census_launches(cov, ["metal_st2_conv_"])
        + census_launches(cov, ["tts_lstm_dir_cls"]))

def st2_stored_formats_held(t : T?; lanes : array<SynthLane>; file, form : string; f32_lane : bool) {
    //! A quantized lane's `@dev` form of a StyleTTS2 voice serves each stored format as the image
    //! holds it, on the tower leg `lanes` keeps of `form`: the loader's tags on the lane's CPU
    //! image, each tagged format's stamps launched and no other format's, the Vulkan fed tile, the
    //! Metal stored stamps; and the f32 tiles, which a leaf widened into the slab would launch -
    //! none of the Metal home's own and, on a block that carries the f32 lane (`f32_lane`), fewer
    //! dense sites than on that lane's image, whose `@dev` leg is the control
    //! (`reference_formats_held`); and the home's count of dense dispatches over widened rows,
    //! exact: one a chunk for each noise conv the form holds off the members' lattice
    //! (`st2_off_lattice_convs`), none with the tower off. A red where the form, or the f32 lane
    //! asked for, kept no tower leg.
    let at = lane_at(lanes, form)
    let ref = f32_lane ? lane_at(lanes, "f32") : -1
    let msg = st2_lane_msg(file, tts_form_at(form, ""))
    let kept = at >= 0 && lanes[at].dev_ran && (!f32_lane || (ref >= 0 && lanes[ref].dev_ran))
    t |> success(kept, "{msg}: the {form} form kept a tower leg"
        + (f32_lane ? ", and so did the f32 lane's `@dev` form" : ""))
    return if (!kept)
    let k & = unsafe(lanes[at])
    return if (seats_served(k.on) == 0 && !tower_served(t, msg, k.declines))
    let f = tts_form_of(k.lane)
    let recipe = f.lane == "kq"
    to_log(LOG_INFO, "{msg}: the tower leg launched {census_text(k.launched)}\n")
    t |> success(recipe ? k.te_fmt != 0 && k.up_fmt != 0 && k.up_fmt != k.te_fmt : k.te_fmt == 0 && k.up_fmt == 0,
        "{msg}: the loader tags the text encoder's convs and the upsamplers {recipe ? "two K-quant formats" : "no K-quant format"} (ids {k.te_fmt}, {k.up_fmt})")
    let held = $(id : int) => recipe && (k.te_fmt == id || k.up_fmt == id) ? STORED_HELD : STORED_ABSENT
    t |> success(empty(f.pins) || k.up_fmt == 5, "{msg}: the pinned upsamplers are Q5_K planes (id {k.up_fmt})")
    stored_members_held(t, msg, k.stored, fixed_array(STORED_HELD, held(4), held(5), held(6), STORED_UNTAGGED))
    vulkan_sched_tile_held(t, msg, k.tile, recipe)
    if (f32_lane) {
        let rows = f32_row_launches(k.launched)
        let ref_rows = f32_row_launches(lanes[ref].launched)
        t |> success(rows < ref_rows, "{msg}: the f32 tiles launched at fewer dense sites than on the "
            + "f32 lane's image ({rows} against {ref_rows}) - the stored leaves' sites ran their "
            + "format's stamps, not a widened copy's")
    }
    let chunks = int64(k.on[0].calls)
    t |> equal(k.widened, chunks * k.off_lattice_noise_convs, "{msg}: the dense dispatches over widened rows "
        + "are the {k.off_lattice_noise_convs} noise conv(s) off the 8 lattice, one a chunk over {chunks} "
        + "chunk(s) - no leaf on a stored member's lattice was widened into the slab")
    t |> equal(k.cpu_widened, 0l, "{msg}: the tower off, no dispatch over widened rows is counted "
        + "(the count's control)")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        to_log(LOG_INFO, "{msg}: stored-plane launches - Q8_0 GEMM {k.metal_on[0]}, Q8_0 recurrence {k.metal_on[1]}, K-quant GEMM {k.metal_on[2]}\n")
        t |> success(k.metal_on[0] > 0l && k.metal_on[1] > 0l, "{msg}: the Q8_0 planes were read as stored ({k.metal_on[0]} GEMM launches, {k.metal_on[1]} recurrence launches)")
        t |> success(recipe ? k.metal_on[2] > 0l : k.metal_on[2] == 0l, "{msg}: {recipe ? "the recipe's K-quant planes were read as stored" : "a lane with no K-quant plane launches no K-quant stamp"} ({k.metal_on[2]})")
        t |> success(k.metal_off[0] == 0l && k.metal_off[1] == 0l, "{msg}: the tower off, the CPU image launches no stored-plane stamp (the launch counts' control)")
        let own = metal_own_f32_launches(k.launched)
        t |> equal(own, 0l, "{msg}: the Metal home's own f32 tiles and the f32 recurrence launched "
            + "none - every dense layer, lattice conv and recurrence the lane stores read its "
            + "stored plane")
    }
}

def f32_lane_held(t : T?; tag : string; launched : table<string; int64>; rows : int64; readers : string) {
    //! An f32 lane's tower leg, `launched` its kernels, launched `rows` times the ones that read f32 slab rows
    //! (`readers`) and no stamp of a quant format - the control of the cells that hold a stored lane to its stamps.
    to_log(LOG_INFO, "{tag}: the tower leg launched {census_text(launched)}\n")
    t |> success(rows > 0l, "{tag}: the f32 lane's dense sites launched {readers} ({rows})")
    stored_members_held(t, tag, stored_marks(launched),
        fixed_array(STORED_ABSENT, STORED_ABSENT, STORED_ABSENT, STORED_ABSENT, STORED_UNTAGGED))
}

//! The f32 lane's tower leg launches the f32 tiles at its dense sites and no stamp of a quant format - the control of the
//! stored lanes' cells (`st2_stored_formats_held`); on the Metal home its own f32 tiles among them.
def private reference_formats_held(t : T?; k : SynthLane; msg : string) {
    f32_lane_held(t, msg, k.launched, f32_row_launches(k.launched), "the f32 tiles")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let own = metal_own_f32_launches(k.launched)
        t |> success(own > 0l,
            "{msg}: the Metal home's own f32 tiles and the f32 recurrence launched ({own})")
    }
}

def synth_leg_held(t : T?; name : string; audio : TtsAudio; encodes, launches : int64;
                   on_tower : bool; declines : string) {
    //! A synthesis leg's own verdict, `name` the cell's: it spoke finite samples on the chain its
    //! form names - `on_tower`, the home's seats encoding and its kernels launching (a skip where
    //! no GPU device serves, a red where a present device declined); else no seat encoding and no
    //! kernel launched on the home, the control of every launch count a tower leg holds.
    return if (on_tower && encodes == 0l && !tower_served(t, name, declines))
    t |> success(!empty(audio.pcm) && speech_level(audio.pcm).finite,
        "{name}: the leg spoke {length(audio.pcm)} finite samples")
    if (on_tower) {
        t |> success(encodes > 0l && launches > 0l,
            "{name}: the home's seats encoded the leg ({encodes} encodes, {launches} launches)")
    } else {
        t |> equal(encodes, 0l, "{name}: the tower off, no seat encoded the CPU chain's leg")
        t |> equal(launches, 0l, "{name}: the tower off, the home launched no kernel")
    }
}

//! The reference lane's tower-off leg on `mc`, the f32 form - the lane where alone the durations round as the CPU's: the
//! untrimmed count sizes the captured noise both legs read, then the synthesis over it. The knob stays off.
def private reference_lane_cpu_leg(var k : SynthLane; var mc : TtsModel; text, voice : string) {
    set_serving_gpu_tower(false, TowerFamily.tts)
    let e0 = gpu_encodes()
    var inscope before <- gpu_census()
    var inscope sized <- synthesize(mc, text, voice)   // the untrimmed count the captured stream covers
    k.noise_rows = 2l * sized.timings.samples + 65536l   // the source's sample count runs past the generator's output; twice covers it
    mc.scratch.noise <- captured_noise(mc.model.source.n_harm, k.noise_rows, 0x5eedul)
    k.a <- synthesize(mc, text, voice)
    var inscope after <- gpu_census()
    k.cpu_launches = census_total(after) - census_total(before)
    k.cpu_encodes = gpu_encodes() - e0
    k.cpu_ran = true
}

//! The reference lane's tower-on leg on `m`, the f32 lane's `@dev` form, over the noise the CPU leg captured, the kernels it
//! launched kept. The knob stays on.
def private reference_lane_dev_leg(var k : SynthLane; var m : TtsModel; text, voice : string) {
    m.scratch.noise <- captured_noise(m.model.source.n_harm, k.noise_rows, 0x5eedul)
    set_serving_gpu_tower(true, TowerFamily.tts)
    let h1 = styletts2_gpu_stats("decode")
    let e1 = gpu_encodes()
    var inscope before <- gpu_census()
    k.b <- synthesize(m, text, voice)
    var inscope after <- gpu_census()
    k.launched <- census_gain(before, after)
    k.stored = stored_marks(k.launched)
    k.dec_served = styletts2_gpu_stats("decode").served - h1.served
    k.encodes = gpu_encodes() - e1
    k.declines = "{gpu_declines()}"
    k.dev_ran = true
}

//! the reference lane held, after its block: the tower encoded every served decode, and the two legs' sample counts agree -
//! the durations token for token
def private reference_lane_held(t : T?; k : SynthLane; file, voice : string) {
    return if (!k.cpu_ran || !k.dev_ran || (k.dec_served == 0 && !tower_served(t, "{file} reference lane", k.declines)))
    t |> success(k.encodes >= int64(k.dec_served), "reference lane: the tower encoded every served decode")
    t |> equal(length(k.b.pcm), length(k.a.pcm), "reference lane: the sample counts (the durations token for token)")
    to_log(LOG_INFO, "{file} {voice}: reference lane, tower vs CPU on one captured noise: {length(k.b.pcm)} / {length(k.a.pcm)} samples, sample-wise rel-l2 {rel_l2_common(k.b.pcm, k.a.pcm)} (the f32 phase's ulps, informational); CPU {timings_line(k.a.timings, k.a.sample_rate)}; tower {timings_line(k.b.timings, k.b.sample_rate)}\n")
}

def tts_gpu_synthesis_lane(t : T?; var m : TtsModel; path, form, text, voice : string; var lanes : array<SynthLane>) {
    //! One form's leg of the served synthesis across the tower knob on the voice at `path`, kept in
    //! `lanes` for the compare after the block: the f32 forms the reference lane, the q8 forms the
    //! lane an unpinned load serves, the kq forms the recipe lane and, pinned, the recipe lane with
    //! those groups pinned - a lane's CPU form its tower-off leg, its `@dev` form its tower-on leg.
    //! The leg holds its own verdict (`synth_leg_held`), the f32 lane's tower leg also the stored
    //! lanes' control (`reference_formats_held`). Skips without the front-end packs, and on the f32
    //! form where no device serves the tower leg its CPU leg is read beside; an `@dev` form whose
    //! lane kept no CPU leg is a red.
    return if (tts_data_missing(t, path))
    let f = tts_form_of(form)
    let name = "{base_name(path)} {form}"
    if (f.home != "dev" && f.lane == "f32") {
        let cpu_leg_why = tts_cpu_leg_why(tts_dev_why())
        if (!empty(cpu_leg_why)) {
            t |> skip("{name}: the reference lane's CPU leg does not run - {cpu_leg_why}")
            return
        }
    }
    with_gpu_knobs() {
        var k & = unsafe(lanes[synth_lane(lanes, form)])
        if (f.home != "dev") {
            if (f.lane == "f32") {
                reference_lane_cpu_leg(k, m, text, voice)
            } else {
                served_lane_cpu_leg(k, m, text, voice)
                k.te_fmt = m.model.text_encoder.cnn[0].kq_fmt
                k.up_fmt = m.model.decoder.ups[0].kq_fmt
            }
            synth_leg_held(t, name, k.a, k.cpu_encodes, k.cpu_launches, on_tower = false, declines = "")
        } elif (!k.cpu_ran) {
            t |> failure("{base_name(path)}: the {form} form ran with no tower-off leg of its lane kept")
        } else {
            if (f.lane == "f32") {
                reference_lane_dev_leg(k, m, text, voice)
            } else {
                served_lane_dev_leg(k, m, text, voice)
            }
            synth_leg_held(t, name, k.b, k.encodes, census_total(k.launched), on_tower = true,
                declines = k.declines)
            if (f.lane == "f32" && k.encodes > 0l) {
                reference_formats_held(t, k, name)
            }
        }
    }
}

//! every lane of a StyleTTS2 voice's served synthesis held, after its block (`reference_lane_held`, `st2_served_lane_held`)
def private st2_synth_lanes_held(t : T?; lanes : array<SynthLane>; file, voice : string) {
    for (k in lanes) {
        if (k.lane == "f32") {
            reference_lane_held(t, k, file, voice)
        } else {
            st2_served_lane_held(t, k, file, voice)
        }
    }
}

def private stage_checks(t : T?; tag : string; tr : StyleTts2Trace?; dump : table<string; TtsDumpTensor>) {
    stage_diff(t, tag, "bert", tr.bert, dump)
    stage_diff(t, tag, "text_enc", tr.text_enc, dump)
    stage_diff(t, tag, "dur_raw", tr.dur_raw, dump)
    stage_diff(t, tag, "f0", tr.f0, dump)
    stage_diff(t, tag, "n", tr.n, dump)
    stage_diff(t, tag, "asr_res", tr.asr_res, dump)
    stage_diff(t, tag, "dec_in", tr.dec_in, dump)
    stage_diff(t, tag, "gen_in", tr.gen_in, dump)
}

//! `n_ids` valid token ids through `styletts2_synthesize` in the model's first voice: the ids it fed, the samples it
//! returns, the samples the timings count, and the wave's level
[cold_path]
def styletts2_spoken(m : StyleTts2Model; n_ids : int) : tuple<ids : int; samples : int64; counted : int64; level : tuple<finite : bool; peak : float; rms : float>> {
    var inscope ids <- [for (i in range(n_ids)); 1 + i % (int(m.n_symbols) - 1)]
    var inscope style : array<float>
    styletts2_style(m, m.voice_names[0], 0, style)
    var inscope noise = TtsNoise()
    var tm = TtsTimings()
    var inscope sc = St2Scratch()
    let samples = styletts2_synthesize(m, ids, style, 1.0, noise, 0x5eedul, null, tm, sc)
    return (ids = length(ids), samples = samples, counted = tm.samples, level = speech_level(sc.wave))
}

//! the roles the StyleTTS2 decode shot stamps, on every home
let private DECODE_PROF_ROLES = fixed_array(RailProfRole.tts_gemm, RailProfRole.tts_im2col, RailProfRole.tts_act,
    RailProfRole.tts_adain, RailProfRole.tts_rows, RailProfRole.tts_source, RailProfRole.tts_stft)

//! what the profile cell's pinned leg leaves its pin-off leg: the decode shot's ledger and the samples spoken
struct private ProfiledLeg {
    sums : RailProfSums
    samples : int64
}

[cold_path]
def tts_seat_profile_cell(t : T?; m : TtsModel; name : string) {
    //! The decode seat's profile ledger on `m`, a `@dev` form of a StyleTTS2 voice, as two cells
    //! `name`: with the ledger pinned on, one synthesis leaves the decode shot's ledger - a
    //! synthesis's last shot - holding stamps and device time on each role the decode stamps and
    //! none of PL-BERT's attention rows; then the same synthesis with the pin off leaves the last
    //! report as it found it and speaks the same samples - a skipped cell where
    //! `DASLLAMA_GPU_PROF=1` keeps the ledger on.
    //! The pin and the knobs are put back however a cell leaves.
    var pinned = ProfiledLeg()
    let pinned_cell = "{name}: the decode seat's profile ledger times each role it runs"
    let plain_cell = "{name}: the profile pin off, a synthesis reports no ledger"
    unsafe {
        t |> run(pinned_cell) @capture(& m, & pinned)(t : T ?) {
            var served = 0
            let was = set_rail_prof_pin(true)
            with_put_back($ {
                set_rail_prof_pin(was)
            }) {
                with_gpu_knobs() {
                    set_serving_gpu_tower(true, TowerFamily.tts)
                    let served0 = styletts2_gpu_stats("decode").served
                    pinned.samples = styletts2_spoken(m.model, 24).samples
                    served = styletts2_gpu_stats("decode").served - served0
                    pinned.sums = rail_prof_last()
                }
            }
            let s & = unsafe(pinned.sums)
            t |> equal(served, 1, "the decode seat served the profiled synthesis ({gpu_declines()})")
            t |> success(s.stamps > 0, "the decode shot's ledger holds {s.stamps} stamps, {s.tot} us")
            for (role in DECODE_PROF_ROLES) {
                let i = int(role)
                t |> success(s.n[i] > 0 && s.us[i] > 0.0lf,
                    "{role}: {s.n[i]} stamps, {s.us[i]} us on the device")
            }
            t |> equal(s.n[int(RailProfRole.tts_attn)], 0,
                "the ledger is the decode shot's: none of PL-BERT's attention rows")
        }
        t |> run(plain_cell) @capture(& m, & pinned)(t : T ?) {
            if (g_env_gpu.gpu_prof) {
                t |> skip("{name}: DASLLAMA_GPU_PROF=1 keeps the profile ledger on - no synthesis "
                    + "runs with it off")
                return
            }
            var before, after : RailProfSums
            var samples = 0l
            let was = set_rail_prof_pin(false)
            with_put_back($ {
                set_rail_prof_pin(was)
            }) {
                with_gpu_knobs() {
                    set_serving_gpu_tower(true, TowerFamily.tts)
                    before = rail_prof_last()
                    samples = styletts2_spoken(m.model, 24).samples
                    after = rail_prof_last()
                }
            }
            t |> equal(samples, pinned.samples,
                "the profiled synthesis speaks the samples the plain one does")
            t |> success(pinned.sums.stamps > 0 && after.stamps == before.stamps
                && after.tot == before.tot, "the pin off, a synthesis reports no ledger "
                + "({after.stamps} stamps, {after.tot} us kept; the pinned one's held "
                + "{pinned.sums.stamps} stamps)")
        }
    }
}

[cold_path]
def tts_cuda_take_fault_cell(t : T?; var m : TtsModel; path, text, voice : string) {
    //! On `m`, an image the CUDA home serves, the tower knob on: with the readback's copy read as
    //! failed a synthesis refuses naming the readback, and with the seam put back the image speaks
    //! again. Skips on any other home
    let name = base_name(path)
    unsafe {
        let cell = "{name}: a failed CUDA readback refuses the synthesis by name"
        t |> run(cell) @capture(& m)(t : T ?) {
            static_if (typeinfo builtin_module_exists(cuda)) {
                if (serving_tower_dev(TowerFamily.tts) != TowerDev.cuda) {
                    t |> skip("{name}: the CUDA home does not serve the TTS seats here")
                    return
                }
                return if (tts_data_missing(t, path))
                with_gpu_cell() {
                    set_serving_gpu_tower(true, TowerFamily.tts)
                    let was = set_cuda_tts_take_fault_(true)
                    var why = ""
                    with_put_back($ {
                        set_cuda_tts_take_fault_(was)
                    }) {
                        why = panic_text() $ {
                            var inscope lost <- synthesize(m, text, voice)
                        }
                    }
                    t |> success(find(why, "its readback failed on the device") >= 0,
                        "{name}: the take's error arm names the readback: '{why}'")
                    var inscope again <- synthesize(m, text, voice)
                    t |> success(logits_maxabs(again.pcm) > 0.05,
                        "{name}: the seam put back, the image speaks")
                }
            } else {
                t |> skip("{name}: no CUDA module in this build")
            }
        }
    }
}

//! A form block's witness for its after-block check that a deleted GPU image leaves its home
//! holding nothing
struct TtsDevHeld {
    held : bool   //! a synthesis on the seats left the serving home holding weights
}

[cold_path, arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-device-release")]
def tts_device_release_cell(t : T?; var m : TtsModel; path, text, voice : string; packs : bool;
                            var kept : TtsDevHeld) {
    //! On `m`, an image of the file at `path` the box's GPU home serves, the tower knob on: a
    //! synthesis leaves the home holding weights and activation rows; `tts_release_scratch` gives
    //! every activation byte back and keeps the weights, and the next synthesis grows the rows
    //! again and speaks the same samples. `packs`: the voice reads the front end's packs, a skip
    //! without them
    let name = base_name(path)
    unsafe {
        let cell = "{name}: the idle release gives the GPU home's rows back and keeps its weights"
        t |> run(cell) @capture(& m, & kept)(t : T ?) {
            return if (packs && tts_data_missing(t, path))
            var h : tuple<weights : int64; activations : int64>[3]
            var samples = 0
            var differing = 0
            with_gpu_cell() {
                set_serving_gpu_tower(true, TowerFamily.tts)
                let dev = serving_tower_dev(TowerFamily.tts)
                var inscope a <- synthesize(m, text, voice)
                h[0] = tts_home_held(dev)
                tts_release_scratch(m)
                h[1] = tts_home_held(dev)
                var inscope b <- synthesize(m, text, voice)
                h[2] = tts_home_held(dev)
                samples = length(a.pcm)
                differing = length(a.pcm) == length(b.pcm) ? 0 : 1
                for (x, y in a.pcm, b.pcm) {
                    differing += x == y ? 0 : 1
                }
            }
            kept.held = h[0].weights > 0l
            t |> success(h[0].weights > 0l && h[0].activations > 0l,
                "a synthesis on the seats leaves the home holding weights and rows "
                + "({h[0]}; {gpu_declines()})")
            t |> equal(h[1].activations, 0l, "the release gives every activation byte back")
            t |> equal(h[1].weights, h[0].weights, "and keeps the weights")
            t |> success(h[2].activations > 0l && h[2].weights == h[0].weights,
                "the next synthesis grows the rows again on the weights it kept ({h[2]})")
            t |> equal(differing, 0, "{differing} of {samples} samples move across the release")
        }
    }
}

//! after a form block whose `tts_device_release_cell` saw the home hold weights: every form's model
//! deleted, the home holds no device byte for the driver
def tts_device_dropped_held(t : T?; name : string; kept : TtsDevHeld) {
    return if (!kept.held)
    let h = tts_home_held(serving_tower_dev(TowerFamily.tts))
    t |> success(h.weights == 0l && h.activations == 0l,
        "{name}: every GPU image deleted, the home holds no device byte for the TTS driver ({h})")
}

//! One synthesis's speech read: every sample finite (a NaN compares false, and no speech sample reaches ten), the peak and the rms.
def speech_level(pcm : array<float>) : tuple<finite : bool; peak : float; rms : float> {
    var finite = true
    for (v in pcm) {
        finite = finite && abs(v) < 10.0
    }
    return (finite = finite, peak = logits_maxabs(pcm), rms = rms(pcm))
}

//! a facade cell on `m`, the image form of the voice at `path`: the front-end packs gated under `gate_on_packs`, caps at 24 kHz with `n_voices` voices, then `body`
def facade_smoke_cell(t : T?; var m : TtsModel; path : string; gate_on_packs : bool; n_voices : int; body : block<(var m : TtsModel; c : TtsCaps) : void>) {
    return if (gate_on_packs && tts_data_missing(t, path))
    var inscope c <- caps(m)
    t |> equal(c.sample_rate, 24000)
    t |> equal(length(c.voices), n_voices, "the voices the facade lists")
    invoke(body, m, c)
}

def tts_parity(t : T?; rig : TtsParityRig; m : StyleTts2Model; var cases : array<ParityCase>) {
    //! One family's rig on `m`, the f32 form of its model: skips loudly when its dumps are not
    //! stocked. A bring-up case (the manifest's `stages`) is synthesized whole and its traced
    //! stages held; any other case stops after its durations stage (`styletts2_frames`), its sample
    //! count the one its frames predict (`styletts2_samples_of`) - the count each bring-up case
    //! holds against its own synthesis. Where a device serves the `@dev` forms after it, each
    //! bring-up case's CPU stages are left in `cases` for the legs of the tower's seats on them
    //! (`tts_parity_legs`); elsewhere no cell reads them and they do not run.
    if (rig.prepare != null) {
        invoke(rig.prepare, t, m)
    }
    var inscope mf <- rig_manifest(t, rig)
    return if (empty(mf.cases))
    let seats_read = empty(tts_dev_why())
    with_reference_f32_tile(t) {   // the bars are measured at the tile's reference order, whatever this box crowned
        t |> equal(m.family, rig.family)
        t |> equal(m.sample_rate, 24000)
        var ids_ok = 0
        var same_length = 0
        var worst = 0.0
        var worst_case = ""
        var staged = 0
        for (c in mf.cases) {
            let tag = "{rig.family} {c.id} {c.voice}"
            var inscope dump <- load_tts_dump(path_join(rig.oracle_dir, c.file))
            var inscope ids <- dump_tensor(dump, type<int>, "input_ids")
            var inscope ours <- invoke(rig.ids, c.ps_espeak)
            if (sequence_equal(ours, ids)) {
                ids_ok++
            } else {
                to_log(LOG_INFO, "{tag}: token ids differ - ours {ours}\n   oracle {ids}\n")
            }
            var inscope noise <- case_noise(dump)
            var inscope style <- dump_tensor(dump, type<float>, "style")
            var tr = new StyleTts2Trace()
            var tm = TtsTimings()
            var inscope sc = St2Scratch()
            var samples = 0l
            if (c.stages) {
                samples = styletts2_synthesize(m, ids, style, c.speed, noise, 0ul, tr, tm, sc)
                t |> equal(styletts2_samples_of(m.decoder, tr.frames), samples,
                    "{tag}: the samples its {tr.frames} frames predict are those synthesized")
            } else {
                let frames = styletts2_frames(m, ids, style, c.speed, tr, tm, sc)
                samples = styletts2_samples_of(m.decoder, frames)
            }
            dump |> get("waveform") $(ref) {
                if (samples != long_length(ref.f)) {
                    t |> failure("{tag}: {samples} samples, oracle has {length(ref.f)} "
                        + "(frames {tr.frames} vs {c.duration_sum})")
                } else {
                    same_length++
                    if (c.stages) {
                        let d = max_abs_diff(sc.wave, ref.f)
                        to_log(LOG_INFO, "{tag}: waveform maxdiff {d} over {length(ref.f)} samples "
                            + "(informational); {timings_line(tm, m.sample_rate)}\n")
                        staged++
                        if (d > worst) {
                            worst = d
                            worst_case = "{c.id} {c.voice}"
                        }
                    }
                }
            }
            if (c.stages) {
                seam_check(t, m, tag, dump, cases, seats_read)
                stage_checks(t, tag, tr, dump)
            }
            unsafe {
                delete tr
            }
        }
        t |> equal(ids_ok, length(mf.cases), "cases whose token ids match the reference driver")
        t |> equal(same_length, length(mf.cases), "cases whose durations, hence sample counts, match the oracle")
        to_log(LOG_INFO, "{rig.family}: worst end-to-end waveform maxdiff {worst} at {worst_case} over "
            + "the {staged} of {length(mf.cases)} cases synthesized whole (informational)\n")
    }
}

//! What a StyleTTS2 voice's block carries from form to form: the bring-up cases' rows of the parity rail's seats, and the
//! served synthesis's lanes
struct St2LaneRows {
    cases : array<ParityCase>
    lanes : array<SynthLane>
}

//! a lane form's leg as its cells name it: the lane, its pinned groups, and the chain the leg runs on
def private st2_leg_name(form : string) : string {
    let f = tts_form_of(form)
    return "the {st2_lane_name(f.lane)} lane{st2_pinned(f)}, {f.home == "dev" ? "the tower's leg" : "the CPU chain's leg"}"
}

def styletts2_synthesis_cell(t : T?; var m : TtsModel; form, path, cell, text, voice : string;
                             var rows : St2LaneRows; f32_lane : bool = true) {
    //! A lane form's cell of the served synthesis across the tower knob (a minted form of the voice
    //! at `path`, `TtsForm`), named `cell` and its leg: `tts_gpu_synthesis_lane`, the leg kept in
    //! `rows`; then, on a quantized lane's `@dev` form, the cell of the formats the form stores
    //! (`st2_stored_formats_held`), skipped without the front-end packs as the leg is. `f32_lane`
    //! says the block carries the f32 lane's forms, the stored formats' cell's control; a published
    //! stored file's block carries none.
    let f = tts_form_of(form)
    let formats_cell = ("{cell}, {st2_leg_name(form)}: each stored format's own stamps launch"
        + (f32_lane ? ", the f32 tiles at fewer sites than on the f32 lane's image" : "")
        + ", the dispatches over widened rows one a chunk an off-lattice noise conv")
    unsafe {
        t |> run("{cell}, {st2_leg_name(form)}") @capture(& m, & rows)(t : T ?) {
            tts_gpu_synthesis_lane(t, m, path, form, text, voice, rows.lanes)
        }
        if (f.home == "dev" && f.lane != "f32") {
            t |> run(formats_cell) @capture(& rows)(t : T ?) {
                return if (tts_data_missing(t, path))
                st2_stored_formats_held(t, rows.lanes, base_name(path), form, f32_lane)
            }
        }
    }
}

def styletts2_synthesis_held(t : T?; cell, file, voice : string; var rows : St2LaneRows) {
    //! The served synthesis's lanes held, after the voice's block, as the cell `cell`: each lane's
    //! tower leg against its CPU leg (`st2_synth_lanes_held`). No cell where no `@dev` form kept a
    //! leg - each such form is a skipped cell already.
    var kept = false
    for (k in rows.lanes) {
        kept ||= k.dev_ran
    }
    return if (!kept)
    unsafe {
        t |> run("{cell}: each lane's tower leg against its CPU leg") @capture(& rows)(t : T ?) {
            st2_synth_lanes_held(t, rows.lanes, file, voice)
        }
    }
}

def styletts2_parity_cell(t : T?; m : TtsModel; form : string; rig : TtsParityRig; cell : string; var rows : St2LaneRows) {
    //! A lane form's cell of the parity rail, named `cell`: the oracle's cases on the f32 form
    //! (`tts_parity`), which keeps the bring-up cases' rows in `rows`; on the q8, the kq and the
    //! `@dev` forms the lane's legs of the tower's seats over those rows (`tts_parity_legs`). An
    //! image form and a pinned form carry no cell of the rail.
    let f = tts_form_of(form)
    return if (form == "image" || form == "image_cpu" || !empty(f.pins))
    unsafe {
        if (form == "f32") {
            t |> run(cell) @capture(& m, & rows)(t : T ?) {
                tts_parity(t, rig, m.model, rows.cases)
            }
        } else {
            let seats = f.home != "dev" ? "decode on the CPU chain" : (f.lane == "f32" ? "stages on the tower" : "decode on the tower")
            t |> run("{cell}: the {st2_lane_name(f.lane)} lane's {seats}") @capture(& m, & rows)(t : T ?) {
                tts_parity_legs(t, rig, m.model, form, rows.cases)
            }
        }
    }
}

def styletts2_parity_held(t : T?; cell : string; var rows : St2LaneRows) {
    //! The parity rail's seats held, after the voice's block, as the cell `cell`: each seat's tower
    //! legs against its CPU leg (`parity_seats_held`). No cell where no `@dev` form kept a leg -
    //! each such form is a skipped cell already.
    var kept = false
    for (c in rows.cases) {
        for (s in c.seats) {
            kept ||= s.ran
        }
    }
    return if (!kept)
    unsafe {
        t |> run("{cell}: each seat on the tower against the CPU stage") @capture(& rows)(t : T ?) {
            with_reference_f32_tile(t) {   // the bars are measured at the tile's reference order, whatever this box crowned
                parity_seats_held(t, rows.cases)
            }
        }
    }
}
