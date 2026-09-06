options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kitten
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_styletts2
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/defer
require daslib/fio
require math
require _tts_corpus
require _tts_parity
require _model_tier

// The KittenTTS family. Model-free: the symbol map (the front end's inventory into the
// espeak-style IPA the models consume) over the 200-sentence corpus, and the reference driver's
// token rule. Model-gated (`kitten-<size>.gguf` + `tts_oracle/kitten_<size>/` under models_dir(),
// both minted by performance/build_tts_data.das through harness/convert_kitten.py and
// harness/kitten_oracle.py): the parity rail of `_tts_parity.das` per size, and a facade smoke
// cell that speaks one sentence.

[test]
def test_symbol_map(t : T?) {
    t |> run("the corpus rewrites symbol for symbol") @(t : T?) {
        var inscope rows <- load_tts_corpus()
        var mismatches = 0
        for (r in rows) {
            let got = kitten_phonemes_from_misaki(r.ps_misaki)
            if (got != r.ps_espeak) {
                mismatches++
                t |> failure("{r.id}: got  {got}\n       want {r.ps_espeak}")
            }
        }
        t |> equal(mismatches, 0, "sentences rewritten differently")
    }
    t |> run("the calibrated rules") @(t : T?) {
        t |> equal(kitten_phonemes_from_misaki("kəmpjˈuTəɹ"), "kəmpjˈuːɾɚ")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("bˈɜɹd"), "bˈɜːd")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("ʧˈOk"), "tʃˈoʊk")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("ɹˈɪTᵊn"), "ɹˈɪɾən")   // nolint:STYLE039
    }
    t |> run("the token rule") @(t : T?) {
        // pad, "h", "i", space, ",", space, "w", space, ".", ellipsis, pad - punctuation is its own token
        var inscope ids <- kitten_token_ids("hi, w.")
        t |> equal(length(ids), 11)
        t |> equal(ids[0], 0)
        t |> equal(ids[3], 16, "the space symbol")
        t |> equal(ids[4], 3, "the comma")
        t |> equal(ids[length(ids) - 2], 10, "the ellipsis closes")
        t |> equal(ids[length(ids) - 1], 0)
        t |> equal(kitten_style_row("abc", 400), 3)
        t |> equal(kitten_style_row("", 400), 0)
    }
}

def private oracle_dir(size : string) : string {
    return path_join(models_dir(), "tts_oracle/kitten_{size}")
}

def private gguf_path(size : string) : string {
    return path_join(models_dir(), "kitten-{size}.gguf")
}

// the front end's espeak-inventory phonemes, as the fixture carries them, into the driver's ids
def private kitten_ids_of(ps : string) : array<int> {
    return <- kitten_token_ids(ps)
}

// text in, speech out through the facade: the front end's packs sit beside the GGUF in the store
def private facade_smoke(t : T?; size : string) {
    if (!stat(gguf_path(size)).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid ||
        !stat(path_join(models_dir(), "tts_postag.bin")).is_valid) {
        t |> skip("kitten-{size}.gguf or the front-end packs not present")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var inscope m <- load_tts_model(gguf_path(size))
        var inscope c <- caps(m)
        t |> equal(c.sample_rate, 24000)
        t |> equal(length(c.voices), 8)
        var inscope audio <- synthesize(m, "The quick brown fox jumps over the lazy dog, twice.", "Bella")
        t |> equal(audio.sample_rate, 24000)
        // 51 characters of text: well over a second of speech, under ten
        t |> success(length(audio.pcm) > 24000 && length(audio.pcm) < 240000, "{length(audio.pcm)} samples")
        var finite = true
        var peak = 0.0
        for (v in audio.pcm) {
            finite = finite && abs(v) < 10.0   // a NaN fails the comparison too
            peak = max(peak, abs(v))
        }
        t |> success(finite, "every sample finite")
        t |> success(peak > 0.05, "peak {peak}: the model spoke")
        let tm & = audio.timings
        t |> success(tm.total_us > 0l && tm.generator_us > 0l && tm.decoder_us > 0l && tm.front_end_us > 0l, "the stage clocks ran")
        t |> success(tm.total_us >= tm.front_end_us + tm.bert_us + tm.decoder_us + tm.generator_us, "the total covers its parts")
        to_log(LOG_INFO, "{size} facade: {length(audio.pcm)} samples, peak {peak}; {timings_line(tm, audio.sample_rate)}\n")
    }
}

[test]
def test_kitten_facade(t : T?) {
    t |> run("nano: text in, speech out") @(t : T?) {
        facade_smoke(t, "nano")
    }
}

// the generator hook under test: declines, or serves a silent waveform of the CPU chain's length
var private g_hook_serves = false

[unused_argument(d, x_rows, c, s, har_spec, har_frames)]
def private generator_stub(d : St2Decoder; x_rows : array<float>; c, t : int64; s : array<float>;
                           har_spec : array<float>; har_frames : int64; var wave : array<float>) : bool {
    if (!g_hook_serves) {
        return false
    }
    wave |> resize(t * 600l)
    for (i in range64(t * 600l)) {
        wave[i] = 0.0
    }
    return true
}

struct HookProbe {
    calls_after_decline : int
    served_after_decline : int
    spoke_after_decline : bool
    served_after_serve : int
    silent_after_serve : bool
}

def private hook_probe(path : string) : HookProbe {
    var r = HookProbe()
    var inscope m <- load_tts_model(path)
    register_styletts2_generator_gpu(@@generator_stub)
    defer() {
        unregister_styletts2_generator_gpu()   // no driver fills the slot: the seat goes back empty
    }
    g_hook_serves = false
    let s0 = styletts2_generator_gpu_stats()
    var inscope a <- synthesize(m, "The hook declines and the model speaks.", "Bella")
    let s1 = styletts2_generator_gpu_stats()
    r.calls_after_decline = s1.calls - s0.calls
    r.served_after_decline = s1.served - s0.served
    var peak = 0.0
    for (v in a.pcm) {
        peak = max(peak, abs(v))
    }
    r.spoke_after_decline = peak > 0.05
    g_hook_serves = true
    var inscope b <- synthesize(m, "The hook serves silence.", "Bella")
    let s2 = styletts2_generator_gpu_stats()
    r.served_after_serve = s2.served - s1.served
    peak = 0.0
    for (v in b.pcm) {
        peak = max(peak, abs(v))
    }
    r.silent_after_serve = peak == 0.0 && !empty(b.pcm)
    g_hook_serves = false
    return r
}

[test]
def test_kitten_generator_hook(t : T?) {
    t |> run("nano: the generator hook declines into the CPU chain and serves when it answers") @(t : T?) {
        if (!stat(gguf_path("nano")).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid) {
            t |> skip("kitten-nano.gguf or the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            let r = hook_probe(gguf_path("nano"))
            t |> equal(r.calls_after_decline, 1, "one chunk, one hook call")
            t |> equal(r.served_after_decline, 0, "a decline serves nothing")
            t |> success(r.spoke_after_decline, "the CPU chain spoke after the decline")
            t |> equal(r.served_after_serve, 1, "the serving hook is counted")
            t |> success(r.silent_after_serve, "the served waveform reached the facade")
        }
    }
}

// ARCHITECTURE_TTS.md sec.2.32: q8 is the served default, f32 the reference lane the parity
// rail holds against; a pin resets from the exact pin it took.
[test]
def test_tts_lane_pin(t : T?) {
    t |> run("the served lane: q8 un-pinned, either lane on demand, the default back on reset") @(t : T?) {
        {
            defer() {
                reset_tts_q8()
            }
            t |> success(tts_serves_q8(), "the un-pinned default serves q8")
            set_tts_q8(false)
            t |> success(!tts_serves_q8(), "the f32 pin is the reference lane")
            t |> success(!styletts2_serves_q8(), "the facade's pin reaches the family")
            reset_tts_q8()
            t |> success(tts_serves_q8(), "reset from the f32 pin returns to the policy default")
            set_tts_q8(true)
            t |> success(tts_serves_q8(), "the q8 pin")
            reset_tts_q8()
            t |> success(tts_serves_q8(), "reset from the q8 pin returns to the policy default")
        }
    }
}

// The reference pipeline truncates a chunk past the encoder's positions and logs it; nothing on
// the served path may panic on a long one.
[test]
def test_kitten_over_long_chunk(t : T?) {
    t |> run("nano: a chunk past the encoder's positions is truncated, not refused") @(t : T?) {
        if (!stat(gguf_path("nano")).is_valid) {
            t |> skip("kitten-nano.gguf not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope m <- load_styletts2(gguf_path("nano"))
            let cap = m.albert.max_pos
            var inscope ids : array<int>
            ids |> resize(int(cap) + 64)
            for (i in range(length(ids))) {
                ids[i] = 1 + i % (int(m.n_symbols) - 1)
            }
            var inscope style : array<float>
            styletts2_style(m, m.voice_names[0], 0, style)
            var inscope noise = TtsNoise()
            var tm = TtsTimings()
            var inscope sc = St2Scratch()
            let samples = styletts2_synthesize(m, ids, style, 1.0, noise, 0x5eedul, null, tm, sc)
            t |> success(samples > 0l, "{long_length(ids)} ids past the encoder's {cap} positions spoke {samples} samples")
            t |> equal(tm.samples, samples, "the timings count the samples the truncated chunk produced")
        }
    }
}

[test]
def test_kitten_oracle(t : T?) {
    for (size in ["nano", "mini"]) {
        t |> run("{size} against onnxruntime, captured noise") @(t : T?) {
            tts_parity(t, TtsParityRig(family = "kitten-tts", gguf = gguf_path(size), oracle_dir = oracle_dir(size), ids = @@kitten_ids_of))
        }
    }
}
