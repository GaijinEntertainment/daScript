options gen2
options _dasllama_internal = true

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

// The greedy pick's two halves: hargmax locates the FIRST maximum (its 4-wide scan must not
// skip past it), hlse sums exp(x - mx) the same way. Both take the empty row as a no-op, which
// is what the vectorized forms' addr(x[0]) would fault on.
[test]
def test_hargmax_hlse(t : T?) {
    t |> run("hargmax on empty / single / ordinary rows") @(t : T?) {
        var e : array<float>
        t |> equal(hargmax(e), 0l, "empty row answers 0, no index fault")
        var one <- [ -7.5 ]
        t |> equal(hargmax(one), 0l, "the single element is the max")
        // 13 entries: past the 4-wide scan's third group, so the tail walk carries the answer
        var many <- [ 0.1, -2.0, 0.3, 0.25, -1.0, 0.9, 0.2, 0.15, -0.5, 0.05, 4.25, 0.0, 1.5 ]
        t |> equal(hargmax(many), 10l, "the max at index 10 (inside the tail of the 4-wide scan)")
        var first <- [ 3.0, 0.0, 1.0, 2.0, 0.5 ]
        t |> equal(hargmax(first), 0l, "a max in lane 0 of the first group")
    }
    t |> run("hargmax on ties takes the FIRST") @(t : T?) {
        var tied <- [ 1.0, 5.0, 2.0, 5.0, 5.0, 0.0 ]
        t |> equal(hargmax(tied), 1l, "three equal maxima, the lowest index wins")
        var tied_group <- [ 5.0, 5.0, 5.0, 5.0 ]
        t |> equal(hargmax(tied_group), 0l, "a whole group at the max still answers lane 0")
    }
    t |> run("hlse sums exp(x - mx)") @(t : T?) {
        var e : array<float>
        t |> success(hlse(e, 0.0) == 0.0lf, "empty row sums to 0, no index fault")
        var one <- [ 2.5 ]
        t |> success(abs(hlse(one, 2.5) - 1.0lf) < 1e-6lf, "x == mx contributes exactly 1")
        // 6 entries: one full 4-wide group plus a 2-element tail, so both loops run
        var x <- [ 0.0, 1.0, 2.0, 3.0, -1.0, 0.5 ]
        var want = 0.0lf
        for (v in x) {
            want += double(exp(v - 3.0))
        }
        t |> success(abs(hlse(x, 3.0) - want) < 1e-6lf, "group + tail sum matches the scalar walk")
        var flat <- [ 1.0, 1.0, 1.0, 1.0, 1.0 ]
        t |> success(abs(hlse(flat, 1.0) - 5.0lf) < 1e-6lf, "five tied entries at the max sum to 5")
    }
}
