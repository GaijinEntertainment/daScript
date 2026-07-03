options gen2
options persistent_heap

// Correctness probe for the x64-avx2-acc8 tier: mm / batch / group3 against the portable backend
// on the same row-major data (no repack), block tails included. Tolerance-gated (float
// accumulation order differs by design). Run: daslang -jit acc8_probe.das

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_x64_avx
require dasllama/dasllama_quant
require daslib/jobque_boost
require math

def fillw(var a : array<float>; cnt : int64; seed : float) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = sin(float(i) * 0.37 + seed) * 0.9
    }
}

def quant_pair(w : array<float>; n, d : int64; var q : array<int8>; var s : array<float>) {
    let qt <- quantize_q8_0(w, n * d)
    q |> resize(n * d)
    s |> resize(d * (n / 32l))
    for (i in range64(n * d)) {
        q[i] = qt.q[i]
    }
    for (i in range64(d * (n / 32l))) {
        s[i] = qt.scales[i]
    }
}

def maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (i in range(length(a))) {
        m = max(m, abs(a[i] - b[i]))
    }
    return m
}

def check_shape(n, d, ntok : int64) {
    let nb = n / 32l
    var w : array<float>
    fillw(w, n * d, 0.123)
    var q : array<int8>
    var s : array<float>
    quant_pair(w, n, d, q, s)

    // activations: ntok tokens, quantized per token
    var x : array<float>
    fillw(x, n * ntok, 4.567)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(ntok * n)
    xs |> resize(ntok * nb)
    for (tk in range64(ntok)) {
        quantize_q8_0_into(x, n, xq, xs, tk * n, tk * nb)
    }

    var y_ref : array<float>
    var y_new : array<float>
    y_ref |> resize(ntok * d)
    y_new |> resize(ntok * d)

    select_kernel_backend("portable")
    matmul_q8q8_batch(y_ref, q, s, xq, xs, n, d, ntok)
    select_kernel_backend("x64-avx2-acc8")
    matmul_q8q8_batch(y_new, q, s, xq, xs, n, d, ntok)
    let md_batch = maxdiff(y_ref, y_new)

    // mm (single token) against the first token's slice
    var y1_ref : array<float>
    var y1_new : array<float>
    y1_ref |> resize(d)
    y1_new |> resize(d)
    select_kernel_backend("portable")
    matmul_q8q8(y1_ref, q, s, xq, xs, n, d)
    select_kernel_backend("x64-avx2-acc8")
    matmul_q8q8(y1_new, q, s, xq, xs, n, d)
    let md_mm = maxdiff(y1_ref, y1_new)

    // group3: split the d rows into three uneven output heads over the same weight region
    let d0 = d / 2l
    let d1 = d / 4l
    let d2 = d - d0 - d1
    var g0_ref : array<float>
    var g1_ref : array<float>
    var g2_ref : array<float>
    var g0_new : array<float>
    var g1_new : array<float>
    var g2_new : array<float>
    g0_ref |> resize(d0)
    g1_ref |> resize(d1)
    g2_ref |> resize(d2)
    g0_new |> resize(d0)
    g1_new |> resize(d1)
    g2_new |> resize(d2)
    let woff1 = d0 * n
    let woff2 = (d0 + d1) * n
    select_kernel_backend("portable")
    matmul_q8q8_group3(g0_ref, g1_ref, g2_ref, q, s, 0l, woff1, woff2, xq, xs, n, d0, d1, d2)
    select_kernel_backend("x64-avx2-acc8")
    matmul_q8q8_group3(g0_new, g1_new, g2_new, q, s, 0l, woff1, woff2, xq, xs, n, d0, d1, d2)
    let md_g3 = max(maxdiff(g0_ref, g0_new), max(maxdiff(g1_ref, g1_new), maxdiff(g2_ref, g2_new)))

    let ok = md_batch < 1.0e-3 && md_mm < 1.0e-3 && md_g3 < 1.0e-3
    print("n={n} d={d} ntok={ntok}: batch maxdiff={md_batch}  mm maxdiff={md_mm}  group3 maxdiff={md_g3}  {ok ? "OK" : "FAIL <<<<"}\n")
    if (!ok) {
        panic("acc8 probe FAILED")
    }

    delete w
    delete q
    delete s
    delete x
    delete xq
    delete xs
    delete y_ref
    delete y_new
    delete y1_ref
    delete y1_new
    delete g0_ref
    delete g1_ref
    delete g2_ref
    delete g0_new
    delete g1_new
    delete g2_new
}

[export]
def main {
    if (!jit_enabled()) {
        print("run with -jit (dot32_acc8 emits scalar fallback otherwise — still valid but slow)\n")
    }
    with_job_que() {
        setup_dasllama_jobque_()
        check_shape(64l, 8l, 4l)      // tiny: 2 blocks
        check_shape(96l, 10l, 7l)     // 3 blocks (odd block-loop tail after the 4-unroll), odd tokens
        check_shape(2048l, 64l, 5l)   // real-ish width, odd tokens
        check_shape(2048l, 2048l, 8l) // qo shape, threaded path engaged
        print("acc8 probe: ALL OK\n")
    }
}
