options gen2
options _dasllama_internal = true
options _ascii_strings = false   // the Russian words and their IPA are the claims
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_g2p_ru
require strings
require daslib/strings_boost
require daslib/fio
require _tts_corpus_ru
require _scratch_dir
require _model_tier

// The Russian front end. Model-free: letter-to-sound on pinned marked words (palatalization,
// iotation, the sign cases, voicing and final devoicing, the vowel reduction, the respellings,
// the clusters), the sandhi across words, hyphenated words, punctuation and the pass-through of
// another script's run, both mark spellings; then the 200-sentence fixture against kokoro-ru's
// own front end fed the same marked text, the word mismatches pinned; and the missing-pack
// panic and the version refusal. Model-gated (`tts_g2p_ru.bin` under models_dir()): the stress
// pack - a form's variants and its default, ё restoration and the ambiguous ё left alone, the
// clitics, a reflexive read through its base form, the shape rule on a word the pack lacks, a
// mark in the text winning, and the whole text-to-phonemes path.

def private pack_path() : string => path_join(models_dir(), "tts_g2p_ru.bin")

[test]
def test_letter_to_sound(t : T?) {
    t |> run("pinned words: the label shape kokoro-ru was trained on") @(t : T?) {
        let cases <- {
            "здр+авствуйте" => "zdrˈɑstvujtʲi", "двен+адцать" => "dvʲinˈɑtʦətʲ", "мешк+ов" => "mʲiʃkˈof", "мук+и" => "mukʲˈi",
            "откр+ыть" => "ɐtkrˈytʲ", "зам+ок" => "zɐmˈok", "з+амок" => "zˈɑmək", "исп+ытывать" => "ɪspˈytyvətʲ",
            "сег+одня" => "sʲivˈodnʲɪ", "хор+ошая" => "xɐrˈoʃəjɪ", "пог+ода" => "pɐɡˈodə", "пойдём" => "pɐjdʲˈom",
            "гул+ять" => "ɡulʲˈɑtʲ", "сказ+ала" => "skɐzˈɑlə", "вернётся" => "vʲirnʲˈoʦə", "час+ов" => "ʧɐsˈof", "в+ечера" => "vʲˈeʧirə",
            "яз+ык" => "jɪzˈyk", "программ+ирования" => "prəɡrɐmmʲˈirəvənʲijɪ", "компил+ируется" => "kəmʲpʲilʲˈirujiʦə",
            "маш+инный" => "mɐʃˈynnyj", "к+од" => "kˈot", "т+олько" => "tˈolkə", "+очень" => "ˈoʧin", "теп+ерь" => "tʲipʲˈerɪ",
            "сд+елать" => "ʒdʲˈelətʲ", "сч+ёт" => "ɕːˈot", "ещ+ё" => "jɪɕːˈo", "объ+ект" => "ɐbjˈekt", "стать+я" => "stɐtʲjˈa",
            "л+естница" => "lʲˈesnʲiʦə", "кон+ечно" => "kɐnʲˈeʃnə", "ег+о" => "jɪvˈo", "чт+обы" => "ʃtˈoby", "л+учше" => "lˈuʧʃy",
            "д+еньги" => "dʲˈenɡʲɪ", "компь+ютер" => "kɐmʲpjˈjutʲir", "с+олнце" => "sˈonʦy", "ч+увство" => "ʧˈustvə",
            "мн+ого" => "mnˈoɡə", "дв+адцать" => "dvˈɑtʦətʲ", "двадцат+и" => "dvəʦɐtʲˈi", "+игры" => "ˈiɡry", "игр+ы" => "ɪɡrˈy",
            "мо+я" => "mɐˈja", "сво+ю" => "svɐˈju", "н+очью" => "nˈoʧjju", "друзь+я" => "druʒjˈa", "вс+ё" => "fsʲˈo", "лёгкий" => "lʲˈoxkʲij"
        }
        for (word, want in keys(cases), values(cases)) {
            t |> equal(ru_word_ipa(word), want, word)
        }
        t |> equal(ru_word_ipa("замо́к"), "zɐmˈok", "the combining acute marks the vowel before it")
        t |> equal(ru_word_ipa("З+амок"), ru_word_ipa("з+амок"), "case does not matter")
    }
    t |> run("sandhi: what opens the next word moves this word's last consonant") @(t : T?) {
        t |> equal(ru_phonemize_marked("в п+арк"), "f pˈɑrk")
        t |> equal(ru_phonemize_marked("в д+ом"), "v dˈom")
        t |> equal(ru_phonemize_marked("в +окна"), "v ˈoknə", "the preposition keeps its voice before a vowel")
        t |> equal(ru_phonemize_marked("игрок+ов +ищут"), "ɪɡrɐkˈof ˈiɕːut", "a word's final в devoices there")
        t |> equal(ru_phonemize_marked("т+ысяч д+олларов"), "tˈysʲɪʤʲ dˈollərəf")
        t |> equal(ru_phonemize_marked("м+ожешь сд+елать"), "mˈoʒyʒ ʒdʲˈelətʲ", "сделать opens voiced")
        t |> equal(ru_phonemize_marked("+очень интер+есно"), "ˈoʧinʲ ɪntʲirʲˈesnə", "a final soft consonant stays soft before a vowel")
        t |> equal(ru_phonemize_marked("+очень т+ак"), "ˈoʧin tˈɑk")
        t |> equal(ru_phonemize_marked("вн+овь заст+али"), "vnˈof zɐstˈɑlʲɪ", "a final вь devoices whatever follows")
    }
    t |> run("hyphenated words, punctuation, another script passing through") @(t : T?) {
        t |> equal(ru_phonemize_marked("чт+о-то"), "ʃtˈotˈo")
        t |> equal(ru_phonemize_marked("когд+а-ниб+удь"), "kɐɡdˈɑnʲibˈutʲ")
        t |> equal(ru_phonemize_marked("как+ой-либо"), "kɐkˈojlʲˈibə")
        t |> equal(ru_phonemize_marked("Как+ой-Либо"), ru_phonemize_marked("как+ой-либо"), "a capital side is the same particle")
        t |> equal(ru_phonemize_marked("из-з+а"), "ɪszˈɑ", "a clitic side takes no stress")
        t |> equal(ru_phonemize_marked("Прив+ет, м+ир! К+ак дел+а?"), "prʲivʲˈet, mʲˈir! kˈɑk dʲilˈɑ?")
        t |> equal(ru_phonemize_marked("«Д+а» — сказ+ал (+он)."), "dˈɑ — skɐzˈɑl ˈon.", "quotes and brackets go, the dash stays")
        t |> equal(ru_phonemize_marked("сл+ово dˈæslæŋ сл+ово"), "slˈovə dˈæslæŋ slˈovə", "phonemes spliced in by the caller pass through")
    }
}

//! a corpus word that carries its stress: a `+` or a ё - the words letter-to-sound alone decides
def private is_marked(word : string) : bool => find(word, "+") >= 0 || find(word, "ё") >= 0 || find(word, "Ё") >= 0

[test]
def test_corpus_ru(t : T?) {
    t |> run("phoneme-identical with kokoro-ru's front end on the marked words of 200 sentences") @(t : T?) {
        let rows <- load_tts_corpus_ru()
        var marked_compared = 0
        var misses = 0
        var bare = 0
        var bare_same = 0
        for (r in rows) {
            let got = ru_phonemize_marked(r.marked)
            var inscope marked_words <- split(r.marked, " ")
            var inscope want_words <- split(r.ps, " ")
            var inscope got_words <- split(got, " ")
            if (length(want_words) != length(got_words) || length(want_words) != length(marked_words)) {
                misses += length(want_words)
                to_log(LOG_INFO, "tts g2p ru {r.id}: word count differs\n  got  {got}\n  want {r.ps}\n")
                continue
            }
            for (got_word, want_word, marked_word in got_words, want_words, marked_words) {
                if (!is_marked(marked_word)) {
                    bare++
                    bare_same += got_word == want_word ? 1 : 0
                    continue
                }
                marked_compared++
                if (got_word != want_word) {
                    misses++
                    to_log(LOG_INFO, "tts g2p ru {r.id}: got {got_word} want {want_word}\n")
                }
            }
        }
        to_log(LOG_INFO, "tts g2p ru: {marked_compared - misses}/{marked_compared} marked words identical; {bare_same}/{bare} bare words agree with the reference's own stress\n")
        // pinned so a new miss reds: the four left are the reference's readings of three loanwords and one voiced final зь
        t |> equal(misses, 4, "{misses} marked words differ from the reference")
        t |> success(marked_compared >= 1500, "{marked_compared} marked words compared")
    }
}

def private load_and_drop(path : string) {
    var inscope p <- load_ru_stress_pack(path)
}

[test]
def test_pack_refusals(t : T?) {
    t |> run("a missing pack panics naming it") @(t : T?) {
        let missing = scratch_path("no-such-tts_g2p_ru.bin")
        let why = panic_text() {
            load_and_drop(missing)
        }
        t |> success(find(why, "cannot open the Russian stress pack") >= 0, why)
    }
    t |> run("a pack of another version is refused by name") @(t : T?) {
        let path = scratch_path("tts_g2p_ru_v99.bin")
        var inscope bytes <- [for (b in [0x54u, 0x47u, 0x52u, 0x55u, 99u, 0u, 0u, 0u, 0u, 0u]); uint8(b)]
        fopen(path, "wb") $(f) {
            fwrite(f, bytes)
        }
        let why = panic_text() {
            load_and_drop(path)
        }
        t |> success(find(why, "version 99") >= 0 && find(why, "build_g2p_ru_data.py") >= 0, why)
    }
    t |> run("another pack's magic is refused by name") @(t : T?) {
        let path = scratch_path("tts_g2p_ru_wrong_magic.bin")
        var inscope bytes <- [for (b in [0x54u, 0x47u, 0x32u, 0x50u, 1u, 0u, 0u, 0u, 0u, 0u]); uint8(b)]
        fopen(path, "wb") $(f) {
            fwrite(f, bytes)
        }
        let why = panic_text() {
            load_and_drop(path)
        }
        t |> success(find(why, "bad magic") >= 0 && find(why, "tts_g2p_ru.bin") >= 0, why)
    }
}

[test]
def test_stress_pack(t : T?) {
    t |> run("the pack's forms, variants and ё spellings") @(t : T?) {
        return if (ru_pack_missing(t))
        var inscope p <- load_ru_stress_pack(pack_path())
        t |> success(ru_pack_loaded(p))
        var inscope milk <- ru_stress_variants(p, "молоко")
        t |> equal(length(milk), 1)
        t |> equal(milk[0], 3, "молоко́")
        var inscope castle <- ru_stress_variants(p, "замок")
        t |> equal(length(castle), 2, "за́мок and замо́к")
        var inscope none <- ru_stress_variants(p, "фывапролдж")
        t |> equal(length(none), 0)
        t |> equal(ru_yo_form(p, "елка"), "ёлка")
        t |> equal(ru_yo_form(p, "все"), "", "все and всё are both words: the text decides")
        t |> equal(ru_yo_form(p, "молоко"), "")
    }
    t |> run("marking a word: the text's mark, the clitics, the pack, the reflexive, the shape rule") @(t : T?) {
        return if (ru_pack_missing(t))
        var inscope p <- load_ru_stress_pack(pack_path())
        t |> equal(ru_mark_word(p, "молоко"), "молок+о")
        t |> equal(ru_mark_word(p, "Привет"), "Прив+ет", "the case stays")
        t |> equal(ru_mark_word(p, "зам+ок"), "зам+ок", "a marked word keeps its mark")
        t |> equal(ru_mark_word(p, "замо́к"), "замо́к")
        t |> equal(ru_mark_word(p, "ёлка"), "ёлка", "a ё is its own mark")
        t |> equal(ru_mark_word(p, "елка"), "ёлка", "and is restored where the е spelling is no word")
        t |> equal(ru_mark_word(p, "в"), "в")
        t |> equal(ru_mark_word(p, "не"), "не", "a clitic takes no stress")
        t |> equal(ru_mark_word(p, "компилируется"), "компил+ируется", "a reflexive the pack has only without -ся")
        t |> equal(ru_mark_word(p, "компилируюсь"), "компил+ируюсь", "and without -сь")
        t |> equal(ru_mark_word(p, "фывапролдж"), "фыв+апролдж", "the shape rule: the vowel before the last")
        t |> equal(ru_mark_word(p, "фывапролджся"), "фывапр+олджся", "a reflexive the pack lacks with its base falls to the shape rule")
        t |> equal(ru_mark_word(p, "из-за"), "из-за")
        t |> equal(ru_mark_stress(p, "Я иду домой, а ты?"), "+Я ид+у дом+ой, а т+ы?")
        t |> equal(ru_phonemize(p, "Я иду домой."), "ˈja ɪdˈu dɐmˈoj.")
        t |> equal(ru_phonemize(p, "Двенадцать мешков."), "dvʲinˈɑtʦətʲ mʲiʃkˈof.")
        var inscope flour <- ru_stress_variants(p, "муки")
        t |> equal(length(flour), 2, "му́ки and муки́ are both forms; the default is the corpus's more frequent one")
    }
}
