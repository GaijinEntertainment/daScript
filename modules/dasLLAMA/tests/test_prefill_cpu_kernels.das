//! The prefill and KV CPU kernels no suite gated before - the batched K-quant GEMMs and the per-position GEMV route a tier without those slots runs instead, the q8_0 cache dequant seam, the batch activation requants, and repack_kq_weight's round trip: every cell asserts on whatever tier is active, and the native batched arms ride a restored kq-backend pin; and the bf16 tile's activation plane.
options gen2
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_gen
require dasllama/dasllama_par   // set_single_thread_: a thread's walk on its calling thread
require dasllama/dasllama_convert
require dasllama/dasllama_kv_codec
require dasllama/dasllama_repack
require dasllama/dasllama_gemm_schema
require _kq_dot   // dot_kq on the run-time format
require _q4k_disk
require daslib/f16_cvt
require daslib/jobque_boost
require daslib/fio
require math
require _compares   // mismatch_exact: the plane bytes' compare

var g_taint = 0.0

[sideeffects]
def private opaque(x : float) : float => x + g_taint

def private poison_of(v : float) : float => max(0.5, abs(v) * 1.0e-3)

def private cmp_exact(t : T?; got : array<float>; want : array<float>; n : int64; tag : string) {
    var bad = 0
    var seen = 0
    let pi = n / 2l
    for (i in range64(n)) {
        if (got[i] != want[i]) {
            bad++
        }
        if (got[i] != (i == pi ? want[i] + poison_of(want[i]) : want[i])) {
            seen++
        }
    }
    t |> success(bad == 0, "{tag}: {bad} of {int(n)} elements differ")
    t |> success(seen > 0, "{tag}: the poisoned expected element must be seen, and was not")
}

def private nan_fill(var a : array<float>) {
    let nan = unsafe(reinterpret<float>(0x7FC00000u))
    for (v in a) {
        v = nan
    }
}

// The bf16 envelope: a bf16 operand sits within 2^-8 of its value, so a weight-activation
// product within 2^-7 of its own magnitude; the terms round independently, so a row's error is
// 2^-7 / sqrt(3) of the terms' rms times sqrt(n), against an output of the same order - 0.5% of
// the image's largest magnitude on these fixtures, held at 1%. A NaN is outside it.
def private cmp_bf16(t : T?; got : array<float>; want : array<float>; n, d : int64; tag : string) {
    var mag = 0.0
    for (i in range64(n)) {
        mag = max(mag, abs(want[i]))
    }
    var over = 0
    var worst = 0.0
    var worstAt = 0l
    for (i in range64(n)) {
        let df = abs(got[i] - want[i])
        if (!(df <= 1.0e-2 * mag)) {
            over++
        }
        if (!(df <= worst)) {
            worst = df
            worstAt = i
        }
    }
    to_log(LOG_INFO, "{tag}: max err {worst} at token {int(worstAt / d)} row {int(worstAt % d)}, magnitude {mag}\n")
    t |> success(over == 0, "{tag}: {over} of {int(n)} elements outside the bf16 envelope (max err {worst} at token {int(worstAt / d)} row {int(worstAt % d)}, magnitude {mag})")
    let pi = n / 2l
    t |> success(abs(got[pi] - (want[pi] + mag)) > 1.0e-2 * mag, "{tag}: the image's magnitude {mag} added to expected element {int(pi)} must land outside the envelope")
}

def private put_f16(var b : array<uint8>; off : int64; v : float) {
    let bits = f32_to_f16(v)
    b[off] = uint8(bits & 0xFFu)
    b[off + 1l] = uint8(bits >> 8u)
}

def private build_q6k_disk() : array<uint8> {
    var b : array<uint8>
    b |> resize(210l)
    for (i in range64(192l)) {
        b[i] = uint8((i * 23l + 5l) % 256l)
    }
    for (i in range64(16l)) {
        b[192l + i] = uint8((i * 41l + 11l) % 256l)
    }
    put_f16(b, 208l, 0.25)
    return <- b
}

def private build_q40_disk() : array<uint8> {
    var b : array<uint8>
    b |> resize(144l)
    for (blk in range64(8l)) {
        put_f16(b, blk * 18l, 0.0625 * float(blk + 1l))
        for (j in range64(16l)) {
            b[blk * 18l + 2l + j] = uint8(((blk * 16l + j) * 19l + 3l) % 256l)
        }
    }
    return <- b
}

def private fmt_tag(fmt : int) : string => fmt == 40 ? "q40" : "k{fmt}"

def private build_kq_region(fmt : int; var kq : array<uint8>; var ks : array<uint8>; rows, n : int64; seed : uint) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let blk <- fmt == 4 ? q4k_disk_superblock() : (fmt == 40 ? build_q40_disk() : build_q6k_disk())
    kq |> resize(rows * nsb * qsb)
    ks |> resize(rows * nsb * ssb)
    for (r in range64(rows)) {
        for (s in range64(nsb)) {
            if (fmt == 4) {
                kq_transcode_superblock(KqFmt.k4, blk, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } elif (fmt == 40) {
                kq_transcode_superblock(KqFmt.q40, blk, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            } else {
                kq_transcode_superblock(KqFmt.k6, blk, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
            }
        }
        var st = seed + uint(r) * 0x85EBCA6Bu
        for (i in range64(nsb * qsb)) {
            st = st * 1664525u + 1013904223u
            kq[r * nsb * qsb + i] = uint8(uint(kq[r * nsb * qsb + i]) ^ (st >> 16u))
        }
    }
}

def private build_acts(var src : array<float>; var xq : array<int8>; var xs : array<float>; var xbs : array<int>; ntok, n : int64) {
    src |> resize(ntok * n)
    for (i in range64(ntok * n)) {
        src[i] = opaque(0.9 * sin(float(i) * 0.27) - 0.5 * cos(float(i) * 0.061))
    }
    xq |> resize(ntok * n)
    xs |> resize(ntok * n / 256l)
    xbs |> resize(ntok * n / 16l)
    unsafe {
        for (p in range64(ntok)) {
            quantize_q8_k_into_ptr(addr(src[p * n]), n, addr(xq[0]), addr(xs[0]), addr(xbs[0]),
                p * n, p * (n / 256l), p * (n / 16l))
        }
    }
}

def private kq_row_dot(fmt : int; kq : array<uint8>; ks : array<uint8>; row, nsb : int64;
                       xq : array<int8>; xs : array<float>; xbs : array<int>;
                       xoff, soff, boff, n : int64) : float {
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        let kqr = addr<uint8 const?>(kq[row * nsb * qsb])
        let ksr = addr<uint8 const?>(ks[row * nsb * ssb])
        let xqp = addr<int8 const?>(xq[xoff])
        let xsp = addr<float const?>(xs[soff])
        let xbp = addr<int const?>(xbs[boff])
        return dot_kq(kqr, ksr, xqp, xsp, xbp, n, kq_fmt_of_id(fmt, "disk_dot"))
    }
}

def private gemv_gen_rows(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?;
                          xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) {
    if (fmt == 4) {
        k4q8_gemv_gen(yp, kqp, ksp, xqp, xsp, xbsp, n, rb, re)
    } elif (fmt == 40) {
        q40q8_gemv_gen(yp, kqp, ksp, xqp, xsp, xbsp, n, rb, re)
    } else {
        k6q8_gemv_gen(yp, kqp, ksp, xqp, xsp, xbsp, n, rb, re)
    }
}

def private grp_row_dot(fmt : int; kqg : uint8 const?; ksg : uint8 const?; r, mr : int64;
                        xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64) : float {
    if (fmt == 40) {
        return q40_grp_row_dot(kqg, ksg, r, mr, xqp, xsp, xbsp, n)
    }
    return kq_grp_row_dot(int64(fmt), kqg, ksg, r, mr, xqp, xsp, xbsp, n)
}

def private repack_grp(fmt : int; var kq : array<uint8>; var ks : array<uint8>; n, d, mr : int64) {
    unsafe {
        if (fmt == 4) {
            repack_kq_grp(KqFmt.k4, addr(kq[0]), addr(ks[0]), n, d, mr)
        } elif (fmt == 40) {
            repack_kq_grp(KqFmt.q40, addr(kq[0]), addr(ks[0]), n, d, mr)
        } else {
            repack_kq_grp(KqFmt.k6, addr(kq[0]), addr(ks[0]), n, d, mr)
        }
    }
}

def private gen_backend_name() : string => get_architecture_name() == "arm64" ? "arm64-gen" : "x64-gen"

def private quant_one(src : array<float>; poff, n : int64; var q1 : array<int8>; var s1 : array<float>; var b1 : array<int>) {
    unsafe {
        quantize_q8_k_into_ptr(addr(src[poff]), n, addr(q1[0]), addr(s1[0]), addr(b1[0]), 0l, 0l, 0l)
    }
}

def private alloc_row_scratch(var q1 : array<int8>; var s1 : array<float>; var b1 : array<int>; n : int64) {
    q1 |> resize(n)
    s1 |> resize(n / 256l)
    b1 |> resize(n / 16l)
}

def private kq_batch_gate(t : T?; fmt : int; n, d, ntok : int64) {
    let nsb = n / 256l
    var kq : array<uint8>
    var ks : array<uint8>
    build_kq_region(fmt, kq, ks, d, n, 0x51ED270Bu)
    var src : array<float>
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    build_acts(src, xq, xs, xbs, ntok, n)
    var want : array<float>
    want |> resize(ntok * d)
    for (p in range64(ntok)) {
        for (r in range64(d)) {
            want[p * d + r] = kq_row_dot(fmt, kq, ks, r, nsb, xq, xs, xbs, p * n, p * nsb, p * (n / 16l), n)
        }
    }
    var q1 : array<int8>
    var s1 : array<float>
    var b1 : array<int>
    alloc_row_scratch(q1, s1, b1, n)
    var y : array<float>
    y |> resize(ntok * d)
    for (p in range64(ntok)) {
        quant_one(src, p * n, n, q1, s1, b1)
        matmul_kq(fmt, y, kq, ks, 0l, q1, s1, b1, n, d, p * d)
    }
    cmp_exact(t, y, want, ntok * d,
        "{fmt_tag(fmt)} serial per-position GEMV (n={int(n)} d={int(d)} ntok={int(ntok)}) vs per-(token,row) disk dots")
    let prev = active_kernel_backend()
    pin_kernel_backend(gen_backend_name())
    if (kernel_backend_has_kq() && kernel_backend_has_kq_batch()) {
        repack_kq_weight(fmt, kq, ks, 0l, n, d)
        var ybatch : array<float>
        ybatch |> resize(ntok * d)
        nan_fill(ybatch)
        matmul_kq_batch(fmt, ybatch, kq, ks, 0l, xq, xs, xbs, n, d, ntok)
        var yser : array<float>
        yser |> resize(ntok * d)
        nan_fill(yser)
        for (p in range64(ntok)) {
            quant_one(src, p * n, n, q1, s1, b1)
            matmul_kq_active(fmt, yser, kq, ks, 0l, q1, s1, b1, n, d, p * d)
        }
        if (kq_tileform_of(fmt) > 0) {
            cmp_bf16(t, ybatch, want, ntok * d, d,
                "{fmt_tag(fmt)} native kq batch on the bf16 tile vs per-(token,row) disk dots (n={int(n)} d={int(d)} ntok={int(ntok)})")
            cmp_bf16(t, ybatch, yser, ntok * d, d,
                "{fmt_tag(fmt)} native kq batch on the bf16 tile vs ntok x matmul_kq_active (n={int(n)} d={int(d)} ntok={int(ntok)})")
        } else {
            cmp_exact(t, ybatch, yser, ntok * d,
                "{fmt_tag(fmt)} native kq batch is bit-for-bit ntok x matmul_kq_active (ntok={int(ntok)})")
        }
        delete ybatch
        delete yser
    }
    clear_kernel_backend_pin()
    select_kernel_backend(prev)
    delete kq
    delete ks
    delete src
    delete xq
    delete xs
    delete xbs
    delete want
    delete q1
    delete s1
    delete b1
    delete y
}

[test]
def test_kq_batch(t : T?) {
    for (fmt in [4, 6, 40]) {
        for (ntok in [1l, 5l, 13l]) {
            t |> run("{fmt_tag(fmt)} batched kq GEMM at ntok={int(ntok)}: the tier's route matches the per-token disk dots") @(t : T?) {
                kq_batch_gate(t, fmt, 512l, 32l, ntok)
            }
        }
        t |> run("{fmt_tag(fmt)} batched kq GEMM at the multi-superblock shape (n=1024 d=64 ntok=6)") @(t : T?) {
            kq_batch_gate(t, fmt, 1024l, 64l, 6l)
        }
        t |> run("{fmt_tag(fmt)} batched kq GEMM at n=512 d=48 ntok=67") @(t : T?) {
            kq_batch_gate(t, fmt, 512l, 48l, 67l)
        }
    }
}

def private bf16_value(h : uint16) : float => unsafe(reinterpret<float>(uint(h) << 16u))

def private xbf16_gate(t : T?; n, ntok : int64) {
    var src : array<float>
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    build_acts(src, xq, xs, xbs, ntok, n)
    var stale : array<int8>
    stale |> resize(ntok * n)
    for (sv, qv in stale, xq) {
        sv = int8(int(qv) > 0 ? int(qv) - 1 : int(qv) + 1)
    }
    var bad = 0
    var worst = 0.0
    var worstAt = 0l
    var truncatedSeen = 0
    unsafe {
        xbf16_build(addr<int8 const?>(stale[0]), addr<float const?>(xs[0]), n, ntok, 256l)
        let hp = reinterpret<uint16 const?>(xbf16_build(addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, ntok, 256l))
        t |> success((int64(intptr(hp)) & 63l) == 0l, "xbf16_build hands back a 64-byte aligned plane")
        for (p in range64(ntok)) {
            for (i in range64(n)) {
                let x = float(xq[p * n + i]) * xs[p * (n / 256l) + i / 256l]
                let df = abs(bf16_value(hp[p * n + i]) - x)
                if (!(df <= abs(x) * 0.00390625)) {
                    bad++
                }
                if (!(df <= worst)) {
                    worst = df
                    worstAt = p * n + i
                }
                let truncated = uint16(reinterpret<uint>(x) >> 16u)
                if (!(abs(bf16_value(truncated) - x) <= abs(x) * 0.00390625)) {
                    truncatedSeen++
                }
            }
        }
    }
    to_log(LOG_INFO, "xbf16_build (n={int(n)} ntok={int(ntok)}): max err {worst} at token {int(worstAt / n)} element {int(worstAt % n)}\n")
    t |> success(bad == 0, "xbf16_build (n={int(n)} ntok={int(ntok)}): {bad} of {int(ntok * n)} elements off q times its superblock scale rounded to bf16, max err {worst} at token {int(worstAt / n)} element {int(worstAt % n)}")
    delete stale
    t |> success(truncatedSeen > 0, "xbf16_build: a truncated bf16 must be seen past half a step, and was not")
    delete src
    delete xq
    delete xs
    delete xbs
}

// Two contexts build their planes at once, each over its own activation row: a plane is a
// context's own, so neither build reads the other's values. The contexts are two threads', the
// shape two concurrent inferences take; a team lane is no such context (the fork pool's lanes
// own no globals). A thread waits for the other to start, then rebuilds and re-reads its plane
// `rounds` times.
def private plane_thread(lane : int; n : int64; rounds : int; xqp : int8 const?; xsp : float const?; wp : uint16 const?;
                         var sp : int?; var dp : int?; var bp : int64?; var pp : int64?; var wg : JobStatus?) {
    new_thread() @() {
        unsafe {
            sp[lane] = 1
            let t0 = ref_time_ticks()
            while (sp[1 - lane] == 0 && get_time_usec(t0) < 2000000) {
                sleep(1u)
            }
            for (_r in range(rounds)) {
                let hp = reinterpret<uint16 const?>(xbf16_build(xqp + int64(lane) * n, xsp + int64(lane) * (n / 256l), n, 1l, 256l))
                pp[lane] = int64(intptr(hp))
                for (i in range64(n)) {
                    if (hp[i] != wp[int64(lane) * n + i]) {
                        bp[lane]++
                    }
                }
                dp[lane]++
            }
        }
        wg |> notify_and_release
    }
}

def private xbf16_two_contexts(t : T?; n : int64; rounds : int) {
    var src : array<float>
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    build_acts(src, xq, xs, xbs, 2l, n)
    var want : array<uint16>
    want |> resize(2l * n)
    unsafe {
        for (p in range64(2l)) {
            let hp = reinterpret<uint16 const?>(xbf16_build(addr<int8 const?>(xq[p * n]), addr<float const?>(xs[p * (n / 256l)]), n, 1l, 256l))
            for (i in range64(n)) {
                want[p * n + i] = hp[i]
            }
        }
    }
    var rowsDiffer = 0
    for (i in range64(n)) {
        rowsDiffer += want[i] != want[n + i] ? 1 : 0
    }
    var started : int[2]
    var done : int[2]
    var bad : int64[2]
    var plane : int64[2]
    with_job_que() {
        with_wait_group(2) $(wg) {
            unsafe {
                for (lane in range(2)) {
                    plane_thread(lane, n, rounds, addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<uint16 const?>(want[0]),
                        addr(started[0]), addr(done[0]), addr(bad[0]), addr(plane[0]), wg)
                }
            }
        }
    }
    to_log(LOG_INFO, "xbf16_build in two threads (n={int(n)}, {rounds} rounds each): {bad[0]} and {bad[1]} elements off, planes at {plane[0]} and {plane[1]}\n")
    t |> equal(done[0] + done[1], 2 * rounds, "both threads ran every round")
    t |> success(rowsDiffer > 0, "the two threads' activation rows differ ({rowsDiffer} of {int(n)} elements), so a shared plane would show")
    t |> success(plane[0] != plane[1], "the two threads' planes sit at distinct addresses")
    t |> equal(bad[0], 0l, "thread 0 read its own plane in every round")
    t |> equal(bad[1], 0l, "thread 1 read its own plane in every round")
    delete src
    delete xq
    delete xs
    delete xbs
    delete want
}

def private walk_seed(lane : int) : uint => 0x51ED270Bu + uint(lane) * 0x9E3779B9u

def private walk_inputs(fmt : int; n, d, ntok : int64; seed : uint; var kq : array<uint8>; var ks : array<uint8>;
                        var src : array<float>; var xq : array<int8>; var xs : array<float>; var xbs : array<int>) {
    build_kq_region(fmt, kq, ks, d, n, seed)
    build_acts(src, xq, xs, xbs, ntok, n)
    repack_kq_weight(fmt, kq, ks, 0l, n, d)
}

// Two contexts run the batch walk at once, each over its own planes: the walk's panels are a
// context's own scratch, so neither walk reads a panel the other built. The contexts are two
// threads', each walking on its calling thread (one dispatch slot, the same slot in both); a
// thread builds the inputs the main thread built for its lane, waits for the other to start,
// then walks `rounds` times and counts the elements off the main thread's walk.
def private walk_thread(lane : int; fmt : int; n, d, ntok : int64; rounds : int; wp : float const?;
                        var sp : int?; var dp : int?; var bp : int64?; var wg : JobStatus?) {
    new_thread() @() {
        set_single_thread_(true)
        pin_kernel_backend(gen_backend_name())
        var kq : array<uint8>
        var ks : array<uint8>
        var src : array<float>
        var xq : array<int8>
        var xs : array<float>
        var xbs : array<int>
        walk_inputs(fmt, n, d, ntok, walk_seed(lane), kq, ks, src, xq, xs, xbs)
        var y : array<float>
        y |> resize(ntok * d)
        unsafe {
            sp[lane] = 1
            let t0 = ref_time_ticks()
            while (sp[1 - lane] == 0 && get_time_usec(t0) < 2000000) {
                sleep(1u)
            }
            for (_r in range(rounds)) {
                nan_fill(y)
                matmul_kq_batch(fmt, y, kq, ks, 0l, xq, xs, xbs, n, d, ntok)
                for (i in range64(ntok * d)) {
                    if (y[i] != wp[int64(lane) * ntok * d + i]) {
                        bp[lane]++
                    }
                }
                dp[lane]++
            }
        }
        delete kq
        delete ks
        delete src
        delete xq
        delete xs
        delete xbs
        delete y
        wg |> notify_and_release
    }
}

def private walk_two_contexts(t : T?; fmt : int; n, d, ntok : int64; rounds : int) {
    let prev = active_kernel_backend()
    pin_kernel_backend(gen_backend_name())
    if (!kernel_backend_has_kq() || !kernel_backend_has_kq_batch()) {
        clear_kernel_backend_pin()
        select_kernel_backend(prev)
        t |> skip("{gen_backend_name()} carries no kq batch kernel on this box")
        return
    }
    var want : array<float>
    want |> resize(2l * ntok * d)
    for (lane in range(2)) {
        var kq : array<uint8>
        var ks : array<uint8>
        var src : array<float>
        var xq : array<int8>
        var xs : array<float>
        var xbs : array<int>
        walk_inputs(fmt, n, d, ntok, walk_seed(lane), kq, ks, src, xq, xs, xbs)
        var y : array<float>
        y |> resize(ntok * d)
        nan_fill(y)
        matmul_kq_batch(fmt, y, kq, ks, 0l, xq, xs, xbs, n, d, ntok)
        for (i in range64(ntok * d)) {
            want[int64(lane) * ntok * d + i] = y[i]
        }
        delete kq
        delete ks
        delete src
        delete xq
        delete xs
        delete xbs
        delete y
    }
    clear_kernel_backend_pin()
    select_kernel_backend(prev)
    var lanesDiffer = 0
    var finite = 0
    for (i in range64(ntok * d)) {
        lanesDiffer += want[i] != want[ntok * d + i] ? 1 : 0
        finite += is_finite(want[i]) ? 1 : 0
    }
    var started : int[2]
    var done : int[2]
    var bad : int64[2]
    with_job_que() {
        with_wait_group(2) $(wg) {
            unsafe {
                for (lane in range(2)) {
                    walk_thread(lane, fmt, n, d, ntok, rounds, addr<float const?>(want[0]), addr(started[0]), addr(done[0]), addr(bad[0]), wg)
                }
            }
        }
    }
    to_log(LOG_INFO, "{fmt_tag(fmt)} batch walk in two threads (n={int(n)} d={int(d)} ntok={int(ntok)}, {rounds} rounds each, a panel of {int(kq_walk_panel_bytes(fmt, n, kq_layout_of(fmt)))} bytes): {bad[0]} and {bad[1]} elements off\n")
    t |> success(kq_walk_panel_bytes(fmt, n, kq_layout_of(fmt)) > 0l, "{fmt_tag(fmt)} walks through a panel on this stamp")
    t |> equal(finite, int(ntok * d), "the main thread's walk wrote every element")
    t |> equal(done[0] + done[1], 2 * rounds, "both threads ran every round")
    t |> success(lanesDiffer > 0, "the two threads' outputs differ ({lanesDiffer} of {int(ntok * d)} elements), so a shared panel would show")
    t |> equal(bad[0], 0l, "thread 0 read its own panels in every round")
    t |> equal(bad[1], 0l, "thread 1 read its own panels in every round")
    delete want
}

[test]
def test_walk_panels(t : T?) {
    t |> run("two threads walking at once each read their own panels") @(t : T?) {
        walk_two_contexts(t, 6, 512l, 48l, 67l, 200)
    }
}

def private bf16_block_gate(t : T?) {
    let knob = get_q8_token_block()
    let budget = get_q8_l2_budget()
    let ts = amx_bf16_tile_tokens()
    let cases = [(1l, 512l), (128l, 512l), (500l, 512l), (512l, 512l), (1000l, 992l), (1024l, 1024l)]
    for (c in cases) {
        set_q8_token_block(c._0)
        t |> equal(bf16_token_block(), c._1, "the knob at {int(c._0)} reads a block of {int(c._1)}")
        t |> equal(bf16_token_block() % ts, 0l, "the block at knob {int(c._0)} is a whole number of {int(ts)}-token tiles")
    }
    set_q8_token_block(512l)
    set_q8_l2_budget(4096l * 512l)
    t |> equal(effective_token_block(512l, 9728l), 215l, "the L2 budget cuts the int8 walk's block at n=9728")
    t |> equal(bf16_token_block(), 512l, "the same budget leaves the bf16 walk's block whole")
    set_q8_l2_budget(budget)
    set_q8_token_block(knob)
}

[test]
def test_bf16_token_block(t : T?) {
    t |> run("the bf16 walk's block is the knob or the floor in whole tiles, under no L2 clamp") @(t : T?) {
        bf16_block_gate(t)
    }
}

[test]
def test_xbf16_build(t : T?) {
    t |> run("the bf16 activation plane is q times its scale, rounded to nearest, on a cache line") @(t : T?) {
        xbf16_gate(t, 512l, 3l)
    }
    t |> run("two threads building at once each read their own plane") @(t : T?) {
        xbf16_two_contexts(t, 8192l, 400)
    }
}

def private groupn_fallback(t : T?; fmt : int; kq : array<uint8>; ks : array<uint8>; src : array<float>;
                            cnts : array<int64>; n, d : int64; want : array<float>; nk : int64) {
    var q1 : array<int8>
    var s1 : array<float>
    var b1 : array<int>
    alloc_row_scratch(q1, s1, b1, n)
    var y : array<float>
    y |> resize(nk * d)
    var row0 = 0l
    for (r in range64(long_length(cnts))) {
        for (tk in range64(row0, row0 + cnts[r])) {
            quant_one(src, tk * n, n, q1, s1, b1)
            matmul_kq(fmt, y, kq, ks, r * d * n, q1, s1, b1, n, d, tk * d)
        }
        row0 += cnts[r]
    }
    cmp_exact(t, y, want, nk * d,
        "{fmt_tag(fmt)} per-expert per-token GEMV route (what a tier with no fused kq slot runs) vs per-(region,token) disk dots")
    delete q1
    delete s1
    delete b1
    delete y
}

def private groupn_fused(t : T?; fmt : int; var kq : array<uint8>; var ks : array<uint8>;
                         offs : array<int64>; cnts : array<int64>; xq : array<int8>; xs : array<float>;
                         xbs : array<int>; n, d, nk : int64; want : array<float>) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let nreg = long_length(cnts)
    for (r in range64(nreg)) {
        repack_kq_weight(fmt, kq, ks, r * d * n, n, d)
    }
    var yb : array<float>
    yb |> resize(nk * d)
    nan_fill(yb)
    matmul_kq_batch_groupn(fmt, yb, kq, ks, offs, nreg, xq, xs, xbs, n, d)
    var wantg : array<float>
    wantg |> resize(nk * d)
    var row0 = 0l
    unsafe {
        for (r in range64(nreg)) {
            let kqr = addr<uint8 const?>(kq[r * d * nsb * qsb])
            let ksr = addr<uint8 const?>(ks[r * d * nsb * ssb])
            for (tk in range64(row0, row0 + cnts[r])) {
                gemv_gen_rows(fmt, addr(wantg[tk * d]), kqr, ksr, addr<int8 const?>(xq[tk * n]),
                    addr<float const?>(xs[tk * nsb]), addr<int const?>(xbs[tk * (n / 16l)]), n, 0l, d)
            }
            row0 += cnts[r]
        }
    }
    var longest = 0l
    for (c in cnts) {
        longest = max(longest, c)
    }
    // a bf16 tile walk sums a region of a tile's tokens or more in the tile; under that the rows core serves every token
    if (kq_tileform_of(fmt) > 0 && longest >= amx_bf16_tile_tokens()) {
        cmp_bf16(t, yb, want, nk * d, d,
            "{fmt_tag(fmt)} fused kq batch groupn on the bf16 tile vs per-(region,token) disk dots")
        cmp_bf16(t, yb, wantg, nk * d, d,
            "{fmt_tag(fmt)} fused kq batch groupn on the bf16 tile vs per-(region, token) rows-core calls")
    } else {
        cmp_exact(t, yb, wantg, nk * d,
            "{fmt_tag(fmt)} fused kq batch groupn bit-matches per-(region, token) rows-core calls")
    }
    delete yb
    delete wantg
}

def private kq_batch_groupn_gate(t : T?; fmt : int; n, d : int64; longRuns : bool = false) {
    var cnts <- longRuns ? [3l, 35l, 67l] : [1l, 4l, 6l]
    let nreg = long_length(cnts)
    var nk = 0l
    for (c in cnts) {
        nk += c
    }
    let nsb = n / 256l
    var kq : array<uint8>
    var ks : array<uint8>
    build_kq_region(fmt, kq, ks, nreg * d, n, 0x7F4A7C15u)
    var src : array<float>
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    build_acts(src, xq, xs, xbs, nk, n)
    var offs : array<int64>
    offs |> reserve(nreg * 3l)
    var want : array<float>
    want |> resize(nk * d)
    var row0 = 0l
    for (r in range64(nreg)) {
        offs |> push(r * d * n)
        offs |> push(row0)
        offs |> push(cnts[r])
        for (tk in range64(row0, row0 + cnts[r])) {
            for (row in range64(d)) {
                want[tk * d + row] = kq_row_dot(fmt, kq, ks, r * d + row, nsb, xq, xs, xbs,
                    tk * n, tk * nsb, tk * (n / 16l), n)
            }
        }
        row0 += cnts[r]
    }
    groupn_fallback(t, fmt, kq, ks, src, cnts, n, d, want, nk)
    let prev = active_kernel_backend()
    pin_kernel_backend(gen_backend_name())
    if (kernel_backend_has_kq() && kernel_backend_has_kq_batch_groupn()) {
        groupn_fused(t, fmt, kq, ks, offs, cnts, xq, xs, xbs, n, d, nk, want)
    }
    clear_kernel_backend_pin()
    select_kernel_backend(prev)
    delete cnts
    delete kq
    delete ks
    delete src
    delete xq
    delete xs
    delete xbs
    delete offs
    delete want
}

[test]
def test_kq_batch_groupn_portable(t : T?) {
    for (fmt in [4, 6, 40]) {
        t |> run("{fmt_tag(fmt)} fused MoE-prefill region walk (runs 1/4/6) on whatever backend is active") @(t : T?) {
            kq_batch_groupn_gate(t, fmt, 512l, 32l)
        }
        t |> run("{fmt_tag(fmt)} fused MoE-prefill region walk (runs 3/35/67, n=512 d=48) on whatever backend is active") @(t : T?) {
            kq_batch_groupn_gate(t, fmt, 512l, 48l, true)
        }
    }
    t |> run("k4 fused region walk at the expert-slice shape (n=1024 d=128)") @(t : T?) {
        kq_batch_groupn_gate(t, 4, 1024l, 128l)
    }
}

def private grp_rows_after_repack(fmt : int; kq : array<uint8>; ks : array<uint8>; nsb, d, mr : int64;
                                  xq : array<int8>; xs : array<float>; xbs : array<int>; n : int64;
                                  var got : array<float>) {
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    got |> resize(d)
    unsafe {
        for (r in range64(d)) {
            let g = r / mr
            got[r] = grp_row_dot(fmt, addr<uint8 const?>(kq[g * mr * nsb * qsb]),
                addr<uint8 const?>(ks[g * mr * nsb * ssb]), r % mr, mr,
                addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<int const?>(xbs[0]), n)
        }
    }
}

def private repack_seam(t : T?; fmt : int; kq : array<uint8>; ks : array<uint8>;
                        kqg : array<uint8>; ksg : array<uint8>; want : array<float>;
                        nsb, n, d : int64; xq : array<int8>; xs : array<float>; xbs : array<int>) {
    var kqs : array<uint8>
    kqs := kq
    var kss : array<uint8>
    kss := ks
    let prev = active_kernel_backend()
    pin_kernel_backend(gen_backend_name())
    let has_kq = kernel_backend_has_kq()
    repack_kq_weight(fmt, kqs, kss, 0l, n, d)
    if (has_kq) {
        let moved = mismatch_exact(kqs, kqg) + mismatch_exact(kss, ksg)
        t |> success(moved == 0, "repack_kq_weight lands on the grp<{int(kq_layout_of(fmt))}> layout the readers expect ({moved} bytes differ)")
        var got : array<float>
        grp_rows_after_repack(fmt, kqs, kss, nsb, d, kq_layout_of(fmt), xq, xs, xbs, n, got)
        cmp_exact(t, got, want, d, "{fmt_tag(fmt)} row dots through repack_kq_weight's own planes")
        delete got
    } else {
        let moved = mismatch_exact(kqs, kq) + mismatch_exact(kss, ks)
        t |> success(moved == 0, "with no kq slot on the tier repack_kq_weight is the registered identity ({moved} bytes moved)")
        var got : array<float>
        got |> resize(d)
        for (r in range64(d)) {
            got[r] = kq_row_dot(fmt, kqs, kss, r, nsb, xq, xs, xbs, 0l, 0l, 0l, n)
        }
        cmp_exact(t, got, want, d, "{fmt_tag(fmt)} disk row dots survive the identity repack")
    }
    clear_kernel_backend_pin()
    select_kernel_backend(prev)
    delete kqs
    delete kss
}

def private repack_gate(t : T?; fmt : int; n, d : int64) {
    let nsb = n / 256l
    var kq : array<uint8>
    var ks : array<uint8>
    build_kq_region(fmt, kq, ks, d, n, 0x9E3779B9u)
    var src : array<float>
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    build_acts(src, xq, xs, xbs, 1l, n)
    var want : array<float>
    want |> resize(d)
    for (r in range64(d)) {
        want[r] = kq_row_dot(fmt, kq, ks, r, nsb, xq, xs, xbs, 0l, 0l, 0l, n)
    }
    let mr = kq_layout_of(fmt)
    var kqg : array<uint8>
    kqg := kq
    var ksg : array<uint8>
    ksg := ks
    repack_grp(fmt, kqg, ksg, n, d, mr)
    var got : array<float>
    grp_rows_after_repack(fmt, kqg, ksg, nsb, d, mr, xq, xs, xbs, n, got)
    cmp_exact(t, got, want, d, "{fmt_tag(fmt)} grp<{int(mr)}> row dots after the repack (n={int(n)} d={int(d)})")
    repack_seam(t, fmt, kq, ks, kqg, ksg, want, nsb, n, d, xq, xs, xbs)
    delete kq
    delete ks
    delete kqg
    delete ksg
    delete src
    delete xq
    delete xs
    delete xbs
    delete want
    delete got
}

[test]
def test_repack_kq_weight(t : T?) {
    for (fmt in [4, 6, 40]) {
        t |> run("{fmt_tag(fmt)} the grp repack and repack_kq_weight preserve every row's dot") @(t : T?) {
            repack_gate(t, fmt, 512l, 32l)
            repack_gate(t, fmt, 1024l, 64l)
        }
    }
}

def private q8kv_block_ref(blob : array<uint8>; base, n : int64; var want : array<float>) {
    want |> resize(n)
    for (bi in range64(n / 32l)) {
        let bb = base + bi * 34l
        let d = f16_to_f32(uint(blob[bb]) | (uint(blob[bb + 1l]) << 8u))
        for (k in range64(32l)) {
            want[bi * 32l + k] = float(int8(blob[bb + 2l + k])) * d
        }
    }
}

def private kv_q8_gate(t : T?; n : int64) {
    let nb = n / 32l
    var src : array<float>
    src |> resize(n)
    for (i in range64(n)) {
        src[i] = opaque(2.5 * sin(float(i) * 0.37 + 0.11))
    }
    var blob : array<uint8>
    blob |> resize(2l * nb * 34l)
    unsafe {
        quantize_q8kv_row(addr(blob[0]), addr<float const?>(src[0]), n)
        quantize_q8kv_row(addr(blob[nb * 34l]), addr<float const?>(src[0]), n)
    }
    var want : array<float>
    q8kv_block_ref(blob, 0l, n, want)
    var got : array<float>
    got |> resize(n)
    unsafe {
        cvt_q8kv_to_f32(addr(got[0]), addr<uint8 const?>(blob[0]), n)
    }
    cmp_exact(t, got, want, n, "cvt_q8kv_to_f32 vs the per-block scalar reference")
    var got2 : array<float>
    got2 |> resize(n)
    unsafe {
        let bp = addr<uint8 const?>(blob[0])
        kv_row_to_f32(addr(got2[0]), bp, kv_head_off(bp, n), n)
    }
    cmp_exact(t, got2, want, n, "kv_row_to_f32(q8_0) at the second row's byte base")
    var rt_bad = 0
    for (bi in range64(nb)) {
        let d = f16_to_f32(uint(blob[bi * 34l]) | (uint(blob[bi * 34l + 1l]) << 8u))
        for (k in range64(32l)) {
            if (abs(src[bi * 32l + k] - want[bi * 32l + k]) > d * 0.57 + 1.0e-7) {
                rt_bad++
            }
        }
    }
    t |> success(rt_bad == 0, "every dequantized element within half a step of the source ({rt_bad} outside)")
    delete src
    delete blob
    delete want
    delete got
    delete got2
}

def private kv_wide_gate(t : T?; n : int64) {
    var src : array<float>
    src |> resize(n)
    for (i in range64(n)) {
        src[i] = opaque(1.75 * sin(float(i) * 0.21 + 0.4))
    }
    var wide : array<float>
    wide |> resize(2l * n)
    for (i in range64(n)) {
        wide[n + i] = src[i]
    }
    var got : array<float>
    got |> resize(n)
    unsafe {
        let wp = addr<float const?>(wide[0])
        kv_row_to_f32(addr(got[0]), wp, kv_head_off(wp, n), n)
    }
    cmp_exact(t, got, src, n, "kv_row_to_f32(f32) is the plain row copy")
    var half : array<uint16>
    half |> resize(2l * n)
    unsafe {
        cvt_f32_to_f16(addr(half[n]), addr<float const?>(src[0]), n)
    }
    var want : array<float>
    want |> resize(n)
    for (i in range64(n)) {
        want[i] = f16_to_f32(uint(half[n + i]))
    }
    var got2 : array<float>
    got2 |> resize(n)
    unsafe {
        let hp = addr<uint16 const?>(half[0])
        kv_row_to_f32(addr(got2[0]), hp, kv_head_off(hp, n), n)
    }
    cmp_exact(t, got2, want, n, "kv_row_to_f32(f16) widens the row it was pointed at")
    delete src
    delete wide
    delete got
    delete half
    delete want
    delete got2
}

[test]
def test_kv_q8_row_to_f32(t : T?) {
    t |> run("cvt_q8kv_to_f32 and kv_row_to_f32 invert quantize_q8kv_row's stored blocks") @(t : T?) {
        kv_q8_gate(t, 256l)
        kv_q8_gate(t, 64l)
    }
    t |> run("kv_row_to_f32's f32 and f16 overloads carry a row at a non-zero base") @(t : T?) {
        kv_wide_gate(t, 128l)
    }
}

def private ref_q8k_row(src : array<float>; base, n : int64; var q : array<int8>; var sc : array<float>; var bs : array<int>) {
    let nsb = n / 256l
    q |> resize(n)
    sc |> resize(nsb)
    bs |> resize(n / 16l)
    for (s in range64(nsb)) {
        var amax = 0.0
        for (i in range64(256l)) {
            amax = max(amax, abs(src[base + s * 256l + i]))
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        sc[s] = d
        for (i in range64(256l)) {
            q[s * 256l + i] = int8(round(src[base + s * 256l + i] * id))
        }
        for (h in range64(16l)) {
            var acc = 0
            for (i in range64(16l)) {
                acc += int(q[s * 256l + h * 16l + i])
            }
            bs[s * 16l + h] = acc
        }
    }
}

def private fill_acts_flat(var x : array<float>; npos, n : int64) {
    x |> resize(npos * n)
    for (i in range64(npos * n)) {
        x[i] = opaque(1.4 * sin(float(i) * 0.19 + 0.3) - 0.6 * cos(float(i) * 0.083))
    }
}

def private requant_q8_gate(t : T?; par : bool; npos, n : int64) {
    var x : array<float>
    fill_acts_flat(x, npos, n)
    let nb = n / 32l
    var xqb : array<int8>
    var xsb : array<float>
    xqb |> resize(npos * n)
    xsb |> resize(npos * nb)
    requant_rows_q8(x, n, npos, xqb, xsb, par)
    var bad = 0
    var seen = 0
    for (p in range64(npos)) {
        var row : array<float>
        row |> resize(n)
        for (i in range64(n)) {
            row[i] = x[p * n + i]
        }
        var qt <- quantize_q8_0(row, n)
        for (i in range64(n)) {
            if (xqb[p * n + i] != qt.q[i]) {
                bad++
            }
            if (p == 0l && i == n / 2l && int(xqb[p * n + i]) != int(qt.q[i]) + 7) {
                seen++
            }
        }
        for (b in range64(nb)) {
            if (xsb[p * nb + b] != qt.scales[b]) {
                bad++
            }
        }
        delete qt
        delete row
    }
    t |> success(bad == 0, "requant_rows_q8 (par={par}) bit-matches quantize_q8_0 per row ({bad} differ)")
    t |> success(seen > 0, "the +7 poison on one expected quant must be seen (par={par})")
    delete x
    delete xqb
    delete xsb
}

def private requant_q8k_gate(t : T?; par : bool; npos, n : int64) {
    var x : array<float>
    fill_acts_flat(x, npos, n)
    let nsb = n / 256l
    var xqb : array<int8>
    var xsb : array<float>
    var xbsb : array<int>
    xqb |> resize(npos * n)
    xsb |> resize(npos * nsb)
    xbsb |> resize(npos * n / 16l)
    requant_rows_q8k_bs(x, n, npos, xqb, xsb, xbsb, par)
    var bad = 0
    var seen = 0
    for (p in range64(npos)) {
        var q : array<int8>
        var sc : array<float>
        var bs : array<int>
        ref_q8k_row(x, p * n, n, q, sc, bs)
        for (i in range64(n)) {
            if (xqb[p * n + i] != q[i]) {
                bad++
            }
        }
        for (s in range64(nsb)) {
            if (xsb[p * nsb + s] != sc[s]) {
                bad++
            }
        }
        for (h in range64(n / 16l)) {
            if (xbsb[p * (n / 16l) + h] != bs[h]) {
                bad++
            }
            if (p == 0l && h == 0l && xbsb[p * (n / 16l) + h] != bs[h] + 3) {
                seen++
            }
        }
        delete q
        delete sc
        delete bs
    }
    t |> success(bad == 0, "requant_rows_q8k_bs (par={par}) matches the in-test Q8_K reference ({bad} differ)")
    t |> success(seen > 0, "the +3 poison on one expected per-16 sum must be seen (par={par})")
    delete x
    delete xqb
    delete xsb
    delete xbsb
}

[test]
def test_requant_rows(t : T?) {
    for (par in [false, true]) {
        t |> run("requant_rows_q8 par={par}: every row is quantize_q8_0 of that row") @(t : T?) {
            requant_q8_gate(t, par, 5l, 512l)
            requant_q8_gate(t, par, 1l, 1024l)
        }
        t |> run("requant_rows_q8k_bs par={par}: per-256 scales and per-16 sums against an in-test reference") @(t : T?) {
            requant_q8k_gate(t, par, 5l, 512l)
            requant_q8k_gate(t, par, 1l, 1024l)
        }
    }
}
