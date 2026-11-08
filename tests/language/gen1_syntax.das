options gen2 = false
require dastest/testing_boost

[test(description = "the gen1 lexer reads the C character escapes - bell, vertical tab, hex, octal, quote and question mark - to their code values")]
def test_c_character_escapes_lex_in_gen1(t : T?)
    t |> equal(7, '\a')
    t |> equal(11u, '\v'u)
    t |> equal(uint8(65), '\x41'u8)
    t |> equal(65, '\101')
    t |> equal(39, '\'')
    t |> equal(63, '\?')

[ comment(large=9007199254740991l, signed_min=-9223372036854775808l, unsigned_max=18446744073709551615ul)]
struct WideOld
    @limit = 4102444800l
    value : int64

[comment(hex=0xD0, values=(0xD0, 7u))]
struct HexOld
    value : uint16

[test(description = "gen1 struct annotations keep full 64-bit signed, unsigned and hex literal arguments, and the int64 minimum literal parses")]
def test_annotation_int64_gen1(t : T?)
    t |> equal(typeinfo struct_get_annotation_argument<comment; large>(type<WideOld>),
               9007199254740991l)
    t |> equal(typeinfo struct_get_annotation_argument<comment; signed_min>(type<WideOld>),
               int64(0x8000000000000000ul))
    t |> equal(typeinfo struct_get_annotation_argument<comment; unsigned_max>(type<WideOld>),
               18446744073709551615ul)
    t |> equal(typeinfo struct_get_annotation_argument<comment; hex>(type<HexOld>), 0xD0ul)
    t |> equal(-9223372036854775808l, int64(0x8000000000000000ul))

[comment(min=-2, offset=-1.5, values=(-3, -1.5))]
struct SignedOld
    @min = -2
    value : int

[test(description = "gen1 struct annotations keep negative int and float literal arguments")]
def test_annotation_signed_numbers_gen1(t : T?)
    t |> equal(typeinfo struct_get_annotation_argument<comment; min>(type<SignedOld>), -2)
    t |> equal(typeinfo struct_get_annotation_argument<comment; offset>(type<SignedOld>), -1.5)
