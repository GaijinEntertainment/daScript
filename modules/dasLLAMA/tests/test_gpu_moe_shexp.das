options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2moe)
require dasllama/dasllama   // create_session

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU-form arm prefills its shared expert on the CPU by design
}
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto / moe_gpu_shexp_on_gpu / set_moe_gpu_shexp_route / set_gpu_resident_route
require dasllama/dasllama_blocks   // shexp_gpu_prefill_layers: the device arm's engage witness
require _model_tier   // models_dir() + model_available() presence/size-tier gate, the logits compares

// The shared expert's prefill on the device: a qwen2moe layer's shexp triple rides the routed
// experts' chain as one region over every position (ARCHITECTURE_GPU_VULKAN_GEMM.md sec.2.2q). The
// device chain and the CPU form are not bit-identical (the f16 gather, f32 accumulation order),
// so the gate is the forced-feed logits-tolerance form over the tier's in-process route lever:
// the same prompt and fed tokens with the route ON (the device arm) and OFF (the CPU form), the
// logits within a bar of the CPU arm after the prefill and at every step, the one-step-off
// control past it, and the engage witness - the arm's layer count grows by the model's layer
// count per prefill on the device arm and not at all on the CPU arm. The whole-model driver is
// pinned off for the load: it would take the twin whole (test_gpu_resident_moe.das is its gate).

let private TWIN = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf"   //! the Q4_K_M mint of the Q8_0 carrier (its recipe row in ../performance/model_specs.das)
//! of the step's max |logit|. The arms agree to 1e-8 at layer 0 and part from layer 1 on: the device chain's
//! summation order moves the residual by ulps, the next layer's Q8 requant rounds a few elements the
//! other way and the router flips a near-tie, and 24 layers of that read 0.05-0.10 on the twin under both
//! tile modes (the routed chain is bit-stable session to session; the shared expert's rows themselves sit
//! within 1e-4 of the CPU form on the sdot4 feed and 0.5% on the f16 feed - test_vulkan_moe_cm2's cells)
let private SHEXP_LOGIT_BAR_REL = 0.12
let private FED_STEPS = 4l

def private argmax_of(v : array<float>) : int64 {
    var best = -1e30
    var bi = 0l
    for (i in range64(long_length(v))) {
        if (v[i] > best) {
            best = v[i]
            bi = i
        }
    }
    return bi
}

//! prefill `prompt`, then feed `toks` one per step; the logits after the prefill and after each step
def private feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>) : array<array<float>> {
    let n = long_length(prompt)
    var rows : array<array<float>>
    rows |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    rows |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        rows |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    }
    return <- rows
}

//! one prompt of `n` tokens plus the fed steps through both arms of the route lever
def private shexp_forced_feed(t : T?; n : int64) {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return
    }
    let path = path_join(models_dir(), TWIN)
    if (!model_available(t, path)) return
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the MoE tier")
        return
    }
    set_gpu_resident_route(false)   //! the per-op rails' claims: the whole-model driver would take the twin (test_gpu_resident_moe.das holds that arm)
    var tr <- load_model_(path, QuantMode.q8)
    set_gpu_resident_route(true)   //! the lever reads at the load alone
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
    with_job_que() {
        setup_dasllama_jobque_()
        var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]
        var toks <- [for (i in range64(FED_STEPS)); 2000l + (i * 53l) % 4000l]
        set_moe_gpu_shexp_route(true)
        let served0 = shexp_gpu_prefill_layers()
        let span0 = span_tokens()
        var sd <- create_session(tr)
        var dev <- feed_logits(tr, sd, prompt, toks)
        delete sd
        t |> equal(shexp_gpu_prefill_layers(), served0 + layers, "the device arm served the shared expert of every layer over the {n}-token prompt")
        t |> equal(span_tokens(), span0 + FED_STEPS, "the decode span served every fed step on the device arm (the shared expert inside it)")
        set_moe_gpu_shexp_route(false)
        var sc <- create_session(tr)
        var cpu <- feed_logits(tr, sc, prompt, toks)
        delete sc
        set_moe_gpu_shexp_route(true)   //! the tier as found
        t |> equal(shexp_gpu_prefill_layers(), served0 + layers, "the CPU arm served no shared expert on the device")
        t |> equal(span_tokens(), span0 + FED_STEPS, "the CPU arm's steps took the per-layer path - the span declines a shared expert off the device")
        for (st in range64(long_length(cpu))) {
            let diff = logits_maxdiff(dev[st], cpu[st])
            let bar = SHEXP_LOGIT_BAR_REL * logits_maxabs(cpu[st])
            let ad = argmax_of(dev[st])
            let ac = argmax_of(cpu[st])
            let what = st == 0l ? "prefill ({n} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})"
            to_log(LOG_INFO, "{what}: argmax cpu {ac} ({cpu[st][ac]}), device {ad} ({dev[st][ad]}); logits maxdiff {diff} (bar {bar})\n")
            t |> success(diff <= bar, "{what} logits: the device shared expert within {bar} of the CPU form (maxdiff {diff})")
            if (st > 0l) {   //! the control: the previous step's CPU logits are a disconnected mechanism the bar must RED
                let off = logits_maxdiff(dev[st], cpu[st - 1l])
                to_log(LOG_INFO, "{what}: one-step-off control maxdiff {off} (bar {bar})\n")
                t |> success(off > bar, "{what} control: the bar discriminates a one-step-off compare (maxdiff {off} vs bar {bar})")
            }
        }
        delete dev
        delete cpu
        delete prompt
        delete toks
    }
    delete tr
}

[test]
def test_gpu_moe_shexp_prefill(t : T?) {
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the shared expert's prefill on the device vs the CPU form, one 64-token prompt") @(t : T?) {
        shexp_forced_feed(t, 64l)
    }
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the same over a 600-token prompt (the chain's chunking)") @(t : T?) {
        shexp_forced_feed(t, 600l)
    }
}
