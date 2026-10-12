options gen2
options indenting = 4
require dastest/testing_boost public
require daslib/utf8_utils
require strings

[test]
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
