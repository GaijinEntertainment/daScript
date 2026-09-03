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
// Model-free: the tagger's word-class and shape features against the trainer's own rules, the
// tokenizer's split rules with an empty exception table, and the missing-file panic.

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
def test_word_features(t : T?) {
    t |> run("the word class is the trainer's normalize") @(t : T?) {
        // harness/train_postag.py: `if "-" in word and word[0] != "-"` - a hyphen ANYWHERE in a
        // token that does not START with one. The packed hashes were trained through that rule
        t |> equal(pos_norm_word("well-known"), "!HYPHEN")
        t |> equal(pos_norm_word("twenty-one"), "!HYPHEN")
        t |> equal(pos_norm_word("--"), "--", "a token that starts with a hyphen is not hyphenated")
        t |> equal(pos_norm_word("-well-known"), "-well-known")
        t |> equal(pos_norm_word("-"), "-")
        t |> equal(pos_norm_word("1999"), "!YEAR")
        t |> equal(pos_norm_word("19999"), "!DIGITS")
        t |> equal(pos_norm_word("3rd"), "!DIGITS")
        t |> equal(pos_norm_word("Smith"), "smith")
        t |> equal(pos_norm_word(""), "")
    }
    t |> run("the shape feature's suffixes d, h and q") @(t : T?) {
        t |> equal(pos_word_shape("NASA"), "X")
        t |> equal(pos_word_shape("Smith"), "Xx")
        t |> equal(pos_word_shape("dogs"), "x")
        t |> equal(pos_word_shape("mp3"), "xd", "d: the token carries a digit")
        t |> equal(pos_word_shape("well-known"), "xh", "h: an interior hyphen")
        t |> equal(pos_word_shape("don't"), "xq", "q: an apostrophe")
        t |> equal(pos_word_shape("A1-b'c"), "Xxdhq", "d then h then q, in that order")
        t |> equal(pos_word_shape("-lead"), "x", "a LEADING hyphen is not the h suffix")
    }
}

[test]
def test_tokenizer_split_rules(t : T?) {
    t |> run("the infix rules with no exception table") @(t : T?) {
        // an unloaded tagger carries an empty exception table, so the prefix, suffix and infix
        // rules are all that run - which is exactly what these cells are about
        let tg = PosTagger()
        let hy <- pos_tokenize(tg, "well-known")
        let hyt <- [for (tok in hy); tok.text]
        t |> equal(join(hyt, "|"), "well|-|known", "a dash run between a letter and a letter splits")
        let comma <- pos_tokenize(tg, "Smith,Jones")
        let commat <- [for (tok in comma); tok.text]
        t |> equal(join(commat, "|"), "Smith|,|Jones", "a comma between two letters splits")
        let colon <- pos_tokenize(tg, "note:this 9:30")
        let colont <- [for (tok in colon); tok.text]
        t |> equal(join(colont, "|"), "note|:|this|9:30", "a colon splits before a letter, not before a digit")
        let dot <- pos_tokenize(tg, "end.Start")
        let dott <- [for (tok in dot); tok.text]
        t |> equal(join(dott, "|"), "end|.|Start", "a period between lower and upper case splits")
        let plain <- pos_tokenize(tg, "a-1 3-4")
        let plaint <- [for (tok in plain); tok.text]
        t |> equal(join(plaint, "|"), "a-1|3-4", "a dash before a digit is not an infix")
    }
    t |> run("a missing pack panics by name") @(t : T?) {
        var refused = false
        try {
            var tg <- load_pos_tagger(path_join(models_dir(), "no-such-tts_postag.bin"))
            delete tg
        } recover {
            refused = true
        }
        t |> success(refused, "loading a missing tagger pack panics rather than tagging with zeros")
    }
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
        t |> success(rate >= 0.96, "tag agreement {rate} over {n} tokens")
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
