options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - isolated per-shape rates of the prefill GEMM tiles.
// RefGemmArgs mirrors that blob's push block byte for byte - its constant fields are dead by design.
// Every arm records once, then times over repeated submits; the serving kernels' oracle gate
// is tests/test_vulkan_kernels.das.
// Args: gate|down|q|kv|kvm|qkv|tl pin a shape (the long-window ngfx GPU Trace vehicle);
// k4|k6 the K-quant tiles; cm2:<fmt> any kq format's l/m tiles; cm2x|k6x|ref the bisect and
// reference arms; khrx the KHR kq tile's weight-stage and structural bisect; mmqx the integer tile
// prototypes.
// Shapes default to Llama-3.2-3B Q8 geometry (dim 3072, hidden 8192, kv_dim 1024).

require math
require strings
require daslib/enum_trait
require vulkan
require vulkan/vulkan_boost
require spirv/spirv_shader
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_prefill
require dasllama/dasllama_kqformat
require daslib/typemacro_boost
require _vk_probe_fixture   // hash_word, cool_f16_pair: the synthetic planes both probes build
require ../tests/_vkd_oracles.das   // k4f16_gemm_oracle: the CPU reference the khrx baseline arm is checked against

let private MM_TILE = 128
let private DISPATCHES = 16
let private SUBMITS = 100
let private ROUNDS = 4   //! the interleaved sweeps: the arms alternate per round, SUBMITS / ROUNDS timed submits each, best-of across rounds

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
        b.ws[i] = cool_f16_pair(uint(i) + 7u)
    }
    for (i in range(rows * nbb * 8)) {
        b.xq[i] = hash_word(uint(i) * 13u + 1u)
    }
    for (i in range(rows * nbb)) {
        // cool inputs: the mm family accumulates in f16, hot fixtures overflow 65504
        b.xs[i] = (0.375 + float(i % 9) * 0.0625) * 0.0625
    }
    for (i in range(rows * nbb * 16)) {
        b.xf[i] = cool_f16_pair(uint(i) * 5u + 11u)
    }
}

// ===== cm2 decode-cost bisect (arg "cm2x") - the shipped l-tile body with the decode method
// varied: nil = constant A (zero decode cost ceiling), flat = quant only (no scale-plane read),
// lit = full decode minus the wg_blk0 shared-memory read. Fast-path no-split arm only =====

[vk_dispatch(name = "cm2px_nil", grid = "wgs", params = "wgs : int64")]
class Cm2PxNil : MoeCmBase {
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
class Cm2PxFlat : MoeCmBase {
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
class Cm2PxPush : MoeCmBase {
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
class Cm2PxLit : MoeCmBase {
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

def private cm2_fmt_names : string {
    return build_string() $(var w) {
        var first = true
        for (f in type<KqFmt>) {
            if (kq_sb(f)) {
                w |> write("{first ? "" : " "}{f}")
                first = false
            }
        }
    }
}

[arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
def private run_cm2_fmt_shape(fmt : int; name : string; d, n, cnt : int) {   // nolint:STYLE038 — one linear measurement sweep
    let nsb = n / 256
    let totsb = d * nsb
    let bb = arena_block_bytes(fmt)
    let wq_words = int(bb.wq / 4l)
    let ws_words = int(bb.ws / 4l)
    var wqh : array<uint>
    var wsuh : array<uint>
    var xfh : array<uint>
    var xqh : array<uint>
    var axsh : array<float>
    wqh |> resize(totsb * wq_words)
    wsuh |> resize(totsb * ws_words)
    xfh |> resize(cnt * n / 2)
    xqh |> resize(cnt * nsb * 64)
    axsh |> resize(cnt * nsb)
    for (i in range(totsb * wq_words)) {
        wqh[i] = hash_word(uint(i) + 29u)
    }
    for (sb in range(totsb)) {
        wsuh[sb * ws_words] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.00005 * float(1 + sb % 5)))
        for (wi in range(1, ws_words)) {
            wsuh[sb * ws_words + wi] = hash_word(uint(sb * ws_words + wi) + 613u)
        }
    }
    for (i in range(cnt * n / 2)) {
        xfh[i] = cool_f16_pair(uint(i) * 3u + 5u)
    }
    for (i in range(cnt * nsb * 64)) {
        xqh[i] = hash_word(uint(i) * 13u + 1u)
    }
    for (i in range(cnt * nsb)) {
        axsh[i] = 0.0625
    }
    let wq_bytes = int64(totsb) * bb.wq
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
        let vnames = fixed_array("cm2l", "cm2m", "kq  ")
        for (v in range(3)) {
            let ttile = v == 0 ? 256 : (v == 1 ? 128 : 32)
            let wtile = v == 2 ? 32 : 128
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
            if (v == 2) {
                var s = kq_batch_cls_set_for(fmt, false,
                    fixed_array(wqd, wsd, scd, xqd, axsd, yd),
                    fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, axs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                for (_k in range(DISPATCHES)) {
                    var hz2 : VkHaz
                    kq_batch_cls_enc_for(fmt, false, raw, hz2, s, pc, int64(wgs))
                }
            } else {
                var s = cm2_cls_set(fmt, int64(ttile), fixed_array(wqd, wsd, scd, xfd, yd),
                    fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 16u))
                for (_k in range(DISPATCHES)) {
                    cm2_cls_enc(fmt, int64(ttile), raw, hz, s, pc, int64(wgs))
                }
            }
            vk_check(vkEndCommandBuffer(raw), null)
            let us = usec_per_dispatch(raw)
            let tf = flop / (us * 1000000.0lf)
            print("{name} {vnames[v]}: {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s  timing-only\n")
        }
    }
    delete wqh
    delete wsuh
    delete xfh
    delete xqh
    delete axsh
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
        xfh[i] = cool_f16_pair(uint(i) * 3u + 5u)
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
        let vnames = fixed_array("k4l ", "k4m ", "k4lnb", "kqnb ")   // kqnb: the kq tile dispatches un-barriered, like k4lnb
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
            let us = usec_per_dispatch(raw)
            let tf = flop / (us * 1000000.0lf)
            print("{name} {vnames[v]}: {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s  timing-only\n")
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
// (no qh stream), pair = upstream's 16-bit-pair compose + ONE unpack8 select (v4 form),
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

let private KHRPX_STRIDE = 40u

class KhrPxBase : MoeCmBase {
    @ssbo @binding = 1 @role = "alias" @readonly ws : array<uint>
    @ssbo @binding = 3 @role = "alias" @readonly xf16w4 : array<uint4>
    @ssbo @binding = 5 y : array<float>
    @workgroup wg_blk0 : uint
    @workgroup khr_a : float16[5120]
    @workgroup khr_b : float16[5120]

    [spirv_decode]
    def decode_v4(blk : VkK4Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let li = int((g * 16u + (e & 15u)) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dm = unpackHalf2x16(ws[srow])
        let ds = dm.x * float((ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu)
        let dmn = dm.y * float((ws[srow + 3u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu)
        return half4(float4(ds * float((l0 >> sh) & 0xFu) - dmn, ds * float((l0 >> (sh + 8u)) & 0xFu) - dmn,
            ds * float((l1 >> sh) & 0xFu) - dmn, ds * float((l1 >> (sh + 8u)) & 0xFu) - dmn))
    }

    //! an override fills khr_a[sbase .. sbase + 16) with weight row wrow's elements e0 .. e0 + 16 of block bcol
    def stage_a(wblk0, wrow, bcol, e0, sbase : uint) {
        for (j in range(16)) {
            khr_a[sbase + uint(j)] = float16(0.0625)
        }
    }

    //! an override fills khr_b[sbase .. sbase + 16) from the activation plane at xb; this is the shipped stage, two 16-byte words
    def stage_b(xb, sbase : uint) {
        let q = xb >> 3u   //! xb is a multiple of 16 halves
        for (h in range(2)) {
            let w4 = xf16w4[q + uint(h)]
            for (c in range(4)) {
                let p = unpackHalf2x16(w4[c])
                let o = sbase + uint(h) * 8u + uint(c) * 2u
                khr_b[o] = float16(p.x)
                khr_b[o + 1u] = float16(p.y)
            }
        }
    }

    def body_f32a {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 127u) / 128u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        barrier()
        let t0 = row0 + xt * 128u
        let m0 = wt * 128u
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let srow = tid >> 1u
        let skh = (tid & 1u) * 16u
        let sbase = srow * KHRPX_STRIDE + skh
        let wrow = m0 + srow
        let wrow0 = (sg & 1u) * 64u
        let trow0 = (sg >> 1u) * 32u
        var acc00, acc01, acc10, acc11, acc20, acc21, acc30, acc31 : coopmatAcc_f32_16x16
        var k = 0u
        while (k < pa.n) {
            let kk = k + skh
            let bcol = kk >> 8u
            stage_a(wblk0, wrow, bcol, kk - bcol * 256u, sbase)
            stage_b((t0 + srow) * pa.n + kk, sbase)
            barrier()
            for (ks in range(2)) {
                let ko = uint(ks) * 16u
                var a0, a1, a2, a3 : coopmatA_f16_16x16
                coopmatLoad(a0, khr_a, int(wrow0 * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a1, khr_a, int((wrow0 + 16u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a2, khr_a, int((wrow0 + 32u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a3, khr_a, int((wrow0 + 48u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                var b0, b1 : coopmatB_f16_16x16
                coopmatLoad(b0, khr_b, int(trow0 * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                coopmatLoad(b1, khr_b, int((trow0 + 16u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc00 = coopmatMulAdd(a0, b0, acc00)
                acc01 = coopmatMulAdd(a0, b1, acc01)
                acc10 = coopmatMulAdd(a1, b0, acc10)
                acc11 = coopmatMulAdd(a1, b1, acc11)
                acc20 = coopmatMulAdd(a2, b0, acc20)
                acc21 = coopmatMulAdd(a2, b1, acc21)
                acc30 = coopmatMulAdd(a3, b0, acc30)
                acc31 = coopmatMulAdd(a3, b1, acc31)
            }
            barrier()
            k += 32u
        }
        let yb = (t0 + trow0) * pa.d + m0 + wrow0
        coopmatStore(acc00, y, int(yb), int(pa.d), 1)
        coopmatStore(acc01, y, int(yb + 16u * pa.d), int(pa.d), 1)
        coopmatStore(acc10, y, int(yb + 16u), int(pa.d), 1)
        coopmatStore(acc11, y, int(yb + 16u * pa.d + 16u), int(pa.d), 1)
        coopmatStore(acc20, y, int(yb + 32u), int(pa.d), 1)
        coopmatStore(acc21, y, int(yb + 16u * pa.d + 32u), int(pa.d), 1)
        coopmatStore(acc30, y, int(yb + 48u), int(pa.d), 1)
        coopmatStore(acc31, y, int(yb + 16u * pa.d + 48u), int(pa.d), 1)
    }

    def body_strip {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 127u) / 128u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        barrier()
        let t0 = row0 + xt * 128u
        let m0 = wt * 128u
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let srow = tid >> 1u
        let skh = (tid & 1u) * 16u
        let sbase = srow * KHRPX_STRIDE + skh
        let wrow = m0 + srow
        var acc0, acc1, acc2, acc3, acc4, acc5, acc6, acc7 : coopmatAcc_f16_16x16
        var k = 0u
        while (k < pa.n) {
            let kk = k + skh
            let bcol = kk >> 8u
            stage_a(wblk0, wrow, bcol, kk - bcol * 256u, sbase)
            stage_b((t0 + srow) * pa.n + kk, sbase)
            barrier()
            for (ks in range(2)) {
                let ko = uint(ks) * 16u
                var a : coopmatA_f16_16x16
                coopmatLoad(a, khr_a, int(sg * 16u * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                var b : coopmatB_f16_16x16
                coopmatLoad(b, khr_b, int(ko), int(KHRPX_STRIDE), 1)
                acc0 = coopmatMulAdd(a, b, acc0)
                coopmatLoad(b, khr_b, int(16u * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc1 = coopmatMulAdd(a, b, acc1)
                coopmatLoad(b, khr_b, int(32u * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc2 = coopmatMulAdd(a, b, acc2)
                coopmatLoad(b, khr_b, int(48u * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc3 = coopmatMulAdd(a, b, acc3)
                coopmatLoad(b, khr_b, int(64u * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc4 = coopmatMulAdd(a, b, acc4)
                coopmatLoad(b, khr_b, int(80u * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc5 = coopmatMulAdd(a, b, acc5)
                coopmatLoad(b, khr_b, int(96u * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc6 = coopmatMulAdd(a, b, acc6)
                coopmatLoad(b, khr_b, int(112u * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc7 = coopmatMulAdd(a, b, acc7)
            }
            barrier()
            k += 32u
        }
        let yb = t0 * pa.d + m0 + sg * 16u
        var accf32 : coopmatAcc_f32_16x16
        coopmatConvert(accf32, acc0)
        coopmatStore(accf32, y, int(yb), int(pa.d), 1)
        coopmatConvert(accf32, acc1)
        coopmatStore(accf32, y, int(yb + 16u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc2)
        coopmatStore(accf32, y, int(yb + 32u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc3)
        coopmatStore(accf32, y, int(yb + 48u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc4)
        coopmatStore(accf32, y, int(yb + 64u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc5)
        coopmatStore(accf32, y, int(yb + 80u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc6)
        coopmatStore(accf32, y, int(yb + 96u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc7)
        coopmatStore(accf32, y, int(yb + 112u * pa.d), int(pa.d), 1)
    }

    [arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
    def body {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 127u) / 128u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        barrier()
        let t0 = row0 + xt * 128u
        let m0 = wt * 128u
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let srow = tid >> 1u
        let skh = (tid & 1u) * 16u
        let sbase = srow * KHRPX_STRIDE + skh
        let wrow = m0 + srow
        let wrow0 = (sg & 1u) * 64u
        let trow0 = (sg >> 1u) * 32u
        var acc00, acc01, acc10, acc11, acc20, acc21, acc30, acc31 : coopmatAcc_f16_16x16
        var k = 0u
        while (k < pa.n) {
            let kk = k + skh
            let bcol = kk >> 8u
            stage_a(wblk0, wrow, bcol, kk - bcol * 256u, sbase)
            stage_b((t0 + srow) * pa.n + kk, sbase)
            barrier()
            for (ks in range(2)) {
                let ko = uint(ks) * 16u
                var a0, a1, a2, a3 : coopmatA_f16_16x16
                coopmatLoad(a0, khr_a, int(wrow0 * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a1, khr_a, int((wrow0 + 16u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a2, khr_a, int((wrow0 + 32u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a3, khr_a, int((wrow0 + 48u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                var b0, b1 : coopmatB_f16_16x16
                coopmatLoad(b0, khr_b, int(trow0 * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                coopmatLoad(b1, khr_b, int((trow0 + 16u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc00 = coopmatMulAdd(a0, b0, acc00)
                acc01 = coopmatMulAdd(a0, b1, acc01)
                acc10 = coopmatMulAdd(a1, b0, acc10)
                acc11 = coopmatMulAdd(a1, b1, acc11)
                acc20 = coopmatMulAdd(a2, b0, acc20)
                acc21 = coopmatMulAdd(a2, b1, acc21)
                acc30 = coopmatMulAdd(a3, b0, acc30)
                acc31 = coopmatMulAdd(a3, b1, acc31)
            }
            barrier()
            k += 32u
        }
        let yb = (t0 + trow0) * pa.d + m0 + wrow0
        var accf32 : coopmatAcc_f32_16x16
        coopmatConvert(accf32, acc00)
        coopmatStore(accf32, y, int(yb), int(pa.d), 1)
        coopmatConvert(accf32, acc01)
        coopmatStore(accf32, y, int(yb + 16u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc10)
        coopmatStore(accf32, y, int(yb + 16u), int(pa.d), 1)
        coopmatConvert(accf32, acc11)
        coopmatStore(accf32, y, int(yb + 16u * pa.d + 16u), int(pa.d), 1)
        coopmatConvert(accf32, acc20)
        coopmatStore(accf32, y, int(yb + 32u), int(pa.d), 1)
        coopmatConvert(accf32, acc21)
        coopmatStore(accf32, y, int(yb + 16u * pa.d + 32u), int(pa.d), 1)
        coopmatConvert(accf32, acc30)
        coopmatStore(accf32, y, int(yb + 48u), int(pa.d), 1)
        coopmatConvert(accf32, acc31)
        coopmatStore(accf32, y, int(yb + 16u * pa.d + 48u), int(pa.d), 1)
    }
}

[vk_dispatch(name = "khrpx_nil", grid = "wgs", params = "wgs : int64")]
class KhrPxNil : KhrPxBase {
    @ssbo @binding = 0 @role = "alias" @readonly wq : array<VkK4Blk>

    [spirv_kernel(local_size_x = 256, name = "khrpx_nil_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#khrx-shared-set-layout")]
    def run {
        body()
    }
}

[vk_dispatch(name = "khrpx_dec4", grid = "wgs", params = "wgs : int64")]
class KhrPxDec4 : KhrPxBase {
    @ssbo @binding = 0 @readonly wq : array<VkK4Blk>

    def override stage_a(wblk0, wrow, bcol, e0, sbase : uint) {
        for (j in range(4)) {
            let v4 = decode_v4(wq[wblk0 + wrow * (pa.n >> 8u) + bcol], uint2(wrow, bcol), uint2(0u, e0 + uint(j) * 4u))
            let o = sbase + uint(j) * 4u
            khr_a[o] = v4.x
            khr_a[o + 1u] = v4.y
            khr_a[o + 2u] = v4.z
            khr_a[o + 3u] = v4.w
        }
    }

    [spirv_kernel(local_size_x = 256, name = "khrpx_dec4_spv")]
    def run {
        body()
    }
}

//! the shipped tile's weight stage, copied: every lever arm below inherits it unmoved
class KhrPxW4Base : KhrPxBase {
    @ssbo @binding = 0 @readonly wq4 : array<uint4>

    def override stage_a(wblk0, wrow, bcol, e0, sbase : uint) {
        let g = e0 >> 5u
        let sh = ((e0 >> 4u) & 1u) * 4u
        let w4 = wq4[(wblk0 + wrow * (pa.n >> 8u) + bcol) * 8u + g]
        let srow = (wg_blk0 + wrow * (pa.n >> 8u) + bcol) * 5u
        let dm = unpackHalf2x16(ws[srow])
        let ds = dm.x * float((ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu)
        let dmn = dm.y * float((ws[srow + 3u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu)
        for [unroll_full] (c in range(4)) {
            let w = w4[c]
            let o = sbase + uint(c) * 4u
            khr_a[o] = float16(ds * float((w >> sh) & 0xFu) - dmn)
            khr_a[o + 1u] = float16(ds * float((w >> (sh + 8u)) & 0xFu) - dmn)
            khr_a[o + 2u] = float16(ds * float((w >> (sh + 16u)) & 0xFu) - dmn)
            khr_a[o + 3u] = float16(ds * float((w >> (sh + 24u)) & 0xFu) - dmn)
        }
    }
}

//! the shipped tile, copied: the control row every lever's arm is timed against
[vk_dispatch(name = "khrpx_ship", grid = "wgs", params = "wgs : int64")]
class KhrPxShip : KhrPxW4Base {
    [spirv_kernel(local_size_x = 256, name = "khrpx_ship_spv")]
    def run {
        body()
    }
}

[vk_dispatch(name = "khrpx_f32a", grid = "wgs", params = "wgs : int64")]
class KhrPxF32a : KhrPxW4Base {
    [spirv_kernel(local_size_x = 256, name = "khrpx_f32a_spv")]
    def run {
        body_f32a()
    }
}

[vk_dispatch(name = "khrpx_strip", grid = "wgs", params = "wgs : int64")]
class KhrPxStrip : KhrPxW4Base {
    [spirv_kernel(local_size_x = 256, name = "khrpx_strip_spv")]
    def run {
        body_strip()
    }
}

[vk_dispatch(name = "khrpx_wg128", grid = "wgs", params = "wgs : int64")]
class KhrPxWg128 : KhrPxW4Base {
    [spirv_kernel(local_size_x = 128, name = "khrpx_wg128_spv")]
    def run {   // nolint:STYLE038 — one measurement body, its sixteen accumulators named
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wblk0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let ttiles = (cnt + 127u) / 128u
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let xt = tix % ttiles
        let wt = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        barrier()
        let t0 = row0 + xt * 128u
        let m0 = wt * 128u
        let srow = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let sbase = srow * KHRPX_STRIDE
        let wrow = m0 + srow
        let wrow0 = (sg & 1u) * 64u
        let trow0 = (sg >> 1u) * 64u
        var acc00, acc01, acc02, acc03, acc10, acc11, acc12, acc13 : coopmatAcc_f16_16x16
        var acc20, acc21, acc22, acc23, acc30, acc31, acc32, acc33 : coopmatAcc_f16_16x16
        var k = 0u
        while (k < pa.n) {
            let bcol = k >> 8u
            let e0 = k - bcol * 256u
            stage_a(wblk0, wrow, bcol, e0, sbase)
            stage_a(wblk0, wrow, bcol, e0 + 16u, sbase + 16u)
            let xb = (t0 + srow) * pa.n + k
            stage_b(xb, sbase)
            stage_b(xb + 16u, sbase + 16u)
            barrier()
            for (ks in range(2)) {
                let ko = uint(ks) * 16u
                var a0, a1, a2, a3 : coopmatA_f16_16x16
                coopmatLoad(a0, khr_a, int(wrow0 * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a1, khr_a, int((wrow0 + 16u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a2, khr_a, int((wrow0 + 32u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                coopmatLoad(a3, khr_a, int((wrow0 + 48u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 0)
                var b0, b1, b2, b3 : coopmatB_f16_16x16
                coopmatLoad(b0, khr_b, int(trow0 * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                coopmatLoad(b1, khr_b, int((trow0 + 16u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                coopmatLoad(b2, khr_b, int((trow0 + 32u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                coopmatLoad(b3, khr_b, int((trow0 + 48u) * KHRPX_STRIDE + ko), int(KHRPX_STRIDE), 1)
                acc00 = coopmatMulAdd(a0, b0, acc00)
                acc01 = coopmatMulAdd(a0, b1, acc01)
                acc02 = coopmatMulAdd(a0, b2, acc02)
                acc03 = coopmatMulAdd(a0, b3, acc03)
                acc10 = coopmatMulAdd(a1, b0, acc10)
                acc11 = coopmatMulAdd(a1, b1, acc11)
                acc12 = coopmatMulAdd(a1, b2, acc12)
                acc13 = coopmatMulAdd(a1, b3, acc13)
                acc20 = coopmatMulAdd(a2, b0, acc20)
                acc21 = coopmatMulAdd(a2, b1, acc21)
                acc22 = coopmatMulAdd(a2, b2, acc22)
                acc23 = coopmatMulAdd(a2, b3, acc23)
                acc30 = coopmatMulAdd(a3, b0, acc30)
                acc31 = coopmatMulAdd(a3, b1, acc31)
                acc32 = coopmatMulAdd(a3, b2, acc32)
                acc33 = coopmatMulAdd(a3, b3, acc33)
            }
            barrier()
            k += 32u
        }
        let yb = (t0 + trow0) * pa.d + m0 + wrow0
        var accf32 : coopmatAcc_f32_16x16
        coopmatConvert(accf32, acc00)
        coopmatStore(accf32, y, int(yb), int(pa.d), 1)
        coopmatConvert(accf32, acc01)
        coopmatStore(accf32, y, int(yb + 16u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc02)
        coopmatStore(accf32, y, int(yb + 32u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc03)
        coopmatStore(accf32, y, int(yb + 48u * pa.d), int(pa.d), 1)
        coopmatConvert(accf32, acc10)
        coopmatStore(accf32, y, int(yb + 16u), int(pa.d), 1)
        coopmatConvert(accf32, acc11)
        coopmatStore(accf32, y, int(yb + 16u * pa.d + 16u), int(pa.d), 1)
        coopmatConvert(accf32, acc12)
        coopmatStore(accf32, y, int(yb + 32u * pa.d + 16u), int(pa.d), 1)
        coopmatConvert(accf32, acc13)
        coopmatStore(accf32, y, int(yb + 48u * pa.d + 16u), int(pa.d), 1)
        coopmatConvert(accf32, acc20)
        coopmatStore(accf32, y, int(yb + 32u), int(pa.d), 1)
        coopmatConvert(accf32, acc21)
        coopmatStore(accf32, y, int(yb + 16u * pa.d + 32u), int(pa.d), 1)
        coopmatConvert(accf32, acc22)
        coopmatStore(accf32, y, int(yb + 32u * pa.d + 32u), int(pa.d), 1)
        coopmatConvert(accf32, acc23)
        coopmatStore(accf32, y, int(yb + 48u * pa.d + 32u), int(pa.d), 1)
        coopmatConvert(accf32, acc30)
        coopmatStore(accf32, y, int(yb + 48u), int(pa.d), 1)
        coopmatConvert(accf32, acc31)
        coopmatStore(accf32, y, int(yb + 16u * pa.d + 48u), int(pa.d), 1)
        coopmatConvert(accf32, acc32)
        coopmatStore(accf32, y, int(yb + 32u * pa.d + 48u), int(pa.d), 1)
        coopmatConvert(accf32, acc33)
        coopmatStore(accf32, y, int(yb + 48u * pa.d + 48u), int(pa.d), 1)
    }
}

let private KHR_ARMS = 8
let private KHR_ARM_NAMES = fixed_array("nil   ", "ship  ", "dec4  ", "khr   ", "kq    ", "f32a  ", "strip ", "wg128 ")
let private KHR_ARM_KHR = 3   //! the shipped class: the compared arms' reference and the CPU check's subject
let private KHR_ARM_KQ = 4    //! the sdot4 kq tile, 32 x 32 where every other arm tiles 128 x 128
let private KHR_ARM_COMPARED = fixed_array(false, true, true, false, false, true, true, true)   //! khr is the reference; nil stages constants and kq reads its own Q8 activation fixture: timing-only
let private KHR_ARM_REL_BOUND = 2e-2lf   //! a compared arm passes within this relative difference of the shipped class; the copies read 0, the f32-accumulator arm 0.005-0.009

//! the bisect arms stage and store whole 128-row tiles - only the two shipped tiles clamp token rows to the window's count, so only they run on a partial window
def private khr_arm_clamps(v : int) : bool => v == KHR_ARM_KHR || v == KHR_ARM_KQ
let private MMQ_ARMS = 5
var private g_prof_arm = ""
var private g_probe_exit = 0   //! the process exit code: a decline, a compared arm over its bound, or a profiler run that dispatched nothing

//! the interleaved timing of one sweep's arms: after a two-submit warm-up each, every round times
//! SUBMITS / ROUNDS submits of every arm in turn, and an arm's figure is its best round
def private sweep_best_us(var raws : array<VkCommandBuffer>) : array<double> {
    var best : array<double>
    best |> resize(length(raws))
    for (raw, bv in raws, best) {
        bv = 1.0e30lf
        if (raw == null) {
            continue
        }
        submit_wait(raw)
        submit_wait(raw)
    }
    for (_r in range(ROUNDS)) {
        for (raw, bv in raws, best) {
            if (raw == null) {
                continue
            }
            let t0 = ref_time_ticks()
            for (_s in range(SUBMITS / ROUNDS)) {
                submit_wait(raw)
            }
            let us = double(get_time_usec(t0)) / double((SUBMITS / ROUNDS) * DISPATCHES)
            if (us < bv) {
                bv = us
            }
        }
    }
    return <- best
}

//! one more submit of an arm's command buffer, then the output buffer its last dispatch wrote read back into `out`
def private read_back_y(var raw : VkCommandBuffer; dev : uint64; bytes : int64; var out : array<float>) {
    submit_wait(raw)
    out |> resize(bytes / 4l)
    var host = make_host_buf(bytes, true, [cached = true])
    var rc = alloc_cmd()
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(rc, begin), null)
    cmd_copy_whole(rc, dev, host.buf, bytes)
    vk_check(vkEndCommandBuffer(rc), null)
    submit_wait(rc)
    unsafe {
        memcpy(addr<void?>(out[0]), host.mapped, bytes)
    }
}

def private max_rel_diff(a, b : array<float>) : double {
    var mx = 0.0
    var mref = 0.0
    for (x, y in a, b) {
        mx = max(mx, abs(x - y))
        mref = max(mref, abs(y))
    }
    return mref > 0.0 ? double(mx / mref) : 0.0lf
}

def private khr_arm_set(v, fmt : int; wqd, wsd, scd, xfd, xqd, axsd, yd : uint64; wq_bytes, ws_bytes, sc_bytes, xf_bytes, xq_bytes, axs_bytes, y_bytes : int64) : VkDescriptorSet {
    if (v == 4) {
        return kq_batch_cls_set_for(fmt, false, fixed_array(wqd, wsd, scd, xqd, axsd, yd),
            fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, axs_bytes, y_bytes), fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
    }
    let bufs = fixed_array(wqd, wsd, scd, xfd, yd)
    let sizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, y_bytes)
    let gbits = fixed_array(1u, 2u, 0u, 4u, 16u)
    if (v == 0) {
        return set_khrpx_nil(bufs, sizes, gbits)
    } elif (v == 1) {
        return set_khrpx_ship(bufs, sizes, gbits)
    } elif (v == 2) {
        return set_khrpx_dec4(bufs, sizes, gbits)
    } elif (v == 3) {
        return set_kq_batch_k4_khr_cls(bufs, sizes, gbits)
    } elif (v == 5) {
        return set_khrpx_f32a(bufs, sizes, gbits)
    } elif (v == 6) {
        return set_khrpx_strip(bufs, sizes, gbits)
    }
    verify(v == 7, "khrx: the arm table has eight arms")
    return set_khrpx_wg128(bufs, sizes, gbits)
}

def private khr_arm_enc(v, fmt : int; raw : VkCommandBuffer; var hz : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; wgs : int64) {
    if (v == 0) {
        enc_khrpx_nil(raw, hz, s, pc, wgs)
    } elif (v == 1) {
        enc_khrpx_ship(raw, hz, s, pc, wgs)
    } elif (v == 2) {
        enc_khrpx_dec4(raw, hz, s, pc, wgs)
    } elif (v == 3) {
        enc_kq_batch_k4_khr_cls(raw, hz, s, pc, wgs)
    } elif (v == 4) {
        kq_batch_cls_enc_for(fmt, false, raw, hz, s, pc, wgs)
    } elif (v == 5) {
        enc_khrpx_f32a(raw, hz, s, pc, wgs)
    } elif (v == 6) {
        enc_khrpx_strip(raw, hz, s, pc, wgs)
    } else {
        verify(v == 7, "khrx: the arm table has eight arms")
        enc_khrpx_wg128(raw, hz, s, pc, wgs)
    }
}

//! the baseline arm against the CPU oracle over six corners of the output - eight weight rows by sixteen tokens at the
//! first, middle and last rows, at the first and last tokens - on the kernel cell's bar: |gpu - cpu| <= 2e-2 |cpu| + 4e-3 max|y|
def private khr_cpu_check(wqh, wsuh, xfh : array<uint>; y_gpu : array<float>; d, n, cnt : int) : tuple<samples : int; off : int; max_abs : double> {
    let nsb = n / 256
    var ymax = 0.0
    for (v in y_gpu) {
        ymax = max(ymax, abs(v))
    }
    var samples = 0
    var off = 0
    var max_abs = 0.0lf
    for (r0 in [0, (d / 2) & ~7, d - 8]) {
        for (t0 in [0, cnt - 16]) {
            let recs = [uint(r0 * nsb), uint(t0), 16u, 0u]
            var ys : array<float>
            ys |> resize((t0 + 16) * 8)
            k4f16_gemm_oracle(wqh, wsuh, xfh, recs, 1, n, 8, ys)
            for (r in range(16)) {
                for (c in range(8)) {
                    let cpu = ys[(t0 + r) * 8 + c]
                    let gpu = y_gpu[(t0 + r) * d + r0 + c]
                    let diff = abs(gpu - cpu)
                    samples++
                    if (diff > 2e-2 * abs(cpu) + 4e-3 * ymax) {
                        off++
                    }
                    max_abs = max(max_abs, double(diff))
                }
            }
            delete ys
        }
    }
    return (samples = samples, off = off, max_abs = max_abs)
}

//! one line per arm: its best rate, and for a compared arm its distance from the shipped class - bit-exact, within the bound,
//! or OVER it (the exit code); the shipped class's output stays in `y_ref` for the CPU check
def private khr_report(name : string; var raws : array<VkCommandBuffer>; best : array<double>; last_out : uint64; y_bytes : int64; flop : double; var y_ref : array<float>) {
    read_back_y(raws[KHR_ARM_KHR], last_out, y_bytes, y_ref)
    var y_arm : array<float>
    for (v in range(KHR_ARMS)) {
        if (raws[v] == null) {
            print("{name} {KHR_ARM_NAMES[v]}: not run - a whole-tile arm on a partial window\n")
            continue
        }
        let tf = flop / (best[v] * 1000000.0lf)
        var tail = "timing-only"
        if (KHR_ARM_COMPARED[v]) {
            read_back_y(raws[v], last_out, y_bytes, y_arm)
            let mrd = max_rel_diff(y_arm, y_ref)
            if (mrd == 0.0lf) {
                tail = "bit-exact vs khr"
            } else {
                let within = mrd <= KHR_ARM_REL_BOUND
                tail = "max rel diff {mrd:.5f} vs khr ({within ? "within" : "OVER"} {KHR_ARM_REL_BOUND})"
                if (!within) {
                    g_probe_exit = 1
                }
            }
        }
        print("{name} {KHR_ARM_NAMES[v]}: {best[v] / 1000.0lf} ms/dispatch  {tf} TFLOP/s  {tail}\n")
    }
    delete y_arm
}

//! the khrprof mode: one submit per arm - or the one arm named - so a GPU profiler sees a single kernel's dispatches
def private khr_prof_submit(name : string; var raws : array<VkCommandBuffer>) {
    var submitted = 0
    for (v in range(KHR_ARMS)) {
        if (raws[v] != null && (g_prof_arm == "" || strip(KHR_ARM_NAMES[v]) == g_prof_arm)) {
            submit_wait(raws[v])
            submitted++
        }
    }
    let which = g_prof_arm == "" ? "all {KHR_ARMS}" : g_prof_arm
    print("{name}: one submit per arm ({which}), {DISPATCHES} dispatches each\n")
    if (submitted == 0) {
        print("khrprof: no arm named '{g_prof_arm}' - nothing was dispatched\n")
        g_probe_exit = 1
    }
}

[arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
def private run_khr_shape(name : string; d, n, cnt : int; prof : bool = false) {   // nolint:STYLE038 — one interleaved measurement sweep over eight arms, its compare and its CPU check
    let fmt = int(KqFmt.k4)
    let nsb = n / 256
    let totsb = d * nsb
    let bb = arena_block_bytes(fmt)
    let wq_words = int(bb.wq / 4l)
    let ws_words = int(bb.ws / 4l)
    var wqh : array<uint>
    var wsuh : array<uint>
    var xfh : array<uint>
    var xqh : array<uint>
    var axsh : array<float>
    wqh |> resize(totsb * wq_words)
    wsuh |> resize(totsb * ws_words)
    xfh |> resize(cnt * n / 2)
    xqh |> resize(cnt * nsb * 64)
    axsh |> resize(cnt * nsb)
    for (i in range(totsb * wq_words)) {
        wqh[i] = hash_word(uint(i) + 29u)
    }
    for (sb in range(totsb)) {
        wsuh[sb * ws_words] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.00005 * float(1 + sb % 5)))
        for (wi in range(1, ws_words)) {
            wsuh[sb * ws_words + wi] = hash_word(uint(sb * ws_words + wi) + 613u)
        }
    }
    for (i in range(cnt * n / 2)) {
        xfh[i] = cool_f16_pair(uint(i) * 3u + 5u)
    }
    for (i in range(cnt * nsb * 64)) {
        xqh[i] = hash_word(uint(i) * 13u + 1u)
    }
    for (i in range(cnt * nsb)) {
        axsh[i] = 0.0625
    }
    let wq_bytes = int64(totsb) * bb.wq
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
        let yd2 = make_device_buf(y_bytes)   //! consecutive dispatches alternate outputs, as the served graph overlaps them
        let partial = cnt % MM_TILE != 0
        var raws : array<VkCommandBuffer>
        raws |> reserve(KHR_ARMS)
        for (v in range(KHR_ARMS)) {
            if (partial && !khr_arm_clamps(v)) {
                var none : VkCommandBuffer
                raws |> push(none)
                continue
            }
            let tile = v == KHR_ARM_KQ ? 32 : 128
            let wgs = ((cnt + tile - 1) / tile) * ((d + tile - 1) / tile)
            var sched : array<uint>
            sched |> resize(4 + wgs)
            sched[2] = uint(cnt)
            let sc_bytes = int64(4 + wgs) * 4l
            let scd = make_device_buf(sc_bytes)
            upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
            delete sched
            var sa = khr_arm_set(v, fmt, wqd, wsd, scd, xfd, xqd, axsd, yd, wq_bytes, ws_bytes, sc_bytes, xf_bytes, xq_bytes, axs_bytes, y_bytes)
            var sb = khr_arm_set(v, fmt, wqd, wsd, scd, xfd, xqd, axsd, yd2, wq_bytes, ws_bytes, sc_bytes, xf_bytes, xq_bytes, axs_bytes, y_bytes)
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
            for (k in range(DISPATCHES)) {
                var hz : VkHaz   //! fresh per dispatch: no barrier between them, the served graph's shape
                if (k % 2 == 0) {
                    khr_arm_enc(v, fmt, raw, hz, sa, pc, int64(wgs))
                } else {
                    khr_arm_enc(v, fmt, raw, hz, sb, pc, int64(wgs))
                }
            }
            vk_check(vkEndCommandBuffer(raw), null)
            raws |> push(raw)
        }
        if (prof) {
            khr_prof_submit(name, raws)
        } else {
            var best <- sweep_best_us(raws)
            let last_out = DISPATCHES % 2 == 0 ? yd2 : yd
            var y_ref : array<float>
            khr_report(name, raws, best, last_out, y_bytes, flop, y_ref)
            let cpu = khr_cpu_check(wqh, wsuh, xfh, y_ref, d, n, cnt)
            print("{name} khr vs CPU oracle: {cpu.off} of {cpu.samples} sampled outputs off the cell bar, max |diff| {cpu.max_abs:.5f}\n")
            if (cpu.off != 0) {
                g_probe_exit = 1
            }
            delete y_ref
            delete best
        }
        delete raws
    }
    delete wqh
    delete wsuh
    delete xfh
    delete xqh
    delete axsh
}

[vk_dispatch(name = "mmqpx_k4", grid = "wgs", params = "wgs : int64")]
class MmqPxK4 : MoeCmBase {
    @ssbo @binding = 0 @readonly wq : array<uint>
    @ssbo @binding = 1 @readonly wsu : array<uint>
    @ssbo @binding = 3 @readonly xqw : array<uint>
    @ssbo @binding = 4 @readonly axs : array<float>
    @ssbo @binding = 5 y : array<float>
    @workgroup mx4 : uint4[1088]   //! 64 token rows x 16 uint4 chunks, stride 17
    @workgroup mw4 : uint4[544]    //! 32 columns x 16 chunks, stride 17 (the sdot4 tile's staging)
    @workgroup mxs : float[64]
    @workgroup w_dsc : float[256]
    @workgroup w_dmin : float[256]

    //! KqBatchK4.stage_w verbatim: chunk k of column wcol's superblock s, the nibble split at k's half
    def stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let rb4 = wsb * 32u + bu * 4u
                let r0 = wq[rb4]
                let r1 = wq[rb4 + 1u]
                let r2 = wq[rb4 + 2u]
                let r3 = wq[rb4 + 3u]
                if (hh == 0u) {
                    wv = uint4(r0 & 0x0F0F0F0F, r1 & 0x0F0F0F0F, r2 & 0x0F0F0F0F, r3 & 0x0F0F0F0F)
                } else {
                    wv = uint4((r0 >> 4u) & 0x0F0F0F0F, (r1 >> 4u) & 0x0F0F0F0F,
                        (r2 >> 4u) & 0x0F0F0F0F, (r3 >> 4u) & 0x0F0F0F0F)
                }
            }
            mw4[grp * 17u + k] = wv
        }
    }

    //! KqBatchBase.load_dm verbatim: the d/min pair's planes for (block sblk, column slane)
    def stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        var sbv = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let dm = unpackHalf2x16(wsu[wsb * 5u])
            let sc = int((wsu[wsb * 5u + 1u + sblk / 4u] >> ((sblk % 4u) * 8u)) & 0xFF)
            let mn = int((wsu[wsb * 5u + 3u + sblk / 4u] >> ((sblk % 4u) * 8u)) & 0xFF)
            sa = dm.x * float(sc)
            sbv = dm.y * float(mn)
        }
        w_dsc[sblk * 32u + slane] = sa
        w_dmin[sblk * 32u + slane] = sbv
    }

    [spirv_kernel(local_size_x = 64, name = "mmqpx_k4_spv")]
    def run {
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wsb0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nsb = pa.n / 256u
        let tid = gl_LocalInvocationID.x
        let tg = tid / 4u
        let cg = tid % 4u
        var acc : float[32]
        for [unroll] (i in range(32)) {
            acc[i] = 0.0
        }
        var s = 0u
        while (s < nsb) {
            let xrow = xt * 64u + tid
            for (ki in range(16)) {
                let k = uint(ki)
                var xv = uint4(0u)
                if (xrow < cnt) {
                    let xb4 = ((row0 + xrow) * nsb + s) * 64u + k * 4u
                    xv = uint4(xqw[xb4], xqw[xb4 + 1u], xqw[xb4 + 2u], xqw[xb4 + 3u])
                }
                mx4[tid * 17u + k] = xv
            }
            mxs[tid] = xrow < cnt ? axs[(row0 + xrow) * nsb + s] : 0.0
            for (i in range(4)) {
                let idx = tid + 64u * uint(i)
                stage_w(wsb0, wt * 32u + idx / 8u, nsb, s, idx % 8u, idx / 8u)
                stage_ws(wsb0, wt, nsb, s, idx)
            }
            barrier()
            for (bu in range(8)) {
                let k0 = uint(bu) * 2u
                var xw : uint4[8]
                var bs : int[4]
                for [unroll] (t in range(4)) {
                    let xr = (tg * 4u + uint(t)) * 17u + k0
                    xw[t * 2] = mx4[xr]
                    xw[t * 2 + 1] = mx4[xr + 1u]
                    bs[t] = (sdot4(xw[t * 2].x, 0x01010101) + sdot4(xw[t * 2].y, 0x01010101) + sdot4(xw[t * 2].z, 0x01010101) + sdot4(xw[t * 2].w, 0x01010101)
                        + sdot4(xw[t * 2 + 1].x, 0x01010101) + sdot4(xw[t * 2 + 1].y, 0x01010101) + sdot4(xw[t * 2 + 1].z, 0x01010101) + sdot4(xw[t * 2 + 1].w, 0x01010101))
                }
                for [unroll] (c in range(8)) {
                    let col = cg * 8u + uint(c)
                    let w0 = mw4[col * 17u + k0]
                    let w1 = mw4[col * 17u + k0 + 1u]
                    let dsc = w_dsc[uint(bu) * 32u + col]
                    let dmin = w_dmin[uint(bu) * 32u + col]
                    for [unroll] (t in range(4)) {
                        let idot = (sdot4(w0.x, xw[t * 2].x) + sdot4(w0.y, xw[t * 2].y) + sdot4(w0.z, xw[t * 2].z) + sdot4(w0.w, xw[t * 2].w)
                            + sdot4(w1.x, xw[t * 2 + 1].x) + sdot4(w1.y, xw[t * 2 + 1].y) + sdot4(w1.z, xw[t * 2 + 1].z) + sdot4(w1.w, xw[t * 2 + 1].w))
                        acc[t * 8 + c] += mxs[tg * 4u + uint(t)] * (dsc * float(idot) - dmin * float(bs[t]))
                    }
                }
            }
            barrier()
            s++
        }
        for [unroll] (t in range(4)) {
            let xrow = xt * 64u + tg * 4u + uint(t)
            for [unroll] (c in range(8)) {
                let col = wt * 32u + cg * 8u + uint(c)
                if (xrow < cnt && col < pa.d) {
                    y[(row0 + xrow) * pa.d + col] = acc[t * 8 + c]
                }
            }
        }
    }
}

[vk_dispatch(name = "mmqpx_k4b", grid = "wgs", params = "wgs : int64")]
class MmqPxK4Blk : MoeCmBase {
    @ssbo @binding = 0 @readonly wq : array<uint>
    @ssbo @binding = 1 @readonly wsu : array<uint>
    @ssbo @binding = 3 @readonly xqw : array<uint>
    @ssbo @binding = 4 @readonly axs : array<float>
    @ssbo @binding = 5 y : array<float>
    @workgroup bx4 : uint4[128]    //! 64 token rows x one block (2 uint4 chunks)
    @workgroup bw4 : uint4[256]    //! 128 columns x one block
    @workgroup bxs : float[64]
    @workgroup w_dsc : float[128]
    @workgroup w_dmin : float[128]

    [spirv_kernel(local_size_x = 128, name = "mmqpx_k4b_spv")]
    def run {   // nolint:STYLE038 — one measurement body
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let wsb0 = sched[rb]
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let wtiles = (pa.d + 127u) / 128u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nsb = pa.n / 256u
        let tid = gl_LocalInvocationID.x
        let tg = tid / 8u
        let cg = tid % 8u
        var acc : float[64]
        for [unroll] (i in range(64)) {
            acc[i] = 0.0
        }
        var s = 0u
        while (s < nsb) {
            if (tid < 64u) {
                let xrow = xt * 64u + tid
                bxs[tid] = xrow < cnt ? axs[(row0 + xrow) * nsb + s] : 0.0
            }
            for (bu in range(8)) {
                let k0 = uint(bu) * 2u
                let xrow = xt * 64u + tid / 2u
                var xv = uint4(0u)
                if (xrow < cnt) {
                    let xb4 = ((row0 + xrow) * nsb + s) * 64u + (k0 + tid % 2u) * 4u
                    xv = uint4(xqw[xb4], xqw[xb4 + 1u], xqw[xb4 + 2u], xqw[xb4 + 3u])
                }
                bx4[tid] = xv
                let wcol = wt * 128u + tid
                var w0 = uint4(0u)
                var w1 = uint4(0u)
                var sa = 0.0
                var sbv = 0.0
                if (wcol < pa.d) {
                    let wsb = wsb0 + wcol * nsb + s
                    let rb4 = wsb * 32u + uint(bu) * 4u
                    let r0 = wq[rb4]
                    let r1 = wq[rb4 + 1u]
                    let r2 = wq[rb4 + 2u]
                    let r3 = wq[rb4 + 3u]
                    w0 = uint4(r0 & 0x0F0F0F0F, r1 & 0x0F0F0F0F, r2 & 0x0F0F0F0F, r3 & 0x0F0F0F0F)
                    w1 = uint4((r0 >> 4u) & 0x0F0F0F0F, (r1 >> 4u) & 0x0F0F0F0F, (r2 >> 4u) & 0x0F0F0F0F, (r3 >> 4u) & 0x0F0F0F0F)
                    let dm = unpackHalf2x16(wsu[wsb * 5u])
                    let sc = int((wsu[wsb * 5u + 1u + uint(bu) / 4u] >> ((uint(bu) % 4u) * 8u)) & 0xFF)
                    let mn = int((wsu[wsb * 5u + 3u + uint(bu) / 4u] >> ((uint(bu) % 4u) * 8u)) & 0xFF)
                    sa = dm.x * float(sc)
                    sbv = dm.y * float(mn)
                }
                bw4[tid * 2u] = w0
                bw4[tid * 2u + 1u] = w1
                w_dsc[tid] = sa
                w_dmin[tid] = sbv
                barrier()
                var xw : uint4[8]
                var bs : int[4]
                for [unroll] (t in range(4)) {
                    let xr = (tg * 4u + uint(t)) * 2u
                    xw[t * 2] = bx4[xr]
                    xw[t * 2 + 1] = bx4[xr + 1u]
                    bs[t] = (sdot4(xw[t * 2].x, 0x01010101) + sdot4(xw[t * 2].y, 0x01010101) + sdot4(xw[t * 2].z, 0x01010101) + sdot4(xw[t * 2].w, 0x01010101)
                        + sdot4(xw[t * 2 + 1].x, 0x01010101) + sdot4(xw[t * 2 + 1].y, 0x01010101) + sdot4(xw[t * 2 + 1].z, 0x01010101) + sdot4(xw[t * 2 + 1].w, 0x01010101))
                }
                for [unroll] (c in range(16)) {
                    let col = cg * 16u + uint(c)
                    let wa = bw4[col * 2u]
                    let wb = bw4[col * 2u + 1u]
                    let dsc = w_dsc[col]
                    let dmin = w_dmin[col]
                    for [unroll] (t in range(4)) {
                        let idot = (sdot4(wa.x, xw[t * 2].x) + sdot4(wa.y, xw[t * 2].y) + sdot4(wa.z, xw[t * 2].z) + sdot4(wa.w, xw[t * 2].w)
                            + sdot4(wb.x, xw[t * 2 + 1].x) + sdot4(wb.y, xw[t * 2 + 1].y) + sdot4(wb.z, xw[t * 2 + 1].z) + sdot4(wb.w, xw[t * 2 + 1].w))
                        acc[t * 16 + c] += bxs[tg * 4u + uint(t)] * (dsc * float(idot) - dmin * float(bs[t]))
                    }
                }
                barrier()
            }
            s++
        }
        for [unroll] (t in range(4)) {
            let xrow = xt * 64u + tg * 4u + uint(t)
            for [unroll] (c in range(16)) {
                let col = wt * 128u + cg * 16u + uint(c)
                if (xrow < cnt && col < pa.d) {
                    y[(row0 + xrow) * pa.d + col] = acc[t * 16 + c]
                }
            }
        }
    }
}

//! eight sdot4 over one 32-quant block: a column's two staged chunks against a row's two
def dot8(wa, wb, xa, xb : uint4) : int {
    return (sdot4(wa.x, xa.x) + sdot4(wa.y, xa.y) + sdot4(wa.z, xa.z) + sdot4(wa.w, xa.w)
        + sdot4(wb.x, xb.x) + sdot4(wb.y, xb.y) + sdot4(wb.z, xb.z) + sdot4(wb.w, xb.w))
}

def bsum8(xa, xb : uint4) : int {
    return (sdot4(xa.x, 0x01010101) + sdot4(xa.y, 0x01010101) + sdot4(xa.z, 0x01010101) + sdot4(xa.w, 0x01010101)
        + sdot4(xb.x, 0x01010101) + sdot4(xb.y, 0x01010101) + sdot4(xb.z, 0x01010101) + sdot4(xb.w, 0x01010101))
}

[ |> template_struct_instance]
class template MmqPxK4CT {   //! standalone: a harness template cannot derive from the module's plain base
    @ssbo @binding = 0 @role = "alias" @readonly wq : array<uint>
    @ssbo @binding = 1 @role = "alias" @readonly wsu : array<uint>
    @ssbo @binding = 2 sched : array<uint>
    @ssbo @binding = 3 @role = "alias" @readonly xqw : array<uint>
    @ssbo @binding = 4 @role = "alias" @readonly axs : array<float>
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : BatchArgs
    @workgroup bx4 : uint4[128]    //! 64 token rows x one block (2 uint4 chunks)
    @workgroup bw4 : uint4[128]    //! 64 columns x one block
    @workgroup bxs : float[64]
    @workgroup w_dsc : float[64]
    @workgroup w_dmin : float[64]
    @template_constant NOSTAGE : bool = false   //! the compute ceiling: constants in place of the plane reads

    [spirv_kernel(local_size_x = 256)]
    def run {   // nolint:STYLE038 — one measurement body, the register block written out
        let rb = sched[pa.map_off + gl_WorkGroupID.x] * 4u
        let row0 = sched[rb + 1u]
        let cnt = sched[rb + 2u]
        let tix = gl_WorkGroupID.x - sched[rb + 3u]
        let wtiles = (pa.d + 63u) / 64u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nsb = pa.n / 256u
        let tid = gl_LocalInvocationID.x
        let tg = tid / 16u
        let cg = tid % 16u
        var a00 = 0.0
        var a01 = 0.0
        var a02 = 0.0
        var a03 = 0.0
        var a10 = 0.0
        var a11 = 0.0
        var a12 = 0.0
        var a13 = 0.0
        var a20 = 0.0
        var a21 = 0.0
        var a22 = 0.0
        var a23 = 0.0
        var a30 = 0.0
        var a31 = 0.0
        var a32 = 0.0
        var a33 = 0.0
        var s = 0u
        while (s < nsb) {
            if (tid < 64u) {
                static_if (NOSTAGE) {
                    bxs[tid] = 0.0625
                } else {
                    let xrow = xt * 64u + tid
                    bxs[tid] = xrow < cnt ? axs[(row0 + xrow) * nsb + s] : 0.0
                }
            }
            for (bu in range(8)) {
                if (tid < 128u) {
                    var xv = uint4(0u)
                    static_if (NOSTAGE) {
                        xv = uint4(0x01020304u + s, 0x05060708u, 0x090A0B0Cu, 0x0D0E0F10u + uint(bu))
                    } else {
                        let xrow = xt * 64u + tid / 2u
                        if (xrow < cnt) {
                            let xb4 = ((row0 + xrow) * nsb + s) * 64u + (uint(bu) * 2u + tid % 2u) * 4u
                            xv = uint4(xqw[xb4], xqw[xb4 + 1u], xqw[xb4 + 2u], xqw[xb4 + 3u])
                        }
                    }
                    bx4[tid] = xv
                } else {
                    let ct = tid - 128u
                    var wv = uint4(0u)
                    var sa = 0.0
                    var sbv = 0.0
                    static_if (NOSTAGE) {
                        wv = uint4(0x01020304u + uint(bu), 0x05060708u, 0x090A0B0Cu + s, 0x0D0E0F10u)
                        sa = 0.001
                        sbv = 0.0001
                    } else {
                        let wcol = wt * 64u + ct / 2u
                        if (wcol < pa.d) {
                            let wsb = sched[rb] + wcol * nsb + s
                            let rb4 = wsb * 32u + uint(bu) * 4u
                            let sh = (ct % 2u) * 4u
                            wv = uint4((wq[rb4] >> sh) & 0x0F0F0F0F, (wq[rb4 + 1u] >> sh) & 0x0F0F0F0F, (wq[rb4 + 2u] >> sh) & 0x0F0F0F0F, (wq[rb4 + 3u] >> sh) & 0x0F0F0F0F)
                            let dm = unpackHalf2x16(wsu[wsb * 5u])
                            let sc = int((wsu[wsb * 5u + 1u + uint(bu) / 4u] >> ((uint(bu) % 4u) * 8u)) & 0xFF)
                            let mn = int((wsu[wsb * 5u + 3u + uint(bu) / 4u] >> ((uint(bu) % 4u) * 8u)) & 0xFF)
                            sa = dm.x * float(sc)
                            sbv = dm.y * float(mn)
                        }
                    }
                    bw4[ct] = wv
                    if (ct % 2u == 0u) {
                        w_dsc[ct / 2u] = sa
                        w_dmin[ct / 2u] = sbv
                    }
                }
                barrier()
                let xr = tg * 8u
                let x0a = bx4[xr]
                let x0b = bx4[xr + 1u]
                let x1a = bx4[xr + 2u]
                let x1b = bx4[xr + 3u]
                let x2a = bx4[xr + 4u]
                let x2b = bx4[xr + 5u]
                let x3a = bx4[xr + 6u]
                let x3b = bx4[xr + 7u]
                let b0 = float(bsum8(x0a, x0b))
                let b1 = float(bsum8(x1a, x1b))
                let b2 = float(bsum8(x2a, x2b))
                let b3 = float(bsum8(x3a, x3b))
                let xs0 = bxs[tg * 4u]
                let xs1 = bxs[tg * 4u + 1u]
                let xs2 = bxs[tg * 4u + 2u]
                let xs3 = bxs[tg * 4u + 3u]
                let cr = cg * 8u
                let w0a = bw4[cr]
                let w0b = bw4[cr + 1u]
                let w1a = bw4[cr + 2u]
                let w1b = bw4[cr + 3u]
                let w2a = bw4[cr + 4u]
                let w2b = bw4[cr + 5u]
                let w3a = bw4[cr + 6u]
                let w3b = bw4[cr + 7u]
                let c0 = cg * 4u
                let ws0 = w_dsc[c0]
                let ws1 = w_dsc[c0 + 1u]
                let ws2 = w_dsc[c0 + 2u]
                let ws3 = w_dsc[c0 + 3u]
                let wm0 = w_dmin[c0]
                let wm1 = w_dmin[c0 + 1u]
                let wm2 = w_dmin[c0 + 2u]
                let wm3 = w_dmin[c0 + 3u]
                a00 += xs0 * (ws0 * float(dot8(w0a, w0b, x0a, x0b)) - wm0 * b0)
                a01 += xs0 * (ws1 * float(dot8(w1a, w1b, x0a, x0b)) - wm1 * b0)
                a02 += xs0 * (ws2 * float(dot8(w2a, w2b, x0a, x0b)) - wm2 * b0)
                a03 += xs0 * (ws3 * float(dot8(w3a, w3b, x0a, x0b)) - wm3 * b0)
                a10 += xs1 * (ws0 * float(dot8(w0a, w0b, x1a, x1b)) - wm0 * b1)
                a11 += xs1 * (ws1 * float(dot8(w1a, w1b, x1a, x1b)) - wm1 * b1)
                a12 += xs1 * (ws2 * float(dot8(w2a, w2b, x1a, x1b)) - wm2 * b1)
                a13 += xs1 * (ws3 * float(dot8(w3a, w3b, x1a, x1b)) - wm3 * b1)
                a20 += xs2 * (ws0 * float(dot8(w0a, w0b, x2a, x2b)) - wm0 * b2)
                a21 += xs2 * (ws1 * float(dot8(w1a, w1b, x2a, x2b)) - wm1 * b2)
                a22 += xs2 * (ws2 * float(dot8(w2a, w2b, x2a, x2b)) - wm2 * b2)
                a23 += xs2 * (ws3 * float(dot8(w3a, w3b, x2a, x2b)) - wm3 * b2)
                a30 += xs3 * (ws0 * float(dot8(w0a, w0b, x3a, x3b)) - wm0 * b3)
                a31 += xs3 * (ws1 * float(dot8(w1a, w1b, x3a, x3b)) - wm1 * b3)
                a32 += xs3 * (ws2 * float(dot8(w2a, w2b, x3a, x3b)) - wm2 * b3)
                a33 += xs3 * (ws3 * float(dot8(w3a, w3b, x3a, x3b)) - wm3 * b3)
                barrier()
            }
            s++
        }
        let r0 = xt * 64u + tg * 4u
        let cb = wt * 64u + cg * 4u
        if (r0 < cnt && cb + 3u < pa.d) {   //! the probe shapes are whole tiles
            let yb = (row0 + r0) * pa.d + cb
            y[yb] = a00
            y[yb + 1u] = a01
            y[yb + 2u] = a02
            y[yb + 3u] = a03
            y[yb + pa.d] = a10
            y[yb + pa.d + 1u] = a11
            y[yb + pa.d + 2u] = a12
            y[yb + pa.d + 3u] = a13
            y[yb + 2u * pa.d] = a20
            y[yb + 2u * pa.d + 1u] = a21
            y[yb + 2u * pa.d + 2u] = a22
            y[yb + 2u * pa.d + 3u] = a23
            y[yb + 3u * pa.d] = a30
            y[yb + 3u * pa.d + 1u] = a31
            y[yb + 3u * pa.d + 2u] = a32
            y[yb + 3u * pa.d + 3u] = a33
        }
    }
}

[vk_dispatch(name = "mmqpx_k4c", kernel = "run", grid = "wgs", params = "wgs : int64")]
class MmqPxK4Staged : MmqPxK4CT {
}

[vk_dispatch(name = "mmqpx_k4d", kernel = "run", grid = "wgs", params = "wgs : int64")]
class MmqPxK4Ceil : MmqPxK4CT {
    override NOSTAGE = true
}

def private mmq_arm_set(v, fmt : int; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (v == 0) {
        return kq_batch_cls_set_for(fmt, false, bufs, sizes, gbits)
    } elif (v == 1) {
        return set_mmqpx_k4(bufs, sizes, gbits)
    } elif (v == 2) {
        return set_mmqpx_k4b(bufs, sizes, gbits)
    } elif (v == 3) {
        return set_mmqpx_k4c(bufs, sizes, gbits)
    }
    return set_mmqpx_k4d(bufs, sizes, gbits)
}

def private mmq_arm_enc(v, fmt : int; raw : VkCommandBuffer; var hz : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; wgs : int64) {
    if (v == 0) {
        kq_batch_cls_enc_for(fmt, false, raw, hz, s, pc, wgs)
    } elif (v == 1) {
        enc_mmqpx_k4(raw, hz, s, pc, wgs)
    } elif (v == 2) {
        enc_mmqpx_k4b(raw, hz, s, pc, wgs)
    } elif (v == 3) {
        enc_mmqpx_k4c(raw, hz, s, pc, wgs)
    } else {
        enc_mmqpx_k4d(raw, hz, s, pc, wgs)
    }
}

[arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
def private run_mmq_shape(name : string; d, n, cnt : int) {   // nolint:STYLE038 — one linear measurement sweep
    let fmt = int(KqFmt.k4)
    let nsb = n / 256
    let totsb = d * nsb
    let bb = arena_block_bytes(fmt)
    let wq_words = int(bb.wq / 4l)
    let ws_words = int(bb.ws / 4l)
    var wqh : array<uint>
    var wsuh : array<uint>
    var xqh : array<uint>
    var axsh : array<float>
    wqh |> resize(totsb * wq_words)
    wsuh |> resize(totsb * ws_words)
    xqh |> resize(cnt * nsb * 64)
    axsh |> resize(cnt * nsb)
    for (i in range(totsb * wq_words)) {
        wqh[i] = hash_word(uint(i) + 29u)
    }
    for (sb in range(totsb)) {
        wsuh[sb * ws_words] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.00005 * float(1 + sb % 5)))
        for (wi in range(1, ws_words)) {
            wsuh[sb * ws_words + wi] = hash_word(uint(sb * ws_words + wi) + 613u)
        }
    }
    for (i in range(cnt * nsb * 64)) {
        xqh[i] = hash_word(uint(i) * 13u + 1u)
    }
    for (i in range(cnt * nsb)) {
        axsh[i] = 0.0625
    }
    let wq_bytes = int64(totsb) * bb.wq
    let ws_bytes = long_length(wsuh) * 4l
    let xq_bytes = long_length(xqh) * 4l
    let axs_bytes = int64(cnt * nsb) * 4l
    let y_bytes = int64(cnt * d) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let xqd = make_device_buf(xq_bytes)
    let axsd = make_device_buf(axs_bytes)
    let yd = make_device_buf(y_bytes)
    let flop = 2.0lf * double(cnt) * double(d) * double(n)
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsuh[0]), ws_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
        upload_region_at(axsd, 0l, addr<uint8 const?>(axsh[0]), axs_bytes)
        let yd2 = make_device_buf(y_bytes)   //! consecutive dispatches alternate outputs, as the served graph overlaps them
        let vnames = fixed_array("kq    ", "mmq   ", "mmqb  ", "mmqc  ", "mmqd  ")
        let compared = fixed_array(false, true, true, true, false)   //! kq is the reference; the ceiling twin reads constants: timing-only
        var raws : array<VkCommandBuffer>
        raws |> reserve(MMQ_ARMS)
        for (v in range(MMQ_ARMS)) {
            let ttile = v == 0 ? 32 : 64
            let wtile = v == 2 ? 128 : (v >= 3 ? 64 : 32)
            let wgs = ((cnt + ttile - 1) / ttile) * ((d + wtile - 1) / wtile)
            var sched : array<uint>
            sched |> resize(4 + wgs)
            sched[2] = uint(cnt)
            let sc_bytes = int64(4 + wgs) * 4l
            let scd = make_device_buf(sc_bytes)
            upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
            delete sched
            let gbits = fixed_array(1u, 2u, 0u, 4u, 8u, 16u)
            let asizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, axs_bytes, y_bytes)
            var sa = mmq_arm_set(v, fmt, fixed_array(wqd, wsd, scd, xqd, axsd, yd), asizes, gbits)
            var sb = mmq_arm_set(v, fmt, fixed_array(wqd, wsd, scd, xqd, axsd, yd2), asizes, gbits)
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
            for (k in range(DISPATCHES)) {
                var hz : VkHaz   //! fresh per dispatch: no barrier between them, the served graph's shape
                if (k % 2 == 0) {
                    mmq_arm_enc(v, fmt, raw, hz, sa, pc, int64(wgs))
                } else {
                    mmq_arm_enc(v, fmt, raw, hz, sb, pc, int64(wgs))
                }
            }
            vk_check(vkEndCommandBuffer(raw), null)
            raws |> push(raw)
        }
        var best <- sweep_best_us(raws)
        let last_out = DISPATCHES % 2 == 0 ? yd2 : yd
        var y_ref : array<float>
        read_back_y(raws[0], last_out, y_bytes, y_ref)
        var y_arm : array<float>
        for (v in range(MMQ_ARMS)) {
            let tf = flop / (best[v] * 1000000.0lf)
            var tail = "timing-only"
            if (compared[v]) {
                read_back_y(raws[v], last_out, y_bytes, y_arm)
                tail = "max rel diff {max_rel_diff(y_arm, y_ref):.5f} vs kq"
            }
            print("{name} {vnames[v]}: {best[v] / 1000.0lf} ms/dispatch  {tf} TFLOP/s  {tail}\n")
        }
        delete y_ref
        delete y_arm
        delete best
        delete raws
    }
    delete wqh
    delete wsuh
    delete xqh
    delete axsh
}

// k6 (Q6_K) cm2 decode-in-load vs the serving kq tile - same instrument as run_k4_shape;
// bisect adds the k6px decode-spelling variants
[arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
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
        xfh[i] = cool_f16_pair(uint(i) * 3u + 5u)
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
        let vnames = fixed_array("k6l ", "k6m ", "k6lnb", "kqnb ", "xnil ", "xflat", "xql  ", "xpair", "xpr16")   // the shipped k6 decode IS the sfix spelling the bisect ended on
        for (v in range(bisect ? 9 : 4)) {
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
                    } else {
                        var s = set_k6px_pair16(bufs, sizes, gbits)
                        enc_k6px_pair16(raw, hz, s, pc, int64(wgs))
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
            let us = usec_per_dispatch(raw)
            let tf = flop / (us * 1000000.0lf)
            print("{name} {vnames[v]}: {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s  timing-only\n")
        }
    }
    delete wqh
    delete wsuh
    delete xfh
    delete xqh
    delete axsh
}

// two warmup submits (pipeline compile + clocks settle), then the timed window
def private usec_per_dispatch(var raw : VkCommandBuffer) : double {
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
    let wtiles = (d + MM_TILE - 1) / MM_TILE
    let wgs = ((cnt + MM_TILE - 1) / MM_TILE) * wtiles
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
        per_us = usec_per_dispatch(raw)
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
            cm2l_us = usec_per_dispatch(rawl)
            var scm = set_q8_batch_cm2m_cls(cbufs, csizes, cgbits)
            var rawm = alloc_cmd()
            vk_check(vkBeginCommandBuffer(rawm, begin), null)
            var hzm : VkHaz
            for (_k in range(DISPATCHES)) {
                enc_q8_batch_cm2m_cls(rawm, hzm, scm, pc, int64(mwgs))
            }
            vk_check(vkEndCommandBuffer(rawm), null)
            cm2m_us = usec_per_dispatch(rawm)
        }
    }
    let flop = 2.0lf * double(rows) * double(d) * double(n)
    let tflops = flop / (per_us * 1000000.0lf)
    let ms = per_us / 1000.0lf
    print("{name}: d={d} K={n} cnt={cnt} wgs={wgs}  {ms} ms/dispatch  {tflops} TFLOP/s  timing-only\n")
    if (cm2l_us > 0.0lf) {
        let ltf = flop / (cm2l_us * 1000000.0lf)
        let mtf = flop / (cm2m_us * 1000000.0lf)
        print("{name}: cm2l wgs={lwgs}  {cm2l_us / 1000.0lf} ms/dispatch  {ltf} TFLOP/s  timing-only\n")
        print("{name}: cm2m wgs={mwgs}  {cm2m_us / 1000.0lf} ms/dispatch  {mtf} TFLOP/s  timing-only\n")
    }
    delete sched
    delete b
}

// the decode-cost bisect: the shipped cm2l against the three probe variants, one shape
[arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
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
            let us = usec_per_dispatch(raw)
            let tf = flop / (us * 1000000.0lf)
            print("{name} {vnames[v]}: {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s  timing-only\n")
        }
    }
    delete sched
    delete b
}

[vk_dispatch(name = "cm2px_f16a", grid = "wgs", params = "wgs : int64")]
class Cm2PxF16A : MoeCmBase {
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

// ===== the reference coopmat2 GEMM blob in our harness (arg "ref") - the probe class only
// shapes the pipeline layout (3 SSBOs + its 17-word push block); DASLLAMA_VK_SPV_OVERRIDE serves
// the glslc-built, spec-const-patched reference shader in place of the dummy body =====

struct RefGemmArgs {
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

[vk_dispatch(name = "cm2px_ref", grid = "wgs", params = "wgs : int64")]
class Cm2PxRef {
    @ssbo @binding = 0 @readonly aq : array<uint>
    @ssbo @binding = 1 @readonly bx : array<float16>
    @ssbo @binding = 2 dy : array<float>
    @push_constant pa : RefGemmArgs

    [spirv_kernel(local_size_x = 256, name = "cm2px_ref_spv")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.m) {
            dy[i] = float(bx[i]) + float(aq[i])
        }
    }
}

[vk_dispatch(name = "cm2px_ref2", grid = "wgs", params = "wgs : int64")]
class Cm2PxRef2 {
    @ssbo @binding = 0 @readonly aq : array<uint>
    @ssbo @binding = 1 @readonly bx : array<float16>
    @ssbo @binding = 2 dy : array<float>
    @ssbo @binding = 3 @readonly sc : array<float16>
    @push_constant pa : RefGemmArgs

    [spirv_kernel(local_size_x = 256, name = "cm2px_ref2_spv")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.m) {
            dy[i] = float(bx[i]) + float(aq[i]) + float(sc[i])
        }
    }
}

// the minimal our-emitter kernel: fast-path loop only, 256 rows literal, wg.x = weight tile,
// two-plane decode without the workgroup base (the two-plane reference form). Recovery here pins the
// cm2l deficit to the preamble/branch structure; no recovery pins it to per-op emission
[vk_dispatch(name = "cm2px_min", grid = "wgs", params = "wgs : int64")]
class Cm2PxMin {
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

// gate shape at cnt = 256 (the reference grid is 2-D; one BN=256 column tile keeps it 1-D):
// the reference interleaved q8 A plane, drain-free timing, our cm2l + mm rows alongside
[arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
def private run_ref_gemm(d, n, cnt : int) {   // nolint:STYLE038 — one linear measurement sweep
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
        xf[i] = cool_f16_pair(uint(i) * 5u + 11u)
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
        var pc = RefGemmArgs(m = uint(d), n = uint(cnt), k = uint(n),
            sa = uint(n), sb = uint(n), sd = uint(d),
            bsa = uint(d) * uint(n), bsb = uint(cnt) * uint(n), bsd = uint(cnt) * uint(d),
            bz = 0u, nb = 1u, ks = uint(n), ne02 = 1u, ne12 = 1u, b2 = 1u, b3 = 1u, pn = uint(cnt))
        var scw : array<uint>
        scw |> resize(totblk / 2 + 1)
        for (i in range(totblk / 2 + 1)) {
            scw[i] = cool_f16_pair(uint(i) + 7u)
        }
        let sc_bytes = int64(totblk) * 2l
        let scd = make_device_buf(sc_bytes)
        upload_region_at(scd, 0l, addr<uint8 const?>(scw[0]), sc_bytes)
        delete scw
        var s = set_cm2px_ref(
            fixed_array(ad, bd, yd),
            fixed_array(a_bytes, b_bytes, y_bytes),
            fixed_array(1u, 2u, 4u))
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        for (_k in range(DISPATCHES)) {
            var hz : VkHaz
            enc_cm2px_ref(raw, hz, s, pc, int64(wgs))
        }
        vk_check(vkEndCommandBuffer(raw), null)
        let us = usec_per_dispatch(raw)
        let tf = flop / (us * 1000000.0lf)
        print("ref cm2 (d={d} K={n} cnt={cnt} wgs={wgs}): {us / 1000.0lf} ms/dispatch  {tf} TFLOP/s  timing-only\n")
        var s2 = set_cm2px_ref2(
            fixed_array(ad, bd, yd, scd),
            fixed_array(a_bytes, b_bytes, y_bytes, sc_bytes),
            fixed_array(1u, 2u, 4u, 8u))
        var raw2 = alloc_cmd()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        for (_k in range(DISPATCHES)) {
            var hz : VkHaz
            enc_cm2px_ref2(raw2, hz, s2, pc, int64(wgs))
        }
        vk_check(vkEndCommandBuffer(raw2), null)
        let us2 = usec_per_dispatch(raw2)
        let tf2 = flop / (us2 * 1000000.0lf)
        print("ref+2plane: {us2 / 1000.0lf} ms/dispatch  {tf2} TFLOP/s  timing-only\n")
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
        let us3 = usec_per_dispatch(raw3)
        let tf3 = flop / (us3 * 1000000.0lf)
        print("our minimal (min): {us3 / 1000.0lf} ms/dispatch  {tf3} TFLOP/s  timing-only\n")
    }
    delete aw
    delete xf
}

[arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
def private run_probe {   // nolint:STYLE037,STYLE038 — the flat per-arg shape dispatcher
    g_probe_exit = 1   //! every early return below is a run with no result row; the sweeps clear it
    if (!ensure_q8_batch_cls()) {
        print("no Vulkan device\n")
        return
    }
    if (g_gpu.coopmat_mode < COOPMAT_MM) {
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
    var cm2_fmt = -1
    for (a in get_command_line_arguments()) {
        if (a == "gate" || a == "down" || a == "q" || a == "kv" || a == "kvm" || a == "qkv" || a == "cm2x" || a == "ref" || a == "tl" || a == "k4" || a == "k6" || a == "k6x" || a == "khrx" || a == "khrprof" || a == "mmqx") {
            only = a
        } elif (a |> starts_with("cm2:")) {
            only = "cm2:"
            var pf : KqFmt
            cm2_fmt = kq_fmt_of_name(slice(a, 4), pf) ? int(pf) : -1
        } elif (a |> starts_with("khrprof:")) {
            only = "khrprof"
            g_prof_arm = slice(a, 8)
        }
    }
    if (only == "khrprof" && g_prof_arm != "") {
        var known = false
        for (vn in KHR_ARM_NAMES) {
            known = known || strip(vn) == g_prof_arm
        }
        if (!known) {
            print("khrprof: no arm named '{g_prof_arm}' - the arms are {KHR_ARM_NAMES}\n")
            return
        }
    }
    g_probe_exit = 0
    if (only == "cm2:") {
        if (!g_gpu.has_coopmat2) {
            print("no coopmat2 on this device\n")
            return
        }
        if (cm2_fmt < 0 || !kq_sb(cm2_fmt)) {
            print("cm2:<fmt> needs a kq superblock format name ({cm2_fmt_names()})\n")
            return
        }
        // the four-wide decode's A/B, both arms in ONE process, interleaved: the class pipelines
        // rebuild under each arm (the twin served, then stripped), two rounds so drift shows
        let twin_device = g_gpu.decvec_on
        let arms = twin_device ? 2 : 1
        for (round in range(2)) {
            for (arm in range(arms)) {
                g_gpu.decvec_on = twin_device && arm == 0
                vkd_pipes_rebuild()
                verify(cm2_cls_ensure(cm2_fmt, 256l) && cm2_cls_ensure(cm2_fmt, 128l)
                    && kq_batch_cls_ensure(cm2_fmt, false), "cm2:<fmt> probe pipelines must engage")
                let tag = twin_device ? (arm == 0 ? "twin on " : "twin off") : "no twin "
                run_cm2_fmt_shape(cm2_fmt, "r{round} {tag} gate", 9728, 2560, 512)
                run_cm2_fmt_shape(cm2_fmt, "r{round} {tag} down", 2560, 9728, 512)
                run_cm2_fmt_shape(cm2_fmt, "r{round} {tag} q/wo", 4096, 2560, 512)
            }
        }
        g_gpu.decvec_on = twin_device
        return
    }
    if (only == "k6x") {
        if (!g_gpu.has_coopmat2) {
            print("no coopmat2 on this device\n")
            return
        }
        verify(ensure_kq_batch_k6_cm2l_cls() && ensure_kq_batch_k6_cm2m_cls()
            && kq_batch_cls_ensure(int(KqFmt.k6), false)
            && ensure_k6px_nil() && ensure_k6px_flat() && ensure_k6px_ql()
            && ensure_k6px_pair() && ensure_k6px_pair16(), "k6x probe pipelines must engage")
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
    if (only == "ref") {
        if (!g_gpu.has_coopmat2) {
            print("no coopmat2 on this device\n")
            return
        }
        verify(ensure_cm2px_ref() && ensure_cm2px_ref2() && ensure_cm2px_min(), "ref-blob pipelines must engage")
        run_ref_gemm(8192, 3072, 256)
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
    if (only == "mmqx") {
        verify(ensure_mmqpx_k4() && ensure_mmqpx_k4b() && ensure_mmqpx_k4c() && ensure_mmqpx_k4d() && kq_batch_cls_ensure(int(KqFmt.k4), false), "integer probe pipelines must engage")
        run_mmq_shape("q4k gate", 9728, 2560, 512)
        run_mmq_shape("q4k down", 2560, 9728, 512)
        run_mmq_shape("q4k q/wo", 4096, 2560, 512)
        return
    }
    if (only == "khrx") {
        verify(ensure_khrpx_nil() && ensure_khrpx_ship() && ensure_khrpx_dec4() && ensure_kq_batch_k4_khr_cls()
            && ensure_khrpx_f32a() && ensure_khrpx_strip() && ensure_khrpx_wg128()
            && kq_batch_cls_ensure(int(KqFmt.k4), false), "KHR probe pipelines must engage")
        run_khr_shape("q4k gate", 9728, 2560, 512)
        run_khr_shape("q4k down", 2560, 9728, 512)
        run_khr_shape("q4k q/wo", 4096, 2560, 512)
        run_khr_shape("q4k gate 300", 9728, 2560, 300)   //! a partial window: the last token tile takes the edge store
        return
    }
    if (only == "khrprof") {
        verify(ensure_khrpx_nil() && ensure_khrpx_ship() && ensure_khrpx_dec4() && ensure_kq_batch_k4_khr_cls()
            && ensure_khrpx_f32a() && ensure_khrpx_strip() && ensure_khrpx_wg128()
            && kq_batch_cls_ensure(int(KqFmt.k4), false), "KHR probe pipelines must engage")
        run_khr_shape("q4k gate", 9728, 2560, 512, true)
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
def main : int {
    run_probe()
    return g_probe_exit
}
