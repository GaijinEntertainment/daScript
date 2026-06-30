options gen2
options persistent_heap

require dastest/testing_boost
require matmul_variants
require dasllama/dasllama_quant
require daslib/jobque_boost
require math

def fill(var a : array<float>; count : int) {
    a |> resize(count)
    for (i in range(count)) {
        a[i] = float(i % 7) * 0.1
    }
}

// Threaded-matmul num_jobs sweep at the classifier shape (32000 x 288), the heaviest matmul.
// "each dot" = num_jobs = d; "each 10 dots" = d/10; N = hw worker count.
[benchmark]
def bench_matmul_par(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    var w : array<float>
    var x : array<float>
    var y : array<float>
    var yref : array<float>
    fill(w, 32000 * 288)
    fill(x, 288)
    y |> resize(32000)
    yref |> resize(32000)
    matmul_llvm_vec(yref, w, x, 288l, 32000l)

    // Q8 + Q4 weights for the threaded fp32-vs-quantized comparison (the production-relevant one).
    let qt <- quantize_q8_0(w, 288l * 32000l)
    let qt4 <- quantize_q4_0(w, 288l * 32000l)

    // small shapes, to find the threshold below which threading isn't worth it
    var wp : array<float>
    var yp : array<float>
    var wf : array<float>
    var yf : array<float>
    fill(wp, 288 * 288)
    yp |> resize(288)
    fill(wf, 768 * 288)
    yf |> resize(768)

    with_job_que() {
        let N = get_total_hw_jobs()
        // threshold sweep: single vs par-N at proj (288), ffn (768), classifier (32000)
        b |> run("proj 288   single") {
            matmul_llvm_vec(yp, wp, x, 288l, 288l)
        }
        b |> run("proj 288   par-N ") {
            matmul_par(yp, wp, x, 288l, 288l, N)
        }
        b |> run("ffn  768   single") {
            matmul_llvm_vec(yf, wf, x, 288l, 768l)
        }
        b |> run("ffn  768   par-N ") {
            matmul_par(yf, wf, x, 288l, 768l, N)
        }
        // correctness gate: threaded result must match single-thread
        matmul_par(y, w, x, 288l, 32000l, N)
        for (i in range(32000)) {
            if (abs(y[i] - yref[i]) > 1.0e-2) {
                b->failNow()
            }
        }
        b |> run("single (llvm)        ") {
            matmul_llvm_vec(y, w, x, 288l, 32000l)
        }
        b |> run("par 1N  ({N} jobs)   ") {
            matmul_par(y, w, x, 288l, 32000l, N)
        }
        b |> run("par 2N  ({N * 2} jobs)") {
            matmul_par(y, w, x, 288l, 32000l, N * 2)
        }
        b |> run("par 4N  ({N * 4} jobs)") {
            matmul_par(y, w, x, 288l, 32000l, N * 4)
        }
        b |> run("par 10N ({N * 10} jobs)") {
            matmul_par(y, w, x, 288l, 32000l, N * 10)
        }
        b |> run("par d/10 (3200 jobs) ") {
            matmul_par(y, w, x, 288l, 32000l, 3200)
        }
        b |> run("par each-dot (32000) ") {
            matmul_par(y, w, x, 288l, 32000l, 32000)
        }
        // Q8 threaded: gate against the fp32 reference, then fp32 vs Q8 head-to-head at par-N.
        var yq : array<float>
        yq |> resize(32000)
        matmul_q8_par(yq, qt.q, qt.scales, x, 288l, 32000l, N)
        for (i in range(32000)) {
            if (abs(yq[i] - yref[i]) > 1.0) {  // lossy-vs-original sanity gate; exact gate is in bench_matmul.das
                b->failNow()
            }
        }
        b |> run("fp32 par-N           ") {
            matmul_par(y, w, x, 288l, 32000l, N)
        }
        b |> run("q8   par-N           ") {
            matmul_q8_par(yq, qt.q, qt.scales, x, 288l, 32000l, N)
        }
        b |> run("q8   par-2N          ") {
            matmul_q8_par(yq, qt.q, qt.scales, x, 288l, 32000l, N * 2)
        }
        b |> run("q8   par-4N          ") {
            matmul_q8_par(yq, qt.q, qt.scales, x, 288l, 32000l, N * 4)
        }
        b |> run("q8f4 par-N           ") {
            matmul_q8f4_par(yq, qt.q, qt.scales, x, 288l, 32000l, N)
        }
        b |> run("q8f4 par-2N          ") {
            matmul_q8f4_par(yq, qt.q, qt.scales, x, 288l, 32000l, N * 2)
        }
        b |> run("q8f4 par-4N          ") {
            matmul_q8f4_par(yq, qt.q, qt.scales, x, 288l, 32000l, N * 4)
        }
        b |> run("q8flat par-N         ") {
            matmul_q8flat_par(yq, qt.q, qt.scales, x, 288l, 32000l, N)
        }
        b |> run("q8flat par-2N        ") {
            matmul_q8flat_par(yq, qt.q, qt.scales, x, 288l, 32000l, N * 2)
        }
        b |> run("q8flat par-4N        ") {
            matmul_q8flat_par(yq, qt.q, qt.scales, x, 288l, 32000l, N * 4)
        }
        // Q4 threaded: gate against the fp32 reference (lossy sanity), then sweep num_jobs.
        matmul_q4_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N)
        for (i in range(32000)) {
            if (abs(yq[i] - yref[i]) > 1.0) {
                b->failNow()
            }
        }
        b |> run("q4   par-N           ") {
            matmul_q4_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N)
        }
        b |> run("q4   par-2N          ") {
            matmul_q4_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N * 2)
        }
        b |> run("q4   par-4N          ") {
            matmul_q4_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N * 4)
        }
        b |> run("q4fv par-N           ") {
            matmul_q4fv_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N)
        }
        b |> run("q4fv par-2N          ") {
            matmul_q4fv_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N * 2)
        }
        b |> run("q4fv par-4N          ") {
            matmul_q4fv_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N * 4)
        }
        b |> run("q4fv-w4u4 par-N      ") {
            matmul_q4fvw4u4_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N)
        }
        b |> run("q4fv-w4u4 par-2N     ") {
            matmul_q4fvw4u4_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N * 2)
        }
        b |> run("q4fv-w4u4 par-4N     ") {
            matmul_q4fvw4u4_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N * 4)
        }
        b |> run("q4flat par-N         ") {
            matmul_q4flat_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N)
        }
        b |> run("q4flat par-2N        ") {
            matmul_q4flat_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N * 2)
        }
        b |> run("q4flat par-4N        ") {
            matmul_q4flat_par(yq, qt4.q, qt4.scales, x, 288l, 32000l, N * 4)
        }
    }
}
