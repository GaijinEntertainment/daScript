options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// The AudioEmbedder carrier, and the pre-encoded rows seam it feeds. Skips: the gemma4a arm on
// `mmproj-gemma-4-E2B-it-bf16.gguf` (absent file), its direct-image route on a prepared
// gemma4a-tagged `.dlim` beside it (absent bake), the rows seam on `gemma-4-E2B-it-Q4_K_M.gguf`
// + that mmproj (absent file), the no-audio-arm refusal on `SmolLM2-135M-Instruct-Q8_0.gguf`
// (absent file). Every skip is loud.

require dastest/testing_boost public
require dasllama/dasllama_audio_embedder
require dasllama/dasllama_image   // image_family_tag — the per-arm dlim selection
require dasllama/dasllama         // add_user_audio_rows: the carrier rows on a plain chat
require dasllama/dasllama_load    // load_model_: the planar rail, no .dlim minted beside the model
require dasllama/dasllama_gemma4a // stage_ + mint_gemma4a_encoder: the encoder in memory, no .dlim
require daslib/jobque_boost
require daslib/fio
require math
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
                let tag = image_family_tag(cand)
                //! only the path the rail mints for THIS box identity is loadable by the direct route - a foreign-identity bake sits beside it under its own hash
                if (tag |> starts_with("gemma4a-") && cand == image_path_for(mm, tag)) {
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

// The pre-encoded audio rows on a PLAIN chat (no tower): the E2B family hears through this seam,
// and every refusal at it is a panic the caller must never reach by accident.
[test]
def test_audio_rows_on_a_plain_chat(t : T?) {
    let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
    let dec = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
    t |> run("rows land, the turn answers, a second clip appends, and the refusals panic") @(t : T?) {
        if (!model_available(t, mm) || !model_available(t, dec)) {
            return
        }
        allow_cpu_prefill()
        var m <- load_model_(dec, QuantMode.q8)   // the planar rail: no .dlim beside the model
        m.config.seq_len = min(m.config.seq_len, 2048l)
        let dim = m.config.dim
        with_job_que() {
            setup_dasllama_jobque()
            var inscope stg <- stage_gemma4a_encoder(mm)   // stage + mint: the encoder in memory, no .dlim
            var inscope e = AudioEmbedder(kind = AudioKind.gemma4a)
            mint_gemma4a_encoder(stg, e.gemma4a)
            var st = AudioState()
            var samples : array<float>
            samples |> resize(16000)   // one second of silence: real rows, a real span
            var rows : array<float>
            let n = encode_audio(e, st, samples, rows)
            t |> equal(n, 25l, "a second of audio is 25 soft tokens on this family (4 s of the JFK clip is 100)")
            // a second clip in the same turn appends its rows
            var chat <- create_chat(m, "", 8l)
            var rows2 : array<float>
            let n2 = encode_audio(e, st, samples, rows2)
            var rows_a := rows
            add_user_audio_rows(m, chat, rows_a, n)
            add_user_audio_rows(m, chat, rows2, n2)
            t |> equal(chat.n_audio_rows, n + n2, "two clips append")
            t |> equal(long_length(chat.audio_rows), (n + n2) * dim, "rows are dim-wide")
            // the length check
            var short : array<float>
            short |> resize(int(dim) - 1)
            var tripped = false
            try {
                add_user_audio_rows(m, chat, short, 1l)
            } recover {
                tripped = true
            }
            t |> success(tripped, "rows that are not n x dim floats panic")
            delete chat
            // the turn answers through the plain chat
            var chat2 <- create_chat(m, "", 8l)
            var rows_b := rows
            add_user_audio_rows(m, chat2, rows_b, n)
            add_user(chat2, "What did you hear?")
            var head : array<int64>
            var tail : array<int64>
            render_turn_audio(m, chat2, head, tail)   // render-only: the spans the prefill will carry around the rows
            var pieces = 0l
            respond(m, chat2, SamplingParams()) $(_piece) {
                pieces++
                return true
            }
            t |> equal(stats(chat2.session).n_gen, pieces, "every generated token reached the block")
            t |> equal(stats(chat2.session).n_prompt, long_length(head) + n + long_length(tail), "the prefill is head + rows + tail")
            t |> equal(chat2.n_audio_rows, 0l, "the rows are consumed by the turn")
            delete chat2
            // one media kind per turn: an image already queued refuses the audio rows
            var chat3 <- create_chat(m, "", 8l)
            var img : array<float>
            img |> resize(int(dim * (1l + m.config.n_deepstack)))
            add_user_image_rows(m, chat3, img, 1l, int2(0))
            var rows_c := rows
            var mixed = false
            try {
                add_user_audio_rows(m, chat3, rows_c, n)
            } recover {
                mixed = true
            }
            t |> success(mixed, "audio rows beside a queued image panic instead of dropping either")
            delete chat3
            delete rows
            delete samples
            delete st
        }
    }
}

[test]
def test_audio_rows_need_an_audio_arm(t : T?) {
    t |> run("a family with no audio span markers refuses the rows by name") @(t : T?) {
        let smol = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!model_available(t, smol)) {
            return
        }
        allow_cpu_prefill()
        let m <- load_model_(smol, QuantMode.q8)   // the planar rail: no .dlim beside the model
        with_job_que() {
            setup_dasllama_jobque()
            var chat <- create_chat(m, "", 8l)
            var rows : array<float>
            rows |> resize(int(m.config.dim))
            var tripped = false
            try {
                add_user_audio_rows(m, chat, rows, 1l)
            } recover {
                tripped = true
            }
            t |> success(tripped, "no audio arm: the rows are refused")
            delete chat
        }
    }
}
