options gen2
options no_aot

//! a child for jit_artifacts.das: a guarded load through an odd int?
//! must not run - the parity test guards it, and a typed pointer
//! parameter promises no alignment; the address comes from a global
//! and the callees stay out of line, so nothing folds the test away
var g_odd_bits = 0xffffffffu

[hint(noinline)]
def odd_free(p : int?) : bool => (intptr(p) & 1ul) == 0ul

[hint(noinline)]
def f(q : int?) : int {
    if (odd_free(q) && deref(q) != 0) {
        return 1
    }
    return 0
}

[export]
def main : int {
    unsafe {
        return f(reinterpret<int?>(g_odd_bits))
    }
}
