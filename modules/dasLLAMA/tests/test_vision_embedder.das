options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_vision_embedder
require daslib/fio
require _model_tier   // models_dir(), model_available()
require _model_form   // own_load: the refusal cell's loads

// The vision carrier's own arms: the text-only (none) shape the seams test against and the sniff's
// refusals by name. The gemma4uv arms the 12B pair distinguishes run on that mmproj's block in
// test_gemma4uv.das (`test_vision_embedder_gemma4uv_arms`); the gemma4v arms are test_gemma4v.das's
// and the image chat cells'.

[test(description = "A default VisionEmbedder reports zero width, zero alignment, no family and the f32 stamp and panics on encode")]
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
}

[test(description = "`load_vision_embedder` panics on a missing file and on an audio-only mmproj")]
def test_vision_embedder_refusals(t : T?) {
    own_load("asserts on the load itself - its refusal: a path that is no file, and an audio-only mmproj")
    t |> run("the loader refuses a missing file by name") @(t : T?) {
        var missing = false
        try {
            var e <- load_vision_embedder(path_join(models_dir(), "no-such-mmproj.gguf"))
            delete e
        } recover {
            missing = true
        }
        t |> success(missing, "a missing path panics")
    }
    t |> run("the loader refuses a non-vision mmproj by name") @(t : T?) {
        let audio_only = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")   // an audio projector, no vision half
        return if (!model_available(t, audio_only))
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

[test(description = "`vision_exec_fmt` stamps qwen3v f32 by default and q8 once its q8 flag is set, while qwen25v always stamps f32")]
def test_vision_embedder_exec_fmt(t : T?) {
    t |> run("the qwen3v q8 flag reaches the exec_fmt stamp") @(t : T?) {
        var e3 = VisionEmbedder(kind = VisionKind.qwen3v)
        t |> equal(vision_exec_fmt(e3), "f32", "qwen3v exact stamps f32")
        e3.qwen3v.q8 = true
        t |> equal(vision_exec_fmt(e3), "q8", "qwen3v q8 stamps q8")
        var e25 = VisionEmbedder(kind = VisionKind.qwen25v)
        t |> equal(vision_exec_fmt(e25), "f32", "qwen25v stamps f32 - the outlier ViT serves exact only")
        delete e3
        delete e25
    }
}
