options gen2
// the guard counts interpreted framed calls - an AOT body recurses
// natively into the stack guard page
options no_aot
module _framed_call_depth_recursers shared

//! Every recurser is [no_jit] for the reason the module keeps out of
//! AOT: a jitted body is native. Each body holds a `var` local it
//! reads, so none is fastcall: the optimizer folds an if-return body
//! into one return, which IS fastcall; a local keeps the frame.

//! every probe result lands here, so no call is dropped as pure
var g_sink = 0
//! added to every argument, so no probe folds to a constant
var g_no_fold = 0
//! the deepest level rec_count entered before the cap stopped it
var g_reached = -1

//! counts the levels it enters, so the test learns how many frames
//! the cap leaves it above the harness's own interpreted frames
[no_jit]
def rec_count(n : int) : int {
    var level = n   // nolint:LINT003 - a var keeps the frame
    g_reached = level
    return rec_count(level + 1)
}

struct Pair {
    a : int
    b : int
}

[no_jit]
def rec_forever(n : int) : int {
    var next = n + 1   // nolint:LINT003 - a var keeps the frame
    if (next < 0) {
        return 0
    }
    return rec_forever(next)
}

[no_jit]
def rec_sum(n : int) : int {
    var here = n   // nolint:LINT003 - a var keeps the frame
    if (here == 0) {
        return 0
    }
    return here + rec_sum(n - 1)
}

//! both arguments simple: the fused two-argument shape
[no_jit]
def rec_sum2(n : int; acc : int) : int {
    var total = acc + n   // nolint:LINT003 - a var keeps the frame
    if (n == 0) {
        return acc
    }
    return rec_sum2(n - 1, total)
}

//! nine arguments: the unrolled node family stops at eight, past it
//! the variadic specialization
[no_jit]
def rec_nine(a, b, c, d, e, f, g, h : int; n : int) : int {
    var sum = a + b + c + d
    sum += e + f + g + h
    if (n == 0) {
        return sum
    }
    return rec_nine(a, b, c, d, e, f, g, h, n - 1)
}

//! a struct result: the copy-or-move-on-return call shape
[no_jit]
def rec_pair(n : int) : Pair {
    if (n == 0) {
        return Pair(a = 0, b = 0)
    }
    let p = rec_pair(n - 1)
    return Pair(a = p.a + 1, b = p.b + n)
}

//! A framed, interpreted entry ([export] is never fastcall): the
//! counted chain is this frame plus the recursion's n + 1, whether
//! the caller is interpreted, jitted or AOT.
[export, never_inline, no_jit]
def framed_rec_sum(n : int) : int => rec_sum(n)

[export, never_inline, no_jit]
def framed_rec_count() : int => rec_count(0)

[export, never_inline, no_jit]
def framed_rec_sum2(n : int) : int => rec_sum2(n, 0)

[export, never_inline, no_jit]
def framed_rec_nine(n : int) : int => rec_nine(1, 2, 3, 4, 5, 6, 7, 8, n)

[export, never_inline, no_jit]
def framed_rec_pair(n : int) : int {
    let p = rec_pair(n)
    return p.a * 1000 + p.b
}

def triangular(n : int) : int => n * (n + 1) / 2
