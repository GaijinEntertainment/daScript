options gen2
options infer_time_folding = false

require dastest/testing_boost public

def operator %(a : float; b : int) : float => a + float(b) + 100.0

def mixed_remainder(a : float; b : int) : float => a % b

[test(description = "with infer-time folding off, the lint, LSP and MCP compile policy, an int constant from a call or an operator still promotes toward the other operand, on either side and in a compound assignment")]
def test_unfolded_int_constant_promotes(t : T?) {
    t |> run("compound assignment takes a cast literal") @(t : T?) {
        var f = 1.5
        f *= int(2)     // nolint:PERF020 -- the cast is under test
        t |> equal(f, 3.0)
        var l = 5l
        l -= int(2)     // nolint:PERF020 -- the cast is under test
        t |> equal(l, 3l)
    }
    t |> run("either side of a binary operator") @(t : T?) {
        let f = 2.0
        t |> equal(typeinfo typename(f + int(1)), "float const")
        t |> equal(int(1) + f, 3.0)     // nolint:PERF020 -- the cast is under test
        t |> equal(f * clz(0xFF), 48.0)
        t |> equal(f * typeinfo sizeof(type<int>), 8.0)
        t |> equal(f * (typeinfo is_pod(type<int>) ? 2 : 3), 4.0)
    }
    t |> run("an operator over literals adapts as a literal does") @(t : T?) {
        let u = 7u
        let l = 5l
        t |> equal(u * (1 + 1), 14u)
        t |> equal(l + (1 << 2), 9l)
    }
    t |> run("a computed constant beside a literal keeps its type") @(t : T?) {
        t |> equal(typeinfo typename(31 - clz(0xFF)), "uint const")
        t |> equal(31 - clz(0xFF), 7u)
    }
}

[test(description = "with infer-time folding off, an operator declared on the mixed pair is still the one called when the int side is a constant")]
def test_user_operator_beats_unfolded_promotion(t : T?) {
    let f = 2.0
    t |> equal(mixed_remainder(f, 3), 105.0)
    t |> equal(f % int(3), 105.0)     // nolint:PERF020 -- the cast is under test
    t |> equal(f % typeinfo sizeof(type<int>), 106.0)
}

[test(description = "with infer-time folding off, an int constant beside a bitfield takes the bitfield type, on either side")]
def test_unfolded_int_constant_beside_bitfield(t : T?) {
    let x : bitfield = bitfield(1)
    t |> equal(int(x | int(2)), 3)     // nolint:PERF020 -- the cast is under test
    t |> equal(int(int(2) | x), 3)     // nolint:PERF020 -- the cast is under test
}
