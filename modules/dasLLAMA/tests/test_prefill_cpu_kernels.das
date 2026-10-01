//! The prefill and KV CPU kernels no suite gated before - the batched K-quant GEMMs and the per-position GEMV route a tier without those slots runs instead, the q8_0 cache dequant seam, the batch activation requants, and repack_kq_weight's round trip: every cell but the two walk-panel cells asserts on whatever tier is active, and the native batched arms ride a restored kq-backend pin; the bf16 tile's activation plane; the batch walk's panels in two threads and across the job queue, which skip where the generated backend carries no kq batch kernel; and the bf16 walk's token block.
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
require _kq_fixtures   // the salted plane regions and the Q8_K activation rows; dot_kq on the run-time format
require daslib/f16_cvt
require daslib/jobque_boost
require daslib/fio
require math
require _compares   // check: the images' compares; mismatch_exact: the plane bytes'; opaque: the fills' taint seam; bf16_value / bf16_bits_truncated

let SAME = Bar(exact = true, poison = 0.5, poison_rel = 1.0e-3)   //! bit for bit; the poison moves max(0.5, 1e-3 of the element's magnitude), so it cannot round away

let QUANT_SENTINEL = int8(-128)   //! the byte a requant output holds before its run: Q8_0 and Q8_K quants land in [-127, 127]
let SUM_SENTINEL = int(0x80000000u)   //! the per-16 sum word before its run: past any sum of sixteen quants

// The bf16 envelope: a bf16 operand sits within 2^-8 of its value, so a weight-activation
// product within 2^-7 of its own magnitude; the terms round independently, so a row's error is
// 2^-7 / sqrt(3) of the terms' rms times sqrt(n), against an output of the same order - 0.5% of
// the image's largest magnitude on these fixtures, held at 1%. The image's magnitude added to one
// expected element must land outside it.
let BF16_ENVELOPE = Bar(kind = BarKind.rel_of_max, rel = 1.0e-2, poison = 1.0, poison_of_max = true)

//! a generated walk adds a row's superblock terms in another order than the disk-order dot: the same integer products, each f32 add within 2^-24 of the running sum - 1e-5 of the image's largest magnitude holds it at these row lengths
let FLOAT_ORDER = Bar(kind = BarKind.rel_of_max, rel = 1.0e-5, poison = 1.0, poison_of_max = true)

//! an image's compare: its elements named by token (row) and output row (column)
def private image_shape(d : int64) : Shape => Shape(row_width = int(d))

def private fmt_tag(fmt : int) : string => fmt == 40 ? "q40" : "k{fmt}"

let ACTS_WAVE = Wave(a = 0.9, fa = 0.27, b = -0.5, fb = 0.061)   //! the activation rows' fill

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
    var a <- build_q8k_acts(ntok, n, ACTS_WAVE)
    var want : array<float>
    want |> resize(ntok * d)
    for (p in range64(ntok)) {
        for (r in range64(d)) {
            want[p * d + r] = kq_row_dot(fmt, kq, ks, r, nsb, a.xq, a.xs, a.xbs, p * n, p * nsb, p * (n / 16l), n)
        }
    }
    var q1 : array<int8>
    var s1 : array<float>
    var b1 : array<int>
    alloc_row_scratch(q1, s1, b1, n)
    var y : array<float>
    y |> resize(ntok * d)
    nan_fill(y)
    for (p in range64(ntok)) {
        quant_one(a.src, p * n, n, q1, s1, b1)
        matmul_kq(fmt, y, kq, ks, 0l, q1, s1, b1, n, d, p * d)
    }
    check(t, "{fmt_tag(fmt)} serial per-position GEMV (n={int(n)} d={int(d)} ntok={int(ntok)}) vs per-(token,row) disk dots",
        y, want, SAME, image_shape(d))
    let seat = kernel_backend_seat()
    pin_kernel_backend(gen_backend_name())
    if (kernel_backend_has_kq() && kernel_backend_has_kq_batch()) {
        repack_kq_weight(fmt, kq, ks, 0l, n, d)
        var ybatch : array<float>
        ybatch |> resize(ntok * d)
        nan_fill(ybatch)
        matmul_kq_batch(fmt, ybatch, kq, ks, 0l, a.xq, a.xs, a.xbs, n, d, ntok)
        var yser : array<float>
        yser |> resize(ntok * d)
        nan_fill(yser)
        for (p in range64(ntok)) {
            quant_one(a.src, p * n, n, q1, s1, b1)
            matmul_kq_active(fmt, yser, kq, ks, 0l, q1, s1, b1, n, d, p * d)
        }
        if (kq_tileform_of(fmt) > 0 && ntok >= amx_bf16_tile_tokens()) {
            check(t, "{fmt_tag(fmt)} native kq batch on the bf16 tile vs per-(token,row) disk dots (n={int(n)} d={int(d)} ntok={int(ntok)})",
                ybatch, want, BF16_ENVELOPE, image_shape(d))
            check(t, "{fmt_tag(fmt)} native kq batch on the bf16 tile vs ntok x matmul_kq_active (n={int(n)} d={int(d)} ntok={int(ntok)})",
                ybatch, yser, BF16_ENVELOPE, image_shape(d))
        } else {
            check(t, "{fmt_tag(fmt)} native kq batch is bit-for-bit ntok x matmul_kq_active (ntok={int(ntok)})",
                ybatch, yser, SAME, image_shape(d))
            check(t, "{fmt_tag(fmt)} native kq batch vs per-(token,row) disk dots (n={int(n)} d={int(d)} ntok={int(ntok)})",
                ybatch, want, FLOAT_ORDER, image_shape(d))
        }
        delete ybatch
        delete yser
    }
    restore_kernel_backend(seat)
    delete kq
    delete ks
    delete a
    delete want
    delete q1
    delete s1
    delete b1
    delete y
}

[test]
def test_kq_batch(t : T?) {
    for (fmt in [4, 6, 40]) {
        for (ntok in [1l, 4l, 5l, 9l, 13l]) {
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

def private xbf16_gate(t : T?; n, ntok : int64) {
    var a <- build_q8k_acts(ntok, n, ACTS_WAVE)
    var stale : array<int8>
    stale |> resize(ntok * n)
    for (sv, qv in stale, a.xq) {
        sv = int8(int(qv) > 0 ? int(qv) - 1 : int(qv) + 1)
    }
    var bad = 0
    var worst = 0.0
    var worstAt = 0l
    var truncatedSeen = 0
    unsafe {
        xbf16_build(addr<int8 const?>(stale[0]), addr<float const?>(a.xs[0]), n, ntok, 256l)
        let hp = reinterpret<uint16 const?>(xbf16_build(addr<int8 const?>(a.xq[0]), addr<float const?>(a.xs[0]), n, ntok, 256l))
        t |> success((int64(intptr(hp)) & 63l) == 0l, "xbf16_build hands back a 64-byte aligned plane")
        for (p in range64(ntok)) {
            for (i in range64(n)) {
                let x = float(a.xq[p * n + i]) * a.xs[p * (n / 256l) + i / 256l]
                let df = abs(bf16_value(hp[p * n + i]) - x)
                if (!(df <= abs(x) * 0.00390625)) {
                    bad++
                }
                if (!(df <= worst)) {
                    worst = df
                    worstAt = p * n + i
                }
                if (!(abs(bf16_value(bf16_bits_truncated(x)) - x) <= abs(x) * 0.00390625)) {
                    truncatedSeen++
                }
            }
        }
    }
    to_log(LOG_INFO, "xbf16_build (n={int(n)} ntok={int(ntok)}): max err {worst} at token {int(worstAt / n)} element {int(worstAt % n)}\n")
    t |> success(bad == 0, "xbf16_build (n={int(n)} ntok={int(ntok)}): {bad} of {int(ntok * n)} elements off q times its superblock scale rounded to bf16, max err {worst} at token {int(worstAt / n)} element {int(worstAt % n)}")
    delete stale
    t |> success(truncatedSeen > 0, "xbf16_build: a truncated bf16 must be seen past half a step, and was not")
    delete a
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
    var a <- build_q8k_acts(2l, n, ACTS_WAVE)
    var want : array<uint16>
    want |> resize(2l * n)
    unsafe {
        for (p in range64(2l)) {
            let hp = reinterpret<uint16 const?>(xbf16_build(addr<int8 const?>(a.xq[p * n]), addr<float const?>(a.xs[p * (n / 256l)]), n, 1l, 256l))
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
                    plane_thread(lane, n, rounds, addr<int8 const?>(a.xq[0]), addr<float const?>(a.xs[0]), addr<uint16 const?>(want[0]),
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
    delete a
    delete want
}

def private walk_seed(lane : int) : uint => 0x51ED270Bu + uint(lane) * 0x9E3779B9u

//! a lane's repacked planes into `kq` / `ks`, and its activation rows
def private walk_inputs(fmt : int; n, d, ntok : int64; seed : uint; var kq : array<uint8>; var ks : array<uint8>) : Q8kActs {
    build_kq_region(fmt, kq, ks, d, n, seed)
    var a <- build_q8k_acts(ntok, n, ACTS_WAVE)
    repack_kq_weight(fmt, kq, ks, 0l, n, d)
    return <- a
}

// Two contexts run the batch walk at once, each over its own planes: the walk's panels are a
// context's own scratch, so neither walk reads a panel the other built. The contexts are two
// threads', each walking on its calling thread (one dispatch slot, the same slot in both); a
// thread builds the inputs the main thread built for its lane, waits for the other to start,
// then walks `rounds` times and counts the elements off the main thread's walk.
def private walk_thread(lane : int; fmt : int; n, d, ntok : int64; rounds : int; wp : float const?;
                        var sp : int?; var dp : int?; var bp : int64?; var fp : int64?; var wg : JobStatus?) {
    new_thread() @() {
        set_single_thread_(true)
        pin_kernel_backend(gen_backend_name())
        var kq : array<uint8>
        var ks : array<uint8>
        var a <- walk_inputs(fmt, n, d, ntok, walk_seed(lane), kq, ks)
        var y : array<float>
        y |> resize(ntok * d)
        unsafe {
            sp[lane] = 1
            let t0 = ref_time_ticks()
            while (sp[1 - lane] == 0 && get_time_usec(t0) < 2000000) {
                sleep(1u)
            }
            for (r in range(rounds)) {
                nan_fill(y)
                matmul_kq_batch(fmt, y, kq, ks, 0l, a.xq, a.xs, a.xbs, n, d, ntok)
                for (i in range64(ntok * d)) {
                    if (y[i] != wp[int64(lane) * ntok * d + i]) {
                        if (bp[lane] == 0l) {
                            fp[lane] = int64(r) * ntok * d + i
                        }
                        bp[lane]++
                    }
                }
                dp[lane]++
            }
        }
        delete kq
        delete ks
        delete a
        delete y
        wg |> notify_and_release
    }
}

//! where a walk's first element off its reference sits: the round, the token and the row
def private first_off(at, ntok, d : int64) : string
    => at < 0l ? "none" : "round {int(at / (ntok * d))} token {int((at % (ntok * d)) / d)} row {int(at % d)}"

def private walk_two_contexts(t : T?; fmt : int; n, d, ntok : int64; rounds : int) {
    let seat = kernel_backend_seat()
    pin_kernel_backend(gen_backend_name())
    if (!kernel_backend_has_kq() || !kernel_backend_has_kq_batch()) {
        restore_kernel_backend(seat)
        t |> skip("{gen_backend_name()} carries no kq batch kernel on this box")
        return
    }
    var want : array<float>
    want |> resize(2l * ntok * d)
    for (lane in range(2)) {
        var kq : array<uint8>
        var ks : array<uint8>
        var a <- walk_inputs(fmt, n, d, ntok, walk_seed(lane), kq, ks)
        var y : array<float>
        y |> resize(ntok * d)
        nan_fill(y)
        matmul_kq_batch(fmt, y, kq, ks, 0l, a.xq, a.xs, a.xbs, n, d, ntok)
        for (i in range64(ntok * d)) {
            want[int64(lane) * ntok * d + i] = y[i]
        }
        delete kq
        delete ks
        delete a
        delete y
    }
    restore_kernel_backend(seat)
    var lanesDiffer = 0
    var finite = 0
    for (i in range64(ntok * d)) {
        lanesDiffer += want[i] != want[ntok * d + i] ? 1 : 0
        finite += is_finite(want[i]) ? 1 : 0
    }
    var started : int[2]
    var done : int[2]
    var bad : int64[2]
    var first = fixed_array(-1l, -1l)
    with_job_que() {
        with_wait_group(2) $(wg) {
            unsafe {
                for (lane in range(2)) {
                    walk_thread(lane, fmt, n, d, ntok, rounds, addr<float const?>(want[0]), addr(started[0]), addr(done[0]), addr(bad[0]), addr(first[0]), wg)
                }
            }
        }
    }
    to_log(LOG_INFO, "{fmt_tag(fmt)} batch walk in two threads (n={int(n)} d={int(d)} ntok={int(ntok)}, {rounds} rounds each, a panel of {int(kq_walk_panel_bytes(fmt, n, kq_layout_of(fmt)))} bytes): {bad[0]} and {bad[1]} elements off, the first at {first_off(first[0], ntok, d)} and {first_off(first[1], ntok, d)}\n")
    t |> equal(finite, int(ntok * d), "the main thread's walk wrote every element")
    t |> equal(done[0] + done[1], 2 * rounds, "both threads ran every round")
    t |> success(lanesDiffer > 0, "the two threads' outputs differ ({lanesDiffer} of {int(ntok * d)} elements), so a shared panel would show")
    t |> equal(bad[0], 0l, "thread 0 read its own panels in every round")
    t |> equal(bad[1], 0l, "thread 1 read its own panels in every round")
    delete want
}

// One context walks across the job queue: every dispatch slot decodes into its own panel, so
// the dispatched walk reads what the same walk reads on its calling thread alone.
def private slots_gate(t : T?; tag : string; ntok, d : int64; rounds : int; walk : block<(var y : array<float>) : void>) {
    var want : array<float>
    want |> resize(ntok * d)
    var y : array<float>
    y |> resize(ntok * d)
    let single = get_single_thread_()
    var bad = 0l
    var first = -1l
    var dispatched = 0l
    var slots = 0
    with_dasllama_jobque() {
        slots = get_dispatch_slot_bound()
        set_single_thread_(true)
        nan_fill(want)
        invoke(walk, want)
        set_single_thread_(false)
        let before = get_disp_count()
        for (r in range(rounds)) {
            nan_fill(y)
            invoke(walk, y)
            for (i in range64(ntok * d)) {
                if (y[i] != want[i]) {
                    if (bad == 0l) {
                        first = int64(r) * ntok * d + i
                    }
                    bad++
                }
            }
        }
        dispatched = get_disp_count() - before
    }
    set_single_thread_(single)
    var finite = 0
    for (v in want) {
        finite += is_finite(v) ? 1 : 0
    }
    to_log(LOG_INFO, "{tag} across the job queue (d={int(d)} ntok={int(ntok)}, {rounds} rounds, {int(dispatched)} dispatches, {slots} slots): {bad} elements off the calling thread's walk, the first at {first_off(first, ntok, d)}\n")
    t |> equal(finite, int(ntok * d), "{tag}: the calling thread's walk wrote every element")
    t |> equal(bad, 0l, "{tag}: every slot read its own panel in every round")
    if (slots < 2) {
        t |> skip("one dispatch slot on this box: the walk ran inline, no two panels in use at once")
    } else {
        t |> success(dispatched >= int64(rounds), "{tag}: every round dispatched its walk ({int(dispatched)} dispatches in {rounds} rounds)")
    }
    delete want
    delete y
}

def private walk_slots(t : T?; fmt : int; n, d, ntok : int64; rounds : int) {
    let seat = kernel_backend_seat()
    pin_kernel_backend(gen_backend_name())
    if (!kernel_backend_has_kq() || !kernel_backend_has_kq_batch()) {
        restore_kernel_backend(seat)
        t |> skip("{gen_backend_name()} carries no kq batch kernel on this box")
        return
    }
    var kq : array<uint8>
    var ks : array<uint8>
    var a <- walk_inputs(fmt, n, d, ntok, walk_seed(0), kq, ks)
    slots_gate(t, "{fmt_tag(fmt)} batch walk (n={int(n)})", ntok, d, rounds) $(var y : array<float>) {
        matmul_kq_batch(fmt, y, kq, ks, 0l, a.xq, a.xs, a.xbs, n, d, ntok)
    }
    if (kernel_backend_has_kq_batch_groupn()) {
        let head = ntok / 3l
        let offs = [0l, 0l, head, 0l, head, ntok - head]   // two token runs over the one weight region
        slots_gate(t, "{fmt_tag(fmt)} region walk (n={int(n)}, runs {int(head)} and {int(ntok - head)})", ntok, d, rounds) $(var y : array<float>) {
            matmul_kq_batch_groupn(fmt, y, kq, ks, offs, 2l, a.xq, a.xs, a.xbs, n, d)
        }
    }
    restore_kernel_backend(seat)
    delete kq
    delete ks
    delete a
}

[test]
def test_walk_panels(t : T?) {
    let k6 = kq_schema_id(KqFmt.k6)
    t |> run("two threads walking at once each read their own panels") @(t : T?) {
        walk_two_contexts(t, k6, 512l, 48l, 67l, 200)
    }
    t |> run("a walk dispatched across the job queue reads its calling thread's walk") @(t : T?) {
        walk_slots(t, k6, 512l, 128l, 67l, 200)
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
    nan_fill(y)
    var row0 = 0l
    for (r in range64(long_length(cnts))) {
        for (tk in range64(row0, row0 + cnts[r])) {
            quant_one(src, tk * n, n, q1, s1, b1)
            matmul_kq(fmt, y, kq, ks, r * d * n, q1, s1, b1, n, d, tk * d)
        }
        row0 += cnts[r]
    }
    check(t, "{fmt_tag(fmt)} per-expert per-token GEMV route (what a tier with no fused kq slot runs) vs per-(region,token) disk dots",
        y, want, SAME, image_shape(d))
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
    nan_fill(wantg)
    var row0 = 0l
    let kf = kq_fmt_of_id(fmt, "groupn_fused")
    unsafe {
        for (r in range64(nreg)) {
            let kqr = addr<uint8 const?>(kq[r * d * nsb * qsb])
            let ksr = addr<uint8 const?>(ks[r * d * nsb * ssb])
            for (tk in range64(row0, row0 + cnts[r])) {
                kq_gemv_rt(kf, addr(wantg[tk * d]), kqr, ksr, addr<int8 const?>(xq[tk * n]),
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
        check(t, "{fmt_tag(fmt)} fused kq batch groupn on the bf16 tile vs per-(region,token) disk dots",
            yb, want, BF16_ENVELOPE, image_shape(d))
        check(t, "{fmt_tag(fmt)} fused kq batch groupn on the bf16 tile vs per-(region, token) rows-core calls",
            yb, wantg, BF16_ENVELOPE, image_shape(d))
    } else {
        check(t, "{fmt_tag(fmt)} fused kq batch groupn bit-matches per-(region, token) rows-core calls",
            yb, wantg, SAME, image_shape(d))
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
    var a <- build_q8k_acts(nk, n, ACTS_WAVE)
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
                want[tk * d + row] = kq_row_dot(fmt, kq, ks, r * d + row, nsb, a.xq, a.xs, a.xbs,
                    tk * n, tk * nsb, tk * (n / 16l), n)
            }
        }
        row0 += cnts[r]
    }
    groupn_fallback(t, fmt, kq, ks, a.src, cnts, n, d, want, nk)
    let seat = kernel_backend_seat()
    pin_kernel_backend(gen_backend_name())
    if (kernel_backend_has_kq() && kernel_backend_has_kq_batch_groupn()) {
        groupn_fused(t, fmt, kq, ks, offs, cnts, a.xq, a.xs, a.xbs, n, d, nk, want)
    }
    restore_kernel_backend(seat)
    delete cnts
    delete kq
    delete ks
    delete a
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
    let kf = kq_fmt_of_id(fmt, "grp_rows_after_repack")
    unsafe {
        for (r in range64(d)) {
            let g = r / mr
            got[r] = kq_grp_row_dot_rt(kf, addr<uint8 const?>(kq[g * mr * nsb * qsb]),
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
    let seat = kernel_backend_seat()
    pin_kernel_backend(gen_backend_name())
    let has_kq = kernel_backend_has_kq()
    repack_kq_weight(fmt, kqs, kss, 0l, n, d)
    if (has_kq) {
        let moved = mismatch_exact(kqs, kqg) + mismatch_exact(kss, ksg)
        t |> success(moved == 0, "repack_kq_weight lands on the grp<{int(kq_layout_of(fmt))}> layout the readers expect ({moved} bytes differ)")
        var got : array<float>
        grp_rows_after_repack(fmt, kqs, kss, nsb, d, kq_layout_of(fmt), xq, xs, xbs, n, got)
        check(t, "{fmt_tag(fmt)} row dots through repack_kq_weight's own planes", got, want, SAME, FLAT_SHAPE)
        delete got
    } else {
        let moved = mismatch_exact(kqs, kq) + mismatch_exact(kss, ks)
        t |> success(moved == 0, "with no kq slot on the tier repack_kq_weight is the registered identity ({moved} bytes moved)")
        var got : array<float>
        got |> resize(d)
        for (r in range64(d)) {
            got[r] = kq_row_dot(fmt, kqs, kss, r, nsb, xq, xs, xbs, 0l, 0l, 0l, n)
        }
        check(t, "{fmt_tag(fmt)} disk row dots survive the identity repack", got, want, SAME, FLAT_SHAPE)
    }
    restore_kernel_backend(seat)
    delete kqs
    delete kss
}

def private repack_gate(t : T?; fmt : int; n, d : int64) {
    let nsb = n / 256l
    var kq : array<uint8>
    var ks : array<uint8>
    build_kq_region(fmt, kq, ks, d, n, 0x9E3779B9u)
    var a <- build_q8k_acts(1l, n, ACTS_WAVE)
    var want : array<float>
    want |> resize(d)
    for (r in range64(d)) {
        want[r] = kq_row_dot(fmt, kq, ks, r, nsb, a.xq, a.xs, a.xbs, 0l, 0l, 0l, n)
    }
    let mr = kq_layout_of(fmt)
    var kqg : array<uint8>
    kqg := kq
    var ksg : array<uint8>
    ksg := ks
    unsafe {
        repack_kq_grp(kq_fmt_of_id(fmt, "repack_gate"), addr(kqg[0]), addr(ksg[0]), n, d, mr)
    }
    var got : array<float>
    grp_rows_after_repack(fmt, kqg, ksg, nsb, d, mr, a.xq, a.xs, a.xbs, n, got)
    check(t, "{fmt_tag(fmt)} grp<{int(mr)}> row dots after the repack (n={int(n)} d={int(d)})", got, want, SAME, FLAT_SHAPE)
    repack_seam(t, fmt, kq, ks, kqg, ksg, want, nsb, n, d, a.xq, a.xs, a.xbs)
    delete kq
    delete ks
    delete kqg
    delete ksg
    delete a
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
    nan_fill(got)
    unsafe {
        cvt_q8kv_to_f32(addr(got[0]), addr<uint8 const?>(blob[0]), n)
    }
    check(t, "cvt_q8kv_to_f32 vs the per-block scalar reference", got, want, SAME, FLAT_SHAPE)
    var got2 : array<float>
    got2 |> resize(n)
    nan_fill(got2)
    unsafe {
        let bp = addr<uint8 const?>(blob[0])
        kv_row_to_f32(addr(got2[0]), bp, kv_head_off(bp, n), n)
    }
    check(t, "kv_row_to_f32(q8_0) at the second row's byte base", got2, want, SAME, FLAT_SHAPE)
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
    nan_fill(got)
    unsafe {
        let wp = addr<float const?>(wide[0])
        kv_row_to_f32(addr(got[0]), wp, kv_head_off(wp, n), n)
    }
    check(t, "kv_row_to_f32(f32) is the plain row copy", got, src, SAME, FLAT_SHAPE)
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
    nan_fill(got2)
    unsafe {
        let hp = addr<uint16 const?>(half[0])
        kv_row_to_f32(addr(got2[0]), hp, kv_head_off(hp, n), n)
    }
    check(t, "kv_row_to_f32(f16) widens the row it was pointed at", got2, want, SAME, FLAT_SHAPE)
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
    for (q in xqb) {
        q = QUANT_SENTINEL
    }
    nan_fill(xsb)
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
    for (q in xqb) {
        q = QUANT_SENTINEL
    }
    nan_fill(xsb)
    for (s in xbsb) {
        s = SUM_SENTINEL
    }
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
