options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tower   // the shared encoder-tower pieces under test
require dasllama/dasllama_math    // setup_dasllama_jobque_
require dasllama/dasllama_rope    // build_rope_tabs_rows — the 2-axis rope's table source
require dasllama/dasllama_plane
require daslib/jobque_boost
require llvm/daslib/f16_cvt   // the binary16 round-trip the geglu table property checks
require math

// Model-free unit tests for the encoder-tower helpers both the audio (gemma4a) and vision
// (gemma4v) towers build on: each pins the shape against an in-test reference so a tower-side
// red localizes to the tower, not to a helper.

def private near(a, b, tol : float) : bool => abs(a - b) <= tol

[test]
def test_clamp_rows(t : T?) {
    t |> run("default Clamp is the inactive identity") @(t : T?) {
        let c = Clamp()
        t |> success(!c.active, "inactive")
        t |> success(c.imin < -1e38 && c.imax > 1e38 && c.omin < -1e38 && c.omax > 1e38, "+-FLT_MAX bounds")
    }
    t |> run("clamps into a separate destination and in place, across the 1024-element chunk seam") @(t : T?) {
        let n = 1500
        var src : array<float>
        src |> resize(n)
        for (i in range(n)) {
            src[i] = float(i - 750) * 0.01   // -7.5 .. 7.49
        }
        var dst : array<float>
        dst |> resize(n)
        clamp_rows(dst, src, int64(n), -2.0, 3.0)
        var bad = 0
        for (i in range(n)) {
            let want = clamp(src[i], -2.0, 3.0)
            if (dst[i] != want) {
                bad++
            }
        }
        t |> equal(bad, 0, "every element clamped (both chunks)")
        t |> success(dst[0] == -2.0 && dst[n - 1] == 3.0 && dst[750] == 0.0, "bounds hit, interior passes")
        clamp_rows(src, src, int64(n), -1.0, 1.0)
        t |> success(src[0] == -1.0 && src[n - 1] == 1.0 && src[760] == src[760], "in place over the same array")
        t |> success(near(src[760], 0.1, 1e-6), "interior untouched in place")
    }
}

[test]
def test_empty_inputs(t : T?) {
    t |> run("every row helper is a no-op on zero rows / zero elements") @(t : T?) {
        var a : array<float>
        var b : array<float>
        var w <- [1.0]
        var wp = PlaneF()
        unsafe {
            wp.p = addr(w[0])
            wp.n = 1l
        }
        clamp_rows(a, b, 0l, -1.0, 1.0)
        rms_rows(a, b, wp, 0l, 4l, 0l, 1e-6)
        rms_rows_weightless(a, 4l, 0l, 1e-6)
        geglu_quick_lut_rows(a, b, b, 4l, 0l)
        rope_neox_2d_rows(a, 0l, 2l, 8l, b, b, b, b)
        t |> equal(int(avg_pool2d_rows(a, b, 2l, 2l, 4l, 3l)), 0, "a grid smaller than the kernel pools to zero rows")
        t |> success(empty(a), "nothing was grown or written")
        wp.p = null
    }
}

[test]
def test_rms_rows(t : T?) {
    t |> run("weighted rms per row against the formula; heads = rows of head width") @(t : T?) {
        // two tokens of two 4-wide heads = four rows of 4 under the per-head form
        var x <- [1.0, 2.0, 3.0, 4.0, -1.0, 0.5, 2.0, -2.0, 3.0, 3.0, 3.0, 3.0, 0.0, 0.0, 1.0, 0.0]
        var w <- [2.0, 1.0, 0.5, -1.0]
        var wp = PlaneF()
        unsafe {
            wp.p = addr(w[0])
            wp.n = 4l
        }
        var out : array<float>
        out |> resize(16)
        rms_rows(out, x, wp, 0l, 4l, 4l, 1e-6)
        var maxdiff = 0.0
        for (r in range(4)) {
            var ss = 0.0
            for (i in range(4)) {
                ss += x[r * 4 + i] * x[r * 4 + i]
            }
            let scale = 1.0 / sqrt(ss / 4.0 + 1e-6)
            for (i in range(4)) {
                maxdiff = max(maxdiff, abs(out[r * 4 + i] - w[i] * (x[r * 4 + i] * scale)))
            }
        }
        t |> success(maxdiff < 1e-6, "rms_rows maxdiff {maxdiff}")
        rms_rows(x, x, wp, 0l, 4l, 4l, 1e-6)
        var inplace_diff = 0.0
        for (i in range(16)) {
            inplace_diff = max(inplace_diff, abs(x[i] - out[i]))
        }
        t |> success(inplace_diff == 0.0, "in-place form is bit-identical")
        wp.p = null
    }
    t |> run("weightless rms leaves every row at unit rms") @(t : T?) {
        var x <- [3.0, 4.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, -2.0, 2.0, -2.0, 2.0]
        rms_rows_weightless(x, 4l, 3l, 0.0)
        for (r in range(3)) {
            var ss = 0.0
            for (i in range(4)) {
                ss += x[r * 4 + i] * x[r * 4 + i]
            }
            t |> success(near(ss / 4.0, 1.0, 1e-6), "row {r} mean square {ss / 4.0}")
        }
        t |> success(near(x[0], 3.0 / 2.5, 1e-6) && near(x[1], 4.0 / 2.5, 1e-6), "row 0 = x / rms(x)")
    }
}

[test]
def test_geglu_quick_lut_rows(t : T?) {
    t |> run("gelu_quick(gate)*up through ggml-cpu's f16 table") @(t : T?) {
        var gate <- [0.0, 1.0, -1.0, 2.5, -3.0, 0.1, 7.0, -0.25]
        var up <- [1.0, 2.0, 3.0, -1.0, 0.5, 4.0, 1.0, 2.0]
        var h : array<float>
        h |> resize(8)
        geglu_quick_lut_rows(h, gate, up, 4l, 2l)
        var maxrel = 0.0
        for (i in range(8)) {
            let g = gate[i]
            let exact = g * (1.0 / (1.0 + exp(-1.702 * g))) * up[i]
            // two binary16 round-trips: ≤ 2 × 2^-11 relative, plus the f16-rounded input's slope term
            let tol = abs(exact) * 2e-3 + 1e-4
            maxrel = max(maxrel, abs(h[i] - exact) - tol)
        }
        t |> success(maxrel <= 0.0, "within the f16 envelope of the exact form (worst excess {maxrel})")
        t |> success(h[0] == 0.0, "gelu_quick(0) = 0")
        // the table property: an input and its binary16 rounding produce the SAME output — the
        // exact formula would not (0.1 is not representable in f16)
        var gate2 <- [0.1, f16_to_f32(f32_to_f16(0.1))]
        var up2 <- [1.0, 1.0]
        var h2 : array<float>
        h2 |> resize(2)
        geglu_quick_lut_rows(h2, gate2, up2, 2l, 1l)
        t |> success(h2[0] == h2[1], "f16 input rounding is applied ({h2[0]} vs {h2[1]})")
        t |> success(f16_to_f32(f32_to_f16(h2[0])) == h2[0], "output is an f16 value")
    }
}

[test]
def test_im2col_rgb_patches(t : T?) {
    t |> run("[C][KH][KW] rows over row-major patches of an interleaved canvas") @(t : T?) {
        // 4x2 canvas, p = 2 -> 2 patches side by side; pixel value encodes (x, y, c)
        let w = 4l
        let h = 2l
        var planes : array<float>
        planes |> resize(w * h * 3l)
        for (y in range(2)) {
            for (x in range(4)) {
                for (c in range(3)) {
                    planes[(y * 4 + x) * 3 + c] = float(x * 100 + y * 10 + c)
                }
            }
        }
        var x0 : array<float>
        let npos = im2col_rgb_patches(planes, w, h, 2l, x0)
        t |> equal(int(npos), 2, "two patches")
        t |> equal(length(x0), 2 * 12, "rows of 3*p^2 = 12")
        // row offset = c*p*p + ky*p + kx; pixel value = x*100 + y*10 + c
        t |> success(x0[12 + 2 * 4 + 1 * 2 + 0] == 212.0, "patch 1 c2 ky1 kx0 = pixel (2,1,2)")
        t |> success(x0[0 * 4 + 0 * 2 + 1] == 100.0, "patch 0 c0 ky0 kx1 = pixel (1,0,0)")
        t |> success(x0[1 * 4 + 1 * 2 + 1] == 111.0, "patch 0 c1 ky1 kx1 = pixel (1,1,1)")
    }
}

[test]
def test_rope_neox_2d_rows(t : T?) {
    t |> run("first half rotates by pos_x, second half by pos_y (NEOX pairs, theta 100)") @(t : T?) {
        let npos = 2l
        let n_head = 2l
        let hs = 8l   // half = 4 per axis, pairs (j, j+2) within each half
        let d = n_head * hs
        var pos_x <- [1l, 3l]
        var pos_y <- [2l, 0l]
        var cx, sx, cy, sy : array<float>
        var no_ff : array<float>
        build_rope_tabs_rows(cx, sx, 100.0, 1.0, 1.0, no_ff, false, pos_x, npos, hs / 2l)
        build_rope_tabs_rows(cy, sy, 100.0, 1.0, 1.0, no_ff, false, pos_y, npos, hs / 2l)
        var v : array<float>
        v |> resize(npos * d)
        for (i in range(int(npos * d))) {
            v[i] = float(i % 7) - 3.0
        }
        let ref := v
        rope_neox_2d_rows(v, npos, n_head, hs, cx, sx, cy, sy)
        var maxdiff = 0.0
        for (r in range64(npos)) {
            for (hh in range64(n_head)) {
                let base = int(r * d + hh * hs)
                for (axis in range(2)) {
                    let pos = axis == 0 ? pos_x[r] : pos_y[r]
                    let off = base + axis * 4
                    for (j in range(2)) {
                        let freq = 1.0 / pow(100.0, float(2 * j) / 4.0)
                        let ang = float(pos) * freq
                        let v0 = ref[off + j]
                        let v1 = ref[off + j + 2]
                        let want0 = v0 * cos(ang) - v1 * sin(ang)
                        let want1 = v0 * sin(ang) + v1 * cos(ang)
                        maxdiff = max(maxdiff, abs(v[off + j] - want0))
                        maxdiff = max(maxdiff, abs(v[off + j + 2] - want1))
                    }
                }
            }
        }
        t |> success(maxdiff < 1e-5, "2-axis NEOX maxdiff {maxdiff}")
        t |> success(v[int(d + 4)] == ref[int(d + 4)] && v[int(d + 12)] == ref[int(d + 12)], "row 1 y-halves identity at pos 0")
        t |> success(v[int(d)] != ref[int(d)], "row 1 x-half rotated at pos 3")
    }
}

[test]
def test_avg_pool2d_rows(t : T?) {
    t |> run("3x3 stride-3 mean over a token-major grid, ragged edge dropped") @(t : T?) {
        // 7x3 grid (one ragged column), d = 2: channel 0 = gx, channel 1 = gy
        let gw = 7l
        let gh = 3l
        var x : array<float>
        x |> resize(gw * gh * 2l)
        for (gy in range(3)) {
            for (gx in range(7)) {
                x[(gy * 7 + gx) * 2] = float(gx)
                x[(gy * 7 + gx) * 2 + 1] = float(gy)
            }
        }
        var out : array<float>
        let n = avg_pool2d_rows(out, x, gw, gh, 2l, 3l)
        t |> equal(int(n), 2, "two windows (column 6 dropped)")
        t |> success(near(out[0], 1.0, 1e-6) && near(out[1], 1.0, 1e-6), "window 0 mean (gx 0..2, gy 0..2) = (1, 1): ({out[0]}, {out[1]})")
        t |> success(near(out[2], 4.0, 1e-6) && near(out[3], 1.0, 1e-6), "window 1 mean (gx 3..5) = (4, 1): ({out[2]}, {out[3]})")
    }
}

// the oracle stays naive on purpose: no blocking, no packing - nothing the kernel does
def private attention_ref(qq, kk, vv : array<float>; tt, d, n_head : int64; scale : float) : array<float> {
    let hs = d / n_head
    var out : array<float>
    out |> resize(tt * d)
    var sc : array<double>
    sc |> resize(tt)
    for (h in range64(n_head)) {
        for (p in range64(tt)) {
            var mx = -1e300lf
            for (q in range64(tt)) {
                var s = 0.0lf
                for (j in range64(hs)) {
                    s += double(qq[p * d + h * hs + j]) * double(kk[q * d + h * hs + j])
                }
                sc[q] = s * double(scale)
                mx = max(mx, sc[q])
            }
            var sum = 0.0lf
            for (q in range64(tt)) {
                sc[q] = exp(sc[q] - mx)
                sum += sc[q]
            }
            for (j in range64(hs)) {
                var acc = 0.0lf
                for (q in range64(tt)) {
                    acc += sc[q] / sum * double(vv[q * d + h * hs + j])
                }
                out[p * d + h * hs + j] = float(acc)
            }
        }
    }
    return <- out
}

[test]
def test_attention_bidir(t : T?) {
    t |> run("matches the naive reference at two scales; the scale is applied") @(t : T?) {
        let tt = 5l
        let d = 8l
        let n_head = 2l
        var qq, kk, vv : array<float>
        qq |> resize(tt * d)
        kk |> resize(tt * d)
        vv |> resize(tt * d)
        for (i in range(int(tt * d))) {
            qq[i] = float((i * 7) % 11) * 0.3 - 1.5
            kk[i] = float((i * 5) % 13) * 0.25 - 1.5
            vv[i] = float((i * 3) % 17) * 0.1 - 0.8
        }
        var out : array<float>
        out |> resize(tt * d)
        var hp, att : array<float>
        with_job_que() {
            setup_dasllama_jobque_()
            for (scale in [0.5, 1.0]) {
                attention_bidir(out, qq, kk, vv, hp, att, tt, d, n_head, scale)
                let ref <- attention_ref(qq, kk, vv, tt, d, n_head, scale)
                var maxdiff = 0.0
                for (i in range(int(tt * d))) {
                    maxdiff = max(maxdiff, abs(out[i] - ref[i]))
                }
                t |> success(maxdiff < 1e-5, "scale {scale}: maxdiff {maxdiff} vs the naive reference")
            }
        }
        t |> success(length(hp) >= int(4l * tt * d), "hp scratch grown by the core")
    }
}
