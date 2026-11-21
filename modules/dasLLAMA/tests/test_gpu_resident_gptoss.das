options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _moe_resident   // MoeFixture, moe_prompt_feeds, moe_perop_heat_cell
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX, LogitsBar
require _model_tier   // models_dir
require dasllama/dasllama   // load_model_, create_session, moe_gpu_drop_model
require daslib/fio   // path_join
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_tier   // set_gpu_resident_route, moe_gpu_layer_on_gpu: the per-op cell's route lever and marks
require dasllama/dasllama_gpu_resident   // tier_expert_decline, gpu_resident_host_layers
require ?vulkan dasllama/dasllama_vulkan_prefill   // vk_xf_biased_acts: the Vulkan chain's biased acts, the stream twin's witness on that home
require ?cuda dasllama/dasllama_cuda_stream   // cud_stream_stats().biased: the same on the CUDA home
require ?cuda _cuda_feed   // the CUDA home's own cells' witnesses
require _kv_codec_cells   // codec_env_why: the sink stamps' codec arms
require dasllama/dasllama_kv_dtype   // KVDtype: the codec forms' mirror asks

// gpt-oss-4L, the 20b's first four blocks (the native-MXFP4
// routed stacks on the mx4 expert rail, the biased router's
// softmax over its four picks, the gate / up / down bias rows at
// the act and the combine under the clamped swiglu, the per-head
// attention sinks in the flash tiles and the token command's
// pass, the output bias on the residual step, sliding windows on
// the even layers, YaRN folded at load), every form of its cells
// loaded once (`with_cpu_and_gpu_model`): the default form's
// forced feeds at one window and two, a q8_0 mirror's sink
// stamp, two and three mirror regions for the regions rig, two
// routed layers' experts on the host with the prompt stream on
// and off, and under DASLLAMA_CUDA=1 on a card with the FP4 rung
// the CUDA home's own feeds and the rings against a flat mirror
// at f16 and q8_0, every load under the MMQ route pinned
// `CUDA_MMQ_WINNERS`. -jit only; skips without the file, the
// armed tier, or a device with neither the cm2 tile family nor
// KHR cooperative matrices at subgroup 32 (the expert pieces'
// two tiles); on a device with either the driver's admission is
// asserted - a decline (a seat, the sink arm, a plane's format)
// is a red that sends the reader to the load log. The two-window
// feed skips where the prompt window is not `PF_WINDOW_ROWS`, a
// form whose plan does not fit the card skips loudly (the ring
// forms ask 32768 positions), and under DASLLAMA_CUDA=1 the CUDA
// home's own cells skip on a card without the FP4 rung. The per-op
// cells (the whole-model route off, the heat pool) load on their
// own and skip under DASLLAMA_CUDA=1 - the per-op tier is the
// Vulkan home's. DASLLAMA_VK_KV32=1 arms the f32 mirror on the
// default form, whose feed then carries the f32 sink stamp in
// the q8_0 form's place.
//! gpt-oss's logits bar: the routed chain's noise reads 0.06-0.15 of a 7-15 max logit on the 5060 Ti (0.9-2.2%) at 40 and 600 tokens and
//! every step, the argmax agreeing everywhere - the four picks over 32 experts on a biased softmax sit far from a near-tie on
//! this fixture - while the one-step-off controls read 4.2-18.9, so a 5% bar keeps a 2x margin over the noise and 12x under the control
let private GPTOSS_LOGIT_BAR_REL = 0.05
let private GPTOSS = MoeFixture(file = "gpt-oss-4L-mxfp4-local.gguf", shexp = false, bar_rel = GPTOSS_LOGIT_BAR_REL, window = 128l)   //! the 20b's four-block cut (its row in ../performance/model_specs.das)
//! the heat pool's walk reaches eighty fed steps, and one of them (step 11) lands a router near-tie the pool's summation order
//! flips - 0.447 against the 5% bar's 0.378, the argmax intact - so the heat cell holds the routed block's bar (`MOE_LOGIT_BAR_REL`)
let private GPTOSS_HEAT = MoeFixture(file = "gpt-oss-4L-mxfp4-local.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, window = 128l)

[test(description = "With the whole-model route off, the per-op tier places every gpt-oss-4L routed layer's mx4 stacks on the device, and a 40-token window plus six fed steps sit within the 5% logits bar of the CPU chain")]
def test_gpu_perop_gptoss(t : T?) {
    t |> run("gpt-oss-4L with the whole-model route off: the per-op expert chain takes every routed layer's mx4 stacks under the biased clamped swiglu, the window and steps against the CPU chain") @(t : T?) {
        if (!family_on(t, "gptoss") || !vulkan_home_asked(t, "the per-op tier is the Vulkan home's")) return
        let path = path_join(models_dir(), GPTOSS.file)
        if (!resident_ready(t, path)) return
        let route = gpu_resident_route_lever()
        set_gpu_resident_route(false)
        var tr <- load_model_(path, QuantMode.q8)
        set_gpu_resident_route(route)
        t |> success(!moe_gpu_resident_active(), "the whole-model route is off: the per-op rails serve")
        var placed = 0l
        var routed = 0l
        for (l in range64(tr.config.n_layers)) {
            continue if (!layer_is_moe(tr, l))
            routed++
            t |> equal(tier_expert_decline(tr, l), "", "layer {l}'s mx4 stacks are the expert chain's")
            placed += moe_gpu_layer_on_gpu(l) ? 1l : 0l
        }
        t |> equal(placed, routed, "the per-op expert walk placed every routed layer ({placed} of {routed}) - none left on the CPU")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(40l)
            var toks <- resident_feed(6l)
            var sg <- create_session(tr, mirror_kv_dtype())
            var gpu <- feed_logits(tr, sg, prompt, toks)
            delete sg
            moe_gpu_drop_model()
            var cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
            feed_steps_within(t, tr, "gpt-oss per-op ", 40l, gpu, cpu, toks, GPTOSS.bar_rel, "per-op")
            delete gpu
            delete cpu
            delete prompt
            delete toks
        }
        delete tr
    }
}

[test(description = "On the per-op tier with two gpt-oss-4L routed layers placed and the rest streamed under an eight-slot heat pool, the window and fed steps hold the routed bar of the CPU chain and the pool serves picks")]
def test_gpu_perop_gptoss_heat(t : T?) {
    t |> run("gpt-oss-4L on the per-op tier: two routed layers placed, two streamed under the heat pool's mx4 slots with the biased clamped swiglu, against the CPU chain") @(t : T?) {
        if (!family_on(t, "gptoss")) return
        let placement = moe_perop_heat_cell(t, GPTOSS_HEAT, 2l, 8l)
        if (placement.routed > 0l) {
            t |> equal(placement.placed, 2l,
                "the walk placed two of gpt-oss-4L's {placement.routed} routed layers")
            t |> equal(placement.routed - placement.placed, 2l, "... and streamed the other two")
        }
    }
}

//! the block's mirror context: the q8_0 mirror beside the planes,
//! the regions rig's cap
let private GPTOSS_CTX = 8192l
let private HOST_LAYERS_GPTOSS = 2l   //! the host-layers forms' ask
//! the long feeds' steps: the codec, CUDA and stream feeds' and the
//! CPU form's; the census feeds' `FED_STEPS` a prefix
let private LONG_FED_STEPS = 8l
let private RING_IDS = 1300l   //! prose ids past the 128-key window many times
//! the ring forms' prompt window: a ring of the window plus these
//! rows (640), which the prompt wraps twice
let private RING_WINDOW = 512l
//! the ring forms' context: unpinned, each load fills the card and
//! the two mirrors weigh the same
let private RING_CTX = 32768l

//! the armed home's count of the stream chain's biased acts: the CUDA home's under the CUDA ask, else the Vulkan driver's; -1 on
//! a build with neither home's chain
def private biased_acts() : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_stream_stats().biased if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_xf_biased_acts()
    }
    return -1l
}

//! why the CUDA home's own cells do not run on this box; "" where
//! the CUDA home is asked on a card with the FP4 rung
def private cuda_cells_why() : string {
    let home = cuda_home_why()
    return home if (!empty(home))
    static_if (typeinfo builtin_module_exists(cuda)) {
        return fp4_rung_why()
    } else {
        return "no cuda module in this build"
    }
}

//! what gpt-oss-4L's forms collect: the forced feeds' prompt
//! lengths; the default form beside its CPU form (`main`) - the
//! default form's census feeds (`FED_STEPS` steps), the CUDA home's
//! feeds and the CPU form's feeds of the same prompts, their
//! readings `carrier`; the sink stamp's device and CPU feeds on
//! `codec_kv`; the regions rig; the host-layers forms' feeds with the
//! prompt stream on and off, their readings; the rings at f16 and at
//! q8_0; each `_ran` says a form collected
struct GptossRows {
    prompts : array<int64>
    cuda : bool   //! the CUDA home's own cells collect
    main : MainPair
    carrier : Held
    main_feeds : array<FeedRows>
    cuda_feeds : array<FeedRows>
    cpu_feeds : array<FeedRows>
    codec_kv : KVDtype
    codec_gpu : FeedRows
    codec_cpu : FeedRows
    codec_gpu_ran : bool
    codec_cpu_ran : bool
    regions : Held
    host_layers : Held
    host_layers_kv : KVDtype
    stream_on : FeedRows
    stream_off : FeedRows
    stream_on_ran : bool
    stream_off_ran : bool
    rings : RingRows
}

//! a host-layers form's 40-token feed; on the CUDA home (`on_cuda`)
//! the stream's witnesses held into `h` - the streamed feed through
//! `cuda_feed_rows`, the stream-off feed the control: no window
//! streamed, no expert met
def private stream_rows(var h : Held; m : Model; form : ModelForm; on_cuda : bool) : FeedRows {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (on_cuda) {
            let at = stream_at()
            if (form.pf_stream) {
                var on <- cuda_feed_rows(h, m, form, 40l, prompt_windows(40l))
                stream_witness(h, GPTOSS.file, at, 0l)
                return <- on
            }
            var off <- resident_feed_rows(m, form, 40l, LONG_FED_STEPS)
            stream_off_witness(h, GPTOSS.file, at)
            return <- off
        }
    }
    return <- resident_feed_rows(m, form, 40l, LONG_FED_STEPS)
}

//! a device form's sink-stamp feed: 40 tokens on its codec
def private gptoss_codec_rows(var r : GptossRows; m : Model; form : ModelForm) {
    r.codec_kv = form.kv
    delete r.codec_gpu
    r.codec_gpu <- resident_feed_rows(m, form, 40l, LONG_FED_STEPS)
    r.codec_gpu_ran = true
}

//! the default form: the admission, the census feeds of every
//! prompt, the CUDA home's feeds of the same prompts, and under
//! DASLLAMA_VK_KV32=1 the f32 sink stamp's feed
def private gptoss_main_rows(var r : GptossRows; m : Model; form : ModelForm) {
    r.carrier |> held_equal(m.arch, "gpt-oss", "the file is a gpt-oss-family model")
    let host_layers = moe_fixture_admitted(r.carrier, m, GPTOSS)
    return if (host_layers < 0l)
    moe_prompt_feeds(r.main_feeds, r.carrier, m, form, GPTOSS, r.prompts, host_layers)
    if (r.cuda) {
        r.cuda_feeds |> reserve(length(r.prompts))
        for (n in r.prompts) {
            r.cuda_feeds |> emplace <| cuda_feed_rows(r.carrier, m, form, n, prompt_windows(n))
        }
    }
    if (form.kv == KVDtype.f32) {
        gptoss_codec_rows(r, m, form)
    }
}

//! the CPU form on the default form's codec: the feeds of every
//! prompt, the first under the CUDA counters' control
def private gptoss_cpu_rows(var r : GptossRows; m : Model; form : ModelForm) {
    r.cpu_feeds |> reserve(length(r.prompts))
    for (n, i in r.prompts, count()) {
        with_cpu_counters_still(r.carrier, r.cuda && i == 0) {
            r.cpu_feeds |> emplace <| resident_feed_rows(m, form, n, LONG_FED_STEPS)
        }
    }
}

//! a host-layers form: the feed with the prompt stream on - on the
//! CUDA home the exact host-layer count beside the stream's witnesses
//! - and the chain's biased acts; or the feed with the stream off,
//! the control: on the CUDA home no window streamed and no expert
//! met, on either home no biased act
def private gptoss_host_layers_rows(var r : GptossRows; m : Model; form : ModelForm) {
    let b0 = biased_acts()
    if (form.pf_stream) {
        r.host_layers_kv = form.kv
        if (r.cuda) {
            r.host_layers |> held_equal(gpu_resident_host_layers(), HOST_LAYERS_GPTOSS,
                "exactly the first {HOST_LAYERS_GPTOSS} routed layers' experts serve on the host " +
                "as asked (set_gpu_host_layers; the card holds the rest)")
        }
        delete r.stream_on
        r.stream_on <- stream_rows(r.host_layers, m, form, r.cuda)
        r.stream_on_ran = true
    } else {
        delete r.stream_off
        r.stream_off <- stream_rows(r.host_layers, m, form, r.cuda)
        r.stream_off_ran = true
    }
    if (b0 < 0l) {
        if (form.pf_stream) {
            r.host_layers |> held_skip(
                "no discrete GPU home on this build: the stream chain is the Vulkan and CUDA " +
                "drivers'")
        }
    } elif (form.pf_stream) {
        r.host_layers |> held_fact(biased_acts() > b0,
            "the chain's acts took the routed form's bias rows ({biased_acts() - b0} biased " +
            "acts)")
    } else {
        r.host_layers |> held_equal(biased_acts(), b0,
            "control: with the prompt stream off the chain took no biased act")
    }
}

//! one form's collection of gpt-oss-4L's cells into `r`
def private gptoss_rows(var r : GptossRows; m : Model; form : ModelForm) {
    let side = main_side(r.main, form)
    if (side == MainSide.device) {
        gptoss_main_rows(r, m, form)
    } elif (side == MainSide.cpu) {
        gptoss_cpu_rows(r, m, form)
    } elif (form.pf_window_rows > 0l) {
        ring_rows_into(r.rings, m, form.kv, form.kv == KVDtype.q8_0, form.ring, RING_IDS)
    } elif (form.host_layers > 0l) {
        gptoss_host_layers_rows(r, m, form)
    } elif (form.regions == 2l) {
        regions_batched_once_rows(r.regions, m, LogitsBar.routed_bar)
    } elif (form.regions == 3l) {
        regions_widen_rows(r.regions, m, LogitsBar.routed_bar)
    } elif (form.on_gpu) {
        gptoss_codec_rows(r, m, form)
    } elif (r.codec_gpu_ran && form.kv == r.codec_kv) {
        delete r.codec_cpu
        r.codec_cpu <- resident_feed_rows(m, form, 40l, LONG_FED_STEPS)
        r.codec_cpu_ran = true
    }
}

//! a default-form feed's bar: the fixture's over one window; the
//! routed block's over two - the two-window prompt's first fed step
//! lands a router near-tie on the RTX PRO 4500, 0.458 against the 5%
//! bar's 0.341, the argmax intact, the later steps 0.07 - and on the
//! CUDA home, whose FP4 leaf reads 1.4-1.6 of a 20 peak on the cut
def private main_feed_bar(f : FeedRows) : float {
    return GPTOSS_LOGIT_BAR_REL if (prompt_windows(f.nprompt) == 1l && !gpu_want_cuda())
    return MOE_LOGIT_BAR_REL
}

//! the sink stamp's feeds: the device's against the CPU's at the
//! same codec, the default form's where the two codecs agree
def private gptoss_codec_within(t : T?; r : GptossRows) {
    return if (!r.codec_gpu_ran)
    let tag = "gpt-oss-4L, a {r.codec_kv} mirror - the codec's sink stamp: "
    if (r.codec_cpu_ran) {
        feed_rows_within(t, tag, r.codec_gpu, r.codec_cpu, MOE_LOGIT_BAR_REL)
    } elif (r.codec_kv == r.main.kv && !empty(r.cpu_feeds)) {
        feed_rows_within(t, tag, r.codec_gpu, r.cpu_feeds[0], MOE_LOGIT_BAR_REL)
    }
}

//! the host-layers forms: the streamed windows' rows against the
//! host's sums at the stream twin's bar, and on the CUDA home the
//! stream-on feed against the CPU chain at the routed block's bar
def private gptoss_host_layers_within(t : T?; r : GptossRows) {
    let tag = "gpt-oss-4L, {HOST_LAYERS_GPTOSS} routed layers on the host: "
    held_within(t, r.host_layers, tag, false)
    if (r.stream_on_ran && r.stream_off_ran) {
        let fed = r.stream_on.nprompt + long_length(r.stream_on.toks)
        if (r.stream_on.cuda >= 0l) {
            t |> equal(r.stream_on.cuda, fed,
                "{tag}the CUDA driver served every position of the streamed prompt's load")
        }
        feed_steps_within(t, tag, r.stream_on.nprompt, r.stream_on.rows, r.stream_off.rows,
                r.stream_on.toks, STREAM_TWIN_BAR_REL, "the streamed windows",
                "the host's sums") $(id) {
            return feed_piece(r.stream_on, r.stream_off, id)
        }
    }
    if (r.cuda && r.stream_on_ran && !empty(r.cpu_feeds) && r.host_layers_kv == r.main.kv) {
        feed_rows_within(t, "{tag}cuda ", r.stream_on, r.cpu_feeds[0], MOE_LOGIT_BAR_REL)
    }
}

//! gpt-oss-4L's compares on what its forms collected, each cell
//! where its forms ran
def private gptoss_rows_within(t : T?; r : GptossRows) {
    held_within(t, r.carrier, "gpt-oss-4L: ", false)
    held_within(t, r.regions, "gpt-oss-4L, the regions rig: ", false)
    with_dasllama_jobque_() {
        for (f, i in r.main_feeds, count()) {
            continue if (i >= length(r.cpu_feeds))
            var inscope cpu <- feed_prefix(r.cpu_feeds[i], FED_STEPS)
            let tag = "gpt-oss-4L, {f.nprompt} tokens ({cuda_knob()}): "
            feed_rows_within(t, tag, f, cpu, main_feed_bar(f))
        }
        for (i in range(min(length(r.cuda_feeds), length(r.cpu_feeds)))) {
            let tag = "gpt-oss-4L cuda, {r.cuda_feeds[i].nprompt} tokens: "
            feed_rows_within(t, tag, r.cuda_feeds[i], r.cpu_feeds[i], MOE_LOGIT_BAR_REL)
        }
        gptoss_codec_within(t, r)
        gptoss_host_layers_within(t, r)
        rings_within(t, "gpt-oss-4L", RING_IDS, r.rings, KVDtype.f16, KVDtype.q8_0)
    }
}

//! the forms the block loads: the default, two and three regions, the
//! host-layers pair, the q8_0 mirror where the environment arms it,
//! and the rings for the CUDA home's own cells; a codec the
//! environment keeps off is held as a skip
def private gptoss_forms(var r : GptossRows) : array<ModelForm> {
    var forms <- [device_form(), device_form(regions = 2l, ctx = REGIONS_RIG_CTX),
        device_form(regions = 3l, ctx = REGIONS_RIG_CTX),
        device_form(regions = 1l, host_layers = HOST_LAYERS_GPTOSS),
        device_form(regions = 1l, host_layers = HOST_LAYERS_GPTOSS, pf_stream = false)]
    for (kv in fixed_array(KVDtype.q8_0, KVDtype.f32)) {
        let why = codec_env_why(kv)
        if (!empty(why)) {
            r.carrier |> held_skip("the {kv} sink stamp: {why}")
        } elif (kv == KVDtype.q8_0) {
            forms |> push(device_form(kv = kv))
        }
    }
    if (!r.cuda) {
        return <- forms
    }
    forms |> reserve(length(forms) + 4)
    for (kv in fixed_array(KVDtype.f16, KVDtype.q8_0)) {
        let why = codec_env_why(kv)
        if (!empty(why)) {
            r.carrier |> held_skip("the rings on a {kv} mirror: {why}")
            continue
        }
        for (ring in fixed_array(true, false)) {
            forms |> push(device_form(kv = kv, ring = ring, ctx = RING_CTX,
                pf_window_rows = RING_WINDOW))
        }
    }
    return <- forms
}

[test(description = "The resident gpt-oss-4L driver - mx4 MoE window, biased router, sinked token command - holds the CPU chain's logits within the bar on one window and across two with the sliding layers masking, the one-step-off control past it, the routed block's census once per routed layer per window; the q8_0 and f32 K/V mirrors' sink stamps hold a 40-token window and eight fed steps within the routed bar of the CPU chain at the same codec; with two routed layers' experts on the host the streamed window and eight steps sit within the stream twin's bar of the stream-off load and the stream chain's acts take the picks' bias rows; over two mirror regions batched steps over two sessions - at one window, past the unsplit position, and before any one-row step - and over three a batched step over two rows, a one-row step, then batched steps over three keep every row within the routed bar of its session alone; on a CUDA card with the FP4 rung the CUDA driver's forced feeds over one and two windows and the host-layers feed hold 20 percent of max logit of the CPU chain with every position served, the meet splitting the experts, and the ringed K/V mirrors match a flat every-row mirror bit for bit over 1300 prose ids in 512-row windows on f16 and q8_0; each form loads once")]
def test_gpu_resident_gptoss(t : T?) {
    return if (!family_on(t, "gptoss"))
    var r : GptossRows
    r.prompts |> push(40l)
    //! two windows, the sliding layers masking below theirs, where the
    //! prompt window is the cells' own
    if (assert_pf_window(t)) {
        r.prompts |> push(PF_WINDOW_ROWS + 88l)
    }
    let cuda_why = cuda_cells_why()
    r.cuda = empty(cuda_why)
    if (gpu_want_cuda() && !r.cuda) {
        r.carrier |> held_skip("the CUDA home's own cells: {cuda_why}")
    }
    var forms <- gptoss_forms(r)
    //! the device claims first: a 32-expert layer at a host-first of
    //! eight hands the host every region
    let pins = pin_stream(0l)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gpt-oss-4L-mxfp4-local.gguf",
                GPTOSS_CTX, forms) $(m, form) {
            gptoss_rows(r, m, form)
        }
    }
    restore_stream_pins(pins)
    if (ran) {
        gptoss_rows_within(t, r)
    }
    delete r
    delete forms
}
