options gen2
options rtti
options no_coverage

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require dastest/testing_boost public
require strings
require daslib/strings_boost
require _optimized_body_helper
require _inline_counter_helper
require _never_inline_module
require _inline_scope_helper
require _inline_unoptimized_scope_helper // nolint:STYLE030 call inlined
require _inline_unsafe_helper

// structural FIRED proof + behavioral safety net for [inline] splicing
// (src/ast/ast_inline.cpp): every direct call to an [inline] function is replaced
// with the callee body during the patch slot. leaf and pure-single-use arguments
// substitute textually; impure arguments bind call-site temps in call order; `var`
// by-value params become mutable local copies; while / elif / && / ?: positions
// lower first. the behavioral tests pin evaluation order and snapshot semantics.

var g_log : string = ""
var g_count = 0

def side(tag : string; v : int) : int {
    g_log += tag;
    return v;
}

struct SVec {
    x : int;
    y : int;
}

[inline]
def add(a, b : int) : int => a + b

[inline]
def clamp01f(x : float) : float =>
    x < 0.0 ? 0.0 : (x > 1.0 ? 1.0 : x)  // nolint:PERF015 -- ternaries in the inline body are the point

[inline]
def madd(a, b, c : int) : int {
    let t = a * b;
    return t + c;
}

[inline]
def bump(var x : int) : int {
    x++;
    return x;
}

[inline]
def add_g(a : int) : int {
    g_count += 10;
    return a + g_count;
}

[inline]
def vsum(v : SVec) : int => v.x + v.y

[inline]
def wsum(var v : SVec) : int {
    v.x++;
    return v.x + v.y;
}

[inline]
def read_after_write(a : int; var s : SVec) : int {
    s.x = 100;
    return a;
}

[inline]
def set_y(var s : SVec) {
    s.y = 99;
}

[inline]
def sum_range(n : int) : int {
    var s = 0;
    for (k in range(n)) {
        s += k;
    }
    return s;
}

[inline]
def chain_inner(x : int) : int => x + 1

[inline]
def chain_outer(x : int) : int => chain_inner(x) + 2

[inline]
def twice(x : auto) => x + x

[inline]
def guard_ladder(x : int) : int {
    if (x < 0) {
        return -1;
    }
    if (x == 0) {
        return 0;
    }
    return 1;
}

[inline]
def both_arms(x : int) : int {
    if (x > 0) {
        let a = x * 2;
        return a + 1;
    } else {
        let b = x * 3;
        return b - 1;
    }
}

[inline]
def find_first_gt(src : array<int>; k : int) : int {
    for (v in src) {
        if (v > k) {
            return v; // return under a loop: rides the generated flag
        }
    }
    return -1;
}

[inline]
def find_nested(m : array<array<int>>; k : int) : int {
    for (row in m) {
        for (v in row) {
            if (v == k) {
                return 1; // nested loops: the cascade breaks both
            }
        }
    }
    return 0;
}

[inline]
def early_void(var s : SVec; x : int) {
    if (x < 0) {
        return ;
    }
    s.y = x;
}

[inline]
def move_under_loop(n : int) : array<int> {
    for (k in range(n)) {
        if (k == 3) {
            var inscope early <- [k, k]; // nolint:PERF009 -- named-local move-return is the shape under test
            return <- early;
        }
    }
    var full <- [n]; // nolint:PERF009 -- same: pins ExprMove from a typed local through the flag store
    return <- full;
}

[inline]
def loop_ret_finally(n : int) : int {
    var seen = 0;
    {
        for (k in range(n)) {
            if (k == 2) {
                return 100 + seen; // the block's finally must still run on this exit
            }
            seen++;
        }
    } finally {
        g_count = seen;
    }
    return seen;
}

def for_each_hlp(src : array<int>; blk : block<(v : int) : void>) {
    for (v in src) {
        invoke(blk, v);
    }
}

[inline]
def gathered_sum(src : array<int>; k : int) : int {
    // a block literal in the body: stays in-frame, clones with the splice
    var total = 0;
    src |> for_each_hlp() $(v) {
        if (v > k) {
            total += v;
        }
    }
    return total;
}

[export]
def target_basic(x, y : int) : int => add(x, y + 1)  // inlines to x + (y + 1), zero temps

[export]
def target_fold : float =>
    clamp01f(2.5)  // nolint:PERF015,LINT019 -- constant arg folds through the inlined body; the inliner splices clamp01f's nolinted ternary to this call site in optimized lint configs

[export]
def target_multi_stmt(x : int) : int {
    return madd(x, 4, 5);
}

[export]
def target_generic(x : int; f : float) : float =>
    float(twice(x)) + twice(f)  // one instance per type, both inlined

[export]
def target_while(x : int) : int {
    var i = 0;
    while (madd(i, 1, 0) < x) { // statement-needing callee in a while cond: lowered
        i++;
    }
    return i;
}

[export]
def target_lazy(q : int) : int {
    let c = (q > 0) && (madd(q, 1, 1) > 0); // && right side: lowered to if-form
    let s = q > 0 ? madd(q, 1, 0) : madd(q, 2, 0); // ?: arms: lowered to if-form
    return (c ? 1 : 0) + s;
}

[export]
def target_guard(x : int) : int {
    return guard_ladder(x); // canonicalization makes it terminal: splices flag-free
}

[export]
def target_both_arms(x : int) : int {
    return both_arms(x); // terminal returns in both arms: in-place stores, no flag
}

[export]
def target_loop_ret(src : array<int>; k : int) : int {
    return find_first_gt(src, k); // return under a loop: generated flag + break
}

def has(s, sub : string) : bool => find(s, sub, 0) >= 0

def count_of(s, sub : string) : int => length(split(s, sub)) - 1

[test(description = "every direct call to an [inline] function is replaced by its body - expression, multi-statement, generic, multi-return and loop-return bodies alike - binding temps only where an argument needs one")]
def test_inline_fired(t : T?) {
    ast_gc_guard() {
        t |> run("expression body with leaf + pure args grafts in place") @(t : T?) {
            let b = body_of(t, "target_basic");
            t |> equal(count_of(b, "add("), 0);
            t |> success(!has(b, "__inl"), "no temps for leaf and pure single-use args: {b}");
        }
        t |> run("constant arg folds through the inlined body") @(t : T?) {
            let b = body_of(t, "target_fold");
            t |> equal(count_of(b, "clamp01f("), 0);
        }
        t |> run("multi-statement body splices") @(t : T?) {
            let b = body_of(t, "target_multi_stmt");
            t |> equal(count_of(b, "madd("), 0);
        }
        t |> run("generic instances inline per instantiation") @(t : T?) {
            let b = body_of(t, "target_generic");
            t |> equal(count_of(b, "twice("), 0);
        }
        t |> run("while condition lowers, call inlines inside the loop") @(t : T?) {
            let b = body_of(t, "target_while");
            t |> equal(count_of(b, "madd("), 0);
        }
        t |> run("lazy operators lower, calls inline in the branches") @(t : T?) {
            let b = body_of(t, "target_lazy");
            t |> equal(count_of(b, "madd("), 0);
        }
        t |> run("guard ladder canonicalizes and splices with no flag") @(t : T?) {
            let b = body_of(t, "target_guard");
            t |> equal(count_of(b, "guard_ladder("), 0);
            t |> success(!has(b, "_ret"), "terminal returns need no flag: {b}");
        }
        t |> run("both-arms-return body splices with in-place stores") @(t : T?) {
            let b = body_of(t, "target_both_arms");
            t |> equal(count_of(b, "both_arms("), 0);
            t |> success(!has(b, "_ret"), "terminal returns need no flag: {b}");
        }
        t |> run("return under a loop splices through the generated flag") @(t : T?) {
            let b = body_of(t, "target_loop_ret");
            t |> equal(count_of(b, "find_first_gt("), 0);
            t |> success(has(b, "_ret"), "loop return rides the flag: {b}");
        }
    }
}

[test(description = "inlined calls compute the same values as calls, evaluating impure arguments once in call order and keeping by-value, by-ref, global-snapshot and finally semantics")]
def test_inline_behavior(t : T?) {  // nolint:STYLE038 - flat list of independent test arms
    t |> run("values preserved") @(t : T?) {
        t |> equal(target_basic(5, 6), 12);
        t |> equal(target_fold(), 1.0);
        t |> equal(target_multi_stmt(3), 17);
        t |> equal(target_generic(21, 1.5), 45.0);
        t |> equal(target_while(5), 5);
        t |> equal(target_lazy(41), 42);
    }
    t |> run("body with a block literal splices") @(t : T?) {
        let arr <- [3, 9, 1, 7];
        t |> equal(gathered_sum(arr, 2), 19);
        t |> equal(gathered_sum(arr, 6), 16);
    }
    t |> run("impure args evaluate once, in call order") @(t : T?) {
        g_log = "";
        let r = add(side("a", 1), side("b", 2));
        t |> equal(r, 3);
        t |> equal(g_log, "ab");
    }
    t |> run("impure prefix stays ahead of arg temps") @(t : T?) {
        g_log = "";
        let r = side("p", 10) + add(side("q", 1), side("r", 2));
        t |> equal(r, 13);
        t |> equal(g_log, "pqr");
    }
    t |> run("var by-value param is a private mutable copy") @(t : T?) {
        var q = 41; // nolint:LINT003 -- mutable on purpose: prove the callee's copy never writes back
        t |> equal(bump(q), 42);
        t |> equal(q, 41);
    }
    t |> run("global arg keeps snapshot semantics against global writes") @(t : T?) {
        g_count = 0;
        t |> equal(add_g(g_count), 10);
    }
    t |> run("struct by-ref params alias like the call did") @(t : T?) {
        var v = SVec(x = 3, y = 4);
        t |> equal(vsum(v), 7);
        t |> equal(wsum(v), 8);
        t |> equal(v.x, 4);
    }
    t |> run("pure memory-reading arg binds a temp when the body writes it") @(t : T?) {
        var v = SVec(x = 7, y = 0);
        t |> equal(read_after_write(v.x, v), 7);
        t |> equal(v.x, 100);
    }
    t |> run("void inline as a statement") @(t : T?) {
        var v = SVec(x = 1, y = 2);
        set_y(v);
        t |> equal(v.y, 99);
    }
    t |> run("callee locals and loop vars rename around caller names") @(t : T?) {
        let s = 1000;
        let k = 2000;
        t |> equal(sum_range(4), 6);
        t |> equal(s, 1000);
        t |> equal(k, 2000);
    }
    t |> run("nested calls and inline chains") @(t : T?) {
        t |> equal(add(add(1, 2), add(3, 4)), 10);
        t |> equal(chain_outer(5), 8);
    }
    t |> run("statement-needing callee in if and elif conditions") @(t : T?) {
        var hits = 0;
        if (madd(1, 2, 3) == 5) {
            hits ++;
        }
        if (false) {
            hits += 100;
        } elif (madd(1, 1, 1) == 2) {
            hits ++;
        }
        t |> equal(hits, 2);
    }
    t |> run("statement-needing callee in a for source") @(t : T?) {
        var total = 0;
        for (z in range(0, madd(1, 2, 1))) {
            total += z;
        }
        t |> equal(total, 3);
    }
    t |> run("multi-return values preserved") @(t : T?) {
        t |> equal(target_guard(-5), -1);
        t |> equal(target_guard(0), 0);
        t |> equal(target_guard(7), 1);
        t |> equal(target_both_arms(2), 5);
        t |> equal(target_both_arms(-1), -4);
    }
    t |> run("return under a loop exits at the right element") @(t : T?) {
        let arr <- [3, 9, 1, 7];
        t |> equal(target_loop_ret(arr, 5), 9);
        t |> equal(target_loop_ret(arr, 100), -1);
    }
    t |> run("return under nested loops breaks all the way out") @(t : T?) {
        var m : array<array<int>>;
        var r1 <- [1, 2];
        var r2 <- [3, 4];
        m |> emplace(r1);
        m |> emplace(r2);
        t |> equal(find_nested(m, 4), 1);
        t |> equal(find_nested(m, 9), 0);
    }
    t |> run("void early return skips the write") @(t : T?) {
        var v = SVec(x = 0, y = 7);
        early_void(v, -1);
        t |> equal(v.y, 7);
        early_void(v, 5);
        t |> equal(v.y, 5);
    }
    t |> run("move return under a loop moves into the result") @(t : T?) {
        let a <- move_under_loop(5);
        t |> equal(length(a), 2);
        t |> equal(a[0], 3);
        let b <- move_under_loop(2);
        t |> equal(length(b), 1);
        t |> equal(b[0], 2);
    }
    t |> run("block finally runs on a flagged loop exit") @(t : T?) {
        g_count = -1;
        t |> equal(loop_ret_finally(5), 102);
        t |> equal(g_count, 2);
        g_count = -1;
        t |> equal(loop_ret_finally(1), 1);
        t |> equal(g_count, 1);
    }
    t |> run("user let keeps its metadata through lazy lowering") @(t : T?) {
        // the ?: lowering must hoist to a generated temp, not rewrite the user
        // declaration in place - an in-place rewrite would drop the aka
        let qq = 41;
        let picked aka chosen = qq > 0 ? madd(qq, 1, 0) : madd(qq, 2, 0);
        t |> equal(chosen, 41);
        t |> equal(picked, 41);
    }
}

// cross-module splice of a body that already carries interior _inl0_* temps
// (src/ast/ast_inline.cpp): pins that the interior temps survive the splice through
// the rename join (never minting `__`) and that the site mints its own distinct ids.
// the cross-module counter BUMP itself has no distinguishing test - a one-site splice
// renames interior temps into the site's namespace either way

var g_cross_seed = 1

def impure() : int {
    g_cross_seed += 0
    return g_cross_seed
}

[never_inline]
def target_cross() : int {
    return cnt_outer(2 + impure())
}

[test(description = "a cross-module splice of a body carrying interior inline temps keeps their renamed ids, mints its own argument temp and never mints a double underscore")]
def test_counter_bump(t : T?) {
    g_cross_seed = 1
    ast_gc_guard() {
        t |> run("interior temps survive a cross-module splice, no `__` minted") @(t : T?) {
            let b = body_of(t, "target_cross")
            t |> success(has(b, "u_inl0_"), "interior temp lost its rename join: {b}")
            t |> success(has(b, "_arg_v"), "site arg temp missing: {b}")
            t |> success(!has(b, "__"), "GLSL-reserved `__` minted: {b}")
        }
    }
    t |> run("value") @(t : T?) {
        t |> equal(target_cross(), 8)
    }
}

// structural proof for `options never_inline` (src/ast/ast_inline.cpp); fixture
// module: _never_inline_module.das - the run labels below carry the claims

[inline]
def nvm_step(x : int) : int => x + 1

var g_seed = 1

[never_inline]  // structural anchors resist the heuristic tier (skills/internal/tests_in_repo.md)
def target_must() : int => nvm_add(g_seed, 2)

[never_inline]
def target_literal() : int {
    return nvm_apply(5 * g_seed) $(v : int) : int {
        return v * 2
    }
}

[never_inline]
def target_instance() : int => nvm_gen(6 + g_seed)

[never_inline]
def target_baseline() : int {
    return nvm_step(3 + g_seed)
}

[never_inline]
def target_canon() : int {
    return nvm_guarded(6 + g_seed) $(v : int) : int {
        return v * 2
    }
}

// a generic instance carries a mangled name (origin`name`hash) the exact-name
// for_each_function lookup misses - match by substring instead
def instance_body_of(t : T?; nm : string) : string {
    var body = ""
    compiling_module() |> for_each_function("") $(func) {
        if (empty(body) && find("{func.name}", nm) >= 0) {
            body = "{describe(func.body)}"
        }
    }
    t |> success(!empty(body), "instance not found")
    let clean <- [for (ln in split(body, "\n")); ln; where !(ln |> has("coverage("))]
    return join(clean, "\n")
}

[test(description = "calls into a never_inline module stay calls in every inline tier while the pass stays live elsewhere, and the module's neverInline flag is rtti-visible")]
def test_never_inline_module(t : T?) {
    g_seed = 1  // runtime taint, written here on purpose: pure targets with constant args fold + reap
    ast_gc_guard() {
        t |> run("[inline] callee from a never_inline module stays a call") @(t : T?) {
            let b = body_of(t, "target_must")
            t |> success(has(b, "nvm_add("), "call survives: {b}")
        }
        t |> run("block-literal tier declines a never_inline callee") @(t : T?) {
            let b = body_of(t, "target_literal")
            t |> success(has(b, "nvm_apply("), "call survives: {b}")
        }
        t |> run("generic instance neither inlines nor hosts splices") @(t : T?) {
            let b = body_of(t, "target_instance")
            t |> success(has(b, "nvm_gen"), "call survives: {b}")  // no paren: the instance call spells the mangled name
            let gb = instance_body_of(t, "nvm_gen")
            t |> success(has(gb, "nvm_step("), "instance body untouched: {gb}")
        }
        t |> run("baseline: the pass is live in this module") @(t : T?) {
            let b = body_of(t, "target_baseline")
            t |> success(!has(b, "nvm_step("), "[inline] splice fired: {b}")
        }
        t |> run("the pass skips the module's own bodies") @(t : T?) {
            var body = ""
            find_module(this_program(), "_never_inline_module") |> for_each_function("nvm_chain") $(func) {
                if (empty(body)) {
                    body = "{describe(func.body)}"
                }
            }
            t |> success(has(body, "nvm_add("), "internal [inline] call survives: {body}")
        }
        t |> run("flagged-origin instance skips return canonicalization") @(t : T?) {
            let gb = instance_body_of(t, "nvm_guarded")
            t |> success(!has(gb, "else"), "early-exit shape kept, no synthesized else: {gb}")
        }
        t |> run("the module bit is set and rtti-visible") @(t : T?) {
            t |> success(find_module(this_program(), "_never_inline_module").moduleFlags.neverInline, "neverInline flag")
        }
    }
    t |> run("targets still compute their values") @(t : T?) {
        t |> equal(target_must(), 3)
        t |> equal(target_literal(), 10)
        t |> equal(target_instance(), 8)
        t |> equal(target_baseline(), 5)
        t |> equal(target_canon(), 14)
    }
}

// structural FIRED proof + behavioral safety net for cross-module splices whose
// bodies use the callee module's private symbols (src/ast/ast_inline.cpp): the
// spliced body clone rides a generated `with (module <origin>)` wrapper, so its
// privates and require graph resolve exactly as at the callee's own compile.
// argument and prefix temps bind OUTSIDE the wrapper (the mirror rule): caller
// expressions keep resolving at the caller, proven by the g_shared collision test.

var g_shared = 555      // collides with _inline_scope_helper's g_shared on purpose

[export]
def target_scoped_add(a, b : int) : int {
    return scoped_add(a, b)
}

[export]
def target_scoped_pick(v : int) : int {
    return scoped_pick(v)
}

[export]
def target_mirror_rule() : int {
    return scoped_echo(g_shared)
}

[export]
def target_block_literal(src : array<int>) : int => scoped_filter_sum(src) $(v) => v % 2 == 0

[export]
def target_inside_with(a, b : int) : int {
    var r : int
    with (module _inline_scope_helper) {
        // the enclosing scope IS the resolution context - the splice lands bare here
        r = scoped_add(a, b)
    }
    return r
}

[export]
def target_visible_dependency() : string {
    var root : string
    with (module _inline_scope_helper) {
        root = must_inline_visibility_root()
    }
    return root
}

[export]
def target_generated_dependency_scope() : string =>
    must_inline_visibility_root()

[export]
def target_unoptimized_scope() : int => scoped_unoptimized_value()

[test(description = "an [inline] callee that uses its module's private symbols still splices, under a generated with (module) wrapper, and so does a call inside a user with (module) block")]
def test_scope_wrapped_inline_fired(t : T?) {
    ast_gc_guard() {
        t |> run("[inline] callee with private use splices under with (module)") @(t : T?) {
            let b = body_of(t, "target_scoped_add")
            t |> success(!has(b, "scoped_add("), "call is gone: {b}")
            t |> success(has(b, "hidden_boost("), "private call spliced in: {b}")
        }
        t |> run("multi-return callee rides the same wrapped splice") @(t : T?) {
            let b = body_of(t, "target_scoped_pick")
            t |> success(!has(b, "scoped_pick("), "call is gone: {b}")
        }
        t |> run("call inside user with (module) splices bare") @(t : T?) {
            let b = body_of(t, "target_inside_with")
            t |> success(!has(b, "scoped_add("), "call is gone: {b}")
        }
        t |> run("caller scope resolves an unseen dependency") @(t : T?) {
            let b = body_of(t, "target_visible_dependency")
            t |> success(!has(b, "must_inline_visibility_root("),
                "call is gone: {b}")
        }
        t |> run("generated scope resolves an unseen dependency") @(t : T?) {
            let b = body_of(t, "target_generated_dependency_scope")
            t |> success(!has(b, "must_inline_visibility_root("),
                "call is gone: {b}")
        }
        t |> run("unoptimized callee keeps its inner scope") @(t : T?) {
            let b = body_of(t, "target_unoptimized_scope")
            t |> success(!has(b, "scoped_unoptimized_value("),
                "call is gone: {b}")
        }
    }
}

[test(description = "scope-wrapped splices read and write the callee module's private globals, while argument expressions keep resolving at the caller even against a same-named global")]
def test_scope_wrapped_inline_behavior(t : T?) {
    t |> run("private helper and private global values") @(t : T?) {
        t |> equal(target_scoped_add(1, 2), (1 + 7) + (2 + 7))
        t |> equal(target_scoped_pick(5), 5 + 7)
        t |> equal(target_scoped_pick(-5), -(5 + 7))
        t |> equal(target_inside_with(3, 4), (3 + 7) + (4 + 7))
        t |> success(!empty(target_visible_dependency()))
        t |> success(!empty(target_generated_dependency_scope()))
        t |> equal(target_unoptimized_scope(), 43)
    }
    t |> run("mirror rule: caller global wins over callee module's same-named global") @(t : T?) {
        // caller g_shared = 555 substitutes outside the wrap; body's own g_shared
        // read resolves to the helper's (100). wrong binding gives 100100 or 555555
        t |> equal(target_mirror_rule(), 555 * 1000 + 100)
    }
    t |> run("block-literal auto tier with private use in the body") @(t : T?) {
        // even numbers boosted by secret_bias: (2+7) + (4+7)
        t |> equal(target_block_literal([1, 2, 3, 4]), 9 + 11)
    }
    t |> run("private global write stays visible to the callee module") @(t : T?) {
        with (module _inline_scope_helper) {
            secret_bias = 20
        }
        t |> equal(target_scoped_add(1, 2), (1 + 20) + (2 + 20))
        with (module _inline_scope_helper) {
            secret_bias = 7
        }
    }
}

// unsafe callee bodies splice under a generated `unsafe { }` wrapper
// (src/ast/ast_inline.cpp): a compiled callee lost its wrappers to foldUnsafe, so a
// wrapper-less spliced body re-errors against the caller on re-infer. FIRED proof: the
// cross-module calls (and the [inline] MUST call) are gone, invokes inside the unsafe
// region devirtualize; behavioral: values identical. The negative pins the kept gate:
// [unsafe_deref]/[unsafe_operation] change runtime semantics when spliced (re-infer
// re-stamps deref null-check elision from the CALLER's flag), so they stay calls.

def local_unsafe_sum(src : array<int>; blk : block<(v : int) : int>) : int {
    // same-module callee: its own `unsafe { }` wrapper is still present at patch time
    var total = 0
    var p = unsafe(addr(total))
    for (v in src) {
        *p += invoke(blk, v)
    }
    return total
}

[export]
def target_block_unsafe(src : array<int>) : int => src |> sum_via_ptr() $(v) => v * v

[export]
def target_single_return_unsafe(src : array<int>) : int => src |> first_via_ptr() $(v) => v + 10

[export]
def target_expr_unsafe(src : array<int>) : int => src |> expr_unsafe_sum() $(v) => v * 3

[export]
def target_local_unsafe(src : array<int>) : int => src |> local_unsafe_sum() $(v) => v - 1

[export]
def target_unsafe_deref(src : array<int>) : int => src |> deref_first() $(v) => v + 1

[export]
def target_must_unsafe(x : int) : int {
    return scaled_bits(x, 7)
}

[export]
def target_devirt_unsafe_literal(x : int) : int {
    // caller-origin literal with expression-form unsafe: the devirt splice keeps the
    // caller's own unsafe accounting (userSaidItsSafe survives non-callee splices)
    let blk <- $(v : int) : int {
        return unsafe(reinterpret<int>(uint(v))) + 1
    }
    return invoke(blk, x)
}

[test(description = "callees with block-form, expression-form or same-module unsafe bodies splice under a generated unsafe wrapper and devirtualize their invokes, while an [unsafe_deref] callee stays a call")]
def test_inline_unsafe_fired(t : T?) {
    ast_gc_guard() {
        t |> run("cross-module block-form unsafe body splices") @(t : T?) {
            let b = body_of(t, "target_block_unsafe")
            t |> success(!has(b, "sum_via_ptr("), "call is gone: {b}")
            t |> success(!has(b, "invoke("), "invoke devirtualized: {b}")
        }
        t |> run("invoke inside the unsafe region devirtualizes") @(t : T?) {
            let b = body_of(t, "target_single_return_unsafe")
            t |> success(!has(b, "first_via_ptr("), "call is gone: {b}")
            t |> success(!has(b, "invoke("), "invoke devirtualized: {b}")
        }
        t |> run("expression-form unsafe body splices") @(t : T?) {
            let b = body_of(t, "target_expr_unsafe")
            t |> success(!has(b, "expr_unsafe_sum("), "call is gone: {b}")
            t |> success(!has(b, "invoke("), "invoke devirtualized: {b}")
        }
        t |> run("same-module unsafe body splices") @(t : T?) {
            let b = body_of(t, "target_local_unsafe")
            t |> success(!has(b, "local_unsafe_sum("), "call is gone: {b}")
            t |> success(!has(b, "invoke("), "invoke devirtualized: {b}")
        }
        t |> run("[unsafe_deref] callee stays a call") @(t : T?) {
            let b = body_of(t, "target_unsafe_deref")
            t |> success(has(b, "deref_first("), "call stays regular: {b}")
        }
        t |> run("[inline] MUST callee with unsafe body splices") @(t : T?) {
            let b = body_of(t, "target_must_unsafe")
            t |> success(!has(b, "scaled_bits("), "call is gone: {b}")
        }
        t |> run("caller-origin unsafe literal devirtualizes") @(t : T?) {
            let b = body_of(t, "target_devirt_unsafe_literal")
            t |> success(!has(b, "invoke("), "invoke devirtualized: {b}")
        }
    }
}

[test(description = "functions whose inlined callees carry unsafe bodies compute the same values as the calls did")]
def test_inline_unsafe_behavior(t : T?) {
    t |> run("values preserved") @(t : T?) {
        let src <- [1, 2, 3, 4]
        t |> equal(target_block_unsafe(src), 30)
        t |> equal(target_single_return_unsafe(src), 11)
        t |> equal(target_expr_unsafe(src), 30)
        t |> equal(target_local_unsafe(src), 6)
        t |> equal(target_unsafe_deref(src), 2)
        t |> equal(target_must_unsafe(6), 42)
        t |> equal(target_devirt_unsafe_literal(41), 42)
    }
}
