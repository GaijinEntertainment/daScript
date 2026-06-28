options gen2
options persistent_heap

require dastest/testing_boost
require matmul_variants
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
    var w : array<float>
    var x : array<float>
    var y : array<float>
    var yref : array<float>
    fill(w, 32000 * 288)
    fill(x, 288)
    y |> resize(32000)
    yref |> resize(32000)
    matmul_llvm_vec(yref, w, x, 288l, 32000l)

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
    }
}
