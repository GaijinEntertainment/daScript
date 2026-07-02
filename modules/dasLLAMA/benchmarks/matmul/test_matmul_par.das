options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require matmul_variants
require dasllama/dasllama_quant
require daslib/jobque_boost
require math

def fill(var a : array<float>; count : int) {
    a |> resize(count)
    for (i in range(count)) {
        a[i] = float(i % 13) * 0.05 - 0.3
    }
}

[test]
def test_matmul_threaded(t : T?) {
    // Shape past MATMUL_PAR_THRESHOLD (n*d = 2.36M) so production matmul takes the THREADED
    // path. Compared against the scalar matmul_naive ground truth: a threading bug (wrong row
    // partition, race, pointer error) yields large errors; fp reorder stays well under 1e-2.
    // (The matmul call MUST be inside with_job_que, which itself proves it threads.)
    t |> run("threaded matmul matches scalar ground truth") @(t : T?) {
        let n = 288
        let d = 8192
        var w : array<float>
        var x : array<float>
        var yref : array<float>
        var ypar : array<float>
        fill(w, n * d)
        fill(x, n)
        yref |> resize(d)
        ypar |> resize(d)
        matmul_naive(yref, w, x, int64(n), int64(d))
        with_job_que() {
            matmul(ypar, w, x, int64(n), int64(d))
        }
        var maxerr = 0.0
        for (i in range(d)) {
            maxerr = max(maxerr, abs(ypar[i] - yref[i]))
        }
        t |> success(maxerr < 1.0e-2)
    }
}

[test]
def test_matmul_q8q8_group3(t : T?) {
    // The fused Q/K/V kernel (one parallel_for over three row-groups sharing x) must produce the
    // same outputs as running matmul_q8q8 per group. A row-routing bug (wrong offset / segment) or a
    // race would show as large per-group errors. Concat-quantizing == per-group quantizing (blocks
    // are 32-aligned), so the comparison is fp-exact bar reorder.
    t |> run("fused Q8Q8 group3 matches per-group matmul") @(t : T?) {
        let n = 64l
        let d0 = 96l
        let d1 = 32l
        let d2 = 32l
        var wg0 : array<float>
        var wg1 : array<float>
        var wg2 : array<float>
        fill(wg0, int(d0 * n))
        fill(wg1, int(d1 * n))
        fill(wg2, int(d2 * n))
        var wall : array<float>
        wall |> resize((d0 + d1 + d2) * n)
        for (i in range64(d0 * n)) {
            wall[i] = wg0[i]
        }
        for (i in range64(d1 * n)) {
            wall[d0 * n + i] = wg1[i]
        }
        for (i in range64(d2 * n)) {
            wall[(d0 + d1) * n + i] = wg2[i]
        }
        let qall <- quantize_q8_0(wall, (d0 + d1 + d2) * n)
        let q0 <- quantize_q8_0(wg0, d0 * n)
        let q1 <- quantize_q8_0(wg1, d1 * n)
        let q2 <- quantize_q8_0(wg2, d2 * n)
        var x : array<float>
        fill(x, int(n))
        var xq : array<int8>
        var xs : array<float>
        xq |> resize(int(n))
        xs |> resize(int(n / 32l))
        quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
        var y0 : array<float>
        var y1 : array<float>
        var y2 : array<float>
        var r0 : array<float>
        var r1 : array<float>
        var r2 : array<float>
        y0 |> resize(int(d0)); y1 |> resize(int(d1)); y2 |> resize(int(d2))
        r0 |> resize(int(d0)); r1 |> resize(int(d1)); r2 |> resize(int(d2))
        with_job_que() {
            matmul_q8q8_group3(y0, y1, y2, qall.q, qall.scales, 0l, d0 * n, (d0 + d1) * n, xq, xs, n, d0, d1, d2)
            matmul_variants::matmul_q8q8(r0, q0.q, q0.scales, xq, xs, n, d0)
            matmul_variants::matmul_q8q8(r1, q1.q, q1.scales, xq, xs, n, d1)
            matmul_variants::matmul_q8q8(r2, q2.q, q2.scales, xq, xs, n, d2)
        }
        var maxerr = 0.0
        for (i in range(int(d0))) {
            maxerr = max(maxerr, abs(y0[i] - r0[i]))
        }
        for (i in range(int(d1))) {
            maxerr = max(maxerr, abs(y1[i] - r1[i]))
        }
        for (i in range(int(d2))) {
            maxerr = max(maxerr, abs(y2[i] - r2[i]))
        }
        t |> success(maxerr < 1.0e-3, "group3 outputs match per-group matmul_q8q8")
    }
}
