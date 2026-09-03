options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_g2p
require dasllama/dasllama_postag
require strings
require daslib/strings_boost
require daslib/fio
require _tts_corpus
require _model_tier   // models_dir(): tts_g2p.bin and tts_postag.bin live in the model store

// TTS grapheme-to-phoneme against the 200-sentence corpus: phoneme-identical with the reference
// front end (arm E of the fidelity experiment, fed the same normalized text), the heteronym
// gate (both annotated readings present), the fallback chain, stress helpers, and the
// load-time budget. Model-gated on tts_g2p.bin + tts_postag.bin (harness/build_g2p_data.py,
// harness/train_postag.py).

def private g2p_path() : string => path_join(models_dir(), "tts_g2p.bin")
def private tagger_path() : string => path_join(models_dir(), "tts_postag.bin")

def private data_present(t : T?) : bool {
    for (p in [g2p_path(), tagger_path()]) {
        let f = fopen(p, "rb")
        if (f == null) {
            t |> skip("{base_name(p)} absent in {models_dir()}")
            return false
        }
        fclose(f)
    }
    return true
}

//! the heteronym scorer's fold: stress and length marks off, the reduced vowels and the flap
//! folded together
def private bare(ps : string) : string {
    var s = replace(replace(replace(ps, "ˈ", ""), "ˌ", ""), "ː", "")   // nolint:STYLE039
    s = replace(s, "ˑ", "")   // nolint:STYLE039
    s = replace(s, " ", "")
    s = replace(replace(replace(replace(replace(s, "ᵻ", "ə"), "ɨ", "ə"), "ɐ", "ə"), "ɪ", "ə"), "ᵊ", "ə")   // nolint:STYLE039
    return replace(replace(s, "ɾ", "t"), "T", "t")   // nolint:STYLE039
}

[test]
def test_corpus_phonemes(t : T?) {
    t |> run("phoneme-identical with the reference front end on 200 normalized sentences") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        let rows <- load_tts_corpus()
        var mismatches = 0
        var beyond = 0
        for (r in rows) {
            let got = strip(g2p_phonemize(g, tg, r.norm))
            if (got != r.ps_misaki) {
                if (key_exists(BEYOND_REFERENCE, r.id)) {
                    beyond++
                    to_log(LOG_INFO, "tts g2p beyond the reference {r.id}: got  {got}\n       ref  {r.ps_misaki}\n")
                } else {
                    mismatches++
                    to_log(LOG_INFO, "tts g2p differs {r.id}: got  {got}\n       want {r.ps_misaki}\n")
                }
            }
        }
        to_log(LOG_INFO, "tts g2p: {200 - mismatches - beyond}/200 sentences phoneme-identical with the reference, {beyond} read a heteronym or a name the reference misses\n")
        // every remaining difference is a tag the reference tagger read differently on a word
        // with a tag-keyed reading (the tagger agrees with it on 97 percent of tokens); the
        // tokenizer and the lexicon rules themselves are identical on the whole corpus
        t |> success(mismatches <= 3, "{mismatches} sentences differ (budget 3 - tag calls)")
        t |> equal(beyond, length(BEYOND_REFERENCE), "the sentences the rules and the lexicon additions read beyond the reference")
    }
    t |> run("heteronyms: both readings present") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        let rows <- load_tts_corpus()
        var n = 0
        var both = 0
        for (r in rows) {
            continue if (empty(r.heteronym.word))
            n++
            let ps = bare(g2p_phonemize(g, tg, r.norm))
            let f1 = find(ps, bare(r.heteronym.form1)) >= 0
            let f2 = find(ps, bare(r.heteronym.form2)) >= 0
            if (f1 && f2) {
                both++
            } else {
                to_log(LOG_INFO, "tts g2p heteronym miss {r.id} '{r.heteronym.word}': {f1 ? "" : "form1 "}{f2 ? "" : "form2 "}absent in {ps}\n")
            }
        }
        to_log(LOG_INFO, "tts g2p: heteronyms both readings {both}/{n}\n")
        t |> equal(n, 38)
        // the reference front end scores 24/38 in this inventory (27/38 in the espeak one), the
        // bare tagger-less pipeline 2/38, espeak 11/38; the infinitive, determiner and
        // collocation rules and the two keyed entries read past it, the six left are four
        // tagger calls (dove, invalid, moped-class) and two annotation spellings
        t |> success(both >= 32, "heteronyms {both}/{n} (reference front end 24 in this inventory)")
    }
}

// the sentences the front end reads past the reference: the heteronyms its rules settle ("lead
// pipe" and "get the lead out", "bass player", "minute detail", "the does", "my resume") and the
// names the lexicon additions of harness/g2p_local_additions.json carry
let private BEYOND_REFERENCE <- { "ht001", "ht003", "ht010", "ht017", "ht024", "ht038",
    "lj024", "lj027", "lj037", "ov000", "ov001", "ov003", "ov006", "ov009", "ov011", "ov012", "ov015", "ov016",
    "ov017", "ov018", "ov019", "ov020", "ov021", "ov022", "ov025", "ov027", "ov028" }

[test]
def test_pieces(t : T?) {
    t |> run("stress helpers") @(t : T?) {
        t |> equal(apply_stress("hɛlˈO", -2.0), "hɛlO")   // nolint:STYLE039
        t |> equal(apply_stress("hɛlˈO", -1.0), "hɛlˌO")   // nolint:STYLE039
        t |> equal(apply_stress("hɛlO", 2.0), "hˈɛlO")   // nolint:STYLE039
        t |> equal(apply_stress("hɛlO", 0.5), "hˌɛlO")   // nolint:STYLE039
        t |> equal(apply_stress("hɛlˌO", 1.0), "hɛlˈO")   // nolint:STYLE039
        t |> equal(apply_stress("sts", 2.0), "sts")
    }
    t |> run("ARPAbet into the inventory") @(t : T?) {
        t |> equal(arpabet_to_misaki(["K", "AH0", "M", "P", "Y", "UW1", "T", "ER0"]), "kəmpjˈutəɹ")   // nolint:STYLE039
        t |> equal(arpabet_to_misaki(["R", "IY2", "K", "AO1", "R", "D"]), "ɹˌikˈɔɹd")   // nolint:STYLE039
        t |> equal(arpabet_to_misaki(["K", "AE2", "T"]), "kæt")   // nolint:STYLE039
    }
    t |> run("the fallback chain never drops a word") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        // in the gold lexicon, in CMUdict only, and a made-up spelling for the GRU
        let ps = g2p_phonemize(g, tg, "The zymurgy of Kierkegaard was flombastic.")
        t |> success(!empty(ps), ps)
        var inscope words <- split(strip(ps), " ")
        t |> equal(length(words), 6, ps)
        for (w in words) {
            t |> success(!empty(w), "an empty word in {ps}")
        }
        let spelled <- g2p_predict(g, "flombastic")
        t |> success(length(spelled) >= 6 && length(spelled) <= 12, "GRU spelled {length(spelled)} phones")
    }
    t |> run("loads inside the budget") @(t : T?) {
        return if (!data_present(t))
        let t0 = ref_time_ticks()
        let g <- load_g2p(g2p_path())
        let usec = get_time_usec(t0)
        to_log(LOG_INFO, "tts g2p: loaded {g.gold.n} gold, {g.silver.n} silver, {g.cmu.n} cmudict entries in {usec} us\n")
        t |> success(usec < 50000, "load took {usec} us")
        t |> success(g.gold.n > 90000 && g.silver.n > 90000 && g.cmu.n > 120000, "table sizes {g.gold.n} {g.silver.n} {g.cmu.n}")
    }
}
