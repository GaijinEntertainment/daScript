// Spectral-norm benchmark — ported from dasProfile/tests/spectral-norm.das.
// Computes the dominant eigenvalue of an N×N matrix via power iteration.
// Renamed from `spectral-norm.das` (hyphen) to `spectral_norm.das` to match the
// wasm cross-compile target naming (CMakeLists' foreach() doesn't tolerate
// hyphens in target names).
options gen2
options stack = 1_000_000
options solid_context

require math

let N = 500

def A(i, j : int) {
    return double((((i + j) * (i + j + 1)) >> 1) + i + 1)
}

def dot(v, u : double[N]) : double {
    var sum = 0.0lf
    for (V, U in v, u) {
        sum += V * U
    }
    return sum
}

def mult_Av(v : double[N]; var out : double[N]) {
    for (i in range(N)) {
        var sum = 0.0lf
        for (j in range(N)) {
            sum += v[j] / A(i, j)
        }
        out[i] = sum
    }
}

def mult_Atv(v : double[N]; var out : double[N]) {
    for (i in range(N)) {
        var sum = 0.0lf
        for (j in range(N)) {
            sum += v[j] / A(j, i)
        }
        out[i] = sum;
    }
}

var tmp : double[N]
def mult_AtAv(v : double[N]; var out : double[N]) {
    mult_Av(v, tmp)
    mult_Atv(tmp, out)
}

[sideeffects]
def test {
    var u, v : double[N]
    for (i in range(N)) {
        u[i] = 1.0lf
    }
    for (_i in range(10)) {
        mult_AtAv(u, v)
        mult_AtAv(v, u)
    }
    let vBv = dot(u, v)
    let vv = dot(v, v)
    return sqrt(vBv / vv)
}

[export]
def main {
    let ref = 1.274224153lf
    var f1 : double
    profile(10, "spectral norm") {
        f1 = test()
    }
    assert(abs(f1 - ref) < 1e-5lf, "spectral norm")
}
