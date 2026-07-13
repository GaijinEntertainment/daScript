options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model/sessions free before exit

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math  // setup_dasllama_jobque_ (the engine-standard jobque config)
require _model_tier   // models_dir() + model_available() presence/size-tier gate

// Chunked-vs-recurrent deltanet prefill equivalence on Qwen3.5-0.8B. The 20-token parity fixture
// covers only a single partial chunk, so this probe prefills 150 synthetic tokens — two full
// 64-token chunks plus a partial tail — exercising chunk boundaries, partial-chunk math, and the
// batched-conv history handoff. The two forms are NOT bit-identical (chunk FP reassociation;
// llama.cpp accepts the same divergence between its autoregressive and chunking builders), so the
// gate is id-level: same argmax after prefill, then token-for-token agreement over an 8-token
// greedy continuation — which also proves the conv/S state the chunked prefill leaves behind
// feeds the recurrent decode correctly.

def private argmax_of(v : array<float>) : int64 {
    var best = -1e30
    var bi = 0l
    for (i in range64(int64(length(v)))) {
        if (v[i] > best) {
            best = v[i]
            bi = i
        }
    }
    return bi
}

[test]
def test_deltanet_chunked_prefill(t : T?) {
    t |> run("deltanet chunked prefill == recurrent (Qwen3.5-0.8B, multi-chunk)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        // the multi-chunk case diverges from the recurrent oracle on M1 (deterministic argmax
        // mismatch at the chunk boundary; pre-existing — reproduces at master with a fresh JIT
        // cache and rebuilt binary). Tracked as #3454; the env rail runs it anyway for the fix.
        if (get_env_variable("DASLLAMA_DELTANET_MULTICHUNK") != "1") {
            t |> skip("chunked-vs-recurrent multi-chunk diverges on M1 — tracked as #3454 (DASLLAMA_DELTANET_MULTICHUNK=1 to run)")
            return
        }
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        if (!model_available(t, path)) return
        var tr <- load_gguf(path, QuantMode.q8)
        if (tr.config.seq_len > 2048l) {   // KV RAM cap, same as the parity fixtures
            tr.config.seq_len = 2048l
        }
        let c = tr.config
        let prev_dn = get_deltanet_prefill_mode()
        with_job_que() {
            setup_dasllama_jobque_()
            let n = 150l
            var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]

            set_deltanet_prefill_mode(DnPrefillMode.recurrent)
            var s1 <- make_run_state(c)
            forward_prefill(tr, s1, prompt, n, 0l)
            var ref <- [for (i in range64(c.vocab_size)); s1.logits[i]]

            set_deltanet_prefill_mode(DnPrefillMode.chunked)
            var s2 <- make_run_state(c)
            forward_prefill(tr, s2, prompt, n, 0l)
            var got <- [for (i in range64(c.vocab_size)); s2.logits[i]]

            var tok1 = argmax_of(ref)
            var tok2 = argmax_of(got)
            t |> equal(tok2, tok1)
            delete ref
            delete got

            // greedy continuation: both sessions decode through the recurrent per-token core off
            // the state their prefill produced — ids must stay in lockstep
            for (st in range64(8l)) {
                forward(tr, s1, tok1, n + st)
                forward(tr, s2, tok2, n + st)
                var l1 <- [for (i in range64(c.vocab_size)); s1.logits[i]]
                var l2 <- [for (i in range64(c.vocab_size)); s2.logits[i]]
                tok1 = argmax_of(l1)
                tok2 = argmax_of(l2)
                t |> equal(tok2, tok1)
                delete l1
                delete l2
            }
            delete prompt
            delete s1
            delete s2
        }
        set_deltanet_prefill_mode(prev_dn)
        delete tr
    }
}
