options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kokoro
require dasllama/dasllama_unicode   // cpt_to_utf8: building a phoneme the vocabulary lacks
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require _tts_parity
require _model_tier

// The Kokoro family. Model-free: the token rule and the style-row rule over a hand-built
// vocabulary, including the drop of a symbol the vocabulary does not carry. Model-gated
// (`kokoro-82m.gguf` + `tts_oracle/kokoro/` under models_dir(), both minted by
// performance/build_tts_data.das through harness/convert_kokoro.py and harness/kokoro_oracle.py):
// the token rule against the reference pipeline, the parity rail of `_tts_parity.das` against
// the PyTorch reference with captured noise, and a facade smoke cell that speaks one sentence
// through the front end's own inventory.

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
def test_kokoro_symbols(t : T?) {
    t |> run("the vocabulary maps in order and drops what it does not carry") @(t : T?) {
        var inscope fam = KokoroFamily()
        fam.sym['a'] = 5
        fam.sym['b'] = 7
        fam.sym[' '] = 16
        var inscope known <- kokoro_token_ids(fam, "a b")
        t |> equal(length(known), 5, "a pad on each side of three known symbols")
        t |> equal(known[0], 0)
        t |> equal(known[1], 5)
        t |> equal(known[2], 16)
        t |> equal(known[3], 7)
        t |> equal(known[4], 0)
        var inscope withOov <- kokoro_token_ids(fam, "a{cpt_to_utf8(690)} b")
        var same = length(withOov) == length(known)
        if (same) {
            for (x, y in known, withOov) {
                same = same && x == y
            }
        }
        t |> success(same, "an unknown symbol leaves no token behind")
        var inscope none <- kokoro_token_ids(fam, "zzz")
        t |> equal(length(none), 2, "nothing known: the two pads alone")
    }
    t |> run("the style row is the phoneme count less one, capped at the last row") @(t : T?) {
        t |> equal(kokoro_style_row("abc", 510), 2)
        t |> equal(kokoro_style_row("", 510), 0, "an empty string clamps up to row zero")
        t |> equal(kokoro_style_row("abcdef", 3), 2, "a long string clamps down to the last row")
    }
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
        var inscope ids <- kokoro_token_ids(g_family, "hˈɛlO wˈɜɹld")   // nolint:STYLE039
        t |> equal(length(ids), 14, "two pads around twelve known symbols")
        t |> equal(ids[0], 0)
        t |> equal(ids[length(ids) - 1], 0)
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
            var inscope m <- load_tts_model(gguf_path())
            var inscope c <- caps(m)
            t |> equal(c.sample_rate, 24000)
            t |> equal(length(c.voices), 28)   // the American and British packs; the other languages wait for their front end
            t |> success(length(c.langs) == 2 && c.langs[0] == "en-us" && c.langs[1] == "en-gb",
                "{length(c.langs)} language(s), first '{c.langs[0]}'")
            t |> equal(length(c.voice_langs), length(c.voices), "one language per listed voice")
            var british = 0
            for (lang in c.voice_langs) {
                t |> success(find_index(c.langs, lang) >= 0, "a listed voice speaks a declared language ({lang})")
                british += lang == "en-gb" ? 1 : 0
            }
            t |> equal(british, 8, "the British packs")
            let emma = find_index(c.voices, "bf_emma")
            t |> success(emma >= 0 && c.voice_langs[emma] == "en-gb", "bf_emma is listed as British")
            var inscope plain <- tts_chunks(m, "hello how are you")
            t |> success(length(plain) == 1 && plain[0] == "hello how are you", "a Kokoro chunk carries no appended mark")
            var inscope audio <- synthesize(m, "The quick brown fox jumps over the lazy dog, twice.", "af_heart")
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
    t |> run("a British voice speaks the British inventory, and the vocabulary carries it") @(t : T?) {
        if (!stat(gguf_path()).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid ||
            !stat(path_join(models_dir(), "tts_postag.bin")).is_valid) {
            t |> skip("kokoro-82m.gguf or the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope m <- load_tts_model(gguf_path())
            let line = "The doctor barked orders at the water tower."
            let ps_us = strip(tts_phonemize(m, line, "en-us"))
            let ps_gb = strip(tts_phonemize(m, line, "en-gb"))
            to_log(LOG_INFO, "kokoro dialects: us {ps_us}\n                gb {ps_gb}\n")
            t |> equal(ps_us, "ðə dˈɑktəɹ bˈɑɹkt ˈɔɹdəɹz æt ðə wˈɔTəɹ tˈWəɹ.")   // nolint:STYLE039
            t |> equal(ps_gb, "ðə dˈɒktə bˈɑːkt ˈɔːdəz at ðə wˈɔːtə tˈWə.")   // nolint:STYLE039
            // every British symbol has to be in the model's own vocabulary, or the token
            // builder drops it silently and the voice speaks a shortened line
            var inscope cps <- utf8_to_cpts(ps_gb)
            var inscope ids_gb <- kokoro_token_ids(m.kokoro, ps_gb)
            var inscope ids_us <- kokoro_token_ids(m.kokoro, ps_us)
            t |> equal(length(ids_gb), length(cps) + 2, "no British symbol falls out of the vocabulary")
            var same_tokens = length(ids_us) == length(ids_gb)
            for (a, b in ids_us, ids_gb) {
                same_tokens = same_tokens && a == b
            }
            t |> success(!same_tokens, "the two dialects feed the model different tokens")
            var inscope audio <- synthesize(m, line, "bf_emma")
            var finite = true
            var peak = 0.0
            for (v in audio.pcm) {
                finite = finite && abs(v) < 10.0   // a NaN fails the comparison too
                peak = max(peak, abs(v))
            }
            t |> success(finite, "every sample finite")
            t |> success(peak > 0.05 && length(audio.pcm) > 24000, "bf_emma spoke {length(audio.pcm)} samples, peak {peak}")
        }
    }
}
