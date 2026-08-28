options gen2
options _enable_default_off_rules = true

// SMT002: a condition that always holds, with no else branch
//
// Problem: the guard reads as a check but can never fail, so it hides the fact
// that the code below is unconditional.
//
// Bad pattern:
//   if (a > 10) {
//       if (a > 5) { ... }        // always true
//   }
//
// Good pattern:
//   if (a > 10) {
//       ...
//   }
//
// SMT002 is default-off; the `options` line above turns it on for this file.

// implied by the enclosing guard
def bad_implied(a : int) : int {
    if (a > 10) {
        if (a > 5) {                                    // smt_expect SMT002
            return 1
        }
    }
    return 0
}

// implied by an assertion
def bad_implied_by_assert(a : int) : int {
    assert(a > 100)
    if (a > 0) {                                        // smt_expect SMT002
        return 1
    }
    return 0
}

// re-checking what the early return already established
def bad_recheck(a : int) : int {
    if (a < 0) {
        return -1
    }
    if (a >= 0) {                                       // smt_expect SMT002
        return 1
    }
    return 0
}

// ----- must stay silent -----

// a genuine check
def good_real_check(a : int) : int {
    if (a > 10) {
        if (a > 50) {
            return 1
        }
    }
    return 0
}

// deliberate revalidation, suppressed at the source
def good_suppressed(a : int) : int {
    if (a > 10) {
        if (a > 5) { // nolint:SMT002
            return 1
        }
    }
    return 0
}
