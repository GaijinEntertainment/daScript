options gen2 = false
[comment(large=9007199254740991l, signed_min=-9223372036854775808l, unsigned_max=18446744073709551615ul)]
struct WideOld
    @limit = 4102444800l
    value : int64
[comment(hex=0xD0, values=(0xD0, 7u))]
struct HexOld
    value : uint16
[export]
def main
    verify(typeinfo struct_get_annotation_argument<comment; large>(type<WideOld>) == 9007199254740991l)
    verify(typeinfo struct_get_annotation_argument<comment; signed_min>(type<WideOld>) == int64(0x8000000000000000ul))
    verify(typeinfo struct_get_annotation_argument<comment; unsigned_max>(type<WideOld>) == 18446744073709551615ul)
    verify(typeinfo struct_get_annotation_argument<comment; hex>(type<HexOld>) == 0xD0ul)
    verify(-9223372036854775808l == int64(0x8000000000000000ul))
