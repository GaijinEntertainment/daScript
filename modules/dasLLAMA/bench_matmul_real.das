options gen2
options persistent_heap

require dastest/testing_boost
require matmul_variants
require dasllama_quant
require daslib/jobque_boost
require math

// Re-measure the parallel-vs-single threshold and the job-splitting granularity on the REAL
// TinyLlama-1.1B matmul shapes, with the current FAST job manager (cond-var waiter + fork pool).
// The original MATMUL_PAR_THRESHOLD (2M) and num_jobs (2N) were tuned against the old, slower
// dispatcher and the stories15M toy shapes — both may have moved. Kernels are already
// correctness-gated in bench_matmul.das (same code, different shapes), so this is perf-only.
//
//   bin/daslang -jit dastest/dastest.das -- --test modules/dasLLAMA/bench_matmul_real.das --bench

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = float(i % 13) * 0.1 - 0.6
    }
}

// Bench one shape (W is [d rows x n cols], x is [n]) across kernels and num_jobs splits.
def bench_shape(b : B?; name : string; n, d : int64; hw : int64) {
    var w : array<float>
    var x : array<float>
    var y : array<float>
    fillf(w, n * d)
    fillf(x, n)
    y |> resize(d)
    let qt8 <- quantize_q8_0(w, n * d)
    let qt4 <- quantize_q4_0(w, n * d)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(n / 32l)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)

    b |> run("{name} fp32 single ") {
        matmul_w8u2(y, w, x, n, d)
    }
    b |> run("{name} fp32 par-2N ") {
        matmul_par(y, w, x, n, d, int(2l * hw))
    }
    b |> run("{name} fp32 par-4N ") {
        matmul_par(y, w, x, n, d, int(4l * hw))
    }
    b |> run("{name} q8q8 single ") {
        matmul_q8q8(y, qt8.q, qt8.scales, xq, xs, n, d)
    }
    b |> run("{name} q8q8 par-1N ") {
        matmul_q8q8_par(y, qt8.q, qt8.scales, xq, xs, n, d, int(hw))
    }
    b |> run("{name} q8q8 par-2N ") {
        matmul_q8q8_par(y, qt8.q, qt8.scales, xq, xs, n, d, int(2l * hw))
    }
    b |> run("{name} q8q8 par-4N ") {
        matmul_q8q8_par(y, qt8.q, qt8.scales, xq, xs, n, d, int(4l * hw))
    }
    b |> run("{name} q8q8 par-8N ") {
        matmul_q8q8_par(y, qt8.q, qt8.scales, xq, xs, n, d, int(8l * hw))
    }
    b |> run("{name} q4   single ") {
        matmul_q4_fv_w4u4(y, qt4.q, qt4.scales, x, n, d)
    }
    b |> run("{name} q4   par-2N ") {
        matmul_q4fvw4u4_par(y, qt4.q, qt4.scales, x, n, d, int(2l * hw))
    }
    b |> run("{name} q4   par-4N ") {
        matmul_q4fvw4u4_par(y, qt4.q, qt4.scales, x, n, d, int(4l * hw))
    }
}

[benchmark]
def bench_matmul_real(b : B?) {
    with_job_que() {
        set_jobque_fork_pool(true, true)
        let hw = int64(get_total_hw_jobs())
        bench_shape(b, "kv  256x2048 ", 2048l, 256l, hw)     // 0.52M — currently SINGLE
        bench_shape(b, "qo  2048x2048", 2048l, 2048l, hw)    // 4.2M
        bench_shape(b, "w13 5632x2048", 2048l, 5632l, hw)    // 11.5M
        bench_shape(b, "w2  2048x5632", 5632l, 2048l, hw)    // 11.5M (other aspect ratio)
        bench_shape(b, "cls 32000x2048", 2048l, 32000l, hw)  // 65.5M
    }
}
