options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity; drop for bit-exact work

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings
require math

// Tutorial dasLLAMA-08: Audio Chat
//
// This tutorial covers:
//   - Audio towers: load_audio_tower reads a multimodal projector (mmproj) GGUF
//   - create_chat(model, tower) — a conversation that can hear
//   - add_user_audio + add_user composing one turn; respond() streams the reply
//   - add_user_audio_rows: pre-encoded carrier rows onto a plain chat (the only chat rail for gemma-4 E-series)
//   - add_user_span + media_body_rows + eval_embd_body: the audio in its place, prefilled as one body
//   - caps(model) — LlmCaps, the chat layer's honesty contract
//
// Audio-capable chat models pair a normal text decoder with a whisper-family audio
// encoder ("tower"): the tower turns 16 kHz PCM into soft tokens the decoder reads
// inline with text. Supported pairs (decoder + mmproj):
//   - Qwen2-Audio-7B-Instruct (self-converted; see modules/dasLLAMA/README.md)
//   - Qwen2.5-Omni 3B/7B (ggml-org GGUFs; audio only — vision is out of scope)
//   - Ultravox v0.5 (STOCK Llama-3.2-1B / 3.1-8B decoders + the ultravox mmproj)
//   - Voxtral-Mini-3B (self-converted)
// The chat template picks the audio framing automatically (qwen2 family wraps the span
// in <|audio_bos|>/<|audio_eos|>, llama-3 splices bare embeddings, voxtral opens with
// [BEGIN_AUDIO]) — the calls below are identical for every pair.
// Qwen3-Omni and Gemma-4 E-series audio are served too, but not by load_audio_tower:
// tutorial 07's two-path load_asr_model transcribes them, and a Gemma-4 E-series pair
// chats here through the AudioEmbedder carrier rail at the end of this file.
//
// Prerequisites: a decoder GGUF + its mmproj GGUF + a 16 kHz mono PCM16 WAV.
//
// Run: daslang.exe -jit tutorials/dasLLAMA/08_audio_chat.das -- <decoder.gguf> <mmproj.gguf> <audio.wav> [prompt]

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Under the hood: the splice
// ──────────────────────────────────────────────────────────────────────────
//
// generate() and generate_embd() meet at the embedding rows: prefilling tokens
// IS prefilling their embedding rows. Proof by identity — same prompt, both
// roads, greedy, byte-identical output. An audio turn is this exact array with
// a span of rows replaced by the tower's soft tokens; add_user_audio builds it
// for you, and a custom modality builds it by hand. A scheduler serving many
// streams builds it by hand too: render_turn_audio renders the turn's tokens
// as two spans — head before the audio rows, tail after, split exactly at the
// template's audio marker (render_turn_image's audio twin) — and splices the
// encoder's rows between them. Unlike an image span, audio rows stay CAUSAL:
// sound has a left-to-right order.

def section_splice(m : Model) {
    let prompt <- encode(m, "The capital of France is")
    let n = long_length(prompt)
    var s1 = create_session(m)
    var out1 = ""
    generate(m, s1, prompt, SamplingParams(), 8l) $(_id, piece) {
        out1 += piece
        return true
    }
    var embd : array<float>
    embd |> resize(n * m.config.dim)
    embed_text_rows(m, prompt, embd, 0l)   // the rows generate() would prefill
    var s2 = create_session(m)
    var out2 = ""
    generate_embd(m, s2, embd, n, SamplingParams(), 8l) $(_id, piece) {
        out2 += piece
        return true
    }
    print("generate:      \"{out1}\"\ngenerate_embd: \"{out2}\"\nidentical: {out1 == out2}\n")
    delete s1
    delete s2
}

// The scheduler's shape, live: a render-only chat carries the same turn, and
// render_turn_audio hands back the two spans — everything before the splice
// point, everything after — with the template's audio markers in place. No KV,
// no encoder: the caller splices its own rows between them and prefills.
def section_render_spans(m : Model; prompt : string) {
    var rchat <- create_chat_renderer(m, "", 1l)
    add_user(rchat, prompt)
    var head : array<int64>
    var tail : array<int64>
    render_turn_audio(m, rchat, head, tail)
    print("render_turn_audio: head {long_length(head)} tokens | <audio rows splice here> | tail {long_length(tail)} tokens\n")
    delete head
    delete tail
    delete rchat
}

// And the rows themselves: the AudioEmbedder carrier is the family-neutral encoder a
// scheduler owns. It serves every audio mmproj this tutorial takes - the
// load_audio_tower families and the gemma-4 E-series alike - and audio_probe_proj_dim
// answers 0 only for a file no audio family serves. load_audio_embedder + encode_audio
// turn the PCM into the soft-token rows that splice between the two spans. The
// pre-encoded-rows seam closes the loop: add_user_audio_rows moves those rows onto a
// PLAIN chat (no tower attached) and respond() runs the spliced turn - the shape a
// scheduler that owns its encoder uses, and how a gemma-4 E-series pair hears at the
// chat layer at all. audio_span_bare says whether the span takes markers: an ultravox
// span sits bare in a stock Llama template, which knows no audio marker.
def section_carrier_encode(m : Model; mmproj_path : string; audio_path : string; samples : array<float>; prompt : string) {
    if (audio_probe_proj_dim(mmproj_path) == 0l) {
        print("carrier: no audio family serves {mmproj_path} - skipping the encode demo\n")
        return
    }
    var inscope e <- load_audio_embedder(mmproj_path)
    var st = AudioState()
    var rows : array<float>
    let n = encode_audio(e, st, samples, rows)
    let bare = audio_span_bare(e)
    print("encode_audio ({audio_family(e)}): {float(length(samples)) / 16000.0}s -> {n} soft tokens x {audio_proj_dim(e)} wide, bare span: {bare}\n")
    var chat <- create_chat(m, "", 96l)
    var chat_rows := rows
    add_user_audio_rows(m, chat, chat_rows, n, bare)   // moves the rows in; the turn renders its audio span around them
    add_user(chat, prompt)
    print("carrier chat > {prompt}\n")
    respond(m, chat, SamplingParams()) $(piece_text) {
        print("{piece_text}")
        return true
    }
    print("\n\n")
    delete chat
    delete st
    section_inline_body(m, hash(audio_path), rows, n, bare, prompt)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — The audio in its place: one prefill body by hand
// ──────────────────────────────────────────────────────────────────────────
//
// So far the audio led the turn. add_user_span puts it where it sits in the
// message: text_before bytes into the turn's text. The span carries the
// media's content key - the same clip, the same key - and render_turn then
// lays n_rows media position ids where the rows go: negative numbers no vocab
// id uses, so a prefix cache matches across the span as it does across text.
//
// A program that owns its encoder and scheduler prefills that turn as ONE
// body instead of three evals (text, rows, text). media_body_rows builds the
// body's rows - head text, the media rows, tail text - and eval_embd_body
// prefills them. An empty span (0, 0) keeps every row causal, as audio wants;
// an image passes its span and its grid. On a gemma-4 E-series decoder the
// session also takes each text row's token id, so text rows get their own
// per-layer input; eval_embd_body spends the ids.
def section_inline_body(m : Model; key : uint64; rows : array<float>; n : int64; bare : bool; prompt : string) {
    let lead = "Here is a recording. "
    var rchat <- create_chat_renderer(m, "", 64l)
    add_user(rchat, "{lead}{prompt}")
    add_user_span(m, rchat, key, n, length(lead), false, bare)
    var inscope turn <- render_turn(m, rchat)
    var lo = 0l
    while (!is_media_position(turn[lo])) {
        lo ++
    }
    var inscope head <- [for (k in range64(lo)); turn[k]]
    var inscope tail <- [for (k in range64(lo + n, long_length(turn))); turn[k]]
    print("add_user_span: {long_length(head)} text tokens | {n} media ids | {long_length(tail)} text tokens\n")
    var s <- create_session(m)
    var body : array<float>
    media_body_rows(m, s, head, rows, n, m.config.dim, tail, body)
    eval_embd_body(m, s, body, long_length(turn), 0l, 0l, int2(0))
    print("one body > {lead}<audio>{prompt}\n")
    let stop_ids <- turn_stop_ids(rchat)
    var guard <- make_nothink_guard(rchat)
    var one <- [0l]
    for (_step in range64(64l)) {
        let tok = sample(s, SamplingParams())
        let text = piece(m, tok)
        break if (find_index(stop_ids, tok) >= 0 || nothink_stop_here(guard, tok, text))
        print("{text}")
        one[0] = tok
        eval(m, s, one)
    }
    print("\n\n")
    delete body
    delete s
    delete rchat
}

// Minimal RIFF/WAVE reader for 16-bit PCM mono 16 kHz input — enough for the tutorial;
// real programs decode arbitrary formats through dasAudio's miniaudio bindings.
def private wav_u16(b : array<uint8> | #; o : int64) : uint {
    return uint(b[o]) | (uint(b[o + 1l]) << 8u)
}

def private wav_u32(b : array<uint8> | #; o : int64) : uint {
    return wav_u16(b, o) | (wav_u16(b, o + 2l) << 16u)
}

def read_wav_pcm16_mono(path : string) : array<float> {
    var out : array<float>
    let f = fopen(path, "rb")
    if (f == null) {
        panic("cannot open '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let n = long_length(bytes)
        var o = 12l                      // "RIFF" size "WAVE"
        while (o + 8l <= n) {
            let id = wav_u32(bytes, o)
            let sz = int64(wav_u32(bytes, o + 4l))
            if (id == 0x61746164u) {     // "data"
                let cnt = min(sz, n - o - 8l) / 2l
                out |> resize(cnt)
                for (i in range64(cnt)) {
                    let u = int(wav_u16(bytes, o + 8l + i * 2l))
                    out[int(i)] = float(u >= 32768 ? u - 65536 : u) / 32768.0
                }
                break
            }
            o += 8l + sz + (sz & 1l)
        }
    }
    fclose(f)
    return <- out
}

[export]
def main {  // nolint:STYLE038 - one linear tutorial walkthrough; the carrier rail is its first fork
    let cmd <- get_command_line_arguments()
    var ggufs : array<string>
    var audio_path = ""
    var prompt = "What is being said in this audio?"
    for (a in cmd) {
        if (ends_with(a, ".gguf")) {
            ggufs |> push(a)
        } elif (ends_with(a, ".wav")) {
            audio_path = a
        } elif (!empty(audio_path) && !starts_with(a, "-")) {
            prompt = a
        }
    }
    if (length(ggufs) < 2 || empty(audio_path)) {
        print("usage: daslang.exe -jit tutorials/dasLLAMA/08_audio_chat.das -- <decoder.gguf> <mmproj.gguf> <audio.wav> [prompt]\n")
        return
    }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — load the decoder and its audio tower
    // ──────────────────────────────────────────────────────────────────────────
    //
    // The decoder is any chat model dasLLAMA runs. The tower is that model's mmproj —
    // the pair must match: the tower projects into the decoder's embedding width, and
    // create_chat panics on a mismatched pair rather than degrading.
    //
    // Two rails, decided by the mmproj: a file load_audio_tower serves
    // (audio_tower_probe_proj_dim answers its width) runs every section below; any other
    // file - the gemma-4 E-series - demos only the scheduler-side carrier path and returns.
    var m <- load_model(ggufs[0], QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, 4096l)
    if (audio_tower_probe_proj_dim(ggufs[1]) == 0l) {
        allow_cpu_prefill()
        let carrier_samples <- read_wav_pcm16_mono(audio_path)
        with_job_que() {
            setup_dasllama_jobque()
            section_render_spans(m, prompt)
            section_carrier_encode(m, ggufs[1], audio_path, carrier_samples, prompt)
        }
        print("this mmproj serves through the AudioEmbedder carrier only - the create_chat sections need a load_audio_tower pair (qwen2a / omni / ultravox / voxtral)\n")
        return
    }
    var tower <- load_audio_tower(ggufs[1])
    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead

    // caps(model) is the chat layer's honesty contract. First entry: gemma has no system
    // role — the chat layer folds the system prompt into the first user turn, and
    // system_prompt=false is how you find out instead of being silently absorbed.
    let lc = caps(m)
    print("decoder arch {m.arch}; system_prompt supported: {lc.system_prompt}\n")

    let samples <- read_wav_pcm16_mono(audio_path)
    print("audio: {float(length(samples)) / 16000.0} s\n---\n")

    with_job_que() {
        setup_dasllama_jobque()

        // ──────────────────────────────────────────────────────────────────────
        // Section 2 — a chat that hears: audio + text in one turn
        // ──────────────────────────────────────────────────────────────────────
        //
        // create_chat(model, tower) moves the tower into the chat (it owns the encoder
        // scratch too). add_user_audio encodes the clip to soft tokens NOW and queues
        // them at the head of the next turn; add_user contributes the text after the
        // audio span. respond() renders the turn — template framing, audio splice,
        // embedding prefill — and streams the reply.
        var chat <- create_chat(m, tower, "", 128l)
        add_user_audio(chat, samples)
        add_user(chat, prompt)
        respond(m, chat, SamplingParams()) $(piece) {
            print("{piece}")
            return true
        }
        print("\n---\n")
        let st = stats(chat.session)
        print("prefill {st.n_prompt} positions; {st.gen_tps} tok/s\n")

        // The conversation continues like any text chat — follow-up turns don't need
        // audio (and a later add_user_audio call can bring a second clip in).
        add_user(chat, "Answer in one word: was that speech or music?")
        respond(m, chat, SamplingParams()) $(piece) {
            print("{piece}")
            return true
        }
        print("\n---\n")

        section_splice(m)
        section_render_spans(m, prompt)
        section_carrier_encode(m, ggufs[1], audio_path, samples, prompt)
    }
}
