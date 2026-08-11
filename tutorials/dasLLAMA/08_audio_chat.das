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
// Qwen3-Omni and Gemma-4 E-series audio are served too, but through the ASR surface
// (tutorial 07's two-path load_asr_model), not load_audio_tower.
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
// for you, and a custom modality builds it by hand.

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
def main {
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
    var m <- load_model(ggufs[0], QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, 4096l)
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
    }
}
