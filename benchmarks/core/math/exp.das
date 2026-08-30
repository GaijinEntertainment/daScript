options gen2
options persistent_heap

require dastest/testing_boost
require math

let N = 65536

var g_sink = 0.

def fill(var xs : array<float>) {
    xs |> resize(N)
    var seed = 12345u
    for (i in range(N)) {
        seed = seed * 1664525u + 1013904223u
        xs[i] = float(seed >> 8u) / float(1 << 24) * 40. - 20.
    }
}

[benchmark]
def exp_scalar(b : B?) {
    var xs : array<float>
    fill(xs)
    b |> run("exp/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += exp(x)
        }
        g_sink += acc
    }
    b |> run("exp_std/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += exp_std(x)
        }
        g_sink += acc
    }
    b |> run("exp_est/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += exp_est(x)
        }
        g_sink += acc
    }
}
