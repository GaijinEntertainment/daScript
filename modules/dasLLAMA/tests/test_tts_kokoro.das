options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kokoro
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require _tts_parity
require _model_tier

// The Kokoro family. Model-gated (`kokoro-82m.gguf` + `tts_oracle/kokoro/` under models_dir(),
// both minted by performance/build_tts_data.das through harness/convert_kokoro.py and
// harness/kokoro_oracle.py): the token rule against the reference pipeline, the parity rail of
// `_tts_parity.das` against the PyTorch reference with captured noise, and a facade smoke cell
// that speaks one sentence through the front end's own inventory.

def private gguf_path() : string {
    return path_join(models_dir(), "kokoro-82m.gguf")
}

def private oracle_dir() : string {
    return path_join(models_dir(), "tts_oracle/kokoro")
}

var private g_family : KokoroFamily

def private kokoro_ids_of(ps : string) : array<int> {
    return <- kokoro_token_ids(g_family, ps)
}

[test]
def test_kokoro_oracle(t : T?) {
    t |> run("against the PyTorch reference, captured noise") @(t : T?) {
        if (!stat(gguf_path()).is_valid) {
            t |> skip("kokoro-82m.gguf not present")
            return
        }
        delete g_family
        g_family <- load_kokoro_family(gguf_path())
        t |> success(length(g_family.sym) > 100, "{length(g_family.sym)} symbols in the vocabulary")
        let ids <- kokoro_token_ids(g_family, "hˈɛlO wˈɜɹld")   // nolint:STYLE039
        t |> equal(length(ids), 14, "two pads around twelve known symbols")
        t |> equal(ids[0], 0)
        t |> equal(ids[length(ids) - 1], 0)
        t |> equal(kokoro_style_row("abc", 510), 2)
        t |> equal(kokoro_style_row("", 510), 0)
        tts_parity(t, TtsParityRig(family = "kokoro", gguf = gguf_path(), oracle_dir = oracle_dir(), ids = @@kokoro_ids_of))
    }
}

[test]
def test_kokoro_facade(t : T?) {
    t |> run("text in, speech out") @(t : T?) {
        if (!stat(gguf_path()).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid ||
            !stat(path_join(models_dir(), "tts_postag.bin")).is_valid) {
            t |> skip("kokoro-82m.gguf or the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            let m <- load_tts_model(gguf_path())
            let c <- caps(m)
            t |> equal(c.sample_rate, 24000)
            t |> success(length(c.voices) >= 50, "{length(c.voices)} voices")
            let audio <- synthesize(m, "The quick brown fox jumps over the lazy dog, twice.", "af_heart")
            t |> equal(audio.sample_rate, 24000)
            t |> success(length(audio.pcm) > 24000 && length(audio.pcm) < 240000, "{length(audio.pcm)} samples")
            var finite = true
            var peak = 0.0
            for (v in audio.pcm) {
                finite = finite && abs(v) < 10.0   // a NaN fails the comparison too
                peak = max(peak, abs(v))
            }
            t |> success(finite, "every sample finite")
            t |> success(peak > 0.05, "peak {peak}: the model spoke")
            to_log(LOG_INFO, "kokoro facade: {length(audio.pcm)} samples, peak {peak}; {timings_line(audio.timings, audio.sample_rate)}\n")
        }
    }
}
