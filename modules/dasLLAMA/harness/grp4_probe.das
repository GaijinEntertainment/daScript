options gen2
options persistent_heap

// Correctness probe for the x64-avx2-repack tier: mm / batch / group3 against the portable
// backend on the same data, tails included (d % 4 != 0, ntok odd). Run: daslang -jit grp4_probe.das

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
    var qr := q     // the repack tier's copy (repacked in place)
    var sr := s

    // activations: ntok tokens, quantized per token
    var x : array<float>
    fillw(x, n * ntok, 4.567)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(ntok * n)
    xs |> resize(ntok * nb)
    for (tk in range64(ntok)) {
        quantize_q8_0_into(x, n, xq, xs, tk * n, tk * nb)   // hmm: per-token offsets — see note below
    }

    var y_ref : array<float>
    var y_new : array<float>
    y_ref |> resize(ntok * d)
    y_new |> resize(ntok * d)

    // reference: portable, row-major
    select_kernel_backend("portable")
    matmul_q8q8_batch(y_ref, q, s, xq, xs, n, d, ntok)

    // candidate: repack tier
    select_kernel_backend("x64-avx2-repack")
    repack_q8q8_weight(qr, sr, 0l, n, d)
    matmul_q8q8_batch(y_new, qr, sr, xq, xs, n, d, ntok)
    let md_batch = maxdiff(y_ref, y_new)

    // mm (single token) against the first token's slice
    var y1_ref : array<float>
    var y1_new : array<float>
    y1_ref |> resize(d)
    y1_new |> resize(d)
    select_kernel_backend("portable")
    matmul_q8q8(y1_ref, q, s, xq, xs, n, d)
    select_kernel_backend("x64-avx2-repack")
    matmul_q8q8(y1_new, qr, sr, xq, xs, n, d)
    let md_mm = maxdiff(y1_ref, y1_new)

    let ok = md_batch < 1.0e-3 && md_mm < 1.0e-3
    print("n={n} d={d} ntok={ntok}: batch maxdiff={md_batch}  mm maxdiff={md_mm}  {ok ? "OK" : "FAIL <<<<"}\n")
    if (!ok) {
        panic("grp4 probe FAILED")
    }

    delete w
    delete q
    delete s
    delete qr
    delete sr
    delete x
    delete xq
    delete xs
    delete y_ref
    delete y_new
    delete y1_ref
    delete y1_new
}

[export]
def main {
    if (!jit_enabled()) {
        print("run with -jit (dot32 emits scalar fallback otherwise — still valid but slow)\n")
    }
    with_job_que() {
        setup_dasllama_jobque_()
        check_shape(64l, 8l, 4l)      // tiny: 2 blocks, exact groups, even tokens (tile path)
        check_shape(64l, 10l, 7l)     // row tail (d%4=2) + odd tokens (tile + token tail)
        check_shape(2048l, 64l, 5l)   // real-ish width, odd tokens
        check_shape(2048l, 2048l, 8l) // qo shape, threaded path engaged
        print("grp4 probe: ALL OK\n")
    }
}
