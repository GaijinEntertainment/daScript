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
            if (empty(image_path) && starts_with(name, "{base_name(mm)}.") && ends_with(name, ".dlim")) {
                let cand = path_join(models_dir(), name)
                let tag = image_family_tag(cand)
                //! only the path the rail mints for THIS box identity is loadable by the direct route - a foreign-identity bake sits beside it under its own hash
                //! and of its two lanes only the one this box serves: the direct route reads under the lane the loader picked above
                if (tag == (e.gemma4a.q8 ? "gemma4a-q8" : "gemma4a-f32") && cand == image_path_for(mm, tag)) {
                    image_path = cand
                }
            }
        }
        if (empty(image_path)) {
            t |> skip("no prepared gemma4a-tagged {base_name(mm)}.*.dlim beside the mmproj - the direct image route needs a baked image")
        } else {
            t |> equal(audio_probe_proj_dim(image_path), 0l, "the gguf probe answers 0 on a .dlim by pinned contract")
            var inscope ei <- load_audio_embedder(image_path)
            t |> equal(audio_family(ei), "gemma4a", "a prepared image routes on its baked family tag, not the probe")
            t |> equal(audio_proj_dim(ei), 1536l, "the image-loaded carrier reports the same width")
        }
    }
}

def private tower_arm_cell(t : T?; file, family : string; width, rows_per_chunk : int64) {
    let mm = path_join(models_dir(), file)
    if (!model_available(t, mm)) {
        return
    }
    t |> equal(audio_probe_proj_dim(mm), width, "the probe reports the projector's output width off its tensors")
    var inscope e <- load_audio_embedder(mm)
    t |> success(e.kind == AudioKind.tower, "the carrier holds the whisper-class tower")
    t |> equal(audio_family(e), family, "the carrier reports the projector kind")
    t |> equal(audio_proj_dim(e), width, "the carrier reports the probed width")
    var st = AudioState()
    var samples : array<float>
    samples |> resize(16000 * 5)
    for (i, s in count(), samples) {
        s = 0.3 * sin(float(i) * 0.05)
    }
    var rows : array<float>
    t |> equal(encode_audio_(e, st, samples, rows), rows_per_chunk, "a 5 s clip is one mel chunk")
    t |> equal(long_length(rows), rows_per_chunk * width, "one row of the decoder's width a soft token")
    var peak = 0.
    var finite = true
    for (v in rows) {
        finite &&= is_finite(v)
        peak = max(peak, abs(v))
    }
    t |> success(finite && peak > 0., "the rows are finite and not all zero (peak {peak})")
    var again : array<float>
    t |> equal(encode_audio_(e, st, samples, again), rows_per_chunk, "the reused state encodes the clip again")
    var moved = 0
    for (a, b in rows, again) {
        moved += a != b ? 1 : 0
    }
    t |> equal(moved, 0, "the reused state reads the same rows bit for bit")
    for (i, s in count(), samples) {
        s = 0.3 * sin(float(i) * 0.11)
    }
    delete again
    encode_audio_(e, st, samples, again)
    moved = 0
    for (a, b in rows, again) {
        moved += a != b ? 1 : 0
    }
    t |> success(moved > length(rows) / 2, "another clip reads other rows ({moved} of {length(rows)} differ) - the control")
    samples |> resize(16000 * 35)
    delete again
    t |> equal(encode_audio_(e, st, samples, again), 2l * rows_per_chunk, "a 35 s clip is two chunks, their rows appended")
    let h0 = heap_bytes_allocated()
    for (_i in range(2)) {
        delete again
        encode_audio_(e, st, samples, again)
    }
    let h1 = heap_bytes_allocated()
    t |> success(h1 <= h0 + 262144ul, "two more encodes leave the heap where it stood ({h0} -> {h1} bytes) - an encode frees its mel and frames")
    delete again
    delete rows
    delete samples
    delete st
}

[test]
def test_audio_embedder_tower_arm(t : T?) {
    t |> run("an ultravox mmproj loads as the whisper-class tower") @(t : T?) {
        with_dasllama_jobque() {
            tower_arm_cell(t, "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", "ultravox", 2048l, 187l)
        }
    }
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
    t |> run("the Qwen2.5-Omni mmproj's audio side loads as the whisper-class tower beside its vision tensors") @(t : T?) {
        with_dasllama_jobque() {
            tower_arm_cell(t, "mmproj-Qwen2.5-Omni-3B-f16.gguf", "qwen2a", 2048l, 750l)
        }
    }
}

// The pre-encoded audio rows on a PLAIN chat (no tower): the E2B family hears through this seam,
// and every refusal at it is a panic the caller must never reach by accident.
// The rows turn with thinking OFF under the chat sampler preset: the E-series opens a media turn
// with a channel marker even so, and the instruct-mode guard lets the reply through.
def private instruct_audio_turn(t : T?; m : Model; rows : array<float>; n : int64) {
    var chat <- create_chat(m, "", 96l)
    set_thinking(chat, false)
    set_seed(chat.session, 1234)
    var rows_d := rows
    add_user_audio_rows(m, chat, rows_d, n)
    add_user(chat, "What did you hear?")
    var pieces = 0l
    let reply = respond(m, chat, SamplingParams(temp = 0.7, top_k = 20l, top_p = 0.95)) $(_piece) {
        pieces++
        return true
    }
    t |> success(pieces > 0l && !empty(reply), "the instruct-mode audio turn answers: {length(reply)} chars over {pieces} pieces")
    delete chat
}

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
        with_dasllama_jobque() {
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
            instruct_audio_turn(t, m, rows, n)
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
        with_dasllama_jobque() {
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
            // an encoder whose span takes no marker (ultravox) owes the template none: the same rows queue
            var bare_rows : array<float>
            bare_rows |> resize(int(m.config.dim) * 2)
            add_user_audio_rows(m, chat, bare_rows, 2l, true)
            t |> equal(chat.n_audio_rows, 2l, "a bare span's rows queue on a template with no audio marker")
            delete chat
        }
    }
}
