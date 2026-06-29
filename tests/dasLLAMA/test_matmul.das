options gen2

require dastest/testing_boost public
require dasllama/dasllama_math

[test]
def test_matmul(t : T?) {
    // W = [[1,2,3],[4,5,6]] (d=2, n=3). Small integers are exact in fp32, so == is safe.
    t |> run("2x3, x = ones") @(t : T?) {
        let w <- [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
        let x <- [1.0, 1.0, 1.0]
        var y <- [0.0, 0.0]
        matmul(y, w, x, 3l, 2l)
        t |> equal(y[0], 6.0)   // 1+2+3
        t |> equal(y[1], 15.0)  // 4+5+6
    }
    t |> run("2x3, x = distinct") @(t : T?) {
        let w <- [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
        let x <- [2.0, 0.0, 1.0]
        var y <- [0.0, 0.0]
        matmul(y, w, x, 3l, 2l)
        t |> equal(y[0], 5.0)   // 1*2 + 2*0 + 3*1
        t |> equal(y[1], 14.0)  // 4*2 + 5*0 + 6*1
    }
    // 1x4 row reduces to a single dot product.
    t |> run("1x4 dot product") @(t : T?) {
        let w <- [2.0, 3.0, 4.0, 5.0]
        let x <- [1.0, 10.0, 100.0, 1000.0]
        var y <- [0.0]
        matmul(y, w, x, 4l, 1l)
        t |> equal(y[0], 5432.0)  // 2 + 30 + 400 + 5000
    }
}
