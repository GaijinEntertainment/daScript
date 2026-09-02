options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_postag
require strings
require daslib/strings_boost
require daslib/fio
require _tts_corpus
require _model_tier   // models_dir(): tts_postag.bin lives in the model store

// TTS tokenizer + tagger against the 200-sentence corpus: token-for-token agreement with the
// reference tokenizer, tag agreement with the reference tagger, the packed file's header
// floors, and the load-time budget. Model-gated on tts_postag.bin (harness/train_postag.py).

def private tagger_path() : string => path_join(models_dir(), "tts_postag.bin")

def private tagger_present(t : T?) : bool {
    let f = fopen(tagger_path(), "rb")
    if (f == null) {
        t |> skip("tts_postag.bin absent in {models_dir()}")
        return false
    }
    fclose(f)
    return true
}

[test]
def test_tokenizer(t : T?) {
    t |> run("token-for-token with the reference tokenizer on 200 normalized sentences") @(t : T?) {
        return if (!tagger_present(t))
        let tg <- load_pos_tagger(tagger_path())
        let rows <- load_tts_corpus()
        var mismatches = 0
        for (r in rows) {
            let toks <- pos_tokenize(tg, r.norm)
            var same = length(toks) == length(r.tokens)
            if (same) {
                for (tok, want in toks, r.tokens) {
                    same = same && tok.text == want[0] && tok.space_after == (want[2] == " ")
                }
            }
            if (!same) {
                mismatches++
                let got = build_string() $(var w) {
                    for (tok in toks) {
                        w |> write(tok.text)
                        w |> write(tok.space_after ? "| " : "|")
                    }
                }
                let want = build_string() $(var w) {
                    for (tok in r.tokens) {
                        w |> write(tok[0])
                        w |> write(tok[2] == " " ? "| " : "|")
                    }
                }
                t |> failure("{r.id}: got {got}\n      want {want}")
            }
        }
        t |> equal(mismatches, 0, "sentences tokenized differently")
    }
    t |> run("shapes the corpus does not cover") @(t : T?) {
        return if (!tagger_present(t))
        let tg <- load_pos_tagger(tagger_path())
        let toks1 <- pos_tokenize(tg, "I don't know, it's Mr. Smith's — the U.S. way... really?")   // nolint:STYLE039
        let texts1 <- [for (tok in toks1); tok.text]
        t |> equal(join(texts1, "|"), "I|do|n't|know|,|it|'s|Mr.|Smith|'s|—|the|U.S.|way|...|really|?")   // nolint:STYLE039
        let toks2 <- pos_tokenize(tg, "well-known twenty-one A. B. p.m. dogs' 'cause")
        let texts2 <- [for (tok in toks2); tok.text]
        t |> equal(join(texts2, "|"), "well|-|known|twenty|-|one|A.|B.|p.m.|dogs|'|'cause")
        t |> equal(toks2[length(toks2) - 1].space_after, false)
        t |> equal(toks2[0].space_after, false)
        t |> equal(toks2[2].space_after, true)
    }
}

[test]
def test_tagger(t : T?) {
    t |> run("the packed file's own floors") @(t : T?) {
        return if (!tagger_present(t))
        let tg <- load_pos_tagger(tagger_path())
        t |> success(tg.test_accuracy >= 0.93, "test-split accuracy {tg.test_accuracy}")
        t |> success(length(tg.tags) >= 40, "{length(tg.tags)} tags")
        t |> success(length(tg.feat_hash) > 10000, "{length(tg.feat_hash)} features")
        t |> success(length(tg.exceptions) > 500, "{length(tg.exceptions)} tokenizer exceptions")
        t |> equal(tg.tagdict?["the"] ?? -1, find_index(tg.tags, "DT"))
    }
    t |> run("tag agreement with the reference tagger on the corpus") @(t : T?) {
        return if (!tagger_present(t))
        let tg <- load_pos_tagger(tagger_path())
        let rows <- load_tts_corpus()
        var n = 0
        var same = 0
        var het_n = 0
        var het_same = 0
        for (r in rows) {
            let words <- [for (tok in r.tokens); tok[0]]
            let tags <- pos_tag_words(tg, words)
            for (tok, tag in r.tokens, tags) {
                n++
                same += tag == tok[1] ? 1 : 0
                if (!empty(r.heteronym.word) && to_lower(tok[0]) == r.heteronym.word) {
                    het_n++
                    het_same += tag == tok[1] ? 1 : 0
                }
            }
        }
        let rate = float(same) / float(n)
        to_log(LOG_INFO, "tts tagger: {same}/{n} tags agree with the reference ({rate}), heteronym words {het_same}/{het_n}\n")
        t |> success(rate >= 0.92, "tag agreement {rate} over {n} tokens")
        t |> success(het_same * 10 >= het_n * 8, "heteronym-word agreement {het_same}/{het_n}")
    }
    t |> run("spelled numbers, punctuation and the determiner") @(t : T?) {
        return if (!tagger_present(t))
        let tg <- load_pos_tagger(tagger_path())
        let tags <- pos_tag_words(tg, ["It", "costs", "twelve", "hundred", "dollars", ",", "a", "lot", "."])
        t |> equal(join(tags, " "), "PRP VBZ CD CD NNS , DT NN .")
    }
    t |> run("loads inside the budget") @(t : T?) {
        return if (!tagger_present(t))
        let t0 = ref_time_ticks()
        let tg <- load_pos_tagger(tagger_path())
        let usec = get_time_usec(t0)
        to_log(LOG_INFO, "tts tagger: loaded {length(tg.feat_hash)} features in {usec} us\n")
        t |> success(usec < 150000, "load took {usec} us")
    }
}
