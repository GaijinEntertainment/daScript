options gen2
require dastest/testing_boost public
require strings
require dasllama/dasllama_whisper
require dasllama/dasllama_parakeet
require dasllama/dasllama_canary
require dasllama/dasllama_gemma4a
require dasllama/dasllama_qwen3a
require ../performance/fetch_models.das

// The family-owned halves of the ASR facade verbs (asr_exec_fmt / asr_encode_bucket), fed
// constructed structs — the moved-bit test the extraction owes. Plus the model-provenance
// manifest's shape invariants (fetch_models.das has no other model-free gate).

[test]
def test_family_exec_fmt_verbs(t : T?) {
    t |> run("each family verb stamps its own precision") @(t : T?) {
        var w = WhisperModel()
        w.enc.q8 = true
        w.dq8 = false
        t |> equal(whisper_exec_fmt(w), "q8/f32", "whisper mixed rail")
        w.dq8 = true
        t |> equal(whisper_exec_fmt(w), "q8/q8", "whisper q8 rail")
        var p = ParakeetModel()
        p.q8 = true
        t |> equal(parakeet_exec_fmt(p), "q8", "parakeet single carrier")
        var c = CanaryEncoder()
        c.q8 = true
        t |> equal(canary_exec_fmt(c, "q8"), "q8/q8", "canary q8 rail")
        c.q8 = false
        t |> equal(canary_exec_fmt(c, "q8"), "f32/q8", "canary fp32 parity encoder")
        t |> equal(gemma4a_exec_fmt("q8"), "f32/q8", "gemma4a fp32 tower")
        t |> equal(qwen3a_exec_fmt("q8"), "f32/q8", "qwen3a fp32 tower")
    }
}

[test]
def test_family_encode_buckets(t : T?) {
    t |> run("encode-total buckets are family facts") @(t : T?) {
        t |> equal(whisper_encode_bucket(), "wh.encode_total")
        t |> equal(canary_encode_bucket(), "cn.encode")
        t |> equal(gemma4a_encode_bucket(), "")
        t |> equal(qwen3a_encode_bucket(), "")
    }
}

[test]
def test_models_provenance_invariants(t : T?) {
    t |> run("the provenance manifest is well-formed") @(t : T?) {
        var inscope entries <- models_provenance()
        t |> success(length(entries) >= 18, "the catalog is complete (18+ entries)")
        var seen : table<string>
        for (e in entries) {
            t |> success(!empty(e.name), "every entry is named")
            t |> success(!key_exists(seen, e.name), "names are unique")
            seen |> insert(e.name)
            // a registry-served file pins bytes; a converted file carries its recipe — one
            // of the two provenance stories must hold for every entry
            let sha_ok = length(e.sha256) == 64
            t |> success(sha_ok || !empty(e.recipe), "sha256 or recipe present: {e.name}")
            if (!empty(e.url)) {
                t |> success(starts_with(e.url, "https://"), "url is https: {e.name}")
            }
        }
        delete seen
    }
}
