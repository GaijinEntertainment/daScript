options gen2
options persistent_heap
options indenting = 4
options _dasllama_internal = true
options stack = 524288

// The CUDA kernel home's cells: the shared GEMV, GEMM, flash and codec bodies stamped under
// [cuda_kernel] / [cuda_dispatch] against the same CPU oracles the Vulkan cells hold them to.
// Model-free; every cell but the closing leak gate skips without a CUDA device and NVRTC, and the
// per-format cells (the kq cm2 tiles, the kq and per-32 GEMVs, the token-table gathers, the Q8_K
// feeds) also skip without the dasVulkan module, whose cells carry their oracles. The leak gate
// never skips.

require dastest/testing_boost public
require math
require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_decode
require daslib/shader_lingua_franca
require _kernel_oracles
require _vk_kq_fixtures
require dasllama/dasllama_gpu_tier
require _compares
require _attn_oracle
require ?vulkan _vkd_oracles   // the kq oracle runs the Vulkan leaves' decodes on the CPU
require dasllama/dasllama_math
require daslib/math_bits   // nolint:STYLE030,LINT019 — the word hashes of the CUDA cells, inside the vulkan arm alone
require dasllama/dasllama_kqformat
require dasllama/dasllama_gpu_math   // ue4m3_half_f32 - the nv4 MMQ cell's reference scales
require _fp4_feed   // the FP4 feed kernels' CPU oracle
require dasllama/dasllama_gemm_schema   // nolint:STYLE029,LINT019 — the schedule widths, read only inside the vulkan arm
require dasllama/dasllama_convert   // nolint:STYLE029,LINT019 — the plane converters, read only inside the vulkan arm
require dasllama/dasllama_gpu_kernels_discrete
require _kv_codec_oracles
require dasllama/dasllama_kv_dtype
require daslib/f16_cvt
require daslib/algorithm
require dasllama/dasllama_common

let Y_SENTINEL = 12345.0   //! a row the GEMV never stores keeps it
let private Q8_GEMV_BAR = Bar(rel = 1e-3, poison_of_max = true, poison = 2.0, poison_abs = 1.0)   //! the q8 GEMV and MMQ cells' bar: 1e-3 of the larger magnitude floored at 1
let private TILE_BAR = Bar(kind = BarKind.rel_of_max, rel = 2e-3, abs_floor = 2e-3, poison_of_max = true, poison = 2.0, poison_abs = 1.0)   //! the f16 tiles reassociate: 2e-3 of the plane's largest magnitude floored at 1

//! `nreg` regions of `d` rows, `nb` blocks a row over the ramp scale plane, the activation scales `(i % 5) x 0.125 - 0.25`
def q8_fix(nreg, d, nb : int) : GemvFix {
    var wsh <- ramp_ws_plane(nreg * d * nb)
    return <- gemv_fix(nreg, d, nb, 8, 55u, wsh, nb, 8, 17u, 9u, -0.25, 0.125, 5)
}

//! the device buffers of a fixture in the GEMV classes' binding order, the output under the sentinel
def q8_upload(f : GemvFix; rows : int) : uint64[6] =>
    fixed_array(buf_upload(f.wqh, g_cud.stream), buf_upload(f.wsh, g_cud.stream), buf_upload(f.regs, g_cud.stream), buf_upload(f.xqh, g_cud.stream), buf_upload(f.xsh, g_cud.stream), buf_fill(rows, Y_SENTINEL, g_cud.stream))

def q8_gemv_run(t : T?; nreg, d, nb : int) {
    let n = nb * 32
    let rows = nreg * d
    var f <- q8_fix(nreg, d, nb)
    var y_ref : array<float>
    y_ref |> resize(rows)
    var rec <- [0u, 0u, 1u, 0u]   // one record of one activation row: the regions' weight rows lie end to end
    q8_gemm_oracle(f.wqh, f.wsh, f.xqh, f.xsh, rec, 1, n, rows, y_ref)
    var bufs = q8_upload(f, rows)
    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u)
    let before = cud_kernel_coverage_of("q8_gemv_cls_cuda")
    enc_q8_gemv_cls(g_cud.stream, bufs, pc, cud_gemv_wgs(int64(rows), 0u))
    t |> success(cud_sync(), "q8 GEMV n {n} d {d} x {nreg}: the launch completes ({cuda_last_error()})")
    t |> equal(cud_kernel_coverage_of("q8_gemv_cls_cuda"), before + 1, "the census counts the launch")
    var y <- buf_download(bufs[5], rows, type<float>)
    cuda_mem_free(bufs)
    var stale = 0
    for (v in y) {
        if (v == Y_SENTINEL) {
            stale++
        }
    }
    t |> equal(stale, 0, "q8 GEMV n {n} d {d} x {nreg}: every row stored")
    check_rows(t, "q8 GEMV n {n} d {d} x {nreg} vs the CPU oracle", y, y_ref, Q8_GEMV_BAR, 1)
    delete y
    delete y_ref
    delete rec
    delete f
}

[test]
def test_cud_q8_gemv(t0 : T?) {
    t0 |> run("CUDA q8 GEMV class kernel == the CPU oracle") <| @(t : T?) {
        if (!ensure_q8_gemv_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        q8_gemv_run(t, 2, 3, 16)      // the Vulkan cell's shape: six rows, two regions, a partial block
        q8_gemv_run(t, 1, 200, 64)    // many blocks, a partial last one
        q8_gemv_run(t, 3, 5, 8)       // three regions interleaved a row at a time
    }
}

//! the first measurement: the q8 GEMV at a llama-1B projection's shape, event-timed over repeated launches
[test]
def test_cud_q8_gemv_rate(t0 : T?) {
    t0 |> run("CUDA q8 GEMV at n 2048 d 2048: a rate, logged") <| @(t : T?) {
        if (!ensure_q8_gemv_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let nb = 64
        let d = 2048
        let n = nb * 32
        var f <- q8_fix(1, d, nb)
        var bufs = q8_upload(f, d)
        var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
        let wgs = cud_gemv_wgs(int64(d), 0u)
        enc_q8_gemv_cls(g_cud.stream, bufs, pc, wgs)   // the warm-up: the first launch pays the context's setup
        t |> success(cud_sync(), "the warm-up completes")
        var y <- buf_download(bufs[5], d, type<float>)
        var y_ref : array<float>
        y_ref |> resize(d)
        var rec <- [0u, 0u, 1u, 0u]
        q8_gemm_oracle(f.wqh, f.wsh, f.xqh, f.xsh, rec, 1, n, d, y_ref)
        check_rows(t, "the warm-up's rows at the rate shape vs the CPU oracle", y, y_ref, Q8_GEMV_BAR, 1)
        delete rec
        delete y_ref
        delete y
        let reps = 50
        let ms = cud_time_ms() {
            for (_i in range(reps)) {
                enc_q8_gemv_cls(g_cud.stream, bufs, pc, wgs)
            }
        }
        let bytes = double(d) * double(n) * (1.0lf + 2.0lf / 32.0lf) + double(n) * (1.0lf + 4.0lf / 32.0lf)
        let per = ms / float(reps)
        let gbs = float(bytes / double(per) / 1.0e6lf)
        to_log(LOG_WARNING, "cuda q8 GEMV n {n} d {d} on {g_cud.name}: {per} ms a launch, {gbs} GB/s of weights\n")
        t |> success(per > 0.0, "the timing reads")
        cuda_mem_free(bufs)
        delete f
    }
}

let TILE_D = 160            //! the tile cells' weight rows: one full 128-tile and a 32-row remainder (the Vulkan cells' shape)
let TILE_ROWS0 = 300        //! the two regions' rows: whole tiles and an edge each
let TILE_ROWS1 = 260

//! the cm2 l tile over two regions of activation rows against the f16-feed oracle: `k` the reduction width; the tile's
//! f16 accumulator reads 1.7e-3 of the largest output at k 576 against the 2e-3 bar, so a fixture change here moves the margin
[arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-tile-body")]
def q8_cm2l_run(t : T?; k : int; wq_seed, ws_seed, xf_mul, xf_add : uint) {
    let d = TILE_D
    let rows = TILE_ROWS0 + TILE_ROWS1
    var wqh, wsh : array<uint>
    q8_plane(2 * d * (k / 32), 1u, wq_seed, 1u, ws_seed, wqh, wsh)
    var xfh <- ws_plane((rows + int(TILE_READ_SLACK)) * k / 2, xf_mul, xf_add)
    let tail = 2
    var sr <- two_region_sched(TILE_ROWS0, TILE_ROWS1, d * (k / 32), 256, (d + 127) / 128, tail)
    let wgs = sr.wgs
    var sched <- sr.words
    for (w in range(wgs, wgs + tail)) {
        sched[8 + w] = SCHED_NONE
    }
    var y_ref : array<float>
    y_ref |> resize(rows * d)
    q8f16_gemm_oracle(wqh, wsh, xfh, sched, 2, k, d, y_ref)
    let s = g_cud.stream
    var bufs = fixed_array(buf_upload(wqh, s), buf_upload(wsh, s), buf_upload(sched, s), buf_upload(xfh, s), buf_fill(rows * d, Y_SENTINEL, s))
    var pc = BatchArgs(n = uint(k), d = uint(d), map_off = uint(2 * 4))
    enc_q8_batch_cm2l_cls(g_cud.stream, bufs, pc, int64(wgs + tail))
    t |> success(cud_sync(), "q8 cm2 l tile k {k}: the launch completes ({cuda_last_error()})")
    var y <- buf_download(bufs[4], rows * d, type<float>)
    cuda_mem_free(bufs)
    var stale = 0
    for (v in y) {
        if (v == Y_SENTINEL) {
            stale++
        }
    }
    t |> equal(stale, 0, "q8 cm2 l tile k {k}: every row stored")
    check_rows(t, "q8 cm2 l tile k {k} vs the f16-feed oracle", y, y_ref, TILE_BAR, d)
    delete y
    delete y_ref
    delete sched
    delete xfh
    delete wqh
    delete wsh
}

//! the two-chunk split through the reduce against the whole-K oracle: the k0 / k1 / ybase prologue the window chain's split GEMMs ride
def q8_cm2l_split_run(t : T?; k : int) {
    let d = TILE_D
    let rows = TILE_ROWS0
    let nsplit = 2
    let ck = int(cm2_chunk(int64(k), int64(nsplit)))
    t |> success(ck > 0 && ck % 256 == 0 && ck < k, "k {k} splits in two 256-aligned chunks ({ck} and {k - ck} deep)")
    var wqh, wsh : array<uint>
    q8_plane(d * (k / 32), 1u, 17u, 1u, 991u, wqh, wsh)
    var xfh <- ws_plane((rows + int(TILE_READ_SLACK)) * k / 2, 3u, 5u)
    let wgs = ((rows + 255) / 256) * ((d + 127) / 128)
    var sched : array<uint>
    sched |> resize(4 + wgs * nsplit)
    sched[2] = uint(rows)
    var y_ref : array<float>
    y_ref |> resize(rows * d)
    q8f16_gemm_oracle(wqh, wsh, xfh, sched, 1, k, d, y_ref)
    let s = g_cud.stream
    var bufs = fixed_array(buf_upload(wqh, s), buf_upload(wsh, s), buf_upload(sched, s), buf_upload(xfh, s), buf_fill(rows * d * nsplit, Y_SENTINEL, s))
    let y = buf_fill(rows * d, Y_SENTINEL, s)
    var pc = BatchArgs(n = uint(k), d = uint(d), map_off = 4u, ksplit = uint(ck))
    enc_q8_batch_cm2l_cls(g_cud.stream, bufs, pc, int64(wgs * nsplit))
    var pr = SkRedArgs(nelem = uint(rows * d), k = uint(nsplit), ybase = 0u)
    enc_splitk_reduce_cls(g_cud.stream, fixed_array(bufs[4], y), pr, int64((rows * d + 1023) / 1024))
    t |> success(cud_sync(), "q8 cm2 l tile split k {k}: the launches complete ({cuda_last_error()})")
    var got <- buf_download(y, rows * d, type<float>)
    cuda_mem_free(bufs)
    cuda_mem_free(y)
    check_rows(t, "q8 cm2 l tile split in two k chunks at k {k} + the reduce vs the whole-K oracle", got, y_ref, TILE_BAR, d)
    delete got
    delete y_ref
    delete sched
    delete xfh
    delete wqh
    delete wsh
}

[test]
def test_cud_q8_cm2l_split(t0 : T?) {
    t0 |> run("CUDA q8 cm2 l-tile split in two k chunks + the reduce == the f16-feed CPU oracle") <| @(t : T?) {
        if (!ensure_q8_batch_cm2l_cls() || !ensure_splitk_reduce_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        q8_cm2l_split_run(t, 576)   // the two-chunk split of the Vulkan cell's K: 512 and 64 deep
        q8_cm2l_split_run(t, 2048)
    }
}

[test]
def test_cud_q8_cm2l_batch(t0 : T?) {
    t0 |> run("CUDA q8 cm2 l-tile class kernel == the f16-feed CPU oracle") <| @(t : T?) {
        if (!ensure_q8_batch_cm2l_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        q8_cm2l_run(t, 576, 17u, 991u, 3u, 5u)   // the Vulkan cell's shape: K off the 256 lattice, a 64-deep tail
    }
}

//! the reference: each block's exact int32 dot scaled by its weight and activation scales, summed in f32
[test]
def test_cud_q8_mmq(t0 : T?) {
    t0 |> run("CUDA MMQ-shaped q8 GEMM == the block-scaled CPU reference") <| @(t : T?) {
        if (!ensure_q8_mmq()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let k = 768   // twelve 64-wide steps, three scale groups of four: the two-deep ring wraps five times
        let d = 256
        let rows = 256
        let nb = k / 32
        var wq, ws : array<uint>
        q8_plane(d * nb, 1u, 17u, 1u, 991u, wq, ws)
        var xq <- hashed_words(rows * k / 4, 7u, 3u)
        var xs <- row_fill(rows * nb, 1, 0, 0.125, 0.25, 5)
        let s = g_cud.stream
        var bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(xq, s), buf_upload(xs, s), buf_fill(rows * d, Y_SENTINEL, s))
        var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(rows))
        enc_q8_mmq(g_cud.stream, bufs, pc, mmq_wgs(int64(d), int64(rows)))
        t |> success(cud_sync(), "mmq: the launch completes ({cuda_last_error()})")
        let y <- buf_download(bufs[4], rows * d, type<float>)
        cuda_mem_free(bufs)
        var want : array<float>
        want |> resize(rows * d)
        for (tok in range(rows)) {
            for (row in range(d)) {
                var acc = 0.0
                for (blk in range(nb)) {
                    var dot = 0
                    for (i in range(32)) {
                        let wi = (row * nb + blk) * 32 + i
                        let xi = tok * k + blk * 32 + i
                        let wb = int((wq[wi / 4] >> uint((wi % 4) * 8)) & 255u)
                        let xb = int((xq[xi / 4] >> uint((xi % 4) * 8)) & 255u)
                        dot += (wb >= 128 ? wb - 256 : wb) * (xb >= 128 ? xb - 256 : xb)
                    }
                    let wsi = row * nb + blk
                    let v = unpackHalf2x16(ws[wsi / 2])
                    acc += float(dot) * ((wsi & 1) == 0 ? v.x : v.y) * xs[tok * nb + blk]
                }
                want[tok * d + row] = acc
            }
        }
        check_rows(t, "q8 MMQ vs the block-scaled reference", y, want, Q8_GEMV_BAR, d)
    }
}

//! a UE4M3 scale word of four codes in [0x30, 0x40) - values 0.5 .. 2 at the eighth, every product short and dyadic
def private nv4_test_scale_word(i : int) : uint {
    var w = 0u
    for (s in range(4)) {
        w |= (0x30u + uint((i * 4 + s * 3 + i / 7) % 16)) << uint(s * 8)
    }
    return w
}

//! the NVFP4 leaf on the FP4 tensor rung: the k4-tiled nv4 plane (a 32-block's 16 bytes: element i the low nibble, i + 16 the
//! high; the superblock's sixteen UE4M3 strip codes as four words) against an FP4 feed in the (k, k + 8) pairing, the
//! reference every element's e2m1 x UE4M3 on both sides, the token's row scale and the plane's factor over the sum
[test]
def test_cud_nv4_mmq(t0 : T?) {
    t0 |> run("CUDA NVFP4 GEMM on the FP4 tensor rung == the e2m1 x UE4M3 CPU reference under the row scales and the plane's factor") <| @(t : T?) {
        if (!ensure_nv4_mmq()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let k = 768   // three superblocks: six k128 steps, the two-deep ring wraps
        let d = 256
        let rows = 256
        let nsb = k / 256
        let s2 = 0.75
        var wq <- hashed_words(d * nsb * 32, 1u, 23u)
        var ws : array<uint>
        ws |> resize(d * nsb * 5)
        for (sb in range(d * nsb)) {
            for (j in range(4)) {
                ws[sb * 5 + j] = nv4_test_scale_word(sb * 4 + j)
            }
        }
        var xq <- hashed_words(rows * k / 8, 5u, 7u)
        var xs : array<uint>
        xs |> resize(rows * k / 64)
        for (i in range(rows * k / 64)) {
            xs[i] = nv4_test_scale_word(i + 3)
        }
        var xr <- [for (tok in range(rows)); 0.5 + 0.125 * float(tok % 7)]
        let s = g_cud.stream
        var bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(xq, s), buf_upload(xs, s), buf_upload(xr, s), buf_fill(rows * d, Y_SENTINEL, s))
        var pc = MmqFp4Args(n = uint(k), d = uint(d), rows = uint(rows), wblk = 0u, s2 = s2)
        enc_nv4_mmq(g_cud.stream, bufs, pc, mmq_wgs(int64(d), int64(rows)))
        t |> success(cud_sync(), "nv4 mmq: the launch completes ({cuda_last_error()})")
        let y <- buf_download(bufs[5], rows * d, type<float>)
        cuda_mem_free(bufs)
        var want : array<float>
        want |> resize(rows * d)
        for (tok in range(rows)) {
            for (row in range(d)) {
                var acc = 0.0
                for (kk in range(k)) {
                    let sb = kk / 256
                    let g = (kk % 256) / 32
                    let e = kk % 32
                    let wbyte = (wq[((row * nsb + sb) * 8 + g) * 4 + (e % 16) / 4] >> uint(((e % 16) % 4) * 8 + (e / 16) * 4)) & 15u
                    let wstrip = g * 2 + e / 16
                    let wsc = (ws[(row * nsb + sb) * 5 + wstrip / 4] >> uint((wstrip % 4) * 8)) & 0xFFu
                    let j = kk % 16
                    let xbi = tok * (k / 2) + (kk / 16) * 8 + j % 8   // the feed's byte: strip kk / 16, pair j % 8, the high nibble past 7
                    let xnib = (xq[xbi / 4] >> uint((xbi % 4) * 8 + (j / 8) * 4)) & 15u
                    let xsc = (xs[tok * (k / 64) + kk / 64] >> uint(((kk / 16) % 4) * 8)) & 0xFFu
                    acc += float(MX4_LUT[int(wbyte)]) * ue4m3_half_f32(wsc) * float(MX4_LUT[int(xnib)]) * ue4m3_half_f32(xsc)
                }
                want[tok * d + row] = acc * xr[tok] * s2
            }
        }
        check_rows(t, "nv4 MMQ on the FP4 rung vs the e2m1 x UE4M3 reference", y, want, Q8_GEMV_BAR, d)
    }
}

//! the Q4_K leaf over the engine's repacked superblocks (a 32-block's 16 bytes: element i the low nibble, i + 16 the high; five
//! scale words a superblock) against the GEMV's fold of each block: xs x (d sc dot - dmin mn xsum)
[test]
def test_cud_k4_mmq(t0 : T?) {
    t0 |> run("CUDA MMQ-shaped Q4_K GEMM == the block-scaled CPU reference") <| @(t : T?) {
        if (!ensure_k4_mmq()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let k = 1024   // four superblocks: the scale rows hand over three times, the min terms fold in two groups of sixteen blocks
        let d = 256
        let rows = 256
        let nsb = k / 256
        let nb = k / 32
        var wq <- hashed_words(d * nsb * 32, 11u, 5u)   // eight uint4 a superblock
        var ws : array<uint>
        ws |> resize(d * nsb * 5)
        for (sb in range(d * nsb)) {
            ws[sb * 5] = packHalf2x16(float2(0.002 + 0.001 * float(sb % 7), 0.0005 + 0.0003 * float(sb % 5)))
            for (g in range(8)) {
                let sc = uint((sb * 3 + g * 7) % 64)
                let mn = uint((sb * 5 + g * 11) % 64)
                ws[sb * 5 + 1 + g / 4] |= sc << uint((g % 4) * 8)
                ws[sb * 5 + 3 + g / 4] |= mn << uint((g % 4) * 8)
            }
        }
        var xq <- hashed_words(rows * k / 4, 7u, 3u)
        var xs <- row_fill(rows * nb, 1, 0, 0.125, 0.25, 5)
        var xsum : array<float>
        xsum |> resize(rows * nb)
        for (tok in range(rows)) {
            for (blk in range(nb)) {
                var sm = 0
                for (i in range(32)) {
                    let xi = tok * k + blk * 32 + i
                    let xb = int((xq[xi / 4] >> uint((xi % 4) * 8)) & 255u)
                    sm += xb >= 128 ? xb - 256 : xb
                }
                xsum[tok * nb + blk] = xs[tok * nb + blk] * float(sm)
            }
        }
        let s = g_cud.stream
        var bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(xq, s), buf_upload(xs, s), buf_fill(rows * d, Y_SENTINEL, s), buf_upload(xsum, s))
        var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(rows))
        enc_k4_mmq(g_cud.stream, bufs, pc, mmq_wgs(int64(d), int64(rows)))
        t |> success(cud_sync(), "k4 mmq: the launch completes ({cuda_last_error()})")
        let y <- buf_download(bufs[4], rows * d, type<float>)
        cuda_mem_free(bufs)
        var want : array<float>
        want |> resize(rows * d)
        for (tok in range(rows)) {
            for (row in range(d)) {
                var acc = 0.0
                for (blk in range(nb)) {
                    let sb = row * nsb + blk / 8
                    let g = blk % 8
                    let dm = unpackHalf2x16(ws[sb * 5])
                    let sc = float((ws[sb * 5 + 1 + g / 4] >> uint((g % 4) * 8)) & 255u)
                    let mn = float((ws[sb * 5 + 3 + g / 4] >> uint((g % 4) * 8)) & 255u)
                    var dot = 0
                    var xsm = 0
                    for (i in range(32)) {
                        let wb = (sb * 8 + g) * 16 + i % 16
                        let byte = int((wq[wb / 4] >> uint((wb % 4) * 8)) & 255u)
                        let code = i < 16 ? byte & 15 : byte >> 4
                        let xi = tok * k + blk * 32 + i
                        let xb = int((xq[xi / 4] >> uint((xi % 4) * 8)) & 255u)
                        let xv = xb >= 128 ? xb - 256 : xb
                        dot += code * xv
                        xsm += xv
                    }
                    acc += xs[tok * nb + blk] * (dm.x * sc * float(dot) - dm.y * mn * float(xsm))
                }
                want[tok * d + row] = acc
            }
        }
        check_rows(t, "k4 MMQ vs the block-scaled reference", y, want, TILE_BAR, d)   // the min terms are f16-staged: the f16 tiles' bar, not the Q8 leaf's per-element one
    }
}

//! one superblock leaf without a min term over the Q4_K nibble tiling: IQ4_XS (the codebook; a two-word scale row - d and the
//! scales' high bits, the scales' low nibbles), Q4_0 (the nibbles less eight; the d-pair row) or IQ4_NL (the codebook; the
//! d-pair row); the reference folds each block as xs x scale x dot
def private cud_sb_mmq_run(t : T?; f : KqFmt) {
    let k = 1024   // four superblocks: the scale rows hand over three times
    let d = 256
    let rows = 256
    let nsb = k / 256
    let nb = k / 32
    let lut = f != KqFmt.q40
    let xs2 = f == KqFmt.iq4xs
    var wq <- hashed_words(d * nsb * 32, 17u, 9u + uint(f))   // eight uint4 a superblock
    var ws <- hashed_words(d * nsb * (xs2 ? 2 : 5), 5u, 23u)   // IQ4_XS: the scale bits hashed, d in the first word's low half; else four words of d pairs and a pad
    for (sb in range(d * nsb)) {
        if (xs2) {
            ws[sb * 2] = (ws[sb * 2] & 0xFFFF0000u) | (packHalf2x16(float2(0.002 + 0.001 * float(sb % 7), 0.0)) & 0xFFFFu)
        } else {
            for (j in range(4)) {
                ws[sb * 5 + j] = packHalf2x16(float2(0.002 + 0.001 * float((sb + 2 * j) % 7), 0.0015 + 0.001 * float((sb + 2 * j + 1) % 5)))
            }
            ws[sb * 5 + 4] = 0u
        }
    }
    var xq <- hashed_words(rows * k / 4, 7u, 3u)
    var xs <- row_fill(rows * nb, 1, 0, 0.125, 0.25, 5)
    var s = g_cud.stream
    var bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(xq, s), buf_upload(xs, s), buf_fill(rows * d, Y_SENTINEL, s))
    var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(rows))
    let wgs = mmq_wgs(int64(d), int64(rows))
    if (xs2) {
        enc_iq4xs_mmq(s, bufs, pc, wgs)
    } elif (lut) {
        enc_iq4nl_mmq(s, bufs, pc, wgs)
    } else {
        enc_q40_mmq(s, bufs, pc, wgs)
    }
    t |> success(cud_sync(), "{f} mmq: the launch completes ({cuda_last_error()})")
    let y <- buf_download(bufs[4], rows * d, type<float>)
    cuda_mem_free(bufs)
    var want : array<float>
    want |> resize(rows * d)
    for (tok in range(rows)) {
        for (row in range(d)) {
            var acc = 0.0
            for (blk in range(nb)) {
                let sb = row * nsb + blk / 8
                let g = blk % 8
                var dot = 0
                for (i in range(32)) {
                    let wb = (sb * 8 + g) * 16 + i % 16
                    let byte = int((wq[wb / 4] >> uint((wb % 4) * 8)) & 255u)
                    let nib = i < 16 ? byte & 15 : byte >> 4
                    let code = lut ? int(iq4_word(uint(nib)) & 255u) : nib - 8
                    let xi = tok * k + blk * 32 + i
                    let xb = int((xq[xi / 4] >> uint((xi % 4) * 8)) & 255u)
                    dot += (code >= 128 ? code - 256 : code) * (xb >= 128 ? xb - 256 : xb)
                }
                let sc = xs2 ? unpackHalf2x16(ws[sb * 2]).x * float(iq4xs_sc(ws[sb * 2], ws[sb * 2 + 1], uint(g))) : q40_d_of(ws[sb * 5 + g / 2], uint(g))
                acc += float(dot) * sc * xs[tok * nb + blk]
            }
            want[tok * d + row] = acc
        }
    }
    check_rows(t, "{f} MMQ vs the block-scaled reference", y, want, Q8_GEMV_BAR, d)
}

[test]
def test_cud_sb_mmq(t0 : T?) {
    t0 |> run("CUDA MMQ-shaped superblock GEMMs without a min term (IQ4_XS, Q4_0, IQ4_NL) == the block-scaled CPU reference") <| @(t : T?) {
        if (!(ensure_iq4xs_mmq() && ensure_q40_mmq() && ensure_iq4nl_mmq())) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        for (f in [KqFmt.iq4xs, KqFmt.q40, KqFmt.iq4nl]) {
            cud_sb_mmq_run(t, f)
        }
    }
}

//! one per-32 leaf against the engine's own per-32 dequant of each block dotted with the activation quants: Q5_1 with its m
//! through the f16-staged correction (the f16 tiles' bar), Q4_0, Q5_0 and IQ4_NL per element
def private cud_b32_mmq_run(t : T?; f : KqFmt) {
    let fmt = int(f)
    let k = 1024   // thirty-two blocks a row: the min term's two groups of sixteen
    let d = 256
    let rows = 256
    let nb = k / 32
    let qwords = int(kq_qsb(f) / 4l)
    var wq <- hashed_words(d * nb * qwords, 19u, uint(fmt) * 31u)
    // the scale plane in the format's form: Q5_1 a d | m word a block, the others two d halves a word
    var ws <- (f == KqFmt.q51
        ? [for (b in range(d * nb)); packHalf2x16(float2(0.002 * float(1 + b % 7), 0.001 * float(b % 5) - 0.002))]
        : [for (j in range(d * nb / 2)); packHalf2x16(float2(0.002 * float(1 + (2 * j) % 7), 0.002 * float(1 + (2 * j + 1) % 7)))])
    var xq <- hashed_words(rows * k / 4, 7u, 3u)
    var xs <- row_fill(rows * nb, 1, 0, 0.125, 0.25, 5)
    var xsum : array<float>
    xsum |> resize(rows * nb)
    for (tok in range(rows)) {
        for (blk in range(nb)) {
            var sm = 0
            for (i in range(32)) {
                let xi = tok * k + blk * 32 + i
                let xb = int((xq[xi / 4] >> uint((xi % 4) * 8)) & 255u)
                sm += xb >= 128 ? xb - 256 : xb
            }
            xsum[tok * nb + blk] = xs[tok * nb + blk] * float(sm)
        }
    }
    var s = g_cud.stream
    var bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(xq, s), buf_upload(xs, s), buf_fill(rows * d, Y_SENTINEL, s), buf_upload(xsum, s))
    var b5 = fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4])
    var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(rows))
    let wgs = mmq_wgs(int64(d), int64(rows))
    if (f == KqFmt.q51) {
        enc_q51_mmq(s, bufs, pc, wgs)
    } elif (f == KqFmt.q4032) {
        enc_q4032_mmq(s, b5, pc, wgs)
    } elif (f == KqFmt.q5032) {
        enc_q5032_mmq(s, b5, pc, wgs)
    } else {
        enc_iq4nl32_mmq(s, b5, pc, wgs)
    }
    t |> success(cud_sync(), "{f} mmq: the launch completes ({cuda_last_error()})")
    let y <- buf_download(bufs[4], rows * d, type<float>)
    cuda_mem_free(bufs)
    let kqb <- [for (i in range(length(wq) * 4)); uint8((wq[i / 4] >> uint((i % 4) * 8)) & 0xFFu)]
    let ksb <- [for (i in range(length(ws) * 4)); uint8((ws[i / 4] >> uint((i % 4) * 8)) & 0xFFu)]
    var want : array<float>
    want |> resize(rows * d)
    var w : array<float>
    w |> resize(32)
    for (row in range(d)) {
        for (blk in range(nb)) {
            dequant_b32_plane_block(f, kqb, ksb, int64(row * nb + blk), w, 0l)
            for (tok in range(rows)) {
                var dot = 0.0
                for (i in range(32)) {
                    let xi = tok * k + blk * 32 + i
                    let xb = int((xq[xi / 4] >> uint((xi % 4) * 8)) & 255u)
                    dot += w[i] * float(xb >= 128 ? xb - 256 : xb)
                }
                want[tok * d + row] += dot * xs[tok * nb + blk]
            }
        }
    }
    check_rows(t, "{f} MMQ vs the dequantized reference", y, want, f == KqFmt.q51 ? TILE_BAR : Q8_GEMV_BAR, d)
}

[test]
def test_cud_b32_mmq(t0 : T?) {
    t0 |> run("CUDA MMQ-shaped per-32 GEMMs (Q5_1, Q4_0, Q5_0, IQ4_NL) == the dequantized CPU reference") <| @(t : T?) {
        if (!(ensure_q51_mmq() && ensure_q4032_mmq() && ensure_q5032_mmq() && ensure_iq4nl32_mmq())) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        for (f in [KqFmt.q51, KqFmt.q4032, KqFmt.q5032, KqFmt.iq4nl32]) {
            cud_b32_mmq_run(t, f)
        }
    }
}

//! the K-quant cm2 tiles (the shared decode-in-load templates) on the l, m and s columns against the Vulkan cells' per-format oracles,
//! over the Vulkan cells' hashed planes and two regions of activation rows (whole tiles and an edge each)
[test]
def test_cud_kq_cm2_family(t0 : T?) {
    t0 |> run("CUDA kq cm2 l/m/s tiles == the CPU oracle (every superblock format)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cm2l_cls()) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            let d = TILE_CELL_D
            let rows = TILE_CELL_ROWS0 + TILE_CELL_ROWS1
            let tail = 2
            var tiles <- fmt_tile_rows()
            for (row in tiles) {
                continue if (row.fmt == KqFmt.mx4)   // an expert-only form: no dense plane on the slice, no tile
                let fmt = int(row.fmt)
                var wqh, wsh : array<uint>
                fmt_tile_planes(row, wqh, wsh)
                var xfh <- ws_plane((rows + int(TILE_READ_SLACK)) * row.n / 2, 3u, row.xf_add)
                for (tc in range(3)) {
                    t |> success(cud_cm2_ensure(fmt, tc), "{row.fmt} cm2 tile {tc}: the leaf compiles")
                    let rows_per_wg = tc == 0 ? 256 : (tc == 1 ? 128 : 32)
                    var sr <- two_region_sched(TILE_CELL_ROWS0, TILE_CELL_ROWS1, d * (row.n / row.blk_k), rows_per_wg, (d + 127) / 128, tail)
                    let wgs = sr.wgs
                    var sched <- sr.words
                    for (w in range(wgs, wgs + tail)) {
                        sched[8 + w] = SCHED_NONE
                    }
                    var y_ref : array<float>
                    y_ref |> resize(rows * d)
                    invoke(row.oracle, wqh, wsh, xfh, sched, 2, row.n, d, y_ref)
                    var s = g_cud.stream
                    var bufs = fixed_array(buf_upload(wqh, s), buf_upload(wsh, s), buf_upload(sched, s), buf_upload(xfh, s), buf_fill(rows * d, Y_SENTINEL, s))
                    var pc = BatchArgs(n = uint(row.n), d = uint(d), map_off = uint(2 * 4))
                    cud_cm2_enc(fmt, tc, s, bufs, pc, int64(wgs + tail))
                    let tag = "{row.fmt} cm2 {tc == 0 ? "l" : (tc == 1 ? "m" : "s")} tile"
                    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
                    var y <- buf_download(bufs[4], rows * d, type<float>)
                    cuda_mem_free(bufs)
                    check_rows(t, "{tag} vs the CPU oracle", y, y_ref, cm_bar(y_ref), d)
                    delete y
                    delete y_ref
                    delete sched
                }
                delete xfh
                delete wqh
                delete wsh
            }
            delete tiles
        } else {
            t |> skip("dasVulkan not present: the per-format oracles live with the Vulkan cells")
        }
    }
}

//! the K-quant decode GEMV family (the shared `KqGemvLeafT` leaves) against the Vulkan cell's CPU oracle, every superblock
//! format at the three lane splits, 256, 512 and 5120 wide - the Vulkan cell's shapes
[test]
def test_cud_kq_gemv_family(t0 : T?) {
    t0 |> run("CUDA kq GEMV leaves == the CPU oracle (all 15 formats)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_cls()) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            let rows_per_region = 3
            let nreg = 2
            let rows = nreg * rows_per_region
            for (fmt in kq_leaf_fmts()) {
                t |> success(cud_gemv_ensure(fmt), "fmt {fmt}: the leaf compiles")
                for (nsb in fixed_array(1, 2, 20)) {
                    let sb_words = int(kq_qsb(kq_fmt_of_slot(fmt)) / 4l)
                    let ws_words_n = rows * nsb * int(kq_dev_ssb_i(fmt) / 4l)
                    var inscope wsh <- kq_ws_plane(fmt, ws_words_n, 1u, uint(fmt) * 13u)
                    var f <- gemv_fix(nreg, rows_per_region, nsb, sb_words, uint(fmt) * 77777u, wsh, nsb, 64, 31u, 5u, 0.5, 0.25, 7)
                    var inscope y_ref <- kq_gemv_oracle(fmt, f.wqh, f.wsh, f.xqh, f.xsh, f.regs, nsb * 256, rows_per_region, nreg)
                    var s = g_cud.stream
                    var bufs = fixed_array(buf_upload(f.wqh, s), buf_upload(f.wsh, s), buf_upload(f.regs, s), buf_upload(f.xqh, s), buf_upload(f.xsh, s), buf_fill(rows, Y_SENTINEL, s))
                    for (lanes_per_row in fixed_array(0u, 16u, 8u)) {
                        cuda_mem_free(bufs[5])
                        bufs[5] = buf_fill(rows, Y_SENTINEL, s)   // a lane split that stores no row leaves the sentinel, not the last split's rows
                        var pc = GemvArgs(n = uint(nsb * 256), d = uint(rows_per_region), nreg = uint(nreg), ybase = 0u, lanes_per_row = lanes_per_row)
                        cud_gemv_cls_enc(fmt, s, bufs, pc, cud_gemv_wgs(int64(rows), lanes_per_row))
                        t |> success(cud_sync(), "fmt {fmt} n {nsb * 256}: the launch completes ({cuda_last_error()})")
                        var inscope y <- buf_download(bufs[5], rows, type<float>)
                        let lanes = lanes_per_row == 0u ? g_cud.subgroup_size : int64(lanes_per_row)
                        check_rows(t, "fmt {fmt} n {nsb * 256} at {lanes} lanes per row vs the CPU oracle", y, y_ref, BAR_APPROX_CTRL, 1)
                    }
                    cuda_mem_free(bufs)
                    delete f
                }
            }
        } else {
            t |> skip("dasVulkan not present: the kq oracle runs its leaves on the CPU")
        }
    }
}

let private BAR_APPROX_CTRL = Bar(rel = APPROX_REL, poison_of_max = true, poison_abs = 0.5)   //! the Vulkan cells' approximate bar with its poison control

//! a per-32 GEMV cell's shape: the Vulkan cell's two widths, quant words a block, the plane seeds
struct private B32Cell {
    f : KqFmt
    n0, n1, qwords, wq_seed, xq_mul, xq_add : int
}

//! the per-32 formats' scalar dot of one row (the Vulkan cells' row oracles)
def private b32_row_oracle(f : KqFmt; wq, ws, xq : array<uint>; xs : array<float>; wb0, nb : int) : float {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (f == KqFmt.q51) return q51_gemv_row_oracle(wq, ws, xq, xs, wb0, nb)
        if (f == KqFmt.iq4nl32) return iq4nl32_gemv_row_oracle(wq, ws, xq, xs, wb0, nb)
        if (f == KqFmt.q4032) return q4032_gemv_row_oracle(wq, ws, xq, xs, wb0, nb)
        return q5032_gemv_row_oracle(wq, ws, xq, xs, wb0, nb)
    } else {
        return 0.0
    }
}

//! the per-32 decode GEMVs (the shared `Blk32GemvRunT` leaves) against the engine's scalar dots at the Vulkan cells' widths - a
//! subgroup a row, a lane a block over Q8_0 activations, two regions of three rows
[test]
def test_cud_b32_gemv_family(t0 : T?) {
    t0 |> run("CUDA per-32 GEMV leaves == the scalar dot's float order (q51, iq4nl32, q4032, q5032)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_cls()) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            let d = 3
            let nreg = 2
            let rows = nreg * d
            for (c in [B32Cell(f = KqFmt.q51, n0 = 704, n1 = 1408, qwords = 5, wq_seed = 61, xq_mul = 19, xq_add = 7),
                       B32Cell(f = KqFmt.iq4nl32, n0 = 640, n1 = 1280, qwords = 4, wq_seed = 67, xq_mul = 23, xq_add = 5),
                       B32Cell(f = KqFmt.q4032, n0 = 640, n1 = 896, qwords = 4, wq_seed = 71, xq_mul = 29, xq_add = 5),
                       B32Cell(f = KqFmt.q5032, n0 = 896, n1 = 1408, qwords = int(KQ_Q5032_QWORDS), wq_seed = 73, xq_mul = 31, xq_add = 5)]) {
                let fmt = int(c.f)
                t |> success(cud_gemv_ensure(fmt), "{c.f}: the leaf compiles")
                for (n in [c.n0, c.n1]) {
                    let nb = n / 32
                    var inscope wsh <- blk32_scale_plane(c.f, rows * nb)
                    var fx <- gemv_fix(nreg, d, nb, c.qwords, uint(c.wq_seed), wsh, nb, 8, uint(c.xq_mul), uint(c.xq_add), 0.25, 0.125, 5)
                    var inscope y_ref : array<float>
                    y_ref |> resize(rows)
                    for (r in range(nreg)) {
                        for (row in range(d)) {
                            y_ref[r * d + row] = b32_row_oracle(c.f, fx.wqh, fx.wsh, fx.xqh, fx.xsh, r * d * nb + row * nb, nb)
                        }
                    }
                    var s = g_cud.stream
                    var bufs = fixed_array(buf_upload(fx.wqh, s), buf_upload(fx.wsh, s), buf_upload(fx.regs, s), buf_upload(fx.xqh, s), buf_upload(fx.xsh, s), buf_fill(rows, Y_SENTINEL, s))
                    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u)
                    cud_gemv_cls_enc(fmt, s, bufs, pc, cud_gemv_wgs(int64(rows), 0u))
                    t |> success(cud_sync(), "{c.f} n {n}: the launch completes ({cuda_last_error()})")
                    var inscope y <- buf_download(bufs[5], rows, type<float>)
                    check_rows(t, "{c.f} GEMV at n {n} against the scalar dot", y, y_ref, BAR_APPROX_CTRL, FLAT_ROWS)
                    cuda_mem_free(bufs)
                    delete fx
                }
            }
        } else {
            t |> skip("dasVulkan not present: the per-32 oracles live with the Vulkan cells")
        }
    }
}

//! `rows` table rows of `n` gathered on the device off the plane pair at block `wblk0` through the format's gather leaf
def private cud_gather_rows(t : T?; fmt : int; wq, ws, ids : array<uint>; xh : array<float>; n, wblk0 : int; escale : float) : array<float> {
    var s = g_cud.stream
    let rows = length(ids)
    let bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(ids, s), buf_upload(xh, s), buf_fill(rows * n, Y_SENTINEL, s))
    cud_emb_enc(fmt, s, bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], int64(rows), int64(n), int64(wblk0), escale)
    t |> success(cud_sync(), "{kq_fmt_of_slot(fmt)} gather: the launch completes ({cuda_last_error()})")
    var y <- buf_download(bufs[4], rows * n, type<float>)
    cuda_mem_free(bufs)
    return <- y
}

//! the token-table gathers: every superblock format's `KqEmb<Fmt>` leaf against the class lane decode (the Vulkan cell's oracle)
//! and the per-32 leaves against the engine's plane dequant - four rows, three ids off a five-row table past a one-row lead,
//! one `EMB_ID_HOST` row copying the host's staged row
[test]
def test_cud_emb_family(t0 : T?) {
    t0 |> run("CUDA token-table gather leaves == the oracles (15 superblock formats, 4 per-32 ones)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_emb_gather_cls()) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            let vocab = 5
            let lead = 1
            let escale = 1.5
            var inscope ids <- [3u, 0u, EMB_ID_HOST, 4u]
            let rows = length(ids)
            let nsb = 2
            let n = nsb * 256
            var inscope xh <- row_fill(rows * n, 7, 3, 0.01, -0.5)
            for (fmt in kq_leaf_fmts()) {
                t |> success(cud_emb_ensure(fmt), "{kq_fmt_of_slot(fmt)}: the gather leaf compiles")
                let sb_words = int(kq_qsb(kq_fmt_of_slot(fmt)) / 4l)
                var inscope wq <- hashed_words((vocab + lead) * nsb * sb_words, 1u, uint(fmt) * 9973u)
                var inscope ws <- kq_ws_plane(fmt, (vocab + lead) * nsb * int(kq_dev_ssb_i(fmt) / 4l), 1u, uint(fmt) * 17u)
                var inscope y_ref <- emb_gather_oracle(fmt, wq, ws, ids, xh, n, uint(lead * nsb), escale)
                var inscope y <- cud_gather_rows(t, fmt, wq, ws, ids, xh, n, lead * nsb, escale)
                check_rows(t, "{kq_fmt_of_slot(fmt)} token-table gather against the class lane decode", y, y_ref, BAR_APPROX_CTRL, n)
            }
            let n32 = 96
            let nb = n32 / 32
            var inscope xh32 <- row_fill(rows * n32, 7, 3, 0.01, -0.5)
            for (f in [KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032]) {
                let fmt = int(f)
                t |> success(cud_emb_ensure(fmt), "{f}: the gather leaf compiles")
                let totblk = (vocab + lead) * nb
                let qwords = int(kq_qsb(f) / 4l)
                var inscope wq <- hashed_words(totblk * qwords, 1u, uint(f) * 9973u)
                var inscope ws <- blk32_scale_plane(f, totblk)
                var inscope kqb <- [for (i in range(totblk * qwords * 4)); uint8((wq[i / 4] >> uint((i % 4) * 8)) & 0xFFu)]
                var inscope ksb <- [for (i in range(length(ws) * 4)); uint8((ws[i / 4] >> uint((i % 4) * 8)) & 0xFFu)]
                var inscope y_ref : array<float>
                y_ref |> resize(rows * n32)
                for (r in range(rows)) {
                    if (ids[r] == EMB_ID_HOST) {
                        for (e in range(n32)) {
                            y_ref[r * n32 + e] = xh32[r * n32 + e]
                        }
                        continue
                    }
                    var inscope blk : array<float>
                    blk |> resize(32)
                    for (b in range(nb)) {
                        dequant_b32_plane_block(f, kqb, ksb, int64((lead + int(ids[r])) * nb + b), blk, 0l)
                        for (e in range(32)) {
                            y_ref[r * n32 + b * 32 + e] = blk[e] * escale
                        }
                    }
                }
                var inscope y <- cud_gather_rows(t, fmt, wq, ws, ids, xh32, n32, lead * nb, escale)
                check_rows(t, "{f} token-table gather against the engine's plane dequant", y, y_ref, BAR_APPROX_CTRL, n32)
            }
        } else {
            t |> skip("dasVulkan not present: the gather oracle runs the Vulkan leaves' decodes on the CPU")
        }
    }
}

let private I8_SENTINEL_BITS = 0x80808080u   //! a Q8 quant plane's fill: -128 in every byte, a value no requant produces

let private QBYTE_BAR = Bar(abs_floor = 1.0, poison = 3.0)   //! a requant byte within one step of the oracle's; the poison three steps off

//! the quant words' signed bytes as floats: the requant compares hold them byte by byte
def private qbyte_values(words : array<uint>) : array<float> => <- [for (i in range(length(words) * 4)); float(int8((words[i / 4] >> uint((i % 4) * 8)) & 0xFFu))]

//! the Q8_K requant of f32 rows: amax is order-free, the pack deterministic - exact
def private cud_feed_rq(t : T?; var s : void?; sent : float) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nsb = 4
        let nelem = nsb * 256
        var src <- row_fill(nelem, 61, 0, 0.05, 2.8, 113)
        var bufs = fixed_array(buf_upload(src, s), buf_fill(nelem / 4, sent, s), buf_fill(nsb, NAN_SENTINEL, s))
        var pc = RqArgs(inbase = 0u, nblk = uint(nsb), lo = 0.0, hi = 0.0)
        enc_cls_q8k_rq(s, bufs, pc, rq_wgs(true, int64(nelem)))
        t |> success(cud_sync(), "q8k requant: the launch completes ({cuda_last_error()})")
        var q <- buf_download(bufs[1], nelem / 4, type<uint>)
        var sc <- buf_download(bufs[2], nsb, type<float>)
        var q_ref : array<uint>
        var s_ref : array<float>
        requant_oracle(src, 0, nelem, true, q_ref, s_ref)
        check_rows(t, "q8k requant's quants vs the CPU oracle, bit-exact (a row a superblock)", qbyte_values(q), qbyte_values(q_ref), BAR_EXACT, 256)
        check_rows(t, "q8k requant's scales vs the CPU oracle, bit-exact (a row a superblock)", sc, s_ref, BAR_EXACT, 1)
        cuda_mem_free(bufs)
        delete src
        delete q
        delete sc
        delete q_ref
        delete s_ref
    }
}

//! the act + requant into a down plane's feed, Q8_K (`q8k`) or Q8_0
def private cud_feed_actrq(t : T?; var s : void?; sent : float; q8k : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nelem = 1024
        let nblk = q8k ? nelem / 256 : nelem / 32
        var gh <- row_fill(nelem, 29, 0, 0.21, 6.0, 61)
        var uh <- row_fill(nelem, 11, 0, 0.17, 4.0, 47)
        var bufs = fixed_array(buf_upload(gh, s), buf_upload(uh, s), buf_fill(nelem / 4, sent, s), buf_fill(nblk, NAN_SENTINEL, s))
        var pc = ActArgs(nelem = uint(nelem), act = ACT_SILU, nblk = uint(nblk), elem0 = 0u)
        if (q8k) {
            enc_q8k_actrq_cls(s, bufs, pc, rq_wgs(true, int64(nelem)))
        } else {
            enc_q8_actrq_cls(s, bufs, pc, rq_wgs(false, int64(nelem)))
        }
        let tag = q8k ? "q8k actrq" : "q8 actrq"
        t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
        var q <- buf_download(bufs[2], nelem / 4, type<uint>)
        var sc <- buf_download(bufs[3], nblk, type<float>)
        var q_ref : array<uint>
        var s_ref : array<float>
        actrq_oracle(gh, uh, nelem, q8k, ACT_SILU, q_ref, s_ref)
        let blk = nelem / nblk
        check_rows(t, "{tag}'s quants vs the CPU oracle, a byte within one step (a row a block)", qbyte_values(q), qbyte_values(q_ref), QBYTE_BAR, blk)
        check_rows(t, "{tag}'s scales vs the CPU oracle (a row a block)", sc, s_ref, BAR_APPROX_CTRL, 1)
        cuda_mem_free(bufs)
        delete gh
        delete uh
        delete q
        delete sc
        delete q_ref
        delete s_ref
    }
}

//! the fused residual step + requant, the Q8_K stamp (`q8k`) or the Q8_0 one
def private cud_feed_ar(t : T?; var s : void?; sent : float; q8k : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let dim = 1024
        let eps = 1e-5
        let ascale = 1.25
        var x <- row_fill(dim, 37, 0, 0.09, 4.2, 101)
        var a <- row_fill(dim, 13, 0, 0.06, 2.6, 89)
        var w <- row_fill(dim, 1, 0, 0.04, -0.5, 17)
        let nblk = q8k ? dim / 256 : dim / 32
        var bufs = fixed_array(buf_upload(x, s), buf_upload(a, s), buf_upload(w, s), buf_fill(dim / 4, sent, s), buf_fill(nblk, NAN_SENTINEL, s))
        var pc = GkAddRmsArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale)
        if (q8k) {
            enc_cls_ar_rqk(s, bufs, pc, 1l)
        } else {
            enc_cls_ar_rq_b(s, bufs, pc, 1l)
        }
        let tag = q8k ? "cls_ar_rqk" : "cls_ar_rq_b"
        t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
        var q <- buf_download(bufs[3], dim / 4, type<uint>)
        var sc <- buf_download(bufs[4], nblk, type<float>)
        var xg <- buf_download(bufs[0], dim, type<float>)
        var x_ref, y_ref : array<float>
        x_ref |> resize(dim)
        y_ref |> resize(dim)
        for (i in range(dim)) {
            x_ref[i] = (x[i] + a[i]) * ascale
        }
        rmsnorm(y_ref, x_ref, w, int64(dim), eps)
        var q_ref : array<uint>
        var s_ref : array<float>
        requant_oracle(y_ref, 0, dim, q8k, q_ref, s_ref)
        check_rows(t, "{tag}'s quants vs the CPU oracle, a byte within one step (a row a block)", qbyte_values(q), qbyte_values(q_ref), QBYTE_BAR, dim / nblk)
        check_rows(t, "{tag}'s scales vs the CPU oracle (a row a block)", sc, s_ref, BAR_APPROX_CTRL, 1)
        check_rows(t, "{tag}'s residual row, updated in place, vs the CPU oracle", xg, x_ref, BAR_APPROX_CTRL, FLAT_ROWS)
        t |> success(moved_off(xg, x, 0, BAR_APPROX), "{tag}: the residual row moved off its input")
        cuda_mem_free(bufs)
        delete x
        delete a
        delete w
        delete q
        delete sc
        delete xg
        delete x_ref
        delete y_ref
        delete q_ref
        delete s_ref
    }
}

//! the FP4 feed planes of `rows` rows of `k` read back off the device against the CPU feed: the nibble words and code words off
//! by count, the row scales held to the approx bar
def private fp4_feed_compare(t : T?; tag : string; bufs : uint64[5]; qi, si, ri : int; ref : Fp4Feed; rows, k : int; allowed : int) {
    var q <- buf_download(bufs[qi], rows * k / 8, type<uint>)
    var sw <- buf_download(bufs[si], rows * k / 64, type<uint>)
    var rs <- buf_download(bufs[ri], rows, type<float>)
    var off = 0
    for (i in range(rows * k / 8)) {
        off += q[i] != ref.xq[i] ? 1 : 0
    }
    for (i in range(rows * k / 64)) {
        off += sw[i] != ref.xsw[i] ? 1 : 0
    }
    t |> success(off <= allowed, "{tag}: {off} nibble / code words off the CPU feed (at most {allowed})")
    check_rows(t, "{tag}: the row scales vs the CPU feed", rs, ref.xrs, BAR_APPROX_CTRL, 1)
    delete q
    delete sw
    delete rs
}

//! the FP4 feed kernels: a float plane's rows (bit for bit the CPU feed - the kernel is `precise`), the gate / up pair through the
//! activation (the act's exponential is fast-math: a few strips may round their code or a nibble the other way), and the fused
//! residual step + norm + feed against the CPU norm's feed
[test]
def test_cud_fp4_feeds(t0 : T?) {
    t0 |> run("CUDA FP4 feed kernels (fp4_rows, fp4_actrows, cls_ar_fp4_b) == the CPU feed") <| @(t : T?) {
        if (!ensure_fp4_rows() || !ensure_fp4_actrows() || !ensure_cls_ar_fp4_b()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        var s = g_cud.stream
        let rows = 6
        let k = 2048
        var inscope src <- row_fill(rows * k, 37, 0, 0.09, 4.2, 101)
        var inscope ref <- fp4_feed_rows(src, rows, k)
        var bufs = fixed_array(buf_upload(src, s), 0ul, buf_fill(rows * k / 8, 0u, s), buf_fill(rows * k / 64, 0u, s), buf_fill(rows, NAN_SENTINEL, s))
        var pc = Fp4RowsArgs(k = uint(k), inbase = 0u)
        enc_fp4_rows(s, fixed_array(bufs[0], bufs[2], bufs[3], bufs[4]), pc, int64(rows))
        t |> success(cud_sync(), "fp4_rows: the launch completes ({cuda_last_error()})")
        fp4_feed_compare(t, "fp4_rows", bufs, 2, 3, 4, ref, rows, k, 0)
        cuda_mem_free(bufs[0])
        var inscope gh <- row_fill(rows * k, 29, 0, 0.21, 6.0, 61)
        var inscope uh <- row_fill(rows * k, 11, 0, 0.17, 4.0, 47)
        var inscope act <- [for (i in range(rows * k)); gk_act_mul(gh[i], uh[i], ACT_SILU)]
        var inscope aref <- fp4_feed_rows(act, rows, k)
        bufs[0] = buf_upload(gh, s)
        bufs[1] = buf_upload(uh, s)
        var pca = Fp4RowsArgs(k = uint(k), inbase = 0u, act = ACT_SILU)
        enc_fp4_actrows(s, fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4]), pca, int64(rows))
        t |> success(cud_sync(), "fp4_actrows: the launch completes ({cuda_last_error()})")
        fp4_feed_compare(t, "fp4_actrows", bufs, 2, 3, 4, aref, rows, k, rows * k / 800)
        cuda_mem_free(bufs[0])
        cuda_mem_free(bufs[1])
        let eps = 1e-5
        let ascale = 1.25
        var inscope x <- row_fill(rows * k, 37, 0, 0.09, 4.2, 101)
        var inscope a <- row_fill(rows * k, 13, 0, 0.06, 2.6, 89)
        var inscope w <- row_fill(k, 1, 0, 0.04, -0.5, 17)
        var inscope y_ref : array<float>
        y_ref |> resize(rows * k)
        for (r in range(rows)) {
            var inscope xr <- [for (i in range(k)); (x[r * k + i] + a[r * k + i]) * ascale]
            var inscope yr : array<float>
            yr |> resize(k)
            rmsnorm(yr, xr, w, int64(k), eps)
            for (i in range(k)) {
                y_ref[r * k + i] = yr[i]
            }
        }
        var inscope nref <- fp4_feed_rows(y_ref, rows, k)
        bufs[0] = buf_upload(x, s)
        bufs[1] = buf_upload(a, s)
        let wb = buf_upload(w, s)
        var pcn = GkAddRmsArgs(dim = uint(k), add_on = 1u, eps = eps, ascale = ascale)
        enc_cls_ar_fp4_b(s, fixed_array(bufs[0], bufs[1], wb, bufs[2], bufs[3], bufs[4]), pcn, int64(rows))
        t |> success(cud_sync(), "cls_ar_fp4_b: the launch completes ({cuda_last_error()})")
        fp4_feed_compare(t, "cls_ar_fp4_b", bufs, 2, 3, 4, nref, rows, k, rows * k / 800)
        cuda_mem_free(wb)
        cuda_mem_free(bufs)
    }
}

//! the Q8_K feed kernels of a K-quant layer - the plain requant of f32 rows, the act + requant into a K-quant down plane's feed,
//! the fused residual step + requant - against the Vulkan cells' CPU oracles, the Q8_0 stamps beside them
[test]
def test_cud_q8k_feeds(t0 : T?) {
    t0 |> run("CUDA Q8_K feed kernels (cls_q8k_rq, q8k_actrq_cls, cls_ar_rqk) == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_q8k_rq() || !ensure_q8k_actrq_cls() || !ensure_cls_ar_rqk() || !ensure_cls_ar_rq_b() || !ensure_q8_actrq_cls()) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            var s = g_cud.stream
            let sent = uint_bits_to_float(I8_SENTINEL_BITS)
            cud_feed_rq(t, s, sent)
            for (q8k in fixed_array(true, false)) {
                cud_feed_actrq(t, s, sent, q8k)
                cud_feed_ar(t, s, sent, q8k)
            }
        } else {
            t |> skip("dasVulkan not present: the oracles live with the Vulkan cells")
        }
    }
}

[test]
def test_cud_words_copy(t0 : T?) {
    t0 |> run("CUDA words copy: a mapped host block in, the device, a mapped host block out") <| @(t : T?) {
        if (!ensure_words_copy()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        // 1000 words ride the quad lane alone; 1003 leave a three-word tail for the word lane; four words of slack past n
        // hold the poison either way
        for (n in [1000, 1003]) {
            let slack = n + 4
            let inb = cuda_mem_alloc_host_mapped(uint64(slack * 4))
            let outb = cuda_mem_alloc_host_mapped(uint64(slack * 4))
            t |> success(inb.host != null && outb.host != null && inb.dev != 0ul && outb.dev != 0ul, "n {n}: the mapped blocks allocate ({cuda_last_error()})")
            var inscope words <- hashed_words(slack, 3u, 11u)
            unsafe {
                memcpy(inb.host, addr<void?>(words[0]), int64(slack * 4))
                var o = reinterpret<uint?>(outb.host)
                for (i in range(slack)) {
                    o[i] = 0xDEADBEEFu
                }
            }
            let dev = buf_fill(slack, 0xDEADBEEFu, g_cud.stream)
            var pc = WordsArgs(n = uint(n))
            enc_words_copy(g_cud.stream, fixed_array(inb.dev, dev), pc, words_copy_wgs(int64(n)))
            enc_words_copy(g_cud.stream, fixed_array(dev, outb.dev), pc, words_copy_wgs(int64(n)))
            t |> success(cud_sync(), "n {n}: the launches complete ({cuda_last_error()})")
            var inscope got : array<uint>
            got |> resize(n)
            var past = 0
            unsafe {
                let o = reinterpret<uint?>(outb.host)
                for (i in range(n)) {
                    got[i] = o[i]
                }
                for (i in range(n, slack)) {
                    past += o[i] == 0xDEADBEEFu ? 0 : 1
                }
            }
            var inscope want <- copy_span(words, 0, n)
            check_exact(t, "n {n}: the words back through the device", got, want)
            t |> equal(past, 0, "n {n}: the words past n keep their poison ({past} of 4 written)")
            cuda_mem_free(dev)
            cuda_mem_free_host(inb.host)
            cuda_mem_free_host(outb.host)
        }
    }
}

//! the Vulkan cell's fixture: a partial 64-tile of rows at a nonzero window base, the f16 mirror's slack rows poisoned
def fa_h64_run(t : T?; window : int) {
    fa_run(t, 64, window)
}

//! the flash tile at head `hs` (64 or 128), f32 and f16 out, against the CPU attention oracle
def fa_run(t : T?; hs, window : int) {
    let nheads = 4
    let kv_mul = 2
    let rows = 77
    let w0 = 64
    let scale = 0.125
    let qd = nheads * hs
    let kvd = (nheads / kv_mul) * hs
    let kpos = w0 + rows
    let kpad = kpos + int(RDEC_MIR_SLACK_ROWS)
    var qh <- row_fill(rows * qd, 41, 0, 0.05, 1.8, 73)
    var kh <- row_fill(kpos * kvd, 17, 0, 0.03, 1.4, 97)
    var vh <- row_fill(kpos * kvd, 53, 0, 0.04, 1.7, 89)
    var kh16, vh16 : array<float16>
    kh16 |> resize(kpad * kvd)
    vh16 |> resize(kpad * kvd)
    for (i in range(kpad * kvd)) {   // the slack rows past the keys carry a large finite value: a probability not exactly zero would drag it in
        kh16[i] = float16(i < kpos * kvd ? kh[i] : 10000.0)
        vh16[i] = float16(i < kpos * kvd ? vh[i] : 10000.0)
    }
    let s = g_cud.stream
    var bufs = fixed_array(buf_upload(qh, s), buf_upload(kh16, s), buf_upload(vh16, s), buf_fill(rows * qd, Y_SENTINEL, s))
    let o16 = buf_fill(rows * qd, HALF_NAN, s)
    var pc = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kvd), kv_mul = uint(kv_mul), kvbase = 0u, scale = scale,
        qstride = uint(qd), qhs = uint(hs), window = uint(window), cap = 0.0, sinkoff = 0u, row0 = 0u, ring = 0u)
    let wgs = int64(nheads * ((rows + 63) / 64))
    var bufs16 = fixed_array(bufs[0], bufs[1], bufs[2], o16)
    if (hs == 64) {
        enc_fa_cm2_h64_cls(g_cud.stream, bufs, pc, wgs)
        enc_fa_cm2_h64_f16_cls(g_cud.stream, bufs16, pc, wgs)
    } elif (hs == 96) {
        enc_fa_cm2_h96_cls(g_cud.stream, bufs, pc, wgs)
        enc_fa_cm2_h96_f16_cls(g_cud.stream, bufs16, pc, wgs)
    } else {
        enc_fa_cm2_h128_cls(g_cud.stream, bufs, pc, wgs)
        enc_fa_cm2_h128_f16_cls(g_cud.stream, bufs16, pc, wgs)
    }
    t |> success(cud_sync(), "fa h{hs} window {window}: the launches complete ({cuda_last_error()})")
    var y <- buf_download(bufs[3], rows * qd, type<float>)
    var y16 <- buf_download(o16, rows * qd, type<float16>)
    cuda_mem_free(bufs)
    cuda_mem_free(o16)
    var y_ref <- fa_h64_oracle(qh, kh, vh, rows, w0, nheads, hs, kv_mul, kvd, scale, window)
    check_rows(t, "fa h{hs} window {window} vs the CPU attention oracle", y, y_ref, TILE_BAR, qd, hs)
    // the f16 twin's store rounds at 2^-11 = 4.9e-4 of the element, under the 2e-3 of the plane's largest the f32 tile's reassociation already takes
    var y16f <- [for (h in y16); float(h)]
    check_rows(t, "fa h{hs} window {window}: the f16-out twin vs the CPU attention oracle", y16f, y_ref, TILE_BAR, qd, hs)
    delete y16f
    delete y
    delete y16
    delete y_ref
    delete qh
    delete kh
    delete vh
    delete kh16
    delete vh16
}

//! the attention oracle on the kernel's f16 grid: q pre-scaled then rounded, K/V rounded through the mirror, softmax in f32
def fa_h64_oracle(qh, kh, vh : array<float>; rows, w0, nheads, hs, kv_mul, kvd : int; scale : float; window : int) : array<float> {
    let qd = nheads * hs
    let kpos = w0 + rows
    var qh2, kh2, vh2 : array<float>
    qh2 |> resize(rows * qd)
    kh2 |> resize(kpos * kvd)
    vh2 |> resize(kpos * kvd)
    for (i in range(rows * qd)) {
        qh2[i] = float(float16(qh[i] * scale))
    }
    for (i in range(kpos * kvd)) {
        kh2[i] = float(float16(kh[i]))
        vh2[i] = float(float16(vh[i]))
    }
    var y_ref : array<float>
    y_ref |> resize(rows * qd)
    for (r in range(rows)) {
        for (hh in range(nheads)) {
            let cnt = w0 + r + 1
            attn_row_oracle(qh2, r * qd + hh * hs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul, cnt, 1.0, y_ref, r * qd + hh * hs,
                window > 0 && cnt > window ? cnt - window : 0)
        }
    }
    delete qh2
    delete kh2
    delete vh2
    return <- y_ref
}

[test]
def test_cud_fa_h64(t0 : T?) {
    t0 |> run("CUDA cm2 flash tile at head 64, f32 and f16 out == the CPU attention oracle, whole and under a 40-key window") <| @(t : T?) {
        if (!ensure_fa_cm2_h64_cls() || !ensure_fa_cm2_h64_f16_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        fa_h64_run(t, 0)
        fa_h64_run(t, 40)
    }
}

//! one q8_0 store arm: the token stamp (rows at their token blocks' positions, two regions) or the window stamp (rows from
//! `pos0`), the mirror bytes against the CPU cache's own codec, every byte outside the stored rows left at its fill
def private cud_q8_store_arm(t : T?; window : bool) {
    var st <- ks_staged()
    let wgs = kv_store_wgs(int64(KS_ROW_BLOCKS * KS_ROWS * 32))
    let pos0 = 4
    var toks : array<TokMeta>
    toks |> resize(KS_ROWS)
    toks[0].pos = 5u
    toks[1].pos = 2u
    toks[1].mirbase = uint(KS_REGION_BLOCKS)
    toks[2].pos = 9u
    var blocks <- [for (r in range(KS_ROWS)); ODD_LAYERBASE + (window ? (pos0 + r) * KS_ROW_BLOCKS : int(toks[r].mirbase) + int(toks[r].pos) * KS_ROW_BLOCKS)]
    let tag = window ? "kv_q8_store_b rows from pos0" : "kv_q8_store rows at their token positions"
    let mir_bytes = mirror_bytes(KVDtype.q8_0, 2 * KS_REGION_BLOCKS)
    var fill <- [for (_i in range(mir_bytes / 4)); MIRROR_FILL]
    var s = g_cud.stream
    let bufs = fixed_array(buf_upload(st.k, s), buf_upload(st.v, s), buf_upload(fill, s), buf_upload(fill, s), buf_upload(toks, s))
    var pa = KvQStoreArgs(kvd = uint(KS_KVD), rows = uint(KS_ROWS), layerbase = uint(ODD_LAYERBASE), pos0 = uint(pos0))
    if (window) {
        enc_kv_q8_store_b_cls(s, fixed_array(bufs[0], bufs[1], bufs[2], bufs[3]), pa, wgs)
    } else {
        enc_kv_q8_store_cls(s, bufs, pa, wgs)
    }
    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
    var want_k <- codec_want(KVDtype.q8_0, st.k, blocks, mir_bytes, KS_KVD)
    var want_v <- codec_want(KVDtype.q8_0, st.v, blocks, mir_bytes, KS_KVD)
    t |> equal(clamp_inf_scales(want_k, KVDtype.q8_0), 1, "{tag}: the K fixture's one block past the half range reaches the scale clamp")
    t |> equal(clamp_inf_scales(want_v, KVDtype.q8_0), 1, "{tag}: the V fixture's one block past the half range reaches the scale clamp")
    var got_k <- buf_download(bufs[2], mir_bytes, type<uint8>)
    var got_v <- buf_download(bufs[3], mir_bytes, type<uint8>)
    check_exact(t, "{tag}, K vs the CPU cache's bytes (a scale past the half range clamped)", got_k, want_k)
    check_exact(t, "{tag}, V vs the CPU cache's bytes (a scale past the half range clamped)", got_v, want_v)
    t |> equal(int(int8(got_k[blocks[0] * block_bytes(KVDtype.q8_0) + 2 + 5])), -127, "{tag}: the block's negative maximum quantizes to -127")
    cuda_mem_free(bufs)
    delete want_k
    delete want_v
    delete got_k
    delete got_v
    delete fill
    delete toks
    delete blocks
    delete st
}

[test]
def test_cud_q8_kv_store(t0 : T?) {
    t0 |> run("CUDA q8_0 K/V store == the CPU cache's bytes, at token rows and at window rows; a block scale past the half range clamped") <| @(t : T?) {
        if (!ensure_kv_q8_store_cls() || !ensure_kv_q8_store_b_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        cud_q8_store_arm(t, false)
        cud_q8_store_arm(t, true)
    }
}

//! one shadow arm: `SH_MIRROR_POS` positions of `kvd` in the mirror from block `ODD_LAYERBASE`, the pass over positions
//! [SH_POS0, SH_POS0 + SH_ROWS) against the CPU dequant rounded to half and clamped, the rows outside the pass left at their NaN fill
def private cud_shadow_arm(t : T?; kv : KVDtype; kvd : int) {
    let tq4 = kv == KVDtype.tq4
    let tag = tq4 ? "kv_tq4_shadow" : "kv_q8_shadow"
    var kf <- row_fill(SH_MIRROR_POS * kvd, 37, 11, 0.071, 3.3)
    var vf <- row_fill(SH_MIRROR_POS * kvd, 29, 5, 0.043, 2.0, 89)
    for (i in range(32)) {
        kf[SH_POS0 * kvd + i] *= PAST_HALF_VALUES
    }
    var km <- codec_mirror_of_stored(kv, kf, SH_MIRROR_POS, kvd, ODD_LAYERBASE)
    var vm <- codec_mirror_of_stored(kv, vf, SH_MIRROR_POS, kvd, ODD_LAYERBASE)
    let halves = SH_MIRROR_POS * kvd
    var nanfill <- [for (_i in range(halves / 2)); HALF_NAN_PAIR]
    var s = g_cud.stream
    let bufs = fixed_array(buf_upload(km.bytes, s), buf_upload(vm.bytes, s), buf_upload(nanfill, s), buf_upload(nanfill, s))
    var pa = KvQShadowArgs(kvd = uint(kvd), pos0 = uint(SH_POS0), rows = uint(SH_ROWS), layerbase = uint(ODD_LAYERBASE))
    let wgs = int64((kvd / 32 * SH_ROWS + 255) / 256)
    if (tq4) {
        enc_kv_tq4_shadow_cls(s, bufs, pa, wgs)
    } else {
        enc_kv_q8_shadow_cls(s, bufs, pa, wgs)
    }
    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
    var got_k <- buf_download(bufs[2], halves, type<uint16>)
    var got_v <- buf_download(bufs[3], halves, type<uint16>)
    var want_k, want_v : array<uint16>
    want_k |> resize(halves)
    want_v |> resize(halves)
    want_k |> fill(HALF_NAN)
    want_v |> fill(HALF_NAN)
    var clamped = 0
    for (i in range(SH_POS0 * kvd, (SH_POS0 + SH_ROWS) * kvd)) {
        want_k[i] = uint16(f32_to_f16(clamp(km.deq[i], -65504.0, 65504.0)))
        want_v[i] = uint16(f32_to_f16(clamp(vm.deq[i], -65504.0, 65504.0)))
        clamped += (uint(want_k[i]) & 0x7FFFu) == uint(HALF_MAX) ? 1 : 0
    }
    t |> success(clamped > 0, "{tag}: the fixture's block past the half range reaches the clamp ({clamped} halves at 65504)")
    check_exact(t, "{tag} K rows vs the CPU dequant as halves, clamped to the half range", got_k, want_k)
    check_exact(t, "{tag} V rows vs the CPU dequant as halves", got_v, want_v)
    cuda_mem_free(bufs)
    delete got_k
    delete got_v
    delete want_k
    delete want_v
    delete nanfill
    delete kf
    delete vf
    delete km
    delete vm
}

[test]
def test_cud_kv_shadow(t0 : T?) {
    t0 |> run("CUDA block-codec mirror's f16 shadow == the CPU dequant rounded to half and clamped, bit for bit, the rows outside the pass left alone") <| @(t : T?) {
        if (!ensure_kv_q8_shadow_cls() || !ensure_kv_tq4_shadow_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        cud_shadow_arm(t, KVDtype.q8_0, 96)
        cud_shadow_arm(t, KVDtype.tq4, 128)
    }
}

//! the oracle reads the rows back through the CPU codec; the control is the attention over the rows before their quantization
def private cud_q8_attn_arm(t : T?; nheads, kv_mul : int) {
    let hs = 64
    let kvd = nheads / kv_mul * hs
    let qd = nheads * hs
    let keys = 300   // past the 256-key chunk, so the running max rescales
    let base = ODD_LAYERBASE
    let g2 = da_slab_is_g2(int64(kv_mul))
    let tag = "da_attn_q8{g2 ? "_g2" : ""} {nheads} heads over {nheads / kv_mul}, {keys} keys"
    var q <- row_fill(qd, 41, 3, 0.05, 1.8, 73)
    for (i in range(qd)) {   // an irregular factor an element: the query's q8_0 quant off a regular ramp is a coin flip between two f32 orders
        q[i] *= 1.0 + 0.011 * float((i * 7919) % 97) / 97.0
    }
    var kf <- row_fill(keys * kvd, 17, 5, 0.03, 1.4)
    var vf <- row_fill(keys * kvd, 53, 7, 0.04, 1.7, 89)
    var km <- codec_mirror_of_stored(KVDtype.q8_0, kf, keys, kvd, base)
    var vm <- codec_mirror_of_stored(KVDtype.q8_0, vf, keys, kvd, base)
    var qrt <- q8_query_roundtrip(q)
    let scale = 1.0 / sqrt(float(hs))
    var y_ref, y_ctrl : array<float>
    y_ref |> resize(qd)
    y_ctrl |> resize(qd)
    for (hh in range(nheads)) {
        attn_row_oracle(qrt, hh * hs, km.deq, 0, vm.deq, 0, kvd, hh, hs, kv_mul, keys, scale, y_ref, hh * hs)
        attn_row_oracle(qrt, hh * hs, kf, 0, vf, 0, kvd, hh, hs, kv_mul, keys, scale, y_ctrl, hh * hs)
    }
    var toks : array<TokMeta>
    toks |> resize(1)
    toks[0].pos = uint(keys - 1)
    toks[0].cnt = uint(keys)
    let rowwg = da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l)
    var pa = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(base), vbase = uint(base), scale = scale,
        qstride = uint(hs), gated = 0u, window = 0u, cap = 0.0, nsplit = 1u, rqk = DA_RQ_NONE, sinkoff = 0u, rowwg = uint(rowwg),
        qrow = uint(qd), cntoff = 0u, ring = 0u)
    var s = g_cud.stream
    let bufs = fixed_array(buf_upload(q, s), buf_upload(km.bytes, s), buf_upload(vm.bytes, s), buf_fill(qd, Y_SENTINEL, s), buf_upload(toks, s),
        buf_fill(64, 0.0, s), buf_fill(64, 0.0, s), buf_fill(64, 0.0, s))
    if (g2) {
        enc_da_attn_q8_g2_cls(s, bufs, pa, rowwg)
    } else {
        enc_da_attn_q8_cls(s, bufs, pa, rowwg)
    }
    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
    var y <- buf_download(bufs[3], qd, type<float>)
    let bar = Bar(rel = 1e-4, poison_of_max = true, poison_abs = 0.5)
    check_rows(t, "{tag} vs the CPU attention over the dequantized rows", y, y_ref, bar, qd)
    check_exceeds(t, "{tag} control: the attention over the rows before their quantization", y, y_ctrl, bar)
    cuda_mem_free(bufs)
    delete y
    delete y_ref
    delete y_ctrl
    delete toks
    delete qrt
    delete km
    delete vm
    delete kf
    delete vf
    delete q
}

[test]
def test_cud_q8_da_attn(t0 : T?) {
    t0 |> run("CUDA decode attention's q8_0 stamps == the CPU attention over the dequantized mirror rows, the four-head and the two-head slab") <| @(t : T?) {
        if (!ensure_da_attn_q8_cls() || !ensure_da_attn_q8_g2_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        cud_q8_attn_arm(t, 8, 4)
        cud_q8_attn_arm(t, 4, 2)
    }
}

let private TQ_ROWS = 2
let private TQ_POS_PER_REGION = 9

//! K's first head of row 1 and V's last sit past the half range
def private cud_tq4_store_arm(t : T?; hs, nqh, nkh : int; window : bool = false) {
    let qd = max(nqh, 1) * hs   // with no q head a dispatch still binds q, with no K head the stages
    let kvd = max(nkh, 1) * hs
    let nblk = kvd / 32
    let pos0 = 3
    let tag = "kv_tq4_store{window ? "_b" : ""} hs {hs}, {nqh} q heads, {nkh} k/v heads"
    var signs <- tq4_sign_row()
    var q <- row_fill(TQ_ROWS * qd, 41, 3, 0.05, 1.8, 73)
    var kf <- row_fill(TQ_ROWS * kvd, 37, 11, 0.071, 3.3)
    var vf <- row_fill(TQ_ROWS * kvd, 29, 5, 0.043, 2.0, 89)
    for (i in range(32)) {
        vf[32 + i] = 0.0   // an all-zero block: the quantizer's zero-scale edge
    }
    for (i in range(hs)) {
        kf[kvd + i] *= PAST_HALF_RANGE
        vf[2 * kvd - hs + i] *= PAST_HALF_RANGE
    }
    var toks : array<TokMeta>
    toks |> resize(TQ_ROWS)
    toks[0].pos = 4u
    toks[1].pos = 2u
    toks[1].mirbase = uint(TQ_POS_PER_REGION * nblk)
    let mir_bytes = mirror_bytes(KVDtype.tq4, 2 * TQ_POS_PER_REGION * nblk)
    var blocks <- [for (r in range(TQ_ROWS)); ODD_LAYERBASE + (window ? (pos0 + r) * nblk : int(toks[r].mirbase) + int(toks[r].pos) * nblk)]
    var want_q <- nqh > 0 ? tq4_rotated(q, TQ_ROWS, qd, hs, signs) : clone_to_move(q)
    var kr <- tq4_rotated(kf, TQ_ROWS, kvd, hs, signs)
    var vr <- tq4_rotated(vf, TQ_ROWS, kvd, hs, signs)
    var stored <- [for (b in blocks); b; where nkh > 0]
    var want_k <- codec_want(KVDtype.tq4, kr, stored, mir_bytes, kvd)
    var want_v <- codec_want(KVDtype.tq4, vr, stored, mir_bytes, kvd)
    t |> success(nkh == 0 || clamp_inf_scales(want_k, KVDtype.tq4) > 0, "{tag}: the K fixture's head past the half range reaches the scale clamp")
    t |> success(nkh == 0 || clamp_inf_scales(want_v, KVDtype.tq4) > 0, "{tag}: the V fixture's head past the half range reaches the scale clamp")
    var fill <- [for (_i in range(mir_bytes / 4)); MIRROR_FILL]
    var s = g_cud.stream
    let bufs = fixed_array(buf_upload(q, s), buf_upload(kf, s), buf_upload(vf, s), buf_upload(fill, s), buf_upload(fill, s), buf_upload(signs, s), buf_upload(toks, s))
    var pa = KvTqStoreArgs(kvd = uint(kvd), hs = uint(hs), layerbase = uint(ODD_LAYERBASE), pos0 = uint(pos0),
        nqh = uint(nqh), nkh = uint(nkh), qrow = uint(qd), qstride = uint(hs), inv_sqrt_hs = 1.0 / sqrt(float(hs)))
    if (window) {
        enc_kv_tq4_store_b_cls(s, fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5]), pa, int64(TQ_ROWS * (nqh + 2 * nkh)))
    } else {
        enc_kv_tq4_store_cls(s, bufs, pa, int64(TQ_ROWS * (nqh + 2 * nkh)))
    }
    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
    var got_q <- buf_download(bufs[0], TQ_ROWS * qd, type<float>)
    var got_k <- buf_download(bufs[3], mir_bytes, type<uint8>)
    var got_v <- buf_download(bufs[4], mir_bytes, type<uint8>)
    check_rows(t, "{tag}: the q rows{nqh > 0 ? " rotated in place vs the CPU rotation" : " left alone"}", got_q, want_q, BAR_EXACT, qd)
    if (nqh > 0) {
        check_exceeds(t, "{tag} control: the rotation moves the q rows off their input", got_q, q, BAR_EXACT)
    }
    check_exact(t, "{tag}: the K rows vs the CPU rotation and cache bytes (a scale past the half range clamped)", got_k, want_k)
    check_exact(t, "{tag}: the V rows vs the CPU rotation and cache bytes (a scale past the half range clamped)", got_v, want_v)
    cuda_mem_free(bufs)
    delete got_q
    delete got_k
    delete got_v
    delete fill
    delete stored
    delete signs
    delete q
    delete kf
    delete vf
    delete toks
    delete blocks
    delete want_q
    delete kr
    delete vr
    delete want_k
    delete want_v
}

[test]
def test_cud_tq4_kv_store(t0 : T?) {
    t0 |> run("CUDA tq4 K/V store == the CPU rotation and the CPU cache's bytes, the q rows rotated in place, at token rows and at window rows") <| @(t : T?) {
        if (!ensure_kv_tq4_store_cls() || !ensure_kv_tq4_store_b_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        cud_tq4_store_arm(t, 64, 4, 2)
        cud_tq4_store_arm(t, 128, 2, 1)
        cud_tq4_store_arm(t, 512, 1, 1)   // the wide head: a thread's second element
        cud_tq4_store_arm(t, 32, 3, 3)    // one block a head
        cud_tq4_store_arm(t, 64, 4, 0)    // a shared-KV layer: q rotates, nothing stores
        cud_tq4_store_arm(t, 64, 4, 2, true)
        cud_tq4_store_arm(t, 64, 0, 2, true)   // the NextN head's prompt warm: no q head to rotate
    }
}

let private UR_ROWS = 3

//! one un-rotation arm: `nh` heads `hs` wide over `UR_ROWS` rows, gated by the q plane's gate halves when `gated`, against the CPU un-rotation
def private cud_unrot_arm(t : T?; hs, nh : int; gated : bool) {
    let tag = "kv_tq4_unrot hs {hs}, {nh} heads{gated ? ", gated" : ""}"
    let orow = nh * hs
    let qstride = gated ? 2 * hs : hs
    let qrow = nh * qstride
    var signs <- tq4_sign_row()
    var y <- row_fill(UR_ROWS * orow, 31, 7, 0.06, 2.4, 83)
    var q <- row_fill(UR_ROWS * qrow, 43, 5, 0.05, 2.0, 79)
    var want := y
    tq4_unrotate_batch(want, int64(UR_ROWS), int64(orow), int64(hs), unsafe(addr<float const?>(signs[0])))
    var ungated := want
    if (gated) {
        for (r in range(UR_ROWS)) {
            for (hh in range(nh)) {
                for (i in range(hs)) {
                    want[r * orow + hh * hs + i] /= 1.0 + exp(-q[r * qrow + hh * qstride + hs + i])
                }
            }
        }
    }
    var s = g_cud.stream
    let bufs = fixed_array(buf_upload(y, s), buf_upload(q, s), buf_upload(signs, s))
    var pa = KvTqUnrotArgs(hs = uint(hs), nh = uint(nh), orow = uint(orow), qrow = uint(qrow), qstride = uint(qstride), inv_sqrt_hs = 1.0 / sqrt(float(hs)))
    enc_kv_tq4_unrot_cls(s, bufs, pa, int64(UR_ROWS * nh))
    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
    var got <- buf_download(bufs[0], UR_ROWS * orow, type<float>)
    check_rows(t, "{tag} vs the CPU un-rotation{gated ? " under the gate" : ""}", got, want, gated ? BAR_APPROX : BAR_EXACT, orow)
    check_exceeds(t, "{tag} control: the un-rotation moves the rows off their input", got, y, BAR_APPROX)
    if (gated) {
        check_exceeds(t, "{tag} control: the rows before the gate", got, ungated, BAR_APPROX)
    }
    cuda_mem_free(bufs)
    delete got
    delete signs
    delete y
    delete q
    delete want
    delete ungated
}

[test]
def test_cud_tq4_unrot(t0 : T?) {
    t0 |> run("CUDA tq4 un-rotation == the CPU un-rotation, bit for bit, and under the head gate") <| @(t : T?) {
        if (!ensure_kv_tq4_unrot_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        cud_unrot_arm(t, 64, 4, false)
        cud_unrot_arm(t, 512, 1, false)
        cud_unrot_arm(t, 128, 2, true)
        cud_unrot_arm(t, 32, 3, false)
    }
}

//! everything in the rotated basis; the control is the attention under the unrotated query
def private cud_tq4_attn_arm(t : T?; nheads, kv_mul : int) {
    let hs = 64
    let kvd = nheads / kv_mul * hs
    let qd = nheads * hs
    let keys = 300
    let base = ODD_LAYERBASE
    let g2 = da_slab_is_g2(int64(kv_mul))
    let tag = "da_attn_tq4{g2 ? "_g2" : ""} {nheads} heads over {nheads / kv_mul}, {keys} keys"
    var signs <- tq4_sign_row()
    var q0 <- row_fill(qd, 41, 3, 0.05, 1.8, 73)
    for (i in range(qd)) {
        q0[i] *= 1.0 + 0.011 * float((i * 7919) % 97) / 97.0
    }
    var q <- tq4_rotated(q0, 1, qd, hs, signs)
    var kf <- row_fill(keys * kvd, 17, 5, 0.03, 1.4)
    var vf <- row_fill(keys * kvd, 53, 7, 0.04, 1.7, 89)
    var kr <- tq4_rotated(kf, keys, kvd, hs, signs)
    var vr <- tq4_rotated(vf, keys, kvd, hs, signs)
    var km <- codec_mirror_of_stored(KVDtype.tq4, kr, keys, kvd, base)
    var vm <- codec_mirror_of_stored(KVDtype.tq4, vr, keys, kvd, base)
    var qrt <- q8_query_roundtrip(q)
    let scale = 1.0 / sqrt(float(hs))
    var y_ref, y_ctrl : array<float>
    y_ref |> resize(qd)
    y_ctrl |> resize(qd)
    for (hh in range(nheads)) {
        attn_row_oracle(qrt, hh * hs, km.deq, 0, vm.deq, 0, kvd, hh, hs, kv_mul, keys, scale, y_ref, hh * hs)
        attn_row_oracle(q0, hh * hs, km.deq, 0, vm.deq, 0, kvd, hh, hs, kv_mul, keys, scale, y_ctrl, hh * hs)
    }
    var toks : array<TokMeta>
    toks |> resize(1)
    toks[0].pos = uint(keys - 1)
    toks[0].cnt = uint(keys)
    let rowwg = da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l)
    var pa = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(base), vbase = uint(base), scale = scale,
        qstride = uint(hs), gated = 0u, window = 0u, cap = 0.0, nsplit = 1u, rqk = DA_RQ_NONE, sinkoff = 0u, rowwg = uint(rowwg),
        qrow = uint(qd), cntoff = 0u, ring = 0u)
    var s = g_cud.stream
    let bufs = fixed_array(buf_upload(q, s), buf_upload(km.bytes, s), buf_upload(vm.bytes, s), buf_fill(qd, Y_SENTINEL, s), buf_upload(toks, s),
        buf_fill(64, 0.0, s), buf_fill(64, 0.0, s), buf_fill(64, 0.0, s))
    if (g2) {
        enc_da_attn_tq4_g2_cls(s, bufs, pa, rowwg)
    } else {
        enc_da_attn_tq4_cls(s, bufs, pa, rowwg)
    }
    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
    var y <- buf_download(bufs[3], qd, type<float>)
    let bar = Bar(rel = 1e-4, poison_of_max = true, poison_abs = 0.5)
    check_rows(t, "{tag} vs the CPU attention over the dequantized rotated rows", y, y_ref, bar, qd)
    check_exceeds(t, "{tag} control: the attention under the unrotated query", y, y_ctrl, bar)
    cuda_mem_free(bufs)
    delete y
    delete y_ref
    delete y_ctrl
    delete toks
    delete qrt
    delete km
    delete vm
    delete kr
    delete vr
    delete kf
    delete vf
    delete q
    delete q0
    delete signs
}

[test]
def test_cud_tq4_da_attn(t0 : T?) {
    t0 |> run("CUDA decode attention's tq4 stamps == the CPU attention over the dequantized rotated rows, the four-head and the two-head slab") <| @(t : T?) {
        if (!ensure_da_attn_tq4_cls() || !ensure_da_attn_tq4_g2_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        cud_tq4_attn_arm(t, 8, 4)
        cud_tq4_attn_arm(t, 4, 2)
    }
}

//! the flash tile at head 128: the 3B class's attention, 32 keys a step
[test]
def test_cud_fa_h96(t0 : T?) {
    t0 |> run("CUDA cm2 flash tile at head 96, f32 and f16 out == the CPU attention oracle, whole and under a 40-key window") <| @(t : T?) {
        if (!ensure_fa_cm2_h96_cls() || !ensure_fa_cm2_h96_f16_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        fa_run(t, 96, 0)
        fa_run(t, 96, 40)
    }
}

[test]
def test_cud_fa_h128(t0 : T?) {
    t0 |> run("CUDA cm2 flash tile at head 128, f32 and f16 out == the CPU attention oracle, whole and under a 40-key window") <| @(t : T?) {
        if (!ensure_fa_cm2_h128_cls() || !ensure_fa_cm2_h128_f16_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        fa_run(t, 128, 0)
        fa_run(t, 128, 40)
    }
}

let private RATE_ROWS = fixed_array(0, 255, 256, 511)   //! the activation rows the rate cell's warm-up is held on: both edges of both 256-row tiles, every weight column
let private RATE_TAPS = 16   //! the rate fixture's nonzero activations a row: 16 x 128 steps of the 2^-7 scale stay within f16's 2048 exact integers
let private RATE_SCALE_WORD = 0x20002000u   //! two f16 block scales of 2^-7

//! The rate cell's activation plane, `rows` rows of `k` halves and `TILE_READ_SLACK` zero rows past them: `RATE_TAPS` entries of
//! +-1 a row, `k / RATE_TAPS` apart, every other element 0. Over q8 weights at the 2^-7 block scale every product and every partial
//! sum is a multiple of 2^-7 under 2048 of them, exact in the l tile's f16 accumulator in any order, so the oracle holds bit for bit.
def private rate_act_plane(rows, k : int) : array<uint> {
    let step = k / RATE_TAPS
    var words : array<uint>
    words |> resize((rows + int(TILE_READ_SLACK)) * k / 2)
    for (r in range(rows)) {
        for (j in range(RATE_TAPS)) {
            let c = j * step + r % step
            let e = r * k + c
            let half = ((j + r) & 1) == 0 ? 0x3C00u : 0xBC00u
            words[e / 2] |= half << uint((e % 2) * 16)
        }
    }
    return <- words
}

//! the warm-up's activation rows `RATE_ROWS` of the `rows` x `d` output at `y` against the f16-feed oracle bit for bit, every column:
//! every workgroup of the launch writes some of them
def private rate_rows_check(t : T?; y : uint64; wqh, wsh, xfh : array<uint>; rows, k, d : int) {
    var recs : array<uint>
    for (r in RATE_ROWS) {
        recs |> push_from([0u, uint(r), 1u, 0u])
    }
    var full_ref : array<float>
    full_ref |> resize(rows * d)
    q8f16_gemm_oracle(wqh, wsh, xfh, recs, length(RATE_ROWS), k, d, full_ref)
    var full <- buf_download(y, rows * d, type<float>)
    var got, want : array<float>
    for (r in RATE_ROWS) {
        got |> push_from(copy_span(full, r * d, d))
        want |> push_from(copy_span(full_ref, r * d, d))
    }
    check_rows(t, "the warm-up at {rows} x {k} -> {d}, activation rows {RATE_ROWS} (a row each), vs the f16-feed oracle", got, want, BAR_EXACT, d)
    delete got
    delete want
    delete full
    delete full_ref
    delete recs
}

//! the cm2 l tile at a llama-1B prefill window's shapes (512 rows by the projection and the FFN widths), event-timed
//! a mirror plane's `n` elements as floats: the halves unpacked under `f16`
def private cud_mirror_floats(buf : uint64; n : int; f16 : bool) : array<float> {
    if (f16) {
        var hk <- buf_download(buf, n, type<uint16>)
        var out <- [for (h in hk); f16_to_f32(uint(h))]
        delete hk
        return <- out
    }
    return <- buf_download(buf, n, type<float>)
}

//! the fused q/k norm, rope and K/V store on this home's stamp (`qkn_rope_cls`, or under `f16` its half-mirror twin) on one
//! row, a head a workgroup, against the oracle both homes' cells hold it to: the four q heads normed and roped in place, the
//! two k heads normed, roped and stored at mirror row `krow`, each k head's workgroup copying its raw V row
def private cud_qkn_rope_arm(t : T?; f16 : bool) {
    let nh = 4
    let nkvh = 2
    let hs = 64
    let qd = nh * hs
    let kvd = nkvh * hs
    let krow = 192
    let eps = 0.00001
    let mirn = krow + nkvh * hs
    let name = f16 ? "qkn_rope_f16" : "qkn_rope_kv"
    var q0h <- row_fill(qd, 31, 0, 0.05, 1.6, 67)
    var kvh <- row_fill(qd + 2 * kvd, 23, 0, 0.04, 1.5, 71)
    var csh <- row_fill(hs, 13, 0, 0.045, 0.9, 41)
    var wnh <- [for (i in range(2 * hs)); 0.75 + float(i % 11) * 0.05]
    let bh : array<float>
    var pc = GkQknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2), neox = 1u, nh = uint(nh),
        layerbase = uint(krow), qwoff = 0u, kwoff = uint(hs), eps = eps, qstride = uint(hs), cosrow = uint(hs))
    var s = g_cud.stream
    var nanfill <- [for (_i in range(mirn / 2)); HALF_NAN_PAIR]
    var tok : array<TokMeta>
    tok |> resize(1)   // the mirror row is layerbase alone: a zero token block
    let pq = buf_upload(q0h, s)
    let pkv = buf_upload(kvh, s)
    let pkm = f16 ? buf_upload(nanfill, s) : buf_fill(mirn, Y_SENTINEL, s)
    let pvm = f16 ? buf_upload(nanfill, s) : buf_fill(mirn, Y_SENTINEL, s)
    let pcs = buf_upload(csh, s)
    let pwn = buf_upload(wnh, s)
    let ptok = buf_upload(tok, s)
    var bufs = fixed_array(pq, pkv, pkm, pvm, pcs, pwn, ptok, pcs, pkv)
    if (f16) {
        enc_qkn_rope_f16_cls(s, bufs, pc, int64(nh + nkvh))
    } else {
        enc_qkn_rope_cls(s, bufs, pc, int64(nh + nkvh))
    }
    t |> success(cud_sync(), "{name}: the launch completes ({cuda_last_error()})")
    var q_cls <- buf_download(pq, qd, type<float>)
    var mk <- cud_mirror_floats(pkm, mirn, f16)
    var mv <- cud_mirror_floats(pvm, mirn, f16)
    cuda_mem_free(fixed_array(pq, pkv, pkm, pvm, pcs, pwn, ptok))   //! the bias and V-source bindings alias the cos and projection planes: each block freed once
    var ref <- qkn_rope_rows(q0h, kvh, bh, wnh, csh, 0, nh, nkvh, hs, krow, mirn, eps) $(x : float) => f16 ? f16_to_f32(f32_to_f16(x)) : x
    check_rows(t, "{name}, q", q_cls, ref.q, BAR_APPROX_CTRL, qd, hs)
    var mk_rows <- [for (i in range(krow, mirn)); mk[i]]
    var mv_rows <- [for (i in range(krow, mirn)); mv[i]]
    var rk_rows <- [for (i in range(krow, mirn)); ref.mk[i]]
    var rv_rows <- [for (i in range(krow, mirn)); ref.mv[i]]
    check_rows(t, "{name}, the K mirror rows", mk_rows, rk_rows, BAR_APPROX_CTRL, hs)
    check_rows(t, "{name}, the V mirror rows", mv_rows, rv_rows, BAR_APPROX_CTRL, hs)
    var untouched = 0
    for (i in range(krow)) {
        untouched += (f16 ? (is_nan(mk[i]) && is_nan(mv[i])) : (mk[i] == Y_SENTINEL && mv[i] == Y_SENTINEL)) ? 1 : 0
    }
    t |> equal(untouched, krow, "{name}: the mirror rows ahead of the layer's keep their fill")
    delete mk_rows
    delete mv_rows
    delete rk_rows
    delete rv_rows
    delete ref
    delete q_cls
    delete mk
    delete mv
    delete nanfill
    delete tok
    delete q0h
    delete kvh
    delete csh
    delete wnh
}

[test]
def test_cud_qkn_rope(t0 : T?) {
    t0 |> run("CUDA fused q/k norm + rope + K/V store == the CPU oracle, the f32 and the f16 mirror stamps") <| @(t : T?) {
        if (!ensure_qkn_rope_cls() || !ensure_qkn_rope_f16_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        cud_qkn_rope_arm(t, false)
        cud_qkn_rope_arm(t, true)
    }
}

[test]
def test_cud_q8_cm2l_rate(t0 : T?) {
    t0 |> run("CUDA q8 cm2 l-tile at pp512 shapes: a rate, logged") <| @(t : T?) {
        if (!ensure_q8_batch_cm2l_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        for (shape in [(k = 2048, d = 2048), (k = 2048, d = 8192), (k = 8192, d = 2048)]) {
            let rows = 512
            var wqh, wsh : array<uint>
            q8_plane(shape.d * (shape.k / 32), 1u, 17u, 1u, 991u, wqh, wsh)
            wsh |> fill(RATE_SCALE_WORD)
            var xfh <- rate_act_plane(rows, shape.k)
            var sr <- two_region_sched(rows, 0, 0, 256, (shape.d + 127) / 128, 0)
            let wgs = sr.wgs
            let s = g_cud.stream
            var bufs = fixed_array(buf_upload(wqh, s), buf_upload(wsh, s), buf_upload(sr.words, s), buf_upload(xfh, s), buf_fill(rows * shape.d, Y_SENTINEL, s))
            var pc = BatchArgs(n = uint(shape.k), d = uint(shape.d), map_off = uint(2 * 4))
            enc_q8_batch_cm2l_cls(g_cud.stream, bufs, pc, int64(wgs))
            t |> success(cud_sync(), "the warm-up completes ({cuda_last_error()})")
            rate_rows_check(t, bufs[4], wqh, wsh, xfh, rows, shape.k, shape.d)
            let reps = 20
            let ms = cud_time_ms() {
                for (_i in range(reps)) {
                    enc_q8_batch_cm2l_cls(g_cud.stream, bufs, pc, int64(wgs))
                }
            }
            let per = ms / float(reps)
            let flops = 2.0lf * double(rows) * double(shape.k) * double(shape.d)
            to_log(LOG_WARNING, "cuda q8 cm2 l tile {rows} x {shape.k} -> {shape.d} on {g_cud.name}: {per} ms a launch, {float(flops / double(per) / 1.0e9lf)} TFLOP/s\n")
            t |> success(per > 0.0, "the timing reads")
            cuda_mem_free(bufs)
            delete sr
            delete xfh
            delete wqh
            delete wsh
        }
    }
}

//! the compile gate: every kernel of the CUDA home compiles through NVRTC and loads - the survey's count on the home itself
[test]
def test_cud_compile_gate(t0 : T?) {
    t0 |> run("every [cuda_dispatch] kernel of the CUDA home compiles and loads") <| @(t : T?) {
        if (!cud_init()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let names <- cud_kernel_names()
        var inscope failed <- cud_ensure_all()
        to_log(LOG_INFO, "cuda home: {length(names)} kernels, {length(failed)} refused\n")
        t |> success(length(names) >= 52, "the home carries the token step's kernels ({length(names)} seeded)")
        t |> equal(length(failed), 0, "every kernel compiles and loads; refused: {failed}")
    }
}

//! the leak gate: every device buffer freed, every module released at the shutdown, nothing counted live
[test]
def test_cud_shutdown_leaks(t0 : T?) {
    t0 |> run("the CUDA tier shuts down with nothing live") <| @(t : T?) {
        cud_shutdown()
        t |> equal(cuda_log_leaks_if_any(), 0l, "no CUDA object or allocation outlives the tier")
    }
}
