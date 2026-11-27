options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemma3v
require dasllama/dasllama_vision_embedder   // vision_exec_fmt / vision_family through the carrier
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require _model_once   // with_model_once: the tower file's one block in the area; own_load
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require ?das_metal dasllama/dasllama_metal_tower   // the GPU tier-1 attempt; the CPU gates pin the knob off
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown - the twin-W leg re-arms crowned PSOs
require ?vulkan dasllama/dasllama_vulkan_tower
require dasllama/dasllama_common   // set_metal_tensor_crowns - the twin-W leg's crown pin
require math   // nolint:STYLE030,LINT019 — used only inside the GPU static_if arms; the no-GPU lane sees it unused
require _tower_twin   // set_every_gpu_tower, the Vulkan twin's seat and instrument
require _vision_forms   // the towers' forms, the twin's kept canvases, a block's cell
require _cpu_gpu_model   // with_cpu_and_gpu_model: a chat decoder's one block in the area
require _vision_chat_cells   // the image chat turn's cells, run under the tower's image form
require _kernel_coverage_cells   // coverage_tower_gemma3v_*_metal: the kernel census's 896 canvas

// Tier-1 gemma3v (SigLIP) tower parity vs llama-mtmd-debug -p encode.
// Reference arm: the f32-widened gemma-3-4b mmproj, CPU, --no-mmproj-offload, -fa off.
// The canvas is FIXED 896² (learned position table), so the fixture axis is content, not
// geometry. Dumps: beside the models in gemma3-vision-oracle/ (mint_gemma3.sh regenerates).
// The mmproj loads once a form, in its one `with_model_once` block (`gemma3v_form`): the image rail's forms carry the CPU
// lanes' cells and the Metal tower's, the forms minted in memory the device lanes' and the discrete twin's. A form's cells
// run on one home, so a compare of the CPU chain with a device chain is two forms, made after the block on the rows they kept.
// The discrete-home cells (the q8 lane's device rung, the twin) run on the serving discrete home: under the CUDA ask
// (DASLLAMA_CUDA=1 or set_gpu_home("cuda")) on the CUDA tower, else on the Vulkan tower. The twin's forms skip under the ask
// without a CUDA device (`cuda_twin_seat_why`), else on a build with das_metal - the Metal driver owns the tower hooks there -,
// without a Vulkan device under DASLLAMA_GPU=1 and without the tower's tile family (`vulkan_twin_seat_why`); the q8 lane's
// device rung is a cell of the twin's `q8@dev` form - the serving home's image - so it goes where that form skips, and skips
// itself where the serving home's device does not stand up with the feed its chain rides.

let private MM = "mmproj-gemma-3-4b-it-F16.gguf"

let private TIER1_ABS = 2e-4
let private TIER1_REL = 4e-3   // × token rms - f32 planes vs the f32 oracle: summation order only
let private TIER1_REL_GPU = 4e-2   // the Metal leg: measured 6.9e-3..2.3e-2 x rms on these dumps - the f32 mulmm stages f16 tiles over 27 high-variance LN blocks
let private TIER1_REL_Q8 = 3.2e-1   // the q8 serving lane: measured 0.22-0.25 x rms on these dumps - 27 blocks of q8×q8 GEMMs incl. the padded down (deeper + higher-variance than gemma4v's 16); the caption floor is the product gate

let private POISON_BLOCK = 13l   //! the whole-tower input poison's block: mid-stack, so the zero rides the residual through half the chain

//! the file's staged read the key `key` names (`tower_stage_key`), under the key's lane pin and image flavor
def private gemma3v_stage(path, key : string) : Gemma3vStaging =>
    <- staged_tower_form(path, key, @@set_gemma3v_q8, @@reset_gemma3v_q8, @@stage_gemma3v_tower)

//! One form of the gemma-3 SigLIP tower's file as the vision carrier holds it, its block's form loader (`tower_form`): the
//! image rail's forms through `load_vision_embedder`, any other minted in memory from `stg`, its own copy of the file's
//! staging under the form's lane pin, cut and poisoned - never `load_gemma3v_tower`, which bakes a `.dlim` under the pin.
def private gemma3v_form(path, form : string; var stg : Gemma3vStaging) : VisionEmbedder {
    let f = tower_form(form)
    return <- image_tower_form(path, f, @@set_gemma3v_q8, @@reset_gemma3v_q8, @@gemma3v_serves_q8) if (f.image)
    var inscope e = VisionEmbedder(kind = VisionKind.gemma3v)
    truncate_and_poison(stg, f.nl, f.zero_block)
    mint_gemma3v_tower(stg, e.gemma3v, tower_form_dev(f))
    return <- e
}

[cold_path]
def private gate_encode(t : T?; tw : Gemma3vTower; var st : Gemma3vState; kind : string; logname : string; rel : float = TIER1_REL) : bool {
    let px = int(tw.image_px)
    return oracle_gate(t, path_join(models_dir(), "gemma3-vision-oracle"), logname, "gemma3-vision-oracle/mint_gemma3.sh", kind, px, px,
            "gemma3v tier-1{tw.q8 ? " q8" : ""} {kind}", int(tw.proj_dim), TIER1_ABS, rel) $(planes : array<float>; var out : array<float>) : int64 {
        return gemma3v_encode(tw, st, planes, tw.image_px, tw.image_px, out)
    }
}

//! the loader's refusals by name, each a load that must panic: a gemma4v mmproj, and a source past the 1 GiB staged-mint line
def private load_refusals(tt : T?) {
    own_load("asserts on the load itself - its refusal: another family's mmproj, and the f32 twin past the staged-mint cap")
    let wrong = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
    if (stat(wrong).is_valid) {
        var wrong_tripped = false
        try {
            var bad <- load_gemma3v_tower(wrong)
            delete bad
        } recover {
            wrong_tripped = true
        }
        tt |> success(wrong_tripped, "a gemma4v mmproj is refused by projector name")
    }
    let f32twin = path_join(models_dir(), "mmproj-gemma-3-4b-it-F32.gguf")
    if (stat(f32twin).is_valid) {   // the oracle artifact doubles as the over-cap fixture
        var cap_tripped = false
        try {
            var bad2 <- load_gemma3v_tower(f32twin)
            delete bad2
        } recover {
            cap_tripped = true
        }
        tt |> success(cap_tripped, "the 1.6 GB f32 twin trips the staged-mint cap")
    }
}

// The exact f32 lane on the CPU, on the `image_f32` form.
[cold_path]
def private test_gemma3v_tier1(tt : T?; tw : Gemma3vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var st = Gemma3vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.patch), 14, "patch")
        tt |> equal(int(tw.merge), 4, "pool kernel")
        tt |> equal(int(tw.image_px), 896, "fixed canvas")
        tt |> equal(int(tw.n_embd), 1152, "tower width")
        tt |> equal(int(tw.n_layer), 27, "blocks")
        tt |> equal(int(tw.n_head), 16, "heads")
        tt |> equal(int(tw.n_ff), 4304, "ffn width")
        tt |> equal(int(tw.proj_dim), 2560, "soft-token width")
        tt |> gate_encode(tw, st, "cb", "g3.encode.cb896.log")
        tt |> gate_encode(tw, st, "gray", "g3.encode.gray896.log")
        tt |> gate_encode(tw, st, "red", "g3.encode.red896.log")
        tt |> gate_encode(tw, st, "green", "g3.encode.green896.log")
        tt |> gate_encode(tw, st, "blue", "g3.encode.blue896.log")
        // the fixed-canvas guard panics by name instead of misindexing the position table
        var tripped = false
        try {
            var planes : array<float>
            planes |> reserve(448l * 448l * 3l)
            planes |> resize(448l * 448l * 3l)
            var out : array<float>
            gemma3v_encode(tw, st, planes, 448l, 448l, out)
            delete out
            delete planes
        } recover {
            tripped = true
        }
        tt |> success(tripped, "a non-896 canvas panics at the fixed-canvas gate")
        // refusals by name: a wrong-family mmproj, and a source past the 1 GiB staged-mint line
        load_refusals(tt)
        delete st
    }
}

// The GPU rung, on the `image` form: the same dumps through the Metal tower's block loop (the f32 mulmm stages f16
// tiles, and the 72-wide heads restride to 128 around the attention trio).
[cold_path]
def private test_gemma3v_tier1_gpu(tt : T?; tw : Gemma3vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        // every lever the legs read pinned at the head - the tower on, the flash attention on, the f16 twin route on - and
        // put back as found at the end
        let knobs_found = gpu_knobs()
        let flash_found = get_metal_tower_flash()
        let f16_found = get_metal_tower_f16()
        set_metal_tower(true)
        set_metal_tower_flash(true)
        set_metal_tower_f16(true)
        let knobs_pinned = gpu_knobs()
        reset_gemma3v_q8()
        let policy_q8 = gemma3v_serves_q8()   // metal armed: the policy default is the exact lane
        var st = Gemma3vState()
        tt |> equal(tw.q8, policy_q8, "the load took the policy's unpinned lane under the Metal tower (q8 = {policy_q8})")
        let fixtures = [(kind = "cb", log = "g3.encode.cb896.log"),
                        (kind = "green", log = "g3.encode.green896.log")]
        for (fx in fixtures) {
            let s0 = metal_tower_stats()
            if (gate_encode(tt, tw, st, fx.kind, fx.log, TIER1_REL_GPU)) {
                device_leg_served(tt, fx.kind, s0, metal_tower_stats(), tw.n_layer, -1l)
            }
        }
        // the flash lever off: the padded slab trio and its unpad (`tw_pad_attention`) serve the attention instead
        set_metal_tower_flash(false)
        let sf0 = metal_tower_stats()
        if (gate_encode(tt, tw, st, "cb", "g3.encode.cb896.log", TIER1_REL_GPU)) {
            device_leg_served(tt, "cb, flash off", sf0, metal_tower_stats(), tw.n_layer, -1l)
        }
        set_metal_tower_flash(true)
        // the knob-off leg: the same tower, the CPU chain, no dispatch
        set_metal_tower(false)
        let sc0 = metal_tower_stats()
        tt |> gate_encode(tw, st, "cb", "g3.encode.cb896.log")
        let sc1 = metal_tower_stats()
        tt |> equal(sc1.encodes, sc0.encodes, "knob-off leg never dispatched")
        tt |> equal(sc1.declines - sc0.declines, 1l, "knob-off leg recorded the knob decline")
        set_gpu_knobs(knobs_pinned)
        set_metal_tower_f16(f16_found)
        set_metal_tower_flash(flash_found)
        set_gpu_knobs(knobs_found)
        delete st
    } else {
        feint("das_metal is not built on this platform - the GPU rung is Apple-only\n")
    }
}

// The GPU rung's q8 lane, on the `q8@dev` form of a das_metal build: the tower minted q8 in the Metal flavor serves every
// block off its q8 planes.
[cold_path]
def private gpu_q8_lane(tt : T?; twq : Gemma3vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            var stq = Gemma3vState()
            tt |> success(twq.q8, "the q8 lane minted under the pin")
            q8_lane_served(tt, "q8 tower", TowerFamily.vision, twq.n_layer) $() : bool {
                return tt |> gate_encode(twq, stq, "cb", "g3.encode.cb896.log", TIER1_REL_Q8)
            }
            delete stq
            tt |> success(metal_tower_vis_slab_live(), "the q8 lane attached the vision slab")
        }
    }
}

// The fixed canvas's device-side dump bar poisoned, on the `q8_z13@dev` form: the q8 tower with `POISON_BLOCK` zeroed must
// exceed the bar the dump holds; on a das_metal build the tower's shutdown then frees the vision slab the q8 lane attached.
[cold_path]
def private g3v_poison_leg(tt : T?; twp : Gemma3vTower) {
    var o = OracleEncode()
    return if (!oracle_dump(tt, "gemma3-vision-oracle", "g3.encode.cb896.log", "gemma3-vision-oracle/mint_gemma3.sh", o))
    var stp = Gemma3vState()
    let px = int(twp.image_px)
    dump_poison_leg(tt, "gemma3v", "cb", px, px, o, TIER1_REL_Q8, POISON_BLOCK, int(twp.proj_dim), TIER1_ABS, twp, stp) $(t; var s; planes; var out) => gemma3v_encode(t, s, planes, t.image_px, t.image_px, out)
    delete stp
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_tower_shutdown()
        tt |> success(!metal_tower_vis_slab_live(), "the shutdown freed the vision slab")
    }
}

// The GPU rung's f16 twin route, on the `image_f32` form - the exact lane is the one that bakes the twin.
[cold_path]
def private gpu_twin_route(tt : T?; twt : Gemma3vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knobs_found = gpu_knobs()
        let flash_found = get_metal_tower_flash()
        let f16_found = get_metal_tower_f16()
        let crowns_found = get_metal_tensor_crowns()
        set_metal_tower(true)
        set_metal_tower_flash(true)
        set_metal_tower_f16(true)
        set_metal_tensor_crowns("mulmm_q8")
        metal_prefill_shutdown()
        var stt = Gemma3vState()
        let tw0 = metal_tower_f16_encodes()
        if (gate_encode(tt, twt, stt, "cb", "g3.encode.cb896.log", TIER1_REL_GPU)) {
            tt |> success(metal_tower_f16_encodes() - tw0 >= 1l, "the f16-sourced tower rode the twin route")
        }
        set_metal_tower_f16(f16_found)
        set_metal_tower_flash(flash_found)
        set_metal_tensor_crowns(crowns_found)
        set_gpu_knobs(knobs_found)
        metal_prefill_shutdown()
        delete stt
    }
}

// The kernel census's f16-route row, on the `f32@seat` form - the
// exact lane's image holds the halfword twin: the tower's half knob
// on and the crowns `mulmm_q8` set around the encode, the prefill
// driver shut down on each side so its half GEMMs arm under them.
[cold_path, unused_argument(tt, what, twt)]
def private twin_w_census_leg(tt : T?; what : string; var twt : Gemma3vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let f16_found = get_metal_tower_f16()
        let crowns_found = get_metal_tensor_crowns()
        set_metal_tower_f16(true)
        set_metal_tensor_crowns("mulmm_q8")
        metal_prefill_shutdown()
        coverage_tower_gemma3v_twin_w_metal(tt, what, twt)
        set_metal_tower_f16(f16_found)
        set_metal_tensor_crowns(crowns_found)
        metal_prefill_shutdown()
    }
}

//! what the device tail's two forms keep: the CPU chain's soft tokens of the cb canvas, the device chain's, and the device
//! chain's of another canvas
struct private TailKept {
    n_layer : int64
    proj_dim : int64
    n : int64
    n_dev : int64
    cpu_out : array<float>
    dev_out : array<float>
    other : array<float>
}

// The device tail's CPU leg, on the `f32` form - the exact lane's image, the CPU image - of a das_metal build.
[cold_path]
def private tail_cpu_leg(tt : T?; tw : Gemma3vTower; var k : TailKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!jit_only(tt))
        var st = Gemma3vState()
        let px = int(tw.image_px)
        var inscope cb <- make_planes("cb", px, px)
        k.n_layer = tw.n_layer
        k.proj_dim = tw.proj_dim
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_metal_tower(false)
                let e0 = metal_tower_stats().encodes
                k.n = gemma3v_encode(tw, st, cb, tw.image_px, tw.image_px, k.cpu_out)
                tt |> equal(metal_tower_stats().encodes - e0, 0l, "the tower off: the CPU chain, no dispatch")
            }
        }
        delete st
    }
}

// The device tail's device leg, on the `f32@seat` form - the same image, which the device reads as stored.
[cold_path]
def private tail_dev_leg(tt : T?; tw : Gemma3vTower; var k : TailKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!jit_only(tt))
        var st = Gemma3vState()
        let px = int(tw.image_px)
        var inscope cb <- make_planes("cb", px, px)
        var inscope green <- make_planes("green", px, px)
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_metal_tower(true)
                let e0 = metal_tower_stats().encodes
                let t0 = metal_tower_tail_encodes()
                k.n_dev = gemma3v_encode(tw, st, cb, tw.image_px, tw.image_px, k.dev_out)
                tt |> equal(metal_tower_stats().encodes - e0, 1l, "the tower on: the blocks ride the device")
                tt |> equal(metal_tower_tail_encodes() - t0, 1l, "the tower on: the tail rides the device behind the blocks")
                gemma3v_encode(tw, st, green, tw.image_px, tw.image_px, k.other)
            }
        }
        delete st
    }
}

// The Metal chain with the projector tail behind its blocks against the CPU chain on the exact lane's image, after the
// block: the soft tokens within `tower_parity_deep`'s bar, another canvas outside the bar (the control).
[cold_path]
def private test_gemma3v_tail_gpu(tt : T?; k : TailKept) {
    return if (!jit_only(tt))
    tt |> equal(k.n_dev, k.n, "the same row count")
    var inscope a <- copy_span(k.cpu_out, 0l, k.n * k.proj_dim)
    var inscope b <- copy_span(k.dev_out, 0l, k.n * k.proj_dim)
    var inscope c <- copy_span(k.other, 0l, k.n * k.proj_dim)
    tower_parity_deep(tt, "gemma3v {k.n_layer}L", a, b, k.n_layer)
    var ctl = TowerParityStats()
    if (tower_parity_stats(tt, "gemma3v control", a, c, ctl)) {
        let bar = tower_deep_bar(k.n_layer)
        tt |> success(is_finite(ctl.rel_l2) && ctl.rel_l2 > bar, "another canvas lands outside the bar (rel_l2 {ctl.rel_l2}) - the control")
    }
}

// The q8 serving lane (the CPU default), on the `image_q8` form - the lane's CPU image, on the CPU chain: the same dumps on
// its own scale-relative bar. The device rung is `q8_device_rung`'s.
[cold_path]
def private test_gemma3v_tier1_q8(tt : T?; via_carrier : VisionEmbedder) {
    let tw & = unsafe(via_carrier.gemma3v)
    tt |> equal(vision_family(via_carrier), "gemma3v", "the carrier sniffs the family")
    tt |> equal(vision_exec_fmt(via_carrier), "q8", "the carrier stamps the q8 lane")
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var st = Gemma3vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> gate_encode(tw, st, "cb", "g3.encode.cb896.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "green", "g3.encode.green896.log", TIER1_REL_Q8)
        delete st
    }
}

// The q8 serving lane's device rung, on the `q8@dev` form where the discrete twin's seat stands: the same two dumps on the
// q8 bar through the serving discrete home's block loop, over the tower minted in that home's image flavor - a driver
// serves its own image, and declines the CPU image the `image_q8` form holds.
[cold_path]
def private q8_device_rung(tt : T?; tw : Gemma3vTower) {
    if (!serving_tower_device_up(TowerFamily.vision)) {
        tt |> skip("no device stands up for the serving discrete home's tower ({serving_tower_dev(TowerFamily.vision)}) - the q8 lane's device rung")
        return
    }
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.vision)
        var st = Gemma3vState()
        for (fx in [(kind = "cb", log = "g3.encode.cb896.log"), (kind = "green", log = "g3.encode.green896.log")]) {
            let v0 = gpu_tower_counters(TowerFamily.vision)
            let ran = gate_encode(tt, tw, st, fx.kind, fx.log, TIER1_REL_Q8)
            if (ran) {
                device_leg_served(tt, fx.kind, v0, gpu_tower_counters(TowerFamily.vision), tw.n_layer, -1l)
            }
        }
        delete st
    }
}

//! the discrete twin's canvases, depths and exact-lane leg: the fixed canvas on one- and two-block towers and the whole
//! tower, the poison on the one-block tower, where the bar is tightest
def private gemma3v_twin() : VisionTwin {
    var twin <- vision_twin("cb", 896, 896, "gemma3v cb", 64l * 64l)
    twin.canvases <- [twin_canvas(1l, "cb", 896, 896, 0l), twin_canvas(2l, "cb", 896, 896), twin_canvas(0l, "cb", 896, 896)]
    twin.depths <- [TwinDepth(nl = 1l), TwinDepth(nl = 2l)]
    return <- twin
}

//! the forms only the discrete twin reads; with the whole exact forms and the q8 one on the device (`f32`, `f32@seat`,
//! `q8@dev`) they are its legs, the forms of one staged read (`tower_stage_key`) side by side
let private TWIN_FORMS = fixed_array("f32_1L", "f32_2L", "f32_z13@seat", "q8_1L", "q8_2L", "q8", "q8_1L@dev", "q8_1L_z0@dev", "q8_2L@dev")

// A form's legs of the discrete twin: the fixed canvas three ways, a form a chain, and the exact lane served on the device.
[cold_path]
def private twin_legs(tt : T?; tw : Gemma3vTower; var twin : VisionTwin) {
    if (twin.form == "f32") {
        tt |> success(!tw.q8, "exact-plane lane minted")
        tt |> equal(tw.image_px / tw.patch, 64l, "the exact lane's canvas is the 64 x 64 patch grid")
    } elif (twin.form == "q8") {
        tt |> success(tw.q8, "q8 lane minted")
    }
    var st = Gemma3vState()
    twin_form_legs(tt, twin, tw, st) $(t; var s; planes; w; h; var out) => gemma3v_encode(t, s, planes, int64(w), int64(h), out)
    delete st
}

// The twin's compares, after the block, on the rows its forms kept.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private test_gemma3v_discrete_twin(tt : T?; twin : VisionTwin) {
    twin_held(tt, "gemma3v", "", 14l, twin)
}

// The gemma-3-4b decoder's block, nested in the tower's `image` form: an image chat turn reads both files.
[cold_path]
def private gemma3_4b_decoder(t : T?; var emb : VisionEmbedder) {
    unsafe {
        t |> run("the gemma-3-4b decoder under the gemma3v tower") @capture(& emb)(t : T ?) {
            return if (!vision_chat_gate(t, "gemma3", "gemma-3-4b-it-Q8_0.gguf", 0))
            with_cpu_and_gpu_model(t, "vision", "gemma-3-4b-it-Q8_0.gguf", VISION_CHAT_CTX, [cpu_cells_form(QuantMode.q8)]) $(var m; form) {
                vision_chat_cell(t, "the gemma-3-4b image turn through the gemma3v tower: stream shape, then the caption", m, form, false, emb, @@test_vision_chat_gemma3)
            }
        }
    }
}

// The gemma-3-4b mmproj, one block.
[test(description = "The gemma-3-4b SigLIP tower's mmproj loads once a form. On the exact f32 lane it reads its geometry off the mmproj and matches the llama-mtmd encode dumps of five 896 canvases within 4e-3 of token rms, panics on a non-896 canvas, and refuses a gemma4v mmproj and the over-cap f32 twin. On the Metal tower it matches the dumps within 4e-2 of token rms with flash attention on and off, the knob-off leg runs the CPU chain with one decline, the q8 lane serves with a red poison control, and the f16 twin route serves; with the tower on, the post-norm, grid pool, soft norm and projection run on the device behind the blocks in one tail encode, landing within the deep-tower bar of the CPU chain while another canvas lands outside it. The q8 block-GEMM serving lane matches the dumps within 0.32 of token rms on the CPU and on the serving discrete home's device rung, with a mid-stack zeroed-block poison red, and the vision carrier stamps q8 or f32 by lane. The serving discrete home's chain matches the CPU q8 chain on one- and two-block towers and the full tower, a zeroed-block poison reds the one-block bar, and the exact f32 lane is served on the device. Under the mmproj's image form the gemma-3-4b decoder loads once: the pair sniffs as gemma3v at 56 px, splices the fixed 256 rows between start_of_image and end_of_image, and greedily captions the cats in over 40 characters")]
def test_gemma3v_tower(t : T?) {
    return if (!family_on(t, "gemma3v"))
    var inscope twin <- gemma3v_twin()
    var inscope tail = TailKept()
    let twin_on = empty(twin_form_why(""))
    let metal_on = empty(metal_form_why(""))
    var inscope forms <- ["image"]
    add_served_forms(t, forms, MM, ["q8@dev"], @@metal_or_twin_form_why)
    add_served_forms(t, forms, MM, ["q8_z13@dev"], @@device_rung_form_why)
    forms |> push("image_f32")
    forms |> push("image_q8")
    add_served_forms(t, forms, MM, ["f32", "f32@seat"], @@metal_or_twin_form_why)
    add_served_forms(t, forms, MM, [for (f in TWIN_FORMS); f], @@twin_form_why)
    let ran = with_model_once(t, "vision", "mmproj-gemma-3-4b-it-F16.gguf", forms, @@tower_stage_key, @@gemma3v_stage, @@gemma3v_form) $(var e; form) {
        tower_form_is(t, MM, form, e.gemma3v)
        twin.form = form
        if (form == "image") {
            tower_cell(t, "tier-1 GPU attempt vs the mtmd encode dumps (scale-relative rung)", e.gemma3v, @@test_gemma3v_tier1_gpu)
            gemma3_4b_decoder(t, e)
        } elif (form == "image_f32") {
            tower_cell(t, "tier-1 tower parity vs the mtmd encode dumps (gemma-3-4b)", e.gemma3v, @@test_gemma3v_tier1)
            tower_cell(t, "tier-1 on the q8 block-GEMM lane: the exact lane's carrier stamp", e, @@carrier_stamps_exact)
            tower_cell(t, "tier-1 GPU attempt: the f16 twin route", e.gemma3v, @@gpu_twin_route)
        } elif (form == "image_q8") {
            tower_cell(t, "tier-1 on the q8 block-GEMM lane (the serving default)", e, @@test_gemma3v_tier1_q8)
        } elif (form == "q8_z13@dev") {
            tower_cell(t, "the q8 lane's dump bar: a zeroed block through the device chain reds it", e.gemma3v, @@g3v_poison_leg)
        } elif (twin_on) {
            if (form == "q8@dev") {
                tower_cell(t, "tier-1 on the q8 block-GEMM lane: the serving discrete home's device rung vs the mtmd encode dumps", e.gemma3v, @@q8_device_rung)
            }
            tower_cell(t, "the serving discrete home's chain, the {form} form's legs", e.gemma3v, twin, @@twin_legs)
        } elif (form == "q8@dev") {
            tower_cell(t, "tier-1 GPU attempt: the q8 lane on the Metal tower", e.gemma3v, @@gpu_q8_lane)
        } elif (form == "f32") {
            tower_cell(t, "the device tail: the CPU chain on the exact lane's image", e.gemma3v, tail, @@tail_cpu_leg)
        } elif (form == "f32@seat") {
            tower_cell(t, "the device tail: the Metal chain on the exact lane's image", e.gemma3v, tail, @@tail_dev_leg)
            coverage_tower_gemma3v_metal(t,
                "gemma3v: the kernel census's 896 canvas, both attention routes",
                e.gemma3v)
            twin_w_census_leg(t,
                "gemma3v: the kernel census's 896 canvas on the halfword twin's route",
                e.gemma3v)
        }
    }
    if (ran && metal_on) {
        kept_cell(t, "the post-norm, grid pool, soft norm and projection ride the device behind the blocks", tail, @@test_gemma3v_tail_gpu)
    }
    if (ran && twin_on) {
        kept_cell(t, "the serving discrete home's chain ({serving_tower_dev(TowerFamily.vision)}) vs the CPU q8 chain, and the exact lane served", twin, @@test_gemma3v_discrete_twin)
        gpu_tower_shutdown(TowerFamily.vision)
    }
}
