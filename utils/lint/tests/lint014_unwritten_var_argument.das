options gen2
// LINT014: mutable (var) function argument is never written
//
// A by-ref parameter declared `var` whose body never writes it forces every
// caller's argument to be mutable for nothing (the caller side of issue #3090).
// Drop the var; callers may then declare their arguments with `let`.
//
// Cascade: a var param that is itself passed onward to a mutable slot is
// skipped — the leaf callee gets flagged first; fixing it re-exposes this one.

expect 50503:3

require daslib/lint

struct private Box {
    value : int
}

// === Positives — var param never written ===

def bad_struct(var b : Box) : int {   // LINT014
    return b.value
}

def bad_fixed_array(var a : float[4][4]) : string {   // LINT014
    return "{a[0][0]}"
}

def bad_explicit_ref(var n : int&) : int {   // LINT014
    return n + 1
}

// === Negatives ===

// writes its param — var is earned
def good_writes(var b : Box) : int {
    b.value = 5
    return b.value
}

// passes the param to a writing callee — access_ref propagates, no warning
def good_pass_to_writer(var b : Box) : int {
    return good_writes(b)
}

// passes the param to a mutable slot of a NON-writing callee — flag the leaf
// (bad_struct above), not this transitive demander
def good_cascade(var b : Box) : int {
    return bad_struct(b)
}

// workhorse by-value var is a mutable local copy, not a caller burden
def good_by_value(var n : int) : int {
    return n + 1
}

// finalize signature is dictated by the delete protocol
def finalize(var b : Box) {
    print("finalizing {b.value}\n")
}

// underscore prefix means leave-it-alone
def good_underscore(var _b : Box) : int {
    return _b.value
}
