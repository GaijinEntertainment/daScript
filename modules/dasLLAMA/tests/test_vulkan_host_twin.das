options gen2
options persistent_heap
options _function_length = 0   // one flat cell per format family: the planar stack, the device planes, the two dots, the compare
options stack = 524288
options _dasllama_internal = true

// The host twin - the tier's own block decode run on the CPU over a streamed stack's device-layout planes - against the
// planar kernels, every expert format the tier streams, model-free and device-free: a planar disk-order stack in a Model
// shell, the engine's own gather into the device layout, the planar dispatcher's rows and the twin's rows within the
// summation bar, the poisoned expectation the control. The twin is the shared module's (`dasllama_gpu_host_twin`), registered by
// the home a build arms; a build with neither discrete home registers none and skips.

require dastest/testing_boost public
require math
require daslib/enum_trait
require dasllama/dasllama_common
require dasllama/dasllama_layout
require dasllama/dasllama_convert
require dasllama/dasllama_kqformat
require dasllama/dasllama_math_default
require dasllama/dasllama_math   // e8m0_half: the mx4 oracle's scale
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_math_gen
require ?vulkan dasllama/dasllama_math_vulkan   // its [init] registers the twin on a Vulkan build
require ?cuda dasllama/dasllama_math_cuda   // ... and on a CUDA build under DASLLAMA_CUDA
require _kq_fixtures
require _compares

let private ROWS = 8l
let private N_SB = 1280l   //! five superblocks: past the four-superblock trip of the row-major gemv's accumulator sets
let private NV4_SITE_SCALE = 1.7   //! the nv4 shell's per-tensor scale, off unity so a kernel that drops it misses the bar
let private N_B32 = 704l   //! 22 blocks, off the superblock lattice - the per-32 formats' reason

def private x_row(n : int64) : array<float> {
    var x : array<float>
    x |> resize(n)
    for (i in range64(n)) {
        x[i] = 1.3 * sin(float(i) * 0.19) + 0.2 * cos(float(i) * 0.057)
    }
    return <- x
}

//! the per-32 formats' hand-built disk blocks, the fixture file's builders
def private b32_block(f : KqFmt; blk : int) : array<uint8> {
    if (f == KqFmt.q51) {
        return <- build_q51_block(blk)
    }
    if (f == KqFmt.q5032) {
        return <- build_q50_block(blk)
    }
    return <- build_iq4nl32_block(blk)   //! iq4nl32 and q4032 share the block form (a q4032 block is iq4nl32's read under q - 8)
}

//! `rows` x `n` of format `f` in disk order on the Model shell `m`'s planes
def private put_planar(var m : Model; f : KqFmt; rows, n : int64) {
    var kq, ks : array<uint8>
    if (kq_sb(f)) {
        synth_kq_region(kq_schema_id(f), kq, ks, rows, n, 0x1234u, 0x85EBCA6Bu)
    } elif (f == KqFmt.mx4) {
        let nb = n / 32l
        kq |> resize(rows * nb * MX4_QB)
        ks |> resize(rows * nb * MX4_SB)
        var st = 0x9876u
        for (c in kq) {
            st = st * 1664525u + 1013904223u
            c = uint8(st >> 16u)
        }
        for (i in range(length(ks))) {   //! E8M0 exponents around 1.0
            ks[i] = uint8(125 + i % 6)
        }
    } elif (f == KqFmt.q8) {
        var wq : array<int8>
        var ws : array<float>
        fill_q8_quants(wq, ws, rows, n, 7l, false)
        m.qblob <- wq
        m.qscales <- ws
        return
    } else {
        let qb = kq_qsb(f)
        let sb = kq_ssb(f)
        let nb = n / 32l
        kq |> resize(rows * nb * qb)
        ks |> resize(rows * nb * sb)
        for (r in range64(rows)) {
            for (b in range64(nb)) {
                var inscope blkb <- b32_block(f, int(r * 31l + b))
                kq_transcode_superblock(f, blkb, 0l, kq, (r * nb + b) * qb, ks, (r * nb + b) * sb)
            }
        }
    }
    kq_plane_pair(m, f) $(var q : array<uint8>; var s : array<uint8>) {
        delete q
        delete s
        q <- kq
        s <- ks
    }
}

//! `rows` outputs holding the sentinel no kernel writes: a row a kernel skips reads as a miss, never as a zero inside the bar
def private sentinel_rows(rows : int64) : array<float> {
    var y : array<float>
    y |> resize(rows)
    for (v in y) {
        v = unsafe(reinterpret<float>(0x7FC00000u))
    }
    return <- y
}

//! a Model shell holding ROWS x `n` of format `f` in disk order, in the q8 serving mode the gathers read; an nv4 stack
//! carries a site scale off unity, so the device row's scale word and the planar epilogue both have to apply it
def private planar_shell(f : KqFmt; n : int64) : Model {
    var m = Model()
    m.quant = QuantMode.q8
    put_planar(m, f, ROWS, n)
    if (f == KqFmt.nv4) {
        m.site_scale[site_scale_key(f, 0l)] = NV4_SITE_SCALE
    }
    return <- m
}

//! the bar every cell here holds: the control moves a tenth of a percent, ten bars, whatever the row's magnitude
def private twin_bar(want : array<float>) : Bar => Bar(rel = 1e-4, abs_floor = 1e-5 * logits_maxabs(want), poison_rel = 1e-3)

//! the activation as the kernels read it, widened back to floats: Q8_K a scale a superblock under a superblock format, Q8_0 a scale a 32-block otherwise
def private act_floats(f : KqFmt; x : array<float>; n : int64) : array<float> {
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(n)
    let blk = kq_sb(f) ? 256l : 32l
    xs |> resize(n / blk)
    if (kq_sb(f)) {
        xbs |> resize(n / 16l)
        quantize_q8_k_into(x, n, xq, xs, xbs, 0l, 0l, 0l)
    } else {
        quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    }
    return <- [for (i in range64(n)); float(xq[i]) * xs[i / blk]]
}

//! the plain oracle: each row's planar planes widened to floats (the engine's dequantizer a block; the nibble table under the
//! halved E8M0 scale for mx4; the int8 under its scale for q8) and dotted with the widened activation in plain float
def private ref_rows(m : Model; f : KqFmt; x : array<float>; n, rows : int64) : array<float> {
    var inscope xa <- act_floats(f, x, n)
    var y <- sentinel_rows(rows)
    var w : array<float>
    w |> resize(n)
    let nb = n / 32l
    for (r in range64(rows)) {
        if (f == KqFmt.q8) {
            for (i in range64(n)) {
                w[i] = float(m.qblob[r * n + i]) * m.qscales[(r * n + i) / 32l]
            }
        } elif (f == KqFmt.mx4) {
            let lut = mx4_lut()
            for (bi in range64(nb)) {
                let sc = e8m0_half(uint(m.mxs[(r * nb + bi) * MX4_SB]))
                for (j in range64(16l)) {
                    let b = int(m.mxq[(r * nb + bi) * MX4_QB + j])
                    w[bi * 32l + j] = float(lut[b & 15]) * sc
                    w[bi * 32l + j + 16l] = float(lut[b >> 4]) * sc
                }
            }
        } else {
            kq_plane_pair(m, f) $(q : array<uint8>; s : array<uint8>) {
                if (kq_sb(f)) {
                    let nsb = n / 256l
                    for (sb in range64(nsb)) {
                        dequant_kq_plane_sb(f, q, s, r * nsb + sb, false, w, sb * 256l)
                    }
                } else {
                    for (bi in range64(nb)) {
                        dequant_b32_plane_block(f, q, s, r * nb + bi, w, bi * 32l)
                    }
                }
            }
        }
        var acc = 0.0
        for (i in range64(n)) {
            acc += w[i] * xa[i]
        }
        y[r] = acc * site_scale(m, f, 0l)   //! the shell's per-tensor scale (nv4's; unity elsewhere), part of the tensor's value
    }
    return <- y
}

//! the planar dispatcher's rows (`mm_f`; the mx4 scalar dot, which no planar dispatcher takes off a bare shell)
def private planar_rows(m : Model; f : KqFmt; x : array<float>; n, rows : int64) : array<float> {
    var y <- sentinel_rows(rows)
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    if (f == KqFmt.mx4) {
        let nb = n / 32l
        xq |> resize(n)
        xs |> resize(nb)
        quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
        var lut = mx4_lut()
        unsafe {
            for (r in range64(rows)) {
                y[r] = dot_mx4q8_scalar(addr<int8 const?>(lut[0]), addr<uint8 const?>(m.mxq[r * nb * MX4_QB]), addr<uint8 const?>(m.mxs[r * nb * MX4_SB]),
                    addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n)
            }
        }
        return <- y
    }
    xq |> resize(n)
    xs |> resize(n / 32l)
    xbs |> resize(n / 16l)
    mm_f(y, m, f, 0l, x, xq, xs, xbs, n, rows)
    return <- y
}

//! the twin's rows over the device planes the engine's gather made of the shell's stack
def private twin_rows(m : Model; f : KqFmt; x : array<float>; n, rows : int64) : array<float> {
    var wq, ws : array<uint8>
    layout_gather_dispatch(m, f, 0l, n, rows, rows, 1l, 32l, 0l, wq, ws, true)
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(n)
    if (kq_sb(f)) {   //! a superblock plane's twin reads the Q8_K form, a scale a superblock
        xs |> resize(n / 256l)
        xbs |> resize(n / 16l)
        quantize_q8_k_into(x, n, xq, xs, xbs, 0l, 0l, 0l)
    } else {
        xs |> resize(n / 32l)
        quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    }
    var y <- sentinel_rows(rows)
    let twin = moe_gpu_host_twin(int(f))
    unsafe {
        moe_gpu_host_rows(twin, addr(y[0]), addr<uint8 const?>(wq[0]), addr<uint8 const?>(ws[0]), addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, 0l, rows)
    }
    return <- y
}

def private twin_cell(t : T?; f : KqFmt) {
    if (!moe_gpu_host_rows_on()) {
        t |> skip("no host twin is registered - a build with neither discrete home (the CUDA home registers it under DASLLAMA_CUDA=1)")
        return
    }
    t |> success(moe_gpu_host_twin_serves(int(f)), "the host twin serves {f}")
    let n = kq_sb(f) ? N_SB : N_B32
    var inscope m <- planar_shell(f, n)
    var inscope x <- x_row(n)
    var inscope want <- planar_rows(m, f, x, n, ROWS)
    var inscope ref <- ref_rows(m, f, x, n, ROWS)
    var inscope got <- twin_rows(m, f, x, n, ROWS)
    check(t, "the planar kernels over the {f} planes against the plain dequantized dot", want, ref, twin_bar(ref), Shape(row_width = int(ROWS)))
    check(t, "the host twin over the {f} device planes against the planar kernels", got, want, twin_bar(want), Shape(row_width = int(ROWS)))
}

[test]
def test_host_twin_superblock(t : T?) {
    for (f in [KqFmt.k2, KqFmt.k3, KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.q40, KqFmt.iq4xs, KqFmt.iq4nl, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq3s4, KqFmt.nv4, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs]) {
        t |> run("the host twin over a {f} expert stack") <| @(ct : T?) {
            twin_cell(ct, f)
        }
    }
}

[test]
def test_host_twin_block32(t : T?) {
    for (f in [KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032, KqFmt.mx4, KqFmt.q8]) {
        t |> run("the host twin over a {f} expert stack") <| @(ct : T?) {
            twin_cell(ct, f)
        }
    }
}

//! the row-major q8 kernel over the device planes the engine's gather made, against the planar kernels: the one-copy rail's kernel, the twin above its reference decode
[test]
def test_host_rm_q8(t : T?) {
    let f = KqFmt.q8
    let n = N_B32
    var inscope m <- planar_shell(f, n)
    var inscope x <- x_row(n)
    var inscope want <- planar_rows(m, f, x, n, ROWS)
    var wq, ws : array<uint8>
    layout_gather_dispatch(m, f, 0l, n, ROWS, ROWS, 1l, 32l, 0l, wq, ws, true)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(n / 32l)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    var inscope got <- sentinel_rows(ROWS)
    unsafe {
        q8q8_rm_gemv_gen(addr(got[0]), addr<int8 const?>(wq[0]), addr<uint16 const?>(ws[0]), addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, 0l, ROWS)
    }
    check(t, "the row-major q8 kernel over the device planes against the planar kernels", got, want, twin_bar(want), Shape(row_width = int(ROWS)))
}

//! the per-32 and MXFP4 row-major kernels over the device planes against the planar kernels
def private rm_b32_rows(m : Model; f : KqFmt; x : array<float>; n, rows : int64) : array<float> {
    var wq, ws : array<uint8>
    layout_gather_dispatch(m, f, 0l, n, rows, rows, 1l, 32l, 0l, wq, ws, true)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(n / 32l)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    var got <- sentinel_rows(rows)
    unsafe {
        let q = addr<uint8 const?>(wq[0])
        let s = addr<uint8 const?>(ws[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        var yp = addr(got[0])
        if (f == KqFmt.iq4nl32) {
            iq4nl32q8_rm_gemv_gen(yp, q, s, xqp, xsp, null, n, 0l, rows)
        } elif (f == KqFmt.q4032) {
            q4032q8_rm_gemv_gen(yp, q, s, xqp, xsp, null, n, 0l, rows)
        } elif (f == KqFmt.q51) {
            q51q8_rm_gemv_gen(yp, q, s, xqp, xsp, null, n, 0l, rows)
        } elif (f == KqFmt.q5032) {
            q5032q8_rm_gemv_gen(yp, q, s, xqp, xsp, null, n, 0l, rows)
        } else {
            mx4q8_rm_gemv_gen(yp, q, s, xqp, xsp, null, n, 0l, rows)
        }
    }
    return <- got
}

[test]
def test_host_rm_block32(t : T?) {
    for (f in [KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q51, KqFmt.q5032, KqFmt.mx4]) {
        t |> run("the row-major kernel over a {f} expert stack") <| @(ct : T?) {
            let n = N_B32
            var inscope m <- planar_shell(f, n)
            var inscope x <- x_row(n)
            var inscope want <- planar_rows(m, f, x, n, ROWS)
            var inscope got <- rm_b32_rows(m, f, x, n, ROWS)
            check(ct, "the row-major kernel over the {f} device planes against the planar kernels", got, want, twin_bar(want), Shape(row_width = int(ROWS)))
        }
    }
}

//! the K-quant row-major kernels over the device planes against the planar kernels, under the Q8_K activation
def private rm_kq_rows(m : Model; f : KqFmt; x : array<float>; n, rows : int64) : array<float> {   // nolint:STYLE037 - kq_fmt_stamp's generated arms, one a format
    var wq, ws : array<uint8>
    layout_gather_dispatch(m, f, 0l, n, rows, rows, 1l, 32l, 0l, wq, ws, true)
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(n)
    xs |> resize(n / 256l)
    xbs |> resize(n / 16l)
    quantize_q8_k_into(x, n, xq, xs, xbs, 0l, 0l, 0l)
    var got <- sentinel_rows(rows)
    unsafe {
        let q = addr<uint8 const?>(wq[0])
        let s = addr<uint8 const?>(ws[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let xbsp = addr<int const?>(xbs[0])
        var yp = addr(got[0])
        kq_fmt_stamp(f) $(F) {
            rm_gemv_gen(yp, q, s, xqp, xsp, xbsp, n, 0l, rows, F)
        }
    }
    return <- got
}

[test]
def test_host_rm_superblock(t : T?) {
    for (f in [KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.k3, KqFmt.k2, KqFmt.q40, KqFmt.iq4nl, KqFmt.iq4xs, KqFmt.iq3s4, KqFmt.nv4,
               KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs]) {
        t |> run("the row-major kernel over a {f} expert stack") <| @(ct : T?) {
            let n = N_SB
            var inscope m <- planar_shell(f, n)
            var inscope x <- x_row(n)
            var inscope want <- planar_rows(m, f, x, n, ROWS)
            var inscope got <- rm_kq_rows(m, f, x, n, ROWS)
            check(ct, "the row-major kernel over the {f} device planes against the planar kernels", got, want, twin_bar(want), Shape(row_width = int(ROWS)))
        }
    }
}

//! four activation rows through a format's row-major tile over the device planes, against the planar kernels a row
def private rm_tile4_rows(m : Model; f : KqFmt; xs4 : array<float>; n, rows : int64) : array<float> {   // nolint:STYLE037 - kq_fmt_stamp's generated arms, one a format
    var wq, ws : array<uint8>
    layout_gather_dispatch(m, f, 0l, n, rows, rows, 1l, 32l, 0l, wq, ws, true)
    let sb = kq_sb(f)
    let nsc = sb ? n / 256l : n / 32l
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    xq |> resize(4l * n)
    xs |> resize(4l * nsc)
    xbs |> resize(4l * (n / 16l))
    for (tk in range64(4l)) {
        var inscope xr : array<float>
        xr |> resize(n)
        for (i in range64(n)) {
            xr[i] = xs4[tk * n + i]
        }
        if (sb) {
            quantize_q8_k_into(xr, n, xq, xs, xbs, tk * n, tk * nsc, tk * (n / 16l))
        } else {
            quantize_q8_0_into(xr, n, xq, xs, tk * n, tk * nsc)
        }
    }
    var got <- sentinel_rows(4l * rows)
    unsafe {
        let q = addr<uint8 const?>(wq[0])
        let s = addr<uint8 const?>(ws[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let xbsp = addr<int const?>(xbs[0])
        var yp = addr(got[0])
        kq_fmt_stamp(f) $(F) {
            rm_tile4_gen(yp, q, s, xqp, xsp, xbsp, n, rows, F)
        }
    }
    return <- got
}

[test]
def test_host_rm_tile4(t : T?) {
    for (f in type<KqFmt>) {
        t |> run("the row-major tile over a {f} expert stack") <| @(ct : T?) {
            let n = kq_sb(f) ? N_SB : N_B32
            var inscope m <- planar_shell(f, n)
            var xs4, want : array<float>
            xs4 |> resize(4l * n)
            want |> resize(4l * ROWS)
            for (tk in range64(4l)) {
                var inscope x <- x_row(n)
                for (i in range64(n)) {
                    x[i] *= 1.0 + 0.25 * float(tk)
                    xs4[tk * n + i] = x[i]
                }
                var inscope w <- planar_rows(m, f, x, n, ROWS)
                for (r in range64(ROWS)) {
                    want[tk * ROWS + r] = w[r]
                }
            }
            var inscope got <- rm_tile4_rows(m, f, xs4, n, ROWS)
            check(ct, "the row-major tile over the {f} device planes against the planar kernels a row", got, want, twin_bar(want), Shape(row_width = int(ROWS)))
        }
    }
}
