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

// ----- differential driver -----

var GRID : array<float>

[init]
def fill_grid {
    GRID <- [-100.0, -3.5, -1.0, -0.0, 0.0, 1.0, 2.5, 7.0, 100.0]
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
}
