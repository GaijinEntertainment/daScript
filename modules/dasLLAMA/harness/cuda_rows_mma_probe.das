options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

// The rows tensor leaf against the N-column GEMV leaf on the CUDA home: one plane of hashed words at [n x d] in --fmt, both
// leaves over the same planes at each column count of --cols - the widest difference of their rows as a share of the largest
// output, and each leaf's wall a dispatch (the N-column leaf tiles past eight columns, so its figure there is two passes).
//
//   DASLLAMA_GPU=1 DASLLAMA_CUDA=1 bin/daslang -jit modules/dasLLAMA/harness/cuda_rows_mma_probe.das -- [--n 5120] [--d 17408] [--cols 1,2,4,8,16]

require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_classes_rows   // the rows tensor leaf's family
require dasllama/dasllama_gpu_kernels_discrete   // GemvArgs
require dasllama/dasllama_kqformat
require daslib/f16_cvt
require daslib/math_bits
require dasllama/dasllama_math   // reserve_resize
require daslib/clargs
require _cuda_time   // cuda_time_best
require _prompt_files   // id_list
require math

[CommandLineArgs]
struct Args {
    @clarg_doc = "The reduction width: a multiple of 256, or of 32 on a q8 plane"
    n : int = 5120
    @clarg_doc = "The weight rows"
    d : int = 17408
    @clarg_doc = "The weight format, a KqFmt member with a rows tensor leaf: q8, k4, k5, k6, nv4, q40, iq4nl, iq4xs, iq3s4"
    fmt : string = "k4"
    @clarg_doc = "The feed written by the requant's own launch (the `OUT_FEED` stamps) instead of the re-lay"
    feedrq : bool
    @clarg_doc = "Column counts, comma-separated, 1 to 16"
    cols : string = "1,2,4,8,16"
    @clarg_doc = "Dispatches a timed chain"
    reps : int = 40
    @clarg_doc = "Copies of the weight planes a chain walks in turn: past the card's cache the dispatches stream from memory, as a decode step's do"
    ring : int = 12
    @clarg_short = "h"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private hash32(i : uint) : uint {
    var x = i * 0x9E3779B9u + 0x7F4A7C15u
    x ^= x >> 16u
    x *= 0x85EBCA6Bu
    x ^= x >> 13u
    x *= 0xC2B2AE35u
    x ^= x >> 16u
    return x
}

[export, cold_path]
def main : int {   // nolint:STYLE037,STYLE038 — one linear measurement sweep, a leg a column count
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help) {
        print_help(get_command_info(type<Args>), "cuda_rows_mma_probe")
        return 0
    }
    if (!cud_init()) {
        print("no CUDA device\n")
        return 1
    }
    var failed <- cud_ensure_all()   // every stamp of the home: the N-column leaves among them
    if (!empty(failed)) {
        print("kernels refused: {failed}\n")
        return 1
    }
    delete failed
    let n = a.n
    let d = a.d
    let nsb = (n + 255) / 256   // a Q8_0 row is whole 32-blocks: the planes size to its last, short superblock
    let maxc = 16
    var fmt = -1
    for (f in type<KqFmt>) {
        if ("{f}" == a.fmt) {
            fmt = int(f)
        }
    }
    if (fmt < 0 || !cud_rows_mma_stamped(fmt)) {
        print("--fmt {a.fmt}: no rows tensor leaf\n")
        return 2
    }
    let kf = kq_fmt_of_slot(fmt)
    let q8 = kf == KqFmt.q8
    let nv4 = kf == KqFmt.nv4
    let k6 = kf == KqFmt.k6
    let d16 = kf == KqFmt.q40 || kf == KqFmt.iq4nl      // an f16 d a block, two a word
    let two = kf == KqFmt.iq4xs || kf == KqFmt.iq3s4    // a two-word scale row: d | the sub-scales' high bits, their nibbles
    var wq : array<uint>
    wq |> reserve_resize(int64(d) * int64(nsb) * (q8 ? 64l : kq_qsb(kf) / 4l))   // a superblock: 256 quant bytes, or the format's quant bytes
    for (i in range64(long_length(wq))) {
        wq[i] = hash32(uint(i))
    }
    var ws : array<uint>
    if (q8) {   // an f16 scale a 32-block, two a word
        ws |> reserve_resize(int64(d) * int64(nsb) * 4l)
        for (r in range64(long_length(ws))) {
            let h = hash32(uint(r) + 0x51u)
            ws[r] = f32_to_f16(0.002 + float(h & 255u) * 0.00002) | (f32_to_f16(0.002 + float((h >> 8u) & 255u) * 0.00002) << 16u)
        }
    } elif (k6) {   // sixteen signed sub-scales a superblock, then its f16 d
        ws |> reserve_resize(int64(d) * int64(nsb) * 5l)
        for (r in range64(int64(d) * int64(nsb))) {
            for (j in range64(4l)) {
                ws[r * 5l + j] = hash32(uint(r * 5l + j))
            }
            ws[r * 5l + 4l] = f32_to_f16(0.0001 + float(hash32(uint(r) + 0x51u) & 255u) * 0.000001)
        }
    } elif (d16 || two) {
        let rw = two ? 2l : 5l
        ws |> reserve_resize(int64(d) * int64(nsb) * rw)
        for (r in range64(long_length(ws))) {
            let h = hash32(uint(r) + 0x51u)
            let lo = f32_to_f16(0.002 + float(h & 255u) * 0.00002)
            ws[r] = two ? ((r & 1l) == 0l ? lo | (h & 0xFFFF0000u) : h) : lo | (f32_to_f16(0.002 + float((h >> 8u) & 255u) * 0.00002) << 16u)
        }
    } elif (nv4) {   // sixteen UE4M3 strip scales a superblock, then the plane's factor
        ws |> reserve_resize(int64(d) * int64(nsb) * 5l)
        for (r in range64(int64(d) * int64(nsb))) {
            for (j in range64(4l)) {
                ws[r * 5l + j] = (hash32(uint(r * 5l + j)) & 0x0F0F0F0Fu) | 0x30303030u   // a narrow band of exponents: the rows' sums stay comparable
            }
            ws[r * 5l + 4l] = float_bits_to_uint(0.01)
        }
    } else {
        ws |> reserve_resize(int64(d) * int64(nsb) * 5l)
        for (r in range64(int64(d) * int64(nsb))) {
            let h = hash32(uint(r) + 0x51u)
            let dd = 0.002 + float(h & 255u) * 0.00002
            let dmin = 0.001 + float((h >> 8u) & 255u) * 0.00001
            ws[r * 5l] = f32_to_f16(dd) | (f32_to_f16(dmin) << 16u)
            for (j in range64(1l, 5l)) {
                ws[r * 5l + j] = hash32(uint(r * 5l + j)) & 0x3F3F3F3Fu   // the 6-bit scales and mins
            }
        }
    }
    var xf : array<float>   // the sixteen activation rows as floats: both feeds quantize them on the device
    xf |> reserve_resize(int64(maxc) * int64(n))
    for (i in range64(long_length(xf))) {
        xf[i] = (float(hash32(uint(i) + 0xA5A5u) & 0xFFFFu) / 32768.0 - 1.0) * (0.5 + float(hash32(uint(i / 256l) + 0x77u) & 255u) * 0.01)
    }
    var s = g_cud.stream
    let dwq = buf_upload(wq, s)
    let dws = buf_upload(ws, s)
    var rwq <- [for (_c in range(max(a.ring, 1))); buf_upload(wq, s)]
    var rws <- [for (_c in range(max(a.ring, 1))); buf_upload(ws, s)]
    let nring = length(rwq)
    let dregs = buf_upload([0u, 0u], s)
    let dxf = buf_upload(xf, s)
    let dxq = cuda_mem_alloc(uint64(maxc * n))            // the N-column leaf's feed: Q8_K, or Q8_0 under a q8 plane
    let dxs = cuda_mem_alloc(uint64(maxc * nsb * 8 * 4))
    let fq = cuda_mem_alloc(uint64(maxc * n))             // the rows tensor leaf's feed
    let fs = cuda_mem_alloc(uint64(nsb * 8 * 16 * 4))
    let fm = cuda_mem_alloc(uint64(nsb * 8 * 16 * 4))
    let ybytes = uint64(maxc * d * 4)
    let y1 = cuda_mem_alloc(ybytes)
    let y2 = cuda_mem_alloc(ybytes)
    var h1, h2 : array<float>
    h1 |> reserve_resize(int64(maxc * d))
    h2 |> reserve_resize(int64(maxc * d))
    print("{a.fmt} plane [n {n} x d {d}], {double(long_length(wq)) * 4.0lf / 1048576.0lf:.1f} MB of quants\n")
    for (nc64 in id_list(a.cols, 1l, int64(maxc))) {
        let nc = int(nc64)
        var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u, ncols = uint(nc), ystride = uint(d))
        cuda_memset_d8(y1, 0u8, ybytes, s)
        cuda_memset_d8(y2, 0u8, ybytes, s)
        // the two feeds off the same floats: the N-column leaf's requant, then the tensor leaf's re-laid off the requant's rows
        var pcq = RqArgs(inbase = 0u, nblk = uint(nc * n / (q8 ? 32 : 256)), lo = 0.0, hi = 0.0)
        let feeds = $ {
            if (a.feedrq) {
                pcq.feed_units = uint(n / (q8 ? 32 : 256))
                if (q8) {
                    enc_cls_dn_rq_feed(s, fixed_array(dxf, dxq, dxs, fq, fs, fm), pcq, rq_wgs(false, int64(nc * n)))
                } else {
                    enc_cls_q8k_rq_feed(s, fixed_array(dxf, dxq, dxs, fq, fs, fm), pcq, rq_wgs(true, int64(nc * n)))
                }
            } else {
                if (q8) {
                    enc_cls_dn_rq(s, fixed_array(dxf, dxq, dxs), pcq, rq_wgs(false, int64(nc * n)))
                } else {
                    enc_cls_q8k_rq(s, fixed_array(dxf, dxq, dxs), pcq, rq_wgs(true, int64(nc * n)))
                }
                cud_rows_mma_relay_enc(fmt, s, fixed_array(dxq, dxs, fq, fs, fm), int64(n), uint(nc))
            }
        }
        let leaf = $(wqb, wsb : uint64) {
            cud_rows_mma_enc(fmt, s, fixed_array(wqb, wsb, dregs, fq, fs, y2, fm), pc, rows_mma_wgs(int64(d)))
        }
        invoke(feeds)
        cud_gemv_enc(fmt, s, fixed_array(dwq, dws, dregs, dxq, dxs, y1), pc, int64(d))
        invoke(leaf, dwq, dws)
        cuda_memcpy_dtoh(unsafe(addr<void?>(h1[0])), y1, uint64(nc * d * 4), s)
        cuda_memcpy_dtoh(unsafe(addr<void?>(h2[0])), y2, uint64(nc * d * 4), s)
        cud_sync()
        var mag = 0.0
        var dmax = 0.0
        for (i in range(nc * d)) {
            mag = max(mag, abs(h1[i]))
            dmax = max(dmax, abs(h1[i] - h2[i]))
        }
        let us_gemv = cuda_time_best(a.reps) $(i) {
            cud_gemv_enc(fmt, s, fixed_array(rwq[i % nring], rws[i % nring], dregs, dxq, dxs, y1), pc, int64(d))
        }
        let us_mma = cuda_time_best(a.reps) $(i) {
            invoke(leaf, rwq[i % nring], rws[i % nring])
        }
        let us_feed = cuda_time_best(a.reps) $(_i) {
            invoke(feeds)
        }
        print("cols {nc}: gemv {us_gemv:.1f} us, rows tensor {us_mma:.1f} us ({us_mma / us_gemv:.2f} of the gemv) over a ring of {nring}; both feeds {us_feed:.1f} us; widest difference {dmax / max(mag, 1.0e-30) * 100.0:.4f}% of the largest output\n")
    }
    for (b in rwq) {
        cuda_mem_free(b)
    }
    for (b in rws) {
        cuda_mem_free(b)
    }
    delete rwq
    delete rws
    cuda_mem_free(dwq); cuda_mem_free(dws); cuda_mem_free(dregs); cuda_mem_free(dxq); cuda_mem_free(dxs); cuda_mem_free(y1); cuda_mem_free(y2)
    cuda_mem_free(dxf); cuda_mem_free(fq); cuda_mem_free(fs); cuda_mem_free(fm)
    delete wq
    delete ws
    delete xf
    delete h1
    delete h2
    return 0
}
