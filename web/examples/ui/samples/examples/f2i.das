// String→Float benchmark — ported from dasProfile/tests/f2i.das.
// (The dasProfile test name is misleading: this actually measures string→float
// parsing performance, hence the "string2float" label in the bench output.)
options gen2

require strings

let TOTAL_NUMBERS = 10000
let TOTAL_TIMES = 100

[sideeffects]
def f2i(nums) {
    var summ_q = 0.0
    for (_i in 0..TOTAL_TIMES) {
        for (s in nums) {
            summ_q += to_float(s)
        }
    }
    return summ_q
}

def mk_float(i) {
    return float(i) + float(i) / float(TOTAL_NUMBERS)
}

[export]
def main {
    let nums : array<string> <- [for (i in range(TOTAL_NUMBERS)); string(mk_float(i))]
    // reference summ
    var summ = 0.0
    for (_j in 0..TOTAL_TIMES) {
        for (i in range(TOTAL_NUMBERS)) {
            summ += mk_float(i)
        }
    }
    var f1 = 0.0
    profile(10, "string2float") {
        f1 = f2i(nums)
    }
    assert(f1 == summ)
}
