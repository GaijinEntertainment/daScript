options gen2
options strict_aliasing = false

//! the program opted out of the typed model: no access carries a tag
[never_inline, hint(noinline)]
def scale_program_off(var fa : float?; n_ptr : int?; k_ptr : int?) {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
    }
}

//! the hint opts one function back in
[never_inline, hint(noinline, strict_aliasing)]
def scale_forced(var fa : float?; n_ptr : int?; k_ptr : int?) {
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
            scale_program_off(addr(fa[0]), addr(ctl[0]), addr(ctl[1]))
            scale_forced(addr(fb[0]), addr(ctl[0]), addr(ctl[1]))
        }
    }
    print("OFF {fa[7]} {fa[63]} FORCED {fb[7]} {fb[63]}\n")
}
