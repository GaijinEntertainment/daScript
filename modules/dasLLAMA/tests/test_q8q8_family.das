//! The q8q8 kernel family against an in-test fp64 dequant reference, on every registered backend. Skips: the tile half of the two stamped-tile cells where the stamp is a bf16 tile.
options gen2
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_math_gen
require dasllama/dasllama_repack
require dasllama/dasllama_convert
require daslib/f16_cvt
require daslib/jobque_boost
require math
require _compares   // nan_plane / nan_fill: the outputs' sentinel fill
require _kq_fixtures   // the unit fills, the Q8_0 rows and their sums, the fp64 references and their bars

//! The q8q8 kernel family - stamped tile vs GEMV, the mm/batch/group3/groupN slots on every registered backend, the wscale_f16 twins, matmul_q8/dot_q8 and the mx4 batch slots - each against an in-test fp64 dequant reference carrying its own poison control.

let WIDTHS <- [64l, 96l, 512l, 1024l, 1056l, 3072l]
let REF_POISON = 0.25

def private slice_i8(src : array<int8>; off, cnt : int64) : array<int8> =>
    <- [for (i in range64(off, off + cnt)); src[i]]

def private slice_f(src : array<float>; off, cnt : int64) : array<float> =>
    <- [for (i in range64(off, off + cnt)); src[i]]

//! bit for bit, the control `REF_POISON` added at the middle expected element
def private identical(t : T?; got : array<float>; want : array<float>; what : string) {
    check(t, what, got, want, Bar(exact = true, poison = REF_POISON, poison_at = length(want) / 2), FLAT_SHAPE)
}

//! within each element's fp64 bar `bar`, the control `REF_POISON` added at the middle expected element; `d` is the image's
//! row count a token (0 = flat)
def private held(t : T?; got : array<float>; ref : array<float>; bar : array<float>; what : string; d : int64 = 0l) {
    check(t, what, got, ref, Bar(kind = BarKind.per_element, poison = REF_POISON, poison_at = length(ref) / 2), Shape(row_width = int(d)), bar)
}

//! the fp64 bar's control alone, on an output a looser bar holds: `REF_POISON` added at the middle expected element must exceed it
def private poisoned(t : T?; got : array<float>; ref : array<float>; bar : array<float>; what : string) {
    let i = long_length(ref) / 2l
    t |> success(!(abs(got[i] - (ref[i] + REF_POISON)) <= bar[i]),
        "{what}: poison {REF_POISON} added at element {i} must exceed bar {bar[i]}")
}

// The bf16 envelope: a bf16 operand sits within 2^-8 of its value, so a weight-activation
// product within 2^-7 of its own magnitude; the terms round independently, so a row's error is
// 2^-7 / sqrt(3) of the terms' rms times sqrt(n), against an output of the same order - 0.5% of
// the image's largest magnitude on these fixtures, held at 1%. The image's magnitude added to the
// middle expected element must land outside it.
def private held_bf16(t : T?; got : array<float>; ref : array<float>; d : int64; what : string) {
    check(t, what, got, ref, Bar(kind = BarKind.rel_of_max, rel = 1.0e-2, poison = 1.0, poison_of_max = true, poison_at = length(ref) / 2), Shape(row_width = int(d)))
}

def private s16_planes(src : array<float>; cnt : int64; var s16 : array<uint16>; var s32 : array<float>) {
    s16 |> resize(cnt)
    s32 |> resize(cnt)
    for (i in range64(cnt)) {
        let h = f32_to_f16(src[i])
        s16[i] = uint16(h)
        s32[i] = f16_to_f32(h)
    }
}

def private narrow_scales(src : array<float>; cnt : int64; var s16 : array<uint16>) {
    s16 |> resize(cnt)
    for (i in range64(cnt)) {
        s16[i] = uint16(f32_to_f16(src[i]))
    }
}

//! the fp64 image of `ntok` activation rows through the `d` weight rows
def private ref_image(wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>;
                      ntok, d, n : int64; var ref : array<float>; var bar : array<float>) {
    ref_plane(ref, bar, 0l, ntok, d, n / 32l, REF_ULP) $(p, r) => ref_dot_q8_f64(wq, ws, r * n, xq, xs, p * n, n)
}

//! the fp64 rows of `nreg` regions of `dreg` weight rows, region r reading activation row r
def private groupn_ref(wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>;
                       nreg, dreg, n : int64; var ref : array<float>; var bar : array<float>) {
    ref_plane(ref, bar, 0l, nreg, dreg, n / 32l, REF_ULP) $(r, row) => ref_dot_q8_f64(wq, ws, (r * dreg + row) * n, xq, xs, r * n, n)
}

//! `ntok` x matmul_q8q8 over the f32 scale plane, or the f16 one at weight offset 0
def private gemv_image(var y : array<float>; wq : array<int8>; ws : array<auto(ST)>;
                       xq : array<int8>; xs : array<float>; ntok, d, n : int64) {
    let nb = n / 32l
    y |> resize(ntok * d)
    var yrow : array<float>
    yrow |> resize(d)
    for (p in range64(ntok)) {
        let rxq <- slice_i8(xq, p * n, n)
        let rxs <- slice_f(xs, p * nb, nb)
        nan_fill(yrow)
        static_if (typeinfo is_float(type<ST>)) {
            matmul_q8q8(yrow, wq, ws, rxq, rxs, n, d)
        } else {
            matmul_q8q8(yrow, wq, ws, 0l, rxq, rxs, n, d)
        }
        for (r in range64(d)) {
            y[p * d + r] = yrow[r]
        }
    }
}

//! rows [0, rows) of every token's image through the GEMV stamp over the f32 or the f16 scale plane
def private stamp_gemv_image(var y : array<float>; q : array<int8>; s : array<auto(ST)>; xq : array<int8>; xs : array<float>;
                             ntok, d, n, rows : int64) {
    let nb = n / 32l
    unsafe {
        let wp = addr<int8 const?>(q[0])
        let sp = addr<ST const?>(s[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        var ygp = addr(y[0])
        for (p in range64(ntok)) {
            static_if (typeinfo is_float(type<ST>)) {
                q8q8_gemv_gen(ygp + p * d, wp, sp, xqp + p * n, xsp + p * nb, n, 0l, rows)
            } else {
                q8q8_gemv_s16_gen(ygp + p * d, wp, sp, xqp + p * n, xsp + p * nb, n, 0l, rows)
            }
        }
    }
}

//! The stamps' tile walk over the f32 or the f16 scale plane: each unit of `gs` mr-row groups runs `ts` tokens a tile call
//! (the 4-token tile under `tile4`, else the stamped tile) and its tail tokens through the GEMV stamp.
def private stamp_tile_walk(var y : array<float>; q : array<int8>; s : array<auto(ST)>; xq : array<int8>; xs : array<float>; xbs : array<int>;
                            ntok, d, n, mr, ts, gs : int64; tile4 : bool) {
    let nb = n / 32l
    unsafe {
        let wp = addr<int8 const?>(q[0])
        let sp = addr<ST const?>(s[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let xbsp = addr<int const?>(xbs[0])
        var yp = addr(y[0])
        for (gu in range64(d / mr / gs)) {
            let gg = gu * gs
            var tk = 0l
            while (tk + ts <= ntok) {
                static_if (typeinfo is_float(type<ST>)) {
                    if (tile4) {
                        q8q8_tile4_gen(yp, wp + gg * mr * n, sp + gg * mr * nb, xqp, xsp, xbsp, n, d, gg, tk)
                    } else {
                        q8q8_tile_gen(yp, wp + gg * mr * n, sp + gg * mr * nb, xqp, xsp, xbsp, n, d, gg, tk)
                    }
                } else {
                    if (tile4) {
                        q8q8_tile4_s16_gen(yp, wp + gg * mr * n, sp + gg * mr * nb, xqp, xsp, xbsp, n, d, gg, tk)
                    } else {
                        q8q8_tile_s16_gen(yp, wp + gg * mr * n, sp + gg * mr * nb, xqp, xsp, xbsp, n, d, gg, tk)
                    }
                }
                tk += ts
            }
            while (tk < ntok) {
                static_if (typeinfo is_float(type<ST>)) {
                    q8q8_gemv_gen(yp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, gg * mr, (gg + gs) * mr)
                } else {
                    q8q8_gemv_s16_gen(yp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, gg * mr, (gg + gs) * mr)
                }
                tk++
            }
        }
    }
}

def private concat3(y0 : array<float>; y1 : array<float>; y2 : array<float>; var fused : array<float>) {
    fused |> resize(long_length(y0) + long_length(y1) + long_length(y2))
    var w = 0l
    for (v in y0) {
        fused[w] = v
        w++
    }
    for (v in y1) {
        fused[w] = v
        w++
    }
    for (v in y2) {
        fused[w] = v
        w++
    }
}

def private q8q8_stamped_gate(t : T?; n : int64) {
    let d = 64l
    let mr = int64(q8q8_layout_gen())
    let ts = int64(q8q8_tokstep_gen())
    let gs = ts == 4l ? 1l : ts / 16l
    let ng = d / mr
    let ntok = 2l * ts + 3l
    var wf, xf : array<float>
    fill_unit(wf, d * n, 0x51D0Au)
    fill_unit(xf, ntok * n, 0x51D0Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref, bar : array<float>
    ref_image(wq0.q, wq0.scales, xq, xs, ntok, d, n, ref, bar)
    var xbs : array<int>
    activation_bsums(xq, ntok, n, xbs)
    var q : array<int8>
    var s : array<float>
    q := wq0.q
    s := wq0.scales
    unsafe {
        repack_q8q8_grp(addr(q[0]), addr(s[0]), n, d, mr, int64(q8q8_wbias_gen()), int64(q8q8_kgroup_gen()))
    }
    var ytile <- nan_plane(int(ntok * d))
    var ygemv <- nan_plane(int(ntok * d))
    stamp_gemv_image(ygemv, q, s, xq, xs, ntok, d, n, ng * mr)
    held(t, ygemv, ref, bar, "q8q8 stamped GEMV, n={n}")
    var ytile4 <- nan_plane(int(ntok * d))
    stamp_tile_walk(ytile4, q, s, xq, xs, xbs, ntok, d, n, mr, 4l, 1l, true)
    identical(t, ytile4, ygemv, "q8q8 tile4 (mr={mr}) bit-matches the per-token GEMV, n={n}")
    held(t, ytile4, ref, bar, "q8q8 stamped tile4, n={n}", d)
    if (q8q8_tileform_gen() > 0) {
        t |> skip("the stamped tile is the bf16 tile - it reads a bf16 panel, not the int8 planes; the mm/batch cell reaches it through the batch kernel")
        return
    }
    q8q8_family_live()   //! on an amx stamp the witness is the tile unit's grant: a tile called with no backend selected faults without it
    q8q8_amx_cfg_gen()
    stamp_tile_walk(ytile, q, s, xq, xs, xbs, ntok, d, n, mr, ts, gs, false)
    identical(t, ytile, ygemv, "q8q8 tile (mr={mr} tokstep={ts}) bit-matches the per-token GEMV, n={n}")
    held(t, ytile, ref, bar, "q8q8 stamped tile, n={n}")
}

def private q8q8_wrapper_gate(t : T?; n : int64; d : int64 = 64l; ntok_override : int64 = 0l) {
    let ts = int64(q8q8_tokstep_gen())
    let ntok = ntok_override > 0l ? ntok_override : 2l * ts + 3l
    var wf, xf : array<float>
    fill_unit(wf, d * n, 0x51D1Au)
    fill_unit(xf, ntok * n, 0x51D1Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref, bar : array<float>
    ref_image(wq0.q, wq0.scales, xq, xs, ntok, d, n, ref, bar)
    let seat = kernel_backend_seat()
    for (name in kernel_backend_names()) {
        pin_kernel_backend(name)
        var q : array<int8>
        var s : array<float>
        q := wq0.q
        s := wq0.scales
        if (kernel_backend_needs_repack(name)) {
            repack_q8q8_weight(q, s, 0l, n, d)
        }
        var ymm : array<float>
        gemv_image(ymm, q, s, xq, xs, ntok, d, n)
        var ybat : array<float>
        ybat |> resize(ntok * d)
        nan_fill(ybat)
        matmul_q8q8_batch(ybat, q, s, xq, xs, n, d, ntok)
        let bf16_stamp = name == "x64-gen" && q8q8_tileform_gen() > 0
        let bf16 = bf16_stamp && ntok >= ts
        if (name == "portable" || (bf16_stamp && ntok < ts)) {
            identical(t, ybat, ymm, "{name} matmul_q8q8_batch bit-matches ntok x matmul_q8q8, n={n} d={d} ntok={ntok}")
        } elif (bf16) {
            held_bf16(t, ybat, ymm, d, "{name} matmul_q8q8_batch on the bf16 tile vs ntok x matmul_q8q8, n={n} d={d}")
        } else {
            held(t, ybat, ymm, bar, "{name} matmul_q8q8_batch within the fp64 bar of ntok x matmul_q8q8, n={n} d={d}")
        }
        held(t, ymm, ref, bar, "{name} matmul_q8q8, n={n} d={d}")
        if (bf16) {
            held_bf16(t, ybat, ref, d, "{name} matmul_q8q8_batch on the bf16 tile, n={n} d={d}")
        } else {
            held(t, ybat, ref, bar, "{name} matmul_q8q8_batch, n={n} d={d}")
        }
    }
    restore_kernel_backend(seat)
}

[test]
def test_q8q8_tile_gemv_reference(t : T?) {
    with_dasllama_jobque_() {
        for (n in WIDTHS) {
            t |> run("q8q8 stamped tile vs GEMV vs fp64 dequant, n={n}") @(t : T?) {
                q8q8_stamped_gate(t, n)
            }
            t |> run("q8q8 mm/batch vs fp64 dequant on every backend, n={n}") @(t : T?) {
                q8q8_wrapper_gate(t, n)
            }
            t |> run("q8q8 mm/batch vs fp64 dequant on every backend, n={n} d=48") @(t : T?) {
                q8q8_wrapper_gate(t, n, 48l)
            }
            t |> run("q8q8 mm/batch at the decode batch shapes, ntok 4 and 9, n={n}") @(t : T?) {
                q8q8_wrapper_gate(t, n, 64l, 4l)
                q8q8_wrapper_gate(t, n, 64l, 9l)
            }
        }
    }
}

def private s16_kernel_gate(t : T?; n : int64) {
    let d = 64l
    let dreg = d / 2l
    let nb = n / 32l
    let ntok = 2l
    var wf, xf : array<float>
    fill_unit(wf, d * n, 0x5F16Au)
    fill_unit(xf, ntok * n, 0x5F16Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ws16 : array<uint16>
    var ws32 : array<float>
    s16_planes(wq0.scales, d * nb, ws16, ws32)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref, bar : array<float>
    ref_image(wq0.q, ws32, xq, xs, 1l, d, n, ref, bar)
    var gref, gbar : array<float>
    groupn_ref(wq0.q, ws32, xq, xs, 2l, dreg, n, gref, gbar)
    var ydot, ydot16, yrows, yrows16, ygn, ygn16 : array<float>
    ydot |> resize(d)
    ydot16 |> resize(d)
    yrows |> resize(d)
    yrows16 |> resize(d)
    ygn |> resize(d)
    ygn16 |> resize(d)
    nan_fill(ydot)
    nan_fill(ydot16)
    nan_fill(yrows)
    nan_fill(yrows16)
    nan_fill(ygn)
    nan_fill(ygn16)
    var offs <- [0l, 0l, dreg * n, n]
    unsafe {
        let wp = addr<int8 const?>(wq0.q[0])
        let s32p = addr<float const?>(ws32[0])
        let s16p = addr<uint16 const?>(ws16[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let offp = addr<int64 const?>(offs[0])
        for (r in range64(d)) {
            ydot[r] = dot_q8q8(wp + r * n, s32p + r * nb, xqp, xsp, n)
            ydot16[r] = dot_q8q8_f16s(wp + r * n, s16p + r * nb, xqp, xsp, n)
        }
        q8q8_rows_kernel(addr(yrows[0]), wp, s32p, xqp, xsp, n, 0l, d)
        q8q8_rows_kernel_s16(addr(yrows16[0]), wp, s16p, xqp, xsp, n, 0l, d)
        q8q8_groupn_kernel(addr(ygn[0]), wp, s32p, offp, 2l, xqp, xsp, null, null, n, dreg)
        q8q8_groupn_kernel_s16(addr(ygn16[0]), wp, s16p, offp, 2l, xqp, xsp, null, null, n, dreg)
    }
    identical(t, ydot16, ydot, "dot_q8q8_f16s bit-matches dot_q8q8 over the widened plane, n={n}")
    identical(t, yrows16, yrows, "q8q8_rows_kernel_s16 bit-matches its f32 twin, n={n}")
    identical(t, yrows16, ydot16, "q8q8_rows_kernel_s16 bit-matches the dot it wraps, n={n}")
    identical(t, ygn16, ygn, "q8q8_groupn_kernel_s16 bit-matches its f32 twin, n={n}")
    held(t, ydot16, ref, bar, "dot_q8q8_f16s, n={n}")
    held(t, yrows16, ref, bar, "q8q8_rows_kernel_s16, n={n}")
    held(t, ygn16, gref, gbar, "q8q8_groupn_kernel_s16, n={n}")
}

def private s16_stamped_gate(t : T?; n : int64) {
    let d = 64l
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ts = int64(q8q8_tokstep_gen())
    let gs = ts == 4l ? 1l : ts / 16l
    let ng = d / mr
    let ntok = 2l * ts + 3l
    var wf, xf : array<float>
    fill_unit(wf, d * n, 0x5F17Au)
    fill_unit(xf, ntok * n, 0x5F17Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ws16m : array<uint16>
    var ws32m : array<float>
    s16_planes(wq0.scales, d * nb, ws16m, ws32m)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref, bar : array<float>
    ref_image(wq0.q, ws32m, xq, xs, ntok, d, n, ref, bar)
    var xbs : array<int>
    activation_bsums(xq, ntok, n, xbs)
    var q : array<int8>
    var s : array<float>
    q := wq0.q
    s := ws32m
    unsafe {
        repack_q8q8_grp(addr(q[0]), addr(s[0]), n, d, mr, int64(q8q8_wbias_gen()), int64(q8q8_kgroup_gen()))
    }
    var s16 : array<uint16>
    narrow_scales(s, d * nb, s16)
    var ytile <- nan_plane(int(ntok * d))
    var ygemv <- nan_plane(int(ntok * d))
    stamp_gemv_image(ygemv, q, s16, xq, xs, ntok, d, n, ng * mr)
    held(t, ygemv, ref, bar, "q8q8 stamped s16 GEMV, n={n}")
    var ytile4 <- nan_plane(int(ntok * d))
    stamp_tile_walk(ytile4, q, s16, xq, xs, xbs, ntok, d, n, mr, 4l, 1l, true)
    identical(t, ytile4, ygemv, "q8q8 s16 tile4 (mr={mr}) bit-matches the per-token s16 GEMV, n={n}")
    held(t, ytile4, ref, bar, "q8q8 stamped s16 tile4, n={n}", d)
    if (q8q8_tileform_gen() > 0) {
        t |> skip("the stamped s16 tile is the bf16 tile - it reads a bf16 panel, not the int8 planes; the overloads cell reaches it through the batch kernel")
        return
    }
    q8q8_family_live()   //! on an amx stamp the witness is the tile unit's grant: a tile called with no backend selected faults without it
    q8q8_amx_cfg_gen()
    stamp_tile_walk(ytile, q, s16, xq, xs, xbs, ntok, d, n, mr, ts, gs, false)
    identical(t, ytile, ygemv, "q8q8 s16 tile (mr={mr} tokstep={ts}) bit-matches the per-token s16 GEMV, n={n}")
    held(t, ytile, ref, bar, "q8q8 stamped s16 tile, n={n}")
}

def private s16_wrapper_gate(t : T?; n : int64; ntok_override : int64 = 0l) {
    let d = 64l
    let dreg = d / 2l
    let nb = n / 32l
    let ts = int64(q8q8_tokstep_gen())
    let ntok = ntok_override > 0l ? ntok_override : 2l * ts + 3l
    var wf, xf : array<float>
    fill_unit(wf, d * n, 0x5F18Au)
    fill_unit(xf, ntok * n, 0x5F18Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ws16m : array<uint16>
    var ws32m : array<float>
    s16_planes(wq0.scales, d * nb, ws16m, ws32m)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref, bar : array<float>
    ref_image(wq0.q, ws32m, xq, xs, ntok, d, n, ref, bar)
    var gref, gbar : array<float>
    groupn_ref(wq0.q, ws32m, xq, xs, 2l, dreg, n, gref, gbar)
    var offs <- [0l, 0l, dreg * n, n]
    let seat = kernel_backend_seat()
    for (name in kernel_backend_names()) {
        pin_kernel_backend(name)
        if (!kernel_backend_has_wscale16()) {
            continue
        }
        var q : array<int8>
        var s : array<float>
        q := wq0.q
        s := ws32m
        if (kernel_backend_needs_repack(name)) {
            repack_q8q8_weight(q, s, 0l, n, d)
        }
        var s16 : array<uint16>
        narrow_scales(s, d * nb, s16)
        var ymm, ymm16 : array<float>
        gemv_image(ymm, q, s, xq, xs, ntok, d, n)
        gemv_image(ymm16, q, s16, xq, xs, ntok, d, n)
        var ybat16 : array<float>
        ybat16 |> resize(ntok * d)
        nan_fill(ybat16)
        matmul_q8q8_batch(ybat16, q, s16, 0l, xq, xs, n, d, ntok)
        var ygn, ygn16 : array<float>
        ygn |> resize(d)
        ygn16 |> resize(d)
        nan_fill(ygn)
        nan_fill(ygn16)
        matmul_q8q8_groupn(ygn, q, s, offs, 2l, xq, xs, n, dreg)
        matmul_q8q8_groupn(ygn16, q, s16, offs, 2l, xq, xs, n, dreg)
        let bf16 = name == "x64-gen" && q8q8_tileform_gen() > 0 && ntok >= ts
        identical(t, ymm16, ymm, "{name} matmul_q8q8 s16 bit-matches the f32 plane, n={n}")
        if (bf16) {
            held_bf16(t, ybat16, ymm16, d, "{name} matmul_q8q8_batch s16 on the bf16 tile vs ntok x matmul_q8q8 s16, n={n}")
        } else {
            identical(t, ybat16, ymm16, "{name} matmul_q8q8_batch s16 bit-matches ntok x matmul_q8q8 s16, n={n} ntok={ntok}")
        }
        identical(t, ygn16, ygn, "{name} matmul_q8q8_groupn s16 bit-matches the f32 plane, n={n}")
        held(t, ymm16, ref, bar, "{name} matmul_q8q8 s16, n={n}")
        if (bf16) {
            held_bf16(t, ybat16, ref, d, "{name} matmul_q8q8_batch s16 on the bf16 tile, n={n}")
            poisoned(t, ybat16, ref, bar, "{name} matmul_q8q8_batch s16, n={n}")
        } else {
            held(t, ybat16, ref, bar, "{name} matmul_q8q8_batch s16, n={n}")
        }
        held(t, ygn16, gref, gbar, "{name} matmul_q8q8_groupn s16, n={n}")
    }
    restore_kernel_backend(seat)
}

[test]
def test_q8q8_wscale_f16(t : T?) {
    with_dasllama_jobque_() {
        for (n in WIDTHS) {
            t |> run("wscale_f16 dot/rows/groupN kernels vs their f32 twins and fp64, n={n}") @(t : T?) {
                s16_kernel_gate(t, n)
            }
            t |> run("wscale_f16 stamped tile vs GEMV vs fp64, n={n}") @(t : T?) {
                s16_stamped_gate(t, n)
            }
            t |> run("wscale_f16 matmul_q8q8 overloads on every backend, n={n}") @(t : T?) {
                s16_wrapper_gate(t, n)
            }
            t |> run("wscale_f16 matmul_q8q8 overloads at the decode batch shapes, ntok 4 and 9, n={n}") @(t : T?) {
                s16_wrapper_gate(t, n, 4l)
                s16_wrapper_gate(t, n, 9l)
            }
        }
    }
}

//! The portable kernels' wasm form, gated on every host: n=64 is the paired-block main loop alone, n=96 adds the single-block tail.
def private idot4_ps_gate(t : T?; n : int64) {
    let d = 64l
    let nb = n / 32l
    let ntok = 3l
    var wf, xf : array<float>
    fill_unit(wf, d * n, 0x1D074u)
    fill_unit(xf, ntok * n, 0x1D075u)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ws16m : array<uint16>
    var ws32m : array<float>
    s16_planes(wq0.scales, d * nb, ws16m, ws32m)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref, bar : array<float>
    ref_image(wq0.q, ws32m, xq, xs, ntok, d, n, ref, bar)
    var y, y16 : array<float>
    y |> resize(ntok * d)
    y16 |> resize(ntok * d)
    nan_fill(y)
    nan_fill(y16)
    unsafe {
        let wp = addr<int8 const?>(wq0.q[0])
        let sp = addr<float const?>(ws32m[0])
        let sp16 = addr<uint16 const?>(ws16m[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        for (p in range64(ntok)) {
            for (i in range64(d)) {
                y[p * d + i] = dot_q8q8_idot4_ps(wp + i * n, sp + i * nb, xqp + p * n, xsp + p * nb, n)
                y16[p * d + i] = dot_q8q8_idot4_ps_f16s(wp + i * n, sp16 + i * nb, xqp + p * n, xsp + p * nb, n)
            }
        }
    }
    held(t, y, ref, bar, "dot_q8q8_idot4_ps, n={n}")
    identical(t, y16, y, "dot_q8q8_idot4_ps_f16s bit-matches its f32 twin over the widened plane, n={n}")
}

[test]
def test_q8q8_idot4_ps(t : T?) {
    for (n in WIDTHS) {
        t |> run("vector-epilogue idot4 dots vs fp64 and the s16 twin, n={n}") @(t : T?) {
            idot4_ps_gate(t, n)
        }
    }
}

def private group3_gate(t : T?; n : int64) {
    let d0 = 32l
    let d1 = 40l
    let d2 = 44l
    let dt = d0 + d1 + d2
    let nb = n / 32l
    let woff0 = 0l
    let woff1 = d0 * n
    let woff2 = woff1 + d1 * n
    var wf, xf : array<float>
    fill_unit(wf, dt * n, 0x63A0Au)
    fill_unit(xf, n, 0x63A0Bu)
    let wq0 <- quantize_q8_0(wf, dt * n)
    var ws16m : array<uint16>
    var ws32m : array<float>
    s16_planes(wq0.scales, dt * nb, ws16m, ws32m)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, 1l, n, xq, xs)
    var ref, bar : array<float>
    ref_image(wq0.q, ws32m, xq, xs, 1l, dt, n, ref, bar)
    let seat = kernel_backend_seat()
    for (name in kernel_backend_names()) {
        pin_kernel_backend(name)
        var q := wq0.q
        var s := ws32m
        if (kernel_backend_needs_repack(name)) {
            repack_q8q8_weight(q, s, woff0, n, d0)
            repack_q8q8_weight(q, s, woff1, n, d1)
            repack_q8q8_weight(q, s, woff2, n, d2)
        }
        var s16 : array<uint16>
        narrow_scales(s, dt * nb, s16)
        var y0 <- nan_plane(int(d0))
        var y1 <- nan_plane(int(d1))
        var y2 <- nan_plane(int(d2))
        matmul_q8q8_group3(y0, y1, y2, q, s, woff0, woff1, woff2, xq, xs, n, d0, d1, d2)
        var g0 <- nan_plane(int(d0))
        var g1 <- nan_plane(int(d1))
        var g2 <- nan_plane(int(d2))
        matmul_q8q8(g0, q, s, woff0, xq, xs, n, d0)
        matmul_q8q8(g1, q, s, woff1, xq, xs, n, d1)
        matmul_q8q8(g2, q, s, woff2, xq, xs, n, d2)
        identical(t, y0, g0, "{name} group3 region 0 bit-matches its own GEMV, n={n}")
        identical(t, y1, g1, "{name} group3 region 1 bit-matches its own GEMV, n={n}")
        identical(t, y2, g2, "{name} group3 region 2 (row tail) bit-matches its own GEMV, n={n}")
        var fused : array<float>
        concat3(y0, y1, y2, fused)
        held(t, fused, ref, bar, "{name} matmul_q8q8_group3, n={n}")
        if (!kernel_backend_has_wscale16()) {
            continue
        }
        var h0 <- nan_plane(int(d0))
        var h1 <- nan_plane(int(d1))
        var h2 <- nan_plane(int(d2))
        matmul_q8q8_group3(h0, h1, h2, q, s16, woff0, woff1, woff2, xq, xs, n, d0, d1, d2)
        if (name == "portable") {
            identical(t, h0, y0, "{name} group3 s16 region 0 bit-matches the f32 plane, n={n}")
            identical(t, h1, y1, "{name} group3 s16 region 1 bit-matches the f32 plane, n={n}")
            identical(t, h2, y2, "{name} group3 s16 region 2 bit-matches the f32 plane, n={n}")
        } else {
            var fused16 : array<float>
            concat3(h0, h1, h2, fused16)
            held(t, fused16, ref, bar, "{name} matmul_q8q8_group3 s16 within the fp64 bar, n={n}")
        }
    }
    restore_kernel_backend(seat)
}

[test]
def test_q8q8_group3(t : T?) {
    with_dasllama_jobque_() {
        for (n in WIDTHS) {
            t |> run("matmul_q8q8_group3 (f32 and s16) vs three independent GEMVs and fp64, n={n}") @(t : T?) {
                group3_gate(t, n)
            }
        }
    }
}

def private q8_f32x_gate(t : T?; n : int64) {
    let d = 64l
    let nb = n / 32l
    var wf, xf : array<float>
    fill_unit(wf, d * n, 0x8F00Au)
    fill_unit(xf, n, 0x8F00Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ref, bar : array<float>
    ref_plane(ref, bar, 0l, 1l, d, nb, REF_ULP) $(_p, r) => ref_dot_q8f_f64(wq0.q, wq0.scales, r * n, xf, n)
    var ymm, yoff, ydot : array<float>
    ymm |> resize(d)
    yoff |> resize(d)
    ydot |> resize(d)
    nan_fill(ymm)
    nan_fill(yoff)
    nan_fill(ydot)
    matmul_q8(ymm, wq0.q, wq0.scales, xf, n, d)
    matmul_q8(yoff, wq0.q, wq0.scales, 0l, xf, n, d)
    unsafe {
        let wp = addr<int8 const?>(wq0.q[0])
        let sp = addr<float const?>(wq0.scales[0])
        let xp = addr<float const?>(xf[0])
        for (r in range64(d)) {
            ydot[r] = dot_q8(wp + r * n, sp + r * nb, xp, n)
        }
    }
    identical(t, ymm, ydot, "matmul_q8 bit-matches the per-row dot_q8, n={n}")
    identical(t, yoff, ydot, "matmul_q8 blob+offset bit-matches the per-row dot_q8, n={n}")
    held(t, ydot, ref, bar, "dot_q8, n={n}")
    held(t, ymm, ref, bar, "matmul_q8, n={n}")
}

[test]
def test_q8_f32_activation(t : T?) {
    with_dasllama_jobque_() {
        for (n in WIDTHS) {
            t |> run("matmul_q8 / dot_q8 vs fp64 dequant, n={n}") @(t : T?) {
                q8_f32x_gate(t, n)
            }
        }
    }
}

def private mx4_planes(var wn : array<uint8>; var we : array<uint8>; cnt, nblk : int64) {
    wn |> resize(cnt / 2l)
    we |> resize(nblk)
    for (i in range64(cnt / 2l)) {
        wn[i] = uint8(mix32(0x3A4Bu, uint(int(i))) >> 24u)
    }
    for (i in range64(nblk)) {
        we[i] = uint8(119u + mix32(0x3A4Cu, uint(int(i))) % 9u)
    }
}

//! the fp64 rows [t0, te) of the mx4 weight region at element `woff`
def private mx4_ref_region(wn : array<uint8>; we : array<uint8>; woff : int64;
                           xq : array<int8>; xs : array<float>; t0, te, d, n : int64;
                           var ref : array<float>; var bar : array<float>) {
    ref_plane(ref, bar, t0, te, d, n / 32l, REF_ULP) $(p, row) => ref_dot_mx4_f64(wn, we, (woff + row * n) / 2l, (woff + row * n) / 32l, xq, xs, p * n, n)
}

def private mx4_leaf_gate(t : T?; wn0 : array<uint8>; we0 : array<uint8>;
                          xq : array<int8>; xs : array<float>; d, n : int64) {
    let nb = n / 32l
    var yleaf, ymm : array<float>
    yleaf |> resize(d)
    ymm |> resize(d)
    nan_fill(yleaf)
    nan_fill(ymm)
    let rxq <- slice_i8(xq, 0l, n)
    let rxs <- slice_f(xs, 0l, nb)
    pin_kernel_backend("portable")
    matmul_mx4q8(ymm, wn0, we0, 0l, rxq, rxs, n, d)
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])
        let wnp = addr<uint8 const?>(wn0[0])
        let wep = addr<uint8 const?>(we0[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        for (r in range64(d)) {
            yleaf[r] = dot_mx4q8_scalar(lutp, wnp + r * (n / 2l), wep + r * nb, xqp, xsp, n)
        }
    }
    identical(t, ymm, yleaf, "portable matmul_mx4q8 bit-matches the dot_mx4q8_scalar leaf, n={n}")
}

def private mx4_batch_gate(t : T?; n : int64) {
    let d = 32l
    let nb = n / 32l
    let ntok = 9l
    let nreg = 2l
    let cnt0 = 4l
    let woff1 = d * n
    var wn0, we0 : array<uint8>
    mx4_planes(wn0, we0, nreg * d * n, nreg * d * nb)
    var xf : array<float>
    fill_unit(xf, ntok * n, 0x3A4Du)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var bref, bbar : array<float>
    bref |> resize(ntok * d)
    bbar |> resize(ntok * d)
    mx4_ref_region(wn0, we0, 0l, xq, xs, 0l, ntok, d, n, bref, bbar)
    var gref, gbar : array<float>
    gref |> resize(ntok * d)
    gbar |> resize(ntok * d)
    mx4_ref_region(wn0, we0, 0l, xq, xs, 0l, cnt0, d, n, gref, gbar)
    mx4_ref_region(wn0, we0, woff1, xq, xs, cnt0, ntok, d, n, gref, gbar)
    var goffs <- [0l, 0l, cnt0, woff1, cnt0, ntok - cnt0]
    let seat = kernel_backend_seat()
    mx4_leaf_gate(t, wn0, we0, xq, xs, d, n)
    var names : array<string>
    names |> push("portable")
    for (nm in kernel_backend_names()) {
        if (kernel_backend_needs_repack(nm)) {
            names |> push(nm)
        }
    }
    for (nm in names) {
        pin_kernel_backend(nm)
        var wn, we : array<uint8>
        wn := wn0
        we := we0
        if (kernel_backend_needs_repack(nm)) {
            for (r in range64(nreg)) {
                repack_mx4_weight(wn, we, r * d * n, n, d)
            }
        }
        var ymm <- nan_plane(int(ntok * d))
        var ygmm <- nan_plane(int(ntok * d))
        var yrow <- nan_plane(int(d))
        for (p in range64(ntok)) {
            let rxq <- slice_i8(xq, p * n, n)
            let rxs <- slice_f(xs, p * nb, nb)
            nan_fill(yrow)
            matmul_mx4q8(yrow, wn, we, 0l, rxq, rxs, n, d)
            for (row in range64(d)) {
                ymm[p * d + row] = yrow[row]
            }
            nan_fill(yrow)
            matmul_mx4q8(yrow, wn, we, p < cnt0 ? 0l : woff1, rxq, rxs, n, d)
            for (row in range64(d)) {
                ygmm[p * d + row] = yrow[row]
            }
        }
        var ybat <- nan_plane(int(ntok * d))
        matmul_mx4q8_batch(ybat, wn, we, 0l, xq, xs, n, d, ntok)
        var ygb <- nan_plane(int(ntok * d))
        matmul_mx4q8_batch_groupn(ygb, wn, we, goffs, nreg, xq, xs, n, d)
        identical(t, ybat, ymm, "{nm} matmul_mx4q8_batch bit-matches ntok x matmul_mx4q8, n={n}")
        identical(t, ygb, ygmm, "{nm} matmul_mx4q8_batch_groupn bit-matches the per-region GEMVs, n={n}")
        held(t, ybat, bref, bbar, "{nm} matmul_mx4q8_batch, n={n}")
        held(t, ygb, gref, gbar, "{nm} matmul_mx4q8_batch_groupn, n={n}")
    }
    restore_kernel_backend(seat)
}

[test]
def test_mx4q8_batch_family(t : T?) {
    with_dasllama_jobque_() {
        for (n in WIDTHS) {
            t |> run("mx4 batch / batch groupN vs per-token GEMVs, the scalar leaf and fp64, n={n}") @(t : T?) {
                mx4_batch_gate(t, n)
            }
        }
    }
}
