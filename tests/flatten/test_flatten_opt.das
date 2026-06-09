options gen2
options indenting = 4
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/flatten
require math

//! Differential net for the flatten OPTIMIZE arc (preshader extraction + CSE).
//! Both passes are VALUE-EXACT — they only hoist and dedup pure subtrees, never
//! regroup or round — so every twin compares with exact `equal` (unlike the
//! reassoc arc, which reserves `equal_approx`). The FIRED proof (a `_preshader_`
//! let on top, a shared `_cse_` let) lives in `no_aot/test_flatten_fold.das`
//! (empty `flatten_opt_residuals`) and the cap_preshader / cap_cse node-count
//! shaders; here we only assert the optimization preserved the result.

// Module globals are UNIFORM (preshader-eligible); function params are VARYING.
var G_GAIN = 2.5
var G_BIAS = -0.75
var G_TINT = float3(0.3, 0.59, 0.11)

// ----- preshader extraction: whole-uniform let -----
[flatten]
def p_whole(x : float) : float {
    let u = G_GAIN * 2.0 + 1.0
    return x * u + u
}

// ----- preshader: a uniform subtree BURIED under a varying parent -----
[flatten]
def p_buried(x : float3) : float3 {
    return x * (dot(G_TINT, float3(1.0, 1.0, 1.0)) + G_GAIN)
}

// ----- preshader: uniform local feeding another uniform local (topological order) -----
[flatten]
def p_chain(x : float) : float {
    let a = G_GAIN * 2.0
    let b = a + G_BIAS
    return x * b
}

// ----- preshader: vector-typed uniform -----
[flatten]
def p_vec(x : float3) : float3 {
    let tint = G_TINT * G_GAIN + float3(G_BIAS, G_BIAS, G_BIAS)
    return x + tint
}

// ----- CSE: a varying subtree computed twice -----
[flatten]
def c_var(x : float) : float {
    let a = (x * x + 1.0) * 2.0
    let b = (x * x + 1.0) * 3.0
    return a + b
}

// ----- CSE: commutative-reordered dup must match (reassoc canonical key) -----
[flatten]
def c_commute(x : float; y : float) : float {
    let a = x * y + 1.0
    let b = y * x + 2.0
    return a + b
}

// ----- CSE: a uniform dup → shared let in the preshader region -----
[flatten]
def c_uni(x : float) : float {
    let p = G_GAIN * G_GAIN + 1.0
    let q = (G_GAIN * G_GAIN + 1.0) * 2.0
    return x * p + x * q
}

var GRID : array<float>

[init]
def fill_grid {
    GRID <- [-100.0, -3.5, -1.0, 0.0, 1.0, 2.5, 7.0, 100.0]
}

def cmp3f(t : T?; a, b : float3) {
    t |> equal(a.x, b.x); t |> equal(a.y, b.y); t |> equal(a.z, b.z)
}

[test]
def test_flatten_opt(t : T?) {
    t |> run("preshader: whole-uniform let -> _preshader_") @(t : T?) {
        for (v in GRID) {
            t |> equal(p_whole_flat(v), p_whole(v))
        }
    }
    t |> run("preshader: buried uniform subtree in return") @(t : T?) {
        for (v in GRID) {
            cmp3f(t, p_buried_flat(float3(v, v * 0.5, -v)), p_buried(float3(v, v * 0.5, -v)))
        }
    }
    t |> run("preshader: uniform local chain (topological order)") @(t : T?) {
        for (v in GRID) {
            t |> equal(p_chain_flat(v), p_chain(v))
        }
    }
    t |> run("preshader: vector-typed uniform") @(t : T?) {
        for (v in GRID) {
            cmp3f(t, p_vec_flat(float3(v, -v, v * 2.0)), p_vec(float3(v, -v, v * 2.0)))
        }
    }
    t |> run("CSE: varying dup -> _cse_") @(t : T?) {
        for (v in GRID) {
            t |> equal(c_var_flat(v), c_var(v))
        }
    }
    t |> run("CSE: commutative-reordered dup matches") @(t : T?) {
        for (v in GRID) {
            t |> equal(c_commute_flat(v, v * 0.5 - 1.0), c_commute(v, v * 0.5 - 1.0))
        }
    }
    t |> run("CSE: uniform dup -> preshader region") @(t : T?) {
        for (v in GRID) {
            t |> equal(c_uni_flat(v), c_uni(v))
        }
    }
}
