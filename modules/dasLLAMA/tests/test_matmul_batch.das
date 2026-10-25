options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default  // registers the portable Q8·Q8 backend (the matmul_q8q8 wrappers dispatch to it)
require dasllama/dasllama_math_gen   // bf16w_tileform_gen: the bf16 plane's walk rounds its activations where the half-width family serves
require dasllama/dasllama_convert
require daslib/jobque_boost
require _compares   // row_fill

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
[test]
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
[test]
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
//! where the box stamps the half-width family (`bf16w_tileform_gen() > 0`, every call of ntok past a
//! tile on an n of whole 32s) the oracle's activations are the family's bf16-rounded ones
def check_bf16(t : T?; n, d, ntok : int64) {
    let wf <- fillf(n * d)
    var x <- fillf(ntok * n)
    if (bf16w_tileform_gen() > 0 && n % 32l == 0l && ntok > 1l && ntok >= int64(bf16w_tokstep_gen())) {
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

[test]
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
