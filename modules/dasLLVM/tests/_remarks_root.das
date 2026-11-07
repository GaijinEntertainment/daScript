options gen2

//! a counted in-place loop: every host's vectorizer takes it
[never_inline, hint(noinline)]
def halve(var fa : float?; n : int) {
    unsafe {
        for (i in range(n)) {
            fa[i] = fa[i] * 0.5f + 1.0f
        }
    }
}

//! a loop whose exit reads the data it writes: no vectorizer takes it
[never_inline, hint(noinline)]
def halve_until_zero(var fa : float?) : int {
    var i = 0
    unsafe {
        while (fa[i] != 0.0f) {
            fa[i] = fa[i] * 0.5f
            i++
        }
    }
    return i
}

[export]
def main {
    var fa : array<float>
    fa |> resize(64)
    unsafe {
        halve(addr(fa[0]), 63)
        print("HALVED {fa[0]} {halve_until_zero(addr(fa[0]))}\n")
    }
}
