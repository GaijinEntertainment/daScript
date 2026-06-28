options gen2

require dastest/testing_boost public
require dasllama_math
require matmul_variants
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
