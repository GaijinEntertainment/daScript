options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_qwen25v
require dasllama/dasllama_vision_embedder   // the carrier the mmproj's forms load as
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require strings
require _model_tier   // models_dir() + model_available() gate
require _model_once   // with_model_once: the mmproj's one block in the area
require ?das_metal dasllama/dasllama_metal_tower   // the CPU gates pin the tower knob off
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown - the twin-W leg re-arms crowned PSOs
require ?vulkan dasllama/dasllama_vulkan_tower
require dasllama/dasllama_common   // set_metal_tensor_crowns - the twin-W leg's crown pin
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require math   // nolint:STYLE030,LINT019 — used only inside the GPU static_if arms; the no-GPU lane sees it unused
require _tower_twin   // set_every_gpu_tower, the Vulkan twin's seat and instrument
require _vision_forms   // the towers' forms, a block's cell
require _cpu_gpu_model   // with_cpu_and_gpu_model: a chat decoder's one block in the area
require _vision_chat_cells   // the image chat turn's cells, run under the tower's image form

// Tier-1 qwen25v tower parity vs llama-mtmd-debug -p encode (the Qwen2.5-Omni window ViT).
// Reference arm: the f32-widened mmproj, CPU, -fa off (qwen3vl-vision-oracle/mint_25o.sh).
// Fixtures: pre-resized [0,1] float canvases - no preprocessing, so a diff isolates the tower
// math; cb448 = four full windows, cb112 = the single-window arm, cb616x336 = ragged windows.
// The mmproj loads once a form, in its one `with_model_once` block (`qwen25v_form`): the image rail's forms carry the CPU
// gate and the Metal tower's cells, the forms minted in memory the poison, the one-block routing towers and the discrete
// twin's depths. A form's cells run on one home, so the exact CPU chain and the device chain of one tower are two forms
// (the device's `@seat`), compared after the block on the rows they kept.
// The twin runs on the serving discrete home: under the CUDA ask (DASLLAMA_CUDA=1 or set_gpu_home("cuda")) on the CUDA
// tower, its forms skipping without a CUDA device there (`cuda_twin_seat_why`); else on the Vulkan tower, skipping on a
// build with das_metal - the Metal driver owns the tower hooks there -, without a Vulkan device under DASLLAMA_GPU=1 and
// without the tower's tile family (`vulkan_twin_seat_why`).

let private MM = "mmproj-Qwen2.5-Omni-3B-f16.gguf"

let private TIER1_ABS = 2e-4
// x token rms; one GELU-LUT chain (the merger) rides the qwen3v Omni floor
let private TIER1_REL = 1e-2
//! measured up to ~4e-2 rel on this 32-layer mix: 2x headroom, and a structural break
//! (zeroed-block class, >= 0.9) still reds
let private TIER1_REL_GPU = 8e-2

let private TWIN_REL_VK_SHALLOW = 1e-2
let private TWIN_REL_VK_DEEP = 0.15

let private SHALLOW_ABS_BAR = 0.1   //! one layer of GPU noise measures ~2.6e-2 abs peak on quad (f16-operand GEMMs + the K=5120 merger on |~70| values), a routing defect mixes quadrants at >= 1.0
let private WA_WINDOW_ONLY = 99l   //! the one-block tower's full-attention period: (0 + 1) % 99 != 0 puts the block on the window arm
let private WA_FULL_ONLY = 1l      //! the one-block tower's full-attention period: (0 + 1) % 1 == 0 puts the block on the full-attention arm

//! the file's staged read, the one every minted form's copy comes from
[unused_argument(key)]
def private qwen25v_stage(path, key : string) : Qwen25vStaging => <- stage_qwen25v_tower(path)

//! One form of the Qwen2.5-Omni window ViT's file as the vision carrier holds it, its block's form loader (`tower_form`; the
//! tower has one lane, the file's planes): `image` and `image_f32` are the carrier's own load through the image rail - the
//! copy the serving tower's cells read and the copy the CPU chain's read; any other is minted in memory from `stg`, its
//! own copy of the file's staging - never `load_qwen25v_tower`, which bakes a `.dlim` - cut to the form's blocks, the form's block zeroed
//! through both staging planes (the baked halfword twin serves the driver, so a blob-only zero is laundered), and under
//! `+wa<n>` with the full-attention period `n` (the shallow routing cells).
def private qwen25v_form(path, form : string; var stg : Qwen25vStaging) : VisionEmbedder {
    let f = tower_form(form)
    return <- load_vision_embedder(path) if (f.image)
    if (f.lane != "f32" || f.dev) {
        panic("the qwen25v tests: '{form}' names a lane or an image flavor the tower does not have")
    }
    var inscope e = VisionEmbedder(kind = VisionKind.qwen25v)
    if (f.edit |> starts_with("wa")) {
        stg.t.n_wa_pattern = to_int64(slice(f.edit, 2), false)
    } elif (!empty(f.edit)) {
        panic("the qwen25v tests: '{form}' names no edit of the staged planes")
    }
    truncate_and_poison(stg, f.nl, f.zero_block)
    mint_qwen25v_tower(stg, e.qwen25v)
    return <- e
}

//! The form is the depth and routing it names: the carrier holds the qwen25v tower, on a minted form cut to the form's
//! blocks and, under `+wa<n>`, on that full-attention period. Nothing on a loaded tower says which home's cells read it,
//! nor which block is zeroed.
def private qwen25v_form_is(t : T?; e : VisionEmbedder; form : string) {
    let f = tower_form(form)
    t |> success(e.kind == VisionKind.qwen25v, "{MM}: the {form} form is the qwen25v tower")
    if (!f.image) {
        t |> success(f.nl == 0l || e.qwen25v.n_layer == f.nl, "{MM}: the {form} form holds the blocks it names ({e.qwen25v.n_layer})")
        t |> success(empty(f.edit) || e.qwen25v.n_wa_pattern == to_int64(slice(f.edit, 2), false), "{MM}: the {form} form routes on the period it names ({e.qwen25v.n_wa_pattern})")
    }
}

[cold_path]
def private gate_encode(t : T?; tw : Qwen25vTower; var st : Qwen25vState; kind : string; w, h : int; logname : string; rel = TIER1_REL) : bool {
    return oracle_gate(t, path_join(models_dir(), "qwen3vl-vision-oracle"), logname, "qwen3vl-vision-oracle/mint_25o.sh", kind, w, h,
            "qwen25v tier-1 {kind} {w}x{h}", int(tw.proj_dim), TIER1_ABS, rel) $(planes : array<float>; var out : array<float>) : int64 {
        return qwen25v_encode(tw, st, planes, int64(w), int64(h), out)
    }
}

// The CPU gate, on the `image_f32` form.
[cold_path]
def private test_qwen25v_tier1(tt : T?; tw : Qwen25vTower) {
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        var st = Qwen25vState()
        tt |> equal(int(tw.patch), 14, "patch")
        tt |> equal(int(tw.merge), 2, "spatial merge")
        tt |> equal(int(tw.n_embd), 1280, "tower width")
        tt |> equal(int(tw.n_layer), 32, "blocks")
        tt |> equal(int(tw.n_head), 16, "heads")
        tt |> equal(int(tw.n_ff), 3420, "gated hidden (from the tensor dims - the metadata lies)")
        tt |> equal(int(tw.proj_dim), 2048, "soft-token width")
        tt |> equal(int(tw.n_wa_pattern), 8, "full attention every 8th block")
        tt |> equal(int(tw.win_px), 112, "attention window")
        tt |> gate_encode(tw, st, "cb", 112, 112, "encode25.cb112.log")   // 16 tokens: window == canvas
        tt |> gate_encode(tw, st, "gray", 448, 448, "encode25.gray448.log")
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode25.cb448.log")   // four full 4x4-group windows
        tt |> gate_encode(tw, st, "cb", 616, 336, "encode25.cb616x336.log")   // ragged window edges + transposed-grid
        tt |> gate_encode(tw, st, "quad", 448, 448, "encode25.quad448.log")   // WINDOW discriminator: uniform/periodic fixtures hide a broken mask
        var ragged_tripped = false
        try {
            var planes : array<float>
            planes |> reserve(42l * 28l * 3l)
            planes |> resize(42l * 28l * 3l)
            var out : array<float>
            qwen25v_encode(tw, st, planes, 42l, 28l, out)   // 42 is not a multiple of the 28 px merged patch
            delete out
            delete planes
        } recover {
            ragged_tripped = true
        }
        tt |> success(ragged_tripped, "a canvas off the 28 px merged-patch grid panics by name")
        delete st
    }
}

// The Metal rung, on the `image` form: the dumps through the device-served tower, the f16 twin route, the knob-off leg.
[cold_path]
def private test_qwen25v_tier1_gpu(tt : T?; tw : Qwen25vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knobs_found = gpu_knobs()
        let f16_found = get_metal_tower_f16()
        let crowns_found = get_metal_tensor_crowns()
        set_metal_tower(true)
        var st = Qwen25vState()
        let fixtures = [(kind = "cb", w = 112, h = 112, log = "encode25.cb112.log"),
                        (kind = "gray", w = 448, h = 448, log = "encode25.gray448.log"),
                        (kind = "cb", w = 448, h = 448, log = "encode25.cb448.log"),
                        (kind = "cb", w = 616, h = 336, log = "encode25.cb616x336.log")]
        for (fx in fixtures) {
            let s0 = metal_tower_stats()
            let ran = gate_encode(tt, tw, st, fx.kind, fx.w, fx.h, fx.log, TIER1_REL_GPU)
            if (ran) {
                device_leg_served(tt, "{fx.kind} {fx.w}x{fx.h}", s0, metal_tower_stats(), tw.n_layer, -1l)
            }
        }
        set_metal_tensor_crowns("mulmm_q8")
        metal_prefill_shutdown()
        set_metal_tower_f16(true)   // the twin route's own knob - established, never inherited
        let tw0 = metal_tower_f16_encodes()
        if (gate_encode(tt, tw, st, "cb", 448, 448, "encode25.cb448.log", TIER1_REL_GPU)) {
            tt |> success(metal_tower_f16_encodes() - tw0 >= 1l, "the f16-sourced tower rode the twin route")
        }
        set_metal_tower_f16(f16_found)
        set_metal_tensor_crowns(crowns_found)
        metal_prefill_shutdown()
        set_metal_tower(false)
        let sc0 = metal_tower_stats()
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode25.cb448.log")
        let sc1 = metal_tower_stats()
        tt |> equal(sc1.encodes, sc0.encodes, "knob-off leg never dispatched")
        tt |> equal(sc1.declines - sc0.declines, 1l, "knob-off leg recorded the knob decline")
        set_gpu_knobs(knobs_found)
        delete st
    } else {
        tt |> skip("no das_metal on this build - the GPU rung has no device to serve")
    }
}

// The Metal rung's control, on the `f32_z16@seat` form: block 16 zeroed through both staging planes, device-served, must
// exceed the GPU bar the cb 448 dump holds.
[cold_path]
def private gpu_poison_leg(tt : T?; twp : Qwen25vTower) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            var stq = Qwen25vState()
            var o = OracleEncode()
            if (load_oracle_encode(path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), "encode25.cb448.log"), o)) {
                let planes <- make_planes("cb", 448, 448)
                var out : array<float>
                with_dasllama_jobque_() {
                    qwen25v_encode(twp, stq, planes, 448l, 448l, out)
                }
                let ex = encode_excess(out, int(twp.proj_dim), o, TIER1_ABS, TIER1_REL_GPU)
                tt |> success(ex > 0.0, "zeroed-block poison EXCEEDS the GPU bar (excess {ex})")
                delete out
            }
            delete stq
        }
    }
}

//! a one-block routing tower's three encodes of the quad canvas, a form each: the CPU chain's, the device chain's and - on
//! the discrete home - the zeroed block's on the device, with the device encodes each leg counted
struct private RoutingKept {
    name : string
    wa : int64
    poison : bool
    oc : array<float>
    og : array<float>
    op : array<float>
    nc : int64
    ng : int64
    served : int64
    poison_served : int64
}

//! what the mmproj's forms keep for the compares after its block: the twin's canvases, the routing towers, the form whose
//! leg runs, and the whole tower's full-attention period
struct private Qwen25vKept {
    form : string
    canvases : array<TwinKept>
    routing : array<RoutingKept>
    wa_pattern : int64
}

// A routing tower's leg on its form (`f32_1L+wa<n>` the CPU chain, `@seat` the device chain, `_z0@seat` the zeroed block
// on the device): the quad canvas through the one-block tower, the rows kept.
[cold_path]
def private routing_leg(tw : Qwen25vTower; var kept : Qwen25vKept) {
    let f = tower_form(kept.form)
    let fam = TowerFamily.vision
    let planes <- make_planes("quad", 448, 448)
    for (r in kept.routing) {
        continue if (r.wa != tw.n_wa_pattern)
        var st = Qwen25vState()
        with_gpu_knobs() {
            set_serving_gpu_tower(f.seat, fam)
            let e0 = gpu_tower_counters(fam).encodes
            var inscope rows : array<float>
            var n = 0l
            with_dasllama_jobque_() {
                n = qwen25v_encode(tw, st, planes, 448l, 448l, rows)
            }
            let served = gpu_tower_counters(fam).encodes - e0
            if (f.zero_block >= 0l) {
                r.op <- rows
                r.poison_served = served
            } elif (f.seat) {
                r.og <- rows
                r.ng = n
                r.served = served
            } else {
                r.oc <- rows
                r.nc = n
            }
        }
        delete st
    }
}

//! the one-block `backend` chain against the CPU chain on quad within `SHALLOW_ABS_BAR`, after the block; where the tower
//! takes the poison, a zeroed block must EXCEED it
def private shallow_routing_held(t : T?; backend : string; r : RoutingKept) {
    let name = r.name
    let kept = !empty(r.oc) && !empty(r.og) && (!r.poison || !empty(r.op))
    t |> success(kept, "shallow {name}: its forms kept their rows")
    return if (!kept)
    t |> equal(r.served, 1l, "shallow {name}: {backend}-served")
    t |> equal(int(r.ng), int(r.nc), "shallow {name}: token counts agree")
    let md = logits_maxdiff(r.og, r.oc)
    to_log(LOG_INFO, "qwen25v shallow {name} on the driver: {backend} vs CPU on quad maxdiff {md} (bar {SHALLOW_ABS_BAR} abs)\n")
    t |> success(md < SHALLOW_ABS_BAR, "shallow {name}: {backend} == CPU on quad within {SHALLOW_ABS_BAR} abs (measured {md})")
    if (r.poison) {
        t |> equal(r.poison_served, 1l, "shallow {name} poison: {backend}-served")
        let mdp = logits_maxdiff(r.op, r.oc)
        to_log(LOG_INFO, "qwen25v shallow {name} poison: the zeroed block on the {backend} vs the clean CPU chain maxdiff {mdp}\n")
        t |> success(mdp >= SHALLOW_ABS_BAR, "shallow {name}: a zeroed block through the {backend} chain EXCEEDS the {SHALLOW_ABS_BAR} abs bar (measured {mdp})")
    }
}

// The Metal rung's one-block window and full routing, after the block: the Metal chain against the CPU chain on quad.
[cold_path]
def private gpu_routing(tt : T?; kept : Qwen25vKept) {
    for (r in kept.routing) {
        shallow_routing_held(tt, "GPU", r)
    }
}

// A form's leg of the discrete twin on the canvases of its depth: the exact CPU chain with the tower knob off, the device
// chain on the `@seat` copy, the input poison on the zeroed `@seat` copy.
[cold_path]
def private twin_legs(tt : T?; tw : Qwen25vTower; var kept : Qwen25vKept) {
    let f = tower_form(kept.form)
    if (f.nl == 0l) {
        kept.wa_pattern = tw.n_wa_pattern
        tt |> success(tw.wblob.n > 0l && !tw.blk_bf16, "the halfword twin is baked from an f16 file: the driver's lane")
    }
    var st = Qwen25vState()
    twin_keep_legs(tt, kept.canvases, f.nl, tw.n_layer, true, f.seat, f.zero_block, tw, st) $(t; var s; planes; w; h; var out) => qwen25v_encode(t, s, planes, int64(w), int64(h), out)
    delete st
}

//! one canvas two ways - the exact CPU chain and the device chain - held after the block: the counters and the bar, and
//! where the canvas takes the poison the device chain over the zeroed block must EXCEED the bar the clean chains set
def private twin_canvas_held(tt : T?; k : TwinKept; wa_pattern : int64) {
    let tag = twin_tag(k)
    let legs = k.zero_block >= 0l ? 3 : 2
    tt |> equal(k.legs, legs, "{tag}: the {k.nl}-block canvas kept its {legs} legs")
    return if (k.legs != legs)
    twin_counted(tt, tag, k.n_cpu, k.n_dev, k.cpu0, k.cpu1, k.dev0, k.dev1, k.n_layer, int64(k.w / 14) * int64(k.h / 14))
    let rms = rms(k.cpu)
    let dev_ex = logits_maxdiff(k.dev, k.cpu)
    let rel = k.n_layer <= 8l ? TWIN_REL_VK_SHALLOW : TWIN_REL_VK_DEEP
    let bar = TWIN_ABS_VK + rel * rms
    // the error's shape: NaNs the maxdiff cannot see, the worst token, the tokens past the bar
    var nans = 0
    var worst_tok = -1
    var worst = 0.0
    var over = 0
    let pd = length(k.cpu) / int(k.n_cpu)
    for (tok in range(length(k.cpu) / pd)) {
        var m = 0.0
        for (i in range(pd)) {
            let a = k.dev[tok * pd + i]
            if (is_nan(a)) {
                nans++
            }
            m = max(m, nan_wide_diff(a, k.cpu[tok * pd + i]))
        }
        if (m > bar) {
            over++
        }
        if (m > worst) {
            worst = m
            worst_tok = tok
        }
    }
    to_log(LOG_INFO, "qwen25v {serving_tower_dev(TowerFamily.vision)} twin {tag} ({k.n_layer} blocks, full every {wa_pattern}): rms {rms}; device vs exact {dev_ex} ({dev_ex / rms} x rms); NaNs {nans}, worst token {worst_tok} of {length(k.cpu) / pd}, {over} tokens past the bar\n")
    tt |> success(dev_ex <= bar, "{tag}: the device chain holds to the exact chain within {rel} x rms ({dev_ex / rms} x rms)")
    if (k.zero_block >= 0l) {
        let spec = TwinSpec(family = "qwen25v", tag = tag, n_layer = k.n_layer, metric = TwinMetric.maxdiff, seat_family = TowerFamily.vision)
        twin_poison_held(tt, spec, rms, double(bar), k.zero_block, k.cpu, k.poisoned)
    }
}

// The twin's compares, after the block, on the rows its forms kept, and the one-block window and full routing with their
// poisons.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private test_qwen25v_discrete_twin(tt : T?; kept : Qwen25vKept) {
    for (k in kept.canvases) {
        twin_canvas_held(tt, k, kept.wa_pattern)
    }
    for (r in kept.routing) {
        shallow_routing_held(tt, "device", r)
    }
}

//! the twin's canvases - the shallow twins first, on the tight bar: a drift names its depth before the deep mix re-rolls it,
//! the poison on the one-block tower, where the bar is tightest - and the two routing towers, `poison` whether they take
//! the zeroed block
def private qwen25v_kept(poison : bool) : Qwen25vKept {
    return <- Qwen25vKept(
        canvases <- [twin_canvas(1l, "cb", 448, 448, 0l), twin_canvas(1l, "cb", 616, 336),
            twin_canvas(2l, "cb", 448, 448), twin_canvas(2l, "cb", 616, 336),
            twin_canvas(8l, "cb", 448, 448), twin_canvas(8l, "cb", 616, 336),
            twin_canvas(0l, "cb", 112, 112), twin_canvas(0l, "cb", 448, 448), twin_canvas(0l, "cb", 616, 336)],
        routing <- [RoutingKept(name = "window", wa = WA_WINDOW_ONLY, poison = poison), RoutingKept(name = "full", wa = WA_FULL_ONLY, poison = poison)])
}

//! the one-block routing towers' forms the Metal rung and the discrete twin both read, the poisons only the twin reads,
//! and the depths only the twin reads
let private ROUTING_FORMS = fixed_array("f32_1L+wa99", "f32_1L@seat+wa99", "f32_1L+wa1", "f32_1L@seat+wa1")
let private ROUTING_POISON_FORMS = fixed_array("f32_1L_z0@seat+wa99", "f32_1L_z0@seat+wa1")
let private TWIN_FORMS = fixed_array("f32_1L", "f32_1L@seat", "f32_1L_z0@seat", "f32_2L", "f32_2L@seat", "f32_8L", "f32_8L@seat", "f32", "f32@seat")

// The Qwen2.5-Omni-3B decoder's block - its cached load and its uncached one, nested in the tower's `image` form: an image chat turn reads both files.
[cold_path]
def private qwen25o_decoder(t : T?; var emb : VisionEmbedder) {
    unsafe {
        t |> run("the Qwen2.5-Omni-3B decoder under the qwen25v tower") @capture(& emb)(t : T ?) {
            return if (!vision_chat_gate(t, "qwen2", "Qwen2.5-Omni-3B-Q8_0.gguf", 0))
            with_cpu_and_gpu_model(t, "vision", "Qwen2.5-Omni-3B-Q8_0.gguf", VISION_CHAT_CTX, [cpu_cells_form(QuantMode.q8), cpu_cells_form(QuantMode.q8, true)]) $(var m; form) {
                vision_chat_cell(t, "the Qwen2.5-Omni image turn: window ViT + the NON-interleaved MROPE decoder, then the caption", m, form, false, emb, @@test_vision_chat_qwen25o)
                vision_chat_cell(t, "a plain chat renders the Omni image span via the vocab's own spelling", m, form, true, emb, @@test_render_image_span_qwen25o)
            }
        }
    }
}

// The Qwen2.5-Omni mmproj, one block.
[test(description = "The Qwen2.5-Omni-3B window-ViT tower's mmproj loads once a form. It loads its pinned shape and its CPU encode matches the llama-mtmd dumps on five canvases within 2e-4 abs plus 1e-2 of token rms, and an off-grid canvas panics. On Metal, the device-served tower matches the mtmd dumps within 8e-2 of token rms on four canvases and via the f16 twin route, the knob-off leg never dispatches, a zeroed-block poison exceeds the bar, and one-block window and full routing hold 0.1 abs against the CPU chain. On the serving discrete home, the device block loop holds to the exact CPU chain within a depth-scaled rms bar at 1, 2, 8 and 32 blocks, one-block window and full routing hold 0.1 abs, and zeroed-block poisons exceed the bars. Under the mmproj's image form the Qwen2.5-Omni-3B decoder loads once a form: the turn loads contiguous qwen2vl MROPE sections, sniffs qwen25v at 2048 wide, splices 391 rows on a 23x17 grid, captions the cats, and sets the rope delta to 23 - 391; a plain chat with no embedder renders an image span whose head closes on the vocab's own vision_bos marker id")]
def test_qwen25v_tower(t : T?) {
    return if (!family_on(t, "qwen25v"))
    let twin_on = empty(twin_form_why(""))
    let metal_on = empty(metal_form_why(""))
    var inscope kept <- qwen25v_kept(twin_on)
    var inscope forms <- ["image", "image_f32"]
    add_served_forms(t, forms, MM, ["f32_z16@seat"], @@metal_form_why)
    add_served_forms(t, forms, MM, [for (f in ROUTING_FORMS); f], @@metal_or_twin_form_why)
    add_served_forms(t, forms, MM, [for (f in ROUTING_POISON_FORMS); f], @@twin_form_why)
    add_served_forms(t, forms, MM, [for (f in TWIN_FORMS); f], @@twin_form_why)
    let ran = with_model_once(t, "vision", "mmproj-Qwen2.5-Omni-3B-f16.gguf", forms, @@tower_stage_key, @@qwen25v_stage, @@qwen25v_form) $(var e; form) {
        qwen25v_form_is(t, e, form)
        kept.form = form
        if (form == "image") {
            tower_cell(t, "tier-1 GPU attempt vs the mtmd encode dumps (scale-relative rung)", e.qwen25v, @@test_qwen25v_tier1_gpu)
            qwen25o_decoder(t, e)
        } elif (form == "image_f32") {
            tower_cell(t, "tier-1 window-ViT parity vs the mtmd encode dumps (Qwen2.5-Omni-3B)", e.qwen25v, @@test_qwen25v_tier1)
        } elif (form == "f32_z16@seat") {
            tower_cell(t, "tier-1 GPU attempt: a zeroed block through the device chain reds the bar", e.qwen25v, @@gpu_poison_leg)
        } elif (find(form, "+wa") >= 0) {
            routing_leg(e.qwen25v, kept)
        } else {
            tower_cell(t, "the serving discrete home's block loop, the {form} form's legs", e.qwen25v, kept, @@twin_legs)
        }
    }
    if (ran && metal_on) {
        kept_cell(t, "tier-1 GPU attempt: the one-block window and full routing", kept, @@gpu_routing)
    }
    if (ran && twin_on) {
        kept_cell(t, "the serving discrete home's block loop ({serving_tower_dev(TowerFamily.vision)}) vs the exact CPU chain, and the window routing", kept, @@test_qwen25v_discrete_twin)
        gpu_tower_shutdown(TowerFamily.vision)
    }
}
