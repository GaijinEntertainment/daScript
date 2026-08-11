options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity; drop for bit-exact work

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings
require math

// Tutorial dasLLAMA-07: Speech to Text
//
// This tutorial covers:
//   - The uniform ASR surface: load_asr_model sniffs the file format, no family names
//   - caps() — what a loaded model honestly supports (languages, timestamps, ...)
//   - One-shot transcription and the per-segment streaming form
//   - TranscribeSegment: timestamps, token ids, avg_logprob (confidence)
//   - Language auto-detection (lang = "auto") and detected_lang
//   - The push-chunks rail: feed / drain / flush
//
// The same verbs run every supported family; the loader tells them apart:
//   - whisper.cpp ggml bins (ggml-tiny.bin ... ggml-large-v3-turbo.bin) — 100 languages,
//     segment timestamps, translation:  https://huggingface.co/ggerganov/whisper.cpp
//   - Qwen3-ASR (and Qwen3-Omni) GGUF decoder + its mmproj (pass both paths) — the model
//     detects and reports the language itself, supports context biasing, no timestamps
//   - Gemma-4 E-series GGUF decoder + its Conformer mmproj — the same two-path shape
//   - Canary-Qwen GGUF decoder + its "CNRY" FastConformer bin — the same two-path shape
//   - Parakeet-TDT ggml bins (converted from the NVIDIA .nemo) — v2 English, v3 25-language
// A prepared .dlim image loads by either route as well — it routes on its baked family tag.
//
// Prerequisites: an ASR model file and a 16 kHz mono PCM16 WAV (e.g. whisper.cpp's
// samples/jfk.wav). For any-format decode or microphone capture, pair this with the
// dasAudio tutorials (11_recording.das records straight to WAV).
//
// Run: daslang.exe -jit tutorials/dasLLAMA/07_speech_to_text.das -- <model> <audio.wav>
//      (two-file families: daslang.exe -jit ... -- <decoder.gguf> <mmproj> <audio.wav>)

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

def pad2(v : int64) => v < 10l ? "0{v}" : "{v}"

def fmt_ts(cs : int64) : string {   // centiseconds -> mm:ss.cc
    return "{pad2(cs / 6000l)}:{pad2(cs / 100l % 60l)}.{pad2(cs % 100l)}"
}

[export]
def main {  // nolint:STYLE038 - one linear tutorial walkthrough
    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — Load a model (the loader sniffs the format)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // One entry point for every family. A ggml bin routes to whisper or parakeet by its
    // vocabulary size; a GGUF decoder needs its audio-encoder mmproj as the second path.
    let cmd <- get_command_line_arguments()
    let paths <- [for (a in cmd); a; where ends_with(a, ".bin") || ends_with(a, ".gguf") || ends_with(a, ".wav")]
    if (length(paths) < 2) {
        print("usage: daslang.exe -jit tutorials/dasLLAMA/07_speech_to_text.das -- <model> [mmproj.gguf] <audio.wav>\n")
        return
    }
    let audio_path = paths[length(paths) - 1]
    var m <- (length(paths) >= 3 ? load_asr_model(paths[0], paths[1]) : load_asr_model(paths[0]))
    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead

    // ──────────────────────────────────────────────────────────────────────────
    // Section 2 — caps(): ask, don't assume
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Capabilities are advisory — they let a UI grey out what the model can't do. Invalid
    // requests still panic loudly at the call site; nothing is silently ignored.
    let c <- caps(m)
    let lang_desc = empty(c.languages) ? "model-detected" : "{length(c.languages)} code(s)"
    print("languages:  {lang_desc}\n")
    print("translate:  {c.translate}\n")
    print("timestamps: {c.timestamps}\n")
    print("prompt:     {c.prompt}\n\n")

    let samples <- read_wav_pcm16_mono(audio_path)
    print("audio: {float(length(samples)) / 16000.0} s\n\n")

    with_job_que() {
        setup_dasllama_jobque()

        // ──────────────────────────────────────────────────────────────────────
        // Section 3 — one-shot transcription
        // ──────────────────────────────────────────────────────────────────────
        //
        // "auto" asks whisper to detect the spoken language from the first 30 s window
        // (one extra prompt decode, then it sticks); Qwen3-ASR always detects. English-only
        // models take "en".
        let lang = empty(c.languages) || length(c.languages) > 1 ? "auto" : c.languages[0]
        var s <- create_session(m, lang)
        let text = transcribe(m, s, samples)
        print("one-shot: '{text}'\n")
        print("language: {s.detected_lang}\n\n")

        // ──────────────────────────────────────────────────────────────────────
        // Section 4 — per-segment streaming + TranscribeSegment fields
        // ──────────────────────────────────────────────────────────────────────
        //
        // The block form yields each segment as its window completes. Times are
        // centiseconds; avg_logprob is the mean per-token log-probability — a confidence
        // signal (closer to 0 = more certain). tokens holds the raw ids.
        var s2 <- create_session(m, lang)
        transcribe(m, s2, samples) $(seg) {
            print("[{fmt_ts(seg.t0)} --> {fmt_ts(seg.t1)}] ({length(seg.tokens)} tokens, avg_logprob {seg.avg_logprob})\n")
            print("    {seg.text}\n")
        }

        // ──────────────────────────────────────────────────────────────────────
        // Section 5 — the push-chunks rail (whisper only)
        // ──────────────────────────────────────────────────────────────────────
        //
        // feed() buffers audio; drain() transcribes every COMPLETE 30 s window and keeps
        // the rest pending; flush() finishes the tail. This is the shape a live audio
        // source drives (see the dasAudio capture tutorial). Whisper-only — clip-at-once
        // models say so via a loud panic, per caps().streaming.
        if (c.timestamps == AsrTimestamps.segment && !empty(c.languages) && length(c.languages) > 1) {
            var s3 <- create_session(m, "en")
            let half = length(samples) / 2
            var part <- [for (i in range(half)); samples[i]]
            feed(m, s3, part)
            var drained = 0
            drain(m, s3) $(_seg) {
                drained++
            }
            print("\nafter feeding {float(half) / 16000.0} s: {drained} segment(s) drained (needs a full 30 s window)\n")
            part |> clear()
            for (i in range(half, length(samples))) {
                part |> push(samples[i])
            }
            feed(m, s3, part)
            flush(m, s3) $(seg) {
                print("flush: '{seg.text}'\n")
            }
        }
    }
}
