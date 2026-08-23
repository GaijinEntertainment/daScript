options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_audio_embedder
require dasllama/dasllama_image   // image_family_tag — the per-arm dlim selection
require daslib/fio
require strings
require _model_tier   // models_dir() + model_available() gate

// the audio twin of test_vision_embedder

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
        t |> equal(audio_family(e), "gemma4a", "the carrier reports the gemma4a family")
        t |> equal(audio_proj_dim(e), 1536l, "the carrier reports the same width")
        var st = AudioState()
        var samples : array<float>
        samples |> resize(320)
        var rows : array<float>
        t |> equal(encode_audio_(e, st, samples, rows), 1l, "a 20ms sub-frame clip pads to exactly one soft token - a non-empty clip can never encode to zero rows")
        delete rows
        delete samples
        delete st
        var image_path = ""
        dir(models_dir()) $(name) {
            // the mmproj bakes one dlim PER ARM (gemma4a audio, gemma4v vision) — select by the
            // baked family tag, or directory order hands this audio cell a vision image. The
            // load above already minted the gemma4a arm when the image rail is on, so this
            // scan only skips under DASLLAMA_IMAGE=0 — a model gate, not an artifact gate
            if (empty(image_path) && starts_with(name, "{base_name(mm)}.0x") && ends_with(name, ".dlim")) {
                let cand = path_join(models_dir(), name)
                if (image_family_tag(cand) |> starts_with("gemma4a-")) {
                    image_path = cand
                }
            }
        }
        if (empty(image_path)) {
            t |> skip("no prepared gemma4a-tagged {base_name(mm)}.0x*.dlim beside the mmproj - the direct image route needs a baked image")
        } else {
            t |> equal(audio_probe_proj_dim(image_path), 0l, "the gguf probe answers 0 on a .dlim by pinned contract")
            var inscope ei <- load_audio_embedder(image_path)
            t |> equal(audio_family(ei), "gemma4a", "a prepared image routes on its baked family tag, not the probe")
            t |> equal(audio_proj_dim(ei), 1536l, "the image-loaded carrier reports the same width")
        }
    }
}
