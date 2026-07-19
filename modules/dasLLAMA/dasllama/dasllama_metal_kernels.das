options gen2
options _comment_hygiene = true

module dasllama_metal_kernels shared public

require math
require metal/msl_shader
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_metal_prefill
require dasllama/dasllama_metal_common

// The family-agnostic dasMetal kernel + PSO registry (the format-matrix arc's W1 split): every
// [metal_kernel] class the resident decode drivers dispatch — GEMV/GEMM/mul_mm forms, the
// single-query attention family across all four KV codecs (f16/f32/q8_0/tq4), the fused
// rope+KV-store family, add+rms/ew — plus their enc_* dispatch helpers and the PSO lifecycle
// (metal_decode_init / metal_kernels_release). The rms/add/swiglu/mul_mm MSL sources are the
// prefill module's, verbatim (own PSOs, zero copy drift). Kernel-shape rationale lives with
// each class; the driver contracts live in dasllama_metal_llama's header.

// ===== the new kernels =====

// One threadgroup per (stream, head), head_size threads — lane owns one V column. Pass 1 scores
// the context simdgroup-cooperatively so every K load coalesces (per-lane-per-row form gathered
// cache lines 2KB apart — measured 27us/layer at depth 24). Pass 2 softmax, pass 3 V-accumulate.
class MetalSqAttnF16 {
    @ssbo @binding = 0 kc : array<float16>  // this layer's K mirror slab (bound at the layer's byte offset)
    @ssbo @binding = 1 vc : array<float16>
    @ssbo @binding = 2 q : array<float>     // [B x qd] roped queries
    @ssbo @binding = 3 yo : array<float>    // [B x qd] attention out
    @uniform @binding = 4 cnt : uint        // context depth (pos + 1)
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 hs : uint         // head_size == threads per group
    @uniform @binding = 9 scale : float
    @workgroup scores : float[128]  // >= ATTN_CHUNK_ROWS — the single-tg depth ceiling
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn16_msl")]
    def metal_sq_attn16 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khoff = (h / kv_mul) * hs
        let qoff = bb * (nheads * hs) + h * hs
        // pass 1: simdgroup-cooperative score fill — simdgroup sg dots q[head] against K row j;
        // the lane's q elements are loop-invariant, hoisted to registers (device loads can't be
        // hoisted by the compiler — buffers may alias)
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u              // elements per lane in the row run
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        var j = sg
        while (j < cnt) {
            let kb = j * kv_dim + khoff + sl * epl
            var s = q0 * float(kc[kb])
            if (epl > 1u) {
                s += q1 * float(kc[kb + 1u])
            }
            if (epl > 2u) {
                s += q2 * float(kc[kb + 2u])
            }
            if (epl > 3u) {
                s += q3 * float(kc[kb + 3u])
            }
            s = simd_sum(s)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        // pass 2a: max — strided local, simd butterfly, cross-simdgroup partials
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        // pass 2b: exp in place + sum
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        // pass 3: lane owns V column `lid` — 4 rows in flight so device-load latency overlaps
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= cnt) {
            acc0 += scores[j] * float(vc[j * kv_dim + khoff + lid])
            acc1 += scores[j + 1u] * float(vc[(j + 1u) * kv_dim + khoff + lid])
            acc2 += scores[j + 2u] * float(vc[(j + 2u) * kv_dim + khoff + lid])
            acc3 += scores[j + 3u] * float(vc[(j + 3u) * kv_dim + khoff + lid])
            j += 4u
        }
        while (j < cnt) {
            acc0 += scores[j] * float(vc[j * kv_dim + khoff + lid])
            j++
        }
        yo[qoff + lid] = (acc0 + acc1 + acc2 + acc3) / tot
    }
}

// f32-mirror twin (sessions created with f32 KV) — same math, f32 loads.
class MetalSqAttnF32 {
    @ssbo @binding = 0 kc : array<float>
    @ssbo @binding = 1 vc : array<float>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 yo : array<float>
    @uniform @binding = 4 cnt : uint
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 hs : uint
    @uniform @binding = 9 scale : float
    @workgroup scores : float[128]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn32_msl")]
    def metal_sq_attn32 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khoff = (h / kv_mul) * hs
        let qoff = bb * (nheads * hs) + h * hs
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        var j = sg
        while (j < cnt) {
            let kb = j * kv_dim + khoff + sl * epl
            var s = q0 * kc[kb]
            if (epl > 1u) {
                s += q1 * kc[kb + 1u]
            }
            if (epl > 2u) {
                s += q2 * kc[kb + 2u]
            }
            if (epl > 3u) {
                s += q3 * kc[kb + 3u]
            }
            s = simd_sum(s)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= cnt) {
            acc0 += scores[j] * vc[j * kv_dim + khoff + lid]
            acc1 += scores[j + 1u] * vc[(j + 1u) * kv_dim + khoff + lid]
            acc2 += scores[j + 2u] * vc[(j + 2u) * kv_dim + khoff + lid]
            acc3 += scores[j + 3u] * vc[(j + 3u) * kv_dim + khoff + lid]
            j += 4u
        }
        while (j < cnt) {
            acc0 += scores[j] * vc[j * kv_dim + khoff + lid]
            j++
        }
        yo[qoff + lid] = (acc0 + acc1 + acc2 + acc3) / tot
    }
}

// q8_0-mirror twin — K/V rows are ggml block_q8_0 (34B/32 elems); the mirror slab binds twice
// (half view for scale, byte view for quants — the dual-view pattern) and dequant rides the
// stream. Measured 1.1-1.7x over the f16 form at d512-2048 (benchmarks/attn/bench_metal_sq_attn.das).
class MetalSqAttnQ8 {
    @ssbo @binding = 0 ksh : array<float16> // K mirror slab, HALF-SCALE view (bound at the layer's byte offset)
    @ssbo @binding = 1 kqb : array<int8>    // the SAME K slab, BYTE view
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<int8>
    @ssbo @binding = 4 q : array<float>     // [B x qd] roped queries
    @ssbo @binding = 5 yo : array<float>    // [B x qd] attention out
    @uniform @binding = 6 cnt : uint        // context depth (pos + 1)
    @uniform @binding = 7 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 hs : uint        // head_size == threads per group
    @uniform @binding = 11 scale : float
    @workgroup scores : float[128]  // >= ATTN_CHUNK_ROWS — the single-tg depth ceiling
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_q8_msl")]
    def metal_sq_attn_q8 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khblk = (h / kv_mul) * (hs / 32u)   // kv head offset in BLOCKS
        let qoff = bb * (nheads * hs) + h * hs
        // pass 1: simdgroup-cooperative score fill — lane sl's epl elements sit in ONE block
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let hb = (sl * epl) / 32u               // lane's block within the head
        let ib = (sl * epl) % 32u               // lane's quant offset within the block
        var j = sg
        while (j < cnt) {
            let blk = j * nblk + khblk + hb
            let d = float(ksh[blk * 17u])
            let kb = blk * 34u + 2u + ib
            var s = q0 * float(kqb[kb])
            if (epl > 1u) {
                s += q1 * float(kqb[kb + 1u])
            }
            if (epl > 2u) {
                s += q2 * float(kqb[kb + 2u])
            }
            if (epl > 3u) {
                s += q3 * float(kqb[kb + 3u])
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        // pass 2a: max — strided local, simd butterfly, cross-simdgroup partials
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        // pass 2b: exp in place + sum
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        // pass 3: lane owns V column `lid` — 4 rows in flight, dequant rides the weighted sum
        let vb0 = khblk + lid / 32u
        let vin = lid % 32u
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= cnt) {
            let b0 = j * nblk + vb0
            let b1 = (j + 1u) * nblk + vb0
            let b2 = (j + 2u) * nblk + vb0
            let b3 = (j + 3u) * nblk + vb0
            acc0 += scores[j] * float(vqb[b0 * 34u + 2u + vin]) * float(vsh[b0 * 17u])
            acc1 += scores[j + 1u] * float(vqb[b1 * 34u + 2u + vin]) * float(vsh[b1 * 17u])
            acc2 += scores[j + 2u] * float(vqb[b2 * 34u + 2u + vin]) * float(vsh[b2 * 17u])
            acc3 += scores[j + 3u] * float(vqb[b3 * 34u + 2u + vin]) * float(vsh[b3 * 17u])
            j += 4u
        }
        while (j < cnt) {
            let b0 = j * nblk + vb0
            acc0 += scores[j] * float(vqb[b0 * 34u + 2u + vin]) * float(vsh[b0 * 17u])
            j++
        }
        yo[qoff + lid] = (acc0 + acc1 + acc2 + acc3) / tot
    }
}

// tq4-mirror twin — 18B rotated 4-bit blocks; q arrives already rotated (the rope-store twin
// rotates it), so attention runs in the rotated basis and yo needs one fwht-unsign pass downstream.
class MetalSqAttnTq4 {
    @ssbo @binding = 0 ksh : array<float16> // K mirror slab, HALF-SCALE view (bound at the layer's byte offset)
    @ssbo @binding = 1 kqb : array<uint8>   // the SAME K slab, BYTE view
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<uint8>
    @ssbo @binding = 4 q : array<float>     // [B x qd] roped + rotated queries
    @ssbo @binding = 5 yo : array<float>    // [B x qd] attention out (rotated basis)
    @uniform @binding = 6 cnt : uint
    @uniform @binding = 7 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 hs : uint
    @uniform @binding = 11 scale : float
    @workgroup scores : float[128]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_tq4_msl")]
    def metal_sq_attn_tq4 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * (nheads * hs) + h * hs
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let hb = (sl * epl) / 32u
        let inb = (sl * epl) % 32u          // this lane's element offset inside its block
        var j = sg
        while (j < cnt) {
            let blk = j * nblk + khblk + hb
            let d = float(ksh[blk * 9u])
            let kb = blk * 18u + 2u + inb / 2u
            let by0 = uint(kqb[kb])
            var s = 0.0
            if (epl == 1u) {
                let nib = (inb & 1u) == 1u ? (by0 >> 4u) : (by0 & 15u)
                s = q0 * float(int(nib) - 8)
            } else {
                s = q0 * float(int(by0 & 15u) - 8)
                s += q1 * float(int(by0 >> 4u) - 8)
                if (epl > 2u) {
                    let by1 = uint(kqb[kb + 1u])
                    s += q2 * float(int(by1 & 15u) - 8)
                    s += q3 * float(int(by1 >> 4u) - 8)
                }
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        // lane owns V column lid — nibble parity picks the half-byte
        let vblk0 = khblk + lid / 32u
        let vbyte = (lid % 32u) / 2u
        let hi = (lid & 1u) == 1u
        var acc = 0.0
        j = 0u
        while (j < cnt) {
            let blk = j * nblk + vblk0
            let by = uint(vqb[blk * 18u + 2u + vbyte])
            let nib = hi ? (by >> 4u) : (by & 15u)
            acc += scores[j] * float(int(nib) - 8) * float(vsh[blk * 9u])
            j++
        }
        yo[qoff + lid] = acc / tot
    }
}

// Chunked split-K twin for deeper contexts (cnt > ATTN_CHUNK_ROWS): each threadgroup owns one
// 128-row chunk, writes an unnormalized partial (acc[hs] + chunk max + expsum); a comb kernel
// rescales across chunks. The single-tg form is latency-bound (measured 7.5ms/token at depth 512).
class MetalSqAttnPartF16 {
    @ssbo @binding = 0 kc : array<float16>
    @ssbo @binding = 1 vc : array<float16>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 part : array<float>  // [B x nheads x nchunks x (hs+2)]: acc[hs], chunk max, chunk expsum
    @uniform @binding = 4 cnt : uint
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 hs : uint
    @uniform @binding = 9 scale : float
    @uniform @binding = 10 nchunks : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_part16_msl")]
    def metal_sq_attn_part16 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let ci = (wg / nheads) % nchunks
        let bb = wg / (nheads * nchunks)
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khoff = (h / kv_mul) * hs
        let qoff = bb * (nheads * hs) + h * hs
        let c0 = ci * 64u
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // pass 1: simdgroup-cooperative score fill over this chunk's rows (coalesced K loads,
        // loop-invariant q hoisted to registers)
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        var j = sg
        while (j < nrows) {
            let kb = (c0 + j) * kv_dim + khoff + sl * epl
            var s = q0 * float(kc[kb])
            if (epl > 1u) {
                s += q1 * float(kc[kb + 1u])
            }
            if (epl > 2u) {
                s += q2 * float(kc[kb + 2u])
            }
            if (epl > 3u) {
                s += q3 * float(kc[kb + 3u])
            }
            s = simd_sum(s)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        // pass 2a: chunk max
        var m = -3.0e38
        j = lid
        while (j < nrows) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        // pass 2b: exp in place + chunk expsum
        var l = 0.0
        j = lid
        while (j < nrows) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        // pass 3: lane owns V column `lid` — 4 rows in flight over this chunk's rows
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= nrows) {
            acc0 += scores[j] * float(vc[(c0 + j) * kv_dim + khoff + lid])
            acc1 += scores[j + 1u] * float(vc[(c0 + j + 1u) * kv_dim + khoff + lid])
            acc2 += scores[j + 2u] * float(vc[(c0 + j + 2u) * kv_dim + khoff + lid])
            acc3 += scores[j + 3u] * float(vc[(c0 + j + 3u) * kv_dim + khoff + lid])
            j += 4u
        }
        while (j < nrows) {
            acc0 += scores[j] * float(vc[(c0 + j) * kv_dim + khoff + lid])
            j++
        }
        let pbase = ((bb * nheads + h) * nchunks + ci) * (hs + 2u)
        part[pbase + lid] = acc0 + acc1 + acc2 + acc3
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// f32-mirror twin — same math, f32 K/V loads.
class MetalSqAttnPartF32 {
    @ssbo @binding = 0 kc : array<float>
    @ssbo @binding = 1 vc : array<float>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 part : array<float>
    @uniform @binding = 4 cnt : uint
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 hs : uint
    @uniform @binding = 9 scale : float
    @uniform @binding = 10 nchunks : uint
    @workgroup scores : float[64]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_part32_msl")]
    def metal_sq_attn_part32 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let ci = (wg / nheads) % nchunks
        let bb = wg / (nheads * nchunks)
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khoff = (h / kv_mul) * hs
        let qoff = bb * (nheads * hs) + h * hs
        let c0 = ci * 64u
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        var j = sg
        while (j < nrows) {
            let kb = (c0 + j) * kv_dim + khoff + sl * epl
            var s = q0 * kc[kb]
            if (epl > 1u) {
                s += q1 * kc[kb + 1u]
            }
            if (epl > 2u) {
                s += q2 * kc[kb + 2u]
            }
            if (epl > 3u) {
                s += q3 * kc[kb + 3u]
            }
            s = simd_sum(s)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < nrows) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()
        var l = 0.0
        j = lid
        while (j < nrows) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= nrows) {
            acc0 += scores[j] * vc[(c0 + j) * kv_dim + khoff + lid]
            acc1 += scores[j + 1u] * vc[(c0 + j + 1u) * kv_dim + khoff + lid]
            acc2 += scores[j + 2u] * vc[(c0 + j + 2u) * kv_dim + khoff + lid]
            acc3 += scores[j + 3u] * vc[(c0 + j + 3u) * kv_dim + khoff + lid]
            j += 4u
        }
        while (j < nrows) {
            acc0 += scores[j] * vc[(c0 + j) * kv_dim + khoff + lid]
            j++
        }
        let pbase = ((bb * nheads + h) * nchunks + ci) * (hs + 2u)
        part[pbase + lid] = acc0 + acc1 + acc2 + acc3
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// q8_0-mirror chunked twin — the Part kernel's block-codec form (dual-view reads, see
// MetalSqAttnQ8); partials stay f32, so the comb below serves this form unchanged.
class MetalSqAttnPartQ8 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 kqb : array<int8>
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<int8>
    @ssbo @binding = 4 q : array<float>
    @ssbo @binding = 5 part : array<float>  // [B x nheads x nchunks x (hs+2)]: acc[hs], chunk max, chunk expsum
    @uniform @binding = 6 cnt : uint
    @uniform @binding = 7 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 hs : uint
    @uniform @binding = 11 scale : float
    @uniform @binding = 12 nchunks : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_part_q8_msl")]
    def metal_sq_attn_part_q8 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let ci = (wg / nheads) % nchunks
        let bb = wg / (nheads * nchunks)
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * (nheads * hs) + h * hs
        let c0 = ci * 64u
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // pass 1: simdgroup-cooperative score fill over this chunk's rows
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let hb = (sl * epl) / 32u
        let ib = (sl * epl) % 32u
        var j = sg
        while (j < nrows) {
            let blk = (c0 + j) * nblk + khblk + hb
            let d = float(ksh[blk * 17u])
            let kb = blk * 34u + 2u + ib
            var s = q0 * float(kqb[kb])
            if (epl > 1u) {
                s += q1 * float(kqb[kb + 1u])
            }
            if (epl > 2u) {
                s += q2 * float(kqb[kb + 2u])
            }
            if (epl > 3u) {
                s += q3 * float(kqb[kb + 3u])
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        // pass 2a: chunk max
        var m = -3.0e38
        j = lid
        while (j < nrows) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        // pass 2b: exp in place + chunk expsum
        var l = 0.0
        j = lid
        while (j < nrows) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        // pass 3: lane owns V column `lid` — 4 rows in flight over this chunk's rows
        let vb0 = khblk + lid / 32u
        let vin = lid % 32u
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= nrows) {
            let b0 = (c0 + j) * nblk + vb0
            let b1 = (c0 + j + 1u) * nblk + vb0
            let b2 = (c0 + j + 2u) * nblk + vb0
            let b3 = (c0 + j + 3u) * nblk + vb0
            acc0 += scores[j] * float(vqb[b0 * 34u + 2u + vin]) * float(vsh[b0 * 17u])
            acc1 += scores[j + 1u] * float(vqb[b1 * 34u + 2u + vin]) * float(vsh[b1 * 17u])
            acc2 += scores[j + 2u] * float(vqb[b2 * 34u + 2u + vin]) * float(vsh[b2 * 17u])
            acc3 += scores[j + 3u] * float(vqb[b3 * 34u + 2u + vin]) * float(vsh[b3 * 17u])
            j += 4u
        }
        while (j < nrows) {
            let b0 = (c0 + j) * nblk + vb0
            acc0 += scores[j] * float(vqb[b0 * 34u + 2u + vin]) * float(vsh[b0 * 17u])
            j++
        }
        let pbase = ((bb * nheads + h) * nchunks + ci) * (hs + 2u)
        part[pbase + lid] = acc0 + acc1 + acc2 + acc3
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// tq4-mirror chunked twin (nibble reads — see MetalSqAttnTq4); partials stay f32, so the comb
// below serves this form unchanged and the fwht-unsign runs after it.
class MetalSqAttnPartTq4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 kqb : array<uint8>
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<uint8>
    @ssbo @binding = 4 q : array<float>
    @ssbo @binding = 5 part : array<float>  // [B x nheads x nchunks x (hs+2)]
    @uniform @binding = 6 cnt : uint
    @uniform @binding = 7 nblk : uint
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 hs : uint
    @uniform @binding = 11 scale : float
    @uniform @binding = 12 nchunks : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_part_tq4_msl")]
    def metal_sq_attn_part_tq4 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let ci = (wg / nheads) % nchunks
        let bb = wg / (nheads * nchunks)
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * (nheads * hs) + h * hs
        let c0 = ci * 64u
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let hb = (sl * epl) / 32u
        let inb = (sl * epl) % 32u
        var j = sg
        while (j < nrows) {
            let blk = (c0 + j) * nblk + khblk + hb
            let d = float(ksh[blk * 9u])
            let kb = blk * 18u + 2u + inb / 2u
            let by0 = uint(kqb[kb])
            var s = 0.0
            if (epl == 1u) {
                let nib = (inb & 1u) == 1u ? (by0 >> 4u) : (by0 & 15u)
                s = q0 * float(int(nib) - 8)
            } else {
                s = q0 * float(int(by0 & 15u) - 8)
                s += q1 * float(int(by0 >> 4u) - 8)
                if (epl > 2u) {
                    let by1 = uint(kqb[kb + 1u])
                    s += q2 * float(int(by1 & 15u) - 8)
                    s += q3 * float(int(by1 >> 4u) - 8)
                }
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < nrows) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        var l = 0.0
        j = lid
        while (j < nrows) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        let vblk0 = khblk + lid / 32u
        let vbyte = (lid % 32u) / 2u
        let hi = (lid & 1u) == 1u
        var acc0 = 0.0
        j = 0u
        while (j < nrows) {
            let blk = (c0 + j) * nblk + vblk0
            let by = uint(vqb[blk * 18u + 2u + vbyte])
            let nib = hi ? (by >> 4u) : (by & 15u)
            acc0 += scores[j] * float(int(nib) - 8) * float(vsh[blk * 9u])
            j++
        }
        let pbase = ((bb * nheads + h) * nchunks + ci) * (hs + 2u)
        part[pbase + lid] = acc0
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// Cross-chunk combine: one threadgroup per (stream, head), hs threads — rescale each chunk's
// unnormalized partial to the global max, sum, normalize. Dtype-agnostic (partials are f32).
class MetalSqAttnComb {
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 yo : array<float>
    @uniform @binding = 2 nchunks : uint
    @uniform @binding = 3 nheads : uint
    @uniform @binding = 4 hs : uint

    [metal_kernel(name="metal_sq_attn_comb_msl")]
    def metal_sq_attn_comb {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let base = (bb * nheads + h) * nchunks
        var mm = -3.0e38
        var ci = 0u
        while (ci < nchunks) {
            mm = max(mm, part[(base + ci) * (hs + 2u) + hs])
            ci++
        }
        var acc = 0.0
        var l = 0.0
        ci = 0u
        while (ci < nchunks) {
            let pb = (base + ci) * (hs + 2u)
            let w = exp(part[pb + hs] - mm)
            acc += part[pb + lid] * w
            l += part[pb + hs + 1u] * w
            ci++
        }
        yo[bb * (nheads * hs) + h * hs + lid] = acc / l
    }
}

// Fused QKV GEMV + rope + KV-row store: element pairs (2r, 2r+1) of q/k ARE weight rows 2r, 2r+1
// of the fused QKV region, so one simdgroup holds a whole rope pair in registers after its two
// row dots. Deletes the separate rope_store dispatch entirely.
class MetalQ8GemvQkvRsF16 {
    @ssbo @binding = 0 wq : array<byte4>    // fused QKV rows (concat region)
    @ssbo @binding = 1 ws : array<float16>  // fused scale planes, halfwords
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 qo : array<float>    // q out, roped ([0, qd) of the QKV buffer)
    @ssbo @binding = 4 kd : array<float16>  // mirror K row (bound at the row offset)
    @ssbo @binding = 5 vd : array<float16>
    @ssbo @binding = 6 tcos : array<float>  // [hs/2] this position's rope row
    @ssbo @binding = 7 tsin : array<float>
    @uniform @binding = 8 ndim : uint
    @uniform @binding = 9 ddim : uint       // qd + 2*kvd
    @uniform @binding = 10 qd : uint
    @uniform @binding = 11 kvd : uint
    @uniform @binding = 12 hs : uint

    [metal_kernel(name="metal_q8_gemv_qkv_rs16_msl")]
    def metal_q8_gemv_qkv_rs16 {
        let row = (gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID) * 2u
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var acc0 = 0.0
        var acc1 = 0.0
        var k = lane
        while (k < nb4) {
            let x4 = x[k]
            let w0 = float4(int4(wq[row * nb4 + k]))
            let w1 = float4(int4(wq[(row + 1u) * nb4 + k]))
            acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(ws[row * nb + k / 8u])
            acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(ws[(row + 1u) * nb + k / 8u])
            k += gl_SubgroupSize
        }
        acc0 = simd_sum(acc0)
        acc1 = simd_sum(acc1)
        if (lane != 0u) {
            return
        }
        if (row < qd) {
            let j = (row % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            qo[row] = acc0 * fcr - acc1 * fci
            qo[row + 1u] = acc0 * fci + acc1 * fcr
        } elif (row < qd + kvd) {
            let ik = row - qd
            let j = (ik % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            kd[ik] = float16(min(max(acc0 * fcr - acc1 * fci, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
            kd[ik + 1u] = float16(min(max(acc0 * fci + acc1 * fcr, -65504.0), 65504.0))   // nolint:STYLE019
        } else {
            let iv = row - qd - kvd
            vd[iv] = float16(min(max(acc0, -65504.0), 65504.0))   // nolint:STYLE019
            vd[iv + 1u] = float16(min(max(acc1, -65504.0), 65504.0))   // nolint:STYLE019
        }
    }
}

// f32-mirror twin — straight stores, no clamp.
class MetalQ8GemvQkvRsF32 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float16>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 qo : array<float>
    @ssbo @binding = 4 kd : array<float>
    @ssbo @binding = 5 vd : array<float>
    @ssbo @binding = 6 tcos : array<float>
    @ssbo @binding = 7 tsin : array<float>
    @uniform @binding = 8 ndim : uint
    @uniform @binding = 9 ddim : uint
    @uniform @binding = 10 qd : uint
    @uniform @binding = 11 kvd : uint
    @uniform @binding = 12 hs : uint

    [metal_kernel(name="metal_q8_gemv_qkv_rs32_msl")]
    def metal_q8_gemv_qkv_rs32 {
        let row = (gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID) * 2u
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var acc0 = 0.0
        var acc1 = 0.0
        var k = lane
        while (k < nb4) {
            let x4 = x[k]
            let w0 = float4(int4(wq[row * nb4 + k]))
            let w1 = float4(int4(wq[(row + 1u) * nb4 + k]))
            acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(ws[row * nb + k / 8u])
            acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(ws[(row + 1u) * nb + k / 8u])
            k += gl_SubgroupSize
        }
        acc0 = simd_sum(acc0)
        acc1 = simd_sum(acc1)
        if (lane != 0u) {
            return
        }
        if (row < qd) {
            let j = (row % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            qo[row] = acc0 * fcr - acc1 * fci
            qo[row + 1u] = acc0 * fci + acc1 * fcr
        } elif (row < qd + kvd) {
            let ik = row - qd
            let j = (ik % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            kd[ik] = acc0 * fcr - acc1 * fci
            kd[ik + 1u] = acc0 * fci + acc1 * fcr
        } else {
            let iv = row - qd - kvd
            vd[iv] = acc0
            vd[iv + 1u] = acc1
        }
    }
}

// The W1|W3 twin of the same shape: one simdgroup dots BOTH ffn rows of an output element —
// gate row r and up row r + hidden of the fused region — and writes silu(g)·u directly.
// Deletes the swiglu dispatch AND its serialization level.
class MetalQ8GemvW13Sw {
    @ssbo @binding = 0 wq : array<byte4>    // fused W1|W3 rows (concat region)
    @ssbo @binding = 1 ws : array<float16>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 ho : array<float>    // [hidden] silu(gate)·up
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 hidden : uint

    [metal_kernel(name="metal_q8_gemv_w13sw16_msl")]
    def metal_q8_gemv_w13sw16 {
        let r = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (r >= hidden) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        let ubase = (r + hidden) * nb4
        let usb = (r + hidden) * nb
        var acc0 = 0.0
        var acc1 = 0.0
        var k = lane
        while (k < nb4) {
            let x4 = x[k]
            let w0 = float4(int4(wq[r * nb4 + k]))
            let w1 = float4(int4(wq[ubase + k]))
            acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(ws[r * nb + k / 8u])
            acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(ws[usb + k / 8u])
            k += gl_SubgroupSize
        }
        acc0 = simd_sum(acc0)
        acc1 = simd_sum(acc1)
        if (lane == 0u) {
            ho[r] = (acc0 / (1.0 + exp(-acc0))) * acc1
        }
    }
}

// Row copy for the zero-copy logits rail: one batched-classifier staging row into a session's
// bytesNoCopy-wrapped logits storage — the scatter rides the cb instead of the landing memcpy.
// src binds at the row's byte offset; grid = ceil(n/256) x 256-tg.
class MetalCopyRow {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 dst : array<float>
    @uniform @binding = 2 n : uint

    [metal_kernel(name="metal_copy_row_msl")]
    def metal_copy_row {
        let i = gl_GlobalInvocationID.x
        if (i < n) {
            dst[i] = src[i]
        }
    }
}

// GPU-side greedy sampling — the speculative chain's token source: one threadgroup, strided
// argmax over the logits with the CPU sampler's exact tie-break (lowest index wins; per-lane the
// strict > keeps the earliest, cross-lane the (==, <) rule picks the lower index).
class MetalArgmax {
    @ssbo @binding = 0 lg : array<float>
    @ssbo @binding = 1 tok : array<uint>    // [1] winner index out
    @uniform @binding = 2 n : uint
    @workgroup redv : float[32]
    @workgroup redi : uint[32]

    [metal_kernel(name="metal_argmax_msl")]
    def metal_argmax {
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        var bv = -3.0e38
        var bi = 0u
        var j = lid
        while (j < n) {
            let v = lg[j]
            if (v > bv) {
                bv = v
                bi = j
            }
            j += nt
        }
        var m = 16u
        while (m >= 1u) {
            let ov = simd_shuffle_xor(bv, m)
            let oi = simd_shuffle_xor(bi, m)
            if (ov > bv || (ov == bv && oi < bi)) {
                bv = ov
                bi = oi
            }
            m /= 2u
        }
        if (gl_SubgroupInvocationID == 0u) {
            redv[gl_SubgroupID] = bv
            redi[gl_SubgroupID] = bi
        }
        barrier()
        if (lid == 0u) {
            var wv = redv[0]
            var wi = redi[0]
            var sgi = 1u
            while (sgi < gl_NumSubgroups) {
                if (redv[sgi] > wv || (redv[sgi] == wv && redi[sgi] < wi)) {
                    wv = redv[sgi]
                    wi = redi[sgi]
                }
                sgi++
            }
            tok[0] = wi
        }
    }
}

// Speculative embed gather: dequant the argmax winner's Q8 embedding row (the cls_q8 linear copy
// at emb_q8_off) into the next step's residual input — the GPU chain's embed_row.
class MetalEmbedQ8 {
    @ssbo @binding = 0 q : array<int8>      // the whole linear embed plane
    @ssbo @binding = 1 sc : array<float>    // its scale plane
    @ssbo @binding = 2 tok : array<uint>
    @ssbo @binding = 3 xo : array<float>    // [dim] residual input out
    @uniform @binding = 4 dim : uint

    [metal_kernel(name="metal_embed_q8_msl")]
    def metal_embed_q8 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= dim) {
            return
        }
        let base = tok[0] * dim + gid
        xo[gid] = float(q[base]) * sc[base / 32u]
    }
}

// f16-scale twin (wscale_f16 models — the qscales16 halfword plane).
class MetalEmbedQ8S16 {
    @ssbo @binding = 0 q : array<int8>
    @ssbo @binding = 1 sc : array<float16>
    @ssbo @binding = 2 tok : array<uint>
    @ssbo @binding = 3 xo : array<float>
    @uniform @binding = 4 dim : uint

    [metal_kernel(name="metal_embed_q8_s16_msl")]
    def metal_embed_q8_s16 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= dim) {
            return
        }
        let base = tok[0] * dim + gid
        xo[gid] = float(q[base]) * float(sc[base / 32u])
    }
}

// Fused residual-add + rmsnorm: x += a in place, then y = rms(x)·w, one dispatch instead of two.
// The added row stages through the threadgroup slab since barrier() is a threadgroup fence only.
// dim caps at the slab; wider models take the unfused pair (enc_add_rms_site).
class MetalAddRms {
    @ssbo @binding = 0 x : array<float>     // [dim] residual stream, add target
    @ssbo @binding = 1 a : array<float>     // [dim] addend (fully read before y is written — y may alias a)
    @ssbo @binding = 2 w : array<float>     // [dim] norm weight
    @ssbo @binding = 3 y : array<float>     // [dim] normed out
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 eps : float
    @workgroup xrow : float[4096]           // == ADD_RMS_MAX_DIM
    @workgroup partial : float[32]          // one per simdgroup at the 1024-thread dispatch
    @workgroup inv_sh : float

    [metal_kernel(name="metal_add_rms_msl")]
    def metal_add_rms {
        let lid = gl_LocalInvocationID.x
        var ss = 0.0
        var k = lid
        while (k < dim) {
            let vv = x[k] + a[k]
            x[k] = vv
            xrow[k] = vv
            ss += vv * vv
            k += gl_WorkGroupSize.x
        }
        ss = simd_sum(ss)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ss
        }
        barrier()
        if (lid == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += partial[sgi]
                sgi++
            }
            inv_sh = rsqrt(tot / float(dim) + eps)
        }
        barrier()
        let inv = inv_sh
        k = lid
        while (k < dim) {
            y[k] = w[k] * (xrow[k] * inv)
            k += gl_WorkGroupSize.x
        }
    }
}

// Fused rope + KV-row store, one thread per rotation pair (ntok == 1): q pairs rope in place; k
// pairs rope straight into the mirror K row (±65504 clamp) and the same thread copies its V pair.
// Replaces the rope_qk + kv_store dispatch pair.
class MetalRopeStoreF16 {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) roped in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 kd : array<float16>  // mirror K row (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 vd : array<float16>
    @ssbo @binding = 4 tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 5 tsin : array<float>
    @uniform @binding = 6 qd : uint
    @uniform @binding = 7 head_size : uint
    @uniform @binding = 8 npairs_q : uint   // qd/2 — the q half of the grid
    @uniform @binding = 9 npairs : uint     // + kvd/2 — grid guard

    [metal_kernel(name="metal_rope_store16_msl")]
    def metal_rope_store16 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let isK = gid >= npairs_q
        let lgid = isK ? gid - npairs_q : gid
        let half = head_size / 2u
        let j = lgid % half
        let i = (lgid / half) * head_size + 2u * j
        let fcr = tcos[j]
        let fci = tsin[j]
        if (isK) {
            let v0 = qk[qd + i]
            let v1 = qk[qd + i + 1u]
            let r0 = v0 * fcr - v1 * fci
            let r1 = v0 * fci + v1 * fcr
            kd[i] = float16(min(max(r0, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
            kd[i + 1u] = float16(min(max(r1, -65504.0), 65504.0))   // nolint:STYLE019
            vd[i] = float16(min(max(vs[i], -65504.0), 65504.0))   // nolint:STYLE019
            vd[i + 1u] = float16(min(max(vs[i + 1u], -65504.0), 65504.0))   // nolint:STYLE019
        } else {
            let v0 = qk[i]
            let v1 = qk[i + 1u]
            qk[i] = v0 * fcr - v1 * fci
            qk[i + 1u] = v0 * fci + v1 * fcr
        }
    }
}

// f32-mirror twin — straight stores, no clamp.
class MetalRopeStoreF32 {
    @ssbo @binding = 0 qk : array<float>
    @ssbo @binding = 1 vs : array<float>
    @ssbo @binding = 2 kd : array<float>
    @ssbo @binding = 3 vd : array<float>
    @ssbo @binding = 4 tcos : array<float>
    @ssbo @binding = 5 tsin : array<float>
    @uniform @binding = 6 qd : uint
    @uniform @binding = 7 head_size : uint
    @uniform @binding = 8 npairs_q : uint
    @uniform @binding = 9 npairs : uint

    [metal_kernel(name="metal_rope_store32_msl")]
    def metal_rope_store32 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let isK = gid >= npairs_q
        let lgid = isK ? gid - npairs_q : gid
        let half = head_size / 2u
        let j = lgid % half
        let i = (lgid / half) * head_size + 2u * j
        let fcr = tcos[j]
        let fci = tsin[j]
        if (isK) {
            let v0 = qk[qd + i]
            let v1 = qk[qd + i + 1u]
            kd[i] = v0 * fcr - v1 * fci
            kd[i + 1u] = v0 * fci + v1 * fcr
            vd[i] = vs[i]
            vd[i + 1u] = vs[i + 1u]
        } else {
            let v0 = qk[i]
            let v1 = qk[i + 1u]
            qk[i] = v0 * fcr - v1 * fci
            qk[i + 1u] = v0 * fci + v1 * fcr
        }
    }
}

// q8_0-mirror rope + KV-row store: q pairs rope in place; K/V rows quantize per 32-block (one
// thread per block) matching quantize_q8kv_row's exact semantics, so CPU fallback steps read the
// same codec. K blocks rope their 16 pairs first; V blocks copy raw.
class MetalRopeStoreQ8 {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) roped in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 ksh : array<float16> // mirror K row, HALF-SCALE view (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 kqb : array<int8>    // the SAME K row, BYTE view
    @ssbo @binding = 4 vsh : array<float16>
    @ssbo @binding = 5 vqb : array<int8>
    @ssbo @binding = 6 tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 7 tsin : array<float>
    @uniform @binding = 8 qd : uint
    @uniform @binding = 9 head_size : uint
    @uniform @binding = 10 npairs_q : uint  // qd/2 — the q-rope half of the grid
    @uniform @binding = 11 nblk : uint      // kv_dim/32 — K blocks (V adds the same again)

    [metal_kernel(name="metal_rope_store_q8_msl")]
    def metal_rope_store_q8 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs_q + 2u * nblk) {
            return
        }
        let half = head_size / 2u
        if (gid < npairs_q) {
            let j = gid % half
            let i = (gid / half) * head_size + 2u * j
            let fcr = tcos[j]
            let fci = tsin[j]
            let v0 = qk[i]
            let v1 = qk[i + 1u]
            qk[i] = v0 * fcr - v1 * fci
            qk[i + 1u] = v0 * fci + v1 * fcr
            return
        }
        let bi = gid - npairs_q
        let isV = bi >= nblk
        let blk = isV ? bi - nblk : bi
        let base = blk * 32u                // element base within the K/V row
        var vals : float[32]
        if (isV) {
            var e = 0u
            while (e < 32u) {
                vals[e] = vs[base + e]
                e++
            }
        } else {
            var p = 0u
            while (p < 16u) {
                let ge = base + 2u * p
                let j = (ge % head_size) / 2u
                let fcr = tcos[j]
                let fci = tsin[j]
                let v0 = qk[qd + ge]
                let v1 = qk[qd + ge + 1u]
                vals[2u * p] = v0 * fcr - v1 * fci
                vals[2u * p + 1u] = v0 * fci + v1 * fcr
                p++
            }
        }
        var amax = 0.0
        var k = 0u
        while (k < 32u) {
            amax = max(amax, abs(vals[k]))
            k++
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (isV) {
            vsh[blk * 17u] = float16(d)
            k = 0u
            while (k < 32u) {
                vqb[blk * 34u + 2u + k] = int8(round(vals[k] * id))
                k++
            }
        } else {
            ksh[blk * 17u] = float16(d)
            k = 0u
            while (k < 32u) {
                kqb[blk * 34u + 2u + k] = int8(round(vals[k] * id))
                k++
            }
        }
    }
}

// tq4-mirror rope + KV-row store: one threadgroup per head (head_size threads — the FWHT spans
// the head). Q heads rope + rotate in place; K heads rope + rotate + quantize into the mirror
// row; V heads rotate + quantize, matching the CPU seam's fwht_signs_row / quantize_tq4kv_row bit-for-bit.
class MetalRopeStoreTq4 {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) ropes + rotates in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 ksh : array<float16> // mirror K row, HALF-SCALE view (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 kqb : array<uint8>   // the SAME row, BYTE view
    @ssbo @binding = 4 vsh : array<float16>
    @ssbo @binding = 5 vqb : array<uint8>
    @ssbo @binding = 6 tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 7 tsin : array<float>
    @ssbo @binding = 8 signs : array<float> // the tq4 rotation signs (prefix per head_size)
    @uniform @binding = 9 qd : uint
    @uniform @binding = 10 head_size : uint
    @uniform @binding = 11 nheads : uint
    @uniform @binding = 12 kv_mul : uint
    @workgroup buf : float[128]

    [metal_kernel(name="metal_rope_store_tq4_msl")]
    def metal_rope_store_tq4 {
        let tgid = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        let hs = head_size
        let nkvh = nheads / kv_mul
        let isQ = tgid < nheads
        let isK = !isQ && tgid < nheads + nkvh
        // this head's element run: q heads in the q region, k heads in the k region, v heads in vs
        let hh = isQ ? tgid : (isK ? tgid - nheads : tgid - nheads - nkvh)
        let base = isQ ? tgid * hs : (isK ? qd + hh * hs : hh * hs)
        var v = 0.0
        if (isQ || isK) {
            // rope via the pair partner (pairs are adjacent and never straddle threads' loads)
            let j = lid / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            let mine = qk[base + lid]
            let other = qk[base + (lid ^ 1u)]
            v = (lid & 1u) == 0u ? mine * fcr - other * fci : other * fci + mine * fcr
        } else {
            v = vs[base + lid]
        }
        // fwht_signs_row: signs, then the orthonormal butterfly
        buf[lid] = v * signs[lid]
        barrier()
        var h = 1u
        while (h < hs) {
            let a = buf[lid]
            let b = buf[lid ^ h]
            barrier()
            buf[lid] = (lid & h) == 0u ? a + b : b - a
            barrier()
            h *= 2u
        }
        v = buf[lid] * (1.0 / sqrt(float(hs)))
        if (isQ) {
            qk[base + lid] = v
            return
        }
        // per-32-block quantize — block == simdgroup; signed max keeps the FIRST max-abs element
        let sl = gl_SubgroupInvocationID
        var ma = abs(v)
        var mv = v
        var ml = sl
        var off = 16u
        while (off > 0u) {
            let oa = simd_shuffle_xor(ma, off)
            let ov = simd_shuffle_xor(mv, off)
            let ol = simd_shuffle_xor(ml, off)
            let take = oa > ma || (oa == ma && ol < ml)
            ma = take ? oa : ma
            mv = take ? ov : mv
            ml = take ? ol : ml
            off /= 2u
        }
        let d = mv / -8.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        let qv = min(15, int(v * id + 8.5))
        let qother = simd_shuffle_xor(qv, 1u)
        let blk = hh * (hs / 32u) + lid / 32u   // block within the K/V row
        if (isK) {
            if (sl == 0u) {
                ksh[blk * 9u] = float16(d)
            }
            if ((lid & 1u) == 0u) {
                kqb[blk * 18u + 2u + (lid % 32u) / 2u] = uint8(uint(qv) | (uint(qother) << 4u))
            }
        } else {
            if (sl == 0u) {
                vsh[blk * 9u] = float16(d)
            }
            if ((lid & 1u) == 0u) {
                vqb[blk * 18u + 2u + (lid % 32u) / 2u] = uint8(uint(qv) | (uint(qother) << 4u))
            }
        }
    }
}

// The tq4 un-rotate: attention output is in the rotated basis per head — fwht_unsign_row it
// (butterfly + 1/sqrt(n), then signs) before the wo projection. One threadgroup per (head, row);
// the single-stream path dispatches one row.
class MetalFwhtUnsign {
    @ssbo @binding = 0 yo : array<float>    // [rows x qstride] attention out, un-rotated in place
    @ssbo @binding = 1 signs : array<float>
    @uniform @binding = 2 head_size : uint
    @uniform @binding = 3 qstride : uint
    @workgroup buf : float[128]

    [metal_kernel(name="metal_fwht_unsign_msl")]
    def metal_fwht_unsign {
        let lid = gl_LocalInvocationID.x
        let hs = head_size
        let base = gl_WorkGroupID.y * qstride + gl_WorkGroupID.x * hs
        buf[lid] = yo[base + lid]
        barrier()
        var h = 1u
        while (h < hs) {
            let a = buf[lid]
            let b = buf[lid ^ h]
            barrier()
            buf[lid] = (lid & h) == 0u ? a + b : b - a
            barrier()
            h *= 2u
        }
        yo[base + lid] = buf[lid] * (1.0 / sqrt(float(hs))) * signs[lid]
    }
}

// ===== the batched-step kernels (P4) =====
// One eval_batch step = B sessions through one weight pass; per-row kernels reach each row's KV
// slice via row table rt[b] = (k base, v base, cap_rows, cnt) in arena elements.

// Fixed-B batched GEMV twins, X-STAGED: the tg's 4 rows share one X panel staged in tgmem
// (cooperative load) instead of per-lane device reads (lab: B=2 +40-88%, B=4 ~2x; per-lane-
// consecutive chunking measured 30-45% WORSE). Geometry pinned at 128 threads = 4 rows.
class MetalGemvB2 {
    @ssbo @binding = 0 wq : array<byte4>    // [d x n] Q8 rows, byte4 view
    @ssbo @binding = 1 wsf : array<float>   // [d x n/32] scales, f32 view
    @ssbo @binding = 2 wsh : array<float16> // ... the same buffer, halfword view (wscale_f16)
    @ssbo @binding = 3 x : array<float4>    // [B x n] f32 rows, float4 view
    @ssbo @binding = 4 y : array<float>     // [B x d]
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 s16 : uint
    @workgroup txp : float4[128]            // [2 streams x 64 float4] X panel chunk

    [metal_kernel(name="metal_gemv_b2_msl")]
    def metal_gemv_b2 {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, ddim - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[2]
        var cb = 0u
        while (cb + 64u <= nb4) {
            barrier()                       // previous round done reading the panel
            txp[lid] = x[(lid / 64u) * nb4 + cb + lid % 64u]
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let sv = s16 != 0u ? float(wsh[sbase + k / 8u]) : wsf[sbase + k / 8u]
                let w4 = float4(int4(wq[wbase + k])) * sv
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(2)) {
                    let xv = txp[uint(b) * 64u + kt]
                    acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
                }
            }
            cb += 64u
        }
        var k = cb + lane
        while (k < nb4) {
            let sv = s16 != 0u ? float(wsh[sbase + k / 8u]) : wsf[sbase + k / 8u]
            let w4 = float4(int4(wq[wbase + k])) * sv
            for [unroll_full] (b in range(2)) {
                let xv = x[uint(b) * nb4 + k]
                acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(2)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalGemvB4 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 wsf : array<float>
    @ssbo @binding = 2 wsh : array<float16>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 s16 : uint
    @workgroup txp : float4[256]            // [4 streams x 64 float4] X panel chunk

    [metal_kernel(name="metal_gemv_b4_msl")]
    def metal_gemv_b4 {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, ddim - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[4]
        var cb = 0u
        while (cb + 64u <= nb4) {
            barrier()                       // previous round done reading the panel
            for [unroll_full] (j in range(2)) {
                let slot = uint(j) * 128u + lid
                txp[slot] = x[(slot / 64u) * nb4 + cb + slot % 64u]
            }
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let sv = s16 != 0u ? float(wsh[sbase + k / 8u]) : wsf[sbase + k / 8u]
                let w4 = float4(int4(wq[wbase + k])) * sv
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(4)) {
                    let xv = txp[uint(b) * 64u + kt]
                    acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
                }
            }
            cb += 64u
        }
        var k = cb + lane
        while (k < nb4) {
            let sv = s16 != 0u ? float(wsh[sbase + k / 8u]) : wsf[sbase + k / 8u]
            let w4 = float4(int4(wq[wbase + k])) * sv
            for [unroll_full] (b in range(4)) {
                let xv = x[uint(b) * nb4 + k]
                acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(4)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// Skinny-batch GEMV: streamed form beats x-staged panel kernels ~12% at B <= 4. 64 threads = 2
// simdgroups x 2 row-halves; 16 lanes stream one output row's interleaved 34B q8 blocks, batch
// columns share the dequantized weight stream. Requires n % 256 == 0 (unguarded 4-chunk stripe).
class MetalQ8MvB2 {
    @ssbo @binding = 0 wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 x : array<float4>    // [B rows x xs4 float4-stride] f32 rows
    @ssbo @binding = 3 y : array<float>     // [B x ddim]
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @uniform @binding = 6 xs4 : uint        // x row stride in float4s (ndim/4 = dense)

    [metal_kernel(name="metal_q8_mv_b2_msl")]
    def metal_q8_mv_b2 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 16u
        let row0 = gl_WorkGroupID.x * 4u + gl_SubgroupID * 2u + lane / 16u
        let row = min(row0, ddim - 1u)      // clamp, not return — keeps the shuffle groups full
        let nb = ndim / 32u
        let blk = row * nb + tx / 8u
        let cq = 4u * (tx % 4u) + 16u * ((tx % 8u) / 4u)   // quant offset of chunk tx%8 in its block
        var sp = unsafe(addr(wsh[blk * 17u]))
        var qp = unsafe(addr(wqb[blk * 34u + 2u + cq]))
        var x0 = unsafe(addr(x[tx]))
        var x1 = unsafe(addr(x[xs4 + tx]))
        var acc : float[2]
        var lw : float4[4]
        var ich = tx
        while (4u * ich < ndim) {
            // all 4 chunk dequants batch AHEAD of the fma chains — the load batching that makes
            // this shape win (interleaved dequant+fma measured +0.09 GPU-ms/step)
            for [unroll_full] (c in range(4)) {
                let d = float(unsafe(sp[uint(c) * 34u]))   // chunk c sits 2 blocks (34 halves) on
                let qb = uint(c) * 68u                     // ... and 68 quant bytes
                lw[c] = float4(float(unsafe(qp[qb])), float(unsafe(qp[qb + 1u])),
                    float(unsafe(qp[qb + 2u])), float(unsafe(qp[qb + 3u]))) * d
            }
            for [unroll_full] (c in range(4)) {
                let xv0 = unsafe(x0[uint(c) * 16u])
                let xv1 = unsafe(x1[uint(c) * 16u])
                acc[0] += lw[c].x * xv0.x + lw[c].y * xv0.y + lw[c].z * xv0.z + lw[c].w * xv0.w
                acc[1] += lw[c].x * xv1.x + lw[c].y * xv1.y + lw[c].z * xv1.z + lw[c].w * xv1.w
            }
            sp = unsafe(sp + 136)           // 8 blocks x 17 halves per stripe
            qp = unsafe(qp + 272)           // 8 blocks x 34 bytes
            x0 = unsafe(x0 + 64)
            x1 = unsafe(x1 + 64)
            ich += 64u
        }
        for [unroll_full] (b in range(2)) {
            var s = acc[b]
            s += simd_shuffle_down(s, 8u)
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row0 < ddim) {
                y[uint(b) * ddim + row0] = s
            }
        }
    }
}

// 4-column twin (serves B = 3..4; pad column reads clamp to row 0, stores gate on nrows). Drops
// to 8 lanes per output row (4 rows/simdgroup, 8 rows/tg): fewer lanes per weight load, more
// rows in flight hides the block-load latency instead.
class MetalQ8MvB4 {
    @ssbo @binding = 0 wsh : array<float16>
    @ssbo @binding = 1 wqb : array<int8>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @uniform @binding = 6 nrows : uint
    @uniform @binding = 7 xs4 : uint

    [metal_kernel(name="metal_q8_mv_b4_msl")]
    def metal_q8_mv_b4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let row0 = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + lane / 8u
        let row = min(row0, ddim - 1u)
        let nb = ndim / 32u
        let blk = row * nb
        let cq = 4u * (tx % 4u) + 16u * (tx / 4u)   // quant offset of chunk tx in its block
        var sp = unsafe(addr(wsh[blk * 17u]))
        var qp = unsafe(addr(wqb[blk * 34u + 2u + cq]))
        var x0 = unsafe(addr(x[tx]))
        var x1 = unsafe(addr(x[xs4 + tx]))
        var x2 = unsafe(addr(x[min(2u, nrows - 1u) * xs4 + tx]))
        var x3 = unsafe(addr(x[min(3u, nrows - 1u) * xs4 + tx]))
        var acc : float[4]
        var lw : float4[4]
        var ich = tx
        while (4u * ich < ndim) {
            for [unroll_full] (c in range(4)) {
                let d = float(unsafe(sp[uint(c) * 17u]))   // chunk c sits 1 block (17 halves) on
                let qb = uint(c) * 34u
                lw[c] = float4(float(unsafe(qp[qb])), float(unsafe(qp[qb + 1u])),
                    float(unsafe(qp[qb + 2u])), float(unsafe(qp[qb + 3u]))) * d
            }
            for [unroll_full] (c in range(4)) {
                let o = uint(c) * 8u
                let xv0 = unsafe(x0[o])
                let xv1 = unsafe(x1[o])
                let xv2 = unsafe(x2[o])
                let xv3 = unsafe(x3[o])
                acc[0] += lw[c].x * xv0.x + lw[c].y * xv0.y + lw[c].z * xv0.z + lw[c].w * xv0.w
                acc[1] += lw[c].x * xv1.x + lw[c].y * xv1.y + lw[c].z * xv1.z + lw[c].w * xv1.w
                acc[2] += lw[c].x * xv2.x + lw[c].y * xv2.y + lw[c].z * xv2.z + lw[c].w * xv2.w
                acc[3] += lw[c].x * xv3.x + lw[c].y * xv3.y + lw[c].z * xv3.z + lw[c].w * xv3.w
            }
            sp = unsafe(sp + 68)            // 4 blocks x 17 halves per stripe
            qp = unsafe(qp + 136)           // 4 blocks x 34 bytes
            x0 = unsafe(x0 + 32)
            x1 = unsafe(x1 + 32)
            x2 = unsafe(x2 + 32)
            x3 = unsafe(x3 + 32)
            ich += 32u
        }
        for [unroll_full] (b in range(4)) {
            var s = acc[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row0 < ddim && uint(b) < nrows) {
                y[uint(b) * ddim + row0] = s
            }
        }
    }
}

// Fused fixed-B W1|W3 GEMV + swiglu (batch twin of MetalQ8GemvW13Sw): each subgroup dots the
// gate row r AND up row r+hidden, writing silu(gate)*up directly — replaces the w1/w3/swiglu
// triple with one dispatch. B2 form serves B <= 2; B4 serves 3..4.
class MetalGemvW13SwB2 {
    @ssbo @binding = 0 wq : array<byte4>    // cat W1|W3 rows, byte4 view
    @ssbo @binding = 1 wsf : array<float>   // scales, f32 view
    @ssbo @binding = 2 wsh : array<float16> // ... same buffer, halfword view (wscale_f16)
    @ssbo @binding = 3 x : array<float4>    // [B x ndim] f32 rows, float4 view
    @ssbo @binding = 4 y : array<float>     // [B x hidden] silu(gate)*up
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 hidden : uint     // output rows AND the gate/up row split
    @uniform @binding = 7 s16 : uint
    @workgroup txp : float4[128]            // [2 streams x 64 float4] X panel chunk

    [metal_kernel(name="metal_gemv_w13sw_b2_msl")]
    def metal_gemv_w13sw_b2 {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, hidden - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let ns = ndim / 32u
        let wbg = row * nb4              // gate weight row (W1)
        let wbu = (row + hidden) * nb4   // up weight row (W3)
        let sbg = row * ns
        let sbu = (row + hidden) * ns
        var accg : float[2]
        var accu : float[2]
        var cb = 0u
        while (cb + 64u <= nb4) {
            barrier()                       // previous round done reading the panel
            txp[lid] = x[(lid / 64u) * nb4 + cb + lid % 64u]
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let svg = s16 != 0u ? float(wsh[sbg + k / 8u]) : wsf[sbg + k / 8u]
                let svu = s16 != 0u ? float(wsh[sbu + k / 8u]) : wsf[sbu + k / 8u]
                let wg = float4(int4(wq[wbg + k])) * svg
                let wu = float4(int4(wq[wbu + k])) * svu
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(2)) {
                    let xv = txp[uint(b) * 64u + kt]
                    accg[b] += wg.x * xv.x + wg.y * xv.y + wg.z * xv.z + wg.w * xv.w
                    accu[b] += wu.x * xv.x + wu.y * xv.y + wu.z * xv.z + wu.w * xv.w
                }
            }
            cb += 64u
        }
        var k = cb + lane
        while (k < nb4) {
            let svg = s16 != 0u ? float(wsh[sbg + k / 8u]) : wsf[sbg + k / 8u]
            let svu = s16 != 0u ? float(wsh[sbu + k / 8u]) : wsf[sbu + k / 8u]
            let wg = float4(int4(wq[wbg + k])) * svg
            let wu = float4(int4(wq[wbu + k])) * svu
            for [unroll_full] (b in range(2)) {
                let xv = x[uint(b) * nb4 + k]
                accg[b] += wg.x * xv.x + wg.y * xv.y + wg.z * xv.z + wg.w * xv.w
                accu[b] += wu.x * xv.x + wu.y * xv.y + wu.z * xv.z + wu.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(2)) {
            let g = simd_sum(accg[b])
            let u = simd_sum(accu[b])
            if (lane == 0u) {
                y[uint(b) * hidden + row] = (g / (1.0 + exp(-g))) * u
            }
        }
    }
}

class MetalGemvW13SwB4 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 wsf : array<float>
    @ssbo @binding = 2 wsh : array<float16>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 hidden : uint
    @uniform @binding = 7 s16 : uint
    @workgroup txp : float4[256]            // [4 streams x 64 float4] X panel chunk

    [metal_kernel(name="metal_gemv_w13sw_b4_msl")]
    def metal_gemv_w13sw_b4 {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, hidden - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let ns = ndim / 32u
        let wbg = row * nb4
        let wbu = (row + hidden) * nb4
        let sbg = row * ns
        let sbu = (row + hidden) * ns
        var accg : float[4]
        var accu : float[4]
        var cb = 0u
        while (cb + 64u <= nb4) {
            barrier()
            for [unroll_full] (j in range(2)) {
                let slot = uint(j) * 128u + lid
                txp[slot] = x[(slot / 64u) * nb4 + cb + slot % 64u]
            }
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let svg = s16 != 0u ? float(wsh[sbg + k / 8u]) : wsf[sbg + k / 8u]
                let svu = s16 != 0u ? float(wsh[sbu + k / 8u]) : wsf[sbu + k / 8u]
                let wg = float4(int4(wq[wbg + k])) * svg
                let wu = float4(int4(wq[wbu + k])) * svu
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(4)) {
                    let xv = txp[uint(b) * 64u + kt]
                    accg[b] += wg.x * xv.x + wg.y * xv.y + wg.z * xv.z + wg.w * xv.w
                    accu[b] += wu.x * xv.x + wu.y * xv.y + wu.z * xv.z + wu.w * xv.w
                }
            }
            cb += 64u
        }
        var k = cb + lane
        while (k < nb4) {
            let svg = s16 != 0u ? float(wsh[sbg + k / 8u]) : wsf[sbg + k / 8u]
            let svu = s16 != 0u ? float(wsh[sbu + k / 8u]) : wsf[sbu + k / 8u]
            let wg = float4(int4(wq[wbg + k])) * svg
            let wu = float4(int4(wq[wbu + k])) * svu
            for [unroll_full] (b in range(4)) {
                let xv = x[uint(b) * nb4 + k]
                accg[b] += wg.x * xv.x + wg.y * xv.y + wg.z * xv.z + wg.w * xv.w
                accu[b] += wu.x * xv.x + wu.y * xv.y + wu.z * xv.z + wu.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(4)) {
            let g = simd_sum(accg[b])
            let u = simd_sum(accu[b])
            if (lane == 0u) {
                y[uint(b) * hidden + row] = (g / (1.0 + exp(-g))) * u
            }
        }
    }
}

// M-pad-32 batch GEMM (B >= 5, cost B-independent to 32 rows): MulMm's staging discipline on a
// 32x32 tile, 110 instr/k-block vs the old 235 (1.16-1.33x on every decode shape). Pad token rows
// carry stale bytes by design — C rows are per-token independent, pad Y rows are never read back.
class MetalQ8GemmB {
    @ssbo @binding = 0 wq : array<byte4>    // W [d x n] row-major Q8 quants, byte4 view
    @ssbo @binding = 1 wsf : array<float>   // W scales [d x n/32], f32 view
    @ssbo @binding = 2 wsh : array<float16> // ... the same buffer, halfword view (wscale_f16)
    @ssbo @binding = 3 x : array<float4>    // X [M' x n] f32 rows (M' = 32-padded B)
    @ssbo @binding = 4 y : array<float>     // Y [M' x d]
    @uniform @binding = 5 kdim : uint       // n (K)
    @uniform @binding = 6 ndim : uint       // d (N)
    @uniform @binding = 7 s16 : uint
    @workgroup tw : float16[1024]           // W: 4 k-tiles x 4 row-tiles of 8x8, (k row, wrow col)
    @workgroup tx : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, (token row, k col)

    [metal_kernel(name="metal_q8_gemm_b_msl")]
    def metal_q8_gemm_b {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        var mc : simdgroup_float8x8[4]      // this simdgroup's 16x16 quadrant: 2 token x 2 wrow tiles
        var ma : simdgroup_half8x8[2]
        var mb : simdgroup_half8x8[2]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // staging split: thread handles row lr (0..31) x k-eighth il (0..3) — 8 elems of W AND X
        let lr = lid / 4u
        let il = lid % 4u
        let tW0 = (il * 4u + lr / 8u) * 64u + (lr % 8u)
        let tX0 = (il * 4u + lr / 8u) * 64u + (lr % 8u) * 8u
        let aQ = (sg % 2u) * 128u           // wrow tile-pair base within a k-tile slab
        let bQ = (sg / 2u) * 128u           // token tile-pair base
        var wp = unsafe(addr(wq[(nBase + lr) * kdim4 + il * 2u]))
        var spf = unsafe(addr(wsf[(nBase + lr) * nkb]))
        var sph = unsafe(addr(wsh[(nBase + lr) * nkb]))
        var xp = unsafe(addr(x[(mBase + lr) * kdim4 + il * 2u]))
        var kb = 0u
        while (kb < nkb) {
            let sA = s16 != 0u ? float(unsafe(sph[0])) : unsafe(spf[0])
            let q0 = unsafe(wp[0])
            let q1 = unsafe(wp[1])
            let b0 = unsafe(xp[0])
            let b1 = unsafe(xp[1])
            let w40 = float4(int4(q0)) * sA
            let w41 = float4(int4(q1)) * sA
            barrier()                       // previous math done reading the tiles
            tw[tW0] = float16(w40.x)
            tw[tW0 + 8u] = float16(w40.y)
            tw[tW0 + 16u] = float16(w40.z)
            tw[tW0 + 24u] = float16(w40.w)
            tw[tW0 + 32u] = float16(w41.x)
            tw[tW0 + 40u] = float16(w41.y)
            tw[tW0 + 48u] = float16(w41.z)
            tw[tW0 + 56u] = float16(w41.w)
            tx[tX0] = float16(b0.x)
            tx[tX0 + 1u] = float16(b0.y)
            tx[tX0 + 2u] = float16(b0.z)
            tx[tX0 + 3u] = float16(b0.w)
            tx[tX0 + 4u] = float16(b1.x)
            tx[tX0 + 5u] = float16(b1.y)
            tx[tX0 + 6u] = float16(b1.z)
            tx[tX0 + 7u] = float16(b1.w)
            barrier()
            var acur = unsafe(addr(tw[aQ]))
            var bcur = unsafe(addr(tx[bQ]))
            for [unroll_full] (_kt in range(4)) {
                simdgroup_load(ma[0], acur, 0u, 8u)
                simdgroup_load(ma[1], acur, 64u, 8u)
                simdgroup_load(mb[0], bcur, 0u, 8u)
                simdgroup_load(mb[1], bcur, 64u, 8u)
                simdgroup_multiply_accumulate(mc[0], mb[0], ma[0], mc[0])
                simdgroup_multiply_accumulate(mc[1], mb[0], ma[1], mc[1])
                simdgroup_multiply_accumulate(mc[2], mb[1], ma[0], mc[2])
                simdgroup_multiply_accumulate(mc[3], mb[1], ma[1], mc[3])
                acur = unsafe(acur + 256)
                bcur = unsafe(bcur + 256)
            }
            wp = unsafe(wp + 8u)
            spf = unsafe(spf + 1u)
            sph = unsafe(sph + 1u)
            xp = unsafe(xp + 8u)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 16u
        simdgroup_store(mc[0], y, row0 * ndim + col0, ndim)
        simdgroup_store(mc[1], y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(mc[2], y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(mc[3], y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// Split-K twin of MetalQ8GemmB: grid.y gains a k-slice axis, each threadgroup walks its own
// partial-C plane, and a separate MetalSkReduce dispatch sums them (no float atomics,
// deterministic). At M-pad-32 this is the only way to grow the grid past d/32 on starved sites.
class MetalQ8GemmBSk {
    @ssbo @binding = 0 wq : array<byte4>    // W [d x n] row-major Q8 quants, byte4 view
    @ssbo @binding = 1 wsf : array<float>   // W scales [d x n/32], f32 view
    @ssbo @binding = 2 wsh : array<float16> // ... the same buffer, halfword view (wscale_f16)
    @ssbo @binding = 3 x : array<float4>    // X [32 x n] f32 rows
    @ssbo @binding = 4 part : array<float>  // [ksplit x 32 x d] partial-C planes
    @uniform @binding = 5 kdim : uint       // n (K)
    @uniform @binding = 6 ndim : uint       // d (N)
    @uniform @binding = 7 s16 : uint
    @uniform @binding = 8 ksplit : uint
    @workgroup tw : float16[1024]           // W: 4 k-tiles x 4 row-tiles of 8x8, (k row, wrow col)
    @workgroup tx : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, (token row, k col)

    [metal_kernel(name="metal_q8_gemm_b_sk_msl")]
    def metal_q8_gemm_b_sk {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let sl = gl_WorkGroupID.y
        var mc : simdgroup_float8x8[4]      // this simdgroup's 16x16 quadrant: 2 token x 2 wrow tiles
        var ma : simdgroup_half8x8[2]
        var mb : simdgroup_half8x8[2]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let kbn = nkb / ksplit
        let kb0 = sl * kbn
        // staging split: thread handles row lr (0..31) x k-eighth il (0..3) — 8 elems of W AND X
        let lr = lid / 4u
        let il = lid % 4u
        let tW0 = (il * 4u + lr / 8u) * 64u + (lr % 8u)
        let tX0 = (il * 4u + lr / 8u) * 64u + (lr % 8u) * 8u
        let aQ = (sg % 2u) * 128u           // wrow tile-pair base within a k-tile slab
        let bQ = (sg / 2u) * 128u           // token tile-pair base
        var wp = unsafe(addr(wq[(nBase + lr) * kdim4 + kb0 * 8u + il * 2u]))
        var spf = unsafe(addr(wsf[(nBase + lr) * nkb + kb0]))
        var sph = unsafe(addr(wsh[(nBase + lr) * nkb + kb0]))
        var xp = unsafe(addr(x[(mBase + lr) * kdim4 + kb0 * 8u + il * 2u]))
        var kb = 0u
        while (kb < kbn) {
            let sA = s16 != 0u ? float(unsafe(sph[0])) : unsafe(spf[0])
            let q0 = unsafe(wp[0])
            let q1 = unsafe(wp[1])
            let b0 = unsafe(xp[0])
            let b1 = unsafe(xp[1])
            let w40 = float4(int4(q0)) * sA
            let w41 = float4(int4(q1)) * sA
            barrier()                       // previous math done reading the tiles
            tw[tW0] = float16(w40.x)
            tw[tW0 + 8u] = float16(w40.y)
            tw[tW0 + 16u] = float16(w40.z)
            tw[tW0 + 24u] = float16(w40.w)
            tw[tW0 + 32u] = float16(w41.x)
            tw[tW0 + 40u] = float16(w41.y)
            tw[tW0 + 48u] = float16(w41.z)
            tw[tW0 + 56u] = float16(w41.w)
            tx[tX0] = float16(b0.x)
            tx[tX0 + 1u] = float16(b0.y)
            tx[tX0 + 2u] = float16(b0.z)
            tx[tX0 + 3u] = float16(b0.w)
            tx[tX0 + 4u] = float16(b1.x)
            tx[tX0 + 5u] = float16(b1.y)
            tx[tX0 + 6u] = float16(b1.z)
            tx[tX0 + 7u] = float16(b1.w)
            barrier()
            var acur = unsafe(addr(tw[aQ]))
            var bcur = unsafe(addr(tx[bQ]))
            for [unroll_full] (_kt in range(4)) {
                simdgroup_load(ma[0], acur, 0u, 8u)
                simdgroup_load(ma[1], acur, 64u, 8u)
                simdgroup_load(mb[0], bcur, 0u, 8u)
                simdgroup_load(mb[1], bcur, 64u, 8u)
                simdgroup_multiply_accumulate(mc[0], mb[0], ma[0], mc[0])
                simdgroup_multiply_accumulate(mc[1], mb[0], ma[1], mc[1])
                simdgroup_multiply_accumulate(mc[2], mb[1], ma[0], mc[2])
                simdgroup_multiply_accumulate(mc[3], mb[1], ma[1], mc[3])
                acur = unsafe(acur + 256)
                bcur = unsafe(bcur + 256)
            }
            wp = unsafe(wp + 8u)
            spf = unsafe(spf + 1u)
            sph = unsafe(sph + 1u)
            xp = unsafe(xp + 8u)
            kb++
        }
        let pbase = sl * 32u * ndim
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 16u
        simdgroup_store(mc[0], part, pbase + row0 * ndim + col0, ndim)
        simdgroup_store(mc[1], part, pbase + row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(mc[2], part, pbase + (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(mc[3], part, pbase + (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// the split-K reduce: y = sum of the ksplit partial planes (one thread per output element);
// total = 32 * d is both the element count and the plane stride
class MetalSkReduce {
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 y : array<float>
    @uniform @binding = 2 total : uint
    @uniform @binding = 3 ksplit : uint

    [metal_kernel(name="metal_sk_reduce_msl")]
    def metal_sk_reduce {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        var acc = 0.0
        var s = 0u
        while (s < ksplit) {
            acc += part[s * total + gid]
            s++
        }
        y[gid] = acc
    }
}

// The 64-wide twin of MulMm's 32(M) x 64(N) tile: half the threadgroups (d/64) at ~29% fewer
// instructions per element — wins where the grid stays fat (e.g. the vocab classifier). Each
// simdgroup owns a 16x32 quadrant (2 token x 4 wrow tiles).
class MetalQ8Gemm64B {
    @ssbo @binding = 0 wq : array<byte4>    // W [d x n] row-major Q8 quants, byte4 view
    @ssbo @binding = 1 wsf : array<float>   // W scales [d x n/32], f32 view
    @ssbo @binding = 2 wsh : array<float16> // ... the same buffer, halfword view (wscale_f16)
    @ssbo @binding = 3 x : array<float4>    // X [M' x n] f32 rows (M' = 32-padded B)
    @ssbo @binding = 4 y : array<float>     // Y [M' x d]
    @uniform @binding = 5 kdim : uint       // n (K)
    @uniform @binding = 6 ndim : uint       // d (N)
    @uniform @binding = 7 s16 : uint
    @workgroup tw : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, (k row, wrow col)
    @workgroup tx : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, (token row, k col)

    [metal_kernel(name="metal_q8_gemm64_b_msl")]
    def metal_q8_gemm64_b {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        var mc : simdgroup_float8x8[8]      // this simdgroup's 16x32 quadrant: 2 token x 4 wrow tiles
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]                // dequantized W staging regs (this thread's k-half)
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // W split: thread stages 16 elems — row lr0 (0..63), k-half il0 (0..1)
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let tW0 = (il0 * 2u * 8u + lr0 / 8u) * 64u + (lr0 % 8u)
        // X split: thread stages 8 elems — token lr1 (0..31), k-eighth il1 (0..3)
        let lr1 = lid / 4u
        let il1 = lid % 4u
        let tX0 = (il1 * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aQ = (sg % 2u) * 256u           // wrow tile-quad base within a k-tile slab
        let bQ = (sg / 2u) * 128u           // token tile-pair base
        var wp = unsafe(addr(wq[(nBase + lr0) * kdim4 + il0 * 4u]))
        var spf = unsafe(addr(wsf[(nBase + lr0) * nkb]))
        var sph = unsafe(addr(wsh[(nBase + lr0) * nkb]))
        var xp = unsafe(addr(x[(mBase + lr1) * kdim4 + il1 * 2u]))
        var kb = 0u
        while (kb < nkb) {
            let sA = s16 != 0u ? float(unsafe(sph[0])) : unsafe(spf[0])
            let b0 = unsafe(xp[0])
            let b1 = unsafe(xp[1])
            for [unroll_full] (g in range(4)) {
                let w4 = float4(int4(unsafe(wp[uint(g)]))) * sA
                va[g * 4] = float16(w4.x)
                va[g * 4 + 1] = float16(w4.y)
                va[g * 4 + 2] = float16(w4.z)
                va[g * 4 + 3] = float16(w4.w)
            }
            barrier()                       // previous math done reading the tiles
            for [unroll_full] (i in range(16)) {
                tw[tW0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tx[tX0] = float16(b0.x)
            tx[tX0 + 1u] = float16(b0.y)
            tx[tX0 + 2u] = float16(b0.z)
            tx[tX0 + 3u] = float16(b0.w)
            tx[tX0 + 4u] = float16(b1.x)
            tx[tX0 + 5u] = float16(b1.y)
            tx[tX0 + 6u] = float16(b1.z)
            tx[tX0 + 7u] = float16(b1.w)
            barrier()
            var acur = unsafe(addr(tw[aQ]))
            var bcur = unsafe(addr(tx[bQ]))
            for [unroll_full] (_kt in range(4)) {
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(ma[i], acur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(2)) {
                    simdgroup_load(mb[i], bcur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(8)) {
                    simdgroup_multiply_accumulate(mc[i], mb[i / 4], ma[i % 4], mc[i])
                }
                acur = unsafe(acur + 512)
                bcur = unsafe(bcur + 256)
            }
            wp = unsafe(wp + 8u)
            spf = unsafe(spf + 1u)
            sph = unsafe(sph + 1u)
            xp = unsafe(xp + 8u)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// Batched single-query attention, single-tg form: grid (nheads x B), same simdgroup-cooperative
// math as MetalSqAttnF16, but each row reads its mirror slice through the row table and its
// roped q lives in the fused QKV buffer at stride qstride.
class MetalSqAttnBF16 {
    @ssbo @binding = 0 kc : array<float16>  // the whole mirror arena, f16 view
    @ssbo @binding = 1 vc : array<float16>
    @ssbo @binding = 2 q : array<float>     // [B x qstride] roped queries (the fused QKV rows)
    @ssbo @binding = 3 yo : array<float>    // [B x nheads*hs] attention out
    @ssbo @binding = 4 rt : array<uint4>    // per-row (k base, v base, cap_rows, cnt) — arena elems
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @workgroup scores : float[128]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_b16_msl")]
    def metal_sq_attn_b16 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rrow = rt[bb]
        let cnt = rrow.w
        let kb0 = rrow.x + lay * rrow.z * kv_dim
        let vb0 = rrow.y + lay * rrow.z * kv_dim
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let yoff = bb * (nheads * hs) + h * hs
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        var j = sg
        while (j < cnt) {
            let kb = kb0 + j * kv_dim + khoff + sl * epl
            var s = q0 * float(kc[kb])
            if (epl > 1u) {
                s += q1 * float(kc[kb + 1u])
            }
            if (epl > 2u) {
                s += q2 * float(kc[kb + 2u])
            }
            if (epl > 3u) {
                s += q3 * float(kc[kb + 3u])
            }
            s = simd_sum(s)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= cnt) {
            acc0 += scores[j] * float(vc[vb0 + j * kv_dim + khoff + lid])
            acc1 += scores[j + 1u] * float(vc[vb0 + (j + 1u) * kv_dim + khoff + lid])
            acc2 += scores[j + 2u] * float(vc[vb0 + (j + 2u) * kv_dim + khoff + lid])
            acc3 += scores[j + 3u] * float(vc[vb0 + (j + 3u) * kv_dim + khoff + lid])
            j += 4u
        }
        while (j < cnt) {
            acc0 += scores[j] * float(vc[vb0 + j * kv_dim + khoff + lid])
            j++
        }
        yo[yoff + lid] = (acc0 + acc1 + acc2 + acc3) / tot
    }
}

// f32-mirror twin.
class MetalSqAttnBF32 {
    @ssbo @binding = 0 kc : array<float>
    @ssbo @binding = 1 vc : array<float>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 yo : array<float>
    @ssbo @binding = 4 rt : array<uint4>
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @workgroup scores : float[128]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_b32_msl")]
    def metal_sq_attn_b32 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rrow = rt[bb]
        let cnt = rrow.w
        let kb0 = rrow.x + lay * rrow.z * kv_dim
        let vb0 = rrow.y + lay * rrow.z * kv_dim
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let yoff = bb * (nheads * hs) + h * hs
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        var j = sg
        while (j < cnt) {
            let kb = kb0 + j * kv_dim + khoff + sl * epl
            var s = q0 * kc[kb]
            if (epl > 1u) {
                s += q1 * kc[kb + 1u]
            }
            if (epl > 2u) {
                s += q2 * kc[kb + 2u]
            }
            if (epl > 3u) {
                s += q3 * kc[kb + 3u]
            }
            s = simd_sum(s)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= cnt) {
            acc0 += scores[j] * vc[vb0 + j * kv_dim + khoff + lid]
            acc1 += scores[j + 1u] * vc[vb0 + (j + 1u) * kv_dim + khoff + lid]
            acc2 += scores[j + 2u] * vc[vb0 + (j + 2u) * kv_dim + khoff + lid]
            acc3 += scores[j + 3u] * vc[vb0 + (j + 3u) * kv_dim + khoff + lid]
            j += 4u
        }
        while (j < cnt) {
            acc0 += scores[j] * vc[vb0 + j * kv_dim + khoff + lid]
            j++
        }
        yo[yoff + lid] = (acc0 + acc1 + acc2 + acc3) / tot
    }
}

// Batched chunked split-K twin of MetalSqAttnQ8 — grid (nheads x nchmax x B), ragged depths
// share one dispatch via per-row chunk counts. Row-table entries carry arena BYTE offsets
// (rowb = nblk*34, always even), so the half-scale view indexes at byte/2 exactly.
class MetalSqAttnBQ8 {
    @ssbo @binding = 0 ksh : array<float16> // the whole mirror arena, half-scale view
    @ssbo @binding = 1 kqb : array<int8>    // the SAME arena, byte view
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<int8>
    @ssbo @binding = 4 q : array<float>     // [B x qstride] roped queries (the fused QKV rows)
    @ssbo @binding = 5 yo : array<float>    // [B x nheads*hs] attention out
    @ssbo @binding = 6 rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @workgroup scores : float[128]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_b_q8_msl")]
    def metal_sq_attn_b_q8 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rrow = rt[bb]
        let cnt = rrow.w
        let rowb = nblk * 34u
        let kb0b = rrow.x + lay * rrow.z * rowb   // layer slab base, BYTES (even)
        let vb0b = rrow.y + lay * rrow.z * rowb
        let kb0h = kb0b / 2u                      // ... and HALFWORDS for the scale view
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * hs
        let yoff = bb * (nheads * hs) + h * hs
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let hb = (sl * epl) / 32u
        let ib = (sl * epl) % 32u
        var j = sg
        while (j < cnt) {
            let blk = j * nblk + khblk + hb
            let d = float(ksh[kb0h + blk * 17u])
            let kb = kb0b + blk * 34u + 2u + ib
            var s = q0 * float(kqb[kb])
            if (epl > 1u) {
                s += q1 * float(kqb[kb + 1u])
            }
            if (epl > 2u) {
                s += q2 * float(kqb[kb + 2u])
            }
            if (epl > 3u) {
                s += q3 * float(kqb[kb + 3u])
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        let vblk0 = khblk + lid / 32u
        let vin = lid % 32u
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= cnt) {
            let b0 = j * nblk + vblk0
            let b1 = (j + 1u) * nblk + vblk0
            let b2 = (j + 2u) * nblk + vblk0
            let b3 = (j + 3u) * nblk + vblk0
            acc0 += scores[j] * float(vqb[vb0b + b0 * 34u + 2u + vin]) * float(vsh[vb0h + b0 * 17u])
            acc1 += scores[j + 1u] * float(vqb[vb0b + b1 * 34u + 2u + vin]) * float(vsh[vb0h + b1 * 17u])
            acc2 += scores[j + 2u] * float(vqb[vb0b + b2 * 34u + 2u + vin]) * float(vsh[vb0h + b2 * 17u])
            acc3 += scores[j + 3u] * float(vqb[vb0b + b3 * 34u + 2u + vin]) * float(vsh[vb0h + b3 * 17u])
            j += 4u
        }
        while (j < cnt) {
            let b0 = j * nblk + vblk0
            acc0 += scores[j] * float(vqb[vb0b + b0 * 34u + 2u + vin]) * float(vsh[vb0h + b0 * 17u])
            j++
        }
        yo[yoff + lid] = (acc0 + acc1 + acc2 + acc3) / tot
    }
}

// tq4-mirror batch twin (nibble reads over the byte-offset row table — see MetalSqAttnBQ8/Tq4;
// rowb = nblk*18).
class MetalSqAttnBTq4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 kqb : array<uint8>
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<uint8>
    @ssbo @binding = 4 q : array<float>     // [B x qstride] roped + rotated queries
    @ssbo @binding = 5 yo : array<float>    // [B x nheads*hs] attention out (rotated basis)
    @ssbo @binding = 6 rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @workgroup scores : float[128]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_b_tq4_msl")]
    def metal_sq_attn_b_tq4 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rrow = rt[bb]
        let cnt = rrow.w
        let rowb = nblk * 18u
        let kb0b = rrow.x + lay * rrow.z * rowb
        let vb0b = rrow.y + lay * rrow.z * rowb
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * hs
        let yoff = bb * (nheads * hs) + h * hs
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let hb = (sl * epl) / 32u
        let inb = (sl * epl) % 32u
        var j = sg
        while (j < cnt) {
            let blk = j * nblk + khblk + hb
            let d = float(ksh[kb0h + blk * 9u])
            let kb = kb0b + blk * 18u + 2u + inb / 2u
            let by0 = uint(kqb[kb])
            var s = 0.0
            if (epl == 1u) {
                let nib = (inb & 1u) == 1u ? (by0 >> 4u) : (by0 & 15u)
                s = q0 * float(int(nib) - 8)
            } else {
                s = q0 * float(int(by0 & 15u) - 8)
                s += q1 * float(int(by0 >> 4u) - 8)
                if (epl > 2u) {
                    let by1 = uint(kqb[kb + 1u])
                    s += q2 * float(int(by1 & 15u) - 8)
                    s += q3 * float(int(by1 >> 4u) - 8)
                }
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        let vblk0 = khblk + lid / 32u
        let vbyte = (lid % 32u) / 2u
        let hi = (lid & 1u) == 1u
        var acc0 = 0.0
        j = 0u
        while (j < cnt) {
            let blk = j * nblk + vblk0
            let by = uint(vqb[vb0b + blk * 18u + 2u + vbyte])
            let nib = hi ? (by >> 4u) : (by & 15u)
            acc0 += scores[j] * float(int(nib) - 8) * float(vsh[vb0h + blk * 9u])
            j++
        }
        yo[yoff + lid] = acc0 / tot
    }
}

class MetalSqAttnPartBF16 {
    @ssbo @binding = 0 kc : array<float16>
    @ssbo @binding = 1 vc : array<float16>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 part : array<float>  // [B x nheads x nchmax x (hs+2)]
    @ssbo @binding = 4 rt : array<uint4>
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @uniform @binding = 12 nchmax : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_part_b16_msl")]
    def metal_sq_attn_part_b16 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let ci = (wg / nheads) % nchmax
        let bb = wg / (nheads * nchmax)
        let rrow = rt[bb]
        let cnt = rrow.w
        let c0 = ci * 64u
        if (c0 >= cnt) {   // this row is shallower than the chunk — whole tg exits together
            return
        }
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let kb0 = rrow.x + lay * rrow.z * kv_dim
        let vb0 = rrow.y + lay * rrow.z * kv_dim
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        var j = sg
        while (j < nrows) {
            let kb = kb0 + (c0 + j) * kv_dim + khoff + sl * epl
            var s = q0 * float(kc[kb])
            if (epl > 1u) {
                s += q1 * float(kc[kb + 1u])
            }
            if (epl > 2u) {
                s += q2 * float(kc[kb + 2u])
            }
            if (epl > 3u) {
                s += q3 * float(kc[kb + 3u])
            }
            s = simd_sum(s)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < nrows) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        var l = 0.0
        j = lid
        while (j < nrows) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= nrows) {
            acc0 += scores[j] * float(vc[vb0 + (c0 + j) * kv_dim + khoff + lid])
            acc1 += scores[j + 1u] * float(vc[vb0 + (c0 + j + 1u) * kv_dim + khoff + lid])
            acc2 += scores[j + 2u] * float(vc[vb0 + (c0 + j + 2u) * kv_dim + khoff + lid])
            acc3 += scores[j + 3u] * float(vc[vb0 + (c0 + j + 3u) * kv_dim + khoff + lid])
            j += 4u
        }
        while (j < nrows) {
            acc0 += scores[j] * float(vc[vb0 + (c0 + j) * kv_dim + khoff + lid])
            j++
        }
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = acc0 + acc1 + acc2 + acc3
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// f32-mirror twin.
class MetalSqAttnPartBF32 {
    @ssbo @binding = 0 kc : array<float>
    @ssbo @binding = 1 vc : array<float>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 part : array<float>
    @ssbo @binding = 4 rt : array<uint4>
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @uniform @binding = 12 nchmax : uint
    @workgroup scores : float[64]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_part_b32_msl")]
    def metal_sq_attn_part_b32 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let ci = (wg / nheads) % nchmax
        let bb = wg / (nheads * nchmax)
        let rrow = rt[bb]
        let cnt = rrow.w
        let c0 = ci * 64u
        if (c0 >= cnt) {
            return
        }
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let kb0 = rrow.x + lay * rrow.z * kv_dim
        let vb0 = rrow.y + lay * rrow.z * kv_dim
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        var j = sg
        while (j < nrows) {
            let kb = kb0 + (c0 + j) * kv_dim + khoff + sl * epl
            var s = q0 * kc[kb]
            if (epl > 1u) {
                s += q1 * kc[kb + 1u]
            }
            if (epl > 2u) {
                s += q2 * kc[kb + 2u]
            }
            if (epl > 3u) {
                s += q3 * kc[kb + 3u]
            }
            s = simd_sum(s)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < nrows) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()
        var l = 0.0
        j = lid
        while (j < nrows) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= nrows) {
            acc0 += scores[j] * vc[vb0 + (c0 + j) * kv_dim + khoff + lid]
            acc1 += scores[j + 1u] * vc[vb0 + (c0 + j + 1u) * kv_dim + khoff + lid]
            acc2 += scores[j + 2u] * vc[vb0 + (c0 + j + 2u) * kv_dim + khoff + lid]
            acc3 += scores[j + 3u] * vc[vb0 + (c0 + j + 3u) * kv_dim + khoff + lid]
            j += 4u
        }
        while (j < nrows) {
            acc0 += scores[j] * vc[vb0 + (c0 + j) * kv_dim + khoff + lid]
            j++
        }
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = acc0 + acc1 + acc2 + acc3
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// q8_0-mirror batched chunked twin — byte-offset row table + dual-view block reads; partials
// stay f32, so MetalSqAttnCombB serves this form unchanged.
class MetalSqAttnPartBQ8 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 kqb : array<int8>
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<int8>
    @ssbo @binding = 4 q : array<float>
    @ssbo @binding = 5 part : array<float>  // [B x nheads x nchmax x (hs+2)]
    @ssbo @binding = 6 rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 nchmax : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_part_b_q8_msl")]
    def metal_sq_attn_part_b_q8 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let ci = (wg / nheads) % nchmax
        let bb = wg / (nheads * nchmax)
        let rrow = rt[bb]
        let cnt = rrow.w
        let c0 = ci * 64u
        if (c0 >= cnt) {   // this row is shallower than the chunk — whole tg exits together
            return
        }
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rowb = nblk * 34u
        let kb0b = rrow.x + lay * rrow.z * rowb
        let vb0b = rrow.y + lay * rrow.z * rowb
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * hs
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let hb = (sl * epl) / 32u
        let ib = (sl * epl) % 32u
        var j = sg
        while (j < nrows) {
            let blk = (c0 + j) * nblk + khblk + hb
            let d = float(ksh[kb0h + blk * 17u])
            let kb = kb0b + blk * 34u + 2u + ib
            var s = q0 * float(kqb[kb])
            if (epl > 1u) {
                s += q1 * float(kqb[kb + 1u])
            }
            if (epl > 2u) {
                s += q2 * float(kqb[kb + 2u])
            }
            if (epl > 3u) {
                s += q3 * float(kqb[kb + 3u])
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < nrows) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        var l = 0.0
        j = lid
        while (j < nrows) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        let vblk0 = khblk + lid / 32u
        let vin = lid % 32u
        var acc0 = 0.0
        var acc1 = 0.0
        var acc2 = 0.0
        var acc3 = 0.0
        j = 0u
        while (j + 4u <= nrows) {
            let b0 = (c0 + j) * nblk + vblk0
            let b1 = (c0 + j + 1u) * nblk + vblk0
            let b2 = (c0 + j + 2u) * nblk + vblk0
            let b3 = (c0 + j + 3u) * nblk + vblk0
            acc0 += scores[j] * float(vqb[vb0b + b0 * 34u + 2u + vin]) * float(vsh[vb0h + b0 * 17u])
            acc1 += scores[j + 1u] * float(vqb[vb0b + b1 * 34u + 2u + vin]) * float(vsh[vb0h + b1 * 17u])
            acc2 += scores[j + 2u] * float(vqb[vb0b + b2 * 34u + 2u + vin]) * float(vsh[vb0h + b2 * 17u])
            acc3 += scores[j + 3u] * float(vqb[vb0b + b3 * 34u + 2u + vin]) * float(vsh[vb0h + b3 * 17u])
            j += 4u
        }
        while (j < nrows) {
            let b0 = (c0 + j) * nblk + vblk0
            acc0 += scores[j] * float(vqb[vb0b + b0 * 34u + 2u + vin]) * float(vsh[vb0h + b0 * 17u])
            j++
        }
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = acc0 + acc1 + acc2 + acc3
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// tq4-mirror batched chunked twin — byte-offset row table + nibble reads; partials stay f32.
class MetalSqAttnPartBTq4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 kqb : array<uint8>
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<uint8>
    @ssbo @binding = 4 q : array<float>
    @ssbo @binding = 5 part : array<float>  // [B x nheads x nchmax x (hs+2)]
    @ssbo @binding = 6 rt : array<uint4>
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 nchmax : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_part_b_tq4_msl")]
    def metal_sq_attn_part_b_tq4 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let ci = (wg / nheads) % nchmax
        let bb = wg / (nheads * nchmax)
        let rrow = rt[bb]
        let cnt = rrow.w
        let c0 = ci * 64u
        if (c0 >= cnt) {   // this row is shallower than the chunk — whole tg exits together
            return
        }
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rowb = nblk * 18u
        let kb0b = rrow.x + lay * rrow.z * rowb
        let vb0b = rrow.y + lay * rrow.z * rowb
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * hs
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let hb = (sl * epl) / 32u
        let inb = (sl * epl) % 32u
        var j = sg
        while (j < nrows) {
            let blk = (c0 + j) * nblk + khblk + hb
            let d = float(ksh[kb0h + blk * 9u])
            let kb = kb0b + blk * 18u + 2u + inb / 2u
            let by0 = uint(kqb[kb])
            var s = 0.0
            if (epl == 1u) {
                let nib = (inb & 1u) == 1u ? (by0 >> 4u) : (by0 & 15u)
                s = q0 * float(int(nib) - 8)
            } else {
                s = q0 * float(int(by0 & 15u) - 8)
                s += q1 * float(int(by0 >> 4u) - 8)
                if (epl > 2u) {
                    let by1 = uint(kqb[kb + 1u])
                    s += q2 * float(int(by1 & 15u) - 8)
                    s += q3 * float(int(by1 >> 4u) - 8)
                }
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        var m = -3.0e38
        j = lid
        while (j < nrows) {
            m = max(m, scores[j])
            j += nt
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        var mm = red[0]
        var sgi = 1u
        while (sgi < gl_NumSubgroups) {
            mm = max(mm, red[sgi])
            sgi++
        }
        barrier()                           // red[] reused for the sum below
        var l = 0.0
        j = lid
        while (j < nrows) {
            let w = exp(scores[j] - mm)
            scores[j] = w
            l += w
            j += nt
        }
        l += simd_shuffle_xor(l, 16u)
        l += simd_shuffle_xor(l, 8u)
        l += simd_shuffle_xor(l, 4u)
        l += simd_shuffle_xor(l, 2u)
        l += simd_shuffle_xor(l, 1u)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = l
        }
        barrier()
        var tot = red[0]
        sgi = 1u
        while (sgi < gl_NumSubgroups) {
            tot += red[sgi]
            sgi++
        }
        let vblk0 = khblk + lid / 32u
        let vbyte = (lid % 32u) / 2u
        let hi = (lid & 1u) == 1u
        var acc0 = 0.0
        j = 0u
        while (j < nrows) {
            let blk = (c0 + j) * nblk + vblk0
            let by = uint(vqb[vb0b + blk * 18u + 2u + vbyte])
            let nib = hi ? (by >> 4u) : (by & 15u)
            acc0 += scores[j] * float(int(nib) - 8) * float(vsh[vb0h + blk * 9u])
            j++
        }
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = acc0
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// Batched cross-chunk combine: grid (nheads x B) — each row combines its OWN chunk count
// (from the table), holes past a shallow row's last chunk are never read.
class MetalSqAttnCombB {
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 yo : array<float>
    @ssbo @binding = 2 rt : array<uint4>
    @uniform @binding = 3 nchmax : uint
    @uniform @binding = 4 nheads : uint
    @uniform @binding = 5 hs : uint

    [metal_kernel(name="metal_sq_attn_comb_b_msl")]
    def metal_sq_attn_comb_b {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let cnt = rt[bb].w
        let nch = (cnt + 63u) / 64u
        let base = (bb * nheads + h) * nchmax
        var mm = -3.0e38
        var ci = 0u
        while (ci < nch) {
            mm = max(mm, part[(base + ci) * (hs + 2u) + hs])
            ci++
        }
        var acc = 0.0
        var l = 0.0
        ci = 0u
        while (ci < nch) {
            let pb = (base + ci) * (hs + 2u)
            let w = exp(part[pb + hs] - mm)
            acc += part[pb + lid] * w
            l += part[pb + hs + 1u] * w
            ci++
        }
        yo[bb * (nheads * hs) + h * hs + lid] = acc / l
    }
}

// Fused single-pass batched attention: beats llama.cpp fa_vec at every depth 512-16K. One
// simdgroup per (row, head, 32-slice-group), scores kept in-register (no divergent store), V
// accumulated lane-private (zero barriers). hs == 128 only; grid is HEAD-FASTEST for SLC locality.
class MetalSqAttnDF16 {
    @ssbo @binding = 0 kc : array<half4>
    @ssbo @binding = 1 vc : array<half4>
    @ssbo @binding = 2 q : array<float4>
    @ssbo @binding = 3 part : array<float>
    @ssbo @binding = 4 rt : array<uint4>
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @uniform @binding = 12 nsgs : uint
    @workgroup wacc : float[512]

    [metal_kernel(name="metal_sq_attn_d16_msl")]
    def metal_sq_attn_d16 {
        let h = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.z
        let gsg = gl_WorkGroupID.y * 4u + gl_SubgroupID
        let rrow = rt[bb]
        let cnt = rrow.w
        if (gsg >= nsgs || gsg * 32u >= cnt) {
            return
        }
        let sl = gl_SubgroupInvocationID
        let kb0 = rrow.x + lay * rrow.z * kv_dim
        let vb0 = rrow.y + lay * rrow.z * kv_dim
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let kv4 = kv_dim / 4u
        let q4 = float4(q[qoff / 4u + sl])
        let wo = gl_SubgroupID * 128u + sl * 4u
        wacc[wo] = 0.0
        wacc[wo + 1u] = 0.0
        wacc[wo + 2u] = 0.0
        wacc[wo + 3u] = 0.0
        var mm = -3.0e38
        var tot = 0.0
        var ss = gsg
        while (ss * 32u < cnt) {
            let c0 = ss * 32u
            let nrows = min(c0 + 32u, cnt) - c0
            var keep = -3.0e38
            var kp = unsafe(addr(kc[(kb0 + c0 * kv_dim + khoff) / 4u + sl]))
            var vp = unsafe(addr(vc[(vb0 + c0 * kv_dim + khoff) / 4u + sl]))
            var bacc = float4(0.0)
            if (nrows == 32u) {
                for [unroll_full] (i in range(32)) {
                    let p = q4 * float4(unsafe(kp[uint(i) * kv4]))
                    let sc = simd_sum(p.x + p.y + p.z + p.w)
                    keep = uint(i) == sl ? sc : keep
                }
            } else {
                var j = 0u
                while (j < nrows) {
                    let p = q4 * float4(unsafe(kp[0]))
                    let sc = simd_sum(p.x + p.y + p.z + p.w)
                    keep = j == sl ? sc : keep
                    kp = unsafe(kp + kv4)
                    j++
                }
            }
            keep *= scale   // invalid tail lanes stay hugely negative -> weight 0
            var bm = keep
            bm = max(bm, simd_shuffle_xor(bm, 16u))
            bm = max(bm, simd_shuffle_xor(bm, 8u))
            bm = max(bm, simd_shuffle_xor(bm, 4u))
            bm = max(bm, simd_shuffle_xor(bm, 2u))
            bm = max(bm, simd_shuffle_xor(bm, 1u))
            let w = exp(keep - bm)
            let btot = simd_sum(w)
            if (nrows == 32u) {
                for [unroll_full] (i in range(32)) {
                    bacc += simd_shuffle(w, uint(i)) * float4(unsafe(vp[uint(i) * kv4]))
                }
            } else {
                var j = 0u
                while (j < nrows) {
                    bacc += simd_shuffle(w, j) * float4(unsafe(vp[0]))
                    vp = unsafe(vp + kv4)
                    j++
                }
            }
            // online merge into the running slice-group state (first pass: r1 = exp(-inf) = 0)
            let nm = max(mm, bm)
            let r1 = exp(mm - nm)
            let r2 = exp(bm - nm)
            wacc[wo] = wacc[wo] * r1 + bacc.x * r2
            wacc[wo + 1u] = wacc[wo + 1u] * r1 + bacc.y * r2
            wacc[wo + 2u] = wacc[wo + 2u] * r1 + bacc.z * r2
            wacc[wo + 3u] = wacc[wo + 3u] * r1 + bacc.w * r2
            tot = tot * r1 + btot * r2
            mm = nm
            ss += nsgs
        }
        let pbase = ((bb * nheads + h) * nsgs + gsg) * (hs + 4u)
        let po = pbase + sl * 4u
        part[po] = wacc[wo]
        part[po + 1u] = wacc[wo + 1u]
        part[po + 2u] = wacc[wo + 2u]
        part[po + 3u] = wacc[wo + 3u]
        if (sl == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// f32-mirror twin — float4 view, no conversion.
class MetalSqAttnDF32 {
    @ssbo @binding = 0 kc : array<float4>
    @ssbo @binding = 1 vc : array<float4>
    @ssbo @binding = 2 q : array<float4>
    @ssbo @binding = 3 part : array<float>
    @ssbo @binding = 4 rt : array<uint4>
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @uniform @binding = 12 nsgs : uint
    @workgroup wacc : float[512]

    [metal_kernel(name="metal_sq_attn_d32_msl")]
    def metal_sq_attn_d32 {
        let h = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.z
        let gsg = gl_WorkGroupID.y * 4u + gl_SubgroupID
        let rrow = rt[bb]
        let cnt = rrow.w
        if (gsg >= nsgs || gsg * 32u >= cnt) {
            return
        }
        let sl = gl_SubgroupInvocationID
        let kb0 = rrow.x + lay * rrow.z * kv_dim
        let vb0 = rrow.y + lay * rrow.z * kv_dim
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let kv4 = kv_dim / 4u
        let q4 = float4(q[qoff / 4u + sl])
        let wo = gl_SubgroupID * 128u + sl * 4u
        wacc[wo] = 0.0
        wacc[wo + 1u] = 0.0
        wacc[wo + 2u] = 0.0
        wacc[wo + 3u] = 0.0
        var mm = -3.0e38
        var tot = 0.0
        var ss = gsg
        while (ss * 32u < cnt) {
            let c0 = ss * 32u
            let nrows = min(c0 + 32u, cnt) - c0
            var keep = -3.0e38
            var kp = unsafe(addr(kc[(kb0 + c0 * kv_dim + khoff) / 4u + sl]))
            var vp = unsafe(addr(vc[(vb0 + c0 * kv_dim + khoff) / 4u + sl]))
            var bacc = float4(0.0)
            if (nrows == 32u) {
                for [unroll_full] (i in range(32)) {
                    let p = q4 * unsafe(kp[uint(i) * kv4])
                    let sc = simd_sum(p.x + p.y + p.z + p.w)
                    keep = uint(i) == sl ? sc : keep
                }
            } else {
                var j = 0u
                while (j < nrows) {
                    let p = q4 * unsafe(kp[0])
                    let sc = simd_sum(p.x + p.y + p.z + p.w)
                    keep = j == sl ? sc : keep
                    kp = unsafe(kp + kv4)
                    j++
                }
            }
            keep *= scale
            var bm = keep
            bm = max(bm, simd_shuffle_xor(bm, 16u))
            bm = max(bm, simd_shuffle_xor(bm, 8u))
            bm = max(bm, simd_shuffle_xor(bm, 4u))
            bm = max(bm, simd_shuffle_xor(bm, 2u))
            bm = max(bm, simd_shuffle_xor(bm, 1u))
            let w = exp(keep - bm)
            let btot = simd_sum(w)
            if (nrows == 32u) {
                for [unroll_full] (i in range(32)) {
                    bacc += simd_shuffle(w, uint(i)) * unsafe(vp[uint(i) * kv4])
                }
            } else {
                var j = 0u
                while (j < nrows) {
                    bacc += simd_shuffle(w, j) * unsafe(vp[0])
                    vp = unsafe(vp + kv4)
                    j++
                }
            }
            let nm = max(mm, bm)
            let r1 = exp(mm - nm)
            let r2 = exp(bm - nm)
            wacc[wo] = wacc[wo] * r1 + bacc.x * r2
            wacc[wo + 1u] = wacc[wo + 1u] * r1 + bacc.y * r2
            wacc[wo + 2u] = wacc[wo + 2u] * r1 + bacc.z * r2
            wacc[wo + 3u] = wacc[wo + 3u] * r1 + bacc.w * r2
            tot = tot * r1 + btot * r2
            mm = nm
            ss += nsgs
        }
        let pbase = ((bb * nheads + h) * nsgs + gsg) * (hs + 4u)
        let po = pbase + sl * 4u
        part[po] = wacc[wo]
        part[po + 1u] = wacc[wo + 1u]
        part[po + 2u] = wacc[wo + 2u]
        part[po + 3u] = wacc[wo + 3u]
        if (sl == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// Cross-slice combine for the D entries: PARALLEL over entries (4 sgs online-merge their
// subsets with float4 loads — m and l arrive in ONE float4), a single tgmem cross-sg merge.
class MetalSqAttnCombD {
    @ssbo @binding = 0 part : array<float4>
    @ssbo @binding = 1 yo : array<float>
    @ssbo @binding = 2 rt : array<uint4>
    @uniform @binding = 3 nchmax : uint
    @uniform @binding = 4 nheads : uint
    @uniform @binding = 5 hs : uint
    @workgroup wacc : float[512]
    @workgroup wml : float[8]

    [metal_kernel(name="metal_sq_attn_comb_d_msl")]
    def metal_sq_attn_comb_d {
        let h = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.y
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let lid = gl_LocalInvocationID.x
        let cnt = rt[bb].w
        let nch = min((cnt + 31u) / 32u, nchmax)
        let s4 = (hs + 4u) / 4u
        let base4 = (bb * nheads + h) * nchmax * s4
        var mm = -3.0e38
        var tot = 0.0
        var acc4 = float4(0.0)
        var e = sg
        while (e < nch) {
            let eb = base4 + e * s4
            let ml = float4(part[eb + hs / 4u])
            let av = float4(part[eb + sl])
            let nm = max(mm, ml.x)
            let r1 = exp(mm - nm)
            let r2 = exp(ml.x - nm)
            acc4 = acc4 * r1 + av * r2
            tot = tot * r1 + ml.y * r2
            mm = nm
            e += gl_NumSubgroups
        }
        let wo = sg * 128u + sl * 4u
        wacc[wo] = acc4.x
        wacc[wo + 1u] = acc4.y
        wacc[wo + 2u] = acc4.z
        wacc[wo + 3u] = acc4.w
        if (sl == 0u) {
            wml[sg * 2u] = mm
            wml[sg * 2u + 1u] = tot
        }
        barrier()
        // final 4-way merge, one lane per output column
        let m0 = wml[0]
        let m1 = wml[2]
        let m2 = wml[4]
        let m3 = wml[6]
        let gm = max(max(m0, m1), max(m2, m3))
        let r0 = exp(m0 - gm)
        let r1 = exp(m1 - gm)
        let r2 = exp(m2 - gm)
        let r3 = exp(m3 - gm)
        let l = wml[1] * r0 + wml[3] * r1 + wml[5] * r2 + wml[7] * r3
        let acc = wacc[lid] * r0 + wacc[128u + lid] * r1 + wacc[256u + lid] * r2 + wacc[384u + lid] * r3
        yo[bb * (nheads * hs) + h * hs + lid] = acc / l
    }
}

// Batched fused rope + KV-row store: grid (pairs x B) — row b's q pairs rope in place, k pairs
// rope into its mirror row (±65504 clamp) at depth cnt-1, v pairs copy alongside.
class MetalRopeStoreBF16 {
    @ssbo @binding = 0 qk : array<float>    // [B x qstride] fused QKV rows — q ropes in place
    @ssbo @binding = 1 kd : array<float16>  // the whole mirror arena, f16 view (k dest)
    @ssbo @binding = 2 vd : array<float16>
    @ssbo @binding = 3 tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 4 tsin : array<float>
    @ssbo @binding = 5 rt : array<uint4>
    @uniform @binding = 6 lay : uint
    @uniform @binding = 7 kv_dim : uint
    @uniform @binding = 8 qd : uint
    @uniform @binding = 9 head_size : uint
    @uniform @binding = 10 npairs_q : uint  // qd/2 — the q half of the pair grid
    @uniform @binding = 11 npairs : uint    // + kvd/2 — grid guard
    @uniform @binding = 12 qstride : uint

    [metal_kernel(name="metal_rope_store_b16_msl")]
    def metal_rope_store_b16 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let bb = gl_WorkGroupID.y
        let rrow = rt[bb]
        let qbase = bb * qstride
        let half = head_size / 2u
        let isK = gid >= npairs_q
        let lgid = isK ? gid - npairs_q : gid
        let j = lgid % half
        let i = (lgid / half) * head_size + 2u * j
        let fcr = tcos[bb * half + j]
        let fci = tsin[bb * half + j]
        if (isK) {
            let kdst = rrow.x + (lay * rrow.z + (rrow.w - 1u)) * kv_dim
            let vdst = rrow.y + (lay * rrow.z + (rrow.w - 1u)) * kv_dim
            let v0 = qk[qbase + qd + i]
            let v1 = qk[qbase + qd + i + 1u]
            let r0 = v0 * fcr - v1 * fci
            let r1 = v0 * fci + v1 * fcr
            kd[kdst + i] = float16(min(max(r0, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
            kd[kdst + i + 1u] = float16(min(max(r1, -65504.0), 65504.0))   // nolint:STYLE019
            vd[vdst + i] = float16(min(max(qk[qbase + qd + kv_dim + i], -65504.0), 65504.0))   // nolint:STYLE019
            vd[vdst + i + 1u] = float16(min(max(qk[qbase + qd + kv_dim + i + 1u], -65504.0), 65504.0))   // nolint:STYLE019
        } else {
            let v0 = qk[qbase + i]
            let v1 = qk[qbase + i + 1u]
            qk[qbase + i] = v0 * fcr - v1 * fci
            qk[qbase + i + 1u] = v0 * fci + v1 * fcr
        }
    }
}

// f32-mirror twin — straight stores, no clamp.
class MetalRopeStoreBF32 {
    @ssbo @binding = 0 qk : array<float>
    @ssbo @binding = 1 kd : array<float>
    @ssbo @binding = 2 vd : array<float>
    @ssbo @binding = 3 tcos : array<float>
    @ssbo @binding = 4 tsin : array<float>
    @ssbo @binding = 5 rt : array<uint4>
    @uniform @binding = 6 lay : uint
    @uniform @binding = 7 kv_dim : uint
    @uniform @binding = 8 qd : uint
    @uniform @binding = 9 head_size : uint
    @uniform @binding = 10 npairs_q : uint
    @uniform @binding = 11 npairs : uint
    @uniform @binding = 12 qstride : uint

    [metal_kernel(name="metal_rope_store_b32_msl")]
    def metal_rope_store_b32 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let bb = gl_WorkGroupID.y
        let rrow = rt[bb]
        let qbase = bb * qstride
        let half = head_size / 2u
        let isK = gid >= npairs_q
        let lgid = isK ? gid - npairs_q : gid
        let j = lgid % half
        let i = (lgid / half) * head_size + 2u * j
        let fcr = tcos[bb * half + j]
        let fci = tsin[bb * half + j]
        if (isK) {
            let kdst = rrow.x + (lay * rrow.z + (rrow.w - 1u)) * kv_dim
            let vdst = rrow.y + (lay * rrow.z + (rrow.w - 1u)) * kv_dim
            let v0 = qk[qbase + qd + i]
            let v1 = qk[qbase + qd + i + 1u]
            kd[kdst + i] = v0 * fcr - v1 * fci
            kd[kdst + i + 1u] = v0 * fci + v1 * fcr
            vd[vdst + i] = qk[qbase + qd + kv_dim + i]
            vd[vdst + i + 1u] = qk[qbase + qd + kv_dim + i + 1u]
        } else {
            let v0 = qk[qbase + i]
            let v1 = qk[qbase + i + 1u]
            qk[qbase + i] = v0 * fcr - v1 * fci
            qk[qbase + i + 1u] = v0 * fci + v1 * fcr
        }
    }
}

// q8_0-mirror partD twin (hs == 128 only, DF16 shape): one sg per (row, head, 32-position
// slice), online slice merge, byte-offset row table. K/V loads are 4 scalar int8s + block scale
// per position (34B blocks break half4 alignment); scale folds in before the simd_sum.
class MetalSqAttnDQ8 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 kqb : array<int8>
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<int8>
    @ssbo @binding = 4 q : array<float4>
    @ssbo @binding = 5 part : array<float>
    @ssbo @binding = 6 rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 nsgs : uint
    @workgroup wacc : float[512]

    [metal_kernel(name="metal_sq_attn_d_q8_msl")]
    def metal_sq_attn_d_q8 {
        let h = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.z
        let gsg = gl_WorkGroupID.y * 4u + gl_SubgroupID
        let rrow = rt[bb]
        let cnt = rrow.w
        if (gsg >= nsgs || gsg * 32u >= cnt) {
            return
        }
        let sl = gl_SubgroupInvocationID
        let rowb = nblk * 34u
        let kb0b = rrow.x + lay * rrow.z * rowb
        let vb0b = rrow.y + lay * rrow.z * rowb
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * hs
        let q4 = float4(q[qoff / 4u + sl])
        let blkoff = khblk + sl / 8u            // this lane's block within the row
        let inb = (sl % 8u) * 4u                // ... and its 4 quants' offset inside it
        let wo = gl_SubgroupID * 128u + sl * 4u
        wacc[wo] = 0.0
        wacc[wo + 1u] = 0.0
        wacc[wo + 2u] = 0.0
        wacc[wo + 3u] = 0.0
        var mm = -3.0e38
        var tot = 0.0
        var ss = gsg
        while (ss * 32u < cnt) {
            let c0 = ss * 32u
            let nrows = min(c0 + 32u, cnt) - c0
            var keep = -3.0e38
            var bacc = float4(0.0)
            if (nrows == 32u) {
                for [unroll_full] (i in range(32)) {
                    let blk = (c0 + uint(i)) * nblk + blkoff
                    let kb = kb0b + blk * 34u + 2u + inb
                    let dq = float4(float(kqb[kb]), float(kqb[kb + 1u]), float(kqb[kb + 2u]), float(kqb[kb + 3u]))
                    let p = q4 * dq
                    let sc = simd_sum((p.x + p.y + p.z + p.w) * float(ksh[kb0h + blk * 17u]))
                    keep = uint(i) == sl ? sc : keep
                }
            } else {
                var j = 0u
                while (j < nrows) {
                    let blk = (c0 + j) * nblk + blkoff
                    let kb = kb0b + blk * 34u + 2u + inb
                    let dq = float4(float(kqb[kb]), float(kqb[kb + 1u]), float(kqb[kb + 2u]), float(kqb[kb + 3u]))
                    let p = q4 * dq
                    let sc = simd_sum((p.x + p.y + p.z + p.w) * float(ksh[kb0h + blk * 17u]))
                    keep = j == sl ? sc : keep
                    j++
                }
            }
            keep *= scale   // invalid tail lanes stay hugely negative -> weight 0
            var bm = keep
            bm = max(bm, simd_shuffle_xor(bm, 16u))
            bm = max(bm, simd_shuffle_xor(bm, 8u))
            bm = max(bm, simd_shuffle_xor(bm, 4u))
            bm = max(bm, simd_shuffle_xor(bm, 2u))
            bm = max(bm, simd_shuffle_xor(bm, 1u))
            let w = exp(keep - bm)
            let btot = simd_sum(w)
            if (nrows == 32u) {
                for [unroll_full] (i in range(32)) {
                    let blk = (c0 + uint(i)) * nblk + blkoff
                    let vb = vb0b + blk * 34u + 2u + inb
                    let dq = float4(float(vqb[vb]), float(vqb[vb + 1u]), float(vqb[vb + 2u]), float(vqb[vb + 3u]))
                    bacc += (simd_shuffle(w, uint(i)) * float(vsh[vb0h + blk * 17u])) * dq
                }
            } else {
                var j = 0u
                while (j < nrows) {
                    let blk = (c0 + j) * nblk + blkoff
                    let vb = vb0b + blk * 34u + 2u + inb
                    let dq = float4(float(vqb[vb]), float(vqb[vb + 1u]), float(vqb[vb + 2u]), float(vqb[vb + 3u]))
                    bacc += (simd_shuffle(w, j) * float(vsh[vb0h + blk * 17u])) * dq
                    j++
                }
            }
            // online merge into the running slice-group state (first pass: r1 = exp(-inf) = 0)
            let nm = max(mm, bm)
            let r1 = exp(mm - nm)
            let r2 = exp(bm - nm)
            wacc[wo] = wacc[wo] * r1 + bacc.x * r2
            wacc[wo + 1u] = wacc[wo + 1u] * r1 + bacc.y * r2
            wacc[wo + 2u] = wacc[wo + 2u] * r1 + bacc.z * r2
            wacc[wo + 3u] = wacc[wo + 3u] * r1 + bacc.w * r2
            tot = tot * r1 + btot * r2
            mm = nm
            ss += nsgs
        }
        let pbase = ((bb * nheads + h) * nsgs + gsg) * (hs + 4u)
        let po = pbase + sl * 4u
        part[po] = wacc[wo]
        part[po + 1u] = wacc[wo + 1u]
        part[po + 2u] = wacc[wo + 2u]
        part[po + 3u] = wacc[wo + 3u]
        if (sl == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// q8_0-mirror batched rope + KV-row store: the RopeStoreQ8 shape with the byte-offset row table
// (grid y = batch row; the new KV row lands at cnt-1). Same quantize_q8kv_row semantics.
class MetalRopeStoreBQ8 {
    @ssbo @binding = 0 qk : array<float>    // [B x qstride] fused QKV rows — q ropes in place
    @ssbo @binding = 1 ksh : array<float16> // the whole mirror arena, half-scale view (k dest)
    @ssbo @binding = 2 kqb : array<int8>    // the SAME arena, byte view
    @ssbo @binding = 3 vsh : array<float16>
    @ssbo @binding = 4 vqb : array<int8>
    @ssbo @binding = 5 tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 6 tsin : array<float>
    @ssbo @binding = 7 rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 8 lay : uint
    @uniform @binding = 9 nblk : uint       // kv_dim/32 — K blocks (V adds the same again)
    @uniform @binding = 10 qd : uint
    @uniform @binding = 11 head_size : uint
    @uniform @binding = 12 npairs_q : uint  // qd/2 — the q-rope half of the grid
    @uniform @binding = 13 qstride : uint

    [metal_kernel(name="metal_rope_store_b_q8_msl")]
    def metal_rope_store_b_q8 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs_q + 2u * nblk) {
            return
        }
        let bb = gl_WorkGroupID.y
        let rrow = rt[bb]
        let qbase = bb * qstride
        let half = head_size / 2u
        if (gid < npairs_q) {
            let j = gid % half
            let i = (gid / half) * head_size + 2u * j
            let fcr = tcos[bb * half + j]
            let fci = tsin[bb * half + j]
            let v0 = qk[qbase + i]
            let v1 = qk[qbase + i + 1u]
            qk[qbase + i] = v0 * fcr - v1 * fci
            qk[qbase + i + 1u] = v0 * fci + v1 * fcr
            return
        }
        let rowb = nblk * 34u
        let bi = gid - npairs_q
        let isV = bi >= nblk
        let blk = isV ? bi - nblk : bi
        let base = blk * 32u                // element base within the K/V row
        var vals : float[32]
        if (isV) {
            var e = 0u
            while (e < 32u) {
                vals[e] = qk[qbase + qd + nblk * 32u + base + e]   // the raw v third of the QKV row
                e++
            }
        } else {
            var p = 0u
            while (p < 16u) {
                let ge = base + 2u * p
                let j = (ge % head_size) / 2u
                let fcr = tcos[bb * half + j]
                let fci = tsin[bb * half + j]
                let v0 = qk[qbase + qd + ge]
                let v1 = qk[qbase + qd + ge + 1u]
                vals[2u * p] = v0 * fcr - v1 * fci
                vals[2u * p + 1u] = v0 * fci + v1 * fcr
                p++
            }
        }
        var amax = 0.0
        var k = 0u
        while (k < 32u) {
            amax = max(amax, abs(vals[k]))
            k++
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (isV) {
            let vb0b = rrow.y + (lay * rrow.z + (rrow.w - 1u)) * rowb
            vsh[vb0b / 2u + blk * 17u] = float16(d)
            k = 0u
            while (k < 32u) {
                vqb[vb0b + blk * 34u + 2u + k] = int8(round(vals[k] * id))
                k++
            }
        } else {
            let kb0b = rrow.x + (lay * rrow.z + (rrow.w - 1u)) * rowb
            ksh[kb0b / 2u + blk * 17u] = float16(d)
            k = 0u
            while (k < 32u) {
                kqb[kb0b + blk * 34u + 2u + k] = int8(round(vals[k] * id))
                k++
            }
        }
    }
}

// tq4-mirror partD twin (hs == 128, the DQ8 shape with nibble reads: lane's 4 elements = 2
// nibble bytes of block sl/8).
class MetalSqAttnDTq4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 kqb : array<uint8>
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<uint8>
    @ssbo @binding = 4 q : array<float4>
    @ssbo @binding = 5 part : array<float>
    @ssbo @binding = 6 rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 nsgs : uint
    @workgroup wacc : float[512]

    [metal_kernel(name="metal_sq_attn_d_tq4_msl")]
    def metal_sq_attn_d_tq4 {
        let h = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.z
        let gsg = gl_WorkGroupID.y * 4u + gl_SubgroupID
        let rrow = rt[bb]
        let cnt = rrow.w
        if (gsg >= nsgs || gsg * 32u >= cnt) {
            return
        }
        let sl = gl_SubgroupInvocationID
        let rowb = nblk * 18u
        let kb0b = rrow.x + lay * rrow.z * rowb
        let vb0b = rrow.y + lay * rrow.z * rowb
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * hs
        let q4 = float4(q[qoff / 4u + sl])
        let blkoff = khblk + sl / 8u
        let inb = (sl % 8u) * 2u            // this lane's byte offset inside its block's nibbles
        let wo = gl_SubgroupID * 128u + sl * 4u
        wacc[wo] = 0.0
        wacc[wo + 1u] = 0.0
        wacc[wo + 2u] = 0.0
        wacc[wo + 3u] = 0.0
        var mm = -3.0e38
        var tot = 0.0
        var ss = gsg
        while (ss * 32u < cnt) {
            let c0 = ss * 32u
            let nrows = min(c0 + 32u, cnt) - c0
            var keep = -3.0e38
            var bacc = float4(0.0)
            if (nrows == 32u) {
                for [unroll_full] (i in range(32)) {
                    let blk = (c0 + uint(i)) * nblk + blkoff
                    let kb = kb0b + blk * 18u + 2u + inb
                    let by0 = uint(kqb[kb])
                    let by1 = uint(kqb[kb + 1u])
                    let dq = float4(float(int(by0 & 15u) - 8), float(int(by0 >> 4u) - 8),
                        float(int(by1 & 15u) - 8), float(int(by1 >> 4u) - 8))
                    let p = q4 * dq
                    let sc = simd_sum((p.x + p.y + p.z + p.w) * float(ksh[kb0h + blk * 9u]))
                    keep = uint(i) == sl ? sc : keep
                }
            } else {
                var j = 0u
                while (j < nrows) {
                    let blk = (c0 + j) * nblk + blkoff
                    let kb = kb0b + blk * 18u + 2u + inb
                    let by0 = uint(kqb[kb])
                    let by1 = uint(kqb[kb + 1u])
                    let dq = float4(float(int(by0 & 15u) - 8), float(int(by0 >> 4u) - 8),
                        float(int(by1 & 15u) - 8), float(int(by1 >> 4u) - 8))
                    let p = q4 * dq
                    let sc = simd_sum((p.x + p.y + p.z + p.w) * float(ksh[kb0h + blk * 9u]))
                    keep = j == sl ? sc : keep
                    j++
                }
            }
            keep *= scale   // invalid tail lanes stay hugely negative -> weight 0
            var bm = keep
            bm = max(bm, simd_shuffle_xor(bm, 16u))
            bm = max(bm, simd_shuffle_xor(bm, 8u))
            bm = max(bm, simd_shuffle_xor(bm, 4u))
            bm = max(bm, simd_shuffle_xor(bm, 2u))
            bm = max(bm, simd_shuffle_xor(bm, 1u))
            let w = exp(keep - bm)
            let btot = simd_sum(w)
            if (nrows == 32u) {
                for [unroll_full] (i in range(32)) {
                    let blk = (c0 + uint(i)) * nblk + blkoff
                    let vb = vb0b + blk * 18u + 2u + inb
                    let by0 = uint(vqb[vb])
                    let by1 = uint(vqb[vb + 1u])
                    let dq = float4(float(int(by0 & 15u) - 8), float(int(by0 >> 4u) - 8),
                        float(int(by1 & 15u) - 8), float(int(by1 >> 4u) - 8))
                    bacc += (simd_shuffle(w, uint(i)) * float(vsh[vb0h + blk * 9u])) * dq
                }
            } else {
                var j = 0u
                while (j < nrows) {
                    let blk = (c0 + j) * nblk + blkoff
                    let vb = vb0b + blk * 18u + 2u + inb
                    let by0 = uint(vqb[vb])
                    let by1 = uint(vqb[vb + 1u])
                    let dq = float4(float(int(by0 & 15u) - 8), float(int(by0 >> 4u) - 8),
                        float(int(by1 & 15u) - 8), float(int(by1 >> 4u) - 8))
                    bacc += (simd_shuffle(w, j) * float(vsh[vb0h + blk * 9u])) * dq
                    j++
                }
            }
            // online merge into the running slice-group state (first pass: r1 = exp(-inf) = 0)
            let nm = max(mm, bm)
            let r1 = exp(mm - nm)
            let r2 = exp(bm - nm)
            wacc[wo] = wacc[wo] * r1 + bacc.x * r2
            wacc[wo + 1u] = wacc[wo + 1u] * r1 + bacc.y * r2
            wacc[wo + 2u] = wacc[wo + 2u] * r1 + bacc.z * r2
            wacc[wo + 3u] = wacc[wo + 3u] * r1 + bacc.w * r2
            tot = tot * r1 + btot * r2
            mm = nm
            ss += nsgs
        }
        let pbase = ((bb * nheads + h) * nsgs + gsg) * (hs + 4u)
        let po = pbase + sl * 4u
        part[po] = wacc[wo]
        part[po + 1u] = wacc[wo + 1u]
        part[po + 2u] = wacc[wo + 2u]
        part[po + 3u] = wacc[wo + 3u]
        if (sl == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// tq4-mirror batched rope + KV-row store: the RopeStoreTq4 shape (threadgroup per head) with the
// byte-offset row table — grid (heads, batch rows); the new KV row lands at cnt-1.
class MetalRopeStoreBTq4 {
    @ssbo @binding = 0 qk : array<float>    // [B x qstride] fused QKV rows — q ropes + rotates in place
    @ssbo @binding = 1 ksh : array<float16> // the whole mirror arena, half-scale view (k dest)
    @ssbo @binding = 2 kqb : array<uint8>   // the SAME arena, byte view
    @ssbo @binding = 3 vsh : array<float16>
    @ssbo @binding = 4 vqb : array<uint8>
    @ssbo @binding = 5 tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 6 tsin : array<float>
    @ssbo @binding = 7 signs : array<float>
    @ssbo @binding = 8 rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 9 lay : uint
    @uniform @binding = 10 qd : uint
    @uniform @binding = 11 head_size : uint
    @uniform @binding = 12 nheads : uint
    @uniform @binding = 13 kv_mul : uint
    @uniform @binding = 14 qstride : uint
    @workgroup buf : float[128]

    [metal_kernel(name="metal_rope_store_b_tq4_msl")]
    def metal_rope_store_b_tq4 {
        let tgid = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let hs = head_size
        let half = hs / 2u
        let nkvh = nheads / kv_mul
        let rrow = rt[bb]
        let qbase = bb * qstride
        let kv_dim = nkvh * hs
        let isQ = tgid < nheads
        let isK = !isQ && tgid < nheads + nkvh
        let hh = isQ ? tgid : (isK ? tgid - nheads : tgid - nheads - nkvh)
        // q/k live in the fused QKV row; v is its third region
        let base = qbase + (isQ ? tgid * hs : (isK ? qd + hh * hs : qd + kv_dim + hh * hs))
        var v = 0.0
        if (isQ || isK) {
            let j = lid / 2u
            let fcr = tcos[bb * half + j]
            let fci = tsin[bb * half + j]
            let mine = qk[base + lid]
            let other = qk[base + (lid ^ 1u)]
            v = (lid & 1u) == 0u ? mine * fcr - other * fci : other * fci + mine * fcr
        } else {
            v = qk[base + lid]
        }
        buf[lid] = v * signs[lid]
        barrier()
        var h = 1u
        while (h < hs) {
            let a = buf[lid]
            let b = buf[lid ^ h]
            barrier()
            buf[lid] = (lid & h) == 0u ? a + b : b - a
            barrier()
            h *= 2u
        }
        v = buf[lid] * (1.0 / sqrt(float(hs)))
        if (isQ) {
            qk[base + lid] = v
            return
        }
        let sl = gl_SubgroupInvocationID
        var ma = abs(v)
        var mv = v
        var ml = sl
        var off = 16u
        while (off > 0u) {
            let oa = simd_shuffle_xor(ma, off)
            let ov = simd_shuffle_xor(mv, off)
            let ol = simd_shuffle_xor(ml, off)
            let take = oa > ma || (oa == ma && ol < ml)
            ma = take ? oa : ma
            mv = take ? ov : mv
            ml = take ? ol : ml
            off /= 2u
        }
        let d = mv / -8.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        let qv = min(15, int(v * id + 8.5))
        let qother = simd_shuffle_xor(qv, 1u)
        let rowb = (kv_dim / 32u) * 18u
        let blk = hh * (hs / 32u) + lid / 32u   // block within the K/V row
        if (isK) {
            let kb0b = rrow.x + (lay * rrow.z + (rrow.w - 1u)) * rowb
            if (sl == 0u) {
                ksh[kb0b / 2u + blk * 9u] = float16(d)
            }
            if ((lid & 1u) == 0u) {
                kqb[kb0b + blk * 18u + 2u + (lid % 32u) / 2u] = uint8(uint(qv) | (uint(qother) << 4u))
            }
        } else {
            let vb0b = rrow.y + (lay * rrow.z + (rrow.w - 1u)) * rowb
            if (sl == 0u) {
                vsh[vb0b / 2u + blk * 9u] = float16(d)
            }
            if ((lid & 1u) == 0u) {
                vqb[vb0b + blk * 18u + 2u + (lid % 32u) / 2u] = uint8(uint(qv) | (uint(qother) << 4u))
            }
        }
    }
}

// Batched fused residual add + rmsnorm (MetalAddRms with a row grid): one threadgroup per row,
// x += a in place, y = rms(x)·w. Same tgmem row slab — dim caps at ADD_RMS_MAX_DIM (the batch
// driver pairs enc_ew2-add + enc_rms_b past the slab, mirroring enc_add_rms_site).
class MetalAddRmsB {
    @ssbo @binding = 0 x : array<float>     // [B x dim] residual stream, add target
    @ssbo @binding = 1 a : array<float>     // [B x dim] addend
    @ssbo @binding = 2 w : array<float>     // [dim] norm weight
    @ssbo @binding = 3 y : array<float>     // [B x dim] normed out (y may alias a)
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 eps : float
    @workgroup xrow : float[4096]           // == ADD_RMS_MAX_DIM
    @workgroup partial : float[32]
    @workgroup inv_sh : float

    [metal_kernel(name="metal_add_rms_b_msl")]
    def metal_add_rms_b {
        let row = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        var ss = 0.0
        var k = lid
        while (k < dim) {
            let vv = x[row * dim + k] + a[row * dim + k]
            x[row * dim + k] = vv
            xrow[k] = vv
            ss += vv * vv
            k += gl_WorkGroupSize.x
        }
        ss = simd_sum(ss)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ss
        }
        barrier()
        if (lid == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += partial[sgi]
                sgi++
            }
            inv_sh = rsqrt(tot / float(dim) + eps)
        }
        barrier()
        let inv = inv_sh
        k = lid
        while (k < dim) {
            y[row * dim + k] = w[k] * (xrow[k] * inv)
            k += gl_WorkGroupSize.x
        }
    }
}


var g_pso_gemv : MetalComputePipeline?
var g_pso_gemv16 : MetalComputePipeline?   // f16-scale twin (wscale_f16 models)
var g_gemv_rows = 4l                       // rows (simdgroups) per GEMV threadgroup
var g_pso_rms : MetalComputePipeline?
var g_pso_add : MetalComputePipeline?
var g_pso_addrms : MetalComputePipeline?
var g_pso_swiglu : MetalComputePipeline?
var g_pso_attn16 : MetalComputePipeline?
var g_pso_attn32 : MetalComputePipeline?
var g_pso_attn_q8 : MetalComputePipeline?   // q8_0-mirror twin (dual-view block reads)
var g_pso_attn_tq4 : MetalComputePipeline?  // tq4-mirror twin (rotated-basis nibble reads)
var g_pso_attnp16 : MetalComputePipeline?
var g_pso_attnp32 : MetalComputePipeline?
var g_pso_attnp_q8 : MetalComputePipeline?
var g_pso_attnp_tq4 : MetalComputePipeline?
var g_pso_attncomb : MetalComputePipeline?
var g_pso_rpst16 : MetalComputePipeline?
var g_pso_rpst32 : MetalComputePipeline?
var g_pso_rpst_q8 : MetalComputePipeline?   // per-32-block quantizing store
var g_pso_rpst_tq4 : MetalComputePipeline?  // threadgroup-per-head FWHT + quantizing store
var g_pso_unsign : MetalComputePipeline?    // the tq4 attention-output un-rotate

var g_pso_argmax : MetalComputePipeline?
var g_pso_copy_row : MetalComputePipeline?   // zero-copy logits: staging row -> wrapped session storage
var g_pso_emb : MetalComputePipeline?
var g_pso_emb16 : MetalComputePipeline?
var g_pso_qkvrs16 : MetalComputePipeline?   // fused QKV+rope+store (s16 path)
var g_pso_qkvrs32 : MetalComputePipeline?
var g_pso_w13sw : MetalComputePipeline?
var g_pso_kq_k4 : MetalComputePipeline?
var g_pso_kq_k5 : MetalComputePipeline?
var g_pso_kq_k6 : MetalComputePipeline?
var g_pso_kq_mvb2_k4 : MetalComputePipeline?   // batch B=2..8: the kq ext small-batch mv twins
var g_pso_kq_mvb4_k4 : MetalComputePipeline?
var g_pso_kq_mvb2_k5 : MetalComputePipeline?
var g_pso_kq_mvb4_k5 : MetalComputePipeline?
var g_pso_kq_mvb2_k6 : MetalComputePipeline?
var g_pso_kq_mvb4_k6 : MetalComputePipeline?
var g_pso_kq_mm_b4 : MetalComputePipeline?   // batch B>=9: the prefill-owned kq mul_mm twins
var g_pso_kq_mm_b5 : MetalComputePipeline?
var g_pso_kq_mm_b6 : MetalComputePipeline?     // fused W1|W3+swiglu (s16 path)
// the batched-step set (P4): fixed-B GEMVs + the M-pad-32 GEMM twin (f32 X) + row-table kernels
var g_pso_gemv_b2 : MetalComputePipeline?
var g_pso_gemv_b4 : MetalComputePipeline?
var g_pso_mv_b2 : MetalComputePipeline?
var g_pso_mv_b4 : MetalComputePipeline?
var g_pso_w13sw_b2 : MetalComputePipeline?
var g_pso_w13sw_b4 : MetalComputePipeline?
var g_pso_gemm_b : MetalComputePipeline?
var g_pso_gemm_b_sk : MetalComputePipeline? // split-K twin (per-site policy)
var g_pso_skred : MetalComputePipeline?     // the split-K plane reduce
var g_pso_gemm64_b : MetalComputePipeline?  // 64-wide N tile (classifier site)
var g_pso_mm : MetalComputePipeline?        // the prefill module's MetalQ8MulMm (34B blob)
var g_pso_attnb16 : MetalComputePipeline?
var g_pso_attnb32 : MetalComputePipeline?
var g_pso_attnb_q8 : MetalComputePipeline?
var g_pso_attnb_tq4 : MetalComputePipeline?
var g_pso_attnpb16 : MetalComputePipeline?
var g_pso_attnpb32 : MetalComputePipeline?
var g_pso_attnpb_q8 : MetalComputePipeline?
var g_pso_attnpb_tq4 : MetalComputePipeline?
var g_pso_attncombb : MetalComputePipeline?
var g_pso_attnd16 : MetalComputePipeline?   // fused single-pass batch attention (hs 128)
var g_pso_attnd32 : MetalComputePipeline?
var g_pso_attnd_q8 : MetalComputePipeline?
var g_pso_attnd_tq4 : MetalComputePipeline?
var g_pso_attncombd : MetalComputePipeline?
var g_pso_rpstb16 : MetalComputePipeline?
var g_pso_rpstb32 : MetalComputePipeline?
var g_pso_rpstb_q8 : MetalComputePipeline?
var g_pso_rpstb_tq4 : MetalComputePipeline?
var g_pso_addrmsb : MetalComputePipeline?

def release_pso(var pso : MetalComputePipeline?&) {
    if (pso != null) {
        metal_release(pso)
        pso = null
    }
}


def compile_pso(source, entry : string; fastmath : bool; var ok : bool&) : MetalComputePipeline? {
    var err : string
    var pso = pipeline_from_source(g_dev, source, entry, fastmath, err)
    if (pso == null) {
        to_log(LOG_ERROR, "dasLLAMA metal decode: `{entry}` compile failed — {err}\n")
        ok = false
    }
    return pso
}


def metal_decode_init : bool {
    if (g_pso_add != null) {
        return true
    }
    if (!metal_common_init()) {
        return false
    }
    var ok = true
    // the GEMV / rms / add / swiglu MSL sources are the prefill module's, verbatim (own PSOs)
    g_pso_gemv = compile_pso(metal_q8_gemv_msl, metal_q8_gemv_msl_entry, metal_q8_gemv_msl_fastmath, ok)
    g_pso_gemv16 = compile_pso(metal_q8_gemv_s16_msl, metal_q8_gemv_s16_msl_entry, metal_q8_gemv_s16_msl_fastmath, ok)
    g_gemv_rows = clamp(int64(env_int("DASLLAMA_METAL_GEMV_TG", 4)), 1l, 32l)
    g_pso_rms = compile_pso(metal_rmsnorm_msl, metal_rmsnorm_msl_entry, metal_rmsnorm_msl_fastmath, ok)
    g_pso_add = compile_pso(metal_add_msl, metal_add_msl_entry, metal_add_msl_fastmath, ok)
    g_pso_addrms = compile_pso(metal_add_rms_msl, metal_add_rms_msl_entry, metal_add_rms_msl_fastmath, ok)
    g_pso_swiglu = compile_pso(metal_swiglu_msl, metal_swiglu_msl_entry, metal_swiglu_msl_fastmath, ok)
    g_pso_attn16 = compile_pso(metal_sq_attn16_msl, metal_sq_attn16_msl_entry, metal_sq_attn16_msl_fastmath, ok)
    g_pso_attn32 = compile_pso(metal_sq_attn32_msl, metal_sq_attn32_msl_entry, metal_sq_attn32_msl_fastmath, ok)
    g_pso_attnp16 = compile_pso(metal_sq_attn_part16_msl, metal_sq_attn_part16_msl_entry, metal_sq_attn_part16_msl_fastmath, ok)
    g_pso_attnp32 = compile_pso(metal_sq_attn_part32_msl, metal_sq_attn_part32_msl_entry, metal_sq_attn_part32_msl_fastmath, ok)
    g_pso_attncomb = compile_pso(metal_sq_attn_comb_msl, metal_sq_attn_comb_msl_entry, metal_sq_attn_comb_msl_fastmath, ok)
    g_pso_attn_q8 = compile_pso(metal_sq_attn_q8_msl, metal_sq_attn_q8_msl_entry, metal_sq_attn_q8_msl_fastmath, ok)
    g_pso_attnp_q8 = compile_pso(metal_sq_attn_part_q8_msl, metal_sq_attn_part_q8_msl_entry, metal_sq_attn_part_q8_msl_fastmath, ok)
    g_pso_attn_tq4 = compile_pso(metal_sq_attn_tq4_msl, metal_sq_attn_tq4_msl_entry, metal_sq_attn_tq4_msl_fastmath, ok)
    g_pso_attnp_tq4 = compile_pso(metal_sq_attn_part_tq4_msl, metal_sq_attn_part_tq4_msl_entry, metal_sq_attn_part_tq4_msl_fastmath, ok)
    g_pso_rpst16 = compile_pso(metal_rope_store16_msl, metal_rope_store16_msl_entry, metal_rope_store16_msl_fastmath, ok)
    g_pso_rpst32 = compile_pso(metal_rope_store32_msl, metal_rope_store32_msl_entry, metal_rope_store32_msl_fastmath, ok)
    g_pso_rpst_q8 = compile_pso(metal_rope_store_q8_msl, metal_rope_store_q8_msl_entry, metal_rope_store_q8_msl_fastmath, ok)
    g_pso_rpst_tq4 = compile_pso(metal_rope_store_tq4_msl, metal_rope_store_tq4_msl_entry, metal_rope_store_tq4_msl_fastmath, ok)
    g_pso_unsign = compile_pso(metal_fwht_unsign_msl, metal_fwht_unsign_msl_entry, metal_fwht_unsign_msl_fastmath, ok)
    g_pso_argmax = compile_pso(metal_argmax_msl, metal_argmax_msl_entry, metal_argmax_msl_fastmath, ok)
    g_pso_copy_row = compile_pso(metal_copy_row_msl, metal_copy_row_msl_entry, metal_copy_row_msl_fastmath, ok)
    g_pso_emb = compile_pso(metal_embed_q8_msl, metal_embed_q8_msl_entry, metal_embed_q8_msl_fastmath, ok)
    g_pso_emb16 = compile_pso(metal_embed_q8_s16_msl, metal_embed_q8_s16_msl_entry, metal_embed_q8_s16_msl_fastmath, ok)
    g_pso_qkvrs16 = compile_pso(metal_q8_gemv_qkv_rs16_msl, metal_q8_gemv_qkv_rs16_msl_entry, metal_q8_gemv_qkv_rs16_msl_fastmath, ok)
    g_pso_qkvrs32 = compile_pso(metal_q8_gemv_qkv_rs32_msl, metal_q8_gemv_qkv_rs32_msl_entry, metal_q8_gemv_qkv_rs32_msl_fastmath, ok)
    g_pso_w13sw = compile_pso(metal_q8_gemv_w13sw16_msl, metal_q8_gemv_w13sw16_msl_entry, metal_q8_gemv_w13sw16_msl_fastmath, ok)
    g_pso_kq_k4 = compile_pso(metal_kq_gemv_k4_msl, metal_kq_gemv_k4_msl_entry, metal_kq_gemv_k4_msl_fastmath, ok)
    g_pso_kq_k5 = compile_pso(metal_kq_gemv_k5_msl, metal_kq_gemv_k5_msl_entry, metal_kq_gemv_k5_msl_fastmath, ok)
    g_pso_kq_k6 = compile_pso(metal_kq_gemv_k6_msl, metal_kq_gemv_k6_msl_entry, metal_kq_gemv_k6_msl_fastmath, ok)
    g_pso_kq_mvb2_k4 = compile_pso(metal_kq_mvb2_k4_msl, metal_kq_mvb2_k4_msl_entry, metal_kq_mvb2_k4_msl_fastmath, ok)
    g_pso_kq_mvb4_k4 = compile_pso(metal_kq_mvb4_k4_msl, metal_kq_mvb4_k4_msl_entry, metal_kq_mvb4_k4_msl_fastmath, ok)
    g_pso_kq_mvb2_k5 = compile_pso(metal_kq_mvb2_k5_msl, metal_kq_mvb2_k5_msl_entry, metal_kq_mvb2_k5_msl_fastmath, ok)
    g_pso_kq_mvb4_k5 = compile_pso(metal_kq_mvb4_k5_msl, metal_kq_mvb4_k5_msl_entry, metal_kq_mvb4_k5_msl_fastmath, ok)
    g_pso_kq_mvb2_k6 = compile_pso(metal_kq_mvb2_k6_msl, metal_kq_mvb2_k6_msl_entry, metal_kq_mvb2_k6_msl_fastmath, ok)
    g_pso_kq_mvb4_k6 = compile_pso(metal_kq_mvb4_k6_msl, metal_kq_mvb4_k6_msl_entry, metal_kq_mvb4_k6_msl_fastmath, ok)
    g_pso_kq_mm_b4 = compile_pso(metal_kq_mulmm_k4_msl, metal_kq_mulmm_k4_msl_entry, metal_kq_mulmm_k4_msl_fastmath, ok)
    g_pso_kq_mm_b5 = compile_pso(metal_kq_mulmm_k5_msl, metal_kq_mulmm_k5_msl_entry, metal_kq_mulmm_k5_msl_fastmath, ok)
    g_pso_kq_mm_b6 = compile_pso(metal_kq_mulmm_k6_msl, metal_kq_mulmm_k6_msl_entry, metal_kq_mulmm_k6_msl_fastmath, ok)
    // the batched-step set (P4)
    g_pso_gemv_b2 = compile_pso(metal_gemv_b2_msl, metal_gemv_b2_msl_entry, metal_gemv_b2_msl_fastmath, ok)
    g_pso_gemv_b4 = compile_pso(metal_gemv_b4_msl, metal_gemv_b4_msl_entry, metal_gemv_b4_msl_fastmath, ok)
    g_pso_mv_b2 = compile_pso(metal_q8_mv_b2_msl, metal_q8_mv_b2_msl_entry, metal_q8_mv_b2_msl_fastmath, ok)
    g_pso_mv_b4 = compile_pso(metal_q8_mv_b4_msl, metal_q8_mv_b4_msl_entry, metal_q8_mv_b4_msl_fastmath, ok)
    g_pso_w13sw_b2 = compile_pso(metal_gemv_w13sw_b2_msl, metal_gemv_w13sw_b2_msl_entry, metal_gemv_w13sw_b2_msl_fastmath, ok)
    g_pso_w13sw_b4 = compile_pso(metal_gemv_w13sw_b4_msl, metal_gemv_w13sw_b4_msl_entry, metal_gemv_w13sw_b4_msl_fastmath, ok)
    g_pso_gemm_b = compile_pso(metal_q8_gemm_b_msl, metal_q8_gemm_b_msl_entry, metal_q8_gemm_b_msl_fastmath, ok)
    g_pso_gemm_b_sk = compile_pso(metal_q8_gemm_b_sk_msl, metal_q8_gemm_b_sk_msl_entry, metal_q8_gemm_b_sk_msl_fastmath, ok)
    g_pso_skred = compile_pso(metal_sk_reduce_msl, metal_sk_reduce_msl_entry, metal_sk_reduce_msl_fastmath, ok)
    g_pso_gemm64_b = compile_pso(metal_q8_gemm64_b_msl, metal_q8_gemm64_b_msl_entry, metal_q8_gemm64_b_msl_fastmath, ok)
    g_pso_mm = compile_pso(metal_q8_mulmm_msl, metal_q8_mulmm_msl_entry, metal_q8_mulmm_msl_fastmath, ok)
    g_pso_attnb16 = compile_pso(metal_sq_attn_b16_msl, metal_sq_attn_b16_msl_entry, metal_sq_attn_b16_msl_fastmath, ok)
    g_pso_attnb32 = compile_pso(metal_sq_attn_b32_msl, metal_sq_attn_b32_msl_entry, metal_sq_attn_b32_msl_fastmath, ok)
    g_pso_attnb_q8 = compile_pso(metal_sq_attn_b_q8_msl, metal_sq_attn_b_q8_msl_entry, metal_sq_attn_b_q8_msl_fastmath, ok)
    g_pso_attnb_tq4 = compile_pso(metal_sq_attn_b_tq4_msl, metal_sq_attn_b_tq4_msl_entry, metal_sq_attn_b_tq4_msl_fastmath, ok)
    g_pso_attnpb16 = compile_pso(metal_sq_attn_part_b16_msl, metal_sq_attn_part_b16_msl_entry, metal_sq_attn_part_b16_msl_fastmath, ok)
    g_pso_attnpb32 = compile_pso(metal_sq_attn_part_b32_msl, metal_sq_attn_part_b32_msl_entry, metal_sq_attn_part_b32_msl_fastmath, ok)
    g_pso_attnpb_q8 = compile_pso(metal_sq_attn_part_b_q8_msl, metal_sq_attn_part_b_q8_msl_entry, metal_sq_attn_part_b_q8_msl_fastmath, ok)
    g_pso_attnpb_tq4 = compile_pso(metal_sq_attn_part_b_tq4_msl, metal_sq_attn_part_b_tq4_msl_entry, metal_sq_attn_part_b_tq4_msl_fastmath, ok)
    g_pso_attncombb = compile_pso(metal_sq_attn_comb_b_msl, metal_sq_attn_comb_b_msl_entry, metal_sq_attn_comb_b_msl_fastmath, ok)
    g_pso_attnd16 = compile_pso(metal_sq_attn_d16_msl, metal_sq_attn_d16_msl_entry, metal_sq_attn_d16_msl_fastmath, ok)
    g_pso_attnd32 = compile_pso(metal_sq_attn_d32_msl, metal_sq_attn_d32_msl_entry, metal_sq_attn_d32_msl_fastmath, ok)
    g_pso_attnd_q8 = compile_pso(metal_sq_attn_d_q8_msl, metal_sq_attn_d_q8_msl_entry, metal_sq_attn_d_q8_msl_fastmath, ok)
    g_pso_attnd_tq4 = compile_pso(metal_sq_attn_d_tq4_msl, metal_sq_attn_d_tq4_msl_entry, metal_sq_attn_d_tq4_msl_fastmath, ok)
    g_pso_attncombd = compile_pso(metal_sq_attn_comb_d_msl, metal_sq_attn_comb_d_msl_entry, metal_sq_attn_comb_d_msl_fastmath, ok)
    g_pso_rpstb16 = compile_pso(metal_rope_store_b16_msl, metal_rope_store_b16_msl_entry, metal_rope_store_b16_msl_fastmath, ok)
    g_pso_rpstb32 = compile_pso(metal_rope_store_b32_msl, metal_rope_store_b32_msl_entry, metal_rope_store_b32_msl_fastmath, ok)
    g_pso_rpstb_q8 = compile_pso(metal_rope_store_b_q8_msl, metal_rope_store_b_q8_msl_entry, metal_rope_store_b_q8_msl_fastmath, ok)
    g_pso_rpstb_tq4 = compile_pso(metal_rope_store_b_tq4_msl, metal_rope_store_b_tq4_msl_entry, metal_rope_store_b_tq4_msl_fastmath, ok)
    g_pso_addrmsb = compile_pso(metal_add_rms_b_msl, metal_add_rms_b_msl_entry, metal_add_rms_b_msl_fastmath, ok)
    if (!ok) {
        // no steps are in flight at init time — releasing PSOs + shared state suffices
        metal_kernels_release()
        metal_common_release()
        g_failed = true
        return false
    }
    to_log(LOG_INFO, "dasLLAMA metal decode: resident path live on {metal_device_name(g_dev)}\n")
    return true
}

// lazily upload one stable region (weight quants / scales / norm vectors), keyed by base address.
// UNTRACKED: weights are GPU-read-only, so they carry no hazards (the prefill driver measured
// tracked weight regions dominating the scheduler's dependency analysis)

// ===== encoders =====

def enc_gemv(enc : MetalComputeEncoder?; s16 : bool; bwq, bws, bx, by, bn, bd : MetalBuffer?; d : int64; yoff : uint64 = 0ul) {
    metal_set_pipeline(enc, s16 ? g_pso_gemv16 : g_pso_gemv)
    metal_set_buffer(enc, bwq, 0ul, 0)
    metal_set_buffer(enc, bws, 0ul, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, yoff, 3)
    metal_set_buffer(enc, bn, 0ul, 4)
    metal_set_buffer(enc, bd, 0ul, 5)
    // the kernel is width-agnostic (one row per simdgroup) — rows/tg via DASLLAMA_METAL_GEMV_TG
    metal_dispatch_threadgroups(enc, uint3(uint((d + g_gemv_rows - 1l) / g_gemv_rows), 1u, 1u), uint3(uint(g_gemv_rows * 32l), 1u, 1u))
}

// K-quant site GEMV — the per-format twin of enc_gemv over the kq plane pair
// (kq_upload_quants + kq_upload_scales). k6 binds the scale buffer twice: the trailing packed
// f16 d plane at byte nsb*16, the 16B sub-scale blocks at 0. Geometry: 64-thread tg, 4 rows.
def enc_kq_gemv(enc : MetalComputeEncoder?; fmt : KqFmt; bq, bs, bx, by, bn, bd : MetalBuffer?; rows, n : int64; yoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    if (fmt == KqFmt.k6) {
        metal_set_pipeline(enc, g_pso_kq_k6)
        metal_set_buffer(enc, bs, uint64(((rows * n) / 256l) * 16l), 0)
    } else {
        metal_set_pipeline(enc, fmt == KqFmt.k4 ? g_pso_kq_k4 : g_pso_kq_k5)
        metal_set_buffer(enc, bs, 0ul, 0)
    }
    metal_set_buffer(enc, bs, 0ul, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, bx, xoff, 3)
    metal_set_buffer(enc, by, yoff, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_set_buffer(enc, bd, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint((rows + 3l) / 4l), 1u, 1u), uint3(64u, 1u, 1u))
}

// K-quant small-batch mv (B = 2..8): the kq ext twins — R=4 column groups via grid.y (gcols),
// R=2 via the B2 PSO at exactly two live columns. y binds at the site's column byte offset;
// bys = the y ROW-stride uniform; bnr = live columns (stores gate).
def enc_kq_mvb(enc : MetalComputeEncoder?; fmt : KqFmt; bq, bs, bx, by : MetalBuffer?; yoff : uint64;
               bn, bd, bys, bnr : MetalBuffer?; rows, n, nlive : int64) {
    let two = nlive == 2l
    if (fmt == KqFmt.k6) {
        metal_set_pipeline(enc, two ? g_pso_kq_mvb2_k6 : g_pso_kq_mvb4_k6)
        metal_set_buffer(enc, bs, uint64(((rows * n) / 256l) * 16l), 0)
    } elif (fmt == KqFmt.k5) {
        metal_set_pipeline(enc, two ? g_pso_kq_mvb2_k5 : g_pso_kq_mvb4_k5)
        metal_set_buffer(enc, bs, 0ul, 0)
    } else {
        metal_set_pipeline(enc, two ? g_pso_kq_mvb2_k4 : g_pso_kq_mvb4_k4)
        metal_set_buffer(enc, bs, 0ul, 0)
    }
    metal_set_buffer(enc, bs, 0ul, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, by, yoff, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_set_buffer(enc, bd, 0ul, 6)
    metal_set_buffer(enc, bys, 0ul, 7)
    metal_set_buffer(enc, bnr, 0ul, 8)
    let gcols = two ? 1l : (nlive + 3l) / 4l
    metal_dispatch_threadgroups(enc, uint3(uint((rows + 7l) / 8l), uint(gcols), 1u), uint3(64u, 1u, 1u))
}

// the batched kq GEMM (B >= 9): the prefill-owned kq mul_mm twins over the padded x panel —
// enc_gemm_mm_b's shape (grid mp/32 x rows/64, y bound at the site's column byte offset)
def enc_kq_gemm_mm_b(enc : MetalComputeEncoder?; fmt : KqFmt; bq, bs, bx, by : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows, n : int64) {
    if (fmt == KqFmt.k6) {
        metal_set_pipeline(enc, g_pso_kq_mm_b6)
        metal_set_threadgroup_memory_length(enc, metal_kq_mulmm_k6_msl_tgmem, 0)
        metal_set_buffer(enc, bs, uint64(((rows * n) / 256l) * 16l), 0)
    } else {
        metal_set_pipeline(enc, fmt == KqFmt.k4 ? g_pso_kq_mm_b4 : g_pso_kq_mm_b5)
        metal_set_threadgroup_memory_length(enc, fmt == KqFmt.k4 ? metal_kq_mulmm_k4_msl_tgmem : metal_kq_mulmm_k5_msl_tgmem, 0)
        metal_set_buffer(enc, bs, 0ul, 0)
    }
    metal_set_buffer(enc, bs, 0ul, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, by, yoff, 4)
    metal_set_buffer(enc, bk, 0ul, 5)
    metal_set_buffer(enc, bn, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(rows / 64l), 1u), uint3(128u, 1u, 1u))
}

def enc_rms(enc : MetalComputeEncoder?; bx, bw, by, bdim, beps : MetalBuffer?) {
    metal_set_pipeline(enc, g_pso_rms)
    metal_set_threadgroup_memory_length(enc, metal_rmsnorm_msl_tgmem, 0)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, bw, 0ul, 1)
    metal_set_buffer(enc, by, 0ul, 2)
    metal_set_buffer(enc, bdim, 0ul, 3)
    metal_set_buffer(enc, beps, 0ul, 4)
    // one row, latency-bound — the widest tg shortens the strided-load rounds (1024 max on M1)
    metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(1024u, 1u, 1u))
}

// fused rope + KV-row store: the v source binds the SAME QKV buffer at its v byte offset
def enc_rope_store(enc : MetalComputeEncoder?; f16 : bool; bqkv : MetalBuffer?; vsoff : uint64;
                   kmb, vmb : MetalBuffer?; koff, voff : uint64;
                   bcos, bsin, bqd, bhs, bnpq, bnpall : MetalBuffer?; npairs : int64) {
    metal_set_pipeline(enc, f16 ? g_pso_rpst16 : g_pso_rpst32)
    metal_set_buffer(enc, bqkv, 0ul, 0)
    metal_set_buffer(enc, bqkv, vsoff, 1)
    metal_set_buffer(enc, kmb, koff, 2)
    metal_set_buffer(enc, vmb, voff, 3)
    metal_set_buffer(enc, bcos, 0ul, 4)
    metal_set_buffer(enc, bsin, 0ul, 5)
    metal_set_buffer(enc, bqd, 0ul, 6)
    metal_set_buffer(enc, bhs, 0ul, 7)
    metal_set_buffer(enc, bnpq, 0ul, 8)
    metal_set_buffer(enc, bnpall, 0ul, 9)
    metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// The tq4 rotation signs, uploaded once at max head size (smaller heads read a prefix — the
// deterministic-seed contract with the CPU codec).

def enc_rope_store_tq4(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                       kmb, vmb : MetalBuffer?; koff, voff : uint64;
                       bcos, bsin, bqd, bhs, bnh, bkvm : MetalBuffer?; ntg, head_size : int64) {
    metal_set_pipeline(enc, g_pso_rpst_tq4)
    metal_set_threadgroup_memory_length(enc, metal_rope_store_tq4_msl_tgmem, 0)
    metal_set_buffer(enc, bqkv, 0ul, 0)
    metal_set_buffer(enc, bqkv, vsoff, 1)
    metal_set_buffer(enc, kmb, koff, 2)
    metal_set_buffer(enc, kmb, koff, 3)
    metal_set_buffer(enc, vmb, voff, 4)
    metal_set_buffer(enc, vmb, voff, 5)
    metal_set_buffer(enc, bcos, 0ul, 6)
    metal_set_buffer(enc, bsin, 0ul, 7)
    metal_set_buffer(enc, signs_buffer(), 0ul, 8)
    metal_set_buffer(enc, bqd, 0ul, 9)
    metal_set_buffer(enc, bhs, 0ul, 10)
    metal_set_buffer(enc, bnh, 0ul, 11)
    metal_set_buffer(enc, bkvm, 0ul, 12)
    metal_dispatch_threadgroups(enc, uint3(uint(ntg), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

// the tq4 attention-output un-rotate: one threadgroup per (head, row), row stride = qstride
def enc_fwht_unsign(enc : MetalComputeEncoder?; byo, bhs, bqs : MetalBuffer?; nheads, nrows, head_size : int64) {
    metal_set_pipeline(enc, g_pso_unsign)
    metal_set_threadgroup_memory_length(enc, metal_fwht_unsign_msl_tgmem, 0)
    metal_set_buffer(enc, byo, 0ul, 0)
    metal_set_buffer(enc, signs_buffer(), 0ul, 1)
    metal_set_buffer(enc, bhs, 0ul, 2)
    metal_set_buffer(enc, bqs, 0ul, 3)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), uint(nrows), 1u), uint3(uint(head_size), 1u, 1u))
}

// q8_0-mirror rope+store: the K/V mirror rows bind TWICE each (half-scale + byte views at the
// same row offset); grid covers the q pairs + one thread per K/V 32-block
def enc_rope_store_q8(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                      kmb, vmb : MetalBuffer?; koff, voff : uint64;
                      bcos, bsin, bqd, bhs, bnpq, bnblk : MetalBuffer?; nthreads : int64) {
    metal_set_pipeline(enc, g_pso_rpst_q8)
    metal_set_buffer(enc, bqkv, 0ul, 0)
    metal_set_buffer(enc, bqkv, vsoff, 1)
    metal_set_buffer(enc, kmb, koff, 2)
    metal_set_buffer(enc, kmb, koff, 3)
    metal_set_buffer(enc, vmb, voff, 4)
    metal_set_buffer(enc, vmb, voff, 5)
    metal_set_buffer(enc, bcos, 0ul, 6)
    metal_set_buffer(enc, bsin, 0ul, 7)
    metal_set_buffer(enc, bqd, 0ul, 8)
    metal_set_buffer(enc, bhs, 0ul, 9)
    metal_set_buffer(enc, bnpq, 0ul, 10)
    metal_set_buffer(enc, bnblk, 0ul, 11)
    metal_dispatch_threadgroups(enc, uint3(uint((nthreads + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// the fused QKV+rope+store dispatch (2 rows per simdgroup — grid covers ddim/2 pairs)
def enc_gemv_qkv_rs(enc : MetalComputeEncoder?; f16 : bool; bwq, bws, bx, bq : MetalBuffer?;
                    kmb, vmb : MetalBuffer?; koff, voff : uint64;
                    bcos, bsin, bn, bd, bqd, bkvd, bhs : MetalBuffer?; d : int64) {
    metal_set_pipeline(enc, f16 ? g_pso_qkvrs16 : g_pso_qkvrs32)
    metal_set_buffer(enc, bwq, 0ul, 0)
    metal_set_buffer(enc, bws, 0ul, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, bq, 0ul, 3)
    metal_set_buffer(enc, kmb, koff, 4)
    metal_set_buffer(enc, vmb, voff, 5)
    metal_set_buffer(enc, bcos, 0ul, 6)
    metal_set_buffer(enc, bsin, 0ul, 7)
    metal_set_buffer(enc, bn, 0ul, 8)
    metal_set_buffer(enc, bd, 0ul, 9)
    metal_set_buffer(enc, bqd, 0ul, 10)
    metal_set_buffer(enc, bkvd, 0ul, 11)
    metal_set_buffer(enc, bhs, 0ul, 12)
    let pairs = d / 2l
    metal_dispatch_threadgroups(enc, uint3(uint((pairs + 3l) / 4l), 1u, 1u), uint3(128u, 1u, 1u))
}

// the fused W1|W3+swiglu dispatch (2 rows per simdgroup — gate + up of one output element)
def enc_gemv_w13sw(enc : MetalComputeEncoder?; bwq, bws, bx, bh : MetalBuffer?; bn, bhid : MetalBuffer?; hidden : int64) {
    metal_set_pipeline(enc, g_pso_w13sw)
    metal_set_buffer(enc, bwq, 0ul, 0)
    metal_set_buffer(enc, bws, 0ul, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, bh, 0ul, 3)
    metal_set_buffer(enc, bn, 0ul, 4)
    metal_set_buffer(enc, bhid, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((hidden + 3l) / 4l), 1u, 1u), uint3(128u, 1u, 1u))
}

def enc_argmax(enc : MetalComputeEncoder?; blg, btok, bn : MetalBuffer?) {
    metal_set_pipeline(enc, g_pso_argmax)
    metal_set_threadgroup_memory_length(enc, metal_argmax_msl_tgmem, 0)
    metal_set_buffer(enc, blg, 0ul, 0)
    metal_set_buffer(enc, btok, 0ul, 1)
    metal_set_buffer(enc, bn, 0ul, 2)
    metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(1024u, 1u, 1u))
}

// zero-copy logits scatter: one staging row (src at its byte offset) into a session's wrapped
// logits storage — replaces that row's landing memcpy
def enc_copy_row(enc : MetalComputeEncoder?; bsrc : MetalBuffer?; soff : uint64; bdst, bn : MetalBuffer?; n : int64) {
    metal_set_pipeline(enc, g_pso_copy_row)
    metal_set_buffer(enc, bsrc, soff, 0)
    metal_set_buffer(enc, bdst, 0ul, 1)
    metal_set_buffer(enc, bn, 0ul, 2)
    metal_dispatch_threadgroups(enc, uint3(uint((n + 255l) / 256l), 1u, 1u), uint3(256u, 1u, 1u))
}

def enc_embed(enc : MetalComputeEncoder?; s16 : bool; bq, bs, btok, bx, bdim : MetalBuffer?; dim : int64) {
    metal_set_pipeline(enc, s16 ? g_pso_emb16 : g_pso_emb)
    metal_set_buffer(enc, bq, 0ul, 0)
    metal_set_buffer(enc, bs, 0ul, 1)
    metal_set_buffer(enc, btok, 0ul, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, bdim, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint((dim + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// fused add + rms when dim fits the kernel's row slab, the unfused pair otherwise
def enc_add_rms_site(enc : MetalComputeEncoder?; bx, ba, bw, by, bdim, beps : MetalBuffer?; dim : int64) {
    if (dim <= ADD_RMS_MAX_DIM) {
        metal_set_pipeline(enc, g_pso_addrms)
        metal_set_threadgroup_memory_length(enc, metal_add_rms_msl_tgmem, 0)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, ba, 0ul, 1)
        metal_set_buffer(enc, bw, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bdim, 0ul, 4)
        metal_set_buffer(enc, beps, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(1024u, 1u, 1u))
    } else {
        enc_ew2(enc, g_pso_add, bx, 0ul, ba, 0ul, bdim, dim)
        enc_rms(enc, bx, bw, by, bdim, beps)
    }
}

def enc_attn(enc : MetalComputeEncoder?; f16 : bool; kmb, vmb : MetalBuffer?; koff, voff : uint64;
             bq, by, bcnt, bkvd, bnh, bkvm, bhs, bscale : MetalBuffer?; nheads, head_size : int64) {
    metal_set_pipeline(enc, f16 ? g_pso_attn16 : g_pso_attn32)
    metal_set_threadgroup_memory_length(enc, f16 ? metal_sq_attn16_msl_tgmem : metal_sq_attn32_msl_tgmem, 0)
    metal_set_buffer(enc, kmb, koff, 0)
    metal_set_buffer(enc, vmb, voff, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, bcnt, 0ul, 4)
    metal_set_buffer(enc, bkvd, 0ul, 5)
    metal_set_buffer(enc, bnh, 0ul, 6)
    metal_set_buffer(enc, bkvm, 0ul, 7)
    metal_set_buffer(enc, bhs, 0ul, 8)
    metal_set_buffer(enc, bscale, 0ul, 9)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part(enc : MetalComputeEncoder?; f16 : bool; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                  bq, bpart, bcnt, bkvd, bnh, bkvm, bhs, bscale, bnch : MetalBuffer?; ntg, head_size : int64) {
    metal_set_pipeline(enc, f16 ? g_pso_attnp16 : g_pso_attnp32)
    metal_set_threadgroup_memory_length(enc, f16 ? metal_sq_attn_part16_msl_tgmem : metal_sq_attn_part32_msl_tgmem, 0)
    metal_set_buffer(enc, kmb, koff, 0)
    metal_set_buffer(enc, vmb, voff, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, bpart, 0ul, 3)
    metal_set_buffer(enc, bcnt, 0ul, 4)
    metal_set_buffer(enc, bkvd, 0ul, 5)
    metal_set_buffer(enc, bnh, 0ul, 6)
    metal_set_buffer(enc, bkvm, 0ul, 7)
    metal_set_buffer(enc, bhs, 0ul, 8)
    metal_set_buffer(enc, bscale, 0ul, 9)
    metal_set_buffer(enc, bnch, 0ul, 10)
    metal_dispatch_threadgroups(enc, uint3(uint(ntg), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

// tq4-mirror attention twins — same dual-view shape as the q8_0 encoders below
def enc_attn_tq4(enc : MetalComputeEncoder?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                 bq, by, bcnt, bnblk, bnh, bkvm, bhs, bscale : MetalBuffer?; nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attn_tq4)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_tq4_msl_tgmem, 0)
    metal_set_buffer(enc, kmb, koff, 0)
    metal_set_buffer(enc, kmb, koff, 1)
    metal_set_buffer(enc, vmb, voff, 2)
    metal_set_buffer(enc, vmb, voff, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, by, 0ul, 5)
    metal_set_buffer(enc, bcnt, 0ul, 6)
    metal_set_buffer(enc, bnblk, 0ul, 7)
    metal_set_buffer(enc, bnh, 0ul, 8)
    metal_set_buffer(enc, bkvm, 0ul, 9)
    metal_set_buffer(enc, bhs, 0ul, 10)
    metal_set_buffer(enc, bscale, 0ul, 11)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part_tq4(enc : MetalComputeEncoder?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                      bq, bpart, bcnt, bnblk, bnh, bkvm, bhs, bscale, bnch : MetalBuffer?; ntg, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attnp_tq4)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_part_tq4_msl_tgmem, 0)
    metal_set_buffer(enc, kmb, koff, 0)
    metal_set_buffer(enc, kmb, koff, 1)
    metal_set_buffer(enc, vmb, voff, 2)
    metal_set_buffer(enc, vmb, voff, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, bpart, 0ul, 5)
    metal_set_buffer(enc, bcnt, 0ul, 6)
    metal_set_buffer(enc, bnblk, 0ul, 7)
    metal_set_buffer(enc, bnh, 0ul, 8)
    metal_set_buffer(enc, bkvm, 0ul, 9)
    metal_set_buffer(enc, bhs, 0ul, 10)
    metal_set_buffer(enc, bscale, 0ul, 11)
    metal_set_buffer(enc, bnch, 0ul, 12)
    metal_dispatch_threadgroups(enc, uint3(uint(ntg), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

// q8_0-mirror attention twins: the K/V slabs bind TWICE each (half-scale + byte views at the
// same layer offset); nblk (blocks per row) replaces the kv_dim uniform
def enc_attn_q8(enc : MetalComputeEncoder?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                bq, by, bcnt, bnblk, bnh, bkvm, bhs, bscale : MetalBuffer?; nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attn_q8)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_q8_msl_tgmem, 0)
    metal_set_buffer(enc, kmb, koff, 0)
    metal_set_buffer(enc, kmb, koff, 1)
    metal_set_buffer(enc, vmb, voff, 2)
    metal_set_buffer(enc, vmb, voff, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, by, 0ul, 5)
    metal_set_buffer(enc, bcnt, 0ul, 6)
    metal_set_buffer(enc, bnblk, 0ul, 7)
    metal_set_buffer(enc, bnh, 0ul, 8)
    metal_set_buffer(enc, bkvm, 0ul, 9)
    metal_set_buffer(enc, bhs, 0ul, 10)
    metal_set_buffer(enc, bscale, 0ul, 11)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part_q8(enc : MetalComputeEncoder?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                     bq, bpart, bcnt, bnblk, bnh, bkvm, bhs, bscale, bnch : MetalBuffer?; ntg, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attnp_q8)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_part_q8_msl_tgmem, 0)
    metal_set_buffer(enc, kmb, koff, 0)
    metal_set_buffer(enc, kmb, koff, 1)
    metal_set_buffer(enc, vmb, voff, 2)
    metal_set_buffer(enc, vmb, voff, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, bpart, 0ul, 5)
    metal_set_buffer(enc, bcnt, 0ul, 6)
    metal_set_buffer(enc, bnblk, 0ul, 7)
    metal_set_buffer(enc, bnh, 0ul, 8)
    metal_set_buffer(enc, bkvm, 0ul, 9)
    metal_set_buffer(enc, bhs, 0ul, 10)
    metal_set_buffer(enc, bscale, 0ul, 11)
    metal_set_buffer(enc, bnch, 0ul, 12)
    metal_dispatch_threadgroups(enc, uint3(uint(ntg), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_comb(enc : MetalComputeEncoder?; bpart, by, bnch, bnh, bhs : MetalBuffer?; nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attncomb)
    metal_set_buffer(enc, bpart, 0ul, 0)
    metal_set_buffer(enc, by, 0ul, 1)
    metal_set_buffer(enc, bnch, 0ul, 2)
    metal_set_buffer(enc, bnh, 0ul, 3)
    metal_set_buffer(enc, bhs, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

// swiglu and add share the (a, b, total) elementwise shape; offsets serve the fused-buffer binds
def enc_ew2(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; ba : MetalBuffer?; aoff : uint64; bb : MetalBuffer?; boff : uint64; btot : MetalBuffer?; total : int64) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, ba, aoff, 0)
    metal_set_buffer(enc, bb, boff, 1)
    metal_set_buffer(enc, btot, 0ul, 2)
    metal_dispatch_threadgroups(enc, uint3(uint((total + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// ===== the batched-step encoders (P4) =====

// fixed-B batched GEMV: B2 form serves B <= 2, B4 serves 3..4 (pad streams read stale rows,
// never consumed). Geometry pinned at 4 rows/tg with 128 threads baked in (g_gemv_rows knob
// stays single-stream-only).
def enc_gemv_b(enc : MetalComputeEncoder?; four : bool; bwq : MetalBuffer?; wqoff : uint64;
               bws : MetalBuffer?; wsoff : uint64; bx, by, bn, bd, bs16 : MetalBuffer?; d : int64) {
    metal_set_pipeline(enc, four ? g_pso_gemv_b4 : g_pso_gemv_b2)
    metal_set_threadgroup_memory_length(enc, four ? metal_gemv_b4_msl_tgmem : metal_gemv_b2_msl_tgmem, 0)
    metal_set_buffer(enc, bwq, wqoff, 0)
    metal_set_buffer(enc, bws, wsoff, 1)
    metal_set_buffer(enc, bws, wsoff, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, by, 0ul, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_set_buffer(enc, bd, 0ul, 6)
    metal_set_buffer(enc, bs16, 0ul, 7)
    metal_dispatch_threadgroups(enc, uint3(uint((d + 3l) / 4l), 1u, 1u), uint3(128u, 1u, 1u))
}

// the mul_mv-shape fixed-B GEMV on an interleaved 34B blob (dual views of the SAME buffer);
// bxs = x row stride in float4s; bnr = the nrows uniform, four-form only; xoff/yoff = the
// column-tile byte offsets (B > 4 streams the weights once per 4-column tile)
def enc_mv_b(enc : MetalComputeEncoder?; four : bool; bwblob, bx, by, bn, bd, bxs : MetalBuffer?; d : int64;
             bnr : MetalBuffer? = null; xoff : uint64 = 0ul; yoff : uint64 = 0ul) {
    metal_set_pipeline(enc, four ? g_pso_mv_b4 : g_pso_mv_b2)
    metal_set_buffer(enc, bwblob, 0ul, 0)
    metal_set_buffer(enc, bwblob, 0ul, 1)
    metal_set_buffer(enc, bx, xoff, 2)
    metal_set_buffer(enc, by, yoff, 3)
    metal_set_buffer(enc, bn, 0ul, 4)
    metal_set_buffer(enc, bd, 0ul, 5)
    if (four) {
        metal_set_buffer(enc, bnr, 0ul, 6)
        metal_set_buffer(enc, bxs, 0ul, 7)
    } else {
        metal_set_buffer(enc, bxs, 0ul, 6)
    }
    // b2 = 4 rows/tg (16 lanes/row); b4 = 8 rows/tg (8 lanes/row)
    metal_dispatch_threadgroups(enc, uint3(uint(four ? (d + 7l) / 8l : (d + 3l) / 4l), 1u, 1u), uint3(64u, 1u, 1u))
}

// one mv GEMV site: a single form dispatch at B <= 4, two 4-column tiles at B = 5..8 (the
// reference's own shape — it never switches to GEMM below B = 9; the weights stream twice but
// stay the fast skinny form). xrowb = the x row stride in bytes; bnr/bnr2 = per-tile nrows.
def enc_mv_site(enc : MetalComputeEncoder?; bwblob, bx, by, bn, bd, bxs : MetalBuffer?;
                d, nrows : int64; xrowb : uint64; bnr, bnr2 : MetalBuffer?) {
    enc_mv_b(enc, nrows > 2l, bwblob, bx, by, bn, bd, bxs, d, bnr)
    if (nrows > 4l) {
        enc_mv_b(enc, true, bwblob, bx, by, bn, bd, bxs, d, bnr2, 4ul * xrowb, uint64(4l * d * 4l))
    }
}

// fused fixed-B W1|W3 GEMV + swiglu — bwq/bws are the cat W1|W3 region at offset 0 (up rows sit
// at +hidden internally), by is the [B x hidden] swiglu output. Replaces the w1 + w3 GEMVs + the
// swiglu ew2 with one dispatch. Same 4-rows/tg geometry, gridded over the hidden output rows.
def enc_gemv_w13sw_b(enc : MetalComputeEncoder?; four : bool; bwq, bws, bx, by, bn, bhid, bs16 : MetalBuffer?; hidden : int64) {
    metal_set_pipeline(enc, four ? g_pso_w13sw_b4 : g_pso_w13sw_b2)
    metal_set_threadgroup_memory_length(enc, four ? metal_gemv_w13sw_b4_msl_tgmem : metal_gemv_w13sw_b2_msl_tgmem, 0)
    metal_set_buffer(enc, bwq, 0ul, 0)
    metal_set_buffer(enc, bws, 0ul, 1)
    metal_set_buffer(enc, bws, 0ul, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, by, 0ul, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_set_buffer(enc, bhid, 0ul, 6)
    metal_set_buffer(enc, bs16, 0ul, 7)
    metal_dispatch_threadgroups(enc, uint3(uint((hidden + 3l) / 4l), 1u, 1u), uint3(128u, 1u, 1u))
}

// the production mul_mm dispatch (the prefill kernel: 34B-blob dual-view W, raw f32 X): y binds
// at a byte offset so per-tensor tiles land inside a wider fused row — the ndim uniform is the
// y ROW STRIDE while this tensor's output width d shapes the grid (d % 64 == 0, mp % 32 == 0)
def enc_gemm_mm_b(enc : MetalComputeEncoder?; bwblob, bx, by : MetalBuffer?; yoff : uint64;
                  bk, bn : MetalBuffer?; mp, d : int64) {
    metal_set_pipeline(enc, g_pso_mm)
    metal_set_threadgroup_memory_length(enc, metal_q8_mulmm_msl_tgmem, 0)
    metal_set_buffer(enc, bwblob, 0ul, 0)
    metal_set_buffer(enc, bwblob, 0ul, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, yoff, 3)
    metal_set_buffer(enc, bk, 0ul, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(d / 64l), 1u), uint3(128u, 1u, 1u))
}

// the M-pad-32 GEMM (B >= 5): raw f32 X, dual-view W scales
def enc_gemm_b(enc : MetalComputeEncoder?; bwq : MetalBuffer?; wqoff : uint64;
               bws : MetalBuffer?; wsoff : uint64; bx, by, bk, bn, bs16 : MetalBuffer?; mp32, d : int64) {
    metal_set_pipeline(enc, g_pso_gemm_b)
    metal_set_threadgroup_memory_length(enc, metal_q8_gemm_b_msl_tgmem, 0)
    metal_set_buffer(enc, bwq, wqoff, 0)
    metal_set_buffer(enc, bws, wsoff, 1)
    metal_set_buffer(enc, bws, wsoff, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, by, 0ul, 4)
    metal_set_buffer(enc, bk, 0ul, 5)
    metal_set_buffer(enc, bn, 0ul, 6)
    metal_set_buffer(enc, bs16, 0ul, 7)
    metal_dispatch_threadgroups(enc, uint3(uint((mp32 / 32l) * (d / 32l)), 1u, 1u), uint3(128u, 1u, 1u))
}

// the split-K pair (per-site policy): the GEMM's grid.y is the k-slice axis, partial planes land
// in bpart, and the reduce sums them into y — the serial encoder orders the two dispatches.
// Planes and grids assume mp == 32 (the policy gate).
def enc_gemm_sk_b(enc : MetalComputeEncoder?; bwq : MetalBuffer?; wqoff : uint64;
                  bws : MetalBuffer?; wsoff : uint64; bx, bpart, bk, bn, bs16, bsk : MetalBuffer?; d, nsk : int64) {
    metal_set_pipeline(enc, g_pso_gemm_b_sk)
    metal_set_threadgroup_memory_length(enc, metal_q8_gemm_b_sk_msl_tgmem, 0)
    metal_set_buffer(enc, bwq, wqoff, 0)
    metal_set_buffer(enc, bws, wsoff, 1)
    metal_set_buffer(enc, bws, wsoff, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, bpart, 0ul, 4)
    metal_set_buffer(enc, bk, 0ul, 5)
    metal_set_buffer(enc, bn, 0ul, 6)
    metal_set_buffer(enc, bs16, 0ul, 7)
    metal_set_buffer(enc, bsk, 0ul, 8)
    metal_dispatch_threadgroups(enc, uint3(uint(d / 32l), uint(nsk), 1u), uint3(128u, 1u, 1u))
}

def enc_sk_reduce_b(enc : MetalComputeEncoder?; bpart, by, btot, bsk : MetalBuffer?; total : int64) {
    metal_set_pipeline(enc, g_pso_skred)
    metal_set_buffer(enc, bpart, 0ul, 0)
    metal_set_buffer(enc, by, 0ul, 1)
    metal_set_buffer(enc, btot, 0ul, 2)
    metal_set_buffer(enc, bsk, 0ul, 3)
    metal_dispatch_threadgroups(enc, uint3(uint((total + 127l) / 128l), 1u, 1u), uint3(128u, 1u, 1u))
}

// the 64-wide N tile (classifier site): the caller gates d % 64 == 0
def enc_gemm64_b(enc : MetalComputeEncoder?; bwq : MetalBuffer?; wqoff : uint64;
                 bws : MetalBuffer?; wsoff : uint64; bx, by, bk, bn, bs16 : MetalBuffer?; mp32, d : int64) {
    metal_set_pipeline(enc, g_pso_gemm64_b)
    metal_set_threadgroup_memory_length(enc, metal_q8_gemm64_b_msl_tgmem, 0)
    metal_set_buffer(enc, bwq, wqoff, 0)
    metal_set_buffer(enc, bws, wsoff, 1)
    metal_set_buffer(enc, bws, wsoff, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, by, 0ul, 4)
    metal_set_buffer(enc, bk, 0ul, 5)
    metal_set_buffer(enc, bn, 0ul, 6)
    metal_set_buffer(enc, bs16, 0ul, 7)
    metal_dispatch_threadgroups(enc, uint3(uint((mp32 / 32l) * (d / 64l)), 1u, 1u), uint3(128u, 1u, 1u))
}

// Per-encoder buffer-hazard tracker for concurrent encoding (g_bconcurrent): each scratch buffer
// owns a bit; a barrier is inserted iff a dispatch's read hits a pending write (RAW), or its
// write hits a pending write/read (WAW/WAR) — read-after-read runs concurrently.

def enc_attn_b(enc : MetalComputeEncoder?; f16 : bool; bq, by, brt, blay, bkvd, bnh, bkvm, bhs, bscale, bqs : MetalBuffer?; nrows, nheads, head_size : int64) {
    metal_set_pipeline(enc, f16 ? g_pso_attnb16 : g_pso_attnb32)
    metal_set_threadgroup_memory_length(enc, f16 ? metal_sq_attn_b16_msl_tgmem : metal_sq_attn_b32_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, brt, 0ul, 4)
    metal_set_buffer(enc, blay, 0ul, 5)
    metal_set_buffer(enc, bkvd, 0ul, 6)
    metal_set_buffer(enc, bnh, 0ul, 7)
    metal_set_buffer(enc, bkvm, 0ul, 8)
    metal_set_buffer(enc, bhs, 0ul, 9)
    metal_set_buffer(enc, bscale, 0ul, 10)
    metal_set_buffer(enc, bqs, 0ul, 11)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part_b(enc : MetalComputeEncoder?; f16 : bool; bq, bpart, brt, blay, bkvd, bnh, bkvm, bhs, bscale, bqs, bnch : MetalBuffer?; nrows, nchmax, nheads, head_size : int64) {
    metal_set_pipeline(enc, f16 ? g_pso_attnpb16 : g_pso_attnpb32)
    metal_set_threadgroup_memory_length(enc, f16 ? metal_sq_attn_part_b16_msl_tgmem : metal_sq_attn_part_b32_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, bpart, 0ul, 3)
    metal_set_buffer(enc, brt, 0ul, 4)
    metal_set_buffer(enc, blay, 0ul, 5)
    metal_set_buffer(enc, bkvd, 0ul, 6)
    metal_set_buffer(enc, bnh, 0ul, 7)
    metal_set_buffer(enc, bkvm, 0ul, 8)
    metal_set_buffer(enc, bhs, 0ul, 9)
    metal_set_buffer(enc, bscale, 0ul, 10)
    metal_set_buffer(enc, bqs, 0ul, 11)
    metal_set_buffer(enc, bnch, 0ul, 12)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads * nchmax * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

// tq4-mirror batch attention twins — same four-way arena binding as the q8_0 encoders below
def enc_attn_b_tq4(enc : MetalComputeEncoder?; bq, by, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs : MetalBuffer?; nrows, nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attnb_tq4)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_b_tq4_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, g_arena, 0ul, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, by, 0ul, 5)
    metal_set_buffer(enc, brt, 0ul, 6)
    metal_set_buffer(enc, blay, 0ul, 7)
    metal_set_buffer(enc, bnblk, 0ul, 8)
    metal_set_buffer(enc, bnh, 0ul, 9)
    metal_set_buffer(enc, bkvm, 0ul, 10)
    metal_set_buffer(enc, bhs, 0ul, 11)
    metal_set_buffer(enc, bscale, 0ul, 12)
    metal_set_buffer(enc, bqs, 0ul, 13)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part_b_tq4(enc : MetalComputeEncoder?; bq, bpart, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bnch : MetalBuffer?; nrows, nchmax, nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attnpb_tq4)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_part_b_tq4_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, g_arena, 0ul, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, bpart, 0ul, 5)
    metal_set_buffer(enc, brt, 0ul, 6)
    metal_set_buffer(enc, blay, 0ul, 7)
    metal_set_buffer(enc, bnblk, 0ul, 8)
    metal_set_buffer(enc, bnh, 0ul, 9)
    metal_set_buffer(enc, bkvm, 0ul, 10)
    metal_set_buffer(enc, bhs, 0ul, 11)
    metal_set_buffer(enc, bscale, 0ul, 12)
    metal_set_buffer(enc, bqs, 0ul, 13)
    metal_set_buffer(enc, bnch, 0ul, 14)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads * nchmax * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_d_tq4(enc : MetalComputeEncoder?; bq, bpart, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bnsg : MetalBuffer?; nsgs, nrows, nheads : int64) {
    metal_set_pipeline(enc, g_pso_attnd_tq4)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_d_tq4_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, g_arena, 0ul, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, bpart, 0ul, 5)
    metal_set_buffer(enc, brt, 0ul, 6)
    metal_set_buffer(enc, blay, 0ul, 7)
    metal_set_buffer(enc, bnblk, 0ul, 8)
    metal_set_buffer(enc, bnh, 0ul, 9)
    metal_set_buffer(enc, bkvm, 0ul, 10)
    metal_set_buffer(enc, bhs, 0ul, 11)
    metal_set_buffer(enc, bscale, 0ul, 12)
    metal_set_buffer(enc, bqs, 0ul, 13)
    metal_set_buffer(enc, bnsg, 0ul, 14)
    // HEAD-FASTEST launch: consecutive tgs share position rows (SLC locality at depth)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), uint((nsgs + 3l) / 4l), uint(nrows)), uint3(128u, 1u, 1u))
}

// q8_0-mirror batch attention twins: the arena binds FOUR times (half-scale + byte views for K
// and V); the row table carries byte offsets and nblk replaces the kv_dim uniform
def enc_attn_b_q8(enc : MetalComputeEncoder?; bq, by, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs : MetalBuffer?; nrows, nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attnb_q8)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_b_q8_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, g_arena, 0ul, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, by, 0ul, 5)
    metal_set_buffer(enc, brt, 0ul, 6)
    metal_set_buffer(enc, blay, 0ul, 7)
    metal_set_buffer(enc, bnblk, 0ul, 8)
    metal_set_buffer(enc, bnh, 0ul, 9)
    metal_set_buffer(enc, bkvm, 0ul, 10)
    metal_set_buffer(enc, bhs, 0ul, 11)
    metal_set_buffer(enc, bscale, 0ul, 12)
    metal_set_buffer(enc, bqs, 0ul, 13)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part_b_q8(enc : MetalComputeEncoder?; bq, bpart, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bnch : MetalBuffer?; nrows, nchmax, nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attnpb_q8)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_part_b_q8_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, g_arena, 0ul, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, bpart, 0ul, 5)
    metal_set_buffer(enc, brt, 0ul, 6)
    metal_set_buffer(enc, blay, 0ul, 7)
    metal_set_buffer(enc, bnblk, 0ul, 8)
    metal_set_buffer(enc, bnh, 0ul, 9)
    metal_set_buffer(enc, bkvm, 0ul, 10)
    metal_set_buffer(enc, bhs, 0ul, 11)
    metal_set_buffer(enc, bscale, 0ul, 12)
    metal_set_buffer(enc, bqs, 0ul, 13)
    metal_set_buffer(enc, bnch, 0ul, 14)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads * nchmax * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_d_q8(enc : MetalComputeEncoder?; bq, bpart, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bnsg : MetalBuffer?; nsgs, nrows, nheads : int64) {
    metal_set_pipeline(enc, g_pso_attnd_q8)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_d_q8_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, g_arena, 0ul, 3)
    metal_set_buffer(enc, bq, 0ul, 4)
    metal_set_buffer(enc, bpart, 0ul, 5)
    metal_set_buffer(enc, brt, 0ul, 6)
    metal_set_buffer(enc, blay, 0ul, 7)
    metal_set_buffer(enc, bnblk, 0ul, 8)
    metal_set_buffer(enc, bnh, 0ul, 9)
    metal_set_buffer(enc, bkvm, 0ul, 10)
    metal_set_buffer(enc, bhs, 0ul, 11)
    metal_set_buffer(enc, bscale, 0ul, 12)
    metal_set_buffer(enc, bqs, 0ul, 13)
    metal_set_buffer(enc, bnsg, 0ul, 14)
    // HEAD-FASTEST launch: consecutive tgs share position rows (SLC locality at depth)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), uint((nsgs + 3l) / 4l), uint(nrows)), uint3(128u, 1u, 1u))
}

def enc_attn_comb_b(enc : MetalComputeEncoder?; bpart, by, brt, bnch, bnh, bhs : MetalBuffer?; nrows, nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attncombb)
    metal_set_buffer(enc, bpart, 0ul, 0)
    metal_set_buffer(enc, by, 0ul, 1)
    metal_set_buffer(enc, brt, 0ul, 2)
    metal_set_buffer(enc, bnch, 0ul, 3)
    metal_set_buffer(enc, bnh, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_d(enc : MetalComputeEncoder?; f16 : bool; bq, bpart, brt, blay, bkvd, bnh, bkvm, bhs, bscale, bqs, bnsg : MetalBuffer?; nsgs, nrows, nheads : int64) {
    metal_set_pipeline(enc, f16 ? g_pso_attnd16 : g_pso_attnd32)
    metal_set_threadgroup_memory_length(enc, f16 ? metal_sq_attn_d16_msl_tgmem : metal_sq_attn_d32_msl_tgmem, 0)
    metal_set_buffer(enc, g_arena, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, bpart, 0ul, 3)
    metal_set_buffer(enc, brt, 0ul, 4)
    metal_set_buffer(enc, blay, 0ul, 5)
    metal_set_buffer(enc, bkvd, 0ul, 6)
    metal_set_buffer(enc, bnh, 0ul, 7)
    metal_set_buffer(enc, bkvm, 0ul, 8)
    metal_set_buffer(enc, bhs, 0ul, 9)
    metal_set_buffer(enc, bscale, 0ul, 10)
    metal_set_buffer(enc, bqs, 0ul, 11)
    metal_set_buffer(enc, bnsg, 0ul, 12)
    // HEAD-FASTEST launch: consecutive tgs share position rows (SLC locality at depth)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), uint((nsgs + 3l) / 4l), uint(nrows)), uint3(128u, 1u, 1u))
}

def enc_attn_comb_d(enc : MetalComputeEncoder?; bpart, by, brt, bnsg, bnh, bhs : MetalBuffer?; nrows, nheads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_attncombd)
    metal_set_threadgroup_memory_length(enc, metal_sq_attn_comb_d_msl_tgmem, 0)
    metal_set_buffer(enc, bpart, 0ul, 0)
    metal_set_buffer(enc, by, 0ul, 1)
    metal_set_buffer(enc, brt, 0ul, 2)
    metal_set_buffer(enc, bnsg, 0ul, 3)
    metal_set_buffer(enc, bnh, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(nheads), uint(nrows), 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_rope_store_b(enc : MetalComputeEncoder?; f16 : bool; bqkv, bcos, bsin, brt, blay, bkvd, bqd, bhs, bnpq, bnpall, bqs : MetalBuffer?; npairs, nrows : int64) {
    metal_set_pipeline(enc, f16 ? g_pso_rpstb16 : g_pso_rpstb32)
    metal_set_buffer(enc, bqkv, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, bcos, 0ul, 3)
    metal_set_buffer(enc, bsin, 0ul, 4)
    metal_set_buffer(enc, brt, 0ul, 5)
    metal_set_buffer(enc, blay, 0ul, 6)
    metal_set_buffer(enc, bkvd, 0ul, 7)
    metal_set_buffer(enc, bqd, 0ul, 8)
    metal_set_buffer(enc, bhs, 0ul, 9)
    metal_set_buffer(enc, bnpq, 0ul, 10)
    metal_set_buffer(enc, bnpall, 0ul, 11)
    metal_set_buffer(enc, bqs, 0ul, 12)
    metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63l) / 64l), uint(nrows), 1u), uint3(64u, 1u, 1u))
}

// tq4-mirror batched rope+store: one threadgroup per (head, batch row), arena bound four ways
def enc_rope_store_b_tq4(enc : MetalComputeEncoder?; bqkv, bcos, bsin, brt, blay, bqd, bhs, bnh, bkvm, bqs : MetalBuffer?; ntg, nrows, head_size : int64) {
    metal_set_pipeline(enc, g_pso_rpstb_tq4)
    metal_set_threadgroup_memory_length(enc, metal_rope_store_b_tq4_msl_tgmem, 0)
    metal_set_buffer(enc, bqkv, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, g_arena, 0ul, 3)
    metal_set_buffer(enc, g_arena, 0ul, 4)
    metal_set_buffer(enc, bcos, 0ul, 5)
    metal_set_buffer(enc, bsin, 0ul, 6)
    metal_set_buffer(enc, signs_buffer(), 0ul, 7)
    metal_set_buffer(enc, brt, 0ul, 8)
    metal_set_buffer(enc, blay, 0ul, 9)
    metal_set_buffer(enc, bqd, 0ul, 10)
    metal_set_buffer(enc, bhs, 0ul, 11)
    metal_set_buffer(enc, bnh, 0ul, 12)
    metal_set_buffer(enc, bkvm, 0ul, 13)
    metal_set_buffer(enc, bqs, 0ul, 14)
    metal_dispatch_threadgroups(enc, uint3(uint(ntg), uint(nrows), 1u), uint3(uint(head_size), 1u, 1u))
}

// q8_0-mirror batched rope+store: arena bound four ways (K/V x scale/byte views); grid covers
// the q pairs + one thread per K/V 32-block, one grid row per batch row
def enc_rope_store_b_q8(enc : MetalComputeEncoder?; bqkv, bcos, bsin, brt, blay, bnblk, bqd, bhs, bnpq, bqs : MetalBuffer?; nthreads, nrows : int64) {
    metal_set_pipeline(enc, g_pso_rpstb_q8)
    metal_set_buffer(enc, bqkv, 0ul, 0)
    metal_set_buffer(enc, g_arena, 0ul, 1)
    metal_set_buffer(enc, g_arena, 0ul, 2)
    metal_set_buffer(enc, g_arena, 0ul, 3)
    metal_set_buffer(enc, g_arena, 0ul, 4)
    metal_set_buffer(enc, bcos, 0ul, 5)
    metal_set_buffer(enc, bsin, 0ul, 6)
    metal_set_buffer(enc, brt, 0ul, 7)
    metal_set_buffer(enc, blay, 0ul, 8)
    metal_set_buffer(enc, bnblk, 0ul, 9)
    metal_set_buffer(enc, bqd, 0ul, 10)
    metal_set_buffer(enc, bhs, 0ul, 11)
    metal_set_buffer(enc, bnpq, 0ul, 12)
    metal_set_buffer(enc, bqs, 0ul, 13)
    metal_dispatch_threadgroups(enc, uint3(uint((nthreads + 63l) / 64l), uint(nrows), 1u), uint3(64u, 1u, 1u))
}

def enc_add_rms_b(enc : MetalComputeEncoder?; bx, ba, bw, by, bdim, beps : MetalBuffer?; nrows : int64) {
    metal_set_pipeline(enc, g_pso_addrmsb)
    metal_set_threadgroup_memory_length(enc, metal_add_rms_b_msl_tgmem, 0)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, ba, 0ul, 1)
    metal_set_buffer(enc, bw, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, bdim, 0ul, 4)
    metal_set_buffer(enc, beps, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(1024u, 1u, 1u))
}

// batched rmsnorm (the prologue): the prefill rms kernel is width-agnostic — one tg per row
def enc_rms_b(enc : MetalComputeEncoder?; bx, bw, by, bdim, beps : MetalBuffer?; nrows : int64) {
    metal_set_pipeline(enc, g_pso_rms)
    metal_set_threadgroup_memory_length(enc, metal_rmsnorm_msl_tgmem, 0)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, bw, 0ul, 1)
    metal_set_buffer(enc, by, 0ul, 2)
    metal_set_buffer(enc, bdim, 0ul, 3)
    metal_set_buffer(enc, beps, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(1024u, 1u, 1u))
}


//! Release every PSO the registry compiled (the PSO half of metal_decode_shutdown — runs
//! before the shared device goes).
def metal_kernels_release {
    if (g_pso_gemv != null) {
        metal_release(g_pso_gemv)
        g_pso_gemv = null
    }
    if (g_pso_gemv16 != null) {
        metal_release(g_pso_gemv16)
        g_pso_gemv16 = null
    }
    release_pso(g_pso_kq_k4)
    release_pso(g_pso_kq_k5)
    release_pso(g_pso_kq_k6)
    release_pso(g_pso_kq_mvb2_k4)
    release_pso(g_pso_kq_mvb4_k4)
    release_pso(g_pso_kq_mvb2_k5)
    release_pso(g_pso_kq_mvb4_k5)
    release_pso(g_pso_kq_mvb2_k6)
    release_pso(g_pso_kq_mvb4_k6)
    release_pso(g_pso_kq_mm_b4)
    release_pso(g_pso_kq_mm_b5)
    release_pso(g_pso_kq_mm_b6)
    if (g_pso_rms != null) {
        metal_release(g_pso_rms)
        g_pso_rms = null
    }
    if (g_pso_add != null) {
        metal_release(g_pso_add)
        g_pso_add = null
    }
    if (g_pso_addrms != null) {
        metal_release(g_pso_addrms)
        g_pso_addrms = null
    }
    if (g_pso_swiglu != null) {
        metal_release(g_pso_swiglu)
        g_pso_swiglu = null
    }
    if (g_pso_attn16 != null) {
        metal_release(g_pso_attn16)
        g_pso_attn16 = null
    }
    if (g_pso_attn32 != null) {
        metal_release(g_pso_attn32)
        g_pso_attn32 = null
    }
    if (g_pso_attnp16 != null) {
        metal_release(g_pso_attnp16)
        g_pso_attnp16 = null
    }
    if (g_pso_attnp32 != null) {
        metal_release(g_pso_attnp32)
        g_pso_attnp32 = null
    }
    release_pso(g_pso_attn_q8)
    release_pso(g_pso_attnp_q8)
    release_pso(g_pso_rpst_q8)
    release_pso(g_pso_attn_tq4)
    release_pso(g_pso_attnp_tq4)
    release_pso(g_pso_rpst_tq4)
    release_pso(g_pso_unsign)
    if (g_pso_attncomb != null) {
        metal_release(g_pso_attncomb)
        g_pso_attncomb = null
    }
    if (g_pso_rpst16 != null) {
        metal_release(g_pso_rpst16)
        g_pso_rpst16 = null
    }
    if (g_pso_rpst32 != null) {
        metal_release(g_pso_rpst32)
        g_pso_rpst32 = null
    }
    if (g_pso_argmax != null) {
        metal_release(g_pso_argmax)
        g_pso_argmax = null
    }
    if (g_pso_copy_row != null) {
        metal_release(g_pso_copy_row)
        g_pso_copy_row = null
    }
    if (g_pso_emb != null) {
        metal_release(g_pso_emb)
        g_pso_emb = null
    }
    if (g_pso_emb16 != null) {
        metal_release(g_pso_emb16)
        g_pso_emb16 = null
    }
    if (g_pso_qkvrs16 != null) {
        metal_release(g_pso_qkvrs16)
        g_pso_qkvrs16 = null
    }
    if (g_pso_qkvrs32 != null) {
        metal_release(g_pso_qkvrs32)
        g_pso_qkvrs32 = null
    }
    if (g_pso_w13sw != null) {
        metal_release(g_pso_w13sw)
        g_pso_w13sw = null
    }
    release_pso(g_pso_gemv_b2)
    release_pso(g_pso_gemv_b4)
    release_pso(g_pso_mv_b2)
    release_pso(g_pso_mv_b4)
    release_pso(g_pso_w13sw_b2)
    release_pso(g_pso_w13sw_b4)
    release_pso(g_pso_gemm_b)
    release_pso(g_pso_gemm_b_sk)
    release_pso(g_pso_skred)
    release_pso(g_pso_gemm64_b)
    release_pso(g_pso_mm)
    release_pso(g_pso_attnb16)
    release_pso(g_pso_attnb32)
    release_pso(g_pso_attnb_q8)
    release_pso(g_pso_attnb_tq4)
    release_pso(g_pso_attnpb16)
    release_pso(g_pso_attnpb32)
    release_pso(g_pso_attnpb_q8)
    release_pso(g_pso_attnpb_tq4)
    release_pso(g_pso_attncombb)
    release_pso(g_pso_attnd16)
    release_pso(g_pso_attnd32)
    release_pso(g_pso_attnd_q8)
    release_pso(g_pso_attnd_tq4)
    release_pso(g_pso_attncombd)
    release_pso(g_pso_rpstb16)
    release_pso(g_pso_rpstb32)
    release_pso(g_pso_rpstb_q8)
    release_pso(g_pso_rpstb_tq4)
    release_pso(g_pso_addrmsb)
}
