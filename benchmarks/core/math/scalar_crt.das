options gen2
options persistent_heap

require dastest/testing_boost
require math
require UnitTest

let N = 65536

var g_sink = 0.

def fill(var xs : array<float>; lo : float; hi : float) {
    xs |> resize(N)
    var seed = 12345u
    for (i in range(N)) {
        seed = seed * 1664525u + 1013904223u
        xs[i] = lo + float(seed >> 8u) / float(1 << 24) * (hi - lo)
    }
}

[benchmark]
def log_scalar(b : B?) {
    var xs : array<float>
    fill(xs, 0.001, 100.)
    b |> run("log/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += log(x)
        }
        g_sink += acc
    }
    b |> run("crt_logf/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += crt_logf(x)
        }
        g_sink += acc
    }
    b |> run("log2/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += log2(x)
        }
        g_sink += acc
    }
    b |> run("crt_log2f/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += crt_log2f(x)
        }
        g_sink += acc
    }
    b |> run("exp2/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += exp2(x * 0.05 - 3.)
        }
        g_sink += acc
    }
    b |> run("crt_exp2f/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += crt_exp2f(x * 0.05 - 3.)
        }
        g_sink += acc
    }
}

[benchmark]
def pow_scalar(b : B?) {
    var xs : array<float>
    var ys : array<float>
    fill(xs, 0.1, 10.)
    fill(ys, -2., 2.)
    b |> run("pow/{N}", N) {
        var acc = 0.
        for (x, y in xs, ys) {
            acc += pow(x, y)
        }
        g_sink += acc
    }
    b |> run("crt_powf/{N}", N) {
        var acc = 0.
        for (x, y in xs, ys) {
            acc += crt_powf(x, y)
        }
        g_sink += acc
    }
}

[benchmark]
def trig_scalar(b : B?) {
    var xs : array<float>
    fill(xs, -1.4, 1.4)
    b |> run("sin/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += sin(x)
        }
        g_sink += acc
    }
    b |> run("crt_sinf/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += crt_sinf(x)
        }
        g_sink += acc
    }
    b |> run("cos/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += cos(x)
        }
        g_sink += acc
    }
    b |> run("crt_cosf/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += crt_cosf(x)
        }
        g_sink += acc
    }
    b |> run("tan/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += tan(x)
        }
        g_sink += acc
    }
    b |> run("crt_tanf/{N}", N) {
        var acc = 0.
        for (x in xs) {
            acc += crt_tanf(x)
        }
        g_sink += acc
    }
}
