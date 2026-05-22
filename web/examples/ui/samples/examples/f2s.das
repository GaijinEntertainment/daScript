// Float→String benchmark — ported from dasProfile/tests/f2s.das.
options gen2

require strings

[sideeffects]
def str_len(s : string) : int {
    return length(s)
}

let TOTAL_NUMBERS = 10000
let TOTAL_TIMES = 100

[sideeffects]
def f2s(nums) {
    var summ_q = 0
    for (_i in 0..TOTAL_TIMES) {
        for (s in nums) {
            let str = string(s)
            summ_q += str_len(str)
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
            let str = string(mk_float(i))
            summ += str_len(str)
        }
    }
    var f1 = 0
    profile(10, "float2string") {
        f1 = f2s(nums)
    }
    assert(f1 == summ)
}
