// Float→String benchmark — the original dasProfile/tests/f2s.das kernel.
options gen2

require strings

let TOTAL_NUMBERS = 10000
let TOTAL_TIMES = 100

[sideeffects]
def f2s(nums) {
    var summ_q = 0
    for (_i in 0..TOTAL_TIMES) {
        for (s in nums) {
            summ_q += length(string(s))
        }
    }
    return summ_q
}

def mk_float(i) {
    return float(i) + float(i) / float(TOTAL_NUMBERS)
}

[export]
def main {
    let nums : array<float> <- [for (i in range(TOTAL_NUMBERS)); mk_float(i)]
    // reference summ
    var summ = 0
    for (_j in 0..TOTAL_TIMES) {
        for (i in range(TOTAL_NUMBERS)) {
            summ += length(string(mk_float(i)))
        }
    }
    var f1 = 0
    profile(10, "float2string") {
        f1 = f2s(nums)
    }
    assert(f1 == summ)
}
