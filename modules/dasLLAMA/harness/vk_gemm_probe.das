options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - isolated per-shape rate of the serving mm_a batch GEMM kernel; the
// rows attribute prefill GEMM cost across the dense role shapes (gate/up, down, q/wo, k/v).
// Direction-grade instrument, never record-grade: the kernel is the shipped, oracle-gated
// one (test_vulkan_kernels.das), dispatched record-once and timed over repeated submits.
// One arg (gate|down|q|kv) pins a single shape - the long-window ngfx GPU Trace vehicle.
// Shapes default to Llama-3.2-3B Q8 geometry (dim 3072, hidden 8192, kv_dim 1024).

require math
require vulkan
require vulkan/vulkan_boost
require spirv/spirv_shader
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_kqformat

let private TILE = 128
let private DISPATCHES = 16
let private SUBMITS = 100

def private hash_word(i : uint) : uint {
    var x = i * 2654435761u
    x ^= x >> 16u
    x *= 2246822519u
    x ^= x >> 13u
    return x
}

def private ws_word(i : uint) : uint {
    // two packed f16 scales per word, small positive values (the kernel-suite fixture shape)
    let h = 0x2c00u + (i % 64u)
    return h | (h << 16u)
}

struct private ShapeBufs {
    wq : array<uint>
    ws : array<uint>
    xq : array<uint>
    xs : array<float>
    xf : array<uint>
}

def private fill_fixture(var b : ShapeBufs; totblk, rows, nbb : int) {
    b.wq |> resize(totblk * 8)
    b.ws |> resize(totblk / 2 + 1)
    b.xq |> resize(rows * nbb * 8)
    b.xs |> resize(rows * nbb)
    b.xf |> resize(rows * nbb * 16)
    for (i in range(totblk * 8)) {
        b.wq[i] = hash_word(uint(i) + 3u)
    }
    for (i in range(totblk / 2 + 1)) {
        b.ws[i] = ws_word(uint(i) + 7u)
    }
    for (i in range(rows * nbb * 8)) {
        b.xq[i] = hash_word(uint(i) * 13u + 1u)
    }
    for (i in range(rows * nbb)) {
        // cool inputs: the mm family accumulates in f16, hot fixtures overflow 65504
        b.xs[i] = (0.375 + float(i % 9) * 0.0625) * 0.0625
    }
    for (i in range(rows * nbb * 16)) {
        b.xf[i] = ws_word(uint(i) * 5u + 11u)   // the cm2 feed: two packed cool f16 rows/word
    }
}

// ===== cm2 decode-cost bisect (arg "cm2x") - the shipped l-tile body with the decode method
// varied: nil = constant A (zero decode cost ceiling), flat = quant only (no scale-plane read),
// lit = full decode minus the wg_blk0 shared-memory read. Fast-path no-split arm only =====

[vk_dispatch(name = "cm2px_nil", grid = "wgs", params = "wgs : int64")]
class Cm2ProbeNil : MoeCmBase {
    @ssbo @binding = 0 @readonly wq : array<VkQ8Blk>
    @ssbo @binding = 1 @role = "alias" @readonly wsh : array<float16>
    @ssbo @binding = 3 @readonly xf16 : array<float16>
    @ssbo @binding = 5 y : array<float>
    @workgroup wg_blk0 : uint

    [spirv_decode]
    def decode_q8(blk : VkQ8Blk; bc, cib : uint2) : float16 {
        return float16(0.05)
    }

    [spirv_kernel(local_size_x = 256, name = "cm2px_nil_spv")]
    def run {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 255u) / 256u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        barrier()
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        let t0 = row0 + xt * 256u
        let m0 = wt * 128u
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 32u)
        tensorLayoutSetDimension(fla, pa.d, pa.n)
        tensorLayoutSetStride(fla, pa.n >> 5u, 1u)
        var flb : tensorLayout2D
        tensorLayoutCreate(flb)
        tensorLayoutSetDimension(flb, row0 + cnt, pa.n)
        tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
        var flo : tensorLayout2D
        tensorLayoutCreate(flo)
        tensorLayoutSetDimension(flo, row0 + cnt, pa.d)
        tensorLayoutSetStride(flo, pa.d & ~7u, 1u)
        var k = 0u
        for (_i in range(int(pa.n / 512u))) {
            for [unroll] (_j in range(8)) {
                coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_q8)
                coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
        }
        while (k < pa.n) {
            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_q8)
            coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, 0u, flo, t0, 256u, m0, 128u, tv)
    }
}

[vk_dispatch(name = "cm2px_flat", grid = "wgs", params = "wgs : int64")]
class Cm2ProbeFlat : MoeCmBase {
    @ssbo @binding = 0 @readonly wq : array<VkQ8Blk>
    @ssbo @binding = 1 @role = "alias" @readonly wsh : array<float16>
    @ssbo @binding = 3 @readonly xf16 : array<float16>
    @ssbo @binding = 5 y : array<float>
    @workgroup wg_blk0 : uint

    [spirv_decode]
    def decode_q8(blk : VkQ8Blk; bc, cib : uint2) : float16 {
        let q = unpack8(blk.qs[int((cib.y & 30u) >> 1u)])[int(cib.y & 1u)]
        return float16(0.05) * float16(float(int(q)))
    }

    [spirv_kernel(local_size_x = 256, name = "cm2px_flat_spv")]
    def run {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 255u) / 256u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        barrier()
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        let t0 = row0 + xt * 256u
        let m0 = wt * 128u
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 32u)
        tensorLayoutSetDimension(fla, pa.d, pa.n)
        tensorLayoutSetStride(fla, pa.n >> 5u, 1u)
        var flb : tensorLayout2D
        tensorLayoutCreate(flb)
        tensorLayoutSetDimension(flb, row0 + cnt, pa.n)
        tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
        var flo : tensorLayout2D
        tensorLayoutCreate(flo)
        tensorLayoutSetDimension(flo, row0 + cnt, pa.d)
        tensorLayoutSetStride(flo, pa.d & ~7u, 1u)
        var k = 0u
        for (_i in range(int(pa.n / 512u))) {
            for [unroll] (_j in range(8)) {
                coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_q8)
                coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
        }
        while (k < pa.n) {
            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_q8)
            coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, 0u, flo, t0, 256u, m0, 128u, tv)
    }
}

// push = full decode with the scale base off a PUSH CONSTANT (pa.ksplit, 0 here) instead of
// the wg_blk0 shared read - prices the dense-dispatch candidate against lit's literal base
[vk_dispatch(name = "cm2px_push", grid = "wgs", params = "wgs : int64")]
class Cm2ProbePush : MoeCmBase {
    @ssbo @binding = 0 @readonly wq : array<VkQ8Blk>
    @ssbo @binding = 1 @role = "alias" @readonly wsh : array<float16>
    @ssbo @binding = 3 @readonly xf16 : array<float16>
    @ssbo @binding = 5 y : array<float>
    @workgroup wg_blk0 : uint

    [spirv_decode]
    def decode_q8(blk : VkQ8Blk; bc, cib : uint2) : float16 {
        let q = unpack8(blk.qs[int((cib.y & 30u) >> 1u)])[int(cib.y & 1u)]
        return wsh[pa.ksplit + bc.x * (pa.n >> 5u) + bc.y] * float16(float(int(q)))
    }

    [spirv_kernel(local_size_x = 256, name = "cm2px_push_spv")]
    def run {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 255u) / 256u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        barrier()
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        let t0 = row0 + xt * 256u
        let m0 = wt * 128u
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 32u)
        tensorLayoutSetDimension(fla, pa.d, pa.n)
        tensorLayoutSetStride(fla, pa.n >> 5u, 1u)
        var flb : tensorLayout2D
        tensorLayoutCreate(flb)
        tensorLayoutSetDimension(flb, row0 + cnt, pa.n)
        tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
        var flo : tensorLayout2D
        tensorLayoutCreate(flo)
        tensorLayoutSetDimension(flo, row0 + cnt, pa.d)
        tensorLayoutSetStride(flo, pa.d & ~7u, 1u)
        var k = 0u
        for (_i in range(int(pa.n / 512u))) {
            for [unroll] (_j in range(8)) {
                coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_q8)
                coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
        }
        while (k < pa.n) {
            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_q8)
            coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, 0u, flo, t0, 256u, m0, 128u, tv)
    }
}

[vk_dispatch(name = "cm2px_lit", grid = "wgs", params = "wgs : int64")]
class Cm2ProbeLit : MoeCmBase {
    @ssbo @binding = 0 @readonly wq : array<VkQ8Blk>
    @ssbo @binding = 1 @role = "alias" @readonly wsh : array<float16>
    @ssbo @binding = 3 @readonly xf16 : array<float16>
    @ssbo @binding = 5 y : array<float>
    @workgroup wg_blk0 : uint

    [spirv_decode]
    def decode_q8(blk : VkQ8Blk; bc, cib : uint2) : float16 {
        let q = unpack8(blk.qs[int((cib.y & 30u) >> 1u)])[int(cib.y & 1u)]
        return wsh[bc.x * (pa.n >> 5u) + bc.y] * float16(float(int(q)))
    }

    [spirv_kernel(local_size_x = 256, name = "cm2px_lit_spv")]
    def run {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 255u) / 256u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        barrier()
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        let t0 = row0 + xt * 256u
        let m0 = wt * 128u
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 32u)
        tensorLayoutSetDimension(fla, pa.d, pa.n)
        tensorLayoutSetStride(fla, pa.n >> 5u, 1u)
        var flb : tensorLayout2D
        tensorLayoutCreate(flb)
        tensorLayoutSetDimension(flb, row0 + cnt, pa.n)
        tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
        var flo : tensorLayout2D
        tensorLayoutCreate(flo)
        tensorLayoutSetDimension(flo, row0 + cnt, pa.d)
        tensorLayoutSetStride(flo, pa.d & ~7u, 1u)
        var k = 0u
        for (_i in range(int(pa.n / 512u))) {
            for [unroll] (_j in range(8)) {
                coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_q8)
                coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
        }
        while (k < pa.n) {
            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_q8)
            coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, 0u, flo, t0, 256u, m0, 128u, tv)
    }
}

// k4 (Q4_K) cm2 decode-in-load vs the serving kq tile - per-shape rate on the qwen role shapes
def private run_k4_shape(name : string; d, n, cnt : int) {   // nolint:STYLE038 — one linear measurement sweep
    let nsb = n / 256
    let totsb = d * nsb
    var wqh : array<uint>
    var wsuh : array<uint>
    var xfh : array<uint>
    var xqh : array<uint>
    var axsh : array<float>
    wqh |> resize(totsb * 32)
    wsuh |> resize(totsb * 5)
    xfh |> resize(cnt * n / 2)
    xqh |> resize(cnt * nsb * 64)
    axsh |> resize(cnt * nsb)
    for (i in range(totsb * 32)) {
        wqh[i] = hash_word(uint(i) + 29u)
    }
    for (sb in range(totsb)) {
        wsuh[sb * 5] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.00005 * float(1 + sb % 5)))
        for (wi in range(4)) {
            wsuh[sb * 5 + 1 + wi] = hash_word(uint(sb * 4 + wi) + 613u)
        }
    }
    for (i in range(cnt * n / 2)) {
        xfh[i] = ws_word(uint(i) * 3u + 5u)
    }
    for (i in range(cnt * nsb * 64)) {
        xqh[i] = hash_word(uint(i) * 13u + 1u)
    }
    for (i in range(cnt * nsb)) {
        axsh[i] = 0.0625
    }
    let wq_bytes = int64(totsb) * 128l
    let ws_bytes = long_length(wsuh) * 4l
    let xf_bytes = int64(cnt * n) * 2l
    let xq_bytes = long_length(xqh) * 4l
    let axs_bytes = int64(cnt * nsb) * 4l
    let y_bytes = int64(cnt * d) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let xfd = make_device_buf(xf_bytes)
    let xqd = make_device_buf(xq_bytes)
    let axsd = make_device_buf(axs_bytes)
    let yd = make_device_buf(y_bytes)
    let flop = 2.0lf * double(cnt) * double(d) * double(n)
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsuh[0]), ws_bytes)
        upload_region_at(xfd, 0l, addr<uint8 const?>(xfh[0]), xf_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
        upload_region_at(axsd, 0l, addr<uint8 const?>(axsh[0]), axs_bytes)
        let vnames = fixed_array("k4l ", "k4m ", "k4lnb", "kqtile")
        for (v in range(4)) {
            let ttile = v == 1 ? 128 : (v == 3 ? 32 : 256)
            let wtile = v == 3 ? 32 : 128
            let wgs = ((cnt + ttile - 1) / ttile) * ((d + wtile - 1) / wtile)
            var sched : array<uint>
            sched |> resize(4 + wgs)
            sched[2] = uint(cnt)
            let sc_bytes = int64(4 + wgs) * 4l
            let scd = make_device_buf(sc_bytes)
            upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
            delete sched
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var hz : VkHaz
            var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
            if (v == 3) {
                var s = kq_batch_cls_set_for(int(KqFmt.k4), false,
                    fixed_array(wqd, wsd, scd, xqd, axsd, yd),
                    fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, axs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                for (_k in range(DISPATCHES)) {
                    var hz2 : VkHaz
                    enc_kq_batch_k4_cls(raw, hz2, s, pc, int64(wgs))
                }
            } else {
                var s = (v == 1
                    ? set_kq_batch_k4_cm2m_cls(fixed_array(wqd, wsd, scd, xfd, yd),
                        fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes),
                        fixed_array(1u, 2u, 0u, 4u, 16u))
                    : set_kq_batch_k4_cm2l_cls(fixed_array(wqd, wsd, scd, xfd, yd),
                        fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes),
                        fixed_array(1u, 2u, 0u, 4u, 16u)))
                for (_k in range(DISPATCHES)) {
                    if (v == 2) {
                        var hz2 : VkHaz
                        enc_kq_batch_k4_cm2l_cls(raw, hz2, s, pc, int64(wgs))
                    } elif (v == 1) {
                        enc_kq_batch_k4_cm2m_cls(raw, hz, s, pc, int64(wgs))
                    } else {
                        enc_kq_batch_k4_cm2l_cls(raw, hz, s, pc, int64(wgs))
                    }
                }
            }
            vk_check(vkEndCommandBuffer(raw), null)
            let us = time_submits(raw)
            let tf = flop / (us * 1000000.0lf)
            print("{name} {vnames[v]}: {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s\n")
        }
    }
    delete wqh
    delete wsuh
    delete xfh
    delete xqh
    delete axsh
}

// ===== k6 decode-spelling bisect (arg "k6x") - the shipped k6 l-tile body with the decode
// varied: nil = constant (ceiling), flat = current compose w/o scale reads, ql = nibble only
// (no qh stream), pair = llama.cpp's 16-bit-pair compose + ONE unpack8 select (v4 form),
// pair16 = the same compose selected through a 2-lane byte2 (the Q8-winning select width) =====

// the shared probe base: members + the single-region fast-path l-tile body; leaves override
// decode_k6 (the kq-family tile_shell/stage_w pattern)
class K6PxBase : MoeCmBase {
    @ssbo @binding = 0 @readonly wq : array<VkK6Blk>
    @ssbo @binding = 1 @role = "alias" @readonly wsu : array<uint>
    @ssbo @binding = 3 @readonly xf16 : array<float16>
    @ssbo @binding = 5 y : array<float>
    @workgroup wg_blk0 : uint

    [spirv_decode]
    def decode_k6(blk : VkK6Blk; bc, cib : uint2) : float16 {
        return float16(0.05)
    }

    def body {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 255u) / 256u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        barrier()
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        let t0 = row0 + xt * 256u
        let m0 = wt * 128u
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 256u)
        tensorLayoutSetDimension(fla, pa.d, pa.n)
        tensorLayoutSetStride(fla, pa.n >> 8u, 1u)
        var flb : tensorLayout2D
        tensorLayoutCreate(flb)
        tensorLayoutSetDimension(flb, row0 + cnt, pa.n)
        tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
        var flo : tensorLayout2D
        tensorLayoutCreate(flo)
        tensorLayoutSetDimension(flo, row0 + cnt, pa.d)
        tensorLayoutSetStride(flo, pa.d & ~7u, 1u)
        var k = 0u
        for (_i in range(int(pa.n / 512u))) {
            for [unroll] (_j in range(8)) {
                coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_k6)
                coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
        }
        while (k < pa.n) {
            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode_k6)
            coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, 0u, flo, t0, 256u, m0, 128u, tv)
    }
}

[vk_dispatch(name = "k6px_nil", grid = "wgs", params = "wgs : int64")]
class K6PxNil : K6PxBase {
    [spirv_kernel(local_size_x = 256, name = "k6px_nil_spv")]
    def run {
        body()
    }
}

[vk_dispatch(name = "k6px_flat", grid = "wgs", params = "wgs : int64")]
class K6PxFlat : K6PxBase {
    [spirv_decode]
    def override decode_k6(blk : VkK6Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let bu = e >> 5u
        let hh = (e >> 4u) & 1u
        let j = e & 15u
        let lo = uint(int(unpack8(blk.ql[int((bu * 16u + j) >> 1u)])[int(j & 1u)])) & 0xFFu
        let hby = uint(int(unpack8(blk.qh[int(((bu >> 2u) * 32u + hh * 16u + j) >> 1u)])[int(j & 1u)])) & 0xFFu
        let q6 = int(((lo >> (hh * 4u)) & 0xFu) | (((hby >> ((bu & 3u) * 2u)) & 3u) << 4u)) - 32
        return float16(0.001) * float16(float(q6))
    }

    [spirv_kernel(local_size_x = 256, name = "k6px_flat_spv")]
    def run {
        body()
    }
}

[vk_dispatch(name = "k6px_ql", grid = "wgs", params = "wgs : int64")]
class K6PxQl : K6PxBase {
    [spirv_decode]
    def override decode_k6(blk : VkK6Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let bu = e >> 5u
        let hh = (e >> 4u) & 1u
        let j = e & 15u
        let lo = uint(int(unpack8(blk.ql[int((bu * 16u + j) >> 1u)])[int(j & 1u)])) & 0xFFu
        let q6 = int((lo >> (hh * 4u)) & 0xFu) - 8
        return float16(0.001) * float16(float(q6))
    }

    [spirv_kernel(local_size_x = 256, name = "k6px_ql_spv")]
    def run {
        body()
    }
}

[vk_dispatch(name = "k6px_pair", grid = "wgs", params = "wgs : int64")]
class K6PxPair : K6PxBase {
    [spirv_decode]
    def override decode_k6(blk : VkK6Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let bu = e >> 5u
        let hh = (e >> 4u) & 1u
        let j = e & 15u
        let qlw = (uint(int(blk.ql[int((bu * 16u + j) >> 1u)])) >> (hh * 4u)) & 0x0F0Fu
        let qhw = ((uint(int(blk.qh[int(((bu >> 2u) * 32u + hh * 16u + j) >> 1u)])) >> ((bu & 3u) * 2u)) & 0x0303u) << 4u
        let q = int(unpack8(int(qlw | qhw))[int(j & 1u)]) - 32
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let sidx = e >> 4u
        let sc = unpack8(int(wsu[srow + (sidx >> 2u)]))[int(sidx & 3u)]
        let d = unpackHalf2x16(wsu[srow + 4u]).x
        return float16(d * float(int(sc)) * float(q))
    }

    [spirv_kernel(local_size_x = 256, name = "k6px_pair_spv")]
    def run {
        body()
    }
}

[vk_dispatch(name = "k6px_sfix", grid = "wgs", params = "wgs : int64")]
class K6PxSfix : K6PxBase {
    [spirv_decode]
    def override decode_k6(blk : VkK6Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let bu = e >> 5u
        let hh = (e >> 4u) & 1u
        let j = e & 15u
        let lo = uint(int(unpack8(blk.ql[int((bu * 16u + j) >> 1u)])[int(j & 1u)])) & 0xFFu
        let hby = uint(int(unpack8(blk.qh[int(((bu >> 2u) * 32u + hh * 16u + j) >> 1u)])[int(j & 1u)])) & 0xFFu
        let q6 = int(((lo >> (hh * 4u)) & 0xFu) | (((hby >> ((bu & 3u) * 2u)) & 3u) << 4u)) - 32
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let sidx = e >> 4u
        // signed sub-scale via shift+mask - NOT a byte4 dynamic select (the Q8-chase death shape)
        let sc = int(wsu[srow + (sidx >> 2u)] << ((3u - (sidx & 3u)) * 8u)) >> 24
        let d = unpackHalf2x16(wsu[srow + 4u]).x
        return float16(d * float(sc) * float(q6))
    }

    [spirv_kernel(local_size_x = 256, name = "k6px_sfix_spv")]
    def run {
        body()
    }
}

[vk_dispatch(name = "k6px_pair16", grid = "wgs", params = "wgs : int64")]
class K6PxPair16 : K6PxBase {
    [spirv_decode]
    def override decode_k6(blk : VkK6Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let bu = e >> 5u
        let hh = (e >> 4u) & 1u
        let j = e & 15u
        let qlw = (uint(int(blk.ql[int((bu * 16u + j) >> 1u)])) >> (hh * 4u)) & 0x0F0Fu
        let qhw = ((uint(int(blk.qh[int(((bu >> 2u) * 32u + hh * 16u + j) >> 1u)])) >> ((bu & 3u) * 2u)) & 0x0303u) << 4u
        let q = int(unpack8(int16(int(qlw | qhw)))[int(j & 1u)]) - 32
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let sidx = e >> 4u
        let sc = unpack8(int(wsu[srow + (sidx >> 2u)]))[int(sidx & 3u)]
        let d = unpackHalf2x16(wsu[srow + 4u]).x
        return float16(d * float(int(sc)) * float(q))
    }

    [spirv_kernel(local_size_x = 256, name = "k6px_pair16_spv")]
    def run {
        body()
    }
}

// k6 (Q6_K) cm2 decode-in-load vs the serving kq tile - same instrument as run_k4_shape;
// bisect adds the k6px decode-spelling variants
def private run_k6_shape(name : string; d, n, cnt : int; bisect : bool = false) {   // nolint:STYLE037,STYLE038 — one linear measurement sweep
    let nsb = n / 256
    let totsb = d * nsb
    var wqh : array<uint>
    var wsuh : array<uint>
    var xfh : array<uint>
    var xqh : array<uint>
    var axsh : array<float>
    wqh |> resize(totsb * 48)
    wsuh |> resize(totsb * 5)
    xfh |> resize(cnt * n / 2)
    xqh |> resize(cnt * nsb * 64)
    axsh |> resize(cnt * nsb)
    for (i in range(totsb * 48)) {
        wqh[i] = hash_word(uint(i) + 41u)
    }
    for (sb in range(totsb)) {
        for (wi in range(4)) {
            wsuh[sb * 5 + wi] = hash_word(uint(sb * 4 + wi) + 811u)
        }
        wsuh[sb * 5 + 4] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0))
    }
    for (i in range(cnt * n / 2)) {
        xfh[i] = ws_word(uint(i) * 3u + 5u)
    }
    for (i in range(cnt * nsb * 64)) {
        xqh[i] = hash_word(uint(i) * 13u + 1u)
    }
    for (i in range(cnt * nsb)) {
        axsh[i] = 0.0625
    }
    let wq_bytes = int64(totsb) * 192l
    let ws_bytes = long_length(wsuh) * 4l
    let xf_bytes = int64(cnt * n) * 2l
    let xq_bytes = long_length(xqh) * 4l
    let axs_bytes = int64(cnt * nsb) * 4l
    let y_bytes = int64(cnt * d) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let xfd = make_device_buf(xf_bytes)
    let xqd = make_device_buf(xq_bytes)
    let axsd = make_device_buf(axs_bytes)
    let yd = make_device_buf(y_bytes)
    let flop = 2.0lf * double(cnt) * double(d) * double(n)
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsuh[0]), ws_bytes)
        upload_region_at(xfd, 0l, addr<uint8 const?>(xfh[0]), xf_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
        upload_region_at(axsd, 0l, addr<uint8 const?>(axsh[0]), axs_bytes)
        let vnames = fixed_array("k6l ", "k6m ", "k6lnb", "kqtile", "xnil ", "xflat", "xql  ", "xpair", "xpr16", "xsfix")
        for (v in range(bisect ? 10 : 4)) {
            let ttile = v == 1 ? 128 : (v == 3 ? 32 : 256)
            let wtile = v == 3 ? 32 : 128
            let wgs = ((cnt + ttile - 1) / ttile) * ((d + wtile - 1) / wtile)
            var sched : array<uint>
            sched |> resize(4 + wgs)
            sched[2] = uint(cnt)
            let sc_bytes = int64(4 + wgs) * 4l
            let scd = make_device_buf(sc_bytes)
            upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
            delete sched
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var hz : VkHaz
            var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
            if (v >= 4) {
                let bufs = fixed_array(wqd, wsd, scd, xfd, yd)
                let sizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes)
                let gbits = fixed_array(1u, 2u, 0u, 4u, 16u)
                for (_k in range(DISPATCHES)) {
                    if (v == 4) {
                        var s = set_k6px_nil(bufs, sizes, gbits)
                        enc_k6px_nil(raw, hz, s, pc, int64(wgs))
                    } elif (v == 5) {
                        var s = set_k6px_flat(bufs, sizes, gbits)
                        enc_k6px_flat(raw, hz, s, pc, int64(wgs))
                    } elif (v == 6) {
                        var s = set_k6px_ql(bufs, sizes, gbits)
                        enc_k6px_ql(raw, hz, s, pc, int64(wgs))
                    } elif (v == 7) {
                        var s = set_k6px_pair(bufs, sizes, gbits)
                        enc_k6px_pair(raw, hz, s, pc, int64(wgs))
                    } elif (v == 8) {
                        var s = set_k6px_pair16(bufs, sizes, gbits)
                        enc_k6px_pair16(raw, hz, s, pc, int64(wgs))
                    } else {
                        var s = set_k6px_sfix(bufs, sizes, gbits)
                        enc_k6px_sfix(raw, hz, s, pc, int64(wgs))
                    }
                }
            } elif (v == 3) {
                var s = kq_batch_cls_set_for(int(KqFmt.k6), false,
                    fixed_array(wqd, wsd, scd, xqd, axsd, yd),
                    fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, axs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                for (_k in range(DISPATCHES)) {
                    var hz2 : VkHaz
                    enc_kq_batch_k6_cls(raw, hz2, s, pc, int64(wgs))
                }
            } else {
                var s = (v == 1
                    ? set_kq_batch_k6_cm2m_cls(fixed_array(wqd, wsd, scd, xfd, yd),
                        fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes),
                        fixed_array(1u, 2u, 0u, 4u, 16u))
                    : set_kq_batch_k6_cm2l_cls(fixed_array(wqd, wsd, scd, xfd, yd),
                        fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes),
                        fixed_array(1u, 2u, 0u, 4u, 16u)))
                for (_k in range(DISPATCHES)) {
                    if (v == 2) {
                        var hz2 : VkHaz
                        enc_kq_batch_k6_cm2l_cls(raw, hz2, s, pc, int64(wgs))
                    } elif (v == 1) {
                        enc_kq_batch_k6_cm2m_cls(raw, hz, s, pc, int64(wgs))
                    } else {
                        enc_kq_batch_k6_cm2l_cls(raw, hz, s, pc, int64(wgs))
                    }
                }
            }
            vk_check(vkEndCommandBuffer(raw), null)
            let us = time_submits(raw)
            let tf = flop / (us * 1000000.0lf)
            print("{name} {vnames[v]}: {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s\n")
        }
    }
    delete wqh
    delete wsuh
    delete xfh
    delete xqh
    delete axsh
}

// two warmup submits (pipeline compile + clocks settle), then the timed window
def private time_submits(var raw : VkCommandBuffer) : double {
    submit_wait(raw)
    submit_wait(raw)
    let t0 = ref_time_ticks()
    for (_s in range(SUBMITS)) {
        submit_wait(raw)
    }
    return double(get_time_usec(t0)) / double(SUBMITS * DISPATCHES)
}

def private run_shape(name : string; d, n, cnt : int) {   // nolint:STYLE038 — one linear measurement sweep
    let nbb = n / 32
    let rows = cnt
    let wtiles = (d + TILE - 1) / TILE
    let wgs = ((cnt + TILE - 1) / TILE) * wtiles
    let totblk = d * nbb
    var b : ShapeBufs
    fill_fixture(b, totblk, rows, nbb)
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    let xq_bytes = int64(rows * nbb) * 32l
    let xs_bytes = int64(rows * nbb) * 4l
    let xf_bytes = int64(rows * nbb) * 64l
    let y_bytes = int64(rows * d) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let xqd = make_device_buf(xq_bytes)
    let xsd = make_device_buf(xs_bytes)
    let xfd = make_device_buf(xf_bytes)
    let yd = make_device_buf(y_bytes)
    var sched : array<uint>
    sched |> resize(4 + wgs)
    sched[2] = uint(cnt)   // one region: [wblk0 = 0, row0 = 0, cnt, wg0 = 0], map all zeros
    var per_us : double
    var cm2l_us = 0.0lf
    var cm2m_us = 0.0lf
    let lwgs = ((cnt + 255) / 256) * ((d + 127) / 128)
    let mwgs = ((cnt + 127) / 128) * ((d + 127) / 128)
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(b.wq[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(b.ws[0]), ws_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(b.xq[0]), xq_bytes)
        upload_region_at(xsd, 0l, addr<uint8 const?>(b.xs[0]), xs_bytes)
        upload_region_at(xfd, 0l, addr<uint8 const?>(b.xf[0]), xf_bytes)
        let sc_bytes = int64(4 + wgs) * 4l
        let scd = make_device_buf(sc_bytes)
        upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
        let bufs = fixed_array(wqd, wsd, scd, xqd, xsd, yd)
        let sizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes)
        let gbits = fixed_array(1u, 2u, 0u, 4u, 8u, 16u)
        var sc = set_q8_batch_mm_a_cls(bufs, sizes, gbits)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var hz : VkHaz
        var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
        for (_k in range(DISPATCHES)) {
            enc_q8_batch_mm_a_cls(raw, hz, sc, pc, int64(wgs))
        }
        vk_check(vkEndCommandBuffer(raw), null)
        per_us = time_submits(raw)
        if (g_gpu.has_coopmat2) {
            // the cm2 tiles at the serving no-split config (f16-fed, decode-in-load)
            let cbufs = fixed_array(wqd, wsd, scd, xfd, yd)
            let csizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes)
            let cgbits = fixed_array(1u, 2u, 0u, 4u, 16u)
            var scl = set_q8_batch_cm2l_cls(cbufs, csizes, cgbits)
            var rawl = alloc_cmd()
            vk_check(vkBeginCommandBuffer(rawl, begin), null)
            var hzl : VkHaz
            for (_k in range(DISPATCHES)) {
                enc_q8_batch_cm2l_cls(rawl, hzl, scl, pc, int64(lwgs))
            }
            vk_check(vkEndCommandBuffer(rawl), null)
            cm2l_us = time_submits(rawl)
            var scm = set_q8_batch_cm2m_cls(cbufs, csizes, cgbits)
            var rawm = alloc_cmd()
            vk_check(vkBeginCommandBuffer(rawm, begin), null)
            var hzm : VkHaz
            for (_k in range(DISPATCHES)) {
                enc_q8_batch_cm2m_cls(rawm, hzm, scm, pc, int64(mwgs))
            }
            vk_check(vkEndCommandBuffer(rawm), null)
            cm2m_us = time_submits(rawm)
        }
    }
    let flop = 2.0lf * double(rows) * double(d) * double(n)
    let tflops = flop / (per_us * 1000000.0lf)
    let ms = per_us / 1000.0lf
    print("{name}: d={d} K={n} cnt={cnt} wgs={wgs}  {ms} ms/dispatch  {tflops} TFLOP/s\n")
    if (cm2l_us > 0.0lf) {
        let ltf = flop / (cm2l_us * 1000000.0lf)
        let mtf = flop / (cm2m_us * 1000000.0lf)
        print("{name}: cm2l wgs={lwgs}  {cm2l_us / 1000.0lf} ms/dispatch  {ltf} TFLOP/s\n")
        print("{name}: cm2m wgs={mwgs}  {cm2m_us / 1000.0lf} ms/dispatch  {mtf} TFLOP/s\n")
    }
    delete sched
    delete b
}

// the decode-cost bisect: the shipped cm2l against the three probe variants, one shape
def private run_cm2x_shape(name : string; d, n, cnt : int) {   // nolint:STYLE038 — one linear measurement sweep
    let nbb = n / 32
    let rows = cnt
    let totblk = d * nbb
    var b : ShapeBufs
    fill_fixture(b, totblk, rows, nbb)
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    let xf_bytes = int64(rows * nbb) * 64l
    let y_bytes = int64(rows * d) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let xfd = make_device_buf(xf_bytes)
    let yd = make_device_buf(y_bytes)
    let lwgs = ((cnt + 255) / 256) * ((d + 127) / 128)
    var sched : array<uint>
    sched |> resize(4 + lwgs)
    sched[2] = uint(cnt)
    let flop = 2.0lf * double(rows) * double(d) * double(n)
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(b.wq[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(b.ws[0]), ws_bytes)
        upload_region_at(xfd, 0l, addr<uint8 const?>(b.xf[0]), xf_bytes)
        let sc_bytes = int64(4 + lwgs) * 4l
        let scd = make_device_buf(sc_bytes)
        upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
        let cbufs = fixed_array(wqd, wsd, scd, xfd, yd)
        let csizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes)
        let cgbits = fixed_array(1u, 2u, 0u, 4u, 16u)
        var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
        let wfd = make_device_buf(wq_bytes * 2l)
        upload_region_at(wfd, 0l, addr<uint8 const?>(b.xf[0]), min(xf_bytes, wq_bytes * 2l))
        let fbufs = fixed_array(wfd, scd, xfd, yd)
        let fsizes = fixed_array(wq_bytes * 2l, sc_bytes, xf_bytes, y_bytes)
        let fgbits = fixed_array(1u, 0u, 4u, 16u)
        let vnames = fixed_array("cm2l", "lit ", "flat", "nil ", "f16a", "nobar", "mmnb ", "push")
        let mm_wgs = ((cnt + 127) / 128) * ((d + 127) / 128)
        var mmsched : array<uint>
        mmsched |> resize(4 + mm_wgs)
        mmsched[2] = uint(cnt)
        let msc_bytes = int64(4 + mm_wgs) * 4l
        let mscd = make_device_buf(msc_bytes)
        upload_region_at(mscd, 0l, addr<uint8 const?>(mmsched[0]), msc_bytes)
        let xq_bytes = int64(rows * nbb) * 32l
        let xs_bytes = int64(rows * nbb) * 4l
        let xqd = make_device_buf(xq_bytes)
        let xsd = make_device_buf(xs_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(b.xq[0]), xq_bytes)
        upload_region_at(xsd, 0l, addr<uint8 const?>(b.xs[0]), xs_bytes)
        delete mmsched
        for (v in range(8)) {
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var hz : VkHaz
            if (v == 0) {
                var s = set_q8_batch_cm2l_cls(cbufs, csizes, cgbits)
                for (_k in range(DISPATCHES)) {
                    enc_q8_batch_cm2l_cls(raw, hz, s, pc, int64(lwgs))
                }
            } elif (v == 1) {
                var s = set_cm2px_lit(cbufs, csizes, cgbits)
                for (_k in range(DISPATCHES)) {
                    enc_cm2px_lit(raw, hz, s, pc, int64(lwgs))
                }
            } elif (v == 2) {
                var s = set_cm2px_flat(cbufs, csizes, cgbits)
                for (_k in range(DISPATCHES)) {
                    enc_cm2px_flat(raw, hz, s, pc, int64(lwgs))
                }
            } elif (v == 3) {
                var s = set_cm2px_nil(cbufs, csizes, cgbits)
                for (_k in range(DISPATCHES)) {
                    enc_cm2px_nil(raw, hz, s, pc, int64(lwgs))
                }
            } elif (v == 4) {
                var s = set_cm2px_f16a(fbufs, fsizes, fgbits)
                for (_k in range(DISPATCHES)) {
                    enc_cm2px_f16a(raw, hz, s, pc, int64(lwgs))
                }
            } elif (v == 5) {
                // no-barrier arm: a fresh hazard rail per enc drops the y write-write barrier,
                // so consecutive dispatches overlap on device - sizes the per-dispatch drain
                var s = set_q8_batch_cm2l_cls(cbufs, csizes, cgbits)
                for (_k in range(DISPATCHES)) {
                    var hz2 : VkHaz
                    enc_q8_batch_cm2l_cls(raw, hz2, s, pc, int64(lwgs))
                }
            } elif (v == 6) {
                // the serving mm kernel, same no-barrier treatment - the fair kernel-rate twin
                var s = set_q8_batch_mm_a_cls(
                    fixed_array(wqd, wsd, mscd, xqd, xsd, yd),
                    fixed_array(wq_bytes, ws_bytes, msc_bytes, xq_bytes, xs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                for (_k in range(DISPATCHES)) {
                    var hz2 : VkHaz
                    enc_q8_batch_mm_a_cls(raw, hz2, s, pc, int64(mm_wgs))
                }
            } else {
                var s = set_cm2px_push(cbufs, csizes, cgbits)
                for (_k in range(DISPATCHES)) {
                    enc_cm2px_push(raw, hz, s, pc, int64(lwgs))
                }
            }
            vk_check(vkEndCommandBuffer(raw), null)
            let us = time_submits(raw)
            let tf = flop / (us * 1000000.0lf)
            print("{name} {vnames[v]}: {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s\n")
        }
    }
    delete sched
    delete b
}

[vk_dispatch(name = "cm2px_f16a", grid = "wgs", params = "wgs : int64")]
class Cm2ProbeF16A : MoeCmBase {
    @ssbo @binding = 0 @readonly wf : array<float16>
    @ssbo @binding = 3 @readonly xf16 : array<float16>
    @ssbo @binding = 5 y : array<float>

    [spirv_kernel(local_size_x = 256, name = "cm2px_f16a_spv")]
    def run {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 255u) / 256u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        let t0 = row0 + xt * 256u
        let m0 = wt * 128u
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetDimension(fla, pa.d, pa.n)
        tensorLayoutSetStride(fla, pa.n & ~7u, 1u)
        var flb : tensorLayout2D
        tensorLayoutCreate(flb)
        tensorLayoutSetDimension(flb, row0 + cnt, pa.n)
        tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
        var flo : tensorLayout2D
        tensorLayoutCreate(flo)
        tensorLayoutSetDimension(flo, row0 + cnt, pa.d)
        tensorLayoutSetStride(flo, pa.d & ~7u, 1u)
        var k = 0u
        for (_i in range(int(pa.n / 512u))) {
            for [unroll] (_j in range(8)) {
                coopmatLoadTensor(a, wf, 0u, fla, m0, 128u, k, 64u)
                coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
        }
        while (k < pa.n) {
            coopmatLoadTensor(a, wf, 0u, fla, m0, 128u, k, 64u)
            coopmatLoadTensor(b, xf16, 0u, flb, t0, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, 0u, flo, t0, 256u, m0, 128u, tv)
    }
}

// ===== their mul_mm_cm2 blob in our harness (arg "their") - the probe class only shapes the
// pipeline layout (3 SSBOs + their 17-word push block); DASLLAMA_VK_SPV_OVERRIDE serves the
// glslc-built, spec-const-patched llama.cpp shader in place of the dummy body =====

struct TheirArgs {
    m : uint
    n : uint
    k : uint
    sa : uint
    sb : uint
    sd : uint
    bsa : uint
    bsb : uint
    bsd : uint
    bz : uint
    nb : uint
    ks : uint
    ne02 : uint
    ne12 : uint
    b2 : uint
    b3 : uint
    pn : uint
}

[vk_dispatch(name = "cm2px_their", grid = "wgs", params = "wgs : int64")]
class Cm2ProbeTheir {
    @ssbo @binding = 0 @readonly aq : array<uint>
    @ssbo @binding = 1 @readonly bx : array<float16>
    @ssbo @binding = 2 dy : array<float>
    @push_constant pa : TheirArgs

    [spirv_kernel(local_size_x = 256, name = "cm2px_their_spv")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.m) {
            dy[i] = float(bx[i]) + float(aq[i])
        }
    }
}

[vk_dispatch(name = "cm2px_their2", grid = "wgs", params = "wgs : int64")]
class Cm2ProbeTheir2 {
    @ssbo @binding = 0 @readonly aq : array<uint>
    @ssbo @binding = 1 @readonly bx : array<float16>
    @ssbo @binding = 2 dy : array<float>
    @ssbo @binding = 3 @readonly sc : array<float16>
    @push_constant pa : TheirArgs

    [spirv_kernel(local_size_x = 256, name = "cm2px_their2_spv")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.m) {
            dy[i] = float(bx[i]) + float(aq[i]) + float(sc[i])
        }
    }
}

// the minimal our-emitter kernel: fast-path loop only, 256 rows literal, wg.x = weight tile,
// two-plane decode without the workgroup base (the T2-proven form). Recovery here pins the
// cm2l deficit to the preamble/branch structure; no recovery pins it to per-op emission
[vk_dispatch(name = "cm2px_min", grid = "wgs", params = "wgs : int64")]
class Cm2ProbeMin {
    @ssbo @binding = 0 @readonly wq : array<VkQ8Blk>
    @ssbo @binding = 1 @readonly wsh : array<float16>
    @ssbo @binding = 3 @readonly xf16 : array<float16>
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : BatchArgs

    [spirv_decode]
    def decode_q8(blk : VkQ8Blk; bc, cib : uint2) : float16 {
        let q = unpack8(blk.qs[int((cib.y & 30u) >> 1u)])[int(cib.y & 1u)]
        return wsh[bc.x * (pa.n >> 5u) + bc.y] * float16(float(int(q)))
    }

    [spirv_kernel(local_size_x = 256, name = "cm2px_min_spv")]
    def run {
        let m0 = gl_WorkGroupID.x * 128u
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 32u)
        tensorLayoutSetDimension(fla, pa.d, pa.n)
        tensorLayoutSetStride(fla, pa.n >> 5u, 1u)
        var flb : tensorLayout2D
        tensorLayoutCreate(flb)
        tensorLayoutSetDimension(flb, 256u, pa.n)
        tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
        var flo : tensorLayout2D
        tensorLayoutCreate(flo)
        tensorLayoutSetDimension(flo, 256u, pa.d)
        tensorLayoutSetStride(flo, pa.d & ~7u, 1u)
        var k = 0u
        for (_i in range(int(pa.n / 512u))) {
            for [unroll] (_j in range(8)) {
                coopmatLoadTensorDecode(a, wq, 0u, fla, m0, 128u, k, 64u, self.decode_q8)
                coopmatLoadTensor(b, xf16, 0u, flb, 0u, 256u, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
        }
        while (k < pa.n) {
            coopmatLoadTensorDecode(a, wq, 0u, fla, m0, 128u, k, 64u, self.decode_q8)
            coopmatLoadTensor(b, xf16, 0u, flb, 0u, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, 0u, flo, 0u, 256u, m0, 128u, tv)
    }
}

// gate shape at cnt = 256 (their grid is 2-D; one BN=256 column tile keeps it 1-D):
// interleaved block_q8_0 A plane, drain-free timing, our cm2l + mm rows alongside
def private run_their(d, n, cnt : int) {   // nolint:STYLE038 — one linear measurement sweep
    let nbb = n / 32
    let totblk = d * nbb
    var aw : array<uint16>
    aw |> resize(totblk * 17)
    for (blk in range(totblk)) {
        aw[blk * 17] = uint16(0x2c00u + (uint(blk) % 64u))
        for (w in range(16)) {
            let h = hash_word(uint(blk * 16 + w) + 3u)
            aw[blk * 17 + 1 + w] = uint16(h & 0xffffu)
        }
    }
    var xf : array<uint>
    xf |> resize(cnt * nbb * 16)
    for (i in range(cnt * nbb * 16)) {
        xf[i] = ws_word(uint(i) * 5u + 11u)
    }
    let a_bytes = int64(totblk) * 34l
    let b_bytes = int64(cnt * nbb) * 64l
    let y_bytes = int64(cnt * d) * 4l
    let ad = make_device_buf(a_bytes)
    let bd = make_device_buf(b_bytes)
    let yd = make_device_buf(y_bytes)
    let flop = 2.0lf * double(cnt) * double(d) * double(n)
    let wgs = (d + 127) / 128
    unsafe {
        upload_region_at(ad, 0l, addr<uint8 const?>(aw[0]), a_bytes)
        upload_region_at(bd, 0l, addr<uint8 const?>(xf[0]), b_bytes)
        var pc = TheirArgs(m = uint(d), n = uint(cnt), k = uint(n),
            sa = uint(n), sb = uint(n), sd = uint(d),
            bsa = uint(d) * uint(n), bsb = uint(cnt) * uint(n), bsd = uint(cnt) * uint(d),
            bz = 0u, nb = 1u, ks = uint(n), ne02 = 1u, ne12 = 1u, b2 = 1u, b3 = 1u, pn = uint(cnt))
        var scw : array<uint>
        scw |> resize(totblk / 2 + 1)
        for (i in range(totblk / 2 + 1)) {
            scw[i] = ws_word(uint(i) + 7u)
        }
        let sc_bytes = int64(totblk) * 2l
        let scd = make_device_buf(sc_bytes)
        upload_region_at(scd, 0l, addr<uint8 const?>(scw[0]), sc_bytes)
        delete scw
        var s = set_cm2px_their(
            fixed_array(ad, bd, yd),
            fixed_array(a_bytes, b_bytes, y_bytes),
            fixed_array(1u, 2u, 4u))
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        for (_k in range(DISPATCHES)) {
            var hz : VkHaz
            enc_cm2px_their(raw, hz, s, pc, int64(wgs))
        }
        vk_check(vkEndCommandBuffer(raw), null)
        let us = time_submits(raw)
        let tf = flop / (us * 1000000.0lf)
        print("their cm2 (d={d} K={n} cnt={cnt} wgs={wgs}): {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s\n")
        var s2 = set_cm2px_their2(
            fixed_array(ad, bd, yd, scd),
            fixed_array(a_bytes, b_bytes, y_bytes, sc_bytes),
            fixed_array(1u, 2u, 4u, 8u))
        var raw2 = alloc_cmd()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        for (_k in range(DISPATCHES)) {
            var hz : VkHaz
            enc_cm2px_their2(raw2, hz, s2, pc, int64(wgs))
        }
        vk_check(vkEndCommandBuffer(raw2), null)
        let us2 = time_submits(raw2)
        let tf2 = flop / (us2 * 1000000.0lf)
        print("their+2plane (T2): {us2 / 1000.0lf} ms/dispatch  {tf2} TFLOP/s\n")
        var s3 = set_cm2px_min(
            fixed_array(ad, scd, bd, yd),
            fixed_array(a_bytes, sc_bytes, b_bytes, y_bytes),
            fixed_array(1u, 2u, 4u, 8u))
        var raw3 = alloc_cmd()
        vk_check(vkBeginCommandBuffer(raw3, begin), null)
        var pcm = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
        for (_k in range(DISPATCHES)) {
            var hz : VkHaz
            enc_cm2px_min(raw3, hz, s3, pcm, int64(wgs))
        }
        vk_check(vkEndCommandBuffer(raw3), null)
        let us3 = time_submits(raw3)
        let tf3 = flop / (us3 * 1000000.0lf)
        print("our minimal (min): {us3 / 1000.0lf} ms/dispatch  {tf3} TFLOP/s\n")
    }
    delete aw
    delete xf
}

def private run_probe {   // nolint:STYLE037,STYLE038 — the flat per-arg shape dispatcher
    if (!ensure_q8_batch_cls()) {
        print("no Vulkan device\n")
        return
    }
    if (g_gpu.coopmat_mode < 3) {
        print("coopmat mode {g_gpu.coopmat_mode} < 3 - mm tiles not routed on this device\n")
        return
    }
    if (!ensure_q8_batch_mm_a_cls()) {
        print("mm_a pipeline failed\n")
        return
    }
    if (g_gpu.has_coopmat2) {
        if (!ensure_q8_batch_cm2l_cls() || !ensure_q8_batch_cm2m_cls()) {
            print("cm2 pipelines failed\n")
            return
        }
    }
    var only = ""
    for (a in get_command_line_arguments()) {
        if (a == "gate" || a == "down" || a == "q" || a == "kv" || a == "kvm" || a == "qkv" || a == "cm2x" || a == "their" || a == "tl" || a == "k4" || a == "k6" || a == "k6x") {
            only = a
        }
    }
    if (only == "k6x") {
        if (!g_gpu.has_coopmat2) {
            print("no coopmat2 on this device\n")
            return
        }
        verify(ensure_kq_batch_k6_cm2l_cls() && ensure_kq_batch_k6_cm2m_cls()
            && kq_batch_cls_ensure(int(KqFmt.k6), false)
            && ensure_k6px_nil() && ensure_k6px_flat() && ensure_k6px_ql()
            && ensure_k6px_pair() && ensure_k6px_pair16() && ensure_k6px_sfix(), "k6x probe pipelines must engage")
        run_k6_shape("q6k gate", 9728, 2560, 512, true)
        run_k6_shape("q6k down", 2560, 9728, 512, true)
        return
    }
    if (only == "k6") {
        if (!g_gpu.has_coopmat2) {
            print("no coopmat2 on this device\n")
            return
        }
        verify(ensure_kq_batch_k6_cm2l_cls() && ensure_kq_batch_k6_cm2m_cls()
            && kq_batch_cls_ensure(int(KqFmt.k6), false), "k6 probe pipelines must engage")
        run_k6_shape("q6k gate", 9728, 2560, 512)
        run_k6_shape("q6k down", 2560, 9728, 512)
        run_k6_shape("q6k kv  ", 1024, 2560, 512)
        return
    }
    if (only == "k4") {
        // Qwen3-4B role shapes (dim 2560, hidden 9728, qd 4096, kvd 1024) - k4 cm2 vs the kq tile
        if (!g_gpu.has_coopmat2) {
            print("no coopmat2 on this device\n")
            return
        }
        verify(ensure_kq_batch_k4_cm2l_cls() && ensure_kq_batch_k4_cm2m_cls()
            && kq_batch_cls_ensure(int(KqFmt.k4), false), "k4 probe pipelines must engage")
        run_k4_shape("q4k gate", 9728, 2560, 512)
        run_k4_shape("q4k down", 2560, 9728, 512)
        run_k4_shape("q4k q/wo", 4096, 2560, 512)
        run_k4_shape("q4k kv  ", 2048, 2560, 512)
        return
    }
    if (only == "their") {
        if (!g_gpu.has_coopmat2) {
            print("no coopmat2 on this device\n")
            return
        }
        verify(ensure_cm2px_their() && ensure_cm2px_their2() && ensure_cm2px_min(), "their-blob pipelines must engage")
        run_their(8192, 3072, 256)
        return
    }
    if (only == "tl") {
        // tinyllama-1.1B role shapes (dim 2048, hidden 5632, kvd 256) - the small-model tile pick data
        run_shape("tl gate", 5632, 2048, 512)
        run_shape("tl down", 2048, 5632, 512)
        run_shape("tl q/wo", 2048, 2048, 512)
        run_shape("tl k/v ", 256, 2048, 512)
        return
    }
    if (only == "cm2x") {
        if (!g_gpu.has_coopmat2) {
            print("no coopmat2 on this device\n")
            return
        }
        verify(ensure_cm2px_nil() && ensure_cm2px_flat() && ensure_cm2px_lit() && ensure_cm2px_f16a()
            && ensure_cm2px_push(), "cm2 probe pipelines must engage")
        run_cm2x_shape("gate/up", 8192, 3072, 512)
        run_cm2x_shape("down   ", 3072, 8192, 512)
        return
    }
    if (only == "gate" || only == "") {
        run_shape("gate/up", 8192, 3072, 512)
    }
    if (only == "down" || only == "") {
        run_shape("down   ", 3072, 8192, 512)
    }
    if (only == "q" || only == "") {
        run_shape("q/wo   ", 3072, 3072, 512)
    }
    if (only == "kv" || only == "") {
        run_shape("k/v    ", 1024, 3072, 512)
    }
    if (only == "kvm" || only == "") {
        run_shape("k+v mrg", 2048, 3072, 512)
    }
    if (only == "qkv" || only == "") {
        run_shape("qkv mrg", 5120, 3072, 512)
    }
}

[export]
def main {
    run_probe()
}
