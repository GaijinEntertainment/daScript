options gen2
options _dasllama_internal = true
options _ascii_strings = false   // the Russian words and their IPA are the claims
options stack = 524288
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_g2p_ru
require dasllama/dasllama_g2p_ru_stress
require dasllama/dasllama_math
require dasllama/dasllama_unicode
require dasllama/dasllama_gguf   // the GGUF tensor type ids the synthesized stress-model headers carry
require strings
require daslib/strings_boost
require daslib/fio
require math
require _tts_corpus_ru
require _scratch_dir
require _gguf_writer   // the synthesized stress-model headers
require _model_tier
require _compares
require _json_file
require _tts_oracle

// The Russian front end. Model-free: letter-to-sound on pinned marked words (palatalization,
// iotation, the sign cases, voicing and final devoicing, the vowel reduction, the respellings,
// the clusters), the sandhi across words, hyphenated words, punctuation and the pass-through of
// another script's run, both mark spellings; then the 200-sentence fixture against kokoro-ru's
// own front end fed the same marked text, the word mismatches pinned; the WordPiece rules on a hand-built tokenizer (the word cap, the Hangul
// split) and its lookups (no allocation, the walk against a search of every slice length); and the stress model's and the tokenizer's load refusals over synthesized headers. The pack's cells on synthetic packs are
// `test_tts_g2p_ru_pack.das`. Each model-gated cell
// skips on the files it reads under models_dir():
// - `tts_g2p_ru.bin`: the stress pack - a form's variants and its default, ё restoration and the
//   ambiguous ё left alone, the clitics, a reflexive read through its base form, the shape rule on
//   a word the pack lacks, a mark in the text winning, the whole text-to-phonemes path, the words
//   the pack's other sources bring, the ё spellings restored and left alone, a hyphenated word by
//   its whole key, the tiers that outrank a context pass's pick, and the model-less tiers on the
//   shipped pack (`test_stress_modelless_tiers`);
// - `tts_g2p_ru.bin` and `tts_ctx_ru.gguf`: the context pass (`test_stress_context`);
// - `ru-stress-rubert-base.gguf` (`harness/convert_ru_stress.py` over ai-forever/ruBert-base with
//   the oracle's seeded head): the tokenizer cells against the 226-sentence fixture
//   `_tts_fixtures/ru_stress_tokens.json` - Russian sentences plus marked, Latin, digit, long-word,
//   punctuation, odd-codepoint and over-length cases - piece for piece and span for span
//   HuggingFace's, the accent strip the tokenizer config asks for and its control with the rewrite
//   tables emptied;
// - that file and the oracle dumps `tts_oracle/ru_stress/` (`harness/ru_stress_oracle.py`): the
//   encoder and API cells - 20 sentences' last hidden state on the f32 lane within F32_BAR of
//   PyTorch's and on the q8 lane within Q8_BAR, the head's logits within their lane's bar and the
//   f32 lane's every pick the reference's, each lane with its poisoned-input control; the batch
//   form against single picks, the catalog and the word finder;
// - `ru-stress-rubert-base-q4.gguf` and `ru-stress-rubert-base-f32.gguf`, the shipped lane against
//   the same model on f32 planes (the embedding and layer cells also read the oracle dumps for
//   their sentences): the pruned vocabulary spelling every fixture sentence in the unpruned pieces,
//   the Q8_0 word rows within EMB_BAR, the Q4_K layers' allowed logits within Q4_LOGIT_BAR and
//   their picks the f32 lane's but under NEAR_TIE, each with its control;
// - `ru-stress-ruelectra-small-q4.gguf`: the ELECTRA lane, its embedding projection included, and the
//   allocations its tokenizer's load makes.

def private pack_path() : string => path_join(models_dir(), "tts_g2p_ru.bin")

[test(description = "Russian letter-to-sound turns pinned stress-marked words into kokoro-ru's exact IPA labels, applies cross-word voicing sandhi, and handles hyphenated words, punctuation and passed-through foreign phonemes exactly")]
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
        t |> equal(ru_phonemize_marked("т+ак-то"), "tˈaktˈo", "a side's last vowel, stressed, reads a before a stressed side")
        t |> equal(ru_phonemize_marked("как+ая-то"), "kɐkˈɑjɪtˈo", "a vowel follows it in its side, so the stressed а reads ɑ")
        t |> equal(ru_phonemize_marked("буль+он"), "bulʲˈon", "a soft sign before a stressed о keeps the consonant soft")
        t |> equal(ru_phonemize_marked("р+оль"), "rˈol", "the control: a final ль reads hard")
        t |> equal(ru_phonemize_marked("Прив+ет, м+ир! К+ак дел+а?"), "prʲivʲˈet, mʲˈir! kˈɑk dʲilˈɑ?")
        t |> equal(ru_phonemize_marked("«Д+а» — сказ+ал (+он)."), "dˈɑ — skɐzˈɑl ˈon.", "quotes and brackets go, the dash stays")
        t |> equal(ru_phonemize_marked("сл+ово dˈæslæŋ сл+ово"), "slˈovə dˈæslæŋ slˈovə", "phonemes spliced in by the caller pass through")
    }
}

//! a corpus word that carries its stress: a `+` or a ё - the words letter-to-sound alone decides
def private is_marked(word : string) : bool => find(word, "+") >= 0 || find(word, "ё") >= 0 || find(word, "Ё") >= 0

[test(description = "On the 200-sentence Russian fixture, at least 1500 stress-marked words are compared with kokoro-ru's own front end and exactly 3 pinned words differ")]
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
        // pinned so a new miss reds: the three left are the reference's voiced final зь (грязь, насквозь) and its гч of облегчением
        t |> equal(misses, 3, "{misses} marked words differ from the reference")
        t |> success(marked_compared >= 1500, "{marked_compared} marked words compared")
    }
}

[test(description = "The Russian stress pack returns each form's stress variants and safe yo restorations, and `ru_mark_word` honours text marks, skips clitics, stresses reflexives via their base and unknown words by the shape rule, through to exact phonemes")]
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
        t |> equal(ru_mark_word(p, "фывапролдж"), "фыв+апролдж", "a word the pack lacks, by its ending")
        t |> equal(ru_mark_word(p, "фывапролджся"), "фывапр+олджся", "a reflexive the pack lacks with its base, and no ending of it held: the shape rule, the vowel before the last")
        t |> equal(ru_mark_word(p, "из-за"), "из-за", "a word made of clitics takes no stress, whatever the pack's whole key says")
        t |> equal(ru_mark_stress(p, "Я иду домой, а ты?"), "+Я ид+у дом+ой, а т+ы?")
        t |> equal(ru_phonemize(p, "Я иду домой."), "ˈja ɪdˈu dɐmˈoj.")
        t |> equal(ru_phonemize(p, "Двенадцать мешков."), "dvʲinˈɑtʦətʲ mʲiʃkˈof.")
        var inscope flour <- ru_stress_variants(p, "муки")
        t |> equal(length(flour), 2, "му́ки and муки́ are both forms; the default is the corpus's more frequent one")
    }
}

[test(description = "the stress pack reads the words its dictionary lacks from its other sources, restores ё as the dictionary does and leaves the true pairs and a name alone, and reads a hyphenated word by its whole key and then side by side")]
def test_stress_pack_sources(t : T?) {
    t |> run("participles, plurals and numerals the dictionary lacks, and an entry the sources correct") @(t : T?) {
        return if (ru_pack_missing(t))
        var inscope p <- load_ru_stress_pack(pack_path())
        let cases <- {
            "назначена" => "назн+ачена", "назначен" => "назн+ачен", "сделана" => "сд+елана", "написана" => "нап+исана",
            "решена" => "решен+а", "получена" => "пол+учена", "годы" => "г+оды", "годах" => "год+ах", "пяти" => "пят+и",
            "десяти" => "десят+и", "происходит" => "происх+одит"
        }
        for (word, want in keys(cases), values(cases)) {
            t |> equal(ru_mark_word(p, word), want, word)
        }
        t |> equal(ru_mark_word(p, "фешена"), "ф+ешена", "the control: a made-up word of решена's shape is read by its ending, which is not решена's reading")
        var inscope made <- ru_stress_variants(p, "создана")
        t |> equal(mismatch_exact(made, [3, 1]), 0, "создана́ first, со́здана second: {made}")
        var inscope chest <- ru_stress_variants(p, "груди")
        t |> equal(mismatch_exact(chest, [1, 2]), 0, "the dictionary's гру́ди keeps its place ahead of the груди́ both Wiktionary editions add: {chest}")
    }
    t |> run("ё: restored as the dictionary restores it, a true pair and a name left to the text") @(t : T?) {
        return if (ru_pack_missing(t))
        var inscope p <- load_ru_stress_pack(pack_path())
        let restored <- { "еще" => "ещё", "ее" => "её", "пришел" => "пришёл", "ребенок" => "ребёнок", "четырех" => "четырёх", "трех" => "трёх", "петр" => "пётр" }
        for (word, want in keys(restored), values(restored)) {
            t |> equal(ru_mark_word(p, word), want, word)
        }
        for (word in fixed_array("все", "небо", "берег", "левин", "якушев")) {
            t |> equal(ru_yo_form(p, word), "", "'{word}' is a word of its own")
        }
        t |> equal(ru_mark_word(p, "Левин"), "Л+евин", "the name keeps its е and its stress")
        t |> equal(ru_yo_form(p, "елка"), "ёлка", "the control: a е spelling that is no word is restored")
    }
    t |> run("a hyphenated word: the whole key first, then each side") @(t : T?) {
        return if (ru_pack_missing(t))
        var inscope p <- load_ru_stress_pack(pack_path())
        let cases <- {
            "что-нибудь" => "чт+о-ниб+удь", "кто-то" => "кт+о-т+о", "когда-нибудь" => "когд+а-ниб+удь", "какой-либо" => "как+ой-л+ибо",
            "из-за" => "из-за", "из-под" => "из-под", "северо-запад" => "с+еверо-з+апад", "по-русски" => "по-р+усски", "все-таки" => "всё-так+и"
        }
        for (word, want in keys(cases), values(cases)) {
            t |> equal(ru_mark_stress(p, word), want, word)
        }
        t |> equal(ru_mark_stress(p, "Что-нибудь"), "Чт+о-ниб+удь", "the case stays")
        t |> equal(ru_mark_stress(p, "из-з+а"), "из-з+а", "a mark in the text skips the whole key: the marked side stands, the clitic side takes none")
        t |> equal(ru_mark_stress(p, "фывапролдж-запад"), "фыв+апролдж-з+апад", "the control: with no whole key each side is read alone")
        t |> equal(ru_mark_stress(p, "северо-фывапролдж"), "с+еверо-фыв+апролдж", "and a first half reads by its own key there")
        t |> success(!ru_word_needs_context(p, "северо-запад"), "a hyphenated word is never a context pass's")
    }
}

[test(description = "with no context model the shipped pack reads its hand-ruled entries and restored ё names, an abbreviation by the last-vowel rule behind its own entries, a name's oblique case through the form it is a case of, twelve words it does not hold by their endings, the pronoun forms a preposition decides with ё, and a homograph by the rule tier behind a context pass's pick")]
def test_stress_modelless_tiers(t : T?) {
    t |> run("entries, abbreviations, case stems and endings") @(t : T?) {
        return if (ru_pack_missing(t))
        var inscope p <- load_ru_stress_pack(pack_path())
        let entries <- {
            "полчаса" => "полчас+а", "маленько" => "мал+енько", "активнее" => "акт+ивнее", "транспортно" => "тр+анспортно", "смолоду" => "см+олоду",
            "божией" => "б+ожией", "рефери" => "рефер+и", "федор" => "фёдор", "федору" => "фёдору", "семеновичем" => "семёновичем", "могилевской" => "могилёвской",
            "НАТО" => "Н+АТО", "РИА" => "Р+ИА", "ОАО" => "ОА+О", "МАГАТЭ" => "МАГАТ+Э", "УЕФА" => "УЕФ+А", "федерер" => "ф+едерер", "Сечин" => "С+ечин"
        }
        for (word, want in keys(entries), values(entries)) {
            t |> equal(ru_mark_word(p, word), want, word)
        }
        t |> equal(ru_mark_word(p, "АЭС"), "А+ЭС", "an abbreviation the pack does not hold: the last vowel")
        t |> equal(ru_mark_word(p, "УФСИН"), "УФС+ИН")
        t |> equal(ru_pack_default(p, "аэс"), 0, "the control: the pack holds no such form")
        let cases <- { "Федереру" => "Ф+едереру", "сечине" => "с+ечине", "вятровичу" => "вятр+овичу", "хартлпулы" => "х+артлпулы" }
        for (word, want in keys(cases), values(cases)) {
            t |> equal(ru_mark_word(p, word), want, "{word}: the stress of the form it is a case of")
            t |> equal(ru_pack_default(p, utf8_to_lower(word)), 0, "the control: '{word}' is no pack form")
        }
        t |> equal(ru_stem_ordinal(p, "фывапролджу"), 0, "the control: a word whose stem the pack lacks")
        t |> equal(ru_stem_ordinal(p, "федерера"), 0, "the control: an ending the fallback does not shed")
        let by_ending <- {
            "абракадабрация" => "абракадабр+ация", "бокренок" => "бокрен+ок", "курдячить" => "курд+ячить", "зюзюкинский" => "зюз+юкинский",
            "штрудельный" => "штруд+ельный", "хливкие" => "хл+ивкие", "шорьки" => "шорьк+и", "мюмзики" => "м+юмзики", "зелюки" => "зелюк+и",
            "хрюкотали" => "хрюкот+али", "варкалось" => "в+аркалось", "тыгыдымский" => "тыгыд+ымский"
        }
        for (word, want in keys(by_ending), values(by_ending)) {
            t |> equal(ru_mark_word(p, word), want, word)
            t |> success(ru_pack_default(p, word) == 0 && ru_stem_ordinal(p, word) == 0 && ru_ending_ordinal(p, word) > 0, "'{word}' is read by the ending table alone")
        }
        t |> equal(ru_ending_ordinal(p, "фывапролджся"), 0, "the control: a word no ending of which the table holds")
        t |> equal(ru_mark_word(p, "фывапролджся"), "фывапр+олджся", "falls to the shape rule")
    }
    t |> run("the ё a preposition decides, and the rule tier") @(t : T?) {
        return if (ru_pack_missing(t))
        var inscope p <- load_ru_stress_pack(pack_path())
        t |> equal(ru_mark_stress(p, "о чем"), "о чём")
        t |> equal(ru_mark_stress(p, "во всем мире"), "во всём м+ире")
        t |> equal(ru_mark_stress(p, "по всем"), "по вс+ем", "the control: по governs another case")
        t |> equal(ru_mark_stress(p, "из стены"), "из стен+ы", "the genitive after из")
        t |> equal(ru_mark_stress(p, "от руки"), "от рук+и")
        var inscope picked <- { 1 => 1 }
        t |> equal(ru_mark_stress(p, "из стены", picked), "из ст+ены", "the control: a context pass's pick outranks the rule")
        var inscope words <- ["из", "стены"]
        var inscope sentence <- [0, 0]
        t |> equal(ru_stress_rule(p, words, sentence, 1), 2, "the rule tier reads it by its neighbour")
        t |> equal(ru_stress_rule(p, words, sentence, 0), 0, "a word outside the rule table is not the tier's")
    }
}

def private ctx_path() : string => path_join(models_dir(), "tts_ctx_ru.gguf")

//! a sentence of a context pair, the homograph in it and the reading the sentence asks for, marked
struct RuContextCase {
    text : string
    word : string
    marked : string
}

//! twelve homographs, each in two sentences that ask for its two readings - one of them the pack's default
def private context_pairs() : array<RuContextCase> {
    return <- [
        RuContextCase(text = "на первой самой высокой террасе стоит замок.", word = "замок", marked = "з+амок"),
        RuContextCase(text = "для этого стоит поставить и дополнительный замок капота.", word = "замок", marked = "зам+ок"),
        RuContextCase(text = "у тебя на дне кружки какая цена стоит?", word = "кружки", marked = "кр+ужки"),
        RuContextCase(text = "спортивные кружки - платные, немалых денег стоит образование.", word = "кружки", marked = "кружк+и"),
        RuContextCase(text = "я все время плачу в этот момент.", word = "плачу", marked = "пл+ачу"),
        RuContextCase(text = "я за все плачу столько, сколько это стоит.", word = "плачу", marked = "плач+у"),
        RuContextCase(text = "потер руки.", word = "руки", marked = "р+уки"),
        RuContextCase(text = "но я лишился руки.", word = "руки", marked = "рук+и"),
        RuContextCase(text = "бегом за мной!", word = "бегом", marked = "бег+ом"),
        RuContextCase(text = "занимаюсь бегом трусцой.", word = "бегом", marked = "б+егом"),
        RuContextCase(text = "а то беды не оберетесь.", word = "беды", marked = "бед+ы"),
        RuContextCase(text = "от нее ваши беды.", word = "беды", marked = "б+еды"),
        RuContextCase(text = "и к белкам - витамины надо пить.", word = "белкам", marked = "белк+ам"),
        RuContextCase(text = "молодым белкам придется хлебнуть самой настоящей лесной жизни.", word = "белкам", marked = "б+елкам"),
        RuContextCase(text = "за какой временной период стоит проводить анализ?", word = "временной", marked = "временн+ой"),
        RuContextCase(text = "неужели она на самом деле хочет временной связи?", word = "временной", marked = "вр+еменной"),
        RuContextCase(text = "векам на память было предано", word = "векам", marked = "век+ам"),
        RuContextCase(text = "сухим векам было горячо и больно.", word = "векам", marked = "в+екам"),
        RuContextCase(text = "это его вина.", word = "вина", marked = "вин+а"),
        RuContextCase(text = "вредны ли крепленые вина?", word = "вина", marked = "в+ина"),
        RuContextCase(text = "лишь бы не было войны", word = "войны", marked = "войн+ы"),
        RuContextCase(text = "прекратим войны!", word = "войны", marked = "в+ойны"),
        RuContextCase(text = "длина волны - несколько нанометров", word = "волны", marked = "волн+ы"),
        RuContextCase(text = "скоро волны.", word = "волны", marked = "в+олны")
    ]
}

def private with_context_model(t : T?; blk : block<(p : RuStressPack; m : RuStressModel; var sc : RuStressScratch) : void>) {
    return if (ru_pack_missing(t) || !model_available(t, ctx_path()))
    var inscope p <- load_ru_stress_pack(pack_path())
    var inscope m <- load_ru_stress(ctx_path(), true)
    var inscope sc = RuStressScratch()
    invoke(blk, p, m, sc)
}

[cold_path]
def private context_marked(p : RuStressPack; m : RuStressModel; text : string; var sc : RuStressScratch) : string {
    var inscope picks <- ru_stress_context_picks(m, p, text, sc)
    return ru_mark_stress(p, text, picks.picked)
}

//! the control of a sentence cut: after the pass over `text`, the scratch holds the pieces of `last` alone, the text's last sentence
[cold_path]
def private last_pass_reads(t : T?; p : RuStressPack; m : RuStressModel; text, last : string; var sc : RuStressScratch) {
    var inscope picks <- ru_stress_context_picks(m, p, text, sc)
    var inscope whole <- ru_stress_pieces(m.tok, text, m.max_pieces)
    var inscope tail <- ru_stress_pieces(m.tok, last, m.max_pieces)
    t |> equal(length(sc.pieces), length(tail), "'{text}': the last pass reads '{last}' alone ({length(tail)} of the text's {length(whole)} pieces)")
    t |> success(length(tail) < length(whole), "the control: the whole text is more pieces")
    t |> equal(picks.fallbacks, 0, "every asked word lies inside its sentence's pass")
}

[test(description = "the tiers ahead of a context pass - a mark in the text, a ё, a clitic, the pack's ё spelling, a hyphenated word - keep their reading under a hand-built pick, and `ru_word_needs_context` names exactly the words a pass may decide")]
def test_stress_pick_order(t : T?) {
    t |> run("a pick for a closed word changes nothing; a pick for an open word is its reading") @(t : T?) {
        return if (ru_pack_missing(t))
        var inscope p <- load_ru_stress_pack(pack_path())
        let text = "з+амок всё не елка из-за замок"
        var inscope cps <- utf8_to_cpts(text)
        var inscope words <- ru_text_words(cps)
        t |> equal(length(words), 6, "six words")
        let plain = ru_mark_stress(p, text)
        t |> equal(plain, "з+амок всё не ёлка из-за з+амок", "the pack alone: its default reading of the open word")
        for (ordinal in range(1, 3)) {
            var inscope picked <- {for (k in range(length(words))); k => ordinal}
            let open_word = ordinal == 1 ? "з+амок" : "зам+ок"
            t |> equal(ru_mark_stress(p, text, picked), "з+амок всё не ёлка из-за {open_word}", "every word picked at {ordinal}: the closed words stand, the open one takes it")
        }
        let closed = fixed_array("з+амок", "замо́к", "всё", "не", "елка", "из-за")
        for (word in closed) {
            t |> success(!ru_word_needs_context(p, word), "'{word}' is decided before a context pass")
        }
        t |> success(ru_word_needs_context(p, "замок"), "the control: 'замок' is a context pass's to decide")
        t |> success(ru_word_needs_context(p, "молоко"), "and so is a word of one reading: the catalog, not this gate, keeps it from the model")
    }
}

[cold_path]
def private context_mark_and_pass_cells(t : T?) {
    t |> run("the text's mark and a ё outrank the model, which is not asked about them") @(t : T?) {
        with_context_model(t) $(p, m; var sc) {
            let marked = "он открыл з+амок ключом."
            var inscope picks <- ru_stress_context_picks(m, p, marked, sc)
            t |> equal(picks.asked, 0, "a marked word is not asked")
            t |> success(find(ru_mark_stress(p, marked, picks.picked), "з+амок") >= 0, "the mark stands")
            var inscope yo <- ru_stress_context_picks(m, p, "всё равно я лишился руки.", sc)
            t |> equal(yo.asked, 1, "руки is asked, всё is not")
        }
    }
    t |> run("one pass for every word of a sentence: each word's reading the single pick's") @(t : T?) {
        with_context_model(t) $(p, m; var sc) {
            let text = "я за все плачу столько, сколько это стоит, а он потер руки."
            var inscope cps <- utf8_to_cpts(text)
            var inscope words <- ru_text_words(cps)
            var inscope picks <- ru_stress_context_picks(m, p, text, sc)
            t |> success(picks.asked >= 2, "плачу and руки asked in one pass ({picks.asked})")
            t |> equal(picks.fallbacks, 0, "every pick a reading the pack holds")
            for (w, k in words, count()) {
                let lower = utf8_to_lower(cpts_to_utf8(cps, w.x, w.y))
                continue if (!key_exists(picks.picked, k))
                let single = ru_stress_pick(m, text, w, ru_stress_allowed(m, lower), sc)
                t |> equal(picks.picked?[k] ?? 0, single.ordinal, "{lower}: the one pass and the single pick")
            }
        }
    }
}

[test(description = "the stress-context model picks a homograph's reading by its sentence, behind the text's own mark and ё, one pass for a sentence's every word, a catalog reading the pack lacks taken and counted")]
def test_stress_context(t : T?) {
    with_dasllama_jobque_() {
        t |> run("twelve homographs read both ways by their sentences; the pack and its rule tier alone misread eight of the sentences") @(t : T?) {
            with_context_model(t) $(p, m; var sc) {
                var inscope cases <- context_pairs()
                t |> equal(length(cases), 24, "twelve pairs")
                var pack_wrong = 0
                for (c in cases) {
                    t |> equal(utf8_to_lower(replace(c.marked, "+", "")), c.word, "'{c.marked}' is a reading of '{c.word}'")
                    t |> success(ru_stress_allowed(m, c.word) != 0u, "'{c.word}' is a form the catalog lists")
                    let got = context_marked(p, m, c.text, sc)
                    t |> success(find(got, c.marked) >= 0, "'{c.text}': {got} reads {c.marked}")
                    pack_wrong += find(ru_mark_stress(p, c.text), c.marked) >= 0 ? 0 : 1
                }
                t |> equal(pack_wrong, 8, "the control: without the model the pack and its rule tier misread 8 of the 24 sentences")
            }
        }
        context_mark_and_pass_cells(t)
        t |> run("a catalog reading the pack lacks is taken and counted; the pack's readings count nothing") @(t : T?) {
            with_context_model(t) $(p, m; var sc) {
                t |> success(ru_stress_pack_has(p, "руки", 2), "руки́ is a reading the pack holds")
                t |> success(!ru_stress_pack_has(p, "руки", 3), "the control: no third reading")
                var inscope held <- ru_stress_context_picks(m, p, "но я лишился руки.", sc)
                t |> equal(held.asked, 1)
                t |> equal(held.beyond_pack, 0, "a pick among the pack's readings")
                var inscope variants <- ru_stress_variants(p, "бухал")
                t |> equal(length(variants), 2, "the pack holds both readings of бухал, the second from the other sources")
                let text = "бухал, как все фронтовики в то время."
                var inscope beyond <- ru_stress_context_picks(m, p, text, sc)
                t |> equal(beyond.beyond_pack, 0, "the model's буха́л is a reading the pack holds")
                t |> equal(beyond.fallbacks, 0)
                t |> success(starts_with(ru_mark_stress(p, text, beyond.picked), "бух+ал"), "and the text reads it")
                t |> success(starts_with(ru_mark_stress(p, text), "б+ухал"), "the control: the pack alone reads its one reading")
            }
        }
    }
}

[test(description = "the context pass cuts a text into sentences at a period, an ellipsis and a `?!`, one pass each with its words at their own offsets, and reads a sentence past the piece budget through windows whose edges sit on word starts")]
def test_stress_context_cuts(t : T?) {
    with_dasllama_jobque_() {
        t |> run("three sentences, one homograph in the first and the third: each pass reads its own sentence, each word its own reading") @(t : T?) {
            with_context_model(t) $(p, m; var sc) {
                var inscope cases <- context_pairs()
                let first = cases[0].text
                let middle = cases[23].text
                let third = cases[1].text
                let text = "{first} {middle} {third}"
                let got = context_marked(p, m, text, sc)
                let want = "{context_marked(p, m, first, sc)} {context_marked(p, m, middle, sc)} {context_marked(p, m, third, sc)}"
                t |> equal(got, want, "the text reads what its sentences read one by one")
                t |> success(find(got, "{cases[0].marked}.") >= 0, "the first sentence's замок: {got}")
                t |> success(find(got, "{cases[1].marked} ") >= 0, "the third sentence's замок: {got}")
                last_pass_reads(t, p, m, text, third, sc)
                last_pass_reads(t, p, m, "потер руки… скоро волны.", "скоро волны.", sc)
                last_pass_reads(t, p, m, "но я лишился руки?! скоро волны.", "скоро волны.", sc)
                var inscope uncut <- ru_stress_context_picks(m, p, "потер руки, скоро волны.", sc)
                var inscope whole <- ru_stress_pieces(m.tok, "потер руки, скоро волны.", m.max_pieces)
                t |> equal(length(sc.pieces), length(whole), "the control: a comma ends no sentence, one pass reads the whole text")
                t |> equal(uncut.asked, 2, "both words asked in that one pass")
            }
        }
        t |> run("a sentence past the piece budget: a window of whole words around each word, both ends of the sentence read") @(t : T?) {
            with_context_model(t) $(p, m; var sc) {
                var filler = ""
                for (_i in range(40)) {
                    filler += "мы шли и шли, "
                }
                let text = "потер руки, {filler}но я лишился руки."
                var inscope all <- ru_stress_pieces(m.tok, text, WORDPIECE_NO_BUDGET)
                t |> success(length(all) > m.max_pieces, "the sentence holds {length(all)} pieces, past the budget of {m.max_pieces}")
                var inscope picks <- ru_stress_context_picks(m, p, text, sc)
                let got = ru_mark_stress(p, text, picks.picked)
                to_log(LOG_INFO, "window: asked {picks.asked}, {got}\n")
                t |> success(find(got, "р+уки,") >= 0, "the first руки reads its own window")
                t |> success(ends_with(got, "рук+и."), "the last руки reads its own window")
            }
        }
        t |> run("a window's edges sit on word starts: among words of several pieces an edge inside a word moves in to the next word") @(t : T?) {
            with_context_model(t) $(p, m; var sc) {
                var filler = ""
                for (_i in range(30)) {
                    filler += "необыкновенностями перераспределялись, "
                }
                let text = "потер руки, {filler}но я лишился руки."
                var inscope all <- ru_stress_pieces(m.tok, text, WORDPIECE_NO_BUDGET)
                let body = length(all) - 2
                let budget = m.max_pieces - 2
                t |> success(body > budget, "the sentence holds {body} pieces, past a window of {budget}")
                var inside_words = 0
                var windows = 0
                var moved_in = 0
                for (a in range(1, body + 1)) {
                    inside_words += is_continuation(m.tok, all[a]) ? 1 : 0
                    continue if (is_continuation(m.tok, all[a]))
                    let w = ru_stress_window(m.tok, all, int2(a, a + 1), budget, body)
                    windows++
                    moved_in += w.y - w.x < min(budget, body) ? 1 : 0
                    t |> success(w.x <= a && w.y > a && w.y - w.x <= budget, "the window [{w.x}, {w.y}) holds piece {a} within the budget")
                    t |> success(!is_continuation(m.tok, all[w.x]), "the window of piece {a} starts on a word start ({w.x})")
                    t |> success(w.y == body + 1 || !is_continuation(m.tok, all[w.y]), "the window of piece {a} ends before a word start ({w.y})")
                }
                t |> success(inside_words >= 30, "the filler's words are several pieces each ({inside_words} continuation pieces, {windows} word starts)")
                t |> success(moved_in > 0, "the control: {moved_in} of {windows} windows are shorter than the budget - an edge fell inside a word and moved")
                let got = context_marked(p, m, text, sc)
                t |> success(find(got, "р+уки,") >= 0, "the first руки reads its own window: {got}")
                t |> success(ends_with(got, "рук+и."), "the last руки reads its own window")
            }
        }
    }
}

// ===== the WordPiece rules on a hand-built tokenizer =====

let HANGUL_HAN : int = 0xD55C   //! a syllable of three jamo: U+1112, U+1161, U+11AB
let HANGUL_HA : int = 0xD558    //! the same without its trailing consonant
let JAMO_LEAD : int = 0x1112
let JAMO_VOWEL : int = 0x1161
let JAMO_TRAIL : int = 0x11AB

//! a tokenizer of one class (every codepoint kept) over three Hangul jamo and the letter а, a word capped at five codepoints
def private toy_tokenizer() : WordPieceTokenizer {
    var tk <- wordpiece_from_pieces(["[UNK]", "[CLS]", "[SEP]", cpt_to_utf8(JAMO_LEAD), "##{cpt_to_utf8(JAMO_VOWEL)}", "##{cpt_to_utf8(JAMO_TRAIL)}", "а", "##а"])
    tk.strip_accents = true
    tk.max_word_chars = 5
    tk.unk_id = 0
    tk.cls_id = 1
    tk.sep_id = 2
    tk.class_starts <- [0]
    tk.class_kinds <- [BertCharKind.keep]
    tk.rewrite_off <- [0]
    return <- tk
}

def private toy_pieces(tk : WordPieceTokenizer; text : string) : array<WordPiece> {
    var inscope cpts <- utf8_to_cpts(text)
    return <- wordpiece_encode(tk, cpts, WORDPIECE_NO_BUDGET)
}

def private piece_ids(pieces : array<WordPiece>) : array<int> => <- [for (p in pieces); p.id]

def private ids_are(t : T?; pieces : array<WordPiece>; want : array<int>; what : string) {
    var inscope got <- piece_ids(pieces)
    t |> equal(mismatch_exact(got, want), 0, "{what}: ids {got} against {want}")
}

[test(description = "WordPiece on a hand-built vocabulary: a word past the word cap is one [UNK] and a word at the cap splits; under the accent strip a Hangul syllable splits into its jamo, without it the syllable is one unknown codepoint")]
def test_wordpiece_rules(t : T?) {
    t |> run("a word one codepoint past the cap is one [UNK] over its whole span; a word at the cap splits") @(t : T?) {
        var inscope tk <- toy_tokenizer()
        var inscope at_cap <- toy_pieces(tk, "ааааа")
        ids_are(t, at_cap, [1, 6, 7, 7, 7, 7, 2], "five codepoints, the cap")
        var inscope past_cap <- toy_pieces(tk, "аааааа")
        ids_are(t, past_cap, [1, 0, 2], "six codepoints")
        t |> equal(int2(past_cap[1].start, past_cap[1].end), int2(0, 6), "the [UNK] spans the word")
    }
    t |> run("a Hangul syllable under the strip: its leading, vowel and trailing jamo, each over the syllable's one codepoint") @(t : T?) {
        var inscope tk <- toy_tokenizer()
        var inscope han <- toy_pieces(tk, cpt_to_utf8(HANGUL_HAN))
        ids_are(t, han, [1, 3, 4, 5, 2], "a syllable with a trailing consonant")
        for (p in han) {
            t |> success(p.id == tk.cls_id || p.id == tk.sep_id || (p.start == 0 && p.end == 1), "piece {p.id} spans the syllable ({p.start}, {p.end})")
        }
        var inscope ha <- toy_pieces(tk, cpt_to_utf8(HANGUL_HA))
        ids_are(t, ha, [1, 3, 4, 2], "a syllable with none: two jamo, no third")
        tk.strip_accents = false
        var inscope whole <- toy_pieces(tk, cpt_to_utf8(HANGUL_HAN))
        ids_are(t, whole, [1, 0, 2], "the control: without the strip the syllable is one codepoint the vocabulary lacks")
    }
}

let LOOKUP_WORDS = 3000
let LOOKUP_FIRST = 4   //! the id of the first numeral word: three specials and one long piece sit ahead of them

//! the base-7 digits of `i` in seven Russian letters: a word of its own for every `i`
def private numeral_word(i : int) : string {
    let letters = fixed_array("а", "б", "в", "г", "д", "е", "ж")
    return i < 7 ? letters[i] : "{numeral_word(i / 7)}{letters[i % 7]}"
}

//! a tokenizer of one class over `[UNK]`, `[CLS]`, `[SEP]`, the piece щщщ, then `words` bare and then behind `##`
def private numeral_tokenizer(words : array<string>) : WordPieceTokenizer {
    var inscope pieces <- ["[UNK]", "[CLS]", "[SEP]", "щщщ"]
    pieces |> reserve(length(pieces) + 2 * length(words))
    pieces |> push_from(words)
    for (w in words) {
        pieces |> push("##{w}")
    }
    var tk <- wordpiece_from_pieces(pieces)
    tk.unk_id = 0
    tk.cls_id = 1
    tk.sep_id = 2
    tk.class_starts <- [0]
    tk.class_kinds <- [BertCharKind.keep]
    tk.rewrite_off <- [0]
    return <- tk
}

//! the split the per-length search makes of `word`: from each start the longest slice that is a piece, a slice past the first behind `##`; one [UNK] where a start has none
def private split_by_length(tk : WordPieceTokenizer; word : string) : array<int> {
    var inscope cpts <- utf8_to_cpts(word)
    var ids : array<int>
    var start = 0
    while (start < length(cpts)) {
        var stop = length(cpts)
        var id = -1
        while (stop > start && id < 0) {
            id = wordpiece_find(tk, cpts_to_utf8(cpts, start, stop), start > 0)
            stop -= id < 0 ? 1 : 0
        }
        if (id < 0) {
            delete ids
            ids <- [tk.unk_id]
            return <- ids
        }
        ids |> push(id)
        start = stop
    }
    return <- ids
}

[test(description = "a WordPiece lookup allocates nothing: 3,000 pieces found bare and behind the prefix, as many absent words missed and a piece's continuation asked leave both heaps' allocation counts where they were, the calls that return a string or an array move theirs, and the one walk down the byte-sorted pieces splits 2,000 words as a search of every slice length splits them")]
def test_wordpiece_lookup(t : T?) {
    t |> run("every piece found by its bytes, every absent word missed, inside one allocation bracket") @(t : T?) {
        var inscope words <- [for (i in range(LOOKUP_WORDS)); numeral_word(i)]
        var inscope absent <- [for (w in words); "{w}ъ"]
        var inscope tk <- numeral_tokenizer(words)
        var inscope word_cpts <- utf8_to_cpts(words[10])
        var bare = 0
        var behind = 0
        var misses = 0
        var continuing = 0
        let heap0 = heap_allocation_count()
        let strings0 = string_heap_allocation_count()
        for (w, a, i in words, absent, count()) {
            bare += wordpiece_find(tk, w) == LOOKUP_FIRST + i ? 1 : 0
            behind += wordpiece_find(tk, w, true) == LOOKUP_FIRST + LOOKUP_WORDS + i ? 1 : 0
            misses += wordpiece_find(tk, a) < 0 ? 1 : 0
            misses += wordpiece_find(tk, a, true) < 0 ? 1 : 0
            continuing += is_continuation(tk, WordPiece(id = LOOKUP_FIRST + LOOKUP_WORDS + i, end = 1)) ? 1 : 0
            continuing += is_continuation(tk, WordPiece(id = LOOKUP_FIRST + i, end = 1)) ? 1 : 0
        }
        let short_of_a_piece = wordpiece_find(tk, "щщ")
        let past_a_piece = wordpiece_find(tk, "щщщщ")
        let the_piece = wordpiece_find(tk, "щщщ")
        let nothing = wordpiece_find(tk, "")
        let prefix_alone = wordpiece_find(tk, "", true)
        let heap1 = heap_allocation_count()
        let strings1 = string_heap_allocation_count()
        t |> equal(bare, LOOKUP_WORDS, "every word found at its own id")
        t |> equal(behind, LOOKUP_WORDS, "every word behind the prefix found at its continuation's id")
        t |> equal(misses, 2 * LOOKUP_WORDS, "no word with a letter the vocabulary lacks is a piece, bare or behind the prefix")
        t |> equal(continuing, LOOKUP_WORDS, "every piece behind the prefix continues a word and no bare one does")
        t |> equal(the_piece, 3, "щщщ is piece 3")
        t |> success(short_of_a_piece < 0 && past_a_piece < 0, "a piece's own first bytes and a word one letter past it are no piece ({short_of_a_piece}, {past_a_piece})")
        t |> success(nothing < 0 && prefix_alone < 0, "the empty word and the prefix alone are no piece ({nothing}, {prefix_alone})")
        t |> equal(heap1 - heap0, 0ul, "heap allocations over {6 * LOOKUP_WORDS} lookups")
        t |> equal(strings1 - strings0, 0ul, "string heap allocations over the same lookups")
        let spelled = wordpiece_piece(tk, LOOKUP_FIRST + 10)
        t |> success(string_heap_allocation_count() > strings1 && spelled == words[10], "the control: the call that returns a piece's spelling allocates the string ({spelled})")
        var inscope pieces <- wordpiece_encode(tk, word_cpts, WORDPIECE_NO_BUDGET)
        t |> success(heap_allocation_count() > heap1 && length(pieces) == 3, "the control: the call that returns the pieces allocates them")
    }
    t |> run("the walk down the sorted pieces against a search of every slice length: 2,000 words of two or three pieces, some with no split") @(t : T?) {
        var inscope words <- [for (i in range(LOOKUP_WORDS)); numeral_word(i)]
        var inscope tk <- numeral_tokenizer(words)
        var several = 0
        var unknown = 0
        var differ = 0
        for (i in range(2000)) {
            let word = "{words[i * 7 % LOOKUP_WORDS]}{numeral_word(i + LOOKUP_WORDS)}{i % 5 == 0 ? "ъ" : ""}{i % 3 == 0 ? "щщщ" : ""}"
            var inscope cpts <- utf8_to_cpts(word)
            var inscope pieces <- wordpiece_encode(tk, cpts, WORDPIECE_NO_BUDGET)
            var inscope got <- [for (p in pieces); p.id; where p.end > p.start]
            var inscope want <- split_by_length(tk, word)
            differ += mismatch_exact(got, want) != 0 ? 1 : 0
            several += length(want) > 1 ? 1 : 0
            unknown += length(want) == 1 && want[0] == tk.unk_id ? 1 : 0
        }
        t |> equal(differ, 0, "words the walk splits apart from the per-length search")
        t |> success(several >= 1000 && unknown >= 300, "the words split into several pieces ({several}) and fall to [UNK] ({unknown})")
        var inscope whole <- utf8_to_cpts("бвг")
        var inscope whole_ids <- piece_ids(wordpiece_encode(tk, whole, WORDPIECE_NO_BUDGET))
        var inscope longest <- [tk.cls_id, wordpiece_find(tk, "бвг"), tk.sep_id]
        var inscope shortest <- [tk.cls_id, wordpiece_find(tk, "б"), wordpiece_find(tk, "в", true), wordpiece_find(tk, "г", true), tk.sep_id]
        t |> equal(mismatch_exact(whole_ids, longest), 0, "бвг is its one longest piece: {whole_ids}")
        t |> success(mismatch_exact(whole_ids, shortest) != 0, "the control: a split into the shortest pieces, each a piece too, is counted a mismatch ({shortest})")
    }
}

// ===== the stress model's load refusals over synthesized headers =====

//! one tensor's info appended to `b` at the data cursor `off`, which moves past the tensor on the 32-byte grid
def private synth_tensor(var b : array<uint8>; var off : uint64&; name : string; dims : array<uint64>; gtype : int) {
    var n = 1l
    for (d in dims) {
        n *= int64(d)
    }
    gguf_tensor_info(b, name, dims, gtype, off)
    off += uint64((ggml_type_bytes(gtype, n) + 31l) & ~31l)
}

//! a `ru_stress` GGUF 256 wide: the shape keys and, `with_tensors`, zeroed embeddings and the first layer's query - its bias,
//! and its weight of disk type `query_type` - so the loader reads up to that weight and judges its type
def private synth_ru_stress(with_tensors : bool; query_type : int) : array<uint8> {
    let d = 256ul
    var b : array<uint8>
    gguf_header(b, with_tensors ? 7ul : 0ul, 11ul)
    gguf_kv_str(b, "general.architecture", "ru_stress")
    gguf_kv_str(b, "ru_stress.encoder", "bert")
    for (kv in [("emb_dim", d), ("hidden", d), ("n_heads", 4ul), ("max_positions", 4ul), ("max_pieces", 4ul), ("n_ordinals", 8ul), ("n_layers", 1ul)]) {
        gguf_kv_u32(b, "ru_stress.{kv._0}", kv._1)
    }
    gguf_kv_f32_bits(b, "ru_stress.layer_norm_eps", 0x2B8CBCCCul)
    gguf_kv_str(b, "ru_stress.activation", "gelu")
    var off = 0ul
    if (with_tensors) {
        synth_tensor(b, off, "bert.embeddings.word_embeddings.weight", [d, 2ul], GGML_TYPE_F32)
        synth_tensor(b, off, "bert.embeddings.position_embeddings.weight", [d, 4ul], GGML_TYPE_F32)
        synth_tensor(b, off, "bert.embeddings.token_type_embeddings.weight", [d, 2ul], GGML_TYPE_F32)
        synth_tensor(b, off, "bert.embeddings.LayerNorm.weight", [d], GGML_TYPE_F32)
        synth_tensor(b, off, "bert.embeddings.LayerNorm.bias", [d], GGML_TYPE_F32)
        synth_tensor(b, off, "bert.layer.0.attention.query.bias", [d], GGML_TYPE_F32)
        synth_tensor(b, off, "bert.layer.0.attention.query.weight", [d, d], query_type)
    }
    gguf_pad_data(b)
    b |> resize(long_length(b) + int64(off))
    return <- b
}

def private synth_file(name : string; var bytes : array<uint8>) : string {
    let path = scratch_path(name)
    fopen(path, "wb") $(f) {
        fwrite(f, bytes)
    }
    delete bytes
    return path
}

//! a `ru_stress` GGUF holding a bert tokenizer of four pieces and, where `sorted` is not empty, that list as their sorted ids
def private synth_tokenizer(sorted : array<int>) : array<uint8> {
    var b : array<uint8>
    gguf_header(b, 0ul, empty(sorted) ? 3ul : 4ul)
    gguf_kv_str(b, "general.architecture", "ru_stress")
    gguf_kv_str(b, "tokenizer.ggml.model", "bert")
    gguf_kv_str_array(b, "tokenizer.ggml.tokens", ["[UNK]", "б", "##а", "а"])
    if (!empty(sorted)) {
        gguf_kv_i32_array(b, "ru_stress.tokenizer.sorted_ids", sorted)
    }
    return <- b
}

def private load_model_and_drop(path : string) {
    var inscope m <- load_ru_stress(path, true)
}

def private load_tokenizer_and_drop(path : string) {
    var inscope tk <- load_wordpiece_gguf(path, "ru_stress")
}

[test(description = "`load_ru_stress` refuses, naming the cause, a missing file, a GGUF of another architecture, a file without a tensor it needs and a layer stored in a quantized type the layer GEMMs do not serve (the tensor, the site and the format member in the text); `load_wordpiece_gguf` refuses a tokenizer that is not bert, one without the sorted piece ids (the key and the converter in the text) and one whose sorted ids are not the pieces' byte order")]
def test_stress_model_refusals(t : T?) {
    t |> run("a missing file panics naming it") @(t : T?) {
        let missing = scratch_path("no-such-ru-stress.gguf")
        let why = panic_text() {
            load_model_and_drop(missing)
        }
        t |> success(find(why, "cannot open model") >= 0 && find(why, "no-such-ru-stress.gguf") >= 0, why)
    }
    t |> run("a GGUF of another architecture is refused by its name") @(t : T?) {
        let other = arch_gguf("kokoro_arch.gguf", "kokoro")
        let why = panic_text() {
            load_model_and_drop(other)
        }
        t |> success(find(why, "architecture is 'kokoro', not ru_stress") >= 0, why)
    }
    t |> run("a file without the word-embedding table is refused naming the tensor") @(t : T?) {
        let bare = synth_file("ru_stress_no_tensors.gguf", synth_ru_stress(false, GGML_TYPE_F32))
        let why = panic_text() {
            load_model_and_drop(bare)
        }
        t |> success(find(why, "tensor 'bert.embeddings.word_embeddings.weight' missing") >= 0, why)
    }
    t |> run("a layer weight stored Q8_0 or Q5_0 is refused naming the tensor, the layer GEMMs and the format member; Q4_K passes") @(t : T?) {
        let members = fixed_array("format q8", "format q5032")
        for (gtype, member in fixed_array(GGML_TYPE_Q8_0, GGML_TYPE_Q5_0), members) {
            let path = synth_file("ru_stress_layer_{ggml_type_name(gtype)}.gguf", synth_ru_stress(true, gtype))
            let why = panic_text() {
                load_model_and_drop(path)
            }
            t |> success(find(why, "'bert.layer.0.attention.query.weight'") >= 0, "the tensor: {why}")
            t |> success(find(why, "the stress encoder's layer GEMMs") >= 0, "the site: {why}")
            t |> success(find(why, ggml_type_name(gtype)) >= 0 && find(why, member) >= 0, "the disk type and the member: {why}")
        }
        let served = synth_file("ru_stress_layer_q4k.gguf", synth_ru_stress(true, GGML_TYPE_Q4_K))
        let next = panic_text() {
            load_model_and_drop(served)
        }
        t |> success(find(next, "tensor 'bert.layer.0.attention.key.weight' missing") >= 0, "the control: a Q4_K query weight is read, the load stops at the next, absent tensor: {next}")
    }
    t |> run("a tokenizer whose model is not bert is refused") @(t : T?) {
        let no_tokenizer = arch_gguf("ru_stress_no_tokenizer.gguf", "ru_stress")
        let why = panic_text() {
            load_tokenizer_and_drop(no_tokenizer)
        }
        t |> success(find(why, "tokenizer.ggml.model is not bert") >= 0, why)
    }
    t |> run("a tokenizer without the sorted piece ids is refused naming the key and the converter") @(t : T?) {
        var inscope none : array<int>
        let unsorted = synth_file("ru_stress_no_sorted_ids.gguf", synth_tokenizer(none))
        let why = panic_text() {
            load_tokenizer_and_drop(unsorted)
        }
        t |> success(find(why, "no ru_stress.tokenizer.sorted_ids") >= 0 && find(why, "harness/convert_ru_stress.py") >= 0, why)
    }
    t |> run("sorted ids that are not the pieces' byte order, or not one a piece, are refused; the right order is read and the load stops at the next, absent table") @(t : T?) {
        let id_order = synth_file("ru_stress_sorted_ids_id_order.gguf", synth_tokenizer([0, 1, 2, 3]))
        let out_of_order = panic_text() {
            load_tokenizer_and_drop(id_order)
        }
        t |> success(find(out_of_order, "ru_stress.tokenizer.sorted_ids is not the pieces' byte order at entry 2") >= 0, out_of_order)
        let short = synth_file("ru_stress_sorted_ids_short.gguf", synth_tokenizer([2, 0, 3]))
        let too_few = panic_text() {
            load_tokenizer_and_drop(short)
        }
        t |> success(find(too_few, "ru_stress.tokenizer.sorted_ids lists 3 ids for 4 pieces") >= 0, too_few)
        let byte_order = synth_file("ru_stress_sorted_ids.gguf", synth_tokenizer([2, 0, 3, 1]))
        let next = panic_text() {
            load_tokenizer_and_drop(byte_order)
        }
        t |> success(find(next, "missing key 'ru_stress.tokenizer.class_starts'") >= 0, "the control: ##а, [UNK], а, б is the byte order, and the load stops at the next key: {next}")
    }
}

let LOAD_STRINGS_BAR = 24ul   //! the string-heap allocations one tokenizer load may make - the key names it spells and the settings it reads as strings: it reads 14
let LOAD_HEAP_BAR = 128ul     //! the heap allocations one tokenizer load may make - its arrays and the integer arrays it widens on the way: it reads 69

[test(description = "loading the small model's tokenizer makes no string a piece: over a vocabulary of more than a hundred thousand pieces the load's string-heap allocations stay within two dozen and its heap allocations within 128, and spelling and tokenizing pieces on demand moves each count past its bound")]
def test_wordpiece_load_allocations(t : T?) {
    t |> run("the tokenizer of ru-stress-ruelectra-small-q4.gguf read off its parsed header inside one allocation bracket") @(t : T?) {
        let path = path_join(models_dir(), RU_STRESS_ELECTRA)
        return if (!model_available(t, path))
        var inscope tk = WordPieceTokenizer()
        var strings = 0ul
        var arrays = 0ul
        let read = gguf_header_read(path) $(m; bytes) {
            let strings0 = string_heap_allocation_count()
            let heap0 = heap_allocation_count()
            delete tk
            tk <- wordpiece_of_gguf(m, bytes, "ru_stress")
            strings = string_heap_allocation_count() - strings0
            arrays = heap_allocation_count() - heap0
        }
        let n = wordpiece_count(tk)
        to_log(LOG_INFO, "wordpiece load: {n} pieces in {length(tk.bytes)} bytes, {int(strings)} string-heap and {int(arrays)} heap allocations\n")
        t |> success(read, "the file's header parses and the tokenizer loads")
        t |> success(n > 100000 && length(tk.piece_at) == n + 1, "the vocabulary holds {n} pieces, one start a piece and one closing the last")
        t |> success(strings <= LOAD_STRINGS_BAR, "the load made {int(strings)} strings, within {int(LOAD_STRINGS_BAR)}")
        t |> success(arrays <= LOAD_HEAP_BAR, "the load made {int(arrays)} heap allocations, within {int(LOAD_HEAP_BAR)}")
        let strings1 = string_heap_allocation_count()
        let heap1 = heap_allocation_count()
        var spelled_bytes = 0
        var encoded = 0
        for (id in range(int(LOAD_HEAP_BAR) + 1)) {
            let spelled = wordpiece_piece(tk, id)
            spelled_bytes += length(spelled)
            var inscope cpts <- utf8_to_cpts(spelled)
            var inscope pieces <- wordpiece_encode(tk, cpts, WORDPIECE_NO_BUDGET)
            encoded += length(pieces)
        }
        let on_demand = string_heap_allocation_count() - strings1
        let per_call = heap_allocation_count() - heap1
        t |> success(on_demand > LOAD_STRINGS_BAR && spelled_bytes > 0, "the control: spelling {int(LOAD_HEAP_BAR) + 1} pieces on demand makes {int(on_demand)} strings, past the bound")
        t |> success(per_call > LOAD_HEAP_BAR && encoded > 0, "the control: tokenizing each of them makes {int(per_call)} heap allocations, past the bound")
    }
}

// ===== the stress-context model against its PyTorch reference =====

let RU_STRESS_GGUF = "ru-stress-rubert-base.gguf"
let F32_BAR = 1e-4   //! of the larger magnitude floored at 1: twelve f32 layers on the tiled GEMM against torch's read 7e-6
let Q8_BAR = 0.25    //! the q8 lane: Q8_0 weights and per-32 activations through twelve layers read 0.084
let F32_LOGIT_BAR = 1e-4   //! reads 1.2e-6
let Q8_LOGIT_BAR = 0.06    //! reads 0.018
let POISON_SCALED = 2.0   //! the poisoned expectation's move, times its magnitude floored at 1: past every bar above
let FIXTURE_SENTENCES = 226
let ORACLE_CASES = 20
let COMBINING_ACUTE = 769   //! U+0301, the stress mark after its vowel

struct RuStressTokCase {
    kind : string
    text : string
    ids : array<int>
    offsets : array<int>
}

struct RuStressTokFixture {
    checkpoint : string
    max_pieces : int
    cases : array<RuStressTokCase>
}

struct RuStressOracleCase {
    file : string
    id : string
    text : string
    form : string
    span_start : int
    span_end : int
    mask : int
    ordinal : int
    n_pieces : int
}

struct RuStressOracleManifest {
    checkpoint : string
    head_seed : int
    cases : array<RuStressOracleCase>
}

def private gguf_path() : string => path_join(models_dir(), RU_STRESS_GGUF)
def private oracle_dir() : string => path_join(models_dir(), "tts_oracle/ru_stress")
def private tokens_path() : string => "{get_das_root()}/modules/dasLLAMA/tests/_tts_fixtures/ru_stress_tokens.json"
def private acute_marked(left, right : string) : string => "{left}{cpt_to_utf8(COMBINING_ACUTE)}{right}"

def private pieces_text(tk : WordPieceTokenizer; ids : array<int>) : string => join([for (id in ids); wordpiece_piece(tk, id)], " ")

def private flat_offsets(pieces : array<WordPiece>) : array<int> {
    var out : array<int>
    out |> reserve(length(pieces) * 2)
    for (p in pieces) {
        out |> push(p.start)
        out |> push(p.end)
    }
    return <- out
}

//! the fixture's sentences of the listed kinds (all of them where `kinds` is empty) whose pieces or spans differ from the reference, the first one logged both ways
def private token_mismatches(tk : WordPieceTokenizer; fx : RuStressTokFixture; kinds : array<string>) : int {
    var bad = 0
    for (c in fx.cases) {
        continue if (!empty(kinds) && find_index(kinds, c.kind) < 0)
        var inscope pieces <- ru_stress_pieces(tk, c.text, fx.max_pieces)
        var inscope ids <- piece_ids(pieces)
        var inscope offs <- flat_offsets(pieces)
        if (mismatch_exact(ids, c.ids) != 0 || mismatch_exact(offs, c.offsets) != 0) {
            if (bad == 0) {
                to_log(LOG_INFO, "ru_stress first mismatch ({c.kind}) '{c.text}'\n  das: {pieces_text(tk, ids)} {offs}\n  ref: {pieces_text(tk, c.ids)} {c.offsets}\n")
            }
            bad++
        }
    }
    return bad
}

def private with_tokenizer(t : T?; blk : block<(var tk : WordPieceTokenizer; fx : RuStressTokFixture) : void>) {
    return if (!model_available(t, gguf_path()))
    var inscope fx : RuStressTokFixture
    if (!load_json_file(tokens_path(), fx)) {
        t |> failure("{tokens_path()} is missing - harness/ru_stress_oracle.py writes it")
        return
    }
    var inscope tk <- load_wordpiece_gguf(gguf_path(), "ru_stress")
    invoke(blk, tk, fx)
}

[test(description = "the WordPiece tokenizer reproduces HuggingFace's ids and codepoint spans on the 226-sentence fixture, accent strip included")]
def test_ru_stress_tokenizer(t : T?) {
    t |> run("every fixture sentence: the same pieces and the same codepoint spans as the HuggingFace tokenizer") @(t : T?) {
        with_tokenizer(t) $(var tk; fx) {
            t |> equal(length(fx.cases), FIXTURE_SENTENCES, "the fixture's sentence count")
            var inscope none : array<string>
            t |> equal(token_mismatches(tk, fx, none), 0, "sentences whose ids or spans differ")
            var inscope pieces <- ru_stress_pieces(tk, fx.cases[0].text, fx.max_pieces)
            to_log(LOG_INFO, "ru_stress '{fx.cases[0].text}' -> {pieces_text(tk, piece_ids(pieces))}\n")
        }
    }
    t |> run("an over-length sentence keeps [CLS], the first 94 pieces and [SEP]") @(t : T?) {
        with_tokenizer(t) $(var tk; fx) {
            var n = 0
            for (c in fx.cases) {
                continue if (c.kind != "over-length")
                var inscope pieces <- ru_stress_pieces(tk, c.text, fx.max_pieces)
                t |> equal(length(pieces), 96, "'{c.text}': the piece count")
                t |> equal(pieces[length(pieces) - 1].id, tk.sep_id, "the last piece is [SEP]")
                n++
            }
            t |> equal(n, 5, "the over-length cases")
        }
    }
    t |> run("the accent strip the config asks for: й reads as и, ё as е, a stress mark as nothing, its span kept") @(t : T?) {
        with_tokenizer(t) $(var tk; fx) {
            let words = fixed_array("йод", "иод", "ёлка", "елка", acute_marked("за", "мок"), "замок")
            for (pair in range(3)) {
                let i = pair * 2
                var inscope a <- piece_ids(ru_stress_pieces(tk, words[i], fx.max_pieces))
                var inscope b <- piece_ids(ru_stress_pieces(tk, words[i + 1], fx.max_pieces))
                t |> equal(mismatch_exact(a, b), 0, "{words[i]} and {words[i + 1]}: {pieces_text(tk, a)} / {pieces_text(tk, b)}")
            }
            var inscope mark <- ru_stress_pieces(tk, acute_marked("за", "мок"), fx.max_pieces)
            t |> equal(mark[length(mark) - 2].end, 6, "the marked word's last piece ends past the mark")
        }
    }
    t |> run("the control: with the rewrite tables emptied the ё, й and marked sentences no longer match") @(t : T?) {
        with_tokenizer(t) $(var tk; fx) {
            delete tk.rewrite_from
            delete tk.rewrite_to
            tk.rewrite_off <- [0]
            tk.strip_accents = false
            for (kind in fixed_array("yo", "short-i", "stress-mark")) {
                let bad = token_mismatches(tk, fx, [kind])
                t |> success(bad > 0, "{kind}: {bad} sentences differ without the strip")
            }
        }
    }
}

struct LaneReading {
    max_abs : float
    sum_abs : double
    n : int64
    picks_same : int
    logit_max_abs : float
}

def private dump_case(c : RuStressOracleCase) : table<string; TtsDumpTensor> => <- load_tts_dump(path_join(oracle_dir(), c.file))

def private lane_bar(rel : float) : Bar => Bar(rel = rel, poison = POISON_SCALED, poison_scaled = true)

def private hidden_bar(q8 : bool) : Bar => lane_bar(q8 ? Q8_BAR : F32_BAR)

def private lane_name(m : RuStressModel) : string => m.q8 ? "q8" : "f32"

def private logits_of(pick : RuStressPick) : array<float> => <- [for (o in range(RU_STRESS_ORDINALS)); pick.logits[o]]

def private oracle_logits(m : RuStressModel; c : RuStressOracleCase; var sc : RuStressScratch) : array<float> =>
    <- logits_of(ru_stress_pick_span(m, int2(c.span_start, c.span_end), uint(c.mask), sc))

[cold_path]
def private lane_cell(t : T?; m : RuStressModel; mf : RuStressOracleManifest; var sc : RuStressScratch) : LaneReading {
    var r = LaneReading()
    let lane = lane_name(m)
    for (c in mf.cases) {
        var inscope dump <- dump_case(c)
        var inscope want <- dump_tensor(dump, 0.0, "hidden")
        var inscope want_ids <- dump_tensor(dump, 0, "ids")
        var inscope want_logits <- dump_tensor(dump, 0.0, "logits")
        let n = ru_stress_encode(m, c.text, sc)
        var inscope ids <- piece_ids(sc.pieces)
        t |> equal(mismatch_exact(ids, want_ids), 0, "{c.id}: the ids are the reference's ({n} against {length(want_ids)})")
        check(t, "{lane} {c.id} hidden", sc.hidden, want, hidden_bar(m.q8), Shape(row_width = int(m.hidden_dim)))
        for (g, w in sc.hidden, want) {
            let d = abs(g - w)
            r.max_abs = is_nan(d) ? FLT_MAX : max(r.max_abs, d)
            r.sum_abs += double(d)
            r.n++
        }
        let pick = ru_stress_pick_span(m, int2(c.span_start, c.span_end), uint(c.mask), sc)
        var inscope got_logits <- oracle_logits(m, c, sc)
        check(t, "{lane} {c.id} logits", got_logits, want_logits, lane_bar(m.q8 ? Q8_LOGIT_BAR : F32_LOGIT_BAR), FLAT_SHAPE)
        for (g, w in got_logits, want_logits) {
            r.logit_max_abs = max(r.logit_max_abs, abs(g - w))
        }
        to_log(LOG_INFO, "{lane} {c.id} '{c.form}': ordinal {pick.ordinal} against {c.ordinal}, logits {got_logits} against {want_logits}\n")
        if (pick.ordinal == c.ordinal) {
            r.picks_same++
        }
    }
    let mean = r.n > 0l ? r.sum_abs / double(r.n) : 0.0lf
    to_log(LOG_INFO, "ru_stress {lane} lane: hidden max abs {r.max_abs}, mean abs {mean}; logits max abs {r.logit_max_abs}; {r.picks_same} of {length(mf.cases)} picks the reference's\n")
    return r
}

//! `m`'s pick for the word of case `c` over the sentence with that word's pieces swapped for [UNK]
[cold_path]
def private unk_word_pick(m : RuStressModel; c : RuStressOracleCase; mask : uint; var sc : RuStressScratch) : RuStressPick {
    ru_stress_encode(m, c.text, sc)
    var inscope ids <- piece_ids(sc.pieces)
    for (p, i in sc.pieces, count()) {
        if (p.end > p.start && p.start < c.span_end && p.end > c.span_start) {
            ids[i] = m.tok.unk_id
        }
    }
    ru_stress_forward(m, ids, sc)
    return ru_stress_pick_span(m, int2(c.span_start, c.span_end), mask, sc)
}

//! the control: the first case with its first word piece swapped for [UNK] lands outside the lane's hidden-state and logit bars
[cold_path]
def private poisoned_input(t : T?; m : RuStressModel; mf : RuStressOracleManifest; var sc : RuStressScratch) {
    let c = mf.cases[0]
    var inscope dump <- dump_case(c)
    var inscope want <- dump_tensor(dump, 0.0, "hidden")
    var inscope want_logits <- dump_tensor(dump, 0.0, "logits")
    ru_stress_encode(m, c.text, sc)
    var inscope ids <- piece_ids(sc.pieces)
    ids[1] = m.tok.unk_id
    ru_stress_forward(m, ids, sc)
    check_exceeds(t, "{lane_name(m)} {c.id} hidden with its first piece as [UNK]", sc.hidden, want, hidden_bar(m.q8))
    var inscope got_logits <- logits_of(unk_word_pick(m, c, uint(c.mask), sc))
    check_exceeds(t, "{lane_name(m)} {c.id} logits with the word's pieces as [UNK]", got_logits, want_logits, lane_bar(m.q8 ? Q8_LOGIT_BAR : F32_LOGIT_BAR))
}

def private with_model(t : T?; q8 : bool; blk : block<(m : RuStressModel; mf : RuStressOracleManifest; var sc : RuStressScratch) : void>) {
    return if (!model_available(t, gguf_path()))
    var inscope mf : RuStressOracleManifest
    if (!load_json_manifest(oracle_dir(), mf)) {
        t |> skip("{oracle_dir()} not present - harness/ru_stress_oracle.py mints it")
        return
    }
    var inscope m <- load_ru_stress(gguf_path(), q8)
    var inscope sc = RuStressScratch()
    invoke(blk, m, mf, sc)
}

[test(description = "the encoder's last hidden state and the head's logits and picks hold PyTorch's on the f32 and the q8 lane")]
def test_ru_stress_encoder(t : T?) {
    with_dasllama_jobque_() {
        t |> run("the f32 lane: 20 sentences' hidden states and logits within the lane's bar, every pick the reference's") @(t : T?) {
            with_model(t, false) $(m, mf; var sc) {
                t |> success(!m.layers[0].ffn.q8, "the layers' GEMMs serve the file's f32 planes")
                t |> equal(length(mf.cases), ORACLE_CASES, "the oracle's case count")
                let r = lane_cell(t, m, mf, sc)
                t |> equal(r.picks_same, length(mf.cases), "every pick the reference's")
                poisoned_input(t, m, mf, sc)
            }
        }
        t |> run("the q8 lane: 20 sentences' hidden states and logits within the lane's bar") @(t : T?) {
            with_model(t, true) $(m, mf; var sc) {
                t |> success(m.layers[0].ffn.q8, "the layers' GEMMs serve Q8_0 quants")
                t |> equal(length(mf.cases), ORACLE_CASES, "the oracle's case count")
                lane_cell(t, m, mf, sc)
                poisoned_input(t, m, mf, sc)
            }
        }
    }
}

[test(description = "the batch form answers every word of a sentence from one pass, bit for bit the single picks; the catalog, the word finder and a pick with no mask")]
def test_ru_stress_api(t : T?) {
    with_dasllama_jobque_() {
        t |> run("the word finder lands on the oracle's span for every case") @(t : T?) {
            with_model(t, false) $(m, mf; var sc) {
                for (c in mf.cases) {
                    t |> equal(ru_stress_find_word(c.text, c.form), int2(c.span_start, c.span_end), "{c.id} '{c.form}'")
                }
                t |> equal(ru_stress_find_word("подзамок замок", "замок"), int2(9, 14), "a form inside a longer word is passed over")
                t |> equal(ru_stress_find_word("дом", "замок"), int2(-1, -1), "an absent form")
            }
        }
        t |> run("the catalog: a listed form's ordinals, nothing for an unlisted one") @(t : T?) {
            with_model(t, false) $(m, mf; var sc) {
                t |> equal(ru_stress_allowed(m, "замок"), 3u, "замок: ordinals 1 and 2")
                t |> equal(ru_stress_allowed(m, "несуществующееслово"), 0u, "an unlisted form")
                for (c in mf.cases) {
                    t |> equal(ru_stress_allowed(m, c.form), uint(c.mask), "{c.id} '{c.form}'")
                }
            }
        }
        t |> run("two words of one sentence from one pass: each pick bit for bit its single pick") @(t : T?) {
            with_model(t, false) $(m, mf; var sc) {
                let text = "Старый замок стоял у самой дороги."
                var inscope spans <- [ru_stress_find_word(text, "замок"), ru_stress_find_word(text, "самой")]
                var inscope masks <- [ru_stress_allowed(m, "замок"), ru_stress_allowed(m, "самой")]
                var inscope both <- ru_stress_pick_words(m, text, spans, masks, sc)
                t |> equal(length(both), 2, "one pick a word")
                for (i in range(2)) {
                    let one = ru_stress_pick(m, text, spans[i], masks[i], sc)
                    t |> equal(both[i].ordinal, one.ordinal, "word {i}: the ordinal")
                    var inscope batch_logits <- logits_of(both[i])
                    var inscope single_logits <- logits_of(one)
                    to_log(LOG_INFO, "word {i}: the one pass {batch_logits} (ordinal {both[i].ordinal}), the single pick {single_logits} (ordinal {one.ordinal})\n")
                    check(t, "word {i}: the logits bit for bit", batch_logits, single_logits, BAR_EXACT, FLAT_SHAPE)
                    t |> success(one.ordinal >= 1 && (masks[i] & (1u << uint(one.ordinal - 1))) != 0u, "word {i}: the pick among the allowed ordinals ({one.ordinal})")
                }
                let none = ru_stress_pick(m, text, int2(40, 50), 0u, sc)
                t |> equal(none.ordinal, 0, "a span no piece covers picks nothing")
            }
        }
        t |> run("a pick with no mask ranks every ordinal: the largest of the eight logits") @(t : T?) {
            with_model(t, false) $(m, mf; var sc) {
                let c = mf.cases[0]
                let span = int2(c.span_start, c.span_end)
                let unmasked = ru_stress_pick(m, c.text, span, 0u, sc)
                var inscope logits <- logits_of(unmasked)
                t |> success(all_finite(logits), "the logits are finite: {logits}")
                t |> equal(unmasked.ordinal, int(argmax_of(logits)) + 1, "no mask: the pick is the largest logit's ordinal")
                let one_bit = 1u << uint((unmasked.ordinal % RU_STRESS_ORDINALS))
                let masked = ru_stress_pick(m, c.text, span, one_bit, sc)
                t |> equal(masked.ordinal, unmasked.ordinal % RU_STRESS_ORDINALS + 1, "the control: a mask of one other ordinal picks that ordinal")
            }
        }
    }
}

let RU_STRESS_Q4 = "ru-stress-rubert-base-q4.gguf"
let RU_STRESS_Q4_REF = "ru-stress-rubert-base-f32.gguf"
let EMB_BAR = 0.05      //! the Q8_0 word rows through the embedding LayerNorm against the f32 rows, of the larger magnitude floored at 1: they read 0.036
let Q4_LOGIT_BAR = 0.12  //! the Q4_K layers' logits of the ordinals the catalog allows against the f32 lane's, of the larger magnitude floored at 1: they read 0.082
let NEAR_TIE = 0.5      //! an f32 margin between the two best allowed ordinals under which the Q4_K pick may differ

def private q4_path() : string => path_join(models_dir(), RU_STRESS_Q4)
def private q4_ref_path() : string => path_join(models_dir(), RU_STRESS_Q4_REF)

def private lane_pair_present(t : T?) : bool => model_available(t, q4_path()) && model_available(t, q4_ref_path())

def private same_pieces(q4, f32 : WordPieceTokenizer; a, b : array<WordPiece>) : bool {
    return false if (length(a) != length(b))
    for (x, y in a, b) {
        return false if (wordpiece_piece(q4, x.id) != wordpiece_piece(f32, y.id) || x.start != y.start || x.end != y.end)
    }
    return true
}

//! whether a lane's pick holds the f32 lane's under the tie rule: the same ordinal, or an f32 margin under NEAR_TIE
def private pick_holds(lane_ordinal, f32_ordinal : int; f32_margin : float) : bool => lane_ordinal == f32_ordinal || f32_margin < NEAR_TIE

def private with_lane_pair(t : T?; blk : block<(q4, f32 : RuStressModel; mf : RuStressOracleManifest; var sq, sf : RuStressScratch) : void>) {
    return if (!lane_pair_present(t))
    var inscope mf : RuStressOracleManifest
    if (!load_json_manifest(oracle_dir(), mf)) {
        t |> skip("{oracle_dir()} not present - harness/ru_stress_oracle.py mints it")
        return
    }
    var inscope q4 <- load_ru_stress(q4_path(), true)
    var inscope f32 <- load_ru_stress(q4_ref_path(), false)
    var inscope sq = RuStressScratch()
    var inscope sf = RuStressScratch()
    invoke(blk, q4, f32, mf, sq, sf)
}

[cold_path]
def private quantized_lane_cells(t : T?) {
    t |> run("the pruned vocabulary spells every fixture sentence in the unpruned pieces and spans, renumbered") @(t : T?) {
        return if (!lane_pair_present(t))
        var inscope fx : RuStressTokFixture
        if (!load_json_file(tokens_path(), fx)) {
            t |> failure("{tokens_path()} is missing - harness/ru_stress_oracle.py writes it")
            return
        }
        var inscope q4 <- load_wordpiece_gguf(q4_path(), "ru_stress")
        var inscope f32 <- load_wordpiece_gguf(q4_ref_path(), "ru_stress")
        var differ = 0
        var renumbered = 0
        for (c in fx.cases) {
            var inscope a <- ru_stress_pieces(q4, c.text, fx.max_pieces)
            var inscope b <- ru_stress_pieces(f32, c.text, fx.max_pieces)
            if (!same_pieces(q4, f32, a, b)) {
                to_log(LOG_INFO, "pruned '{c.text}'\n  q4:  {pieces_text(q4, piece_ids(a))}\n  f32: {pieces_text(f32, piece_ids(b))}\n")
                differ++
            }
            for (x, y in a, b) {
                renumbered += x.id != y.id ? 1 : 0
            }
        }
        t |> equal(differ, 0, "fixture sentences the pruned vocabulary spells apart from the whole one")
        t |> success(wordpiece_count(q4) < wordpiece_count(f32), "the vocabulary is pruned ({wordpiece_count(q4)} of {wordpiece_count(f32)} pieces)")
        t |> success(renumbered > 0, "the control: the pieces carry other ids ({renumbered} renumbered)")
    }
    t |> run("the Q8_0 word table: every oracle sentence's embedding rows within EMB_BAR of the f32 file's") @(t : T?) {
        with_lane_pair(t) $(q4, f32, mf; var sq, sf) {
            for (c in mf.cases) {
                var inscope pa <- ru_stress_pieces(q4.tok, c.text, q4.max_pieces)
                var inscope pb <- ru_stress_pieces(f32.tok, c.text, f32.max_pieces)
                var inscope ia <- piece_ids(pa)
                var inscope ib <- piece_ids(pb)
                ru_stress_embed(q4, ia, sq)
                ru_stress_embed(f32, ib, sf)
                check(t, "{c.id} embeddings", sq.emb, sf.emb, lane_bar(EMB_BAR), Shape(row_width = int(q4.emb_dim)))
            }
            let c = mf.cases[0]
            var inscope pa <- ru_stress_pieces(q4.tok, c.text, q4.max_pieces)
            var inscope shifted <- [for (p in pa); p.end > p.start ? p.id + 1 : p.id]
            var inscope pb <- ru_stress_pieces(f32.tok, c.text, f32.max_pieces)
            ru_stress_embed(q4, shifted, sq)
            ru_stress_embed(f32, piece_ids(pb), sf)
            check_exceeds(t, "{c.id} embeddings with every word piece one table row off", sq.emb, sf.emb, lane_bar(EMB_BAR))
        }
    }
    t |> run("the Q4_K layers: the oracle sentences' logits within Q4_LOGIT_BAR of the f32 lane, the picks the same but under NEAR_TIE") @(t : T?) {
        with_lane_pair(t) $(q4, f32, mf; var sq, sf) {
            t |> success(q4.layers[0].ffn.kq_fmt != 0 && q4.words.form == RuStressTableForm.q8_0, "the file serves K-quant layers and a Q8_0 table")
            t |> equal(length(mf.cases), ORACLE_CASES, "the oracle's case count")
            var outside_tie_rule = 0
            for (c in mf.cases) {
                let span = int2(c.span_start, c.span_end)
                let mask = ru_stress_allowed(q4, c.form)
                let a = ru_stress_pick(q4, c.text, span, mask, sq)
                let b = ru_stress_pick(f32, c.text, span, mask, sf)
                var inscope la <- [for (o in range(RU_STRESS_ORDINALS)); a.logits[o]; where(mask & (1u << uint(o))) != 0u]
                var inscope lb <- [for (o in range(RU_STRESS_ORDINALS)); b.logits[o]; where(mask & (1u << uint(o))) != 0u]
                check(t, "{c.id} allowed logits", la, lb, lane_bar(Q4_LOGIT_BAR), FLAT_SHAPE)
                t |> success(all_finite(lb), "{c.id}: the f32 lane's allowed logits are finite")
                let margin = top2_margin(lb, long_length(lb))
                to_log(LOG_INFO, "{c.id} '{c.form}': q4 {a.ordinal} f32 {b.ordinal}, f32 margin {margin}\n")
                t |> success(pick_holds(a.ordinal, b.ordinal, margin), "{c.id} '{c.form}': the Q4_K pick {a.ordinal}, the f32 pick {b.ordinal} at a margin of {margin}")
                let poisoned = unk_word_pick(q4, c, mask, sq)
                outside_tie_rule += pick_holds(poisoned.ordinal, b.ordinal, margin) ? 0 : 1
            }
            t |> success(outside_tie_rule > 0, "the control: with the word's pieces as [UNK], {outside_tie_rule} of {length(mf.cases)} picks leave the f32 pick at a margin past NEAR_TIE")
            let c = mf.cases[0]
            let poisoned = unk_word_pick(q4, c, ru_stress_allowed(q4, c.form), sq)
            let clean = ru_stress_pick(f32, c.text, int2(c.span_start, c.span_end), ru_stress_allowed(q4, c.form), sf)
            var inscope lp <- logits_of(poisoned)
            var inscope lc <- logits_of(clean)
            check_exceeds(t, "{c.id} logits with the word's pieces as [UNK]", lp, lc, lane_bar(Q4_LOGIT_BAR))
        }
    }
}

[test(description = "the shipped lane - Q8_0 word rows, a pruned vocabulary, Q4_K layers - holds the same model's f32 file")]
def test_ru_stress_quantized_lane(t : T?) {
    with_dasllama_jobque_() {
        quantized_lane_cells(t)
    }
}

let RU_STRESS_ELECTRA = "ru-stress-ruelectra-small-q4.gguf"

[test(description = "the ELECTRA lane: the small model's embeddings pass its 128-to-256 projection, one pass answers a sentence's words bit for bit the single picks, and a homograph reads both ways by its sentence")]
def test_ru_stress_electra(t : T?) {
    with_dasllama_jobque_() {
        t |> run("the embedding projection, one pass against single picks, a homograph pair") @(t : T?) {
            let path = path_join(models_dir(), RU_STRESS_ELECTRA)
            return if (!model_available(t, path))
            var inscope m <- load_ru_stress(path, true)
            var inscope sc = RuStressScratch()
            t |> success(m.map_in.nout == m.hidden_dim && m.map_in.nin == m.emb_dim && m.emb_dim < m.hidden_dim, "the model projects its {m.emb_dim}-wide embeddings to {m.hidden_dim}")
            let text = "Старый замок стоял у самой дороги."
            var inscope spans <- [ru_stress_find_word(text, "замок"), ru_stress_find_word(text, "самой")]
            var inscope masks <- [ru_stress_allowed(m, "замок"), ru_stress_allowed(m, "самой")]
            t |> success(masks[0] != 0u && masks[1] != 0u, "both words are catalog forms")
            var inscope both <- ru_stress_pick_words(m, text, spans, masks, sc)
            for (i in range(2)) {
                let one = ru_stress_pick(m, text, spans[i], masks[i], sc)
                var inscope batch_logits <- logits_of(both[i])
                var inscope single_logits <- logits_of(one)
                t |> success(all_finite(batch_logits), "word {i}: the logits are finite: {batch_logits}")
                t |> equal(both[i].ordinal, one.ordinal, "word {i}: the ordinal")
                check(t, "word {i}: the logits bit for bit", batch_logits, single_logits, BAR_EXACT, FLAT_SHAPE)
            }
            var inscope cases <- context_pairs()
            for (i in range(2)) {
                let text_i = cases[i].text
                let word = cases[i].word
                let pick = ru_stress_pick(m, text_i, ru_stress_find_word(text_i, word), ru_stress_allowed(m, word), sc)
                var inscope logits <- logits_of(pick)
                to_log(LOG_INFO, "electra '{text_i}': ordinal {pick.ordinal}, logits {logits}\n")
                t |> equal(cases[i].marked, i == 0 ? "з+амок" : "зам+ок", "the pair's two readings")
                t |> equal(pick.ordinal, i + 1, "'{text_i}' reads {cases[i].marked}")
            }
        }
    }
}
