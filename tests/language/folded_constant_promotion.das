options gen2
options infer_time_folding = true

// In a binary operator a literal adapts to the other operand's type, so `a - 1` reads
// like `a - 1u`; `-1` and `(1 + 1)` are built from literals and adapt the same way
// once infer-time folding collapses them, which this file pins on. A constant folded
// from a typed expression (clz(0xFF), 1u + 1u) keeps its declared type and never
// adapts: the literal side moves.

require dastest/testing_boost public

let CONST_BITS = 0xFF

def typed_uint_on_right(bits : uint) : uint => 31 - clz(bits)

def typed_uint_on_left(bits : uint) : uint => clz(bits) - 31

def minus_one(a : auto(T)) => a - 1

def operator %(a : float; b : int) : float => a + float(b) + 100.0

def mixed_remainder(a : float; b : int) : float => a % b

[test(description = "a constant folded from a typed expression keeps its uint type under infer-time folding, so the literal on the other side adapts to it")]
def test_folded_constant_keeps_its_type(t : T?) {
    t |> run("folded call result keeps uint, literal adapts") @(t : T?) {
        t |> equal(typeinfo typename(31 - clz(0xFF)), "uint const")
        t |> equal(typeinfo typename(clz(0xFF) - 31), "uint const")
        t |> equal(typeinfo typename(31 - clz(CONST_BITS)), "uint const")
        t |> equal(31 - clz(0xFF), 7u)
    }
    t |> run("runtime operand behaves the same as the folded one") @(t : T?) {
        t |> equal(typeinfo typename(typed_uint_on_right(0xFF)), "uint const")
        t |> equal(typed_uint_on_right(0xFF), 7u)
        t |> equal(typeinfo typename(typed_uint_on_left(0xFF)), "uint const")
        t |> equal(typed_uint_on_left(0xFF), 0xfffffff9u)
    }
}

[test(description = "bare, negated and parenthesized literals still promote toward the typed operand under infer-time folding, two literals stay int, and a generic instance keeps the literal mark")]
def test_bare_literal_still_adapts(t : T?) {
    t |> run("literal promotes toward the typed operand") @(t : T?) {
        let ua = 100u
        t |> equal(typeinfo typename(ua - 1), "uint const")
        t |> equal(ua - 1, 99u)
        t |> equal(typeinfo typename(31u - 1), "uint const")
        t |> equal(31u - 1, 30u)
    }
    t |> run("negated and parenthesized literals still promote") @(t : T?) {
        let l = 5l
        let f = 2.0
        let d = 2.0lf
        let u = 7u
        t |> equal(l + -1, 4l)
        t |> equal(f * -1, -2.0)
        t |> equal(d * -2, -4.0lf)
        t |> success(l > -1)
        t |> equal(l & ~1, 4l)
        t |> equal(u * (1 + 1), 14u)
        t |> equal(l + (1 << 2), 9l)
    }
    t |> run("two literals keep the existing int result") @(t : T?) {
        t |> equal(typeinfo typename(31 - 5u), "int const")
        t |> equal(31 - 5u, 26)
        t |> equal(typeinfo typename(31 - (1u + 1u)), "int const")
        t |> equal(31 - (1u + 1u), 29)
    }
    t |> run("a generic instance keeps the literal mark through the body clone") @(t : T?) {
        t |> equal(minus_one(5u), 4u)
        t |> equal(minus_one(5l), 4l)
    }
}

[test(description = "an int constant folded from a call or a typeinfo promotes toward the other operand where the operator would not compile without it, on either side and in a compound assignment")]
def test_folded_int_constant_promotes(t : T?) {
    t |> run("compound assignment takes a cast literal") @(t : T?) {
        var f = 1.5
        f *= 1
        f *= int(2)
        t |> equal(f, 3.0)
        var d = 1.5lf
        d += int(2)
        t |> equal(d, 3.5lf)
        var l = 5l
        l -= int(2)
        t |> equal(l, 3l)
    }
    t |> run("either side of a binary operator") @(t : T?) {
        let f = 2.0
        t |> equal(typeinfo typename(f + int(1)), "float const")
        t |> equal(f + int(1), 3.0)
        t |> equal(int(1) + f, 3.0)
        t |> success(f > int(1))
    }
    t |> run("a folded call and a typeinfo") @(t : T?) {
        let f = 2.0
        t |> equal(f * clz(0xFF), 48.0)
        t |> equal(f * typeinfo sizeof(type<int>), 8.0)
        t |> equal(f * (typeinfo is_pod(type<int>) ? 2 : 3), 4.0)
    }
}

[test(description = "an operator declared on the mixed pair is still the one called when the int side is a folded constant")]
def test_user_operator_beats_folded_promotion(t : T?) {
    let f = 2.0
    t |> equal(mixed_remainder(f, 3), 105.0)
    t |> equal(f % int(3), 105.0)
    t |> equal(f % typeinfo sizeof(type<int>), 106.0)
}

[test(description = "a folded int constant beside a bitfield takes the bitfield type, on either side")]
def test_folded_int_constant_beside_bitfield(t : T?) {
    let x : bitfield = bitfield(1)
    t |> equal(int(x | int(2)), 3)
    t |> equal(int(int(2) | x), 3)
}
