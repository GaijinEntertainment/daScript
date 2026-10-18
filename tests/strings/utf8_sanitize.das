options gen2
options indenting = 4
require dastest/testing_boost public
require daslib/utf8_utils
require strings

//! the replacement character's bytes, the text a client decodes where the server's reply held an invalid sequence
def private replacement_char_utf8 : array<uint8> => <- [uint8(0xEF), uint8(0xBF), uint8(0xBD)]

[test]
def utf8_sanitize_keeps_valid_text(t : T?) {
    let text = string([uint8(0x41), uint8(0xC3), uint8(0xA9), uint8(0xE2), uint8(0x82), uint8(0xAC), uint8(0xF0), uint8(0x9F), uint8(0x98), uint8(0x80)])
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
    t |> equal(utf8_sanitize(text), string(want), "the stray byte becomes one replacement, the letters around it stay")
}

[test]
def utf8_sanitize_replaces_a_sequence_cut_short(t : T?) {
    let text = string([uint8(0x41), uint8(0xE2), uint8(0x82)])
    var want : array<uint8>
    want |> push(uint8(0x41))
    want |> push_from(replacement_char_utf8())
    t |> equal(utf8_sanitize(text), string(want), "an incomplete sequence at the end is one replacement")
    let mid = string([uint8(0xE2), uint8(0x82), uint8(0x41)])
    var want_mid <- replacement_char_utf8()
    want_mid |> push(uint8(0x41))
    t |> equal(utf8_sanitize(mid), string(want_mid), "an incomplete sequence before a letter is one replacement and the letter")
}

[test]
def utf8_sanitize_output_is_valid(t : T?) {
    let text = string([uint8(0xFF), uint8(0xC3), uint8(0x28), uint8(0xF0), uint8(0x90), uint8(0x80), uint8(0x41)])
    let out = utf8_sanitize(text)
    t |> success(is_utf8_string_valid(out), "whatever came in, the result is valid UTF-8")
    t |> success(!is_utf8_string_valid(text), "... and the input was not")
}
