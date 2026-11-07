options gen2
require dastest/testing_boost

def low_bits(p : uint8?) : uint => unsafe(reinterpret<uint>(p))

def all_bits(p : uint8?) : uint64 => unsafe(reinterpret<uint64>(p))

[test]
def test_a_pointer_reinterpreted_to_a_32_bit_integer_is_its_low_bits(t : T?) {
    var data : uint8[64]
    let p = unsafe(addr(data[0]))
    let q : uint8? = unsafe(p + 4)
    t |> equal(all_bits(q) - all_bits(p), 4ul)
    t |> equal(low_bits(q) - low_bits(p), 4u)
    t |> equal(low_bits(p), uint(all_bits(p) & 0xfffffffful))
}
