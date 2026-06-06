options gen2
options indenting = 4
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/flatten

//! M4 differential tests: fixed-count loop unrolling (constant `range` and array
//! literals) plus compound-assignment lowering. Each twin is run against its
//! original over an input grid AND (via the macro's cycle-2 scan) proven
//! branchless + call-free — i.e. each loop fully unrolled to straight-line `?:`.

// ----- helper callee (inlined inside an unrolled iteration) -----

def sq(a : int) : int {
    return a * a
}

// ----- compound assignment + increment (no loop): the latent-bug regression -----
// `a += x` is an ExprOp2 op="+=" and `a++` is an ExprOp1 op="++"; neither is an
// ExprCopy, so both must be lowered explicitly rather than dropped on the floor.
[flatten]
def f_compound(x : int) : int {
    var a = 0
    a += x
    a += x * 2
    a -= 4
    a *= 3
    a ++
    a --
    a --
    return a
}

// ----- range(CONST) accumulation -----
[flatten]
def f_range(x : int) : int {
    var acc = 0
    for (i in range(4)) {
        acc += x * i
    }
    return acc
}

// ----- range with a non-zero start -----
[flatten]
def f_range_start(x : int) : int {
    var acc = 0
    for (i in range(2, 6)) {
        acc += x + i
    }
    return acc
}

// ----- array-literal source -----
[flatten]
def f_array(x : int) : int {
    var acc = 0
    for (k in [2, 5, 9]) {
        acc += x * k
    }
    return acc
}

// ----- nested loops (both unrolled) -----
[flatten]
def f_nested(x : int) : int {
    var acc = 0
    for (i in range(3)) {
        for (j in range(2)) {
            acc += x + i * 10 + j
        }
    }
    return acc
}

// ----- if inside an unrolled iteration (predicated body per copy) -----
[flatten]
def f_loop_if(x : int) : int {
    var acc = 0
    for (i in range(5)) {
        if (i > 2) {
            acc += x
        } else {
            acc ++          // ++ inside a predicated unrolled iteration
        }
    }
    return acc
}

// ----- loop-dependent cond that DOES const-fold (cond is a function of `i` only) -----
// After unrolling, `i` is a literal per copy, so `i >= 3` folds to a compile-time
// `false`/`true`: the twin becomes `acc = false ? (acc+x) : acc` (×3) then
// `acc = true ? (acc+x) : acc` (×2). The constant-comparison fold is the TYPER's
// (re-infer), not flatten's const_prop — flatten emits `(i>=3)?…` and relies on the
// compiler to fold the now-constant comparison. Verified branchless by the cycle-2 scan.
[flatten]
def f_loop_constfold(x : int) : int {
    var acc = 0
    for (i in range(5)) {
        if (i >= 3) {
            acc += x
        }
    }
    return acc
}

// ----- loop-dependent cond that does NOT const-fold (cond depends on runtime `x`) -----
// `x > i` keeps a genuine runtime select per unrolled copy: `acc = (x>0)?…`,
// `acc = (x>1)?…`, … `i` folds to a literal but `x` is runtime, so the select stays.
[flatten]
def f_loop_runtime(x : int) : int {
    var acc = 0
    for (i in range(5)) {
        if (x > i) {
            acc += i
        }
    }
    return acc
}

// ----- helper call inside an unrolled iteration (inlined per copy) -----
[flatten]
def f_loop_call(x : int) : int {
    var acc = 0
    for (i in range(3)) {
        acc += sq(i) + x
    }
    return acc
}

// ----- loop body with a per-iteration LOCAL (shadow-collision regression) -----
// Each unrolled copy redeclares `step`; without per-iteration renaming the copies
// shadow each other in the single flattened scope (error[30704]).
[flatten]
def f_loop_local(x : int) : int {
    var acc = 0
    for (i in range(4)) {
        let step = x + i * 2
        acc += step * step
    }
    return acc
}

// ----- nested loops, each with its own local (double-rename path) -----
[flatten]
def f_nested_local(x : int) : int {
    var acc = 0
    for (i in range(3)) {
        let base = i * 100
        for (j in range(2)) {
            let cell = base + j + x
            acc += cell
        }
    }
    return acc
}

// ----- identity folds: `x * i` over an unrolled range exercises *0, *1 (and *-1) -----
// i=0 -> x*0 -> the whole `acc += x*0` store collapses away; i=1 -> x*1 -> x.
[flatten]
def f_loop_mul(x : int) : int {
    var acc = 0
    for (i in range(4)) {
        acc += x * i
    }
    return acc
}

// negative start -> i=-1 -> x*-1 -> -x (the `* -1` negation fold)
[flatten]
def f_loop_negidx(x : int) : int {
    var acc = 0
    for (i in range(-1, 3)) {
        acc += x * i
    }
    return acc
}

// ----- driver -----

var GRID : array<int>

[init]
def fill_grid {
    GRID <- [-7, -1, 0, 1, 3, 10, 42]
}

[test]
def test_flatten_loops(t : T?) {
    t |> run("compound assignment + increment (+= -= *= ++ --)") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_compound_flat(v), f_compound(v))
        }
    }
    t |> run("range(CONST) accumulation") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_range_flat(v), f_range(v))
        }
    }
    t |> run("range with non-zero start") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_range_start_flat(v), f_range_start(v))
        }
    }
    t |> run("array-literal loop source") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_array_flat(v), f_array(v))
        }
    }
    t |> run("nested loops (both unrolled)") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_nested_flat(v), f_nested(v))
        }
    }
    t |> run("if inside an unrolled iteration") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_loop_if_flat(v), f_loop_if(v))
        }
    }
    t |> run("loop-dependent cond that const-folds (cond of i only)") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_loop_constfold_flat(v), f_loop_constfold(v))
        }
    }
    t |> run("loop-dependent cond that stays runtime (cond of x)") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_loop_runtime_flat(v), f_loop_runtime(v))
        }
    }
    t |> run("helper call inside an unrolled iteration") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_loop_call_flat(v), f_loop_call(v))
        }
    }
    t |> run("per-iteration local (shadow-collision regression)") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_loop_local_flat(v), f_loop_local(v))
        }
    }
    t |> run("nested loops with per-level locals (double rename)") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_nested_local_flat(v), f_nested_local(v))
        }
    }
    t |> run("identity folds: x*0 / x*1 over an unrolled range") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_loop_mul_flat(v), f_loop_mul(v))
        }
    }
    t |> run("identity fold: x*-1 -> -x (negative loop index)") @(t : T?) {
        for (v in GRID) {
            t |> equal(f_loop_negidx_flat(v), f_loop_negidx(v))
        }
    }
}
