options gen2
require dastest/testing_boost public

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

[test]
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

[test]
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

[test]
def test_a_field_or_index_argument_keeps_the_side_effects_of_its_base(t : T?) {
    var k = 0
    takes_int(make_pair(k).a)
    t |> equal(k, 1, "a field of a call result")
    var s = 0
    let v = float3(1.0)
    takes_int(int(v[bump(s)]))
    t |> equal(s, 1, "a vector index computed by a call")
}
