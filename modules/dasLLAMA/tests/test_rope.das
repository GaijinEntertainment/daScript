options gen2
options _dasllama_internal = true

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

def private imrope_axis_ref(j, sx, sy, sz, sw : int) : int {
    let sector = j % (sx + sy + sz + sw)
    if (sector % 3 == 1 && sector < 3 * sy) {
        return 1
    } elif (sector % 3 == 2 && sector < 3 * sz) {
        return 2
    } elif (sector % 3 == 0 && sector < 3 * sx) {
        return 0
    }
    return 3
}

[test]
def test_rope_gen_imrope_collapse(t : T?) {
    let qsec = int4(24, 20, 20, 0)
    // Text rows: all four sections carry the same position, so every dim reads it and the
    // table must be BIT-identical to the plain rows builder — the collapse that lets text
    // decoding stay on the untouched path.
    t |> run("all-equal positions collapse bit-exactly to the rows builder") @(t : T?) {
        var ff : array<float>
        var p4 <- [int4(7, 7, 7, 7), int4(1234, 1234, 1234, 1234), int4(0, 0, 0, 0)]
        var pl <- [7l, 1234l, 0l]
        var c1 : array<float>
        var s1 : array<float>
        var c2 : array<float>
        var s2 : array<float>
        build_rope_tabs_imrope(c1, s1, 1000000.0, 1.0, 1.0, ff, false, p4, qsec, 3l, 128l)
        build_rope_tabs_rows(c2, s2, 1000000.0, 1.0, 1.0, ff, false, pl, 3l, 128l)
        var same = true
        for (ca, cb, sa, sb in c1, c2, s1, s2) {
            same &&= ca == cb && sa == sb
        }
        t |> success(same, "the (p,p,p,p) collapse must be bit-exact")
        delete p4
        delete pl
        delete c1
        delete s1
        delete c2
        delete s2
    }
}

[test]
def test_rope_gen_imrope_reference(t : T?) {
    let qsec = int4(24, 20, 20, 0)
    // The sector walk, re-derived per element in a different loop order: every dim of
    // an image-shaped row must read the axis its section assigns it, including
    // the [24,20,20,0] tail (60->t, 61->e, 62->e, 63->t).
    t |> run("image row matches a per-element ggml-order reference") @(t : T?) {
        var ff : array<float>
        var p4 <- [int4(100, 103, 117, 0)]
        var ct : array<float>
        var st : array<float>
        build_rope_tabs_imrope(ct, st, 1000000.0, 1.0, 1.0, ff, false, p4, qsec, 1l, 128l)
        var same = true
        for (j in range(64)) {
            let axis = imrope_axis_ref(j, 24, 20, 20, 0)
            let pos = p4[0][axis]
            let freq = 1.0 / pow(1000000.0, float(2 * j) / 128.0)
            let angle = float(pos) * 1.0 * freq
            // 1e-5, not bit-equal: under -jit the builder's cos and this recomputed one live in
            // different compilation units and drift by ulps; a wrong AXIS moves angles by O(1)
            same &&= abs(ct[j] - cos(angle)) < 1e-5 && abs(st[j] - sin(angle)) < 1e-5
        }
        t |> success(same, "every half-dim must read its IMROPE axis")
        t |> equal(imrope_axis_ref(60, 24, 20, 20, 0), 0)
        t |> equal(imrope_axis_ref(61, 24, 20, 20, 0), 3)
        t |> equal(imrope_axis_ref(62, 24, 20, 20, 0), 3)
        t |> equal(imrope_axis_ref(63, 24, 20, 20, 0), 0)
        t |> equal(ct[61], 1.0)
        t |> equal(st[61], 0.0)
        t |> equal(ct[62], 1.0)
        t |> equal(st[62], 0.0)
        delete p4
        delete ct
        delete st
    }
}

[test]
def test_rope_gen_imrope_shape(t : T?) {
    let qsec = int4(24, 20, 20, 0)
    // Perturbing ONE axis moves exactly that axis's dims: h (the grid row) owns the
    // j%3==1 half-dims below 60 and nothing else.
    t |> run("an h perturbation touches exactly the h dims") @(t : T?) {
        var ff : array<float>
        var pa <- [int4(50, 52, 57, 0)]
        var pb <- [int4(50, 53, 57, 0)]
        var ca : array<float>
        var sa : array<float>
        var cb : array<float>
        var sb : array<float>
        build_rope_tabs_imrope(ca, sa, 1000000.0, 1.0, 1.0, ff, false, pa, qsec, 1l, 128l)
        build_rope_tabs_imrope(cb, sb, 1000000.0, 1.0, 1.0, ff, false, pb, qsec, 1l, 128l)
        var shape_ok = true
        for (j in range(64)) {
            let moved = ca[j] != cb[j] || sa[j] != sb[j]
            let is_h = imrope_axis_ref(j, 24, 20, 20, 0) == 1
            if (is_h) {
                shape_ok &&= moved
            } else {
                shape_ok &&= !moved
            }
        }
        t |> success(shape_ok, "only the h-axis dims may move when h moves")
        delete pa
        delete pb
        delete ca
        delete sa
        delete cb
        delete sb
    }
}

// the NON-interleaved walk: sections read as contiguous ranges
def private mrope_axis_ref(j, sx, sy, sz : int) : int {
    if (j >= sx + sy + sz) {
        return 3
    } elif (j >= sx + sy) {
        return 2
    } elif (j >= sx) {
        return 1
    }
    return 0
}

[test]
def test_rope_gen_mrope_noninterleaved(t : T?) {
    let qsec = int4(16, 24, 24, 0)   // the qwen2vl decoder's sections
    t |> run("the contiguous-section walk: collapse, reference, and the imrope divergence") @(t : T?) {
        var ff : array<float>
        // text rows collapse bit-exactly to the rows builder, like the interleaved form
        var p4 <- [int4(7, 7, 7, 7), int4(1234, 1234, 1234, 1234)]
        var pl <- [7l, 1234l]
        var c1 : array<float>
        var s1 : array<float>
        var c2 : array<float>
        var s2 : array<float>
        build_rope_tabs_mrope(c1, s1, 1000000.0, 1.0, 1.0, ff, false, p4, qsec, 2l, 128l)
        build_rope_tabs_rows(c2, s2, 1000000.0, 1.0, 1.0, ff, false, pl, 2l, 128l)
        var same = true
        for (ca, cb, sa, sb in c1, c2, s1, s2) {
            same &&= ca == cb && sa == sb
        }
        t |> success(same, "the (p,p,p,p) collapse must be bit-exact")
        // an image-shaped row reads each axis over its CONTIGUOUS range, frequencies continuous
        var pi4 <- [int4(100, 103, 117, 0)]
        var ct : array<float>
        var st : array<float>
        build_rope_tabs_mrope(ct, st, 1000000.0, 1.0, 1.0, ff, false, pi4, qsec, 1l, 128l)
        var ref_ok = true
        for (j in range(64)) {
            let axis = mrope_axis_ref(j, 16, 24, 24)
            let pos = pi4[0][axis]
            let freq = 1.0 / pow(1000000.0, float(2 * j) / 128.0)
            let angle = float(pos) * freq
            // 1e-5, not bit-equal: cross-compilation-unit cos drifts by ulps under -jit
            ref_ok &&= abs(ct[j] - cos(angle)) < 1e-5 && abs(st[j] - sin(angle)) < 1e-5
        }
        t |> success(ref_ok, "every half-dim must read its contiguous-section axis")
        // the two walks must genuinely differ on grid rows (same sections both ways)
        var ci : array<float>
        var si : array<float>
        build_rope_tabs_imrope(ci, si, 1000000.0, 1.0, 1.0, ff, false, pi4, qsec, 1l, 128l)
        var differs = false
        for (j in range(64)) {
            differs ||= ct[j] != ci[j] || st[j] != si[j]
        }
        t |> success(differs, "interleaved and contiguous walks differ on an image row")
        delete p4
        delete pl
        delete pi4
        delete c1
        delete s1
        delete c2
        delete s2
        delete ct
        delete st
        delete ci
        delete si
    }
}

[test]
def test_mrope_span_positions(t : T?) {
    // the decoder walk: head sequential, span (pos0, pos0+i/nx, pos0+i%nx, 0), tail from
    // pos0 + max(nx, ny) — the grid's long side, not its row count
    t |> run("the quantum map follows the mtmd walk") @(t : T?) {
        var map : array<int4>
        mrope_span_positions(map, 10l, 12l, 3l, 9l, 3l, 2l)
        t |> equal(map[0], int4(10, 10, 10, 10))
        t |> equal(map[2], int4(12, 12, 12, 12))
        t |> equal(map[3], int4(13, 13, 13, 0))
        t |> equal(map[4], int4(13, 13, 14, 0))
        t |> equal(map[5], int4(13, 13, 15, 0))
        t |> equal(map[6], int4(13, 14, 13, 0))
        t |> equal(map[8], int4(13, 14, 15, 0))
        // tail resumes at pos0 + max(3, 2) = 16, NOT pos0 + 6 rows
        t |> equal(map[9], int4(16, 16, 16, 16))
        t |> equal(map[11], int4(18, 18, 18, 18))
        delete map
    }
    t |> run("a grid that does not match the span row count panics") @(t : T?) {
        var tripped = false
        try {
            var map : array<int4>
            mrope_span_positions(map, 0l, 8l, 2l, 6l, 3l, 2l)
            delete map
        } recover {
            tripped = true
        }
        t |> success(tripped, "4 span rows vs a 3x2 grid must refuse")
    }
}

[test]
def test_rope_gen_vision(t : T?) {
    // The qwen3vl ViT table: hs/2 slots per row — the h ladder then the w ladder restarted,
    // freq divisor hs/2 (VISION mode: sections hs/4 x4, each axis independent).
    t |> run("vision table matches the per-element ladder reference") @(t : T?) {
        var p2 <- [int2(3, 11), int2(0, 0)]
        var ct : array<float>
        var st : array<float>
        build_rope_tabs_vision(ct, st, 10000.0, p2, 2l, 72l)
        var same = true
        for (j in range(36)) {
            let jj = j % 18
            let freq = 1.0 / pow(10000.0, float(2 * jj) / 36.0)
            let a0 = float(j < 18 ? 3 : 11) * freq
            // 1e-5, not bit-equal: cross-compilation-unit cos drifts by ulps under -jit
            same &&= abs(ct[j] - cos(a0)) < 1e-5 && abs(st[j] - sin(a0)) < 1e-5
            same &&= ct[36 + j] == 1.0 && st[36 + j] == 0.0
        }
        t |> success(same, "h slots then a restarted w ladder; (0,0) is the identity row")
        delete p2
        delete ct
        delete st
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
