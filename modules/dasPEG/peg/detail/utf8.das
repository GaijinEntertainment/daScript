options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module peg_utf8

require dastest/testing_boost
require daslib/utf8_utils

// Array is asserted to be valid utf-8 at the beginning of the parsing.
def public get_next_character(utf8_string: array<uint8>; var index: int&): int
    var codepoint = 0u
    var state = 0u

    while true
        let byte = utf8_string[index++] |> uint()
        let type_ = s_utf8d[byte]

        if state == UTF8_ACCEPT
            codepoint = (0xFF >> type_) & byte
        else
            codepoint = (byte & 0x3F) | (codepoint << 6u)

        state = s_utf8d[256u + state + type_]

        if state == UTF8_ACCEPT
            return unsafe (reinterpret<int> codepoint)
        if state == -1u // UTF8_REJECT
            return -1

    return -1


[test]
def test_decoding_ascii(t: T?)
    var input : array<uint8> <- [{auto 'a'u8; 'b'u8; 'c'u8}]
    var index = 0

    t |> equal('a', input |> get_next_character <| index)
    t |> equal('b', input |> get_next_character <| index)
    t |> equal('c', input |> get_next_character <| index)


[test]
def test_decoding_russian(t: T?)
    var input : array<uint8> <- [{auto 0xD0u8; 0xB0u8;
                                       0xD1u8; 0x80u8;
                                       0xD0u8; 0xB8u8;
                                       0xD0u8; 0xB2u8;
                                       0xD0u8; 0xB5u8;
                                       0xD1u8; 0x82u8}]
    var index = 0

    // Codes for symbols:
    // https://symbl.cc/ru/unicode/blocks/cyrillic

    t |> equal(1072, input |> get_next_character <| index) // п
    t |> equal(1088, input |> get_next_character <| index) // р
    t |> equal(1080, input |> get_next_character <| index) // и
    t |> equal(1074, input |> get_next_character <| index) // в
    t |> equal(1077, input |> get_next_character <| index) // е
    t |> equal(1090, input |> get_next_character <| index) // т


[test]
def public test_decoding_japanese(t: T?)
    var input : array<uint8> <- [{auto 0xE5u8; 0xA5u8; 0xBDu8;
                                       0xE3u8; 0x81u8; 0x8Du8}]
    var index = 0

    t |> equal(22909, input |> get_next_character <| index) // 好
    t |> equal(12365, input |> get_next_character <| index) // き
