options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _moe_resident   // stream_twin_within, resident_ready through its public _resident_feed
require _model_tier   // models_dir
require dasllama/dasllama   // load_model_, create_session, moe_gpu_drop_model
require daslib/fio   // path_join
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_tier   // set_gpu_resident_route, moe_gpu_layer_on_gpu: the per-op cell's route lever and marks
require dasllama/dasllama_gpu_resident   // tier_expert_decline
require ?vulkan dasllama/dasllama_vulkan_prefill   // vk_xf_biased_acts: the Vulkan chain's biased acts, the stream twin's witness on that home
require ?cuda dasllama/dasllama_cuda_stream   // cud_stream_stats().biased: the same on the CUDA home
require _kv_codec_cells   // cell_codec_forced_feed: the sink stamps under the K/V codecs
require dasllama/dasllama_kv_dtype   // KVDtype: the codec cells' mirror asks
require dasllama/dasllama_core   // set_gpu_ctx_max: the codec cells' context

// gpt-oss-20b (the native-MXFP4 routed stacks on the mx4 expert rail, the biased router's softmax over its four picks,
// the gate / up / down bias rows at the act and the combine under the clamped swiglu, the per-head attention sinks in
// the flash tiles and the token command's pass, the output bias on the residual step, sliding windows on the even
// layers, YaRN folded at load) in _moe_resident's forced-feed form: one window, then two. Large-tier
// (DASLLAMA_PARITY_FULL=1); -jit only; skips without the file, the armed tier, or a device with neither the cm2 tile
// family nor KHR cooperative matrices at subgroup 32 (the expert pieces' two tiles); on a device with either the
// driver's admission is asserted - a decline (a seat, the sink arm, a plane's format) is a red that sends the reader
// to the load log. The per-op cells (the whole-model route off, the heat pool) skip under DASLLAMA_CUDA=1 - the per-op
// tier is the Vulkan home's; the host-stream cell skips on a build with neither discrete home's stream chain; the sink
// codec cells skip under DASLLAMA_VK_KV32=1, which arms the f32 mirror over the codec asked.
//! gpt-oss's logits bar: the routed chain's noise reads 0.06-0.15 of a 7-15 max logit on the 5060 Ti (0.9-2.2%) at 40 and 600 tokens and
//! every step, the argmax agreeing everywhere - the four picks over 32 experts on a biased softmax sit far from a near-tie on
//! this fixture - while the one-step-off controls read 4.2-18.9, so a 5% bar keeps a 2x margin over the noise and 12x under the control
let private GPTOSS_LOGIT_BAR_REL = 0.05
let private GPTOSS = MoeFixture(file = "gpt-oss-20b-mxfp4.gguf", shexp = false, bar_rel = GPTOSS_LOGIT_BAR_REL, window = 128l)   //! the pinned upstream file (its row in ../performance/model_specs.das)
//! the heat pool's walk reaches eighty fed steps, and one of them (step 11) lands a router near-tie the pool's summation order
//! flips - 0.447 against the 5% bar's 0.378, the argmax intact - so the heat cell holds the routed block's bar (`MOE_LOGIT_BAR_REL`)
let private GPTOSS_HEAT = MoeFixture(file = "gpt-oss-20b-mxfp4.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, window = 128l)
//! the two-window prompt's first fed step lands a router near-tie on the RTX PRO 4500 - 0.458 against the 5% bar's 0.341, the
//! argmax intact, the later steps 0.07 - so the two-window cell holds the routed block's bar too
let private GPTOSS_WINDOWS = MoeFixture(file = "gpt-oss-20b-mxfp4.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, window = 128l)

[test]
def test_gpu_perop_gptoss(t : T?) {
    t |> run("gpt-oss-20b with the whole-model route off: the per-op expert chain takes every routed layer's mx4 stacks under the biased clamped swiglu, the window and steps against the CPU chain") @(t : T?) {
        if (!family_on(t, "gptoss")) return
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

[test]
def test_gpu_perop_gptoss_heat(t : T?) {
    t |> run("gpt-oss-20b on the per-op tier: twelve routed layers placed, twelve streamed under the heat pool's mx4 slots with the biased clamped swiglu, against the CPU chain") @(t : T?) {
        if (!family_on(t, "gptoss")) return
        moe_perop_heat_cell(t, GPTOSS_HEAT, 12l, 8l)
    }
}

let private HOST_LAYERS_GPTOSS = 2l   //! the host-layers ask of the stream cell: two routed layers' experts on the host, streamed for the prompt

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

//! gpt-oss's biased stacks on the host-layers form: the window's picks through the per-expert chain under the clamped swiglu and the
//! picks' gate and up bias rows - the streamed windows' rows against the same load with the stream off, on whichever discrete home is armed
[test]
def test_gpu_resident_gptoss_host_stream(t : T?) {
    t |> run("gpt-oss-20b with {HOST_LAYERS_GPTOSS} routed layers' experts pinned to the host: the streamed windows' rows against the host's sums' at the stream twin's bar, one window (40 tokens) and eight steps; the chain's acts under the picks' bias rows") @(t : T?) {
        let path = path_join(models_dir(), GPTOSS.file)
        return if (!resident_ready(t, path))
        let b0 = biased_acts()
        if (b0 < 0l) {
            t |> skip("no discrete GPU home on this build: the stream chain is the Vulkan and CUDA drivers'")
            return
        }
        stream_twin_within(t, path, HOST_LAYERS_GPTOSS, 40l, 8l, STREAM_TWIN_BAR_REL, "gpt-oss-20b ")
        t |> success(biased_acts() > b0, "the chain's acts took the routed form's bias rows ({biased_acts() - b0} biased acts)")
    }
}

let private CODEC_CTX = 8192l   //! the codec arms' context (`set_gpu_ctx_max`, put back): the f32 mirror beside the 12 GB of planes on a 16 GB card

//! the sink stamps of the K/V codecs (one stamp a mirror codec, gpt-oss the one sink carrier): the q8_0 mirror and, under the
//! environment's knob, the f32 mirror in the forced-feed form; the f16 stamp is the file's other cells' (the CUDA gpt-oss file's
//! cell). No tq4 arm: the carrier's Q/K/V bias steps a tq4 ask down to q8_0 on every home, so the tq4 sink stamp has no carrier
[test]
def test_gpu_resident_gptoss_sink_codecs(t : T?) {
    for (kv in fixed_array(KVDtype.q8_0, KVDtype.f32)) {
        t |> run("gpt-oss-20b: a {kv} mirror - the codec's sink stamp - one window (40 tokens) and eight steps") @(t : T?) {
            return if (!codec_env_arms(t, kv))
            let levers = resident_levers()
            set_gpu_ctx_max(CODEC_CTX)
            cell_codec_forced_feed(t, GPTOSS.file, 40l, kv, MOE_LOGIT_BAR_REL)
            restore_resident_levers(levers)
        }
    }
}

[test]
def test_gpu_resident_gptoss(t : T?) {
    t |> run("gpt-oss-20b: the resident mx4 MoE prefill window + the biased, sinked token command vs the CPU chain, one window") @(t : T?) {
        moe_forced_feed(t, 40l, GPTOSS)
    }
    t |> run("gpt-oss-20b: the same across two windows (the 128-key sliding layers masking below their window on both eras), at the routed block's bar") @(t : T?) {
        assert_pf_window(t)
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, GPTOSS_WINDOWS)
    }
}
