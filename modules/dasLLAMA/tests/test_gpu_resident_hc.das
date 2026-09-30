options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen4exp)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the model
require dasllama/dasllama_env   // g_env_vulkan.vk_kv32: the f32 mirror passes the f16 sessions as codec
require dasllama/dasllama_moe   // moe_pick_tape: the CPU arm routes as the device arm did
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // feed_logits, feed_steps_within

// The whole-model resident driver on the hyper-connection carrier (Qwen3.8-Flash-Next UD-IQ4_XS, three shards, the large tier,
// DASLLAMA_GPU=1): the wide residual's mixer seams, the n-gram side input and the deltanet sigmoid out-gate on the device, the
// routed experts summed on the host between the segments of the window chain and of the split token command. The forced-feed
// logits-tolerance form of test_gpu_resident_hybrid: the same fixed tokens through the armed driver - the prompt through the
// window chain, every step through the token command - and through the all-CPU chain, the CPU chain routing as the device
// arm did (the decode pick tape): 512 experts top-10 over 4 mixed streams flip a near-tie on kernel-order noise alone, and
// one flipped expert moves the whole row past any bar. Every row's logits within the bar, the one-step-off control past it.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm prefills on the CPU by design
}

//! the routed block's bar (test_gpu_resident_moe's reading); with the routing pinned the arms read 0.37-0.68 against
//! bars of 1.7-2.6, the control 5.2-12.0 (PERF_LEDGER.md, the hot expert pool's entry)
let private LOGIT_BAR_REL = 0.20
let private HC_PROMPT = 48l   //! past the pool's 32-row window floor: the prompt's routed rows split between the device and the host
let private FN_TRUNK = "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf"

[test]
def test_gpu_resident_hc(t : T?) {
    t |> run("Qwen3.8-Flash-Next: the window chain and the split token command (the mixers, the side input and the sigmoid out-gate on device, the routed experts on the host) vs the CPU chain") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), FN_TRUNK)
        if (!model_available(t, path)) return
        if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
            t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
            return
        }
        if (g_env_vulkan.vk_kv32 != 0l) {
            t |> skip("DASLLAMA_VK_KV32=1: the f32 mirror passes every f16 session as codec")
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(tr.config.hyper_conn && tr.config.n_expert > 0l && tr.config.dn_z_sigmoid, "the carrier is the hyper-connection MoE with the sigmoid out-gate the chain exists for")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on it (the load log names any decline)")
        with_job_que() {
            setup_dasllama_jobque_()
            var prompt <- [for (i in range64(HC_PROMPT)); 1000l + (i * 37l) % 5000l]
            var toks <- [for (i in range64(6l)); 2000l + (i * 53l) % 4000l]
            let served0 = moe_gpu_resident_prefills()
            var sg <- create_session(tr, mirror_kv_dtype())
            moe_pick_tape(MoePickTape.record)
            var gpu <- feed_logits(tr, sg, prompt, toks)
            moe_pick_tape(MoePickTape.off)
            t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
            t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the window chain served the prompt (its hyper-connection form)")
            let hot = gpu_resident_hot_stats()
            let slots = gpu_resident_hot_slots()
            to_log(LOG_INFO, "hot expert pool: {gpu_resident_hot_slots()} slots a layer, {hot.uploads} experts uploaded, {hot.hits} of {hot.picks} decode picks and {hot.window_hits} of {hot.window_rows} window slot rows served on the device\n")
            if (slots > 0l) {
                let knob = "gpu_resident_hot_slots() = {slots}, DASLLAMA_GPU_HEAT = {g_env_gpu.gpu_heat ?? HOT_SLOTS_AUTO} (-1 = unset, the plan's own count)"
                t |> equal(hot.picks, long_length(toks) * tr.config.n_layers * tr.config.n_expert_used, "every routed layer took the pool, and every decode pick went through it ({knob})")
                t |> success(hot.uploads > 0l && hot.hits > 0l && hot.hits < hot.picks, "the prompt's picks filled the pool, the device served some of the steps' picks and the host the rest - both shares of the sum under the bar below ({knob}: {hot.hits} hits of {hot.picks} picks, {hot.uploads} uploads)")
                t |> equal(hot.window_rows, long_length(prompt) * tr.config.n_layers * tr.config.n_expert_used, "the tier's expert chain took every routed layer's window ({knob})")
                t |> success(hot.window_hits > 0l && hot.window_hits < hot.window_rows, "the device served some of the window's slot rows through the expert tiles and the host the rest ({knob}: {hot.window_hits} hits of {hot.window_rows} rows)")
            }
            delete sg
            moe_gpu_drop_model()
            var sc <- create_session(tr, mirror_kv_dtype())
            moe_pick_tape(MoePickTape.replay)   // the compare reads the seams, not the near-ties
            var cpu <- feed_logits(tr, sc, prompt, toks)
            let tape = moe_pick_tape_stats()
            moe_pick_tape(MoePickTape.off)
            delete sc
            t |> equal(tape.seen, (long_length(prompt) + long_length(toks)) * tr.config.n_layers, "every CPU select replayed one taped select (the arms route in the same order, the prompt's rows first)")
            to_log(LOG_INFO, "pick tape: the CPU chain's own decode top-k left {tape.miss} picks off the device arm's tape ({tape.seen} selects replayed)\n")
            feed_steps_within(t, tr, "", long_length(prompt), gpu, cpu, toks, LOGIT_BAR_REL, "the split command")
            delete gpu
            delete cpu
            delete prompt
            delete toks
        }
        delete tr
    }
}
