options gen2
require dastest/testing_boost
require strings

[test]
def test_c_character_escapes_lex(t : T?) {
    t |> equal(10, '\n')
    t |> equal(0, '\0')
    t |> equal(7, '\a')
    t |> equal(11, '\v')
    t |> equal(65, '\x41')
    t |> equal(65, '\101')
}

[test]
def test_the_remaining_c_escapes_lex(t : T?) {
    t |> equal(8, '\b')
    t |> equal(9, '\t')
    t |> equal(12, '\f')
    t |> equal(13, '\r')
    t |> equal(92, '\\')
    t |> equal(39, '\'')
    t |> equal(34, '\"')
    t |> equal(63, '\?')
    t |> equal(15, '\xf')
    t |> equal(255, '\xFF')
    t |> equal(7, '\7')
    t |> equal(63, '\77')
    t |> equal(255, '\377')
}

[test]
def test_escapes_take_the_unsigned_suffixes(t : T?) {
    t |> equal(0u, '\0'u)
    t |> equal(11u, '\v'u)
    t |> equal(uint8(65), '\x41'u8)
    t |> equal(uint8(7), '\a'u8)
}

[test]
def test_a_string_takes_the_c_escape_set(t : T?) {
    t |> equal("\x41\101", "AA")
    t |> equal("\101" + "2", "\1012")
    t |> equal("it\'s", "it's")
    peek_data("\a\v\?\377\1012") $(bytes) {
        t |> equal(length(bytes), 6)
        t |> equal(int(bytes[0]), 7)
        t |> equal(int(bytes[1]), 11)
        t |> equal(int(bytes[2]), 63)
        t |> equal(int(bytes[3]), 255)
        t |> equal(int(bytes[4]), 65)
        t |> equal(int(bytes[5]), '2')
    }
    peek_data("\4777") $(bytes) {
        t |> equal(length(bytes), 3)
        t |> equal(int(bytes[0]), 39)
        t |> equal(int(bytes[1]), '7')
    }
}

[test]
def test_an_embedded_nul_ends_a_string(t : T?) {
    peek_data("a\0b") $(bytes) {
        t |> equal(length(bytes), 1)
        t |> equal(int(bytes[0]), 'a')
    }
}
