options gen2
options indenting = 4

module dasllama_vulkan_classes shared public

require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require math

// The CLASS-KERNEL home (the [spirv_kernel]/[vk_dispatch] port of dasllama_vulkan_kernels —
// families become one base class + per-format overrides, devirtualized at emission). Kernels
// here carry clean per-member bindings and push-constant params; region lists stay SSBO.

// ===== add + rmsnorm =====

struct ArArgs {
    dim : uint
    add_on : uint
    eps : float
    ascale : float
}

// the shared Q8_0 quantize pieces (pure — the amax fold is subgroup-wide, the pack is scalar)
def private q8_amax3(m0 : float) : float {
    var m = m0
    m = max(m, subgroupShuffleXor(m, 1u))
    m = max(m, subgroupShuffleXor(m, 2u))
    m = max(m, subgroupShuffleXor(m, 4u))
    return m
}

def private q8_pack4(v0, v1, v2, v3 : float; id : float) : uint {
    let q0 = int(round(v0 * id))
    let q1 = int(round(v1 * id))
    let q2 = int(round(v2 * id))
    let q3 = int(round(v3 * id))
    return ((uint(q0) & 0xFFu) | ((uint(q1) & 0xFFu) << 8u)
        | ((uint(q2) & 0xFFu) << 16u) | ((uint(q3) & 0xFFu) << 24u))
}

// the add+rmsnorm core: x/a/w members, the row stash, and the verbatim reduce (stride-256 accum,
// subgroupAdd, sequential partials, 1/sqrt) — leaves add their output members + kernels
class ArBase {
    @ssbo @binding = 0 xs : array<float>   // x rows, the residual stream — updated in place
    @ssbo @binding = 1 aa : array<float>   // the add partner rows
    @ssbo @binding = 2 wn : array<float>   // the norm weight row
    @push_constant pa : ArArgs
    @workgroup row : float[4096]           // AR_MAX_DIM — staged so the post-reduce reread is local
    @workgroup part : float[64]            // one partial per subgroup (subgroupSize >= 4 floor)
    @workgroup inv1 : float[1]

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
    def rms_inv(ss0 : float) : float {
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
            inv1[0u] = 1.0 / sqrt(tot / float(pa.dim) + pa.eps)   // the CPU rmsnorm's own form
        }
        barrier()
        return inv1[0u]
    }
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
            yo[base + k] = wn[k] * (row[k] * inv)
            k += 256u
        }
    }
}

// ar_add_rms + Q8_0 requant in ONE dispatch: the normed row quantizes out of the row stash, so
// xb never reaches memory. Verbatim reduce/amax/rounding => bit-identical to the split pair.
[vk_dispatch(name = "cls_ar_rq", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRq : ArBase {
    @ssbo @binding = 3 outq : array<uint>    // the quantized words
    @ssbo @binding = 4 outs : array<float>   // per-32-block scales

    [spirv_kernel(local_size_x = 256, name = "cls_ar_rq_spv")]
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
            let v0 = wn[base] * (row[base] * inv)
            let v1 = wn[base + 1u] * (row[base + 1u] * inv)
            let v2 = wn[base + 2u] * (row[base + 2u] * inv)
            let v3 = wn[base + 3u] * (row[base + 3u] * inv)
            let m = q8_amax3(max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3))))
            let d = m / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            if (lane8 == 0u) {
                outs[b] = d
            }
            outq[b * 8u + lane8] = q8_pack4(v0, v1, v2, v3, id)
            b += 32u
        }
    }
}

struct RqArgs {
    inbase : uint   // element base of the source rows
    nblk : uint     // block / superblock count
}

// straight Q8_0 requant of f32 rows (the deltanet chain's quantize step)
[vk_dispatch(name = "cls_dn_rq", grid = "wgs", params = "wgs : int64")]
class DnRequant {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 outq : array<uint>
    @ssbo @binding = 2 outs : array<float>
    @push_constant pa : RqArgs

    [spirv_kernel(local_size_x = 256, name = "cls_dn_rq_spv")]
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
            outq[b * 8u + lane8] = q8_pack4(v0, v1, v2, v3, id)
        }
    }
}

// straight Q8_K requant of f32 rows (the K-quant chains' mid-step re-quantize)
[vk_dispatch(name = "cls_q8k_rq", grid = "wgs", params = "wgs : int64")]
class Q8kRequant {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 outq : array<uint>
    @ssbo @binding = 2 outs : array<float>
    @push_constant pa : RqArgs

    def blk_scale(b, lane : uint; m0 : float) : float {
        var m = m0
        m = max(m, subgroupShuffleXor(m, 1u))
        m = max(m, subgroupShuffleXor(m, 2u))
        m = max(m, subgroupShuffleXor(m, 4u))
        m = max(m, subgroupShuffleXor(m, 8u))
        m = max(m, subgroupShuffleXor(m, 16u))
        let d = m / 127.0
        if (lane == 0u) {
            outs[b] = d
        }
        return d != 0.0 ? 1.0 / d : 0.0
    }

    def pack_word(b, r, lane : uint; v, id : float) {
        let q = int(round(v * id))
        let b8 = uint(q) & 0xFFu
        let p = b8 | (subgroupShuffleXor(b8, 1u) << 8u)
        let w = p | (subgroupShuffleXor(p, 2u) << 16u)
        if (lane % 4u == 0u) {
            outq[b * 64u + r * 8u + lane / 4u] = w
        }
    }

    [spirv_kernel(local_size_x = 256, name = "cls_q8k_rq_spv")]
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
                pack_word(b, uint(r), lane, vals[r], id)
            }
        }
    }
}

// Q8_0 words -> f16 rows (the cm2 chain's activation feed; one word = 4 quants per thread)
[vk_dispatch(name = "cls_xf16", grid = "wgs", params = "wgs : int64")]
class Q8Xf16 {
    @ssbo @binding = 0 xqw : array<uint>
    @ssbo @binding = 1 axs : array<float>     // one scale per 8 words (a 32-quant block)
    @ssbo @binding = 2 outh : array<float16>
    @push_constant pa : RqArgs                // nblk carries the WORD count here

    [spirv_kernel(local_size_x = 256, name = "cls_xf16_spv")]
    def run {
        let w = gl_GlobalInvocationID.x
        if (w < pa.nblk) {
            let v = int(xqw[w])
            let s = axs[w / 8u]
            outh[w * 4u] = float16(float((v << 24) >> 24) * s)
            outh[w * 4u + 1u] = float16(float((v << 16) >> 24) * s)
            outh[w * 4u + 2u] = float16(float((v << 8) >> 24) * s)
            outh[w * 4u + 3u] = float16(float(v >> 24) * s)
        }
    }
}

// ===== act + requant (the FFN mid-chain elementwise family) =====

struct ActArgs {
    nelem : uint   // element count (4-wide kernels; always a 32-multiple, the guard is exact)
    gelu : uint    // activation select: 0 = silu, else tanh-gelu
    nblk : uint    // 32-block / superblock count (the requant kernels' guard)
}

// members shared by the act/requant kernels; act_mul is the one activation formula
class ActRqMembers {
    @ssbo @binding = 0 gate : array<float>     // gate rows (act's v side)
    @ssbo @binding = 1 up : array<float>       // up rows (act's u side)
    @ssbo @binding = 2 outq : array<uint>      // quantized words out
    @ssbo @binding = 3 outs : array<float>     // per-block scales out
    @push_constant pa : ActArgs

    def act_mul(v, u : float) : float {
        return (pa.gelu != 0u ? (0.5 * v * (1.0 + tanh(0.7978845608028654 * (v + 0.044715 * v * v * v)))) * u
            : (v / (1.0 + exp(-v))) * u)
    }
}

// fused act + Q8_0 requant: subgroup per 4 blocks, word per lane, 8-lane shuffle amax (xor 1/2/4)
[vk_dispatch(name = "q8_actrq_cls", grid = "wgs", params = "wgs : int64")]
class Q8ActRq : ActRqMembers {
    // 8-lane block quantize: amax fold over the block's 8 co-active lanes, d to outs[b],
    // returns this lane's packed word
    def blk_pack(b, lane8 : uint; v0, v1, v2, v3 : float) : uint {
        var m = max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3)))
        m = max(m, subgroupShuffleXor(m, 1u))
        m = max(m, subgroupShuffleXor(m, 2u))
        m = max(m, subgroupShuffleXor(m, 4u))
        let d = m / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (lane8 == 0u) {
            outs[b] = d
        }
        let q0 = int(round(v0 * id))
        let q1 = int(round(v1 * id))
        let q2 = int(round(v2 * id))
        let q3 = int(round(v3 * id))
        return ((uint(q0) & 0xFFu) | ((uint(q1) & 0xFFu) << 8u)
            | ((uint(q2) & 0xFFu) << 16u) | ((uint(q3) & 0xFFu) << 24u))
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
            outq[b * 8u + lane % 8u] = blk_pack(b, lane % 8u, v0, v1, v2, v3)
        }
    }
}

// the Q8_K twin: one subgroup per 256-weight superblock, whole-subgroup 5-shuffle amax,
// butterfly byte pack
[vk_dispatch(name = "q8k_actrq_cls", grid = "wgs", params = "wgs : int64")]
class Q8kActRq : ActRqMembers {
    def blk_scale(b, lane : uint; m0 : float) : float {
        var m = m0
        m = max(m, subgroupShuffleXor(m, 1u))
        m = max(m, subgroupShuffleXor(m, 2u))
        m = max(m, subgroupShuffleXor(m, 4u))
        m = max(m, subgroupShuffleXor(m, 8u))
        m = max(m, subgroupShuffleXor(m, 16u))
        let d = m / 127.0
        if (lane == 0u) {
            outs[b] = d
        }
        return d != 0.0 ? 1.0 / d : 0.0
    }

    // butterfly byte pack of one quantized round: lanes 4j..4j+3 fold into the superblock's word j
    def pack_word(b, r, lane : uint; v, id : float) {
        let q = int(round(v * id))
        let b8 = uint(q) & 0xFFu
        let p = b8 | (subgroupShuffleXor(b8, 1u) << 8u)
        let w = p | (subgroupShuffleXor(p, 2u) << 16u)
        if (lane % 4u == 0u) {
            outq[b * 64u + r * 8u + lane / 4u] = w
        }
    }

    [spirv_kernel(local_size_x = 256, name = "q8k_actrq_cls_spv")]
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
                pack_word(b, uint(r), lane, vals[r], id)
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

    def act_mul(v, u : float) : float {
        return (pa.gelu != 0u ? (0.5 * v * (1.0 + tanh(0.7978845608028654 * (v + 0.044715 * v * v * v)))) * u
            : (v / (1.0 + exp(-v))) * u)
    }

    [spirv_kernel(local_size_x = 256, name = "actf16_cls_spv")]
    def run {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            outh[e] = float16(act_mul(gate[e], up[e]))
            outh[e + 1u] = float16(act_mul(gate[e + 1u], up[e + 1u]))
            outh[e + 2u] = float16(act_mul(gate[e + 2u], up[e + 2u]))
            outh[e + 3u] = float16(act_mul(gate[e + 3u], up[e + 3u]))
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
            outh[e] = float16(src[e])
            outh[e + 1u] = float16(src[e + 1u])
            outh[e + 2u] = float16(src[e + 2u])
            outh[e + 3u] = float16(src[e + 3u])
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
                if (jb >= pa.r0 && jb < pa.r1) {
                    acc += slot_w[p * pa.k + j] * down_rows[(jb - pa.r0) * pa.n + i]
                }
                j++
            }
            acc_rows[p * pa.n + i] = acc
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
    par : uint     // ring parity (the old kernel smuggled it through a float scale slot)
    eps : float
    qscale : float
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
        let rrb = pa.hist_off + pa.par * pa.cd * taps
        let wrb = pa.hist_off + (1u - pa.par) * pa.cd * taps
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
    def run {   // nolint:STYLE037,STYLE038 — the verbatim phase-1 pass, parity-locked to the shipped kernel
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
    def run {   // nolint:STYLE037,STYLE038 — the verbatim phase-2 walk, parity-locked to the shipped kernel
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

// ===== decode attention =====

struct DaAttnArgs {
    cnt : uint      // KV positions to attend
    kvd : uint      // kv row width
    hs : uint       // head size
    kv_mul : uint   // GQA group (query heads per kv head)
    kbase : uint    // layer base into the shared K mirror (0 for the standalone seam)
    vbase : uint
    scale : float
}

// decode attention (one query row): one wg per head, GQA, f32 K/V mirror. Online softmax over
// 256-position chunks (running max/denom, alpha-rescaled V accumulators) — any cnt works.
[vk_dispatch(name = "da_attn_cls", grid = "nheads", params = "nheads : int64")]
class DaAttn {
    @ssbo @binding = 0 q : array<float>      // roped query rows, one per head
    @ssbo @binding = 1 kmir : array<float>   // the K mirror plane
    @ssbo @binding = 2 vmir : array<float>   // the V mirror plane
    @ssbo @binding = 3 y : array<float>      // attention out rows
    @push_constant pa : DaAttnArgs
    @workgroup qrow : float[256]   // this head's query row (hs <= 256)
    @workgroup sc : float[256]     // chunk scores, then the cross-group V partial reduce
    @workgroup part : float[64]    // per-subgroup partials
    @workgroup ml : float[2]       // [0] = running max, [1] = running softmax denom

    [spirv_kernel(local_size_x = 256, name = "da_attn_cls_spv")]
    def run {   // nolint:STYLE038 — the verbatim online-softmax pass, parity-locked to the shipped kernel
        let hs = pa.hs
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
        while (c0 < pa.cnt) {
            let clen = min(256u, pa.cnt - c0)
            // chunk scores: thread tid owns position c0 + tid
            var s = -3.0e38
            if (tid < clen) {
                var sv = 0.0
                var d = 0u
                while (d < hs) {
                    sv += qrow[d] * kmir[pa.kbase + (c0 + tid) * pa.kvd + kvh * hs + d]
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
                    acc += sc[jj] * vmir[pa.vbase + (c0 + jj) * pa.kvd + kvh * hs + c]
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

// ===== the Q8_0 batch GEMM tile (the sdot4 prefill workhorse) =====

struct BatchArgs {
    n : uint         // reduction dim
    d : uint         // output cols
    map_off : uint   // the per-wg map's word offset in sched (region records sit at 0)
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

    def k5_dep(hb : uint) : uint {
        return ((hb & 1u) << 4u) | ((hb & 2u) << 11u) | ((hb & 4u) << 18u) | ((hb & 8u) << 25u)
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
[vk_dispatch(name = "kq_batch_k4_cls", grid = "wgs", params = "wgs : int64")]
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
[vk_dispatch(name = "kq_batch_q40_cls", grid = "wgs", params = "wgs : int64")]
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

// Q5_K tile: the k4 compose with the 5th bit OR'd in from the 40B block's qh words
[vk_dispatch(name = "kq_batch_k5_cls", grid = "wgs", params = "wgs : int64")]
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
[vk_dispatch(name = "kq_batch_k6_cls", grid = "wgs", params = "wgs : int64")]
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

// ===== rope + KV store + qk-norm =====

// the workgroup-rms machinery the head-row kernels share (one wg reduces one row)
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
    krow : uint     // this position's K mirror row base
    vrow : uint     // ... and V mirror row base
}

// fused rope + KV-row store (decode, one token), f32 mirror: one thread per rotation pair,
// each owning 2 distinct K + 2 V slots
[vk_dispatch(name = "rope_kv_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStore {
    @ssbo @binding = 0 qrows : array<float>    // q, roped in place
    @ssbo @binding = 1 kvsrc : array<float>    // the projection buffer: [q | k | v] rows
    @ssbo @binding = 2 kmir : array<float>     // the K mirror plane
    @ssbo @binding = 3 vmir : array<float>     // the V mirror plane
    @ssbo @binding = 4 cossin : array<float>   // this position's cos row then sin row
    @push_constant pa : RopeKvArgs

    [spirv_kernel(local_size_x = 256, name = "rope_kv_cls_spv")]
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
                let kb = pa.qd + h * pa.hs
                let a0 = kvsrc[kb + e0]
                let a1 = kvsrc[kb + e1]
                kmir[pa.krow + h * pa.hs + e0] = a0 * fcr - a1 * fci
                kmir[pa.krow + h * pa.hs + e1] = a0 * fci + a1 * fcr
                // v is not roped — copy the same two elements raw into the V plane
                let vb = pa.qd + pa.kvd + h * pa.hs
                vmir[pa.vrow + h * pa.hs + e0] = kvsrc[vb + e0]
                vmir[pa.vrow + h * pa.hs + e1] = kvsrc[vb + e1]
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
    voff : uint      // v rows' base inside the kv projection buffer
}

// batched rope + KV-store (prefill, npos positions): q roped in place, k/v from the projection
// buffer into the mirrors at absolute positions
[vk_dispatch(name = "rope_kv_b_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStoreB {
    @ssbo @binding = 0 qrows : array<float>    // q rows, roped in place
    @ssbo @binding = 1 kvrows : array<float>   // k rows at 0, v rows at voff
    @ssbo @binding = 2 kmir : array<float>
    @ssbo @binding = 3 vmir : array<float>
    @ssbo @binding = 4 cossin : array<float>   // per-position cos row then sin row (hs floats each)
    @push_constant pa : RopeKvBArgs

    [spirv_kernel(local_size_x = 256, name = "rope_kv_b_cls_spv")]
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
                let kb = p * pa.kvd + h * pa.hs
                let a0 = kvrows[kb + e0]
                let a1 = kvrows[kb + e1]
                let mo = pa.layerbase + apos * pa.kvd + h * pa.hs
                kmir[mo + e0] = a0 * fcr - a1 * fci
                kmir[mo + e1] = a0 * fci + a1 * fcr
                let vb = pa.voff + p * pa.kvd + h * pa.hs
                vmir[mo + e0] = kvrows[vb + e0]
                vmir[mo + e1] = kvrows[vb + e1]
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

struct QknRopeArgs {
    qd : uint
    kvd : uint
    hs : uint
    half : uint
    neox : uint
    nh : uint       // q heads (wgs >= nh are k heads)
    krow : uint
    vrow : uint
    qwoff : uint    // q norm-weight row base in wn
    kwoff : uint
    eps : float
}

// qk_rms + rope_kv_store in ONE dispatch (qk_norm models), one wg per head-row; verbatim
// reduce + rotation order, so bit-identical to the split pair
[vk_dispatch(name = "qkn_rope_cls", grid = "wgs", params = "wgs : int64")]
class QknRopeKv : RmsWgBase {
    @ssbo @binding = 0 qrows : array<float>    // q rows, normed + roped in place
    @ssbo @binding = 1 kvsrc : array<float>    // the projection buffer: [q | k | v] rows
    @ssbo @binding = 2 kmir : array<float>
    @ssbo @binding = 3 vmir : array<float>
    @ssbo @binding = 4 cossin : array<float>
    @ssbo @binding = 5 wn : array<float>       // the q/k norm weight rows (its OWN member now —
                                               // the old kernel packed them into the cos buffer)
    @push_constant pa : QknRopeArgs
    @workgroup row : float[256]                // this head's normed q or k row (hs <= 256)

    [spirv_kernel(local_size_x = 256, name = "qkn_rope_cls_spv")]
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
                kmir[pa.krow + h * pa.hs + e0] = r0
                kmir[pa.krow + h * pa.hs + e1] = r1
            } else {
                qrows[h * pa.hs + e0] = r0
                qrows[h * pa.hs + e1] = r1
            }
        }
        // v is not roped or normed — the k-head wgs copy their own raw row into the V plane
        if (is_k && tid < pa.hs) {
            vmir[pa.vrow + h * pa.hs + tid] = kvsrc[pa.qd + pa.kvd + h * pa.hs + tid]
        }
    }
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

// q8n_moe_groupn's class twin: the region skeleton on the 34B blocks — 17 u16 per block, u16-pair
// loads combined to words, 8 lanes per block, d broadcast-read from u16[0]. No binding-1 scale
// plane (scales are inline), so the class layout carries the GAP — sparse bindings by design.
[vk_dispatch(name = "q8n_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q8nGemv {
    @ssbo @binding = 0 wq16 : array<uint16>    // the 34B blocks as u16 words ([d][16 quant pairs])
    @ssbo @binding = 2 regions : array<uint>
    @ssbo @binding = 3 xqw : array<uint>
    @ssbo @binding = 4 axs : array<float>
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q8n_gemv_cls_spv")]
    def run {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.nreg * pa.d) {
            let r = rg / pa.d
            let row = rg % pa.d
            let nb = pa.n / 32u
            let wb0 = regions[r * 2u] + row * nb
            let xb0 = regions[r * 2u + 1u]
            let lane = gl_SubgroupInvocationID
            let my_word = lane % 8u
            let my_boff = lane / 8u
            let bpp = gl_SubgroupSize / 8u
            var acc = 0.0
            var bi0 = 0u
            while (bi0 < nb) {
                let b = bi0 + my_boff
                var idot = 0
                var dbits = 0u
                if (b < nb) {
                    let base = (wb0 + b) * 17u
                    let lo = uint(wq16[base + 1u + my_word * 2u])
                    let hi = uint(wq16[base + 2u + my_word * 2u])
                    idot = sdot4(lo | (hi << 16u), xqw[(xb0 + b) * 8u + my_word])
                    dbits = uint(wq16[base])
                }
                idot += subgroupShuffleXor(idot, 1u)
                idot += subgroupShuffleXor(idot, 2u)
                idot += subgroupShuffleXor(idot, 4u)
                if (my_word == 0u && b < nb) {
                    acc += float(idot) * unpackHalf2x16(dbits).x * axs[xb0 + b]
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
    @ssbo @binding = 1 wsu : array<uint>       // scale planes ([f16 d][f16 dmin][8 sc][8 mn] / q40 half pairs / k6 int8+d)
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

// Q4_K: w = d*sc*q - dmin*mn*1, q in [0,15] — per 32-block axs*(d*sc*idot - dmin*mn*bsum)
[vk_dispatch(name = "kq_gemv_k4_cls", grid = "wgs", params = "wgs : int64")]
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
[vk_dispatch(name = "kq_gemv_q40_cls", grid = "wgs", params = "wgs : int64")]
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

// Q5_K: the k4 fold with the 5th bit deposited from the block's qh word
[vk_dispatch(name = "kq_gemv_k5_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK5 : KqGemvBase {
    // deposit 4 packed high-bits (one per nibble lane of hb) at bit 4 of each byte lane
    def k5_dep(hb : uint) : uint {
        return ((hb & 1u) << 4u) | ((hb & 2u) << 11u) | ((hb & 4u) << 18u) | ((hb & 8u) << 25u)
    }

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
[vk_dispatch(name = "kq_gemv_k6_cls", grid = "wgs", params = "wgs : int64")]
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
