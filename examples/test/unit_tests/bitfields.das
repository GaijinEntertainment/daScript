options gen2
bitfield bits123 {
    one,
    two,
    three,
}

bitfield bits123_8 : uint8 {
    one,
    two,
    three,
}

bitfield bits123_16 : uint16 {
    one,
    two,
    three,
}

bitfield bits123_64 : uint64 {
    one,
    two,
    three,
}

[sideeffects]
def bit(b : int) {
    return bitfield(1 << b)
}

// Use global mutable variables to avoid optimizations.
var t : bitfield < one; two; three > = bitfield(1 << 1) | bit(2)
var bf_8 : bits123_8 = bits123_8.one
var bf_16 : bits123_16 = bits123_16.two
var bf : bits123 = bits123.three
var bf_64 : bits123_64 = bits123_64.one

[export]
def test() {
    assert(!t.one && t.two && t.three)
    assert("{t}" == "(two|three)")
    t ^= bitfield(1 << 1)
    assert(!t.one && !t.two && t.three)
    assert(t == bits123.three)
    t |= bitfield(1 << 0)
    assert(t.one && !t.two && t.three)

    assert(bf_8.one && !bf_8.two)
    assert(bf_16.two && !bf_8.three)
    assert(bf.three && !bf.one)
    assert(bf_64.one && !bf_64.two)
    return true
}
