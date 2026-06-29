options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require math

[test]
def test_rmsnorm(t : T?) {
    // A constant vector normalizes to ~1, whatever its magnitude — RMSNorm removes scale.
    t |> run("constant vector -> ones") @(t : T?) {
        var o <- [0.0, 0.0, 0.0]
        let x <- [7.0, 7.0, 7.0]
        let w <- [1.0, 1.0, 1.0]
        rmsnorm(o, x, w, 3l)
        for (j in range(3)) {
            t |> success(abs(o[j] - 1.0) < 1e-4)
        }
    }
    // Defining property: with unit weights, mean(o^2) == 1, so sum(o^2) == size.
    t |> run("unit mean-square") @(t : T?) {
        var o <- [0.0, 0.0, 0.0, 0.0]
        let x <- [1.0, 2.0, 3.0, 4.0]
        let w <- [1.0, 1.0, 1.0, 1.0]
        rmsnorm(o, x, w, 4l)
        var ss = 0.0
        for (j in range(4)) {
            ss += o[j] * o[j]
        }
        t |> success(abs(ss - 4.0) < 1e-3)
    }
    // A normalized input (rms ~ 1) passes the learned weight straight through.
    t |> run("weight passthrough") @(t : T?) {
        var o <- [0.0, 0.0, 0.0, 0.0]
        let x <- [1.0, 1.0, 1.0, 1.0]
        let w <- [2.0, 3.0, 4.0, 5.0]
        rmsnorm(o, x, w, 4l)
        for (j in range(4)) {
            t |> success(abs(o[j] - w[j]) < 1e-3)
        }
    }
}
