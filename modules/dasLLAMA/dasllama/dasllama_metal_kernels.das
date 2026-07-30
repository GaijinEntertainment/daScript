options gen2
options _comment_hygiene = true

module dasllama_metal_kernels shared public

require math
require daslib/math_bits   // uint_bits_to_float in the mx4 MoE GEMV (msl_emit as_type)
require dasllama/dasllama_env  // env_int: the shared typed readers + the knob registry
require metal/msl_shader
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_lens

// The family-agnostic dasMetal kernel + PSO registry (the format-matrix arc's W1 split): every
// [metal_kernel] class the resident decode drivers dispatch — GEMV/GEMM/mul_mm forms, the
// single-query attention family across all four KV codecs (f16/f32/q8_0/tq4), the fused
// rope+KV-store family, add+rms/ew — plus their enc_* dispatch helpers and the PSO lifecycle
// (metal_decode_init / metal_kernels_release). The rms/add/swiglu/mul_mm MSL sources are the
// prefill module's, verbatim (own PSOs, zero copy drift). Kernel-shape rationale lives with
// each class; the driver contracts live in dasllama_metal_decode's header.

// ===== the new kernels =====

// One threadgroup per (stream, head), head_size threads — lane owns one V column. Pass 1 scores
// the context simdgroup-cooperatively so every K load coalesces (per-lane-per-row form gathered
// cache lines 2KB apart — measured 27us/layer at depth 24). Pass 2 softmax, pass 3 V-accumulate.
[metal_dispatch(name = "enc_attn16_c", pso = "g_pso_attn16", tgmem = "metal_sq_attn16_msl_tgmem", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnF16 {
    @ssbo @binding = 0 @off = "koff" kc : array<float16>  // this layer's K mirror slab (bound at the layer's byte offset)
    @ssbo @binding = 1 @off = "voff" vc : array<float16>
    @ssbo @binding = 2 q : array<float>     // [B x qd] roped queries
    @ssbo @binding = 3 yo : array<float>    // [B x qd] attention out
    @uniform @binding = 4 cnt : uint        // context depth (pos + 1)
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 hs : uint         // head_size == threads per group
    @uniform @binding = 9 scale : float
    @uniform @binding = 10 softcap : float  // > 0 = attention-logit soft cap (gemma2)
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup scores : float[128]  // >= ATTN_CHUNK_ROWS — the single-tg depth ceiling
    @workgroup red : float[16]

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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * float(kc[kb + 4u])
            }
            if (epl > 5u) {
                s += q5 * float(kc[kb + 5u])
            }
            if (epl > 6u) {
                s += q6 * float(kc[kb + 6u])
            }
            if (epl > 7u) {
                s += q7 * float(kc[kb + 7u])
            }
            if (epl > 8u) {
                s += q8 * float(kc[kb + 8u])
                s += q9 * float(kc[kb + 9u])
                s += q10 * float(kc[kb + 10u])
                s += q11 * float(kc[kb + 11u])
            }
            if (epl > 12u) {
                s += q12 * float(kc[kb + 12u])
                s += q13 * float(kc[kb + 13u])
                s += q14 * float(kc[kb + 14u])
                s += q15 * float(kc[kb + 15u])
            }
            s = simd_sum(s)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
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
        if (hass != 0u) {
            mm = max(mm, sink[h])
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
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
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
[metal_dispatch(name = "enc_attn32_c", pso = "g_pso_attn32", tgmem = "metal_sq_attn32_msl_tgmem", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnF32 {
    @ssbo @binding = 0 @off = "koff" kc : array<float>
    @ssbo @binding = 1 @off = "voff" vc : array<float>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 yo : array<float>
    @uniform @binding = 4 cnt : uint
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 hs : uint
    @uniform @binding = 9 scale : float
    @uniform @binding = 10 softcap : float
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup scores : float[128]
    @workgroup red : float[16]

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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * kc[kb + 4u]
            }
            if (epl > 5u) {
                s += q5 * kc[kb + 5u]
            }
            if (epl > 6u) {
                s += q6 * kc[kb + 6u]
            }
            if (epl > 7u) {
                s += q7 * kc[kb + 7u]
            }
            if (epl > 8u) {
                s += q8 * kc[kb + 8u]
                s += q9 * kc[kb + 9u]
                s += q10 * kc[kb + 10u]
                s += q11 * kc[kb + 11u]
            }
            if (epl > 12u) {
                s += q12 * kc[kb + 12u]
                s += q13 * kc[kb + 13u]
                s += q14 * kc[kb + 14u]
                s += q15 * kc[kb + 15u]
            }
            s = simd_sum(s)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
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
        if (hass != 0u) {
            mm = max(mm, sink[h])
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
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
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
[metal_dispatch(name = "enc_attn_q8_c", pso = "g_pso_attn_q8", tgmem = "metal_sq_attn_q8_msl_tgmem", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnQ8 {
    @ssbo @binding = 0 @off = "koff" ksh : array<float16> // K mirror slab, HALF-SCALE view (bound at the layer's byte offset)
    @ssbo @binding = 1 @role = "alias" @off = "koff" kqb : array<int8>    // the SAME K slab, BYTE view
    @ssbo @binding = 2 @off = "voff" vsh : array<float16>
    @ssbo @binding = 3 @role = "alias" @off = "voff" vqb : array<int8>
    @ssbo @binding = 4 q : array<float>     // [B x qd] roped queries
    @ssbo @binding = 5 yo : array<float>    // [B x qd] attention out
    @uniform @binding = 6 cnt : uint        // context depth (pos + 1)
    @uniform @binding = 7 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 hs : uint        // head_size == threads per group
    @uniform @binding = 11 scale : float
    @uniform @binding = 12 softcap : float
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup scores : float[128]  // >= ATTN_CHUNK_ROWS — the single-tg depth ceiling
    @workgroup red : float[16]

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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * float(kqb[kb + 4u])
            }
            if (epl > 5u) {
                s += q5 * float(kqb[kb + 5u])
            }
            if (epl > 6u) {
                s += q6 * float(kqb[kb + 6u])
            }
            if (epl > 7u) {
                s += q7 * float(kqb[kb + 7u])
            }
            if (epl > 8u) {
                s += q8 * float(kqb[kb + 8u])
                s += q9 * float(kqb[kb + 9u])
                s += q10 * float(kqb[kb + 10u])
                s += q11 * float(kqb[kb + 11u])
            }
            if (epl > 12u) {
                s += q12 * float(kqb[kb + 12u])
                s += q13 * float(kqb[kb + 13u])
                s += q14 * float(kqb[kb + 14u])
                s += q15 * float(kqb[kb + 15u])
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
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
        if (hass != 0u) {
            mm = max(mm, sink[h])
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
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
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
[metal_dispatch(name = "enc_attn_tq4_c", pso = "g_pso_attn_tq4", tgmem = "metal_sq_attn_tq4_msl_tgmem", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnTq4 {
    @ssbo @binding = 0 @off = "koff" ksh : array<float16> // K mirror slab, HALF-SCALE view (bound at the layer's byte offset)
    @ssbo @binding = 1 @role = "alias" @off = "koff" kqb : array<uint8>   // the SAME K slab, BYTE view
    @ssbo @binding = 2 @off = "voff" vsh : array<float16>
    @ssbo @binding = 3 @role = "alias" @off = "voff" vqb : array<uint8>
    @ssbo @binding = 4 q : array<float>     // [B x qd] roped + rotated queries
    @ssbo @binding = 5 yo : array<float>    // [B x qd] attention out (rotated basis)
    @uniform @binding = 6 cnt : uint
    @uniform @binding = 7 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 hs : uint
    @uniform @binding = 11 scale : float
    @uniform @binding = 12 softcap : float
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup scores : float[128]
    @workgroup red : float[16]

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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
                if (epl > 4u) {
                    let by2 = uint(kqb[kb + 2u])
                    s += q4 * float(int(by2 & 15u) - 8)
                    s += q5 * float(int(by2 >> 4u) - 8)
                }
                if (epl > 6u) {
                    let by3 = uint(kqb[kb + 3u])
                    s += q6 * float(int(by3 & 15u) - 8)
                    s += q7 * float(int(by3 >> 4u) - 8)
                }
                if (epl > 8u) {
                    let by4 = uint(kqb[kb + 4u])
                    let by5 = uint(kqb[kb + 5u])
                    s += q8 * float(int(by4 & 15u) - 8)
                    s += q9 * float(int(by4 >> 4u) - 8)
                    s += q10 * float(int(by5 & 15u) - 8)
                    s += q11 * float(int(by5 >> 4u) - 8)
                }
                if (epl > 12u) {
                    let by6 = uint(kqb[kb + 6u])
                    let by7 = uint(kqb[kb + 7u])
                    s += q12 * float(int(by6 & 15u) - 8)
                    s += q13 * float(int(by6 >> 4u) - 8)
                    s += q14 * float(int(by7 & 15u) - 8)
                    s += q15 * float(int(by7 >> 4u) - 8)
                }
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
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
        if (hass != 0u) {
            mm = max(mm, sink[h])
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
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
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
[metal_dispatch(name = "enc_attnp16_c", pso = "g_pso_attnp16", tgmem = "metal_sq_attn_part16_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalSqAttnPartF16 {
    @ssbo @binding = 0 @off = "koff" kc : array<float16>
    @ssbo @binding = 1 @off = "voff" vc : array<float16>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 part : array<float>  // [B x nheads x nchunks x (hs+2)]: acc[hs], chunk max, chunk expsum
    @uniform @binding = 4 cnt : uint
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 hs : uint
    @uniform @binding = 9 scale : float
    @uniform @binding = 10 nchunks : uint
    @uniform @binding = 11 softcap : float  // > 0 = attention-logit soft cap (gemma2)
    @uniform @binding = 12 window : uint    // > 0 = sliding span: rows < cnt - window are masked
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]

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
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        if (c1 <= wstart) {
            return
        }
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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * float(kc[kb + 4u])
            }
            if (epl > 5u) {
                s += q5 * float(kc[kb + 5u])
            }
            if (epl > 6u) {
                s += q6 * float(kc[kb + 6u])
            }
            if (epl > 7u) {
                s += q7 * float(kc[kb + 7u])
            }
            if (epl > 8u) {
                s += q8 * float(kc[kb + 8u])
                s += q9 * float(kc[kb + 9u])
                s += q10 * float(kc[kb + 10u])
                s += q11 * float(kc[kb + 11u])
            }
            if (epl > 12u) {
                s += q12 * float(kc[kb + 12u])
                s += q13 * float(kc[kb + 13u])
                s += q14 * float(kc[kb + 14u])
                s += q15 * float(kc[kb + 15u])
            }
            s = simd_sum(s)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
            }
            j += gl_NumSubgroups
        }
        barrier()
        // pass 2a: chunk max
        if (wstart > c0) {
            // boundary chunk: mask rows below the window start (uniform branch — barrier is safe)
            var jm = lid
            while (jm < nrows) {
                if (c0 + jm < wstart) {
                    scores[jm] = -3.0e38
                }
                jm += nt
            }
            barrier()
        }
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
[metal_dispatch(name = "enc_attnp32_c", pso = "g_pso_attnp32", tgmem = "metal_sq_attn_part32_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalSqAttnPartF32 {
    @ssbo @binding = 0 @off = "koff" kc : array<float>
    @ssbo @binding = 1 @off = "voff" vc : array<float>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 part : array<float>
    @uniform @binding = 4 cnt : uint
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 hs : uint
    @uniform @binding = 9 scale : float
    @uniform @binding = 10 nchunks : uint
    @uniform @binding = 11 softcap : float
    @uniform @binding = 12 window : uint
    @workgroup scores : float[64]
    @workgroup red : float[16]

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
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        if (c1 <= wstart) {
            return
        }
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * kc[kb + 4u]
            }
            if (epl > 5u) {
                s += q5 * kc[kb + 5u]
            }
            if (epl > 6u) {
                s += q6 * kc[kb + 6u]
            }
            if (epl > 7u) {
                s += q7 * kc[kb + 7u]
            }
            if (epl > 8u) {
                s += q8 * kc[kb + 8u]
                s += q9 * kc[kb + 9u]
                s += q10 * kc[kb + 10u]
                s += q11 * kc[kb + 11u]
            }
            if (epl > 12u) {
                s += q12 * kc[kb + 12u]
                s += q13 * kc[kb + 13u]
                s += q14 * kc[kb + 14u]
                s += q15 * kc[kb + 15u]
            }
            s = simd_sum(s)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
            }
            j += gl_NumSubgroups
        }
        barrier()
        if (wstart > c0) {
            // boundary chunk: mask rows below the window start (uniform branch — barrier is safe)
            var jm = lid
            while (jm < nrows) {
                if (c0 + jm < wstart) {
                    scores[jm] = -3.0e38
                }
                jm += nt
            }
            barrier()
        }
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
[metal_dispatch(name = "enc_attnp_q8_c", pso = "g_pso_attnp_q8", tgmem = "metal_sq_attn_part_q8_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalSqAttnPartQ8 {
    @ssbo @binding = 0 @off = "koff" ksh : array<float16>
    @ssbo @binding = 1 @role = "alias" @off = "koff" kqb : array<int8>
    @ssbo @binding = 2 @off = "voff" vsh : array<float16>
    @ssbo @binding = 3 @role = "alias" @off = "voff" vqb : array<int8>
    @ssbo @binding = 4 q : array<float>
    @ssbo @binding = 5 part : array<float>  // [B x nheads x nchunks x (hs+2)]: acc[hs], chunk max, chunk expsum
    @uniform @binding = 6 cnt : uint
    @uniform @binding = 7 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 hs : uint
    @uniform @binding = 11 scale : float
    @uniform @binding = 12 nchunks : uint
    @uniform @binding = 13 softcap : float
    @uniform @binding = 14 window : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]

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
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        if (c1 <= wstart) {
            return
        }
        // pass 1: simdgroup-cooperative score fill over this chunk's rows
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * float(kqb[kb + 4u])
            }
            if (epl > 5u) {
                s += q5 * float(kqb[kb + 5u])
            }
            if (epl > 6u) {
                s += q6 * float(kqb[kb + 6u])
            }
            if (epl > 7u) {
                s += q7 * float(kqb[kb + 7u])
            }
            if (epl > 8u) {
                s += q8 * float(kqb[kb + 8u])
                s += q9 * float(kqb[kb + 9u])
                s += q10 * float(kqb[kb + 10u])
                s += q11 * float(kqb[kb + 11u])
            }
            if (epl > 12u) {
                s += q12 * float(kqb[kb + 12u])
                s += q13 * float(kqb[kb + 13u])
                s += q14 * float(kqb[kb + 14u])
                s += q15 * float(kqb[kb + 15u])
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
            }
            j += gl_NumSubgroups
        }
        barrier()
        // pass 2a: chunk max
        if (wstart > c0) {
            // boundary chunk: mask rows below the window start (uniform branch — barrier is safe)
            var jm = lid
            while (jm < nrows) {
                if (c0 + jm < wstart) {
                    scores[jm] = -3.0e38
                }
                jm += nt
            }
            barrier()
        }
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
[metal_dispatch(name = "enc_attnp_tq4_c", pso = "g_pso_attnp_tq4", tgmem = "metal_sq_attn_part_tq4_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalSqAttnPartTq4 {
    @ssbo @binding = 0 @off = "koff" ksh : array<float16>
    @ssbo @binding = 1 @role = "alias" @off = "koff" kqb : array<uint8>
    @ssbo @binding = 2 @off = "voff" vsh : array<float16>
    @ssbo @binding = 3 @role = "alias" @off = "voff" vqb : array<uint8>
    @ssbo @binding = 4 q : array<float>
    @ssbo @binding = 5 part : array<float>  // [B x nheads x nchunks x (hs+2)]
    @uniform @binding = 6 cnt : uint
    @uniform @binding = 7 nblk : uint
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 hs : uint
    @uniform @binding = 11 scale : float
    @uniform @binding = 12 nchunks : uint
    @uniform @binding = 13 softcap : float
    @uniform @binding = 14 window : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]

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
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        if (c1 <= wstart) {
            return
        }
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
                if (epl > 4u) {
                    let by2 = uint(kqb[kb + 2u])
                    s += q4 * float(int(by2 & 15u) - 8)
                    s += q5 * float(int(by2 >> 4u) - 8)
                }
                if (epl > 6u) {
                    let by3 = uint(kqb[kb + 3u])
                    s += q6 * float(int(by3 & 15u) - 8)
                    s += q7 * float(int(by3 >> 4u) - 8)
                }
                if (epl > 8u) {
                    let by4 = uint(kqb[kb + 4u])
                    let by5 = uint(kqb[kb + 5u])
                    s += q8 * float(int(by4 & 15u) - 8)
                    s += q9 * float(int(by4 >> 4u) - 8)
                    s += q10 * float(int(by5 & 15u) - 8)
                    s += q11 * float(int(by5 >> 4u) - 8)
                }
                if (epl > 12u) {
                    let by6 = uint(kqb[kb + 6u])
                    let by7 = uint(kqb[kb + 7u])
                    s += q12 * float(int(by6 & 15u) - 8)
                    s += q13 * float(int(by6 >> 4u) - 8)
                    s += q14 * float(int(by7 & 15u) - 8)
                    s += q15 * float(int(by7 >> 4u) - 8)
                }
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
            }
            j += gl_NumSubgroups
        }
        barrier()
        if (wstart > c0) {
            // boundary chunk: mask rows below the window start (uniform branch — barrier is safe)
            var jm = lid
            while (jm < nrows) {
                if (c0 + jm < wstart) {
                    scores[jm] = -3.0e38
                }
                jm += nt
            }
            barrier()
        }
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
[metal_dispatch(name = "enc_attncomb_c", pso = "g_pso_attncomb", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnComb {
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 yo : array<float>
    @uniform @binding = 2 nchunks : uint
    @uniform @binding = 3 nheads : uint
    @uniform @binding = 4 hs : uint
    @uniform @binding = 5 chlo : uint       // first live chunk (sliding: chunks below the window are unwritten)
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint

    [metal_kernel(name="metal_sq_attn_comb_msl")]
    def metal_sq_attn_comb {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let base = (bb * nheads + h) * nchunks
        var mm = -3.0e38
        var ci = chlo
        while (ci < nchunks) {
            mm = max(mm, part[(base + ci) * (hs + 2u) + hs])
            ci++
        }
        if (hass != 0u) {
            mm = max(mm, sink[h])
        }
        var acc = 0.0
        var l = 0.0
        ci = chlo
        while (ci < nchunks) {
            let pb = (base + ci) * (hs + 2u)
            let w = exp(part[pb + hs] - mm)
            acc += part[pb + lid] * w
            l += part[pb + hs + 1u] * w
            ci++
        }
        if (hass != 0u) {
            l += exp(max(sink[h] - mm, -80.0))
        }
        yo[bb * (nheads * hs) + h * hs + lid] = acc / l
    }
}

// The 2-rows-per-simdgroup fused QKV GEMV + rope + KV-row store: the rope pair's two elements
// of q/k ARE two weight rows of the fused region — adjacent (2p, 2p+1) under NORM, (r, r+hs/2)
// within the head under NEOX. Deletes the separate rope_store dispatch entirely.
[metal_dispatch(name = "enc_qkvrs16_c", pso = "g_pso_qkvrs16", tg = 128, grid = "pairs/4", params = "pairs : int64")]
class MetalQ8GemvQkvRsF16 {
    @ssbo @binding = 0 @role = "weight" @off = "wqoff" wqbq : array<int8>   // wq blob, byte view @ the q tensor's byte offset
    @ssbo @binding = 1 @role = "weight" @off = "wqoff" wshq : array<float16>   // ... half-scale view (same offset). Segments bind
                                            // separately — kind-major layout keeps q/k/v apart
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 qo : array<float>    // q out, roped ([0, qd) of the QKV buffer)
    @ssbo @binding = 4 @off = "koff" kd : array<float16>  // mirror K row (bound at the row offset)
    @ssbo @binding = 5 @off = "voff" vd : array<float16>
    @ssbo @binding = 6 @role = "weight" tcos : array<float>  // [hs/2] this position's rope row
    @ssbo @binding = 7 @role = "weight" tsin : array<float>
    @uniform @binding = 8 ndim : uint
    @uniform @binding = 9 ddim : uint       // qd + 2*kvd
    @uniform @binding = 10 qd : uint
    @uniform @binding = 11 kvd : uint
    @uniform @binding = 12 hs : uint
    @uniform @binding = 13 neox : uint      // rope pair = rows (r, r+hs/2) instead of (2p, 2p+1)
    @ssbo @binding = 14 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 15 has_bias : uint
    @ssbo @binding = 16 @role = "weight" @off = "wkoff" wqbk : array<int8>  // wk blob views
    @ssbo @binding = 17 @role = "weight" @off = "wkoff" wshk : array<float16>
    @ssbo @binding = 18 @role = "weight" @off = "wvoff" wqbv : array<int8>  // wv blob views
    @ssbo @binding = 19 @role = "weight" @off = "wvoff" wshv : array<float16>

    [metal_kernel(name="metal_q8_gemv_qkv_rs16_msl")]
    def metal_q8_gemv_qkv_rs16 {
        let pr = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (pr * 2u >= ddim) {
            return
        }
        // the simdgroup's two rows ARE the rope pair: adjacent under NORM; (r, r+half) within
        // the row's head under NEOX for the q/k ranges (v rows stay adjacent — no rope)
        let half = hs / 2u
        var row = pr * 2u
        var row2 = row + 1u
        if (neox != 0u && row < qd + kvd) {
            let lp = row < qd ? pr : pr - qd / 2u
            let seg = row < qd ? 0u : qd
            row = seg + (lp / half) * hs + lp % half
            row2 = row + half
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var acc0 = 0.0
        var acc1 = 0.0
        var k = lane
        // both pair rows sit in ONE segment (qd/kvd even, NEOX pairs stay in-head) — dot them
        // off that segment's blob views with segment-LOCAL rows
        if (row < qd) {
            while (k < nb4) {
                let x4 = x[k]
                let co = (k % 8u) * 4u
                let qb0 = (row * nb + k / 8u) * 34u + 2u + co
                let qb1 = (row2 * nb + k / 8u) * 34u + 2u + co
                let w0 = float4(float(wqbq[qb0]), float(wqbq[qb0 + 1u]), float(wqbq[qb0 + 2u]), float(wqbq[qb0 + 3u]))
                let w1 = float4(float(wqbq[qb1]), float(wqbq[qb1 + 1u]), float(wqbq[qb1 + 2u]), float(wqbq[qb1 + 3u]))
                acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(wshq[(row * nb + k / 8u) * 17u])
                acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(wshq[(row2 * nb + k / 8u) * 17u])
                k += gl_SubgroupSize
            }
        } elif (row < qd + kvd) {
            let lr = row - qd
            let lr2 = row2 - qd
            while (k < nb4) {
                let x4 = x[k]
                let co = (k % 8u) * 4u
                let qb0 = (lr * nb + k / 8u) * 34u + 2u + co
                let qb1 = (lr2 * nb + k / 8u) * 34u + 2u + co
                let w0 = float4(float(wqbk[qb0]), float(wqbk[qb0 + 1u]), float(wqbk[qb0 + 2u]), float(wqbk[qb0 + 3u]))
                let w1 = float4(float(wqbk[qb1]), float(wqbk[qb1 + 1u]), float(wqbk[qb1 + 2u]), float(wqbk[qb1 + 3u]))
                acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(wshk[(lr * nb + k / 8u) * 17u])
                acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(wshk[(lr2 * nb + k / 8u) * 17u])
                k += gl_SubgroupSize
            }
        } else {
            let lr = row - qd - kvd
            let lr2 = row2 - qd - kvd
            while (k < nb4) {
                let x4 = x[k]
                let co = (k % 8u) * 4u
                let qb0 = (lr * nb + k / 8u) * 34u + 2u + co
                let qb1 = (lr2 * nb + k / 8u) * 34u + 2u + co
                let w0 = float4(float(wqbv[qb0]), float(wqbv[qb0 + 1u]), float(wqbv[qb0 + 2u]), float(wqbv[qb0 + 3u]))
                let w1 = float4(float(wqbv[qb1]), float(wqbv[qb1 + 1u]), float(wqbv[qb1 + 2u]), float(wqbv[qb1 + 3u]))
                acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(wshv[(lr * nb + k / 8u) * 17u])
                acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(wshv[(lr2 * nb + k / 8u) * 17u])
                k += gl_SubgroupSize
            }
        }
        acc0 = simd_sum(acc0)
        acc1 = simd_sum(acc1)
        if (lane != 0u) {
            return
        }
        if (has_bias != 0u) {
            acc0 += bias[row]
            acc1 += bias[row2]
        }
        if (row < qd) {
            let j = neox != 0u ? row % hs : (row % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            qo[row] = acc0 * fcr - acc1 * fci
            qo[row2] = acc0 * fci + acc1 * fcr
        } elif (row < qd + kvd) {
            let ik = row - qd
            let ik2 = row2 - qd
            let j = neox != 0u ? ik % hs : (ik % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            kd[ik] = float16(min(max(acc0 * fcr - acc1 * fci, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
            kd[ik2] = float16(min(max(acc0 * fci + acc1 * fcr, -65504.0), 65504.0))   // nolint:STYLE019
        } else {
            let iv = row - qd - kvd
            vd[iv] = float16(min(max(acc0, -65504.0), 65504.0))   // nolint:STYLE019
            vd[iv + 1u] = float16(min(max(acc1, -65504.0), 65504.0))   // nolint:STYLE019
        }
    }
}

// f32-mirror twin — straight stores, no clamp.
[metal_dispatch(name = "enc_qkvrs32_c", pso = "g_pso_qkvrs32", tg = 128, grid = "pairs/4", params = "pairs : int64")]
class MetalQ8GemvQkvRsF32 {
    @ssbo @binding = 0 @role = "weight" @off = "wqoff" wqbq : array<int8>   // per-segment blob views — the F16 twin's scheme
    @ssbo @binding = 1 @role = "weight" @off = "wqoff" wshq : array<float16>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 qo : array<float>
    @ssbo @binding = 4 @off = "koff" kd : array<float>
    @ssbo @binding = 5 @off = "voff" vd : array<float>
    @ssbo @binding = 6 @role = "weight" tcos : array<float>
    @ssbo @binding = 7 @role = "weight" tsin : array<float>
    @uniform @binding = 8 ndim : uint
    @uniform @binding = 9 ddim : uint
    @uniform @binding = 10 qd : uint
    @uniform @binding = 11 kvd : uint
    @uniform @binding = 12 hs : uint
    @uniform @binding = 13 neox : uint
    @ssbo @binding = 14 @role = "weight" bias : array<float>
    @uniform @binding = 15 has_bias : uint
    @ssbo @binding = 16 @role = "weight" @off = "wkoff" wqbk : array<int8>
    @ssbo @binding = 17 @role = "weight" @off = "wkoff" wshk : array<float16>
    @ssbo @binding = 18 @role = "weight" @off = "wvoff" wqbv : array<int8>
    @ssbo @binding = 19 @role = "weight" @off = "wvoff" wshv : array<float16>

    [metal_kernel(name="metal_q8_gemv_qkv_rs32_msl")]
    def metal_q8_gemv_qkv_rs32 {
        let pr = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (pr * 2u >= ddim) {
            return
        }
        let half = hs / 2u
        var row = pr * 2u
        var row2 = row + 1u
        if (neox != 0u && row < qd + kvd) {
            let lp = row < qd ? pr : pr - qd / 2u
            let seg = row < qd ? 0u : qd
            row = seg + (lp / half) * hs + lp % half
            row2 = row + half
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var acc0 = 0.0
        var acc1 = 0.0
        var k = lane
        if (row < qd) {
            while (k < nb4) {
                let x4 = x[k]
                let co = (k % 8u) * 4u
                let qb0 = (row * nb + k / 8u) * 34u + 2u + co
                let qb1 = (row2 * nb + k / 8u) * 34u + 2u + co
                let w0 = float4(float(wqbq[qb0]), float(wqbq[qb0 + 1u]), float(wqbq[qb0 + 2u]), float(wqbq[qb0 + 3u]))
                let w1 = float4(float(wqbq[qb1]), float(wqbq[qb1 + 1u]), float(wqbq[qb1 + 2u]), float(wqbq[qb1 + 3u]))
                acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(wshq[(row * nb + k / 8u) * 17u])
                acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(wshq[(row2 * nb + k / 8u) * 17u])
                k += gl_SubgroupSize
            }
        } elif (row < qd + kvd) {
            let lr = row - qd
            let lr2 = row2 - qd
            while (k < nb4) {
                let x4 = x[k]
                let co = (k % 8u) * 4u
                let qb0 = (lr * nb + k / 8u) * 34u + 2u + co
                let qb1 = (lr2 * nb + k / 8u) * 34u + 2u + co
                let w0 = float4(float(wqbk[qb0]), float(wqbk[qb0 + 1u]), float(wqbk[qb0 + 2u]), float(wqbk[qb0 + 3u]))
                let w1 = float4(float(wqbk[qb1]), float(wqbk[qb1 + 1u]), float(wqbk[qb1 + 2u]), float(wqbk[qb1 + 3u]))
                acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(wshk[(lr * nb + k / 8u) * 17u])
                acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(wshk[(lr2 * nb + k / 8u) * 17u])
                k += gl_SubgroupSize
            }
        } else {
            let lr = row - qd - kvd
            let lr2 = row2 - qd - kvd
            while (k < nb4) {
                let x4 = x[k]
                let co = (k % 8u) * 4u
                let qb0 = (lr * nb + k / 8u) * 34u + 2u + co
                let qb1 = (lr2 * nb + k / 8u) * 34u + 2u + co
                let w0 = float4(float(wqbv[qb0]), float(wqbv[qb0 + 1u]), float(wqbv[qb0 + 2u]), float(wqbv[qb0 + 3u]))
                let w1 = float4(float(wqbv[qb1]), float(wqbv[qb1 + 1u]), float(wqbv[qb1 + 2u]), float(wqbv[qb1 + 3u]))
                acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(wshv[(lr * nb + k / 8u) * 17u])
                acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(wshv[(lr2 * nb + k / 8u) * 17u])
                k += gl_SubgroupSize
            }
        }
        acc0 = simd_sum(acc0)
        acc1 = simd_sum(acc1)
        if (lane != 0u) {
            return
        }
        if (has_bias != 0u) {
            acc0 += bias[row]
            acc1 += bias[row2]
        }
        if (row < qd) {
            let j = neox != 0u ? row % hs : (row % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            qo[row] = acc0 * fcr - acc1 * fci
            qo[row2] = acc0 * fci + acc1 * fcr
        } elif (row < qd + kvd) {
            let ik = row - qd
            let ik2 = row2 - qd
            let j = neox != 0u ? ik % hs : (ik % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            kd[ik] = acc0 * fcr - acc1 * fci
            kd[ik2] = acc0 * fci + acc1 * fcr
        } else {
            let iv = row - qd - kvd
            vd[iv] = acc0
            vd[iv + 1u] = acc1
        }
    }
}

// The W1|W3 twin of the same shape: one simdgroup dots BOTH ffn rows of an output element (gate
// off the w1 blob views, up off the w3 views — kind-major layout keeps them apart) and writes
// act(g)·u directly (act = silu or gelu per the uniform). Deletes the swiglu/geglu dispatch AND its serialization level.
[metal_dispatch(name = "enc_w13sw_c", pso = "g_pso_w13sw", tg = 128, grid = "hidden/4", params = "hidden : int64")]
class MetalQ8GemvW13Sw {
    @ssbo @binding = 0 @role = "weight" @off = "w1off" wqb1 : array<int8>   // w1 blob, byte view @ its tensor offset
    @ssbo @binding = 1 @role = "weight" @off = "w1off" wsh1 : array<float16>   // ... half-scale view
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 @span = "hidden*4" ho : array<float>    // [hidden] silu(gate)·up
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 hidden : uint
    @ssbo @binding = 6 @role = "weight" @off = "w3off" wqb3 : array<int8>   // w3 blob views
    @ssbo @binding = 7 @role = "weight" @off = "w3off" wsh3 : array<float16>
    @uniform @binding = 8 @role = "weight" act : uint        // gate activation: 0 = silu (swiglu), 1 = gelu (geglu, gemma line)

    [metal_kernel(name="metal_q8_gemv_w13sw16_msl")]
    def metal_q8_gemv_w13sw16 {
        let r = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (r >= hidden) {
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
            let bi = r * nb + k / 8u
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let w0 = float4(float(wqb1[qb]), float(wqb1[qb + 1u]), float(wqb1[qb + 2u]), float(wqb1[qb + 3u]))
            let w1 = float4(float(wqb3[qb]), float(wqb3[qb + 1u]), float(wqb3[qb + 2u]), float(wqb3[qb + 3u]))
            acc0 += (w0.x * x4.x + w0.y * x4.y + w0.z * x4.z + w0.w * x4.w) * float(wsh1[bi * 17u])
            acc1 += (w1.x * x4.x + w1.y * x4.y + w1.z * x4.z + w1.w * x4.w) * float(wsh3[bi * 17u])
            k += gl_SubgroupSize
        }
        acc0 = simd_sum(acc0)
        acc1 = simd_sum(acc1)
        if (lane == 0u) {
            if (act == 0u) {
                ho[r] = (acc0 / (1.0 + exp(-acc0))) * acc1
            } else {
                // geglu: gelu(gate)·up — the CPU geglu4 / MetalGeglu tanh-via-exp identity (exp saturates, no NaN)
                let ga = 0.7978845608028654 * (acc0 + 0.044715 * acc0 * acc0 * acc0)
                ho[r] = 0.5 * acc0 * (2.0 - 2.0 / (1.0 + exp(2.0 * ga))) * acc1
            }
        }
    }
}

// Row copy for the zero-copy logits rail: one batched-classifier staging row into a session's
// bytesNoCopy-wrapped logits storage — the scatter rides the cb instead of the landing memcpy.
// src binds at the row's byte offset; grid = ceil(n/256) x 256-tg.
[metal_dispatch(name = "enc_copy_row", pso = "g_pso_copy_row", tg = 256, grid = "n/256", params = "n : int64")]
class MetalCopyRow {
    @ssbo @binding = 0 @off = "soff" @span = "n*4" src : array<float>
    @ssbo @binding = 1 @off = "doff" @span = "n*4" dst : array<float>
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
[metal_dispatch(name = "enc_argmax", pso = "g_pso_argmax", tgmem = "metal_argmax_msl_tgmem", tg = 1024, grid = "1")]
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

// Speculative embed gather: dequant the argmax winner's Q8 embedding row (the cls_q8 linear
// copy's slice of the whole-model 34B blob, bound at emb_q8_off's byte offset) into the next
// step's residual input — the GPU chain's embed_row.
[metal_dispatch(name = "enc_embed_c", pso = "g_pso_emb", tg = 64, grid = "dim/64", params = "dim : int64")]
class MetalEmbedQ8 {
    @ssbo @binding = 0 @role = "weight" @off = "eoff" q : array<int8>      // the embed blob slice, byte view (quants at 34*b + 2)
    @ssbo @binding = 1 @role = "weight" @off = "eoff" sc : array<float16>  // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 2 tok : array<uint>
    @ssbo @binding = 3 xo : array<float>    // [dim] residual input out
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 @role = "weight" escale : float    // embed_scale (gemma: sqrt(dim); 1.0 elsewhere — g_one or an uploaded scalar)

    [metal_kernel(name="metal_embed_q8_msl")]
    def metal_embed_q8 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= dim) {
            return
        }
        let e = tok[0] * dim + gid
        let b = e / 32u
        xo[gid] = (float(q[b * 34u + 2u + e % 32u]) * float(sc[b * 17u])) * escale
    }
}

// k6 twin (tied Q6_K classifiers — the spec chain for kq): dequant the winner's row from the
// disk-order k6 planes, dequant_k6_plane_superblock's exact math — (d * sc16) * (q - 32), the
// same split scale layout as MetalKqGemvK6 (16B sub-scale blocks @0, f16 d plane @nsb*16).
[metal_dispatch(name = "enc_embed_k6_c", pso = "g_pso_emb_k6", tg = 64, grid = "dim/64", params = "dim : int64")]
class MetalEmbedK6 {
    @ssbo @binding = 0 @role = "weight" @off = "doff" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k6 quant plane, byte view (ql at 192*blk, qh at +128)
    @ssbo @binding = 3 tok : array<uint>
    @ssbo @binding = 4 xo : array<float>    // [dim] residual input out
    @uniform @binding = 5 dim : uint
    @uniform @binding = 6 @role = "weight" escale : float    // embed_scale (gemma: sqrt(dim); 1.0 elsewhere — g_one or an uploaded scalar)

    [metal_kernel(name="metal_embed_k6_msl")]
    def metal_embed_k6 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= dim) {
            return
        }
        let nb = dim / 256u
        let ib = gid / 256u
        let blk = tok[0] * nb + ib
        let p = gid % 256u
        let ip = p / 128u
        let rr = p % 128u
        let quarter = rr / 32u
        let l = rr % 32u
        let qlb = uint(kqb[blk * 192u + ip * 64u + l + ((quarter & 1u) != 0u ? 32u : 0u)])
        let h = uint(kqb[blk * 192u + 128u + ip * 32u + l])
        var qv = 0u
        if (quarter == 0u) {
            qv = (qlb & 0x0F) | ((h & 0x03) << 4u)
        } elif (quarter == 1u) {
            qv = (qlb & 0x0F) | ((h & 0x0C) << 2u)
        } elif (quarter == 2u) {
            qv = (qlb >> 4u) | (h & 0x30)
        } else {
            qv = (qlb >> 4u) | ((h & 0xC0) >> 2u)
        }
        let sc = float((int(kscb[blk * 16u + ip * 8u + l / 16u + quarter * 2u]) ^ 128) - 128)
        xo[gid] = (float(kdh[blk]) * sc * (float(qv) - 32.0)) * escale
    }
}

// Fused residual-add + rmsnorm: x += a in place, then y = rms(x)·w, one dispatch instead of two.
// The added row stages through the threadgroup slab since barrier() is a threadgroup fence only.
// dim caps at the slab; wider models take the unfused pair (enc_add_rms_site).
[metal_dispatch(name = "enc_addrms_c", pso = "g_pso_addrms", tgmem = "metal_add_rms_msl_tgmem", tg = 1024, grid = "1")]
class MetalAddRms {
    @ssbo @binding = 0 x : array<float>     // [dim] residual stream, add target
    @ssbo @binding = 1 a : array<float>     // [dim] addend (fully read before y is written — y may alias a)
    @ssbo @binding = 2 @role = "weight" w : array<float>     // [dim] norm weight
    @ssbo @binding = 3 y : array<float>     // [dim] normed out
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 eps : float
    @uniform @binding = 6 @role = "weight" ascale : float    // (x + a) * ascale — gemma4 layer_out_scale scales
                                            // the whole layer output; every other site binds 1.0 (g_one)
    @workgroup xrow : float[4096]           // == ADD_RMS_MAX_DIM
    @workgroup partial : float[32]          // one per simdgroup at the 1024-thread dispatch
    @workgroup inv_sh : float

    [metal_kernel(name="metal_add_rms_msl")]
    def metal_add_rms {
        let lid = gl_LocalInvocationID.x
        var ss = 0.0
        var k = lid
        while (k < dim) {
            let vv = (x[k] + a[k]) * ascale
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

// Fused post-attn-norm + residual add + ffn-norm: a_n = rms(a)·wpa; x += a_n; y = rms(x)·wffn — one
// dispatch collapsing the pre_post_norm chain (enc_rms(a) + enc_add_rms_site) with two register
// reductions, no inter-dispatch barrier. x is staged for the trailing norm; same slab cap as MetalAddRms.
[metal_dispatch(name = "enc_preaddrms_c", pso = "g_pso_preaddrms", tgmem = "metal_pre_add_rms_msl_tgmem", tg = 1024, grid = "1")]
class MetalPreAddRms {
    @ssbo @binding = 0 x : array<float>     // [dim] residual stream, add target + written back
    @ssbo @binding = 1 a : array<float>     // [dim] attn out (post-wo); wpa norm applies inline during the add
    @ssbo @binding = 2 @role = "weight" wpa : array<float>   // [dim] post-attention norm weight
    @ssbo @binding = 3 @role = "weight" wffn : array<float>  // [dim] ffn norm weight
    @ssbo @binding = 4 y : array<float>     // [dim] ffn-normed out
    @uniform @binding = 5 dim : uint
    @uniform @binding = 6 eps : float
    @workgroup xrow : float[4096]           // == ADD_RMS_MAX_DIM — stages x for the trailing norm
    @workgroup partial : float[32]          // one per simdgroup at the 1024-thread dispatch
    @workgroup inv_sh : float

    [metal_kernel(name="metal_pre_add_rms_msl")]
    def metal_pre_add_rms {
        let lid = gl_LocalInvocationID.x
        // reduction 1: mean(a²) → inv_a
        var ssa = 0.0
        var k = lid
        while (k < dim) {
            let av = a[k]
            ssa += av * av
            k += gl_WorkGroupSize.x
        }
        ssa = simd_sum(ssa)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ssa
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
        let inva = inv_sh
        // x += a·inva·wpa; stage x; reduction 2: mean(x²) → inv_x
        var ssx = 0.0
        k = lid
        while (k < dim) {
            let xv = x[k] + a[k] * inva * wpa[k]
            x[k] = xv
            xrow[k] = xv
            ssx += xv * xv
            k += gl_WorkGroupSize.x
        }
        ssx = simd_sum(ssx)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ssx
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
        let invx = inv_sh
        k = lid
        while (k < dim) {
            y[k] = wffn[k] * (xrow[k] * invx)
            k += gl_WorkGroupSize.x
        }
    }
}

// Fused rope + KV-row store, one thread per rotation pair (ntok == 1): q pairs rope in place; k
// pairs rope straight into the mirror K row (±65504 clamp) and the same thread copies its V pair.
// Replaces the rope_qk + kv_store dispatch pair.
[metal_dispatch(name = "enc_rpst16_c", pso = "g_pso_rpst16", tg = 64, grid = "npairs/64", params = "npairs : int64")]
class MetalRopeStoreF16 {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) roped in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 @off = "koff" kd : array<float16>  // mirror K row (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 @off = "voff" vd : array<float16>
    @ssbo @binding = 4 @role = "weight" tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 5 @role = "weight" tsin : array<float>
    @uniform @binding = 6 qd : uint
    @uniform @binding = 7 head_size : uint
    @uniform @binding = 8 npairs_q : uint   // qd/2 — the q half of the grid
    @uniform @binding = 9 npairs : uint     // + kvd/2 — grid guard
    @uniform @binding = 10 neox : uint      // pair (j, j+half) instead of (2j, 2j+1); same table row j
    @ssbo @binding = 11 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 12 has_bias : uint
    @uniform @binding = 13 rot : uint       // partial rotary width (qwen35: 64 of 256); == head_size for full

    [metal_kernel(name="metal_rope_store16_msl")]
    def metal_rope_store16 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let isK = gid >= npairs_q
        let lgid = isK ? gid - npairs_q : gid
        let half = head_size / 2u
        let halfr = rot / 2u
        let j = lgid % half
        let hbase = (lgid / half) * head_size
        // threads j < rot/2 rotate the (j, j+rot/2) pair; the rest identity-cover [rot, hs)
        let rotp = j < halfr
        let i = neox != 0u ? (rotp ? hbase + j : hbase + rot + (j - halfr)) : hbase + 2u * j
        let i2 = neox != 0u ? (rotp ? i + halfr : i + (half - halfr)) : i + 1u
        let fcr = rotp ? tcos[j] : 1.0
        let fci = rotp ? tsin[j] : 0.0
        let kvd = npairs * 2u - qd   // grid = (qd + kvd)/2 pairs
        if (isK) {
            let v0 = qk[qd + i] + (has_bias != 0u ? bias[qd + i] : 0.0)
            let v1 = qk[qd + i2] + (has_bias != 0u ? bias[qd + i2] : 0.0)
            let r0 = v0 * fcr - v1 * fci
            let r1 = v0 * fci + v1 * fcr
            kd[i] = float16(min(max(r0, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
            kd[i2] = float16(min(max(r1, -65504.0), 65504.0))   // nolint:STYLE019
            let b0 = has_bias != 0u ? bias[qd + kvd + i] : 0.0
            let b1 = has_bias != 0u ? bias[qd + kvd + i2] : 0.0
            vd[i] = float16(min(max(vs[i] + b0, -65504.0), 65504.0))   // nolint:STYLE019
            vd[i2] = float16(min(max(vs[i2] + b1, -65504.0), 65504.0))   // nolint:STYLE019
        } else {
            let v0 = qk[i] + (has_bias != 0u ? bias[i] : 0.0)
            let v1 = qk[i2] + (has_bias != 0u ? bias[i2] : 0.0)
            qk[i] = v0 * fcr - v1 * fci
            qk[i2] = v0 * fci + v1 * fcr
        }
    }
}

// The head-cooperative fused form (QK-norm models): ONE threadgroup per head folds bias ->
// per-head RMSNorm -> rope -> f16 store — the thread-per-pair kernels can't host the head-wide
// reduction. Grid: nheads q + nkvh k + nkvh v; rope pairs via the tgmem-staged POST-NORM partner.
[metal_dispatch(name = "enc_rpst_h16_c", pso = "g_pso_rpst_h16", tgmem = "metal_rope_store_h16_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalRopeStoreHF16 {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) roped in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 @off = "koff" kd : array<float16>  // mirror K row (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 @off = "voff" vd : array<float16>
    @ssbo @binding = 4 @role = "weight" tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 5 @role = "weight" tsin : array<float>
    @ssbo @binding = 6 @role = "weight" w : array<float>     // [qnorm | knorm] per-head RMS weights
    @ssbo @binding = 7 @role = "weight" bias : array<float>  // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 8 qd : uint
    @uniform @binding = 9 head_size : uint
    @uniform @binding = 10 nheads : uint
    @uniform @binding = 11 kv_mul : uint
    @uniform @binding = 12 neox : uint
    @uniform @binding = 13 has_bias : uint
    @uniform @binding = 14 has_norm : uint
    @uniform @binding = 15 eps : float
    @uniform @binding = 16 rot : uint       // partial rotary width; == head_size for full
    @workgroup buf : float[512]     // <= max head_size (gemma4 global heads are 512)
    @workgroup part : float[16]

    [metal_kernel(name="metal_rope_store_h16_msl")]
    def metal_rope_store_h16 {
        let tgid = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        let hs = head_size
        let nkvh = nheads / kv_mul
        let kvd = nkvh * hs
        let isQ = tgid < nheads
        let isK = !isQ && tgid < nheads + nkvh
        let hh = isQ ? tgid : (isK ? tgid - nheads : tgid - nheads - nkvh)
        let base = isQ ? tgid * hs : (isK ? qd + hh * hs : hh * hs)
        let bbase = isQ ? tgid * hs : (isK ? qd + hh * hs : qd + kvd + hh * hs)
        var v = (isQ || isK ? qk[base + lid] : vs[base + lid]) + (has_bias != 0u ? bias[bbase + lid] : 0.0)
        if (isQ || isK) {
            if (has_norm != 0u) {
                let s1 = simd_sum(v * v)
                if (gl_SubgroupInvocationID == 0u) {
                    part[gl_SubgroupID] = s1
                }
                barrier()
                var total = 0.0
                var sg = 0u
                while (sg < gl_NumSubgroups) {
                    total += part[sg]
                    sg++
                }
                let inv = 1.0 / sqrt(total / float(hs) + eps)
                v = w[(isK ? hs : 0u) + lid] * (v * inv)
            }
            // rope via the tgmem-staged partner (the whole head is resident in this tg);
            // partial rot: lanes >= rot pass through (identity coefficients, self partner)
            buf[lid] = v
            barrier()
            let halfr = rot / 2u
            let inrot = lid < rot
            let isHi = neox != 0u ? (lid >= halfr && inrot) : ((lid & 1u) != 0u && inrot)
            let j = min(neox != 0u ? (isHi ? lid - halfr : lid) : lid / 2u, halfr - 1u)
            let fcr = inrot ? tcos[j] : 1.0
            let fci = inrot ? tsin[j] : 0.0
            let other = buf[neox != 0u ? (isHi ? lid - halfr : (inrot ? lid + halfr : lid)) : (lid ^ 1u)]
            v = isHi ? other * fci + v * fcr : v * fcr - other * fci
        }
        if (isQ) {
            qk[base + lid] = v
        } elif (isK) {
            kd[hh * hs + lid] = float16(min(max(v, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
        } else {
            vd[hh * hs + lid] = float16(min(max(v, -65504.0), 65504.0))   // nolint:STYLE019
        }
    }
}

// f32-mirror twin — straight stores, no clamp.
[metal_dispatch(name = "enc_rpst32_c", pso = "g_pso_rpst32", tg = 64, grid = "npairs/64", params = "npairs : int64")]
class MetalRopeStoreF32 {
    @ssbo @binding = 0 qk : array<float>
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>
    @ssbo @binding = 2 @off = "koff" kd : array<float>
    @ssbo @binding = 3 @off = "voff" vd : array<float>
    @ssbo @binding = 4 @role = "weight" tcos : array<float>
    @ssbo @binding = 5 @role = "weight" tsin : array<float>
    @uniform @binding = 6 qd : uint
    @uniform @binding = 7 head_size : uint
    @uniform @binding = 8 npairs_q : uint
    @uniform @binding = 9 npairs : uint
    @uniform @binding = 10 neox : uint
    @ssbo @binding = 11 @role = "weight" bias : array<float>
    @uniform @binding = 12 has_bias : uint
    @uniform @binding = 13 rot : uint       // partial rotary width; == head_size for full

    [metal_kernel(name="metal_rope_store32_msl")]
    def metal_rope_store32 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let isK = gid >= npairs_q
        let lgid = isK ? gid - npairs_q : gid
        let half = head_size / 2u
        let halfr = rot / 2u
        let j = lgid % half
        let hbase = (lgid / half) * head_size
        let rotp = j < halfr
        let i = neox != 0u ? (rotp ? hbase + j : hbase + rot + (j - halfr)) : hbase + 2u * j
        let i2 = neox != 0u ? (rotp ? i + halfr : i + (half - halfr)) : i + 1u
        let fcr = rotp ? tcos[j] : 1.0
        let fci = rotp ? tsin[j] : 0.0
        let kvd = npairs * 2u - qd
        if (isK) {
            let v0 = qk[qd + i] + (has_bias != 0u ? bias[qd + i] : 0.0)
            let v1 = qk[qd + i2] + (has_bias != 0u ? bias[qd + i2] : 0.0)
            kd[i] = v0 * fcr - v1 * fci
            kd[i2] = v0 * fci + v1 * fcr
            vd[i] = vs[i] + (has_bias != 0u ? bias[qd + kvd + i] : 0.0)
            vd[i2] = vs[i2] + (has_bias != 0u ? bias[qd + kvd + i2] : 0.0)
        } else {
            let v0 = qk[i] + (has_bias != 0u ? bias[i] : 0.0)
            let v1 = qk[i2] + (has_bias != 0u ? bias[i2] : 0.0)
            qk[i] = v0 * fcr - v1 * fci
            qk[i2] = v0 * fci + v1 * fcr
        }
    }
}

// q8_0-mirror rope + KV-row store: q pairs rope in place; K/V rows quantize per 32-block (one
// thread per block) matching quantize_q8kv_row's exact semantics, so CPU fallback steps read the
// same codec. K blocks rope their 16 pairs first; V blocks copy raw.
[metal_dispatch(name = "enc_rpst_q8_c", pso = "g_pso_rpst_q8", tg = 64, grid = "nthreads/64", params = "nthreads : int64")]
class MetalRopeStoreQ8 {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) roped in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 @off = "koff" ksh : array<float16> // mirror K row, HALF-SCALE view (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 @role = "alias" @off = "koff" kqb : array<int8>    // the SAME K row, BYTE view
    @ssbo @binding = 4 @off = "voff" vsh : array<float16>
    @ssbo @binding = 5 @role = "alias" @off = "voff" vqb : array<int8>
    @ssbo @binding = 6 @role = "weight" tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 7 @role = "weight" tsin : array<float>
    @uniform @binding = 8 qd : uint
    @uniform @binding = 9 head_size : uint
    @uniform @binding = 10 npairs_q : uint  // qd/2 — the q-rope half of the grid
    @uniform @binding = 11 nblk : uint      // kv_dim/32 — K blocks (V adds the same again)
    @uniform @binding = 12 neox : uint      // pair (j, j+half) instead of (2j, 2j+1); same table row j
    @ssbo @binding = 13 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 14 has_bias : uint

    [metal_kernel(name="metal_rope_store_q8_msl")]
    def metal_rope_store_q8 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs_q + 2u * nblk) {
            return
        }
        let half = head_size / 2u
        if (gid < npairs_q) {
            let j = gid % half
            let hbase = (gid / half) * head_size
            let i = neox != 0u ? hbase + j : hbase + 2u * j
            let i2 = neox != 0u ? i + half : i + 1u
            let fcr = tcos[j]
            let fci = tsin[j]
            let v0 = qk[i] + (has_bias != 0u ? bias[i] : 0.0)
            let v1 = qk[i2] + (has_bias != 0u ? bias[i2] : 0.0)
            qk[i] = v0 * fcr - v1 * fci
            qk[i2] = v0 * fci + v1 * fcr
            return
        }
        let kvd = nblk * 32u
        let bi = gid - npairs_q
        let isV = bi >= nblk
        let blk = isV ? bi - nblk : bi
        let base = blk * 32u                // element base within the K/V row
        var vals : float[32]
        if (isV) {
            var e = 0u
            while (e < 32u) {
                vals[e] = vs[base + e] + (has_bias != 0u ? bias[qd + kvd + base + e] : 0.0)
                e++
            }
        } elif (neox != 0u) {
            // NEOX pairs straddle 32-blocks (partner at ±half) — rope per ELEMENT; partner
            // reads come from qk, which no K/V thread writes, so cross-block reads are safe
            var e = 0u
            while (e < 32u) {
                let ge = base + e
                let hp = ge % head_size
                let isHi = hp >= half
                let j = isHi ? hp - half : hp
                let fcr = tcos[j]
                let fci = tsin[j]
                let oi = isHi ? ge - half : ge + half
                let mine = qk[qd + ge] + (has_bias != 0u ? bias[qd + ge] : 0.0)
                let other = qk[qd + oi] + (has_bias != 0u ? bias[qd + oi] : 0.0)
                vals[e] = isHi ? other * fci + mine * fcr : mine * fcr - other * fci
                e++
            }
        } else {
            var p = 0u
            while (p < 16u) {
                let ge = base + 2u * p
                let j = (ge % head_size) / 2u
                let fcr = tcos[j]
                let fci = tsin[j]
                let v0 = qk[qd + ge] + (has_bias != 0u ? bias[qd + ge] : 0.0)
                let v1 = qk[qd + ge + 1u] + (has_bias != 0u ? bias[qd + ge + 1u] : 0.0)
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
[metal_dispatch(name = "enc_rpst_tq4_c", pso = "g_pso_rpst_tq4", tgmem = "metal_rope_store_tq4_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalRopeStoreTq4 {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) ropes + rotates in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 @off = "koff" ksh : array<float16> // mirror K row, HALF-SCALE view (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 @role = "alias" @off = "koff" kqb : array<uint8>   // the SAME row, BYTE view
    @ssbo @binding = 4 @off = "voff" vsh : array<float16>
    @ssbo @binding = 5 @role = "alias" @off = "voff" vqb : array<uint8>
    @ssbo @binding = 6 @role = "weight" tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 7 @role = "weight" tsin : array<float>
    @ssbo @binding = 8 @role = "weight" signs : array<float> // the tq4 rotation signs (prefix per head_size)
    @uniform @binding = 9 qd : uint
    @uniform @binding = 10 head_size : uint
    @uniform @binding = 11 nheads : uint
    @uniform @binding = 12 kv_mul : uint
    @uniform @binding = 13 neox : uint      // pair (j, j+half) instead of (2j, 2j+1); same table row j
    @ssbo @binding = 14 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 15 has_bias : uint
    @workgroup buf : float[512]     // <= max head_size (gemma4 global heads are 512)

    [metal_kernel(name="metal_rope_store_tq4_msl")]
    def metal_rope_store_tq4 {
        let tgid = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        let hs = head_size
        let half = hs / 2u
        let nkvh = nheads / kv_mul
        let kvd = nkvh * hs
        let isQ = tgid < nheads
        let isK = !isQ && tgid < nheads + nkvh
        // this head's element run: q heads in the q region, k heads in the k region, v heads in vs
        let hh = isQ ? tgid : (isK ? tgid - nheads : tgid - nheads - nkvh)
        let base = isQ ? tgid * hs : (isK ? qd + hh * hs : hh * hs)
        // the element's index in the [qd | kvd | kvd] qkv vector — the bias plane's layout
        let bbase = isQ ? tgid * hs : (isK ? qd + hh * hs : qd + kvd + hh * hs)
        var v = 0.0
        if (isQ || isK) {
            // rope via the pair partner (the whole head is resident in this tg either way)
            let isHi = neox != 0u ? lid >= half : (lid & 1u) != 0u
            let j = neox != 0u ? (isHi ? lid - half : lid) : lid / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            let po = neox != 0u ? (isHi ? lid - half : lid + half) : (lid ^ 1u)
            let mine = qk[base + lid] + (has_bias != 0u ? bias[bbase + lid] : 0.0)
            let other = qk[base + po] + (has_bias != 0u ? bias[bbase + po] : 0.0)
            v = isHi ? other * fci + mine * fcr : mine * fcr - other * fci
        } else {
            v = vs[base + lid] + (has_bias != 0u ? bias[bbase + lid] : 0.0)
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
[metal_dispatch(name = "enc_unsign_c", pso = "g_pso_unsign", tgmem = "metal_fwht_unsign_msl_tgmem", tg = "head_size", grid = "nheads, nrows", params = "nheads : int64, nrows : int64, head_size : int64")]
class MetalFwhtUnsign {
    @ssbo @binding = 0 yo : array<float>    // [rows x qstride] attention out, un-rotated in place
    @ssbo @binding = 1 @role = "weight" signs : array<float>
    @uniform @binding = 2 head_size : uint
    @uniform @binding = 3 qstride : uint
    @workgroup buf : float[512]     // <= max head_size (gemma4 global heads are 512)

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
    @ssbo @binding = 0 @role = "weight" wqb : array<int8>    // 34B-block W blob, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" x : array<float4>    // [B x n] f32 rows, float4 view
    @ssbo @binding = 4 @role = "write" y : array<float>     // [B x d]
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y ROW stride (== ddim dense)
    @workgroup txp : float4[128]            // [2 streams x 64 float4] X panel chunk

    [metal_kernel(name="metal_gemv_b2_msl")]
    def metal_gemv_b2 {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, ddim - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var acc : float[2]
        var cb = 0u
        while (cb + 64u <= nb4) {
            barrier()                       // previous round done reading the panel
            txp[lid] = x[(lid / 64u) * nb4 + cb + lid % 64u]
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let bi = row * nb + k / 8u
                let qb = bi * 34u + 2u + (k % 8u) * 4u
                let w4 = float4(float(wqb[qb]), float(wqb[qb + 1u]), float(wqb[qb + 2u]),
                    float(wqb[qb + 3u])) * float(wsh[bi * 17u])
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
            let bi = row * nb + k / 8u
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let w4 = float4(float(wqb[qb]), float(wqb[qb + 1u]), float(wqb[qb + 2u]),
                float(wqb[qb + 3u])) * float(wsh[bi * 17u])
            for [unroll_full] (b in range(2)) {
                let xv = x[uint(b) * nb4 + k]
                acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(2)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalGemvB4 {
    @ssbo @binding = 0 @role = "weight" wqb : array<int8>
    @ssbo @binding = 2 @role = "weight" wsh : array<float16>
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y ROW stride (== ddim dense)
    @workgroup txp : float4[256]            // [4 streams x 64 float4] X panel chunk

    [metal_kernel(name="metal_gemv_b4_msl")]
    def metal_gemv_b4 {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, ddim - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
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
                let bi = row * nb + k / 8u
                let qb = bi * 34u + 2u + (k % 8u) * 4u
                let w4 = float4(float(wqb[qb]), float(wqb[qb + 1u]), float(wqb[qb + 2u]),
                    float(wqb[qb + 3u])) * float(wsh[bi * 17u])
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
            let bi = row * nb + k / 8u
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let w4 = float4(float(wqb[qb]), float(wqb[qb + 1u]), float(wqb[qb + 2u]),
                float(wqb[qb + 3u])) * float(wsh[bi * 17u])
            for [unroll_full] (b in range(4)) {
                let xv = x[uint(b) * nb4 + k]
                acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(4)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

// Skinny-batch GEMV: streamed form beats x-staged panel kernels ~12% at B <= 4. 64 threads = 2
// simdgroups x 2 row-halves; 16 lanes stream one output row's interleaved 34B q8 blocks, batch
// columns share the dequantized weight stream. Requires n % 256 == 0 (unguarded 4-chunk stripe).
class MetalQ8MvB2 {
    @ssbo @binding = 0 @role = "weight" wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "read" x : array<float4>    // [B rows x xs4 float4-stride] f32 rows
    @ssbo @binding = 3 @role = "write" y : array<float>     // [B x ddim]
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @uniform @binding = 6 xs4 : uint        // x row stride in float4s (ndim/4 = dense)
    @uniform @binding = 7 ys : uint         // y ROW stride (== ddim dense; qkvd at the fused-QKV
                                            // segment sites — the kq mv twins' bys contract)

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
                y[uint(b) * ys + row0] = s
            }
        }
    }
}

// 4-column twin (serves B = 3..4; pad column reads clamp to row 0, stores gate on nrows). Drops
// to 8 lanes per output row (4 rows/simdgroup, 8 rows/tg): fewer lanes per weight load, more
// rows in flight hides the block-load latency instead.
class MetalQ8MvB4 {
    @ssbo @binding = 0 @role = "weight" wsh : array<float16>
    @ssbo @binding = 1 @role = "weight" wqb : array<int8>
    @ssbo @binding = 2 @role = "read" x : array<float4>
    @ssbo @binding = 3 @role = "write" y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @uniform @binding = 6 nrows : uint
    @uniform @binding = 7 xs4 : uint
    @uniform @binding = 8 ys : uint         // y ROW stride (== ddim dense)

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
                y[uint(b) * ys + row0] = s
            }
        }
    }
}

// Fused fixed-B W1|W3 GEMV + swiglu (batch twin of MetalQ8GemvW13Sw): each subgroup dots the
// gate row r AND up row r+hidden, writing silu(gate)*up directly — replaces the w1/w3/swiglu
// triple with one dispatch. B2 form serves B <= 2; B4 serves 3..4.
class MetalGemvW13SwB2 {
    @ssbo @binding = 0 @role = "weight" wqb1 : array<int8>   // w1 blob, byte view @ its tensor offset
    @ssbo @binding = 2 @role = "weight" wsh1 : array<float16>   // ... half-scale view
    @ssbo @binding = 3 @role = "read" x : array<float4>    // [B x ndim] f32 rows, float4 view
    @ssbo @binding = 4 @role = "write" y : array<float>     // [B x hidden] silu(gate)*up
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 hidden : uint     // output rows
    @ssbo @binding = 8 @role = "weight" wqb3 : array<int8>   // w3 blob views
    @ssbo @binding = 9 @role = "weight" wsh3 : array<float16>
    @workgroup txp : float4[128]            // [2 streams x 64 float4] X panel chunk

    [metal_kernel(name="metal_gemv_w13sw_b2_msl")]
    def metal_gemv_w13sw_b2 {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, hidden - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var accg : float[2]
        var accu : float[2]
        var cb = 0u
        while (cb + 64u <= nb4) {
            barrier()                       // previous round done reading the panel
            txp[lid] = x[(lid / 64u) * nb4 + cb + lid % 64u]
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let bi = row * nb + k / 8u
                let qb = bi * 34u + 2u + (k % 8u) * 4u
                let wg = float4(float(wqb1[qb]), float(wqb1[qb + 1u]), float(wqb1[qb + 2u]),
                    float(wqb1[qb + 3u])) * float(wsh1[bi * 17u])
                let wu = float4(float(wqb3[qb]), float(wqb3[qb + 1u]), float(wqb3[qb + 2u]),
                    float(wqb3[qb + 3u])) * float(wsh3[bi * 17u])
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
            let bi = row * nb + k / 8u
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let wg = float4(float(wqb1[qb]), float(wqb1[qb + 1u]), float(wqb1[qb + 2u]),
                float(wqb1[qb + 3u])) * float(wsh1[bi * 17u])
            let wu = float4(float(wqb3[qb]), float(wqb3[qb + 1u]), float(wqb3[qb + 2u]),
                float(wqb3[qb + 3u])) * float(wsh3[bi * 17u])
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
    @ssbo @binding = 0 @role = "weight" wqb1 : array<int8>
    @ssbo @binding = 2 @role = "weight" wsh1 : array<float16>
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 hidden : uint
    @ssbo @binding = 8 @role = "weight" wqb3 : array<int8>
    @ssbo @binding = 9 @role = "weight" wsh3 : array<float16>
    @workgroup txp : float4[256]            // [4 streams x 64 float4] X panel chunk

    [metal_kernel(name="metal_gemv_w13sw_b4_msl")]
    def metal_gemv_w13sw_b4 {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, hidden - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
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
                let bi = row * nb + k / 8u
                let qb = bi * 34u + 2u + (k % 8u) * 4u
                let wg = float4(float(wqb1[qb]), float(wqb1[qb + 1u]), float(wqb1[qb + 2u]),
                    float(wqb1[qb + 3u])) * float(wsh1[bi * 17u])
                let wu = float4(float(wqb3[qb]), float(wqb3[qb + 1u]), float(wqb3[qb + 2u]),
                    float(wqb3[qb + 3u])) * float(wsh3[bi * 17u])
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
            let bi = row * nb + k / 8u
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let wg = float4(float(wqb1[qb]), float(wqb1[qb + 1u]), float(wqb1[qb + 2u]),
                float(wqb1[qb + 3u])) * float(wsh1[bi * 17u])
            let wu = float4(float(wqb3[qb]), float(wqb3[qb + 1u]), float(wqb3[qb + 2u]),
                float(wqb3[qb + 3u])) * float(wsh3[bi * 17u])
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
    @ssbo @binding = 0 @role = "weight" wqb : array<int8>    // 34B-block W blob, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" x : array<float4>    // X [M' x n] f32 rows (M' = 32-padded B)
    @ssbo @binding = 4 @role = "write" y : array<float>     // Y [M' x d]
    @uniform @binding = 5 kdim : uint       // n (K)
    @uniform @binding = 6 ndim : uint       // d (N)
    @uniform @binding = 7 ys : uint         // y TOKEN-row stride (== ndim dense; qkvd at the
                                            // fused-QKV segment sites, y bound at the column offset)
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
        var qp = unsafe(addr(wqb[((nBase + lr) * nkb) * 34u + 2u + il * 8u]))
        var sph = unsafe(addr(wsh[((nBase + lr) * nkb) * 17u]))
        var xp = unsafe(addr(x[(mBase + lr) * kdim4 + il * 2u]))
        var kb = 0u
        while (kb < nkb) {
            let sA = float(unsafe(sph[0]))
            let b0 = unsafe(xp[0])
            let b1 = unsafe(xp[1])
            let w40 = float4(float(unsafe(qp[0u])), float(unsafe(qp[1u])),
                float(unsafe(qp[2u])), float(unsafe(qp[3u]))) * sA
            let w41 = float4(float(unsafe(qp[4u])), float(unsafe(qp[5u])),
                float(unsafe(qp[6u])), float(unsafe(qp[7u]))) * sA
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
            qp = unsafe(qp + 34u)
            sph = unsafe(sph + 17u)
            xp = unsafe(xp + 8u)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 16u
        simdgroup_store(mc[0], y, row0 * ys + col0, ys)
        simdgroup_store(mc[1], y, row0 * ys + (col0 + 8u), ys)
        simdgroup_store(mc[2], y, (row0 + 8u) * ys + col0, ys)
        simdgroup_store(mc[3], y, (row0 + 8u) * ys + (col0 + 8u), ys)
    }
}

// Split-K twin of MetalQ8GemmB: grid.y gains a k-slice axis, each threadgroup walks its own
// partial-C plane, and a separate MetalSkReduce dispatch sums them (no float atomics,
// deterministic). At M-pad-32 this is the only way to grow the grid past d/32 on starved sites.
class MetalQ8GemmBSk {
    @ssbo @binding = 0 @role = "weight" wqb : array<int8>    // 34B-block W blob, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" x : array<float4>    // X [32 x n] f32 rows
    @ssbo @binding = 4 @role = "write" part : array<float>  // [ksplit x 32 x d] partial-C planes
    @uniform @binding = 5 kdim : uint       // n (K)
    @uniform @binding = 6 ndim : uint       // d (N)
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
        var qp = unsafe(addr(wqb[((nBase + lr) * nkb + kb0) * 34u + 2u + il * 8u]))
        var sph = unsafe(addr(wsh[((nBase + lr) * nkb + kb0) * 17u]))
        var xp = unsafe(addr(x[(mBase + lr) * kdim4 + kb0 * 8u + il * 2u]))
        var kb = 0u
        while (kb < kbn) {
            let sA = float(unsafe(sph[0]))
            let b0 = unsafe(xp[0])
            let b1 = unsafe(xp[1])
            let w40 = float4(float(unsafe(qp[0u])), float(unsafe(qp[1u])),
                float(unsafe(qp[2u])), float(unsafe(qp[3u]))) * sA
            let w41 = float4(float(unsafe(qp[4u])), float(unsafe(qp[5u])),
                float(unsafe(qp[6u])), float(unsafe(qp[7u]))) * sA
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
            qp = unsafe(qp + 34u)
            sph = unsafe(sph + 17u)
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
    @ssbo @binding = 0 @role = "read" part : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
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

// Metal-4 tensor twins of the batch GEMMs: same binds/grids, bodies lowered to one
// tmm2d_q8b_f32 each (raced via runtime.metal_tensor — crowns "gemmb_q8" / "gemmb_sk_q8" /
// "gemm64b_q8"). The split-K twin passes the FULL row stride in blocks (ldwb) with a k-slice kk.
class MetalQ8GemmBT {
    @ssbo @binding = 0 wqb : array<int8>    // 34B-block W blob, byte view
    @ssbo @binding = 2 wsh : array<float16> // the same buffer, half-scale view
    @ssbo @binding = 3 x : array<float>     // X [M' x n] f32 rows
    @ssbo @binding = 4 y : array<float>     // Y [M' x d]
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @uniform @binding = 7 ys : uint

    [metal_kernel(name="metal_q8_gemm_b_t_msl")]
    def metal_q8_gemm_b_t {
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let nkb = kdim / 32u
        var sp = unsafe(addr(wsh[(nBase * nkb) * 17u]))
        var wp = unsafe(addr(wqb[(nBase * nkb) * 34u + 2u]))
        var ap = unsafe(addr(x[mBase * kdim]))
        var cp = unsafe(addr(y[mBase * ys + nBase]))
        tmm2d_q8b_f32(32u, 32u, 4u, sp, wp, ap, kdim, cp, ys, kdim, nkb)
    }
}

class MetalQ8GemmBSkT {
    @ssbo @binding = 0 wqb : array<int8>    // 34B-block W blob, byte view
    @ssbo @binding = 2 wsh : array<float16> // the same buffer, half-scale view
    @ssbo @binding = 3 x : array<float>     // X [32 x n] f32 rows
    @ssbo @binding = 4 part : array<float>  // [ksplit x 32 x d] partial-C planes
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @uniform @binding = 8 ksplit : uint

    [metal_kernel(name="metal_q8_gemm_b_sk_t_msl")]
    def metal_q8_gemm_b_sk_t {
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let sl = gl_WorkGroupID.y
        let nkb = kdim / 32u
        let kbn = nkb / ksplit
        let kb0 = sl * kbn
        var sp = unsafe(addr(wsh[(nBase * nkb + kb0) * 17u]))
        var wp = unsafe(addr(wqb[(nBase * nkb + kb0) * 34u + 2u]))
        var ap = unsafe(addr(x[mBase * kdim + kb0 * 32u]))
        var cp = unsafe(addr(part[sl * 32u * ndim + mBase * ndim + nBase]))
        tmm2d_q8b_f32(32u, 32u, 4u, sp, wp, ap, kdim, cp, ndim, kbn * 32u, nkb)
    }
}

class MetalQ8Gemm64BT {
    @ssbo @binding = 0 wqb : array<int8>    // 34B-block W blob, byte view
    @ssbo @binding = 2 wsh : array<float16> // the same buffer, half-scale view
    @ssbo @binding = 3 x : array<float>     // X [M' x n] f32 rows
    @ssbo @binding = 4 y : array<float>     // Y [M' x d]
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint

    [metal_kernel(name="metal_q8_gemm64_b_t_msl")]
    def metal_q8_gemm64_b_t {
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let nkb = kdim / 32u
        var sp = unsafe(addr(wsh[(nBase * nkb) * 17u]))
        var wp = unsafe(addr(wqb[(nBase * nkb) * 34u + 2u]))
        var ap = unsafe(addr(x[mBase * kdim]))
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_q8b_f32(32u, 64u, 4u, sp, wp, ap, kdim, cp, ndim, kdim, nkb)
    }
}

// The 64-wide twin of MulMm's 32(M) x 64(N) tile: half the threadgroups (d/64) at ~29% fewer
// instructions per element — wins where the grid stays fat (e.g. the vocab classifier). Each
// simdgroup owns a 16x32 quadrant (2 token x 4 wrow tiles).
class MetalQ8Gemm64B {
    @ssbo @binding = 0 @role = "weight" wqb : array<int8>    // 34B-block W blob, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" x : array<float4>    // X [M' x n] f32 rows (M' = 32-padded B)
    @ssbo @binding = 4 @role = "write" y : array<float>     // Y [M' x d]
    @uniform @binding = 5 kdim : uint       // n (K)
    @uniform @binding = 6 ndim : uint       // d (N)
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
        var qp = unsafe(addr(wqb[((nBase + lr0) * nkb) * 34u + 2u + il0 * 16u]))
        var sph = unsafe(addr(wsh[((nBase + lr0) * nkb) * 17u]))
        var xp = unsafe(addr(x[(mBase + lr1) * kdim4 + il1 * 2u]))
        var kb = 0u
        while (kb < nkb) {
            let sA = float(unsafe(sph[0]))
            let b0 = unsafe(xp[0])
            let b1 = unsafe(xp[1])
            for [unroll_full] (i in range(16)) {
                va[i] = float16(float(unsafe(qp[i])) * sA)
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
            qp = unsafe(qp + 34u)
            sph = unsafe(sph + 17u)
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
    @ssbo @binding = 0 @role = "read" kc : array<float16>  // the whole mirror arena, f16 view
    @ssbo @binding = 1 @role = "read" vc : array<float16>
    @ssbo @binding = 2 @role = "read" q : array<float>     // [B x qstride] roped queries (the fused QKV rows)
    @ssbo @binding = 3 @role = "write" yo : array<float>    // [B x nheads*hs] attention out
    @ssbo @binding = 4 @role = "read" rt : array<uint4>    // per-row (k base, v base, cap_rows, cnt) — arena elems
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @uniform @binding = 12 softcap : float
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup scores : float[128]
    @workgroup red : float[16]

    [metal_kernel(name="metal_sq_attn_b16_msl")]
    def metal_sq_attn_b16 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rrow = rt[bb]
        let cnt = rrow.w
        let kb0 = rrow.x + lay * rrow.z   // lay = the layer's KV-row ELEMENT prefix (Σ kv_dim below l)
        let vb0 = rrow.y + lay * rrow.z
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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * float(kc[kb + 4u])
            }
            if (epl > 5u) {
                s += q5 * float(kc[kb + 5u])
            }
            if (epl > 6u) {
                s += q6 * float(kc[kb + 6u])
            }
            if (epl > 7u) {
                s += q7 * float(kc[kb + 7u])
            }
            if (epl > 8u) {
                s += q8 * float(kc[kb + 8u])
                s += q9 * float(kc[kb + 9u])
                s += q10 * float(kc[kb + 10u])
                s += q11 * float(kc[kb + 11u])
            }
            if (epl > 12u) {
                s += q12 * float(kc[kb + 12u])
                s += q13 * float(kc[kb + 13u])
                s += q14 * float(kc[kb + 14u])
                s += q15 * float(kc[kb + 15u])
            }
            s = simd_sum(s)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
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
        if (hass != 0u) {
            mm = max(mm, sink[h])
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
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
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
    @ssbo @binding = 0 @role = "read" kc : array<float>
    @ssbo @binding = 1 @role = "read" vc : array<float>
    @ssbo @binding = 2 @role = "read" q : array<float>
    @ssbo @binding = 3 @role = "write" yo : array<float>
    @ssbo @binding = 4 @role = "read" rt : array<uint4>
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @uniform @binding = 12 softcap : float
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup scores : float[128]
    @workgroup red : float[16]

    [metal_kernel(name="metal_sq_attn_b32_msl")]
    def metal_sq_attn_b32 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rrow = rt[bb]
        let cnt = rrow.w
        let kb0 = rrow.x + lay * rrow.z   // lay = the layer's KV-row ELEMENT prefix (Σ kv_dim below l)
        let vb0 = rrow.y + lay * rrow.z
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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * kc[kb + 4u]
            }
            if (epl > 5u) {
                s += q5 * kc[kb + 5u]
            }
            if (epl > 6u) {
                s += q6 * kc[kb + 6u]
            }
            if (epl > 7u) {
                s += q7 * kc[kb + 7u]
            }
            if (epl > 8u) {
                s += q8 * kc[kb + 8u]
                s += q9 * kc[kb + 9u]
                s += q10 * kc[kb + 10u]
                s += q11 * kc[kb + 11u]
            }
            if (epl > 12u) {
                s += q12 * kc[kb + 12u]
                s += q13 * kc[kb + 13u]
                s += q14 * kc[kb + 14u]
                s += q15 * kc[kb + 15u]
            }
            s = simd_sum(s)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
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
        if (hass != 0u) {
            mm = max(mm, sink[h])
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
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
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
    @ssbo @binding = 0 @role = "read" ksh : array<float16> // the whole mirror arena, half-scale view
    @ssbo @binding = 1 @role = "read" kqb : array<int8>    // the SAME arena, byte view
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<int8>
    @ssbo @binding = 4 @role = "read" q : array<float>     // [B x qstride] roped queries (the fused QKV rows)
    @ssbo @binding = 5 @role = "write" yo : array<float>    // [B x nheads*hs] attention out
    @ssbo @binding = 6 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 softcap : float
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup scores : float[128]
    @workgroup red : float[16]

    [metal_kernel(name="metal_sq_attn_b_q8_msl")]
    def metal_sq_attn_b_q8 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rrow = rt[bb]
        let cnt = rrow.w
        let kb0b = rrow.x + lay * rrow.z   // layer slab base, BYTES (lay = the layer's row-BYTE prefix)
        let vb0b = rrow.y + lay * rrow.z
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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * float(kqb[kb + 4u])
            }
            if (epl > 5u) {
                s += q5 * float(kqb[kb + 5u])
            }
            if (epl > 6u) {
                s += q6 * float(kqb[kb + 6u])
            }
            if (epl > 7u) {
                s += q7 * float(kqb[kb + 7u])
            }
            if (epl > 8u) {
                s += q8 * float(kqb[kb + 8u])
                s += q9 * float(kqb[kb + 9u])
                s += q10 * float(kqb[kb + 10u])
                s += q11 * float(kqb[kb + 11u])
            }
            if (epl > 12u) {
                s += q12 * float(kqb[kb + 12u])
                s += q13 * float(kqb[kb + 13u])
                s += q14 * float(kqb[kb + 14u])
                s += q15 * float(kqb[kb + 15u])
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
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
        if (hass != 0u) {
            mm = max(mm, sink[h])
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
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
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
    @ssbo @binding = 0 @role = "read" ksh : array<float16>
    @ssbo @binding = 1 @role = "read" kqb : array<uint8>
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<uint8>
    @ssbo @binding = 4 @role = "read" q : array<float>     // [B x qstride] roped + rotated queries
    @ssbo @binding = 5 @role = "write" yo : array<float>    // [B x nheads*hs] attention out (rotated basis)
    @ssbo @binding = 6 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 softcap : float
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup scores : float[128]
    @workgroup red : float[16]

    [metal_kernel(name="metal_sq_attn_b_tq4_msl")]
    def metal_sq_attn_b_tq4 {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let rrow = rt[bb]
        let cnt = rrow.w
        let kb0b = rrow.x + lay * rrow.z   // lay = the layer's row-BYTE prefix
        let vb0b = rrow.y + lay * rrow.z
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
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
                if (epl > 4u) {
                    let by2 = uint(kqb[kb + 2u])
                    s += q4 * float(int(by2 & 15u) - 8)
                    s += q5 * float(int(by2 >> 4u) - 8)
                }
                if (epl > 6u) {
                    let by3 = uint(kqb[kb + 3u])
                    s += q6 * float(int(by3 & 15u) - 8)
                    s += q7 * float(int(by3 >> 4u) - 8)
                }
                if (epl > 8u) {
                    let by4 = uint(kqb[kb + 4u])
                    let by5 = uint(kqb[kb + 5u])
                    s += q8 * float(int(by4 & 15u) - 8)
                    s += q9 * float(int(by4 >> 4u) - 8)
                    s += q10 * float(int(by5 & 15u) - 8)
                    s += q11 * float(int(by5 >> 4u) - 8)
                }
                if (epl > 12u) {
                    let by6 = uint(kqb[kb + 6u])
                    let by7 = uint(kqb[kb + 7u])
                    s += q12 * float(int(by6 & 15u) - 8)
                    s += q13 * float(int(by6 >> 4u) - 8)
                    s += q14 * float(int(by7 & 15u) - 8)
                    s += q15 * float(int(by7 >> 4u) - 8)
                }
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
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
        if (hass != 0u) {
            mm = max(mm, sink[h])
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
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
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
    @ssbo @binding = 0 @role = "read" kc : array<float16>
    @ssbo @binding = 1 @role = "read" vc : array<float16>
    @ssbo @binding = 2 @role = "read" q : array<float>
    @ssbo @binding = 3 @role = "write" part : array<float>  // [B x nheads x nchmax x (hs+2)]
    @ssbo @binding = 4 @role = "read" rt : array<uint4>
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @uniform @binding = 12 nchmax : uint
    @uniform @binding = 13 softcap : float
    @uniform @binding = 14 window : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]

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
        let kb0 = rrow.x + lay * rrow.z   // lay = the layer's KV-row ELEMENT prefix (Σ kv_dim below l)
        let vb0 = rrow.y + lay * rrow.z
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        if (c1 <= wstart) {
            return
        }
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * float(kc[kb + 4u])
            }
            if (epl > 5u) {
                s += q5 * float(kc[kb + 5u])
            }
            if (epl > 6u) {
                s += q6 * float(kc[kb + 6u])
            }
            if (epl > 7u) {
                s += q7 * float(kc[kb + 7u])
            }
            if (epl > 8u) {
                s += q8 * float(kc[kb + 8u])
                s += q9 * float(kc[kb + 9u])
                s += q10 * float(kc[kb + 10u])
                s += q11 * float(kc[kb + 11u])
            }
            if (epl > 12u) {
                s += q12 * float(kc[kb + 12u])
                s += q13 * float(kc[kb + 13u])
                s += q14 * float(kc[kb + 14u])
                s += q15 * float(kc[kb + 15u])
            }
            s = simd_sum(s)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
            }
            j += gl_NumSubgroups
        }
        barrier()
        if (wstart > c0) {
            // boundary chunk: mask rows below the window start (uniform branch — barrier is safe)
            var jm = lid
            while (jm < nrows) {
                if (c0 + jm < wstart) {
                    scores[jm] = -3.0e38
                }
                jm += nt
            }
            barrier()
        }
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
    @ssbo @binding = 0 @role = "read" kc : array<float>
    @ssbo @binding = 1 @role = "read" vc : array<float>
    @ssbo @binding = 2 @role = "read" q : array<float>
    @ssbo @binding = 3 @role = "write" part : array<float>
    @ssbo @binding = 4 @role = "read" rt : array<uint4>
    @uniform @binding = 5 lay : uint
    @uniform @binding = 6 kv_dim : uint
    @uniform @binding = 7 nheads : uint
    @uniform @binding = 8 kv_mul : uint
    @uniform @binding = 9 hs : uint
    @uniform @binding = 10 scale : float
    @uniform @binding = 11 qstride : uint
    @uniform @binding = 12 nchmax : uint
    @uniform @binding = 13 softcap : float
    @uniform @binding = 14 window : uint
    @workgroup scores : float[64]
    @workgroup red : float[16]

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
        let kb0 = rrow.x + lay * rrow.z   // lay = the layer's KV-row ELEMENT prefix (Σ kv_dim below l)
        let vb0 = rrow.y + lay * rrow.z
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        if (c1 <= wstart) {
            return
        }
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * kc[kb + 4u]
            }
            if (epl > 5u) {
                s += q5 * kc[kb + 5u]
            }
            if (epl > 6u) {
                s += q6 * kc[kb + 6u]
            }
            if (epl > 7u) {
                s += q7 * kc[kb + 7u]
            }
            if (epl > 8u) {
                s += q8 * kc[kb + 8u]
                s += q9 * kc[kb + 9u]
                s += q10 * kc[kb + 10u]
                s += q11 * kc[kb + 11u]
            }
            if (epl > 12u) {
                s += q12 * kc[kb + 12u]
                s += q13 * kc[kb + 13u]
                s += q14 * kc[kb + 14u]
                s += q15 * kc[kb + 15u]
            }
            s = simd_sum(s)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
            }
            j += gl_NumSubgroups
        }
        barrier()
        if (wstart > c0) {
            // boundary chunk: mask rows below the window start (uniform branch — barrier is safe)
            var jm = lid
            while (jm < nrows) {
                if (c0 + jm < wstart) {
                    scores[jm] = -3.0e38
                }
                jm += nt
            }
            barrier()
        }
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
    @ssbo @binding = 0 @role = "read" ksh : array<float16>
    @ssbo @binding = 1 @role = "read" kqb : array<int8>
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<int8>
    @ssbo @binding = 4 @role = "read" q : array<float>
    @ssbo @binding = 5 @role = "write" part : array<float>  // [B x nheads x nchmax x (hs+2)]
    @ssbo @binding = 6 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 nchmax : uint
    @uniform @binding = 15 softcap : float
    @uniform @binding = 16 window : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]

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
        let kb0b = rrow.x + lay * rrow.z   // lay = the layer's row-BYTE prefix
        let vb0b = rrow.y + lay * rrow.z
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * hs
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        if (c1 <= wstart) {
            return
        }
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
            if (epl > 4u) {
                s += q4 * float(kqb[kb + 4u])
            }
            if (epl > 5u) {
                s += q5 * float(kqb[kb + 5u])
            }
            if (epl > 6u) {
                s += q6 * float(kqb[kb + 6u])
            }
            if (epl > 7u) {
                s += q7 * float(kqb[kb + 7u])
            }
            if (epl > 8u) {
                s += q8 * float(kqb[kb + 8u])
                s += q9 * float(kqb[kb + 9u])
                s += q10 * float(kqb[kb + 10u])
                s += q11 * float(kqb[kb + 11u])
            }
            if (epl > 12u) {
                s += q12 * float(kqb[kb + 12u])
                s += q13 * float(kqb[kb + 13u])
                s += q14 * float(kqb[kb + 14u])
                s += q15 * float(kqb[kb + 15u])
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
            }
            j += gl_NumSubgroups
        }
        barrier()
        if (wstart > c0) {
            // boundary chunk: mask rows below the window start (uniform branch — barrier is safe)
            var jm = lid
            while (jm < nrows) {
                if (c0 + jm < wstart) {
                    scores[jm] = -3.0e38
                }
                jm += nt
            }
            barrier()
        }
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
    @ssbo @binding = 0 @role = "read" ksh : array<float16>
    @ssbo @binding = 1 @role = "read" kqb : array<uint8>
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<uint8>
    @ssbo @binding = 4 @role = "read" q : array<float>
    @ssbo @binding = 5 @role = "write" part : array<float>  // [B x nheads x nchmax x (hs+2)]
    @ssbo @binding = 6 @role = "read" rt : array<uint4>
    @uniform @binding = 7 lay : uint
    @uniform @binding = 8 nblk : uint
    @uniform @binding = 9 nheads : uint
    @uniform @binding = 10 kv_mul : uint
    @uniform @binding = 11 hs : uint
    @uniform @binding = 12 scale : float
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 nchmax : uint
    @uniform @binding = 15 softcap : float
    @uniform @binding = 16 window : uint
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]

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
        let kb0b = rrow.x + lay * rrow.z   // lay = the layer's row-BYTE prefix
        let vb0b = rrow.y + lay * rrow.z
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * hs
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        if (c1 <= wstart) {
            return
        }
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let epl = hs / 32u
        let qb = qoff + sl * epl
        let q0 = q[qb]
        let q1 = epl > 1u ? q[qb + 1u] : 0.0
        let q2 = epl > 2u ? q[qb + 2u] : 0.0
        let q3 = epl > 3u ? q[qb + 3u] : 0.0
        let q4 = epl > 4u ? q[qb + 4u] : 0.0
        let q5 = epl > 5u ? q[qb + 5u] : 0.0
        let q6 = epl > 6u ? q[qb + 6u] : 0.0
        let q7 = epl > 7u ? q[qb + 7u] : 0.0
        let q8 = epl > 8u ? q[qb + 8u] : 0.0
        let q9 = epl > 9u ? q[qb + 9u] : 0.0
        let q10 = epl > 10u ? q[qb + 10u] : 0.0
        let q11 = epl > 11u ? q[qb + 11u] : 0.0
        let q12 = epl > 12u ? q[qb + 12u] : 0.0
        let q13 = epl > 13u ? q[qb + 13u] : 0.0
        let q14 = epl > 14u ? q[qb + 14u] : 0.0
        let q15 = epl > 15u ? q[qb + 15u] : 0.0
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
                if (epl > 4u) {
                    let by2 = uint(kqb[kb + 2u])
                    s += q4 * float(int(by2 & 15u) - 8)
                    s += q5 * float(int(by2 >> 4u) - 8)
                }
                if (epl > 6u) {
                    let by3 = uint(kqb[kb + 3u])
                    s += q6 * float(int(by3 & 15u) - 8)
                    s += q7 * float(int(by3 >> 4u) - 8)
                }
                if (epl > 8u) {
                    let by4 = uint(kqb[kb + 4u])
                    let by5 = uint(kqb[kb + 5u])
                    s += q8 * float(int(by4 & 15u) - 8)
                    s += q9 * float(int(by4 >> 4u) - 8)
                    s += q10 * float(int(by5 & 15u) - 8)
                    s += q11 * float(int(by5 >> 4u) - 8)
                }
                if (epl > 12u) {
                    let by6 = uint(kqb[kb + 6u])
                    let by7 = uint(kqb[kb + 7u])
                    s += q12 * float(int(by6 & 15u) - 8)
                    s += q13 * float(int(by6 >> 4u) - 8)
                    s += q14 * float(int(by7 & 15u) - 8)
                    s += q15 * float(int(by7 >> 4u) - 8)
                }
            }
            s = simd_sum(s * d)
            if (sl == 0u) {
                let sc = s * scale
                // tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past
                // ~44, while exp saturates: exp(INF) -> denom INF -> the term is 0
                scores[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
            }
            j += gl_NumSubgroups
        }
        barrier()
        if (wstart > c0) {
            // boundary chunk: mask rows below the window start (uniform branch — barrier is safe)
            var jm = lid
            while (jm < nrows) {
                if (c0 + jm < wstart) {
                    scores[jm] = -3.0e38
                }
                jm += nt
            }
            barrier()
        }
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
    @ssbo @binding = 0 @role = "read" part : array<float>
    @ssbo @binding = 1 @role = "write" yo : array<float>
    @ssbo @binding = 2 @role = "read" rt : array<uint4>
    @uniform @binding = 3 nchmax : uint
    @uniform @binding = 4 nheads : uint
    @uniform @binding = 5 hs : uint
    @uniform @binding = 6 window : uint     // > 0 = sliding span — this row's below-window chunks are unwritten
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint

    [metal_kernel(name="metal_sq_attn_comb_b_msl")]
    def metal_sq_attn_comb_b {
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let cnt = rt[bb].w
        let nch = (cnt + 63u) / 64u
        let wstart = window > 0u && cnt > window ? cnt - window : 0u
        let chlo = wstart / 64u
        let base = (bb * nheads + h) * nchmax
        var mm = -3.0e38
        var ci = chlo
        while (ci < nch) {
            mm = max(mm, part[(base + ci) * (hs + 2u) + hs])
            ci++
        }
        if (hass != 0u) {
            mm = max(mm, sink[h])
        }
        var acc = 0.0
        var l = 0.0
        ci = chlo
        while (ci < nch) {
            let pb = (base + ci) * (hs + 2u)
            let w = exp(part[pb + hs] - mm)
            acc += part[pb + lid] * w
            l += part[pb + hs + 1u] * w
            ci++
        }
        if (hass != 0u) {
            l += exp(max(sink[h] - mm, -80.0))
        }
        yo[bb * (nheads * hs) + h * hs + lid] = acc / l
    }
}

// Fused single-pass batched attention: beats llama.cpp fa_vec at every depth 512-16K. One
// simdgroup per (row, head, 32-slice-group), scores kept in-register (no divergent store), V
// accumulated lane-private (zero barriers). hs == 128 only; grid is HEAD-FASTEST for SLC locality.
class MetalSqAttnDF16 {
    @ssbo @binding = 0 @role = "read" kc : array<half4>
    @ssbo @binding = 1 @role = "read" vc : array<half4>
    @ssbo @binding = 2 @role = "read" q : array<float4>
    @ssbo @binding = 3 @role = "write" part : array<float>
    @ssbo @binding = 4 @role = "read" rt : array<uint4>
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
        let kb0 = rrow.x + lay * rrow.z   // lay = the layer's KV-row ELEMENT prefix (Σ kv_dim below l)
        let vb0 = rrow.y + lay * rrow.z
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
    @ssbo @binding = 0 @role = "read" kc : array<float4>
    @ssbo @binding = 1 @role = "read" vc : array<float4>
    @ssbo @binding = 2 @role = "read" q : array<float4>
    @ssbo @binding = 3 @role = "write" part : array<float>
    @ssbo @binding = 4 @role = "read" rt : array<uint4>
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
        let kb0 = rrow.x + lay * rrow.z   // lay = the layer's KV-row ELEMENT prefix (Σ kv_dim below l)
        let vb0 = rrow.y + lay * rrow.z
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
    @ssbo @binding = 0 @role = "read" part : array<float4>
    @ssbo @binding = 1 @role = "write" yo : array<float>
    @ssbo @binding = 2 @role = "read" rt : array<uint4>
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
    @ssbo @binding = 0 @role = "readwrite" qk : array<float>    // [B x qstride] fused QKV rows — q ropes in place
    @ssbo @binding = 1 @role = "write" kd : array<float16>  // the whole mirror arena, f16 view (k dest)
    @ssbo @binding = 2 @role = "write" vd : array<float16>
    @ssbo @binding = 3 @role = "weight" tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 4 @role = "weight" tsin : array<float>
    @ssbo @binding = 5 @role = "read" rt : array<uint4>
    @uniform @binding = 6 lay : uint
    @uniform @binding = 7 kv_dim : uint
    @uniform @binding = 8 qd : uint
    @uniform @binding = 9 head_size : uint
    @uniform @binding = 10 npairs_q : uint  // qd/2 — the q half of the pair grid
    @uniform @binding = 11 npairs : uint    // + kvd/2 — grid guard
    @uniform @binding = 12 qstride : uint
    @uniform @binding = 13 neox : uint      // pair (j, j+half) instead of (2j, 2j+1); same table row j
    @ssbo @binding = 14 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 15 has_bias : uint
    @uniform @binding = 16 rot : uint       // partial rotary width; == head_size for full

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
        let halfr = rot / 2u
        let isK = gid >= npairs_q
        let lgid = isK ? gid - npairs_q : gid
        let j = lgid % half
        let hbase = (lgid / half) * head_size
        // threads j < rot/2 rotate the (j, j+rot/2) pair; the rest identity-cover [rot, hs)
        let rotp = j < halfr
        let i = neox != 0u ? (rotp ? hbase + j : hbase + rot + (j - halfr)) : hbase + 2u * j
        let i2 = neox != 0u ? (rotp ? i + halfr : i + (half - halfr)) : i + 1u
        let fcr = rotp ? tcos[bb * halfr + j] : 1.0
        let fci = rotp ? tsin[bb * halfr + j] : 0.0
        if (isK) {
            let kdst = rrow.x + lay * rrow.z + (rrow.w - 1u) * kv_dim   // lay = element prefix
            let vdst = rrow.y + lay * rrow.z + (rrow.w - 1u) * kv_dim
            let v0 = qk[qbase + qd + i] + (has_bias != 0u ? bias[qd + i] : 0.0)
            let v1 = qk[qbase + qd + i2] + (has_bias != 0u ? bias[qd + i2] : 0.0)
            let r0 = v0 * fcr - v1 * fci
            let r1 = v0 * fci + v1 * fcr
            kd[kdst + i] = float16(min(max(r0, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
            kd[kdst + i2] = float16(min(max(r1, -65504.0), 65504.0))   // nolint:STYLE019
            let b0 = has_bias != 0u ? bias[qd + kv_dim + i] : 0.0
            let b1 = has_bias != 0u ? bias[qd + kv_dim + i2] : 0.0
            vd[vdst + i] = float16(min(max(qk[qbase + qd + kv_dim + i] + b0, -65504.0), 65504.0))   // nolint:STYLE019
            vd[vdst + i2] = float16(min(max(qk[qbase + qd + kv_dim + i2] + b1, -65504.0), 65504.0))   // nolint:STYLE019
        } else {
            let v0 = qk[qbase + i] + (has_bias != 0u ? bias[i] : 0.0)
            let v1 = qk[qbase + i2] + (has_bias != 0u ? bias[i2] : 0.0)
            qk[qbase + i] = v0 * fcr - v1 * fci
            qk[qbase + i2] = v0 * fci + v1 * fcr
        }
    }
}

// f32-mirror twin — straight stores, no clamp.
class MetalRopeStoreBF32 {
    @ssbo @binding = 0 @role = "readwrite" qk : array<float>
    @ssbo @binding = 1 @role = "write" kd : array<float>
    @ssbo @binding = 2 @role = "write" vd : array<float>
    @ssbo @binding = 3 @role = "weight" tcos : array<float>
    @ssbo @binding = 4 @role = "weight" tsin : array<float>
    @ssbo @binding = 5 @role = "read" rt : array<uint4>
    @uniform @binding = 6 lay : uint
    @uniform @binding = 7 kv_dim : uint
    @uniform @binding = 8 qd : uint
    @uniform @binding = 9 head_size : uint
    @uniform @binding = 10 npairs_q : uint
    @uniform @binding = 11 npairs : uint
    @uniform @binding = 12 qstride : uint
    @uniform @binding = 13 neox : uint
    @ssbo @binding = 14 @role = "weight" bias : array<float>
    @uniform @binding = 15 has_bias : uint
    @uniform @binding = 16 rot : uint       // partial rotary width; == head_size for full

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
        let halfr = rot / 2u
        let isK = gid >= npairs_q
        let lgid = isK ? gid - npairs_q : gid
        let j = lgid % half
        let hbase = (lgid / half) * head_size
        // threads j < rot/2 rotate the (j, j+rot/2) pair; the rest identity-cover [rot, hs)
        let rotp = j < halfr
        let i = neox != 0u ? (rotp ? hbase + j : hbase + rot + (j - halfr)) : hbase + 2u * j
        let i2 = neox != 0u ? (rotp ? i + halfr : i + (half - halfr)) : i + 1u
        let fcr = rotp ? tcos[bb * halfr + j] : 1.0
        let fci = rotp ? tsin[bb * halfr + j] : 0.0
        if (isK) {
            let kdst = rrow.x + lay * rrow.z + (rrow.w - 1u) * kv_dim   // lay = element prefix
            let vdst = rrow.y + lay * rrow.z + (rrow.w - 1u) * kv_dim
            let v0 = qk[qbase + qd + i] + (has_bias != 0u ? bias[qd + i] : 0.0)
            let v1 = qk[qbase + qd + i2] + (has_bias != 0u ? bias[qd + i2] : 0.0)
            kd[kdst + i] = v0 * fcr - v1 * fci
            kd[kdst + i2] = v0 * fci + v1 * fcr
            vd[vdst + i] = qk[qbase + qd + kv_dim + i] + (has_bias != 0u ? bias[qd + kv_dim + i] : 0.0)
            vd[vdst + i2] = qk[qbase + qd + kv_dim + i2] + (has_bias != 0u ? bias[qd + kv_dim + i2] : 0.0)
        } else {
            let v0 = qk[qbase + i] + (has_bias != 0u ? bias[i] : 0.0)
            let v1 = qk[qbase + i2] + (has_bias != 0u ? bias[i2] : 0.0)
            qk[qbase + i] = v0 * fcr - v1 * fci
            qk[qbase + i2] = v0 * fci + v1 * fcr
        }
    }
}

// q8_0-mirror partD twin (hs == 128 only, DF16 shape): one sg per (row, head, 32-position
// slice), online slice merge, byte-offset row table. K/V loads are 4 scalar int8s + block scale
// per position (34B blocks break half4 alignment); scale folds in before the simd_sum.
class MetalSqAttnDQ8 {
    @ssbo @binding = 0 @role = "read" ksh : array<float16>
    @ssbo @binding = 1 @role = "read" kqb : array<int8>
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<int8>
    @ssbo @binding = 4 @role = "read" q : array<float4>
    @ssbo @binding = 5 @role = "write" part : array<float>
    @ssbo @binding = 6 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
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
        let kb0b = rrow.x + lay * rrow.z   // lay = the layer's row-BYTE prefix
        let vb0b = rrow.y + lay * rrow.z
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
    @ssbo @binding = 0 @role = "readwrite" qk : array<float>    // [B x qstride] fused QKV rows — q ropes in place
    @ssbo @binding = 1 @role = "write" ksh : array<float16> // the whole mirror arena, half-scale view (k dest)
    @ssbo @binding = 2 @role = "write" kqb : array<int8>    // the SAME arena, byte view
    @ssbo @binding = 3 @role = "write" vsh : array<float16>
    @ssbo @binding = 4 @role = "write" vqb : array<int8>
    @ssbo @binding = 5 @role = "weight" tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 6 @role = "weight" tsin : array<float>
    @ssbo @binding = 7 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 8 lay : uint
    @uniform @binding = 9 nblk : uint       // kv_dim/32 — K blocks (V adds the same again)
    @uniform @binding = 10 qd : uint
    @uniform @binding = 11 head_size : uint
    @uniform @binding = 12 npairs_q : uint  // qd/2 — the q-rope half of the grid
    @uniform @binding = 13 qstride : uint
    @uniform @binding = 14 neox : uint      // pair (j, j+half) instead of (2j, 2j+1); same table row j
    @ssbo @binding = 15 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 16 has_bias : uint

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
            let hbase = (gid / half) * head_size
            let i = neox != 0u ? hbase + j : hbase + 2u * j
            let i2 = neox != 0u ? i + half : i + 1u
            let fcr = tcos[bb * half + j]
            let fci = tsin[bb * half + j]
            let v0 = qk[qbase + i] + (has_bias != 0u ? bias[i] : 0.0)
            let v1 = qk[qbase + i2] + (has_bias != 0u ? bias[i2] : 0.0)
            qk[qbase + i] = v0 * fcr - v1 * fci
            qk[qbase + i2] = v0 * fci + v1 * fcr
            return
        }
        let rowb = nblk * 34u
        let kvd = nblk * 32u
        let bi = gid - npairs_q
        let isV = bi >= nblk
        let blk = isV ? bi - nblk : bi
        let base = blk * 32u                // element base within the K/V row
        var vals : float[32]
        if (isV) {
            var e = 0u
            while (e < 32u) {
                // the raw v third of the QKV row
                vals[e] = qk[qbase + qd + kvd + base + e] + (has_bias != 0u ? bias[qd + kvd + base + e] : 0.0)
                e++
            }
        } elif (neox != 0u) {
            // NEOX pairs straddle 32-blocks (partner at ±half) — rope per ELEMENT; partner
            // reads come from qk, which no K/V thread writes, so cross-block reads are safe
            var e = 0u
            while (e < 32u) {
                let ge = base + e
                let hp = ge % head_size
                let isHi = hp >= half
                let j = isHi ? hp - half : hp
                let fcr = tcos[bb * half + j]
                let fci = tsin[bb * half + j]
                let oi = isHi ? ge - half : ge + half
                let mine = qk[qbase + qd + ge] + (has_bias != 0u ? bias[qd + ge] : 0.0)
                let other = qk[qbase + qd + oi] + (has_bias != 0u ? bias[qd + oi] : 0.0)
                vals[e] = isHi ? other * fci + mine * fcr : mine * fcr - other * fci
                e++
            }
        } else {
            var p = 0u
            while (p < 16u) {
                let ge = base + 2u * p
                let j = (ge % head_size) / 2u
                let fcr = tcos[bb * half + j]
                let fci = tsin[bb * half + j]
                let v0 = qk[qbase + qd + ge] + (has_bias != 0u ? bias[qd + ge] : 0.0)
                let v1 = qk[qbase + qd + ge + 1u] + (has_bias != 0u ? bias[qd + ge + 1u] : 0.0)
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
            let vb0b = rrow.y + lay * rrow.z + (rrow.w - 1u) * rowb   // lay = row-byte prefix
            vsh[vb0b / 2u + blk * 17u] = float16(d)
            k = 0u
            while (k < 32u) {
                vqb[vb0b + blk * 34u + 2u + k] = int8(round(vals[k] * id))
                k++
            }
        } else {
            let kb0b = rrow.x + lay * rrow.z + (rrow.w - 1u) * rowb
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
    @ssbo @binding = 0 @role = "read" ksh : array<float16>
    @ssbo @binding = 1 @role = "read" kqb : array<uint8>
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<uint8>
    @ssbo @binding = 4 @role = "read" q : array<float4>
    @ssbo @binding = 5 @role = "write" part : array<float>
    @ssbo @binding = 6 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
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
        let kb0b = rrow.x + lay * rrow.z   // lay = the layer's row-BYTE prefix
        let vb0b = rrow.y + lay * rrow.z
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
    @ssbo @binding = 0 @role = "readwrite" qk : array<float>    // [B x qstride] fused QKV rows — q ropes + rotates in place
    @ssbo @binding = 1 @role = "write" ksh : array<float16> // the whole mirror arena, half-scale view (k dest)
    @ssbo @binding = 2 @role = "write" kqb : array<uint8>   // the SAME arena, byte view
    @ssbo @binding = 3 @role = "write" vsh : array<float16>
    @ssbo @binding = 4 @role = "write" vqb : array<uint8>
    @ssbo @binding = 5 @role = "weight" tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 6 @role = "weight" tsin : array<float>
    @ssbo @binding = 7 @role = "weight" signs : array<float>
    @ssbo @binding = 8 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 9 lay : uint
    @uniform @binding = 10 qd : uint
    @uniform @binding = 11 head_size : uint
    @uniform @binding = 12 nheads : uint
    @uniform @binding = 13 kv_mul : uint
    @uniform @binding = 14 qstride : uint
    @uniform @binding = 15 neox : uint      // pair (j, j+half) instead of (2j, 2j+1); same table row j
    @ssbo @binding = 16 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 17 has_bias : uint
    @workgroup buf : float[512]     // <= max head_size (gemma4 global heads are 512)

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
        // q/k live in the fused QKV row; v is its third region (bbase = the bias-plane index)
        let bbase = isQ ? tgid * hs : (isK ? qd + hh * hs : qd + kv_dim + hh * hs)
        let base = qbase + bbase
        var v = 0.0
        if (isQ || isK) {
            let isHi = neox != 0u ? lid >= half : (lid & 1u) != 0u
            let j = neox != 0u ? (isHi ? lid - half : lid) : lid / 2u
            let fcr = tcos[bb * half + j]
            let fci = tsin[bb * half + j]
            let po = neox != 0u ? (isHi ? lid - half : lid + half) : (lid ^ 1u)
            let mine = qk[base + lid] + (has_bias != 0u ? bias[bbase + lid] : 0.0)
            let other = qk[base + po] + (has_bias != 0u ? bias[bbase + po] : 0.0)
            v = isHi ? other * fci + mine * fcr : mine * fcr - other * fci
        } else {
            v = qk[base + lid] + (has_bias != 0u ? bias[bbase + lid] : 0.0)
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
            let kb0b = rrow.x + lay * rrow.z + (rrow.w - 1u) * rowb   // lay = row-byte prefix
            if (sl == 0u) {
                ksh[kb0b / 2u + blk * 9u] = float16(d)
            }
            if ((lid & 1u) == 0u) {
                kqb[kb0b + blk * 18u + 2u + (lid % 32u) / 2u] = uint8(uint(qv) | (uint(qother) << 4u))
            }
        } else {
            let vb0b = rrow.y + lay * rrow.z + (rrow.w - 1u) * rowb
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
    @ssbo @binding = 0 @role = "readwrite" x : array<float>     // [B x dim] residual stream, add target
    @ssbo @binding = 1 @role = "read" a : array<float>     // [B x dim] addend
    @ssbo @binding = 2 @role = "weight" w : array<float>     // [dim] norm weight
    @ssbo @binding = 3 @role = "write" y : array<float>     // [B x dim] normed out (y may alias a)
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 eps : float
    @uniform @binding = 6 ascale : float    // (x + a) * ascale — gemma4 layer_out_scale scales
                                            // the WHOLE layer output; every other site binds 1.0
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
            let vv = (x[row * dim + k] + a[row * dim + k]) * ascale
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


// The decode PSO + resource globals moved to dasllama_metal_common (declared there so
// [metal_dispatch]-generated builders can see the pipeline they dispatch); the compile_pso()
// assignments below and the shutdown releases stay here.

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


[cold_path]   // one-time PSO/device bring-up, guarded by its own inited flag
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
    g_gemv_rows = clamp(int64(env_int("DASLLAMA_METAL_GEMV_TG", 4)), 1l, 32l)
    g_pso_rms = compile_pso(metal_rmsnorm_msl, metal_rmsnorm_msl_entry, metal_rmsnorm_msl_fastmath, ok)
    g_pso_add = compile_pso(metal_add_msl, metal_add_msl_entry, metal_add_msl_fastmath, ok)
    g_pso_addrms = compile_pso(metal_add_rms_msl, metal_add_rms_msl_entry, metal_add_rms_msl_fastmath, ok)
    g_pso_preaddrms = compile_pso(metal_pre_add_rms_msl, metal_pre_add_rms_msl_entry, metal_pre_add_rms_msl_fastmath, ok)
    g_pso_swiglu = compile_pso(metal_swiglu_msl, metal_swiglu_msl_entry, metal_swiglu_msl_fastmath, ok)
    g_one = metal_new_buffer(g_dev, 4ul)
    unsafe {
        var onep = reinterpret<float?>(metal_buffer_contents(g_one))
        onep[0] = 1.0
    }
    g_zero = metal_new_buffer(g_dev, 4ul)
    unsafe {
        var zp = reinterpret<uint?>(metal_buffer_contents(g_zero))
        zp[0] = 0u
    }
    g_pso_geglu = compile_pso(metal_geglu_msl, metal_geglu_msl_entry, metal_geglu_msl_fastmath, ok)
    g_pso_softcap = compile_pso(metal_softcap_row_msl, metal_softcap_row_msl_entry, metal_softcap_row_msl_fastmath, ok)
    g_pso_suppress = compile_pso(metal_suppress_row_msl, metal_suppress_row_msl_entry, metal_suppress_row_msl_fastmath, ok)
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
    g_pso_rpst_h16 = compile_pso(metal_rope_store_h16_msl, metal_rope_store_h16_msl_entry, metal_rope_store_h16_msl_fastmath, ok)
    g_pso_rpst32 = compile_pso(metal_rope_store32_msl, metal_rope_store32_msl_entry, metal_rope_store32_msl_fastmath, ok)
    g_pso_rpst_q8 = compile_pso(metal_rope_store_q8_msl, metal_rope_store_q8_msl_entry, metal_rope_store_q8_msl_fastmath, ok)
    g_pso_rpst_tq4 = compile_pso(metal_rope_store_tq4_msl, metal_rope_store_tq4_msl_entry, metal_rope_store_tq4_msl_fastmath, ok)
    g_pso_unsign = compile_pso(metal_fwht_unsign_msl, metal_fwht_unsign_msl_entry, metal_fwht_unsign_msl_fastmath, ok)
    g_pso_qknorm = compile_pso(metal_qk_norm_msl, metal_qk_norm_msl_entry, metal_qk_norm_msl_fastmath, ok)
    g_pso_argmax = compile_pso(metal_argmax_msl, metal_argmax_msl_entry, metal_argmax_msl_fastmath, ok)
    g_pso_moe_router = compile_pso(metal_moe_router_msl, metal_moe_router_msl_entry, metal_moe_router_msl_fastmath, ok)
    g_pso_moe_select = compile_pso(metal_moe_select_msl, metal_moe_select_msl_entry, metal_moe_select_msl_fastmath, ok)
    g_pso_moe_gemv_q8 = compile_pso(metal_moe_gemv_q8_msl, metal_moe_gemv_q8_msl_entry, metal_moe_gemv_q8_msl_fastmath, ok)
    g_pso_moe_kq_k4 = compile_pso(metal_moe_gemv_k4_msl, metal_moe_gemv_k4_msl_entry, metal_moe_gemv_k4_msl_fastmath, ok)
    g_pso_moe_kq_k5 = compile_pso(metal_moe_gemv_k5_msl, metal_moe_gemv_k5_msl_entry, metal_moe_gemv_k5_msl_fastmath, ok)
    g_pso_moe_kq_k6 = compile_pso(metal_moe_gemv_k6_msl, metal_moe_gemv_k6_msl_entry, metal_moe_gemv_k6_msl_fastmath, ok)
    g_pso_moe_combine = compile_pso(metal_moe_combine_msl, metal_moe_combine_msl_entry, metal_moe_combine_msl_fastmath, ok)
    g_pso_g4rnorm = compile_pso(metal_g4_router_norm_msl, metal_g4_router_norm_msl_entry, metal_g4_router_norm_msl_fastmath, ok)
    g_pso_moe_wscale = compile_pso(metal_moe_wscale_msl, metal_moe_wscale_msl_entry, metal_moe_wscale_msl_fastmath, ok)
    g_pso_swiglu_oai = compile_pso(metal_swiglu_oai_msl, metal_swiglu_oai_msl_entry, metal_swiglu_oai_msl_fastmath, ok)
    g_pso_moe_gemv_mx4 = compile_pso(metal_moe_gemv_mx4_msl, metal_moe_gemv_mx4_msl_entry, metal_moe_gemv_mx4_msl_fastmath, ok)
    g_pso_moe_gemv_q51 = compile_pso(metal_moe_gemv_q51_msl, metal_moe_gemv_q51_msl_entry, metal_moe_gemv_q51_msl_fastmath, ok)
    g_pso_dn_conv = compile_pso(metal_dn_conv_msl, metal_dn_conv_msl_entry, metal_dn_conv_msl_fastmath, ok)
    g_pso_dn_hist = compile_pso(metal_dn_conv_hist_msl, metal_dn_conv_hist_msl_entry, metal_dn_conv_hist_msl_fastmath, ok)
    g_pso_dn_l2 = compile_pso(metal_dn_l2norm_msl, metal_dn_l2norm_msl_entry, metal_dn_l2norm_msl_fastmath, ok)
    g_pso_dn_scan = compile_pso(metal_dn_scan_msl, metal_dn_scan_msl_entry, metal_dn_scan_msl_fastmath, ok)
    g_pso_dn_gate = compile_pso(metal_dn_gate_msl, metal_dn_gate_msl_entry, metal_dn_gate_msl_fastmath, ok)
    g_pso_dn_deint = compile_pso(metal_dn_deint_msl, metal_dn_deint_msl_entry, metal_dn_deint_msl_fastmath, ok)
    g_pso_sigmul = compile_pso(metal_sigmul_msl, metal_sigmul_msl_entry, metal_sigmul_msl_fastmath, ok)
    g_pso_axpy_sig = compile_pso(metal_axpy_sig_msl, metal_axpy_sig_msl_entry, metal_axpy_sig_msl_fastmath, ok)
    g_mx4_vtab = metal_new_buffer(g_dev, 64ul)
    unsafe {
        var vp = reinterpret<float?>(metal_buffer_contents(g_mx4_vtab))
        let mags = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0)
        for (vi in range(16)) {
            vp[vi] = vi < 8 ? mags[vi] : -mags[vi - 8]
        }
    }
    g_pso_copy_row = compile_pso(metal_copy_row_msl, metal_copy_row_msl_entry, metal_copy_row_msl_fastmath, ok)
    g_pso_emb = compile_pso(metal_embed_q8_msl, metal_embed_q8_msl_entry, metal_embed_q8_msl_fastmath, ok)
    g_pso_emb_k6 = compile_pso(metal_embed_k6_msl, metal_embed_k6_msl_entry, metal_embed_k6_msl_fastmath, ok)
    g_pso_qkvrs16 = compile_pso(metal_q8_gemv_qkv_rs16_msl, metal_q8_gemv_qkv_rs16_msl_entry, metal_q8_gemv_qkv_rs16_msl_fastmath, ok)
    g_pso_qkvrs32 = compile_pso(metal_q8_gemv_qkv_rs32_msl, metal_q8_gemv_qkv_rs32_msl_entry, metal_q8_gemv_qkv_rs32_msl_fastmath, ok)
    g_pso_w13sw = compile_pso(metal_q8_gemv_w13sw16_msl, metal_q8_gemv_w13sw16_msl_entry, metal_q8_gemv_w13sw16_msl_fastmath, ok)
    g_pso_kq_k4 = compile_pso(metal_kq_gemv_k4_msl, metal_kq_gemv_k4_msl_entry, metal_kq_gemv_k4_msl_fastmath, ok)
    g_pso_kq_k5 = compile_pso(metal_kq_gemv_k5_msl, metal_kq_gemv_k5_msl_entry, metal_kq_gemv_k5_msl_fastmath, ok)
    g_pso_kq_k5c = compile_pso(metal_kq_gemv_k5c_msl, metal_kq_gemv_k5c_msl_entry, metal_kq_gemv_k5c_msl_fastmath, ok)
    g_pso_kq_k6 = compile_pso(metal_kq_gemv_k6_msl, metal_kq_gemv_k6_msl_entry, metal_kq_gemv_k6_msl_fastmath, ok)
    g_pso_kq_mvb2_k4 = compile_pso(metal_kq_mvb2_k4_msl, metal_kq_mvb2_k4_msl_entry, metal_kq_mvb2_k4_msl_fastmath, ok)
    g_pso_kq_mvb4_k4 = compile_pso(metal_kq_mvb4_k4_msl, metal_kq_mvb4_k4_msl_entry, metal_kq_mvb4_k4_msl_fastmath, ok)
    g_pso_kq_mvb2_k5 = compile_pso(metal_kq_mvb2_k5_msl, metal_kq_mvb2_k5_msl_entry, metal_kq_mvb2_k5_msl_fastmath, ok)
    g_pso_kq_mvb4_k5 = compile_pso(metal_kq_mvb4_k5_msl, metal_kq_mvb4_k5_msl_entry, metal_kq_mvb4_k5_msl_fastmath, ok)
    g_pso_kq_mvb2_k6 = compile_pso(metal_kq_mvb2_k6_msl, metal_kq_mvb2_k6_msl_entry, metal_kq_mvb2_k6_msl_fastmath, ok)
    g_pso_kq_mvb4_k6 = compile_pso(metal_kq_mvb4_k6_msl, metal_kq_mvb4_k6_msl_entry, metal_kq_mvb4_k6_msl_fastmath, ok)
    g_pso_kq_mvb8_k4 = compile_pso(metal_kq_mvb8_k4_msl, metal_kq_mvb8_k4_msl_entry, metal_kq_mvb8_k4_msl_fastmath, ok)
    g_pso_kq_mvb8_k5 = compile_pso(metal_kq_mvb8_k5_msl, metal_kq_mvb8_k5_msl_entry, metal_kq_mvb8_k5_msl_fastmath, ok)
    g_pso_kq_mvb8_k6 = compile_pso(metal_kq_mvb8_k6_msl, metal_kq_mvb8_k6_msl_entry, metal_kq_mvb8_k6_msl_fastmath, ok)
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
    g_gemm_b_tensor = metal_tensor_crowned("gemmb_q8")
    g_pso_gemm_b = (g_gemm_b_tensor
        ? compile_pso(metal_q8_gemm_b_t_msl, metal_q8_gemm_b_t_msl_entry, metal_q8_gemm_b_t_msl_fastmath, ok)
        : compile_pso(metal_q8_gemm_b_msl, metal_q8_gemm_b_msl_entry, metal_q8_gemm_b_msl_fastmath, ok))
    g_gemm_b_sk_tensor = metal_tensor_crowned("gemmb_sk_q8")
    g_pso_gemm_b_sk = (g_gemm_b_sk_tensor
        ? compile_pso(metal_q8_gemm_b_sk_t_msl, metal_q8_gemm_b_sk_t_msl_entry, metal_q8_gemm_b_sk_t_msl_fastmath, ok)
        : compile_pso(metal_q8_gemm_b_sk_msl, metal_q8_gemm_b_sk_msl_entry, metal_q8_gemm_b_sk_msl_fastmath, ok))
    g_pso_skred = compile_pso(metal_sk_reduce_msl, metal_sk_reduce_msl_entry, metal_sk_reduce_msl_fastmath, ok)
    g_gemm64_b_tensor = metal_tensor_crowned("gemm64b_q8")
    g_pso_gemm64_b = (g_gemm64_b_tensor
        ? compile_pso(metal_q8_gemm64_b_t_msl, metal_q8_gemm64_b_t_msl_entry, metal_q8_gemm64_b_t_msl_fastmath, ok)
        : compile_pso(metal_q8_gemm64_b_msl, metal_q8_gemm64_b_msl_entry, metal_q8_gemm64_b_msl_fastmath, ok))
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

def enc_gemv(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64; bx, by, bn, bd : MetalBuffer?; d : int64; yoff : uint64 = 0ul) {
    enc_gemv_c(enc, bw, boff, bw, bx, by, yoff, bn, bd, d)
}

// K-quant site GEMV — the per-format twin of enc_gemv over the kq plane pair
// (kq_upload_quants + kq_upload_scales). k6 binds the scale buffer twice: the trailing packed
// f16 d plane at byte nsb*16, the 16B sub-scale blocks at 0. Geometry: 64-thread tg, 4 rows.
def enc_kq_gemv(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
                bs : MetalBuffer?; soff, doff : uint64; bx, by, bn, bd : MetalBuffer?;
                rows, n : int64; yoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    var groups = (rows + 3l) / 4l
    if (fmt == KqFmt.k6) {
        kn_pipeline(enc, g_pso_kq_k6)
        kn_buffer(enc, bs, doff, 0)   // the split plane's f16 d tail
    } elif (fmt == KqFmt.k5 && (n < 3072l || rows <= 1024l)) {
        // the select-form nr0=1 twin: wins every d at n < 3072 (gemv lab, 4B round) and the
        // small-d kv shape (b2r under-occupied); 2 rows/tg instead of b2r's 4
        kn_pipeline(enc, g_pso_kq_k5c)
        kn_buffer(enc, bs, soff, 0)
        groups = (rows + 1l) / 2l
    } else {
        kn_pipeline(enc, fmt == KqFmt.k4 ? g_pso_kq_k4 : g_pso_kq_k5)
        kn_buffer(enc, bs, soff, 0)
    }
    kn_buffer(enc, bs, soff, 1)
    kn_buffer(enc, bq, qoff, 2)
    kn_buffer(enc, bx, xoff, 3)
    kn_buffer(enc, by, yoff, 4)
    kn_buffer(enc, bn, 0ul, 5)
    kn_buffer(enc, bd, 0ul, 6)
    hz_read(bx, xoff, uint64(n * 4l))
    hz_write(by, yoff, uint64(rows * 4l))
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(groups), 1u, 1u), uint3(64u, 1u, 1u))
}

// K-quant small-batch mv (B = 2..8): R=4 column groups via grid.y, R=2 via the B2 PSO; B = 5..8
// rides the single-pass B8 twin (requires the mp == 8 pad the driver guarantees at nlive >= 5;
// DASLLAMA_METAL_KQ_B8=0 = the A/B rail). bys = y ROW-stride; bnr = live columns (stores gate).
var private g_kq_b8_mode = -1

[cold_path]   // one-time env resolution; kq_b8_on is a global test after the first call
def private load_kq_b8_mode() {
    g_kq_b8_mode = env_int("DASLLAMA_METAL_KQ_B8", 1) != 0 ? 1 : 0
}

def private kq_b8_on : bool {
    if (g_kq_b8_mode < 0) {
        load_kq_b8_mode()
    }
    return g_kq_b8_mode == 1
}

def enc_kq_mvb(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
               bs : MetalBuffer?; soff, doff : uint64; bx, by : MetalBuffer?; yoff : uint64;
               bn, bd, bys, bnr : MetalBuffer?; rows, nlive : int64) {
    let two = nlive == 2l
    let b8 = nlive >= 5l && kq_b8_on()
    if (fmt == KqFmt.k6) {
        kn_pipeline(enc, b8 ? g_pso_kq_mvb8_k6 : (two ? g_pso_kq_mvb2_k6 : g_pso_kq_mvb4_k6))
        if (b8) {
            kn_tgmem(enc, metal_kq_mvb8_k6_msl_tgmem, 0)
        }
        kn_buffer(enc, bs, doff, 0)
    } elif (fmt == KqFmt.k5) {
        kn_pipeline(enc, b8 ? g_pso_kq_mvb8_k5 : (two ? g_pso_kq_mvb2_k5 : g_pso_kq_mvb4_k5))
        if (b8) {
            kn_tgmem(enc, metal_kq_mvb8_k5_msl_tgmem, 0)
        }
        kn_buffer(enc, bs, soff, 0)
    } else {
        kn_pipeline(enc, b8 ? g_pso_kq_mvb8_k4 : (two ? g_pso_kq_mvb2_k4 : g_pso_kq_mvb4_k4))
        if (b8) {
            kn_tgmem(enc, metal_kq_mvb8_k4_msl_tgmem, 0)
        }
        kn_buffer(enc, bs, soff, 0)
    }
    kn_buffer(enc, bs, soff, 1)
    kn_buffer(enc, bq, qoff, 2)
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, yoff, 4)
    kn_buffer(enc, bn, 0ul, 5)
    kn_buffer(enc, bd, 0ul, 6)
    kn_buffer(enc, bys, 0ul, 7)
    kn_buffer(enc, bnr, 0ul, 8)
    let gcols = (two || b8) ? 1l : (nlive + 3l) / 4l
    hz_read(bx)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((rows + 7l) / 8l), uint(gcols), 1u), uint3(64u, 1u, 1u))
}

// the batched kq GEMM (B >= 9): the prefill-owned kq mul_mm twins over the padded x panel —
// enc_gemm_mm_b's shape (grid mp/32 x rows/64, y bound at the site's column byte offset)
def enc_kq_gemm_mm_b(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
                     bs : MetalBuffer?; soff, doff : uint64; bx, by : MetalBuffer?; yoff : uint64;
                     bk, bn : MetalBuffer?; mp, rows : int64) {
    if (fmt == KqFmt.k6) {
        kn_pipeline(enc, g_pso_kq_mm_b6)
        kn_tgmem(enc, metal_kq_mulmm_k6_msl_tgmem, 0)
        kn_buffer(enc, bs, doff, 0)
    } else {
        kn_pipeline(enc, fmt == KqFmt.k4 ? g_pso_kq_mm_b4 : g_pso_kq_mm_b5)
        kn_tgmem(enc, fmt == KqFmt.k4 ? metal_kq_mulmm_k4_msl_tgmem : metal_kq_mulmm_k5_msl_tgmem, 0)
        kn_buffer(enc, bs, soff, 0)
    }
    kn_buffer(enc, bs, soff, 1)
    kn_buffer(enc, bq, qoff, 2)
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, yoff, 4)
    kn_buffer(enc, bk, 0ul, 5)
    kn_buffer(enc, bn, 0ul, 6)
    hz_read(bx)
    hz_write(by, yoff, uint64(mp * rows * 4l))
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(mp / 32l), uint(rows / 64l), 1u), uint3(128u, 1u, 1u))
}

// One weight site's GEMV — a q8-tagged tensor rides the planar q8 kernels, a kq tensor the
// plane GEMVs (kq_quants_of/kq_scales_of + enc_kq_gemv). yoff places the k/v/w3 output segments.
def enc_site_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64; bx, by, bn, bd : MetalBuffer?; yoff : uint64 = 0ul) {
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, woff)
        enc_gemv(enc, bw.buf, bw.boff, bx, by, bn, bd, rows, yoff)
    } else {
        let bq = kq_quants_of(g_dev, t, fmt, woff)
        let bs = kq_scales_of(g_dev, t, fmt, woff)
        enc_kq_gemv(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, bn, bd, rows, n, yoff)
    }
}

// One kq weight site in a batched step. B <= 8: nrows separate single-stream plane GEMVs (the
// kq GEMV is ALU-bound, so this matches the lcpp kq batch wall). B >= 9: the kq mul_mm twin over
// the M-padded x panel. Disjoint y rows/columns: the caller's one barrier covers the whole group.
def enc_kq_site_b(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64;
                  bx : MetalBuffer?; xstride : int64; by : MetalBuffer?; ystride, ycol : int64;
                  bn, bd, bys, bnr : MetalBuffer?; nrows, mp : int64) {
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    if (nrows > 8l) {
        enc_kq_gemm_mm_b(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, uint64(ycol * 4l), bn, bys, mp, rows)
        return
    }
    // B = 2..8: the ext small-batch twins — one weight pass per R-column group instead of B per-stream passes (-25..-40% at the 3B sites)
    if (nrows >= 2l && bnr != null) {
        assert(xstride == n)
        enc_kq_mvb(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, uint64(ycol * 4l), bn, bd, bys, bnr, rows, nrows)
        return
    }
    for (b in range64(nrows)) {
        enc_kq_gemv(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, bn, bd, rows, n,
            uint64((b * ystride + ycol) * 4l), uint64(b * xstride * 4l))
    }
}

// fused rope + KV-row store: the v source binds the SAME QKV buffer at its v byte offset
def enc_rope_store(enc : MetalComputeEncoder?; f16 : bool; bqkv : MetalBuffer?; vsoff : uint64;
                   kmb, vmb : MetalBuffer?; koff, voff : uint64;
                   bcos, bsin, bqd, bhs, bnpq, bnpall, bneox, bbias, bhasb : MetalBuffer?; npairs : int64;
                   brot : MetalBuffer? = null) {
    // rot defaults to head_size (full rope)
    if (f16) {
        enc_rpst16_c(enc, bqkv, bqkv, vsoff, kmb, koff, vmb, voff, bcos, bsin,
            bqd, bhs, bnpq, bnpall, bneox, bbias, bhasb, brot != null ? brot : bhs, npairs)
    } else {
        enc_rpst32_c(enc, bqkv, bqkv, vsoff, kmb, koff, vmb, voff, bcos, bsin,
            bqd, bhs, bnpq, bnpall, bneox, bbias, bhasb, brot != null ? brot : bhs, npairs)
    }
}

// the head-cooperative fused rope-store (QK-norm models, f16 mirror): grid = nheads + 2*nkvh
// threadgroups of head_size threads
def enc_rope_store_h16(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                       kmb, vmb : MetalBuffer?; koff, voff : uint64;
                       bcos, bsin, bqkw, bbias, bqd, bhs, bnh, bkvm, bneox, bhasb, bhasn, beps : MetalBuffer?;
                       ntg, head_size : int64; brot : MetalBuffer? = null) {
    // rot defaults to head_size (full rope)
    enc_rpst_h16_c(enc, bqkv, bqkv, vsoff, kmb, koff, vmb, voff, bcos, bsin, bqkw, bbias,
        bqd, bhs, bnh, bkvm, bneox, bhasb, bhasn, beps, brot != null ? brot : bhs, ntg, head_size)
}

// The tq4 rotation signs, uploaded once at max head size (smaller heads read a prefix — the
// deterministic-seed contract with the CPU codec).

def enc_rope_store_tq4(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                       kmb, vmb : MetalBuffer?; koff, voff : uint64;
                       bcos, bsin, bqd, bhs, bnh, bkvm, bneox, bbias, bhasb : MetalBuffer?; ntg, head_size : int64) {
    enc_rpst_tq4_c(enc, bqkv, bqkv, vsoff, kmb, koff, kmb, vmb, voff, vmb, bcos, bsin, signs_buffer(),
        bqd, bhs, bnh, bkvm, bneox, bbias, bhasb, ntg, head_size)
}

// the tq4 attention-output un-rotate: one threadgroup per (head, row), row stride = qstride
def enc_fwht_unsign(enc : MetalComputeEncoder?; byo, bhs, bqs : MetalBuffer?; nheads, nrows, head_size : int64) {
    enc_unsign_c(enc, byo, signs_buffer(), bhs, bqs, nheads, nrows, head_size)
}

// q8_0-mirror rope+store: the K/V mirror rows bind TWICE each (half-scale + byte views at the
// same row offset); grid covers the q pairs + one thread per K/V 32-block
def enc_rope_store_q8(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                      kmb, vmb : MetalBuffer?; koff, voff : uint64;
                      bcos, bsin, bqd, bhs, bnpq, bnblk, bneox, bbias, bhasb : MetalBuffer?; nthreads : int64) {
    enc_rpst_q8_c(enc, bqkv, bqkv, vsoff, kmb, koff, kmb, vmb, voff, vmb, bcos, bsin,
        bqd, bhs, bnpq, bnblk, bneox, bbias, bhasb, nthreads)
}

// the fused QKV+rope+store dispatch (2 rows per simdgroup — grid covers ddim/2 pairs)
def enc_gemv_qkv_rs(enc : MetalComputeEncoder?; f16 : bool; bw : MetalBuffer?; wqoff, wkoff, wvoff : uint64;
                    bx, bq : MetalBuffer?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                    bcos, bsin, bn, bd, bqd, bkvd, bhs, bneox, bbias, bhasb : MetalBuffer?; d : int64) {
    let pairs = d / 2l
    if (f16) {
        enc_qkvrs16_c(enc, bw, wqoff, bw, bx, bq, kmb, koff, vmb, voff, bcos, bsin,
            bn, bd, bqd, bkvd, bhs, bneox, bbias, bhasb, bw, wkoff, bw, bw, wvoff, bw, pairs)
    } else {
        enc_qkvrs32_c(enc, bw, wqoff, bw, bx, bq, kmb, koff, vmb, voff, bcos, bsin,
            bn, bd, bqd, bkvd, bhs, bneox, bbias, bhasb, bw, wkoff, bw, bw, wvoff, bw, pairs)
    }
}

// the fused W1|W3 + silu/gelu dispatch (2 rows per simdgroup — gate + up of one output element; bact picks the activation)
def enc_gemv_w13sw(enc : MetalComputeEncoder?; bw : MetalBuffer?; w1off, w3off : uint64; bx, bh : MetalBuffer?; bn, bhid : MetalBuffer?; hidden : int64; bact : MetalBuffer?) {
    enc_w13sw_c(enc, bw, w1off, bw, bx, bh, bn, bhid, bw, w3off, bw, bact, hidden)
}

def enc_embed(enc : MetalComputeEncoder?; bw : MetalBuffer?; eoff : uint64; btok, bx, bdim, bescale : MetalBuffer?; dim : int64) {
    enc_embed_c(enc, bw, eoff, bw, btok, bx, bdim, bescale != null ? bescale : g_one, dim)
}

// the tied-k6 spec embed gather: the scale buffer binds twice (f16 d plane @doff + sub-scale
// strips @soff — kq_scales_of's whole-plane offsets), the quant plane once @qoff
def enc_embed_k6(enc : MetalComputeEncoder?; bq : MetalBuffer?; qoff : uint64; bs : MetalBuffer?; soff, doff : uint64; btok, bx, bdim, bescale : MetalBuffer?; dim : int64) {
    enc_embed_k6_c(enc, bs, doff, bs, soff, bq, qoff, btok, bx, bdim, bescale != null ? bescale : g_one, dim)
}

// ===== MoE routing encoders (Wave C) =====

// enc_moe_router / enc_moe_select / enc_moe_combine are [metal_dispatch]-generated from their
// prefill classes (the selection buffer's idx/w views are the seli/selw fields — callers pass
// the same buffer twice)

// one expert-indexed weight site (gate/up/down): weight binds at the layer's expert STACK base,
// grid.y = k slots, grid.z = streams; bes = blocks-per-expert (q8 ege/32, kq ege/256), bxs =
// per-slot x stride, bss/bbxs/bbys = per-stream sel/x/y strides (single decode binds zeros)
def enc_moe_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, k, nst : int64;
                 bx, by, bn, bd, bsel, bes, bxs, bss, bbxs, bbys : MetalBuffer?; yoff : uint64 = 0ul) {
    hz_read(bx)
    hz_read(bsel)
    hz_write(by, yoff, uint64(rows * k * nst * 4l))
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, stack_off)
        kn_pipeline(enc, g_pso_moe_gemv_q8)
        kn_buffer(enc, bw.buf, bw.boff, 0)
        kn_buffer(enc, bw.buf, bw.boff, 1)
        kn_buffer(enc, bx, 0ul, 2)
        kn_buffer(enc, by, yoff, 3)
        kn_buffer(enc, bn, 0ul, 4)
        kn_buffer(enc, bd, 0ul, 5)
        kn_buffer(enc, bsel, 0ul, 6)
        kn_buffer(enc, bes, 0ul, 7)
        kn_buffer(enc, bxs, 0ul, 8)
        kn_buffer(enc, bss, 0ul, 9)
        kn_buffer(enc, bbxs, 0ul, 10)
        kn_buffer(enc, bbys, 0ul, 11)
        hz_gate(enc)
        kn_dispatch(enc, uint3(uint((rows + g_gemv_rows - 1l) / g_gemv_rows), uint(k), uint(nst)), uint3(uint(g_gemv_rows * 32l), 1u, 1u))
    } elif (fmt == KqFmt.q51) {
        let qp = q51_planes_of(g_dev, t, stack_off)
        kn_pipeline(enc, g_pso_moe_gemv_q51)
        kn_buffer(enc, qp.qbuf, qp.qoff, 0)
        kn_buffer(enc, qp.sbuf, qp.soff, 1)
        kn_buffer(enc, bx, 0ul, 2)
        kn_buffer(enc, by, yoff, 3)
        kn_buffer(enc, bn, 0ul, 4)
        kn_buffer(enc, bd, 0ul, 5)
        kn_buffer(enc, bsel, 0ul, 6)
        kn_buffer(enc, bes, 0ul, 7)
        kn_buffer(enc, bxs, 0ul, 8)
        kn_buffer(enc, bss, 0ul, 9)
        kn_buffer(enc, bbxs, 0ul, 10)
        kn_buffer(enc, bbys, 0ul, 11)
        hz_gate(enc)
        kn_dispatch(enc, uint3(uint((rows + 3l) / 4l), uint(k), uint(nst)), uint3(64u, 1u, 1u))
    } else {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        if (fmt == KqFmt.k6) {
            kn_pipeline(enc, g_pso_moe_kq_k6)
            kn_buffer(enc, bs.buf, bs.doff, 0)
        } else {
            kn_pipeline(enc, fmt == KqFmt.k4 ? g_pso_moe_kq_k4 : g_pso_moe_kq_k5)
            kn_buffer(enc, bs.buf, bs.soff, 0)
        }
        kn_buffer(enc, bs.buf, bs.soff, 1)
        kn_buffer(enc, bq.buf, bq.qoff, 2)
        kn_buffer(enc, bx, 0ul, 3)
        kn_buffer(enc, by, yoff, 4)
        kn_buffer(enc, bn, 0ul, 5)
        kn_buffer(enc, bd, 0ul, 6)
        kn_buffer(enc, bsel, 0ul, 7)
        kn_buffer(enc, bes, 0ul, 8)
        kn_buffer(enc, bxs, 0ul, 9)
        kn_buffer(enc, bss, 0ul, 10)
        kn_buffer(enc, bbxs, 0ul, 11)
        kn_buffer(enc, bbys, 0ul, 12)
        hz_gate(enc)
        kn_dispatch(enc, uint3(uint((rows + 3l) / 4l), uint(k), uint(nst)), uint3(64u, 1u, 1u))
    }
}

// the MXFP4 expert site (gpt-oss): the mx4 planes bind at the layer stack's byte offsets;
// bwb/bhasb = the layer's per-expert bias slab (identity g_one + g_zero when absent)
def enc_moe_gemv_mx4(enc : MetalComputeEncoder?; t : Model; stack_off, rows, k, nst : int64;
                     bx, by, bn, bd, bsel, beblk, bxs, bss, bbxs, bbys, bwb, bhasb : MetalBuffer?; yoff : uint64 = 0ul) {
    let mp = mx4_of(g_dev, t, stack_off)
    kn_pipeline(enc, g_pso_moe_gemv_mx4)
    kn_tgmem(enc, metal_moe_gemv_mx4_msl_tgmem, 0)
    kn_buffer(enc, mp.qbuf, mp.qoff, 0)
    kn_buffer(enc, mp.sbuf, mp.soff, 1)
    kn_buffer(enc, bx, 0ul, 2)
    kn_buffer(enc, by, yoff, 3)
    kn_buffer(enc, bn, 0ul, 4)
    kn_buffer(enc, bd, 0ul, 5)
    kn_buffer(enc, bsel, 0ul, 6)
    kn_buffer(enc, beblk, 0ul, 7)
    kn_buffer(enc, bxs, 0ul, 8)
    kn_buffer(enc, bss, 0ul, 9)
    kn_buffer(enc, bbxs, 0ul, 10)
    kn_buffer(enc, bbys, 0ul, 11)
    kn_buffer(enc, bwb, 0ul, 12)
    kn_buffer(enc, bhasb, 0ul, 13)
    kn_buffer(enc, g_mx4_vtab, 0ul, 15)
    hz_read(bx)
    hz_read(bsel)
    hz_write(by, yoff, uint64(rows * k * nst * 4l))
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((rows + 3l) / 4l), uint(k), uint(nst)), uint3(64u, 1u, 1u))
}

// fused add + rms when dim fits the kernel's row slab, the unfused pair otherwise
def enc_add_rms_site(enc : MetalComputeEncoder?; bx, ba, bw, by, bdim, beps : MetalBuffer?; dim : int64; bascale : MetalBuffer? = null) {
    if (dim <= ADD_RMS_MAX_DIM) {
        enc_addrms_c(enc, bx, ba, bw, by, bdim, beps, bascale != null ? bascale : g_one)
    } else {
        enc_ew2(enc, g_pso_add, bx, 0ul, ba, 0ul, bdim, dim, bascale)
        enc_rms(enc, bx, bw, by, bdim, beps)
    }
}

// Fused post-attn-norm + residual add + ffn-norm for pre_post_norm models: a = rms(a)·wpa in place,
// x += a, y = rms(x)·wffn — one dispatch collapsing enc_rms(a) + enc_add_rms_site. Falls back to the
// separate pair past the slab cap. `ba` is normed in place (matches the un-fused enc_rms(a) write).
def enc_pre_add_rms_site(enc : MetalComputeEncoder?; bx, ba, bwpa, bwffn, by, bdim, beps : MetalBuffer?; dim : int64) {
    if (dim <= ADD_RMS_MAX_DIM) {
        enc_preaddrms_c(enc, bx, ba, bwpa, bwffn, by, bdim, beps)
    } else {
        enc_rms(enc, ba, bwpa, ba, bdim, beps)
        enc_add_rms_site(enc, bx, ba, bwffn, by, bdim, beps, dim)
    }
}

def enc_attn(enc : MetalComputeEncoder?; f16 : bool; kmb, vmb : MetalBuffer?; koff, voff : uint64;
             bq, by, bcnt, bkvd, bnh, bkvm, bhs, bscale, bcap : MetalBuffer?; nheads, head_size : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null) {
    if (f16) {
        enc_attn16_c(enc, kmb, koff, vmb, voff, bq, by, bcnt, bkvd, bnh, bkvm, bhs, bscale, bcap,
            bsink != null ? bsink : g_one, bhass != null ? bhass : g_zero, nheads, head_size)
    } else {
        enc_attn32_c(enc, kmb, koff, vmb, voff, bq, by, bcnt, bkvd, bnh, bkvm, bhs, bscale, bcap,
            bsink != null ? bsink : g_one, bhass != null ? bhass : g_zero, nheads, head_size)
    }
}

def enc_attn_part(enc : MetalComputeEncoder?; f16 : bool; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                  bq, bpart, bcnt, bkvd, bnh, bkvm, bhs, bscale, bnch, bcap, bwin : MetalBuffer?; ntg, head_size : int64) {
    if (f16) {
        enc_attnp16_c(enc, kmb, koff, vmb, voff, bq, bpart, bcnt, bkvd, bnh, bkvm, bhs, bscale,
            bnch, bcap, bwin, ntg, head_size)
    } else {
        enc_attnp32_c(enc, kmb, koff, vmb, voff, bq, bpart, bcnt, bkvd, bnh, bkvm, bhs, bscale,
            bnch, bcap, bwin, ntg, head_size)
    }
}

// tq4-mirror attention twins — same dual-view shape as the q8_0 encoders below
def enc_attn_tq4(enc : MetalComputeEncoder?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                 bq, by, bcnt, bnblk, bnh, bkvm, bhs, bscale, bcap : MetalBuffer?; nheads, head_size : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null) {
    enc_attn_tq4_c(enc, kmb, koff, kmb, vmb, voff, vmb, bq, by, bcnt, bnblk, bnh, bkvm, bhs, bscale, bcap,
        bsink != null ? bsink : g_one, bhass != null ? bhass : g_zero, nheads, head_size)
}

def enc_attn_part_tq4(enc : MetalComputeEncoder?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                      bq, bpart, bcnt, bnblk, bnh, bkvm, bhs, bscale, bnch, bcap, bwin : MetalBuffer?; ntg, head_size : int64) {
    enc_attnp_tq4_c(enc, kmb, koff, kmb, vmb, voff, vmb, bq, bpart, bcnt, bnblk, bnh, bkvm, bhs, bscale,
        bnch, bcap, bwin, ntg, head_size)
}

// q8_0-mirror attention twins: the K/V slabs bind TWICE each (half-scale + byte views at the
// same layer offset); nblk (blocks per row) replaces the kv_dim uniform
def enc_attn_q8(enc : MetalComputeEncoder?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                bq, by, bcnt, bnblk, bnh, bkvm, bhs, bscale, bcap : MetalBuffer?; nheads, head_size : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null) {
    enc_attn_q8_c(enc, kmb, koff, kmb, vmb, voff, vmb, bq, by, bcnt, bnblk, bnh, bkvm, bhs, bscale, bcap,
        bsink != null ? bsink : g_one, bhass != null ? bhass : g_zero, nheads, head_size)
}

def enc_attn_part_q8(enc : MetalComputeEncoder?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                     bq, bpart, bcnt, bnblk, bnh, bkvm, bhs, bscale, bnch, bcap, bwin : MetalBuffer?; ntg, head_size : int64) {
    enc_attnp_q8_c(enc, kmb, koff, kmb, vmb, voff, vmb, bq, bpart, bcnt, bnblk, bnh, bkvm, bhs, bscale,
        bnch, bcap, bwin, ntg, head_size)
}

def enc_attn_comb(enc : MetalComputeEncoder?; bpart, by, bnch, bnh, bhs, bchlo : MetalBuffer?; nheads, head_size : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null) {
    enc_attncomb_c(enc, bpart, by, bnch, bnh, bhs, bchlo,
        bsink != null ? bsink : g_one, bhass != null ? bhass : g_zero, nheads, head_size)
}

// swiglu and add share the (a, b, total) elementwise shape; offsets serve the fused-buffer binds
def enc_ew2(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; ba : MetalBuffer?; aoff : uint64; bb : MetalBuffer?; boff : uint64; btot : MetalBuffer?; total : int64; bascale : MetalBuffer? = null) {
    kn_pipeline(enc, pso)
    kn_buffer(enc, ba, aoff, 0)
    kn_buffer(enc, bb, boff, 1)
    kn_buffer(enc, btot, 0ul, 2)
    kn_buffer(enc, bascale != null ? bascale : g_one, 0ul, 3)   // the add's ascale slot (unused by swiglu/geglu)
    hz_read(ba, aoff, uint64(total * 4l))
    hz_read(bb, boff, uint64(total * 4l))
    hz_write(ba, aoff, uint64(total * 4l))
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((total + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// ===== the batched-step encoders (P4) =====

// fixed-B batched GEMV (B2 serves B <= 2, B4 3..4 — pad streams read stale rows, outputs never
// consumed); the blob slice binds through both views at the tensor's byte offset. Pinned at
// 4 rows/tg — the x-staged cooperative-load slot math bakes 128 threads in.
def enc_gemv_b(enc : MetalComputeEncoder?; four : bool; bw : MetalBuffer?; boff : uint64;
               bx, by, bn, bd, bys : MetalBuffer?; d : int64; yoff : uint64 = 0ul) {
    kn_pipeline(enc, four ? g_pso_gemv_b4 : g_pso_gemv_b2)
    kn_tgmem(enc, four ? metal_gemv_b4_msl_tgmem : metal_gemv_b2_msl_tgmem, 0)
    kn_buffer(enc, bw, boff, 0)   // byte view
    kn_buffer(enc, bw, boff, 2)   // half-scale view
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, yoff, 4)
    kn_buffer(enc, bn, 0ul, 5)
    kn_buffer(enc, bd, 0ul, 6)
    kn_buffer(enc, bys, 0ul, 7)
    hz_read(bx)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((d + 3l) / 4l), 1u, 1u), uint3(128u, 1u, 1u))
}

// the mul_mv-shape fixed-B GEMV on an interleaved 34B blob (dual views of the SAME buffer);
// bxs = x row stride in float4s; bnr = the nrows uniform, four-form only; xoff/yoff = the
// column-tile byte offsets (B > 4 streams the weights once per 4-column tile)
def enc_mv_b(enc : MetalComputeEncoder?; four : bool; bwblob : MetalBuffer?; wboff : uint64;
             bx, by, bn, bd, bxs, bys : MetalBuffer?; d : int64;
             bnr : MetalBuffer? = null; xoff : uint64 = 0ul; yoff : uint64 = 0ul) {
    kn_pipeline(enc, four ? g_pso_mv_b4 : g_pso_mv_b2)
    kn_buffer(enc, bwblob, wboff, 0)
    kn_buffer(enc, bwblob, wboff, 1)
    kn_buffer(enc, bx, xoff, 2)
    kn_buffer(enc, by, yoff, 3)
    kn_buffer(enc, bn, 0ul, 4)
    kn_buffer(enc, bd, 0ul, 5)
    if (four) {
        kn_buffer(enc, bnr, 0ul, 6)
        kn_buffer(enc, bxs, 0ul, 7)
        kn_buffer(enc, bys, 0ul, 8)
    } else {
        kn_buffer(enc, bxs, 0ul, 6)
        kn_buffer(enc, bys, 0ul, 7)
    }
    // b2 = 4 rows/tg (16 lanes/row); b4 = 8 rows/tg (8 lanes/row)
    hz_read(bx)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(four ? (d + 7l) / 8l : (d + 3l) / 4l), 1u, 1u), uint3(64u, 1u, 1u))
}

// one mv GEMV site: a single form dispatch at B <= 4, two 4-column tiles at B = 5..8 (the
// reference's own shape — it never switches to GEMM below B = 9; the weights stream twice but
// stay the fast skinny form). xrowb = the x row stride in bytes; bnr/bnr2 = per-tile nrows.
def enc_mv_site(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                bx, by, bn, bd, bxs, bys : MetalBuffer?;
                d, nrows, ys : int64; xrowb : uint64; bnr, bnr2 : MetalBuffer?;
                yoff : uint64 = 0ul) {
    enc_mv_b(enc, nrows > 2l, bwblob, wboff, bx, by, bn, bd, bxs, bys, d, bnr, 0ul, yoff)
    if (nrows > 4l) {
        // the rows-5..8 dispatch offsets y by 4 rows of the SITE's y row stride (the ys
        // uniform's value — fused-buffer segment sites included)
        enc_mv_b(enc, true, bwblob, wboff, bx, by, bn, bd, bxs, bys, d, bnr2, 4ul * xrowb, yoff + uint64(4l * ys * 4l))
    }
}

// fused fixed-B W1|W3 GEMV + swiglu — the w1 and w3 blob slices bind separately (kind-major
// layout), by is the [B x hidden] swiglu output. Replaces the w1 + w3 GEMVs + the swiglu ew2
// with one dispatch. Same 4-rows/tg geometry, gridded over the hidden output rows.
def enc_gemv_w13sw_b(enc : MetalComputeEncoder?; four : bool; bw : MetalBuffer?; w1off, w3off : uint64;
                     bx, by, bn, bhid : MetalBuffer?; hidden : int64) {
    kn_pipeline(enc, four ? g_pso_w13sw_b4 : g_pso_w13sw_b2)
    kn_tgmem(enc, four ? metal_gemv_w13sw_b4_msl_tgmem : metal_gemv_w13sw_b2_msl_tgmem, 0)
    kn_buffer(enc, bw, w1off, 0)   // w1: byte view
    kn_buffer(enc, bw, w1off, 2)   // w1: half-scale view
    kn_buffer(enc, bw, w3off, 8)   // w3: byte view
    kn_buffer(enc, bw, w3off, 9)   // w3: half-scale view
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, 0ul, 4)
    kn_buffer(enc, bn, 0ul, 5)
    kn_buffer(enc, bhid, 0ul, 6)
    hz_read(bx)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((hidden + 3l) / 4l), 1u, 1u), uint3(128u, 1u, 1u))
}

// the production mul_mm dispatch (the prefill kernel: 34B-blob dual-view W, raw f32 X): y binds
// at a byte offset so per-tensor tiles land inside a wider fused row — the ndim uniform is the
// y ROW STRIDE while this tensor's output width d shapes the grid (d % 64 == 0, mp % 32 == 0)
def enc_gemm_mm_b(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                  bx, by : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, d : int64) {
    kn_pipeline(enc, g_pso_mm)
    kn_tgmem(enc, metal_q8_mulmm_msl_tgmem, 0)
    kn_buffer(enc, bwblob, wboff, 0)
    kn_buffer(enc, bwblob, wboff, 1)
    kn_buffer(enc, bx, 0ul, 2)
    kn_buffer(enc, by, yoff, 3)
    kn_buffer(enc, bk, 0ul, 4)
    kn_buffer(enc, bn, 0ul, 5)
    hz_read(bx)
    hz_write(by, yoff, uint64(mp * d * 4l))
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(mp / 32l), uint(d / 64l), 1u), uint3(128u, 1u, 1u))
}

// the M-pad-32 GEMM (B >= 5): raw f32 X, dual-view blob W at the tensor's byte offset
def enc_gemm_b(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64;
               bx, by, bk, bn, bys : MetalBuffer?; mp32, d : int64; yoff : uint64 = 0ul) {
    kn_pipeline(enc, g_pso_gemm_b)
    if (!g_gemm_b_tensor) {
        kn_tgmem(enc, metal_q8_gemm_b_msl_tgmem, 0)
    }
    kn_buffer(enc, bw, boff, 0)   // byte view
    kn_buffer(enc, bw, boff, 2)   // half-scale view
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, yoff, 4)
    kn_buffer(enc, bk, 0ul, 5)
    kn_buffer(enc, bn, 0ul, 6)
    kn_buffer(enc, bys, 0ul, 7)
    hz_read(bx)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((mp32 / 32l) * (d / 32l)), 1u, 1u), uint3(128u, 1u, 1u))
}

// the split-K pair (per-site policy): the GEMM's grid.y is the k-slice axis, partial planes land
// in bpart, and the reduce sums them into y — the serial encoder orders the two dispatches.
// Planes and grids assume mp == 32 (the policy gate).
def enc_gemm_sk_b(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64;
                  bx, bpart, bk, bn, bsk : MetalBuffer?; d, nsk : int64) {
    kn_pipeline(enc, g_pso_gemm_b_sk)
    if (!g_gemm_b_sk_tensor) {
        kn_tgmem(enc, metal_q8_gemm_b_sk_msl_tgmem, 0)
    }
    kn_buffer(enc, bw, boff, 0)   // byte view
    kn_buffer(enc, bw, boff, 2)   // half-scale view
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, bpart, 0ul, 4)
    kn_buffer(enc, bk, 0ul, 5)
    kn_buffer(enc, bn, 0ul, 6)
    kn_buffer(enc, bsk, 0ul, 8)
    hz_read(bx)
    hz_write(bpart)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(d / 32l), uint(nsk), 1u), uint3(128u, 1u, 1u))
}

def enc_sk_reduce_b(enc : MetalComputeEncoder?; bpart, by, btot, bsk : MetalBuffer?; total : int64) {
    kn_pipeline(enc, g_pso_skred)
    kn_buffer(enc, bpart, 0ul, 0)
    kn_buffer(enc, by, 0ul, 1)
    kn_buffer(enc, btot, 0ul, 2)
    kn_buffer(enc, bsk, 0ul, 3)
    hz_read(bpart)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((total + 127l) / 128l), 1u, 1u), uint3(128u, 1u, 1u))
}

// the 64-wide N tile (classifier site): the caller gates d % 64 == 0
def enc_gemm64_b(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64;
                 bx, by, bk, bn : MetalBuffer?; mp32, d : int64) {
    kn_pipeline(enc, g_pso_gemm64_b)
    if (!g_gemm64_b_tensor) {
        kn_tgmem(enc, metal_q8_gemm64_b_msl_tgmem, 0)
    }
    kn_buffer(enc, bw, boff, 0)   // byte view
    kn_buffer(enc, bw, boff, 2)   // half-scale view
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, 0ul, 4)
    kn_buffer(enc, bk, 0ul, 5)
    kn_buffer(enc, bn, 0ul, 6)
    hz_read(bx)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((mp32 / 32l) * (d / 64l)), 1u, 1u), uint3(128u, 1u, 1u))
}

// Per-encoder buffer-hazard tracker for concurrent encoding (g_bconcurrent): each scratch buffer
// owns a bit; a barrier is inserted iff a dispatch's read hits a pending write (RAW), or its
// write hits a pending write/read (WAW/WAR) — read-after-read runs concurrently.

def enc_attn_b(enc : MetalComputeEncoder?; f16 : bool; bq, by, brt, blay, bkvd, bnh, bkvm, bhs, bscale, bqs, bcap : MetalBuffer?; nrows, nheads, head_size : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null; rtoff : uint64 = 0ul) {
    kn_pipeline(enc, f16 ? g_pso_attnb16 : g_pso_attnb32)
    kn_tgmem(enc, f16 ? metal_sq_attn_b16_msl_tgmem : metal_sq_attn_b32_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, bq, 0ul, 2)
    kn_buffer(enc, by, 0ul, 3)
    kn_buffer(enc, brt, rtoff, 4)
    kn_buffer(enc, blay, 0ul, 5)
    kn_buffer(enc, bkvd, 0ul, 6)
    kn_buffer(enc, bnh, 0ul, 7)
    kn_buffer(enc, bkvm, 0ul, 8)
    kn_buffer(enc, bhs, 0ul, 9)
    kn_buffer(enc, bscale, 0ul, 10)
    kn_buffer(enc, bqs, 0ul, 11)
    kn_buffer(enc, bcap, 0ul, 12)
    kn_buffer(enc, bsink != null ? bsink : g_one, 0ul, 20)
    kn_buffer(enc, bhass != null ? bhass : g_zero, 0ul, 21)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part_b(enc : MetalComputeEncoder?; f16 : bool; bq, bpart, brt, blay, bkvd, bnh, bkvm, bhs, bscale, bqs, bnch, bcap, bwin : MetalBuffer?; nrows, nchmax, nheads, head_size : int64; rtoff : uint64 = 0ul) {
    kn_pipeline(enc, f16 ? g_pso_attnpb16 : g_pso_attnpb32)
    kn_tgmem(enc, f16 ? metal_sq_attn_part_b16_msl_tgmem : metal_sq_attn_part_b32_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, bq, 0ul, 2)
    kn_buffer(enc, bpart, 0ul, 3)
    kn_buffer(enc, brt, rtoff, 4)
    kn_buffer(enc, blay, 0ul, 5)
    kn_buffer(enc, bkvd, 0ul, 6)
    kn_buffer(enc, bnh, 0ul, 7)
    kn_buffer(enc, bkvm, 0ul, 8)
    kn_buffer(enc, bhs, 0ul, 9)
    kn_buffer(enc, bscale, 0ul, 10)
    kn_buffer(enc, bqs, 0ul, 11)
    kn_buffer(enc, bnch, 0ul, 12)
    kn_buffer(enc, bcap, 0ul, 13)
    kn_buffer(enc, bwin, 0ul, 14)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(bpart)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads * nchmax * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

// tq4-mirror batch attention twins — same four-way arena binding as the q8_0 encoders below
def enc_attn_b_tq4(enc : MetalComputeEncoder?; bq, by, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bcap : MetalBuffer?; nrows, nheads, head_size : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null) {
    kn_pipeline(enc, g_pso_attnb_tq4)
    kn_tgmem(enc, metal_sq_attn_b_tq4_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, g_arena, 0ul, 3)
    kn_buffer(enc, bq, 0ul, 4)
    kn_buffer(enc, by, 0ul, 5)
    kn_buffer(enc, brt, 0ul, 6)
    kn_buffer(enc, blay, 0ul, 7)
    kn_buffer(enc, bnblk, 0ul, 8)
    kn_buffer(enc, bnh, 0ul, 9)
    kn_buffer(enc, bkvm, 0ul, 10)
    kn_buffer(enc, bhs, 0ul, 11)
    kn_buffer(enc, bscale, 0ul, 12)
    kn_buffer(enc, bqs, 0ul, 13)
    kn_buffer(enc, bcap, 0ul, 14)
    kn_buffer(enc, bsink != null ? bsink : g_one, 0ul, 20)
    kn_buffer(enc, bhass != null ? bhass : g_zero, 0ul, 21)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part_b_tq4(enc : MetalComputeEncoder?; bq, bpart, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bnch, bcap, bwin : MetalBuffer?; nrows, nchmax, nheads, head_size : int64) {
    kn_pipeline(enc, g_pso_attnpb_tq4)
    kn_tgmem(enc, metal_sq_attn_part_b_tq4_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, g_arena, 0ul, 3)
    kn_buffer(enc, bq, 0ul, 4)
    kn_buffer(enc, bpart, 0ul, 5)
    kn_buffer(enc, brt, 0ul, 6)
    kn_buffer(enc, blay, 0ul, 7)
    kn_buffer(enc, bnblk, 0ul, 8)
    kn_buffer(enc, bnh, 0ul, 9)
    kn_buffer(enc, bkvm, 0ul, 10)
    kn_buffer(enc, bhs, 0ul, 11)
    kn_buffer(enc, bscale, 0ul, 12)
    kn_buffer(enc, bqs, 0ul, 13)
    kn_buffer(enc, bnch, 0ul, 14)
    kn_buffer(enc, bcap, 0ul, 15)
    kn_buffer(enc, bwin, 0ul, 16)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(bpart)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads * nchmax * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_d_tq4(enc : MetalComputeEncoder?; bq, bpart, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bnsg : MetalBuffer?; nsgs, nrows, nheads : int64) {
    kn_pipeline(enc, g_pso_attnd_tq4)
    kn_tgmem(enc, metal_sq_attn_d_tq4_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, g_arena, 0ul, 3)
    kn_buffer(enc, bq, 0ul, 4)
    kn_buffer(enc, bpart, 0ul, 5)
    kn_buffer(enc, brt, 0ul, 6)
    kn_buffer(enc, blay, 0ul, 7)
    kn_buffer(enc, bnblk, 0ul, 8)
    kn_buffer(enc, bnh, 0ul, 9)
    kn_buffer(enc, bkvm, 0ul, 10)
    kn_buffer(enc, bhs, 0ul, 11)
    kn_buffer(enc, bscale, 0ul, 12)
    kn_buffer(enc, bqs, 0ul, 13)
    kn_buffer(enc, bnsg, 0ul, 14)
    // HEAD-FASTEST launch: consecutive tgs share position rows (SLC locality at depth)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(bpart)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads), uint((nsgs + 3l) / 4l), uint(nrows)), uint3(128u, 1u, 1u))
}

// q8_0-mirror batch attention twins: the arena binds FOUR times (half-scale + byte views for K
// and V); the row table carries byte offsets and nblk replaces the kv_dim uniform
def enc_attn_b_q8(enc : MetalComputeEncoder?; bq, by, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bcap : MetalBuffer?; nrows, nheads, head_size : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null) {
    kn_pipeline(enc, g_pso_attnb_q8)
    kn_tgmem(enc, metal_sq_attn_b_q8_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, g_arena, 0ul, 3)
    kn_buffer(enc, bq, 0ul, 4)
    kn_buffer(enc, by, 0ul, 5)
    kn_buffer(enc, brt, 0ul, 6)
    kn_buffer(enc, blay, 0ul, 7)
    kn_buffer(enc, bnblk, 0ul, 8)
    kn_buffer(enc, bnh, 0ul, 9)
    kn_buffer(enc, bkvm, 0ul, 10)
    kn_buffer(enc, bhs, 0ul, 11)
    kn_buffer(enc, bscale, 0ul, 12)
    kn_buffer(enc, bqs, 0ul, 13)
    kn_buffer(enc, bcap, 0ul, 14)
    kn_buffer(enc, bsink != null ? bsink : g_one, 0ul, 20)
    kn_buffer(enc, bhass != null ? bhass : g_zero, 0ul, 21)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_part_b_q8(enc : MetalComputeEncoder?; bq, bpart, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bnch, bcap, bwin : MetalBuffer?; nrows, nchmax, nheads, head_size : int64) {
    kn_pipeline(enc, g_pso_attnpb_q8)
    kn_tgmem(enc, metal_sq_attn_part_b_q8_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, g_arena, 0ul, 3)
    kn_buffer(enc, bq, 0ul, 4)
    kn_buffer(enc, bpart, 0ul, 5)
    kn_buffer(enc, brt, 0ul, 6)
    kn_buffer(enc, blay, 0ul, 7)
    kn_buffer(enc, bnblk, 0ul, 8)
    kn_buffer(enc, bnh, 0ul, 9)
    kn_buffer(enc, bkvm, 0ul, 10)
    kn_buffer(enc, bhs, 0ul, 11)
    kn_buffer(enc, bscale, 0ul, 12)
    kn_buffer(enc, bqs, 0ul, 13)
    kn_buffer(enc, bnch, 0ul, 14)
    kn_buffer(enc, bcap, 0ul, 15)
    kn_buffer(enc, bwin, 0ul, 16)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(bpart)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads * nchmax * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_d_q8(enc : MetalComputeEncoder?; bq, bpart, brt, blay, bnblk, bnh, bkvm, bhs, bscale, bqs, bnsg : MetalBuffer?; nsgs, nrows, nheads : int64) {
    kn_pipeline(enc, g_pso_attnd_q8)
    kn_tgmem(enc, metal_sq_attn_d_q8_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, g_arena, 0ul, 3)
    kn_buffer(enc, bq, 0ul, 4)
    kn_buffer(enc, bpart, 0ul, 5)
    kn_buffer(enc, brt, 0ul, 6)
    kn_buffer(enc, blay, 0ul, 7)
    kn_buffer(enc, bnblk, 0ul, 8)
    kn_buffer(enc, bnh, 0ul, 9)
    kn_buffer(enc, bkvm, 0ul, 10)
    kn_buffer(enc, bhs, 0ul, 11)
    kn_buffer(enc, bscale, 0ul, 12)
    kn_buffer(enc, bqs, 0ul, 13)
    kn_buffer(enc, bnsg, 0ul, 14)
    // HEAD-FASTEST launch: consecutive tgs share position rows (SLC locality at depth)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(bpart)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads), uint((nsgs + 3l) / 4l), uint(nrows)), uint3(128u, 1u, 1u))
}

def enc_attn_comb_b(enc : MetalComputeEncoder?; bpart, by, brt, bnch, bnh, bhs, bwin : MetalBuffer?; nrows, nheads, head_size : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null; rtoff : uint64 = 0ul) {
    kn_pipeline(enc, g_pso_attncombb)
    kn_buffer(enc, bpart, 0ul, 0)
    kn_buffer(enc, by, 0ul, 1)
    kn_buffer(enc, brt, rtoff, 2)
    kn_buffer(enc, bnch, 0ul, 3)
    kn_buffer(enc, bnh, 0ul, 4)
    kn_buffer(enc, bhs, 0ul, 5)
    kn_buffer(enc, bwin, 0ul, 6)
    kn_buffer(enc, bsink != null ? bsink : g_one, 0ul, 20)
    kn_buffer(enc, bhass != null ? bhass : g_zero, 0ul, 21)
    hz_read(bpart)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads * nrows), 1u, 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_attn_d(enc : MetalComputeEncoder?; f16 : bool; bq, bpart, brt, blay, bkvd, bnh, bkvm, bhs, bscale, bqs, bnsg : MetalBuffer?; nsgs, nrows, nheads : int64) {
    kn_pipeline(enc, f16 ? g_pso_attnd16 : g_pso_attnd32)
    kn_tgmem(enc, f16 ? metal_sq_attn_d16_msl_tgmem : metal_sq_attn_d32_msl_tgmem, 0)
    kn_buffer(enc, g_arena, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, bq, 0ul, 2)
    kn_buffer(enc, bpart, 0ul, 3)
    kn_buffer(enc, brt, 0ul, 4)
    kn_buffer(enc, blay, 0ul, 5)
    kn_buffer(enc, bkvd, 0ul, 6)
    kn_buffer(enc, bnh, 0ul, 7)
    kn_buffer(enc, bkvm, 0ul, 8)
    kn_buffer(enc, bhs, 0ul, 9)
    kn_buffer(enc, bscale, 0ul, 10)
    kn_buffer(enc, bqs, 0ul, 11)
    kn_buffer(enc, bnsg, 0ul, 12)
    // HEAD-FASTEST launch: consecutive tgs share position rows (SLC locality at depth)
    hz_read(g_arena)
    hz_read(bq)
    hz_write(bpart)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads), uint((nsgs + 3l) / 4l), uint(nrows)), uint3(128u, 1u, 1u))
}

def enc_attn_comb_d(enc : MetalComputeEncoder?; bpart, by, brt, bnsg, bnh, bhs : MetalBuffer?; nrows, nheads, head_size : int64) {
    kn_pipeline(enc, g_pso_attncombd)
    kn_tgmem(enc, metal_sq_attn_comb_d_msl_tgmem, 0)
    kn_buffer(enc, bpart, 0ul, 0)
    kn_buffer(enc, by, 0ul, 1)
    kn_buffer(enc, brt, 0ul, 2)
    kn_buffer(enc, bnsg, 0ul, 3)
    kn_buffer(enc, bnh, 0ul, 4)
    kn_buffer(enc, bhs, 0ul, 5)
    hz_read(bpart)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nheads), uint(nrows), 1u), uint3(uint(head_size), 1u, 1u))
}

def enc_rope_store_b(enc : MetalComputeEncoder?; f16 : bool; bqkv, bcos, bsin, brt, blay, bkvd, bqd, bhs, bnpq, bnpall, bqs, bneox, bbias, bhasb : MetalBuffer?; npairs, nrows : int64; rtoff : uint64 = 0ul; brot : MetalBuffer? = null) {
    kn_pipeline(enc, f16 ? g_pso_rpstb16 : g_pso_rpstb32)
    kn_buffer(enc, bqkv, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, bcos, 0ul, 3)
    kn_buffer(enc, bsin, 0ul, 4)
    kn_buffer(enc, brt, rtoff, 5)
    kn_buffer(enc, blay, 0ul, 6)
    kn_buffer(enc, bkvd, 0ul, 7)
    kn_buffer(enc, bqd, 0ul, 8)
    kn_buffer(enc, bhs, 0ul, 9)
    kn_buffer(enc, bnpq, 0ul, 10)
    kn_buffer(enc, bnpall, 0ul, 11)
    kn_buffer(enc, bqs, 0ul, 12)
    kn_buffer(enc, bneox, 0ul, 13)
    kn_buffer(enc, bbias, 0ul, 14)
    kn_buffer(enc, bhasb, 0ul, 15)
    kn_buffer(enc, brot != null ? brot : bhs, 0ul, 16)   // rot defaults to head_size (full rope)
    hz_write(bqkv)
    hz_write(g_arena)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((npairs + 63l) / 64l), uint(nrows), 1u), uint3(64u, 1u, 1u))
}

// tq4-mirror batched rope+store: one threadgroup per (head, batch row), arena bound four ways
def enc_rope_store_b_tq4(enc : MetalComputeEncoder?; bqkv, bcos, bsin, brt, blay, bqd, bhs, bnh, bkvm, bqs, bneox, bbias, bhasb : MetalBuffer?; ntg, nrows, head_size : int64) {
    kn_pipeline(enc, g_pso_rpstb_tq4)
    kn_tgmem(enc, metal_rope_store_b_tq4_msl_tgmem, 0)
    kn_buffer(enc, bqkv, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, g_arena, 0ul, 3)
    kn_buffer(enc, g_arena, 0ul, 4)
    kn_buffer(enc, bcos, 0ul, 5)
    kn_buffer(enc, bsin, 0ul, 6)
    kn_buffer(enc, signs_buffer(), 0ul, 7)
    kn_buffer(enc, brt, 0ul, 8)
    kn_buffer(enc, blay, 0ul, 9)
    kn_buffer(enc, bqd, 0ul, 10)
    kn_buffer(enc, bhs, 0ul, 11)
    kn_buffer(enc, bnh, 0ul, 12)
    kn_buffer(enc, bkvm, 0ul, 13)
    kn_buffer(enc, bqs, 0ul, 14)
    kn_buffer(enc, bneox, 0ul, 15)
    kn_buffer(enc, bbias, 0ul, 16)
    kn_buffer(enc, bhasb, 0ul, 17)
    hz_write(bqkv)
    hz_write(g_arena)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(ntg), uint(nrows), 1u), uint3(uint(head_size), 1u, 1u))
}

// q8_0-mirror batched rope+store: arena bound four ways (K/V x scale/byte views); grid covers
// the q pairs + one thread per K/V 32-block, one grid row per batch row
def enc_rope_store_b_q8(enc : MetalComputeEncoder?; bqkv, bcos, bsin, brt, blay, bnblk, bqd, bhs, bnpq, bqs, bneox, bbias, bhasb : MetalBuffer?; nthreads, nrows : int64) {
    kn_pipeline(enc, g_pso_rpstb_q8)
    kn_buffer(enc, bqkv, 0ul, 0)
    kn_buffer(enc, g_arena, 0ul, 1)
    kn_buffer(enc, g_arena, 0ul, 2)
    kn_buffer(enc, g_arena, 0ul, 3)
    kn_buffer(enc, g_arena, 0ul, 4)
    kn_buffer(enc, bcos, 0ul, 5)
    kn_buffer(enc, bsin, 0ul, 6)
    kn_buffer(enc, brt, 0ul, 7)
    kn_buffer(enc, blay, 0ul, 8)
    kn_buffer(enc, bnblk, 0ul, 9)
    kn_buffer(enc, bqd, 0ul, 10)
    kn_buffer(enc, bhs, 0ul, 11)
    kn_buffer(enc, bnpq, 0ul, 12)
    kn_buffer(enc, bqs, 0ul, 13)
    kn_buffer(enc, bneox, 0ul, 14)
    kn_buffer(enc, bbias, 0ul, 15)
    kn_buffer(enc, bhasb, 0ul, 16)
    hz_write(bqkv)
    hz_write(g_arena)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint((nthreads + 63l) / 64l), uint(nrows), 1u), uint3(64u, 1u, 1u))
}

def enc_add_rms_b(enc : MetalComputeEncoder?; bx, ba, bw, by, bdim, beps : MetalBuffer?; nrows : int64; bascale : MetalBuffer? = null) {
    kn_pipeline(enc, g_pso_addrmsb)
    kn_tgmem(enc, metal_add_rms_b_msl_tgmem, 0)
    kn_buffer(enc, bx, 0ul, 0)
    kn_buffer(enc, ba, 0ul, 1)
    kn_buffer(enc, bw, 0ul, 2)
    kn_buffer(enc, by, 0ul, 3)
    kn_buffer(enc, bdim, 0ul, 4)
    kn_buffer(enc, beps, 0ul, 5)
    kn_buffer(enc, bascale != null ? bascale : g_one, 0ul, 6)   // the add's ascale slot
    hz_read(ba)
    hz_write(bx)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nrows), 1u, 1u), uint3(1024u, 1u, 1u))
}

// batched rmsnorm (the prologue): the prefill rms kernel is width-agnostic — one tg per row
def enc_rms_b(enc : MetalComputeEncoder?; bx, bw, by, bdim, beps : MetalBuffer?; nrows : int64) {
    kn_pipeline(enc, g_pso_rms)
    kn_tgmem(enc, metal_rmsnorm_msl_tgmem, 0)
    kn_buffer(enc, bx, 0ul, 0)
    kn_buffer(enc, bw, 0ul, 1)
    kn_buffer(enc, by, 0ul, 2)
    kn_buffer(enc, bdim, 0ul, 3)
    kn_buffer(enc, beps, 0ul, 4)
    hz_read(bx)
    hz_write(by)
    hz_gate(enc)
    kn_dispatch(enc, uint3(uint(nrows), 1u, 1u), uint3(1024u, 1u, 1u))
}


//! Release every PSO the registry compiled (the PSO half of metal_decode_shutdown — runs
//! before the shared device goes).
def metal_kernels_release {
    if (g_pso_gemv != null) {
        metal_release(g_pso_gemv)
        g_pso_gemv = null
    }
    release_pso(g_pso_kq_k4)
    release_pso(g_pso_kq_k5)
    release_pso(g_pso_kq_k5c)
    release_pso(g_pso_kq_k6)
    release_pso(g_pso_kq_mvb2_k4)
    release_pso(g_pso_kq_mvb4_k4)
    release_pso(g_pso_kq_mvb2_k5)
    release_pso(g_pso_kq_mvb4_k5)
    release_pso(g_pso_kq_mvb2_k6)
    release_pso(g_pso_kq_mvb4_k6)
    release_pso(g_pso_kq_mvb8_k4)
    release_pso(g_pso_kq_mvb8_k5)
    release_pso(g_pso_kq_mvb8_k6)
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
    if (g_pso_preaddrms != null) {
        metal_release(g_pso_preaddrms)
        g_pso_preaddrms = null
    }
    if (g_one != null) {
        metal_release(g_one)
        g_one = null
    }
    if (g_zero != null) {
        metal_release(g_zero)
        g_zero = null
    }
    if (g_pso_geglu != null) {
        metal_release(g_pso_geglu)
        g_pso_geglu = null
    }
    if (g_pso_softcap != null) {
        metal_release(g_pso_softcap)
        g_pso_softcap = null
    }
    if (g_pso_suppress != null) {
        metal_release(g_pso_suppress)
        g_pso_suppress = null
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
    release_pso(g_pso_qknorm)
    if (g_pso_attncomb != null) {
        metal_release(g_pso_attncomb)
        g_pso_attncomb = null
    }
    if (g_pso_rpst16 != null) {
        metal_release(g_pso_rpst16)
        g_pso_rpst16 = null
    }
    release_pso(g_pso_rpst_h16)
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
    release_pso(g_pso_emb_k6)
    release_pso(g_pso_moe_router)
    release_pso(g_pso_moe_select)
    release_pso(g_pso_moe_gemv_q8)
    release_pso(g_pso_moe_kq_k4)
    release_pso(g_pso_moe_kq_k5)
    release_pso(g_pso_moe_kq_k6)
    release_pso(g_pso_moe_combine)
    release_pso(g_pso_g4rnorm)
    release_pso(g_pso_moe_wscale)
    release_pso(g_pso_swiglu_oai)
    release_pso(g_pso_moe_gemv_mx4)
    release_pso(g_pso_moe_gemv_q51)
    release_pso(g_pso_dn_conv)
    release_pso(g_pso_dn_hist)
    release_pso(g_pso_dn_l2)
    release_pso(g_pso_dn_scan)
    release_pso(g_pso_dn_gate)
    release_pso(g_pso_dn_deint)
    release_pso(g_pso_sigmul)
    release_pso(g_pso_axpy_sig)
    if (g_mx4_vtab != null) {
        metal_release(g_mx4_vtab)
        g_mx4_vtab = null
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


// ===== Metal-4 tensor twin races (decode-side batch GEMM families) =====

// one q8b batch-GEMM race: shared blob/X geometry, per-family psos + binds via the blocks
def private race_gemmb_family(dev : MetalDevice?; queue : MetalCommandQueue?; family : string;
                              m, kdim, ndim, out_n : int; grid : uint3;
                              base_pso, twin_pso : MetalComputePipeline?; base_tgmem : uint64;
                              bind_extra : block<(enc : MetalComputeEncoder?; slot_y : MetalBuffer?) : void>) : MetalTensorRaceResult {
    var res = MetalTensorRaceResult(family = family, winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    let nkb = kdim / 32
    var xa <- race_x_f32(m * kdim)
    var blob <- race_q8_blob(ndim * nkb)
    var bw = race_buf(dev, uint64(ndim * nkb * 34), unsafe(addr<void?>(blob[0])))
    var bxa = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = race_buf(dev, uint64(out_n * 4), null)
    var by_twin = race_buf(dev, uint64(out_n * 4), null)
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_tgmem(enc, base_tgmem, 0)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 2)
        kn_buffer(enc, bxa, 0ul, 3)
        invoke(bind_extra, enc, by_base)
        kn_dispatch(enc, grid, tg)
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 2)
        kn_buffer(enc, bxa, 0ul, 3)
        invoke(bind_extra, enc, by_twin)
        kn_dispatch(enc, grid, tg)
    })
    if (res.base_ms < 0.0lf || res.twin_ms < 0.0lf) {
        res.winner = res.base_ms < 0.0lf ? "" : "simdgroup"
        res.note = "dispatch failed"
    } elif (!race_envelope_ok(by_base, by_twin, out_n, res.note)) {
        res.winner = "simdgroup"
    } else {
        res.winner = res.twin_ms < res.base_ms ? "tensor" : "simdgroup"
    }
    metal_release(bw)
    metal_release(bxa)
    metal_release(by_base)
    metal_release(by_twin)
    return res
}

def private race_decode_family(dev : MetalDevice?; queue : MetalCommandQueue?; family : string;
                               base_src, base_entry : string; base_fm : bool; base_tgmem : uint64;
                               twin_src, twin_entry : string; twin_fm : bool;
                               m, kdim, ndim, out_n : int; grid : uint3;
                               bind_extra : block<(enc : MetalComputeEncoder?; slot_y : MetalBuffer?) : void>) : MetalTensorRaceResult {
    var err = ""
    var base_pso = pipeline_from_source(dev, base_src, base_entry, base_fm, err)
    if (base_pso == null) {
        return MetalTensorRaceResult(family = family, winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "base pso: {err}")
    }
    var twin_pso = pipeline_from_source(dev, twin_src, twin_entry, twin_fm, err)
    if (twin_pso == null) {
        metal_release(base_pso)
        return MetalTensorRaceResult(family = family, winner = "simdgroup", base_ms = -1.0lf, twin_ms = -1.0lf, note = "twin pso: {err}")
    }
    let res = race_gemmb_family(dev, queue, family, m, kdim, ndim, out_n, grid, base_pso, twin_pso, base_tgmem) $(enc, slot_y) {
        invoke(bind_extra, enc, slot_y)
    }
    metal_release(base_pso)
    metal_release(twin_pso)
    return res
}

//! Decode-side tuner entry (the runtime.metal_tensor crown): the batch q8 GEMM families.
def metal_tensor_race_decode : array<MetalTensorRaceResult> {
    var results : array<MetalTensorRaceResult>
    var dev = metal_create_system_default_device()
    if (dev == null) {
        return <- results
    }
    var queue = metal_new_command_queue(dev)
    let kdim = 2048
    let ndim = 1024
    var bk = race_uniform_u32(dev, uint(kdim))
    var bn = race_uniform_u32(dev, uint(ndim))
    var bys = race_uniform_u32(dev, uint(ndim))
    var bsk = race_uniform_u32(dev, 4u)
    results |> emplace(race_decode_family(dev, queue, "gemmb_q8",
        metal_q8_gemm_b_msl, metal_q8_gemm_b_msl_entry, metal_q8_gemm_b_msl_fastmath, metal_q8_gemm_b_msl_tgmem,
        metal_q8_gemm_b_t_msl, metal_q8_gemm_b_t_msl_entry, metal_q8_gemm_b_t_msl_fastmath,
        32, kdim, ndim, 32 * ndim, uint3(uint(ndim / 32), 1u, 1u)) $(enc, slot_y) {
        kn_buffer(enc, slot_y, 0ul, 4)
        kn_buffer(enc, bk, 0ul, 5)
        kn_buffer(enc, bn, 0ul, 6)
        kn_buffer(enc, bys, 0ul, 7)
    })
    results |> emplace(race_decode_family(dev, queue, "gemmb_sk_q8",
        metal_q8_gemm_b_sk_msl, metal_q8_gemm_b_sk_msl_entry, metal_q8_gemm_b_sk_msl_fastmath, metal_q8_gemm_b_sk_msl_tgmem,
        metal_q8_gemm_b_sk_t_msl, metal_q8_gemm_b_sk_t_msl_entry, metal_q8_gemm_b_sk_t_msl_fastmath,
        32, kdim, ndim, 4 * 32 * ndim, uint3(uint(ndim / 32), 4u, 1u)) $(enc, slot_y) {
        kn_buffer(enc, slot_y, 0ul, 4)
        kn_buffer(enc, bk, 0ul, 5)
        kn_buffer(enc, bn, 0ul, 6)
        kn_buffer(enc, bsk, 0ul, 8)
    })
    results |> emplace(race_decode_family(dev, queue, "gemm64b_q8",
        metal_q8_gemm64_b_msl, metal_q8_gemm64_b_msl_entry, metal_q8_gemm64_b_msl_fastmath, metal_q8_gemm64_b_msl_tgmem,
        metal_q8_gemm64_b_t_msl, metal_q8_gemm64_b_t_msl_entry, metal_q8_gemm64_b_t_msl_fastmath,
        64, kdim, ndim, 64 * ndim, uint3(uint(2 * (ndim / 64)), 1u, 1u)) $(enc, slot_y) {
        kn_buffer(enc, slot_y, 0ul, 4)
        kn_buffer(enc, bk, 0ul, 5)
        kn_buffer(enc, bn, 0ul, 6)
    })
    metal_release(bk)
    metal_release(bn)
    metal_release(bys)
    metal_release(bsk)
    metal_release(queue)
    metal_release(dev)
    return <- results
}

// ===== family-shared kernel classes (M1: moved from prefill) =====

// One threadgroup per row: cooperative sum of squares (simd_sum + per-simdgroup partials),
// lane 0 folds partials into rsqrt(mean + eps), every thread scales its strided slice.
[metal_dispatch(name = "enc_rms", pso = "g_pso_rms", tgmem = "metal_rmsnorm_msl_tgmem", tg = 1024, grid = "1")]
class MetalRmsNorm {
    @ssbo @binding = 0 x : array<float>     // [nrows x dim] source
    @ssbo @binding = 1 @role = "weight" w : array<float>     // [dim] weight
    @ssbo @binding = 2 y : array<float>     // [nrows x dim] out (x aliasing is safe: x is fully
                                            // read before the barrier that precedes any write)
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 eps : float
    @workgroup partial : float[32]          // one per simdgroup — width-agnostic up to 1024
                                            // threads (prefill dispatches 256, decode 1024)
    @workgroup inv_sh : float

    [metal_kernel(name="metal_rmsnorm_msl")]
    def metal_rmsnorm {
        let row = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        var ss = 0.0
        var k = lid
        while (k < dim) {
            let vv = x[row * dim + k]
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
            y[row * dim + k] = w[k] * (x[row * dim + k] * inv)
            k += gl_WorkGroupSize.x
        }
    }
}

// The production GEMM: 64-output x 32-token C tiles (4 simdgroups, 8 accumulators), W as
// interleaved 34B q8 blocks, raw f32 activations. Rolled [unroll_full] math over simdgroup-matrix
// arrays. Requires out-dim % 64, M padded to 64. Beats llama.cpp's equivalent 2-3% on M1 Max.
class MetalQ8MulMm {
    @ssbo @binding = 0 @role = "weight" wsh : array<float16> // 34B-block W blob, HALF-SCALE view: scale of block b at 17*b
    @ssbo @binding = 1 @role = "weight" wqb : array<int8>    // the SAME blob buffer, BYTE view: quants of block b at 34*b + 2
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // raw f32 activations, float4 view
    @ssbo @binding = 3 @role = "write" y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="metal_q8_mulmm_msl")]
    def metal_q8_mulmm {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var mc : simdgroup_float8x8[8]      // per-simdgroup C quadrant: 2 token-tiles x 4 output-tiles
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]                // dequantized A staging regs (this thread's block half)
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tile il0*2 (lanes at stride 8; +512 = next k-tile)
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        // device streams walk as REAL das pointers (unsafe(addr()) + bump at the loop tail):
        // pointer-vs-index register-allocates differently on AGX and is shape-dependent — this
        // kernel measured pointer (llama.cpp's loop reads its scale straight off a carried pointer)
        let blk0 = (nBase + lr0) * nkb
        var scur = unsafe(addr(wsh[blk0 * 17u]))                    // half scale rides the quants' cacheline
        var qp = unsafe(addr(wqb[blk0 * 34u + 2u + il0 * 16u]))
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            // device reads + dequant first — they fly while the previous math loop drains
            let sA = float(unsafe(scur[0]))
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            for [unroll_full] (i in range(16)) {
                va[i] = float16(float(unsafe(qp[i])) * sA)
            }
            barrier()                       // previous math done reading the tiles
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)     // no dequant on B — pure convert-and-store
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            // math ROLLED: the pragma'd loops reach the backend intact — 4 k-slabs x
            // (6 tile loads + 8 macs); tile POINTERS bump at the slab tail (index form
            // measured max_threads 896 and -10% — the backend precomputes every address)
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
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
            scur = unsafe(scur + 17)
            qp = unsafe(qp + 34)
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

class MetalKqMulMmK4 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B compact scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_kq_mulmm_k4_msl")]
    def metal_kq_mulmm_k4 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        let nsb = kdim / 256u
        let wrow = nBase + lr0
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = wrow * nsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let sv = ks4[blk]
            var sc = 0u
            var mn = 0u
            if (js < 4u) {
                sc = (sv.y >> (8u * js)) & 63u
                mn = (sv.z >> (8u * js)) & 63u
            } else {
                let j2 = js - 4u
                let hi = sv.w >> (8u * j2)
                sc = (hi & 15u) | (((sv.y >> (8u * j2 + 6u)) & 3u) << 4u)
                mn = ((hi >> 4u) & 15u) | (((sv.z >> (8u * j2 + 6u)) & 3u) << 4u)
            }
            let dsc = float(ksh[blk * 8u]) * float(sc)
            let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
            let qb = blk * 32u + (js / 2u) * 8u + il0 * 4u
            let nsh = (js & 1u) * 4u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                for [unroll_full] (c in range(4)) {
                    let q = (u >> (8u * uint(c) + nsh)) & 15u
                    va[k * 4 + c] = float16(dsc * float(q) - dmn)
                }
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
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
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

class MetalKqMulMmK5 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B compact scale blocks (the Q4_K form)
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k5 quant plane, uint view (qs at 40*blk, qh at 40*blk+32)
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_kq_mulmm_k5_msl")]
    def metal_kq_mulmm_k5 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        let nsb = kdim / 256u
        let wrow = nBase + lr0
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = wrow * nsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let sv = ks4[blk]
            var sc = 0u
            var mn = 0u
            if (js < 4u) {
                sc = (sv.y >> (8u * js)) & 63u
                mn = (sv.z >> (8u * js)) & 63u
            } else {
                let j2 = js - 4u
                let hi = sv.w >> (8u * j2)
                sc = (hi & 15u) | (((sv.y >> (8u * j2 + 6u)) & 3u) << 4u)
                mn = ((hi >> 4u) & 15u) | (((sv.z >> (8u * j2 + 6u)) & 3u) << 4u)
            }
            let dsc = float(ksh[blk * 8u]) * float(sc)
            let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
            let qb = blk * 40u + (js / 2u) * 8u + il0 * 4u
            let hb = blk * 40u + 32u + il0 * 4u
            let nsh = (js & 1u) * 4u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                let hu = kqu[hb + uint(k)]
                for [unroll_full] (c in range(4)) {
                    let q = ((u >> (8u * uint(c) + nsh)) & 15u) | (((hu >> (8u * uint(c) + js)) & 1u) << 4u)
                    va[k * 4 + c] = float16(dsc * float(q) - dmn)
                }
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
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
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

class MetalKqMulMmK6 {
    @ssbo @binding = 0 @role = "weight" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" ksc : array<uint4>   // 16B sub-scale blocks (16 int8 at blk), same buffer at 0
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_kq_mulmm_k6_msl")]
    def metal_kq_mulmm_k6 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        let nsb = kdim / 256u
        let wrow = nBase + lr0
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var sv : uint4
        var dall = 0.0
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = wrow * nsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let half6 = js / 4u
            let gg = js % 4u
            if (js == 0u) {   // superblock scalars change every 8th kb; uniform branch, no divergence
                sv = ksc[blk]
                dall = float(kdh[blk])
            }
            let si = half6 * 8u + gg * 2u + il0
            let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
            let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
            let dsc = dall * s6
            let dmn = dsc * 32.0   // -32 folded to the fma form (k4's dmn shape)
            // byte-position shift folded into exact power-of-two pre-scales (lcpp dequantize_q6_K
            // shape): q<<8c is an exact f32 integer (6-bit quants), dsc/2^8c is an exponent shift,
            // so each fma sees the same real product — bit-identical, one shift fewer per element
            let dsc1 = dsc * 0.00390625
            let dsc2 = dsc * 0.0000152587890625
            let dsc3 = dsc * 0.000000059604644775390625
            let qb = blk * 48u + half6 * 16u + (gg & 1u) * 8u + il0 * 4u
            let hb = blk * 48u + 32u + half6 * 8u + il0 * 4u
            let nsh = (gg / 2u) * 4u
            let hsh = gg * 2u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                let hu = kqu[hb + uint(k)]
                // all 4 bytes composed at once — cross-byte bleed of the uint-wide shifts is
                // masked off; per element only mask+fma remain (shift folded into dsc1..3)
                let qv = ((u >> nsh) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                va[k * 4] = float16(dsc * float(qv & 255u) - dmn)
                va[k * 4 + 1] = float16(dsc1 * float(qv & 65280u) - dmn)
                va[k * 4 + 2] = float16(dsc2 * float(qv & 16711680u) - dmn)
                va[k * 4 + 3] = float16(dsc3 * float(qv & 4278190080u) - dmn)
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
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
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// Router GEMV over the uploaded fp32 router slab: y[ne] = W[ne x dim] . x per stream. ne <= 256
// rows — latency-trivial next to the expert GEMVs. grid.y = stream (batch; single decode = 1).
[metal_dispatch(name = "enc_moe_router", pso = "g_pso_moe_router", tg = "g_gemv_rows*32", grid = "ne/g_gemv_rows, nst", params = "ne : int64, nst : int64")]
class MetalMoeRouter {
    @ssbo @binding = 0 @role = "weight" w : array<float4>    // [ne x dim] router weights, float4 view
    @ssbo @binding = 1 x : array<float4>    // [streams x dim] activations, float4 view
    @ssbo @binding = 2 @span = "ne*nst*4" y : array<float>     // [streams x ne] logits out
    @uniform @binding = 3 ndim : uint       // dim (% 32)
    @uniform @binding = 4 ddim : uint       // ne
    @uniform @binding = 5 xs : uint         // per-stream x stride, elements
    @ssbo @binding = 6 @role = "weight" rb : array<float>    // [ne] router bias row (hasb models — gpt-oss)
    @uniform @binding = 7 hasb : uint

    [metal_kernel(name="metal_moe_router_msl")]
    def metal_moe_router {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let st = gl_WorkGroupID.y
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let x0e = st * (xs / 4u)
        var acc = 0.0
        var k = lane
        while (k < nb4) {
            let wv = w[row * nb4 + k]
            let xv = x[x0e + k]
            acc += wv.x * xv.x + wv.y * xv.y + wv.z * xv.z + wv.w * xv.w
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[st * ddim + row] = hasb != 0u ? acc + rb[row] : acc
        }
    }
}

// Router select — moe_select_core's softmax + top-k + norm_topk on one simdgroup per stream
// (grid.x): per-lane strided ownership (ne <= 256), per-winner argmax butterfly with the CPU
// tie-break (lowest index), register knockout. Stream row layout: [k idx u32][k w f32].
[metal_dispatch(name = "enc_moe_select", pso = "g_pso_moe_select", tg = 32, grid = "nst", params = "nst : int64")]
class MetalMoeSelect {
    @ssbo @binding = 0 lg : array<float>     // [streams x ne] router logits
    @ssbo @binding = 1 seli : array<uint>    // selection buffer, uint view — idx of stream s at s*2k
    @ssbo @binding = 2 @role = "alias" selw : array<float>   // the same buffer, float view — w of stream s at s*2k + k
    @uniform @binding = 3 ne : uint
    @uniform @binding = 4 nk : uint
    @uniform @binding = 5 gmode : uint       // 0 = softmax + norm_topk; 1 = softmax_weight (gpt-oss); 2 = softmax, no renorm (qwen2moe)

    [metal_kernel(name="metal_moe_select_msl")]
    def metal_moe_select {
        let st = gl_WorkGroupID.x
        let lg0 = st * ne
        let sel0 = st * 2u * nk
        let lane = gl_SubgroupInvocationID
        var v : float[8]           // lane owns logits lane, lane+32, ... — 8 deep covers ne <= 256
        for [unroll_full] (t in range(8)) {
            let j = lane + uint(t) * 32u
            v[t] = j < ne ? lg[lg0 + j] : -3.0e38
        }
        // gmode 0/2: softmax over all ne (the CPU order: max, exp with the -80 floor, sum,
        // divide-all); gmode 1 (gpt-oss softmax_weight) selects on the RAW logits instead
        if (gmode != 1u) {
            var mx = v[0]
            for [unroll_full] (t in range(8)) {
                mx = max(mx, v[t])
            }
            var bm = 16u
            while (bm >= 1u) {
                mx = max(mx, simd_shuffle_xor(mx, bm))
                bm /= 2u
            }
            var psum = 0.0
            for [unroll_full] (t in range(8)) {
                let j = lane + uint(t) * 32u
                v[t] = j < ne ? exp(max(v[t] - mx, -80.0)) : 0.0
                psum += v[t]
            }
            let sum = simd_sum(psum)
            for [unroll_full] (t in range(8)) {
                v[t] /= sum
            }
        }
        var ws = 0.0
        var j = 0u
        while (j < nk) {
            var bv = -3.0e38
            var bt = 0u
            for [unroll_full] (t in range(8)) {
                if (v[t] > bv) {
                    bv = v[t]
                    bt = uint(t)
                }
            }
            var bi = lane + bt * 32u
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
            if (lane == 0u) {
                seli[sel0 + j] = bi
                selw[sel0 + nk + j] = bv
            }
            ws += bv   // every lane holds the same winner value after the butterfly
            for [unroll_full] (t in range(8)) {   // owning lane knocks the winner out (constant indices)
                if (uint(t) == (bi >> 5u) && (bi & 31u) == lane) {
                    v[t] = -3.0e38
                }
            }
            j++
        }
        if (gmode == 1u) {
            // gpt-oss: softmax over just the selected k raw logits
            if (lane == 0u) {
                var mxw = selw[sel0 + nk]
                var i = 1u
                while (i < nk) {
                    mxw = max(mxw, selw[sel0 + nk + i])
                    i++
                }
                var sw = 0.0
                i = 0u
                while (i < nk) {
                    let ev = exp(max(selw[sel0 + nk + i] - mxw, -80.0))
                    selw[sel0 + nk + i] = ev
                    sw += ev
                    i++
                }
                i = 0u
                while (i < nk) {
                    selw[sel0 + nk + i] /= sw
                    i++
                }
            }
        } elif (gmode == 0u) {
            // norm_topk renorm to sum 1 (the CPU f16-min clamp); gmode 2 (qwen2moe) keeps
            // the softmax weights as selected — no renorm
            ws = max(ws, 6.103515625e-5)
            if (lane == 0u) {
                var i = 0u
                while (i < nk) {
                    selw[sel0 + nk + i] /= ws
                    i++
                }
            }
        }
    }
}

// Expert-indexed q8 GEMV — MetalQ8Gemv's exact dot with the expert plane picked GPU-side:
// the blob binds at the layer's expert STACK base, the kernel adds sel[slot] * eblk to the
// block index, reads x at slot * xs (0 = shared activation), writes y at slot * ddim.
class MetalMoeGemvQ8 {
    @ssbo @binding = 0 @role = "weight" wsh : array<float16> // the expert stack's blob slice, half-scale view
    @ssbo @binding = 1 @role = "weight" wqb : array<int8>    // the same blob buffer, byte view
    @ssbo @binding = 2 @role = "read" x : array<float4>
    @ssbo @binding = 3 @role = "write" y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 7 eblk : uint       // 34B blocks per expert plane (ege / 32)
    @uniform @binding = 8 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 9 ss : uint         // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 10 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 11 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_q8_msl")]
    def metal_moe_gemv_q8 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * eblk
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let nb4 = ndim / 4u
        let x0e = (st * bxs + slot * xs) / 4u
        var sp = unsafe(addr(wsh[(eb + row * nb + lane / 8u) * 17u]))
        var qp = unsafe(addr(wqb[(eb + row * nb + lane / 8u) * 34u + 2u + (lane % 8u) * 4u]))
        var x0 = unsafe(addr(x[x0e + lane]))
        var acc = 0.0
        var lw : float4[4]
        var k = lane
        while (k + 96u < nb4) {
            for [unroll_full] (c in range(4)) {
                let d = float(unsafe(sp[uint(c) * 68u]))
                let qb = uint(c) * 136u
                lw[c] = float4(float(unsafe(qp[qb])), float(unsafe(qp[qb + 1u])),
                    float(unsafe(qp[qb + 2u])), float(unsafe(qp[qb + 3u]))) * d
            }
            for [unroll_full] (c in range(4)) {
                let xv = unsafe(x0[uint(c) * 32u])
                acc += lw[c].x * xv.x + lw[c].y * xv.y + lw[c].z * xv.z + lw[c].w * xv.w
            }
            sp = unsafe(sp + 272)
            qp = unsafe(qp + 544)
            x0 = unsafe(x0 + 128)
            k += 128u
        }
        while (k < nb4) {
            let bi = eb + row * nb + k / 8u
            let d = float(wsh[bi * 17u])
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let xv = x[x0e + k]
            acc += (float(wqb[qb]) * xv.x + float(wqb[qb + 1u]) * xv.y +
                float(wqb[qb + 2u]) * xv.z + float(wqb[qb + 3u]) * xv.w) * d
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[st * bys + slot * ddim + row] = acc
        }
    }
}

// Expert-indexed Q4_K GEMV — MetalKqGemvK4's dot; the superblock index shifts by sel[slot] * esb.
// x rides a float4 view: the 32-scalar-x-load form was load-issue-bound at the routed expert
// shapes (nfe-row planes) — the vector view is 2.25x there (lab: bench_metal_moe_lab, 142→321 wGB/s).
class MetalMoeGemvK4 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 @role = "read" x4 : array<float4>   // activations, float4 view (every x stride is % 4)
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 8 esb : uint        // superblocks per expert plane (ege / 256)
    @uniform @binding = 9 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 10 ss : uint        // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 11 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 12 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_k4_msl")]
    def metal_moe_gemv_k4 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * esb
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb4 = (xoff + ib * 256u + iq * 64u + ir * 8u) / 4u
            var sumy : float[4]
            for [unroll_full] (h in range(4)) {   // h: yl lo/hi, yh lo/hi — float4 pairs at 0/8/32/40
                let xi = yb4 + (h < 2 ? uint(h) * 8u : 16u + uint(h) * 8u)
                let va = x4[xi]
                let vb = x4[xi + 1u]
                let o = (h % 2) * 8
                if (h < 2) {   // constant under unroll_full — folds per iteration
                    yl[o] = va.x
                    yl[o + 1] = va.y
                    yl[o + 2] = va.z
                    yl[o + 3] = va.w
                    yl[o + 4] = vb.x
                    yl[o + 5] = vb.y
                    yl[o + 6] = vb.z
                    yl[o + 7] = vb.w
                } else {
                    yh[o] = va.x
                    yh[o + 1] = va.y
                    yh[o + 2] = va.z
                    yh[o + 3] = va.w
                    yh[o + 4] = vb.x
                    yh[o + 5] = vb.y
                    yh[o + 6] = vb.z
                    yh[o + 7] = vb.w
                }
                sumy[h] = va.x + va.y + va.z + va.w + vb.x + vb.y + vb.z + vb.w
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let sv = ks4[blk]
                let hw0 = (sv.y >> (16u * iq)) & 0xFFFF
                let hw2 = (sv.z >> (16u * iq)) & 0xFFFF
                let hw4 = (sv.w >> (16u * iq)) & 0xFFFF
                let sc0 = hw0 & 0x3F3F
                let sc1 = hw2 & 0x3F3F
                let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
                let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
                let qb = blk * 32u + iq * 8u + ir * 2u
                let q1a = kqu[qb]
                let q1b = kqu[qb + 1u]
                let q2a = kqu[qb + 16u]
                let q2b = kqu[qb + 17u]
                var acc1 : float[4]
                var acc2 : float[4]
                for [unroll_full] (i in range(4)) {
                    let sh = 16u * (uint(i) & 1u)
                    let q1 = ((i < 2 ? q1a : q1b) >> sh) & 0xFFFF
                    let q2 = ((i < 2 ? q2a : q2b) >> sh) & 0xFFFF
                    acc1[0] += yl[2 * i] * float(q1 & 0x000F)
                    acc1[1] += yl[2 * i + 1] * float(q1 & 0x0F00)
                    acc1[2] += yl[2 * i + 8] * float(q1 & 0x00F0)
                    acc1[3] += yl[2 * i + 9] * float(q1 & 0xF000)
                    acc2[0] += yh[2 * i] * float(q2 & 0x000F)
                    acc2[1] += yh[2 * i + 1] * float(q2 & 0x0F00)
                    acc2[2] += yh[2 * i + 8] * float(q2 & 0x00F0)
                    acc2[3] += yh[2 * i + 9] * float(q2 & 0xF000)
                }
                let dd = float(ksh[blk * 8u])
                let dm = float(ksh[blk * 8u + 1u])
                sumf[r] += dd * ((acc1[0] + 0.00390625 * acc1[1]) * float(sc0 & 255u)
                    + (acc1[2] + 0.00390625 * acc1[3]) * float(sc0 >> 8u) * 0.0625
                    + (acc2[0] + 0.00390625 * acc2[1]) * float(sc2 & 255u)
                    + (acc2[2] + 0.00390625 * acc2[3]) * float(sc2 >> 8u) * 0.0625)
                sumf[r] -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                    + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[y0 + first_row + uint(r)] = s
            }
        }
    }
}

// Expert-indexed Q5_K GEMV — MetalKqGemvK5's dot with the expert superblock shift.
class MetalMoeGemvK5 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B scale blocks, half view
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 8 esb : uint        // superblocks per expert plane (ege / 256)
    @uniform @binding = 9 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 10 ss : uint        // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 11 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 12 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_k5_msl")]
    def metal_moe_gemv_k5 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hb0 = 2u * iq
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * esb
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = xoff + ib * 256u + iq * 64u + l0
            var sumy : float[4]
            for [unroll_full] (l in range(8)) {
                yl[l] = x[yb + uint(l)]
                sumy[0] += yl[l]
                yl[l + 8] = x[yb + 32u + uint(l)]
                sumy[1] += yl[l + 8]
                yh[l] = x[yb + 128u + uint(l)]
                sumy[2] += yh[l]
                yh[l + 8] = x[yb + 160u + uint(l)]
                sumy[3] += yh[l + 8]
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let sv = ks4[blk]
                let hw0 = (sv.y >> (16u * iq)) & 0xFFFF
                let hw2 = (sv.z >> (16u * iq)) & 0xFFFF
                let hw4 = (sv.w >> (16u * iq)) & 0xFFFF
                let sc0 = hw0 & 0x3F3F
                let sc1 = hw2 & 0x3F3F
                let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
                let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
                let qs0 = blk * 160u + iq * 32u + l0
                let qh0 = blk * 160u + 128u + l0
                var acc : float[4]
                for [unroll_full] (l in range(8)) {
                    let q1 = uint(kqb[qs0 + uint(l)])
                    let q2 = uint(kqb[qs0 + 64u + uint(l)])
                    let h = uint(kqb[qh0 + uint(l)]) >> hb0
                    acc[0] += yl[l] * float((q1 & 0x0F) | ((h & 1u) << 4u))
                    acc[1] += yl[l + 8] * float((q1 >> 4u) | ((h & 2u) << 3u))
                    acc[2] += yh[l] * float((q2 & 0x0F) | ((h & 16u) << 0u))
                    acc[3] += yh[l + 8] * float((q2 >> 4u) | ((h & 32u) >> 1u))
                }
                let dd = float(ksh[blk * 8u])
                let dm = float(ksh[blk * 8u + 1u])
                sumf[r] += dd * (float(sc0 & 255u) * acc[0] + float(sc0 >> 8u) * acc[1]
                    + float(sc2 & 255u) * acc[2] + float(sc2 >> 8u) * acc[3])
                sumf[r] -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                    + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[y0 + first_row + uint(r)] = s
            }
        }
    }
}

// Expert-indexed Q6_K GEMV — MetalKqGemvK6's dot; the expert shift covers sub-scales AND the
// trailing f16 d plane (both indexed by the same region-relative superblock).
class MetalMoeGemvK6 {
    @ssbo @binding = 0 @role = "weight" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" kqb : array<uint8>   // k6 quant plane, byte view (ql at 192*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 8 esb : uint        // superblocks per expert plane (ege / 256)
    @uniform @binding = 9 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 10 ss : uint        // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 11 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 12 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_k6_msl")]
    def metal_moe_gemv_k6 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let is0 = ip * 8u + l0 / 16u
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * esb
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = xoff + ib * 256u + ip * 128u + l0
            for [unroll_full] (l in range(4)) {
                yl[4 * l] = x[yb + uint(l)]
                yl[4 * l + 1] = x[yb + 32u + uint(l)]
                yl[4 * l + 2] = x[yb + 64u + uint(l)]
                yl[4 * l + 3] = x[yb + 96u + uint(l)]
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let ql0 = blk * 192u + ip * 64u + l0
                let qh0 = blk * 192u + 128u + ip * 32u + l0
                let sb = blk * 16u + is0
                var sums : float[4]
                for [unroll_full] (l in range(4)) {
                    let q1 = uint(kqb[ql0 + uint(l)])
                    let q2 = uint(kqb[ql0 + 32u + uint(l)])
                    let h = uint(kqb[qh0 + uint(l)])
                    sums[0] += yl[4 * l] * (float((q1 & 0x0F) | ((h & 0x03) << 4u)) - 32.0)
                    sums[1] += yl[4 * l + 1] * (float((q2 & 0x0F) | ((h & 0x0C) << 2u)) - 32.0)
                    sums[2] += yl[4 * l + 2] * (float((q1 >> 4u) | (h & 0x30)) - 32.0)
                    sums[3] += yl[4 * l + 3] * (float((q2 >> 4u) | ((h & 0xC0) >> 2u)) - 32.0)
                }
                let s0 = float((int(kscb[sb]) ^ 128) - 128)
                let s2 = float((int(kscb[sb + 2u]) ^ 128) - 128)
                let s4 = float((int(kscb[sb + 4u]) ^ 128) - 128)
                let s6 = float((int(kscb[sb + 6u]) ^ 128) - 128)
                let dd = float(kdh[blk])
                sumf[r] += dd * (sums[0] * s0 + sums[1] * s2 + sums[2] * s4 + sums[3] * s6)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[y0 + first_row + uint(r)] = s
            }
        }
    }
}

// Expert-indexed MXFP4 GEMV (gpt-oss): a 32-elem block = 16 nibble bytes (byte b holds elems
// b LOW / b+16 HIGH — the split-half layout) + one E8M0 scale byte; value = doubled-e2m1
// magnitude x halved scale (2^(e-128)). Optional per-expert bias rows — gpt-oss's biased MoE.
class MetalMoeGemvMx4 {
    @ssbo @binding = 0 @role = "weight" mxu2 : array<uint2>  // nibble plane, uint2 view (block at 2*blk; off/2 % 8)
    @ssbo @binding = 1 @role = "weight" mxe : array<uint8>   // E8M0 plane, bound at off/32
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 @role = "write" y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 7 eblk : uint       // 32-elem blocks per expert plane (ege / 32)
    @uniform @binding = 8 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 9 ss : uint         // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 10 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 11 bys : uint       // per-stream y stride, elements (0 single)
    @ssbo @binding = 12 @role = "weight" wb : array<float>   // [ne x ddim] per-expert bias rows (hasb models)
    @uniform @binding = 13 hasb : uint
    @ssbo @binding = 15 @role = "weight" vtab : array<float> // [16] signed doubled-e2m1 values (host-built)
    @workgroup vt : float[32]

    [metal_kernel(name="metal_moe_gemv_mx4_msl")]
    def metal_moe_gemv_mx4 {
        // the lcpp mul_mv_mxfp4 shape (moe-lab lcppe rung, 239->301 wGB/s at gpt-oss shapes):
        // 16 blocks in flight x 2 threads/block, yl[4] float4 x prefetch shared across both rows,
        // uint2 weight loads, tgmem value LUT, e8m0 scale computed in ALU (no dependent etab load)
        let lane = gl_SubgroupInvocationID
        vt[lane] = vtab[lane % 16u]
        barrier()
        let ix = lane / 2u   // 16-way block stride
        let it = lane % 2u   // 2 threads per block, 8 nibble bytes (one uint2) each
        let nb = ndim / 32u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let e = sel[st * ss + slot]
        let eb = e * eblk
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let x4 = (xoff + ib * 32u + it * 8u) / 4u
            let yl0 = xf[x4]
            let yl1 = xf[x4 + 4u]
            let yl2 = xf[x4 + 1u]
            let yl3 = xf[x4 + 5u]
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let u2 = mxu2[blk * 2u + it]
                let u0 = u2.x
                let u1 = u2.y
                let acc1 = yl0 * float4(vt[u0 & 15u], vt[(u0 >> 8u) & 15u], vt[(u0 >> 16u) & 15u], vt[(u0 >> 24u) & 15u])
                let acc2 = yl1 * float4(vt[(u0 >> 4u) & 15u], vt[(u0 >> 12u) & 15u], vt[(u0 >> 20u) & 15u], vt[u0 >> 28u])
                let acc3 = yl2 * float4(vt[u1 & 15u], vt[(u1 >> 8u) & 15u], vt[(u1 >> 16u) & 15u], vt[(u1 >> 24u) & 15u])
                let acc4 = yl3 * float4(vt[(u1 >> 4u) & 15u], vt[(u1 >> 12u) & 15u], vt[(u1 >> 20u) & 15u], vt[u1 >> 28u])
                let am = (acc1 + acc3) + (acc2 + acc4)
                let ex = uint(mxe[blk])
                let sc = uint_bits_to_float(ex < 2u ? (0x00200000u << ex) : ((ex - 1u) << 23u))
                sumf[r] += sc * ((am.x + am.y) + (am.z + am.w))
            }
            ib += 16u
        }
        for [unroll_full] (r in range(2)) {
            let sr = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                var outv = sr
                if (hasb != 0u) {
                    outv += wb[e * ddim + first_row + uint(r)]
                }
                y[y0 + first_row + uint(r)] = outv
            }
        }
    }
}

// Expert-indexed Q5_1 GEMV — per-32 planes (a 20B quant block = 5 uints: 16 nibble bytes in the
// k/k+16 split-half pairing + the qh u32; 4B scales = f16 d, m). value = d*q + m with
// q = nibble | qh_bit << 4, folded as acc += d*Σ(q·x) + m*Σx per block (f32 activations).
class MetalMoeGemvQ51 {
    @ssbo @binding = 0 @role = "weight" wqu : array<uint>    // quant plane, uint view (block at 5*blk; qh at +4)
    @ssbo @binding = 1 @role = "weight" wsh : array<float16> // scale plane (d at 2*blk, m at 2*blk+1)
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 @role = "write" y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 7 eblk : uint       // 32-elem blocks per expert plane (ege / 32)
    @uniform @binding = 8 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 9 ss : uint         // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 10 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 11 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_q51_msl")]
    def metal_moe_gemv_q51 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u    // 4-way block stride
        let it = lane % 8u    // 8 threads per block, one float4 (4 elems) each
        let nb = ndim / 32u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * eblk
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        // this thread's 4 elems: half = it/4 selects the nibble half, ku = it%4 the quant uint
        let half = it / 4u
        let ku = it % 4u
        let nsh = half * 4u
        let hb = ku * 4u + half * 16u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let xv = xf[(xoff + ib * 32u) / 4u + it]
            let sx = xv.x + xv.y + xv.z + xv.w
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let qb = blk * 5u
                let u = wqu[qb + ku]
                let qh = wqu[qb + 4u] >> hb
                var qx = float((u >> nsh) & 15u) * xv.x
                qx += float((u >> (8u + nsh)) & 15u) * xv.y
                qx += float((u >> (16u + nsh)) & 15u) * xv.z
                qx += float((u >> (24u + nsh)) & 15u) * xv.w
                var hx = (qh & 1u) != 0u ? xv.x : 0.0
                hx += (qh & 2u) != 0u ? xv.y : 0.0
                hx += (qh & 4u) != 0u ? xv.z : 0.0
                hx += (qh & 8u) != 0u ? xv.w : 0.0
                sumf[r] += float(wsh[blk * 2u]) * (qx + 16.0 * hx) + float(wsh[blk * 2u + 1u]) * sx
            }
            ib += 4u
        }
        for [unroll_full] (r in range(2)) {
            let sr = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[y0 + first_row + uint(r)] = sr
            }
        }
    }
}

// Weighted top-k reduce per stream (grid.y): y[st*dim + i] = sum_j w[st][j] * dn[(st*k+j)*dim + i]
// — the CPU slot-order axpy chain as one deterministic in-kernel loop. Overwrites y (the dense
// w2 site's write shape).
[metal_dispatch(name = "enc_moe_combine", pso = "g_pso_moe_combine", tg = 256, grid = "dim/256, nst", params = "dim : int64, nst : int64")]
class MetalMoeCombine {
    @ssbo @binding = 0 dn : array<float>    // [streams x k x dim] per-slot down outputs
    @ssbo @binding = 1 selw : array<float>  // the selection buffer's float view — w of stream s at s*2k + k
    @ssbo @binding = 2 y : array<float>     // [streams x dim] combined out
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 nk : uint

    [metal_kernel(name="metal_moe_combine_msl")]
    def metal_moe_combine {
        let gid = gl_GlobalInvocationID.x
        if (gid >= dim) {
            return
        }
        let st = gl_WorkGroupID.y
        let w0 = st * 2u * nk + nk
        let dn0 = st * nk * dim
        var acc = 0.0
        var j = 0u
        while (j < nk) {
            acc += selw[w0 + j] * dn[dn0 + j * dim + gid]
            j++
        }
        y[st * dim + gid] = acc
    }
}

// gpt-oss clamped swiglu (stage 4): x = min(g, 7), y = clamp(u, -7, 7),
// g = (x / (1 + exp(-1.702 x))) * (y + 1) — the clamps and the +1 are trained weights' contract.
// SHARED with the decode rail (dasllama_metal_decode's enc_ew2 binds slots 0-2) — keep the layout.
class MetalSwigluOai {
    @ssbo @binding = 0 @role = "readwrite" g : array<float4>
    @ssbo @binding = 1 @role = "read" u : array<float4>
    @uniform @binding = 2 total : uint      // scalar element count

    [metal_kernel(name="metal_swiglu_oai_msl")]
    def metal_swiglu_oai {
        let gid = gl_GlobalInvocationID.x
        if (gid * 4u >= total) {
            return
        }
        let x = min(g[gid], float4(7.0))
        let y = min(max(u[gid], float4(-7.0)), float4(7.0))   // nolint:STYLE019 — the MSL emitter doesn't lower clamp
        g[gid] = (x / (float4(1.0) + exp(-1.702 * x))) * (y + float4(1.0))
    }
}

// gemma4 MoE router input (stage 3): y = x * rsqrt(mean(x^2) + eps) * (1/sqrt(dim)) * rscale[k],
// one tg per row. f32 accumulation (the CPU runs this sum in double for top-8 tie stability;
// no GPU doubles — rare tie flips ride the tolerance methodology, per the wave ruling).
[metal_dispatch(name = "enc_g4_router_norm", pso = "g_pso_g4rnorm", tgmem = "metal_g4_router_norm_msl_tgmem", tg = 256, grid = "nst", params = "nst : int64")]
class MetalG4RouterNorm {
    @ssbo @binding = 0 x : array<float>     // [rows x dim]
    @ssbo @binding = 1 @role = "weight" rs : array<float>    // [dim] per-channel router scale
    @ssbo @binding = 2 y : array<float>     // [rows x dim]
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 eps : float
    @uniform @binding = 5 invd : float      // 1/sqrt(dim)
    @workgroup partial : float[32]
    @workgroup inv_sh : float

    [metal_kernel(name="metal_g4_router_norm_msl")]
    def metal_g4_router_norm {
        let row = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        // the norm scale is an inverse TEMPERATURE on the router softmax (selection is
        // scale-invariant; the expert WEIGHTS are not) — Kahan-compensated f32 tracks the
        // CPU's double sum to f32 rounding, and 1/sqrt avoids the fast-rsqrt ulp error
        var ss = 0.0
        var comp = 0.0
        var k = lid
        while (k < dim) {
            let vv = x[row * dim + k]
            let yv = vv * vv - comp
            let tt = ss + yv
            comp = (tt - ss) - yv
            ss = tt
            k += gl_WorkGroupSize.x
        }
        ss = simd_sum(ss)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ss
        }
        barrier()
        if (lid == 0u) {
            var tot = 0.0
            var tc = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                let yv = partial[sgi] - tc
                let tt = tot + yv
                tc = (tt - tot) - yv
                tot = tt
                sgi++
            }
            inv_sh = (1.0 / sqrt(tot / float(dim) + eps)) * invd
        }
        barrier()
        let inv = inv_sh
        k = lid
        while (k < dim) {
            y[row * dim + k] = x[row * dim + k] * inv * rs[k]
            k += gl_WorkGroupSize.x
        }
    }
}

// gemma4 per-expert down-scale fold (stage 3): selw[row][j] *= ds[seli[row][j]] — the CPU
// folds ffn_down_exps_s into the selection weights right after select.
[metal_dispatch(name = "enc_moe_wscale", pso = "g_pso_moe_wscale", tg = 32, grid = "nst", params = "nst : int64")]
class MetalMoeWScale {
    @ssbo @binding = 0 seli : array<uint>   // selection rows, uint view (idx of row r at r*2k)
    @ssbo @binding = 1 @role = "alias" selw : array<float>  // the same buffer, float view (w of row r at r*2k + k)
    @ssbo @binding = 2 @role = "weight" ds : array<float>    // [ne] this layer's per-expert down scales
    @uniform @binding = 3 nk : uint

    [metal_kernel(name="metal_moe_wscale_msl")]
    def metal_moe_wscale {
        let row = gl_WorkGroupID.x
        let j = gl_LocalInvocationID.x
        if (j < nk) {
            selw[row * 2u * nk + nk + j] *= ds[seli[row * 2u * nk + j]]
        }
    }
}

// One simdgroup per output row over the 34B q8 blob (f16 scale + 32 int8 inline — the SAME
// dual-view W the mul_mm reads, bound at the tensor's byte offset): lane owns chunk lane%8 of
// every 4th block, 4 in flight; the scale rides the quants' cacheline. Width-agnostic — rows/tg via DASLLAMA_METAL_GEMV_TG (g_gemv_rows).
[metal_dispatch(name = "enc_gemv_c", pso = "g_pso_gemv", tg = "g_gemv_rows*32", grid = "d/g_gemv_rows", params = "d : int64")]
class MetalQ8Gemv {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" @off = "boff" wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 x : array<float4>    // [n] f32 vector, float4 view
    @ssbo @binding = 3 @off = "yoff" @span = "d*4" y : array<float>     // [d] outputs
    @uniform @binding = 4 ndim : uint       // n (reduction dim, % 32)
    @uniform @binding = 5 ddim : uint       // d (output rows)

    [metal_kernel(name="metal_q8_gemv_msl")]
    def metal_q8_gemv {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let nb4 = ndim / 4u
        var sp = unsafe(addr(wsh[(row * nb + lane / 8u) * 17u]))
        var qp = unsafe(addr(wqb[(row * nb + lane / 8u) * 34u + 2u + (lane % 8u) * 4u]))
        var x0 = unsafe(addr(x[lane]))
        var acc = 0.0
        var lw : float4[4]
        var k = lane
        while (k + 96u < nb4) {
            // all 4 chunk dequants batch AHEAD of the fma chains (the MvB2 load batching)
            for [unroll_full] (c in range(4)) {
                let d = float(unsafe(sp[uint(c) * 68u]))   // chunk c sits 4 blocks (68 halves) on
                let qb = uint(c) * 136u                    // ... and 136 quant bytes
                lw[c] = float4(float(unsafe(qp[qb])), float(unsafe(qp[qb + 1u])),
                    float(unsafe(qp[qb + 2u])), float(unsafe(qp[qb + 3u]))) * d
            }
            for [unroll_full] (c in range(4)) {
                let xv = unsafe(x0[uint(c) * 32u])
                acc += lw[c].x * xv.x + lw[c].y * xv.y + lw[c].z * xv.z + lw[c].w * xv.w
            }
            sp = unsafe(sp + 272)           // 16 blocks x 17 halves per stripe
            qp = unsafe(qp + 544)           // 16 blocks x 34 bytes
            x0 = unsafe(x0 + 128)
            k += 128u
        }
        while (k < nb4) {
            let bi = row * nb + k / 8u
            let d = float(wsh[bi * 17u])
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let xv = x[k]
            acc += (float(wqb[qb]) * xv.x + float(wqb[qb + 1u]) * xv.y +
                float(wqb[qb + 2u]) * xv.z + float(wqb[qb + 3u]) * xv.w) * d
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[row] = acc
        }
    }
}

// Q4_K: lane owns 32 elems of every 4th superblock; lcpp's kmask halfword scale decode +
// nibble-mask acc folds (x16/x256 groups folded by pow2 constants).
class MetalKqGemvK4 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k4_msl")]
    def metal_kq_gemv_k4 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + iq * 64u + ir * 8u
            var sumy : float[4]
            for [unroll_full] (i in range(8)) {
                yl[i] = x[yb + uint(i)]
                sumy[0] += yl[i]
                yl[i + 8] = x[yb + 32u + uint(i)]
                sumy[1] += yl[i + 8]
                yh[i] = x[yb + 128u + uint(i)]
                sumy[2] += yh[i]
                yh[i + 8] = x[yb + 160u + uint(i)]
                sumy[3] += yh[i + 8]
            }
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let sv = ks4[blk]
                let hw0 = (sv.y >> (16u * iq)) & 0xFFFF
                let hw2 = (sv.z >> (16u * iq)) & 0xFFFF
                let hw4 = (sv.w >> (16u * iq)) & 0xFFFF
                let sc0 = hw0 & 0x3F3F
                let sc1 = hw2 & 0x3F3F
                let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
                let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
                let qb = blk * 32u + iq * 8u + ir * 2u
                let q1a = kqu[qb]
                let q1b = kqu[qb + 1u]
                let q2a = kqu[qb + 16u]
                let q2b = kqu[qb + 17u]
                var acc1 : float[4]
                var acc2 : float[4]
                for [unroll_full] (i in range(4)) {
                    let sh = 16u * (uint(i) & 1u)
                    let q1 = ((i < 2 ? q1a : q1b) >> sh) & 0xFFFF
                    let q2 = ((i < 2 ? q2a : q2b) >> sh) & 0xFFFF
                    acc1[0] += yl[2 * i] * float(q1 & 0x000F)
                    acc1[1] += yl[2 * i + 1] * float(q1 & 0x0F00)
                    acc1[2] += yl[2 * i + 8] * float(q1 & 0x00F0)
                    acc1[3] += yl[2 * i + 9] * float(q1 & 0xF000)
                    acc2[0] += yh[2 * i] * float(q2 & 0x000F)
                    acc2[1] += yh[2 * i + 1] * float(q2 & 0x0F00)
                    acc2[2] += yh[2 * i + 8] * float(q2 & 0x00F0)
                    acc2[3] += yh[2 * i + 9] * float(q2 & 0xF000)
                }
                let dd = float(ksh[blk * 8u])
                let dm = float(ksh[blk * 8u + 1u])
                sumf[r] += dd * ((acc1[0] + 0.00390625 * acc1[1]) * float(sc0 & 255u)
                    + (acc1[2] + 0.00390625 * acc1[3]) * float(sc0 >> 8u) * 0.0625
                    + (acc2[0] + 0.00390625 * acc2[1]) * float(sc2 & 255u)
                    + (acc2[2] + 0.00390625 * acc2[3]) * float(sc2 >> 8u) * 0.0625)
                sumf[r] -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                    + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// Q5_K: int-composed q5 (nibble | qh bit << 4) — one fma per element; same kmask scale decode.
// The quant plane is [qs 128][qh 32] per superblock, read through the byte view.
class MetalKqGemvK5 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B scale blocks, half view
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k5_msl")]
    def metal_kq_gemv_k5 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hb0 = 2u * iq
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + iq * 64u + l0
            var sumy : float[4]
            for [unroll_full] (l in range(8)) {
                yl[l] = x[yb + uint(l)]
                sumy[0] += yl[l]
                yl[l + 8] = x[yb + 32u + uint(l)]
                sumy[1] += yl[l + 8]
                yh[l] = x[yb + 128u + uint(l)]
                sumy[2] += yh[l]
                yh[l + 8] = x[yb + 160u + uint(l)]
                sumy[3] += yh[l + 8]
            }
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let sv = ks4[blk]
                let hw0 = (sv.y >> (16u * iq)) & 0xFFFF
                let hw2 = (sv.z >> (16u * iq)) & 0xFFFF
                let hw4 = (sv.w >> (16u * iq)) & 0xFFFF
                let sc0 = hw0 & 0x3F3F
                let sc1 = hw2 & 0x3F3F
                let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
                let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
                let qs0 = blk * 160u + iq * 32u + l0
                let qh0 = blk * 160u + 128u + l0
                var acc : float[4]
                for [unroll_full] (l in range(8)) {
                    let q1 = uint(kqb[qs0 + uint(l)])
                    let q2 = uint(kqb[qs0 + 64u + uint(l)])
                    let h = uint(kqb[qh0 + uint(l)]) >> hb0
                    acc[0] += yl[l] * float((q1 & 0x0F) | ((h & 1u) << 4u))
                    acc[1] += yl[l + 8] * float((q1 >> 4u) | ((h & 2u) << 3u))
                    acc[2] += yh[l] * float((q2 & 0x0F) | ((h & 16u) << 0u))
                    acc[3] += yh[l + 8] * float((q2 >> 4u) | ((h & 32u) >> 1u))
                }
                let dd = float(ksh[blk * 8u])
                let dm = float(ksh[blk * 8u + 1u])
                sumf[r] += dd * (float(sc0 & 255u) * acc[0] + float(sc0 >> 8u) * acc[1]
                    + float(sc2 & 255u) * acc[2] + float(sc2 >> 8u) * acc[3])
                sumf[r] -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                    + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// Q5_K, select-form nr0=1 twin (the gemv-lab b1c winner): one row per simdgroup, lcpp's q5 lane
// map, qh via mask-selects. Wins at n < 3072 at EVERY d (-9..-11% vs b2r) and the small-d kv
// shape (b2r under-occupied); b2r keeps n >= 3072. enc_kq_gemv picks per site.
class MetalKqGemvK5C {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 @role = "weight" kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k5c_msl")]
    def metal_kq_gemv_k5c {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hm1 = 1u << (2u * iq)
        let hm2 = hm1 << 1u
        let hm3 = hm1 << 4u
        let hm4 = hm2 << 4u
        let nb = ndim / 256u
        let first_row = gl_WorkGroupID.x * 2u + gl_SubgroupID
        var yl : float[16]
        var yh : float[16]
        var sumf = 0.0
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + iq * 64u + l0
            var sumy : float[4]
            for [unroll_full] (l in range(8)) {
                yl[l] = x[yb + uint(l)]
                sumy[0] += yl[l]
                yl[l + 8] = x[yb + 32u + uint(l)]
                sumy[1] += yl[l + 8]
                yh[l] = x[yb + 128u + uint(l)]
                sumy[2] += yh[l]
                yh[l + 8] = x[yb + 160u + uint(l)]
                sumy[3] += yh[l + 8]
            }
            let blk = first_row * nb + ib
            let sv = ks4[blk]
            let hw0 = (sv.y >> (16u * iq)) & 0xFFFF
            let hw2 = (sv.z >> (16u * iq)) & 0xFFFF
            let hw4 = (sv.w >> (16u * iq)) & 0xFFFF
            let sc0 = hw0 & 0x3F3F
            let sc1 = hw2 & 0x3F3F
            let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
            let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
            let qs0 = blk * 160u + iq * 32u + l0
            let qh0 = blk * 160u + 128u + l0
            var acc1 : float[4]
            var acc2 : float[4]
            for [unroll_full] (l in range(8)) {
                let q1 = uint(kqb[qs0 + uint(l)])
                let q2 = uint(kqb[qs0 + 64u + uint(l)])
                let h = uint(kqb[qh0 + uint(l)])
                acc1[0] += yl[l] * float(q1 & 0x0F)
                acc1[1] += yl[l + 8] * float(q1 & 0xF0)
                acc1[2] += yh[l] * float(q2 & 0x0F)
                acc1[3] += yh[l + 8] * float(q2 & 0xF0)
                acc2[0] += (h & hm1) != 0u ? yl[l] : 0.0
                acc2[1] += (h & hm2) != 0u ? yl[l + 8] : 0.0
                acc2[2] += (h & hm3) != 0u ? yh[l] : 0.0
                acc2[3] += (h & hm4) != 0u ? yh[l + 8] : 0.0
            }
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            sumf += dd * (float(sc0 & 255u) * (acc1[0] + 16.0 * acc2[0])
                + float(sc0 >> 8u) * (acc1[1] * 0.0625 + 16.0 * acc2[1])
                + float(sc2 & 255u) * (acc1[2] + 16.0 * acc2[2])
                + float(sc2 >> 8u) * (acc1[3] * 0.0625 + 16.0 * acc2[3]))
            sumf -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            ib += 4u
        }
        let s = simd_sum(sumf)
        if (lane == 0u && first_row < ddim) {
            y[first_row] = s
        }
    }
}

// Q6_K: 6-bit q = nibble | 2 qh bits, minus 32; per-16-element SIGNED sub-scales as 4 scattered
// byte loads (the uint4+select form costs 2 resident simdgroups); d off the trailing f16 plane.
class MetalKqGemvK6 {
    @ssbo @binding = 0 @role = "weight" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" kqb : array<uint8>   // k6 quant plane, byte view (ql at 192*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k6_msl")]
    def metal_kq_gemv_k6 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let is0 = ip * 8u + l0 / 16u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + ip * 128u + l0
            for [unroll_full] (l in range(4)) {
                yl[4 * l] = x[yb + uint(l)]
                yl[4 * l + 1] = x[yb + 32u + uint(l)]
                yl[4 * l + 2] = x[yb + 64u + uint(l)]
                yl[4 * l + 3] = x[yb + 96u + uint(l)]
            }
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let ql0 = blk * 192u + ip * 64u + l0
                let qh0 = blk * 192u + 128u + ip * 32u + l0
                let sb = blk * 16u + is0
                var sums : float[4]
                for [unroll_full] (l in range(4)) {
                    let q1 = uint(kqb[ql0 + uint(l)])
                    let q2 = uint(kqb[ql0 + 32u + uint(l)])
                    let h = uint(kqb[qh0 + uint(l)])
                    sums[0] += yl[4 * l] * (float((q1 & 0x0F) | ((h & 0x03) << 4u)) - 32.0)
                    sums[1] += yl[4 * l + 1] * (float((q2 & 0x0F) | ((h & 0x0C) << 2u)) - 32.0)
                    sums[2] += yl[4 * l + 2] * (float((q1 >> 4u) | (h & 0x30)) - 32.0)
                    sums[3] += yl[4 * l + 3] * (float((q2 >> 4u) | ((h & 0xC0) >> 2u)) - 32.0)
                }
                let s0 = float((int(kscb[sb]) ^ 128) - 128)
                let s2 = float((int(kscb[sb + 2u]) ^ 128) - 128)
                let s4 = float((int(kscb[sb + 4u]) ^ 128) - 128)
                let s6 = float((int(kscb[sb + 6u]) ^ 128) - 128)
                let dd = float(kdh[blk])
                sumf[r] += dd * (sums[0] * s0 + sums[1] * s2 + sums[2] * s4 + sums[3] * s6)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

class MetalKqMvB2K4 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y row stride, elements
    @uniform @binding = 8 nr : uint         // live columns — stores gate on it

    [metal_kernel(name="metal_kq_mvb2_k4_msl")]
    def metal_kq_mvb2_k4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ndim / 4u
        var sumf : float[2]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let qb = blk * 32u + (q4i + uint(ch) * 2u) * 8u + lo
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qv = kqu[qb + uint(u)]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qv >> (8u * uint(e))) & 255u
                        w[4 * u + e] = float((byv >> nsh) & 0x0Fu) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + ((tx % 4u) < 2u ? 0u : 8u) + lo
                for [unroll_full] (b in range(2)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(2)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB4K4 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint

    [metal_kernel(name="metal_kq_mvb4_k4_msl")]
    def metal_kq_mvb4_k4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 4u
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ndim / 4u
        var sumf : float[4]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let qb = blk * 32u + (q4i + uint(ch) * 2u) * 8u + lo
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qv = kqu[qb + uint(u)]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qv >> (8u * uint(e))) & 255u
                        w[4 * u + e] = float((byv >> nsh) & 0x0Fu) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + ((tx % 4u) < 2u ? 0u : 8u) + lo
                for [unroll_full] (b in range(4)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && colbase + uint(b) < nr) {
                y[(colbase + uint(b)) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB2K6 {
    @ssbo @binding = 0 @role = "weight" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint

    [metal_kernel(name="metal_kq_mvb2_k6_msl")]
    def metal_kq_mvb2_k6 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u             // 16B offset within the slot, in uints
        let qoff = (s % 2u) * 8u + o4       // ql uint offset within the 128-half's 16 uints
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ndim / 4u
        var sumf : float[2]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 48u + ip * 16u + qoff
                let hb = blk * 48u + 32u + ip * 8u + o4
                let g = ip * 8u + s * 2u + (tx % 2u)
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 32.0
                var w : float[16]
                // 4+4 uint loads per chunk (the byte-view form was 32 scattered byte loads);
                // the slot's lsh/rsh dance is identically ((h >> 2s) & 3) << 4, vectorized
                for [unroll_full] (u in range(4)) {
                    let qu = kqu[qb + uint(u)]
                    let hu = kqu[hb + uint(u)]
                    let qv = ((qu >> shr) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                    for [unroll_full] (e in range(4)) {
                        w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                    }
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                for [unroll_full] (b in range(2)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(2)) {
            var sm = sumf[b]
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = sm
            }
        }
    }
}

class MetalKqMvB4K6 {
    @ssbo @binding = 0 @role = "weight" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" kscb : array<uint8>
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint

    [metal_kernel(name="metal_kq_mvb4_k6_msl")]
    def metal_kq_mvb4_k6 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 4u
        let nb = ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ndim / 4u
        var sumf : float[4]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 48u + ip * 16u + qoff
                let hb = blk * 48u + 32u + ip * 8u + o4
                let g = ip * 8u + s * 2u + (tx % 2u)
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 32.0
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qu = kqu[qb + uint(u)]
                    let hu = kqu[hb + uint(u)]
                    let qv = ((qu >> shr) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                    for [unroll_full] (e in range(4)) {
                        w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                    }
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                for [unroll_full] (b in range(4)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var sm = sumf[b]
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim && colbase + uint(b) < nr) {
                y[(colbase + uint(b)) * ys + row] = sm
            }
        }
    }
}

// k5 twins: k4's chunk geometry and kmask scale decode verbatim (q5_K shares the 12-byte scale
// block) + the qh 5th-bit overlay. The 160B plane rides the SAME uint view (40 uints/blk: qs at
// 40*blk, qh at 40*blk+32) — the overlay bit equals its kmask pair index, so scale indexing serves it too.
class MetalKqMvB2K5 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k5 plane, uint view (qs at 40*blk, qh at 40*blk+32)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y row stride, elements
    @uniform @binding = 8 nr : uint         // live columns — stores gate on it

    [metal_kernel(name="metal_kq_mvb2_k5_msl")]
    def metal_kq_mvb2_k5 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ndim / 4u
        var sumf : float[2]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            var hq : uint[4]
            for [unroll_full] (u in range(4)) {
                hq[u] = kqu[blk * 40u + 32u + lo + uint(u)]
            }
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let qb = blk * 40u + (q4i + uint(ch) * 2u) * 8u + lo
                let hsh = jl + 4u * uint(ch)
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qv = kqu[qb + uint(u)]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qv >> (8u * uint(e))) & 255u
                        let hb = (hq[u] >> (8u * uint(e) + hsh)) & 1u
                        w[4 * u + e] = float(((byv >> nsh) & 0x0Fu) | (hb << 4u)) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(2)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(2)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB4K5 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint

    [metal_kernel(name="metal_kq_mvb4_k5_msl")]
    def metal_kq_mvb4_k5 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 4u
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ndim / 4u
        var sumf : float[4]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            var hq : uint[4]
            for [unroll_full] (u in range(4)) {
                hq[u] = kqu[blk * 40u + 32u + lo + uint(u)]
            }
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let qb = blk * 40u + (q4i + uint(ch) * 2u) * 8u + lo
                let hsh = jl + 4u * uint(ch)
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qv = kqu[qb + uint(u)]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qv >> (8u * uint(e))) & 255u
                        let hb = (hq[u] >> (8u * uint(e) + hsh)) & 1u
                        w[4 * u + e] = float(((byv >> nsh) & 0x0Fu) | (hb << 4u)) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(4)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && colbase + uint(b) < nr) {
                y[(colbase + uint(b)) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB8K4 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y row stride, elements
    @uniform @binding = 8 nr : uint         // live columns — stores gate on it
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_k4_msl")]
    def metal_kq_mvb8_k4 {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ndim / 4u
        var acc : float4[8]
        var sb = 0u
        while (sb < nb) {
            barrier()
            for [unroll_full] (j in range(8)) {
                let slot = uint(j) * 64u + lid
                txp[slot] = x[(slot / 64u) * nb4 + sb * 64u + (slot % 64u)]
            }
            barrier()
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            let qb = blk * 32u + q4i * 8u + lo
            var qv : uint[8]
            for [unroll_full] (u in range(4)) {
                qv[u] = kqu[qb + uint(u)]
                qv[4 + u] = kqu[qb + 16u + uint(u)]
            }
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvv = qv[4 * ch + u]
                    w4[u] = float4(float((qvv >> nsh) & 15u), float((qvv >> (8u + nsh)) & 15u),
                        float((qvv >> (16u + nsh)) & 15u), float((qvv >> (24u + nsh)) & 15u)) * dl - float4(ml)
                }
                let cxl = (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(8)) {
                    for [unroll_full] (u in range(4)) {
                        acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB8K5 {
    @ssbo @binding = 0 @role = "weight" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k5 plane, uint view (qs at 40*blk, qh at 40*blk+32)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint
    @workgroup txp : float4[512]

    [metal_kernel(name="metal_kq_mvb8_k5_msl")]
    def metal_kq_mvb8_k5 {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ndim / 4u
        var acc : float4[8]
        var sb = 0u
        while (sb < nb) {
            barrier()
            for [unroll_full] (j in range(8)) {
                let slot = uint(j) * 64u + lid
                txp[slot] = x[(slot / 64u) * nb4 + sb * 64u + (slot % 64u)]
            }
            barrier()
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            let qb = blk * 40u + q4i * 8u + lo
            var qv : uint[8]
            var hq : uint[4]
            for [unroll_full] (u in range(4)) {
                qv[u] = kqu[qb + uint(u)]
                qv[4 + u] = kqu[qb + 16u + uint(u)]
                hq[u] = kqu[blk * 40u + 32u + lo + uint(u)]
            }
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let hsh = jl + 4u * uint(ch)
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvv = qv[4 * ch + u]
                    let hv = hq[u] >> hsh
                    w4[u] = float4(float(((qvv >> nsh) & 15u) | ((hv & 1u) << 4u)),
                        float(((qvv >> (8u + nsh)) & 15u) | (((hv >> 8u) & 1u) << 4u)),
                        float(((qvv >> (16u + nsh)) & 15u) | (((hv >> 16u) & 1u) << 4u)),
                        float(((qvv >> (24u + nsh)) & 15u) | (((hv >> 24u) & 1u) << 4u))) * dl - float4(ml)
                }
                let cxl = (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(8)) {
                    for [unroll_full] (u in range(4)) {
                        acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB8K6 {
    @ssbo @binding = 0 @role = "weight" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint
    @workgroup txp : float4[512]

    [metal_kernel(name="metal_kq_mvb8_k6_msl")]
    def metal_kq_mvb8_k6 {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ndim / 4u
        var acc : float4[8]
        var sb = 0u
        while (sb < nb) {
            barrier()
            for [unroll_full] (j in range(8)) {
                let slot = uint(j) * 64u + lid
                txp[slot] = x[(slot / 64u) * nb4 + sb * 64u + (slot % 64u)]
            }
            barrier()
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 48u + ip * 16u + qoff
                let hb = blk * 48u + 32u + ip * 8u + o4
                var qu : uint[4]
                var hu : uint[4]
                for [unroll_full] (u in range(4)) {
                    qu[u] = kqu[qb + uint(u)]
                    hu[u] = kqu[hb + uint(u)]
                }
                let g = ip * 8u + s * 2u + (tx % 2u)
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 32.0
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvc = ((qu[u] >> shr) & 0x0F0F0F0F) | (((hu[u] >> hsh) & 0x03030303) << 4u)
                    w4[u] = float4(float(qvc & 255u), float((qvc >> 8u) & 255u),
                        float((qvc >> 16u) & 255u), float((qvc >> 24u) & 255u)) * dl - float4(ml)
                }
                let cxl = ip * 32u + s * 8u + o4
                for [unroll_full] (b in range(8)) {
                    for [unroll_full] (u in range(4)) {
                        acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var sm = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = sm
            }
        }
    }
}

// Per-head RMSNorm on Q/K before rope (qwen3-line QK-norm): one threadgroup = hs threads per
// (head, row), nq q-head groups then the k heads, weight w[(k ? woffe : 0) + j]. Decode covers
// q+k in ONE grid over the fused QKV buffer; prefill dispatches separate q/k panels per-plane.
[metal_dispatch(name = "enc_qk_norm", pso = "g_pso_qknorm", tgmem = "metal_qk_norm_msl_tgmem", tg = "head_size", grid = "ntg, nrows", params = "ntg : int64, nrows : int64, head_size : int64")]
class MetalQkNorm {
    @ssbo @binding = 0 x : array<float>     // bound at the panel offset; normalized in place
    @ssbo @binding = 1 @role = "weight" w : array<float>
    @uniform @binding = 2 nq : uint         // q-head threadgroups before the k heads
    @uniform @binding = 3 koffe : uint      // k region's element offset within a row (the WRITE side)
    @uniform @binding = 4 rstride : uint    // row stride (qkvd / qd / kv_dim)
    @uniform @binding = 5 hs : uint         // head size == the threadgroup width
    @uniform @binding = 6 woffe : uint      // k plane's element offset in w
    @uniform @binding = 7 eps : float
    @uniform @binding = 8 soffe : uint      // READ offset — == koffe for in-place; the no-wv V
                                            // fusion reads the pre-norm K slice and writes the v
                                            // segment (the copy dispatch dissolves into this kernel)
    @workgroup part : float[16]     // one per simdgroup — hs <= 512 (gemma4 global heads)

    [metal_kernel(name="metal_qk_norm_msl")]
    def metal_qk_norm {
        let tgid = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        let isK = tgid >= nq
        let h = isK ? tgid - nq : tgid
        let rbase = gl_WorkGroupID.y * rstride + (isK ? soffe : 0u) + h * hs
        let wbase = gl_WorkGroupID.y * rstride + (isK ? koffe : 0u) + h * hs
        let v = x[rbase + lid]
        let s1 = simd_sum(v * v)
        if (gl_SubgroupInvocationID == 0u) {
            part[gl_SubgroupID] = s1
        }
        barrier()
        var total = 0.0
        var sg = 0u
        while (sg < gl_NumSubgroups) {
            total += part[sg]
            sg++
        }
        let inv = 1.0 / sqrt(total / float(hs) + eps)
        x[wbase + lid] = w[(isK ? woffe : 0u) + lid] * (v * inv)
    }
}

// Fused SwiGLU gate, one thread per element: g = silu(g) * u — swiglu exactly (the CPU side's
// exp4 polynomial vs the GPU exp differ inside the shared tolerance envelope).
class MetalSwiglu {
    @ssbo @binding = 0 @role = "readwrite" g : array<float>     // [ntok x hidden] W1 gate, in place
    @ssbo @binding = 1 @role = "read" u : array<float>     // [ntok x hidden] W3 up
    @uniform @binding = 2 total : uint      // ntok * hidden — grid guard

    [metal_kernel(name="metal_swiglu_msl")]
    def metal_swiglu {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let vv = g[gid]
        g[gid] = (vv / (1.0 + exp(-vv))) * u[gid]
    }
}

// GeGLU twin (gemma line): g = gelu(g) * u with the CPU's tanh-approx gelu (dasllama_math gelu —
// 0.5*v*(1+tanh(sqrt(2/pi)*(v + 0.044715*v^3)))); GPU tanh vs the CPU exp identity differ inside
// the shared tolerance envelope, same as swiglu's exp.
class MetalGeglu {
    @ssbo @binding = 0 @role = "readwrite" g : array<float>     // [ntok x hidden] W1 gate, in place
    @ssbo @binding = 1 @role = "read" u : array<float>     // [ntok x hidden] W3 up
    @uniform @binding = 2 total : uint      // ntok * hidden — grid guard

    [metal_kernel(name="metal_geglu_msl")]
    def metal_geglu {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let vv = g[gid]
        // tanh via the exp identity (the CPU geglu4's form) — Metal tanh NaNs past ~44,
        // while exp saturates: exp(INF) -> denom INF -> the term is 0
        let a = 0.7978845608028654 * (vv + 0.044715 * vv * vv * vv)
        g[gid] = 0.5 * vv * (2.0 - 2.0 / (1.0 + exp(2.0 * a))) * u[gid]
    }
}

// Residual add, one thread per element: x += y (add_inplace).
class MetalAdd {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "read" y : array<float>
    @uniform @binding = 2 total : uint      // grid guard
    @uniform @binding = 3 ascale : float    // (x + y) * ascale — gemma4 layer_out_scale scales
                                            // the WHOLE layer output; every other site binds 1.0

    [metal_kernel(name="metal_add_msl")]
    def metal_add {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] = (x[gid] + y[gid]) * ascale
    }
}

// Suppress-token pinning (gemma4): logits[ids[i]] = -1e30 after the soft cap — the CPU
// forward's pin value exactly; one thread per suppressed id.
[metal_dispatch(name = "enc_suppress_row", pso = "g_pso_suppress", tg = 64, grid = "n/64", params = "n : int64")]
class MetalSuppressRow {
    @ssbo @binding = 0 @off = "xoff" x : array<float>
    @ssbo @binding = 1 @role = "weight" ids : array<uint>
    @uniform @binding = 2 n : uint

    [metal_kernel(name="metal_suppress_row_msl")]
    def metal_suppress_row {
        let gid = gl_GlobalInvocationID.x
        if (gid >= n) {
            return
        }
        x[ids[gid]] = -1.0e30
    }
}

// Logit soft-cap, one thread per element: x = cap * tanh(x / cap) — softcap4's scalar reference
// (gemma2/gemma4 final_logit_softcap on the GPU classifier row).
[metal_dispatch(name = "enc_softcap_row", pso = "g_pso_softcap", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSoftcapRow {
    @ssbo @binding = 0 @off = "xoff" @span = "total*4" x : array<float>
    @uniform @binding = 1 total : uint      // grid guard
    @uniform @binding = 2 cap : float

    [metal_kernel(name="metal_softcap_row_msl")]
    def metal_softcap_row {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        // tanh via the exp identity (the CPU softcap4's form; Metal tanh NaNs past ~44)
        x[gid] = cap * (1.0 - 2.0 / (1.0 + exp(x[gid] * 2.0 / cap)))
    }
}

// Causal depthwise conv over the fused [q;k;v] rows + SiLU (dn_conv_step's batch form): one
// thread per (channel, position); positions reaching before the window read the channel-major
// history taps (the CPU dn_conv_state layout). Never writes history — MetalDnConvHist advances.
[metal_dispatch(name = "enc_dn_conv", pso = "g_pso_dn_conv", tg = 256, grid = "cd/256, npos", params = "cd : int64, npos : int64")]
class MetalDnConv {
    @ssbo @binding = 0 x : array<float>     // [npos x cd] fused qkv projection rows
    @ssbo @binding = 1 @off = "hoff" hist : array<float>  // [cd x taps] channel-major history (layer slice)
    @ssbo @binding = 2 @role = "weight" w : array<float>     // [cd x dconv] conv taps (raw fblob layout)
    @ssbo @binding = 3 y : array<float>     // [npos x cd] conv output, silu'd
    @uniform @binding = 4 cd : uint
    @uniform @binding = 5 dconv : uint
    @uniform @binding = 6 npos : uint

    [metal_kernel(name="metal_dn_conv_msl")]
    def metal_dn_conv {
        let ch = gl_GlobalInvocationID.x
        let p = gl_GlobalInvocationID.y
        if (ch >= cd) {
            return
        }
        let taps = dconv - 1u
        var acc = 0.0
        var k = 0u
        while (k < dconv) {
            let sp = int(p + k) - int(taps)
            let xv = sp >= 0 ? x[uint(sp) * cd + ch] : hist[ch * taps + (p + k)]
            acc += xv * w[ch * dconv + k]
            k++
        }
        y[p * cd + ch] = acc / (1.0 + exp(-acc))
    }
}

// Advance the channel-major conv history past an npos window: the new taps are the window's
// last `taps` input rows, shifted in from the old history when npos < taps (straddle). Reads
// land in registers before any write, so the in-place buffer update is safe per channel.
[metal_dispatch(name = "enc_dn_conv_hist", pso = "g_pso_dn_hist", tg = 256, grid = "cd/256", params = "cd : int64")]
class MetalDnConvHist {
    @ssbo @binding = 0 x : array<float>     // [npos x cd] the window's fused qkv rows
    @ssbo @binding = 1 @off = "hoff" hist : array<float>  // [cd x taps] channel-major history (layer slice)
    @uniform @binding = 2 cd : uint
    @uniform @binding = 3 dconv : uint
    @uniform @binding = 4 npos : uint
    @ssbo @binding = 5 @role = "alias" @off = "hmoff" histm : array<float> // dual-store: the npos-1 window's history (verify shadow region)
    @uniform @binding = 6 mid : uint        // 1 = also store the npos-1 window's history to histm

    [metal_kernel(name="metal_dn_conv_hist_msl")]
    def metal_dn_conv_hist {
        let ch = gl_GlobalInvocationID.x
        if (ch >= cd) {
            return
        }
        let taps = dconv - 1u
        var nh : float[8]                   // dconv <= 8 by the serve gate
        var k = 0u
        while (k < taps) {
            let src = int(npos + k) - int(taps)
            nh[k] = src >= 0 ? x[uint(src) * cd + ch] : hist[ch * taps + uint(src + int(taps))]
            k++
        }
        if (mid != 0u) {                    // every hist read precedes the in-place write below
            k = 0u
            while (k < taps) {
                let src = int(npos - 1u + k) - int(taps)
                histm[ch * taps + k] = src >= 0 ? x[uint(src) * cd + ch] : hist[ch * taps + uint(src + int(taps))]
                k++
            }
        }
        k = 0u
        while (k < taps) {
            hist[ch * taps + k] = nh[k]
            k++
        }
    }
}

// Per-head L2 norm over the conv-out q/k regions in place, q pre-scale folded (l2_norm_rows +
// scale_inplace): one simdgroup per (head block, position). CPU accumulates the square-sum in
// DOUBLE — the f32 simd tree here is the deltanet drift class (same as the Vulkan tier).
[metal_dispatch(name = "enc_dn_l2norm", pso = "g_pso_dn_l2", tg = 256, grid = "nblk/8, npos", params = "nblk : int64, npos : int64")]
class MetalDnL2Norm {
    @ssbo @binding = 0 y : array<float>     // [npos x cd] conv rows: q at 0, k at kd
    @uniform @binding = 1 cd : uint
    @uniform @binding = 2 kd : uint         // key_dim = nkh * ds
    @uniform @binding = 3 ds : uint         // 128 (gated)
    @uniform @binding = 4 eps : float
    @uniform @binding = 5 qscale : float    // 1/sqrt(ds); k blocks get 1

    [metal_kernel(name="metal_dn_l2norm_msl")]
    def metal_dn_l2norm {
        let blk = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let p = gl_WorkGroupID.y
        let nblk = (2u * kd) / ds
        if (blk >= nblk) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let base = p * cd + blk * ds + lane * 4u
        var v : float[4]
        var ss = 0.0
        for [unroll_full] (c in range(4)) {
            v[c] = y[base + uint(c)]
            ss += v[c] * v[c]
        }
        ss = simd_sum(ss)
        let sc = (1.0 / max(sqrt(ss), eps)) * (blk < kd / ds ? qscale : 1.0)
        for [unroll_full] (c in range(4)) {
            y[base + uint(c)] = v[c] * sc
        }
    }
}

// The fused sequential delta rule (llama.cpp's gated_delta_net shape): one simdgroup per
// (state column j, v-head) holds S[:,j] in registers and walks ALL npos tokens in-kernel —
// zero per-token state traffic; β/σ/softplus fold into the loop (redundant per column, trivial).
[metal_dispatch(name = "enc_dn_scan", pso = "g_pso_dn_scan", tg = 128, grid = "ds/4, nvh", params = "ds : int64, nvh : int64")]
class MetalDnScan {
    @ssbo @binding = 0 cv : array<float>    // [npos x cd] conv rows: q at 0 / k at kd / v at 2kd
    @ssbo @binding = 1 @off = "stoff" st : array<float>    // state layer slice [nvh x ds x ds] row-major (i·ds + j)
    @ssbo @binding = 2 braw : array<float>  // [npos x nvh] raw beta projections
    @ssbo @binding = 3 graw : array<float>  // [npos x nvh] raw alpha projections
    @ssbo @binding = 4 @role = "weight" aa : array<float>    // [nvh] ssm_a = -exp(A_log) (fblob row)
    @ssbo @binding = 5 @role = "weight" dtb : array<float>   // [nvh] dt_bias (fblob row)
    @ssbo @binding = 6 o : array<float>     // [npos x di] delta-rule output rows
    @uniform @binding = 7 cd : uint
    @uniform @binding = 8 kd : uint
    @uniform @binding = 9 ds : uint         // 128 (gated)
    @uniform @binding = 10 nvh : uint
    @uniform @binding = 11 nkh : uint
    @uniform @binding = 12 di : uint        // nvh * ds
    @uniform @binding = 13 npos : uint
    @ssbo @binding = 14 @role = "alias" @off = "stmoff" stm : array<float>  // dual-store: post-token-(npos-2) state (verify shadow region)
    @uniform @binding = 15 mid : uint       // 1 = store S after token npos-2 to stm

    [metal_kernel(name="metal_dn_scan_msl")]
    def metal_dn_scan {
        let j = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let h = gl_WorkGroupID.y
        if (j >= ds) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let kh = h % nkh
        let qoff = kh * ds + lane * 4u
        let koff = kd + kh * ds + lane * 4u
        let voff = 2u * kd + h * ds + j
        let a_h = aa[h]
        let dt_h = dtb[h]
        let s0 = h * ds * ds + (lane * 4u) * ds + j
        var ls : float[4]
        for [unroll_full] (c in range(4)) {
            ls[c] = st[s0 + uint(c) * ds]
        }
        var p = 0u
        while (p < npos) {
            let gr = graw[p * nvh + h] + dt_h
            let dec = exp(a_h * (gr > 20.0 ? gr : log(1.0 + exp(gr))))
            let beta = 1.0 / (1.0 + exp(-braw[p * nvh + h]))
            let cb = p * cd
            var kv4 : float[4]
            var sk = 0.0
            for [unroll_full] (c in range(4)) {
                kv4[c] = cv[cb + koff + uint(c)]
                ls[c] *= dec
                sk += kv4[c] * ls[c]
            }
            sk = simd_sum(sk)
            let d = (cv[cb + voff] - sk) * beta
            var yy = 0.0
            for [unroll_full] (c in range(4)) {
                ls[c] += kv4[c] * d
                yy += cv[cb + qoff + uint(c)] * ls[c]
            }
            yy = simd_sum(yy)
            if (lane == 0u) {
                o[p * di + h * ds + j] = yy
            }
            if (mid != 0u && p + 2u == npos) {
                for [unroll_full] (c in range(4)) {
                    stm[s0 + uint(c) * ds] = ls[c]
                }
            }
            p++
        }
        for [unroll_full] (c in range(4)) {
            st[s0 + uint(c) * ds] = ls[c]
        }
    }
}

// Gated out-norm (dn_token_core's tail): per (v-head, position) RMSNorm over ds with the
// SHARED ssm_norm row, then ⊙ SiLU(z) — in place on o, one simdgroup per (head, position).
[metal_dispatch(name = "enc_dn_gate", pso = "g_pso_dn_gate", tg = 256, grid = "nvh/8, npos", params = "nvh : int64, npos : int64")]
class MetalDnGate {
    @ssbo @binding = 0 o : array<float>     // [npos x di] delta-rule output, gated in place
    @ssbo @binding = 1 z : array<float>     // [npos x di] raw gate projections
    @ssbo @binding = 2 @role = "weight" wn : array<float>    // [ds] shared norm weight (fblob row)
    @uniform @binding = 3 ds : uint         // 128 (gated)
    @uniform @binding = 4 di : uint
    @uniform @binding = 5 eps : float

    [metal_kernel(name="metal_dn_gate_msl")]
    def metal_dn_gate {
        let h = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let p = gl_WorkGroupID.y
        if (h >= di / ds) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let base = p * di + h * ds + lane * 4u
        var v : float[4]
        var ss = 0.0
        for [unroll_full] (c in range(4)) {
            v[c] = o[base + uint(c)]
            ss += v[c] * v[c]
        }
        ss = simd_sum(ss)
        let inv = rsqrt(ss / float(ds) + eps)
        for [unroll_full] (c in range(4)) {
            let zz = z[base + uint(c)]
            o[base + uint(c)] = (wn[lane * 4u + uint(c)] * (v[c] * inv)) * (zz / (1.0 + exp(-zz)))
        }
    }
}

// Deinterleave the 2x-wide gated-attention Q projection (head h's q at 2h·hs, its gate at
// (2h+1)·hs): q lands in the qkv panel's q segment, the gate in its own rows — keeps the
// rope/attention classes untouched by the packing.
[metal_dispatch(name = "enc_dn_deint", pso = "g_pso_dn_deint", tg = 256, grid = "qd/256, npos", params = "qd : int64, npos : int64")]
class MetalDnDeint {
    @ssbo @binding = 0 qg : array<float>    // [npos x 2qd] packed [q|gate] projection rows
    @ssbo @binding = 1 q : array<float>     // qkv panel, q segment at row stride qs
    @ssbo @binding = 2 gt : array<float>    // [npos x qd] sigmoid-gate rows
    @uniform @binding = 3 hs : uint
    @uniform @binding = 4 qd : uint
    @uniform @binding = 5 qs : uint         // q panel row stride, elements (qkvd; decode binds qd)

    [metal_kernel(name="metal_dn_deint_msl")]
    def metal_dn_deint {
        let i = gl_GlobalInvocationID.x
        let p = gl_GlobalInvocationID.y
        if (i >= qd) {
            return
        }
        let sb = p * 2u * qd + (i / hs) * 2u * hs + i % hs
        q[p * qs + i] = qg[sb]
        gt[p * qd + i] = qg[sb + hs]
    }
}

// Per-head sigmoid output gate (sigmoid_gate): x ⊙ σ(g) in place — the gated-attention
// epilogue between the attention output and the out projection.
class MetalSigmul {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "read" g : array<float>
    @uniform @binding = 2 total : uint      // grid guard

    [metal_kernel(name="metal_sigmul_msl")]
    def metal_sigmul {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] = x[gid] / (1.0 + exp(-g[gid]))
    }
}

// Shared-expert combine (qwen35moe): y += x ⊙ σ(g[row]) — one raw gate dot per position
// (decode total = dim; prefill total = npos x dim, row = the position).
[metal_dispatch(name = "enc_axpy_sig", pso = "g_pso_axpy_sig", tg = 256, grid = "total/256", params = "total : int64")]
class MetalAxpySig {
    @ssbo @binding = 0 y : array<float>
    @ssbo @binding = 1 x : array<float>
    @ssbo @binding = 2 g : array<float>     // per-position raw gate dots (σ applied here)
    @uniform @binding = 3 total : uint      // grid guard
    @uniform @binding = 4 dim : uint

    [metal_kernel(name="metal_axpy_sig_msl")]
    def metal_axpy_sig {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        y[gid] += x[gid] / (1.0 + exp(-g[gid / dim]))
    }
}
