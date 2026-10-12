options gen2
options indenting = 4
options _ascii_strings = false
require dastest/testing_boost public
require daslib/unicode_text
require strings
require daslib/strings_boost
require daslib/fio
require daslib/utf8_utils

[test]
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
[test]
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

[test]
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

[test]
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

[test]
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
