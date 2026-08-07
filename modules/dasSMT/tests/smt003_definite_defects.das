options gen2
options _enable_default_off_rules = true

// SMT003..SMT007: constructs that are broken on every path that reaches them
//
// Unlike SMT001/SMT002 these are not redundancy but definite defects: each asks
// "is this ALWAYS wrong here", never "could it be wrong". A merely-possible zero
// divisor would fire on every unconstrained value, so only the provable case is
// reported.

// ----- SMT003: division / modulo by zero -----

def bad_div_by_zero(a : int) : int {
    let d = 0
    if (a > 5) {
        return a / d                                    // smt_expect SMT003
    }
    return 0
}

def bad_mod_by_zero(a, n : int) : int {
    if (n == 0) {
        return a % n                                    // smt_expect SMT003
    }
    return 0
}

def good_div_guarded(a, n : int) : int {
    if (n != 0) {
        return a / n
    }
    return 0
}

def good_div_unknown(a, n : int) : int {
    return a / n
}

// ----- SMT004: an assertion that cannot hold -----

def bad_assert_contradiction(a : int) : int {
    if (a > 100) {
        assert(a < 0)                                   // smt_expect SMT004
        return 1
    }
    return 0
}

def good_assert_plausible(a : int) : int {
    if (a > 100) {
        assert(a > 50)
        return 1
    }
    return 0
}

// ----- SMT005: a loop that never runs -----

def bad_loop_never_runs(n : int) : int {
    var total = 0
    if (n < 0) {
        while (n > 10) {                                // smt_expect SMT005
            total++
        }
    }
    return total
}

def good_loop_runs(n : int) : int {
    var total = 0
    var i = 0
    while (i < n) {
        total++
        i++
    }
    return total
}

// ----- SMT006: shift count outside 0..31 -----

def bad_shift_too_far(a : int) : int {
    let sh = 40
    if (a > 0) {
        return a << sh                                  // smt_expect SMT006
    }
    return 0
}

def bad_shift_negative(a, sh : int) : int {
    if (sh < 0) {
        return a << sh                                  // smt_expect SMT006
    }
    return 0
}

def good_shift_in_range(a, sh : int) : int {
    if (sh >= 0 && sh < 32) {
        return a << sh
    }
    return 0
}

def good_shift_unknown(a, sh : int) : int {
    return a << sh
}

// ----- SMT007: index that is always negative -----

def bad_negative_index(arr : array<int>; i : int) : int {
    if (i < 0) {
        return arr[i]                                   // smt_expect SMT007
    }
    return 0
}

def good_index_guarded(arr : array<int>; i : int) : int {
    if (i >= 0 && i < length(arr)) {
        return arr[i]
    }
    return 0
}

def good_index_unknown(arr : array<int>; i : int) : int {
    return arr[i]
}

// `a - a` cannot leave the int range, so the divisor is provably zero even though
// no literal 0 appears
def bad_div_simplifies_to_zero(a : int) : int {
    let d = a - a // nolint:LINT007 — the point of the test is that Z3 folds this to 0
    return 10 / d                                       // smt_expect SMT003
}

// ----- SMT008: a condition that is constant whatever its inputs are -----

// the author meant && — as written this always holds
def bad_tautological_or(x : int) : int {
    if (x != 1 || x != 2) {                             // smt_expect SMT008
        return 1
    }
    return 0
}

def bad_contradictory_and(x : int) : int {
    if (x > 5 && x < 3) {                               // smt_expect SMT008
        return 1
    }
    return 0
}

def good_real_range(x : int) : int {
    if (x > 5 && x < 30) {
        return 1
    }
    return 0
}

def good_real_exclusion(x : int) : int {
    if (x < 0 || x > 100) {
        return 1
    }
    return 0
}
