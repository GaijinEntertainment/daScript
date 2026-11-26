options gen2
options persistent_heap

require dastest/testing_boost

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER

var g_values : array<int>
var g_table : int[1024]

[benchmark]
def global_at(b : B?) {
    delete g_values
    g_values <- [for (i in range(N)); i & 1023]
    for (i in range(1024)) {
        g_table[i] = i
    }
    var expected = 0
    for (v in g_values) {
        expected += v
    }
    expected *= OUTER
    b |> run("array_local_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += g_values[i]
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("array_expr_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += g_values[N - 1 - i]
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("fixed_expr_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += g_table[i & 1023]
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("fixed_write_expr_idx/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                g_table[i & 1023] = i
            }
        }
        b |> equal(g_table[0], 9216)
    }
}
