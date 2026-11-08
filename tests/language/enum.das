options gen2
options rtti

require UnitTest

require daslib/enum_trait
require daslib/json_boost
require dastest/testing_boost public

enum Numbers : int16 {
    zero
    one
    two
    ten = 10
    sixteen = 0x10
    neg_ten = -10
    neg_one = 0xffffffff
    neg_one_i = -1
}

enum Zen {
    zero
    not_zero
}

struct Foo {
    a : int
    b : float
}

enum QQ {
    size_of_Foo_Padded = typeinfo sizeof(type<Foo>) + 8
}

enum BooHoo {
    boo
    hoo
    boo_hoo = 5
}

enum BooHoo8 : int8 {
    boo = 1
    hoo = 2
    boo_hoo = 3
}

enum BooHoo16 : int16 {
    boo = 4
    hoo = 5
    boo_hoo = 6
}

enum BooHoo64 : int64 {
    boo = 7l
    hoo = 8l
    boo_hoo = 9l
}

// Exercises high bits beyond int32 to lock in the int64-width cast in
// enum_trait's operator !/bool() — a naive int() cast would truncate
// (1ul << 63) to zero and incorrectly report the value as "no bits set".
enum BooHooU64 : uint64 {
    zero = 0ul
    low_bit = 1ul
    high_bit = 0x8000000000000000ul   // 2^63 — bit 63 set, all bits 0..62 zero
}

// Auto-increment after an expression initializer (lastEnuValue path) —
// requires infer-time folding to be on for the expression visit so that
// the +1 path sees a real ExprConstInt rather than the unfolded ExprOp1.
enum AutoIncrAfterExpr {
    first = -10
    second   // expected: -9 via lastEnuValue + 1
    third    // expected: -8
}

[sideeffects]
def getZenOne => Zen.not_zero

[export, unused_argument(arg)]
def testBindEnum(arg : SomeEnum) {
    feint("hello, world!\n")
}

[test(description = "enum values, defaults, comparisons, typeinfo-sized initializers, iteration over type<Enum> and C++-bound enums of every width behave as declared")]
def test_enum(t : T?) {
    t |> equal(int(QQ.size_of_Foo_Padded), 16)
    t |> equal(int(Numbers.zero), 0)
    t |> equal(int(Numbers.two), 2)
    t |> equal(int(Numbers.ten), 10)
    t |> equal(int(Numbers.sixteen), 16)
    t |> equal(int(Numbers.neg_ten), -10)
    t |> equal(int(Numbers.neg_one), -1)
    t |> equal(int(Numbers.neg_one_i), -1)
    let n : Numbers
    t |> equal(n, Numbers.zero)
    var q = Numbers.one
    t |> equal(int(q), 1)
    t |> equal(q, Numbers.one)
    t |> success(n != q)
    q = Numbers.zero
    t |> equal(n, q)
    t |> success(n != Numbers.one)
    t |> success(Zen.zero != Zen.not_zero)
    t |> equal(Zen.zero, Zen.zero)
    t |> equal(int(n), 0)
    t |> equal(int(Zen.not_zero), 1)
    t |> equal(getZenOne(), Zen.not_zero)
    t |> equal(free_takeOne_giveTwo(SomeEnum.one), SomeEnum.two)
    t |> equal(free_takeOne_giveTwo(SomeEnum.two), SomeEnum.zero)
    t |> equal(efn_takeOne_giveTwo(SomeEnum.one), SomeEnum.two)
    t |> equal(efn_takeOne_giveTwo_98(SomeEnum98.SomeEnum98_one), SomeEnum98.SomeEnum98_two)

    let result_efn = efn_takeOne_giveTwo(SomeEnum.one)
    t |> equal(result_efn, SomeEnum.two)

    let goo : GooEnum = GooEnum.regular
    t |> equal(efn_flip(goo), GooEnum.hazardous)
    var foo16 : TestObjectFoo
    t |> equal(foo16.e16, SomeEnum_16.SomeEnum_16_zero)
    foo16.e16 = SomeEnum_16.SomeEnum_16_one
    t |> equal(foo16.e16, SomeEnum_16.SomeEnum_16_one)
    t |> equal(int(foo16.e16), 1)
    t |> equal(typeinfo sizeof(foo16.e16), 2)
    for (e, tt in type<BooHoo>, fixed_array<int>(0, 1, 5)) {
        t |> equal(int(e), tt)
    }
    for (e, tt in type<BooHoo8>, fixed_array<int>(1, 2, 3)) {
        t |> equal(int(e), tt)
    }
    for (e, tt in type<BooHoo16>, fixed_array<int>(4, 5, 6)) {
        t |> equal(int(e), tt)
    }
    // binds
    let bind_ok = testBindEnumFunction()
    t |> success(bind_ok, "testBindEnumFunction failed")
}

[test(description = "enum_trait's operator ! and bool() test the full-width underlying value of any enum, bit 63 of a uint64 enum included, and auto-increment after an expression initializer counts on from its value")]
def test_enum_bool_not_ops(t : T?) {
    // daslib/enum_trait.das `operator !(arg)` and `bool(arg)` for any enum:
    // returns true/false based on the underlying integer being zero / non-zero.
    // Enables flag-op-enum read-side: `if (bool(flags & MASK))` / `if (!(flags & MASK))`.

    // Zen — default int32 backing
    t |> success(!Zen.zero, "!Zen.zero (value 0) must be true")
    t |> success(!!Zen.not_zero, "!!Zen.not_zero (value 1) must be true")
    t |> success(!bool(Zen.zero), "bool(Zen.zero) must be false")
    t |> success(bool(Zen.not_zero), "bool(Zen.not_zero) must be true")

    // Numbers — int16 backing, including negative-valued members
    t |> success(!Numbers.zero, "!Numbers.zero must be true")
    t |> success(!!Numbers.one, "Numbers.one is non-zero")
    t |> success(!bool(Numbers.zero), "bool(Numbers.zero) must be false")
    t |> success(bool(Numbers.one), "bool(Numbers.one) must be true")
    t |> success(bool(Numbers.neg_one), "bool(Numbers.neg_one) must be true (non-zero)")
    t |> success(bool(Numbers.neg_ten), "bool(Numbers.neg_ten) must be true (non-zero)")

    // BooHoo8 — int8 backing, no zero member, all values non-zero
    t |> success(bool(BooHoo8.boo), "bool(BooHoo8.boo=1) must be true")
    t |> success(!!BooHoo8.hoo, "BooHoo8.hoo=2 is non-zero")
    t |> success(!(!BooHoo8.boo_hoo), "BooHoo8.boo_hoo=3 is non-zero")

    // BooHoo16 — int16 backing, same shape
    t |> success(bool(BooHoo16.boo), "bool(BooHoo16.boo=4) must be true")
    t |> success(!!BooHoo16.boo_hoo, "BooHoo16.boo_hoo=6 is non-zero")

    // BooHoo64 — int64 backing
    t |> success(bool(BooHoo64.boo), "bool(BooHoo64.boo=7) must be true")

    // BooHooU64 — uint64 backing, exercises bit 63 (the truncation hazard).
    // A naive `int(arg)` cast would chop high bits and read `high_bit` as 0.
    t |> success(!BooHooU64.zero, "!BooHooU64.zero must be true")
    t |> success(!bool(BooHooU64.zero), "bool(BooHooU64.zero) must be false")
    t |> success(bool(BooHooU64.low_bit), "bool(BooHooU64.low_bit) must be true")
    t |> success(bool(BooHooU64.high_bit),
        "bool(BooHooU64.high_bit=2^63) must be true (int-truncation regression check)")
    t |> success(!!BooHooU64.high_bit,
        "!!BooHooU64.high_bit=2^63 must be true (int-truncation regression check)")

    // Auto-increment after expression initializer — confirms preVisitEnumerationValue
    // folds the `-10` literal so lastEnuValue carries a real ExprConstInt into the
    // +1 path. (Pre-fix: `error[30147] must be integer constant, and not an expression`
    // under lint policies; post-fix: AutoIncrAfterExpr.second resolves to -9.)
    t |> equal(int(AutoIncrAfterExpr.first), -10)
    t |> equal(int(AutoIncrAfterExpr.second), -9)
    t |> equal(int(AutoIncrAfterExpr.third), -8)
}

// Casts of uint8/uint16/uint64-backed enums must zero-extend, not sign-extend.
// Signed-underlying enums must keep their existing sign-extending bit-reinterpret
// semantics (e.g. uint(int8_value=-50) stays 0xffffffce).

// Natural opcode byte — uint8 is the canonical choice for compact packed enums.
enum Opcode : uint8 {
    NOP = 0u8
    PING = 50u8
    HEARTBEAT = 200u8       // > 127, triggers the bug
    SHUTDOWN = 254u8
}

// Like a port id or compact channel — uint16 representable.
enum Channel : uint16 {
    SYS = 100ul
    USER = 5000ul
    BROADCAST = 60000ul     // > 32767, triggers the bug
}

// uint32-backed — the WalkEnumeration (32-bit) path also needed the zero-extend fix.
enum Mask32 : uint {
    NONE = 0u
    LOW = 0x1u
    HIGH_BIT = 0x80000000u  // > INT32_MAX, exercises the WalkEnumeration (32-bit) fix
}

// uint64-backed — exercises the EnumStub64u binding (cast helpers, ==/!= policies, isSameType
// gating for tEnumeration64). The HIGH_BIT value has the int64 sign bit set, so any leftover
// sign-vs-unsigned mismatch in the lookup paths surfaces here.
enum Wide64 : uint64 {
    LOW = 100ul
    HIGH_BIT = 0x8000000000000000ul   // 2^63 — top bit set
}

// Signed counterparts — behaviour preserved. Hex literals avoid the unary-minus form
// (daslang's lint pass treats `-N` as an expression, not an integer constant, in enum values).
enum Signed8 : int8 {
    LOW = 50
    NEGATIVE = 0xce              // int8 bit-reinterpret = -50
}

enum Signed16 : int16 {
    LOW = 1000l
    NEGATIVE = 0xfc18l           // int16 bit-reinterpret = -1000
}

[test(description = "casts of uint8, uint16 and uint64-backed enums zero-extend to wider integers, keeping values past the signed range")]
def test_unsigned_underlying_zero_extends(t : T?) {
    t |> run("uint8-backed enum cast preserves byte value") @(t : T?) {
        t |> equal(uint(Opcode.HEARTBEAT), 200u)
        t |> equal(int(Opcode.HEARTBEAT), 200)
        t |> equal(int64(Opcode.SHUTDOWN), 254l)
        t |> equal(uint64(Opcode.SHUTDOWN), 254ul)
    }
    t |> run("uint16-backed enum cast preserves half-word value") @(t : T?) {
        t |> equal(uint(Channel.BROADCAST), 60000u)
        t |> equal(int(Channel.BROADCAST), 60000)
        t |> equal(int64(Channel.BROADCAST), 60000l)
    }
    t |> run("uint64-backed enum cast preserves bit pattern (top bit set)") @(t : T?) {
        t |> equal(uint64(Wide64.HIGH_BIT), 0x8000000000000000ul)
        // int64(uint64-enum) is a bit-reinterpret in daslang convention — top bit set → INT64_MIN
        // (written as -INT64_MAX - 1 because INT64_MIN as a literal overflows the int64 parser).
        t |> equal(int64(Wide64.HIGH_BIT), -9223372036854775807l - 1l)
        t |> equal(uint64(Wide64.LOW), 100ul)
    }
}

[test(description = "casts of int8 and int16-backed enums keep sign-extending, so a negative value reads negative and reinterprets as a high uint")]
def test_signed_underlying_unchanged(t : T?) {
    t |> run("int8-backed enum cast keeps sign-reinterpret") @(t : T?) {
        t |> equal(int(Signed8.NEGATIVE), -50)
        t |> equal(uint(Signed8.NEGATIVE), 0xffffffceu)
    }
    t |> run("int16-backed enum cast keeps sign-reinterpret") @(t : T?) {
        t |> equal(int(Signed16.NEGATIVE), -1000)
    }
}

// Runtime path — defeats const folding so we exercise the SimNode side.
[test(description = "unsigned enum values read from an array at run time zero-extend in casts, past const folding")]
def test_runtime_iteration(t : T?) {
    t |> run("runtime: for-loop sum of uint8 enums") @(t : T?) {
        var arr <- [Opcode.PING, Opcode.HEARTBEAT]
        var sum = 0u
        for (op in arr) {
            sum += uint(op)
        }
        t |> equal(sum, 250u)               // 50 + 200
    }
    t |> run("runtime: uint64 enum cast through array iteration (top bit set)") @(t : T?) {
        var arr <- [Wide64.LOW, Wide64.HIGH_BIT]
        var hit_high = false
        for (w in arr) {
            if (w == Wide64.HIGH_BIT) {
                hit_high = true
                t |> equal(uint64(w), 0x8000000000000000ul)
            }
        }
        t |> equal(hit_high, true)
    }
}

// Equality on uint-backed enums — the isSameType tightening must NOT remove the ==/!= operator.
[test(description = "== and != compare uint8, uint16, uint32 and uint64-backed enum values correctly, top bit set included")]
def test_unsigned_enum_equality(t : T?) {
    t |> run("uint8 enum == operator works") @(t : T?) {
        let a = Opcode.HEARTBEAT
        let b = Opcode.HEARTBEAT
        let c = Opcode.PING
        t |> equal(a == b, true)
        t |> equal(a == c, false)
        t |> equal(a != c, true)
    }
    t |> run("uint16 enum == operator works") @(t : T?) {
        let a = Channel.BROADCAST
        let b = Channel.BROADCAST
        let c = Channel.SYS
        t |> equal(a == b, true)
        t |> equal(a != c, true)
    }
    t |> run("uint32 enum == operator works") @(t : T?) {
        let a = Mask32.HIGH_BIT
        let b = Mask32.HIGH_BIT
        let c = Mask32.LOW
        t |> equal(a == b, true)
        t |> equal(a != c, true)
    }
    t |> run("uint64 enum == operator works (top bit set)") @(t : T?) {
        let a = Wide64.HIGH_BIT
        let b = Wide64.HIGH_BIT
        let c = Wide64.LOW
        t |> equal(a == b, true)
        t |> equal(a == c, false)
        t |> equal(a != c, true)
    }
}

// Struct field access — exercises SimNode_GetField lowering.
struct Pkt {
    op : Opcode
}

struct Pkt64 {
    w : Wide64
}

[test(description = "an unsigned enum read from a struct field zero-extends when cast")]
def test_struct_field_cast(t : T?) {
    t |> run("struct field of uint8 enum, cast to uint") @(t : T?) {
        let p = Pkt(op = Opcode.HEARTBEAT)
        t |> equal(uint(p.op), 200u)
    }
    t |> run("struct field of uint64 enum, cast to uint64") @(t : T?) {
        let p = Pkt64(w = Wide64.HIGH_BIT)
        t |> equal(uint64(p.w), 0x8000000000000000ul)
    }
}

// String interpolation goes through WalkEnumeration8 / Enum() value-name lookup.
[test(description = "string interpolation prints the value name of uint8, uint16, uint32 and uint64-backed enums whose values exceed the signed range")]
def test_string_interpolation_names(t : T?) {
    t |> run("uint8 enum interpolates value name") @(t : T?) {
        t |> equal("{Opcode.HEARTBEAT}", "HEARTBEAT")
    }
    t |> run("uint16 enum interpolates value name") @(t : T?) {
        t |> equal("{Channel.BROADCAST}", "BROADCAST")
    }
    t |> run("uint32 enum interpolates value name (high bit set)") @(t : T?) {
        t |> equal("{Mask32.HIGH_BIT}", "HIGH_BIT")
    }
    t |> run("uint64 enum interpolates value name (top bit set)") @(t : T?) {
        t |> equal("{Wide64.HIGH_BIT}", "HIGH_BIT")
    }
}

// JSON serialization of a struct containing a uint-backed enum field — exercises
// json_print.cpp's WalkEnumeration*/Enum() value-name lookup. Before the fix the
// uint8 byte 0xC8 was read as int8(-56) and the lookup against the int64-stored
// field value (200) missed, producing {"op":} with an empty value.
struct PktU8 {
    op : Opcode
}

struct PktU16 {
    ch : Channel
}

struct PktU32 {
    mk : Mask32
}

struct PktU64 {
    w : Wide64
}

[test(description = "sprint_json renders an unsigned enum struct field by its value name for every width, top bit set included")]
def test_sprint_json_unsigned_enum_field(t : T?) {
    t |> run("sprint_json: uint8 enum HEARTBEAT renders by name") @(t : T?) {
        let p = PktU8(op = Opcode.HEARTBEAT)
        t |> equal(sprint_json(p, false), "\{\"op\":\"HEARTBEAT\"\}")
    }
    t |> run("sprint_json: uint16 enum BROADCAST renders by name") @(t : T?) {
        let p = PktU16(ch = Channel.BROADCAST)
        t |> equal(sprint_json(p, false), "\{\"ch\":\"BROADCAST\"\}")
    }
    t |> run("sprint_json: uint32 enum HIGH_BIT renders by name") @(t : T?) {
        let p = PktU32(mk = Mask32.HIGH_BIT)
        t |> equal(sprint_json(p, false), "\{\"mk\":\"HIGH_BIT\"\}")
    }
    t |> run("sprint_json: uint64 enum HIGH_BIT renders by name (top bit set)") @(t : T?) {
        let p = PktU64(w = Wide64.HIGH_BIT)
        t |> equal(sprint_json(p, false), "\{\"w\":\"HIGH_BIT\"\}")
    }
}
