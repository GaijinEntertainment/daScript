options gen2
options indenting = 4
require dastest/testing_boost public
require daslib/utf8_utils
require strings

def private replacement_char_utf8 : array<uint8> => <- [uint8(0xEF), uint8(0xBF), uint8(0xBD)]

[test]
def utf8_sanitize_keeps_valid_text(t : T?) {
    let text = string([uint8(0x41), uint8(0xC3), uint8(0xA9), uint8(0xE2), uint8(0x82), uint8(0xAC),
                       uint8(0xF0), uint8(0x9F), uint8(0x98), uint8(0x80)])
    t |> equal(utf8_sanitize(text), text, "a valid 1-, 2-, 3- and 4-byte run comes back as it is")
    t |> equal(utf8_sanitize(""), "", "an empty string stays empty")
}

[test]
def utf8_sanitize_replaces_a_lone_continuation_byte(t : T?) {
    let text = string([uint8(0x41), uint8(0x80), uint8(0x42)])
    var want : array<uint8>
    want |> push(uint8(0x41))
    want |> push_from(replacement_char_utf8())
    want |> push(uint8(0x42))
    t |> equal(utf8_sanitize(text), string(want),
        "the stray byte becomes one replacement, the letters around it stay")
}

[test]
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
[test]
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

[test]
def utf8_sanitize_output_is_valid(t : T?) {
    let text = string([uint8(0xFF), uint8(0xC3), uint8(0x28), uint8(0xF0), uint8(0x90), uint8(0x80),
                       uint8(0x41)])
    let out = utf8_sanitize(text)
    t |> success(is_utf8_string_valid(out), "whatever came in, the result is valid UTF-8")
    t |> success(!is_utf8_string_valid(text), "... and the input was not")
}
