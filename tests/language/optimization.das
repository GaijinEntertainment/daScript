options gen2
options _nolint = "PERF013" // the '-= 1' compound write is a shape under test, beside the postfix '--'

require dastest/testing_boost public
require daslib/defer

// behavioral coverage for the ConstFolding algebraic identities:
// every fold must be value-preserving, and purity gates must keep
// impure operands evaluated. folding QUALITY (that the rewrite
// actually fired) is asserted by hand via `options log`.

var g_i = 7
var g_u = 0xDEADu
var g_i64 = 9000000000l
var g_f = 3.5
var g_d = 2.25lf
var g_v = float3(1.0, 2.0, 4.0)
var g_b = true
var g_calls = 0

def side_i : int {
    g_calls++
    return g_i
}

def side_b : bool {
    g_calls++
    return g_b
}

def only_finally {
    defer() {
        g_calls++
    }
}

def void_return_impure {
    if (side_b()) { // nolint:LINT009 -- deliberate: equivalent void arms are the fold under test
        return
    } else {
        return
    }
}

def elif_grade(x : int) : int {
    if (x > 10) {
        return 3
    } elif (x > 5) {
        return 2
    } elif (x > 0) {
        return 1
    } else {
        return 0
    }
}

def defer_in_arm(c : bool) : int {
    if (c) {
        defer() {
            g_calls++
        }
        return 1
    } else {
        return 2
    }
}

[test(description = "folded multiplication identities (x*1, x*0, x*-1 and the like) keep the value and still evaluate an impure operand")]
def test_mul_identities(t : T?) {
    t |> run("x*1 and 1*x") @(t : T?) {
        t |> equal(g_i * 1, 7)
        t |> equal(1 * g_i, 7)
        t |> equal(g_f * 1.0, 3.5)
        t |> equal(1.0 * g_f, 3.5)
        t |> equal(g_d * 1.0lf, 2.25lf)
        t |> equal(g_v * 1.0, float3(1.0, 2.0, 4.0))
    }
    t |> run("x*-1") @(t : T?) {
        t |> equal(g_i * -1, -7)
        t |> equal(g_f * -1.0, -3.5)
    }
    t |> run("x*0 folds for int, keeps impure operands") @(t : T?) {
        t |> equal(g_i * 0, 0)
        t |> equal(0 * g_i, 0)
        g_calls = 0
        t |> equal(side_i() * 0, 0)
        t |> equal(g_calls, 1)    // impure operand must still run
    }
}

[test(description = "folded addition and subtraction identities (x+0, x-0, x-x and the like) keep the value")]
def test_add_sub_identities(t : T?) {
    t |> run("x+0 and x-0") @(t : T?) {
        t |> equal(g_i + 0, 7)
        t |> equal(0 + g_i, 7)
        t |> equal(g_i - 0, 7)
        t |> equal(g_f + 0.0, 3.5)
        t |> equal(g_f - 0.0, 3.5)
        t |> equal(g_d + 0.0lf, 2.25lf)
        t |> equal(g_v + float3(0.0), float3(1.0, 2.0, 4.0))
    }
    t |> run("0-x negates") @(t : T?) {
        t |> equal(0 - g_i, -7)
        t |> equal(0.0 - g_f, -3.5)
    }
    t |> run("x-x is zero for ints") @(t : T?) {
        t |> equal(g_i - g_i, 0)
        t |> equal(g_i64 - g_i64, 0l)
    }
}

[test(description = "folded division and modulo identities keep the value")]
def test_div_mod_identities(t : T?) {
    t |> equal(g_i / 1, 7)
    t |> equal(g_f / 1.0, 3.5)
    t |> equal(g_v / 1.0, float3(1.0, 2.0, 4.0))
    t |> equal(g_i % 1, 0)
    g_calls = 0
    t |> equal(side_i() % 1, 0)
    t |> equal(g_calls, 1)
}

[test(description = "folded bitwise identities (x^x, x&x, x|x and the like) keep the value")]
def test_bitwise_identities(t : T?) {
    t |> equal(g_u & 0u, 0u)
    t |> equal(g_u | 0u, 0xDEADu)
    t |> equal(g_u ^ 0u, 0xDEADu)
    t |> equal(g_u ^ g_u, 0u) // nolint:LINT007 -- deliberate: exercises the x^x fold
    t |> equal(g_u & g_u, 0xDEADu) // nolint:LINT007 -- deliberate: exercises the x&x fold
    t |> equal(g_u | g_u, 0xDEADu) // nolint:LINT007 -- deliberate: exercises the x|x fold
    t |> equal(g_u << 0u, 0xDEADu)
    t |> equal(g_u >> 0u, 0xDEADu)
    t |> equal(g_i << 0, 7)
}

[test(description = "folded boolean identities keep the value for both values of the variable operand")]
def test_bool_identities(t : T?) {
    t |> run("const arms fold") @(t : T?) {
        // both values of g_b — with only g_b==true, a fold that wrongly
        // rewrote `b && true` to constant `true` would pass undetected
        t |> equal(g_b && true, true)
        t |> equal(g_b && false, false)
        t |> equal(g_b || false, true)
        t |> equal(g_b || true, true)
        t |> equal(true && g_b, true)
        t |> equal(false || g_b, true)
        g_b = false
        t |> equal(g_b && true, false)
        t |> equal(g_b && false, false)
        t |> equal(g_b || false, false)
        t |> equal(g_b || true, true)
        t |> equal(true && g_b, false)
        t |> equal(false || g_b, false)
        g_b = true
    }
    t |> run("short-circuit never evaluates right of const-false") @(t : T?) {
        g_calls = 0
        t |> equal(false && side_b(), false)
        t |> equal(true || side_b(), true)
        t |> equal(g_calls, 0)
    }
    t |> run("purity gate keeps impure left evaluated") @(t : T?) {
        g_calls = 0
        t |> equal(side_b() && false, false)
        t |> equal(g_calls, 1)
        t |> equal(side_b() || true, true)
        t |> equal(g_calls, 2)
    }
    t |> run("double negation and compare flip") @(t : T?) {
        t |> equal(!!g_b, true)
        t |> equal(!(g_i < 5), g_i >= 5)
        t |> equal(!(g_i == 7), false)
        t |> equal(-(-g_i), 7)
    }
}

[test(description = "a folded ternary whose arms are the same expression keeps the value")]
def test_ternary_identities(t : T?) {
    t |> equal(g_i > 3 ? true : false, true)
    t |> equal(g_i > 3 ? false : true, false)
    t |> equal(g_i < 3 ? g_f : g_f, 3.5) // nolint:LINT008 -- deliberate: exercises the c?a:a fold
}

[test(description = "a folded cast to the operand's own type leaves the value unchanged for every workhorse type")]
def test_same_type_casts(t : T?) {
    t |> equal(int(g_i), 7) // nolint:PERF020 -- deliberate: this whole test exercises the same-type cast fold
    t |> equal(uint(g_u), 0xDEADu) // nolint:PERF020
    t |> equal(int64(g_i64), 9000000000l) // nolint:PERF020
    t |> equal(float(g_f), 3.5) // nolint:PERF020
    t |> equal(double(g_d), 2.25lf) // nolint:PERF020
    t |> equal(float3(g_v), float3(1.0, 2.0, 4.0))
}

[test(description = "collapsing splat-vector arithmetic keeps the vector result, and splat-with-scalar shapes are left for the vector operator")]
def test_splat_collapse(t : T?) {
    if (HOST_FAST_MATH) {
        t |> skip("host runtime is built -ffast-math: the splat collapse folds through a reciprocal, which rounds")
        return
    }
    t |> equal(g_v * float3(g_f), float3(1.0 * g_f, 2.0 * g_f, 4.0 * g_f))
    t |> equal(g_v / float3(g_f), float3(1.0 / g_f, 2.0 / g_f, 4.0 / g_f))
    t |> equal(float3(g_f) * g_v, float3(g_f * 1.0, g_f * 2.0, g_f * 4.0))
    // splat against a SCALAR is not a collapse candidate: the other side
    // must be the vector, or the fold finds the scalar operator and
    // leaves a float under a floatN-typed node
    t |> equal(float2(g_f) * g_f, float2(g_f * g_f, g_f * g_f))          // splat * scalar
    t |> equal(g_f * float2(g_f), float2(g_f * g_f, g_f * g_f))          // scalar * splat
    t |> equal(g_f / float3(g_f), float3(1.0, 1.0, 1.0))                 // scalar / splat
    t |> equal(float3(g_f) * 2.0, float3(g_f * 2.0, g_f * 2.0, g_f * 2.0))
    // splat / scalar: never matched, keep it so
    t |> equal(float4(g_f) / g_f, float4(1.0, 1.0, 1.0, 1.0))
    // splat * splat: the cascade
    t |> equal(float2(g_f) * float2(g_f), float2(g_f * g_f, g_f * g_f))
}

[test(description = "loops the folder removes as dead leave the surrounding values exactly as running them would")]
def test_dead_loops(t : T?) {
    g_calls = 0
    while (false) {
        g_calls++
    }
    for (_i in range(0, 0)) {
        g_calls++
    }
    for (_i in range(3, 3)) {
        g_calls++
    }
    for (_i in urange(5u, 5u)) {
        g_calls++
    }
    t |> equal(g_calls, 0)
}

[test(description = "a call to a function whose body is only a finally is kept, so the finally's side effect happens")]
def test_finalizer_only_function_still_runs(t : T?) {
    // regression: isNop used to treat a finalizer-only body as a nop and
    // delete the call, silently dropping the defer's side effect under
    // optimization
    g_calls = 0
    only_finally()
    t |> equal(g_calls, 1)
}

[test(description = "folding an if whose two arms are equivalent still evaluates its impure condition")]
def test_cond_fold_keeps_impure_cond(t : T?) {
    // regression: `if (impure()) return; else return;` folded to a bare
    // return, silently dropping the condition's side effect
    g_calls = 0
    void_return_impure()
    t |> equal(g_calls, 1)
}

[test(description = "an if/elif return chain folded to nested ternaries returns each branch's value")]
def test_elif_chain_values(t : T?) {
    // the elif chain folds to nested ternaries (shape proof in
    // optimization_fold_shape.das); every branch must keep its value
    t |> equal(elif_grade(11), 3)
    t |> equal(elif_grade(7), 2)
    t |> equal(elif_grade(1), 1)
    t |> equal(elif_grade(-5), 0)
}

[test(description = "an if arm holding a return plus a defer is not folded to a ternary return, so the defer still runs")]
def test_cond_fold_keeps_arm_finally(t : T?) {
    // regression: an arm holding one return plus a finally section
    // (defer) used to fold to a ternary return, silently dropping the
    // finally
    g_calls = 0
    t |> equal(defer_in_arm(true), 1)
    t |> equal(g_calls, 1)
    t |> equal(defer_in_arm(false), 2)
    t |> equal(g_calls, 1)
}

[test(description = "without fast_math the folder does not regroup float arithmetic chains and their values stay exact")]
def test_reassociation_off_by_default(t : T?) {
    // without fast_math the chain is NOT regrouped; values are exact here
    // either way
    t |> equal((g_f + 1.0) + 2.0, 6.5)
    t |> equal((g_f * 2.0) * 4.0, 28.0)
    t |> equal(2.0 * (g_f * 4.0), 28.0)
}

var step : int = 4

def through(n : int) : int => n <= 0 ? 1 : looks_pure(n - 1) + step

def looks_pure(n : int) : int => n <= 0 ? 2 : through(n - 1) * 2

var depth_unit : int = 3

def ring_a(n : int) : int => n <= 0 ? 0 : ring_b(n - 1) + 1

def ring_b(n : int) : int => n <= 0 ? 0 : ring_c(n - 1) + 1

def ring_c(n : int) : int => n <= 0 ? depth_unit : ring_a(n - 1) + 1

[test(description = "a constant-argument call into two mutually recursive functions that read a global is not folded to a compile-time value")]
def test_a_constant_call_into_mutual_recursion_reading_a_global(t : T?) {
    t |> equal(looks_pure(2), 12)
    t |> equal(through(1), 6)
}

[test(description = "a constant-argument call into a three-function cycle that reads a global is not folded to a compile-time value")]
def test_a_constant_call_into_a_three_function_cycle_reading_a_global(t : T?) {
    t |> equal(ring_a(5), 8)
    t |> equal(ring_b(4), 7)
    t |> equal(ring_c(3), 6)
}

def private offset_or_null(p : int?; n : int64) : int const? {
    unsafe {
        return p != null ? p + n : null
    }
}

[test(description = "a ternary guarding pointer arithmetic on a null pointer folds to null instead of an offset pointer")]
def test_folded_null_pointer_arith(t : T?) {
    var p : int? = null
    unsafe {
        let q = p != null ? p + 3l : null
        t |> equal(q == null, true)
    }
    t |> equal(offset_or_null(null, 5l) == null, true)
}

struct Pair {
    a : int
    b : int
}

def takes(_a : int?) {
    pass
}

def takes_int(_a : int) {
    pass
}

def bump(var i : int&) : int {
    i++
    return i - 1
}

def make_pair(var i : int&) : Pair {
    i++
    return Pair(a = 1, b = 2)
}

struct Holder {
    p : Pair?
    arr : array<int>?
    v : float4
}

variant IV {
    i : int
    f : float
}

def takes_float(_a : float) {
    pass
}

def takes_bool(_b : bool) {
    pass
}

def make_holder(var i : int&) : Holder {
    i++
    return Holder(v = float4(1.0))
}

def make_variant(var i : int&) : IV {
    i++
    return IV(i = 3)
}

def make_array(var i : int&) : array<int> {
    i++
    return <- [1, 2]
}

[test(description = "a call result read through a swizzle, safe field, is, as, ?as, index or safe index still runs the call when the consumer does nothing")]
def test_every_read_of_a_call_result_keeps_the_side_effects_of_the_call(t : T?) {
    var a = 0
    takes_float(make_holder(a).v.xy.x)
    t |> equal(a, 1, "a swizzle of a call result's field")
    var b = 0
    takes(make_holder(b).p?.a)
    t |> equal(b, 1, "a safe field through a call result")
    var c = 0
    takes_bool(make_variant(c) is i)
    t |> equal(c, 1, "an is test of a call result")
    var d = 0
    takes_int(make_variant(d) as i)
    t |> equal(d, 1, "an as read of a call result")
    var e = 0
    takes(unsafe(make_variant(e) ?as i))
    t |> equal(e, 1, "a safe as read of a call result")
    var f = 0
    takes_int(make_array(f)[0])
    t |> equal(f, 1, "an index into a call result")
    var g = 0
    takes(unsafe(make_holder(g).arr?[0]))
    t |> equal(g, 1, "a safe index into a call result")
}

[test(description = "an addr() argument whose index has a side effect keeps it once when the empty call is dropped, inline or through a let")]
def test_an_addr_argument_keeps_its_side_effects_when_the_call_is_dropped(t : T?) {
    var arr : int[3]
    var c = 0
    takes(unsafe(addr(arr[bump(c)])))
    t |> equal(c, 1, "a call inside the index of an addr() argument")
    var d = 0
    let q = unsafe(addr(arr[d++]))
    takes(q)
    t |> equal(d, 1, "a let bound to addr() and then passed")
    var e = 0
    let r = unsafe(addr(arr[e++]))
    takes(r)
    takes(r)
    t |> equal(e, 1, "a let bound to addr() and passed twice")
}

[test(description = "a field or vector-index argument keeps the side effect of the call that computes its base or index")]
def test_a_field_or_index_argument_keeps_the_side_effects_of_its_base(t : T?) {
    var k = 0
    takes_int(make_pair(k).a)
    t |> equal(k, 1, "a field of a call result")
    var s = 0
    let v = float3(1.0)
    takes_int(int(v[bump(s)]))
    t |> equal(s, 1, "a vector index computed by a call")
}

struct S {
    a : int
    b : int[4]
}

def dec_offset(var s : S?; k : int) {
    var q : int? = unsafe(reinterpret<int?>(unsafe(reinterpret<uint64>(s)) + 4ul))
    unsafe(q[k]) -= 1
}

def dec_offset_postfix(var s : S?; k : int) {
    var q : int? = unsafe(reinterpret<int?>(unsafe(reinterpret<uint64>(s)) + 4ul))
    unsafe(q[k])--
}

def dec_field(var s : S?; k : int) {
    s.b[k] -= 1
}

def dec_branch(var s : S?; k : int) {
    if (k < 2) {
        var q : int? = unsafe(reinterpret<int?>(unsafe(reinterpret<uint64>(s)) + 4ul))
        unsafe(q[k]) -= 1
    } else {
        var r : int? = unsafe(reinterpret<int?>(unsafe(reinterpret<uint64>(s)) + 12ul))
        unsafe(r[k - 2]) -= 1
    }
}

[unsafe_deref]
def store_byte(dst : uint64; value : uint8) : uint64 {
    unsafe(unsafe(reinterpret<uint8?>(dst))[0]) = value
    return dst
}

[unsafe_deref]
def store_via(p : uint8?; value : uint8) {
    store_byte(unsafe(reinterpret<uint64>(p)), value)
}

[test(description = "a store through a pointer computed from an address counts as a side effect, so the function doing it is not optimized away")]
def test_a_store_through_a_pointer_computed_from_an_address_is_a_side_effect(t : T?) {
    var w = S()
    var x = S()
    var y = S()
    var z = S()
    for (k in [0, 1, 1, 3]) {
        dec_offset(unsafe(addr(w)), k)
        dec_offset_postfix(unsafe(addr(x)), k)
        dec_field(unsafe(addr(y)), k)
        dec_branch(unsafe(addr(z)), k)
    }
    t |> equal([w.b[0], w.b[1], w.b[3]], [-1, -2, -1], "pointer held in a local, -= 1")
    t |> equal([x.b[0], x.b[1], x.b[3]], [-1, -2, -1], "pointer held in a local, postfix --")
    t |> equal([y.b[0], y.b[1], y.b[3]], [-1, -2, -1], "field access")
    t |> equal([z.b[0], z.b[1], z.b[3]], [-1, -2, -1], "pointer held in a local inside a branch")
}

[test(description = "a call statement whose only effect is a store through a reinterpreted pointer is kept")]
def test_a_call_whose_only_effect_is_a_store_through_a_reinterpreted_pointer_is_kept(t : T?) {
    var a : uint8[16]
    store_byte(unsafe(reinterpret<uint64>(unsafe(addr(a[0])))), uint8(7))
    t |> equal(int(a[0]), 7, "store_byte called as a statement")
    store_via(unsafe(addr(a[1])), uint8(9))
    t |> equal(int(a[1]), 9, "store_via called as a statement under [unsafe_deref]")
}

struct Rt {
    pc : int = 0
    code : array<int>
    stack : array<int>
}

[inline]
def next_code(var rt : Rt) : int {
    let v = rt.code[rt.pc]
    rt.pc++
    return v
}

[inline]
def at(rt : Rt; i : int) : int => rt.stack[i]

[inline]
def read(var rt : Rt) : int {
    let i = next_code(rt)
    return at(rt, i)
}

[inline]
def read_one_expression(var rt : Rt) : int {
    return at(rt, next_code(rt))
}

def op(var rt : Rt) : int {
    return read(rt)
}

def op_one_expression(var rt : Rt) : int {
    return read_one_expression(rt)
}

def make_rt() : Rt => <- Rt(code <- [1], stack <- [7, 9])

[test(description = "a nested [inline] call that mutates the caller's var struct parameter sees the mutation before the next read")]
def test_a_nested_inline_call_with_the_callers_var_struct_parameter(t : T?) {
    var rt <- make_rt()
    t |> equal(op(rt), 9)
    var rt2 <- make_rt()
    t |> equal(op_one_expression(rt2), 9)
}

[inline]
def counted_positive(value : int; var calls : int&) : bool {
    calls++
    return value > 0
}

def conditional_stream(enabled : bool; mode : int) : iterator<int> {
    return <- generator<int> {
        var calls = 0
        for (value in range(3)) {
            if (mode == 0) {
                if (enabled && counted_positive(value, calls)) { yield value }
            } elif (mode == 1) {
                if (enabled || counted_positive(value, calls)) { yield value }
            } else {
                if (enabled ? counted_positive(value, calls) : false) { yield value }
            }
        }
        yield -calls
        return false
    }
}

def stream_matches(enabled : bool; mode : int; expected : array<int>) : bool {
    var stream <- conditional_stream(enabled, mode)
    var values : array<int> // nolint:STYLE027 - explicit consumption keeps the generator regression and call-count observation separate
    for (value in stream) { values |> push(value) }
    var equal = length(values) == length(expected)
    if (equal) { for (a, b in values, expected) { equal &&= a == b } }
    delete values
    return equal
}

def generator_conditions_work() : bool {
    return (stream_matches(true, 0, [1, 2, -3])
        && stream_matches(false, 0, [0])
        && stream_matches(true, 1, [0, 1, 2, 0])
        && stream_matches(false, 1, [1, 2, -3])
        && stream_matches(true, 2, [1, 2, -3])
        && stream_matches(false, 2, [0]))
}

[test(description = "an [inline] call inside a generator's &&, || or ternary condition keeps short-circuit evaluation")]
def inline_generator_preserves_conditional_evaluation(t : T?) {
    t |> success(generator_conditions_work())
}

// a global read in a call's evaluation prefix keeps left-to-right
// order when the call splices: the load is snapshotted above the
// spliced body, element and scalar reads alike
var g_array : array<int>
var g_scalar = 10

[inline]
def clobber_and_one() : int {
    g_array[0] = 99
    g_scalar = 99
    // multi-statement body: the splice must land as statements, which is
    // what forces the prefix hoist
    var n = 0
    n++
    return n
}

[test(description = "a global element or scalar read left of a spliced [inline] call that overwrites it reads the value from before the call")]
def test_prefix_global_order(t : T?) {
    t |> run("global element read keeps left-to-right order") @(t : T?) {
        g_array |> clear()
        g_array |> push(10)
        g_scalar = 10
        let x = g_array[0] + clobber_and_one()
        t |> equal(x, 11)
    }
    t |> run("global scalar read keeps left-to-right order") @(t : T?) {
        g_array[0] = 10
        g_scalar = 10
        let y = g_scalar + clobber_and_one()
        t |> equal(y, 11)
    }
}
