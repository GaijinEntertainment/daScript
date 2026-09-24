// The compares behind the suites' bars - the logits and kernel-output maxdiff, the relative bar's scale, the
// id-list equality - with no fixture root beside them, so a model-free file requires this and never
// `_model_tier` (the per-PR split reads a same-folder `require _<fixture>` as reaching that fixture's roots).

options gen2
options _dasllama_internal = true

module _compares shared public

require dasllama/dasllama_lint public
require math   // abs

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

//! Element-for-element equality of two id lists.
def ids_equal(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
            return false
        }
    }
    return true
}
