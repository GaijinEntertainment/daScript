options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// STYLE039: a condition an interval lattice collapses to a single comparison
//
// Problem: a chain of comparisons on one int variable often describes a simpler
// range than it looks. This is NOT a redundant operand — in `x != 1 && x > 0`
// neither operand implies the other, so subsumption cannot see it. Over the
// integers the two merge: x > 0 and x != 1 leaves exactly x >= 2.
//
// Bad pattern:
//   if (x != 1 && x > 0) { ... }
//
// Good pattern:
//   if (x >= 2) { ... }

expect 31209:5

require daslib/style_lint

def bad_merge_gt(x : int) : int {
    if (x != 1 && x > 0) {                  // STYLE039 — x >= 2
        return 1
    }
    return 0
}

def bad_subsumed(x : int) : int {
    if (x > 5 && x > 3) {                   // STYLE039 — x >= 6
        return 1
    }
    return 0
}

def bad_merge_eq(x : int) : int {
    if (x > 0 && x < 2) {                   // STYLE039 — x == 1
        return 1
    }
    return 0
}

def bad_ge_and_ne(x : int) : int {
    if (x >= 1 && x != 1) {                 // STYLE039 — x >= 2
        return 1
    }
    return 0
}

def bad_union_le(x : int) : int {
    if (x < 0 || x == 0) {                  // STYLE039 — x <= 0
        return 1
    }
    return 0
}

// ----- must stay silent -----

// a genuine two-sided range: no single comparison is equivalent
def good_two_sided(x : int) : int {
    if (x > 0 && x < 10) {
        return 1
    }
    return 0
}

// two disjoint intervals
def good_disjoint(x : int) : int {
    if (x < 0 || x > 100) {
        return 1
    }
    return 0
}

// two variables — the lattice is single-variable
def good_two_vars(x, y : int) : int {
    if (x > 0 && y < 10) {
        return 1
    }
    return 0
}

// already a single comparison; nothing to collapse
def good_single(x : int) : int {
    if (x > 0) {
        return 1
    }
    return 0
}

// always true is STYLE010/SMT008 territory, not a simplification
def good_tautology(x : int) : int {
    if (x != 1 || x != 2) {
        return 1
    }
    return 0
}

// non-int operands are out of scope
def good_float(x : float) : int {
    if (x > 0.0f && x != 1.0f) {
        return 1
    }
    return 0
}
