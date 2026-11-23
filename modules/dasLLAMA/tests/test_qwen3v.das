options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_qwen3v
require dasllama/dasllama_image   // image_path_for - the baked-tag sniff cell locates the q8 .dlim
require dasllama/dasllama_vision_embedder   // load_vision_embedder - the sniff cell routes the baked tag
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require _model_once   // with_model_once: a tower file's one block in the area; own_load
require _gpu_knobs   // serving_tower_dev: the q8 lane's GPU image leg
require dasllama/dasllama_tower   // tower_dev_tag: the GPU image's lane name
require ?das_metal dasllama/dasllama_metal_tower   // the CPU gates pin the tower knob off
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown - the f16-route cell re-arms crowned PSOs
require ?vulkan dasllama/dasllama_vulkan_tower
require dasllama/dasllama_common   // set_metal_tensor_crowns - the f16-route cell's crown pin
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require math   // nolint:STYLE030,LINT019 — used only inside the GPU static_if arms; the no-GPU lane sees it unused
require _f32_tile_pin   // with_reference_f32_tile - the q8 legs' bars are measured at the tile's reference order
require _tower_twin   // set_every_gpu_tower, the Vulkan twin's seat and instrument
require _vision_forms   // the towers' forms, the twin's kept canvases, a block's cell
require _cpu_gpu_model   // with_cpu_and_gpu_model: a chat decoder's one block in the area
require _vision_chat_cells   // the image chat turn's cells, run under the tower's image form

// Tier-1 qwen3v tower parity vs llama-mtmd-debug -p encode (the Omni-30B SigLIP-geometry ViT).
// Reference arm: the f32-widened mmprojs, CPU, -fa off (qwen3vl-vision-oracle/mint.sh mints the
// Omni encode.* dumps, mint_4b.sh the 4B deepstack encode4b.* dumps).
// Fixtures: pre-resized [0,1] float canvases - no preprocessing, so a diff isolates the tower math.
// Each mmproj loads once a form, in its one `with_model_once` block (`qwen3v_form`): the image rail's forms carry the CPU
// lanes' cells and the Metal tower's, the forms minted in memory the zeroed-layer controls, the device lanes and the
// discrete twin's. A form's cells run on one home, so a compare of the CPU chain with a device chain is two forms, made
// after the block on the rows they kept. The q8 cell's baked-tag and GPU-image legs hold their own loads.
// The twin runs on the serving discrete home: under the CUDA ask (DASLLAMA_CUDA=1 or set_gpu_home("cuda")) on the CUDA
// tower, its forms skipping without a CUDA device there (`cuda_twin_seat_why`); else on the Vulkan tower, skipping on a
// build with das_metal - the Metal driver owns the tower hooks there -, without a Vulkan device under DASLLAMA_GPU=1 and
// without the tower's tile family (`vulkan_twin_seat_why`). The q8 cell's GPU image leg skips where the serving tower's
// device does not stand up.

let private MM = "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf"
let private MM_4B = "mmproj-Qwen3VL-4B-Instruct-F16.gguf"

let private TIER1_ABS = 2e-4
// x token rms. Poison-calibrated: 27 blocks whose GELU rides the f16 LUT quantize the
// summation-order drift between das GEMMs and the reference's to a measured floor of
// 0.6-3.2e-3 (the ff_pad width regroups the down GEMM's f32 accumulation - red448 tops the
// set; the solid-color fixtures sit at low rms, where the floor shows; preproc, the
// pos-table antialias resize, and the rope tables are each independently bit-exact vs their
// references). The live zero-layer poison leg below reds this bar at 100x+.
let private TIER1_REL = 1.5e-2

// the deepstack leg runs FOUR GELU-LUT'd merger chains per wide row (main + three taps) where
// the Omni runs one, and the q-probes read individual slice values - the compounded
// summation-order floor measures 1.12e-2 on the low-rms gray fixture (a skipped-tap poison
// lands at 6.9-9.7 on the same probes, 600x this bar)
let private TIER1_REL_DS = 4e-2

// x token rms - the q8 serving lane vs the f32 oracle, both q8 legs measured at the fp32 batch tile's
// reference permutation (with_reference_f32_tile): the q8 requant turns any change in the fp32 GEMMs'
// summation order into a different draw of the q8 noise, and the low-rms gray448 fixture's worst token
// reads 0.48 / 0.76 / 0.82 / 0.84 x rms under the row nest, the reference tile, the M5's crowned tile
// and DAS_TUNE_POLICY=reference (the exact lane holds 1.5e-2 under all of them). The bar covers the
// spread; the qwen ViT's mid-network outlier channels put it far above gemma3v's 0.22-0.25 q8 floor.
// The zero-layer poison leg proves the bar still discriminates (2.7 x rms on gray448); the caption is
// the product gate.
// the Metal tower legs: the f32 mulmm stages f16 tiles (the gemma3v precedent, its own
// TIER1_REL_GPU = 4e-2), so the GPU chain's floor runs ~6x the CPU chain's on the low-rms
// uniform fixtures - measured 8.9e-3 worst on the Omni (gray448). The deepstack leg reads
// mid-network tap slices where that noise arrives unwashed by the final post-LN, so - the
// q8-ds precedent one bar up - the uniform gray fixture stays on the CPU set only and the
// GPU-ds legs run the structured fixtures (measured 0.8-1.0e-2). The live poison legs red
// these bars at 10-150x (exact zero-layer 0.87, q8 zero-layer 0.73, zeroed tap 11.4).
let private TIER1_REL_GPU = 4e-2
let private TIER1_REL_GPU_DS = 8e-2

let private TIER1_REL_Q8 = 1.0
// the deepstack q8 leg reads tap slices SAMPLED MID-NETWORK (blocks 5/11/17), where the q8
// outlier noise arrives unwashed by the final post-LN: cb448 measures 0.645 x rms under the
// reference tile and the row nest, 0.66 under the M5's crowned tile; the
// uniform gray fixture measures 7.9 x rms on the q1/q2/q3 slice probes and stays on the EXACT
// lane's set only - the zeroed-slices decoder control in test_vision_chat is the gate that
// proves the served slices still carry their signal
let private TIER1_REL_Q8_DS = 8.2e-1

let private POISON_BLOCK = 13l   //! the whole-tower input poison's block: a mid-stack block past the first deepstack tap
let private TAP1_BLOCK = 11l   //! the 4B tower's tap-1 block (`ds_l1`), the deepstack q8 bar's poison: it hits the main path and a served slice

//! a qwen3v tower file's staged read the key `key` names (`tower_stage_key`), under the key's lane pin and image flavor
def private qwen3v_stage(path, key : string) : Qwen3vStaging =>
    <- staged_tower_form(path, key, @@set_qwen3v_q8, @@reset_qwen3v_q8, @@stage_qwen3v_tower)

//! One form of a qwen3v tower's file as the vision carrier holds it, its block's form loader (`tower_form`): the image
//! rail's forms through `load_vision_embedder`, any other minted in memory from `stg`, its own copy of the file's staging
//! under the form's lane pin, cut and poisoned - never `load_qwen3v_tower`, which bakes a `.dlim` under the pin.
def private qwen3v_form(path, form : string; var stg : Qwen3vStaging) : VisionEmbedder {
    let f = tower_form(form)
    return <- image_tower_form(path, f, @@set_qwen3v_q8, @@reset_qwen3v_q8, @@qwen3v_serves_q8) if (f.image)
    var inscope e = VisionEmbedder(kind = VisionKind.qwen3v)
    truncate_and_poison(stg, f.nl, f.zero_block)
    mint_qwen3v_tower(stg, e.qwen3v, tower_form_dev(f))
    return <- e
}

def private gate_encode(t : T?; tw : Qwen3vTower; var st : Qwen3vState; kind : string; w, h : int; logname : string;
                        rel : float = TIER1_REL) : bool {
    return oracle_gate(t, path_join(models_dir(), "qwen3vl-vision-oracle"), logname, "qwen3vl-vision-oracle/mint.sh (encode.*) / mint_4b.sh (encode4b.*)", kind, w, h,
            "qwen3v tier-1 {kind} {w}x{h}", int(qwen3v_row_width(tw)), TIER1_ABS, rel) $(planes : array<float>; var out : array<float>) : int64 {
        return qwen3v_encode(tw, st, planes, int64(w), int64(h), out)
    }
}

//! how far the `kind` canvas at w x h through `ptw` - a tower with a layer zeroed - lands past the bar `rel` the dump
//! `logname` holds (`encode_excess`); `found` false where the dump is not stocked
def private poison_excess(ptw : Qwen3vTower; kind : string; w, h : int; logname : string; rel : float) : tuple<found : bool; excess : float> {
    var o = OracleEncode()
    return (found = false, excess = 0.0) if (!load_oracle_encode(path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), logname), o))
    var st = Qwen3vState()
    let planes <- make_planes(kind, w, h)
    var pout : array<float>
    with_dasllama_jobque_() {
        qwen3v_encode(ptw, st, planes, int64(w), int64(h), pout)
    }
    let excess = encode_excess(pout, int(qwen3v_row_width(ptw)), o, TIER1_ABS, rel)
    delete pout
    delete st
    return (found = true, excess = excess)
}

// The Omni-30B tower on the exact CPU lane, on the Omni file's `image_f32` form.
[cold_path]
def private test_qwen3v_tier1(tt : T?; tw : Qwen3vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var st = Qwen3vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.patch), 16, "patch")
        tt |> equal(int(tw.merge), 2, "spatial merge")
        tt |> equal(int(tw.n_embd), 1152, "tower width")
        tt |> equal(int(tw.n_layer), 27, "blocks")
        tt |> equal(int(tw.n_head), 16, "heads")
        tt |> equal(int(tw.n_ff), 4304, "ffn width")
        tt |> equal(int(tw.proj_dim), 2048, "soft-token width")
        tt |> equal(int(tw.pos_side), 48, "pos-table side")
        tt |> success(tw.blk_bf16 && tw.mm_bf16, "the file carries bf16 GEMMs (the exact lane serves the blocks f32)")
        tt |> gate_encode(tw, st, "cb", 96, 96, "encode.cb96.log")   // 9 tokens: the pos-table DOWNSCALE arm
        tt |> gate_encode(tw, st, "gray", 448, 448, "encode.gray448.log")
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode.cb448.log")
        tt |> gate_encode(tw, st, "red", 448, 448, "encode.red448.log")
        tt |> gate_encode(tw, st, "green", 448, 448, "encode.green448.log")
        tt |> gate_encode(tw, st, "blue", 448, 448, "encode.blue448.log")
        tt |> gate_encode(tw, st, "cb", 640, 320, "encode.cb640x320.log")   // non-square: the merge-reorder + transposed-grid gate
        var ragged_tripped = false
        try {
            var planes : array<float>
            planes |> reserve(48l * 32l * 3l)
            planes |> resize(48l * 32l * 3l)
            var out : array<float>
            qwen3v_encode(tw, st, planes, 48l, 32l, out)   // 48 is not a multiple of the 32 px merged patch
            delete out
            delete planes
        } recover {
            ragged_tripped = true
        }
        tt |> success(ragged_tripped, "a canvas off the 32 px merged-patch grid panics by name")
        delete st
    }
}

// The negative control for the exact bar, on the Omni file's `f32_z13@seat` form: layer 13's f32 layer_w region zeroed
// through the staging planes (the baked twin serves the GPU route - a blob-only poison is laundered) must red the same
// fixture on the same bar (the loosened-bar rule), on the tower the box serves.
[cold_path]
def private exact_poison_leg(tt : T?; ptw : Qwen3vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(true)
        let p = poison_excess(ptw, "gray", 448, 448, "encode.gray448.log", TIER1_REL)
        if (p.found) {
            to_log(LOG_INFO, "qwen3v exact poison leg: zero layer 13 -> excess {p.excess} over the bar\n")
            tt |> success(p.excess > 0.0, "a zeroed mid-layer f32 region reds the exact bar (excess {p.excess})")
        }
    }
}

// The negative control for the GPU bars, on the same form of a das_metal build: the same zero-layer-13 poison, GPU-served,
// must red TIER1_REL_GPU (the loosened-bar rule wants the control ON THIS lane).
[cold_path]
def private gpu_poison_leg(tt : T?; ptw : Qwen3vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            let p = poison_excess(ptw, "gray", 448, 448, "encode.gray448.log", TIER1_REL_GPU)
            if (p.found) {
                to_log(LOG_INFO, "qwen3v GPU-lane poison leg: zero layer 13 -> excess {p.excess} over the GPU bar\n")
                tt |> success(p.excess > 0.0, "a zeroed mid-layer reds the GPU bar on the GPU-served lane (excess {p.excess})")
            }
        }
    }
}

// The Metal tower arm, on a file's `image` form: the SAME fixtures and bars, tower pinned ON — the GPU chain's parity
// gate, with engage evidence (encode/block counter deltas), never "the model ran". The Omni file runs the narrow fixtures,
// the 4B file the deepstack ones.
[cold_path]
def private test_qwen3v_tier1_metal(tt : T?; tw : Qwen3vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            var st = Qwen3vState()
            let s0 = metal_tower_stats()
            var gated = 0l
            if (tw.n_deepstack == 0l) {
                for (fx in [(kind = "gray", w = 448, h = 448, log = "encode.gray448.log"), (kind = "cb", w = 448, h = 448, log = "encode.cb448.log"),
                            (kind = "cb", w = 640, h = 320, log = "encode.cb640x320.log")]) {
                    gated += (tt |> gate_encode(tw, st, fx.kind, fx.w, fx.h, fx.log, TIER1_REL_GPU)) ? 1l : 0l
                }
            } else {
                for (fx in [(kind = "cb", w = 448, h = 448, log = "encode4b.cb448.log"), (kind = "cb", w = 640, h = 320, log = "encode4b.cb640x320.log"),
                            (kind = "cb", w = 96, h = 96, log = "encode4b.cb96.log")]) {
                    gated += (tt |> gate_encode(tw, st, fx.kind, fx.w, fx.h, fx.log, TIER1_REL_GPU_DS)) ? 1l : 0l
                }
            }
            let s1 = metal_tower_stats()
            to_log(LOG_INFO, "qwen3v metal tier-1 ({tw.n_layer} blocks): +{s1.encodes - s0.encodes} encodes, +{s1.blocks - s0.blocks} blocks, +{s1.declines - s0.declines} declines ({gated} fixtures gated)\n")
            if (gated == 0l) {
                tt |> skip("no oracle dumps present - the GPU parity fixtures all skipped")
            } else {
                tt |> success(s1.encodes - s0.encodes >= gated, "every gated encode served on the Metal tower")
                tt |> equal(s1.blocks - s0.blocks, gated * tw.n_layer, "the block counters advanced per gated fixture")
            }
            delete st
        }
    } else {
        tt |> skip("no das_metal on this build - the Metal tower arm has no device to serve")
    }
}

// The q8 lane on the Metal device, on the 4B file's `q8@dev` form of a das_metal build: the deepstack tower minted q8 in
// the Metal flavor serves every block off its q8 planes on the q8 bar, no quant_mode decline.
[cold_path]
def private q3v_metal_q8_leg(tt : T?; twq : Qwen3vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            tt |> success(twq.q8, "the q8 lane minted under the pin")
            var stq = Qwen3vState()
            q8_lane_served(tt, "q8 tower", TowerFamily.vision, twq.n_layer) $() : bool {
                return tt |> gate_encode(twq, stq, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_Q8_DS)
            }
            delete stq
        }
    }
}

// A canvas's device-side dump bar poisoned, on the 4B file's `q8_z13@dev` form: the deepstack q8 tower with `POISON_BLOCK`
// zeroed must exceed the bar the cb 448 dump holds.
[cold_path]
def private q3v_poison_leg(tt : T?; twp : Qwen3vTower) {
    var o = OracleEncode()
    return if (!oracle_dump(tt, "qwen3vl-vision-oracle", "encode4b.cb448.log", "qwen3vl-vision-oracle/mint_4b.sh", o))
    var stp = Qwen3vState()
    dump_poison_leg(tt, "qwen3v", "cb", 448, 448, o, TIER1_REL_Q8_DS, POISON_BLOCK, int(qwen3v_row_width(twp)), TIER1_ABS, twp, stp) $(t; var s; planes; var out) => qwen3v_encode(t, s, planes, 448l, 448l, out)
    delete stp
}

// The Metal tower's twin-W route (crowned twins), on a file's `image` form: the 4B f16 tower's gated encodes ride the
// f16-W route and its knob-off leg keeps the f32 GEMM route; the Omni bf16 tower rides the twin route.
[cold_path, arch(at="../ARCHITECTURE_IMAGE_GPU_PLANES.md#image-tower-twin-plane")]
def private test_qwen3v_tier1_metal_f16(tt : T?; tw : Qwen3vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knobs_found = gpu_knobs()
        let f16_found = get_metal_tower_f16()
        let crowns_found = get_metal_tensor_crowns()
        set_metal_tensor_crowns("mulmm_q8")
        metal_prefill_shutdown()
        set_metal_tower(true)
        set_metal_tower_f16(true)
        var st = Qwen3vState()
        if (tw.n_deepstack == 0l) {
            let b0 = metal_tower_f16_encodes()
            if (tt |> gate_encode(tw, st, "cb", 448, 448, "encode.cb448.log", TIER1_REL_GPU)) {
                tt |> success(metal_tower_f16_encodes() - b0 >= 1l, "the bf16-sourced tower rode the twin route")
            }
        } else {
            let f0 = metal_tower_f16_encodes()
            var gated = 0l
            if (tt |> gate_encode(tw, st, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_GPU_DS)) {
                gated++
            }
            if (tt |> gate_encode(tw, st, "cb", 96, 96, "encode4b.cb96.log", TIER1_REL_GPU_DS)) {
                gated++
            }
            let f1 = metal_tower_f16_encodes()
            if (gated == 0l) {
                tt |> skip("no oracle dumps present - the f16-route parity fixtures all skipped")
            } else {
                tt |> success(f1 - f0 >= gated, "every gated encode rode the f16-W route ({f1 - f0} of {gated})")
                set_metal_tower_f16(false)
                tt |> gate_encode(tw, st, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_GPU_DS)
                tt |> equal(metal_tower_f16_encodes() - f1, 0l, "the knob-off leg keeps the f32 GEMM route")
            }
        }
        set_metal_tower_f16(f16_found)
        set_metal_tensor_crowns(crowns_found)
        set_gpu_knobs(knobs_found)
        metal_prefill_shutdown()
        delete st
    } else {
        tt |> skip("no das_metal on this build - the f16-W route has no device to serve")
    }
}

// The Qwen3-VL 4B deepstack tower on the exact CPU lane, on the 4B file's `image_f32` form.
[cold_path]
def private test_qwen3v_tier1_deepstack(tt : T?; tw : Qwen3vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var st = Qwen3vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.n_embd), 1024, "tower width")
        tt |> equal(int(tw.n_layer), 24, "blocks")
        tt |> equal(int(tw.n_ff), 4096, "ffn width")
        tt |> equal(int(tw.proj_dim), 2560, "soft-token width (main slice)")
        tt |> equal(int(tw.n_deepstack), 3, "deepstack taps")
        tt |> equal(int(tw.ds_l0), 5, "tap 0 block")
        tt |> equal(int(tw.ds_l1), 11, "tap 1 block")
        tt |> equal(int(tw.ds_l2), 17, "tap 2 block")
        tt |> equal(int(qwen3v_row_width(tw)), 10240, "wide row = (1+3) x 2560")
        tt |> gate_encode(tw, st, "gray", 448, 448, "encode4b.gray448.log", TIER1_REL_DS)
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_DS)
        tt |> gate_encode(tw, st, "cb", 640, 320, "encode4b.cb640x320.log", TIER1_REL_DS)   // non-square: merge-reorder + transposed-grid
        tt |> gate_encode(tw, st, "cb", 96, 96, "encode4b.cb96.log", TIER1_REL_DS)   // 9 tokens: the pos-table DOWNSCALE arm
        delete st
    }
}

//! the q8-lane image's baked tag routes through the carrier sniff (the qwen3v-q8 arm), with the GPU towers off as the caller
//! left them; the image is this build's own mint, never a file an earlier build left beside the mmproj
def private q8_baked_tag_leg(tt : T?) {
    own_load("the cell reloads the model: it mints the q8 lane's CPU image into scratch through the cold load, then loads that image through the carrier")
    let mm = path_join(models_dir(), "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
    let img = path_join(test_temp_dir(), base_name(image_path_for(mm, "qwen3v-q8")))
    remove(img)
    set_qwen3v_q8(true)
    with_image_destination(img) {
        var inscope minted <- load_qwen3v_tower(mm)
        tt |> success(minted.q8, "the q8 lane mints its CPU image")
    }
    reset_qwen3v_q8()
    tt |> success(stat(img).is_valid, "the mint wrote the image the tag leg reads")
    if (stat(img).is_valid) {
        var inscope e <- load_vision_embedder(img)
        tt |> equal(vision_family(e), "qwen3v", "the baked q8 tag sniffs to the qwen3v arm")
        tt |> equal(vision_exec_fmt(e), "q8", "the carrier stamps the q8 lane")
    }
    remove(img)
}

//! the q8 lane's GPU image, with the GPU towers on as the caller pinned them: the cold load mints it into scratch where the
//! serving tower's device stands up, and the image loads by its path with its device block bound
def private q8_gpu_image_leg(tt : T?) {
    own_load("the cell reloads the model: it mints the q8 lane's GPU image into scratch through the cold load, then loads that image by its path")
    let mm = path_join(models_dir(), "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
    let dev = serving_tower_dev(TowerFamily.vision)
    set_qwen3v_q8(true)
    let imgd = path_join(test_temp_dir(), base_name(image_path_for(mm, tower_dev_tag("qwen3v-q8", dev))))
    remove(imgd)
    with_image_destination(imgd) {
        var inscope cold <- load_qwen3v_tower(mm)
        tt |> equal(cold.dev_backend, dev, "the cold load of the q8 lane mints the {dev} image where its device stands up")
    }
    var inscope byp <- load_qwen3v_tower(imgd)
    tt |> success(byp.dev_backend == dev && !empty(byp.dev), "the {dev} q8 image loads by its path, its device block bound")
    reset_qwen3v_q8()
    remove(imgd)
}

// The q8 serving lane (the CPU default), on the Omni file's `image_q8` form: the same dumps on their own scale-relative
// bars; then the baked-tag and GPU-image legs, each its own load.
[cold_path]
def private test_qwen3v_tier1_q8(tt : T?; tw : Qwen3vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)   // a CPU-lane claim: the knob-off load took the lane's CPU image (`#tower-dev-block`)
        var st = Qwen3vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> equal(int(tw.ff_pad), 4352, "ffn served at the layout's padded width")
        with_reference_f32_tile(tt) {
            tt |> gate_encode(tw, st, "gray", 448, 448, "encode.gray448.log", TIER1_REL_Q8)
            tt |> gate_encode(tw, st, "cb", 448, 448, "encode.cb448.log", TIER1_REL_Q8)
            tt |> gate_encode(tw, st, "cb", 96, 96, "encode.cb96.log", TIER1_REL_Q8)
        }
        delete st
        q8_baked_tag_leg(tt)
        set_every_gpu_tower(true)   // the GPU leg's pin
        if (serving_tower_device_up(TowerFamily.vision)) {
            q8_gpu_image_leg(tt)
        } else {
            tt |> skip("no GPU device stands up for the serving tower - the GPU image leg")
        }
    }
}

// The q8 bar's negative control, on the Omni file's `q8_z13` form: layer 13's qblob region zeroed through the staging
// planes (the gemma4v knockout shape) must red the same fixture on the same bar.
[cold_path]
def private q8_poison_leg(tt : T?; ptw : Qwen3vTower) {
    q8_poison_reds(tt, ptw, "gray", "encode.gray448.log", TIER1_REL_Q8, "q8 poison leg: zero layer 13",
                   "a zeroed mid-layer qblob region reds the bar")
}

//! a q8 poison form's control: the poisoned tower `ptw` on the `kind` 448 x 448 canvas against the dump `log` exceeds the
//! bar `rel`, logged as `leg` and asserted as `what`, the CPU lane under the reference f32 tile
def private q8_poison_reds(tt : T?; ptw : Qwen3vTower; kind, log : string; rel : float; leg, what : string) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        with_reference_f32_tile(tt) {
            let p = poison_excess(ptw, kind, 448, 448, log, rel)
            if (p.found) {
                to_log(LOG_INFO, "qwen3v {leg} -> excess {p.excess} over the bar\n")
                tt |> success(p.excess > 0.0, "{what} (excess {p.excess})")
            }
        }
    }
}

// The deepstack q8 lane, on the 4B file's `image_q8` form.
[cold_path]
def private test_qwen3v_tier1_q8_deepstack(tt : T?; tw : Qwen3vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)   // a CPU-lane claim: the knob-off load took the lane's CPU image (`#tower-dev-block`)
        var st = Qwen3vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> equal(tw.ds_l1, TAP1_BLOCK, "the tap-1 block is the one the bar's poison form zeroes")
        with_reference_f32_tile(tt) {
            tt |> gate_encode(tw, st, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_Q8_DS)
            tt |> gate_encode(tw, st, "cb", 96, 96, "encode4b.cb96.log", TIER1_REL_Q8_DS)
        }
        delete st
    }
}

// The negative control for the deepstack q8 bar, on the 4B file's `q8_z11` form: the same zero-layer knockout must red it
// (tap 1's block, so the poison hits both the main path and a served slice).
[cold_path]
def private q8_deepstack_poison_leg(tt : T?; ptw : Qwen3vTower) {
    q8_poison_reds(tt, ptw, "cb", "encode4b.cb448.log", TIER1_REL_Q8_DS, "deepstack q8 poison leg: zero the tap-1 block",
                   "a zeroed tap block reds the deepstack bar")
}

//! the discrete twin's canvases, depths and exact-lane leg: structured canvases on one-, two- and six-block towers, the
//! poison on the one-block tower, where the bar is tightest, then on the whole tower with the non-square canvas (the
//! merge-reorder and the transposed grid); no gray canvas - the q8 lane is no instrument there (its own cells keep gray on
//! the exact set)
def private qwen3v_twin() : VisionTwin {
    var twin <- vision_twin("cb", 448, 448, "qwen3v cb 448x448", 784l)
    twin.canvases <- [twin_canvas(1l, "cb", 96, 96, 0l), twin_canvas(1l, "cb", 448, 448),
        twin_canvas(2l, "cb", 96, 96), twin_canvas(2l, "cb", 448, 448),
        twin_canvas(6l, "cb", 96, 96), twin_canvas(6l, "cb", 448, 448),
        twin_canvas(0l, "cb", 96, 96), twin_canvas(0l, "cb", 448, 448), twin_canvas(0l, "cb", 640, 320)]
    twin.depths <- [TwinDepth(nl = 1l), TwinDepth(nl = 2l), TwinDepth(nl = 6l)]
    return <- twin
}

//! the forms only the discrete twin reads; with the q8 form on the device (`q8@dev`) they are its legs, the forms of one
//! staged read (`tower_stage_key`) side by side
let private TWIN_FORMS = fixed_array("q8_1L", "q8_2L", "q8_6L", "q8", "q8_1L@dev", "q8_1L_z0@dev", "q8_2L@dev", "q8_6L@dev",
    "f32_1L", "f32_2L", "f32_6L", "f32", "f32@seat", "f32_z13@seat")

// A form's legs of the discrete twin: its canvases three ways, a form a chain, and the exact lane served on the device.
[cold_path]
def private twin_legs(tt : T?; tw : Qwen3vTower; var twin : VisionTwin) {
    if (twin.form == "f32") {
        tt |> success(!tw.q8 && tw.n_deepstack == 3l, "exact-plane lane minted, three deepstack taps")
    } elif (twin.form == "q8") {
        tt |> success(tw.q8, "q8 lane minted")
    }
    var st = Qwen3vState()
    twin_form_legs(tt, twin, tw, st) $(t; var s; planes; w; h; var out) => qwen3v_encode(t, s, planes, int64(w), int64(h), out)
    delete st
}

// The twin's compares, after the block, on the rows its forms kept.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private test_qwen3v_discrete_twin(tt : T?; twin : VisionTwin) {
    twin_held(tt, "qwen3v", ", 3 taps", 16l, twin)
}

// The Qwen3-Omni-30B decoder's block, nested in the tower's `image` form: an image chat turn reads both files.
[cold_path]
def private omni_decoder(t : T?; var emb : VisionEmbedder) {
    unsafe {
        t |> run("the Qwen3-Omni-30B decoder under the qwen3v tower") @capture(& emb)(t : T ?) {
            return if (!vision_chat_gate(t, "qwen3", "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf", 56))
            with_cpu_and_gpu_model(t, "vision", "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf", VISION_CHAT_CTX, [cpu_cells_form(QuantMode.q8)]) $(var m; form) {
                vision_chat_cell(t, "the Omni image turn through the qwen3v tower: stream shape, the grid, the caption", m, form, false, emb, @@test_vision_chat_omni)
                vision_chat_cell(t, "one Omni session: image turn, then a text turn recalling it", m, form, false, emb, @@test_omni_showcase)
            }
        }
    }
}

// The Omni-30B mmproj, one block.
[test(description = "The Omni-30B qwen3v tower's mmproj loads once a form. On the exact CPU lane it loads its pinned shape and matches the mtmd dumps on seven canvases within 2e-4 abs plus 1.5e-2 of token rms; an off-grid canvas panics and a zeroed layer 13 reds the bar. On the Metal tower it matches the dumps within 4e-2 of token rms with every gated encode and block counted, rides the twin-W route, and a zeroed layer reds the GPU bar. The q8 serving lane matches the dumps within 1.0 of token rms at the reference f32 tile, a zeroed layer-13 qblob reds that bar, the baked q8 image sniffs to qwen3v, and the GPU q8 image mints and reloads. Under the mmproj's image form the Qwen3-Omni-30B decoder loads once: the image turn splices 300 rows on a 20x15 grid between the vision markers with user text after, captions the cats greedily, and leaves the session's rope delta at 20 - 300; one session then describes the cats on an image turn and answers yes on a text follow-up that needs the image turn's history, the rope delta persisting across turns")]
def test_qwen3v_omni(t : T?) {
    return if (!family_on(t, "qwen3v"))
    with_model_once(t, "vision", "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf", ["image", "image_f32", "f32_z13@seat", "image_q8", "q8_z13"], @@tower_stage_key, @@qwen3v_stage, @@qwen3v_form) $(var e; form) {
        tower_form_is(t, MM, form, e.qwen3v)
        if (form == "image") {
            tower_cell(t, "tier-1 on the Metal tower (Omni-30B narrow)", e.qwen3v, @@test_qwen3v_tier1_metal)
            tower_cell(t, "tier-1 on the Metal tower's twin-W route (crowned twins, Omni bf16)", e.qwen3v, @@test_qwen3v_tier1_metal_f16)
            omni_decoder(t, e)
        } elif (form == "image_f32") {
            tower_cell(t, "tier-1 tower parity vs the mtmd encode dumps (Omni-30B)", e.qwen3v, @@test_qwen3v_tier1)
        } elif (form == "f32_z13@seat") {
            tower_cell(t, "tier-1 tower parity: a zeroed layer 13 reds the exact bar", e.qwen3v, @@exact_poison_leg)
            tower_cell(t, "tier-1 on the Metal tower: a zeroed layer 13 reds the GPU bar", e.qwen3v, @@gpu_poison_leg)
        } elif (form == "image_q8") {
            tower_cell(t, "tier-1 on the q8 block-GEMM lane (the serving default, Omni-30B)", e.qwen3v, @@test_qwen3v_tier1_q8)
        } elif (form == "q8_z13") {
            tower_cell(t, "tier-1 on the q8 block-GEMM lane: a zeroed layer 13 reds the bar", e.qwen3v, @@q8_poison_leg)
        }
    }
}

// The Qwen3-VL 4B decoder's block, its host sessions' context cut to `HOST_CTX_4B`, nested in the tower's `image` form: an image chat turn reads both files.
[cold_path]
def private qwen3vl_4b_decoder(t : T?; var emb : VisionEmbedder) {
    unsafe {
        t |> run("the Qwen3-VL 4B decoder under the qwen3v deepstack tower") @capture(& emb)(t : T ?) {
            return if (!vision_chat_gate(t, "", "Qwen3VL-4B-Instruct-Q8_0.gguf", 0))
            with_cpu_and_gpu_model(t, "vision", "Qwen3VL-4B-Instruct-Q8_0.gguf", HOST_CTX_4B, [cpu_cells_form(QuantMode.q8)]) $(var m; form) {
                vision_chat_cell(t, "the Qwen3-VL 4B image turn: deepstack wide rows through the chat, then the caption", m, form, false, emb, @@test_vision_chat_deepstack)
                vision_chat_cell(t, "pre-encoded wide rows through add_user_image_rows_ reproduce the embedder walk", m, form, false, emb, @@test_vision_chat_rows_seam)
            }
        }
    }
}

// The Qwen3-VL 4B mmproj, one block.
[test(description = "The Qwen3-VL 4B deepstack tower's mmproj loads once a form. On the exact CPU lane it loads three taps at blocks 5/11/17 with a 10240-wide row and matches the mtmd dumps on four canvases within 2e-4 abs plus 4e-2 of token rms. On the Metal tower it matches the dumps within 8e-2 of token rms with every gated encode and block counted, the q8 lane serves on its bar with a red zeroed-layer poison, every gated encode rides the f16-W route and the knob-off leg stays on the f32 GEMM route. The q8 lane matches the dumps within 0.82 of token rms at the reference f32 tile, and zeroing the tap-1 block's qblob reds that bar. On the serving discrete home, the device chain holds to the CPU q8 and exact chains at 1, 2, 6 and 24 blocks on structured canvases, a one-block zeroed poison exceeds the bar, and the exact lane is device-served. Under the mmproj's image form the Qwen3-VL 4B decoder loads once: the deepstack turn carries 10240-wide rows (1+3 taps of 2560) into the chat and captions the cats, zeroing the deepstack slices moves the decoder's logits by over 1e-2, pre-encoded wide rows fed to a plain chat through `add_user_image_rows_` produce the embedder chat's greedy reply token for token, and narrow rows on a deepstack decoder or a second image before respond panic")]
def test_qwen3v_4b(t : T?) {
    return if (!family_on(t, "qwen3v"))
    var inscope twin <- qwen3v_twin()
    let twin_on = empty(twin_form_why(""))
    var inscope forms <- ["image"]
    add_served_forms(t, forms, MM_4B, ["q8@dev"], @@metal_or_twin_form_why)
    add_served_forms(t, forms, MM_4B, ["q8_z13@dev"], @@metal_form_why)
    forms |> push_from(["image_f32", "image_q8", "q8_z11"])
    add_served_forms(t, forms, MM_4B, [for (f in TWIN_FORMS); f], @@twin_form_why)
    let ran = with_model_once(t, "vision", "mmproj-Qwen3VL-4B-Instruct-F16.gguf", forms, @@tower_stage_key, @@qwen3v_stage, @@qwen3v_form) $(var e; form) {
        tower_form_is(t, MM_4B, form, e.qwen3v)
        twin.form = form
        if (form == "image") {
            tower_cell(t, "tier-1 on the Metal tower (4B deepstack)", e.qwen3v, @@test_qwen3v_tier1_metal)
            tower_cell(t, "tier-1 on the Metal tower's twin-W route (crowned twins, 4B f16)", e.qwen3v, @@test_qwen3v_tier1_metal_f16)
            qwen3vl_4b_decoder(t, e)
        } elif (form == "image_f32") {
            tower_cell(t, "tier-1 deepstack tower parity vs the mtmd encode dumps (Qwen3-VL 4B)", e.qwen3v, @@test_qwen3v_tier1_deepstack)
        } elif (form == "image_q8") {
            tower_cell(t, "tier-1 deepstack on the q8 block-GEMM lane (Qwen3-VL 4B)", e.qwen3v, @@test_qwen3v_tier1_q8_deepstack)
        } elif (form == "q8_z11") {
            tower_cell(t, "tier-1 deepstack on the q8 block-GEMM lane: a zeroed tap block reds the bar", e.qwen3v, @@q8_deepstack_poison_leg)
        } elif (form == "q8_z13@dev") {
            tower_cell(t, "tier-1 on the Metal tower: the q8 lane's dump bar, a zeroed block reds it", e.qwen3v, @@q3v_poison_leg)
        } elif (twin_on) {
            tower_cell(t, "the serving discrete home's chain, the {form} form's legs", e.qwen3v, twin, @@twin_legs)
        } elif (form == "q8@dev") {
            tower_cell(t, "tier-1 on the Metal tower: the q8 lane", e.qwen3v, @@q3v_metal_q8_leg)
        }
    }
    if (ran && twin_on) {
        kept_cell(t, "the serving discrete home's chain ({serving_tower_dev(TowerFamily.vision)}) vs the CPU q8 chain (Qwen3-VL 4B, deepstack), and the exact lane served", twin, @@test_qwen3v_discrete_twin)
        gpu_tower_shutdown(TowerFamily.vision)
    }
}

// the lane policy itself - model-free: the default follows the GPU tower's exact lane and the box's
// accelerate tier (q8 when neither serves), the pin flips it either way, reset returns to the policy
[test(description = "The qwen3v lane default is q8 unless the GPU exact lane or the accelerate tier serves, the pin forces q8 or exact either way, and reset from the exact pin returns to the policy default")]
def test_qwen3v_lane_knob(t : T?) {
    t |> run("the q8 lane pin and policy default") @(tt : T?) {
        tt |> equal(qwen3v_serves_q8(), !(qwen3v_gpu_exact_serves() || float_batch_override_active()), "the policy default follows the Metal tower and the accelerate tier")
        set_qwen3v_q8(true)
        tt |> success(qwen3v_serves_q8(), "pinned q8")
        set_qwen3v_q8(false)
        tt |> success(!qwen3v_serves_q8(), "pinned exact")
        reset_qwen3v_q8()   // from the EXACT pin, so a no-op reset cannot pass as the (q8) policy default
        tt |> equal(qwen3v_serves_q8(), !(qwen3v_gpu_exact_serves() || float_batch_override_active()), "reset returns to the policy default")
    }
}
