options gen2
options loop_unroll_cost = 0

module _loop_unroll_off_helper public

var public g_unroll_off = 1

def public unroll_off_sum() : int {
    var s = g_unroll_off
    for (i in range(4)) {
        s += i
    }
    return s
}
