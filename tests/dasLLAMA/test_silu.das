options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require math

[test]
def test_silu(t : T?) {
    // Exact + hand-computed shape: silu(0)=0, silu(1)=0.7310586, silu(2)=1.7615942,
    // silu(-1)=-0.2689414 (negative — SiLU is non-monotonic, dips below zero).
    t |> run("known values") @(t : T?) {
        var x <- [0.0, 1.0, 2.0, -1.0]
        silu(x, 4l)
        t |> success(abs(x[0]) < 1e-6)              // 0 * sigmoid(0) = 0 exactly
        t |> success(abs(x[1] - 0.7310586) < 1e-4)
        t |> success(abs(x[2] - 1.7615942) < 1e-4)
        t |> success(abs(x[3] + 0.2689414) < 1e-4)
    }
    // Saturation: sigmoid -> 1 so silu(x) -> x for large +x; sigmoid -> 0 so silu(x) -> 0 for large -x.
    t |> run("saturation") @(t : T?) {
        var x <- [20.0, -20.0]
        silu(x, 2l)
        t |> success(abs(x[0] - 20.0) < 1e-4)
        t |> success(abs(x[1]) < 1e-4)
    }
}
