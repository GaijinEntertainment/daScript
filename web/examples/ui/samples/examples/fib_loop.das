// Fibonacci-loop benchmark — ported from dasProfile/tests/fib_loop.das.
options gen2

[sideeffects]
def fibI(n : int) {
    var last = 1
    var cur = 0
    for (_i in range(n)) {
        let tmp = cur
        cur += last
        last = tmp
    }
    return cur
}

[export]
def main {
    var f1 = 0
    profile(10, "fibonacci loop") {
        f1 = fibI(6511134)
    }
}
