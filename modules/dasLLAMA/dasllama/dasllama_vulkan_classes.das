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

// ar_add_rms_b's class twin: x/a/w/y stop sharing packed buffers, eps/ascale ride the push
// struct. The reduce order is VERBATIM (stride-256 accum, subgroupAdd, sequential partials,
// 1/sqrt), so against vk_add_rms on the same inputs the outputs are BIT-IDENTICAL.
[vk_dispatch(name = "cls_ar", grid = "nrows", params = "nrows : int64")]
class ClsArAddRms {
    @ssbo @binding = 0 xs : array<float>   // x rows, the residual stream — updated in place
    @ssbo @binding = 1 aa : array<float>   // the add partner rows
    @ssbo @binding = 2 wn : array<float>   // the norm weight row
    @ssbo @binding = 3 yo : array<float>   // the normed output rows
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
