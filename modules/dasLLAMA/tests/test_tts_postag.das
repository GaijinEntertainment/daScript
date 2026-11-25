options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_postag
require strings
require daslib/strings_boost
require daslib/fio
require _tts_corpus
require _model_tier   // models_dir(): tts_postag.bin lives in the model store
require _model_form   // own_load: the load budget's own load

// TTS tokenizer + tagger. Model-free: the tagger's word-class and shape features against the
// trainer's own rules, the tokenizer's split rules with an empty exception table, and the
// missing-file panic. Model-gated on tts_postag.bin (harness/train_postag.py): the load-time
// budget, a load of its own. The cells on a loaded tagger - the 200-sentence corpus against the
// reference tokenizer and tagger, the packed file's header floors - are `_tts_postag_cells.das`'s,
// run on the pack's block in test_tts_g2p.das.

def private tagger_path() : string => path_join(models_dir(), "tts_postag.bin")

def private tagger_present(t : T?) : bool {
    return false if (!family_on(t, "g2p"))
    let f = fopen(tagger_path(), "rb")
    if (f == null) {
        t |> skip("tts_postag.bin absent in {models_dir()}")
        return false
    }
    fclose(f)
    return true
}

[test(description = "`pos_norm_word` folds tokens to the trainer's classes (hyphen not leading, year, digits, lower case) and `pos_word_shape` emits case shapes with the d, h, q suffixes in order, a leading hyphen not counting")]
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

[test(description = "With an empty exception table `pos_tokenize` splits dash, comma, colon and case-changing period infixes between letters but not before digits, and loading a missing tagger pack panics")]
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

[test(description = "The packed POS tagger loads under 150 ms")]
def test_tagger_load_budget(t : T?) {
    own_load("the cell asserts on the load itself: the load's own time")
    t |> run("loads inside the budget") @(t : T?) {
        return if (!tagger_present(t))
        let t0 = ref_time_ticks()
        let tg <- load_pos_tagger(path_join(models_dir(), "tts_postag.bin"))
        let usec = get_time_usec(t0)
        to_log(LOG_INFO, "tts tagger: loaded {length(tg.feat_hash)} features in {usec} us\n")
        t |> success(usec < 150000, "load took {usec} us")
    }
}
