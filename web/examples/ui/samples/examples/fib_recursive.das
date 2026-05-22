// Fibonacci-recursive benchmark — ported from dasProfile/tests/fib_recursive.das.
options gen2

[sideeffects]
def fibR(n) {
    return n if (n < 2)
    return fibR(n - 1) + fibR(n - 2)
}

[export]
def main {
    var f3 = 0
    profile(10, "fibonacci recursive") {
        f3 = fibR(31)
    }
    assert(f3 == 1346269)
}
