options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _moe_resident
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_common   // kv_codec_for: the serving rule the biased twin's tq4 ask meets
require dasllama/dasllama   // moe_gpu_drop_model
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2moe)
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_gpu_tier   // moe_gpu_shexp_on_gpu / set_moe_gpu_shexp_route / set_gpu_resident_route
require _gpu_tier_armed   // gpu_tier_armed: the armed-tier skip
require dasllama/dasllama_blocks   // shexp_gpu_prefill_layers: the device arm's engage witness
require _model_tier   // models_dir() + model_available() presence/size-tier gate, the logits compares
require _resident_feed   // feed_logits, the per-step bar, Held
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows
require _resident_regions   // the regions rig's batched-step collections, REGIONS_RIG_CTX
require _kv_codec_cells   // codec_env_why: a codec form the environment keeps off
require _format_census   // census_cell: the per-32 cuts' every site in its own format
require dasllama/dasllama_env   // g_env_gpu.gpu_heat: the environment's slot ask, which shadows the pool's
require dasllama/dasllama_core   // gpu_device_session_ctx: the context the pool's plan armed at
require dasllama/dasllama_cuda_decode   // cud_rdec_hot_stats: the pool's hit chains; set_cuda_mmq_pin: the 35B cells' MMQ route
require dasllama/dasllama_cuda_stream   // cud_stream_stats, cud_stream_dma_fills: the pool's witnesses
require _cuda_feed   // cuda_forced_feed, stream_witness: the CUDA home's own cells
require dasllama/dasllama_moe   // shexp_gpu_decode_rows: the 35B per-op form's shared-expert rows
require _segment_cells   // recurrent_segments_cell, SEG_WINDOW: the 35B cut's segmented windows
require _parity_cells   // parity_qwen2moe_cut_cell, parity_qwen35moe_cut_cell: the cuts' parity streams on their pinned CPU forms; the 30B's parity row
require _batch_decode_cells   // batch_decode_hybrid_moe_cell: the 35B UD-Q4_K_M cut's portable-pinned CPU form
require _metal_batched_cells   // metal_qwen_moe_cells: the Qwen1.5 Q8_0 cut's Metal form's cells

// The whole-model resident driver on a MoE under DASLLAMA_GPU=1, on the Vulkan home or under DASLLAMA_CUDA=1 on the CUDA home:
// the expert stacks in the arena beside the attention quads and the shared experts, the window chain's routed block (the
// router GEMM, the per-row select, the device bucket schedule, the f16 gather, the expert tiles, the gated combine) and the
// token command's routed block (the router GEMV, the top-k writing the expert GEMVs' slot regions, the combine), in
// _moe_resident's forced-feed form.
// Qwen1.5-MoE-A2.7B's four-block cuts (`Qwen1.5-MoE-A2.7B-4L-*-local.gguf`, family `qwen2moe`; a cut writes no confident
// text, so its rows are held to the CPU chain's, never to an argmax): each file is ONE block (`with_cpu_and_gpu_model`), its
// forms loaded once each and the CPU form once a mirror codec, the cells collecting what the compares after the block read.
// - The Q4_K_M cut (`test_gpu_resident_moe`): the default form's feeds over one window and two, the routed census growing by
//   the routed layer count; a q8_0 mirror's feed against the CPU chain on q8_0 sessions; a tq4 ask served at q8_0 by the
//   serving rule (the cut's Q/K/V bias); two of its four routed layers' experts asked onto the host with the prompt stream
//   on and off - the pinned feeds over one window and two, the streamed windows against the host's sums at the stream twin's
//   bar, the device claiming first; under DASLLAMA_CUDA=1 the CUDA driver's feeds over one window and two, its host cells
//   (the meet handing the host its eight rarest, the stream-off control) and the hot pool's cells (an eight-slot ask at an
//   8192 context: the DMA fill and the decode meet, both off as the control, an all-device load arming no pool, the prompt
//   stream off).
// - The Q8_0 cut (`test_gpu_resident_moe_q8`): the same two host-layers forms; two mirror regions, the batched-step cells bit
//   for bit (the N-row command's plain act and the shared expert's Q8_0 requant); under DASLLAMA_CUDA=1 the default form's
//   one-window feed; on its CPU form under the environment's backend pin (DASLLAMA_PIN_BACKEND, none where unset), on every
//   box, the parity reference's greedy stream (`_parity_cells.das`); a pin the box cannot take skips that form loudly.
// - The per-32 cuts (`test_gpu_resident_moe_per32`, arm `vk-per32-moe`): Q5_1, Q4_0 and Q5_0, each read site by site in its
//   own format, a 40-token window against the CPU chain.
// The Qwen3-30B UD-IQ2_XXS, the MoE with no shared expert (the residual step's add partner off), holds the format census
// first - every site the file carries read in its own format - as does the Q4_K_M host-layer fixture; the Qwen3.6-35B
// cuts' blocks follow the per-op heat cell. Qwen3-Coder-Next
// (512 experts) holds the device select past 256 experts beside the host-layer split. Those whole-file fixtures are their categories'
// (qwen3moe-30b, qwen35moe-35b, qwen3coder-next), run under `--category`. The Qwen1.5 Q8_0 cut's Metal form (`metal_form`, the blob twin) carries the
// Metal batched driver's shared-expert cells (`_metal_batched_cells.das`: the batched row and the one-row same-slab verify), one
// skipped cell on a build without das_metal. The file is -jit only, and skips without the file or the armed tier; a codec form skips where
// DASLLAMA_VK_KV32=1 arms the f32 mirror over its codec; the two-window feeds where the prompt window is not PF_WINDOW_ROWS
// rows; the CUDA home's own cells off DASLLAMA_CUDA=1, the pool cells where DASLLAMA_GPU_HEAT shadows their slot ask or the
// plan seats no pool; the per-op heat cells skip under DASLLAMA_CUDA=1 (the per-op tier and its heat pool are the Vulkan
// home's); the driver's admission of every fixture is asserted, never skipped - on a device with neither the cm2 tile
// family nor KHR cooperative matrices at subgroup 32 (or under DASLLAMA_COOPMAT=sdot4) the expert pieces ride the Q8-fed
// chain's batch tiles.

//! the Qwen1.5-MoE cuts' blocks: their four trunk blocks, the routed
//! layers asked onto the host (of four)
let private QWEN2MOE_BLOCKS = 4l
let private QWEN2MOE_HOST_LAYERS = 2l
let private LONG_FED_STEPS = 8l   //! the CPU form's feeds: the CUDA feeds' steps, the routed feeds' a prefix
let private POOL_SLOTS = 8l   //! the pool cells' slot ask a layer: fewer than the cut's sixty experts, so misses land by DMA
let private POOL_CTX = 8192l  //! ... and their context, asserted on the plan

//! the Q4_K_M mint of the Q8_0 carrier cut to its first four blocks
//! (the cut's row in ../performance/model_specs.das)
let private TWIN = MoeFixture(file = "Qwen1.5-MoE-A2.7B-4L-Q4_K_M-local.gguf", shexp = true,
    bar_rel = MOE_LOGIT_BAR_REL)
let private TWIN_HOST = MoeFixture(file = "Qwen1.5-MoE-A2.7B-4L-Q4_K_M-local.gguf", shexp = true,
    bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = QWEN2MOE_HOST_LAYERS)
//! the Q8_0 carrier's cut
let private Q8 = MoeFixture(file = "Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf", shexp = true,
    bar_rel = MOE_LOGIT_BAR_REL)
let private Q8_HOST = MoeFixture(file = "Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf", shexp = true,
    bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = QWEN2MOE_HOST_LAYERS)

//! a Qwen1.5-MoE cut's forms' readings: the prompts, the default form
//! beside its CPU form (`main`) - its admission and census (`carrier`),
//! its routed feeds, the CUDA home's feeds - the CPU form's feeds of
//! the prompts on the main codec, the q8_0 mirror's feed and the CPU's
//! on q8_0, the tq4 ask's readings, the host-layers forms', the pool
//! cells', the regions rig's
struct QwenMoeRows {
    prompts : array<int64>
    cuda : bool
    main : MainPair
    carrier : Held
    main_feeds : array<FeedRows>
    cuda_feeds : array<FeedRows>
    cpu_kv : KVDtype
    cpu_feeds : array<FeedRows>
    codec : Held
    codec_gpu : FeedRows
    codec_gpu_ran : bool
    codec_cpu : FeedRows
    codec_cpu_ran : bool
    host : MoeHostRows = MoeHostRows()
    pool : Held
    pool_feeds : array<FeedRows>
    regions : Held
    batched_cell_bar : LogitsBar  //! the regions rig's batched-step bar
    split_cell_bar : LogitsBar    //! ... and its split cell's
    blocks : int64              //! the cut's trunk blocks
    arch : string               //! ... and its family
    tq4_distance : bool         //! the tq4 form's distance cell, else the serving rule
    tq4_gpu : array<array<float>>
    tq4_gpu_ran : bool
    tq4_cpu : FeedRows
    tq4_cpu_ran : bool
    tq4_f16 : FeedRows          //! ... and the CPU f16 chain the distances read, off the same CPU load
    host_cpu : array<FeedRows>  //! the CPU form's feeds on the host-layers forms' codec where the main codec is another
    host_exact : bool           //! the host-layers forms' count asserted exact on every home
}

//! the block's prompts and home: 40 tokens, and two windows where the
//! prompt window is the cells' own
def private qwen_moe_rows_init(var r : QwenMoeRows; fx : MoeFixture; blocks : int64; arch : string;
                               host_first : int64) {
    r.cuda = gpu_want_cuda()
    r.blocks = blocks
    r.arch = arch
    r.prompts <- window_prompts(r.carrier)
    r.host.fx = fx
    r.host.prompts := r.prompts
    r.host.cuda = r.cuda && host_first > 0l
    r.host.host_first = host_first
}

//! the default form: the admission, the routed feeds of every prompt
//! (the census bound to the first), the CUDA home's feeds of them
def private qwen_moe_main_rows(t : T?; var r : QwenMoeRows; m : Model; form : ModelForm; fx : MoeFixture;
                               routed : bool) {
    cut_held(r.carrier, m, r.blocks, r.arch)
    let host_layers = moe_fixture_admitted(r.carrier, m, fx)
    return if (host_layers < 0l)
    if (fx.census) {
        var inscope any_sites, bound : array<string>
        census_cell(t, m, path_join(models_dir(), fx.file), "vulkan load", any_sites, bound)
    }
    if (routed) {
        moe_prompt_feeds(r.main_feeds, r.carrier, m, form, fx, r.prompts, host_layers)
    }
    if (r.cuda) {
        r.cuda_feeds |> reserve(length(r.prompts))
        for (i in range(routed ? length(r.prompts) : 1)) {
            let n = r.prompts[i]
            r.cuda_feeds |> emplace <| cuda_feed_rows(r.carrier, m, form, n, prompt_windows(n))
        }
    }
}

//! the CPU form: the feeds of every prompt on the main codec, the
//! first under the CUDA counters' control; on q8_0 the codec cell's
def private qwen_moe_cpu_rows(var r : QwenMoeRows; m : Model; form : ModelForm) {
    if (form.kv == (r.main.ran ? r.main.kv : r.host.kv)) {
        r.cpu_kv = form.kv
        for (n, i in r.prompts, count()) {
            with_cpu_counters_still(r.carrier, r.cuda && i == 0) {
                r.cpu_feeds |> emplace <| resident_feed_rows(m, form, n, LONG_FED_STEPS)
            }
        }
    } elif (r.host.ran && form.kv == r.host.kv) {
        r.host_cpu |> reserve(length(r.prompts))
        for (n in r.prompts) {
            r.host_cpu |> emplace <| resident_feed_rows(m, form, n, LONG_FED_STEPS)
        }
    } elif (form.kv == KVDtype.tq4 && r.tq4_gpu_ran) {
        delete r.tq4_cpu
        r.tq4_cpu <- resident_feed_rows(m, form, 40l, CODEC_FED_STEPS)
        var f16 = form
        f16.kv = KVDtype.f16
        delete r.tq4_f16
        r.tq4_f16 <- resident_feed_rows(m, f16, 40l, CODEC_FED_STEPS)
        r.tq4_cpu_ran = true
    } elif (form.kv == KVDtype.q8_0 && r.codec_gpu_ran) {
        delete r.codec_cpu
        r.codec_cpu <- resident_feed_rows(m, form, 40l, FED_STEPS)
        r.codec_cpu_ran = true
    }
}

//! a codec form: the q8_0 mirror's routed feed; the tq4 ask - the ask
//! the load took tells it apart, the serving rule arming both at q8_0
//! - its serving rule
def private qwen_moe_codec_rows(t : T?; var r : QwenMoeRows; m : Model; form : ModelForm) {
    if (gpu_kv_want_pin().value == KVDtype.tq4 && r.tq4_distance) {
        delete r.tq4_gpu
        r.tq4_gpu <- codec_device_rows(t, m, 40l, KVDtype.tq4, false)
        r.tq4_gpu_ran = true
        return
    }
    if (gpu_kv_want_pin().value == KVDtype.tq4) {
        r.codec |> held_fact(m.config.attn_qkv_bias, "the twin's attention carries a Q/K/V bias")
        r.codec |> held_equal(form.kv, KVDtype.q8_0, "the tq4 ask armed the q8_0 mirror by the serving rule")
        r.codec |> held_equal(kv_codec_for(m.config, KVDtype.tq4), KVDtype.q8_0,
            "and a session's codec steps down the same way")
        return
    }
    let host_layers = moe_fixture_admitted(r.codec, m, TWIN)
    return if (host_layers < 0l)
    delete r.codec_gpu
    r.codec_gpu <- moe_feed_rows(r.codec, m, form, TWIN, 40l, FED_STEPS, host_layers, true)
    r.codec_gpu_ran = true
}
//! the pool cells' slot ask goes through the tier's want, which
//! DASLLAMA_GPU_HEAT in the environment shadows: why the pool forms do
//! not load, "" where the ask stands
def private pool_shadow_why() : string {
    let env = g_env_gpu.gpu_heat ?? POOL_SLOTS
    return env != POOL_SLOTS ? ("DASLLAMA_GPU_HEAT={env} in the environment shadows the pool " +
        "cells' slot ask of {POOL_SLOTS}") : ""
}

//! the pool's counters a cell reads across a feed: the experts the copy stream carried, the hit chains and the picks they
//! served, the DMA-placed experts, the met steps, the host's uploads and the streamed windows
struct private PoolMeter {
    fills : int64
    submits : int64
    hits : int64
    dma : int64
    meet_steps : int64
    uploads : int64
    windows : int64
}

//! the pool's counters now (`pool_since` reads a feed's share against them)
def private pool_meter() : PoolMeter {
    static_if (typeinfo builtin_module_exists(cuda)) {
        let hot = cud_rdec_hot_stats()
        let s = gpu_resident_hot_stats()
        return PoolMeter(fills = cud_stream_dma_fills(), submits = hot.submits, hits = hot.hits, dma = s.dma,
            meet_steps = s.meet_steps, uploads = s.uploads, windows = cud_stream_stats().windows)
    } else {
        return PoolMeter()
    }
}

//! the pool's counters since `p0` (`pool_meter`)
def private pool_since(p0 : PoolMeter) : PoolMeter {
    let p1 = pool_meter()
    return PoolMeter(fills = p1.fills - p0.fills, submits = p1.submits - p0.submits, hits = p1.hits - p0.hits,
        dma = p1.dma - p0.dma, meet_steps = p1.meet_steps - p0.meet_steps, uploads = p1.uploads - p0.uploads,
        windows = p1.windows - p0.windows)
}

//! a pool form's plan, held: the context asked, the slots asked where
//! the plan seated a pool; false after the counted skip where it seated
//! none
def private pool_plan_held(var h : Held) : bool {
    h |> held_equal(gpu_device_session_ctx(), POOL_CTX,
        "the plan armed at the {POOL_CTX} context asked (set_gpu_ctx_max)")
    if (gpu_resident_hot_slots() == 0l) {
        h |> held_cell_skip("the plan found no room for the pool beside the mirror on this card")
        return false
    }
    h |> held_equal(gpu_resident_hot_slots(), POOL_SLOTS,
        "the pool seats the {POOL_SLOTS} slots a layer asked")
    return true
}

//! the pool form with the prompt stream on: the steps' misses filled
//! by DMA off the stream stacks with the hit chains serving the pooled
//! picks; the decode meet claiming the steps' picks over the pool; both
//! knobs off, the control - the knobs read live, so the three feeds
//! share the load, the DMA cell first while the pool is cold
def private pool_streamed_rows(var r : QwenMoeRows; m : Model; form : ModelForm) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        let file = TWIN.file
        set_gpu_hot_dma(1)
        set_gpu_dec_meet(0)
        let s0 = pool_meter()
        r.pool_feeds |> emplace <| cuda_feed_rows(r.pool, m, form, 40l, 1l)
        let s = pool_since(s0)
        let hot = cud_rdec_hot_stats()
        to_log(LOG_INFO, "{file}: {gpu_resident_hot_slots()} pool slots a layer; the DMA fill " +
            "copied {s.fills} experts ({s.dma} placed), the pool " +
            "launched {hot.submits} hit chains serving {hot.hits} picks\n")
        if (pool_plan_held(r.pool)) {
            r.pool |> held_fact(s.dma > 0l, "the DMA fill landed {s.dma} " +
                "experts in the pool off the stream stacks over the cell")
            r.pool |> held_fact(s.fills >= s.dma, "every landed " +
                "expert was the copy stream's ({s.fills} copies)")
            r.pool |> held_fact(hot.submits > 0l && hot.hits > 0l, "the pool's hit chains served " +
                "picks beside the host's sums ({hot.submits} chains, {hot.hits} picks)")
        }
        set_gpu_dec_meet(1)
        let m0 = pool_meter()
        r.pool_feeds |> emplace <| cuda_feed_rows(r.pool, m, form, 40l, 1l)
        let mt = pool_since(m0)
        let met = mt.meet_steps
        to_log(LOG_INFO, "{file}: the decode meet served {met} steps over " +
            "{mt.windows} streamed windows, the pool promoted " +
            "{mt.uploads} experts\n")
        if (pool_plan_held(r.pool)) {
            r.pool |> held_fact(met > 0l, "the decode meet served {met} of the fed steps' routed " +
                "layers over the pool")
            r.pool |> held_fact(mt.windows > met, "every met step's layer ran " +
                "as a streamed window beside the prompt's ({mt.windows} windows)")
        }
        set_gpu_hot_dma(0)
        set_gpu_dec_meet(0)
        let o0 = pool_meter()
        r.pool_feeds |> emplace <| cuda_feed_rows(r.pool, m, form, 40l, 1l)
        let o = pool_since(o0)
        let o1 = gpu_resident_hot_stats()
        if (pool_plan_held(r.pool)) {
            r.pool |> held_equal(o.fills, 0l,
                "control: with the fill off the copy stream carried no pool expert")
            r.pool |> held_equal(o.meet_steps, 0l,
                "control: with the meet off no fed step's layer went through the window's meet")
            r.pool |> held_fact(o1.uploads > 0l, "the window's placements and the steps' misses " +
                "took the host's upload ({o1.uploads})")
        }
    }
}

//! the pool form with the prompt stream off: no stream stack stands,
//! so the DMA fill places nothing and the host's fill lands every
//! pooled expert, the hit chains still serving off the slots
def private pool_unstreamed_rows(var r : QwenMoeRows; m : Model; form : ModelForm) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        set_gpu_hot_dma(1)
        set_gpu_dec_meet(0)
        let d0 = pool_meter()
        r.pool_feeds |> emplace <| cuda_feed_rows(r.pool, m, form, 40l, 1l)
        let d = pool_since(d0)
        let s1 = gpu_resident_hot_stats()
        let hot = cud_rdec_hot_stats()
        to_log(LOG_INFO, "{TWIN.file}: {gpu_resident_hot_slots()} pool slots a layer with the " +
            "prompt stream off; the DMA fill copied {d.fills} experts " +
            "({s1.dma} placed), the pool launched {hot.submits} hit chains serving {hot.hits} " +
            "picks\n")
        if (pool_plan_held(r.pool)) {
            r.pool |> held_equal(s1.dma, 0l, "with no streamed stack the DMA fill placed nothing " +
                "over this load (the load resets the count) - the host's fill took every pooled " +
                "expert")
            r.pool |> held_equal(d.fills, 0l, "and the copy stream copied none")
            r.pool |> held_fact(hot.submits > 0l && hot.hits > 0l, "the pool's hit chains served " +
                "picks off the host-filled slots ({hot.submits} chains, {hot.hits} picks)")
        }
    }
}

//! the pool counters' control: the pool's ask with every routed
//! layer's experts on the device - no host layer, so no pool, no hit
//! chain, no DMA fill and no met step
def private pool_unpooled_rows(var r : QwenMoeRows; m : Model; form : ModelForm) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        set_gpu_hot_dma(1)
        set_gpu_dec_meet(1)
        let p0 = pool_meter()
        r.pool_feeds |> emplace <| cuda_feed_rows(r.pool, m, form, 40l, 1l)
        let p = pool_since(p0)
        r.pool |> held_equal(gpu_resident_host_layers(), 0l,
            "control: no routed layer's experts serve on the host")
        r.pool |> held_equal(gpu_resident_hot_slots(), 0l,
            "control: with no host layer the plan arms no pool")
        r.pool |> held_equal(p.submits, 0l,
            "control: the pool launched no hit chain ({p.submits})")
        r.pool |> held_equal(p.hits, 0l, "control: ... and served no pick")
        r.pool |> held_equal(p.fills, 0l, "control: the copy stream filled no slot")
        r.pool |> held_equal(p.meet_steps, 0l,
            "control: no fed step went through the meet")
    }
}

//! a pool form's cells, the decode knobs put back as found
def private qwen2moe_pool_rows(var r : QwenMoeRows; m : Model; form : ModelForm) {
    let pins = pin_stream(MOE_HOST_FIRST, plain_steps = false)
    if (form.host_layers == 0l) {
        pool_unpooled_rows(r, m, form)
    } else {
        r.pool |> held_equal(gpu_resident_host_layers(), QWEN2MOE_HOST_LAYERS,
            "exactly the first {QWEN2MOE_HOST_LAYERS} routed layers' experts serve on the host " +
            "as asked (set_gpu_host_layers; the card holds the rest)")
        if (form.pf_stream) {
            pool_streamed_rows(r, m, form)
        } else {
            pool_unstreamed_rows(r, m, form)
        }
    }
    restore_stream_pins(pins)
}

//! one form's collection of a Qwen1.5-MoE cut's cells into `r`
//! (`routed` the default form's routed feeds and the codec forms on)
def private qwen_moe_rows(t : T?; var r : QwenMoeRows; m : Model; form : ModelForm; fx : MoeFixture;
                          routed : bool) {
    let side = main_side(r.main, form)
    if (side == MainSide.device) {
        qwen_moe_main_rows(t, r, m, form, fx, routed)
    } elif (!form.on_gpu) {
        qwen_moe_cpu_rows(r, m, form)
    } elif (form.pool_slots > 0l) {
        qwen2moe_pool_rows(r, m, form)
    } elif (form.host_layers > 0l) {
        if (form.pf_stream) {
            cut_held(r.host.h, m, r.blocks, r.arch)
            if (r.host_exact) {
                r.host.h |> held_equal(gpu_resident_host_layers(), r.host.fx.host_layers_ask,
                    "exactly {r.host.fx.host_layers_ask} of the cut's {r.blocks} routed layers' experts " +
                    "serve on the host as asked (set_gpu_host_layers)")
            }
        }
        moe_host_rows(r.host, m, form)
    } elif (form.regions == 2l) {
        regions_batched_rows(r.regions, m, r.batched_cell_bar)
        regions_split_rows(r.regions, m, r.split_cell_bar)
    } elif (form.kv_asked) {
        qwen_moe_codec_rows(t, r, m, form)
    }
}

//! a cut's compares after its block: the default form's routed feeds
//! within the fixture's bar of the CPU form's prefix, the CUDA feeds
//! and the pool cells' within the routed bar, the codec cell, the
//! host-layers forms', the regions rig's
def private qwen_moe_rows_within(t : T?; tag : string; r : QwenMoeRows) {
    held_within(t, r.carrier, tag, false)
    held_within(t, r.codec, "{tag}codec forms: ", false)
    held_within(t, r.pool, "{tag}the pool: ", false)
    held_within(t, r.regions, "{tag}the regions rig: ", false)
    with_dasllama_jobque_() {
        feeds_within(t, tag, r.main_feeds, r.cpu_feeds, MOE_LOGIT_BAR_REL, "", FED_STEPS)
        feeds_within(t, "{tag}cuda, ", r.cuda_feeds, r.cpu_feeds, MOE_LOGIT_BAR_REL)
        for (f in r.pool_feeds) {
            let what = "{tag}the pool, {f.nprompt} tokens: "
            continue if (!twin_collected(t, 0, length(r.cpu_feeds), what))
            feed_rows_within(t, what, f, r.cpu_feeds[0], MOE_LOGIT_BAR_REL)
        }
        if (r.codec_gpu_ran && r.codec_cpu_ran) {
            feed_rows_within(t, "{tag}a q8_0 mirror: ", r.codec_gpu, r.codec_cpu, MOE_LOGIT_BAR_REL)
        }
        if (r.host.ran) {
            moe_host_rows_within(t, "{tag}{r.host.fx.host_layers_ask} routed layers on the host: ",
                r.host, r.host.kv == r.cpu_kv ? r.cpu_feeds : r.host_cpu)
        }
        if (r.tq4_gpu_ran) {
            t |> success(r.tq4_cpu_ran, "{tag}the CPU form ran the tq4 chain and the f16 chain the tq4 mirror's distances read")
        }
        if (r.tq4_gpu_ran && r.tq4_cpu_ran) {
            unsafe {
                t |> run("{tag}the routed chain over a tq4 mirror - the device, the CPU tq4 chain and the CPU f16 chain over one prompt, the distances held as ratios") @capture(& r)(t : T ?) {
                    codec_distances_within(t, "tq4, 40 tokens", r.tq4_gpu, r.tq4_cpu.rows, r.tq4_f16.rows, 0,
                            MOE_CODEC_DIST_REL, MOE_CODEC_PAIR_REL) $(id) {
                        return feed_piece(r.tq4_cpu, r.tq4_f16, id)
                    }
                }
            }
        }
    }
}

//! a Qwen1.5-MoE cut's host-layers pair: the prompt stream on, then off
def private host_pair(var forms : array<ModelForm>; layers : int64) {
    forms |> push(device_form(regions = 1l, host_layers = layers) |> asking(KVDtype.f16))
    forms |> push(device_form(regions = 1l, host_layers = layers, pf_stream = false) |> asking(KVDtype.f16))
}

//! the Q4_K_M cut's forms: the default, the q8_0 mirror and the tq4
//! ask (a codec the environment keeps off held as a skip), the
//! host-layers pair, and on the CUDA home the pool's three forms (held
//! as a skip where the environment shadows their ask)
def private twin_forms(var r : QwenMoeRows) : array<ModelForm> {
    var forms <- [device_form()]
    codec_forms(forms, r.codec, [KVDtype.q8_0, KVDtype.tq4], device_form()) $(kv) => "the {kv} ask"
    host_pair(forms, QWEN2MOE_HOST_LAYERS)
    return <- forms if (!r.cuda)
    let why = pool_shadow_why()
    if (!empty(why)) {
        r.pool |> held_cell_skip("the pool cells: {why}")
        return <- forms
    }
    forms |> push(device_form(regions = 1l, ctx = POOL_CTX, host_layers = QWEN2MOE_HOST_LAYERS,
        pool_slots = POOL_SLOTS))
    forms |> push(device_form(regions = 1l, ctx = POOL_CTX, pool_slots = POOL_SLOTS))
    forms |> push(device_form(regions = 1l, ctx = POOL_CTX, host_layers = QWEN2MOE_HOST_LAYERS,
        pf_stream = false, pool_slots = POOL_SLOTS))
    return <- forms
}

[test(description = "On Qwen1.5-MoE-A2.7B-4L Q4_K_M, the four-block cut, the resident routed window and token command hold the CPU chain's logits within the routed bar over one and two windows, the routed census growing by the routed layer count; a q8_0 mirror holds the CPU chain on q8_0 sessions, and a tq4 ask serves at q8_0 by the serving rule; with two of the four routed layers' experts asked onto the host the split token command and the cut window chain hold the routed bar over one and two windows and the streamed windows the stream twin's bar of the host's sums; on the CUDA driver the feeds over one and two windows, the host cells with the meet splitting the experts and the prompt stream off as control, and the hot pool's DMA fill and decode meet, both off, an all-device load and an unstreamed pool as controls, every feed within 20 percent of max logit of the CPU chain; each form loads once")]
def test_gpu_resident_moe(t : T?) {
    return if (!family_on(t, "qwen2moe"))
    var r = QwenMoeRows()
    qwen_moe_rows_init(r, TWIN_HOST, QWEN2MOE_BLOCKS, "qwen2moe", MOE_HOST_FIRST)
    var forms <- twin_forms(r)
    let pins = pin_stream(0l)   //! the device claims first: the meet's share the host cells pin live
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen1.5-MoE-A2.7B-4L-Q4_K_M-local.gguf", MOE_CUT_CTX, forms) $(m, form) {
            qwen_moe_rows(t, r, m, form, TWIN, true)
        }
    }
    restore_stream_pins(pins)
    if (ran) {
        qwen_moe_rows_within(t, "Qwen1.5-MoE-A2.7B-4L Q4_K_M: ", r)
    }
    delete r
    delete forms
}

let private PARITY_CUT_CTX = 2048l   //! the parity row's load context (`parity_row_load`)


[test(description = "On Qwen1.5-MoE-A2.7B-4L Q8_0, the four-block cut over Q8_0 expert stacks, two of the four routed layers' experts asked onto the host hold the CPU chain within the routed bar over one and two windows and the streamed windows the stream twin's bar of the host's sums; the N-row command's plain act and shared-expert requant batch two sessions bit for bit what each reads alone, at one window and past the unsplit position; on the CUDA driver under the pinned MMQ winners the one-window feed and the host cells hold 20 percent of max logit of the CPU chain, the meet splitting the experts and the prompt stream off as control; on the q8 CPU form, on every box, the cut fed the parity reference's greedy stream after the counting fixture keeps every reference id within 2% of its row's top logit, the stream one step late outside; on the Metal home's blob twin, one load on a das_metal build: on the shared-expert Qwen1.5-MoE-A2.7B-4L Q8_0, the batched bench row serves four streams at a rate through the Metal batched driver's rows site, every timed step a device step, none on the CPU stack; on Qwen1.5-MoE-A2.7B-4L Q8_0, one row through the batched driver's same-slab verify lands logits within 2.0 of the single-row step's over 12 rounds, while the one-step-off control lands outside that bar; each form loads once")]
def test_gpu_resident_moe_q8(t : T?) {
    return if (!family_on(t, "qwen2moe"))
    var r = QwenMoeRows()
    qwen_moe_rows_init(r, Q8_HOST, QWEN2MOE_BLOCKS, "qwen2moe", MOE_HOST_FIRST)
    //! past the unsplit position, where the card splits the keys apart
    //! from the row alone, the cut's router sits on a near-tie: 0.0603
    //! of a 13.3 peak at batched step 3 row 1 on the RTX PRO 4500 (the
    //! argmax flipping there, both picks logged), past the split bar's
    //! 0.06 and inside the wide bar's 0.10; the row one off reads 15-17
    r.split_cell_bar = LogitsBar.bit_or_wide
    var forms : array<ModelForm>
    if (r.cuda) {
        forms |> push(device_form())
    }
    host_pair(forms, QWEN2MOE_HOST_LAYERS)
    forms |> push(device_form(regions = 2l, ctx = REGIONS_RIG_CTX))
    //! the parity row's environment pin; the row's K-quant native knob reads nothing on a file whose tagged sites are all Q8_0
    forms |> push(cpu_cells_form(backend = g_env_engine.pin_backend))
    forms |> push(metal_form(ctx = METAL_ROW_CTX))
    let pins = pin_stream(0l)   //! the device claims first: the meet's share the host cells pin live
    var ran = false
    var dev_ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf", MOE_CUT_CTX, forms) $(var m, form) {
            if (form.on_metal) {
                metal_qwen_moe_cells(t, m)
                return
            }
            dev_ran ||= form.on_gpu
            if (dev_ran && (form.on_gpu || empty(form.backend))) {
                qwen_moe_rows(t, r, m, form, Q8, false)
            }
            if (!form.on_gpu && form.backend == g_env_engine.pin_backend && form.alone) {
                with_ctx_cut(m, PARITY_CUT_CTX) {
                    parity_qwen2moe_cut_cell(t, m)
                }
            }
        }
    }
    restore_stream_pins(pins)
    if (ran) {
        qwen_moe_rows_within(t, "Qwen1.5-MoE-A2.7B-4L Q8_0: ", r)
    }
    delete r
    delete forms
}

//! a per-32 cut's block: the census site by site, a 40-token window
//! and its steps against the CPU chain
struct private Per32Rows {
    main : MainPair
    carrier : Held
    gpu : FeedRows
    cpu : FeedRows
    gpu_ran : bool
}

def private per32_rows(t : T?; var r : Per32Rows; m : Model; form : ModelForm; fx : MoeFixture) {
    let side = main_side(r.main, form)
    if (side == MainSide.device) {
        cut_held(r.carrier, m, QWEN2MOE_BLOCKS, "qwen2moe")
        let host_layers = moe_fixture_admitted(r.carrier, m, fx)
        return if (host_layers < 0l)
        var inscope any_sites, bound : array<string>
        census_cell(t, m, path_join(models_dir(), fx.file), "vulkan load", any_sites, bound)
        delete r.gpu
        r.gpu <- moe_feed_rows(r.carrier, m, form, fx, 40l, FED_STEPS, host_layers, true)
        r.gpu_ran = true
    } elif (side == MainSide.cpu) {
        delete r.cpu
        r.cpu <- resident_feed_rows(m, form, 40l, FED_STEPS)
    }
}

def private per32_rows_within(t : T?; fx : MoeFixture; r : Per32Rows) {
    held_within(t, r.carrier, "{fx.file}: ", false)
    return if (!r.gpu_ran)
    with_dasllama_jobque_() {
        feed_rows_within(t, "{fx.file}: ", r.gpu, r.cpu, fx.bar_rel)
    }
}

//! the per-32 mints of the Q8_0 carrier cut to four blocks (`--pure`,
//! their recipe and cut rows in ../performance/model_specs.das): the
//! Q5_1 file's every plane q51 - the shared expert and the 1408-wide
//! routed rows among them - the Q4_0 file's 1408-wide routed down rows
//! q4032, and the Q5_0 file's every plane q5032 - the routed and
//! shared expert stacks among them
let private PER32_Q51 = MoeFixture(file = "Qwen1.5-MoE-A2.7B-4L-Q5_1-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, census = true)
let private PER32_Q4032 = MoeFixture(file = "Qwen1.5-MoE-A2.7B-4L-Q4_0-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, census = true)
let private PER32_Q5032 = MoeFixture(file = "Qwen1.5-MoE-A2.7B-4L-Q5_0-pure-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, census = true)

[test(description = "The per-32 four-block cuts of Qwen1.5-MoE - Q5_1, Q4_0 and Q5_0 routed and shared expert stacks - read every site in its own format on the device and hold the CPU chain's logits within the routed bar over one window; each form loads once")]
def test_gpu_resident_moe_per32(t : T?) {
    return if (!family_on(t, "qwen2moe") || !arm_on(t, "vk-per32-moe"))
    var q51 : Per32Rows
    if (with_cpu_and_gpu_model(t, "llm", "Qwen1.5-MoE-A2.7B-4L-Q5_1-local.gguf", MOE_CUT_CTX, [device_form()]) $(m, form) {
                per32_rows(t, q51, m, form, PER32_Q51)
            }) {
        per32_rows_within(t, PER32_Q51, q51)
    }
    delete q51
    var q4032 : Per32Rows
    if (with_cpu_and_gpu_model(t, "llm", "Qwen1.5-MoE-A2.7B-4L-Q4_0-local.gguf", MOE_CUT_CTX, [device_form()]) $(m, form) {
                per32_rows(t, q4032, m, form, PER32_Q4032)
            }) {
        per32_rows_within(t, PER32_Q4032, q4032)
    }
    delete q4032
    var q5032 : Per32Rows
    if (with_cpu_and_gpu_model(t, "llm", "Qwen1.5-MoE-A2.7B-4L-Q5_0-pure-local.gguf", MOE_CUT_CTX, [device_form()]) $(m, form) {
                per32_rows(t, q5032, m, form, PER32_Q5032)
            }) {
        per32_rows_within(t, PER32_Q5032, q5032)
    }
    delete q5032
}

[test(description = "On the per-op tier with half the routed layers placed and half streamed under the heat pool, Qwen3.6-35B-A3B-4L IQ4_XS and Qwen1.5-MoE-A2.7B-4L Q5_1 (each two routed layers placed, two streamed) hold the CPU chain's routed bar, every shared expert on the device")]
def test_gpu_perop_moe_heat(t : T?) {
    t |> run("Qwen3.6-35B-A3B-4L IQ4_XS on the per-op tier: two routed layers placed, two streamed under the heat pool, every layer's shared expert on the device after the routed chain or the pool's hits, against the CPU chain") @(t : T?) {
        if (!family_on(t, "qwen35moe")) return
        let placement = moe_perop_heat_cell(t, CENSUS35, 2l, 16l)
        if (placement.routed > 0l) {
            t |> equal(placement.routed, QWEN35MOE_BLOCKS,
                "every block of the {QWEN35MOE_BLOCKS}-block cut routes")
            t |> equal(placement.placed, 2l, "the walk placed two of the {placement.routed} routed layers")
            t |> equal(placement.routed - placement.placed, 2l, "... and streamed the other two")
        }
    }
    t |> run("Qwen1.5-MoE-A2.7B-4L Q5_1 on the per-op tier: two routed layers placed, two streamed under the heat pool's q51 slots, the q51 shared expert on the device after them, against the CPU chain") @(t : T?) {
        if (!family_on(t, "qwen2moe")) return
        let placement = moe_perop_heat_cell(t, PER32_Q51, 2l, 8l)
        if (placement.routed > 0l) {
            t |> equal(placement.routed, QWEN2MOE_BLOCKS,
                "every block of the {QWEN2MOE_BLOCKS}-block cut routes")
            t |> equal(placement.placed, 2l, "the walk placed two of the {placement.routed} routed layers")
            t |> equal(placement.routed - placement.placed, 2l, "... and streamed the other two")
        }
    }
}

// Qwen3.6-35B-A3B's four-block cuts (`Qwen3.6-35B-A3B-4L-*-local.gguf`, family `qwen35moe`): three recurrent deltanet
// layers and one full-attention layer, 256 experts, 8 routed, the gated shared expert; the MTP cuts keep the NextN
// draft block as block 4. A cut writes no confident text: its rows are held to the CPU chain's or to the driver's own
// other forms, never to an argmax, and a draft's acceptance is low by nature - the cells hold the device round
// against the CPU round and the round's mechanics, never a rate. Each file is ONE block (`with_cpu_and_gpu_model`).
// - The UD-IQ2_XXS cut (`test_gpu_resident_moe_hybrid`): the format census and the default form's feeds over one
//   window and two against the CPU chain (the conv tail and the state carry the seam); on a one-region form of its
//   own the F32 beta/alpha rows through the f32 tile once a recurrent layer and the f32 router-form GEMV each step
//   (arm `vk-census`).
// - The MTP-IQ4_XS-pure cut (`test_gpu_resident_moe_iq4xs`): at one region on the rig's context the IQ4_XS census
//   (every site native, no layer decline, the IQ4_XS tile, GEMV and table gather on the device; arm `vk-census`),
//   the greedy chain forced on and adaptive bit for bit the plain walk's and the short tail's peel, speculation
//   off; at two regions the verify rows against one-row steps (arm `vk-census`), a prefill continued past zero
//   from the region's live recurrent state and the pool's openings; at two regions with the planes on the host the
//   routed NextN head against the CPU head (arm `vk-census`); at two regions on the 512-row window the segmented
//   windows and the stops one call captures; at ten regions the command groups on column tiles and, on the CUDA
//   home, the rows tensor leaf; the per-op form's deltanet and shared-expert rails (arm `vk-census`, the Vulkan
//   home's).
// - The MTP-UD-Q4_K_M cut (`test_gpu_resident_moe_host_35b`, the CUDA home's): the first routed layer's experts
//   asked onto the host, the split token command and the cut window chain against the CPU chain with the hot pool
//   and the stream's meet, and six speculative rounds drafted and verified on the device.
// - The UD-Q4_K_M cut (`test_gpu_resident_moe_35b_cut_cpu`), on two CPU forms on every box: under the portable backend's
//   pin batched decode against sequential over f32-on-disk beta/alpha rows (`_batch_decode_cells.das`), and under the
//   environment's backend pin (DASLLAMA_PIN_BACKEND, none where unset) with the K-quant planes requantized to q8 the parity
//   reference's greedy stream (`_parity_cells.das`); a pin the box cannot take skips its form loudly.
// The whole UD-IQ2_XXS file's room-forced host split at 131072 positions (`test_gpu_resident_moe_ctx_strict`) stays
// on it, the `qwen35moe-35b` category's. Skips: the vk-census arms filtered out, the per-op form under the CUDA ask,
// the rows leaf on a home with none, the pool assert where the plan seats no pool, the two-window feed where the
// prompt window is not PF_WINDOW_ROWS rows.

let private QWEN35MOE_BLOCKS = 4l
let private Q35_IQ2XXS_CUT = MoeFixture(file = "Qwen3.6-35B-A3B-4L-UD-IQ2_XXS-local.gguf", shexp = true,
    bar_rel = MOE_LOGIT_BAR_REL, census = true, dn_chunk = true)
//! the cut's F32 beta/alpha rows (the whole file's, every recurrent layer's)
let private BA_F32 = MoeFixture(file = "Qwen3.6-35B-A3B-4L-UD-IQ2_XXS-local.gguf", shexp = true,
    bar_rel = MOE_LOGIT_BAR_REL)
//! every big plane IQ4_XS on disk, the beta/alpha and the NextN block's
let private CENSUS35 = MoeFixture(file = "Qwen3.6-35B-A3B-4L-MTP-IQ4_XS-pure-local.gguf", shexp = true,
    bar_rel = MOE_LOGIT_BAR_REL)
let private Q35_PROMPT = 40l   //! one window, past the per-op deltanet chain's 32-row floor
let private Q35_STEPS = 6l
let private Q35_ROUNDS = 6l   //! the speculative rounds a NextN cell runs a session
let private WIDE_ROWS = 10l   //! past the N-row command's eight rows and its GEMVs' eight-column tile

//! the F32 beta/alpha cell's device half on its one-region form: the
//! window's f32 tile dispatches and the steps' f32 router-form GEMVs
struct private BaRows {
    h : Held
    ran : bool
    gpu : FeedRows
    nrec : int64
    pf_ba : int64
    st_rt : int64
    census_read : bool
}

def private ba_rows(var b : BaRows; m : Model; form : ModelForm) {
    cut_held(b.h, m, QWEN35MOE_BLOCKS, "qwen35moe")
    b.h |> held_fact(m.dn_ba_f32, "the file ships its beta/alpha rows F32")
    return if (moe_fixture_admitted(b.h, m, BA_F32) < 0l)
    b.nrec = recurrent_layer_count(m.config)
    var inscope prompt <- resident_prompt(Q35_PROMPT)
    var inscope toks <- resident_feed(Q35_STEPS)
    let ba0 = census("dn_ba_cls_spv")
    var rt0 = 0l
    b.gpu <- feed_rows(m, form, prompt, toks) $ {
        b.pf_ba = census("dn_ba_cls_spv") - ba0
        rt0 = census("router_gemv_cls_spv")
    }
    b.st_rt = census("router_gemv_cls_spv") - rt0
    b.census_read = ba0 != NO_CENSUS
    b.h |> held_served()
    b.ran = true
}

//! the F32 beta/alpha cell against the CPU form's feed of the prompt
def private ba_within(t : T?; b : BaRows; cpu : FeedRows) {
    unsafe {
        t |> run("{BA_F32.file}: the F32 beta/alpha rows ride the f32 GEMV and the f32 tile, no f16 router-form dispatch, the window and steps against the CPU chain") @capture(& b, & cpu)(t : T ?) {
            held_within(t, b.h)
            return if (!b.ran)
            var inscope c <- feed_prefix(cpu, Q35_STEPS)
            feed_steps_within(t, "F32 beta/alpha ", Q35_PROMPT, b.gpu.rows, c.rows, b.gpu.toks, BA_F32.bar_rel,
                    "resident", "the CPU chain") $(id) {
                return feed_piece(b.gpu, c, id)
            }
            if (b.census_read) {
                t |> equal(b.pf_ba, b.nrec, "the window's f32 beta/alpha tile ran once per recurrent layer")
                t |> success(b.st_rt >= b.nrec, "the token command recorded an f32 router-form GEMV per recurrent layer at least ({b.st_rt} over {b.nrec})")
            }
        }
    }
}

//! the UD-Q4_K_M cut's batched-decode cell's sessions' context
let private BATCH_CUT_CTX = 64l

[test(description = "On Qwen3.6-35B-A3B-4L UD-Q4_K_M, the 35B's first four blocks, on its CPU forms on every box: under the portable pin every batched step over its f32-on-disk beta/alpha rows runs on the CPU stack and matches sequential logits, KV, deltanet state and conv history within 1e-6 under f16 KV, and decoding below the deltanet position panics; fed the parity reference's greedy stream after the counting fixture every reference id keeps within 3.5% of its row's top logit, the stream one step late outside on all but one row in eight; each form loads once")]
def test_gpu_resident_moe_35b_cut_cpu(t : T?) {
    return if (!family_on(t, "qwen35moe"))
    var forms <- [cpu_cells_form(backend = "portable"),
        cpu_cells_form(backend = g_env_engine.pin_backend, kq_native = KqNative.off)]
    with_cpu_and_gpu_model(t, "llm", "Qwen3.6-35B-A3B-4L-UD-Q4_K_M-local.gguf", PARITY_CUT_CTX, forms) $(var m, form) {
        if (form.kq_native == KqNative.off) {
            parity_qwen35moe_cut_cell(t, m)
        } else {
            with_ctx_cut(m, BATCH_CUT_CTX) {
                batch_decode_hybrid_moe_cell(t, m)
            }
        }
    }
    delete forms
}

[test(description = "On Qwen3.6-35B-A3B-4L UD-IQ2_XXS, the four-block cut of the deltanet hybrid MoE, the recurrent head and routed block hold the CPU chain within the routed bar over one and two windows, every site read in its file format; its F32 beta and alpha rows run through the f32 tile once per recurrent layer and the f32 router-form GEMV each step within 20 percent of max logit of the CPU chain; each form loads once")]
def test_gpu_resident_moe_hybrid(t : T?) {
    return if (!family_on(t, "qwen35moe"))
    var r = QwenMoeRows()
    qwen_moe_rows_init(r, Q35_IQ2XXS_CUT, QWEN35MOE_BLOCKS, "qwen35moe", 0l)
    r.cuda = false   //! the routed feeds on either home: the hybrid carries no CUDA-counted feed
    var ba : BaRows
    var forms <- [device_form()]
    let why = arm_why("vk-census")
    if (empty(why)) {
        forms |> push(device_form(regions = 1l))
    } else {
        ba.h |> held_cell_skip("the F32 beta/alpha cell: {why}")
    }
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.6-35B-A3B-4L-UD-IQ2_XXS-local.gguf", MOE_CUT_CTX, forms) $(m, form) {
            if (form.on_gpu && form.regions == 1l) {
                ba_rows(ba, m, form)
            } else {
                qwen_moe_rows(t, r, m, form, Q35_IQ2XXS_CUT, true)
            }
        }
    }
    if (ran) {
        qwen_moe_rows_within(t, "Qwen3.6-35B-A3B-4L UD-IQ2_XXS: ", r)
        with_dasllama_jobque_() {
            if (!empty(r.cpu_feeds)) {
                ba_within(t, ba, r.cpu_feeds[0])
            } else {
                held_within(t, ba.h, "the F32 beta/alpha cell: ", false)
            }
        }
    }
    delete ba
    delete r
    delete forms
}

//! one speculative round's outcome on a session: its accept, its draft, the position it left and its verify row 0's logits
struct private Round {
    hit : bool
    draft : int64
    n_past : int64
    row0 : array<float>
}

//! one speculative round a fed token on `s` (the token forced, the round's own draft and verify), each round's outcome
def private spec_rounds(tr : Model; var s : Session; toks : array<int64>) : array<Round> {
    var out : array<Round>
    out |> reserve(long_length(toks))
    let vocab = tr.config.vocab_size
    for (tok in toks) {
        var acc = 0l
        let hit = mtp_spec_eval(tr, s, tok, acc)
        out |> emplace <| Round(hit = hit, draft = s.mtp_vbatch[1], n_past = s.n_past, row0 <- [for (i in range64(vocab)); s.mtp_logits_b[i]])
    }
    return <- out
}

//! a round's two verify rows off the device: the logits and the post-norm carry a row, the draft, the accepted count
struct private VerifyRows {
    draft : int64
    accepted : int64
    logits0, logits1 : array<float>
    carry0, carry1 : array<float>
}

//! one speculative round on `s` at its position: the draft, the two-row verify, the walk - the rows landed on the device
[cold_path]
def private verify_round_rows(tr : Model; var s : Session; tok : int64) : VerifyRows {
    var r : VerifyRows
    let vocab = tr.config.vocab_size
    let dim = tr.config.dim
    r.accepted = mtp_spec_round(tr, s, tok)
    r.draft = s.mtp_vbatch[1]
    r.logits0 <- [for (i in range64(vocab)); s.mtp_logits_b[i]]
    r.logits1 <- [for (i in range64(vocab)); s.mtp_logits_b[vocab + i]]
    r.carry0 <- [for (i in range64(dim)); s.mtp_hrows[i]]
    r.carry1 <- [for (i in range64(dim)); s.mtp_hrows[dim + i]]
    return <- r
}

//! where a stream ends on a session: its logits and its carry
struct private StreamEnd {
    logits : array<float>
    carry : array<float>
}

def private stream_end(s : Session) : StreamEnd => <- StreamEnd(logits <- [for (x in s.logits); x], carry <- [for (x in s.mtp_h); x])

//! `rounds` speculative rounds on `s` over `fed`, each round's token then its accepted drafts committed onto `toks`; where `s` ends
[cold_path]
def private committed_rounds(tr : Model; var s : Session; fed : array<int64>; rounds : int64; var toks : array<int64>) : StreamEnd {
    toks |> reserve(rounds * MTP_MAX_ROWS)
    spec_rounds_into(tr, s, toks, $(round, _committed) => round < rounds, $(round) => fed[round])
    return <- stream_end(s)
}

//! `toks` as one-row steps on `s` from its position: where `s` ends
[cold_path]
def private stepped(tr : Model; var s : Session; toks : array<int64>) : StreamEnd {
    for (tok in toks) {
        forward(tr, s, tok, s.n_past)
        s.n_past++
    }
    return <- stream_end(s)
}

//! `prompt` prefilled on `s` with the speculative warm on, the session's position at its end
def private prefill_warm(tr : Model; var s : Session; prompt : array<int64>) {
    forward_prefill(tr, s, prompt, long_length(prompt), 0l)
    s.n_past = long_length(prompt)
}

//! the census of the named kernels as one sum (NO_CENSUS when the module is absent)
def private census_sum(names : array<string>) : int64 {
    var n = 0l
    for (k in names) {
        let c = census(k)
        return NO_CENSUS if (c == NO_CENSUS)
        n += c
    }
    return n
}

//! the device-home session's rounds `home` against the host-cached session's `host`, the forced-feed form: both took the same fed
//! token at the same position until their drafts part, so through the parting round each verify row 0 is one input on two K/V
//! homes - held within `bar_rel` with the host's previous round's row as the control; the drafts are the head's picks over the
//! whole vocabulary and can part on a near-tie, so the rounds compare exactly only while they agree
def private rounds_agree(t : T?; tr : Model; host, home : array<Round>; bar_rel : float) {
    var agreed = 0l
    for (r in range64(long_length(host))) {
        to_log(LOG_INFO, "round {r}: host-cached draft {host[r].draft} '{decode_(tr, [host[r].draft])}' hit {host[r].hit} at {host[r].n_past}, device-home draft {home[r].draft} '{decode_(tr, [home[r].draft])}' hit {home[r].hit} at {home[r].n_past}\n")
        let ctrl_round = r > 0l ? r - 1l : 1l
        row_within_ctrl(t, "round {r}: the device-home verify row 0 against the host-cached session's", home[r].row0, host[r].row0, host[ctrl_round].row0, bar_rel, "(the host's round {ctrl_round} as the control)")
        break if (home[r].draft != host[r].draft)
        t |> equal(home[r].hit, host[r].hit, "round {r}: both accept or both reject")
        t |> equal(home[r].n_past, host[r].n_past, "round {r}: both stand at one position")
        agreed++
    }
    to_log(LOG_INFO, "the device-home session rounds as the host-cached session for {agreed} of {long_length(host)} rounds before the drafts part\n")
}

//! what the MTP IQ4_XS cut's forms collect for the compares after the
//! block: the census form's feed and its IQ4_XS counts, the per-op
//! form's feed and its rails' reads, the CPU form's feed of the same
//! prompt, and the regions cells' readings
struct private Iq4xsRows {
    census : Held
    census_ran : bool
    census_gpu : FeedRows
    nrec : int64
    pf_tile : int64
    st_gemv : int64
    pf_emb : int64
    st_rows : int64
    census_read : bool
    gathers : bool
    perop : Held
    perop_ran : bool
    perop_gpu : FeedRows
    cpu : FeedRows
    cpu_ran : bool
    chain : Held
    groups : Held
    leaf : Held
}

//! the census cell's device half, the load's first feed: every site's tag, no layer declined, the untied IQ4_XS table, a
//! window and the fed steps with the IQ4_XS tile, GEMV and table gather counted
def private iq4xs_census_rows(t : T?; var r : Iq4xsRows; m : Model; form : ModelForm) {
    cut_held(r.census, m, QWEN35MOE_BLOCKS, "qwen35moe")
    census_cell(t, m, path_join(models_dir(), CENSUS35.file), "vulkan load", census_sites_all())
    for (l in range64(m.config.n_layers)) {
        r.census |> held_equal(resident_layer_decline(m, l), "", "layer {l} draws no decline from the whole-model driver")
    }
    return if (moe_fixture_admitted(r.census, m, CENSUS35) < 0l)
    r.census |> held_equal(gpu_resident_decline_(), "", "the whole-model driver armed: no decline reason")
    r.nrec = recurrent_layer_count(m.config)
    r.census |> held_fact(!m.config.shared_weights && emb_kq(m) && m.emb_fmt == KqFmt.iq4xs, "the file's token table is untied and kept as its own IQ4_XS plane ({m.emb_fmt})")
    var inscope prompt <- resident_prompt(Q35_PROMPT)
    var inscope toks <- resident_feed(Q35_STEPS)
    let tile0 = census("kq_batch_iq4xs_cls_spv")
    let emb0 = census(emb_kernel(KqFmt.iq4xs))
    var gemv0 = 0l
    var rows0 = 0l
    r.gathers = gpu_resident_embed_gathers()
    r.census_gpu <- feed_rows(m, form, prompt, toks) $ {
        r.pf_tile = census("kq_batch_iq4xs_cls_spv") - tile0
        r.pf_emb = census(emb_kernel(KqFmt.iq4xs)) - emb0
        gemv0 = census_sum(["kq_gemv_iq4xs_cls_spv", "kq_gemv_iq4xs_n_cls_spv"])
        rows0 = gathered_rows()
    }
    r.st_gemv = census_sum(["kq_gemv_iq4xs_cls_spv", "kq_gemv_iq4xs_n_cls_spv"]) - gemv0
    r.st_rows = gathered_rows() - rows0
    r.census_read = tile0 != NO_CENSUS
    r.census |> held_served()
    r.census_ran = true
}

def private iq4xs_census_within(t : T?; r : Iq4xsRows) {
    unsafe {
        t |> run("{CENSUS35.file}: every site's tag is the file's format, the whole-model driver armed, and its window and steps hold the CPU chain on the IQ4_XS kernels") @capture(& r)(t : T ?) {
            held_within(t, r.census)
            return if (!r.census_ran)
            if (r.cpu_ran) {
                var inscope c <- feed_prefix(r.cpu, Q35_STEPS)
                feed_steps_within(t, "IQ4_XS census ", Q35_PROMPT, r.census_gpu.rows, c.rows, r.census_gpu.toks,
                        CENSUS35.bar_rel, "resident", "the CPU chain") $(id) {
                    return feed_piece(r.census_gpu, c, id)
                }
            } else {
                t |> failure("IQ4_XS census, the window and steps against the CPU chain: the CPU form ran no feed")
            }
            if (r.census_read) {
                t |> success(r.pf_tile >= 2l * r.nrec, "the window read every recurrent layer's beta and alpha planes through the IQ4_XS tile ({r.pf_tile} dispatches, {2l * r.nrec} planes)")
                t |> success(r.st_gemv >= 5l * r.nrec, "the token command read every recurrent layer's five planes through the IQ4_XS GEMV ({r.st_gemv} recorded, {5l * r.nrec} planes)")
                if (r.gathers) {
                    t |> success(r.pf_emb >= 1l, "the window gathered its token rows off the IQ4_XS table on the device ({r.pf_emb} dispatches)")
                    t |> equal(r.st_rows, Q35_STEPS, "every fed step's row gathered on the device off its token id, no host row uploaded")
                } else {
                    t |> equal(r.st_rows, 0l, "the table's plane stayed on the host (the load log names the room): no step row gathered on the device")
                }
            }
        }
    }
}

//! the per-op cell's device half: every recurrent layer's deltanet planes and every layer's shared expert on the per-op
//! rails in the file's own formats, a window and the fed steps with the rails' scans, steps, tiles and shared-expert rows
def private iq4xs_perop_rows(var r : Iq4xsRows; m : Model; form : ModelForm) {
    cut_held(r.perop, m, QWEN35MOE_BLOCKS, "qwen35moe")
    let nrec = recurrent_layer_count(m.config)
    var marked = 0l
    for (l in range64(m.config.n_layers)) {
        continue if (!layer_is_recurrent(m.config, l))
        r.perop |> held_equal(perop_dn_decline(m, l), "", "layer {l}'s deltanet planes are the per-op chain's formats")
        marked += moe_gpu_dn_on_gpu(l) ? 1l : 0l
    }
    r.perop |> held_equal(marked, nrec, "the per-op deltanet rail holds every recurrent layer on the device ({marked} of {nrec}) - none left on the CPU rail")
    var shexp_native = 0l
    for (l in range64(m.config.n_layers)) {
        shexp_native += moe_gpu_shexp_on_gpu(l) && perop_shexp_native(m, l) ? 1l : 0l
    }
    r.perop |> held_equal(shexp_native, m.config.n_layers, "the per-op rail holds every layer's shared expert in the file's own format ({shexp_native} of {m.config.n_layers}) - no q8 transcode")
    let sh_pf0 = shexp_gpu_prefill_layers()
    let sh_ovl0 = shexp_gpu_decode_rows()
    let sh_gemm0 = shexp_gpu_gemm_rows()
    let sh_aft0 = shexp_gpu_after_rows()
    let sh_dec0 = sh_ovl0 + sh_aft0 + shexp_gpu_gemm_rows() + span_tokens()
    var inscope prompt <- resident_prompt(Q35_PROMPT)
    var inscope toks <- resident_feed(Q35_STEPS)
    let scan0 = census("dn_scan_cls_spv")
    let step0 = census("dn_step_cls_spv")
    let tile0 = census("kq_batch_iq4xs_cls_spv")
    r.perop_gpu <- feed_rows(m, form, prompt, toks)
    let scan = census("dn_scan_cls_spv") - scan0
    let step = census("dn_step_cls_spv") - step0
    let tile = census("kq_batch_iq4xs_cls_spv") - tile0
    if (scan0 != NO_CENSUS) {
        r.perop |> held_fact(scan >= nrec, "the per-op window chain scanned every recurrent layer on the device ({scan} of {nrec})")
        r.perop |> held_fact(tile >= 3l * nrec, "its qkv, z and out GEMMs read the IQ4_XS stacks ({tile} dispatches, {3l * nrec} planes)")
        r.perop |> held_fact(step >= nrec, "the per-op decode step recorded every recurrent layer's chain ({step} of {nrec})")
    }
    r.perop |> held_fact(shexp_gpu_prefill_layers() - sh_pf0 >= m.config.n_layers, "the window ran every layer's shared expert on the device over its own planes ({shexp_gpu_prefill_layers() - sh_pf0} layers)")
    let sh_dec = shexp_gpu_decode_rows() + shexp_gpu_after_rows() + shexp_gpu_gemm_rows() + span_tokens() - sh_dec0
    r.perop |> held_fact(sh_dec >= Q35_STEPS, "every fed step ran the shared expert on the device - the span or the per-layer chain ({sh_dec} rows)")
    to_log(LOG_INFO, "per-op shared expert rows: overlapped chain {shexp_gpu_decode_rows() - sh_ovl0}, after the routed chain {shexp_gpu_after_rows() - sh_aft0}, dense GEMMs {shexp_gpu_gemm_rows() - sh_gemm0}\n")
    r.perop |> held_equal(shexp_gpu_gemm_rows() - sh_gemm0, 0l, "one activation form a layer: no layer took the dense GEMMs - the overlapped chain or the span serves every one")
    r.perop_ran = true
}

def private iq4xs_perop_within(t : T?; r : Iq4xsRows) {
    unsafe {
        t |> run("{CENSUS35.file} with the whole-model route off: the per-op rails keep every deltanet layer on the device over its IQ4_XS planes, the window and steps against the CPU chain") @capture(& r)(t : T ?) {
            held_within(t, r.perop)
            return if (!r.perop_ran)
            if (!r.cpu_ran) {
                t |> failure("IQ4_XS per-op, the window and steps against the CPU chain: the CPU form ran no feed")
                return
            }
            var inscope c <- feed_prefix(r.cpu, Q35_STEPS)
            feed_steps_within(t, "IQ4_XS per-op ", Q35_PROMPT, r.perop_gpu.rows, c.rows, r.perop_gpu.toks,
                    CENSUS35.bar_rel, "per-op", "the CPU chain") $(id) {
                return feed_piece(r.perop_gpu, c, id)
            }
        }
    }
}

//! the verify-rows cell on a two-region form: one round's two verify rows against two one-row steps, then four rounds'
//! committed stream against the same tokens stepped
def private verify_rows_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("{CENSUS35.file}: the verify's rows through the split command's segments hold the one-row steps, and a run of rounds commits the stream the steps commit") @capture(& tr)(t : T ?) {
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            t |> success(rdec_verify_rows() >= 2l, "the verify seat holds two rows or more (the load log names any decline)")
            var prompt <- resident_prompt(Q35_PROMPT)
            var toks <- resident_feed(Q35_ROUNDS + 1l)
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            let verifies0 = moe_gpu_resident_verifies()
            var vr <- verify_round_rows(tr, s, toks[0])
            t |> equal(moe_gpu_resident_verifies(), verifies0 + 1l, "the round's verify ran on the device")
            let a0 = argmax_of(vr.logits0)
            let a1 = argmax_of(vr.logits1)
            to_log(LOG_INFO, "35B round: draft {vr.draft} '{decode_(tr, [vr.draft])}', row 0 argmax {a0} '{decode_(tr, [a0])}' ({a0 == vr.draft ? "accepts" : "rejects"}), row 1 argmax {a1} '{decode_(tr, [a1])}', accepted {vr.accepted}\n")
            t |> equal(vr.accepted, a0 == vr.draft ? 1l : 0l, "the round accepted the draft exactly where row 0's argmax is the draft")
            var sd <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, sd, prompt)
            let verifies1 = moe_gpu_resident_verifies()
            forward(tr, sd, toks[0], Q35_PROMPT)
            var sl0 <- [for (x in sd.logits); x]
            var sh0 <- [for (x in sd.mtp_h); x]
            forward(tr, sd, vr.draft, Q35_PROMPT + 1l)
            var sl1 <- [for (x in sd.logits); x]
            var sh1 <- [for (x in sd.mtp_h); x]
            t |> equal(moe_gpu_resident_verifies(), verifies1, "control: the two one-row steps run no verify - the counter stands")
            row_within_ctrl(t, "verify row 0 against the split command's one-row step", vr.logits0, sl0, sl1, CENSUS35.bar_rel, "(the step's row 1 as the control)")
            row_within_ctrl(t, "verify row 1 against the split command's one-row step", vr.logits1, sl1, sl0, CENSUS35.bar_rel, "(the step's row 0 as the control)")
            row_within_ctrl(t, "verify carry row 0 against the split command's one-row step", vr.carry0, sh0, sh1, CENSUS35.bar_rel, "(the step's carry row 1 as the control)")
            row_within_ctrl(t, "verify carry row 1 against the split command's one-row step", vr.carry1, sh1, sh0, CENSUS35.bar_rel, "(the step's carry row 0 as the control)")
            delete sd
            delete s
            var sr <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, sr, prompt)
            var fed <- [for (i in range64(Q35_ROUNDS)); toks[i]]
            var committed : array<int64>
            var rounds <- committed_rounds(tr, sr, fed, 4l, committed)
            var ss <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, ss, prompt)
            var steps <- stepped(tr, ss, committed)
            var off <- stepped(tr, ss, [toks[Q35_ROUNDS]])
            let ar = argmax_of(rounds.logits)
            let ast = argmax_of(steps.logits)
            to_log(LOG_INFO, "35B rounds committed {long_length(committed)} tokens '{decode_(tr, committed)}'; next argmax rounds {ar} '{decode_(tr, [ar])}', steps {ast} '{decode_(tr, [ast])}'\n")
            t |> equal(sr.n_past, ss.n_past - 1l, "the rounds stand where the steps stand")
            row_within_ctrl(t, "the logits after four rounds against the same stream stepped", rounds.logits, steps.logits, off.logits, CENSUS35.bar_rel, "(the steps one token further as the control)")
            row_within_ctrl(t, "the carry after four rounds against the same stream stepped", rounds.carry, steps.carry, off.carry, CENSUS35.bar_rel, "(the steps' carry one token further as the control)")
            served_every_call(t)
            delete off
            delete steps
            delete ss
            delete rounds
            delete committed
            delete sr
            delete fed
            delete vr
            delete prompt
            delete toks
            set_mtp_spec(spec0)
        }
    }
}

//! the NextN cell on a two-region form with the planes on the host: the head's prompt rows warmed by the window chain, one
//! device draft against the CPU head's, six rounds drafted and verified on the device, a device-home session's rounds
//! against the host-cached session's
def private nextn_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("{CENSUS35.file}: the routed NextN head on the device - its prompt rows warmed by the window chain, its draft against the CPU head's, every speculative round drafted and verified there, and a device-home session's rounds token for token the host-cached session's") @capture(& tr)(t : T ?) {
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            t |> equal(tr.config.n_layer_nextn, 1l, "the -MTP- file carries its NextN head")
            t |> success(layer_is_moe(tr, tr.config.n_layers), "the head's FFN is routed")
            t |> success(rdec_verify_rows() >= 2l, "the routed head rides the driver: its verify seat holds two rows or more (the load log names any decline)")
            t |> success(gpu_resident_mtp_device_home_(tr), "every seat of the round runs on the device, so a device-home session speculates")
            var prompt <- resident_prompt(Q35_PROMPT)
            var toks <- resident_feed(Q35_ROUNDS + 1l)
            var fed <- [for (i in range64(Q35_ROUNDS)); toks[i]]
            var s <- create_session(tr, mirror_kv_dtype())
            prefill_warm(tr, s, prompt)
            t |> equal(moe_gpu_resident_head_rows(s), Q35_PROMPT - 1l, "the window chain warmed the head's {Q35_PROMPT - 1l} prompt rows on the device")
            var h0 <- [for (x in s.mtp_h); x]
            let row = Q35_PROMPT - 1l
            let drafts0 = moe_gpu_resident_drafts()
            var dev <- draft_from(tr, s, toks[0], row, h0)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the routed head drafted on the device")
            let seat = unset_rdec_draft()
            var cpu <- draft_from(tr, s, toks[0], row, h0)
            var ctrl <- draft_from(tr, s, toks[Q35_ROUNDS], row, h0)
            reinstall_rdec_draft(seat)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the unset seat left both CPU drafts to forward_mtp")
            row_within_ctrl(t, "the routed head's draft", dev.logits, cpu.logits, ctrl.logits, CENSUS35.bar_rel, "(the CPU draft of another token as the control)")
            pick_agrees(t, tr, "the routed head's draft", argmax_of(dev.logits), cpu.logits, CENSUS35.bar_rel)
            copy_floats(h0, 0l, s.mtp_h, 0l, long_length(h0))
            s.mtp_h_pos1 = Q35_PROMPT
            let verifies0 = moe_gpu_resident_verifies()
            var host <- spec_rounds(tr, s, fed)
            t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l + Q35_ROUNDS, "every round drafted on the device")
            t |> equal(moe_gpu_resident_verifies(), verifies0 + Q35_ROUNDS, "every round verified on the device")
            t |> equal(moe_gpu_resident_head_rows(s), s.n_past - 1l, "the region holds the head's rows through the session's position less one")
            var sd <- create_device_session(tr, mirror_kv_dtype())
            t |> success(sd.device_kv, "the second session is device-home: its K/V lives in the driver's region alone")
            prefill_warm(tr, sd, prompt)
            t |> equal(moe_gpu_resident_head_rows(sd), Q35_PROMPT - 1l, "the device-home session's head rows warmed on the device, with no host copy")
            let drafts1 = moe_gpu_resident_drafts()
            let verifies1 = moe_gpu_resident_verifies()
            var home <- spec_rounds(tr, sd, fed)
            t |> equal(moe_gpu_resident_drafts(), drafts1 + Q35_ROUNDS, "every device-home round drafted on the device")
            t |> equal(moe_gpu_resident_verifies(), verifies1 + Q35_ROUNDS, "every device-home round verified on the device")
            rounds_agree(t, tr, host, home, CENSUS35.bar_rel)
            served_every_call(t)
            delete sd
            delete home
            delete host
            delete s
            delete dev
            delete cpu
            delete ctrl
            delete h0
            delete fed
            delete prompt
            delete toks
            set_mtp_spec(spec0)
        }
    }
}

//! the one-region form's cells: the census first (its feed the load's first, which records the token command), then
//! with speculation off the greedy chain and the short tail's peel
def private iq4xs_one_region_rows(t : T?; var r : Iq4xsRows; var m : Model; form : ModelForm; census_armed : bool) {
    if (census_armed) {
        iq4xs_census_rows(t, r, m, form)
    }
    let spec0 = get_mtp_spec()
    set_mtp_spec(false)
    greedy_chain_rows(r.chain, m, CENSUS35.file)
    unsafe {
        t |> run("a short tail peels through the token commands, the recurrent rows in order in the region's slot, the routed block every row, and reads what the one-row steps read") @capture(& m)(t : T ?) {
            recurrent_peel_cell(t, m, MOE_LOGIT_BAR_REL)
        }
    }
    set_mtp_spec(spec0)
}

//! one form's collection of the MTP IQ4_XS cut's cells into `r`;
//! `census_armed` whether the vk-census arm's cells run
def private iq4xs_rows(t : T?; var r : Iq4xsRows; var m : Model; form : ModelForm; census_armed : bool) {
    if (!form.on_gpu) {
        delete r.cpu
        r.cpu <- resident_feed_rows(m, form, Q35_PROMPT, Q35_STEPS)
        r.cpu_ran = true
    } elif (form.perop) {
        iq4xs_perop_rows(r, m, form)
    } elif (form.regions == 1l) {
        iq4xs_one_region_rows(t, r, m, form, census_armed)
    } elif (form.regions == WIDE_ROWS) {
        regions_groups_rows(r.groups, m, WIDE_ROWS, LogitsBar.wide_bar)
        regions_leaf_rows(r.leaf, m, WIDE_ROWS)
    } elif (form.pf_window_rows == SEG_WINDOW) {
        unsafe {
            t |> run("segmented windows with the routed block over every row hold the routed bar of solo windows, and checkpoint stops captured inside one call adopt and continue at their rows") @capture(& m)(t : T ?) {
                recurrent_segments_cell(t, m)
            }
        }
    } elif (form.host) {
        nextn_cell(t, m)
    } else {
        if (census_armed) {
            verify_rows_cell(t, m)
        }
        regions_continues_cell(t, m, MOE_LOGIT_BAR_REL)
        regions_pool_cell(t, m)   //! the pool last: its streams leave parked claims behind
    }
}

[test(description = "On Qwen3.6-35B-A3B-4L-MTP IQ4_XS-pure, the four-block cut with its NextN block: every site tagged with the file's own format and the whole-model driver armed with no layer decline, its forced-feed steps within 20 percent of the CPU chain on the IQ4_XS tile, GEMV and table gather; the greedy chain forced on and adaptive bit for bit the plain walk's and a short tail peeled within the routed bar of one-row steps, speculation off; a device verify accepting exactly where row 0's argmax is the draft, its rows within 20 percent of one-row steps and four rounds landing the stream stepped; a prefill continued past zero within the routed bar of the one-call prefill; the pool's openings taken up as the host prefix cache does; the routed NextN head's draft within 20 percent of the CPU head's, every round drafted and verified on the device and a device-home session's rounds matching the host-cached session's while drafts agree; segmented windows within the routed bar of solo windows and captured stops adopting at their rows; ten sessions in command groups within the wide bar of each alone and on the CUDA home's rows leaf nearer each alone than its previous step; with the whole-model route off the per-op rails holding every deltanet layer and shared expert in the file's format within 20 percent of the CPU chain; each form loads once")]
def test_gpu_resident_moe_iq4xs(t : T?) {
    return if (!family_on(t, "qwen35moe"))
    var r : Iq4xsRows
    let why = arm_why("vk-census")
    let census_armed = empty(why)
    if (!census_armed) {
        r.census |> held_cell_skip("the census, verify-rows, NextN and per-op cells: {why}")
    }
    var forms <- [device_form(regions = 1l, ctx = REGIONS_RIG_CTX), device_form(regions = 2l, ctx = REGIONS_RIG_CTX),
        device_form(regions = 2l, pf_window_rows = SEG_WINDOW), device_form(regions = WIDE_ROWS, ctx = REGIONS_RIG_CTX)]
    if (census_armed) {
        forms |> push(device_form(regions = 2l, host = true))
        let perop_why = vulkan_home_why("the per-op cell - the per-op rails' cells are the Vulkan home's")
        if (!empty(perop_why)) {
            r.perop |> held_cell_skip(perop_why)
        } else {
            forms |> push(device_form(perop = true))
        }
    }
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.6-35B-A3B-4L-MTP-IQ4_XS-pure-local.gguf", REGIONS_RIG_CTX, forms) $(m, form) {
            iq4xs_rows(t, r, m, form, census_armed)
        }
    }
    if (ran) {
        with_dasllama_jobque_() {
            if (census_armed) {
                iq4xs_census_within(t, r)
                iq4xs_perop_within(t, r)
            } else {
                held_within(t, r.census, "", false)
            }
            unsafe {
                t |> run("the greedy chain forced on reads the plain walk's tokens and logits bit for bit, every on-pick step chained") @capture(& r)(t : T ?) {
                    held_within(t, r.chain)
                }
                t |> run("{WIDE_ROWS} sessions, a region each: a batched step over all of them runs in groups of the command's rows, its GEMVs in column tiles, and every row reads what its session reads alone") @capture(& r)(t : T ?) {
                    held_within(t, r.groups)
                }
                t |> run("{WIDE_ROWS} sessions, a region each, on the rows tensor leaf: a leaf stamp launches and every row reads nearer its session alone than its previous step") @capture(& r)(t : T ?) {
                    held_within(t, r.leaf)
                }
            }
        }
    }
    delete r
    delete forms
}

// Qwen3-30B-A3B's seven-block cuts (`Qwen3-30B-A3B-7L-*-local.gguf`, family `qwen3moe`): 128 experts, 8 routed, no shared
// expert, the renormalized softmax, the residual step's add partner off. Each file is ONE block (`with_cpu_and_gpu_model`).
// - The UD-IQ2_XXS cut (`test_gpu_resident_moe_no_shexp`): the format census - every site the cut carries in its own format
//   (IQ2_XXS / IQ2_S stacks, IQ3_XXS / IQ4_XS downs, Q5_K attention, Q6_K attn_output; the whole file's IQ3_S downs and IQ2_XS
//   stacks sit past block 7) - then the default form's feeds over one window and two against the CPU chain; a tq4 mirror's
//   distance from f16 within 1.15 of the CPU tq4 chain's own; two of its seven routed layers' experts asked onto the host (exact) -
//   the pinned feeds on the grid-format token table's gather; under
//   DASLLAMA_CUDA=1 the CUDA driver's feeds over one window and two.
// - The Q4_K_M cut (`test_regions_batched_steps_q30`): the N-row routed block over K-quant expert stacks fed the rows' Q8_K
//   requant batches two sessions within the wide bar of each alone, at one window and past the unsplit position, the
//   one-token-off control past it - the carrier's K-quant projection leaves take the N-column form, which rounds apart from the
//   one-row command's (the bar's reading is `LogitsBar`'s, `_resident_regions.das`).
// The whole Q4_K_M file's host-layer cells - the room a strict 32768 context leaves forcing the split - stay on it, and the
// stream twin on the whole UD-IQ2_XXS file (`test_gpu_resident_moe_stream_twin_iq`: the cut's prefill row sits past the
// twin's bar), both in the `qwen3moe-30b` category. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, without the file, where
// DASLLAMA_VK_KV32=1 arms the f32 mirror over the tq4 ask, and where the prompt window is not PF_WINDOW_ROWS rows.

let private QWEN3MOE_BLOCKS = 7l
//! of the cut's seven, asserted exact on the load: the whole file's
//! four of forty-eight would put most of the cut on the host
let private QWEN3MOE_HOST_LAYERS = 2l
let private NO_SHEXP_CUT = MoeFixture(file = "Qwen3-30B-A3B-7L-UD-IQ2_XXS-local.gguf", shexp = false,
    bar_rel = MOE_LOGIT_BAR_REL, census = true)
let private NO_SHEXP_HOST = MoeFixture(file = "Qwen3-30B-A3B-7L-UD-IQ2_XXS-local.gguf", shexp = false,
    bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = QWEN3MOE_HOST_LAYERS)

//! the tq4 distance bars (the Qwen1.5 twin's Q/K/V bias steps a tq4 ask down to q8_0, so it carries the serving-rule cell
//! instead): the device's summed distance from the f16 chain within 1.15 of the CPU tq4 chain's own, and the two tq4 chains
//! within 1.4 of it apart - the router's near-ties part two tq4 chains as far as tq4 parts either from f16
let private MOE_CODEC_DIST_REL = 1.15
let private MOE_CODEC_PAIR_REL = 1.4

[test(description = "On Qwen3-30B-A3B-7L UD-IQ2_XXS, the seven-block cut of a MoE with no shared expert, every site reads in its file format and the routed block alone at the FFN site holds the CPU chain within the routed bar over one and two windows; a tq4 mirror's distance from f16 stays within 1.15 of the CPU tq4 chain's; with two of the seven routed layers' experts on the host the split token command and the cut window chain hold the routed bar over one and two windows ; on the CUDA driver the feeds over one and two windows hold 20 percent of max logit; each form loads once")]
def test_gpu_resident_moe_no_shexp(t : T?) {
    return if (!family_on(t, "qwen3moe"))
    var r = QwenMoeRows()
    qwen_moe_rows_init(r, NO_SHEXP_HOST, QWEN3MOE_BLOCKS, "qwen3moe", 0l)
    r.tq4_distance = true
    r.host_exact = true
    r.host.twin = false   //! the stream twin is the whole file's (`test_gpu_resident_moe_stream_twin_iq`)
    var forms <- [device_form()]
    let why = codec_env_why(KVDtype.tq4)
    if (!empty(why)) {
        r.codec |> held_skip("the tq4 mirror's distance cell: {why}")
    } else {
        forms |> push(device_form() |> asking(KVDtype.tq4))
    }
    forms |> push(device_form(regions = 1l, host_layers = QWEN3MOE_HOST_LAYERS))
    let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-30B-A3B-7L-UD-IQ2_XXS-local.gguf", MOE_CUT_CTX, forms) $(m, form) {
        qwen_moe_rows(t, r, m, form, NO_SHEXP_CUT, true)
    }
    if (ran) {
        qwen_moe_rows_within(t, "Qwen3-30B-A3B-7L UD-IQ2_XXS: ", r)
    }
    delete r
    delete forms
}

//! the stream twin on the whole UD-IQ2_XXS file (the qwen3moe-30b category's): on the seven-block cut its 40-token prefill
//! row reads 2.77 against the twin's 2.66 bar with two routed layers on the host (3.21 against 2.61 with four), the steps and
//! the one-step-off controls (17) as on the whole file
let private TWIN_IQ_WHOLE = "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf"

[test(description = "Qwen3-30B UD-IQ2_XXS with four routed layers pinned to the host: the streamed windows' rows sit within the stream twin's bar of the host sums, the device claiming first")]
def test_gpu_resident_moe_stream_twin_iq(t : T?) {
    t |> run("Qwen3-30B-A3B UD-IQ2_XXS with four routed layers' experts pinned to the host: the streamed windows' rows against the host's sums' at the stream twin's bar, the device claiming first") @(ct : T?) {
        return if (!family_on(ct, "qwen3moe"))
        let path = path_join(models_dir(), TWIN_IQ_WHOLE)
        return if (!resident_ready(ct, path))
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            stream_twin_within(ct, path, 4l, 40l, 8l, STREAM_TWIN_BAR_REL, "Qwen3-30B-A3B UD-IQ2_XXS ")
        }
    }
}

[test(description = "On Qwen3-30B-A3B-7L Q4_K_M, the seven-block cut of a MoE with no shared expert, the N-row routed block over K-quant stacks fed Q8_K batches two sessions within the wide bar of each alone, at one window and past the unsplit position, the one-token-off control past it; the form loads once")]
def test_regions_batched_steps_q30(t : T?) {
    return if (!family_on(t, "qwen3moe"))
    var h : Held
    let forms <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-30B-A3B-7L-Q4_K_M-local.gguf", REGIONS_RIG_CTX, forms, false) $(m, _form) {
        cut_held(h, m, QWEN3MOE_BLOCKS, "qwen3moe")
        regions_batched_rows(h, m, LogitsBar.wide_bar)
        regions_split_rows(h, m, LogitsBar.wide_bar)
    }
    if (ran) {
        held_within(t, h, "Qwen3-30B-A3B-7L Q4_K_M, the regions rig: ", false)
    }
    delete h
}

//! 32768 positions asked whole: a 16 GB card cannot hold every expert stack of this 17 GB file beside the mirror, so the first routed layers' go to the host
let private Q30_HOST_SPLIT = MoeFixture(file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 32768l, census = true)

//! the same load asking 30 routed layers onto the host - more than the room forces: the ask raises the plan's own count
let private Q30_HOST_SPLIT_ASKED = MoeFixture(file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 32768l, host_layers_ask = 30l)

//! 131072 positions asked whole: on a 16 GB card the mirror leaves room for the dense planes and most of the expert stacks, so the first routed layers' go to the host
//! the whole file (the qwen35moe-35b category): the cut fits any card whole
let private Q35_CTX_STRICT = MoeFixture(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 131072l, census = true)

//! the 512-expert qwen3next hybrid (48.5 GB): no card the rig stocks holds its expert stacks whole, so the driver arms with the
//! first routed layers' experts on the host at a context asked whole
let private CODER_NEXT = MoeFixture(file = "Qwen3-Coder-Next-Q4_K_M.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 8192l)

[test(description = "The 512-expert Qwen3-Coder-Next arms the whole-model driver with host-layer experts and its device select past 256 experts holds the CPU chain within the routed bar over one window")]
def test_gpu_resident_moe_512_experts(t : T?) {
    t |> run("Qwen3-Coder-Next-Q4_K_M: the 512-expert hybrid on the whole-model driver - the device select past 256 experts, the host-layer split, one window against the CPU chain") @(t : T?) {
        moe_forced_feed(t, 40l, CODER_NEXT)
    }
}

// The whole Q4_K_M file's forms, loaded once each (`with_cpu_and_gpu_model`): one region at 32768 positions asked whole,
// so the first routed layers' experts serve on the host and the window chain is cut at them - the routed feeds over one window
// and two against the CPU chain and the two-token prompt; the same with 30 routed layers asked onto the host; an f16 mirror
// asked at the same context - layer 0's stored rows after a 40-token prompt and the fed steps against the CPU cache's, through
// the codec rig's stored-rows cell, the instrument that says whether the cut window chain and the split token command write
// the mirror the decode reads.

//! what the whole Q4_K_M file's forms collect: the host-split form's admission and census (`carrier`) and its feeds of the
//! prompts, the 30-layer ask's admission (`asked_h`) and feed, the CPU form's feeds on the main codec, layer 0's stored rows
struct private Q30HostRows {
    prompts : array<int64>
    carrier : Held
    feeds : array<FeedRows>
    asked_h : Held
    asked : array<FeedRows>
    cpu : array<FeedRows>
    rows : CodecSides
    rows_dev : bool
    rows_host : bool
}

//! the host-split form: the admission, the census, each prompt's routed feed (the census bound to the first), then the
//! two-token prompt's cell on the same load
def private q30_split_rows(t : T?; var r : Q30HostRows; var m : Model; form : ModelForm) {
    let host_layers = moe_fixture_admitted(r.carrier, m, Q30_HOST_SPLIT)
    if (host_layers >= 0l) {
        var inscope any_sites, bound : array<string>
        census_cell(t, m, path_join(models_dir(), Q30_HOST_SPLIT.file), "vulkan load", any_sites, bound)
        moe_prompt_feeds(r.feeds, r.carrier, m, form, Q30_HOST_SPLIT, r.prompts, host_layers)
    }
    unsafe {   //! the cell captures the loaded model by reference
        t |> run("Qwen3-30B-A3B-Q4_K_M with host layers: a two-token prompt takes the window chain, not the token command's peel") @capture(& m)(t : T ?) {
            if (moe_fixture_admitted(t, m, Q30_HOST_SPLIT) > 0l) {
                assert_short_window_takes_chain(t, m, "routed experts on the host")
            }
        }
    }
}

//! the f16-asked form: layer 0's stored rows off the device after the 40-token prompt and the fed steps
def private q30_stored_rows(t : T?; var r : Q30HostRows; m : Model) {
    if (mirror_kv_dtype() == KVDtype.f32) {
        t |> skip("DASLLAMA_VK_KV32=1 arms the f32 mirror over the {KVDtype.f16} asked - a codec cell needs it unset")
        return
    }
    t |> equal(mirror_kv_dtype(), KVDtype.f16, "the mirror armed at the asked codec")
    codec_stored_device_rows(t, r.rows, m, KVDtype.f16)
    r.rows_dev = true
}

//! one form's collection into `r`
def private q30_host_rows(t : T?; var r : Q30HostRows; var m : Model; form : ModelForm) {
    if (!form.on_gpu) {
        if (empty(r.cpu)) {
            r.cpu |> reserve(length(r.prompts))
            for (n in r.prompts) {
                r.cpu |> emplace <| resident_feed_rows(m, form, n, FED_STEPS)
            }
        }
        if (r.rows_dev && form.kv == KVDtype.f16 && !r.rows_host) {
            r.rows.stored_host <- codec_stored_host(m, KVDtype.f16)
            r.rows_host = true
        }
    } elif (form.kv_asked) {
        q30_stored_rows(t, r, m)
    } elif (form.host_layers > 0l) {
        let host_layers = moe_fixture_admitted(r.asked_h, m, Q30_HOST_SPLIT_ASKED)
        if (host_layers >= 0l) {
            r.asked |> emplace <| moe_feed_rows(r.asked_h, m, form, Q30_HOST_SPLIT_ASKED, 40l, FED_STEPS, host_layers, true)
        }
    } else {
        q30_split_rows(t, r, m, form)
    }
}

//! the whole file's compares after its block: each form's routed feeds within the routed bar of the CPU form's, layer 0's
//! stored rows against the CPU cache's
def private q30_host_rows_within(t : T?; r : Q30HostRows) {
    held_within(t, r.carrier, "Qwen3-30B-A3B-Q4_K_M, the host split: ", false)
    held_within(t, r.asked_h, "Qwen3-30B-A3B-Q4_K_M, 30 routed layers asked onto the host: ", false)
    with_dasllama_jobque_() {
        feeds_within(t, "Qwen3-30B-A3B-Q4_K_M, the host split, ", r.feeds, r.cpu, Q30_HOST_SPLIT.bar_rel)
        for (f in r.asked) {
            let what = "Qwen3-30B-A3B-Q4_K_M, 30 routed layers asked onto the host, {f.nprompt} tokens: "
            continue if (!twin_collected(t, 0, length(r.cpu), what))
            feed_rows_within(t, what, f, r.cpu[0], Q30_HOST_SPLIT_ASKED.bar_rel)
        }
    }
    if (r.rows_dev) {
        t |> success(r.rows_host, "the CPU form read layer 0's stored rows on the f16 sessions the device form's rows answer")
    }
    if (r.rows_dev && r.rows_host) {
        codec_stored_cell(t, "Qwen3-30B-A3B-Q4_K_M at a strict 32768 context with host-layer experts: layer 0's f16 mirror rows against the CPU cache's", KVDtype.f16, r.rows, ROWS_BARS)
    }
}

//! the device prefill's f16 K/V rows read 0.006 (K) and 0.013 (V) of the CPU rows' magnitude from them, a row held to the next 0.68
let private ROWS_BARS = StoredRowsBars(k = 0.02, v_step = 0.05, v_prefill_of_ctrl = 0.6, f16_dist = 0.05)

[test(description = "Qwen3-30B Q4_K_M at 32768 context serves the experts the card cannot hold on the host, the split token command and cut window holding the CPU chain over one and two windows; a 30-layer ask raises the host count; on a MoE whose experts serve on the host a two-token prompt runs through the window chain, because the token command's short-tail peel declines host experts; with host-layer experts at a strict 32768 context layer 0's f16 mirror rows written by the cut window chain and split token command sit within 2% (K) and 5% (V) of the CPU cache's; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token with its reference-prefill arm; each form loads once")]
def test_gpu_resident_moe_host_layers(t : T?) {
    return if (!family_on(t, "qwen3moe"))
    var r : Q30HostRows
    r.prompts <- window_prompts(r.carrier)
    var forms <- [device_form(regions = 1l, strict_ctx = true), device_form(regions = 1l, host_layers = Q30_HOST_SPLIT_ASKED.host_layers_ask, strict_ctx = true),
        device_form(strict_ctx = true) |> asking(KVDtype.f16)]
    let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", Q30_HOST_SPLIT.strict_ctx, forms, parity = true) $(var m, form) {
        q30_host_rows(t, r, m, form)
    }
    if (ran) {
        q30_host_rows_within(t, r)
    }
    delete r
    delete forms
}

[test(description = "At a 131072 context asked whole the Qwen3.6-35B hybrid's experts yield to the mirror and serve on the host, and the chain holds the CPU chain within the routed bar over two windows")]
def test_gpu_resident_moe_ctx_strict(t : T?) {
    t |> run("Qwen3.6-35B-A3B-UD-IQ2_XXS at a context asked whole (131072): the experts yield to the mirror and serve on the host, the hybrid chain against the CPU over two windows") @(t : T?) {
        assert_pf_window(t)
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, Q35_CTX_STRICT)
    }
}

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU-form arm prefills its shared expert on the CPU by design
}

// The shared expert's prefill on the device: a qwen2moe layer's shexp triple rides the routed
// experts' chain as one region over every position (ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain). The
// device chain and the CPU form are not bit-identical (the f16 gather, f32 accumulation order),
// so the gate is the forced-feed logits-tolerance form over the tier's in-process route lever:
// the same prompt and fed tokens with the route ON (the device arm) and OFF (the CPU form), the
// logits within a bar of the CPU arm after the prefill and at every step, the one-step-off
// control past it, and the engage witness - the arm's layer count grows by the model's layer
// count per prefill on the device arm and not at all on the CPU arm. The whole-model driver is
// pinned off for the load: it would take the twin whole (the blocks above are its gate). The carrier is the whole Q4_K_M
// file, the `qwen15-moe` category's: on the four-block cut the decode span serves no fed step and the prefill row sits past
// the bar.

let private SHEXP_TWIN = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf"   //! the Q4_K_M mint of the Q8_0 carrier (its recipe row in ../performance/model_specs.das)

//! of the step's max |logit|. The arms agree to 1e-8 at layer 0 and part from layer 1 on: the device chain's
//! summation order moves the residual by ulps, the next layer's Q8 requant rounds a few elements the
//! other way and the router flips a near-tie, and 24 layers of that read 0.05-0.10 on the twin under both
//! tile modes (the routed chain is bit-stable session to session; the shared expert's rows themselves sit
//! within 1e-4 of the CPU form on the sdot4 feed and 0.5% on the f16 feed - test_vulkan_moe_cm2's cells)
let private SHEXP_LOGIT_BAR_REL = 0.12
let private SHEXP_FED_STEPS = 4l

//! one prompt of `n` tokens plus the fed steps through both arms of the route lever
def private shexp_forced_feed(t : T?; n : int64) {
    return if (!family_on(t, "qwen2moe"))
    let path = path_join(models_dir(), SHEXP_TWIN)
    return if (!jit_model_available(t, path) || !gpu_tier_armed(t))
    let route0 = gpu_resident_route_lever()
    set_gpu_resident_route(false)   //! the per-op rails' claims: the whole-model driver would take the twin (the blocks above hold that arm)
    var tr <- load_model_(path, QuantMode.q8)
    set_gpu_resident_route(route0)   //! the lever reads at the load alone
    t |> success(!moe_gpu_resident_active(), "the route lever kept the whole-model driver off the twin")
    t |> success(tr.config.n_ff_shexp > 0l && !tr.config.moe_dense_shexp, "the twin carries a gated shared expert")
    let layers = tr.config.n_layers
    var marked = 0l
    for (l in range64(layers)) {
        if (moe_gpu_shexp_on_gpu(l)) {
            marked++
        }
    }
    if (marked != layers) {
        t |> skip("the shared experts are resident on {marked} of {layers} layers - the tier's budget or the shexp mark declined (the load log says which)")
        delete tr
        return
    }
    with_dasllama_jobque_() {
        var prompt <- resident_prompt(n)
        var toks <- resident_feed(SHEXP_FED_STEPS)
        set_moe_gpu_shexp_route(true)
        let served0 = shexp_gpu_prefill_layers()
        let span0 = span_tokens()
        var dev <- feed_alone(tr, KVDtype.f16, prompt, toks)
        t |> equal(shexp_gpu_prefill_layers(), served0 + layers, "the device arm served the shared expert of every layer over the {n}-token prompt")
        t |> equal(span_tokens(), span0 + SHEXP_FED_STEPS, "the decode span served every fed step on the device arm (the shared expert inside it)")
        set_moe_gpu_shexp_route(false)
        var cpu <- feed_alone(tr, KVDtype.f16, prompt, toks)
        set_moe_gpu_shexp_route(true)   //! the tier as found
        t |> equal(shexp_gpu_prefill_layers(), served0 + layers, "the CPU arm served no shared expert on the device")
        t |> equal(span_tokens(), span0 + SHEXP_FED_STEPS, "the CPU arm's steps took the per-layer path - the span declines a shared expert off the device")
        feed_steps_within(t, tr, "", n, dev, cpu, toks, SHEXP_LOGIT_BAR_REL, "the device shared expert", "the CPU form")
        delete dev
        delete cpu
        delete prompt
        delete toks
    }
    delete tr
}

[test(description = "On Qwen1.5-MoE Q4_K_M with the per-op tier, the shared expert prefilled on the device serves every layer and keeps forced-feed logits within 12 percent of max logit of the CPU form over 64- and 600-token prompts, with a one-step-off control red")]
def test_gpu_moe_shexp_prefill(t : T?) {
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the shared expert's prefill on the device vs the CPU form, one 64-token prompt") @(t : T?) {
        shexp_forced_feed(t, 64l)
    }
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the same over a 600-token prompt (the chain's chunking)") @(t : T?) {
        shexp_forced_feed(t, 600l)
    }
}

// The host-experts form on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the split token command - a
// captured graph a segment, the host's routed sum between them - and the cut window chain. The 35B NextN cut rides the form
// with its head: the speculative round's draft and verify over the segments, the hot expert pool's hit chains serving picks.
// The Qwen1.5-MoE cuts' host and pool cells ride the blocks above, gpt-oss-4L's the one of
// `test_gpu_resident_gptoss.das`. The forced-feed form the CUDA files share (`_cuda_feed.das`) at the MoE bar, the MMQ route
// pinned `CUDA_MMQ_WINNERS` for the load. The cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a
// device the driver serves, or without the file under models_dir(); its pool assert skips where the plan armed no pool. The
// meet's asserts run under DASLLAMA_GPU_MOE_SPLIT=1 (the default) alone, and their texts name the knob's value.
//! the first routed layer asked onto the host: the whole file's one of
//! forty, one of the cut's four (the cut fits any card, so the plan
//! moves no more)
let private Q35_HOST_LAYERS = 1l
let private Q35_ROUNDS_HOST = 6l

//! the speculative round on the 35B: every round's draft and verify on the device over the segments, the rollback where a draft
//! misses, the stream decoded; then the same tokens stepped plain with speculation off - no draft, no verify (the counters' control)
[cold_path]
def private mtp_rounds_served(t : T?; tr : Model; file : string) {
    var prompt <- resident_prompt(40l)
    var toks <- resident_feed(Q35_ROUNDS_HOST)
    var s <- create_session(tr, mirror_kv_dtype())
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    forward_prefill(tr, s, prompt, long_length(prompt), 0l)
    s.n_past = long_length(prompt)
    let drafts0 = moe_gpu_resident_drafts()
    let verifies0 = moe_gpu_resident_verifies()
    var ids : array<int64>
    ids |> reserve(Q35_ROUNDS_HOST * 2l)
    for (tok in toks) {
        var acc = 0l
        let hit = mtp_spec_eval(tr, s, tok, acc)
        ids |> push(tok)
        if (hit) {
            ids |> push(s.mtp_vbatch[1])
        }
    }
    set_mtp_spec(spec0)
    to_log(LOG_INFO, "{file}: {Q35_ROUNDS_HOST} rounds committed {length(ids)} tokens: '{decode_(tr, ids)}'\n")
    t |> equal(moe_gpu_resident_drafts() - drafts0, Q35_ROUNDS_HOST, "every round drafted on the device")
    t |> equal(moe_gpu_resident_verifies() - verifies0, Q35_ROUNDS_HOST, "every round verified on the device over the segments")
    served_every_call(t)
    delete s
    var p <- create_session(tr, mirror_kv_dtype())
    set_mtp_spec(false)
    forward_prefill(tr, p, prompt, long_length(prompt), 0l)
    let drafts1 = moe_gpu_resident_drafts()
    let verifies1 = moe_gpu_resident_verifies()
    for (tok, i in toks, count()) {
        forward(tr, p, tok, long_length(prompt) + int64(i))
    }
    set_mtp_spec(spec0)
    t |> equal(moe_gpu_resident_drafts(), drafts1, "control: with speculation off the plain steps drafted nothing")
    t |> equal(moe_gpu_resident_verifies(), verifies1, "control: ... and verified nothing")
    served_every_call(t)
    delete p
    delete ids
    delete toks
    delete prompt
}

//! the 35B host cell's readings: the host form's CUDA feed with the
//! pool's and the stream's witnesses, the CPU form's feed of the prompt
struct private Host35Rows {
    h : Held
    gpu : FeedRows
    gpu_ran : bool
    cpu : FeedRows
    cpu_ran : bool
}

def private host35_rows(t : T?; var r : Host35Rows; var m : Model; form : ModelForm; file : string) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (!form.on_gpu) {
            with_cpu_counters_still(r.h, true) {
                delete r.cpu
                r.cpu <- resident_feed_rows(m, form, 40l, DENSE_FED_STEPS)
            }
            r.cpu_ran = true
            return
        }
        cut_held(r.h, m, QWEN35MOE_BLOCKS, "qwen35moe")
        r.h |> held_equal(gpu_resident_host_layers(), Q35_HOST_LAYERS,
            "exactly the first {Q35_HOST_LAYERS} of the cut's {QWEN35MOE_BLOCKS} routed layers' experts serve on " +
            "the host as asked (set_gpu_host_layers; the card holds the rest)")
        let at = stream_at()
        let p0 = pool_meter()
        r.gpu <- cuda_feed_rows(r.h, m, form, 40l, 1l)
        r.gpu_ran = true
        let p = pool_since(p0)
        let submits = p.submits
        let hits = p.hits
        to_log(LOG_INFO, "{file}: {gpu_resident_hot_slots()} pool slots a layer; the pool launched {submits} hit chains serving {hits} picks; the DMA fill copied {p.fills} experts\n")
        if (gpu_resident_hot_slots() == 0l) {
            r.h |> held_cell_skip("the pool's hit chains: the plan found no room for the pool beside the mirror on this card")
        } else {
            r.h |> held_fact(submits > 0l && hits > 0l, "the pool's hit chains served picks beside the host's sums ({submits} chains, {hits} picks)")
        }
        stream_witness(r.h, file, at, MOE_HOST_FIRST)
        unsafe {
            t |> run("{file}: the NextN head beside the host layer - {Q35_ROUNDS_HOST} speculative rounds, every draft and verify on the device; the same tokens plain with speculation off drafting and verifying nothing") @capture(& m)(t : T ?) {
                mtp_rounds_served(t, m, file)
            }
        }
    }
}

[test(description = "On the CUDA driver Qwen3.6-35B-A3B-4L-MTP UD-Q4_K_M, the four-block cut with its NextN block, with its first routed layer's experts on the host stays within 20 percent of the CPU chain with the hot pool's hit chains serving picks and the stream's meet splitting the experts, and every speculative round drafts and verifies on the device while plain steps draft and verify nothing; each form loads once")]
def test_gpu_resident_moe_host_35b(t : T?) {
    return if (!family_on(t, "qwen35moe") || !cuda_home_asked(t))
    let file = "Qwen3.6-35B-A3B-4L-MTP-UD-Q4_K_M-local.gguf"
    var r : Host35Rows
    let pins = pin_stream(MOE_HOST_FIRST, pf_stream = 1)   //! the meet's host share and the plain steps, read at the load and live
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.6-35B-A3B-4L-MTP-UD-Q4_K_M-local.gguf", MOE_CUT_CTX, [device_form(regions = 1l, host_layers = Q35_HOST_LAYERS)]) $(m, form) {
            host35_rows(t, r, m, form, file)
        }
    }
    restore_stream_pins(pins)
    if (ran) {
        held_within(t, r.h, "{file}: ", false)
        with_dasllama_jobque_() {
            if (r.gpu_ran && r.cpu_ran) {
                feed_rows_within(t, "{file} cuda, the first routed layer on the host, 40 tokens ({cuda_knob()}): ", r.gpu, r.cpu, MOE_LOGIT_BAR_REL)
            }
        }
    }
    delete r
}
