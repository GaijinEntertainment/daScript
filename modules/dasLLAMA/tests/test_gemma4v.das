options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemma4v
require dasllama/dasllama_common   // QuantMode: the decoders' load mode
require dasllama/dasllama_vision_embedder   // vision_exec_fmt through the carrier
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // models_dir() + model_available() gate
require _model_once   // with_model_once: a tower file's one block in the area
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require ?das_metal dasllama/dasllama_metal_tower   // the GPU tier-1 attempt; the CPU gates pin the knob off
require ?vulkan dasllama/dasllama_vulkan_tower
require _tower_twin   // set_every_gpu_tower, the Vulkan twin's seat and instrument
require _vision_forms   // the towers' forms, the twin's kept canvases, a block's cell
require _cpu_gpu_model   // with_cpu_and_gpu_model: a chat decoder's one block in the area
require _vision_chat_cells   // the image chat turn's cells, run under the tower's image form
require _kernel_coverage_cells   // coverage_tower_gemma4v_metal: the kernel census's canvas

// Tier-1 gemma4v tower parity vs llama-mtmd-debug -p encode.
// Reference arm: the f32-widened mmproj, CPU, --no-mmproj-offload, -fa off (the only true-f32 arm).
// Fixtures: pre-resized [0,1] float canvases - no preprocessing, so a diff isolates the tower math.
// Dumps: beside the models in gemma4-vision-oracle/ (mint_e2b.sh / mint_e4b.sh regenerate).
// Bar: scale-relative - 16 blocks of bf16-weight GEMMs under summation-order drift cannot promise
// gemma4uv's 2e-4; the logged maxdiff is the number.
// Each mmproj loads once a form, in its one `with_model_once` block (`gemma4v_form`): the image rail's forms carry the CPU
// lanes' cells and the Metal tower's, the forms minted in memory the device lanes', the staged-plane knockouts and the
// discrete twin's. A form's cells run on one home, so a compare of the CPU chain with a device chain is two forms, made
// after the block on the rows they kept.
// The discrete-home cells (the tier-1 rung on the serving discrete home, the twin, the E4B cell's device leg) run on the
// serving discrete home: under the CUDA ask (DASLLAMA_CUDA=1 or set_gpu_home("cuda")) on the CUDA tower, else on the
// Vulkan tower. The tier-1 rung's and the twin's forms skip under the ask without a CUDA device (`cuda_twin_seat_why`), else
// on a build with das_metal - the Metal driver owns the tower hooks there -, without a Vulkan device under DASLLAMA_GPU=1 and
// without the tower's tile family (`vulkan_twin_seat_why`); the E4B device leg has no seat on a das_metal build and skips
// where the serving home's device does not stand up.

let private MM = "mmproj-gemma-4-E2B-it-bf16.gguf"
let private MM_E4B = "mmproj-gemma-4-E4B-it-BF16.gguf"

let private TIER1_ABS = 2e-4
let private TIER1_REL = 4e-3   // × token rms (≈0.75 on E2B) - the rung gemma4uv's GPU leg holds
let private TIER1_REL_GPU = 8e-3   // the Metal leg: 112 f16-staged GEMM tiles over 16 blocks - measured 8.9e-4..3.7e-3 on these dumps
let private TIER1_REL_Q8 = 1.2e-1   // the q8 serving lane: measured 3.1e-2..6.8e-2 (≈9% of rms) on these dumps - 16 blocks of q8×q8 GEMMs; the caption floor is the product gate
let private KNOCKOUT_FLOOR = 1e-3   // the clamp knockout must miss the oracle by at least this (the armed tower holds 1e-3)

let private POISON_BLOCK = 8l   //! the whole-tower input poison's block: mid-stack, so the zero rides the residual through half the chain

var private g_clamps_armed = 0   //! the block clamps the last `+clamps_off` form's staging carried armed before it disarmed them

//! every block GEMM's clamp of the staging `stg` disarmed; how many were armed
def private disarm_clamps(var stg : Gemma4vStaging) : int {
    let d = stg.t.n_embd
    let hd = d / stg.t.n_head
    var armed = 0
    for (l in range64(stg.t.n_layer)) {
        let clamps = stg.t.layer_f + l * stg.t.layer_f_stride + 4l * d + 2l * hd
        for (g in range64(GEMMA4V_GEMMS)) {
            let flag = clamps + g * GEMMA4V_CLAMP_F + 4l
            if (stg.blob[flag] != 0.0) {
                armed++
            }
            stg.blob[flag] = 0.0
        }
    }
    return armed
}

//! a std_bias / std_scale pair synthesized into the staging `stg`: bias 0 and scale 1 (neutral), or bias 0.25 and scale -1
def private add_std_pair(var stg : Gemma4vStaging; biased : bool) {
    let d = stg.t.n_embd
    stg.t.has_std = true
    stg.t.std_bias = long_length(stg.blob)
    stg.t.std_scale = stg.t.std_bias + d
    stg.blob |> reserve(stg.t.std_scale + d)
    stg.blob |> resize(stg.t.std_scale + d)
    for (i in range64(d)) {
        stg.blob[stg.t.std_bias + i] = biased ? 0.25 : 0.0
        stg.blob[stg.t.std_scale + i] = biased ? -1.0 : 1.0
    }
}

//! a gemma4v tower file's staged read the key `key` names (`tower_stage_key`), under the key's lane pin and image flavor
def private gemma4v_stage(path, key : string) : Gemma4vStaging =>
    <- staged_tower_form(path, key, @@set_gemma4v_q8, @@reset_gemma4v_q8, @@stage_gemma4v_tower)

//! One form of a gemma4v tower's file as the vision carrier holds it, its block's form loader (`tower_form`): the image
//! rail's forms through `load_vision_embedder`, any other minted in memory from `stg`, its own copy of the file's staging
//! under the form's lane pin, cut and poisoned - never `load_gemma4v_tower`, which bakes a `.dlim` under the pin. The
//! edits: `+clamps_off` disarms every block clamp, `+std_neutral` and `+std_biased` synthesize a std pair.
def private gemma4v_form(path, form : string; var stg : Gemma4vStaging) : VisionEmbedder {
    let f = tower_form(form)
    return <- image_tower_form(path, f, @@set_gemma4v_q8, @@reset_gemma4v_q8, @@gemma4v_serves_q8) if (f.image)
    var inscope e = VisionEmbedder(kind = VisionKind.gemma4v)
    truncate_and_poison(stg, f.nl, f.zero_block)
    if (f.edit == "clamps_off") {
        g_clamps_armed = disarm_clamps(stg)
    } elif (f.edit == "std_neutral" || f.edit == "std_biased") {
        add_std_pair(stg, f.edit == "std_biased")
    } elif (!empty(f.edit)) {
        panic("the gemma4v tests: '{form}' names no edit of the staged planes")
    }
    mint_gemma4v_tower(stg, e.gemma4v, tower_form_dev(f))
    return <- e
}

[cold_path]
def private gate_encode(t : T?; tw : Gemma4vTower; var st : Gemma4vState; kind : string; w, h : int; logname : string; rel : float = TIER1_REL) : bool {
    return oracle_gate(t, path_join(models_dir(), "gemma4-vision-oracle"), logname, "its gemma4-vision-oracle mint script", kind, w, h,
            "gemma4v tier-1{tw.q8 ? " q8" : ""} {kind} {w}x{h}", int(tw.proj_dim), TIER1_ABS, rel) $(planes : array<float>; var out : array<float>) : int64 {
        return gemma4v_encode(tw, st, planes, int64(w), int64(h), out)
    }
}

// The exact lane on the CPU, on the E2B file's `image_f32` form.
[cold_path]
def private test_gemma4v_tier1(tt : T?; tw : Gemma4vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var st = Gemma4vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.patch), 16, "patch")
        tt |> equal(int(tw.merge), 3, "pool kernel")
        tt |> equal(int(tw.n_embd), 768, "tower width")
        tt |> equal(int(tw.n_layer), 16, "blocks")
        tt |> equal(int(tw.n_head), 12, "heads")
        tt |> equal(int(tw.n_ff), 3072, "ffn width")
        tt |> equal(int(tw.proj_dim), 1536, "soft-token width")
        tt |> equal(int(tw.pos_entries), 10240, "pos-table rows per axis")
        tt |> success(tw.blk_bf16 && tw.proj_bf16 && !tw.patch_bf16, "plane split as the file has it: bf16 GEMMs, f32 patch conv")
        tt |> success(!tw.has_std, "E2B carries no std_bias/std_scale")
        tt |> gate_encode(tw, st, "cb", 96, 96, "e2b.encode.cb96.log")   // 4 tokens, the smallest grid
        tt |> gate_encode(tw, st, "gray", 336, 336, "e2b.encode.gray336.log")
        tt |> gate_encode(tw, st, "cb", 336, 336, "e2b.encode.cb336.log")
        tt |> gate_encode(tw, st, "red", 336, 336, "e2b.encode.red336.log")
        tt |> gate_encode(tw, st, "green", 336, 336, "e2b.encode.green336.log")
        tt |> gate_encode(tw, st, "blue", 336, 336, "e2b.encode.blue336.log")
        tt |> gate_encode(tw, st, "cb", 480, 480, "e2b.encode.cb480.log")
        tt |> gate_encode(tw, st, "cb", 672, 336, "e2b.encode.cb672x336.log")   // non-square: the transposed-grid gate
        // the two canvas guards panic by name instead of reading past a plane
        var wide_tripped = false
        try {
            let w = (tw.pos_entries + 1l) * tw.patch
            var planes : array<float>
            planes |> reserve(w * tw.patch * 3l)
            planes |> resize(w * tw.patch * 3l)
            var wide : array<float>
            gemma4v_encode(tw, st, planes, w, tw.patch, wide)
            delete wide
            delete planes
        } recover {
            wide_tripped = true
        }
        tt |> success(wide_tripped, "a canvas past pos_entries columns panics at the grid check")
        var ragged_tripped = false
        try {
            var planes : array<float>
            planes |> reserve(64l * 48l * 3l)
            planes |> resize(64l * 48l * 3l)
            var out : array<float>
            gemma4v_encode(tw, st, planes, 64l, 48l, out)   // 64 is not a multiple of the 48 px pooled patch
            delete out
            delete planes
        } recover {
            ragged_tripped = true
        }
        tt |> success(ragged_tripped, "a canvas that is not a multiple of the pooled patch panics at the span check")
        delete st
    }
}

// The GPU tier-1 gate, on the E2B file's `image` form: the same dumps through the Metal tower's block loop (bf16 mul_mm
// stages f16 tiles, so the bar is scale-relative and wider than the CPU gate's).
[cold_path]
def private test_gemma4v_tier1_gpu(tt : T?; tw : Gemma4vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knobs_found = gpu_knobs()
        reset_gemma4v_q8()
        set_metal_tower(false)
        tt |> success(gemma4v_serves_q8(), "with the Metal tower off (and no accelerate tier) the default lane is q8")
        set_metal_tower(true)
        tt |> success(!gemma4v_serves_q8(), "with the Metal tower armed the default lane is the file's planes")
        let policy_q8 = gemma4v_serves_q8()
        var st = Gemma4vState()
        tt |> equal(tw.q8, policy_q8, "the load took the policy's unpinned lane under the Metal tower (q8 = {policy_q8})")
        let fixtures = [(kind = "cb", w = 96, h = 96, log = "e2b.encode.cb96.log"),
                        (kind = "cb", w = 336, h = 336, log = "e2b.encode.cb336.log"),
                        (kind = "green", w = 336, h = 336, log = "e2b.encode.green336.log"),
                        (kind = "cb", w = 480, h = 480, log = "e2b.encode.cb480.log"),
                        (kind = "cb", w = 672, h = 336, log = "e2b.encode.cb672x336.log")]
        for (fx in fixtures) {
            let s0 = metal_tower_stats()
            if (gate_encode(tt, tw, st, fx.kind, fx.w, fx.h, fx.log, TIER1_REL_GPU)) {
                device_leg_served(tt, "{fx.kind} {fx.w}x{fx.h}", s0, metal_tower_stats(), tw.n_layer, -1l)
            }
        }
        // the knob-off leg: the same tower, the CPU chain, no dispatch
        set_metal_tower(false)
        let sc0 = metal_tower_stats()
        tt |> gate_encode(tw, st, "cb", 336, 336, "e2b.encode.cb336.log")
        let sc1 = metal_tower_stats()
        tt |> equal(sc1.encodes, sc0.encodes, "knob-off leg never dispatched")
        tt |> equal(sc1.declines - sc0.declines, 1l, "knob-off leg recorded the knob decline")
        set_gpu_knobs(knobs_found)
        delete st
    } else {
        feint("das_metal is not built on this platform - the GPU rung is Apple-only\n")
    }
}

// The GPU rung's q8 lane, on the E2B file's `q8@dev` form of a das_metal build: the tower minted q8 in the Metal flavor
// serves every block off its q8 planes.
[cold_path]
def private gpu_q8_lane(tt : T?; twq : Gemma4vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            var stq = Gemma4vState()
            tt |> success(twq.q8, "the q8 lane minted under the pin")
            q8_lane_served(tt, "q8 tower", TowerFamily.vision, twq.n_layer) $() : bool {
                return gate_encode(tt, twq, stq, "cb", 336, 336, "e2b.encode.cb336.log", TIER1_REL_Q8)
            }
            delete stq
        }
    }
}

//! the dump whose bar the device poison of the tower file under the running block must exceed
var private g_poison_log = "e2b.encode.cb336.log"

// A canvas's device-side dump bar poisoned, on a `q8_z8@dev` form: the q8 tower with `POISON_BLOCK` zeroed must exceed the
// bar the cb 336 dump holds.
[cold_path]
def private g4v_poison_leg(tt : T?; twp : Gemma4vTower) {
    var o = OracleEncode()
    return if (!oracle_dump(tt, "gemma4-vision-oracle", g_poison_log, "its gemma4-vision-oracle mint script", o))
    var stp = Gemma4vState()
    dump_poison_leg(tt, "gemma4v", "cb", 336, 336, o, TIER1_REL_Q8, POISON_BLOCK, int(twp.proj_dim), TIER1_ABS, twp, stp) $(t; var s; planes; var out) => gemma4v_encode(t, s, planes, 336l, 336l, out)
    delete stp
}

//! the clamp knockout's miss on `tw`, a `+clamps_off` form: the largest v0 distance of its cb 336 encode from the mtmd dump
//! on the CPU chain; negative where the dump is not stocked (a loud skip)
def private knockout_miss(tt : T?; tw : Gemma4vTower) : float {
    var o = OracleEncode()
    let logpath = path_join(path_join(models_dir(), "gemma4-vision-oracle"), "e2b.encode.cb336.log")
    if (!load_oracle_encode(logpath, o)) {
        tt |> skip("oracle dump e2b.encode.cb336.log not present/parseable - mint via gemma4-vision-oracle/mint_e2b.sh")
        return -1.0
    }
    var maxdiff = 0.0
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var st = Gemma4vState()
        let planes <- make_planes("cb", 336, 336)
        var out : array<float>
        with_dasllama_jobque_() {
            gemma4v_encode(tw, st, planes, 336l, 336l, out)
        }
        for (p in range(o.npos)) {
            maxdiff = max(maxdiff, nan_wide_diff(out[p * int(tw.proj_dim)], o.v0[p]))
        }
        delete out
        delete st
    }
    return maxdiff
}

// The clamp knockout, on the `f32+clamps_off` form: the same tower with every block clamp disarmed must MISS the oracle -
// the sidecar scalars are load-bearing, not decoration (the E2B file's bounds sit where activations live).
[cold_path]
def private test_gemma4v_clamps_bind(tt : T?; tw : Gemma4vTower) {
    let maxdiff = knockout_miss(tt, tw)
    return if (maxdiff < 0.0)
    tt |> equal(g_clamps_armed, int(tw.n_layer * GEMMA4V_GEMMS), "every block GEMM carries an active clamp on E2B")
    to_log(LOG_INFO, "gemma4v clamp knockout cb 336x336: maxdiff {maxdiff} (floor {KNOCKOUT_FLOOR})\n")
    tt |> success(maxdiff > KNOCKOUT_FLOOR, "disarmed clamps miss the oracle by {maxdiff}")
}

// The same knockout on the q8 lane, on the `q8+clamps_off` form: the q8 GEMM route's inactive-clamp arm runs and misses too.
[cold_path]
def private clamps_bind_q8(tt : T?; twq : Gemma4vTower) {
    let maxdiffq = knockout_miss(tt, twq)
    return if (maxdiffq < 0.0)
    tt |> success(twq.q8 && maxdiffq > KNOCKOUT_FLOOR, "q8 lane: disarmed clamps miss the oracle by {maxdiffq}")
}

//! the standardize guard's three encodes of the cb 96 canvas, a form each: the file's tower, the neutral pair's, the biased
//! pair's
struct private StdKept {
    form : string
    plain : array<float>
    neutral : array<float>
    biased : array<float>
}

// The standardize guard's encode on one of its forms (`f32`, `f32+std_neutral`, `f32+std_biased`), the rows kept.
[cold_path]
def private std_guard_encode(tt : T?; tw : Gemma4vTower; var k : StdKept) {
    if (k.form == "f32") {
        tt |> success(!tw.has_std, "E2B carries no std pair - the arm is synthesized below")
    } else {
        tt |> success(tw.has_std, "arm {k.form == "f32+std_neutral" ? 0 : 1}: the served tower carries the std pair")
    }
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        let planes <- make_planes("cb", 96, 96)
        var s = Gemma4vState()
        with_dasllama_jobque_() {
            if (k.form == "f32") {
                gemma4v_encode(tw, s, planes, 96l, 96l, k.plain)
            } elif (k.form == "f32+std_neutral") {
                gemma4v_encode(tw, s, planes, 96l, 96l, k.neutral)
            } else {
                gemma4v_encode(tw, s, planes, 96l, 96l, k.biased)
            }
        }
        delete s
    }
}

// The standardize guard (std_bias / std_scale), after the block: absent on E2B, so the arm is reached through staging - a
// neutral pair (bias 0, scale 1) must leave the encode bit-identical, a biased pair must move it. The sign convention
// (x - bias) * scale is what the second half pins.
[cold_path]
def private test_gemma4v_std_guard(tt : T?; k : StdKept) {
    tt |> success(!empty(k.plain) && length(k.neutral) == length(k.plain) && length(k.biased) == length(k.plain), "the three forms kept their encodes")
    var neutral_diff = 0.0
    var biased_diff = 0.0
    for (p0, pn, pb in k.plain, k.neutral, k.biased) {
        neutral_diff = max(neutral_diff, nan_wide_diff(pn, p0))
        biased_diff = max(biased_diff, nan_wide_diff(pb, p0))
    }
    tt |> success(neutral_diff == 0.0, "bias 0 / scale 1 leaves the encode bit-identical (diff {neutral_diff})")
    tt |> success(biased_diff > 1e-2, "bias 0.25 / scale -1 moves the tokens (diff {biased_diff})")
}

// The q8 serving lane (the CPU default), on the E2B file's `image_q8` form: the same dumps on its own scale-relative bar.
// The exact-plane gate above is the parity rail; this one says what serving costs in fidelity.
[cold_path]
def private test_gemma4v_tier1_q8(tt : T?; via_carrier : VisionEmbedder) {
    let tw & = unsafe(via_carrier.gemma4v)
    tt |> equal(vision_exec_fmt(via_carrier), "q8", "the carrier stamps the q8 lane")
    with_gpu_knobs() {
        set_every_gpu_tower(false)   // a CPU-chain claim: the knob-off load took the lane's CPU image (`#tower-dev-block`)
        var st = Gemma4vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> success(length(tw.qblob) == tw.n_layer * tw.layer_w_stride, "qblob holds exactly the block GEMM set")
        tt |> gate_encode(tw, st, "cb", 96, 96, "e2b.encode.cb96.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "cb", 336, 336, "e2b.encode.cb336.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "green", 336, 336, "e2b.encode.green336.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "cb", 672, 336, "e2b.encode.cb672x336.log", TIER1_REL_Q8)
        delete st
    }
}

// The q8 lane's bar on the serving discrete home, on the E2B file's `q8@dev` form: the device runs the q8 chain's
// arithmetic with f16 weight scales and its own summation order.
[cold_path]
def private test_gemma4v_tier1_vulkan(tt : T?; tw : Gemma4vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(true)
        var st = Gemma4vState()
        let fixtures = [(kind = "cb", w = 96, h = 96, log = "e2b.encode.cb96.log"),
                        (kind = "cb", w = 336, h = 336, log = "e2b.encode.cb336.log"),
                        (kind = "green", w = 336, h = 336, log = "e2b.encode.green336.log"),
                        (kind = "cb", w = 672, h = 336, log = "e2b.encode.cb672x336.log")]
        for (fx in fixtures) {
            let s0 = gpu_tower_counters(TowerFamily.vision)
            let ran = gate_encode(tt, tw, st, fx.kind, fx.w, fx.h, fx.log, TIER1_REL_Q8)
            if (ran) {
                device_leg_served(tt, "{fx.kind} {fx.w}x{fx.h}", s0, gpu_tower_counters(TowerFamily.vision), tw.n_layer, -1l)
            }
        }
        delete st
    }
}

//! the discrete twin's canvases, depths and exact-lane leg: square canvases on one- and two-block towers - a drift names
//! its block before the deep chain compounds it, the poison on the one-block tower, where the bar is tightest - then
//! square, non-square and 128-aligned canvases on the whole tower
def private gemma4v_twin() : VisionTwin {
    var twin <- vision_twin("cb", 336, 336, "gemma4v cb 336x336", 441l)
    twin.canvases <- [twin_canvas(1l, "cb", 96, 96, 0l), twin_canvas(1l, "cb", 336, 336),
        twin_canvas(2l, "cb", 96, 96), twin_canvas(2l, "cb", 336, 336),
        twin_canvas(0l, "cb", 96, 96), twin_canvas(0l, "cb", 336, 336), twin_canvas(0l, "green", 672, 336),
        twin_canvas(0l, "cb", 768, 384)]   // 768 x 384: 1152 rows, a 128-multiple - the aligned tile variant's canvas
    twin.depths <- [TwinDepth(nl = 1l), TwinDepth(nl = 2l)]
    return <- twin
}

//! the forms only the discrete twin reads; with the whole exact form and the q8 one on the device (`f32`, `q8@dev`) they
//! are its legs, the forms of one staged read (`tower_stage_key`) side by side
let private TWIN_FORMS = fixed_array("f32_1L", "f32_2L", "f32@seat", "f32_z8@seat", "q8_1L", "q8_2L", "q8", "q8_1L@dev", "q8_1L_z0@dev", "q8_2L@dev")

// A form's legs of the discrete twin: its canvases three ways, a form a chain, and the exact lane served on the device.
[cold_path]
def private twin_legs(tt : T?; tw : Gemma4vTower; var twin : VisionTwin) {
    if (twin.form == "f32") {
        tt |> success(!tw.q8, "exact-plane lane minted")
    } elif (twin.form == "q8") {
        tt |> success(tw.q8, "q8 lane minted")
    }
    var st = Gemma4vState()
    twin_form_legs(tt, twin, tw, st) $(t; var s; planes; w; h; var out) => gemma4v_encode(t, s, planes, int64(w), int64(h), out)
    delete st
}

// The twin's compares, after the block, on the rows its forms kept.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private test_gemma4v_discrete_twin(tt : T?; twin : VisionTwin) {
    twin_held(tt, "gemma4v", "", 16l, twin)
}

// The E2B Q8_0 decoder's block, nested in the tower's `image` form: an image chat turn reads both files.
[cold_path]
def private e2b_q8_decoder(t : T?; var emb : VisionEmbedder) {
    unsafe {
        t |> run("the gemma-4 E2B Q8_0 decoder under the gemma4v tower") @capture(& emb)(t : T ?) {
            return if (!vision_chat_gate(t, "gemma4e", "gemma-4-E2B-it-Q8_0.gguf", 0))
            with_cpu_and_gpu_model(t, "vision", "gemma-4-E2B-it-Q8_0.gguf", VISION_CHAT_CTX, [cpu_cells_form(QuantMode.q8)]) $(var m; form) {
                vision_chat_cell(t, "the E2B image turn through the gemma4v tower: stream shape, then the caption", m, form, false, emb, @@test_vision_chat_e2b)
            }
        }
    }
}

// The E2B Q4_K_M decoder's block, nested in the tower's `image` form: an image chat turn reads both files.
[cold_path]
def private e2b_q4_decoder(t : T?; var emb : VisionEmbedder) {
    unsafe {
        t |> run("the gemma-4 E2B Q4_K_M decoder under the gemma4v tower") @capture(& emb)(t : T ?) {
            return if (!vision_chat_gate(t, "gemma4e", "gemma-4-E2B-it-Q4_K_M.gguf", 0))
            with_cpu_and_gpu_model(t, "vision", "gemma-4-E2B-it-Q4_K_M.gguf", VISION_CHAT_CTX, [cpu_cells_form(QuantMode.q8)]) $(var m; form) {
                vision_chat_cell(t, "the E2B image turn on the first turn under set_thinking(false) answers", m, form, false, emb, @@test_vision_chat_e2b_nothink)
            }
        }
    }
}

// The E2B mmproj, one block.
[test(description = "The gemma4v E2B tower's mmproj loads once a form. On the exact lane it reads its geometry and plane split off the mmproj and matches the mtmd encode dumps on eight canvases within 4e-3 of token rms, panicking on a grid past the position table or a ragged canvas. The lane policy is q8 with the Metal tower off and exact with it armed, and the Metal tower matches the dumps within 8e-3 of token rms on five canvases, with a knob-off decline leg and the q8 lane served with a red poison control. A knockout: every block GEMM carries an active clamp, and disarming them all moves the encode more than 1e-3 off the mtmd dump on both the exact and the q8 lane, so the clamp scalars are load-bearing. A synthesized std_bias / std_scale pair leaves the encode bit-identical at bias 0 and scale 1, and bias 0.25 with scale -1 moves the tokens by more than 1e-2. The q8 serving lane holds exactly the block GEMM set in its qblob and matches the dumps within 0.12 of token rms on four canvases, the vision carrier stamps q8 or f32 by lane, and on the serving discrete home's device the q8 tower matches four dumps within that bar, each device-served, while a mid-stack zeroed-block poison exceeds it. The serving discrete home's chain matches the CPU q8 chain on one- and two-block towers and the full tower across square, non-square and 128-aligned canvases, a zeroed-block poison reds, and the exact lane is served on the device. Under the mmproj's image form each E2B decoder loads once: through the Q8_0 decoder the mmproj sniffs as gemma4v at 48 px, a carrier with no vision arm is refused at create, the cats turn splices 130 rows between the image markers, and the greedy caption names the cats; the Q4_K_M decoder's turn with thinking off under the sampled preset ends on a stop before the budget, files any leading thought block as reasoning, and leaves a cats answer with no channel marker in the content and the history")]
def test_gemma4v_e2b(t : T?) {
    return if (!family_on(t, "gemma4v"))
    var inscope twin <- gemma4v_twin()
    var inscope stdk = StdKept()
    let twin_on = empty(twin_form_why(""))
    g_poison_log = "e2b.encode.cb336.log"
    var inscope forms <- ["image"]
    add_served_forms(t, forms, MM, ["q8@dev", "q8_z8@dev"], @@metal_or_twin_form_why)
    forms |> push_from(["image_f32", "image_q8", "q8+clamps_off", "f32+clamps_off", "f32", "f32+std_neutral", "f32+std_biased"])
    add_served_forms(t, forms, MM, [for (f in TWIN_FORMS); f], @@twin_form_why)
    let ran = with_model_once(t, "vision", "mmproj-gemma-4-E2B-it-bf16.gguf", forms, @@tower_stage_key, @@gemma4v_stage, @@gemma4v_form) $(var e; form) {
        tower_form_is(t, MM, form, e.gemma4v)
        twin.form = form
        stdk.form = form
        if (form == "image") {
            tower_cell(t, "tier-1 GPU attempt vs the mtmd encode dumps (scale-relative rung)", e.gemma4v, @@test_gemma4v_tier1_gpu)
            e2b_q8_decoder(t, e)
            e2b_q4_decoder(t, e)
        } elif (form == "image_f32") {
            tower_cell(t, "tier-1 tower parity vs the mtmd encode dumps (E2B)", e.gemma4v, @@test_gemma4v_tier1)
            tower_cell(t, "tier-1 on the q8 block-GEMM lane: the exact lane's carrier stamp", e, @@carrier_stamps_exact)
        } elif (form == "image_q8") {
            tower_cell(t, "tier-1 on the q8 block-GEMM lane (the serving default)", e, @@test_gemma4v_tier1_q8)
        } elif (form == "f32+clamps_off") {
            tower_cell(t, "disarming the block clamps moves tier-1 past the bar", e.gemma4v, @@test_gemma4v_clamps_bind)
        } elif (form == "q8+clamps_off") {
            tower_cell(t, "disarming the block clamps moves tier-1 past the bar, the q8 lane", e.gemma4v, @@clamps_bind_q8)
        } elif (form == "f32+std_neutral" || form == "f32+std_biased") {
            tower_cell(t, "synthesized std_bias/std_scale: the {form} form's encode", e.gemma4v, stdk, @@std_guard_encode)
        } elif (form == "q8_z8@dev") {
            tower_cell(t, "the q8 lane's dump bar: a zeroed block through the device chain reds it", e.gemma4v, @@g4v_poison_leg)
        } elif (form == "q8@dev" && !twin_on) {
            tower_cell(t, "tier-1 GPU attempt: the q8 lane on the Metal tower", e.gemma4v, @@gpu_q8_lane)
        } else {
            if (form == "f32") {
                tower_cell(t, "synthesized std_bias/std_scale: the file's own encode", e.gemma4v, stdk, @@std_guard_encode)
                if (empty(metal_build_why())) {
                    coverage_tower_gemma4v_metal(t,
                        "gemma4v E2B: the kernel census's canvas on the Metal tower",
                        e.gemma4v)
                }
            } elif (form == "q8@dev") {
                tower_cell(t, "tier-1 attempt on the serving discrete home's tower ({serving_tower_dev(TowerFamily.vision)}) vs the mtmd encode dumps (the q8 lane's bar)", e.gemma4v, @@test_gemma4v_tier1_vulkan)
            }
            if (twin_on) {
                tower_cell(t, "the serving discrete home's chain, the {form} form's legs", e.gemma4v, twin, @@twin_legs)
            }
        }
    }
    if (ran) {
        kept_cell(t, "synthesized std_bias/std_scale: neutral is identity, a bias moves the tokens", stdk, @@test_gemma4v_std_guard)
    }
    if (ran && twin_on) {
        kept_cell(t, "the serving discrete home's chain ({serving_tower_dev(TowerFamily.vision)}) vs the CPU q8 chain, and the exact lane served", twin, @@test_gemma4v_discrete_twin)
        gpu_tower_shutdown(TowerFamily.vision)
    }
}

// The E4B rung, on the E4B file's `image_f32` form: the larger E-series vehicle shares the tower geometry block for block -
// only the soft-token width differs (2560 vs 1536) - so the dump set is the seam subset (smallest grid, one product size,
// the orientation gate; mint_e4b.sh), with one GPU and one q8-lane fixture on their own forms.
[cold_path]
def private test_gemma4v_tier1_e4b(tt : T?; tw : Gemma4vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var st = Gemma4vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.proj_dim), 2560, "soft-token width - the one dimension E4B changes")
        tt |> equal(int(tw.n_embd), 768, "tower width matches E2B's")
        tt |> equal(int(tw.n_layer), 16, "block count matches E2B's")
        tt |> success(tw.blk_bf16 && tw.proj_bf16 && !tw.patch_bf16, "plane split as the file has it: bf16 GEMMs, f32 patch conv")
        tt |> success(!tw.has_std, "E4B carries no std_bias/std_scale")
        tt |> gate_encode(tw, st, "cb", 96, 96, "e4b.encode.cb96.log")
        tt |> gate_encode(tw, st, "cb", 336, 336, "e4b.encode.cb336.log")
        tt |> gate_encode(tw, st, "green", 336, 336, "e4b.encode.green336.log")
        tt |> gate_encode(tw, st, "cb", 672, 336, "e4b.encode.cb672x336.log")
        delete st
    }
}

// The E4B rung on the Metal tower, on the E4B file's `image` form.
[cold_path]
def private e4b_gpu_leg(tt : T?; tw : Gemma4vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            var st = Gemma4vState()
            let s0 = metal_tower_stats()
            let ran = gate_encode(tt, tw, st, "cb", 336, 336, "e4b.encode.cb336.log", TIER1_REL_GPU)
            if (ran) {
                device_leg_served(tt, "GPU", s0, metal_tower_stats(), tw.n_layer, -1l)
            }
            delete st
        }
    }
}

// The E4B serving lane on its own bar, on the E4B file's `q8` form - minted in memory (a load under the pin bakes a q8 image).
[cold_path]
def private e4b_q8_lane(tt : T?; twq : Gemma4vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var stq = Gemma4vState()
        tt |> success(twq.q8, "q8 lane loaded")
        tt |> gate_encode(twq, stq, "cb", 336, 336, "e4b.encode.cb336.log", TIER1_REL_Q8)
        delete stq
    }
}

// The E4B q8 lane's device leg on the serving discrete home, on the E4B file's `q8@dev` form.
[cold_path]
def private e4b_q8_device_leg(tt : T?; twv : Gemma4vTower) {
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.vision)
        var stv = Gemma4vState()
        let v0 = gpu_tower_counters(TowerFamily.vision)
        let ran = gate_encode(tt, twv, stv, "cb", 336, 336, "e4b.encode.cb336.log", TIER1_REL_Q8)
        if (ran) {
            device_leg_served(tt, "E4B q8", v0, gpu_tower_counters(TowerFamily.vision), twv.n_layer, -1l)
        }
        delete stv
    }
}

// The E4B mmproj, one block.
[test(description = "The gemma4v E4B tower's mmproj loads once a form: it shares E2B's geometry but a 2560 soft-token width and matches its mtmd dumps on the exact lane within 4e-3 of token rms, on Metal within 8e-3, and on the q8 lane and its discrete device rung within 0.12, a mid-stack zeroed-block poison exceeding that bar on the device")]
def test_gemma4v_e4b(t : T?) {
    return if (!family_on(t, "gemma4v"))
    g_poison_log = "e4b.encode.cb336.log"
    var inscope forms <- ["image_f32"]
    add_served_forms(t, forms, MM_E4B, ["image"], @@metal_form_why)
    forms |> push("q8")
    add_served_forms(t, forms, MM_E4B, ["q8@dev", "q8_z8@dev"], @@discrete_rung_form_why)
    with_model_once(t, "vision", "mmproj-gemma-4-E4B-it-BF16.gguf", forms, @@tower_stage_key, @@gemma4v_stage, @@gemma4v_form) $(var e; form) {
        tower_form_is(t, MM_E4B, form, e.gemma4v)
        if (form == "image_f32") {
            tower_cell(t, "tier-1 tower parity vs the mtmd encode dumps (E4B)", e.gemma4v, @@test_gemma4v_tier1_e4b)
        } elif (form == "image") {
            tower_cell(t, "tier-1 on the Metal tower (E4B)", e.gemma4v, @@e4b_gpu_leg)
        } elif (form == "q8") {
            tower_cell(t, "tier-1 on the q8 lane (E4B)", e.gemma4v, @@e4b_q8_lane)
        } elif (form == "q8@dev") {
            tower_cell(t, "tier-1 on the q8 lane's device rung (E4B)", e.gemma4v, @@e4b_q8_device_leg)
        } elif (form == "q8_z8@dev") {
            tower_cell(t, "the q8 lane's dump bar: a zeroed block through the device chain reds it (E4B)", e.gemma4v, @@g4v_poison_leg)
        }
    }
}
