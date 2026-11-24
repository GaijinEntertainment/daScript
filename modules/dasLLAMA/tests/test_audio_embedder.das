options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// The AudioEmbedder carrier, and the pre-encoded rows seam it feeds. The gemma4a arm's cells and the rows seam on a
// plain chat run under the gemma-4 mmprojs' blocks in test_audio.das, the whisper-class tower arm under the ultravox and
// Qwen2.5-Omni mmprojs' blocks there (`_audio_embedder_cells.das`). SmolLM2-135M-Instruct-Q8_0.gguf loads once, in its
// block here. Skips here: the no-audio-arm refusal without SmolLM2 (absent file), and each staged-lane leg on
// `mmproj-Voxtral-Mini-3B-2507-Q8_0.gguf` / `voxtral-mini-mmproj-f32.gguf` (absent file). Every skip is loud.

require dastest/testing_boost public
require dasllama/dasllama_audio_embedder
require dasllama/dasllama         // add_user_audio_rows: the carrier rows on a plain chat
require dasllama/dasllama_gemma4a // stage_ + mint_gemma4a_encoder: the encoder in memory, no .dlim
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // models_dir() + model_available() gate
require _cpu_gpu_model   // with_cpu_and_gpu_model: SmolLM2's one block in the area; tower_cell, own_load
require dasllama/dasllama_common   // with_cpu_prefill: the cell's CPU-prefill intent, put back however it leaves

// the audio twin of test_vision_embedder

[test(description = "A default AudioEmbedder reports width 0 and no family and panics on encode, a missing mmproj probes width 0 without panicking, and loading it panics by name")]
def test_audio_embedder_none(t : T?) {
    t |> run("a default carrier is the no-audio arm") @(t : T?) {
        var e = AudioEmbedder()
        t |> equal(audio_proj_dim(e), 0l, "no soft-token width")
        t |> equal(audio_family(e), "", "no family")
        var st = AudioState()
        var out, samples : array<float>
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

// The whisper-class arm's own cells; the tower arm itself runs under the ultravox and Qwen2.5-Omni mmprojs' blocks in
// test_audio.das (`_audio_embedder_cells.das`).
[test(description = "Only an ultravox mmproj probes a bare audio span - a qwen2.5o mmproj does not, a missing file probes false - and on the Voxtral mmprojs the f32 lane stages no halfword twin while the f16 lane stages its GEMMs as halfwords")]
def test_audio_embedder_tower_arm(t : T?) {
    own_load("asserts on the load itself - its allocation: the halfword twin a Voxtral mmproj's f32 lane stages none of and its f16 lane stages")
    t |> run("the span rule rides the projector: ultravox rows take no marker, the others their template's") @(t : T?) {
        let uv = path_join(models_dir(), "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf")
        let omni = path_join(models_dir(), "mmproj-Qwen2.5-Omni-3B-f16.gguf")
        if (!model_available(t, uv) || !model_available(t, omni)) {
            return
        }
        t |> success(audio_probe_span_bare(uv), "an ultravox mmproj probes a bare span")
        t |> success(!audio_probe_span_bare(omni), "a qwen2.5o mmproj does not")
        t |> success(!audio_probe_span_bare(path_join(models_dir(), "no-such-mmproj.gguf")), "a missing file probes false, never panics")
    }
    for (file in ["mmproj-Voxtral-Mini-3B-2507-Q8_0.gguf", "voxtral-mini-mmproj-f32.gguf"]) {
        t |> run("{file}: the f32 lane stages no halfword twin, the f16 lane its GEMMs as f16 halfwords") @(t : T?) {
            let mm = path_join(models_dir(), file)
            if (!model_available(t, mm)) {
                return
            }
            var inscope st <- stage_audio_tower(mm, false, false)
            t |> success(!st.e.gemm_f16 && empty(st.wblob), "{file}: the f32 lane carries no twin - a GPU home's exact lane is the f16 image")
            var inscope sh <- stage_audio_tower(mm, false, true)
            t |> success(sh.e.gemm_f16 && !empty(sh.wblob), "{file}: the f16 lane stages the GEMMs as halfwords ({length(sh.wblob)} halves)")
        }
    }
}

// A chat template with no audio span markers, on SmolLM2's CPU form - the planar rail, no .dlim beside the model.
[cold_path]
def private audio_arm_refusal(t : T?; var m : Model) {
    with_dasllama_jobque() {
        var chat <- create_chat(m, "", 8l)
        var rows : array<float>
        rows |> resize(m.config.dim)
        var tripped = false
        try {
            add_user_audio_rows(m, chat, rows, 1l)
        } recover {
            tripped = true
        }
        t |> success(tripped, "no audio arm: the rows are refused")
        // an encoder whose span takes no marker (ultravox) owes the template none: the same rows queue
        var bare_rows : array<float>
        bare_rows |> resize(int(m.config.dim) * 2)
        add_user_audio_rows(m, chat, bare_rows, 2l, true)
        t |> equal(chat.n_audio_rows, 2l, "a bare span's rows queue on a template with no audio marker")
        delete chat
    }
}

// SmolLM2 135M, one block: its CPU form, loaded alone.
[test(description = "A chat template with no audio span markers (SmolLM2) panics on audio rows, yet queues rows flagged as a bare span that needs no marker")]
def test_audio_rows_need_an_audio_arm(t : T?) {
    return if (!family_on(t, "llama"))
    with_cpu_and_gpu_model(t, "audio", "SmolLM2-135M-Instruct-Q8_0.gguf", 512l, [cpu_cells_form(QuantMode.q8, true)]) $(var m; form) {
        return if (!form.alone)
        with_cpu_prefill(true) {
            tower_cell(t, "a family with no audio span markers refuses the rows by name", m, @@audio_arm_refusal)
        }
    }
}
