options gen2

//! a loop over float? bounded and fed through two int? - the C
//! shape `for (i = 0; i < *n; i++) fa[i] = fa[i] * 0.5f + *k`
[never_inline, hint(noinline)]
def scale(var fa : float?; n_ptr : int?; k_ptr : int?) {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
    }
}

//! the same loop with the caller's promise that nothing else in
//! the function reaches what `fa` points at
[never_inline, hint(noinline, noalias = fa)]
def scale_hinted(var fa : float?; n_ptr : int?; k_ptr : int?) {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
    }
}

[export]
def main {
    var fa, fb : array<float>
    fa |> resize(64)
    fb |> resize(64)
    var ctl : int[2]
    ctl[0] = 64
    ctl[1] = 3
    for (_p in range(3)) {
        unsafe {
            scale(addr(fa[0]), addr(ctl[0]), addr(ctl[1]))
            scale_hinted(addr(fb[0]), addr(ctl[0]), addr(ctl[1]))
        }
    }
    print("SCALE {fa[7]} {fa[63]} HINTED {fb[7]} {fb[63]}\n")
}
