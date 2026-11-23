options gen2
options _dasllama_internal = true

module _tts_postag_cells shared public

//! the TTS tokenizer's and tagger's cells on a tagger pack its caller loaded; no entry loads a pack

require dastest/testing_boost public
require dasllama/dasllama_postag
require strings
require daslib/strings_boost
require _tts_corpus

// The tokenizer and tagger on a loaded tts_postag.bin (its block's one form, `load_pos_tagger`) against the 200-sentence
// corpus: token-for-token agreement with the reference tokenizer, tag agreement with the reference tagger, and the packed
// file's header floors.

//! the loaded tokenizer matches the reference tokenizer token for token, spacing included, on all 200 corpus sentences, and
//! splits contractions, abbreviations, dashes and possessives exactly on extra shapes
def private test_tokenizer(t : T?; tg : PosTagger) {
    unsafe {
        t |> run("token-for-token with the reference tokenizer on 200 normalized sentences") @capture(& tg)(t : T ?) {
            let rows <- load_tts_corpus()
            var mismatches = 0
            for (r in rows) {
                var inscope toks <- pos_tokenize(tg, r.norm)
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
        t |> run("shapes the corpus does not cover") @capture(& tg)(t : T ?) {
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
}

//! the packed tagger meets its header floors, agrees with the reference tagger on at least 96 percent of corpus tokens and
//! 80 percent of heteronym words, and tags a pinned sentence exactly
def private test_tagger(t : T?; tg : PosTagger) {
    unsafe {
        t |> run("the packed file's own floors") @capture(& tg)(t : T ?) {
            t |> success(tg.test_accuracy >= 0.93, "test-split accuracy {tg.test_accuracy}")
            t |> success(length(tg.tags) >= 40, "{length(tg.tags)} tags")
            t |> success(length(tg.feat_hash) > 10000, "{length(tg.feat_hash)} features")
            t |> success(length(tg.exceptions) > 500, "{length(tg.exceptions)} tokenizer exceptions")
            t |> equal(tg.tagdict?["the"] ?? -1, find_index(tg.tags, "DT"))
        }
        t |> run("tag agreement with the reference tagger on the corpus") @capture(& tg)(t : T ?) {
            let rows <- load_tts_corpus()
            var n = 0
            var same = 0
            var het_n = 0
            var het_same = 0
            for (r in rows) {
                var inscope words <- [for (tok in r.tokens); tok[0]]
                var inscope tags <- pos_tag_words(tg, words)
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
            t |> success(rate >= 0.96, "tag agreement {rate} over {n} tokens")
            t |> success(het_same * 10 >= het_n * 8, "heteronym-word agreement {het_same}/{het_n}")
        }
        t |> run("spelled numbers, punctuation and the determiner") @capture(& tg)(t : T ?) {
            let tags <- pos_tag_words(tg, ["It", "costs", "twelve", "hundred", "dollars", ",", "a", "lot", "."])
            t |> equal(join(tags, " "), "PRP VBZ CD CD NNS , DT NN .")
        }
    }
}

//! the tokenizer's and the tagger's cells on `tg`, the loaded tagger pack
def tts_postag_cells(t : T?; tg : PosTagger) {
    test_tokenizer(t, tg)
    test_tagger(t, tg)
}
