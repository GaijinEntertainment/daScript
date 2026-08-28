options gen2

// SMT001: a branch that cannot be taken on any path that reaches it
//
// Problem: a condition that contradicts what is already known at that point is
// dead code, and usually a typo in a bound or a copy-pasted guard.
//
// Bad pattern:
//   if (a > 10) {
//       if (a < 5) { ... }        // cannot hold
//   }
//
// Good pattern:
//   if (a > 10) {
//       if (a < 50) { ... }
//   }
//
// The `smt_expect` comments are the contract: the driver (smt_lint_check.das)
// asserts one warning per marked line and none anywhere else. This file is never
// run, only analyzed.

// nested conditions that cannot both hold
def bad_nested(a : int) : int {
    if (a > 10) {
        if (a < 5) {                                    // smt_expect SMT001
            return 1
        }
    }
    return 0
}

// the guard already left the function for a > 7
def bad_after_guard(a : int) : int {
    if (a > 7) {
        return -1
    }
    if (a > 7) {                                        // smt_expect SMT001
        return 2
    }
    return 0
}

// an assertion narrows the range just like a guard does
def bad_after_assert(a : int) : int {
    assert(a > 100)
    if (a < 0) {                                        // smt_expect SMT001
        return 3
    }
    return 0
}

// the else branch is the impossible one. Reports on the `if` line, not the `else`
// line, so a nolint comment has one obvious home.
def bad_else_branch(a : int) : int {
    if (a > 0) {
        if (a > -5) {                                   // smt_expect SMT001
            return 1
        } else {
            return 2
        }
    }
    return 0
}

// a conjunction that contradicts itself is SMT008, not SMT001: the defect is in the
// expression, not in the path that reaches it
def bad_conjunction(a, b : int) : int {
    if (a > b && b > a) {                               // smt_expect SMT008
        return 1
    }
    return 0
}

// a tracked local carries the contradiction
def bad_via_local(a : int) : int {
    let limit = a
    if (limit > 100) {
        if (a < 100) {                                  // smt_expect SMT001
            return 1
        }
    }
    return 0
}

// float comparisons are exact IEEE, so this really is impossible
def bad_float(x : float) : int {
    if (x > 2.0f) {
        if (x < 1.0f) {                                 // smt_expect SMT001
            return 1
        }
    }
    return 0
}

// ----- must stay silent -----

// reachable at INT_MAX, where a + 1 wraps negative
def good_overflow_guard(a : int) : int {
    let b = a + 1
    if (b < a) {
        return 1
    }
    return 0
}

// NaN is not equal to itself, so this branch is reachable
def good_nan(x : float) : int {
    if (x != x) { // nolint:LINT007 — comparing x to itself IS the NaN test
        return 1
    }
    return 0
}

// nothing is known about a
def good_unconstrained(a : int) : int {
    if (a > 10) {
        return 1
    }
    return 0
}

// consistent nested bounds
def good_consistent(a : int) : int {
    if (a > 10) {
        if (a < 50) {
            return 1
        }
    }
    return 0
}

// the callee could return anything
def opaque(a : int) : int {
    return a * 3
}

def good_opaque_call(a : int) : int {
    if (opaque(a) > 10) {
        if (opaque(a) < 5) {
            return 1
        }
    }
    return 0
}

// the loop rewrites i, so no fact about it survives the loop edge
def good_loop_carried(n : int) : int {
    var i = 0
    while (i < n) {
        if (i > 1000) {
            return 1
        }
        i++
    }
    return 0
}

// int is a workhorse type, so the reference is what makes this a real write-back
def fill(var out : int&) {
    out = 7
}

def good_mutable_ref(a : int) : int {
    var v = a
    if (v > 100) {
        fill(v)
        if (v < 0) {
            return 1
        }
    }
    return 0
}

// uint is not modeled, so no verdict either way
def good_unsigned(u : uint) : int {
    if (u > 10u) {
        if (u < 5u) {
            return 1
        }
    }
    return 0
}

// deliberate duplicate guard, suppressed at the source
def good_suppressed(a : int) : int {
    if (a > 10) {
        if (a < 5) { // nolint:SMT001
            return 1
        }
    }
    return 0
}
