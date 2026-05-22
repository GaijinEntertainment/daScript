// N-Queens benchmark — ported from dasProfile/tests/queen.das.
// Solves 8-queens via recursive backtracking; 100 samples (10× the
// dasProfile multiplier of 10).
options gen2
options solid_context

let N = 8

[hint(unsafe_range_check)]
def isplaceok(a : int[8]; n, c, cmn, cpn : int) {
    for (i in range(n)) {
        let ai = a[i]
        return false if (ai == c || ai == cmn + i || ai  == cpn - i)
    }
    return true
}

var solutions : int

[hint(unsafe_range_check)]
def addqueen(var a : int[8]; n : int) {
    for [unroll_full] (c in range(N)) {
        if (isplaceok(a, n, c, c - n, c + n)) {
            a[n] = c
            if (n == (N - 1)) {
                solutions++
            } else {
                addqueen(a, n + 1)
            }
        }
    }
}

def test() {
    solutions = 0
    var a : int[8]
    addqueen(a, 0)
    assert(solutions == 92)
}

[export]
def main {
    profile(100, "queen") {
        test()
    }
}
