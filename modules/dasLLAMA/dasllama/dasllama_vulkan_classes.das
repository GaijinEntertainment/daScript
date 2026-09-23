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
require daslib/math_bits   // uint_bits_to_float: the KHR tile's f32 store bounce read back through its uint staging array
require daslib/enum_trait   // kq_gemv_fmts walks the enum
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
    parity : uint
    mirbase : uint  //! the N-row command: this row's K/V mirror region as an element offset; 0 in the one-row command, whose push carries the region
    dnslot : uint
    pad0 : uint
    pad1 : uint
    pad2 : uint
}

// ===== add + rmsnorm =====

struct ArArgs {
    dim : uint
    add_on : uint
    woff : uint     // norm-weight row base in wn (the driver's norms plane is shared per-layer)
    eps : float
    ascale : float
    row0 : uint
    slots : uint    //! the combine twins: routed slots per position
    lstride : uint  //! the combine twins: the logits row stride (the gate logit, when `gated`, is the row's last)
    gated : uint    //! the combine twins: the shared expert's gate scales its row
    pre_on : uint   //! the add partner is rms-normed with the row at `pwoff` before the add (a gemma's post-attention / post-FFN norm)
    pwoff : uint
    routed_post_off : uint   //! the gemma-4 MoE combine's norm rows: `pwoff` norms the dense branch, this the routed sum
    sum_post_off : uint      //! ... and this the two branches' sum
    no_norm : uint           //! the requant twins quantize the updated row as it is - the E-series residual feeding its per-layer-embedding gate
    presummed : uint //! the gemma-4 combine: the routed rows arrive summed under their weights in row 0 (the down GEMV folded them)
    dbias_on : uint  //! the combine twins: each routed row takes its expert's down bias (gpt-oss) before its routing weight scales it
    dboff : uint     //! ... this layer's row base in the down-bias plane ([ne x dim] rows a layer)
    abias_on : uint  //! the add partner carries a bias row out of wn before the add (gpt-oss's attention output bias, `RD_NORM_OBIAS`)
    aboff : uint     //! ... that row's base in wn
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

// the workgroup-rms the row kernels share (one wg reduces one row): one barrier a reduce, every
// thread summing the subgroup partials in the same order, so every thread holds the same inverse
class RmsWgBase {
    @workgroup part : float[128]   //! two slots of 64 partials, one per subgroup (subgroupSize >= 4 floor)

    //! `slot` alternates 0 / 1 between consecutive reduces of one kernel
    def wg_rms_inv(ss0 : float; n : uint; eps : float; slot : uint) : float {
        let ss = subgroupAdd(ss0)
        if (gl_SubgroupInvocationID == 0u) {
            part[slot * 64u + gl_SubgroupID] = ss
        }
        barrier()
        var tot = 0.0
        var sgi = 0u
        while (sgi < gl_NumSubgroups) {
            tot += part[slot * 64u + sgi]
            sgi++
        }
        return 1.0 / sqrt(tot / float(n) + eps)   // the CPU rmsnorm's own form
    }
}

// the add+rmsnorm core: x/a/w members, the row stash, and the shared workgroup reduce — leaves
// add their output members + kernels
class ArBase : RmsWgBase {
    @ssbo @binding = 0 xs : array<float>   // x rows, the residual stream — updated in place
    @ssbo @binding = 1 aa : array<float>   // the add partner rows
    @ssbo @binding = 2 wn : array<float>   // the norm weight row
    @push_constant pa : ArArgs
    @workgroup row : float[8192]           // AR_MAX_DIM — staged so the post-reduce reread is local

    //! the add partner's element `j` of the row at `base`, its bias row added under `abias_on`
    def apart(base, j : uint) : float => aa[base + j] + (pa.abias_on != 0u ? wn[pa.aboff + j] : 0.0)

    // the residual add + row stash + sum-of-squares pass over one row at `base`; under pre_on the add
    // partner is normed first (its own reduce, the row stash holding it meanwhile)
    [arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#residual-step-fma")]
    def accum_row(base : uint) : float {
        var ainv = 0.0
        if (pa.pre_on != 0u && pa.add_on != 0u) {
            var sa = 0.0
            var j = gl_LocalInvocationID.x
            while (j < pa.dim) {
                let a = apart(base, j)
                row[j] = a
                sa += a * a
                j += 256u
            }
            ainv = wg_rms_inv(sa, pa.dim, pa.eps, 0u)
        }
        var ss = 0.0
        var k = gl_LocalInvocationID.x
        if (pa.pre_on != 0u) {   //! the sandwich form: the partner off the row stash, a column a round
            while (k < pa.dim) {
                let vv = pa.add_on != 0u ? mad(wn[pa.pwoff + k], row[k] * ainv, xs[base + k]) * pa.ascale : xs[base + k]
                xs[base + k] = vv
                row[k] = vv
                ss += vv * vv
                k += 256u
            }
            return ss
        }
        let last = pa.dim - 1u
        while (k < pa.dim) {   //! four columns a round, every load issued before the adds: a one-workgroup row kernel is its load rounds
            let k1 = min(k + 256u, last)
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            var v0 = xs[base + k]
            var v1 = xs[base + k1]
            var v2 = xs[base + k2]
            var v3 = xs[base + k3]
            if (pa.add_on != 0u) {
                let a0 = apart(base, k)
                let a1 = apart(base, k1)
                let a2 = apart(base, k2)
                let a3 = apart(base, k3)
                v0 = (v0 + a0) * pa.ascale
                v1 = (v1 + a1) * pa.ascale
                v2 = (v2 + a2) * pa.ascale
                v3 = (v3 + a3) * pa.ascale
            }
            xs[base + k] = v0
            row[k] = v0
            ss += v0 * v0
            if (k + 256u < pa.dim) {
                xs[base + k1] = v1
                row[k1] = v1
                ss += v1 * v1
            }
            if (k + 512u < pa.dim) {
                xs[base + k2] = v2
                row[k2] = v2
                ss += v2 * v2
            }
            if (k + 768u < pa.dim) {
                xs[base + k3] = v3
                row[k3] = v3
                ss += v3 * v3
            }
            k += 1024u
        }
        return ss
    }

    def normed(k : uint; inv : float) : float => wn[pa.woff + k] * (row[k] * inv)

    // workgroup-wide rms inverse from per-thread sum-of-squares partials (slot 1: the add partner's norm took slot 0)
    def rms_inv(ss0 : float) : float => wg_rms_inv(ss0, pa.dim, pa.eps, 1u)
}

// ar_add_rms_b's class twin: one workgroup per row, the normed output to its own member
[vk_dispatch(name = "cls_ar", grid = "nrows", params = "nrows : int64")]
class ClsArAddRms : ArBase {
    @ssbo @binding = 3 yo : array<float>   // the normed output rows

    [spirv_kernel(local_size_x = 256, name = "cls_ar_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
    def run {
        let base = (gl_WorkGroupID.x + pa.row0) * pa.dim
        let ss = accum_row(base)
        let inv = rms_inv(ss)
        let last = pa.dim - 1u
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let k1 = min(k + 256u, last)
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            let n0 = normed(k, inv)
            let n1 = normed(k1, inv)
            let n2 = normed(k2, inv)
            let n3 = normed(k3, inv)
            yo[base + k] = n0
            if (k + 256u < pa.dim) {
                yo[base + k1] = n1
            }
            if (k + 512u < pa.dim) {
                yo[base + k2] = n2
            }
            if (k + 768u < pa.dim) {
                yo[base + k3] = n3
            }
            k += 1024u
        }
    }
}

// ar_add_rms + Q8_0 requant in ONE dispatch: the normed row quantizes out of the row stash, so
// xb never reaches memory. Verbatim reduce/amax/rounding => bit-identical to the split pair.
class ArRqBase : ArBase {
    @ssbo @binding = 3 outq : array<int8>    // the quantized bytes, row-major blocks
    @ssbo @binding = 4 outs : array<float>   // per-32-block scales

    // quantize the normed row instead of storing it: 8 lanes per 32-block, 32 blocks per pass
    [arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def rq_row(rbase, qb0 : uint) {
        let ss = accum_row(rbase)
        quant32(qb0, row_inv(ss))
    }

    //! the row's norm inverse - one under `no_norm` (the reduce still runs: its barrier is every thread's)
    def row_inv(ss : float) : float {
        let inv = rms_inv(ss)
        return pa.no_norm != 0u ? 1.0 : inv
    }

    //! the norm weight at `e`, one under `no_norm`
    def nw(e : uint) : float => pa.no_norm != 0u ? 1.0 : wn[pa.woff + e]

    def quant32(qb0 : uint; inv : float) {
        let tid = gl_LocalInvocationID.x
        let nblk = pa.dim / 32u
        let lane8 = tid % 8u
        var b = tid / 8u
        while (b < nblk) {
            let base = b * 32u + lane8 * 4u
            let v0 = nw(base) * (row[base] * inv)
            let v1 = nw(base + 1u) * (row[base + 1u] * inv)
            let v2 = nw(base + 2u) * (row[base + 2u] * inv)
            let v3 = nw(base + 3u) * (row[base + 3u] * inv)
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

[vk_dispatch(name = "cls_ar_rq", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRq : ArRqBase {
    [spirv_kernel(local_size_x = 256, name = "cls_ar_rq_spv")]
    def run {
        rq_row(0u, 0u)
    }
}

[vk_dispatch(name = "cls_ar_rq_b", grid = "nrows", params = "nrows : int64")]
class ClsArAddRmsRqB : ArRqBase {
    [spirv_kernel(local_size_x = 256, name = "cls_ar_rq_b_spv")]
    def run {
        rq_row(gl_WorkGroupID.x * pa.dim, gl_WorkGroupID.x * (pa.dim / 32u))
    }
}

[ |> template_struct_instance]
class template ArRqT : ArRqBase {
    @template_constant KBLK : bool = false    //!< the quant form: Q8_K superblocks (a scale per 256) instead of Q8_0's 32-blocks
    @template_constant STOREX : bool = false  //!< the normed row stored to binding 5 as floats beside the quants
    @ssbo @binding = 5 @template_gate = STOREX yo : array<float>   //! the normed row, for the consumers that read it as floats

    //! the Q8_K superblocks of the stashed row at inv: a 32-lane block a 256-block, eight values a lane - the block by the invocation id, as the requant class maps it, so the amax fold's masks stay inside it on any subgroup width
    [arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def quant_k(inv : float) {
        let nblk = pa.dim / 256u
        let lane = gl_LocalInvocationID.x & 31u
        var b = gl_LocalInvocationID.x >> 5u
        while (b < nblk) {
            let base = b * 256u + lane
            var vals : float[8]
            var m = 0.0
            for [unroll] (r in range(8)) {
                let e = base + uint(r) * 32u
                let v = nw(e) * (row[e] * inv)
                vals[r] = v
                m = max(m, abs(v))
            }
            let d = q8k_amax5(m) / 127.0
            if (lane == 0u) {
                outs[b] = d
            }
            let id = d != 0.0 ? 1.0 / d : 0.0
            for [unroll] (r in range(8)) {
                outq[base + uint(r) * 32u] = q8_quant1(vals[r], id)
            }
            b += 8u   //! the 256-thread workgroup's eight blocks
        }
    }

    [spirv_kernel(local_size_x = 256)]
    def run {
        let ss = accum_row(0u)
        let inv = row_inv(ss)
        static_if (STOREX) {
            var k = gl_LocalInvocationID.x
            while (k < pa.dim) {
                yo[k] = nw(k) * (row[k] * inv)
                k += 256u
            }
        }
        static_if (KBLK) {
            quant_k(inv)
        } else {
            quant32(0u, inv)
        }
    }
}

[vk_dispatch(name = "cls_ar_rqx", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqX : ArRqT {
    override STOREX = true
}

[vk_dispatch(name = "cls_ar_rqk", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqK : ArRqT {
    override KBLK = true
}

[vk_dispatch(name = "cls_ar_rqkx", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqKX : ArRqT {
    override KBLK = true
    override STOREX = true
}

class ArCombBase : ArBase {
    @ssbo @binding = 4 slot_idx : array<uint>    //! position -> its k slot rows
    @ssbo @binding = 5 slot_w : array<float>     //! ... and their routing weights
    @ssbo @binding = 6 down_rows : array<float>  //! the routed down-GEMM output rows
    @ssbo @binding = 7 logits : array<float>     //! [npos x lstride]; the gate logit is the row's last
    @ssbo @binding = 8 @role = "weight" dbias : array<float>   //! the expert down biases, [ne x dim] a layer (gpt-oss), read under `dbias_on`
    @ssbo @binding = 9 slot_ex : array<uint>     //! slot -> its expert, the down bias's row (the decode's top-k picks; the window's per-slot picks)

    //! slot `s`'s down row at column k, its expert's bias added under `dbias_on`
    def drow(s, k : uint) : float {
        let d = down_rows[slot_idx[s] * pa.dim + k]
        return pa.dbias_on != 0u ? d + dbias[pa.dboff + slot_ex[s] * pa.dim + k] : d
    }

    //! the same row off registers: `rb` the slot's row base, `bb` its bias row base
    def drow_at(rb, bb, k : uint) : float => down_rows[rb + k] + dbias[bb + k]

    def override accum_row(base : uint) : float {   // nolint:STYLE038 — one row pass: the hoisted metas, the four-column round, the memory tail
        let p = base / pa.dim
        var g = 1.0
        if (pa.gated != 0u) {
            g = 1.0 / (1.0 + exp(-logits[p * pa.lstride + pa.lstride - 1u]))
        }
        let pk = p * pa.slots
        let nreg = min(pa.slots, 8u)
        var rb : uint[8]
        var bb : uint[8]
        var sw : float[8]
        for (j in range(8)) {
            if (uint(j) < nreg) {
                rb[j] = slot_idx[pk + uint(j)] * pa.dim
                sw[j] = slot_w[pk + uint(j)]
                bb[j] = pa.dbias_on != 0u ? pa.dboff + slot_ex[pk + uint(j)] * pa.dim : 0u
            } else {
                rb[j] = 0u
                sw[j] = 0.0
                bb[j] = 0u
            }
        }
        let last = pa.dim - 1u
        var ss = 0.0
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let k1 = min(k + 256u, last)   //! the clamped columns past the row read a live column and store nothing
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            var f0 = 0.0
            var f1 = 0.0
            var f2 = 0.0
            var f3 = 0.0
            if (pa.add_on != 0u) {
                f0 = g * aa[base + k]
                f1 = g * aa[base + k1]
                f2 = g * aa[base + k2]
                f3 = g * aa[base + k3]
            }
            for (j in range(8)) {
                if (uint(j) < nreg) {
                    f0 += sw[j] * drow_at(rb[j], bb[j], k)
                    f1 += sw[j] * drow_at(rb[j], bb[j], k1)
                    f2 += sw[j] * drow_at(rb[j], bb[j], k2)
                    f3 += sw[j] * drow_at(rb[j], bb[j], k3)
                }
            }
            var j = 8u
            while (j < pa.slots) {
                let w = slot_w[pk + j]
                f0 += w * drow(pk + j, k)
                f1 += w * drow(pk + j, k1)
                f2 += w * drow(pk + j, k2)
                f3 += w * drow(pk + j, k3)
                j++
            }
            let v0 = (xs[base + k] + f0) * pa.ascale
            let v1 = (xs[base + k1] + f1) * pa.ascale
            let v2 = (xs[base + k2] + f2) * pa.ascale
            let v3 = (xs[base + k3] + f3) * pa.ascale
            xs[base + k] = v0
            row[k] = v0
            ss += v0 * v0
            if (k + 256u < pa.dim) {
                xs[base + k1] = v1
                row[k1] = v1
                ss += v1 * v1
            }
            if (k + 512u < pa.dim) {
                xs[base + k2] = v2
                row[k2] = v2
                ss += v2 * v2
            }
            if (k + 768u < pa.dim) {
                xs[base + k3] = v3
                row[k3] = v3
                ss += v3 * v3
            }
            k += 1024u
        }
        return ss
    }
}

[vk_dispatch(name = "cls_ar_comb", grid = "nrows", params = "nrows : int64")]
class ClsArComb : ArCombBase {
    @ssbo @binding = 3 yo : array<float>   //! the normed output rows

    [spirv_kernel(local_size_x = 256, name = "cls_ar_comb_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let base = (gl_WorkGroupID.x + pa.row0) * pa.dim
        let ss = accum_row(base)
        let inv = rms_inv(ss)
        let last = pa.dim - 1u
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let k1 = min(k + 256u, last)
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            let n0 = normed(k, inv)
            let n1 = normed(k1, inv)
            let n2 = normed(k2, inv)
            let n3 = normed(k3, inv)
            yo[base + k] = n0
            if (k + 256u < pa.dim) {
                yo[base + k1] = n1
            }
            if (k + 512u < pa.dim) {
                yo[base + k2] = n2
            }
            if (k + 768u < pa.dim) {
                yo[base + k3] = n3
            }
            k += 1024u
        }
    }
}

//! the plain combine's quantizing twin (`ClsArCombG4Rq`'s form): the normed row into Q8_0 blocks at the combine's free bindings, so no requant dispatch follows
[vk_dispatch(name = "cls_ar_comb_rq", grid = "nrows", params = "nrows : int64")]
class ClsArCombRq : ArCombBase {
    @ssbo @binding = 3 outq : array<int8>     //! the normed row's Q8_0 quants (the next layer's q feed)
    @ssbo @binding = 10 outs : array<float>   //! ... and its block scales

    [spirv_kernel(local_size_x = 256, name = "cls_ar_comb_rq_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let base = (gl_WorkGroupID.x + pa.row0) * pa.dim
        let ss = accum_row(base)
        let inv = rms_inv(ss)
        let tid = gl_LocalInvocationID.x
        let nblk = pa.dim / 32u
        let lane8 = tid % 8u
        let qb0 = (gl_WorkGroupID.x + pa.row0) * nblk
        var b = tid / 8u
        while (b < nblk) {
            let e = b * 32u + lane8 * 4u
            let v0 = wn[pa.woff + e] * (row[e] * inv)
            let v1 = wn[pa.woff + e + 1u] * (row[e + 1u] * inv)
            let v2 = wn[pa.woff + e + 2u] * (row[e + 2u] * inv)
            let v3 = wn[pa.woff + e + 3u] * (row[e + 3u] * inv)
            let m = q8_amax3(max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3))))
            let dq = m / 127.0
            let id = dq != 0.0 ? 1.0 / dq : 0.0
            if (lane8 == 0u) {
                outs[qb0 + b] = dq
            }
            let qb = qb0 * 32u + e
            outq[qb] = q8_quant1(v0, id)
            outq[qb + 1u] = q8_quant1(v1, id)
            outq[qb + 2u] = q8_quant1(v2, id)
            outq[qb + 3u] = q8_quant1(v3, id)
            b += 32u
        }
    }
}

[vk_dispatch(name = "cls_ar_comb_f16_b", grid = "nrows", params = "nrows : int64")]
class ClsArCombF16B : ArCombBase {
    @ssbo @binding = 3 outh : array<float16>   //! the normed rows, f16

    [spirv_kernel(local_size_x = 256, name = "cls_ar_comb_f16_b_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let rbase = gl_WorkGroupID.x * pa.dim
        let ss = accum_row(rbase)
        let inv = rms_inv(ss)
        let last = pa.dim - 1u
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let k1 = min(k + 256u, last)
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            let n0 = normed(k, inv)
            let n1 = normed(k1, inv)
            let n2 = normed(k2, inv)
            let n3 = normed(k3, inv)
            outh[rbase + k] = float16(n0)
            if (k + 256u < pa.dim) {
                outh[rbase + k1] = float16(n1)
            }
            if (k + 512u < pa.dim) {
                outh[rbase + k2] = float16(n2)
            }
            if (k + 768u < pa.dim) {
                outh[rbase + k3] = float16(n3)
            }
            k += 1024u
        }
    }
}

//! the gemma-4 MoE combine: x = (x + post3(post2(sum_j w_j d_j) + post1(dense))) * ascale, the dense row always on and ungated
class ArCombG4Base : ArBase {
    @ssbo @binding = 4 slot_idx : array<uint>    //! position -> its k slot rows
    @ssbo @binding = 5 slot_w : array<float>     //! ... and their routing weights (the per-expert down scale folded in)
    @ssbo @binding = 6 down_rows : array<float>  //! the routed down-GEMM output rows

    //! the position's k weighted routed rows at element k (one row, already summed, under `presummed`)
    def routed_sum(pk, k : uint) : float {
        if (pa.presummed != 0u) {
            return down_rows[k]
        }
        var f = 0.0
        var j = 0u
        while (j + 4u <= pa.slots) {
            let d0 = down_rows[slot_idx[pk + j] * pa.dim + k]
            let d1 = down_rows[slot_idx[pk + j + 1u] * pa.dim + k]
            let d2 = down_rows[slot_idx[pk + j + 2u] * pa.dim + k]
            let d3 = down_rows[slot_idx[pk + j + 3u] * pa.dim + k]
            f += slot_w[pk + j] * d0
            f += slot_w[pk + j + 1u] * d1
            f += slot_w[pk + j + 2u] * d2
            f += slot_w[pk + j + 3u] * d3
            j += 4u
        }
        while (j < pa.slots) {
            f += slot_w[pk + j] * down_rows[slot_idx[pk + j] * pa.dim + k]
            j++
        }
        return f
    }

    def override accum_row(base : uint) : float {
        let pk = (base / pa.dim) * pa.slots
        var sr = 0.0
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let r = routed_sum(pk, k)
            row[k] = r
            sr += r * r
            k += 256u
        }
        let inv_r = wg_rms_inv(sr, pa.dim, pa.eps, 0u)
        var sd = 0.0
        k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let a = aa[base + k]
            sd += a * a
            k += 256u
        }
        let inv_d = wg_rms_inv(sd, pa.dim, pa.eps, 1u)
        var sc = 0.0
        k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let c = wn[pa.routed_post_off + k] * (row[k] * inv_r) + wn[pa.pwoff + k] * (aa[base + k] * inv_d)
            row[k] = c
            sc += c * c
            k += 256u
        }
        let inv_c = wg_rms_inv(sc, pa.dim, pa.eps, 0u)   //! slot 0 again: the slot-1 reduce between sits every thread past this slot's readers
        var ss = 0.0
        k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let vv = (xs[base + k] + wn[pa.sum_post_off + k] * (row[k] * inv_c)) * pa.ascale
            xs[base + k] = vv
            row[k] = vv
            ss += vv * vv
            k += 256u
        }
        return ss
    }
}

[vk_dispatch(name = "cls_ar_comb_g4", grid = "nrows", params = "nrows : int64")]
class ClsArCombG4 : ArCombG4Base {
    @ssbo @binding = 3 yo : array<float>   //! the normed output rows

    [spirv_kernel(local_size_x = 256, name = "cls_ar_comb_g4_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block")]
    def run {
        let base = (gl_WorkGroupID.x + pa.row0) * pa.dim
        let ss = accum_row(base)
        let inv = rms_inv(ss)
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            yo[base + k] = wn[pa.woff + k] * (row[k] * inv)
            k += 256u
        }
    }
}

[vk_dispatch(name = "cls_ar_comb_g4_f16_b", grid = "nrows", params = "nrows : int64")]
class ClsArCombG4F16B : ArCombG4Base {
    @ssbo @binding = 3 outh : array<float16>   //! the normed rows, f16

    [spirv_kernel(local_size_x = 256, name = "cls_ar_comb_g4_f16_b_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block")]
    def run {
        let rbase = gl_WorkGroupID.x * pa.dim
        let ss = accum_row(rbase)
        let inv = rms_inv(ss)
        let last = pa.dim - 1u
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let k1 = min(k + 256u, last)
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            let n0 = normed(k, inv)
            let n1 = normed(k1, inv)
            let n2 = normed(k2, inv)
            let n3 = normed(k3, inv)
            outh[rbase + k] = float16(n0)
            if (k + 256u < pa.dim) {
                outh[rbase + k1] = float16(n1)
            }
            if (k + 512u < pa.dim) {
                outh[rbase + k2] = float16(n2)
            }
            if (k + 768u < pa.dim) {
                outh[rbase + k3] = float16(n3)
            }
            k += 1024u
        }
    }
}

//! the gemma-4 combine that quantizes the next head's feed itself: the normed row into Q8_0 blocks (`ArRqBase.quant32`'s
//! form at the combine's free bindings) instead of a float row a requant dispatch would read
[vk_dispatch(name = "cls_ar_comb_g4_rq", grid = "nrows", params = "nrows : int64")]
class ClsArCombG4Rq : ArCombG4Base {
    @ssbo @binding = 3 outq : array<int8>    //! the normed row's Q8_0 quants (the next layer's q feed)
    @ssbo @binding = 7 outs : array<float>   //! ... and its block scales

    [spirv_kernel(local_size_x = 256, name = "cls_ar_comb_g4_rq_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let base = (gl_WorkGroupID.x + pa.row0) * pa.dim
        let ss = accum_row(base)
        let inv = rms_inv(ss)
        let tid = gl_LocalInvocationID.x
        let nblk = pa.dim / 32u
        let lane8 = tid % 8u
        let qb0 = (gl_WorkGroupID.x + pa.row0) * nblk
        var b = tid / 8u
        while (b < nblk) {
            let e = b * 32u + lane8 * 4u
            let v0 = wn[pa.woff + e] * (row[e] * inv)
            let v1 = wn[pa.woff + e + 1u] * (row[e + 1u] * inv)
            let v2 = wn[pa.woff + e + 2u] * (row[e + 2u] * inv)
            let v3 = wn[pa.woff + e + 3u] * (row[e + 3u] * inv)
            let m = q8_amax3(max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3))))
            let dq = m / 127.0
            let id = dq != 0.0 ? 1.0 / dq : 0.0
            if (lane8 == 0u) {
                outs[qb0 + b] = dq
            }
            let qb = qb0 * 32u + e
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
        let last = pa.dim - 1u
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let k1 = min(k + 256u, last)
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            let n0 = normed(k, inv)
            let n1 = normed(k1, inv)
            let n2 = normed(k2, inv)
            let n3 = normed(k3, inv)
            outh[rbase + k] = float16(n0)
            if (k + 256u < pa.dim) {
                outh[rbase + k1] = float16(n1)
            }
            if (k + 512u < pa.dim) {
                outh[rbase + k2] = float16(n2)
            }
            if (k + 768u < pa.dim) {
                outh[rbase + k3] = float16(n3)
            }
            k += 1024u
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
    act : uint    // the activation code: ACT_SILU / ACT_GELU / ACT_OAI
    nblk : uint    // 32-block / superblock count (the requant kernels' guard)
    elem0 : uint   // a 32-multiple; ActF16 only - the requant classes ignore it
    ubase : uint   //! the u side's element base; with `ustride` the rows of `ulen` elements sit `ustride` apart (a side input stored position-major); 0/0/0 = u is laid out like gate
    ustride : uint
    ulen : uint
    bias_on : uint   //! the biased twins: gate and up take their expert's bias rows before the activation (gpt-oss)
    gboff : uint     //! ... this layer's row base in the gate-bias plane ([ne x brow] rows a layer)
    uboff : uint     //! ... and in the up-bias plane
    brow : uint      //! ... the row width: element e sits in row e / brow, whose expert `rex` names
}

//! gpt-oss's clamped swiglu (`swiglu_oai`): the clamps and the +1 are the trained activation, not a guard
let ACT_SILU = 0u
let ACT_GELU = 1u   //! tanh-gelu
let ACT_OAI = 2u    //! gpt-oss's clamped swiglu (`swiglu_oai`): the clamps and the +1 are the trained activation, not a guard

//! the act kernels' `act` push word from a model's flags
def act_code(oai, gelu : bool) : uint => oai ? ACT_OAI : (gelu ? ACT_GELU : ACT_SILU)

// the one activation formula (silu, tanh-gelu or the clamped swiglu on v, times u) — the classes' act_mul
// delegates pass their own pa.act
def private act_mul_sel(v, u : float; act : uint) : float {
    if (act == ACT_OAI) {
        let x = min(v, 7.0)
        let y = clamp(u, -7.0, 7.0)
        return (x / (1.0 + exp(-1.702 * x))) * (y + 1.0)
    }
    return (act == ACT_GELU ? (0.5 * v * (1.0 + tanh(0.7978845608028654 * (v + 0.044715 * v * v * v)))) * u
        : (v / (1.0 + exp(-v))) * u)
}

// members shared by the act/requant kernels
class ActRqMembers {
    @ssbo @binding = 0 gate : array<float>     // gate rows (act's v side)
    @ssbo @binding = 1 up : array<float>       // up rows (act's u side)
    @ssbo @binding = 2 outq : array<int8>      // quantized bytes out
    @ssbo @binding = 3 outs : array<float>     // per-block scales out
    @push_constant pa : ActArgs

    def act_mul(v, u : float) : float => act_mul_sel(v, u, pa.act)
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
            let ul = max(pa.ulen, 1u)   //! the select evaluates both arms: the flat form's ulen is 0, and a modulo by it is a fault under GPU-assisted validation
            let ub = pa.ustride != 0u ? pa.ubase + (eb / ul) * pa.ustride + eb % ul : pa.ubase + eb   //! four elements never cross a row: ulen is a 4-multiple
            let v0 = act_mul(gate[eb], up[ub])
            let v1 = act_mul(gate[eb + 1u], up[ub + 1u])
            let v2 = act_mul(gate[eb + 2u], up[ub + 2u])
            let v3 = act_mul(gate[eb + 3u], up[ub + 3u])
            blk_store(b, lane % 8u, v0, v1, v2, v3)
        }
    }
}

//! the biased twin of the fused act + Q8_0 requant: element e's row (e / brow) names the expert whose gate and up bias rows join before the activation
[vk_dispatch(name = "q8_actrq_b_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8ActRqB : Q8ActRq {
    @ssbo @binding = 4 @role = "weight" gb : array<float>   //! the gate-bias plane
    @ssbo @binding = 5 @role = "weight" ub : array<float>   //! the up-bias plane
    @ssbo @binding = 6 rex : array<uint>     //! row -> its expert

    //! the biased v, u pair of element e (the gate row's element and the up element at ue)
    def biased(e, ue : uint) : float2 {
        let ex = rex[e / pa.brow]
        let c = e % pa.brow
        return float2(gate[e] + gb[pa.gboff + ex * pa.brow + c], up[ue] + ub[pa.uboff + ex * pa.brow + c])
    }

    [spirv_kernel(local_size_x = 256, name = "q8_actrq_b_cls_spv")]
    def override run {
        let gid = gl_GlobalInvocationID.x
        let sg = gid / 32u
        let lane = gid % 32u
        let b = sg * 4u + lane / 8u
        if (b < pa.nblk) {
            let eb = sg * 128u + lane * 4u
            let ul = max(pa.ulen, 1u)
            let ub0 = pa.ustride != 0u ? pa.ubase + (eb / ul) * pa.ustride + eb % ul : pa.ubase + eb
            let p0 = biased(eb, ub0)
            let p1 = biased(eb + 1u, ub0 + 1u)
            let p2 = biased(eb + 2u, ub0 + 2u)
            let p3 = biased(eb + 3u, ub0 + 3u)
            blk_store(b, lane % 8u, act_mul(p0.x, p0.y), act_mul(p1.x, p1.y), act_mul(p2.x, p2.y), act_mul(p3.x, p3.y))
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

    def act_mul(v, u : float) : float => act_mul_sel(v, u, pa.act)

    [spirv_kernel(local_size_x = 256, name = "actf16_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
    def run {
        let e = pa.elem0 + gl_GlobalInvocationID.x * 4u
        if (e < pa.elem0 + pa.nelem) {
            outh[e] = float16(clamp(act_mul(gate[e], up[e]), -65504.0, 65504.0))
            outh[e + 1u] = float16(clamp(act_mul(gate[e + 1u], up[e + 1u]), -65504.0, 65504.0))
            outh[e + 2u] = float16(clamp(act_mul(gate[e + 2u], up[e + 2u]), -65504.0, 65504.0))
            outh[e + 3u] = float16(clamp(act_mul(gate[e + 3u], up[e + 3u]), -65504.0, 65504.0))
        }
    }
}

//! the biased twin of the f16 act: the window's bucket rows, row r's expert in `rex` (the schedule's bucket row -> expert map), its gate and up bias rows joining before the activation
[vk_dispatch(name = "actf16_b_cls", grid = "wgs", params = "wgs : int64")]
class ActF16B : ActF16 {
    @ssbo @binding = 3 @role = "weight" gb : array<float>   //! the gate-bias plane
    @ssbo @binding = 4 @role = "weight" ub : array<float>   //! the up-bias plane
    @ssbo @binding = 5 rex : array<uint>     //! bucket row -> its expert

    def bact(e : uint) : float16 {
        let ex = rex[e / pa.brow]
        let c = e % pa.brow
        return float16(clamp(act_mul(gate[e] + gb[pa.gboff + ex * pa.brow + c], up[e] + ub[pa.uboff + ex * pa.brow + c]), -65504.0, 65504.0))
    }

    [spirv_kernel(local_size_x = 256, name = "actf16_b_cls_spv")]
    def override run {
        let e = pa.elem0 + gl_GlobalInvocationID.x * 4u
        if (e < pa.elem0 + pa.nelem) {
            outh[e] = bact(e)
            outh[e + 1u] = bact(e + 1u)
            outh[e + 2u] = bact(e + 2u)
            outh[e + 3u] = bact(e + 3u)
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

struct ClsEpiArgs {
    vocab : uint
    cap : float     //! the final logit softcap; 0 = none
    nsupp : uint    //! suppressed token ids in `supp`
    rows : uint     //! logits rows in `y`, `vocab` apart: the N-row command's rows, 1 on the one-row command
}

//! gemma's logit softcap, cap * tanh(v / cap), in the exp form the CPU's softcap4 takes
def softcap_exp(v, cap : float) : float => cap * (1.0 - 2.0 / (1.0 + exp(2.0 * v / cap)))

//! the classifier tail's epilogue in place over `rows` logits rows: the final logit softcap, then the
//! checkpoint's suppressed ids pinned to -1e30 - after the cap, so the pin survives it
[vk_dispatch(name = "cls_epi", grid = "wgs", params = "wgs : int64")]
class ClsEpilogue {
    @ssbo @binding = 0 y : array<float>      // the logits rows
    @ssbo @binding = 1 @role = "weight" supp : array<uint>    // the suppressed ids (a placeholder when nsupp is 0)
    @push_constant pa : ClsEpiArgs

    [spirv_kernel(local_size_x = 256, name = "cls_epi_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.vocab * pa.rows) {
            let id = pa.nsupp > 0u ? gid % pa.vocab : 0u   // the row's id, wanted by the pins alone
            var v = y[gid]
            if (pa.cap > 0.0) {
                v = softcap_exp(v, pa.cap)
            }
            var j = 0u
            while (j < pa.nsupp) {
                if (supp[j] == id) {
                    v = -1.0e30
                }
                j++
            }
            y[gid] = v
        }
    }
}

let CLS_ARGMAX_CHUNKS = 64u   //! the chunks a row's pick splits into, compiled into both passes: a workgroup each over a 4096-wide slice of a 262144 vocab

struct ArgmaxArgs {
    vocab : uint
}

//! the pick's fold, shared by the two passes: a workgroup's first maximum over its threads' candidates - the subgroups'
//! maxes, then the lowest id among the lanes holding the widest (every thread returns the same answer)
class ClsArgmaxBase {
    @workgroup wm : float[32]   //! a subgroup's max: 256 threads make eight at the tier's subgroup floor of 32, thirty-two at a subgroup of 8
    @workgroup wi : uint[32]

    def fold(m : float; mi : uint) : uint {
        let sm = subgroupMax(m)
        let si = subgroupMin(m == sm ? mi : 0xFFFFFFFFu)
        if (gl_SubgroupInvocationID == 0u) {
            wm[gl_SubgroupID] = sm
            wi[gl_SubgroupID] = si
        }
        barrier()
        var bm = wm[0]
        var bi = wi[0]
        var sg = 1u
        while (sg < gl_NumSubgroups) {
            if (wm[sg] > bm || (wm[sg] == bm && wi[sg] < bi)) {
                bm = wm[sg]
                bi = wi[sg]
            }
            sg++
        }
        return bi
    }
}

//! the classifier tail's pick, the first pass: a workgroup a (row, chunk) over its slice of the row's logits, the slice's
//! first maximum and its id into the partial planes (`part[row * CLS_ARGMAX_CHUNKS + chunk]`); a thread's first element seeds its
//! candidate, so a row of equal or non-finite values still lands an id, and an empty slice lands none (`0xFFFFFFFF`)
[vk_dispatch(name = "cls_argmax_part", grid = "wgs", params = "wgs : int64")]
class ClsArgmaxPart : ClsArgmaxBase {
    @ssbo @binding = 0 y : array<float>        // the logits rows
    @ssbo @binding = 1 part_max : array<float>
    @ssbo @binding = 2 part_id : array<uint>
    @push_constant pa : ArgmaxArgs

    [spirv_kernel(local_size_x = 256, name = "cls_argmax_part_spv")]
    def run {
        let wg = gl_WorkGroupID.x
        let row = wg / CLS_ARGMAX_CHUNKS
        let len = (pa.vocab + CLS_ARGMAX_CHUNKS - 1u) / CLS_ARGMAX_CHUNKS
        let c0 = (wg % CLS_ARGMAX_CHUNKS) * len
        let c1 = min(pa.vocab, c0 + len)
        let base = row * pa.vocab
        let tid = gl_LocalInvocationID.x
        var m = -3.4e38
        var mi = 0xFFFFFFFFu
        var i = c0 + tid
        while (i < c1) {
            let v = y[base + i]
            if (mi == 0xFFFFFFFFu || v > m) {
                m = v
                mi = i
            }
            i += 256u
        }
        let bi = fold(m, mi)
        if (tid == 0u) {
            part_max[wg] = bi == 0xFFFFFFFFu ? -3.4e38 : y[base + bi]
            part_id[wg] = bi
        }
    }
}

//! ... the second pass: a workgroup a row over its `CLS_ARGMAX_CHUNKS` partials, the row's first maximum's id (the lowest id on a
//! tie, as the host's `parallel_argmax` reads, so a device pick and a host pick agree) into `pick`
[vk_dispatch(name = "cls_argmax_fin", grid = "rows", params = "rows : int64")]
class ClsArgmaxFin : ClsArgmaxBase {
    @ssbo @binding = 0 part_max : array<float>
    @ssbo @binding = 1 part_id : array<uint>
    @ssbo @binding = 2 pick : array<uint>
    @push_constant pa : ArgmaxArgs

    [spirv_kernel(local_size_x = 256, name = "cls_argmax_fin_spv")]
    def run {
        let row = gl_WorkGroupID.x
        let tid = gl_LocalInvocationID.x
        var m = -3.4e38
        var mi = 0xFFFFFFFFu
        var c = tid
        while (c < CLS_ARGMAX_CHUNKS) {
            let pm = part_max[row * CLS_ARGMAX_CHUNKS + c]
            let pi = part_id[row * CLS_ARGMAX_CHUNKS + c]
            if (pm > m || (pm == m && pi < mi)) {
                m = pm
                mi = pi
            }
            c += 256u
        }
        let bi = fold(m, mi)
        if (tid == 0u) {
            pick[row] = bi
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

struct DecCombineShArgs {
    n : uint      //! row width
    k : uint      //! routed slots
    gidx : uint   //! the shared expert's gate logit index in `gate`
    gated : uint
}

[vk_dispatch(name = "dec_combine_sh_cls", grid = "wgs", params = "wgs : int64")]
class DecCombineSh {
    @ssbo @binding = 0 xin : array<float>        //! the residual row before the FFN
    @ssbo @binding = 1 w : array<float>          //! the k routing weights
    @ssbo @binding = 2 down_rows : array<float>  //! the routed down GEMV's k rows
    @ssbo @binding = 3 sh_row : array<float>     //! the shared expert's down GEMV row
    @ssbo @binding = 4 gate : array<float>       //! the logits row carrying the gate at pa.gidx
    @ssbo @binding = 5 xout : array<float>       //! the residual row after the FFN
    @push_constant pa : DecCombineShArgs

    [spirv_kernel(local_size_x = 256, name = "dec_combine_sh_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            var acc = 0.0
            var j = 0u
            while (j < pa.k) {
                acc += w[j] * down_rows[j * pa.n + i]
                j++
            }
            let g = pa.gated != 0u ? 1.0 / (1.0 + exp(-gate[pa.gidx])) : 1.0
            xout[i] = xin[i] + acc + g * sh_row[i]
        }
    }
}

struct RouterArgs {
    ne : uint     // expert rows
    dim : uint    // row width
    woff : uint   // this layer's row base in the span's router plane (elements)
    obase : uint  // output row base (the resident deltanet arm lands beta/alpha rows in the smalls)
    norm_on : uint      //! the row read as its weightless rms under the norm row at `nwoff` (the gemma-4 router's input: the norm hop folded into the GEMV)
    nwoff : uint
    eps : float
    ncols : uint        //! the rows of x the dispatch serves, at most eight (the per-row planes' extent): each its own column of logits at `obase + c * ne`, or past `ne_split` at `obase_split + c * (ne - ne_split)`; the row's weights read once for all; 0 or 1 = one row
    ne_split : uint     //! the row the second output base takes over at (the deltanet beta rows at obase, the alpha rows at obase_split, one dispatch); 0 = one base
    obase_split : uint
}

[ |> template_struct_instance]
class template RouterGemvT {
    @template_constant PAIRS : bool = false   //!< the f16 stamp reads its row as half pairs, one word a step
    @ssbo @binding = 0 @role = "weight" w : array<KT>   // the rows, row-major [ne x dim] per layer
    @ssbo @binding = 0 @role = "alias" @template_gate = PAIRS w_pairs : array<uint>   //! the same rows as half pairs
    @ssbo @binding = 1 x : array<float>       // the FFN-normed row (the raw residual row under `norm_on`)
    @ssbo @binding = 2 logits : array<float>  // ne logits
    @ssbo @binding = 3 wn : array<float>      //! the norms plane: the row at `nwoff` scales x under `norm_on`
    @push_constant pa : RouterArgs
    @workgroup part : float[256]   //! a subgroup's eight column partials at `sg * 8`: the dot's in 0..127, the norm's in 128..255 - sixteen subgroups, the 256-thread workgroup at a 16-lane subgroup (the tier's floor is 32, dasllama_vulkan_common's subgroup gate)

    //! the row element at `i` of the row at `xb` as the dot reads it: under the norm row when `norm_on`
    def xn(xb, i : uint) : float => pa.norm_on != 0u ? x[xb + i] * wn[pa.nwoff + i] : x[xb + i]

    //! a workgroup an expert row: the grid is `ne` rows, whatever the column count
    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
    def run {   // nolint:STYLE038 — one row pass over the columns: the per-column rms, the dot, the per-column reduce and store
        let e = gl_WorkGroupID.x
        let nc = clamp(pa.ncols, 1u, 8u)
        let base = pa.woff + e * pa.dim
        let tid = gl_LocalInvocationID.x
        var inv : float[8]
        var c0 = 0u
        while (c0 < 8u) {
            inv[c0] = 1.0
            c0++
        }
        if (pa.norm_on != 0u) {
            var ss : float[8]
            var c1 = 0u
            while (c1 < nc) {
                var acc0 = 0.0
                var j = tid
                let xb = c1 * pa.dim
                while (j < pa.dim) {
                    let v = x[xb + j]
                    acc0 += v * v
                    j += 256u
                }
                ss[c1] = acc0
                c1++
            }
            var c2 = 0u
            while (c2 < nc) {
                let s0 = subgroupAdd(ss[c2])
                if (gl_SubgroupInvocationID == 0u) {
                    part[128u + gl_SubgroupID * 8u + c2] = s0
                }
                c2++
            }
            barrier()
            var c3 = 0u
            while (c3 < nc) {
                var tot0 = 0.0
                var sg0 = 0u
                while (sg0 < gl_NumSubgroups) {
                    tot0 += part[128u + sg0 * 8u + c3]
                    sg0++
                }
                inv[c3] = 1.0 / sqrt(tot0 / float(pa.dim) + pa.eps)
                c3++
            }
        }
        var acc : float[8]
        var c4 = 0u
        while (c4 < 8u) {
            acc[c4] = 0.0
            c4++
        }
        var i = tid
        static_if (PAIRS) {
            let hb = base >> 1u
            let hd = pa.dim >> 1u
            while (i < hd) {
                let p = unpackHalf2x16(w_pairs[hb + i])
                var c5 = 0u
                while (c5 < nc) {
                    let xb = c5 * pa.dim
                    acc[c5] += p.x * xn(xb, 2u * i) + p.y * xn(xb, 2u * i + 1u)
                    c5++
                }
                i += 256u
            }
        } else {
            while (i < pa.dim) {
                let wv = float(w[base + i])   // nolint:PERF020 -- KT converts; the f32 twin's cast is free
                var c6 = 0u
                while (c6 < nc) {
                    acc[c6] += wv * xn(c6 * pa.dim, i)
                    c6++
                }
                i += 256u
            }
        }
        var c7 = 0u
        while (c7 < nc) {
            let s = subgroupAdd(acc[c7])
            if (gl_SubgroupInvocationID == 0u) {
                part[gl_SubgroupID * 8u + c7] = s
            }
            c7++
        }
        barrier()
        if (gl_LocalInvocationID.x == 0u) {
            var c8 = 0u
            while (c8 < nc) {
                var tot = 0.0
                var sgi = 0u
                while (sgi < gl_NumSubgroups) {
                    tot += part[sgi * 8u + c8]
                    sgi++
                }
                var o = pa.obase + c8 * pa.ne + e
                if (pa.ne_split != 0u && e >= pa.ne_split) {
                    o = pa.obase_split + c8 * (pa.ne - pa.ne_split) + (e - pa.ne_split)
                }
                logits[o] = tot * inv[c8]
                c8++
            }
        }
    }
}

[vk_dispatch(name = "router_gemv_cls", grid = "ne", params = "ne : int64")]
class RouterGemv : RouterGemvT {
    typedef KT = float
}

[vk_dispatch(name = "router_gemv_f16_cls", grid = "ne", params = "ne : int64")]
class RouterGemvF16 : RouterGemvT {
    typedef KT = float16
    override PAIRS = true
}

struct DnBaArgs {
    npos : uint
    dim : uint
    nvh : uint       // beta rows (the alpha rows follow them in w)
    woff : uint      // this layer's [beta ; alpha] row base in w (elements)
    xstride : uint   // x row stride (elements)
    obase_b : uint   // beta output base in outp: row p's nvh values at obase_b + p * ostride
    obase_g : uint   // alpha output base, the same layout
    ostride : uint
}

[vk_dispatch(name = "dn_ba_cls", grid = "wgs", params = "wgs : int64")]
class DnBaGemm {
    @ssbo @binding = 0 @role = "weight" w : array<float16>   // the [beta ; alpha] rows per layer, f16 on the device
    @ssbo @binding = 1 x : array<float>       // the normed rows
    @ssbo @binding = 2 outp : array<float>    // the layer smalls (beta / g rows, position-major)
    @push_constant pa : DnBaArgs
    @workgroup xs : float[528]                 // 16 positions x 32 k, stride 33 (bank-spread)
    @workgroup ws : float[528]                 // 16 rows x 32 k, stride 33

    [spirv_kernel(local_size_x = 256, name = "dn_ba_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-dn-block")]
    def run {
        let tid = gl_LocalInvocationID.x
        let ne = 2u * pa.nvh
        let egroups = (ne + 15u) / 16u
        let p0 = (gl_WorkGroupID.x / egroups) * 16u
        let eg0 = (gl_WorkGroupID.x % egroups) * 16u
        let p = tid / 16u
        let el = tid % 16u
        let e = eg0 + el
        let live = p0 + p < pa.npos && e < ne
        let r = tid / 32u
        let k = tid % 32u
        var a = 0.0
        var kb = 0u
        while (kb < pa.dim) {
            if (p0 + r < pa.npos) {
                xs[r * 33u + k] = x[(p0 + r) * pa.xstride + kb + k]
            } else {
                xs[r * 33u + k] = 0.0
            }
            if (p0 + r + 8u < pa.npos) {
                xs[(r + 8u) * 33u + k] = x[(p0 + r + 8u) * pa.xstride + kb + k]
            } else {
                xs[(r + 8u) * 33u + k] = 0.0
            }
            if (eg0 + r < ne) {
                ws[r * 33u + k] = float(w[pa.woff + (eg0 + r) * pa.dim + kb + k])
            } else {
                ws[r * 33u + k] = 0.0
            }
            if (eg0 + r + 8u < ne) {
                ws[(r + 8u) * 33u + k] = float(w[pa.woff + (eg0 + r + 8u) * pa.dim + kb + k])
            } else {
                ws[(r + 8u) * 33u + k] = 0.0
            }
            barrier()
            if (live) {
                var kk = 0u
                while (kk < 32u) {
                    a += xs[p * 33u + kk] * ws[el * 33u + kk]
                    kk++
                }
            }
            barrier()
            kb += 32u
        }
        if (live) {
            outp[(e < pa.nvh ? pa.obase_b : pa.obase_g) + (p0 + p) * pa.ostride + (e % pa.nvh)] = a
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
    xnb2 : uint      // the down feed's blocks per hidden row: slot s reads row s (gate/up read their row's feed)
    dsoff : uint     //! this layer's row base in the per-expert down-scale plane (gemma-4: ffn_down_exps.scale folds into the routing weight); NO_DSCALE = none
    lstride : uint   //! the logits row stride (elements): row p's logits sit at p * lstride (the one-row dispatch reads row 0 whatever it holds)
    xnb1 : uint      //! the feed's blocks per row - one feed row serves the gate and the up (their formats share the block unit by the load's decline): row p's slots read it at p * xnb1 (0 on the one-row dispatch: the one feed row)
    rbias_on : uint  //! the router bias (gpt-oss): logit e takes rb[rboff + e] before the pick
    rboff : uint     //! ... this layer's row base in the router-bias plane ([ne] a layer)
}

let NO_DSCALE = 0xFFFFFFFFu   //! the top-k forms' "no per-expert down scale" sentinel

//! the select over one logit row, shared by the decode's single-row and the window's per-row
//! classes: thread tid holds logit lg, live = tid < ne, the picks land in pick / pickw for the
//! deriving class to write
class TopKCore {
    @workgroup part : float[64]
    @workgroup parti : uint[64]
    @workgroup pick : uint[64]
    @workgroup pickw : float[64]
    @workgroup wg_max : float[1]
    @workgroup wg_sum : float[1]

    def select_row(lg : float; live : bool; ne, k : uint) {
        let tid = gl_LocalInvocationID.x
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
        let wmax = wg_max[0u]
        let e = live ? exp(max(lg - wmax, -80.0)) : 0.0
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
        let wsum = wg_sum[0u]
        var p = live ? e / wsum : -3.0e38
        var j = 0u
        while (j < k) {
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
                    let pv = part[sgi]
                    let pi = parti[sgi]
                    if (pv > bv || (pv == bv && pi < bi)) {
                        bv = pv
                        bi = pi
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
    }

    //! pick jj's routing weight: renormalized to a unit sum when norm is set, then scaled
    def pick_weight(jj, k, norm : uint; wscale : float) : float {
        var ws = 0.0
        var i = 0u
        while (i < k) {
            ws += pickw[i]
            i++
        }
        ws = max(ws, F16_MIN_NORMAL)
        var wj = pickw[jj]
        if (norm != 0u) {
            wj /= ws
        }
        if (wscale != 0.0 && wscale != 1.0) {
            wj *= wscale
        }
        return wj
    }
}

//! the decode top-k's records: row `p`'s picks land in slots `p * k + j`
class TopKRecords : TopKCore {
    @ssbo @binding = 0 logits : array<float>  //! [npos x lstride]
    @ssbo @binding = 1 meta1 : array<uint>   //! the gate stack's (wblock, xblock) per slot row
    @ssbo @binding = 2 meta3 : array<uint>
    @ssbo @binding = 3 meta2 : array<uint>
    @ssbo @binding = 4 w : array<float>      //! the routing weights, a slot each
    @ssbo @binding = 5 @role = "weight" ds : array<float>     //! the per-expert down scales (gemma-4), read at dsoff when the push names a row
    @ssbo @binding = 6 @role = "weight" rb : array<float>     //! the router biases ([ne] a layer, gpt-oss), read at rboff under `rbias_on`
    @ssbo @binding = 7 eidx : array<uint>    //! the picks' experts, a slot each: the bias rows the act and the combine read
    @push_constant pa : TopkArgs

    //! row `p`: its logits selected, its k slots' records written by thread 0
    def row_picks(p : uint) {
        let tid = gl_LocalInvocationID.x
        let live = tid < pa.ne
        let e = min(tid, pa.ne - 1u)
        let lv = logits[p * pa.lstride + e] + (pa.rbias_on != 0u ? rb[pa.rboff + e] : 0.0)
        select_row(live ? lv : -3.0e38, live, pa.ne, pa.k)
        if (tid == 0u) {
            write_picks(p)
        }
    }

    def write_picks(p : uint) {
        var jj = 0u
        while (jj < pa.k) {
            let s = p * pa.k + jj
            let ex = min(pick[jj], pa.ne - 1u)   // a NaN row leaves the sentinel: never a wild weight block
            var wj = pick_weight(jj, pa.k, pa.norm, pa.wscale)
            if (pa.dsoff != NO_DSCALE) {
                wj *= ds[pa.dsoff + ex]
            }
            w[s] = wj
            eidx[s] = ex
            meta1[s * 2u] = pa.base1 + ex * pa.stride1
            meta1[s * 2u + 1u] = p * pa.xnb1
            meta3[s * 2u] = pa.base3 + ex * pa.stride3
            meta3[s * 2u + 1u] = p * pa.xnb1
            meta2[s * 2u] = pa.base2 + ex * pa.stride2
            meta2[s * 2u + 1u] = s * pa.xnb2
            jj++
        }
    }
}

[vk_dispatch(name = "topk_cls", grid = "one", params = "one : int64")]
class TopK : TopKRecords {
    [spirv_kernel(local_size_x = 256, name = "topk_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
    def run {
        row_picks(0u)
    }
}

[vk_dispatch(name = "topk_n_cls", grid = "npos", params = "npos : int64")]
class TopKN : TopKRecords {
    [spirv_kernel(local_size_x = 256, name = "topk_n_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
    def run {
        row_picks(gl_WorkGroupID.x)
    }
}

struct TopkRowsArgs {
    npos : uint
    ne : uint
    k : uint
    norm : uint      //! renormalize the selected weights to sum 1
    lstride : uint   //! the logits row stride (elements)
    wscale : float   //! 0 or 1 = none
    dsoff : uint     //! this layer's row base in the per-expert down-scale plane; NO_DSCALE = none
    rbias_on : uint  //! the router bias (gpt-oss): logit e takes rb[rboff + e] before the pick
    rboff : uint
}

[vk_dispatch(name = "topk_rows_cls", grid = "npos", params = "npos : int64")]
class TopKRows : TopKCore {
    @ssbo @binding = 0 logits : array<float>   //! [npos x lstride]
    @ssbo @binding = 1 idx : array<uint>       //! [npos x k] the picked expert per slot
    @ssbo @binding = 2 w : array<float>        //! ... and its routing weight
    @ssbo @binding = 3 @role = "weight" ds : array<float>       //! the per-expert down scales (gemma-4), read at dsoff when the push names a row
    @ssbo @binding = 4 @role = "weight" rb : array<float>       //! the router biases ([ne] a layer, gpt-oss), read at rboff under `rbias_on`
    @push_constant pa : TopkRowsArgs

    [spirv_kernel(local_size_x = 256, name = "topk_rows_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let tid = gl_LocalInvocationID.x
        let p = gl_WorkGroupID.x
        let live = tid < pa.ne
        let e = min(tid, pa.ne - 1u)
        let lv = logits[p * pa.lstride + e] + (pa.rbias_on != 0u ? rb[pa.rboff + e] : 0.0)
        select_row(live ? lv : -3.0e38, live, pa.ne, pa.k)
        if (tid == 0u) {
            var jj = 0u
            while (jj < pa.k) {
                let ex = min(pick[jj], pa.ne - 1u)
                var wj = pick_weight(jj, pa.k, pa.norm, pa.wscale)
                if (pa.dsoff != NO_DSCALE) {
                    wj *= ds[pa.dsoff + ex]
                }
                w[p * pa.k + jj] = wj
                idx[p * pa.k + jj] = ex
                jj++
            }
        }
    }
}

struct RouterGemmArgs {
    npos : uint      //! rows
    ne : uint        //! router rows (the experts, plus the shared expert's gate row when it rides along)
    dim : uint       //! row width
    woff : uint      //! this layer's row base in w (elements)
    ostride : uint   //! the logits row stride (elements)
}

let ROUTER_TP = 64u   //! the router tile's positions per workgroup
let ROUTER_TE = 32u   //! ... and router rows
let ROUTER_TH = ROUTER_TE / 2u   //! an invocation's two rows sit a half-tile apart: e and e + ROUTER_TH
let ROUTER_K4 = 16u   //! a 64-wide K step in float4
let ROUTER_LD = ROUTER_K4 + 1u   //! the stage's row stride in float4: the step plus one of pad, so the sixteen rows a warp reads (and a fetch writes) spread the eight bank groups

[vk_dispatch(name = "router_gemm_cls", grid = "wgs", params = "wgs : int64")]
class RouterGemm {
    @ssbo @binding = 0 @role = "weight" w4 : array<float4>   //! the router rows, row-major [ne x dim] per layer, as the stage's float4
    @ssbo @binding = 1 x4 : array<float4>      //! the FFN-normed rows, as float4
    @ssbo @binding = 2 logits : array<float>   //! [npos x ostride]
    @push_constant pa : RouterGemmArgs
    @workgroup xs : float4[int(ROUTER_TP * ROUTER_LD)]
    @workgroup ws : float4[int(ROUTER_TE * ROUTER_LD)]

    //! the stage's float4 of slot `s` (row s / ROUTER_K4, k4 s % ROUTER_K4) of x rows from `p0` at step `kb`; zero past the window
    def fetch_x(p0, kb, s : uint) : float4 {
        let row = s / ROUTER_K4
        if (p0 + row >= pa.npos) return float4(0.0)
        return x4[((p0 + row) * pa.dim + kb) / 4u + (s % ROUTER_K4)]
    }

    //! ... and of the router rows from `eg0`; zero past the experts
    def fetch_w(eg0, kb, s : uint) : float4 {
        let row = s / ROUTER_K4
        if (eg0 + row >= pa.ne) return float4(0.0)
        return w4[(pa.woff + (eg0 + row) * pa.dim + kb) / 4u + (s % ROUTER_K4)]
    }

    //! a fetched float4 into its stage row at k4 `s % ROUTER_K4`
    def stage_x(s : uint; v : float4) {
        xs[(s / ROUTER_K4) * ROUTER_LD + (s % ROUTER_K4)] = v
    }

    def stage_w(s : uint; v : float4) {
        ws[(s / ROUTER_K4) * ROUTER_LD + (s % ROUTER_K4)] = v
    }

    //! `a` plus the four products in k order - the scalar loop's sums to the bit
    def acc4(a : float; x, w : float4) : float {
        var s = a
        s += x.x * w.x
        s += x.y * w.y
        s += x.z * w.z
        s += x.w * w.w
        return s
    }

    [spirv_kernel(local_size_x = 256, name = "router_gemm_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let tid = gl_LocalInvocationID.x
        let egroups = (pa.ne + ROUTER_TE - 1u) / ROUTER_TE
        let p0 = (gl_WorkGroupID.x / egroups) * ROUTER_TP
        let eg0 = (gl_WorkGroupID.x % egroups) * ROUTER_TE
        let tp = tid / ROUTER_TH   //! the invocation's four positions 4tp..4tp+3
        let te = tid % ROUTER_TH   //! ... and rows te, te + ROUTER_TH
        var a00 = 0.0
        var a01 = 0.0
        var a10 = 0.0
        var a11 = 0.0
        var a20 = 0.0
        var a21 = 0.0
        var a30 = 0.0
        var a31 = 0.0
        //! the stage: 64 x rows x 16 float4 = four per invocation, 32 w rows x 16 = two
        var xr0 = fetch_x(p0, 0u, tid)
        var xr1 = fetch_x(p0, 0u, tid + 256u)
        var xr2 = fetch_x(p0, 0u, tid + 512u)
        var xr3 = fetch_x(p0, 0u, tid + 768u)
        var wr0 = fetch_w(eg0, 0u, tid)
        var wr1 = fetch_w(eg0, 0u, tid + 256u)
        var kb = 0u
        while (kb < pa.dim) {
            stage_x(tid, xr0)
            stage_x(tid + 256u, xr1)
            stage_x(tid + 512u, xr2)
            stage_x(tid + 768u, xr3)
            stage_w(tid, wr0)
            stage_w(tid + 256u, wr1)
            barrier()
            let nb = kb + 64u
            if (nb < pa.dim) {
                xr0 = fetch_x(p0, nb, tid)
                xr1 = fetch_x(p0, nb, tid + 256u)
                xr2 = fetch_x(p0, nb, tid + 512u)
                xr3 = fetch_x(p0, nb, tid + 768u)
                wr0 = fetch_w(eg0, nb, tid)
                wr1 = fetch_w(eg0, nb, tid + 256u)
            }
            let xb = (4u * tp) * ROUTER_LD
            let wb = te * ROUTER_LD
            let wb1 = wb + ROUTER_TH * ROUTER_LD
            var kk = 0u
            while (kk < ROUTER_K4) {
                let x0 = xs[xb + kk]
                let x1 = xs[xb + ROUTER_LD + kk]
                let x2 = xs[xb + 2u * ROUTER_LD + kk]
                let x3 = xs[xb + 3u * ROUTER_LD + kk]
                let w0 = ws[wb + kk]
                let w1 = ws[wb1 + kk]
                a00 = acc4(a00, x0, w0)
                a01 = acc4(a01, x0, w1)
                a10 = acc4(a10, x1, w0)
                a11 = acc4(a11, x1, w1)
                a20 = acc4(a20, x2, w0)
                a21 = acc4(a21, x2, w1)
                a30 = acc4(a30, x3, w0)
                a31 = acc4(a31, x3, w1)
                kk++
            }
            barrier()
            kb = nb
        }
        let pa0 = p0 + 4u * tp
        let ea0 = eg0 + te
        store_pair(pa0, ea0, a00, a01)
        store_pair(pa0 + 1u, ea0, a10, a11)
        store_pair(pa0 + 2u, ea0, a20, a21)
        store_pair(pa0 + 3u, ea0, a30, a31)
    }

    //! position p's two logits at rows e, e + ROUTER_TH, each within the window and the experts
    def store_pair(p, e : uint; v0, v1 : float) {
        if (p >= pa.npos) return
        if (e < pa.ne) {
            logits[p * pa.ostride + e] = v0
        }
        if (e + ROUTER_TH < pa.ne) {
            logits[p * pa.ostride + e + ROUTER_TH] = v1
        }
    }
}

struct SchedArgs {
    nk : uint          //! slot rows (npos x k)
    ne : uint          //! experts (at most 256: one thread per expert)
    wt_gu : uint       //! the gate/up GEMMs' weight tiles (ceil(nfe / 128))
    wt_dn : uint       //! the down GEMM's (ceil(dim / 128))
    map_off : uint     //! the s dispatch's per-wg map word offset in every sched plane; the records precede it
    map_off_mgu : uint //! the m dispatch's map offset in the gate/up planes
    map_off_mdn : uint //! ... and in the down plane
    bound_gu : uint    //! the s dispatch's workgroups per gate/up plane; the map's tail past the schedule reads the sentinel
    bound_dn : uint
    bound_mgu : uint   //! the m dispatch's
    bound_mdn : uint
    base1 : uint       //! expert 0's block base in the gate plane (slab-local, in the plane's units)
    stride1 : uint     //! blocks per expert
    base3 : uint
    stride3 : uint
    base2 : uint
    stride2 : uint
}

let SCHED_S_ROWS = 32u    //! the s column: the ladder's small-bucket and remainder tile
let SCHED_M_ROWS = 128u   //! the m column: whole columns over a big bucket, the last one partial
let CM2_TC_E = -1l        //! the class ladders' `tc` key for the expert schedule's e column: a sentinel no column width can be (the dense m stamp already keys by its 128 rows)

// the batch-tile class pick ladders: one arm per KqFmt member, stamped by kq_tile_stamp from the
// `tile_<verb>_<tail>` placeholders; a format without the stamp fails the compile, never the run
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#khr-mm-kq-tile")]
def khr_cls_ensure(fmt : int) : bool {
    var r = false
    kq_tile_stamp(fmt) <| $() {
        r = tile_ensure_khr_cls()
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#khr-mm-kq-tile")]
def khr_cls_set(fmt : int; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet {
    var r : VkDescriptorSet
    kq_tile_stamp(fmt) <| $() {
        r = tile_set_khr_cls(bufs, sizes, gbits)
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#khr-mm-kq-tile")]
def khr_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {
    kq_tile_stamp(fmt) <| $() {
        tile_enc_khr_cls(raw, h, s, pc, groups)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2e_cls_ensure(fmt : int) : bool {
    var r = false
    kq_tile_stamp(fmt) <| $() {
        r = tile_ensure_cm2e_cls()
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2e_cls_set(fmt : int; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet {
    var r : VkDescriptorSet
    kq_tile_stamp(fmt) <| $() {
        r = tile_set_cm2e_cls(bufs, sizes, gbits)
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2e_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {
    kq_tile_stamp(fmt) <| $() {
        tile_enc_cm2e_cls(raw, h, s, pc, groups)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_ensure(fmt : int; tc : int64) : bool {
    verify(fmt == int(KqFmt.q8) || fmt == int(KqFmt.q51) || fmt == int(KqFmt.mx4) || kq_sb(fmt), "vk classes: a cm2 tile for a format the f16 feed never admits")
    verify(tc == 256l || tc == 128l || tc == 32l || tc == CM2_TC_E, "vk classes: a cm2 tile column the pick never returns")
    if (g_gpu.coopmat_mode == COOPMAT_MM) {   //! the KHR arm is one 128 x 128 tile for every column, the expert schedule's s and m pieces included
        return khr_cls_ensure(fmt)
    }
    if (tc == CM2_TC_E) {
        return cm2e_cls_ensure(fmt)
    }
    if (fmt == int(KqFmt.q51)) {   //! the q51 rail is the expert schedule's: its s and e stamps; a dense q51 plane is declined at the plan
        verify(tc == 32l, "vk classes: a q51 plane reached a dense tile column - the resident plan admits q51 as a routed expert plane only")
        return ensure_q51_batch_cm2s_cls()
    }
    if (fmt == int(KqFmt.mx4)) {   //! the mx4 rail is the expert schedule's too: the loader tags only routed expert stacks mx4
        verify(tc == 32l, "vk classes: an mx4 plane reached a dense tile column - the resident plan admits mx4 as a routed expert plane only")
        return ensure_mx4_batch_cm2s_cls()
    }
    var r = false
    kq_tile_stamp(fmt, "q51 mx4") <| $() {
        r = tc == 256l ? tile_ensure_cm2l_cls() : (tc == 128l ? tile_ensure_cm2m_cls() : tile_ensure_cm2s_cls())
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_set(fmt : int; tc : int64; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet {
    verify(tc == 256l || tc == 128l || tc == 32l || tc == CM2_TC_E, "vk classes: a cm2 tile column the pick never returns")
    if (g_gpu.coopmat_mode == COOPMAT_MM) {
        return khr_cls_set(fmt, bufs, sizes, gbits)
    }
    if (tc == CM2_TC_E) {
        return cm2e_cls_set(fmt, bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.q51)) {
        verify(tc == 32l, "vk classes: a q51 plane reached a dense tile column - the resident plan admits q51 as a routed expert plane only")
        return set_q51_batch_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.mx4)) {
        verify(tc == 32l, "vk classes: an mx4 plane reached a dense tile column - the resident plan admits mx4 as a routed expert plane only")
        return set_mx4_batch_cm2s_cls(bufs, sizes, gbits)
    }
    var r : VkDescriptorSet
    kq_tile_stamp(fmt, "q51 mx4") <| $() {
        if (tc == 256l) {
            r = tile_set_cm2l_cls(bufs, sizes, gbits)
        } elif (tc == 128l) {
            r = tile_set_cm2m_cls(bufs, sizes, gbits)
        } else {
            r = tile_set_cm2s_cls(bufs, sizes, gbits)
        }
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_enc(fmt : int; tc : int64; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {
    verify(tc == 256l || tc == 128l || tc == 32l || tc == CM2_TC_E, "vk classes: a cm2 tile column the pick never returns")
    if (g_gpu.coopmat_mode == COOPMAT_MM) {
        khr_cls_enc(fmt, raw, h, s, pc, groups)
    } elif (tc == CM2_TC_E) {
        cm2e_cls_enc(fmt, raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.q51)) {
        verify(tc == 32l, "vk classes: a q51 plane reached a dense tile column - the resident plan admits q51 as a routed expert plane only")
        enc_q51_batch_cm2s_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.mx4)) {
        verify(tc == 32l, "vk classes: an mx4 plane reached a dense tile column - the resident plan admits mx4 as a routed expert plane only")
        enc_mx4_batch_cm2s_cls(raw, h, s, pc, groups)
    } else {
        kq_tile_stamp(fmt, "q51 mx4") <| $() {
            if (tc == 256l) {
                tile_enc_cm2l_cls(raw, h, s, pc, groups)
            } elif (tc == 128l) {
                tile_enc_cm2m_cls(raw, h, s, pc, groups)
            } else {
                tile_enc_cm2s_cls(raw, h, s, pc, groups)
            }
        }
    }
}

//! the ladder's m rows of a bucket of `c` rows: none up to the s column, else whole m columns plus a
//! partial one when the remainder passes the s column (a remainder within it takes the s column)
def sched_ladder_m_rows(c : uint) : uint {
    if (c <= SCHED_S_ROWS) return 0u
    return (c % SCHED_M_ROWS) > SCHED_S_ROWS ? c : (c / SCHED_M_ROWS) * SCHED_M_ROWS
}

[vk_dispatch(name = "moe_sched_cls", grid = "one", params = "one : int64")]
class MoeSched {
    @ssbo @binding = 0 idx : array<uint>       //! [nk] the picked expert per slot row
    @ssbo @binding = 1 inv : array<uint>       //! [nk] slot row -> bucket row
    @ssbo @binding = 2 sched1 : array<uint>    //! the gate plane's records + maps
    @ssbo @binding = 3 sched3 : array<uint>
    @ssbo @binding = 4 sched2 : array<uint>
    @ssbo @binding = 5 bex : array<uint>       //! [nk] bucket row -> its expert (the biased act's row map)
    @push_constant pa : SchedArgs
    @workgroup scan : uint[256]
    @workgroup cnt : uint[256]    //! the experts' slot counts, then their bucket cursors

    //! one piece's record and per-wg map on the three planes: expert `tid`'s `rows` from `row0`,
    //! record `ri`, its `rt` column tiles from workgroup `wg_gu` / `wg_dn` of the dispatch whose
    //! maps sit at `map_gu` / `map_dn`
    def write_piece(ri, row0, rows, rt, wg_gu, wg_dn, map_gu, map_dn : uint) {
        let tid = gl_LocalInvocationID.x
        let rb = ri * 4u
        sched1[rb] = pa.base1 + tid * pa.stride1
        sched1[rb + 1u] = row0
        sched1[rb + 2u] = rows
        sched1[rb + 3u] = wg_gu
        sched3[rb] = pa.base3 + tid * pa.stride3
        sched3[rb + 1u] = row0
        sched3[rb + 2u] = rows
        sched3[rb + 3u] = wg_gu
        sched2[rb] = pa.base2 + tid * pa.stride2
        sched2[rb + 1u] = row0
        sched2[rb + 2u] = rows
        sched2[rb + 3u] = wg_dn
        var g = 0u
        while (g < rt * pa.wt_gu) {
            sched1[map_gu + wg_gu + g] = ri
            sched3[map_gu + wg_gu + g] = ri
            g++
        }
        g = 0u
        while (g < rt * pa.wt_dn) {
            sched2[map_dn + wg_dn + g] = ri
            g++
        }
    }

    //! the sentinel tails of the four maps, from the live workgroup counts (the s count in the low
    //! half of each total, the m count in the high) to the dispatch bounds
    def write_tails(total_gu, total_dn : uint) {
        let tid = gl_LocalInvocationID.x
        var t = (total_gu & 0xFFFFu) + tid
        while (t < pa.bound_gu) {
            sched1[pa.map_off + t] = SCHED_NONE
            sched3[pa.map_off + t] = SCHED_NONE
            t += 256u
        }
        t = (total_dn & 0xFFFFu) + tid
        while (t < pa.bound_dn) {
            sched2[pa.map_off + t] = SCHED_NONE
            t += 256u
        }
        t = (total_gu >> 16u) + tid
        while (t < pa.bound_mgu) {
            sched1[pa.map_off_mgu + t] = SCHED_NONE
            sched3[pa.map_off_mgu + t] = SCHED_NONE
            t += 256u
        }
        t = (total_dn >> 16u) + tid
        while (t < pa.bound_mdn) {
            sched2[pa.map_off_mdn + t] = SCHED_NONE
            t += 256u
        }
    }

    //! the exclusive prefix over the 256 threads' v; scan[255] holds the total after the call
    def excl_scan(v : uint) : uint {
        let tid = gl_LocalInvocationID.x
        barrier()   //! the previous scan's readers are done with scan
        scan[tid] = v
        barrier()
        var off = 1u
        while (off < 256u) {
            var add = 0u
            if (tid >= off) {
                add = scan[tid - off]
            }
            barrier()
            scan[tid] += add
            barrier()
            off = off * 2u
        }
        return scan[tid] - v
    }

    [spirv_kernel(local_size_x = 256, name = "moe_sched_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let tid = gl_LocalInvocationID.x
        let mine = tid < pa.ne
        cnt[tid] = 0u
        barrier()
        var i = tid
        while (i < pa.nk) {   //! the top-k clamps every pick under ne, so the tally stays inside cnt
            let _c = atomicAdd(cnt[idx[i]], 1u)
            i += 256u
        }
        barrier()
        let c = mine ? cnt[tid] : 0u
        let st = excl_scan(c)
        let mrows = mine ? sched_ladder_m_rows(c) : 0u
        let srows = mine ? c - mrows : 0u
        let ri_s = excl_scan(srows > 0u ? 1u : 0u)
        let ri_m = pa.ne + excl_scan(mrows > 0u ? 1u : 0u)
        let rt_s = (srows + SCHED_S_ROWS - 1u) / SCHED_S_ROWS
        let rt_m = (mrows + SCHED_M_ROWS - 1u) / SCHED_M_ROWS
        //! the packed word each scan carries: the s tile count in the low half, the m count in the high
        let g_gu = excl_scan(rt_s * pa.wt_gu | ((rt_m * pa.wt_gu) << 16u))
        let total_gu = scan[255u]
        let g_dn = excl_scan(rt_s * pa.wt_dn | ((rt_m * pa.wt_dn) << 16u))
        let total_dn = scan[255u]
        if (srows > 0u) {
            write_piece(ri_s, st + mrows, srows, rt_s, g_gu & 0xFFFFu, g_dn & 0xFFFFu, pa.map_off, pa.map_off)
        }
        if (mrows > 0u) {
            write_piece(ri_m, st, mrows, rt_m, g_gu >> 16u, g_dn >> 16u, pa.map_off_mgu, pa.map_off_mdn)
        }
        write_tails(total_gu, total_dn)
        barrier()   //! every thread has read its count
        cnt[tid] = st
        barrier()
        i = tid
        while (i < pa.nk) {
            let b = atomicAdd(cnt[idx[i]], 1u)
            inv[i] = b
            bex[b] = idx[i]
            i += 256u
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

    [spirv_kernel(local_size_x = 256, name = "moe_gather_f16_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
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
    npos : uint       // the window's rows
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

[vk_dispatch(name = "dn_conv_cls", grid = "wgs", params = "wgs : int64")]
class DnConv {
    @ssbo @binding = 0 src : array<float>      // the projected rows (position-major)
    @ssbo @binding = 1 outr : array<float>     // conv output rows
    @ssbo @binding = 2 smalls : array<float>   // taps + history + any small planes (push offsets)
    @push_constant pa : DnConvArgs
    @workgroup wsum : float[int(8u * DN_CONV_PB)]   //! the eight 32-lane blocks' per-position sums of squares

    //! channel ch's input at window row sp: a window row, or the history row sp + taps below the window in the layout hist_mode names
    def xrow(sp : int; ch, taps : uint) : float {
        if (sp >= 0) {
            return src[uint(sp) * pa.cd + ch]
        }
        let hidx = uint(max(sp + int(taps), 0))
        if (pa.hist_mode == 1u) {
            return smalls[pa.hist_off + ch * taps + hidx]
        }
        return smalls[pa.hist_off + hidx * pa.cd + ch]
    }

    [spirv_kernel(local_size_x = 256, name = "dn_conv_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-dn-block")]
    def run {
        let cblocks = (pa.cd + 255u) / 256u
        let p0 = (gl_WorkGroupID.x / cblocks) * DN_CONV_PB
        let ch = (gl_WorkGroupID.x % cblocks) * 256u + gl_LocalInvocationID.x
        let live = ch < pa.cd
        let cch = live ? ch : 0u               // a dead lane reads channel 0 and stores nothing
        let taps = pa.dconv - 1u
        let lead = 4u - pa.dconv               // the ring holds the last four rows whatever dconv: the leading taps read zero
        var w : float[4]
        for [unroll] (j in range(4)) {
            let jj = uint(j)
            w[j] = jj >= lead ? smalls[pa.taps_off + cch * pa.dconv + (jj - lead)] : 0.0
        }
        var x0 = xrow(int(p0) - 3, cch, taps)
        var x1 = xrow(int(p0) - 2, cch, taps)
        var x2 = xrow(int(p0) - 1, cch, taps)
        let normed = live && ch < 2u * pa.kd
        var y : float[int(DN_CONV_PB)]
        var s : float[int(DN_CONV_PB)]
        for [unroll] (i in range(int(DN_CONV_PB))) {
            let p = p0 + uint(i)
            let x3 = p < pa.npos ? xrow(int(p), cch, taps) : 0.0
            let acc = w[0] * x0 + w[1] * x1 + w[2] * x2 + w[3] * x3
            y[i] = (live && p < pa.npos) ? acc / (1.0 + exp(-acc)) : 0.0
            x0 = x1
            x1 = x2
            x2 = x3
            //! the head's sum of squares over an aligned 32-lane block (ds lanes below 32): the fold's masks stay inside the block on any subgroup width
            var v = normed ? y[i] * y[i] : 0.0
            if (pa.ds > 16u) {
                v += subgroupShuffleXor(v, 16u)
            }
            v += subgroupShuffleXor(v, 8u)
            v += subgroupShuffleXor(v, 4u)
            v += subgroupShuffleXor(v, 2u)
            v += subgroupShuffleXor(v, 1u)
            s[i] = v
        }
        let blk = gl_LocalInvocationID.x >> 5u   //! the lane block by the invocation id, never the subgroup: a 64-lane subgroup is two blocks
        if ((gl_LocalInvocationID.x & 31u) == 0u) {
            for [unroll] (i in range(int(DN_CONV_PB))) {
                wsum[blk * DN_CONV_PB + uint(i)] = s[i]
            }
        }
        barrier()
        let blocks_per_head = pa.ds > 32u ? pa.ds / 32u : 1u
        let wb = (blk / blocks_per_head) * blocks_per_head
        let qs = ch < pa.kd ? pa.qscale : 1.0
        for [unroll] (i in range(int(DN_CONV_PB))) {
            let p = p0 + uint(i)
            var tot = s[i]
            if (blocks_per_head > 1u) {
                tot = 0.0
                var ww = 0u
                while (ww < blocks_per_head) {
                    tot += wsum[(wb + ww) * DN_CONV_PB + uint(i)]
                    ww++
                }
            }
            let sc = normed ? (1.0 / max(sqrt(tot), pa.eps)) * qs : 1.0
            if (live && p < pa.npos) {
                outr[p * pa.cd + ch] = y[i] * sc
            }
        }
    }
}

struct DnTailArgs {
    cd : uint         // conv width (channels)
    taps : uint       // history rows (dconv - 1)
    row0 : uint       // the first tail row in src (the window's last taps raw rows)
    hist_off : uint   // the ring image's float offset in smalls
    zero_rows : uint  // leading ring rows that read as zero: a lone window shorter than the taps, whose history is zero
}

[vk_dispatch(name = "dn_tail_cls", grid = "wgs", params = "wgs : int64")]
class DnTail {
    @ssbo @binding = 0 src : array<float>      // the projected rows (position-major)
    @ssbo @binding = 1 smalls : array<float>   // the layer's smalls (the ring image at hist_off)
    @push_constant pa : DnTailArgs

    [spirv_kernel(local_size_x = 256, name = "dn_tail_cls_spv")]
    def run {
        let idx = gl_GlobalInvocationID.x
        if (idx < pa.cd * pa.taps) {
            let ch = idx / pa.taps
            let k = idx % pa.taps
            var v = 0.0
            if (k >= pa.zero_rows) {
                v = src[(pa.row0 + k - pa.zero_rows) * pa.cd + ch]
            }
            smalls[pa.hist_off + ch * pa.taps + k] = v
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
    qscale : float
    out_f32 : uint
    beta_row_stride : uint
}

[vk_dispatch(name = "dn_step_cls", grid = "wgs", params = "wgs : int64")]
class DnStepFused {
    @ssbo @binding = 0 proj : array<float>     // the fresh projection row (+ z rows at cd)
    @ssbo @binding = 1 state : array<float>    // the persistent per-head f32 state (in place)
    @ssbo @binding = 2 smalls : array<float>   // beta/g/taps/wnorm/a/dt + the conv HISTORY ring (rw)
    @ssbo @binding = 3 outs : array<float>     // the head's Q8_0 scales
    @ssbo @binding = 4 outq : array<uint>      // ... and quant words
    @ssbo @binding = 5 tok : array<TokMeta>    // the shared per-token block (element 0)
    @ssbo @binding = 6 outf : array<float>     // the head's o row as f32
    @push_constant pa : DnStepArgs
    @workgroup dq : float[128]
    @workgroup dk : float[128]
    @workgroup dv : float[128]
    @workgroup dvn : float[128]
    @workgroup dout : float[128]
    @workgroup partk : float[256]   //! each row part's k.S column partials (part-major, ds wide: the thread's own slot)
    @workgroup partq : float[256]   //! ... and q.S
    @workgroup sg_sumsq : float[16]   //! per-subgroup partial squared sums (q and k), re-used for the out-norm's after the second barrier
    @workgroup sg_qk : float[16]      //! per-subgroup partials of the raw q.k
    @workgroup ds8 : float[8]

    [spirv_kernel(local_size_x = 256, name = "dn_step_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
    def run {   // nolint:STYLE037,STYLE038 — the fused chain: conv, norms, the delta rule, out-norm and the o row in one dispatch
        let ds = pa.ds
        let nvh = max(1u, (pa.cd - 2u * pa.kd) / ds)   //! the head count the geometry pins: cd = 2 kd + nvh ds
        let row = gl_WorkGroupID.x / nvh
        let h = gl_WorkGroupID.x % nvh
        let taps = pa.dconv - 1u
        let par = tok[row].parity
        let slot = tok[row].dnslot
        let prow = row * (pa.cd + nvh * ds)
        let ringb = pa.hist_off + slot * 2u * pa.cd * taps
        let rrb = ringb + par * pa.cd * taps
        let wrb = ringb + (1u - par) * pa.cd * taps
        let tid = gl_LocalInvocationID.x
        let kh = h % pa.nkh
        var e = tid
        while (e < 3u * ds) {
            let sl = e / ds
            let j = e % ds
            let ch = (sl == 0u ? kh * ds : (sl == 1u ? pa.kd + kh * ds : 2u * pa.kd + h * ds)) + j
            let fresh = proj[prow + ch]
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
        //! the q and k squared sums and the raw q.k by subgroup adds: the first half of the threads hold q, the second k
        var pn = 0.0
        var pq = 0.0
        if (tid < ds) {
            pn = dq[tid] * dq[tid]
            pq = dq[tid] * dk[tid]
        } elif (tid >= 128u && tid < 128u + ds) {
            pn = dk[tid - 128u] * dk[tid - 128u]
        }
        let sn = subgroupAdd(pn)
        let spq = subgroupAdd(pq)
        if (gl_SubgroupInvocationID == 0u) {
            sg_sumsq[gl_SubgroupID] = sn
            sg_qk[gl_SubgroupID] = spq
        }
        barrier()
        if (tid == 0u) {   //! q.k is summed raw; the q and k scales apply here
            var sumq = 0.0
            var sumk = 0.0
            var qkr = 0.0
            let half = gl_NumSubgroups / 2u
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                if (sgi < half) {
                    sumq += sg_sumsq[sgi]
                    qkr += sg_qk[sgi]
                } else {
                    sumk += sg_sumsq[sgi]
                }
                sgi++
            }
            let sq = (1.0 / max(sqrt(sumq), pa.eps)) * pa.qscale
            let sk = 1.0 / max(sqrt(sumk), pa.eps)
            ds8[0u] = sq
            ds8[1u] = sk
            let graw = smalls[pa.g_off + row * nvh + h] + smalls[pa.dt_off + h]
            ds8[2u] = exp(smalls[pa.a_off + h] * (graw > 20.0 ? graw : log(1.0 + exp(graw))))
            ds8[3u] = 1.0 / (1.0 + exp(-smalls[pa.beta_off + row * pa.beta_row_stride + h]))
            ds8[4u] = qkr * sq * sk
        }
        barrier()
        let sq = ds8[0u]
        let sk = ds8[1u]
        let eg = ds8[2u]
        let bt = ds8[3u]
        let qk = ds8[4u]
        let rows_per_part = min(ds, 64u)
        let nparts = ds / rows_per_part
        let active = tid < ds * nparts
        let c = tid % ds
        let r0 = (tid / ds) * rows_per_part
        let sbase = slot * nvh * ds * ds + h * ds * ds
        var sreg : float[64]
        var ksp = 0.0
        var qsp = 0.0
        if (active) {
            //! the loads first, straight-line (a short part re-reads its last row), so all 64 issue before a sum waits on one
            for [unroll] (j in range(64)) {
                sreg[j] = state[sbase + (r0 + min(uint(j), rows_per_part - 1u)) * ds + c]
            }
            for [unroll] (j in range(64)) {
                if (uint(j) < rows_per_part) {
                    ksp += dk[r0 + uint(j)] * sreg[j]
                    qsp += dq[r0 + uint(j)] * sreg[j]
                }
            }
        }
        if (active) {
            partk[tid] = ksp
            partq[tid] = qsp
        }
        barrier()
        if (tid < ds) {
            var ks = 0.0
            var qs = 0.0
            var p = 0u
            while (p < nparts) {
                ks += partk[p * ds + tid]
                qs += partq[p * ds + tid]
                p++
            }
            let vn = bt * (dv[tid] - eg * (sk * ks))
            dvn[tid] = vn
            dout[tid] = eg * (sq * qs) + qk * vn
        }
        barrier()
        if (active) {
            let vnc = dvn[c]
            for [unroll] (j in range(64)) {
                if (uint(j) < rows_per_part) {
                    state[sbase + (r0 + uint(j)) * ds + c] = sreg[j] * eg + (sk * dk[r0 + uint(j)]) * vnc
                }
            }
        }
        var po = 0.0
        if (tid < ds) {
            po = dout[tid] * dout[tid]
        }
        let so = subgroupAdd(po)
        if (gl_SubgroupInvocationID == 0u) {
            sg_sumsq[gl_SubgroupID] = so
        }
        barrier()
        if (tid == 0u) {
            var sum = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                sum += sg_sumsq[sgi]
                sgi++
            }
            ds8[5u] = 1.0 / sqrt(sum / float(ds) + pa.eps)
        }
        barrier()
        e = tid
        while (e < ds) {
            let zv = proj[prow + pa.cd + h * ds + e]
            let xn = dout[e] * ds8[5u] * smalls[pa.wnorm_off + e]
            dout[e] = xn * (zv / (1.0 + exp(-zv)))
            e += 256u
        }
        barrier()
        let orow = (row * nvh + h) * ds
        if (pa.out_f32 != 0u) {
            e = tid
            while (e < ds) {
                outf[orow + e] = dout[e]
                e += 256u
            }
        } elif (tid < ds / 32u) {   // the head's Q8_0 blocks (ds % 32 asserted host-side)
            let gb = orow / 32u + tid
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

// ===== deltanet: the sequential scan + the gated out-norm =====

struct DnScanArgs {
    npos : uint
    cd : uint
    kd : uint
    nvh : uint
    nkh : uint
    ds : uint
    beta_off : uint   // raw beta per (pos, head) in smalls; sigmoid in kernel
    g_off : uint      // raw g per (pos, head); the decay is exp(a * softplus(g + dt))
    a_off : uint
    dt_off : uint
    o_off : uint      // the raw o rows' base in orows (position-major, di wide)
    di : uint
    region_state_base : uint
}

[vk_dispatch(name = "dn_scan_cls", grid = "wgs", params = "wgs : int64")]
class DnScan {
    @ssbo @binding = 0 @readonly conv : array<float>     //! the conv output rows (position-major: q | k | v)
    @ssbo @binding = 1 state : array<float>              // the persistent per-head f32 state [nvh][ds][ds]
    @ssbo @binding = 2 @readonly smalls : array<float>   // raw beta/g rows, a/dt per head (push offsets)
    @ssbo @binding = 3 orows : array<float>              // the raw o rows at o_off
    @push_constant pa : DnScanArgs
    @workgroup egs : float[int(DN_WINDOW)]   //! the head's decay per token, exp(a * softplus(g + dt)), staged once per workgroup
    @workgroup bts : float[int(DN_WINDOW)]   //! the head's sigmoid(beta) per token, the same way

    def cluster_add(v : float; lanes_per_col : uint) : float {
        var r = v
        if (lanes_per_col > 4u) {
            r += subgroupShuffleXor(r, 4u)
        }
        if (lanes_per_col > 2u) {
            r += subgroupShuffleXor(r, 2u)
        }
        if (lanes_per_col > 1u) {
            r += subgroupShuffleXor(r, 1u)
        }
        return r
    }

    [spirv_kernel(local_size_x = 128, name = "dn_scan_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-dn-block")]
    def run {   // nolint:STYLE038 — the per-token recurrence over 16 register rows of one column, written out
        let ds = pa.ds
        let lanes_per_col = ds / 16u           // 16 state rows each; ds % 16 == 0 host-side
        let cols_per_block = 32u / lanes_per_col   //! the 128-thread workgroup's four 32-lane blocks, by the invocation id: a cluster's shuffles (masks under 8) stay inside a block on any subgroup width
        let cols_per_group = 4u * cols_per_block
        let cgroups = (ds + cols_per_group - 1u) / cols_per_group
        let h = gl_WorkGroupID.x / cgroups
        let sl = gl_LocalInvocationID.x & 31u
        let lane = sl % lanes_per_col
        let col = (gl_WorkGroupID.x % cgroups) * cols_per_group + (gl_LocalInvocationID.x >> 5u) * cols_per_block + sl / lanes_per_col
        let live = col < ds                    // a whole cluster shares col, so its shuffles stay uniform
        let kh = h % pa.nkh
        let q_elem0 = kh * ds + lane
        let k_elem0 = pa.kd + kh * ds + lane
        let voff = 2u * pa.kd + h * ds + (live ? col : 0u)   // an idle lane reads column 0: past the last head its own column is past the plane
        let aa = smalls[pa.a_off + h]
        let dtb = smalls[pa.dt_off + h]
        var ti = gl_LocalInvocationID.x
        while (ti < pa.npos) {
            let graw = smalls[pa.g_off + ti * pa.nvh + h] + dtb
            egs[ti] = exp(aa * (graw > 20.0 ? graw : log(1.0 + exp(graw))))
            bts[ti] = 1.0 / (1.0 + exp(-smalls[pa.beta_off + ti * pa.nvh + h]))
            ti += 128u
        }
        barrier()
        let sb = pa.region_state_base + h * ds * ds + (live ? col : 0u) + lane * ds
        let row_stride = lanes_per_col * ds
        var s : float[16]
        s[0] = state[sb]
        s[1] = state[sb + row_stride]
        s[2] = state[sb + 2u * row_stride]
        s[3] = state[sb + 3u * row_stride]
        s[4] = state[sb + 4u * row_stride]
        s[5] = state[sb + 5u * row_stride]
        s[6] = state[sb + 6u * row_stride]
        s[7] = state[sb + 7u * row_stride]
        s[8] = state[sb + 8u * row_stride]
        s[9] = state[sb + 9u * row_stride]
        s[10] = state[sb + 10u * row_stride]
        s[11] = state[sb + 11u * row_stride]
        s[12] = state[sb + 12u * row_stride]
        s[13] = state[sb + 13u * row_stride]
        s[14] = state[sb + 14u * row_stride]
        s[15] = state[sb + 15u * row_stride]
        //! the last token re-reads its own row
        var k_next : float[16]
        var q_next : float[16]
        var v_next = conv[voff]
        k_next[0] = conv[k_elem0]
        k_next[1] = conv[k_elem0 + lanes_per_col]
        k_next[2] = conv[k_elem0 + 2u * lanes_per_col]
        k_next[3] = conv[k_elem0 + 3u * lanes_per_col]
        k_next[4] = conv[k_elem0 + 4u * lanes_per_col]
        k_next[5] = conv[k_elem0 + 5u * lanes_per_col]
        k_next[6] = conv[k_elem0 + 6u * lanes_per_col]
        k_next[7] = conv[k_elem0 + 7u * lanes_per_col]
        k_next[8] = conv[k_elem0 + 8u * lanes_per_col]
        k_next[9] = conv[k_elem0 + 9u * lanes_per_col]
        k_next[10] = conv[k_elem0 + 10u * lanes_per_col]
        k_next[11] = conv[k_elem0 + 11u * lanes_per_col]
        k_next[12] = conv[k_elem0 + 12u * lanes_per_col]
        k_next[13] = conv[k_elem0 + 13u * lanes_per_col]
        k_next[14] = conv[k_elem0 + 14u * lanes_per_col]
        k_next[15] = conv[k_elem0 + 15u * lanes_per_col]
        q_next[0] = conv[q_elem0]
        q_next[1] = conv[q_elem0 + lanes_per_col]
        q_next[2] = conv[q_elem0 + 2u * lanes_per_col]
        q_next[3] = conv[q_elem0 + 3u * lanes_per_col]
        q_next[4] = conv[q_elem0 + 4u * lanes_per_col]
        q_next[5] = conv[q_elem0 + 5u * lanes_per_col]
        q_next[6] = conv[q_elem0 + 6u * lanes_per_col]
        q_next[7] = conv[q_elem0 + 7u * lanes_per_col]
        q_next[8] = conv[q_elem0 + 8u * lanes_per_col]
        q_next[9] = conv[q_elem0 + 9u * lanes_per_col]
        q_next[10] = conv[q_elem0 + 10u * lanes_per_col]
        q_next[11] = conv[q_elem0 + 11u * lanes_per_col]
        q_next[12] = conv[q_elem0 + 12u * lanes_per_col]
        q_next[13] = conv[q_elem0 + 13u * lanes_per_col]
        q_next[14] = conv[q_elem0 + 14u * lanes_per_col]
        q_next[15] = conv[q_elem0 + 15u * lanes_per_col]
        var t = 0u
        while (t < pa.npos) {
            let eg = egs[t]
            let bt = bts[t]
            let v = v_next
            var k : float[16]
            var q : float[16]
            k[0] = k_next[0]
            k[1] = k_next[1]
            k[2] = k_next[2]
            k[3] = k_next[3]
            k[4] = k_next[4]
            k[5] = k_next[5]
            k[6] = k_next[6]
            k[7] = k_next[7]
            k[8] = k_next[8]
            k[9] = k_next[9]
            k[10] = k_next[10]
            k[11] = k_next[11]
            k[12] = k_next[12]
            k[13] = k_next[13]
            k[14] = k_next[14]
            k[15] = k_next[15]
            q[0] = q_next[0]
            q[1] = q_next[1]
            q[2] = q_next[2]
            q[3] = q_next[3]
            q[4] = q_next[4]
            q[5] = q_next[5]
            q[6] = q_next[6]
            q[7] = q_next[7]
            q[8] = q_next[8]
            q[9] = q_next[9]
            q[10] = q_next[10]
            q[11] = q_next[11]
            q[12] = q_next[12]
            q[13] = q_next[13]
            q[14] = q_next[14]
            q[15] = q_next[15]
            let crow_next = min(t + 1u, pa.npos - 1u) * pa.cd
            v_next = conv[crow_next + voff]
            let kbn = crow_next + k_elem0
            k_next[0] = conv[kbn]
            k_next[1] = conv[kbn + lanes_per_col]
            k_next[2] = conv[kbn + 2u * lanes_per_col]
            k_next[3] = conv[kbn + 3u * lanes_per_col]
            k_next[4] = conv[kbn + 4u * lanes_per_col]
            k_next[5] = conv[kbn + 5u * lanes_per_col]
            k_next[6] = conv[kbn + 6u * lanes_per_col]
            k_next[7] = conv[kbn + 7u * lanes_per_col]
            k_next[8] = conv[kbn + 8u * lanes_per_col]
            k_next[9] = conv[kbn + 9u * lanes_per_col]
            k_next[10] = conv[kbn + 10u * lanes_per_col]
            k_next[11] = conv[kbn + 11u * lanes_per_col]
            k_next[12] = conv[kbn + 12u * lanes_per_col]
            k_next[13] = conv[kbn + 13u * lanes_per_col]
            k_next[14] = conv[kbn + 14u * lanes_per_col]
            k_next[15] = conv[kbn + 15u * lanes_per_col]
            let qbn = crow_next + q_elem0
            q_next[0] = conv[qbn]
            q_next[1] = conv[qbn + lanes_per_col]
            q_next[2] = conv[qbn + 2u * lanes_per_col]
            q_next[3] = conv[qbn + 3u * lanes_per_col]
            q_next[4] = conv[qbn + 4u * lanes_per_col]
            q_next[5] = conv[qbn + 5u * lanes_per_col]
            q_next[6] = conv[qbn + 6u * lanes_per_col]
            q_next[7] = conv[qbn + 7u * lanes_per_col]
            q_next[8] = conv[qbn + 8u * lanes_per_col]
            q_next[9] = conv[qbn + 9u * lanes_per_col]
            q_next[10] = conv[qbn + 10u * lanes_per_col]
            q_next[11] = conv[qbn + 11u * lanes_per_col]
            q_next[12] = conv[qbn + 12u * lanes_per_col]
            q_next[13] = conv[qbn + 13u * lanes_per_col]
            q_next[14] = conv[qbn + 14u * lanes_per_col]
            q_next[15] = conv[qbn + 15u * lanes_per_col]
            var kv = k[0] * s[0]
            kv += k[1] * s[1]
            kv += k[2] * s[2]
            kv += k[3] * s[3]
            kv += k[4] * s[4]
            kv += k[5] * s[5]
            kv += k[6] * s[6]
            kv += k[7] * s[7]
            kv += k[8] * s[8]
            kv += k[9] * s[9]
            kv += k[10] * s[10]
            kv += k[11] * s[11]
            kv += k[12] * s[12]
            kv += k[13] * s[13]
            kv += k[14] * s[14]
            kv += k[15] * s[15]
            kv = cluster_add(kv, lanes_per_col)
            let delta = bt * (v - eg * kv)
            s[0] = s[0] * eg + k[0] * delta
            s[1] = s[1] * eg + k[1] * delta
            s[2] = s[2] * eg + k[2] * delta
            s[3] = s[3] * eg + k[3] * delta
            s[4] = s[4] * eg + k[4] * delta
            s[5] = s[5] * eg + k[5] * delta
            s[6] = s[6] * eg + k[6] * delta
            s[7] = s[7] * eg + k[7] * delta
            s[8] = s[8] * eg + k[8] * delta
            s[9] = s[9] * eg + k[9] * delta
            s[10] = s[10] * eg + k[10] * delta
            s[11] = s[11] * eg + k[11] * delta
            s[12] = s[12] * eg + k[12] * delta
            s[13] = s[13] * eg + k[13] * delta
            s[14] = s[14] * eg + k[14] * delta
            s[15] = s[15] * eg + k[15] * delta
            var o = q[0] * s[0]
            o += q[1] * s[1]
            o += q[2] * s[2]
            o += q[3] * s[3]
            o += q[4] * s[4]
            o += q[5] * s[5]
            o += q[6] * s[6]
            o += q[7] * s[7]
            o += q[8] * s[8]
            o += q[9] * s[9]
            o += q[10] * s[10]
            o += q[11] * s[11]
            o += q[12] * s[12]
            o += q[13] * s[13]
            o += q[14] * s[14]
            o += q[15] * s[15]
            o = cluster_add(o, lanes_per_col)
            if (live && lane == 0u) {
                orows[pa.o_off + t * pa.di + h * ds + col] = o
            }
            t++
        }
        if (live) {
            state[sb] = s[0]
            state[sb + row_stride] = s[1]
            state[sb + 2u * row_stride] = s[2]
            state[sb + 3u * row_stride] = s[3]
            state[sb + 4u * row_stride] = s[4]
            state[sb + 5u * row_stride] = s[5]
            state[sb + 6u * row_stride] = s[6]
            state[sb + 7u * row_stride] = s[7]
            state[sb + 8u * row_stride] = s[8]
            state[sb + 9u * row_stride] = s[9]
            state[sb + 10u * row_stride] = s[10]
            state[sb + 11u * row_stride] = s[11]
            state[sb + 12u * row_stride] = s[12]
            state[sb + 13u * row_stride] = s[13]
            state[sb + 14u * row_stride] = s[14]
            state[sb + 15u * row_stride] = s[15]
        }
    }
}

struct DnScanP3Args {
    npos : uint
    nvh : uint
    di : uint
    ds : uint
    o_off : uint      // the raw o rows' base in orows
    wnorm_off : uint  // the out-norm weight row in smalls
    eps : float
}

//! the gated out-norm over the scan's raw o rows, in place: rms(o_h) * wnorm, gated by silu(z)
[vk_dispatch(name = "dn_scan_p3_cls", grid = "npos", params = "npos : int64")]
class DnScanP3 {
    @ssbo @binding = 0 zrows : array<float>    // the z gate rows
    @ssbo @binding = 1 orows : array<float>    // the work surface holding the raw o rows at o_off
    @ssbo @binding = 2 smalls : array<float>   // wnorm (push offset)
    @push_constant pa : DnScanP3Args
    @workgroup hinv : float[64]                // per-head inverse rms (heads <= 64)

    [spirv_kernel(local_size_x = 256, name = "dn_scan_p3_cls_spv")]
    def run {
        let p = gl_WorkGroupID.x
        let tid = gl_LocalInvocationID.x
        let ds = pa.ds
        let ob = pa.o_off + p * pa.di
        if (tid < pa.nvh) {
            var sum = 0.0
            var j = 0u
            while (j < ds) {
                let v = orows[ob + tid * ds + j]
                sum += v * v
                j++
            }
            hinv[tid] = 1.0 / sqrt(sum / float(ds) + pa.eps)
        }
        barrier()
        var e = tid
        while (e < pa.di) {
            let zv = zrows[p * pa.di + e]
            let xn = orows[ob + e] * hinv[e / ds] * smalls[pa.wnorm_off + e % ds]
            orows[ob + e] = xn * (zv / (1.0 + exp(-zv)))
            e += 256u
        }
    }
}

// ===== prefill attention: the prep pass =====

struct AtPrepArgs {
    d : uint         // this pass's row width (qd for q, kvd for k/v)
    nh : uint        // heads in this pass
    hs : uint
    half : uint      // partial-rope half (0 = no rope; the v pass runs here)
    flags : uint     // bit 0 = gated q, bit 1 = qk-norm
    rms_off : uint   // this pass's norm-weight row in smalls
    cos_off : uint   // per-position cos rows in smalls
    sin_off : uint
    obase : uint     // output panel's row base (0 for q, npos for k)
    qsrc : uint      // 1 = the q source (gating applies), 0 = k/v
    boff : uint      // this pass's row base in the bias buffer ([q | k | v] per-head biases)
    hasb : uint
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
    @ssbo @binding = 4 bias : array<float>     // per-layer [q | k | v] projection biases (hasb), else a placeholder
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
                row[i] = src[p * 2u * pa.d + h * 2u * hs + j] + (pa.hasb != 0u ? bias[pa.boff + i] : 0.0)
                gates[p * pa.d + i] = src[p * 2u * pa.d + h * 2u * hs + hs + j]
            } else {
                row[i] = src[p * pa.d + i] + (pa.hasb != 0u ? bias[pa.boff + i] : 0.0)
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
    def load_q(q0, nrows, qstride, h, hs, qhs : uint) {
        let tid = gl_LocalInvocationID.x
        var i = tid
        while (i < 8u * hs) {
            let qr = q0 + i / hs
            fq[i] = qr < nrows ? qpanel[qr * qstride + h * qhs + i % hs] : 0.0
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

    // tile QK^T: lane (r, c) scores query r against position k0+c, masked into fp - causal, and
    // below the query's window on a sliding layer (win 0 = every position)
    def qk_score(r, c, hs, k0, gq, q0, nrows, win : uint; scale, cap : float) {
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
        let inwin = win == 0u || gk + win > gq
        fp[r * 32u + c] = (gk <= gq && inwin && q0 + r < nrows) ? attn_softcap(s * scale, cap) : -3.0e38
    }

    //! the first K/V tile a window's queries can see: a sliding layer's tile below every query's window start skips whole
    def k_first(w0, q0, win : uint) : uint => (win != 0u && w0 + q0 > win) ? ((w0 + q0 - win) / 32u) * 32u : 0u

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
    // mirrors, K/V read at kbase/vbase; q read head-strided (qhs), the gated epilogue multiplies
    // by the sigmoid of the head's gate half; dpl-guarded accumulators serve hs 32..256.
    // pa/outp are instance-declared (DaAttnBArgs); AtAttn overrides run
    [spirv_kernel(local_size_x = 256)]
    def run {   // nolint:STYLE038 — the verbatim flash pass, parity-locked to the shipped kernel
        let qd = pa.nh * pa.hs
        let qhs = pa.qhs == 0u ? pa.hs : pa.qhs   // 0 = compact (non-gated); a gated model passes 2 * hs
        let qrow = pa.nh * qhs
        let dpl = pa.hs / 32u
        let qtiles = (pa.npos + 7u) / 8u
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * 8u
        let tid = gl_LocalInvocationID.x
        let r = tid / 32u
        let c = tid % 32u
        let kvh = h / pa.kv_mul
        load_q(q0, pa.npos, qrow, h, pa.hs, qhs)
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
        var k0 = k_first(pa.w0, q0, pa.window)
        while (k0 < kend) {
            load_k(k0, kend, pa.kbase, pa.kvd, kvh, pa.hs)
            barrier()
            qk_score(r, c, pa.hs, k0, gq, q0, pa.npos, pa.window, pa.scale, pa.cap)
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
            let gb = (q0 + r) * qrow + h * qhs + pa.hs + c
            var jj = 0u
            while (jj < dpl) {
                var v = (jj == 0u ? o0 : (jj == 1u ? o1 : (jj == 2u ? o2 : (jj == 3u ? o3
                    : (jj == 4u ? o4 : (jj == 5u ? o5 : (jj == 6u ? o6 : o7))))))) * inv
                if (pa.gated != 0u) {
                    let g = qpanel[gb + jj * 32u]
                    v = v / (1.0 + exp(-g))
                }
                outp[ob + jj * 32u] = v
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
        load_q(q0, pa.rows, pa.qd, h, pa.hs, pa.hs)
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
            qk_score(r, c, pa.hs, k0, gq, q0, pa.rows, 0u, pa.scale, 0.0)   //! the per-op tile serves no softcap model
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

//! the flash tiles' row-max bias, 3 ln 2: every P = e^(S - M) sits at an eighth or under, so a step's f16 P x V accumulator cannot overflow; L carries the same bias and the final divide cancels it
let FA_MAX_BIAS = 2.0794415

[spirv_per_element, unused_argument(r, c)]
def facm2_pick(r, c : uint; v, o : float) : float => max(v + FA_MAX_BIAS, o)

// causal: key kbase+c may not exceed query w0+q0+r (-FLT_MAX/2 keeps Mold-M NaN-free), nor sit
// below the query's window on a sliding layer (win 0 = every position); padding rows keep their
// causal window so no row is all -inf (rowsum stays positive)
[spirv_per_element]
def facm2_mask(r, c : uint; v : float; qbase, kbase, win : uint) : float => (kbase + c > qbase + r || (win != 0u && kbase + c + win <= qbase + r)) ? -1.7e38 : v

[spirv_per_element, unused_argument(r, c)]
def facm2_recip(r, c : uint; v : float) : float => v == 0.0 ? 0.0 : 1.0 / v

[spirv_per_element, unused_argument(r, c)]
def facm2_sigmoid(r, c : uint; v : float) : float => 1.0 / (1.0 + exp(-v))

struct FaArgs {
    rows : uint     // window query rows
    w0 : uint       // window base (queries attend positions [0, w0 + row])
    qd : uint       // o row stride (n_heads * hs); the q row's too unless qrow says otherwise
    kvd : uint      // k/v row stride (n_kv * hs)
    kv_mul : uint   // GQA ratio (head / kv_mul = kv head)
    kvbase : uint   // element base of this layer's plane in the K/V shadow mirrors
    scale : float
    qstride : uint  // the q panel's row stride when it is not qd (a gated q plane: [q | gate] per head); 0 = qd
    qhs : uint      // the q panel's per-head stride (2 x hs on a gated plane); 0 = hs
    window : uint   //! a sliding layer's queries attend the `window` positions up to their own; 0 = every position
    cap : float     //! the attention logit softcap on the scaled scores (gemma-2: 50), read by the CAP stamps; 0 = none
    sinkoff : uint  //! the SINK stamps: this layer's row base in the sink plane ([n_heads] a layer)
}

//! gemma-2's attention logit softcap on a scaled score tile
[spirv_per_element, unused_argument(r, c)]
def facm2_softcap(r, c : uint; v : float; cap : float) : float => softcap_exp(v, cap)

// The flash pass over the f16 K/V shadows: ONE class template over the head size and the cooperative-matrix arm (KHR). The head
// template sizes the eight coopmat typedefs of the cm2 arm: QACC/QA the q and o tiles (64 x HS), SACC/PA the
// score tiles (64 x BC), KB/VB the K^T and V steps, ONE the rowsum ones-matrix, OF the f16 store; the KHR leaves inherit them unread.
[ |> template_struct_instance]
class template FaT {
    @ssbo @binding = 0 qpanel : array<float>     // the prepped/roped window q rows (window-local)
    @ssbo @binding = 1 kh : array<float16>       // f16 K shadow at absolute positions
    @ssbo @binding = 2 vh : array<float16>       // f16 V shadow at absolute positions
    @ssbo @binding = 3 outp : array<OT>          // attention out plane (rows x qd)
    @ssbo @binding = 4 @role = "weight" @template_gate = SINK sinks : array<float>   //! the SINK stamps: the per-head sink logits, `pa.sinkoff` the layer's row
    @push_constant pa : FaArgs
    @template_constant HS : uint = 64u           // the head width
    @template_constant BC : uint = 64u           // the cm2 arm's K/V step: 64 at hs 64, 32 above (the head-size cut: K^T, V and the softmax state at 32 columns)
    @template_constant OUT16 : bool = false      // the o store as f16
    @template_constant GATED : bool = false      // the cm2 arm: multiply the sigmoid of the gate half of a [q | gate] head into o; qstride/qhs describe that panel and read as qd/HS when unset
    @template_constant CAP : bool = false        //! the softcap stamps: every scaled score through cap * tanh(s / cap) before the mask (gemma-2)
    @template_constant SINK : bool = false       //! the sink stamps (gpt-oss): the online softmax seeds its max with the head's sink logit and its sum with one - the phantom key with no value row
    @template_constant KHR : bool = false        //! the KHR cooperative-matrix arm; its lane map is the cited section's
    @workgroup @template_gate = KHR fq : uint[4096]     // the 16-row q tile, scaled, f16 pairs: HS / 2 words a row (sized for the 512 head on every stamp)
    @workgroup @template_gate = KHR fs : float[1024]    // the 16 x 64 score tile, row stride 64
    @workgroup @template_gate = KHR fp : uint[512]      // P as f16 pairs, 16 x 64: 32 words a row
    @workgroup @template_gate = KHR fpv : uint[4096]    // the step's P x V as f16 pairs, 16 x HS: HS / 2 words a row

    //! the KHR arm's running max seed: the head's sink logit on the sink stamps (the phantom key), the floor elsewhere
    def seed_m(h : uint) : float {
        static_if (SINK) {
            return sinks[pa.sinkoff + h]
        } else {
            return -3.0e38
        }
    }

    //! ... and its running sum seed: the phantom key's own mass, one, on the sink stamps
    def seed_l : float => SINK ? 1.0 : 0.0

    [spirv_kernel(local_size_x = 128), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
    def run {   // nolint:STYLE037,STYLE038 — the two arms of one flash pass, each the verbatim shipped kernel; a method call would emit a SPIR-V function the emitter never inlines
        static_if (KHR) {
            let qtiles = (pa.rows + 15u) / 16u
            let h = gl_WorkGroupID.x / qtiles
            let q0 = (gl_WorkGroupID.x % qtiles) * 16u
            let tid = gl_LocalInvocationID.x
            let sg = tid / 32u
            let kvh = h / pa.kv_mul
            let kvlen = pa.w0 + pa.rows
            let qstride = pa.qstride == 0u ? pa.qd : pa.qstride
            let qhs = pa.qhs == 0u ? HS : pa.qhs
            let hw = HS / 2u
            var i = tid
            while (i < 16u * hw) {
                let qr = q0 + i / hw
                let jp = (i % hw) * 2u
                if (qr < pa.rows) {
                    let src = qr * qstride + h * qhs + jp
                    fq[i] = packHalf2x16(float2(qpanel[src] * pa.scale, qpanel[src + 1u] * pa.scale))
                } else {
                    fq[i] = 0u
                }
                i += 128u
            }
            let srow = tid / 8u           // the softmax lanes: eight a row
            let key_col0 = (tid % 8u) * 8u
            let gq = pa.w0 + q0 + srow
            let o_dim0 = (tid % 8u) * (HS / 8u)
            var m = seed_m(h)
            var l = seed_l()
            var o : float[64]
            for [unroll] (z in range(int(HS / 8u))) {
                o[z] = 0.0
            }
            let qbase = pa.w0 + q0
            let kend = pa.w0 + min(q0 + 16u, pa.rows)
            var j = (pa.window != 0u && qbase > pa.window) ? ((qbase - pa.window) / 64u) * 64u : 0u
            barrier()
            while [dont_unroll] (j < kend) {
                let krow = j + sg * 16u
                if (krow < kvlen) {
                    var s : coopmatAcc_f32_16x16
                    var d = 0u
                    while (d < HS) {
                        var qa : coopmatA_f16_16x16
                        coopmatLoad(qa, fq, int(d / 2u), int(hw), 0)
                        var kb : coopmatB_f16_16x16
                        coopmatLoad(kb, kh, pa.kvbase + krow * pa.kvd + kvh * HS + d, int(pa.kvd), 1)
                        s = coopmatMulAdd(qa, kb, s)
                        d += 16u
                    }
                    coopmatStore(s, fs, int(sg * 16u), 64, 0)
                }
                barrier()
                let m_old = m
                var mloc = -3.0e38
                for [unroll] (c in range(8)) {
                    let idx = srow * 64u + key_col0 + uint(c)
                    let gk = j + key_col0 + uint(c)
                    let v = CAP ? softcap_exp(fs[idx], pa.cap) : fs[idx]
                    let sv = (gk > gq || gk >= kvlen || (pa.window != 0u && gk + pa.window <= gq)) ? -3.0e38 : v
                    fs[idx] = sv
                    mloc = max(mloc, sv)
                }
                mloc = max(mloc, subgroupShuffleXor(mloc, 1u))
                mloc = max(mloc, subgroupShuffleXor(mloc, 2u))
                mloc = max(mloc, subgroupShuffleXor(mloc, 4u))
                let mt = max(m_old, mloc + FA_MAX_BIAS)
                var lloc = 0.0
                for [unroll] (c2 in range(4)) {
                    let idx = srow * 64u + key_col0 + uint(c2) * 2u
                    let e0 = exp(fs[idx] - mt)
                    let e1 = exp(fs[idx + 1u] - mt)
                    fp[idx / 2u] = packHalf2x16(float2(e0, e1))
                    lloc += e0 + e1
                }
                lloc += subgroupShuffleXor(lloc, 1u)
                lloc += subgroupShuffleXor(lloc, 2u)
                lloc += subgroupShuffleXor(lloc, 4u)
                let alpha = exp(m_old - mt)
                l = l * alpha + lloc
                m = mt
                barrier()
                let sg_col0 = sg * (HS / 4u)
                var acc : coopmatAcc_f16_16x16[8]
                var kc = 0u
                while (kc < 64u) {
                    if (j + kc < kvlen) {
                        var pf : coopmatA_f16_16x16
                        coopmatLoad(pf, fp, int(kc / 2u), 32, 0)
                        for [unroll_full] (f in range(int(HS / 64u))) {
                            var vb : coopmatB_f16_16x16
                            coopmatLoad(vb, vh, pa.kvbase + (j + kc) * pa.kvd + kvh * HS + sg_col0 + uint(f) * 16u, int(pa.kvd), 0)
                            acc[f] = coopmatMulAdd(pf, vb, acc[f])
                        }
                    }
                    kc += 16u
                }
                for [unroll_full] (f2 in range(int(HS / 64u))) {
                    coopmatStore(acc[f2], fpv, int((sg_col0 + uint(f2) * 16u) / 2u), int(hw), 0)
                }
                barrier()
                for [unroll] (z2 in range(int(HS / 16u))) {
                    let pair = unpackHalf2x16(fpv[srow * hw + (o_dim0 + uint(z2) * 2u) / 2u])
                    o[z2 * 2] = o[z2 * 2] * alpha + pair.x
                    o[z2 * 2 + 1] = o[z2 * 2 + 1] * alpha + pair.y
                }
                barrier()
                j += 64u
            }
            if (q0 + srow < pa.rows) {
                let inv = 1.0 / l
                let ob = (q0 + srow) * pa.qd + h * HS + o_dim0
                for [unroll] (z3 in range(int(HS / 8u))) {
                    static_if (OUT16) {
                        outp[ob + uint(z3)] = float16(o[z3] * inv)
                    } else {
                        outp[ob + uint(z3)] = o[z3] * inv
                    }
                }
            }
        } else {
            let qtiles = (pa.rows + 63u) / 64u
            let h = gl_WorkGroupID.x / qtiles
            let qt = gl_WorkGroupID.x % qtiles
            let q0 = qt * 64u
            let kvh = h / pa.kv_mul
            let kvlen = pa.w0 + pa.rows
            let qstride = pa.qstride == 0u ? pa.qd : pa.qstride
            let qhs = pa.qhs == 0u ? HS : pa.qhs
            var tlq : tensorLayout2DPad
            tensorLayoutCreate(tlq)
            tensorLayoutSetDimension(tlq, pa.rows, qstride)
            tensorLayoutSetStride(tlq, qstride, 1u)
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
            var q : QACC
            coopmatLoadTensor(q, qpanel, 0u, tlq, q0, 64u, h * qhs, HS)
            coopmatScale(q, pa.scale)
            var qf : QA
            coopmatConvert(qf, q)
            var mrow : SACC
            var lrow : SACC
            static_if (SINK) {
                coopmatFill(mrow, sinks[pa.sinkoff + h])
                coopmatFill(lrow, 1.0)
            } else {
                coopmatFill(mrow, -1.7e38)
            }
            var o : OF
            let qbase = pa.w0 + q0
            let kmax = pa.w0 + min(q0 + 64u, pa.rows)   // causal bound (exclusive) of the tile's last row
            // a sliding layer's first step: the step below the tile's first query's window start
            var j = (pa.window != 0u && qbase > pa.window) ? ((qbase - pa.window) / BC) * BC : 0u
            while [dont_unroll] (j < kmax) {
                var s : SACC
                var kt : KB
                coopmatLoadTensor(kt, kh, pa.kvbase, tlk, j, BC, kvh * HS, HS, tv)
                s = coopmatMulAdd(qf, kt, s)
                static_if (CAP) {
                    coopmatPerElement(s, s, @@facm2_softcap, pa.cap)
                }
                if (j + BC > qbase + 1u || (pa.window != 0u && j + pa.window < qbase + 64u)) {
                    coopmatPerElement(s, s, @@facm2_mask, qbase, j, pa.window)
                }
                var rmax : SACC
                coopmatReduce(rmax, s, COOPMAT_REDUCE_ROW, @@facm2_max)
                var mold : SACC
                coopmatCopy(mold, mrow)
                coopmatPerElement(mrow, rmax, @@facm2_pick, mold)
                coopmatSub(s, s, mrow)
                coopmatPerElement(s, s, @@facm2_exp)                        // P = e^(S - M), in place
                var em : SACC
                coopmatSub(em, mold, mrow)
                coopmatPerElement(em, em, @@facm2_exp)                      // eM = e^(Mold - M)
                var pf : PA
                coopmatConvert(pf, s)
                var rsum : SACC
                var one : ONE
                coopmatFill(one, 1.0)
                rsum = coopmatMulAdd(pf, one, rsum)                         // rowsum by ones-matrix multiply
                coopmatMulElem(lrow, em, lrow)
                coopmatAdd(lrow, lrow, rsum)                                // L = eM*L + rowsum
                var emd : QACC
                coopmatReduce(emd, em, COOPMAT_REDUCE_ROW, @@facm2_smear)
                var emf : OF
                coopmatConvert(emf, emd)
                coopmatMulElem(o, emf, o)
                var vt : VB
                coopmatLoadTensor(vt, vh, pa.kvbase, tlk, j, BC, kvh * HS, HS)
                o = coopmatMulAdd(pf, vt, o)                                // O = eMdiag*O + P @ V
                j += BC
            }
            var o32 : QACC
            coopmatConvert(o32, o)
            var ld : QACC
            coopmatReduce(ld, lrow, COOPMAT_REDUCE_ROW, @@facm2_smear)
            coopmatPerElement(ld, ld, @@facm2_recip)
            coopmatMulElem(o32, ld, o32)                                    // O /= L (padding rows discard at store)
            static_if (GATED) {
                var g : QACC
                coopmatLoadTensor(g, qpanel, 0u, tlq, q0, 64u, h * qhs + HS, HS)
                coopmatPerElement(g, g, @@facm2_sigmoid)
                coopmatMulElem(o32, g, o32)
            }
            static_if (OUT16) {
                var of : OF
                coopmatConvert(of, o32)
                coopmatStoreTensor(of, outp, 0u, tlo, q0, 64u, h * HS, HS)
            } else {
                coopmatStoreTensor(o32, outp, 0u, tlo, q0, 64u, h * HS, HS)
            }
        }
    }
}

// hs=64 (tinyllama-class): the whole head is one K/V step on the cm2 arm
[ |> template_struct_instance]
class template FaH64T : FaT {
    override HS = 64u
    override BC = 64u
    typedef QACC = coopmatWgAcc_f32_64x64
    typedef QA = coopmatWgA_f16_64x64
    typedef SACC = coopmatWgAcc_f32_64x64
    typedef PA = coopmatWgA_f16_64x64
    typedef KB = coopmatWgB_f16_64x64
    typedef VB = coopmatWgB_f16_64x64
    typedef ONE = coopmatWgB_f16_64x64
    typedef OF = coopmatWgAcc_f16_64x64
}

[vk_dispatch(name = "fa_cm2_h64_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64 : FaH64T {
    typedef OT = float
}

[vk_dispatch(name = "fa_cm2_h64_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64F16 : FaH64T {
    typedef OT = float16
    override OUT16 = true
}

[vk_dispatch(name = "fa_cm2_h64_sink_cls", kernel = "run", family = "fa_cm2_sink_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64Sink : FaH64T {
    typedef OT = float
    override SINK = true
}

[vk_dispatch(name = "fa_cm2_h64_sink_f16_cls", kernel = "run", family = "fa_cm2_sink_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64SinkF16 : FaH64T {
    typedef OT = float16
    override OUT16 = true
    override SINK = true
}

// hs=128 (llama-3.2-3B-class) at Bc=32. Fast standalone (24.7 TFLOP/s, right beside the h64) and
// WINS end-to-end when the shadows fit (3B 4287 vs 3955 no-fa) — serving is gated only on the
// resident plan learning the f16 shadow cost (see the pf_facm2 gate)
[ |> template_struct_instance]
class template FaH128T : FaT {
    override HS = 128u
    override BC = 32u
    typedef QACC = coopmatWgAcc_f32_64x128
    typedef QA = coopmatWgA_f16_64x128
    typedef SACC = coopmatWgAcc_f32_64x32
    typedef PA = coopmatWgA_f16_64x32
    typedef KB = coopmatWgB_f16_128x32
    typedef VB = coopmatWgB_f16_32x128
    typedef ONE = coopmatWgB_f16_32x32
    typedef OF = coopmatWgAcc_f16_64x128
}

[vk_dispatch(name = "fa_cm2_h128_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H128 : FaH128T {
    typedef OT = float
}

[vk_dispatch(name = "fa_cm2_h128_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H128F16 : FaH128T {
    typedef OT = float16
    override OUT16 = true
}

// hs=256 (the deltanet hybrids' attention heads) at Bc=32; the gated leaves ride the [q | gate] panel
[ |> template_struct_instance]
class template FaH256T : FaT {
    override HS = 256u
    override BC = 32u
    typedef QACC = coopmatWgAcc_f32_64x256
    typedef QA = coopmatWgA_f16_64x256
    typedef SACC = coopmatWgAcc_f32_64x32
    typedef PA = coopmatWgA_f16_64x32
    typedef KB = coopmatWgB_f16_256x32
    typedef VB = coopmatWgB_f16_32x256
    typedef ONE = coopmatWgB_f16_32x32
    typedef OF = coopmatWgAcc_f16_64x256
}

[vk_dispatch(name = "fa_cm2_h256_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256 : FaH256T {
    typedef OT = float
}

[vk_dispatch(name = "fa_cm2_h256_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256F16 : FaH256T {
    typedef OT = float16
    override OUT16 = true
}

[vk_dispatch(name = "fa_cm2_h256_g_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256G : FaH256T {
    typedef OT = float
    override GATED = true
}

[vk_dispatch(name = "fa_cm2_h256_g_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256GF16 : FaH256T {
    typedef OT = float16
    override OUT16 = true
    override GATED = true
}

//! the softcap leaves (gemma-2's 256-wide heads): the h256 pass with every scaled score capped before the mask
[vk_dispatch(name = "fa_cm2_h256_cap_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256Cap : FaH256T {
    typedef OT = float
    override CAP = true
}

[vk_dispatch(name = "fa_cm2_h256_cap_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256CapF16 : FaH256T {
    typedef OT = float16
    override OUT16 = true
    override CAP = true
}

// hs=512 (gemma-4's global heads, V from K): the h256 pass at twice the head width, Bc=32
[ |> template_struct_instance]
class template FaH512T : FaT {
    override HS = 512u
    override BC = 32u
    typedef QACC = coopmatWgAcc_f32_64x512
    typedef QA = coopmatWgA_f16_64x512
    typedef SACC = coopmatWgAcc_f32_64x32
    typedef PA = coopmatWgA_f16_64x32
    typedef KB = coopmatWgB_f16_512x32
    typedef VB = coopmatWgB_f16_32x512
    typedef ONE = coopmatWgB_f16_32x32
    typedef OF = coopmatWgAcc_f16_64x512
}

[vk_dispatch(name = "fa_cm2_h512_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H512 : FaH512T {
    typedef OT = float
}

[vk_dispatch(name = "fa_cm2_h512_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H512F16 : FaH512T {
    typedef OT = float16
    override OUT16 = true
}

[vk_dispatch(name = "fa_khr_h64_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64 : FaH64T {
    override KHR = true
    typedef OT = float
}

[vk_dispatch(name = "fa_khr_h64_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64F16 : FaH64T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true
}

[vk_dispatch(name = "fa_khr_h64_sink_cls", kernel = "run", family = "fa_khr_sink_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64Sink : FaH64T {
    override KHR = true
    typedef OT = float
    override SINK = true
}

[vk_dispatch(name = "fa_khr_h64_sink_f16_cls", kernel = "run", family = "fa_khr_sink_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64SinkF16 : FaH64T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true
    override SINK = true
}

[vk_dispatch(name = "fa_khr_h128_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH128 : FaH128T {
    override KHR = true
    typedef OT = float
}

[vk_dispatch(name = "fa_khr_h128_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH128F16 : FaH128T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true
}

[vk_dispatch(name = "fa_khr_h256_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH256 : FaH256T {
    override KHR = true
    typedef OT = float
}

[vk_dispatch(name = "fa_khr_h256_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH256F16 : FaH256T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true
}

[vk_dispatch(name = "fa_khr_h256_cap_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH256Cap : FaH256T {
    override KHR = true
    typedef OT = float
    override CAP = true
}

[vk_dispatch(name = "fa_khr_h256_cap_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH256CapF16 : FaH256T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true
    override CAP = true
}

[vk_dispatch(name = "fa_khr_h512_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH512 : FaH512T {
    override KHR = true
    typedef OT = float
}

[vk_dispatch(name = "fa_khr_h512_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH512F16 : FaH512T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def fa_stamp_wgs(khr : bool; nheads, rows : int64) : int64 {
    let qrows_per_wg = khr ? 16l : 64l
    return nheads * ((rows + qrows_per_wg - 1l) / qrows_per_wg)
}

//! the refusals are `panic`s, not `verify`s: a `verify` prints a stack walk before it throws, and the refusal cell probes them under `recover`
def private fa_stamp_refuse(khr : bool; hs : int64; gated, cap, sink : bool) {
    if (khr && gated) {
        panic("vk flash: the KHR flash tile has no gated stamp")
    }
    if (cap && hs != 256l) {
        panic("vk flash: the softcap stamps serve head size 256 alone")
    }
    if (gated && hs != 256l) {
        panic("vk flash: the gated stamps serve head size 256 alone")
    }
    if (sink && hs != 64l) {   //! the gated and capped stamps sit at 256 alone (above), so this one test also keeps the sinks off them
        panic("vk flash: the sink stamps serve head size 64 alone")
    }
    if (hs != 64l && hs != 128l && hs != 256l && hs != 512l) {
        panic("vk flash: a stamp for a head size the tile never serves")
    }
}

def private fa_sink_ensure(khr, f16 : bool) : bool
    => khr ? (f16 ? ensure_fa_khr_h64_sink_f16_cls() : ensure_fa_khr_h64_sink_cls()) : (f16 ? ensure_fa_cm2_h64_sink_f16_cls() : ensure_fa_cm2_h64_sink_cls())

def fa_stamp_ensure(khr : bool; hs : int64; gated, cap, sink, f16 : bool) : bool {   // nolint:STYLE037 — one pick over every flash stamp; a split per arm is the twin ladder this replaces
    fa_stamp_refuse(khr, hs, gated, cap, sink)
    if (sink) return fa_sink_ensure(khr, f16)
    if (hs == 64l) return khr ? (f16 ? ensure_fa_khr_h64_f16_cls() : ensure_fa_khr_h64_cls()) : (f16 ? ensure_fa_cm2_h64_f16_cls() : ensure_fa_cm2_h64_cls())
    if (hs == 128l) return khr ? (f16 ? ensure_fa_khr_h128_f16_cls() : ensure_fa_khr_h128_cls()) : (f16 ? ensure_fa_cm2_h128_f16_cls() : ensure_fa_cm2_h128_cls())
    if (hs == 512l) return khr ? (f16 ? ensure_fa_khr_h512_f16_cls() : ensure_fa_khr_h512_cls()) : (f16 ? ensure_fa_cm2_h512_f16_cls() : ensure_fa_cm2_h512_cls())
    if (gated) return f16 ? ensure_fa_cm2_h256_g_f16_cls() : ensure_fa_cm2_h256_g_cls()
    if (cap) return khr ? (f16 ? ensure_fa_khr_h256_cap_f16_cls() : ensure_fa_khr_h256_cap_cls()) : (f16 ? ensure_fa_cm2_h256_cap_f16_cls() : ensure_fa_cm2_h256_cap_cls())
    return khr ? (f16 ? ensure_fa_khr_h256_f16_cls() : ensure_fa_khr_h256_cls()) : (f16 ? ensure_fa_cm2_h256_f16_cls() : ensure_fa_cm2_h256_cls())
}

def fa_stamp_pair_ensure(khr : bool; hs : int64; gated, cap, sink, with_f16 : bool) : bool
    => fa_stamp_ensure(khr, hs, gated, cap, sink, false) && (!with_f16 || fa_stamp_ensure(khr, hs, gated, cap, sink, true))

def fa_stamp_set(khr : bool; bufs : uint64 const[4]; sizes : int64 const[4]; gbits : uint const[4]) : VkDescriptorSet
    => khr ? set_fa_khr_cls(bufs, sizes, gbits) : set_fa_cm2_cls(bufs, sizes, gbits)

//! the sink stamps' set: the four planes and the sink plane
def fa_stamp_sink_set(khr : bool; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet
    => khr ? set_fa_khr_sink_cls(bufs, sizes, gbits) : set_fa_cm2_sink_cls(bufs, sizes, gbits)

def private fa_sink_enc(khr, f16 : bool; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : FaArgs; wgs : int64) {
    if (khr) {
        if (f16) { enc_fa_khr_h64_sink_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_khr_h64_sink_cls(raw, h, s, pc, wgs) }
    } else {
        if (f16) { enc_fa_cm2_h64_sink_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_cm2_h64_sink_cls(raw, h, s, pc, wgs) }
    }
}

def fa_stamp_enc(khr : bool; hs : int64; gated, cap, sink, f16 : bool; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : FaArgs; wgs : int64) {   // nolint:STYLE037 — one pick over every flash stamp; a split per arm is the twin ladder this replaces
    fa_stamp_refuse(khr, hs, gated, cap, sink)
    if (sink) {
        fa_sink_enc(khr, f16, raw, h, s, pc, wgs)
    } elif (hs == 64l) {
        if (khr) {
            if (f16) { enc_fa_khr_h64_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_khr_h64_cls(raw, h, s, pc, wgs) }
        } else {
            if (f16) { enc_fa_cm2_h64_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_cm2_h64_cls(raw, h, s, pc, wgs) }
        }
    } elif (hs == 128l) {
        if (khr) {
            if (f16) { enc_fa_khr_h128_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_khr_h128_cls(raw, h, s, pc, wgs) }
        } else {
            if (f16) { enc_fa_cm2_h128_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_cm2_h128_cls(raw, h, s, pc, wgs) }
        }
    } elif (hs == 512l) {
        if (khr) {
            if (f16) { enc_fa_khr_h512_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_khr_h512_cls(raw, h, s, pc, wgs) }
        } else {
            if (f16) { enc_fa_cm2_h512_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_cm2_h512_cls(raw, h, s, pc, wgs) }
        }
    } else {
        if (gated) {
            if (f16) { enc_fa_cm2_h256_g_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_cm2_h256_g_cls(raw, h, s, pc, wgs) }
        } elif (cap) {
            if (khr) {
                if (f16) { enc_fa_khr_h256_cap_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_khr_h256_cap_cls(raw, h, s, pc, wgs) }
            } else {
                if (f16) { enc_fa_cm2_h256_cap_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_cm2_h256_cap_cls(raw, h, s, pc, wgs) }
            }
        } elif (khr) {
            if (f16) { enc_fa_khr_h256_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_khr_h256_cls(raw, h, s, pc, wgs) }
        } else {
            if (f16) { enc_fa_cm2_h256_f16_cls(raw, h, s, pc, wgs) } else { enc_fa_cm2_h256_cls(raw, h, s, pc, wgs) }
        }
    }
}

//! gemma-2's attention logit softcap on a scaled score; 0 = none
def private attn_softcap(s, cap : float) : float => cap != 0.0 ? softcap_exp(s, cap) : s

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
    qhs : uint      // per-head q stride: hs, or 2 * hs on a gated model ([q | gate] per head)
    gated : uint    // multiply each head's output by the sigmoid of its gate half
    window : uint   //! a sliding layer's queries attend the `window` positions up to their own; 0 = every position
    cap : float     //! the attention logit softcap on the scaled scores (gemma-2: 50); 0 = none
}

// the wide-head twin of DaAttnB for heads past 256 (gemma-4's global layers, 512): a 4-row q tile and
// 16-position K/V stages in the same shared memory, 64 lanes scoring, every thread accumulating eight
// output dims of one row; the window and causal masks as the 8-row kernel's
[ |> template_struct_instance]
class template DaAttnBWT {
    @ssbo @binding = 0 qpanel : array<float>   // the prepped/roped q rows
    @ssbo @binding = 1 kpanel : array<KT>      // the K mirror plane (kbase offsets)
    @ssbo @binding = 2 vpanel : array<KT>      // the V mirror plane (vbase offsets)
    @ssbo @binding = 3 outp : array<float>     // attention out rows
    @push_constant pa : DaAttnBArgs
    @workgroup fq : float[2048]    // the tile's 4 q rows (hs <= 512)
    @workgroup fkv : float[8208]   // one 16-row K/V stage, stride 513 (bank-spread)
    @workgroup fp : float[64]      // the 4x16 score/probability tile
    @workgroup fm : float[4]       // per-row online-softmax state
    @workgroup fl : float[4]
    @workgroup fal : float[4]

    [spirv_kernel(local_size_x = 256)]
    def run {   // nolint:STYLE037,STYLE038 — the whole flash pass over one q tile, the shape its 8-row sibling takes
        let hs = pa.hs
        let qd = pa.nh * hs
        let qtiles = (pa.npos + 3u) / 4u
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * 4u
        let tid = gl_LocalInvocationID.x
        let kvh = h / pa.kv_mul
        var i = tid
        while (i < 4u * hs) {
            let qr = q0 + i / hs
            fq[i] = qr < pa.npos ? qpanel[qr * qd + h * hs + i % hs] : 0.0
            i += 256u
        }
        if (tid < 4u) {
            fm[tid] = -3.0e38
            fl[tid] = 0.0
        }
        var o : float[8]   // row vr's dims vc + 64 * z
        for (z in range(8)) {
            o[z] = 0.0
        }
        let vr = tid / 64u
        let vc = tid % 64u
        barrier()
        let kend = pa.w0 + min(q0 + 4u, pa.npos)   // causal bound (exclusive) of the tile's last row
        var k0 = (pa.window != 0u && pa.w0 + q0 > pa.window) ? ((pa.w0 + q0 - pa.window) / 16u) * 16u : 0u
        while (k0 < kend) {
            i = tid
            while (i < 16u * hs) {
                let kr = k0 + i / hs
                fkv[(i / hs) * 513u + i % hs] = kr < kend ? float(kpanel[pa.kbase + kr * pa.kvd + kvh * hs + i % hs]) : 0.0   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
                i += 256u
            }
            barrier()
            if (tid < 64u) {   // lane (r, c) scores query r against position k0 + c
                let r = tid / 16u
                let c = tid % 16u
                var s = 0.0
                var j = 0u
                while (j < hs) {   // 4-wide manual unroll (hs % 32 == 0)
                    s += fq[r * hs + j] * fkv[c * 513u + j]
                    s += fq[r * hs + j + 1u] * fkv[c * 513u + j + 1u]
                    s += fq[r * hs + j + 2u] * fkv[c * 513u + j + 2u]
                    s += fq[r * hs + j + 3u] * fkv[c * 513u + j + 3u]
                    j += 4u
                }
                let gq = pa.w0 + q0 + r
                let gk = k0 + c
                let inwin = pa.window == 0u || gk + pa.window > gq
                fp[r * 16u + c] = (gk <= gq && inwin && q0 + r < pa.npos) ? attn_softcap(s * pa.scale, pa.cap) : -3.0e38
            }
            barrier()
            if (tid < 4u) {   // per-row online softmax: max, rescale, exponentiate in place
                var mt = fm[tid]
                var j2 = 0u
                while (j2 < 16u) {
                    mt = max(mt, fp[tid * 16u + j2])
                    j2++
                }
                let alpha = exp(fm[tid] - mt)
                var lsum = 0.0
                j2 = 0u
                while (j2 < 16u) {
                    let e = exp(fp[tid * 16u + j2] - mt)
                    fp[tid * 16u + j2] = e
                    lsum += e
                    j2++
                }
                fl[tid] = fl[tid] * alpha + lsum
                fm[tid] = mt
                fal[tid] = alpha
            }
            barrier()
            let av = fal[vr]
            for (z in range(8)) {
                o[z] *= av
            }
            i = tid
            while (i < 16u * hs) {   // the V stage reuses the K staging
                let kr = k0 + i / hs
                fkv[(i / hs) * 513u + i % hs] = kr < kend ? float(vpanel[pa.vbase + kr * pa.kvd + kvh * hs + i % hs]) : 0.0   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
                i += 256u
            }
            barrier()
            var c2 = 0u
            while (c2 < 16u) {
                let pw = fp[vr * 16u + c2]
                let vb = c2 * 513u + vc
                for [unroll] (z in range(8)) {
                    o[z] += pw * fkv[vb + 64u * uint(z)]
                }
                c2++
            }
            barrier()
            k0 += 16u
        }
        if (q0 + vr < pa.npos) {
            let inv = 1.0 / fl[vr]
            let ob = (q0 + vr) * qd + h * hs + vc
            for (z in range(8)) {
                if (vc + 64u * uint(z) < hs) {
                    outp[ob + 64u * uint(z)] = o[z] * inv
                }
            }
        }
    }
}

[vk_dispatch(name = "da_attn_bw_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBW : DaAttnBWT {
    typedef KT = float
}

// f16-mirror twin — convert-on-load, staging/compute stay f32
[vk_dispatch(name = "da_attn_bw_f16_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBWF16 : DaAttnBWT {
    typedef KT = float16
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
    qstride : uint  // q head stride: 0 = hs; 2*hs on a gated-q model ([q | gate] per head)
    gated : uint    // 1 = multiply the head's output by sigmoid of its gate half before the store
    window : uint   //! a sliding layer attends the last `window` positions of the cnt; 0 = every position
    cap : float     //! the attention logit softcap on the scaled scores (gemma-2: 50); 0 = none
    nsplit : uint   //! the key range split across this many workgroups a group, each leaving a (max, denom, acc) piece a head that the group's last piece combines; 0 or 1 = one workgroup a group stores the rows itself
    rqk : uint      //! the row's requant folded into the store: 0 = Q8_0 blocks of 32, 1 = Q8_K superblocks of 256 (a head a multiple of 256), 2 = none (the row alone)
    sinkoff : uint  //! the SINK stamps: this layer's row base in the sink plane ([n_heads] a layer)
    rowwg : uint    //! (kv head, slab, split) workgroups a row - `da_attn_row_wgs`: the dispatch past them is the next row, which reads its own `tok` entry; 0 = the whole dispatch is row 0 (the one-row command passes its row's count: the same reading)
    qrow : uint     //! a row's stride in q (the projection row's width)
    cntoff : uint   //! the split form's arrival counters, a word a (row, kv head, slab) group, at this word offset of the partials binding: zero between dispatches
}

//! a (head, split) partial's floats in the partials plane: the running max, the denom, then the hs accumulators
def da_attn_part_stride(hs : uint) : uint => hs + 2u

let DA_G = 4u   //! the q heads a decode-attention workgroup serves off one K/V read: a GQA group's, in slabs of four

[arch(at="../ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-decode-attn-split")]
def da_slab_is_g2(kv_mul : int64) : bool => kv_mul <= 2l

//! the slab's q heads at a group width: the kernel's `G` and the host's workgroup count derive from this one expression
def da_slab_heads(kv_mul : int64) : int64 => da_slab_is_g2(kv_mul) ? 2l : int64(DA_G)

//! the decode attention's workgroups a row: one a (kv head, slab of the group's q heads, key split)
def da_attn_row_wgs(n_heads, kv_mul, nsplit : int64) : int64 {
    let g = da_slab_heads(kv_mul)
    return n_heads / kv_mul * ((kv_mul + g - 1l) / g) * nsplit
}

let DA_RQ_Q8 = 0u    //! `DaAttnArgs.rqk`: the wo plane reads Q8_0 activations
let DA_RQ_Q8K = 1u   //! ... Q8_K activations
let DA_RQ_NONE = 2u  //! ... no fold: a separate requant serves (or nothing reads the quants)

//! the Q8_0 block scale of a head row's dim: the 32-lane group's amax over 127 (the group is the block on a 32- or 64-wide subgroup)
def da_q8_blk_d(o : float) : float => q8k_amax5(abs(o)) / 127.0

[ |> template_struct_instance]
class template DaAttnT {
    @template_constant KV16 : bool = false   //!< the f16-mirror stamp: a lane's eight halves of a K row load as one 16-byte word
    @template_constant SINK : bool = false   //! the sink stamps (gpt-oss): the head's sink logit joins the softmax's max and denominator at the unsplit store; a split piece leaves it to the combine
    @template_constant G : int = 4           //!< the slab's q heads, DA_G or two (`da_slab_heads`): every score and V loop unrolls over them; the workgroup arrays hold four, so no stamp goes wider
    @ssbo @binding = 0 q : array<float>      // roped query rows, one per head
    @ssbo @binding = 1 kmir : array<KT>      // the K mirror plane
    @ssbo @binding = 1 @role = "alias" @template_gate = KV16 kw4 : array<uint4>   //! the K mirror as 16-byte words
    @ssbo @binding = 2 vmir : array<KT>      // the V mirror plane
    @ssbo @binding = 2 @role = "alias" @template_gate = KV16 vw4 : array<uint4>   //! the V mirror as 16-byte words
    @ssbo @binding = 3 y : array<float>      // attention out rows
    @ssbo @binding = 4 tok : array<TokMeta>  // the shared per-token block (element 0)
    @ssbo @binding = 5 @coherent part : array<float>   //! the split form's partials [head x split x (hs + 2)], the arrival counters past them; unsplit, any buffer (unread); coherent: the last-arriving piece reads the others' stores
    @ssbo @binding = 5 @role = "alias" @coherent partu : array<uint>   //! the partials binding as words: the arrival counters
    @ssbo @binding = 6 aq : array<int8>      //! the row's quants when `rqk` folds the requant (the wo plane's feed); else any buffer (unwritten)
    @ssbo @binding = 7 aqs : array<float>    //! ... and its block scales
    @ssbo @binding = 8 @role = "weight" @template_gate = SINK sinks : array<float>   //! the SINK stamps: the per-head sink logits, `pa.sinkoff` the layer's row
    @push_constant pa : DaAttnArgs
    @workgroup qrow : float[2048]  //! the slab's query rows, DA_G x 512 (hs <= 512); a dead head's row is zero
    @workgroup sc : float[1024]    //! the slab's chunk scores, DA_G x 256
    @workgroup sgp : float[128]    //! per-subgroup partials, DA_G x 32: a chunk's maxes in a head's lower half, its sums in the upper
    @workgroup ml : float[16]      //! a head's [running max, running softmax denom, -, -], DA_G quads
    @workgroup arrived : uint[1]   //! the split form: the pieces of this group that finished before this one

    //! one V mirror element as f32
    def vld(i : uint) : float => float(vmir[i])   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp

    //! the unsplit store's normalization: 1 / L, or on the sink stamps the phantom key folded into the max and the denominator once
    def unsplit_linv(h : uint; mrun, l : float) : float {
        static_if (SINK) {
            let sk = sinks[pa.sinkoff + h]
            let msk = max(mrun, sk)
            let os = exp(mrun - msk)
            return os / (l * os + exp(sk - msk))
        } else {
            return 1.0 / l
        }
    }

    //! eight halves of a K row word against the q row's slice at `qo`
    def kdot8(w : uint4; qo : uint) : float {
        let a = unpackHalf2x16(w.x)
        let b = unpackHalf2x16(w.y)
        let c = unpackHalf2x16(w.z)
        let d = unpackHalf2x16(w.w)
        return ((qrow[qo] * a.x + qrow[qo + 1u] * a.y) + (qrow[qo + 2u] * b.x + qrow[qo + 3u] * b.y)) + ((qrow[qo + 4u] * c.x + qrow[qo + 5u] * c.y) + (qrow[qo + 6u] * d.x + qrow[qo + 7u] * d.y))
    }

    //! the finished row (o at dim `d` = tid, o2 at tid + 256 on a wide head) stored, with the push's requant fold
    def store_row(h, hs, d : uint; o, o2 : float; wide : bool) {
        let live = d < hs
        if (live) {
            y[h * hs + d] = o
        }
        if (wide) {
            y[h * hs + d + 256u] = o2
        }
        if (pa.rqk == DA_RQ_Q8) {
            if (live) {   // a 32-aligned head keeps every lane of a block live together
                let dq = da_q8_blk_d(o)
                if ((d & 31u) == 0u) {
                    aqs[(h * hs + d) >> 5u] = dq
                }
                aq[h * hs + d] = q8_quant1(o, dq != 0.0 ? 1.0 / dq : 0.0)
            }
            if (wide) {
                let dq2 = da_q8_blk_d(o2)
                if ((d & 31u) == 0u) {
                    aqs[(h * hs + d + 256u) >> 5u] = dq2
                }
                aq[h * hs + d + 256u] = q8_quant1(o2, dq2 != 0.0 ? 1.0 / dq2 : 0.0)
            }
        } elif (pa.rqk == DA_RQ_Q8K) {   // every thread of the workgroup takes part: the head is 256 or 512 wide
            let m1 = subgroupMax(abs(o))
            let m2 = subgroupMax(wide ? abs(o2) : 0.0)
            if (gl_SubgroupInvocationID == 0u) {
                sgp[gl_SubgroupID] = m1
                sgp[32u + gl_SubgroupID] = m2
            }
            barrier()
            var mx1 = 0.0
            var mx2 = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                mx1 = max(mx1, sgp[sgi])
                mx2 = max(mx2, sgp[32u + sgi])
                sgi++
            }
            let d1 = mx1 / 127.0
            let id1 = d1 != 0.0 ? 1.0 / d1 : 0.0
            let b1 = (h * hs) >> 8u
            if (d == 0u) {
                aqs[b1] = d1
            }
            aq[h * hs + d] = q8_quant1(o, id1)
            if (wide) {
                let d2 = mx2 / 127.0
                let id2 = d2 != 0.0 ? 1.0 / d2 : 0.0
                if (d == 0u) {
                    aqs[b1 + 1u] = d2
                }
                aq[h * hs + d + 256u] = q8_quant1(o2, id2)
            }
            barrier()   //! the slab's next head reuses sgp
        }
    }

    //! the aligned max's seed: the head's sink logit on the sink stamp (the phantom key), the floor elsewhere
    def seed_m(h : uint) : float {
        static_if (SINK) {
            return sinks[pa.sinkoff + h]
        } else {
            return -3.0e38
        }
    }

    //! ... and the aligned denominator's: the phantom key's mass against the aligned max, once for the head
    def seed_l(h : uint; m : float) : float {
        static_if (SINK) {
            return exp(sinks[pa.sinkoff + h] - m)
        } else {
            return 0.0
        }
    }

    //! the split form's end, run by the group's last piece: a head's pieces aligned by their maxes, normalized, then the row
    def combine_head(h, hflat, hs, nsplit, stride, qb, qstride, tid : uint; wide : bool) {
        let hb = hflat * nsplit * stride
        var m = seed_m(h)
        var sp = 0u
        while (sp < nsplit) {
            m = max(m, part[hb + sp * stride])
            sp++
        }
        var l = seed_l(h, m)
        var o = 0.0
        var o2 = 0.0
        sp = 0u
        while (sp < nsplit) {
            let pb = hb + sp * stride
            let wgt = exp(part[pb] - m)
            l += part[pb + 1u] * wgt
            if (tid < hs) {
                o += part[pb + 2u + tid] * wgt
            }
            if (wide) {
                o2 += part[pb + 2u + tid + 256u] * wgt
            }
            sp++
        }
        let linv = 1.0 / l
        finish_head(h, hflat, hs, qb, qstride, tid, o * linv, o2 * linv, wide)
    }

    //! one head's finished value (`o` at dim tid, `o2` at tid + 256 on a wide head) gated and stored, the unsplit form's end
    def finish_head(h, hflat, hs, qb, qstride, tid : uint; o, o2 : float; wide : bool) {
        var og = o
        var og2 = o2
        if (pa.gated != 0u && (wide || tid < hs)) {
            og = og / (1.0 + exp(-q[qb + h * qstride + hs + tid]))
            if (wide) {
                og2 = og2 / (1.0 + exp(-q[qb + h * qstride + hs + tid + 256u]))
            }
        }
        store_row(hflat, hs, tid, og, og2, wide)
    }

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-decode-attn-split")]
    def run {   // nolint:STYLE037,STYLE038 — the online-softmax pass over a slab of heads, one shape for every head width and form
        let hs = pa.hs
        let nsplit = max(pa.nsplit, 1u)
        let rowwg = max(pa.rowwg, 1u)
        let row = pa.rowwg == 0u ? 0u : gl_WorkGroupID.x / rowwg
        let wg = pa.rowwg == 0u ? gl_WorkGroupID.x : gl_WorkGroupID.x % rowwg
        let cnt = tok[row].cnt
        let sp = wg % nsplit
        let kvslab = wg / nsplit
        let nslab = (pa.kv_mul + uint(G) - 1u) / uint(G)
        let kvh = kvslab / nslab
        let h0 = kvh * pa.kv_mul + (kvslab % nslab) * uint(G)   //! the slab's first q head
        let live = min(uint(G), pa.kv_mul - (kvslab % nslab) * uint(G))   //! its heads: a group's last slab may be short
        let hflat0 = row * (pa.kvd / hs * pa.kv_mul) + h0   //! the slab's first head in the attention-out and partial planes, whose row stride is the heads' width
        let qb = row * pa.qrow
        let kbase = pa.kbase + tok[row].mirbase
        let vbase = pa.vbase + tok[row].mirbase
        let tid = gl_LocalInvocationID.x
        let wide = hs > 256u
        let qstride = pa.qstride == 0u ? hs : pa.qstride
        let lane = gl_SubgroupInvocationID
        var i = tid
        while (i < hs) {
            for [unroll] (j in range(G)) {
                let hj = uint(j)
                qrow[hj * 512u + i] = hj < live ? q[qb + (h0 + hj) * qstride + i] : 0.0
            }
            i += 256u
        }
        if (tid < uint(G)) {
            ml[tid * 4u] = -3.0e38
            ml[tid * 4u + 1u] = 0.0
        }
        barrier()
        let vrow_words = hs / 8u
        let kgroups = 256u / vrow_words
        let vw_pow2 = (vrow_words & (vrow_words - 1u)) == 0u
        let kgroups_per_subgroup = vw_pow2 ? gl_SubgroupSize / vrow_words : 0u   //! whole key groups a subgroup: the fold pairs lanes of one dim slice
        let folded = kgroups_per_subgroup >= 1u
        let my_kgroup = folded ? gl_SubgroupID * kgroups_per_subgroup + lane / vrow_words : tid / vrow_words   //! off the subgroup's own lanes where the fold serves
        let w = folded ? lane % vrow_words : tid % vrow_words
        var acc : float[32]                    // [head * 8 + dim]
        for [unroll] (j in range(G * 8)) {
            acc[j] = 0.0
        }
        let cstart = pa.window != 0u && cnt > pa.window ? cnt - pa.window : 0u
        let per = ((cnt - cstart + nsplit - 1u) / nsplit + 31u) & ~31u
        var c0 = min(cnt, cstart + sp * per)
        let cend = min(cnt, cstart + (sp + 1u) * per)
        let vec = KV16 && (kbase & 7u) == 0u && (vbase & 7u) == 0u && (pa.kvd & 7u) == 0u   //! the f16 stamp with every mirror row on a 16-byte word: rows load as words
        let vb = vbase + kvh * hs + w * 8u
        while (c0 < cend) {
            let clen = min(256u, cend - c0)
            let krow = kbase + (c0 + min(tid, clen - 1u)) * pa.kvd + kvh * hs
            var kw : uint4[8]   // nolint:LINT002,LINT019 -- the f32 stamp's static_if leaves the word batches unused; the rule fires in that stamp's compile alone
            var vw : uint4[8]   // nolint:LINT002,LINT019 -- the f32 stamp's static_if leaves the word batches unused; the rule fires in that stamp's compile alone
            static_if (KV16) {
                if (vec) {
                    for [unroll] (b in range(8)) {
                        kw[b] = kw4[krow / 8u + min(uint(b), vrow_words - 1u)]   //! a head of 64 fills eight words, a wider one takes more batches
                        vw[b] = vw4[(vb + (c0 + min(my_kgroup + uint(b) * kgroups, clen - 1u)) * pa.kvd) / 8u]
                    }
                }
            }
            if (tid < clen) {
                var pv : float[4]
                for [unroll] (j in range(G)) {
                    pv[j] = 0.0
                }
                static_if (KV16) {
                    if (vec) {
                        var wi = 0u
                        while (wi < vrow_words) {
                            if (wi != 0u) {
                                for [unroll] (b in range(8)) {
                                    kw[b] = kw4[krow / 8u + min(wi + uint(b), vrow_words - 1u)]
                                }
                            }
                            for [unroll] (b in range(8)) {
                                let wb = wi + uint(b)
                                if (wb < vrow_words) {
                                    for [unroll] (j in range(G)) {
                                        pv[j] += kdot8(kw[b], uint(j) * 512u + wb * 8u)
                                    }
                                }
                            }
                            wi += 8u
                        }
                    }
                }
                if (!vec) {
                    var di = 0u
                    while (di < hs) {
                        let kv0 = float(kmir[krow + di])   // nolint:PERF020 -- KT convert-on-load; a no-op only in the f32 stamp
                        for [unroll] (j in range(G)) {
                            pv[j] += qrow[uint(j) * 512u + di] * kv0
                        }
                        di++
                    }
                }
                for [unroll] (j in range(G)) {
                    sc[uint(j) * 256u + tid] = attn_softcap(pv[j] * pa.scale, pa.cap)
                }
            }
            barrier()
            var s : float[4]
            for [unroll] (j in range(G)) {
                let hj = uint(j)
                s[j] = tid < clen && hj < live ? sc[hj * 256u + tid] : -3.0e38
                let tmax = subgroupMax(s[j])
                if (lane == 0u) {
                    sgp[hj * 32u + gl_SubgroupID] = tmax
                }
            }
            barrier()
            var alpha : float[4]
            for [unroll] (j in range(G)) {
                let hj = uint(j)
                var mnew = ml[hj * 4u]
                var sgi = 0u
                while (sgi < gl_NumSubgroups) {
                    mnew = max(mnew, sgp[hj * 32u + sgi])
                    sgi++
                }
                alpha[j] = exp(ml[hj * 4u] - mnew)
                let e = tid < clen && hj < live ? exp(s[j] - mnew) : 0.0
                sc[hj * 256u + tid] = e
                let tsum = subgroupAdd(e)
                if (lane == 0u) {
                    sgp[hj * 32u + 16u + gl_SubgroupID] = tsum
                }
            }
            barrier()
            if (tid < uint(G)) {   //! a thread a head carries the running max and denominator forward
                var mnew = ml[tid * 4u]
                var lsum = 0.0
                var sgi = 0u
                while (sgi < gl_NumSubgroups) {
                    mnew = max(mnew, sgp[tid * 32u + sgi])
                    lsum += sgp[tid * 32u + 16u + sgi]
                    sgi++
                }
                ml[tid * 4u + 1u] = ml[tid * 4u + 1u] * exp(ml[tid * 4u] - mnew) + lsum
                ml[tid * 4u] = mnew
            }
            for [unroll] (j in range(G)) {
                for [unroll] (e in range(8)) {
                    acc[j * 8 + e] *= alpha[j]
                }
            }
            var jj = my_kgroup < kgroups ? my_kgroup : clen
            while (jj < clen) {
                static_if (KV16) {
                    if (vec) {
                        if (jj != my_kgroup) {
                            for [unroll] (b in range(8)) {
                                vw[b] = vw4[(vb + (c0 + min(jj + uint(b) * kgroups, clen - 1u)) * pa.kvd) / 8u]
                            }
                        }
                        for [unroll] (b in range(8)) {
                            let kb = jj + uint(b) * kgroups
                            if (kb < clen) {
                                let a = unpackHalf2x16(vw[b].x)
                                let bb = unpackHalf2x16(vw[b].y)
                                let cc = unpackHalf2x16(vw[b].z)
                                let d = unpackHalf2x16(vw[b].w)
                                for [unroll] (j in range(G)) {
                                    let sj = sc[uint(j) * 256u + kb]
                                    acc[j * 8] += sj * a.x
                                    acc[j * 8 + 1] += sj * a.y
                                    acc[j * 8 + 2] += sj * bb.x
                                    acc[j * 8 + 3] += sj * bb.y
                                    acc[j * 8 + 4] += sj * cc.x
                                    acc[j * 8 + 5] += sj * cc.y
                                    acc[j * 8 + 6] += sj * d.x
                                    acc[j * 8 + 7] += sj * d.y
                                }
                            }
                        }
                    }
                }
                if (!vec) {
                    let vrow = vb + (c0 + jj) * pa.kvd
                    for [unroll] (e in range(8)) {
                        let v = vld(vrow + uint(e))
                        for [unroll] (j in range(G)) {
                            acc[j * 8 + e] += sc[uint(j) * 256u + jj] * v
                        }
                    }
                }
                jj += vec ? 8u * kgroups : kgroups
            }
            barrier()   // sc/sgp/ml settle before the next chunk overwrites them
            c0 += 256u
        }
        let stride = da_attn_part_stride(hs)
        var r = 1u
        while (r < kgroups_per_subgroup) {
            for [unroll] (k in range(G * 8)) {
                acc[k] += subgroupShuffleXor(acc[k], r * vrow_words)
            }
            r *= 2u
        }
        let ngroups_folded = folded ? gl_NumSubgroups : kgroups
        let nsl_hs = ngroups_folded * hs
        let round_cap = min(2048u / nsl_hs, 256u / hs)   //! a round's heads: what the slab holds, and what a thread a dim can reduce
        let heads_per_round = max(1u, round_cap)
        let has_sum = folded ? lane < vrow_words : my_kgroup < kgroups
        let slot = folded ? gl_SubgroupID : my_kgroup
        var hr0 = 0u
        while (hr0 < live) {
            for [unroll] (j in range(G)) {
                let hj = uint(j)
                if (hj >= hr0 && hj < hr0 + heads_per_round && hj < live && has_sum) {
                    for [unroll] (e in range(8)) {
                        qrow[(hj - hr0) * nsl_hs + slot * hs + w * 8u + uint(e)] = acc[j * 8 + e]
                    }
                }
            }
            barrier()
            let hr = wide ? 0u : tid / hs
            let dd = wide ? tid : tid % hs
            if (hr < heads_per_round && hr0 + hr < live) {
                let h = hr0 + hr
                var tot = 0.0
                var tot2 = 0.0
                var sl = 0u
                while (sl < ngroups_folded) {
                    tot += qrow[hr * nsl_hs + sl * hs + dd]
                    if (wide) {
                        tot2 += qrow[hr * nsl_hs + sl * hs + dd + 256u]
                    }
                    sl++
                }
                let mrun = ml[h * 4u]
                let l = ml[h * 4u + 1u]
                let pb = ((hflat0 + h) * nsplit + sp) * stride   // this (row, head, split)'s piece
                if (nsplit > 1u) {
                    if (dd == 0u) {
                        part[pb] = mrun
                        part[pb + 1u] = l
                    }
                    part[pb + 2u + dd] = tot
                    if (wide) {
                        part[pb + 2u + dd + 256u] = tot2
                    }
                } else {
                    let linv = unsplit_linv(h0 + h, mrun, l)
                    finish_head(h0 + h, hflat0 + h, hs, qb, qstride, dd, tot * linv, tot2 * linv, wide)
                }
            }
            barrier()   //! the slab settles before the next round
            hr0 += heads_per_round
        }
        if (nsplit > 1u) {
            let rowgroups = pa.rowwg == 0u ? gl_NumWorkGroups.x / nsplit : pa.rowwg / nsplit
            let ci = pa.cntoff + row * rowgroups + kvslab
            memoryBarrierBuffer()
            barrier()
            if (tid == 0u) {
                arrived[0] = atomicAdd(partu[ci], 1u)
            }
            barrier()
            if (arrived[0] + 1u == nsplit) {
                memoryBarrierBuffer()
                for [unroll] (j in range(G)) {
                    let hj = uint(j)
                    if (hj < live) {
                        combine_head(h0 + hj, hflat0 + hj, hs, nsplit, stride, qb, qstride, tid, wide)
                    }
                }
                if (tid == 0u) {
                    partu[ci] = 0u
                }
            }
        }
    }
}

[vk_dispatch(name = "da_attn_cls", grid = "wgs", params = "wgs : int64")]
class DaAttn : DaAttnT {
    typedef KT = float
}

// f16-mirror twin — convert-on-load, compute stays f32
[vk_dispatch(name = "da_attn_f16_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnF16 : DaAttnT {
    typedef KT = float16
    override KV16 = true
}

[vk_dispatch(name = "da_attn_g2_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnG2 : DaAttnT {
    typedef KT = float
    override G = 2
}

[vk_dispatch(name = "da_attn_f16_g2_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnF16G2 : DaAttnT {
    typedef KT = float16
    override KV16 = true
    override G = 2
}

[vk_dispatch(name = "da_attn_sink_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnSink : DaAttnT {
    typedef KT = float
    override SINK = true
}

[vk_dispatch(name = "da_attn_f16_sink_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnF16Sink : DaAttnT {
    typedef KT = float16
    override KV16 = true
    override SINK = true
}

// ===== the Q8_0 batch GEMM tile (the sdot4 prefill workhorse) =====

struct BatchArgs {
    n : uint         // reduction dim
    d : uint         // output cols
    map_off : uint   // the per-wg map's word offset in sched (region records sit at 0)
    ksplit : uint    // cm2 split-k: 256-aligned k chunk per partial plane; 0 = no split
}

// the MoE region rail every batch tile shares: sched = 4-word region records [wblk0, row0, cnt,
// wg0] at 0, then the per-wg map at pa.map_off; region_rec = this workgroup's record - x = weight
// base, y = activation row base, z = row count, w = the wg's tile index rebased to the region
let SCHED_NONE = 0xFFFFFFFFu   //! the per-wg map's sentinel: a tile workgroup past a device-written schedule's end reads a zero-row region and returns

class MoeCmBase {
    @ssbo @binding = 2 sched : array<uint>   // region records + per-wg map (bulk schedule data)
    @push_constant pa : BatchArgs

    def region_rec : uint4 {
        let rid = sched[pa.map_off + gl_WorkGroupID.x]
        if (rid == SCHED_NONE) {
            return uint4(0u, 0u, 0u, 0u)
        }
        let rb = rid * 4u
        return uint4(sched[rb], sched[rb + 1u], sched[rb + 2u], gl_WorkGroupID.x - sched[rb + 3u])
    }
}

// 32x32 tiles through shared so weight rows stream once per 32 rows; K advances 8 blocks per
// barrier pair.
[vk_dispatch(name = "q8_batch_cls", grid = "wgs", params = "wgs : int64")]
class Q8Batch : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @workgroup xw4 : uint4[544]   // x tile: 32 rows x 16 uint4 chunks, stride 17 (bank-spread)
    @workgroup ww4 : uint4[544]   // w tile: 32 cols x 16 chunks, stride 17
    @workgroup xsw : float[256]   // x scales per (block, row)
    @workgroup wsw : float[256]   // w scales per (block, col)

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
// the grid codebooks, one word per call - the staging loops copy them into workgroup memory once
// per kernel (a few indexed reads per thread), so the local-array storage class (the slow path
// of `followup_general.md` item 136) never sits on a hot path; the literal is kqformat's, baked
// in by [grid_words] because a shader function cannot return a fixed array
[grid_words(of = "iq3s_grid")]
def iq3s_grid_word(i : int) : uint {}

// iq2xxs_grid as low/high word pairs - 256 u64 entries, one uint word per call (word
// 2i = magnitudes 0..3 of entry i, word 2i+1 = 4..7); staged into workgroup memory per kernel.
[grid_words(of = "iq2xxs_grid2")]
def iq2xxs_grid_word(i : int) : uint {}

// iq2xs_grid as low/high word pairs - 512 u64 entries, one uint word per call (word
// 2i = magnitudes 0..3 of entry i, word 2i+1 = 4..7); staged into workgroup memory per kernel.
[grid_words(of = "iq2xs_grid2")]
def private iq2xs_grid_word(i : int) : uint {}

// iq2s_grid as low/high word pairs - 1024 u64 entries, one uint word per call (word 2i =
// magnitudes 0..3 of entry i, word 2i+1 = 4..7); the staging loops copy it into workgroup
// memory once per kernel, so the local-array storage class never sits on a hot path.
[grid_words(of = "iq2s_grid2")]
def private iq2s_grid_word(i : int) : uint {}

// the HALVED iq3xxs grid (iq3xxs_grid with every byte / 2 - the plane convention that
// absorbs the format's 0.25 into the stored d), one word per call; staging loops copy it into
// workgroup memory (the slow path of `followup_general.md` item 136 stays off the hot loop).
[grid_words(of = "iq3xxs_grid")]
def iq3xxs_grid_word(i : int) : uint {}

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

[arch(at="../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane")]
def iq4xs_sc(w0, w1, blk : uint) : int {
    let ls = ((w1 >> (4u * blk)) & 15u) | (((w0 >> (16u + 2u * blk)) & 3u) << 4u)
    return int(ls) - 32
}

[arch(at="../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane")]
def grid_sc(w1, blk : uint) : int {
    return 1 + 2 * int((w1 >> (4u * blk)) & 15u)
}

//! sign-apply by xor-and-add: the magnitudes are odd, so no byte carries into the next
def iq3s_signed(g, nib : uint) : uint {
    let m1 = (nib * 0x00204081u) & 0x01010101u
    let mask = m1 * 255u
    return (g ^ mask) + m1
}

class KqBatchBase : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant planes (per-format block layout)
    @ssbo @binding = 1 wsu : array<uint>       // scale planes (5-word superblock layout)
    @ssbo @binding = 3 xqw : array<uint>       // Q8_K activation words (64 per row-superblock)
    @ssbo @binding = 4 axs : array<float>      // one Q8_K d per (row, superblock)
    @ssbo @binding = 5 y : array<float>
    @workgroup xw4 : uint4[544]
    @workgroup ww4 : uint4[544]
    @workgroup xsw : float[256]
    @workgroup wsw : float[256]    // first per-(block, col) scale plane
    @workgroup wsw2 : float[256]   // second plane (the d/min pair's min side; k6's hi sub-scale)

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
            let w0 = wsu[wsb * 2u]
            sa = unpackHalf2x16(w0).x * float(iq4xs_sc(w0, wsu[wsb * 2u + 1u], sblk))
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

// the grid formats' scale stage over the iq3 row: d x the (1 + 2s) fields, not iq4xs's ls - 32;
// iq4xs's fold (no bsum term) inherits. iq3s, iq3xxs and iq2xxs stage their grids and gather on it
class KqBatchGridBase : KqBatchIq4xs {
    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            sa = unpackHalf2x16(wsu[wsb * 2u]).x * float(grid_sc(wsu[wsb * 2u + 1u], sblk))
        }
        wsw[sblk * 32u + slane] = sa
    }
}

// IQ3_S tile: the iq4xs staging with the GRID GATHER in stage_w - each staged uint4 is four
// grid words sign-applied. The 2 KB grid stages into workgroup memory once, before the tile loop.
[vk_dispatch(name = "kq_batch_iq3s_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq3s : KqBatchGridBase {
    @workgroup gridw : uint[512]

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
// bit), signs from the aux32's four 7-bit ksigns indices via the parity helper (no table).
// The 1 KB grid stages once.
[vk_dispatch(name = "kq_batch_iq3xxs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq3xxs : KqBatchGridBase {
    @workgroup gridw : uint[256]

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
// carries four BYTE indices, its aux32 (column 2b+1) the parity-derived ksigns; the iq3 strip
// stage and iq4xs's fma inherit (the (2ls+1) strips read exactly under the signed extract,
// staying below 128).
[vk_dispatch(name = "kq_batch_iq2xxs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2xxs : KqBatchGridBase {
    @workgroup gridw : uint[512]

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

    //! the quant offset the split-half fold subtracts through each half's block sum
    def q_off : int => 32

    // the split-half FMA: each 16-weight half keeps its own dot, sum, and signed sub-scale
    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        let off = q_off()
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
        return float4(xscl * (wsw[bu * 32u + word] * float(l0 - off * blo) + wsw2[bu * 32u + word] * float(h0 - off * bhi)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l1 - off * blo) + wsw2[bu * 32u + word + 8u] * float(h1 - off * bhi)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l2 - off * blo) + wsw2[bu * 32u + word + 16u] * float(h2 - off * bhi)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l3 - off * blo) + wsw2[bu * 32u + word + 24u] * float(h3 - off * bhi)))
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

    def override q_off : int => 4

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

// the iq2 grid formats' scale stage and fold: per-16 UNSIGNED strips on k6's wsw/wsw2 planes and a
// plain split fma (no offset, no min term); iq2s and iq2xs stage their grids and gather on it
class KqBatchIq2Base : KqBatchK6 {
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
}

// IQ2_S tile: the iq3s grid gather widened to the u64 grid (a 10-bit qs|qh index picks TWO
// words - eight magnitudes) with the block's own sign bytes. The 8 KB grid stages once, 8 words
// per thread.
[vk_dispatch(name = "kq_batch_iq2s_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2s : KqBatchIq2Base {
    @workgroup gridw : uint[2048]

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
// (no sign plane, no qh). The 4 KB grid stages once.
[vk_dispatch(name = "kq_batch_iq2xs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2xs : KqBatchIq2Base {
    @workgroup gridw : uint[1024]

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
let private KHR_STRIDE = 20u   // the KHR kq tile's shared row stride in 32-bit words: 32 k halves as 16 f16 pairs + 4 pad (bank spread)
let private KHR_KHALF_WORDS = 8u   // the 16 k halves one thread stages per step, as f16 pairs

// The f16 tensor-core 32x32 tile shell (DASLLAMA_COOPMAT=f16): the MoE routing of Q8Batch with the
// inner GEMM on the tensor cores - 128 threads / 4 subgroups, one 16x16 subtile each, the staged
// f16 A (32 rows x 256 k) and B (256 k x 32 cols) tiles, 16 MMAs per step, the f32 scratch bounce
// for the bounds-checked store. A leaf stages its own weight format into the tiles (edge rows
// zero-fill) and hands the shell its step count and the block count its stage reads.
class CmF16Base : MoeCmBase {
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @workgroup cm_as : float16[8192]   // 32 local rows x 256 k, row-major stride 256 (f16 dequant A)
    @workgroup cm_bs : float16[8192]   // 256 k x 32 cols, row-major stride 32 (f16 dequant B)
    @workgroup cm_out : float[1024]    // 4 subgroups x 16x16 f32 scratch (bounds-checked write-out)

    // step s of the leaf's format: the activation rows into cm_as, the weight columns into cm_bs
    def stage(s, wblk0, row0, cnt, xt, wt, nblk, tid : uint) {
    }

    def body(nsteps, nblk : uint) {
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID          // 0..3 (subgroupSize 32)
        let row_half = sg / 2u               // which 16-row half of the tile
        let col_half = sg % 2u               // which 16-col half of the tile
        var acc : coopmatAcc_f32_16x16
        var s = 0u
        while (s < nsteps) {
            stage(s, wblk0, row0, cnt, xt, wt, nblk, tid)
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

// the Q8_0 leaf: eight 32-blocks per step, each staged uint the four quants of one block word
[vk_dispatch(name = "q8_batch_cmf16_cls", grid = "wgs", params = "wgs : int64")]
class CmF16Batch : CmF16Base {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales

    def override stage(s, wblk0, row0, cnt, xt, wt, nblk, tid : uint) {
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
            if (xrow < cnt && gblk < nblk) {
                packed = xqw[((row0 + xrow) * nblk + gblk) * 8u + w]
                xs = axs[(row0 + xrow) * nblk + gblk]
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
            if (wcol < pa.d && gblk < nblk) {
                packed = wq[(wblk0 + wcol * nblk + gblk) * 8u + w]
                ws = float(wsh[wblk0 + wcol * nblk + gblk])
            }
            let qv = int4(unpack8(int(packed)))
            let k = blk * 32u + w * 4u
            cm_bs[k * 32u + col] = float16(float(qv.x) * ws)
            cm_bs[(k + 1u) * 32u + col] = float16(float(qv.y) * ws)
            cm_bs[(k + 2u) * 32u + col] = float16(float(qv.z) * ws)
            cm_bs[(k + 3u) * 32u + col] = float16(float(qv.w) * ws)
        }
    }

    [spirv_kernel(local_size_x = 128, name = "q8_batch_cmf16_cls_spv")]
    def run {
        let nbb = pa.n / 32u
        body((nbb + 7u) / 8u, nbb)
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

// The mul_mm tile (DASLLAMA_COOPMAT=mm) as ONE class template over the tile edge and the edge
// policy. The fragment sets are per-edge `static_if` arms: a cooperative-matrix handle cannot sit
// in a sized array, and each arm's MMA order is parity-locked to the shipped kernel.
[ |> template_struct_instance]
class template MmBatchT : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @template_constant BT : uint = 128u        // the tile edge: 128 = the aligned-L geometry (BM=BN=128 BK=32, 4 warps 2x2 with 16 f16acc fragments each), 64 = the M twin (2x2 fragments each, routed in when cnt or d <= 64)
    @template_constant EDGE : bool = true      // rows past d / cnt exist: guarded staging, bounced stores; the aligned-L leaf runs with it off, its own pipeline for d and every cnt 128-multiples
    @workgroup cml_a : uint[int(BT * CML_SST)]   // A tile: BT weight rows x one block (32 K) as f16 pairs
    @workgroup cml_b : uint[int(BT * CML_SST)]   // B tile: BT token rows x one block
    @workgroup @template_gate = EDGE cml_o : float16[1024]   // 4 warps x 16x16 f16-acc store bounce (edge tiles only)

    // stage A: BT weight rows x 1 block; OOR rows -> 0 on the edge policy
    def stage_a(wblk0, gm0, nbb, kb, lw, lr0 : uint) {
        for [unroll] (p in range(int(BT) / 16)) {
            let row = lr0 + uint(p) * 16u
            let wrow = gm0 + row
            var v = float4(0.0)
            static_if (EDGE) {
                if (wrow < pa.d) {
                    let ib = wblk0 + wrow * nbb + kb
                    v = float4(int4(unpack8(int(wq[ib * 8u + lw])))) * float(wsh[ib])
                }
            } else {
                let ib = wblk0 + wrow * nbb + kb
                v = float4(int4(unpack8(int(wq[ib * 8u + lw])))) * float(wsh[ib])
            }
            let o = row * CML_SST + lw * 2u
            cml_a[o] = packHalf2x16(v.xy)
            cml_a[o + 1u] = packHalf2x16(v.zw)
        }
    }

    // stage B: BT token rows x 1 block; OOR rows -> 0 on the edge policy
    def stage_b(row0, gn0, cnt, nbb, kb, lw, lr0 : uint) {
        for [unroll] (p in range(int(BT) / 16)) {
            let row = lr0 + uint(p) * 16u
            var v = float4(0.0)
            static_if (EDGE) {
                if (gn0 + row < cnt) {
                    let ib = (row0 + gn0 + row) * nbb + kb
                    v = float4(int4(unpack8(int(xqw[ib * 8u + lw])))) * axs[ib]
                }
            } else {
                let ib = (row0 + gn0 + row) * nbb + kb
                v = float4(int4(unpack8(int(xqw[ib * 8u + lw])))) * axs[ib]
            }
            let o = row * CML_SST + lw * 2u
            cml_b[o] = packHalf2x16(v.xy)
            cml_b[o + 1u] = packHalf2x16(v.zw)
        }
    }

    // an edge tile's fragment, already stored to this warp's cml_o slab (rowmajor, row = weight
    // dim): 32 lanes widen and write 8 elements each under the row and column bounds
    def bounce_out(fr, fc, dr, dc, row0, cnt : uint) {
        static_if (EDGE) {
            let lane = gl_SubgroupInvocationID
            let ob = gl_SubgroupID * 256u
            for (e in range(8)) {
                let idx = lane * 8u + uint(e)
                let wr = dr + fr * 16u + idx / 16u
                let tc = dc + fc * 16u + idx % 16u
                if (wr < pa.d && tc < cnt) {
                    y[(row0 + tc) * pa.d + wr] = float(cml_o[ob + idx])
                }
            }
        }
    }

    [spirv_kernel(local_size_x = 128)]
    def run {
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + BT - 1u) / BT
        let xt = tix / wtiles             // token (B) tile
        let wt = tix % wtiles             // weight (A) tile
        let gm0 = wt * BT
        let gn0 = xt * BT
        let nbb = pa.n / 32u
        let tid = gl_LocalInvocationID.x
        let warp_r = gl_SubgroupID % 2u   // which half of the A tile
        let warp_c = gl_SubgroupID / 2u   // which half of the B tile
        let lw = tid % 8u                 // which packed word (4 qs) of the staged row's block
        let lr0 = tid / 8u                // staging row base 0..15
        static_if (BT == 128u) {
            l_tile(wblk0, row0, cnt, gm0, gn0, nbb, warp_r, warp_c, lw, lr0)
        } else {
            m_tile(wblk0, row0, cnt, gm0, gn0, nbb, warp_r, warp_c, lw, lr0)
        }
    }

    def l_tile(wblk0, row0, cnt, gm0, gn0, nbb, warp_r, warp_c, lw, lr0 : uint) {   // nolint:STYLE038 — the verbatim tile pass, parity-locked to the shipped kernel
        static_if (BT == 128u) {
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
            stage_a(wblk0, gm0, nbb, kb, lw, lr0)
            stage_b(row0, gn0, cnt, nbb, kb, lw, lr0)
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
        let whole = !EDGE || (gm0 + 128u <= pa.d && gn0 + 128u <= cnt)
        if (whole) {
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
            static_if (EDGE) {
                // edge tile: every fragment bounces its f16 acc through this warp's cml_o slab
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
                    bounce_out(fr, fc, dr, dc, row0, cnt)
                    barrier()
                }
            }
        }
        }
    }

    def m_tile(wblk0, row0, cnt, gm0, gn0, nbb, warp_r, warp_c, lw, lr0 : uint) {
        static_if (BT == 64u) {
        var acc00 : coopmatAcc_f16_16x16
        var acc01 : coopmatAcc_f16_16x16
        var acc10 : coopmatAcc_f16_16x16
        var acc11 : coopmatAcc_f16_16x16
        var kb = 0u
        while (kb < nbb) {
            stage_a(wblk0, gm0, nbb, kb, lw, lr0)
            stage_b(row0, gn0, cnt, nbb, kb, lw, lr0)
            barrier()
            for [unroll] (ks in range(2)) {   // 2 K-subtiles x (2 A rows x 2 B cols), the L MMA order
                let k8 = uint(ks) * 8u
                var ca : coopmatA_f16_16x16
                var cb : coopmatB_f16_16x16
                coopmatLoad(ca, cml_a, int((warp_r * 32u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 32u) * CML_SST + k8), int(CML_SST), 1)
                acc00 = coopmatMulAdd(ca, cb, acc00)
                coopmatLoad(cb, cml_b, int((warp_c * 32u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc01 = coopmatMulAdd(ca, cb, acc01)
                coopmatLoad(ca, cml_a, int((warp_r * 32u + 16u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 32u) * CML_SST + k8), int(CML_SST), 1)
                acc10 = coopmatMulAdd(ca, cb, acc10)
                coopmatLoad(cb, cml_b, int((warp_c * 32u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc11 = coopmatMulAdd(ca, cb, acc11)
            }
            barrier()
            kb++
        }
        // stores: y[(row0 + token) * d + wrow] — same column-major layout as the L tile
        let dr = gm0 + warp_r * 32u
        let dc = gn0 + warp_c * 32u
        let whole = !EDGE || (gm0 + 64u <= pa.d && gn0 + 64u <= cnt)
        if (whole) {
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
            static_if (EDGE) {
                // edge tile: bounce each f16 acc through this warp's cml_o slab, bounds-checked write-out
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
                    bounce_out(fr, fc, dr, dc, row0, cnt)
                    barrier()
                }
            }
        }
        }
    }
}

// the L tile with edge tiles
[vk_dispatch(name = "q8_batch_mm_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class MmBatch : MmBatchT {
    override BT = 128u
    override EDGE = true
}

// the aligned L twin: staging guards + edge branch removed — its own pipeline for d and every cnt 128-multiples
[vk_dispatch(name = "q8_batch_mm_a_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class MmABatch : MmBatchT {
    override BT = 128u
    override EDGE = false
}

// the M twin (64x64), routed in when cnt or d <= 64
[vk_dispatch(name = "q8_batch_mm_m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class MmMBatch : MmBatchT {
    override BT = 64u
    override EDGE = true
}

// Q4_0 coopmat f16 variant (fmt=4 stacks): the q40 math baked into f16 dequant — w = (nib-8)*d_b,
// a = aq*xs (per-256 Q8_K) — then the same 4-subgroup MMA as CmF16Batch
[vk_dispatch(name = "kq_batch_q40_cmf16_cls", grid = "wgs", params = "wgs : int64")]
class KqQ40CmF16 : CmF16Base {
    @ssbo @binding = 0 wq : array<uint>        // nibble-packed weight superblocks (32 words each)
    @ssbo @binding = 1 wsu : array<uint>       // 5-word scale superblocks (d halves)

    def override stage(s, wblk0, row0, cnt, xt, wt, nblk, tid : uint) {
        // stage A: 32 rows x 64 uints (one superblock of Q8_K acts), 16/thread — one thread stays
        // in one row (16 divides 64), so xs (per-256) loads ONCE per thread
        let arow0 = tid * 16u / 64u            // this thread's row
        let ac0 = tid * 16u % 64u              // first chunk within the row
        let axrow = xt * 32u + arow0
        var xs = 0.0
        if (axrow < cnt) {
            xs = axs[(row0 + axrow) * nblk + s]
        }
        for (i in range(16)) {
            let c = ac0 + uint(i)
            let bu = c / 8u
            let ww = c % 8u
            var packed = 0u
            if (axrow < cnt) {
                packed = xqw[((row0 + axrow) * nblk + s) * 64u + c]
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
                let wsb = wblk0 + wcol * nblk + s
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
    }

    [spirv_kernel(local_size_x = 128, name = "kq_batch_q40_cmf16_cls_spv")]
    def run {
        let nsb = pa.n / 256u
        body(nsb, nsb)
    }
}

// ===== the cm2 (NV_cooperative_matrix2) prefill tiles: native planes, decode-in-load =====

struct VkQ8Blk {
    qs : int16[16]   // one fmt-0 q8 block: 32 packed int8 quants as 16-bit lanes; the scale lives in ws
}

struct VkK4Blk {
    qs : int16[64]   // one Q4_K superblock's 128 nibble bytes as 16-bit lanes; its scale row (d|dmin + 8 sc + 8 mn bytes) lives in ws
}

struct VkQ51Blk {
    qs : int16[10]   //! one Q5_1 32-block: 16 nibble bytes (element j low, j + 16 high) then the 4 qh bytes (bit e = element e's fifth bit); d | m live in ws as one word
}

struct VkMx4Blk {
    qs : int16[8]   //! one MXFP4 32-block: 16 nibble bytes (element j low, j + 16 high, the e2m1 code); the E8M0 scale byte lives in ws, four blocks a word
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

let SC_STRIDE = 130u   //! the cm2 scale cache's sub-block stride: the tile's 128 rows and two of pad

// The cm2 tile: one class template over three stamp axes - the token column BN, the k step BK
// and the weight format: BLK
// the 16-bit-lane block struct, ST the scale plane's element, BLKW the block's element width, and
// the format template's decode. 256 threads, one 128-weight x BN-token tile per wg, BK per
// step, a k loop unrolled UNR steps (one superblock); the decode METHOD reads the separate scale plane by block index -
// the reason the class-method decode form exists. Binding 4 unused (sparse)
[ |> template_struct_instance]
class template KqCm2BatchT : MoeCmBase {
    @ssbo @binding = 0 @role = "weight" wq : array<BLK>   // the quant plane, read as 16-bit lanes
    @ssbo @binding = 1 @role = "weight" ws : array<ST>    // the scale plane (q8: f16 per block; kq: 5 words per superblock)
    @ssbo @binding = 3 @role = "alias" xf16 : array<float16>    // f16 activation plane (the tensor loads' view)
    @ssbo @binding = 3 @role = "alias" xf16w4 : array<uint4>    // the same plane as 16-byte words: the KHR arm's activation stage
    @ssbo @binding = 0 @role = "alias" @template_gate = KHR @readonly wq4 : array<uint4>
    @ssbo @binding = 0 @role = "alias" @template_gate = KHR @readonly wqw : array<uint>
    @ssbo @binding = 5 y : array<float>
    @workgroup wg_blk0 : uint                             // the region's block base, staged for the decode
    @workgroup wg_m0 : uint                               //! the tile's first weight row: the scale caches index a row by its offset from it (a partial last tile starts off the 128 grid)
    @template_constant BN : uint = 256u                   // the token column
    @template_constant STILE : bool = false               // the 32-row column's partial-column fast path
    @template_constant BLKW : uint = 256u                 // elements per weight block
    @template_constant BK : uint = 64u                    //!< the k step of the tile loop (its AT/BT carry the depth)
    @template_constant UNR : uint = 4u                    //!< the k loop's hand unroll: BK x UNR is one superblock (8 on a 32-deep stamp)
    @template_constant DECVEC : bool = true               //!< when DECV4 is off: the synthesized four-wide twin rides the loads; false serves the scalar callback only
    @template_constant DECV4 : bool = false               //!< the format's hand-laid `decode_v4` is the twin instead of the synthesized one
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
    @template_constant KHR : bool = false                 //!< the KHR cooperative-matrix arm: hand-staged f16 tiles over the format's own decode, no cm2 API (a card without NV_coopmat2)
    @workgroup @template_gate = KHR khr_ao : uint[int(128u * KHR_STRIDE)]   //! the A stage: 128 weight rows of f16 pairs; then the edge tile's f32 store bounce, 8 subgroups x 256 words
    @workgroup @template_gate = KHR khr_b : uint[int(128u * KHR_STRIDE)]    //! the B stage: BN (128) token rows of f16 pairs
    @template_constant SCACHE : bool = false              //!< the scale cache: the tile's 128 rows' eight sub-block scale pairs staged per superblock
    @template_constant SCIQ4 : bool = false               //!< the cache's fill reads the IQ4_XS two-word scale row (d | the high scale bits, the low nibbles) instead of the K-quant five
    @template_constant SCK6 : bool = false                //!< the cache's fill reads the Q6_K row (sixteen int8 sub-block scales and d) into sixteen d x sc entries instead of the K-quant five's eight pairs
    @workgroup @template_gate = SCACHE sc_cache : float2[int(8u * SC_STRIDE)]   //! sub-block-major, row-minor, two rows of pad per sub-block against bank conflicts
    @workgroup @template_gate = SCK6 sc6_cache : float[int(16u * SC_STRIDE)]     //! k6's sixteen d x sc entries a superblock, the same layout, no minuend
    @ssbo @binding = 1 @role = "alias" @template_gate = SCACHE ws_words : array<uint>   //! the scale plane as words, the cache's source (K-quant: five per superblock - d | dmin, eight scale bytes, eight min bytes; IQ4_XS: two)

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
    def run {
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        if (cnt == 0u) {
            return
        }
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
        let k0 = (ks * pa.ksplit) & ~(BK * UNR - 1u)
        let xt = tix % ttiles             // token BN-tile (fastest)
        let wt = tix / ttiles             // weight 128-tile
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
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
        static_if (KHR) {
            khr_tile(wblk0, row0, cnt, k0, k1, ybase, xt, wt)
        } else {
            cm2_tile(wblk0, row0, cnt, k0, k1, ybase, xt, wt)
        }
    }

    //! the scale cache for superblock column `sb` of the tile's rows from m0; a row past the plane reads the last row (its entries unused)
    def sc_fill(m0, wblk0, sb : uint) {
        static_if (SCACHE) {
            let t = gl_LocalInvocationID.x
            let r = t >> 1u
            let g0 = (t & 1u) * 4u
            let row = min(m0 + r, pa.d - 1u)
            static_if (SCIQ4) {
                let srow = (wblk0 + row * (pa.n >> 8u) + sb) * 2u
                let w0 = ws_words[srow]
                let w1 = ws_words[srow + 1u]
                let d = unpackHalf2x16(w0).x
                for [unroll] (i in range(4)) {
                    sc_cache[(g0 + uint(i)) * SC_STRIDE + r] = float2(d * float(iq4xs_sc(w0, w1, g0 + uint(i))), 0.0)
                }
            } static_elif (SCK6) {
                //! the Q6_K row: words 0-3 the sixteen int8 sub-block scales, word 4 the f16 d; a thread fills eight sub-blocks of its row from two words
                let srow = (wblk0 + row * (pa.n >> 8u) + sb) * 5u
                let d = unpackHalf2x16(ws_words[srow + 4u]).x
                let s0 = g0 * 2u
                let w0 = ws_words[srow + (s0 >> 2u)]
                let w1 = ws_words[srow + (s0 >> 2u) + 1u]
                for [unroll] (i in range(8)) {
                    let w = i < 4 ? w0 : w1
                    let sc = int(w << ((3u - uint(i & 3)) * 8u)) >> 24
                    sc6_cache[(s0 + uint(i)) * SC_STRIDE + r] = d * float(sc)
                }
            } else {
                let srow = (wblk0 + row * (pa.n >> 8u) + sb) * 5u
                let dm = unpackHalf2x16(ws_words[srow])
                let scw = ws_words[srow + 1u + (g0 >> 2u)]
                let mnw = ws_words[srow + 3u + (g0 >> 2u)]
                for [unroll] (i in range(4)) {
                    let sh = uint(i) * 8u
                    sc_cache[(g0 + uint(i)) * SC_STRIDE + r] = float2(dm.x * float((scw >> sh) & 0xFFu), dm.y * float((mnw >> sh) & 0xFFu))
                }
            }
        }
    }

    [arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def sc_step(m0, wblk0, k : uint) {
        static_if (SCACHE) {
            if ((k & (BLKW - 1u)) == 0u) {
                barrier()
                sc_fill(m0, wblk0, k / BLKW)
                barrier()
            }
        }
    }

    def cm2_tile(wblk0, row0, cnt, k0, k1, ybase, xt, wt : uint) {   // nolint:STYLE037,STYLE038 — the fast/edge path pair, barrier- and register-coupled
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var a : AT
        var b : BT
        var acc : ACC
        let t0 = row0 + xt * BN
        //! a partial last weight tile takes the plane's last whole 128 rows, its overlap rewriting the same values
        let m0 = wt * 128u + 128u <= pa.d || pa.d < 128u ? wt * 128u : pa.d - 128u
        if (gl_LocalInvocationID.x == 0u) {
            wg_m0 = m0   //! visible past the scale cache's first barrier, ahead of every decode
        }
        if (m0 + 128u <= pa.d && (STILE || xt * BN + BN <= cnt) && (pa.n & (BK - 1u)) == 0u) {
            var fla : tensorLayout2D
            tensorLayoutCreate(fla)
            tensorLayoutSetBlockSize(fla, 1u, BLKW)
            tensorLayoutSetDimension(fla, pa.d, pa.n)
            tensorLayoutSetStride(fla, pa.n / BLKW, 1u)
            var flb : tensorLayout2D
            tensorLayoutCreate(flb)
            tensorLayoutSetDimension(flb, row0 + (STILE ? ((cnt + BN - 1u) & ~(BN - 1u)) : cnt), pa.n)
            tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
            let whole = xt * BN + BN <= cnt
            var flw : tensorLayout2D
            tensorLayoutCreate(flw)
            tensorLayoutSetDimension(flw, row0 + cnt, pa.d)
            tensorLayoutSetStride(flw, pa.d & ~7u, 1u)
            var flp : tensorLayout2DPad
            tensorLayoutCreate(flp)
            tensorLayoutSetDimension(flp, row0 + cnt, pa.d)
            tensorLayoutSetStride(flp, pa.d & ~7u, 1u)
            if (pa.ksplit == 0u) {
                var k = 0u
                for (_i in range(int(pa.n / (BK * UNR)))) {
                    sc_step(m0, wblk0, k)
                    for [unroll] (_j in range(int(UNR))) {
                        static_if (DECV4) {
                            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, self.decode_v4)
                        } else {
                            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, DECVEC)
                        }
                        coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, BK, tv)
                        acc = coopmatMulAdd(a, b, acc)
                        k += BK
                    }
                }
                while (k < pa.n) {
                    sc_step(m0, wblk0, k)
                    static_if (DECV4) {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, self.decode_v4)
                    } else {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, DECVEC)
                    }
                    coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, BK, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += BK
                }
                coopmatClamp(acc, -65504.0, 65504.0)
                var accw : ACCW
                coopmatConvert(accw, acc)
                if (whole) {
                    coopmatStoreTensor(accw, y, 0u, flw, t0, BN, m0, 128u, tv)
                } else {
                    coopmatStoreTensor(accw, y, 0u, flp, t0, BN, m0, 128u, tv)
                }
                return
            }
            var k = k0
            for (_i in range(int((k1 - k0) / (BK * UNR)))) {
                sc_step(m0, wblk0, k)
                for [unroll] (_j in range(int(UNR))) {
                    static_if (DECV4) {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, self.decode_v4)
                    } else {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, DECVEC)
                    }
                    coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, BK, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += BK
                }
            }
            while (k < k1) {
                sc_step(m0, wblk0, k)
                static_if (DECV4) {
                    coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, self.decode_v4)
                } else {
                    coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, DECVEC)
                }
                coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, BK, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += BK
            }
            coopmatClamp(acc, -65504.0, 65504.0)
            var accw : ACCW
            coopmatConvert(accw, acc)
            if (whole) {
                coopmatStoreTensor(accw, y, ybase, flw, t0, BN, m0, 128u, tv)
            } else {
                coopmatStoreTensor(accw, y, ybase, flp, t0, BN, m0, 128u, tv)
            }
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
            for (_i in range(int(pa.n / (BK * UNR)))) {
                sc_step(m0, wblk0, k)
                for [unroll] (_j in range(int(UNR))) {
                    coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, BK, self.decode)
                    coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, BK, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += BK
                }
            }
            while (k < pa.n) {
                sc_step(m0, wblk0, k)
                coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, BK, self.decode)
                coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, BK, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += BK
            }
            coopmatClamp(acc, -65504.0, 65504.0)
            var accw : ACCW
            coopmatConvert(accw, acc)
            coopmatStoreTensor(accw, y, 0u, tlo, t0, BN, m0, 128u, tv)
            return
        }
        var k = k0
        for (_i in range(int((k1 - k0) / (BK * UNR)))) {
            sc_step(m0, wblk0, k)
            for [unroll] (_j in range(int(UNR))) {
                coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, BK, self.decode)
                coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, BK, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += BK
            }
        }
        while (k < k1) {
            sc_step(m0, wblk0, k)
            coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, BK, self.decode)
            coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, BK, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += BK
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : ACCW
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, ybase, tlo, t0, BN, m0, 128u, tv)
    }

    //! a KHR stamp's format writes weight row `blk`'s 16 values e0 .. e0 + 16 into khr_ao[sbase .. sbase + 8) as f16 pairs, read from the plane's words; the cm2 stamps never call it
    def abstract khr_stage16(blk, e0, sbase : uint) : void

    [arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#khr-mm-kq-tile")]
    def khr_tile(wblk0, row0, cnt, k0, k1, ybase, xt, wt : uint) {   // nolint:STYLE038 — the staged k loop and its fast/edge store are one register set
        static_if (KHR) {   // the gated staging arrays exist on the KHR instantiations alone
        let t0 = row0 + xt * BN
        let m0 = wt * 128u
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let srow = tid >> 1u
        let khalf = tid & 1u   //! the low or the high 16 k of the step
        let skh = khalf * 16u
        let nbk = pa.n / BLKW
        let wrow = m0 + srow
        let trow = xt * BN + srow
        let sbase = srow * KHR_STRIDE + khalf * KHR_KHALF_WORDS
        let wrow_in_d = wrow < pa.d
        let trow_in_cnt = trow < cnt
        let wrow0 = (sg & 1u) * 64u    //! the subgroup's first weight row of the tile's 128
        let trow0 = (sg >> 1u) * 32u   //! the subgroup's first token row of the tile's 128
        var acc : coopmatAcc_f16_16x16[8]   //! [r * 2 + c]: weight fragment r against token fragment c
        var k = k0
        while (k < k1) {
            let kk = k + skh
            if (wrow_in_d && kk < k1) {
                let bcol = kk / BLKW
                khr_stage16(wblk0 + wrow * nbk + bcol, kk - bcol * BLKW, sbase)
            } else {
                for (j in range(int(KHR_KHALF_WORDS))) {
                    khr_ao[sbase + uint(j)] = 0u
                }
            }
            if (trow_in_cnt && kk < k1) {
                let q = ((t0 + srow) * pa.n + kk) >> 3u   // the 16 halves as two 16-byte words
                for (h in range(2)) {
                    let w4 = xf16w4[q + uint(h)]
                    for (c in range(4)) {
                        khr_b[sbase + uint(h) * 4u + uint(c)] = w4[c]
                    }
                }
            } else {
                for (j in range(int(KHR_KHALF_WORDS))) {
                    khr_b[sbase + uint(j)] = 0u
                }
            }
            barrier()
            for [unroll_full] (ks in range(2)) {
                let ko = uint(ks) * 8u
                var a : coopmatA_f16_16x16[4]
                for [unroll_full] (r in range(4)) {
                    coopmatLoad(a[r], khr_ao, int((wrow0 + uint(r) * 16u) * KHR_STRIDE + ko), int(KHR_STRIDE), 0)
                }
                for [unroll_full] (c in range(2)) {
                    var b : coopmatB_f16_16x16
                    coopmatLoad(b, khr_b, int((trow0 + uint(c) * 16u) * KHR_STRIDE + ko), int(KHR_STRIDE), 1)
                    for [unroll_full] (r in range(4)) {
                        acc[r * 2 + c] = coopmatMulAdd(a[r], b, acc[r * 2 + c])
                    }
                }
            }
            barrier()
            k += 32u
        }
        let tile_whole = m0 + 128u <= pa.d && xt * BN + BN <= cnt
        if (tile_whole) {
            let yb = ybase + (t0 + trow0) * pa.d + m0 + wrow0
            var accf32 : coopmatAcc_f32_16x16
            for [unroll_full] (r in range(4)) {
                for [unroll_full] (c in range(2)) {
                    coopmatConvert(accf32, acc[r * 2 + c])
                    coopmatStore(accf32, y, int(yb + uint(c) * 16u * pa.d + uint(r) * 16u), int(pa.d), 1)
                }
            }
        } else {
            let lane = gl_SubgroupInvocationID
            let ob = sg * 256u
            var accf32 : coopmatAcc_f32_16x16
            for [unroll_full] (r in range(4)) {
                for [unroll_full] (c in range(2)) {
                    coopmatConvert(accf32, acc[r * 2 + c])
                    coopmatStore(accf32, khr_ao, int(ob), 16, 0)
                    barrier()
                    for (e in range(8)) {
                        let idx = lane * 8u + uint(e)
                        let mr = m0 + wrow0 + uint(r) * 16u + idx / 16u
                        let tc = xt * BN + trow0 + uint(c) * 16u + idx % 16u
                        if (mr < pa.d && tc < cnt) {
                            y[ybase + (row0 + tc) * pa.d + mr] = uint_bits_to_float(khr_ao[ob + idx])
                        }
                    }
                    barrier()
                }
            }
        }
        }
    }
}

// the format templates: the block struct, the scale element, the block width and the decode
[ |> template_struct_instance]
class template Q8Cm2T : KqCm2BatchT {
    typedef BLK = VkQ8Blk
    typedef ST = float16
    override BLKW = 32u

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkQ8Blk; bc, cib : uint2) : float16 {
        let q = unpack8(blk.qs[int((cib.y & 30u) >> 1u)])[int(cib.y & 1u)]
        return ws[wg_blk0 + bc.x * (pa.n >> 5u) + bc.y] * float16(float(int(q)))
    }

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let w4 = wq4[blk * 2u + (e0 >> 4u)]   // the block's low or high 16 weights, four words of int8
            let d = float(ws[blk])
            for [unroll_full] (c in range(4)) {
                let q = float4(int4(unpack8(int(w4[c])))) * d
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(q.xy)
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(q.zw)
            }
        }
    }
}

[ |> template_struct_instance]
class template K4Cm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk
    typedef ST = uint
    override DECV4 = true
    override SCACHE = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let sh = ((e0 >> 4u) & 1u) * 4u   //! the low or the high nibbles of sub-block g's 16 bytes
            let w4 = wq4[blk * 8u + g]
            let srow = blk * 5u
            let dm = unpackHalf2x16(ws[srow])
            let ds = dm.x * float((ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu)
            let dmn = dm.y * float((ws[srow + 3u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu)
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> sh
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(ds * float(w & 0xFu) - dmn, ds * float((w >> 8u) & 0xFu) - dmn))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(ds * float((w >> 16u) & 0xFu) - dmn, ds * float((w >> 24u) & 0xFu) - dmn))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let by = uint(int(unpack8(blk.qs[int(bidx >> 1u)])[int(bidx & 1u)])) & 0xFFu
        let q = (by >> ((e >> 4u) * 4u)) & 0xFu
        let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
        return float16(sc_pair.x * float(q) - sc_pair.y)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK4Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let li = int((g * 16u + (e & 15u)) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
        return half4(float4(sc_pair.x * float((l0 >> sh) & 0xFu) - sc_pair.y, sc_pair.x * float((l0 >> (sh + 8u)) & 0xFu) - sc_pair.y,
            sc_pair.x * float((l1 >> sh) & 0xFu) - sc_pair.y, sc_pair.x * float((l1 >> (sh + 8u)) & 0xFu) - sc_pair.y))
    }
}

[ |> template_struct_instance]
class template K6Cm2T : KqCm2BatchT {
    typedef BLK = VkK6Blk
    typedef ST = uint
    override DECV4 = true
    override SCACHE = true
    override SCK6 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let bu = e0 >> 5u
            let hh = (e0 >> 4u) & 1u
            let lw4 = wq4[blk * 12u + bu]                            //! the low-nibble bytes bu * 16 + j
            let hw4 = wq4[blk * 12u + 8u + (bu >> 2u) * 2u + hh]   //! the high-2-bit bytes (bu >> 2) * 32 + hh * 16 + j
            let ls = hh * 4u
            let hs = (bu & 3u) * 2u
            let srow = blk * 5u
            let sidx = e0 >> 4u
            let sc = int(ws[srow + (sidx >> 2u)] << ((3u - (sidx & 3u)) * 8u)) >> 24
            let ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
            for [unroll_full] (c in range(4)) {
                let lw = lw4[c] >> ls
                let hw = hw4[c] >> hs
                let q0 = int((lw & 0xFu) | ((hw & 3u) << 4u)) - 32
                let q1 = int(((lw >> 8u) & 0xFu) | (((hw >> 8u) & 3u) << 4u)) - 32
                let q2 = int(((lw >> 16u) & 0xFu) | (((hw >> 16u) & 3u) << 4u)) - 32
                let q3 = int(((lw >> 24u) & 0xFu) | (((hw >> 24u) & 3u) << 4u)) - 32
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(ds * float(q0), ds * float(q1)))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(ds * float(q2), ds * float(q3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK6Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u   //! the pair's two low bytes are one 16-bit lane, its high bits two of one byte
        let bu = pair0 >> 5u
        let hh = (pair0 >> 4u) & 1u
        let j = pair0 & 15u
        let lb = bu * 16u + j
        let lw = uint(int(blk.ql[int(lb >> 1u)])) & 0xFFFFu
        let qb = (bu >> 2u) * 32u + hh * 16u + j
        let hw = uint(int(blk.qh[int(qb >> 1u)])) & 0xFFFFu
        //! both bytes of the pair compose under one mask (the low nibbles, the two high bits shifted up), and the element's byte is selected last
        let q2 = ((lw >> (hh * 4u)) & 0x0F0Fu) | (((hw >> ((bu & 3u) * 2u)) & 0x0303u) << 4u)
        let q6 = int((q2 >> ((e & 1u) * 8u)) & 0xFFu) - 32
        let sidx = pair0 >> 4u
        var ds : float
        //! d x the sub-block's int8 scale: staged per superblock under the cache, else the row's words - byte `sidx` of words 0-3, the f16 d in word 4
        static_if (SCK6) {
            ds = sc6_cache[sidx * SC_STRIDE + (bc.x - wg_m0)]
        } else {
            let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
            let sc = int(ws[srow + (sidx >> 2u)] << ((3u - (sidx & 3u)) * 8u)) >> 24
            ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
        }
        return float16(ds * float(q6))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK6Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let bu = e >> 5u
        let hh = (e >> 4u) & 1u
        let j = e & 15u
        let li = int((bu * 16u + j) >> 1u)
        let l0 = uint(int(blk.ql[li])) & 0xFFFFu
        let l1 = uint(int(blk.ql[li + 1])) & 0xFFFFu
        let hi = int(((bu >> 2u) * 32u + hh * 16u + j) >> 1u)
        let h0 = uint(int(blk.qh[hi])) & 0xFFFFu
        let h1 = uint(int(blk.qh[hi + 1])) & 0xFFFFu
        let ls = hh * 4u
        let hs = (bu & 3u) * 2u
        let p0 = ((l0 >> ls) & 0x0F0Fu) | (((h0 >> hs) & 0x0303u) << 4u)   //! the two pairs compose under one mask each
        let p1 = ((l1 >> ls) & 0x0F0Fu) | (((h1 >> hs) & 0x0303u) << 4u)
        let sidx = e >> 4u
        var ds : float
        static_if (SCK6) {
            ds = sc6_cache[sidx * SC_STRIDE + (bc.x - wg_m0)]
        } else {
            let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
            let sc = int(ws[srow + (sidx >> 2u)] << ((3u - (sidx & 3u)) * 8u)) >> 24
            ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
        }
        return half4(float4(ds * float(int(p0 & 0xFFu) - 32), ds * float(int(p0 >> 8u) - 32), ds * float(int(p1 & 0xFFu) - 32), ds * float(int(p1 >> 8u) - 32)))
    }
}

[ |> template_struct_instance]
class template K5Cm2T : KqCm2BatchT {
    typedef BLK = VkK5Blk
    typedef ST = uint
    override DECV4 = true
    override SCACHE = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let h = (e0 >> 4u) & 1u
            let sh = h * 4u
            let w4 = wq4[blk * 10u + g]
            let hw = wq4[blk * 10u + 8u + (g >> 2u)][int(g & 3u)] >> sh   //! sub-block g's four 5th-bit bytes; byte c holds j = 4c .. 4c + 3 at bit (j & 3) + h * 4
            let srow = blk * 5u
            let dm = unpackHalf2x16(ws[srow])
            let ds = dm.x * float((ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu)
            let dmn = dm.y * float((ws[srow + 3u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu)
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> sh
                let hb = hw >> (uint(c) * 8u)
                let q0 = (w & 0xFu) | ((hb & 1u) << 4u)
                let q1 = ((w >> 8u) & 0xFu) | (((hb >> 1u) & 1u) << 4u)
                let q2 = ((w >> 16u) & 0xFu) | (((hb >> 2u) & 1u) << 4u)
                let q3 = ((w >> 24u) & 0xFu) | (((hb >> 3u) & 1u) << 4u)
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(ds * float(q0) - dmn, ds * float(q1) - dmn))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(ds * float(q2) - dmn, ds * float(q3) - dmn))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK5Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let pair0 = e & ~1u   //! the pair's two quant bytes are one 16-bit lane, its high bits two of one byte
        let j = pair0 & 15u
        let h = pair0 >> 4u
        let bidx = g * 16u + j
        let w = uint(int(blk.qs[int(bidx >> 1u)])) & 0xFFFFu
        let hidx = g * 4u + (j >> 2u)
        let hb = ((uint(int(blk.qh[int(hidx >> 1u)])) & 0xFFFFu) >> ((hidx & 1u) * 8u)) >> ((j & 3u) + h * 4u)
        let q0 = ((w >> (h * 4u)) & 0xFu) | ((hb & 1u) << 4u)
        let q1 = ((w >> (h * 4u + 8u)) & 0xFu) | (((hb >> 1u) & 1u) << 4u)
        let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
        return float16((e & 1u) != 0u ? sc_pair.x * float(q1) - sc_pair.y : sc_pair.x * float(q0) - sc_pair.y)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK5Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let j = e & 15u
        let li = int((g * 16u + j) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let hidx = g * 4u + (j >> 2u)
        let hby = uint(int(unpack8(blk.qh[int(hidx >> 1u)])[int(hidx & 1u)])) & 0xFFu
        let sh = (e >> 4u) * 4u
        let hb = hby >> sh
        let q0 = ((l0 >> sh) & 0xFu) | ((hb & 1u) << 4u)
        let q1 = ((l0 >> (sh + 8u)) & 0xFu) | (((hb >> 1u) & 1u) << 4u)
        let q2 = ((l1 >> sh) & 0xFu) | (((hb >> 2u) & 1u) << 4u)
        let q3 = ((l1 >> (sh + 8u)) & 0xFu) | (((hb >> 3u) & 1u) << 4u)
        let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
        return half4(float4(sc_pair.x * float(q0) - sc_pair.y, sc_pair.x * float(q1) - sc_pair.y, sc_pair.x * float(q2) - sc_pair.y, sc_pair.x * float(q3) - sc_pair.y))
    }
}

[ |> template_struct_instance]
class template Q40Cm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk   // the same 128-nibble lane view - q40 rows land in the k/k+16 device pairing too
    typedef ST = uint
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let sh = ((e0 >> 4u) & 1u) * 4u
            let w4 = wq4[blk * 8u + g]
            let dp = unpackHalf2x16(ws[blk * 5u + (g >> 1u)])
            let d = (g & 1u) == 0u ? dp.x : dp.y
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> sh
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(d * (float(w & 0xFu) - 8.0), d * (float((w >> 8u) & 0xFu) - 8.0)))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(d * (float((w >> 16u) & 0xFu) - 8.0), d * (float((w >> 24u) & 0xFu) - 8.0)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
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

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK4Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let li = int((g * 16u + (e & 15u)) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dp = unpackHalf2x16(ws[srow + (g >> 1u)])
        let d = (g & 1u) == 0u ? dp.x : dp.y
        return half4(float4(d * (float((l0 >> sh) & 0xFu) - 8.0), d * (float((l0 >> (sh + 8u)) & 0xFu) - 8.0),
            d * (float((l1 >> sh) & 0xFu) - 8.0), d * (float((l1 >> (sh + 8u)) & 0xFu) - 8.0)))
    }
}

//! Q5_1 on the cm2 tile: the per-32 plane verbatim (the CPU's `dot_q51q8_scalar` layout), w = d * q + m with
//! q = nibble | qh bit << 4; the scale word is f16 d in the low half, f16 m in the high
[ |> template_struct_instance]
class template Q51Cm2T : KqCm2BatchT {
    typedef BLK = VkQ51Blk
    typedef ST = uint
    override BLKW = 32u
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {   //! the 20-byte block off its words: four nibble words then the qh word, the low or high nibbles by e0
            let sh = (e0 >> 4u) * 4u
            let dm = unpackHalf2x16(ws[blk])
            let qh = wqw[blk * 5u + 4u] >> e0
            for [unroll_full] (c in range(4)) {
                let w = wqw[blk * 5u + uint(c)] >> sh
                let hb = qh >> (uint(c) * 4u)
                let q0 = float((w & 0xFu) | ((hb & 1u) << 4u))
                let q1 = float(((w >> 8u) & 0xFu) | (((hb >> 1u) & 1u) << 4u))
                let q2 = float(((w >> 16u) & 0xFu) | (((hb >> 2u) & 1u) << 4u))
                let q3 = float(((w >> 24u) & 0xFu) | (((hb >> 3u) & 1u) << 4u))
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(dm.x * q0 + dm.y, dm.x * q1 + dm.y))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(dm.x * q2 + dm.y, dm.x * q3 + dm.y))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkQ51Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let j = e & 15u
        let by = ((uint(int(blk.qs[int(j >> 1u)])) & 0xFFFFu) >> ((j & 1u) * 8u)) & 0xFFu
        let qh = (uint(int(blk.qs[8])) & 0xFFFFu) | ((uint(int(blk.qs[9])) & 0xFFFFu) << 16u)
        let q = ((by >> ((e >> 4u) * 4u)) & 0xFu) | (((qh >> e) & 1u) << 4u)
        let dm = unpackHalf2x16(ws[wg_blk0 + bc.x * (pa.n >> 5u) + bc.y])
        return float16(dm.x * float(q) + dm.y)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkQ51Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let li = int((e & 15u) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let qh = ((uint(int(blk.qs[8])) & 0xFFFFu) | ((uint(int(blk.qs[9])) & 0xFFFFu) << 16u)) >> e
        let dm = unpackHalf2x16(ws[wg_blk0 + bc.x * (pa.n >> 5u) + bc.y])
        return half4(float4(dm.x * float(((l0 >> sh) & 0xFu) | ((qh & 1u) << 4u)) + dm.y,
            dm.x * float(((l0 >> (sh + 8u)) & 0xFu) | (((qh >> 1u) & 1u) << 4u)) + dm.y,
            dm.x * float(((l1 >> sh) & 0xFu) | (((qh >> 2u) & 1u) << 4u)) + dm.y,
            dm.x * float(((l1 >> (sh + 8u)) & 0xFu) | (((qh >> 3u) & 1u) << 4u)) + dm.y))
    }
}

[ |> template_struct_instance]
class template Mx4Cm2T : KqCm2BatchT {
    typedef BLK = VkMx4Blk
    typedef ST = uint
    override BLKW = 32u
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {   //! the block's 16 nibble bytes as one word4: the low nibbles are elements 0..15, the high 16..31; `decode_v4`'s packed-byte arithmetic
            let sh = (e0 >> 4u) * 4u
            let w4 = wq4[blk]
            let ex = (ws[blk >> 2u] >> ((blk & 3u) * 8u)) & 0xFFu
            let sc = mx4_e8m0_half(ex)
            for [unroll_full] (c in range(4)) {
                let v = mx4_dec4((w4[c] >> sh) & 0x0F0F0F0Fu, sc)
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(v.x, v.y))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(v.z, v.w))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def decode(blk : VkMx4Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let j = e & 15u
        let by = ((uint(int(blk.qs[int(j >> 1u)])) & 0xFFFFu) >> ((j & 1u) * 8u)) & 0xFFu
        let nib = (by >> ((e >> 4u) * 4u)) & 0xFu
        let mag = nib & 7u
        let mag2 = mag < 4u ? mag : ((4u + ((mag & 1u) << 1u)) << (((mag >> 1u) - 2u) & 1u))
        let idx = wg_blk0 + bc.x * (pa.n >> 5u) + bc.y
        let ex = (ws[idx >> 2u] >> ((idx & 3u) * 8u)) & 0xFFu
        let v = float(mag2) * mx4_e8m0_half(ex)
        return float16((nib & 8u) != 0u ? -v : v)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def decode_v4(blk : VkMx4Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let li = int((e & 15u) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let nw = ((l0 >> sh) & 0xFu) | (((l0 >> (sh + 8u)) & 0xFu) << 8u) | (((l1 >> sh) & 0xFu) << 16u) | (((l1 >> (sh + 8u)) & 0xFu) << 24u)
        let idx = wg_blk0 + bc.x * (pa.n >> 5u) + bc.y
        let ex = (ws[idx >> 2u] >> ((idx & 3u) * 8u)) & 0xFFu
        return half4(mx4_dec4(nw, mx4_e8m0_half(ex)))
    }
}

//! the halved E8M0 scale of an MXFP4 block from its exponent byte: 2^(e - 128), the bit patterns below exponent 2 the denormals
def mx4_e8m0_half(ex : uint) : float => uint_bits_to_float(ex < 2u ? (0x00200000u << ex) : ((ex - 1u) << 23u))

//! the DOUBLED e2m1 magnitudes {0, 1, 2, 3, 4, 6, 8, 12} of four nibble codes, one per byte of `nib`, built without a table
def mx4_mag2x4(nib : uint) : uint {
    let mag = nib & 0x07070707u
    let lo2 = mag & 0x03030303u
    let hi1 = (mag >> 2u) & 0x01010101u
    return lo2 + ((lo2 + 0x04040404u + (((lo2 & (lo2 >> 1u)) & 0x01010101u) << 1u)) & (hi1 * 0xFFu))
}

//! four MXFP4 values off four nibble codes (one per byte of `nib`) under the halved scale `sc`
def mx4_dec4(nib : uint; sc : float) : float4 {
    let mag2 = mx4_mag2x4(nib)
    let sgn = (nib >> 3u) & 0x01010101u
    return float4(float(mag2 & 0xFFu) * (1.0 - 2.0 * float(sgn & 1u)) * sc,
        float((mag2 >> 8u) & 0xFFu) * (1.0 - 2.0 * float((sgn >> 8u) & 1u)) * sc,
        float((mag2 >> 16u) & 0xFFu) * (1.0 - 2.0 * float((sgn >> 16u) & 1u)) * sc,
        float(mag2 >> 24u) * (1.0 - 2.0 * float(sgn >> 24u)) * sc)
}

[ |> template_struct_instance]
class template Iq4xsCm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk   // the iq4xs plane is the k/k+16 pairing verbatim - the k4 lane view fits
    typedef ST = uint
    override IQLUT = true
    override DECV4 = true
    override SCACHE = true
    override SCIQ4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let sh = ((e0 >> 4u) & 1u) * 4u
            let w4 = wq4[blk * 8u + g]
            let srow = blk * 2u
            let w0 = ws[srow]
            let ds = unpackHalf2x16(w0).x * float(iq4xs_sc(w0, ws[srow + 1u], g))
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> sh
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(ds * float(iq4lut[int(w & 0xFu)]), ds * float(iq4lut[int((w >> 8u) & 0xFu)])))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(ds * float(iq4lut[int((w >> 16u) & 0xFu)]), ds * float(iq4lut[int((w >> 24u) & 0xFu)])))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let by = uint(int(unpack8(blk.qs[int(bidx >> 1u)])[int(bidx & 1u)])) & 0xFFu
        let q = (by >> ((e >> 4u) * 4u)) & 0xFu
        let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
        return float16(sc_pair.x * float(iq4lut[int(q)]))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK4Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let li = int((g * 16u + (e & 15u)) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let ds = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)].x
        return half4(float4(ds * float(iq4lut[int((l0 >> sh) & 0xFu)]), ds * float(iq4lut[int((l0 >> (sh + 8u)) & 0xFu)]),
            ds * float(iq4lut[int((l1 >> sh) & 0xFu)]), ds * float(iq4lut[int((l1 >> (sh + 8u)) & 0xFu)])))
    }
}

[ |> template_struct_instance]
class template K3Cm2T : KqCm2BatchT {
    typedef BLK = VkK3Blk
    typedef ST = uint
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let l0 = e0 & 31u
            let qw4 = wq4[blk * 6u + (g >> 2u) * 2u + (l0 >> 4u)]   //! the qs bytes (g >> 2) * 32 + l
            let hw4 = wq4[blk * 6u + 4u + (l0 >> 4u)]               //! the hmask bytes l
            let qs2 = (g & 3u) * 2u
            let srow = blk * 5u
            let sidx = e0 >> 4u
            let sc = int(ws[srow + (sidx >> 2u)] << ((3u - (sidx & 3u)) * 8u)) >> 24
            let ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
            for [unroll_full] (c in range(4)) {
                let qw = qw4[c] >> qs2
                let hw = hw4[c] >> g
                let q0 = int(qw & 3u) - ((hw & 1u) == 0u ? 4 : 0)
                let q1 = int((qw >> 8u) & 3u) - (((hw >> 8u) & 1u) == 0u ? 4 : 0)
                let q2 = int((qw >> 16u) & 3u) - (((hw >> 16u) & 1u) == 0u ? 4 : 0)
                let q3 = int((qw >> 24u) & 3u) - (((hw >> 24u) & 1u) == 0u ? 4 : 0)
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(ds * float(q0), ds * float(q1)))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(ds * float(q2), ds * float(q3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
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

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK3Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let l = e & 31u
        let qi = int(((g >> 2u) * 32u + l) >> 1u)
        let q0w = uint(int(blk.qs[qi])) & 0xFFFFu
        let q1w = uint(int(blk.qs[qi + 1])) & 0xFFFFu
        let hi = int(l >> 1u)
        let h0w = uint(int(blk.hm[hi])) & 0xFFFFu
        let h1w = uint(int(blk.hm[hi + 1])) & 0xFFFFu
        let qs2 = (g & 3u) * 2u
        let q3_0 = int((q0w >> qs2) & 3u) - (((h0w >> g) & 1u) == 0u ? 4 : 0)
        let q3_1 = int((q0w >> (qs2 + 8u)) & 3u) - (((h0w >> (g + 8u)) & 1u) == 0u ? 4 : 0)
        let q3_2 = int((q1w >> qs2) & 3u) - (((h1w >> g) & 1u) == 0u ? 4 : 0)
        let q3_3 = int((q1w >> (qs2 + 8u)) & 3u) - (((h1w >> (g + 8u)) & 1u) == 0u ? 4 : 0)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let sidx = e >> 4u
        let sc = int(ws[srow + (sidx >> 2u)] << ((3u - (sidx & 3u)) * 8u)) >> 24
        let ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
        return half4(float4(ds * float(q3_0), ds * float(q3_1), ds * float(q3_2), ds * float(q3_3)))
    }
}

[ |> template_struct_instance]
class template K2Cm2T : KqCm2BatchT {
    typedef BLK = VkK2Blk
    typedef ST = uint
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 4u
            let qw4 = wq4[blk * 4u + (g >> 3u) * 2u + (g & 1u)]   //! the qs bytes (g >> 3) * 32 + (g & 1) * 16 + l
            let qs2 = ((g >> 1u) & 3u) * 2u
            let srow = blk * 5u
            let dm = unpackHalf2x16(ws[srow])
            let pb = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
            let ds = dm.x * float(pb & 15u)
            let dmn = dm.y * float(pb >> 4u)
            for [unroll_full] (c in range(4)) {
                let qw = qw4[c] >> qs2
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(ds * float(qw & 3u) - dmn, ds * float((qw >> 8u) & 3u) - dmn))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(ds * float((qw >> 16u) & 3u) - dmn, ds * float((qw >> 24u) & 3u) - dmn))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
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

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK2Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 4u
        let l = e & 15u
        let qi = int(((g >> 3u) * 32u + (g & 1u) * 16u + l) >> 1u)
        let q0w = uint(int(blk.qs[qi])) & 0xFFFFu
        let q1w = uint(int(blk.qs[qi + 1])) & 0xFFFFu
        let qs2 = ((g >> 1u) & 3u) * 2u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dm = unpackHalf2x16(ws[srow])
        let pb = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
        let ds = dm.x * float(pb & 15u)
        let dmn = dm.y * float(pb >> 4u)
        return half4(float4(ds * float((q0w >> qs2) & 3u) - dmn, ds * float((q0w >> (qs2 + 8u)) & 3u) - dmn,
            ds * float((q1w >> qs2) & 3u) - dmn, ds * float((q1w >> (qs2 + 8u)) & 3u) - dmn))
    }
}

[ |> template_struct_instance]
class template Iq3sCm2T : KqCm2BatchT {
    typedef BLK = VkIq3sBlk
    typedef ST = uint
    override IQ3GRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let r0 = e0 & 31u
            let bw = blk * 26u
            let qsw = wqw[bw + g * 2u + (r0 >> 4u)]                             //! the four grid-index bytes g * 8 + (r >> 2)
            let qh = (wqw[bw + 16u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu   //! the ninth-bit byte g
            let sgw = wqw[bw + 18u + g] >> ((r0 >> 3u) * 8u)                   //! the two sign bytes g * 4 + (r >> 3) in the low half
            let srow = blk * 2u
            let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
            for [unroll_full] (i in range(4)) {
                let wj = (r0 >> 2u) + uint(i)
                let qb = (qsw >> (uint(i) * 8u)) & 0xFFu
                let gw = iq3s_gridc[qb | ((qh << (8u - wj)) & 256u)]
                let sgn = sgw >> ((uint(i) >> 1u) * 8u + (uint(i) & 1u) * 4u)
                let g0 = float(gw & 0xFFu)
                let g1 = float((gw >> 8u) & 0xFFu)
                let g2 = float((gw >> 16u) & 0xFFu)
                let g3 = float(gw >> 24u)
                khr_ao[sbase + uint(i) * 2u] = packHalf2x16(float2(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)))
                khr_ao[sbase + uint(i) * 2u + 1u] = packHalf2x16(float2(ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq3sBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let g = pair0 >> 5u
        let r = pair0 & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu   //! the grid-index byte, shifted out of its lane
        let qhw = (uint(int(blk.qh[int(g >> 1u)])) & 0xFFFFu) >> ((g & 1u) * 8u)   //! the ninth-bit byte g, shifted out of its lane
        let si = g * 4u + (r >> 3u)
        let sgw = (uint(int(blk.sg[int(si >> 1u)])) & 0xFFFFu) >> ((si & 1u) * 8u)   //! the sign byte, shifted out of its lane
        let sgn = sgw >> (((r >> 2u) & 1u) * 4u + (r & 3u))
        let gw = iq3s_gridc[qb | ((qhw << (8u - wj)) & 256u)]
        let gp = gw >> ((r & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
        let g0 = float(gp & 0xFFu)
        let g1 = float((gp >> 8u) & 0xFFu)
        let v0 = ds * ((sgn & 1u) != 0u ? -g0 : g0)
        let v1 = ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)
        return float16((e & 1u) != 0u ? v1 : v0)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq3sBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let qhw = (uint(int(blk.qh[int(g >> 1u)])) & 0xFFFFu) >> ((g & 1u) * 8u)
        let si = g * 4u + (r >> 3u)
        let sgw = (uint(int(blk.sg[int(si >> 1u)])) & 0xFFFFu) >> ((si & 1u) * 8u)
        let gw = iq3s_gridc[qb | ((qhw << (8u - wj)) & 256u)]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let d = unpackHalf2x16(ws[srow]).x
        let sc = grid_sc(ws[srow + 1u], g)
        let ds = d * float(sc)
        let sgn = sgw >> (((r >> 2u) & 1u) * 4u)
        let g0 = float(gw & 0xFFu)
        let g1 = float((gw >> 8u) & 0xFFu)
        let g2 = float((gw >> 16u) & 0xFFu)
        let g3 = float(gw >> 24u)
        return half4(float4(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1),
            ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
    }
}

[ |> template_struct_instance]
class template Iq3xxsCm2T : KqCm2BatchT {
    typedef BLK = VkIq3xxsBlk
    typedef ST = uint
    override IQ3XGRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let r0 = e0 & 31u
            let bw = blk * 24u
            let qsw = wqw[bw + g * 2u + (r0 >> 4u)]   //! the four grid-index bytes g * 8 + (r >> 2)
            let aux = wqw[bw + 16u + g]               //! sub-block g's aux32: four 7-bit ksigns indices and the ls nibble
            let srow = blk * 2u
            let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
            for [unroll_full] (i in range(4)) {
                let l = (r0 >> 3u) + (uint(i) >> 1u)
                let sidx = (aux >> (7u * l)) & 127u
                var tt = sidx ^ (sidx >> 4u)
                tt = tt ^ (tt >> 2u)
                tt = tt ^ (tt >> 1u)
                let sgn = (sidx | ((tt & 1u) << 7u)) >> ((uint(i) & 1u) * 4u)
                let gw = iq3x_gridc[(qsw >> (uint(i) * 8u)) & 0xFFu]
                let g0 = float(gw & 0xFFu)
                let g1 = float((gw >> 8u) & 0xFFu)
                let g2 = float((gw >> 16u) & 0xFFu)
                let g3 = float(gw >> 24u)
                khr_ao[sbase + uint(i) * 2u] = packHalf2x16(float2(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)))
                khr_ao[sbase + uint(i) * 2u + 1u] = packHalf2x16(float2(ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq3xxsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let g = pair0 >> 5u
        let r = pair0 & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let l = r >> 3u
        let ai = int(g * 2u)   //! sub-block g's aux32 as its two 16-bit lanes: four 7-bit ksigns indices
        let aux = (uint(int(blk.ax[ai])) & 0xFFFFu) | ((uint(int(blk.ax[ai + 1])) & 0xFFFFu) << 16u)
        let sidx = (aux >> (7u * l)) & 127u
        var tt = sidx ^ (sidx >> 4u)
        tt = tt ^ (tt >> 2u)
        tt = tt ^ (tt >> 1u)
        let sgn = (sidx | ((tt & 1u) << 7u)) >> (r & 7u)
        let gw = iq3x_gridc[qb]
        let gp = gw >> ((r & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
        let g0 = float(gp & 0xFFu)
        let g1 = float((gp >> 8u) & 0xFFu)
        let v0 = ds * ((sgn & 1u) != 0u ? -g0 : g0)
        let v1 = ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)
        return float16((e & 1u) != 0u ? v1 : v0)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq3xxsBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let l = r >> 3u
        let ai = int(g * 2u)
        let aux = (uint(int(blk.ax[ai])) & 0xFFFFu) | ((uint(int(blk.ax[ai + 1])) & 0xFFFFu) << 16u)
        let sidx = (aux >> (7u * l)) & 127u
        var tt = sidx ^ (sidx >> 4u)
        tt = tt ^ (tt >> 2u)
        tt = tt ^ (tt >> 1u)
        let sgb = sidx | ((tt & 1u) << 7u)
        let gw = iq3x_gridc[qb]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let d = unpackHalf2x16(ws[srow]).x
        let sc = grid_sc(ws[srow + 1u], g)
        let ds = d * float(sc)
        let sgn = sgb >> (r & 7u)
        let g0 = float(gw & 0xFFu)
        let g1 = float((gw >> 8u) & 0xFFu)
        let g2 = float((gw >> 16u) & 0xFFu)
        let g3 = float(gw >> 24u)
        return half4(float4(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1),
            ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
    }
}

[ |> template_struct_instance]
class template Iq4nlCm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk   // q40's nibble plane verbatim - the k4 lane view fits
    typedef ST = uint
    override IQLUT = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let sh = ((e0 >> 4u) & 1u) * 4u
            let w4 = wq4[blk * 8u + g]
            let dp = unpackHalf2x16(ws[blk * 5u + (g >> 1u)])
            let d = (g & 1u) == 0u ? dp.x : dp.y
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> sh
                khr_ao[sbase + uint(c) * 2u] = packHalf2x16(float2(d * float(iq4lut[int(w & 0xFu)]), d * float(iq4lut[int((w >> 8u) & 0xFu)])))
                khr_ao[sbase + uint(c) * 2u + 1u] = packHalf2x16(float2(d * float(iq4lut[int((w >> 16u) & 0xFu)]), d * float(iq4lut[int((w >> 24u) & 0xFu)])))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
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

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK4Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let li = int((g * 16u + (e & 15u)) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dp = unpackHalf2x16(ws[srow + (g >> 1u)])
        let d = (g & 1u) == 0u ? dp.x : dp.y
        return half4(float4(d * float(iq4lut[int((l0 >> sh) & 0xFu)]), d * float(iq4lut[int((l0 >> (sh + 8u)) & 0xFu)]),
            d * float(iq4lut[int((l1 >> sh) & 0xFu)]), d * float(iq4lut[int((l1 >> (sh + 8u)) & 0xFu)])))
    }
}

[ |> template_struct_instance]
class template Iq2sCm2T : KqCm2BatchT {
    typedef BLK = VkIq2sBlk
    typedef ST = uint
    override IQ2SGRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let r0 = e0 & 31u
            let l0 = r0 >> 3u
            let bw = blk * 18u
            let qsw = wqw[bw + g] >> (l0 * 8u)                                //! the grid-index bytes g * 4 + l, this run's two in the low half
            let sgw = wqw[bw + 8u + g] >> (l0 * 8u)                           //! the sign bytes 32 + g * 4 + l
            let qh = (wqw[bw + 16u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu   //! the qh byte g
            let srow = blk * 5u
            let si = g * 2u + (r0 >> 4u)
            let ds = unpackHalf2x16(ws[srow]).x * float((ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu)
            for [unroll_full] (i in range(4)) {
                let li = uint(i) >> 1u
                let l = l0 + li
                let qb = (qsw >> (li * 8u)) & 0xFFu
                let sgn = (sgw >> (li * 8u + (uint(i) & 1u) * 4u)) & 0xFu
                let gw = iq2s_gridc[(qb | ((qh << (8u - 2u * l)) & 0x300u)) * 2u + (uint(i) & 1u)]
                let g0 = float(gw & 0xFFu)
                let g1 = float((gw >> 8u) & 0xFFu)
                let g2 = float((gw >> 16u) & 0xFFu)
                let g3 = float(gw >> 24u)
                khr_ao[sbase + uint(i) * 2u] = packHalf2x16(float2(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)))
                khr_ao[sbase + uint(i) * 2u + 1u] = packHalf2x16(float2(ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2sBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let g = pair0 >> 5u
        let r = pair0 & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 4u + l
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let qhw = (uint(int(blk.qh[int(g >> 1u)])) & 0xFFFFu) >> ((g & 1u) * 8u)
        let sgn = ((uint(int(blk.sg[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) >> j
        let gw = iq2s_gridc[(qb | ((qhw << (8u - 2u * l)) & 0x300u)) * 2u + (j >> 2u)]
        let gp = gw >> ((j & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let si = g * 2u + (r >> 4u)
        let ds = unpackHalf2x16(ws[srow]).x * float((ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu)
        let g0 = float(gp & 0xFFu)
        let g1 = float((gp >> 8u) & 0xFFu)
        let v0 = ds * ((sgn & 1u) != 0u ? -g0 : g0)
        let v1 = ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)
        return float16((e & 1u) != 0u ? v1 : v0)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq2sBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 4u + l
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let qhw = (uint(int(blk.qh[int(g >> 1u)])) & 0xFFFFu) >> ((g & 1u) * 8u)
        let sgw = (uint(int(blk.sg[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)
        let gw = iq2s_gridc[(qb | ((qhw << (8u - 2u * l)) & 0x300u)) * 2u + (j >> 2u)]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let d = unpackHalf2x16(ws[srow]).x
        let si = g * 2u + (r >> 4u)
        let sc = (ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu
        let ds = d * float(sc)
        let sgn = sgw >> j
        let g0 = float(gw & 0xFFu)
        let g1 = float((gw >> 8u) & 0xFFu)
        let g2 = float((gw >> 16u) & 0xFFu)
        let g3 = float(gw >> 24u)
        return half4(float4(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1),
            ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
    }
}

class template Iq2xsCm2T : KqCm2BatchT {
    typedef BLK = VkIq2xsBlk
    typedef ST = uint
    override IQ2XSGRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let qw = wqw[blk * 16u + (e0 >> 4u)]   //! two u16 qs words: the run's 8-value windows
            let srow = blk * 5u
            let si = e0 >> 4u
            let ds = unpackHalf2x16(ws[srow]).x * float((ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu)
            for [unroll_full] (i in range(4)) {
                let w16 = (qw >> ((uint(i) >> 1u) * 16u)) & 0xFFFFu
                let sidx = w16 >> 9u
                var tt = sidx ^ (sidx >> 4u)
                tt = tt ^ (tt >> 2u)
                tt = tt ^ (tt >> 1u)
                let sgn = (sidx | ((tt & 1u) << 7u)) >> ((uint(i) & 1u) * 4u)
                let gw = iq2xs_gridc[(w16 & 511u) * 2u + (uint(i) & 1u)]
                let g0 = float(gw & 0xFFu)
                let g1 = float((gw >> 8u) & 0xFFu)
                let g2 = float((gw >> 16u) & 0xFFu)
                let g3 = float(gw >> 24u)
                khr_ao[sbase + uint(i) * 2u] = packHalf2x16(float2(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)))
                khr_ao[sbase + uint(i) * 2u + 1u] = packHalf2x16(float2(ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2xsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let wi = pair0 >> 3u
        let j = pair0 & 7u
        let w16 = uint(int(blk.qs[int(wi)])) & 0xFFFFu
        let sidx = w16 >> 9u
        var tt = sidx ^ (sidx >> 4u)
        tt = tt ^ (tt >> 2u)
        tt = tt ^ (tt >> 1u)
        let sgn = (sidx | ((tt & 1u) << 7u)) >> j
        let gw = iq2xs_gridc[(w16 & 511u) * 2u + (j >> 2u)]
        let gp = gw >> ((j & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let si = pair0 >> 4u
        let ds = unpackHalf2x16(ws[srow]).x * float((ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu)
        let g0 = float(gp & 0xFFu)
        let g1 = float((gp >> 8u) & 0xFFu)
        let v0 = ds * ((sgn & 1u) != 0u ? -g0 : g0)
        let v1 = ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)
        return float16((e & 1u) != 0u ? v1 : v0)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq2xsBlk; bc, cib : uint2) : half4 {
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
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dv = unpackHalf2x16(ws[srow]).x
        let si = e >> 4u
        let sc = (ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu
        let ds = dv * float(sc)
        let sgn = sgb >> j
        let g0 = float(gw & 0xFFu)
        let g1 = float((gw >> 8u) & 0xFFu)
        let g2 = float((gw >> 16u) & 0xFFu)
        let g3 = float(gw >> 24u)
        return half4(float4(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1),
            ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
    }
}

class template Iq2xxsCm2T : KqCm2BatchT {
    typedef BLK = VkIq2xxsBlk
    typedef ST = uint
    override IQ2XXSGRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let l0 = (e0 & 31u) >> 3u
            let bw = blk * 16u
            let qsw = wqw[bw + g * 2u] >> (l0 * 8u)   //! sub-block g's four grid-index bytes, this run's two in the low half
            let aux = wqw[bw + g * 2u + 1u]           //! its aux32: four 7-bit ksigns indices and the ls nibble
            let srow = blk * 2u
            let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
            for [unroll_full] (i in range(4)) {
                let li = uint(i) >> 1u
                let l = l0 + li
                let qb = (qsw >> (li * 8u)) & 0xFFu
                let sidx = (aux >> (7u * l)) & 127u
                var tt = sidx ^ (sidx >> 4u)
                tt = tt ^ (tt >> 2u)
                tt = tt ^ (tt >> 1u)
                let sgn = (sidx | ((tt & 1u) << 7u)) >> ((uint(i) & 1u) * 4u)
                let gw = iq2xxs_gridc[qb * 2u + (uint(i) & 1u)]
                let g0 = float(gw & 0xFFu)
                let g1 = float((gw >> 8u) & 0xFFu)
                let g2 = float((gw >> 16u) & 0xFFu)
                let g3 = float(gw >> 24u)
                khr_ao[sbase + uint(i) * 2u] = packHalf2x16(float2(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)))
                khr_ao[sbase + uint(i) * 2u + 1u] = packHalf2x16(float2(ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2xxsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let g = pair0 >> 5u
        let r = pair0 & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 8u + l
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let ai = int(g * 4u + 2u)   //! sub-block g's aux32 as its two 16-bit lanes: four 7-bit ksigns indices
        let aux = (uint(int(blk.qs[ai])) & 0xFFFFu) | ((uint(int(blk.qs[ai + 1])) & 0xFFFFu) << 16u)
        let sidx = (aux >> (7u * l)) & 127u
        var tt = sidx ^ (sidx >> 4u)
        tt = tt ^ (tt >> 2u)
        tt = tt ^ (tt >> 1u)
        let sgn = (sidx | ((tt & 1u) << 7u)) >> j
        let gw = iq2xxs_gridc[qb * 2u + (j >> 2u)]
        let gp = gw >> ((j & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
        let g0 = float(gp & 0xFFu)
        let g1 = float((gp >> 8u) & 0xFFu)
        let v0 = ds * ((sgn & 1u) != 0u ? -g0 : g0)
        let v1 = ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)
        return float16((e & 1u) != 0u ? v1 : v0)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq2xxsBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 8u + l
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let ai = int(g * 4u + 2u)
        let aux = (uint(int(blk.qs[ai])) & 0xFFFFu) | ((uint(int(blk.qs[ai + 1])) & 0xFFFFu) << 16u)
        let sidx = (aux >> (7u * l)) & 127u
        var tt = sidx ^ (sidx >> 4u)
        tt = tt ^ (tt >> 2u)
        tt = tt ^ (tt >> 1u)
        let sgb = sidx | ((tt & 1u) << 7u)
        let gw = iq2xxs_gridc[qb * 2u + (j >> 2u)]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let dv = unpackHalf2x16(ws[srow]).x
        let sc = grid_sc(ws[srow + 1u], g)
        let ds = dv * float(sc)
        let sgn = sgb >> j
        let g0 = float(gw & 0xFFu)
        let g1 = float((gw >> 8u) & 0xFFu)
        let g2 = float((gw >> 16u) & 0xFFu)
        let g3 = float(gw >> 24u)
        return half4(float4(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1),
            ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3)))
    }
}

[vk_dispatch(name = "q8_batch_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2LBatch : Q8Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "q8_batch_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2MBatch : Q8Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "q8_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2SBatch : Q8Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

//! the q51 rail's s column: the expert schedule's 32-row pieces (the routed down planes' one Vulkan home - a dense q51 plane never reaches the tiles)
[vk_dispatch(name = "q51_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q51Cm2SBatch : Q51Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

//! the mx4 rail's s column: the expert schedule's 32-row pieces (gpt-oss's routed stacks; a dense mx4 plane never exists)
[vk_dispatch(name = "mx4_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Mx4Cm2SBatch : Mx4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_k4_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2LBatch : K4Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_k4_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2MBatch : K4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k4_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2SBatch : K4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_k6_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2LBatch : K6Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_k6_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2MBatch : K6Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k6_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2SBatch : K6Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_k5_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2LBatch : K5Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_k5_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2MBatch : K5Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k5_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2SBatch : K5Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_q40_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2LBatch : Q40Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_q40_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2MBatch : Q40Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_q40_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2SBatch : Q40Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2LBatch : Iq4xsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2MBatch : Iq4xsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2SBatch : Iq4xsCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_k3_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2LBatch : K3Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_k3_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2MBatch : K3Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k3_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2SBatch : K3Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_iq3s_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2LBatch : Iq3sCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_iq3s_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2MBatch : Iq3sCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq3s_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2SBatch : Iq3sCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2LBatch : Iq3xxsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2MBatch : Iq3xxsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2SBatch : Iq3xxsCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2LBatch : Iq4nlCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2MBatch : Iq4nlCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2SBatch : Iq4nlCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_k2_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2LBatch : K2Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_k2_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2MBatch : K2Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k2_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2SBatch : K2Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_iq2s_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2LBatch : Iq2sCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_iq2s_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2MBatch : Iq2sCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2s_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2SBatch : Iq2sCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2LBatch : Iq2xsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2MBatch : Iq2xsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2SBatch : Iq2xsCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2LBatch : Iq2xxsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2MBatch : Iq2xxsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2SBatch : Iq2xxsCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32
}

[vk_dispatch(name = "q51_batch_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q51Cm2EBatch : Q51Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "mx4_batch_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Mx4Cm2EBatch : Mx4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq3s_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2EBatch : Iq3sCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2EBatch : Iq3xxsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2s_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2EBatch : Iq2sCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2EBatch : Iq2xsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2EBatch : Iq2xxsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

// the KHR arm of every kq format (KHR, BN 128); the cm2 typedefs belong to the template's uncalled
// body and are never materialized
[vk_dispatch(name = "kq_batch_k4_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4KhrBatch : K4Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "q8_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8KhrBatch : Q8Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "q51_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q51KhrBatch : Q51Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "mx4_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Mx4KhrBatch : Mx4Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k5_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5KhrBatch : K5Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k6_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6KhrBatch : K6Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_q40_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40KhrBatch : Q40Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq4xs_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsKhrBatch : Iq4xsCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k3_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3KhrBatch : K3Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq3s_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sKhrBatch : Iq3sCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq3xxs_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsKhrBatch : Iq3xxsCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq4nl_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlKhrBatch : Iq4nlCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_k2_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2KhrBatch : K2Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2s_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sKhrBatch : Iq2sCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2xs_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsKhrBatch : Iq2xsCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

[vk_dispatch(name = "kq_batch_iq2xxs_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsKhrBatch : Iq2xxsCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128
}

struct SkRedArgs {
    nelem : uint   //! one partial plane's float count (rows x d): any count - the reduce guards its four-wide tail element by element
    k : uint       // planes
    ybase : uint   // the sum's element base in y (the smalls' beta or alpha rows; 0 for a plane of its own)
}

// ===== the split-k reduce (sums the cm2 partial planes into y) =====

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
            let n1 = e + 1u < pa.nelem
            let n2 = e + 2u < pa.nelem
            let n3 = e + 3u < pa.nelem
            var p = 0u
            while (p < pa.k) {
                let b = p * pa.nelem + e
                s0 += part[b]
                if (n1) {
                    s1 += part[b + 1u]
                }
                if (n2) {
                    s2 += part[b + 2u]
                }
                if (n3) {
                    s3 += part[b + 3u]
                }
                p++
            }
            let o = pa.ybase + e
            y[o] = s0
            if (n1) {
                y[o + 1u] = s1
            }
            if (n2) {
                y[o + 2u] = s2
            }
            if (n3) {
                y[o + 3u] = s3
            }
        }
    }
}

struct F16GemmArgs {
    n : uint        //! K, the row width (elements), a 64-multiple
    d : uint        //! M, the weight rows
    rows : uint     //! N, the positions
    woff : uint     //! the weight rows' element base
    ybase : uint    //! the output's element base: the sum's, or the first partial plane's
    ystride : uint  //! the output row stride (elements) when the GEMM stores whole
    ksplit : uint   //! the k chunk per workgroup; 0 = the whole K, one plane at ybase with ystride
}

//! a small f16 x f16 GEMM on the cm2 128 x 128 tile, y[rows x d] = x[rows x n] w[d x n]^T; a k chunk's partial plane lands at ybase + chunk * rows * d (stride d)
[vk_dispatch(name = "f16_gemm_cm2_cls", grid = "wgs", params = "wgs : int64")]
class F16GemmCm2 {
    @ssbo @binding = 0 @role = "weight" w : array<float16>
    @ssbo @binding = 1 xf16 : array<float16>
    @ssbo @binding = 2 y : array<float>
    @push_constant pa : F16GemmArgs
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256, name = "f16_gemm_cm2_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let xtiles = (pa.rows + 127u) / 128u
        let wtiles = (pa.d + 127u) / 128u
        let chunk = gl_WorkGroupID.x / (xtiles * wtiles)
        let rem = gl_WorkGroupID.x % (xtiles * wtiles)
        let t0 = (rem / wtiles) * 128u
        let m0 = (rem % wtiles) * 128u
        let split = pa.ksplit != 0u
        let k0 = split ? (chunk * pa.ksplit) & ~(F16G_BK - 1u) : 0u
        let k1 = split ? min(k0 + pa.ksplit, pa.n) : pa.n
        let ybase = split ? pa.ybase + chunk * pa.rows * pa.d : pa.ybase
        let ystride = split ? pa.d : pa.ystride
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var tla : tensorLayout2DPad
        tensorLayoutCreate(tla)
        tensorLayoutSetDimension(tla, pa.d, pa.n)
        tensorLayoutSetStride(tla, pa.n, 1u)
        var tlb : tensorLayout2DPad
        tensorLayoutCreate(tlb)
        tensorLayoutSetDimension(tlb, pa.rows, pa.n)
        tensorLayoutSetStride(tlb, pa.n, 1u)
        var tlo : tensorLayout2DPad
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, pa.rows, pa.d)
        tensorLayoutSetStride(tlo, ystride, 1u)
        var a : AT
        var b : BT
        var acc : ACC
        var k = k0
        while (k < k1) {
            coopmatLoadTensor(a, w, pa.woff, tla, m0, 128u, k, F16G_BK)
            coopmatLoadTensor(b, xf16, 0u, tlb, t0, 128u, k, F16G_BK, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += F16G_BK
        }
        coopmatStoreTensor(acc, y, ybase, tlo, t0, 128u, m0, 128u, tv)
    }
}

[vk_dispatch(name = "f16_gemm_khr_cls", grid = "wgs", params = "wgs : int64")]
class F16GemmKhr {
    @ssbo @binding = 0 @role = "weight" w : array<float16>
    @ssbo @binding = 1 xf16 : array<float16>
    @ssbo @binding = 2 y : array<float>
    @push_constant pa : F16GemmArgs

    [spirv_kernel(local_size_x = 128, name = "f16_gemm_khr_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let wtiles = (pa.d + 63u) / 64u
        let tile_row0 = (gl_WorkGroupID.x / wtiles) * 64u
        let tile_col0 = (gl_WorkGroupID.x % wtiles) * 64u
        let sg = gl_LocalInvocationID.x / 32u
        let sg_row0 = tile_row0 + (sg / 2u) * 32u
        let sg_col0 = tile_col0 + (sg % 2u) * 32u
        //! a fragment past `rows` or `d` is neither loaded nor stored: the planes end at their 16-multiples, and the store guard below discards its product
        let a1_on = sg_row0 + 16u < pa.rows
        let b1_on = sg_col0 + 16u < pa.d
        var acc : coopmatAcc_f32_16x16[4]
        var k = 0u
        while (k < pa.n) {
            var a0 : coopmatA_f16_16x16
            var a1 : coopmatA_f16_16x16
            var b0 : coopmatB_f16_16x16
            var b1 : coopmatB_f16_16x16
            if (sg_row0 < pa.rows) {
                coopmatLoad(a0, xf16, sg_row0 * pa.n + k, int(pa.n), 0)
            }
            if (a1_on) {
                coopmatLoad(a1, xf16, (sg_row0 + 16u) * pa.n + k, int(pa.n), 0)
            }
            if (sg_col0 < pa.d) {
                coopmatLoad(b0, w, pa.woff + sg_col0 * pa.n + k, int(pa.n), 1)
            }
            if (b1_on) {
                coopmatLoad(b1, w, pa.woff + (sg_col0 + 16u) * pa.n + k, int(pa.n), 1)
            }
            acc[0] = coopmatMulAdd(a0, b0, acc[0])
            acc[1] = coopmatMulAdd(a0, b1, acc[1])
            acc[2] = coopmatMulAdd(a1, b0, acc[2])
            acc[3] = coopmatMulAdd(a1, b1, acc[3])
            k += 16u
        }
        for [unroll] (f in range(4)) {
            let r = sg_row0 + uint(f / 2) * 16u
            let c = sg_col0 + uint(f % 2) * 16u
            if (r < pa.rows && c < pa.d) {
                coopmatStore(acc[f], y, int(pa.ybase + r * pa.ystride + c), int(pa.ystride), 0)
            }
        }
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def f16_gemm_wgs(khr : bool; rows, d : int64) : int64 {
    let tile = khr ? 64l : 128l
    return ((rows + tile - 1l) / tile) * ((d + tile - 1l) / tile)
}

def f16_gemm_ensure(khr : bool) : bool => khr ? ensure_f16_gemm_khr_cls() : ensure_f16_gemm_cm2_cls()

def f16_gemm_set(khr : bool; bufs : uint64 const[3]; sizes : int64 const[3]; gbits : uint const[3]) : VkDescriptorSet
    => khr ? set_f16_gemm_khr_cls(bufs, sizes, gbits) : set_f16_gemm_cm2_cls(bufs, sizes, gbits)

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def f16_gemm_enc(khr : bool; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : F16GemmArgs; rows, d : int64) {
    let wgs = f16_gemm_wgs(khr, rows, d)
    if (khr) {
        enc_f16_gemm_khr_cls(raw, h, s, pc, wgs)
    } else {
        enc_f16_gemm_cm2_cls(raw, h, s, pc, wgs)
    }
}

// ===== rope + KV store + qk-norm =====

struct QkRmsArgs {
    hs : uint
    nh : uint
    nkvh : uint
    qstride : uint   // q row stride
    kstride : uint
    kbase : uint
    qwoff : uint
    kwoff : uint
    eps : float
    qhs : uint       // per-head q stride: hs, or 2 * hs on a gated model
    vfk : uint       //! the k groups also write the V row: the raw k row scaled by its rms (a layer with no v plane)
    vnorm : uint     //! nkvh more groups norm the V rows in place, weightless (gemma-4's v-norm)
    vbase : uint     //! the V rows' base in krows (kstride apart), read under vnorm and written under vfk
}

// per-head RMSNorm on q/k between the projections and rope (qk_norm models), one wg per
// head-row, in place; a head past 256 (gemma-4's global layers, 512) gives each thread a second
// element at tid + 256. Under vfk the k groups write V beside their norm; under vnorm nkvh
// groups past the k ones scale the V rows by their own rms with no weight.
[vk_dispatch(name = "qk_rms_cls", grid = "wgs", params = "wgs : int64")]
class QkRms : RmsWgBase {
    @ssbo @binding = 0 qrows : array<float>   // q rows, normed in place
    @ssbo @binding = 1 krows : array<float>   // k rows, normed in place (the V rows sit at vbase in the same buffer)
    @ssbo @binding = 2 wn : array<float>      // the q/k norm weight rows
    @push_constant pa : QkRmsArgs

    [spirv_kernel(local_size_x = 256, name = "qk_rms_cls_spv")]
    def run {   // nolint:STYLE037 — one head-row's norm: the q, k and v group selects and the two-element wide arm, one pass
        let vgroups = (pa.vnorm != 0u && pa.vfk == 0u) ? pa.nkvh : 0u
        let per = max(pa.nh + pa.nkvh + vgroups, 1u)
        let wid = gl_WorkGroupID.x
        let r = wid / per
        let hh = wid % per
        let is_q = hh < pa.nh
        let is_v = hh >= pa.nh + pa.nkvh
        let qhs = pa.qhs == 0u ? pa.hs : pa.qhs   // 0 = compact (non-gated); a gated model passes 2 * hs
        let kh = is_v ? hh - pa.nh - pa.nkvh : hh - pa.nh
        let base = is_q ? r * pa.qstride + hh * qhs : (is_v ? pa.vbase + r * pa.kstride + kh * pa.hs : pa.kbase + r * pa.kstride + kh * pa.hs)
        let wbase = is_q ? pa.qwoff : pa.kwoff
        let tid = gl_LocalInvocationID.x
        let t1 = tid + 256u
        let two = t1 < pa.hs
        var v = 0.0
        var v1 = 0.0
        if (tid < pa.hs) {
            v = is_q ? qrows[base + tid] : krows[base + tid]
        }
        if (two) {
            v1 = is_q ? qrows[base + t1] : krows[base + t1]
        }
        let inv = wg_rms_inv(tid < pa.hs ? v * v + v1 * v1 : 0.0, pa.hs, pa.eps, 0u)
        if (tid < pa.hs) {
            let w = is_v ? 1.0 : wn[wbase + tid]
            if (is_q) {
                qrows[base + tid] = w * (v * inv)
            } else {
                krows[base + tid] = w * (v * inv)
            }
        }
        if (two) {
            let w = is_v ? 1.0 : wn[wbase + t1]
            if (is_q) {
                qrows[base + t1] = w * (v1 * inv)
            } else {
                krows[base + t1] = w * (v1 * inv)
            }
        }
        if (pa.vfk != 0u && !is_q) {   // the V row of this k head: the raw k row at its own rms
            let vb = pa.vbase + r * pa.kstride + kh * pa.hs
            if (tid < pa.hs) {
                krows[vb + tid] = v * inv
            }
            if (two) {
                krows[vb + t1] = v1 * inv
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
    layerbase : uint   // this layer's K/V mirror base; the row rides tok[row].pos past tok[row].mirbase
    boff : uint     // this layer's row base in the bias buffer ([q | k | v] rows, one layer after another)
    hasb : uint
    cbase : uint    //! this layer's rope class's row base in the cos/sin plane (a second class's row follows the first's)
    rows : uint     //! rows in the dispatch, `npairs` threads each; 0 = one (the one-row command passes 1: the same reading)
    kvrow : uint    //! ... a row's stride in kvsrc (the projection row's width), in qrows, and in the cos/sin plane
    qrow : uint
    cosrow : uint
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
    @ssbo @binding = 6 @role = "weight" bias : array<float>   // per-layer [q | k | v] projection biases (hasb), else a placeholder
    @push_constant pa : RopeKvArgs
    @template_constant CLAMP16 : bool = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.npairs * max(pa.rows, 1u)) {
            let row = gid / pa.npairs
            let pr = gid % pa.npairs
            let is_k = pr >= pa.kpair0
            let lp = is_k ? pr - pa.kpair0 : pr
            let h = lp / pa.half
            let j = lp % pa.half
            let cb = row * pa.cosrow + pa.cbase
            let fcr = cossin[cb + j]
            let fci = cossin[cb + pa.half + j]
            let e0 = pa.neox != 0u ? j : j * 2u
            let e1 = pa.neox != 0u ? j + pa.half : j * 2u + 1u
            if (is_k) {
                let mrow = pa.layerbase + tok[row].mirbase + tok[row].pos * pa.kvd
                let kb = row * pa.kvrow + pa.qd + h * pa.hs
                let bk = pa.boff + pa.qd + h * pa.hs
                let a0 = kvsrc[kb + e0] + (pa.hasb != 0u ? bias[bk + e0] : 0.0)
                let a1 = kvsrc[kb + e1] + (pa.hasb != 0u ? bias[bk + e1] : 0.0)
                // v is not roped — copy the same two elements raw (plus their bias) into the V plane
                let vb = row * pa.kvrow + pa.qd + pa.kvd + h * pa.hs
                let bv = pa.boff + pa.qd + pa.kvd + h * pa.hs
                let v0 = kvsrc[vb + e0] + (pa.hasb != 0u ? bias[bv + e0] : 0.0)
                let v1 = kvsrc[vb + e1] + (pa.hasb != 0u ? bias[bv + e1] : 0.0)
                static_if (CLAMP16) {
                    kmir[mrow + h * pa.hs + e0] = float16(clamp(a0 * fcr - a1 * fci, -65504.0, 65504.0))
                    kmir[mrow + h * pa.hs + e1] = float16(clamp(a0 * fci + a1 * fcr, -65504.0, 65504.0))
                    vmir[mrow + h * pa.hs + e0] = float16(clamp(v0, -65504.0, 65504.0))
                    vmir[mrow + h * pa.hs + e1] = float16(clamp(v1, -65504.0, 65504.0))
                } else {
                    kmir[mrow + h * pa.hs + e0] = a0 * fcr - a1 * fci
                    kmir[mrow + h * pa.hs + e1] = a0 * fci + a1 * fcr
                    vmir[mrow + h * pa.hs + e0] = v0
                    vmir[mrow + h * pa.hs + e1] = v1
                }
            } else {
                let qb = row * pa.qrow + h * pa.hs
                let bq = pa.boff + h * pa.hs
                let a0 = qrows[qb + e0] + (pa.hasb != 0u ? bias[bq + e0] : 0.0)
                let a1 = qrows[qb + e1] + (pa.hasb != 0u ? bias[bq + e1] : 0.0)
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
    half : uint      // rotation pairs per head; pairs past it pass through (k and v stored unrotated)
    neox : uint
    kpair0 : uint    // first k pair's per-position index
    ppp : uint       // pairs per position (hs / 2 per head, rotated or not)
    layerbase : uint // this layer's K/V mirror base
    npos : uint
    pos0 : uint      // the window's first absolute position
    voff : uint
    kstride : uint
    qhs : uint       // per-head q stride: hs, or 2 * hs on a gated model ([q | gate] per head, the gate half left in place)
    boff : uint      // this layer's row base in the bias buffer ([q | k | v] rows, one layer after another)
    hasb : uint
    cstride : uint   //! a position's cos/sin plane width (every rope class's row); 0 = hs, the one-class plane
    cbase : uint     //! this layer's rope class's row base inside the plane
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
    @ssbo @binding = 5 @role = "weight" bias : array<float>   // per-layer [q | k | v] projection biases (hasb), else a placeholder
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
            let hpairs = pa.hs / 2u
            let h = pr / hpairs
            let j = pr % hpairs
            let rot = j < pa.half
            let cb = p * (pa.cstride == 0u ? pa.hs : pa.cstride) + pa.cbase
            let cj = min(j, pa.half - 1u)   // the pass-through pairs past the rotated half read a clamped table entry the select then drops
            let tcr = cossin[cb + cj]
            let tci = cossin[cb + pa.half + cj]
            let fcr = rot ? tcr : 1.0
            let fci = rot ? tci : 0.0
            let e0 = rot ? (pa.neox != 0u ? j : j * 2u) : 2u * pa.half + 2u * (j - pa.half)
            let e1 = rot ? (pa.neox != 0u ? j + pa.half : j * 2u + 1u) : e0 + 1u
            let apos = pa.pos0 + p
            let bhead = h * pa.hs
            if (is_k) {
                let kb = p * pa.kstride + h * pa.hs
                let a0 = kvrows[kb + e0] + (pa.hasb != 0u ? bias[pa.boff + pa.qd + bhead + e0] : 0.0)
                let a1 = kvrows[kb + e1] + (pa.hasb != 0u ? bias[pa.boff + pa.qd + bhead + e1] : 0.0)
                let mo = pa.layerbase + apos * pa.kvd + h * pa.hs
                let vb = pa.voff + p * pa.kstride + h * pa.hs
                let v0 = kvrows[vb + e0] + (pa.hasb != 0u ? bias[pa.boff + pa.qd + pa.kvd + bhead + e0] : 0.0)
                let v1 = kvrows[vb + e1] + (pa.hasb != 0u ? bias[pa.boff + pa.qd + pa.kvd + bhead + e1] : 0.0)
                static_if (CLAMP16) {
                    kmir[mo + e0] = float16(clamp(a0 * fcr - a1 * fci, -65504.0, 65504.0))
                    kmir[mo + e1] = float16(clamp(a0 * fci + a1 * fcr, -65504.0, 65504.0))
                    vmir[mo + e0] = float16(clamp(v0, -65504.0, 65504.0))
                    vmir[mo + e1] = float16(clamp(v1, -65504.0, 65504.0))
                } else {
                    kmir[mo + e0] = a0 * fcr - a1 * fci
                    kmir[mo + e1] = a0 * fci + a1 * fcr
                    vmir[mo + e0] = v0
                    vmir[mo + e1] = v1
                }
            } elif (rot || pa.hasb != 0u) {   // a pass-through q pair (fcr 1, fci 0) is rewritten only when a bias lands on it
                let qhs = pa.qhs == 0u ? pa.hs : pa.qhs   // 0 = compact (non-gated); a gated model passes 2 * hs
                let qb = p * (pa.qd / pa.hs) * qhs + h * qhs
                let a0 = qrows[qb + e0] + (pa.hasb != 0u ? bias[pa.boff + bhead + e0] : 0.0)
                let a1 = qrows[qb + e1] + (pa.hasb != 0u ? bias[pa.boff + bhead + e1] : 0.0)
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
    half : uint     // rotation pairs per head: hs/2, or rot/2 on a partial-rope model (the rest of the head passes through)
    neox : uint
    nh : uint       // q heads (wgs >= nh are k heads)
    layerbase : uint   // this layer's K/V mirror base; the row rides tok[0].pos
    qwoff : uint    // q norm-weight row base in wn
    kwoff : uint
    eps : float
    qstride : uint  // q head stride in qrows: 0 = hs; 2*hs on a gated-q model ([q | gate] per head, roped in place)
    boff : uint     // this layer's row base in the bias buffer ([q | k | v] rows, one layer after another)
    hasb : uint
    cbase : uint    //! this layer's rope class's row base in the cos/sin plane
    vfk : uint      //! V is the raw k row scaled by its own rms (a layer with no v plane: gemma-4's global layers)
    vnorm : uint    //! V is its own projection scaled by its rms, no weight (gemma-4's v-norm)
    rowwg : uint    //! head-row workgroups a row (q heads + k heads); 0 = the whole dispatch is row 0 (the one-row command passes its count: the same reading)
    kvrow : uint    //! a row's stride in the projection plane
    qrow : uint     //! a row's stride in qrows
    cosrow : uint   //! a row's stride in the cos/sin plane
}

// qk_rms + rope_kv_store in ONE dispatch (qk_norm models); KT/CLAMP16 as in RopeKvStoreT.
// A head past 256 (gemma-4's global layers, 512) gives each thread a second element at tid + 256.
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
    @ssbo @binding = 7 @role = "weight" bias : array<float>   // per-layer [q | k | v] projection biases (hasb), else a placeholder
    @push_constant pa : QknRopeArgs
    @workgroup row : float[512]                // this head's normed q or k row (hs <= 512)
    @template_constant CLAMP16 : bool = true

    //! one K mirror element, clamped into f16 on the f16 stamp
    def st_k(i : uint; v : float) {
        static_if (CLAMP16) {
            kmir[i] = float16(clamp(v, -65504.0, 65504.0))
        } else {
            kmir[i] = v
        }
    }

    //! ... and one V mirror element
    def st_v(i : uint; v : float) {
        static_if (CLAMP16) {
            vmir[i] = float16(clamp(v, -65504.0, 65504.0))
        } else {
            vmir[i] = v
        }
    }

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
    def run {   // nolint:STYLE037,STYLE038 — the verbatim norm + rotation + store pass, parity-locked to the shipped kernel; the V forms and the wide arm ride the same head row
        let gwid = gl_WorkGroupID.x
        let rowwg = max(pa.rowwg, 1u)
        let rowi = pa.rowwg == 0u ? 0u : gwid / rowwg
        let wid = pa.rowwg == 0u ? gwid : gwid % rowwg
        let is_k = wid >= pa.nh
        let h = is_k ? wid - pa.nh : wid
        let tid = gl_LocalInvocationID.x
        let t1 = tid + 256u
        let two = t1 < pa.hs   // the wide head's second element
        let qstride = pa.qstride == 0u ? pa.hs : pa.qstride
        let qb = rowi * pa.qrow + h * qstride
        let kvb = rowi * pa.kvrow
        let sbase = is_k ? kvb + pa.qd + h * pa.hs : qb
        let bbase = is_k ? pa.qd + h * pa.hs : h * pa.hs
        let cb = rowi * pa.cosrow + pa.cbase
        let mrow = pa.layerbase + tok[rowi].mirbase + tok[rowi].pos * pa.kvd
        var v = 0.0
        var v1 = 0.0
        if (tid < pa.hs) {
            v = (is_k ? kvsrc[sbase + tid] : qrows[sbase + tid]) + (pa.hasb != 0u ? bias[pa.boff + bbase + tid] : 0.0)
        }
        if (two) {
            v1 = (is_k ? kvsrc[sbase + t1] : qrows[sbase + t1]) + (pa.hasb != 0u ? bias[pa.boff + bbase + t1] : 0.0)
        }
        let ss = tid < pa.hs ? v * v + v1 * v1 : 0.0
        let inv = wg_rms_inv(ss, pa.hs, pa.eps, 0u)
        let woff = is_k ? pa.kwoff : pa.qwoff
        if (tid < pa.hs) {
            row[tid] = wn[woff + tid] * (v * inv)
        }
        if (two) {
            row[t1] = wn[woff + t1] * (v1 * inv)
        }
        barrier()
        // rope this head's pairs off the normed row; the pairs partition [0, 2 * half)
        if (tid < pa.half) {
            let fcr = cossin[cb + tid]
            let fci = cossin[cb + pa.half + tid]
            let e0 = pa.neox != 0u ? tid : tid * 2u
            let e1 = pa.neox != 0u ? tid + pa.half : tid * 2u + 1u
            let a0 = row[e0]
            let a1 = row[e1]
            let r0 = a0 * fcr - a1 * fci
            let r1 = a0 * fci + a1 * fcr
            if (is_k) {
                st_k(mrow + h * pa.hs + e0, r0)
                st_k(mrow + h * pa.hs + e1, r1)
            } else {
                qrows[qb + e0] = r0
                qrows[qb + e1] = r1
            }
        }
        if (!is_k && tid >= 2u * pa.half && tid < pa.hs) {
            qrows[qb + tid] = row[tid]
        }
        if (!is_k && two && t1 >= 2u * pa.half) {
            qrows[qb + t1] = row[t1]
        }
        // the V row, by the k-head wgs: the raw projection (plus its bias), that row scaled by its own
        // rms under vnorm, or under vfk the raw k row scaled by the rms the k-norm just took
        if (is_k) {
            var w0 = 0.0
            var w1 = 0.0
            if (pa.vfk != 0u) {
                w0 = v * inv
                w1 = v1 * inv
            } else {
                let vb = pa.qd + pa.kvd + h * pa.hs   //! the V head's offset in the projection row and in the layer's bias block: the row stride joins the first alone
                let vi = kvb + vb
                if (tid < pa.hs) {
                    w0 = kvsrc[vi + tid] + (pa.hasb != 0u ? bias[pa.boff + vb + tid] : 0.0)
                }
                if (two) {
                    w1 = kvsrc[vi + t1] + (pa.hasb != 0u ? bias[pa.boff + vb + t1] : 0.0)
                }
                if (pa.vnorm != 0u) {
                    let vinv = wg_rms_inv(tid < pa.hs ? w0 * w0 + w1 * w1 : 0.0, pa.hs, pa.eps, 1u)
                    w0 *= vinv
                    w1 *= vinv
                }
            }
            if (tid < pa.hs) {
                st_v(mrow + h * pa.hs + tid, w0)
                if (tid >= 2u * pa.half) {
                    st_k(mrow + h * pa.hs + tid, row[tid])
                }
            }
            if (two) {
                st_v(mrow + h * pa.hs + t1, w1)
                if (t1 >= 2u * pa.half) {
                    st_k(mrow + h * pa.hs + t1, row[t1])
                }
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
    lanes_per_row : uint   //! 8, 16, or 0 = the whole subgroup (the q8 GEMV ignores it)
    act : uint    //! the fused gate-up twins' activation code (`act_code`: silu, gelu, the clamped swiglu); the plain GEMVs ignore it
    bias_on : uint   //! the mx4 gate-up twin: each slot's expert (`rex`) names its gate and up bias rows, `gboff` / `uboff` this layer's base in the planes ([ne x d] rows a layer)
    gboff : uint
    uboff : uint
    ncols : uint     //! the N-column forms: activation rows dotted against one weight pass, 1 to 8; the one-column forms ignore it
    ystride : uint   //! ... and the distance between their y rows
}

// ===== the Q8_0 decode GEMV pair =====

// q8_moe_groupn's class twin: TWO lanes per 32-block (a uint4 each), xor-1 fold, even lane folds
// the scales. The workhorse decode GEMV.
[vk_dispatch(name = "q8_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q8Gemv {
    @ssbo @binding = 0 @role = "weight" wq4 : array<uint4>      // weight quant words, vec4 view (16B loads)
    @ssbo @binding = 1 @role = "weight" wsh : array<float16>    // per-block f16 weight scales
    @ssbo @binding = 2 regions : array<uint>   // (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xq4 : array<uint4>      // activation quant words, vec4 view: a lane's half-block in one 16-byte load
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q8_gemv_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
    def run {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.nreg * pa.d) {
            let r = rg % pa.nreg   //! the regions interleaved a row at a time: a weight row two regions share (two routed slots on one expert) leaves DRAM once and reaches the next from cache
            let row = rg / pa.nreg
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
                    let xv = xq4[(xb0 + bi0) * 2u + lane]
                    var idot = sdot4(wv.x, xv.x)
                    idot += sdot4(wv.y, xv.y)
                    idot += sdot4(wv.z, xv.z)
                    idot += sdot4(wv.w, xv.w)
                    idot += subgroupShuffleXor(idot, 1u)
                    if (lane % 2u == 0u) {
                        acc += float(idot) * float(wsh[wb0 + b]) * axs[xb0 + b]
                    }
                }
                bi0 += bpp
            }
            let total = subgroupAdd(acc)
            if (subgroupElect()) {
                y[pa.ybase + r * pa.d + row] = total
            }
        }
    }
}

//! the Q8_0 GEMV's N-column forms: `NC` activation rows (column c's blocks c x nb past the region's, `pa.ncols` of
//! them live) against a lane pair's weight word and scale read once, each row's half-block one 16-byte load, the y
//! rows `pa.ystride` apart; the `R2` stamps take two output rows a subgroup, so every activation load feeds two
//! weight rows and a long row's re-reads halve - the rows a region holds must then be even
[ |> template_struct_instance]
class template Q8GemvNT {
    @ssbo @binding = 0 @role = "weight" wq4 : array<uint4>      // weight quant words, vec4 view (16B loads)
    @ssbo @binding = 1 @role = "weight" wsh : array<float16>    // per-block f16 weight scales
    @ssbo @binding = 2 regions : array<uint>   // (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xq4 : array<uint4>      // activation quant words, vec4 view
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs
    @template_constant NC : int = 4       //!< the columns a lane dots per weight word
    @template_constant R2 : bool = false  //!< two output rows a subgroup

    //! the row pair's second weight word and scale at block `b`, dotted against the same activation word
    def pair_dot(wv1 : uint4; xv : uint4) : int {
        var i1 = sdot4(wv1.x, xv.x)
        i1 += sdot4(wv1.y, xv.y)
        i1 += sdot4(wv1.z, xv.z)
        i1 += sdot4(wv1.w, xv.w)
        return i1 + subgroupShuffleXor(i1, 1u)
    }

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        let sg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let per = R2 ? pa.d / 2u : pa.d   // the subgroups a region takes: a row each, or a row pair
        if (sg < pa.nreg * per) {
            let r = sg / per
            let row = R2 ? (sg % per) * 2u : sg % per
            let nb = pa.n / 32u
            let wb0 = regions[r * 2u] + row * nb
            let xb0 = regions[r * 2u + 1u]
            let lane = gl_SubgroupInvocationID
            let bpp = gl_SubgroupSize / 2u
            var acc0 : float[8]
            var acc1 : float[8]
            for [unroll] (c in range(NC)) {
                acc0[c] = 0.0
                acc1[c] = 0.0
            }
            let clast = max(pa.ncols, 1u) - 1u   // a column past the live ones dots column `clast`'s row again: its loads issue beside the live ones and its sum is never stored
            var bi0 = 0u
            while (bi0 < nb) {
                let b = bi0 + lane / 2u
                if (b < nb) {
                    let wv0 = wq4[(wb0 + bi0) * 2u + lane]
                    let ws0 = float(wsh[wb0 + b])
                    for [unroll] (c in range(NC)) {
                        let xb = xb0 + min(uint(c), clast) * nb
                        let xv = xq4[(xb + bi0) * 2u + lane]
                        let i0 = pair_dot(wv0, xv)
                        static_if (R2) {
                            let i1 = pair_dot(wq4[(wb0 + nb + bi0) * 2u + lane], xv)
                            if (lane % 2u == 0u) {
                                let xs = axs[xb + b]
                                acc0[c] += float(i0) * ws0 * xs
                                acc1[c] += float(i1) * float(wsh[wb0 + nb + b]) * xs
                            }
                        } else {
                            if (lane % 2u == 0u) {
                                acc0[c] += float(i0) * ws0 * axs[xb + b]
                            }
                        }
                    }
                }
                bi0 += bpp
            }
            for [unroll] (c in range(NC)) {
                if (uint(c) < pa.ncols) {
                    let t0 = subgroupAdd(acc0[c])
                    static_if (R2) {
                        let t1 = subgroupAdd(acc1[c])
                        if (subgroupElect()) {
                            let yb = pa.ybase + uint(c) * pa.ystride + r * pa.d + row
                            y[yb] = t0
                            y[yb + 1u] = t1
                        }
                    } else {
                        if (subgroupElect()) {
                            y[pa.ybase + uint(c) * pa.ystride + sg] = t0
                        }
                    }
                }
            }
        }
    }
}

[vk_dispatch(name = "q8_gemv_n2_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN2 : Q8GemvNT {
    override NC = 2
}

[vk_dispatch(name = "q8_gemv_n4_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN4 : Q8GemvNT {
    override NC = 4
}

[vk_dispatch(name = "q8_gemv_n8_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN8 : Q8GemvNT {
    override NC = 8
}

[vk_dispatch(name = "q8_gemv_n2p_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN2P : Q8GemvNT {
    override NC = 2
    override R2 = true
}

[vk_dispatch(name = "q8_gemv_n4p_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN4P : Q8GemvNT {
    override NC = 4
    override R2 = true
}

[vk_dispatch(name = "q8_gemv_n8p_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN8P : Q8GemvNT {
    override NC = 8
    override R2 = true
}

struct GemvArArgs {
    n : uint        //! the GEMV's reduction width
    d : uint        //! its output rows - the residual row's width
    wblk : uint     //! the plane's block base in its slab (one region: the pair rides the push)
    xblk : uint     //! the activation's block base
    add_on : uint   //! the residual step's `ArArgs` fields (the add partner is the GEMV's own row)
    woff : uint
    pwoff : uint
    pre_on : uint
    no_norm : uint
    eps : float
    ascale : float
    abias_on : uint   //! the GEMV's row takes a bias row out of wn before the add (gpt-oss's attention output bias)
    aboff : uint      //! ... that row's base in wn
}

//! the q8 decode GEMV carrying the residual step as its last workgroup's epilogue: a subgroup a row, eight rows a
//! workgroup, the rows into a `@coherent` y, the last workgroup to arrive running `ArRq`'s step and resetting the counter
[vk_dispatch(name = "q8_gemv_ar_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvAr : RmsWgBase {
    @ssbo @binding = 0 wq4 : array<uint4>          // weight quant words, vec4 view
    @ssbo @binding = 1 wsh : array<float16>        // per-block f16 weight scales
    @ssbo @binding = 2 xqw : array<uint>           // the GEMV's activation quant words
    @ssbo @binding = 3 axs : array<float>          // ... and their block scales
    @ssbo @binding = 4 @coherent y : array<float>  // the rows out, read back whole by the last workgroup
    @ssbo @binding = 5 xres : array<float>         // the residual row, updated in place
    @ssbo @binding = 6 wn : array<float>           // the norms plane
    @ssbo @binding = 7 outq : array<int8>          // the normed row's Q8_0 quants (the next GEMV's feed)
    @ssbo @binding = 8 outs : array<float>         // ... and its block scales
    @ssbo @binding = 9 cnt : array<uint>           // the arrival counter at word 0, zero between dispatches
    @push_constant pa : GemvArArgs
    @workgroup arrived : uint[1]

    //! the norm weight at `e`, one under `no_norm`
    def nw(e : uint) : float => pa.no_norm != 0u ? 1.0 : wn[pa.woff + e]

    //! the finished row's element `j`, its bias row added under `abias_on`
    def ya(j : uint) : float => y[j] + (pa.abias_on != 0u ? wn[pa.aboff + j] : 0.0)

    //! the residual step over the finished rows, `ArRqBase.rq_row`'s math on the workgroup's 256 threads
    [arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#residual-step-fma")]
    def epilogue {   // nolint:STYLE038 — one row pass: the sandwich column, the four-column round, the requant
        let tid = gl_LocalInvocationID.x
        var ainv = 0.0
        if (pa.pre_on != 0u && pa.add_on != 0u) {
            var sa = 0.0
            var j = tid
            while (j < pa.d) {
                let a = ya(j)
                sa += a * a
                j += 256u
            }
            ainv = wg_rms_inv(sa, pa.d, pa.eps, 0u)
        }
        var ss = 0.0
        var k = tid
        if (pa.pre_on != 0u) {   //! the sandwich form, a column a round
            while (k < pa.d) {
                let vv = pa.add_on != 0u ? mad(wn[pa.pwoff + k], ya(k) * ainv, xres[k]) * pa.ascale : xres[k]
                xres[k] = vv
                ss += vv * vv
                k += 256u
            }
        }
        let last = pa.d - 1u
        while (pa.pre_on == 0u && k < pa.d) {   //! four columns a round, every load issued before the adds
            let k1 = min(k + 256u, last)
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            var v0 = xres[k]
            var v1 = xres[k1]
            var v2 = xres[k2]
            var v3 = xres[k3]
            if (pa.add_on != 0u) {
                let a0 = ya(k)
                let a1 = ya(k1)
                let a2 = ya(k2)
                let a3 = ya(k3)
                v0 = (v0 + a0) * pa.ascale
                v1 = (v1 + a1) * pa.ascale
                v2 = (v2 + a2) * pa.ascale
                v3 = (v3 + a3) * pa.ascale
            }
            xres[k] = v0
            ss += v0 * v0
            if (k + 256u < pa.d) {
                xres[k1] = v1
                ss += v1 * v1
            }
            if (k + 512u < pa.d) {
                xres[k2] = v2
                ss += v2 * v2
            }
            if (k + 768u < pa.d) {
                xres[k3] = v3
                ss += v3 * v3
            }
            k += 1024u
        }
        let inv0 = wg_rms_inv(ss, pa.d, pa.eps, 1u)
        let inv = pa.no_norm != 0u ? 1.0 : inv0
        memoryBarrierBuffer()   //! the row's writes reach the lanes that quantize them
        barrier()
        let nblk = pa.d / 32u
        let lane8 = tid % 8u
        var b = tid / 8u
        while (b < nblk) {
            let base = b * 32u + lane8 * 4u
            let v0 = nw(base) * (xres[base] * inv)
            let v1 = nw(base + 1u) * (xres[base + 1u] * inv)
            let v2 = nw(base + 2u) * (xres[base + 2u] * inv)
            let v3 = nw(base + 3u) * (xres[base + 3u] * inv)
            let m = q8_amax3(max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3))))
            let dq = m / 127.0
            let id = dq != 0.0 ? 1.0 / dq : 0.0
            if (lane8 == 0u) {
                outs[b] = dq
            }
            outq[base] = q8_quant1(v0, id)
            outq[base + 1u] = q8_quant1(v1, id)
            outq[base + 2u] = q8_quant1(v2, id)
            outq[base + 3u] = q8_quant1(v3, id)
            b += 32u
        }
        if (tid == 0u) {
            cnt[0] = 0u
        }
    }

    [spirv_kernel(local_size_x = 256, name = "q8_gemv_ar_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.d) {   // the q8 GEMV's row: two lanes a block, the xor fold
            let nb = pa.n / 32u
            let wb0 = pa.wblk + rg * nb
            let lane = gl_SubgroupInvocationID
            let bpp = gl_SubgroupSize / 2u
            var acc = 0.0
            var bi0 = 0u
            while (bi0 < nb) {
                let b = bi0 + lane / 2u
                if (b < nb) {
                    let wv = wq4[(wb0 + bi0) * 2u + lane]
                    let xbase = (pa.xblk + bi0) * 8u + lane * 4u
                    var idot = sdot4(wv.x, xqw[xbase])
                    idot += sdot4(wv.y, xqw[xbase + 1u])
                    idot += sdot4(wv.z, xqw[xbase + 2u])
                    idot += sdot4(wv.w, xqw[xbase + 3u])
                    idot += subgroupShuffleXor(idot, 1u)
                    if (lane % 2u == 0u) {
                        acc += float(idot) * float(wsh[wb0 + b]) * axs[pa.xblk + b]
                    }
                }
                bi0 += bpp
            }
            let total = subgroupAdd(acc)
            if (subgroupElect()) {
                y[rg] = total
            }
        }
        memoryBarrierBuffer()
        barrier()
        if (gl_LocalInvocationID.x == 0u) {
            arrived[0] = atomicAdd(cnt[0], 1u)
        }
        barrier()
        if (arrived[0] + 1u == gl_NumWorkGroups.x) {
            memoryBarrierBuffer()
            epilogue()
        }
    }
}

struct GuArgs {
    n : uint      //! the reduction width (the model dim)
    d : uint      //! the output rows (the FFN width), a 32-multiple
    gblk : uint   //! the gate plane's block base in its slab
    ublk : uint   //! the up plane's block base in its slab
    act : uint    //! the activation code: ACT_SILU / ACT_GELU / ACT_OAI
}

//! the dense FFN's gate and up GEMVs, the activation and its Q8_0 requant in one dispatch: a workgroup owns 32
//! output rows (one requant block), its subgroups dot both q8 planes, and one subgroup activates and quantizes the block
[vk_dispatch(name = "q8_gemv_gu_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvGu {
    @ssbo @binding = 0 @role = "weight" gq4 : array<uint4>     // gate quant words, vec4 view
    @ssbo @binding = 1 @role = "weight" gsh : array<float16>   // gate per-block f16 scales
    @ssbo @binding = 2 @role = "weight" uq4 : array<uint4>     // up quant words, vec4 view
    @ssbo @binding = 3 @role = "weight" ush : array<float16>   // up per-block f16 scales
    @ssbo @binding = 4 xqw : array<uint>      // the normed row's Q8_0 quant words
    @ssbo @binding = 5 axs : array<float>     // ... and its per-block scales
    @ssbo @binding = 6 outq : array<int8>     // the activated hidden row's Q8_0 quants (the down GEMV's feed)
    @ssbo @binding = 7 outs : array<float>    // ... and its per-block scales
    @push_constant pa : GuArgs
    @workgroup wg_g : float[32]   //! the block's gate dots
    @workgroup wg_u : float[32]   //! ... and up dots

    //! one output row's gate and up dots over the shared x, a subgroup wide
    def row_dots(row : uint) : float2 {
        let nb = pa.n / 32u
        let gb0 = pa.gblk + row * nb
        let ub0 = pa.ublk + row * nb
        let lane = gl_SubgroupInvocationID
        let bpp = gl_SubgroupSize / 2u
        var accg = 0.0
        var accu = 0.0
        var bi0 = 0u
        while (bi0 < nb) {
            let b = bi0 + lane / 2u
            if (b < nb) {
                let gv = gq4[(gb0 + bi0) * 2u + lane]
                let uv = uq4[(ub0 + bi0) * 2u + lane]
                let xbase = bi0 * 8u + lane * 4u
                let x0 = xqw[xbase]
                let x1 = xqw[xbase + 1u]
                let x2 = xqw[xbase + 2u]
                let x3 = xqw[xbase + 3u]
                var gd = sdot4(gv.x, x0)
                gd += sdot4(gv.y, x1)
                gd += sdot4(gv.z, x2)
                gd += sdot4(gv.w, x3)
                var ud = sdot4(uv.x, x0)
                ud += sdot4(uv.y, x1)
                ud += sdot4(uv.z, x2)
                ud += sdot4(uv.w, x3)
                gd += subgroupShuffleXor(gd, 1u)
                ud += subgroupShuffleXor(ud, 1u)
                if (lane % 2u == 0u) {
                    let xs = axs[b]
                    accg += float(gd) * float(gsh[gb0 + b]) * xs
                    accu += float(ud) * float(ush[ub0 + b]) * xs
                }
            }
            bi0 += bpp
        }
        return float2(subgroupAdd(accg), subgroupAdd(accu))
    }

    [spirv_kernel(local_size_x = 256, name = "q8_gemv_gu_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let row0 = gl_WorkGroupID.x * 32u
        let nsg = gl_NumSubgroups
        let rows_per = 32u / nsg   // four rows a subgroup at 32 lanes (eight subgroups), eight at 64: every row of the block in flight at once
        var k = 0u
        while (k < rows_per) {
            let r = gl_SubgroupID + k * nsg
            let v = row_dots(row0 + r)
            if (subgroupElect()) {
                wg_g[r] = v.x
                wg_u[r] = v.y
            }
            k++
        }
        barrier()
        if (gl_SubgroupID == 0u) {   // the block's activation and Q8_0 quant: one lane an output row
            let lane = gl_SubgroupInvocationID
            let li = min(lane, 31u)
            let a = lane < 32u ? act_mul_sel(wg_g[li], wg_u[li], pa.act) : 0.0
            let d = da_q8_blk_d(a)
            let id = d != 0.0 ? 1.0 / d : 0.0
            if (lane == 0u) {
                outs[gl_WorkGroupID.x] = d
            }
            if (lane < 32u) {
                outq[row0 + lane] = q8_quant1(a, id)
            }
        }
    }
}

struct GuNArgs {
    n : uint      //! the reduction width (the model dim)
    d : uint      //! the output rows (the FFN width), a 32-multiple
    gblk : uint   //! the gate plane's block base in its slab
    ublk : uint   //! the up plane's block base in its slab
    act : uint    //! the activation code: ACT_SILU / ACT_GELU / ACT_OAI
    ncols : uint  //! the rows the command steps at once, 1 to GU_N_COLS: column c's activation blocks sit c x (n / 32) past the first, its quants c x d past
}

let GU_N_COLS = 8u   //! the widest gate-up N stamp's columns - the width every stamp's slabs and accumulators carry; `q8_gemv_gu_n_ensure` holds the rail's row count to it

//! the fused gate-up GEMV's N-column forms: `NC` column slots a weight word, `pa.ncols` of them live and stored
[ |> template_struct_instance]
class template Q8GemvGuNT {
    @ssbo @binding = 0 @role = "weight" gq4 : array<uint4>     // gate quant words, vec4 view
    @ssbo @binding = 1 @role = "weight" gsh : array<float16>   // gate per-block f16 scales
    @ssbo @binding = 2 @role = "weight" uq4 : array<uint4>     // up quant words, vec4 view
    @ssbo @binding = 3 @role = "weight" ush : array<float16>   // up per-block f16 scales
    @ssbo @binding = 4 xq4 : array<uint4>     // the normed rows' Q8_0 quant words, vec4 view, a row a column
    @ssbo @binding = 5 axs : array<float>     // ... and their per-block scales
    @ssbo @binding = 6 outq : array<int8>     // the activated hidden rows' Q8_0 quants (the down GEMV's feed), d apart
    @ssbo @binding = 7 outs : array<float>    // ... and their per-block scales, d / 32 apart
    @push_constant pa : GuNArgs
    @template_constant NC : int = 4       //!< the columns a lane dots per weight word
    @workgroup wg_g : float[256]   //! the block's gate dots, [column x 32 rows]
    @workgroup wg_u : float[256]   //! ... and up dots

    //! one output row's gate and up dots against every column's row, a subgroup wide, into the slab
    def row_dots(r : uint) {
        let nb = pa.n / 32u
        let row = gl_WorkGroupID.x * 32u + r
        let gb0 = pa.gblk + row * nb
        let ub0 = pa.ublk + row * nb
        let lane = gl_SubgroupInvocationID
        let bpp = gl_SubgroupSize / 2u
        var accg : float[8]
        var accu : float[8]
        for [unroll] (c in range(NC)) {
            accg[c] = 0.0
            accu[c] = 0.0
        }
        let clast = max(pa.ncols, 1u) - 1u   // a column past the live ones dots column `clast`'s row again: its loads issue beside the live ones and its block is never stored
        var bi0 = 0u
        while (bi0 < nb) {
            let b = bi0 + lane / 2u
            if (b < nb) {
                let gv = gq4[(gb0 + bi0) * 2u + lane]
                let uv = uq4[(ub0 + bi0) * 2u + lane]
                let gs = float(gsh[gb0 + b])
                let us = float(ush[ub0 + b])
                for [unroll] (c in range(NC)) {
                    let xb = min(uint(c), clast) * nb
                    let xv = xq4[(xb + bi0) * 2u + lane]
                    var gd = sdot4(gv.x, xv.x)
                    gd += sdot4(gv.y, xv.y)
                    gd += sdot4(gv.z, xv.z)
                    gd += sdot4(gv.w, xv.w)
                    var ud = sdot4(uv.x, xv.x)
                    ud += sdot4(uv.y, xv.y)
                    ud += sdot4(uv.z, xv.z)
                    ud += sdot4(uv.w, xv.w)
                    gd += subgroupShuffleXor(gd, 1u)
                    ud += subgroupShuffleXor(ud, 1u)
                    if (lane % 2u == 0u) {
                        let xs = axs[xb + b]
                        accg[c] += float(gd) * gs * xs
                        accu[c] += float(ud) * us * xs
                    }
                }
            }
            bi0 += bpp
        }
        for [unroll] (c in range(NC)) {
            if (uint(c) < pa.ncols) {
                let g = subgroupAdd(accg[c])
                let u = subgroupAdd(accu[c])
                if (subgroupElect()) {
                    wg_g[uint(c) * 32u + r] = g
                    wg_u[uint(c) * 32u + r] = u
                }
            }
        }
    }

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let row0 = gl_WorkGroupID.x * 32u
        let nsg = gl_NumSubgroups
        let rows_per = 32u / nsg
        var k = 0u
        while (k < rows_per) {
            row_dots(gl_SubgroupID + k * nsg)
            k++
        }
        barrier()
        let ncols = min(pa.ncols, uint(NC))
        var c = gl_SubgroupID
        while (c < ncols) {
            let lane = gl_SubgroupInvocationID
            let li = min(lane, 31u)
            let a = lane < 32u ? act_mul_sel(wg_g[c * 32u + li], wg_u[c * 32u + li], pa.act) : 0.0
            let d = da_q8_blk_d(a)
            let id = d != 0.0 ? 1.0 / d : 0.0
            if (lane == 0u) {
                outs[c * (pa.d / 32u) + gl_WorkGroupID.x] = d
            }
            if (lane < 32u) {
                outq[c * pa.d + row0 + lane] = q8_quant1(a, id)
            }
            c += nsg
        }
    }
}

[vk_dispatch(name = "q8_gemv_gu_n2_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvGuN2 : Q8GemvGuNT {
    override NC = 2
}

[vk_dispatch(name = "q8_gemv_gu_n4_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvGuN4 : Q8GemvGuNT {
    override NC = 4
}

[vk_dispatch(name = "q8_gemv_gu_n8_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvGuN8 : Q8GemvGuNT {
    override NC = 8
}

//! the fused gate-up N stamps for up to `nb` columns
def q8_gemv_gu_n_ensure(nb : int64 = 8l) : bool {
    verify(nb <= int64(GU_N_COLS), "vk decode: the N-row command's row count is past the widest gate-up stamp")
    var ok = ensure_q8_gemv_gu_n2_cls()
    if (nb > 2l) {
        ok = ok && ensure_q8_gemv_gu_n4_cls()
    }
    if (nb > 4l) {
        ok = ok && ensure_q8_gemv_gu_n8_cls()
    }
    return ok
}

//! the gate-up N stamps share one layout, so the narrowest stamp's set binds them all
def q8_gemv_gu_n_set(bufs : uint64 const[8]; sizes : int64 const[8]; gbits : uint const[8]) : VkDescriptorSet
    => set_q8_gemv_gu_n2_cls(bufs, sizes, gbits)

//! the stamp for `pc.ncols` columns: the narrowest that holds them
def q8_gemv_gu_n_enc(raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GuNArgs; wgs : int64) {
    if (pc.ncols <= 2u) {
        enc_q8_gemv_gu_n2_cls(raw, h, set_, pc, wgs)
    } elif (pc.ncols <= 4u) {
        enc_q8_gemv_gu_n4_cls(raw, h, set_, pc, wgs)
    } else {
        enc_q8_gemv_gu_n8_cls(raw, h, set_, pc, wgs)
    }
}

struct PleActArgs {
    n : uint      //! the side row's width (the reduction; at most 256, a 32-multiple)
    d : uint      //! the output rows (the model dim)
    wblk : uint   //! the proj plane's block base in its slab
    ubase : uint  //! this layer's row base in the side plane
    act : uint   // the activation code: ACT_SILU / ACT_GELU / ACT_OAI
    ncols : uint    //! the rows the dispatch serves (the N-row command, at most eight): column c's gate row at `c * n`, its side row `c * sstride` past `ubase`, its proj row at `c * d`; 0 or 1 = one row
    sstride : uint  //! a row's stride in the side plane (every layer's row)
}

//! the per-layer-embedding branch's activation, its Q8_0 requant and the proj GEMV in one dispatch: every workgroup
//! rebuilds the (at most 256-wide) x in workgroup memory, then dots one output row a subgroup - the redundant build costs 256 elements a workgroup
[vk_dispatch(name = "q8_gemv_pleact_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvPleAct {
    @ssbo @binding = 0 wq4 : array<uint4>     // proj quant words, vec4 view
    @ssbo @binding = 1 wsh : array<float16>   // proj per-block f16 scales
    @ssbo @binding = 2 gate : array<float>    // the gate GEMV's row
    @ssbo @binding = 3 side : array<float>    // the side plane (every layer's row)
    @ssbo @binding = 4 y : array<float>       // the proj row out
    @push_constant pa : PleActArgs
    @workgroup xw : uint[512]   //! the built x's Q8_0 quant words, a column's 64 at `c * 64` (eight columns, the per-row planes' extent)
    @workgroup xs : float[64]   //! ... and its block scales, a column's eight at `c * 8`

    [spirv_kernel(local_size_x = 256, name = "q8_gemv_pleact_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def run {
        let tid = gl_LocalInvocationID.x
        let nc = clamp(pa.ncols, 1u, 8u)
        var k0 = tid
        while (k0 < 64u * nc) {
            xw[k0] = 0u
            k0 += 256u
        }
        barrier()
        let e = min(tid, pa.n - 1u)
        var c0 = 0u
        while (c0 < nc) {
            let v = tid < pa.n ? act_mul_sel(gate[c0 * pa.n + e], side[pa.ubase + c0 * pa.sstride + e], pa.act) : 0.0
            let d = da_q8_blk_d(v)
            let id = d != 0.0 ? 1.0 / d : 0.0
            let q = uint(int(round(v * id))) & 0xFFu
            atomicOr(xw[c0 * 64u + tid / 4u], q << ((tid % 4u) * 8u))
            if ((tid & 31u) == 0u) {
                xs[c0 * 8u + tid / 32u] = d
            }
            c0++
        }
        barrier()
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row < pa.d) {
            let nb = pa.n / 32u
            let wb0 = pa.wblk + row * nb
            let lane = gl_SubgroupInvocationID
            let bpp = gl_SubgroupSize / 2u
            var acc : float[8]
            var c1 = 0u
            while (c1 < 8u) {
                acc[c1] = 0.0
                c1++
            }
            var bi0 = 0u
            while (bi0 < nb) {
                let b = bi0 + lane / 2u
                let inb = b < nb
                let wv = inb ? wq4[(wb0 + bi0) * 2u + lane] : uint4(0u)
                let ws = inb ? float(wsh[wb0 + min(b, nb - 1u)]) : 0.0
                let xbase = inb ? bi0 * 8u + lane * 4u : 0u   //! a dead lane's index stays inside the words whatever the emitter makes of the ternaries below
                var c2 = 0u
                while (c2 < nc) {
                    let xb = c2 * 64u + xbase
                    var idot = inb ? sdot4(wv.x, xw[xb]) : 0
                    idot += inb ? sdot4(wv.y, xw[xb + 1u]) : 0
                    idot += inb ? sdot4(wv.z, xw[xb + 2u]) : 0
                    idot += inb ? sdot4(wv.w, xw[xb + 3u]) : 0
                    idot += subgroupShuffleXor(idot, 1u)
                    if (inb && lane % 2u == 0u) {
                        acc[c2] += float(idot) * ws * xs[c2 * 8u + b]
                    }
                    c2++
                }
                bi0 += bpp
            }
            var c3 = 0u
            while (c3 < nc) {
                let total = subgroupAdd(acc[c3])
                if (subgroupElect()) {
                    y[c3 * pa.d + row] = total
                }
                c3++
            }
        }
    }
}

//! the four fifth bits of a nibble word's elements as their byte's 0x10: nibble n (bits 0..3) spread to bits 4, 12, 20, 28
def q51_hbits(n : uint) : uint => ((n & 0xFu) * 0x02040810u) & 0x10101010u

//! Q5_1 x Q8_0 decode GEMV, `q8_gemv_cls`'s shape with ONE lane per 32-block: the block's five words (four
//! nibble words, the qh word), the low and high nibbles raised to bytes with their fifth bit, sdot4 against the
//! activation words; (d * isum + m * asum) * xs per block - `dot_q51q8_scalar`'s float order, reassociated by lane
class Q51GemvBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words, five per block
    @ssbo @binding = 1 wsu : array<uint>       // per-block f16 d | f16 m
    @ssbo @binding = 2 regions : array<uint>   // (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs

    //! one row's dot over `nb` blocks from weight block `wb0` and activation block `xb0`: a lane a block, the subgroup's sum
    def row_dot(wb0, xb0, nb : uint) : float {
        let lane = gl_SubgroupInvocationID
        var acc = 0.0
        var bi0 = 0u
        while (bi0 < nb) {
            let b = bi0 + lane
            if (b < nb) {
                let wbase = (wb0 + b) * 5u
                let xbase = (xb0 + b) * 8u
                let qh = wq[wbase + 4u]
                var idot = 0
                var asum = 0
                for [unroll_full] (c in range(4)) {
                    let w = wq[wbase + uint(c)]
                    let xl = xqw[xbase + uint(c)]
                    let xh = xqw[xbase + 4u + uint(c)]
                    idot += sdot4((w & 0x0F0F0F0Fu) | q51_hbits(qh >> (uint(c) * 4u)), xl)
                    idot += sdot4(((w >> 4u) & 0x0F0F0F0Fu) | q51_hbits(qh >> (16u + uint(c) * 4u)), xh)
                    asum += sdot4(xl, 0x01010101u) + sdot4(xh, 0x01010101u)
                }
                let dm = unpackHalf2x16(wsu[wb0 + b])
                acc += (dm.x * float(idot) + dm.y * float(asum)) * axs[xb0 + b]
            }
            bi0 += gl_SubgroupSize
        }
        return subgroupAdd(acc)
    }
}

[vk_dispatch(name = "q51_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q51Gemv : Q51GemvBase {
    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q51_gemv_cls_spv")]
    def run {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.nreg * pa.d) {
            let r = rg % pa.nreg
            let row = rg / pa.nreg
            let nb = pa.n / 32u
            let total = row_dot(regions[r * 2u] + row * nb, regions[r * 2u + 1u], nb)
            if (subgroupElect()) {
                y[pa.ybase + r * pa.d + row] = total
            }
        }
    }
}

//! the routed down GEMV summed under the routing weights: a subgroup a row of the model dim, y[row] = sum_r w[r] dot_r
[vk_dispatch(name = "q51_gemv_sum_cls", grid = "wgs", params = "wgs : int64")]
class Q51GemvSum : Q51GemvBase {
    @ssbo @binding = 6 sw : array<float>   //! the k routing weights (the per-expert down scale folded in)

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q51_gemv_sum_cls_spv")]
    def run {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row < pa.d) {
            let nb = pa.n / 32u
            var tot = 0.0
            var r = 0u
            while (r < pa.nreg) {
                tot += sw[r] * row_dot(regions[r * 2u] + row * nb, regions[r * 2u + 1u], nb)
                r++
            }
            if (subgroupElect()) {
                y[pa.ybase + row] = tot
            }
        }
    }
}

//! the sdot4 of four MXFP4 nibbles (one per byte of `nib`) against four Q8_0 activation bytes: the doubled magnitudes, the sign applied by subtracting the negative bytes twice
def mx4_sdot(nib, x : uint) : int {
    let mag2 = mx4_mag2x4(nib)
    let neg = mag2 & (((nib >> 3u) & 0x01010101u) * 0xFFu)
    return sdot4(mag2, x) - 2 * sdot4(neg, x)
}

//! MXFP4 x Q8_0 decode GEMV, `q8_gemv_cls`'s shape with ONE lane per 32-block: `dot_mx4q8_scalar`'s float order, reassociated by lane
class Mx4GemvBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words, four per block
    @ssbo @binding = 1 wex : array<uint>       // per-block E8M0 exponent bytes, four blocks a word
    @ssbo @binding = 2 regions : array<uint>   // (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs

    //! one row's dot over `nb` blocks from weight block `wb0` and activation block `xb0`: a lane a block, the subgroup's sum
    def row_dot(wb0, xb0, nb : uint) : float {
        let lane = gl_SubgroupInvocationID
        var acc = 0.0
        var bi0 = 0u
        while (bi0 < nb) {
            let b = bi0 + lane
            if (b < nb) {
                let wbase = (wb0 + b) * 4u
                let xbase = (xb0 + b) * 8u
                var idot = 0
                for [unroll_full] (c in range(4)) {
                    let w = wq[wbase + uint(c)]
                    idot += mx4_sdot(w & 0x0F0F0F0Fu, xqw[xbase + uint(c)])
                    idot += mx4_sdot((w >> 4u) & 0x0F0F0F0Fu, xqw[xbase + 4u + uint(c)])
                }
                let sb = wb0 + b
                let ex = (wex[sb >> 2u] >> ((sb & 3u) * 8u)) & 0xFFu
                acc += float(idot) * mx4_e8m0_half(ex) * axs[xb0 + b]
            }
            bi0 += gl_SubgroupSize
        }
        return subgroupAdd(acc)
    }
}

[vk_dispatch(name = "mx4_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Mx4Gemv : Mx4GemvBase {
    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "mx4_gemv_cls_spv")]
    def run {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.nreg * pa.d) {
            let r = rg % pa.nreg
            let row = rg / pa.nreg
            let nb = pa.n / 32u
            let total = row_dot(regions[r * 2u] + row * nb, regions[r * 2u + 1u], nb)
            if (subgroupElect()) {
                y[pa.ybase + r * pa.d + row] = total
            }
        }
    }
}

//! bindings 10 and 11 sit at the hazard rail's cap (`SetBits` holds twelve): a thirteenth plane here is a rail change, not a line
[vk_dispatch(name = "mx4_gemv_gu_cls", grid = "wgs", params = "wgs : int64")]
class Mx4GemvGu : Mx4GemvBase {
    @ssbo @binding = 6 regions3 : array<uint>   // the up stack's (rel_wblock, xblock) pairs per slot
    @ssbo @binding = 7 @role = "weight" gb : array<float>   //! the gate-bias plane ([ne x d] rows a layer), read under `bias_on`
    @ssbo @binding = 8 @role = "weight" ub : array<float>   //! ... and the up-bias plane
    @ssbo @binding = 9 rex : array<uint>        //! slot -> its expert (the top-k's picks)
    @ssbo @binding = 10 outq : array<int8>      // the activated hidden rows' Q8_0 quants (the down GEMV's feed)
    @ssbo @binding = 11 outs : array<float>     // ... and their per-block scales
    @workgroup wg_g : float[32]   //! the block's gate dots
    @workgroup wg_u : float[32]   //! ... and up dots

    [spirv_kernel(local_size_x = 256, name = "mx4_gemv_gu_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
    def run {
        let row0 = gl_WorkGroupID.x * 32u
        let nsg = gl_NumSubgroups
        let rows_per = 32u / nsg   // four rows a subgroup at 32 lanes (eight subgroups), eight at 64: every row of the block in flight at once
        let nb = pa.n / 32u
        var t = 0u
        while (t < rows_per) {
            let lr = gl_SubgroupID + t * nsg
            let rg = row0 + lr
            var g = 0.0
            var u = 0.0
            if (rg < pa.nreg * pa.d) {
                let r = rg / pa.d
                let row = rg % pa.d
                let xb0 = regions[r * 2u + 1u]
                g = row_dot(regions[r * 2u] + row * nb, xb0, nb)
                u = row_dot(regions3[r * 2u] + row * nb, xb0, nb)
            }
            if (subgroupElect()) {
                wg_g[lr] = g
                wg_u[lr] = u
                y[pa.ybase + rg] = g   //! the gate rows land in the family's y as well: the dot half's own witness
            }
            t++
        }
        barrier()
        if (gl_SubgroupID == 0u) {   // the block's activation and Q8_0 quant: one lane an output row
            let lane = gl_SubgroupInvocationID
            let li = min(lane, 31u)
            let rg = row0 + li
            var gv = wg_g[li]
            var uv = wg_u[li]
            if (pa.bias_on != 0u) {   //! the slot's expert names the bias rows; the add before the activation is the biased requant kernel's
                let ex = rex[rg / pa.d]
                let c = rg % pa.d
                gv += gb[pa.gboff + ex * pa.d + c]
                uv += ub[pa.uboff + ex * pa.d + c]
            }
            let a = lane < 32u ? act_mul_sel(gv, uv, pa.act) : 0.0
            let dq = da_q8_blk_d(a)
            let id = dq != 0.0 ? 1.0 / dq : 0.0
            if (lane == 0u) {
                outs[gl_WorkGroupID.x] = dq
            }
            if (lane < 32u) {
                outq[row0 + lane] = q8_quant1(a, id)
            }
        }
    }
}

// ===== the K-quant decode GEMV family: one base shell, per-format block decodes =====
// ONE LANE PER 32-BLOCK via uint4 loads (one scale decode per block).
// The formats share the region walk, the lane->block map, the fold and the subgroup reduce — only
// the block's decode into packed int8 quads and fold terms differs, so each format is an override
// of blk_decode and the devirtualizer inlines the family back to standalone kernels at emission.

let private KQC_LOW = 0x0F0F0F0Fu   // nibble mask
let private KQC_ONES = 0x01010101u  // sdot4 against this sums 4 signed bytes

let KQ_GRID_IQ2S_WORDS = 2048u
let KQ_GRID_IQ2XS_WORDS = 1024u
let KQ_GRID_IQ2XXS_WORDS = 512u
let KQ_GRID_IQ3S_WORDS = 512u
let KQ_GRID_IQ3XXS_WORDS = 256u
let KQ_GRID_IQ2S = 0u
let KQ_GRID_IQ2XS = KQ_GRID_IQ2S + KQ_GRID_IQ2S_WORDS
let KQ_GRID_IQ2XXS = KQ_GRID_IQ2XS + KQ_GRID_IQ2XS_WORDS
let KQ_GRID_IQ3S = KQ_GRID_IQ2XXS + KQ_GRID_IQ2XXS_WORDS
let KQ_GRID_IQ3XXS = KQ_GRID_IQ3S + KQ_GRID_IQ3S_WORDS
let KQ_GRID_WORDS = int(KQ_GRID_IQ3XXS + KQ_GRID_IQ3XXS_WORDS)
let KQ_GRID_BYTES = int64(KQ_GRID_WORDS) * 4l

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#kq-gemv-grid-buffer")]
def kq_grid_dev() : uint64 {
    if (g_gpu.kq_grid_dev == 0ul) {
        var words : array<uint>
        words |> resize(KQ_GRID_WORDS)
        for (i in range(int(KQ_GRID_IQ2S_WORDS))) {
            words[int(KQ_GRID_IQ2S) + i] = iq2s_grid_word(i)
        }
        for (i in range(int(KQ_GRID_IQ2XS_WORDS))) {
            words[int(KQ_GRID_IQ2XS) + i] = iq2xs_grid_word(i)
        }
        for (i in range(int(KQ_GRID_IQ2XXS_WORDS))) {
            words[int(KQ_GRID_IQ2XXS) + i] = iq2xxs_grid_word(i)
            words[int(KQ_GRID_IQ3S) + i] = iq3s_grid_word(i)
        }
        for (i in range(int(KQ_GRID_IQ3XXS_WORDS))) {
            words[int(KQ_GRID_IQ3XXS) + i] = iq3xxs_grid_word(i)
        }
        g_gpu.kq_grid_dev = make_device_buf(KQ_GRID_BYTES)
        upload_region_at(g_gpu.kq_grid_dev, 0l, unsafe(addr<uint8 const?>(words[0])), KQ_GRID_BYTES)
    }
    return g_gpu.kq_grid_dev
}

class KqGemvBase {
    @ssbo @binding = 0 @role = "alias" wq : array<uint>   // weight quant words (k5's qh rides this view)
    @ssbo @binding = 0 wq4 : array<uint4>      // vec4 view of the same words
    @ssbo @binding = 1 wsu : array<uint>       // scale planes ([f16 d][f16 dmin][8 sc][8 mn] / q40 half pairs / k6 int8+d / iq4xs d + 8 signed sc)
    @ssbo @binding = 2 regions : array<uint>   // (rel_wblock, xblock) pairs per region — bulk data, stays SSBO
    @ssbo @binding = 3 xq4 : array<uint4>      // activation quant words, vec4 view
    @ssbo @binding = 4 axs : array<float>      // activation block scales
    @ssbo @binding = 5 y : array<float>        // output rows
    @ssbo @binding = 6 @role = "weight" gridb : array<uint>   // the grid codebooks (kq_grid_dev; the grid formats stage from it)
    @push_constant pa : GemvArgs

    //! fills wl / wh with the lo / hi int8 quads (x's lo / hi words), returns (A_lo, B_lo, A_hi, B_hi): the block adds axs x (A_lo dot_lo + B_lo bsum_lo + A_hi dot_hi + B_hi bsum_hi)
    def blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        return float4(0.0)
    }

    //! the fold carries B terms (a min or an offset against the block sums): the k-lattice leaves; the codebook and grid leaves skip the sums
    def fold_reads_bsum : bool => false

    def nib_split(wv : uint4; var wl : uint4&; var wh : uint4&) {
        wl = uint4(wv.x & KQC_LOW, wv.y & KQC_LOW, wv.z & KQC_LOW, wv.w & KQC_LOW)
        wh = uint4((wv.x >> 4u) & KQC_LOW, (wv.y >> 4u) & KQC_LOW, (wv.z >> 4u) & KQC_LOW, (wv.w >> 4u) & KQC_LOW)
    }

    def iq4_split(wv : uint4; var wl : uint4&; var wh : uint4&) {
        wl = uint4(iq4_word(wv.x & KQC_LOW), iq4_word(wv.y & KQC_LOW), iq4_word(wv.z & KQC_LOW), iq4_word(wv.w & KQC_LOW))
        wh = uint4(iq4_word((wv.x >> 4u) & KQC_LOW), iq4_word((wv.y >> 4u) & KQC_LOW), iq4_word((wv.z >> 4u) & KQC_LOW), iq4_word((wv.w >> 4u) & KQC_LOW))
    }

    def dm_terms(wsb, blk : uint) : float4 {
        let dm = unpackHalf2x16(wsu[wsb * 5u])
        let sc = float((wsu[wsb * 5u + 1u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFFu)
        let mn = float((wsu[wsb * 5u + 3u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFFu)
        return float4(dm.x * sc, -dm.y * mn, dm.x * sc, -dm.y * mn)
    }

    //! the decoded block's dot against activation block `xsb`; the one-column and N-column forms both add its result to
    //! their accumulator as a plain product, so the two modules compile one text and sum bit for bit - a `mad` here
    //! would be a fused multiply-add the CPU oracle's `mad` (a multiply, then an add) does not reproduce
    def blk_fold(wl, wh : uint4; terms : float4; xsb, blk : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        let dlo = sdot4(wl.x, xl.x) + sdot4(wl.y, xl.y) + sdot4(wl.z, xl.z) + sdot4(wl.w, xl.w)
        let dhi = sdot4(wh.x, xh.x) + sdot4(wh.y, xh.y) + sdot4(wh.z, xh.z) + sdot4(wh.w, xh.w)
        var f = mad(terms.z, float(dhi), terms.x * float(dlo))
        if (fold_reads_bsum()) {
            let blo = sdot4(xl.x, KQC_ONES) + sdot4(xl.y, KQC_ONES) + sdot4(xl.z, KQC_ONES) + sdot4(xl.w, KQC_ONES)
            let bhi = sdot4(xh.x, KQC_ONES) + sdot4(xh.y, KQC_ONES) + sdot4(xh.z, KQC_ONES) + sdot4(xh.w, KQC_ONES)
            f = mad(terms.w, float(bhi), mad(terms.y, float(blo), f))
        }
        return axs[xsb] * f
    }

    def blk_contrib(wsb, blk, xsb : uint) : float {
        var wl = uint4(0u)
        var wh = uint4(0u)
        let terms = blk_decode(wsb, blk, wl, wh)
        return blk_fold(wl, wh, terms, xsb, blk)
    }

    //! this lane's share of one row's dot: the row's `nsb` superblocks from weight superblock `wsb0` against the
    //! activation's from `xsb0`, the row's `lanes_per_row` lanes striding the 32-blocks, four a step then the tail
    def row_acc(wsb0, xsb0, nsb, lanes_per_row, row_lane : uint) : float {
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 + 4u * lanes_per_row <= nb) {
            let bb = b0 + row_lane
            acc += blk_contrib(wsb0 + bb / 8u, bb % 8u, xsb0 + bb / 8u)
            let b1 = bb + lanes_per_row
            acc += blk_contrib(wsb0 + b1 / 8u, b1 % 8u, xsb0 + b1 / 8u)
            let b2 = b1 + lanes_per_row
            acc += blk_contrib(wsb0 + b2 / 8u, b2 % 8u, xsb0 + b2 / 8u)
            let b3 = b2 + lanes_per_row
            acc += blk_contrib(wsb0 + b3 / 8u, b3 % 8u, xsb0 + b3 / 8u)
            b0 += 4u * lanes_per_row
        }
        while (b0 < nb) {
            let bb = b0 + row_lane
            if (bb < nb) {
                acc += blk_contrib(wsb0 + bb / 8u, bb % 8u, xsb0 + bb / 8u)
            }
            b0 += lanes_per_row
        }
        return acc
    }

    //! the row's fold is an xor-shuffle butterfly over its lanes (the shuffle every GEMV already requires;
    //! a clustered add would want the clustered subgroup feature the tier never checks)
    def fold_lanes(acc : float; lanes_per_row : uint) : float {
        var total = acc
        if (lanes_per_row == 8u) {
            total += subgroupShuffleXor(total, 4u)
            total += subgroupShuffleXor(total, 2u)
            total += subgroupShuffleXor(total, 1u)
        } elif (lanes_per_row == 16u) {
            total += subgroupShuffleXor(total, 8u)
            total += subgroupShuffleXor(total, 4u)
            total += subgroupShuffleXor(total, 2u)
            total += subgroupShuffleXor(total, 1u)
        } else {
            total = subgroupAdd(acc)
        }
        return total
    }

    [arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#kq-gemv-lanes")]
    def gemv_shell {
        let lanes_per_row = pa.lanes_per_row == 0u ? gl_SubgroupSize : pa.lanes_per_row
        let lane = gl_SubgroupInvocationID
        let row_lane = lane % lanes_per_row
        let rg = (gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID) * (gl_SubgroupSize / lanes_per_row) + lane / lanes_per_row
        let live = rg < pa.nreg * pa.d
        let nreg = max(pa.nreg, 1u)   //! the divisor guarded: a dead lane computes it too
        let r = rg % nreg
        let row = rg / nreg
        var acc = 0.0
        if (live) {
            let nsb = pa.n / 256u
            acc = row_acc(regions[r * 2u] + row * nsb, regions[r * 2u + 1u], nsb, lanes_per_row, row_lane)
        }
        let total = fold_lanes(acc, lanes_per_row)
        if (live && row_lane == 0u) {
            y[pa.ybase + r * pa.d + row] = total
        }
    }

    //! the N-column form: a lane's blocks decode once and dot against every column's activation row (column c's
    //! superblocks sit c x nsb past the region's), `pa.ncols` columns to 8, their y rows `pa.ystride` apart
    def gemv_shell_n {
        let lanes_per_row = pa.lanes_per_row == 0u ? gl_SubgroupSize : pa.lanes_per_row
        let lane = gl_SubgroupInvocationID
        let row_lane = lane % lanes_per_row
        let rg = (gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID) * (gl_SubgroupSize / lanes_per_row) + lane / lanes_per_row
        let live = rg < pa.nreg * pa.d
        var acc : float[8]
        for [unroll] (c in range(8)) {
            acc[c] = 0.0
        }
        if (live) {
            let r = rg / pa.d
            let row = rg % pa.d
            let nsb = pa.n / 256u
            let wsb0 = regions[r * 2u] + row * nsb
            let xsb0 = regions[r * 2u + 1u]
            let nb = nsb * 8u
            var b0 = row_lane
            while (b0 < nb) {
                let wsb = wsb0 + b0 / 8u
                let blk = b0 % 8u
                let xsb = xsb0 + b0 / 8u
                var wl = uint4(0u)
                var wh = uint4(0u)
                let terms = blk_decode(wsb, blk, wl, wh)
                for [unroll] (c in range(8)) {
                    if (uint(c) < pa.ncols) {   //! uniform on the push constant; the clamp form folds every dead column against a live row and costs 1.6x at four columns
                        acc[c] += blk_fold(wl, wh, terms, xsb + uint(c) * nsb, blk)
                    }
                }
                b0 += lanes_per_row
            }
        }
        for [unroll] (c in range(8)) {
            if (uint(c) < pa.ncols) {
                let total = fold_lanes(acc[c], lanes_per_row)
                if (live && row_lane == 0u) {
                    y[pa.ybase + uint(c) * pa.ystride + rg] = total
                }
            }
        }
    }
}

// ===== the q8 batch tile router (class side) =====
// The (mode, tile, aligned) pick as a variant ordinal — all six classes
// share the 6-buffer member layout + BatchArgs, but each carries its own set layout.
// variant: 0 = sdot4 tile, 1 = cmf16, 2 = cmi8, 3 = mm M-tile, 4 = mm aligned-L, 5 = mm L

def q8_batch_cls_variant(tile : int64; aligned : bool) : int {
    if (g_gpu.coopmat_mode == COOPMAT_F16) {
        return 1
    } elif (g_gpu.coopmat_mode == COOPMAT_INT8) {
        return 2
    } elif (g_gpu.coopmat_mode >= COOPMAT_MM) {
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
    } elif (fmt == int(KqFmt.q51)) {
        return ensure_q51_gemv_cls()
    } elif (fmt == int(KqFmt.mx4)) {
        return ensure_mx4_gemv_cls()
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
    } elif (fmt == int(KqFmt.q51)) {
        return set_q51_gemv_cls(bufs, sizes, gbits)
    } elif (fmt == int(KqFmt.mx4)) {
        return set_mx4_gemv_cls(bufs, sizes, gbits)
    } elif (kq_sb(fmt)) {
        // the family shares one layout — one set serves every kq format
        return set_kq_gemv_cls(fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5], kq_grid_dev()),
            fixed_array(sizes[0], sizes[1], sizes[2], sizes[3], sizes[4], sizes[5], KQ_GRID_BYTES),
            fixed_array(gbits[0], gbits[1], gbits[2], gbits[3], gbits[4], gbits[5], 0u))
    }
    panic("dasLLAMA vulkan tier: no GEMV class for format {fmt}")
    return VkDescriptorSet()
}

def gemv_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {
    if (fmt == int(KqFmt.q8)) {
        enc_q8_gemv_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.q51)) {
        enc_q51_gemv_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.mx4)) {
        enc_mx4_gemv_cls(raw, h, set_, pc, wgs)
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

//! the GEMV family's dispatch grid over `rows` output rows at `lanes_per_row` (0 = a whole subgroup per
//! row, the q8 GEMV's only form): a workgroup's subgroups each take subgroup_size / lanes_per_row rows
def gemv_wgs(rows : int64; lanes_per_row : uint) : int64 {
    let l = lanes_per_row == 0u ? g_gpu.subgroup_size : int64(lanes_per_row)
    let rpw = g_gpu.rows_per_wg * (g_gpu.subgroup_size / l)
    return (rows + rpw - 1l) / rpw
}

//! the codebook and grid formats: a block's decode gathers, so a lane wants more blocks in flight than the k-lattice's
def gemv_grid_fmt(fmt : int) : bool {
    return fmt == int(KqFmt.iq2s) || fmt == int(KqFmt.iq2xs) || fmt == int(KqFmt.iq2xxs) || fmt == int(KqFmt.iq3xxs) || fmt == int(KqFmt.iq4xs) || fmt == int(KqFmt.iq4nl)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#kq-gemv-lanes")]
def gemv_lanes_per_row(fmt : int; n : int64) : uint {
    if (kq_block32(fmt)) return 0u   //! the per-32 formats (q8, q51, mx4): a whole subgroup a row, a lane a block
    let nb = n / 32l
    let grid = gemv_grid_fmt(fmt)
    if (nb <= 24l) return 8u
    if (nb <= 48l) return grid ? 8u : 16u
    if (nb <= 96l) return 16u
    return grid ? 16u : 0u
}

//! the family's dispatch over `rows` output rows: the lanes per row by the row length, the grid to match; `pc.ncols`
//! past one takes the N-column leaf over that many activation rows, its y rows `pc.ystride` apart
def gemv_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; rows : int64) {
    pc.lanes_per_row = gemv_lanes_per_row(fmt, int64(pc.n))
    let wgs = gemv_wgs(rows, pc.lanes_per_row)
    if (pc.ncols > 1u) {
        gemv_cls_enc_n(fmt, raw, h, set_, pc, wgs)
    } else {
        gemv_cls_enc(fmt, raw, h, set_, pc, wgs)
    }
}

// ===== the N-column GEMV leaves: one weight pass over `pa.ncols` activation rows =====

//! the formats with an N-column leaf: every kq leaf and the q8 stamps; the per-32 expert GEMVs (q51, mx4) have no N form, so a dense plane in one steps a row at a time
def gemv_cls_has_n(fmt : int) : bool => fmt == int(KqFmt.q8) || kq_sb(fmt)

//! the decode GEMV family's roster - q8 and every kq leaf, in enum order: what the kernel cells and the probe's ruler walk
def kq_gemv_fmts() : array<KqFmt> {
    return <- [for (f in type<KqFmt>); f; where gemv_cls_has_n(int(f))]
}

//! the reduction width past which the q8 N-column form pairs output rows a subgroup; off until a box's token command proves the pair
var g_q8_n2_min_n = 0xFFFFFFFFu

//! the q8 row-pair form is armed: `g_q8_n2_min_n` sits under the widest reduction
def q8_pair_armed() : bool => g_q8_n2_min_n != 0xFFFFFFFFu

//! the N-column leaves of `fmt` for up to `nb` columns: the q8 stamps the ladder can pick, the pair twins only while armed
def gemv_cls_ensure_n(fmt : int; nb : int64 = 8l) : bool {
    if (fmt == int(KqFmt.q8)) {
        let pair = q8_pair_armed()
        var ok = ensure_q8_gemv_n2_cls() && (!pair || ensure_q8_gemv_n2p_cls())
        if (nb > 2l) {
            ok = ok && ensure_q8_gemv_n4_cls() && (!pair || ensure_q8_gemv_n4p_cls())
        }
        if (nb > 4l) {
            ok = ok && ensure_q8_gemv_n8_cls() && (!pair || ensure_q8_gemv_n8p_cls())
        }
        return ok
    } elif (fmt == int(KqFmt.k4)) {
        return ensure_kq_gemv_k4_n_cls()
    } elif (fmt == int(KqFmt.k5)) {
        return ensure_kq_gemv_k5_n_cls()
    } elif (fmt == int(KqFmt.k6)) {
        return ensure_kq_gemv_k6_n_cls()
    } elif (fmt == int(KqFmt.q40)) {
        return ensure_kq_gemv_q40_n_cls()
    } elif (fmt == int(KqFmt.iq4xs)) {
        return ensure_kq_gemv_iq4xs_n_cls()
    } elif (fmt == int(KqFmt.k3)) {
        return ensure_kq_gemv_k3_n_cls()
    } elif (fmt == int(KqFmt.iq3s)) {
        return ensure_kq_gemv_iq3s_n_cls()
    } elif (fmt == int(KqFmt.iq3xxs)) {
        return ensure_kq_gemv_iq3xxs_n_cls()
    } elif (fmt == int(KqFmt.iq4nl)) {
        return ensure_kq_gemv_iq4nl_n_cls()
    } elif (fmt == int(KqFmt.k2)) {
        return ensure_kq_gemv_k2_n_cls()
    } elif (fmt == int(KqFmt.iq2s)) {
        return ensure_kq_gemv_iq2s_n_cls()
    } elif (fmt == int(KqFmt.iq2xs)) {
        return ensure_kq_gemv_iq2xs_n_cls()
    } elif (fmt == int(KqFmt.iq2xxs)) {
        return ensure_kq_gemv_iq2xxs_n_cls()
    }
    panic("dasLLAMA vulkan tier: no N-column GEMV class for format {fmt}")
    return false
}

//! the q8 stamps share one layout of their own, so the narrowest stamp's set - the one every row count builds - binds them all; a kq leaf shares the family's
def gemv_cls_set_n(fmt : int; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (fmt == int(KqFmt.q8)) {
        return set_q8_gemv_n2_cls(bufs, sizes, gbits)
    }
    return gemv_cls_set(fmt, bufs, sizes, gbits)
}

//! the q8 stamp for `pc.ncols` columns: the narrowest that holds them, the row-pair form past `g_q8_n2_min_n` on an even row count
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private q8_gemv_n_enc(raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {
    let pair_wanted = q8_pair_armed() && pc.n >= g_q8_n2_min_n && pc.d % 2u == 0u
    let g2 = (wgs + 1l) / 2l   //! a subgroup a row pair: half the row grid
    if (pc.ncols <= 2u) {   //! each pair twin builds on first use, since the lever can move after the family's ensure ran
        if (pair_wanted && ensure_q8_gemv_n2p_cls()) {
            enc_q8_gemv_n2p_cls(raw, h, set_, pc, g2)
        } else {
            enc_q8_gemv_n2_cls(raw, h, set_, pc, wgs)
        }
    } elif (pc.ncols <= 4u) {
        if (pair_wanted && ensure_q8_gemv_n4p_cls()) {
            enc_q8_gemv_n4p_cls(raw, h, set_, pc, g2)
        } else {
            enc_q8_gemv_n4_cls(raw, h, set_, pc, wgs)
        }
    } elif (pair_wanted && ensure_q8_gemv_n8p_cls()) {
        enc_q8_gemv_n8p_cls(raw, h, set_, pc, g2)
    } else {
        enc_q8_gemv_n8_cls(raw, h, set_, pc, wgs)
    }
}

def gemv_cls_enc_n(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {
    if (fmt == int(KqFmt.q8)) {
        q8_gemv_n_enc(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k4)) {
        enc_kq_gemv_k4_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k5)) {
        enc_kq_gemv_k5_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k6)) {
        enc_kq_gemv_k6_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.q40)) {
        enc_kq_gemv_q40_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq4xs)) {
        enc_kq_gemv_iq4xs_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k3)) {
        enc_kq_gemv_k3_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq3s)) {
        enc_kq_gemv_iq3s_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq3xxs)) {
        enc_kq_gemv_iq3xxs_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq4nl)) {
        enc_kq_gemv_iq4nl_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.k2)) {
        enc_kq_gemv_k2_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2s)) {
        enc_kq_gemv_iq2s_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2xs)) {
        enc_kq_gemv_iq2xs_n_cls(raw, h, set_, pc, wgs)
    } elif (fmt == int(KqFmt.iq2xxs)) {
        enc_kq_gemv_iq2xxs_n_cls(raw, h, set_, pc, wgs)
    } else {
        panic("dasLLAMA vulkan tier: no N-column GEMV class for format {fmt}")
    }
}

// Q4_K: w = d*sc*q - dmin*mn*1, q in [0,15] — per 32-block axs*(d*sc*idot - dmin*mn*bsum).
// Every kq format shares family=kq_gemv_cls: ONE set layout + ONE set_kq_gemv_cls serves every
// pipeline (identical inherited members), each leaf keeping its own ensure_/enc_ names.
//! every decode GEMV leaf's shape: the format's decode on a `<Fmt>GemvT` template, its one-column and N-column stamps
//! two overrides of `NCOL`, and a grid format's codebook staged into workgroup memory ahead of either shell
[ |> template_struct_instance]
class template KqGemvLeafT : KqGemvBase {
    @template_constant NCOL : bool = false      //!< the N-column shell (`gemv_shell_n`) in place of the one-column one
    @template_constant GRID : bool = false      //!< a grid-codebook format: `GRID_WORDS` words of `gridb` from `GRID_OFF` stage into `gridw` ahead of the shell
    @template_constant GRID_WORDS : uint = 64u  //!< the codebook's words and its first word in `gridb` - literals (template constants), held equal to the format's `KQ_GRID_<FMT>_WORDS` / `KQ_GRID_<FMT>` by REVIEW.das
    @template_constant GRID_OFF : uint = 0u
    @workgroup @template_gate = GRID gridw : uint[int(GRID_WORDS)]

    def stage_grid {
        static_if (GRID) {
            let lid = gl_LocalInvocationID.x
            for (k in range(int(GRID_WORDS / 64u))) {
                let idx = lid + uint(k) * 64u
                gridw[idx] = gridb[GRID_OFF + idx]
            }
            barrier()
        }
    }

    [spirv_kernel(local_size_x = 64)]
    def run {
        static_if (GRID) {
            stage_grid()
        }
        static_if (NCOL) {
            gemv_shell_n()
        } else {
            gemv_shell()
        }
    }
}

[ |> template_struct_instance]
class template K4GemvT : KqGemvLeafT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        nib_split(wq4[wsb * 8u + blk], wl, wh)
        return dm_terms(wsb, blk)
    }

    def override fold_reads_bsum : bool => true
}

[vk_dispatch(name = "kq_gemv_k4_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK4 : K4GemvT {}

[vk_dispatch(name = "kq_gemv_k4_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK4N : K4GemvT {
    override NCOL = true
}

//! the routed experts' Q4_K gate and up GEMVs, the activation and its Q8_0 requant in one dispatch (`Q8GemvGu`'s form
//! over the slot regions): the gate stack's regions at binding 2, the up stack's at 7, the one feed row
[vk_dispatch(name = "kq_gemv_k4_gu_cls", kernel = "run_gu", grid = "wgs", params = "wgs : int64")]
class KqGemvK4Gu : K4GemvT {
    @ssbo @binding = 7 regions3 : array<uint>   // the up stack's (rel_wblock, xblock) pairs per slot
    @ssbo @binding = 8 outq : array<int8>       // the activated hidden rows' Q8_0 quants (the down GEMV's feed)
    @ssbo @binding = 9 outs : array<float>      // ... and their per-block scales
    @workgroup wg_g : float[32]   //! the block's gate dots
    @workgroup wg_u : float[32]   //! ... and up dots

    [spirv_kernel(local_size_x = 256, name = "kq_gemv_k4_gu_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
    def run_gu {
        let lanes_per_row = pa.lanes_per_row == 0u ? gl_SubgroupSize : pa.lanes_per_row
        let lane = gl_SubgroupInvocationID
        let row_lane = lane % lanes_per_row
        let per_sg = gl_SubgroupSize / lanes_per_row   // the rows a subgroup holds at once
        let per_turn = gl_NumSubgroups * per_sg        // ... and the workgroup
        let row0 = gl_WorkGroupID.x * 32u
        let nsb = pa.n / 256u
        var t = 0u
        while (t * per_turn < 32u) {
            let lr = t * per_turn + gl_SubgroupID * per_sg + lane / lanes_per_row
            let rg = row0 + lr
            let live = lr < 32u && rg < pa.nreg * pa.d
            var g = 0.0
            var u = 0.0
            if (live) {
                let r = rg / pa.d
                let row = rg % pa.d
                let xsb0 = regions[r * 2u + 1u]
                g = row_acc(regions[r * 2u] + row * nsb, xsb0, nsb, lanes_per_row, row_lane)
                u = row_acc(regions3[r * 2u] + row * nsb, xsb0, nsb, lanes_per_row, row_lane)
            }
            g = fold_lanes(g, lanes_per_row)
            u = fold_lanes(u, lanes_per_row)
            if (live && row_lane == 0u) {
                wg_g[lr] = g
                wg_u[lr] = u
                y[pa.ybase + rg] = g   //! the gate rows land in the family's y as well: the dot half's own witness
            }
            t++
        }
        barrier()
        if (gl_SubgroupID == 0u) {   // the block's activation and Q8_0 quant: one lane an output row
            let li = min(lane, 31u)
            let a = lane < 32u ? act_mul_sel(wg_g[li], wg_u[li], pa.act) : 0.0
            let dq = da_q8_blk_d(a)
            let id = dq != 0.0 ? 1.0 / dq : 0.0
            if (lane == 0u) {
                outs[gl_WorkGroupID.x] = dq
            }
            if (lane < 32u) {
                outq[row0 + lane] = q8_quant1(a, id)
            }
        }
    }
}

// Q4_0: w = d*(q - 8) — the k4 nibble tiling with no min plane; -8 folds against the block sum
[ |> template_struct_instance]
class template Q40GemvT : KqGemvLeafT {
    def q40_d(wsb, blk : uint) : float {
        let dp = unpackHalf2x16(wsu[wsb * 5u + blk / 2u])
        return blk % 2u == 0u ? dp.x : dp.y
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        nib_split(wq4[wsb * 8u + blk], wl, wh)
        let d = q40_d(wsb, blk)
        return float4(d, -8.0 * d, d, -8.0 * d)
    }

    def override fold_reads_bsum : bool => true
}

[vk_dispatch(name = "kq_gemv_q40_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvQ40 : Q40GemvT {}

[vk_dispatch(name = "kq_gemv_q40_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvQ40N : Q40GemvT {
    override NCOL = true
}

// IQ4_XS: w = d*sc*LUT[q] — the q40 nibble tiling decoded through the codebook into signed
// lanes (no block-sum term), the k4 scale-row shape with SIGNED (ls - 32) sub-scales and no min
[ |> template_struct_instance]
class template Iq4xsGemvT : KqGemvLeafT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        iq4_split(wq4[wsb * 8u + blk], wl, wh)
        let w0 = wsu[wsb * 2u]
        let a = unpackHalf2x16(w0).x * float(iq4xs_sc(w0, wsu[wsb * 2u + 1u], blk))
        return float4(a, 0.0, a, 0.0)
    }
}

[vk_dispatch(name = "kq_gemv_iq4xs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4xs : Iq4xsGemvT {}

[vk_dispatch(name = "kq_gemv_iq4xs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4xsN : Iq4xsGemvT {
    override NCOL = true
}

// IQ3_S: grid gather (8-bit index | qh ninth bit -> 4 magnitudes per word), the block's own
// sign bytes flip them, per-32 signed (1 + 2s) sub-scale x d - iq4xs's fold (no bsum term)
// over the 26-word rows; the 2 KB grid stages into workgroup memory ahead of the shell.
[ |> template_struct_instance]
class template Iq3sGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 512u
    override GRID_OFF = 3584u

    def iq3s_quad(qsw, qh, sgw, l0 : uint) : uint4 {
        let sg0 = (sgw >> (l0 * 8u)) & 255u
        let sg1 = (sgw >> (l0 * 8u + 8u)) & 255u
        return uint4(iq3s_signed(gridw[(qsw & 255u) | ((qh << (8u - 2u * l0)) & 256u)], sg0 & 15u),
                     iq3s_signed(gridw[((qsw >> 8u) & 255u) | ((qh << (7u - 2u * l0)) & 256u)], sg0 >> 4u),
                     iq3s_signed(gridw[((qsw >> 16u) & 255u) | ((qh << (8u - 2u * (l0 + 1u))) & 256u)], sg1 & 15u),
                     iq3s_signed(gridw[(qsw >> 24u) | ((qh << (7u - 2u * (l0 + 1u))) & 256u)], sg1 >> 4u))
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let qb = wsb * 26u + blk * 2u
        let qh = (wq[wsb * 26u + 16u + blk / 4u] >> ((blk % 4u) * 8u)) & 255u
        let sgw = wq[wsb * 26u + 18u + blk]
        wl = iq3s_quad(wq[qb], qh, sgw, 0u)
        wh = iq3s_quad(wq[qb + 1u], qh, sgw, 2u)
        let a = unpackHalf2x16(wsu[wsb * 2u]).x * float(grid_sc(wsu[wsb * 2u + 1u], blk))
        return float4(a, 0.0, a, 0.0)
    }
}

[vk_dispatch(name = "kq_gemv_iq3s_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3s : Iq3sGemvT {}

[vk_dispatch(name = "kq_gemv_iq3s_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3sN : Iq3sGemvT {
    override NCOL = true
}

// IQ3_XXS: halved-grid gather (bare 8-bit index -> 4 magnitudes per word), signs from the
// aux32's four 7-bit ksigns indices (bit 7 = parity, computed - no table), per-32 (2ls + 1)
// sub-scale x halved d - iq3s's fold exactly, over the 24-word rows; the 1 KB grid stages
// into workgroup memory ahead of the shell.
[ |> template_struct_instance]
class template Iq3xxsGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 256u
    override GRID_OFF = 4096u

    def iq3xxs_quad(qsw, aux, l0 : uint) : uint4 {
        let sg0 = ksign7((aux >> (7u * l0)) & 127u)
        let sg1 = ksign7((aux >> (7u * l0 + 7u)) & 127u)
        return uint4(iq3s_signed(gridw[qsw & 255u], sg0 & 15u), iq3s_signed(gridw[(qsw >> 8u) & 255u], sg0 >> 4u),
                     iq3s_signed(gridw[(qsw >> 16u) & 255u], sg1 & 15u), iq3s_signed(gridw[qsw >> 24u], sg1 >> 4u))
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let qb = wsb * 24u + blk * 2u
        let aux = wq[wsb * 24u + 16u + blk]
        wl = iq3xxs_quad(wq[qb], aux, 0u)
        wh = iq3xxs_quad(wq[qb + 1u], aux, 2u)
        let a = unpackHalf2x16(wsu[wsb * 2u]).x * float(grid_sc(wsu[wsb * 2u + 1u], blk))
        return float4(a, 0.0, a, 0.0)
    }

}

[vk_dispatch(name = "kq_gemv_iq3xxs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3xxs : Iq3xxsGemvT {}

[vk_dispatch(name = "kq_gemv_iq3xxs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3xxsN : Iq3xxsGemvT {
    override NCOL = true
}

// IQ4_NL: iq4xs's codebook dot (iq4_word, pure ALU) folded by q40's per-block f16 d - no
// sub-scales, no bsum term; the rows are q40's 32-word planes verbatim.
[ |> template_struct_instance]
class template Iq4nlGemvT : Q40GemvT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        iq4_split(wq4[wsb * 8u + blk], wl, wh)
        let d = q40_d(wsb, blk)
        return float4(d, 0.0, d, 0.0)
    }

    def override fold_reads_bsum : bool => false
}

[vk_dispatch(name = "kq_gemv_iq4nl_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4nl : Iq4nlGemvT {}

[vk_dispatch(name = "kq_gemv_iq4nl_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4nlN : Iq4nlGemvT {
    override NCOL = true
}

// Q3_K: q = 2-bit qs lane | hmask bit << 2, minus 4, per-16 SIGNED sub-scales x d — k6's fold with
// the offset 4 over the k3 planes (6 uint4 per superblock: qs 0..3, hmask 4..5)
[ |> template_struct_instance]
class template K3GemvT : KqGemvLeafT {
    def k3_quad(q, h : uint4; sh, bit : uint) : uint4 {
        let m2 = 0x03030303u
        let m1 = 0x01010101u
        return uint4(((q.x >> sh) & m2) | (((h.x >> bit) & m1) << 2u), ((q.y >> sh) & m2) | (((h.y >> bit) & m1) << 2u),
                     ((q.z >> sh) & m2) | (((h.z >> bit) & m1) << 2u), ((q.w >> sh) & m2) | (((h.w >> bit) & m1) << 2u))
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let q4 = wsb * 6u + (blk / 4u) * 2u
        let sh = (blk % 4u) * 2u
        wl = k3_quad(wq4[q4], wq4[wsb * 6u + 4u], sh, blk)
        wh = k3_quad(wq4[q4 + 1u], wq4[wsb * 6u + 5u], sh, blk)
        let s4 = int4(unpack8(int(wsu[wsb * 5u + blk / 2u])))   // 4 signed sub-scales
        let slo = float((blk % 2u) == 0u ? s4.x : s4.z)
        let shi = float((blk % 2u) == 0u ? s4.y : s4.w)
        let dd = unpackHalf2x16(wsu[wsb * 5u + 4u]).x
        return float4(dd * slo, -4.0 * dd * slo, dd * shi, -4.0 * dd * shi)
    }

    def override fold_reads_bsum : bool => true
}

[vk_dispatch(name = "kq_gemv_k3_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK3 : K3GemvT {}

[vk_dispatch(name = "kq_gemv_k3_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK3N : K3GemvT {
    override NCOL = true
}

// Q2_K: q = the unsigned 2-bit qs lane, per-16 sc/min NIBBLE pairs x d/dmin - the k4 d|dmin
// fold at k6's granularity over the 16-word rows (4 uint4 of qs; scale row [d|dmin][16 pair bytes])
[ |> template_struct_instance]
class template K2GemvT : KqGemvLeafT {
    def k2_quad(q : uint4; sh : uint) : uint4 {
        let m2 = 0x03030303u
        return uint4((q.x >> sh) & m2, (q.y >> sh) & m2, (q.z >> sh) & m2, (q.w >> sh) & m2)
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let q4 = wsb * 4u + (blk / 4u) * 2u
        let sh = (blk % 4u) * 2u
        wl = k2_quad(wq4[q4], sh)
        wh = k2_quad(wq4[q4 + 1u], sh)
        let w = wsu[wsb * 5u + 1u + blk / 2u]
        let b0 = (w >> ((blk % 2u) * 16u)) & 255u
        let b1 = (w >> ((blk % 2u) * 16u + 8u)) & 255u
        let dm = unpackHalf2x16(wsu[wsb * 5u])
        return float4(dm.x * float(b0 & 15u), -dm.y * float(b0 >> 4u), dm.x * float(b1 & 15u), -dm.y * float(b1 >> 4u))
    }

    def override fold_reads_bsum : bool => true
}

[vk_dispatch(name = "kq_gemv_k2_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK2 : K2GemvT {}

[vk_dispatch(name = "kq_gemv_k2_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK2N : K2GemvT {
    override NCOL = true
}

// Q5_K: the k4 fold with the 5th bit deposited from the block's qh word
[ |> template_struct_instance]
class template K5GemvT : KqGemvLeafT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let wv = wq4[wsb * 10u + blk]
        let qh = wq[wsb * 40u + 32u + blk]
        let h0 = qh & 0xFFu
        let h1 = (qh >> 8u) & 0xFFu
        let h2 = (qh >> 16u) & 0xFFu
        let h3 = (qh >> 24u) & 0xFFu
        wl = uint4((wv.x & KQC_LOW) | k5_dep(h0 & 0xFu), (wv.y & KQC_LOW) | k5_dep(h1 & 0xFu),
                   (wv.z & KQC_LOW) | k5_dep(h2 & 0xFu), (wv.w & KQC_LOW) | k5_dep(h3 & 0xFu))
        wh = uint4(((wv.x >> 4u) & KQC_LOW) | k5_dep(h0 >> 4u), ((wv.y >> 4u) & KQC_LOW) | k5_dep(h1 >> 4u),
                   ((wv.z >> 4u) & KQC_LOW) | k5_dep(h2 >> 4u), ((wv.w >> 4u) & KQC_LOW) | k5_dep(h3 >> 4u))
        return dm_terms(wsb, blk)
    }

    def override fold_reads_bsum : bool => true
}

[vk_dispatch(name = "kq_gemv_k5_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK5 : K5GemvT {}

[vk_dispatch(name = "kq_gemv_k5_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK5N : K5GemvT {
    override NCOL = true
}

// Q6_K: q = (ql | qh<<4) - 32 with per-16 SIGNED int8 sub-scales; the lo/hi nibble halves of a
// block are exactly its two 16-weight sub-scale groups, so each keeps its own dot + sum
[ |> template_struct_instance]
class template K6GemvT : KqGemvLeafT {
    def k6_quad(nib, qh : uint4; shift : uint) : uint4 {
        let m2 = 0x03030303u
        return uint4(nib.x | (((qh.x >> shift) & m2) << 4u), nib.y | (((qh.y >> shift) & m2) << 4u),
                     nib.z | (((qh.z >> shift) & m2) << 4u), nib.w | (((qh.w >> shift) & m2) << 4u))
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let qh4 = wsb * 12u + 8u + (blk / 4u) * 2u
        let shift = (blk % 4u) * 2u
        var nl = uint4(0u)
        var nh = uint4(0u)
        nib_split(wq4[wsb * 12u + blk], nl, nh)
        wl = k6_quad(nl, wq4[qh4], shift)
        wh = k6_quad(nh, wq4[qh4 + 1u], shift)
        let s4 = int4(unpack8(int(wsu[wsb * 5u + blk / 2u])))   // 4 signed sub-scales
        let slo = float((blk % 2u) == 0u ? s4.x : s4.z)
        let shi = float((blk % 2u) == 0u ? s4.y : s4.w)
        let dd = unpackHalf2x16(wsu[wsb * 5u + 4u]).x
        return float4(dd * slo, -32.0 * dd * slo, dd * shi, -32.0 * dd * shi)
    }

    def override fold_reads_bsum : bool => true
}

[vk_dispatch(name = "kq_gemv_k6_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK6 : K6GemvT {}

[vk_dispatch(name = "kq_gemv_k6_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK6N : K6GemvT {
    override NCOL = true
}

// IQ2_S: the iq3s gather widened to the u64 grid - a 10-bit qs|qh index picks TWO grid words
// (eight magnitudes {8, 25, 43}), the block's own sign bytes flip them; per-16 UNSIGNED
// strips (1 + 2s) keep split lo/hi dots under the eighth-ed d. The 8 KB grid stages ahead.
[ |> template_struct_instance]
class template Iq2sGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 2048u
    override GRID_OFF = 0u

    def iq2s_quad(qw, qh, sgw, l0 : uint) : uint4 {
        let sg0 = (sgw >> (l0 * 8u)) & 255u
        let sg1 = (sgw >> (l0 * 8u + 8u)) & 255u
        let i0 = (((qw >> (l0 * 8u)) & 255u) | ((qh << (8u - 2u * l0)) & 0x300u)) * 2u
        let i1 = (((qw >> (l0 * 8u + 8u)) & 255u) | ((qh << (8u - 2u * (l0 + 1u))) & 0x300u)) * 2u
        return uint4(iq3s_signed(gridw[i0], sg0 & 15u), iq3s_signed(gridw[i0 + 1u], sg0 >> 4u),
                     iq3s_signed(gridw[i1], sg1 & 15u), iq3s_signed(gridw[i1 + 1u], sg1 >> 4u))
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let qw = wq[wsb * 18u + blk]
        let sgw = wq[wsb * 18u + 8u + blk]
        let qh = (wq[wsb * 18u + 16u + blk / 4u] >> ((blk % 4u) * 8u)) & 255u
        wl = iq2s_quad(qw, qh, sgw, 0u)
        wh = iq2s_quad(qw, qh, sgw, 2u)
        let sw = wsu[wsb * 5u + 1u + blk / 2u]
        let s0 = float((sw >> ((blk % 2u) * 16u)) & 255u)
        let s1 = float((sw >> ((blk % 2u) * 16u + 8u)) & 255u)
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return float4(dd * s0, 0.0, dd * s1, 0.0)
    }

}

[vk_dispatch(name = "kq_gemv_iq2s_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2s : Iq2sGemvT {}

[vk_dispatch(name = "kq_gemv_iq2s_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2sN : Iq2sGemvT {
    override NCOL = true
}

// IQ2_XS: the iq2s u64-grid walk with ksigns-by-parity - each u16 qs word carries a 9-bit
// grid index (TWO staged words) and a 7-bit KSIGNS_IQ2XS index whose sign byte ksign7
// recomputes (no sign plane, no qh); per-16 UNSIGNED strips under the eighth-ed d.
[ |> template_struct_instance]
class template Iq2xsGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 1024u
    override GRID_OFF = 2048u

    def iq2xs_quad(qw : uint) : uint4 {
        let wa = qw & 0xFFFFu
        let wb = qw >> 16u
        let sg0 = ksign7(wa >> 9u)
        let sg1 = ksign7(wb >> 9u)
        let i0 = (wa & 511u) * 2u
        let i1 = (wb & 511u) * 2u
        return uint4(iq3s_signed(gridw[i0], sg0 & 15u), iq3s_signed(gridw[i0 + 1u], sg0 >> 4u),
                     iq3s_signed(gridw[i1], sg1 & 15u), iq3s_signed(gridw[i1 + 1u], sg1 >> 4u))
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        wl = iq2xs_quad(wq[wsb * 16u + blk * 2u])
        wh = iq2xs_quad(wq[wsb * 16u + blk * 2u + 1u])
        let sw = wsu[wsb * 5u + 1u + blk / 2u]
        let s0 = float((sw >> ((blk % 2u) * 16u)) & 255u)
        let s1 = float((sw >> ((blk % 2u) * 16u + 8u)) & 255u)
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return float4(dd * s0, 0.0, dd * s1, 0.0)
    }

}

[vk_dispatch(name = "kq_gemv_iq2xs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xs : Iq2xsGemvT {}

[vk_dispatch(name = "kq_gemv_iq2xs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xsN : Iq2xsGemvT {
    override NCOL = true
}

// IQ2_XXS: the iq3xxs GEMV shell over the two-word u64 grid - block b's grid word carries
// four BYTE indices (two staged words each), its aux32 the parity-derived ksigns; the per-32
// (1 + 2s) strip under the eighth-ed d reads exactly through the shared signed extract.
[ |> template_struct_instance]
class template Iq2xxsGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 512u
    override GRID_OFF = 3072u

    def iq2xxs_quad(gw, aux, hu : uint) : uint4 {
        let b0 = (gw >> (16u * hu)) & 255u
        let b1 = (gw >> (16u * hu + 8u)) & 255u
        let sg0 = ksign7((aux >> (14u * hu)) & 127u)
        let sg1 = ksign7((aux >> (14u * hu + 7u)) & 127u)
        return uint4(iq3s_signed(gridw[b0 * 2u], sg0 & 15u), iq3s_signed(gridw[b0 * 2u + 1u], sg0 >> 4u),
                     iq3s_signed(gridw[b1 * 2u], sg1 & 15u), iq3s_signed(gridw[b1 * 2u + 1u], sg1 >> 4u))
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let gw = wq[wsb * 16u + blk * 2u]
        let aux = wq[wsb * 16u + blk * 2u + 1u]
        wl = iq2xxs_quad(gw, aux, 0u)
        wh = iq2xxs_quad(gw, aux, 1u)
        let a = unpackHalf2x16(wsu[wsb * 2u]).x * float(grid_sc(wsu[wsb * 2u + 1u], blk))
        return float4(a, 0.0, a, 0.0)
    }

}

[vk_dispatch(name = "kq_gemv_iq2xxs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xxs : Iq2xxsGemvT {}

[vk_dispatch(name = "kq_gemv_iq2xxs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xxsN : Iq2xxsGemvT {
    override NCOL = true
}
