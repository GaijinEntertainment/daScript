options gen2
options _nolint = "LINT031"   // the write through a const-stripped parameter is the shape under test

require dastest/testing_boost
require UnitTest
require daslib/lpipe
require _const_arg_readonly_writer

// bool is a 4-byte slot in memory but i1 in a signature, so a bool
// that reaches a call argument or a return through a variable (a
// load, not a compare) needs narrowing to the parameter type.

struct Box {
    x : int
    y : int
}

def take_bool(b : bool) : int => b ? 1 : 0

[sideeffects]
def take_two_bools(a, b : bool) : bool => a && b

[sideeffects]
def pass_loaded_bool(v : bool) : int {
    var stored = v  // nolint:LINT003 - const propagates, bypassing the store/load this exercises
    return take_bool(stored)
}

[sideeffects]
def return_loaded_bool(v : bool) : bool {
    var stored = v  // nolint:LINT003 - const propagates, bypassing the store/load this exercises
    return stored
}

[sideeffects]
def return_compared_bool(a, b : int) : bool => a < b

def take_box(b : Box) : int => b.x + b.y

// a temporary passed where the parameter is taken by reference
[sideeffects]
def pass_temp_struct(x, y : int) : int => take_box(Box(x = x, y = y))

[test(description = "bool and temporary-struct arguments and return values cross JIT call boundaries with the right register or stack representation")]
def test_call_abi(t : T?) {
    // without this the cases can silently run interpreted, and would pass
    // either way
    t |> success(!jit_enabled() || is_jit_function(@@pass_loaded_bool))
    t |> success(!jit_enabled() || is_jit_function(@@return_loaded_bool))
    t |> success(!jit_enabled() || is_jit_function(@@return_compared_bool))
    t |> success(!jit_enabled() || is_jit_function(@@take_two_bools))
    t |> success(!jit_enabled() || is_jit_function(@@pass_temp_struct))
    t |> equal(1, pass_loaded_bool(true))
    t |> equal(0, pass_loaded_bool(false))
    t |> success(return_loaded_bool(true))
    t |> success(!return_loaded_bool(false))
    t |> success(return_compared_bool(1, 2))
    t |> success(!return_compared_bool(2, 1))
    t |> success(take_two_bools(true, true))
    t |> success(!take_two_bools(true, false))
    t |> equal(7, pass_temp_struct(3, 4))
}

def cmres_via_copy(var a : int[10]) {
    for (i in range(10)) {
        a[i] = 13
    }
    return a
}

def cmres_alias {
    var a : int[10]
    for (i in range(10)) {
        a[i] = i
    }
    return a
}

def cmres_var_init_via_call {
    let a = cmres_alias()
    return a
}

def cmres_copy_local(which : bool) {
    var a : int[10]
    let b : int[10]
    a = cmres_alias()
    return which ? a : b
}

def cmres_alias_no_jit {
    var a : int[10]
    for (i in range(10)) {
        a[i] = i
    }
    return a
}

def cmres_var_init_via_call_with_cmres {
    let a = cmres_alias_no_jit()    // calling non-jit function
    return a
}

def cmres_call_native {
    let a = testCMRES()
    return a
}

def cmres_return_call_cmres => testCMRES()

def cmres_passthrough(var a : int[10]) => cmres_via_copy(a)

struct ReturnFromFinally {
    values : table<string; int>
}

[export]
def ReturnFromFinally(str : string) {
    var res : ReturnFromFinally
    if (empty(str)) {
        return <- res
    }
    var inscope bytes : array<uint8>
    return <- res
}

[test(description = "JIT-compiled functions returning by copy-or-move-result produce the right value through copies, aliases, native calls and nested cmres calls")]
def test_cmres(t : T?) {
    t |> success(!jit_enabled() || is_jit_function(@@ReturnFromFinally))
    t |> run("cmres via copy") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_via_copy))
        var x : int[10]
        var b = cmres_via_copy(x)
        for (xx in x) {
            t |> equal(13, xx)
        }
        for (bb in b) {
            t  |> equal(13, bb)
        }
    }
    t |> run("cmres passthrough via copy") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_passthrough))
        var x : int[10]
        var b = cmres_passthrough(x)
        for (xx in x) {
            t |> equal(13, xx)
        }
        for (bb in b) {
            t  |> equal(13, bb)
        }
    }
    t |> run("cmres alias") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_alias))
        var x = cmres_alias()
        for (xx, i in x, count()) {
            t |> equal(i, xx)
        }
    }
    t |> run("cmres var init via call") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_var_init_via_call))
        var x = cmres_var_init_via_call()
        for (xx, i in x, count()) {
            t |> equal(i, xx)
        }
    }
    t |> run("cmres copy local") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_copy_local))
        var x = cmres_copy_local(true)
        for (xx, i in x, count()) {
            t |> equal(i, xx)
        }
    }
    t |> run("cmres var init via call with cmres") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_var_init_via_call_with_cmres))
        var x = cmres_var_init_via_call_with_cmres()
        for (xx, i in x, count()) {
            t |> equal(i, xx)
        }
    }
    t |> run("cmres call native") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_call_native))
        var b = cmres_call_native()
        for (bb in b) {
            t  |> equal(13, bb)
        }
    }
    t |> run("cmres return call cmres") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@cmres_return_call_cmres))
        var b = cmres_return_call_cmres()
        for (bb in b) {
            t  |> equal(13, bb)
        }
    }
}

var g_invoke_A = 0
var g_invoke_B = 0.

def call_function(a : int; b : float) {
    g_invoke_A = a
    g_invoke_B = b
    return 13
}

def test_invoke_call(fun : function<(a : int; b : float) : int>) => invoke(fun, 1, 2.0)

struct InvokeFoo {
    a : int
    b : float
}

def call2cmres_function(a : int; b : float) {
    g_invoke_A = a
    g_invoke_B = b
    return InvokeFoo(a = a + 10, b = b + 10.)
}

def test_invoke_call2cmres(fun : function<(a : int; b : float) : InvokeFoo>) => invoke(fun, 1, 2.0)

def test_invoke_lambda(lmb : lambda<(a : int; b : float) : int>) => invoke(lmb, 1, 2.0)

def test_invoke_and_delete_lambda(var lmb : lambda<(a : int; b : float) : int>) {
    let res = invoke(lmb, 1, 2.0)
    unsafe { delete lmb; }
    return res
}

def test_invoke_lambda2cmres(lmb : lambda<(a : int; b : float) : InvokeFoo>) => invoke(lmb, 1, 2.0)

def test_invoke_block_arg(blk : block<(a : int; b : float) : int>) => invoke(blk, 1, 2.0)

def test_invoke_block2cmres(blk : block<(a : int; b : float) : InvokeFoo>) => invoke(blk, 1, 2.0)

[test(description = "invoke of function pointers, lambdas and blocks, including cmres results and null, returns the interpreter's results under JIT")]
def test_invoke(t : T?) {    // nolint:STYLE038 - one arm per invoke shape
    t |> run("invoke call") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_invoke_call))
        g_invoke_A = 0
        g_invoke_B = 0.
        t |> equal(13, test_invoke_call(@@call_function))
        t |> equal(1, g_invoke_A)
        t |> equal(2.0, g_invoke_B)
    }
    t |> run("invoke call2cmres") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_invoke_call2cmres))
        g_invoke_A = 0
        g_invoke_B = 0.
        let res = test_invoke_call2cmres(@@call2cmres_function)
        t |> equal(11, res.a)
        t |> equal(12., res.b)
        t |> equal(1, g_invoke_A)
        t |> equal(2.0, g_invoke_B)
    }
    t |> run("invoke lambda") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_invoke_lambda))
        g_invoke_A = 0
        g_invoke_B = 0.
        let res = test_invoke_lambda() @(a : int; b : float) : int {
            g_invoke_A = a
            g_invoke_B = b
            return 13
        }
        t |> equal(13, res)
        t |> equal(1, g_invoke_A)
        t |> equal(2.0, g_invoke_B)
    }
    t |> run("invoke and delete lambda") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_invoke_and_delete_lambda))
        g_invoke_A = 0
        g_invoke_B = 0.
        let res = test_invoke_and_delete_lambda() @(a : int; b : float) : int {
            g_invoke_A = a
            return 13
        } finally {
            g_invoke_B = 2.0
        }
        t |> equal(13, res)
        t |> equal(1, g_invoke_A)
        t |> equal(2.0, g_invoke_B)
    }
        // now empty lambda

    t |> run("invoke lambda2cmres") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_invoke_lambda2cmres))
        g_invoke_A = 0
        g_invoke_B = 0.
        let res = test_invoke_lambda2cmres() @(a : int; b : float) : InvokeFoo {
            g_invoke_A = a
            g_invoke_B = b
            return InvokeFoo(a = a + 10, b = b + 10.)
        }
        t |> equal(11, res.a)
        t |> equal(12., res.b)
        t |> equal(1, g_invoke_A)
        t |> equal(2.0, g_invoke_B)
    }
    t |> run("invoke block") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_invoke_block_arg))
        g_invoke_A = 0
        g_invoke_B = 0.
        let res = test_invoke_block_arg() $(a : int; b : float) : int {
            g_invoke_A = a
            g_invoke_B = b
            return 13
        }
        t |> equal(13, res)
        t |> equal(1, g_invoke_A)
        t |> equal(2.0, g_invoke_B)
    }
    t |> run("invoke block2cmres") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_invoke_block2cmres))
        g_invoke_A = 0
        g_invoke_B = 0.
        let res = test_invoke_block2cmres() $(a : int; b : float) : InvokeFoo {
            g_invoke_A = a
            g_invoke_B = b
            return InvokeFoo(a = a + 10, b = b + 10.)
        }
        t |> equal(11, res.a)
        t |> equal(12., res.b)
        t |> equal(1, g_invoke_A)
        t |> equal(2.0, g_invoke_B)
    }
    t |> run("invoke null") @(t : T?) {
        try {
            test_invoke_call(default<function<(a : int; b : float) : int>>);
            panic("unreachable")
        } recover {}
        try {
            test_invoke_lambda(default<lambda<(a : int; b : float) : int>>);
            panic("unreachable")
        } recover {}
    }
}

struct Big {
    a : int
    b : int
    c : int
    d : int
}

def make_big(k : int) : Big => Big(a = k, b = k * 2, c = k * 3, d = k * 4)

def via_fn_ptr(fun : function<(k : int) : Big>; k : int) : Big => invoke(fun, k)

def via_lambda(fun : lambda<(k : int) : Big>; k : int) : Big => invoke(fun, k)

class Maker {
    scale : int
    def make(k : int) : Big =>
        Big(a = k * scale, b = k * scale * 2, c = k * scale * 3, d = k * scale * 4)
}

def expect_big(t : T?; got : Big; a, b, c, d : int) {
    t |> equal(a, got.a)
    t |> equal(b, got.b)
    t |> equal(c, got.c)
    t |> equal(d, got.d)
}

[test(description = "invoke with a cmres result through a function pointer, lambda and class method does not alias the result between two invokes")]
def test_cmres_invoke(t : T?) {
    t |> run("function pointer") @(t : T?) {
        expect_big(t, via_fn_ptr(@@make_big, 7), 7, 14, 21, 28)
        expect_big(t, make_big(7), 7, 14, 21, 28)
    }
    t |> run("lambda") @(t : T?) {
        var fn <- @(k : int) : Big => Big(a = k, b = k * 2, c = k * 3, d = k * 4)
        expect_big(t, via_lambda(fn, 5), 5, 10, 15, 20)
        unsafe {
            delete fn
        }
    }
    t |> run("class method") @(t : T?) {
        var m = new Maker(scale = 3)
        expect_big(t, m->make(2), 6, 12, 18, 24)
        unsafe {
            delete m
        }
    }
    t |> run("result is not aliased across two invokes") @(t : T?) {
        let first = via_fn_ptr(@@make_big, 1)
        let second = via_fn_ptr(@@make_big, 9)
        expect_big(t, first, 1, 2, 3, 4)
        expect_big(t, second, 9, 18, 27, 36)
    }
}

var ctor_count = 0
var to_upper_count = 0
var utf8_count = 0

class MyString {
    def MyString {
        ctor_count ++
    }
    def to_upper() : MyString? {
        to_upper_count ++
        return addr(self)
    }
    def utf8() : MyString? {
        utf8_count ++
        return addr(self)
    }
}

[test(description = "a chained method call on a new object runs the constructor and each method exactly once")]
def test_chain_invoke_methods(t : T?) {
    t |> success(!jit_enabled() || is_jit_function(@@MyString))
    let res = new MyString()->to_upper()->utf8()
    t |> equal(1, ctor_count)
    t |> equal(1, to_upper_count)
    t |> equal(1, utf8_count)
}

def length_jit(a : string) => length(a)

[test(description = "a JIT-compiled call to an external C++ function returns the interpreter's result")]
def test_extern_call(t : T?) {
    t |> run("extern call") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@length_jit))
        t |> equal(length_jit("hello"), 5)
    }
}

def print_me(foo : int) => sprint(foo, print_flags.singleLine)

[test(description = "a JIT-compiled call to an interop function returns the interpreter's result")]
def test_interop_call(t : T?) {
    t |> run("interop call") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@print_me))
        t |> equal(print_me(13), "int const = 13")
    }
}

var g_A = 0
var g_B = 0.

struct Foo {
    a : int
    b : float
}

def test_invoke_block_jit(lmb : block<(a : int; b : float) : int>) => invoke(lmb, 1, 2.0)

def test_invoke_block(lmb : block<(a : int; b : float) : int>) => invoke(lmb, 1, 2.0)

def test_invoke_block_refval(lmb : block<(a : int; b : float) : Foo>) => invoke(lmb, 1, 2.0)

def test_invoke_block_void(lmb : block<(a : int; b : float) : void>) {
    invoke(lmb, 1, 2.0)
}

def test_make_block {
    var local_C = 123
    let res = test_invoke_block() $(a : int; b : float) : int {
        g_A = a
        g_B = b
        local_C = 456
        return 13
    }
    return (res, local_C)
}

def test_make_block_jit_invoke {
    var local_C = 123
    let res = test_invoke_block_jit() $(a : int; b : float) : int {
        g_A = a
        g_B = b
        local_C = 456
        return 13
    }
    return (res, local_C)
}

def test_make_block_return_refval {
    let res = test_invoke_block_refval() $(a : int; b : float) : Foo {
        g_A = a
        g_B = b
        return Foo(a = a, b = b)
    }
    return res
}

def test_make_block_capture_refval {
    var f : Foo
    test_invoke_block_void() $(a : int; b : float) : void {
        f.a = a
        f.b = b
    }
    return f
}

def test_make_block_nested(q : float) {
    var f : Foo
    test_invoke_block_void() $(a : int; b : float) : void {
        f.a = a
        f.b = b
        test_invoke_block_void() $(c : int; _d : float) : void {
            f.a = c + 10
            f.b = b + q
        }
    }
    return f
}

var g_a_i : array<int>

def capture_args(var a : array<int>) {
    invoke() $(var b : array<int>) {
        invoke() {
            g_a_i <- b // nolint:PERF030 - the tested capture shape; the global starts empty
        }
    }
    lpipe(a)
}

var g_i : int

def capture_args_ref(var a : int&) {
    invoke() $(var b : int&) {
        invoke() {
            g_i <- b
        }
    }
    lpipe(a)
}

[test(description = "JIT-compiled block creation, invocation, ref-value returns and by-ref captures, nested to depth, return the interpreter's results")]
def test_block(t : T?) {
    t |> run("make block") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@test_make_block))
        g_A = 0
        g_B = 0.
        let res = test_make_block()
        t |> equal(13, res._0)
        t |> equal(456, res._1)
        t |> equal(1, g_A)
        t |> equal(2., g_B)
    }
    t |> run("make block, invoke with jit") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@test_make_block_jit_invoke))
        g_A = 0
        g_B = 0.
        let res = test_make_block_jit_invoke()
        t |> equal(13, res._0)
        t |> equal(456, res._1)
        t |> equal(1, g_A)
        t |> equal(2., g_B)
    }
    t |> run("make block return refval") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@test_make_block_return_refval))
        g_A = 0
        g_B = 0.
        let res = test_make_block_return_refval()
        t |> equal(1, res.a)
        t |> equal(2., res.b)
        t |> equal(1, g_A)
        t |> equal(2., g_B)
    }
    t |> run("make block capture refval") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@test_make_block_capture_refval))
        let res = test_make_block_capture_refval()
        t |> equal(1, res.a)
        t |> equal(2., res.b)
    }
    t |> run("make block capture refval nested") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@test_make_block_nested))
        let res = test_make_block_nested(10.)
        t |> equal(11, res.a)
        t |> equal(12., res.b)
    }
    t |> run("make block capture refval nested") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@capture_args))
        var a <- [ 1, 2, 3, 4]
        capture_args(a)
        t |> equal(4, g_a_i |> length)
        t |> equal(0, a |> length)
    }
    t |> run("make block capture by ref nested") @(t : T?) {
        verify(!jit_enabled() || is_jit_function(@@capture_args_ref))
        var a = 13
        g_i = 0
        capture_args_ref(a)
        t |> equal(13, g_i)
        t |> equal(a, 0)
    }
}

def noop_handler(_msg : string) {
    pass
}

def do_something(handler : function<(msg : string) : void> = @@noop_handler) {
    invoke(handler, "hello")
}

[test(description = "invoke through a function-pointer argument works both with the default pointer and with an explicit one")]
def test_default_fn_ptr(t : T?) {
    t |> run("invoke with default function pointer") @(t : T?) {
        t |> success(true)
        do_something()
    }
    t |> run("invoke with explicit function pointer") @(t : T?) {
        t |> success(true)
        do_something(@@noop_handler)
    }
}

// doubleSamplePoint3 is a C++-bound extern taking `const Point3 &` —
// an implicit const& parameter. A named-variable argument works (a
// variable read is a ref); an rvalue such as `doubleSamplePoint3(a +
// b)` is a compile error by design ("can't pass non-ref to ref") —
// das deliberately has no materialization of value arguments to
// implicit const& parameters.
//
// The inliner, however, MANUFACTURES that shape: grafting `wrapped(a
// + b)` substitutes an rvalue expression for `p`, and the body hands
// `p` to the const& extern. The inliner must bind storage (a
// generated `__inlN_arg_p` temp local) instead of substituting the
// bare value. These tests pin that behavior — interp and JIT.

[inline]
def wrapped(p : float3) : float3 => UnitTest::doubleSamplePoint3(p)

[inline]
def wrapped_chain(p : float3) : float3 {
    // two inline levels: the graft of this body hands another rvalue to
    // `wrapped`
    return wrapped(p + float3(1, 1, 1))
}

def double_sum(a, b : float3) => wrapped(a + b)

[test(description = "an inlined wrapper binds storage for an rvalue passed to a const-reference parameter")]
def test_inlined_rvalue_to_const_ref(t : T?) {
    t |> run("inlined wrapper binds storage for an rvalue argument") @(t : T?) {
        let a = float3(1, 2, 3)
        let b = float3(1, 0, 0)
        t |> equal(wrapped(a + b), float3(4, 4, 6))
        t |> equal(wrapped(float3(1, 2, 3)), float3(2, 4, 6))       // constant-folded argument
        t |> equal(wrapped_chain(a + a), float3(6, 10, 14))
        t |> equal(double_sum(a, b), float3(4, 4, 6))
    }
}

// A const pointer or reference parameter the callee writes through
// after an unsafe cast. The JIT gives a const indirect parameter the
// LLVM readonly attribute unless the compiler marked the parameter
// written; with the attribute in place the optimizer folds the
// caller's read of that memory to its pre-call value from
// --jit-opt-level=1 up. Every check passes in the interpreter and at
// --jit-opt-level=0 whatever the attribute says.

struct Cell {
    a : int
    b : int
}

// const void? out-parameter, cast to a typed pointer
[hint(noinline)]
def route_void(dst : void?; v : int) {
    var p = unsafe(reinterpret<int?>(dst))
    *p = v
}

// const typed pointer, cast to its own type
[hint(noinline)]
def route_same_type(dst : int?; v : int) {
    var p = unsafe(reinterpret<int?>(dst))
    *p = v
}

// const void? out-parameter as a memcpy destination
[hint(noinline)]
def route_memcpy(dst : void?; v : int) {
    var src = v
    unsafe {
        memcpy(dst, addr<void?>(src), 4)
    }
}

// const structure reference, address taken and cast back
[hint(noinline)]
def route_const_ref(c : Cell; v : int) {
    var p = unsafe(addr<Cell?>(c))
    p.a = v
}

// const typed pointer under a noalias hint - with no readonly the
// hint alone keeps the write
[hint(noinline, noalias = dst)]
def route_noalias(dst : int const?; v : int) {
    var p = unsafe(reinterpret<int?>(dst))
    *p = v
}

[test(description = "a write routed through a const-qualified pointer or reference parameter still reaches the caller's variable under JIT")]
def test_write_through_const_argument(t : T?) {
    t |> success(!jit_enabled() || is_jit_function(@@route_void))
    t |> run("const typed pointer under noalias") @(t : T?) {
        var slot = 0
        route_noalias(unsafe(addr(slot)), 40)
        t |> equal(40, slot)
    }
    t |> run("const void? out-parameter") @(t : T?) {
        var slot = 0
        route_void(unsafe(addr<void?>(slot)), 41)
        t |> equal(41, slot)
    }
    t |> run("const typed pointer") @(t : T?) {
        var slot = 0
        route_same_type(unsafe(addr(slot)), 42)
        t |> equal(42, slot)
    }
    t |> run("const void? memcpy destination") @(t : T?) {
        var slot = 0
        route_memcpy(unsafe(addr<void?>(slot)), 43)
        t |> equal(43, slot)
    }
    t |> run("const structure reference") @(t : T?) {
        var cell = Cell(a = 0, b = 0)
        route_const_ref(cell, 44)
        t |> equal(44, cell.a)
    }
}

// the control: the same body with a var parameter carries no readonly
// and is correct in every mode
[hint(noinline)]
def route_var_parameter(var dst : void?; v : int) {
    var p = unsafe(reinterpret<int?>(dst))
    *p = v
}

[test(description = "the same write through a var parameter reaches the caller's variable, as the control for the const-parameter cases")]
def test_var_parameter_is_the_control(t : T?) {
    var slot = 0
    route_var_parameter(unsafe(addr<void?>(slot)), 47)
    t |> equal(47, slot, "the same body with a var parameter")
}

[test(description = "a write through a const parameter in a callee from another module reaches the caller's variable without an inlining hint")]
def test_write_through_const_argument_across_modules(t : T?) {
    var slot = 0
    store_int(unsafe(addr<void?>(slot)), 46)
    t |> equal(46, slot, "a cross-partition callee needs no inlining hint")
}
