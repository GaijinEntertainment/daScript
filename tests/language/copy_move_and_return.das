options gen2
options no_unused_function_arguments = false
require UnitTest
require dastest/testing_boost public
require daslib/constant_expression
require _cmres_block_common
require _invoke_cmres_global_helper

struct Foo {
    a : int
    b : array<int>
}

struct Bar {
    z : array<Foo>
}

[sideeffects]
def test_clone_complex(t : T?) {
    var x, y : array<Bar>
    resize(x, 2)
    for (xx in x) {
        resize(xx.z, 4)
        for (xxx in xx.z) {
            xxx.a = 5
            resize(xxx.b, 3)
            for (ii in range(3)) {
                xxx.b[ii] = ii
            }
        }
    }
    y := x
    for (xx in x) {
        for (xxx in xx.z) {
            xxx.a = 6
            for (ii in range(3)) {
                xxx.b[ii] = ii + 1
            }
        }
    }
    for (yy in y) {
        for (yyy in yy.z) {
            t |> equal(yyy.a, 5)
            for (ii in range(3)) {
                t |> equal(yyy.b[ii], ii)
            }
        }
    }
}

[sideeffects]
def test_clone_struct(t : T?) {
    var x, y : Foo
    x.a = 5
    resize(x.b, 3)
    for (ii in range(3)) {
        x.b[ii] = ii
    }
    y := x
    x.a = 6
    for (ii in range(3)) {
        x.b[ii] = ii + 1
    }
    t |> equal(y.a, 5)
    for (ii in range(3)) {
        t |> equal(y.b[ii], ii)
    }
}

[sideeffects]
def test_clone_array(t : T?) {
    var x, y : array<int>
    resize(x, 3)
    for (ii in range(3)) {
        x[ii] = ii
    }
    y := x
    for (ii in range(3)) {
        x[ii] = ii + 1
    }
    for (ii in range(3)) {
        t |> equal(y[ii], ii)
    }
}

[sideeffects]
def test_clone_table(t : T?) {
    var x <- {for (ii in range(3)); ii => string(ii)}
    var y : table<int; string>
    y := x
    for (ii in range(3)) {
        x |> insert(ii, string(ii + 1))
    }
    t |> equal(length(y), 3)
    for (ii in range(3)) {
        t |> equal(y |> get_value(ii), string(ii))
    }
}

// custom clone overloads on a struct: regression for synthesized
// tuple/variant clone constness propagation. The outer `:= ` on the
// tuple must propagate the RHS constness to each field, so the user's
// const-flavor clone fires for `let` (const) tuples and the non-const
// flavor fires for `var` (non-const) tuples.
struct CFoo {
    a : int
    b : array<int>
    via : int   // 1 = const path, 2 = non-const path
}

def clone(var res : CFoo; var src : CFoo ==const) {
    res.a = src.a
    res.b := src.b
    res.via = 2
}

def clone(var res : CFoo; src : CFoo) {
    res.a = src.a
    res.b := src.b
    res.via = 1
}

[sideeffects]
def test_clone_tuple_constness_dispatch(t : T?) {
    let tup1 = (10, CFoo(a = 1))
    var tup2 = (20, CFoo(a = 2))
    let c1 := tup1
    let c2 := tup2
    t |> equal(c1._0, 10)
    t |> equal(c1._1.a, 1)
    t |> equal(c1._1.via, 1)
    t |> equal(c2._0, 20)
    t |> equal(c2._1.a, 2)
    t |> equal(c2._1.via, 2)
}

variant VFoo {
    a : array<string>
    b : float
}

typedef TBar = tuple<a : array<string>; b : float>

struct SHolder {
    f : CFoo
}

def make_vfoo(arr : array<string>) : VFoo => VFoo(a := arr)

def make_tbar(arr : array<string>) : TBar => TBar(a := arr, b = 1.5)

def make_generic(proto : auto(TT); arr : array<string>) : TT -const => <- TT(a := arr, b = 2.5)

def take_named(a : array<string>) : int => length(a)

[sideeffects]
def test_clone_init_field(t : T?) {
    var arr <- ["x", "y"]
    let vf <- make_vfoo(arr)
    t |> equal(length(arr), 2)
    t |> success(vf is a)
    t |> equal(length(vf as a), 2)
    arr[0] = "z"
    t |> equal((vf as a)[0], "x")
    let vb <- VFoo(b := 1)
    t |> equal(vb as b, 1.0)
    let ev <- variant type<VFoo>(a := arr)
    t |> equal(length(arr), 2)
    t |> equal((ev as a)[0], "z")
    let tb <- make_tbar(arr)
    t |> equal(length(arr), 2)
    t |> equal(length(tb.a), 2)
    t |> equal(tb.b, 1.5)
    arr[1] = "w"
    t |> equal(tb.a[1], "y")
    let proto : TBar
    let tg <- make_generic(proto, arr)
    t |> equal(length(arr), 2)
    t |> equal(tg.a[1], "w")
    t |> equal(tg.b, 2.5)
    let td : TBar[2] <- fixed_array(TBar(a := arr, b = 3.0), TBar(a := arr, b = 4.0))
    t |> equal(length(arr), 2)
    t |> equal(td[1].a[0], "z")
    t |> equal(td[1].b, 4.0)
    t |> equal(take_named(a := arr), 2)
    t |> equal(length(arr), 2)
    let src = CFoo(a = 7)
    let sh <- SHolder(f := src)
    t |> equal(sh.f.a, 7)
    t |> equal(sh.f.via, 1)
    var vsrc = CFoo(a = 8)
    let sh2 <- SHolder(f := vsrc)
    t |> equal(sh2.f.a, 8)
    t |> equal(sh2.f.via, 2)
}

[test(description = "clone (:=) deep-copies arrays, tables, structs and nested containers, picks a user clone by the source's constness, and clone-initializes variant, tuple and struct fields")]
def test_clone(t : T?) {
    test_clone_array(t)
    test_clone_table(t)
    test_clone_struct(t)
    test_clone_complex(t)
    test_clone_tuple_constness_dispatch(t)
    test_clone_init_field(t)
}

[export]
def test_if_compiles(var a : dasvector`TestObjectFoo; b : TestObjectFoo#) {
    push_clone(a, b)
}

struct CloneTempFoo {
    a : int = 10
    s : string = "bar"
}

def test_clone_temp_c() : CloneTempFoo const# {
    unsafe {
        return struct<CloneTempFoo#>(uninitialized a = 4)
    }
}

[test(description = "a const temporary struct returned by value clones into a local with its field values")]
def test_clone_temp(t : T?) {
    t |> run("clone temporary struct") @(t : T?) {
        let foo = test_clone_temp_c()
        let cop := foo
        t |> equal(foo.a, 4)
        t |> equal(cop.a, 4)
    }
}

let {
    g_ctm_a <- [1, 2, 3]
    g_ctm_b := g_ctm_a
}

[test(description = "clone-initializing an array from another, global or local, copies every element and leaves the source intact")]
def test_clone_to_move(t : T?) {
    t |> run("global clone-assign arrays") @(t : T?) {
        t |> equal(length(g_ctm_a), 3)
        t |> equal(length(g_ctm_b), 3)
        for (x, y in g_ctm_a, g_ctm_b) {
            t |> equal(x, y)
        }
    }
    t |> run("local clone-assign arrays") @(t : T?) {
        let a <- [1, 2, 3]
        let b := a
        t |> equal(length(a), 3)
        t |> equal(length(b), 3)
        for (x, y in a, b) {
            t |> equal(x, y)
        }
    }
}

struct BlockCounts {
    before : int
    count : int
    after : int
}

[never_inline]
def copied_block_result() : BlockCounts {
    var result = BlockCounts(before = 11, count = 3, after = 17)
    for (_ in range(4)) {
        let reply = call_with_result($(value) {
            result.count += value
        })
        result.after += reply.code
    }
    return result
}

struct BlockValues {
    values : array<int>
}

[never_inline]
def moved_block_result() : BlockValues {
    var result : BlockValues
    call_with_result($(value) {
        result.values |> push(value)
    })
    return <- result
}

[never_inline]
def read_captured_result(var observed : int&) : BlockCounts {
    let result = BlockCounts(before = 11, count = 3, after = 17)
    call_with_result($(_value) {
        call_with_result($(_nested) {
            observed = result.before + result.count + result.after
        })
    })
    return result
}

[test(description = "a returned local that a block captures and writes keeps every write the block made when it is copied out")]
def test_copied_local_captured_in_block(t : T?) {
    let result = copied_block_result()
    t |> equal(result.before, 11)
    t |> equal(result.count, 31)
    t |> equal(result.after, 93)
}

[test(description = "a returned local that a block captures and pushes into keeps the pushed element when it is moved out")]
def test_moved_local_captured_in_block(t : T?) {
    let result <- moved_block_result()
    t |> equal(length(result.values), 1)
    if (length(result.values) == 1) {
        t |> equal(result.values[0], 7)
    }
}

[test(description = "a nested block reading the local that is later returned sees its real field values, not the return slot")]
def test_return_local_read_by_nested_block(t : T?) {
    var observed = 0
    let result = read_captured_result(observed)
    t |> equal(observed, 31)
    t |> equal(result.before, 11)
    t |> equal(result.count, 3)
    t |> equal(result.after, 17)
}

// a local aliasing the return CMRES (`var r` + `return <- r`), read
// as a call argument inside a HEAP-mode array literal:
// SimNode_MakeArrayHeap repoints abiCMRES at the heap block while it
// builds the elements, so without the VarCMRes fence those reads land
// in the zeroed heap buffer
struct Rec {
    s : uint64
    bind : uint
    items : array<int>
}

struct MiniRes {
    set0 : uint64
    set1 : uint64
}

def private mk(h : uint64; b : uint) : Rec {
    var w = Rec(s = h, bind = b)
    w.items |> push(int(b))
    return <- w
}

def private consume(r : MiniRes) : uint64 {
    // any gen2 [..] literal lowers to to_array_move(make-array) and is
    // heap-mode whatever its length
    var writes <- [
        mk(r.set0, 0u), mk(r.set0, 1u), mk(r.set0, 2u), mk(r.set0, 3u), mk(r.set0, 4u),
        mk(r.set0, 5u), mk(r.set0, 6u), mk(r.set0, 7u), mk(r.set0, 8u),
        mk(r.set1, 9u), mk(r.set1, 10u), mk(r.set1, 11u), mk(r.set1, 12u), mk(r.set1, 13u),
        mk(r.set1, 14u), mk(r.set1, 15u), mk(r.set1, 16u), mk(r.set1, 17u), mk(r.set1, 18u)]
    var sum = 0ul
    for (w in writes) {
        sum += w.s
    }
    return sum
}

def build_direct : MiniRes {
    var r = MiniRes(set0 = 0x1111ul, set1 = 0x2222ul)
    var writes <- [
        mk(r.set0, 0u), mk(r.set0, 1u), mk(r.set0, 2u), mk(r.set0, 3u), mk(r.set0, 4u),
        mk(r.set0, 5u), mk(r.set0, 6u), mk(r.set0, 7u), mk(r.set0, 8u),
        mk(r.set1, 9u), mk(r.set1, 10u), mk(r.set1, 11u), mk(r.set1, 12u), mk(r.set1, 13u),
        mk(r.set1, 14u), mk(r.set1, 15u), mk(r.set1, 16u), mk(r.set1, 17u), mk(r.set1, 18u)]
    var sum = 0ul
    for (w in writes) {
        sum += w.s
    }
    r.set0 = sum        // 9*0x1111 + 10*0x2222 = 0x9999 + 0x15554 = 0x1EEED
    return <- r
}

def build_via_callee : MiniRes {
    // the literal arrives via a by-ref callee - the shape auto-inline
    // splices into the CMRES-elided local
    var r = MiniRes(set0 = 0x1111ul, set1 = 0x2222ul)
    r.set0 = consume(r)
    return <- r
}

[test(description = "a heap-mode array literal built next to a CMRES-elided return local reads that local's fields, not the zeroed heap buffer")]
def test_cmres_heap_literal_direct(t : T?) {
    let r <- build_direct()
    t |> equal(r.set0, 0x1EEEDul)
    t |> equal(r.set1, 0x2222ul)
}

[test(description = "a heap-mode array literal in a by-ref callee of a CMRES-elided return local reads the local's fields correctly")]
def test_cmres_heap_literal_via_callee(t : T?) {
    let r <- build_via_callee()
    t |> equal(r.set0, 0x1EEEDul)
    t |> equal(r.set1, 0x2222ul)
}

// a local returned by `return a` shares the return CMRES only when
// every return returns it: any other return builds into the same slot
// and could read `a` while overwriting it
struct Pair {
    p : int
    q : int
}

[never_inline]
def swapped(x : Pair) : Pair => Pair(p = x.q, q = x.p)

[never_inline]
def make_local_reads(c : bool) : Pair {
    let a = Pair(p = 5, q = 7)
    if (c) return Pair(p = a.q, q = a.p)
    return a
}

[never_inline]
def call_reads(c : bool) : Pair {
    let a = Pair(p = 5, q = 7)
    if (c) return swapped(a)
    return a
}

[never_inline]
def partial_make_local_reads(c : bool) : Pair {
    let a = Pair(p = 5, q = 7)
    if (c) return Pair(p = a.p)
    return a
}

[never_inline]
def reference_reads(c : bool) : Pair {
    var a = Pair(p = 5, q = 7)
    let b & = unsafe(a)
    if (c) return Pair(p = b.q, q = b.p)
    return a
}

[never_inline]
def pointer_reads(c : bool) : Pair {
    var a = Pair(p = 5, q = 7)
    let pa = unsafe(addr(a))
    if (c) return Pair(p = pa.q, q = pa.p)
    return a
}

[never_inline]
def block_reads(c : bool) : Pair {
    let a = Pair(p = 5, q = 7)
    let get_q = $ { return a.q }
    let get_p = $ { return a.p }
    if (c) return Pair(p = invoke(get_q), q = invoke(get_p))
    return a
}

var private g_rra_pair : Pair? = null

[never_inline]
def swapped_global() : Pair => Pair(p = g_rra_pair.q, q = g_rra_pair.p)

[never_inline]
def global_pointer_reads(c : bool) : Pair {
    var a = Pair(p = 5, q = 7)
    g_rra_pair = unsafe(addr(a))
    if (c) return swapped_global()
    return a
}

class Holder {
    def pick(c : bool) : Pair {
        let a = Pair(p = 5, q = 7)
        if (c) return Pair(p = a.q, q = a.p)
        return a
    }
}

[test(description = "a struct literal returned on one path that reads the local returned on the other gets the local's original fields")]
def test_make_local_return_reads_alias(t : T?) {
    let r = make_local_reads(true)
    t |> equal(r.p, 7)
    t |> equal(r.q, 5)
    let s = make_local_reads(false)
    t |> equal(s.p, 5)
    t |> equal(s.q, 7)
}

[test(description = "a call result returned on one path that reads the local returned on the other gets the local's original fields")]
def test_call_return_reads_alias(t : T?) {
    let r = call_reads(true)
    t |> equal(r.p, 7)
    t |> equal(r.q, 5)
}

[test(description = "a partial struct literal returned beside a returned local zero-fills the fields it omits")]
def test_partial_make_local_return_reads_alias(t : T?) {
    let r = partial_make_local_reads(true)
    t |> equal(r.p, 5)
    t |> equal(r.q, 0)
}

[test(description = "a class method's other return path reads its returned local's original fields")]
def test_method_return_reads_alias(t : T?) {
    var h = new Holder()
    let r = h->pick(true)
    t |> equal(r.p, 7)
    t |> equal(r.q, 5)
    unsafe {
        delete h
    }
}

[test(description = "the other return path reads the returned local's original fields through a reference, a pointer, a block and a global pointer")]
def test_indirect_return_reads_alias(t : T?) {
    let r = reference_reads(true)
    t |> equal(r.p, 7)
    t |> equal(r.q, 5)
    let s = pointer_reads(true)
    t |> equal(s.p, 7)
    t |> equal(s.q, 5)
    let u = block_reads(true)
    t |> equal(u.p, 7)
    t |> equal(u.q, 5)
    let g = global_pointer_reads(true)
    t |> equal(g.p, 7)
    t |> equal(g.q, 5)
}

struct CmrFoo {
    bar : int
}

def cmr_newFoo() : CmrFoo {
    var tf : CmrFoo
    tf.bar = 1
    return tf
}

def cmr_newFooB(b : block<CmrFoo>) : CmrFoo => invoke(b)

def cmr_addBar(a, b : CmrFoo) : int => a.bar + b.bar

def cmr_makeSequence(var base : auto(numT) -&; max : numT) : array<numT> {
    var a : array<numT>
    resize(a, int(max) + 1 - int(base))
    for (tf in a) {
        tf = base++
    }
    return <- a
}

def cmr_joinSequences(l : array<auto(numT)>; r : array<numT>) : array<numT> {
    var a : array<numT>
    push_from(a, l, r)
    return <- a
}

def cmr_makeSequenceB(b : block<array<auto>>) : auto => <- invoke(b)

[test(description = "a struct returns by copy and an array by move, from functions and from blocks, as call arguments")]
def test_copy_and_move_on_return(t : T?) {
    t |> run("copy on return") @(t : T?) {
        let val = cmr_addBar(cmr_newFoo(), cmr_newFoo())
        t |> equal(val, 2)
        let val2 = cmr_addBar(
            cmr_newFooB() ${ var tf : CmrFoo; tf.bar = 1; return tf; },
            cmr_newFooB() ${ var tf : CmrFoo; tf.bar = 1; return tf; }
        )
        t |> equal(val2, 2)
    }
    t |> run("move on return") @(t : T?) {
        let z <- cmr_joinSequences(cmr_makeSequence(0, 4), cmr_makeSequence(5, 9))
        t |> equal(length(z), 10)
        for (i in range(9)) {
            t |> equal(z[i], i)
        }
    }
    t |> run("move on return in block") @(t : T?) {
        let z <- cmr_joinSequences(
            cmr_makeSequenceB() ${ return <- cmr_makeSequence(0, 4); },
            cmr_makeSequenceB() ${ return <- cmr_makeSequence(5, 9); }
        )
        t |> equal(length(z), 10)
        for (i in range(9)) {
            t |> equal(z[i], i)
        }
    }
}

struct MorPipeline {
    a : int
}

def operator delete(var p : MorPipeline) {
    assert(p.a == 2)
    p.a = 0
}

def mor_create_pipelines() : array<MorPipeline?> =>
    <- [new MorPipeline(a = 1), new MorPipeline(a = 2)]

def mor_fooBar() : MorPipeline? {
    var pipelines <- mor_create_pipelines()
    return <- pipelines[0]
} finally {
    unsafe {
        delete pipelines
    }
}

[test(description = "an element moved out of an array in the return survives the finally block that deletes the array")]
def test_move_on_return(t : T?) {
    t |> run("move on return with finally delete") @(t : T?) {
        var x = mor_fooBar()
        t |> equal(x.a, 1)
    }
}

var g_rr_t = 2

[unsafe_operation]
def rr_ret_ref() : auto& {
    var qq : auto& = g_rr_t
    unsafe {
        return qq
    }
}

[unsafe_operation]
def rr_ret_safe_ref(var a : auto&) : auto& {
    unsafe {
        return a
    }
}

def rr_add2(bl : block<int&>) {
    invoke(bl) += 2
}

[test(description = "a function or block returning a reference hands back the referenced variable itself, so writes through the result reach it")]
def test_return_reference(t : T?) {
    t |> run("return global ref") @(t : T?) {
        g_rr_t = 2
        unsafe {
            var x = rr_ret_ref()
            let xx : int& = rr_ret_safe_ref(x)
            t |> equal(xx, 2)
        }
    }
    t |> run("assign via returned ref") @(t : T?) {
        g_rr_t = 2
        unsafe {
            let x = rr_ret_ref()
            let z : int& = rr_ret_ref()
            t |> equal(x, 2)
            t |> equal(z, 2)
            rr_ret_ref() = 3
            t |> equal(x, 2)
            t |> equal(z, 3)
        }
    }
    t |> run("block returning ref") @(t : T?) {
        g_rr_t = 2
        unsafe {
            var x = rr_ret_ref()
            var xx : int& = rr_ret_safe_ref(x)
            xx = 4
            t |> equal(x, 4)
            rr_add2() $() : auto& {
                unsafe {
                    return x
                }
            }
            t |> equal(x, 6)
        }
    }
}

var g_hits = 0

def bump() {
    g_hits++
}

def bump_by(n : int) {
    g_hits += n
}

def call_void(a : block<(x : int) : void>) {
    a(3)
}

def ret_void_explicit(n : int) : void {
    return bump_by(n)
}

def ret_void_auto(n : int) {
    return bump_by(n)
}

def ret_void_early(n : int) {
    if (n > 0) {
        return bump_by(n)
    }
    bump_by(-1)
}

def ret_void_pipe(n : int) {
    return n |> bump_by()
}

def ret_void_fn_arrow(n : int) => bump_by(n)

def forward_void(blk : block<() : void>) {
    return blk()
}

def forward_generic(blk) => blk()

def mixed_result(n : int) : int {
    call_void() $(x) => bump_by(x)
    return g_hits + n
}

// until [constant_expression] folds the argument in, the raw instance
// is an empty void function
[generic, constant_expression(name), unused_argument(name)]
def get_by_name(name : string) {
    static_if (!typeinfo is_argument(name)) {
        static_if (name == "one") {
            return 1
        } static_elif (name == "two") {
            return 2
        }
    }
}

def pick_by_name(sel : int) {
    if (sel == 1) {
        return get_by_name("one")
    }
    return get_by_name("two")
}

def ping(n : int) : void {
    if (n <= 0) {
        return
    }
    g_hits++
    return pong(n - 1)
}

def pong(n : int) : void {
    if (n <= 0) {
        return
    }
    g_hits++
    return ping(n - 1)
}

[test(description = "return of a void call runs the call and returns, in blocks, lambdas, local functions, explicit, auto, early-out, piped and arrow-bodied functions")]
def test_return_void_expression(t : T?) {
    t |> run("block arrow with void body") @(t : T?) {
        g_hits = 0
        call_void() $(x) => bump_by(x)
        t |> equal(g_hits, 3)
    }

    t |> run("block arrow with explicit void result") @(t : T?) {
        g_hits = 0
        call_void() $(x : int) : void => bump_by(x)
        t |> equal(g_hits, 3)
    }

    t |> run("lambda arrow with void body") @(t : T?) {
        g_hits = 0
        let f = @(x : int) => bump_by(x)
        invoke(f, 5)
        t |> equal(g_hits, 5)
    }

    t |> run("local function arrow with void body") @(t : T?) {
        g_hits = 0
        let f = @@(x : int) => bump_by(x)
        invoke(f, 7)
        t |> equal(g_hits, 7)
    }

    t |> run("explicit return of a void call") @(t : T?) {
        g_hits = 0
        ret_void_explicit(11)
        t |> equal(g_hits, 11)
    }

    t |> run("auto result forced to void") @(t : T?) {
        g_hits = 0
        ret_void_auto(13)
        t |> equal(g_hits, 13)
        let fp : function<(n : int) : void> = @@ret_void_auto
        g_hits = 0
        invoke(fp, 2)
        t |> equal(g_hits, 2)
    }

    t |> run("early-out return of a void call") @(t : T?) {
        g_hits = 0
        ret_void_early(17)
        t |> equal(g_hits, 17)
        g_hits = 0
        ret_void_early(0)
        t |> equal(g_hits, -1)
    }

    t |> run("pipe into void call in return position") @(t : T?) {
        g_hits = 0
        ret_void_pipe(19)
        t |> equal(g_hits, 19)
    }

    t |> run("named function arrow body with void call") @(t : T?) {
        g_hits = 0
        ret_void_fn_arrow(23)
        t |> equal(g_hits, 23)
    }
}

[test(description = "a void block result forwards through return, a transiently void generic call does not poison an auto result, and mutually recursive void functions forward through return")]
def test_return_void_forwarding(t : T?) {
    t |> run("forwarding a void block result") @(t : T?) {
        g_hits = 0
        forward_void() $ => bump()
        t |> equal(g_hits, 1)
    }

    t |> run("generic forwarding of a void block") @(t : T?) {
        g_hits = 0
        forward_generic() $ => bump()
        t |> equal(g_hits, 1)
    }

    t |> run("void block inside a non-void function") @(t : T?) {
        g_hits = 0
        t |> equal(mixed_result(10), 13)
    }

    t |> run("transiently-void generic call does not poison a bare-auto result") @(t : T?) {
        t |> equal(pick_by_name(1), 1)
        t |> equal(pick_by_name(2), 2)
    }

    t |> run("mutually recursive void functions forward through return") @(t : T?) {
        g_hits = 0
        ping(5)
        t |> equal(g_hits, 5)
    }
}

struct InscopeFoo {
    a : int
}

[sideeffects]
def inscope_buggy(skip : bool) {
    var a = new InscopeFoo
    if (skip) {
        return
    }
    var b = new InscopeFoo
} finally {
    unsafe {
        delete a
        delete b
    }
}

[test(description = "an early return before a later local is declared still runs the finally block that deletes both locals, without touching the undeclared one")]
def test_inscope_return(t : T?) {
    t |> run("inscope return with finally delete") @(t : T?) {
        inscope_buggy(true)
        t |> success(true)
    }
}

struct IcFoo {
    a, b : int
}

def func : IcFoo => IcFoo(a = 1, b = 2)

def funcC : IcFoo const => IcFoo(a = 1, b = 2)

def funcA(t : int) : IcFoo => IcFoo(a = t, b = t + 1)

def funcAC(t : int) : IcFoo const => IcFoo(a = t, b = t + 1)

[hybrid]
def hfunc : IcFoo => IcFoo(a = 1, b = 2)

[hybrid]
def hfuncC : IcFoo const => IcFoo(a = 1, b = 2)

[hybrid]
def hfuncA(t : int) : IcFoo => IcFoo(a = t, b = t + 1)

[hybrid]
def hfuncAC(t : int) : IcFoo const => IcFoo(a = t, b = t + 1)

def check(t : T?; f : IcFoo) {
    t |> equal(f.a, 1)
    t |> equal(f.b, 2)
}

def checkInvoke(t : T?; obj) {
    check(t, invoke(obj))
}

def checkInvoke(t : T?; arg; obj) {
    check(t, invoke(obj, arg))
}

[test(description = "a struct returned by value arrives intact from direct and hybrid calls and from invoked function pointers, blocks and lambdas, const or not")]
def test_invoke_cmres(t : T?) {
    t |> run("direct function calls") @(t : T?) {
        check(t, func())
        check(t, funcA(1))
        check(t, funcC())
        check(t, funcAC(1))
    }
    t |> run("hybrid function calls") @(t : T?) {
        check(t, hfunc())
        check(t, hfuncA(1))
        check(t, hfuncC())
        check(t, hfuncAC(1))
    }
    t |> run("invoke function pointers") @(t : T?) {
        checkInvoke(t, @@func)
        checkInvoke(t, 1, @@funcA)
        checkInvoke(t, @@funcC)
        checkInvoke(t, 1, @@funcAC)
    }
    t |> run("invoke blocks") @(t : T?) {
        checkInvoke(t) $() : IcFoo {
            return IcFoo(a = 1, b = 2)
        }
        checkInvoke(t) $() : IcFoo const {
            return IcFoo(a = 1, b = 2)
        }
        checkInvoke(t, 1) $(tt : int) : IcFoo {
            return IcFoo(a = tt, b = tt + 1)
        }
        checkInvoke(t, 1) $(tt : int) : IcFoo const {
            return IcFoo(a = tt, b = tt + 1)
        }
    }
    t |> run("invoke lambdas") @(t : T?) {
        checkInvoke(t) <| @() : IcFoo {
            return IcFoo(a = 1, b = 2)
        }
        checkInvoke(t) <| @() : IcFoo const {
            return IcFoo(a = 1, b = 2)
        }
        checkInvoke(t, 1) <| @(tt : int) : IcFoo {
            return IcFoo(a = tt, b = tt + 1)
        }
        checkInvoke(t, 1) <| @(tt : int) : IcFoo const {
            return IcFoo(a = tt, b = tt + 1)
        }
    }
}

def swapped_pair() : tuple<int; int> => (g_pair._1, g_pair._0)

[test(description = "an invoked function writing its tuple result into another module's global reads that global's old value first")]
def test_invoke_cmres_into_global_read_by_later_module(t : T?) {
    g_pair = (1, 2)
    set_pair(@@swapped_pair)
    t |> equal(g_pair._0, 2)
    t |> equal(g_pair._1, 1)
}

// the write a recursive self-call makes through a `var T&` argument
// is recorded by the access analysis, so a later read of that local
// is not const-folded back to its initializer
def rec_cond(n : int; var out : bool&) : bool {
    if (n <= 1) { out = true; return true }
    var inner = false
    let unused = rec_cond(n - 1, inner)
    if (inner) { return true } // nolint:STYLE017 - the condition read is under test
    return false
}

def rec_ret(n : int; var out : bool&) : bool {
    if (n <= 1) { out = true; return true }
    var inner = false
    let unused = rec_ret(n - 1, inner)
    return inner
}

def rec_int(n : int; var acc : int&) : int {
    if (n <= 0) { acc = 100; return 100 }
    var inner = 0
    let unused = rec_int(n - 1, inner)
    if (inner == 100) { return inner }
    return -1
}

// mutual recursion through a cycle: both the back edge and the
// forward edge record the write
def mr_a(n : int; var out : bool&) : bool {
    if (n <= 1) { out = true; return true }
    var inner = false
    let unused = mr_b(n - 1, inner)
    return inner
}
def mr_b(n : int; var out : bool&) : bool {
    if (n <= 1) { out = true; return true }
    var inner = false
    let unused = mr_a(n - 1, inner)
    return inner
}

def leaf(x : int; var out : bool&) : bool { out = x > 0; return out }
def caller(x : int) : bool {
    var inner = false
    let unused = leaf(x, inner)
    if (inner) { return true } // nolint:STYLE017 - mirrors rec_cond's condition-read shape
    return false
}

[test(description = "a write a recursive or mutually recursive call makes through a var reference argument is seen by the caller's later read, in a condition, a return and arithmetic")]
def test_recursive_ref_out_param(t : T?) {
    t |> run("direct recursion, condition read") @(t : T?) {
        var b = false
        t |> success(rec_cond(2, b), "recursive write read in a condition")
    }
    t |> run("direct recursion, return read") @(t : T?) {
        var b = false
        t |> success(rec_ret(2, b), "recursive write read as a return value")
    }
    t |> run("direct recursion, int condition read") @(t : T?) {
        var a = 0
        t |> equal(rec_int(1, a), 100)
    }
    t |> run("mutual recursion") @(t : T?) {
        var b1 = false
        var b2 = false
        t |> success(mr_a(2, b1), "mutual recursion a->b")
        t |> success(mr_b(2, b2), "mutual recursion b->a")
    }
    t |> run("non-recursive control") @(t : T?) {
        t |> success(caller(5), "non-recursive write read in a condition")
    }
}
