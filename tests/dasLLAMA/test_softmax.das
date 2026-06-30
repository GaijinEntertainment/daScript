options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require math

[test]
def test_softmax(t : T?) {
    // Output is a probability distribution: positive, order-preserving, sums to 1.
    t |> run("distribution sums to 1") @(t : T?) {
        var x <- [1.0, 2.0, 3.0]
        softmax(x, 3l)
        var sum = 0.0
        for (i in range(3)) {
            sum += x[i]
        }
        t |> success(abs(sum - 1.0) < 1e-5)
        t |> success(x[0] < x[1] && x[1] < x[2])  // larger score -> larger probability
        t |> success(x[0] > 0.0 && x[2] < 1.0)
    }
    // Equal scores -> uniform distribution.
    t |> run("uniform input -> uniform output") @(t : T?) {
        var x <- [5.0, 5.0, 5.0, 5.0]
        softmax(x, 4l)
        for (i in range(4)) {
            t |> success(abs(x[i] - 0.25) < 1e-6)
        }
    }
    // Max-subtraction is overflow-safe and shift-invariant: exp(1000) would be +inf,
    // but softmax([1000,1001,1002]) must equal softmax([0,1,2]).
    t |> run("numerical stability") @(t : T?) {
        var big <- [1000.0, 1001.0, 1002.0]
        var small <- [0.0, 1.0, 2.0]
        softmax(big, 3l)
        softmax(small, 3l)
        var sum = 0.0
        for (i in range(3)) {
            sum += big[i]
            t |> success(abs(big[i] - small[i]) < 1e-6)
        }
        t |> success(abs(sum - 1.0) < 1e-5)  // would be NaN without the max-subtract
    }
    // Regression: the array overload forwards via addr(x[0]), which index-faults on an empty
    // array; size <= 0 must be a safe no-op.
    t |> run("empty / zero size is a no-op") @(t : T?) {
        var e : array<float>
        softmax(e, 0l)
        var x <- [1.0, 2.0, 3.0]
        softmax(x, 0l)
        t |> success(abs(x[0] - 1.0) < 1e-6)   // untouched
        t |> success(true)                      // reached here ⇒ no index-fault
    }
}
