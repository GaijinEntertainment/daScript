options gen2
require at_unset_breaker

// One function per guess the verify has to make for a node with no `at`; at_unset_breaker.das clears
// the locations. Operands are arguments so `a + b` survives folding - the breaker needs a live op.

// the subtree still has locations: the guess comes from inside it
def lost_one(a, b : int) : int {
    return a + b
}

// nothing located inside, and in pre-order the enclosing return is the node passed last
def deep_add(a, b : int) : int {
    return a + b
}

// nothing located in the body at all: what is left is the argument the walk came in through
def all_gone(a : int) : int {
    return a
}

// and with no argument either, the function itself
def no_args : int {
    return 5
}

// an `assume` alias is substituted at its use sites and never inferred, so the verify refuses to
// walk into it - and so must the search for a location, or it anchors at a spelling nobody ran
def with_assume(a : int) : int {
    assume alias = a + 111
    return alias * 2
}

[export]
def main {
    var x = 2 // nolint:LINT003
    print("{lost_one(x, 3)} {deep_add(x, 3)} {all_gone(x)} {no_args()} {with_assume(x)}\n")
}
