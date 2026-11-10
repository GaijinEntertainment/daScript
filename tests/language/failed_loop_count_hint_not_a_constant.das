//! a loop count hint takes an integer or the name of a constant
//! integer global: a mutable global, a float, a float constant, an
//! unknown name, a count past 32 bits and a negative count are all
//! refused
expect 30100:6

options gen2

var g_count = 4
let G_RATIO = 2.0
let G_HUGE = 4294967296l

def f(n : int) : int {
    var sum = 0
    for [unroll_count = g_count] (i in range(n)) {
        sum += i
    }
    for [interleave_count = 2.0] (i in range(n)) {
        sum += i
    }
    for [vectorize_width = G_RATIO] (i in range(n)) {
        sum += i
    }
    while [unroll_count = no_such_global] (sum > 1000000) {
        sum--
    }
    for [unroll_count = G_HUGE] (i in range(n)) {
        sum += i
    }
    for [interleave_count = -1] (i in range(n)) {
        sum += i
    }
    return sum
}

[export]
def main {
    print("{f(8)}\n")
}
