options gen2
options _ascii_strings = false

require dastest/testing_boost public
require daslib/utf8_utils
require strings
require daslib/unicode_text
require daslib/strings_boost
require daslib/fio

[test(description = "utf8_decode strips a leading BOM by default and keeps it as U+FEFF when told so, for byte and string input and for the appending overloads")]
def utf8_decode_bom_policy_preserves_default_and_all_explicit_overloads(t : T?) {
    let bytes = [uint8(0xef), uint8(0xbb), uint8(0xbf), uint8(0x41)]
    let text = string(bytes)
    var inscope default_bytes <- utf8_decode(bytes)
    var inscope default_text <- utf8_decode(text)
    t |> equal(default_bytes, [0x41u])
    t |> equal(default_text, [0x41u])
    var inscope kept_bytes <- utf8_decode(bytes, false)
    var inscope kept_text <- utf8_decode(text, false)
    t |> equal(kept_bytes, [0xfeffu, 0x41u])
    t |> equal(kept_text, [0xfeffu, 0x41u])
    var inscope appended_bytes = [0x42u]
    var inscope appended_text = [0x42u]
    utf8_decode(appended_bytes, bytes, false)
    utf8_decode(appended_text, text, false)
    t |> equal(appended_bytes, [0x42u, 0xfeffu, 0x41u])
    t |> equal(appended_text, [0x42u, 0xfeffu, 0x41u])
}

def private replacement_char_utf8 : array<uint8> => <- [uint8(0xEF), uint8(0xBF), uint8(0xBD)]

[test(description = "utf8_sanitize returns valid text with one-, two-, three- and four-byte characters unchanged")]
def utf8_sanitize_keeps_valid_text(t : T?) {
    let text = string([uint8(0x41), uint8(0xC3), uint8(0xA9), uint8(0xE2), uint8(0x82), uint8(0xAC),
                       uint8(0xF0), uint8(0x9F), uint8(0x98), uint8(0x80)])
    t |> equal(utf8_sanitize(text), text, "a valid 1-, 2-, 3- and 4-byte run comes back as it is")
    t |> equal(utf8_sanitize(""), "", "an empty string stays empty")
}

[test(description = "utf8_sanitize replaces a stray continuation byte with the replacement character")]
def utf8_sanitize_replaces_a_lone_continuation_byte(t : T?) {
    let text = string([uint8(0x41), uint8(0x80), uint8(0x42)])
    var want : array<uint8>
    want |> push(uint8(0x41))
    want |> push_from(replacement_char_utf8())
    want |> push(uint8(0x42))
    t |> equal(utf8_sanitize(text), string(want),
        "the stray byte becomes one replacement, the letters around it stay")
}

[test(description = "utf8_sanitize replaces a multi-byte sequence cut short at the end of the text")]
def utf8_sanitize_replaces_a_sequence_cut_short(t : T?) {
    let text = string([uint8(0x41), uint8(0xE2), uint8(0x82)])
    var want : array<uint8>
    want |> push(uint8(0x41))
    want |> push_from(replacement_char_utf8())
    t |> equal(utf8_sanitize(text), string(want),
        "an incomplete sequence at the end is one replacement")
    let mid = string([uint8(0xE2), uint8(0x82), uint8(0x41)])
    var want_mid <- replacement_char_utf8()
    want_mid |> push(uint8(0x41))
    t |> equal(utf8_sanitize(mid), string(want_mid),
        "an incomplete sequence before a letter is one replacement and the letter")
}

//! `bytes` sanitized against `want`
def private sanitized_is(t : T?; bytes : array<uint8>; want : array<uint8>; what : string) {
    t |> equal(utf8_sanitize(string(bytes)), string(want), what)
}

//! `n` replacements then the letter A
def private replacements_then_a(n : int) : array<uint8> {
    var want : array<uint8>
    for (_i in range(n)) {
        want |> push_from(replacement_char_utf8())
    }
    want |> push(uint8(0x41))
    return <- want
}

//! every maximal subpart of an ill-formed sequence is its own
//! replacement: a lead whose next byte is outside the lead's own
//! range stands alone, and each byte after it that starts nothing
//! stands alone too
[test(description = "utf8_sanitize replaces each maximal invalid subpart: cut-off leads, surrogates, overlong forms and code points past U+10FFFF")]
def utf8_sanitize_replaces_the_maximal_subpart(t : T?) {
    sanitized_is(t, [uint8(0xE2), uint8(0xFF), uint8(0x41)], replacements_then_a(2),
        "a lead cut off by a byte that starts nothing: one replacement each")
    sanitized_is(t, [uint8(0xED), uint8(0xA0), uint8(0x80), uint8(0x41)], replacements_then_a(3),
        "a surrogate: ED takes 80-9F next, so the lead and each continuation stand alone")
    sanitized_is(t, [uint8(0xE0), uint8(0x80), uint8(0x80), uint8(0x41)], replacements_then_a(3),
        "an overlong three-byte encoding: E0 takes A0-BF next")
    sanitized_is(t, [uint8(0xC0), uint8(0x80), uint8(0x41)], replacements_then_a(2),
        "an overlong two-byte encoding: C0 starts nothing")
    sanitized_is(t, [uint8(0xF4), uint8(0x90), uint8(0x80), uint8(0x80), uint8(0x41)],
        replacements_then_a(4), "a code point past U+10FFFF: F4 takes 80-8F next")
}

[test(description = "utf8_sanitize turns a mix of invalid sequences into text that is valid UTF-8")]
def utf8_sanitize_output_is_valid(t : T?) {
    let text = string([uint8(0xFF), uint8(0xC3), uint8(0x28), uint8(0xF0), uint8(0x90), uint8(0x80),
                       uint8(0x41)])
    let out = utf8_sanitize(text)
    t |> success(is_utf8_string_valid(out), "whatever came in, the result is valid UTF-8")
    t |> success(!is_utf8_string_valid(text), "... and the input was not")
}

[test(description = "utf32_is_word_char accepts ASCII alphanumerics, underscore, Latin-1, Greek, Cyrillic and CJK letters and rejects punctuation and whitespace in each block")]
def test_utf32_is_word_char(t : T?) {
    t |> run("ASCII alphanumeric + underscore are word chars") @(t : T?) {
        t |> equal(utf32_is_word_char(uint('a')), true)
        t |> equal(utf32_is_word_char(uint('Z')), true)
        t |> equal(utf32_is_word_char(uint('0')), true)
        t |> equal(utf32_is_word_char(uint('9')), true)
        t |> equal(utf32_is_word_char(uint('_')), true)
    }
    t |> run("ASCII punctuation + whitespace are non-word") @(t : T?) {
        t |> equal(utf32_is_word_char(uint(' ')), false)
        t |> equal(utf32_is_word_char(uint('.')), false)
        t |> equal(utf32_is_word_char(uint(',')), false)
        t |> equal(utf32_is_word_char(uint(';')), false)
        t |> equal(utf32_is_word_char(uint('-')), false)
        t |> equal(utf32_is_word_char(uint('!')), false)
        t |> equal(utf32_is_word_char(uint('@')), false)
        t |> equal(utf32_is_word_char(uint('[')), false)
        t |> equal(utf32_is_word_char(uint('`')), false)
    }
    t |> run("Latin-1 letters are word chars") @(t : T?) {
        t |> equal(utf32_is_word_char(0x00C0u), true)   // À
        t |> equal(utf32_is_word_char(0x00DFu), true)   // ß
        t |> equal(utf32_is_word_char(0x00E9u), true)   // é
        t |> equal(utf32_is_word_char(0x00F1u), true)   // ñ
        t |> equal(utf32_is_word_char(0x00FFu), true)   // ÿ
    }
    t |> run("Latin-1 punctuation/symbols are non-word") @(t : T?) {
        t |> equal(utf32_is_word_char(0x00A0u), false)  // no-break space
        t |> equal(utf32_is_word_char(0x00A1u), false)  // ¡
        t |> equal(utf32_is_word_char(0x00BFu), false)  // ¿
        t |> equal(utf32_is_word_char(0x00D7u), false)  // ×
        t |> equal(utf32_is_word_char(0x00F7u), false)  // ÷
    }
    t |> run("Greek + Cyrillic letters are word chars") @(t : T?) {
        t |> equal(utf32_is_word_char(0x0391u), true)   // Α
        t |> equal(utf32_is_word_char(0x03B1u), true)   // α
        t |> equal(utf32_is_word_char(0x0410u), true)   // А
        t |> equal(utf32_is_word_char(0x044Fu), true)   // я
        t |> equal(utf32_is_word_char(0x0401u), true)   // Ё
    }
    t |> run("CJK ideographs are word chars") @(t : T?) {
        t |> equal(utf32_is_word_char(0x4E2Du), true)   // 中
        t |> equal(utf32_is_word_char(0x6587u), true)   // 文
        t |> equal(utf32_is_word_char(0x65E5u), true)   // 日
    }
    t |> run("CJK punctuation is non-word") @(t : T?) {
        t |> equal(utf32_is_word_char(0x3000u), false)  // ideographic space
        t |> equal(utf32_is_word_char(0x3001u), false)  // ideographic comma
        t |> equal(utf32_is_word_char(0x3002u), false)  // ideographic period
        t |> equal(utf32_is_word_char(0x300Au), false)  // 《 left double angle bracket
    }
    t |> run("General punctuation block is non-word") @(t : T?) {
        t |> equal(utf32_is_word_char(0x2014u), false)  // — em dash
        t |> equal(utf32_is_word_char(0x2018u), false)  // ' left single quote
        t |> equal(utf32_is_word_char(0x201Cu), false)  // " left double quote
        t |> equal(utf32_is_word_char(0x2026u), false)  // … horizontal ellipsis
    }
    t |> run("Math + arrow + symbol blocks are non-word") @(t : T?) {
        t |> equal(utf32_is_word_char(0x2192u), false)  // → rightwards arrow
        t |> equal(utf32_is_word_char(0x2200u), false)  // ∀ for all
        t |> equal(utf32_is_word_char(0x2603u), false)  // ☃ snowman
    }
    t |> run("Fullwidth ASCII punctuation is non-word") @(t : T?) {
        t |> equal(utf32_is_word_char(0xFF01u), false)  // ！
        t |> equal(utf32_is_word_char(0xFF0Cu), false)  // ，
        t |> equal(utf32_is_word_char(0xFF1Fu), false)  // ？
    }
    t |> run("Fullwidth letters and digits remain word chars") @(t : T?) {
        t |> equal(utf32_is_word_char(0xFF21u), true)   // Ａ
        t |> equal(utf32_is_word_char(0xFF41u), true)   // ａ
        t |> equal(utf32_is_word_char(0xFF10u), true)   // ０
    }
}


[test(description = "utf32_to_lower folds ASCII, Latin-1, Latin Extended-A, Greek and Cyrillic capitals, skips the Latin-1 special cases and passes CJK and out-of-range codepoints through")]
def test_utf32_to_lower(t : T?) {
    t |> run("ASCII case fold") @(t : T?) {
        t |> equal(utf32_to_lower(uint('A')), uint('a'))
        t |> equal(utf32_to_lower(uint('Z')), uint('z'))
        t |> equal(utf32_to_lower(uint('a')), uint('a'))
        t |> equal(utf32_to_lower(uint('z')), uint('z'))
        t |> equal(utf32_to_lower(uint('0')), uint('0'))
        t |> equal(utf32_to_lower(uint(' ')), uint(' '))
    }
    t |> run("Latin-1 supplement folds") @(t : T?) {
        t |> equal(utf32_to_lower(0x00C0u), 0x00E0u)    // À → à
        t |> equal(utf32_to_lower(0x00C9u), 0x00E9u)    // É → é
        t |> equal(utf32_to_lower(0x00D1u), 0x00F1u)    // Ñ → ñ
        t |> equal(utf32_to_lower(0x00DEu), 0x00FEu)    // Þ → þ
    }
    t |> run("Latin-1 special-case characters do not fold") @(t : T?) {
        t |> equal(utf32_to_lower(0x00DFu), 0x00DFu)    // ß stays ß
        t |> equal(utf32_to_lower(0x00D7u), 0x00D7u)    // × stays ×
        t |> equal(utf32_to_lower(0x00F7u), 0x00F7u)    // ÷ stays ÷
    }
    t |> run("Latin Extended-A folds") @(t : T?) {
        t |> equal(utf32_to_lower(0x0100u), 0x0101u)    // Ā → ā
        t |> equal(utf32_to_lower(0x0139u), 0x013Au)    // Ĺ → ĺ
        t |> equal(utf32_to_lower(0x0178u), 0x00FFu)    // Ÿ → ÿ
    }
    t |> run("Greek capitals fold") @(t : T?) {
        t |> equal(utf32_to_lower(0x0391u), 0x03B1u)    // Α → α
        t |> equal(utf32_to_lower(0x03A9u), 0x03C9u)    // Ω → ω
    }
    t |> run("Cyrillic capitals fold") @(t : T?) {
        t |> equal(utf32_to_lower(0x0410u), 0x0430u)    // А → а
        t |> equal(utf32_to_lower(0x042Fu), 0x044Fu)    // Я → я
        t |> equal(utf32_to_lower(0x0401u), 0x0451u)    // Ё → ё
    }
    t |> run("CJK / out-of-range codepoints pass through") @(t : T?) {
        t |> equal(utf32_to_lower(0x4E2Du), 0x4E2Du)    // 中
        t |> equal(utf32_to_lower(0x10000u), 0x10000u)  // beyond BMP
    }
}


[test(description = "each_word tokenizes on punctuation and whitespace, lowercases Latin and Cyrillic text, yields nothing for empty input and appends to its output buffer")]
def test_each_word(t : T?) {
    t |> run("ASCII tokenization") @(t : T?) {
        var words : array<string>
        each_word("the quick brown fox", words)
        t |> equal(length(words), 4)
        t |> equal(words[0], "the")
        t |> equal(words[1], "quick")
        t |> equal(words[2], "brown")
        t |> equal(words[3], "fox")
    }
    t |> run("Punctuation splits tokens") @(t : T?) {
        var words : array<string>
        each_word("hello, world! foo.bar-baz_qux", words)
        t |> equal(length(words), 5)
        t |> equal(words[0], "hello")
        t |> equal(words[1], "world")
        t |> equal(words[2], "foo")
        t |> equal(words[3], "bar")
        t |> equal(words[4], "baz_qux")
    }
    t |> run("Case folding lowercases ASCII") @(t : T?) {
        var words : array<string>
        each_word("The Quick BROWN Fox", words)
        t |> equal(words[0], "the")
        t |> equal(words[1], "quick")
        t |> equal(words[2], "brown")
        t |> equal(words[3], "fox")
    }
    t |> run("Latin-1 case folding") @(t : T?) {
        var words : array<string>
        each_word("Élève NIÑO café", words)
        t |> equal(length(words), 3)
        t |> equal(words[0], "élève")
        t |> equal(words[1], "niño")
        t |> equal(words[2], "café")
    }
    t |> run("Cyrillic case folding") @(t : T?) {
        var words : array<string>
        each_word("Привет Мир", words)
        t |> equal(length(words), 2)
        t |> equal(words[0], "привет")
        t |> equal(words[1], "мир")
    }
    t |> run("CJK ideographs tokenize on punctuation") @(t : T?) {
        var words : array<string>
        each_word("中文，日本語", words)
        t |> equal(length(words), 2)
        t |> equal(words[0], "中文")
        t |> equal(words[1], "日本語")
    }
    t |> run("Empty input produces no tokens") @(t : T?) {
        var words : array<string>
        each_word("", words)
        t |> equal(length(words), 0)
    }
    t |> run("Whitespace-only input produces no tokens") @(t : T?) {
        var words : array<string>
        each_word("   \t\n  ", words)
        t |> equal(length(words), 0)
    }
    t |> run("Output buffer is appended, not cleared") @(t : T?) {
        var words : array<string>
        words |> push("seed")
        each_word("hello world", words)
        t |> equal(length(words), 3)
        t |> equal(words[0], "seed")
        t |> equal(words[1], "hello")
        t |> equal(words[2], "world")
    }
}

[test(description = "NFD, full case folding and the caseless key give the known results for composed, special-case, Greek and CJK text, including aliased input and output")]
def unicode_normalization_and_full_folding_known_vectors(t : T?) {
    var value : string
    t |> success(try_unicode_nfd("é각", value))
    t |> equal(value, "é각")
    t |> success(try_unicode_casefold("StraßeΣςİ𐐀", value))
    t |> equal(value, "strasseσσi̇𐐨")
    t |> success(try_unicode_caseless_key("Άͅ", value))
    t |> equal(value, "άι", "canonical order precedes folding U+0345 to a starter")
    var equivalent : string
    t |> success(try_unicode_caseless_key("ÉVA", value))
    t |> success(try_unicode_caseless_key("éva", equivalent))
    t |> equal(value, equivalent)
    t |> success(try_unicode_caseless_key("café", value))
    t |> success(try_unicode_caseless_key("cafe", equivalent))
    t |> success(value != equivalent, "accents are preserved")
    t |> success(try_unicode_caseless_key("中文 日本語 한국어 👩‍💻", value))
    t |> equal(value, "中文 日本語 한국어 👩‍💻")
    value = "Straße"
    t |> success(try_unicode_caseless_key(value, value))
    t |> equal(value, "strasse", "input/output alias is supported")
}
[test(description = "the unicode conversions reject invalid UTF-8 and leave the output string untouched, while empty input succeeds")]
def unicode_conversion_rejects_invalid_utf8_without_publishing_output(t : T?) {
    let invalid = string([uint8(0xc0), uint8(0xaf)])
    var value = "unchanged"
    t |> success(!try_unicode_nfd(invalid, value))
    t |> equal(value, "unchanged")
    t |> success(!try_unicode_casefold(invalid, value))
    t |> equal(value, "unchanged")
    t |> success(!try_unicode_caseless_key(invalid, value))
    t |> equal(value, "unchanged")
    t |> success(try_unicode_caseless_key("", value))
    t |> equal(value, "")
}

def private scalar_sequence(t : T?; field : string) : string {
    var inscope bytes : array<uint8>
    reserve(bytes, length(field))
    var inscope words <- split(strip(field), " ")
    for (word in words) {
        continue if (empty(word))
        var conversion : ConversionResult
        var consumed = 0
        let codepoint = uint("0x{word}", conversion, consumed, true)
        t |> success(conversion == ConversionResult.ok && consumed == length(word) + 2)
        utf8_encode(bytes, codepoint)
    }
    return string(bytes)
}

[test(description = "try_unicode_nfd agrees with every line of the official Unicode 17 NormalizationTest corpus")]
def unicode_nfd_matches_the_official_conformance_corpus(t : T?) {
    let corpus = fread(path_join(get_das_root(), "3rdparty/unicode/17.0.0/NormalizationTest.txt"))
    t |> success(contains(corpus, "NormalizationTest-17.0.0.txt"))
    var inscope lines <- split(corpus, "\n")
    var count = 0
    for (line in lines) {
        continue if (empty(line) || starts_with(line, "#") || starts_with(line, "@"))
        var inscope fields <- split(line, ";")
        t |> success(length(fields) >= 5)
        if (length(fields) < 5) return
        var inscope values : array<string>
        reserve(values, 5)
        for (index in range(5)) { values |> push(scalar_sequence(t, fields[index])) }
        for (index in range(5)) {
            var normalized : string
            t |> success(try_unicode_nfd(values[index], normalized))
            let expected = values[index < 3 ? 2 : 4]
            if (normalized != expected) { t |> equal(normalized, expected, "NFD vector {count}, column {index}") }
        }
        count++
    }
    t |> equal(count, 20034)
}

[test(description = "full case folding agrees with every default mapping of the official Unicode 17 CaseFolding table")]
def unicode_full_casefold_matches_every_official_default_mapping(t : T?) {
    let corpus = fread(path_join(get_das_root(), "3rdparty/unicode/17.0.0/CaseFolding.txt"))
    t |> success(contains(corpus, "CaseFolding-17.0.0.txt"))
    var inscope lines <- split(corpus, "\n")
    var count = 0
    for (line in lines) {
        continue if (empty(line) || starts_with(line, "#"))
        var inscope fields <- split(line, ";")
        t |> success(length(fields) >= 3)
        if (length(fields) < 3) return
        let status = strip(fields[1])
        continue if (status != "C" && status != "F")
        let source = scalar_sequence(t, fields[0])
        let expected = scalar_sequence(t, fields[2])
        var folded : string
        t |> success(try_unicode_casefold(source, folded))
        if (folded != expected) { t |> equal(folded, expected, "CaseFolding source {fields[0]}") }
        count++
    }
    t |> equal(count, 1585)
}

[test(description = "NFD, case folding and the caseless key treat a leading U+FEFF as text and keep it")]
def unicode_transformations_preserve_leading_feff(t : T?) {
    let marked = string([uint8(0xef), uint8(0xbb), uint8(0xbf), uint8(0x41)])
    let folded = string([uint8(0xef), uint8(0xbb), uint8(0xbf), uint8(0x61)])
    var result : string
    t |> success(try_unicode_nfd(marked, result))
    t |> equal(result, marked, "U+FEFF is text data for Unicode normalization")
    t |> success(try_unicode_casefold(marked, result))
    t |> equal(result, folded)
    t |> success(try_unicode_caseless_key(marked, result))
    t |> equal(result, folded, "leading FEFF must not collide with an unmarked name")
}
