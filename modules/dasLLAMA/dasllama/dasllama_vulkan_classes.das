options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_vulkan_classes shared public

require dasllama/dasllama_lint public
require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require dasllama/dasllama_kqformat
require daslib/typemacro_boost  // [template_struct_instance] — the KV-codec kernel templates
require math

// The Vulkan KERNEL home: every kernel is a [spirv_kernel]/[vk_dispatch] class — families are
// one base class + per-format overrides, devirtualized at emission. Kernels carry clean
// per-member bindings and push-constant params; region lists stay SSBO.

// The ONE per-token word block (a tiny host-poked SSBO, shared by every carrier kernel's set):
// push constants freeze at record time, so the recorded-once token cmd keeps its per-token
// state here — everything else stays structural in the push (--rerecord-ab priced the
// alternative at -8..-11% tg).
struct TokMeta {
    pos : uint      // this token's absolute position (mirror row = layerbase + pos * kvd)
    cnt : uint      // cached positions incl. this one
    parity : uint   // deltanet conv-ring parity
    _pad : uint
}

// ===== add + rmsnorm =====

struct ArArgs {
    dim : uint
    add_on : uint
    woff : uint     // norm-weight row base in wn (the driver's norms plane is shared per-layer)
    eps : float
    ascale : float
}

// the shared Q8_0 quantize pieces (pure — the amax fold is subgroup-wide)
def private q8_amax3(m0 : float) : float {
    var m = m0
    m = max(m, subgroupShuffleXor(m, 1u))
    m = max(m, subgroupShuffleXor(m, 2u))
    m = max(m, subgroupShuffleXor(m, 4u))
    return m
}

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
def private q8_quant1(v, id : float) : int8 {
    return int8(int(round(v * id)))
}

// ... and the Q8_K twin: whole-subgroup 5-shuffle amax fold
def private q8k_amax5(m0 : float) : float {
    var m = m0
    m = max(m, subgroupShuffleXor(m, 1u))
    m = max(m, subgroupShuffleXor(m, 2u))
    m = max(m, subgroupShuffleXor(m, 4u))
    m = max(m, subgroupShuffleXor(m, 8u))
    m = max(m, subgroupShuffleXor(m, 16u))
    return m
}

// the workgroup-rms machinery the row kernels share (one wg reduces one row)
class RmsWgBase {
    @workgroup part : float[64]   // one partial per subgroup (subgroupSize >= 4 floor)
    @workgroup inv1 : float[1]

    def wg_rms_inv(ss0 : float; n : uint; eps : float) : float {
        let ss = subgroupAdd(ss0)
        if (gl_SubgroupInvocationID == 0u) {
            part[gl_SubgroupID] = ss
        }
        barrier()
        if (gl_LocalInvocationID.x == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += part[sgi]
                sgi++
            }
            inv1[0u] = 1.0 / sqrt(tot / float(n) + eps)   // the CPU rmsnorm's own form
        }
        barrier()
        return inv1[0u]
    }
}

// the add+rmsnorm core: x/a/w members, the row stash, and the shared workgroup reduce — leaves
// add their output members + kernels
class ArBase : RmsWgBase {
    @ssbo @binding = 0 xs : array<float>   // x rows, the residual stream — updated in place
    @ssbo @binding = 1 aa : array<float>   // the add partner rows
    @ssbo @binding = 2 wn : array<float>   // the norm weight row
    @push_constant pa : ArArgs
    @workgroup row : float[4096]           // AR_MAX_DIM — staged so the post-reduce reread is local

    // the residual add + row stash + sum-of-squares pass over one row at `base`
    def accum_row(base : uint) : float {
        var ss = 0.0
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let vv = pa.add_on != 0u ? (xs[base + k] + aa[base + k]) * pa.ascale : xs[base + k]
            xs[base + k] = vv
            row[k] = vv
            ss += vv * vv
            k += 256u
        }
        return ss
    }

    // workgroup-wide rms inverse from per-thread sum-of-squares partials
    def rms_inv(ss0 : float) : float => wg_rms_inv(ss0, pa.dim, pa.eps)
}

// ar_add_rms_b's class twin: one workgroup per row, the normed output to its own member
[vk_dispatch(name = "cls_ar", grid = "nrows", params = "nrows : int64")]
class ClsArAddRms : ArBase {
    @ssbo @binding = 3 yo : array<float>   // the normed output rows

    [spirv_kernel(local_size_x = 256, name = "cls_ar_spv")]
    def run {
        let base = gl_WorkGroupID.x * pa.dim
        let ss = accum_row(base)
        let inv = rms_inv(ss)
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            yo[base + k] = wn[pa.woff + k] * (row[k] * inv)
            k += 256u
        }
    }
}

// ar_add_rms + Q8_0 requant in ONE dispatch: the normed row quantizes out of the row stash, so
// xb never reaches memory. Verbatim reduce/amax/rounding => bit-identical to the split pair.
[vk_dispatch(name = "cls_ar_rq", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRq : ArBase {
    @ssbo @binding = 3 outq : array<int8>    // the quantized bytes
    @ssbo @binding = 4 outs : array<float>   // per-32-block scales

    [spirv_kernel(local_size_x = 256, name = "cls_ar_rq_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let tid = gl_LocalInvocationID.x
        let ss = accum_row(0u)
        let inv = rms_inv(ss)
        // quantize the normed row instead of storing it: 8 lanes per 32-block, 32 blocks per pass
        let nblk = pa.dim / 32u
        let lane8 = tid % 8u
        var b = tid / 8u
        while (b < nblk) {
            let base = b * 32u + lane8 * 4u
            let v0 = wn[pa.woff + base] * (row[base] * inv)
            let v1 = wn[pa.woff + base + 1u] * (row[base + 1u] * inv)
            let v2 = wn[pa.woff + base + 2u] * (row[base + 2u] * inv)
            let v3 = wn[pa.woff + base + 3u] * (row[base + 3u] * inv)
            let m = q8_amax3(max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3))))
            let d = m / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            if (lane8 == 0u) {
                outs[b] = d
            }
            outq[base] = q8_quant1(v0, id)
            outq[base + 1u] = q8_quant1(v1, id)
            outq[base + 2u] = q8_quant1(v2, id)
            outq[base + 3u] = q8_quant1(v3, id)
            b += 32u
        }
    }
}

[vk_dispatch(name = "cls_ar_rq_b", grid = "nrows", params = "nrows : int64")]
class ClsArAddRmsRqB : ArBase {
    @ssbo @binding = 3 outq : array<int8>    // the quantized bytes, row-major blocks
    @ssbo @binding = 4 outs : array<float>   // per-32-block scales

    [spirv_kernel(local_size_x = 256, name = "cls_ar_rq_b_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let tid = gl_LocalInvocationID.x
        let rbase = gl_WorkGroupID.x * pa.dim
        let ss = accum_row(rbase)
        let inv = rms_inv(ss)
        let nblk = pa.dim / 32u
        let qb0 = gl_WorkGroupID.x * nblk
        let lane8 = tid % 8u
        var b = tid / 8u
        while (b < nblk) {
            let base = b * 32u + lane8 * 4u
            let v0 = wn[pa.woff + base] * (row[base] * inv)
            let v1 = wn[pa.woff + base + 1u] * (row[base + 1u] * inv)
            let v2 = wn[pa.woff + base + 2u] * (row[base + 2u] * inv)
            let v3 = wn[pa.woff + base + 3u] * (row[base + 3u] * inv)
            let m = q8_amax3(max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3))))
            let d = m / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            if (lane8 == 0u) {
                outs[qb0 + b] = d
            }
            let qb = qb0 * 32u + base
            outq[qb] = q8_quant1(v0, id)
            outq[qb + 1u] = q8_quant1(v1, id)
            outq[qb + 2u] = q8_quant1(v2, id)
            outq[qb + 3u] = q8_quant1(v3, id)
            b += 32u
        }
    }
}

[vk_dispatch(name = "cls_ar_f16_b", grid = "nrows", params = "nrows : int64")]
class ClsArAddRmsF16B : ArBase {
    @ssbo @binding = 3 outh : array<float16>   // the normed rows, f16

    [spirv_kernel(local_size_x = 256, name = "cls_ar_f16_b_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
    def run {
        let rbase = gl_WorkGroupID.x * pa.dim
        let ss = accum_row(rbase)
        let inv = rms_inv(ss)
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            outh[rbase + k] = float16(wn[pa.woff + k] * (row[k] * inv))
            k += 256u
        }
    }
}

struct RqArgs {
    inbase : uint   // element base of the source rows
    nblk : uint     // block / superblock count
}

// straight Q8_0 requant of f32 rows (the deltanet chain's quantize step)
[vk_dispatch(name = "cls_dn_rq", kernel = "run", family = "rq_cls", grid = "wgs", params = "wgs : int64")]
class DnRequant {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 outq : array<int8>
    @ssbo @binding = 2 outs : array<float>
    @push_constant pa : RqArgs

    [spirv_kernel(local_size_x = 256, name = "cls_dn_rq_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let gid = gl_GlobalInvocationID.x
        let sg = gid / 32u
        let lane = gid % 32u
        let b = sg * 4u + lane / 8u
        if (b < pa.nblk) {
            let eb = pa.inbase + sg * 128u + lane * 4u
            let lane8 = lane % 8u
            let v0 = src[eb]
            let v1 = src[eb + 1u]
            let v2 = src[eb + 2u]
            let v3 = src[eb + 3u]
            let m = q8_amax3(max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3))))
            let d = m / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            if (lane8 == 0u) {
                outs[b] = d
            }
            let qb = b * 32u + lane8 * 4u
            outq[qb] = q8_quant1(v0, id)
            outq[qb + 1u] = q8_quant1(v1, id)
            outq[qb + 2u] = q8_quant1(v2, id)
            outq[qb + 3u] = q8_quant1(v3, id)
        }
    }
}

// straight Q8_K requant of f32 rows (the K-quant chains' mid-step re-quantize)
[vk_dispatch(name = "cls_q8k_rq", kernel = "run", family = "rq_cls", grid = "wgs", params = "wgs : int64")]
class Q8kRequant {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 outq : array<int8>
    @ssbo @binding = 2 outs : array<float>
    @push_constant pa : RqArgs

    def blk_scale(b, lane : uint; m0 : float) : float {
        let d = q8k_amax5(m0) / 127.0
        if (lane == 0u) {
            outs[b] = d
        }
        return d != 0.0 ? 1.0 / d : 0.0
    }

    [spirv_kernel(local_size_x = 256, name = "cls_q8k_rq_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let gid = gl_GlobalInvocationID.x
        let b = gid / 32u
        let lane = gid % 32u
        if (b < pa.nblk) {
            let base = pa.inbase + b * 256u
            var vals : float[8]
            var m = 0.0
            for [unroll] (r in range(8)) {
                let v = src[base + uint(r) * 32u + lane]
                vals[r] = v
                m = max(m, abs(v))
            }
            let id = blk_scale(b, lane, m)
            for [unroll] (r in range(8)) {
                outq[b * 256u + uint(r) * 32u + lane] = q8_quant1(vals[r], id)
            }
        }
    }
}

// ===== act + requant (the FFN mid-chain elementwise family) =====

struct ActArgs {
    nelem : uint   // element count (4-wide kernels; always a 32-multiple, the guard is exact)
    gelu : uint    // activation select: 0 = silu, else tanh-gelu
    nblk : uint    // 32-block / superblock count (the requant kernels' guard)
}

// the one activation formula (silu or tanh-gelu on v, times u) — the classes' act_mul
// delegates pass their own pa.gelu
def private act_mul_sel(v, u : float; gelu : uint) : float {
    return (gelu != 0u ? (0.5 * v * (1.0 + tanh(0.7978845608028654 * (v + 0.044715 * v * v * v)))) * u
        : (v / (1.0 + exp(-v))) * u)
}

// members shared by the act/requant kernels
class ActRqMembers {
    @ssbo @binding = 0 gate : array<float>     // gate rows (act's v side)
    @ssbo @binding = 1 up : array<float>       // up rows (act's u side)
    @ssbo @binding = 2 outq : array<int8>      // quantized bytes out
    @ssbo @binding = 3 outs : array<float>     // per-block scales out
    @push_constant pa : ActArgs

    def act_mul(v, u : float) : float => act_mul_sel(v, u, pa.gelu)
}

// fused act + Q8_0 requant: subgroup per 4 blocks, 4 quants per lane, 8-lane shuffle amax (xor 1/2/4)
[vk_dispatch(name = "q8_actrq_cls", kernel = "run", family = "actrq_cls", grid = "wgs", params = "wgs : int64")]
class Q8ActRq : ActRqMembers {
    // 8-lane block quantize: amax fold over the block's 8 co-active lanes, d to outs[b]
    [arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def blk_store(b, lane8 : uint; v0, v1, v2, v3 : float) {
        let m = q8_amax3(max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3))))
        let d = m / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (lane8 == 0u) {
            outs[b] = d
        }
        let qb = b * 32u + lane8 * 4u
        outq[qb] = q8_quant1(v0, id)
        outq[qb + 1u] = q8_quant1(v1, id)
        outq[qb + 2u] = q8_quant1(v2, id)
        outq[qb + 3u] = q8_quant1(v3, id)
    }

    [spirv_kernel(local_size_x = 256, name = "q8_actrq_cls_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        let sg = gid / 32u
        let lane = gid % 32u
        let b = sg * 4u + lane / 8u
        if (b < pa.nblk) {
            let eb = sg * 128u + lane * 4u
            let v0 = act_mul(gate[eb], up[eb])
            let v1 = act_mul(gate[eb + 1u], up[eb + 1u])
            let v2 = act_mul(gate[eb + 2u], up[eb + 2u])
            let v3 = act_mul(gate[eb + 3u], up[eb + 3u])
            blk_store(b, lane % 8u, v0, v1, v2, v3)
        }
    }
}

// the Q8_K twin: one subgroup per 256-weight superblock, whole-subgroup 5-shuffle amax
[vk_dispatch(name = "q8k_actrq_cls", kernel = "run", family = "actrq_cls", grid = "wgs", params = "wgs : int64")]
class Q8kActRq : ActRqMembers {
    def blk_scale(b, lane : uint; m0 : float) : float {
        let d = q8k_amax5(m0) / 127.0
        if (lane == 0u) {
            outs[b] = d
        }
        return d != 0.0 ? 1.0 / d : 0.0
    }

    [spirv_kernel(local_size_x = 256, name = "q8k_actrq_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let gid = gl_GlobalInvocationID.x
        let b = gid / 32u
        let lane = gid % 32u
        if (b < pa.nblk) {
            let base = b * 256u
            var vals : float[8]
            var m = 0.0
            for [unroll] (r in range(8)) {
                let e = base + uint(r) * 32u + lane
                let v = act_mul(gate[e], up[e])
                vals[r] = v
                m = max(m, abs(v))
            }
            let id = blk_scale(b, lane, m)
            for [unroll] (r in range(8)) {
                outq[base + uint(r) * 32u + lane] = q8_quant1(vals[r], id)
            }
        }
    }
}

// act straight to the f16 hidden plane (the cm2 route — no quantization)
[vk_dispatch(name = "actf16_cls", grid = "wgs", params = "wgs : int64")]
class ActF16 {
    @ssbo @binding = 0 gate : array<float>
    @ssbo @binding = 1 up : array<float>
    @ssbo @binding = 2 outh : array<float16>
    @push_constant pa : ActArgs

    def act_mul(v, u : float) : float => act_mul_sel(v, u, pa.gelu)

    [spirv_kernel(local_size_x = 256, name = "actf16_cls_spv")]
    def run {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            outh[e] = float16(clamp(act_mul(gate[e], up[e]), -65504.0, 65504.0))
            outh[e + 1u] = float16(clamp(act_mul(gate[e + 1u], up[e + 1u]), -65504.0, 65504.0))
            outh[e + 2u] = float16(clamp(act_mul(gate[e + 2u], up[e + 2u]), -65504.0, 65504.0))
            outh[e + 3u] = float16(clamp(act_mul(gate[e + 3u], up[e + 3u]), -65504.0, 65504.0))
        }
    }
}

// straight f32 -> f16 rows (the arena cm2 chain's activation feed)
[vk_dispatch(name = "f16cvt_cls", grid = "wgs", params = "wgs : int64")]
class F16Cvt {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 outh : array<float16>
    @push_constant pa : ActArgs

    [spirv_kernel(local_size_x = 256, name = "f16cvt_cls_spv")]
    def run {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            outh[e] = float16(clamp(src[e], -65504.0, 65504.0))
            outh[e + 1u] = float16(clamp(src[e + 1u], -65504.0, 65504.0))
            outh[e + 2u] = float16(clamp(src[e + 2u], -65504.0, 65504.0))
            outh[e + 3u] = float16(clamp(src[e + 3u], -65504.0, 65504.0))
        }
    }
}

struct EmbArgs {
    npos : uint
    dim : uint
    wblk0 : uint   // the embd plane's slab-local block base
    embed_scale : float
}

[vk_dispatch(name = "emb_gather_cls", grid = "wgs", params = "wgs : int64")]
class EmbGather {
    @ssbo @binding = 0 @role = "weight" wq : array<uint>        // weight quant words
    @ssbo @binding = 1 @role = "weight" wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 2 ids : array<uint>       // one token id per row
    @ssbo @binding = 3 xout : array<float>
    @push_constant pa : EmbArgs

    [spirv_kernel(local_size_x = 256, name = "emb_gather_cls_spv"), arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
    def run {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.npos * pa.dim) {
            let row = e / pa.dim
            let col = e % pa.dim
            let ib = pa.wblk0 + ids[row] * (pa.dim / 32u) + col / 32u
            let lw = (col % 32u) / 4u
            let v = (float4(int4(unpack8(int(wq[ib * 8u + lw])))) * float(wsh[ib])) * pa.embed_scale   // dequant then scale - the CPU's order
            xout[e] = v.x
            xout[e + 1u] = v.y
            xout[e + 2u] = v.z
            xout[e + 3u] = v.w
        }
    }
}

[vk_dispatch(name = "emb_gather_f32_cls", grid = "wgs", params = "wgs : int64")]
class EmbGatherF32 {
    @ssbo @binding = 0 @role = "weight" wf : array<float>       // the f32 embd table, vocab x dim rows
    @ssbo @binding = 2 ids : array<uint>       // one token id per row (binding 1 unused: ids/xout sit where the q8 twin binds them)
    @ssbo @binding = 3 xout : array<float>
    @push_constant pa : EmbArgs

    [spirv_kernel(local_size_x = 256, name = "emb_gather_f32_cls_spv"), arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
    def run {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.npos * pa.dim) {
            let row = e / pa.dim
            let col = e % pa.dim
            let src = ids[row] * pa.dim + col
            xout[e] = wf[src] * pa.embed_scale
            xout[e + 1u] = wf[src + 1u] * pa.embed_scale
            xout[e + 2u] = wf[src + 2u] * pa.embed_scale
            xout[e + 3u] = wf[src + 3u] * pa.embed_scale
        }
    }
}

// ===== the MoE combine =====

struct CombineArgs {
    n : uint    // row width
    k : uint    // slots per position
    r0 : uint   // this dispatch's slot-row window [r0, r1)
    r1 : uint
}

// one workgroup per output POSITION, summing its k slots' down-GEMM rows ascending j (the
// decode accumulation order); r0 == 0 starts the sum, chunk-spanning positions accumulate
[vk_dispatch(name = "combine_cls", grid = "npos", params = "npos : int64")]
class MoeCombine {
    @ssbo @binding = 0 acc_rows : array<float>   // the per-position accumulator rows (in/out)
    @ssbo @binding = 1 slot_idx : array<uint>    // position -> its k slot rows
    @ssbo @binding = 2 slot_w : array<float>     // ... and their routing weights
    @ssbo @binding = 3 down_rows : array<float>  // the window's down-GEMM output rows
    @push_constant pa : CombineArgs

    [spirv_kernel(local_size_x = 256, name = "combine_cls_spv")]
    def run {
        let p = gl_WorkGroupID.x
        var i = gl_LocalInvocationID.x
        while (i < pa.n) {
            var acc = pa.r0 == 0u ? 0.0 : acc_rows[p * pa.n + i]
            var j = 0u
            while (j < pa.k) {
                let jb = slot_idx[p * pa.k + j]
                let wj = slot_w[p * pa.k + j]
                if (jb >= pa.r0 && jb < pa.r1 && wj != 0.0) {   // a zero-weight slot's row may be stale (the split's CPU share)
                    acc += wj * down_rows[(jb - pa.r0) * pa.n + i]
                }
                j++
            }
            acc_rows[p * pa.n + i] = acc
            i += 256u
        }
    }
}

struct DecCombineArgs {
    n : uint    // row width
    k : uint    // routed slots
}

[vk_dispatch(name = "dec_combine_cls", grid = "wgs", params = "wgs : int64")]
class DecCombine {
    @ssbo @binding = 0 xin : array<float>        // the residual row before the FFN
    @ssbo @binding = 1 w : array<float>          // the k routing weights
    @ssbo @binding = 2 down_rows : array<float>  // the down GEMV's k rows
    @ssbo @binding = 3 xout : array<float>       // the residual row after the FFN
    @push_constant pa : DecCombineArgs

    [spirv_kernel(local_size_x = 256, name = "dec_combine_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            var acc = 0.0
            var j = 0u
            while (j < pa.k) {
                acc += w[j] * down_rows[j * pa.n + i]
                j++
            }
            xout[i] = xin[i] + acc
        }
    }
}

struct RouterArgs {
    ne : uint     // expert rows
    dim : uint    // row width
    woff : uint   // this layer's row base in the span's router plane (elements)
}

[vk_dispatch(name = "router_gemv_cls", grid = "ne", params = "ne : int64")]
class RouterGemv {
    @ssbo @binding = 0 @role = "weight" w : array<float>   // the router planes, row-major [ne x dim] per layer
    @ssbo @binding = 1 x : array<float>       // the FFN-normed row
    @ssbo @binding = 2 logits : array<float>  // ne logits
    @push_constant pa : RouterArgs
    @workgroup part : float[64]

    [spirv_kernel(local_size_x = 256, name = "router_gemv_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
    def run {
        let e = gl_WorkGroupID.x
        let base = pa.woff + e * pa.dim
        var acc = 0.0
        var i = gl_LocalInvocationID.x
        while (i < pa.dim) {
            acc += w[base + i] * x[i]
            i += 256u
        }
        let s = subgroupAdd(acc)
        if (gl_SubgroupInvocationID == 0u) {
            part[gl_SubgroupID] = s
        }
        barrier()
        if (gl_LocalInvocationID.x == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += part[sgi]
                sgi++
            }
            logits[e] = tot
        }
    }
}

let private F16_MIN_NORMAL = 6.103515625e-5   // the host select's renorm floor

struct TopkArgs {
    ne : uint
    k : uint
    norm : uint      // renormalize the selected weights to sum 1
    wscale : float   // 0 or 1 = none
    base1 : uint     // the gate stack's expert-0 weight block (stack-relative, in the plane's units)
    stride1 : uint   // blocks per expert
    base3 : uint
    stride3 : uint
    base2 : uint
    stride2 : uint
    xnb2 : uint      // the down feed's blocks per hidden row: slot j reads row j (gate/up read the one feed row)
}

[vk_dispatch(name = "topk_cls", grid = "one", params = "one : int64")]
class TopK {
    @ssbo @binding = 0 logits : array<float>
    @ssbo @binding = 1 meta1 : array<uint>   // the gate stack's (wblock, xblock) per slot row
    @ssbo @binding = 2 meta3 : array<uint>
    @ssbo @binding = 3 meta2 : array<uint>
    @ssbo @binding = 4 w : array<float>      // the k routing weights
    @push_constant pa : TopkArgs
    @workgroup part : float[64]
    @workgroup parti : uint[64]
    @workgroup pick : uint[64]
    @workgroup pickw : float[64]
    @workgroup wg_max : float[1]
    @workgroup wg_sum : float[1]

    [spirv_kernel(local_size_x = 256, name = "topk_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
    def run {
        let tid = gl_LocalInvocationID.x
        let live = tid < pa.ne
        let lg = live ? logits[tid] : -3.0e38
        let m0 = subgroupMax(lg)
        if (gl_SubgroupInvocationID == 0u) {
            part[gl_SubgroupID] = m0
        }
        barrier()
        if (tid == 0u) {
            var m = part[0u]
            var sgi = 1u
            while (sgi < gl_NumSubgroups) {
                m = max(m, part[sgi])
                sgi++
            }
            wg_max[0u] = m
        }
        barrier()
        let e = live ? exp(max(lg - wg_max[0u], -80.0)) : 0.0
        let s0 = subgroupAdd(e)
        if (gl_SubgroupInvocationID == 0u) {
            part[gl_SubgroupID] = s0
        }
        barrier()
        if (tid == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += part[sgi]
                sgi++
            }
            wg_sum[0u] = tot
        }
        barrier()
        var p = live ? e / wg_sum[0u] : -3.0e38
        var j = 0u
        while (j < pa.k) {
            let vm = subgroupMax(p)
            let im = subgroupMin(p == vm ? tid : 0xFFFFFFFFu)
            if (gl_SubgroupInvocationID == 0u) {
                part[gl_SubgroupID] = vm
                parti[gl_SubgroupID] = im
            }
            barrier()
            if (tid == 0u) {
                var bv = part[0u]
                var bi = parti[0u]
                var sgi = 1u
                while (sgi < gl_NumSubgroups) {
                    if (part[sgi] > bv || (part[sgi] == bv && parti[sgi] < bi)) {
                        bv = part[sgi]
                        bi = parti[sgi]
                    }
                    sgi++
                }
                pick[j] = bi
                pickw[j] = bv
            }
            barrier()
            if (tid == pick[j]) {
                p = -3.0e38
            }
            j++
        }
        if (tid == 0u) {
            write_picks()
        }
    }

    def write_picks {
        var ws = 0.0
        var jj = 0u
        while (jj < pa.k) {
            ws += pickw[jj]
            jj++
        }
        ws = max(ws, F16_MIN_NORMAL)
        jj = 0u
        while (jj < pa.k) {
            var wj = pickw[jj]
            if (pa.norm != 0u) {
                wj /= ws
            }
            if (pa.wscale != 0.0 && pa.wscale != 1.0) {
                wj *= pa.wscale
            }
            w[jj] = wj
            let ex = min(pick[jj], pa.ne - 1u)   // a NaN row leaves the sentinel: never a wild weight block
            meta1[jj * 2u] = pa.base1 + ex * pa.stride1
            meta1[jj * 2u + 1u] = 0u
            meta3[jj * 2u] = pa.base3 + ex * pa.stride3
            meta3[jj * 2u + 1u] = 0u
            meta2[jj * 2u] = pa.base2 + ex * pa.stride2
            meta2[jj * 2u + 1u] = jj * pa.xnb2
            jj++
        }
    }
}

struct GatherArgs {
    n : uint    // row width
    k : uint    // slots per position (grid entry r sits at position r / k)
    r0 : uint   // this dispatch's bucket-row window [r0, r1)
    r1 : uint
}

[vk_dispatch(name = "moe_gather_f16_cls", grid = "nk", params = "nk : int64")]
class MoeGatherF16 {
    @ssbo @binding = 0 x : array<float>        // the normed activation rows, one per position
    @ssbo @binding = 1 slot_idx : array<uint>  // position -> its k slot rows (the combine's map)
    @ssbo @binding = 2 xg : array<float16>     // the gathered rows, bucket order, window-relative
    @push_constant pa : GatherArgs

    [spirv_kernel(local_size_x = 256, name = "moe_gather_f16_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-expert-chain")]
    def run {
        let r = gl_WorkGroupID.x
        let b = slot_idx[r]
        if (b < pa.r0 || b >= pa.r1) {
            return
        }
        let src = (r / pa.k) * pa.n
        let dst = (b - pa.r0) * pa.n
        var i = gl_LocalInvocationID.x
        while (i < pa.n) {
            xg[dst + i] = float16(clamp(x[src + i], -65504.0, 65504.0))
            i += 256u
        }
    }
}

// ===== deltanet: the conv front =====

struct DnConvArgs {
    cd : uint         // conv width (channels)
    kd : uint         // q/k region width (2*kd channels L2-normed in ds-blocks)
    ds : uint         // d_state (the norm block)
    dconv : uint      // conv taps + 1
    hist_mode : uint  // 1 = per-channel history rows, else per-position
    taps_off : uint   // offsets into the smalls member
    hist_off : uint
    eps : float
    qscale : float
}

// depthwise conv + SiLU + per-head q/k L2-norm, one workgroup per position; every handoff stays
// in shared, each global output element has exactly one writer
[vk_dispatch(name = "dn_conv_cls", grid = "npos", params = "npos : int64")]
class DnConv {
    @ssbo @binding = 0 src : array<float>      // the projected rows (position-major)
    @ssbo @binding = 1 outr : array<float>     // conv output rows
    @ssbo @binding = 2 smalls : array<float>   // taps + history + any small planes (push offsets)
    @push_constant pa : DnConvArgs
    @workgroup row : float[8192]               // the whole position row, silu'd
    @workgroup nsc : float[64]                 // per-block L2 scales

    [spirv_kernel(local_size_x = 256, name = "dn_conv_cls_spv")]
    def run {
        let p = gl_WorkGroupID.x
        let taps = pa.dconv - 1u
        let tid = gl_LocalInvocationID.x
        // depthwise conv + SiLU into shared, strided channels; taps in the raw fblob layout
        var ch = tid
        while (ch < pa.cd) {
            var acc = 0.0
            var k = 0u
            while (k < pa.dconv) {
                let sp = int(p + k) - int(taps)
                var xv = 0.0
                if (sp >= 0) {
                    xv = src[uint(sp) * pa.cd + ch]
                } elif (pa.hist_mode == 1u) {
                    xv = smalls[pa.hist_off + ch * taps + p + k]
                } else {
                    xv = smalls[pa.hist_off + (p + k) * pa.cd + ch]
                }
                acc += xv * smalls[pa.taps_off + ch * pa.dconv + k]
                k++
            }
            row[ch] = acc / (1.0 + exp(-acc))
            ch += 256u
        }
        barrier()
        // per-head L2 norm scale over the q and k regions, q pre-scale folded in
        let nblk = (2u * pa.kd) / pa.ds
        if (tid < nblk) {
            var sum = 0.0
            var j = 0u
            while (j < pa.ds) {
                let v = row[tid * pa.ds + j]
                sum += v * v
                j++
            }
            nsc[tid] = (1.0 / max(sqrt(sum), pa.eps)) * (tid < pa.kd / pa.ds ? pa.qscale : 1.0)
        }
        barrier()
        ch = tid
        while (ch < pa.cd) {
            let s = ch < 2u * pa.kd ? nsc[ch / pa.ds] : 1.0
            outr[p * pa.cd + ch] = row[ch] * s
            ch += 256u
        }
    }
}

// ===== deltanet: the fused decode step =====

struct DnStepArgs {
    cd : uint
    kd : uint
    nkh : uint
    ds : uint
    dconv : uint
    beta_off : uint
    g_off : uint
    taps_off : uint
    wnorm_off : uint
    a_off : uint
    dt_off : uint
    hist_off : uint
    eps : float
    qscale : float   // the conv-ring parity rides tok[0].parity
}

// the decode-step FUSED chain (one workgroup per v-head): conv preamble -> chunk-length-1 delta
// rule IN PLACE on the persistent state -> gated out-norm -> the head's Q8_0 requant. The conv
// ring is parity-double-buffered inside the smalls member.
[vk_dispatch(name = "dn_step_cls", grid = "nvh", params = "nvh : int64")]
class DnStepFused {
    @ssbo @binding = 0 proj : array<float>     // the fresh projection row (+ z rows at cd)
    @ssbo @binding = 1 state : array<float>    // the persistent per-head f32 state (in place)
    @ssbo @binding = 2 smalls : array<float>   // beta/g/taps/wnorm/a/dt + the conv HISTORY ring (rw)
    @ssbo @binding = 3 outs : array<float>     // the head's Q8_0 scales
    @ssbo @binding = 4 outq : array<uint>      // ... and quant words
    @ssbo @binding = 5 tok : array<TokMeta>    // the shared per-token block (element 0)
    @push_constant pa : DnStepArgs
    @workgroup dq : float[128]
    @workgroup dk : float[128]
    @workgroup dv : float[128]
    @workgroup dvn : float[128]
    @workgroup dout : float[128]
    @workgroup ds8 : float[8]

    [spirv_kernel(local_size_x = 256, name = "dn_step_cls_spv")]
    def run {   // nolint:STYLE037,STYLE038 — the verbatim fused chain, parity-locked to the shipped kernel
        let h = gl_WorkGroupID.x
        let ds = pa.ds
        let taps = pa.dconv - 1u
        let par = tok[0u].parity
        let rrb = pa.hist_off + par * pa.cd * taps
        let wrb = pa.hist_off + (1u - par) * pa.cd * taps
        let tid = gl_LocalInvocationID.x
        let kh = h % pa.nkh
        var e = tid
        while (e < 3u * ds) {
            let sl = e / ds
            let j = e % ds
            let ch = (sl == 0u ? kh * ds : (sl == 1u ? pa.kd + kh * ds : 2u * pa.kd + h * ds)) + j
            let fresh = proj[ch]
            var acc = fresh * smalls[pa.taps_off + ch * pa.dconv + taps]
            var k = 0u
            while (k < taps) {
                acc += smalls[rrb + ch * taps + k] * smalls[pa.taps_off + ch * pa.dconv + k]
                k++
            }
            if (sl == 2u || h < pa.nkh) {   // this head owns the channel's ring advance
                k = 0u
                while (k + 1u < taps) {
                    smalls[wrb + ch * taps + k] = smalls[rrb + ch * taps + k + 1u]
                    k++
                }
                smalls[wrb + ch * taps + taps - 1u] = fresh
            }
            let sv = acc / (1.0 + exp(-acc))
            if (sl == 0u) {
                dq[j] = sv
            } elif (sl == 1u) {
                dk[j] = sv
            } else {
                dv[j] = sv
            }
            e += 256u
        }
        barrier()
        // q/k L2 scales + the per-token beta/g transforms — tiny serial lanes (ds is 128-class)
        if (tid < 2u) {
            var sum = 0.0
            var j = 0u
            while (j < ds) {
                let v = tid == 0u ? dq[j] : dk[j]
                sum += v * v
                j++
            }
            ds8[tid] = (1.0 / max(sqrt(sum), pa.eps)) * (tid == 0u ? pa.qscale : 1.0)
        } elif (tid == 2u) {
            let graw = smalls[pa.g_off + h] + smalls[pa.dt_off + h]
            ds8[2u] = exp(smalls[pa.a_off + h] * (graw > 20.0 ? graw : log(1.0 + exp(graw))))
            ds8[3u] = 1.0 / (1.0 + exp(-smalls[pa.beta_off + h]))
        }
        barrier()
        e = tid
        while (e < ds) {
            dq[e] = dq[e] * ds8[0u]
            dk[e] = dk[e] * ds8[1u]
            e += 256u
        }
        barrier()
        if (tid == 0u) {
            var qk = 0.0
            var j = 0u
            while (j < ds) {
                qk += dq[j] * dk[j]
                j++
            }
            ds8[4u] = qk
        }
        barrier()
        // the delta rule at chunk length 1, state in place
        let sbase = h * ds * ds
        let eg = ds8[2u]
        let bt = ds8[3u]
        if (tid < ds) {
            var ks = 0.0
            var qs = 0.0
            var i = 0u
            while (i < ds) {
                let sv = state[sbase + i * ds + tid]
                ks += dk[i] * sv
                qs += dq[i] * sv
                i++
            }
            let vn = bt * (dv[tid] - eg * ks)
            dvn[tid] = vn
            dout[tid] = eg * qs + ds8[4u] * vn
        }
        barrier()
        e = tid
        while (e < ds * ds) {
            state[sbase + e] = state[sbase + e] * eg + dk[e / ds] * dvn[e % ds]
            e += 256u
        }
        if (tid == 0u) {
            var sum = 0.0
            var j = 0u
            while (j < ds) {
                sum += dout[j] * dout[j]
                j++
            }
            ds8[5u] = 1.0 / sqrt(sum / float(ds) + pa.eps)
        }
        barrier()
        e = tid
        while (e < ds) {
            let zv = proj[pa.cd + h * ds + e]
            let xn = dout[e] * ds8[5u] * smalls[pa.wnorm_off + e]
            dout[e] = xn * (zv / (1.0 + exp(-zv)))
            e += 256u
        }
        barrier()
        // the head's Q8_0 blocks (ds % 32 asserted host-side)
        if (tid < ds / 32u) {
            let gb = (h * ds) / 32u + tid
            let jb = tid * 32u
            var amax = 0.0
            for (i in range(32)) {
                amax = max(amax, abs(dout[jb + uint(i)]))
            }
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            outs[gb] = d
            for (wi in range(8)) {
                let wj = jb + uint(wi) * 4u
                var word = 0u
                for (j in range(4)) {
                    let q = int(round(dout[wj + uint(j)] * id))
                    word = word | ((uint(q) & 0xFF) << (uint(j) * 8u))
                }
                outq[gb * 8u + uint(wi)] = word
            }
        }
    }
}

// ===== deltanet: the two-phase chunked scan =====

// the slab + staged-GEMM machinery both phases share: C[MxN] (+)= A[MxK] * B[KxN] over the
// DN_WS work surface, 16-deep K stages through shared, C in per-thread Private registers
class DnScanBase {
    @ssbo @binding = 2 slab : array<float>   // the DN_WS work surface (every GEMM operand)
    @workgroup tm : float[4096]              // the 64x64 unit-lower solve tile
    @workgroup ta : float[2048]
    @workgroup tb : float[2048]
    @private_global acc64 : float[64]        // per-invocation C accumulator (ds*ds/256 max)

    def gemm(cofs, aofs, bofs : uint; mm, kk, nn : uint; lda, ldb, ldc : uint; accum : uint) {
        let tid = gl_LocalInvocationID.x
        var o = tid
        var oi = 0u
        while (o < mm * nn) {
            acc64[oi] = accum == 0u ? 0.0 : slab[cofs + (o / nn) * ldc + o % nn]
            oi++
            o += 256u
        }
        var kb = 0u
        while (kb < kk) {
            let kt = min(kk - kb, 16u)
            var i = tid
            while (i < mm * kt) {
                ta[(i / kt) * 16u + i % kt] = slab[aofs + (i / kt) * lda + kb + i % kt]
                i += 256u
            }
            i = tid
            while (i < kt * nn) {
                tb[(i / nn) * 128u + i % nn] = slab[bofs + (kb + i / nn) * ldb + i % nn]
                i += 256u
            }
            barrier()
            o = tid
            oi = 0u
            while (o < mm * nn) {
                let r = o / nn
                let c = o % nn
                var acc = acc64[oi]
                var k2 = 0u
                while (k2 < kt) {
                    acc += ta[r * 16u + k2] * tb[k2 * 128u + c]
                    k2++
                }
                acc64[oi] = acc
                oi++
                o += 256u
            }
            barrier()
            kb += 16u
        }
        o = tid
        oi = 0u
        while (o < mm * nn) {
            slab[cofs + (o / nn) * ldc + o % nn] = acc64[oi]
            oi++
            o += 256u
        }
        memoryBarrierBuffer()
        barrier()
    }
}

struct DnScanP1Args {
    npos : uint
    cd : uint
    kd : uint
    nvh : uint
    nkh : uint
    ds : uint
    beta_off : uint
    g_off : uint
    a_off : uint
    dt_off : uint
    slab_stride : uint   // floats per (head x chunk) slab region
}

// phase 1: every state-independent piece per (head x chunk) — panels, decay-scaled A, the
// unit-lower solve, the local vnew half, the transition operand
[vk_dispatch(name = "dn_scan_p1_cls", grid = "wgs", params = "wgs : int64")]
class DnScanP1 : DnScanBase {
    @ssbo @binding = 0 conv : array<float>     // the conv output rows (position-major)
    @ssbo @binding = 1 smalls : array<float>   // beta/g/a/dt planes (push offsets)
    @push_constant pa : DnScanP1Args

    [spirv_kernel(local_size_x = 256, name = "dn_scan_p1_cls_spv")]
    def run {   // nolint:STYLE038 — the verbatim phase-1 pass, parity-locked to the shipped kernel
        let ds = pa.ds
        let tid = gl_LocalInvocationID.x
        let CS = 64u
        let nch = (pa.npos + CS - 1u) / CS
        let wid = gl_WorkGroupID.x
        let h = wid / nch
        let c0 = (wid % nch) * CS
        let cl = min(CS, pa.npos - c0)
        let slb = pa.slab_stride * wid
        let qc = slb
        let kcT = slb + CS * ds
        let kb = slb + 2u * CS * ds
        let vb = slb + 3u * CS * ds
        let w = slb + 4u * CS * ds
        let vnew = slb + 5u * CS * ds
        let kgT = slb + 6u * CS * ds
        let tmg = slb + 7u * CS * ds
        let kq = tmg + CS * CS
        let gcum = kq + CS * CS
        let egc = gcum + CS
        let kgc = gcum + 2u * CS
        let bvec = gcum + 3u * CS
        let kh = h % pa.nkh
        let qoff = kh * ds
        let koff = pa.kd + kh * ds
        let voff = 2u * pa.kd + h * ds
        let aa = smalls[pa.a_off + h]
        let dtb = smalls[pa.dt_off + h]
        // per-token transforms: beta = sigmoid(raw), g-cumsum of a*softplus(raw + dt)
        if (tid == 0u) {
            var gacc = 0.0
            var t = 0u
            while (t < cl) {
                let graw = smalls[pa.g_off + (c0 + t) * pa.nvh + h] + dtb
                gacc += aa * (graw > 20.0 ? graw : log(1.0 + exp(graw)))
                slab[gcum + t] = gacc
                slab[egc + t] = exp(gacc)
                slab[bvec + t] = 1.0 / (1.0 + exp(-smalls[pa.beta_off + (c0 + t) * pa.nvh + h]))
                t++
            }
        }
        memoryBarrierBuffer()
        barrier()
        // panels off the conv rows: qc/kb/vb row-major cl x ds, kcT transposed ds x CS
        var e = tid
        while (e < cl * ds) {
            let i = e / ds
            let j = e % ds
            let crow = (c0 + i) * pa.cd
            let kv = conv[crow + koff + j]
            let bv = slab[bvec + i]
            slab[qc + i * ds + j] = conv[crow + qoff + j]
            slab[kcT + j * CS + i] = kv
            slab[kb + i * ds + j] = kv * bv
            slab[vb + i * ds + j] = conv[crow + voff + j] * bv
            e += 256u
        }
        memoryBarrierBuffer()
        barrier()
        // A = (k_beta k^T) decay-scaled strictly lower (negated); KQ keeps its diagonal
        gemm(tmg, kb, kcT, cl, ds, cl, ds, CS, CS, 0u)
        gemm(kq, qc, kcT, cl, ds, cl, ds, CS, CS, 0u)
        e = tid
        while (e < cl * cl) {
            let i = e / cl
            let j = e % cl
            if (j < i) {
                let d = exp(slab[gcum + i] - slab[gcum + j])
                slab[tmg + i * CS + j] = -slab[tmg + i * CS + j] * d
                slab[kq + i * CS + j] = slab[kq + i * CS + j] * d
            } elif (j == i) {
                slab[tmg + i * CS + j] = 1.0
            } else {
                slab[tmg + i * CS + j] = 0.0
                slab[kq + i * CS + j] = 0.0
            }
            e += 256u
        }
        memoryBarrierBuffer()
        barrier()
        // unit-lower solve in shared, rows finalized top-down
        e = tid
        while (e < cl * cl) {
            tm[(e / cl) * CS + e % cl] = slab[tmg + (e / cl) * CS + e % cl]
            e += 256u
        }
        barrier()
        var i2 = 1u
        while (i2 < cl) {
            var fin = 0.0
            if (tid < i2) {
                fin = tm[i2 * CS + tid]
                var t2 = tid + 1u
                while (t2 < i2) {
                    fin += tm[i2 * CS + t2] * tm[t2 * CS + tid]
                    t2++
                }
            }
            barrier()
            if (tid < i2) {
                tm[i2 * CS + tid] = fin
            }
            barrier()
            i2++
        }
        e = tid
        while (e < cl * cl) {
            slab[tmg + (e / cl) * CS + e % cl] = tm[(e / cl) * CS + e % cl]
            e += 256u
        }
        // d = T v_beta - (T (k_beta e^gcum)) S — kb scaled in place, minus folded into the scale
        e = tid
        while (e < cl * ds) {
            slab[kb + e] = slab[kb + e] * -slab[egc + e / ds]
            e += 256u
        }
        memoryBarrierBuffer()
        barrier()
        gemm(w, tmg, kb, cl, cl, ds, CS, ds, ds, 0u)
        gemm(vnew, tmg, vb, cl, cl, ds, CS, ds, ds, 0u)
        // qc scaled in place to (q e^gcum) — phase 2's och GEMM reads it directly
        e = tid
        while (e < cl * ds) {
            slab[qc + e] = slab[qc + e] * slab[egc + e / ds]
            e += 256u
        }
        // transition piece: kgT = (k e^{glast - gcum})^T, phase 2's state-advance A operand
        let glast = slab[gcum + cl - 1u]
        if (tid < cl) {
            slab[kgc + tid] = exp(glast - slab[gcum + tid])
        }
        memoryBarrierBuffer()
        barrier()
        e = tid
        while (e < ds * cl) {
            let j = e / cl
            let i = e % cl
            slab[kgT + j * CS + i] = slab[kcT + j * CS + i] * slab[kgc + i]
            e += 256u
        }
    }
}

struct DnScanP2Args {
    npos : uint
    di : uint
    ds : uint
    o_off : uint
    wnorm_off : uint
    slab_stride : uint
    st_off : uint    // per-head state+och working region base
    eps : float
}

// phase 2: chunks sequentially, only the 4 state-dependent GEMMs + the gated out-norm
[vk_dispatch(name = "dn_scan_p2_cls", grid = "nvh", params = "nvh : int64")]
class DnScanP2 : DnScanBase {
    @ssbo @binding = 0 zrows : array<float>    // the z gate rows
    @ssbo @binding = 1 state : array<float>    // the persistent per-head f32 state
    @ssbo @binding = 3 smalls : array<float>   // wnorm (push offset)
    @push_constant pa : DnScanP2Args

    [spirv_kernel(local_size_x = 256, name = "dn_scan_p2_cls_spv")]
    def run {
        let h = gl_WorkGroupID.x
        let ds = pa.ds
        let tid = gl_LocalInvocationID.x
        let CS = 64u
        let nch = (pa.npos + CS - 1u) / CS
        let st = pa.st_off + h * (ds * ds + CS * ds)
        let och = st + ds * ds
        // the persistent state works in its slab copy (one GEMM surface for everything)
        var i0 = tid
        while (i0 < ds * ds) {
            slab[st + i0] = state[h * ds * ds + i0]
            i0 += 256u
        }
        memoryBarrierBuffer()
        barrier()
        var c = 0u
        while (c < nch) {
            let c0 = c * CS
            let cl = min(CS, pa.npos - c0)
            let slb = pa.slab_stride * (h * nch + c)
            let qc = slb
            let w = slb + 4u * CS * ds
            let vnew = slb + 5u * CS * ds
            let kgT = slb + 6u * CS * ds
            let kq = slb + 7u * CS * ds + CS * CS
            let gcum = slb + 7u * CS * ds + 2u * CS * CS
            // d = vnloc + w S (in place over phase 1's local half)
            gemm(vnew, w, st, cl, ds, ds, ds, ds, ds, 1u)
            // o = (q e^gcum) S + KQ d
            gemm(och, qc, st, cl, ds, ds, ds, ds, ds, 0u)
            gemm(och, kq, vnew, cl, cl, ds, CS, ds, ds, 1u)
            // gated out-norm straight into the o rows: rms(och) * wnorm, silu(z)-gated
            if (tid < cl) {
                let ob = och + tid * ds
                var sum = 0.0
                var j3 = 0u
                while (j3 < ds) {
                    let v = slab[ob + j3]
                    sum += v * v
                    j3++
                }
                let scale = 1.0 / sqrt(sum / float(ds) + pa.eps)
                let orow = pa.o_off + (c0 + tid) * pa.di + h * ds
                let zrow = (c0 + tid) * pa.di + h * ds
                j3 = 0u
                while (j3 < ds) {
                    let zv = zrows[zrow + j3]
                    let xn = slab[ob + j3] * scale * smalls[pa.wnorm_off + j3]
                    slab[orow + j3] = xn * (zv / (1.0 + exp(-zv)))
                    j3++
                }
            }
            barrier()
            // state advance, once per chunk: S = e^glast S + kgT d
            let egl = exp(slab[gcum + cl - 1u])
            var e = tid
            while (e < ds * ds) {
                slab[st + e] = slab[st + e] * egl
                e += 256u
            }
            memoryBarrierBuffer()
            barrier()
            gemm(st, kgT, vnew, ds, cl, ds, CS, ds, ds, 1u)
            c++
        }
        // the advanced state back to its persistent plane
        i0 = tid
        while (i0 < ds * ds) {
            state[h * ds * ds + i0] = slab[st + i0]
            i0 += 256u
        }
    }
}

// ===== prefill attention: the prep pass =====

struct AtPrepArgs {
    d : uint         // this pass's row width (qd for q, kvd for k)
    nh : uint        // heads in this pass
    hs : uint
    half : uint      // partial-rope half (0 = no rope)
    flags : uint     // bit 0 = gated q, bit 1 = qk-norm
    rms_off : uint   // this pass's norm-weight row in smalls
    cos_off : uint   // per-position cos rows in smalls
    sin_off : uint
    obase : uint     // output panel's row base (0 for q, npos for k)
    qsrc : uint      // 1 = the q source (gating applies), 0 = k
    eps : float
}

// deinterleave (gated q), per-head RMSNorm, partial NEOX rope — ONE kernel where the old rail
// had at_prep_q/at_prep_k twins: the distinction was pure parameters, so the driver dispatches
// this twice with different push values.
[vk_dispatch(name = "at_prep_cls", grid = "npos", params = "npos : int64")]
class AtPrep {
    @ssbo @binding = 0 src : array<float>      // the projection rows
    @ssbo @binding = 1 outp : array<float>     // the prepped panel rows
    @ssbo @binding = 2 gates : array<float>    // the gate stash (gated-q models; written only then)
    @ssbo @binding = 3 smalls : array<float>   // cos/sin rows + norm weights (push offsets)
    @push_constant pa : AtPrepArgs
    @workgroup row : float[8192]               // one position's projection row (d <= 8192)
    @workgroup hsinv : float[64]               // per-head inverse rms (heads <= 64)

    [spirv_kernel(local_size_x = 256, name = "at_prep_cls_spv")]
    def run {
        let p = gl_WorkGroupID.x
        let hs = pa.hs
        let half = pa.half
        let gated = pa.qsrc != 0u ? (pa.flags & 1u) : 0u
        let qknorm = pa.flags & 2u
        let tid = gl_LocalInvocationID.x
        var i = tid
        while (i < pa.d) {
            let h = i / hs
            let j = i % hs
            if (gated != 0u) {
                row[i] = src[p * 2u * pa.d + h * 2u * hs + j]
                gates[p * pa.d + i] = src[p * 2u * pa.d + h * 2u * hs + hs + j]
            } else {
                row[i] = src[p * pa.d + i]
            }
            i += 256u
        }
        barrier()
        if (qknorm != 0u && tid < pa.nh) {
            var ss = 0.0
            var j = 0u
            while (j < hs) {
                let v = row[tid * hs + j]
                ss += v * v
                j++
            }
            hsinv[tid] = 1.0 / sqrt(ss / float(hs) + pa.eps)
        }
        barrier()
        i = tid
        while (i < pa.d) {
            let h = i / hs
            let j = i % hs
            var v = row[i]
            if (qknorm != 0u) {
                v = smalls[pa.rms_off + j] * (v * hsinv[h])
            }
            var outv = v
            if (j < half) {
                var v1 = row[h * hs + j + half]
                if (qknorm != 0u) {
                    v1 = smalls[pa.rms_off + j + half] * (v1 * hsinv[h])
                }
                outv = v * smalls[pa.cos_off + p * half + j] - v1 * smalls[pa.sin_off + p * half + j]
            } elif (j < 2u * half) {
                let jj = j - half
                var v0 = row[h * hs + jj]
                if (qknorm != 0u) {
                    v0 = smalls[pa.rms_off + jj] * (v0 * hsinv[h])
                }
                outv = v0 * smalls[pa.sin_off + p * half + jj] + v * smalls[pa.cos_off + p * half + jj]
            }
            outp[(pa.obase + p) * pa.d + i] = outv
            i += 256u
        }
    }
}

// ===== prefill attention: the flash Q-tile =====

struct AtAttnArgs {
    rows : uint     // window query rows
    w0 : uint       // window base (queries attend positions [0, w0 + row])
    qd : uint
    kvd : uint
    hs : uint
    kv_mul : uint
    flags : uint    // bit 0 = gated output
    dpl : uint      // output dims per lane (hs / 32)
    scale : float
}

// the flash Q-tile core (AtAttn and DaAttnB share it; the h128 coopmat twin does not): staged
// K/V tiles at stride 257, per-row online softmax. KT is the K/V plane element type (staging and
// compute stay f32 — the f16 arm converts on load). The template also carries the mirror-window
// kernel (DaAttnB's run); AtAttn authors its own run over the same core. Leaves bring their own
// out plane + push args, so the methods take every varying scalar as a param
[ |> template_struct_instance]
class template AtAttnTileT {
    @ssbo @binding = 0 qpanel : array<float>   // the prepped/roped q rows
    @ssbo @binding = 1 kpanel : array<KT>      // the K plane (panel or mirror — kbase offsets)
    @ssbo @binding = 2 vpanel : array<KT>      // the V plane (raw rows or mirror — vbase offsets)
    @workgroup fq : float[2048]    // the tile's 8 q rows
    @workgroup fkv : float[8224]   // one 32-row K/V stage, stride 257 (bank-spread)
    @workgroup fp : float[256]     // the 8x32 score/probability tile
    @workgroup fm : float[8]       // per-row online-softmax state
    @workgroup fl : float[8]
    @workgroup fal : float[8]

    // stage the 8-query tile and reset the online-softmax state
    def load_q(q0, nrows, qd, h, hs : uint) {
        let tid = gl_LocalInvocationID.x
        var i = tid
        while (i < 8u * hs) {
            let qr = q0 + i / hs
            fq[i] = qr < nrows ? qpanel[qr * qd + h * hs + i % hs] : 0.0
            i += 256u
        }
        if (tid < 8u) {
            fm[tid] = -3.0e38
            fl[tid] = 0.0
        }
    }

    // stage one 32-position K tile from kpanel at kbase
    def load_k(k0, kend, kbase, kvd, kvh, hs : uint) {
        var i = gl_LocalInvocationID.x
        while (i < 32u * hs) {
            let kr = k0 + i / hs
            fkv[(i / hs) * 257u + i % hs] = kr < kend ? float(kpanel[kbase + kr * kvd + kvh * hs + i % hs]) : 0.0   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
            i += 256u
        }
    }

    // ... and the V tile reuse of the K staging at vbase (call only after all K reads completed)
    def load_v(k0, kend, vbase, kvd, kvh, hs : uint) {
        var i = gl_LocalInvocationID.x
        while (i < 32u * hs) {
            let kr = k0 + i / hs
            fkv[(i / hs) * 257u + i % hs] = kr < kend ? float(vpanel[vbase + kr * kvd + kvh * hs + i % hs]) : 0.0   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
            i += 256u
        }
    }

    // tile QK^T: lane (r, c) scores query r against position k0+c, causal-masked into fp
    def qk_score(r, c, hs, k0, gq, q0, nrows : uint; scale : float) {
        var s = 0.0
        var j = 0u
        while (j < hs) {   // 4-wide manual unroll (hs % 32 == 0)
            s += fq[r * hs + j] * fkv[c * 257u + j]
            s += fq[r * hs + j + 1u] * fkv[c * 257u + j + 1u]
            s += fq[r * hs + j + 2u] * fkv[c * 257u + j + 2u]
            s += fq[r * hs + j + 3u] * fkv[c * 257u + j + 3u]
            j += 4u
        }
        let gk = k0 + c
        fp[r * 32u + c] = (gk <= gq && q0 + r < nrows) ? s * scale : -3.0e38
    }

    // per-row online-softmax update: max, rescale, exponentiate in place
    def softmax_update {
        let tid = gl_LocalInvocationID.x
        if (tid < 8u) {
            var mt = fm[tid]
            var j2 = 0u
            while (j2 < 32u) {
                mt = max(mt, fp[tid * 32u + j2])
                j2++
            }
            let alpha = exp(fm[tid] - mt)
            var lsum = 0.0
            j2 = 0u
            while (j2 < 32u) {
                let e = exp(fp[tid * 32u + j2] - mt)
                fp[tid * 32u + j2] = e
                lsum += e
                j2++
            }
            fl[tid] = fl[tid] * alpha + lsum
            fm[tid] = mt
            fal[tid] = alpha
        }
    }

    // the mirror-window kernel (DaAttnB's pass): batched causal attention over the decode
    // mirrors, K/V read at kbase/vbase, out = plain rows (no gating); dpl-guarded accumulators
    // serve hs 32..256. pa/outp are instance-declared (DaAttnBArgs); AtAttn overrides run
    [spirv_kernel(local_size_x = 256)]
    def run {   // nolint:STYLE038 — the verbatim flash pass, parity-locked to the shipped kernel
        let qd = pa.nh * pa.hs
        let dpl = pa.hs / 32u
        let qtiles = (pa.npos + 7u) / 8u
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * 8u
        let tid = gl_LocalInvocationID.x
        let r = tid / 32u
        let c = tid % 32u
        let kvh = h / pa.kv_mul
        load_q(q0, pa.npos, qd, h, pa.hs)
        var o0 = 0.0
        var o1 = 0.0
        var o2 = 0.0
        var o3 = 0.0
        var o4 = 0.0
        var o5 = 0.0
        var o6 = 0.0
        var o7 = 0.0
        barrier()
        let gq = pa.w0 + q0 + r
        let kend = pa.w0 + min(q0 + 8u, pa.npos)   // causal bound (exclusive) of the tile's last row
        var k0 = 0u
        while (k0 < kend) {
            load_k(k0, kend, pa.kbase, pa.kvd, kvh, pa.hs)
            barrier()
            qk_score(r, c, pa.hs, k0, gq, q0, pa.npos, pa.scale)
            barrier()
            softmax_update()
            barrier()
            let av = fal[r]
            o0 *= av
            if (dpl > 1u) {
                o1 *= av
            }
            if (dpl > 2u) {
                o2 *= av
                o3 *= av
            }
            if (dpl > 4u) {
                o4 *= av
                o5 *= av
                o6 *= av
                o7 *= av
            }
            load_v(k0, kend, pa.vbase, pa.kvd, kvh, pa.hs)
            barrier()
            var c2 = 0u
            while (c2 < 32u) {
                let pw = fp[r * 32u + c2]
                let vb = c2 * 257u + c
                o0 += pw * fkv[vb]
                if (dpl > 1u) {
                    o1 += pw * fkv[vb + 32u]
                }
                if (dpl > 2u) {
                    o2 += pw * fkv[vb + 64u]
                    o3 += pw * fkv[vb + 96u]
                }
                if (dpl > 4u) {
                    o4 += pw * fkv[vb + 128u]
                    o5 += pw * fkv[vb + 160u]
                    o6 += pw * fkv[vb + 192u]
                    o7 += pw * fkv[vb + 224u]
                }
                c2++
            }
            barrier()
            k0 += 32u
        }
        if (q0 + r < pa.npos) {
            let inv = 1.0 / fl[r]
            let ob = (q0 + r) * qd + h * pa.hs + c
            var jj = 0u
            while (jj < dpl) {
                outp[ob + jj * 32u] = (jj == 0u ? o0 : (jj == 1u ? o1 : (jj == 2u ? o2 : (jj == 3u ? o3
                    : (jj == 4u ? o4 : (jj == 5u ? o5 : (jj == 6u ? o6 : o7))))))) * inv
                jj++
            }
        }
    }
}

// batched causal attention (prefill), flash-style Q tile: one wg per (head, 8-query tile);
// lane c owns dims c + 32*jj. V = the raw v rows; the epilogue gates through the out plane
[vk_dispatch(name = "at_attn_cls", grid = "wgs", params = "wgs : int64")]
class AtAttn : AtAttnTileT {
    typedef KT = float
    @ssbo @binding = 3 outp : array<float>   // attention out (gated models read the gate stash here)
    @push_constant pa : AtAttnArgs

    [spirv_kernel(local_size_x = 256, name = "at_attn_cls_spv")]
    def override run {
        let qtiles = (pa.rows + 7u) / 8u
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * 8u
        let tid = gl_LocalInvocationID.x
        let r = tid / 32u
        let c = tid % 32u
        let kvh = h / pa.kv_mul
        load_q(q0, pa.rows, pa.qd, h, pa.hs)
        var o0 = 0.0
        var o1 = 0.0
        var o2 = 0.0
        var o3 = 0.0
        var o4 = 0.0
        var o5 = 0.0
        var o6 = 0.0
        var o7 = 0.0
        barrier()
        let gq = pa.w0 + q0 + r
        let kend = pa.w0 + min(q0 + 8u, pa.rows)   // causal bound (exclusive) of the tile's last row
        var k0 = 0u
        while (k0 < kend) {
            load_k(k0, kend, 0u, pa.kvd, kvh, pa.hs)
            barrier()
            qk_score(r, c, pa.hs, k0, gq, q0, pa.rows, pa.scale)
            barrier()
            softmax_update()
            barrier()
            let av = fal[r]
            o0 *= av
            o1 *= av
            o2 *= av
            o3 *= av
            if (pa.dpl > 4u) {
                o4 *= av
                o5 *= av
                o6 *= av
                o7 *= av
            }
            load_v(k0, kend, 0u, pa.kvd, kvh, pa.hs)
            barrier()
            var c2 = 0u
            while (c2 < 32u) {
                let pw = fp[r * 32u + c2]
                let vb = c2 * 257u + c
                o0 += pw * fkv[vb]
                o1 += pw * fkv[vb + 32u]
                o2 += pw * fkv[vb + 64u]
                o3 += pw * fkv[vb + 96u]
                if (pa.dpl > 4u) {
                    o4 += pw * fkv[vb + 128u]
                    o5 += pw * fkv[vb + 160u]
                    o6 += pw * fkv[vb + 192u]
                    o7 += pw * fkv[vb + 224u]
                }
                c2++
            }
            barrier()
            k0 += 32u
        }
        if (q0 + r < pa.rows) {
            let inv = 1.0 / fl[r]
            let ob = (q0 + r) * pa.qd + h * pa.hs + c
            let gated = pa.flags & 1u
            var jj = 0u
            while (jj < pa.dpl) {
                var v = (jj == 0u ? o0 : (jj == 1u ? o1 : (jj == 2u ? o2 : (jj == 3u ? o3
                    : (jj == 4u ? o4 : (jj == 5u ? o5 : (jj == 6u ? o6 : o7))))))) * inv
                if (gated != 0u) {
                    let g = outp[ob + jj * 32u]
                    v = v / (1.0 + exp(-g))
                }
                outp[ob + jj * 32u] = v
                jj++
            }
        }
    }
}

// ===== cm2 flash attention (Br=Bc=64, online softmax on
// whole-workgroup tiles). One wg per (head, 64-query tile); Q loads as an f32 accumulator and
// use-converts to f16 A; K^T loads through the transpose view from the f16 K shadow; the causal
// mask computes on the fly in a per-element callback (no mask buffer); softmax state, the P
// conversion, and the O rescale ride the reduce/per-element/componentwise rails. =====

[spirv_combine]
def facm2_max(a, b : float) : float => max(a, b)

[spirv_combine, unused_argument(b)]
def facm2_smear(a, b : float) : float => a

[spirv_per_element, unused_argument(r, c)]
def facm2_exp(r, c : uint; v : float) : float => exp(v)

[spirv_per_element, unused_argument(r, c)]
def facm2_pick(r, c : uint; v, o : float) : float => max(v, o)

// causal: key kbase+c may not exceed query w0+q0+r (-FLT_MAX/2 keeps Mold-M NaN-free);
// padding rows keep their causal window so no row is all -inf (rowsum stays positive)
[spirv_per_element]
def facm2_mask(r, c : uint; v : float; qbase, kbase : uint) : float => kbase + c > qbase + r ? -1.7e38 : v

[spirv_per_element, unused_argument(r, c)]
def facm2_recip(r, c : uint; v : float) : float => v == 0.0 ? 0.0 : 1.0 / v

struct FaCm2Args {
    rows : uint     // window query rows
    w0 : uint       // window base (queries attend positions [0, w0 + row])
    qd : uint       // q/o row stride (n_heads * hs)
    kvd : uint      // k/v row stride (n_kv * hs)
    kv_mul : uint   // GQA ratio (head / kv_mul = kv head)
    kvbase : uint   // element base of this layer's plane in the K/V shadow mirrors
    scale : float
}

// hs=64 (tinyllama-class). The h128 twin below differs only in the head-size-shaped tiles —
// duplication tolerated until the kernel-reification arc.
[ |> template_struct_instance]
class template FaCm2H64T {
    @ssbo @binding = 0 qpanel : array<float>     // the prepped/roped window q rows (window-local)
    @ssbo @binding = 1 kh : array<float16>       // f16 K shadow at absolute positions
    @ssbo @binding = 2 vh : array<float16>       // f16 V shadow at absolute positions
    @ssbo @binding = 3 outp : array<OT>          // attention out plane (rows x qd)
    @push_constant pa : FaCm2Args
    @template_constant OUT16 : bool = false

    [spirv_kernel(local_size_x = 128), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
    def run {
        let qtiles = (pa.rows + 63u) / 64u
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * 64u
        let kvh = h / pa.kv_mul
        let kvlen = pa.w0 + pa.rows
        var tlq : tensorLayout2DPad
        tensorLayoutCreate(tlq)
        tensorLayoutSetDimension(tlq, pa.rows, pa.qd)
        tensorLayoutSetStride(tlq, pa.qd, 1u)
        var tlk : tensorLayout2DPad
        tensorLayoutCreate(tlk)
        tensorLayoutSetDimension(tlk, kvlen, pa.kvd)
        tensorLayoutSetStride(tlk, pa.kvd, 1u)
        var tlo : tensorLayout2DPad
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, pa.rows, pa.qd)
        tensorLayoutSetStride(tlo, pa.qd, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var q : coopmatWgAcc_f32_64x64
        coopmatLoadTensor(q, qpanel, 0u, tlq, q0, 64u, h * 64u, 64u)
        coopmatScale(q, pa.scale)
        var qf : coopmatWgA_f16_64x64
        coopmatConvert(qf, q)
        var mrow : coopmatWgAcc_f32_64x64
        coopmatFill(mrow, -1.7e38)
        var lrow : coopmatWgAcc_f32_64x64
        var o : coopmatWgAcc_f32_64x64
        let kmax = pa.w0 + min(q0 + 64u, pa.rows)   // causal bound (exclusive) of the tile's last row
        var j = 0u
        while (j < kmax) {
            var s : coopmatWgAcc_f32_64x64
            var kt : coopmatWgB_f16_64x64
            coopmatLoadTensor(kt, kh, pa.kvbase, tlk, j, 64u, kvh * 64u, 64u, tv)
            s = coopmatMulAdd(qf, kt, s)
            coopmatPerElement(s, s, @@facm2_mask, pa.w0 + q0, j)
            var rmax : coopmatWgAcc_f32_64x64
            coopmatReduce(rmax, s, COOPMAT_REDUCE_ROW, @@facm2_max)
            var mold : coopmatWgAcc_f32_64x64
            coopmatCopy(mold, mrow)
            coopmatPerElement(mrow, rmax, @@facm2_pick, mold)
            coopmatSub(s, s, mrow)
            coopmatPerElement(s, s, @@facm2_exp)                        // P = e^(S - M), in place
            var em : coopmatWgAcc_f32_64x64
            coopmatSub(em, mold, mrow)
            coopmatPerElement(em, em, @@facm2_exp)                      // eM = e^(Mold - M)
            var pf : coopmatWgA_f16_64x64
            coopmatConvert(pf, s)
            var rsum : coopmatWgAcc_f32_64x64
            var one : coopmatWgB_f16_64x64
            coopmatFill(one, 1.0)
            rsum = coopmatMulAdd(pf, one, rsum)                         // rowsum by ones-matrix multiply
            coopmatMulElem(lrow, em, lrow)
            coopmatAdd(lrow, lrow, rsum)                                // L = eM*L + rowsum
            var emd : coopmatWgAcc_f32_64x64
            coopmatReduce(emd, em, COOPMAT_REDUCE_ROW, @@facm2_smear)
            coopmatMulElem(o, emd, o)
            var vt : coopmatWgB_f16_64x64
            coopmatLoadTensor(vt, vh, pa.kvbase, tlk, j, 64u, kvh * 64u, 64u)
            o = coopmatMulAdd(pf, vt, o)                                // O = eMdiag*O + P @ V
            j += 64u
        }
        var ld : coopmatWgAcc_f32_64x64
        coopmatReduce(ld, lrow, COOPMAT_REDUCE_ROW, @@facm2_smear)
        coopmatPerElement(ld, ld, @@facm2_recip)
        coopmatMulElem(o, ld, o)                                        // O /= L (padding rows discard at store)
        static_if (OUT16) {
            var of : coopmatWgAcc_f16_64x64
            coopmatConvert(of, o)
            coopmatStoreTensor(of, outp, 0u, tlo, q0, 64u, h * 64u, 64u)
        } else {
            coopmatStoreTensor(o, outp, 0u, tlo, q0, 64u, h * 64u, 64u)
        }
    }
}

[vk_dispatch(name = "fa_cm2_h64_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64 : FaCm2H64T {
    typedef OT = float
}

[vk_dispatch(name = "fa_cm2_h64_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64F16 : FaCm2H64T {
    typedef OT = float16
    override OUT16 = true
}

// hs=128 (llama-3.2-3B-class): same algorithm at Bc=32 (the head-size cut — K^T/V/softmax
// state at 32 columns, KV loop steps 32). Fast standalone (24.7 TFLOP/s, right beside the h64) and
// WINS end-to-end when the shadows fit (3B 4287 vs 3955 no-fa) — serving is gated only on the
// resident plan learning the f16 shadow cost (see the pf_facm2 gate)
[ |> template_struct_instance]
class template FaCm2H128T {
    @ssbo @binding = 0 qpanel : array<float>
    @ssbo @binding = 1 kh : array<float16>
    @ssbo @binding = 2 vh : array<float16>
    @ssbo @binding = 3 outp : array<OT>
    @push_constant pa : FaCm2Args
    @template_constant OUT16 : bool = false

    [spirv_kernel(local_size_x = 128), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
    def run {
        let qtiles = (pa.rows + 63u) / 64u
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * 64u
        let kvh = h / pa.kv_mul
        let kvlen = pa.w0 + pa.rows
        var tlq : tensorLayout2DPad
        tensorLayoutCreate(tlq)
        tensorLayoutSetDimension(tlq, pa.rows, pa.qd)
        tensorLayoutSetStride(tlq, pa.qd, 1u)
        var tlk : tensorLayout2DPad
        tensorLayoutCreate(tlk)
        tensorLayoutSetDimension(tlk, kvlen, pa.kvd)
        tensorLayoutSetStride(tlk, pa.kvd, 1u)
        var tlo : tensorLayout2DPad
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, pa.rows, pa.qd)
        tensorLayoutSetStride(tlo, pa.qd, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var q : coopmatWgAcc_f32_64x128
        coopmatLoadTensor(q, qpanel, 0u, tlq, q0, 64u, h * 128u, 128u)
        coopmatScale(q, pa.scale)
        var qf : coopmatWgA_f16_64x128
        coopmatConvert(qf, q)
        var mrow : coopmatWgAcc_f32_64x32
        coopmatFill(mrow, -1.7e38)
        var lrow : coopmatWgAcc_f32_64x32
        var o : coopmatWgAcc_f32_64x128
        let kmax = pa.w0 + min(q0 + 64u, pa.rows)
        var j = 0u
        while (j < kmax) {
            var s : coopmatWgAcc_f32_64x32
            var kt : coopmatWgB_f16_128x32
            coopmatLoadTensor(kt, kh, pa.kvbase, tlk, j, 32u, kvh * 128u, 128u, tv)
            s = coopmatMulAdd(qf, kt, s)
            coopmatPerElement(s, s, @@facm2_mask, pa.w0 + q0, j)
            var rmax : coopmatWgAcc_f32_64x32
            coopmatReduce(rmax, s, COOPMAT_REDUCE_ROW, @@facm2_max)
            var mold : coopmatWgAcc_f32_64x32
            coopmatCopy(mold, mrow)
            coopmatPerElement(mrow, rmax, @@facm2_pick, mold)
            coopmatSub(s, s, mrow)
            coopmatPerElement(s, s, @@facm2_exp)
            var em : coopmatWgAcc_f32_64x32
            coopmatSub(em, mold, mrow)
            coopmatPerElement(em, em, @@facm2_exp)
            var pf : coopmatWgA_f16_64x32
            coopmatConvert(pf, s)
            var rsum : coopmatWgAcc_f32_64x32
            var one : coopmatWgB_f16_32x32
            coopmatFill(one, 1.0)
            rsum = coopmatMulAdd(pf, one, rsum)
            coopmatMulElem(lrow, em, lrow)
            coopmatAdd(lrow, lrow, rsum)
            var emd : coopmatWgAcc_f32_64x128
            coopmatReduce(emd, em, COOPMAT_REDUCE_ROW, @@facm2_smear)
            coopmatMulElem(o, emd, o)
            var vt : coopmatWgB_f16_32x128
            coopmatLoadTensor(vt, vh, pa.kvbase, tlk, j, 32u, kvh * 128u, 128u)
            o = coopmatMulAdd(pf, vt, o)
            j += 32u
        }
        var ld : coopmatWgAcc_f32_64x128
        coopmatReduce(ld, lrow, COOPMAT_REDUCE_ROW, @@facm2_smear)
        coopmatPerElement(ld, ld, @@facm2_recip)
        coopmatMulElem(o, ld, o)
        static_if (OUT16) {
            var of : coopmatWgAcc_f16_64x128
            coopmatConvert(of, o)
            coopmatStoreTensor(of, outp, 0u, tlo, q0, 64u, h * 128u, 128u)
        } else {
            coopmatStoreTensor(o, outp, 0u, tlo, q0, 64u, h * 128u, 128u)
        }
    }
}

[vk_dispatch(name = "fa_cm2_h128_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H128 : FaCm2H128T {
    typedef OT = float
}

[vk_dispatch(name = "fa_cm2_h128_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H128F16 : FaCm2H128T {
    typedef OT = float16
    override OUT16 = true
}

struct DaAttnBArgs {
    npos : uint     // window query rows
    nh : uint
    kvd : uint
    hs : uint
    kv_mul : uint
    kbase : uint    // word offsets into the K / V mirror planes
    vbase : uint
    w0 : uint       // window base (queries attend mirror positions [0, w0 + row])
    scale : float
}

// batched causal attention over the decode mirrors (the template's run): K/V read at kbase/vbase,
// out = plain rows (no gating). Reduce order differs from the row kernel so the parity bar there
// is IDS; vs the shipped da_attn_b it is bit-exact
[vk_dispatch(name = "da_attn_b_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnB : AtAttnTileT {
    typedef KT = float
    @ssbo @binding = 3 outp : array<float>   // attention out rows
    @push_constant pa : DaAttnBArgs
}

// f16-mirror twin — convert-on-load, staging/compute stay f32
[vk_dispatch(name = "da_attn_b_f16_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBF16 : AtAttnTileT {
    typedef KT = float16
    @ssbo @binding = 3 outp : array<float>   // attention out rows
    @push_constant pa : DaAttnBArgs
}

// hs=128 flash-attention twin of DaAttnB (coopmat devices): QK^T on f16 MMAs (hoisted q frags),
// shuffle-reduced online softmax, register-tiled PV over the f16 V tile. Same args contract
// (hs unread — baked 128); parity bar vs DaAttnB is IDS, vs the shipped kernel bit-exact.
// KT is the K/V plane element type — the f16 arm's float()-then-pack roundtrip is lossless
[ |> template_struct_instance]
class template DaAttnBH128T {
    @ssbo @binding = 0 qpanel : array<float>   // roped q rows
    @ssbo @binding = 1 kpanel : array<KT>      // the K mirror plane (kbase offsets)
    @ssbo @binding = 2 vpanel : array<KT>      // the V mirror plane (vbase offsets)
    @ssbo @binding = 3 outp : array<float>     // attention out rows
    @push_constant pa : DaAttnBArgs
    @workgroup fbq : uint[2048]    // the 32-row q tile, f16 pairs
    @workgroup fbkv : uint[2048]   // one 32-row K/V stage, f16 pairs
    @workgroup fbp : float[1024]   // the 32x32 score/probability tile
    @workgroup fbm : float[32]     // per-row online-softmax state
    @workgroup fbl : float[32]
    @workgroup fba : float[32]

    [spirv_kernel(local_size_x = 128)]
    def run {   // nolint:STYLE037,STYLE038 — the verbatim flash pass, parity-locked to the shipped kernel
        let qd = pa.nh * 128u
        let qtiles = (pa.npos + 31u) / 32u
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * 32u
        let tid = gl_LocalInvocationID.x
        let warp = tid / 32u
        let kvh = h / pa.kv_mul
        var i = tid
        while (i < 2048u) {   // stage the 32-row q tile once, f32 -> f16 pairs
            let qr = q0 + i / 64u
            let jp = (i % 64u) * 2u
            if (qr < pa.npos) {
                let src = qr * qd + h * 128u + jp
                fbq[i] = packHalf2x16(float2(qpanel[src], qpanel[src + 1u]))
            } else {
                fbq[i] = 0u
            }
            i += 128u
        }
        if (tid < 32u) {
            fbm[tid] = -3.0e38
            fbl[tid] = 0.0
        }
        var o : float[32]   // 2 rows x 16 cols per thread
        for (z in range(32)) {
            o[z] = 0.0
        }
        let orow0 = (tid / 8u) * 2u
        let oe = (tid % 8u) * 16u
        let srow = tid / 4u
        let cb = (tid % 4u) * 8u
        let lane4 = tid % 4u
        barrier()
        // my S block's q fragments — invariant across the whole K loop
        let br = warp / 2u
        let bc = warp % 2u
        var qa0 : coopmatA_f16_16x16
        var qa1 : coopmatA_f16_16x16
        var qa2 : coopmatA_f16_16x16
        var qa3 : coopmatA_f16_16x16
        var qa4 : coopmatA_f16_16x16
        var qa5 : coopmatA_f16_16x16
        var qa6 : coopmatA_f16_16x16
        var qa7 : coopmatA_f16_16x16
        coopmatLoad(qa0, fbq, int(br * 16u) * 64, 64, 0)
        coopmatLoad(qa1, fbq, int(br * 16u) * 64 + 8, 64, 0)
        coopmatLoad(qa2, fbq, int(br * 16u) * 64 + 16, 64, 0)
        coopmatLoad(qa3, fbq, int(br * 16u) * 64 + 24, 64, 0)
        coopmatLoad(qa4, fbq, int(br * 16u) * 64 + 32, 64, 0)
        coopmatLoad(qa5, fbq, int(br * 16u) * 64 + 40, 64, 0)
        coopmatLoad(qa6, fbq, int(br * 16u) * 64 + 48, 64, 0)
        coopmatLoad(qa7, fbq, int(br * 16u) * 64 + 56, 64, 0)
        let kend = pa.w0 + min(q0 + 32u, pa.npos)
        var k0 = 0u
        while (k0 < kend) {
            i = tid
            while (i < 2048u) {   // K tile, f16 pairs
                let kr = k0 + i / 64u
                let jp = (i % 64u) * 2u
                if (kr < kend) {
                    let src = pa.kbase + kr * pa.kvd + kvh * 128u + jp
                    fbkv[i] = packHalf2x16(float2(float(kpanel[src]), float(kpanel[src + 1u])))   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
                } else {
                    fbkv[i] = 0u
                }
                i += 128u
            }
            barrier()
            var acc : coopmatAcc_f32_16x16
            var kb : coopmatB_f16_16x16
            coopmatLoad(kb, fbkv, int(bc * 16u) * 64, 64, 1)
            acc = coopmatMulAdd(qa0, kb, acc)
            coopmatLoad(kb, fbkv, int(bc * 16u) * 64 + 8, 64, 1)
            acc = coopmatMulAdd(qa1, kb, acc)
            coopmatLoad(kb, fbkv, int(bc * 16u) * 64 + 16, 64, 1)
            acc = coopmatMulAdd(qa2, kb, acc)
            coopmatLoad(kb, fbkv, int(bc * 16u) * 64 + 24, 64, 1)
            acc = coopmatMulAdd(qa3, kb, acc)
            coopmatLoad(kb, fbkv, int(bc * 16u) * 64 + 32, 64, 1)
            acc = coopmatMulAdd(qa4, kb, acc)
            coopmatLoad(kb, fbkv, int(bc * 16u) * 64 + 40, 64, 1)
            acc = coopmatMulAdd(qa5, kb, acc)
            coopmatLoad(kb, fbkv, int(bc * 16u) * 64 + 48, 64, 1)
            acc = coopmatMulAdd(qa6, kb, acc)
            coopmatLoad(kb, fbkv, int(bc * 16u) * 64 + 56, 64, 1)
            acc = coopmatMulAdd(qa7, kb, acc)
            coopmatStore(acc, fbp, int(br * 16u) * 32 + int(bc * 16u), 32, 0)
            barrier()
            // online softmax: 4 threads/row (8 cols each), shuffle-reduced within the aligned quad
            let gq = pa.w0 + q0 + srow
            let m_old = fbm[srow]
            var mloc = -3.0e38
            for [unroll] (j in range(8)) {
                let idx = srow * 32u + cb + uint(j)
                let gk = k0 + cb + uint(j)
                let s = (gk <= gq && q0 + srow < pa.npos && gk < kend) ? fbp[idx] * pa.scale : -3.0e38
                fbp[idx] = s
                mloc = max(mloc, s)
            }
            mloc = max(mloc, subgroupShuffleXor(mloc, 1u))
            mloc = max(mloc, subgroupShuffleXor(mloc, 2u))
            let mt = max(m_old, mloc)
            var lloc = 0.0
            for [unroll] (j in range(8)) {
                let idx = srow * 32u + cb + uint(j)
                let e = exp(fbp[idx] - mt)
                fbp[idx] = e
                lloc += e
            }
            lloc += subgroupShuffleXor(lloc, 1u)
            lloc += subgroupShuffleXor(lloc, 2u)
            if (lane4 == 0u) {
                let alpha = exp(m_old - mt)
                fbl[srow] = fbl[srow] * alpha + lloc
                fbm[srow] = mt
                fba[srow] = alpha
            }
            barrier()
            i = tid
            while (i < 2048u) {   // V tile reuses the K staging
                let kr = k0 + i / 64u
                let jp = (i % 64u) * 2u
                if (kr < kend) {
                    let src = pa.vbase + kr * pa.kvd + kvh * 128u + jp
                    fbkv[i] = packHalf2x16(float2(float(vpanel[src]), float(vpanel[src + 1u])))   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
                } else {
                    fbkv[i] = 0u
                }
                i += 128u
            }
            barrier()
            let av0 = fba[orow0]
            let av1 = fba[orow0 + 1u]
            for (z in range(16)) {
                o[z] *= av0
                o[16 + z] *= av1
            }
            var c2 = 0u
            while (c2 < 32u) {
                let pw0 = fbp[orow0 * 32u + c2]
                let pw1 = fbp[(orow0 + 1u) * 32u + c2]
                let vb = c2 * 64u + oe / 2u
                for [unroll] (zp in range(8)) {
                    let pair = unpackHalf2x16(fbkv[vb + uint(zp)])
                    o[zp * 2] += pw0 * pair.x
                    o[zp * 2 + 1] += pw0 * pair.y
                    o[16 + zp * 2] += pw1 * pair.x
                    o[16 + zp * 2 + 1] += pw1 * pair.y
                }
                c2++
            }
            barrier()
            k0 += 32u
        }
        if (q0 + orow0 < pa.npos) {
            let inv0 = 1.0 / fbl[orow0]
            let ob0 = (q0 + orow0) * qd + h * 128u + oe
            for (z in range(16)) {
                outp[ob0 + uint(z)] = o[z] * inv0
            }
        }
        if (q0 + orow0 + 1u < pa.npos) {
            let inv1 = 1.0 / fbl[orow0 + 1u]
            let ob1 = (q0 + orow0 + 1u) * qd + h * 128u + oe
            for (z in range(16)) {
                outp[ob1 + uint(z)] = o[16 + z] * inv1
            }
        }
    }
}

[vk_dispatch(name = "da_attn_b_h128_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBH128 : DaAttnBH128T {
    typedef KT = float
}

// f16-mirror twin — the tiles were f16 all along, only the plane loads change
[vk_dispatch(name = "da_attn_b_h128_f16_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBH128F16 : DaAttnBH128T {
    typedef KT = float16
}

// ===== decode attention =====

struct DaAttnArgs {
    kvd : uint      // kv row width
    hs : uint       // head size
    kv_mul : uint   // GQA group (query heads per kv head)
    kbase : uint    // layer base into the shared K mirror (0 for the standalone seam)
    vbase : uint
    scale : float   // the KV count to attend rides tok[0].cnt
}

// decode attention (one query row): one wg per head, GQA, KT-typed K/V mirror. Online softmax
// over 256-position chunks (running max/denom, alpha-rescaled V accumulators) — any cnt works.
[ |> template_struct_instance]
class template DaAttnT {
    @ssbo @binding = 0 q : array<float>      // roped query rows, one per head
    @ssbo @binding = 1 kmir : array<KT>      // the K mirror plane
    @ssbo @binding = 2 vmir : array<KT>      // the V mirror plane
    @ssbo @binding = 3 y : array<float>      // attention out rows
    @ssbo @binding = 4 tok : array<TokMeta>  // the shared per-token block (element 0)
    @push_constant pa : DaAttnArgs
    @workgroup qrow : float[256]   // this head's query row (hs <= 256)
    @workgroup sc : float[256]     // chunk scores, then the cross-group V partial reduce
    @workgroup part : float[64]    // per-subgroup partials
    @workgroup ml : float[2]       // [0] = running max, [1] = running softmax denom

    [spirv_kernel(local_size_x = 256)]
    def run {   // nolint:STYLE038 — the verbatim online-softmax pass, parity-locked to the shipped kernel
        let hs = pa.hs
        let cnt = tok[0u].cnt
        let h = gl_WorkGroupID.x
        let kvh = h / pa.kv_mul
        let tid = gl_LocalInvocationID.x
        let npart = 256u / hs     // V-phase partial groups; thread tid = group g, output dim c
        let g = tid / hs
        let c = tid % hs
        var i = tid
        while (i < hs) {
            qrow[i] = q[h * hs + i]
            i += 256u
        }
        if (tid == 0u) {
            ml[1u] = 0.0
        }
        barrier()
        var acc = 0.0             // this thread's V partial (group g's slice of dim c)
        var mrun = -3.0e38        // running max — uniform, tracked in lockstep by every thread
        var c0 = 0u
        while (c0 < cnt) {
            let clen = min(256u, cnt - c0)
            // chunk scores: thread tid owns position c0 + tid
            var s = -3.0e38
            if (tid < clen) {
                var sv = 0.0
                var d = 0u
                while (d < hs) {
                    sv += qrow[d] * float(kmir[pa.kbase + (c0 + tid) * pa.kvd + kvh * hs + d])   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
                    d++
                }
                s = sv * pa.scale
            }
            let tmax = subgroupMax(s)
            if (gl_SubgroupInvocationID == 0u) {
                part[gl_SubgroupID] = tmax
            }
            barrier()
            if (tid == 0u) {
                var m = mrun
                var sgi = 0u
                while (sgi < gl_NumSubgroups) {
                    m = max(m, part[sgi])
                    sgi++
                }
                ml[0u] = m
            }
            barrier()
            let mnew = ml[0u]
            let alpha = exp(mrun - mnew)
            mrun = mnew
            // exp + chunk sum; denom rescale rides the same alpha
            let e = tid < clen ? exp(s - mnew) : 0.0
            sc[tid] = e
            let tsum = subgroupAdd(e)
            if (gl_SubgroupInvocationID == 0u) {
                part[gl_SubgroupID] = tsum
            }
            barrier()
            if (tid == 0u) {
                var lsum = 0.0
                var sgi = 0u
                while (sgi < gl_NumSubgroups) {
                    lsum += part[sgi]
                    sgi++
                }
                ml[1u] = ml[1u] * alpha + lsum
            }
            // V accumulate: group g strides the chunk, rescaled by this chunk's alpha
            acc *= alpha
            if (tid < npart * hs) {
                var jj = g
                while (jj < clen) {
                    acc += sc[jj] * float(vmir[pa.vbase + (c0 + jj) * pa.kvd + kvh * hs + c])   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
                    jj += npart
                }
            }
            barrier()   // sc/part settle before the next chunk overwrites them
            c0 += 256u
        }
        // cross-group reduce (sc reused as the partial slab: slot tid == g * hs + c)
        sc[tid] = acc
        barrier()
        if (tid < hs) {
            let linv = 1.0 / ml[1u]
            var tot = 0.0
            var gg = 0u
            while (gg < npart) {
                tot += sc[gg * hs + tid]
                gg++
            }
            y[h * hs + tid] = tot * linv
        }
    }
}

[vk_dispatch(name = "da_attn_cls", grid = "nheads", params = "nheads : int64")]
class DaAttn : DaAttnT {
    typedef KT = float
}

// f16-mirror twin — convert-on-load, compute stays f32
[vk_dispatch(name = "da_attn_f16_cls", grid = "nheads", params = "nheads : int64")]
class DaAttnF16 : DaAttnT {
    typedef KT = float16
}

// ===== the Q8_0 batch GEMM tile (the sdot4 prefill workhorse) =====

struct BatchArgs {
    n : uint         // reduction dim
    d : uint         // output cols
    map_off : uint   // the per-wg map's word offset in sched (region records sit at 0)
    ksplit : uint    // cm2 split-k: 256-aligned k chunk per partial plane; 0 = no split
}

// 32x32 tiles through shared so weight rows stream once per 32 rows; K advances 8 blocks per
// barrier pair. sched = 4-word region records [wblk0, row0, cnt, wg0] then the wg -> region map.
[vk_dispatch(name = "q8_batch_cls", grid = "wgs", params = "wgs : int64")]
class Q8Batch {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 2 sched : array<uint>     // region records + per-wg map (bulk schedule data)
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : BatchArgs
    @workgroup xw4 : uint4[544]   // x tile: 32 rows x 16 uint4 chunks, stride 17 (bank-spread)
    @workgroup ww4 : uint4[544]   // w tile: 32 cols x 16 chunks, stride 17
    @workgroup xsw : float[256]   // x scales per (block, row)
    @workgroup wsw : float[256]   // w scales per (block, col)

    // this workgroup's region record: x = weight base, y = activation row base, z = row count,
    // w = the wg's tile index rebased to the region
    def region_rec : uint4 {
        let rid = sched[pa.map_off + gl_WorkGroupID.x]
        let rb = rid * 4u
        return uint4(sched[rb], sched[rb + 1u], sched[rb + 2u], gl_WorkGroupID.x - sched[rb + 3u])
    }

    [spirv_kernel(local_size_x = 256, name = "q8_batch_cls_spv")]
    def run {   // nolint:STYLE038 — the verbatim tile pass (stage/reduce/store), parity-locked to the shipped kernel
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nbb = pa.n / 32u
        let nsteps = (nbb + 7u) / 8u   // q8 K need not be superblock-aligned
        let tid = gl_LocalInvocationID.x
        let word = tid % 8u
        let grp = tid / 8u             // 0..31: the x row AND w col this thread stages
        let xrow = xt * 32u + grp
        let wcol = wt * 32u + grp
        let slane = tid % 32u
        let sblk = tid / 32u
        var f0 = 0.0
        var f1 = 0.0
        var f2 = 0.0
        var f3 = 0.0
        var s = 0u
        while (s < nsteps) {
            let b0 = s * 8u
            for (ki in range(2)) {     // this thread stages v4 chunks word and word+8 of its row/col
                let k = word + uint(ki) * 8u
                let bu = k / 2u
                let h = (k % 2u) * 4u
                var xv = uint4(0u)
                if (xrow < cnt && b0 + bu < nbb) {
                    let xb4 = ((row0 + xrow) * nbb + b0 + bu) * 8u + h
                    xv = uint4(xqw[xb4], xqw[xb4 + 1u], xqw[xb4 + 2u], xqw[xb4 + 3u])
                }
                xw4[grp * 17u + k] = xv
                var wv = uint4(0u)
                if (wcol < pa.d && b0 + bu < nbb) {
                    let wb4 = (wblk0 + wcol * nbb + b0 + bu) * 8u + h
                    wv = uint4(wq[wb4], wq[wb4 + 1u], wq[wb4 + 2u], wq[wb4 + 3u])
                }
                ww4[grp * 17u + k] = wv
            }
            var xs = 0.0
            if (xt * 32u + slane < cnt && b0 + sblk < nbb) {
                xs = axs[(row0 + xt * 32u + slane) * nbb + b0 + sblk]
            }
            xsw[sblk * 32u + slane] = xs
            var ws = 0.0
            if (wt * 32u + slane < pa.d && b0 + sblk < nbb) {
                ws = float(wsh[wblk0 + (wt * 32u + slane) * nbb + b0 + sblk])
            }
            wsw[sblk * 32u + slane] = ws
            barrier()
            for (bb in range(8)) {     // zero-filled tail blocks contribute 0 (ws = xs = 0)
                let bu = uint(bb)
                var i0 = 0
                var i1 = 0
                var i2 = 0
                var i3 = 0
                for (kk in range(2)) {
                    let k = bu * 2u + uint(kk)
                    let xw = xw4[grp * 17u + k]
                    let wv0 = ww4[word * 17u + k]
                    let wv1 = ww4[(word + 8u) * 17u + k]
                    let wv2 = ww4[(word + 16u) * 17u + k]
                    let wv3 = ww4[(word + 24u) * 17u + k]
                    i0 += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
                    i1 += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
                    i2 += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
                    i3 += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
                }
                let xscl = xsw[bu * 32u + grp]
                f0 += float(i0) * wsw[bu * 32u + word] * xscl
                f1 += float(i1) * wsw[bu * 32u + word + 8u] * xscl
                f2 += float(i2) * wsw[bu * 32u + word + 16u] * xscl
                f3 += float(i3) * wsw[bu * 32u + word + 24u] * xscl
            }
            barrier()
            s++
        }
        if (xrow < cnt) {
            let yb = (row0 + xrow) * pa.d + wt * 32u + word
            if (wt * 32u + word < pa.d) {
                y[yb] = f0
            }
            if (wt * 32u + word + 8u < pa.d) {
                y[yb + 8u] = f1
            }
            if (wt * 32u + word + 16u < pa.d) {
                y[yb + 16u] = f2
            }
            if (wt * 32u + word + 24u < pa.d) {
                y[yb + 24u] = f3
            }
        }
    }
}

// ===== the K-quant batch GEMM tile family: one shell, per-format stage/fma overrides =====
// Q8Batch's 32x32 tile shape on Q8_K activations (256-weight superblocks); only the w-nibble
// compose, the scale stage, and the per-block FMA (float order contractual per format) differ —
// each is an override, and the devirtualizer inlines the family back to four standalone kernels.

// deposit 4 packed high-bits (one per nibble lane of hb) at bit 4 of each byte lane (Q5_K —
// the batch and gemv k5 variants share it)
def private k5_dep(hb : uint) : uint {
    return ((hb & 1u) << 4u) | ((hb & 2u) << 11u) | ((hb & 4u) << 18u) | ((hb & 8u) << 25u)
}

// IQ4_XS codebook decode of one word of 4 nibble lanes (bits 7..4 of each byte clear) into 4
// SIGNED int8 lanes (kvalues_iq4nl) — the sdot4 operand form; batch and gemv share it.
// The codebook is four packed words picked by a dynamic vector index and byte-extracted: pure
// ALU (a fixed_array local was a Function-storage table, 3.4x slower on the decode GEMV - `benchmarks/lcpp_bench.das` tg128, 5060 Ti)
// iq3s_grid, one word per call - the staging loops copy it into workgroup memory once
// per kernel (a few indexed reads per thread), so the local-array storage class (QUIRK 20's
// slow path) never sits on a hot path; a shader function cannot return a fixed array.
def private iq3s_grid_word(i : int) : uint {
    let tbl = fixed_array<uint>(
        0x01010101u, 0x01010103u, 0x01010105u, 0x0101010bu, 0x0101010fu, 0x01010301u, 0x01010303u, 0x01010305u,
        0x01010309u, 0x0101030du, 0x01010501u, 0x01010503u, 0x0101050bu, 0x01010707u, 0x01010901u, 0x01010905u,
        0x0101090bu, 0x0101090fu, 0x01010b03u, 0x01010b07u, 0x01010d01u, 0x01010d05u, 0x01010f03u, 0x01010f09u,
        0x01010f0fu, 0x01030101u, 0x01030103u, 0x01030105u, 0x01030109u, 0x01030301u, 0x01030303u, 0x0103030bu,
        0x01030501u, 0x01030507u, 0x0103050fu, 0x01030703u, 0x0103070bu, 0x01030909u, 0x01030d03u, 0x01030d0bu,
        0x01030f05u, 0x01050101u, 0x01050103u, 0x0105010bu, 0x0105010fu, 0x01050301u, 0x01050307u, 0x0105030du,
        0x01050503u, 0x0105050bu, 0x01050701u, 0x01050709u, 0x01050905u, 0x0105090bu, 0x0105090fu, 0x01050b03u,
        0x01050b07u, 0x01050f01u, 0x01050f07u, 0x01070107u, 0x01070303u, 0x0107030bu, 0x01070501u, 0x01070505u,
        0x01070703u, 0x01070707u, 0x0107070du, 0x01070909u, 0x01070b01u, 0x01070b05u, 0x01070d0fu, 0x01070f03u,
        0x01070f0bu, 0x01090101u, 0x01090307u, 0x0109030fu, 0x01090503u, 0x01090509u, 0x01090705u, 0x01090901u,
        0x01090907u, 0x01090b03u, 0x01090f01u, 0x010b0105u, 0x010b0109u, 0x010b0501u, 0x010b0505u, 0x010b050du,
        0x010b0707u, 0x010b0903u, 0x010b090bu, 0x010b090fu, 0x010b0d0du, 0x010b0f07u, 0x010d010du, 0x010d0303u,
        0x010d0307u, 0x010d0703u, 0x010d0b05u, 0x010d0f03u, 0x010f0101u, 0x010f0105u, 0x010f0109u, 0x010f0501u,
        0x010f0505u, 0x010f050du, 0x010f0707u, 0x010f0b01u, 0x010f0b09u, 0x03010101u, 0x03010103u, 0x03010105u,
        0x03010109u, 0x03010301u, 0x03010303u, 0x03010307u, 0x0301030bu, 0x0301030fu, 0x03010501u, 0x03010505u,
        0x03010703u, 0x03010709u, 0x0301070du, 0x03010b09u, 0x03010b0du, 0x03010d03u, 0x03010f05u, 0x03030101u,
        0x03030103u, 0x03030107u, 0x0303010du, 0x03030301u, 0x03030309u, 0x03030503u, 0x03030701u, 0x03030707u,
        0x03030903u, 0x03030b01u, 0x03030b05u, 0x03030f01u, 0x03030f0du, 0x03050101u, 0x03050305u, 0x0305030bu,
        0x0305030fu, 0x03050501u, 0x03050509u, 0x03050705u, 0x03050901u, 0x03050907u, 0x03050b0bu, 0x03050d01u,
        0x03050f05u, 0x03070103u, 0x03070109u, 0x0307010fu, 0x03070301u, 0x03070307u, 0x03070503u, 0x0307050fu,
        0x03070701u, 0x03070709u, 0x03070903u, 0x03070d05u, 0x03070f01u, 0x03090107u, 0x0309010bu, 0x03090305u,
        0x03090309u, 0x03090703u, 0x03090707u, 0x03090905u, 0x0309090du, 0x03090b01u, 0x03090b09u, 0x030b0103u,
        0x030b0301u, 0x030b0307u, 0x030b0503u, 0x030b0701u, 0x030b0705u, 0x030b0b03u, 0x030d0501u, 0x030d0509u,
        0x030d050fu, 0x030d0909u, 0x030d090du, 0x030f0103u, 0x030f0107u, 0x030f0301u, 0x030f0305u, 0x030f0503u,
        0x030f070bu, 0x030f0903u, 0x030f0d05u, 0x030f0f01u, 0x05010101u, 0x05010103u, 0x05010107u, 0x0501010bu,
        0x0501010fu, 0x05010301u, 0x05010305u, 0x05010309u, 0x0501030du, 0x05010503u, 0x05010507u, 0x0501050fu,
        0x05010701u, 0x05010705u, 0x05010903u, 0x05010907u, 0x0501090bu, 0x05010b01u, 0x05010b05u, 0x05010d0fu,
        0x05010f01u, 0x05010f07u, 0x05010f0bu, 0x05030101u, 0x05030105u, 0x05030301u, 0x05030307u, 0x0503030fu,
        0x05030505u, 0x0503050bu, 0x05030703u, 0x05030709u, 0x05030905u, 0x05030b03u, 0x05050103u, 0x05050109u,
        0x0505010fu, 0x05050503u, 0x05050507u, 0x05050701u, 0x0505070fu, 0x05050903u, 0x05050b07u, 0x05050b0fu,
        0x05050f03u, 0x05050f09u, 0x05070101u, 0x05070105u, 0x0507010bu, 0x05070303u, 0x05070505u, 0x05070509u,
        0x05070703u, 0x05070707u, 0x05070905u, 0x05070b01u, 0x05070d0du, 0x05090103u, 0x0509010fu, 0x05090501u,
        0x05090507u, 0x05090705u, 0x0509070bu, 0x05090903u, 0x05090f05u, 0x05090f0bu, 0x050b0109u, 0x050b0303u,
        0x050b0505u, 0x050b070fu, 0x050b0901u, 0x050b0b07u, 0x050b0f01u, 0x050d0101u, 0x050d0105u, 0x050d010fu,
        0x050d0503u, 0x050d0b0bu, 0x050d0d03u, 0x050f010bu, 0x050f0303u, 0x050f050du, 0x050f0701u, 0x050f0907u,
        0x050f0b01u, 0x07010105u, 0x07010303u, 0x07010307u, 0x0701030bu, 0x0701030fu, 0x07010505u, 0x07010703u,
        0x07010707u, 0x0701070bu, 0x07010905u, 0x07010909u, 0x0701090fu, 0x07010b03u, 0x07010d07u, 0x07010f03u,
        0x07030103u, 0x07030107u, 0x0703010bu, 0x07030309u, 0x07030503u, 0x07030507u, 0x07030901u, 0x07030d01u,
        0x07030f05u, 0x07030f0du, 0x07050101u, 0x07050305u, 0x07050501u, 0x07050705u, 0x07050709u, 0x07050b01u,
        0x07070103u, 0x07070301u, 0x07070309u, 0x07070503u, 0x07070507u, 0x0707050fu, 0x07070701u, 0x07070903u,
        0x07070907u, 0x0707090fu, 0x07070b0bu, 0x07070f07u, 0x07090107u, 0x07090303u, 0x0709030du, 0x07090505u,
        0x07090703u, 0x07090b05u, 0x07090d01u, 0x07090d09u, 0x070b0103u, 0x070b0301u, 0x070b0305u, 0x070b050bu,
        0x070b0705u, 0x070b0909u, 0x070b0b0du, 0x070b0f07u, 0x070d030du, 0x070d0903u, 0x070f0103u, 0x070f0107u,
        0x070f0501u, 0x070f0505u, 0x070f070bu, 0x09010101u, 0x09010109u, 0x09010305u, 0x09010501u, 0x09010509u,
        0x0901050fu, 0x09010705u, 0x09010903u, 0x09010b01u, 0x09010f01u, 0x09030105u, 0x0903010fu, 0x09030303u,
        0x09030307u, 0x09030505u, 0x09030701u, 0x0903070bu, 0x09030907u, 0x09030b03u, 0x09030b0bu, 0x09050103u,
        0x09050107u, 0x09050301u, 0x0905030bu, 0x09050503u, 0x09050707u, 0x09050901u, 0x09050b0fu, 0x09050d05u,
        0x09050f01u, 0x09070109u, 0x09070303u, 0x09070307u, 0x09070501u, 0x09070505u, 0x09070703u, 0x0907070bu,
        0x09090101u, 0x09090105u, 0x09090509u, 0x0909070fu, 0x09090901u, 0x09090f03u, 0x090b010bu, 0x090b010fu,
        0x090b0503u, 0x090b0d05u, 0x090d0307u, 0x090d0709u, 0x090d0d01u, 0x090f0301u, 0x090f030bu, 0x090f0701u,
        0x090f0907u, 0x090f0b03u, 0x0b010105u, 0x0b010301u, 0x0b010309u, 0x0b010505u, 0x0b010901u, 0x0b010909u,
        0x0b01090fu, 0x0b010b05u, 0x0b010d0du, 0x0b010f09u, 0x0b030103u, 0x0b030107u, 0x0b03010bu, 0x0b030305u,
        0x0b030503u, 0x0b030705u, 0x0b030f05u, 0x0b050101u, 0x0b050303u, 0x0b050507u, 0x0b050701u, 0x0b05070du,
        0x0b050b07u, 0x0b070105u, 0x0b07010fu, 0x0b070301u, 0x0b07050fu, 0x0b070909u, 0x0b070b03u, 0x0b070d0bu,
        0x0b070f07u, 0x0b090103u, 0x0b090109u, 0x0b090501u, 0x0b090705u, 0x0b09090du, 0x0b0b0305u, 0x0b0b050du,
        0x0b0b0b03u, 0x0b0b0b07u, 0x0b0d0905u, 0x0b0f0105u, 0x0b0f0109u, 0x0b0f0505u, 0x0d010303u, 0x0d010307u,
        0x0d01030bu, 0x0d010703u, 0x0d010707u, 0x0d010d01u, 0x0d030101u, 0x0d030501u, 0x0d03050fu, 0x0d030d09u,
        0x0d050305u, 0x0d050709u, 0x0d050905u, 0x0d050b0bu, 0x0d050d05u, 0x0d050f01u, 0x0d070101u, 0x0d070309u,
        0x0d070503u, 0x0d070901u, 0x0d09050bu, 0x0d090907u, 0x0d090d05u, 0x0d0b0101u, 0x0d0b0107u, 0x0d0b0709u,
        0x0d0b0d01u, 0x0d0d010bu, 0x0d0d0901u, 0x0d0f0303u, 0x0d0f0307u, 0x0f010101u, 0x0f010109u, 0x0f01010fu,
        0x0f010501u, 0x0f010505u, 0x0f01070du, 0x0f010901u, 0x0f010b09u, 0x0f010d05u, 0x0f030105u, 0x0f030303u,
        0x0f030509u, 0x0f030907u, 0x0f03090bu, 0x0f050103u, 0x0f050109u, 0x0f050301u, 0x0f05030du, 0x0f050503u,
        0x0f050701u, 0x0f050b03u, 0x0f070105u, 0x0f070705u, 0x0f07070bu, 0x0f070b07u, 0x0f090103u, 0x0f09010bu,
        0x0f090307u, 0x0f090501u, 0x0f090b01u, 0x0f0b0505u, 0x0f0b0905u, 0x0f0d0105u, 0x0f0d0703u, 0x0f0f0101u)
    return tbl[i]
}

// iq2xxs_grid as low/high word pairs - 256 u64 entries, one uint word per call (word
// 2i = magnitudes 0..3 of entry i, word 2i+1 = 4..7); staged into workgroup memory per kernel.
def private iq2xxs_grid_word(i : int) : uint {
    let tbl = fixed_array<uint>(
        0x08080808, 0x08080808, 0x0808082b, 0x08080808, 0x08081919, 0x08080808, 0x08082b08, 0x08080808,
        0x08082b2b, 0x08080808, 0x08190819, 0x08080808, 0x08191908, 0x08080808, 0x082b0808, 0x08080808,
        0x082b082b, 0x08080808, 0x082b2b08, 0x08080808, 0x082b2b2b, 0x08080808, 0x19080819, 0x08080808,
        0x19081908, 0x08080808, 0x19190808, 0x08080808, 0x19192b08, 0x08080808, 0x192b0819, 0x08080808,
        0x192b1908, 0x08080808, 0x2b080808, 0x08080808, 0x2b08082b, 0x08080808, 0x2b082b2b, 0x08080808,
        0x2b2b082b, 0x08080808, 0x08080819, 0x08080819, 0x08081908, 0x08080819, 0x08190808, 0x08080819,
        0x08191919, 0x08080819, 0x19080808, 0x08080819, 0x2b081908, 0x08080819, 0x2b192b08, 0x08080819,
        0x08080808, 0x0808082b, 0x0808082b, 0x0808082b, 0x082b082b, 0x0808082b, 0x2b08082b, 0x0808082b,
        0x08080819, 0x08081908, 0x08081908, 0x08081908, 0x08190808, 0x08081908, 0x082b0819, 0x08081908,
        0x082b1908, 0x08081908, 0x19080808, 0x08081908, 0x1908082b, 0x08081908, 0x19082b08, 0x08081908,
        0x192b0808, 0x08081908, 0x2b080819, 0x08081908, 0x2b081908, 0x08081908, 0x2b190808, 0x08081908,
        0x2b2b1908, 0x08081908, 0x08080808, 0x08081919, 0x0808082b, 0x08081919, 0x08082b08, 0x08081919,
        0x082b0808, 0x08081919, 0x1908192b, 0x08081919, 0x192b2b19, 0x08081919, 0x2b080808, 0x08081919,
        0x2b190819, 0x08081919, 0x08082b19, 0x0808192b, 0x08190808, 0x0808192b, 0x19080808, 0x0808192b,
        0x2b081908, 0x0808192b, 0x2b2b1908, 0x0808192b, 0x08080808, 0x08082b08, 0x08081919, 0x08082b08,
        0x08082b08, 0x08082b08, 0x08191908, 0x08082b08, 0x082b2b08, 0x08082b08, 0x19080819, 0x08082b08,
        0x19081908, 0x08082b08, 0x19190808, 0x08082b08, 0x1919082b, 0x08082b08, 0x2b082b08, 0x08082b08,
        0x08081908, 0x08082b19, 0x19080808, 0x08082b19, 0x0808082b, 0x08082b2b, 0x08191908, 0x08082b2b,
        0x08080819, 0x08190808, 0x08081908, 0x08190808, 0x08190808, 0x08190808, 0x082b0819, 0x08190808,
        0x19080808, 0x08190808, 0x192b0808, 0x08190808, 0x2b081908, 0x08190808, 0x2b190808, 0x08190808,
        0x2b191919, 0x08190808, 0x08080808, 0x08190819, 0x08082b08, 0x08190819, 0x082b0808, 0x08190819,
        0x19190808, 0x08190819, 0x19192b2b, 0x08190819, 0x2b080808, 0x08190819, 0x082b1908, 0x0819082b,
        0x19081919, 0x0819082b, 0x08080808, 0x08191908, 0x08082b08, 0x08191908, 0x082b0808, 0x08191908,
        0x082b1919, 0x08191908, 0x19082b19, 0x08191908, 0x2b080808, 0x08191908, 0x08192b08, 0x08191919,
        0x192b082b, 0x08191919, 0x08080808, 0x0819192b, 0x0819192b, 0x0819192b, 0x08080819, 0x08192b08,
        0x08081908, 0x08192b08, 0x08190808, 0x08192b08, 0x19080808, 0x08192b08, 0x2b080819, 0x08192b08,
        0x08080808, 0x08192b19, 0x08081919, 0x08192b19, 0x2b2b0808, 0x08192b19, 0x19190819, 0x08192b2b,
        0x08080808, 0x082b0808, 0x0808082b, 0x082b0808, 0x08082b2b, 0x082b0808, 0x19081908, 0x082b0808,
        0x192b0819, 0x082b0808, 0x2b080808, 0x082b0808, 0x2b08082b, 0x082b0808, 0x082b2b19, 0x082b0819,
        0x19082b08, 0x082b0819, 0x08080808, 0x082b082b, 0x0808082b, 0x082b082b, 0x08080819, 0x082b1908,
        0x08081908, 0x082b1908, 0x08190808, 0x082b1908, 0x19080808, 0x082b1908, 0x1919192b, 0x082b1908,
        0x08080808, 0x082b1919, 0x19080819, 0x082b1919, 0x192b1908, 0x082b1919, 0x2b190808, 0x082b192b,
        0x08082b08, 0x082b2b08, 0x082b0808, 0x082b2b08, 0x2b191908, 0x082b2b08, 0x19081908, 0x082b2b2b,
        0x08080819, 0x19080808, 0x08081908, 0x19080808, 0x08190808, 0x19080808, 0x08192b08, 0x19080808,
        0x082b0819, 0x19080808, 0x082b1908, 0x19080808, 0x19080808, 0x19080808, 0x19082b08, 0x19080808,
        0x1919192b, 0x19080808, 0x192b0808, 0x19080808, 0x2b080819, 0x19080808, 0x2b081908, 0x19080808,
        0x2b190808, 0x19080808, 0x08080808, 0x19080819, 0x082b0808, 0x19080819, 0x192b0819, 0x19080819,
        0x2b080808, 0x19080819, 0x2b081919, 0x19080819, 0x08080819, 0x1908082b, 0x08190808, 0x1908082b,
        0x19082b08, 0x1908082b, 0x1919192b, 0x1908082b, 0x192b2b08, 0x1908082b, 0x08080808, 0x19081908,
        0x08082b08, 0x19081908, 0x082b0808, 0x19081908, 0x2b080808, 0x19081908, 0x2b192b19, 0x19081908,
        0x0819082b, 0x19081919, 0x082b1908, 0x19081919, 0x08080808, 0x1908192b, 0x08080819, 0x19082b08,
        0x08081908, 0x19082b08, 0x08190808, 0x19082b08, 0x19080808, 0x19082b08, 0x19081919, 0x19082b08,
        0x08080808, 0x19082b19, 0x19192b08, 0x19082b19, 0x192b0819, 0x19082b19, 0x2b08082b, 0x19082b19,
        0x19081919, 0x19082b2b, 0x2b190808, 0x19082b2b, 0x08080808, 0x19190808, 0x08082b08, 0x19190808,
        0x08190819, 0x19190808, 0x08192b19, 0x19190808, 0x082b0808, 0x19190808, 0x2b080808, 0x19190808,
        0x2b082b08, 0x19190808, 0x08081908, 0x19190819, 0x1908082b, 0x19190819, 0x2b2b1908, 0x19190819,
        0x2b190819, 0x1919082b, 0x2b190808, 0x19191908, 0x2b19082b, 0x19191908, 0x08082b2b, 0x19191919,
        0x08080819, 0x1919192b, 0x19191908, 0x1919192b, 0x08080808, 0x19192b08, 0x08190819, 0x19192b08,
        0x08192b19, 0x19192b08, 0x192b1908, 0x19192b08, 0x19080808, 0x19192b19, 0x08082b08, 0x19192b2b,
        0x08081908, 0x192b0808, 0x08190808, 0x192b0808, 0x19080808, 0x192b0808, 0x192b2b08, 0x192b0808,
        0x08080808, 0x192b0819, 0x19191919, 0x192b0819, 0x08192b08, 0x192b082b, 0x192b0808, 0x192b082b,
        0x08080808, 0x192b1908, 0x08081919, 0x192b1908, 0x08190808, 0x192b1919, 0x0819082b, 0x192b1919,
        0x2b081908, 0x192b1919, 0x1908082b, 0x192b2b08, 0x08080808, 0x2b080808, 0x0808082b, 0x2b080808,
        0x08082b2b, 0x2b080808, 0x19080819, 0x2b080808, 0x2b08082b, 0x2b080808, 0x08081908, 0x2b080819,
        0x08192b08, 0x2b080819, 0x19080808, 0x2b080819, 0x08190819, 0x2b08082b, 0x08080819, 0x2b081908,
        0x08081908, 0x2b081908, 0x08190808, 0x2b081908, 0x08191919, 0x2b081908, 0x19080808, 0x2b081908,
        0x192b0808, 0x2b081908, 0x08080808, 0x2b081919, 0x1908192b, 0x2b081919, 0x2b191908, 0x2b081919,
        0x08082b19, 0x2b08192b, 0x19080808, 0x2b08192b, 0x192b0808, 0x2b08192b, 0x0808082b, 0x2b082b08,
        0x08081908, 0x2b082b19, 0x08190819, 0x2b082b2b, 0x08081908, 0x2b190808, 0x08190808, 0x2b190808,
        0x082b1908, 0x2b190808, 0x19080808, 0x2b190808, 0x2b2b0819, 0x2b190808, 0x0819192b, 0x2b190819,
        0x2b080808, 0x2b190819, 0x19081919, 0x2b19082b, 0x08080808, 0x2b191908, 0x082b082b, 0x2b191908,
        0x19081908, 0x2b191908, 0x19190819, 0x2b191919, 0x2b080819, 0x2b192b08, 0x082b0808, 0x2b192b19,
        0x0808082b, 0x2b2b0808, 0x19190808, 0x2b2b0808, 0x2b081919, 0x2b2b0808, 0x08082b19, 0x2b2b0819,
        0x08080808, 0x2b2b082b, 0x08192b08, 0x2b2b1908, 0x19190808, 0x2b2b2b08, 0x08081908, 0x2b2b2b19)
    return tbl[i]
}

// iq2xs_grid as low/high word pairs - 512 u64 entries, one uint word per call (word
// 2i = magnitudes 0..3 of entry i, word 2i+1 = 4..7); staged into workgroup memory per kernel.
def private iq2xs_grid_word(i : int) : uint {   // nolint:STYLE038 - a 1024-word data table, not splittable
    let tbl = fixed_array<uint>(
        0x08080808, 0x08080808, 0x0808082b, 0x08080808, 0x08081919, 0x08080808, 0x08082b08, 0x08080808,
        0x08082b2b, 0x08080808, 0x08190819, 0x08080808, 0x08191908, 0x08080808, 0x0819192b, 0x08080808,
        0x08192b19, 0x08080808, 0x082b0808, 0x08080808, 0x082b082b, 0x08080808, 0x082b1919, 0x08080808,
        0x082b2b08, 0x08080808, 0x19080819, 0x08080808, 0x19081908, 0x08080808, 0x1908192b, 0x08080808,
        0x19082b19, 0x08080808, 0x19190808, 0x08080808, 0x1919082b, 0x08080808, 0x19191919, 0x08080808,
        0x19192b08, 0x08080808, 0x192b0819, 0x08080808, 0x192b1908, 0x08080808, 0x2b080808, 0x08080808,
        0x2b08082b, 0x08080808, 0x2b081919, 0x08080808, 0x2b082b08, 0x08080808, 0x2b190819, 0x08080808,
        0x2b191908, 0x08080808, 0x2b192b19, 0x08080808, 0x2b2b0808, 0x08080808, 0x08080819, 0x08080819,
        0x08081908, 0x08080819, 0x0808192b, 0x08080819, 0x08082b19, 0x08080819, 0x08190808, 0x08080819,
        0x0819082b, 0x08080819, 0x08191919, 0x08080819, 0x08192b08, 0x08080819, 0x08192b2b, 0x08080819,
        0x082b0819, 0x08080819, 0x082b1908, 0x08080819, 0x19080808, 0x08080819, 0x1908082b, 0x08080819,
        0x19081919, 0x08080819, 0x19082b08, 0x08080819, 0x19190819, 0x08080819, 0x19191908, 0x08080819,
        0x192b0808, 0x08080819, 0x192b2b08, 0x08080819, 0x2b080819, 0x08080819, 0x2b081908, 0x08080819,
        0x2b190808, 0x08080819, 0x08080808, 0x0808082b, 0x0808082b, 0x0808082b, 0x08081919, 0x0808082b,
        0x08082b08, 0x0808082b, 0x08190819, 0x0808082b, 0x08191908, 0x0808082b, 0x082b0808, 0x0808082b,
        0x19080819, 0x0808082b, 0x19081908, 0x0808082b, 0x19190808, 0x0808082b, 0x19191919, 0x0808082b,
        0x2b080808, 0x0808082b, 0x2b082b2b, 0x0808082b, 0x08080819, 0x08081908, 0x08081908, 0x08081908,
        0x0808192b, 0x08081908, 0x08082b19, 0x08081908, 0x08190808, 0x08081908, 0x0819082b, 0x08081908,
        0x08191919, 0x08081908, 0x08192b08, 0x08081908, 0x082b0819, 0x08081908, 0x082b1908, 0x08081908,
        0x19080808, 0x08081908, 0x1908082b, 0x08081908, 0x19081919, 0x08081908, 0x19082b08, 0x08081908,
        0x19190819, 0x08081908, 0x19191908, 0x08081908, 0x1919192b, 0x08081908, 0x192b0808, 0x08081908,
        0x2b080819, 0x08081908, 0x2b081908, 0x08081908, 0x2b190808, 0x08081908, 0x08080808, 0x08081919,
        0x0808082b, 0x08081919, 0x08081919, 0x08081919, 0x08082b08, 0x08081919, 0x08190819, 0x08081919,
        0x08191908, 0x08081919, 0x082b0808, 0x08081919, 0x19080819, 0x08081919, 0x19081908, 0x08081919,
        0x19190808, 0x08081919, 0x192b0819, 0x08081919, 0x2b080808, 0x08081919, 0x08080819, 0x0808192b,
        0x08081908, 0x0808192b, 0x08190808, 0x0808192b, 0x082b192b, 0x0808192b, 0x19080808, 0x0808192b,
        0x1908082b, 0x0808192b, 0x2b081908, 0x0808192b, 0x08080808, 0x08082b08, 0x0808082b, 0x08082b08,
        0x08081919, 0x08082b08, 0x08082b08, 0x08082b08, 0x08082b2b, 0x08082b08, 0x08190819, 0x08082b08,
        0x08191908, 0x08082b08, 0x082b0808, 0x08082b08, 0x082b1919, 0x08082b08, 0x19080819, 0x08082b08,
        0x19081908, 0x08082b08, 0x19190808, 0x08082b08, 0x19192b08, 0x08082b08, 0x2b080808, 0x08082b08,
        0x2b2b0808, 0x08082b08, 0x2b2b2b2b, 0x08082b08, 0x08080819, 0x08082b19, 0x08081908, 0x08082b19,
        0x08190808, 0x08082b19, 0x19080808, 0x08082b19, 0x2b080819, 0x08082b19, 0x2b082b19, 0x08082b19,
        0x08080808, 0x08082b2b, 0x082b0808, 0x08082b2b, 0x082b2b08, 0x08082b2b, 0x2b19192b, 0x08082b2b,
        0x2b2b0808, 0x08082b2b, 0x08080819, 0x08190808, 0x08081908, 0x08190808, 0x0808192b, 0x08190808,
        0x08082b19, 0x08190808, 0x08190808, 0x08190808, 0x0819082b, 0x08190808, 0x08191919, 0x08190808,
        0x08192b08, 0x08190808, 0x082b0819, 0x08190808, 0x082b1908, 0x08190808, 0x19080808, 0x08190808,
        0x1908082b, 0x08190808, 0x19081919, 0x08190808, 0x19082b08, 0x08190808, 0x19190819, 0x08190808,
        0x19191908, 0x08190808, 0x192b0808, 0x08190808, 0x192b2b2b, 0x08190808, 0x2b080819, 0x08190808,
        0x2b081908, 0x08190808, 0x2b190808, 0x08190808, 0x08080808, 0x08190819, 0x0808082b, 0x08190819,
        0x08081919, 0x08190819, 0x08082b08, 0x08190819, 0x08190819, 0x08190819, 0x08191908, 0x08190819,
        0x082b0808, 0x08190819, 0x19080819, 0x08190819, 0x19081908, 0x08190819, 0x19190808, 0x08190819,
        0x2b080808, 0x08190819, 0x2b191908, 0x08190819, 0x2b19192b, 0x08190819, 0x08080819, 0x0819082b,
        0x08081908, 0x0819082b, 0x0808192b, 0x0819082b, 0x08190808, 0x0819082b, 0x19080808, 0x0819082b,
        0x192b0808, 0x0819082b, 0x08080808, 0x08191908, 0x0808082b, 0x08191908, 0x08081919, 0x08191908,
        0x08082b08, 0x08191908, 0x08190819, 0x08191908, 0x08191908, 0x08191908, 0x082b0808, 0x08191908,
        0x19080819, 0x08191908, 0x19081908, 0x08191908, 0x19082b19, 0x08191908, 0x19190808, 0x08191908,
        0x192b1908, 0x08191908, 0x2b080808, 0x08191908, 0x08080819, 0x08191919, 0x08081908, 0x08191919,
        0x08190808, 0x08191919, 0x19080808, 0x08191919, 0x08080808, 0x0819192b, 0x08191908, 0x0819192b,
        0x19082b19, 0x0819192b, 0x08080819, 0x08192b08, 0x08081908, 0x08192b08, 0x08190808, 0x08192b08,
        0x0819082b, 0x08192b08, 0x19080808, 0x08192b08, 0x19191908, 0x08192b08, 0x2b08192b, 0x08192b08,
        0x08080808, 0x08192b19, 0x08081919, 0x08192b19, 0x192b192b, 0x08192b19, 0x19190819, 0x08192b2b,
        0x2b2b2b19, 0x08192b2b, 0x08080808, 0x082b0808, 0x0808082b, 0x082b0808, 0x08081919, 0x082b0808,
        0x08082b08, 0x082b0808, 0x08082b2b, 0x082b0808, 0x08190819, 0x082b0808, 0x08191908, 0x082b0808,
        0x082b0808, 0x082b0808, 0x19080819, 0x082b0808, 0x19081908, 0x082b0808, 0x19190808, 0x082b0808,
        0x2b080808, 0x082b0808, 0x2b2b0808, 0x082b0808, 0x08080819, 0x082b0819, 0x08081908, 0x082b0819,
        0x08190808, 0x082b0819, 0x19080808, 0x082b0819, 0x19082b08, 0x082b0819, 0x192b1919, 0x082b0819,
        0x08080808, 0x082b082b, 0x082b082b, 0x082b082b, 0x2b080808, 0x082b082b, 0x2b2b2b08, 0x082b082b,
        0x08080819, 0x082b1908, 0x08081908, 0x082b1908, 0x08190808, 0x082b1908, 0x082b2b19, 0x082b1908,
        0x19080808, 0x082b1908, 0x08080808, 0x082b1919, 0x19080819, 0x082b1919, 0x1919082b, 0x082b1919,
        0x2b192b19, 0x082b1919, 0x08080819, 0x082b192b, 0x08192b2b, 0x082b192b, 0x2b2b192b, 0x082b192b,
        0x08080808, 0x082b2b08, 0x08082b08, 0x082b2b08, 0x08082b2b, 0x082b2b08, 0x082b0808, 0x082b2b08,
        0x19191919, 0x082b2b08, 0x2b082b08, 0x082b2b08, 0x2b2b082b, 0x082b2b08, 0x192b2b08, 0x082b2b19,
        0x2b190808, 0x082b2b19, 0x08082b08, 0x082b2b2b, 0x082b0808, 0x082b2b2b, 0x2b08082b, 0x082b2b2b,
        0x2b082b08, 0x082b2b2b, 0x2b082b2b, 0x082b2b2b, 0x08080819, 0x19080808, 0x08081908, 0x19080808,
        0x0808192b, 0x19080808, 0x08082b19, 0x19080808, 0x08190808, 0x19080808, 0x0819082b, 0x19080808,
        0x08191919, 0x19080808, 0x08192b08, 0x19080808, 0x082b0819, 0x19080808, 0x082b1908, 0x19080808,
        0x19080808, 0x19080808, 0x1908082b, 0x19080808, 0x19081919, 0x19080808, 0x19082b08, 0x19080808,
        0x19082b2b, 0x19080808, 0x19190819, 0x19080808, 0x19191908, 0x19080808, 0x192b0808, 0x19080808,
        0x192b1919, 0x19080808, 0x2b080819, 0x19080808, 0x2b081908, 0x19080808, 0x2b190808, 0x19080808,
        0x08080808, 0x19080819, 0x0808082b, 0x19080819, 0x08081919, 0x19080819, 0x08082b08, 0x19080819,
        0x08190819, 0x19080819, 0x08191908, 0x19080819, 0x082b0808, 0x19080819, 0x19080819, 0x19080819,
        0x19081908, 0x19080819, 0x19190808, 0x19080819, 0x2b080808, 0x19080819, 0x2b081919, 0x19080819,
        0x2b2b082b, 0x19080819, 0x08080819, 0x1908082b, 0x08081908, 0x1908082b, 0x08190808, 0x1908082b,
        0x0819082b, 0x1908082b, 0x082b2b19, 0x1908082b, 0x19080808, 0x1908082b, 0x08080808, 0x19081908,
        0x0808082b, 0x19081908, 0x08081919, 0x19081908, 0x08082b08, 0x19081908, 0x08190819, 0x19081908,
        0x08191908, 0x19081908, 0x08192b19, 0x19081908, 0x082b0808, 0x19081908, 0x19080819, 0x19081908,
        0x19081908, 0x19081908, 0x19190808, 0x19081908, 0x2b080808, 0x19081908, 0x2b191908, 0x19081908,
        0x08080819, 0x19081919, 0x08081908, 0x19081919, 0x08190808, 0x19081919, 0x082b1908, 0x19081919,
        0x19080808, 0x19081919, 0x2b192b2b, 0x19081919, 0x08080808, 0x1908192b, 0x08082b2b, 0x1908192b,
        0x19081908, 0x1908192b, 0x19190808, 0x1908192b, 0x08080819, 0x19082b08, 0x08081908, 0x19082b08,
        0x08190808, 0x19082b08, 0x19080808, 0x19082b08, 0x19081919, 0x19082b08, 0x19191908, 0x19082b08,
        0x192b082b, 0x19082b08, 0x08080808, 0x19082b19, 0x08190819, 0x19082b19, 0x19081908, 0x19082b19,
        0x19190808, 0x19082b19, 0x192b2b19, 0x19082b19, 0x08081908, 0x19082b2b, 0x08080808, 0x19190808,
        0x0808082b, 0x19190808, 0x08081919, 0x19190808, 0x08082b08, 0x19190808, 0x08190819, 0x19190808,
        0x08191908, 0x19190808, 0x082b0808, 0x19190808, 0x082b2b08, 0x19190808, 0x19080819, 0x19190808,
        0x19081908, 0x19190808, 0x19190808, 0x19190808, 0x2b080808, 0x19190808, 0x08080819, 0x19190819,
        0x08081908, 0x19190819, 0x08190808, 0x19190819, 0x08191919, 0x19190819, 0x19080808, 0x19190819,
        0x1908082b, 0x19190819, 0x08080808, 0x1919082b, 0x19081908, 0x1919082b, 0x2b2b2b2b, 0x1919082b,
        0x08080819, 0x19191908, 0x08081908, 0x19191908, 0x08190808, 0x19191908, 0x082b0819, 0x19191908,
        0x19080808, 0x19191908, 0x192b0808, 0x19191908, 0x2b080819, 0x19191908, 0x2b2b0819, 0x19191908,
        0x08080808, 0x19191919, 0x08082b08, 0x19191919, 0x2b080808, 0x19191919, 0x2b082b08, 0x19191919,
        0x082b0819, 0x1919192b, 0x192b2b08, 0x1919192b, 0x2b2b0819, 0x1919192b, 0x08080808, 0x19192b08,
        0x08191908, 0x19192b08, 0x19080819, 0x19192b08, 0x19190808, 0x19192b08, 0x2b192b19, 0x19192b08,
        0x08192b2b, 0x19192b19, 0x19080808, 0x19192b19, 0x1908082b, 0x19192b19, 0x2b081919, 0x19192b2b,
        0x08080819, 0x192b0808, 0x08081908, 0x192b0808, 0x08190808, 0x192b0808, 0x19080808, 0x192b0808,
        0x19191908, 0x192b0808, 0x192b082b, 0x192b0808, 0x2b08192b, 0x192b0808, 0x2b2b2b19, 0x192b0808,
        0x08080808, 0x192b0819, 0x082b1908, 0x192b082b, 0x19082b2b, 0x192b082b, 0x2b19082b, 0x192b082b,
        0x08080808, 0x192b1908, 0x0819192b, 0x192b1908, 0x08190808, 0x192b1919, 0x19080808, 0x192b1919,
        0x19081919, 0x192b1919, 0x2b2b1908, 0x192b1919, 0x08080819, 0x192b2b08, 0x192b2b2b, 0x192b2b08,
        0x082b1919, 0x192b2b19, 0x0808192b, 0x192b2b2b, 0x19191908, 0x192b2b2b, 0x192b082b, 0x192b2b2b,
        0x08080808, 0x2b080808, 0x0808082b, 0x2b080808, 0x08081919, 0x2b080808, 0x08082b08, 0x2b080808,
        0x08190819, 0x2b080808, 0x08191908, 0x2b080808, 0x082b0808, 0x2b080808, 0x082b2b2b, 0x2b080808,
        0x19080819, 0x2b080808, 0x19081908, 0x2b080808, 0x19190808, 0x2b080808, 0x2b080808, 0x2b080808,
        0x2b08082b, 0x2b080808, 0x2b2b2b08, 0x2b080808, 0x2b2b2b2b, 0x2b080808, 0x08080819, 0x2b080819,
        0x08081908, 0x2b080819, 0x0808192b, 0x2b080819, 0x08190808, 0x2b080819, 0x19080808, 0x2b080819,
        0x19190819, 0x2b080819, 0x19192b19, 0x2b080819, 0x08080808, 0x2b08082b, 0x082b0808, 0x2b08082b,
        0x2b080808, 0x2b08082b, 0x2b08082b, 0x2b08082b, 0x2b2b0808, 0x2b08082b, 0x2b2b2b08, 0x2b08082b,
        0x08080819, 0x2b081908, 0x08081908, 0x2b081908, 0x08190808, 0x2b081908, 0x0819082b, 0x2b081908,
        0x08191919, 0x2b081908, 0x19080808, 0x2b081908, 0x192b0808, 0x2b081908, 0x2b082b19, 0x2b081908,
        0x08080808, 0x2b081919, 0x19081908, 0x2b081919, 0x2b2b1919, 0x2b081919, 0x08192b08, 0x2b08192b,
        0x192b2b2b, 0x2b08192b, 0x08080808, 0x2b082b08, 0x08082b08, 0x2b082b08, 0x082b1919, 0x2b082b08,
        0x19192b2b, 0x2b082b08, 0x2b080808, 0x2b082b08, 0x2b08082b, 0x2b082b08, 0x2b2b2b08, 0x2b082b08,
        0x0808192b, 0x2b082b19, 0x082b082b, 0x2b082b2b, 0x2b080808, 0x2b082b2b, 0x2b082b08, 0x2b082b2b,
        0x2b19192b, 0x2b082b2b, 0x2b2b2b08, 0x2b082b2b, 0x08080819, 0x2b190808, 0x08081908, 0x2b190808,
        0x08190808, 0x2b190808, 0x19080808, 0x2b190808, 0x1919192b, 0x2b190808, 0x2b081908, 0x2b190808,
        0x08080808, 0x2b190819, 0x082b082b, 0x2b190819, 0x192b1908, 0x2b190819, 0x1919192b, 0x2b19082b,
        0x2b082b19, 0x2b19082b, 0x08080808, 0x2b191908, 0x08081919, 0x2b191908, 0x19081908, 0x2b191908,
        0x19190808, 0x2b191908, 0x19192b08, 0x2b191908, 0x082b2b19, 0x2b191919, 0x2b190808, 0x2b191919,
        0x2b19082b, 0x2b191919, 0x19080819, 0x2b19192b, 0x19190819, 0x2b192b08, 0x2b2b192b, 0x2b192b08,
        0x19082b19, 0x2b192b19, 0x08191919, 0x2b192b2b, 0x192b0808, 0x2b192b2b, 0x08080808, 0x2b2b0808,
        0x0808082b, 0x2b2b0808, 0x08082b08, 0x2b2b0808, 0x08082b2b, 0x2b2b0808, 0x082b0808, 0x2b2b0808,
        0x082b2b2b, 0x2b2b0808, 0x2b2b0808, 0x2b2b0808, 0x19190819, 0x2b2b0819, 0x19192b19, 0x2b2b0819,
        0x2b2b192b, 0x2b2b0819, 0x08080808, 0x2b2b082b, 0x0808082b, 0x2b2b082b, 0x08082b08, 0x2b2b082b,
        0x082b2b2b, 0x2b2b082b, 0x2b080808, 0x2b2b082b, 0x2b2b0808, 0x2b2b082b, 0x19080808, 0x2b2b1908,
        0x2b191919, 0x2b2b1908, 0x192b1919, 0x2b2b192b, 0x2b192b08, 0x2b2b192b, 0x08082b2b, 0x2b2b2b08,
        0x082b0808, 0x2b2b2b08, 0x082b082b, 0x2b2b2b08, 0x082b2b08, 0x2b2b2b08, 0x2b2b0808, 0x2b2b2b08,
        0x2b2b2b08, 0x2b2b2b08, 0x08081908, 0x2b2b2b19, 0x2b081908, 0x2b2b2b19, 0x2b08192b, 0x2b2b2b19,
        0x082b2b08, 0x2b2b2b2b, 0x082b2b2b, 0x2b2b2b2b, 0x2b190819, 0x2b2b2b2b, 0x2b2b2b2b, 0x2b2b2b2b)
    return tbl[i]
}

// iq2s_grid as low/high word pairs - 1024 u64 entries, one uint word per call (word 2i =
// magnitudes 0..3 of entry i, word 2i+1 = 4..7); the staging loops copy it into workgroup
// memory once per kernel, so the local-array storage class never sits on a hot path.
def private iq2s_grid_word(i : int) : uint {   // nolint:STYLE038 - a 2048-word data table, not splittable
    let tbl = fixed_array<uint>(
        0x08080808, 0x08080808, 0x0808082b, 0x08080808, 0x08081919, 0x08080808, 0x08082b08, 0x08080808,
        0x08082b2b, 0x08080808, 0x08190819, 0x08080808, 0x08191908, 0x08080808, 0x0819192b, 0x08080808,
        0x08192b19, 0x08080808, 0x082b0808, 0x08080808, 0x082b082b, 0x08080808, 0x082b1919, 0x08080808,
        0x082b2b08, 0x08080808, 0x19080819, 0x08080808, 0x19081908, 0x08080808, 0x1908192b, 0x08080808,
        0x19082b19, 0x08080808, 0x19190808, 0x08080808, 0x1919082b, 0x08080808, 0x19191919, 0x08080808,
        0x19192b08, 0x08080808, 0x192b0819, 0x08080808, 0x192b1908, 0x08080808, 0x192b192b, 0x08080808,
        0x192b2b19, 0x08080808, 0x2b080808, 0x08080808, 0x2b08082b, 0x08080808, 0x2b081919, 0x08080808,
        0x2b082b08, 0x08080808, 0x2b190819, 0x08080808, 0x2b191908, 0x08080808, 0x2b2b0808, 0x08080808,
        0x2b2b1919, 0x08080808, 0x2b2b2b2b, 0x08080808, 0x08080819, 0x08080819, 0x08081908, 0x08080819,
        0x0808192b, 0x08080819, 0x08082b19, 0x08080819, 0x08190808, 0x08080819, 0x0819082b, 0x08080819,
        0x08191919, 0x08080819, 0x08192b08, 0x08080819, 0x082b0819, 0x08080819, 0x082b1908, 0x08080819,
        0x19080808, 0x08080819, 0x1908082b, 0x08080819, 0x19081919, 0x08080819, 0x19082b08, 0x08080819,
        0x19190819, 0x08080819, 0x19191908, 0x08080819, 0x1919192b, 0x08080819, 0x19192b19, 0x08080819,
        0x192b0808, 0x08080819, 0x192b1919, 0x08080819, 0x192b2b08, 0x08080819, 0x2b080819, 0x08080819,
        0x2b081908, 0x08080819, 0x2b190808, 0x08080819, 0x2b19082b, 0x08080819, 0x2b191919, 0x08080819,
        0x2b2b0819, 0x08080819, 0x2b2b1908, 0x08080819, 0x08080808, 0x0808082b, 0x0808082b, 0x0808082b,
        0x08081919, 0x0808082b, 0x08082b08, 0x0808082b, 0x08190819, 0x0808082b, 0x08191908, 0x0808082b,
        0x082b0808, 0x0808082b, 0x082b2b2b, 0x0808082b, 0x19080819, 0x0808082b, 0x19081908, 0x0808082b,
        0x1908192b, 0x0808082b, 0x19082b19, 0x0808082b, 0x19190808, 0x0808082b, 0x19191919, 0x0808082b,
        0x2b080808, 0x0808082b, 0x2b081919, 0x0808082b, 0x2b082b2b, 0x0808082b, 0x2b191908, 0x0808082b,
        0x2b2b082b, 0x0808082b, 0x08080819, 0x08081908, 0x08081908, 0x08081908, 0x0808192b, 0x08081908,
        0x08082b19, 0x08081908, 0x08190808, 0x08081908, 0x0819082b, 0x08081908, 0x08191919, 0x08081908,
        0x08192b08, 0x08081908, 0x082b0819, 0x08081908, 0x082b1908, 0x08081908, 0x082b192b, 0x08081908,
        0x082b2b19, 0x08081908, 0x19080808, 0x08081908, 0x1908082b, 0x08081908, 0x19081919, 0x08081908,
        0x19082b08, 0x08081908, 0x19082b2b, 0x08081908, 0x19190819, 0x08081908, 0x19191908, 0x08081908,
        0x1919192b, 0x08081908, 0x19192b19, 0x08081908, 0x192b0808, 0x08081908, 0x192b082b, 0x08081908,
        0x192b1919, 0x08081908, 0x2b080819, 0x08081908, 0x2b081908, 0x08081908, 0x2b08192b, 0x08081908,
        0x2b082b19, 0x08081908, 0x2b190808, 0x08081908, 0x2b191919, 0x08081908, 0x2b192b08, 0x08081908,
        0x2b2b0819, 0x08081908, 0x2b2b1908, 0x08081908, 0x08080808, 0x08081919, 0x0808082b, 0x08081919,
        0x08081919, 0x08081919, 0x08082b08, 0x08081919, 0x08082b2b, 0x08081919, 0x08190819, 0x08081919,
        0x08191908, 0x08081919, 0x0819192b, 0x08081919, 0x08192b19, 0x08081919, 0x082b0808, 0x08081919,
        0x082b1919, 0x08081919, 0x082b2b08, 0x08081919, 0x19080819, 0x08081919, 0x19081908, 0x08081919,
        0x1908192b, 0x08081919, 0x19082b19, 0x08081919, 0x19190808, 0x08081919, 0x1919082b, 0x08081919,
        0x19191919, 0x08081919, 0x19192b08, 0x08081919, 0x192b0819, 0x08081919, 0x192b1908, 0x08081919,
        0x2b080808, 0x08081919, 0x2b08082b, 0x08081919, 0x2b081919, 0x08081919, 0x2b082b08, 0x08081919,
        0x2b190819, 0x08081919, 0x2b191908, 0x08081919, 0x2b2b0808, 0x08081919, 0x08080819, 0x0808192b,
        0x08081908, 0x0808192b, 0x0808192b, 0x0808192b, 0x08082b19, 0x0808192b, 0x08190808, 0x0808192b,
        0x08191919, 0x0808192b, 0x19080808, 0x0808192b, 0x19081919, 0x0808192b, 0x19082b08, 0x0808192b,
        0x19190819, 0x0808192b, 0x19191908, 0x0808192b, 0x192b0808, 0x0808192b, 0x2b080819, 0x0808192b,
        0x2b081908, 0x0808192b, 0x2b190808, 0x0808192b, 0x08080808, 0x08082b08, 0x0808082b, 0x08082b08,
        0x08081919, 0x08082b08, 0x08082b08, 0x08082b08, 0x08190819, 0x08082b08, 0x08191908, 0x08082b08,
        0x0819192b, 0x08082b08, 0x08192b19, 0x08082b08, 0x082b0808, 0x08082b08, 0x082b1919, 0x08082b08,
        0x082b2b2b, 0x08082b08, 0x19080819, 0x08082b08, 0x19081908, 0x08082b08, 0x1908192b, 0x08082b08,
        0x19082b19, 0x08082b08, 0x19190808, 0x08082b08, 0x1919082b, 0x08082b08, 0x19191919, 0x08082b08,
        0x19192b08, 0x08082b08, 0x192b0819, 0x08082b08, 0x192b1908, 0x08082b08, 0x2b080808, 0x08082b08,
        0x2b081919, 0x08082b08, 0x2b191908, 0x08082b08, 0x2b2b2b2b, 0x08082b08, 0x08080819, 0x08082b19,
        0x08081908, 0x08082b19, 0x08190808, 0x08082b19, 0x0819082b, 0x08082b19, 0x08191919, 0x08082b19,
        0x08192b08, 0x08082b19, 0x082b0819, 0x08082b19, 0x19080808, 0x08082b19, 0x19081919, 0x08082b19,
        0x19082b08, 0x08082b19, 0x19190819, 0x08082b19, 0x19191908, 0x08082b19, 0x192b0808, 0x08082b19,
        0x2b080819, 0x08082b19, 0x2b190808, 0x08082b19, 0x08080808, 0x08082b2b, 0x08190819, 0x08082b2b,
        0x08191908, 0x08082b2b, 0x082b082b, 0x08082b2b, 0x082b2b08, 0x08082b2b, 0x082b2b2b, 0x08082b2b,
        0x19190808, 0x08082b2b, 0x2b192b19, 0x08082b2b, 0x08080819, 0x08190808, 0x08081908, 0x08190808,
        0x0808192b, 0x08190808, 0x08082b19, 0x08190808, 0x08190808, 0x08190808, 0x0819082b, 0x08190808,
        0x08191919, 0x08190808, 0x08192b08, 0x08190808, 0x082b0819, 0x08190808, 0x082b1908, 0x08190808,
        0x082b192b, 0x08190808, 0x19080808, 0x08190808, 0x1908082b, 0x08190808, 0x19081919, 0x08190808,
        0x19082b08, 0x08190808, 0x19190819, 0x08190808, 0x19191908, 0x08190808, 0x1919192b, 0x08190808,
        0x19192b19, 0x08190808, 0x192b0808, 0x08190808, 0x192b082b, 0x08190808, 0x192b1919, 0x08190808,
        0x192b2b08, 0x08190808, 0x2b080819, 0x08190808, 0x2b081908, 0x08190808, 0x2b08192b, 0x08190808,
        0x2b190808, 0x08190808, 0x2b191919, 0x08190808, 0x2b192b08, 0x08190808, 0x2b2b0819, 0x08190808,
        0x2b2b1908, 0x08190808, 0x08080808, 0x08190819, 0x0808082b, 0x08190819, 0x08081919, 0x08190819,
        0x08082b08, 0x08190819, 0x08082b2b, 0x08190819, 0x08190819, 0x08190819, 0x08191908, 0x08190819,
        0x0819192b, 0x08190819, 0x08192b19, 0x08190819, 0x082b0808, 0x08190819, 0x082b082b, 0x08190819,
        0x082b1919, 0x08190819, 0x082b2b08, 0x08190819, 0x19080819, 0x08190819, 0x19081908, 0x08190819,
        0x1908192b, 0x08190819, 0x19082b19, 0x08190819, 0x19190808, 0x08190819, 0x1919082b, 0x08190819,
        0x19191919, 0x08190819, 0x19192b08, 0x08190819, 0x192b0819, 0x08190819, 0x192b1908, 0x08190819,
        0x2b080808, 0x08190819, 0x2b08082b, 0x08190819, 0x2b081919, 0x08190819, 0x2b082b08, 0x08190819,
        0x2b190819, 0x08190819, 0x2b191908, 0x08190819, 0x08080819, 0x0819082b, 0x08081908, 0x0819082b,
        0x08082b19, 0x0819082b, 0x08190808, 0x0819082b, 0x08191919, 0x0819082b, 0x082b0819, 0x0819082b,
        0x082b1908, 0x0819082b, 0x19080808, 0x0819082b, 0x19081919, 0x0819082b, 0x19190819, 0x0819082b,
        0x19191908, 0x0819082b, 0x2b080819, 0x0819082b, 0x2b081908, 0x0819082b, 0x2b190808, 0x0819082b,
        0x08080808, 0x08191908, 0x0808082b, 0x08191908, 0x08081919, 0x08191908, 0x08082b08, 0x08191908,
        0x08190819, 0x08191908, 0x08191908, 0x08191908, 0x0819192b, 0x08191908, 0x08192b19, 0x08191908,
        0x082b0808, 0x08191908, 0x082b1919, 0x08191908, 0x082b2b08, 0x08191908, 0x19080819, 0x08191908,
        0x19081908, 0x08191908, 0x1908192b, 0x08191908, 0x19082b19, 0x08191908, 0x19190808, 0x08191908,
        0x1919082b, 0x08191908, 0x19191919, 0x08191908, 0x19192b08, 0x08191908, 0x192b0819, 0x08191908,
        0x192b1908, 0x08191908, 0x2b080808, 0x08191908, 0x2b08082b, 0x08191908, 0x2b081919, 0x08191908,
        0x2b082b08, 0x08191908, 0x2b190819, 0x08191908, 0x2b191908, 0x08191908, 0x2b2b0808, 0x08191908,
        0x08080819, 0x08191919, 0x08081908, 0x08191919, 0x0808192b, 0x08191919, 0x08082b19, 0x08191919,
        0x08190808, 0x08191919, 0x0819082b, 0x08191919, 0x08191919, 0x08191919, 0x08192b08, 0x08191919,
        0x082b0819, 0x08191919, 0x082b1908, 0x08191919, 0x19080808, 0x08191919, 0x1908082b, 0x08191919,
        0x19081919, 0x08191919, 0x19082b08, 0x08191919, 0x19190819, 0x08191919, 0x19191908, 0x08191919,
        0x192b0808, 0x08191919, 0x2b080819, 0x08191919, 0x2b081908, 0x08191919, 0x2b190808, 0x08191919,
        0x08080808, 0x0819192b, 0x08081919, 0x0819192b, 0x08082b08, 0x0819192b, 0x08190819, 0x0819192b,
        0x08191908, 0x0819192b, 0x082b0808, 0x0819192b, 0x19080819, 0x0819192b, 0x19081908, 0x0819192b,
        0x19190808, 0x0819192b, 0x2b080808, 0x0819192b, 0x2b2b2b2b, 0x0819192b, 0x08080819, 0x08192b08,
        0x08081908, 0x08192b08, 0x0808192b, 0x08192b08, 0x08082b19, 0x08192b08, 0x08190808, 0x08192b08,
        0x08191919, 0x08192b08, 0x08192b08, 0x08192b08, 0x082b0819, 0x08192b08, 0x19080808, 0x08192b08,
        0x1908082b, 0x08192b08, 0x19081919, 0x08192b08, 0x19082b08, 0x08192b08, 0x19190819, 0x08192b08,
        0x19191908, 0x08192b08, 0x192b0808, 0x08192b08, 0x2b080819, 0x08192b08, 0x2b081908, 0x08192b08,
        0x08080808, 0x08192b19, 0x0808082b, 0x08192b19, 0x08081919, 0x08192b19, 0x08082b08, 0x08192b19,
        0x08190819, 0x08192b19, 0x08191908, 0x08192b19, 0x082b0808, 0x08192b19, 0x19080819, 0x08192b19,
        0x19081908, 0x08192b19, 0x19190808, 0x08192b19, 0x192b2b19, 0x08192b19, 0x2b2b082b, 0x08192b19,
        0x08081908, 0x08192b2b, 0x08190808, 0x08192b2b, 0x19080808, 0x08192b2b, 0x1919192b, 0x08192b2b,
        0x08080808, 0x082b0808, 0x0808082b, 0x082b0808, 0x08081919, 0x082b0808, 0x08082b08, 0x082b0808,
        0x08190819, 0x082b0808, 0x08191908, 0x082b0808, 0x0819192b, 0x082b0808, 0x08192b19, 0x082b0808,
        0x082b0808, 0x082b0808, 0x082b1919, 0x082b0808, 0x082b2b2b, 0x082b0808, 0x19080819, 0x082b0808,
        0x19081908, 0x082b0808, 0x19190808, 0x082b0808, 0x1919082b, 0x082b0808, 0x19191919, 0x082b0808,
        0x192b1908, 0x082b0808, 0x2b080808, 0x082b0808, 0x2b082b2b, 0x082b0808, 0x2b191908, 0x082b0808,
        0x2b2b2b2b, 0x082b0808, 0x08080819, 0x082b0819, 0x08081908, 0x082b0819, 0x08190808, 0x082b0819,
        0x0819082b, 0x082b0819, 0x08191919, 0x082b0819, 0x082b0819, 0x082b0819, 0x19080808, 0x082b0819,
        0x1908082b, 0x082b0819, 0x19081919, 0x082b0819, 0x19190819, 0x082b0819, 0x19191908, 0x082b0819,
        0x192b0808, 0x082b0819, 0x2b080819, 0x082b0819, 0x2b081908, 0x082b0819, 0x2b190808, 0x082b0819,
        0x08080808, 0x082b082b, 0x08082b2b, 0x082b082b, 0x082b082b, 0x082b082b, 0x082b2b08, 0x082b082b,
        0x082b2b2b, 0x082b082b, 0x19081908, 0x082b082b, 0x19190808, 0x082b082b, 0x2b082b08, 0x082b082b,
        0x2b082b2b, 0x082b082b, 0x2b2b2b08, 0x082b082b, 0x08080819, 0x082b1908, 0x08081908, 0x082b1908,
        0x0808192b, 0x082b1908, 0x08082b19, 0x082b1908, 0x08190808, 0x082b1908, 0x08191919, 0x082b1908,
        0x08192b08, 0x082b1908, 0x082b0819, 0x082b1908, 0x082b1908, 0x082b1908, 0x19080808, 0x082b1908,
        0x1908082b, 0x082b1908, 0x19081919, 0x082b1908, 0x19082b08, 0x082b1908, 0x19190819, 0x082b1908,
        0x19191908, 0x082b1908, 0x192b0808, 0x082b1908, 0x2b080819, 0x082b1908, 0x2b081908, 0x082b1908,
        0x2b190808, 0x082b1908, 0x08080808, 0x082b1919, 0x08081919, 0x082b1919, 0x08082b08, 0x082b1919,
        0x08190819, 0x082b1919, 0x08191908, 0x082b1919, 0x082b0808, 0x082b1919, 0x19080819, 0x082b1919,
        0x19081908, 0x082b1919, 0x19190808, 0x082b1919, 0x192b192b, 0x082b1919, 0x2b080808, 0x082b1919,
        0x08080819, 0x082b192b, 0x08081908, 0x082b192b, 0x08190808, 0x082b192b, 0x19080808, 0x082b192b,
        0x19192b19, 0x082b192b, 0x08080808, 0x082b2b08, 0x08081919, 0x082b2b08, 0x08190819, 0x082b2b08,
        0x08191908, 0x082b2b08, 0x19080819, 0x082b2b08, 0x19081908, 0x082b2b08, 0x19190808, 0x082b2b08,
        0x2b082b2b, 0x082b2b08, 0x2b2b2b2b, 0x082b2b08, 0x08080819, 0x082b2b19, 0x08081908, 0x082b2b19,
        0x08190808, 0x082b2b19, 0x2b191919, 0x082b2b19, 0x08082b2b, 0x082b2b2b, 0x082b082b, 0x082b2b2b,
        0x192b1908, 0x082b2b2b, 0x2b082b08, 0x082b2b2b, 0x2b082b2b, 0x082b2b2b, 0x08080819, 0x19080808,
        0x08081908, 0x19080808, 0x0808192b, 0x19080808, 0x08082b19, 0x19080808, 0x08190808, 0x19080808,
        0x0819082b, 0x19080808, 0x08191919, 0x19080808, 0x08192b08, 0x19080808, 0x08192b2b, 0x19080808,
        0x082b0819, 0x19080808, 0x082b1908, 0x19080808, 0x082b192b, 0x19080808, 0x19080808, 0x19080808,
        0x1908082b, 0x19080808, 0x19081919, 0x19080808, 0x19082b08, 0x19080808, 0x19082b2b, 0x19080808,
        0x19190819, 0x19080808, 0x19191908, 0x19080808, 0x1919192b, 0x19080808, 0x19192b19, 0x19080808,
        0x192b0808, 0x19080808, 0x192b082b, 0x19080808, 0x192b1919, 0x19080808, 0x2b080819, 0x19080808,
        0x2b081908, 0x19080808, 0x2b190808, 0x19080808, 0x2b191919, 0x19080808, 0x2b192b08, 0x19080808,
        0x2b2b0819, 0x19080808, 0x2b2b1908, 0x19080808, 0x08080808, 0x19080819, 0x0808082b, 0x19080819,
        0x08081919, 0x19080819, 0x08082b08, 0x19080819, 0x08190819, 0x19080819, 0x08191908, 0x19080819,
        0x0819192b, 0x19080819, 0x08192b19, 0x19080819, 0x082b0808, 0x19080819, 0x082b082b, 0x19080819,
        0x082b1919, 0x19080819, 0x19080819, 0x19080819, 0x19081908, 0x19080819, 0x1908192b, 0x19080819,
        0x19082b19, 0x19080819, 0x19190808, 0x19080819, 0x1919082b, 0x19080819, 0x19191919, 0x19080819,
        0x19192b08, 0x19080819, 0x192b0819, 0x19080819, 0x192b1908, 0x19080819, 0x2b080808, 0x19080819,
        0x2b08082b, 0x19080819, 0x2b081919, 0x19080819, 0x2b082b08, 0x19080819, 0x2b190819, 0x19080819,
        0x2b191908, 0x19080819, 0x2b2b0808, 0x19080819, 0x08080819, 0x1908082b, 0x08081908, 0x1908082b,
        0x08190808, 0x1908082b, 0x0819082b, 0x1908082b, 0x08191919, 0x1908082b, 0x08192b08, 0x1908082b,
        0x082b1908, 0x1908082b, 0x19080808, 0x1908082b, 0x19081919, 0x1908082b, 0x19082b08, 0x1908082b,
        0x19190819, 0x1908082b, 0x19191908, 0x1908082b, 0x192b0808, 0x1908082b, 0x2b080819, 0x1908082b,
        0x2b081908, 0x1908082b, 0x08080808, 0x19081908, 0x0808082b, 0x19081908, 0x08081919, 0x19081908,
        0x08082b08, 0x19081908, 0x08082b2b, 0x19081908, 0x08190819, 0x19081908, 0x08191908, 0x19081908,
        0x0819192b, 0x19081908, 0x08192b19, 0x19081908, 0x082b0808, 0x19081908, 0x082b082b, 0x19081908,
        0x082b1919, 0x19081908, 0x082b2b08, 0x19081908, 0x19080819, 0x19081908, 0x19081908, 0x19081908,
        0x1908192b, 0x19081908, 0x19082b19, 0x19081908, 0x19190808, 0x19081908, 0x1919082b, 0x19081908,
        0x19191919, 0x19081908, 0x19192b08, 0x19081908, 0x192b0819, 0x19081908, 0x192b1908, 0x19081908,
        0x2b080808, 0x19081908, 0x2b08082b, 0x19081908, 0x2b081919, 0x19081908, 0x2b082b08, 0x19081908,
        0x2b190819, 0x19081908, 0x2b191908, 0x19081908, 0x2b2b0808, 0x19081908, 0x08080819, 0x19081919,
        0x08081908, 0x19081919, 0x0808192b, 0x19081919, 0x08082b19, 0x19081919, 0x08190808, 0x19081919,
        0x0819082b, 0x19081919, 0x08191919, 0x19081919, 0x08192b08, 0x19081919, 0x082b0819, 0x19081919,
        0x082b1908, 0x19081919, 0x19080808, 0x19081919, 0x1908082b, 0x19081919, 0x19081919, 0x19081919,
        0x19082b08, 0x19081919, 0x19190819, 0x19081919, 0x19191908, 0x19081919, 0x192b0808, 0x19081919,
        0x192b2b2b, 0x19081919, 0x2b080819, 0x19081919, 0x2b081908, 0x19081919, 0x2b190808, 0x19081919,
        0x08080808, 0x1908192b, 0x0808082b, 0x1908192b, 0x08081919, 0x1908192b, 0x08082b08, 0x1908192b,
        0x08190819, 0x1908192b, 0x08191908, 0x1908192b, 0x082b0808, 0x1908192b, 0x19080819, 0x1908192b,
        0x19081908, 0x1908192b, 0x19190808, 0x1908192b, 0x2b080808, 0x1908192b, 0x2b2b1919, 0x1908192b,
        0x08080819, 0x19082b08, 0x08081908, 0x19082b08, 0x08082b19, 0x19082b08, 0x08190808, 0x19082b08,
        0x0819082b, 0x19082b08, 0x08191919, 0x19082b08, 0x08192b08, 0x19082b08, 0x082b0819, 0x19082b08,
        0x082b1908, 0x19082b08, 0x19080808, 0x19082b08, 0x1908082b, 0x19082b08, 0x19081919, 0x19082b08,
        0x19082b08, 0x19082b08, 0x19190819, 0x19082b08, 0x19191908, 0x19082b08, 0x192b0808, 0x19082b08,
        0x2b081908, 0x19082b08, 0x2b190808, 0x19082b08, 0x08080808, 0x19082b19, 0x0808082b, 0x19082b19,
        0x08081919, 0x19082b19, 0x08082b08, 0x19082b19, 0x08190819, 0x19082b19, 0x08191908, 0x19082b19,
        0x082b0808, 0x19082b19, 0x19080819, 0x19082b19, 0x19081908, 0x19082b19, 0x19190808, 0x19082b19,
        0x2b080808, 0x19082b19, 0x2b19192b, 0x19082b19, 0x08080819, 0x19082b2b, 0x08081908, 0x19082b2b,
        0x08190808, 0x19082b2b, 0x19080808, 0x19082b2b, 0x08080808, 0x19190808, 0x0808082b, 0x19190808,
        0x08081919, 0x19190808, 0x08082b08, 0x19190808, 0x08190819, 0x19190808, 0x08191908, 0x19190808,
        0x0819192b, 0x19190808, 0x08192b19, 0x19190808, 0x082b0808, 0x19190808, 0x082b082b, 0x19190808,
        0x082b1919, 0x19190808, 0x082b2b08, 0x19190808, 0x19080819, 0x19190808, 0x19081908, 0x19190808,
        0x1908192b, 0x19190808, 0x19082b19, 0x19190808, 0x19190808, 0x19190808, 0x1919082b, 0x19190808,
        0x19191919, 0x19190808, 0x19192b08, 0x19190808, 0x192b0819, 0x19190808, 0x192b1908, 0x19190808,
        0x2b080808, 0x19190808, 0x2b08082b, 0x19190808, 0x2b081919, 0x19190808, 0x2b082b08, 0x19190808,
        0x2b190819, 0x19190808, 0x2b191908, 0x19190808, 0x08080819, 0x19190819, 0x08081908, 0x19190819,
        0x0808192b, 0x19190819, 0x08082b19, 0x19190819, 0x08190808, 0x19190819, 0x0819082b, 0x19190819,
        0x08191919, 0x19190819, 0x08192b08, 0x19190819, 0x082b0819, 0x19190819, 0x082b1908, 0x19190819,
        0x19080808, 0x19190819, 0x1908082b, 0x19190819, 0x19081919, 0x19190819, 0x19082b08, 0x19190819,
        0x19190819, 0x19190819, 0x19191908, 0x19190819, 0x192b0808, 0x19190819, 0x2b080819, 0x19190819,
        0x2b081908, 0x19190819, 0x2b190808, 0x19190819, 0x08080808, 0x1919082b, 0x08081919, 0x1919082b,
        0x08082b08, 0x1919082b, 0x08190819, 0x1919082b, 0x08191908, 0x1919082b, 0x082b0808, 0x1919082b,
        0x19080819, 0x1919082b, 0x19081908, 0x1919082b, 0x19190808, 0x1919082b, 0x192b2b19, 0x1919082b,
        0x2b080808, 0x1919082b, 0x08080819, 0x19191908, 0x08081908, 0x19191908, 0x0808192b, 0x19191908,
        0x08082b19, 0x19191908, 0x08190808, 0x19191908, 0x0819082b, 0x19191908, 0x08191919, 0x19191908,
        0x08192b08, 0x19191908, 0x082b0819, 0x19191908, 0x082b1908, 0x19191908, 0x19080808, 0x19191908,
        0x1908082b, 0x19191908, 0x19081919, 0x19191908, 0x19082b08, 0x19191908, 0x19190819, 0x19191908,
        0x19191908, 0x19191908, 0x192b0808, 0x19191908, 0x2b080819, 0x19191908, 0x2b081908, 0x19191908,
        0x2b190808, 0x19191908, 0x08080808, 0x19191919, 0x0808082b, 0x19191919, 0x08081919, 0x19191919,
        0x08082b08, 0x19191919, 0x08190819, 0x19191919, 0x08191908, 0x19191919, 0x082b0808, 0x19191919,
        0x19080819, 0x19191919, 0x19081908, 0x19191919, 0x19190808, 0x19191919, 0x2b080808, 0x19191919,
        0x08080819, 0x1919192b, 0x08081908, 0x1919192b, 0x08190808, 0x1919192b, 0x082b192b, 0x1919192b,
        0x19080808, 0x1919192b, 0x08080808, 0x19192b08, 0x0808082b, 0x19192b08, 0x08081919, 0x19192b08,
        0x08082b08, 0x19192b08, 0x08190819, 0x19192b08, 0x08191908, 0x19192b08, 0x082b0808, 0x19192b08,
        0x19080819, 0x19192b08, 0x19081908, 0x19192b08, 0x19190808, 0x19192b08, 0x19192b2b, 0x19192b08,
        0x2b080808, 0x19192b08, 0x08080819, 0x19192b19, 0x08081908, 0x19192b19, 0x08190808, 0x19192b19,
        0x19080808, 0x19192b19, 0x08080808, 0x19192b2b, 0x08192b19, 0x19192b2b, 0x2b081919, 0x19192b2b,
        0x2b2b2b08, 0x19192b2b, 0x08080819, 0x192b0808, 0x08081908, 0x192b0808, 0x0808192b, 0x192b0808,
        0x08190808, 0x192b0808, 0x0819082b, 0x192b0808, 0x08191919, 0x192b0808, 0x08192b08, 0x192b0808,
        0x082b0819, 0x192b0808, 0x082b1908, 0x192b0808, 0x19080808, 0x192b0808, 0x19081919, 0x192b0808,
        0x19082b08, 0x192b0808, 0x19190819, 0x192b0808, 0x19191908, 0x192b0808, 0x192b0808, 0x192b0808,
        0x2b081908, 0x192b0808, 0x2b190808, 0x192b0808, 0x08080808, 0x192b0819, 0x0808082b, 0x192b0819,
        0x08081919, 0x192b0819, 0x08082b08, 0x192b0819, 0x08190819, 0x192b0819, 0x08191908, 0x192b0819,
        0x082b0808, 0x192b0819, 0x19080819, 0x192b0819, 0x19081908, 0x192b0819, 0x19190808, 0x192b0819,
        0x2b080808, 0x192b0819, 0x2b192b19, 0x192b0819, 0x08081908, 0x192b082b, 0x08190808, 0x192b082b,
        0x19080808, 0x192b082b, 0x1919192b, 0x192b082b, 0x2b2b0819, 0x192b082b, 0x08080808, 0x192b1908,
        0x08081919, 0x192b1908, 0x08082b08, 0x192b1908, 0x08190819, 0x192b1908, 0x08191908, 0x192b1908,
        0x082b0808, 0x192b1908, 0x19080819, 0x192b1908, 0x19081908, 0x192b1908, 0x19190808, 0x192b1908,
        0x2b080808, 0x192b1908, 0x08080819, 0x192b1919, 0x08081908, 0x192b1919, 0x08190808, 0x192b1919,
        0x19080808, 0x192b1919, 0x19082b2b, 0x192b1919, 0x192b2b08, 0x192b1919, 0x2b19082b, 0x192b1919,
        0x08080808, 0x192b192b, 0x2b191908, 0x192b192b, 0x08080819, 0x192b2b08, 0x08081908, 0x192b2b08,
        0x08190808, 0x192b2b08, 0x192b1919, 0x192b2b08, 0x2b192b08, 0x192b2b08, 0x08080808, 0x192b2b19,
        0x082b2b2b, 0x192b2b19, 0x1908082b, 0x192b2b2b, 0x2b2b0819, 0x192b2b2b, 0x08080808, 0x2b080808,
        0x0808082b, 0x2b080808, 0x08081919, 0x2b080808, 0x08082b08, 0x2b080808, 0x08190819, 0x2b080808,
        0x08191908, 0x2b080808, 0x08192b19, 0x2b080808, 0x082b0808, 0x2b080808, 0x082b1919, 0x2b080808,
        0x19080819, 0x2b080808, 0x19081908, 0x2b080808, 0x19190808, 0x2b080808, 0x1919082b, 0x2b080808,
        0x19191919, 0x2b080808, 0x19192b08, 0x2b080808, 0x192b0819, 0x2b080808, 0x2b080808, 0x2b080808,
        0x2b081919, 0x2b080808, 0x2b190819, 0x2b080808, 0x2b191908, 0x2b080808, 0x08080819, 0x2b080819,
        0x08081908, 0x2b080819, 0x08082b19, 0x2b080819, 0x08190808, 0x2b080819, 0x0819082b, 0x2b080819,
        0x08191919, 0x2b080819, 0x08192b08, 0x2b080819, 0x082b0819, 0x2b080819, 0x082b1908, 0x2b080819,
        0x19080808, 0x2b080819, 0x1908082b, 0x2b080819, 0x19081919, 0x2b080819, 0x19082b08, 0x2b080819,
        0x19190819, 0x2b080819, 0x19191908, 0x2b080819, 0x2b080819, 0x2b080819, 0x2b081908, 0x2b080819,
        0x2b190808, 0x2b080819, 0x2b2b2b19, 0x2b080819, 0x08080808, 0x2b08082b, 0x08081919, 0x2b08082b,
        0x08082b2b, 0x2b08082b, 0x08190819, 0x2b08082b, 0x08191908, 0x2b08082b, 0x19080819, 0x2b08082b,
        0x19081908, 0x2b08082b, 0x19190808, 0x2b08082b, 0x08080819, 0x2b081908, 0x08081908, 0x2b081908,
        0x0808192b, 0x2b081908, 0x08082b19, 0x2b081908, 0x08190808, 0x2b081908, 0x0819082b, 0x2b081908,
        0x08191919, 0x2b081908, 0x08192b08, 0x2b081908, 0x082b0819, 0x2b081908, 0x19080808, 0x2b081908,
        0x1908082b, 0x2b081908, 0x19081919, 0x2b081908, 0x19082b08, 0x2b081908, 0x19190819, 0x2b081908,
        0x19191908, 0x2b081908, 0x192b0808, 0x2b081908, 0x2b080819, 0x2b081908, 0x2b081908, 0x2b081908,
        0x2b190808, 0x2b081908, 0x08080808, 0x2b081919, 0x0808082b, 0x2b081919, 0x08081919, 0x2b081919,
        0x08082b08, 0x2b081919, 0x08190819, 0x2b081919, 0x08191908, 0x2b081919, 0x082b0808, 0x2b081919,
        0x19080819, 0x2b081919, 0x19081908, 0x2b081919, 0x19190808, 0x2b081919, 0x2b080808, 0x2b081919,
        0x2b082b2b, 0x2b081919, 0x08080819, 0x2b08192b, 0x08081908, 0x2b08192b, 0x08190808, 0x2b08192b,
        0x082b2b19, 0x2b08192b, 0x19080808, 0x2b08192b, 0x08080808, 0x2b082b08, 0x08081919, 0x2b082b08,
        0x08190819, 0x2b082b08, 0x08191908, 0x2b082b08, 0x19080819, 0x2b082b08, 0x19081908, 0x2b082b08,
        0x19190808, 0x2b082b08, 0x2b2b082b, 0x2b082b08, 0x08080819, 0x2b082b19, 0x08081908, 0x2b082b19,
        0x19080808, 0x2b082b19, 0x192b1919, 0x2b082b19, 0x082b082b, 0x2b082b2b, 0x19192b08, 0x2b082b2b,
        0x19192b2b, 0x2b082b2b, 0x2b08082b, 0x2b082b2b, 0x2b2b082b, 0x2b082b2b, 0x08080819, 0x2b190808,
        0x08081908, 0x2b190808, 0x08082b19, 0x2b190808, 0x08190808, 0x2b190808, 0x0819082b, 0x2b190808,
        0x08191919, 0x2b190808, 0x08192b08, 0x2b190808, 0x082b1908, 0x2b190808, 0x19080808, 0x2b190808,
        0x1908082b, 0x2b190808, 0x19081919, 0x2b190808, 0x19082b08, 0x2b190808, 0x19190819, 0x2b190808,
        0x19191908, 0x2b190808, 0x192b0808, 0x2b190808, 0x2b080819, 0x2b190808, 0x2b081908, 0x2b190808,
        0x2b190808, 0x2b190808, 0x08080808, 0x2b190819, 0x08081919, 0x2b190819, 0x08190819, 0x2b190819,
        0x08191908, 0x2b190819, 0x19080819, 0x2b190819, 0x19081908, 0x2b190819, 0x19190808, 0x2b190819,
        0x19192b2b, 0x2b190819, 0x08080819, 0x2b19082b, 0x08081908, 0x2b19082b, 0x08190808, 0x2b19082b,
        0x19080808, 0x2b19082b, 0x2b2b192b, 0x2b19082b, 0x08080808, 0x2b191908, 0x0808082b, 0x2b191908,
        0x08081919, 0x2b191908, 0x08082b08, 0x2b191908, 0x08190819, 0x2b191908, 0x08191908, 0x2b191908,
        0x082b0808, 0x2b191908, 0x19080819, 0x2b191908, 0x19081908, 0x2b191908, 0x19190808, 0x2b191908,
        0x2b080808, 0x2b191908, 0x2b19192b, 0x2b191908, 0x08080819, 0x2b191919, 0x08081908, 0x2b191919,
        0x08190808, 0x2b191919, 0x19080808, 0x2b191919, 0x2b192b08, 0x2b191919, 0x2b2b0819, 0x2b191919,
        0x08080808, 0x2b19192b, 0x1908192b, 0x2b19192b, 0x192b1908, 0x2b19192b, 0x08080819, 0x2b192b08,
        0x08081908, 0x2b192b08, 0x08190808, 0x2b192b08, 0x082b192b, 0x2b192b08, 0x19080808, 0x2b192b08,
        0x2b2b2b19, 0x2b192b08, 0x08080808, 0x2b192b19, 0x19082b19, 0x2b192b19, 0x1919082b, 0x2b192b19,
        0x2b190808, 0x2b192b2b, 0x08080808, 0x2b2b0808, 0x08081919, 0x2b2b0808, 0x08082b2b, 0x2b2b0808,
        0x08191908, 0x2b2b0808, 0x082b082b, 0x2b2b0808, 0x082b2b2b, 0x2b2b0808, 0x19080819, 0x2b2b0808,
        0x19081908, 0x2b2b0808, 0x19190808, 0x2b2b0808, 0x2b2b082b, 0x2b2b0808, 0x2b2b2b2b, 0x2b2b0808,
        0x19080808, 0x2b2b0819, 0x192b1919, 0x2b2b0819, 0x0808082b, 0x2b2b082b, 0x08082b2b, 0x2b2b082b,
        0x082b082b, 0x2b2b082b, 0x082b2b08, 0x2b2b082b, 0x082b2b2b, 0x2b2b082b, 0x2b08082b, 0x2b2b082b,
        0x2b082b08, 0x2b2b082b, 0x2b082b2b, 0x2b2b082b, 0x2b2b2b08, 0x2b2b082b, 0x08080819, 0x2b2b1908,
        0x08081908, 0x2b2b1908, 0x08190808, 0x2b2b1908, 0x19080808, 0x2b2b1908, 0x2b082b19, 0x2b2b1908,
        0x2b2b1908, 0x2b2b1908, 0x08080808, 0x2b2b1919, 0x08192b19, 0x2b2b1919, 0x19190819, 0x2b2b192b,
        0x08082b2b, 0x2b2b2b08, 0x082b2b08, 0x2b2b2b08, 0x2b2b082b, 0x2b2b2b08, 0x19191908, 0x2b2b2b19,
        0x2b08192b, 0x2b2b2b19, 0x08082b08, 0x2b2b2b2b, 0x08082b2b, 0x2b2b2b2b, 0x082b0808, 0x2b2b2b2b,
        0x082b082b, 0x2b2b2b2b, 0x082b2b08, 0x2b2b2b2b, 0x2b082b08, 0x2b2b2b2b, 0x2b2b2b2b, 0x2b2b2b2b)
    return tbl[i]
}

// the HALVED iq3xxs grid (iq3xxs_grid with every byte / 2 - the plane convention that
// absorbs the format's 0.25 into the stored d), one word per call; staging loops copy it into
// workgroup memory (QUIRK 20's slow path stays off the hot loop).
def private iq3xxs_grid_word(i : int) : uint {
    let tbl = fixed_array<uint>(
        0x02020202u, 0x0202020au, 0x02020212u, 0x02020606u, 0x0202060eu, 0x0202061fu, 0x02020a02u, 0x02020a0au,
        0x02020e06u, 0x0202120au, 0x02021f0eu, 0x02021f16u, 0x02060206u, 0x0206020eu, 0x02060602u, 0x0206060au,
        0x02060a06u, 0x02060a16u, 0x02060e02u, 0x02060e0au, 0x02061206u, 0x02061612u, 0x02061f02u, 0x020a0202u,
        0x020a020au, 0x020a0212u, 0x020a0606u, 0x020a0a02u, 0x020a0a0au, 0x020a0e06u, 0x020a0e0eu, 0x020a0e1fu,
        0x020a1606u, 0x020a161fu, 0x020a1f16u, 0x020e0206u, 0x020e021fu, 0x020e0602u, 0x020e060au, 0x020e0a16u,
        0x020e1f02u, 0x0212060eu, 0x02120e1fu, 0x02121212u, 0x0212161fu, 0x02121f0eu, 0x02121f16u, 0x02160206u,
        0x0216021fu, 0x02160e0au, 0x0216160au, 0x021a0e16u, 0x021a1a12u, 0x021f0602u, 0x021f0612u, 0x021f061au,
        0x021f120eu, 0x021f1a06u, 0x06020206u, 0x0602020eu, 0x06020602u, 0x0602060au, 0x06020a06u, 0x06020a0eu,
        0x06020e02u, 0x06020e0au, 0x06020e12u, 0x0602121fu, 0x06021602u, 0x06060202u, 0x0606020au, 0x06060606u,
        0x06060a02u, 0x06060a0au, 0x060a0206u, 0x060a020eu, 0x060a0602u, 0x060a060au, 0x060a0a06u, 0x060a0e02u,
        0x060a1f0au, 0x060e0202u, 0x060e020au, 0x060e0a02u, 0x060e0e06u, 0x060e121au, 0x060e1a1au, 0x06120206u,
        0x06120216u, 0x06121602u, 0x06160a02u, 0x06160a12u, 0x0616121au, 0x06161f06u, 0x061a0216u, 0x061f0a0au,
        0x061f1202u, 0x0a020202u, 0x0a02020au, 0x0a020606u, 0x0a02060eu, 0x0a020a02u, 0x0a020a0au, 0x0a020a1au,
        0x0a020e06u, 0x0a02120au, 0x0a060206u, 0x0a06020eu, 0x0a060216u, 0x0a060602u, 0x0a06060au, 0x0a060a06u,
        0x0a060e02u, 0x0a061a0eu, 0x0a061a1fu, 0x0a061f02u, 0x0a0a0202u, 0x0a0a020au, 0x0a0a0606u, 0x0a0a061fu,
        0x0a0a0a02u, 0x0a0a0a0au, 0x0a0a0e1fu, 0x0a0a1202u, 0x0a0a1616u, 0x0a0e0206u, 0x0a0e0602u, 0x0a0e0612u,
        0x0a0e1f02u, 0x0a0e1f12u, 0x0a120e16u, 0x0a12160eu, 0x0a16020eu, 0x0a160a1fu, 0x0a161206u, 0x0a161f12u,
        0x0a1f0206u, 0x0a1f020eu, 0x0a1f061au, 0x0a1f1216u, 0x0e020206u, 0x0e020602u, 0x0e02060au, 0x0e020a06u,
        0x0e020a0eu, 0x0e021602u, 0x0e021a16u, 0x0e021f0au, 0x0e060202u, 0x0e06020au, 0x0e060a02u, 0x0e060e06u,
        0x0e061212u, 0x0e06121au, 0x0e0a0206u, 0x0e0a020eu, 0x0e0a0602u, 0x0e0a0a16u, 0x0e0a160au, 0x0e0a1f0au,
        0x0e0e0606u, 0x0e0e0e0eu, 0x0e120e02u, 0x0e12121fu, 0x0e121f0au, 0x0e160202u, 0x0e16021au, 0x0e160a0au,
        0x0e161616u, 0x0e1a0612u, 0x0e1a0e1au, 0x0e1a1a0eu, 0x0e1f0e0eu, 0x0e1f1a02u, 0x12020212u, 0x1202061fu,
        0x12020e16u, 0x12020e1fu, 0x1202160eu, 0x1202161fu, 0x12061f12u, 0x120a0a02u, 0x120a0e1fu, 0x120a1202u,
        0x120a1a02u, 0x120a1a1au, 0x120e021fu, 0x120e1216u, 0x12120212u, 0x12121606u, 0x12121a12u, 0x12160a16u,
        0x1216120eu, 0x12161f02u, 0x121f0216u, 0x121f0602u, 0x121f060au, 0x121f0e02u, 0x1602060au, 0x16021206u,
        0x16021f02u, 0x16060202u, 0x1606021au, 0x16060a1au, 0x16061616u, 0x160a0612u, 0x160a0e0au, 0x160a1f0au,
        0x160e020au, 0x160e160eu, 0x16120602u, 0x16120a0eu, 0x16120a1fu, 0x16121f0au, 0x1616020au, 0x16160e06u,
        0x161a1602u, 0x161f0a12u, 0x161f120au, 0x1a020a12u, 0x1a021212u, 0x1a02121au, 0x1a021a12u, 0x1a060a06u,
        0x1a061a06u, 0x1a0a061fu, 0x1a0a1a12u, 0x1a0e0e02u, 0x1a0e0e1au, 0x1a121212u, 0x1a160216u, 0x1a16160au,
        0x1a1a0e0eu, 0x1a1f020eu, 0x1a1f0a06u, 0x1f02020eu, 0x1f020216u, 0x1f02021fu, 0x1f020602u, 0x1f020e0au,
        0x1f02160au, 0x1f060a1au, 0x1f061202u, 0x1f0a060au, 0x1f0a1216u, 0x1f0a160au, 0x1f0e0202u, 0x1f0e0616u,
        0x1f0e0e0eu, 0x1f0e1a02u, 0x1f120a06u, 0x1f121206u, 0x1f160202u, 0x1f16020au, 0x1f160a12u, 0x1f1a0e02u)
    return tbl[i]
}

// ksigns_iq2xs without the table: bit 7 of the sign byte is the EVEN PARITY of the 7-bit
// index, so the byte is idx | (parity << 7) - five ALU ops, nothing staged.
def private ksign7(v : uint) : uint {
    var t = v ^ (v >> 4u)
    t = t ^ (t >> 2u)
    t = t ^ (t >> 1u)
    return v | ((t & 1u) << 7u)
}

def private iq4_word(n : uint) : uint {
    let tbl = uint4(3215825025u, 4142587343u, 639175937u, 1901675829u)   // kvalues_iq4nl, 4 bytes per word
    let q0 = n & 15u
    let q1 = (n >> 8u) & 15u
    let q2 = (n >> 16u) & 15u
    let q3 = (n >> 24u) & 15u
    let b0 = (tbl[int(q0 >> 2u)] >> ((q0 & 3u) * 8u)) & 0xFFu
    let b1 = (tbl[int(q1 >> 2u)] >> ((q1 & 3u) * 8u)) & 0xFFu
    let b2 = (tbl[int(q2 >> 2u)] >> ((q2 & 3u) * 8u)) & 0xFFu
    let b3 = (tbl[int(q3 >> 2u)] >> ((q3 & 3u) * 8u)) & 0xFFu
    return b0 | (b1 << 8u) | (b2 << 16u) | (b3 << 24u)
}

// the iq4xs scale row's sub-scale for block blk: word 1..2 of the 5-word row, SIGNED (ls - 32)
def private iq4_sc(w : uint; blk : uint) : int {
    let s4 = int4(unpack8(int(w)))
    return blk % 4u == 0u ? s4.x : (blk % 4u == 1u ? s4.y : (blk % 4u == 2u ? s4.z : s4.w))
}

class KqBatchBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant planes (per-format block layout)
    @ssbo @binding = 1 wsu : array<uint>       // scale planes (5-word superblock layout)
    @ssbo @binding = 2 sched : array<uint>     // region records + per-wg map
    @ssbo @binding = 3 xqw : array<uint>       // Q8_K activation words (64 per row-superblock)
    @ssbo @binding = 4 axs : array<float>      // one Q8_K d per (row, superblock)
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : BatchArgs
    @workgroup xw4 : uint4[544]
    @workgroup ww4 : uint4[544]
    @workgroup xsw : float[256]
    @workgroup wsw : float[256]    // first per-(block, col) scale plane
    @workgroup wsw2 : float[256]   // second plane (the d/min pair's min side; k6's hi sub-scale)

    def region_rec : uint4 {
        let rid = sched[pa.map_off + gl_WorkGroupID.x]
        let rb = rid * 4u
        return uint4(sched[rb], sched[rb + 1u], sched[rb + 2u], gl_WorkGroupID.x - sched[rb + 3u])
    }

    // x tile stage: this thread's two uint4 chunks of the Q8_K activation rows
    def load_x(row0, xrow, cnt, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var xv = uint4(0u)
            if (xrow < cnt) {
                let xb4 = ((row0 + xrow) * nsb + s) * 64u + bu * 8u + hh * 4u
                xv = uint4(xqw[xb4], xqw[xb4 + 1u], xqw[xb4 + 2u], xqw[xb4 + 3u])
            }
            xw4[grp * 17u + k] = xv
        }
    }

    def load_xs(row0, xt, cnt, nsb, s, tid : uint) {
        if (tid < 32u) {
            var xs = 0.0
            if (xt * 32u + tid < cnt) {
                xs = axs[(row0 + xt * 32u + tid) * nsb + s]
            }
            xsw[tid] = xs
        }
    }

    // w-scale stage for the d/min pair form (Q4_K/Q5_K share the 5-word wsu layout)
    def load_dm(wsb0, wt, nsb, s, slane, sblk : uint) {
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
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
    }

    // one 32-weight block's integer tile dots: this thread's 4 output columns + the x block sum
    def dots(bu, grp, word : uint; var i : int4&) : int {
        var bs = 0
        for (kk in range(2)) {
            let k = bu * 2u + uint(kk)
            let xw = xw4[grp * 17u + k]
            let wv0 = ww4[word * 17u + k]
            let wv1 = ww4[(word + 8u) * 17u + k]
            let wv2 = ww4[(word + 16u) * 17u + k]
            let wv3 = ww4[(word + 24u) * 17u + k]
            bs += sdot4(xw.x, 0x01010101) + sdot4(xw.y, 0x01010101) + sdot4(xw.z, 0x01010101) + sdot4(xw.w, 0x01010101)
            i.x += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
            i.y += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
            i.z += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
            i.w += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
        }
        return bs
    }

    // per-block FMA, d/min form: xscl * (d*sc*idot - dmin*mn*bsum)
    def fma_dm(bu, grp, word : uint; xscl : float) : float4 {
        var i = int4(0)
        let bs = dots(bu, grp, word, i)
        let ws = float4(wsw[bu * 32u + word], wsw[bu * 32u + word + 8u],
            wsw[bu * 32u + word + 16u], wsw[bu * 32u + word + 24u])
        let wsb2 = float4(wsw2[bu * 32u + word], wsw2[bu * 32u + word + 8u],
            wsw2[bu * 32u + word + 16u], wsw2[bu * 32u + word + 24u])
        return xscl * (ws * float4(i) - wsb2 * float(bs))
    }

    def store_y(xrow, cnt, row0, wt, word : uint; f : float4) {
        if (xrow < cnt) {
            let yb = (row0 + xrow) * pa.d + wt * 32u + word
            if (wt * 32u + word < pa.d) {
                y[yb] = f.x
            }
            if (wt * 32u + word + 8u < pa.d) {
                y[yb + 8u] = f.y
            }
            if (wt * 32u + word + 16u < pa.d) {
                y[yb + 16u] = f.z
            }
            if (wt * 32u + word + 24u < pa.d) {
                y[yb + 24u] = f.w
            }
        }
    }

    // the variant points — every [spirv_kernel] leaf overrides stage_w; q40/k6 also stage_ws/blk_fma
    def stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        pass
    }

    def stage_ws(wsb0, wt, nsb, s, tid : uint) {
        load_dm(wsb0, wt, nsb, s, tid % 32u, tid / 32u)
    }

    def blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        return fma_dm(bu, grp, word, xscl)
    }

    def tile_shell {
        let reg = region_rec()
        let wsb0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nsb = pa.n / 256u
        let tid = gl_LocalInvocationID.x
        let word = tid % 8u
        let grp = tid / 8u
        let xrow = xt * 32u + grp
        let wcol = wt * 32u + grp
        var f = float4(0.0)
        var s = 0u
        while (s < nsb) {
            load_x(row0, xrow, cnt, nsb, s, word, grp)
            stage_w(wsb0, wcol, nsb, s, word, grp)
            load_xs(row0, xt, cnt, nsb, s, tid)
            stage_ws(wsb0, wt, nsb, s, tid)
            barrier()
            let xscl = xsw[grp]
            for (bb in range(8)) {
                f += blk_fma(uint(bb), grp, word, xscl)
            }
            barrier()
            s++
        }
        store_y(xrow, cnt, row0, wt, word, f)
    }
}

// Q4_K tile: per block acc += xs * (d*sc*idot - dmin*mn*bsum)
[vk_dispatch(name = "kq_batch_k4_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK4 : KqBatchBase {
    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
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
            ww4[grp * 17u + k] = wv
        }
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k4_cls_spv")]
    def run {
        tile_shell()
    }
}

// Q4_0 tile: the k4 staging with one scale plane — per block acc += xs * d * (idot - 8*bsum)
[vk_dispatch(name = "kq_batch_q40_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchQ40 : KqBatchK4 {
    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let dp = unpackHalf2x16(wsu[wsb * 5u + sblk / 2u])
            sa = sblk % 2u == 0u ? dp.x : dp.y
        }
        wsw[sblk * 32u + slane] = sa
    }

    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        var i = int4(0)
        let bs = dots(bu, grp, word, i)
        let ws = float4(wsw[bu * 32u + word], wsw[bu * 32u + word + 8u],
            wsw[bu * 32u + word + 16u], wsw[bu * 32u + word + 24u])
        return xscl * ws * float4(i - int4(8 * bs))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_q40_cls_spv")]
    def override run {
        tile_shell()
    }
}

// IQ4_XS tile: the k4 staging with the codebook decode in stage_w (signed weight lanes, so no
// block-sum term) and one signed-scale plane — per block acc += xs * d*sc * idot
[vk_dispatch(name = "kq_batch_iq4xs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq4xs : KqBatchBase {
    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let sh = (k % 2u) * 4u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let rb4 = wsb * 32u + bu * 4u
                wv = uint4(iq4_word((wq[rb4] >> sh) & 0x0F0F0F0F), iq4_word((wq[rb4 + 1u] >> sh) & 0x0F0F0F0F),
                    iq4_word((wq[rb4 + 2u] >> sh) & 0x0F0F0F0F), iq4_word((wq[rb4 + 3u] >> sh) & 0x0F0F0F0F))
            }
            ww4[grp * 17u + k] = wv
        }
    }

    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            sa = unpackHalf2x16(wsu[wsb * 5u]).x * float(iq4_sc(wsu[wsb * 5u + 1u + sblk / 4u], sblk))
        }
        wsw[sblk * 32u + slane] = sa
    }

    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        var i = int4(0)
        dots(bu, grp, word, i)
        let ws = float4(wsw[bu * 32u + word], wsw[bu * 32u + word + 8u],
            wsw[bu * 32u + word + 16u], wsw[bu * 32u + word + 24u])
        return xscl * ws * float4(i)
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq4xs_cls_spv")]
    def run {
        tile_shell()
    }
}

// IQ3_S tile: the iq4xs staging with the GRID GATHER in stage_w - each staged uint4 is four
// grid words sign-applied (mask = ((nibble * 0x00204081) & 0x01010101) * 255; magnitudes odd
// 1..15, no cross-byte carry); scale row and blk_fma are iq4xs's verbatim (inherited). The 2 KB
// grid stages into workgroup memory once, before the tile loop.
[vk_dispatch(name = "kq_batch_iq3s_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq3s : KqBatchIq4xs {
    @workgroup gridw : uint[512]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let qsw = wq[wsb * 26u + bu * 2u + hh]
                let qh = (wq[wsb * 26u + 16u + bu / 4u] >> ((bu % 4u) * 8u)) & 255u
                let sgw = wq[wsb * 26u + 18u + bu]
                let l0 = hh * 2u
                let sg0 = (sgw >> (l0 * 8u)) & 255u
                let sg1 = (sgw >> (l0 * 8u + 8u)) & 255u
                let i0 = (qsw & 255u) | ((qh << (8u - 2u * l0)) & 256u)
                let i1 = ((qsw >> 8u) & 255u) | ((qh << (7u - 2u * l0)) & 256u)
                let i2 = ((qsw >> 16u) & 255u) | ((qh << (8u - 2u * (l0 + 1u))) & 256u)
                let i3 = (qsw >> 24u) | ((qh << (7u - 2u * (l0 + 1u))) & 256u)
                wv = uint4(iq3s_signed(gridw[i0], sg0 & 15u), iq3s_signed(gridw[i1], sg0 >> 4u),
                    iq3s_signed(gridw[i2], sg1 & 15u), iq3s_signed(gridw[i3], sg1 >> 4u))
            }
            ww4[grp * 17u + k] = wv
        }
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq3s_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        gridw[tid] = iq3s_grid_word(int(tid))
        gridw[tid + 256u] = iq3s_grid_word(int(tid + 256u))
        barrier()   // the grid visible before the first stage_w
        tile_shell()
    }
}

// IQ3_XXS tile: the iq3s staging with the halved-grid gather - bare 8-bit indices (no ninth
// bit), signs from the aux32's four 7-bit ksigns indices via the parity helper (no table);
// scale row and blk_fma are iq4xs's verbatim (inherited). The 1 KB grid stages once.
[vk_dispatch(name = "kq_batch_iq3xxs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq3xxs : KqBatchIq4xs {
    @workgroup gridw : uint[256]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let qsw = wq[wsb * 24u + bu * 2u + hh]
                let aux = wq[wsb * 24u + 16u + bu]
                let l0 = hh * 2u
                let sg0 = ksign7((aux >> (7u * l0)) & 127u)
                let sg1 = ksign7((aux >> (7u * l0 + 7u)) & 127u)
                wv = uint4(iq3s_signed(gridw[qsw & 255u], sg0 & 15u), iq3s_signed(gridw[(qsw >> 8u) & 255u], sg0 >> 4u),
                    iq3s_signed(gridw[(qsw >> 16u) & 255u], sg1 & 15u), iq3s_signed(gridw[qsw >> 24u], sg1 >> 4u))
            }
            ww4[grp * 17u + k] = wv
        }
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq3xxs_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        gridw[tid] = iq3xxs_grid_word(int(tid))
        barrier()   // the grid visible before the first stage_w
        tile_shell()
    }
}

// IQ2_XXS tile: the iq3xxs shell over the two-word u64 grid - block b's grid word (column 2b)
// carries four BYTE indices, its aux32 (column 2b+1) the parity-derived ksigns; the per-32
// strip plane and fma inherit from KqBatchIq4xs (the (2ls+1) strips read exactly under the
// signed extract, staying below 128).
[vk_dispatch(name = "kq_batch_iq2xxs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2xxs : KqBatchIq4xs {
    @workgroup gridw : uint[512]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let gw = wq[wsb * 16u + bu * 2u]
                let aux = wq[wsb * 16u + bu * 2u + 1u]
                let b0 = (gw >> (16u * hh)) & 255u
                let b1 = (gw >> (16u * hh + 8u)) & 255u
                let sg0 = ksign7((aux >> (14u * hh)) & 127u)
                let sg1 = ksign7((aux >> (14u * hh + 7u)) & 127u)
                wv = uint4(iq3s_signed(gridw[b0 * 2u], sg0 & 15u), iq3s_signed(gridw[b0 * 2u + 1u], sg0 >> 4u),
                    iq3s_signed(gridw[b1 * 2u], sg1 & 15u), iq3s_signed(gridw[b1 * 2u + 1u], sg1 >> 4u))
            }
            ww4[grp * 17u + k] = wv
        }
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq2xxs_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        gridw[tid] = iq2xxs_grid_word(int(tid))
        gridw[tid + 256u] = iq2xxs_grid_word(int(tid + 256u))
        barrier()   // the iq2xxs grid visible before the first stage_w
        tile_shell()
    }
}

// IQ4_NL tile: iq4xs's LUT staging (stage_w and the no-min blk_fma inherit) with q40's
// per-block f16 d staged as the scale - no sub-scales, no bsum term anywhere.
[vk_dispatch(name = "kq_batch_iq4nl_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq4nl : KqBatchIq4xs {
    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let dp = unpackHalf2x16(wsu[wsb * 5u + sblk / 2u])
            sa = sblk % 2u == 0u ? dp.x : dp.y
        }
        wsw[sblk * 32u + slane] = sa
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq4nl_cls_spv")]
    def override run {
        tile_shell()
    }
}

// Q5_K tile: the k4 compose with the 5th bit OR'd in from the 40B block's qh words
[vk_dispatch(name = "kq_batch_k5_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK5 : KqBatchBase {
    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let rb4 = wsb * 40u + bu * 4u
                let r0 = wq[rb4]
                let r1 = wq[rb4 + 1u]
                let r2 = wq[rb4 + 2u]
                let r3 = wq[rb4 + 3u]
                let hbw = wq[wsb * 40u + 32u + bu]
                let h0 = hbw & 0xFF
                let h1 = (hbw >> 8u) & 0xFF
                let h2 = (hbw >> 16u) & 0xFF
                let h3 = (hbw >> 24u) & 0xFF
                if (hh == 0u) {
                    wv = uint4((r0 & 0x0F0F0F0F) | k5_dep(h0 & 0xF), (r1 & 0x0F0F0F0F) | k5_dep(h1 & 0xF),
                        (r2 & 0x0F0F0F0F) | k5_dep(h2 & 0xF), (r3 & 0x0F0F0F0F) | k5_dep(h3 & 0xF))
                } else {
                    wv = uint4(((r0 >> 4u) & 0x0F0F0F0F) | k5_dep(h0 >> 4u), ((r1 >> 4u) & 0x0F0F0F0F) | k5_dep(h1 >> 4u),
                        ((r2 >> 4u) & 0x0F0F0F0F) | k5_dep(h2 >> 4u), ((r3 >> 4u) & 0x0F0F0F0F) | k5_dep(h3 >> 4u))
                }
            }
            ww4[grp * 17u + k] = wv
        }
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k5_cls_spv")]
    def run {
        tile_shell()
    }
}

// Q6_K tile: composed 6-bit quants, the -32 folded per 16-half as slo/shi vs the half's block
// sum — the lo/hi word split IS the sub-scale split
[vk_dispatch(name = "kq_batch_k6_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK6 : KqBatchBase {
    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let rb4 = wsb * 48u + bu * 4u
                let r0 = wq[rb4]
                let r1 = wq[rb4 + 1u]
                let r2 = wq[rb4 + 2u]
                let r3 = wq[rb4 + 3u]
                let qb4 = wsb * 48u + 32u + (bu / 4u) * 8u + hh * 4u
                let q0 = wq[qb4]
                let q1 = wq[qb4 + 1u]
                let q2 = wq[qb4 + 2u]
                let q3 = wq[qb4 + 3u]
                let shift = (bu % 4u) * 2u
                if (hh == 0u) {
                    wv = uint4((r0 & 0x0F0F0F0F) | (((q0 >> shift) & 0x03030303) << 4u),
                        (r1 & 0x0F0F0F0F) | (((q1 >> shift) & 0x03030303) << 4u),
                        (r2 & 0x0F0F0F0F) | (((q2 >> shift) & 0x03030303) << 4u),
                        (r3 & 0x0F0F0F0F) | (((q3 >> shift) & 0x03030303) << 4u))
                } else {
                    wv = uint4(((r0 >> 4u) & 0x0F0F0F0F) | (((q0 >> shift) & 0x03030303) << 4u),
                        ((r1 >> 4u) & 0x0F0F0F0F) | (((q1 >> shift) & 0x03030303) << 4u),
                        ((r2 >> 4u) & 0x0F0F0F0F) | (((q2 >> shift) & 0x03030303) << 4u),
                        ((r3 >> 4u) & 0x0F0F0F0F) | (((q3 >> shift) & 0x03030303) << 4u))
                }
            }
            ww4[grp * 17u + k] = wv
        }
    }

    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        var sbv = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let s4 = int4(unpack8(int(wsu[wsb * 5u + sblk / 2u])))
            let slo = (sblk % 2u) == 0u ? s4.x : s4.z
            let shi = (sblk % 2u) == 0u ? s4.y : s4.w
            let dd = unpackHalf2x16(wsu[wsb * 5u + 4u]).x
            sa = dd * float(slo)
            sbv = dd * float(shi)
        }
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
    }

    // the split-half FMA: each 16-weight half keeps its own dot, sum, and signed sub-scale
    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        let kL = bu * 2u
        let kH = bu * 2u + 1u
        let xwL = xw4[grp * 17u + kL]
        let xwH = xw4[grp * 17u + kH]
        let blo = sdot4(xwL.x, 0x01010101) + sdot4(xwL.y, 0x01010101) + sdot4(xwL.z, 0x01010101) + sdot4(xwL.w, 0x01010101)
        let bhi = sdot4(xwH.x, 0x01010101) + sdot4(xwH.y, 0x01010101) + sdot4(xwH.z, 0x01010101) + sdot4(xwH.w, 0x01010101)
        let wL0 = ww4[word * 17u + kL]
        let wL1 = ww4[(word + 8u) * 17u + kL]
        let wL2 = ww4[(word + 16u) * 17u + kL]
        let wL3 = ww4[(word + 24u) * 17u + kL]
        let wH0 = ww4[word * 17u + kH]
        let wH1 = ww4[(word + 8u) * 17u + kH]
        let wH2 = ww4[(word + 16u) * 17u + kH]
        let wH3 = ww4[(word + 24u) * 17u + kH]
        let l0 = sdot4(wL0.x, xwL.x) + sdot4(wL0.y, xwL.y) + sdot4(wL0.z, xwL.z) + sdot4(wL0.w, xwL.w)
        let l1 = sdot4(wL1.x, xwL.x) + sdot4(wL1.y, xwL.y) + sdot4(wL1.z, xwL.z) + sdot4(wL1.w, xwL.w)
        let l2 = sdot4(wL2.x, xwL.x) + sdot4(wL2.y, xwL.y) + sdot4(wL2.z, xwL.z) + sdot4(wL2.w, xwL.w)
        let l3 = sdot4(wL3.x, xwL.x) + sdot4(wL3.y, xwL.y) + sdot4(wL3.z, xwL.z) + sdot4(wL3.w, xwL.w)
        let h0 = sdot4(wH0.x, xwH.x) + sdot4(wH0.y, xwH.y) + sdot4(wH0.z, xwH.z) + sdot4(wH0.w, xwH.w)
        let h1 = sdot4(wH1.x, xwH.x) + sdot4(wH1.y, xwH.y) + sdot4(wH1.z, xwH.z) + sdot4(wH1.w, xwH.w)
        let h2 = sdot4(wH2.x, xwH.x) + sdot4(wH2.y, xwH.y) + sdot4(wH2.z, xwH.z) + sdot4(wH2.w, xwH.w)
        let h3 = sdot4(wH3.x, xwH.x) + sdot4(wH3.y, xwH.y) + sdot4(wH3.z, xwH.z) + sdot4(wH3.w, xwH.w)
        return float4(xscl * (wsw[bu * 32u + word] * float(l0 - 32 * blo) + wsw2[bu * 32u + word] * float(h0 - 32 * bhi)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l1 - 32 * blo) + wsw2[bu * 32u + word + 8u] * float(h1 - 32 * bhi)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l2 - 32 * blo) + wsw2[bu * 32u + word + 16u] * float(h2 - 32 * bhi)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l3 - 32 * blo) + wsw2[bu * 32u + word + 24u] * float(h3 - 32 * bhi)))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k6_cls_spv")]
    def run {
        tile_shell()
    }
}

// Q3_K tile: k6's split-half fold with the offset 4 over the k3 planes (24 words per superblock:
// 16 qs words, 8 hmask words) — block bu takes qs words (bu/4)*8 + hh*4.. at shift 2*(bu%4)
// OR the hmask words 16 + hh*4.. bit bu, shifted up two
[vk_dispatch(name = "kq_batch_k3_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK3 : KqBatchK6 {
    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let qb4 = wsb * 24u + (bu / 4u) * 8u + hh * 4u
                let hb4 = wsb * 24u + 16u + hh * 4u
                let shift = (bu % 4u) * 2u
                wv = uint4(((wq[qb4] >> shift) & 0x03030303) | (((wq[hb4] >> bu) & 0x01010101) << 2u),
                    ((wq[qb4 + 1u] >> shift) & 0x03030303) | (((wq[hb4 + 1u] >> bu) & 0x01010101) << 2u),
                    ((wq[qb4 + 2u] >> shift) & 0x03030303) | (((wq[hb4 + 2u] >> bu) & 0x01010101) << 2u),
                    ((wq[qb4 + 3u] >> shift) & 0x03030303) | (((wq[hb4 + 3u] >> bu) & 0x01010101) << 2u))
            }
            ww4[grp * 17u + k] = wv
        }
    }

    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        let kL = bu * 2u
        let kH = bu * 2u + 1u
        let xwL = xw4[grp * 17u + kL]
        let xwH = xw4[grp * 17u + kH]
        let blo = sdot4(xwL.x, 0x01010101) + sdot4(xwL.y, 0x01010101) + sdot4(xwL.z, 0x01010101) + sdot4(xwL.w, 0x01010101)
        let bhi = sdot4(xwH.x, 0x01010101) + sdot4(xwH.y, 0x01010101) + sdot4(xwH.z, 0x01010101) + sdot4(xwH.w, 0x01010101)
        let wL0 = ww4[word * 17u + kL]
        let wL1 = ww4[(word + 8u) * 17u + kL]
        let wL2 = ww4[(word + 16u) * 17u + kL]
        let wL3 = ww4[(word + 24u) * 17u + kL]
        let wH0 = ww4[word * 17u + kH]
        let wH1 = ww4[(word + 8u) * 17u + kH]
        let wH2 = ww4[(word + 16u) * 17u + kH]
        let wH3 = ww4[(word + 24u) * 17u + kH]
        let l0 = sdot4(wL0.x, xwL.x) + sdot4(wL0.y, xwL.y) + sdot4(wL0.z, xwL.z) + sdot4(wL0.w, xwL.w)
        let l1 = sdot4(wL1.x, xwL.x) + sdot4(wL1.y, xwL.y) + sdot4(wL1.z, xwL.z) + sdot4(wL1.w, xwL.w)
        let l2 = sdot4(wL2.x, xwL.x) + sdot4(wL2.y, xwL.y) + sdot4(wL2.z, xwL.z) + sdot4(wL2.w, xwL.w)
        let l3 = sdot4(wL3.x, xwL.x) + sdot4(wL3.y, xwL.y) + sdot4(wL3.z, xwL.z) + sdot4(wL3.w, xwL.w)
        let h0 = sdot4(wH0.x, xwH.x) + sdot4(wH0.y, xwH.y) + sdot4(wH0.z, xwH.z) + sdot4(wH0.w, xwH.w)
        let h1 = sdot4(wH1.x, xwH.x) + sdot4(wH1.y, xwH.y) + sdot4(wH1.z, xwH.z) + sdot4(wH1.w, xwH.w)
        let h2 = sdot4(wH2.x, xwH.x) + sdot4(wH2.y, xwH.y) + sdot4(wH2.z, xwH.z) + sdot4(wH2.w, xwH.w)
        let h3 = sdot4(wH3.x, xwH.x) + sdot4(wH3.y, xwH.y) + sdot4(wH3.z, xwH.z) + sdot4(wH3.w, xwH.w)
        return float4(xscl * (wsw[bu * 32u + word] * float(l0 - 4 * blo) + wsw2[bu * 32u + word] * float(h0 - 4 * bhi)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l1 - 4 * blo) + wsw2[bu * 32u + word + 8u] * float(h1 - 4 * bhi)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l2 - 4 * blo) + wsw2[bu * 32u + word + 16u] * float(h2 - 4 * bhi)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l3 - 4 * blo) + wsw2[bu * 32u + word + 24u] * float(h3 - 4 * bhi)))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k3_cls_spv")]
    def override run {
        tile_shell()
    }
}

// Q2_K tile: k3's staging minus the hmask (unsigned 0..3 lanes), FOUR scale planes - d*sc and
// dmin*mn per 16-half - and a split-half fma folding the min sides against the half sums.
[vk_dispatch(name = "kq_batch_k2_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK2 : KqBatchK6 {
    @workgroup wsw3 : float[256]   // dmin*mn, lo half
    @workgroup wsw4 : float[256]   // dmin*mn, hi half

    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let qb4 = wsb * 16u + (bu / 4u) * 8u + hh * 4u
                let shift = (bu % 4u) * 2u
                wv = uint4((wq[qb4] >> shift) & 0x03030303, (wq[qb4 + 1u] >> shift) & 0x03030303,
                    (wq[qb4 + 2u] >> shift) & 0x03030303, (wq[qb4 + 3u] >> shift) & 0x03030303)
            }
            ww4[grp * 17u + k] = wv
        }
    }

    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        var sbv = 0.0
        var ma = 0.0
        var mb = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let w = wsu[wsb * 5u + 1u + sblk / 2u]
            let b0 = (w >> ((sblk % 2u) * 16u)) & 255u
            let b1 = (w >> ((sblk % 2u) * 16u + 8u)) & 255u
            let dm = unpackHalf2x16(wsu[wsb * 5u])
            sa = dm.x * float(b0 & 15u)
            sbv = dm.x * float(b1 & 15u)
            ma = dm.y * float(b0 >> 4u)
            mb = dm.y * float(b1 >> 4u)
        }
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
        wsw3[sblk * 32u + slane] = ma
        wsw4[sblk * 32u + slane] = mb
    }

    // the split-half fma with the min planes: xscl * (sc_lo*l + sc_hi*h - mn_lo*blo - mn_hi*bhi)
    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        let kL = bu * 2u
        let kH = bu * 2u + 1u
        let xwL = xw4[grp * 17u + kL]
        let xwH = xw4[grp * 17u + kH]
        let blo = sdot4(xwL.x, 0x01010101) + sdot4(xwL.y, 0x01010101) + sdot4(xwL.z, 0x01010101) + sdot4(xwL.w, 0x01010101)
        let bhi = sdot4(xwH.x, 0x01010101) + sdot4(xwH.y, 0x01010101) + sdot4(xwH.z, 0x01010101) + sdot4(xwH.w, 0x01010101)
        let wL0 = ww4[word * 17u + kL]
        let wL1 = ww4[(word + 8u) * 17u + kL]
        let wL2 = ww4[(word + 16u) * 17u + kL]
        let wL3 = ww4[(word + 24u) * 17u + kL]
        let wH0 = ww4[word * 17u + kH]
        let wH1 = ww4[(word + 8u) * 17u + kH]
        let wH2 = ww4[(word + 16u) * 17u + kH]
        let wH3 = ww4[(word + 24u) * 17u + kH]
        let l0 = sdot4(wL0.x, xwL.x) + sdot4(wL0.y, xwL.y) + sdot4(wL0.z, xwL.z) + sdot4(wL0.w, xwL.w)
        let l1 = sdot4(wL1.x, xwL.x) + sdot4(wL1.y, xwL.y) + sdot4(wL1.z, xwL.z) + sdot4(wL1.w, xwL.w)
        let l2 = sdot4(wL2.x, xwL.x) + sdot4(wL2.y, xwL.y) + sdot4(wL2.z, xwL.z) + sdot4(wL2.w, xwL.w)
        let l3 = sdot4(wL3.x, xwL.x) + sdot4(wL3.y, xwL.y) + sdot4(wL3.z, xwL.z) + sdot4(wL3.w, xwL.w)
        let h0 = sdot4(wH0.x, xwH.x) + sdot4(wH0.y, xwH.y) + sdot4(wH0.z, xwH.z) + sdot4(wH0.w, xwH.w)
        let h1 = sdot4(wH1.x, xwH.x) + sdot4(wH1.y, xwH.y) + sdot4(wH1.z, xwH.z) + sdot4(wH1.w, xwH.w)
        let h2 = sdot4(wH2.x, xwH.x) + sdot4(wH2.y, xwH.y) + sdot4(wH2.z, xwH.z) + sdot4(wH2.w, xwH.w)
        let h3 = sdot4(wH3.x, xwH.x) + sdot4(wH3.y, xwH.y) + sdot4(wH3.z, xwH.z) + sdot4(wH3.w, xwH.w)
        return float4(xscl * (wsw[bu * 32u + word] * float(l0) + wsw2[bu * 32u + word] * float(h0) - wsw3[bu * 32u + word] * float(blo) - wsw4[bu * 32u + word] * float(bhi)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l1) + wsw2[bu * 32u + word + 8u] * float(h1) - wsw3[bu * 32u + word + 8u] * float(blo) - wsw4[bu * 32u + word + 8u] * float(bhi)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l2) + wsw2[bu * 32u + word + 16u] * float(h2) - wsw3[bu * 32u + word + 16u] * float(blo) - wsw4[bu * 32u + word + 16u] * float(bhi)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l3) + wsw2[bu * 32u + word + 24u] * float(h3) - wsw3[bu * 32u + word + 24u] * float(blo) - wsw4[bu * 32u + word + 24u] * float(bhi)))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k2_cls_spv")]
    def override run {
        tile_shell()
    }
}

// IQ2_S tile: the iq3s grid gather widened to the u64 grid (a 10-bit qs|qh index picks TWO
// words - eight magnitudes) with the block's own sign bytes; the per-16 UNSIGNED strips ride
// k6's wsw/wsw2 planes with a plain split fma (no offset, no min term). The 8 KB grid stages
// once, 8 words per thread.
[vk_dispatch(name = "kq_batch_iq2s_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2s : KqBatchK6 {
    @workgroup gridw : uint[2048]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let qsw = wq[wsb * 18u + bu]
                let sgw = wq[wsb * 18u + 8u + bu]
                let qh = (wq[wsb * 18u + 16u + bu / 4u] >> ((bu % 4u) * 8u)) & 255u
                let l0 = hh * 2u
                let sg0 = (sgw >> (l0 * 8u)) & 255u
                let sg1 = (sgw >> (l0 * 8u + 8u)) & 255u
                let i0 = (((qsw >> (l0 * 8u)) & 255u) | ((qh << (8u - 2u * l0)) & 0x300u)) * 2u
                let i1 = (((qsw >> (l0 * 8u + 8u)) & 255u) | ((qh << (8u - 2u * (l0 + 1u))) & 0x300u)) * 2u
                wv = uint4(iq3s_signed(gridw[i0], sg0 & 15u), iq3s_signed(gridw[i0 + 1u], sg0 >> 4u),
                    iq3s_signed(gridw[i1], sg1 & 15u), iq3s_signed(gridw[i1 + 1u], sg1 >> 4u))
            }
            ww4[grp * 17u + k] = wv
        }
    }

    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        var sbv = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let w = wsu[wsb * 5u + 1u + sblk / 2u]
            let s0 = (w >> ((sblk % 2u) * 16u)) & 255u
            let s1 = (w >> ((sblk % 2u) * 16u + 8u)) & 255u
            let dd = unpackHalf2x16(wsu[wsb * 5u]).x
            sa = dd * float(s0)
            sbv = dd * float(s1)
        }
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
    }

    // the split-half fma with UNSIGNED strips: xscl * (d*s0*l + d*s1*h) - no offset, no min
    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        let kL = bu * 2u
        let kH = bu * 2u + 1u
        let xwL = xw4[grp * 17u + kL]
        let xwH = xw4[grp * 17u + kH]
        let wL0 = ww4[word * 17u + kL]
        let wL1 = ww4[(word + 8u) * 17u + kL]
        let wL2 = ww4[(word + 16u) * 17u + kL]
        let wL3 = ww4[(word + 24u) * 17u + kL]
        let wH0 = ww4[word * 17u + kH]
        let wH1 = ww4[(word + 8u) * 17u + kH]
        let wH2 = ww4[(word + 16u) * 17u + kH]
        let wH3 = ww4[(word + 24u) * 17u + kH]
        let l0 = sdot4(wL0.x, xwL.x) + sdot4(wL0.y, xwL.y) + sdot4(wL0.z, xwL.z) + sdot4(wL0.w, xwL.w)
        let l1 = sdot4(wL1.x, xwL.x) + sdot4(wL1.y, xwL.y) + sdot4(wL1.z, xwL.z) + sdot4(wL1.w, xwL.w)
        let l2 = sdot4(wL2.x, xwL.x) + sdot4(wL2.y, xwL.y) + sdot4(wL2.z, xwL.z) + sdot4(wL2.w, xwL.w)
        let l3 = sdot4(wL3.x, xwL.x) + sdot4(wL3.y, xwL.y) + sdot4(wL3.z, xwL.z) + sdot4(wL3.w, xwL.w)
        let h0 = sdot4(wH0.x, xwH.x) + sdot4(wH0.y, xwH.y) + sdot4(wH0.z, xwH.z) + sdot4(wH0.w, xwH.w)
        let h1 = sdot4(wH1.x, xwH.x) + sdot4(wH1.y, xwH.y) + sdot4(wH1.z, xwH.z) + sdot4(wH1.w, xwH.w)
        let h2 = sdot4(wH2.x, xwH.x) + sdot4(wH2.y, xwH.y) + sdot4(wH2.z, xwH.z) + sdot4(wH2.w, xwH.w)
        let h3 = sdot4(wH3.x, xwH.x) + sdot4(wH3.y, xwH.y) + sdot4(wH3.z, xwH.z) + sdot4(wH3.w, xwH.w)
        return float4(xscl * (wsw[bu * 32u + word] * float(l0) + wsw2[bu * 32u + word] * float(h0)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l1) + wsw2[bu * 32u + word + 8u] * float(h1)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l2) + wsw2[bu * 32u + word + 16u] * float(h2)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l3) + wsw2[bu * 32u + word + 24u] * float(h3)))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq2s_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        for (k in range(8)) {
            gridw[tid + uint(k) * 256u] = iq2s_grid_word(int(tid + uint(k) * 256u))
        }
        barrier()   // the grid visible before the first stage_w
        tile_shell()
    }
}

// IQ2_XS tile: the iq2s gather with ksigns-by-parity - each u16 qs word carries a 9-bit grid
// index (two staged words) and a 7-bit KSIGNS_IQ2XS index whose sign byte ksign7 recomputes
// (no sign plane, no qh); strips and fma ride the iq2s spellings. The 4 KB grid stages once.
[vk_dispatch(name = "kq_batch_iq2xs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2xs : KqBatchK6 {
    @workgroup gridw : uint[1024]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                let wsb = wsb0 + wcol * nsb + s
                let qw = wq[wsb * 16u + k]
                let wa = qw & 0xFFFFu
                let wb = qw >> 16u
                let sg0 = ksign7(wa >> 9u)
                let sg1 = ksign7(wb >> 9u)
                let i0 = (wa & 511u) * 2u
                let i1 = (wb & 511u) * 2u
                wv = uint4(iq3s_signed(gridw[i0], sg0 & 15u), iq3s_signed(gridw[i0 + 1u], sg0 >> 4u),
                    iq3s_signed(gridw[i1], sg1 & 15u), iq3s_signed(gridw[i1 + 1u], sg1 >> 4u))
            }
            ww4[grp * 17u + k] = wv
        }
    }

    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        var sbv = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let w = wsu[wsb * 5u + 1u + sblk / 2u]
            let s0 = (w >> ((sblk % 2u) * 16u)) & 255u
            let s1 = (w >> ((sblk % 2u) * 16u + 8u)) & 255u
            let dd = unpackHalf2x16(wsu[wsb * 5u]).x
            sa = dd * float(s0)
            sbv = dd * float(s1)
        }
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
    }

    // the split-half fma with UNSIGNED strips: xscl * (d*s0*l + d*s1*h) - no offset, no min
    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        let kL = bu * 2u
        let kH = bu * 2u + 1u
        let xwL = xw4[grp * 17u + kL]
        let xwH = xw4[grp * 17u + kH]
        let wL0 = ww4[word * 17u + kL]
        let wL1 = ww4[(word + 8u) * 17u + kL]
        let wL2 = ww4[(word + 16u) * 17u + kL]
        let wL3 = ww4[(word + 24u) * 17u + kL]
        let wH0 = ww4[word * 17u + kH]
        let wH1 = ww4[(word + 8u) * 17u + kH]
        let wH2 = ww4[(word + 16u) * 17u + kH]
        let wH3 = ww4[(word + 24u) * 17u + kH]
        let l0 = sdot4(wL0.x, xwL.x) + sdot4(wL0.y, xwL.y) + sdot4(wL0.z, xwL.z) + sdot4(wL0.w, xwL.w)
        let l1 = sdot4(wL1.x, xwL.x) + sdot4(wL1.y, xwL.y) + sdot4(wL1.z, xwL.z) + sdot4(wL1.w, xwL.w)
        let l2 = sdot4(wL2.x, xwL.x) + sdot4(wL2.y, xwL.y) + sdot4(wL2.z, xwL.z) + sdot4(wL2.w, xwL.w)
        let l3 = sdot4(wL3.x, xwL.x) + sdot4(wL3.y, xwL.y) + sdot4(wL3.z, xwL.z) + sdot4(wL3.w, xwL.w)
        let h0 = sdot4(wH0.x, xwH.x) + sdot4(wH0.y, xwH.y) + sdot4(wH0.z, xwH.z) + sdot4(wH0.w, xwH.w)
        let h1 = sdot4(wH1.x, xwH.x) + sdot4(wH1.y, xwH.y) + sdot4(wH1.z, xwH.z) + sdot4(wH1.w, xwH.w)
        let h2 = sdot4(wH2.x, xwH.x) + sdot4(wH2.y, xwH.y) + sdot4(wH2.z, xwH.z) + sdot4(wH2.w, xwH.w)
        let h3 = sdot4(wH3.x, xwH.x) + sdot4(wH3.y, xwH.y) + sdot4(wH3.z, xwH.z) + sdot4(wH3.w, xwH.w)
        return float4(xscl * (wsw[bu * 32u + word] * float(l0) + wsw2[bu * 32u + word] * float(h0)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l1) + wsw2[bu * 32u + word + 8u] * float(h1)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l2) + wsw2[bu * 32u + word + 16u] * float(h2)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l3) + wsw2[bu * 32u + word + 24u] * float(h3)))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq2xs_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        for (k in range(4)) {
            gridw[tid + uint(k) * 256u] = iq2xs_grid_word(int(tid + uint(k) * 256u))
        }
        barrier()   // the iq2xs grid visible before the first stage_w
        tile_shell()
    }
}

// ===== the prefill batch GEMMs — cooperative-matrix variants (DASLLAMA_COOPMAT=f16|int8|mm) =====

let private CML_SST = 20u   // mm shared row stride in uints (16 f16-pair data + 4 pad — bank spread)

// the MoE region rail the coopmat batch tiles share: sched = 4-word region records
// [wblk0, row0, cnt, wg0] at 0, then the per-wg map at pa.map_off
class MoeCmBase {
    @ssbo @binding = 2 sched : array<uint>   // region records + per-wg map (bulk schedule data)
    @push_constant pa : BatchArgs

    def region_rec : uint4 {
        let rid = sched[pa.map_off + gl_WorkGroupID.x]
        let rb = rid * 4u
        return uint4(sched[rb], sched[rb + 1u], sched[rb + 2u], gl_WorkGroupID.x - sched[rb + 3u])
    }
}

// f16 tensor-core 32x32 tile: same MoE routing + Q8_0 layout as Q8Batch, but the inner GEMM rides
// the tensor cores — 128 threads / 4 subgroups, one 16x16 subtile each; edge tiles zero-fill
[vk_dispatch(name = "q8_batch_cmf16_cls", grid = "wgs", params = "wgs : int64")]
class CmF16Batch : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @workgroup cm_as : float16[8192]   // 32 local rows x 256 k, row-major stride 256 (f16 dequant A)
    @workgroup cm_bs : float16[8192]   // 256 k x 32 cols, row-major stride 32 (f16 dequant B)
    @workgroup cm_out : float[1024]    // 4 subgroups x 16x16 f32 scratch (bounds-checked write-out)

    [spirv_kernel(local_size_x = 128, name = "q8_batch_cmf16_cls_spv")]
    def run {   // nolint:STYLE038 — the verbatim tile pass, parity-locked to the shipped kernel
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nbb = pa.n / 32u
        let nsteps = (nbb + 7u) / 8u
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID          // 0..3 (subgroupSize 32)
        let row_half = sg / 2u               // which 16-row half of the tile
        let col_half = sg % 2u               // which 16-col half of the tile
        var acc : coopmatAcc_f32_16x16
        var s = 0u
        while (s < nsteps) {
            let b0 = s * 8u
            for (i in range(16)) {       // stage A: 32x64 = 2048 uints, 16/thread; OOR -> 0
                let u = tid * 16u + uint(i)
                let row = u / 64u          // 0..31 local token row
                let chunk = u % 64u
                let blk = chunk / 8u
                let w = chunk % 8u
                let xrow = xt * 32u + row
                let gblk = b0 + blk
                var packed = 0u
                var xs = 0.0
                if (xrow < cnt && gblk < nbb) {
                    packed = xqw[((row0 + xrow) * nbb + gblk) * 8u + w]
                    xs = axs[(row0 + xrow) * nbb + gblk]
                }
                let qv = int4(unpack8(int(packed)))
                let o = row * 256u + blk * 32u + w * 4u
                cm_as[o] = float16(float(qv.x) * xs)
                cm_as[o + 1u] = float16(float(qv.y) * xs)
                cm_as[o + 2u] = float16(float(qv.z) * xs)
                cm_as[o + 3u] = float16(float(qv.w) * xs)
            }
            for (i in range(16)) {       // stage B: 32x64 = 2048 uints, 16/thread; OOR -> 0
                let u = tid * 16u + uint(i)
                let col = u / 64u          // 0..31 output col
                let chunk = u % 64u
                let blk = chunk / 8u
                let w = chunk % 8u
                let wcol = wt * 32u + col
                let gblk = b0 + blk
                var packed = 0u
                var ws = 0.0
                if (wcol < pa.d && gblk < nbb) {
                    packed = wq[(wblk0 + wcol * nbb + gblk) * 8u + w]
                    ws = float(wsh[wblk0 + wcol * nbb + gblk])
                }
                let qv = int4(unpack8(int(packed)))
                let k = blk * 32u + w * 4u
                cm_bs[k * 32u + col] = float16(float(qv.x) * ws)
                cm_bs[(k + 1u) * 32u + col] = float16(float(qv.y) * ws)
                cm_bs[(k + 2u) * 32u + col] = float16(float(qv.z) * ws)
                cm_bs[(k + 3u) * 32u + col] = float16(float(qv.w) * ws)
            }
            barrier()
            for (bb in range(8)) {       // 8 blocks x 2 K-subtiles = 16 MMAs into one acc
                for (ks in range(2)) {
                    var a : coopmatA_f16_16x16
                    var b : coopmatB_f16_16x16
                    coopmatLoad(a, cm_as, int(row_half * 16u * 256u + uint(bb) * 32u + uint(ks) * 16u), 256, 0)
                    coopmatLoad(b, cm_bs, int((uint(bb) * 32u + uint(ks) * 16u) * 32u + col_half * 16u), 32, 0)
                    acc = coopmatMulAdd(a, b, acc)
                }
            }
            barrier()
            s++
        }
        coopmatStore(acc, cm_out, int(sg * 256u), 16, 0)   // scratch, then bounds-checked write-out
        barrier()
        let lane = gl_SubgroupInvocationID
        for (e in range(8)) {
            let idx = lane * 8u + uint(e)
            let m = idx / 16u
            let nn = idx % 16u
            let orow = xt * 32u + row_half * 16u + m
            let ocol = wt * 32u + col_half * 16u + nn
            if (orow < cnt && ocol < pa.d) {
                y[(row0 + orow) * pa.d + ocol] = cm_out[sg * 256u + idx]
            }
        }
    }
}

// int8 path: the tensor cores eat s8 quants directly — K-tile 32 = one Q8_0 block, one MMA per
// block, then the s32 tile spills to shared for the SIMT xs[m]*ws[n] scale
[vk_dispatch(name = "q8_batch_cmi8_cls", grid = "wgs", params = "wgs : int64")]
class CmI8Batch : MoeCmBase {
    @ssbo @binding = 0 wq8 : array<int8>       // int8 view of the weight quant bytes
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xq8 : array<int8>       // int8 view of the activation quant bytes
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @workgroup cmi_s32 : int[1024]     // 4 subgroups x 16x16 s32 spill
    @workgroup cmi_acc : float[1024]   // 4 subgroups x 16x16 f32 accumulator

    [spirv_kernel(local_size_x = 128, name = "q8_batch_cmi8_cls_spv")]
    def run {
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nbb = pa.n / 32u
        let sg = gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let row_half = sg / 2u
        let col_half = sg % 2u
        let arow = xt * 32u + row_half * 16u        // this subgroup's local token-row base
        let bcol = wt * 32u + col_half * 16u        // this subgroup's output-col base
        for (e in range(8)) {
            cmi_acc[sg * 256u + lane * 8u + uint(e)] = 0.0
        }
        var blk = 0u
        while (blk < nbb) {
            var a : coopmatA_s8_16x32
            var b : coopmatB_s8_32x16
            coopmatLoad(a, xq8, ((row0 + arow) * nbb + blk) * 32u, int(pa.n), 0)      // A 16 rows, rowmajor
            coopmatLoad(b, wq8, (wblk0 + bcol * nbb + blk) * 32u, int(pa.n), 1)       // B 16 cols, colmajor
            var s32 : coopmatAcc_s32_16x16
            s32 = coopmatMulAdd(a, b, s32)   // nolint:STYLE011 — MulAdd seeds from its own zero-init tile
            coopmatStore(s32, cmi_s32, int(sg * 256u), 16, 0)
            barrier()
            for (e in range(8)) {              // SIMT: acc += s32 * xs[m] * ws[n]
                let idx = lane * 8u + uint(e)
                let m = idx / 16u
                let nn = idx % 16u
                let xs = axs[(row0 + arow + m) * nbb + blk]
                let ws = float(wsh[wblk0 + (bcol + nn) * nbb + blk])
                // i*ws*xs, matching Q8Batch's multiply association — bit-parity with the sdot4 kernel
                cmi_acc[sg * 256u + idx] += float(cmi_s32[sg * 256u + idx]) * ws * xs
            }
            barrier()
            blk++
        }
        for (e in range(8)) {                  // bounds-checked write-out (edge tiles)
            let idx = lane * 8u + uint(e)
            let m = idx / 16u
            let nn = idx % 16u
            let orow = arow + m
            let ocol = bcol + nn
            if (orow < cnt && ocol < pa.d) {
                y[(row0 + orow) * pa.d + ocol] = cmi_acc[sg * 256u + idx]
            }
        }
    }
}

// the mul_mm L-tile variant (DASLLAMA_COOPMAT=mm): the aligned-L geometry — BM=BN=128
// BK=32, 4 warps 2x2 with 16 f16acc fragments each. Edge tiles bounce stores via cml_o
[vk_dispatch(name = "q8_batch_mm_cls", grid = "wgs", params = "wgs : int64")]
class MmBatch : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @workgroup cml_a : uint[2560]     // A tile: 128 weight rows x one block (32 K) as f16 pairs
    @workgroup cml_b : uint[2560]     // B tile: 128 token rows x one block
    @workgroup cml_o : float16[1024]  // 4 warps x 16x16 f16-acc store bounce (edge tiles only)

    [spirv_kernel(local_size_x = 128, name = "q8_batch_mm_cls_spv")]
    def run {   // nolint:STYLE037,STYLE038 — the verbatim tile pass, parity-locked to the shipped kernel
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 127u) / 128u
        let xt = tix / wtiles             // token (B) tile
        let wt = tix % wtiles             // weight (A) tile
        let gm0 = wt * 128u
        let gn0 = xt * 128u
        let nbb = pa.n / 32u
        let tid = gl_LocalInvocationID.x
        let warp_r = gl_SubgroupID % 2u   // which 64-row half of the A tile
        let warp_c = gl_SubgroupID / 2u   // which 64-col half of the B tile
        let lw = tid % 8u                 // which packed word (4 qs) of the staged row's block
        let lr0 = tid / 8u                // staging row base 0..15
        var acc00 : coopmatAcc_f16_16x16
        var acc01 : coopmatAcc_f16_16x16
        var acc02 : coopmatAcc_f16_16x16
        var acc03 : coopmatAcc_f16_16x16
        var acc10 : coopmatAcc_f16_16x16
        var acc11 : coopmatAcc_f16_16x16
        var acc12 : coopmatAcc_f16_16x16
        var acc13 : coopmatAcc_f16_16x16
        var acc20 : coopmatAcc_f16_16x16
        var acc21 : coopmatAcc_f16_16x16
        var acc22 : coopmatAcc_f16_16x16
        var acc23 : coopmatAcc_f16_16x16
        var acc30 : coopmatAcc_f16_16x16
        var acc31 : coopmatAcc_f16_16x16
        var acc32 : coopmatAcc_f16_16x16
        var acc33 : coopmatAcc_f16_16x16
        var kb = 0u
        while (kb < nbb) {
            for [unroll] (p in range(8)) {   // stage A: 128 weight rows x 1 block; OOR rows -> 0
                let row = lr0 + uint(p) * 16u
                let wrow = gm0 + row
                var v = float4(0.0)
                if (wrow < pa.d) {
                    let ib = wblk0 + wrow * nbb + kb
                    v = float4(int4(unpack8(int(wq[ib * 8u + lw])))) * float(wsh[ib])
                }
                let o = row * CML_SST + lw * 2u
                cml_a[o] = packHalf2x16(v.xy)
                cml_a[o + 1u] = packHalf2x16(v.zw)
            }
            for [unroll] (p in range(8)) {   // stage B: 128 token rows x 1 block; OOR rows -> 0
                let row = lr0 + uint(p) * 16u
                var v = float4(0.0)
                if (gn0 + row < cnt) {
                    let ib = (row0 + gn0 + row) * nbb + kb
                    v = float4(int4(unpack8(int(xqw[ib * 8u + lw])))) * axs[ib]
                }
                let o = row * CML_SST + lw * 2u
                cml_b[o] = packHalf2x16(v.xy)
                cml_b[o + 1u] = packHalf2x16(v.zw)
            }
            barrier()
            for [unroll] (ks in range(2)) {   // 2 K-subtiles x (4 A rows x 4 B cols), their MMA order
                let k8 = uint(ks) * 8u
                var ca : coopmatA_f16_16x16
                var cb : coopmatB_f16_16x16
                coopmatLoad(ca, cml_a, int((warp_r * 64u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc00 = coopmatMulAdd(ca, cb, acc00)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc01 = coopmatMulAdd(ca, cb, acc01)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc02 = coopmatMulAdd(ca, cb, acc02)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc03 = coopmatMulAdd(ca, cb, acc03)
                coopmatLoad(ca, cml_a, int((warp_r * 64u + 16u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc10 = coopmatMulAdd(ca, cb, acc10)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc11 = coopmatMulAdd(ca, cb, acc11)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc12 = coopmatMulAdd(ca, cb, acc12)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc13 = coopmatMulAdd(ca, cb, acc13)
                coopmatLoad(ca, cml_a, int((warp_r * 64u + 32u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc20 = coopmatMulAdd(ca, cb, acc20)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc21 = coopmatMulAdd(ca, cb, acc21)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc22 = coopmatMulAdd(ca, cb, acc22)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc23 = coopmatMulAdd(ca, cb, acc23)
                coopmatLoad(ca, cml_a, int((warp_r * 64u + 48u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc30 = coopmatMulAdd(ca, cb, acc30)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc31 = coopmatMulAdd(ca, cb, acc31)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc32 = coopmatMulAdd(ca, cb, acc32)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc33 = coopmatMulAdd(ca, cb, acc33)
            }
            barrier()
            kb++
        }
        // stores: y[(row0 + token) * d + wrow] — fragment weight-dim contiguous = column-major, stride d
        let dr = gm0 + warp_r * 64u
        let dc = gn0 + warp_c * 64u
        if (gm0 + 128u <= pa.d && gn0 + 128u <= cnt) {
            var accw : coopmatAcc_f32_16x16
            let yb = (row0 + dc) * pa.d + dr
            coopmatConvert(accw, acc00)
            coopmatStore(accw, y, int(yb), int(pa.d), 1)
            coopmatConvert(accw, acc01)
            coopmatStore(accw, y, int(yb + 16u * pa.d), int(pa.d), 1)
            coopmatConvert(accw, acc02)
            coopmatStore(accw, y, int(yb + 32u * pa.d), int(pa.d), 1)
            coopmatConvert(accw, acc03)
            coopmatStore(accw, y, int(yb + 48u * pa.d), int(pa.d), 1)
            coopmatConvert(accw, acc10)
            coopmatStore(accw, y, int(yb + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc11)
            coopmatStore(accw, y, int(yb + 16u * pa.d + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc12)
            coopmatStore(accw, y, int(yb + 32u * pa.d + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc13)
            coopmatStore(accw, y, int(yb + 48u * pa.d + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc20)
            coopmatStore(accw, y, int(yb + 32u), int(pa.d), 1)
            coopmatConvert(accw, acc21)
            coopmatStore(accw, y, int(yb + 16u * pa.d + 32u), int(pa.d), 1)
            coopmatConvert(accw, acc22)
            coopmatStore(accw, y, int(yb + 32u * pa.d + 32u), int(pa.d), 1)
            coopmatConvert(accw, acc23)
            coopmatStore(accw, y, int(yb + 48u * pa.d + 32u), int(pa.d), 1)
            coopmatConvert(accw, acc30)
            coopmatStore(accw, y, int(yb + 48u), int(pa.d), 1)
            coopmatConvert(accw, acc31)
            coopmatStore(accw, y, int(yb + 16u * pa.d + 48u), int(pa.d), 1)
            coopmatConvert(accw, acc32)
            coopmatStore(accw, y, int(yb + 32u * pa.d + 48u), int(pa.d), 1)
            coopmatConvert(accw, acc33)
            coopmatStore(accw, y, int(yb + 48u * pa.d + 48u), int(pa.d), 1)
        } else {
            // edge tile: every fragment bounces its f16 acc through this warp's cml_o slab (rowmajor,
            // row = weight dim), then 32 lanes widen + write 8 elements each under bounds checks
            let lane = gl_SubgroupInvocationID
            let ob = gl_SubgroupID * 256u
            for (fi in range(16)) {
                let fr = uint(fi) / 4u       // fragment row: weight offset fr*16
                let fc = uint(fi) % 4u       // fragment col: token offset fc*16
                if (fi == 0) {
                    coopmatStore(acc00, cml_o, int(ob), 16, 0)
                } elif (fi == 1) {
                    coopmatStore(acc01, cml_o, int(ob), 16, 0)
                } elif (fi == 2) {
                    coopmatStore(acc02, cml_o, int(ob), 16, 0)
                } elif (fi == 3) {
                    coopmatStore(acc03, cml_o, int(ob), 16, 0)
                } elif (fi == 4) {
                    coopmatStore(acc10, cml_o, int(ob), 16, 0)
                } elif (fi == 5) {
                    coopmatStore(acc11, cml_o, int(ob), 16, 0)
                } elif (fi == 6) {
                    coopmatStore(acc12, cml_o, int(ob), 16, 0)
                } elif (fi == 7) {
                    coopmatStore(acc13, cml_o, int(ob), 16, 0)
                } elif (fi == 8) {
                    coopmatStore(acc20, cml_o, int(ob), 16, 0)
                } elif (fi == 9) {
                    coopmatStore(acc21, cml_o, int(ob), 16, 0)
                } elif (fi == 10) {
                    coopmatStore(acc22, cml_o, int(ob), 16, 0)
                } elif (fi == 11) {
                    coopmatStore(acc23, cml_o, int(ob), 16, 0)
                } elif (fi == 12) {
                    coopmatStore(acc30, cml_o, int(ob), 16, 0)
                } elif (fi == 13) {
                    coopmatStore(acc31, cml_o, int(ob), 16, 0)
                } elif (fi == 14) {
                    coopmatStore(acc32, cml_o, int(ob), 16, 0)
                } else {
                    coopmatStore(acc33, cml_o, int(ob), 16, 0)
                }
                barrier()
                for (e in range(8)) {
                    let idx = lane * 8u + uint(e)
                    let wr = dr + fr * 16u + idx / 16u
                    let tc = dc + fc * 16u + idx % 16u
                    if (wr < pa.d && tc < cnt) {
                        y[(row0 + tc) * pa.d + wr] = float(cml_o[ob + idx])
                    }
                }
                barrier()
            }
        }
    }
}

// the aligned L twin: staging guards + edge branch removed — its own pipeline for
// d + every cnt 128-multiples
[vk_dispatch(name = "q8_batch_mm_a_cls", grid = "wgs", params = "wgs : int64")]
class MmABatch : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @workgroup cma_a : uint[2560]     // A tile: 128 weight rows x one block (32 K) as f16 pairs
    @workgroup cma_b : uint[2560]     // B tile: 128 token rows x one block

    [spirv_kernel(local_size_x = 128, name = "q8_batch_mm_a_cls_spv")]
    def run {   // nolint:STYLE038 — the verbatim tile pass, parity-locked to the shipped kernel
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let tix = reg.w
        let wtiles = (pa.d + 127u) / 128u
        let xt = tix / wtiles             // token (B) tile
        let wt = tix % wtiles             // weight (A) tile
        let gm0 = wt * 128u
        let gn0 = xt * 128u
        let nbb = pa.n / 32u
        let tid = gl_LocalInvocationID.x
        let warp_r = gl_SubgroupID % 2u   // which 64-row half of the A tile
        let warp_c = gl_SubgroupID / 2u   // which 64-col half of the B tile
        let lw = tid % 8u                 // which packed word (4 qs) of the staged row's block
        let lr0 = tid / 8u                // staging row base 0..15
        var acc00 : coopmatAcc_f16_16x16
        var acc01 : coopmatAcc_f16_16x16
        var acc02 : coopmatAcc_f16_16x16
        var acc03 : coopmatAcc_f16_16x16
        var acc10 : coopmatAcc_f16_16x16
        var acc11 : coopmatAcc_f16_16x16
        var acc12 : coopmatAcc_f16_16x16
        var acc13 : coopmatAcc_f16_16x16
        var acc20 : coopmatAcc_f16_16x16
        var acc21 : coopmatAcc_f16_16x16
        var acc22 : coopmatAcc_f16_16x16
        var acc23 : coopmatAcc_f16_16x16
        var acc30 : coopmatAcc_f16_16x16
        var acc31 : coopmatAcc_f16_16x16
        var acc32 : coopmatAcc_f16_16x16
        var acc33 : coopmatAcc_f16_16x16
        var kb = 0u
        while (kb < nbb) {
            for [unroll] (p in range(8)) {   // stage A: 128 weight rows x 1 block, unguarded
                let row = lr0 + uint(p) * 16u
                let ib = wblk0 + (gm0 + row) * nbb + kb
                let v = float4(int4(unpack8(int(wq[ib * 8u + lw])))) * float(wsh[ib])
                let o = row * CML_SST + lw * 2u
                cma_a[o] = packHalf2x16(v.xy)
                cma_a[o + 1u] = packHalf2x16(v.zw)
            }
            for [unroll] (p in range(8)) {   // stage B: 128 token rows x 1 block, unguarded
                let row = lr0 + uint(p) * 16u
                let ib = (row0 + gn0 + row) * nbb + kb
                let v = float4(int4(unpack8(int(xqw[ib * 8u + lw])))) * axs[ib]
                let o = row * CML_SST + lw * 2u
                cma_b[o] = packHalf2x16(v.xy)
                cma_b[o + 1u] = packHalf2x16(v.zw)
            }
            barrier()
            for [unroll] (ks in range(2)) {   // 2 K-subtiles x (4 A rows x 4 B cols), their MMA order
                let k8 = uint(ks) * 8u
                var ca : coopmatA_f16_16x16
                var cb : coopmatB_f16_16x16
                coopmatLoad(ca, cma_a, int((warp_r * 64u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cma_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc00 = coopmatMulAdd(ca, cb, acc00)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc01 = coopmatMulAdd(ca, cb, acc01)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc02 = coopmatMulAdd(ca, cb, acc02)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc03 = coopmatMulAdd(ca, cb, acc03)
                coopmatLoad(ca, cma_a, int((warp_r * 64u + 16u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cma_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc10 = coopmatMulAdd(ca, cb, acc10)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc11 = coopmatMulAdd(ca, cb, acc11)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc12 = coopmatMulAdd(ca, cb, acc12)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc13 = coopmatMulAdd(ca, cb, acc13)
                coopmatLoad(ca, cma_a, int((warp_r * 64u + 32u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cma_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc20 = coopmatMulAdd(ca, cb, acc20)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc21 = coopmatMulAdd(ca, cb, acc21)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc22 = coopmatMulAdd(ca, cb, acc22)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc23 = coopmatMulAdd(ca, cb, acc23)
                coopmatLoad(ca, cma_a, int((warp_r * 64u + 48u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cma_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc30 = coopmatMulAdd(ca, cb, acc30)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc31 = coopmatMulAdd(ca, cb, acc31)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc32 = coopmatMulAdd(ca, cb, acc32)
                coopmatLoad(cb, cma_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc33 = coopmatMulAdd(ca, cb, acc33)
            }
            barrier()
            kb++
        }
        // stores: y[(row0 + token) * d + wrow] — fragment weight-dim contiguous = column-major, stride d
        let dr = gm0 + warp_r * 64u
        let dc = gn0 + warp_c * 64u
        var accw : coopmatAcc_f32_16x16
        let yb = (row0 + dc) * pa.d + dr
        coopmatConvert(accw, acc00)
        coopmatStore(accw, y, int(yb), int(pa.d), 1)
        coopmatConvert(accw, acc01)
        coopmatStore(accw, y, int(yb + 16u * pa.d), int(pa.d), 1)
        coopmatConvert(accw, acc02)
        coopmatStore(accw, y, int(yb + 32u * pa.d), int(pa.d), 1)
        coopmatConvert(accw, acc03)
        coopmatStore(accw, y, int(yb + 48u * pa.d), int(pa.d), 1)
        coopmatConvert(accw, acc10)
        coopmatStore(accw, y, int(yb + 16u), int(pa.d), 1)
        coopmatConvert(accw, acc11)
        coopmatStore(accw, y, int(yb + 16u * pa.d + 16u), int(pa.d), 1)
        coopmatConvert(accw, acc12)
        coopmatStore(accw, y, int(yb + 32u * pa.d + 16u), int(pa.d), 1)
        coopmatConvert(accw, acc13)
        coopmatStore(accw, y, int(yb + 48u * pa.d + 16u), int(pa.d), 1)
        coopmatConvert(accw, acc20)
        coopmatStore(accw, y, int(yb + 32u), int(pa.d), 1)
        coopmatConvert(accw, acc21)
        coopmatStore(accw, y, int(yb + 16u * pa.d + 32u), int(pa.d), 1)
        coopmatConvert(accw, acc22)
        coopmatStore(accw, y, int(yb + 32u * pa.d + 32u), int(pa.d), 1)
        coopmatConvert(accw, acc23)
        coopmatStore(accw, y, int(yb + 48u * pa.d + 32u), int(pa.d), 1)
        coopmatConvert(accw, acc30)
        coopmatStore(accw, y, int(yb + 48u), int(pa.d), 1)
        coopmatConvert(accw, acc31)
        coopmatStore(accw, y, int(yb + 16u * pa.d + 48u), int(pa.d), 1)
        coopmatConvert(accw, acc32)
        coopmatStore(accw, y, int(yb + 32u * pa.d + 48u), int(pa.d), 1)
        coopmatConvert(accw, acc33)
        coopmatStore(accw, y, int(yb + 48u * pa.d + 48u), int(pa.d), 1)
    }
}

// the mul_mm M-tile twin (64x64): the L scheme at half edge — 4 warps 2x2 over the tile,
// 2x2 fragments each. Routed in when cnt or d <= 64
[vk_dispatch(name = "q8_batch_mm_m_cls", grid = "wgs", params = "wgs : int64")]
class MmMBatch : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @workgroup cmm_a : uint[1280]     // A tile: 64 weight rows x one block (32 K) as f16 pairs
    @workgroup cmm_b : uint[1280]     // B tile: 64 token rows x one block
    @workgroup cml_o : float16[1024]  // 4 warps x 16x16 f16-acc store bounce (edge tiles only)

    [spirv_kernel(local_size_x = 128, name = "q8_batch_mm_m_cls_spv")]
    def run {   // nolint:STYLE038 — the verbatim tile pass, parity-locked to the shipped kernel
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 63u) / 64u
        let xt = tix / wtiles             // token (B) tile
        let wt = tix % wtiles             // weight (A) tile
        let gm0 = wt * 64u
        let gn0 = xt * 64u
        let nbb = pa.n / 32u
        let tid = gl_LocalInvocationID.x
        let warp_r = gl_SubgroupID % 2u   // which 32-row half of the A tile
        let warp_c = gl_SubgroupID / 2u   // which 32-col half of the B tile
        let lw = tid % 8u                 // which packed word (4 qs) of the staged row's block
        let lr0 = tid / 8u                // staging row base 0..15
        var acc00 : coopmatAcc_f16_16x16
        var acc01 : coopmatAcc_f16_16x16
        var acc10 : coopmatAcc_f16_16x16
        var acc11 : coopmatAcc_f16_16x16
        var kb = 0u
        while (kb < nbb) {
            for [unroll] (p in range(4)) {   // stage A: 64 weight rows x 1 block; OOR rows -> 0
                let row = lr0 + uint(p) * 16u
                let wrow = gm0 + row
                var v = float4(0.0)
                if (wrow < pa.d) {
                    let ib = wblk0 + wrow * nbb + kb
                    v = float4(int4(unpack8(int(wq[ib * 8u + lw])))) * float(wsh[ib])
                }
                let o = row * CML_SST + lw * 2u
                cmm_a[o] = packHalf2x16(v.xy)
                cmm_a[o + 1u] = packHalf2x16(v.zw)
            }
            for [unroll] (p in range(4)) {   // stage B: 64 token rows x 1 block; OOR rows -> 0
                let row = lr0 + uint(p) * 16u
                var v = float4(0.0)
                if (gn0 + row < cnt) {
                    let ib = (row0 + gn0 + row) * nbb + kb
                    v = float4(int4(unpack8(int(xqw[ib * 8u + lw])))) * axs[ib]
                }
                let o = row * CML_SST + lw * 2u
                cmm_b[o] = packHalf2x16(v.xy)
                cmm_b[o + 1u] = packHalf2x16(v.zw)
            }
            barrier()
            for [unroll] (ks in range(2)) {   // 2 K-subtiles x (2 A rows x 2 B cols), the L MMA order
                let k8 = uint(ks) * 8u
                var ca : coopmatA_f16_16x16
                var cb : coopmatB_f16_16x16
                coopmatLoad(ca, cmm_a, int((warp_r * 32u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cmm_b, int((warp_c * 32u) * CML_SST + k8), int(CML_SST), 1)
                acc00 = coopmatMulAdd(ca, cb, acc00)
                coopmatLoad(cb, cmm_b, int((warp_c * 32u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc01 = coopmatMulAdd(ca, cb, acc01)
                coopmatLoad(ca, cmm_a, int((warp_r * 32u + 16u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cmm_b, int((warp_c * 32u) * CML_SST + k8), int(CML_SST), 1)
                acc10 = coopmatMulAdd(ca, cb, acc10)
                coopmatLoad(cb, cmm_b, int((warp_c * 32u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc11 = coopmatMulAdd(ca, cb, acc11)
            }
            barrier()
            kb++
        }
        // stores: y[(row0 + token) * d + wrow] — same column-major layout as the L tile
        let dr = gm0 + warp_r * 32u
        let dc = gn0 + warp_c * 32u
        if (gm0 + 64u <= pa.d && gn0 + 64u <= cnt) {
            var accw : coopmatAcc_f32_16x16
            let yb = (row0 + dc) * pa.d + dr
            coopmatConvert(accw, acc00)
            coopmatStore(accw, y, int(yb), int(pa.d), 1)
            coopmatConvert(accw, acc01)
            coopmatStore(accw, y, int(yb + 16u * pa.d), int(pa.d), 1)
            coopmatConvert(accw, acc10)
            coopmatStore(accw, y, int(yb + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc11)
            coopmatStore(accw, y, int(yb + 16u * pa.d + 16u), int(pa.d), 1)
        } else {
            // edge tile: bounce each f16 acc through this warp's cml_o slab, bounds-checked write-out
            let lane = gl_SubgroupInvocationID
            let ob = gl_SubgroupID * 256u
            for (fi in range(4)) {
                let fr = uint(fi) / 2u       // fragment row: weight offset fr*16
                let fc = uint(fi) % 2u       // fragment col: token offset fc*16
                if (fi == 0) {
                    coopmatStore(acc00, cml_o, int(ob), 16, 0)
                } elif (fi == 1) {
                    coopmatStore(acc01, cml_o, int(ob), 16, 0)
                } elif (fi == 2) {
                    coopmatStore(acc10, cml_o, int(ob), 16, 0)
                } else {
                    coopmatStore(acc11, cml_o, int(ob), 16, 0)
                }
                barrier()
                for (e in range(8)) {
                    let idx = lane * 8u + uint(e)
                    let wr = dr + fr * 16u + idx / 16u
                    let tc = dc + fc * 16u + idx % 16u
                    if (wr < pa.d && tc < cnt) {
                        y[(row0 + tc) * pa.d + wr] = float(cml_o[ob + idx])
                    }
                }
                barrier()
            }
        }
    }
}

// Q4_0 coopmat f16 variant (fmt=4 stacks): the q40 math baked into f16 dequant — w = (nib-8)*d_b,
// a = aq*xs (per-256 Q8_K) — then the same 4-subgroup MMA as CmF16Batch
[vk_dispatch(name = "kq_batch_q40_cmf16_cls", grid = "wgs", params = "wgs : int64")]
class KqQ40CmF16 : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // nibble-packed weight superblocks (32 words each)
    @ssbo @binding = 1 wsu : array<uint>       // 5-word scale superblocks (d halves)
    @ssbo @binding = 3 xqw : array<uint>       // Q8_K activation words (64 per row-superblock)
    @ssbo @binding = 4 axs : array<float>      // one Q8_K d per (row, superblock)
    @ssbo @binding = 5 y : array<float>
    @workgroup cm_as : float16[8192]   // 32 local rows x 256 k, row-major stride 256 (f16 dequant A)
    @workgroup cm_bs : float16[8192]   // 256 k x 32 cols, row-major stride 32 (f16 dequant B)
    @workgroup cm_out : float[1024]    // 4 subgroups x 16x16 f32 scratch (bounds-checked write-out)

    [spirv_kernel(local_size_x = 128, name = "kq_batch_q40_cmf16_cls_spv")]
    def run {   // nolint:STYLE038 — the verbatim tile pass, parity-locked to the shipped kernel
        let reg = region_rec()
        let wsb0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nsb = pa.n / 256u
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let row_half = sg / 2u
        let col_half = sg % 2u
        var acc : coopmatAcc_f32_16x16
        var s = 0u
        while (s < nsb) {
            // stage A: 32 rows x 64 uints (one superblock of Q8_K acts), 16/thread — one thread stays
            // in one row (16 divides 64), so xs (per-256) loads ONCE per thread
            let arow0 = tid * 16u / 64u            // this thread's row
            let ac0 = tid * 16u % 64u              // first chunk within the row
            let axrow = xt * 32u + arow0
            var xs = 0.0
            if (axrow < cnt) {
                xs = axs[(row0 + axrow) * nsb + s]
            }
            for (i in range(16)) {
                let c = ac0 + uint(i)
                let bu = c / 8u
                let ww = c % 8u
                var packed = 0u
                if (axrow < cnt) {
                    packed = xqw[((row0 + axrow) * nsb + s) * 64u + c]
                }
                let qv = int4(unpack8(int(packed)))
                let o = arow0 * 256u + bu * 32u + ww * 4u
                cm_as[o] = float16(float(qv.x) * xs)
                cm_as[o + 1u] = float16(float(qv.y) * xs)
                cm_as[o + 2u] = float16(float(qv.z) * xs)
                cm_as[o + 3u] = float16(float(qv.w) * xs)
            }
            // stage B: 32 cols x 32 uints (one superblock of nibble-packed weights), 8/thread; each
            // uint yields 4 lo-nibble weights (k = bu*32 + w*4 + j) and 4 hi (k + 16), dequant (nib-8)*d_b
            for (i in range(8)) {
                let u2 = tid * 8u + uint(i)
                let col = u2 / 32u
                let c = u2 % 32u
                let bu = c / 4u
                let w = c % 4u
                let wcol = wt * 32u + col
                var r = 0u
                var db = 0.0
                if (wcol < pa.d) {
                    let wsb = wsb0 + wcol * nsb + s
                    r = wq[wsb * 32u + c]
                    let dp = unpackHalf2x16(wsu[wsb * 5u + bu / 2u])
                    db = bu % 2u == 0u ? dp.x : dp.y
                }
                let k = bu * 32u + w * 4u
                for (j in range(4)) {
                    let lo = int((r >> (uint(j) * 8u)) & 0x0Fu)
                    let hi = int((r >> (uint(j) * 8u + 4u)) & 0x0Fu)
                    cm_bs[(k + uint(j)) * 32u + col] = float16(float(lo - 8) * db)
                    cm_bs[(k + 16u + uint(j)) * 32u + col] = float16(float(hi - 8) * db)
                }
            }
            barrier()
            for (bb in range(8)) {
                for (ks in range(2)) {
                    var a : coopmatA_f16_16x16
                    var b : coopmatB_f16_16x16
                    coopmatLoad(a, cm_as, int(row_half * 16u * 256u + uint(bb) * 32u + uint(ks) * 16u), 256, 0)
                    coopmatLoad(b, cm_bs, int((uint(bb) * 32u + uint(ks) * 16u) * 32u + col_half * 16u), 32, 0)
                    acc = coopmatMulAdd(a, b, acc)
                }
            }
            barrier()
            s++
        }
        coopmatStore(acc, cm_out, int(sg * 256u), 16, 0)
        barrier()
        let lane = gl_SubgroupInvocationID
        for (e in range(8)) {
            let idx = lane * 8u + uint(e)
            let m = idx / 16u
            let nn = idx % 16u
            let orow = xt * 32u + row_half * 16u + m
            let ocol = wt * 32u + col_half * 16u + nn
            if (orow < cnt && ocol < pa.d) {
                y[(row0 + orow) * pa.d + ocol] = cm_out[sg * 256u + idx]
            }
        }
    }
}

// ===== the cm2 (NV_cooperative_matrix2) prefill tiles: native planes, decode-in-load =====

struct VkQ8Blk {
    qs : int16[16]   // one fmt-0 q8 block: 32 packed int8 quants as 16-bit lanes; the scale lives in ws
}

struct VkK4Blk {
    qs : int16[64]   // one Q4_K superblock's 128 nibble bytes as 16-bit lanes; its scale row (d|dmin + 8 sc + 8 mn bytes) lives in ws
}

struct VkK6Blk {
    ql : int16[64]   // one Q6_K superblock's 128 low-nibble bytes as 16-bit lanes
    qh : int16[32]   // ... and its 64 high-2-bit bytes; scales (16 int8 + f16 d) live in ws
}

struct VkK5Blk {
    qs : int16[64]   // one Q5_K superblock's 128 nibble bytes (the k/k+16 device pairing)
    qh : int16[16]   // ... and its 32 packed 5th-bit bytes: byte g*4+q = bits of elems g*32+4q..+3 (lo) | +16.. (hi)
}

struct VkIq3sBlk {
    qs : int16[32]   // one IQ3_S superblock's 64 grid-index bytes as 16-bit lanes
    qh : int16[4]    // ... its 8 ninth-bit bytes
    sg : int16[16]   // ... and its 32 sign bytes; scales (f16 d + 8 int8 (1 + 2s)) live in ws
}

struct VkIq3xxsBlk {
    qs : int16[32]   // one IQ3_XXS superblock's 64 grid-index bytes as 16-bit lanes
    ax : int16[16]   // ... and its 32 aux bytes (per block: 4x7-bit ksigns indices + 4-bit ls); scales (halved f16 d + 8 x (2ls+1)) live in ws
}

struct VkK3Blk {
    qs : int16[32]   // one Q3_K superblock's 64 qs bytes, four 2-bit lanes each, verbatim disk order
    hm : int16[16]   // ... and its 32 hmask bytes; scales (16 int8 (sc-32) + f16 d) live in ws
}

struct VkK2Blk {
    qs : int16[32]   // one Q2_K superblock's 64 qs bytes, four 2-bit lanes each; scales ([f16 d][f16 dmin][16 sc/min pair bytes]) live in ws
}

struct VkIq2sBlk {
    qs : int16[16]   // one IQ2_S superblock's 32 grid-index bytes as 16-bit lanes
    sg : int16[16]   // ... its 32 sign bytes; scales (f16 d8 + 16 unsigned (1 + 2s) strips) live in ws
    qh : int16[4]    // ... and its 8 qh bytes (two index high bits per (block, l) window)
}

struct VkIq2xsBlk {
    qs : int16[32]   // one IQ2_XS superblock's 32 u16 qs words as 16-bit lanes (9-bit grid index + 7-bit ksigns index each); scales (f16 d8 + 16 unsigned (1 + 2s) strips) live in ws
}

struct VkIq2xxsBlk {
    qs : int16[32]   // one IQ2_XXS superblock's 64B qs as 16-bit lanes: per block 4 grid byte indices + its aux32 (4 x 7-bit ksigns + per-32 ls nibble); scales (f16 d8 + 8 unsigned (1 + 2s) strips) live in ws
}

// The cm2 tile: one class template over the two stamp axes. The token column BN - l 256, m 128,
// s 32 (the expert-bucket geometry) - and the weight format: BLK
// the 16-bit-lane block struct, ST the scale plane's element, BLKW the block's element width, and
// the format template's decode. 256 threads, one 128-weight x BN-token tile per wg, BK=64 per
// step, an 8x-unrolled k loop; the decode METHOD reads the separate scale plane by block index -
// the reason the class-method decode form exists. Binding 4 unused (sparse)
[ |> template_struct_instance]
class template KqCm2BatchT : MoeCmBase {
    @ssbo @binding = 0 @role = "weight" wq : array<BLK>   // the quant plane, read as 16-bit lanes
    @ssbo @binding = 1 @role = "weight" ws : array<ST>    // the scale plane (q8: f16 per block; kq: 5 words per superblock)
    @ssbo @binding = 3 xf16 : array<float16>              // f16 activation plane
    @ssbo @binding = 5 y : array<float>
    @workgroup wg_blk0 : uint                             // the region's block base, staged for the decode
    @template_constant BN : uint = 256u                   // the token column
    @template_constant STILE : bool = false               // the 32-row column's partial-column fast path
    @template_constant BLKW : uint = 256u                 // elements per weight block
    @template_constant DECVEC : bool = true               // the four-wide decode twin rides the loads; a format whose probe row shows the synthesized twin losing turns it off
    @template_constant IQLUT : bool = false               // stage the iq4nl codebook into workgroup memory
    @workgroup @template_gate = IQLUT iq4lut : float16[16]   // kvalues_iq4nl as f16
    @template_constant IQ3GRID : bool = false             // stage the 2 KB iq3s grid into workgroup memory
    @workgroup @template_gate = IQ3GRID iq3s_gridc : uint[512]
    @template_constant IQ3XGRID : bool = false            // stage the 1 KB halved iq3xxs grid into workgroup memory
    @workgroup @template_gate = IQ3XGRID iq3x_gridc : uint[256]
    @template_constant IQ2SGRID : bool = false            // stage the 8 KB iq2s u64 grid into workgroup memory
    @workgroup @template_gate = IQ2SGRID iq2s_gridc : uint[2048]
    @template_constant IQ2XSGRID : bool = false           // stage the 4 KB iq2xs u64 grid into workgroup memory
    @workgroup @template_gate = IQ2XSGRID iq2xs_gridc : uint[1024]
    @template_constant IQ2XXSGRID : bool = false          // stage the 2 KB iq2xxs u64 grid into workgroup memory
    @workgroup @template_gate = IQ2XXSGRID iq2xxs_gridc : uint[512]

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-tile-pick-and-default")]
    def run {   // nolint:STYLE037,STYLE038 — the fast/edge path pair, barrier- and register-coupled
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let ttiles = (cnt + BN - 1u) / BN
        // split-k: wgs stack nsplit planes of (wtiles x ttiles); plane ks covers k [k0, k1) and
        // stores f32 partials at ybase (the reduce sums the planes into y). SINGLE-REGION (dense)
        // dispatches only — the plane size rides row0 + cnt, so multi-region planes would collide
        var tix = reg.w
        var ks = 0u
        var k1 = pa.n
        var ybase = 0u
        if (pa.ksplit != 0u) {
            let ptiles = ((pa.d + 127u) / 128u) * ttiles
            ks = tix / ptiles
            tix -= ks * ptiles
            k1 = min(pa.n, (ks + 1u) * pa.ksplit)
            ybase = ks * (row0 + cnt) * pa.d
        }
        let k0 = ks * pa.ksplit
        let xt = tix % ttiles             // token BN-tile (fastest)
        let wt = tix / ttiles             // weight 128-tile
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        static_if (IQLUT) {
            if (gl_LocalInvocationID.x < 16u) {
                let li = gl_LocalInvocationID.x
                let tbl = uint4(3215825025u, 4142587343u, 639175937u, 1901675829u)
                iq4lut[li] = float16(float((int((tbl[int(li >> 2u)] >> ((li & 3u) * 8u)) & 255u) ^ 128) - 128))
            }
        }
        static_if (IQ3GRID) {
            let gi = gl_LocalInvocationID.x
            iq3s_gridc[gi] = iq3s_grid_word(int(gi))
            iq3s_gridc[gi + 256u] = iq3s_grid_word(int(gi + 256u))
        }
        static_if (IQ3XGRID) {
            iq3x_gridc[gl_LocalInvocationID.x] = iq3xxs_grid_word(int(gl_LocalInvocationID.x))
        }
        static_if (IQ2SGRID) {
            let g2 = gl_LocalInvocationID.x
            for (k in range(8)) {
                iq2s_gridc[g2 + uint(k) * 256u] = iq2s_grid_word(int(g2 + uint(k) * 256u))
            }
        }
        static_if (IQ2XSGRID) {
            let g2x = gl_LocalInvocationID.x
            for (k in range(4)) {
                iq2xs_gridc[g2x + uint(k) * 256u] = iq2xs_grid_word(int(g2x + uint(k) * 256u))
            }
        }
        static_if (IQ2XXSGRID) {
            let g2xx = gl_LocalInvocationID.x
            for (k in range(2)) {
                iq2xxs_gridc[g2xx + uint(k) * 256u] = iq2xxs_grid_word(int(g2xx + uint(k) * 256u))
            }
        }
        barrier()   // wg_blk0 (and the gated codebook) visible before the first decode load
        var a : coopmatWgA_f16_128x64
        var b : BT
        var acc : ACC
        let t0 = row0 + xt * BN
        let m0 = wt * 128u
        if (m0 + 128u <= pa.d && (STILE || xt * BN + BN <= cnt) && (pa.n & 63u) == 0u) {
            var fla : tensorLayout2D
            tensorLayoutCreate(fla)
            tensorLayoutSetBlockSize(fla, 1u, BLKW)
            tensorLayoutSetDimension(fla, pa.d, pa.n)
            tensorLayoutSetStride(fla, pa.n / BLKW, 1u)
            var flb : tensorLayout2D
            tensorLayoutCreate(flb)
            tensorLayoutSetDimension(flb, row0 + (STILE ? ((cnt + 31u) & ~31u) : cnt), pa.n)
            tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
            var flo : FLO
            tensorLayoutCreate(flo)
            tensorLayoutSetDimension(flo, row0 + cnt, pa.d)
            tensorLayoutSetStride(flo, pa.d & ~7u, 1u)
            if (pa.ksplit == 0u) {
                var k = 0u
                for (_i in range(int(pa.n / 512u))) {
                    for [unroll] (_j in range(8)) {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode, DECVEC)
                        coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, 64u, tv)
                        acc = coopmatMulAdd(a, b, acc)
                        k += 64u
                    }
                }
                while (k < pa.n) {
                    coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode, DECVEC)
                    coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, 64u, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += 64u
                }
                coopmatClamp(acc, -65504.0, 65504.0)
                var accw : ACCW
                coopmatConvert(accw, acc)
                coopmatStoreTensor(accw, y, 0u, flo, t0, BN, m0, 128u, tv)
                return
            }
            var k = k0
            for (_i in range(int((k1 - k0) / 512u))) {
                for [unroll] (_j in range(8)) {
                    coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode, DECVEC)
                    coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, 64u, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += 64u
                }
            }
            while (k < k1) {
                coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode, DECVEC)
                coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
            coopmatClamp(acc, -65504.0, 65504.0)
            var accw : ACCW
            coopmatConvert(accw, acc)
            coopmatStoreTensor(accw, y, ybase, flo, t0, BN, m0, 128u, tv)
            return
        }
        // EDGE PATH — clamp-Constant layouts everywhere (store discard load-bearing)
        var tla : tensorLayout2DPad
        tensorLayoutCreate(tla)
        tensorLayoutSetBlockSize(tla, 1u, BLKW)
        tensorLayoutSetDimension(tla, pa.d, pa.n)
        tensorLayoutSetStride(tla, pa.n / BLKW, 1u)
        var tlb : tensorLayout2DPad
        tensorLayoutCreate(tlb)
        tensorLayoutSetDimension(tlb, row0 + cnt, pa.n)
        tensorLayoutSetStride(tlb, pa.n, 1u)
        var tlo : tensorLayout2DPad
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, row0 + cnt, pa.d)
        tensorLayoutSetStride(tlo, pa.d, 1u)
        if (pa.ksplit == 0u) {
            var k = 0u
            for (_i in range(int(pa.n / 512u))) {
                for [unroll] (_j in range(8)) {
                    coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, 64u, self.decode)
                    coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, 64u, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += 64u
                }
            }
            while (k < pa.n) {
                coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, 64u, self.decode)
                coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
            coopmatClamp(acc, -65504.0, 65504.0)
            var accw : ACCW
            coopmatConvert(accw, acc)
            coopmatStoreTensor(accw, y, 0u, tlo, t0, BN, m0, 128u, tv)
            return
        }
        var k = k0
        for (_i in range(int((k1 - k0) / 512u))) {
            for [unroll] (_j in range(8)) {
                coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, 64u, self.decode)
                coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, 64u, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += 64u
            }
        }
        while (k < k1) {
            coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, 64u, self.decode)
            coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : ACCW
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, ybase, tlo, t0, BN, m0, 128u, tv)
    }
}

// the format templates: the block struct, the scale element, the block width and the decode
[ |> template_struct_instance]
class template Q8Cm2T : KqCm2BatchT {
    typedef BLK = VkQ8Blk
    typedef ST = float16
    override BLKW = 32u

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkQ8Blk; bc, cib : uint2) : float16 {
        let q = unpack8(blk.qs[int((cib.y & 30u) >> 1u)])[int(cib.y & 1u)]
        return ws[wg_blk0 + bc.x * (pa.n >> 5u) + bc.y] * float16(float(int(q)))
    }
}

[ |> template_struct_instance]
class template K4Cm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk
    typedef ST = uint

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let by = uint(int(unpack8(blk.qs[int(bidx >> 1u)])[int(bidx & 1u)])) & 0xFFu
        let q = (by >> ((e >> 4u) * 4u)) & 0xFu
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dm = unpackHalf2x16(ws[srow])
        let sc = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
        let mn = (ws[srow + 3u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
        return float16(dm.x * float(sc) * float(q) - dm.y * float(mn))
    }
}

[ |> template_struct_instance]
class template K6Cm2T : KqCm2BatchT {
    typedef BLK = VkK6Blk
    typedef ST = uint

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK6Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let bu = e >> 5u
        let hh = (e >> 4u) & 1u
        let j = e & 15u
        let lb = bu * 16u + j
        let lo = uint(int(unpack8(blk.ql[int(lb >> 1u)])[int(lb & 1u)])) & 0xFFu
        let qb = (bu >> 2u) * 32u + hh * 16u + j
        let hby = uint(int(unpack8(blk.qh[int(qb >> 1u)])[int(qb & 1u)])) & 0xFFu
        let q6 = int(((lo >> (hh * 4u)) & 0xFu) | (((hby >> ((bu & 3u) * 2u)) & 3u) << 4u)) - 32
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let sidx = e >> 4u
        let sc = int(ws[srow + (sidx >> 2u)] << ((3u - (sidx & 3u)) * 8u)) >> 24
        let d = unpackHalf2x16(ws[srow + 4u]).x
        return float16(d * float(sc) * float(q6))
    }
}

[ |> template_struct_instance]
class template K5Cm2T : KqCm2BatchT {
    typedef BLK = VkK5Blk
    typedef ST = uint

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK5Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let j = e & 15u
        let h = e >> 4u
        let bidx = g * 16u + j
        let by = uint(int(unpack8(blk.qs[int(bidx >> 1u)])[int(bidx & 1u)])) & 0xFFu
        let hidx = g * 4u + (j >> 2u)
        let hby = uint(int(unpack8(blk.qh[int(hidx >> 1u)])[int(hidx & 1u)])) & 0xFFu
        let q = ((by >> (h * 4u)) & 0xFu) | (((hby >> ((j & 3u) + h * 4u)) & 1u) << 4u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dm = unpackHalf2x16(ws[srow])
        let sc = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
        let mn = (ws[srow + 3u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
        return float16(dm.x * float(sc) * float(q) - dm.y * float(mn))
    }
}

[ |> template_struct_instance]
class template Q40Cm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk   // the same 128-nibble lane view - q40 rows land in the k/k+16 device pairing too
    typedef ST = uint

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let by = uint(int(unpack8(blk.qs[int(bidx >> 1u)])[int(bidx & 1u)])) & 0xFFu
        let q = (by >> ((e >> 4u) * 4u)) & 0xFu
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dp = unpackHalf2x16(ws[srow + (g >> 1u)])
        let d = (g & 1u) == 0u ? dp.x : dp.y
        return float16(d * (float(q) - 8.0))
    }
}

[ |> template_struct_instance]
class template Iq4xsCm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk   // the iq4xs plane is the k/k+16 pairing verbatim - the k4 lane view fits
    typedef ST = uint
    override IQLUT = true

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let by = uint(int(unpack8(blk.qs[int(bidx >> 1u)])[int(bidx & 1u)])) & 0xFFu
        let q = (by >> ((e >> 4u) * 4u)) & 0xFu
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let d = unpackHalf2x16(ws[srow]).x
        let sc = int(ws[srow + 1u + (g >> 2u)] << ((3u - (g & 3u)) * 8u)) >> 24
        return float16(d * float(sc) * float(iq4lut[int(q)]))
    }
}

[ |> template_struct_instance]
class template K3Cm2T : KqCm2BatchT {
    typedef BLK = VkK3Blk
    typedef ST = uint

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK3Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let g = e >> 5u
        let l = e & 31u
        let qi = (g >> 2u) * 32u + l
        let qby = uint(int(unpack8(blk.qs[int(qi >> 1u)])[int(qi & 1u)])) & 0xFFu
        let hby = uint(int(unpack8(blk.hm[int(l >> 1u)])[int(l & 1u)])) & 0xFFu
        let q3 = int((qby >> ((g & 3u) * 2u)) & 3u) - (((hby >> g) & 1u) == 0u ? 4 : 0)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let sidx = e >> 4u
        let sc = int(ws[srow + (sidx >> 2u)] << ((3u - (sidx & 3u)) * 8u)) >> 24
        let d = unpackHalf2x16(ws[srow + 4u]).x
        return float16(d * float(sc) * float(q3))
    }
}

[ |> template_struct_instance]
class template K2Cm2T : KqCm2BatchT {
    typedef BLK = VkK2Blk
    typedef ST = uint

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK2Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let g = e >> 4u
        let l = e & 15u
        let qi = (g >> 3u) * 32u + (g & 1u) * 16u + l
        let qby = uint(int(unpack8(blk.qs[int(qi >> 1u)])[int(qi & 1u)])) & 0xFFu
        let q = (qby >> (((g >> 1u) & 3u) * 2u)) & 3u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dm = unpackHalf2x16(ws[srow])
        let pb = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
        return float16(dm.x * float(pb & 15u) * float(q) - dm.y * float(pb >> 4u))
    }
}

[ |> template_struct_instance]
class template Iq3sCm2T : KqCm2BatchT {
    typedef BLK = VkIq3sBlk
    typedef ST = uint
    override IQ3GRID = true
    override DECVEC = false   // probe cm2:iq3s: the synthesized twin loses 4-9% (a grid read + sign per element, four calls do not merge)

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq3sBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = uint(int(unpack8(blk.qs[int(qi >> 1u)])[int(qi & 1u)])) & 0xFFu
        let qh = uint(int(unpack8(blk.qh[int(g >> 1u)])[int(g & 1u)])) & 0xFFu
        let si = g * 4u + (r >> 3u)
        let sgb = uint(int(unpack8(blk.sg[int(si >> 1u)])[int(si & 1u)])) & 0xFFu
        let gw = iq3s_gridc[qb | ((qh << (8u - wj)) & 256u)]
        let gb = float((gw >> ((r & 3u) * 8u)) & 0xFFu)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let d = unpackHalf2x16(ws[srow]).x
        let sc = int(ws[srow + 1u + (g >> 2u)] << ((3u - (g & 3u)) * 8u)) >> 24
        return float16(d * float(sc) * (((sgb >> (((r >> 2u) & 1u) * 4u + (r & 3u))) & 1u) != 0u ? -gb : gb))
    }
}

[ |> template_struct_instance]
class template Iq3xxsCm2T : KqCm2BatchT {
    typedef BLK = VkIq3xxsBlk
    typedef ST = uint
    override IQ3XGRID = true

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq3xxsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = uint(int(unpack8(blk.qs[int(qi >> 1u)])[int(qi & 1u)])) & 0xFFu
        let l = r >> 3u
        let bl = 7u * l
        let a0i = g * 4u + (bl >> 3u)
        let b0 = uint(int(unpack8(blk.ax[int(a0i >> 1u)])[int(a0i & 1u)])) & 0xFFu
        let b1 = uint(int(unpack8(blk.ax[int((a0i + 1u) >> 1u)])[int((a0i + 1u) & 1u)])) & 0xFFu
        let sidx = ((b0 | (b1 << 8u)) >> (bl & 7u)) & 127u
        var tt = sidx ^ (sidx >> 4u)
        tt = tt ^ (tt >> 2u)
        tt = tt ^ (tt >> 1u)
        let sgb = sidx | ((tt & 1u) << 7u)
        let gw = iq3x_gridc[qb]
        let gb = float((gw >> ((r & 3u) * 8u)) & 0xFFu)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let d = unpackHalf2x16(ws[srow]).x
        let sc = int(ws[srow + 1u + (g >> 2u)] << ((3u - (g & 3u)) * 8u)) >> 24
        return float16(d * float(sc) * (((sgb >> (r & 7u)) & 1u) != 0u ? -gb : gb))
    }
}

[ |> template_struct_instance]
class template Iq4nlCm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk   // q40's nibble plane verbatim - the k4 lane view fits
    typedef ST = uint
    override IQLUT = true

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let by = uint(int(unpack8(blk.qs[int(bidx >> 1u)])[int(bidx & 1u)])) & 0xFFu
        let q = (by >> ((e >> 4u) * 4u)) & 0xFu
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dp = unpackHalf2x16(ws[srow + (g >> 1u)])
        let d = (g & 1u) == 0u ? dp.x : dp.y
        return float16(d * float(iq4lut[int(q)]))
    }
}

[ |> template_struct_instance]
class template Iq2sCm2T : KqCm2BatchT {
    typedef BLK = VkIq2sBlk
    typedef ST = uint
    override IQ2SGRID = true
    override DECVEC = false   // probe cm2:iq2s: the synthesized twin loses 1-11%

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2sBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 4u + l
        let qb = uint(int(unpack8(blk.qs[int(qi >> 1u)])[int(qi & 1u)])) & 0xFFu
        let qh = uint(int(unpack8(blk.qh[int(g >> 1u)])[int(g & 1u)])) & 0xFFu
        let sgb = uint(int(unpack8(blk.sg[int(qi >> 1u)])[int(qi & 1u)])) & 0xFFu
        let gw = iq2s_gridc[(qb | ((qh << (8u - 2u * l)) & 0x300u)) * 2u + (j >> 2u)]
        let gb = float((gw >> ((j & 3u) * 8u)) & 0xFFu)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let d = unpackHalf2x16(ws[srow]).x
        let si = g * 2u + (r >> 4u)
        let sc = (ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu
        return float16(d * float(sc) * (((sgb >> j) & 1u) != 0u ? -gb : gb))
    }
}

class template Iq2xsCm2T : KqCm2BatchT {
    typedef BLK = VkIq2xsBlk
    typedef ST = uint
    override IQ2XSGRID = true

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2xsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let wi = e >> 3u
        let j = e & 7u
        let w16 = uint(int(blk.qs[int(wi)])) & 0xFFFFu
        let sidx = w16 >> 9u
        var tt = sidx ^ (sidx >> 4u)
        tt = tt ^ (tt >> 2u)
        tt = tt ^ (tt >> 1u)
        let sgb = sidx | ((tt & 1u) << 7u)
        let gw = iq2xs_gridc[(w16 & 511u) * 2u + (j >> 2u)]
        let gb = float((gw >> ((j & 3u) * 8u)) & 0xFFu)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dv = unpackHalf2x16(ws[srow]).x
        let si = e >> 4u
        let sc = (ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu
        return float16(dv * float(sc) * (((sgb >> j) & 1u) != 0u ? -gb : gb))
    }
}

class template Iq2xxsCm2T : KqCm2BatchT {
    typedef BLK = VkIq2xxsBlk
    typedef ST = uint
    override IQ2XXSGRID = true
    override DECVEC = false   // probe cm2:iq2xxs: the synthesized twin loses 13-17%; one grid word covers eight weights, the hand-laid twin's case

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2xxsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 8u + l
        let qb = uint(int(unpack8(blk.qs[int(qi >> 1u)])[int(qi & 1u)])) & 0xFFu
        let bl = 7u * l
        let a0i = g * 8u + 4u + (bl >> 3u)
        let b0 = uint(int(unpack8(blk.qs[int(a0i >> 1u)])[int(a0i & 1u)])) & 0xFFu
        let b1 = uint(int(unpack8(blk.qs[int((a0i + 1u) >> 1u)])[int((a0i + 1u) & 1u)])) & 0xFFu
        let sidx = ((b0 | (b1 << 8u)) >> (bl & 7u)) & 127u
        var tt = sidx ^ (sidx >> 4u)
        tt = tt ^ (tt >> 2u)
        tt = tt ^ (tt >> 1u)
        let sgb = sidx | ((tt & 1u) << 7u)
        let gw = iq2xxs_gridc[qb * 2u + (j >> 2u)]
        let gb = float((gw >> ((j & 3u) * 8u)) & 0xFFu)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dv = unpackHalf2x16(ws[srow]).x
        let sc = int(ws[srow + 1u + (g >> 2u)] << ((3u - (g & 3u)) * 8u)) >> 24
        return float16(dv * float(sc) * (((sgb >> j) & 1u) != 0u ? -gb : gb))
    }
}

// the stamps: the three width columns of each format (the dispatcher keys on (fmt, tc))
[vk_dispatch(name = "q8_batch_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2LBatch : Q8Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "q8_batch_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2MBatch : Q8Cm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "q8_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2SBatch : Q8Cm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_k4_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2LBatch : K4Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k4_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2MBatch : K4Cm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k4_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2SBatch : K4Cm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_k6_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2LBatch : K6Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k6_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2MBatch : K6Cm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k6_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2SBatch : K6Cm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_k5_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2LBatch : K5Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k5_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2MBatch : K5Cm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k5_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2SBatch : K5Cm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_q40_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2LBatch : Q40Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_q40_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2MBatch : Q40Cm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_q40_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2SBatch : Q40Cm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2LBatch : Iq4xsCm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2MBatch : Iq4xsCm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2SBatch : Iq4xsCm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_k3_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2LBatch : K3Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k3_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2MBatch : K3Cm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k3_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2SBatch : K3Cm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_iq3s_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2LBatch : Iq3sCm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq3s_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2MBatch : Iq3sCm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq3s_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2SBatch : Iq3sCm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2LBatch : Iq3xxsCm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2MBatch : Iq3xxsCm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2SBatch : Iq3xxsCm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2LBatch : Iq4nlCm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2MBatch : Iq4nlCm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2SBatch : Iq4nlCm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_k2_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2LBatch : K2Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k2_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2MBatch : K2Cm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_k2_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2SBatch : K2Cm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_iq2s_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2LBatch : Iq2sCm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq2s_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2MBatch : Iq2sCm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq2s_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2SBatch : Iq2sCm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2LBatch : Iq2xsCm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2MBatch : Iq2xsCm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2SBatch : Iq2xsCm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2LBatch : Iq2xxsCm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2MBatch : Iq2xxsCm2T {
    override BN = 128u
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2SBatch : Iq2xxsCm2T {
    override BN = 32u
    override STILE = true
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
    typedef FLO = tensorLayout2DPad
}

// ===== the split-k reduce (sums the cm2 partial planes into y) =====

struct SkRedArgs {
    nelem : uint   // one partial plane's float count (cnt x d; always a 4-multiple, d is 32-aligned)
    k : uint       // planes
}

// 4 floats per thread, like the elementwise family
[vk_dispatch(name = "splitk_reduce_cls", grid = "wgs", params = "wgs : int64")]
class SplitKReduce {
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 y : array<float>
    @push_constant pa : SkRedArgs

    [spirv_kernel(local_size_x = 256, name = "splitk_reduce_cls_spv")]
    def run {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            var s0 = 0.0
            var s1 = 0.0
            var s2 = 0.0
            var s3 = 0.0
            var p = 0u
            while (p < pa.k) {
                let b = p * pa.nelem + e
                s0 += part[b]
                s1 += part[b + 1u]
                s2 += part[b + 2u]
                s3 += part[b + 3u]
                p++
            }
            y[e] = s0
            y[e + 1u] = s1
            y[e + 2u] = s2
            y[e + 3u] = s3
        }
    }
}

// ===== rope + KV store + qk-norm =====

struct QkRmsArgs {
    hs : uint
    nh : uint
    nkvh : uint
    qstride : uint
    kstride : uint
    kbase : uint
    qwoff : uint
    kwoff : uint
    eps : float
}

// per-head RMSNorm on q/k between the projections and rope (qk_norm models), one wg per
// head-row, in place
[vk_dispatch(name = "qk_rms_cls", grid = "wgs", params = "wgs : int64")]
class QkRms : RmsWgBase {
    @ssbo @binding = 0 qrows : array<float>   // q rows, normed in place
    @ssbo @binding = 1 krows : array<float>   // k rows, normed in place
    @ssbo @binding = 2 wn : array<float>      // the q/k norm weight rows
    @push_constant pa : QkRmsArgs

    [spirv_kernel(local_size_x = 256, name = "qk_rms_cls_spv")]
    def run {
        let per = pa.nh + pa.nkvh
        let wid = gl_WorkGroupID.x
        let r = wid / per
        let hh = wid % per
        let is_q = hh < pa.nh
        let base = is_q ? r * pa.qstride + hh * pa.hs : pa.kbase + r * pa.kstride + (hh - pa.nh) * pa.hs
        let wbase = is_q ? pa.qwoff : pa.kwoff
        let tid = gl_LocalInvocationID.x
        var ss = 0.0
        if (tid < pa.hs) {
            let v = is_q ? qrows[base + tid] : krows[base + tid]
            ss = v * v
        }
        let inv = wg_rms_inv(ss, pa.hs, pa.eps)
        if (tid < pa.hs) {
            let w = wn[wbase + tid]
            if (is_q) {
                qrows[base + tid] = w * (qrows[base + tid] * inv)
            } else {
                krows[base + tid] = w * (krows[base + tid] * inv)
            }
        }
    }
}

struct RopeKvArgs {
    qd : uint       // query width (the kv rows start here in kvsrc)
    kvd : uint      // kv width (v rows start at qd + kvd)
    hs : uint
    half : uint     // rotation pairs per head
    neox : uint     // pair select: 0 = NORM (2j, 2j+1), else NEOX (j, j+half)
    kpair0 : uint   // first k pair's global index (q pairs come first)
    npairs : uint
    layerbase : uint   // this layer's K/V mirror base; the row rides tok[0].pos
}

// fused rope + KV-row store (decode, one token): one thread per rotation pair, each owning
// 2 distinct K + 2 V slots. KT is the mirror element type; f16 stores clamp +-65504 (the
// metal CLAMP16 contract — an inf in the mirror poisons attention)
[ |> template_struct_instance]
class template RopeKvStoreT {
    @ssbo @binding = 0 qrows : array<float>    // q, roped in place
    @ssbo @binding = 1 kvsrc : array<float>    // the projection buffer: [q | k | v] rows
    @ssbo @binding = 2 kmir : array<KT>        // the K mirror plane
    @ssbo @binding = 3 vmir : array<KT>        // the V mirror plane
    @ssbo @binding = 4 cossin : array<float>   // this position's cos row then sin row
    @ssbo @binding = 5 tok : array<TokMeta>    // the shared per-token block (element 0)
    @push_constant pa : RopeKvArgs
    @template_constant CLAMP16 : bool = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.npairs) {
            let is_k = gid >= pa.kpair0
            let lp = is_k ? gid - pa.kpair0 : gid
            let h = lp / pa.half
            let j = lp % pa.half
            let fcr = cossin[j]
            let fci = cossin[pa.half + j]
            let e0 = pa.neox != 0u ? j : j * 2u
            let e1 = pa.neox != 0u ? j + pa.half : j * 2u + 1u
            if (is_k) {
                let mrow = pa.layerbase + tok[0u].pos * pa.kvd
                let kb = pa.qd + h * pa.hs
                let a0 = kvsrc[kb + e0]
                let a1 = kvsrc[kb + e1]
                // v is not roped — copy the same two elements raw into the V plane
                let vb = pa.qd + pa.kvd + h * pa.hs
                static_if (CLAMP16) {
                    kmir[mrow + h * pa.hs + e0] = float16(clamp(a0 * fcr - a1 * fci, -65504.0, 65504.0))
                    kmir[mrow + h * pa.hs + e1] = float16(clamp(a0 * fci + a1 * fcr, -65504.0, 65504.0))
                    vmir[mrow + h * pa.hs + e0] = float16(clamp(kvsrc[vb + e0], -65504.0, 65504.0))
                    vmir[mrow + h * pa.hs + e1] = float16(clamp(kvsrc[vb + e1], -65504.0, 65504.0))
                } else {
                    kmir[mrow + h * pa.hs + e0] = a0 * fcr - a1 * fci
                    kmir[mrow + h * pa.hs + e1] = a0 * fci + a1 * fcr
                    vmir[mrow + h * pa.hs + e0] = kvsrc[vb + e0]
                    vmir[mrow + h * pa.hs + e1] = kvsrc[vb + e1]
                }
            } else {
                let qb = h * pa.hs
                let a0 = qrows[qb + e0]
                let a1 = qrows[qb + e1]
                qrows[qb + e0] = a0 * fcr - a1 * fci
                qrows[qb + e1] = a0 * fci + a1 * fcr
            }
        }
    }
}

[vk_dispatch(name = "rope_kv_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStore : RopeKvStoreT {
    typedef KT = float
    override CLAMP16 = false
}

// f16-mirror twin — clamped stores
[vk_dispatch(name = "rope_kv_f16_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStoreF16 : RopeKvStoreT {
    typedef KT = float16
}

struct RopeKvBArgs {
    qd : uint
    kvd : uint
    hs : uint
    half : uint
    neox : uint
    kpair0 : uint    // first k pair's per-position index
    ppp : uint       // rotation pairs per position
    layerbase : uint // this layer's K/V mirror base
    npos : uint
    pos0 : uint      // the window's first absolute position
    voff : uint
    kstride : uint
}

// batched rope + KV-store (prefill, npos positions): q roped in place, k/v from the projection
// buffer into the mirrors at absolute positions. KT/CLAMP16 as in RopeKvStoreT
[ |> template_struct_instance]
class template RopeKvStoreBT {
    @ssbo @binding = 0 qrows : array<float>    // q rows, roped in place
    @ssbo @binding = 1 kvrows : array<float>   // k rows at 0, v rows at voff
    @ssbo @binding = 2 kmir : array<KT>
    @ssbo @binding = 3 vmir : array<KT>
    @ssbo @binding = 4 cossin : array<float>   // per-position cos row then sin row (hs floats each)
    @push_constant pa : RopeKvBArgs
    @template_constant CLAMP16 : bool = true

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
    def run {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.npos * pa.ppp) {
            let p = gid / pa.ppp
            let lp = gid % pa.ppp
            let is_k = lp >= pa.kpair0
            let pr = is_k ? lp - pa.kpair0 : lp
            let h = pr / pa.half
            let j = pr % pa.half
            let cb = p * pa.hs
            let fcr = cossin[cb + j]
            let fci = cossin[cb + pa.half + j]
            let e0 = pa.neox != 0u ? j : j * 2u
            let e1 = pa.neox != 0u ? j + pa.half : j * 2u + 1u
            let apos = pa.pos0 + p
            if (is_k) {
                let kb = p * pa.kstride + h * pa.hs
                let a0 = kvrows[kb + e0]
                let a1 = kvrows[kb + e1]
                let mo = pa.layerbase + apos * pa.kvd + h * pa.hs
                let vb = pa.voff + p * pa.kstride + h * pa.hs
                static_if (CLAMP16) {
                    kmir[mo + e0] = float16(clamp(a0 * fcr - a1 * fci, -65504.0, 65504.0))
                    kmir[mo + e1] = float16(clamp(a0 * fci + a1 * fcr, -65504.0, 65504.0))
                    vmir[mo + e0] = float16(clamp(kvrows[vb + e0], -65504.0, 65504.0))
                    vmir[mo + e1] = float16(clamp(kvrows[vb + e1], -65504.0, 65504.0))
                } else {
                    kmir[mo + e0] = a0 * fcr - a1 * fci
                    kmir[mo + e1] = a0 * fci + a1 * fcr
                    vmir[mo + e0] = kvrows[vb + e0]
                    vmir[mo + e1] = kvrows[vb + e1]
                }
            } else {
                let qb = p * pa.qd + h * pa.hs
                let a0 = qrows[qb + e0]
                let a1 = qrows[qb + e1]
                qrows[qb + e0] = a0 * fcr - a1 * fci
                qrows[qb + e1] = a0 * fci + a1 * fcr
            }
        }
    }
}

[vk_dispatch(name = "rope_kv_b_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStoreB : RopeKvStoreBT {
    typedef KT = float
    override CLAMP16 = false
}

// f16-mirror twin — clamped stores
[vk_dispatch(name = "rope_kv_b_f16_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStoreBF16 : RopeKvStoreBT {
    typedef KT = float16
}

struct QknRopeArgs {
    qd : uint
    kvd : uint
    hs : uint
    half : uint
    neox : uint
    nh : uint       // q heads (wgs >= nh are k heads)
    layerbase : uint   // this layer's K/V mirror base; the row rides tok[0].pos
    qwoff : uint    // q norm-weight row base in wn
    kwoff : uint
    eps : float
}

// qk_rms + rope_kv_store in ONE dispatch (qk_norm models), one wg per head-row; verbatim
// reduce + rotation order, so bit-identical to the split pair. KT/CLAMP16 as in RopeKvStoreT
[ |> template_struct_instance]
class template QknRopeKvT : RmsWgBase {
    @ssbo @binding = 0 qrows : array<float>    // q rows, normed + roped in place
    @ssbo @binding = 1 kvsrc : array<float>    // the projection buffer: [q | k | v] rows
    @ssbo @binding = 2 kmir : array<KT>
    @ssbo @binding = 3 vmir : array<KT>
    @ssbo @binding = 4 cossin : array<float>
    @ssbo @binding = 5 wn : array<float>       // the q/k norm weight rows (its OWN member now —
                                               // the old kernel packed them into the cos buffer)
    @ssbo @binding = 6 tok : array<TokMeta>    // the shared per-token block (element 0)
    @push_constant pa : QknRopeArgs
    @workgroup row : float[256]                // this head's normed q or k row (hs <= 256)
    @template_constant CLAMP16 : bool = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        let wid = gl_WorkGroupID.x
        let is_k = wid >= pa.nh
        let h = is_k ? wid - pa.nh : wid
        let tid = gl_LocalInvocationID.x
        let sbase = is_k ? pa.qd + h * pa.hs : h * pa.hs
        var v = 0.0
        if (tid < pa.hs) {
            v = is_k ? kvsrc[sbase + tid] : qrows[sbase + tid]
        }
        let ss = tid < pa.hs ? v * v : 0.0
        let inv = wg_rms_inv(ss, pa.hs, pa.eps)
        if (tid < pa.hs) {
            row[tid] = wn[(is_k ? pa.kwoff : pa.qwoff) + tid] * (v * inv)
        }
        barrier()
        // rope this head's pairs off the normed row; the pairs partition [0, hs)
        if (tid < pa.half) {
            let fcr = cossin[tid]
            let fci = cossin[pa.half + tid]
            let e0 = pa.neox != 0u ? tid : tid * 2u
            let e1 = pa.neox != 0u ? tid + pa.half : tid * 2u + 1u
            let a0 = row[e0]
            let a1 = row[e1]
            let r0 = a0 * fcr - a1 * fci
            let r1 = a0 * fci + a1 * fcr
            if (is_k) {
                let mrow = pa.layerbase + tok[0u].pos * pa.kvd
                static_if (CLAMP16) {
                    kmir[mrow + h * pa.hs + e0] = float16(clamp(r0, -65504.0, 65504.0))
                    kmir[mrow + h * pa.hs + e1] = float16(clamp(r1, -65504.0, 65504.0))
                } else {
                    kmir[mrow + h * pa.hs + e0] = r0
                    kmir[mrow + h * pa.hs + e1] = r1
                }
            } else {
                qrows[h * pa.hs + e0] = r0
                qrows[h * pa.hs + e1] = r1
            }
        }
        // v is not roped or normed — the k-head wgs copy their own raw row into the V plane
        if (is_k && tid < pa.hs) {
            let mrow = pa.layerbase + tok[0u].pos * pa.kvd
            static_if (CLAMP16) {
                vmir[mrow + h * pa.hs + tid] = float16(clamp(kvsrc[pa.qd + pa.kvd + h * pa.hs + tid], -65504.0, 65504.0))
            } else {
                vmir[mrow + h * pa.hs + tid] = kvsrc[pa.qd + pa.kvd + h * pa.hs + tid]
            }
        }
    }
}

[vk_dispatch(name = "qkn_rope_cls", grid = "wgs", params = "wgs : int64")]
class QknRopeKv : QknRopeKvT {
    typedef KT = float
    override CLAMP16 = false
}

// f16-mirror twin — clamped stores
[vk_dispatch(name = "qkn_rope_f16_cls", grid = "wgs", params = "wgs : int64")]
class QknRopeKvF16 : QknRopeKvT {
    typedef KT = float16
}

// ===== the region-GEMV push params (every decode GEMV shares the shape) =====

struct GemvArgs {
    n : uint       // reduction dim
    d : uint       // output rows per region
    nreg : uint    // region count
    ybase : uint   // output base row
}

// ===== the Q8_0 decode GEMV pair =====

// q8_moe_groupn's class twin: TWO lanes per 32-block (a uint4 each), xor-1 fold, even lane folds
// the scales. The workhorse decode GEMV.
[vk_dispatch(name = "q8_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q8Gemv {
    @ssbo @binding = 0 wq4 : array<uint4>      // weight quant words, vec4 view (16B loads)
    @ssbo @binding = 1 wsh : array<float16>    // per-block f16 weight scales
    @ssbo @binding = 2 regions : array<uint>   // (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q8_gemv_cls_spv")]
    def run {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.nreg * pa.d) {
            let r = rg / pa.d
            let row = rg % pa.d
            let nb = pa.n / 32u
            let wb0 = regions[r * 2u] + row * nb
            let xb0 = regions[r * 2u + 1u]
            let lane = gl_SubgroupInvocationID
            let bpp = gl_SubgroupSize / 2u
            var acc = 0.0
            var bi0 = 0u
            while (bi0 < nb) {
                let b = bi0 + lane / 2u
                if (b < nb) {
                    let wv = wq4[(wb0 + bi0) * 2u + lane]
                    let xbase = (xb0 + bi0) * 8u + lane * 4u
                    var idot = sdot4(wv.x, xqw[xbase])
                    idot += sdot4(wv.y, xqw[xbase + 1u])
                    idot += sdot4(wv.z, xqw[xbase + 2u])
                    idot += sdot4(wv.w, xqw[xbase + 3u])
                    idot += subgroupShuffleXor(idot, 1u)
                    if (lane % 2u == 0u) {
                        acc += float(idot) * float(wsh[wb0 + b]) * axs[xb0 + b]
                    }
                }
                bi0 += bpp
            }
            let total = subgroupAdd(acc)
            if (subgroupElect()) {
                y[pa.ybase + rg] = total
            }
        }
    }
}

// ===== the K-quant decode GEMV family: one base shell, per-format block contributions =====
// ONE LANE PER 32-BLOCK via uint4 loads (512B/warp in flight, one scale decode per block).
// The four formats share the region walk, the lane->block map, and the subgroup fold — only
// the block's dot + scale fold differ, so each format is an override of blk_contrib and the
// devirtualizer inlines the family back to four standalone kernels at emission.

let private KQC_LOW = 0x0F0F0F0Fu   // nibble mask
let private KQC_ONES = 0x01010101u  // sdot4 against this sums 4 signed bytes

class KqGemvBase {
    @ssbo @binding = 0 @role = "alias" wq : array<uint>   // weight quant words (k5's qh rides this view)
    @ssbo @binding = 0 wq4 : array<uint4>      // vec4 view of the same words
    @ssbo @binding = 1 wsu : array<uint>       // scale planes ([f16 d][f16 dmin][8 sc][8 mn] / q40 half pairs / k6 int8+d / iq4xs d + 8 signed sc)
    @ssbo @binding = 2 regions : array<uint>   // (rel_wblock, xblock) pairs per region — bulk data, stays SSBO
    @ssbo @binding = 3 xq4 : array<uint4>      // activation quant words, vec4 view
    @ssbo @binding = 4 axs : array<float>      // activation block scales
    @ssbo @binding = 5 y : array<float>        // output rows
    @push_constant pa : GemvArgs

    // per-format: one 32-block's scaled contribution — every [spirv_kernel] variant overrides
    def blk_contrib(wsb, blk, xsb : uint) : float {
        return 0.0
    }

    // a block's activation sum (the min/offset terms fold against it)
    def blk_sum(xl, xh : uint4) : int {
        let bs = sdot4(xl.x, KQC_ONES) + sdot4(xl.y, KQC_ONES) + sdot4(xl.z, KQC_ONES) + sdot4(xl.w, KQC_ONES)
        return bs + sdot4(xh.x, KQC_ONES) + sdot4(xh.y, KQC_ONES) + sdot4(xh.z, KQC_ONES) + sdot4(xh.w, KQC_ONES)
    }

    // a 4-bit block's dot: weight word j's LOW nibbles hit x word j, its HIGH nibbles x word j+4
    def blk_dot4(wv, xl, xh : uint4) : int {
        var idot = sdot4(wv.x & KQC_LOW, xl.x) + sdot4(wv.y & KQC_LOW, xl.y)
        idot += sdot4(wv.z & KQC_LOW, xl.z) + sdot4(wv.w & KQC_LOW, xl.w)
        idot += sdot4((wv.x >> 4u) & KQC_LOW, xh.x) + sdot4((wv.y >> 4u) & KQC_LOW, xh.y)
        return idot + sdot4((wv.z >> 4u) & KQC_LOW, xh.z) + sdot4((wv.w >> 4u) & KQC_LOW, xh.w)
    }

    // the Q4_K/Q5_K per-block scale fold: d*sc*idot - dmin*mn*bsum, int products kept exact
    def dm_fold(wsb, blk : uint; idot, bs : int) : float {
        let dm = unpackHalf2x16(wsu[wsb * 5u])
        let sc = int((wsu[wsb * 5u + 1u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFF)
        let mn = int((wsu[wsb * 5u + 3u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFF)
        return dm.x * float(sc * idot) - dm.y * float(mn * bs)
    }

    def gemv_shell {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.nreg * pa.d) {
            let r = rg / pa.d
            let row = rg % pa.d
            let nsb = pa.n / 256u
            let wsb0 = regions[r * 2u] + row * nsb
            let xsb0 = regions[r * 2u + 1u]
            let lane = gl_SubgroupInvocationID
            let nb = nsb * 8u
            var acc = 0.0
            var b0 = 0u
            while (b0 < nb) {
                let bb = b0 + lane
                if (bb < nb) {
                    acc += blk_contrib(wsb0 + bb / 8u, bb % 8u, xsb0 + bb / 8u)
                }
                b0 += gl_SubgroupSize
            }
            let total = subgroupAdd(acc)
            if (subgroupElect()) {
                y[pa.ybase + rg] = total
            }
        }
    }
}

// ===== the q8 batch tile router (class side) =====
// The (mode, tile, aligned) pick as a variant ordinal — all six classes
// share the 6-buffer member layout + BatchArgs, but each carries its own set layout.
// variant: 0 = sdot4 tile, 1 = cmf16, 2 = cmi8, 3 = mm M-tile, 4 = mm aligned-L, 5 = mm L

def q8_batch_cls_variant(tile : int64; aligned : bool) : int {
    if (g_gpu.coopmat_mode == 1) {
        return 1
    } elif (g_gpu.coopmat_mode == 2) {
        return 2
    } elif (g_gpu.coopmat_mode >= 3) {
        if (tile == BATCH_TILE) {   // the small tier IS the sdot4 kernel — measured, not a fallback
            return 0
        }
        return tile == 64l ? 3 : (aligned ? 4 : 5)
    }
    return 0
}

def q8_batch_cls_ensure(v : int) : bool {
    if (v == 1) {
        return ensure_q8_batch_cmf16_cls()
    } elif (v == 2) {
        return ensure_q8_batch_cmi8_cls()
    } elif (v == 3) {
        return ensure_q8_batch_mm_m_cls()
    } elif (v == 4) {
        return ensure_q8_batch_mm_a_cls()
    } elif (v == 5) {
        return ensure_q8_batch_mm_cls()
    } elif (v == 0) {
        return ensure_q8_batch_cls()
    }
    panic("dasLLAMA vulkan tier: no q8 batch class for variant {v}")
    return false
}

def q8_batch_cls_set(v : int; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (v == 1) {
        return set_q8_batch_cmf16_cls(bufs, sizes, gbits)
    } elif (v == 2) {
        return set_q8_batch_cmi8_cls(bufs, sizes, gbits)
    } elif (v == 3) {
        return set_q8_batch_mm_m_cls(bufs, sizes, gbits)
    } elif (v == 4) {
        return set_q8_batch_mm_a_cls(bufs, sizes, gbits)
    } elif (v == 5) {
        return set_q8_batch_mm_cls(bufs, sizes, gbits)
    } elif (v == 0) {
        return set_q8_batch_cls(bufs, sizes, gbits)
    }
    panic("dasLLAMA vulkan tier: no q8 batch class for variant {v}")
    return VkDescriptorSet()
}

def q8_batch_cls_enc(v : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : BatchArgs; wgs : int64) {
    if (v == 1) {
        enc_q8_batch_cmf16_cls(raw, h, set_, pc, wgs)
    } elif (v == 2) {
        enc_q8_batch_cmi8_cls(raw, h, set_, pc, wgs)
    } elif (v == 3) {
        enc_q8_batch_mm_m_cls(raw, h, set_, pc, wgs)
    } elif (v == 4) {
        enc_q8_batch_mm_a_cls(raw, h, set_, pc, wgs)
    } elif (v == 5) {
        enc_q8_batch_mm_cls(raw, h, set_, pc, wgs)
    } elif (v == 0) {
        enc_q8_batch_cls(raw, h, set_, pc, wgs)
    } else {
        panic("dasLLAMA vulkan tier: no q8 batch class for variant {v}")
    }
}

// ===== the kq batch tile router (class side) =====
// The family shares one set layout ("kq_batch_cls"); the q40 f16-MMA twin (coopmat_mode 1 —
// no s8-native for nibbles) carries its own. The stack and resident-prefill encoders share it.

def kq_batch_cls_ensure(fmt : int; q40cm : bool) : bool {
    if (q40cm) {
        return ensure_kq_batch_q40_cmf16_cls()
    } elif (fmt == int(KqFmt.k4)) {
        return ensure_kq_batch_k4_cls()
    } elif (fmt == int(KqFmt.k5)) {
        return ensure_kq_batch_k5_cls()
    } elif (fmt == int(KqFmt.k6)) {
        return ensure_kq_batch_k6_cls()
    } elif (fmt == int(KqFmt.q40)) {
        return ensure_kq_batch_q40_cls()
    } elif (fmt == int(KqFmt.iq4xs)) {
        return ensure_kq_batch_iq4xs_cls()
    } elif (fmt == int(KqFmt.k3)) {
        return ensure_kq_batch_k3_cls()
    } elif (fmt == int(KqFmt.iq3s)) {
        return ensure_kq_batch_iq3s_cls()
    } elif (fmt == int(KqFmt.iq3xxs)) {
        return ensure_kq_batch_iq3xxs_cls()
    } elif (fmt == int(KqFmt.iq4nl)) {
        return ensure_kq_batch_iq4nl_cls()
    } elif (fmt == int(KqFmt.k2)) {
        return ensure_kq_batch_k2_cls()
    } elif (fmt == int(KqFmt.iq2s)) {
        return ensure_kq_batch_iq2s_cls()
    } elif (fmt == int(KqFmt.iq2xs)) {
        return ensure_kq_batch_iq2xs_cls()
    } elif (fmt == int(KqFmt.iq2xxs)) {
        return ensure_kq_batch_iq2xxs_cls()
    }
    panic("dasLLAMA vulkan tier: no kq batch class for format {fmt}")
    return false
}

def kq_batch_cls_set_for(fmt : int; q40cm : bool; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (q40cm) {
        return set_kq_batch_q40_cmf16_cls(bufs, sizes, gbits)
    }
    if (!kq_sb(fmt)) {
        panic("dasLLAMA vulkan tier: no kq batch class for format {fmt}")
    }
    return set_kq_batch_cls(bufs, sizes, gbits)
}

def kq_batch_cls_enc_for(fmt : int; q40cm : bool; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : BatchArgs; wgs : int64) {
    if (q40cm) {
        enc_kq_batch_q40_cmf16_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k4)) {
        enc_kq_batch_k4_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k5)) {
        enc_kq_batch_k5_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k6)) {
        enc_kq_batch_k6_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.q40)) {
        enc_kq_batch_q40_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq4xs)) {
        enc_kq_batch_iq4xs_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k3)) {
        enc_kq_batch_k3_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq3s)) {
        enc_kq_batch_iq3s_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq3xxs)) {
        enc_kq_batch_iq3xxs_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq4nl)) {
        enc_kq_batch_iq4nl_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k2)) {
        enc_kq_batch_k2_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2s)) {
        enc_kq_batch_iq2s_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2xs)) {
        enc_kq_batch_iq2xs_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2xxs)) {
        enc_kq_batch_iq2xxs_cls(raw, h, set_, pc, wgs)
    } else {
        panic("dasLLAMA vulkan tier: no kq batch class for format {fmt}")
    }
}

// ===== format-dispatched class-GEMV rail =====
// The drivers' format pick as ensure/set/enc: every decode GEMV class
// shares the 6-buffer shape (wq, ws, regions, xq, xs, y) and GemvArgs.

def gemv_cls_ensure(fmt : int) : bool {
    if (fmt == int(KqFmt.q8)) {
        return ensure_q8_gemv_cls()
    } elif (fmt == int(KqFmt.k4)) {
        return ensure_kq_gemv_k4_cls()
    } elif (fmt == int(KqFmt.k5)) {
        return ensure_kq_gemv_k5_cls()
    } elif (fmt == int(KqFmt.k6)) {
        return ensure_kq_gemv_k6_cls()
    } elif (fmt == int(KqFmt.q40)) {
        return ensure_kq_gemv_q40_cls()
    } elif (fmt == int(KqFmt.iq4xs)) {
        return ensure_kq_gemv_iq4xs_cls()
    } elif (fmt == int(KqFmt.k3)) {
        return ensure_kq_gemv_k3_cls()
    } elif (fmt == int(KqFmt.iq3s)) {
        return ensure_kq_gemv_iq3s_cls()
    } elif (fmt == int(KqFmt.iq3xxs)) {
        return ensure_kq_gemv_iq3xxs_cls()
    } elif (fmt == int(KqFmt.iq4nl)) {
        return ensure_kq_gemv_iq4nl_cls()
    } elif (fmt == int(KqFmt.k2)) {
        return ensure_kq_gemv_k2_cls()
    } elif (fmt == int(KqFmt.iq2s)) {
        return ensure_kq_gemv_iq2s_cls()
    } elif (fmt == int(KqFmt.iq2xs)) {
        return ensure_kq_gemv_iq2xs_cls()
    } elif (fmt == int(KqFmt.iq2xxs)) {
        return ensure_kq_gemv_iq2xxs_cls()
    }
    panic("dasLLAMA vulkan tier: no GEMV class for format {fmt}")
    return false
}

def gemv_cls_set(fmt : int; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (fmt == int(KqFmt.q8)) {
        return set_q8_gemv_cls(bufs, sizes, gbits)
    } elif (kq_sb(fmt)) {
        return set_kq_gemv_cls(bufs, sizes, gbits)   // the family shares one layout — one set serves every kq format
    }
    panic("dasLLAMA vulkan tier: no GEMV class for format {fmt}")
    return VkDescriptorSet()
}

def gemv_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {
    if (fmt == int(KqFmt.q8)) {
        enc_q8_gemv_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k4)) {
        enc_kq_gemv_k4_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k5)) {
        enc_kq_gemv_k5_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k6)) {
        enc_kq_gemv_k6_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.q40)) {
        enc_kq_gemv_q40_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq4xs)) {
        enc_kq_gemv_iq4xs_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k3)) {
        enc_kq_gemv_k3_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq3s)) {
        enc_kq_gemv_iq3s_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq3xxs)) {
        enc_kq_gemv_iq3xxs_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq4nl)) {
        enc_kq_gemv_iq4nl_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k2)) {
        enc_kq_gemv_k2_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2s)) {
        enc_kq_gemv_iq2s_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2xs)) {
        enc_kq_gemv_iq2xs_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2xxs)) {
        enc_kq_gemv_iq2xxs_cls(raw, h, set_, pc, wgs)
    } else {
        panic("dasLLAMA vulkan tier: no GEMV class for format {fmt}")
    }
}

// Q4_K: w = d*sc*q - dmin*mn*1, q in [0,15] — per 32-block axs*(d*sc*idot - dmin*mn*bsum).
// Every kq format shares family=kq_gemv_cls: ONE set layout + ONE set_kq_gemv_cls serves every
// pipeline (identical inherited members), each leaf keeping its own ensure_/enc_ names.
[vk_dispatch(name = "kq_gemv_k4_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK4 : KqGemvBase {
    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        let idot = blk_dot4(wq4[wsb * 8u + blk], xl, xh)
        return axs[xsb] * dm_fold(wsb, blk, idot, blk_sum(xl, xh))
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_k4_cls_spv")]
    def run {
        gemv_shell()
    }
}

// Q4_0: w = d*(q - 8) — the k4 nibble tiling with no min plane; -8 folds against the block sum
[vk_dispatch(name = "kq_gemv_q40_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvQ40 : KqGemvBase {
    def q40_d(wsb, blk : uint) : float {
        let dp = unpackHalf2x16(wsu[wsb * 5u + blk / 2u])
        return blk % 2u == 0u ? dp.x : dp.y
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        let idot = blk_dot4(wq4[wsb * 8u + blk], xl, xh)
        return axs[xsb] * q40_d(wsb, blk) * float(idot - 8 * blk_sum(xl, xh))
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_q40_cls_spv")]
    def run {
        gemv_shell()
    }
}

// IQ4_XS: w = d*sc*LUT[q] — the q40 nibble tiling decoded through the codebook into signed
// lanes (no block-sum term), the k4 scale-row shape with SIGNED (ls - 32) sub-scales and no min
[vk_dispatch(name = "kq_gemv_iq4xs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4xs : KqGemvBase {
    def iq4_dot(wv, xl, xh : uint4) : int {
        var idot = sdot4(iq4_word(wv.x & KQC_LOW), xl.x) + sdot4(iq4_word(wv.y & KQC_LOW), xl.y)
        idot += sdot4(iq4_word(wv.z & KQC_LOW), xl.z) + sdot4(iq4_word(wv.w & KQC_LOW), xl.w)
        idot += sdot4(iq4_word((wv.x >> 4u) & KQC_LOW), xh.x) + sdot4(iq4_word((wv.y >> 4u) & KQC_LOW), xh.y)
        return idot + sdot4(iq4_word((wv.z >> 4u) & KQC_LOW), xh.z) + sdot4(iq4_word((wv.w >> 4u) & KQC_LOW), xh.w)
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let idot = iq4_dot(wq4[wsb * 8u + blk], xq4[xb4], xq4[xb4 + 1u])
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return axs[xsb] * dd * float(iq4_sc(wsu[wsb * 5u + 1u + blk / 4u], blk) * idot)
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_iq4xs_cls_spv")]
    def run {
        gemv_shell()
    }
}

// IQ3_S: grid gather (8-bit index | qh ninth bit -> 4 magnitudes per word), the block's own
// sign bytes flip them, per-32 signed (1 + 2s) sub-scale x d - iq4xs's fold (no bsum term)
// over the 26-word rows; the 2 KB grid stages into workgroup memory ahead of the shell.
[vk_dispatch(name = "kq_gemv_iq3s_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3s : KqGemvBase {
    @workgroup gridw : uint[512]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let qb = wsb * 26u + blk * 2u
        let qh = (wq[wsb * 26u + 16u + blk / 4u] >> ((blk % 4u) * 8u)) & 255u
        let sgw = wq[wsb * 26u + 18u + blk]
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        var isum = 0
        for (l in range(2)) {
            let qsw = wq[qb + uint(l)]
            let l0 = uint(l) * 2u
            let sg0 = (sgw >> (l0 * 8u)) & 255u
            let sg1 = (sgw >> (l0 * 8u + 8u)) & 255u
            let w0 = iq3s_signed(gridw[(qsw & 255u) | ((qh << (8u - 2u * l0)) & 256u)], sg0 & 15u)
            let w1 = iq3s_signed(gridw[((qsw >> 8u) & 255u) | ((qh << (7u - 2u * l0)) & 256u)], sg0 >> 4u)
            let w2 = iq3s_signed(gridw[((qsw >> 16u) & 255u) | ((qh << (8u - 2u * (l0 + 1u))) & 256u)], sg1 & 15u)
            let w3 = iq3s_signed(gridw[(qsw >> 24u) | ((qh << (7u - 2u * (l0 + 1u))) & 256u)], sg1 >> 4u)
            if (l == 0) {
                isum += sdot4(w0, xl.x) + sdot4(w1, xl.y) + sdot4(w2, xl.z) + sdot4(w3, xl.w)
            } else {
                isum += sdot4(w0, xh.x) + sdot4(w1, xh.y) + sdot4(w2, xh.z) + sdot4(w3, xh.w)
            }
        }
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return axs[xsb] * dd * float(iq4_sc(wsu[wsb * 5u + 1u + blk / 4u], blk) * isum)
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_iq3s_cls_spv")]
    def run {
        let lid = gl_LocalInvocationID.x
        for (k in range(8)) {
            let idx = lid + uint(k) * 64u
            gridw[idx] = iq3s_grid_word(int(idx))
        }
        barrier()   // the grid visible before the shell's block walk
        gemv_shell()
    }
}

// IQ3_XXS: halved-grid gather (bare 8-bit index -> 4 magnitudes per word), signs from the
// aux32's four 7-bit ksigns indices (bit 7 = parity, computed - no table), per-32 (2ls + 1)
// sub-scale x halved d - iq3s's fold exactly, over the 24-word rows; the 1 KB grid stages
// into workgroup memory ahead of the shell.
[vk_dispatch(name = "kq_gemv_iq3xxs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3xxs : KqGemvBase {
    @workgroup gridw : uint[256]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let qb = wsb * 24u + blk * 2u
        let aux = wq[wsb * 24u + 16u + blk]
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        var isum = 0
        for (l in range(2)) {
            let qsw = wq[qb + uint(l)]
            let l0 = uint(l) * 2u
            let sg0 = ksign7((aux >> (7u * l0)) & 127u)
            let sg1 = ksign7((aux >> (7u * l0 + 7u)) & 127u)
            let w0 = iq3s_signed(gridw[qsw & 255u], sg0 & 15u)
            let w1 = iq3s_signed(gridw[(qsw >> 8u) & 255u], sg0 >> 4u)
            let w2 = iq3s_signed(gridw[(qsw >> 16u) & 255u], sg1 & 15u)
            let w3 = iq3s_signed(gridw[qsw >> 24u], sg1 >> 4u)
            if (l == 0) {
                isum += sdot4(w0, xl.x) + sdot4(w1, xl.y) + sdot4(w2, xl.z) + sdot4(w3, xl.w)
            } else {
                isum += sdot4(w0, xh.x) + sdot4(w1, xh.y) + sdot4(w2, xh.z) + sdot4(w3, xh.w)
            }
        }
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return axs[xsb] * dd * float(iq4_sc(wsu[wsb * 5u + 1u + blk / 4u], blk) * isum)
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_iq3xxs_cls_spv")]
    def run {
        let lid = gl_LocalInvocationID.x
        for (k in range(4)) {
            let idx = lid + uint(k) * 64u
            gridw[idx] = iq3xxs_grid_word(int(idx))
        }
        barrier()   // the grid visible before the shell's block walk
        gemv_shell()
    }
}

// IQ4_NL: iq4xs's codebook dot (iq4_word, pure ALU) folded by q40's per-block f16 d - no
// sub-scales, no bsum term; the rows are q40's 32-word planes verbatim.
[vk_dispatch(name = "kq_gemv_iq4nl_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4nl : KqGemvBase {
    def iq4_dot(wv, xl, xh : uint4) : int {
        var idot = sdot4(iq4_word(wv.x & KQC_LOW), xl.x) + sdot4(iq4_word(wv.y & KQC_LOW), xl.y)
        idot += sdot4(iq4_word(wv.z & KQC_LOW), xl.z) + sdot4(iq4_word(wv.w & KQC_LOW), xl.w)
        idot += sdot4(iq4_word((wv.x >> 4u) & KQC_LOW), xh.x) + sdot4(iq4_word((wv.y >> 4u) & KQC_LOW), xh.y)
        return idot + sdot4(iq4_word((wv.z >> 4u) & KQC_LOW), xh.z) + sdot4(iq4_word((wv.w >> 4u) & KQC_LOW), xh.w)
    }

    def q40_d(wsb, blk : uint) : float {
        let dp = unpackHalf2x16(wsu[wsb * 5u + blk / 2u])
        return blk % 2u == 0u ? dp.x : dp.y
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let idot = iq4_dot(wq4[wsb * 8u + blk], xq4[xb4], xq4[xb4 + 1u])
        return axs[xsb] * q40_d(wsb, blk) * float(idot)
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_iq4nl_cls_spv")]
    def run {
        gemv_shell()
    }
}

// Q3_K: q = 2-bit qs lane | hmask bit << 2, minus 4, per-16 SIGNED sub-scales x d — k6's fold with
// the offset 4 over the k3 planes (6 uint4 per superblock: qs 0..3, hmask 4..5)
[vk_dispatch(name = "kq_gemv_k3_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK3 : KqGemvBase {
    // (ilo, ihi, blo, bhi) — an int4, since a shader function cannot return a tuple
    def k3_dot(ql, qh, xl, xh, hl, hh : uint4; sh, bit : uint) : int4 {
        let m2 = 0x03030303u
        let m1 = 0x01010101u
        let l0 = ((ql.x >> sh) & m2) | (((hl.x >> bit) & m1) << 2u)
        let l1 = ((ql.y >> sh) & m2) | (((hl.y >> bit) & m1) << 2u)
        let l2 = ((ql.z >> sh) & m2) | (((hl.z >> bit) & m1) << 2u)
        let l3 = ((ql.w >> sh) & m2) | (((hl.w >> bit) & m1) << 2u)
        let h0 = ((qh.x >> sh) & m2) | (((hh.x >> bit) & m1) << 2u)
        let h1 = ((qh.y >> sh) & m2) | (((hh.y >> bit) & m1) << 2u)
        let h2 = ((qh.z >> sh) & m2) | (((hh.z >> bit) & m1) << 2u)
        let h3 = ((qh.w >> sh) & m2) | (((hh.w >> bit) & m1) << 2u)
        return int4(sdot4(l0, xl.x) + sdot4(l1, xl.y) + sdot4(l2, xl.z) + sdot4(l3, xl.w),
                    sdot4(h0, xh.x) + sdot4(h1, xh.y) + sdot4(h2, xh.z) + sdot4(h3, xh.w),
                    sdot4(xl.x, KQC_ONES) + sdot4(xl.y, KQC_ONES) + sdot4(xl.z, KQC_ONES) + sdot4(xl.w, KQC_ONES),
                    sdot4(xh.x, KQC_ONES) + sdot4(xh.y, KQC_ONES) + sdot4(xh.z, KQC_ONES) + sdot4(xh.w, KQC_ONES))
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let q4 = wsb * 6u + (blk / 4u) * 2u
        let dot = k3_dot(wq4[q4], wq4[q4 + 1u], xq4[xb4], xq4[xb4 + 1u],
            wq4[wsb * 6u + 4u], wq4[wsb * 6u + 5u], (blk % 4u) * 2u, blk)
        let s4 = int4(unpack8(int(wsu[wsb * 5u + blk / 2u])))   // 4 signed sub-scales
        let slo = (blk % 2u) == 0u ? s4.x : s4.z
        let shi = (blk % 2u) == 0u ? s4.y : s4.w
        let dd = unpackHalf2x16(wsu[wsb * 5u + 4u]).x
        return axs[xsb] * dd * float(slo * (dot.x - 4 * dot.z) + shi * (dot.y - 4 * dot.w))
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_k3_cls_spv")]
    def run {
        gemv_shell()
    }
}

// Q2_K: q = the unsigned 2-bit qs lane, per-16 sc/min NIBBLE pairs x d/dmin - the k4 d|dmin
// fold at k6's granularity over the 16-word rows (4 uint4 of qs; scale row [d|dmin][16 pair bytes])
[vk_dispatch(name = "kq_gemv_k2_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK2 : KqGemvBase {
    // (ilo, ihi, blo, bhi) - an int4, since a shader function cannot return a tuple
    def k2_dot(ql, qh, xl, xh : uint4; sh : uint) : int4 {
        let m2 = 0x03030303u
        let l0 = (ql.x >> sh) & m2
        let l1 = (ql.y >> sh) & m2
        let l2 = (ql.z >> sh) & m2
        let l3 = (ql.w >> sh) & m2
        let h0 = (qh.x >> sh) & m2
        let h1 = (qh.y >> sh) & m2
        let h2 = (qh.z >> sh) & m2
        let h3 = (qh.w >> sh) & m2
        return int4(sdot4(l0, xl.x) + sdot4(l1, xl.y) + sdot4(l2, xl.z) + sdot4(l3, xl.w),
                    sdot4(h0, xh.x) + sdot4(h1, xh.y) + sdot4(h2, xh.z) + sdot4(h3, xh.w),
                    sdot4(xl.x, KQC_ONES) + sdot4(xl.y, KQC_ONES) + sdot4(xl.z, KQC_ONES) + sdot4(xl.w, KQC_ONES),
                    sdot4(xh.x, KQC_ONES) + sdot4(xh.y, KQC_ONES) + sdot4(xh.z, KQC_ONES) + sdot4(xh.w, KQC_ONES))
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let q4 = wsb * 4u + (blk / 4u) * 2u
        let dot = k2_dot(wq4[q4], wq4[q4 + 1u], xq4[xb4], xq4[xb4 + 1u], (blk % 4u) * 2u)
        let w = wsu[wsb * 5u + 1u + blk / 2u]
        let b0 = (w >> ((blk % 2u) * 16u)) & 255u
        let b1 = (w >> ((blk % 2u) * 16u + 8u)) & 255u
        let dm = unpackHalf2x16(wsu[wsb * 5u])
        return axs[xsb] * (dm.x * float(int(b0 & 15u) * dot.x + int(b1 & 15u) * dot.y)
            - dm.y * float(int(b0 >> 4u) * dot.z + int(b1 >> 4u) * dot.w))
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_k2_cls_spv")]
    def run {
        gemv_shell()
    }
}

// Q5_K: the k4 fold with the 5th bit deposited from the block's qh word
[vk_dispatch(name = "kq_gemv_k5_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK5 : KqGemvBase {
    def k5_dot(wv, xl, xh : uint4; qh : uint) : int {
        let h0 = qh & 0xFFu
        let h1 = (qh >> 8u) & 0xFFu
        let h2 = (qh >> 16u) & 0xFFu
        let h3 = (qh >> 24u) & 0xFFu
        var idot = sdot4((wv.x & KQC_LOW) | k5_dep(h0 & 0xFu), xl.x)
        idot += sdot4((wv.y & KQC_LOW) | k5_dep(h1 & 0xFu), xl.y)
        idot += sdot4((wv.z & KQC_LOW) | k5_dep(h2 & 0xFu), xl.z)
        idot += sdot4((wv.w & KQC_LOW) | k5_dep(h3 & 0xFu), xl.w)
        idot += sdot4(((wv.x >> 4u) & KQC_LOW) | k5_dep(h0 >> 4u), xh.x)
        idot += sdot4(((wv.y >> 4u) & KQC_LOW) | k5_dep(h1 >> 4u), xh.y)
        idot += sdot4(((wv.z >> 4u) & KQC_LOW) | k5_dep(h2 >> 4u), xh.z)
        return idot + sdot4(((wv.w >> 4u) & KQC_LOW) | k5_dep(h3 >> 4u), xh.w)
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        let idot = k5_dot(wq4[wsb * 10u + blk], xl, xh, wq[wsb * 40u + 32u + blk])
        return axs[xsb] * dm_fold(wsb, blk, idot, blk_sum(xl, xh))
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_k5_cls_spv")]
    def run {
        gemv_shell()
    }
}

// Q6_K: q = (ql | qh<<4) - 32 with per-16 SIGNED int8 sub-scales; the lo/hi nibble halves of a
// block are exactly its two 16-weight sub-scale groups, so each keeps its own dot + sum
[vk_dispatch(name = "kq_gemv_k6_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK6 : KqGemvBase {
    // (ilo, ihi, blo, bhi) — an int4, since a shader function cannot return a tuple
    def k6_dot(wv, xl, xh, qhl, qhh : uint4; shift : uint) : int4 {
        let m2 = 0x03030303u
        let l0 = (wv.x & KQC_LOW) | (((qhl.x >> shift) & m2) << 4u)
        let l1 = (wv.y & KQC_LOW) | (((qhl.y >> shift) & m2) << 4u)
        let l2 = (wv.z & KQC_LOW) | (((qhl.z >> shift) & m2) << 4u)
        let l3 = (wv.w & KQC_LOW) | (((qhl.w >> shift) & m2) << 4u)
        let h0 = ((wv.x >> 4u) & KQC_LOW) | (((qhh.x >> shift) & m2) << 4u)
        let h1 = ((wv.y >> 4u) & KQC_LOW) | (((qhh.y >> shift) & m2) << 4u)
        let h2 = ((wv.z >> 4u) & KQC_LOW) | (((qhh.z >> shift) & m2) << 4u)
        let h3 = ((wv.w >> 4u) & KQC_LOW) | (((qhh.w >> shift) & m2) << 4u)
        return int4(sdot4(l0, xl.x) + sdot4(l1, xl.y) + sdot4(l2, xl.z) + sdot4(l3, xl.w),
                    sdot4(h0, xh.x) + sdot4(h1, xh.y) + sdot4(h2, xh.z) + sdot4(h3, xh.w),
                    sdot4(xl.x, KQC_ONES) + sdot4(xl.y, KQC_ONES) + sdot4(xl.z, KQC_ONES) + sdot4(xl.w, KQC_ONES),
                    sdot4(xh.x, KQC_ONES) + sdot4(xh.y, KQC_ONES) + sdot4(xh.z, KQC_ONES) + sdot4(xh.w, KQC_ONES))
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let qh4 = wsb * 12u + 8u + (blk / 4u) * 2u
        let xb4 = xsb * 16u + blk * 2u
        let dot = k6_dot(wq4[wsb * 12u + blk], xq4[xb4], xq4[xb4 + 1u],
            wq4[qh4], wq4[qh4 + 1u], (blk % 4u) * 2u)
        let s4 = int4(unpack8(int(wsu[wsb * 5u + blk / 2u])))   // 4 signed sub-scales
        let slo = (blk % 2u) == 0u ? s4.x : s4.z
        let shi = (blk % 2u) == 0u ? s4.y : s4.w
        let dd = unpackHalf2x16(wsu[wsb * 5u + 4u]).x
        return axs[xsb] * dd * float(slo * (dot.x - 32 * dot.z) + shi * (dot.y - 32 * dot.w))
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_k6_cls_spv")]
    def run {
        gemv_shell()
    }
}

// IQ2_S: the iq3s gather widened to the u64 grid - a 10-bit qs|qh index picks TWO grid words
// (eight magnitudes {8, 25, 43}), the block's own sign bytes flip them; per-16 UNSIGNED
// strips (1 + 2s) keep split lo/hi dots under the eighth-ed d. The 8 KB grid stages ahead.
[vk_dispatch(name = "kq_gemv_iq2s_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2s : KqGemvBase {
    @workgroup gridw : uint[2048]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let qw = wq[wsb * 18u + blk]
        let sgw = wq[wsb * 18u + 8u + blk]
        let qh = (wq[wsb * 18u + 16u + blk / 4u] >> ((blk % 4u) * 8u)) & 255u
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        let sw = wsu[wsb * 5u + 1u + blk / 2u]
        let s0 = int((sw >> ((blk % 2u) * 16u)) & 255u)
        let s1 = int((sw >> ((blk % 2u) * 16u + 8u)) & 255u)
        var isum = 0
        for (h in range(2)) {
            let l0 = uint(h) * 2u
            let sg0 = (sgw >> (l0 * 8u)) & 255u
            let sg1 = (sgw >> (l0 * 8u + 8u)) & 255u
            let i0 = (((qw >> (l0 * 8u)) & 255u) | ((qh << (8u - 2u * l0)) & 0x300u)) * 2u
            let i1 = (((qw >> (l0 * 8u + 8u)) & 255u) | ((qh << (8u - 2u * (l0 + 1u))) & 0x300u)) * 2u
            let w0 = iq3s_signed(gridw[i0], sg0 & 15u)
            let w1 = iq3s_signed(gridw[i0 + 1u], sg0 >> 4u)
            let w2 = iq3s_signed(gridw[i1], sg1 & 15u)
            let w3 = iq3s_signed(gridw[i1 + 1u], sg1 >> 4u)
            if (h == 0) {
                isum += s0 * (sdot4(w0, xl.x) + sdot4(w1, xl.y) + sdot4(w2, xl.z) + sdot4(w3, xl.w))
            } else {
                isum += s1 * (sdot4(w0, xh.x) + sdot4(w1, xh.y) + sdot4(w2, xh.z) + sdot4(w3, xh.w))
            }
        }
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return axs[xsb] * dd * float(isum)
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_iq2s_cls_spv")]
    def run {
        let lid = gl_LocalInvocationID.x
        for (k in range(32)) {
            let idx = lid + uint(k) * 64u
            gridw[idx] = iq2s_grid_word(int(idx))
        }
        barrier()   // the grid visible before the shell's block walk
        gemv_shell()
    }
}

// IQ2_XS: the iq2s u64-grid walk with ksigns-by-parity - each u16 qs word carries a 9-bit
// grid index (TWO staged words) and a 7-bit KSIGNS_IQ2XS index whose sign byte ksign7
// recomputes (no sign plane, no qh); per-16 UNSIGNED strips under the eighth-ed d.
[vk_dispatch(name = "kq_gemv_iq2xs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xs : KqGemvBase {
    @workgroup gridw : uint[1024]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        let sw = wsu[wsb * 5u + 1u + blk / 2u]
        let s0 = int((sw >> ((blk % 2u) * 16u)) & 255u)
        let s1 = int((sw >> ((blk % 2u) * 16u + 8u)) & 255u)
        var isum = 0
        for (h in range(2)) {
            let qw = wq[wsb * 16u + blk * 2u + uint(h)]
            let wa = qw & 0xFFFFu
            let wb = qw >> 16u
            let sg0 = ksign7(wa >> 9u)
            let sg1 = ksign7(wb >> 9u)
            let i0 = (wa & 511u) * 2u
            let i1 = (wb & 511u) * 2u
            let w0 = iq3s_signed(gridw[i0], sg0 & 15u)
            let w1 = iq3s_signed(gridw[i0 + 1u], sg0 >> 4u)
            let w2 = iq3s_signed(gridw[i1], sg1 & 15u)
            let w3 = iq3s_signed(gridw[i1 + 1u], sg1 >> 4u)
            if (h == 0) {
                isum += s0 * (sdot4(w0, xl.x) + sdot4(w1, xl.y) + sdot4(w2, xl.z) + sdot4(w3, xl.w))
            } else {
                isum += s1 * (sdot4(w0, xh.x) + sdot4(w1, xh.y) + sdot4(w2, xh.z) + sdot4(w3, xh.w))
            }
        }
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return axs[xsb] * dd * float(isum)
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_iq2xs_cls_spv")]
    def run {
        let lid = gl_LocalInvocationID.x
        for (k in range(16)) {
            let idx = lid + uint(k) * 64u
            gridw[idx] = iq2xs_grid_word(int(idx))
        }
        barrier()   // the iq2xs grid visible before the shell's block walk
        gemv_shell()
    }
}

// IQ2_XXS: the iq3xxs GEMV shell over the two-word u64 grid - block b's grid word carries
// four BYTE indices (two staged words each), its aux32 the parity-derived ksigns; the per-32
// (1 + 2s) strip under the eighth-ed d reads exactly through the shared signed extract.
[vk_dispatch(name = "kq_gemv_iq2xxs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xxs : KqGemvBase {
    @workgroup gridw : uint[512]

    def iq3s_signed(g, nib : uint) : uint {
        let m1 = (nib * 0x00204081u) & 0x01010101u
        let mask = m1 * 255u
        return (g ^ mask) + m1
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        let gw = wq[wsb * 16u + blk * 2u]
        let aux = wq[wsb * 16u + blk * 2u + 1u]
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        var isum = 0
        for (h in range(2)) {
            let hu = uint(h)
            let b0 = (gw >> (16u * hu)) & 255u
            let b1 = (gw >> (16u * hu + 8u)) & 255u
            let sg0 = ksign7((aux >> (14u * hu)) & 127u)
            let sg1 = ksign7((aux >> (14u * hu + 7u)) & 127u)
            let w0 = iq3s_signed(gridw[b0 * 2u], sg0 & 15u)
            let w1 = iq3s_signed(gridw[b0 * 2u + 1u], sg0 >> 4u)
            let w2 = iq3s_signed(gridw[b1 * 2u], sg1 & 15u)
            let w3 = iq3s_signed(gridw[b1 * 2u + 1u], sg1 >> 4u)
            if (h == 0) {
                isum += sdot4(w0, xl.x) + sdot4(w1, xl.y) + sdot4(w2, xl.z) + sdot4(w3, xl.w)
            } else {
                isum += sdot4(w0, xh.x) + sdot4(w1, xh.y) + sdot4(w2, xh.z) + sdot4(w3, xh.w)
            }
        }
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return axs[xsb] * dd * float(iq4_sc(wsu[wsb * 5u + 1u + blk / 4u], blk) * isum)
    }

    [spirv_kernel(local_size_x = 64, name = "kq_gemv_iq2xxs_cls_spv")]
    def run {
        let lid = gl_LocalInvocationID.x
        for (k in range(8)) {
            let idx = lid + uint(k) * 64u
            gridw[idx] = iq2xxs_grid_word(int(idx))
        }
        barrier()   // the iq2xxs grid visible before the shell's block walk
        gemv_shell()
    }
}
