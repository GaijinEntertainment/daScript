options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default  // registers the portable Q8·Q8 backend (the matmul_q8q8 wrappers dispatch to it)
require dasllama/dasllama_quant
require daslib/jobque_boost

// The batched (prefill) GEMM must be BIT-FOR-BIT identical to running the per-token matmul
// ntok times — same dots, only the loop order changes. We verify both the single-thread and
// the threaded branch (the one real prefill hits) for fp32 and Q8·Q8.

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = float((i * 7l) % 13l) * 0.1 - 0.6
    }
}

// fp32: Y [ntok x d] from matmul_batch == ntok× matmul, element for element.
def check_fp32(t : T?; n, d, ntok : int64) {
    var w : array<float>
    var x : array<float>
    fillf(w, n * d)
    fillf(x, ntok * n)
    var yref : array<float>
    yref |> resize(ntok * d)
    var xt : array<float>
    xt |> resize(n)
    var yt : array<float>
    yt |> resize(d)
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xt[i] = x[tk * n + i]
        }
        matmul(yt, w, xt, n, d)
        for (i in range64(d)) {
            yref[tk * d + i] = yt[i]
        }
    }
    var y : array<float>
    y |> resize(ntok * d)
    matmul_batch(y, w, x, n, d, ntok)
    for (k in range64(ntok * d)) {
        t |> equal(y[k], yref[k])
    }
}

// Q8·Q8: same, with weights quantized once and the activation quantized per token.
def check_q8(t : T?; n, d, ntok : int64) {
    let nb = n / 32l
    var w : array<float>
    var x : array<float>
    fillf(w, n * d)
    fillf(x, ntok * n)
    let qt <- quantize_q8_0(w, n * d)
    var xq_all : array<int8>
    var xs_all : array<float>
    xq_all |> resize(ntok * n)
    xs_all |> resize(ntok * nb)
    var yref : array<float>
    yref |> resize(ntok * d)
    var xt : array<float>
    xt |> resize(n)
    var xqt : array<int8>
    xqt |> resize(n)
    var xst : array<float>
    xst |> resize(nb)
    var yt : array<float>
    yt |> resize(d)
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xt[i] = x[tk * n + i]
        }
        quantize_q8_0_into(xt, n, xqt, xst, 0l, 0l)
        matmul_q8q8(yt, qt.q, qt.scales, xqt, xst, n, d)
        for (i in range64(d)) {
            yref[tk * d + i] = yt[i]
        }
        for (i in range64(n)) {
            xq_all[tk * n + i] = xqt[i]
        }
        for (i in range64(nb)) {
            xs_all[tk * nb + i] = xst[i]
        }
    }
    var y : array<float>
    y |> resize(ntok * d)
    matmul_q8q8_batch(y, qt.q, qt.scales, xq_all, xs_all, n, d, ntok)
    for (k in range64(ntok * d)) {
        t |> equal(y[k], yref[k])
    }
}

[test]
def test_matmul_batch(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
        t |> run("fp32 single 64x5 ntok=4") @(t : T?) {
            check_fp32(t, 64l, 5l, 4l)
        }
        t |> run("fp32 ntok=1 == matmul") @(t : T?) {
            check_fp32(t, 96l, 7l, 1l)
        }
        t |> run("fp32 threaded 2048x512 ntok=2") @(t : T?) {
            check_fp32(t, 2048l, 512l, 2l)
        }
        t |> run("q8 single 64x5 ntok=4") @(t : T?) {
            check_q8(t, 64l, 5l, 4l)
        }
        t |> run("q8 threaded 2048x512 ntok=2") @(t : T?) {
            check_q8(t, 2048l, 512l, 2l)
        }
    }
}
