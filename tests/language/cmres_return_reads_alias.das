options gen2
options indenting = 4

// A local returned by `return a` shares the return CMRES only
// when every return returns it: any other return builds into
// the same slot and could read `a` while overwriting it.

require dastest/testing_boost public

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

var private g_pair : Pair? = null

[never_inline]
def swapped_global() : Pair => Pair(p = g_pair.q, q = g_pair.p)

[never_inline]
def global_pointer_reads(c : bool) : Pair {
    var a = Pair(p = 5, q = 7)
    g_pair = unsafe(addr(a))
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

[test]
def test_make_local_return_reads_alias(t : T?) {
    let r = make_local_reads(true)
    t |> equal(r.p, 7)
    t |> equal(r.q, 5)
    let s = make_local_reads(false)
    t |> equal(s.p, 5)
    t |> equal(s.q, 7)
}

[test]
def test_call_return_reads_alias(t : T?) {
    let r = call_reads(true)
    t |> equal(r.p, 7)
    t |> equal(r.q, 5)
}

[test]
def test_partial_make_local_return_reads_alias(t : T?) {
    let r = partial_make_local_reads(true)
    t |> equal(r.p, 5)
    t |> equal(r.q, 0)
}

[test]
def test_method_return_reads_alias(t : T?) {
    var h = new Holder()
    let r = h->pick(true)
    t |> equal(r.p, 7)
    t |> equal(r.q, 5)
    unsafe {
        delete h
    }
}

[test]
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
