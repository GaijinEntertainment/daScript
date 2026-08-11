options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings
require math

// Tutorial dasLLAMA-12: Voice Activity Detection
//
// This tutorial covers:
//   - Silero-VAD: a trained speech gate, weights checked in — runs with no downloads
//   - vad_probs — the per-chunk speech-probability curve; why an energy gate is not enough
//   - find_speech_timestamps + VadOpts — cutting audio into speech segments
//   - Streaming: VadIter events, the live-microphone shape
//   - VAD-gated transcription (needs an ASR model; skipped without one)
//
// Prerequisites: none for sections 1-3; tutorial 07 and an ASR model for section 4.
//
// Run: daslang.exe -jit tutorials/dasLLAMA/12_voice_activity.das
//      (own audio:  ... -- <audio.wav>            — 16 kHz mono PCM16)
//      (section 4:  ... -- [audio.wav] <asr model file(s)> — see tutorial 07)

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

// Synthetic signal appended to s: half a second of silence (the model is stateful — it
// judges sound in context, so every span gets one), then one second of a fake vowel
// (formant-shaped harmonics, syllable-rate AM), uniform noise, or a pure tone.
def push_fake(var s : array<float>; kind : string) {
    let n = 16000
    let base = length(s) + 8000   // the half-second of leading silence
    s |> resize(base + n)
    var rng = 12345
    for (i in range(n)) {
        let t = float(i) / 16000.0
        var v = 0.0
        if (kind == "vowel") {
            let f0 = 110.0 + 20.0 * t
            let am = 0.55 + 0.45 * sin(2.0 * PI * 4.0 * t)
            for (h in range(1, 25)) {
                let f = f0 * float(h)
                var g = 0.06
                if (abs(f - 700.0) < 250.0) {
                    g = 0.35
                } elif (abs(f - 1200.0) < 300.0) {
                    g = 0.25
                } elif (abs(f - 2600.0) < 400.0) {
                    g = 0.12
                }
                v += g * sin(2.0 * PI * f * t)
            }
            v *= am * 0.25
        } elif (kind == "noise") {
            rng = rng * 1664525 + 1013904223
            v = float(rng & 0xFFFF) / 65536.0 * 0.3 - 0.15
        } elif (kind == "tone") {
            v = 0.3 * sin(2.0 * PI * 440.0 * t)
        }
        s[base + i] = v
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — A trained gate, not an energy gate
// ──────────────────────────────────────────────────────────────────────────
//
// vad_probs runs the model over 512-sample chunks (32 ms at 16 kHz) and returns
// one speech probability per chunk. The point of a TRAINED gate: loud is not
// speech. White noise and a pure tone carry plenty of energy — an energy
// detector fires on both; Silero pins them near zero. A formant-shaped fake
// vowel, built to mimic speech's structure, brushes the threshold — and real
// speech pins the ceiling. The gate reads structure, not volume.

def section_probs(m : VadModel; speech : array<float>) {
    var fake : array<float>
    push_fake(fake, "vowel")
    push_fake(fake, "noise")
    push_fake(fake, "tone")
    let fake_probs <- vad_probs(m, fake)
    var peaks = fixed_array(0.0, 0.0, 0.0)
    for (i in range(length(fake_probs))) {
        let span = min(i * 512 / 24000, 2)   // 1.5 s per span: silence + the fake second
        peaks[span] = max(peaks[span], fake_probs[i])
    }
    print("peak speech probability: fake vowel {peaks[0]}, noise {peaks[1]}, tone {peaks[2]}\n")
    let speech_probs <- vad_probs(m, speech)
    var peak = 0.0
    for (p in speech_probs) {
        peak = max(peak, p)
    }
    print("peak speech probability: real speech {peak}\n\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Segments: where the speech is
// ──────────────────────────────────────────────────────────────────────────
//
// find_speech_timestamps turns the probability curve into [start, end) sample
// regions with silero's hysteresis: a segment opens at prob >= threshold (0.5),
// closes only after prob stays below neg_threshold (0.35) for min_silence
// (100 ms), segments shorter than min_speech (250 ms) are dropped, and the
// boundaries get speech_pad (30 ms) of margin. default_vad_opts() is that
// tuning; override fields for a different trade.

def section_segments(m : VadModel; speech : array<float>) : array<SpeechSegment> {
    var segs <- find_speech_timestamps(m, speech, default_vad_opts())
    print("{length(segs)} speech segments:\n")
    for (g in segs) {
        print("  {float(g.start) / 16000.0} - {float(g.end) / 16000.0} s\n")
    }
    print("\n")
    return <- segs
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Streaming: the live-microphone shape
// ──────────────────────────────────────────────────────────────────────────
//
// A live capture cannot wait for the whole recording. VadIter is the online
// segmenter: feed audio in whatever chunk sizes arrive, and it emits a
// speech_start / speech_end event the moment each boundary is crossed —
// leftover samples buffer until the next call. Same hysteresis as section 2.

def section_streaming(m : VadModel; speech : array<float>) {
    var it = VadIter()
    vad_iter_reset(it, default_vad_opts())
    var fed = 0
    let n = length(speech)
    while (fed < n) {
        let step = min(1234, n - fed)   // deliberately odd-sized "capture" chunks
        var piece : array<float>
        piece |> resize(step)
        for (j in range(step)) {
            piece[j] = speech[fed + j]
        }
        vad_iter_feed(m, it, piece) $(ev) {
            print("  {ev.kind} at {float(ev.sample) / 16000.0} s (heard {float(fed) / 16000.0} s so far)\n")
        }
        fed += step
        delete piece
    }
    print("\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — The pipeline payoff: transcribe only the speech
// ──────────────────────────────────────────────────────────────────────────
//
// VAD in front of an ASR model is the standard rig: long silences never reach
// the transcriber. Each segment becomes its own transcribe() call — tutorial 07
// covers the ASR surface itself.

def section_transcribe(asr_paths : array<string>; speech : array<float>; segs : array<SpeechSegment>) {
    var am <- (length(asr_paths) >= 2 ? load_asr_model(asr_paths[0], asr_paths[1]) : load_asr_model(asr_paths[0]))
    var s = create_session(am, "en")
    for (g in segs) {
        var cut : array<float>
        cut |> resize(g.end - g.start)
        for (j in range(length(cut))) {
            cut[j] = speech[int(g.start) + j]
        }
        let text = transcribe(am, s, cut)
        print("  [{float(g.start) / 16000.0} - {float(g.end) / 16000.0} s] {text}\n")
        delete cut
    }
    delete s
    delete am
}

[export]
def main() {
    let cmd <- get_command_line_arguments()
    var wav = ""
    var asr_paths : array<string>
    for (a in cmd) {
        if (ends_with(a, ".wav")) {
            wav = a
        } elif (ends_with(a, ".bin") || ends_with(a, ".gguf")) {
            asr_paths |> push(a)
        }
    }
    // the checked-in four seconds of JFK beside the VAD weights — zero downloads
    if (empty(wav)) {
        wav = "{get_das_root()}/modules/dasLLAMA/models/jfk_ask_not.wav"
    }

    var m <- load_vad_model("{get_das_root()}/modules/dasLLAMA/models/silero_vad.bin")
    let speech <- read_wav_pcm16_mono(wav)
    print("audio: {float(length(speech)) / 16000.0} s of \"{base_name(wav)}\"\n\n")

    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead
    with_job_que() {
        setup_dasllama_jobque()
        section_probs(m, speech)
        let segs <- section_segments(m, speech)
        section_streaming(m, speech)
        if (empty(asr_paths)) {
            print("(no ASR model given — section 4 skipped; pass a model as in tutorial 07 to transcribe the segments)\n")
        } else {
            section_transcribe(asr_paths, speech, segs)
        }
    }
}
