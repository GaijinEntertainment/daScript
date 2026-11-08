options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/flatten
require math
require _flat_api_macro

//! Differential tests for daslib/flatten: each `[flatten]`-annotated
//! function is run against its generated `<name>_flat` twin over an
//! input grid; the flattened (branchless, call-free) twin must
//! compute identical results.
//!
//! NOTE: the interpreter's `?:` short-circuits, so these tests prove
//! logical EQUIVALENCE — they do not exercise the branchless "both
//! arms execute" behavior of a real GPU backend. See
//! tests/flatten/README.md.

// ----- functions under flatten -----

[flatten]
def tail(x : int) : int => x * 2 + 1

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

def square(a : int) : int => a * a

def addk(a : int; k : int) : int => a + k

[flatten]
def inlines(x : int) : int {
    let s = square(x)
    return addk(s, 3) + square(x + 1)
}

def cc(n : int) : int => n + 100

def bb(n : int) : int => cc(n) * 2

def aa(n : int) : int => bb(n) - 1

[flatten]
def transitive(x : int) : int => aa(x) + aa(x + 1)

[flatten]
def with_builtin(x : int) : int => max(x, 0) + min(x, 5)

// helper WITH a local — inlining it at multiple sites (loop unroll,
// or two direct call sites) must uniquify the callee's locals, else
// the copies emit a colliding `var t` in the single flattened scope
// (error[30704]). Every other helper here is a single-expression
// return with no locals, so this case was previously uncovered.
def withlocal(a : int) : int {
    let t = a * 3
    let u = t + 1
    return u * t
}

[flatten]
def inline_loop(x : int) : int {
    var acc = 0
    for (i in range(3)) {
        acc += withlocal(x + i)
    }
    return acc
}

[flatten]
def inline_twice(x : int) : int => withlocal(x) + withlocal(x + 1)

// A STRUCT return type with an early return: the predicated
// `__flat_ret = pred ? v : __flat_ret` write reads the bare
// accumulator decl, which a live early return leaves intact (the
// mask-fold can only collapse it for a single unconditional return).
// For a struct (unsafe-when-uninitialized) result that read was
// previously rejected (error[31016]); the lowering now marks the
// accumulator safe-when-uninitialized because the placeholder read is
// always discarded.
struct Pt {
    a : float3 = float3(0)
    b : float = 1.0
}

[flatten]
def struct_uncond(x : float) : Pt => Pt(a = float3(x))

[flatten]
def struct_guard(x : float; c : bool) : Pt {
    if (c) {
        return Pt(a = float3(x))
    }
    return Pt(b = x)
}

// ----- partially-assigned bare decls: each branch writes a DIFFERENT
// var; the other keeps its implicit zero (the lowering materializes
// the zero-init the predicated select's false path reads) -----

[flatten]
def partial_vec(x : float) : float3 {
    var a, b : float3
    if (x > 0.5) {
        a = float3(1.0, 0.0, 0.0)
    } else {
        b = float3(0.0, 1.0, 0.0)
    }
    return a + b
}

[flatten]
def partial_scalar(x : float) : float {
    var s : float
    if (x > 0.5) {
        s = x * 2.0
    }
    return s + 1.0
}

// fixed-array literal in a flatten body (ExprMakeArray): elements
// lower/optimize like any operand, and a user call inside is inlined
// (lift_expr recurses the array). Integer arithmetic so reassoc stays
// exact for the differential equal().
def isq(a : int) : int => a * a + 1

[flatten]
def fixed_arr(x : int) : int {
    let a = fixed_array(isq(x), x * 2, x - 1, isq(x + 1))
    return a[0] + a[1] - a[2] + a[3]
}

// ----- differential driver -----

var GRID <- [-201, -101, -7, -1, 0, 1, 7, 100, 101, 250]

[test(description = "each [flatten] twin of a tail return, early-return guard, nested if with mutation, inlined call chain, whitelisted builtin and unrolled helper computes the same results as its original over an input grid")]
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
    t |> run("helper-with-local inlined across an unrolled loop") @(t : T?) {
        for (v in GRID) {
            t |> equal(inline_loop_flat(v), inline_loop(v))
        }
    }
    t |> run("helper-with-local inlined at two direct call sites") @(t : T?) {
        for (v in GRID) {
            t |> equal(inline_twice_flat(v), inline_twice(v))
        }
    }
    t |> run("partially-assigned bare decls (each branch writes a different var)") @(t : T?) {
        for (v in GRID) {
            let fv = float(v) * 0.1
            let pv = partial_vec_flat(fv)
            let ev = partial_vec(fv)
            t |> equal(pv.x, ev.x)
            t |> equal(pv.y, ev.y)
            t |> equal(pv.z, ev.z)
            t |> equal(partial_scalar_flat(fv), partial_scalar(fv))
        }
    }
    t |> run("struct return type, unconditional + early return (was error[31016])") @(t : T?) {
        for (v in GRID) {
            let fv = float(v)
            let u = struct_uncond_flat(fv)
            let eu = struct_uncond(fv)
            t |> equal(u.a.x, eu.a.x)
            t |> equal(u.b, eu.b)
            for (c in fixed_array(false, true)) {
                let g = struct_guard_flat(fv, c)
                let eg = struct_guard(fv, c)
                t |> equal(g.a.x, eg.a.x)
                t |> equal(g.b, eg.b)
            }
        }
    }
    t |> run("fixed-array literal (ExprMakeArray), user call inside") @(t : T?) {
        for (v in GRID) {
            t |> equal(fixed_arr_flat(v), fixed_arr(v))
        }
    }
}

// Branch-coverage differential tests for daslib/flatten. Each
// function targets a specific lowering/lifting branch NOT exercised
// by test_flatten_basic.das; every twin is run against its original
// over an input grid AND (via the macro's own cycle-2 scan) proven
// branchless + call-free at compile time.

// ----- helper callees -----

def sq_cov(a : int) : int => a * a

def addk_cov(a : int; k : int) : int => a + k

def konst : int => 42  // 0-parameter callee

// ----- lift_expr: ExprOp1 (unary) wrapping a user call -----
[flatten]
def f_unary(x : int) : int => -sq_cov(x) + 1

// ----- lift_expr: ExprOp3 (source ternary) with a user call in each
// arm ----- Branchless lowering inlines BOTH arms unconditionally,
// then selects — relies on both callees being pure (double-eval is
// harmless).
[flatten]
def f_ternary(x : int) : int => x > 0 ? sq_cov(x) : addk_cov(x, 5)

// ----- inline_call: structPred non-null (user call inside an if
// branch) -----
[flatten]
def f_call_in_if(x : int) : int {
    if (x > 0) {
        return sq_cov(x)
    }
    return x
}

// ----- inline_call: 0-parameter callee (param-bind loop runs zero
// times) -----
[flatten]
def f_zero_param(x : int) : int => x + konst()

// ----- lower_stmt: bare expression statement (discarded user call)
// -----
[flatten]
def f_discard(x : int) : int {
    sq_cov(x)
    return x + 1
}

// ----- lower_stmt: bare block (lexical scope) -----
[flatten]
def f_block(x : int) : int {
    var r = x
    {
        r = r + 1
    }
    return r
}

// ----- lower_stmt: bare block declaring a block-local (scope is
// omittable) -----
[flatten]
def f_block_local(x : int) : int {
    var r = x
    {
        let y = r * 3
        r = y + 1
    }
    return r
}

// ----- lower_stmt: `with` unwrap (pure infer-time scope; node
// simulates to body) -----
struct Q {
    px : int
    py : int
}

[flatten]
def f_with(n : int) : int {
    let q = Q(px = n, py = n * 2)
    var acc = n
    with (q) {
        acc += px + py     // px, py resolve to q.px, q.py at infer; with is omittable
    }
    return acc
}

// ----- lower_stmt: ExprLet, multiple variables + no initializer
// (v.init==null) -----
[flatten]
def f_multivar(x : int) : int {
    var a, b : int   // no-init is the point: covers the v.init==null branch
    a = x   // nolint:STYLE011
    b = x + 1
    return a + b
}

// ----- void entry: early return + predicated writes to an output
// global ----- Mirrors the shader pattern where a void shader writes
// outputs to globals.
var g_vout : int

[flatten]
def vshader(x : int) {
    if (x < 0) {
        g_vout = -1
        return
    }
    g_vout = x * 10
}

// ----- void callee: inlined for its side effect (global write) only
// -----
var g_sink_cov : int

def veffect(x : int) {
    g_sink_cov = x * 2
}

[flatten]
def usev(x : int) : int {
    veffect(x)
    return x + 1
}

// ----- driver -----

var GRID_COV <- [-201, -101, -7, -1, 0, 1, 7, 100, 101, 250]

[test(description = "twins for rarely exercised lowering branches (unary and ternary wrapping calls, bare blocks, with-scopes, discarded calls and more) compute the same results as their originals and are proven branchless and call-free")]
def test_flatten_coverage(t : T?) {
    t |> run("ExprOp1 unary wrapping call") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_unary_flat(v), f_unary(v))
        }
    }
    t |> run("ExprOp3 source ternary, call per arm") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_ternary_flat(v), f_ternary(v))
        }
    }
    t |> run("call inside if branch (structPred non-null)") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_call_in_if_flat(v), f_call_in_if(v))
        }
    }
    t |> run("zero-parameter callee") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_zero_param_flat(v), f_zero_param(v))
        }
    }
    t |> run("bare discarded call statement") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_discard_flat(v), f_discard(v))
        }
    }
    t |> run("bare block scope") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_block_flat(v), f_block(v))
        }
    }
    t |> run("bare block declaring a block-local") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_block_local_flat(v), f_block_local(v))
        }
    }
    t |> run("with-scope unwrap") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_with_flat(v), f_with(v))
        }
    }
    t |> run("let with multiple variables") @(t : T?) {
        for (v in GRID_COV) {
            t |> equal(f_multivar_flat(v), f_multivar(v))
        }
    }
    t |> run("void entry: predicated write to output global") @(t : T?) {
        for (v in GRID_COV) {
            g_vout = 0
            vshader(v)
            let orig = g_vout
            g_vout = 0
            vshader_flat(v)
            t |> equal(g_vout, orig)
        }
    }
    t |> run("void callee inlined for its global-write side effect") @(t : T?) {
        for (v in GRID_COV) {
            g_sink_cov = 0
            let origRet = usev(v)
            let origSink = g_sink_cov
            g_sink_cov = 0
            let flatRet = usev_flat(v)
            t |> equal(flatRet, origRet)
            t |> equal(g_sink_cov, origSink)
        }
    }
}

// lift_expr operand-coverage differential tests. A user helper call
// can hide inside a member / swizzle / constructor / tuple / wrapper
// expression; lift_expr must recurse into those operands and inline
// the call. Without the recursion the call survives lowering and the
// post-pass verify scan rejects it as an "internal error". Each twin
// is differential-checked against its original AND proven call-free
// by the macro's cycle-2 scan.

struct P {
    a : int
    b : int
}

def mk(x : int) : P => P(a = x + 1, b = x * 2)

def vec(x : float) : float4 => float4(x, x + 1.0, x + 2.0, 1.0)

def sq_lift(a : int) : int => a * a

def addk_lift(a : int; k : int) : int => a + k

// ExprField — user call as the object of a `.field` read (wrapped in
// ExprRef2Value)
[flatten]
def f_field(x : int) : int => mk(x).a + mk(x).b

// ExprSwizzle — user call as the value of a `.xyz` swizzle
[flatten]
def f_swizzle(x : float) : float3 => vec(x).xyz

// ExprMakeStruct — user call inside a returned struct field
[flatten]
def f_makestruct(x : int) : P => P(a = mk(x).b, b = mk(x).a)

// ExprMakeTuple — user calls inside a tuple constructor, then tuple
// field access
[flatten]
def f_tuple(x : int) : int {
    let tp = (mk(x).a, mk(x).b)
    return tp._0 * 10 + tp._1
}

// nested: a call buried under swizzle + field + op together
[flatten]
def f_nested_lift(x : float) : float => vec(x).x + vec(x * 2.0).y

// inline_call argument is itself a user call — the argument must be
// lifted (nested call inlined) BEFORE binding to the frame temp, else
// `sq_lift(x)` survives in `var __flat_arg = sq_lift(x)` and the
// cycle-2 scan rejects the twin.
[flatten]
def f_arg_call(x : int) : int => addk_lift(sq_lift(x), sq_lift(x + 1))

var GRID_LIFT <- [-7, -1, 0, 1, 3, 10, 42]

[test(description = "a user helper call hidden inside a field, swizzle, struct, tuple or argument expression is inlined, so the twin matches its original and is call-free")]
def test_flatten_lift(t : T?) {
    t |> run("ExprField on a user call") @(t : T?) {
        for (v in GRID_LIFT) {
            t |> equal(f_field_flat(v), f_field(v))
        }
    }
    t |> run("ExprSwizzle on a user call") @(t : T?) {
        for (v in GRID_LIFT) {
            t |> equal(f_swizzle_flat(float(v)), f_swizzle(float(v)))
        }
    }
    t |> run("ExprMakeStruct field is a user call") @(t : T?) {
        for (v in GRID_LIFT) {
            let a = f_makestruct(v)
            let b = f_makestruct_flat(v)
            t |> equal(a.a, b.a)
            t |> equal(a.b, b.b)
        }
    }
    t |> run("ExprMakeTuple values are user calls") @(t : T?) {
        for (v in GRID_LIFT) {
            t |> equal(f_tuple_flat(v), f_tuple(v))
        }
    }
    t |> run("nested swizzle + field + op") @(t : T?) {
        for (v in GRID_LIFT) {
            t |> equal(f_nested_lift_flat(float(v)), f_nested_lift(float(v)))
        }
    }
    t |> run("user call in an argument position") @(t : T?) {
        for (v in GRID_LIFT) {
            t |> equal(f_arg_call_flat(v), f_arg_call(v))
        }
    }
}

// Nested-if differential tests for daslib/flatten. Each twin is run
// against its original over a 2-D input grid AND (via the macro's
// cycle-2 scan) proven branchless + call-free. Covers the
// predicate-AND threading (`structPred && c` / `structPred && !c`
// recursed through nesting) and the live-mask interplay when an early
// return sits inside a nested branch.

// ----- if nested in the then-branch (no else anywhere): single deep
// predicate ----- Lowers to one select gated on `(x>0) && (y>0)`.
[flatten]
def f_then_then(x : int; y : int) : int {
    var r = 0
    if (x > 0) {
        if (y > 0) {
            r = 1
        }
    }
    return r
}

// ----- if nested in the else-branch -----
// `if (x>0) r=1 else { if (y>0) r=2 }` -> selects gated on `x>0` and
// `!(x>0) && (y>0)`.
[flatten]
def f_else_then(x : int; y : int) : int {
    var r = 0
    if (x > 0) {
        r = 1
    } else {
        if (y > 0) {
            r = 2
        }
    }
    return r
}

// ----- if/else nested in BOTH arms (all four quadrants) -----
// Lowers to four selects, one per quadrant: (x>0)&&(y>0),
// (x>0)&&!(y>0), !(x>0)&&(y>0), !(x>0)&&!(y>0).
[flatten]
def f_quad(x : int; y : int) : int {
    var r = 0
    if (x > 0) {
        if (y > 0) {
            r = 1
        } else {
            r = 2
        }
    } else {
        if (y > 0) {
            r = 3
        } else {
            r = 4
        }
    }
    return r
}

// ----- triple nesting (if in if in if) -----
[flatten]
def f_triple(x : int; y : int; z : int) : int {
    var r = 0
    if (x > 0) {
        if (y > 0) {
            if (z > 0) {
                r = 7
            }
        }
    }
    return r
}

// ----- early return inside a nested if (live-mask threading) -----
// `__flat_live` tracks "has a return fired"; the trailing return is
// gated on it.
[flatten]
def f_nested_ret(x : int; y : int) : int {
    if (x > 0) {
        if (y > 0) {
            return 10
        }
        return 20
    }
    return 30
}

// ----- both inner arms return, outer falls through (no inner
// fallthrough) -----
[flatten]
def f_both_ret(x : int; y : int) : int {
    if (x > 0) {
        if (y > 0) {
            return 1
        } else {
            return 2
        }
    }
    return 3
}

// ----- nested predicate over a SHARED accumulator (compound-assign
// under nesting) ----- Mixes early-return live mask, structural
// predicate, and `+=` lowering in one body.
[flatten]
def f_nested_accum(x : int; y : int) : int {
    var acc = 0
    if (x > 0) {
        acc ++                    // ++ under a structural predicate
        if (y > 0) {
            return acc + 100      // early return out of the nested then
        }
        acc += 2
    }
    acc += 3
    return acc
}

// ----- driver -----

var GRID_NESTED <- [-5, -1, 0, 1, 5]

[test(description = "twins of nested if and if/else shapes, with early returns and shared accumulators, compute the same results as their originals over a two-dimensional grid")]
def test_flatten_nested(t : T?) {
    t |> run("if nested in then (deep AND predicate)") @(t : T?) {
        for (a in GRID_NESTED) {
            for (b in GRID_NESTED) {
                t |> equal(f_then_then_flat(a, b), f_then_then(a, b))
            }
        }
    }
    t |> run("if nested in else") @(t : T?) {
        for (a in GRID_NESTED) {
            for (b in GRID_NESTED) {
                t |> equal(f_else_then_flat(a, b), f_else_then(a, b))
            }
        }
    }
    t |> run("if/else in both arms (four quadrants)") @(t : T?) {
        for (a in GRID_NESTED) {
            for (b in GRID_NESTED) {
                t |> equal(f_quad_flat(a, b), f_quad(a, b))
            }
        }
    }
    t |> run("triple nesting") @(t : T?) {
        for (a in GRID_NESTED) {
            for (b in GRID_NESTED) {
                for (c in GRID_NESTED) {
                    t |> equal(f_triple_flat(a, b, c), f_triple(a, b, c))
                }
            }
        }
    }
    t |> run("early return inside nested if (live mask)") @(t : T?) {
        for (a in GRID_NESTED) {
            for (b in GRID_NESTED) {
                t |> equal(f_nested_ret_flat(a, b), f_nested_ret(a, b))
            }
        }
    }
    t |> run("both inner arms return") @(t : T?) {
        for (a in GRID_NESTED) {
            for (b in GRID_NESTED) {
                t |> equal(f_both_ret_flat(a, b), f_both_ret(a, b))
            }
        }
    }
    t |> run("nested predicate over a shared accumulator") @(t : T?) {
        for (a in GRID_NESTED) {
            for (b in GRID_NESTED) {
                t |> equal(f_nested_accum_flat(a, b), f_nested_accum(a, b))
            }
        }
    }
}

// M4 differential tests: fixed-count loop unrolling (constant `range`
// and array literals) plus compound-assignment lowering. Each twin is
// run against its original over an input grid AND (via the macro's
// cycle-2 scan) proven branchless + call-free — i.e. each loop fully
// unrolled to straight-line `?:`.

// ----- helper callee (inlined inside an unrolled iteration) -----

def sq_loops(a : int) : int => a * a

// ----- compound assignment + increment (no loop): the latent-bug
// regression ----- `a += x` is an ExprOp2 op="+=" and `a++` is an
// ExprOp1 op="++"; neither is an ExprCopy, so both must be lowered
// explicitly rather than dropped on the floor.
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

// ----- if inside an unrolled iteration (predicated body per copy)
// -----
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

// ----- loop-dependent cond that DOES const-fold (cond is a function
// of `i` only) ----- After unrolling, `i` is a literal per copy, so
// `i >= 3` folds to a compile-time `false`/`true`: the twin becomes
// `acc = false ? (acc+x) : acc` (×3) then `acc = true ? (acc+x) :
// acc` (×2). The constant-comparison fold is the TYPER's (re-infer),
// not flatten's const_prop — flatten emits `(i>=3)?…` and relies on
// the compiler to fold the now-constant comparison. Verified
// branchless by the cycle-2 scan.
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

// ----- loop-dependent cond that does NOT const-fold (cond depends on
// runtime `x`) ----- `x > i` keeps a genuine runtime select per
// unrolled copy: `acc = (x>0)?…`, `acc = (x>1)?…`, … `i` folds to a
// literal but `x` is runtime, so the select stays.
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

// ----- helper call inside an unrolled iteration (inlined per copy)
// -----
[flatten]
def f_loop_call(x : int) : int {
    var acc = 0
    for (i in range(3)) {
        acc += sq_loops(i) + x
    }
    return acc
}

// ----- loop body with a per-iteration LOCAL (shadow-collision
// regression) ----- Each unrolled copy redeclares `step`; without
// per-iteration renaming the copies shadow each other in the single
// flattened scope (error[30704]).
[flatten]
def f_loop_local(x : int) : int {
    var acc = 0
    for (i in range(4)) {
        let step = x + i * 2
        acc += step * step
    }
    return acc
}

// ----- nested loops, each with its own local (double-rename path)
// -----
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

// ----- identity folds: `x * i` over an unrolled range exercises *0,
// *1 (and *-1) ----- i=0 -> x*0 -> the whole `acc += x*0` store
// collapses away; i=1 -> x*1 -> x.
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

// ----- urange source (ExprConstURange -> ExprConstUInt) -----
// urange yields uint; cast per copy. i=0 folds the `x*int(i)` store
// away, i=1 -> x.
[flatten]
def f_urange(x : int) : int {
    var acc = 0
    for (i in urange(4u)) {
        acc += x * int(i)
    }
    return acc
}

// ----- urange with a non-zero start (urange(a, b)) -----
[flatten]
def f_urange_start(x : int) : int {
    var acc = 0
    for (i in urange(2u, 6u)) {
        acc += x + int(i)
    }
    return acc
}

// ----- parallel multi-source: two array literals unrolled in
// lockstep -----
[flatten]
def f_parallel(x : int) : int {
    var acc = 0
    for (a, b in [1, 2, 3], [10, 20, 30]) {
        acc += x + a * b
    }
    return acc
}

// ----- parallel mixed sources: range + array, in lockstep -----
[flatten]
def f_parallel_mix(x : int) : int {
    var acc = 0
    for (i, k in range(3), [5, 6, 7]) {
        acc += x + i * 100 + k
    }
    return acc
}

// ----- three parallel sources -----
[flatten]
def f_parallel_three(x : int) : int {
    var acc = 0
    for (a, b, c in [1, 2], [3, 4], range(2)) {
        acc += x + a * 10 + b + c * 100
    }
    return acc
}

// ----- parallel loop with a per-iteration local (multi-iterator
// rename path) -----
[flatten]
def f_parallel_local(x : int) : int {
    var acc = 0
    for (a, b in [2, 3, 5], [7, 11, 13]) {
        let prod = a * b + x
        acc += prod * prod
    }
    return acc
}

// ----- parallel loop containing a nested loop (recursion under
// multi-iterator) -----
[flatten]
def f_parallel_nested(x : int) : int {
    var acc = 0
    for (a, b in [1, 2, 3], [4, 5, 6]) {
        for (j in range(2)) {
            acc += x + a * b + j
        }
    }
    return acc
}

// ----- driver -----

var GRID_LOOPS <- [-7, -1, 0, 1, 3, 10, 42]

[test(description = "fixed-count loops (constant range or array literal), nested loops and compound assignments unroll into twins that match their originals")]
def test_flatten_loops(t : T?) {    // nolint:STYLE038 -- flat list of independent differential arms, one per loop shape
    t |> run("compound assignment + increment (+= -= *= ++ --)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_compound_flat(v), f_compound(v))
        }
    }
    t |> run("range(CONST) accumulation") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_range_flat(v), f_range(v))
        }
    }
    t |> run("range with non-zero start") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_range_start_flat(v), f_range_start(v))
        }
    }
    t |> run("array-literal loop source") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_array_flat(v), f_array(v))
        }
    }
    t |> run("nested loops (both unrolled)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_nested_flat(v), f_nested(v))
        }
    }
    t |> run("if inside an unrolled iteration") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_loop_if_flat(v), f_loop_if(v))
        }
    }
    t |> run("loop-dependent cond that const-folds (cond of i only)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_loop_constfold_flat(v), f_loop_constfold(v))
        }
    }
    t |> run("loop-dependent cond that stays runtime (cond of x)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_loop_runtime_flat(v), f_loop_runtime(v))
        }
    }
    t |> run("helper call inside an unrolled iteration") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_loop_call_flat(v), f_loop_call(v))
        }
    }
    t |> run("per-iteration local (shadow-collision regression)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_loop_local_flat(v), f_loop_local(v))
        }
    }
    t |> run("nested loops with per-level locals (double rename)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_nested_local_flat(v), f_nested_local(v))
        }
    }
    t |> run("identity folds: x*0 / x*1 over an unrolled range") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_loop_mul_flat(v), f_loop_mul(v))
        }
    }
    t |> run("identity fold: x*-1 -> -x (negative loop index)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_loop_negidx_flat(v), f_loop_negidx(v))
        }
    }
    t |> run("urange source (uint loop var, cast per copy)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_urange_flat(v), f_urange(v))
        }
    }
    t |> run("urange with non-zero start") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_urange_start_flat(v), f_urange_start(v))
        }
    }
    t |> run("parallel multi-source (two arrays, lockstep)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_parallel_flat(v), f_parallel(v))
        }
    }
    t |> run("parallel mixed sources (range + array)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_parallel_mix_flat(v), f_parallel_mix(v))
        }
    }
    t |> run("three parallel sources") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_parallel_three_flat(v), f_parallel_three(v))
        }
    }
    t |> run("parallel loop with per-iteration local (multi-iterator rename)") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_parallel_local_flat(v), f_parallel_local(v))
        }
    }
    t |> run("parallel loop containing a nested loop") @(t : T?) {
        for (v in GRID_LOOPS) {
            t |> equal(f_parallel_nested_flat(v), f_parallel_nested(v))
        }
    }
}

// M4.1 differential tests: `break` / `continue` inside an unrolled
// loop. break narrows a loop-scoped live mask that persists across
// unrolled copies (rest of the breaking copy + every later copy masks
// off); continue narrows a per-copy mask (rest of THIS copy masks
// off, next copy resets it). Each twin is run against its original
// over an input grid AND (via the macro's cycle-2 scan) proven
// branchless + call-free. The interesting interactions:
// break/continue composing with the function-return mask, with nested
// loops (inner break must not touch the outer loop), and with an
// inlined callee whose own early `return` must NOT kill any of the
// caller's masks (frame isolation).

def sq(a : int) : int => a * a

// ----- runtime break (depends on x) -----
[flatten]
def f_break(x : int) : int {
    var acc = 0
    for (i in range(6)) {
        if (i > x) {
            break
        }
        acc += i
    }
    return acc
}

// ----- runtime continue (depends on x) -----
[flatten]
def f_continue(x : int) : int {
    var acc = 0
    for (i in range(6)) {
        if (i == x) {
            continue
        }
        acc += i
    }
    return acc
}

// ----- const-foldable break (cond is a function of the unroll
// constant only) -----
[flatten]
def f_break_const(x : int) : int {
    var acc = 0
    for (i in range(8)) {
        if (i >= 4) {
            break
        }
        acc += x + i
    }
    return acc
}

// ----- const-foldable continue -----
[flatten]
def f_continue_const(x : int) : int {
    var acc = 0
    for (i in range(6)) {
        if (i % 2 == 0) {
            continue
        }
        acc += x * i
    }
    return acc
}

// ----- both in one loop -----
[flatten]
def f_break_and_continue(x : int) : int {
    var acc = 0
    for (i in range(8)) {
        if (i == 1) {
            continue
        }
        if (i > x) {
            break
        }
        acc += i
    }
    return acc
}

// ----- break inside a nested if (structPred non-null at the break)
// -----
[flatten]
def f_break_nested_if(x : int) : int {
    var acc = 0
    for (i in range(6)) {
        if (i > 0) {
            if (i * 2 > x) {
                break
            }
            acc += i
        }
    }
    return acc
}

// ----- nested loops, inner break only (inner mask must not stop the
// outer loop) -----
[flatten]
def f_nested_inner_break(x : int) : int {
    var acc = 0
    for (i in range(3)) {
        for (j in range(4)) {
            if (j > i) {
                break
            }
            acc += x + i * 10 + j
        }
        acc += 100          // runs every outer iteration — inner break must not reach here
    }
    return acc
}

// ----- nested loops, break in the OUTER body after the inner loop
// -----
[flatten]
def f_nested_outer_break(x : int) : int {
    var acc = 0
    for (i in range(4)) {
        for (j in range(2)) {
            acc += j
        }
        if (i >= x) {
            break
        }
        acc += i * 10
    }
    return acc
}

// ----- break then return: a return after the loop must still fire
// -----
[flatten]
def f_break_then_return(x : int) : int {
    var acc = 0
    for (i in range(5)) {
        if (i > x) {
            break
        }
        acc += i
    }
    return acc + 1000       // break leaves the loop; this return must run
}

// ----- return INSIDE the loop (interacts with break mask in the same
// body) -----
[flatten]
def f_return_in_loop(x : int) : int {
    var acc = 0
    for (i in range(6)) {
        if (i == x) {
            return acc + 50     // early function return from inside the loop
        }
        if (i >= 4) {
            break
        }
        acc += i
    }
    return acc
}

// ----- raymarch idiom: accumulate, break on a runtime hit, use the
// result after -----
[flatten]
def f_raymarch_like(x : int) : int {
    var hit = -1
    var steps = 0
    for (i in range(8)) {
        steps ++
        if (i * i >= x) {
            hit = i
            break
        }
    }
    return hit * 100 + steps
}

// ===== callee-return frame isolation =====
// An inlined callee's early `return` narrows the callee's OWN frame
// mask only. It must not kill the caller's funcLive / loopLive /
// iterLive — so the caller's work after the call (same copy, later
// copies, after the loop) still runs.

def hlp(x : int) : int {
    if (x > 5) {
        return 100          // early return inside the callee
    }
    return x * 2
}

// no loop: callee return must not kill the caller's funcLive
[flatten]
def f_callee_ret(x : int) : int {
    var acc = hlp(x)
    acc += x                // MUST run regardless of hlp's internal return
    return acc + 7          // …and this
}

def hlp2(x : int; i : int) : int {
    if (i > x) {
        return 0            // callee early-returns internally
    }
    return i
}

// inside a loop WITH break/continue: callee return must not touch
// loopLive/iterLive/funcLive
[flatten]
def f_loop_callee_ret(x : int) : int {
    var acc = 0
    for (i in range(5)) {
        if (i == 4) {
            continue
        }
        if (i > x + 3) {
            break
        }
        acc += hlp2(x, i)   // callee return must not kill any caller mask
        acc ++              // must run after the call (same copy)
    }
    acc += 100              // must run after the loop
    return acc
}

// ===== void entry: break gating a global write =====
var g_sink : int

def f_void_break_impl(x : int) {
    g_sink = 0
    for (i in range(6)) {
        if (i > x) {
            break
        }
        g_sink = g_sink + i
    }
}

[flatten]
def f_void_break(x : int) {
    g_sink = 0
    for (i in range(6)) {
        if (i > x) {
            break
        }
        g_sink = g_sink + i
    }
}

// ----- driver -----

var GRID_BC <- [-3, -1, 0, 1, 2, 3, 4, 5, 7, 10]

[test(description = "break and continue inside unrolled loops, alone, nested, folded to constants or beside returns, produce twins that match their originals")]
def test_flatten_break_continue(t : T?) {
    t |> run("runtime break") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_break_flat(v), f_break(v))
        }
    }
    t |> run("runtime continue") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_continue_flat(v), f_continue(v))
        }
    }
    t |> run("const-foldable break") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_break_const_flat(v), f_break_const(v))
        }
    }
    t |> run("const-foldable continue") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_continue_const_flat(v), f_continue_const(v))
        }
    }
    t |> run("break and continue in one loop") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_break_and_continue_flat(v), f_break_and_continue(v))
        }
    }
    t |> run("break inside a nested if") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_break_nested_if_flat(v), f_break_nested_if(v))
        }
    }
    t |> run("nested loops, inner break only") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_nested_inner_break_flat(v), f_nested_inner_break(v))
        }
    }
    t |> run("nested loops, break in the outer body") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_nested_outer_break_flat(v), f_nested_outer_break(v))
        }
    }
    t |> run("break then a return after the loop") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_break_then_return_flat(v), f_break_then_return(v))
        }
    }
    t |> run("return inside the loop, alongside break") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_return_in_loop_flat(v), f_return_in_loop(v))
        }
    }
    t |> run("raymarch idiom: accumulate then break on hit") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_raymarch_like_flat(v), f_raymarch_like(v))
        }
    }
    t |> run("callee early return does not kill caller funcLive") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_callee_ret_flat(v), f_callee_ret(v))
        }
    }
    t |> run("callee return does not kill loop masks") @(t : T?) {
        for (v in GRID_BC) {
            t |> equal(f_loop_callee_ret_flat(v), f_loop_callee_ret(v))
        }
    }
    t |> run("void entry: break gates a global write") @(t : T?) {
        for (v in GRID_BC) {
            f_void_break_impl(v)
            let ref = g_sink
            f_void_break_flat(v)
            t |> equal(g_sink, ref)
        }
    }
}

// Regression: a [flatten] function calling a pure helper whose
// for-over-const-array loop body DSE's to empty used to SIGSEGV the
// constant-folder — it simulated the fused single-statement for-node
// (`loop1`, chosen from the AST statement count) with a null body
// list, then read list[0]. Compiling this file is the test; the
// differential confirms the flattened twin still matches the pristine
// original.

def helper_e(p0, p1 : bool) : bool {
    for (_i in [-1, -2, 4]) {
        var v = ((!false) != (p1 || false))   // nolint:LINT002 -- intentional dead store: the single body statement that DSE's to 0 SimNodes (the crash trigger)
    }
    return (!(!p0))
}

[flatten]
def cand_e(a, b, _c : bool) : bool {
    if (((!b) == (false && b))) {
        var v225 = helper_e(((false || true) && true), (!(!false)))
        v225 = (v225 != (false || a))
    }
    return false
}

[test(description = "a flatten function whose helper loop body is eliminated to empty compiles without crashing the constant folder and its twin matches the original")]
def test_flatten_empty_loop_fold(t : T?) {
    for (a in [false, true]) {
        for (b in [false, true]) {
            for (c in [false, true]) {
                t |> equal(cand_e(a, b, c), cand_e_flat(a, b, c))
            }
        }
    }
}

// A predicated `++` / `--` lowers to `x + 1` / `x - 1`; the
// synthesized 1 must carry x's own type, or a uint / int64 / uint64 /
// float counter fails to type-check.

[flatten]
def count_u(x : int) : uint {
    var u = 0u
    for (i in range(4)) {
        if (i > x) {
            u++
        }
    }
    return u
}

[flatten]
def count_l(x : int) : int64 {
    var l = 0l
    for (i in range(4)) {
        if (i > x) {
            l--
        }
    }
    return l
}

[flatten]
def count_ul(x : int) : uint64 {
    var ul = 10ul
    for (i in range(4)) {
        if (i > x) {
            ul--
        }
    }
    return ul
}

[flatten]
def count_f(x : int) : float {
    var f = 0.0
    for (i in range(4)) {
        if (i > x) {
            f++
        }
    }
    return f
}

[flatten]
def count_d(x : int) : double {
    var d = 0.0lf
    for (i in range(4)) {
        if (i > x) {
            d--
        }
    }
    return d
}

[test(description = "a predicated increment keeps the counter's own type for uint, int64, uint64, float and double counters")]
def test_predicated_increment_keeps_the_counter_type(t : T?) {
    t |> run("uint / int64 / uint64 / float / double counters under a runtime predicate") @(t : T?) {
        t |> equal(count_u(1), 2u)
        t |> equal(count_l(1), -2l)
        t |> equal(count_ul(1), 8ul)
        t |> equal(count_f(1), 2.0)
        t |> equal(count_d(1), -2.0lf)
    }
}

// Cross-module integration test for the PUBLIC `flatten_function` /
// `flatten_fold` entries.
//
// `_flat_api_macro` (a separate module, the real backend topology)
// calls `flatten_function` then `flatten_fold` directly with its own
// whitelist {max, min}. This proves both entries are callable from
// another module's macro, honor a caller-supplied primitive set, and
// that the post-infer fold collapses const selects. The flattened
// twin must match an unflattened reference.

// Unflattened reference (identical body) for differential comparison.
def ref_max_min(x : int) : int => max(x, 0) + min(x, 10)

[flat_api]
def api_demo(x : int) : int => max(x, 0) + min(x, 10)

// A recursive user function the backend whitelists as a leaf
// primitive. flatten must KEEP the call (whitelist precedence), not
// inline it — inlining a recursive function would trip the recursion
// boundary and fail the compile. That this compiles + matches the
// reference proves whitelisted calls survive un-inlined even though
// they have a (here recursive) body — the [hint]/[stub] shader case.
def myrec(n : int) : int => n <= 0 ? 0 : n + myrec(n - 1)

def ref_rec(x : int) : int => myrec(x) + 1

[flat_api]
def api_rec(x : int) : int => myrec(x) + 1

// A loop-var-gated accumulator: each unrolled copy's `i >= 3` is a
// compile-time constant, so flatten_fold (run via the backend-style
// cycle 2 in _flat_api_macro) must collapse the selects and drop the
// false-iteration self-assigns. Matching the reference proves the
// PUBLIC fold companion works cross-module.
def ref_loopcond(x : int) : int {
    var acc = 0
    for (i in range(5)) {
        if (i >= 3) {
            acc += x
        }
    }
    return acc
}

[flat_api]
def api_loopcond(x : int) : int {
    var acc = 0
    for (i in range(5)) {
        if (i >= 3) {
            acc += x
        }
    }
    return acc
}

// ----- ?: fold side-effect safety -----
// A whitelisted "primitive" WITH a side effect (global write).
// flatten keeps the call (whitelist precedence). The const conditions
// below arise from loop unrolling (i substituted to a literal), so
// `flatten_fold`'s `const ? a : b` collapse is what actually fires.
// The fold must keep the call in the LIVE arm and drop it from the
// DEAD arm (matching the source `if`, which never runs the dead
// branch).
var g_calls : int

def g_writer(x : int) : int {
    g_calls ++     // the observable side effect under test
    return x + 100
}

def ref_keep(x : int) : int {
    var r = -1
    for (i in range(1)) {
        if (i >= 0) {       // i=0 → true: g_writer IS called
            r = g_writer(x)
        }
    }
    return r
}

[flat_api]
def api_keep(x : int) : int {
    var r = -1
    for (i in range(1)) {
        if (i >= 0) {
            r = g_writer(x)
        }
    }
    return r
}

def ref_drop(x : int) : int {
    var r = -1
    for (i in range(1)) {
        if (i >= 1) {       // i=0 → false: g_writer is NOT called
            r = g_writer(x)
        }
    }
    return r
}

[flat_api]
def api_drop(x : int) : int {
    var r = -1
    for (i in range(1)) {
        if (i >= 1) {
            r = g_writer(x)
        }
    }
    return r
}

// `e * 0 → 0` is gated by has_sideeffects(e): a side-effecting `e`
// must NOT be dropped. i=0 makes `g_writer(x) * 0`; the call must
// survive (global incremented) even though the product is statically
// 0.
def ref_mul0(x : int) : int {
    var acc = 0
    for (i in range(1)) {
        acc += g_writer(x) * i      // i=0 → g_writer(x) * 0
    }
    return acc
}

[flat_api]
def api_mul0(x : int) : int {
    var acc = 0
    for (i in range(1)) {
        acc += g_writer(x) * i
    }
    return acc
}

var GRID_API <- [-5, -1, 0, 1, 5, 10, 20]

[test(description = "flatten_function and flatten_fold called from another module's macro honor the caller's whitelist, collapse constant selects and keep or drop side-effecting calls as the source semantics require")]
def test_flatten_api(t : T?) {
    t |> run("flatten_function callable cross-module with custom whitelist") @(t : T?) {
        for (v in GRID_API) {
            t |> equal(api_demo_apiflat(v), ref_max_min(v))
        }
    }
    t |> run("whitelisted recursive user fn is kept, not inlined") @(t : T?) {
        for (v in GRID_API) {
            t |> equal(api_rec_apiflat(v), ref_rec(v))
        }
    }
    t |> run("flatten_fold collapses const selects cross-module") @(t : T?) {
        for (v in GRID_API) {
            t |> equal(api_loopcond_apiflat(v), ref_loopcond(v))
        }
    }
    t |> run("?: fold KEEPS a side-effecting call in the const-true arm") @(t : T?) {
        for (v in GRID_API) {
            g_calls = 0
            let refv = ref_keep(v)
            let refCalls = g_calls          // 1 — source runs g_writer
            g_calls = 0
            let flatv = api_keep_apiflat(v)
            t |> equal(flatv, refv)
            t |> equal(g_calls, refCalls)   // the twin must ALSO call g_writer
        }
    }
    t |> run("?: fold DROPS a side-effecting call from the dead arm (matches source)") @(t : T?) {
        for (v in GRID_API) {
            g_calls = 0
            let refv = ref_drop(v)
            let refCalls = g_calls          // 0 — source's `if (false)` never calls g_writer
            g_calls = 0
            let flatv = api_drop_apiflat(v)
            t |> equal(flatv, refv)
            t |> equal(g_calls, refCalls)   // the twin must ALSO not call it
        }
    }
    t |> run("e*0 fold is side-effect-gated: g_writer(x)*0 keeps the call") @(t : T?) {
        for (v in GRID_API) {
            g_calls = 0
            let refv = ref_mul0(v)
            let refCalls = g_calls          // 1 — g_writer runs even though *0
            g_calls = 0
            let flatv = api_mul0_apiflat(v)
            t |> equal(flatv, refv)
            t |> equal(g_calls, refCalls)   // the twin must ALSO call g_writer (not fold *0 away)
        }
    }
}
