// The compares behind the suites' bars - the logits and kernel-output maxdiff, the relative bar's scale, the
// id-list equality - with no fixture root beside them, so a model-free file requires this and never
// `_model_tier` (the per-PR split reads a same-folder `require _<fixture>` as reaching that fixture's roots).

options gen2
options _dasllama_internal = true

module _compares shared public

require dasllama/dasllama_lint public
require math
require daslib/linq   // sequence_equal

//! Max |a[i] - b[i]| over two float arrays of one length (the shorter's, where they differ) - the compare behind every
//! logits or kernel-output bar in the suites.
def logits_maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (x, y in a, b) {
        m = max(m, abs(x - y))
    }
    return m
}

//! Max |a[i]| - the scale a relative logits bar multiplies.
def logits_maxabs(a : array<float>) : float {
    var m = 0.0
    for (x in a) {
        m = max(m, abs(x))
    }
    return m
}

//! Element-for-element equality of two id lists - the token oracles' compare, named for the sites that read it.
def ids_equal(a, b : array<int64>) : bool => sequence_equal(a, b)

//! The kernel cells' relative bar - the device's reassociation and its transcendentals against the CPU oracle: an f16 store
//! rounds at 2^-11 = 4.9e-4 relative, f32 sums of up to 4096 terms drift n x 6e-8 <= 2.5e-4, the device transcendentals ~1e-6;
//! 2e-3 covers each with margin.
let APPROX_REL = 2e-3

//! within `APPROX_REL` of the larger magnitude, floored at 1
def approx(x, y : float) : bool => abs(x - y) <= APPROX_REL * max(1.0, max(abs(x), abs(y)))

//! The elements of `got` off `want` by more than `bar` relative (floored at 1), the first six logged for the eyeball -
//! enough to tell tolerance drift from garbage.
def mismatch_rel(got, want : array<float>; bar : float) : int {
    var bad = 0
    for (i in range(length(got))) {
        let g = got[i]
        let w = want[i]
        if (!(abs(g - w) <= bar * max(1.0, max(abs(g), abs(w))))) {
            if (bad < 6) {
                to_log(LOG_ERROR, "  mismatch [{i}] gpu {g} cpu {w}\n")
            }
            bad++
        }
    }
    return bad
}

//! `mismatch_rel` at the kernel cells' bar.
def mismatch_approx(got, want : array<float>) : int => mismatch_rel(got, want, APPROX_REL)

//! The elements of `got` not bit-equal to `want` - the compare behind a kernel cell's exact bar (a copy, a clamp, a pad).
def mismatch_exact(got, want : array<float>) : int {
    var bad = 0
    for (g, w in got, want) {
        if (g != w) {
            bad++
        }
    }
    return bad
}

//! The Q8_0 planes' differences, scale for scale then byte for byte: a scale off by more than `scale_bar` relative (floored at
//! 1; 0 = bit-exact), a quant byte off by more than `qtol` steps.
def mismatch_q8(qd : array<int8>; sd : array<float>; qr : array<int8>; sr : array<float>; scale_bar : float; qtol : int) : int {
    var bad = scale_bar == 0.0 ? mismatch_exact(sd, sr) : mismatch_rel(sd, sr, scale_bar)
    for (a, b in qd, qr) {
        if (abs(int(a) - int(b)) > qtol) {
            bad++
        }
    }
    return bad
}

//! The word a kernel cell fills an output with before its dispatch: a NaN, so a stale or unwritten element cannot sit
//! inside any bar.
let NAN_SENTINEL_BITS = 0x7fc00000u
