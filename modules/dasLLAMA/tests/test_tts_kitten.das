options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kitten
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
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
        let rows <- load_tts_corpus()
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
        let ids <- kitten_token_ids("hi, w.")
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
        let m <- load_tts_model(gguf_path(size))
        let c <- caps(m)
        t |> equal(c.sample_rate, 24000)
        t |> equal(length(c.voices), 8)
        let audio <- synthesize(m, "The quick brown fox jumps over the lazy dog, twice.", "Bella")
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

[test]
def test_kitten_oracle(t : T?) {
    for (size in ["nano", "mini"]) {
        t |> run("{size} against onnxruntime, captured noise") @(t : T?) {
            tts_parity(t, TtsParityRig(family = "kitten-tts", gguf = gguf_path(size), oracle_dir = oracle_dir(size), ids = @@kitten_ids_of))
        }
    }
}
