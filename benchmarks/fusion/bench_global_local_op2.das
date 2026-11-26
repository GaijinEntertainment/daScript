options gen2
options persistent_heap
options solid_context = true

require dastest/testing_boost

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER

var g_scale = 3
var g_bias = 0.5

[benchmark]
def global_op_local(b : B?) {
    var expected = 0
    for (i in range(N)) {
        expected += g_scale * i
    }
    expected *= OUTER
    b |> run("int_mul/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += g_scale * i
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("float_add/{TOTAL}", TOTAL) {
        var sum = 0.0
        for (_j in range(OUTER)) {
            var x = 0.0
            for (_i in range(N)) {
                x = g_bias + x
            }
            sum += x
        }
        b |> equal(sum > 0.0, true)
    }
}
