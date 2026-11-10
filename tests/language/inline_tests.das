options gen2
options auto_inline_functions = true

require dastest/testing_boost public
require daslib/math_boost
require UnitTest
require math
require _inline_shadow_helper

enum Color {
    red
    green
}

struct Defaults {
    a : int = 7
    b : int = 11
}

struct Built {
    v : int
}

def Built(n : int) => Built(v = n * 2)

[inline]
def swizzle_sum(v : float3) : float {
    var r = v.x
    if (v.z > 100.0) {
        return 0.0
    }
    r += v.yz.x * 10.0
    return r
}

[inline]
def built_value(n : int) : int {
    var p = new Built(n)
    let r = p.v
    unsafe {
        delete p
    }
    return r
}

[inline]
def defaults_sum() : int {
    let d = Defaults()
    return d.a + d.b
}

def take_witness(v : auto(VT)) : int => typeinfo sizeof(v) + (v == default<VT> ? 100 : 0)

[inline]
def pass_witness(t : auto(TT)) : int => take_witness(t)

[never_inline]
def other_array() : array<int> => <- [7, 8, 9]

[inline]
def made_array(n : int) : array<int> {
    var a <- [for (i in range(n)); i]
    if (n > 100) {
        return <- a
    }
    return <- a
}

[inline]
def falsy(x : int) : bool {
    var r = false
    if (x > 100) {
        r = true
        return r
    }
    return r
}

[inline]
def loop_sum(a : array<int>) : int {
    var s = 0
    for (x in a) {
        s += x
    }
    if (s > 100000) {
        return 0
    }
    return s
}

var g_bumped = 0

[inline]
def bump(a : int) {
    var r = a
    if (a > 100) {
        return
    }
    r += a
    g_bumped += r
}

def bump_in_labeled_block() : int {
    g_bumped = 0
    var i = 0
    label 0:
    bump(i)
    i ++
    if (i < 3) {
        goto label 0
    }
    return g_bumped
}

[test(description = "an auto-inlined body clone keeps its inferred state: swizzles, constructor calls, field initializers and by-value type witnesses behave as in the call")]
def test_cloned_body_keeps_inferred_state(t : T?) {
    t |> run("a swizzle in a spliced body reads the fields it named") @(t : T?) {
        t |> equal(swizzle_sum(float3(1.0, 2.0, 3.0)), 21.0)
    }
    t |> run("new with a constructor in a spliced body calls the constructor") @(t : T?) {
        t |> equal(built_value(5), 10)
    }
    t |> run("a struct literal in a spliced body runs its field initializers") @(t : T?) {
        t |> equal(defaults_sum(), 18)
    }
    t |> run("a type witness passed on by value becomes the type's zero") @(t : T?) {
        t |> equal(pass_witness(type<int>), 104)
        t |> equal(pass_witness(type<Color>), 104)
        t |> equal(pass_witness(type<float>), 104)
    }
}

[test(description = "splices lowered into a conditional move, an &&= right side, a loop and a compiled cross-module callee all compute the call's result")]
def test_splice_lowering(t : T?) {
    t |> run("a conditional array result moves into its declaration") @(t : T?) {
        let c = !empty(other_array())
        let arr <- c ? made_array(4) : other_array()
        t |> equal(length(arr), 4)
        t |> equal(arr[3], 3)
        let other <- !c ? made_array(4) : other_array()
        t |> equal(other[0], 7)
    }
    t |> run("&&= with an [inline] right side stores the result") @(t : T?) {
        var b = true
        b &&= falsy(1)
        t |> equal(b, false)
        var skipped = false
        skipped &&= falsy(200)
        t |> equal(skipped, false)
    }
    t |> run("a loop in a spliced body iterates its own source") @(t : T?) {
        var a <- [1, 2, 3, 4]
        t |> equal(loop_sum(a), 10)
    }
    t |> run("a compiled callee from another module splices with its reads unfolded") @(t : T?) {
        let x = 0.25
        t |> success(abs(linear_to_SRGB(x) - (1.055 * pow(x, 1.0 / 2.4) - 0.055)) < 0.001)
    }
}

[test(description = "an [inline] callee with an early return splices correctly at a call between a label and a goto back to it")]
def test_splice_in_labeled_block(t : T?) {
    t |> equal(bump_in_labeled_block(), 6)
}

def pure_sum(a : int = 13; b : int) => a + b

def discard_sum(x : int) : int {
    pure_sum(x, 2)
    pure_sum([b = 2])
    return x
}

[test(description = "a pure inlined call whose result is discarded, named-argument form included, compiles and leaves the caller's value alone")]
def test_discarded_pure_inline_result(t : T?) {
    t |> equal(discard_sum(5), 5)
}

// doubleSamplePoint3 is bound from C++ as `Point3
// doubleSamplePoint3(const Point3 &)`, so its parameter is an
// implicit const ref.
//
// `private` + referenced once makes this callee budget-exempt, so it
// splices even though the body is over the heuristic budget. The
// splice substitutes the caller's argument EXPRESSION for `p`, and
// the caller passes `a + b` built from two const locals - which
// constant-folds. So the folder would evaluate
// doubleSamplePoint3(<constant>) at compile time and dereference a
// `const Point3 &` that was never materialized. Same family as
// fold_extern_at_compile.das - the compiler must not execute a
// C++-bound function. A non-constant argument does not trigger it:
// the trigger is specifically inlining + constant propagation + a
// native const-ref param.
def private big_callee(p : float3; k : float) : float3 {
    var acc = float3()
    var i = 0
    while (i < 2) { acc += float3(float(i)); i++ }
    let scaled = acc * k
    let doubled = UnitTest::doubleSamplePoint3(p)
    var out = doubled + scaled
    out.x += 0.
    return out
}

[test(description = "an inlined callee passing a constant-folded argument to a C++-bound const-ref function is not evaluated at compile time and returns the run-time value")]
def test_no_compile_time_call_of_native_const_ref(t : T?) {
    let a = float3(1, 2, 3)
    let b = float3(1, 0, 0)
    t |> equal(big_callee(a + b, 0.), float3(4, 4, 6))
}

// A spliced block literal declares ARGUMENTS in the caller's scope
// chain, and infer re-resolves every ExprVar by name. Unless the
// splice renames them, a substituted argument expression that spells
// the same name captures the block's variable instead: a plain block
// reports 30701 at the callee, and a can_shadow block (host query
// blocks, linq_boost) is silently wrong.

[never_inline] def apply_team(blk : block<(team : int) : int>) : int => invoke(blk, 5)

def callee(squad_team : int) : int {
    return apply_team() $ [shadowable] (team : int) : int {
        return squad_team * 100 + team
    }
}

def caller(team : int) : int => callee(team)

// the explicit [inline] flavor of this shape is a hard error by
// design (30100) - the can_shadow argument name is semantic, so the
// capture cannot be renamed away and the demanded splice must be
// refused; pinned in failed_inline_shadow_capture.das

// same shape without can_shadow: the capture is a compile error
// (30701) rather than a wrong value, so this function failing to
// rename does not compile at all
def plain_callee(squad_team : int) : int {
    return apply_team() $(team : int) : int {
        return squad_team * 100 + team
    }
}

def plain_caller(team : int) : int => plain_callee(team)

[test(description = "inlining a caller whose argument spells a can_shadow block argument's name does not capture the block's variable")]
def test_no_capture_when_inlined(t : T?) {
    t |> equal(caller(7), 705)
}

[test(description = "inlining a caller whose argument spells a plain block argument's name renames it, so it compiles and does not capture")]
def test_no_capture_without_can_shadow(t : T?) {
    t |> equal(plain_caller(7), 705)
}

// A devirt site that DECLINES (can_shadow capture risk) after the
// eager path already hoisted its impure evaluation prefix must keep
// the `__inl<N>_pre*` temp declarations it references: otherwise the
// statement is rewritten in place and the decline discards the
// pending temps - 30838 at best, a segfault in the access-flags pass
// at worst. All four ingredients are load-bearing: two devirt sites
// in one statement, an if-condition (forces the eager path), an
// impure literal body (forces the prefix hoist), and a caller
// declaration spelling the can_shadow name.

var g_prefix_hits = 0

def apply2(k : block<(team : int) : int>) : int {
    var minn = 1   // nolint:LINT003 — a `let` would const-fold into k(1) and change the pinned devirt shape
    if (k(1) < k(minn)) {
        return 1
    }
    return 0
}

// the function ARGUMENT spells the can_shadow name (a `var team` let
// would also trip the pre-existing aot_module RenameVar shadow bug,
// which is not what this test pins)
def run_it(team : int) : int {
    let r = apply2() $ [shadowable] (team : int) : int {
        g_prefix_hits += team   // impure: forces the prefix hoist
        return team * 2
    }
    return r + team
}

[test(description = "a devirt site that declines on can_shadow capture after hoisting its impure prefix keeps the statement and its temps, and runs the block each time")]
def test_declined_site_keeps_its_statement(t : T?) {
    g_prefix_hits = 0
    t |> equal(run_it(10), 10)
    t |> equal(g_prefix_hits, 2)
}

// Minimal shape, reduced from a host ECS migration script: my_region
// has an expression body, so its call site takes the pure-graft
// splice. Its block literal declares `transform`, which the graft
// renames to __inl1_l_transform. The caller's callback -- substituted
// into that body -- has its own outer `transform` and ALSO contains a
// later can_shadow block redeclaring the name. The caller's own
// reference `transform[3] = center` must stay the caller's, not be
// renamed into the callee's __inl1_l_ namespace (error 30838). Only
// the INNER [shadowable] matters: callee/outer/inner =
// any/any/shadowable -> 30838; any/any/plain -> 30701. Renaming the
// inner block argument also makes it compile, which a host cannot
// use: ECS query components must keep their component names, and the
// host marks every query block argument can_shadow.

[never_inline] def each_region(blk : block<(var tm : float3x4) : void>) {
    var t : float3x4
    invoke(blk, t)
}

def my_region(cb : block<(center : float3) : void>) {
    each_region() $(var transform : float3x4) {
        cb |> invoke(transform[3])
    }
}

[test(description = "a declined splice keeps a caller's outer reference distinct from a same-named can_shadow block argument nested in its callback, so the shape compiles and runs")]
def test_inline_shadow_nested_query(t : T?) {
    each_region() $(var transform : float3x4) {
        my_region() $(center : float3) {
            transform[3] = center
            each_region() $ [shadowable] (var transform : float3x4) {
                t |> equal(transform[3], float3(0))
            }
        }
    }
}
