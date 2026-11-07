options gen2 = false
require dastest/testing_boost

[test]
def test_c_character_escapes_lex_in_gen1(t : T?)
    t |> equal(7, '\a')
    t |> equal(11u, '\v'u)
    t |> equal(uint8(65), '\x41'u8)
    t |> equal(65, '\101')
    t |> equal(39, '\'')
    t |> equal(63, '\?')
