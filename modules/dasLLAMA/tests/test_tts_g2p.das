options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_g2p
require dasllama/dasllama_postag
require strings
require daslib/strings_boost
require daslib/fio
require dasllama/dasllama_unicode   // utf8_to_cpts: the inventory sweep reads codepoints
require _tts_corpus
require _tts_corpus_gb
require _model_tier   // models_dir(): tts_g2p.bin and tts_postag.bin live in the model store
require daslib/rtti   // this_context().last_exception: the American-only pack's British refusal is read as text

// TTS grapheme-to-phoneme against the 200-sentence corpus: phoneme-identical with the reference
// front end (arm E of the fidelity experiment, fed the same normalized text), the heteronym
// gate (both annotated readings present), the function-word and glued-piece arms of the
// lexicon, the number reader, the fallback chain, stress helpers, and the load-time budget;
// then the British half - the same corpus through the British lexicon tier against the
// reference's own British front end, the inventory sweep, the inflection rules and the
// rewrite the American fallback owes a British voice.
// Model-gated on tts_g2p.bin + tts_postag.bin (harness/build_g2p_data.py,
// harness/train_postag.py, harness/mint_tts_g2p_gb_fixture.py); the missing-file panic, the
// pack-version refusal and the rewrite table are model-free.

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
        // tokenizer and the lexicon rules themselves are identical on the whole corpus. Pinned
        // exactly: a budget silently absorbs the next regression, and both differences here are
        // named - lj019 (a sentence-final abbreviation) and ht012 ("dove").
        t |> equal(mismatches, 2, "{mismatches} sentences differ on a tag call")
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
// names the lexicon additions of harness/g2p_local_additions.json carry - ov004 among them since
// the fallback consults the lexicon: "PyTorch" is a camelCase group no lookup resolves, so the
// addition pˈItɔɹʧ reaches the reader only there, where the reference spells out pˈɪtəɹk
let private BEYOND_REFERENCE <- { "ht001", "ht003", "ht010", "ht017", "ht024", "ht038",
    "lj024", "lj027", "lj037", "ov000", "ov001", "ov003", "ov004", "ov006", "ov009", "ov011", "ov012", "ov015", "ov016",
    "ov017", "ov018", "ov019", "ov020", "ov021", "ov022", "ov025", "ov027", "ov028" }

[test, arch(at = "../ARCHITECTURE_TTS.md#tts-heteronym-context")]
def test_heteronym_context(t : T?) {
    t |> run("a verb tag out-ranks the collocation table") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        // "lead out" is in the collocation table as the metal; tagged VB it is the verb
        t |> equal(strip(g2p_phonemize(g, tg, "The pipes lead out to the street.")), "ðə pˈIps lˈid ˈWt tə ðə stɹˈit.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "Get the lead out.")), "ɡˌɛt ðə lˈɛd ˈWt.", "the noun reading survives")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "They found lead pipes in the wall.")), "ðˌA fˈWnd lˈɛd pˈIps ɪn ðə wˈɔl.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "The lead singer sang.")), "ðə lˈid sˈɪŋəɹ sˈæŋ.")   // nolint:STYLE039
    }
}

[test]
def test_number_reader(t : T?) {
    t |> run("a number reads all its digits, past 2^31 too") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        // to_int answers 0 past 2^31, so the reader that used it said "zero" out loud
        t |> equal(strip(g2p_phonemize(g, tg, "3000000000")), strip(g2p_phonemize(g, tg, "three billion")))
        t |> equal(strip(g2p_phonemize(g, tg, "3000000000th")), "θɹˈi bˈɪljənθ", "the ordinal suffix too")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "21st")), "twˈɛnti fˈɜɹst")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "1990s")), "wˈʌn θˈWzᵊnd nˈIn hˈʌndɹəd nˈIndiz", "the -s suffix rides the joined reading")   // nolint:STYLE039
        // past the int64 accumulator's width the run reads digit by digit
        let wide = strip(g2p_phonemize(g, tg, "12345678901234567890"))
        t |> equal(wide, "wˈʌn tˈu θɹˈi fˈɔɹ fˈIv sˈɪks sˈɛvən ˈAt nˈIn zˈɪɹO wˈʌn tˈu θɹˈi fˈɔɹ fˈIv sˈɪks sˈɛvən ˈAt nˈIn zˈɪɹO")   // nolint:STYLE039
    }
}

[test]
def test_function_words(t : T?) {
    t |> run("the arms that read what follows") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        t |> equal(strip(g2p_phonemize(g, tg, "A dog barked.")), "ɐ dˈɔɡ bˈɑɹkt.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "An apple a day.")), "ɐn ˈæpᵊl ɐ dˈA.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "I am here.")), "ˌI ɐm hˈɪɹ.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "in the box")), "ɪn ðə bˈɑks")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "the end")), "ði ˈɛnd", "a vowel follows: the long form")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "to run")), "tə ɹˈʌn")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "to open")), "tʊ ˈOpᵊn", "a vowel follows: the long form")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "He used to run.")), "hˌi jˈust tə ɹˈʌn.", "\"used to\" is the VBD reading")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "He used a hammer.")), "hˌi jˈuzd ɐ hˈæməɹ.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "Rangers vs. Kings")), "ɹˈAnʤəɹz vˈɜɹsəs kˈɪŋz")   // nolint:STYLE039
    }
    t |> run("the symbols that speak") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        t |> equal(strip(g2p_phonemize(g, tg, "50% of it")), "fˈɪfti pəɹsˈɛnt ʌv ɪt")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "R&D and A+B and x@y")), "ˈɑɹ ænd dˈi ænd ˈA plˈʌs bˈi ænd ˈɛks æt wˈI")   // nolint:STYLE039
    }
    t |> run("dotted acronyms and names spell out") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        t |> equal(strip(g2p_phonemize(g, tg, "The U.S. way.")), "ðə jˌuˈɛs wˈA.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "a.m.")), "ˌAˈɛm")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "IBM and NASA")), "ˌIbˌiˈɛm ænd nˈæsə", "an acronym letter by letter, a pronounceable one as a word")   // nolint:STYLE039
    }
}

[test]
def test_pieces_and_stems(t : T?) {
    t |> run("the splitter's alternatives") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        t |> equal(strip(g2p_phonemize(g, tg, "twenty-one")), "twˌɛntiwˈʌn", "the dash piece goes silent, the group merges")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "MacDonald and O'Brien")), "məkdˈɑnəld ænd ObɹˈIɪn", "a capital before Capital+lower, and an inner apostrophe")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "camelCase iPhone")), "kˈæmᵊlkˌAs ˈIfˌOn", "the camelCase piece")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "wifi/lan")), "wˈIfilˈæn", "the slash is junk between two known pieces")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "'cause")), "kˈʌz", "leading apostrophes open a piece")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "back2back")), "bˈæktəbˈæk", "a 2 between letters is the \"to\" alias")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "The dam — it broke.")), "ðə dˈæm — ɪt bɹˈOk.", "the em dash speaks itself")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "the dogs' bowls")), "ðə dˈɔɡz bˈOlz", "a plural possessive reads the singular possessive")   // nolint:STYLE039
    }
    t |> run("inflections a lexicon entry does not carry") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        t |> equal(strip(g2p_phonemize(g, tg, "podcast")), "pˈɑdkˌæst")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "podcasts")), "pˈɑdkˌæsts", "-s after a voiceless stop")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "podcasting")), "pˈɑdkˌæstɪŋ", "-ing on the bare stem")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "unfriends")), "ˌʌnfɹˈɛndz", "-s after a voiced stop")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "unfriended")), "ˌʌnfɹˈɛndᵻd", "-ed after d")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "vlogging")), "vlˈɑɡɪŋ", "a doubled consonant before -ing drops back to the stem")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "zymurgies")), "zˈImˌɜɹʤiz", "-ies reads the -y stem")   // nolint:STYLE039
    }
}

// per-process temp dir for the version-refusal fixture — writing it into the worktree would
// leave untracked state, and a shared path races parallel dastest runs
var private _tmp_dir : string
var private _tmp_dir_inited = false

def private tmp_path(name : string) : string {
    if (!_tmp_dir_inited) {
        let r = create_temp_directory_result("dasllama_tts_g2p")
        if (!(r is value)) {
            panic("could not create temp dir for the g2p version test: {unsafe(r.error)}")
        }
        _tmp_dir = unsafe(r.value)
        _tmp_dir_inited = true
    }
    return path_join(_tmp_dir, name)
}

[finalize]
def cleanup_temp_dir {
    if (_tmp_dir_inited && !empty(_tmp_dir)) {
        rmdir_rec(_tmp_dir)
    }
}

//! a pack header carrying the magic and `version`, and nothing else
def private write_pack_header(path : string; version : int) {
    let f = fopen(path, "wb")
    if (f == null) {
        panic("cannot write the synthetic g2p pack {path}")
    }
    var inscope bytes <- [uint8(0x54), uint8(0x47), uint8(0x32), uint8(0x50),
        uint8(version), uint8(0), uint8(0), uint8(0), uint8(0), uint8(0)]
    fwrite(f, bytes)
    fclose(f)
}

[test]
def test_us_only_pack(t : T?) {
    t |> run("the American-only pack reads the corpus as the full pack does and refuses British") @(t : T?) {
        return if (!data_present(t))
        let us_path = path_join(models_dir(), "tts_g2p_en_us.bin")
        if (!stat(us_path).is_valid) {
            t |> skip("tts_g2p_en_us.bin absent in {models_dir()}")
            return
        }
        let full <- load_g2p(g2p_path())
        let us <- load_g2p(us_path)
        let tg <- load_pos_tagger(tagger_path())
        t |> success(!full.us_only, "the full pack carries both tiers")
        t |> success(us.us_only, "the American pack says so in its source line: {us.source}")
        let rows <- load_tts_corpus()
        var differs = 0
        for (r in rows) {
            let a = g2p_phonemize(full, tg, r.norm)
            let b = g2p_phonemize(us, tg, r.norm)
            if (a != b) {
                differs++
                to_log(LOG_INFO, "tts g2p us-only differs {r.id}: full {a}\n       us   {b}\n")
            }
        }
        t |> equal(differs, 0, "American readings identical on {length(rows)} sentences")
        var refused = ""
        try {
            let gb = g2p_phonemize(us, tg, "the water is warm", true)
            to_log(LOG_INFO, "tts g2p us-only british read: {gb}\n")
        } recover {
            refused = this_context().last_exception
        }
        t |> success(find(refused, "American tier only") >= 0, "British on the American pack is refused by name: {refused}")
        let gb_full = g2p_phonemize(full, tg, "the water is warm", true)
        t |> success(!empty(gb_full), "the control: the full pack reads British")
    }
}

[test]
def test_missing_pack(t : T?) {
    t |> run("a missing pack panics by name") @(t : T?) {
        var refused = false
        try {
            var g <- load_g2p(path_join(models_dir(), "no-such-tts_g2p.bin"))
            delete g
        } recover {
            refused = true
        }
        t |> success(refused, "loading a missing g2p pack panics rather than phonemizing from zeros")
    }
    t |> run("a pack from before the British tier panics by version") @(t : T?) {
        let old = tmp_path("v1_tts_g2p.bin")
        write_pack_header(old, 1)
        var refused = false
        try {
            var g <- load_g2p(old)
            delete g
        } recover {
            refused = true
        }
        t |> success(refused, "a version 1 pack is refused rather than read as a merged one")
        // the control: the loader is not simply refusing everything - the stocked pack, whose
        // only difference from the header above is what follows a version 2, loads
        let f = fopen(g2p_path(), "rb")
        if (f == null) {
            to_log(LOG_INFO, "tts g2p: {base_name(g2p_path())} absent, the version refusal has no positive control this run\n")
            return
        }
        fclose(f)
        var loaded = false
        var g <- load_g2p(g2p_path())
        loaded = g.gold.n > 0
        delete g
        t |> success(loaded, "the stocked version 2 pack loads through the same call")
    }
}

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
    t |> run("the fallback reads the lexicon before CMUdict") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        // one unresolvable piece sends the WHOLE glued group here, so a letter run both
        // dialects' lexicons carry does reach the fallback - and CMUdict, pruned of exactly
        // those words, has only its own coarser reading to offer (wˈɔtəɹ, hˈɛlO, hˈɪɹd)
        t |> equal(strip(g2p_phonemize(g, tg, "the water-tzarina")), "ðə wˈɔTəɹtɑɹˈinə", "the lexicon's flapped water")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "hello-flurbnik")), "həlˈOflˈɜɹnbɪk")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "heard-flurbnik")), "hˈɜɹdflˈɜɹnbɪk")   // nolint:STYLE039
        // the British rail reads the British part itself, not the American one through the
        // rewrite - the bath-trap split lives in the lexicon and no rewrite can invent it
        // (misaki_us_to_gb of the American bˈæθ is bˈaθ)
        t |> equal(strip(g2p_phonemize(g, tg, "the water-tzarina", true)), "ðə wˈɔːtətɑːɹˈiːnə")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "hello-flurbnik", true)), "həlˈQflˈɜːnbɪk")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "heard-flurbnik", true)), "hˈɜːdflˈɜːnbɪk")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "bath-flurbnik", true)), "bˈɑːθflˈɜːnbɪk", "the British bath vowel")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "laughter-flurbnik", true)), "lˈɑːftəflˈɜːnbɪk")   // nolint:STYLE039
    }
    t |> run("loads inside the budget") @(t : T?) {
        return if (!data_present(t))
        let t0 = ref_time_ticks()
        let g <- load_g2p(g2p_path())
        let usec = get_time_usec(t0)
        to_log(LOG_INFO, "tts g2p: loaded {g.gold.n} gold, {g.silver.n} silver, {g.cmu.n} cmudict entries in {usec} us\n")
        t |> success(usec < 50000, "load took {usec} us")
        // the merged tables key on the union of both dialects; CMUdict drops every word BOTH
        // dialects' lexicons already carry, since the lexicon answers first for either of them
        t |> success(g.gold.n > 95000 && g.silver.n > 115000 && g.cmu.n > 75000, "table sizes {g.gold.n} {g.silver.n} {g.cmu.n}")
    }
    t |> run("the pack stays inside its size budget") @(t : T?) {
        return if (!data_present(t))
        let bytes = stat(g2p_path()).size
        to_log(LOG_INFO, "tts g2p: {g2p_path()} is {bytes} bytes\n")
        t |> success(bytes > 10000000ul && bytes < 16000000ul, "{bytes} bytes (both dialects' lexicons, CMUdict and the f16 GRU)")
    }
}

// ===== British English =====

//! the American-only symbols: what a British phoneme string must never carry (`T` is the
//! flap after the pipeline's own rewrite, so it is on the list too)
let private AMERICAN_ONLY <- { 230, 'O', 638, 7547, 660, 'T' }   // æ O ɾ ᵻ ʔ T

//! the symbols outside a lexicon's inventory that either dialect still emits: the weak `a`
//! vowel misaki spells for "a"/"an", the space, and the punctuation a token speaks as itself
let private INVENTORY_EXEMPT <- { 592, ' ', 8212, 8230, 8220, 8221 }   // ɐ, space, — … “ ”

def private off_inventory(ps : string; vocab : array<int>) : int {
    var n = 0
    var inscope cps <- utf8_to_cpts(ps)
    for (cp in cps) {
        continue if (key_exists(INVENTORY_EXEMPT, cp) || (cp < 128 && !is_alpha(cp)))
        n += find_index(vocab, cp) < 0 ? 1 : 0
    }
    return n
}

def private american_only_count(ps : string) : int {
    var n = 0
    var inscope cps <- utf8_to_cpts(ps)
    for (cp in cps) {
        n += key_exists(AMERICAN_ONLY, cp) ? 1 : 0
    }
    return n
}

// the sentences the British front end reads past the reference: the heteronyms our own rules
// settle. The lexicon additions of harness/g2p_local_additions.json are American, so the rows
// they carry in the American rail read WITH the reference here.
let private GB_BEYOND_REFERENCE <- { "ht001", "ht003", "ht010", "ht017", "ht024", "ht038" }

[test]
def test_british_corpus(t : T?) {
    t |> run("British: phoneme-identical with the reference front end where no word is out of vocabulary") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        var inscope corpus <- load_tts_corpus_gb()
        var clean = 0
        var mismatches = 0
        var beyond = 0
        for (r in corpus.rows) {
            continue if (r.oov)
            clean++
            let got = strip(g2p_phonemize(g, tg, r.norm, true))
            continue if (got == r.ps_gb)
            if (key_exists(GB_BEYOND_REFERENCE, r.id)) {
                beyond++
                to_log(LOG_INFO, "tts g2p gb beyond the reference {r.id}: got  {got}\n       ref  {r.ps_gb}\n")
            } else {
                mismatches++
                to_log(LOG_INFO, "tts g2p gb differs {r.id}: got  {got}\n       want {r.ps_gb}\n")
            }
        }
        to_log(LOG_INFO, "tts g2p gb: {clean - mismatches - beyond}/{clean} lexicon-only sentences phoneme-identical with the reference British front end, {beyond} read a heteronym past it\n")
        t |> equal(clean, 157, "the sentences the reference answered from its lexicon alone")
        // the same class of remainder the American rail carries: a tag the reference tagger
        // read differently on a word with a tag-keyed reading, pinned exactly for the same
        // reason - the two are lj019 and ht012, the American rail's own pair
        t |> equal(mismatches, 2, "{mismatches} sentences differ on a tag call")
        t |> equal(beyond, length(GB_BEYOND_REFERENCE), "the sentences our heteronym rules read beyond the reference")
    }
    t |> run("British: nothing American survives, and the American rail proves the sweep sees it") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        var inscope corpus <- load_tts_corpus_gb()
        var inscope vocab <- utf8_to_cpts(corpus.gb_vocab)
        var gb_american = 0
        var us_american = 0
        var gb_off = 0
        for (r in corpus.rows) {
            let gb = g2p_phonemize(g, tg, r.norm, true)
            let us = g2p_phonemize(g, tg, r.norm)
            gb_american += american_only_count(gb)
            us_american += american_only_count(us)
            let off = off_inventory(gb, vocab)
            if (off > 0) {
                gb_off += off
                to_log(LOG_INFO, "tts g2p gb off-inventory {r.id}: {gb}\n") if (gb_off <= 10)
            }
        }
        t |> equal(gb_american, 0, "American-only symbols in the British output of 200 sentences")
        t |> equal(gb_off, 0, "symbols outside gb_gold's inventory in the British output")
        // the control: the same sweep over the American output of the same sentences must find
        // plenty, or it proves nothing about the British one
        t |> success(us_american > 100, "the American output carries {us_american} of them")
    }
}

[test]
def test_british_rules(t : T?) {
    t |> run("British: the inflection rules the reference switches on the flag") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        // -ed after d is ɪd, not ᵻd; -ed after t is ɪd with no flap; -s after a sibilant is ɪz
        t |> equal(strip(g2p_phonemize(g, tg, "unfriended", true)), "ʌnfɹˈɛndɪd")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "unfriended")), "ˌʌnfɹˈɛndᵻd", "the American reading keeps ᵻ")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "podcasting", true)), "pˈɒdkɑːstɪŋ")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "podcasts", true)), "pˈɒdkɑːsts")   // nolint:STYLE039
    }
    t |> run("British: -ing declines a stem ending in a schwa or a length mark") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        // the British lexicon carries "mentor" mˈɛntɔː and "roster" ɹˈɒstə but neither -ing
        // form, so the stemmer runs and declines: the reference has no rule for the linking r
        // such a stem wants, and the word goes to the fallback (the American reading rewritten)
        // rather than reading mˈɛntɔːɪŋ / ɹˈɒstəɪŋ out loud
        t |> equal(strip(g2p_phonemize(g, tg, "mentor", true)), "mˈɛntɔː", "the length-mark stem")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "mentoring", true)), "mˈɛntəɹɪŋ")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "roster", true)), "ɹˈɒstə", "the schwa stem")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "rostering", true)), "ɹˈɒstəɹɪŋ")   // nolint:STYLE039
        // -ed keeps the same stems, so the decline is the -ing rule's alone
        t |> equal(strip(g2p_phonemize(g, tg, "mentored", true)), "mˈɛntɔːd")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "rostered", true)), "ɹˈɒstəd")   // nolint:STYLE039
        // and the American rail, which has no such rule, stems both
        t |> equal(strip(g2p_phonemize(g, tg, "mentoring")), "mˈɛntˌɔɹɪŋ")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "rostering")), "ɹˈɑstəɹɪŋ")   // nolint:STYLE039
    }
    t |> run("British: the lexicon tier answers, and a word only the American tier carries is unknown") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        // the bath-trap split, the LOT vowel and the non-rhotic vowels come from the British
        // lexicon itself, not from the rewrite
        t |> equal(strip(g2p_phonemize(g, tg, "Bath, dance, laughter and the last bus home.", true)),
            "bˈɑːθ, dˈɑːns, lˈɑːftə and ðə lˈɑːst bˈʌs hˈQm.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "The dogs barked at the water tower.", true)),
            "ðə dˈɒɡz bˈɑːkt at ðə wˈɔːtə tˈWə.")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "The dogs barked at the water tower.")),
            "ðə dˈɔɡz bˈɑɹkt æt ðə wˈɔTəɹ tˈWəɹ.", "the American reading is rhotic and flapped")   // nolint:STYLE039
    }
}

// what the American fallback chain owes a British voice: the rewrite table itself, then the
// chain end to end on words no lexicon carries
[test]
def test_british_rewrite(t : T?) {
    t |> run("the American inventory rewritten into the British one") @(t : T?) {
        // a post-vocalic ɹ drops into the vowel it leaves, and links before a vowel
        t |> equal(misaki_us_to_gb("hˈɜɹd"), "hˈɜːd")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("kˈɑɹd"), "kˈɑːd")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("kˈɔɹd"), "kˈɔːd")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("bˈɪɹd"), "bˈɪəd")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("bˈɛɹd"), "bˈɛːd")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("kjˈʊɹd"), "kjˈʊəd")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("hˈɜɹdəɹ"), "hˈɜːdə")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("wˈɔɹəɹ"), "wˈɔːɹə", "the ɹ before a vowel links")   // nolint:STYLE039
        // the rows derived from the pack's own merged entries at a non-linking position:
        // queerly kwˈiɹli -> kwˈɪəli, procurement pɹəkjˈuɹmᵊnt -> pɹəkjˈʊəmᵊnt,
        // ireful ˈIɹfᵊl -> ˈIəfʊl, ourself Wɹsˈɛlf -> Wəsˈɛlf
        t |> equal(misaki_us_to_gb("hˈiɹd"), "hˈɪəd")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("pˈuɹ"), "pˈʊə")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("ˈIɹfᵊl"), "ˈIəfᵊl")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("Wɹsˈɛlf"), "Wəsˈɛlf")   // nolint:STYLE039
        // a vowel the pack gives no row keeps its ɹ - the pack's own Sverige svˈæɹjə -> svˈaɹjə
        // is the evidence, and dropping a rhotic the British inventory does spell loses a sound
        t |> equal(misaki_us_to_gb("svˈæɹjə"), "svˈaɹjə")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("kˈæɹt"), "kˈaɹt")   // nolint:STYLE039
        // ᵻ is American-only, so this reading never re-enters the rewrite; it pins the arm
        t |> equal(misaki_us_to_gb("bˈᵻɹd"), "bˈɪɹd")   // nolint:STYLE039
        // the merry/Mary limit: the American inventory spells both vowels ɛ, so ɛɹ before a
        // vowel is SQUARE whatever the word was
        t |> equal(misaki_us_to_gb("mˈɛɹi"), "mˈɛːɹi")   // nolint:STYLE039
        // then the symbols the two inventories spell differently
        t |> equal(misaki_us_to_gb("kˈæt"), "kˈat")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("bˈOt"), "bˈQt")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("bˈɑks"), "bˈɒks")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("bˈʌsᵻz"), "bˈʌsɪz")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("wˈɔɾəɹ"), "wˈɔːtə", "the flap becomes a plain t")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("wˈɔTəɹ"), "wˈɔːtə", "and so does the T the pipeline folds it to")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("kəmpjˈutəɹ"), "kəmpjˈuːtə", "u lengthens; unstressed ə stays")   // nolint:STYLE039
        t |> equal(misaki_us_to_gb("sˈiti"), "sˈiːti", "i lengthens under stress only")   // nolint:STYLE039
        // the rewrite is idempotent on what it emits, so a caller may hand it a string it
        // already answered (ᵻ excepted, and unreachable: no British reading carries one back in)
        for (ps in ["hˈɜɹd", "kˈɑɹd", "bˈɪɹd", "wˈɔɹəɹ", "kˈæt", "bˈOt", "bˈɑks", "wˈɔTəɹ", "kəmpjˈutəɹ", "sˈiti",   // nolint:STYLE039
                "hˈiɹd", "pˈuɹ", "ˈIɹfᵊl", "Wɹsˈɛlf", "mˈɛɹi", "svˈæɹjə", "kˈæɹt"]) {   // nolint:STYLE039
            let once = misaki_us_to_gb(ps)
            t |> equal(misaki_us_to_gb(once), once, "'{ps}' rewritten twice")
        }
    }
    t |> run("British: the fallback's rhotic vowels reach the whole word") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        // neither word is in either lexicon, so both read through CMUdict and the rewrite - and
        // the iɹ and uɹ rows are what keeps their vowels from collapsing to iː and uː
        t |> equal(strip(g2p_phonemize(g, tg, "comandeered", true)), "kɒməndˈɪəd")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "boorstin", true)), "bˈʊəstɪn")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "comandeered")), "kɑməndˈiɹd", "the American reading is rhotic")   // nolint:STYLE039
        t |> equal(strip(g2p_phonemize(g, tg, "boorstin")), "bˈuɹstɪn")   // nolint:STYLE039
    }
    t |> run("British: the fallback chain is the American one rewritten") @(t : T?) {
        return if (!data_present(t))
        let g <- load_g2p(g2p_path())
        let tg <- load_pos_tagger(tagger_path())
        var inscope corpus <- load_tts_corpus_gb()
        var inscope vocab <- utf8_to_cpts(corpus.gb_vocab)
        for (o in corpus.oov_words) {
            let us = strip(g2p_phonemize(g, tg, o.word))
            let gb = strip(g2p_phonemize(g, tg, o.word, true))
            to_log(LOG_INFO, "tts g2p gb oov '{o.word}': ours {us} -> {gb}; reference espeak {o.ps_us_ref} -> {o.ps_gb_ref}\n")
            t |> equal(gb, misaki_us_to_gb(us), "'{o.word}': the British reading is the American one through the rewrite")
            t |> equal(off_inventory(gb, vocab), 0, "'{o.word}': {gb} stays inside the British inventory")
            t |> equal(off_inventory(o.ps_gb_ref, vocab), 0, "the reference's own '{o.ps_gb_ref}' does too")
        }
    }
}
