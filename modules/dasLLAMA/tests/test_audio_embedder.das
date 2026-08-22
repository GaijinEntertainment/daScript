options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_audio_embedder
require daslib/fio
require _model_tier   // models_dir() + model_available() gate

// The audio carrier's own arms, mirroring test_vision_embedder: the no-audio (none) shape the
// seams test against, the probe's 0-not-panic contract, the loader's refusal by name, and
// (model-gated) the gemma4a arm the E2B mmproj distinguishes.

[test]
def test_audio_embedder_none(t : T?) {
    t |> run("a default carrier is the no-audio arm") @(t : T?) {
        var e = AudioEmbedder()
        t |> equal(audio_proj_dim(e), 0l, "no soft-token width")
        t |> equal(audio_family(e), "", "no family")
        var st = AudioState()
        var out : array<float>
        var samples : array<float>
        var tripped = false
        try {
            encode_audio_(e, st, samples, out)
        } recover {
            tripped = true
        }
        t |> success(tripped, "encode on the no-audio arm panics by name")
        delete samples
        delete out
        delete st
        delete e
    }
    t |> run("the probe answers 0 and the loader refuses by name") @(t : T?) {
        t |> equal(audio_probe_proj_dim(path_join(models_dir(), "no-such-mmproj.gguf")), 0l, "a missing path probes 0, never panics")
        var refused = false
        try {
            var e <- load_audio_embedder(path_join(models_dir(), "no-such-mmproj.gguf"))
            delete e
        } recover {
            refused = true
        }
        t |> success(refused, "loading a towerless path panics by name")
    }
}

[test]
def test_audio_embedder_gemma4a_arm(t : T?) {
    t |> run("the E2B mmproj probes and loads as gemma4a with its width") @(t : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(t, mm)) {
            return
        }
        t |> equal(audio_probe_proj_dim(mm), 1536l, "the probe reports the E2B projection width")
        var inscope e <- load_audio_embedder(mm)
        t |> equal(audio_family(e), "gemma4a")
        t |> equal(audio_proj_dim(e), 1536l, "the carrier reports the same width")
    }
}
