options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_kernels shared public

require dasllama/dasllama_lint public
require math
require daslib/math_bits   // uint_bits_to_float in the mx4 MoE GEMV (msl_emit as_type)
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require daslib/typemacro_boost  // [template_struct_instance] — the sq_attn codec templates below
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

// ===== single-query attention: the shared stages =====

// The sq_attn family is ONE algorithm in four KV codecs (f16/f32/q8_0/tq4) across four dispatch
// shapes (single-tg, split-K chunk, batched, batched split-K). The codec axis changes only how a
// K/V element is loaded; the shape axis is pure addressing — a row-table base, a chunk origin,
// whether attention sinks fold in, and where the result lands. So all sixteen kernels below are
// these five stages with different arguments. They lower to plain MSL functions (one copy per
// kernel, since each kernel emits its own source), and every float operation stays in the order
// the hand-written bodies had it.
//
// Conventions: `row0` is the first context row this dispatch owns (0 unless split-K), `n` the row
// count, and the `*base` arguments the row-table byte/halfword/element offsets (0 unless batched).
// `scores`/`red` are the caller's @workgroup slabs; `red` is written by both softmax halves, so
// the caller must barrier between them.

// f16/f32 codecs — one generic, instantiated per KV element type.
def private sq_fill_scores(kc : array<auto(KT)>; q : array<float>; var scores : auto(SS);   // nolint:STYLE037 — MSL stage body; the epl register-hoist ladder is the point
                           kbase, row0, n, kv_dim, khoff, qoff, hs : uint; scale, softcap : float) {
    // simdgroup sg dots q[head] against K row j so every K load coalesces; the lane's q elements
    // are loop-invariant and hoist to registers (device loads can't — buffers may alias)
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
    while (j < n) {
        let kb = kbase + (row0 + j) * kv_dim + khoff + sl * epl
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
            scores[j] = sq_score_entry(s, scale, softcap)
        }
        j += gl_NumSubgroups
    }
}

// q8_0 codec — 34B blocks read through the dual view (half scale + byte quants); the lane's epl
// elements sit in ONE block, so the scale is loaded once per row and rides the simd sum.
def private sq_fill_scores_q8(ksh : array<float16>; kqb : array<int8>; q : array<float>; var scores : auto(SS);   // nolint:STYLE037 — MSL stage body; the epl register-hoist ladder is the point
                              kb0h, kb0b, row0, n, nblk, khblk, qoff, hs : uint; scale, softcap : float) {
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
    while (j < n) {
        let blk = (row0 + j) * nblk + khblk + hb
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
            scores[j] = sq_score_entry(s, scale, softcap)
        }
        j += gl_NumSubgroups
    }
}

// tq4 codec — 18B rotated 4-bit blocks; q arrives already rotated, so this runs in the rotated
// basis and yo needs one fwht-unsign pass downstream. epl == 1 reads a single nibble by parity.
def private sq_fill_scores_tq4(ksh : array<float16>; kqb : array<uint8>; q : array<float>; var scores : auto(SS);   // nolint:STYLE037 — MSL stage body; the epl register-hoist ladder is the point
                               kb0h, kb0b, row0, n, nblk, khblk, qoff, hs : uint; scale, softcap : float) {
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
    while (j < n) {
        let blk = (row0 + j) * nblk + khblk + hb
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
            scores[j] = sq_score_entry(s, scale, softcap)
        }
        j += gl_NumSubgroups
    }
}

// tanh via the exp identity (the CPU softcap4's form) — Metal tanh NaNs past ~44, while exp
// saturates: exp(INF) -> denom INF -> the term is 0.
def private sq_score_entry(s, scale, softcap : float) : float {
    let sc = s * scale
    return softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sc * 2.0 / softcap))) : sc
}

// Sliding window (gemma2): rows below the span start are masked out of this chunk. The guard is
// threadgroup-uniform, so the barrier inside it is safe.
def private sq_mask_window(var scores : auto(SS); c0, n, wstart, lid, nt : uint) {
    if (wstart > c0) {
        var jm = lid
        while (jm < n) {
            if (c0 + jm < wstart) {
                scores[jm] = -3.0e38
            }
            jm += nt
        }
        barrier()
    }
}

// Softmax half 1: row max — strided local, simd butterfly, cross-simdgroup partials through red[].
def private sq_score_max(scores : auto(SS); var red : float[16]; n, lid, nt : uint) : float {
    var m = -3.0e38
    var j = lid
    while (j < n) {
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
    return mm
}

// Softmax half 2: exp in place + the weight sum. Reuses red[], so callers barrier between halves.
def private sq_exp_sum(var scores : auto(SS); var red : float[16]; mm : float; n, lid, nt : uint) : float {
    var l = 0.0
    var j = lid
    while (j < n) {
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
    var sgi = 1u
    while (sgi < gl_NumSubgroups) {
        tot += red[sgi]
        sgi++
    }
    return tot
}

// Lane `lid` owns one V column; 4 rows in flight so device-load latency overlaps. Returns the
// unnormalized column sum — the caller divides (single/batched) or banks it as a partial (split-K).
def private sq_vacc(vc : array<auto(VT)>; scores : auto(SS); vbase, row0, n, kv_dim, khoff, lid : uint) : float {
    var acc0 = 0.0
    var acc1 = 0.0
    var acc2 = 0.0
    var acc3 = 0.0
    var j = 0u
    while (j + 4u <= n) {
        acc0 += scores[j] * float(vc[vbase + (row0 + j) * kv_dim + khoff + lid])
        acc1 += scores[j + 1u] * float(vc[vbase + (row0 + j + 1u) * kv_dim + khoff + lid])
        acc2 += scores[j + 2u] * float(vc[vbase + (row0 + j + 2u) * kv_dim + khoff + lid])
        acc3 += scores[j + 3u] * float(vc[vbase + (row0 + j + 3u) * kv_dim + khoff + lid])
        j += 4u
    }
    while (j < n) {
        acc0 += scores[j] * float(vc[vbase + (row0 + j) * kv_dim + khoff + lid])
        j++
    }
    return acc0 + acc1 + acc2 + acc3
}

// q8_0 twin — dequant rides the weighted sum.
def private sq_vacc_q8(vsh : array<float16>; vqb : array<int8>; scores : auto(SS);
                       vb0h, vb0b, row0, n, nblk, khblk, lid : uint) : float {
    let vblk0 = khblk + lid / 32u
    let vin = lid % 32u
    var acc0 = 0.0
    var acc1 = 0.0
    var acc2 = 0.0
    var acc3 = 0.0
    var j = 0u
    while (j + 4u <= n) {
        let b0 = (row0 + j) * nblk + vblk0
        let b1 = (row0 + j + 1u) * nblk + vblk0
        let b2 = (row0 + j + 2u) * nblk + vblk0
        let b3 = (row0 + j + 3u) * nblk + vblk0
        acc0 += scores[j] * float(vqb[vb0b + b0 * 34u + 2u + vin]) * float(vsh[vb0h + b0 * 17u])
        acc1 += scores[j + 1u] * float(vqb[vb0b + b1 * 34u + 2u + vin]) * float(vsh[vb0h + b1 * 17u])
        acc2 += scores[j + 2u] * float(vqb[vb0b + b2 * 34u + 2u + vin]) * float(vsh[vb0h + b2 * 17u])
        acc3 += scores[j + 3u] * float(vqb[vb0b + b3 * 34u + 2u + vin]) * float(vsh[vb0h + b3 * 17u])
        j += 4u
    }
    while (j < n) {
        let b0 = (row0 + j) * nblk + vblk0
        acc0 += scores[j] * float(vqb[vb0b + b0 * 34u + 2u + vin]) * float(vsh[vb0h + b0 * 17u])
        j++
    }
    return acc0 + acc1 + acc2 + acc3
}

// tq4 twin — nibble parity picks the half-byte; one row in flight (the nibble unpack already
// serializes the dependent loads, so the 4-way form measured no better).
def private sq_vacc_tq4(vsh : array<float16>; vqb : array<uint8>; scores : auto(SS);
                        vb0h, vb0b, row0, n, nblk, khblk, lid : uint) : float {
    let vblk0 = khblk + lid / 32u
    let vbyte = (lid % 32u) / 2u
    let hi = (lid & 1u) == 1u
    var acc0 = 0.0
    var j = 0u
    while (j < n) {
        let blk = (row0 + j) * nblk + vblk0
        let by = uint(vqb[vb0b + blk * 18u + 2u + vbyte])
        let nib = hi ? (by >> 4u) : (by & 15u)
        acc0 += scores[j] * float(int(nib) - 8) * float(vsh[vb0h + blk * 9u])
        j++
    }
    return acc0
}

// ===== the new kernels =====

// The single-query attention family binds its uniforms as ONE kargs value. `kstride` is the
// per-codec K-row stride: elements for f16/f32, 32-element blocks for q8_0/tq4.
struct SqAttnArgs {
    cnt : uint
    kstride : uint
    nheads : uint
    kv_mul : uint
    hs : uint
    scale : float
    softcap : float
    hass : uint
}

struct SqAttnPartArgs {
    cnt : uint
    kstride : uint
    nheads : uint
    kv_mul : uint
    hs : uint
    scale : float
    nchunks : uint
    softcap : float
    window : uint
}

// The comb pair binds ONE kargs shape at slot 2 (both stamps of MetalSqAttnCombT): `nch` is the
// chunk stride (and the single's loop bound); the single reads `chlo`, the BATCHED stamp derives
// per-row bounds from rt + `window`. Each form ignores the other's field.
struct SqAttnCombArgs {
    nch : uint
    nheads : uint
    hs : uint
    chlo : uint
    window : uint
    hass : uint
}

// The BATCHED attention family binds its uniforms as ONE kargs value. `kstride` is the
// per-codec K-row stride: elements for f16/f32, 32-element blocks for q8_0/tq4.
struct SqAttnBArgs {
    lay : uint
    kstride : uint
    nheads : uint
    kv_mul : uint
    hs : uint
    scale : float
    qstride : uint
    softcap : float
    hass : uint
}

struct SqAttnPartBArgs {
    lay : uint
    kstride : uint
    nheads : uint
    kv_mul : uint
    hs : uint
    scale : float
    qstride : uint
    nchmax : uint
    softcap : float
    window : uint
}

// The split-K "D" attention family binds its uniforms as ONE kargs value. `kstride` is the
// per-codec K-row stride: elements for f16/f32, 32-element blocks for q8_0/tq4.
struct SqAttnDArgs {
    lay : uint
    kstride : uint
    nheads : uint
    kv_mul : uint
    hs : uint
    scale : float
    qstride : uint
    nsgs : uint
}

struct SqAttnCombDArgs {
    nchmax : uint
    nheads : uint
    hs : uint
}

// One threadgroup per (stream, head), head_size threads — lane owns one V column. Pass 1 scores
// the context simdgroup-cooperatively so every K load coalesces (per-lane-per-row form gathered
// cache lines 2KB apart — measured 27us/layer at depth 24). Pass 2 softmax, pass 3 V-accumulate.

// One template for both float KV mirror codecs — KT is the K/V element type; each stamped instance emits its own MSL (globals MetalSqAttn*_metal_sq_attn_msl*)
[ |> template_struct_instance]
class template MetalSqAttnKvT {
    @ssbo @binding = 0 @off = "koff" kc : array<KT>  // this layer's K mirror slab (bound at the layer's byte offset)
    @ssbo @binding = 1 @off = "voff" vc : array<KT>
    @ssbo @binding = 2 q : array<float>     // [B x qd] roped queries
    @ssbo @binding = 3 yo : array<float>    // [B x qd] attention out
    @uniform @binding = 4 ka : SqAttnArgs
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @workgroup scores : float[128]  // >= ATTN_CHUNK_ROWS — the single-tg depth ceiling
    @workgroup red : float[16]

    [metal_kernel]
    def metal_sq_attn {
        let cnt = ka.cnt
        let kv_dim = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let softcap = ka.softcap
        let hass = ka.hass
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khoff = (h / kv_mul) * hs
        let qoff = bb * (nheads * hs) + h * hs
        sq_fill_scores(kc, q, scores, 0u, 0u, cnt, kv_dim, khoff, qoff, hs, scale, softcap)
        barrier()
        var mm = sq_score_max(scores, red, cnt, lid, nt)
        if (hass != 0u) {
            mm = max(mm, sink[h])
        }
        barrier()                           // red[] is reused for the sum below
        var tot = sq_exp_sum(scores, red, mm, cnt, lid, nt)
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
        }
        yo[qoff + lid] = sq_vacc(vc, scores, 0u, 0u, cnt, kv_dim, khoff, lid) / tot
    }
}

[metal_dispatch(name = "enc_attn16_c", pso = "g_pso_attn16", tgmem = "MetalSqAttnF16_metal_sq_attn_msl_tgmem", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnF16 : MetalSqAttnKvT {
    typedef KT = float16
}

// f32-mirror twin (sessions created with f32 KV) — same math, f32 loads.
[metal_dispatch(name = "enc_attn32_c", pso = "g_pso_attn32", tgmem = "MetalSqAttnF32_metal_sq_attn_msl_tgmem", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnF32 : MetalSqAttnKvT {
    typedef KT = float
}

// Quantized-mirror twin pair — K/V rows are quant blocks; the mirror slab binds twice (half view
// for scale, byte view for quants — the dual-view pattern) and dequant rides the stream. QB is the
// byte-view element type; the fill/vacc stage calls are @template_call slots the instances route.
[ |> template_struct_instance]
class template MetalSqAttnQuantT {
    @ssbo @binding = 0 @off = "koff" ksh : array<float16> // K mirror slab, HALF-SCALE view (bound at the layer's byte offset)
    @ssbo @binding = 1 @role = "alias" @off = "koff" kqb : array<QB>    // the SAME K slab, BYTE view
    @ssbo @binding = 2 @off = "voff" vsh : array<float16>
    @ssbo @binding = 3 @role = "alias" @off = "voff" vqb : array<QB>
    @ssbo @binding = 4 q : array<float>     // [B x qd] roped queries
    @ssbo @binding = 5 yo : array<float>    // [B x qd] attention out
    @uniform @binding = 6 ka : SqAttnArgs
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @workgroup scores : float[128]  // >= ATTN_CHUNK_ROWS — the single-tg depth ceiling
    @workgroup red : float[16]
    @template_call sq_fill_scores_q = @@sq_fill_scores_q8
    @template_call sq_vacc_q = @@sq_vacc_q8

    [metal_kernel]
    def metal_sq_attn {
        let cnt = ka.cnt
        let nblk = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let softcap = ka.softcap
        let hass = ka.hass
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let khblk = (h / kv_mul) * (hs / 32u)   // kv head offset in BLOCKS
        let qoff = bb * (nheads * hs) + h * hs
        sq_fill_scores_q(ksh, kqb, q, scores, 0u, 0u, 0u, cnt, nblk, khblk, qoff, hs, scale, softcap)
        barrier()
        var mm = sq_score_max(scores, red, cnt, lid, nt)
        if (hass != 0u) {
            mm = max(mm, sink[h])
        }
        barrier()                           // red[] is reused for the sum below
        var tot = sq_exp_sum(scores, red, mm, cnt, lid, nt)
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
        }
        yo[qoff + lid] = sq_vacc_q(vsh, vqb, scores, 0u, 0u, 0u, cnt, nblk, khblk, lid) / tot
    }
}

// q8_0 instance — 34B/32-elem q8_0 blocks. Measured 1.1-1.7x over the f16 form at d512-2048
// (benchmarks/attn/bench_metal_sq_attn.das).
[metal_dispatch(name = "enc_attn_q8_c", pso = "g_pso_attn_q8", tgmem = "MetalSqAttnQ8_metal_sq_attn_msl_tgmem", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnQ8 : MetalSqAttnQuantT {
    typedef QB = int8
}

// tq4 instance — 18B rotated 4-bit blocks; q arrives already rotated (the rope-store twin
// rotates it), so attention runs in the rotated basis and yo needs one fwht-unsign pass downstream.
[metal_dispatch(name = "enc_attn_tq4_c", pso = "g_pso_attn_tq4", tgmem = "MetalSqAttnTq4_metal_sq_attn_msl_tgmem", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnTq4 : MetalSqAttnQuantT {
    typedef QB = uint8
    override sq_fill_scores_q = @@sq_fill_scores_tq4
    override sq_vacc_q = @@sq_vacc_tq4
}

// Chunked split-K twin for deeper contexts (cnt > ATTN_CHUNK_ROWS): each threadgroup owns one
// 128-row chunk, writes an unnormalized partial (acc[hs] + chunk max + expsum); a comb kernel
// rescales across chunks. The single-tg form is latency-bound (measured 7.5ms/token at depth 512).

// One template for both float KV mirror codecs — KT is the K/V element type
[ |> template_struct_instance]
class template MetalSqAttnPartKvT {
    @ssbo @binding = 0 @off = "koff" kc : array<KT>
    @ssbo @binding = 1 @off = "voff" vc : array<KT>
    @ssbo @binding = 2 q : array<float>
    @ssbo @binding = 3 part : array<float>  // [B x nheads x nchunks x (hs+2)]: acc[hs], chunk max, chunk expsum
    @uniform @binding = 4 ka : SqAttnPartArgs
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]

    [metal_kernel]
    def metal_sq_attn_part {
        let cnt = ka.cnt
        let kv_dim = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let nchunks = ka.nchunks
        let softcap = ka.softcap
        let window = ka.window
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
        sq_fill_scores(kc, q, scores, 0u, c0, nrows, kv_dim, khoff, qoff, hs, scale, softcap)
        barrier()
        sq_mask_window(scores, c0, nrows, wstart, lid, nt)
        let mm = sq_score_max(scores, red, nrows, lid, nt)
        barrier()                           // red[] is reused for the sum below
        let tot = sq_exp_sum(scores, red, mm, nrows, lid, nt)
        let pbase = ((bb * nheads + h) * nchunks + ci) * (hs + 2u)
        part[pbase + lid] = sq_vacc(vc, scores, 0u, c0, nrows, kv_dim, khoff, lid)
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

[metal_dispatch(name = "enc_attnp16_c", pso = "g_pso_attnp16", tgmem = "MetalSqAttnPartF16_metal_sq_attn_part_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalSqAttnPartF16 : MetalSqAttnPartKvT {
    typedef KT = float16
}

// f32-mirror twin — same math, f32 K/V loads.
[metal_dispatch(name = "enc_attnp32_c", pso = "g_pso_attnp32", tgmem = "MetalSqAttnPartF32_metal_sq_attn_part_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalSqAttnPartF32 : MetalSqAttnPartKvT {
    typedef KT = float
}

// Quantized chunked twin pair — dual-view block reads (see MetalSqAttnQuantT); partials stay
// f32, so the comb below serves both forms unchanged (tq4 adds the fwht-unsign after it).
[ |> template_struct_instance]
class template MetalSqAttnPartQuantT {
    @ssbo @binding = 0 @off = "koff" ksh : array<float16>
    @ssbo @binding = 1 @role = "alias" @off = "koff" kqb : array<QB>
    @ssbo @binding = 2 @off = "voff" vsh : array<float16>
    @ssbo @binding = 3 @role = "alias" @off = "voff" vqb : array<QB>
    @ssbo @binding = 4 q : array<float>
    @ssbo @binding = 5 part : array<float>  // [B x nheads x nchunks x (hs+2)]: acc[hs], chunk max, chunk expsum
    @uniform @binding = 6 ka : SqAttnPartArgs
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]
    @template_call sq_fill_scores_q = @@sq_fill_scores_q8
    @template_call sq_vacc_q = @@sq_vacc_q8

    [metal_kernel]
    def metal_sq_attn_part {
        let cnt = ka.cnt
        let nblk = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let nchunks = ka.nchunks
        let softcap = ka.softcap
        let window = ka.window
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
        sq_fill_scores_q(ksh, kqb, q, scores, 0u, 0u, c0, nrows, nblk, khblk, qoff, hs, scale, softcap)
        barrier()
        sq_mask_window(scores, c0, nrows, wstart, lid, nt)
        let mm = sq_score_max(scores, red, nrows, lid, nt)
        barrier()                           // red[] is reused for the sum below
        let tot = sq_exp_sum(scores, red, mm, nrows, lid, nt)
        let pbase = ((bb * nheads + h) * nchunks + ci) * (hs + 2u)
        part[pbase + lid] = sq_vacc_q(vsh, vqb, scores, 0u, 0u, c0, nrows, nblk, khblk, lid)
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// q8_0 instance — 34B/32-elem q8_0 blocks.
[metal_dispatch(name = "enc_attnp_q8_c", pso = "g_pso_attnp_q8", tgmem = "MetalSqAttnPartQ8_metal_sq_attn_part_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalSqAttnPartQ8 : MetalSqAttnPartQuantT {
    typedef QB = int8
}

// tq4 instance — 18B rotated 4-bit blocks (rotated basis; fwht-unsign runs downstream).
[metal_dispatch(name = "enc_attnp_tq4_c", pso = "g_pso_attnp_tq4", tgmem = "MetalSqAttnPartTq4_metal_sq_attn_part_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalSqAttnPartTq4 : MetalSqAttnPartQuantT {
    typedef QB = uint8
    override sq_fill_scores_q = @@sq_fill_scores_tq4
    override sq_vacc_q = @@sq_vacc_tq4
}

// Cross-chunk combine: one threadgroup per (stream, head), hs threads — rescale each chunk's
// unnormalized partial to the global max, sum, normalize. Dtype-agnostic (partials are f32).
// BATCHED gates the per-row chunk-count table rt@3: each row combines its OWN bounds.
[ |> template_struct_instance]
class template MetalSqAttnCombT {
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 yo : array<float>
    @uniform @binding = 2 ka : SqAttnCombArgs
    @ssbo @binding = 3 @role = "read" @off = "rtoff" @template_gate = BATCHED rt : array<uint4>
    @ssbo @binding = 20 @role = "weight" @default = g_one sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @template_constant BATCHED : bool = false

    [metal_kernel]
    def metal_sq_attn_comb {
        let nchmax = ka.nch
        let nheads = ka.nheads
        let hs = ka.hs
        let hass = ka.hass
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        var nch = nchmax     // nolint:LINT003,LINT010 — the single stamp reads these inits, the
        var chlo = ka.chlo   // nolint:LINT003,LINT010 — BATCHED stamp overwrites them in its arm
        static_if (BATCHED) {
            // holes past a shallow row's last chunk are never read; sliding layers skip the
            // row's below-window chunks
            let cnt = rt[bb].w
            nch = (cnt + 63u) / 64u
            let wstart = ka.window > 0u && cnt > ka.window ? cnt - ka.window : 0u
            chlo = wstart / 64u
        }
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

[metal_dispatch(name = "enc_attncomb_c", pso = "g_pso_attncomb", tg = "head_size", grid = "nheads", params = "nheads : int64, head_size : int64")]
class MetalSqAttnComb : MetalSqAttnCombT {
}

// The fused QKV GEMV + rope + KV-row store, one THREADGROUP per rope pair — rows adjacent
// (2p, 2p+1) under NORM, (r, r+hs/2) in-head under NEOX — reduction split across the 4 sgs
// (the MetalQ8Gemv discipline); KT/CLAMP16 = the mirror codec axis (the MetalRopeStoreKvT shape).
[ |> template_struct_instance]
class template MetalQ8GemvQkvRsT {
    @ssbo @binding = 0 @role = "weight" @off = "wqoff" wqbq : array<int8>   // wq blob, byte view @ the q tensor's byte offset
    @ssbo @binding = 1 @role = "weight" @off = "wqoff" wshq : array<float16>   // ... half-scale view (same offset). Segments bind
                                            // separately — kind-major layout keeps q/k/v apart
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 qo : array<float>    // q out, roped ([0, qd) of the QKV buffer)
    @ssbo @binding = 4 @off = "koff" kd : array<KT>  // mirror K row (bound at the row offset)
    @ssbo @binding = 5 @off = "voff" vd : array<KT>
    @ssbo @binding = 6 @role = "weight" tcos : array<float>  // [hs/2] this position's rope row
    @ssbo @binding = 7 @role = "weight" tsin : array<float>
    @ssbo @binding = 8 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @ssbo @binding = 9 @role = "weight" @off = "wkoff" wqbk : array<int8>  // wk blob views
    @ssbo @binding = 10 @role = "weight" @off = "wkoff" wshk : array<float16>
    @ssbo @binding = 11 @role = "weight" @off = "wvoff" wqbv : array<int8>  // wv blob views
    @ssbo @binding = 12 @role = "weight" @off = "wvoff" wshv : array<float16>
    @uniform @binding = 13 ka : Q8QkvRsArgs
    @workgroup red : float[64]              // [2 pair rows][32 lanes] cross-sg partials
    @template_constant CLAMP16 : bool = true

    [metal_kernel]
    def metal_q8_gemv_qkv_rs {   // nolint:STYLE038 — MSL kernel body; the QKV phases share tgmem + simdgroup state
        let ndim = ka.ndim
        let qd = ka.qd
        let kvd = ka.kvd
        let hs = ka.hs
        let neox = ka.neox
        let has_bias = ka.has_bias
        let pr = gl_WorkGroupID.x
        // the threadgroup's two rows ARE the rope pair: adjacent under NORM; (r, r+half) within
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
        let sg = gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let ix = lane / 4u                  // block-within-round: 8 blocks per sg per round
        let il = lane % 4u                  // 8-quant chunk within the block
        var acc0 = 0.0
        var acc1 = 0.0
        var ib = sg * 8u + ix
        // both pair rows sit in ONE segment (qd/kvd even, NEOX pairs stay in-head) — dot them
        // off that segment's blob views with segment-LOCAL rows
        if (row < qd) {
            while (ib < nb) {
                let xi = ib * 8u + il * 2u
                let x0 = x[xi]
                let x1 = x[xi + 1u]
                let qb0 = (row * nb + ib) * 34u + 2u + il * 8u
                let qb1 = (row2 * nb + ib) * 34u + 2u + il * 8u
                acc0 += (float(wqbq[qb0]) * x0.x + float(wqbq[qb0 + 1u]) * x0.y +
                    float(wqbq[qb0 + 2u]) * x0.z + float(wqbq[qb0 + 3u]) * x0.w +
                    float(wqbq[qb0 + 4u]) * x1.x + float(wqbq[qb0 + 5u]) * x1.y +
                    float(wqbq[qb0 + 6u]) * x1.z + float(wqbq[qb0 + 7u]) * x1.w) * float(wshq[(row * nb + ib) * 17u])
                acc1 += (float(wqbq[qb1]) * x0.x + float(wqbq[qb1 + 1u]) * x0.y +
                    float(wqbq[qb1 + 2u]) * x0.z + float(wqbq[qb1 + 3u]) * x0.w +
                    float(wqbq[qb1 + 4u]) * x1.x + float(wqbq[qb1 + 5u]) * x1.y +
                    float(wqbq[qb1 + 6u]) * x1.z + float(wqbq[qb1 + 7u]) * x1.w) * float(wshq[(row2 * nb + ib) * 17u])
                ib += 32u
            }
        } elif (row < qd + kvd) {
            let lr = row - qd
            let lr2 = row2 - qd
            while (ib < nb) {
                let xi = ib * 8u + il * 2u
                let x0 = x[xi]
                let x1 = x[xi + 1u]
                let qb0 = (lr * nb + ib) * 34u + 2u + il * 8u
                let qb1 = (lr2 * nb + ib) * 34u + 2u + il * 8u
                acc0 += (float(wqbk[qb0]) * x0.x + float(wqbk[qb0 + 1u]) * x0.y +
                    float(wqbk[qb0 + 2u]) * x0.z + float(wqbk[qb0 + 3u]) * x0.w +
                    float(wqbk[qb0 + 4u]) * x1.x + float(wqbk[qb0 + 5u]) * x1.y +
                    float(wqbk[qb0 + 6u]) * x1.z + float(wqbk[qb0 + 7u]) * x1.w) * float(wshk[(lr * nb + ib) * 17u])
                acc1 += (float(wqbk[qb1]) * x0.x + float(wqbk[qb1 + 1u]) * x0.y +
                    float(wqbk[qb1 + 2u]) * x0.z + float(wqbk[qb1 + 3u]) * x0.w +
                    float(wqbk[qb1 + 4u]) * x1.x + float(wqbk[qb1 + 5u]) * x1.y +
                    float(wqbk[qb1 + 6u]) * x1.z + float(wqbk[qb1 + 7u]) * x1.w) * float(wshk[(lr2 * nb + ib) * 17u])
                ib += 32u
            }
        } else {
            let lr = row - qd - kvd
            let lr2 = row2 - qd - kvd
            while (ib < nb) {
                let xi = ib * 8u + il * 2u
                let x0 = x[xi]
                let x1 = x[xi + 1u]
                let qb0 = (lr * nb + ib) * 34u + 2u + il * 8u
                let qb1 = (lr2 * nb + ib) * 34u + 2u + il * 8u
                acc0 += (float(wqbv[qb0]) * x0.x + float(wqbv[qb0 + 1u]) * x0.y +
                    float(wqbv[qb0 + 2u]) * x0.z + float(wqbv[qb0 + 3u]) * x0.w +
                    float(wqbv[qb0 + 4u]) * x1.x + float(wqbv[qb0 + 5u]) * x1.y +
                    float(wqbv[qb0 + 6u]) * x1.z + float(wqbv[qb0 + 7u]) * x1.w) * float(wshv[(lr * nb + ib) * 17u])
                acc1 += (float(wqbv[qb1]) * x0.x + float(wqbv[qb1 + 1u]) * x0.y +
                    float(wqbv[qb1 + 2u]) * x0.z + float(wqbv[qb1 + 3u]) * x0.w +
                    float(wqbv[qb1 + 4u]) * x1.x + float(wqbv[qb1 + 5u]) * x1.y +
                    float(wqbv[qb1 + 6u]) * x1.z + float(wqbv[qb1 + 7u]) * x1.w) * float(wshv[(lr2 * nb + ib) * 17u])
                ib += 32u
            }
        }
        if (sg == 0u) {
            red[lane] = 0.0
            red[32u + lane] = 0.0
        }
        acc0 = simd_sum(acc0)
        acc1 = simd_sum(acc1)
        barrier()
        if (lane == 0u) {
            red[sg] = acc0
            red[32u + sg] = acc1
        }
        barrier()
        var tot0 = simd_sum(red[lane])
        var tot1 = simd_sum(red[32u + lane])
        if (sg != 0u || lane != 0u) {
            return
        }
        if (has_bias != 0u) {
            tot0 += bias[row]
            tot1 += bias[row2]
        }
        if (row < qd) {
            let j = neox != 0u ? row % hs : (row % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            qo[row] = tot0 * fcr - tot1 * fci
            qo[row2] = tot0 * fci + tot1 * fcr
        } elif (row < qd + kvd) {
            let ik = row - qd
            let ik2 = row2 - qd
            let j = neox != 0u ? ik % hs : (ik % hs) / 2u
            let fcr = tcos[j]
            let fci = tsin[j]
            static_if (CLAMP16) {
                kd[ik] = float16(min(max(tot0 * fcr - tot1 * fci, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
                kd[ik2] = float16(min(max(tot0 * fci + tot1 * fcr, -65504.0), 65504.0))   // nolint:STYLE019
            } else {
                kd[ik] = tot0 * fcr - tot1 * fci
                kd[ik2] = tot0 * fci + tot1 * fcr
            }
        } else {
            let iv = row - qd - kvd
            static_if (CLAMP16) {
                vd[iv] = float16(min(max(tot0, -65504.0), 65504.0))   // nolint:STYLE019
                vd[iv + 1u] = float16(min(max(tot1, -65504.0), 65504.0))   // nolint:STYLE019
            } else {
                vd[iv] = tot0
                vd[iv + 1u] = tot1
            }
        }
    }
}

[metal_dispatch(name = "enc_qkvrs16_c", pso = "g_pso_qkvrs16", tgmem = "MetalQ8GemvQkvRsF16_metal_q8_gemv_qkv_rs_msl_tgmem", tg = 128, grid = "pairs", params = "pairs : int64")]
class MetalQ8GemvQkvRsF16 : MetalQ8GemvQkvRsT {
    typedef KT = float16
}

// f32-mirror twin — straight stores, no clamp.
[metal_dispatch(name = "enc_qkvrs32_c", pso = "g_pso_qkvrs32", tgmem = "MetalQ8GemvQkvRsF32_metal_q8_gemv_qkv_rs_msl_tgmem", tg = 128, grid = "pairs", params = "pairs : int64")]
class MetalQ8GemvQkvRsF32 : MetalQ8GemvQkvRsT {
    typedef KT = float
    override CLAMP16 = false
}


// The W1|W3 twin: one THREADGROUP dots BOTH ffn rows of an output element (gate off the w1 blob
// views, up off the w3 views), reduction split across its 4 simdgroups, and writes act(g)·u
// directly (act = silu or gelu). Deletes the swiglu/geglu dispatch AND its serialization level.
[metal_dispatch(name = "enc_w13sw_c", pso = "g_pso_w13sw", tgmem = "metal_q8_gemv_w13sw16_msl_tgmem", tg = 128, grid = "hidden", params = "hidden : int64")]
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
    @workgroup red : float[64]              // [gate|up][32 lanes] cross-sg partials

    [metal_kernel(name="metal_q8_gemv_w13sw16_msl")]
    def metal_q8_gemv_w13sw16 {
        let r = gl_WorkGroupID.x
        let sg = gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let ix = lane / 4u                  // block-within-round: 8 blocks per sg per round
        let il = lane % 4u                  // 8-quant chunk within the block
        var acc0 = 0.0
        var acc1 = 0.0
        var ib = sg * 8u + ix
        while (ib < nb) {
            let xi = ib * 8u + il * 2u
            let x0 = x[xi]
            let x1 = x[xi + 1u]
            let qb = (r * nb + ib) * 34u + 2u + il * 8u
            acc0 += (float(wqb1[qb]) * x0.x + float(wqb1[qb + 1u]) * x0.y +
                float(wqb1[qb + 2u]) * x0.z + float(wqb1[qb + 3u]) * x0.w +
                float(wqb1[qb + 4u]) * x1.x + float(wqb1[qb + 5u]) * x1.y +
                float(wqb1[qb + 6u]) * x1.z + float(wqb1[qb + 7u]) * x1.w) * float(wsh1[(r * nb + ib) * 17u])
            acc1 += (float(wqb3[qb]) * x0.x + float(wqb3[qb + 1u]) * x0.y +
                float(wqb3[qb + 2u]) * x0.z + float(wqb3[qb + 3u]) * x0.w +
                float(wqb3[qb + 4u]) * x1.x + float(wqb3[qb + 5u]) * x1.y +
                float(wqb3[qb + 6u]) * x1.z + float(wqb3[qb + 7u]) * x1.w) * float(wsh3[(r * nb + ib) * 17u])
            ib += 32u
        }
        if (sg == 0u) {
            red[lane] = 0.0
            red[32u + lane] = 0.0
        }
        acc0 = simd_sum(acc0)
        acc1 = simd_sum(acc1)
        barrier()
        if (lane == 0u) {
            red[sg] = acc0
            red[32u + sg] = acc1
        }
        barrier()
        let tot0 = simd_sum(red[lane])
        let tot1 = simd_sum(red[32u + lane])
        if (sg == 0u && lane == 0u) {
            if (act == 0u) {
                ho[r] = (tot0 / (1.0 + exp(-tot0))) * tot1
            } else {
                // geglu: gelu(gate)·up — the CPU geglu4 / MetalGeglu tanh-via-exp identity (exp saturates, no NaN)
                let ga = 0.7978845608028654 * (tot0 + 0.044715 * tot0 * tot0 * tot0)
                ho[r] = 0.5 * tot0 * (2.0 - 2.0 / (1.0 + exp(2.0 * ga))) * tot1
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

// Fused residual-add + rmsnorm, one threadgroup per row: x += a in place, then y = rms(x)·w
// (the row stages through the tgmem slab — barrier() is a threadgroup fence only; dim caps at the
// slab, wider models take enc_add_rms_site). Single-row = the lensed grid=1; batch = enc_add_rms_b.
[ |> template_struct_instance]
class template MetalAddRmsT {
    @ssbo @binding = 0 x : array<float>     // [dim] residual stream, add target
    @ssbo @binding = 1 a : array<float>     // [dim] addend (fully read before y is written — y may alias a)
    @ssbo @binding = 2 @role = "weight" w : array<float>     // [dim] norm weight
    @ssbo @binding = 3 y : array<float>     // [dim] normed out
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 eps : float
    @uniform @binding = 6 @role = "weight" ascale : float    // (x + a) * ascale — gemma4 layer_out_scale scales
                                            // the whole layer output; every other site binds 1.0 (g_one)
    @ssbo @binding = 7 @role = "write" @template_gate = HX yh : array<float16>   // f16 twin of y (the dual-store stamp)
    @template_constant HX : bool = false
    @workgroup xrow : float[4096]           // == ADD_RMS_MAX_DIM
    @workgroup partial : float[32]          // one per simdgroup at the 1024-thread dispatch
    @workgroup inv_sh : float

    [metal_kernel]
    def metal_add_rms {
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
            let ov = w[k] * (xrow[k] * inv)
            y[row * dim + k] = ov
            static_if (HX) {
                yh[row * dim + k] = float16(ov)
            }
            k += gl_WorkGroupSize.x
        }
    }
}

[metal_dispatch(name = "enc_addrms_c", pso = "g_pso_addrms", tgmem = "MetalAddRms_metal_add_rms_msl_tgmem", tg = 1024, grid = "1"),
    metal_dispatch(name = "enc_add_rms_b_c", pso = "g_pso_addrms", tgmem = "MetalAddRms_metal_add_rms_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64")]
class MetalAddRms : MetalAddRmsT {
}

[metal_dispatch(name = "pf_enc_add_rms_bhx_c", pso = "g_pso_addrms_hx", tgmem = "MetalAddRmsHX_metal_add_rms_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64")]
class MetalAddRmsHX : MetalAddRmsT {
    override HX = true
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
// pairs rope straight into the mirror K row (f16 instances clamp to ±65504) and the same thread
// copies its V pair. Replaces the rope_qk + kv_store dispatch pair. KT is the mirror element type.
[ |> template_struct_instance]
class template MetalRopeStoreKvT {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) roped in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 @off = "koff" kd : array<KT>  // mirror K row (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 @off = "voff" vd : array<KT>
    @ssbo @binding = 4 @role = "weight" tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 5 @role = "weight" tsin : array<float>
    @ssbo @binding = 6 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 7 ka : RopeStoreArgs
    @template_constant CLAMP16 : bool = true

    [metal_kernel]
    def metal_rope_store {
        let qd = ka.qd
        let head_size = ka.head_size
        let npairs_q = ka.npairs_q
        let npairs = ka.npairs
        let neox = ka.neox
        let has_bias = ka.has_bias
        let rot = ka.rot
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
            static_if (CLAMP16) {
                let r0 = v0 * fcr - v1 * fci
                let r1 = v0 * fci + v1 * fcr
                kd[i] = float16(min(max(r0, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
                kd[i2] = float16(min(max(r1, -65504.0), 65504.0))   // nolint:STYLE019
                let b0 = has_bias != 0u ? bias[qd + kvd + i] : 0.0
                let b1 = has_bias != 0u ? bias[qd + kvd + i2] : 0.0
                vd[i] = float16(min(max(vs[i] + b0, -65504.0), 65504.0))   // nolint:STYLE019
                vd[i2] = float16(min(max(vs[i2] + b1, -65504.0), 65504.0))   // nolint:STYLE019
            } else {
                kd[i] = v0 * fcr - v1 * fci
                kd[i2] = v0 * fci + v1 * fcr
                vd[i] = vs[i] + (has_bias != 0u ? bias[qd + kvd + i] : 0.0)
                vd[i2] = vs[i2] + (has_bias != 0u ? bias[qd + kvd + i2] : 0.0)
            }
        } else {
            let v0 = qk[i] + (has_bias != 0u ? bias[i] : 0.0)
            let v1 = qk[i2] + (has_bias != 0u ? bias[i2] : 0.0)
            qk[i] = v0 * fcr - v1 * fci
            qk[i2] = v0 * fci + v1 * fcr
        }
    }
}

[metal_dispatch(name = "enc_rpst16_c", pso = "g_pso_rpst16", tg = 64, grid = "npairs/64", params = "npairs : int64")]
class MetalRopeStoreF16 : MetalRopeStoreKvT {
    typedef KT = float16
}

// f32-mirror twin — straight stores, no clamp.
[metal_dispatch(name = "enc_rpst32_c", pso = "g_pso_rpst32", tg = 64, grid = "npairs/64", params = "npairs : int64")]
class MetalRopeStoreF32 : MetalRopeStoreKvT {
    typedef KT = float
    override CLAMP16 = false
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
    @uniform @binding = 8 ka : RopeStoreHArgs
    @workgroup buf : float[512]     // <= max head_size (gemma4 global heads are 512)
    @workgroup part : float[16]

    [metal_kernel(name="metal_rope_store_h16_msl")]
    def metal_rope_store_h16 {   // nolint:STYLE037 — MSL kernel body; rope + KV store share the row's register state
        let qd = ka.qd
        let head_size = ka.head_size
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let neox = ka.neox
        let has_bias = ka.has_bias
        let has_norm = ka.has_norm
        let eps = ka.eps
        let rot = ka.rot
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

// Shared q8_0-mirror rope+store stages, spliced by the single and batched kernels below: the
// q pair rope, the 3-arm K/V vals fill (V copy / NEOX per-element / NORM pair), the amax scale,
// and the 34B-block store. Base offsets parameterize the single (0) vs batched (row) layouts.
class MetalRopeStoreQ8Base {
    def q8_rope_q(var dst : array<float>; tc, ts, bs : array<float>; gid, half, head_size, neox, has_bias, qkb, tb : uint) {
        let j = gid % half
        let hbase = (gid / half) * head_size
        let i = neox != 0u ? hbase + j : hbase + 2u * j
        let i2 = neox != 0u ? i + half : i + 1u
        let fcr = tc[tb + j]
        let fci = ts[tb + j]
        let v0 = dst[qkb + i] + (has_bias != 0u ? bs[i] : 0.0)
        let v1 = dst[qkb + i2] + (has_bias != 0u ? bs[i2] : 0.0)
        dst[qkb + i] = v0 * fcr - v1 * fci
        dst[qkb + i2] = v0 * fci + v1 * fcr
    }

    def q8_fill_kv_vals(src : array<float>; vsrc : array<float>; tc, ts, bs : array<float>; var vals : float[32];
                        kbase, vbase, tb, base, head_size, half, qd, kvd, neox, has_bias : uint; isV : bool) {
        if (isV) {
            var e = 0u
            while (e < 32u) {
                vals[e] = vsrc[vbase + base + e] + (has_bias != 0u ? bs[qd + kvd + base + e] : 0.0)
                e++
            }
        } elif (neox != 0u) {
            // NEOX pairs straddle 32-blocks (partner at ±half) — rope per ELEMENT; partner
            // reads come from the q/k region, which no K/V thread writes, so cross-block reads are safe
            var e = 0u
            while (e < 32u) {
                let ge = base + e
                let hp = ge % head_size
                let isHi = hp >= half
                let j = isHi ? hp - half : hp
                let fcr = tc[tb + j]
                let fci = ts[tb + j]
                let oi = isHi ? ge - half : ge + half
                let mine = src[kbase + ge] + (has_bias != 0u ? bs[qd + ge] : 0.0)
                let other = src[kbase + oi] + (has_bias != 0u ? bs[qd + oi] : 0.0)
                vals[e] = isHi ? other * fci + mine * fcr : mine * fcr - other * fci
                e++
            }
        } else {
            var p = 0u
            while (p < 16u) {
                let ge = base + 2u * p
                let j = (ge % head_size) / 2u
                let fcr = tc[tb + j]
                let fci = ts[tb + j]
                let v0 = src[kbase + ge] + (has_bias != 0u ? bs[qd + ge] : 0.0)
                let v1 = src[kbase + ge + 1u] + (has_bias != 0u ? bs[qd + ge + 1u] : 0.0)
                vals[2u * p] = v0 * fcr - v1 * fci
                vals[2u * p + 1u] = v0 * fci + v1 * fcr
                p++
            }
        }
    }

    def q8_scale(vals : float[32]; var d : float&; var id : float&) {
        var amax = 0.0
        var k = 0u
        while (k < 32u) {
            amax = max(amax, abs(vals[k]))
            k++
        }
        d = amax / 127.0
        id = d != 0.0 ? 1.0 / d : 0.0
    }

    def q8_store(var sh : array<float16>; var qb : array<int8>; vals : float[32]; d, id : float; shb, qbb, blk : uint) {
        sh[shb + blk * 17u] = float16(d)
        var k = 0u
        while (k < 32u) {
            qb[qbb + blk * 34u + 2u + k] = int8(round(vals[k] * id))
            k++
        }
    }
}

// q8_0-mirror rope + KV-row store: q pairs rope in place; K/V rows quantize per 32-block (one
// thread per block) matching quantize_q8kv_row's exact semantics, so CPU fallback steps read the
// same codec. K blocks rope their 16 pairs first; V blocks copy raw.
[metal_dispatch(name = "enc_rpst_q8_c", pso = "g_pso_rpst_q8", tg = 64, grid = "nthreads/64", params = "nthreads : int64")]
class MetalRopeStoreQ8 : MetalRopeStoreQ8Base {
    @ssbo @binding = 0 @role = "readwrite" qk : array<float>    // q at [0, qd) roped in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 @role = "write" @off = "koff" ksh : array<float16> // mirror K row, HALF-SCALE view (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 @role = "alias" @off = "koff" kqb : array<int8>    // the SAME K row, BYTE view
    @ssbo @binding = 4 @role = "write" @off = "voff" vsh : array<float16>
    @ssbo @binding = 5 @role = "alias" @off = "voff" vqb : array<int8>
    @ssbo @binding = 6 @role = "weight" tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 7 @role = "weight" tsin : array<float>
    @ssbo @binding = 8 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 9 ka : RopeStoreQ8Args

    [metal_kernel(name="metal_rope_store_q8_msl")]
    def metal_rope_store_q8 {
        let qd = ka.qd
        let head_size = ka.head_size
        let npairs_q = ka.npairs_q
        let nblk = ka.nblk
        let neox = ka.neox
        let has_bias = ka.has_bias
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs_q + 2u * nblk) {
            return
        }
        let half = head_size / 2u
        if (gid < npairs_q) {
            q8_rope_q(qk, tcos, tsin, bias, gid, half, head_size, neox, has_bias, 0u, 0u)
            return
        }
        let kvd = nblk * 32u
        let bi = gid - npairs_q
        let isV = bi >= nblk
        let blk = isV ? bi - nblk : bi
        let base = blk * 32u                // element base within the K/V row
        var vals : float[32]
        q8_fill_kv_vals(qk, vs, tcos, tsin, bias, vals, qd, 0u, 0u, base, head_size, half, qd, kvd, neox, has_bias, isV)
        var d = 0.0
        var id = 0.0
        q8_scale(vals, d, id)
        if (isV) {
            q8_store(vsh, vqb, vals, d, id, 0u, 0u, blk)
        } else {
            q8_store(ksh, kqb, vals, d, id, 0u, 0u, blk)
        }
    }
}

// Shared tq4-mirror rope+store stages, spliced by the single and batched kernels below: the
// buf-resident FWHT butterfly, the per-32-block signed-max quantize scalars, and the packed
// nibble store. Base byte offsets parameterize the single (0) vs batched (arena row) layouts.
class MetalRopeStoreTq4Base {
    @workgroup buf : float[512]     // <= max head_size (gemma4 global heads are 512)

    def tq4_fwht(var v : float&; sg : array<float>; lid, hs : uint) {
        // fwht_signs_row: signs, then the orthonormal butterfly
        buf[lid] = v * sg[lid]
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
    }

    def tq4_quant(v : float; var d : float&; var qv : int&; var qother : int&; sl : uint) {
        // per-32-block quantize — block == simdgroup; signed max keeps the FIRST max-abs element
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
        d = mv / -8.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        qv = min(15, int(v * id + 8.5))
        qother = simd_shuffle_xor(qv, 1u)
    }

    def tq4_store(var sh : array<float16>; var qb : array<uint8>; d : float; qv, qother : int; b0b, blk, lid, sl : uint) {
        if (sl == 0u) {
            sh[b0b / 2u + blk * 9u] = float16(d)
        }
        if ((lid & 1u) == 0u) {
            qb[b0b + blk * 18u + 2u + (lid % 32u) / 2u] = uint8(uint(qv) | (uint(qother) << 4u))
        }
    }
}

// tq4-mirror rope + KV-row store: one threadgroup per head (head_size threads — the FWHT spans
// the head). Q heads rope + rotate in place; K heads rope + rotate + quantize into the mirror
// row; V heads rotate + quantize, matching the CPU seam's fwht_signs_row / quantize_tq4kv_row bit-for-bit.
[metal_dispatch(name = "enc_rpst_tq4_c", pso = "g_pso_rpst_tq4", tgmem = "metal_rope_store_tq4_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64")]
class MetalRopeStoreTq4 : MetalRopeStoreTq4Base {
    @ssbo @binding = 0 qk : array<float>    // q at [0, qd) ropes + rotates in place, k at [qd, qd + kvd)
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 @off = "koff" ksh : array<float16> // mirror K row, HALF-SCALE view (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 @role = "alias" @off = "koff" kqb : array<uint8>   // the SAME row, BYTE view
    @ssbo @binding = 4 @off = "voff" vsh : array<float16>
    @ssbo @binding = 5 @role = "alias" @off = "voff" vqb : array<uint8>
    @ssbo @binding = 6 @role = "weight" tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 7 @role = "weight" tsin : array<float>
    @ssbo @binding = 8 @role = "weight" signs : array<float> // the tq4 rotation signs (prefix per head_size)
    @ssbo @binding = 9 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 10 ka : RopeStoreTq4Args

    [metal_kernel(name="metal_rope_store_tq4_msl")]
    def metal_rope_store_tq4 {
        let qd = ka.qd
        let head_size = ka.head_size
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let neox = ka.neox
        let has_bias = ka.has_bias
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
        tq4_fwht(v, signs, lid, hs)
        if (isQ) {
            qk[base + lid] = v
            return
        }
        let sl = gl_SubgroupInvocationID
        var d = 0.0
        var qv = 0
        var qother = 0
        tq4_quant(v, d, qv, qother, sl)
        let blk = hh * (hs / 32u) + lid / 32u   // block within the K/V row
        if (isK) {
            tq4_store(ksh, kqb, d, qv, qother, 0u, blk, lid, sl)
        } else {
            tq4_store(vsh, vqb, d, qv, qother, 0u, blk, lid, sl)
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

// The width pair as one template — the only structural split is the panel load (one slot per
// thread at B2, two at B4), which WIDE carries; both stamps byte-identical.
[ |> template_struct_instance]
class template MetalGemvB24T {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wqb : array<int8>    // 34B-block W blob, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" @off = "boff" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" x : array<float4>    // [B x n] f32 rows, float4 view
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>     // [B x d]
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y ROW stride (== ddim dense)
    @workgroup txp : float4[PANEL]          // [NR streams x 64 float4] X panel chunk
    @template_constant NR : int = 2
    @template_constant PANEL : int = 128    // NR * 64 slots — override together with NR and WIDE
    @template_constant WIDE : bool = false  // B4: two panel slots per thread

    [metal_kernel]
    def metal_gemv {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, ddim - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var acc : float[NR]
        var cb = 0u
        while (cb + 64u <= nb4) {
            barrier()                       // previous round done reading the panel
            static_if (WIDE) {
                for [unroll_full] (j in range(2)) {
                    let slot = uint(j) * 128u + lid
                    txp[slot] = x[(slot / 64u) * nb4 + cb + slot % 64u]
                }
            } else {
                txp[lid] = x[(lid / 64u) * nb4 + cb + lid % 64u]
            }
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let bi = row * nb + k / 8u
                let qb = bi * 34u + 2u + (k % 8u) * 4u
                let w4 = float4(float(wqb[qb]), float(wqb[qb + 1u]), float(wqb[qb + 2u]),
                    float(wqb[qb + 3u])) * float(wsh[bi * 17u])
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(NR)) {
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
            for [unroll_full] (b in range(NR)) {
                let xv = x[uint(b) * nb4 + k]
                acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(NR)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

[metal_dispatch(name = "enc_gemv_b2_c", pso = "g_pso_gemv_b2", tgmem = "MetalGemvB2_metal_gemv_msl_tgmem", tg = 128, grid = "d/4", params = "d : int64")]
class MetalGemvB2 : MetalGemvB24T {
    override NR = 2
    override PANEL = 128
    override WIDE = false
}

[metal_dispatch(name = "enc_gemv_b4_c", pso = "g_pso_gemv_b4", tgmem = "MetalGemvB4_metal_gemv_msl_tgmem", tg = 128, grid = "d/4", params = "d : int64")]
class MetalGemvB4 : MetalGemvB24T {
    override NR = 4
    override PANEL = 256
    override WIDE = true
}

// Skinny-batch GEMV: streamed form beats x-staged panel kernels ~12% at B <= 4. 64 threads = 2
// simdgroups x 2 row-halves; 16 lanes stream one output row's interleaved 34B q8 blocks, batch
// columns share the dequantized weight stream. Requires n % 256 == 0 (unguarded 4-chunk stripe).
[metal_dispatch(name = "enc_mv_b2_c", pso = "g_pso_mv_b2", tg = 64, grid = "d/4", params = "d : int64")]
class MetalQ8MvB2 {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "read" @off = "xoff" x : array<float4>    // [B rows x xs4 float4-stride] f32 rows
    @ssbo @binding = 3 @role = "write" @off = "yoff" y : array<float>     // [B x ddim]
    @uniform @binding = 4 ka : Q8MvArgs

    [metal_kernel(name="metal_q8_mv_b2_msl")]
    def metal_q8_mv_b2 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 16u
        let row0 = gl_WorkGroupID.x * 4u + gl_SubgroupID * 2u + lane / 16u
        let row = min(row0, ka.ddim - 1u)      // clamp, not return — keeps the shuffle groups full
        let nb = ka.ndim / 32u
        let blk = row * nb + tx / 8u
        let cq = 4u * (tx % 4u) + 16u * ((tx % 8u) / 4u)   // quant offset of chunk tx%8 in its block
        var sp = unsafe(addr(wsh[blk * 17u]))
        var qp = unsafe(addr(wqb[blk * 34u + 2u + cq]))
        var x0 = unsafe(addr(x[tx]))
        var x1 = unsafe(addr(x[ka.xs4 + tx]))
        var acc : float[2]
        var lw : float4[4]
        var ich = tx
        while (4u * ich < ka.ndim) {
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
            if (tx == 0u && row0 < ka.ddim && uint(b) < ka.nrows) {
                y[uint(b) * ka.ys + row0] = s
            }
        }
    }
}

// 4-column twin (serves B = 3..4; pad column reads clamp to row 0, stores gate on nrows). Drops
// to 8 lanes per output row (4 rows/simdgroup, 8 rows/tg): fewer lanes per weight load, more
// rows in flight hides the block-load latency instead.
[metal_dispatch(name = "enc_mv_b4_c", pso = "g_pso_mv_b4", tg = 64, grid = "d/8", params = "d : int64")]
class MetalQ8MvB4 {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>
    @ssbo @binding = 2 @role = "read" @off = "xoff" x : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 4 ka : Q8MvArgs

    [metal_kernel(name="metal_q8_mv_b4_msl")]
    def metal_q8_mv_b4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let row0 = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + lane / 8u
        let row = min(row0, ka.ddim - 1u)
        let nb = ka.ndim / 32u
        let blk = row * nb
        let cq = 4u * (tx % 4u) + 16u * (tx / 4u)   // quant offset of chunk tx in its block
        var sp = unsafe(addr(wsh[blk * 17u]))
        var qp = unsafe(addr(wqb[blk * 34u + 2u + cq]))
        var x0 = unsafe(addr(x[tx]))
        var x1 = unsafe(addr(x[ka.xs4 + tx]))
        var x2 = unsafe(addr(x[min(2u, ka.nrows - 1u) * ka.xs4 + tx]))
        var x3 = unsafe(addr(x[min(3u, ka.nrows - 1u) * ka.xs4 + tx]))
        var acc : float[4]
        var lw : float4[4]
        var ich = tx
        while (4u * ich < ka.ndim) {
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
            if (tx == 0u && row0 < ka.ddim && uint(b) < ka.nrows) {
                y[uint(b) * ka.ys + row0] = s
            }
        }
    }
}

// Fused fixed-B W1|W3 GEMV + swiglu (batch twin of MetalQ8GemvW13Sw): each subgroup dots the
// gate row r AND up row r+hidden, writing silu(gate)*up directly — replaces the w1/w3/swiglu
// triple with one dispatch. B2 form serves B <= 2; B4 serves 3..4.

// Same WIDE panel-load split as MetalGemvB24T above.
[ |> template_struct_instance]
class template MetalGemvW13SwB24T {
    @ssbo @binding = 0 @role = "weight" @off = "w1off" wqb1 : array<int8>   // w1 blob, byte view @ its tensor offset
    @ssbo @binding = 2 @role = "weight" @off = "w1off" wsh1 : array<float16>   // ... half-scale view
    @ssbo @binding = 3 @role = "read" x : array<float4>    // [B x ndim] f32 rows, float4 view
    @ssbo @binding = 4 @role = "write" y : array<float>     // [B x hidden] silu(gate)*up
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 hidden : uint     // output rows
    @ssbo @binding = 8 @role = "weight" @off = "w3off" wqb3 : array<int8>   // w3 blob views
    @ssbo @binding = 9 @role = "weight" @off = "w3off" wsh3 : array<float16>
    @workgroup txp : float4[PANEL]          // [NR streams x 64 float4] X panel chunk
    @template_constant NR : int = 2
    @template_constant PANEL : int = 128    // NR * 64 slots — override together with NR and WIDE
    @template_constant WIDE : bool = false  // B4: two panel slots per thread

    [metal_kernel]
    def metal_gemv_w13sw {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, hidden - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var accg : float[NR]
        var accu : float[NR]
        var cb = 0u
        while (cb + 64u <= nb4) {
            barrier()                       // previous round done reading the panel
            static_if (WIDE) {
                for [unroll_full] (j in range(2)) {
                    let slot = uint(j) * 128u + lid
                    txp[slot] = x[(slot / 64u) * nb4 + cb + slot % 64u]
                }
            } else {
                txp[lid] = x[(lid / 64u) * nb4 + cb + lid % 64u]
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
                for [unroll_full] (b in range(NR)) {
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
            for [unroll_full] (b in range(NR)) {
                let xv = x[uint(b) * nb4 + k]
                accg[b] += wg.x * xv.x + wg.y * xv.y + wg.z * xv.z + wg.w * xv.w
                accu[b] += wu.x * xv.x + wu.y * xv.y + wu.z * xv.z + wu.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(NR)) {
            let g = simd_sum(accg[b])
            let u = simd_sum(accu[b])
            if (lane == 0u) {
                y[uint(b) * hidden + row] = (g / (1.0 + exp(-g))) * u
            }
        }
    }
}

[metal_dispatch(name = "enc_w13sw_b2_c", pso = "g_pso_w13sw_b2", tgmem = "MetalGemvW13SwB2_metal_gemv_w13sw_msl_tgmem", tg = 128, grid = "hid/4", params = "hid : int64")]
class MetalGemvW13SwB2 : MetalGemvW13SwB24T {
    override NR = 2
    override PANEL = 128
    override WIDE = false
}

[metal_dispatch(name = "enc_w13sw_b4_c", pso = "g_pso_w13sw_b4", tgmem = "MetalGemvW13SwB4_metal_gemv_w13sw_msl_tgmem", tg = 128, grid = "hid/4", params = "hid : int64")]
class MetalGemvW13SwB4 : MetalGemvW13SwB24T {
    override NR = 4
    override PANEL = 256
    override WIDE = true
}

// M-pad-32 batch GEMM (B >= 5, cost B-independent to 32 rows): MulMm's staging on a 32x32 tile;
// pad token rows carry stale bytes by design (C rows independent, pad Y rows never read back).
// IS_SK: grid.y = k-slice axis, per-tg partial-C planes, MetalSkReduce sums them (deterministic).
[ |> template_struct_instance]
class template MetalQ8GemmBSplitT {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wqb : array<int8>    // 34B-block W blob, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" @off = "boff" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" x : array<float4>    // X [M' x n] f32 rows (M' = 32-padded B; the SK form runs at M' = 32)
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>     // Y [M' x d] rows at ka.ys stride; SK: [ksplit x 32 x d] partial-C planes
    @uniform @binding = 5 ka : Q8GemmArgs
    @workgroup tw : float16[1024]           // W: 4 k-tiles x 4 row-tiles of 8x8, (k row, wrow col)
    @workgroup tx : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, (token row, k col)
    @template_constant IS_SK : bool = false

    [metal_kernel]
    def metal_q8_gemm_b {   // nolint:STYLE038 — MSL kernel body; the staging tiles are barrier coupled
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ka.ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        var mc : simdgroup_float8x8[4]      // this simdgroup's 16x16 quadrant: 2 token x 2 wrow tiles
        var ma : simdgroup_half8x8[2]
        var mb : simdgroup_half8x8[2]
        let kdim4 = ka.kdim / 4u
        let nkb = ka.kdim / 32u
        // staging split: thread handles row lr (0..31) x k-eighth il (0..3) — 8 elems of W AND X
        let lr = lid / 4u
        let il = lid % 4u
        let tW0 = (il * 4u + lr / 8u) * 64u + (lr % 8u)
        let tX0 = (il * 4u + lr / 8u) * 64u + (lr % 8u) * 8u
        let aQ = (sg % 2u) * 128u           // wrow tile-pair base within a k-tile slab
        let bQ = (sg / 2u) * 128u           // token tile-pair base
        // SK: this slice's first k-block is gl_WorkGroupID.y * (nkb / ka.ksplit) — the const-select
        // folds keep the dense stamp byte-identical while the SK stamp inlines the slice origin
        var qp = unsafe(addr(wqb[(IS_SK ? (nBase + lr) * nkb + gl_WorkGroupID.y * (nkb / ka.ksplit) : (nBase + lr) * nkb) * 34u + 2u + il * 8u]))
        var sph = unsafe(addr(wsh[(IS_SK ? (nBase + lr) * nkb + gl_WorkGroupID.y * (nkb / ka.ksplit) : (nBase + lr) * nkb) * 17u]))
        var xp = unsafe(addr(x[(IS_SK ? (mBase + lr) * kdim4 + gl_WorkGroupID.y * (nkb / ka.ksplit) * 8u : (mBase + lr) * kdim4) + il * 2u]))
        var kb = 0u
        while (kb < (IS_SK ? nkb / ka.ksplit : nkb)) {
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
        static_if (IS_SK) {
            let pbase = gl_WorkGroupID.y * 32u * ka.ndim
            let row0 = mBase + (sg / 2u) * 16u
            let col0 = nBase + (sg % 2u) * 16u
            simdgroup_store(mc[0], y, pbase + row0 * ka.ndim + col0, ka.ndim)
            simdgroup_store(mc[1], y, pbase + row0 * ka.ndim + (col0 + 8u), ka.ndim)
            simdgroup_store(mc[2], y, pbase + (row0 + 8u) * ka.ndim + col0, ka.ndim)
            simdgroup_store(mc[3], y, pbase + (row0 + 8u) * ka.ndim + (col0 + 8u), ka.ndim)
        } else {
            let row0 = mBase + (sg / 2u) * 16u
            let col0 = nBase + (sg % 2u) * 16u
            simdgroup_store(mc[0], y, row0 * ka.ys + col0, ka.ys)
            simdgroup_store(mc[1], y, row0 * ka.ys + (col0 + 8u), ka.ys)
            simdgroup_store(mc[2], y, (row0 + 8u) * ka.ys + col0, ka.ys)
            simdgroup_store(mc[3], y, (row0 + 8u) * ka.ys + (col0 + 8u), ka.ys)
        }
    }
}

[metal_dispatch(name = "enc_gemm_b_c", pso = "g_pso_gemm_b", tgmem = "MetalQ8GemmB_metal_q8_gemm_b_msl_tgmem", tg = 128, grid = "ntg", params = "ntg : int64")]
class MetalQ8GemmB : MetalQ8GemmBSplitT {
}

[metal_dispatch(name = "enc_gemm_sk_b_c", pso = "g_pso_gemm_b_sk", tgmem = "MetalQ8GemmBSk_metal_q8_gemm_b_msl_tgmem", tg = 128, grid = "d/32, nsk", params = "d : int64, nsk : int64")]
class MetalQ8GemmBSk : MetalQ8GemmBSplitT {
    override IS_SK = true
}

// the split-K reduce: y = sum of the ksplit partial planes (one thread per output element);
// total = 32 * d is both the element count and the plane stride
[metal_dispatch(name = "enc_sk_reduce_c", pso = "g_pso_skred", tg = 128, grid = "total/128", params = "total : int64")]
class MetalSkReduce {
    @ssbo @binding = 0 @role = "read" part : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @uniform @binding = 2 ka : SkReduceArgs

    [metal_kernel(name="metal_sk_reduce_msl")]
    def metal_sk_reduce {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        var acc = 0.0
        var s = 0u
        while (s < ka.ksplit) {
            acc += part[s * ka.total + gid]
            s++
        }
        y[gid] = acc
    }
}

// Metal-4 tensor twins of the batch GEMMs, one template: same binds/grids as their simdgroup
// parents, bodies lowered to one tmm2d_q8u_f32 (raced via runtime.metal_tensor — crowns
// "gemmb_q8" / "gemmb_sk_q8" / "gemm64b_q8"). Axes: TILE_N width, IS_SK k-slice, LDC_ND stride.
[ |> template_struct_instance]
class template MetalQ8GemmTensorT {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wqb : array<int8>    // 34B-block W blob, byte view
    @ssbo @binding = 2 @role = "weight" @off = "boff" wsh : array<float16> // the same buffer, half-scale view
    @ssbo @binding = 3 @role = "read" x : array<float>     // X [M' x n] f32 rows
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>     // Y [M' x d]; SK: [ksplit x 32 x d] partial-C planes
    @uniform @binding = 5 ka : Q8GemmArgs
    @template_constant TILE_N : uint = 32u
    @template_constant IS_SK : bool = false
    @template_constant LDC_ND : bool = false   // C stride = ka.ndim (SK planes, 64-wide) vs ka.ys
    @workgroup twb : float16[2048]          // W chunk: TILE_N wcols x 32 k, dequant-staged by the helper

    [metal_kernel(float_a_ok=true)]   // batch-decode/classifier family streams f32 X rows - the half-X extension is an open ledger item, this license covers the interim
    def metal_q8_gemm_b_t {
        let ntileN = ka.ndim / TILE_N
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * TILE_N
        let nkb = ka.kdim / 32u
        var sp = unsafe(addr(wsh[(IS_SK ? nBase * nkb + gl_WorkGroupID.y * (nkb / ka.ksplit) : nBase * nkb) * 17u]))
        var wp = unsafe(addr(wqb[(IS_SK ? nBase * nkb + gl_WorkGroupID.y * (nkb / ka.ksplit) : nBase * nkb) * 34u + 2u]))
        var ap = unsafe(addr(x[IS_SK ? mBase * ka.kdim + gl_WorkGroupID.y * (nkb / ka.ksplit) * 32u : mBase * ka.kdim]))
        var cp = unsafe(addr(y[IS_SK ? gl_WorkGroupID.y * 32u * ka.ndim + mBase * ka.ndim + nBase : (LDC_ND ? mBase * ka.ndim + nBase : mBase * ka.ys + nBase)]))
        tmm2d_q8u_f32(32u, TILE_N, 4u, twb, sp, wp, ap, ka.kdim, cp, LDC_ND ? ka.ndim : ka.ys,
            IS_SK ? (nkb / ka.ksplit) * 32u : ka.kdim, nkb, gl_LocalInvocationIndex)
    }
}

[metal_dispatch(name = "enc_gemm_b_t_c", pso = "g_pso_gemm_b", tgmem = "MetalQ8GemmBT_metal_q8_gemm_b_t_msl_tgmem", tg = 128, grid = "ntg", params = "ntg : int64")]
class MetalQ8GemmBT : MetalQ8GemmTensorT {
}

[metal_dispatch(name = "enc_gemm_sk_b_t_c", pso = "g_pso_gemm_b_sk", tgmem = "MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_tgmem", tg = 128, grid = "d/32, nsk", params = "d : int64, nsk : int64")]
class MetalQ8GemmBSkT : MetalQ8GemmTensorT {
    override IS_SK = true
    override LDC_ND = true
}

[metal_dispatch(name = "enc_gemm64_b_t_c", pso = "g_pso_gemm64_b", tgmem = "MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_tgmem", tg = 128, grid = "ntg", params = "ntg : int64")]
class MetalQ8Gemm64BT : MetalQ8GemmTensorT {
    override TILE_N = 64u
    override LDC_ND = true
}

// The 64-wide twin of MulMm's 32(M) x 64(N) tile: half the threadgroups (d/64) at ~29% fewer
// instructions per element — wins where the grid stays fat (e.g. the vocab classifier). Each
// simdgroup owns a 16x32 quadrant (2 token x 4 wrow tiles).
[metal_dispatch(name = "enc_gemm64_b_c", pso = "g_pso_gemm64_b", tgmem = "metal_q8_gemm64_b_msl_tgmem", tg = 128, grid = "ntg", params = "ntg : int64")]
class MetalQ8Gemm64B {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wqb : array<int8>    // 34B-block W blob, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" @off = "boff" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" x : array<float4>    // X [M' x n] f32 rows (M' = 32-padded B)
    @ssbo @binding = 4 @role = "write" y : array<float>     // Y [M' x d]
    @uniform @binding = 5 ka : Q8GemmArgs
    @workgroup tw : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, (k row, wrow col)
    @workgroup tx : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, (token row, k col)

    [metal_kernel(name="metal_q8_gemm64_b_msl")]
    def metal_q8_gemm64_b {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ka.ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        var mc : simdgroup_float8x8[8]      // this simdgroup's 16x32 quadrant: 2 token x 4 wrow tiles
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]                // dequantized W staging regs (this thread's k-half)
        let kdim4 = ka.kdim / 4u
        let nkb = ka.kdim / 32u
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
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ka.ndim + col0 + uint(i % 4) * 8u, ka.ndim)
        }
    }
}

// Batched single-query attention, single-tg form: grid (nheads x B), same simdgroup-cooperative
// math as MetalSqAttnF16, but each row reads its mirror slice through the row table and its
// roped q lives in the fused QKV buffer at stride qstride.
[ |> template_struct_instance]
class template MetalSqAttnBKvT {
    @ssbo @binding = 0 @role = "read" kc : array<KT>  // the whole mirror arena, f16 view
    @ssbo @binding = 1 @role = "read" vc : array<KT>
    @ssbo @binding = 2 @role = "read" q : array<float>     // [B x qstride] roped queries (the fused QKV rows)
    @ssbo @binding = 3 @role = "write" yo : array<float>    // [B x nheads*hs] attention out
    @ssbo @binding = 4 @role = "read" @off = "rtoff" rt : array<uint4>    // per-row (k base, v base, cap_rows, cnt) — arena elems
    @uniform @binding = 5 ka : SqAttnBArgs
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @workgroup scores : float[128]
    @workgroup red : float[16]

    [metal_kernel]
    def metal_sq_attn_b {
        let lay = ka.lay
        let kv_dim = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let qstride = ka.qstride
        let softcap = ka.softcap
        let hass = ka.hass
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
        sq_fill_scores(kc, q, scores, kb0, 0u, cnt, kv_dim, khoff, qoff, hs, scale, softcap)
        barrier()
        var mm = sq_score_max(scores, red, cnt, lid, nt)
        if (hass != 0u) {
            mm = max(mm, sink[h])
        }
        barrier()                           // red[] is reused for the sum below
        var tot = sq_exp_sum(scores, red, mm, cnt, lid, nt)
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
        }
        yo[yoff + lid] = sq_vacc(vc, scores, vb0, 0u, cnt, kv_dim, khoff, lid) / tot
    }
}

[metal_dispatch(name = "enc_attnb16_c", pso = "g_pso_attnb16", tgmem = "MetalSqAttnBF16_metal_sq_attn_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnBF16 : MetalSqAttnBKvT {
    typedef KT = float16
}

// f32-mirror twin.
[metal_dispatch(name = "enc_attnb32_c", pso = "g_pso_attnb32", tgmem = "MetalSqAttnBF32_metal_sq_attn_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnBF32 : MetalSqAttnBKvT {
    typedef KT = float
}

// Batched single-tg quant twin pair — grid (nheads x B), dual-view block reads (see
// MetalSqAttnQuantT). Row-table entries carry arena BYTE offsets, so the half-scale view
// indexes at byte/2 exactly.
[ |> template_struct_instance]
class template MetalSqAttnBQuantT {
    @ssbo @binding = 0 @role = "read" ksh : array<float16> // the whole mirror arena, half-scale view
    @ssbo @binding = 1 @role = "read" kqb : array<QB>    // the SAME arena, byte view
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<QB>
    @ssbo @binding = 4 @role = "read" q : array<float>     // [B x qstride] roped queries (the fused QKV rows)
    @ssbo @binding = 5 @role = "write" yo : array<float>    // [B x nheads*hs] attention out
    @ssbo @binding = 6 @role = "read" @off = "rtoff" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 ka : SqAttnBArgs
    @ssbo @binding = 20 @role = "weight" sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @workgroup scores : float[128]
    @workgroup red : float[16]
    @template_call sq_fill_scores_q = @@sq_fill_scores_q8
    @template_call sq_vacc_q = @@sq_vacc_q8

    [metal_kernel]
    def metal_sq_attn_b {
        let lay = ka.lay
        let nblk = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let qstride = ka.qstride
        let softcap = ka.softcap
        let hass = ka.hass
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
        sq_fill_scores_q(ksh, kqb, q, scores, kb0h, kb0b, 0u, cnt, nblk, khblk, qoff, hs, scale, softcap)
        barrier()
        var mm = sq_score_max(scores, red, cnt, lid, nt)
        if (hass != 0u) {
            mm = max(mm, sink[h])
        }
        barrier()                           // red[] is reused for the sum below
        var tot = sq_exp_sum(scores, red, mm, cnt, lid, nt)
        if (hass != 0u) {
            tot += exp(max(sink[h] - mm, -80.0))
        }
        yo[yoff + lid] = sq_vacc_q(vsh, vqb, scores, vb0h, vb0b, 0u, cnt, nblk, khblk, lid) / tot
    }
}

// q8_0 instance — rowb = nblk*34 (always even).
[metal_dispatch(name = "enc_attnb_q8_c", pso = "g_pso_attnb_q8", tgmem = "MetalSqAttnBQ8_metal_sq_attn_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnBQ8 : MetalSqAttnBQuantT {
    typedef QB = int8
}

// tq4-mirror batch twin (nibble reads over the byte-offset row table; rowb = nblk*18).
[metal_dispatch(name = "enc_attnb_tq4_c", pso = "g_pso_attnb_tq4", tgmem = "MetalSqAttnBTq4_metal_sq_attn_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnBTq4 : MetalSqAttnBQuantT {
    typedef QB = uint8
    override sq_fill_scores_q = @@sq_fill_scores_tq4
    override sq_vacc_q = @@sq_vacc_tq4
}

// Batched chunked split-K float pair — grid (nheads x nchmax x B), ragged depths share one
// dispatch via per-row chunk counts; partials stay f32 for MetalSqAttnCombB.
[ |> template_struct_instance]
class template MetalSqAttnPartBKvT {
    @ssbo @binding = 0 @role = "read" kc : array<KT>
    @ssbo @binding = 1 @role = "read" vc : array<KT>
    @ssbo @binding = 2 @role = "read" q : array<float>
    @ssbo @binding = 3 @role = "write" part : array<float>  // [B x nheads x nchmax x (hs+2)]
    @ssbo @binding = 4 @role = "read" @off = "rtoff" rt : array<uint4>
    @uniform @binding = 5 ka : SqAttnPartBArgs
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]

    [metal_kernel]
    def metal_sq_attn_part_b {
        let lay = ka.lay
        let kv_dim = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let qstride = ka.qstride
        let nchmax = ka.nchmax
        let softcap = ka.softcap
        let window = ka.window
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
        sq_fill_scores(kc, q, scores, kb0, c0, nrows, kv_dim, khoff, qoff, hs, scale, softcap)
        barrier()
        sq_mask_window(scores, c0, nrows, wstart, lid, nt)
        let mm = sq_score_max(scores, red, nrows, lid, nt)
        barrier()                           // red[] is reused for the sum below
        let tot = sq_exp_sum(scores, red, mm, nrows, lid, nt)
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = sq_vacc(vc, scores, vb0, c0, nrows, kv_dim, khoff, lid)
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

[metal_dispatch(name = "enc_attnpb16_c", pso = "g_pso_attnpb16", tgmem = "MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnPartBF16 : MetalSqAttnPartBKvT {
    typedef KT = float16
}

// f32-mirror twin.
[metal_dispatch(name = "enc_attnpb32_c", pso = "g_pso_attnpb32", tgmem = "MetalSqAttnPartBF32_metal_sq_attn_part_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnPartBF32 : MetalSqAttnPartBKvT {
    typedef KT = float
}

// Batched chunked quant twin pair — byte-offset row table + dual-view block reads; partials
// stay f32, so MetalSqAttnCombB serves both forms unchanged.
[ |> template_struct_instance]
class template MetalSqAttnPartBQuantT {
    @ssbo @binding = 0 @role = "read" ksh : array<float16>
    @ssbo @binding = 1 @role = "read" kqb : array<QB>
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<QB>
    @ssbo @binding = 4 @role = "read" q : array<float>
    @ssbo @binding = 5 @role = "write" part : array<float>  // [B x nheads x nchmax x (hs+2)]
    @ssbo @binding = 6 @role = "read" @off = "rtoff" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 ka : SqAttnPartBArgs
    @workgroup scores : float[64]   // == ATTN_CHUNK_ROWS
    @workgroup red : float[16]
    @template_call sq_fill_scores_q = @@sq_fill_scores_q8
    @template_call sq_vacc_q = @@sq_vacc_q8

    [metal_kernel]
    def metal_sq_attn_part_b {
        let lay = ka.lay
        let nblk = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let qstride = ka.qstride
        let nchmax = ka.nchmax
        let softcap = ka.softcap
        let window = ka.window
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
        sq_fill_scores_q(ksh, kqb, q, scores, kb0h, kb0b, c0, nrows, nblk, khblk, qoff, hs, scale, softcap)
        barrier()
        sq_mask_window(scores, c0, nrows, wstart, lid, nt)
        let mm = sq_score_max(scores, red, nrows, lid, nt)
        barrier()                           // red[] is reused for the sum below
        let tot = sq_exp_sum(scores, red, mm, nrows, lid, nt)
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = sq_vacc_q(vsh, vqb, scores, vb0h, vb0b, c0, nrows, nblk, khblk, lid)
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// q8_0 instance.
[metal_dispatch(name = "enc_attnpb_q8_c", pso = "g_pso_attnpb_q8", tgmem = "MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnPartBQ8 : MetalSqAttnPartBQuantT {
    typedef QB = int8
}

// tq4 instance — nibble reads.
[metal_dispatch(name = "enc_attnpb_tq4_c", pso = "g_pso_attnpb_tq4", tgmem = "MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnPartBTq4 : MetalSqAttnPartBQuantT {
    typedef QB = uint8
    override sq_fill_scores_q = @@sq_fill_scores_tq4
    override sq_vacc_q = @@sq_vacc_tq4
}

// Batched cross-chunk combine: MetalSqAttnCombT's BATCHED stamp, grid flattened (nheads x B) —
// callers pass wgs = nheads * nrows.
[metal_dispatch(name = "enc_attn_comb_b", pso = "g_pso_attncombb", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64")]
class MetalSqAttnCombB : MetalSqAttnCombT {
    override BATCHED = true
}

// The split-K reduce's own pair — `total` is the flat element count of one partial plane.
struct SkReduceArgs {
    total : uint
    ksplit : uint
}

// The batched q8 GEMM twins bind their uniforms as ONE kargs value: `ys` is the y TOKEN-row stride
// (the fused-QKV row stride at a segment site), `ksplit` the k-slice count the split-K forms grid
// on. Each form ignores the other's field, which is what lets all six share one slot.
struct Q8GemmArgs {
    kdim : uint
    ndim : uint
    ys : uint
    ksplit : uint
}

// the family's two shapes: the dense form carries a y row stride, the split-K form a k-slice count
def gemm_ka(kdim, ndim, ys : int64) : Q8GemmArgs => (
    Q8GemmArgs(kdim = uint(kdim), ndim = uint(ndim), ys = uint(ys)))

def gemm_sk_ka(kdim, ndim, ksplit : int64) : Q8GemmArgs => (
    Q8GemmArgs(kdim = uint(kdim), ndim = uint(ndim), ksplit = uint(ksplit)))

// The mul_mv-shape fixed-B GEMV twins bind their uniforms as ONE kargs value. `xs4` is the x row
// stride in float4s; `ys` the y ROW stride (ddim dense, or the fused-QKV row stride at a segment
// site). Sharing it lets both twins bind at one slot — the split forms shifted xs4/ys past nrows.
struct Q8MvArgs {
    ndim : uint
    ddim : uint
    nrows : uint
    xs4 : uint
    ys : uint
}

// The K-quant small-batch mv family (9 twins: B = 2|4|8 x k4|k5|k6) binds its uniforms as ONE
// kargs value. `ys` is the y ROW stride in elements; `nr` the live column count the stores gate
// on — the pad columns of a partial group compute and discard.
struct KqMvArgs {
    ndim : uint
    ddim : uint
    ys : uint
    nr : uint
}

// The MoE GEMV family's uniforms as ONE kargs value. `eplane` is the per-expert plane's block
// count in whatever unit the codec counts in (34B blocks for q8_0/mx4, superblocks for k-quants);
// `hasb` is read only by the format that carries per-expert bias rows.
struct MoeGemvArgs {
    ndim : uint
    ddim : uint
    eplane : uint
    xs : uint
    ss : uint
    bxs : uint
    bys : uint
    hasb : uint
}

// The fused rope + KV-row store family binds its uniforms as ONE kargs value. `rot` is the
// partial-rotary width (== head_size for full rope); the batched forms add the layer prefix and
// `qstride`, the per-row stride of the fused QKV block.
struct RopeStoreArgs {
    qd : uint
    head_size : uint
    npairs_q : uint
    npairs : uint
    neox : uint
    has_bias : uint
    rot : uint
}

struct RopeStoreBArgs {
    lay : uint
    kv_dim : uint
    qd : uint
    head_size : uint
    npairs_q : uint
    npairs : uint
    qstride : uint
    neox : uint
    has_bias : uint
    rot : uint
}

// The head-cooperative form folds the per-head RMSNorm in, so it carries that norm's
// weights-present flag and epsilon.
struct RopeStoreHArgs {
    qd : uint
    head_size : uint
    nheads : uint
    kv_mul : uint
    neox : uint
    has_bias : uint
    has_norm : uint
    eps : float
    rot : uint
}

// The q8_0 mirror counts its K/V work in 32-element blocks (`nblk`), not in rope pairs.
struct RopeStoreQ8Args {
    qd : uint
    head_size : uint
    npairs_q : uint
    nblk : uint
    neox : uint
    has_bias : uint
}

struct RopeStoreBQ8Args {
    lay : uint
    nblk : uint
    qd : uint
    head_size : uint
    npairs_q : uint
    qstride : uint
    neox : uint
    has_bias : uint
}

// The tq4 mirror is head-cooperative (the FWHT spans a whole head), so it counts heads.
struct RopeStoreTq4Args {
    qd : uint
    head_size : uint
    nheads : uint
    kv_mul : uint
    neox : uint
    has_bias : uint
}

struct RopeStoreBTq4Args {
    lay : uint
    qd : uint
    head_size : uint
    nheads : uint
    kv_mul : uint
    qstride : uint
    neox : uint
    has_bias : uint
}

// The DeltaNet layer's geometry as ONE kargs value: conv, history advance, per-head L2, the
// fused scan and the gated out-norm are stages of one layer, so they share it.
struct DnArgs {
    cd : uint
    dconv : uint
    npos : uint
    kd : uint
    ds : uint
    nvh : uint
    nkh : uint
    di : uint
    eps : float
    qscale : float
    mid : uint
}

// The fused q8 QKV-GEMV + rope + KV-store twins share one kargs — f16 and f32 mirrors differ
// only in the destination views, never in the geometry.
struct Q8QkvRsArgs {
    ndim : uint
    qd : uint
    kvd : uint
    hs : uint
    neox : uint
    has_bias : uint
}

// Per-head RMSNorm's uniforms as ONE kargs value. `soffe` is the READ offset and `koffe` the
// WRITE one — equal for the in-place QK-norm, different where the no-wv V fusion reads the
// pre-norm K slice and writes the v segment.
struct QkNormArgs {
    nq : uint
    koffe : uint
    rstride : uint
    hs : uint
    woffe : uint
    eps : float
    soffe : uint
}

// The [q|gate] deinterleave is not DeltaNet-only (any q_gated model runs it), so it keeps its own.
struct DnDeintArgs {
    hs : uint
    qd : uint
    qs : uint
}

// The whole DeltaNet geometry derives from the config, so ONE place derives it and the three
// sites (prefill window, single decode, the B=2 verify) differ only in `npos` and `mid`.
def dn_args(c : Config; npos : int64; mid : bool) : DnArgs {
    return DnArgs(cd = uint(dn_conv_dim(c)), dconv = uint(c.ssm_d_conv), npos = uint(npos),
        kd = uint(dn_key_dim(c)), ds = uint(c.ssm_d_state), nvh = uint(c.ssm_dt_rank),
        nkh = uint(c.ssm_n_group), di = uint(c.ssm_d_inner), eps = c.norm_eps,
        qscale = 1.0 / sqrt(float(c.ssm_d_state)), mid = mid ? 1u : 0u)
}

// ===== the split-K "D" attention family: the stages that do not depend on the K/V codec =====
// One simdgroup owns a 32-position slice and merges it online into its running state. Only the
// score and V-accumulate steps read K/V, so only those stay per-codec in each kernel below.

def private sqd_zero(var wacc : float[512]; wo : uint) {
    wacc[wo] = 0.0
    wacc[wo + 1u] = 0.0
    wacc[wo + 2u] = 0.0
    wacc[wo + 3u] = 0.0
}

def private sqd_block_max(keep : float) : float {
    var bm = keep
    bm = max(bm, simd_shuffle_xor(bm, 16u))
    bm = max(bm, simd_shuffle_xor(bm, 8u))
    bm = max(bm, simd_shuffle_xor(bm, 4u))
    bm = max(bm, simd_shuffle_xor(bm, 2u))
    bm = max(bm, simd_shuffle_xor(bm, 1u))
    return bm
}

// The f16/f32 codec pair: K and V stream through an advancing pointer, and `float4()` over an
// already-float4 element is the no-op that lets one source line serve both instances.

// this lane's score for its own row of the 32-row block; invalid tail lanes keep the -inf seed
def private sqd_score_blk(var kp : auto(KP) const?; q4 : float4; keep0 : float; nrows, kv4, sl : uint) : float {
    var keep = keep0
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
    return keep
}

// The block-quantized codec pair. Every stride is a codec property, so both live in the
// per-codec overloads as literals — nothing about the block layout reaches the GPU as a value.

// this lane's 4 K/V elements of block `blk` — q8_0 blocks are 34B (f16 d, then 32 int8 quants)
def private sqd_quants(qb : array<int8>; b0b, blk, inb : uint) : float4 {
    let o = b0b + blk * 34u + 2u + inb
    return float4(float(qb[o]), float(qb[o + 1u]), float(qb[o + 2u]), float(qb[o + 3u]))
}

// tq4 twin: 18B blocks, and the lane's 4 elements are the nibbles of 2 bytes
def private sqd_quants(qb : array<uint8>; b0b, blk, inb : uint) : float4 {
    let o = b0b + blk * 18u + 2u + inb
    let by0 = uint(qb[o])
    let by1 = uint(qb[o + 1u])
    return float4(float(int(by0 & 15u) - 8), float(int(by0 >> 4u) - 8),
        float(int(by1 & 15u) - 8), float(int(by1 >> 4u) - 8))
}

// the scale plane strides with the block size: 17 f16 per q8_0 block, 9 per tq4. `qb` names the
// codec (it is the overload discriminator), not data.
[unused_argument(qb)]
def private sqd_dscale(qb : array<int8>; sh : array<float16>; b0h, blk : uint) : float => float(sh[b0h + blk * 17u])

[unused_argument(qb)]
def private sqd_dscale(qb : array<uint8>; sh : array<float16>; b0h, blk : uint) : float => float(sh[b0h + blk * 9u])

def private sqd_score_blk_q(qb : array<auto(QT)>; sh : array<float16>; q4 : float4; keep0 : float;
                            c0, nrows, nblk, blkoff, b0b, b0h, inb, sl : uint) : float {
    var keep = keep0
    if (nrows == 32u) {
        for [unroll_full] (i in range(32)) {
            let blk = (c0 + uint(i)) * nblk + blkoff
            let p = q4 * sqd_quants(qb, b0b, blk, inb)
            let sc = simd_sum((p.x + p.y + p.z + p.w) * sqd_dscale(qb, sh, b0h, blk))
            keep = uint(i) == sl ? sc : keep
        }
    } else {
        var j = 0u
        while (j < nrows) {
            let blk = (c0 + j) * nblk + blkoff
            let p = q4 * sqd_quants(qb, b0b, blk, inb)
            let sc = simd_sum((p.x + p.y + p.z + p.w) * sqd_dscale(qb, sh, b0h, blk))
            keep = j == sl ? sc : keep
            j++
        }
    }
    return keep
}

def private sqd_vacc_blk_q(qb : array<auto(QT)>; sh : array<float16>; w : float;
                           c0, nrows, nblk, blkoff, b0b, b0h, inb : uint) : float4 {
    var bacc = float4(0.0)
    if (nrows == 32u) {
        for [unroll_full] (i in range(32)) {
            let blk = (c0 + uint(i)) * nblk + blkoff
            bacc += (simd_shuffle(w, uint(i)) * sqd_dscale(qb, sh, b0h, blk)) * sqd_quants(qb, b0b, blk, inb)
        }
    } else {
        var j = 0u
        while (j < nrows) {
            let blk = (c0 + j) * nblk + blkoff
            bacc += (simd_shuffle(w, j) * sqd_dscale(qb, sh, b0h, blk)) * sqd_quants(qb, b0b, blk, inb)
            j++
        }
    }
    return bacc
}

def private sqd_vacc_blk(var vp : auto(VP) const?; w : float; nrows, kv4 : uint) : float4 {
    var bacc = float4(0.0)
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
    return bacc
}

// online merge into the running slice-group state (first pass: r1 = exp(-inf) = 0)
def private sqd_merge(var wacc : float[512]; wo : uint; bacc : float4; bm, btot : float;
                      var mm : float&; var tot : float&) {
    let nm = max(mm, bm)
    let r1 = exp(mm - nm)
    let r2 = exp(bm - nm)
    wacc[wo] = wacc[wo] * r1 + bacc.x * r2
    wacc[wo + 1u] = wacc[wo + 1u] * r1 + bacc.y * r2
    wacc[wo + 2u] = wacc[wo + 2u] * r1 + bacc.z * r2
    wacc[wo + 3u] = wacc[wo + 3u] * r1 + bacc.w * r2
    tot = tot * r1 + btot * r2
    mm = nm
}

def private sqd_store(var part : array<float>; wacc : float[512]; pbase, wo, sl, hs : uint;
                      mm, tot : float) {
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

// Fused single-pass batched attention: beats the upstream vector form at every depth 512-16K. One
// simdgroup per (row, head, 32-slice-group), scores kept in-register (no divergent store), V
// accumulated lane-private (zero barriers). hs == 128 only; grid is HEAD-FASTEST for SLC locality.
[ |> template_struct_instance]
class template MetalSqAttnDKvT {
    @ssbo @binding = 0 @role = "read" kc : array<KT>
    @ssbo @binding = 1 @role = "read" vc : array<KT>
    @ssbo @binding = 2 @role = "read" q : array<float4>
    @ssbo @binding = 3 @role = "write" part : array<float>
    @ssbo @binding = 4 @role = "read" rt : array<uint4>
    @uniform @binding = 5 ka : SqAttnDArgs
    @workgroup wacc : float[512]

    [metal_kernel]
    def metal_sq_attn_d {
        let lay = ka.lay
        let kv_dim = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let qstride = ka.qstride
        let nsgs = ka.nsgs
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
        sqd_zero(wacc, wo)
        var mm = -3.0e38
        var tot = 0.0
        var ss = gsg
        while (ss * 32u < cnt) {
            let c0 = ss * 32u
            let nrows = min(c0 + 32u, cnt) - c0
            var kp = unsafe(addr(kc[(kb0 + c0 * kv_dim + khoff) / 4u + sl]))
            var vp = unsafe(addr(vc[(vb0 + c0 * kv_dim + khoff) / 4u + sl]))
            // invalid tail lanes keep the -inf seed -> weight 0
            let keep = sqd_score_blk(kp, q4, -3.0e38, nrows, kv4, sl) * scale
            let bm = sqd_block_max(keep)
            let w = exp(keep - bm)
            let btot = simd_sum(w)
            let bacc = sqd_vacc_blk(vp, w, nrows, kv4)
            sqd_merge(wacc, wo, bacc, bm, btot, mm, tot)
            ss += nsgs
        }
        let pbase = ((bb * nheads + h) * nsgs + gsg) * (hs + 4u)
        sqd_store(part, wacc, pbase, wo, sl, hs, mm, tot)
    }
}

[metal_dispatch(name = "enc_attnd16_c", pso = "g_pso_attnd16", tgmem = "MetalSqAttnDF16_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs/4, nrows", params = "nheads : int64, nsgs : int64, nrows : int64")]
class MetalSqAttnDF16 : MetalSqAttnDKvT {
    typedef KT = half4
}

// f32-mirror twin — float4 view, no conversion.
[metal_dispatch(name = "enc_attnd32_c", pso = "g_pso_attnd32", tgmem = "MetalSqAttnDF32_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs/4, nrows", params = "nheads : int64, nsgs : int64, nrows : int64")]
class MetalSqAttnDF32 : MetalSqAttnDKvT {
    typedef KT = float4
}

// Cross-slice combine for the D entries: PARALLEL over entries (4 sgs online-merge their
// subsets with float4 loads — m and l arrive in ONE float4), a single tgmem cross-sg merge.
[metal_dispatch(name = "enc_attncombd_c", pso = "g_pso_attncombd", tgmem = "metal_sq_attn_comb_d_msl_tgmem", tg = "head_size", grid = "nheads, nrows", params = "nheads : int64, nrows : int64, head_size : int64")]
class MetalSqAttnCombD {
    @ssbo @binding = 0 @role = "read" part : array<float4>
    @ssbo @binding = 1 @role = "write" yo : array<float>
    @ssbo @binding = 2 @role = "read" rt : array<uint4>
    @uniform @binding = 3 ka : SqAttnCombDArgs
    @workgroup wacc : float[512]
    @workgroup wml : float[8]

    [metal_kernel(name="metal_sq_attn_comb_d_msl")]
    def metal_sq_attn_comb_d {
        let nchmax = ka.nchmax
        let nheads = ka.nheads
        let hs = ka.hs
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
// rope into its mirror row (f16 instances clamp to ±65504) at depth cnt-1, v pairs copy alongside.
[ |> template_struct_instance]
class template MetalRopeStoreBKvT {
    @ssbo @binding = 0 @role = "readwrite" qk : array<float>    // [B x qstride] fused QKV rows — q ropes in place
    @ssbo @binding = 1 @role = "write" kd : array<KT>  // the whole mirror arena (k dest)
    @ssbo @binding = 2 @role = "write" vd : array<KT>
    @ssbo @binding = 3 @role = "weight" tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 4 @role = "weight" tsin : array<float>
    @ssbo @binding = 5 @role = "read" @off = "rtoff" rt : array<uint4>
    @ssbo @binding = 6 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 7 ka : RopeStoreBArgs
    @template_constant CLAMP16 : bool = true

    [metal_kernel]
    def metal_rope_store_b {
        let lay = ka.lay
        let kv_dim = ka.kv_dim
        let qd = ka.qd
        let head_size = ka.head_size
        let npairs_q = ka.npairs_q
        let npairs = ka.npairs
        let qstride = ka.qstride
        let neox = ka.neox
        let has_bias = ka.has_bias
        let rot = ka.rot
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
            static_if (CLAMP16) {
                let r0 = v0 * fcr - v1 * fci
                let r1 = v0 * fci + v1 * fcr
                kd[kdst + i] = float16(min(max(r0, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
                kd[kdst + i2] = float16(min(max(r1, -65504.0), 65504.0))   // nolint:STYLE019
                let b0 = has_bias != 0u ? bias[qd + kv_dim + i] : 0.0
                let b1 = has_bias != 0u ? bias[qd + kv_dim + i2] : 0.0
                vd[vdst + i] = float16(min(max(qk[qbase + qd + kv_dim + i] + b0, -65504.0), 65504.0))   // nolint:STYLE019
                vd[vdst + i2] = float16(min(max(qk[qbase + qd + kv_dim + i2] + b1, -65504.0), 65504.0))   // nolint:STYLE019
            } else {
                kd[kdst + i] = v0 * fcr - v1 * fci
                kd[kdst + i2] = v0 * fci + v1 * fcr
                vd[vdst + i] = qk[qbase + qd + kv_dim + i] + (has_bias != 0u ? bias[qd + kv_dim + i] : 0.0)
                vd[vdst + i2] = qk[qbase + qd + kv_dim + i2] + (has_bias != 0u ? bias[qd + kv_dim + i2] : 0.0)
            }
        } else {
            let v0 = qk[qbase + i] + (has_bias != 0u ? bias[i] : 0.0)
            let v1 = qk[qbase + i2] + (has_bias != 0u ? bias[i2] : 0.0)
            qk[qbase + i] = v0 * fcr - v1 * fci
            qk[qbase + i2] = v0 * fci + v1 * fcr
        }
    }
}

[metal_dispatch(name = "enc_rpstb16_c", pso = "g_pso_rpstb16", tg = 64, grid = "npairs/64, nrows", params = "npairs : int64, nrows : int64")]
class MetalRopeStoreBF16 : MetalRopeStoreBKvT {
    typedef KT = float16
}

// f32-mirror twin — straight stores, no clamp.
[metal_dispatch(name = "enc_rpstb32_c", pso = "g_pso_rpstb32", tg = 64, grid = "npairs/64, nrows", params = "npairs : int64, nrows : int64")]
class MetalRopeStoreBF32 : MetalRopeStoreBKvT {
    typedef KT = float
    override CLAMP16 = false
}
// Quant partD twin pair (hs == 128 only, DF16 shape): one sg per (row, head, 32-position
// slice), online slice merge, byte-offset row table. The sqd_*_blk_q stages overload on the
// byte-view type, so only LANE_BYTES (quant bytes per lane's 4 elements) tells them apart.
[ |> template_struct_instance]
class template MetalSqAttnDQuantT {
    @ssbo @binding = 0 @role = "read" ksh : array<float16>
    @ssbo @binding = 1 @role = "read" kqb : array<QB>
    @ssbo @binding = 2 @role = "read" vsh : array<float16>
    @ssbo @binding = 3 @role = "read" vqb : array<QB>
    @ssbo @binding = 4 @role = "read" q : array<float4>
    @ssbo @binding = 5 @role = "write" part : array<float>
    @ssbo @binding = 6 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 ka : SqAttnDArgs
    @workgroup wacc : float[512]
    @template_constant LANE_BYTES : uint = 4u

    [metal_kernel]
    def metal_sq_attn_d {
        let lay = ka.lay
        let nblk = ka.kstride
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let hs = ka.hs
        let scale = ka.scale
        let qstride = ka.qstride
        let nsgs = ka.nsgs
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
        let inb = (sl % 8u) * LANE_BYTES        // lane's byte offset inside its block (4 quant bytes / 2 nibble bytes)
        let wo = gl_SubgroupID * 128u + sl * 4u
        sqd_zero(wacc, wo)
        var mm = -3.0e38
        var tot = 0.0
        var ss = gsg
        while (ss * 32u < cnt) {
            let c0 = ss * 32u
            let nrows = min(c0 + 32u, cnt) - c0
            // invalid tail lanes keep the -inf seed -> weight 0
            let keep = sqd_score_blk_q(kqb, ksh, q4, -3.0e38, c0, nrows, nblk, blkoff, kb0b, kb0h, inb, sl) * scale
            let bm = sqd_block_max(keep)
            let w = exp(keep - bm)
            let btot = simd_sum(w)
            let bacc = sqd_vacc_blk_q(vqb, vsh, w, c0, nrows, nblk, blkoff, vb0b, vb0h, inb)
            sqd_merge(wacc, wo, bacc, bm, btot, mm, tot)
            ss += nsgs
        }
        let pbase = ((bb * nheads + h) * nsgs + gsg) * (hs + 4u)
        sqd_store(part, wacc, pbase, wo, sl, hs, mm, tot)
    }
}

// q8_0 instance — K/V loads are 4 scalar int8s + block scale per position (34B blocks break
// half4 alignment); scale folds in before the simd_sum.
[metal_dispatch(name = "enc_attnd_q8_c", pso = "g_pso_attnd_q8", tgmem = "MetalSqAttnDQ8_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs/4, nrows", params = "nheads : int64, nsgs : int64, nrows : int64")]
class MetalSqAttnDQ8 : MetalSqAttnDQuantT {
    typedef QB = int8
}

// tq4 instance — nibble reads: lane's 4 elements = 2 nibble bytes of block sl/8.
[metal_dispatch(name = "enc_attnd_tq4_c", pso = "g_pso_attnd_tq4", tgmem = "MetalSqAttnDTq4_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs/4, nrows", params = "nheads : int64, nsgs : int64, nrows : int64")]
class MetalSqAttnDTq4 : MetalSqAttnDQuantT {
    typedef QB = uint8
    override LANE_BYTES = 2u
}

// q8_0-mirror batched rope + KV-row store: the RopeStoreQ8 shape with the byte-offset row table
// (grid y = batch row; the new KV row lands at cnt-1). Same quantize_q8kv_row semantics.
[metal_dispatch(name = "enc_rpstb_q8_c", pso = "g_pso_rpstb_q8", tg = 64, grid = "nthreads/64, nrows", params = "nthreads : int64, nrows : int64")]
class MetalRopeStoreBQ8 : MetalRopeStoreQ8Base {
    @ssbo @binding = 0 @role = "readwrite" qk : array<float>    // [B x qstride] fused QKV rows — q ropes in place
    @ssbo @binding = 1 @role = "write" ksh : array<float16> // the whole mirror arena, half-scale view (k dest)
    @ssbo @binding = 2 @role = "write" kqb : array<int8>    // the SAME arena, byte view
    @ssbo @binding = 3 @role = "write" vsh : array<float16>
    @ssbo @binding = 4 @role = "write" vqb : array<int8>
    @ssbo @binding = 5 @role = "weight" tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 6 @role = "weight" tsin : array<float>
    @ssbo @binding = 7 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @ssbo @binding = 8 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 9 ka : RopeStoreBQ8Args

    [metal_kernel(name="metal_rope_store_b_q8_msl")]
    def metal_rope_store_b_q8 {
        let lay = ka.lay
        let nblk = ka.nblk
        let qd = ka.qd
        let head_size = ka.head_size
        let npairs_q = ka.npairs_q
        let qstride = ka.qstride
        let neox = ka.neox
        let has_bias = ka.has_bias
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs_q + 2u * nblk) {
            return
        }
        let bb = gl_WorkGroupID.y
        let rrow = rt[bb]
        let qbase = bb * qstride
        let half = head_size / 2u
        let tcb = bb * half
        if (gid < npairs_q) {
            q8_rope_q(qk, tcos, tsin, bias, gid, half, head_size, neox, has_bias, qbase, tcb)
            return
        }
        let rowb = nblk * 34u
        let kvd = nblk * 32u
        let bi = gid - npairs_q
        let isV = bi >= nblk
        let blk = isV ? bi - nblk : bi
        let base = blk * 32u                // element base within the K/V row
        let kqoff = qbase + qd              // the fused row's k region (v = its third region)
        let vqoff = qbase + qd + kvd
        var vals : float[32]
        q8_fill_kv_vals(qk, qk, tcos, tsin, bias, vals, kqoff, vqoff, tcb, base, head_size, half, qd, kvd, neox, has_bias, isV)
        var d = 0.0
        var id = 0.0
        q8_scale(vals, d, id)
        if (isV) {
            let vb0b = rrow.y + lay * rrow.z + (rrow.w - 1u) * rowb   // lay = row-byte prefix
            let vb0h = vb0b / 2u
            q8_store(vsh, vqb, vals, d, id, vb0h, vb0b, blk)
        } else {
            let kb0b = rrow.x + lay * rrow.z + (rrow.w - 1u) * rowb
            let kb0h = kb0b / 2u
            q8_store(ksh, kqb, vals, d, id, kb0h, kb0b, blk)
        }
    }
}

// tq4-mirror batched rope + KV-row store: the RopeStoreTq4 shape (threadgroup per head) with the
// byte-offset row table — grid (heads, batch rows); the new KV row lands at cnt-1.
[metal_dispatch(name = "enc_rpstb_tq4_c", pso = "g_pso_rpstb_tq4", tgmem = "metal_rope_store_b_tq4_msl_tgmem", tg = "head_size", grid = "ntg, nrows", params = "ntg : int64, nrows : int64, head_size : int64")]
class MetalRopeStoreBTq4 : MetalRopeStoreTq4Base {
    @ssbo @binding = 0 @role = "readwrite" qk : array<float>    // [B x qstride] fused QKV rows — q ropes + rotates in place
    @ssbo @binding = 1 @role = "write" ksh : array<float16> // the whole mirror arena, half-scale view (k dest)
    @ssbo @binding = 2 @role = "write" kqb : array<uint8>   // the SAME arena, byte view
    @ssbo @binding = 3 @role = "write" vsh : array<float16>
    @ssbo @binding = 4 @role = "write" vqb : array<uint8>
    @ssbo @binding = 5 @role = "weight" tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 6 @role = "weight" tsin : array<float>
    @ssbo @binding = 7 @role = "weight" signs : array<float>
    @ssbo @binding = 8 @role = "read" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @ssbo @binding = 9 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 10 ka : RopeStoreBTq4Args

    [metal_kernel(name="metal_rope_store_b_tq4_msl")]
    def metal_rope_store_b_tq4 {
        let lay = ka.lay
        let qd = ka.qd
        let head_size = ka.head_size
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let qstride = ka.qstride
        let neox = ka.neox
        let has_bias = ka.has_bias
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
        tq4_fwht(v, signs, lid, hs)
        if (isQ) {
            qk[base + lid] = v
            return
        }
        let sl = gl_SubgroupInvocationID
        var d = 0.0
        var qv = 0
        var qother = 0
        tq4_quant(v, d, qv, qother, sl)
        let rowb = (kv_dim / 32u) * 18u
        let blk = hh * (hs / 32u) + lid / 32u   // block within the K/V row
        if (isK) {
            let kb0b = rrow.x + lay * rrow.z + (rrow.w - 1u) * rowb   // lay = row-byte prefix
            tq4_store(ksh, kqb, d, qv, qother, kb0b, blk, lid, sl)
        } else {
            let vb0b = rrow.y + lay * rrow.z + (rrow.w - 1u) * rowb
            tq4_store(vsh, vqb, d, qv, qother, vb0b, blk, lid, sl)
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
        to_log(LOG_ERROR, "dasLLAMA metal decode: `{entry}` compile failed - {err}\n")
        ok = false
    } else {
        kcov_register(pso, entry)   // the coverage census names every compiled kernel
    }
    return pso
}


[cold_path]   // one-time PSO/device bring-up, guarded by its own inited flag
def metal_decode_init : bool {   // nolint:STYLE038 — a flat compile_pso per kernel; no seam to split on
    if (g_pso_add != null) {
        return true
    }
    if (!metal_common_init()) {
        return false
    }
    var ok = true
    // the GEMV / rms / add / swiglu MSL sources are the prefill module's, verbatim (own PSOs)
    g_pso_gemv = compile_pso(MetalQ8Gemv_metal_q8_gemv_msl, MetalQ8Gemv_metal_q8_gemv_msl_entry, MetalQ8Gemv_metal_q8_gemv_msl_fastmath, ok)
    g_gemv_rows = clamp(int64(g_env_metal.gemv_tg), 1l, 32l)
    g_pso_rms = compile_pso(MetalRmsNorm_metal_rmsnorm_msl, MetalRmsNorm_metal_rmsnorm_msl_entry, MetalRmsNorm_metal_rmsnorm_msl_fastmath, ok)
    g_pso_rms_hx = compile_pso(MetalRmsNormHX_metal_rmsnorm_msl, MetalRmsNormHX_metal_rmsnorm_msl_entry, MetalRmsNormHX_metal_rmsnorm_msl_fastmath, ok)
    g_pso_add = compile_pso(MetalAdd_metal_ew2_msl, MetalAdd_metal_ew2_msl_entry, MetalAdd_metal_ew2_msl_fastmath, ok)
    g_pso_addrms = compile_pso(MetalAddRms_metal_add_rms_msl, MetalAddRms_metal_add_rms_msl_entry, MetalAddRms_metal_add_rms_msl_fastmath, ok)
    g_pso_addrms_hx = compile_pso(MetalAddRmsHX_metal_add_rms_msl, MetalAddRmsHX_metal_add_rms_msl_entry, MetalAddRmsHX_metal_add_rms_msl_fastmath, ok)
    g_pso_preaddrms = compile_pso(metal_pre_add_rms_msl, metal_pre_add_rms_msl_entry, metal_pre_add_rms_msl_fastmath, ok)
    g_pso_swiglu = compile_pso(MetalSwiglu_metal_ew2_msl, MetalSwiglu_metal_ew2_msl_entry, MetalSwiglu_metal_ew2_msl_fastmath, ok)
    g_pso_swiglu_hx = compile_pso(MetalSwigluHX_metal_ew2_msl, MetalSwigluHX_metal_ew2_msl_entry, MetalSwigluHX_metal_ew2_msl_fastmath, ok)
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
    g_pso_geglu = compile_pso(MetalGeglu_metal_ew2_msl, MetalGeglu_metal_ew2_msl_entry, MetalGeglu_metal_ew2_msl_fastmath, ok)
    g_pso_geglu_hx = compile_pso(MetalGegluHX_metal_ew2_msl, MetalGegluHX_metal_ew2_msl_entry, MetalGegluHX_metal_ew2_msl_fastmath, ok)
    g_pso_ln = compile_pso(metal_layernorm_msl, metal_layernorm_msl_entry, metal_layernorm_msl_fastmath, ok)
    g_pso_f32mm = compile_pso(metal_f32_mulmm_msl, metal_f32_mulmm_msl_entry, metal_f32_mulmm_msl_fastmath, ok)
    g_pso_gelu_erf = compile_pso(MetalGeluErf_metal_ew1_msl, MetalGeluErf_metal_ew1_msl_entry, MetalGeluErf_metal_ew1_msl_fastmath, ok)
    g_pso_gelu_lut = compile_pso(MetalGeluLut_metal_ew1_msl, MetalGeluLut_metal_ew1_msl_entry, MetalGeluLut_metal_ew1_msl_fastmath, ok)
    g_pso_posadd = compile_pso(metal_posadd2d_msl, metal_posadd2d_msl_entry, metal_posadd2d_msl_fastmath, ok)
    g_pso_q3v_stem = compile_pso(metal_q3v_stem_msl, metal_q3v_stem_msl_entry, metal_q3v_stem_msl_fastmath, ok)
    g_pso_clamp = compile_pso(metal_clamp_msl, metal_clamp_msl_entry, metal_clamp_msl_fastmath, ok)
    g_pso_rope2d = compile_pso(metal_rope2d_msl, metal_rope2d_msl_entry, metal_rope2d_msl_fastmath, ok)
    g_pso_head_restride = compile_pso(metal_head_restride_msl, metal_head_restride_msl_entry, metal_head_restride_msl_fastmath, ok)
    g_pso_geglu_quick = compile_pso(MetalGegluQuick_metal_ew2_msl, MetalGegluQuick_metal_ew2_msl_entry, MetalGegluQuick_metal_ew2_msl_fastmath, ok)
    g_pso_cross_kx = compile_pso(metal_cross_kx_msl, metal_cross_kx_msl_entry, metal_cross_kx_msl_fastmath, ok)
    g_pso_cross_vx = compile_pso(metal_cross_vx_msl, metal_cross_vx_msl_entry, metal_cross_vx_msl_fastmath, ok)
    g_pso_cross_vx_h = compile_pso(metal_cross_vx_h_msl, metal_cross_vx_h_msl_entry, metal_cross_vx_h_msl_fastmath, ok)
    g_pso_im2col3 = compile_pso(metal_im2col3_msl, metal_im2col3_msl_entry, metal_im2col3_msl_fastmath, ok)
    g_pso_im2col2d = compile_pso(metal_im2col2d_msl, metal_im2col2d_msl_entry, metal_im2col2d_msl_fastmath, ok)
    g_pso_wdec_attn_part = compile_pso(metal_wdec_attn_part_msl, metal_wdec_attn_part_msl_entry, metal_wdec_attn_part_msl_fastmath, ok)
    g_pso_wdec_attn_comb = compile_pso(metal_wdec_attn_comb_msl, metal_wdec_attn_comb_msl_entry, metal_wdec_attn_comb_msl_fastmath, ok)
    g_pso_bias_add_res = compile_pso(metal_bias_add_res_msl, metal_bias_add_res_msl_entry, metal_bias_add_res_msl_fastmath, ok)
    g_pso_bias_gelu_lut = compile_pso(metal_bias_gelu_lut_msl, metal_bias_gelu_lut_msl_entry, metal_bias_gelu_lut_msl_fastmath, ok)
    g_pso_softcap = compile_pso(metal_softcap_row_msl, metal_softcap_row_msl_entry, metal_softcap_row_msl_fastmath, ok)
    g_pso_suppress = compile_pso(metal_suppress_row_msl, metal_suppress_row_msl_entry, metal_suppress_row_msl_fastmath, ok)
    g_pso_attn16 = compile_pso(MetalSqAttnF16_metal_sq_attn_msl, MetalSqAttnF16_metal_sq_attn_msl_entry, MetalSqAttnF16_metal_sq_attn_msl_fastmath, ok)
    g_pso_attn32 = compile_pso(MetalSqAttnF32_metal_sq_attn_msl, MetalSqAttnF32_metal_sq_attn_msl_entry, MetalSqAttnF32_metal_sq_attn_msl_fastmath, ok)
    g_pso_attnp16 = compile_pso(MetalSqAttnPartF16_metal_sq_attn_part_msl, MetalSqAttnPartF16_metal_sq_attn_part_msl_entry, MetalSqAttnPartF16_metal_sq_attn_part_msl_fastmath, ok)
    g_pso_attnp32 = compile_pso(MetalSqAttnPartF32_metal_sq_attn_part_msl, MetalSqAttnPartF32_metal_sq_attn_part_msl_entry, MetalSqAttnPartF32_metal_sq_attn_part_msl_fastmath, ok)
    g_pso_attncomb = compile_pso(MetalSqAttnComb_metal_sq_attn_comb_msl, MetalSqAttnComb_metal_sq_attn_comb_msl_entry, MetalSqAttnComb_metal_sq_attn_comb_msl_fastmath, ok)
    g_pso_attn_q8 = compile_pso(MetalSqAttnQ8_metal_sq_attn_msl, MetalSqAttnQ8_metal_sq_attn_msl_entry, MetalSqAttnQ8_metal_sq_attn_msl_fastmath, ok)
    g_pso_attnp_q8 = compile_pso(MetalSqAttnPartQ8_metal_sq_attn_part_msl, MetalSqAttnPartQ8_metal_sq_attn_part_msl_entry, MetalSqAttnPartQ8_metal_sq_attn_part_msl_fastmath, ok)
    g_pso_attn_tq4 = compile_pso(MetalSqAttnTq4_metal_sq_attn_msl, MetalSqAttnTq4_metal_sq_attn_msl_entry, MetalSqAttnTq4_metal_sq_attn_msl_fastmath, ok)
    g_pso_attnp_tq4 = compile_pso(MetalSqAttnPartTq4_metal_sq_attn_part_msl, MetalSqAttnPartTq4_metal_sq_attn_part_msl_entry, MetalSqAttnPartTq4_metal_sq_attn_part_msl_fastmath, ok)
    g_pso_rpst16 = compile_pso(MetalRopeStoreF16_metal_rope_store_msl, MetalRopeStoreF16_metal_rope_store_msl_entry, MetalRopeStoreF16_metal_rope_store_msl_fastmath, ok)
    g_pso_rpst_h16 = compile_pso(metal_rope_store_h16_msl, metal_rope_store_h16_msl_entry, metal_rope_store_h16_msl_fastmath, ok)
    g_pso_rpst32 = compile_pso(MetalRopeStoreF32_metal_rope_store_msl, MetalRopeStoreF32_metal_rope_store_msl_entry, MetalRopeStoreF32_metal_rope_store_msl_fastmath, ok)
    g_pso_rpst_q8 = compile_pso(metal_rope_store_q8_msl, metal_rope_store_q8_msl_entry, metal_rope_store_q8_msl_fastmath, ok)
    g_pso_rpst_tq4 = compile_pso(metal_rope_store_tq4_msl, metal_rope_store_tq4_msl_entry, metal_rope_store_tq4_msl_fastmath, ok)
    g_pso_unsign = compile_pso(metal_fwht_unsign_msl, metal_fwht_unsign_msl_entry, metal_fwht_unsign_msl_fastmath, ok)
    g_pso_qknorm = compile_pso(metal_qk_norm_msl, metal_qk_norm_msl_entry, metal_qk_norm_msl_fastmath, ok)
    g_pso_argmax = compile_pso(metal_argmax_msl, metal_argmax_msl_entry, metal_argmax_msl_fastmath, ok)
    g_pso_moe_router = compile_pso(MetalMoeRouter_metal_moe_router_msl, MetalMoeRouter_metal_moe_router_msl_entry, MetalMoeRouter_metal_moe_router_msl_fastmath, ok)
    g_pso_moe_router_b = compile_pso(MetalMoeRouterB_metal_moe_router_msl, MetalMoeRouterB_metal_moe_router_msl_entry, MetalMoeRouterB_metal_moe_router_msl_fastmath, ok)
    g_pso_dn_ba = compile_pso(MetalDnBa_metal_q8_gemv_msl, MetalDnBa_metal_q8_gemv_msl_entry, MetalDnBa_metal_q8_gemv_msl_fastmath, ok)
    g_pso_moe_select = compile_pso(metal_moe_select_msl, metal_moe_select_msl_entry, metal_moe_select_msl_fastmath, ok)
    g_pso_moe_gemv_q8 = compile_pso(metal_moe_gemv_q8_msl, metal_moe_gemv_q8_msl_entry, metal_moe_gemv_q8_msl_fastmath, ok)
    g_pso_moe_kq_k4 = compile_pso(metal_moe_gemv_k4_msl, metal_moe_gemv_k4_msl_entry, metal_moe_gemv_k4_msl_fastmath, ok)
    g_pso_moe_kq_k5 = compile_pso(metal_moe_gemv_k5_msl, metal_moe_gemv_k5_msl_entry, metal_moe_gemv_k5_msl_fastmath, ok)
    g_pso_moe_kq_k6 = compile_pso(metal_moe_gemv_k6_msl, metal_moe_gemv_k6_msl_entry, metal_moe_gemv_k6_msl_fastmath, ok)
    g_pso_moe_combine = compile_pso(metal_moe_combine_msl, metal_moe_combine_msl_entry, metal_moe_combine_msl_fastmath, ok)
    g_pso_g4rnorm = compile_pso(metal_g4_router_norm_msl, metal_g4_router_norm_msl_entry, metal_g4_router_norm_msl_fastmath, ok)
    g_pso_moe_wscale = compile_pso(metal_moe_wscale_msl, metal_moe_wscale_msl_entry, metal_moe_wscale_msl_fastmath, ok)
    g_pso_swiglu_oai = compile_pso(MetalSwigluOai_metal_swiglu_oai_msl, MetalSwigluOai_metal_swiglu_oai_msl_entry, MetalSwigluOai_metal_swiglu_oai_msl_fastmath, ok)
    g_pso_moe_gemv_mx4 = compile_pso(metal_moe_gemv_mx4_msl, metal_moe_gemv_mx4_msl_entry, metal_moe_gemv_mx4_msl_fastmath, ok)
    g_pso_moe_gemv_q51 = compile_pso(metal_moe_gemv_q51_msl, metal_moe_gemv_q51_msl_entry, metal_moe_gemv_q51_msl_fastmath, ok)
    g_pso_dn_conv = compile_pso(metal_dn_conv_msl, metal_dn_conv_msl_entry, metal_dn_conv_msl_fastmath, ok)
    g_pso_dn_hist = compile_pso(metal_dn_conv_hist_msl, metal_dn_conv_hist_msl_entry, metal_dn_conv_hist_msl_fastmath, ok)
    g_pso_dn_l2 = compile_pso(metal_dn_l2norm_msl, metal_dn_l2norm_msl_entry, metal_dn_l2norm_msl_fastmath, ok)
    g_pso_dn_scan = compile_pso(metal_dn_scan_msl, metal_dn_scan_msl_entry, metal_dn_scan_msl_fastmath, ok)
    g_pso_dn_gate = compile_pso(metal_dn_gate_msl, metal_dn_gate_msl_entry, metal_dn_gate_msl_fastmath, ok)
    g_pso_dn_deint = compile_pso(metal_dn_deint_msl, metal_dn_deint_msl_entry, metal_dn_deint_msl_fastmath, ok)
    g_pso_sigmul = compile_pso(MetalSigmul_metal_ew2_msl, MetalSigmul_metal_ew2_msl_entry, MetalSigmul_metal_ew2_msl_fastmath, ok)
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
    g_pso_qkvrs16 = compile_pso(MetalQ8GemvQkvRsF16_metal_q8_gemv_qkv_rs_msl, MetalQ8GemvQkvRsF16_metal_q8_gemv_qkv_rs_msl_entry, MetalQ8GemvQkvRsF16_metal_q8_gemv_qkv_rs_msl_fastmath, ok)
    g_pso_qkvrs32 = compile_pso(MetalQ8GemvQkvRsF32_metal_q8_gemv_qkv_rs_msl, MetalQ8GemvQkvRsF32_metal_q8_gemv_qkv_rs_msl_entry, MetalQ8GemvQkvRsF32_metal_q8_gemv_qkv_rs_msl_fastmath, ok)
    g_pso_w13sw = compile_pso(metal_q8_gemv_w13sw16_msl, metal_q8_gemv_w13sw16_msl_entry, metal_q8_gemv_w13sw16_msl_fastmath, ok)
    g_pso_kq_k4 = compile_pso(metal_kq_gemv_k4_msl, metal_kq_gemv_k4_msl_entry, metal_kq_gemv_k4_msl_fastmath, ok)
    g_pso_kq_k5 = compile_pso(metal_kq_gemv_k5_msl, metal_kq_gemv_k5_msl_entry, metal_kq_gemv_k5_msl_fastmath, ok)
    g_pso_kq_k5c = compile_pso(metal_kq_gemv_k5c_msl, metal_kq_gemv_k5c_msl_entry, metal_kq_gemv_k5c_msl_fastmath, ok)
    g_pso_kq_k6 = compile_pso(metal_kq_gemv_k6_msl, metal_kq_gemv_k6_msl_entry, metal_kq_gemv_k6_msl_fastmath, ok)
    g_pso_kq_mvb2_k4 = compile_pso(MetalKqMvB2K4_metal_kq_mv_k4_msl, MetalKqMvB2K4_metal_kq_mv_k4_msl_entry, MetalKqMvB2K4_metal_kq_mv_k4_msl_fastmath, ok)
    g_pso_kq_mvb4_k4 = compile_pso(MetalKqMvB4K4_metal_kq_mv_k4_msl, MetalKqMvB4K4_metal_kq_mv_k4_msl_entry, MetalKqMvB4K4_metal_kq_mv_k4_msl_fastmath, ok)
    g_pso_kq_mvb2_k5 = compile_pso(MetalKqMvB2K5_metal_kq_mv_k5_msl, MetalKqMvB2K5_metal_kq_mv_k5_msl_entry, MetalKqMvB2K5_metal_kq_mv_k5_msl_fastmath, ok)
    g_pso_kq_mvb4_k5 = compile_pso(MetalKqMvB4K5_metal_kq_mv_k5_msl, MetalKqMvB4K5_metal_kq_mv_k5_msl_entry, MetalKqMvB4K5_metal_kq_mv_k5_msl_fastmath, ok)
    g_pso_kq_mvb2_k6 = compile_pso(MetalKqMvB2K6_metal_kq_mv_k6_msl, MetalKqMvB2K6_metal_kq_mv_k6_msl_entry, MetalKqMvB2K6_metal_kq_mv_k6_msl_fastmath, ok)
    g_pso_kq_mvb4_k6 = compile_pso(MetalKqMvB4K6_metal_kq_mv_k6_msl, MetalKqMvB4K6_metal_kq_mv_k6_msl_entry, MetalKqMvB4K6_metal_kq_mv_k6_msl_fastmath, ok)
    g_pso_kq_mvb8_k4 = compile_pso(MetalKqMvB8K4_metal_kq_mvb8_msl, MetalKqMvB8K4_metal_kq_mvb8_msl_entry, MetalKqMvB8K4_metal_kq_mvb8_msl_fastmath, ok)
    g_pso_kq_mvb8_k5 = compile_pso(MetalKqMvB8K5_metal_kq_mvb8_msl, MetalKqMvB8K5_metal_kq_mvb8_msl_entry, MetalKqMvB8K5_metal_kq_mvb8_msl_fastmath, ok)
    g_pso_kq_mvb8_k6 = compile_pso(metal_kq_mvb8_k6_msl, metal_kq_mvb8_k6_msl_entry, metal_kq_mvb8_k6_msl_fastmath, ok)
    g_pso_kq_mm_b4 = compile_pso(MetalKqMulMmK4_metal_kq_mulmm_msl, MetalKqMulMmK4_metal_kq_mulmm_msl_entry, MetalKqMulMmK4_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_mm_b5 = compile_pso(MetalKqMulMmK5_metal_kq_mulmm_msl, MetalKqMulMmK5_metal_kq_mulmm_msl_entry, MetalKqMulMmK5_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_mm_b6 = compile_pso(MetalKqMulMmK6_metal_kq_mulmm_msl, MetalKqMulMmK6_metal_kq_mulmm_msl_entry, MetalKqMulMmK6_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_iq4xs = compile_pso(metal_kq_gemv_iq4xs_msl, metal_kq_gemv_iq4xs_msl_entry, metal_kq_gemv_iq4xs_msl_fastmath, ok)
    g_pso_kq_mvb2_iq4xs = compile_pso(MetalKqMvB2Iq4xs_metal_kq_mv_iq4xs_msl, MetalKqMvB2Iq4xs_metal_kq_mv_iq4xs_msl_entry, MetalKqMvB2Iq4xs_metal_kq_mv_iq4xs_msl_fastmath, ok)
    g_pso_kq_mvb4_iq4xs = compile_pso(MetalKqMvB4Iq4xs_metal_kq_mv_iq4xs_msl, MetalKqMvB4Iq4xs_metal_kq_mv_iq4xs_msl_entry, MetalKqMvB4Iq4xs_metal_kq_mv_iq4xs_msl_fastmath, ok)
    g_pso_kq_mvb8_iq4xs = compile_pso(metal_kq_mvb8_iq4xs_msl, metal_kq_mvb8_iq4xs_msl_entry, metal_kq_mvb8_iq4xs_msl_fastmath, ok)
    g_pso_kq_mm_b44 = compile_pso(MetalKqMulMmIq4xs_metal_kq_mulmm_msl, MetalKqMulMmIq4xs_metal_kq_mulmm_msl_entry, MetalKqMulMmIq4xs_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_k3 = compile_pso(metal_kq_gemv_k3_msl, metal_kq_gemv_k3_msl_entry, metal_kq_gemv_k3_msl_fastmath, ok)
    g_pso_kq_mvb2_k3 = compile_pso(MetalKqMvB2K3_metal_kq_mv_k3_msl, MetalKqMvB2K3_metal_kq_mv_k3_msl_entry, MetalKqMvB2K3_metal_kq_mv_k3_msl_fastmath, ok)
    g_pso_kq_mvb4_k3 = compile_pso(MetalKqMvB4K3_metal_kq_mv_k3_msl, MetalKqMvB4K3_metal_kq_mv_k3_msl_entry, MetalKqMvB4K3_metal_kq_mv_k3_msl_fastmath, ok)
    g_pso_kq_mvb8_k3 = compile_pso(metal_kq_mvb8_k3_msl, metal_kq_mvb8_k3_msl_entry, metal_kq_mvb8_k3_msl_fastmath, ok)
    g_pso_kq_mm_b3 = compile_pso(MetalKqMulMmK3_metal_kq_mulmm_msl, MetalKqMulMmK3_metal_kq_mulmm_msl_entry, MetalKqMulMmK3_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_iq3s = compile_pso(metal_kq_gemv_iq3s_msl, metal_kq_gemv_iq3s_msl_entry, metal_kq_gemv_iq3s_msl_fastmath, ok)
    g_pso_kq_mvb2_iq3s = compile_pso(MetalKqMvB2Iq3s_metal_kq_mv_iq3s_msl, MetalKqMvB2Iq3s_metal_kq_mv_iq3s_msl_entry, MetalKqMvB2Iq3s_metal_kq_mv_iq3s_msl_fastmath, ok)
    g_pso_kq_mvb4_iq3s = compile_pso(MetalKqMvB4Iq3s_metal_kq_mv_iq3s_msl, MetalKqMvB4Iq3s_metal_kq_mv_iq3s_msl_entry, MetalKqMvB4Iq3s_metal_kq_mv_iq3s_msl_fastmath, ok)
    g_pso_kq_mvb8_iq3s = compile_pso(metal_kq_mvb8_iq3s_msl, metal_kq_mvb8_iq3s_msl_entry, metal_kq_mvb8_iq3s_msl_fastmath, ok)
    g_pso_kq_mm_b33 = compile_pso(MetalKqMulMmIq3s_metal_kq_mulmm_msl, MetalKqMulMmIq3s_metal_kq_mulmm_msl_entry, MetalKqMulMmIq3s_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_iq3xxs = compile_pso(metal_kq_gemv_iq3xxs_msl, metal_kq_gemv_iq3xxs_msl_entry, metal_kq_gemv_iq3xxs_msl_fastmath, ok)
    g_pso_kq_mvb2_iq3xxs = compile_pso(MetalKqMvB2Iq3xxs_metal_kq_mv_iq3xxs_msl, MetalKqMvB2Iq3xxs_metal_kq_mv_iq3xxs_msl_entry, MetalKqMvB2Iq3xxs_metal_kq_mv_iq3xxs_msl_fastmath, ok)
    g_pso_kq_mvb4_iq3xxs = compile_pso(MetalKqMvB4Iq3xxs_metal_kq_mv_iq3xxs_msl, MetalKqMvB4Iq3xxs_metal_kq_mv_iq3xxs_msl_entry, MetalKqMvB4Iq3xxs_metal_kq_mv_iq3xxs_msl_fastmath, ok)
    g_pso_kq_mvb8_iq3xxs = compile_pso(metal_kq_mvb8_iq3xxs_msl, metal_kq_mvb8_iq3xxs_msl_entry, metal_kq_mvb8_iq3xxs_msl_fastmath, ok)
    g_pso_kq_mm_b34 = compile_pso(MetalKqMulMmIq3xxs_metal_kq_mulmm_msl, MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_entry, MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_iq4nl = compile_pso(metal_kq_gemv_iq4nl_msl, metal_kq_gemv_iq4nl_msl_entry, metal_kq_gemv_iq4nl_msl_fastmath, ok)
    g_pso_kq_mvb2_iq4nl = compile_pso(MetalKqMvB2Iq4nl_metal_kq_mv_iq4nl_msl, MetalKqMvB2Iq4nl_metal_kq_mv_iq4nl_msl_entry, MetalKqMvB2Iq4nl_metal_kq_mv_iq4nl_msl_fastmath, ok)
    g_pso_kq_mvb4_iq4nl = compile_pso(MetalKqMvB4Iq4nl_metal_kq_mv_iq4nl_msl, MetalKqMvB4Iq4nl_metal_kq_mv_iq4nl_msl_entry, MetalKqMvB4Iq4nl_metal_kq_mv_iq4nl_msl_fastmath, ok)
    g_pso_kq_mvb8_iq4nl = compile_pso(metal_kq_mvb8_iq4nl_msl, metal_kq_mvb8_iq4nl_msl_entry, metal_kq_mvb8_iq4nl_msl_fastmath, ok)
    g_pso_kq_iq2s = compile_pso(metal_kq_gemv_iq2s_msl, metal_kq_gemv_iq2s_msl_entry, metal_kq_gemv_iq2s_msl_fastmath, ok)
    g_pso_kq_mvb2_iq2s = compile_pso(MetalKqMvB2Iq2s_metal_kq_mv_iq2s_msl, MetalKqMvB2Iq2s_metal_kq_mv_iq2s_msl_entry, MetalKqMvB2Iq2s_metal_kq_mv_iq2s_msl_fastmath, ok)
    g_pso_kq_mvb4_iq2s = compile_pso(MetalKqMvB4Iq2s_metal_kq_mv_iq2s_msl, MetalKqMvB4Iq2s_metal_kq_mv_iq2s_msl_entry, MetalKqMvB4Iq2s_metal_kq_mv_iq2s_msl_fastmath, ok)
    g_pso_kq_mvb8_iq2s = compile_pso(metal_kq_mvb8_iq2s_msl, metal_kq_mvb8_iq2s_msl_entry, metal_kq_mvb8_iq2s_msl_fastmath, ok)
    g_pso_kq_mm_b23 = compile_pso(MetalKqMulMmIq2s_metal_kq_mulmm_msl, MetalKqMulMmIq2s_metal_kq_mulmm_msl_entry, MetalKqMulMmIq2s_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_iq2xs = compile_pso(metal_kq_gemv_iq2xs_msl, metal_kq_gemv_iq2xs_msl_entry, metal_kq_gemv_iq2xs_msl_fastmath, ok)
    g_pso_kq_mvb2_iq2xs = compile_pso(MetalKqMvB2Iq2xs_metal_kq_mv_iq2xs_msl, MetalKqMvB2Iq2xs_metal_kq_mv_iq2xs_msl_entry, MetalKqMvB2Iq2xs_metal_kq_mv_iq2xs_msl_fastmath, ok)
    g_pso_kq_mvb4_iq2xs = compile_pso(MetalKqMvB4Iq2xs_metal_kq_mv_iq2xs_msl, MetalKqMvB4Iq2xs_metal_kq_mv_iq2xs_msl_entry, MetalKqMvB4Iq2xs_metal_kq_mv_iq2xs_msl_fastmath, ok)
    g_pso_kq_mvb8_iq2xs = compile_pso(metal_kq_mvb8_iq2xs_msl, metal_kq_mvb8_iq2xs_msl_entry, metal_kq_mvb8_iq2xs_msl_fastmath, ok)
    g_pso_kq_mm_b24 = compile_pso(MetalKqMulMmIq2xs_metal_kq_mulmm_msl, MetalKqMulMmIq2xs_metal_kq_mulmm_msl_entry, MetalKqMulMmIq2xs_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_iq2xxs = compile_pso(metal_kq_gemv_iq2xxs_msl, metal_kq_gemv_iq2xxs_msl_entry, metal_kq_gemv_iq2xxs_msl_fastmath, ok)
    g_pso_kq_mvb2_iq2xxs = compile_pso(MetalKqMvB2Iq2xxs_metal_kq_mv_iq2xxs_msl, MetalKqMvB2Iq2xxs_metal_kq_mv_iq2xxs_msl_entry, MetalKqMvB2Iq2xxs_metal_kq_mv_iq2xxs_msl_fastmath, ok)
    g_pso_kq_mvb4_iq2xxs = compile_pso(MetalKqMvB4Iq2xxs_metal_kq_mv_iq2xxs_msl, MetalKqMvB4Iq2xxs_metal_kq_mv_iq2xxs_msl_entry, MetalKqMvB4Iq2xxs_metal_kq_mv_iq2xxs_msl_fastmath, ok)
    g_pso_kq_mvb8_iq2xxs = compile_pso(metal_kq_mvb8_iq2xxs_msl, metal_kq_mvb8_iq2xxs_msl_entry, metal_kq_mvb8_iq2xxs_msl_fastmath, ok)
    g_pso_kq_mm_b25 = compile_pso(MetalKqMulMmIq2xxs_metal_kq_mulmm_msl, MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_entry, MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_mm_b45 = compile_pso(MetalKqMulMmIq4nl_metal_kq_mulmm_msl, MetalKqMulMmIq4nl_metal_kq_mulmm_msl_entry, MetalKqMulMmIq4nl_metal_kq_mulmm_msl_fastmath, ok)
    g_pso_kq_k2 = compile_pso(metal_kq_gemv_k2_msl, metal_kq_gemv_k2_msl_entry, metal_kq_gemv_k2_msl_fastmath, ok)
    g_pso_kq_mvb2_k2 = compile_pso(MetalKqMvB2K2_metal_kq_mv_k2_msl, MetalKqMvB2K2_metal_kq_mv_k2_msl_entry, MetalKqMvB2K2_metal_kq_mv_k2_msl_fastmath, ok)
    g_pso_kq_mvb4_k2 = compile_pso(MetalKqMvB4K2_metal_kq_mv_k2_msl, MetalKqMvB4K2_metal_kq_mv_k2_msl_entry, MetalKqMvB4K2_metal_kq_mv_k2_msl_fastmath, ok)
    g_pso_kq_mvb8_k2 = compile_pso(metal_kq_mvb8_k2_msl, metal_kq_mvb8_k2_msl_entry, metal_kq_mvb8_k2_msl_fastmath, ok)
    g_pso_kq_mm_b2 = compile_pso(MetalKqMulMmK2_metal_kq_mulmm_msl, MetalKqMulMmK2_metal_kq_mulmm_msl_entry, MetalKqMulMmK2_metal_kq_mulmm_msl_fastmath, ok)
    // the batched-step set (P4)
    g_pso_gemv_b2 = compile_pso(MetalGemvB2_metal_gemv_msl, MetalGemvB2_metal_gemv_msl_entry, MetalGemvB2_metal_gemv_msl_fastmath, ok)
    g_pso_gemv_b4 = compile_pso(MetalGemvB4_metal_gemv_msl, MetalGemvB4_metal_gemv_msl_entry, MetalGemvB4_metal_gemv_msl_fastmath, ok)
    g_pso_mv_b2 = compile_pso(metal_q8_mv_b2_msl, metal_q8_mv_b2_msl_entry, metal_q8_mv_b2_msl_fastmath, ok)
    g_pso_mv_b4 = compile_pso(metal_q8_mv_b4_msl, metal_q8_mv_b4_msl_entry, metal_q8_mv_b4_msl_fastmath, ok)
    g_pso_w13sw_b2 = compile_pso(MetalGemvW13SwB2_metal_gemv_w13sw_msl, MetalGemvW13SwB2_metal_gemv_w13sw_msl_entry, MetalGemvW13SwB2_metal_gemv_w13sw_msl_fastmath, ok)
    g_pso_w13sw_b4 = compile_pso(MetalGemvW13SwB4_metal_gemv_w13sw_msl, MetalGemvW13SwB4_metal_gemv_w13sw_msl_entry, MetalGemvW13SwB4_metal_gemv_w13sw_msl_fastmath, ok)
    g_gemm_b_tensor = metal_tensor_crowned("gemmb_q8")
    g_pso_gemm_b = (g_gemm_b_tensor
        ? compile_pso(MetalQ8GemmBT_metal_q8_gemm_b_t_msl, MetalQ8GemmBT_metal_q8_gemm_b_t_msl_entry, MetalQ8GemmBT_metal_q8_gemm_b_t_msl_fastmath, ok)
        : compile_pso(MetalQ8GemmB_metal_q8_gemm_b_msl, MetalQ8GemmB_metal_q8_gemm_b_msl_entry, MetalQ8GemmB_metal_q8_gemm_b_msl_fastmath, ok))
    g_gemm_b_sk_tensor = metal_tensor_crowned("gemmb_sk_q8")
    g_pso_gemm_b_sk = (g_gemm_b_sk_tensor
        ? compile_pso(MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl, MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_entry, MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_fastmath, ok)
        : compile_pso(MetalQ8GemmBSk_metal_q8_gemm_b_msl, MetalQ8GemmBSk_metal_q8_gemm_b_msl_entry, MetalQ8GemmBSk_metal_q8_gemm_b_msl_fastmath, ok))
    g_pso_skred = compile_pso(metal_sk_reduce_msl, metal_sk_reduce_msl_entry, metal_sk_reduce_msl_fastmath, ok)
    g_gemm64_b_tensor = metal_tensor_crowned("gemm64b_q8")
    g_pso_gemm64_b = (g_gemm64_b_tensor
        ? compile_pso(MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl, MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_entry, MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_fastmath, ok)
        : compile_pso(metal_q8_gemm64_b_msl, metal_q8_gemm64_b_msl_entry, metal_q8_gemm64_b_msl_fastmath, ok))
    g_pso_mm = compile_pso(metal_q8_mulmm_msl, metal_q8_mulmm_msl_entry, metal_q8_mulmm_msl_fastmath, ok)
    g_pso_attnb16 = compile_pso(MetalSqAttnBF16_metal_sq_attn_b_msl, MetalSqAttnBF16_metal_sq_attn_b_msl_entry, MetalSqAttnBF16_metal_sq_attn_b_msl_fastmath, ok)
    g_pso_attnb32 = compile_pso(MetalSqAttnBF32_metal_sq_attn_b_msl, MetalSqAttnBF32_metal_sq_attn_b_msl_entry, MetalSqAttnBF32_metal_sq_attn_b_msl_fastmath, ok)
    g_pso_attnb_q8 = compile_pso(MetalSqAttnBQ8_metal_sq_attn_b_msl, MetalSqAttnBQ8_metal_sq_attn_b_msl_entry, MetalSqAttnBQ8_metal_sq_attn_b_msl_fastmath, ok)
    g_pso_attnb_tq4 = compile_pso(MetalSqAttnBTq4_metal_sq_attn_b_msl, MetalSqAttnBTq4_metal_sq_attn_b_msl_entry, MetalSqAttnBTq4_metal_sq_attn_b_msl_fastmath, ok)
    g_pso_attnpb16 = compile_pso(MetalSqAttnPartBF16_metal_sq_attn_part_b_msl, MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_entry, MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_fastmath, ok)
    g_pso_attnpb32 = compile_pso(MetalSqAttnPartBF32_metal_sq_attn_part_b_msl, MetalSqAttnPartBF32_metal_sq_attn_part_b_msl_entry, MetalSqAttnPartBF32_metal_sq_attn_part_b_msl_fastmath, ok)
    g_pso_attnpb_q8 = compile_pso(MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl, MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl_entry, MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl_fastmath, ok)
    g_pso_attnpb_tq4 = compile_pso(MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl, MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl_entry, MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl_fastmath, ok)
    g_pso_attncombb = compile_pso(MetalSqAttnCombB_metal_sq_attn_comb_msl, MetalSqAttnCombB_metal_sq_attn_comb_msl_entry, MetalSqAttnCombB_metal_sq_attn_comb_msl_fastmath, ok)
    g_pso_attnd16 = compile_pso(MetalSqAttnDF16_metal_sq_attn_d_msl, MetalSqAttnDF16_metal_sq_attn_d_msl_entry, MetalSqAttnDF16_metal_sq_attn_d_msl_fastmath, ok)
    g_pso_attnd32 = compile_pso(MetalSqAttnDF32_metal_sq_attn_d_msl, MetalSqAttnDF32_metal_sq_attn_d_msl_entry, MetalSqAttnDF32_metal_sq_attn_d_msl_fastmath, ok)
    g_pso_attnd_q8 = compile_pso(MetalSqAttnDQ8_metal_sq_attn_d_msl, MetalSqAttnDQ8_metal_sq_attn_d_msl_entry, MetalSqAttnDQ8_metal_sq_attn_d_msl_fastmath, ok)
    g_pso_attnd_tq4 = compile_pso(MetalSqAttnDTq4_metal_sq_attn_d_msl, MetalSqAttnDTq4_metal_sq_attn_d_msl_entry, MetalSqAttnDTq4_metal_sq_attn_d_msl_fastmath, ok)
    g_pso_attncombd = compile_pso(metal_sq_attn_comb_d_msl, metal_sq_attn_comb_d_msl_entry, metal_sq_attn_comb_d_msl_fastmath, ok)
    g_pso_rpstb16 = compile_pso(MetalRopeStoreBF16_metal_rope_store_b_msl, MetalRopeStoreBF16_metal_rope_store_b_msl_entry, MetalRopeStoreBF16_metal_rope_store_b_msl_fastmath, ok)
    g_pso_rpstb32 = compile_pso(MetalRopeStoreBF32_metal_rope_store_b_msl, MetalRopeStoreBF32_metal_rope_store_b_msl_entry, MetalRopeStoreBF32_metal_rope_store_b_msl_fastmath, ok)
    g_pso_rpstb_q8 = compile_pso(metal_rope_store_b_q8_msl, metal_rope_store_b_q8_msl_entry, metal_rope_store_b_q8_msl_fastmath, ok)
    g_pso_rpstb_tq4 = compile_pso(metal_rope_store_b_tq4_msl, metal_rope_store_b_tq4_msl_entry, metal_rope_store_b_tq4_msl_fastmath, ok)
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

def enc_gemv(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64; bx, by, bn, bd : MetalBuffer?; d : int64; yoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    enc_gemv_c(enc, bw, boff, bw, bx, xoff, by, yoff, bn, bd, d, 1l)
}

// K-quant site GEMV — the per-format twin of enc_gemv over the kq plane pair
// (kq_upload_quants + kq_upload_scales). k6 binds the scale buffer twice: the trailing packed
// f16 d plane at byte nsb*16, the 16B sub-scale blocks at 0. Geometry: 64-thread tg, 4 rows.
def enc_kq_gemv(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
                bs : MetalBuffer?; soff, doff : uint64; bx, by, bn, bd : MetalBuffer?;
                rows, n : int64; yoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    if (fmt == KqFmt.iq4xs) {
        enc_kq_iq4xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.k3) {
        enc_kq_k3_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq3s) {
        enc_kq_iq3s_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq3xxs) {
        enc_kq_iq3xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq4nl) {
        enc_kq_iq4nl_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.k2) {
        enc_kq_k2_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq2s) {
        enc_kq_iq2s_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq2xs) {
        enc_kq_iq2xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq2xxs) {
        enc_kq_iq2xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.k6) {
        enc_kq_k6_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.k5 && (n < 3072l || rows <= 1024l)) {
        // the select-form nr0=1 twin: wins every d at n < 3072 (gemv lab, 4B round) and the
        // small-d kv shape (b2r under-occupied); 2 rows/tg instead of b2r's 4
        enc_kq_k5c_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.k4) {
        enc_kq_k4_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } else {
        enc_kq_k5_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    }
}

// K-quant small-batch mv (B = 2..8): R=4 column groups via grid.y, R=2 via the B2 PSO; B = 5..8
// rides the single-pass B8 twin (requires the mp == 8 pad the driver guarantees at nlive >= 5;
// DASLLAMA_METAL_KQ_B8=0 = the A/B rail).
var private g_kq_b8_mode = -1

[cold_path]   // one-time env resolution; kq_b8_on is a global test after the first call
def private load_kq_b8_mode() {
    g_kq_b8_mode = g_env_metal.kq_b8 ? 1 : 0
}

def private kq_b8_on : bool {
    if (g_kq_b8_mode < 0) {
        load_kq_b8_mode()
    }
    return g_kq_b8_mode == 1
}

def enc_kq_mvb(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;   // nolint:STYLE037 — the flat per-format ladder (one arm per kq format) is the honest shape
               bs : MetalBuffer?; soff, doff : uint64; bx, by : MetalBuffer?; yoff : uint64;
               var ka : KqMvArgs; rows, nlive : int64) {
    let two = nlive == 2l
    let b8 = nlive >= 5l && kq_b8_on()
    let gcols = (nlive + 3l) / 4l
    if (fmt == KqFmt.iq4xs) {
        if (b8) {
            enc_kq_mvb8_iq4xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_iq4xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_iq4xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.k3) {
        if (b8) {
            enc_kq_mvb8_k3_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_k3_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_k3_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.iq3s) {
        if (b8) {
            enc_kq_mvb8_iq3s_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_iq3s_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_iq3s_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.iq3xxs) {
        if (b8) {
            enc_kq_mvb8_iq3xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_iq3xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_iq3xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.iq4nl) {
        if (b8) {
            enc_kq_mvb8_iq4nl_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_iq4nl_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_iq4nl_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.k2) {
        if (b8) {
            enc_kq_mvb8_k2_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_k2_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_k2_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.iq2s) {
        if (b8) {
            enc_kq_mvb8_iq2s_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_iq2s_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_iq2s_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.iq2xs) {
        if (b8) {
            enc_kq_mvb8_iq2xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_iq2xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_iq2xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.iq2xxs) {
        if (b8) {
            enc_kq_mvb8_iq2xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_iq2xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_iq2xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.k6) {
        if (b8) {
            enc_kq_mvb8_k6_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_k6_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_k6_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } elif (fmt == KqFmt.k5) {
        if (b8) {
            enc_kq_mvb8_k5_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_k5_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_k5_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    } else {
        if (b8) {
            enc_kq_mvb8_k4_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } elif (two) {
            enc_kq_mvb2_k4_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows)
        } else {
            enc_kq_mvb4_k4_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, ka, rows, gcols)
        }
    }
}

// the batched kq GEMM (B >= 9): the prefill-owned kq mul_mm twins over the padded x panel —
// enc_gemm_mm_b's shape (grid mp/32 x rows/64, y bound at the site's column byte offset)
def enc_kq_gemm_mm_b(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
                     bs : MetalBuffer?; soff, doff : uint64; bx, by : MetalBuffer?; yoff : uint64;
                     bk, bn : MetalBuffer?; mp, rows : int64) {
    if (fmt == KqFmt.iq4xs) {
        enc_kq_mm_iq4xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.k3) {
        enc_kq_mm_k3_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.iq3s) {
        enc_kq_mm_iq3s_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.iq3xxs) {
        enc_kq_mm_iq3xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.iq4nl) {
        enc_kq_mm_iq4nl_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.k2) {
        enc_kq_mm_k2_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.iq2s) {
        enc_kq_mm_iq2s_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.iq2xs) {
        enc_kq_mm_iq2xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.iq2xxs) {
        enc_kq_mm_iq2xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.k6) {
        enc_kq_mm_k6_c(enc, bs, doff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } elif (fmt == KqFmt.k4) {
        enc_kq_mm_k4_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    } else {
        enc_kq_mm_k5_c(enc, bs, soff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
    }
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
// kq GEMV is ALU-bound, so this matches the upstream kq batch wall). B >= 9: the kq mul_mm twin over
// the M-padded x panel. Disjoint y rows/columns: the caller's one barrier covers the whole group.
def enc_kq_site_b(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64;
                  bx : MetalBuffer?; xstride : int64; by : MetalBuffer?; ystride, ycol : int64;
                  bn, bd, bys : MetalBuffer?; nrows, mp : int64) {
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    if (nrows > 8l) {
        enc_kq_gemm_mm_b(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, uint64(ycol * 4l), bn, bys, mp, rows)
        return
    }
    // B = 2..8: the ext small-batch twins — one weight pass per R-column group instead of B per-stream passes (-25..-40% at the 3B sites)
    if (nrows >= 2l) {
        assert(xstride == n)
        var ka = KqMvArgs(ndim = uint(n), ddim = uint(rows), ys = uint(ystride), nr = uint(nrows))
        enc_kq_mvb(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, uint64(ycol * 4l), ka, rows, nrows)
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
                   bcos, bsin, bbias : MetalBuffer?; var ka : RopeStoreArgs; npairs : int64) {
    if (f16) {
        enc_rpst16_c(enc, bqkv, bqkv, vsoff, kmb, koff, vmb, voff, bcos, bsin, bbias, ka, npairs)
    } else {
        enc_rpst32_c(enc, bqkv, bqkv, vsoff, kmb, koff, vmb, voff, bcos, bsin, bbias, ka, npairs)
    }
}

// the head-cooperative fused rope-store (QK-norm models, f16 mirror): grid = nheads + 2*nkvh
// threadgroups of head_size threads
def enc_rope_store_h16(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                       kmb, vmb : MetalBuffer?; koff, voff : uint64;
                       bcos, bsin, bqkw, bbias : MetalBuffer?; var ka : RopeStoreHArgs;
                       ntg, head_size : int64) {
    enc_rpst_h16_c(enc, bqkv, bqkv, vsoff, kmb, koff, vmb, voff, bcos, bsin, bqkw, bbias, ka, ntg, head_size)
}

// The tq4 rotation signs, uploaded once at max head size (smaller heads read a prefix — the
// deterministic-seed contract with the CPU codec).

def enc_rope_store_tq4(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                       kmb, vmb : MetalBuffer?; koff, voff : uint64;
                       bcos, bsin, bbias : MetalBuffer?; var ka : RopeStoreTq4Args; ntg, head_size : int64) {
    enc_rpst_tq4_c(enc, bqkv, bqkv, vsoff, kmb, koff, kmb, vmb, voff, vmb, bcos, bsin, signs_buffer(),
        bbias, ka, ntg, head_size)
}

// the tq4 attention-output un-rotate: one threadgroup per (head, row), row stride = qstride
def enc_fwht_unsign(enc : MetalComputeEncoder?; byo, bhs, bqs : MetalBuffer?; nheads, nrows, head_size : int64) {
    enc_unsign_c(enc, byo, signs_buffer(), bhs, bqs, nheads, nrows, head_size)
}

// q8_0-mirror rope+store: the K/V mirror rows bind TWICE each (half-scale + byte views at the
// same row offset); grid covers the q pairs + one thread per K/V 32-block
def enc_rope_store_q8(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                      kmb, vmb : MetalBuffer?; koff, voff : uint64;
                      bcos, bsin, bbias : MetalBuffer?; var ka : RopeStoreQ8Args; nthreads : int64) {
    enc_rpst_q8_c(enc, bqkv, bqkv, vsoff, kmb, koff, kmb, vmb, voff, vmb, bcos, bsin,
        bbias, ka, nthreads)
}

// the fused QKV+rope+store dispatch (one 128-thread threadgroup per rope pair — grid covers ddim/2 pairs)
def enc_gemv_qkv_rs(enc : MetalComputeEncoder?; f16 : bool; bw : MetalBuffer?; wqoff, wkoff, wvoff : uint64;
                    bx, bq : MetalBuffer?; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                    bcos, bsin, bbias : MetalBuffer?; var ka : Q8QkvRsArgs; d : int64) {
    let pairs = d / 2l
    if (f16) {
        enc_qkvrs16_c(enc, bw, wqoff, bw, bx, bq, kmb, koff, vmb, voff, bcos, bsin,
            bbias, bw, wkoff, bw, bw, wvoff, bw, ka, pairs)
    } else {
        enc_qkvrs32_c(enc, bw, wqoff, bw, bx, bq, kmb, koff, vmb, voff, bcos, bsin,
            bbias, bw, wkoff, bw, bw, wvoff, bw, ka, pairs)
    }
}

// the fused W1|W3 + silu/gelu dispatch (one 128-thread threadgroup per output element — gate + up rows; bact picks the activation)
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
                 bx, by, bsel : MetalBuffer?; var ka : MoeGemvArgs; yoff : uint64 = 0ul) {
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, stack_off)
        enc_moe_gemv_q8_c(enc, bw.buf, bw.boff, bw.buf, bx, by, yoff, ka, bsel, rows, k, nst)
    } elif (fmt == KqFmt.q51) {
        let qp = q51_planes_of(g_dev, t, stack_off)
        enc_moe_gemv_q51_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, by, yoff, ka, bsel, rows, k, nst)
    } else {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        if (fmt == KqFmt.k6) {
            enc_moe_gemv_k6_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.k4) {
            enc_moe_gemv_k4_c(enc, bs.buf, bs.soff, bs.buf, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } else {
            enc_moe_gemv_k5_c(enc, bs.buf, bs.soff, bs.buf, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        }
    }
}

// the MXFP4 expert site (gpt-oss): the mx4 planes bind at the layer stack's byte offsets;
// bwb = the layer's per-expert bias slab (every caller uploads a real slab; ka.hasb gates the read)
def enc_moe_gemv_mx4(enc : MetalComputeEncoder?; t : Model; stack_off, rows, k, nst : int64;
                     bx, by, bsel, bwb : MetalBuffer?; var ka : MoeGemvArgs; yoff : uint64 = 0ul) {
    let mp = mx4_of(g_dev, t, stack_off)
    enc_moe_gemv_mx4_c(enc, mp.qbuf, mp.qoff, mp.sbuf, mp.soff, bx, by, yoff, ka, bsel, bwb, g_mx4_vtab, rows, k, nst)
}

// fused add + rms when dim fits the kernel's row slab, the unfused pair otherwise
def enc_add_rms_site(enc : MetalComputeEncoder?; bx, ba, bw, by, bdim, beps : MetalBuffer?; dim : int64; bascale : MetalBuffer? = null) {
    if (dim <= ADD_RMS_MAX_DIM) {
        enc_addrms_c(enc, bx, ba, bw, by, bdim, beps, bascale != null ? bascale : g_one)
    } else {
        enc_add(enc, bx, 0ul, ba, 0ul, bdim, dim, bascale)
        enc_rms(enc, bx, 0ul, bw, by, bdim, beps)
    }
}

// Fused post-attn-norm + residual add + ffn-norm for pre_post_norm models: a = rms(a)·wpa in place,
// x += a, y = rms(x)·wffn — one dispatch collapsing enc_rms(a) + enc_add_rms_site. Falls back to the
// separate pair past the slab cap. `ba` is normed in place (matches the un-fused enc_rms(a) write).
def enc_pre_add_rms_site(enc : MetalComputeEncoder?; bx, ba, bwpa, bwffn, by, bdim, beps : MetalBuffer?; dim : int64) {
    if (dim <= ADD_RMS_MAX_DIM) {
        enc_preaddrms_c(enc, bx, ba, bwpa, bwffn, by, bdim, beps)
    } else {
        enc_rms(enc, ba, 0ul, bwpa, ba, bdim, beps)
        enc_add_rms_site(enc, bx, ba, bwffn, by, bdim, beps, dim)
    }
}

// one fmt-dispatching encoder per attention shape — the KV codec picks the generated _c builder
// (f16/f32: one view per K/V slab; q8_0/tq4: the slabs bind TWICE each, half-scale + byte views
// at the same layer offset, and nblk replaces the kv_dim uniform)
def enc_attn(enc : MetalComputeEncoder?; kdt : KVDtype; kmb, vmb : MetalBuffer?; koff, voff : uint64;
             bq, by : MetalBuffer?; var ka : SqAttnArgs; nheads, head_size : int64; bsink : MetalBuffer? = null) {
    let bsnk = bsink != null ? bsink : g_one
    if (kdt == KVDtype.q8_0) {
        enc_attn_q8_c(enc, kmb, koff, kmb, vmb, voff, vmb, bq, by, ka, bsnk, nheads, head_size)
    } elif (kdt == KVDtype.tq4) {
        enc_attn_tq4_c(enc, kmb, koff, kmb, vmb, voff, vmb, bq, by, ka, bsnk, nheads, head_size)
    } elif (kdt == KVDtype.f16) {
        enc_attn16_c(enc, kmb, koff, vmb, voff, bq, by, ka, bsnk, nheads, head_size)
    } else {
        enc_attn32_c(enc, kmb, koff, vmb, voff, bq, by, ka, bsnk, nheads, head_size)
    }
}

def enc_attn_part(enc : MetalComputeEncoder?; kdt : KVDtype; kmb, vmb : MetalBuffer?; koff, voff : uint64;
                  bq, bpart : MetalBuffer?; var ka : SqAttnPartArgs; ntg, head_size : int64) {
    if (kdt == KVDtype.q8_0) {
        enc_attnp_q8_c(enc, kmb, koff, kmb, vmb, voff, vmb, bq, bpart, ka, ntg, head_size)
    } elif (kdt == KVDtype.tq4) {
        enc_attnp_tq4_c(enc, kmb, koff, kmb, vmb, voff, vmb, bq, bpart, ka, ntg, head_size)
    } elif (kdt == KVDtype.f16) {
        enc_attnp16_c(enc, kmb, koff, vmb, voff, bq, bpart, ka, ntg, head_size)
    } else {
        enc_attnp32_c(enc, kmb, koff, vmb, voff, bq, bpart, ka, ntg, head_size)
    }
}

def enc_attn_comb(enc : MetalComputeEncoder?; bpart, by : MetalBuffer?; var ka : SqAttnCombArgs; nheads, head_size : int64; bsink : MetalBuffer? = null) {
    enc_attncomb_c(enc, bpart, by, ka, bsink, nheads, head_size) // null sink resolves @default g_one
}

// swiglu and add share the (a, b, total) elementwise shape; offsets serve the fused-buffer binds
// the ew2 combines are per-op [metal_dispatch] builders (enc_swiglu / enc_geglu / enc_sigmul /
// enc_swiglu_oai, and enc_add_c under this g_one-defaulting wrapper) — callers name the op
def enc_add(enc : MetalComputeEncoder?; bg : MetalBuffer?; goff : uint64; bu : MetalBuffer?; uoff : uint64; btot : MetalBuffer?; total : int64; bascale : MetalBuffer? = null) {
    enc_add_c(enc, bg, goff, bu, uoff, btot, bascale != null ? bascale : g_one, total)
}

// ===== the batched-step encoders (P4) =====

// fixed-B batched GEMV (B2 serves B <= 2, B4 3..4 — pad streams read stale rows, outputs never
// consumed); the blob slice binds through both views at the tensor's byte offset. Pinned at
// 4 rows/tg — the x-staged cooperative-load slot math bakes 128 threads in.
def enc_gemv_b(enc : MetalComputeEncoder?; four : bool; bw : MetalBuffer?; boff : uint64;
               bx, by, bn, bd, bys : MetalBuffer?; d : int64; yoff : uint64 = 0ul) {
    if (four) {
        enc_gemv_b4_c(enc, bw, boff, bw, bx, by, yoff, bn, bd, bys, d)
    } else {
        enc_gemv_b2_c(enc, bw, boff, bw, bx, by, yoff, bn, bd, bys, d)
    }
}

// the mul_mv-shape fixed-B GEMV on an interleaved 34B blob (dual views of the SAME buffer);
// xoff/yoff = the column-tile byte offsets (B > 4 streams the weights once per 4-column tile)
def enc_mv_b(enc : MetalComputeEncoder?; four : bool; bwblob : MetalBuffer?; wboff : uint64;
             bx, by : MetalBuffer?; var ka : Q8MvArgs;
             xoff : uint64 = 0ul; yoff : uint64 = 0ul) {
    // b2 = 4 rows/tg (16 lanes/row, grid d/4); b4 = 8 rows/tg (8 lanes/row, grid d/8)
    let d = int64(ka.ddim)
    if (four) {
        enc_mv_b4_c(enc, bwblob, wboff, bwblob, bx, xoff, by, yoff, ka, d)
    } else {
        enc_mv_b2_c(enc, bwblob, wboff, bwblob, bx, xoff, by, yoff, ka, d)
    }
}

// one mv GEMV site: a single form dispatch at B <= 4, two 4-column tiles at B = 5..8 (the
// reference's own shape — it never switches to GEMM below B = 9; the weights stream twice but
// stay the fast skinny form). xrowb = the x row stride in bytes; the tiles differ only in nrows.
def enc_mv_site(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                bx, by : MetalBuffer?; n, xs4, d, nrows, ys : int64; xrowb : uint64;
                yoff : uint64 = 0ul) {
    var ka = Q8MvArgs(ndim = uint(n), ddim = uint(d), nrows = uint(min(nrows, 4l)),
        xs4 = uint(xs4), ys = uint(ys))
    enc_mv_b(enc, nrows > 2l, bwblob, wboff, bx, by, ka, 0ul, yoff)
    if (nrows > 4l) {
        // the rows-5..8 dispatch offsets y by 4 rows of the SITE's y row stride — fused-buffer
        // segment sites included
        ka.nrows = uint(nrows - 4l)
        enc_mv_b(enc, true, bwblob, wboff, bx, by, ka, 4ul * xrowb, yoff + uint64(4l * ys * 4l))
    }
}

// fused fixed-B W1|W3 GEMV + swiglu — the w1 and w3 blob slices bind separately (kind-major
// layout), by is the [B x hidden] swiglu output. Replaces the w1 + w3 GEMVs + the swiglu ew2
// with one dispatch. Same 4-rows/tg geometry, gridded over the hidden output rows.
def enc_gemv_w13sw_b(enc : MetalComputeEncoder?; four : bool; bw : MetalBuffer?; w1off, w3off : uint64;
                     bx, by, bn, bhid : MetalBuffer?; hidden : int64) {
    if (four) {
        enc_w13sw_b4_c(enc, bw, w1off, bw, bx, by, bn, bhid, bw, w3off, bw, hidden)
    } else {
        enc_w13sw_b2_c(enc, bw, w1off, bw, bx, by, bn, bhid, bw, w3off, bw, hidden)
    }
}

// the production mul_mm dispatch (the prefill kernel: 34B-blob dual-view W, raw f32 X): y binds
// at a byte offset so per-tensor tiles land inside a wider fused row — the ndim uniform is the
// y ROW STRIDE while this tensor's output width d shapes the grid (d % 64 == 0, mp % 32 == 0)
def enc_gemm_mm_b(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                  bx, by : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, d : int64) {
    enc_gemm_mm_c(enc, bwblob, wboff, bwblob, bx, by, yoff, bk, bn, mp, d)
}

// the M-pad-32 GEMM (B >= 5): raw f32 X, dual-view blob W at the tensor's byte offset
def enc_gemm_b(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64;
               bx, by : MetalBuffer?; var ka : Q8GemmArgs; mp32, d : int64; yoff : uint64 = 0ul) {
    let ntg = (mp32 / 32l) * (d / 32l)
    if (g_gemm_b_tensor) {
        enc_gemm_b_t_c(enc, bw, boff, bw, bx, by, yoff, ka, ntg)
    } else {
        enc_gemm_b_c(enc, bw, boff, bw, bx, by, yoff, ka, ntg)
    }
}

// the split-K pair (per-site policy): the GEMM's grid.y is the k-slice axis, partial planes land
// in bpart, and the reduce sums them into y — the serial encoder orders the two dispatches.
// Planes and grids assume mp == 32 (the policy gate).
def enc_gemm_sk_b(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64;
                  bx, bpart : MetalBuffer?; var ka : Q8GemmArgs; d, nsk : int64) {
    if (g_gemm_b_sk_tensor) {
        enc_gemm_sk_b_t_c(enc, bw, boff, bw, bx, bpart, 0ul, ka, d, nsk)
    } else {
        enc_gemm_sk_b_c(enc, bw, boff, bw, bx, bpart, 0ul, ka, d, nsk)
    }
}

def enc_sk_reduce_b(enc : MetalComputeEncoder?; bpart, by : MetalBuffer?; total, nsk : int64) {
    var ka = SkReduceArgs(total = uint(total), ksplit = uint(nsk))
    enc_sk_reduce_c(enc, bpart, by, ka, total)
}

// the 64-wide N tile (classifier site): the caller gates d % 64 == 0
def enc_gemm64_b(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64;
                 bx, by : MetalBuffer?; var ka : Q8GemmArgs; mp32, d : int64) {
    let ntg = (mp32 / 32l) * (d / 64l)
    if (g_gemm64_b_tensor) {
        enc_gemm64_b_t_c(enc, bw, boff, bw, bx, by, 0ul, ka, ntg)
    } else {
        enc_gemm64_b_c(enc, bw, boff, bw, bx, by, ka, ntg)
    }
}

// Per-encoder buffer-hazard tracker for concurrent encoding (g_bconcurrent): each scratch buffer
// owns a bit; a barrier is inserted iff a dispatch's read hits a pending write (RAW), or its
// write hits a pending write/read (WAW/WAR) — read-after-read runs concurrently.

// the batch attention family dispatches on the KV codec: f16/f32 bind the arena twice (K + V
// views), q8_0/tq4 FOUR times (half-scale + byte views for each) and shift the tail binds by 2;
// for the quant codecs the row table carries byte offsets and nblk replaces the kv_dim uniform
def enc_attn_b(enc : MetalComputeEncoder?; kdt : KVDtype; bq, by, brt : MetalBuffer?; var ka : SqAttnBArgs; nrows, nheads, head_size : int64; bsink : MetalBuffer? = null; rtoff : uint64 = 0ul) {
    let bsnk = bsink != null ? bsink : g_one
    let wgs = nheads * nrows
    if (kdt == KVDtype.q8_0) {
        enc_attnb_q8_c(enc, g_arena, g_arena, g_arena, g_arena, bq, by, brt, rtoff, ka, bsnk, wgs, head_size)
    } elif (kdt == KVDtype.tq4) {
        enc_attnb_tq4_c(enc, g_arena, g_arena, g_arena, g_arena, bq, by, brt, rtoff, ka, bsnk, wgs, head_size)
    } elif (kdt == KVDtype.f16) {
        enc_attnb16_c(enc, g_arena, g_arena, bq, by, brt, rtoff, ka, bsnk, wgs, head_size)
    } else {
        enc_attnb32_c(enc, g_arena, g_arena, bq, by, brt, rtoff, ka, bsnk, wgs, head_size)
    }
}

def enc_attn_part_b(enc : MetalComputeEncoder?; kdt : KVDtype; bq, bpart, brt : MetalBuffer?; var ka : SqAttnPartBArgs; nrows, nchmax, nheads, head_size : int64; rtoff : uint64 = 0ul) {
    let wgs = nheads * nchmax * nrows
    if (kdt == KVDtype.q8_0) {
        enc_attnpb_q8_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, rtoff, ka, wgs, head_size)
    } elif (kdt == KVDtype.tq4) {
        enc_attnpb_tq4_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, rtoff, ka, wgs, head_size)
    } elif (kdt == KVDtype.f16) {
        enc_attnpb16_c(enc, g_arena, g_arena, bq, bpart, brt, rtoff, ka, wgs, head_size)
    } else {
        enc_attnpb32_c(enc, g_arena, g_arena, bq, bpart, brt, rtoff, ka, wgs, head_size)
    }
}

// HEAD-FASTEST launch (grid.x = heads): consecutive tgs share position rows (SLC locality at depth)
def enc_attn_d(enc : MetalComputeEncoder?; kdt : KVDtype; bq, bpart, brt : MetalBuffer?; var ka : SqAttnDArgs; nsgs, nrows, nheads : int64) {
    if (kdt == KVDtype.q8_0) {
        enc_attnd_q8_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, ka, nheads, nsgs, nrows)
    } elif (kdt == KVDtype.tq4) {
        enc_attnd_tq4_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, ka, nheads, nsgs, nrows)
    } elif (kdt == KVDtype.f16) {
        enc_attnd16_c(enc, g_arena, g_arena, bq, bpart, brt, ka, nheads, nsgs, nrows)
    } else {
        enc_attnd32_c(enc, g_arena, g_arena, bq, bpart, brt, ka, nheads, nsgs, nrows)
    }
}

def enc_rope_store_b(enc : MetalComputeEncoder?; f16 : bool; bqkv, bcos, bsin, brt, bbias : MetalBuffer?; var ka : RopeStoreBArgs; npairs, nrows : int64; rtoff : uint64 = 0ul) {
    if (f16) {
        enc_rpstb16_c(enc, bqkv, g_arena, g_arena, bcos, bsin, brt, rtoff, bbias, ka, npairs, nrows)
    } else {
        enc_rpstb32_c(enc, bqkv, g_arena, g_arena, bcos, bsin, brt, rtoff, bbias, ka, npairs, nrows)
    }
}

// tq4-mirror batched rope+store: one threadgroup per (head, batch row), arena bound four ways
def enc_rope_store_b_tq4(enc : MetalComputeEncoder?; bqkv, bcos, bsin, brt, bbias : MetalBuffer?; var ka : RopeStoreBTq4Args; ntg, nrows, head_size : int64) {
    enc_rpstb_tq4_c(enc, bqkv, g_arena, g_arena, g_arena, g_arena, bcos, bsin, signs_buffer(), brt, bbias,
        ka, ntg, nrows, head_size)
}

// q8_0-mirror batched rope+store: arena bound four ways (K/V x scale/byte views); grid covers
// the q pairs + one thread per K/V 32-block, one grid row per batch row
def enc_rope_store_b_q8(enc : MetalComputeEncoder?; bqkv, bcos, bsin, brt, bbias : MetalBuffer?; var ka : RopeStoreBQ8Args; nthreads, nrows : int64) {
    enc_rpstb_q8_c(enc, bqkv, g_arena, g_arena, g_arena, g_arena, bcos, bsin, brt, bbias, ka, nthreads, nrows)
}

def enc_add_rms_b(enc : MetalComputeEncoder?; bx, ba, bw, by, bdim, beps : MetalBuffer?; nrows : int64; bascale : MetalBuffer? = null) {
    enc_add_rms_b_c(enc, bx, ba, bw, by, bdim, beps, bascale != null ? bascale : g_one, nrows)
}

def pf_enc_add_rms_bhx(enc : MetalComputeEncoder?; bx, ba, bw, by, bdim, beps, bxh : MetalBuffer?; nrows : int64; bascale : MetalBuffer? = null) {
    pf_enc_add_rms_bhx_c(enc, bx, ba, bw, by, bdim, beps, bascale != null ? bascale : g_one, bxh, nrows)
}

// enc_rms_b (the batched prologue) and pf_enc_rms (the prefill 256/tg shape) are
// [metal_dispatch]-generated from MetalRmsNorm alongside enc_rms.

//! Release every PSO the registry compiled (the PSO half of metal_decode_shutdown — runs
//! before the shared device goes).
def metal_kernels_release {   // nolint:STYLE037,STYLE038 — a flat release_pso per kernel; no seam to split on
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
    release_pso(g_pso_kq_iq4xs)
    release_pso(g_pso_kq_mvb2_iq4xs)
    release_pso(g_pso_kq_mvb4_iq4xs)
    release_pso(g_pso_kq_mvb8_iq4xs)
    release_pso(g_pso_kq_mm_b44)
    release_pso(g_pso_kq_k3)
    release_pso(g_pso_kq_mvb2_k3)
    release_pso(g_pso_kq_mvb4_k3)
    release_pso(g_pso_kq_mvb8_k3)
    release_pso(g_pso_kq_mm_b3)
    release_pso(g_pso_kq_iq3s)
    release_pso(g_pso_kq_mvb2_iq3s)
    release_pso(g_pso_kq_mvb4_iq3s)
    release_pso(g_pso_kq_mvb8_iq3s)
    release_pso(g_pso_kq_mm_b33)
    release_pso(g_pso_kq_iq3xxs)
    release_pso(g_pso_kq_mvb2_iq3xxs)
    release_pso(g_pso_kq_mvb4_iq3xxs)
    release_pso(g_pso_kq_mvb8_iq3xxs)
    release_pso(g_pso_kq_mm_b34)
    release_pso(g_pso_kq_iq4nl)
    release_pso(g_pso_kq_iq2s)
    release_pso(g_pso_kq_mvb2_iq2s)
    release_pso(g_pso_kq_mvb4_iq2s)
    release_pso(g_pso_kq_mvb8_iq2s)
    release_pso(g_pso_kq_mm_b23)
    release_pso(g_pso_kq_iq2xs)
    release_pso(g_pso_kq_mvb2_iq2xs)
    release_pso(g_pso_kq_mvb4_iq2xs)
    release_pso(g_pso_kq_mvb8_iq2xs)
    release_pso(g_pso_kq_mm_b24)
    release_pso(g_pso_kq_iq2xxs)
    release_pso(g_pso_kq_mvb2_iq2xxs)
    release_pso(g_pso_kq_mvb4_iq2xxs)
    release_pso(g_pso_kq_mvb8_iq2xxs)
    release_pso(g_pso_kq_mm_b25)
    release_pso(g_pso_kq_mvb2_iq4nl)
    release_pso(g_pso_kq_mvb4_iq4nl)
    release_pso(g_pso_kq_mvb8_iq4nl)
    release_pso(g_pso_kq_mm_b45)
    release_pso(g_pso_kq_k2)
    release_pso(g_pso_kq_mvb2_k2)
    release_pso(g_pso_kq_mvb4_k2)
    release_pso(g_pso_kq_mvb8_k2)
    release_pso(g_pso_kq_mm_b2)
    if (g_pso_rms != null) {
        metal_release(g_pso_rms)
        g_pso_rms = null
    }
    if (g_pso_rms_hx != null) {
        metal_release(g_pso_rms_hx)
        g_pso_rms_hx = null
    }
    release_pso(g_pso_dn_ba)
    release_pso(g_pso_moe_router_b)
    if (g_pso_add != null) {
        metal_release(g_pso_add)
        g_pso_add = null
    }
    if (g_pso_addrms_hx != null) {
        metal_release(g_pso_addrms_hx)
        g_pso_addrms_hx = null
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
    if (g_pso_geglu_hx != null) {
        metal_release(g_pso_geglu_hx)
        g_pso_geglu_hx = null
    }
    if (g_pso_ln != null) {
        metal_release(g_pso_ln)
        g_pso_ln = null
    }
    if (g_pso_gelu_erf != null) {
        metal_release(g_pso_gelu_erf)
        g_pso_gelu_erf = null
    }
    if (g_pso_gelu_lut != null) {
        metal_release(g_pso_gelu_lut)
        g_pso_gelu_lut = null
    }
    if (g_pso_posadd != null) {
        metal_release(g_pso_posadd)
        g_pso_posadd = null
    }
    release_pso(g_pso_q3v_stem)
    release_pso(g_pso_clamp)
    release_pso(g_pso_rope2d)
    release_pso(g_pso_head_restride)
    release_pso(g_pso_geglu_quick)
    release_pso(g_pso_cross_kx)
    release_pso(g_pso_cross_vx)
    release_pso(g_pso_cross_vx_h)
    release_pso(g_pso_im2col3)
    release_pso(g_pso_im2col2d)
    release_pso(g_pso_wdec_attn_part)
    release_pso(g_pso_wdec_attn_comb)
    release_pso(g_pso_bias_add_res)
    release_pso(g_pso_bias_gelu_lut)
    if (g_pso_f32mm != null) {
        metal_release(g_pso_f32mm)
        g_pso_f32mm = null
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
    if (g_pso_swiglu_hx != null) {
        metal_release(g_pso_swiglu_hx)
        g_pso_swiglu_hx = null
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
}


// ===== Metal-4 tensor twin races (decode-side batch GEMM families) =====

// one q8b batch-GEMM race: shared blob/X geometry, per-family psos + binds via the blocks
def private race_gemmb_family(dev : MetalDevice?; queue : MetalCommandQueue?; family : string;
                              m, kdim, ndim, out_n : int; grid : uint3;
                              base_pso, twin_pso : MetalComputePipeline?; base_tgmem, twin_tgmem : uint64;
                              var ka : Q8GemmArgs;
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
        kn_kargs(enc, ka, 5)
        kn_dispatch(enc, grid, tg)
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_tgmem(enc, twin_tgmem, 0)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 2)
        kn_buffer(enc, bxa, 0ul, 3)
        invoke(bind_extra, enc, by_twin)
        kn_kargs(enc, ka, 5)
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
                               twin_src, twin_entry : string; twin_fm : bool; twin_tgmem : uint64;
                               m, kdim, ndim, out_n : int; grid : uint3; var ka : Q8GemmArgs;
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
    let res = race_gemmb_family(dev, queue, family, m, kdim, ndim, out_n, grid, base_pso, twin_pso, base_tgmem, twin_tgmem, ka) $(enc, slot_y) {
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
    // the race binds the SAME kargs the encoders do — a hand-rolled bind list here silently
    // desyncs the moment the family's args change, and no parity suite runs this path
    var rka = gemm_ka(int64(kdim), int64(ndim), int64(ndim))
    var rka_sk = gemm_sk_ka(int64(kdim), int64(ndim), 4l)
    results |> emplace(race_decode_family(dev, queue, "gemmb_q8",
        MetalQ8GemmB_metal_q8_gemm_b_msl, MetalQ8GemmB_metal_q8_gemm_b_msl_entry, MetalQ8GemmB_metal_q8_gemm_b_msl_fastmath, MetalQ8GemmB_metal_q8_gemm_b_msl_tgmem,
        MetalQ8GemmBT_metal_q8_gemm_b_t_msl, MetalQ8GemmBT_metal_q8_gemm_b_t_msl_entry, MetalQ8GemmBT_metal_q8_gemm_b_t_msl_fastmath, MetalQ8GemmBT_metal_q8_gemm_b_t_msl_tgmem,
        32, kdim, ndim, 32 * ndim, uint3(uint(ndim / 32), 1u, 1u), rka) $(enc, slot_y) {
        kn_buffer(enc, slot_y, 0ul, 4)
    })
    results |> emplace(race_decode_family(dev, queue, "gemmb_sk_q8",
        MetalQ8GemmBSk_metal_q8_gemm_b_msl, MetalQ8GemmBSk_metal_q8_gemm_b_msl_entry, MetalQ8GemmBSk_metal_q8_gemm_b_msl_fastmath, MetalQ8GemmBSk_metal_q8_gemm_b_msl_tgmem,
        MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl, MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_entry, MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_fastmath, MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_tgmem,
        32, kdim, ndim, 4 * 32 * ndim, uint3(uint(ndim / 32), 4u, 1u), rka_sk) $(enc, slot_y) {
        kn_buffer(enc, slot_y, 0ul, 4)
    })
    results |> emplace(race_decode_family(dev, queue, "gemm64b_q8",
        metal_q8_gemm64_b_msl, metal_q8_gemm64_b_msl_entry, metal_q8_gemm64_b_msl_fastmath, metal_q8_gemm64_b_msl_tgmem,
        MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl, MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_entry, MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_fastmath, MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_tgmem,
        64, kdim, ndim, 64 * ndim, uint3(uint(2 * (ndim / 64)), 1u, 1u), rka) $(enc, slot_y) {
        kn_buffer(enc, slot_y, 0ul, 4)
    })
    metal_release(queue)
    metal_release(dev)
    return <- results
}

// ===== family-shared kernel classes (M1: moved from prefill) =====

// One threadgroup per row: cooperative sum of squares, lane 0 folds rsqrt(mean + eps), every
// thread scales its strided slice. HX = the dual-store stamp: the scale loop also writes the
// f16 twin panel the half-X GEMM stamps stream — the norm's consumers pay no convert dispatch.
[ |> template_struct_instance]
class template MetalRmsNormT {
    @ssbo @binding = 0 @off = "xoff" x : array<float>     // [nrows x dim] source
    @ssbo @binding = 1 @role = "weight" w : array<float>     // [dim] weight
    @ssbo @binding = 2 y : array<float>     // [nrows x dim] out (x aliasing is safe: x is fully
                                            // read before the barrier that precedes any write)
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 eps : float
    @ssbo @binding = 5 @role = "write" @template_gate = HX yh : array<float16>   // [nrows x dim] f16 twin
    @template_constant HX : bool = false
    @workgroup partial : float[32]          // one per simdgroup — width-agnostic up to 1024
                                            // threads (prefill dispatches 256, decode 1024)
    @workgroup inv_sh : float

    [metal_kernel]
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
            let ov = w[k] * (x[row * dim + k] * inv)
            y[row * dim + k] = ov
            static_if (HX) {
                yh[row * dim + k] = float16(ov)
            }
            k += gl_WorkGroupSize.x
        }
    }
}

// Three dispatch shapes on one stamp: decode single-row, batched prologue, prefill (256/tg).
[metal_dispatch(name = "enc_rms", pso = "g_pso_rms", tgmem = "MetalRmsNorm_metal_rmsnorm_msl_tgmem", tg = 1024, grid = "1"),
    metal_dispatch(name = "enc_rms_b", pso = "g_pso_rms", tgmem = "MetalRmsNorm_metal_rmsnorm_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "pf_enc_rms", pso = "g_pso_rms", tgmem = "MetalRmsNorm_metal_rmsnorm_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalRmsNorm : MetalRmsNormT {
}

[metal_dispatch(name = "pf_enc_rms_hx", pso = "g_pso_rms_hx", tgmem = "MetalRmsNormHX_metal_rmsnorm_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalRmsNormHX : MetalRmsNormT {
    override HX = true
}

// LayerNorm (weight + bias), the tower/embedder norm: two-pass mean then POPULATION variance,
// affine out. In-place safe: the write loop's x read is per-element by its owner, after both
// reduction barriers (gemma4uv calls it in place).
[metal_dispatch(name = "enc_ln", pso = "g_pso_ln", tgmem = "metal_layernorm_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalLayerNorm {
    @ssbo @binding = 0 x : array<float>     // [nrows x dim] source
    @ssbo @binding = 1 @role = "weight" @off = "woff" w : array<float>     // [dim]
    @ssbo @binding = 2 @role = "weight" @off = "boff" b : array<float>     // [dim]
    @ssbo @binding = 3 y : array<float>     // [nrows x dim] out (x aliasing safe, see header)
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 eps : float
    @workgroup partial : float[32]          // one per simdgroup; reused across the two passes
    @workgroup mean_sh : float
    @workgroup inv_sh : float

    [metal_kernel(name="metal_layernorm_msl")]
    def metal_layernorm {
        let row = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        var ss = 0.0
        var k = lid
        while (k < dim) {
            ss += x[row * dim + k]
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
            mean_sh = tot / float(dim)
        }
        barrier()
        let mean = mean_sh
        var vs = 0.0
        k = lid
        while (k < dim) {
            let dv = x[row * dim + k] - mean
            vs += dv * dv
            k += gl_WorkGroupSize.x
        }
        vs = simd_sum(vs)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = vs
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
            y[row * dim + k] = (x[row * dim + k] - mean) * inv * w[k] + b[k]
            k += gl_WorkGroupSize.x
        }
    }
}

// One elementwise map g[i] = map(g[i]) in place — the ew1 family (the 2-operand twin is MetalEw2T).
[ |> template_struct_instance]
class template MetalEw1T {
    @ssbo @binding = 0 @role = "readwrite" @off = "goff" @span = "total*4" g : array<float>
    @uniform @binding = 1 total : uint      // grid guard

    def abstract map(a : float) : float

    [metal_kernel]
    def metal_ew1 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let vv = g[gid]
        g[gid] = map(vv)
    }
}

// The tower GELUs are FORM-MATCHED per model, never substituted — a ~1e-3/element form delta
// amplifies coherently through a 32-layer residual stream into ~4e-2 rel_l2 (measured). The erf
// flavor's A&S 7.1.26 polynomial sits within f32 rounding of the CPU's double-precision erf.
[never_inline]   // the auto-inliner would fold this back into `map`, breaking its value-position splice (single-return rule)
def private msl_gelu_erf_form(a : float) : float {
    let z = abs(a) * 0.7071067811865476
    let tt = 1.0 / (1.0 + 0.3275911 * z)
    let e = tt * (0.254829592 + tt * (-0.284496736 + tt * (1.421413741 + tt * (-1.453152027 + tt * 1.061405429)))) * exp(-z * z)
    return 0.5 * a * (1.0 + (a >= 0.0 ? 1.0 - e : e - 1.0))
}

[metal_dispatch(name = "enc_gelu_erf", pso = "g_pso_gelu_erf", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGeluErf : MetalEw1T {
    def override map(a : float) : float => msl_gelu_erf_form(a)
}

[never_inline]   // same single-return-splice protection as msl_gelu_erf_form
def private msl_gelu_lut_form(a : float) : float {
    if (a <= -10.0) {
        return 0.0
    }
    if (a >= 10.0) {
        return a
    }
    let xr = float(float16(a))
    let gv = 0.5 * xr * (2.0 - 2.0 / (1.0 + exp(2.0 * (0.79788456080286535587989211986876 * xr * (1.0 + 0.044715 * xr * xr)))))
    return float(float16(gv))
}

// The fp16-LUT tanh GELU, form-matched to the CPU table: the +-10 clamps and BOTH f16 round-trips
// run in kernel (RNE, same as the CPU table's domain), tanh via the exp identity (Metal tanh NaNs
// past ~44; the clamped argument reaches 43.7). Near-bit-exact vs the CPU LUT by construction.
[metal_dispatch(name = "enc_gelu_lut", pso = "g_pso_gelu_lut", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGeluLut : MetalEw1T {
    def override map(a : float) : float => msl_gelu_lut_form(a)
}

// The gemma4uv 2-axis position add: row p gains its x-table row (p % ncols) and its y-table
// row (pos_entries + p / ncols), one thread per element — bit-exact vs the CPU loop's
// (row + ex) + ey order.
[metal_dispatch(name = "enc_posadd2d", pso = "g_pso_posadd", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPosAdd2D {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>   // [npos x dim]
    @ssbo @binding = 1 @role = "weight" @off = "tbloff" tbl : array<float>    // [2*pos_entries x dim] x rows then y rows
    @uniform @binding = 2 dim : uint
    @uniform @binding = 3 ncols : uint
    @uniform @binding = 4 pos_entries : uint
    @uniform @binding = 5 total : uint

    [metal_kernel(name="metal_posadd2d_msl")]
    def metal_posadd2d {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let p = gid / dim
        let i = gid % dim
        let ex = tbl[(p % ncols) * dim + i]
        let ey = tbl[(pos_entries + p / ncols) * dim + i]
        x[gid] = (x[gid] + ex) + ey
    }
}

// The qwen3v stem assemble: residual row r (the merge-adjacent walk - 2x2 partners adjacent,
// groups row-major) gathers its natural-order conv row, + conv bias + that row's position -
// bit-exact vs the CPU stem's (crow + bp) + prow order.
[metal_dispatch(name = "enc_q3v_stem", pso = "g_pso_q3v_stem", tg = 64, grid = "total/64", params = "total : int64")]
class MetalQ3vStemAsm {
    @ssbo @binding = 0 @role = "read" xn : array<float>                  // conv out, natural patch order [npos x dim]
    @ssbo @binding = 1 @role = "weight" @off = "boff" b : array<float>   // conv bias [dim]
    @ssbo @binding = 2 @role = "read" @off = "poff" pos : array<float>   // pos rows, natural order [npos x dim]
    @ssbo @binding = 3 @role = "write" x : array<float>                  // residual, merge-adjacent [npos x dim]
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 pw : uint
    @uniform @binding = 6 total : uint

    [metal_kernel(name="metal_q3v_stem_msl")]
    def metal_q3v_stem {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let r = gid / dim
        let i = gid % dim
        let g = r / 4u
        let sub = r % 4u
        let gcols = pw / 2u
        let src = ((g / gcols) * 2u + sub / 2u) * pw + (g % gcols) * 2u + sub % 2u
        x[gid] = (xn[src * dim + i] + b[i]) + pos[src * dim + i]
    }
}

// The ClippableLinear clamp (gemma4v): dst = clamp(src, lo, hi), the bounds read off the tower's
// f32 blob at the GEMM's clamp record (imin imax omin omax …; `side` 0 = input pair, 1 = output
// pair). dst may be src (each thread owns one element); an inactive record holds ±FLT_MAX = identity.
[metal_dispatch(name = "enc_clamp", pso = "g_pso_clamp", tg = 64, grid = "total/64", params = "total : int64")]
class MetalClamp {
    @ssbo @binding = 0 @role = "read" @off = "soff" @span = "total*4" src : array<float>
    @ssbo @binding = 1 @role = "write" @off = "doff" @span = "total*4" dst : array<float>
    @ssbo @binding = 2 @role = "weight" @off = "boff" bounds : array<float>   // [imin imax omin omax] at the record
    @uniform @binding = 3 side : uint
    @uniform @binding = 4 total : uint

    [metal_kernel(name="metal_clamp_msl")]
    def metal_clamp {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let lo = bounds[side * 2u]
        let hi = bounds[side * 2u + 1u]
        dst[gid] = min(max(src[gid], lo), hi)   // nolint:STYLE019 — the MSL subset has no clamp lowering
    }
}

// Head restride (gemma3v): repack token-major rows [npos x n_head*hs_src] to [npos x n_head*hs_dst],
// zero-filling dst lanes past hs_src — pads Q/K/V to the attention tile head width (72 -> 128) and
// compacts the AV output back; zero pads keep both directions exact.
[metal_dispatch(name = "enc_head_restride", pso = "g_pso_head_restride", tg = 64, grid = "total/64", params = "total : int64")]
class MetalHeadRestride {
    @ssbo @binding = 0 @role = "read" @off = "soff" src : array<float>
    @ssbo @binding = 1 @role = "write" @off = "doff" dst : array<float>
    @uniform @binding = 2 hs_src : uint
    @uniform @binding = 3 hs_dst : uint
    @uniform @binding = 4 total : uint   // npos * n_head * hs_dst

    [metal_kernel(name="metal_head_restride_msl")]
    def metal_head_restride {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let lane = gid % hs_dst
        let hp = gid / hs_dst
        dst[gid] = lane < hs_src ? src[hp * hs_src + lane] : 0.0
    }
}

// The gemma4v two-axis NEOX rope over token-major rows [npos x n_head*hs]: each head's first hs/2
// dims rotate by the row's x table, the second hs/2 by its y table — `tabs` packs the CPU's four
// tables per row as [cx | sx | cy | sy], `pairs` (= hs/4) wide each; one thread per (row, head, j).
[metal_dispatch(name = "enc_rope2d", pso = "g_pso_rope2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalRope2D {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>     // [npos x d]
    @ssbo @binding = 1 @role = "read" tabs : array<float>       // [npos x 4q]
    @uniform @binding = 2 d : uint
    @uniform @binding = 3 pairs : uint   // rotated pairs per axis, one table row; the head is 4 of them
    @uniform @binding = 4 total : uint   // npos * n_head * pairs

    [metal_kernel(name="metal_rope2d_msl")]
    def metal_rope2d {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let hs = 4u * pairs
        let n_head = d / hs
        let r = gid / (n_head * pairs)
        let rem = gid % (n_head * pairs)
        let h = rem / pairs
        let j = rem % pairs
        let base = r * d + h * hs
        let tb = r * 4u * pairs
        let cx = tabs[tb + j]
        let sx = tabs[tb + pairs + j]
        let cy = tabs[tb + 2u * pairs + j]
        let sy = tabs[tb + 3u * pairs + j]
        let x0 = x[base + j]
        let x1 = x[base + j + pairs]
        x[base + j] = x0 * cx - x1 * sx
        x[base + j + pairs] = x0 * sx + x1 * cx
        let y0 = x[base + 2u * pairs + j]
        let y1 = x[base + 3u * pairs + j]
        x[base + 2u * pairs + j] = y0 * cy - y1 * sy
        x[base + 3u * pairs + j] = y0 * sy + y1 * cy
    }
}

// The whisper cross-KV memory shuffles: one thread per OUTPUT element. Kx transposes the ck GEMM
// rows [ta x d] into the decoder's head-major [hh][hs][ta], pre-scaled hs^-0.25 (the scale rides
// the K store — attention then applies no K scale); Vx re-tiles to [hh][ta][hs], no scale.
[metal_dispatch(name = "enc_cross_kx", pso = "g_pso_cross_kx", tg = 64, grid = "total/64", params = "total : int64")]
class MetalCrossKx {
    @ssbo @binding = 0 @role = "read" src : array<float>              // [ta x d] ck GEMM rows
    @ssbo @binding = 1 @role = "write" @off = "kxoff" kx : array<float>   // [hh][hs][ta] pre-scaled
    @uniform @binding = 2 dim : uint
    @uniform @binding = 3 hs : uint
    @uniform @binding = 4 ta : uint
    @uniform @binding = 5 kscale : float
    @uniform @binding = 6 total : uint

    [metal_kernel(name="metal_cross_kx_msl")]
    def metal_cross_kx {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let hsta = hs * ta
        let h = gid / hsta
        let rem = gid % hsta
        let j = rem / ta
        let t = rem % ta
        kx[gid] = src[t * dim + h * hs + j] * kscale
    }
}

// Vx doubles as the decode step's KV APPEND: `ta` is the row EXTENT being written, `t0` the
// destination position, `tstride` the per-head t capacity ([hh][tstride][hs] layout); `soff`
// picks a source column block (the fused-QKV row), `hasb` folds a per-column bias on the way.
[metal_dispatch(name = "enc_cross_vx", pso = "g_pso_cross_vx", tg = 64, grid = "total/64", params = "total : int64")]
class MetalCrossVx {
    @ssbo @binding = 0 @role = "read" @off = "soff" src : array<float>    // [ta x d] source rows
    @ssbo @binding = 1 @role = "write" @off = "vxoff" vx : array<float>   // [hh][tstride][hs]
    @ssbo @binding = 2 @role = "weight" @off = "boff" bias : array<float> // [d] (read only when hasb)
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 hs : uint
    @uniform @binding = 5 ta : uint
    @uniform @binding = 6 t0 : uint
    @uniform @binding = 7 tstride : uint
    @uniform @binding = 8 hasb : uint
    @uniform @binding = 9 total : uint

    [metal_kernel(name="metal_cross_vx_msl")]
    def metal_cross_vx {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let tahs = ta * hs
        let h = gid / tahs
        let rem = gid % tahs
        let t = rem / hs
        let j = rem % hs
        var v = src[t * dim + h * hs + j]
        if (hasb != 0u) {
            v += bias[h * hs + j]
        }
        vx[h * tstride * hs + (t0 + t) * hs + j] = v
    }
}

// The f16 store twin: the decoder's RESIDENT K/V memory is float16 (format parity with the
// reference KV — half the attention read bytes); the f32 form above keeps the CPU-layout
// readbacks. Same index math, converted store.
[metal_dispatch(name = "enc_cross_vx_h", pso = "g_pso_cross_vx_h", tg = 64, grid = "total/64", params = "total : int64")]
class MetalCrossVxH {
    @ssbo @binding = 0 @role = "read" @off = "soff" src : array<float>
    @ssbo @binding = 1 @role = "write" @off = "vxoff" vx : array<float16>   // [hh][tstride][hs] f16
    @ssbo @binding = 2 @role = "weight" @off = "boff" bias : array<float>
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 hs : uint
    @uniform @binding = 5 ta : uint
    @uniform @binding = 6 t0 : uint
    @uniform @binding = 7 tstride : uint
    @uniform @binding = 8 hasb : uint
    @uniform @binding = 9 total : uint

    [metal_kernel(name="metal_cross_vx_h_msl")]
    def metal_cross_vx_h {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let tahs = ta * hs
        let h = gid / tahs
        let rem = gid % tahs
        let t = rem / hs
        let j = rem % hs
        var v = src[t * dim + h * hs + j]
        if (hasb != 0u) {
            v += bias[h * hs + j]
        }
        vx[h * tstride * hs + (t0 + t) * hs + j] = float16(min(max(v, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp
    }
}

// The chunked twin pair: Part computes one 256-t chunk's online-softmax partial per
// (head, row, chunk) — local max m, exp-sum s, unnormalized weighted-V y — Comb merges the
// chunks by log-sum-exp. 6-8x the threadgroups of the single-pass form on a 1500-t cross.
[metal_dispatch(name = "enc_wdec_attn_part", pso = "g_pso_wdec_attn_part", tgmem = "metal_wdec_attn_part_msl_tgmem", tg = 128, grid = "heads, nrows, nch", params = "heads : int64, nrows : int64, nch : int64")]
class MetalWdecAttnPart {
    @ssbo @binding = 0 @role = "read" q : array<float>                    // [n x dim] (unscaled, pre-bias)
    @ssbo @binding = 1 @role = "read" @off = "koff" kk : array<float16>   // [hh][tstride][hs]
    @ssbo @binding = 2 @role = "read" @off = "voff" vv : array<float16>   // [hh][tstride][hs]
    @ssbo @binding = 3 @role = "write" pms : array<float>                 // [nch][hh][n][2] (m, s)
    @ssbo @binding = 4 @role = "write" py : array<float>                  // [nch][hh][n][hs]
    @ssbo @binding = 5 @role = "weight" @off = "qboff" qb : array<float>  // [dim] q bias, folded at load
    @uniform @binding = 6 dim : uint
    @uniform @binding = 7 hs : uint
    @uniform @binding = 8 tstride : uint
    @uniform @binding = 9 len0 : uint
    @uniform @binding = 10 causal : uint
    @uniform @binding = 11 scale : float
    @uniform @binding = 12 nheads : uint
    @uniform @binding = 13 nrows_u : uint
    @workgroup scores : float[256]
    @workgroup partial : float[32]
    @workgroup red_sh : float

    [metal_kernel(name="metal_wdec_attn_part_msl")]
    def metal_wdec_attn_part {   // nolint:STYLE038 — the phases are barrier-coupled, the kernel body is the shape
        let h = gl_WorkGroupID.x
        let r = gl_WorkGroupID.y
        let c = gl_WorkGroupID.z
        let lid = gl_LocalInvocationID.x
        let len = causal != 0u ? len0 + r : len0
        let tb = c * 256u
        let te = min(tb + 256u, len)
        let slot = (c * nheads + h) * nrows_u + r
        let qbase = r * dim + h * hs
        let kbase = h * tstride * hs
        var t = tb + lid
        while (t < te) {
            var s = 0.0
            var j = 0u
            while (j < hs) {
                s += (q[qbase + j] + qb[h * hs + j]) * float(kk[kbase + t * hs + j])
                s += (q[qbase + j + 1u] + qb[h * hs + j + 1u]) * float(kk[kbase + t * hs + j + 1u])
                s += (q[qbase + j + 2u] + qb[h * hs + j + 2u]) * float(kk[kbase + t * hs + j + 2u])
                s += (q[qbase + j + 3u] + qb[h * hs + j + 3u]) * float(kk[kbase + t * hs + j + 3u])
                j += 4u
            }
            scores[t - tb] = s * scale
            t += gl_WorkGroupSize.x
        }
        barrier()
        let nloc = te > tb ? te - tb : 0u
        var mx = -3.0e38
        t = lid
        while (t < nloc) {
            mx = max(mx, scores[t])
            t += gl_WorkGroupSize.x
        }
        var bm = 16u
        while (bm >= 1u) {
            mx = max(mx, simd_shuffle_xor(mx, bm))
            bm /= 2u
        }
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = mx
        }
        barrier()
        if (lid == 0u) {
            var tm = -3.0e38
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tm = max(tm, partial[sgi])
                sgi++
            }
            red_sh = tm
        }
        barrier()
        let m = red_sh
        var ss = 0.0
        t = lid
        while (t < nloc) {
            let e = exp(scores[t] - m)
            scores[t] = e
            ss += e
            t += gl_WorkGroupSize.x
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
            pms[slot * 2u] = nloc > 0u ? m : -3.0e38
            pms[slot * 2u + 1u] = tot
        }
        barrier()
        var j = lid
        while (j < hs) {
            var acc = 0.0
            var tt = 0u
            while (tt < nloc) {
                acc += scores[tt] * float(vv[kbase + (tb + tt) * hs + j])
                tt++
            }
            py[slot * hs + j] = acc
            j += gl_WorkGroupSize.x
        }
    }
}

[metal_dispatch(name = "enc_wdec_attn_comb", pso = "g_pso_wdec_attn_comb", tg = 64, grid = "heads, nrows", params = "heads : int64, nrows : int64")]
class MetalWdecAttnComb {
    @ssbo @binding = 0 @role = "read" pms : array<float>    // [nch][hh][n][2]
    @ssbo @binding = 1 @role = "read" py : array<float>     // [nch][hh][n][hs]
    @ssbo @binding = 2 @role = "write" y : array<float>     // [n x dim]
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 hs : uint
    @uniform @binding = 5 nch : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 nrows_u : uint

    [metal_kernel(name="metal_wdec_attn_comb_msl")]
    def metal_wdec_attn_comb {
        let h = gl_WorkGroupID.x
        let r = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        var mg = -3.0e38
        var c = 0u
        while (c < nch) {
            mg = max(mg, pms[((c * nheads + h) * nrows_u + r) * 2u])
            c++
        }
        var tot = 0.0
        c = 0u
        while (c < nch) {
            let slot = (c * nheads + h) * nrows_u + r
            tot += pms[slot * 2u + 1u] * exp(pms[slot * 2u] - mg)
            c++
        }
        let inv = 1.0 / tot
        var j = lid
        while (j < hs) {
            var acc = 0.0
            c = 0u
            while (c < nch) {
                let slot = (c * nheads + h) * nrows_u + r
                acc += py[slot * hs + j] * exp(pms[slot * 2u] - mg)
                c++
            }
            y[r * dim + h * hs + j] = acc * inv
            j += gl_WorkGroupSize.x
        }
    }
}

// The whisper conv-frontend im2col (k=3, "same" pad): output row i is 3 x nch columns in
// (dk, c) order — the loader's weight permute. `chmajor` picks the source layout: conv1 reads
// the channel-major mel ([c][t]), conv2 the row-major y1 ([t][c]); `stride` is 1 or 2.
[metal_dispatch(name = "enc_im2col3", pso = "g_pso_im2col3", tg = 64, grid = "total/64", params = "total : int64")]
class MetalIm2col3 {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" col : array<float>   // [rows x 3*nch]
    @uniform @binding = 2 nch : uint
    @uniform @binding = 3 srclen : uint     // source positions (the pad guard)
    @uniform @binding = 4 stride : uint
    @uniform @binding = 5 chmajor : uint
    @uniform @binding = 6 total : uint

    [metal_kernel(name="metal_im2col3_msl")]
    def metal_im2col3 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let w3 = 3u * nch
        let i = gid / w3
        let rem = gid % w3
        let dk = rem / nch
        let c = rem % nch
        let sp = int(i * stride + dk) - 1
        var v = 0.0
        if (sp >= 0 && sp < int(srclen)) {
            v = chmajor != 0u ? src[c * srclen + uint(sp)] : src[uint(sp) * nch + c]
        }
        col[gid] = v
    }
}

// The qwen3a conv2d im2col (k=3, s=2, p=1, one [iw x ih] block, `sstride` floats per source
// pixel): output row (oy*ow+ox) carries `kd` columns — cin*9 real ones in (ic, kh, kw) order
// (conv2d_s2's walk) then zero pad, so the tile GEMM's kdim%32 holds at any cin.
[metal_dispatch(name = "enc_im2col2d", pso = "g_pso_im2col2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalIm2col2d {
    @ssbo @binding = 0 @role = "read" @off = "soff" src : array<float>   // [ih x iw x sstride], c fastest
    @ssbo @binding = 1 @role = "write" col : array<float>                // [oh*ow x kd]
    @uniform @binding = 2 iw : uint
    @uniform @binding = 3 ih : uint
    @uniform @binding = 4 cin : uint
    @uniform @binding = 5 ow : uint
    @uniform @binding = 6 kd : uint
    @uniform @binding = 7 sstride : uint
    @uniform @binding = 8 total : uint

    [metal_kernel(name="metal_im2col2d_msl")]
    def metal_im2col2d {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let r = gid / kd
        let k = gid % kd
        var v = 0.0
        if (k < cin * 9u) {
            let ic = k / 9u
            let kh = (k % 9u) / 3u
            let kw = k % 3u
            let oy = r / ow
            let ox = r % ow
            let iy = int(oy * 2u + kh) - 1
            let ix = int(ox * 2u + kw) - 1
            if (iy >= 0 && iy < int(ih) && ix >= 0 && ix < int(iw)) {
                v = src[(uint(iy) * iw + uint(ix)) * sstride + ic]
            }
        }
        col[gid] = v
    }
}

// bias + residual add in one pass: x[i] += y[i] + b[i % dim]
[metal_dispatch(name = "enc_bias_add_res", pso = "g_pso_bias_add_res", tg = 64, grid = "total/64", params = "total : int64")]
class MetalBiasAddRes {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "read" y : array<float>
    @ssbo @binding = 2 @role = "weight" @off = "boff" b : array<float>
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 total : uint

    [metal_kernel(name="metal_bias_add_res_msl")]
    def metal_bias_add_res {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] += y[gid] + b[gid % dim]
    }
}

// bias + the LUT-form GELU in one pass (the form-matched flavor — never substitute forms)
[metal_dispatch(name = "enc_bias_gelu_lut", pso = "g_pso_bias_gelu_lut", tg = 64, grid = "total/64", params = "total : int64")]
class MetalBiasGeluLut {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "weight" @off = "boff" b : array<float>
    @uniform @binding = 2 dim : uint
    @uniform @binding = 3 total : uint

    [metal_kernel(name="metal_bias_gelu_lut_msl")]
    def metal_bias_gelu_lut {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] = msl_gelu_lut_form(x[gid] + b[gid % dim])
    }
}


// The shared 32x64 GEMM tile stage: every mul_mm-family kernel (dense, MoE, attention, KV-quant)
// stages A into ta (4 k-tiles x 8 row-tiles of 8x8) and B into tb (4 k-tiles x 4 token-tiles),
// then walks 4 k-steps of 4 A-loads / 2 B-loads / 8 MACs. The method splices flat at emission.
class MetalMmTileBase {
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    def acc_quad(aB, bB : uint; var ma : simdgroup_half8x8[4]; var mb : simdgroup_half8x8[2]; var mc : simdgroup_float8x8[8]) {
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
    }
}

// Dense f32-weight GEMM y[m x ndim] = x[m x kdim] . W^T: the attention QK mm form with head
// indexing and the causal skip stripped — both operands stage f32 -> half tiles, f32
// accumulate; ndim % 64, kdim % 32, M padded to 32.
[metal_dispatch(name = "enc_f32_mm", pso = "g_pso_f32mm", tgmem = "metal_f32_mulmm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalF32MulMm : MetalMmTileBase {
    @ssbo @binding = 0 @role = "weight" @off = "woff" w : array<float4>   // [ndim x kdim/4] weight rows
    @ssbo @binding = 1 @role = "read" xf : array<float4>                  // [m x kdim/4] activations
    @ssbo @binding = 2 @role = "write" @off = "yoff" @span = "mp*d*4" y : array<float>
    @uniform @binding = 3 kdim : uint
    @uniform @binding = 4 ndim : uint
    // inherited ta = W tiles (k row, wrow col), tb = X tiles (token row, k col)

    [metal_kernel(name="metal_f32_mulmm_msl")]
    def metal_f32_mulmm {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        let kdim4 = kdim / 4u
        let lr0 = lid / 2u                  // A split: weight row + 16-k half
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u                  // B split: token row + 8-k run
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        var kb = 0u
        while (kb * 32u < kdim) {
            let srcA = (nBase + lr0) * kdim4 + (kb * 32u + il0 * 16u) / 4u
            let a0 = w[srcA]
            let a1 = w[srcA + 1u]
            let a2 = w[srcA + 2u]
            let a3 = w[srcA + 3u]
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xf[srcB]
            let b1 = xf[srcB + 1u]
            var va : float16[16]
            va[0] = float16(a0.x)
            va[1] = float16(a0.y)
            va[2] = float16(a0.z)
            va[3] = float16(a0.w)
            va[4] = float16(a1.x)
            va[5] = float16(a1.y)
            va[6] = float16(a1.z)
            va[7] = float16(a1.w)
            va[8] = float16(a2.x)
            va[9] = float16(a2.y)
            va[10] = float16(a2.z)
            va[11] = float16(a2.w)
            va[12] = float16(a3.x)
            va[13] = float16(a3.y)
            va[14] = float16(a3.z)
            va[15] = float16(a3.w)
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            acc_quad(aB, bB, ma, mb, mc)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = weight rows
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// The production GEMM: 64-output x 32-token C tiles (4 simdgroups, 8 accumulators), W as
// interleaved 34B q8 blocks, raw f32 activations. Rolled [unroll_full] math over simdgroup-matrix
// arrays. Requires out-dim % 64, M padded to 64. Beats the upstream equivalent 2-3% on M1 Max.
[metal_dispatch(name = "enc_gemm_mm_c", pso = "g_pso_mm", tgmem = "metal_q8_mulmm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalQ8MulMm : MetalMmTileBase {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> // 34B-block W blob, HALF-SCALE view: scale of block b at 17*b
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    // the SAME blob buffer, BYTE view: quants of block b at 34*b + 2
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // raw f32 activations, float4 view
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "mp*d*4" y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    // inherited ta = W tiles (k row, wrow col), tb = X tiles (token row, k col)

    [metal_kernel(name="metal_q8_mulmm_msl")]
    def metal_q8_mulmm {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (rasterization order)
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
        // kernel measured pointer, with the scale read straight off the carried pointer
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
            acc_quad(aB, bB, ma, mb, mc)
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

// The K-quant prefill GEMM trio as one template — q5_K = q4_K's kmask decode + the QH overlay
// (nsh repeated per arm to keep line order); q6_K = the SIXBIT arm (own scale/quant decode,
// everything around it shared). Q8 (blob walk) stays its own class.
[ |> template_struct_instance]
class template MetalKqMulMmK45T : MetalMmTileBase {
    // s0off is the SLOT-0 view offset: k4/k5 callers pass soff again, k6 passes the d-plane doff
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // k4/k5: 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1); k6: the d plane (bound at byte nsb*16)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view (k6: 16 int8 sub-scales at blk, bound at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // quant plane, uint view (qs at BLK*blk; k5 qh at BLK*blk+32; k6 ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "mp*rows*4" y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @template_constant BLK : uint = 32u  // superblock quant-plane stride in uints (unused in the SIXBIT arm — k6 strides are hardcoded 48/16/8)
    @template_constant QH : bool = false // q5_K: the 5th-bit overlay plane rides after the nibbles
    @template_constant SIXBIT : bool = false // q6_K: int8 sub-scales x super-d, ql/qh compose
    @template_constant IQ4XS : bool = false // iq4xs: the k6 split scale form, the q40 pairing, codebook decode
    @template_constant K3 : bool = false // q3_K: the k6 split scale form, 2-bit lane + hmask compose, offset 4
    @template_constant IQ3S : bool = false // iq3s: the k6 split scale form, threadgroup grid gather + sign flip
    @template_constant IQ3XXS : bool = false // iq3xxs: the same form over the halved grid + parity signs
    @template_constant IQ4NL : bool = false // iq4nl: the IQ4XS nibble walk with the per-32-block f16 d plane (no sub-scale; s0off = soff)
    @template_constant K2 : bool = false // q2_K: per-16 sc/min nibble pairs (one uint4 strip), the split 4B d+dmin tail at s0off
    @template_constant IQ2S : bool = false // iq2s: the u64-grid gather DIRECT off the hoisted constant table, per-16 UNSIGNED strips
    @template_constant IQ2XS : bool = false // iq2xs: iq2s's walk with ksigns-by-parity off each u16 qs word (no sign plane, no qh)
    @template_constant IQ2XXS : bool = false // iq2xxs: byte grid indices + aux32 parity ksigns, per-32 UNSIGNED strips
    @workgroup @template_gate = IQ4XS iq4lut : float[16]   // kvalues_iq4nl as floats, staged once per threadgroup
    @workgroup @template_gate = IQ3S gridw3 : uint[512]   // the 2 KB iq3s grid, staged once per threadgroup
    @workgroup @template_gate = IQ3XXS gridw3x : uint[256]   // the 1 KB halved iq3xxs grid, staged once per threadgroup

    [metal_kernel]
    def metal_kq_mulmm {   // nolint:STYLE037,STYLE038 — MSL kernel body; the mul_mm tiles are simdgroup-barrier coupled
        let lid = gl_LocalInvocationID.x
        static_if (IQ4XS) {
            if (lid < 16u) {
                iq4lut[lid] = iq4_lut(lid)
            }
            barrier()
        }
        static_if (IQ3S) {
            for [unroll_full] (gk in range(4)) {
                let gi = lid + uint(gk) * 128u
                gridw3[gi] = iq3s_gw(int(gi))
            }
            barrier()
        }
        static_if (IQ3XXS) {
            for [unroll_full] (gk in range(2)) {
                let gi = lid + uint(gk) * 128u
                gridw3x[gi] = iq3xxs_gw(int(gi))
            }
            barrier()
        }
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
            static_if (SIXBIT) {
                // q6_K: scalars reload per kb (uniform, L1-served) — the hoisted sv/dall cache cost
                // an occupancy tier (896 vs 1024 max_threads) and measured 1.7-10% slower per shape
                // (bench_metal_kq_mm_lab, 3 launches, bit-exact)
                let half6 = js / 4u
                let gg = js % 4u
                let sv = ks4[blk]
                let dall = float(ksh[blk])
                let si = half6 * 8u + gg * 2u + il0
                let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
                let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
                let dsc = dall * s6
                let dmn = dsc * 32.0   // -32 folded to the fma form (k4's dmn shape)
                // byte-position shift folded into exact power-of-two pre-scales: q<<8c is an exact
                // f32 integer (6-bit quants), dsc/2^8c is an exponent shift, so each fma sees the
                // same real product — bit-identical, one shift fewer per element
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
            } else {
                static_if (K3) {
                    // q3_K: strip byte si is the SIGNED sub-scale (k6's split form); block js of the
                    // superblock = qs uints (js/4)*8 + il0*4.. at shift 2*(js%4) | hmask uints 16 + il0*4.. bit js
                    let hf3 = js / 4u
                    let gg3 = js % 4u
                    let sv = ks4[blk]
                    let dall = float(ksh[blk])
                    let si = hf3 * 8u + gg3 * 2u + il0
                    let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
                    let s3 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
                    let dsc = dall * s3
                    let dmn = dsc * 4.0
                    let dsc1 = dsc * 0.00390625
                    let dsc2 = dsc * 0.0000152587890625
                    let dsc3 = dsc * 0.000000059604644775390625
                    let qb = blk * 24u + hf3 * 8u + il0 * 4u
                    let hb = blk * 24u + 16u + il0 * 4u
                    let nsh = gg3 * 2u
                    let hsh = hf3 * 4u + gg3
                    for [unroll_full] (k in range(4)) {
                        let u = kqu[qb + uint(k)]
                        let hu = kqu[hb + uint(k)]
                        let qv = ((u >> nsh) & 0x03030303) | (((hu >> hsh) & 0x01010101) << 2u)
                        va[k * 4] = float16(dsc * float(qv & 255u) - dmn)
                        va[k * 4 + 1] = float16(dsc1 * float(qv & 65280u) - dmn)
                        va[k * 4 + 2] = float16(dsc2 * float(qv & 16711680u) - dmn)
                        va[k * 4 + 3] = float16(dsc3 * float(qv & 4278190080u) - dmn)
                    }
                } else {
                    static_if (IQ2XXS) {
                        // iq2xxs: per-32 UNSIGNED strip byte js; block js's grid word carries
                        // four byte indices, its aux32 the parity-derived ksigns
                        let sv = ks4[blk]
                        let scw = js < 4u ? sv.x : sv.y
                        let dsc = float(ksh[blk]) * float((scw >> ((js & 3u) * 8u)) & 255u)
                        let gw2 = kqu[blk * 16u + js * 2u]
                        let aux2 = kqu[blk * 16u + js * 2u + 1u]
                        for [unroll_full] (k in range(4)) {
                            let q3 = il0 * 2u + uint(k) / 2u
                            let bidx = (gw2 >> (8u * q3)) & 255u
                            let sgb2 = ksign7m((aux2 >> (7u * q3)) & 127u)
                            let nib = uint(k) % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u
                            let sw = iq3s_sw(iq2xxs_gw(int(bidx * 2u + uint(k) % 2u)), nib)
                            for [unroll_full] (c in range(4)) {
                                va[k * 4 + c] = float16(dsc * iq3s_sx(sw, uint(c)))
                            }
                        }
                    } else {
                    static_if (IQ2XS) {
                        // iq2xs: the iq2s strip/scale reads; each u16 qs word = 9-bit grid
                        // index + 7-bit ksigns index whose sign byte ksign7m recomputes
                        let sv = ks4[blk]
                        let si2 = js * 2u + il0
                        let scw = si2 < 8u ? (si2 < 4u ? sv.x : sv.y) : (si2 < 12u ? sv.z : sv.w)
                        let dsc = float(ksh[blk]) * float((scw >> ((si2 & 3u) * 8u)) & 255u)
                        let qw2 = kqu[blk * 16u + js * 2u + il0]
                        for [unroll_full] (k in range(4)) {
                            let w16 = (qw2 >> (16u * (uint(k) / 2u))) & 0xFFFFu
                            let idx2 = (w16 & 511u) * 2u + uint(k) % 2u
                            let sgb2 = ksign7m(w16 >> 9u)
                            let nib = uint(k) % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u
                            let sw = iq3s_sw(iq2xs_gw(int(idx2)), nib)
                            for [unroll_full] (c in range(4)) {
                                va[k * 4 + c] = float16(dsc * iq3s_sx(sw, uint(c)))
                            }
                        }
                    } else {
                    static_if (IQ2S) {
                        // iq2s: strip byte js*2+il0 is the UNSIGNED per-16 sub-scale; idx byte l of
                        // block js doubles into the u64 grid's word pair, signs off the block's bytes
                        let sv = ks4[blk]
                        let si2 = js * 2u + il0
                        let scw = si2 < 8u ? (si2 < 4u ? sv.x : sv.y) : (si2 < 12u ? sv.z : sv.w)
                        let dsc = float(ksh[blk]) * float((scw >> ((si2 & 3u) * 8u)) & 255u)
                        let qw2 = kqu[blk * 18u + js]
                        let sgw2 = kqu[blk * 18u + 8u + js]
                        let qh2 = (kqu[blk * 18u + 16u + js / 4u] >> ((js % 4u) * 8u)) & 255u
                        for [unroll_full] (k in range(4)) {
                            let l2 = il0 * 2u + uint(k) / 2u
                            let idx2 = (((qw2 >> (8u * l2)) & 255u) | ((qh2 << (8u - 2u * l2)) & 0x300u)) * 2u + uint(k) % 2u
                            let sgb2 = (sgw2 >> (8u * l2)) & 255u
                            let nib = uint(k) % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u
                            let sw = iq3s_sw(iq2s_gw(int(idx2)), nib)
                            for [unroll_full] (c in range(4)) {
                                va[k * 4 + c] = float16(dsc * iq3s_sx(sw, uint(c)))
                            }
                        }
                    } else {
                    static_if (IQ3XXS) {
                        // iq3xxs: the iq3s arm over the halved grid - bare byte index, parity
                        // signs off the block's aux32 (no table)
                        let sv = ks4[blk]
                        let scw = js < 4u ? sv.x : sv.y
                        let dsc = float(ksh[blk]) * float(((int(scw >> ((js & 3u) * 8u)) & 255) ^ 128) - 128)
                        let qsw = kqu[blk * 24u + js * 2u + il0]
                        let aux3 = kqu[blk * 24u + 16u + js]
                        for [unroll_full] (k in range(4)) {
                            let q3 = il0 * 2u + uint(k) / 2u
                            let sgb = ksign7m((aux3 >> (7u * q3)) & 127u)
                            let nib = uint(k) % 2u == 0u ? sgb & 15u : sgb >> 4u
                            let sw = iq3s_sw(gridw3x[(qsw >> (8u * uint(k))) & 255u], nib)
                            for [unroll_full] (c in range(4)) {
                                va[k * 4 + c] = float16(dsc * iq3s_sx(sw, uint(c)))
                            }
                        }
                    } else {
                        static_if (IQ3S) {
                            // iq3s: strip byte js is the SIGNED sub-scale (the k6 split form); block js's
                            // qs word (js*2 + il0) = 4 grid words = 16 elems, signs off the block's own bytes
                            let sv = ks4[blk]
                            let scw = js < 4u ? sv.x : sv.y
                            let dsc = float(ksh[blk]) * float(((int(scw >> ((js & 3u) * 8u)) & 255) ^ 128) - 128)
                            let qsw = kqu[blk * 26u + js * 2u + il0]
                            let qh3 = (kqu[blk * 26u + 16u + js / 4u] >> ((js % 4u) * 8u)) & 255u
                            let sgw3 = kqu[blk * 26u + 18u + js]
                            for [unroll_full] (k in range(4)) {
                                let b3 = il0 * 4u + uint(k)
                                let idx = ((qsw >> (8u * uint(k))) & 255u) | ((qh3 << (8u - b3)) & 256u)
                                let nib = (sgw3 >> ((b3 / 2u) * 8u + (b3 % 2u) * 4u)) & 15u
                                let sw = iq3s_sw(gridw3[idx], nib)
                                for [unroll_full] (c in range(4)) {
                                    va[k * 4 + c] = float16(dsc * iq3s_sx(sw, uint(c)))
                                }
                            }
                        } else {
                            static_if (IQ4XS) {
                                // iq4xs: strip byte js is the SIGNED sub-scale, super-d off the d plane (the k6 split
                                // form); block js = 4 uints of the q40 pairing, il0 picks the nibble half; the codebook
                                // off threadgroup memory. iq4nl: same walk, dsc = block js's own f16 d
                                var dsc = 0.0
                                static_if (IQ4NL) {
                                    dsc = float(ksh[blk * 8u + js])
                                } else {
                                    let sv = ks4[blk]
                                    let scw = js < 4u ? sv.x : sv.y
                                    dsc = float(ksh[blk]) * float(((int(scw >> ((js & 3u) * 8u)) & 255) ^ 128) - 128)
                                }
                                let qb = blk * 32u + js * 4u
                                let nsh = il0 * 4u
                                for [unroll_full] (k in range(4)) {
                                    let u = kqu[qb + uint(k)]
                                    for [unroll_full] (c in range(4)) {
                                        va[k * 4 + c] = float16(dsc * iq4lut[(u >> (8u * uint(c) + nsh)) & 15u])
                                    }
                                }
                            } else {
                                static_if (K2) {
                                    // q2_K: pair byte si = the group's sc|mn nibbles; d/dmin off the split tail
                                    let hf3 = js / 4u
                                    let gg3 = js % 4u
                                    let sv = ks4[blk]
                                    let si = hf3 * 8u + gg3 * 2u + il0
                                    let pw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
                                    let pb = (pw >> ((si & 3u) * 8u)) & 255u
                                    let dsc = float(ksh[blk * 2u]) * float(pb & 15u)
                                    let dmn = float(ksh[blk * 2u + 1u]) * float(pb >> 4u)
                                    let dsc1 = dsc * 0.00390625
                                    let dsc2 = dsc * 0.0000152587890625
                                    let dsc3 = dsc * 0.000000059604644775390625
                                    let qb = blk * 16u + hf3 * 8u + il0 * 4u
                                    let nsh = gg3 * 2u
                                    for [unroll_full] (k in range(4)) {
                                        let u = kqu[qb + uint(k)]
                                        let qv = (u >> nsh) & 0x03030303
                                        va[k * 4] = float16(dsc * float(qv & 255u) - dmn)
                                        va[k * 4 + 1] = float16(dsc1 * float(qv & 65280u) - dmn)
                                        va[k * 4 + 2] = float16(dsc2 * float(qv & 16711680u) - dmn)
                                        va[k * 4 + 3] = float16(dsc3 * float(qv & 4278190080u) - dmn)
                                    }
                                } else {
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
                                let qb = blk * BLK + (js / 2u) * 8u + il0 * 4u
                                static_if (QH) {
                                    let hb = blk * BLK + 32u + il0 * 4u
                                    let nsh = (js & 1u) * 4u
                                    for [unroll_full] (k in range(4)) {
                                        let u = kqu[qb + uint(k)]
                                        let hu = kqu[hb + uint(k)]
                                        for [unroll_full] (c in range(4)) {
                                            let q = ((u >> (8u * uint(c) + nsh)) & 15u) | (((hu >> (8u * uint(c) + js)) & 1u) << 4u)
                                            va[k * 4 + c] = float16(dsc * float(q) - dmn)
                                        }
                                    }
                                } else {
                                    let nsh = (js & 1u) * 4u
                                    for [unroll_full] (k in range(4)) {
                                        let u = kqu[qb + uint(k)]
                                        for [unroll_full] (c in range(4)) {
                                            let q = (u >> (8u * uint(c) + nsh)) & 15u
                                            va[k * 4 + c] = float16(dsc * float(q) - dmn)
                                        }
                                    }
                                }
                            }
                        }
                        }
                    }
                    }
                    }
                    }
                }
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            acc_quad(aB, bB, ma, mb, mc)
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

[metal_dispatch(name = "enc_kq_mm_k4_c", pso = "g_pso_kq_mm_b4", tgmem = "MetalKqMulMmK4_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmK4 : MetalKqMulMmK45T {
    override BLK = 32u
    override QH = false
}

[metal_dispatch(name = "enc_kq_mm_k5_c", pso = "g_pso_kq_mm_b5", tgmem = "MetalKqMulMmK5_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmK5 : MetalKqMulMmK45T {
    override BLK = 40u
    override QH = true
}

// q6_K rides the SIXBIT arm; scalars reload per kb — the cached form cost an occupancy tier
// (896 vs 1024 max_threads; bench_metal_kq_mm_lab measured -1.7..-10% per shape).
[metal_dispatch(name = "enc_kq_mm_k6_c", pso = "g_pso_kq_mm_b6", tgmem = "MetalKqMulMmK6_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmK6 : MetalKqMulMmK45T {
    override SIXBIT = true
}

// iq4xs rides the IQ4XS arm; the split scale plane binds twice like k6 (d plane at s0off = doff)
[metal_dispatch(name = "enc_kq_mm_iq4xs_c", pso = "g_pso_kq_mm_b44", tgmem = "MetalKqMulMmIq4xs_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmIq4xs : MetalKqMulMmK45T {
    override IQ4XS = true
}

// q3_K rides the K3 arm; the split scale plane binds twice like k6 (d plane at s0off = doff)
[metal_dispatch(name = "enc_kq_mm_k3_c", pso = "g_pso_kq_mm_b3", tgmem = "MetalKqMulMmK3_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmK3 : MetalKqMulMmK45T {
    override K3 = true
}

// iq3s rides the IQ3S arm; the split scale plane binds twice like k6 (d plane at s0off = doff)
[metal_dispatch(name = "enc_kq_mm_iq3s_c", pso = "g_pso_kq_mm_b33", tgmem = "MetalKqMulMmIq3s_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmIq3s : MetalKqMulMmK45T {
    override IQ3S = true
}

[metal_dispatch(name = "enc_kq_mm_iq3xxs_c", pso = "g_pso_kq_mm_b34", tgmem = "MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmIq3xxs : MetalKqMulMmK45T {
    override IQ3XXS = true
}

// q2_K rides its own K2 arm; the split scale plane binds twice like k6 (the 4B tail at s0off = doff)
[metal_dispatch(name = "enc_kq_mm_k2_c", pso = "g_pso_kq_mm_b2", tgmem = "MetalKqMulMmK2_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmK2 : MetalKqMulMmK45T {
    override K2 = true
}

// iq2s rides its own IQ2S arm; the split scale plane binds twice like k6 (d8 plane at s0off = doff)
[metal_dispatch(name = "enc_kq_mm_iq2s_c", pso = "g_pso_kq_mm_b23", tgmem = "MetalKqMulMmIq2s_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmIq2s : MetalKqMulMmK45T {
    override IQ2S = true
}

// iq2xs rides its own IQ2XS arm; the same split scale plane binds (d8 plane at s0off = doff)
[metal_dispatch(name = "enc_kq_mm_iq2xs_c", pso = "g_pso_kq_mm_b24", tgmem = "MetalKqMulMmIq2xs_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmIq2xs : MetalKqMulMmK45T {
    override IQ2XS = true
}

// iq2xxs rides its own IQ2XXS arm; the same split scale plane binds (d8 plane at s0off = doff)
[metal_dispatch(name = "enc_kq_mm_iq2xxs_c", pso = "g_pso_kq_mm_b25", tgmem = "MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmIq2xxs : MetalKqMulMmK45T {
    override IQ2XXS = true
}

// iq4nl rides the IQ4XS arm with IQ4NL flipping the scale read to q40's per-32-block d plane
[metal_dispatch(name = "enc_kq_mm_iq4nl_c", pso = "g_pso_kq_mm_b45", tgmem = "MetalKqMulMmIq4nl_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmIq4nl : MetalKqMulMmK45T {
    override IQ4XS = true
    override IQ4NL = true
}

// Router GEMV over the uploaded fp32 router slab: y[ne] = W[ne x dim] . x per stream. ne <= 256
// rows. Each sg serves NR positions per weight load (slab SLC traffic /NR); BATCHED stamps
// clamp tails via ns; singles fold the clamp away and the gate erases ns outright.
[ |> template_struct_instance]
class template MetalMoeRouterT {
    @ssbo @binding = 0 @role = "weight" w : array<float4>    // [ne x dim] router weights, float4 view
    @ssbo @binding = 1 x : array<float4>    // [streams x dim] activations, float4 view
    @ssbo @binding = 2 @span = "ne*nst*4" y : array<float>     // [streams x ne] logits out
    @uniform @binding = 3 ndim : uint       // dim (% 32)
    @uniform @binding = 4 ddim : uint       // ne
    @uniform @binding = 5 xs : uint         // per-stream x stride, elements
    @ssbo @binding = 6 @role = "weight" rb : array<float>    // [ne] router bias row (hasb models — gpt-oss)
    @uniform @binding = 7 hasb : uint
    @uniform @binding = 8 @template_gate = BATCHED ns : uint    // stream count (npos) — the BATCHED tail clamp; gated out of singles
    @template_constant NR : int = 1
    @template_constant NRU : uint = 1u      // NR's uint spelling (st0 math) — override BOTH together
    @template_constant BATCHED : bool = false

    [metal_kernel]
    def metal_moe_router {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let st0 = gl_WorkGroupID.y * NRU
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let xs4 = xs / 4u
        var acc : float[NR]
        var k = lane
        while (k < nb4) {
            let wv = w[row * nb4 + k]
            // arms duplicated (the TILED precedent): the gated ns is only referable from a
            // BATCHED static_if arm — infer sees both TERNARY arms, and the single stamp no
            // longer has the field. Both stamps keep the pre-gate MSL byte-for-byte.
            for [unroll_full] (i in range(NR)) {
                static_if (BATCHED) {
                    let st = min(st0 + uint(i), ns - 1u)
                    let xv = x[st * xs4 + k]
                    acc[i] += wv.x * xv.x + wv.y * xv.y + wv.z * xv.z + wv.w * xv.w
                } else {
                    let st = st0
                    let xv = x[st * xs4 + k]
                    acc[i] += wv.x * xv.x + wv.y * xv.y + wv.z * xv.z + wv.w * xv.w
                }
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (i in range(NR)) {
            let s = simd_sum(acc[i])
            static_if (BATCHED) {
                if (lane == 0u && st0 + uint(i) < ns) {
                    y[(st0 + uint(i)) * ddim + row] = hasb != 0u ? s + rb[row] : s
                }
            } else {
                if (lane == 0u) {
                    y[(st0 + uint(i)) * ddim + row] = hasb != 0u ? s + rb[row] : s
                }
            }
        }
    }
}

// grid.y = stream (batch decode; single = 1). Latency-trivial next to the expert GEMVs.
[metal_dispatch(name = "enc_moe_router", pso = "g_pso_moe_router", tg = "g_gemv_rows*32", grid = "ne/g_gemv_rows, nst", params = "ne : int64, nst : int64")]
class MetalMoeRouter : MetalMoeRouterT {
}

// Position-blocked prefill twin: 8 positions per sg (the prefill router's entire cost is slab
// traffic). Same per-lane k stride and per-position dot order as the single: logits stay
// bit-identical. Also the prefill f32-slab GEMV (pf_enc_slab_gemv_b rides this builder).
[metal_dispatch(name = "enc_moe_router_b", pso = "g_pso_moe_router_b", tg = 128, grid = "ne/4, nst/8", params = "ne : int64, nst : int64")]
class MetalMoeRouterB : MetalMoeRouterT {
    override NR = 8
    override NRU = 8u
    override BATCHED = true
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
    def metal_moe_select {   // nolint:STYLE037,STYLE038 — MSL kernel body; the per-lane logit ladder is register-resident
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
[metal_dispatch(name = "enc_moe_gemv_q8_c", pso = "g_pso_moe_gemv_q8", tg = "g_gemv_rows*32", grid = "rows/g_gemv_rows, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvQ8 {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wsh : array<float16> // the expert stack's blob slice, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "boff" wqb : array<int8>    // the same blob buffer, byte view
    @ssbo @binding = 2 @role = "read" x : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    @uniform @binding = 4 ka : MoeGemvArgs
    @ssbo @binding = 6 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    [metal_kernel(name="metal_moe_gemv_q8_msl")]
    def metal_moe_gemv_q8 {
        let ndim = ka.ndim
        let ddim = ka.ddim
        let eblk = ka.eplane
        let xs = ka.xs
        let ss = ka.ss
        let bxs = ka.bxs
        let bys = ka.bys
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

// q4_K/q5_K kmask halfword-pair scale decode — sc/min byte pairs for both 32-halves of the
// lane's superblock quadrant (x = sc lo-pair, y = min lo-pair, z = sc hi-pair, w = min hi-pair).
def private kmask_scales(sv : uint4; iq : uint) : uint4 {
    let hw0 = (sv.y >> (16u * iq)) & 0xFFFF
    let hw2 = (sv.z >> (16u * iq)) & 0xFFFF
    let hw4 = (sv.w >> (16u * iq)) & 0xFFFF
    return uint4(hw0 & 0x3F3F, hw2 & 0x3F3F,
        (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u),
        ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u))
}

// IQ4_XS codebook (ggml's kvalues_iq4nl) as four packed words, selected + byte-extracted —
// no table memory; the k6 sign trick widens the byte
def private iq4_lut(q : uint) : float {
    let w = q < 8u ? (q < 4u ? 3215825025u : 4142587343u) : (q < 12u ? 639175937u : 1901675829u)
    return float((int((w >> ((q & 3u) * 8u)) & 255u) ^ 128) - 128)
}

// ggml's iq3s_grid, one word per call - the GEMV/mul_mm stage it into threadgroup memory once
// per group; the mv twins read it directly (unmeasured batch shapes - followup #58's stance).
def private iq3s_gw(i : int) : uint {
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

// one signed grid word: the 4-bit sign nibble expands to a byte mask; magnitudes are odd
// 1..15, so the negate's +1 never carries across a byte lane
def private iq3s_sw(g, nib : uint) : uint {
    let m1 = (nib * 0x00204081u) & 0x01010101u
    return (g ^ (m1 * 255u)) + m1
}

def private iq3s_sx(sw, t : uint) : float {
    return float((int((sw >> (t * 8u)) & 255u) ^ 128) - 128)
}

// the HALVED iq3xxs grid (ggml's iq3xxs_grid, every byte / 2 - the plane convention), one
// word per call; the GEMV and mul_mm stage it, the mv twins read it directly.
def private iq3xxs_gw(i : int) : uint {
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

// ksigns_iq2xs without the table: bit 7 = the even parity of the 7-bit index.
def private ksign7m(v : uint) : uint {
    var t = v ^ (v >> 4u)
    t = t ^ (t >> 2u)
    t = t ^ (t >> 1u)
    return v | ((t & 1u) << 7u)
}

// ggml's iq2xxs_grid as low/high word pairs (word 2i = magnitudes 0..3 of u64 entry i, word
// 2i+1 = 4..7); the same all-literal program-scope-constant form as its grid siblings.
def private iq2xxs_gw(i : int) : uint {
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

// ggml's iq2xs_grid as low/high word pairs (word 2i = magnitudes 0..3 of u64 entry i, word
// 2i+1 = 4..7); the same all-literal program-scope-constant form as iq2s_gw below.
def private iq2xs_gw(i : int) : uint {   // nolint:STYLE038 - a 1024-word data table, not splittable
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

// ggml's iq2s_grid as low/high word pairs (word 2i = magnitudes 0..3 of u64 entry i, word 2i+1
// = 4..7); an all-literal fixed_array local, so it lowers to a program-scope constant table -
// every kernel reads it DIRECT (an f4 slab of 1024 entries would be 32 KB, past the tg budget).
def private iq2s_gw(i : int) : uint {   // nolint:STYLE038 - a 2048-word data table, not splittable
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

// Expert-indexed Q4_K GEMV — MetalKqGemvK4's dot; the superblock index shifts by sel[slot] * esb.
// x rides a float4 view: the 32-scalar-x-load form was load-issue-bound at the routed expert
// shapes (nfe-row planes) — the vector view is 2.25x there (lab: bench_metal_moe_lab, 142→321 wGB/s).
[metal_dispatch(name = "enc_moe_gemv_k4_c", pso = "g_pso_moe_kq_k4", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvK4 {
    @ssbo @binding = 0 @role = "weight" @off = "soff" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 @role = "read" x4 : array<float4>   // activations, float4 view (every x stride is % 4)
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    @uniform @binding = 5 ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    [metal_kernel(name="metal_moe_gemv_k4_msl")]
    def metal_moe_gemv_k4 {   // nolint:STYLE038 — MSL kernel body; the gathered GEMV phases share tgmem state
        let ndim = ka.ndim
        let ddim = ka.ddim
        let esb = ka.eplane
        let xs = ka.xs
        let ss = ka.ss
        let bxs = ka.bxs
        let bys = ka.bys
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
                let scs = kmask_scales(ks4[blk], iq)
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
                sumf[r] += dd * ((acc1[0] + 0.00390625 * acc1[1]) * float(scs.x & 255u)
                    + (acc1[2] + 0.00390625 * acc1[3]) * float(scs.x >> 8u) * 0.0625
                    + (acc2[0] + 0.00390625 * acc2[1]) * float(scs.z & 255u)
                    + (acc2[2] + 0.00390625 * acc2[3]) * float(scs.z >> 8u) * 0.0625)
                sumf[r] -= dm * (sumy[0] * float(scs.y & 255u) + sumy[1] * float(scs.y >> 8u)
                    + sumy[2] * float(scs.w & 255u) + sumy[3] * float(scs.w >> 8u))
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
[metal_dispatch(name = "enc_moe_gemv_k5_c", pso = "g_pso_moe_kq_k5", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvK5 {
    @ssbo @binding = 0 @role = "weight" @off = "soff" ksh : array<float16> // 16B scale blocks, half view
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    @uniform @binding = 5 ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    [metal_kernel(name="metal_moe_gemv_k5_msl")]
    def metal_moe_gemv_k5 {
        let ndim = ka.ndim
        let ddim = ka.ddim
        let esb = ka.eplane
        let xs = ka.xs
        let ss = ka.ss
        let bxs = ka.bxs
        let bys = ka.bys
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
                let scs = kmask_scales(ks4[blk], iq)
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
                sumf[r] += dd * (float(scs.x & 255u) * acc[0] + float(scs.x >> 8u) * acc[1]
                    + float(scs.z & 255u) * acc[2] + float(scs.z >> 8u) * acc[3])
                sumf[r] -= dm * (sumy[0] * float(scs.y & 255u) + sumy[1] * float(scs.y >> 8u)
                    + sumy[2] * float(scs.w & 255u) + sumy[3] * float(scs.w >> 8u))
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
[metal_dispatch(name = "enc_moe_gemv_k6_c", pso = "g_pso_moe_kq_k6", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvK6 {
    @ssbo @binding = 0 @role = "weight" @off = "doff" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k6 quant plane, byte view (ql at 192*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    @uniform @binding = 5 ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    [metal_kernel(name="metal_moe_gemv_k6_msl")]
    def metal_moe_gemv_k6 {
        let ndim = ka.ndim
        let ddim = ka.ddim
        let esb = ka.eplane
        let xs = ka.xs
        let ss = ka.ss
        let bxs = ka.bxs
        let bys = ka.bys
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
[metal_dispatch(name = "enc_moe_gemv_mx4_c", pso = "g_pso_moe_gemv_mx4", tgmem = "metal_moe_gemv_mx4_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvMx4 {
    @ssbo @binding = 0 @role = "weight" @off = "qoff" mxu2 : array<uint2>  // nibble plane, uint2 view (block at 2*blk; off/2 % 8)
    @ssbo @binding = 1 @role = "weight" @off = "soff" mxe : array<uint8>   // E8M0 plane, bound at off/32
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    @uniform @binding = 4 ka : MoeGemvArgs
    @ssbo @binding = 6 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @ssbo @binding = 12 @role = "weight" wb : array<float>   // [ne x ddim] per-expert bias rows (hasb models)
    @ssbo @binding = 15 @role = "weight" vtab : array<float> // [16] signed doubled-e2m1 values (host-built)
    @workgroup vt : float[32]

    [metal_kernel(name="metal_moe_gemv_mx4_msl")]
    def metal_moe_gemv_mx4 {
        let ndim = ka.ndim
        let ddim = ka.ddim
        let eblk = ka.eplane
        let xs = ka.xs
        let ss = ka.ss
        let bxs = ka.bxs
        let bys = ka.bys
        let hasb = ka.hasb
        // the wide-block MXFP4 GEMV shape (moe-lab lcppe rung, 239->301 wGB/s at gpt-oss shapes):
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
[metal_dispatch(name = "enc_moe_gemv_q51_c", pso = "g_pso_moe_gemv_q51", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvQ51 {
    @ssbo @binding = 0 @role = "weight" @off = "qoff" wqu : array<uint>    // quant plane, uint view (block at 5*blk; qh at +4)
    @ssbo @binding = 1 @role = "weight" @off = "soff" wsh : array<float16> // scale plane (d at 2*blk, m at 2*blk+1)
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    @uniform @binding = 4 ka : MoeGemvArgs
    @ssbo @binding = 6 @role = "read" sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    [metal_kernel(name="metal_moe_gemv_q51_msl")]
    def metal_moe_gemv_q51 {
        let ndim = ka.ndim
        let ddim = ka.ddim
        let eblk = ka.eplane
        let xs = ka.xs
        let ss = ka.ss
        let bxs = ka.bxs
        let bys = ka.bys
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
// SHARED with decode's enc_ew2 (slots 0-2, keep) — the prefill twin's PF fields append at 3-6.
[ |> template_struct_instance]
class template MetalSwigluOaiT {
    @ssbo @binding = 0 @role = "readwrite" @off = "goff" @span = "total*4" g : array<float4>
    @ssbo @binding = 1 @role = "read" @off = "uoff" @span = "total*4" u : array<float4>
    @uniform @binding = 2 total : uint      // scalar element count
    @ssbo @binding = 3 @role = "read" @template_gate = PF cnt : array<uint>
    @ssbo @binding = 4 @role = "read" @template_gate = PF basep : array<uint>
    @uniform @binding = 5 @template_gate = PF ne : uint
    @uniform @binding = 6 @template_gate = PF nfe : uint    // columns per bucket row (n_ff_exp)
    @template_constant PF : bool = false

    [metal_kernel]
    def metal_swiglu_oai {
        let gid = gl_GlobalInvocationID.x
        if (gid * 4u >= total) {
            return
        }
        static_if (PF) {
            // ghost rows past the last bucket's padded end (mpad worst-case vs the GPU-side true
            // count) exit on a uniform-broadcast liveness bound — never read by the down mm
            let live = (basep[ne - 1u] + (cnt[ne - 1u] + 31u) / 32u * 32u) * nfe
            if (gid * 4u >= live) {
                return
            }
        }
        let x = min(g[gid], float4(7.0))
        let y = min(max(u[gid], float4(-7.0)), float4(7.0))   // nolint:STYLE019 — the MSL emitter doesn't lower clamp
        g[gid] = (x / (float4(1.0) + exp(-1.702 * x))) * (y + float4(1.0))
    }
}

[metal_dispatch(name = "enc_swiglu_oai", pso = "g_pso_swiglu_oai", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSwigluOai : MetalSwigluOaiT {
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

// One simdgroup per output row over the 34B q8 blob (f16 scale + 32 int8 inline, dual-viewed),
// 4 blocks in flight — now the DnBa narrow-projection form only (d = dt_rank 16..64, where the
// 2-rows/tg MetalQ8Gemv would starve the grid); BATCHED adds the grid-y position axis.
[ |> template_struct_instance]
class template MetalQ8GemvT {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" @off = "boff" wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 x : array<float4>    // [n] f32 vector, float4 view ([npos x n] when BATCHED)
    @ssbo @binding = 3 @off = "yoff" @span = "d*np*4" y : array<float>     // [d] outputs ([np x d] when BATCHED; singles pass np = 1)
    @uniform @binding = 4 ndim : uint       // n (reduction dim, % 32)
    @uniform @binding = 5 ddim : uint       // d (output rows)
    @template_constant BATCHED : bool = false

    [metal_kernel]
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
        var x0 = unsafe(addr(x[BATCHED ? gl_WorkGroupID.y * nb4 + lane : lane]))
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
            let xv = x[BATCHED ? gl_WorkGroupID.y * nb4 + k : k]
            acc += (float(wqb[qb]) * xv.x + float(wqb[qb + 1u]) * xv.y +
                float(wqb[qb + 2u]) * xv.z + float(wqb[qb + 3u]) * xv.w) * d
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[BATCHED ? gl_WorkGroupID.y * ddim + row : row] = acc
        }
    }
}

// Reduction-split GEMV (2 rows/tg, 4 simdgroups, 8 quants/thread/round): the n-walk splits
// ACROSS the sgs (n/4 x-window per sg stays L1-resident at every decode dim) and one x load
// feeds both rows — the 1-row/sg full-n walk it replaces spills x past L1 from n=4096.
[metal_dispatch(name = "enc_gemv_c", pso = "g_pso_gemv", tgmem = "MetalQ8Gemv_metal_q8_gemv_msl_tgmem", tg = 128, grid = "d/2", params = "d : int64, np : int64")]
class MetalQ8Gemv {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" @off = "boff" wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "read" @off = "xoff" x : array<float4>    // [n] f32 vector, float4 view
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "d*np*4" y : array<float>     // [d] outputs (np = 1 on this form)
    @uniform @binding = 4 ndim : uint       // n (reduction dim, % 32)
    @uniform @binding = 5 ddim : uint       // d (output rows)
    @workgroup red : float[64]              // [2 rows][32 lanes] cross-sg partials

    [metal_kernel]
    def metal_q8_gemv {
        let sg = gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let first_row = gl_WorkGroupID.x * 2u
        let ix = lane / 4u                  // block-within-round: 8 blocks per sg per round
        let il = lane % 4u                  // 8-quant chunk within the block
        var sumf : float[2]
        var ib = sg * 8u + ix
        while (ib < nb) {
            let xi = ib * 8u + il * 2u      // the x chunk once per round, shared by both rows
            let x0 = x[xi]
            let x1 = x[xi + 1u]
            for [unroll_full] (r in range(2)) {
                let bi = (first_row + uint(r)) * nb + ib
                let d = float(wsh[bi * 17u])
                let qb = bi * 34u + 2u + il * 8u
                sumf[r] += (float(wqb[qb]) * x0.x + float(wqb[qb + 1u]) * x0.y +
                    float(wqb[qb + 2u]) * x0.z + float(wqb[qb + 3u]) * x0.w +
                    float(wqb[qb + 4u]) * x1.x + float(wqb[qb + 5u]) * x1.y +
                    float(wqb[qb + 6u]) * x1.z + float(wqb[qb + 7u]) * x1.w) * d
            }
            ib += 32u                       // NSG*NQ blocks per round
        }
        for [unroll_full] (r in range(2)) {
            if (sg == 0u) {
                red[uint(r) * 32u + lane] = 0.0
            }
            sumf[r] = simd_sum(sumf[r])
        }
        barrier()
        for [unroll_full] (r in range(2)) {
            if (lane == 0u) {
                red[uint(r) * 32u + sg] = sumf[r]
            }
        }
        barrier()
        for [unroll_full] (r in range(2)) {
            let tot = simd_sum(red[uint(r) * 32u + lane])
            if (lane == 0u && sg == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = tot
            }
        }
    }
}

// Batched narrow projection twin (DeltaNet β/α: d = dt_rank, 16..64 — under every GEMM N tile):
// one simdgroup per (output row, position); 256/tg = 8 rows per threadgroup.
[metal_dispatch(name = "pf_enc_dn_ba", pso = "g_pso_dn_ba", tg = 256, grid = "d/8, np", params = "d : int64, np : int64")]
class MetalDnBa : MetalQ8GemvT {
    override BATCHED = true
}

// Q4_K: lane owns 32 elems of every 4th superblock; the kmask halfword scale decode +
// nibble-mask acc folds (x16/x256 groups folded by pow2 constants).
[metal_dispatch(name = "enc_kq_k4_c", pso = "g_pso_kq_k4", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK4 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
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
                let scs = kmask_scales(ks4[blk], iq)
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
                sumf[r] += dd * ((acc1[0] + 0.00390625 * acc1[1]) * float(scs.x & 255u)
                    + (acc1[2] + 0.00390625 * acc1[3]) * float(scs.x >> 8u) * 0.0625
                    + (acc2[0] + 0.00390625 * acc2[1]) * float(scs.z & 255u)
                    + (acc2[2] + 0.00390625 * acc2[3]) * float(scs.z >> 8u) * 0.0625)
                sumf[r] -= dm * (sumy[0] * float(scs.y & 255u) + sumy[1] * float(scs.y >> 8u)
                    + sumy[2] * float(scs.w & 255u) + sumy[3] * float(scs.w >> 8u))
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
[metal_dispatch(name = "enc_kq_k5_c", pso = "g_pso_kq_k5", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK5 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
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
                let scs = kmask_scales(ks4[blk], iq)
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
                sumf[r] += dd * (float(scs.x & 255u) * acc[0] + float(scs.x >> 8u) * acc[1]
                    + float(scs.z & 255u) * acc[2] + float(scs.z >> 8u) * acc[3])
                sumf[r] -= dm * (sumy[0] * float(scs.y & 255u) + sumy[1] * float(scs.y >> 8u)
                    + sumy[2] * float(scs.w & 255u) + sumy[3] * float(scs.w >> 8u))
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

// Q5_K, select-form nr0=1 twin (the gemv-lab b1c winner): one row per simdgroup, the q5 lane
// map, qh via mask-selects. Wins at n < 3072 at EVERY d (-9..-11% vs b2r) and the small-d kv
// shape (b2r under-occupied); b2r keeps n >= 3072. enc_kq_gemv picks per site.
[metal_dispatch(name = "enc_kq_k5c_c", pso = "g_pso_kq_k5c", tg = 64, grid = "rows/2", params = "rows : int64, n : int64")]
class MetalKqGemvK5C {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
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
            let scs = kmask_scales(ks4[blk], iq)
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
            sumf += dd * (float(scs.x & 255u) * (acc1[0] + 16.0 * acc2[0])
                + float(scs.x >> 8u) * (acc1[1] * 0.0625 + 16.0 * acc2[1])
                + float(scs.z & 255u) * (acc1[2] + 16.0 * acc2[2])
                + float(scs.z >> 8u) * (acc1[3] * 0.0625 + 16.0 * acc2[3]))
            sumf -= dm * (sumy[0] * float(scs.y & 255u) + sumy[1] * float(scs.y >> 8u)
                + sumy[2] * float(scs.w & 255u) + sumy[3] * float(scs.w >> 8u))
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
[metal_dispatch(name = "enc_kq_k6_c", pso = "g_pso_kq_k6", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK6 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k6 quant plane, byte view (ql at 192*blk, qh at +128)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
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

// Q2_K: the k3 GEMV's lane map minus the hmask - unsigned 2-bit lanes, per-16 sc/min NIBBLE
// pairs off the split strips (sc scales the dot, mn folds on the lane's x sums against dmin),
// d + dmin off the 4B tail plane.
[metal_dispatch(name = "enc_kq_k2_c", pso = "g_pso_kq_k2", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK2 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d+dmin tail - the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B pair-byte strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k2 quant plane, byte view (qs at 64*blk)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k2_msl")]
    def metal_kq_gemv_k2 {
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
            var ysum : float[4]
            for [unroll_full] (l in range(4)) {
                yl[4 * l] = x[yb + uint(l)]
                yl[4 * l + 1] = x[yb + 32u + uint(l)]
                yl[4 * l + 2] = x[yb + 64u + uint(l)]
                yl[4 * l + 3] = x[yb + 96u + uint(l)]
                ysum[0] += yl[4 * l]
                ysum[1] += yl[4 * l + 1]
                ysum[2] += yl[4 * l + 2]
                ysum[3] += yl[4 * l + 3]
            }
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let qs0 = blk * 64u + ip * 32u + l0
                let sb = blk * 16u + is0
                var sums : float[4]
                for [unroll_full] (l in range(4)) {
                    let q = uint(kqb[qs0 + uint(l)])
                    sums[0] += yl[4 * l] * float(q & 3u)
                    sums[1] += yl[4 * l + 1] * float((q >> 2u) & 3u)
                    sums[2] += yl[4 * l + 2] * float((q >> 4u) & 3u)
                    sums[3] += yl[4 * l + 3] * float(q >> 6u)
                }
                let b0 = uint(kscb[sb])
                let b2 = uint(kscb[sb + 2u])
                let b4 = uint(kscb[sb + 4u])
                let b6 = uint(kscb[sb + 6u])
                let dd = float(kdh[blk * 2u])
                let dmn = float(kdh[blk * 2u + 1u])
                sumf[r] += dd * (sums[0] * float(b0 & 15u) + sums[1] * float(b2 & 15u) + sums[2] * float(b4 & 15u) + sums[3] * float(b6 & 15u))
                sumf[r] -= dmn * (ysum[0] * float(b0 >> 4u) + ysum[1] * float(b2 >> 4u) + ysum[2] * float(b4 >> 4u) + ysum[3] * float(b6 >> 4u))
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

// The k2 B2/B4 pair: the K3T shell minus the hmask - w = q*d*sc - dmin*mn per element, the
// pair byte picked per (slot, half).
[ |> template_struct_instance]
class template MetalKqMvK2T {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d+dmin tail - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B pair-byte strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k2 quant plane, uint view (16 qs words per superblock)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_k2 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp — measured free
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let shr = 2u * s
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dd = float(kdh[blk * 2u])
            let dmn = float(kdh[blk * 2u + 1u])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 16u + ip * 8u + o4
                let g = ip * 8u + s * 2u + (tx % 2u)
                let pb = uint(kscb[blk * 16u + g])
                let dl = dd * float(pb & 15u)
                let ml = dmn * float(pb >> 4u)
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qu = kqu[qb + uint(u)]
                    let qv = (qu >> shr) & 0x03030303
                    for [unroll_full] (e in range(4)) {
                        w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                    }
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                static_if (TILED) {
                    for [unroll_full] (b in range(NR)) {
                        let xb4 = (colbase + uint(b)) * nb4 + cx4
                        var acc = 0.0
                        for [unroll_full] (u in range(4)) {
                            let v = x[xb4 + uint(u)]
                            acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                        }
                        sumf[b] += acc
                    }
                } else {
                    for [unroll_full] (b in range(NR)) {
                        let xb4 = uint(b) * nb4 + cx4
                        var acc = 0.0
                        for [unroll_full] (u in range(4)) {
                            let v = x[xb4 + uint(u)]
                            acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                        }
                        sumf[b] += acc
                    }
                }
            }
            sb++
        }
        static_if (TILED) {
            for [unroll_full] (b in range(NR)) {
                var sm = sumf[b]
                sm += simd_shuffle_down(sm, 4u)
                sm += simd_shuffle_down(sm, 2u)
                sm += simd_shuffle_down(sm, 1u)
                if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                    y[(colbase + uint(b)) * ka.ys + row] = sm
                }
            }
        } else {
            for [unroll_full] (b in range(NR)) {
                var sm = sumf[b]
                sm += simd_shuffle_down(sm, 4u)
                sm += simd_shuffle_down(sm, 2u)
                sm += simd_shuffle_down(sm, 1u)
                if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                    y[uint(b) * ka.ys + row] = sm
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k2_c", pso = "g_pso_kq_mvb2_k2", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2K2 : MetalKqMvK2T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_k2_c", pso = "g_pso_kq_mvb4_k2", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4K2 : MetalKqMvK2T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the k2 B8 twin: the k3 B8 shell minus the hmask, per-element w = q*d*sc - dmin*mn
[metal_dispatch(name = "enc_kq_mvb8_k2_c", pso = "g_pso_kq_mvb8_k2", tgmem = "metal_kq_mvb8_k2_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8K2 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d+dmin tail - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B pair-byte strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k2 quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]

    [metal_kernel(name="metal_kq_mvb8_k2_msl")]
    def metal_kq_mvb8_k2 {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let shr = 2u * s
        let nb4 = ka.ndim / 4u
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
            let dd = float(kdh[blk * 2u])
            let dmn = float(kdh[blk * 2u + 1u])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 16u + ip * 8u + o4
                let g = ip * 8u + s * 2u + (tx % 2u)
                let pb = uint(kscb[blk * 16u + g])
                let dl = dd * float(pb & 15u)
                let ml = dmn * float(pb >> 4u)
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvc = (kqu[qb + uint(u)] >> shr) & 0x03030303
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
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = sm
            }
        }
    }
}

// Q3_K: k6's lane map (a lane's 4 elements of each of a half's 4 blocks) over the k3 planes — the
// 2-bit lane of qs byte ip*32 + l at shift 2j | hmask bit 4ip + j of byte l, minus 4; per-16 SIGNED
// sub-scales off the split strips, d off the d plane.
[metal_dispatch(name = "enc_kq_k3_c", pso = "g_pso_kq_k3", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK3 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k3 quant plane, byte view (qs at 96*blk, hmask at +64)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k3_msl")]
    def metal_kq_gemv_k3 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let is0 = ip * 8u + l0 / 16u
        let hb0 = ip * 4u
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
                let qs0 = blk * 96u + ip * 32u + l0
                let hm0 = blk * 96u + 64u + l0
                let sb = blk * 16u + is0
                var sums : float[4]
                for [unroll_full] (l in range(4)) {
                    let q = uint(kqb[qs0 + uint(l)])
                    let h = uint(kqb[hm0 + uint(l)]) >> hb0
                    sums[0] += yl[4 * l] * (float((q & 3u) | ((h & 1u) << 2u)) - 4.0)
                    sums[1] += yl[4 * l + 1] * (float(((q >> 2u) & 3u) | ((h & 2u) << 1u)) - 4.0)
                    sums[2] += yl[4 * l + 2] * (float(((q >> 4u) & 3u) | (h & 4u)) - 4.0)
                    sums[3] += yl[4 * l + 3] * (float((q >> 6u) | ((h & 8u) >> 1u)) - 4.0)
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

// The k3 B2/B4 pair: the K6T shell (slot s = block of the half, o4 = its 16-half) with the
// lane + mask compose off the 24-uint superblock (qs 0..15, hmask 16..23), offset 4.
[ |> template_struct_instance]
class template MetalKqMvK3T {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k3 quant plane, uint view (qs at 24*blk, hmask at 24*blk+16)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_k3 {   // nolint:STYLE038 — the TILED twin loops are byte-identity-driven, not splittable
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp — measured free
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let shr = 2u * s
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 24u + ip * 8u + o4
                let hb = blk * 24u + 16u + o4
                let hsh = ip * 4u + s
                let g = ip * 8u + s * 2u + (tx % 2u)
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 4.0
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qu = kqu[qb + uint(u)]
                    let hu = kqu[hb + uint(u)]
                    let qv = ((qu >> shr) & 0x03030303) | (((hu >> hsh) & 0x01010101) << 2u)
                    for [unroll_full] (e in range(4)) {
                        w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                    }
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                static_if (TILED) {
                    for [unroll_full] (b in range(NR)) {
                        let xb4 = (colbase + uint(b)) * nb4 + cx4
                        var acc = 0.0
                        for [unroll_full] (u in range(4)) {
                            let v = x[xb4 + uint(u)]
                            acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                        }
                        sumf[b] += acc
                    }
                } else {
                    for [unroll_full] (b in range(NR)) {
                        let xb4 = uint(b) * nb4 + cx4
                        var acc = 0.0
                        for [unroll_full] (u in range(4)) {
                            let v = x[xb4 + uint(u)]
                            acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                        }
                        sumf[b] += acc
                    }
                }
            }
            sb++
        }
        static_if (TILED) {
            for [unroll_full] (b in range(NR)) {
                var sm = sumf[b]
                sm += simd_shuffle_down(sm, 4u)
                sm += simd_shuffle_down(sm, 2u)
                sm += simd_shuffle_down(sm, 1u)
                if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                    y[(colbase + uint(b)) * ka.ys + row] = sm
                }
            }
        } else {
            for [unroll_full] (b in range(NR)) {
                var sm = sumf[b]
                sm += simd_shuffle_down(sm, 4u)
                sm += simd_shuffle_down(sm, 2u)
                sm += simd_shuffle_down(sm, 1u)
                if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                    y[uint(b) * ka.ys + row] = sm
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k3_c", pso = "g_pso_kq_mvb2_k3", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2K3 : MetalKqMvK3T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_k3_c", pso = "g_pso_kq_mvb4_k3", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4K3 : MetalKqMvK3T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the k3 B8 twin: the k6 B8 shell (one superblock's X panel staged per threadgroup) with the
// lane + mask compose
[metal_dispatch(name = "enc_kq_mvb8_k3_c", pso = "g_pso_kq_mvb8_k3", tgmem = "metal_kq_mvb8_k3_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8K3 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k3 quant plane, uint view (qs at 24*blk, hmask at 24*blk+16)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]

    [metal_kernel(name="metal_kq_mvb8_k3_msl")]
    def metal_kq_mvb8_k3 {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let shr = 2u * s
        let nb4 = ka.ndim / 4u
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
                let qb = blk * 24u + ip * 8u + o4
                let hb = blk * 24u + 16u + o4
                let hsh = ip * 4u + s
                var qu : uint[4]
                var hu : uint[4]
                for [unroll_full] (u in range(4)) {
                    qu[u] = kqu[qb + uint(u)]
                    hu[u] = kqu[hb + uint(u)]
                }
                let g = ip * 8u + s * 2u + (tx % 2u)
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 4.0
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvc = ((qu[u] >> shr) & 0x03030303) | (((hu[u] >> hsh) & 0x01010101) << 2u)
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
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = sm
            }
        }
    }
}

// IQ4_XS: w = d*sc*LUT[q], sc the SIGNED strip byte, no min plane; the codebook in threadgroup
// floats, a 16-lane half-simdgroup per superblock, lane (bu, il) = block bu's 8-byte half il
// (two uint loads, four float4 x loads; the q40 pairing). Scale plane = the k6 split form.
[metal_dispatch(name = "enc_kq_iq4xs_c", pso = "g_pso_kq_iq4xs", tgmem = "metal_kq_gemv_iq4xs_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq4xs {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq4xs quant plane, uint view (block bu of blk at 32*blk + 4*bu)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @workgroup lut : float[64]   // kvalues_iq4nl as floats, one slot per thread of the 64-wide group

    [metal_kernel(name="metal_kq_gemv_iq4xs_msl")]
    def metal_kq_gemv_iq4xs {
        let lid = gl_LocalInvocationID.x
        lut[lid] = iq4_lut(lid & 15u)
        barrier()
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb4 = ib * 64u + bu * 8u + il * 2u
            let y0 = x[yb4]
            let y1 = x[yb4 + 1u]
            let y2 = x[yb4 + 4u]
            let y3 = x[yb4 + 5u]
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let qb = blk * 32u + bu * 4u + il * 2u
                let q0 = kqu[qb]
                let q1 = kqu[qb + 1u]
                var acc = y0 * float4(lut[q0 & 15u], lut[(q0 >> 8u) & 15u], lut[(q0 >> 16u) & 15u], lut[(q0 >> 24u) & 15u])
                acc += y2 * float4(lut[(q0 >> 4u) & 15u], lut[(q0 >> 12u) & 15u], lut[(q0 >> 20u) & 15u], lut[q0 >> 28u])
                acc += y1 * float4(lut[q1 & 15u], lut[(q1 >> 8u) & 15u], lut[(q1 >> 16u) & 15u], lut[(q1 >> 24u) & 15u])
                acc += y3 * float4(lut[(q1 >> 4u) & 15u], lut[(q1 >> 12u) & 15u], lut[(q1 >> 20u) & 15u], lut[q1 >> 28u])
                let sc = float((int(kscb[blk * 16u + bu]) ^ 128) - 128)
                sumf[r] += float(kdh[blk]) * sc * (acc.x + acc.y + acc.z + acc.w)
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

// IQ4_NL: w = d*LUT[q] with a PER-32-BLOCK f16 d (q40's 16B/sb scale plane verbatim, one bind,
// no sub-scale, no split plane); the quant walk is the iq4xs GEMV's q40 pairing over the same
// threadgroup codebook. kscb keeps the shared kq binding shape (iq4nl has no strip bytes).
[metal_dispatch(name = "enc_kq_iq4nl_c", pso = "g_pso_kq_iq4nl", tgmem = "metal_kq_gemv_iq4nl_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq4nl {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the 16B/sb d plane, half view - block bu's d at blk*8+bu
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // the same plane, byte view (binding-shape twin; unused)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq4nl quant plane, uint view (q40's 128B/sb nibble tiling)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @workgroup lut : float[64]   // kvalues_iq4nl as floats, one slot per thread of the 64-wide group

    [metal_kernel(name="metal_kq_gemv_iq4nl_msl")]
    def metal_kq_gemv_iq4nl {
        let lid = gl_LocalInvocationID.x
        lut[lid] = iq4_lut(lid & 15u)
        barrier()
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb4 = ib * 64u + bu * 8u + il * 2u
            let y0 = x[yb4]
            let y1 = x[yb4 + 1u]
            let y2 = x[yb4 + 4u]
            let y3 = x[yb4 + 5u]
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let qb = blk * 32u + bu * 4u + il * 2u
                let q0 = kqu[qb]
                let q1 = kqu[qb + 1u]
                var acc = y0 * float4(lut[q0 & 15u], lut[(q0 >> 8u) & 15u], lut[(q0 >> 16u) & 15u], lut[(q0 >> 24u) & 15u])
                acc += y2 * float4(lut[(q0 >> 4u) & 15u], lut[(q0 >> 12u) & 15u], lut[(q0 >> 20u) & 15u], lut[q0 >> 28u])
                acc += y1 * float4(lut[q1 & 15u], lut[(q1 >> 8u) & 15u], lut[(q1 >> 16u) & 15u], lut[(q1 >> 24u) & 15u])
                acc += y3 * float4(lut[(q1 >> 4u) & 15u], lut[(q1 >> 12u) & 15u], lut[(q1 >> 20u) & 15u], lut[q1 >> 28u])
                sumf[r] += float(kdh[blk * 8u + bu]) * (acc.x + acc.y + acc.z + acc.w)
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

// IQ3_S: w = d*sc*(+-grid byte); the 2 KB grid in threadgroup memory (llama.cpp's shape), a
// 16-lane half-simdgroup per superblock, lane (bu, il) = block bu's half il: one qs word = 4
// grid words = 16 elems = 4 CONSECUTIVE float4 x loads. Scale plane = the iq4xs/k6 split form.
[metal_dispatch(name = "enc_kq_iq3s_c", pso = "g_pso_kq_iq3s", tgmem = "metal_kq_gemv_iq3s_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64, n : int64")]
class MetalKqGemvIq3s {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3s quant plane, uint view (26 words per superblock: 16 qs, 2 qh, 8 signs)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @workgroup gridf : float4[512]   // magnitudes pre-expanded at stage time: the inner loop reads one float4 and flips signs, no byte extracts (the best of the eight raced forms - llama.cpp's own geometry included)

    [metal_kernel(name="metal_kq_gemv_iq3s_msl")]
    def metal_kq_gemv_iq3s {
        let lid = gl_LocalInvocationID.x
        for [unroll_full] (k in range(8)) {
            let gi = lid + uint(k) * 64u
            let gw = iq3s_gw(int(gi))
            gridf[gi] = float4(float(gw & 255u), float((gw >> 8u) & 255u), float((gw >> 16u) & 255u), float(gw >> 24u))
        }
        barrier()
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 4u   // 4 rows per simdgroup - y loads and grid gathers amortize x4 (llama.cpp N_R0_IQ3_S)
        var sumf : float[4]
        var ib = ix
        while (ib < nb) {
            let yb4 = ib * 64u + bu * 8u + il * 4u
            let y0 = x[yb4]
            let y1 = x[yb4 + 1u]
            let y2 = x[yb4 + 2u]
            let y3 = x[yb4 + 3u]
            for [unroll_full] (r in range(4)) {
                let blk = (first_row + uint(r)) * nb + ib
                let qsw = kqu[blk * 26u + bu * 2u + il]
                let qh = (kqu[blk * 26u + 16u + bu / 4u] >> ((bu % 4u) * 8u)) & 255u
                let sgw = kqu[blk * 26u + 18u + bu]
                var accv = float4(0.0)
                for [unroll_full] (wi in range(4)) {
                    let b = il * 4u + uint(wi)
                    let idx = ((qsw >> (8u * uint(wi))) & 255u) | ((qh << (8u - b)) & 256u)
                    let nib = (sgw >> ((b / 2u) * 8u + (b % 2u) * 4u)) & 15u
                    let fs = float4((nib & 1u) != 0u ? -1.0 : 1.0, (nib & 2u) != 0u ? -1.0 : 1.0,
                                    (nib & 4u) != 0u ? -1.0 : 1.0, (nib & 8u) != 0u ? -1.0 : 1.0)
                    let yv = wi == 0 ? y0 : (wi == 1 ? y1 : (wi == 2 ? y2 : y3))
                    accv += yv * (gridf[idx] * fs)
                }
                let sc = float((int(kscb[blk * 16u + bu]) ^ 128) - 128)
                sumf[r] += float(kdh[blk]) * sc * (accv.x + accv.y + accv.z + accv.w)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(4)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// IQ2_S: the iq3s GEMV geometry (4 rows per simdgroup, dispatch rows/8) over the u64 grid read
// DIRECT off the hoisted constant table; per-16 UNSIGNED strips scale each 16-element half,
// d pre-eighth-ed at transcode - the fold is d8 * strip * signed_grid_byte.
[metal_dispatch(name = "enc_kq_iq2s_c", pso = "g_pso_kq_iq2s", tg = 64, grid = "rows/8", params = "rows : int64, n : int64")]
class MetalKqGemvIq2s {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2s quant plane, uint view (18 words per superblock: 8 idx, 8 signs, 2 qh)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_iq2s_msl")]
    def metal_kq_gemv_iq2s {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 4u
        var sumf : float[4]
        var ib = ix
        while (ib < nb) {
            let yb4 = ib * 64u + bu * 8u + il * 4u
            let y0 = x[yb4]
            let y1 = x[yb4 + 1u]
            let y2 = x[yb4 + 2u]
            let y3 = x[yb4 + 3u]
            for [unroll_full] (r in range(4)) {
                let blk = (first_row + uint(r)) * nb + ib
                let qw = kqu[blk * 18u + bu]
                let sgw = kqu[blk * 18u + 8u + bu]
                let qh = (kqu[blk * 18u + 16u + bu / 4u] >> ((bu % 4u) * 8u)) & 255u
                var accv = float4(0.0)
                for [unroll_full] (wi in range(2)) {
                    let l = il * 2u + uint(wi)
                    let idx2 = (((qw >> (8u * l)) & 255u) | ((qh << (8u - 2u * l)) & 0x300u)) * 2u
                    let sgn = (sgw >> (8u * l)) & 255u
                    let w0 = iq3s_sw(iq2s_gw(int(idx2)), sgn & 15u)
                    let w1 = iq3s_sw(iq2s_gw(int(idx2 + 1u)), sgn >> 4u)
                    let ya = wi == 0 ? y0 : y2
                    let yb = wi == 0 ? y1 : y3
                    accv += ya * float4(iq3s_sx(w0, 0u), iq3s_sx(w0, 1u), iq3s_sx(w0, 2u), iq3s_sx(w0, 3u))
                    accv += yb * float4(iq3s_sx(w1, 0u), iq3s_sx(w1, 1u), iq3s_sx(w1, 2u), iq3s_sx(w1, 3u))
                }
                let sc = float(kscb[blk * 16u + bu * 2u + il])
                sumf[r] += float(kdh[blk]) * sc * (accv.x + accv.y + accv.z + accv.w)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(4)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// IQ2_XS: the iq2s GEMV geometry over the parity-signed u16 words - per (block, half) one u32
// carries two u16 qs words; 9-bit index doubles into the grid pair, ksign7m recomputes the
// sign byte (no sign plane, no qh); per-16 UNSIGNED strips, d pre-eighth-ed.
[metal_dispatch(name = "enc_kq_iq2xs_c", pso = "g_pso_kq_iq2xs", tg = 64, grid = "rows/8", params = "rows : int64, n : int64")]
class MetalKqGemvIq2xs {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xs quant plane, uint view (16 words per superblock: 32 u16 qs words)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_iq2xs_msl")]
    def metal_kq_gemv_iq2xs {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 4u
        var sumf : float[4]
        var ib = ix
        while (ib < nb) {
            let yb4 = ib * 64u + bu * 8u + il * 4u
            let y0 = x[yb4]
            let y1 = x[yb4 + 1u]
            let y2 = x[yb4 + 2u]
            let y3 = x[yb4 + 3u]
            for [unroll_full] (r in range(4)) {
                let blk = (first_row + uint(r)) * nb + ib
                let qw = kqu[blk * 16u + bu * 2u + il]
                var accv = float4(0.0)
                for [unroll_full] (wi in range(2)) {
                    let w16 = (qw >> (16u * uint(wi))) & 0xFFFFu
                    let idx2 = (w16 & 511u) * 2u
                    let sgn = ksign7m(w16 >> 9u)
                    let w0 = iq3s_sw(iq2xs_gw(int(idx2)), sgn & 15u)
                    let w1 = iq3s_sw(iq2xs_gw(int(idx2 + 1u)), sgn >> 4u)
                    let ya = wi == 0 ? y0 : y2
                    let yb = wi == 0 ? y1 : y3
                    accv += ya * float4(iq3s_sx(w0, 0u), iq3s_sx(w0, 1u), iq3s_sx(w0, 2u), iq3s_sx(w0, 3u))
                    accv += yb * float4(iq3s_sx(w1, 0u), iq3s_sx(w1, 1u), iq3s_sx(w1, 2u), iq3s_sx(w1, 3u))
                }
                let sc = float(kscb[blk * 16u + bu * 2u + il])
                sumf[r] += float(kdh[blk]) * sc * (accv.x + accv.y + accv.z + accv.w)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(4)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// IQ2_XXS: the iq2xs GEMV geometry over the aux32 form - per (block, half) the block's grid
// word supplies two BYTE indices (each doubling into the u64 grid pair), its aux32 the
// parity ksigns; ONE per-32 UNSIGNED strip scales the whole block on the eighth-ed d.
[metal_dispatch(name = "enc_kq_iq2xxs_c", pso = "g_pso_kq_iq2xxs", tg = 64, grid = "rows/8", params = "rows : int64, n : int64")]
class MetalKqGemvIq2xxs {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // strip region: 8 UNSIGNED per-32 sub-scales + 8 pad, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xxs quant plane, uint view (16 words per superblock: [grid bytes][aux32] per block)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_iq2xxs_msl")]
    def metal_kq_gemv_iq2xxs {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 4u
        var sumf : float[4]
        var ib = ix
        while (ib < nb) {
            let yb4 = ib * 64u + bu * 8u + il * 4u
            let y0 = x[yb4]
            let y1 = x[yb4 + 1u]
            let y2 = x[yb4 + 2u]
            let y3 = x[yb4 + 3u]
            for [unroll_full] (r in range(4)) {
                let blk = (first_row + uint(r)) * nb + ib
                let gw = kqu[blk * 16u + bu * 2u]
                let aux = kqu[blk * 16u + bu * 2u + 1u]
                var accv = float4(0.0)
                for [unroll_full] (wi in range(2)) {
                    let q3 = il * 2u + uint(wi)
                    let bidx = (gw >> (8u * q3)) & 255u
                    let sgn = ksign7m((aux >> (7u * q3)) & 127u)
                    let w0 = iq3s_sw(iq2xxs_gw(int(bidx * 2u)), sgn & 15u)
                    let w1 = iq3s_sw(iq2xxs_gw(int(bidx * 2u + 1u)), sgn >> 4u)
                    let ya = wi == 0 ? y0 : y2
                    let yb = wi == 0 ? y1 : y3
                    accv += ya * float4(iq3s_sx(w0, 0u), iq3s_sx(w0, 1u), iq3s_sx(w0, 2u), iq3s_sx(w0, 3u))
                    accv += yb * float4(iq3s_sx(w1, 0u), iq3s_sx(w1, 1u), iq3s_sx(w1, 2u), iq3s_sx(w1, 3u))
                }
                let sc = float(kscb[blk * 16u + bu])
                sumf[r] += float(kdh[blk]) * sc * (accv.x + accv.y + accv.z + accv.w)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(4)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// IQ3_XXS: the iq3s GEMV shape verbatim over the 24-word rows - the 1 KB halved grid as a
// float4 magnitude slab, signs from the block's aux32 through the parity helper (no table),
// the same (2ls+1)-strip x halved-d fold. 4 rows per simdgroup, dispatch rows/8.
[metal_dispatch(name = "enc_kq_iq3xxs_c", pso = "g_pso_kq_iq3xxs", tgmem = "metal_kq_gemv_iq3xxs_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64, n : int64")]
class MetalKqGemvIq3xxs {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 sub-scale bytes (2ls+1) + 8 pad, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3xxs quant plane, uint view (24 words per superblock: 16 qs, 8 aux)
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @workgroup gridf : float4[256]   // magnitudes pre-expanded at stage time (the iq3s GEMV's winning form)

    [metal_kernel(name="metal_kq_gemv_iq3xxs_msl")]
    def metal_kq_gemv_iq3xxs {
        let lid = gl_LocalInvocationID.x
        for [unroll_full] (k in range(4)) {
            let gi = lid + uint(k) * 64u
            let gw = iq3xxs_gw(int(gi))
            gridf[gi] = float4(float(gw & 255u), float((gw >> 8u) & 255u), float((gw >> 16u) & 255u), float(gw >> 24u))
        }
        barrier()
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 4u
        var sumf : float[4]
        var ib = ix
        while (ib < nb) {
            let yb4 = ib * 64u + bu * 8u + il * 4u
            let y0 = x[yb4]
            let y1 = x[yb4 + 1u]
            let y2 = x[yb4 + 2u]
            let y3 = x[yb4 + 3u]
            for [unroll_full] (r in range(4)) {
                let blk = (first_row + uint(r)) * nb + ib
                let qsw = kqu[blk * 24u + bu * 2u + il]
                let aux = kqu[blk * 24u + 16u + bu]
                var accv = float4(0.0)
                for [unroll_full] (wi in range(4)) {
                    let q3 = il * 2u + uint(wi) / 2u
                    let sgb = ksign7m((aux >> (7u * q3)) & 127u)
                    let nib = uint(wi) % 2u == 0u ? sgb & 15u : sgb >> 4u
                    let idx = (qsw >> (8u * uint(wi))) & 255u
                    let fs = float4((nib & 1u) != 0u ? -1.0 : 1.0, (nib & 2u) != 0u ? -1.0 : 1.0,
                                    (nib & 4u) != 0u ? -1.0 : 1.0, (nib & 8u) != 0u ? -1.0 : 1.0)
                    let yv = wi == 0 ? y0 : (wi == 1 ? y1 : (wi == 2 ? y2 : y3))
                    accv += yv * (gridf[idx] * fs)
                }
                let sc = float((int(kscb[blk * 16u + bu]) ^ 128) - 128)
                sumf[r] += float(kdh[blk]) * sc * (accv.x + accv.y + accv.z + accv.w)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(4)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// The iq3xxs B2/B4 pair: the iq3s shells over the 24-word rows - direct halved-grid reads,
// parity signs (unmeasured batch shapes - followup #58's stance).
[ |> template_struct_instance]
class template MetalKqMvIq3xxsT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 sub-scale bytes (2ls+1) + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3xxs quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_iq3xxs {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp — measured free
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
            let aux = kqu[blk * 24u + 16u + tx]
            var w : float4[8]
            for [unroll_full] (h in range(2)) {
                let qsw = kqu[blk * 24u + tx * 2u + uint(h)]
                for [unroll_full] (wi in range(4)) {
                    let q3 = uint(h) * 2u + uint(wi) / 2u
                    let sgb = ksign7m((aux >> (7u * q3)) & 127u)
                    let nib = uint(wi) % 2u == 0u ? sgb & 15u : sgb >> 4u
                    let sw = iq3s_sw(iq3xxs_gw(int((qsw >> (8u * uint(wi))) & 255u)), nib)
                    w[h * 4 + wi] = float4(iq3s_sx(sw, 0u), iq3s_sx(sw, 1u), iq3s_sx(sw, 2u), iq3s_sx(sw, 3u)) * dl
                }
            }
            let cx4 = sb * 64u + tx * 8u
            static_if (TILED) {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            } else {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        static_if (TILED) {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                    y[(colbase + uint(b)) * ka.ys + row] = s
                }
            }
        } else {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                    y[uint(b) * ka.ys + row] = s
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq3xxs_c", pso = "g_pso_kq_mvb2_iq3xxs", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2Iq3xxs : MetalKqMvIq3xxsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq3xxs_c", pso = "g_pso_kq_mvb4_iq3xxs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4Iq3xxs : MetalKqMvIq3xxsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq3xxs B8 twin: the iq3s B8 shell (one superblock's X panel staged per threadgroup)
// over the 24-word rows; the grid read direct.
[metal_dispatch(name = "enc_kq_mvb8_iq3xxs_c", pso = "g_pso_kq_mvb8_iq3xxs", tgmem = "metal_kq_mvb8_iq3xxs_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8Iq3xxs {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 sub-scale bytes (2ls+1) + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3xxs quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_iq3xxs_msl")]
    def metal_kq_mvb8_iq3xxs {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
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
            let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
            let aux = kqu[blk * 24u + 16u + tx]
            var w : float4[8]
            for [unroll_full] (h in range(2)) {
                let qsw = kqu[blk * 24u + tx * 2u + uint(h)]
                for [unroll_full] (wi in range(4)) {
                    let q3 = uint(h) * 2u + uint(wi) / 2u
                    let sgb = ksign7m((aux >> (7u * q3)) & 127u)
                    let nib = uint(wi) % 2u == 0u ? sgb & 15u : sgb >> 4u
                    let sw = iq3s_sw(iq3xxs_gw(int((qsw >> (8u * uint(wi))) & 255u)), nib)
                    w[h * 4 + wi] = float4(iq3s_sx(sw, 0u), iq3s_sx(sw, 1u), iq3s_sx(sw, 2u), iq3s_sx(sw, 3u)) * dl
                }
            }
            let cxl = tx * 8u
            for [unroll_full] (b in range(8)) {
                for [unroll_full] (u in range(8)) {
                    acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w[u]
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

// The iq3s B2/B4 pair: the iq4xs shell over the per-32-block lane map — lane tx owns block tx,
// 8 grid words = 32 consecutive elems as float4s; the grid read direct (unmeasured batch shapes).
[ |> template_struct_instance]
class template MetalKqMvIq3sT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3s quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_iq3s {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp — measured free
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
            let qh = (kqu[blk * 26u + 16u + tx / 4u] >> ((tx % 4u) * 8u)) & 255u
            let sgw = kqu[blk * 26u + 18u + tx]
            var w : float4[8]
            for [unroll_full] (h in range(2)) {
                let qsw = kqu[blk * 26u + tx * 2u + uint(h)]
                for [unroll_full] (wi in range(4)) {
                    let b = uint(h) * 4u + uint(wi)
                    let idx = ((qsw >> (8u * uint(wi))) & 255u) | ((qh << (8u - b)) & 256u)
                    let nib = (sgw >> ((b / 2u) * 8u + (b % 2u) * 4u)) & 15u
                    let sw = iq3s_sw(iq3s_gw(int(idx)), nib)
                    w[h * 4 + wi] = float4(iq3s_sx(sw, 0u), iq3s_sx(sw, 1u), iq3s_sx(sw, 2u), iq3s_sx(sw, 3u)) * dl
                }
            }
            let cx4 = sb * 64u + tx * 8u
            static_if (TILED) {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            } else {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        static_if (TILED) {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                    y[(colbase + uint(b)) * ka.ys + row] = s
                }
            }
        } else {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                    y[uint(b) * ka.ys + row] = s
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq3s_c", pso = "g_pso_kq_mvb2_iq3s", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2Iq3s : MetalKqMvIq3sT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq3s_c", pso = "g_pso_kq_mvb4_iq3s", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4Iq3s : MetalKqMvIq3sT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq3s B8 twin: the iq4xs B8 shell (one superblock's X panel staged per threadgroup) over
// the per-32-block lane map; the grid read direct.
[metal_dispatch(name = "enc_kq_mvb8_iq3s_c", pso = "g_pso_kq_mvb8_iq3s", tgmem = "metal_kq_mvb8_iq3s_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8Iq3s {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3s quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_iq3s_msl")]
    def metal_kq_mvb8_iq3s {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
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
            let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
            let qh = (kqu[blk * 26u + 16u + tx / 4u] >> ((tx % 4u) * 8u)) & 255u
            let sgw = kqu[blk * 26u + 18u + tx]
            var w : float4[8]
            for [unroll_full] (h in range(2)) {
                let qsw = kqu[blk * 26u + tx * 2u + uint(h)]
                for [unroll_full] (wi in range(4)) {
                    let b = uint(h) * 4u + uint(wi)
                    let idx = ((qsw >> (8u * uint(wi))) & 255u) | ((qh << (8u - b)) & 256u)
                    let nib = (sgw >> ((b / 2u) * 8u + (b % 2u) * 4u)) & 15u
                    let sw = iq3s_sw(iq3s_gw(int(idx)), nib)
                    w[h * 4 + wi] = float4(iq3s_sx(sw, 0u), iq3s_sx(sw, 1u), iq3s_sx(sw, 2u), iq3s_sx(sw, 3u)) * dl
                }
            }
            let cxl = tx * 8u
            for [unroll_full] (b in range(8)) {
                for [unroll_full] (u in range(8)) {
                    acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w[u]
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

// The iq2s B2/B4 pair: the iq3s shell over the u64 grid read direct - lane tx owns block tx,
// TWO grid words per 10-bit index; per-16 UNSIGNED strips give each half its own d8*strip.
[ |> template_struct_instance]
class template MetalKqMvIq2sT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2s quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) - override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_iq2s {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp - measured free
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dl0 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u])
            let dl1 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u + 1u])
            let qw = kqu[blk * 18u + tx]
            let sgw = kqu[blk * 18u + 8u + tx]
            let qh = (kqu[blk * 18u + 16u + tx / 4u] >> ((tx % 4u) * 8u)) & 255u
            var w : float4[8]
            for [unroll_full] (l in range(4)) {
                let lu = uint(l)
                let idx2 = (((qw >> (8u * lu)) & 255u) | ((qh << (8u - 2u * lu)) & 0x300u)) * 2u
                let sgn = (sgw >> (8u * lu)) & 255u
                let dl = l < 2 ? dl0 : dl1
                let sw0 = iq3s_sw(iq2s_gw(int(idx2)), sgn & 15u)
                let sw1 = iq3s_sw(iq2s_gw(int(idx2 + 1u)), sgn >> 4u)
                w[l * 2] = float4(iq3s_sx(sw0, 0u), iq3s_sx(sw0, 1u), iq3s_sx(sw0, 2u), iq3s_sx(sw0, 3u)) * dl
                w[l * 2 + 1] = float4(iq3s_sx(sw1, 0u), iq3s_sx(sw1, 1u), iq3s_sx(sw1, 2u), iq3s_sx(sw1, 3u)) * dl
            }
            let cx4 = sb * 64u + tx * 8u
            static_if (TILED) {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            } else {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(NR)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq2s_c", pso = "g_pso_kq_mvb2_iq2s", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2Iq2s : MetalKqMvIq2sT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq2s_c", pso = "g_pso_kq_mvb4_iq2s", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4Iq2s : MetalKqMvIq2sT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq2s B8 twin: the iq3s B8 shell (one superblock's X panel staged per threadgroup) with
// the u64 two-word compose; the grid read direct.
[metal_dispatch(name = "enc_kq_mvb8_iq2s_c", pso = "g_pso_kq_mvb8_iq2s", tgmem = "metal_kq_mvb8_iq2s_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8Iq2s {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2s quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_iq2s_msl")]
    def metal_kq_mvb8_iq2s {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
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
            let dl0 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u])
            let dl1 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u + 1u])
            let qw = kqu[blk * 18u + tx]
            let sgw = kqu[blk * 18u + 8u + tx]
            let qh = (kqu[blk * 18u + 16u + tx / 4u] >> ((tx % 4u) * 8u)) & 255u
            var w : float4[8]
            for [unroll_full] (l in range(4)) {
                let lu = uint(l)
                let idx2 = (((qw >> (8u * lu)) & 255u) | ((qh << (8u - 2u * lu)) & 0x300u)) * 2u
                let sgn = (sgw >> (8u * lu)) & 255u
                let dl = l < 2 ? dl0 : dl1
                let sw0 = iq3s_sw(iq2s_gw(int(idx2)), sgn & 15u)
                let sw1 = iq3s_sw(iq2s_gw(int(idx2 + 1u)), sgn >> 4u)
                w[l * 2] = float4(iq3s_sx(sw0, 0u), iq3s_sx(sw0, 1u), iq3s_sx(sw0, 2u), iq3s_sx(sw0, 3u)) * dl
                w[l * 2 + 1] = float4(iq3s_sx(sw1, 0u), iq3s_sx(sw1, 1u), iq3s_sx(sw1, 2u), iq3s_sx(sw1, 3u)) * dl
            }
            let cxl = tx * 8u
            for [unroll_full] (b in range(8)) {
                for [unroll_full] (u in range(8)) {
                    acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w[u]
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

// The iq2xs B2/B4 pair: the iq2s shell with the parity-signed u16 walk - lane tx owns block
// tx; two u32 qs words per block, ksign7m per u16; per-16 UNSIGNED strips as dl0/dl1.
[ |> template_struct_instance]
class template MetalKqMvIq2xsT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xs quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) - override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_iq2xs {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp - measured free
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dl0 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u])
            let dl1 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u + 1u])
            let qw0 = kqu[blk * 16u + tx * 2u]
            let qw1 = kqu[blk * 16u + tx * 2u + 1u]
            var w : float4[8]
            for [unroll_full] (l in range(4)) {
                let lu = uint(l)
                let w16 = ((l < 2 ? qw0 : qw1) >> (16u * (lu & 1u))) & 0xFFFFu
                let idx2 = (w16 & 511u) * 2u
                let sgn = ksign7m(w16 >> 9u)
                let dl = l < 2 ? dl0 : dl1
                let sw0 = iq3s_sw(iq2xs_gw(int(idx2)), sgn & 15u)
                let sw1 = iq3s_sw(iq2xs_gw(int(idx2 + 1u)), sgn >> 4u)
                w[l * 2] = float4(iq3s_sx(sw0, 0u), iq3s_sx(sw0, 1u), iq3s_sx(sw0, 2u), iq3s_sx(sw0, 3u)) * dl
                w[l * 2 + 1] = float4(iq3s_sx(sw1, 0u), iq3s_sx(sw1, 1u), iq3s_sx(sw1, 2u), iq3s_sx(sw1, 3u)) * dl
            }
            let cx4 = sb * 64u + tx * 8u
            static_if (TILED) {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            } else {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(NR)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq2xs_c", pso = "g_pso_kq_mvb2_iq2xs", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2Iq2xs : MetalKqMvIq2xsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq2xs_c", pso = "g_pso_kq_mvb4_iq2xs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4Iq2xs : MetalKqMvIq2xsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq2xs B8 twin: the iq2s B8 shell (one superblock's X panel staged per threadgroup) with
// the parity-signed u16 compose; the grid read direct.
[metal_dispatch(name = "enc_kq_mvb8_iq2xs_c", pso = "g_pso_kq_mvb8_iq2xs", tgmem = "metal_kq_mvb8_iq2xs_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8Iq2xs {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xs quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_iq2xs_msl")]
    def metal_kq_mvb8_iq2xs {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
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
            let dl0 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u])
            let dl1 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u + 1u])
            let qw0 = kqu[blk * 16u + tx * 2u]
            let qw1 = kqu[blk * 16u + tx * 2u + 1u]
            var w : float4[8]
            for [unroll_full] (l in range(4)) {
                let lu = uint(l)
                let w16 = ((l < 2 ? qw0 : qw1) >> (16u * (lu & 1u))) & 0xFFFFu
                let idx2 = (w16 & 511u) * 2u
                let sgn = ksign7m(w16 >> 9u)
                let dl = l < 2 ? dl0 : dl1
                let sw0 = iq3s_sw(iq2xs_gw(int(idx2)), sgn & 15u)
                let sw1 = iq3s_sw(iq2xs_gw(int(idx2 + 1u)), sgn >> 4u)
                w[l * 2] = float4(iq3s_sx(sw0, 0u), iq3s_sx(sw0, 1u), iq3s_sx(sw0, 2u), iq3s_sx(sw0, 3u)) * dl
                w[l * 2 + 1] = float4(iq3s_sx(sw1, 0u), iq3s_sx(sw1, 1u), iq3s_sx(sw1, 2u), iq3s_sx(sw1, 3u)) * dl
            }
            let cxl = tx * 8u
            for [unroll_full] (b in range(8)) {
                for [unroll_full] (u in range(8)) {
                    acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w[u]
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

// The iq2xxs B2/B4 pair: the iq2xs shell over the aux32 walk - lane tx owns block tx; the
// block's grid word supplies four byte indices, ksign7m per index; ONE per-32 strip as dl.
[ |> template_struct_instance]
class template MetalKqMvIq2xxsT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // strip region: 8 UNSIGNED per-32 sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xxs quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) - override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_iq2xxs {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp - measured free
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dl = float(kdh[blk]) * float(kscb[blk * 16u + tx])
            let gw = kqu[blk * 16u + tx * 2u]
            let aux = kqu[blk * 16u + tx * 2u + 1u]
            var w : float4[8]
            for [unroll_full] (l in range(4)) {
                let lu = uint(l)
                let bidx = (gw >> (8u * lu)) & 255u
                let sgn = ksign7m((aux >> (7u * lu)) & 127u)
                let sw0 = iq3s_sw(iq2xxs_gw(int(bidx * 2u)), sgn & 15u)
                let sw1 = iq3s_sw(iq2xxs_gw(int(bidx * 2u + 1u)), sgn >> 4u)
                w[l * 2] = float4(iq3s_sx(sw0, 0u), iq3s_sx(sw0, 1u), iq3s_sx(sw0, 2u), iq3s_sx(sw0, 3u)) * dl
                w[l * 2 + 1] = float4(iq3s_sx(sw1, 0u), iq3s_sx(sw1, 1u), iq3s_sx(sw1, 2u), iq3s_sx(sw1, 3u)) * dl
            }
            let cx4 = sb * 64u + tx * 8u
            static_if (TILED) {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            } else {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(8)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(NR)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq2xxs_c", pso = "g_pso_kq_mvb2_iq2xxs", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2Iq2xxs : MetalKqMvIq2xxsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq2xxs_c", pso = "g_pso_kq_mvb4_iq2xxs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4Iq2xxs : MetalKqMvIq2xxsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq2xxs B8 twin: the iq2xs B8 shell (one superblock's X panel staged per threadgroup)
// with the aux32 compose; the grid read direct.
[metal_dispatch(name = "enc_kq_mvb8_iq2xxs_c", pso = "g_pso_kq_mvb8_iq2xxs", tgmem = "metal_kq_mvb8_iq2xxs_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8Iq2xxs {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // strip region: 8 UNSIGNED per-32 sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xxs quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_iq2xxs_msl")]
    def metal_kq_mvb8_iq2xxs {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
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
            let dl = float(kdh[blk]) * float(kscb[blk * 16u + tx])
            let gw = kqu[blk * 16u + tx * 2u]
            let aux = kqu[blk * 16u + tx * 2u + 1u]
            var w : float4[8]
            for [unroll_full] (l in range(4)) {
                let lu = uint(l)
                let bidx = (gw >> (8u * lu)) & 255u
                let sgn = ksign7m((aux >> (7u * lu)) & 127u)
                let sw0 = iq3s_sw(iq2xxs_gw(int(bidx * 2u)), sgn & 15u)
                let sw1 = iq3s_sw(iq2xxs_gw(int(bidx * 2u + 1u)), sgn >> 4u)
                w[l * 2] = float4(iq3s_sx(sw0, 0u), iq3s_sx(sw0, 1u), iq3s_sx(sw0, 2u), iq3s_sx(sw0, 3u)) * dl
                w[l * 2 + 1] = float4(iq3s_sx(sw1, 0u), iq3s_sx(sw1, 1u), iq3s_sx(sw1, 2u), iq3s_sx(sw1, 3u)) * dl
            }
            let cxl = tx * 8u
            for [unroll_full] (b in range(8)) {
                for [unroll_full] (u in range(8)) {
                    acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w[u]
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

// The iq4xs B2/B4 pair: the K4T shell (row/colbase/TILED twins) over the per-32-block lane map of
// the GEMV above — lane tx owns block tx of each superblock, 16 lo + 16 hi weights as float4s.
[ |> template_struct_instance]
class template MetalKqMvIq4xsT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq4xs quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_iq4xs {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp — measured free
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
            let qb = blk * 32u + tx * 4u
            var wl : float4[4]
            var wh : float4[4]
            for [unroll_full] (u in range(4)) {
                let qv = kqu[qb + uint(u)]
                wl[u] = float4(iq4_lut(qv & 15u), iq4_lut((qv >> 8u) & 15u), iq4_lut((qv >> 16u) & 15u), iq4_lut((qv >> 24u) & 15u)) * dl
                wh[u] = float4(iq4_lut((qv >> 4u) & 15u), iq4_lut((qv >> 12u) & 15u), iq4_lut((qv >> 20u) & 15u), iq4_lut(qv >> 28u)) * dl
            }
            let cx4 = sb * 64u + tx * 8u
            static_if (TILED) {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        let vh = x[xb4 + 4u + uint(u)]
                        acc += v.x * wl[u].x + v.y * wl[u].y + v.z * wl[u].z + v.w * wl[u].w
                        acc += vh.x * wh[u].x + vh.y * wh[u].y + vh.z * wh[u].z + vh.w * wh[u].w
                    }
                    sumf[b] += acc
                }
            } else {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        let vh = x[xb4 + 4u + uint(u)]
                        acc += v.x * wl[u].x + v.y * wl[u].y + v.z * wl[u].z + v.w * wl[u].w
                        acc += vh.x * wh[u].x + vh.y * wh[u].y + vh.z * wh[u].z + vh.w * wh[u].w
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        static_if (TILED) {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                    y[(colbase + uint(b)) * ka.ys + row] = s
                }
            }
        } else {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                    y[uint(b) * ka.ys + row] = s
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq4xs_c", pso = "g_pso_kq_mvb2_iq4xs", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2Iq4xs : MetalKqMvIq4xsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq4xs_c", pso = "g_pso_kq_mvb4_iq4xs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4Iq4xs : MetalKqMvIq4xsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq4xs B8 twin: the K45 B8 shell (one superblock's X panel staged per threadgroup) over the
// per-32-block lane map
[metal_dispatch(name = "enc_kq_mvb8_iq4xs_c", pso = "g_pso_kq_mvb8_iq4xs", tgmem = "metal_kq_mvb8_iq4xs_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8Iq4xs {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq4xs quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_iq4xs_msl")]
    def metal_kq_mvb8_iq4xs {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
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
            let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
            let qb = blk * 32u + tx * 4u
            var wl : float4[4]
            var wh : float4[4]
            for [unroll_full] (u in range(4)) {
                let qv = kqu[qb + uint(u)]
                wl[u] = float4(iq4_lut(qv & 15u), iq4_lut((qv >> 8u) & 15u), iq4_lut((qv >> 16u) & 15u), iq4_lut((qv >> 24u) & 15u)) * dl
                wh[u] = float4(iq4_lut((qv >> 4u) & 15u), iq4_lut((qv >> 12u) & 15u), iq4_lut((qv >> 20u) & 15u), iq4_lut(qv >> 28u)) * dl
            }
            let cxl = tx * 8u
            for [unroll_full] (b in range(8)) {
                for [unroll_full] (u in range(4)) {
                    acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * wl[u] + txp[uint(b) * 64u + cxl + 4u + uint(u)] * wh[u]
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

// The iq4nl B2/B4 pair: the iq4xs shells with the per-32-block d fold (no strip byte).
[ |> template_struct_instance]
class template MetalKqMvIq4nlT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the 16B/sb d plane, half view
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // binding-shape twin (unused)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq4nl quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_iq4nl {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp — measured free
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dl = float(kdh[blk * 8u + tx])
            let qb = blk * 32u + tx * 4u
            var wl : float4[4]
            var wh : float4[4]
            for [unroll_full] (u in range(4)) {
                let qv = kqu[qb + uint(u)]
                wl[u] = float4(iq4_lut(qv & 15u), iq4_lut((qv >> 8u) & 15u), iq4_lut((qv >> 16u) & 15u), iq4_lut((qv >> 24u) & 15u)) * dl
                wh[u] = float4(iq4_lut((qv >> 4u) & 15u), iq4_lut((qv >> 12u) & 15u), iq4_lut((qv >> 20u) & 15u), iq4_lut(qv >> 28u)) * dl
            }
            let cx4 = sb * 64u + tx * 8u
            static_if (TILED) {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        let vh = x[xb4 + 4u + uint(u)]
                        acc += v.x * wl[u].x + v.y * wl[u].y + v.z * wl[u].z + v.w * wl[u].w
                        acc += vh.x * wh[u].x + vh.y * wh[u].y + vh.z * wh[u].z + vh.w * wh[u].w
                    }
                    sumf[b] += acc
                }
            } else {
                for [unroll_full] (b in range(NR)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        let vh = x[xb4 + 4u + uint(u)]
                        acc += v.x * wl[u].x + v.y * wl[u].y + v.z * wl[u].z + v.w * wl[u].w
                        acc += vh.x * wh[u].x + vh.y * wh[u].y + vh.z * wh[u].z + vh.w * wh[u].w
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        static_if (TILED) {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                    y[(colbase + uint(b)) * ka.ys + row] = s
                }
            }
        } else {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                    y[uint(b) * ka.ys + row] = s
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq4nl_c", pso = "g_pso_kq_mvb2_iq4nl", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2Iq4nl : MetalKqMvIq4nlT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq4nl_c", pso = "g_pso_kq_mvb4_iq4nl", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4Iq4nl : MetalKqMvIq4nlT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq4nl B8 twin: the iq4xs B8 shell (one superblock's X panel staged per threadgroup) with
// the per-32-block d fold
[metal_dispatch(name = "enc_kq_mvb8_iq4nl_c", pso = "g_pso_kq_mvb8_iq4nl", tgmem = "metal_kq_mvb8_iq4nl_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8Iq4nl {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the 16B/sb d plane, half view
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // binding-shape twin (unused)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq4nl quant plane, uint view
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_iq4nl_msl")]
    def metal_kq_mvb8_iq4nl {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let nb4 = ka.ndim / 4u
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
            let dl = float(kdh[blk * 8u + tx])
            let qb = blk * 32u + tx * 4u
            var wl : float4[4]
            var wh : float4[4]
            for [unroll_full] (u in range(4)) {
                let qv = kqu[qb + uint(u)]
                wl[u] = float4(iq4_lut(qv & 15u), iq4_lut((qv >> 8u) & 15u), iq4_lut((qv >> 16u) & 15u), iq4_lut((qv >> 24u) & 15u)) * dl
                wh[u] = float4(iq4_lut((qv >> 4u) & 15u), iq4_lut((qv >> 12u) & 15u), iq4_lut((qv >> 20u) & 15u), iq4_lut(qv >> 28u)) * dl
            }
            let cxl = tx * 8u
            for [unroll_full] (b in range(8)) {
                for [unroll_full] (u in range(4)) {
                    acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * wl[u] + txp[uint(b) * 64u + cxl + 4u + uint(u)] * wh[u]
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

// The B2/B4 width pair as one template per format (the B8 trio below is a DIFFERENT algorithm and
// stays hand-written). TILED branch-duplicates the b-loop/writeback because a LIVE colbase on B2
// costs +2% (k4) / +0.5% (k6) at cls (bench_metal_gemv_kernels KqMv round); B4 stamps byte-identical.
[ |> template_struct_instance]
class template MetalKqMvK4T {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_k4 {   // nolint:STYLE038 — the TILED twin loops are byte-identity-driven, not splittable
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp — measured free
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
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
                static_if (TILED) {
                    for [unroll_full] (b in range(NR)) {
                        let xb4 = (colbase + uint(b)) * nb4 + cx4
                        var acc = 0.0
                        for [unroll_full] (u in range(4)) {
                            let v = x[xb4 + uint(u)]
                            acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                        }
                        sumf[b] += acc
                    }
                } else {
                    for [unroll_full] (b in range(NR)) {
                        let xb4 = uint(b) * nb4 + cx4
                        var acc = 0.0
                        for [unroll_full] (u in range(4)) {
                            let v = x[xb4 + uint(u)]
                            acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                        }
                        sumf[b] += acc
                    }
                }
            }
            sb++
        }
        static_if (TILED) {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                    y[(colbase + uint(b)) * ka.ys + row] = s
                }
            }
        } else {
            for [unroll_full] (b in range(NR)) {
                var s = sumf[b]
                s += simd_shuffle_down(s, 4u)
                s += simd_shuffle_down(s, 2u)
                s += simd_shuffle_down(s, 1u)
                if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                    y[uint(b) * ka.ys + row] = s
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k4_c", pso = "g_pso_kq_mvb2_k4", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2K4 : MetalKqMvK4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_k4_c", pso = "g_pso_kq_mvb4_k4", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4K4 : MetalKqMvK4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

[ |> template_struct_instance]
class template MetalKqMvK6T {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    [metal_kernel]
    def metal_kq_mv_k6 {   // nolint:STYLE038 — the TILED twin loops are byte-identity-driven, not splittable
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU   // nolint:LINT002 dead in the TILED=false stamp — measured free
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u             // 16B offset within the slot, in uints
        let qoff = (s % 2u) * 8u + o4       // ql uint offset within the 128-half's 16 uints
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
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
                static_if (TILED) {
                    for [unroll_full] (b in range(NR)) {
                        let xb4 = (colbase + uint(b)) * nb4 + cx4
                        var acc = 0.0
                        for [unroll_full] (u in range(4)) {
                            let v = x[xb4 + uint(u)]
                            acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                        }
                        sumf[b] += acc
                    }
                } else {
                    for [unroll_full] (b in range(NR)) {
                        let xb4 = uint(b) * nb4 + cx4
                        var acc = 0.0
                        for [unroll_full] (u in range(4)) {
                            let v = x[xb4 + uint(u)]
                            acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                        }
                        sumf[b] += acc
                    }
                }
            }
            sb++
        }
        static_if (TILED) {
            for [unroll_full] (b in range(NR)) {
                var sm = sumf[b]
                sm += simd_shuffle_down(sm, 4u)
                sm += simd_shuffle_down(sm, 2u)
                sm += simd_shuffle_down(sm, 1u)
                if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                    y[(colbase + uint(b)) * ka.ys + row] = sm
                }
            }
        } else {
            for [unroll_full] (b in range(NR)) {
                var sm = sumf[b]
                sm += simd_shuffle_down(sm, 4u)
                sm += simd_shuffle_down(sm, 2u)
                sm += simd_shuffle_down(sm, 1u)
                if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                    y[uint(b) * ka.ys + row] = sm
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k6_c", pso = "g_pso_kq_mvb2_k6", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2K6 : MetalKqMvK6T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_k6_c", pso = "g_pso_kq_mvb4_k6", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4K6 : MetalKqMvK6T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// k5 twins: k4's chunk geometry and kmask scale decode verbatim (q5_K shares the 12-byte scale
// block) + the qh 5th-bit overlay. The 160B plane rides the SAME uint view (40 uints/blk: qs at
// 40*blk, qh at 40*blk+32) — the overlay bit equals its kmask pair index, so scale indexing serves it too.

// No TILED here: k5's live-colbase B2 measured neutral-to-BETTER (-1.4% cls, max_threads 448 -> 512;
// bench_metal_gemv_kernels KqMv round), so one spelling serves both widths; B4 stamps byte-identical.
[ |> template_struct_instance]
class template MetalKqMvK5T {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k5 plane, uint view (qs at 40*blk, qh at 40*blk+32)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together

    [metal_kernel]
    def metal_kq_mv_k5 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
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
                for [unroll_full] (b in range(NR)) {
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
        for [unroll_full] (b in range(NR)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                y[(colbase + uint(b)) * ka.ys + row] = s
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k5_c", pso = "g_pso_kq_mvb2_k5", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB2K5 : MetalKqMvK5T {
    override NR = 2
    override NRU = 2u
}

[metal_dispatch(name = "enc_kq_mvb4_k5_c", pso = "g_pso_kq_mvb4_k5", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64")]
class MetalKqMvB4K5 : MetalKqMvK5T {
    override NR = 4
    override NRU = 4u
}

// The B8 K4/K5 pair as one template — same X-panel algorithm, q5_K adds the qh overlay (w4 is
// arm-scoped; K4's stamp carries one dead hq decl, below the measured-free bar). K6 stays forked
// on the weight-decode axis (its 48B superblock walk shares no BLK/QH shape with these).
[ |> template_struct_instance]
class template MetalKqMvB8K45T {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // quant plane, uint view (qs at BLK*blk; k5 qh at BLK*blk+32)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel
    @template_constant BLK : uint = 32u  // superblock quant-plane stride in uints
    @template_constant QH : bool = false // q5_K: the 5th-bit overlay plane rides after the nibbles

    [metal_kernel]
    def metal_kq_mvb8 {   // nolint:STYLE038 — the QH twin arms are byte-identity-driven, not splittable
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ka.ndim / 4u
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
            let qb = blk * BLK + q4i * 8u + lo
            var qv : uint[8]
            var hq : uint[4]   // nolint:LINT002 dead in the QH=false stamp — no init, no dataflow
            for [unroll_full] (u in range(4)) {
                qv[u] = kqu[qb + uint(u)]
                qv[4 + u] = kqu[qb + 16u + uint(u)]
                static_if (QH) {
                    hq[u] = kqu[blk * BLK + 32u + lo + uint(u)]
                }
            }
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                static_if (QH) {
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
                } else {
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
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb8_k4_c", pso = "g_pso_kq_mvb8_k4", tgmem = "MetalKqMvB8K4_metal_kq_mvb8_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8K4 : MetalKqMvB8K45T {
    override BLK = 32u
    override QH = false
}

[metal_dispatch(name = "enc_kq_mvb8_k5_c", pso = "g_pso_kq_mvb8_k5", tgmem = "MetalKqMvB8K5_metal_kq_mvb8_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8K5 : MetalKqMvB8K45T {
    override BLK = 40u
    override QH = true
}

[metal_dispatch(name = "enc_kq_mvb8_k6_c", pso = "g_pso_kq_mvb8_k6", tgmem = "metal_kq_mvb8_k6_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64")]
class MetalKqMvB8K6 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]

    [metal_kernel(name="metal_kq_mvb8_k6_msl")]
    def metal_kq_mvb8_k6 {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ka.ndim / 4u
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
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = sm
            }
        }
    }
}

// Per-head RMSNorm on Q/K before rope (qwen3-line QK-norm): one threadgroup = hs threads per
// (head, row), nq q-head groups then the k heads, weight w[(k ? woffe : 0) + j]. Decode covers
// q+k in ONE grid over the fused QKV buffer; prefill dispatches separate q/k panels per-plane.
[metal_dispatch(name = "enc_qk_norm", pso = "g_pso_qknorm", tgmem = "metal_qk_norm_msl_tgmem", tg = "head_size", grid = "ntg, nrows", params = "ntg : int64, nrows : int64, head_size : int64")]
class MetalQkNorm {
    @ssbo @binding = 0 @off = "xoff" x : array<float>     // bound at the panel offset; normalized in place
    @ssbo @binding = 1 @role = "weight" w : array<float>
    @uniform @binding = 2 ka : QkNormArgs
    @workgroup part : float[16]     // one per simdgroup — hs <= 512 (gemma4 global heads)

    [metal_kernel(name="metal_qk_norm_msl")]
    def metal_qk_norm {
        let nq = ka.nq
        let koffe = ka.koffe
        let rstride = ka.rstride
        let hs = ka.hs
        let woffe = ka.woffe
        let eps = ka.eps
        let soffe = ka.soffe
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

// One elementwise combine g[i] = combine(g[i], u[i]), one thread per element — the ew2 family.
// Per-op [metal_dispatch] builders on the stamps; the add's layer_out_scale rides the gated
// ascale slot — the other combines erase it (field, binding, and MSL declaration all gone).
[ |> template_struct_instance]
class template MetalEw2T {
    @ssbo @binding = 0 @role = "readwrite" @off = "goff" @span = "total*4" g : array<float>     // in place
    @ssbo @binding = 1 @role = "read" @off = "uoff" @span = "total*4" u : array<float>
    @uniform @binding = 2 total : uint      // grid guard
    @uniform @binding = 3 @template_gate = HAS_ASCALE ascale : float
    @ssbo @binding = 4 @role = "write" @template_gate = HX gh : array<float16>   // f16 twin of g (the dual-store stamps)
    @template_constant HAS_ASCALE : bool = false
    @template_constant HX : bool = false

    def abstract combine(a, b : float) : float

    [metal_kernel]
    def metal_ew2 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let vv = g[gid]
        let res = combine(vv, u[gid])
        g[gid] = res
        static_if (HX) {
            gh[gid] = float16(res)
        }
    }
}

// g = silu(g) * u — swiglu exactly (the CPU side's exp4 polynomial vs the GPU exp differ
// inside the shared tolerance envelope); silu shared with the HX stamp so both compute one form
def private msl_silu_mul(a, b : float) : float => (a / (1.0 + exp(-a))) * b

[metal_dispatch(name = "enc_swiglu", pso = "g_pso_swiglu", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSwiglu : MetalEw2T {
    def override combine(a, b : float) : float => msl_silu_mul(a, b)
}

[metal_dispatch(name = "enc_swiglu_hx", pso = "g_pso_swiglu_hx", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSwigluHX : MetalEw2T {
    override HX = true

    def override combine(a, b : float) : float => msl_silu_mul(a, b)
}

// GeGLU twin (gemma line), the CPU's tanh-approx gelu via the exp identity — Metal tanh NaNs
// past ~44 while exp saturates: exp(INF) -> denom INF -> the term is 0
def private msl_geglu_mul(a, b : float) : float => (
    0.5 * a * (2.0 - 2.0 / (1.0 + exp(2.0 * (0.7978845608028654 * (a + 0.044715 * a * a * a))))) * b)

[metal_dispatch(name = "enc_geglu", pso = "g_pso_geglu", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGeglu : MetalEw2T {
    def override combine(a, b : float) : float => msl_geglu_mul(a, b)
}

[metal_dispatch(name = "enc_geglu_hx", pso = "g_pso_geglu_hx", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGegluHX : MetalEw2T {
    override HX = true

    def override combine(a, b : float) : float => msl_geglu_mul(a, b)
}

// GEGLU-quick (gemma4v): gelu_quick(g)*u with the CPU's f16-table form — both binary16
// round-trips run in kernel (RNE), so the residual stream sees the same activation bits as the
// CPU lane rather than the exact x*sigmoid(1.702x) (form-matched like the tower GELUs).
[never_inline]   // the single-return-splice protection of msl_gelu_lut_form
def private msl_gelu_quick_lut_form(a : float) : float {
    let xr = float(float16(a))
    let gv = xr * (1.0 / (1.0 + exp(-1.702 * xr)))
    return float(float16(gv))
}

[metal_dispatch(name = "enc_geglu_quick", pso = "g_pso_geglu_quick", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGegluQuick : MetalEw2T {
    def override combine(a, b : float) : float => msl_gelu_quick_lut_form(a) * b
}

// residual add: x += y, scaled — gemma4 layer_out_scale scales the WHOLE layer output
[metal_dispatch(name = "enc_add_c", pso = "g_pso_add", tg = 64, grid = "total/64", params = "total : int64")]
class MetalAdd : MetalEw2T {
    override HAS_ASCALE = true

    def override combine(a, b : float) : float => (a + b) * ascale
}

// per-head sigmoid output gate (sigmoid_gate): x ⊙ σ(g) in place — the gated-attention
// epilogue between the attention output and the out projection
[metal_dispatch(name = "enc_sigmul", pso = "g_pso_sigmul", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSigmul : MetalEw2T {
    def override combine(a, b : float) : float => a / (1.0 + exp(-b))
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
    @uniform @binding = 4 ka : DnArgs

    [metal_kernel(name="metal_dn_conv_msl")]
    def metal_dn_conv {
        let cd = ka.cd
        let dconv = ka.dconv
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
    @ssbo @binding = 2 @role = "alias" @off = "hmoff" histm : array<float> // dual-store: the npos-1 window's history (verify shadow region)
    @uniform @binding = 3 ka : DnArgs

    [metal_kernel(name="metal_dn_conv_hist_msl")]
    def metal_dn_conv_hist {
        let cd = ka.cd
        let dconv = ka.dconv
        let npos = ka.npos
        let mid = ka.mid
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
    @uniform @binding = 1 ka : DnArgs

    [metal_kernel(name="metal_dn_l2norm_msl")]
    def metal_dn_l2norm {
        let cd = ka.cd
        let kd = ka.kd
        let ds = ka.ds
        let eps = ka.eps
        let qscale = ka.qscale
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

// The fused sequential gated delta-net rule: one simdgroup per
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
    @ssbo @binding = 7 @role = "alias" @off = "stmoff" stm : array<float>  // dual-store: post-token-(npos-2) state (verify shadow region)
    @uniform @binding = 8 ka : DnArgs

    [metal_kernel(name="metal_dn_scan_msl")]
    def metal_dn_scan {
        let cd = ka.cd
        let kd = ka.kd
        let ds = ka.ds
        let nvh = ka.nvh
        let nkh = ka.nkh
        let di = ka.di
        let npos = ka.npos
        let mid = ka.mid
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
    @uniform @binding = 3 ka : DnArgs

    [metal_kernel(name="metal_dn_gate_msl")]
    def metal_dn_gate {
        let ds = ka.ds
        let di = ka.di
        let eps = ka.eps
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
    @uniform @binding = 3 ka : DnDeintArgs

    [metal_kernel(name="metal_dn_deint_msl")]
    def metal_dn_deint {
        let hs = ka.hs
        let qd = ka.qd
        let qs = ka.qs
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
