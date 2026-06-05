options gen2
options indenting = 4
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/flatten
require math

//! Differential tests for daslib/flatten: each `[flatten]`-annotated function
//! is run against its generated `<name>_flat` twin over an input grid; the
//! flattened (branchless, call-free) twin must compute identical results.
//!
//! NOTE: the interpreter's `?:` short-circuits, so these tests prove logical
//! EQUIVALENCE — they do not exercise the branchless "both arms execute"
//! behavior of a real GPU backend. See tests/flatten/README.md.

// ----- functions under flatten -----

[flatten]
def tail(x : int) : int {
    return x * 2 + 1
}

[flatten]
def guard(x : int) : int {
    if (x < 0) {
        return -1
    }
    return x * 10
}

[flatten]
def nested(x : int) : int {
    var r = 0
    if (x < 0) {
        r = -1
    } else {
        if (x > 100) {
            return 999
        }
        r = x
    }
    return r + 1
}

def square(a : int) : int {
    return a * a
}

def addk(a : int; k : int) : int {
    return a + k
}

[flatten]
def inlines(x : int) : int {
    let s = square(x)
    return addk(s, 3) + square(x + 1)
}

def cc(n : int) : int {
    return n + 100
}

def bb(n : int) : int {
    return cc(n) * 2
}

def aa(n : int) : int {
    return bb(n) - 1
}

[flatten]
def transitive(x : int) : int {
    return aa(x) + aa(x + 1)
}

[flatten]
def with_builtin(x : int) : int {
    return max(x, 0) + min(x, 5)
}

// ----- differential driver -----

var GRID : array<int>

[init]
def fill_grid {
    GRID <- [-201, -101, -7, -1, 0, 1, 7, 100, 101, 250]
}

[test]
def test_flatten_equivalence(t : T?) {
    t |> run("tail return") @(t : T?) {
        for (v in GRID) {
            t |> equal(tail_flat(v), tail(v))
        }
    }
    t |> run("early-return guard") @(t : T?) {
        for (v in GRID) {
            t |> equal(guard_flat(v), guard(v))
        }
    }
    t |> run("nested if/else + early return + local mutation") @(t : T?) {
        for (v in GRID) {
            t |> equal(nested_flat(v), nested(v))
        }
    }
    t |> run("inlining (let-init + nested-expression calls)") @(t : T?) {
        for (v in GRID) {
            t |> equal(inlines_flat(v), inlines(v))
        }
    }
    t |> run("transitive inlining a->b->c, lifted operands") @(t : T?) {
        for (v in GRID) {
            t |> equal(transitive_flat(v), transitive(v))
        }
    }
    t |> run("whitelisted builtins") @(t : T?) {
        for (v in GRID) {
            t |> equal(with_builtin_flat(v), with_builtin(v))
        }
    }
}
