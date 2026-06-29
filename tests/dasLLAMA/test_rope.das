options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require math

[test]
def test_rope(t : T?) {
    // Position 0 -> angle 0 -> identity (cos 0 = 1, sin 0 = 0): nothing moves.
    t |> run("pos 0 is identity") @(t : T?) {
        var vec <- [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
        rope(vec, 0l, 4l, 6l)
        let orig <- [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
        for (i in range(6)) {
            t |> success(abs(vec[i] - orig[i]) < 1e-6)
        }
    }
    // A rotation preserves the length of each pair: v0^2 + v1^2 is unchanged.
    t |> run("preserves pair norm") @(t : T?) {
        var vec <- [1.0, 2.0, 3.0, 4.0]
        rope(vec, 7l, 4l, 4l)
        t |> success(abs((vec[0] * vec[0] + vec[1] * vec[1]) - 5.0) < 1e-4)   // 1^2+2^2
        t |> success(abs((vec[2] * vec[2] + vec[3] * vec[3]) - 25.0) < 1e-4)  // 3^2+4^2
    }
    // head_size 2 -> freq 1, so the single pair rotates by exactly `pos` radians.
    // [1,0] at pos 1 -> [cos 1, sin 1].
    t |> run("known rotation") @(t : T?) {
        var vec <- [1.0, 0.0]
        rope(vec, 1l, 2l, 2l)
        t |> success(abs(vec[0] - 0.5403023) < 1e-5)
        t |> success(abs(vec[1] - 0.8414710) < 1e-5)
    }
    // The defining property: Q.K depends only on the position OFFSET. Rotating a by m
    // and b by n, then dotting, equals rotating by 0 and (n-m). Here 5-3 == 2-0 == 2.
    t |> run("relative position invariance") @(t : T?) {
        var a1 <- [1.0, 2.0, 3.0, 4.0]
        var b1 <- [4.0, 3.0, 2.0, 1.0]
        rope(a1, 3l, 4l, 4l)
        rope(b1, 5l, 4l, 4l)
        var dot1 = 0.0
        for (i in range(4)) {
            dot1 += a1[i] * b1[i]
        }
        var a2 <- [1.0, 2.0, 3.0, 4.0]
        var b2 <- [4.0, 3.0, 2.0, 1.0]
        rope(a2, 0l, 4l, 4l)
        rope(b2, 2l, 4l, 4l)
        var dot2 = 0.0
        for (i in range(4)) {
            dot2 += a2[i] * b2[i]
        }
        t |> success(abs(dot1 - dot2) < 1e-3)
    }
}
