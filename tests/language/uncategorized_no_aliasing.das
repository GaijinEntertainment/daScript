options gen2
require dastest/testing_boost public

options no_aliasing

require _aliasing_generic_lax
require UnitTest

struct Plain {
    a : int
    b : float
}

struct IntPair {
    a : int
    b : int
}

def private store_constants(var p : Plain?; var d : Plain?; var t : tuple<int; string>?) {
    *p = default<Plain>
    *d = Plain(a = 1, b = 2.0)
    *t = (3, "three")
}

[test(description = "a default or a structure or tuple made of constants stored through a pointer compiles under no_aliasing and holds those constants")]
def test_store_constants(t : T?) {
    var p = Plain(a = 9, b = 9.0)
    var d : Plain
    var tup : tuple<int; string>
    store_constants(unsafe(addr(p)), unsafe(addr(d)), unsafe(addr(tup)))
    t |> equal(p.a, 0)
    t |> equal(p.b, 0.0)
    t |> equal(d.a, 1)
    t |> equal(d.b, 2.0)
    t |> equal(tup._0, 3)
    t |> equal(tup._1, "three")
}

def private store_handle(var p : TestObjectFoo?) {
    *p = TestObjectFoo(fooData = 5)
}

[test(description = "a C++ handled type made of constants stored through a pointer compiles under no_aliasing and holds those constants")]
def test_store_handle(t : T?) {
    var foo = TestObjectFoo(fooData = 9)
    store_handle(unsafe(addr(foo)))
    t |> equal(foo.fooData, 5)
}

struct SubRef {
    nameId : int
    mip : int
    layer : int
}

struct AttachmentRequest {
    name : string
    mip : int
    layer : int
}

struct PassRequest {
    depth : SubRef
}

def private store_other_type(var req : PassRequest&; attachment : AttachmentRequest; id : int) {
    req.depth = SubRef(nameId = id, mip = attachment.mip, layer = attachment.layer)
}

[test(description = "a structure made from a reference of a type that cannot hold the destination compiles under no_aliasing")]
def test_store_other_type(t : T?) {
    var req : PassRequest
    store_other_type(req, AttachmentRequest(name = "depth", mip = 2, layer = 3), 7)
    t |> equal(req.depth.nameId, 7)
    t |> equal(req.depth.mip, 2)
    t |> equal(req.depth.layer, 3)
}

[marker(no_coverage)]
def private half(x : float) : float => x * 0.5

struct Scaled {
    a : float
    b : float
}

def private store_pure_call(var s : Scaled&; x : float) {
    s = Scaled(a = half(x), b = x)
}

[test(description = "a structure whose values call a das function with no side effects on plain values compiles under no_aliasing")]
def test_store_pure_call(t : T?) {
    var s : Scaled
    store_pure_call(s, 4.0)
    t |> equal(s.a, 2.0)
    t |> equal(s.b, 4.0)
}

var private g_lists : table<string; array<int>>

def private store_empty_array(key : string) {
    g_lists[key] <- array<int>()
}

def private store_select(var s : Scaled&; pick : bool) {
    s = Scaled(a = pick ? 1.0 : 2.0, b = 3.0)
}

[test(description = "an empty array made with its initializer, and a structure whose value is a select of constants, compile under no_aliasing")]
def test_store_initializer_and_select(t : T?) {
    store_empty_array("a")
    t |> equal(length(g_lists["a"]), 0)
    var s : Scaled
    store_select(s, false)
    t |> equal(s.a, 2.0)
    t |> equal(s.b, 3.0)
}

let private MIN_SCALE = 0.5

struct DefaultScaled {
    a : float = 1.0
    b : float = MIN_SCALE
}

var private g_scaled = DefaultScaled()

def private store_let_global(var s : Scaled&) {
    s = Scaled(a = MIN_SCALE, b = 2.0)
    g_scaled = DefaultScaled()
}

[test(description = "a structure whose values or initializers read a let global compiles under no_aliasing, since no destination can be that global")]
def test_store_let_global(t : T?) {
    var s : Scaled
    g_scaled.b = 9.0
    store_let_global(s)
    t |> equal(s.a, 0.5)
    t |> equal(g_scaled.b, 0.5)
}

def private store_values(var p : IntPair?; var t : tuple<int; int>?; x : int) {
    *p = IntPair(a = x, b = 1)
    *t = (1, x)
}

[test(description = "a structure or tuple made of plain local values stored through a pointer compiles under no_aliasing")]
def test_store_values(t : T?) {
    var p : IntPair
    var tup : tuple<int; int>
    store_values(unsafe(addr(p)), unsafe(addr(tup)), 7)
    t |> equal(p.a, 7)
    t |> equal(p.b, 1)
    t |> equal(tup._0, 1)
    t |> equal(tup._1, 7)
}

[test(description = "an aliasing generic from a module with no_aliasing off compiles in a no_aliasing caller and swaps through a temporary")]
def test_lax_generic_instance(t : T?) {
    var q = IntPair(a = 3, b = 4)
    swap_fields(q)
    t |> equal(q.a, 4)
    t |> equal(q.b, 3)
}

struct BlockP {
    a : int
    b : int
    c : int
}

var g_block_p = BlockP(a = 10, b = 20, c = 0)

[never_inline]
def set_block_p(blk : block<() : BlockP>) {
    g_block_p = invoke(blk)
}

[never_inline]
def set_block_p_from(blk : block<(src : BlockP) : BlockP>) {
    g_block_p = invoke(blk, g_block_p)
}

[never_inline]
def local_from_block(blk : block<(src : BlockP) : BlockP>) : int {
    var p = BlockP(a = 10, b = 20, c = 0)
    p = invoke(blk, p)
    return p.c
}

[test(description = "a block invoke result stored to a target the block reads, as a global or through an argument, is built in a temporary")]
def test_store_block_invoke(t : T?) {
    g_block_p = BlockP(a = 10, b = 20, c = 0)
    set_block_p() $ => BlockP(a = 1, b = 2, c = g_block_p.a + g_block_p.b)
    t |> equal(g_block_p.c, 30)
    g_block_p = BlockP(a = 10, b = 20, c = 0)
    set_block_p_from() $(src : BlockP) => BlockP(a = 1, b = 2, c = src.a + src.b)
    t |> equal(g_block_p.c, 30)
    t |> equal(local_from_block() $(src : BlockP) => BlockP(a = 1, b = 2, c = src.a + src.b), 30)
}
