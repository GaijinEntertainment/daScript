options gen2
options persistent_heap

require dastest/testing_boost
require math

// Horizontal reduce (hmin/hmax/hadd) over a hot array of float4. In JIT these lower
// to a single llvm.vector.reduce.* op; without the intrinsic mapping they fall back to
// an extern call across the JIT boundary (no inline, ABI marshalling) — this benchmark
// measures that gap. Non-constant data + a global sink defeat const-fold / DCE.

let N = 2048

var g_sink = 0.0

def make_data : array<float4> {
    var data : array<float4>
    data |> resize(N)
    for (i in range(N)) {
        let f = float(i)
        data[i] = float4(f * 0.123 + 1.0, f * -0.7 + 3.0, f * 0.31 - 2.0, f * 0.05 + 0.5)
    }
    return <- data
}

[benchmark]
def bench_horizontal_reduce(b : B?) {
    let data <- make_data()
    b |> run("hmin4/{N}", N) {
        var acc = 0.0
        for (v in data) {
            acc += hmin(v)
        }
        g_sink += acc
    }
    b |> run("hmax4/{N}", N) {
        var acc = 0.0
        for (v in data) {
            acc += hmax(v)
        }
        g_sink += acc
    }
    b |> run("hadd4/{N}", N) {
        var acc = 0.0
        for (v in data) {
            acc += hadd(v)
        }
        g_sink += acc
    }
}
