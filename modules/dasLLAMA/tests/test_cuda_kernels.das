options gen2
options persistent_heap
options indenting = 4
options _dasllama_internal = true
options stack = 524288

// The CUDA kernel home's cells: the shared GEMV, GEMM, flash and codec bodies stamped under
// [cuda_kernel] / [cuda_dispatch] against the same CPU oracles the Vulkan cells hold them to.
// Model-free; every cell but the closing leak gate skips without a CUDA device and NVRTC, and the
// per-format cells (the kq cm2 tiles, the kq and per-32 GEMVs, the token-table gathers, the Q8_K
// feeds), the biased act twins, the deltanet family and the mx4 FP4 leaf over the expert schedule
// also skip without the dasVulkan module, whose cells carry their oracles - the routing cell's
// bucket-schedule arms register a skip of their own there; the mx4 FP4 leaf over the expert
// schedule also skips on a card below compute capability 120 (`FP4_RUNG_SM`). The leak gate never
// skips.

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
require daslib/jobque_boost   // the worker-thread cell: a context of its own on another thread
require dasllama/dasllama_kqformat
require dasllama/dasllama_gpu_math   // ue4m3_half_f32 - the nv4 MMQ cell's reference scales
require dasllama/dasllama_gpu_knee   // cud_split_k_tgs: the short-window pick cell pins the knee
require _fp4_feed   // the FP4 feed kernels' CPU oracle
require _mmq_ref   // the MMQ leaves' CPU references, shared with the GEMM race
require _moe_routing_oracles   // the routed block's routing kernels' fixtures and oracles, the Vulkan kernel file's too
require dasllama/dasllama_common   // MoeGate: the routing cells' select oracle takes the softmax router
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
let private MMQ_ROW_PAD = int(MMQ_SCHED_ROW_PAD)   //! the MMQ-fed chain's bucket row pad, the MMQ schedule stamp's
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

//! the cm2 l tile over two regions of activation rows against the f16-feed oracle: `k` the reduction width
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

//! the column `tc`'s tile of format `fmt` split along K through the reduce against the whole-K oracle: one region of `rows` rows
//! against a `d` x `n` plane in `nsplit` 256-aligned chunks, the last short where K does not divide; the s column takes the f16
//! operand bar (`cm_bar`), the l column the tile bar. The chunk depth does not enter either bar: the tiles accumulate in f32
//! (`coopmatWgAcc_f32_64x64`), so a chunk's sum and the reduce's sum of chunks reassociate the same f32 terms (an error of order
//! K . 2^-24 of the sum of magnitudes, under the bars' floor at every K here), and what the bars cover is the operand rounding -
//! every x and w rounded to f16 once, each product within 2^-10 of exact, the same at every depth. The partials plane carries
//! the tile's row footprint past the region (the s tile covers 32 rows at 31, the l tile 256 at 300), sentinel-filled and
//! asserted untouched: a broken row guard writes there before it writes past the buffer
def cm2_split_run(t : T?; tag : string; fmt, tc, rows, d, n, nsplit : int; wqh, wsh : array<uint>; xf_add : uint;
                  oracle : function<(wq : array<uint>; wsu : array<uint>; xf : array<uint>; recs : array<uint>; nrec, n, d : int; var y : array<float>) : void>) {
    let ck = int(cm2_chunk(int64(n), int64(nsplit)))
    t |> success(ck > 0 && ck % 256 == 0 && ck < n, "{tag}: K {n} splits {nsplit} ways in 256-aligned chunks ({ck} deep)")
    return if (ck <= 0)
    let cols = tc == CUD_CM2_TC_S ? 32 : 256
    let rows_pad = ((rows + cols - 1) / cols) * cols
    let slack = (rows_pad - rows) * d
    var xfh <- ws_plane((rows + int(TILE_READ_SLACK)) * n / 2, 3u, xf_add)
    let wgs = (rows_pad / cols) * ((d + 127) / 128) * nsplit
    var sched <- one_region_words(rows, wgs)
    var y_ref : array<float>
    y_ref |> resize(rows * d)
    invoke(oracle, wqh, wsh, xfh, sched, 1, n, d, y_ref)
    var s = g_cud.stream
    var bufs = fixed_array(buf_upload(wqh, s), buf_upload(wsh, s), buf_upload(sched, s), buf_upload(xfh, s), buf_fill(rows * d * nsplit + slack, Y_SENTINEL, s))
    let y = buf_fill(rows * d, Y_SENTINEL, s)
    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = uint(ck))
    cud_cm2_enc(fmt, tc, s, bufs, pc, int64(wgs))
    var pr = SkRedArgs(nelem = uint(rows * d), k = uint(nsplit), ybase = 0u)
    enc_splitk_reduce_cls(s, fixed_array(bufs[4], y), pr, splitk_reduce_wgs(int64(rows * d)))
    t |> success(cud_sync(), "{tag} split {nsplit} ways at k {n}: the launches complete ({cuda_last_error()})")
    var got <- buf_download(y, rows * d, type<float>)
    var inscope parts <- buf_download(bufs[4], rows * d * nsplit + slack, type<float>)
    cuda_mem_free(bufs)
    cuda_mem_free(y)
    check_rows(t, "{tag} split {nsplit} ways at k {n} + the reduce vs the whole-K oracle", got, y_ref, tc == CUD_CM2_TC_S ? cm_bar(y_ref) : TILE_BAR, d)
    if (slack > 0) {
        var inscope kept <- [for (_i in range(slack)); Y_SENTINEL]
        check_exact(t, "{tag} split {nsplit} ways: the {rows_pad - rows} rows of the tile's footprint past the region keep the partials' sentinel", copy_span(parts, rows * d * nsplit, slack), kept)
    }
    delete got
    delete y_ref
    delete sched
    delete xfh
}

//! the q8 tile of column `tc` split along K at `rows` rows: the dense window's split on the l column, the short window's on the s column
def q8_split_run(t : T?; tc, rows, k, nsplit : int) {
    var wqh, wsh : array<uint>
    q8_plane(TILE_D * (k / 32), 1u, 17u, 1u, 991u, wqh, wsh)
    cm2_split_run(t, "q8 cm2 {tc == CUD_CM2_TC_S ? "s" : "l"} tile", int(KqFmt.q8), tc, rows, TILE_D, k, nsplit, wqh, wsh, 5u, @@q8f16_gemm_oracle)
    delete wqh
    delete wsh
}

//! what a worker thread's context saw of the device, carried back without a string (a string crossing contexts is a pointer into the worker's heap)
struct WorkerBind {
    ok : bool
    rebound : bool
    free_ : uint64
    clean : bool
}

//! a worker thread's own context reaches the device: its `[init]` made the device on the creating thread, and the tier's entry
//! gate binds the calling thread at EVERY entry - a ready device's too - so after the thread's binding is dropped the next entry
//! binds again and the free-memory query (the plan's budget) answers as on the main thread; an entry that only answers "ready"
//! leaves the thread unbound and the query reads nothing
[test]
def test_cud_worker_thread_binds(t0 : T?) {
    t0 |> run("a worker thread's context binds the CUDA device on every entry: dropped from the thread, the ready device binds again and the free-memory query answers") <| @(t : T?) {
        if (!cud_init()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let main_free = cuda_device_free_memory()
        t |> success(main_free > 0ul, "the main thread reads the device's free memory ({main_free} bytes)")
        var saw = WorkerBind()
        with_channel(1) $(ch) {
            new_thread() <| @capture(= ch) {
                let ok = cud_init()
                cuCtxSetCurrent(null)   // the thread's binding dropped: a ready device's next entry must bind again
                let rebound = cud_init()
                let free_ = cuda_device_free_memory()
                ch |> push_clone(WorkerBind(ok = ok, rebound = rebound, free_ = free_, clean = empty(cuda_last_error())))
                ch |> notify_and_release
            }
            ch |> join()
            ch |> for_each_clone() $(w : WorkerBind#) {
                saw = WorkerBind(ok = w.ok, rebound = w.rebound, free_ = w.free_, clean = w.clean)
            }
        }
        t |> success(saw.ok, "the worker's context initializes the tier")
        t |> success(saw.rebound, "the ready device's entry answers on the unbound thread")
        t |> success(saw.free_ > 0ul && saw.clean, "the worker's thread reads the device's free memory after the rebind ({saw.free_} bytes, the driver's last error {saw.clean ? "clear" : "set"})")
    }
}

[test]
def test_cud_q8_cm2s_split(t0 : T?) {
    t0 |> run("CUDA q8 cm2 s-tile split in two and four k chunks + the reduce == the f16-feed CPU oracle: the short window's form") <| @(t : T?) {
        if (!ensure_q8_batch_cm2s_cls() || !ensure_splitk_reduce_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        q8_split_run(t, CUD_CM2_TC_S, 31, 1024, 2)
        q8_split_run(t, CUD_CM2_TC_S, 31, 2048, 4)
        q8_split_run(t, CUD_CM2_TC_S, 31, 1280, 2)   // the last chunk short: 1280 in two 256-aligned shares is 768 and 512
    }
}

//! every format's s column split along K at 31 rows - the short window's form on every plane the window admits - against the
//! format's own oracle: the Vulkan short-split cell's rows (a per-32 row's K off the 256 grid splits in two, its last chunk short)
[test]
def test_cud_cm2s_short_split_family(t0 : T?) {
    t0 |> run("CUDA s column split along K under 32 rows == the CPU oracle, every per-format tile row") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cm2s_cls() || !ensure_splitk_reduce_cls()) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            var tiles <- fmt_tile_rows()
            for (row in tiles) {
                let fmt = int(row.fmt)
                t |> success(cud_cm2_ensure(fmt, CUD_CM2_TC_S), "{row.fmt} s column: the leaf compiles")
                var wqh, wsh : array<uint>
                fmt_tile_planes(row, wqh, wsh)
                cm2_split_run(t, "{row.fmt} s column", fmt, CUD_CM2_TC_S, 31, TILE_CELL_D, row.n, fmt_split_ways(row.n), wqh, wsh, row.xf_add, row.oracle)
                delete wqh
                delete wsh
            }
            delete tiles
        } else {
            t |> skip("dasVulkan not present: the per-format oracles live with the Vulkan cells")
        }
    }
}


[test]
def test_cud_short_window_pick(t0 : T?) {
    t0 |> run("the CUDA window pick under the s column's rows: whole off the knee, split by the shared rule on it") <| @(t : T?) {
        if (!ensure_q8_batch_cm2s_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let saved = cud_split_k_tgs()
        set_cud_split_k_tgs(0l)
        t |> equal(pick_str(cud_cm2_pick(1024l, 31l, 1024l)), "32x1x0", "the short window off its knee: the s column whole - knee 0 leaves those windows whole, never the wave model's split")
        set_cud_split_k_tgs(256l)
        t |> equal(pick_str(cud_cm2_pick(1024l, 31l, 1024l)), "32x4x256", "... on it the shared rule splits the 8-block grid into four 256-deep chunks")
        t |> equal(pick_str(cud_cm2_pick(1024l, 31l, 1536l)), "32x2x768", "a 1536-deep K stops doubling where the next share would leave the 256-wide chunks")
        let full = pick_str(cud_cm2_pick(1024l, 32l, 1024l))
        t |> equal(cud_cm2_pick(1024l, 32l, 1024l).tc, 32l, "a full column is the wave model's whatever the knee: the s column, split by the card's block slots")
        set_cud_split_k_tgs(0l)
        t |> equal(pick_str(cud_cm2_pick(1024l, 32l, 1024l)), full, "... the same pick with the knee off: the short rule stops under 32 rows, the full column is the wave model's")
        set_cud_split_k_tgs(saved)
    }
}

[test]
def test_cud_q8_cm2l_split(t0 : T?) {
    t0 |> run("CUDA q8 cm2 l-tile split in two k chunks + the reduce == the f16-feed CPU oracle") <| @(t : T?) {
        if (!ensure_q8_batch_cm2l_cls() || !ensure_splitk_reduce_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        q8_split_run(t, CUD_CM2_TC_L, TILE_ROWS0, 576, 2)   // the two-chunk split of the Vulkan cell's K: 512 and 64 deep
        q8_split_run(t, CUD_CM2_TC_L, TILE_ROWS0, 2048, 2)
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

//! the NVFP4 leaf on the FP4 tensor rung: the k4-tiled nv4 plane (sixteen UE4M3 strip codes a superblock as four words) against an FP4 feed in the (k, k + 8) pairing, the reference each element's e2m1 x UE4M3 on both sides under the row scale and the site scale
[test]
def test_cud_nv4_mmq(t0 : T?) {
    t0 |> run("CUDA NVFP4 GEMM on the FP4 tensor rung == the e2m1 x UE4M3 CPU reference under the row scales and the site scale") <| @(t : T?) {
        if (!ensure_nv4_mmq()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let k = 768   // three superblocks: six k128 steps, the two-deep ring wraps
        let d = 256
        let rows = 256
        let nsb = k / 256
        let site_scale_v = 0.75
        var wq <- hashed_words(d * nsb * 32, 1u, 23u)
        var ws : array<uint>
        ws |> resize(d * nsb * 5)
        for (sb in range(d * nsb)) {
            for (j in range(4)) {
                ws[sb * 5 + j] = nv4_test_scale_word(sb * 4 + j)
            }
            ws[sb * 5 + 4] = float_bits_to_uint(site_scale_v)   // the plane's factor rides its scale rows' fifth word
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
        var pc = MmqFp4Args(n = uint(k), d = uint(d), rows = uint(rows), wblk = 0u)
        enc_nv4_mmq(g_cud.stream, bufs, pc, mmq_wgs(int64(d), int64(rows)))
        t |> success(cud_sync(), "nv4 mmq: the launch completes ({cuda_last_error()})")
        let y <- buf_download(bufs[5], rows * d, type<float>)
        cuda_mem_free(bufs)
        var feed <- Fp4Feed(xq <- xq, xsw <- xs, xrs <- xr)
        let want <- [for (i in range(rows * d)); nv4_mmq_ref(wq, ws, feed, k, i / d, i % d, site_scale_v)]
        check_rows(t, "nv4 MMQ on the FP4 rung vs the e2m1 x UE4M3 reference", y, want, Q8_GEMV_BAR, d)
        delete feed
    }
}

//! the Q4_K leaf over the engine's repacked superblocks (element i the low nibble, i + 16 the high; five scale words a superblock) against the GEMV's fold of each block: xs x (d sc dot - dmin mn qsum)
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
        var xsum <- q8_block_sums(xq, xs, rows, k)
        let s = g_cud.stream
        var bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(xq, s), buf_upload(xs, s), buf_fill(rows * d, Y_SENTINEL, s), buf_upload(xsum, s))
        var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(rows))
        enc_k4_mmq(g_cud.stream, bufs, pc, mmq_wgs(int64(d), int64(rows)))
        t |> success(cud_sync(), "k4 mmq: the launch completes ({cuda_last_error()})")
        let y <- buf_download(bufs[4], rows * d, type<float>)
        cuda_mem_free(bufs)
        let want <- [for (i in range(rows * d)); k4_mmq_ref(wq, ws, xq, xs, k, i / d, i % d)]
        check_rows(t, "k4 MMQ vs the block-scaled reference", y, want, TILE_BAR, d)   // the min terms are f16-staged: the f16 tiles' bar, not the Q8 leaf's per-element one
    }
}

//! one min-term-free superblock leaf over the Q4_K nibble tiling - IQ4_XS (the codebook; a two-word row of d and the scales' bits), Q4_0 (the nibbles less eight; the d-pair row) or IQ4_NL (the codebook; the d-pair row) - against xs x scale x dot a block
def private cud_sb_mmq_run(t : T?; f : KqFmt) {
    let k = 1024   // four superblocks: the scale rows hand over three times
    let d = 256
    let rows = 256
    let nsb = k / 256
    let nb = k / 32
    let two_word_scales = f == KqFmt.iq4xs
    var wq <- hashed_words(d * nsb * 32, 17u, 9u + uint(f))   // eight uint4 a superblock
    var ws <- hashed_words(d * nsb * (two_word_scales ? 2 : 5), 5u, 23u)   // IQ4_XS: the scale bits hashed, d in the first word's low half; else four words of d pairs and a pad
    for (sb in range(d * nsb)) {
        if (two_word_scales) {
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
    let bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(xq, s), buf_upload(xs, s), buf_fill(rows * d, Y_SENTINEL, s))
    var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(rows))
    let wgs = mmq_wgs(int64(d), int64(rows))
    var bufs6 = fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], 0ul)
    cud_mmq_enc(int(f), s, bufs6, pc, wgs)
    t |> success(cud_sync(), "{f} mmq: the launch completes ({cuda_last_error()})")
    let y <- buf_download(bufs[4], rows * d, type<float>)
    cuda_mem_free(bufs)
    let want <- [for (i in range(rows * d)); sb_mmq_ref(int(f), wq, ws, xq, xs, k, i / d, i % d)]
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

//! one per-32 leaf against the engine's own per-32 dequant dotted with the activation quants: Q5_1's m through the f16-staged correction (the f16 tiles' bar), Q4_0, Q5_0 and IQ4_NL per element
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
    var xsum <- q8_block_sums(xq, xs, rows, k)
    var s = g_cud.stream
    var bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(xq, s), buf_upload(xs, s), buf_fill(rows * d, Y_SENTINEL, s), buf_upload(xsum, s))
    var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(rows))
    let wgs = mmq_wgs(int64(d), int64(rows))
    cud_mmq_enc(int(f), s, bufs, pc, wgs)
    t |> success(cud_sync(), "{f} mmq: the launch completes ({cuda_last_error()})")
    let y <- buf_download(bufs[4], rows * d, type<float>)
    cuda_mem_free(bufs)
    let kqb <- word_plane_bytes(wq)
    let ksb <- word_plane_bytes(ws)
    let want <- [for (i in range(rows * d)); b32_mmq_ref(f, kqb, ksb, xq, xs, k, i / d, i % d)]
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

//! buckets of 200 and 300 rows padded to 128-row tiles by the CPU twin, k 2880 (the k tail: 22 full steps and a half), d 320 (a column past `d` stored into the next row reds that row), two sentinel workgroups
[test]
def test_cud_mx4_fp4_mmq_moe(t0 : T?) {
    t0 |> run("CUDA mx4 FP4 leaf over the expert schedule == the e2m1 x UE8M0 reference: padded buckets, the k tail at 2880, the column guard at 320, sentinel workgroups") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_mx4_fp4_mmq_moe()) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            if (g_cud.sm < FP4_RUNG_SM) {
                t |> skip("the FP4 tensor rung needs compute capability {FP4_RUNG_SM}: this card is {g_cud.sm}")
                return
            }
            let n = 2880
            let nb = n / 32
            let d = 320
            let ne = 2
            let wt = (d + 127) / 128
            let bucket_a = 200
            let bucket_b = 300
            var idx <- [for (r in range(bucket_a + bucket_b)); uint(r % 5 < 2 ? 0 : 1)]   // the two buckets' rows interleaved, two of expert 0 to three of expert 1
            let nk = length(idx)
            let start_b = bucket_start(idx, 1, MMQ_ROW_PAD)
            let nk_pad = bucket_start(idx, ne, MMQ_ROW_PAD)
            let sh = sched_shape(nk, ne, wt, wt)
            let expert_blocks = uint(d * nb)
            let bases = fixed_array(0u, expert_blocks, 0u, expert_blocks, 0u, expert_blocks)
            var inv, s1, s3, s2 : array<uint>
            sched_oracle(idx, ne, wt, wt, sh, bases, inv, s1, s3, s2, false, MMQ_ROW_PAD, true)
            s1 |> push(SCHED_NONE)
            s1 |> push(SCHED_NONE)
            var wq <- hashed_words(ne * d * nb * 4, 53u, 977u)
            var ws <- mx4_scale_words(ne * d * nb)
            let xrows = nk_pad + int(TILE_READ_SLACK)
            var inscope src <- row_fill(xrows * n, 37, 0, 0.09, 4.2, 101)
            var inscope feed <- mx4_feed_rows(src, xrows, n)
            var s = g_cud.stream
            var bufs = fixed_array(buf_upload(wq, s), buf_upload(ws, s), buf_upload(feed.xq, s), buf_upload(feed.xsw, s), buf_upload(feed.xrs, s), buf_fill(nk_pad * d, Y_SENTINEL, s), buf_upload(s1, s))
            var pc = MmqFp4Args(n = uint(n), d = uint(d), rows = 0u, wblk = 0u, map_off = uint(sh.map_off_mgu))
            enc_mx4_fp4_mmq_moe(s, bufs, pc, int64(sh.bound_mgu + 2))
            t |> success(cud_sync(), "mx4 FP4 leaf over the schedule: the launch completes ({cuda_last_error()})")
            var y <- buf_download(bufs[5], nk_pad * d, type<float>)
            cuda_mem_free(bufs)
            var got, want : array<float>
            got |> reserve(nk * d)
            want |> reserve(nk * d)
            for (b in range(nk_pad)) {
                let e = b < start_b ? 0 : 1
                continue if (e == 0 ? b >= bucket_a : b >= start_b + bucket_b)
                for (col in range(d)) {
                    got |> push(y[b * d + col])
                    want |> push(mx4_fp4_mmq_ref(wq, ws, feed, e * int(expert_blocks), n, b, col))
                }
            }
            check_rows(t, "mx4 FP4 leaf over the schedule vs the e2m1 x UE8M0 reference (the real rows of both buckets)", got, want, Q8_GEMV_BAR, d)
            delete got
            delete want
            delete y
            delete idx
            delete inv
            delete s1
            delete s3
            delete s2
            delete wq
            delete ws
        } else {
            t |> skip("dasVulkan not present: the schedule oracle lives with the Vulkan cells")
        }
    }
}

//! the K-quant cm2 tiles (the shared decode-in-load templates) on the l, m and s columns against the Vulkan cells' per-format oracles,
//! over the Vulkan cells' hashed planes and two regions of activation rows (whole tiles and an edge each)
[test]
def test_cud_kq_cm2_family(t0 : T?) {
    t0 |> run("CUDA kq cm2 l/m/s tiles == the CPU oracle (every superblock format; mx4 on its s and e columns alone)") <| @(t : T?) {
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
                let fmt = int(row.fmt)
                var wqh, wsh : array<uint>
                fmt_tile_planes(row, wqh, wsh)
                var xfh <- ws_plane((rows + int(TILE_READ_SLACK)) * row.n / 2, 3u, row.xf_add)
                var tcs <- row.fmt == KqFmt.mx4 ? [CUD_CM2_TC_S, CUD_CM2_TC_E] : [CUD_CM2_TC_L, CUD_CM2_TC_M, CUD_CM2_TC_S]
                for (tc in tcs) {
                    t |> success(cud_cm2_ensure(fmt, tc), "{row.fmt} cm2 tile {tc}: the leaf compiles")
                    let rows_per_wg = tc == CUD_CM2_TC_L ? 256 : (tc == CUD_CM2_TC_S ? 32 : 128)
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
                    let tag = "{row.fmt} cm2 {tc == CUD_CM2_TC_L ? "l" : (tc == CUD_CM2_TC_M ? "m" : (tc == CUD_CM2_TC_S ? "s" : "e"))} tile"
                    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
                    var y <- buf_download(bufs[4], rows * d, type<float>)
                    cuda_mem_free(bufs)
                    check_rows(t, "{tag} vs the CPU oracle", y, y_ref, cm_bar(y_ref), d)
                    delete y
                    delete y_ref
                    delete sched
                }
                delete tcs
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
        if (f == KqFmt.mx4) return mx4_gemv_row_oracle(wq, ws, xq, xs, wb0, nb)
        return q5032_gemv_row_oracle(wq, ws, xq, xs, wb0, nb)
    } else {
        return 0.0
    }
}

//! the per-32 decode GEMVs (the shared `Blk32GemvRunT` leaves) against the engine's scalar dots at the Vulkan cells' widths - a
//! subgroup a row, a lane a block over Q8_0 activations, two regions of three rows
[test]
def test_cud_b32_gemv_family(t0 : T?) {
    t0 |> run("CUDA per-32 GEMV leaves == the scalar dot's float order (q51, iq4nl32, q4032, q5032, and mx4 at gpt-oss's 2880)") <| @(t : T?) {
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
                       B32Cell(f = KqFmt.q5032, n0 = 896, n1 = 1408, qwords = int(KQ_Q5032_QWORDS), wq_seed = 73, xq_mul = 31, xq_add = 5),
                       B32Cell(f = KqFmt.mx4, n0 = 704, n1 = 2880, qwords = 4, wq_seed = 53, xq_mul = 19, xq_add = 7)]) {   // mx4: 22 blocks a row, then gpt-oss's 90 - three lane passes, the last partial
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

//! the token-table gathers: every superblock format's `KqEmb<Fmt>` leaf against the class lane decode (the Vulkan cell's oracle),
//! the per-32 leaves against the engine's plane dequant, and the exact f32 table's leaf against the rows' own floats - four rows,
//! three ids off a five-row table past a one-row lead, one `EMB_ID_HOST` row copying the host's staged row
[test]
def test_cud_emb_family(t0 : T?) {   // nolint:STYLE038 - one flat arm per gather stamp (every kq leaf, the q8 and the f32 table), a split would hide the arm list
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
            // the exact table's gather (`CuEmbGatherF32`, the `WF` arm): the row's floats scaled, the `EMB_ID_HOST` row the host's
            t |> success(ensure_emb_gather_f32_cls(), "the f32 table gather compiles")
            var inscope wf <- row_fill(vocab * n32, 11, 5, 0.02, -0.7)
            var inscope yf_ref : array<float>
            yf_ref |> resize(rows * n32)
            for (r in range(rows)) {
                for (e in range(n32)) {
                    yf_ref[r * n32 + e] = ids[r] == EMB_ID_HOST ? xh32[r * n32 + e] : wf[int(ids[r]) * n32 + e] * escale
                }
            }
            var sf = g_cud.stream
            let fwgs = (rows * n32 / 4 + 255) / 256
            let footprint = fwgs * 256 * 4   // the grid's lanes, four elements each: a broken lane guard writes up to here
            let fbufs = fixed_array(buf_upload(wf, sf), buf_upload(ids, sf), buf_fill(footprint, Y_SENTINEL, sf), buf_upload(xh32, sf))
            var pcf = EmbArgs(npos = uint(rows), dim = uint(n32), wblk0 = 0u, embed_scale = escale)
            enc_emb_gather_f32_cls(sf, fbufs, pcf, int64(fwgs))
            t |> success(cud_sync(), "f32 table gather: the launch completes ({cuda_last_error()})")
            var inscope yf_all <- buf_download(fbufs[2], footprint, type<float>)
            cuda_mem_free(fbufs)
            var inscope yf <- copy_span(yf_all, 0, rows * n32)
            check_rows(t, "the f32 table gather against the rows' own floats", yf, yf_ref, BAR_APPROX_CTRL, n32)
            var inscope kept_f <- [for (_i in range(footprint - rows * n32)); Y_SENTINEL]
            check_exact(t, "the f32 table gather: the {footprint - rows * n32} elements of the grid's footprint past the rows keep the sentinel", copy_span(yf_all, rows * n32, footprint - rows * n32), kept_f)
        } else {
            t |> skip("dasVulkan not present: the gather oracle runs the Vulkan leaves' decodes on the CPU")
        }
    }
}

//! the E-series pre-step's finish (`CuPleFinish`, a `GkPleFinish` stamp, the Vulkan cell's fixture): three layers' slices of 256
//! over two rows in place, side = (side + pnorm . rms(proj)) / sqrt(2) against the CPU form at the approx bar; the slices read as
//! one row miss it, the control
[test]
def test_cud_ple_finish(t0 : T?) {
    t0 |> run("ple_finish: the side rows == the CPU finish a slice at a time") <| @(t : T?) {
        if (!ensure_ple_finish_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let ple = 256
        let nsl = 6
        let eps = 1e-6
        let ascale = 1.0 / sqrt(2.0)
        var inscope a <- row_fill(ple * nsl, 13, 0, 0.06, 3.0, 89)
        var inscope x <- row_fill(ple * nsl, 37, 0, 0.11, 5.0, 101)
        var inscope w <- row_fill(ple, 1, 0, 0.05, 0.0, 17)
        var inscope want, flat : array<float>
        want |> resize(ple * nsl)
        flat |> resize(ple * nsl)
        var sall = 0.0
        for (v in a) {
            sall += v * v
        }
        let finv = 1.0 / sqrt(sall / float(ple * nsl) + eps)
        for (r in range(nsl)) {
            var sa = 0.0
            for (k in range(ple)) {
                sa += a[r * ple + k] * a[r * ple + k]
            }
            let ainv = 1.0 / sqrt(sa / float(ple) + eps)
            for (k in range(ple)) {
                want[r * ple + k] = (x[r * ple + k] + w[k] * a[r * ple + k] * ainv) * ascale
                flat[r * ple + k] = (x[r * ple + k] + w[k] * a[r * ple + k] * finv) * ascale
            }
        }
        check_exceeds(t, "ple_finish: the slice norms differ from one norm over every slice (the control)", want, flat, BAR_EXACT)
        var s = g_cud.stream
        var bufs = fixed_array(buf_upload(a, s), buf_upload(w, s), buf_upload(x, s))
        var pc = GkPleFinArgs(ple = uint(ple), eps = eps)
        enc_ple_finish_cls(s, bufs, pc, int64(nsl))
        t |> success(cud_sync(), "ple_finish: the launch completes ({cuda_last_error()})")
        var inscope got <- buf_download(bufs[2], ple * nsl, type<float>)
        cuda_mem_free(bufs)
        check_rows(t, "ple_finish against the CPU finish a slice at a time", got, want, BAR_APPROX_CTRL, ple)
        t |> success(moved_off(got, x, 0, BAR_APPROX), "ple_finish: the finish moved element 0 off its input past the bar (the in-place compare's control)")
    }
}

//! the bf16 router-form GEMV stamps: the pairs stamp at an even dim and the halfword stamp at an odd one, ne rows at a row base,
//! three columns in one dispatch under the output scale (`yscale`), against the CPU dot at the approx bar; the sentinel past the
//! last column stands, a poisoned output the control
[test]
def test_cud_router_gemv_bf16(t0 : T?) {
    for (spec in [(dim = 1536, hw = false), (dim = 1537, hw = true)]) {
        t0 |> run("router_gemv_bf16{spec.hw ? "_hw" : ""} (dim {spec.dim}) == the CPU dot at a row base, an output base and three columns") <| @(t : T?) {
            if (!(spec.hw ? ensure_router_gemv_bf16_hw_cls() : ensure_router_gemv_bf16_cls())) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            let dim = spec.dim
            let ne = 32
            let ncols = 3
            let layer1_woff = ne * dim
            let obase = 16
            let out_n = obase + ncols * ne + 8
            var inscope wh <- hash_fill(layer1_woff + ne * dim, 4242u, 1000u, 500.0, 1.0)
            var inscope xh <- hash_fill(ncols * dim, 99u, 1000u, 500.0, 1.0)
            var inscope w16 <- pack_bf16(wh)
            let yscale = 1.0 / sqrt(float(dim))
            var inscope want : array<float>
            want |> resize(out_n)
            for (i in range(out_n)) {
                want[i] = Y_SENTINEL
            }
            for (c in range(ncols)) {
                for (e in range(ne)) {
                    var acc = 0.0
                    for (k in range(dim)) {
                        acc += bf16_value(w16[layer1_woff + e * dim + k]) * xh[c * dim + k]
                    }
                    want[obase + c * ne + e] = acc * yscale
                }
            }
            var s = g_cud.stream
            var bufs = fixed_array(buf_upload(w16, s), buf_upload(xh, s), buf_fill(out_n, Y_SENTINEL, s), buf_upload(xh, s))
            var pc = GkRouterArgs(ne = uint(ne), dim = uint(dim), woff = uint(layer1_woff), obase = uint(obase), ncols = uint(ncols), yscale = yscale)
            if (spec.hw) {
                enc_router_gemv_bf16_hw_cls(s, bufs, pc, int64(ne))
            } else {
                enc_router_gemv_bf16_cls(s, bufs, pc, int64(ne))
            }
            let tag = spec.hw ? "router_gemv_bf16_hw" : "router_gemv_bf16"
            t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
            var inscope got <- buf_download(bufs[2], out_n, type<float>)
            cuda_mem_free(bufs)
            check_rows(t, "{tag} against the CPU dot over three columns, the sentinel outside them", got, want, BAR_APPROX_CTRL, out_n)
        }
    }
}

//! the small f16 x f16 GEMM on the cm2 tile (`CuF16GemmCm2`, the template both discrete homes stamp): the Vulkan cell's direct arm
//! at its four shapes against the f32 dot of the same f16 values, the header under the output base kept
[test]
def test_cud_f16_gemm(t0 : T?) {
    t0 |> run("the small f16 GEMM on the cm2 tile (the E-series projection's window form) == the f32 dot, direct") <| @(t : T?) {
        if (!ensure_f16_gemm_cm2_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        for (shape in [(tag = "ba-like", d = 32, n = 512, rows = 70), (tag = "router-like", d = 200, n = 1024, rows = 130),
                       (tag = "odd router", d = 61, n = 512, rows = 41), (tag = "rel plane, ragged d, short K", d = 2495, n = 64, rows = 37),
                       (tag = "E2B projection, a whole window", d = 8960, n = 1536, rows = 512)]) {
            let d = shape.d
            let n = shape.n
            let rows = shape.rows
            let ybase = 16   // the rows land past a header the kernel must leave alone
            var inscope wh, xh : array<float16>
            wh |> resize(d * n)
            xh |> resize(rows * n)
            for (i in range(d * n)) {
                wh[i] = float16(float((i * 7) % 61) * 0.02 - 0.6)
            }
            for (i in range(rows * n)) {
                xh[i] = float16(float((i * 11) % 53) * 0.03 - 0.8)
            }
            var inscope ref : array<float>
            ref |> resize(rows * d)
            for (p in range(rows)) {
                for (e in range(d)) {
                    var acc = 0.0
                    for (kk in range(n)) {
                        acc += float(xh[p * n + kk]) * float(wh[e * n + kk])
                    }
                    ref[p * d + e] = acc
                }
            }
            // both operands are f16-exact, so every product is exact in f32 and the arms differ by f32 reassociation over K alone
            let bar = Bar(kind = BarKind.rel_of_max, rel = 2e-5, poison = 8e-5, poison_of_max = true)
            // the tile grid's footprint: 128-row by 128-column tiles, so a broken guard writes rows up to `rows_pad` and columns up to `d_pad` at the row stride - the plane carries that far past the rows, sentinel-filled and asserted untouched
            let rows_pad = ((rows + 127) / 128) * 128
            let d_pad = ((d + 127) / 128) * 128
            let total = ybase + (rows_pad - 1) * d + d_pad
            var s = g_cud.stream
            var bufs = fixed_array(buf_upload(wh, s), buf_upload(xh, s), buf_fill(total, NAN_SENTINEL, s))
            var pc = F16GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = 0u, ybase = uint(ybase), ystride = uint(d), ksplit = 0u)
            enc_f16_gemm_cm2_cls(s, bufs, pc, f16_gemm_cm2_wgs(int64(rows), int64(d)))
            t |> success(cud_sync(), "f16 GEMM {shape.tag}: the launch completes ({cuda_last_error()})")
            var inscope y <- buf_download(bufs[2], total, type<float>)
            cuda_mem_free(bufs)
            var inscope header_bits <- [for (i in range(ybase)); float_bits_to_uint(y[i])]
            var inscope sentinel_bits <- [for (_i in range(ybase)); NAN_SENTINEL_BITS]
            check_exact(t, "f16 GEMM {shape.tag}: the {ybase} words under the base stay the sentinel", header_bits, sentinel_bits)
            var inscope got <- copy_span(y, ybase, rows * d)
            check_rows(t, "f16 GEMM {shape.tag}: {rows} x {d} over K {n} vs the f32 dot", got, ref, bar, d)
            let past = total - ybase - rows * d
            if (past > 0) {
                var inscope past_bits <- [for (i in range(past)); float_bits_to_uint(y[ybase + rows * d + i])]
                var inscope past_sentinel <- [for (_i in range(past)); NAN_SENTINEL_BITS]
                check_exact(t, "f16 GEMM {shape.tag}: the {past} words of the tile grid's footprint past the rows keep the sentinel", past_bits, past_sentinel)
            }
        }
    }
}

//! the per-layer-embedding projection's f16 panel (`CuPleBf16Cvt`, the Vulkan cell's form): bf16 halves widened by their bit shift
//! and stored f16 bit for bit the language's half cast of the widened value, a value past the half range saturated, the elements
//! past the count kept
[test]
def test_cud_ple_bf16_cvt(t0 : T?) {
    t0 |> run("ple_bf16_cvt: the bf16 rows as f16 == the widened value's half, saturated") <| @(t : T?) {
        if (!ensure_ple_bf16_cvt_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        let n = 1030
        var inscope src <- [for (i in range(n)); 0.37 * float(i % 61) - 11.0]
        src[7] = 1.0e6   // past the half range: saturates at 65504
        src[8] = -1.0e6
        var inscope w16 <- pack_bf16(src)
        var inscope want <- [for (h in w16); float(float16(clamp(bf16_value(h), -65504.0, 65504.0)))]
        var s = g_cud.stream
        let wgs = (n / 4 + 255) / 256 + 1   // a workgroup past the count: its lanes are the guard under test
        let footprint = wgs * 256 * 4
        let bufs = fixed_array(buf_upload(w16, s), buf_fill(footprint, uint16(0xFFFF), s))
        var pc = gk_f16_rows_args(uint(n))
        enc_ple_bf16_cvt_cls(s, bufs, pc, int64(wgs))
        t |> success(cud_sync(), "ple_bf16_cvt: the launch completes ({cuda_last_error()})")
        var inscope got <- buf_download(bufs[1], n, type<float16>)
        var inscope tail <- buf_download(bufs[1], footprint, type<uint16>)
        cuda_mem_free(bufs)
        var inscope gotf <- [for (i in range(n)); float(got[i])]
        check_exact(t, "ple_bf16_cvt: every element the widened value's half", gotf, want)
        t |> equal(float(got[7]), 65504.0, "ple_bf16_cvt: a value past the half range saturates")
        var inscope kept <- [for (_i in range(footprint - n)); uint16(0xFFFF)]
        check_exact(t, "ple_bf16_cvt: the {footprint - n} elements of the grid's footprint past the count kept", copy_span(tail, n, footprint - n), kept)
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

//! the biased act twins (`ActRqBT` Q8_0 / Q8_K, `ActF16T` BIASED) over four rows of experts `rex`, the second of two bias layers bound (`gboff`); the oracle is the unbiased act over the host-biased rows, the unbiased oracle each form's control
def private cud_act_biased_arm(t : T?; var s : void?; act : uint; q8k : bool; f16 : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let brow = 256
        let nrows = 4
        let nelem = brow * nrows
        let ne = 6
        let nblk = q8k ? nelem / 256 : nelem / 32
        var gh <- row_fill(nelem, 29, 0, 0.21, 6.0, 61)
        var uh <- row_fill(nelem, 11, 0, 0.17, 4.0, 47)
        var gbh <- row_fill(2 * ne * brow, 37, 0, 0.09, 0.8, 31)
        var ubh <- row_fill(2 * ne * brow, 43, 0, 0.07, 0.6, 23)
        var rex <- [for (i in range(nrows)); uint(fixed_array(1, 4, 0, 5)[i])]
        let boff = ne * brow   // the second layer's rows
        var gb_in <- [for (i in range(nelem)); gh[i] + gbh[boff + int(rex[i / brow]) * brow + i % brow]]
        var ub_in <- [for (i in range(nelem)); uh[i] + ubh[boff + int(rex[i / brow]) * brow + i % brow]]
        var pc = ActArgs(nelem = uint(nelem), act = act, nblk = uint(nblk), elem0 = 0u, bias_on = 1u, gboff = uint(boff), brow = uint(brow))
        let tag = "{f16 ? "f16" : (q8k ? "q8k" : "q8")} biased act{act == ACT_OAI ? " under the clamped swiglu" : ""}"
        if (f16) {
            var bufs = fixed_array(buf_upload(gh, s), buf_upload(uh, s), buf_fill(nelem / 2, NAN_SENTINEL_BITS, s), buf_upload(gbh, s), buf_upload(ubh, s), buf_upload(rex, s))
            enc_actf16_b_cls(s, bufs, pc, rq_wgs(false, int64(nelem)))
            t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
            var hw <- buf_download(bufs[2], nelem / 2, type<uint>)
            var got <- [for (i in range(nelem)); half_at(hw, i)]
            var want <- [for (i in range(nelem)); float(float16(act_mul_ref(gb_in[i], ub_in[i], act)))]
            var ctrl <- [for (i in range(nelem)); float(float16(act_mul_ref(gh[i], uh[i], act)))]
            check_rows(t, "{tag} vs the CPU act over the host-biased rows, as halves", got, want, Bar(rel = 2e-3, abs_floor = 2e-3 * logits_maxabs(want)), brow)
            check_exceeds(t, "{tag} control: the unbiased act", got, ctrl, Bar(rel = 2e-3, abs_floor = 2e-3 * logits_maxabs(want)))
            cuda_mem_free(bufs)
            delete hw
            delete got
            delete want
            delete ctrl
        } else {
            var bufs = fixed_array(buf_upload(gh, s), buf_upload(uh, s), buf_fill(nelem / 4, I8_SENTINEL_BITS, s), buf_fill(nblk, NAN_SENTINEL, s), buf_upload(gbh, s), buf_upload(ubh, s), buf_upload(rex, s))
            if (q8k) {
                enc_q8k_actrq_b_cls(s, bufs, pc, rq_wgs(true, int64(nelem)))
            } else {
                enc_q8_actrq_b_cls(s, bufs, pc, rq_wgs(false, int64(nelem)))
            }
            t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
            var q <- buf_download(bufs[2], nelem / 4, type<uint>)
            var sc <- buf_download(bufs[3], nblk, type<float>)
            var q_ref, q_ctrl : array<uint>
            var s_ref, s_ctrl : array<float>
            actrq_oracle(gb_in, ub_in, nelem, q8k, act, q_ref, s_ref)
            actrq_oracle(gh, uh, nelem, q8k, act, q_ctrl, s_ctrl)
            let blk = nelem / nblk
            check_rows(t, "{tag}'s quants vs the CPU oracle over the host-biased rows, a byte within one step (a row a block)", qbyte_values(q), qbyte_values(q_ref), QBYTE_BAR, blk)
            check_rows(t, "{tag}'s scales vs the CPU oracle (a row a block)", sc, s_ref, BAR_APPROX_CTRL, 1)
            check_exceeds(t, "{tag} control: the unbiased oracle's scales", sc, s_ctrl, BAR_APPROX_CTRL)
            cuda_mem_free(bufs)
            delete q
            delete sc
            delete q_ref
            delete q_ctrl
            delete s_ref
            delete s_ctrl
        }
        delete gh
        delete uh
        delete gbh
        delete ubh
        delete rex
        delete gb_in
        delete ub_in
    }
}

[test]
def test_cud_act_biased(t0 : T?) {
    t0 |> run("CUDA biased act twins (gpt-oss) == the CPU act over the host-biased rows: the Q8_0 and Q8_K requant forms and the f16 form, under silu and the clamped swiglu") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_q8_actrq_b_cls() && ensure_q8k_actrq_b_cls() && ensure_actf16_b_cls())) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            var s = g_cud.stream
            cud_act_biased_arm(t, s, ACT_OAI, false, false)
            cud_act_biased_arm(t, s, ACT_OAI, true, false)
            cud_act_biased_arm(t, s, ACT_OAI, false, true)
            cud_act_biased_arm(t, s, ACT_SILU, false, false)
            cud_act_biased_arm(t, s, ACT_SILU, false, true)
        } else {
            t |> skip("dasVulkan not present: the act oracles live with the Vulkan cells")
        }
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

let private FEED_WORD_SENTINEL = 0xFFFFFFFFu   //! an FP4 feed plane's fill: every scale code the format's NaN, which no finite strip takes, every nibble -6 (the compare asserts its CPU feed holds no such word)

//! the feed planes of `rows` rows of `k` (`bufs[2..4]`: nibbles, codes, row scales) put back under their sentinels before a run
def private fp4_feed_refill(var bufs : uint64[5]; rows, k : int) {
    let s = g_cud.stream
    cuda_mem_free(fixed_array(bufs[2], bufs[3], bufs[4]))
    bufs[2] = buf_fill(rows * k / 8, FEED_WORD_SENTINEL, s)
    bufs[3] = buf_fill(rows * k / 64, FEED_WORD_SENTINEL, s)
    bufs[4] = buf_fill(rows, NAN_SENTINEL, s)
}

//! the nibble and code words of `q` / `sw` off `ref`'s: the count and the first index off (-1 where none is; the code words count past the nibble words)
def private fp4_feed_words_off(q, sw : array<uint>; ref : Fp4Feed) : tuple<off : int; first : int> {
    let fq = first_mismatch(q, ref.xq)
    let fs = first_mismatch(sw, ref.xsw)
    return (off = mismatch_exact(q, ref.xq) + mismatch_exact(sw, ref.xsw), first = fq >= 0 ? fq : (fs >= 0 ? length(q) + fs : -1))
}

//! the FP4 feed planes of `rows` rows of `k` read back against the CPU feed: nibble and code words off by count (at most `allowed`),
//! the count and the first word off logged, the CPU feed with `allowed + 1` words flipped past the allowance (the count's control);
//! the row scales at the approx bar
def private fp4_feed_compare(t : T?; tag : string; bufs : uint64[5]; qi, si, ri : int; ref : Fp4Feed; rows, k : int; allowed : int) {
    var q <- buf_download(bufs[qi], rows * k / 8, type<uint>)
    var sw <- buf_download(bufs[si], rows * k / 64, type<uint>)
    var rs <- buf_download(bufs[ri], rows, type<float>)
    t |> success(find_index(ref.xq, FEED_WORD_SENTINEL) < 0 && find_index(ref.xsw, FEED_WORD_SENTINEL) < 0, "{tag}: the CPU feed holds no word of the fill, so the fill is a sentinel")
    let o = fp4_feed_words_off(q, sw, ref)
    to_log(LOG_INFO, "{tag}: {o.off} of {length(q) + length(sw)} nibble / code words off the CPU feed, the first at word {o.first}\n")
    t |> success(o.off <= allowed, "{tag}: {o.off} nibble / code words off the CPU feed (at most {allowed}), the first at word {o.first}")
    var poisoned := ref
    for (i in range(min(2 * allowed + 1, length(poisoned.xq)))) {   // at most `allowed` of these words were off: flipping them leaves at least `allowed + 1` off
        poisoned.xq[i] ^= 1u
    }
    t |> success(fp4_feed_words_off(q, sw, poisoned).off > allowed, "{tag}: the count's control - the CPU feed with {2 * allowed + 1} words flipped lands past the allowance")
    check_rows(t, "{tag}: the row scales vs the CPU feed", rs, ref.xrs, BAR_APPROX_CTRL, 1)
    delete poisoned
    delete q
    delete sw
    delete rs
}

//! the FP4 feed kernels against the CPU feed: a float plane's rows bit for bit (the kernel is `precise`), the gate / up pair through the act (its fast-math exponential rounds a few strips' code or nibble the other way), the fused residual step + norm + feed
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
        var bufs = fixed_array(buf_upload(src, s), 0ul, buf_fill(rows * k / 8, FEED_WORD_SENTINEL, s), buf_fill(rows * k / 64, FEED_WORD_SENTINEL, s), buf_fill(rows, NAN_SENTINEL, s))
        var pc = Fp4RowsArgs(k = uint(k), inbase = 0u)
        enc_fp4_rows(s, fixed_array(bufs[0], bufs[2], bufs[3], bufs[4]), pc, int64(rows))
        t |> success(cud_sync(), "fp4_rows: the launch completes ({cuda_last_error()})")
        fp4_feed_compare(t, "fp4_rows", bufs, 2, 3, 4, ref, rows, k, 0)
        cuda_mem_free(bufs[0])
        fp4_feed_refill(bufs, rows, k)
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
        fp4_feed_refill(bufs, rows, k)
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

//! the FP4 expert leaf's width admission: both reduction widths on the 64 lattice serve (gpt-oss's 2880 over 2880), a width at 32 past it on either side keeps the cm2 tiles
[test]
def test_cud_moe_mmq_fits(t : T?) {
    t |> run("moe_mmq_fits: the gate/up and down reduction widths on the 64 lattice") @(t : T?) {
        t |> equal(true, crd_moe_mmq_fits(2880l, 2880l))
        t |> equal(true, crd_moe_mmq_fits(2048l, 768l))
        t |> equal(false, crd_moe_mmq_fits(2912l, 2880l))
        t |> equal(false, crd_moe_mmq_fits(2880l, 2912l))
        t |> equal(false, crd_moe_mmq_fits(2880l, 2896l))
    }
}

//! mx4_rows bit for bit with the CPU feed (the kernel is `precise`); mx4_actrows and its biased twin (each row's expert's gate and up bias added first) allowed a few strips the act's fast-math exponential rounds the other way
[test]
def test_cud_mx4_feeds(t0 : T?) {
    t0 |> run("CUDA MXFP4 feed kernels (mx4_rows, mx4_actrows, mx4_actrows_b) == the CPU feed") <| @(t : T?) {
        if (!ensure_mx4_rows() || !ensure_mx4_actrows() || !ensure_mx4_actrows_b()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        var s = g_cud.stream
        let rows = 6
        let k = 2880   // gpt-oss's width: 45 k64 words a row
        var inscope src <- row_fill(rows * k, 37, 0, 0.09, 4.2, 101)
        var inscope ref <- mx4_feed_rows(src, rows, k)
        var bufs = fixed_array(buf_upload(src, s), 0ul, buf_fill(rows * k / 8, FEED_WORD_SENTINEL, s), buf_fill(rows * k / 64, FEED_WORD_SENTINEL, s), buf_fill(rows, NAN_SENTINEL, s))
        var pc = Fp4RowsArgs(k = uint(k), inbase = 0u)
        enc_mx4_rows(s, fixed_array(bufs[0], bufs[2], bufs[3], bufs[4]), pc, int64(rows))
        t |> success(cud_sync(), "mx4_rows: the launch completes ({cuda_last_error()})")
        fp4_feed_compare(t, "mx4_rows", bufs, 2, 3, 4, ref, rows, k, 0)
        cuda_mem_free(bufs[0])
        fp4_feed_refill(bufs, rows, k)
        var inscope gh <- row_fill(rows * k, 29, 0, 0.21, 6.0, 61)
        var inscope uh <- row_fill(rows * k, 11, 0, 0.17, 4.0, 47)
        var inscope act <- [for (i in range(rows * k)); gk_act_mul(gh[i], uh[i], ACT_OAI)]
        var inscope aref <- mx4_feed_rows(act, rows, k)
        bufs[0] = buf_upload(gh, s)
        bufs[1] = buf_upload(uh, s)
        var pca = Fp4RowsArgs(k = uint(k), inbase = 0u, act = ACT_OAI)
        enc_mx4_actrows(s, fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4]), pca, int64(rows))
        t |> success(cud_sync(), "mx4_actrows: the launch completes ({cuda_last_error()})")
        fp4_feed_compare(t, "mx4_actrows", bufs, 2, 3, 4, aref, rows, k, rows * k / 800)
        fp4_feed_refill(bufs, rows, k)
        let ne = 3
        let gboff = 2 * ne * k   // the third layer's rows in a [layers x ne x k] bias plane
        var inscope gb <- row_fill(3 * ne * k, 5, 0, 0.3, 1.0, 19)
        var inscope ub <- row_fill(3 * ne * k, 7, 0, 0.2, -0.7, 23)
        var inscope rex <- [for (r in range(rows)); uint((r * 2 + 1) % ne)]
        var inscope bact <- [for (i in range(rows * k)); gk_act_mul(gh[i] + gb[gboff + int(rex[i / k]) * k + i % k], uh[i] + ub[gboff + int(rex[i / k]) * k + i % k], ACT_OAI)]
        var inscope bref <- mx4_feed_rows(bact, rows, k)
        let gbb = buf_upload(gb, s)
        let ubb = buf_upload(ub, s)
        let rxb = buf_upload(rex, s)
        var pcb = Fp4RowsArgs(k = uint(k), inbase = 0u, act = ACT_OAI, gboff = uint(gboff))
        enc_mx4_actrows_b(s, fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], gbb, ubb, rxb), pcb, int64(rows))
        t |> success(cud_sync(), "mx4_actrows_b: the launch completes ({cuda_last_error()})")
        let allowed = rows * k / 800
        fp4_feed_compare(t, "mx4_actrows_b", bufs, 2, 3, 4, bref, rows, k, allowed)
        var inscope bq <- buf_download(bufs[2], rows * k / 8, type<uint>)
        var inscope bsw <- buf_download(bufs[3], rows * k / 64, type<uint>)
        let unb = fp4_feed_words_off(bq, bsw, aref)
        t |> success(unb.off > allowed, "the control: the biased feed lands {unb.off} words off the unbiased CPU feed, past the {allowed} the compare allows - a kernel that dropped the biases fails it")
        cuda_mem_free(gbb)
        cuda_mem_free(ubb)
        cuda_mem_free(rxb)
        cuda_mem_free(bufs[0])
        cuda_mem_free(bufs[1])
        cuda_mem_free(bufs[2])
        cuda_mem_free(bufs[3])
        cuda_mem_free(bufs[4])
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

//! the driver's flash ladder (`cud_fa_enc`) twice over the same rows: the f32 store into `bufs[3]`, the f16 one into `o16`
def private fa_launch(t : T?; hs : int; gated, sink : bool; bufs : uint64[4]; o16 : uint64; sinkh : array<float>; var pc : FaArgs; wgs : int64) {
    var s = g_cud.stream
    let psink = sink ? buf_upload(sinkh, s) : 0ul
    cud_fa_enc(s, int64(hs), gated, false, sink, bufs, psink, pc, wgs)
    cud_fa_enc(s, int64(hs), gated, true, sink, fixed_array(bufs[0], bufs[1], bufs[2], o16), psink, pc, wgs)
    if (sink) {
        t |> success(cud_sync(), "fa h{hs} sink: the launches land before the sink plane goes ({cuda_last_error()})")
        cuda_mem_free(psink)
    }
}

//! the flash tile at head `hs` (64, 96, 128 or 256), f32 and f16 out, against the CPU attention oracle; under `gated` the q rows carry [q | gate] a head (the h256 `G` stamps); under `sink` a per-head sink logit inside the scaled scores' range (the gpt-oss h64 stamps)
def fa_run(t : T?; hs, window : int; gated : bool = false; sink : bool = false; cap : float = 0.0) {
    let nheads = 4
    let kv_mul = 2
    let rows = 77
    let w0 = 64
    let scale = 0.125
    var sinkh <- sink ? [for (hh in range(nheads)); 0.45 * float(hh) - 0.3] : array<float>()
    let qd = nheads * hs
    let qhs = gated ? 2 * hs : hs
    let qrow = nheads * qhs
    let kvd = (nheads / kv_mul) * hs
    let kpos = w0 + rows
    let kpad = kpos + int(RDEC_MIR_SLACK_ROWS)
    // the softcap fixture's wide q and k values (the Vulkan cell's): the scaled scores run past the cap, so the cap is load-bearing
    var qh <- row_fill(rows * qrow, 41, 0, cap != 0.0 ? 0.25 : 0.05, cap != 0.0 ? 9.0 : 1.8, 73)
    var kh <- row_fill(kpos * kvd, 17, 0, cap != 0.0 ? 0.15 : 0.03, cap != 0.0 ? 7.0 : 1.4, 97)
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
        qstride = uint(qrow), qhs = uint(qhs), window = uint(window), cap = cap, sinkoff = 0u, row0 = 0u, ring = 0u)
    let wgs = fa_stamp_wgs(cud_fa_khr(int64(hs)), int64(nheads), int64(rows))
    fa_launch(t, hs, gated, sink, bufs, o16, sinkh, pc, wgs)
    let cap_tag = cap != 0.0 ? " softcap {cap}" : ""
    let tag = "fa h{hs}{gated ? " gated" : ""}{sink ? " sink" : ""}{cap_tag} window {window}"
    t |> success(cud_sync(), "{tag}: the launches complete ({cuda_last_error()})")
    var y <- buf_download(bufs[3], rows * qd, type<float>)
    var y16 <- buf_download(o16, rows * qd, type<float16>)
    cuda_mem_free(bufs)
    cuda_mem_free(o16)
    var y_ref <- fa_oracle(qh, kh, vh, rows, w0, nheads, hs, kv_mul, kvd, scale, window, gated, sinkh, cap)
    check_rows(t, "{tag} vs the CPU attention oracle", y, y_ref, TILE_BAR, qd, hs)
    // the f16 twin's store rounds at 2^-11 = 4.9e-4 of the element, under the 2e-3 of the plane's largest the f32 tile's reassociation already takes
    var y16f <- [for (h in y16); float(h)]
    check_rows(t, "{tag}: the f16-out twin vs the CPU attention oracle", y16f, y_ref, TILE_BAR, qd, hs)
    if (gated || sink || cap != 0.0) {   // the ungated (sink-free, uncapped) oracle is the control the rows must miss
        let nosink : array<float>
        var qplain <- q_halves(qh, rows, nheads, hs, qhs)
        var y_plain <- fa_oracle(qplain, kh, vh, rows, w0, nheads, hs, kv_mul, kvd, scale, window, false, nosink)
        check_exceeds(t, "{tag}: the rows land off the {gated ? "ungated" : (sink ? "sink-free" : "uncapped")} oracle", y, y_plain, Bar(rel = 2e-3, abs_floor = 2e-3 * logits_maxabs(y_plain)))
        delete qplain
        delete y_plain
    }
    delete sinkh
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

[test]
def test_cud_fa_h64(t0 : T?) {
    t0 |> run("CUDA cm2 flash tile at head 64, f32 and f16 out == the CPU attention oracle, whole and under a 40-key window; the sink stamps (gpt-oss) seeded with a per-head sink logit, whole and under the window") <| @(t : T?) {
        if (!(ensure_fa_cm2_h64_cls() && ensure_fa_cm2_h64_f16_cls() && ensure_fa_cm2_h64_sink_cls() && ensure_fa_cm2_h64_sink_f16_cls())) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        fa_h64_run(t, 0)
        fa_h64_run(t, 40)
        fa_run(t, 64, 0, false, true)
        fa_run(t, 64, 40, false, true)
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
def private cud_q8_attn_arm(t : T?; nheads, kv_mul : int; sink : bool = false; nsplit : int = 1) {
    let hs = 64
    let kvd = nheads / kv_mul * hs
    let qd = nheads * hs
    let keys = 300   // past the 256-key chunk, so the running max rescales; under eight pieces of 64 the last three pieces are empty
    let base = ODD_LAYERBASE
    let g2 = !sink && da_slab_is_g2(int64(kv_mul))   // the sink stamp is the four-head slab's
    let pieces = nsplit > 1 ? " in {nsplit} pieces" : ""
    let tag = "da_attn_q8{sink ? "_sink" : (g2 ? "_g2" : "")} {nheads} heads over {nheads / kv_mul}, {keys} keys{pieces}"
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
    var sinks <- sink ? sink_logits(qrt, km.deq, nheads, hs, kv_mul, keys, kvd, scale) : array<float>()
    var y_ref, y_ctrl : array<float>
    y_ref |> resize(qd)
    y_ctrl |> resize(qd)
    for (hh in range(nheads)) {
        attn_row_oracle(qrt, hh * hs, km.deq, 0, vm.deq, 0, kvd, hh, hs, kv_mul, keys, scale, y_ref, hh * hs, 0, 0.0, sink, sink ? sinks[hh] : 0.0)
        if (sink) {
            attn_row_oracle(qrt, hh * hs, km.deq, 0, vm.deq, 0, kvd, hh, hs, kv_mul, keys, scale, y_ctrl, hh * hs)
        } else {
            attn_row_oracle(qrt, hh * hs, kf, 0, vf, 0, kvd, hh, hs, kv_mul, keys, scale, y_ctrl, hh * hs)
        }
    }
    var toks : array<TokMeta>
    toks |> resize(1)
    toks[0].pos = uint(keys - 1)
    toks[0].cnt = uint(keys)
    let rowwg = da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit))
    let cntoff = nheads * nsplit * (hs + 2)   // the split form's partials, the arrival counters past them (the driver's `attn_cnt_off` at one row)
    var pa = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(base), vbase = uint(base), scale = scale,
        qstride = uint(hs), gated = 0u, window = 0u, cap = 0.0, nsplit = uint(nsplit), rqk = DA_RQ_NONE, sinkoff = 0u, rowwg = uint(rowwg),
        qrow = uint(qd), cntoff = uint(cntoff), ring = 0u)
    var s = g_cud.stream
    let bufs = fixed_array(buf_upload(q, s), buf_upload(km.bytes, s), buf_upload(vm.bytes, s), buf_fill(qd, Y_SENTINEL, s), buf_upload(toks, s),
        buf_fill(cntoff + 64, 0.0, s), buf_fill(64, 0.0, s), buf_fill(64, 0.0, s))
    if (sink) {
        let psink = buf_upload(sinks, s)
        enc_da_attn_q8_sink_cls(s, fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5], bufs[6], bufs[7], psink), pa, rowwg)
        t |> success(cud_sync(), "{tag}: the launch lands before the sink plane goes ({cuda_last_error()})")
        cuda_mem_free(psink)
    } elif (g2) {
        enc_da_attn_q8_g2_cls(s, bufs, pa, rowwg)
    } else {
        enc_da_attn_q8_cls(s, bufs, pa, rowwg)
    }
    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
    var y <- buf_download(bufs[3], qd, type<float>)
    let bar = Bar(rel = 1e-4, poison_of_max = true, poison_abs = 0.5)
    check_rows(t, "{tag} vs the CPU attention over the dequantized rows", y, y_ref, bar, qd)
    check_exceeds(t, "{tag} control: the attention over the {sink ? "same rows with no sink" : "rows before their quantization"}", y, y_ctrl, bar)
    cuda_mem_free(bufs)
    delete sinks
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
    t0 |> run("CUDA decode attention's q8_0 stamps == the CPU attention over the dequantized mirror rows, the four-head and the two-head slab, and the sink stamp (gpt-oss) under a per-head sink logit") <| @(t : T?) {
        if (!(ensure_da_attn_q8_cls() && ensure_da_attn_q8_g2_cls() && ensure_da_attn_q8_sink_cls())) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        cud_q8_attn_arm(t, 8, 4)
        cud_q8_attn_arm(t, 4, 2)
        cud_q8_attn_arm(t, 8, 4, false, 8)   // the split form: eight pieces, the last-arriving one combining
        cud_q8_attn_arm(t, 8, 8, true)
        cud_q8_attn_arm(t, 8, 8, true, 8)   // ... and the sink seeded at the combine
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

//! the flash tile at head 128: the 3B class's attention, 32 keys a step
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

//! head 256 is the deltanet hybrids' attention head
[test]
def test_cud_fa_h256(t0 : T?) {
    t0 |> run("CUDA cm2 flash tile at head 256, f32 and f16 out, plain and gated == the CPU attention oracle, whole and under a 40-key window") <| @(t : T?) {
        if (!ensure_fa_cm2_h256_cls() || !ensure_fa_cm2_h256_f16_cls() || !ensure_fa_cm2_h256_g_cls() || !ensure_fa_cm2_h256_g_f16_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        fa_run(t, 256, 0)
        fa_run(t, 256, 40)
        fa_run(t, 256, 0, true)
        fa_run(t, 256, 40, true)
    }
}

//! the head-256 softcap stamps (gemma-2's shape, the Vulkan cell's cap of 50 over its wide scores): the capped CPU oracle, the
//! uncapped one the control the rows must miss
[test]
def test_cud_fa_h256_softcap(t0 : T?) {
    t0 |> run("CUDA cm2 flash tile at head 256 under a softcap of 50, f32 and f16 out == the capped CPU oracle, whole and under a 40-key window") <| @(t : T?) {
        if (!ensure_fa_cm2_h256_cap_cls() || !ensure_fa_cm2_h256_cap_f16_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        fa_run(t, 256, 0, false, false, 50.0)
        fa_run(t, 256, 40, false, false, 50.0)
    }
}

//! the flash tile at head 512: gemma-4's global layers on the window (V off the raw k row), the Vulkan cell's h512 arm
[test]
def test_cud_fa_h512(t0 : T?) {
    t0 |> run("CUDA flash tile at head 512 on the KHR arm, f32 and f16 out == the CPU attention oracle, whole and under a 40-key window") <| @(t : T?) {
        if (!ensure_fa_khr_h512_cls() || !ensure_fa_khr_h512_f16_cls()) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        fa_run(t, 512, 0)
        fa_run(t, 512, 40)
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

//! the fused q/k norm, rope and K/V store (`qkn_rope_cls`, or under `f16` its half-mirror twin) on one row, a head a workgroup, against `qkn_rope_rows`: four q heads in place, two k heads stored at mirror row `krow`, each copying its raw V row
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
    cuda_mem_free(fixed_array(pq, pkv, pkm, pvm, pcs, pwn, ptok))   // the bias and V-source bindings alias the cos and projection planes: each block freed once
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

//! the cm2 l tile at a llama-1B prefill window's shapes (512 rows by the projection and the FFN widths), event-timed
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

enum private DnStepArm {
    q8_out
    f32_out
    sig_f32_out
}

[test]
def test_cud_dn_family(t0 : T?) {   // nolint:STYLE038 — the family's four cells over two head sizes, the Vulkan cell's shape
    t0 |> run("CUDA deltanet kernels == the CPU oracles (conv, fused step, scan + gate, ring tail)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_dn_step_cls()) {
                t |> skip("no CUDA device with NVRTC")
                return
            }
            verify(ensure_dn_conv_cls() && ensure_dn_scan_cls() && ensure_dn_scan_p3_cls() && ensure_dn_scan_p3_sig_cls() && ensure_dn_step_sig_cls() && ensure_dn_tail_cls(),
                "the dn stamps must compile on a device that compiled the fused step")
            var s = g_cud.stream
            for (ds in fixed_array(64, 128)) {   // ds 64 keeps a state column in one row part of the fused step; 128, the served hybrids' head size, splits it in two and takes the scan's eight-lane clusters
                let nkh = 2
                let nvh = 4
                let kd = nkh * ds
                let cd = 2 * kd + nvh * ds
                let dconv = 4
                let taps = dconv - 1
                let eps = 0.00001
                let qscale = 1.25
                {
                    let npos = 37   // a second position block primed from window rows
                    let taps_off = 16
                    let hist_off = taps_off + cd * dconv
                    let sm_n = hist_off + cd * taps
                    var inscope srch <- row_fill(npos * cd, 19, 0, 0.03, 1.1, 79)
                    var inscope smh <- row_fill(sm_n, 11, 0, 0.02, 0.35, 37)
                    smh[0] = eps
                    smh[1] = qscale
                    var bufs = fixed_array(buf_upload(srch, s), buf_fill(npos * cd, Y_SENTINEL, s), buf_upload(smh, s))
                    var pc = GkDnConvArgs(npos = uint(npos), cd = uint(cd), kd = uint(kd), ds = uint(ds), dconv = uint(dconv), hist_mode = 1u,
                        taps_off = uint(taps_off), hist_off = uint(hist_off), eps = eps, qscale = qscale)
                    enc_dn_conv_cls(s, bufs, pc, dn_conv_wgs(int64(cd), int64(npos)))
                    t |> success(cud_sync(), "dn_conv at ds {ds}: the launch completes ({cuda_last_error()})")
                    var inscope o_cls <- buf_download(bufs[1], npos * cd, type<float>)
                    var inscope o_ref <- dn_conv_ref(srch, smh, hist_off, smh, taps_off, npos, cd, dconv, kd, ds, eps, qscale)
                    check_rows(t, "dn_conv at ds {ds} against the CPU oracle", o_cls, o_ref, BAR_APPROX_CTRL, cd)
                    cuda_mem_free(bufs)
                }
                {
                    let par = 1
                    let beta_off = 16
                    let g_off = beta_off + nvh
                    let a_off = g_off + nvh
                    let dt_off = a_off + nvh
                    let taps_off = dt_off + nvh
                    let wnorm_off = taps_off + cd * dconv
                    let hist_off = wnorm_off + ds
                    let sm_n = hist_off + 2 * cd * taps
                    let nblk = nvh * ds / 32
                    var inscope projh <- row_fill(cd + nvh * ds, 23, 0, 0.03, 1.0, 71)
                    var inscope sth <- row_fill(nvh * ds * ds, 13, 0, 0.02, 0.5, 53)
                    var inscope smh <- row_fill(sm_n, 17, 0, 0.02, 0.4, 43)
                    smh[0] = eps
                    smh[1] = qscale
                    for (i in range(nvh)) {
                        smh[a_off + i] = -0.4 - float(i) * 0.1   // a stays negative (decay)
                    }
                    var inscope ref <- dn_step_ref(projh, sth, smh, par, nvh, nkh, cd, ds, dconv, beta_off, g_off, a_off, dt_off, taps_off, wnorm_off, hist_off, eps, qscale)
                    var inscope q_ref : array<uint>
                    var inscope s_ref : array<float>
                    requant_oracle(ref.out, 0, nvh * ds, false, q_ref, s_ref)
                    var inscope tok <- [TokMeta(parity = uint(par), dnslot = 0u)]
                    for (arm in fixed_array(DnStepArm.q8_out, DnStepArm.f32_out, DnStepArm.sig_f32_out)) {
                        let bufs = fixed_array(buf_upload(projh, s), buf_upload(sth, s), buf_upload(smh, s), buf_fill(nblk, NAN_SENTINEL, s), buf_fill(nblk * 8, I8_SENTINEL_BITS, s), buf_upload(tok, s), buf_fill(nvh * ds, Y_SENTINEL, s))
                        var pc = DnStepArgs(cd = uint(cd), kd = uint(kd), nkh = uint(nkh), ds = uint(ds), dconv = uint(dconv),
                            beta_off = uint(beta_off), g_off = uint(g_off), taps_off = uint(taps_off), wnorm_off = uint(wnorm_off),
                            a_off = uint(a_off), dt_off = uint(dt_off), hist_off = uint(hist_off), eps = eps, qscale = qscale,
                            out_f32 = arm != DnStepArm.q8_out ? 1u : 0u, beta_row_stride = 0u, row0 = 0u)
                        if (arm == DnStepArm.sig_f32_out) {
                            enc_dn_step_sig_cls(s, bufs, pc, int64(nvh))
                        } else {
                            enc_dn_step_cls(s, bufs, pc, int64(nvh))
                        }
                        t |> success(cud_sync(), "dn_step arm {arm} at ds {ds}: the launch completes ({cuda_last_error()})")
                        var inscope state <- buf_download(bufs[1], nvh * ds * ds, type<float>)
                        var inscope smalls <- buf_download(bufs[2], sm_n, type<float>)
                        check_rows(t, "dn_step arm {arm} at ds {ds}: the state, advanced in place, against the CPU oracle", state, ref.state, BAR_APPROX_CTRL, ds)
                        check_rows(t, "dn_step arm {arm} at ds {ds}: the smalls plane (the conv ring advanced in place) against the CPU oracle", smalls, ref.smalls, BAR_APPROX_CTRL, FLAT_ROWS)
                        let st_at = maxdiff_at(ref.state, sth)   // where the oracle moved the state furthest: an in-place step that never ran leaves it there
                        t |> success(moved_off(state, sth, st_at, BAR_APPROX), "dn_step arm {arm} at ds {ds}: the state moved off its input at element {st_at}")
                        let sm_at = maxdiff_at(ref.smalls, smh)
                        t |> success(moved_off(smalls, smh, sm_at, BAR_APPROX), "dn_step arm {arm} at ds {ds}: the ring moved off its input at element {sm_at}")
                        if (arm == DnStepArm.q8_out) {
                            var inscope oscales <- buf_download(bufs[3], nblk, type<float>)
                            var inscope oquants <- buf_download(bufs[4], nblk * 8, type<uint>)
                            check_rows(t, "dn_step's Q8_0 o row's scales at ds {ds} against the CPU oracle (a row a block)", oscales, s_ref, BAR_APPROX_CTRL, 1)
                            check_rows(t, "dn_step's Q8_0 o row's quants at ds {ds} against the CPU oracle, a byte within one step (a row a block)", qbyte_values(oquants), qbyte_values(q_ref), QBYTE_BAR, 32)
                        } else {
                            var inscope of32 <- buf_download(bufs[6], nvh * ds, type<float>)
                            let want & = arm == DnStepArm.f32_out ? ref.out : ref.out_sig
                            check_rows(t, "dn_step arm {arm}'s o row at ds {ds} against the {arm == DnStepArm.sig_f32_out ? "sigmoid-gated" : "pre-quant"} oracle", of32, want, BAR_APPROX_CTRL, ds)
                            if (arm == DnStepArm.sig_f32_out) {
                                check_exceeds(t, "dn_step_sig at ds {ds} lands off the silu oracle (the two stamps differ)", of32, ref.out, BAR_APPROX)
                            }
                        }
                        cuda_mem_free(bufs)
                    }
                }
                {
                    let npos = 70
                    let di = nvh * ds
                    let beta_off = 16
                    let g_off = beta_off + npos * nvh
                    let a_off = g_off + npos * nvh
                    let dt_off = a_off + nvh
                    let wnorm_off = dt_off + nvh
                    let sm_n = wnorm_off + ds
                    var inscope convh : array<float>
                    convh |> resize(npos * cd)
                    for (i in range(npos * cd)) {   // unit-scale q/k rows, the conv stage's L2-normed shape: on unnormed rows the delta rule's recurrence overflows float range within the window at ds 128
                        convh[i] = (float((i * 19) % 79) * 0.025 - 0.9) * 0.15
                    }
                    var inscope zh <- row_fill(npos * di, 29, 0, 0.05, 1.5, 61)
                    var inscope sth <- row_fill(nvh * ds * ds, 13, 0, 0.02, 0.5, 53)
                    var inscope smh <- row_fill(sm_n, 17, 0, 0.03, 0.6, 43)
                    smh[0] = eps
                    for (i in range(nvh)) {
                        smh[a_off + i] = -0.4 - float(i) * 0.1
                    }
                    for (i in range(ds)) {
                        smh[wnorm_off + i] = 0.8 + float(i % 9) * 0.04
                    }
                    let cmp_n = npos * di + nvh * ds * ds
                    var inscope o_raw <- dn_delta_ref(convh, sth, smh, npos, nkh, nvh, ds, beta_off, g_off, a_off, dt_off)
                    var inscope o_ref <- dn_gate_ref(o_raw, zh, smh, npos, nvh, ds, wnorm_off, eps, false)
                    var inscope o_ref_sig <- dn_gate_ref(o_raw, zh, smh, npos, nvh, ds, wnorm_off, eps, true)   // the sigmoid out-gate's rows (the `dn_scan_p3_sig_cls` stamp's), the state part alike
                    var inscope st_ref <- copy_span(o_raw, npos * di, nvh * ds * ds)
                    let st_at = maxdiff_at(st_ref, sth)   // where the oracle moved the state furthest: a scan that never wrote its state leaves it there
                    let floor = 1e-2 * logits_maxabs(o_ref)
                    let scan_bar = Bar(rel = 1e-2, abs_floor = floor, poison_of_max = true, poison = 2.0, poison_abs = 1.0)
                    var inscope nocap <- [0u, 0u]
                    var pcs = DnScanArgs(npos = uint(npos), cd = uint(cd), kd = uint(kd), nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
                        beta_off = uint(beta_off), g_off = uint(g_off), a_off = uint(a_off), dt_off = uint(dt_off), o_off = 0u, di = uint(di))
                    var pc3 = GkDnGateArgs(nvh = uint(nvh), di = uint(di), ds = uint(ds), o_off = 0u, wnorm_off = uint(wnorm_off), eps = eps)
                    for (sig in fixed_array(false, true)) {
                        let bufs = fixed_array(buf_upload(convh, s), buf_upload(sth, s), buf_upload(smh, s), buf_fill(npos * di, Y_SENTINEL, s), buf_upload(nocap, s), buf_fill(16, 0.0, s), buf_upload(zh, s))
                        enc_dn_scan_cls(s, fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5]), pcs, dn_scan_wgs(int64(nvh), int64(ds)))
                        if (sig) {
                            enc_dn_scan_p3_sig_cls(s, fixed_array(bufs[6], bufs[3], bufs[2]), pc3, int64(npos))
                        } else {
                            enc_dn_scan_p3_cls(s, fixed_array(bufs[6], bufs[3], bufs[2]), pc3, int64(npos))
                        }
                        t |> success(cud_sync(), "dn_scan{sig ? " + sig" : ""} at ds {ds}: the launches complete ({cuda_last_error()})")
                        var inscope o <- buf_download(bufs[3], npos * di, type<float>)
                        var inscope st_got <- buf_download(bufs[1], nvh * ds * ds, type<float>)
                        t |> success(moved_off(st_got, sth, st_at, scan_bar), "dn_scan{sig ? " + sig" : ""} at ds {ds}: the state, written in place, moved off its input at element {st_at}")
                        o |> push_from(st_got)
                        let want & = sig ? o_ref_sig : o_ref
                        check_rows(t, "dn_scan + the {sig ? "sigmoid" : "silu"} out-norm at ds {ds} against the CPU oracle ({cmp_n} elements: the o rows, then the states)", o, want, scan_bar, ds)
                        if (sig) {
                            check_exceeds(t, "dn_scan_p3_sig at ds {ds} lands off the silu oracle (the two stamps differ)", o, o_ref, Bar(rel = 1e-2, abs_floor = floor))
                        } else {
                            var inscope poison := o   // the bar's control: one normed o value pushed past both bars must red
                            poison[0] += 1.0 + 2.0 * logits_maxabs(o_ref)
                            check_exceeds(t, "dn_scan + out-norm at ds {ds}: the bar against a poisoned value", poison, o_ref, Bar(rel = 1e-2, abs_floor = floor))
                        }
                        cuda_mem_free(bufs)
                    }
                }
                {
                    let hist_off = 16
                    let img = cd * taps
                    let sm_n = hist_off + 2 * img
                    for (rows in fixed_array(5, 2)) {
                        var inscope src <- row_fill(rows * cd, 7, 0, 0.05, 0.9, 59)
                        var inscope smh <- row_fill(sm_n, 5, 0, 0.04, 0.3, 41)
                        let row0 = max(rows - taps, 0)
                        let old_rows = max(taps - rows, 0)
                        var inscope want <- dn_tail_oracle(src, smh, cd, taps, row0, old_rows, hist_off, hist_off + img)
                        var bufs = fixed_array(buf_upload(src, s), buf_upload(smh, s))
                        var pct = DnTailArgs(cd = uint(cd), taps = uint(taps), row0 = uint(row0), hist_off = uint(hist_off + img), old_rows = uint(old_rows), old_off = uint(hist_off))
                        enc_dn_tail_cls(s, bufs, pct, int64((cd * taps + 255) / 256))
                        t |> success(cud_sync(), "dn_tail over {rows} rows at cd {cd}: the launch completes ({cuda_last_error()})")
                        var inscope got <- buf_download(bufs[1], sm_n, type<float>)
                        check_exact(t, "dn_tail over {rows} rows at cd {cd} against the per-channel CPU form (the entry image and the words between untouched)", got, want)
                        check_exceeds(t, "dn_tail over {rows} rows at cd {cd} wrote the other image", got, smh, BAR_EXACT)
                        cuda_mem_free(bufs)
                    }
                }
            }
        } else {
            t |> skip("dasVulkan not present: the requant oracle lives in its cells")
        }
    }
}

//! the router GEMM over two layers' [ner x dim] rows, the second layer's dispatched through woff, against the shared oracle
def private cud_router_arm(t : T?; ner : int) {
    var wh, xh : array<float>
    router_gemm_planes(ner, wh, xh)
    var s = g_cud.stream
    var bufs = fixed_array(buf_upload(wh, s), buf_upload(xh, s), buf_fill(MR_NPOS * ner, Y_SENTINEL, s))
    var pc = RouterGemmArgs(npos = uint(MR_NPOS), ne = uint(ner), dim = uint(MR_DIM), woff = uint(ner * MR_DIM))
    enc_router_gemm_cls(s, bufs, pc, router_gemm_wgs(int64(MR_NPOS), int64(ner)))
    t |> success(cud_sync(), "router GEMM ({ner} rows): the launch completes ({cuda_last_error()})")
    var got <- buf_download(bufs[2], MR_NPOS * ner, type<float>)
    cuda_mem_free(bufs)
    var want <- router_gemm_oracle(wh, xh, ner)
    router_gemm_check(t, ner, got, want)
    delete wh
    delete xh
    delete got
    delete want
}

//! the per-row select against the host select over the same rows
def private cud_topk_rows_arm(t : T?; norm : bool; wscale : float; gate : MoeGate; rbias : bool) {
    var lh, rb : array<float>
    topk_rows_planes(rbias, lh, rb)
    var s = g_cud.stream
    let pl = buf_upload(lh, s)
    // no down scale: the logits fill the ds binding, never read at GK_NO_DSCALE
    var bufs = fixed_array(pl, buf_fill(MR_NPOS * MR_K, Y_SENTINEL, s), pl, buf_upload(rb, s), buf_fill(MR_NPOS * MR_K, 0xFFFFFFFFu, s))
    var pc = GkTopKArgs(ne = uint(MR_NE), k = uint(MR_K), norm = norm || gate == MoeGate.softmax_weight ? 1u : 0u, lstride = uint(MR_NER), wscale = wscale, dsoff = GK_NO_DSCALE,
        rbias_on = rbias ? 1u : 0u, rboff = uint(MR_NE))
    cud_topk_rows_enc(int64(MR_NE), s, bufs, pc, int64(MR_NPOS))
    t |> success(cud_sync(), "per-row top-k: the launch completes ({cuda_last_error()})")
    var gw <- buf_download(bufs[1], MR_NPOS * MR_K, type<float>)
    var gidx <- buf_download(bufs[4], MR_NPOS * MR_K, type<uint>)
    cuda_mem_free(fixed_array(bufs[0], bufs[1], bufs[3], bufs[4]))
    topk_rows_verdict(t, gidx, gw, lh, rb, norm, wscale, gate, rbias)
    delete lh
    delete rb
    delete gw
    delete gidx
}

//! the wide select (`topk_rows2_cls`, the two-deep stamp past 256 experts) over `ne` experts at `k` picks a row, on the Vulkan
//! two-deep cell's planes, against the host select; under `depth1` the one-deep stamp over the same rows is the control that misses
//! the host select's picks past the first 256
def private cud_topk_wide_arm(t : T?; rbias : bool; ne, k : int; depth1 : bool = false) {
    let npos = 4
    let lstride = ne + 8
    var lh, rb : array<float>
    topk_wide_planes(ne, npos, lstride, rbias, lh, rb)
    var s = g_cud.stream
    let pl = buf_upload(lh, s)
    // no down scale: the logits fill the ds binding, never read at GK_NO_DSCALE
    var bufs = fixed_array(pl, buf_fill(npos * k, Y_SENTINEL, s), pl, buf_upload(rb, s), buf_fill(npos * k, 0xFFFFFFFFu, s))
    var pc = GkTopKArgs(ne = uint(ne), k = uint(k), norm = 1u, lstride = uint(lstride), wscale = 0.0, dsoff = GK_NO_DSCALE,
        rbias_on = rbias ? 1u : 0u, rboff = uint(ne))
    cud_topk_rows_enc(depth1 ? 1l : int64(ne), s, bufs, pc, int64(npos))   // the stamp the depth of `ne` names, or under `depth1` the one-deep one
    let tag = "per-row top-k over {ne} experts ({rbias ? "router bias" : "plain"}{depth1 ? ", the one-deep stamp" : ", the two-deep stamp"})"
    t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
    var gw <- buf_download(bufs[1], npos * k, type<float>)
    var gidx <- buf_download(bufs[4], npos * k, type<uint>)
    cuda_mem_free(fixed_array(bufs[0], bufs[1], bufs[3], bufs[4]))
    var sel <- topk_rows_select(MoeGate.softmax, lh, rb, npos, ne, k, lstride, ne, true, true, 0.0)
    if (depth1) {
        let off = mismatch_exact([for (g in gidx); int64(g)], sel.idx)
        t |> success(off > 0, "{tag} control: the stamp that reaches 256 misses the host select's picks past them ({off} of {npos * k} off)")
    } else {
        topk_rows_compare(t, tag, gidx, gw, sel)
    }
    delete sel
    delete gidx
    delete gw
    delete lh
    delete rb
}

//! the routed block's gather (`moe_gather_f16_cls` as halves, `moe_gather_f32_cls` as floats) over seven positions of three slots,
//! the slot map a permutation of the 21 bucket rows, into the window [r0, r0 + rows) at widths under one stride of the 256
//! threads, whole strides and a tail, against the CPU gather; the rows past the window keep their sentinel, and the CPU gather over
//! the window at 0 is the control that misses
def private cud_gather_arm(t : T?; f16 : bool; r0, rows : int) {
    let npos = 7
    let k = 3
    let nk = npos * k
    let slack = 2   // rows past the window the dispatch never writes
    var slot_idx <- [for (r in range(nk)); uint((r * 5) % nk)]   // 5 is coprime to 21: a permutation
    var s = g_cud.stream
    for (n in fixed_array(100, 256, 300)) {
        var x <- hash_steps(npos * n, 91u)
        let plane = (rows + slack) * n
        var pc = GatherArgs(n = uint(n), k = uint(k), r0 = uint(r0), r1 = uint(r0 + rows))
        var bufs = fixed_array(buf_upload(x, s), buf_upload(slot_idx, s), f16 ? buf_fill(plane / 2, HALF_NAN_PAIR, s) : buf_fill(plane, NAN_SENTINEL, s))
        if (f16) {
            enc_moe_gather_f16_cls(s, bufs, pc, int64(nk))
        } else {
            enc_moe_gather_f32_cls(s, bufs, pc, int64(nk))
        }
        let tag = "moe gather {f16 ? "f16" : "f32"} n {n}, window [{r0}, {r0 + rows})"
        t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
        var got <- cud_mirror_floats(bufs[2], plane, f16)
        cuda_mem_free(bufs)
        var want <- moe_gather_oracle(x, slot_idx, n, k, r0, r0 + rows, f16)
        var want0 <- moe_gather_oracle(x, slot_idx, n, k, 0, rows, f16)
        // within 1e-3 of the largest magnitude on the half form: a half's step at |x| < 1 is under 1e-3, the two conversions round
        // at most a step apart; the f32 form copies, bit for bit
        let bar = f16 ? Bar(kind = BarKind.rel_of_max, rel = 1e-3, poison_abs = 0.05, poison_at = 0) : BAR_EXACT
        check_rows(t, "{tag}: the window's rows against the CPU gather", copy_span(got, 0, rows * n), want, bar, n)
        check_exceeds(t, "{tag}: control - the CPU gather over the window at 0 reads other positions", copy_span(got, 0, rows * n), want0, bar)
        let off = kept_off(got, 0, rows * n, NAN_SENTINEL)
        t |> equal(off.bad, 0, "{tag}: the {slack} rows past the window keep their sentinel ({off.bad} elements moved, the first at {off.first})")
        delete x
        delete got
        delete want
        delete want0
    }
    delete slot_idx
}

//! the per-expert chain's by-row gather (`moe_gather_f16_row_cls`): window row b of [r0, r0 + rows) takes its grid entry's
//! position row as halves over seven positions of three slots, at widths under one 256-thread stride, whole strides and a tail;
//! the rows past the window keep their sentinel, and the oracle over the window at 0 is the control that misses
def private cud_gather_row_arm(t : T?; r0, rows : int) {
    let npos = 7
    let k = 3
    let nk = npos * k
    let slack = 2
    var entry_of_bucket_row <- gather_row_entries(nk)
    var s = g_cud.stream
    for (n in fixed_array(100, 256, 300)) {
        var x <- hash_steps(npos * n, 91u)
        let plane = (rows + slack) * n
        var pc = GatherArgs(n = uint(n), k = uint(k), r0 = uint(r0), r1 = uint(r0 + rows))
        var bufs = fixed_array(buf_upload(x, s), buf_upload(entry_of_bucket_row, s), buf_fill(plane / 2, HALF_NAN_PAIR, s))
        enc_moe_gather_f16_row_cls(s, bufs, pc, int64(rows))
        let tag = "moe gather row n {n}, window [{r0}, {r0 + rows})"
        t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
        var got <- cud_mirror_floats(bufs[2], plane, true)
        cuda_mem_free(bufs)
        gather_row_check(t, tag, copy_span(got, 0, rows * n), x, entry_of_bucket_row, n, k, r0, rows)
        let off = kept_off(got, 0, rows * n, NAN_SENTINEL)
        t |> equal(off.bad, 0, "{tag}: the {slack} rows past the window keep their sentinel ({off.bad} elements moved, the first at {off.first})")
        delete x
        delete got
    }
    delete entry_of_bucket_row
}

//! the per-expert chain's by-row combine (`combine_row_cls`): window row b adds its weighted down row onto its position's
//! accumulated row - the window [r0, r0 + rows) one row a position at alternating slots, a position no row reaches untouched -
//! at widths under one stride of the 256 threads, whole strides and a tail; the oracle over the window at 0 is the control that misses
def private cud_combine_row_arm(t : T?; r0, rows : int) {
    let npos = 7
    let k = 2
    let nk = npos * k
    var entry_of_bucket_row <- combine_row_entries(npos, k, r0, rows)
    var w_packed <- hash_steps(nk, 97u)   // a position's k weights in a row: the layout every dispatch ships
    var s = g_cud.stream
    for (n in fixed_array(100, 256, 300)) {
        var acc <- hash_steps(npos * n, 71u)
        var down <- hash_steps(rows * n, 83u)
        var pc = GkMoeCombArgs(n = uint(n), k = uint(k), r0 = uint(r0), r1 = uint(r0 + rows))
        var bufs = fixed_array(buf_upload(acc, s), buf_upload(w_packed, s), buf_upload(down, s), buf_upload(entry_of_bucket_row, s))
        enc_combine_row_cls(s, bufs, pc, int64(rows))
        let tag = "combine row n {n}, window [{r0}, {r0 + rows})"
        t |> success(cud_sync(), "{tag}: the launch completes ({cuda_last_error()})")
        var y <- buf_download(bufs[0], npos * n, type<float>)
        cuda_mem_free(bufs)
        combine_row_check(t, tag, y, acc, w_packed, down, entry_of_bucket_row, n, k, npos, r0, rows)
        delete acc
        delete down
        delete y
    }
    delete w_packed
    delete entry_of_bucket_row
}

//! the routed combine (`combine_cls`, the window form of `GkMoeCombine` the hot expert pool and the whole-layer rail dispatch): a
//! workgroup a position over its row, the slot window past 0 over slot rows 1 .. k adding the hit rows onto the host's sum under
//! their weights, a zero weight skipping its stale row - the Vulkan `test_vkd_hot_combine` fixture at widths under one stride of
//! the 256 threads, at it and past it (the stamp's own loop); the window at 0 starts the sum over and reads a row past the hits,
//! the control that misses
def private cud_combine_arm(t : T?) {
    let k = 5
    let nhit = 3
    var s = g_cud.stream
    var w <- [for (j in range(k)); j < nhit ? 0.05 + float(j) * 0.07 : 0.0]
    var ident1 <- [for (j in range(k)); uint(j + 1)]
    for (n in fixed_array(100, 256, 300)) {
        var miss <- hash_steps(n, 71u)
        var down <- hash_steps(k * n, 83u)
        for (i in range(nhit * n, k * n)) {
            down[i] = NAN_SENTINEL   // a miss's row is stale: no hit chain wrote it
        }
        let tag = "combine n {n}"
        var pc = GkMoeCombArgs(n = uint(n), k = uint(k), r0 = 1u, r1 = uint(k) + 1u)
        var pc0 = GkMoeCombArgs(n = uint(n), k = uint(k), r0 = 0u, r1 = uint(k))   // the window at 0 starts the sum: the host's row is lost
        var bufs = fixed_array(buf_upload(miss, s), buf_upload(ident1, s), buf_upload(w, s), buf_upload(down, s))
        var bufs0 = fixed_array(buf_upload(miss, s), buf_upload(ident1, s), buf_upload(w, s), buf_upload(down, s))
        enc_combine_cls(s, bufs, pc, 1l)
        enc_combine_cls(s, bufs0, pc0, 1l)
        t |> success(cud_sync(), "{tag}: the launches complete ({cuda_last_error()})")
        var y <- buf_download(bufs[0], n, type<float>)
        var y0 <- buf_download(bufs0[0], n, type<float>)
        cuda_mem_free(bufs)
        cuda_mem_free(bufs0)
        var want : array<float>
        want |> resize(n)
        for (i in range(n)) {
            var acc = miss[i]
            for (j in range(nhit)) {
                acc += w[j] * down[j * n + i]
            }
            want[i] = acc
        }
        // within 1e-5 of the largest magnitude: three multiply-adds per element against the f32 oracle in the same order
        let bar = Bar(kind = BarKind.rel_of_max, rel = 1e-5, abs_floor = 1e-6, poison_abs = 0.01, poison_at = n - 1)
        t |> equal(nan_count(y), 0, "{tag}: no stale row reached the sum - a zero weight skips its row, whose NaN a product would carry")
        check_rows(t, "{tag}: the hit rows add to the host's sum under their weights", y, want, bar, FLAT_ROWS)
        // the control's window at 0 reads the stale rows past the hits, which are NaN by construction - `check_count` reads a NaN as off
        let lost = check_count(y0, want, bar)
        t |> success(lost > n / 2, "{tag}: control - a slot window at 0 starts the sum over, loses the host's row and reads a row past the hits ({lost} of {n} off)")
        delete miss
        delete down
        delete y
        delete y0
        delete want
    }
    delete w
    delete ident1
}

//! the bucket schedule against its CPU twin, word for word over the whole planes (three empty experts, a sentinel fill); past 256 experts the 512-thread stamp
def private cud_sched_arm(t : T?; ne : int; pad : bool = false) {
    static_if (typeinfo builtin_module_exists(vulkan)) {   // the schedule oracle lives with the Vulkan cells
        let nk = 4 * MR_NPOS * MR_K
        let wt_gu = 3
        let wt_dn = 2
        let sh = sched_shape(nk, ne, wt_gu, wt_dn)
        var idx <- sched_cell_idx(nk, ne)
        let bases = fixed_array(1_000u, 96u, 2_000u, 96u, 3_000u, 48u)
        var winv, w1, w3, w2 : array<uint>
        sched_oracle(idx, ne, wt_gu, wt_dn, sh, bases, winv, w1, w3, w2, false, pad ? MMQ_ROW_PAD : 0, pad)
        var s = g_cud.stream
        let nw = length(w1)
        let row_pad = pad ? MMQ_ROW_PAD : 1
        let nrows = bucket_start(idx, ne, row_pad)   // the bucket rows, the pads among them
        var bufs = fixed_array(buf_upload(idx, s), buf_fill(nk, 0xFFFFFFFFu, s), buf_fill(nw, MR_SENT, s), buf_fill(nw, MR_SENT, s), buf_fill(nw, MR_SENT, s), buf_fill(nrows, 0xFFFFFFFFu, s))
        var pc = SchedArgs(nk = uint(nk), ne = uint(ne), wt_gu = uint(wt_gu), wt_dn = uint(wt_dn),
            map_off = uint(sh.map_off), map_off_mgu = uint(sh.map_off_mgu), map_off_mdn = uint(sh.map_off_mdn),
            bound_gu = uint(sh.bound_gu), bound_dn = uint(sh.bound_dn), bound_mgu = uint(sh.bound_mgu), bound_mdn = uint(sh.bound_mdn),
            base1 = bases[0], stride1 = bases[1], base3 = bases[2], stride3 = bases[3], base2 = bases[4], stride2 = bases[5], s_only = 0u)
        cud_moe_sched_enc(int64(ne), pad, s, bufs, pc)
        let pad_tag = pad ? ", padded to {MMQ_ROW_PAD}-row tiles on the m column" : ""
        t |> success(cud_sync(), "bucket schedule ({ne} experts{pad_tag}): the launch completes ({cuda_last_error()})")
        var ginv <- buf_download(bufs[1], nk, type<uint>)
        var g1 <- buf_download(bufs[2], nw, type<uint>)
        var g3 <- buf_download(bufs[3], nw, type<uint>)
        var g2 <- buf_download(bufs[4], nw, type<uint>)
        var gbex <- buf_download(bufs[5], nrows, type<uint>)
        cuda_mem_free(bufs)
        sched_maps_check(t, idx, ginv, gbex) $(e) => (start = bucket_start(idx, e, row_pad), rows = bucket_rows(idx, e))
        check_exact(t, "the gate plane's s and m records, maps and sentinel tails against the CPU twin, word for word ({nw} words, {ne} experts{pad_tag})", g1, w1)
        check_exact(t, "the up plane's schedule against the CPU twin ({ne} experts{pad_tag})", g3, w3)
        check_exact(t, "the down plane's schedule against the CPU twin (its own tile counts and tails, {ne} experts{pad_tag})", g2, w2)
        w2[sh.map_off_mdn + sh.bound_mdn - 1] = 0u
        t |> success(mismatch_exact(g2, w2) > 0, "schedule control ({ne} experts): the last map's last sentinel word flipped reds the compare")
        delete idx
        delete winv
        delete w1
        delete w3
        delete w2
        delete ginv
        delete g1
        delete g3
        delete g2
        delete gbex
    } else {
        t |> skip("dasVulkan not present: the bucket schedule's CPU twin lives with the Vulkan cells, so the {ne}-expert{pad ? " padded" : ""} schedule arm does not run")
    }
}

//! the combine-folded residual step (`ArCombBase`, the f32 normed-row form the home stamps) against the shared oracle (`ar_comb_oracle`)
def private cud_ar_comb_arm(t : T?; gated, with_sh, dbias : bool) {
    let n = 64
    let k = dbias ? 12 : 3   // twelve slots on the biased arm: the row pass's eight-slot register form and its tail past them
    let npos = 5
    let ne = 6
    let eps = 1.0e-6
    var fx <- ar_comb_fixture(n, k, npos)
    var wn, lg, db : array<float>
    var sex : array<uint>
    ar_comb_planes(n, k, npos, ne, dbias, wn, lg, db, sex)
    var s = g_cud.stream
    var bufs = fixed_array(buf_upload(fx.xs, s), buf_upload(fx.sh, s), buf_upload(wn, s), buf_fill(npos * n, Y_SENTINEL, s), buf_upload(fx.sidx, s),
        buf_upload(fx.sw, s), buf_upload(fx.rows, s), buf_upload(lg, s), buf_upload(db, s), buf_upload(sex, s))
    var pc = GkAddRmsArgs(dim = uint(n), add_on = with_sh ? 1u : 0u, woff = 0u, eps = eps, ascale = 1.0, row0 = 0u,
        slots = uint(k), lstride = uint(MR_NER), gated = gated ? 1u : 0u, dbias_on = dbias ? 1u : 0u, dboff = dbias ? uint(ne * n) : 0u)
    enc_cls_ar_comb(s, bufs, pc, int64(npos))
    let arm = (with_sh ? (gated ? "gated shared" : "ungated shared") : "no shared") + " expert" + (dbias ? ", down biases" : "")
    t |> success(cud_sync(), "the combine-folded residual step ({arm}): the launch completes ({cuda_last_error()})")
    var got_x <- buf_download(bufs[0], npos * n, type<float>)
    var got_y <- buf_download(bufs[3], npos * n, type<float>)
    cuda_mem_free(bufs)
    var want_x, want_y : array<float>
    let got_h : array<uint>   // no f16 twin on this home: the CUDA stamp is the f32 form alone
    ar_comb_oracle(fx, wn, lg, db, sex, n, k, npos, ne, gated, with_sh, dbias, eps, want_x, want_y)
    ar_comb_check(t, "the combine-folded residual step ({arm})", "combine-folded residual control", fx.xs, got_x, got_y, got_h, false, want_x, want_y, n)
    delete fx
    delete wn
    delete lg
    delete db
    delete sex
    delete got_x
    delete got_y
    delete want_x
    delete want_y
}

[test]
def test_cud_moe_routing(t0 : T?) {
    t0 |> run("CUDA routed-block routing kernels == the CPU oracles: router GEMM, per-row top-k (the one-deep stamp, and the two-deep one over 512 experts), the bucket schedule, the gather as halves and as floats, the per-expert chain's by-row gather and combine, the combine-folded residual step") <| @(t : T?) {
        if (!(ensure_router_gemm_cls() && ensure_topk_rows_cls() && ensure_moe_sched_cls() && ensure_moe_sched2_cls() && ensure_moe_sched_mmq_cls() && ensure_moe_sched2_mmq_cls() && ensure_cls_ar_comb())) {
            t |> skip("no CUDA device with NVRTC")
            return
        }
        verify(cud_topk_rows_ensure(512l) && ensure_moe_gather_f16_cls() && ensure_moe_gather_f32_cls() && ensure_moe_gather_f16_row_cls() && ensure_combine_row_cls() && ensure_combine_cls(),
            "the two-deep select, the gathers, the routed combine and the per-expert chain's by-row pair must compile on a device that compiled the one-deep select")
        cud_router_arm(t, MR_NER)
        cud_router_arm(t, 9)   // fewer rows than the tile's half: the second row of every invocation is guarded off
        cud_topk_rows_arm(t, true, 0.0, MoeGate.softmax, false)
        cud_topk_rows_arm(t, false, 2.5, MoeGate.softmax, false)
        cud_topk_rows_arm(t, false, 0.0, MoeGate.softmax_weight, true)   // gpt-oss: the biased pick, softmax over the picks
        cud_topk_wide_arm(t, false, 512, 10)   // Qwen3-Next's 512 experts at ten picks a row
        cud_topk_wide_arm(t, true, 512, 10)
        cud_topk_wide_arm(t, false, 512, 10, true)
        cud_gather_arm(t, true, 5, 9)   // the window [5, 14) of the 21 bucket rows
        cud_gather_arm(t, false, 5, 9)
        cud_gather_row_arm(t, 5, 9)   // the per-expert chain's by-row pair over the same window
        cud_combine_row_arm(t, 4, 6)
        cud_combine_arm(t)
        cud_sched_arm(t, MR_NE)
        cud_sched_arm(t, 512)   // past 256 experts: the 512-thread stamp, experts past 256 routed
        cud_sched_arm(t, MR_NE, true)   // the MMQ form: every bucket's rows from a 128-multiple, all on the m column
        cud_sched_arm(t, 512, true)   // the 512-thread MMQ stamp: the pad and the scan past 256 experts
        cud_ar_comb_arm(t, true, true, false)
        cud_ar_comb_arm(t, false, true, false)
        cud_ar_comb_arm(t, false, false, false)
        cud_ar_comb_arm(t, true, true, true)
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

var private g_shutdown_hook_ran = 0   //! the leak gate's hook: how many times the shutdown ran it

def private shutdown_hook() {
    g_shutdown_hook_ran++
}

//! the leak gate: the registered shutdown hooks run ahead of the device (a module's device state goes with it), every device buffer
//! freed, every module released at the shutdown, nothing counted live
[test]
def test_cud_shutdown_leaks(t0 : T?) {
    t0 |> run("the CUDA tier shuts down with nothing live, its registered hooks run") <| @(t : T?) {
        let up = cud_init()
        cud_on_shutdown(@@shutdown_hook)
        cud_shutdown()
        t |> equal(g_shutdown_hook_ran, up ? 1 : 0, "the shutdown ran the registered hook exactly once (device up: {up})")
        t |> equal(cuda_log_leaks_if_any(), 0l, "no CUDA object or allocation outlives the tier")
    }
}
