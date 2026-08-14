options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_rope   // the angle/table generation under test
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

// ===== dasllama_rope: the angle/table generation =====

def private ff_none() : array<float> {
    var e : array<float>
    return <- e
}

[test]
def test_rope_gen_pos0(t : T?) {
    t |> run("pos 0: cos row = mscale exactly, sin row = 0, both layouts") @(t : T?) {
        var ct : array<float>
        var st : array<float>
        var ff <- ff_none()
        build_rope_tabs(ct, st, 10000.0, 1.0, 1.5, ff, true, 0l, 1l, 8l)
        var packed : array<float>
        packed |> resize(8)
        build_rope_row_packed(packed, 0l, 0l, 8l, 10000.0, 1.0, 1.5, ff)
        for (j in range(4)) {
            t |> equal(ct[j], 1.5)
            t |> equal(st[j], 0.0)
            t |> equal(packed[j], 1.5)
            t |> equal(packed[4 + j], 0.0)
        }
        delete ct
        delete st
        delete packed
        delete ff
    }
}

[test]
def test_rope_gen_ff_ones(t : T?) {
    t |> run("all-ones rope_freqs is bit-identical to no rope_freqs") @(t : T?) {
        var ones : array<float>
        ones |> resize(8)
        for (c in ones) {
            c = 1.0
        }
        var none <- ff_none()
        var c1 : array<float>
        var s1 : array<float>
        var c2 : array<float>
        var s2 : array<float>
        build_rope_tabs(c1, s1, 10000.0, 0.7, 1.0, ones, true, 5l, 3l, 16l)
        build_rope_tabs(c2, s2, 10000.0, 0.7, 1.0, none, true, 5l, 3l, 16l)
        var same = true
        for (ca, cb, sa, sb in c1, c2, s1, s2) {
            same &&= ca == cb && sa == sb
        }
        t |> success(same, "ff of ones must divide away exactly")
        delete ones
        delete none
        delete c1
        delete s1
        delete c2
        delete s2
    }
}

[test]
def test_rope_gen_cross_layout(t : T?) {
    t |> run("packed row == two-tab row at fscale=1 (the orders coincide there)") @(t : T?) {
        var ct : array<float>
        var st : array<float>
        var ff <- ff_none()
        let pos = 37l
        let hs = 32l
        build_rope_tabs(ct, st, 10000.0, 1.0, 1.0, ff, true, pos, 1l, hs)
        var packed : array<float>
        packed |> resize(int(hs))
        build_rope_row_packed(packed, 0l, pos, hs, 10000.0, 1.0, 1.0, ff)
        var same = true
        for (j in range(int(hs / 2l))) {
            same &&= packed[j] == ct[j] && packed[int(hs / 2l) + j] == st[j]
        }
        t |> success(same, "layouts must agree element-wise at fscale=1")
        delete ct
        delete st
        delete packed
        delete ff
    }
}

[test]
def test_rope_gen_rows_twin(t : T?) {
    t |> run("rows([p]) is bit-identical to tabs(start=p, npos=1)") @(t : T?) {
        var ff : array<float>
        ff |> resize(8)
        for (i in range(8)) {
            ff[i] = 1.0 + 0.25 * float(i)
        }
        var c1 : array<float>
        var s1 : array<float>
        var c2 : array<float>
        var s2 : array<float>
        var pos <- [1234l]
        build_rope_tabs_rows(c1, s1, 500000.0, 0.5, 0.8, ff, true, pos, 1l, 16l)
        build_rope_tabs(c2, s2, 500000.0, 0.5, 0.8, ff, true, 1234l, 1l, 16l)
        var same = true
        for (ca, cb, sa, sb in c1, c2, s1, s2) {
            same &&= ca == cb && sa == sb
        }
        t |> success(same, "the rows twin must match the table at the same position")
        delete ff
        delete c1
        delete s1
        delete c2
        delete s2
        delete pos
    }
}

[test]
def test_rope_gen_mscale(t : T?) {
    t |> run("mscale is a pure post-scale: mscale=2 row == 2 x mscale=1 row") @(t : T?) {
        var ff <- ff_none()
        var c1 : array<float>
        var s1 : array<float>
        var c2 : array<float>
        var s2 : array<float>
        build_rope_tabs(c1, s1, 10000.0, 1.3, 1.0, ff, true, 9l, 2l, 8l)
        build_rope_tabs(c2, s2, 10000.0, 1.3, 2.0, ff, true, 9l, 2l, 8l)
        var same = true
        for (ca, cb, sa, sb in c1, c2, s1, s2) {
            same &&= cb == 2.0 * ca && sb == 2.0 * sa
        }
        t |> success(same, "x2 by mscale must be exact")
        delete ff
        delete c1
        delete s1
        delete c2
        delete s2
    }
}
