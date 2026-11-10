options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default  // registers the portable Q8·Q8 backend (the matmul_q8q8 wrappers dispatch to it)
require dasllama/dasllama_math_aarch64_neon   // arm64 backends (registered under arm64 + JIT only)
require dasllama/dasllama_math_gen   // bf16w_tileform_gen: the bf16 plane's walk rounds its activations where the half-width family serves
require ?das_metal dasllama/dasllama_metal_gemm  // registers the "metal" batch donor on Apple builds
require ?das_metal metal/das_metal_boost  // metal_live_object_count for the leak gate
require dasllama/dasllama_convert
require daslib/jobque_boost
require math
require _compares   // row_fill; mismatch_exact: the grid cell's bit-for-bit compare

// The batched (prefill) GEMM answers the per-token matmul run ntok times: the Q8·Q8 path
// BIT-FOR-BIT (same dots, only the loop order changes), the fp32 path to the lanes' rounding
// (its 4x4 tile accumulates each dot in its own lanes; the row and token tails are the same
// `dot`). We verify both the single-thread and the threaded branch (the one real prefill hits).

//! `cnt` floats of the file's fill: (i * 7) % 13 scaled by 0.1, less 0.6
def private fillf(cnt : int64) : array<float> => row_fill(int(cnt), 7, 0, 0.1, 0.6, 13)

//! the fp32 walk's allowance against the double oracle, relative to max(|result|, 1): an fp32 sum of n
//! products carries at most n * 2^-24 of the products' summed magnitudes, under 3e-4 of that sum at
//! n 4320 with this fill's terms under 0.5, and the fill's cancellation leaves results of the same
//! order as the sum, so 1e-5 holds at every shape here (the logged worst reads 2e-6 at n 1152, 6e-6
//! at n 4320) while a skipped or misplaced tile lands at 1e-1 and past
let private F32_TILE_REL = 1.0e-5

//! `cnt` floats of NaN: a sentinel every element a walk must write, so a skipped range reads as NaN
def private nan_fill(cnt : int64) : array<float> {
    var y : array<float>
    y |> resize(cnt)
    let nan_f = unsafe(reinterpret<float>(0x7fc00000u))
    for (k in range64(cnt)) {
        y[k] = nan_f
    }
    return <- y
}

//! the walk's route pinned to the shipped knobs - the tile at every dot length, the 256 KB X block - around a block,
//! then put back as found
def private with_shipped_knobs(blk : block<void>) {
    let min_n_was = get_f32_batch_tile_min_n()
    let x_block_was = get_f32_batch_x_block()
    set_f32_batch_tile_min_n(1l)
    set_f32_batch_x_block(256l * 1024l)
    invoke(blk)
    set_f32_batch_tile_min_n(min_n_was)
    set_f32_batch_x_block(x_block_was)
}

//! an exact compare's log line, every run: the count and the first differing index
def private log_exact(tag : string; got, want : array<float>) : int {
    let n = mismatch_exact(got, want)
    let first = n > 0 ? first_mismatch(got, want) : -1
    to_log(LOG_INFO, "{tag}: {n} of {length(want)} elements differ, the first at {first}\n")
    return n
}

//! the plain-loop oracle: every output a double dot, no kernel of ../dasllama/ in it
def private oracle_fp32(w, x : array<float>; n, d, ntok : int64) : array<float> {
    var want : array<float>
    want |> resize(ntok * d)
    for (tk in range64(ntok)) {
        for (i in range64(d)) {
            var acc = 0.0lf
            for (j in range64(n)) {
                acc += double(w[i * n + j]) * double(x[tk * n + j])
            }
            want[tk * d + i] = float(acc)
        }
    }
    return <- want
}

// fp32: Y [ntok x d] from matmul_batch against the plain oracle within the tile's rounding; the
// largest difference is logged with its row and column, and a poisoned element must red the bar.
def check_fp32(t : T?; n, d, ntok : int64) {
    let w <- fillf(n * d)
    let x <- fillf(ntok * n)
    var want <- oracle_fp32(w, x, n, d, ntok)
    var y <- nan_fill(ntok * d)
    with_shipped_knobs() {
        matmul_batch(y, w, x, n, d, ntok)
    }
    var worst = 0.0
    var at = 0l
    for (k in range64(ntok * d)) {
        let diff = nan_wide_diff(y[k], want[k]) / max(abs(want[k]), 1.0)
        if (diff > worst) {
            worst = diff
            at = k
        }
    }
    to_log(LOG_INFO, "fp32 batch {n}x{d} ntok={ntok}: worst relative difference {worst} at token {at / d} row {at % d}, bar {F32_TILE_REL}\n")
    t |> success(all_finite(y), "fp32 batch {n}x{d} ntok={ntok}: every output finite")
    t |> equal(mismatch_rel("fp32 batch {n}x{d} ntok={ntok}", y, want, F32_TILE_REL), 0, "fp32 batch {n}x{d} ntok={ntok}: elements past {F32_TILE_REL} relative of the plain oracle")
    want[(ntok * d) / 2l] += 1.0e-3 * max(abs(want[(ntok * d) / 2l]), 1.0)   // the control: a poisoned expectation reds the bar
    t |> success(mismatch_rel("fp32 batch {n}x{d} ntok={ntok} poisoned", y, want, F32_TILE_REL) > 0, "control: the poisoned element must miss the {F32_TILE_REL} bar")
}

//! the threaded walk against the one-lane walk, bit for bit: the inline leg runs before any job
//! que exists (dastest runs this file's tests in order, and this one comes first)
[test(description = "The fp32 `matmul_batch` walk under a job queue lands the one-lane walk's bits exactly at 1152x480 over 300 tokens, where moving one element by 1e-3 shows as a mismatch")]
def test_matmul_batch_lanes(t : T?) {
    t |> run("fp32 batch: the threaded walk lands the one-lane walk's bits") @(t : T?) {
        t |> success(!is_job_que_available(), "the inline leg needs a que-less process to be one lane")
        let n = 1152l
        let d = 480l
        let ntok = 300l
        let w <- fillf(n * d)
        let x <- fillf(ntok * n)
        var inline_y <- nan_fill(ntok * d)
        var threaded_y <- nan_fill(ntok * d)
        with_shipped_knobs() {
            matmul_batch(inline_y, w, x, n, d, ntok)
            with_dasllama_jobque_() {
                t |> success(is_job_que_available(), "the threaded leg runs under the que")
                matmul_batch(threaded_y, w, x, n, d, ntok)
            }
        }
        t |> equal(log_exact("fp32 batch lanes", threaded_y, inline_y), 0, "elements differing between the threaded and the one-lane walk")
        var poisoned := inline_y
        poisoned[(ntok * d) / 2l] += 1.0e-3
        t |> success(log_exact("fp32 batch lanes poisoned", threaded_y, poisoned) > 0, "control: one element moved in the one-lane result is a mismatch")
    }
}

//! Y [ntok x d] as the per-token `matmul` lands it, a token at a time: the nest cells' expectation
def private per_token_matmul(w, x : array<float>; n, d, ntok : int64) : array<float> {
    var want : array<float>
    want |> resize(ntok * d)
    var xt : array<float>
    xt |> resize(n)
    var yt : array<float>
    yt |> resize(d)
    for (tk in range64(ntok)) {
        for (j in range64(n)) {
            xt[j] = x[tk * n + j]
        }
        matmul(yt, w, xt, n, d)
        for (i in range64(d)) {
            want[tk * d + i] = yt[i]
        }
    }
    return <- want
}

//! the race's nest arm, reached through the knob alone: every output the per-token `dot`'s bits,
//! where the tile at the shipped knob lands other bits (the control that the knob switched arms);
//! then the nest as its own entry at the MoE router's shape, the tile at the shipped knob apart again
[test(description = "The fp32 batch row nest, reached by the knob or through `matmul_batch_rows` at a 128-expert router shape, equals the per-token `matmul` bit for bit, where the shipped tile rounds apart")]
def test_matmul_batch_nest_arm(t : T?) {
    t |> run("fp32 batch: the row nest under the knob is the per-token matmul bit for bit") @(t : T?) {
        let n = 1152l
        let d = 480l
        let ntok = 37l
        let w <- fillf(n * d)
        let x <- fillf(ntok * n)
        let want <- per_token_matmul(w, x, n, d, ntok)
        let min_n_was = get_f32_batch_tile_min_n()
        set_f32_batch_tile_min_n(1l << 30)
        var nest_y <- nan_fill(ntok * d)
        matmul_batch(nest_y, w, x, n, d, ntok)
        set_f32_batch_tile_min_n(min_n_was)
        t |> equal(log_exact("fp32 batch nest arm", nest_y, want), 0, "elements differing between the nest arm and the per-token matmul")
        let oracle <- oracle_fp32(w, x, n, d, ntok)
        t |> equal(mismatch_rel("fp32 batch nest arm against the oracle", nest_y, oracle, F32_TILE_REL), 0, "the nest arm against the plain oracle within {F32_TILE_REL}")
        var tile_y <- nan_fill(ntok * d)
        with_shipped_knobs() {
            matmul_batch(tile_y, w, x, n, d, ntok)
        }
        t |> success(log_exact("fp32 batch tile at the shipped knob", tile_y, want) > 0, "control: the tile at the shipped knob rounds apart from the per-token dot")
    }
    t |> run("fp32 batch: the nest entry is the per-token matmul bit for bit at the router's shape") @(t : T?) {
        let n = 2048l   // a 128-expert router over a 2048 hidden, a 97-token prompt: a whole row tile set, a token tail
        let d = 128l
        let ntok = 97l
        let w <- fillf(n * d)
        let x <- fillf(ntok * n)
        let want <- per_token_matmul(w, x, n, d, ntok)
        var rows_y <- nan_fill(ntok * d)
        with_shipped_knobs() {
            matmul_batch_rows(rows_y, w, 0l, x, n, d, ntok)
        }
        t |> equal(log_exact("fp32 batch rows entry", rows_y, want), 0, "elements differing between the nest entry and the per-token matmul")
        var tile_y <- nan_fill(ntok * d)
        with_shipped_knobs() {
            matmul_batch(tile_y, w, x, n, d, ntok)
        }
        t |> success(log_exact("fp32 batch tile at the router's shape", tile_y, want) > 0, "control: the tile at the shipped knob rounds apart from the per-token dot at this shape")
    }
}

//! the weight as bf16 bits (round to nearest even), and the oracle over the widened value
def private bf16_bits_of(v : float) : uint16 {
    let bits = unsafe(reinterpret<uint>(v))
    return uint16((bits + 0x7FFFu + ((bits >> 16u) & 1u)) >> 16u)
}

//! the bf16 plane through the same walk: Y against the plain oracle over the widened weights within
//! the tile's rounding, a NaN sentinel under every output, the poisoned expectation the control;
//! where the stamped half-width family serves the shape (`bf16w_batch_serves`) the oracle's
//! activations are the family's bf16-rounded ones
def check_bf16(t : T?; n, d, ntok : int64) {
    let wf <- fillf(n * d)
    var x <- fillf(ntok * n)
    if (ntok > 1l && bf16w_batch_serves(n, d, ntok)) {
        for (k in range64(ntok * n)) {
            x[k] = unsafe(reinterpret<float>(uint(bf16_bits_of(x[k])) << 16u))
        }
    }
    var w16 : array<uint16>
    w16 |> resize(n * d)
    var wwide : array<float>
    wwide |> resize(n * d)
    for (k in range64(n * d)) {
        w16[k] = bf16_bits_of(wf[k])
        wwide[k] = unsafe(reinterpret<float>(uint(w16[k]) << 16u))
    }
    var want <- oracle_fp32(wwide, x, n, d, ntok)
    var y <- nan_fill(ntok * d)
    with_shipped_knobs() {
        matmul_bf16_batch(y, w16, 0l, x, n, d, ntok)
    }
    t |> success(all_finite(y), "bf16 batch {n}x{d} ntok={ntok}: every output finite")
    t |> success(all_finite(want), "bf16 batch {n}x{d} ntok={ntok}: every oracle element finite")
    t |> equal(mismatch_rel("bf16 batch {n}x{d} ntok={ntok}", y, want, F32_TILE_REL), 0, "bf16 batch {n}x{d} ntok={ntok}: elements past {F32_TILE_REL} relative of the plain oracle over the widened weights")
    want[(ntok * d) / 2l] += 1.0e-3 * max(abs(want[(ntok * d) / 2l]), 1.0)
    t |> success(mismatch_rel("bf16 batch {n}x{d} ntok={ntok} poisoned", y, want, F32_TILE_REL) > 0, "control: the poisoned element must miss the {F32_TILE_REL} bar")
}

// Q8·Q8: same, with weights quantized once and the activation quantized per token.
def check_q8(t : T?; n, d, ntok : int64) {
    let nb = n / 32l
    let w <- fillf(n * d)
    let x <- fillf(ntok * n)
    let qt <- quantize_q8_0(w, n * d)
    var xq_all : array<int8>
    var xs_all : array<float>
    xq_all |> resize(ntok * n)
    xs_all |> resize(ntok * nb)
    var yref : array<float>
    yref |> resize(ntok * d)
    var xt : array<float>
    xt |> resize(n)
    var xqt : array<int8>
    xqt |> resize(n)
    var xst : array<float>
    xst |> resize(nb)
    var yt : array<float>
    yt |> resize(d)
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xt[i] = x[tk * n + i]
        }
        quantize_q8_0_into(xt, n, xqt, xst, 0l, 0l)
        matmul_q8q8(yt, qt.q, qt.scales, xqt, xst, n, d)
        for (i in range64(d)) {
            yref[tk * d + i] = yt[i]
        }
        for (i in range64(n)) {
            xq_all[tk * n + i] = xqt[i]
        }
        for (i in range64(nb)) {
            xs_all[tk * nb + i] = xst[i]
        }
    }
    var y : array<float>
    y |> resize(ntok * d)
    matmul_q8q8_batch(y, qt.q, qt.scales, xq_all, xs_all, n, d, ntok)
    for (k in range64(ntok * d)) {
        t |> equal(y[k], yref[k])
    }
}

[test(description = "The fp32 and bf16 batch GEMMs match a double-dot oracle within 1e-5 relative over tile and tail shapes, a poisoned element missing the bar, and the Q8 batch GEMM equals the per-token `matmul_q8q8` exactly")]
def test_matmul_batch(t : T?) {
    with_dasllama_jobque_() {
        t |> run("fp32 single 64x5 ntok=4") @(t : T?) {
            check_fp32(t, 64l, 5l, 4l)
        }
        t |> run("fp32 ntok=1 == matmul") @(t : T?) {
            check_fp32(t, 96l, 7l, 1l)
        }
        t |> run("fp32 threaded 2048x512 ntok=2") @(t : T?) {
            check_fp32(t, 2048l, 512l, 2l)
        }
        t |> run("fp32 nine-tap dots 9x480 ntok=100") @(t : T?) {
            check_fp32(t, 9l, 480l, 100l)
        }
        t |> run("fp32 tiles with row and token tails 100x13 ntok=7") @(t : T?) {
            check_fp32(t, 100l, 13l, 7l)
        }
        t |> run("bf16 plane 1152x480 ntok=300") @(t : T?) {
            check_bf16(t, 1152l, 480l, 300l)
        }
        t |> run("bf16 plane, row and token tails 100x13 ntok=7") @(t : T?) {
            check_bf16(t, 100l, 13l, 7l)
        }
        t |> run("bf16 plane, a group tail and a token tail 2560x200 ntok=100") @(t : T?) {
            check_bf16(t, 2560l, 200l, 100l)
        }
        t |> run("bf16 plane ntok=1, the gemv shaper") @(t : T?) {
            check_bf16(t, 96l, 7l, 1l)
        }
        t |> run("fp32 threaded 1152x480 ntok=300") @(t : T?) {
            check_fp32(t, 1152l, 480l, 300l)
        }
        t |> run("q8 single 64x5 ntok=4") @(t : T?) {
            check_q8(t, 64l, 5l, 4l)
        }
        t |> run("q8 threaded 2048x512 ntok=2") @(t : T?) {
            check_q8(t, 2048l, 512l, 2l)
        }
    }
}

[test(description = "The fp32 `matmul` computes each output as its weight row's dot with x, exactly on small-integer 2x3 and 1x4 cases where every sum is representable")]
def test_matmul(t : T?) {
    // W = [[1,2,3],[4,5,6]] (d=2, n=3). Small integers are exact in fp32, so == is safe.
    t |> run("2x3, x = ones") @(t : T?) {
        let w <- [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
        let x <- [1.0, 1.0, 1.0]
        var y <- [0.0, 0.0]
        matmul(y, w, x, 3l, 2l)
        t |> equal(y[0], 6.0)   // 1+2+3
        t |> equal(y[1], 15.0)  // 4+5+6
    }
    t |> run("2x3, x = distinct") @(t : T?) {
        let w <- [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
        let x <- [2.0, 0.0, 1.0]
        var y <- [0.0, 0.0]
        matmul(y, w, x, 3l, 2l)
        t |> equal(y[0], 5.0)   // 1*2 + 2*0 + 3*1
        t |> equal(y[1], 14.0)  // 4*2 + 5*0 + 6*1
    }
    // 1x4 row reduces to a single dot product.
    t |> run("1x4 dot product") @(t : T?) {
        let w <- [2.0, 3.0, 4.0, 5.0]
        let x <- [1.0, 10.0, 100.0, 1000.0]
        var y <- [0.0]
        matmul(y, w, x, 4l, 1l)
        t |> equal(y[0], 5432.0)  // 2 + 30 + 400 + 5000
    }
}

// The 2-D batch chunk grid (batch_grid_2d) must be BIT-FOR-BIT the 1-D walk: only chunk
// boundaries move, never the per-(token,row) dot fold. Every registered backend runs the same
// batch under knob 0/1/2 and the outputs must match exactly (portable/sdot and a bf16 tile walk
// ignore the knob — trivial equality; the gen batch walk is the real subject). target_chunk_work=1 admits the
// full lane pool so the starved shapes engage the grid on any box with enough lanes; a tiny
// q8_token_block exercises the TB-block loop INSIDE a token cell.

def private restore_grid_knobs() {
    set_batch_grid_2d(0)
    set_target_chunk_work(1_000_000l)
    set_q8_token_block(128l)
}

def private run_grid_batch(var y : array<float>; wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>; n, d, ntok : int64; knob : int) {
    set_batch_grid_2d(knob)
    matmul_q8q8_batch(y, wq, ws, xq, xs, n, d, ntok)
    set_batch_grid_2d(0)
}

def private check_shape(t : T?; n, d, ntok, tb : int64) {
    let nb = n / 32l
    var wf, xf : array<float>
    wf |> resize(n * d)
    xf |> resize(ntok * n)
    for (i in range64(n * d)) {
        wf[i] = sin(float(i) * 0.11f)
    }
    for (i in range64(ntok * n)) {
        xf[i] = cos(float(i) * 0.07f)
    }
    let wq0 <- quantize_q8_0(wf, n * d)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(ntok * n)
    xs |> resize(ntok * nb)
    var xt : array<float>
    xt |> resize(n)
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xt[i] = xf[tk * n + i]
        }
        quantize_q8_0_into(xt, n, xq, xs, tk * n, tk * nb)
    }
    set_q8_token_block(tb)
    set_target_chunk_work(1l)   // admit every lane — the small d is then chunk-starved for real
    let seat = kernel_backend_seat()
    for (name in kernel_backend_names()) {
        continue if (kernel_backend_batch_only(name))
        pin_kernel_backend(name)
        var inscope wq : array<int8>
        var inscope ws : array<float>
        wq := wq0.q
        ws := wq0.scales
        if (kernel_backend_needs_repack(name)) {
            repack_q8q8_weight(wq, ws, 0l, n, d)
        }
        var inscope y0, y1, y2 : array<float>
        y0 |> resize(ntok * d)
        y1 |> resize(ntok * d)
        y2 |> resize(ntok * d)
        run_grid_batch(y0, wq, ws, xq, xs, n, d, ntok, 0)
        run_grid_batch(y1, wq, ws, xq, xs, n, d, ntok, 1)
        run_grid_batch(y2, wq, ws, xq, xs, n, d, ntok, 2)
        t |> equal(mismatch_exact(y1, y0), 0, "fine grid == 1-D ({n}x{d} ntok={ntok} tb={tb}, backend {name})")
        t |> equal(mismatch_exact(y2, y0), 0, "wave-aligned grid == 1-D ({n}x{d} ntok={ntok} tb={tb}, backend {name})")
        delete wq
        delete ws
        delete y0
        delete y1
        delete y2
    }
    restore_kernel_backend(seat)
    restore_grid_knobs()
    delete wf
    delete xf
    delete xq
    delete xs
    delete xt
}

[test(description = "On every registered kernel backend the q8q8 batch matmul under the fine and wave-aligned 2-D chunk grids is bit-for-bit the 1-D walk, on starved, ragged-tail and in-cell token-block shapes")]
def test_batch_grid_bit_exact(t : T?) {
    with_dasllama_jobque_() {
        restore_grid_knobs()
        t |> run("starved 64x64, ragged token tail (33 = 8 tiles + 1)") @(t : T?) {
            check_shape(t, 64l, 64l, 33l, 128l)
        }
        t |> run("row tail (d=40), two fine token cells (ntok=20)") @(t : T?) {
            check_shape(t, 96l, 40l, 20l, 128l)
        }
        t |> run("token cell wider than TB (tb=8): the in-cell TB loop") @(t : T?) {
            check_shape(t, 64l, 64l, 33l, 8l)
        }
    }
}

// The Metal GPU prefill-GEMM donor (Phase-5 dasMetal offload) vs the portable CPU kernel on
// synthetic Q8 data. Two comparisons: power-of-two scales make the GPU's dequant-to-half EXACT
// (int8 quants and int8 x pow2 products are exactly representable in half, block sums stay under
// 2^24 in the f32 accumulators) so the compare is bit-exact; arbitrary scales carry the half
// rounding of the folded scale product and compare under relative tolerance — the documented
// envelope of the f16-tile path (the same one the upstream Metal GEMM accepts). Also exercised: the
// 32-row padding rail (ntok deliberately NOT a multiple of 32) and the small-ntok CPU delegation.
// Off-Apple (or with no Metal device) every check feints cleanly.

let N = 128     // K: weight row length, %32
let D = 64      // output rows, %32
let NTOK = 40   // deliberately unpadded — exercises the 32-row pad rail (M' = 64)

def private fill_case(var wq : array<int8>; var ws : array<float>; var xq : array<int8>; var xs : array<float>; pow2_scales : bool; n, d, ntok : int) {
    wq |> resize(d * n)
    ws |> resize(d * (n / 32))
    xq |> resize(ntok * n)
    xs |> resize(ntok * (n / 32))
    for (i in range(d * n)) {
        wq[i] = int8((i * 7) % 255 - 127)
    }
    for (i in range(ntok * n)) {
        xq[i] = int8((i * 11) % 255 - 127)
    }
    for (i in range(d * (n / 32))) {
        ws[i] = pow2_scales ? (((i % 3) == 0) ? 0.5 : (((i % 3) == 1) ? 0.25 : 2.0)) : (0.013 + 0.001 * float(i % 17))
    }
    for (i in range(ntok * (n / 32))) {
        xs[i] = pow2_scales ? (((i % 2) == 0) ? 1.0 : 0.125) : (0.021 + 0.003 * float(i % 13))
    }
}

def private run_batch(var y : array<float>; wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>; n, d, ntok : int) {
    y |> resize(ntok * d)
    matmul_q8q8_batch(y, wq, ws, 0l, xq, xs, int64(n), int64(d), int64(ntok))
}

[test(description = "The Metal prefill Q8 GEMM donor bit-matches the portable CPU kernel under power-of-two scales on the 32x32 and 64x64 tiles, stays in the f16-tile envelope otherwise, delegates small batches, and leaks nothing")]
def test_metal_gemm_vs_portable(t : T?) {   // nolint:STYLE038 — flat fixture build + one comparison cell per scale flavor
    t |> run("metal batch donor == portable CPU kernel on synthetic Q8 GEMM") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var found = false
            for (n in kernel_backend_names()) {
                if (n == "metal") {
                    found = true
                }
            }
            t |> success(found, "metal backend registered at [init]")
            var wq : array<int8>
            var ws : array<float>
            var xq : array<int8>
            var xs, want, got : array<float>
            // exact case: pow2 scales, GPU path forced (min_ntok 1)
            fill_case(wq, ws, xq, xs, true, N, D, NTOK)
            run_batch(want, wq, ws, xq, xs, N, D, NTOK)     // portable serves the batch slot
            select_batch_backend("metal")
            set_metal_gemm_min_ntok(1l)
            run_batch(got, wq, ws, xq, xs, N, D, NTOK)
            let st = metal_gemm_stats()
            if (st.dispatches == 0l) {
                // registered but no usable device (or compile failure) — the fallback served it;
                // results still match, but the GPU claims are not being tested
                select_batch_backend("")
                set_metal_gemm_min_ntok(32l)
                metal_gemm_shutdown()
                feint("metal backend fell back to CPU (no device?); GPU compare skipped\n")
                return
            }
            var bad_exact = 0
            for (k in range(NTOK * D)) {
                if (got[k] != want[k]) {
                    bad_exact++
                }
            }
            t |> equal(bad_exact, 0)                        // pow2 scales: bit-exact
            // 64x64-kernel case: a big output dim pushes the grid past the gemm_use64 threshold,
            // so this exact compare runs through the big-tile kernel (the D=64 case above stays
            // on the 32x32 kernel — both dispatch paths covered). Fresh fill sizes = fresh weight
            // arrays, so drop the pointer-keyed cache first.
            metal_gemm_shutdown()
            let D64 = 8192
            t |> success(gemm_use64(64l, int64(D64)), "test shape reaches the 64x64 kernel path")
            fill_case(wq, ws, xq, xs, true, N, D64, NTOK)
            select_batch_backend("portable")
            run_batch(want, wq, ws, xq, xs, N, D64, NTOK)
            select_batch_backend("metal")
            run_batch(got, wq, ws, xq, xs, N, D64, NTOK)
            var bad64 = 0
            for (k in range(NTOK * D64)) {
                if (got[k] != want[k]) {
                    bad64++
                }
            }
            t |> equal(bad64, 0)                            // pow2 scales: bit-exact on the 64 path too
            // tolerance case: arbitrary scales carry the dequant-to-half rounding. The refill
            // reuses the SAME arrays, so the pointer-keyed lazy weight cache must be dropped —
            // in production weights are load-once-stable (the documented cache contract). The
            // oracle run pins "portable" explicitly: select_batch_backend("") only clears the
            // standing pin — the slot restores at the NEXT whole-backend activation.
            metal_gemm_shutdown()
            fill_case(wq, ws, xq, xs, false, N, D, NTOK)
            select_batch_backend("portable")
            run_batch(want, wq, ws, xq, xs, N, D, NTOK)
            select_batch_backend("metal")
            run_batch(got, wq, ws, xq, xs, N, D, NTOK)
            // each dequantized half rounds at 2^-11 relative, so a product carries ~2^-10 of its
            // own magnitude — the error bound scales with the DOT ENVELOPE sum |w·x| (cancellation
            // makes |y| itself arbitrarily smaller), 2e-3 = 2x margin over the worst case
            var bad_tol = 0
            for (tok in range(NTOK)) {
                for (o in range(D)) {
                    var den = 0.0
                    for (k in range(N)) {
                        den += (abs(float(wq[o * N + k]) * ws[o * (N / 32) + k / 32]) *
                            abs(float(xq[tok * N + k]) * xs[tok * (N / 32) + k / 32]))
                    }
                    let idx = tok * D + o
                    if (abs(got[idx] - want[idx]) > 2e-3 * max(den, 1.0)) {
                        bad_tol++
                    }
                }
            }
            t |> equal(bad_tol, 0)                          // f16-tile envelope
            // small-ntok delegation: below the threshold the CPU fallback serves the call
            set_metal_gemm_min_ntok(1000l)
            let fb0 = metal_gemm_stats().fallbacks
            run_batch(got, wq, ws, xq, xs, N, D, NTOK)
            t |> success(metal_gemm_stats().fallbacks > fb0, "small batches delegate to the CPU kernel")
            var bad_fb = 0
            for (k in range(NTOK * D)) {
                if (got[k] != want[k]) {
                    bad_fb++
                }
            }
            t |> equal(bad_fb, 0)                           // the delegate IS the portable kernel
            // restore process-wide state (clear the pin AND re-activate to restore the slot) +
            // the leak gate
            select_batch_backend("")
            select_kernel_backend(active_kernel_backend())
            set_metal_gemm_min_ntok(32l)
            metal_gemm_shutdown()
            t |> equal(metal_log_leaks_if_any(), 0l)
            delete wq
            delete ws
            delete xq
            delete xs
            delete want
            delete got
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}
