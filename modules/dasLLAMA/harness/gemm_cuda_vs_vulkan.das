options gen2
options _dasllama_internal = true
options stack = 524288

// The window chain's tiles on both discrete homes of one card, the same planes: the Vulkan class kernel through the hazard
// rail and the CUDA class kernel through its launch rail - the Q8_0 cm2 l and m tiles (the window's batch GEMM) at a
// llama-1B prefill window's shapes (512 rows by the projection and FFN widths), the K-quant cm2 tiles and the int8 GEMM's
// Q8_0 and Q4_K leaves at the same shapes, the cm2 flash tile at head 64 over the same window (32 heads, kv width 512) and the decode
// attention's split and unsplit forms - the time a dispatch and the rate it makes.
// Baseline: the mmq arm, checked against the CPU reference mmq_ref. Compare region: 64 sampled outputs, token (s x 7919) % 512
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
require _cuda_probe_fixture
require daslib/strings_boost
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema

let ROUNDS = 10
let WARMUPS = 2
let DISPATCHES = 20
let MMQ_REL_BOUND = 1.0e-3   //! the MMQ kernel cell's bar (`test_cud_q8_mmq`): relative to the larger magnitude, floored at 1
let K4_MMQ_REL_BOUND = 2.0e-3   //! the Q4_K leaf's (`test_cud_k4_mmq`, the f16 tiles' `TILE_BAR`): relative to the compare region's largest magnitude
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
    p.ws <- ws_plane(d * nsb * int(kq_dev_ssb_i(fmt) / 4l), 1u, uint(fmt) * 13u)
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
    xsum : array<float> //! a block's scale x the sum of its quants (the Q4_K leaf's min-term feed)
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

def byte_at(words : array<uint>; i : int) : int {
    let b = int((words[i / 4] >> uint((i % 4) * 8)) & 255u)
    return b >= 128 ? b - 256 : b
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
            dot += byte_at(p.wq, (row * nb + b) * 32 + i) * byte_at(m.xq, tok * k + b * 32 + i)
        }
        acc += float(dot) * ws_at(p, row * nb + b) * m.xs[tok * nb + b]
    }
    return acc
}

//! the Q4_K reference over the engine's repacked superblocks (a 32-block's 16 bytes: element i the low nibble, i + 16 the
//! high; five scale words a superblock): every block's xs x (d sc dot - dmin mn xsum)
def k4_mmq_ref(p : Planes; m : MmqPlanes; k, tok, row : int) : float {
    let nb = k / 32
    let nsb = k / 256
    var acc = 0.0
    for (b in range(nb)) {
        let sb = row * nsb + b / 8
        let g = b % 8
        let dm = unpackHalf2x16(p.ws[sb * 5])
        let sc = float((p.ws[sb * 5 + 1 + g / 4] >> uint((g % 4) * 8)) & 255u)
        let mn = float((p.ws[sb * 5 + 3 + g / 4] >> uint((g % 4) * 8)) & 255u)
        var dot = 0
        var xsm = 0
        for (i in range(32)) {
            let wb = (sb * 8 + g) * 16 + i % 16
            let byte = int((p.wq[wb / 4] >> uint((wb % 4) * 8)) & 255u)
            let xv = byte_at(m.xq, tok * k + b * 32 + i)
            dot += (i < 16 ? byte & 15 : byte >> 4) * xv
            xsm += xv
        }
        acc += m.xs[tok * nb + b] * (dm.x * sc * float(dot) - dm.y * mn * float(xsm))
    }
    return acc
}

//! the MMQ arm's report line - the Q8_0 leaf, or under `k4` the Q4_K one with its sum feed: its largest relative error against
//! the reference over the compare region, then its rounds when the error is inside `MMQ_REL_BOUND`, or the miss and no time;
//! 1 when it measured
def mmq_report(shape : string; p : Planes; m : MmqPlanes; k, d : int; k4 : bool) : int {
    if (!(k4 ? dasllama_cuda_classes::ensure_k4_mmq() : dasllama_cuda_classes::ensure_q8_mmq())) {
        print("{shape} cuda refused: {CU_CLASS_REFUSED}\n")
        return 0
    }
    let cs = g_cud.stream
    var bufs = fixed_array(buf_upload(p.wq, cs), buf_upload(p.ws, cs), buf_upload(m.xq, cs), buf_upload(m.xs, cs), buf_fill(ROWS * d, 0.0, cs), buf_upload(m.xsum, cs))
    var pc = MmqArgs(n = uint(k), d = uint(d), rows = uint(ROWS))
    let wgs = dasllama_cuda_classes::mmq_wgs(int64(d), int64(ROWS))
    mmq_launch(k4, bufs, pc, wgs)
    cud_sync()
    let y <- buf_download(bufs[4], ROWS * d, type<float>)
    var wants : float[64]
    var ymax = 1.0
    for (s in range(64)) {
        let tok = (s * 7919) % ROWS
        let row = (s * 104729 + 13) % d
        wants[s] = k4 ? k4_mmq_ref(p, m, k, tok, row) : mmq_ref(p, m, k, tok, row)
        ymax = max(ymax, abs(wants[s]))
    }
    // the Q8 leaf's error is per element (exact int dots, f32 scales); the Q4_K leaf's min terms are f16-staged, so its bound
    // is the f16 tiles' - relative to the region's largest magnitude
    let bound = k4 ? K4_MMQ_REL_BOUND : MMQ_REL_BOUND
    var err = 0.0
    for (s in range(64)) {
        let got = y[((s * 7919) % ROWS) * d + (s * 104729 + 13) % d]
        err = max(err, abs(got - wants[s]) / (k4 ? ymax : max(1.0, max(abs(got), abs(wants[s])))))
    }
    if (!(err <= bound)) {
        cuda_mem_free(bufs)
        print("{shape} cuda MISS: max rel err {err} > {bound}\n")
        return 0
    }
    let row = cuda_rounds(ROUNDS, WARMUPS, DISPATCHES) $(_i) {
        mmq_launch(k4, bufs, pc, wgs)
    }
    cuda_mem_free(bufs)
    return report_cuda(shape, row, "bounded vs cpu: max rel err {err} <= {bound}", "TFLOP/s") $(us) => tflops(k, d, us)
}

//! one dispatch of the MMQ leaf `k4` names over `bufs` (the Q8_0 leaf binds the first five: no sum feed)
def mmq_launch(k4 : bool; bufs : uint64[6]; var pc : MmqArgs; wgs : int64) {
    if (k4) {
        dasllama_cuda_classes::enc_k4_mmq(g_cud.stream, bufs, pc, wgs)
    } else {
        dasllama_cuda_classes::enc_q8_mmq(g_cud.stream, fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4]), pc, wgs)
    }
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

[export]
def main {
    let shapes <- [(k = 2048, d = 2048), (k = 2048, d = 8192), (k = 8192, d = 2048)]
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
    print("K-quant cm2 tiles (the Q4_K_M file's formats)\n")
    for (fmt in fixed_array(int(KqFmt.k4), int(KqFmt.k6))) {
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
        measured += mmq_report(pad_right("mmq k {s.k} d {s.d}", 24, ' '), p, m, s.k, s.d, false)
        delete m
        delete p
    }
    print("the Q4_K int8 GEMM (CUDA alone): the nibble decode in the weight stage and the min term\n")
    for (s in shapes) {
        var p <- kq_tile_planes(int(KqFmt.k4), s.k, s.d, 128)
        var m <- mmq_planes(p, s.k)
        measured += mmq_report(pad_right("k4 mmq k {s.k} d {s.d}", 24, ' '), p, m, s.k, s.d, true)
        delete m
        delete p
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
