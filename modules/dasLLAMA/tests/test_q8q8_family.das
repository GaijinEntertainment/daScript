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
require llvm/daslib/f16_cvt
require daslib/jobque_boost
require math

//! The q8q8 kernel family - stamped tile vs GEMV, the mm/batch/group3/groupN slots on every registered backend, the wscale_f16 twins, matmul_q8/dot_q8 and the mx4 batch slots - each against an in-test fp64 dequant reference carrying its own poison control.

let WIDTHS <- [64l, 96l, 512l, 1024l, 1056l, 3072l]
let REF_ULP = 1.0e-6
let REF_POISON = 0.25
let MX4_E2M1 <- [0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0]

def private mix32(seed, idx : uint) : uint {
    var s = seed + idx * 2654435761u
    s = s ^ (s >> 15u)
    s = s * 2246822519u
    s = s ^ (s >> 13u)
    s = s * 3266489917u
    return s ^ (s >> 16u)
}

def private unit_at(seed, idx : uint) : float {
    return float(int(mix32(seed, idx) % 20001u) - 10000) * 1.0e-4
}

def private fill_unit(var a : array<float>; cnt : int64; seed : uint) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = unit_at(seed, uint(int(i)))
    }
}

def private slice_i8(src : array<int8>; off, cnt : int64) : array<int8> {
    return <- [for (i in range64(off, off + cnt)); src[i]]
}

def private slice_f(src : array<float>; off, cnt : int64) : array<float> {
    return <- [for (i in range64(off, off + cnt)); src[i]]
}

def private pow2d(k : int) : double {
    var v = 1.0lf
    for (_i in range(abs(k))) {
        v = k >= 0 ? v * 2.0lf : v * 0.5lf
    }
    return v
}

def private ref_bar(env : double; nb : int64) : float {
    return REF_ULP * float(nb) * max(float(env), 1.0)
}

def private ref_dot_f64(wq : array<int8>; ws : array<float>; woff : int64;
                        xq : array<int8>; xs : array<float>; xoff, n : int64) : tuple<dot : double; env : double> {
    var res : tuple<dot : double; env : double>
    for (bi in range64(n / 32l)) {
        var iacc = 0
        for (k in range64(32l)) {
            iacc += int(wq[woff + bi * 32l + k]) * int(xq[xoff + bi * 32l + k])
        }
        let term = double(iacc) * double(ws[woff / 32l + bi]) * double(xs[xoff / 32l + bi])
        res.dot += term
        res.env += abs(term)
    }
    return res
}

def private ref_dot_q8f_f64(wq : array<int8>; ws : array<float>; woff : int64;
                            x : array<float>; n : int64) : tuple<dot : double; env : double> {
    var res : tuple<dot : double; env : double>
    for (bi in range64(n / 32l)) {
        var bacc : double
        for (k in range64(32l)) {
            bacc += double(int(wq[woff + bi * 32l + k])) * double(x[bi * 32l + k])
        }
        let term = bacc * double(ws[woff / 32l + bi])
        res.dot += term
        res.env += abs(term)
    }
    return res
}

def private mx4_val(nib : int) : double {
    let m = MX4_E2M1[nib & 7]
    return double((nib & 8) != 0 ? -m : m)
}

def private ref_dot_mx4_f64(wn : array<uint8>; we : array<uint8>; roff, eoff : int64;
                            xq : array<int8>; xs : array<float>; xoff, n : int64) : tuple<dot : double; env : double> {
    var res : tuple<dot : double; env : double>
    for (bi in range64(n / 32l)) {
        var bacc : double
        for (j in range64(16l)) {
            let b = int(wn[roff + bi * 16l + j])
            bacc += mx4_val(b & 15) * double(int(xq[xoff + bi * 32l + j]))
            bacc += mx4_val(b >> 4) * double(int(xq[xoff + bi * 32l + j + 16l]))
        }
        let term = bacc * pow2d(int(we[eoff + bi]) - 127) * double(xs[xoff / 32l + bi])
        res.dot += term
        res.env += abs(term)
    }
    return res
}

def private identical(t : T?; got : array<float>; want : array<float>; what : string) {
    var bad = 0
    for (gv, wv in got, want) {
        if (gv != wv) {
            bad++
        }
    }
    t |> success(bad == 0, "{what}: {bad} of {long_length(got)} elements differ")
}

def private held(t : T?; got : array<float>; ref : array<float>; bar : array<float>; what : string) {
    var over = 0
    var worst = 0.0
    for (gv, rv, bv in got, ref, bar) {
        let df = abs(gv - rv)
        if (df > bv) {
            over++
        }
        worst = max(worst, df)
    }
    t |> success(over == 0, "{what}: {over} of {long_length(got)} past the fp64 bar, maxdiff {worst}, bar[0] {bar[0]}")
}

def private poisoned(t : T?; got : array<float>; ref : array<float>; bar : array<float>; what : string) {
    let i = long_length(ref) / 2l
    t |> success(abs(got[i] - (ref[i] + REF_POISON)) > bar[i],
        "{what}: poison {REF_POISON} added at element {i} must exceed bar {bar[i]}")
}

def private quantize_rows(src : array<float>; ntok, n : int64; var xq : array<int8>; var xs : array<float>) {
    let nb = n / 32l
    xq |> resize(ntok * n)
    xs |> resize(ntok * nb)
    var row : array<float>
    row |> resize(n)
    for (p in range64(ntok)) {
        for (i in range64(n)) {
            row[i] = src[p * n + i]
        }
        quantize_q8_0_into(row, n, xq, xs, p * n, p * nb)
    }
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

def private activation_bsums(xq : array<int8>; ntok, n : int64; var xbs : array<int>) {
    let nb = n / 32l
    xbs |> resize(ntok * nb)
    for (p in range64(ntok)) {
        for (bi in range64(nb)) {
            var sm = 0
            for (j in range64(32l)) {
                sm += int(xq[p * n + bi * 32l + j])
            }
            xbs[p * nb + bi] = -128 * sm
        }
    }
}

def private ref_image(wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>;
                      ntok, d, n : int64; var ref : array<float>; var bar : array<float>) {
    let nb = n / 32l
    ref |> resize(ntok * d)
    bar |> resize(ntok * d)
    for (p in range64(ntok)) {
        for (r in range64(d)) {
            let rf = ref_dot_f64(wq, ws, r * n, xq, xs, p * n, n)
            ref[p * d + r] = float(rf.dot)
            bar[p * d + r] = ref_bar(rf.env, nb)
        }
    }
}

def private groupn_ref(wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>;
                       nreg, dreg, n : int64; var ref : array<float>; var bar : array<float>) {
    let nb = n / 32l
    ref |> resize(nreg * dreg)
    bar |> resize(nreg * dreg)
    for (r in range64(nreg)) {
        for (row in range64(dreg)) {
            let rf = ref_dot_f64(wq, ws, (r * dreg + row) * n, xq, xs, r * n, n)
            ref[r * dreg + row] = float(rf.dot)
            bar[r * dreg + row] = ref_bar(rf.env, nb)
        }
    }
}

def private gemv_image_q8(var y : array<float>; wq : array<int8>; ws : array<float>;
                          xq : array<int8>; xs : array<float>; ntok, d, n : int64) {
    let nb = n / 32l
    y |> resize(ntok * d)
    var yrow : array<float>
    yrow |> resize(d)
    for (p in range64(ntok)) {
        let rxq <- slice_i8(xq, p * n, n)
        let rxs <- slice_f(xs, p * nb, nb)
        matmul_q8q8(yrow, wq, ws, rxq, rxs, n, d)
        for (r in range64(d)) {
            y[p * d + r] = yrow[r]
        }
    }
}

def private gemv_image_s16(var y : array<float>; wq : array<int8>; ws : array<uint16>;
                           xq : array<int8>; xs : array<float>; ntok, d, n : int64) {
    let nb = n / 32l
    y |> resize(ntok * d)
    var yrow : array<float>
    yrow |> resize(d)
    for (p in range64(ntok)) {
        let rxq <- slice_i8(xq, p * n, n)
        let rxs <- slice_f(xs, p * nb, nb)
        matmul_q8q8(yrow, wq, ws, 0l, rxq, rxs, n, d)
        for (r in range64(d)) {
            y[p * d + r] = yrow[r]
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

def private restore_backend(saved : string) {
    clear_kernel_backend_pin()
    if (saved != "") {
        select_kernel_backend(saved)
    }
}

def private q8q8_stamped_gate(t : T?; n : int64) {
    let d = 64l
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ts = int64(q8q8_tokstep_gen())
    let gs = ts == 4l ? 1l : ts / 16l
    let ng = d / mr
    let ntok = 2l * ts + 3l
    var wf : array<float>
    var xf : array<float>
    fill_unit(wf, d * n, 0x51D0Au)
    fill_unit(xf, ntok * n, 0x51D0Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref : array<float>
    var bar : array<float>
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
    var ytile : array<float>
    var ygemv : array<float>
    ytile |> resize(ntok * d)
    ygemv |> resize(ntok * d)
    unsafe {
        let wp = addr<int8 const?>(q[0])
        let sp = addr<float const?>(s[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let xbsp = addr<int const?>(xbs[0])
        var ytp = addr(ytile[0])
        var ygp = addr(ygemv[0])
        q8q8_amx_cfg_gen()
        for (gu in range64(ng / gs)) {
            let gg = gu * gs
            var tk = 0l
            while (tk + ts <= ntok) {
                q8q8_tile_gen(ytp, wp + gg * mr * n, sp + gg * mr * nb, xqp, xsp, xbsp, n, d, gg, tk)
                tk += ts
            }
            while (tk < ntok) {
                q8q8_gemv_gen(ytp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, gg * mr, (gg + gs) * mr)
                tk++
            }
        }
        for (p in range64(ntok)) {
            q8q8_gemv_gen(ygp + p * d, wp, sp, xqp + p * n, xsp + p * nb, n, 0l, ng * mr)
        }
    }
    identical(t, ytile, ygemv, "q8q8 tile (mr={mr} tokstep={ts}) bit-matches the per-token GEMV, n={n}")
    held(t, ygemv, ref, bar, "q8q8 stamped GEMV, n={n}")
    held(t, ytile, ref, bar, "q8q8 stamped tile, n={n}")
    poisoned(t, ygemv, ref, bar, "q8q8 stamped GEMV, n={n}")
}

def private q8q8_wrapper_gate(t : T?; n : int64) {
    let d = 64l
    let ntok = 2l * int64(q8q8_tokstep_gen()) + 3l
    var wf : array<float>
    var xf : array<float>
    fill_unit(wf, d * n, 0x51D1Au)
    fill_unit(xf, ntok * n, 0x51D1Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref : array<float>
    var bar : array<float>
    ref_image(wq0.q, wq0.scales, xq, xs, ntok, d, n, ref, bar)
    let saved = active_kernel_backend()
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
        gemv_image_q8(ymm, q, s, xq, xs, ntok, d, n)
        var ybat : array<float>
        ybat |> resize(ntok * d)
        matmul_q8q8_batch(ybat, q, s, xq, xs, n, d, ntok)
        if (name == "portable") {
            identical(t, ybat, ymm, "{name} matmul_q8q8_batch bit-matches ntok x matmul_q8q8, n={n}")
        } else {
            held(t, ybat, ymm, bar, "{name} matmul_q8q8_batch within the fp64 bar of ntok x matmul_q8q8, n={n}")
        }
        held(t, ymm, ref, bar, "{name} matmul_q8q8, n={n}")
        held(t, ybat, ref, bar, "{name} matmul_q8q8_batch, n={n}")
        poisoned(t, ymm, ref, bar, "{name} matmul_q8q8, n={n}")
    }
    restore_backend(saved)
}

[test]
def test_q8q8_tile_gemv_reference(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
        for (n in WIDTHS) {
            t |> run("q8q8 stamped tile vs GEMV vs fp64 dequant, n={n}") @(t : T?) {
                q8q8_stamped_gate(t, n)
            }
            t |> run("q8q8 mm/batch vs fp64 dequant on every backend, n={n}") @(t : T?) {
                q8q8_wrapper_gate(t, n)
            }
        }
    }
}

def private s16_kernel_gate(t : T?; n : int64) {
    let d = 64l
    let dreg = d / 2l
    let nb = n / 32l
    let ntok = 2l
    var wf : array<float>
    var xf : array<float>
    fill_unit(wf, d * n, 0x5F16Au)
    fill_unit(xf, ntok * n, 0x5F16Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ws16 : array<uint16>
    var ws32 : array<float>
    s16_planes(wq0.scales, d * nb, ws16, ws32)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref : array<float>
    var bar : array<float>
    ref_image(wq0.q, ws32, xq, xs, 1l, d, n, ref, bar)
    var gref : array<float>
    var gbar : array<float>
    groupn_ref(wq0.q, ws32, xq, xs, 2l, dreg, n, gref, gbar)
    var ydot : array<float>
    var ydot16 : array<float>
    var yrows : array<float>
    var yrows16 : array<float>
    var ygn : array<float>
    var ygn16 : array<float>
    ydot |> resize(d)
    ydot16 |> resize(d)
    yrows |> resize(d)
    yrows16 |> resize(d)
    ygn |> resize(d)
    ygn16 |> resize(d)
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
    poisoned(t, ydot16, ref, bar, "dot_q8q8_f16s, n={n}")
    poisoned(t, ygn16, gref, gbar, "q8q8_groupn_kernel_s16, n={n}")
}

def private s16_stamped_gate(t : T?; n : int64) {
    let d = 64l
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ts = int64(q8q8_tokstep_gen())
    let gs = ts == 4l ? 1l : ts / 16l
    let ng = d / mr
    let ntok = 2l * ts + 3l
    var wf : array<float>
    var xf : array<float>
    fill_unit(wf, d * n, 0x5F17Au)
    fill_unit(xf, ntok * n, 0x5F17Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ws16m : array<uint16>
    var ws32m : array<float>
    s16_planes(wq0.scales, d * nb, ws16m, ws32m)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref : array<float>
    var bar : array<float>
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
    var ytile : array<float>
    var ygemv : array<float>
    ytile |> resize(ntok * d)
    ygemv |> resize(ntok * d)
    unsafe {
        let wp = addr<int8 const?>(q[0])
        let sp = addr<uint16 const?>(s16[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let xbsp = addr<int const?>(xbs[0])
        var ytp = addr(ytile[0])
        var ygp = addr(ygemv[0])
        q8q8_amx_cfg_gen()
        for (gu in range64(ng / gs)) {
            let gg = gu * gs
            var tk = 0l
            while (tk + ts <= ntok) {
                q8q8_tile_s16_gen(ytp, wp + gg * mr * n, sp + gg * mr * nb, xqp, xsp, xbsp, n, d, gg, tk)
                tk += ts
            }
            while (tk < ntok) {
                q8q8_gemv_s16_gen(ytp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, gg * mr, (gg + gs) * mr)
                tk++
            }
        }
        for (p in range64(ntok)) {
            q8q8_gemv_s16_gen(ygp + p * d, wp, sp, xqp + p * n, xsp + p * nb, n, 0l, ng * mr)
        }
    }
    identical(t, ytile, ygemv, "q8q8 s16 tile (mr={mr} tokstep={ts}) bit-matches the per-token s16 GEMV, n={n}")
    held(t, ygemv, ref, bar, "q8q8 stamped s16 GEMV, n={n}")
    held(t, ytile, ref, bar, "q8q8 stamped s16 tile, n={n}")
    poisoned(t, ytile, ref, bar, "q8q8 stamped s16 tile, n={n}")
}

def private s16_wrapper_gate(t : T?; n : int64) {
    let d = 64l
    let dreg = d / 2l
    let nb = n / 32l
    let ntok = 2l * int64(q8q8_tokstep_gen()) + 3l
    var wf : array<float>
    var xf : array<float>
    fill_unit(wf, d * n, 0x5F18Au)
    fill_unit(xf, ntok * n, 0x5F18Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ws16m : array<uint16>
    var ws32m : array<float>
    s16_planes(wq0.scales, d * nb, ws16m, ws32m)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var ref : array<float>
    var bar : array<float>
    ref_image(wq0.q, ws32m, xq, xs, ntok, d, n, ref, bar)
    var gref : array<float>
    var gbar : array<float>
    groupn_ref(wq0.q, ws32m, xq, xs, 2l, dreg, n, gref, gbar)
    var offs <- [0l, 0l, dreg * n, n]
    let saved = active_kernel_backend()
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
        var ymm : array<float>
        var ymm16 : array<float>
        gemv_image_q8(ymm, q, s, xq, xs, ntok, d, n)
        gemv_image_s16(ymm16, q, s16, xq, xs, ntok, d, n)
        var ybat16 : array<float>
        ybat16 |> resize(ntok * d)
        matmul_q8q8_batch(ybat16, q, s16, 0l, xq, xs, n, d, ntok)
        var ygn : array<float>
        var ygn16 : array<float>
        ygn |> resize(d)
        ygn16 |> resize(d)
        matmul_q8q8_groupn(ygn, q, s, offs, 2l, xq, xs, n, dreg)
        matmul_q8q8_groupn(ygn16, q, s16, offs, 2l, xq, xs, n, dreg)
        identical(t, ymm16, ymm, "{name} matmul_q8q8 s16 bit-matches the f32 plane, n={n}")
        identical(t, ybat16, ymm16, "{name} matmul_q8q8_batch s16 bit-matches ntok x matmul_q8q8 s16, n={n}")
        identical(t, ygn16, ygn, "{name} matmul_q8q8_groupn s16 bit-matches the f32 plane, n={n}")
        held(t, ymm16, ref, bar, "{name} matmul_q8q8 s16, n={n}")
        held(t, ybat16, ref, bar, "{name} matmul_q8q8_batch s16, n={n}")
        held(t, ygn16, gref, gbar, "{name} matmul_q8q8_groupn s16, n={n}")
        poisoned(t, ybat16, ref, bar, "{name} matmul_q8q8_batch s16, n={n}")
        poisoned(t, ygn16, gref, gbar, "{name} matmul_q8q8_groupn s16, n={n}")
    }
    restore_backend(saved)
}

[test]
def test_q8q8_wscale_f16(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
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
    var wf : array<float>
    var xf : array<float>
    fill_unit(wf, dt * n, 0x63A0Au)
    fill_unit(xf, n, 0x63A0Bu)
    let wq0 <- quantize_q8_0(wf, dt * n)
    var ws16m : array<uint16>
    var ws32m : array<float>
    s16_planes(wq0.scales, dt * nb, ws16m, ws32m)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, 1l, n, xq, xs)
    var ref : array<float>
    var bar : array<float>
    ref_image(wq0.q, ws32m, xq, xs, 1l, dt, n, ref, bar)
    let saved = active_kernel_backend()
    for (name in kernel_backend_names()) {
        pin_kernel_backend(name)
        var q : array<int8>
        var s : array<float>
        q := wq0.q
        s := ws32m
        if (kernel_backend_needs_repack(name)) {
            repack_q8q8_weight(q, s, woff0, n, d0)
            repack_q8q8_weight(q, s, woff1, n, d1)
            repack_q8q8_weight(q, s, woff2, n, d2)
        }
        var s16 : array<uint16>
        narrow_scales(s, dt * nb, s16)
        var y0 : array<float>
        var y1 : array<float>
        var y2 : array<float>
        y0 |> resize(d0)
        y1 |> resize(d1)
        y2 |> resize(d2)
        matmul_q8q8_group3(y0, y1, y2, q, s, woff0, woff1, woff2, xq, xs, n, d0, d1, d2)
        var g0 : array<float>
        var g1 : array<float>
        var g2 : array<float>
        g0 |> resize(d0)
        g1 |> resize(d1)
        g2 |> resize(d2)
        matmul_q8q8(g0, q, s, woff0, xq, xs, n, d0)
        matmul_q8q8(g1, q, s, woff1, xq, xs, n, d1)
        matmul_q8q8(g2, q, s, woff2, xq, xs, n, d2)
        identical(t, y0, g0, "{name} group3 region 0 bit-matches its own GEMV, n={n}")
        identical(t, y1, g1, "{name} group3 region 1 bit-matches its own GEMV, n={n}")
        identical(t, y2, g2, "{name} group3 region 2 (row tail) bit-matches its own GEMV, n={n}")
        var fused : array<float>
        concat3(y0, y1, y2, fused)
        held(t, fused, ref, bar, "{name} matmul_q8q8_group3, n={n}")
        poisoned(t, fused, ref, bar, "{name} matmul_q8q8_group3, n={n}")
        if (!kernel_backend_has_wscale16()) {
            continue
        }
        var h0 : array<float>
        var h1 : array<float>
        var h2 : array<float>
        h0 |> resize(d0)
        h1 |> resize(d1)
        h2 |> resize(d2)
        matmul_q8q8_group3(h0, h1, h2, q, s16, woff0, woff1, woff2, xq, xs, n, d0, d1, d2)
        identical(t, h0, y0, "{name} group3 s16 region 0 bit-matches the f32 plane, n={n}")
        identical(t, h1, y1, "{name} group3 s16 region 1 bit-matches the f32 plane, n={n}")
        identical(t, h2, y2, "{name} group3 s16 region 2 bit-matches the f32 plane, n={n}")
    }
    restore_backend(saved)
}

[test]
def test_q8q8_group3(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
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
    var wf : array<float>
    var xf : array<float>
    fill_unit(wf, d * n, 0x8F00Au)
    fill_unit(xf, n, 0x8F00Bu)
    let wq0 <- quantize_q8_0(wf, d * n)
    var ref : array<float>
    var bar : array<float>
    ref |> resize(d)
    bar |> resize(d)
    for (r in range64(d)) {
        let rf = ref_dot_q8f_f64(wq0.q, wq0.scales, r * n, xf, n)
        ref[r] = float(rf.dot)
        bar[r] = ref_bar(rf.env, nb)
    }
    var ymm : array<float>
    var yoff : array<float>
    var ydot : array<float>
    ymm |> resize(d)
    yoff |> resize(d)
    ydot |> resize(d)
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
    poisoned(t, ydot, ref, bar, "dot_q8, n={n}")
}

[test]
def test_q8_f32_activation(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
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

def private mx4_ref_region(wn : array<uint8>; we : array<uint8>; woff : int64;
                           xq : array<int8>; xs : array<float>; t0, te, d, n : int64;
                           var ref : array<float>; var bar : array<float>) {
    let nb = n / 32l
    for (p in range64(t0, te)) {
        for (row in range64(d)) {
            let rf = ref_dot_mx4_f64(wn, we, (woff + row * n) / 2l, (woff + row * n) / 32l, xq, xs, p * n, n)
            ref[p * d + row] = float(rf.dot)
            bar[p * d + row] = ref_bar(rf.env, nb)
        }
    }
}

def private mx4_leaf_gate(t : T?; wn0 : array<uint8>; we0 : array<uint8>;
                          xq : array<int8>; xs : array<float>; d, n : int64) {
    let nb = n / 32l
    var yleaf : array<float>
    var ymm : array<float>
    yleaf |> resize(d)
    ymm |> resize(d)
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
    var wn0 : array<uint8>
    var we0 : array<uint8>
    mx4_planes(wn0, we0, nreg * d * n, nreg * d * nb)
    var xf : array<float>
    fill_unit(xf, ntok * n, 0x3A4Du)
    var xq : array<int8>
    var xs : array<float>
    quantize_rows(xf, ntok, n, xq, xs)
    var bref : array<float>
    var bbar : array<float>
    bref |> resize(ntok * d)
    bbar |> resize(ntok * d)
    mx4_ref_region(wn0, we0, 0l, xq, xs, 0l, ntok, d, n, bref, bbar)
    var gref : array<float>
    var gbar : array<float>
    gref |> resize(ntok * d)
    gbar |> resize(ntok * d)
    mx4_ref_region(wn0, we0, 0l, xq, xs, 0l, cnt0, d, n, gref, gbar)
    mx4_ref_region(wn0, we0, woff1, xq, xs, cnt0, ntok, d, n, gref, gbar)
    var goffs <- [0l, 0l, cnt0, woff1, cnt0, ntok - cnt0]
    let saved = active_kernel_backend()
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
        var wn : array<uint8>
        var we : array<uint8>
        wn := wn0
        we := we0
        if (kernel_backend_needs_repack(nm)) {
            for (r in range64(nreg)) {
                repack_mx4_weight(wn, we, r * d * n, n, d)
            }
        }
        var ymm : array<float>
        var ygmm : array<float>
        var yrow : array<float>
        ymm |> resize(ntok * d)
        ygmm |> resize(ntok * d)
        yrow |> resize(d)
        for (p in range64(ntok)) {
            let rxq <- slice_i8(xq, p * n, n)
            let rxs <- slice_f(xs, p * nb, nb)
            matmul_mx4q8(yrow, wn, we, 0l, rxq, rxs, n, d)
            for (row in range64(d)) {
                ymm[p * d + row] = yrow[row]
            }
            matmul_mx4q8(yrow, wn, we, p < cnt0 ? 0l : woff1, rxq, rxs, n, d)
            for (row in range64(d)) {
                ygmm[p * d + row] = yrow[row]
            }
        }
        var ybat : array<float>
        ybat |> resize(ntok * d)
        matmul_mx4q8_batch(ybat, wn, we, 0l, xq, xs, n, d, ntok)
        var ygb : array<float>
        ygb |> resize(ntok * d)
        matmul_mx4q8_batch_groupn(ygb, wn, we, goffs, nreg, xq, xs, n, d)
        identical(t, ybat, ymm, "{nm} matmul_mx4q8_batch bit-matches ntok x matmul_mx4q8, n={n}")
        identical(t, ygb, ygmm, "{nm} matmul_mx4q8_batch_groupn bit-matches the per-region GEMVs, n={n}")
        held(t, ybat, bref, bbar, "{nm} matmul_mx4q8_batch, n={n}")
        held(t, ygb, gref, gbar, "{nm} matmul_mx4q8_batch_groupn, n={n}")
        poisoned(t, ybat, bref, bbar, "{nm} matmul_mx4q8_batch, n={n}")
        poisoned(t, ygb, gref, gbar, "{nm} matmul_mx4q8_batch_groupn, n={n}")
    }
    restore_backend(saved)
}

[test]
def test_mx4q8_batch_family(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
        for (n in WIDTHS) {
            t |> run("mx4 batch / batch groupN vs per-token GEMVs, the scalar leaf and fp64, n={n}") @(t : T?) {
                mx4_batch_gate(t, n)
            }
        }
    }
}
