options gen2
options _dasllama_internal = true
options _ascii_strings = false   // the Russian words and their marks are the claims
options stack = 524288
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_g2p_ru
require dasllama/dasllama_g2p_pack
require dasllama/dasllama_tts   // a directory loaded as a front end with no voice
require dasllama/dasllama_unicode
require dasllama/dasllama_gguf   // gguf_put_le: a pack's little-endian words
require strings
require daslib/strings_boost
require daslib/fio
require math
require _scratch_dir
require _compares

// The Russian stress pack and the tiers that need no stocked file, on packs a test-side writer builds:
// the load refusals, the front-coded tables, the lookups that allocate nothing, and the model-less tiers -
// an abbreviation's last vowel, the ending table, the ё a preposition decides, the rule tier.

def private load_and_drop(path : string) {
    var inscope p <- load_ru_stress_pack(path)
}

def private put_u32(var b : array<uint8>; v : int) {
    gguf_put_le(b, uint64(uint(v)), 4)
}

//! a pack's header - the four letters of its magic, its version, an empty source note
def private pack_header(magic : string; version : int) : array<uint8> {
    var bytes <- to_bytes(magic)
    put_u32(bytes, version)
    bytes |> push(uint8(0))
    bytes |> push(uint8(0))
    return <- bytes
}

[test(description = "`load_ru_stress_pack` panics with a message naming the cause for a missing file, a foreign version and a version 1 pack (pointing at the build script), another pack's magic and a section that runs past the file")]
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
        var inscope bytes <- pack_header("TGRU", 99)
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
        var inscope bytes <- pack_header("TG2P", 1)
        fopen(path, "wb") $(f) {
            fwrite(f, bytes)
        }
        let why = panic_text() {
            load_and_drop(path)
        }
        t |> success(find(why, "bad magic") >= 0 && find(why, "tts_g2p_ru.bin") >= 0, why)
    }
    t |> run("a version 1 pack is refused by name; a version 2 pack of two empty tables loads") @(t : T?) {
        let old = scratch_path("tts_g2p_ru_v1.bin")
        var inscope v1 <- pack_header("TGRU", 1)
        v1 |> resize(64)
        fopen(old, "wb") $(f) {
            fwrite(f, v1)
        }
        let why = panic_text() {
            load_and_drop(old)
        }
        t |> success(find(why, "version 1 pack") >= 0 && find(why, "reads version 2") >= 0 && find(why, "build_g2p_ru_data.py") >= 0, why)
        var inscope none : SynthPack
        let empty_pack = write_synth_pack("tts_g2p_ru_v2_empty.bin", none, 16)
        let loaded = panic_text() {
            load_and_drop(empty_pack)
        }
        t |> equal(loaded, "", "the control: the same header at version 2 loads")
    }
    t |> run("a section whose byte count runs past the file is refused; a section of an unknown tag is passed over") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "вода" => [2] }
        var inscope bytes <- synth_pack_bytes(s, 16)
        put_u32(bytes, UNKNOWN_TAG)
        put_u32(bytes, 4)
        put_u32(bytes, 0)
        let skipped = write_bytes("tts_g2p_ru_unknown_section.bin", bytes)
        var inscope p <- load_ru_stress_pack(skipped)
        t |> equal(ru_pack_default(p, "вода"), 2, "the tables ahead of an unknown section read as they did")
        put_u32(bytes, SECTION_ENDINGS)
        put_u32(bytes, 4096)
        let torn = write_bytes("tts_g2p_ru_torn_section.bin", bytes)
        let why = panic_text() {
            load_and_drop(torn)
        }
        t |> success(find(why, "runs past the file's end") >= 0 && find(why, "build_g2p_ru_data.py") >= 0, why)
    }
}

let CYR_SMALL_A = 1072
let CYR_SMALL_E = 1077
let CYR_SMALL_YO = 1105
let SYNTH_VOWELS = "аеёиоуыэюя"
let SECTION_ENDINGS = int(0x53444E45)
let SECTION_RULES = int(0x534C5552)
let UNKNOWN_TAG = int(0x3F3F3F3F)
let RULE_SLOT_BITS = 3
let SLOT_TWO_BACK = 1
let SLOT_ONE_BACK = 2
let SLOT_ONE_AHEAD = 3
let VERDICT_SIDE_1 = 1
let VERDICT_SURE = 2
let KEY_HYPHEN = 1
let KEY_A = 2
let KEY_YO = 34
let KEY_IMAGE_BASE = 64

//! One rule of a synthetic rule list: the feature's kind, the neighbour's slot, the verdict byte and the key
struct SynthRule {
    kind : RuRuleKind
    slot : int
    verdict : int
    key : string
}

//! A pack as plain tables: a form's ordinals, a е spelling's ё spelling, an ending's position from the word's end, a rule form's family, the ordinal of either side and its unruled side, and every family's rules
struct SynthPack {
    stress : table<string; array<int>>
    yo : table<string; string>
    endings : table<string; int>
    rule_forms : table<string; array<int>>
    rule_lists : array<array<SynthRule>>
}

def private pack_letter_code(cp : int) : int => cp == '-' ? KEY_HYPHEN : (cp == CYR_SMALL_YO ? KEY_YO : cp - CYR_SMALL_A + KEY_A)

//! a key as ASCII letters in the pack's key order
def private key_image(word : string) : string {
    var inscope cps <- utf8_to_cpts(word)
    return build_string() $(var wr) {
        for (cp in cps) {
            wr |> write_char(KEY_IMAGE_BASE + pack_letter_code(cp))
        }
    }
}

//! one front-coded table appended to `out`; `runs` maps a key to the 7-bit groups of its value
def private put_front_table(var out : array<uint8>; runs : table<string; array<int>>; block_keys : int) {
    var inscope images <- [for (word in keys(runs)); key_image(word)]
    var inscope word_of <- {for (word in keys(runs)); key_image(word) => word}
    sort(images)
    var inscope blob : array<uint8>
    var inscope offsets : array<int>
    var inscope prev : array<uint8>
    var key_max = 0
    for (image, i in images, count()) {
        var inscope key <- to_bytes(image)
        var prefix_len = 0
        if (i % block_keys == 0) {
            offsets |> push(length(blob))
        } else {
            while (prefix_len < length(prev) && prefix_len < length(key) && prev[prefix_len] == key[prefix_len]) {
                prefix_len++
            }
        }
        let suffix_len = length(key) - prefix_len
        key_max = max(key_max, length(key))
        blob |> push(uint8((min(prefix_len, FRONT_LEN_ESCAPE) << FRONT_LEN_BITS) | min(suffix_len, FRONT_LEN_ESCAPE)))
        if (prefix_len >= FRONT_LEN_ESCAPE) {
            blob |> push(uint8(prefix_len))
        }
        if (suffix_len >= FRONT_LEN_ESCAPE) {
            blob |> push(uint8(suffix_len))
        }
        blob |> reserve(length(blob) + suffix_len)
        for (k in range(prefix_len, length(key))) {
            blob |> push(uint8(int(key[k]) - KEY_IMAGE_BASE))
        }
        get(runs, word_of?[image] ?? "") $(run) {
            for (g, k in run, count()) {
                blob |> push(uint8(g | (k + 1 < length(run) ? FRONT_VALUE_MORE : 0)))
            }
        }
        prev := key
    }
    put_u32(out, length(images))
    put_u32(out, block_keys)
    put_u32(out, length(offsets))
    put_u32(out, key_max)
    put_u32(out, length(blob))
    for (off in offsets) {
        put_u32(out, off)
    }
    out |> push_from(blob)
}

//! the ё spelling as the 7-bit groups of a mask over the е letters of its key
def private yo_groups(plain, word : string) : array<int> {
    var inscope a <- utf8_to_cpts(plain)
    var inscope b <- utf8_to_cpts(word)
    var mask = 0
    var k = 0
    for (x, y in a, b) {
        if (x == CYR_SMALL_E) {
            mask |= y == CYR_SMALL_YO ? 1 << k : 0
            k++
        }
    }
    var groups <- [mask & (FRONT_VALUE_MORE - 1)]
    while ((mask >> FRONT_VALUE_BITS) != 0) {
        mask = mask >> FRONT_VALUE_BITS
        groups |> push(mask & (FRONT_VALUE_MORE - 1))
    }
    return <- groups
}

def private put_section(var out : array<uint8>; tag : int; body : array<uint8>) {
    put_u32(out, tag)
    put_u32(out, length(body))
    out |> push_from(body)
}

def private reversed_word(word : string) : string {
    var inscope cps <- utf8_to_cpts(word)
    let n = length(cps)
    var inscope back <- [for (i in range(n)); cps[n - 1 - i]]
    return cpts_to_utf8(back)
}

//! the rule tier's section body: the form table, the family count, where each family's rules start, the rules
def private rules_body(s : SynthPack; block_keys : int) : array<uint8> {
    var body : array<uint8>
    put_front_table(body, s.rule_forms, block_keys)
    put_u32(body, length(s.rule_lists))
    var inscope rules : array<uint8>
    for (list in s.rule_lists) {
        put_u32(body, length(rules))
        for (r in list) {
            var inscope cps <- utf8_to_cpts(r.key)
            rules |> reserve(length(rules) + length(cps) + 3)
            rules |> push(uint8((int(r.kind) << RULE_SLOT_BITS) | r.slot))
            rules |> push(uint8(r.verdict))
            rules |> push(uint8(length(cps)))
            for (cp in cps) {
                rules |> push(uint8(pack_letter_code(cp)))
            }
        }
    }
    put_u32(body, length(rules))
    body |> push_from(rules)
    return <- body
}

//! a version 2 pack's bytes: the two tables, `block_keys` keys a block, then the ending section and the rule section where `s` holds any
def private synth_pack_bytes(s : SynthPack; block_keys : int) : array<uint8> {
    var bytes <- pack_header("TGRU", 2)
    put_front_table(bytes, s.stress, block_keys)
    var inscope masks : table<string; array<int>>
    for (plain, word in keys(s.yo), values(s.yo)) {
        masks[plain] <- yo_groups(plain, word)
    }
    put_front_table(bytes, masks, block_keys)
    if (!empty(s.endings)) {
        var inscope runs : table<string; array<int>>
        for (ending, from_end in keys(s.endings), values(s.endings)) {
            runs[reversed_word(ending)] <- [from_end]
        }
        var inscope body : array<uint8>
        put_front_table(body, runs, block_keys)
        put_section(bytes, SECTION_ENDINGS, body)
    }
    if (!empty(s.rule_forms)) {
        var inscope body <- rules_body(s, block_keys)
        put_section(bytes, SECTION_RULES, body)
    }
    return <- bytes
}

def private write_bytes(name : string; bytes : array<uint8>) : string {
    let path = scratch_path(name)
    fopen(path, "wb") $(f) {
        fwrite(f, bytes)
    }
    return path
}

//! a synthetic pack written under the scratch directory; answers its path
def private write_synth_pack(name : string; s : SynthPack; block_keys : int) : string {
    var inscope bytes <- synth_pack_bytes(s, block_keys)
    return write_bytes(name, bytes)
}

def private synth_vowels(word : string) : int {
    var inscope cps <- utf8_to_cpts(word)
    var n = 0
    for (cp in cps) {
        n += find(SYNTH_VOWELS, cpt_to_utf8(cp)) >= 0 ? 1 : 0
    }
    return n
}

//! `word` with the е of ordinal `k` (0-based) spelled ё
def private with_yo(word : string; k : int) : string {
    var inscope cps <- utf8_to_cpts(word)
    var seen = 0
    for (cp in cps) {
        if (cp == CYR_SMALL_E) {
            cp = seen == k ? CYR_SMALL_YO : cp
            seen++
        }
    }
    return cpts_to_utf8(cps)
}

//! 51 stems by 60 endings: every form with a vowel takes an ordinal by formula, every fifth a second one, and every third form with an е a ё spelling
def private synth_tables() : SynthPack {
    var s : SynthPack
    var inscope stems <- split("а аб абажур баб вод вода город дом ел ела елк жен зим зима игр кто- кто-нибуд лес лет мол молок мор нос окн пол пере перевод переселенческ рек рук сел сестр стол трав ум улиц фонар хлеб цвет час что- шум щук эхо юг юго- яблок северо-запад высокопревосходительств достопримечательност из- по-русск", " ")
    var inscope endings <- split("а ам ами ах е ев ее ей ем еми ени енем и ие ий им ими их ию ия ка ки ко ку о ов ого ое ой ом ому ою у ую ы ые ый ым ыми ых ь ью я ям ями ях енка енке енки енку еночек ечка ечке ечки ечку ешенька ешеньке ешеньки ешеньку ество естве еству", " ")
    endings |> push("")
    for (stem, i in stems, count()) {
        for (ending, j in endings, count()) {
            let form = "{stem}{ending}"
            let v = synth_vowels(form)
            continue if (v == 0 || key_exists(s.stress, form) || ends_with(form, "-"))
            let first = 1 + (i * 7 + j * 3) % v
            var ordinals <- [first]
            if (v >= 2 && (i + j) % 5 == 0) {
                ordinals |> push(1 + first % v)
            }
            s.stress[form] <- ordinals
            let plain_e = count(form, "е")
            if (plain_e > 0 && (i + j) % 3 == 0) {
                s.yo[form] = with_yo(form, (i + j) % plain_e)
            }
        }
    }
    return <- s
}

//! the count of `want`'s forms whose ordinals the pack reads otherwise, plus the probes beside them - a form less its last letter, with one more letter, with a hyphen - that the pack answers though `want` lacks them
def private stress_mismatches(p : RuStressPack; want : table<string; array<int>>) : int {
    var bad = 0
    for (form, ordinals in keys(want), values(want)) {
        var inscope got <- ru_stress_variants(p, form)
        bad += mismatch_exact(got, ordinals) != 0 ? 1 : 0
        var inscope cps <- utf8_to_cpts(form)
        for (probe in [cpts_to_utf8(cps, 0, length(cps) - 1), "{form}я", "{form}-"]) {
            continue if (key_exists(want, probe))
            var inscope none <- ru_stress_variants(p, probe)
            bad += empty(none) ? 0 : 1
        }
    }
    return bad
}

def private yo_mismatches(p : RuStressPack; s : SynthPack) : int {
    var bad = 0
    for (form in keys(s.stress)) {
        bad += ru_yo_form(p, form) == (s.yo?[form] ?? "") ? 0 : 1
    }
    return bad
}

def private variants_are(t : T?; p : RuStressPack; word : string; want : array<int>; what : string) {
    var inscope got <- ru_stress_variants(p, word)
    t |> equal(mismatch_exact(got, want), 0, "{what}: '{word}' reads {got} against {want}")
}

[test(description = "the stress pack's front-coded tables answer every key of synthetic version 2 packs - first, last and block-edge keys, a form with two ordinals in its order, a ё mask of two bytes - and no key beside them, at 16, 3 and 1 keys a block")]
def test_pack_front_table(t : T?) {
    t |> run("named keys of a seven-key pack at four keys a block") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "аб" => [1], "абажур" => [3], "абажура" => [3, 1], "абажуры" => [2], "вода" => [2, 1], "что-нибудь" => [1], "я" => [1] }
        s.yo <- { "елка" => "ёлка", "все-таки" => "всё-таки", "бебебебебебебебебе" => "бёбебебебебебебебё" }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_named.bin", s, 4))
        variants_are(t, p, "аб", [1], "the first key")
        variants_are(t, p, "я", [1], "the last key")
        variants_are(t, p, "абажуры", [2], "the last key of the first block")
        variants_are(t, p, "вода", [2, 1], "the first key of the second block, its default first")
        variants_are(t, p, "абажура", [3, 1], "two ordinals in the pack's order")
        variants_are(t, p, "что-нибудь", [1], "a hyphenated key")
        var inscope swapped <- ru_stress_variants(p, "вода")
        t |> success(mismatch_exact(swapped, [1, 2]) != 0, "the control: the other order is another answer")
        for (absent in fixed_array("абажурам", "аба", "а", "яя", "абажу", "водаа", "что", "Вода", "abc", "")) {
            var inscope none <- ru_stress_variants(p, absent)
            t |> equal(length(none), 0, "'{absent}' is no key")
        }
        t |> equal(ru_yo_form(p, "елка"), "ёлка")
        t |> equal(ru_yo_form(p, "все-таки"), "всё-таки", "the second е stays")
        t |> equal(ru_yo_form(p, "бебебебебебебебебе"), "бёбебебебебебебебё", "the ninth е sits in the mask's second byte")
        t |> equal(ru_yo_form(p, "вода"), "", "a stress key is no ё key")
        t |> equal(ru_yo_form(p, "елк"), "")
    }
    t |> run("a few thousand forms against the plain tables they were packed from") @(t : T?) {
        var inscope s <- synth_tables()
        t |> success(length(s.stress) >= 2500 && length(s.yo) >= 200, "{length(s.stress)} forms, {length(s.yo)} ё spellings")
        for (block_keys in fixed_array(16, 3, 1)) {
            var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_{block_keys}.bin", s, block_keys))
            t |> equal(stress_mismatches(p, s.stress), 0, "{block_keys} keys a block: forms read otherwise, or probes answered")
            t |> equal(yo_mismatches(p, s), 0, "{block_keys} keys a block: ё spellings read otherwise")
        }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_control.bin", s, 16))
        s.stress["абажура"] |> push(7)
        var restored = ""
        for (plain in keys(s.yo)) {
            restored = plain
            break
        }
        s.yo[restored] = restored
        t |> equal(stress_mismatches(p, s.stress), 1, "the control: one form's ordinals changed in the plain table is one mismatch")
        t |> equal(yo_mismatches(p, s), 1, "the control: one ё spelling changed is one mismatch")
    }
}

//! the rule list the rule cells share: a word two back decides side 0 for sure, an ending one back decides side 1, a sentence edge one back decides side 0 for sure
def private wall_rules() : array<SynthRule> {
    return <- [
        SynthRule(kind = RuRuleKind.word, slot = SLOT_TWO_BACK, verdict = VERDICT_SURE, key = "из"),
        SynthRule(kind = RuRuleKind.ending, slot = SLOT_ONE_BACK, verdict = VERDICT_SIDE_1, key = "ые"),
        SynthRule(kind = RuRuleKind.edge, slot = SLOT_ONE_BACK, verdict = VERDICT_SURE, key = "")
    ]
}

[test(description = "a stress pack lookup that returns no container allocates nothing: the default ordinal of present, absent and reflexive words, a reading's presence, an absent ё spelling, the ending table's ordinal, the case-ending stem's and the rule tier's answer leave both heaps' allocation counts where they were over thousands of words, and the calls that return an array or a string move theirs")]
def test_pack_lookup_allocates_nothing(t : T?) {
    t |> run("some 2,900 forms, each looked up eight ways inside one allocation bracket") @(t : T?) {
        var inscope s <- synth_tables()
        s.endings <- { "ами" => 2, "ов" => 1, "ешенька" => 3, "ия" => 2 }
        s.rule_forms <- { "вода" => [1, 2, 1, 1], "города" => [1, 3, 1, 2] }
        s.rule_lists |> emplace(wall_rules())
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_alloc.bin", s, 16))
        var inscope words <- [for (w in keys(s.stress)); w]
        var inscope absent <- [for (w in words); "{w}-"]
        var inscope reflexive <- [for (w in words); "{w}ся"]
        var inscope one_sentence <- [for (_w in words); 0]
        var restored = ""
        for (plain in keys(s.yo)) {
            restored = plain
            break
        }
        var held = 0
        var through_base = 0
        var misses = 0
        var by_ending = 0
        var by_stem = 0
        var by_rule = 0
        let heap0 = heap_allocation_count()
        let strings0 = string_heap_allocation_count()
        for (w, a, r, k in words, absent, reflexive, count()) {
            held += ru_pack_default(p, w) > 0 ? 1 : 0
            held += ru_stress_pack_has(p, w, ru_pack_default(p, w)) ? 1 : 0
            through_base += ru_pack_default(p, r) > 0 ? 1 : 0
            misses += ru_pack_default(p, a) == 0 ? 1 : 0
            misses += empty(ru_yo_form(p, a)) ? 1 : 0
            by_ending += ru_ending_ordinal(p, w) > 0 ? 1 : 0
            by_stem += ru_stem_pick(p, w) > 0 ? 1 : 0
            by_rule += ru_stress_rule(p, words, one_sentence, k) > 0 ? 1 : 0
        }
        let heap1 = heap_allocation_count()
        let strings1 = string_heap_allocation_count()
        t |> equal(held, 2 * length(words), "every form's default read and found among its readings")
        t |> equal(through_base, length(words), "every form with -ся read through its base, or as a form of its own")
        t |> equal(misses, 2 * length(words), "no word with a trailing hyphen is a key of either table")
        t |> success(by_ending >= 100, "the ending table answered {by_ending} of the words")
        t |> success(by_stem >= 100, "the case-ending stem answered {by_stem} of the words")
        t |> equal(by_rule, 2, "the rule tier walked its list for its two forms and answered each, and abstained on every other word")
        t |> equal(heap1 - heap0, 0ul, "heap allocations over {8 * length(words)} lookups")
        t |> equal(strings1 - strings0, 0ul, "string heap allocations over the same lookups")
        var inscope variants <- ru_stress_variants(p, words[0])
        t |> success(heap_allocation_count() > heap1, "the control: the call that returns the ordinals array allocates it")
        let spelled = ru_yo_form(p, restored)
        t |> success(string_heap_allocation_count() > strings1 && !empty(spelled), "the control: the call that returns a ё spelling allocates the string")
    }
}

[test(description = "a word the pack does not hold, written in capitals and closed by a consonant, takes its stress on the last vowel at two or three vowels; a pack entry, a final vowel, a small letter and a fourth vowel each leave it to the tiers around the rule")]
def test_abbreviation_last_vowel(t : T?) {
    t |> run("abbreviations by the rule, and the words it leaves alone") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "нато" => [1], "вода" => [2] }
        s.endings <- { "ом" => 2 }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_abbreviation.bin", s, 16))
        t |> equal(ru_mark_word(p, "вциом"), "вц+иом", "the control: in small letters the ending table reads the word, the vowel before the last")
        let by_rule <- { "ООН" => "О+ОН", "АЭС" => "А+ЭС", "ВЦИОМ" => "ВЦИ+ОМ", "ГЛОНАСС" => "ГЛОН+АСС", "УФСИН" => "УФС+ИН", "УНИАН" => "УНИ+АН" }
        for (word, want in keys(by_rule), values(by_rule)) {
            t |> equal(ru_mark_word(p, word), want, word)
        }
        t |> equal(ru_mark_word(p, "НАТО"), "Н+АТО", "the control: a pack entry wins")
        t |> equal(ru_mark_word(p, "ВОДА"), "ВОД+А", "a word in capitals the pack holds reads as the pack reads it")
        t |> equal(ru_mark_word(p, "РИА"), "Р+ИА", "a final vowel: the shape rule")
        t |> equal(ru_mark_word(p, "ОАО"), "О+АО", "every letter a vowel: the shape rule")
        t |> equal(ru_mark_word(p, "оон"), "+оон", "small letters: the shape rule")
        t |> equal(ru_mark_word(p, "Оон"), "+Оон", "one capital: the shape rule")
        t |> equal(ru_mark_word(p, "АВТОВАЗБАНК"), "АВТОВ+АЗБАНК", "four vowels: the shape rule")
        t |> equal(ru_mark_word(p, "МИД"), "М+ИД", "one vowel is its own stress")
        t |> equal(ru_mark_stress(p, "в ООН и НАТО"), "в О+ОН и Н+АТО", "in running text")
    }
}

[test(description = "the ending table reads a word the pack does not hold by its longest held ending, counts the stressed vowel from the word's end, passes over an ending that names a vowel the word lacks or is the whole word, and stands behind a pack entry; without the section the shape rule answers")]
def test_ending_table(t : T?) {
    t |> run("longest ending first, the position from the end, the fall to the shape rule") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "станция" => [1] }
        var inscope bare <- load_ru_stress_pack(write_synth_pack("synth_ru_no_endings.bin", s, 16))
        s.endings <- { "ация" => 3, "ия" => 2, "ость" => 2, "овать" => 1, "ировать" => 3 }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_endings.bin", s, 4))
        let cases <- {
            "фывация" => "фыв+ация", "кардия" => "кард+ия", "рисовать" => "рисов+ать", "планировать" => "план+ировать",
            "радость" => "р+адость", "Фывация" => "Фыв+ация"
        }
        for (word, want in keys(cases), values(cases)) {
            t |> equal(ru_mark_word(p, word), want, word)
        }
        t |> equal(ru_ending_ordinal(p, "планировать"), 2, "-ировать outranks the -овать it ends in")
        t |> equal(ru_ending_ordinal(p, "рисовать"), 3, "the control: -овать alone reads the last vowel")
        t |> equal(ru_ending_ordinal(p, "ация"), 2, "an ending that is the whole word is passed over: -ия reads it")
        t |> equal(ru_ending_ordinal(p, "жость"), 0, "-ость names the vowel before its own, and the word has one vowel")
        t |> equal(ru_mark_word(p, "жость"), "ж+ость", "so the shape rule reads it")
        t |> equal(ru_ending_ordinal(p, "фывапролдж"), 0, "no ending of the word is held")
        t |> equal(ru_mark_word(p, "фывапролдж"), "фыв+апролдж", "the shape rule: the vowel before the last")
        t |> equal(ru_mark_word(p, "станция"), "ст+анция", "a pack entry wins over -ия")
        t |> equal(ru_ending_ordinal(bare, "фывация"), 0, "the control: a pack with no ending section holds no ending")
        t |> equal(ru_mark_word(bare, "фывация"), "фывац+ия", "and reads the word by the shape rule")
    }
}

[test(description = "a word the pack does not hold takes the stressed vowel of the form it is a case of - the word less a case ending, held with one reading and two vowels or more - ahead of the ending table; an absent stem, a stem of one vowel, a stem of two readings and an ending outside the list each leave it to the tiers after")]
def test_case_ending_stem(t : T?) {
    t |> run("a name in its oblique cases, and the controls") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "федерер" => [1], "дуб" => [1], "замок" => [1, 2] }
        s.endings <- { "еру" => 1 }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_stem.bin", s, 16))
        let cases <- { "Федереру" => "Ф+едереру", "федерере" => "ф+едерере", "федереры" => "ф+едереры", "федерери" => "ф+едерери", "федерерими" => "ф+едерерими" }
        for (word, want in keys(cases), values(cases)) {
            t |> equal(ru_mark_word(p, word), want, word)
        }
        t |> equal(ru_stem_pick(p, "федереру"), 1, "the stem's vowel, counted from the word's start")
        t |> equal(ru_ending_ordinal(p, "федереру"), 4, "the control: the ending table alone reads the last vowel")
        t |> equal(ru_stem_pick(p, "фывапролджу"), 0, "no such stem in the pack")
        t |> equal(ru_stem_pick(p, "дубы"), 0, "a stem of one vowel is not read through")
        t |> equal(ru_stem_pick(p, "замоку"), 0, "nor a stem the pack reads two ways")
        t |> equal(ru_stem_pick(p, "федерера"), 0, "nor an ending outside the list")
        t |> equal(ru_mark_word(p, "федерера"), "федер+ера", "which the shape rule reads")
    }
    t |> run("a stem the ё table restores") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "дело" => [1] }
        s.yo <- {
            "королев" => "королёв", "лев" => "лёв", "пелен" => "пелён", "шевелев" => "шёвелев", "колерв" => "колёрв",
            "теререререререлев" => "теререререререлёв"
        }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_yo_stem.bin", s, 16))
        t |> equal(ru_stem_pick(p, "королеву"), RU_PICK_YO + 3, "the stem's ё, as a ё pick on its third vowel")
        t |> equal(ru_mark_word(p, "Королеву"), "Королёву", "the word takes the ё")
        t |> equal(ru_mark_word(p, "КОРОЛЕВО"), "КОРОЛЁВО", "in the letter's own case")
        t |> equal(ru_stem_pick(p, "леву"), 0, "the control: a stem of one vowel is not read through")
        t |> equal(ru_stem_pick(p, "пелено"), 0, "the control: a restored stem that does not end in -ёв")
        t |> equal(ru_stem_pick(p, "шевелеву"), 0, "the control: a stem in -ев whose ё is another vowel")
        t |> equal(ru_stem_pick(p, "королева"), 0, "the control: an ending outside the list")
        t |> equal(ru_stem_pick(p, "колерву"), 0, "the control: a restored stem in -в whose letter before it is no е")
        t |> equal(ru_stem_pick(p, "федереву"), 0, "the control: a stem in -ев neither table holds")
        t |> equal(ru_stem_pick(p, "теререререререлеву"), RU_PICK_YO + 8, "a ё past the mask's first byte: the stem's eighth е")
        t |> equal(ru_mark_word(p, "теререререререлеву"), "теререререререлёву")
        t |> equal(ru_yo_form(p, "теререререререлев"), "теререререререлёв", "and the ё table restores the stem itself")
    }
}

[test(description = "a short patronymic the pack does not hold takes the stressed vowel of its full form - -еич read as -еевич, -ыч as -ович, a case ending of two letters at most kept - ahead of the ending table; a full form the pack lacks, reads two ways or stresses in the suffix, a longer case ending and a word of another ending each leave it to the tiers after")]
def test_short_patronymic(t : T?) {
    t |> run("short patronymics in three cases, and the controls") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- {
            "андреевич" => [2], "андреевичу" => [2], "егорович" => [2], "егоровичем" => [2], "егоровичами" => [2],
            "матвеевич" => [1, 2], "фомович" => [2], "лукеевич" => [3]
        }
        s.endings <- { "еич" => 3, "еичу" => 4, "ыч" => 1, "ычем" => 2 }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_patronymic.bin", s, 16))
        let cases <- { "Андреич" => "Андр+еич", "андреичу" => "андр+еичу", "Егорыч" => "Ег+орыч", "егорычем" => "ег+орычем" }
        for (word, want in keys(cases), values(cases)) {
            t |> equal(ru_mark_word(p, word), want, word)
        }
        t |> equal(ru_patronymic_ordinal(p, "андреич"), 2, "the full form's vowel, counted from the word's start")
        t |> equal(ru_ending_ordinal(p, "андреич"), 1, "the control: the ending table alone reads the first vowel")
        t |> equal(ru_patronymic_ordinal(p, "сергеич"), 0, "no full form in the pack")
        t |> equal(ru_patronymic_ordinal(p, "матвеич"), 0, "nor a full form the pack reads two ways")
        t |> equal(ru_patronymic_ordinal(p, "фомыч"), 0, "nor a full form stressed in its suffix")
        t |> equal(ru_patronymic_ordinal(p, "лукеич"), 0, "the same for -еич: the suffix's own vowel")
        t |> equal(ru_patronymic_ordinal(p, "егорычами"), 0, "nor a case ending of three letters")
        t |> equal(ru_patronymic_ordinal(p, "егорович"), 0, "nor a word that is no short patronymic")
    }
}

[test(description = "a text written without ё has чем, нем, всем and моем spelled with ё right after the prepositions that leave them no other reading, in the letter's own case; without the preposition, across anything but spaces, after не, before an abbreviation's stop, marked or already spelled, the word stands as written")]
def test_yo_after_preposition(t : T?) {
    t |> run("the forms the rule spells, and the controls it leaves") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "дело" => [1] }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_yo_rule.bin", s, 16))
        let spelled <- {
            "о чем" => "о чём", "В чем дело" => "В чём д+ело", "ни о чем" => "ни о чём", "на чем" => "на чём", "при чем" => "при чём",
            "в нем" => "в нём", "на нем" => "на нём", "о нем" => "о нём", "при нем" => "при нём",
            "во всем" => "во всём", "на всем" => "на всём", "обо всем" => "обо всём", "при всем" => "при всём",
            "в моем" => "в моём", "о моем" => "о моём",
            "О ЧЕМ" => "О ЧЁМ", "Во Всем" => "Во Всём", "о  чем" => "о  чём", "на нем." => "на нём.", "на нем. Яз" => "на нём. +Яз"
        }
        for (text, want in keys(spelled), values(spelled)) {
            t |> equal(ru_mark_stress(p, text), want, text)
        }
        let left <- {
            "чем" => "ч+ем", "дело чем" => "д+ело ч+ем", "по всем" => "по вс+ем", "по нем" => "по н+ем", "о всем" => "о вс+ем",
            "на моем" => "на м+оем", "не о чем" => "не о ч+ем", "не в чем" => "не в ч+ем", "о, чем" => "о, ч+ем",
            "на нем. яз" => "на н+ем. +яз", "на нем., англ" => "на н+ем., +англ", "о ч+ем" => "о ч+ем", "о чём" => "о чём", "в ком" => "в к+ом"
        }
        for (text, want in keys(left), values(left)) {
            t |> equal(ru_mark_stress(p, text), want, "the control: {text}")
        }
    }
}

[test(description = "the rule tier reads a form of its table by the first rule of its family that holds - a neighbour's word, a neighbour's ending, a sentence edge - inside the word's own sentence, behind a context pass's pick and the text's mark; a word rule holds for the whole word alone, a rule may read a word ahead, and a pack with no rule section abstains on every word")]
def test_rule_tier(t : T?) {
    t |> run("three rules over one form, in running text and asked directly") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "стены" => [1, 2], "новые" => [1], "дома" => [1, 2] }
        var inscope bare <- load_ru_stress_pack(write_synth_pack("synth_ru_no_rules.bin", s, 16))
        s.rule_forms <- { "стены" => [1, 2, 1, 0], "дома" => [1, 1, 2, 2] }
        s.rule_lists |> emplace(wall_rules())
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_rules.bin", s, 16))
        let cases <- {
            "у новые стены" => "у н+овые ст+ены", "из новые стены" => "из н+овые стен+ы", "стены" => "стен+ы", "из, новые стены" => "из, н+овые стен+ы",
            "из новые. стены" => "из н+овые. стен+ы", "у стены" => "у ст+ены", "бые стены" => "б+ые ст+ены", "из новые ст+ены" => "из н+овые ст+ены"
        }
        for (text, want in keys(cases), values(cases)) {
            t |> equal(ru_mark_stress(p, text), want, text)
        }
        t |> equal(ru_mark_stress(bare, "из новые стены"), "из н+овые ст+ены", "the control: without the section the pack's default stands")
        var inscope picked <- { 2 => 1 }
        t |> equal(ru_mark_stress(p, "из новые стены", picked), "из н+овые ст+ены", "a context pass's pick outranks the rule")
        var inscope sentence <- [0, 0, 0]
        var inscope by_word <- ["из", "новые", "стены"]
        var inscope by_ending <- ["у", "новые", "стены"]
        t |> equal(ru_stress_rule(p, by_word, sentence, 2), 2, "the word two back decides side 0")
        t |> equal(ru_stress_rule(p, by_ending, sentence, 2), 1, "the ending one back decides side 1")
        t |> equal(ru_stress_rule(p, by_ending, sentence, 1), 0, "a word outside the rule table is not the tier's")
        t |> equal(ru_stress_rule(bare, by_word, sentence, 2), 0, "the control: a pack with no rule section abstains")
        var inscope unruled <- ["у", "дома"]
        t |> equal(ru_stress_rule(p, unruled, sentence, 1), 2, "where no rule holds the form's unruled side answers")
        t |> equal(ru_mark_stress(p, "у дома"), "у дом+а", "and running text reads it")
        t |> equal(ru_mark_stress(bare, "у дома"), "у д+ома", "the control: the pack's default without the section")
        var inscope cut <- [0, 0, 1]
        t |> equal(ru_stress_rule(p, by_word, cut, 2), 2, "a neighbour in another sentence is no neighbour: the edge rule decides")
        var inscope by_suffix <- ["приз", "новые", "стены"]
        t |> equal(ru_stress_rule(p, by_suffix, sentence, 2), 1, "a word rule holds for the whole word: приз only ends in из, so the ending rule after it decides")
    }
    t |> run("a rule that reads the word ahead") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "дома" => [1, 2] }
        s.rule_forms <- { "дома" => [1, 2, 1, 0] }
        s.rule_lists |> emplace([SynthRule(kind = RuRuleKind.word, slot = SLOT_ONE_AHEAD, verdict = VERDICT_SURE, key = "нет")])
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_rule_ahead.bin", s, 16))
        var inscope sentence <- [0, 0]
        var inscope ahead <- ["дома", "нет"]
        var inscope behind <- ["нет", "дома"]
        t |> equal(ru_stress_rule(p, ahead, sentence, 0), 2, "the word one ahead decides side 0")
        t |> equal(ru_stress_rule(p, behind, sentence, 1), 0, "the control: the same word behind is not the rule's, and the form names no unruled side")
        t |> equal(ru_mark_stress(p, "дома нет"), "дом+а н+ет", "in running text")
    }
    t |> run("a side that is the ё spelling") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "все" => [1], "всем" => [1], "они" => [2], "время" => [1], "дома" => [1, 2] }
        var inscope bare <- load_ru_stress_pack(write_synth_pack("synth_ru_yo_no_rules.bin", s, 16))
        let yo_first = RU_PICK_YO + 1
        s.rule_forms <- { "все" => [1, 1, yo_first, 2], "всем" => [2, 1, yo_first, 1], "чем-то" => [2, 1, yo_first, 2] }
        s.rule_lists <- [
            [SynthRule(kind = RuRuleKind.word, slot = SLOT_ONE_AHEAD, verdict = VERDICT_SURE, key = "они")],
            [SynthRule(kind = RuRuleKind.word, slot = SLOT_ONE_AHEAD, verdict = VERDICT_SIDE_1, key = "времени")]
        ]
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_yo_rules.bin", s, 16))
        let cases <- {
            "все они" => "вс+е он+и", "все время" => "всё вр+емя", "Все время" => "Всё вр+емя", "ВСЕ" => "ВСЁ",
            "во всем" => "во всём", "по всем" => "по вс+ем", "вс+е время" => "вс+е вр+емя",
            "чем-то" => "чём-т+о", "ч+ем-то" => "ч+ем-т+о"
        }
        for (text, want in keys(cases), values(cases)) {
            t |> equal(ru_mark_stress(p, text), want, text)
        }
        t |> equal(ru_mark_stress(bare, "все время"), "вс+е вр+емя", "the control: without the section the е spelling stands")
        t |> equal(ru_mark_stress(bare, "по всем"), "по вс+ем", "the control: the tier's unruled side for всем is the pack's own reading")
        var inscope picked <- { 0 => 1 }
        t |> equal(ru_mark_stress(p, "все время", picked), "вс+е вр+емя", "a context pass's pick outranks the tier's ё")
        var inscope as_yo <- { 0 => yo_first }
        t |> equal(ru_mark_stress(bare, "все они", as_yo), "всё он+и", "a context pass may pick the ё spelling")
        t |> equal(ru_mark_word(p, "дома", yo_first), "дома", "a ё pick on a vowel that is no е leaves the word as written")
    }
}

[test(description = "a word past the pack's 64-letter key is no key: no tier indexes past the key buffer, the word reads by the shape rule, and a pack that holds a longer key refuses to load naming the key's length")]
def test_word_longer_than_a_key(t : T?) {
    t |> run("a 70-letter word through every tier, and a pack built with one") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "вода" => [2] }
        s.endings <- { "ыч" => 1 }
        var inscope p <- load_ru_stress_pack(write_synth_pack("synth_ru_short_keys.bin", s, 16))
        let long_word = "{repeat("ба", 34)}ыч"
        t |> equal(ru_pack_default(p, long_word), 0, "no entry")
        t |> equal(ru_stem_pick(p, long_word), 0, "no stem")
        t |> equal(ru_patronymic_ordinal(p, long_word), 0, "no full patronymic")
        t |> equal(ru_ending_ordinal(p, long_word), 0, "no ending")
        t |> equal(ru_mark_word(p, long_word), "{repeat("ба", 33)}б+аыч", "the shape rule: the vowel before the last")
        s.stress |> emplace(long_word, [1])
        let path = write_synth_pack("synth_ru_long_key.bin", s, 16)
        let why = panic_text() {
            load_and_drop(path)
        }
        t |> success(find(why, "holds a key of 70 letters") >= 0, why)
    }
}

def private load_model_and_drop(path : string) {
    var inscope m <- load_tts_model(path)
}

def private speak_and_drop(var m : TtsModel; text : string) {
    var inscope a <- synthesize(m, text, "")
}

[test(description = "a directory loads as a front end with no voice: the Russian pack it holds is its one language, marks stress and phonemizes, another language marks nothing, the roster is empty and a synthesis panics naming the missing voice; a directory with no pack refuses to load naming the packs")]
def test_front_end_from_a_directory(t : T?) {
    t |> run("a directory holding the Russian pack alone, and an empty one") @(t : T?) {
        var inscope s : SynthPack
        s.stress <- { "вода" => [2], "замок" => [1, 2] }
        let dir = scratch_path("front_end_only")
        mkdir(dir)
        var inscope bytes <- synth_pack_bytes(s, 16)
        write_bytes("front_end_only/tts_g2p_ru.bin", bytes)
        var inscope m <- load_tts_model(dir)
        t |> success(tts_front_end_only(m), "a directory is a front end with no voice")
        t |> success(tts_has_phonemes(m), "which phonemizes")
        var inscope c <- caps(m)
        t |> equal(length(c.voices), 0, "no voice in the roster")
        t |> success(length(c.langs) == 1 && c.langs[0] == "ru", "Russian the one language: {c.langs}")
        t |> equal(tts_mark_stress(m, "вода и замок", "ru"), "вод+а и з+амок", "the pack's stress on the text")
        t |> success(!empty(tts_phonemize(m, "вода", "ru")), "and its phonemes")
        t |> equal(tts_mark_stress(m, "water", "en-us"), "", "a language whose front end marks no text answers nothing")
        let no_voice = panic_text() {
            speak_and_drop(m, "вода")
        }
        t |> success(find(no_voice, "no voice to speak") >= 0, no_voice)
        let empty_dir = scratch_path("front_end_empty")
        mkdir(empty_dir)
        let no_pack = panic_text() {
            load_model_and_drop(empty_dir)
        }
        t |> success(find(no_pack, "holds no front-end pack") >= 0 && find(no_pack, "tts_g2p_ru.bin") >= 0, no_pack)
    }
}
