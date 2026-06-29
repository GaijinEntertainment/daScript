options gen2
options persistent_heap

require dastest/testing_boost
require dasllama/dasllama_math
require dasllama/dasllama_quant
require daslib/jobque_boost
require math

// Prefill payoff: processing N prompt tokens as ONE batched GEMM (weights streamed from DRAM
// once, reused across all N columns) vs the current path of N per-token GEMV calls (weights
// re-read every call). Both take pre-quantized activations, so this isolates the matmul. ns/op
// is normalized to ONE token-projection (chunk_size = ntok), so the gemm line should fall as
// ntok grows while the gemv line stays flat — that gap is the per-token speedup.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/bench_matmul_batch.das

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = float(i % 13l) * 0.1 - 0.6
    }
}

// W is [d rows x n cols]; sweep ntok and compare ntok× GEMV against one batched GEMM, fp32 and Q8.
def bench_batch_shape(b : B?; name : string; n, d : int64) {
    let nb = n / 32l
    var w : array<float>
    fillf(w, n * d)
    let qt8 <- quantize_q8_0(w, n * d)
    // one token's activation + its Q8 quant; timing is data-independent, so the gemv loop reuses it
    var x : array<float>
    fillf(x, n)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(nb)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    var yv : array<float>
    yv |> resize(d)

    let NT <- [1l, 8l, 32l, 128l]
    for (ntok in NT) {
        var xb : array<float>
        var xqb : array<int8>
        var xsb : array<float>
        var yb : array<float>
        xb |> resize(ntok * n)
        xqb |> resize(ntok * n)
        xsb |> resize(ntok * nb)
        yb |> resize(ntok * d)
        for (tk in range64(ntok)) {
            for (i in range64(n)) {
                xb[tk * n + i] = x[i]
                xqb[tk * n + i] = xq[i]
            }
            for (i in range64(nb)) {
                xsb[tk * nb + i] = xs[i]
            }
        }

        b |> run("{name} fp32 gemv x{ntok}", int(ntok)) {
            for (_tk in range64(ntok)) {
                matmul(yv, w, x, n, d)
            }
        }
        b |> run("{name} fp32 gemm n={ntok}", int(ntok)) {
            matmul_batch(yb, w, xb, n, d, ntok)
        }
        b |> run("{name} q8   gemv x{ntok}", int(ntok)) {
            for (_tk in range64(ntok)) {
                matmul_q8q8(yv, qt8.q, qt8.scales, xq, xs, n, d)
            }
        }
        b |> run("{name} q8   gemm n={ntok}", int(ntok)) {
            matmul_q8q8_batch(yb, qt8.q, qt8.scales, xqb, xsb, n, d, ntok)
        }
    }
}

[benchmark]
def bench_matmul_batch(b : B?) {
    with_job_que() {
        set_jobque_fork_pool(true, true)
        bench_batch_shape(b, "qo  2048x2048", 2048l, 2048l)
        bench_batch_shape(b, "w13 2048x5632", 2048l, 5632l)
    }
}
