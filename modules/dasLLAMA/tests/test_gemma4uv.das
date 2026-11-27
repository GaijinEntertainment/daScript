options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_vision_embedder   // the carrier's arms: the family sniff, the geometry, the exec stamp
require dasllama/dasllama_common   // the tensor-crown pin (the rung is simdgroup-calibrated)
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require _model_once   // with_model_once: the mmproj's one block in the area
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require ?das_metal dasllama/dasllama_metal_tower   // the GPU tier-1 attempt; the CPU gate pins the knob off
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_common   // vk_drop_model_state: the drop-and-encode leg
require _tower_twin
require _vision_forms   // the towers' forms, a block's cell
require _cpu_gpu_model   // with_cpu_and_gpu_model: a chat decoder's one block in the area
require _vision_chat_cells   // the image chat turn's cells, run under the tower's image form
require _kernel_coverage_cells   // coverage_tower_gemma4uv_metal: the kernel census's canvas
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown — the crown-pin cell drops the trio's cached flags

// Tier-1 gemma4uv embedder parity vs llama-mtmd-debug -p encode (CPU, --no-mmproj-offload, dumps
// minted from the f32-widened 12B mmproj twin; the embedder has no attention, so the flash-attn
// setting does not enter): pre-resized [0,1] float fixtures skip preprocessing entirely, so a diff
// here isolates the embedder math. Oracle per-token lines live beside the models in gemma4-vision-oracle/
// (mint.sh regenerates); both the mmproj and the dumps gate this test with explicit skips. On an Apple build the
// GPU cell runs the same dumps through the Metal embedder, on a Vulkan build through the Vulkan one, each off the
// planes with the stem on the device and engage proven per fixture by the encodes counter.
// The mmproj loads once a form, in its one `with_model_once` block (`gemma4uv_form`): the image rail's forms carry the
// Metal cell, the carrier's arms (the vision carrier's cell, from test_vision_embedder.das's topic) and the CPU gate, the
// forms minted in memory the discrete-home cells. A form's cells run on one home, so the device twin's CPU chain, device
// chain and patch-zeroed control are three forms, compared after the block on the rows they kept.
// The discrete-home cells (the device twin, the tier-1 rung on the serving discrete home) seat on the serving discrete
// home: under the CUDA ask (DASLLAMA_CUDA=1 or set_gpu_home("cuda")) on the CUDA tower, their forms skipping without a
// CUDA device there (`cuda_twin_seat_why`); else on the Vulkan tower, skipping on a das_metal build (its tower hooks own
// the seats), without a Vulkan device under DASLLAMA_GPU=1 and without the tower's tile family (`vulkan_twin_seat_why`).
// The twin's control zeroes the patch weights and must miss the bar.

let private MM = "mmproj-gemma-4-12B-it-BF16.gguf"
let private MM_F32 = "mmproj-gemma-4-12B-it-F32.gguf"

// the tier-1 gate: encode a fixture, compare per-token mean/v0..v3 against the oracle dump.
// %.6f print rounding is ±5e-7 and the measured accumulation-order noise is ~2e-5 (logged per
// fixture); 2e-4 leaves 10x headroom while staying far under any structural failure
let TIER1_TOL = 2e-4

//! the file's staged read, the one every minted form's copy comes from
[unused_argument(key)]
def private gemma4uv_stage(path, key : string) : Gemma4uvStaging => <- stage_gemma4uv_embedder(path)

//! One form of the gemma4uv embedder's file as the vision carrier holds it, its block's form loader (`tower_form`; the
//! embedder has one lane, the file's planes): `image` and `image_f32` are the carrier's own load through the image rail -
//! the copy the serving tower's cells read and the copy the CPU chain's read; `f32` is minted in memory from `stg`, its
//! own copy of the file's staging, no `.dlim` baked, `@seat` the copy the serving discrete home's cells read,
//! `+patch_zeroed` with the patch weights zeroed in every plane the chain reads them from - the device twin's input poison.
def private gemma4uv_form(path, form : string; var stg : Gemma4uvStaging) : VisionEmbedder {
    let f = tower_form(form)
    return <- load_vision_embedder(path) if (f.image)
    var inscope e = VisionEmbedder(kind = VisionKind.gemma4uv)
    if (f.edit == "patch_zeroed") {
        let n = stg.e.n_embd * stg.e.patch_dim
        for (i in range64(long_length(stg.blob) >= stg.e.patch_w + n ? n : 0l)) {
            stg.blob[stg.e.patch_w + i] = 0.0
        }
        for (i in range64(long_length(stg.wblob) >= stg.e.patch_w + n ? n : 0l)) {
            stg.wblob[stg.e.patch_w + i] = uint16(0)
        }
    } elif (!empty(f.edit)) {
        panic("the gemma4uv tests: '{form}' names no edit of the staged planes")
    }
    mint_gemma4uv_embedder(stg, e.gemma4uv)
    return <- e
}

//! The form is the family it names: the carrier holds the gemma4uv
//! embedder on the planes of `file`. Nothing on a loaded embedder
//! says which home's cells read it, nor that its patch weights are
//! zeroed.
def private gemma4uv_form_is(t : T?; file : string; e : VisionEmbedder; form : string) {
    t |> success(e.kind == VisionKind.gemma4uv && vision_exec_fmt(e) == "f32",
        "{file}: the {form} form is the gemma4uv embedder on the file's planes")
}

[cold_path]
def private gate_encode(t : T?; emb : Gemma4uvEmbedder; var st : Gemma4uvState;
                        kind : string; w, h : int; logname : string; gpu_ladder : bool = false) : bool {
    return oracle_gate(t, path_join(models_dir(), "gemma4-vision-oracle"), logname, "gemma4-vision-oracle/mint.sh", kind, w, h,
            "gemma4uv tier-1{gpu_ladder ? " GPU" : ""} {kind} {w}x{h}", int(emb.proj_dim), TIER1_TOL, gpu_ladder ? 4e-3 : 0.0) $(planes : array<float>; var out : array<float>) : int64 {
        return gemma4uv_encode(emb, st, planes, int64(w), int64(h), out)
    }
}

// The CPU gate, on the `image_f32` form.
[cold_path]
def private test_gemma4uv_tier1(tt : T?; emb : Gemma4uvEmbedder) {
    with_gpu_knobs() {
        var st = Gemma4uvState()
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain; the GPU rung is the `image` form's
        }
        tt |> equal(int(emb.patch), 48, "folded superpatch")
        tt |> equal(int(emb.patch_dim), 6912, "im2col row")
        tt |> equal(int(emb.n_embd), 3840, "embedding width")
        tt |> equal(int(emb.proj_dim), 3840, "soft-token width")
        tt |> equal(int(emb.pos_entries), 1120, "pos-table rows per axis")
        tt |> gate_encode(emb, st, "gray", 480, 480, "encode.gray480.log")
        tt |> gate_encode(emb, st, "cb", 336, 336, "encode.cb336.log")
        tt |> gate_encode(emb, st, "red", 336, 336, "encode.red336.log")
        tt |> gate_encode(emb, st, "green", 336, 336, "encode.green336.log")
        tt |> gate_encode(emb, st, "blue", 336, 336, "encode.blue336.log")
        // the non-square orientation / transposed-grid gate: identical cb patches make the
        // per-token values a pure fingerprint of pos_x[col] + pos_y[row]
        tt |> gate_encode(emb, st, "cb", 672, 336, "encode.cb672x336.log")
        // a grid wider than the pos table panics by name instead of reading past the plane
        var tripped = false
        try {
            let w = (emb.pos_entries + 1l) * emb.patch
            var planes : array<float>
            planes |> reserve(w * emb.patch * 3l)
            planes |> resize(w * emb.patch * 3l)
            var wide : array<float>
            gemma4uv_encode(emb, st, planes, w, emb.patch, wide)
            delete wide
            delete planes
        } recover {
            tripped = true
        }
        tt |> success(tripped, "a canvas past pos_entries columns panics at the grid check")
        delete st
    }
}

// The GPU tier-1 gate, on the `image` form: the same oracle dumps through the Metal tower.
// The f16-staged GEMMs cannot promise the CPU 2e-4, so the bar is the scale-relative rung
// (TIER1_TOL + 4e-3 * token rms) with the CPU constant untouched; the log states which rung
// each fixture actually held. Engage evidence is the encodes counter, per fixture.
[cold_path]
def private test_gemma4uv_tier1_gpu(tt : T?; emb : Gemma4uvEmbedder) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        // the scale-relative rung is calibrated on the SIMDGROUP kernel set - pin it
        // (a bare setter would not survive a load, which re-applies the box
        // profile's crowns): the envelope-verified mulmm_bf16 tensor crown rounds
        // differently and sits a hair past the rung on some canvases; the crowned
        // lane's quality is the board pregate's claim, not this cell's
        let prev_crowns = get_metal_tensor_crowns()
        let knobs = gpu_knobs()
        pin_metal_tensor_crowns("")
        metal_tower_shutdown()
        metal_prefill_shutdown()   // the crown flags the shared GEMM/trio builders cache
        var st = Gemma4uvState()
        set_metal_tower(true)
        let fixtures = [(kind = "gray", w = 480, h = 480, log = "encode.gray480.log"),
                        (kind = "cb", w = 336, h = 336, log = "encode.cb336.log"),
                        (kind = "red", w = 336, h = 336, log = "encode.red336.log"),
                        (kind = "cb", w = 672, h = 336, log = "encode.cb672x336.log")]
        for (fx in fixtures) {
            let s0 = metal_tower_stats()
            let ran = gate_encode(tt, emb, st, fx.kind, fx.w, fx.h, fx.log, true)
            if (ran) {
                tt |> equal(metal_tower_stats().encodes - s0.encodes, 1l,
                            "{fx.kind} {fx.w}x{fx.h}: GPU-served (encodes delta)")
            }
        }
        unpin_metal_tensor_crowns()
        set_metal_tensor_crowns(prev_crowns)
        set_gpu_knobs(knobs)
        metal_tower_shutdown()
        metal_prefill_shutdown()
        delete st
    }
}

// The carrier's gemma4uv arms, on the `image` form: what the 12B pair distinguishes.
def private test_vision_embedder_gemma4uv_arms(t : T?; e : VisionEmbedder) {
    t |> equal(vision_family(e), "gemma4uv")
    t |> equal(vision_align(e), 48l, "48 px superpatch")
    t |> equal(vision_proj_dim(e), 3840l, "the 12B width")
    t |> equal(vision_exec_fmt(e), "f32", "the embedder serves the file's planes")
}

//! one canvas of the device twin as its three forms keep it: the CPU chain's rows, the device chain's and the patch-zeroed
//! embedder's, the token counts, and the driver's counters around each leg
struct private UvTwinKept {
    w : int
    h : int
    cpu : array<float>
    dev : array<float>
    poisoned : array<float>
    n_cpu : int64
    n_dev : int64
    off0 : GpuTowerCounters
    off1 : GpuTowerCounters
    dev0 : GpuTowerCounters
    dev1 : GpuTowerCounters
    poison0 : GpuTowerCounters
    poison1 : GpuTowerCounters
}

//! the device twin's canvases and the form whose leg runs
struct private UvTwin {
    form : string
    canvases : array<UvTwinKept>
}

// A form's leg of the device twin on each canvas: the CPU chain on `f32` with the tower knob off, the device chain on
// `f32@seat` and the patch-zeroed control on `f32@seat+patch_zeroed` with it on.
[cold_path]
def private device_twin_leg(emb : Gemma4uvEmbedder; var twin : UvTwin) {
    let fam = TowerFamily.vision
    var st = Gemma4uvState()
    for (k in twin.canvases) {
        var inscope planes <- make_planes("cb", k.w, k.h)
        with_gpu_knobs() {
            set_serving_gpu_tower(twin.form != "f32", fam)
            let s0 = gpu_tower_counters(fam)
            var inscope rows : array<float>
            var n = 0l
            with_dasllama_jobque_() {
                n = gemma4uv_encode(emb, st, planes, int64(k.w), int64(k.h), rows)
            }
            let s1 = gpu_tower_counters(fam)
            if (twin.form == "f32") {
                k.cpu <- rows
                k.n_cpu = n
                k.off0 = s0
                k.off1 = s1
            } elif (twin.form == "f32@seat") {
                k.dev <- rows
                k.n_dev = n
                k.dev0 = s0
                k.dev1 = s1
            } else {
                k.poisoned <- rows
                k.poison0 = s0
                k.poison1 = s1
            }
        }
    }
    delete st
}

// The device twin's compares, after the block: on each canvas the knob-off leg never dispatched, the device chain served
// the encode with the stem on the device and holds the twin bar against the CPU chain, and the patch-zeroed control misses it.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private test_gemma4uv_device_twin(tt : T?; twin : UvTwin) {
    let home = serving_tower_dev(TowerFamily.vision)
    for (k in twin.canvases) {
        let tag = "gemma4uv {home} cb {k.w}x{k.h}"
        let kept = !empty(k.cpu) && !empty(k.dev) && !empty(k.poisoned)
        tt |> success(kept, "{tag}: the three forms kept their rows")
        continue if (!kept)
        tt |> equal(k.off1.encodes - k.off0.encodes, 0l, "{tag}: the knob-off leg never dispatched (encodes delta)")
        tt |> equal(k.off1.convs - k.off0.convs, 0l, "{tag}: the knob-off leg never ran the stem on the device (convs delta)")
        tt |> equal(k.n_cpu, k.n_dev, "{tag}: token count on both chains")
        tt |> equal(k.dev1.encodes - k.dev0.encodes, 1l, "{tag}: device-served (encodes delta)")
        tt |> equal(k.dev1.convs - k.dev0.convs, 1l, "{tag}: the stem ran on the device (convs delta)")
        tt |> equal(k.poison1.encodes - k.poison0.encodes, 1l, "{tag} patch-zeroed: device-served (encodes delta)")
        tower_twin_held(tt, tag, TowerFamily.vision, k.cpu, k.dev, TWIN_EXACT_REL_L2_VK)
        tower_twin_poisoned(tt, "{tag} patch-zeroed", TowerFamily.vision, k.cpu, k.poisoned, TWIN_EXACT_REL_L2_VK)
    }
}

// The embedder whole on the serving device off the planes, on the `f32@seat` form: it holds the CPU bar on the mtmd dumps;
// engage evidence: the encodes counter, per fixture; and it serves again after the driver's model state is dropped.
[cold_path]
def private test_gemma4uv_tier1_vulkan(tt : T?; emb : Gemma4uvEmbedder) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var st = Gemma4uvState()
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.vision)
            let fixtures = [(kind = "gray", w = 480, h = 480, log = "encode.gray480.log"),
                            (kind = "cb", w = 336, h = 336, log = "encode.cb336.log"),
                            (kind = "red", w = 336, h = 336, log = "encode.red336.log"),
                            (kind = "cb", w = 672, h = 336, log = "encode.cb672x336.log")]
            for (fx in fixtures) {
                let s0 = gpu_tower_counters(TowerFamily.vision)
                let ran = gate_encode(tt, emb, st, fx.kind, fx.w, fx.h, fx.log, false)
                if (ran) {
                    tt |> equal(gpu_tower_counters(TowerFamily.vision).encodes - s0.encodes, 1l, "{fx.kind} {fx.w}x{fx.h}: device-served (encodes delta)")
                    tt |> equal(gpu_tower_counters(TowerFamily.vision).convs - s0.convs, 1l, "{fx.kind} {fx.w}x{fx.h}: the stem ran on the device (convs delta)")
                }
            }
            vk_drop_model_state()   //! the drop forgets the vision marks: the next encode regrows its buffers and uploads the stem off a fresh key
            let sd = gpu_tower_counters(TowerFamily.vision)
            if (gate_encode(tt, emb, st, "cb", 336, 336, "encode.cb336.log", false)) {
                tt |> equal(gpu_tower_counters(TowerFamily.vision).encodes - sd.encodes, 1l, "after the model drop: device-served again (encodes delta)")
            }
        }
        delete st
    } else {
        tt |> skip("dasVulkan not present")
    }
}

// The tier-1 rung's control, on the `f32@seat+patch_zeroed` form: the patch-zeroed embedder on the device reds the dump's bar.
[cold_path]
def private tier1_vulkan_poison(tt : T?; embp : Gemma4uvEmbedder) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var o = OracleEncode()
        if (oracle_dump(tt, "gemma4-vision-oracle", "encode.cb336.log", "gemma4-vision-oracle/mint.sh", o)) {
            var stp = Gemma4uvState()
            dump_poison_leg(tt, "gemma4uv", "cb", 336, 336, o, 0.0, 0l, int(embp.proj_dim), TIER1_TOL, embp, stp) $(t; var s; planes; var out) => gemma4uv_encode(t, s, planes, 336l, 336l, out)
            delete stp
        }
    } else {
        tt |> skip("dasVulkan not present")
    }
}

// The Metal tower against the CPU chain on synthetic canvases, on the
// `image` form: engage evidence is counter deltas, and the knob-off
// leg runs beside the device leg on the one load, so the knob is
// proven both ways.
[cold_path, unused_argument(tt, emb)]
def private test_gemma4uv_metal_tower(tt : T?; emb : Gemma4uvEmbedder) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(tt, "gemma4v") || !arm_on(tt, "gemma4uv-metal") || !jit_only(tt)) return
        with_gpu_knobs() {
            var st = Gemma4uvState()
            // 672x336 is the non-square/orientation gate, 384x384 the
            // no-pad shape (npos % 32 == 0)
            let shapes = [(w = 480l, h = 480l), (w = 336l, h = 336l), (w = 672l, h = 336l),
                          (w = 384l, h = 384l), (w = 96l, h = 96l)]
            for (sh in shapes) {
                let tag = "gemma4uv tower {sh.w}x{sh.h}"
                var planes : array<float>
                planes |> resize(sh.w * sh.h * 3l)
                for (i in range64(sh.w * sh.h * 3l)) {
                    planes[i] = sin(float(i % 977l) * 0.013) * 0.7
                }
                let s0 = metal_tower_stats()
                set_metal_tower(false)
                var cpu_out : array<float>
                let np_cpu = gemma4uv_encode(emb, st, planes, sh.w, sh.h, cpu_out)
                set_metal_tower(true)
                let s1 = metal_tower_stats()
                tt |> equal(s1.encodes, s0.encodes, "{tag}: OFF leg never dispatches")
                tt |> equal(s1.declines - s0.declines, 1l,
                    "{tag}: OFF leg records the knob decline")
                var gpu_out : array<float>
                let np_gpu = gemma4uv_encode(emb, st, planes, sh.w, sh.h, gpu_out)
                let s2 = metal_tower_stats()
                let np_geo = (sh.w / emb.patch) * (sh.h / emb.patch)
                tt |> equal(np_cpu, np_geo, "{tag}: npos from geometry")
                tt |> equal(np_gpu, np_cpu, "{tag}: both legs agree on npos")
                tt |> equal(s2.encodes - s1.encodes, 1l, "{tag}: GPU leg served (encodes delta)")
                tt |> equal(s2.rows - s1.rows, np_gpu, "{tag}: rows delta is npos")
                tt |> equal(s2.declines, s1.declines, "{tag}: GPU leg declined nothing")
                tower_parity(tt, tag, cpu_out, gpu_out)
                delete gpu_out
                delete cpu_out
                delete planes
            }
            delete st
        }
    }
}

// The all-F32 mmproj on the Metal tower, on its `image` form: both
// GEMMs ride the f32 plane, the projection arm the BF16 file's mixed
// layout never exercises.
[cold_path, unused_argument(tt, emb32)]
def private test_gemma4uv_f32_metal_tower(tt : T?; emb32 : Gemma4uvEmbedder) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(tt, "gemma4v") || !arm_on(tt, "gemma4uv-metal") || !jit_only(tt)) return
        tt |> success(!emb32.patch_bf16 && !emb32.proj_bf16,
            "the F32 file keeps both GEMMs on the f32 plane")
        with_gpu_knobs() {
            var st32 = Gemma4uvState()
            var planes32 : array<float>
            planes32 |> resize(96l * 96l * 3l)
            for (i in range64(96l * 96l * 3l)) {
                planes32[i] = sin(float(i) * 0.013) * 0.7
            }
            set_metal_tower(false)
            var cpu32 : array<float>
            gemma4uv_encode(emb32, st32, planes32, 96l, 96l, cpu32)
            set_metal_tower(true)
            let sf0 = metal_tower_stats()
            var gpu32 : array<float>
            gemma4uv_encode(emb32, st32, planes32, 96l, 96l, gpu32)
            tt |> equal(metal_tower_stats().encodes - sf0.encodes, 1l, "f32-mmproj leg GPU-served")
            tower_parity(tt, "gemma4uv tower f32-mmproj 96x96", cpu32, gpu32)
            delete gpu32
            delete cpu32
            delete planes32
            delete st32
        }
    }
}

// The gemma-4 12B decoder's block, nested in the tower's `image` form: an image chat turn reads both files.
[cold_path]
def private gemma4_12b_decoder(t : T?; var emb : VisionEmbedder) {
    unsafe {
        t |> run("the gemma-4 12B decoder under the gemma4uv embedder") @capture(& emb)(t : T ?) {
            return if (!vision_chat_gate(t, "gemma4", "gemma-4-12B-it-Q4_K_M.gguf", 80))
            with_cpu_and_gpu_model(t, "vision", "gemma-4-12B-it-Q4_K_M.gguf", VISION_CHAT_CTX, [cpu_cells_form(QuantMode.fp32)]) $(var m; form) {
                vision_chat_cell(t, "the image turn: stream shape, then the caption", m, form, false, emb, @@test_vision_chat)
            }
        }
    }
}

// The 12B mmproj, one block.
[test(description = "The gemma-4 12B mmproj loads once a form. The gemma4uv embedder on the CPU reads the mmproj geometry and matches the mtmd encode dumps per token within 2e-4 on square fixtures and a non-square 672 x 336 position fingerprint, and panics on a grid wider than the position table. On the Metal tower with the SIMDGROUP kernel set pinned it matches the dumps within 2e-4 plus 4e-3 of token rms on four fixtures, each GPU-served by one encode. Through the vision carrier the mmproj sniffs as gemma4uv with a 48 px superpatch alignment, a 3840-wide projection and the f32 exec stamp. On the serving discrete home the embedder, stem included, holds the twin bar against the CPU chain on square and non-square canvases with matching token counts, while a patch-zeroed embedder misses it; whole on that home's device it holds the CPU 2e-4 bar on the mtmd encode dumps with the stem on device, a patch-zeroed control reds, and it serves again after a model-state drop. Under the mmproj's image form the gemma-4 12B decoder loads once: the image turn on the cats photo splices 130 rows between the image markers with the user text after them, refuses queued audio beside the image, and greedily captions the cats in over 40 characters, also with the Metal tower off. On the Metal tower the embedder matches the CPU chain on five synthetic canvas shapes within tower parity (rel_l2 and norm_dev at most 4e-3, cos at least 1-1e-5), with npos and encode counters from the geometry and a counted knob decline")]
def test_gemma4uv_embedder(t : T?) {
    return if (!family_on(t, "gemma4v"))
    var inscope twin = UvTwin()
    twin.canvases <- [UvTwinKept(w = 336, h = 336), UvTwinKept(w = 672, h = 336)]
    var inscope forms <- ["image", "image_f32"]
    add_served_forms(t, forms, MM, ["f32", "f32@seat", "f32@seat+patch_zeroed"], @@either_home_twin_form_why)
    var twin_ran = false
    with_model_once(t, "vision", "mmproj-gemma-4-12B-it-BF16.gguf", forms, @@tower_stage_key, @@gemma4uv_stage, @@gemma4uv_form) $(var e; form) {
        gemma4uv_form_is(t, MM, e, form)
        twin.form = form
        if (form == "image") {
            tower_cell(t, "tier-1 GPU attempt vs the mtmd encode dumps (scale-relative rung)", e.gemma4uv, @@test_gemma4uv_tier1_gpu)
            tower_cell(t, "the 12B mmproj sniffs as gemma4uv and reports its geometry", e, @@test_vision_embedder_gemma4uv_arms)
            tower_cell(t,
                "gemma4uv: the Metal tower serves the embedder - parity + counters + knob",
                e.gemma4uv, @@test_gemma4uv_metal_tower)
            if (empty(metal_build_why())) {
                coverage_tower_gemma4uv_metal(t,
                    "gemma4uv: the kernel census's small canvas on the Metal tower",
                    e.gemma4uv)
            }
            gemma4_12b_decoder(t, e)
        } elif (form == "image_f32") {
            tower_cell(t, "tier-1 embedder parity vs the mtmd encode dumps", e.gemma4uv, @@test_gemma4uv_tier1)
        } else {
            twin_ran = true
            device_twin_leg(e.gemma4uv, twin)
            if (form == "f32@seat") {
                tower_cell(t, "tier-1 encode on the serving discrete home's tower ({serving_tower_dev(TowerFamily.vision)}) vs the mtmd encode dumps", e.gemma4uv, @@test_gemma4uv_tier1_vulkan)
            } elif (form == "f32@seat+patch_zeroed") {
                tower_cell(t, "tier-1 encode on the serving discrete home's tower: the patch-zeroed control", e.gemma4uv, @@tier1_vulkan_poison)
            }
        }
    }
    if (twin_ran) {
        kept_cell(t, "the device embedder vs the CPU chain, dump-free, and the patch-zeroed control", twin, @@test_gemma4uv_device_twin)
        gpu_tower_shutdown(TowerFamily.vision)
    }
}

// The all-F32 12B mmproj, one block.
[test(description = "The gemma-4 12B all-F32 mmproj loads once a form: its gemma4uv embedder keeps both GEMMs on the f32 plane, and on the Metal tower it matches the CPU chain on a 96 x 96 canvas within tower parity, GPU-served by one encode")]
def test_gemma4uv_f32_embedder(t : T?) {
    return if (!family_on(t, "gemma4v"))
    with_model_once(t, "vision", "mmproj-gemma-4-12B-it-F32.gguf", ["image"],
            @@tower_stage_key, @@gemma4uv_stage, @@gemma4uv_form) $(var e; form) {
        gemma4uv_form_is(t, MM_F32, e, form)
        tower_cell(t, "gemma4uv: the all-F32 mmproj on the Metal tower - parity + counter",
            e.gemma4uv, @@test_gemma4uv_f32_metal_tower)
    }
}
