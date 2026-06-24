options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/flatten
require math

//! Differential net for the PHASE_FUSED horizontal-sum re-pack: a sum of >=4 independent scalars
//! (no shared vector base, so the dot fold above can't reach them) collapses into `hadd(float4(...))`
//! — one combine + one hadd where the chain had >=3 adds. Only full groups of four pack (a strict
//! node win); a remainder of 1-3 and any `*`-weighted term (mad fuses those better) stay scalar. The
//! hadd reorders the adds, so floats compare approx (same contract as the dot/mad corpus). The
//! structural FIRED half (a `hadd(` call in the twin, and its absence on the guarded shapes) lives
//! in no_aot/test_flatten_fold.das.

// ----- pure 4-way sum -> hadd(float4(...)) -----
[flatten]
def h_four(a, b, c, d : float) : float {
    return a + b + c + d
}

// ----- 5 terms: one group of four, the fifth rides as a scalar add -----
[flatten]
def h_five(a, b, c, d, e : float) : float {
    return a + b + c + d + e
}

// ----- 8 terms: two groups of four -----
[flatten]
def h_eight(a, b, c, d, e, f, g, h : float) : float {
    return a + b + c + d + e + f + g + h
}

// ----- 3 terms: below the four-lane threshold, stays a scalar add chain -----
[flatten]
def h_three(a, b, c : float) : float {
    return a + b + c
}

// ----- negated tail: four positives pack, the `- e` stays -----
[flatten]
def h_neg(a, b, c, d, e : float) : float {
    return a + b + c + d - e
}

// ----- const term stays out of the ctor (it folds), four vars pack -----
[flatten]
def h_const(a, b, c, d : float) : float {
    return a + b + c + d + 0.5
}

// ----- horizontal sum buried inside a call argument -----
[flatten]
def h_buried(a, b, c, d : float) : float {
    return sin(a + b + c + d)
}

// ----- the plasma shape: `(sum) * s + k` -> mad(hadd(...), s, k) -----
[flatten]
def h_mad(a, b, c, d : float) : float {
    return (a + b + c + d) * 2.0 + 1.0
}

// ----- weighted products must NOT pack: `n*w` is mad-fusable, mad beats hadd (mandelbrot fBm) -----
[flatten]
def h_products(a, b, c, d : float) : float {
    return a * 2.0 + b * 3.0 + c * 4.0 + d * 5.0
}

var GRID : array<float>

[init]
def fill_grid {
    GRID <- [-100.0, -3.5, -1.0, 0.0, 1.0, 2.5, 7.0, 100.0]
}

def fapprox(t : T?; a, b : float) {
    let m = max(max(abs(a), abs(b)), 1.0)
    t |> success(abs(a - b) <= 1.0e-4 * m, "hadd-fused approx mismatch: {a} vs {b}")
}

[test]
def test_flatten_hadd(t : T?) {
    t |> run("four / five / eight scalar sums — value approx") @(t : T?) {
        for (v in GRID) {
            fapprox(t, h_four_flat(v, 1.5, -v, 2.5), h_four(v, 1.5, -v, 2.5))
            fapprox(t, h_five_flat(v, 1.5, -v, 2.5, 0.125), h_five(v, 1.5, -v, 2.5, 0.125))
            fapprox(t, h_eight_flat(v, 1.5, -v, 2.5, 0.125, -0.5, v * 0.25, 3.0),
                       h_eight(v, 1.5, -v, 2.5, 0.125, -0.5, v * 0.25, 3.0))
        }
    }
    t |> run("below-threshold / negated / const tail — value approx") @(t : T?) {
        for (v in GRID) {
            fapprox(t, h_three_flat(v, 1.5, -v), h_three(v, 1.5, -v))
            fapprox(t, h_neg_flat(v, 1.5, -v, 2.5, 0.75), h_neg(v, 1.5, -v, 2.5, 0.75))
            fapprox(t, h_const_flat(v, 1.5, -v, 2.5), h_const(v, 1.5, -v, 2.5))
        }
    }
    t |> run("buried sum / mad shape / weighted products — value approx") @(t : T?) {
        for (v in GRID) {
            fapprox(t, h_buried_flat(v, 1.5, -v, 2.5), h_buried(v, 1.5, -v, 2.5))
            fapprox(t, h_mad_flat(v, 1.5, -v, 2.5), h_mad(v, 1.5, -v, 2.5))
            fapprox(t, h_products_flat(v, 1.5, -v, 2.5), h_products(v, 1.5, -v, 2.5))
        }
    }
}
