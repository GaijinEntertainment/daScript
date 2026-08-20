options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_vision_embedder
require daslib/fio
require _model_tier   // models_dir() + model_available() gate

// The vision carrier's own arms: the text-only (none) shape the seams test against, the sniff's
// refusals by name, and (model-gated) the gemma4uv arms the 12B pair distinguishes. The gemma4v
// arms are test_gemma4v.das / test_vision_chat.das's.

[test]
def test_vision_embedder_none(t : T?) {
    t |> run("a default carrier is the text-only arm") @(t : T?) {
        var e = VisionEmbedder()
        t |> equal(vision_proj_dim(e), 0l, "no soft-token width")
        t |> equal(vision_align(e), 0l, "no canvas unit")
        t |> equal(vision_family(e), "", "no family")
        t |> equal(vision_exec_fmt(e), "f32", "the exact stamp is the default")
        var st = VisionState()
        var out : array<float>
        var tripped = false
        try {
            var img = VisionImage()
            encode_image_(e, st, img, "none", out)
        } recover {
            tripped = true
        }
        t |> success(tripped, "encode on the text-only arm panics by name")
        delete st
        delete e
    }
    t |> run("the loader refuses a missing file and a non-vision mmproj by name") @(t : T?) {
        var missing = false
        try {
            var e <- load_vision_embedder(path_join(models_dir(), "no-such-mmproj.gguf"))
            delete e
        } recover {
            missing = true
        }
        t |> success(missing, "a missing path panics")
        let audio_only = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")   // an audio projector, no vision half
        if (stat(audio_only).is_valid) {
            var refused = false
            try {
                var e <- load_vision_embedder(audio_only)
                delete e
            } recover {
                refused = true
            }
            t |> success(refused, "an audio-only mmproj is refused by name")
        }
    }
}

[test]
def test_vision_embedder_gemma4uv_arms(t : T?) {
    t |> run("the 12B mmproj sniffs as gemma4uv and reports its geometry") @(t : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
        if (!model_available(t, mm)) {
            return
        }
        var inscope e <- load_vision_embedder(mm)
        t |> equal(vision_family(e), "gemma4uv")
        t |> equal(vision_align(e), 48l, "48 px superpatch")
        t |> equal(vision_proj_dim(e), 3840l, "the 12B width")
        t |> equal(vision_exec_fmt(e), "f32", "the embedder serves the file's planes")
    }
}
