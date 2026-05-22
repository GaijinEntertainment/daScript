// Exp loop benchmark — ported from dasProfile/tests/exp.das.
// Runs an exp() loop 1M iterations × 10 samples; prints timings via `profile`.
options gen2

require math

[sideeffects]
def expLoop(n) {
    var sum = 0.
    for (i in range(n)) {
        sum += exp(rcp_est(float(i + 1)))
    }
    return sum
}

def verifyExp(f) {
    let t = 1e+06
    let eps = 10.
    let q = abs(f - t)
    assert(q < eps)
}

[export]
def main {
    var f1 = 0.
    profile(10, "exp loop") {
        f1 = expLoop(1000000)
    }
    verifyExp(f1)
}
