options gen2
options _dasllama_internal = true
options stack = 524288

// The window chain's tiles on both discrete homes of one card, the same planes: the Vulkan class kernel through the hazard
// rail and the CUDA class kernel through its launch rail - the Q8_0 cm2 l and m tiles (the window's batch GEMM) at a
// llama-1B prefill window's shapes (512 rows by the projection and FFN widths; the llama-8B's under `--only ...,8b`), the
// superblock cm2 tiles (the K-quants and the three 4-bit lattices, NVFP4 among them), the int8 MMQ leaves (Q8_0, Q4_K, IQ4_XS,
// Q4_0, IQ4_NL and the four per-32 formats) and the FP4 rung's nv4 leaf at the same shapes, the cm2 flash tile at head 64
// over the same window (32 heads, kv width 512) and the decode
// attention's split and unsplit forms - the time a dispatch and the rate it makes.
// Baseline: each MMQ arm against its CPU reference under its leaf's bound (per element, or relative to the region's largest
// magnitude for the min-term leaves). Compare region: 64 sampled outputs, token (s x 7919) % 512
// and row (s x 104729 + 13) % d for s < 64. Every Vulkan and CUDA tile, flash and decode-attention arm is timing-only.
// The arms of a race run in turn, one round of each (a round: one submit of DISPATCHES dispatches); each arm drops 2 warmup
// rounds, and every number is its best round after them, in microseconds a dispatch.
//   DASLLAMA_GPU=1 daslang -jit modules/dasLLAMA/harness/gemm_cuda_vs_vulkan.das

require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_gpu_tier
require ../tests/_vk_kq_fixtures.das
require ../tests/_fp4_feed.das   // fp4_feed_rows, fp4_feed_value
require ../tests/_mmq_ref.das   // the MMQ leaves' CPU references, the kernel cells' own
require _cuda_probe_fixture
require daslib/strings_boost
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema

let ROUNDS = 10
let WARMUPS = 2
let DISPATCHES = 20
let MMQ_REL_BOUND = 1.0e-3   //! the MMQ kernel cell's bar (`test_cud_q8_mmq`): relative to the larger magnitude, floored at 1
let MIN_TERM_MMQ_REL_BOUND = 2.0e-3   //! the min-term leaves' (Q4_K, Q5_1; `test_cud_k4_mmq`, the f16 tiles' `TILE_BAR`): relative to the compare region's largest magnitude
let ROWS = 512
let FA_HEADS = 32
let FA_HS = 64
let FA_KVD = 512
let FA_QD = FA_HEADS * FA_HS

struct Planes {
    wq : array<uint>
    ws : array<uint>
    sched : array<uint>
    xf : array<uint>
    wgs : int
}

def planes(k, d, tc : int) : Planes {
    var p : Planes
    q8_plane(d * (k / 32), 1u, 17u, 1u, 991u, p.wq, p.ws)
    p.xf <- ws_plane((ROWS + int(TILE_READ_SLACK)) * k / 2, 3u, 5u)
    var sr <- two_region_sched(ROWS, 0, 0, tc, (d + 127) / 128, 0)
    p.sched <- sr.words
    p.wgs = sr.wgs
    return <- p
}

def tflops(k, d : int; us : double) : double => 2.0lf * double(ROWS) * double(k) * double(d) / us / 1.0e6lf

//! why a cm2 stamp's Vulkan arm refuses, "" when it runs; `armed` is the stamp's ensure, which arms the device the mode is
//! read off
def cm2_why(armed : bool) : string {
    return VK_CLASS_REFUSED if (!armed)
    return "no cm2 device" if (g_gpu == null || g_gpu.coopmat_mode != COOPMAT_CM2)
    return ""
}

def cu_why(armed : bool) : string => armed ? "" : CU_CLASS_REFUSED

//! one race of a tile over `p`'s planes: the Vulkan arm unless `vk_why` refuses it, its dispatches over the descriptor set
//! `mk_set` binds and `vk_enc` encodes from, and the CUDA arm unless `cu_why_` refuses it, its launches over the launch set
//! `cu_enc` launches from
def race_tile(p : Planes; d : int; vk_why, cu_why_ : string;
              mk_set : block<(bufs : uint64 const[5]; sizes : int64 const[5]; bits : uint const[5]) : VkDescriptorSet>;
              vk_enc : block<(cmd : VkCommandBuffer; var hz : VkHaz; var set_ : VkDescriptorSet) : void>;
              cu_enc : block<(bufs : uint64 const[5]) : void>) : RaceRow {
    var vbufs : uint64[5]
    var raw : VkCommandBuffer
    if (vk_why == "") {
        vbufs = fixed_array(dev_upload(p.wq), dev_upload(p.ws), dev_upload(p.sched), dev_upload(p.xf), make_device_buf(int64(ROWS) * int64(d) * 4l))
        let sizes = fixed_array(long_length(p.wq) * 4l, long_length(p.ws) * 4l, long_length(p.sched) * 4l, long_length(p.xf) * 4l, int64(ROWS) * int64(d) * 4l)
        var set_ = invoke(mk_set, vbufs, sizes, fixed_array(0u, 0u, 0u, 0u, 1u))
        raw = record_arm() $(cmd : VkCommandBuffer; var hz : VkHaz) {
            for (_i in range(DISPATCHES)) {
                invoke(vk_enc, cmd, hz, set_)
            }
        }
    }
    var cbufs : uint64[5]
    if (cu_why_ == "") {
        let s = g_cud.stream
        cbufs = fixed_array(buf_upload(p.wq, s), buf_upload(p.ws, s), buf_upload(p.sched, s), buf_upload(p.xf, s), buf_fill(ROWS * d, 0.0, s))
    }
    let row = race(ROUNDS, WARMUPS, DISPATCHES, vk_why, raw, cu_why_) $(_i) {
        invoke(cu_enc, cbufs)
    }
    if (vk_why == "") {
        for (b in vbufs) {
            destroy_device_buf(b)
        }
    }
    if (cu_why_ == "") {
        cuda_mem_free(cbufs)
    }
    return row
}

//! the Q8_0 tile race at the column `tc` (256: the l tile, 128: the m tile)
def race_q8_tile(p : Planes; k, d, tc : int) : RaceRow {
    let l = tc == 256
    var pc = BatchArgs(n = uint(k), d = uint(d), map_off = uint(2 * 4))
    let wgs = int64(p.wgs)
    let vk_why = cm2_why(l ? dasllama_vulkan_classes::ensure_q8_batch_cm2l_cls() : dasllama_vulkan_classes::ensure_q8_batch_cm2m_cls())
    let cuw = cu_why(l ? dasllama_cuda_classes::ensure_q8_batch_cm2l_cls() : dasllama_cuda_classes::ensure_q8_batch_cm2m_cls())
    return race_tile(p, d, vk_why, cuw,
        $(bufs; sizes; bits) { return l ? set_q8_batch_cm2l_cls(bufs, sizes, bits) : set_q8_batch_cm2m_cls(bufs, sizes, bits); },
        $(cmd : VkCommandBuffer; var hz : VkHaz; var set_ : VkDescriptorSet) {
            if (l) {
                dasllama_vulkan_classes::enc_q8_batch_cm2l_cls(cmd, hz, set_, pc, wgs)
            } else {
                dasllama_vulkan_classes::enc_q8_batch_cm2m_cls(cmd, hz, set_, pc, wgs)
            }
        },
        $(bufs) {
            if (l) {
                dasllama_cuda_classes::enc_q8_batch_cm2l_cls(g_cud.stream, bufs, pc, wgs)
            } else {
                dasllama_cuda_classes::enc_q8_batch_cm2m_cls(g_cud.stream, bufs, pc, wgs)
            }
        })
}

//! a K-quant plane pair of `d` rows of `k` elements in the device layout, hashed, with the f16 activation panel and the schedule
def kq_tile_planes(fmt, k, d, tc : int) : Planes {
    var p : Planes
    let nsb = k / 256
    p.wq <- hashed_words(d * nsb * int(kq_qsb(kq_fmt_of_slot(fmt)) / 4l), 1u, uint(fmt) * 77777u)
    p.ws <- kq_ws_plane(fmt, d * nsb * int(kq_dev_ssb_i(fmt) / 4l), 1u, uint(fmt) * 13u)
    p.xf <- ws_plane((ROWS + int(TILE_READ_SLACK)) * k / 2, 3u, 5u)
    var sr <- two_region_sched(ROWS, 0, 0, tc, (d + 127) / 128, 0)
    p.sched <- sr.words
    p.wgs = sr.wgs
    return <- p
}

//! a per-32 plane pair of `d` rows of `k` elements (the format's quant words a block, a scale word a block or a half of one), hashed,
//! with the f16 activation panel and the column's schedule
def b32_tile_planes(fmt, k, d, tc : int) : Planes {
    var p : Planes
    let nb = k / 32
    let f = kq_fmt_of_slot(fmt)
    p.wq <- hashed_words(d * nb * int(kq_qsb(f) / 4l), 1u, uint(fmt) * 77777u)
    p.ws <- ws_plane((d * nb * int(kq_ssb(f)) + 3) / 4, 1u, uint(fmt) * 13u)
    p.xf <- ws_plane((ROWS + int(TILE_READ_SLACK)) * k / 2, 3u, 5u)
    var sr <- two_region_sched(ROWS, 0, 0, tc, (d + 127) / 128, 0)
    p.sched <- sr.words
    p.wgs = sr.wgs
    return <- p
}

//! a K-quant or per-32 tile race at the column `tc` (256: the l tile, 128: the m tile, 32: the s tile)
def race_kq_tile(fmt : int; p : Planes; k, d, tc : int) : RaceRow {
    let tci = tc == 256 ? 0 : (tc == 128 ? 1 : 2)
    var pc = BatchArgs(n = uint(k), d = uint(d), map_off = uint(2 * 4))
    let wgs = int64(p.wgs)
    let vk_why = cm2_why(cm2_cls_ensure(fmt, int64(tc)))
    let cuw = cu_why(cud_cm2_ensure(fmt, tci))
    return race_tile(p, d, vk_why, cuw,
        $(bufs; sizes; bits) { return cm2_cls_set(fmt, int64(tc), bufs, sizes, bits); },
        $(cmd : VkCommandBuffer; var hz : VkHaz; var set_ : VkDescriptorSet) { cm2_cls_enc(fmt, int64(tc), cmd, hz, set_, pc, wgs); },
        $(bufs) { cud_cm2_enc(fmt, tci, g_cud.stream, bufs, pc, wgs); })
}

struct MmqPlanes {
    xq : array<uint>    //! the activation quants, a byte an element, [token][k]
    xs : array<float>   //! a scale a 32-block a token
    xsum : array<float> //! a block's scale x the sum of its quants (a min-term leaf's feed: Q4_K, Q5_1)
}

def xf_at(p : Planes; e : int) : float {
    let v = unpackHalf2x16(p.xf[e / 2])
    return (e & 1) == 0 ? v.x : v.y
}

//! the int8 activation panel off the f16 one: a 32-block's scale is its largest magnitude over 127, the quants rounded to it
def mmq_planes(p : Planes; k : int) : MmqPlanes {
    var m : MmqPlanes
    let nb = k / 32
    m.xq |> resize(ROWS * k / 4)
    m.xs |> resize(ROWS * nb)
    m.xsum |> resize(ROWS * nb)
    for (tok in range(ROWS)) {
        for (b in range(nb)) {
            var amax = 0.0
            for (i in range(32)) {
                amax = max(amax, abs(xf_at(p, tok * k + b * 32 + i)))
            }
            let sc = amax / 127.0
            m.xs[tok * nb + b] = sc
            let inv = sc > 0.0 ? 1.0 / sc : 0.0
            var qsum = 0
            for (i in range(32)) {
                let e = tok * k + b * 32 + i
                let q = clamp(roundi(xf_at(p, e) * inv), -127, 127)
                qsum += q
                m.xq[e / 4] |= uint(q & 255) << uint((e % 4) * 8)
            }
            m.xsum[tok * nb + b] = sc * float(qsum)
        }
    }
    return <- m
}

def ws_at(p : Planes; i : int) : float {
    let v = unpackHalf2x16(p.ws[i / 2])
    return (i & 1) == 0 ? v.x : v.y
}

//! the reference: every block's int32 dot scaled by its weight and activation scales
def mmq_ref(p : Planes; m : MmqPlanes; k, tok, row : int) : float {
    let nb = k / 32
    var acc = 0.0
    for (b in range(nb)) {
        var dot = 0
        for (i in range(32)) {
            dot += word_i8_at(p.wq, (row * nb + b) * 32 + i) * word_i8_at(m.xq, tok * k + b * 32 + i)
        }
        acc += float(dot) * ws_at(p, row * nb + b) * m.xs[tok * nb + b]
    }
    return acc
}

def mmq_is_b32(fmt : int) : bool => fmt == int(KqFmt.q51) || fmt == int(KqFmt.q4032) || fmt == int(KqFmt.q5032) || fmt == int(KqFmt.iq4nl32)

//! the MMQ arm's report line for the leaf of `fmt` (Q8_0, a superblock or a per-32 leaf): its largest relative error over the compare region, then its rounds when inside the leaf's bound, else the miss and no time; 1 when it measured
def mmq_report(shape : string; p : Planes; m : MmqPlanes; k, d, fmt : int) : int {
    if (!cud_mmq_ensure(fmt)) {
        print("{shape} cuda refused: {CU_CLASS_REFUSED}\n")
        return 0
    }
    let is_k4 = fmt == int(KqFmt.k4)
    let is_sb_leaf = fmt == int(KqFmt.iq4xs) || fmt == int(KqFmt.q40) || fmt == int(KqFmt.iq4nl)
    let is_b32 = mmq_is_b32(fmt)
    let has_min_term = is_k4 || fmt == int(KqFmt.q51)
    let cs = g_cud.stream
    var bufs = fixed_array(buf_upload(p.wq, cs), buf_upload(p.ws, cs), buf_upload(m.xq, cs), buf_upload(m.xs, cs), buf_fill(ROWS * d, 0.0, cs), buf_upload(m.xsum, cs))
    var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(ROWS))
    let wgs = dasllama_cuda_classes::mmq_wgs(int64(d), int64(ROWS))
    cud_mmq_enc(fmt, g_cud.stream, bufs, pc, wgs)
    cud_sync()
    let y <- buf_download(bufs[4], ROWS * d, type<float>)
    let b32_wq_bytes <- word_plane_bytes(p.wq)
    let b32_ws_bytes <- word_plane_bytes(p.ws)
    var wants : float[64]
    var ymax = 1.0
    for (s in range(64)) {
        let tok = (s * 7919) % ROWS
        let row = (s * 104729 + 13) % d
        if (is_b32) {
            wants[s] = b32_mmq_ref(kq_fmt_of_slot(fmt), b32_wq_bytes, b32_ws_bytes, m.xq, m.xs, k, tok, row)
        } else {
            wants[s] = is_k4 ? k4_mmq_ref(p.wq, p.ws, m.xq, m.xs, k, tok, row) : (is_sb_leaf ? sb_mmq_ref(fmt, p.wq, p.ws, m.xq, m.xs, k, tok, row) : mmq_ref(p, m, k, tok, row))
        }
        ymax = max(ymax, abs(wants[s]))
    }
    // no min term: exact int dots under f32 scales, a per-element bound; a min term is f16-staged, so the f16 tiles' bound, relative to the region's largest magnitude
    let bound = has_min_term ? MIN_TERM_MMQ_REL_BOUND : MMQ_REL_BOUND
    var err = 0.0
    for (s in range(64)) {
        let got = y[((s * 7919) % ROWS) * d + (s * 104729 + 13) % d]
        err = max(err, abs(got - wants[s]) / (has_min_term ? ymax : max(1.0, max(abs(got), abs(wants[s])))))
    }
    if (!(err <= bound)) {
        cuda_mem_free(bufs)
        print("{shape} cuda MISS: max rel err {err} > {bound}\n")
        return 0
    }
    let row = cuda_rounds(ROUNDS, WARMUPS, DISPATCHES) $(_i) {
        cud_mmq_enc(fmt, g_cud.stream, bufs, pc, wgs)
    }
    cuda_mem_free(bufs)
    return report_cuda(shape, row, "bounded vs cpu: max rel err {err} <= {bound}", "TFLOP/s") $(us) => tflops(k, d, us)
}

//! the FP4 tensor rung's arm (CUDA alone): the nv4 leaf over the panel quantized into the FP4 feed, its largest relative error against the e2m1 x UE4M3 reference, then its rounds; 1 when it measured
def fp4_mmq_report(shape : string; p : Planes; k, d : int) : int {
    if (!dasllama_cuda_classes::ensure_nv4_mmq()) {
        print("{shape} cuda refused: {CU_CLASS_REFUSED}\n")
        return 0
    }
    var inscope xrows <- [for (e in range(ROWS * k)); xf_at(p, e)]
    var inscope feed <- fp4_feed_rows(xrows, ROWS, k)
    let cs = g_cud.stream
    var bufs = fixed_array(buf_upload(p.wq, cs), buf_upload(p.ws, cs), buf_upload(feed.xq, cs), buf_upload(feed.xsw, cs), buf_upload(feed.xrs, cs), buf_fill(ROWS * d, 0.0, cs))
    var pc = MmqFp4Args(n = uint(k), d = uint(d), rows = uint(ROWS), wblk = 0u, site_scale = 1.0)
    let wgs = dasllama_cuda_classes::mmq_wgs(int64(d), int64(ROWS))
    dasllama_cuda_classes::enc_nv4_mmq(g_cud.stream, bufs, pc, wgs)
    cud_sync()
    let y <- buf_download(bufs[5], ROWS * d, type<float>)
    var err = 0.0
    for (s in range(64)) {
        let tok = (s * 7919) % ROWS
        let row = (s * 104729 + 13) % d
        let want = nv4_mmq_ref(p.wq, p.ws, feed, k, tok, row, 1.0)
        let got = y[tok * d + row]
        err = max(err, abs(got - want) / max(1.0, max(abs(got), abs(want))))
    }
    if (!(err <= MMQ_REL_BOUND)) {
        cuda_mem_free(bufs)
        print("{shape} cuda MISS: max rel err {err} > {MMQ_REL_BOUND}\n")
        return 0
    }
    let row = cuda_rounds(ROUNDS, WARMUPS, DISPATCHES) $(_i) {
        dasllama_cuda_classes::enc_nv4_mmq(g_cud.stream, bufs, pc, wgs)
    }
    cuda_mem_free(bufs)
    return report_cuda(shape, row, "bounded vs cpu: max rel err {err} <= {MMQ_REL_BOUND}", "TFLOP/s") $(us) => tflops(k, d, us)
}

struct FaPlanes {
    q : array<float>
    kh : array<uint>   //! the f16 mirror planes as half pairs
    vh : array<uint>
    wgs : int
}

def fa_planes : FaPlanes {
    var p : FaPlanes
    p.q |> resize(ROWS * FA_QD)
    for (i in range(ROWS * FA_QD)) {
        p.q[i] = float((i * 7919) % 2001 - 1000) / 1000.0
    }
    let mir_rows = ROWS + int(TILE_READ_SLACK)
    p.kh <- ws_plane(mir_rows * FA_KVD / 2, 3u, 5u)
    p.vh <- ws_plane(mir_rows * FA_KVD / 2, 11u, 13u)
    p.wgs = FA_HEADS * ((ROWS + 63) / 64)
    return <- p
}

def fa_args : FaArgs => FaArgs(rows = uint(ROWS), w0 = 0u, qd = uint(FA_QD), kvd = uint(FA_KVD), kv_mul = uint(FA_HEADS / (FA_KVD / FA_HS)),
    kvbase = 0u, scale = 0.125, qstride = uint(FA_QD), qhs = uint(FA_HS), window = 0u, cap = 0.0, sinkoff = 0u, row0 = 0u, ring = 0u)

//! the flash tile's useful work: the causal half of QK^T and PV over the window
def fa_tflops(us : double) : double => 2.0lf * 2.0lf * double(FA_HEADS) * double(ROWS) * double(ROWS) / 2.0lf * double(FA_HS) / us / 1.0e6lf

def race_fa(p : FaPlanes) : RaceRow {
    var pc = fa_args()
    let wgs = int64(p.wgs)
    let vk_why = cm2_why(fa_stamp_ensure(false, int64(FA_HS), false, false, false, true))
    let cuw = cu_why(dasllama_cuda_classes::ensure_fa_cm2_h64_f16_cls())
    var vbufs : uint64[4]
    var raw : VkCommandBuffer
    if (vk_why == "") {
        vbufs = fixed_array(dev_upload(p.q), dev_upload(p.kh), dev_upload(p.vh), make_device_buf(int64(ROWS) * int64(FA_QD) * 2l))
        var set_ = fa_stamp_set(false, vbufs,
            fixed_array(long_length(p.q) * 4l, long_length(p.kh) * 4l, long_length(p.vh) * 4l, int64(ROWS) * int64(FA_QD) * 2l),
            fixed_array(0u, 0u, 0u, 1u))
        raw = record_arm() $(cmd : VkCommandBuffer; var hz : VkHaz) {
            for (_i in range(DISPATCHES)) {
                fa_stamp_enc(false, int64(FA_HS), false, false, false, true, cmd, hz, set_, pc, wgs)
            }
        }
    }
    var cbufs : uint64[4]
    if (cuw == "") {
        let s = g_cud.stream
        cbufs = fixed_array(buf_upload(p.q, s), buf_upload(p.kh, s), buf_upload(p.vh, s), buf_fill(ROWS * FA_QD / 2, 0.0, s))
    }
    let row = race(ROUNDS, WARMUPS, DISPATCHES, vk_why, raw, cuw) $(_i) {
        dasllama_cuda_classes::enc_fa_cm2_h64_f16_cls(g_cud.stream, cbufs, pc, wgs)
    }
    if (vk_why == "") {
        for (b in vbufs) {
            destroy_device_buf(b)
        }
    }
    if (cuw == "") {
        cuda_mem_free(cbufs)
    }
    return row
}

let DA_KEYS = 1024   //! the keys a token step attends: a served prompt's depth, not the bench's short one

struct DaPlanes {
    q : array<float>
    kh : array<uint>
    vh : array<uint>
    tok : array<uint>    //! TokMeta: pos, cnt, the rest zero
    part : array<float>  //! the split pieces, the arrival counters past them at rest
    nsplit : int
    cntoff : int
    wgs : int
}

def da_planes(nsplit : int) : DaPlanes {
    var p : DaPlanes
    p.q |> resize(FA_QD)
    for (i in range(FA_QD)) {
        p.q[i] = float((i * 7919) % 2001 - 1000) / 1000.0
    }
    let mir_rows = DA_KEYS + int(TILE_READ_SLACK)
    p.kh <- ws_plane(mir_rows * FA_KVD / 2, 3u, 5u)
    p.vh <- ws_plane(mir_rows * FA_KVD / 2, 11u, 13u)
    p.tok <- [uint(DA_KEYS - 1), uint(DA_KEYS), 0u, 0u, 0u, 0u, 0u, 0u]
    p.nsplit = nsplit
    p.cntoff = FA_HEADS * nsplit * (FA_HS + 2)
    p.part |> resize(p.cntoff + FA_HEADS)
    p.wgs = int(da_attn_row_wgs(int64(FA_HEADS), int64(FA_HEADS / (FA_KVD / FA_HS)), int64(nsplit)))
    return <- p
}

def da_args(p : DaPlanes) : DaAttnArgs => DaAttnArgs(kvd = uint(FA_KVD), hs = uint(FA_HS), kv_mul = uint(FA_HEADS / (FA_KVD / FA_HS)),
    kbase = 0u, vbase = 0u, scale = 0.125, qstride = uint(FA_HS), gated = 0u, window = 0u, cap = 0.0, nsplit = uint(p.nsplit),
    rqk = DA_RQ_Q8, sinkoff = 0u, rowwg = uint(p.wgs), qrow = uint(FA_QD), cntoff = uint(p.cntoff), ring = 0u)

def race_da(p : DaPlanes) : RaceRow {
    var pc = da_args(p)
    let wgs = int64(p.wgs)
    let vk_why = dasllama_vulkan_classes::ensure_da_attn_f16_cls() ? "" : VK_CLASS_REFUSED
    let cuw = cu_why(dasllama_cuda_classes::ensure_da_attn_f16_cls())
    var vbufs : uint64[8]
    var raw : VkCommandBuffer
    if (vk_why == "") {
        vbufs = fixed_array(dev_upload(p.q), dev_upload(p.kh), dev_upload(p.vh), make_device_buf(int64(FA_QD) * 4l), dev_upload(p.tok),
            dev_upload(p.part), make_device_buf(int64(FA_QD)), make_device_buf(int64(FA_QD / 32) * 4l))
        var set_ = set_da_attn_f16_cls(vbufs,
            fixed_array(int64(FA_QD) * 4l, long_length(p.kh) * 4l, long_length(p.vh) * 4l, int64(FA_QD) * 4l, 32l, long_length(p.part) * 4l, int64(FA_QD), int64(FA_QD / 32) * 4l),
            fixed_array(0u, 0u, 0u, 1u, 0u, 1u, 2u, 2u))
        raw = record_arm() $(cmd : VkCommandBuffer; var hz : VkHaz) {
            for (_i in range(DISPATCHES)) {
                dasllama_vulkan_classes::enc_da_attn_f16_cls(cmd, hz, set_, pc, wgs)
            }
        }
    }
    var cbufs : uint64[8]
    if (cuw == "") {
        let s = g_cud.stream
        cbufs = fixed_array(buf_upload(p.q, s), buf_upload(p.kh, s), buf_upload(p.vh, s), buf_fill(FA_QD, 0.0, s), buf_upload(p.tok, s),
            buf_upload(p.part, s), buf_fill(FA_QD / 4, 0u, s), buf_fill(FA_QD / 32, 0.0, s))
    }
    let row = race(ROUNDS, WARMUPS, DISPATCHES, vk_why, raw, cuw) $(_i) {
        dasllama_cuda_classes::enc_da_attn_f16_cls(g_cud.stream, cbufs, pc, wgs)
    }
    if (vk_why == "") {
        for (b in vbufs) {
            destroy_device_buf(b)
        }
    }
    if (cuw == "") {
        cuda_mem_free(cbufs)
    }
    return row
}

//! the q8 cm2 l and m tiles over the window shapes, both homes; the rows measured
def race_q8_rows(shapes : array<tuple<k : int; d : int>>) : int {
    var measured = 0
    print("q8 cm2 tiles over {ROWS} rows, one card, both discrete homes: the best of {ROUNDS} rounds of {DISPATCHES} dispatches after {WARMUPS} warmup rounds, us a dispatch\n")
    for (tc in [256, 128]) {
        for (s in shapes) {
            var p <- planes(s.k, s.d, tc)
            let row = race_q8_tile(p, s.k, s.d, tc)
            measured += report_race(pad_right("{tc == 256 ? "l" : "m"} k {s.k} d {s.d}", 24, ' '), row, "TFLOP/s") $(us) => tflops(s.k, s.d, us)
            delete p
        }
    }
    return measured
}

//! the superblock formats' cm2 l and m tiles - every format of the list, or the ones `only` names; the rows measured
def race_sb_rows(shapes : array<tuple<k : int; d : int>>; only : table<string; void>) : int {
    var measured = 0
    print("superblock cm2 tiles (the Q4_K_M file's formats; the three 4-bit lattices - Q4_0, IQ4_NL, NVFP4)\n")
    for (fmt in fixed_array(int(KqFmt.k4), int(KqFmt.k6), int(KqFmt.q40), int(KqFmt.iq4nl), int(KqFmt.nv4))) {
        if (!empty(only) && !key_exists(only, "{kq_fmt_of_slot(fmt)}")) continue
        for (tc in [256, 128]) {
            for (s in shapes) {
                var p <- kq_tile_planes(fmt, s.k, s.d, tc)
                let row = race_kq_tile(fmt, p, s.k, s.d, tc)
                let shape = pad_right("{kq_fmt_of_slot(fmt)} {tc == 256 ? "l" : "m"} k {s.k} d {s.d}", 24, ' ')
                measured += report_race(shape, row, "TFLOP/s") $(us) => tflops(s.k, s.d, us)
                delete p
            }
        }
    }
    return measured
}

//! the superblock int8 GEMMs (CUDA alone) and the nv4 leaf on the FP4 tensor rung - every listed format, or the ones `only` names; the rows measured
def race_sb_mmq_rows(shapes : array<tuple<k : int; d : int>>; only : table<string; void>) : int {
    var measured = 0
    print("the superblock int8 GEMMs (CUDA alone): the decode in the weight stage, Q4_K's min term, the codebooks, Q4_0's offset\n")
    for (fmt in fixed_array(int(KqFmt.k4), int(KqFmt.iq4xs), int(KqFmt.q40), int(KqFmt.iq4nl))) {
        if (!empty(only) && !key_exists(only, "{kq_fmt_of_slot(fmt)}")) continue
        for (s in shapes) {
            var p <- kq_tile_planes(fmt, s.k, s.d, 128)
            var m <- mmq_planes(p, s.k)
            measured += mmq_report(pad_right("{kq_fmt_of_slot(fmt)} mmq k {s.k} d {s.d}", 24, ' '), p, m, s.k, s.d, fmt)
            delete m
            delete p
        }
    }
    if (empty(only) || key_exists(only, "nv4")) {
        print("the nv4 leaf on the FP4 tensor rung (CUDA alone): raw e2m1 on both sides under the block-scaled mma\n")
        for (s in shapes) {
            var p <- kq_tile_planes(int(KqFmt.nv4), s.k, s.d, 128)
            measured += fp4_mmq_report(pad_right("nv4 fp4 mmq k {s.k} d {s.d}", 24, ' '), p, s.k, s.d)
            delete p
        }
    }
    return measured
}

[export]
def main {
    let only <- only_arms()
    let llama8b_shapes = key_exists(only, "8b")
    let shapes <- [(k = llama8b_shapes ? 4096 : 2048, d = llama8b_shapes ? 4096 : 2048), (k = llama8b_shapes ? 4096 : 2048, d = llama8b_shapes ? 14336 : 8192), (k = llama8b_shapes ? 14336 : 8192, d = llama8b_shapes ? 4096 : 2048)]
    var measured = 0
    if (!empty(only)) {
        dasllama_vulkan_classes::ensure_q8_batch_cm2l_cls()   // arms the device the format picks read their mode off
        measured += race_sb_rows(shapes, only)
        measured += race_sb_mmq_rows(shapes, only)
        cud_shutdown()
        require_measured(measured)
        return
    }
    measured += race_q8_rows(shapes)
    measured += race_sb_rows(shapes, only)
    print("per-32 cm2 tiles (the Q5_1 file's format)\n")
    for (fmt in fixed_array(int(KqFmt.q51))) {
        for (tc in [256, 128, 32]) {
            for (s in shapes) {
                var p <- b32_tile_planes(fmt, s.k, s.d, tc)
                let row = race_kq_tile(fmt, p, s.k, s.d, tc)
                let shape = pad_right("{kq_fmt_of_slot(fmt)} {tc == 256 ? "l" : (tc == 128 ? "m" : "s")} k {s.k} d {s.d}", 24, ' ')
                measured += report_race(shape, row, "TFLOP/s") $(us) => tflops(s.k, s.d, us)
                delete p
            }
        }
    }
    print("the int8 GEMM (CUDA alone), the baseline arm\n")
    for (s in shapes) {
        var p <- planes(s.k, s.d, 128)
        var m <- mmq_planes(p, s.k)
        measured += mmq_report(pad_right("mmq k {s.k} d {s.d}", 24, ' '), p, m, s.k, s.d, int(KqFmt.q8))
        delete m
        delete p
    }
    measured += race_sb_mmq_rows(shapes, only)
    print("the per-32 int8 GEMMs (CUDA alone): Q5_1's min term, Q4_0 and Q5_0's offsets in the codes, IQ4_NL's codebook\n")
    for (fmt in fixed_array(int(KqFmt.q51), int(KqFmt.q4032), int(KqFmt.q5032), int(KqFmt.iq4nl32))) {
        for (s in shapes) {
            var p <- b32_tile_planes(fmt, s.k, s.d, 128)
            var m <- mmq_planes(p, s.k)
            measured += mmq_report(pad_right("{kq_fmt_of_slot(fmt)} mmq k {s.k} d {s.d}", 24, ' '), p, m, s.k, s.d, fmt)
            delete m
            delete p
        }
    }
    var fp <- fa_planes()
    let fa_row = race_fa(fp)
    measured += report_race("flash h64 f16, {FA_HEADS} heads x {ROWS} rows:", fa_row, "TFLOP/s") $(us) => fa_tflops(us)
    delete fp
    for (ns in [1, 3, 8]) {
        var dp <- da_planes(ns)
        let da_row = race_da(dp)
        measured += report_race("decode attention h64 f16, {FA_HEADS} heads over {DA_KEYS} keys in {ns} piece(s) ({dp.wgs} workgroups):", da_row)
        delete dp
    }
    cud_shutdown()
    require_measured(measured)
}
