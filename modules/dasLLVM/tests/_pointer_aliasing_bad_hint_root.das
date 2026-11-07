options gen2

//! `fb` is not a parameter: the hint names nothing
[hint(noinline, noalias = fb)]
def scale(var fa : float?; n : int) {
    unsafe {
        for (i in range(n)) {
            fa[i] = fa[i] * 0.5f
        }
    }
}

//! the other two parameter hints, each naming nothing
[hint(noinline, nocapture = zz)]
def keep(var fa : float?) {
    unsafe(fa[0]) = 1.0f
}

[hint(noinline, align16 = qq)]
def aligned(var fa : float?) {
    unsafe(fa[1]) = 2.0f
}

[export]
def main {
    var fa : array<float>
    fa |> resize(8)
    scale(unsafe(addr(fa[0])), 8)
    keep(unsafe(addr(fa[0])))
    aligned(unsafe(addr(fa[0])))
    print("SCALED {fa[0]}\n")
}
