options gen2

module enumerations

require dastest/testing_boost public
require daslib/enum_trait

enum Foo {
    one
    two
    three
}

[test(description = "an enum value interpolates and casts to its name, and to_enum parses a name back, falling back to the given default on an unknown name")]
def test_enum_conversions(t : T?) {
    t |> run("enum to string") @(t : T?) {
        let s = "{Foo.two}"
        t |> equal("two", s)
        let q = string(Foo.two)
        t |> equal("two", q)
    }
    t |> run("string to enum") @(t : T?) {
        let q = to_enum(type<Foo>, "two")
        t |> equal(Foo.two, q)
        let r = to_enum(type<Foo>, "five", Foo.three)
        t |> equal(Foo.three, r)
    }
}

[string_to_enum]
enum Bar {
    one
    two
    three
}

[test(description = "[string_to_enum] gives an enum a string constructor, with and without a default, beside the standard to_enum path")]
def test_enum_macro_conversions(t : T?) {
    t |> run("enum macro") @(t : T?) {
        let b = Bar("three")                // this conversion can panic
        t |> equal(Bar.three, b)
        let s = "{b}"
        t |> equal("three", s)
        let q = string(Bar.two)
        t |> equal("two", q)
        let r = to_enum(type<Bar>, "one")   // conversion via standard path
        t |> equal(Bar.one, r)
        let bb = Bar("five", Bar.two)       // conversion with the default value
        t |> equal(Bar.two, bb)
    }
}

// the "operations on different enumerations are prohibited" gate runs
// only after operator-overload lookup fails (reportOp2Errors), so a
// user operator over two different enums wins
def operator |(a : Foo; b : Bar) : Foo => unsafe(reinterpret<Foo>(int(a) | int(b)))

[test(description = "a user-defined operator over two different enum types is honored instead of the cross-enum error")]
def test_cross_enum_operator(t : T?) {
    t |> run("user operator | combines two different enums") @(t : T?) {
        let r = Foo.two | Bar.three     // 1 | 2 == 3
        t |> equal(3, int(r))
    }
}

// an enum named like its own module, used as a struct field: AOT must
// keep the two names apart
enum enumerations {
    A
    B
}

struct HoldsEnum {
    f : enumerations
}

[test(description = "an enum named like its own module works as a struct field type")]
def test_enum_named_like_module_as_field(t : T?) {
    let h = HoldsEnum(f = enumerations.B)
    t |> equal(int(h.f), 1)
}

bitfield Baz {
    bit0
    bit1
    bit2
    bit02 = Baz.bit0 | Baz.bit2
}

bitfield BazAllFields { // this is to test all 32 bits (if they compile)
    bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11, bit12, bit13, bit14, bit15,
    bit16, bit17, bit18, bit19, bit20, bit21, bit22, bit23, bit24, bit25, bit26, bit27, bit28, bit29, bit30, bit31
}

[test(description = "a 32-bit bitfield is 4 bytes, supports ==, !=, |, &, ^, ~ and their compound forms, converts to and from uint, and prints each of its 32 named bits")]
def test_bitfield_32(t : T?) {
    t |> run("regular 32-bit bitfield") @(t : T?) {
        var a = Baz.bit0
        static_assert(typeinfo sizeof(type<Baz>) == 4, "regular bitfield size is 32 bits")
        static_assert(typeinfo alignof(type<Baz>) == 4, "regular bitfield alignment is 32 bits")
        t |> equal(bitfield(0x1), a)
        // eq and neq
        t |> success(a == Baz.bit0)
        t |> success(a != Baz.bit1)
        // or and set or
        a = a | Baz.bit2
        t |> equal(bitfield(0x5), a)
        t |> equal(Baz.bit0 | Baz.bit2, a)
        a |= Baz.bit1                   // nolint:STYLE022 - set-or on a bitfield is the tested operator
        t |> equal(bitfield(0x7), a)
        t |> equal(Baz.bit0 | Baz.bit1 | Baz.bit2, a)
        // not(~), and and set and
        a &= ~Baz.bit1                  // nolint:STYLE022 - set-and with ~ is the tested operator
        t |> equal(bitfield(0x5), a)
        t |> equal(Baz.bit0 | Baz.bit2, a)
        a = a & Baz.bit0
        t |> equal(bitfield(0x1), a)
        t |> equal(Baz.bit0, a)
        // xor and set xor
        a = a ^ Baz.bit2
        t |> equal(bitfield(0x5), a)
        t |> equal(Baz.bit0 | Baz.bit2, a)
        a ^= Baz.bit0
        t |> equal(bitfield(0x4), a)
        t |> equal(Baz.bit2, a)
        // convert to uint
        t |> equal(0x4, uint(a))
        // construct from uint
        a = bitfield(0x3)
        t |> equal(Baz.bit0 | Baz.bit1, a)
    }
    t |> run("all fields in 32-bit bitfield") @(t : T?) {
        // lets test all fields and printing
        for (i in 0 .. 31) {
            // regular conversions and c-tor
            let baf = BazAllFields(1 << i)
            t |> equal(bitfield(1 << i), baf)
            // string conversions
            let s = "{baf}"
            t |> equal("(bit{i})", s)
        }
    }
}

bitfield Baz8 : uint8 {
    bit0
    bit1
    bit2
    bit02 = Baz8.bit0 | Baz8.bit2
}

bitfield BazAllFields8 : uint8 { // this is to test all 8 bits (if they compile)
    bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
}

[test(description = "a uint8 bitfield is 1 byte, supports ==, !=, |, &, ^, ~ and their compound forms, converts to and from uint8, and prints each of its 8 named bits")]
def test_bitfield_8(t : T?) {
    t |> run("8-bit bitfield") @(t : T?) {
        var a = Baz8.bit0
        static_assert(typeinfo sizeof(type<Baz8>) == 1, "8-bit bitfield size is 8 bits")
        static_assert(typeinfo alignof(type<Baz8>) == 1, "8-bit bitfield alignment is 8 bits")
        t |> equal(bitfield8(0x1), a)
        // eq and neq
        t |> success(a == Baz8.bit0)
        t |> success(a != Baz8.bit1)
        // or and set or
        a = a | Baz8.bit2
        t |> equal(bitfield8(0x5), a)
        t |> equal(Baz8.bit0 | Baz8.bit2, a)
        a |= Baz8.bit1
        t |> equal(bitfield8(0x7), a)
        t |> equal(Baz8.bit0 | Baz8.bit1 | Baz8.bit2, a)
        // not(~), and and set and
        a &= ~Baz8.bit1
        t |> equal(bitfield8(0x5), a)
        t |> equal(Baz8.bit0 | Baz8.bit2, a)
        a = a & Baz8.bit0
        t |> equal(bitfield8(0x1), a)
        t |> equal(Baz8.bit0, a)
        // xor and set xor
        a = a ^ Baz8.bit2
        t |> equal(bitfield8(0x5), a)
        t |> equal(Baz8.bit0 | Baz8.bit2, a)
        a ^= Baz8.bit0
        t |> equal(bitfield8(0x4), a)
        t |> equal(Baz8.bit2, a)
        // convert to uint8
        t |> equal(0x4u8, uint8(a))
        // construct from uint8
        a = bitfield8(uint8(0x3))
        t |> equal(Baz8.bit0 | Baz8.bit1, a)
    }
    t |> run("all fields in 8-bit bitfield") @(t : T?) {
        // lets test all fields and printing
        for (i in 0 .. 7) {
            // regular conversions and c-tor
            let baf = BazAllFields8(1 << i)
            t |> equal(bitfield8(1 << i), baf)
            // string conversions
            let s = "{baf}"
            t |> equal("(bit{i})", s)
        }
    }
}

bitfield Baz16 : uint16 {
    bit0
    bit1
    bit2
    bit02 = Baz16.bit0 | Baz16.bit2
}

bitfield BazAllFields16 : uint16 { // this is to test all 16 bits (if they compile)
    bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7,
    bit8, bit9, bit10, bit11, bit12, bit13, bit14, bit15
}

[test(description = "a uint16 bitfield is 2 bytes, supports ==, !=, |, &, ^, ~ and their compound forms, converts to and from uint16, and prints each of its 16 named bits")]
def test_bitfield_16(t : T?) {
    t |> run("16-bit bitfield") @(t : T?) {
        var a = Baz16.bit0
        static_assert(typeinfo sizeof(type<Baz16>) == 2, "16-bit bitfield size is 16 bits")
        static_assert(typeinfo alignof(type<Baz16>) == 2, "16-bit bitfield alignment is 16 bits")
        t |> equal(bitfield16(0x1), a)
        // eq and neq
        t |> success(a == Baz16.bit0)
        t |> success(a != Baz16.bit1)
        // or and set or
        a = a | Baz16.bit2
        t |> equal(bitfield16(0x5), a)
        t |> equal(Baz16.bit0 | Baz16.bit2, a)
        a |= Baz16.bit1
        t |> equal(bitfield16(0x7), a)
        t |> equal(Baz16.bit0 | Baz16.bit1 | Baz16.bit2, a)
        // not(~), and and set and
        a &= ~Baz16.bit1
        t |> equal(bitfield16(0x5), a)
        t |> equal(Baz16.bit0 | Baz16.bit2, a)
        a = a & Baz16.bit0
        t |> equal(bitfield16(0x1), a)
        t |> equal(Baz16.bit0, a)
        // xor and set xor
        a = a ^ Baz16.bit2
        t |> equal(bitfield16(0x5), a)
        t |> equal(Baz16.bit0 | Baz16.bit2, a)
        a ^= Baz16.bit0
        t |> equal(bitfield16(0x4), a)
        t |> equal(Baz16.bit2, a)
        // convert to uint16
        t |> equal(uint16(0x4), uint16(a))
        // construct from uint16
        a = bitfield16(uint16(0x3))
        t |> equal(Baz16.bit0 | Baz16.bit1, a)
    }
    t |> run("all fields in 16-bit bitfield") @(t : T?) {
        // lets test all fields and printing
        for (i in 0 .. 15) {
            // regular conversions and c-tor
            let baf = BazAllFields16(1 << i)
            t |> equal(bitfield16(1 << i), baf)
            // string conversions
            let s = "{baf}"
            t |> equal("(bit{i})", s)
        }
    }
}

bitfield Baz64 : uint64 {
    bit0
    bit1
    bit2
    bit02 = Baz64.bit0 | Baz64.bit2
}

bitfield BazAllFields64 : uint64 { // this is to test all 64 bits (if they compile)
    bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7,
    bit8, bit9, bit10, bit11, bit12, bit13, bit14, bit15,
    bit16, bit17, bit18, bit19, bit20, bit21, bit22, bit23,
    bit24, bit25, bit26, bit27, bit28, bit29, bit30, bit31,
    bit32, bit33, bit34, bit35, bit36, bit37, bit38, bit39,
    bit40, bit41, bit42, bit43, bit44, bit45, bit46, bit47,
    bit48, bit49, bit50, bit51, bit52, bit53, bit54, bit55,
    bit56, bit57, bit58, bit59, bit60, bit61, bit62, bit63
}

[test(description = "a uint64 bitfield is 8 bytes, supports ==, !=, |, &, ^, ~ and their compound forms, converts to and from uint64, and prints each of its 64 named bits")]
def test_bitfield_64(t : T?) {
    t |> run("64-bit bitfield") @(t : T?) {
        var a = Baz64.bit0
        static_assert(typeinfo sizeof(type<Baz64>) == 8, "64-bit bitfield size is 64 bits")
        static_assert(typeinfo alignof(type<Baz64>) == 8, "64-bit bitfield alignment is 64 bits")
        t |> equal(bitfield64(0x1), a)
        // eq and neq
        t |> success(a == Baz64.bit0)
        t |> success(a != Baz64.bit1)
        // or and set or
        a = a | Baz64.bit2
        t |> equal(bitfield64(0x5), a)
        t |> equal(Baz64.bit0 | Baz64.bit2, a)
        a |= Baz64.bit1
        t |> equal(bitfield64(0x7), a)
        t |> equal(Baz64.bit0 | Baz64.bit1 | Baz64.bit2, a)
        // not(~), and and set and
        a &= ~Baz64.bit1
        t |> equal(bitfield64(0x5), a)
        t |> equal(Baz64.bit0 | Baz64.bit2, a)
        a = a & Baz64.bit0
        t |> equal(bitfield64(0x1), a)
        t |> equal(Baz64.bit0, a)
        // xor and set xor
        a = a ^ Baz64.bit2
        t |> equal(bitfield64(0x5), a)
        t |> equal(Baz64.bit0 | Baz64.bit2, a)
        a ^= Baz64.bit0
        t |> equal(bitfield64(0x4), a)
        t |> equal(Baz64.bit2, a)
        // convert to uint64
        t |> equal(uint64(0x4), uint64(a))
        // construct from uint64
        a = bitfield64(uint64(0x3))
        t |> equal(Baz64.bit0 | Baz64.bit1, a)
    }
    t |> run("all fields in 64-bit bitfield") @(t : T?) {
        // lets test all fields and printing
        for (i in 0 .. 63) {
            // regular conversions and c-tor
            let baf = BazAllFields64(1ul << uint64(i))
            t |> equal(bitfield64(1ul << uint64(i)), baf)
            // string conversions
            let s = "{baf}"
            t |> equal("(bit{i})", s)
        }
    }
}
