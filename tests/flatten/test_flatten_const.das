options gen2
options indenting = 4
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/flatten

//! Differential tests for the flatten const-optimization arc: each `[flatten]`
//! function carries a fold pattern (float algebraic identities here); its `_flat`
//! twin must compute identical results over a FINITE input grid. On finite inputs
//! the lossy float folds (`x*0`, `x+0`, `x-0`) are value-exact (`finite*0 == 0`,
//! `x+0 == x`), so plain `equal` suffices — `equal_approx` is reserved for the
//! reassociation arc, where rounding genuinely differs. (The `_flat` twin is also
//! proven to have actually folded via the cap_fold_* shaders' opcode-count checks.)

// ----- float algebraic identities (#1a) -----

[flatten]
def f_mul1(x : float) : float {
    return x * 1.0
}

[flatten]
def f_mul1l(x : float) : float {
    return 1.0 * x
}

[flatten]
def f_mulneg1(x : float) : float {
    return x * -1.0
}

[flatten]
def f_mul0(x : float) : float {
    return x * 0.0
}

[flatten]
def f_add0(x : float) : float {
    return x + 0.0
}

[flatten]
def f_sub0(x : float) : float {
    return x - 0.0
}

[flatten]
def f_zerosub(x : float) : float {
    return 0.0 - x
}

// Chained identities collapse bottom-up: (x*1) + (x*0) - 0 -> x + 0 - 0 -> x.
[flatten]
def f_combo(x : float) : float {
    return (x * 1.0) + (x * 0.0) - 0.0
}

// Vector * scalar-0: the `*0` fold is gated to a scalar result, so this must NOT
// fold to a scalar zero (that would mistype float3 -> float). It stays `x * 0.0`
// and still computes float3(0,0,0); the test's value is that it COMPILES (the gate
// held) and is value-correct.
[flatten]
def f_vec1(x : float3) : float3 {
    return x * 1.0
}

[flatten]
def f_vec0(x : float3) : float3 {
    return x * 0.0
}

// ----- const-constructor folding (#1c): vector ctors over const args fold to a
// single const literal (the typer leaves them; flatten collapses them) -----

[flatten]
def f_ctor_zero : float3 {
    return float3(0.0)
}

[flatten]
def f_ctor_arith : float3 {
    return float3(2.0 * 3.0)
}

[flatten]
def f_ctor_mixed : float3 {
    return float3(1.0, 2.0, 3.0)
}

// A const ctor buried under arithmetic with a runtime operand: the ctor folds, the
// surrounding add stays.
[flatten]
def f_ctor_mixed_rt(x : float3) : float3 {
    return x + float3(0.5)
}

// ----- boolean algebraic identities (&&, ||, !, bool-select) -----
//
// `true && x` / `x || true` etc. carry a const operand the typer leaves alone (it
// folds only fully-const ops). In a no-early-return twin the lowering's mask-collapse
// pass folds them; the early-return variants below (f_*_early) reassign the live mask,
// so that pass bails and the post-infer FoldVisitor is the sole folder — exercising the
// new arms directly. Differential proves the fold is value-preserving either way.

[flatten]
def f_and_tl(x : bool) : bool {
    return true && x
}

[flatten]
def f_and_tr(x : bool) : bool {
    return x && true
}

[flatten]
def f_and_fl(x : bool) : bool {
    return false && x
}

[flatten]
def f_and_fr(x : bool) : bool {
    return x && false
}

[flatten]
def f_or_fl(x : bool) : bool {
    return false || x
}

[flatten]
def f_or_fr(x : bool) : bool {
    return x || false
}

[flatten]
def f_or_tl(x : bool) : bool {
    return true || x
}

[flatten]
def f_or_tr(x : bool) : bool {
    return x || true
}

[flatten]
def f_not_t : bool {
    return !true
}

[flatten]
def f_not_f : bool {
    return !false
}

[flatten]
def f_sel_tf(c : bool) : bool {
    return c ? true : false
}

[flatten]
def f_sel_ft(c : bool) : bool {
    return c ? false : true
}

// Early-return variants: the early return reassigns the live mask, so the mask-collapse
// pass bails (empty const map) and the post-infer FoldVisitor folds the boolean. Scalar
// return type auto-zero-inits, so no struct-init issue.
[flatten]
def f_and_early(x : bool; g : bool) : bool {
    if (g) {
        return true && x
    }
    return x || true
}

[flatten]
def f_sel_early(c : bool; g : bool) : bool {
    if (g) {
        return c ? true : false
    }
    return c ? false : true
}

// ----- const-propagation of single-def locals via SSA-rename (#2) -----
//
// A reassigned local is SSA-renamed to single-def versions at cycle 1, then the post-
// infer const-prop folds the chain. Differential proves the fold is value-preserving
// (an unfolded accumulator computes the same), incl. the soundness case where the
// accumulator depends on a parameter and must NOT fold. The cap_fold_accumulator shader
// proves it actually fires (0 add nodes reach the backend).

// Pure const accumulator chain -> a constant.
[flatten]
def f_acc_const : int {
    var acc = 5
    acc = acc + 3
    acc = acc * 2
    return acc
}

// Const loop accumulator (unrolled then folded): 0+0+1+2+3 = 6.
[flatten]
def f_acc_loop : int {
    var acc = 0
    for (i in range(4)) {
        acc = acc + i
    }
    return acc
}

// Const prefix folds, runtime tail stays: 30 + x.
[flatten]
def f_acc_mixed(x : int) : int {
    var acc = 1
    acc = acc + 2
    acc = acc * 10
    return acc + x
}

// Accumulator seeded from a parameter must NOT fold: (x + 5) * 2.
[flatten]
def f_acc_runtime(x : int) : int {
    var acc = x
    acc = acc + 5
    acc = acc * 2
    return acc
}

// Float const loop accumulator: 0.25 * 4 = 1.0.
[flatten]
def f_acc_float : float {
    var acc = 0.0
    for (_i in range(4)) {
        acc = acc + 0.25
    }
    return acc
}

// Const accumulator across an early return: g ? 10 : 15.
[flatten]
def f_acc_early(g : bool) : int {
    var acc = 10
    if (g) {
        return acc
    }
    acc = acc + 5
    return acc
}

// ----- differential driver -----

var GRID : array<float>
var BOOLS : array<bool>
var IGRID : array<int>

[init]
def fill_grid {
    GRID <- [-100.0, -3.5, -1.0, -0.0, 0.0, 1.0, 2.5, 7.0, 100.0]
    BOOLS <- [true, false]
    IGRID <- [-100, -7, -1, 0, 1, 3, 42, 1000]
}

[test]
def test_flatten_const(t : T?) {
    t |> run("x * 1.0 -> x") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_mul1_flat(v), f_mul1(v))
        }
    }
    t |> run("1.0 * x -> x") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_mul1l_flat(v), f_mul1l(v))
        }
    }
    t |> run("x * -1.0 -> -x") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_mulneg1_flat(v), f_mulneg1(v))
        }
    }
    t |> run("x * 0.0 -> 0 (finite-exact)") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_mul0_flat(v), f_mul0(v))
        }
    }
    t |> run("x + 0.0 -> x") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_add0_flat(v), f_add0(v))
        }
    }
    t |> run("x - 0.0 -> x") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_sub0_flat(v), f_sub0(v))
        }
    }
    t |> run("0.0 - x -> -x") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_zerosub_flat(v), f_zerosub(v))
        }
    }
    t |> run("chained identities collapse") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_combo_flat(v), f_combo(v))
        }
    }
    t |> run("float3 * 1.0 -> x (vector)") @(t : T?) {
        for (v in GRID) {
            let a = f_vec1(float3(v, v + 1.0, v - 1.0))
            let b = f_vec1_flat(float3(v, v + 1.0, v - 1.0))
            t |> equal(a.x, b.x)
            t |> equal(a.y, b.y)
            t |> equal(a.z, b.z)
        }
    }
    t |> run("float3 * 0.0 stays vector (gate held, no scalar mistype)") @(t : T?) {
        for (v in GRID) {
            let a = f_vec0(float3(v, v + 1.0, v - 1.0))
            let b = f_vec0_flat(float3(v, v + 1.0, v - 1.0))
            t |> equal(a.x, b.x)
            t |> equal(a.y, b.y)
            t |> equal(a.z, b.z)
        }
    }
    t |> run("const ctor float3(0.0) folds to a const") @(t : T?) {
        let a = f_ctor_zero()
        let b = f_ctor_zero_flat()
        t |> equal(a.x, b.x); t |> equal(a.y, b.y); t |> equal(a.z, b.z)
    }
    t |> run("const ctor float3(2*3) folds to float3(6)") @(t : T?) {
        let a = f_ctor_arith()
        let b = f_ctor_arith_flat()
        t |> equal(a.x, b.x); t |> equal(b.x, 6.0)
    }
    t |> run("const ctor float3(1,2,3) folds") @(t : T?) {
        let a = f_ctor_mixed()
        let b = f_ctor_mixed_flat()
        t |> equal(a.x, b.x); t |> equal(a.y, b.y); t |> equal(a.z, b.z)
    }
    t |> run("const ctor under runtime add") @(t : T?) {
        for (v in GRID) {
            let a = f_ctor_mixed_rt(float3(v, v + 1.0, v - 1.0))
            let b = f_ctor_mixed_rt_flat(float3(v, v + 1.0, v - 1.0))
            t |> equal(a.x, b.x); t |> equal(a.y, b.y); t |> equal(a.z, b.z)
        }
    }
    t |> run("true && x -> x ; x && true -> x") @(t : T?) {
        for (b in BOOLS) {
            t |> equal(f_and_tl_flat(b), f_and_tl(b))
            t |> equal(f_and_tr_flat(b), f_and_tr(b))
            t |> equal(f_and_tl_flat(b), b)
        }
    }
    t |> run("false && x -> false ; x && false -> false") @(t : T?) {
        for (b in BOOLS) {
            t |> equal(f_and_fl_flat(b), f_and_fl(b))
            t |> equal(f_and_fr_flat(b), f_and_fr(b))
            t |> equal(f_and_fl_flat(b), false)
        }
    }
    t |> run("false || x -> x ; x || false -> x") @(t : T?) {
        for (b in BOOLS) {
            t |> equal(f_or_fl_flat(b), f_or_fl(b))
            t |> equal(f_or_fr_flat(b), f_or_fr(b))
            t |> equal(f_or_fl_flat(b), b)
        }
    }
    t |> run("true || x -> true ; x || true -> true") @(t : T?) {
        for (b in BOOLS) {
            t |> equal(f_or_tl_flat(b), f_or_tl(b))
            t |> equal(f_or_tr_flat(b), f_or_tr(b))
            t |> equal(f_or_tl_flat(b), true)
        }
    }
    t |> run("!true -> false ; !false -> true") @(t : T?) {
        t |> equal(f_not_t_flat(), f_not_t())
        t |> equal(f_not_f_flat(), f_not_f())
        t |> equal(f_not_t_flat(), false)
        t |> equal(f_not_f_flat(), true)
    }
    t |> run("c ? true : false -> c ; c ? false : true -> !c") @(t : T?) {
        for (b in BOOLS) {
            t |> equal(f_sel_tf_flat(b), b)
            t |> equal(f_sel_ft_flat(b), !b)
        }
    }
    t |> run("boolean fold under early return (FoldVisitor path)") @(t : T?) {
        for (g in BOOLS) {
            for (b in BOOLS) {
                t |> equal(f_and_early_flat(b, g), f_and_early(b, g))
                t |> equal(f_sel_early_flat(b, g), f_sel_early(b, g))
            }
        }
    }
    t |> run("const accumulator chain -> constant") @(t : T?) {
        t |> equal(f_acc_const_flat(), f_acc_const())
        t |> equal(f_acc_const_flat(), 16)
    }
    t |> run("const loop accumulator -> constant") @(t : T?) {
        t |> equal(f_acc_loop_flat(), f_acc_loop())
        t |> equal(f_acc_loop_flat(), 6)
    }
    t |> run("const prefix folds, runtime tail stays") @(t : T?) {
        for (x in IGRID) {
            t |> equal(f_acc_mixed_flat(x), f_acc_mixed(x))
            t |> equal(f_acc_mixed_flat(x), 30 + x)
        }
    }
    t |> run("param-seeded accumulator must NOT fold (soundness)") @(t : T?) {
        for (x in IGRID) {
            t |> equal(f_acc_runtime_flat(x), f_acc_runtime(x))
            t |> equal(f_acc_runtime_flat(x), (x + 5) * 2)
        }
    }
    t |> run("float const loop accumulator -> 1.0") @(t : T?) {
        t |> equal(f_acc_float_flat(), f_acc_float())
        t |> equal(f_acc_float_flat(), 1.0)
    }
    t |> run("const accumulator across early return") @(t : T?) {
        for (g in BOOLS) {
            t |> equal(f_acc_early_flat(g), f_acc_early(g))
        }
    }
}
