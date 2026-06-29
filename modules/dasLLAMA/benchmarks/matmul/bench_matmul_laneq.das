options gen2
options persistent_heap

require dastest/testing_boost
require matmul_variants
require dasllama/dasllama_quant
require math

// Isolation proof for the lane-indexed SDOT + interleaved repack (4-row tile, 1 token) vs the current
// sdot4x4 kernel, single-thread, on the real Llama matmul shapes. The repack is amortized at model
// load, so it is done ONCE outside the timed region — only the kernel is timed. Correctness is gated
// first (max abs diff vs sdot4x4 must be ~0; the laneq path is bit-for-bit the same int math).
//
//   bin/daslang -jit dastest/dastest.das -- --test modules/dasLLAMA/benchmarks/matmul/bench_matmul_laneq.das --bench

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = float(i % 13) * 0.1 - 0.6
    }
}

def max_abs_diff(a, b : array<float>; cnt : int64) : float {
    var m = 0.0
    for (i in range64(cnt)) {
        m = max(m, abs(a[i] - b[i]))
    }
    return m
}

def bench_shape(b : B?; name : string; n, d : int64) {
    var w : array<float>
    var x : array<float>
    fillf(w, n * d)
    fillf(x, n)
    let qt8 <- quantize_q8_0(w, n * d)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(n / 32l)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)

    // repack ONCE (load-time cost, not timed)
    var wgq : array<int8>
    var wgs : array<float>
    repack_q8q8_4(qt8.q, qt8.scales, n, d, wgq, wgs)

    // correctness gate
    var yref : array<float>
    var ylq : array<float>
    yref |> resize(d)
    ylq |> resize(d)
    matmul_q8q8_sdot4x4(yref, qt8.q, qt8.scales, xq, xs, n, d)
    matmul_q8q8_laneq(ylq, wgq, wgs, xq, xs, n, d)
    let mad = max_abs_diff(yref, ylq, d)
    to_log(LOG_INFO, "{name}  max|sdot4x4 - laneq| = {mad}\n")
    verify(mad < 1.0e-3)

    b |> run("{name} q8q8_sdot4x4 single ") {
        matmul_q8q8_sdot4x4(yref, qt8.q, qt8.scales, xq, xs, n, d)
    }
    b |> run("{name} q8q8_laneq   single ") {
        matmul_q8q8_laneq(ylq, wgq, wgs, xq, xs, n, d)
    }
}

[benchmark]
def bench_matmul_laneq(b : B?) {
    bench_shape(b, "kv  256x2048 ", 2048l, 256l)
    bench_shape(b, "qo  2048x2048", 2048l, 2048l)
    bench_shape(b, "w13 5632x2048", 2048l, 5632l)
    bench_shape(b, "w2  2048x5632", 5632l, 2048l)
    bench_shape(b, "cls 32000x2048", 2048l, 32000l)
}
