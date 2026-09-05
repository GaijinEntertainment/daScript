options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen35)
require dasllama/dasllama   // create_session, moe_gpu_drop_model

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the resident window chain has no recurrent arm yet: a hybrid's prompt prefills on the CPU by design
}
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the hybrid
require _model_tier   // models_dir() + model_available() presence/size-tier gate

// The whole-model resident driver on a deltanet hybrid (Qwen3.5-0.8B-Q8_0 under DASLLAMA_GPU=1):
// the CPU prefills the prompt, the resident decode takes the mirror over and runs every layer -
// the recurrent ones through the fused deltanet step on device - as one submit per token. The
// GPU step and the CPU chain are not bit-identical (GPU exp / f32 accumulation order), so the gate
// is the forced-feed logits-tolerance form: the same fixed tokens fed to both arms, the logits
// within a bar of the CPU chain at every step, and the one-step-off control that must exceed it.

let private DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit| - the deltanet bar test_deltanet.das holds too

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

// prefill `prompt`, then feed `toks` one per step; the logits after the prefill and after each step
def private feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>) : array<array<float>> {
    let n = long_length(prompt)
    var out : array<array<float>>
    out |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    out |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    }
    return <- out
}

[test]
def test_gpu_resident_hybrid_decode(t : T?) {
    t |> run("Qwen3.5-0.8B: the resident decode with deltanet layers on device vs the CPU chain, forced feed") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        if (!model_available(t, path)) return
        if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
            t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the hybrid (the load log names any decline)")
        with_job_que() {
            setup_dasllama_jobque_()
            let n = 40l
            var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]
            var toks <- [for (i in range64(8l)); 2000l + (i * 53l) % 4000l]
            var sg <- create_session(tr, KVDtype.f16)   // the armed mirror codec
            var gpu <- feed_logits(tr, sg, prompt, toks)
            t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
            delete sg
            moe_gpu_drop_model()   // every later step is the CPU chain
            var sc <- create_session(tr, KVDtype.f16)
            var cpu <- feed_logits(tr, sc, prompt, toks)
            delete sc
            for (st in range64(long_length(cpu))) {
                let diff = logits_maxdiff(gpu[st], cpu[st])
                let bar = DN_LOGIT_BAR_REL * logits_maxabs(cpu[st])
                let ag = argmax_of(gpu[st])
                let ac = argmax_of(cpu[st])
                let what = st == 0l ? "prefill" : "step {st - 1l} (fed {toks[st - 1l]})"
                to_log(LOG_INFO, "{what}: argmax cpu {ac} ({cpu[st][ac]}), resident {ag} ({gpu[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
                t |> success(diff <= bar, "{what} logits: resident within {bar} of the CPU chain (maxdiff {diff})")
                if (st > 0l) {   // the control: the previous step's CPU logits are a disconnected mechanism the bar must RED
                    let off = logits_maxdiff(gpu[st], cpu[st - 1l])
                    t |> success(off > bar, "{what} control: the bar discriminates a one-step-off compare (maxdiff {off} vs bar {bar})")
                }
            }
            delete gpu
            delete cpu
            delete prompt
            delete toks
        }
        delete tr
    }
}
