options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_kernels shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require daslib/typemacro_boost  // [template_struct_instance] — the sq_attn codec templates below
require metal/msl_shader
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_kqformat
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_knee   // the knee races' rung loop and pricing (`knee_rungs`, `knee_pick`)
require dasllama/dasllama_gpu_kernels_common public
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
    return softcap > 0.0 ? softcap_exp(sc, softcap) : sc
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

//! The cross-simdgroup fold over red[], the simdgroups' slots a barrier already fenced: the sum, and the max.
def public tg_fold_sum(red : auto(RR); n : uint) : float {
    var tot = 0.0
    var sgi = 0u
    while (sgi < n) {
        tot += red[sgi]
        sgi++
    }
    return tot
}

def public tg_fold_max(red : auto(RR); n : uint) : float {
    var mm = red[0]
    var sgi = 1u
    while (sgi < n) {
        mm = max(mm, red[sgi])
        sgi++
    }
    return mm
}

//! Every lane holds the threadgroup sum (max) of v after one barrier; every lane rereads red[], so two folds in a row take a barrier between them.
def public tg_sum_all(v : float; var red : auto(RR)) : float {
    let s = simd_sum(v)
    if (gl_SubgroupInvocationID == 0u) {
        red[gl_SubgroupID] = s
    }
    barrier()
    let tot = tg_fold_sum(red, gl_NumSubgroups)
    return tot
}

def public tg_max_all(v : float; var red : auto(RR)) : float {
    let m = simd_max(v)
    if (gl_SubgroupInvocationID == 0u) {
        red[gl_SubgroupID] = m
    }
    barrier()
    let mm = tg_fold_max(red, gl_NumSubgroups)
    return mm
}

//! The staged scores' softmax with the sink logit sinkv (read only when hass) joining the max and the sum: the weights in place, the sum returned.
def private sq_softmax_sink(var scores : auto(SS); var red : float[16]; n, lid, nt, hass : uint; sinkv : float) : float {
    barrier()
    var mm = sq_score_max(scores, red, n, lid, nt)
    if (hass != 0u) {
        mm = max(mm, sinkv)
    }
    barrier()                           // red[] is reused for the sum below
    var tot = sq_exp_sum(scores, red, mm, n, lid, nt)
    if (hass != 0u) {
        tot += exp(max(sinkv - mm, -80.0))
    }
    return tot
}

def private sq_score_max(scores : auto(SS); var red : float[16]; n, lid, nt : uint) : float {
    var m = -3.0e38
    var j = lid
    while (j < n) {
        m = max(m, scores[j])
        j += nt
    }
    let mm = tg_max_all(m, red)
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
    let tot = tg_sum_all(l, red)
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
    qhs : uint      //! a q head's stride: hs (0 reads as hs), or 2 hs where a gated model's head carries its gate past it
    gated : uint    //! the out scaled by the sigmoid of the gate a q head carries (`sq_gated`)
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
    qhs : uint
    gated : uint
    ring : uint     //! a sliding layer's ring rows: a key's row is its `kv_ring_row` slot; 0 = a row a position
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
    qhs : uint
    gated : uint
    layr : uint     //! the rings' rows ahead of the layer in a stream's slice (elements, or bytes under a block codec): its base is `lay * cap + layr`
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
    qhs : uint
    gated : uint
    layr : uint     //! the rings' rows ahead of the layer in a stream's slice (elements, or bytes under a block codec): its base is `lay * cap + layr`
    ring : uint     //! a sliding layer's ring rows: a key's row is its `kv_ring_row` slot; 0 = a row a position
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
    window : uint   //! a sliding layer's span in keys, 0 on a full layer
    softcap : float //! the attention-logit soft cap, 0 where the model has none
    qhs : uint
    gated : uint
    layr : uint     //! the rings' rows ahead of the layer in a stream's slice (elements, or bytes under a block codec): its base is `lay * cap + layr`
    ring : uint     //! a sliding layer's ring rows: a key's row is its `kv_ring_row` slot; 0 = a row a position
}

//! a q head's stride: `hs`, or the gated head's `2 hs` the args carry
def sq_qhs(qhs, hs : uint) : uint => qhs == 0u ? hs : qhs

//! the attention out `o` of the q head's element at `qi`: under `gated` scaled by the sigmoid of its gate `hs` past it in `q`
def sq_gated(o : float; q : array<float>; qi, hs, gated : uint) : float => gated != 0u ? sig_gate(o, q[qi + hs]) : o

struct SqAttnCombDArgs {
    nchmax : uint
    nheads : uint
    hs : uint
    hass : uint     // the layer carries attention sinks
    hdiv : uint     // the combine's heads a model head: 2 or 4 where a wide head lands as parts of 128
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
        let qhs = sq_qhs(ka.qhs, hs)
        let qoff = bb * (nheads * qhs) + h * qhs
        sq_fill_scores(kc, q, scores, 0u, 0u, cnt, kv_dim, khoff, qoff, hs, scale, softcap)
        let tot = sq_softmax_sink(scores, red, cnt, lid, nt, hass, hass != 0u ? sink[h] : 0.0)
        let o = sq_vacc(vc, scores, 0u, 0u, cnt, kv_dim, khoff, lid) / tot
        yo[bb * (nheads * hs) + h * hs + lid] = sq_gated(o, q, qoff + lid, hs, ka.gated)
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
        let qhs = sq_qhs(ka.qhs, hs)
        let qoff = bb * (nheads * qhs) + h * qhs
        sq_fill_scores_q(ksh, kqb, q, scores, 0u, 0u, 0u, cnt, nblk, khblk, qoff, hs, scale, softcap)
        let tot = sq_softmax_sink(scores, red, cnt, lid, nt, hass, hass != 0u ? sink[h] : 0.0)
        let o = sq_vacc_q(vsh, vqb, scores, 0u, 0u, 0u, cnt, nblk, khblk, lid) / tot
        yo[bb * (nheads * hs) + h * hs + lid] = sq_gated(o, q, qoff + lid, hs, ka.gated)
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
        let qhs = sq_qhs(ka.qhs, hs)
        let qoff = bb * (nheads * qhs) + h * qhs
        let c0 = ci * 64u
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = sqd_window_start(window, cnt)
        if (c1 <= wstart) {
            return
        }
        sq_fill_scores(kc, q, scores, 0u, kv_ring_row(c0, ka.ring), nrows, kv_dim, khoff, qoff, hs, scale, softcap)
        barrier()
        sq_mask_window(scores, c0, nrows, wstart, lid, nt)
        let mm = sq_score_max(scores, red, nrows, lid, nt)
        barrier()                           // red[] is reused for the sum below
        let tot = sq_exp_sum(scores, red, mm, nrows, lid, nt)
        let pbase = ((bb * nheads + h) * nchunks + ci) * (hs + 2u)
        part[pbase + lid] = sq_gated(sq_vacc(vc, scores, 0u, kv_ring_row(c0, ka.ring), nrows, kv_dim, khoff, lid), q, qoff + lid, hs, ka.gated)   //! the gate is linear, so a chunk's partial takes it
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

[metal_dispatch(name = "enc_attnp16_c", pso = "g_pso_attnp16", tgmem = "MetalSqAttnPartF16_metal_sq_attn_part_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64", requires = "ka.ring % 64")]
class MetalSqAttnPartF16 : MetalSqAttnPartKvT {
    typedef KT = float16
}

// f32-mirror twin — same math, f32 K/V loads.
[metal_dispatch(name = "enc_attnp32_c", pso = "g_pso_attnp32", tgmem = "MetalSqAttnPartF32_metal_sq_attn_part_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64", requires = "ka.ring % 64")]
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
        let qhs = sq_qhs(ka.qhs, hs)
        let qoff = bb * (nheads * qhs) + h * qhs
        let c0 = ci * 64u
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = sqd_window_start(window, cnt)
        if (c1 <= wstart) {
            return
        }
        sq_fill_scores_q(ksh, kqb, q, scores, 0u, 0u, kv_ring_row(c0, ka.ring), nrows, nblk, khblk, qoff, hs, scale, softcap)
        barrier()
        sq_mask_window(scores, c0, nrows, wstart, lid, nt)
        let mm = sq_score_max(scores, red, nrows, lid, nt)
        barrier()                           // red[] is reused for the sum below
        let tot = sq_exp_sum(scores, red, mm, nrows, lid, nt)
        let pbase = ((bb * nheads + h) * nchunks + ci) * (hs + 2u)
        part[pbase + lid] = sq_gated(sq_vacc_q(vsh, vqb, scores, 0u, 0u, kv_ring_row(c0, ka.ring), nrows, nblk, khblk, lid), q, qoff + lid, hs, ka.gated)
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// q8_0 instance — 34B/32-elem q8_0 blocks.
[metal_dispatch(name = "enc_attnp_q8_c", pso = "g_pso_attnp_q8", tgmem = "MetalSqAttnPartQ8_metal_sq_attn_part_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64", requires = "ka.ring % 64")]
class MetalSqAttnPartQ8 : MetalSqAttnPartQuantT {
    typedef QB = int8
}

// tq4 instance — 18B rotated 4-bit blocks (rotated basis; fwht-unsign runs downstream).
[metal_dispatch(name = "enc_attnp_tq4_c", pso = "g_pso_attnp_tq4", tgmem = "MetalSqAttnPartTq4_metal_sq_attn_part_msl_tgmem", tg = "head_size", grid = "ntg", params = "ntg : int64, head_size : int64", requires = "ka.ring % 64")]
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
        var nch = nchmax     // nolint:LINT003,LINT010,LINT019 — the single stamp reads these inits, the
        var chlo = ka.chlo   // nolint:LINT003,LINT010,LINT019 — BATCHED stamp overwrites them in its arm
        static_if (BATCHED) {
            // holes past a shallow row's last chunk are never read; sliding layers skip the
            // row's below-window chunks
            let cnt = rt[bb].w
            nch = min((cnt + 63u) / 64u, nchmax)   // a row deeper than the dispatch's plane reads a prefix, never its neighbours' partials
            let wstart = sqd_window_start(ka.window, cnt)
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
// (the MetalQ8Gemv discipline); KT/CLAMP16 = the mirror codec axis (the GkRopeKvStore stamps' shape).
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

    def seg_dot(qv : array<int8>; sv : array<float16>; seg_row, seg_row2, nb, ib0, il : uint;
                var acc0 : float&; var acc1 : float&) {
        var ib = ib0
        while (ib < nb) {
            let xi = ib * 8u + il * 2u
            let x0 = x[xi]
            let x1 = x[xi + 1u]
            let qb0 = (seg_row * nb + ib) * 34u + 2u + il * 8u
            let qb1 = (seg_row2 * nb + ib) * 34u + 2u + il * 8u
            acc0 += q8_oct_dot(qv, qb0, x0, x1) * float(sv[(seg_row * nb + ib) * 17u])
            acc1 += q8_oct_dot(qv, qb1, x0, x1) * float(sv[(seg_row2 * nb + ib) * 17u])
            ib += 32u
        }
    }

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
        let ib0 = sg * 8u + ix
        // both pair rows sit in ONE segment (qd/kvd even, NEOX pairs stay in-head)
        if (row < qd) {
            seg_dot(wqbq, wshq, row, row2, nb, ib0, il, acc0, acc1)
        } elif (row < qd + kvd) {
            seg_dot(wqbk, wshk, row - qd, row2 - qd, nb, ib0, il, acc0, acc1)
        } else {
            seg_dot(wqbv, wshv, row - qd - kvd, row2 - qd - kvd, nb, ib0, il, acc0, acc1)
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


// The W1|W3 twin: BOTH ffn rows of an output element dotted together, act(g)·u written directly -
// the swiglu/geglu dispatch and its serialization level are gone. SPLIT = one threadgroup per
// element, the reduction split across its 4 simdgroups; !SPLIT = a row per simdgroup.
[ |> template_struct_instance]
class template MetalQ8GemvW13SwT {
    @ssbo @binding = 0 @role = "weight" @off = "w1off" wqb1 : array<int8>   // w1 blob, byte view @ its tensor offset
    @ssbo @binding = 1 @role = "weight" @off = "w1off" wsh1 : array<float16>   // ... half-scale view
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 @span = "hidden*4" ho : array<float>    // [hidden] silu(gate)·up
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 hidden : uint
    @ssbo @binding = 6 @role = "weight" @off = "w3off" wqb3 : array<int8>   // w3 blob views
    @ssbo @binding = 7 @role = "weight" @off = "w3off" wsh3 : array<float16>
    @uniform @binding = 8 @role = "weight" act : uint        // gate activation: 0 = silu (swiglu), 1 = gelu (geglu, gemma line)
    @workgroup @template_gate = SPLIT red : float[64]   // [gate|up][32 lanes] cross-sg partials (the SPLIT reduce's)
    @template_constant SPLIT : bool = true

    [metal_kernel]
    def metal_q8_gemv_w13sw16 {
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let ix = lane / 4u                  // block-within-round: 8 blocks per sg per round
        let il = lane % 4u                  // 8-quant chunk within the block
        let r = elem_row()
        static_if (!SPLIT) {
            if (r >= hidden) {
                return
            }
        }
        var acc0 = 0.0
        var acc1 = 0.0
        var ib = first_block(ix)
        while (ib < nb) {
            let xi = ib * 8u + il * 2u
            let x0 = x[xi]
            let x1 = x[xi + 1u]
            let qb = (r * nb + ib) * 34u + 2u + il * 8u
            acc0 += q8_oct_dot(wqb1, qb, x0, x1) * float(wsh1[(r * nb + ib) * 17u])
            acc1 += q8_oct_dot(wqb3, qb, x0, x1) * float(wsh3[(r * nb + ib) * 17u])
            ib += block_step()
        }
        let s0 = simd_sum(acc0)
        let s1 = simd_sum(acc1)
        static_if (SPLIT) {
            let sg = gl_SubgroupID
            if (sg == 0u) {
                red[lane] = 0.0
                red[32u + lane] = 0.0
            }
            barrier()
            if (lane == 0u) {
                red[sg] = s0
                red[32u + sg] = s1
            }
            barrier()
            let tot0 = simd_sum(red[lane])
            let tot1 = simd_sum(red[32u + lane])
            if (sg == 0u && lane == 0u) {
                put_act(r, tot0, tot1)
            }
        } else {
            if (lane == 0u) {
                put_act(r, s0, s1)
            }
        }
    }

    // the output element this simdgroup (or, split, this threadgroup) owns
    def elem_row : uint {
        static_if (SPLIT) {
            return gl_WorkGroupID.x
        } else {
            return gl_WorkGroupID.x * 4u + gl_SubgroupID
        }
    }
    def first_block(ix : uint) : uint {
        static_if (SPLIT) {
            return gl_SubgroupID * 8u + ix
        } else {
            return ix
        }
    }
    def block_step : uint {
        static_if (SPLIT) {
            return 32u
        } else {
            return 8u
        }
    }
    def put_act(r : uint; g, u : float) {
        ho[r] = gk_act_mul(g, u, min(act, 1u))
    }
}

[metal_dispatch(name = "enc_w13sw_c", pso = "g_pso_w13sw", tgmem = "MetalQ8GemvW13Sw_metal_q8_gemv_w13sw16_msl_tgmem", tg = 128, grid = "hidden", params = "hidden : int64")]
class MetalQ8GemvW13Sw : MetalQ8GemvW13SwT {}

// the row-per-simdgroup stamp gates `red` off, so it carries no threadgroup memory and no tgmem= spec
[metal_dispatch(name = "enc_w13sw_sg_c", pso = "g_pso_w13sw_sg", tg = 128, grid = "hidden/4", params = "hidden : int64")]
class MetalQ8GemvW13SwSg : MetalQ8GemvW13SwT {
    override SPLIT = false
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

[ |> template_struct_instance]
class template MetalArgmaxT {
    @ssbo @binding = 0 lg : array<float>    // [nrows x n]
    @ssbo @binding = 1 tok : array<uint>    // [nrows] winner indices out
    @uniform @binding = 2 n : uint
    @workgroup redv : float[32]
    @workgroup redi : uint[32]
    @template_constant PER_ROW : bool = false

    [arch(at="../ARCHITECTURE_GPU_MTP_DECODE_PICK.md#greedy-chain")]
    def argmax_row {
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        var base = 0u    // nolint:LINT003,LINT010,LINT019 — the single stamp reads these inits, the
        var slot = 0u    // nolint:LINT003,LINT010,LINT019 — PER_ROW stamp overwrites them in its arm
        static_if (PER_ROW) {
            let row = gl_WorkGroupID.x
            base = row * n
            slot = row
        }
        var bv = -3.0e38
        var bi = 0u
        var j = lid
        while (j < n) {
            let v = lg[base + j]
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
            tok[slot] = wi
        }
    }
}

[metal_dispatch(name = "enc_argmax", pso = "g_pso_argmax", tgmem = "metal_argmax_msl_tgmem", tg = 1024, grid = "1")]
class MetalArgmax : MetalArgmaxT {
    [metal_kernel(name="metal_argmax_msl")]
    def metal_argmax {
        argmax_row()
    }
}

//! The Q8 token-table gather: dequant the rows of `nrows` token ids off the linear Q8 copy (the
//! whole-model 34B blob, bound at emb_q8_off's byte offset) into the residual input rows - the
//! device's embed_row, the decode step's, the greedy chain's and the prefill window's.
[metal_dispatch(name = "enc_embed_c", pso = "g_pso_emb", tg = 64, grid = "dim/64, nrows", params = "dim : int64, nrows : int64")]
class MetalEmbedQ8 {
    @ssbo @binding = 0 @role = "weight" @off = "eoff" q : array<int8>      // the embed blob slice, byte view (quants at 34*b + 2)
    @ssbo @binding = 1 @role = "weight" @off = "eoff" sc : array<float16>  // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 2 @off = "toff" tok : array<uint>     //! the tokens at a byte offset: row i of a token list
    @ssbo @binding = 3 @off = "xoff" xo : array<float>     //! [nrows x dim] residual input out, at row i's byte offset
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 @role = "weight" escale : float    // embed_scale (gemma: sqrt(dim); 1.0 elsewhere — g_one or an uploaded scalar)

    [metal_kernel(name="metal_embed_q8_msl")]
    def metal_embed_q8 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= dim) {
            return
        }
        let row = gl_WorkGroupID.y
        let e = tok[row] * dim + gid
        let b = e / 32u
        xo[row * dim + gid] = (float(q[b * 34u + 2u + e % 32u]) * float(sc[b * 17u])) * escale
    }
}

[metal_dispatch(name = "enc_argmax_rows", pso = "g_pso_argmax_rows", tgmem = "metal_argmax_rows_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64")]
class MetalArgmaxRows : MetalArgmaxT {
    override PER_ROW = true

    [metal_kernel(name="metal_argmax_rows_msl")]
    def metal_argmax_rows {
        argmax_row()
    }
}

//! byte `si` of a superblock's 16B sub-scale block
def public kq_strip_byte(sv : uint4; si : uint) : uint {
    let w = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
    return (w >> ((si & 3u) * 8u)) & 255u
}

//! byte `si` of the strip read signed (the split formats' i8 sub-scales)
def public kq_strip_sbyte(sv : uint4; si : uint) : float => float((int(kq_strip_byte(sv, si)) ^ 128) - 128)

//! an nv4 sub-block's scale off its 16-byte row: UE4M3 byte `si` at half (`ue4m3_half_f32`)
def public nv4_sub_scale(sv : uint4; si : uint) : float => ue4m3_half_f32(kq_strip_byte(sv, si))

//! the token-table gathers' element leaves (the embed walk and the
//! PLE walk both decode through them) take element `e` (0..31) of a
//! 32-element sub-block or block; this one, its word among four
def public el32_word(e : uint) : uint => (e % 16u) / 4u

//! element e's nibble off its word: the low halves, high from e = 16
def public el32_nib(u, e : uint) : uint => (u >> (8u * (e % 4u) + (e / 16u) * 4u)) & 15u

//! a per-32 five-bit code (q51, q5032): the nibble, bit e of qh fifth
def public el32_code5(u, qh, e : uint) : uint => el32_nib(u, e) | (((qh >> e) & 1u) << 4u)

//! q40's level: d * (q - 8)
def public q40_el_level(d : float; q : uint) : float => d * (float(q) - 8.0)

//! q51's level: d * q + m
def public q51_el_level(d, m : float; q : uint) : float => d * float(q) + m

//! q5032's level: d * (q - 16)
def public q5032_el_level(d : float; q : uint) : float => d * (float(q) - 16.0)

//! k4 / k5: element e of sub-block js's word among the nibble words
def public k45_el_word(js, e : uint) : uint => (js / 2u) * 8u + e / 4u

//! k4 / k5: element e's code - sub-block js's nibble of word u, k5's
//! fifth bit off its qh word `hu` (0 on k4)
def public k45_el_code(u, hu, js, e : uint) : uint => (
    ((u >> (8u * (e % 4u) + (js & 1u) * 4u)) & 15u) | (((hu >> (8u * (e % 4u) + js)) & 1u) << 4u))

//! k4 / k5 sub-block js's d * sc and dmin * mn off the 6-bit strip sv
def public k45_el_scales(sv : uint4; d, dmin : float; js : uint; var dsc, dmn : float&) {
    var sc = 0u
    var mn = 0u
    kmask_scmn(sv, js, sc, mn)
    dsc = d * float(sc)
    dmn = dmin * float(mn)
}

//! k4 / k5's level: d * sc * q less dmin * mn
def public k45_el_level(dsc, dmn : float; q : uint) : float => dsc * float(q) - dmn

struct EmbedKqArgs {
    dim : uint       //!< the table row width (whole superblocks)
    nsb : uint       //!< superblocks a table row
    escale : float   //!< embed_scale (gemma: sqrt(dim); 1.0 elsewhere)
}

//! the per-32 gather's args: the row width (its 32-blocks derived in the body), the embed scale, and the table's first block
//! past its bind
struct EmbedB32Args {
    dim : uint       //!< the table row width, whole 32-blocks
    escale : float
    b0 : uint        //!< `b32_bind_rem`'s blocks: the bind sits on the 16-byte grid below the table
}

//! The K-quant token-table gather: `nrows` token ids' rows off a format's blob-form planes (kq_scales_of / kq_quants_of),
//! dequantized in f32 by the format's own math; one invocation decodes one 16-element half of a 32-element sub-block,
//! a format stamp supplying `unit16`.
[ |> template_struct_instance]
class template MetalEmbedKqT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16>   //!< the d halves: the compact 16B block (soff) or the d tail (doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ksc : array<uint4>      //!< the same buffer, one 16B sub-scale block a superblock
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>       //!< the quant plane, uint view
    @ssbo @binding = 3 @off = "toff" tok : array<uint>         //!< one token id an output row
    @ssbo @binding = 4 @off = "xoff" xo : array<float>         //!< [nrows x dim] residual input rows
    @kargs ka : EmbedKqArgs
    @template_constant SB_UINTS : uint = 32u   //!< quant words a superblock: 40 on k5

    //! the 16 elements of half `ile` of sub-block `js` of superblock `blk`, scaled, at xo[ob ..]
    def abstract unit16(blk, js, ile, ob : uint) : void

    def gather_body {
        let u = gl_GlobalInvocationID.x
        if (u >= ka.dim / 16u) {
            return
        }
        let row = gl_WorkGroupID.y
        let jsl = u / 2u
        unit16(tok[row] * ka.nsb + jsl / 8u, jsl % 8u, u & 1u, row * ka.dim + u * 16u)
    }

    //! four decoded elements, scaled, at xo[o ..]
    def store4(o : uint; mv : float4) {
        xo[o] = mv.x * ka.escale
        xo[o + 1u] = mv.y * ka.escale
        xo[o + 2u] = mv.z * ka.escale
        xo[o + 3u] = mv.w * ka.escale
    }

    [metal_kernel]
    def metal_embed_kq {
        gather_body()
    }
}

//! k4 / k5: the 6-bit sc/min pair, d and dmin in the compact block; k5's fifth bit off the qh strip
[ |> template_struct_instance]
class template MetalEmbedK45T : MetalEmbedKqT {
    @template_constant QH0 : uint = 32u   //!< k5's qh strip's first word, past the nibbles
    @template_constant QH : bool = false

    def override unit16(blk, js, ile, ob : uint) {
        var dsc, dmn : float
        k45_el_scales(ksc[blk], float(kdh[blk * 8u]), float(kdh[blk * 8u + 1u]), js, dsc, dmn)
        for [unroll_full] (t in range(16)) {
            let e = ile * 16u + uint(t)
            let u = kqu[blk * SB_UINTS + k45_el_word(js, e)]
            static_if (QH) {
                let q = k45_el_code(u, kqu[blk * SB_UINTS + QH0 + e / 4u], js, e)
                xo[ob + uint(t)] = k45_el_level(dsc, dmn, q) * ka.escale
            } else {
                xo[ob + uint(t)] = k45_el_level(dsc, dmn, k45_el_code(u, 0u, js, e)) * ka.escale
            }
        }
    }
}

[metal_dispatch(name = "enc_embed_kq_k4", pso = "g_pso_emb_kq_k4", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedK4 : MetalEmbedK45T {}

[metal_dispatch(name = "enc_embed_kq_k5", pso = "g_pso_emb_kq_k5", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedK5 : MetalEmbedK45T {
    override SB_UINTS = 40u
    override QH = true
}

//! k6: (d * sc16) * (q - 32), q the low nibble pair plus two high bits
[metal_dispatch(name = "enc_embed_kq_k6", pso = "g_pso_emb_kq_k6", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedK6 : MetalEmbedKqT {
    def override unit16(blk, js, ile, ob : uint) {
        let half6 = js / 4u
        let gg = js % 4u
        let dsc = float(kdh[blk]) * kq_strip_sbyte(ksc[blk], half6 * 8u + gg * 2u + ile)
        for [unroll_full] (t in range(16)) {
            let k = uint(t) / 4u
            let c = uint(t) % 4u
            let u = kqu[blk * 48u + half6 * 16u + (gg & 1u) * 8u + ile * 4u + k]
            let hu = kqu[blk * 48u + 32u + half6 * 8u + ile * 4u + k]
            let q = ((u >> ((gg / 2u) * 4u + 8u * c)) & 15u) | (((hu >> (gg * 2u + 8u * c)) & 3u) << 4u)
            xo[ob + uint(t)] = (dsc * (float(q) - 32.0)) * ka.escale
        }
    }
}

//! the 4-bit codebook formats: iq4xs (d x the signed strip byte), iq4nl (a d a sub-block), q40 (iq4nl's d, the linear map q - 8),
//! iq3s4 (iq4xs's strip and d, the odd codebook), nv4 (a UE4M3 scale a 16-element half, the doubled e2m1 codebook)
[ |> template_struct_instance]
class template MetalEmbedIq4T : MetalEmbedKqT {
    @template_constant SCALE_D : bool = false   //!< iq4nl / q40: one f16 d a 32-element sub-block, no strip
    @template_constant LINEAR : bool = false    //!< q40: q - 8 in the codebook's place
    @template_constant IQODD : bool = false     //!< iq3s4: the odd codebook (`iq3s4_val`) in the codebook's place
    @template_constant NV4 : bool = false       //!< nv4: the 16 UE4M3 bytes at soff, byte 2 js + ile the half's scale, `mx4_val` in the codebook's place

    def override unit16(blk, js, ile, ob : uint) {
        var dsc : float
        static_if (NV4) {
            dsc = nv4_sub_scale(ksc[blk], js * 2u + ile)
        } else {
            static_if (SCALE_D) {
                dsc = float(kdh[blk * 8u + js])
            } else {
                dsc = float(kdh[blk]) * kq_strip_sbyte(ksc[blk], js)
            }
        }
        let qp = unsafe(addr(kqu[blk * SB_UINTS + js * 4u]))
        for [unroll_full] (t in range(16)) {
            let e = ile * 16u + uint(t)
            let q = el32_nib(unsafe(qp[el32_word(e)]), e)
            static_if (LINEAR) {
                xo[ob + uint(t)] = q40_el_level(dsc, q) * ka.escale
            } else {
                static_if (IQODD) {
                    xo[ob + uint(t)] = (dsc * iq3s4_val(q)) * ka.escale
                } else {
                    static_if (NV4) {
                        xo[ob + uint(t)] = (dsc * mx4_val(q)) * ka.escale
                    } else {
                        xo[ob + uint(t)] = (dsc * iq4_lut(q)) * ka.escale
                    }
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_embed_kq_iq4xs", pso = "g_pso_emb_kq_iq4xs", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedIq4xs : MetalEmbedIq4T {}

[metal_dispatch(name = "enc_embed_kq_iq3s4", pso = "g_pso_emb_kq_iq3s4", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedIq3s4 : MetalEmbedIq4T {
    override IQODD = true
}

[metal_dispatch(name = "enc_embed_kq_iq4nl", pso = "g_pso_emb_kq_iq4nl", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedIq4nl : MetalEmbedIq4T {
    override SCALE_D = true
}

[metal_dispatch(name = "enc_embed_kq_q40", pso = "g_pso_emb_kq_q40", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedQ40 : MetalEmbedIq4T {
    override SCALE_D = true
    override LINEAR = true
}

[metal_dispatch(name = "enc_embed_kq_nv4", pso = "g_pso_emb_kq_nv4", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedNv4 : MetalEmbedIq4T {
    override NV4 = true
}

//! The per-32 token-table gather over a per-32 plane pair (`b32_planes_of`): one invocation a 16-element half of a 32-block -
//! q5_1's d * (q + 16 h) + m, q5032's d * (q + 16 h - 16), the codebook formats' d * code (iq4nl32's table, q4032's q - 8)
[ |> template_struct_instance]
class template MetalEmbedB32T {
    @ssbo @binding = 0 @role = "weight" @off = "soff" kdh : array<float16>   //!< the scale plane: d (and m on q5_1) a block
    @ssbo @binding = 1 @role = "weight" @off = "qoff" kqu : array<uint>       //!< the quant plane, uint view
    @ssbo @binding = 2 @off = "toff" tok : array<uint>         //!< one token id an output row
    @ssbo @binding = 3 @off = "xoff" xo : array<float>         //!< [nrows x dim] residual input rows
    @kargs ka : EmbedB32Args
    @template_constant Q51 : bool = false
    @template_constant Q40L : bool = false
    @template_constant Q50 : bool = false   //!< a Q5_0 table (q5032): q51's quant block under one d, d * (q + 16 h - 16)
    @template_constant BE : uint = 32u      //!< a block's weights, KQ_B32_ELEMS (`test_kqformat.das` pins it)
    @template_constant QW : uint = 4u       //!< quant words a block: 5 on q51, q5032 (qh last)
    @template_constant SH : uint = 1u       //!< scale halves a block: 2 on q51 (d then m)

    [metal_kernel]
    def metal_embed_b32 {
        let u = gl_GlobalInvocationID.x
        if (u >= ka.dim / 16u) {
            return
        }
        let row = gl_WorkGroupID.y
        let blk = ka.b0 + tok[row] * (ka.dim / BE) + u / 2u
        let half = u & 1u
        let ob = row * ka.dim + u * 16u
        let d = float(kdh[blk * SH])
        let p = unsafe(addr(kqu[blk * QW]))
        static_if (Q51 || Q50) {
            let qh = unsafe(p[QW - 1u])
            for [unroll_full] (t in range(16)) {
                let e = half * 16u + uint(t)
                let q = el32_code5(unsafe(p[el32_word(e)]), qh, e)
                static_if (Q51) {
                    xo[ob + uint(t)] = q51_el_level(d, float(kdh[blk * SH + 1u]), q) * ka.escale
                } else {
                    xo[ob + uint(t)] = q5032_el_level(d, q) * ka.escale
                }
            }
        } else {
            for [unroll_full] (t in range(16)) {
                let e = half * 16u + uint(t)
                let q = el32_nib(unsafe(p[el32_word(e)]), e)
                xo[ob + uint(t)] = (d * b32_code_val(q, Q40L)) * ka.escale
            }
        }
    }
}

[metal_dispatch(name = "enc_q51_embed_b32", pso = "g_pso_emb_b32_q51", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64", requires = "ka.dim % 32")]
class MetalEmbedB32Q51 : MetalEmbedB32T {
    override Q51 = true
    override QW = 5u
    override SH = 2u
}

[metal_dispatch(name = "enc_iq4nl32_embed_b32", pso = "g_pso_emb_b32_iq4nl32", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64", requires = "ka.dim % 32")]
class MetalEmbedB32Iq4nl32 : MetalEmbedB32T {}

[metal_dispatch(name = "enc_q4032_embed_b32", pso = "g_pso_emb_b32_q4032", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64", requires = "ka.dim % 32")]
class MetalEmbedB32Q4032 : MetalEmbedB32T {
    override Q40L = true
}

[metal_dispatch(name = "enc_q5032_embed_b32", pso = "g_pso_emb_b32_q5032", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64", requires = "ka.dim % 32")]
class MetalEmbedB32Q5032 : MetalEmbedB32T {
    override Q50 = true
    override QW = 5u
}

//! k3: d x the signed sub-scale, the low two bits plus the high bit, less 4 d where the high bit is clear
[metal_dispatch(name = "enc_embed_kq_k3", pso = "g_pso_emb_kq_k3", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedK3 : MetalEmbedKqT {
    def override unit16(blk, js, ile, ob : uint) {
        let hf3 = js / 4u
        let gg3 = js % 4u
        let dsc = float(kdh[blk]) * kq_strip_sbyte(ksc[blk], hf3 * 8u + gg3 * 2u + ile)
        let dmn = dsc * 4.0
        for [unroll_full] (t in range(16)) {
            let k = uint(t) / 4u
            let c = uint(t) % 4u
            let u = kqu[blk * 24u + hf3 * 8u + ile * 4u + k]
            let hu = kqu[blk * 24u + 16u + ile * 4u + k]
            let q = ((u >> (gg3 * 2u + 8u * c)) & 3u) | (((hu >> (hf3 * 4u + gg3 + 8u * c)) & 1u) << 2u)
            xo[ob + uint(t)] = (dsc * float(q) - dmn) * ka.escale
        }
    }
}

//! k2: the nibble pair scale and min off one strip byte, d and dmin in the 4B tail
[metal_dispatch(name = "enc_embed_kq_k2", pso = "g_pso_emb_kq_k2", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedK2 : MetalEmbedKqT {
    def override unit16(blk, js, ile, ob : uint) {
        let hf3 = js / 4u
        let gg3 = js % 4u
        let pb = kq_strip_byte(ksc[blk], hf3 * 8u + gg3 * 2u + ile)
        let dsc = float(kdh[blk * 2u]) * float(pb & 15u)
        let dmn = float(kdh[blk * 2u + 1u]) * float(pb >> 4u)
        let qp = unsafe(addr(kqu[blk * 16u + hf3 * 8u + ile * 4u]))
        for [unroll_full] (t in range(16)) {
            let q = (unsafe(qp[t / 4]) >> (gg3 * 2u + 8u * (uint(t) % 4u))) & 3u
            xo[ob + uint(t)] = (dsc * float(q) - dmn) * ka.escale
        }
    }
}

//! iq3s: the 9-bit grid index (byte + qh bit), a sign nibble a grid word, d x the signed strip byte
[metal_dispatch(name = "enc_embed_kq_iq3s", pso = "g_pso_emb_kq_iq3s", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedIq3s : MetalEmbedKqT {
    def override unit16(blk, js, ile, ob : uint) {
        let dsc = float(kdh[blk]) * kq_strip_sbyte(ksc[blk], js)
        let qsw = kqu[blk * 26u + js * 2u + ile]
        let qh3 = (kqu[blk * 26u + 16u + js / 4u] >> ((js % 4u) * 8u)) & 255u
        let sgw3 = kqu[blk * 26u + 18u + js]
        for [unroll_full] (k in range(4)) {
            let b3 = ile * 4u + uint(k)
            let idx = ((qsw >> (8u * uint(k))) & 255u) | ((qh3 << (8u - b3)) & 256u)
            let nib = (sgw3 >> ((b3 / 2u) * 8u + (b3 % 2u) * 4u)) & 15u
            store4(ob + uint(k) * 4u, u8x4_f4(iq3s_gw(int(idx))) * iq_sign_apply(float4(dsc), nib))
        }
    }
}

//! iq3xxs: the halved 8-bit grid, the 7-bit sign code a grid pair, d x the signed strip byte
[metal_dispatch(name = "enc_embed_kq_iq3xxs", pso = "g_pso_emb_kq_iq3xxs", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedIq3xxs : MetalEmbedKqT {
    def override unit16(blk, js, ile, ob : uint) {
        let dsc = float(kdh[blk]) * kq_strip_sbyte(ksc[blk], js)
        let qsw = kqu[blk * 24u + js * 2u + ile]
        let aux3 = kqu[blk * 24u + 16u + js]
        for [unroll_full] (k in range(4)) {
            let sgb = ksign7((aux3 >> (7u * (ile * 2u + uint(k) / 2u))) & 127u)
            let nib = uint(k) % 2u == 0u ? sgb & 15u : sgb >> 4u
            store4(ob + uint(k) * 4u, u8x4_f4(iq3xxs_gw(int((qsw >> (8u * uint(k))) & 255u))) * iq_sign_apply(float4(dsc), nib))
        }
    }
}

//! the 2-bit grid formats: one signed grid word of four magnitudes a k; d x the unsigned strip byte (a sub-block's half
//! on iq2s / iq2xs, a sub-block on iq2xxs)
[ |> template_struct_instance]
class template MetalEmbedIq2T : MetalEmbedKqT {
    def abstract grid_word(blk, js, ile, k : uint; var sw : uint&) : void
    def unit_scale(blk, js, ile : uint) : float => (float(kdh[blk]) *
                                                    float(kq_strip_byte(ksc[blk], js * 2u + ile)))

    def override unit16(blk, js, ile, ob : uint) {
        let dsc = unit_scale(blk, js, ile)
        for [unroll_full] (k in range(4)) {
            var sw = 0u
            grid_word(blk, js, ile, uint(k), sw)
            for [unroll_full] (c in range(4)) {
                xo[ob + uint(k) * 4u + uint(c)] = (dsc * iq3s_sx(sw, uint(c))) * ka.escale
            }
        }
    }
}

[metal_dispatch(name = "enc_embed_kq_iq2s", pso = "g_pso_emb_kq_iq2s", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedIq2s : MetalEmbedIq2T {
    def override grid_word(blk, js, ile, k : uint; var sw : uint&) {
        let l2 = ile * 2u + k / 2u
        let qw2 = kqu[blk * 18u + js]
        let qh2 = (kqu[blk * 18u + 16u + js / 4u] >> ((js % 4u) * 8u)) & 255u
        let idx2 = (((qw2 >> (8u * l2)) & 255u) | ((qh2 << (8u - 2u * l2)) & 0x300u)) * 2u + k % 2u
        let sgb2 = (kqu[blk * 18u + 8u + js] >> (8u * l2)) & 255u
        sw = iq3s_signed(iq2s_gw(int(idx2)), k % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u)
    }
}

[metal_dispatch(name = "enc_embed_kq_iq2xs", pso = "g_pso_emb_kq_iq2xs", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedIq2xs : MetalEmbedIq2T {
    def override grid_word(blk, js, ile, k : uint; var sw : uint&) {
        let w16 = (kqu[blk * 16u + js * 2u + ile] >> (16u * (k / 2u))) & 0xFFFFu
        let sgb2 = ksign7(w16 >> 9u)
        sw = iq3s_signed(iq2xs_gw(int((w16 & 511u) * 2u + k % 2u)), k % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u)
    }
}

[metal_dispatch(name = "enc_embed_kq_iq2xxs", pso = "g_pso_emb_kq_iq2xxs", tg = 64, grid = "units/64, nrows", params = "units : int64, nrows : int64")]
class MetalEmbedIq2xxs : MetalEmbedIq2T {
    [unused_argument(ile)]
    def override unit_scale(blk, js, ile : uint) : float => float(kdh[blk]) * float(kq_strip_byte(ksc[blk], js))

    def override grid_word(blk, js, ile, k : uint; var sw : uint&) {
        let q3 = ile * 2u + k / 2u
        let bidx = (kqu[blk * 16u + js * 2u] >> (8u * q3)) & 255u
        let sgb2 = ksign7((kqu[blk * 16u + js * 2u + 1u] >> (7u * q3)) & 127u)
        sw = iq3s_signed(iq2xxs_gw(int(bidx * 2u + k % 2u)), k % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u)
    }
}

//! The threadgroup reductions in value form over the shared `GkWgReduce` slab's first slot - tg_sum_all's contract: every
//! lane holds the result after one barrier, and two folds in a row take a barrier between them.
[ |> template_struct_instance]
class template MetalTgReduceBase : GkWgReduce {
    def tg_sum(v : float) : float => tg_sum_all(v, part)
    def tg_max(v : float) : float => tg_max_all(v, part)

    //! rsqrt(mean + eps) of the squares summed into ss - the RMS norm's inverse scale
    def tg_rms_inv(sum_sq : float; dim : uint; eps : float) : float => rsqrt(tg_sum_all(sum_sq, part) / float(dim) + eps)
}

//! The residual step's Metal stamps on `GkAddRmsRows`, a threadgroup a row (its id the row), `rsqrt`, the loops striding the threadgroup:
//! the first `SLAB` columns staged in the slab, the columns past it re-read from the rows written and the partner rows; the window's
//! builders at 256 lanes, the decode rows' at 256 within the slab and 1024 past it (`enc_residual`'s pick); `PRE` the sandwich arm
[ |> template_struct_instance]
class template MetalAddRmsT : GkAddRmsRows {
    @workgroup xrow : float[SLAB]
    @template_constant SLAB : int = 4096   //!< the row stash's floats: ADD_RMS_MAX_DIM
    override LANES = 0u
    override RSQRT = true
    override PRE = false
    override X4 = false
    override ARMS = false

    def override stash(k : uint; v : float) : void {
        if (k < uint(SLAB)) {
            xrow[k] = v
        }
    }
    def override stashed(k : uint) : float => k < uint(SLAB) ? xrow[k] : xs[gl_WorkGroupID.x * pa.dim + k]
    def override pre_partner(base, k : uint) : float => k < uint(SLAB) ? xrow[k] : apart(base, k)

    //! `seam_norms`' layout: the post norm at row 0, the next norm past it on a PRE stamp
    def override norm_base : uint {
        static_if (PRE) {
            return pa.dim
        } else {
            return 0u
        }
    }

    def override pre_base : uint => 0u
}

//! the plain step, its next norm out as floats
[metal_dispatch(name = "enc_addrms_c", pso = "g_pso_addrms", tgmem = "metal_add_rms_msl_tgmem", tg = 256, grid = "1"),
    metal_dispatch(name = "enc_add_rms_b_c", pso = "g_pso_addrms", tgmem = "metal_add_rms_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "pf_enc_add_rms_b_c", pso = "g_pso_addrms", tgmem = "metal_add_rms_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "enc_add_rms_bw_c", pso = "g_pso_addrms", tgmem = "metal_add_rms_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64")]
class MetalAddRms : MetalAddRmsT {
    override OUT_F32 = true

    [metal_kernel(name="metal_add_rms_msl")]
    def metal_add_rms {
        add_rms_row(gl_WorkGroupID.x)
    }
}

//! ... and the f16 twin beside (the dual-store stamp the half-x GEMM forms read)
[metal_dispatch(name = "enc_add_rms_bhx_c", pso = "g_pso_addrms_hx", tgmem = "metal_add_rms_hx_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "pf_enc_add_rms_bhx_c", pso = "g_pso_addrms_hx", tgmem = "metal_add_rms_hx_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "enc_add_rms_bhxw_c", pso = "g_pso_addrms_hx", tgmem = "metal_add_rms_hx_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64")]
class MetalAddRmsHX : MetalAddRmsT {
    override OUT_F32 = true
    override HX = true

    [metal_kernel(name="metal_add_rms_hx_msl")]
    def metal_add_rms_hx {
        add_rms_row(gl_WorkGroupID.x)
    }
}

//! the sandwich seam: the branch under its post norm, the add, the next norm out as floats
[metal_dispatch(name = "enc_preaddrms_c", pso = "g_pso_preaddrms", tgmem = "metal_pre_add_rms_msl_tgmem", tg = 256, grid = "1"),
    metal_dispatch(name = "enc_preaddrms_b_c", pso = "g_pso_preaddrms", tgmem = "metal_pre_add_rms_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "pf_enc_preaddrms_b_c", pso = "g_pso_preaddrms", tgmem = "metal_pre_add_rms_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "enc_preaddrms_bw_c", pso = "g_pso_preaddrms", tgmem = "metal_pre_add_rms_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64")]
class MetalPreAddRms : MetalAddRmsT {
    override PRE = true
    override PRE_SWITCH = false
    override OUT_F32 = true

    [metal_kernel(name="metal_pre_add_rms_msl")]
    def metal_pre_add_rms {
        add_rms_row(gl_WorkGroupID.x)
    }
}

//! ... and the f16 twin beside
[metal_dispatch(name = "enc_preaddrms_bhx_c", pso = "g_pso_preaddrms_hx", tgmem = "metal_pre_add_rms_hx_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "pf_enc_preaddrms_bhx_c", pso = "g_pso_preaddrms_hx", tgmem = "metal_pre_add_rms_hx_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "enc_preaddrms_bhxw_c", pso = "g_pso_preaddrms_hx", tgmem = "metal_pre_add_rms_hx_msl_tgmem", tg = 1024, grid = "nrows",
        params = "nrows : int64")]
class MetalPreAddRmsHX : MetalAddRmsT {
    override PRE = true
    override PRE_SWITCH = false
    override OUT_F32 = true
    override HX = true

    [metal_kernel(name="metal_pre_add_rms_hx_msl")]
    def metal_pre_add_rms_hx {
        add_rms_row(gl_WorkGroupID.x)
    }
}

//! the sandwich seam with no norm stored: the stream rows updated alone (the per-layer-embedding residual, the last layer's step)
[metal_dispatch(name = "enc_preadd_c", pso = "g_pso_preadd", tgmem = "metal_pre_add_msl_tgmem", tg = 256, grid = "1"),
    metal_dispatch(name = "enc_preadd_b_c", pso = "g_pso_preadd", tgmem = "metal_pre_add_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "pf_enc_preadd_b_c", pso = "g_pso_preadd", tgmem = "metal_pre_add_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "enc_preadd_bw_c", pso = "g_pso_preadd", tgmem = "metal_pre_add_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64")]
class MetalPreAdd : MetalAddRmsT {
    override PRE = true
    override PRE_SWITCH = false

    [metal_kernel(name="metal_pre_add_msl")]
    def metal_pre_add {
        add_rms_row(gl_WorkGroupID.x)
    }
}

//! The fused rope and K/V store at one row, the f16-mirror stamp of the template both homes stamp (`GkRopeKvStore`): the q and
//! k projections in the one QKV buffer (ksrc its second view, vsrc its view at the v offset), the mirror K and V rows bound at
//! their (layer, pos) offsets, this position's rope row bound at its offset.
[metal_dispatch(name = "enc_rpst16_c", pso = "g_pso_rpst16", tg = 64, grid = "npairs/64", params = "npairs : int64")]
class MetalRopeStoreF16 : GkRopeKvStore {
    typedef KT = float16
    override BOUND_VIEWS = true

    [metal_kernel]
    def metal_rope_store {
        rope_kv_one(gl_GlobalInvocationID.x)
    }
}

//! the f32-mirror stamp of `GkRopeKvStore`: straight stores, no clamp
[metal_dispatch(name = "enc_rpst32_c", pso = "g_pso_rpst32", tg = 64, grid = "npairs/64", params = "npairs : int64")]
class MetalRopeStoreF32 : GkRopeKvStore {
    typedef KT = float
    override CLAMP16 = false
    override BOUND_VIEWS = true

    [metal_kernel]
    def metal_rope_store {
        rope_kv_one(gl_GlobalInvocationID.x)
    }
}

//! the fused q/k norm, rope and K/V store on the template both homes stamp (`GkQknRopeKv`) at one row: the mirror K and V rows
//! bound at the row's offsets, the position's cos and sin rows bound at theirs; a threadgroup a q head, then a k head with its V
[ |> template_struct_instance]
class template MetalQknRopeOneT : GkQknRopeKv {
    @ssbo @binding = 6 @role = "read" @off = "soff" tsin : array<float>   //! the rope table's sin rows
    override LANES = 256u
    override EPT = 1
    override V_WG = false

    [unused_argument(rowi)]
    def override mir_base(rowi : uint) : uint2 => uint2(0u, 0u)

    def override rope_sin(cb, j : uint) : float => tsin[cb + j]
}

[metal_dispatch(name = "enc_qkn_rope16", pso = "g_pso_qkn16", tgmem = "metal_qkn_rope16_msl_tgmem", tg = "npair", grid = "nwg", params = "nwg : int64, npair : int64")]
class MetalQknRope16 : MetalQknRopeOneT {
    typedef KT = float16

    [metal_kernel(name="metal_qkn_rope16_msl")]
    def metal_qkn_rope16 {
        qkn_head(0u, gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_qkn_rope32", pso = "g_pso_qkn32", tgmem = "metal_qkn_rope32_msl_tgmem", tg = "npair", grid = "nwg", params = "nwg : int64, npair : int64")]
class MetalQknRope32 : MetalQknRopeOneT {
    typedef KT = float
    override CLAMP16 = false

    [metal_kernel(name="metal_qkn_rope32_msl")]
    def metal_qkn_rope32 {
        qkn_head(0u, gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! the rows form of the same template over the route table: row b's mirror K and V bases its table entry's, the per-layer stride
//! `layerbase` layers of which sit ahead, at depth - 1; the cos and sin rows `cosrow` apart
//! derives GkQknRopeKv, the template both homes stamp
[ |> template_struct_instance]
class template MetalQknRopeRowsT : GkQknRopeKv {
    @ssbo @binding = 6 @role = "read" @off = "soff" tsin : array<float>
    @ssbo @binding = 8 @role = "read" @off = "rtoff" rt : array<uint4>   //! a row's [k base, v base, per-layer stride, depth]
    override LANES = 256u
    override EPT = 1
    override V_WG = true

    def row_ahead(lay, depth : uint) : uint => pa.layerbase * lay + pa.layr + kv_ring_row(depth - 1u, pa.ring) * pa.kvd

    def override mir_base(rowi : uint) : uint2 => uint2(rt[rowi].x + row_ahead(rt[rowi].z, rt[rowi].w), rt[rowi].y + row_ahead(rt[rowi].z, rt[rowi].w))

    def override rope_sin(cb, j : uint) : float => tsin[cb + j]
}

[metal_dispatch(name = "enc_qkn_rope_b16", pso = "g_pso_qknb16", tgmem = "metal_qkn_rope_b16_msl_tgmem", tg = "npair", grid = "nwg, nrows", params = "nwg : int64, nrows : int64, npair : int64")]
class MetalQknRopeB16 : MetalQknRopeRowsT {
    typedef KT = float16

    [metal_kernel(name="metal_qkn_rope_b16_msl")]
    def metal_qkn_rope_b16 {
        qkn_head(gl_WorkGroupID.y, gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_qkn_rope_b32", pso = "g_pso_qknb32", tgmem = "metal_qkn_rope_b32_msl_tgmem", tg = "npair", grid = "nwg, nrows", params = "nwg : int64, nrows : int64, npair : int64")]
class MetalQknRopeB32 : MetalQknRopeRowsT {
    typedef KT = float
    override CLAMP16 = false

    [metal_kernel(name="metal_qkn_rope_b32_msl")]
    def metal_qkn_rope_b32 {
        qkn_head(gl_WorkGroupID.y, gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

// Shared q8_0-mirror rope+store stages, spliced by the single and batched kernels below: the
// q pair rope, the 3-arm K/V vals fill (V copy / NEOX per-element / NORM pair), the amax scale,
// and the 34B-block store. Base offsets parameterize the single (0) vs batched (row) layouts.
class MetalRopeStoreQ8Base {
    def q8_rope_q(var dst : array<float>; tc, ts, bs : array<float>; gid, half, head_size, qhs, neox, has_bias, rot, qkb0, tb : uint) {
        let j = gid % half
        let hbase = (gid / half) * head_size
        let qkb = qkb0 + (gid / half) * (qhs - head_size)   //! a gated model's q head sits `qhs` apart in `dst`, its bias row `head_size`
        let part = rot != 0u && rot != head_size
        let halfr = part ? rot / 2u : half
        // the threads identity-covering [rot, hs) still add the bias
        let rotp = j < halfr
        let pairs = neox != 0u || part
        let i = pairs ? (rotp ? hbase + j : hbase + rot + (j - halfr)) : hbase + 2u * j
        let i2 = pairs ? (rotp ? i + halfr : i + (half - halfr)) : i + 1u
        let fcr = rotp ? tc[tb + j] : 1.0
        let fci = rotp ? ts[tb + j] : 0.0
        let v0 = dst[qkb + i] + (has_bias != 0u ? bs[i] : 0.0)
        let v1 = dst[qkb + i2] + (has_bias != 0u ? bs[i2] : 0.0)
        dst[qkb + i] = v0 * fcr - v1 * fci
        dst[qkb + i2] = v0 * fci + v1 * fcr
    }

    //! one K/V element of the new row as the mirror stores it: a K element roped against its pair partner (the adjacent
    //! element, or the one `halfr` away on a NEOX or partial rope; past the rotated prefix the element itself), a V element raw
    def q8_kv_elem(src : array<float>; vsrc : array<float>; tc, ts, bs : array<float>;
                   kbase, vbase, tb, ge, head_size, half, qd, kvd, neox, has_bias, rot : uint; isV : bool; var v : float&) {
        if (isV) {
            v = vsrc[vbase + ge] + (has_bias != 0u ? bs[qd + kvd + ge] : 0.0)
        } else {
            let part = rot != 0u && rot != head_size
            let halfr = part ? rot / 2u : half
            let hp = ge % head_size
            let mine = src[kbase + ge] + (has_bias != 0u ? bs[qd + ge] : 0.0)
            if (part && hp >= rot) {
                v = mine
            } elif (neox != 0u || part) {
                let isHi = hp >= halfr
                let j = isHi ? hp - halfr : hp
                let oi = isHi ? ge - halfr : ge + halfr
                let other = src[kbase + oi] + (has_bias != 0u ? bs[qd + oi] : 0.0)
                v = isHi ? other * ts[tb + j] + mine * tc[tb + j] : mine * tc[tb + j] - other * ts[tb + j]
            } else {
                let j = hp / 2u
                let odd = (ge & 1u) != 0u
                let oi = odd ? ge - 1u : ge + 1u
                let other = src[kbase + oi] + (has_bias != 0u ? bs[qd + oi] : 0.0)
                v = odd ? other * ts[tb + j] + mine * tc[tb + j] : mine * tc[tb + j] - other * ts[tb + j]
            }
        }
    }

    //! a simdgroup is one 32-block: its lanes hold the block's elements, the block max is a simd reduction, lane 0 writes the
    //! scale and every lane its quant - quantize_q8kv_row's exact semantics, so CPU fallback steps read the same codec
    def q8_store_lane(var sh : array<float16>; var qb : array<int8>; v : float; shb, qbb, blk, lane : uint) {
        let amax = simd_max(abs(v))
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (lane == 0u) {
            sh[shb + blk * 17u] = float16(min(d, 65504.0))   // the half's largest finite, as the CPU codec clamps
        }
        qb[qbb + blk * 34u + 2u + lane] = int8(round(v * id))
    }

    //! the first K/V lane: the q pairs' threads rounded up to a simdgroup boundary, so a block never straddles two simdgroups
    def q8_kv_lane0(npairs_q : uint) : uint => (npairs_q + 31u) & ~31u

    //! the K/V lane `gid` past the q pairs: `inside` the store's lanes (whole simdgroups from the next boundary), the element `ge`
    //! of the K row or, `isV`, of the V row
    def q8_kv_lane(gid, npairs_q, kvd : uint; var inside : bool&; var isV : bool&; var ge : uint&) {
        let kv0 = q8_kv_lane0(npairs_q)
        inside = gid >= kv0 && gid < kv0 + 2u * kvd
        let e = gid - kv0
        isV = e >= kvd
        ge = isV ? e - kvd : e
    }
}

// q8_0-mirror rope + KV-row store: q pairs rope in place; K/V rows quantize per 32-block, a simdgroup a block (a lane an
// element, the block max a simd reduction) matching quantize_q8kv_row's exact semantics, so CPU fallback steps read the
// same codec. K lanes rope against their pair partner first; V lanes copy raw.
[metal_dispatch(name = "enc_rpst_q8_c", pso = "g_pso_rpst_q8", tg = 64, grid = "nthreads/64", params = "nthreads : int64")]
class MetalRopeStoreQ8 : MetalRopeStoreQ8Base {
    @ssbo @binding = 0 @role = "read" qk : array<float>    //! k at [qd, qd + kvd) of the projection row (q ropes in `qr`)
    @ssbo @binding = 1 @role = "alias" @off = "vsoff" vs : array<float>    // raw v (the QKV buffer at the v offset)
    @ssbo @binding = 2 @role = "write" @off = "koff" ksh : array<float16> // mirror K row, HALF-SCALE view (bound at the (layer, pos) row offset)
    @ssbo @binding = 3 @role = "alias" @off = "koff" kqb : array<int8>    // the SAME K row, BYTE view
    @ssbo @binding = 4 @role = "write" @off = "voff" vsh : array<float16>
    @ssbo @binding = 5 @role = "alias" @off = "voff" vqb : array<int8>
    @ssbo @binding = 6 @role = "weight" tcos : array<float>  // [head_size/2] this position's rope row
    @ssbo @binding = 7 @role = "weight" tsin : array<float>
    @ssbo @binding = 8 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 9 ka : RopeStoreQ8Args
    @ssbo @binding = 10 @role = "write" qr : array<float>   //! the q rows roped in place: `qk` itself, or a gated model's [q | gate] heads

    [metal_kernel(name="metal_rope_store_q8_msl")]
    def metal_rope_store_q8 {
        let qd = ka.qd
        let head_size = ka.head_size
        let npairs_q = ka.npairs_q
        let nblk = ka.nblk
        let neox = ka.neox
        let has_bias = ka.has_bias
        let rot = ka.rot
        let gid = gl_GlobalInvocationID.x
        let half = head_size / 2u
        if (gid < npairs_q) {
            q8_rope_q(qr, tcos, tsin, bias, gid, half, head_size, sq_qhs(ka.qhs, head_size), neox, has_bias, rot, 0u, 0u)
            return
        }
        let kvd = nblk * 32u
        var inside = false
        var isV = false
        var ge = 0u
        q8_kv_lane(gid, npairs_q, kvd, inside, isV, ge)
        if (!inside) {   // whole simdgroups: a block's 32 lanes return together
            return
        }
        var v = 0.0
        q8_kv_elem(qk, vs, tcos, tsin, bias, qd, 0u, 0u, ge, head_size, half, qd, kvd, neox, has_bias, rot, isV, v)
        if (isV) {
            q8_store_lane(vsh, vqb, v, 0u, 0u, ge / 32u, ge % 32u)
        } else {
            q8_store_lane(ksh, kqb, v, 0u, 0u, ge / 32u, ge % 32u)
        }
    }
}

// Shared tq4-mirror rope+store stages, spliced by the single and batched kernels below: the
// buf-resident FWHT butterfly, the per-32-block signed-max quantize scalars, and the packed
// nibble store. Base byte offsets parameterize the single (0) vs batched (arena row) layouts.
class template MetalRopeStoreTq4Base : GkBlkSignedMax {
    @workgroup buf : float[512]     // <= max head_size (gemma4 global heads are 512)

    def tq4_rope_elem(qk, tc, ts, bs : array<float>; base, bbase, tb, lid, hs, neox, has_bias, rot : uint; var v : float&) {
        let half = hs / 2u
        let part = rot != 0u && rot != hs
        let halfr = part ? rot / 2u : half
        let mine = qk[base + lid] + (has_bias != 0u ? bs[bbase + lid] : 0.0)
        if (part && lid >= rot) {
            v = mine
        } else {
            let pairs = neox != 0u || part
            let isHi = pairs ? lid >= halfr : (lid & 1u) != 0u
            let j = pairs ? (isHi ? lid - halfr : lid) : lid / 2u
            let fcr = tc[tb + j]
            let fci = ts[tb + j]
            let po = pairs ? (isHi ? lid - halfr : lid + halfr) : (lid ^ 1u)
            let other = qk[base + po] + (has_bias != 0u ? bs[bbase + po] : 0.0)
            v = isHi ? other * fci + mine * fcr : mine * fcr - other * fci
        }
    }

    //! the caller seeds buf[lid] and scales what it reads back - the 1/sqrt(n) and the signs are its own
    def fwht_butterfly(lid, hs : uint) {
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
    }

    def tq4_fwht_signs_row(var v : float&; sg : array<float>; lid, hs : uint) {
        buf[lid] = v * sg[lid]
        fwht_butterfly(lid, hs)
        v = buf[lid] * (1.0 / sqrt(float(hs)))
    }

    def tq4_quant(v : float; var d : float&; var qv : int&; var qother : int&; sl : uint) {
        var mv = v
        blk_signed_max(sl, mv)   // per-32-block quantize: block == simdgroup, the FIRST max-abs element sets the scale
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
    @ssbo @binding = 11 @role = "readwrite" qr : array<float>   //! the q rows roped and rotated in place: `qk` itself, or a gated model's [q | gate] heads

    [metal_kernel(name="metal_rope_store_tq4_msl")]
    def metal_rope_store_tq4 {
        let qd = ka.qd
        let head_size = ka.head_size
        let nheads = ka.nheads
        let kv_mul = ka.kv_mul
        let neox = ka.neox
        let has_bias = ka.has_bias
        let rot = ka.rot
        let tgid = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        let hs = head_size
        let nkvh = nheads / kv_mul
        let kvd = nkvh * hs
        let isQ = tgid < nheads
        let isK = !isQ && tgid < nheads + nkvh
        // this head's element run: q heads in the q region, k heads in the k region, v heads in vs
        let hh = isQ ? tgid : (isK ? tgid - nheads : tgid - nheads - nkvh)
        let base = isQ ? tgid * hs : (isK ? qd + hh * hs : hh * hs)
        // the element's index in the [qd | kvd | kvd] qkv vector — the bias plane's layout
        let bbase = isQ ? tgid * hs : (isK ? qd + hh * hs : qd + kvd + hh * hs)
        let qb = tgid * sq_qhs(ka.qhs, hs)
        var v = 0.0
        if (isQ) {
            tq4_rope_elem(qr, tcos, tsin, bias, qb, bbase, 0u, lid, hs, neox, has_bias, rot, v)
        } elif (isK) {
            tq4_rope_elem(qk, tcos, tsin, bias, base, bbase, 0u, lid, hs, neox, has_bias, rot, v)
        } else {
            v = vs[base + lid] + (has_bias != 0u ? bias[bbase + lid] : 0.0)
        }
        tq4_fwht_signs_row(v, signs, lid, hs)
        if (isQ) {
            qr[qb + lid] = v
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
class MetalFwhtUnsign : MetalRopeStoreTq4Base {
    @ssbo @binding = 0 yo : array<float>    // [rows x qstride] attention out, un-rotated in place
    @ssbo @binding = 1 @role = "weight" signs : array<float>
    @uniform @binding = 2 head_size : uint
    @uniform @binding = 3 qstride : uint
    @ssbo @binding = 4 @role = "read" @default = g_one qg : array<float>   //! a gated model's [q | gate] rows: the gate applies after the un-rotation
    @uniform @binding = 5 @default = g_zero gstride : uint                  //! their row stride, 0 = no gate

    [metal_kernel(name="metal_fwht_unsign_msl")]
    def metal_fwht_unsign {
        let lid = gl_LocalInvocationID.x
        let hs = head_size
        let base = gl_WorkGroupID.y * qstride + gl_WorkGroupID.x * hs
        buf[lid] = yo[base + lid]
        fwht_butterfly(lid, hs)
        let o = buf[lid] * (1.0 / sqrt(float(hs))) * signs[lid]
        yo[base + lid] = sq_gated(o, qg, gl_WorkGroupID.y * gstride + gl_WorkGroupID.x * 2u * hs + lid, hs, gstride)
    }
}

// ===== the batched-step kernels (P4) =====
// One eval_batch step = B sessions through one weight pass; per-row kernels reach each row's KV
// slice via row table rt[b] = (k base, v base, cap_rows, cnt) in arena elements.

// Fixed-B batched GEMV twins, X-STAGED: the tg's 4 rows share one X panel staged in tgmem
// (cooperative load) instead of per-lane device reads (lab: B=2 +40-88%, B=4 ~2x; per-lane-
// consecutive chunking measured 30-45% WORSE). Geometry pinned at 128 threads = 4 rows.

[ |> template_struct_instance]
class template MetalGemvB24T {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wqb : array<int8>    // 34B-block W blob (W13: w1), byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" @off = "boff" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" @off = "xoff" x : array<float4>    // [B x n] f32 rows, float4 view (xoff = the 4-row tile at B > 4)
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>     // [B x ddim]
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint       // output rows (W13: the hidden width, which is also the y stride)
    @uniform @binding = 7 @template_gate = "!W13" ys : uint         // y ROW stride (== ddim dense)
    @uniform @binding = 7 @role = "weight" @template_gate = W13 act : uint   //! the W13 stamps' gate activation (`gk_act_mul`: 0 silu, 1 gelu)
    @ssbo @binding = 8 @role = "weight" @off = "w3off" @template_gate = W13 wqb3 : array<int8>
    @ssbo @binding = 9 @role = "weight" @off = "w3off" @template_gate = W13 wsh3 : array<float16>
    @workgroup txp : float4[PANEL]          // [NR streams x 64 float4] X panel chunk
    @template_constant NR : int = 2
    @template_constant PANEL : int = 128    // NR * 64 slots — override together with NR and WIDE
    @template_constant WIDE : bool = false  // B4: two panel slots per thread
    @template_constant W13 : bool = false   // the fused W1|W3 + activation form

    def w4_at(qv : array<int8>; sv : array<float16>; bi, qb : uint) : float4 => (float4(float(qv[qb]),
        float(qv[qb + 1u]), float(qv[qb + 2u]), float(qv[qb + 3u])) * float(sv[bi * 17u]))

    def gemv_core {
        let lid = gl_LocalInvocationID.x
        let row = min(gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID, ddim - 1u)
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let nb = ndim / 32u
        var acc_gate : float[NR]
        var acc_up : float[NR]                // nolint:LINT002 — the W13 arms alone accumulate into it
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
                let w_gate = w4_at(wqb, wsh, bi, qb)
                let kt = uint(i) * 32u + lane
                static_if (W13) {
                    let w_up = w4_at(wqb3, wsh3, bi, qb)
                    for [unroll_full] (b in range(NR)) {
                        let xv = txp[uint(b) * 64u + kt]
                        acc_gate[b] += w_gate.x * xv.x + w_gate.y * xv.y + w_gate.z * xv.z + w_gate.w * xv.w
                        acc_up[b] += w_up.x * xv.x + w_up.y * xv.y + w_up.z * xv.z + w_up.w * xv.w
                    }
                } else {
                    for [unroll_full] (b in range(NR)) {
                        let xv = txp[uint(b) * 64u + kt]
                        acc_gate[b] += w_gate.x * xv.x + w_gate.y * xv.y + w_gate.z * xv.z + w_gate.w * xv.w
                    }
                }
            }
            cb += 64u
        }
        var k = cb + lane
        while (k < nb4) {
            let bi = row * nb + k / 8u
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let w_gate = w4_at(wqb, wsh, bi, qb)
            static_if (W13) {
                let w_up = w4_at(wqb3, wsh3, bi, qb)
                for [unroll_full] (b in range(NR)) {
                    let xv = x[uint(b) * nb4 + k]
                    acc_gate[b] += w_gate.x * xv.x + w_gate.y * xv.y + w_gate.z * xv.z + w_gate.w * xv.w
                    acc_up[b] += w_up.x * xv.x + w_up.y * xv.y + w_up.z * xv.z + w_up.w * xv.w
                }
            } else {
                for [unroll_full] (b in range(NR)) {
                    let xv = x[uint(b) * nb4 + k]
                    acc_gate[b] += w_gate.x * xv.x + w_gate.y * xv.y + w_gate.z * xv.z + w_gate.w * xv.w
                }
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(NR)) {
            let g = simd_sum(acc_gate[b])
            static_if (W13) {
                let u = simd_sum(acc_up[b])
                if (lane == 0u) {
                    y[uint(b) * ddim + row] = gk_act_mul(g, u, act)
                }
            } else {
                if (lane == 0u) {
                    y[uint(b) * ys + row] = g
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_gemv_b2_c", pso = "g_pso_gemv_b2", tgmem = "MetalGemvB2_metal_gemv_msl_tgmem", tg = 128, grid = "d/4", params = "d : int64")]
class MetalGemvB2 : MetalGemvB24T {
    override NR = 2
    override PANEL = 128
    override WIDE = false

    [metal_kernel]
    def metal_gemv {
        gemv_core()
    }
}

[metal_dispatch(name = "enc_gemv_b4_c", pso = "g_pso_gemv_b4", tgmem = "MetalGemvB4_metal_gemv_msl_tgmem", tg = 128, grid = "d/4", params = "d : int64")]
class MetalGemvB4 : MetalGemvB24T {
    override NR = 4
    override PANEL = 256
    override WIDE = true

    [metal_kernel]
    def metal_gemv {
        gemv_core()
    }
}

// Skinny-batch GEMV: streamed form beats x-staged panel kernels ~12% at B <= 4. 64 threads = 2
// simdgroups x 2 row-halves; 16 lanes stream one output row's interleaved 34B q8 blocks, batch
// columns share the dequantized weight stream. The 4-chunk K stripe has no tail guard (requires=).
[metal_dispatch(name = "enc_mv_b2_c", pso = "g_pso_mv_b2", tg = 64, grid = "d/4", params = "d : int64", requires = "ka.ndim % 256")]
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
[metal_dispatch(name = "enc_mv_b4_c", pso = "g_pso_mv_b4", tg = 64, grid = "d/8", params = "d : int64", requires = "ka.ndim % 128")]
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

// MetalGemvB24T's W13 stamps: the B2 form serves B <= 2 rows, the B4 form 3..4
[metal_dispatch(name = "enc_w13sw_b2_c", pso = "g_pso_w13sw_b2", tgmem = "MetalGemvW13SwB2_metal_gemv_w13sw_msl_tgmem", tg = 128, grid = "hid/4", params = "hid : int64")]
class MetalGemvW13SwB2 : MetalGemvB24T {
    override NR = 2
    override PANEL = 128
    override WIDE = false
    override W13 = true

    [metal_kernel]
    def metal_gemv_w13sw {
        gemv_core()
    }
}

[metal_dispatch(name = "enc_w13sw_b4_c", pso = "g_pso_w13sw_b4", tgmem = "MetalGemvW13SwB4_metal_gemv_w13sw_msl_tgmem", tg = 128, grid = "hid/4", params = "hid : int64")]
class MetalGemvW13SwB4 : MetalGemvB24T {
    override NR = 4
    override PANEL = 256
    override WIDE = true
    override W13 = true

    [metal_kernel]
    def metal_gemv_w13sw {
        gemv_core()
    }
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
    def metal_q8_gemm_b {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ka.ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        var mc : simdgroup_float8x8[4]      // this simdgroup's 16x16 quadrant: 2 token x 2 wrow tiles
        var ma, mb : simdgroup_half8x8[2]
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
            tg_store_half4(tx, tX0, b0)
            tg_store_half4(tx, tX0 + 4u, b1)
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

[metal_dispatch(name = "enc_gemm_b_c", pso = "g_pso_gemm_b", tgmem = "MetalQ8GemmB_metal_q8_gemm_b_msl_tgmem", tg = 128, grid = "ntg", params = "ntg : int64", requires = "ka.ndim % 32, ka.kdim % 32")]
class MetalQ8GemmB : MetalQ8GemmBSplitT {
}

[metal_dispatch(name = "enc_gemm_sk_b_c", pso = "g_pso_gemm_b_sk", tgmem = "MetalQ8GemmBSk_metal_q8_gemm_b_msl_tgmem", tg = 128, grid = "d/32, nsk", params = "d : int64, nsk : int64", requires = "d % 32, ka.kdim % 32")]
class MetalQ8GemmBSk : MetalQ8GemmBSplitT {
    override IS_SK = true
}

// the split-K reduce: y = the sum of the ksplit partial planes, one float4 a thread; `total` float4s is both the count and
// the plane stride (the batch GEMM's 32 x d planes, the prefill window's mp x d)
[metal_dispatch(name = "enc_sk_reduce_c", pso = "g_pso_skred", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSkReduce {
    @ssbo @binding = 0 @role = "read" part : array<float4>
    @ssbo @binding = 1 @role = "write" @off = "yoff" @span = "total*16" y : array<float4>
    @uniform @binding = 2 ka : SkReduceArgs

    [metal_kernel(name="metal_sk_reduce_msl")]
    def metal_sk_reduce {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        var acc = part[gid]
        var s = 1u
        while (s < ka.ksplit) {
            acc += part[s * ka.total + gid]
            s++
        }
        y[ka.cols4 == 0u ? gid : (gid / ka.cols4) * ka.ld4 + gid % ka.cols4] = acc
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
        let sp = unsafe(addr(wsh[(IS_SK ? nBase * nkb + gl_WorkGroupID.y * (nkb / ka.ksplit) : nBase * nkb) * 17u]))
        let wp = unsafe(addr(wqb[(IS_SK ? nBase * nkb + gl_WorkGroupID.y * (nkb / ka.ksplit) : nBase * nkb) * 34u + 2u]))
        let ap = unsafe(addr(x[IS_SK ? mBase * ka.kdim + gl_WorkGroupID.y * (nkb / ka.ksplit) * 32u : mBase * ka.kdim]))
        var cp = unsafe(addr(y[IS_SK ? gl_WorkGroupID.y * 32u * ka.ndim + mBase * ka.ndim + nBase : (LDC_ND ? mBase * ka.ndim + nBase : mBase * ka.ys + nBase)]))
        tmm2d_q8u_f32(32u, TILE_N, 4u, twb, sp, wp, ap, ka.kdim, cp, LDC_ND ? ka.ndim : ka.ys,
            IS_SK ? (nkb / ka.ksplit) * 32u : ka.kdim, nkb, gl_LocalInvocationIndex)
    }
}

[metal_dispatch(name = "enc_gemm_b_t_c", pso = "g_pso_gemm_b", tgmem = "MetalQ8GemmBT_metal_q8_gemm_b_t_msl_tgmem", tg = 128, grid = "ntg", params = "ntg : int64", requires = "ka.ndim % 32, ka.kdim % 32")]
class MetalQ8GemmBT : MetalQ8GemmTensorT {
}

[metal_dispatch(name = "enc_gemm_sk_b_t_c", pso = "g_pso_gemm_b_sk", tgmem = "MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_tgmem", tg = 128, grid = "d/32, nsk", params = "d : int64, nsk : int64", requires = "d % 32, ka.kdim % 32")]
class MetalQ8GemmBSkT : MetalQ8GemmTensorT {
    override IS_SK = true
    override LDC_ND = true
}

[metal_dispatch(name = "enc_gemm64_b_t_c", pso = "g_pso_gemm64_b", tgmem = "MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_tgmem", tg = 128, grid = "ntg", params = "ntg : int64", requires = "ka.ndim % 64, ka.kdim % 32")]
class MetalQ8Gemm64BT : MetalQ8GemmTensorT {
    override TILE_N = 64u
    override LDC_ND = true
}

// The shared 32x64 GEMM tile stage: every mul_mm-family kernel (dense, MoE, attention, KV-quant)
// stages A into ta (4 k-tiles x 8 row-tiles of 8x8) and B into tb (4 k-tiles x 4 token-tiles),
// then walks 4 k-steps of 4 A-loads / 2 B-loads / 8 MACs. The method splices flat at emission.
[ |> template_struct_instance]
class template MetalMmTileBase {
    @workgroup ta : TILET[2048]
    @workgroup tb : TILET[1024]

    def acc_quad(aB, bB : uint; var ma : SGT[4]; var mb : SGT[2]; var mc : simdgroup_float8x8[8]) {
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

// The 64-wide twin of MulMm's 32(M) x 64(N) tile: half the threadgroups (d/64) at ~29% fewer
// instructions per element — wins where the grid stays fat (e.g. the vocab classifier). Each
// simdgroup owns a 16x32 quadrant (2 token x 4 wrow tiles).
[metal_dispatch(name = "enc_gemm64_b_c", pso = "g_pso_gemm64_b", tgmem = "metal_q8_gemm64_b_msl_tgmem", tg = 128, grid = "ntg", params = "ntg : int64", requires = "ka.ndim % 64, ka.kdim % 32")]
class MetalQ8Gemm64B : MetalMmTileBase {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    @ssbo @binding = 0 @role = "weight" @off = "boff" wqb : array<int8>    // 34B-block W blob, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "weight" @off = "boff" wsh : array<float16> // the same buffer, half-scale view (block b at 17*b)
    @ssbo @binding = 3 @role = "read" x : array<float4>    // X [M' x n] f32 rows (M' = 32-padded B)
    @ssbo @binding = 4 @role = "write" y : array<float>     // Y [M' x d]
    @uniform @binding = 5 ka : Q8GemmArgs
    // inherited ta = W tiles (k row, wrow col), tb = X tiles (token row, k col)

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
                ta[tW0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, tX0, b0)
            tg_store_half4(tb, tX0 + 4u, b1)
            barrier()
            acc_quad(aQ, bQ, ma, mb, mc)
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
        let kb0 = rrow.x + lay * rrow.z + ka.layr   // lay = the layer's flat KV-row ELEMENT prefix, layr the rings' rows ahead
        let vb0 = rrow.y + lay * rrow.z + ka.layr
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * sq_qhs(ka.qhs, hs)
        let yoff = bb * (nheads * hs) + h * hs
        sq_fill_scores(kc, q, scores, kb0, 0u, cnt, kv_dim, khoff, qoff, hs, scale, softcap)
        let tot = sq_softmax_sink(scores, red, cnt, lid, nt, hass, hass != 0u ? sink[h] : 0.0)
        yo[yoff + lid] = sq_gated(sq_vacc(vc, scores, vb0, 0u, cnt, kv_dim, khoff, lid) / tot, q, qoff + lid, hs, ka.gated)
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
        let kb0b = rrow.x + lay * rrow.z + ka.layr   // layer slab base, BYTES (lay = the layer's flat row-BYTE prefix, layr the rings')
        let vb0b = rrow.y + lay * rrow.z + ka.layr
        let kb0h = kb0b / 2u                      // ... and HALFWORDS for the scale view
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * sq_qhs(ka.qhs, hs)
        let yoff = bb * (nheads * hs) + h * hs
        sq_fill_scores_q(ksh, kqb, q, scores, kb0h, kb0b, 0u, cnt, nblk, khblk, qoff, hs, scale, softcap)
        let tot = sq_softmax_sink(scores, red, cnt, lid, nt, hass, hass != 0u ? sink[h] : 0.0)
        yo[yoff + lid] = sq_gated(sq_vacc_q(vsh, vqb, scores, vb0h, vb0b, 0u, cnt, nblk, khblk, lid) / tot, q, qoff + lid, hs, ka.gated)
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
        let kb0 = rrow.x + lay * rrow.z + ka.layr   // lay = the layer's flat KV-row ELEMENT prefix, layr the rings' rows ahead
        let vb0 = rrow.y + lay * rrow.z + ka.layr
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * sq_qhs(ka.qhs, hs)
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = sqd_window_start(window, cnt)
        if (c1 <= wstart) {
            return
        }
        sq_fill_scores(kc, q, scores, kb0, kv_ring_row(c0, ka.ring), nrows, kv_dim, khoff, qoff, hs, scale, softcap)
        barrier()
        sq_mask_window(scores, c0, nrows, wstart, lid, nt)
        let mm = sq_score_max(scores, red, nrows, lid, nt)
        barrier()                           // red[] is reused for the sum below
        let tot = sq_exp_sum(scores, red, mm, nrows, lid, nt)
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = sq_gated(sq_vacc(vc, scores, vb0, kv_ring_row(c0, ka.ring), nrows, kv_dim, khoff, lid), q, qoff + lid, hs, ka.gated)
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

[metal_dispatch(name = "enc_attnpb16_c", pso = "g_pso_attnpb16", tgmem = "MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64", requires = "ka.ring % 64")]
class MetalSqAttnPartBF16 : MetalSqAttnPartBKvT {
    typedef KT = float16
}

// f32-mirror twin.
[metal_dispatch(name = "enc_attnpb32_c", pso = "g_pso_attnpb32", tgmem = "MetalSqAttnPartBF32_metal_sq_attn_part_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64", requires = "ka.ring % 64")]
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
        let kb0b = rrow.x + lay * rrow.z + ka.layr   // lay = the layer's flat row-BYTE prefix, layr the rings'
        let vb0b = rrow.y + lay * rrow.z + ka.layr
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u)
        let qoff = bb * qstride + h * sq_qhs(ka.qhs, hs)
        let c1 = min(c0 + 64u, cnt)
        let nrows = c1 - c0
        // sliding window (gemma2): rows < wstart are outside the span. A chunk fully below the
        // window exits whole-tg with its partial UNWRITTEN — the comb twin skips those chunks.
        let wstart = sqd_window_start(window, cnt)
        if (c1 <= wstart) {
            return
        }
        sq_fill_scores_q(ksh, kqb, q, scores, kb0h, kb0b, kv_ring_row(c0, ka.ring), nrows, nblk, khblk, qoff, hs, scale, softcap)
        barrier()
        sq_mask_window(scores, c0, nrows, wstart, lid, nt)
        let mm = sq_score_max(scores, red, nrows, lid, nt)
        barrier()                           // red[] is reused for the sum below
        let tot = sq_exp_sum(scores, red, mm, nrows, lid, nt)
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = sq_gated(sq_vacc_q(vsh, vqb, scores, vb0h, vb0b, kv_ring_row(c0, ka.ring), nrows, nblk, khblk, lid), q, qoff + lid, hs, ka.gated)
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// q8_0 instance.
[metal_dispatch(name = "enc_attnpb_q8_c", pso = "g_pso_attnpb_q8", tgmem = "MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64", requires = "ka.ring % 64")]
class MetalSqAttnPartBQ8 : MetalSqAttnPartBQuantT {
    typedef QB = int8
}

// tq4 instance — nibble reads.
[metal_dispatch(name = "enc_attnpb_tq4_c", pso = "g_pso_attnpb_tq4", tgmem = "MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl_tgmem", tg = "head_size", grid = "wgs", params = "wgs : int64, head_size : int64", requires = "ka.ring % 64")]
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
    total : uint    //!< float4s a partial plane
    ksplit : uint
    cols4 : uint    //!< float4s a partial plane's row where the output rows are wider (a dev-W column tile); 0: contiguous
    ld4 : uint      //!< the output's row stride in float4s under `cols4`
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

// The q8_0 mirror counts its K/V work in 32-element blocks (`nblk`), not in rope pairs.
struct RopeStoreQ8Args {
    qd : uint
    head_size : uint
    npairs_q : uint
    nblk : uint
    neox : uint
    has_bias : uint
    rot : uint          // the rotated prefix of a head (head_size or 0 = the whole head; the rope table row is rot/2 wide)
    qhs : uint          //! a q head's stride in `qr`: head_size (0 reads as it), or 2 head_size on a gated model
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
    rot : uint
    qhs : uint
    qrow : uint         //! a q row's stride in `qr` (0: `qstride`)
    layr : uint     //! the rings' rows ahead of the layer in a stream's slice (elements, or bytes under a block codec): its base is `lay * cap + layr`
    ring : uint     //! a sliding layer's ring rows: a key's row is its `kv_ring_row` slot; 0 = a row a position
}

// The tq4 mirror is head-cooperative (the FWHT spans a whole head), so it counts heads.
struct RopeStoreTq4Args {
    qd : uint
    head_size : uint
    nheads : uint
    kv_mul : uint
    neox : uint
    has_bias : uint
    rot : uint
    qhs : uint
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
    rot : uint
    qhs : uint
    qrow : uint
    layr : uint     //! the rings' rows ahead of the layer in a stream's slice (elements, or bytes under a block codec): its base is `lay * cap + layr`
    ring : uint     //! a sliding layer's ring rows: a key's row is its `kv_ring_row` slot; 0 = a row a position
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
    split : uint
    row : uint   //! the first row of the [npos x ...] planes the row-wise stages (L2, prep, gate) read and write
    sslice : uint   //! the layer's state plane, floats past a group's region base
    cslice : uint   //! the layer's history plane, floats past a group's region base
    bstr : uint     //! the scan's beta and alpha row stride: nvh, or 2 nvh where one GEMV wrote [beta; alpha] a row
    goff : uint     //! the alpha rows' first float in their plane: 0, or nvh past beta in the [beta; alpha] rows
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
        qscale = 1.0 / sqrt(float(c.ssm_d_state)), mid = mid ? 1u : 0u, split = 0u, row = 0u, sslice = 0u, cslice = 0u,
        bstr = uint(c.ssm_dt_rank), goff = 0u)
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

def private sqd_window_start(window, cnt : uint) : uint => window > 0u && cnt > window ? cnt - window : 0u

def private sqd_wide_sum(var wsc : float[128]; hp, sl : uint; share : float; four : bool) : float {
    wsc[hp * 32u + sl] = share
    barrier()
    var keep = wsc[sl] + wsc[32u + sl]
    if (four) {
        keep += wsc[64u + sl] + wsc[96u + sl]
    }
    barrier()
    return keep
}

def private sqd_rescale(var wacc : float[512]; wo : uint; bacc : float4; r1, r2 : float) {
    wacc[wo] = wacc[wo] * r1 + bacc.x * r2
    wacc[wo + 1u] = wacc[wo + 1u] * r1 + bacc.y * r2
    wacc[wo + 2u] = wacc[wo + 2u] * r1 + bacc.z * r2
    wacc[wo + 3u] = wacc[wo + 3u] * r1 + bacc.w * r2
}

// online merge into the running slice-group state (first pass: r1 = exp(-inf) = 0)
def private sqd_merge(var wacc : float[512]; wo : uint; bacc : float4; bm, btot : float;
                      var mm : float&; var tot : float&) {
    let nm = max(mm, bm)
    let r1 = exp(mm - nm)
    let r2 = exp(bm - nm)
    sqd_rescale(wacc, wo, bacc, r1, r2)
    tot = tot * r1 + btot * r2
    mm = nm
}

//! a lane's four accumulators under the sigmoid of their gate quad `g`
def private sqd_gate(var wacc : float[512]; g : float4; wo : uint) {
    wacc[wo] = sig_gate(wacc[wo], g.x)
    wacc[wo + 1u] = sig_gate(wacc[wo + 1u], g.y)
    wacc[wo + 2u] = sig_gate(wacc[wo + 2u], g.z)
    wacc[wo + 3u] = sig_gate(wacc[wo + 3u], g.w)
}

def private sqd_store(var part : array<float>; wacc : float[512]; pbase, wo, sl, hs : uint;
                      mm, tot : float) {
    let po = pbase + sl * 4u
    if (sl * 4u < hs) {   // a head narrower than the 32 lanes' quads: the lanes past it own no dims
        part[po] = wacc[wo]
        part[po + 1u] = wacc[wo + 1u]
        part[po + 2u] = wacc[wo + 2u]
        part[po + 3u] = wacc[wo + 3u]
    }
    if (sl == 0u) {
        part[pbase + hs] = mm
        part[pbase + hs + 1u] = tot
    }
}

//! the first 32-key slice this slice group scores: the group's own slice at or past the window's start
def private sqd_first_slice(gsg, nsgs, wstart : uint) : uint => gsg + ((wstart / 32u + nsgs - 1u - gsg) / nsgs) * nsgs

//! a slice's softmax weights for this lane's key from its raw score: the entry under the scale and cap for a key inside the
//! slice and the window (-inf elsewhere), the slice max in `bm`, the weights' sum in `btot`
def private sqd_block_weights(raw : float; sl, nrows, c0, wstart : uint; scale, softcap : float; var bm : float&; var btot : float&) : float {
    let keep = sl < nrows && c0 + sl >= wstart ? sq_score_entry(raw, scale, softcap) : -3.0e38
    bm = simd_max(keep)
    let w = exp(keep - bm)
    btot = simd_sum(w)
    return w
}

//! the partial record's base for this simdgroup's slice group: a wide head's part records are 132 floats (128 dims, m,
//! tot and a pad), a narrow head's hs + 4
def private sqd_part_base(bb, nheads, h, hp, gsg, nsgs, hs : uint; wide : bool; parts : uint) : uint
    => wide ? (((bb * nheads + h) * parts + hp) * nsgs + gsg) * 132u : ((bb * nheads + h) * nsgs + gsg) * (hs + 4u)

// Fused single-pass batched attention: beats the upstream vector form at every depth 512-16K. One
// simdgroup per (row, head, 32-slice-group), scores in-register, V lane-private; grid is HEAD-FASTEST.
[ |> template_struct_instance]
class template MetalSqAttnDKvT {
    @ssbo @binding = 0 @role = "read" kc : array<KT>
    @ssbo @binding = 1 @role = "read" vc : array<KT>
    @ssbo @binding = 2 @role = "read" q : array<float4>
    @ssbo @binding = 3 @role = "write" part : array<float>
    @ssbo @binding = 4 @role = "read" @off = "rtoff" rt : array<uint4>
    @uniform @binding = 5 ka : SqAttnDArgs
    @workgroup wacc : float[512]
    @workgroup @template_gate = WIDE wsc : float[128]   // a slice's scores, a simdgroup's share of each key
    @template_constant WIDE : bool = false
    @template_constant PARTS : uint = 1u     // the 128-dim parts of a wide head

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
        let hp = WIDE ? gl_SubgroupID : 0u   // the part of a wide head this simdgroup owns
        let gsg = WIDE ? gl_WorkGroupID.y : gl_WorkGroupID.y * 4u + gl_SubgroupID
        let rrow = rt[bb]
        let cnt = rrow.w
        if (gsg >= nsgs || gsg * 32u >= cnt) {
            return
        }
        let sl = gl_SubgroupInvocationID
        let kb0 = rrow.x + lay * rrow.z + ka.layr   // lay = the layer's flat KV-row ELEMENT prefix, layr the rings' rows ahead
        let vb0 = rrow.y + lay * rrow.z + ka.layr
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * sq_qhs(ka.qhs, hs)
        let kv4 = kv_dim / 4u
        // a lane owns one quad of the head; a head narrower than 32 quads leaves the lanes past it
        // re-reading a valid quad with a zero q, so they score nothing and their sums are never stored
        let ld = hs / 4u
        let slc = sl % ld
        let q4 = sl < ld ? float4(q[qoff / 4u + hp * 32u + slc]) : float4(0.0)
        let wo = (gl_SubgroupID * 32u + sl) * 4u
        sqd_zero(wacc, wo)
        var mm = -3.0e38
        var tot = 0.0
        let wstart = sqd_window_start(ka.window, cnt)
        var ss = sqd_first_slice(gsg, nsgs, wstart)
        while (ss * 32u < cnt) {
            let c0 = ss * 32u
            let nrows = min(c0 + 32u, cnt) - c0
            let cr = kv_ring_row(c0, ka.ring)   //! a 32-key slice sits whole in a ring: the slice and the ring are multiples of 32
            var kp = unsafe(addr(kc[(kb0 + cr * kv_dim + khoff) / 4u + hp * 32u + slc]))
            var vp = unsafe(addr(vc[(vb0 + cr * kv_dim + khoff) / 4u + hp * 32u + slc]))
            // invalid tail lanes keep the -inf seed -> weight 0
            var keep = sqd_score_blk(kp, q4, WIDE ? 0.0 : -3.0e38, nrows, kv4, sl)
            static_if (WIDE) {
                keep = sqd_wide_sum(wsc, hp, sl, keep, PARTS == 4u)
            }
            var bm = 0.0
            var btot = 0.0
            let w = sqd_block_weights(keep, sl, nrows, c0, wstart, scale, ka.softcap, bm, btot)
            let bacc = sqd_vacc_blk(vp, w, nrows, kv4)
            sqd_merge(wacc, wo, bacc, bm, btot, mm, tot)
            ss += nsgs
        }
        if (ka.gated != 0u && sl < ld) {   //! the gate is linear, so the slice group's partial takes it
            sqd_gate(wacc, float4(q[(qoff + hs) / 4u + hp * 32u + slc]), wo)
        }
        sqd_store(part, wacc, sqd_part_base(bb, nheads, h, hp, gsg, nsgs, hs, WIDE, PARTS), wo, sl, WIDE ? 128u : hs, mm, tot)
    }
}

[metal_dispatch(name = "enc_attnd16_c", pso = "g_pso_attnd16", tgmem = "MetalSqAttnDF16_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs/4, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDF16 : MetalSqAttnDKvT {
    typedef KT = half4
}

// f32-mirror twin — float4 view, no conversion.
[metal_dispatch(name = "enc_attnd32_c", pso = "g_pso_attnd32", tgmem = "MetalSqAttnDF32_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs/4, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDF32 : MetalSqAttnDKvT {
    typedef KT = float4
}

// the head-of-256 pair: a threadgroup a slice group, two simdgroups each half the head
[metal_dispatch(name = "enc_attndw16_c", pso = "g_pso_attndw16", tgmem = "MetalSqAttnDWF16_metal_sq_attn_d_msl_tgmem", tg = 64, grid = "nheads, nsgs, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDWF16 : MetalSqAttnDKvT {
    typedef KT = half4
    override WIDE = true
    override PARTS = 2u
}

[metal_dispatch(name = "enc_attndw32_c", pso = "g_pso_attndw32", tgmem = "MetalSqAttnDWF32_metal_sq_attn_d_msl_tgmem", tg = 64, grid = "nheads, nsgs, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDWF32 : MetalSqAttnDKvT {
    typedef KT = float4
    override WIDE = true
    override PARTS = 2u
}

// the head-of-512 pair: four simdgroups each a quarter of the head
[metal_dispatch(name = "enc_attndx16_c", pso = "g_pso_attndx16", tgmem = "MetalSqAttnDXF16_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDXF16 : MetalSqAttnDKvT {
    typedef KT = half4
    override WIDE = true
    override PARTS = 4u
}

[metal_dispatch(name = "enc_attndx32_c", pso = "g_pso_attndx32", tgmem = "MetalSqAttnDXF32_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDXF32 : MetalSqAttnDKvT {
    typedef KT = float4
    override WIDE = true
    override PARTS = 4u
}

// Cross-slice combine for the D entries: PARALLEL over entries (4 sgs online-merge their
// subsets with float4 loads — m and l arrive in ONE float4), a single tgmem cross-sg merge.
[metal_dispatch(name = "enc_attncombd_c", pso = "g_pso_attncombd", tgmem = "metal_sq_attn_comb_d_msl_tgmem", tg = 128, grid = "nheads, nrows", params = "nheads : int64, nrows : int64")]
class MetalSqAttnCombD {
    @ssbo @binding = 0 @role = "read" part : array<float4>
    @ssbo @binding = 1 @role = "write" yo : array<float>
    @ssbo @binding = 2 @role = "read" @off = "rtoff" rt : array<uint4>
    @uniform @binding = 3 ka : SqAttnCombDArgs
    @ssbo @binding = 20 @role = "weight" @default = g_one sink : array<float> //! [model heads] this layer's attention sinks (hass models)
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
        let ld = hs / 4u   // the lanes that own a quad of this head; the 128-thread group merges four subgroups whatever the head
        var mm = -3.0e38
        var tot = 0.0
        var acc4 = float4(0.0)
        var e = sg
        while (e < nch) {
            let eb = base4 + e * s4
            let ml = float4(part[eb + hs / 4u])
            let av = sl < ld ? float4(part[eb + sl]) : float4(0.0)
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
        let snk = ka.hass != 0u ? sink[h / ka.hdiv] : -3.0e38
        let gm = max(max(max(m0, m1), max(m2, m3)), snk)
        let r0 = exp(m0 - gm)
        let r1 = exp(m1 - gm)
        let r2 = exp(m2 - gm)
        let r3 = exp(m3 - gm)
        let l = wml[1] * r0 + wml[3] * r1 + wml[5] * r2 + wml[7] * r3 + exp(max(snk - gm, -80.0)) * float(ka.hass)
        let acc = wacc[lid] * r0 + wacc[128u + lid] * r1 + wacc[256u + lid] * r2 + wacc[384u + lid] * r3
        if (lid < hs) {
            yo[bb * (nheads * hs) + h * hs + lid] = acc / l
        }
    }
}

//! The window's fused rope and K/V store, the f16-mirror stamp of the template both homes stamp (`GkRopeKvStore`), the stores
//! clamped: the grid (pairs x rows), row b's q pairs roped in place in the [q | k | v] rows, its k pairs roped into the mirror row
//! its row-table entry names at depth cnt - 1 and its v pairs copied beside (`rope_kv_tabled`; `pa.mirbase` the layer index)
[metal_dispatch(name = "enc_rpstb16_c", pso = "g_pso_rpstb16", tg = 64, grid = "npairs/64, nrows", params = "npairs : int64, nrows : int64")]
class MetalRopeStoreBF16 : GkRopeKvStore {
    typedef KT = float16
    override SPLIT_TABLES = true
    @ssbo @binding = 8 @role = "read" @off = "rtoff" rt : array<uint4>   //! a row's [k base, v base, per-layer stride, depth]

    [metal_kernel]
    def metal_rope_store_b {
        rope_kv_tabled(gl_GlobalInvocationID.x, gl_WorkGroupID.y, rt[gl_WorkGroupID.y])
    }
}

//! the f32-mirror stamp of the same form on `GkRopeKvStore`: straight stores, no clamp
[metal_dispatch(name = "enc_rpstb32_c", pso = "g_pso_rpstb32", tg = 64, grid = "npairs/64, nrows", params = "npairs : int64, nrows : int64")]
class MetalRopeStoreBF32 : GkRopeKvStore {
    typedef KT = float
    override CLAMP16 = false
    override SPLIT_TABLES = true
    @ssbo @binding = 8 @role = "read" @off = "rtoff" rt : array<uint4>   //! a row's [k base, v base, per-layer stride, depth]

    [metal_kernel]
    def metal_rope_store_b {
        rope_kv_tabled(gl_GlobalInvocationID.x, gl_WorkGroupID.y, rt[gl_WorkGroupID.y])
    }
}
// Quant partD twin pair (the DF16 shape over block codecs): one sg per (row, head, 32-position
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
    @ssbo @binding = 6 @role = "read" @off = "rtoff" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @uniform @binding = 7 ka : SqAttnDArgs
    @workgroup wacc : float[512]
    @workgroup @template_gate = WIDE wsc : float[128]   // a slice's scores, a simdgroup's share of each key
    @template_constant LANE_BYTES : uint = 4u
    @template_constant WIDE : bool = false
    @template_constant PARTS : uint = 1u     // the 128-dim parts of a wide head

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
        let hp = WIDE ? gl_SubgroupID : 0u   // the part of a wide head this simdgroup owns
        let gsg = WIDE ? gl_WorkGroupID.y : gl_WorkGroupID.y * 4u + gl_SubgroupID
        let rrow = rt[bb]
        let cnt = rrow.w
        if (gsg >= nsgs || gsg * 32u >= cnt) {
            return
        }
        let sl = gl_SubgroupInvocationID
        let kb0b = rrow.x + lay * rrow.z + ka.layr   // lay = the layer's flat row-BYTE prefix, layr the rings'
        let vb0b = rrow.y + lay * rrow.z + ka.layr
        let kb0h = kb0b / 2u
        let vb0h = vb0b / 2u
        let khblk = (h / kv_mul) * (hs / 32u) + hp * 4u
        let qoff = bb * qstride + h * sq_qhs(ka.qhs, hs)
        let ld = hs / 4u
        let slc = sl % ld
        let q4 = sl < ld ? float4(q[qoff / 4u + hp * 32u + slc]) : float4(0.0)
        let blkoff = khblk + slc / 8u           // this lane's block within the row
        let inb = (slc % 8u) * LANE_BYTES       // lane's byte offset inside its block (4 quant bytes / 2 nibble bytes)
        let wo = gl_SubgroupID * 128u + sl * 4u
        sqd_zero(wacc, wo)
        var mm = -3.0e38
        var tot = 0.0
        let wstart = sqd_window_start(ka.window, cnt)
        var ss = sqd_first_slice(gsg, nsgs, wstart)
        while (ss * 32u < cnt) {
            let c0 = ss * 32u
            let nrows = min(c0 + 32u, cnt) - c0
            var keep = sqd_score_blk_q(kqb, ksh, q4, WIDE ? 0.0 : -3.0e38, kv_ring_row(c0, ka.ring), nrows, nblk, blkoff, kb0b, kb0h, inb, sl)
            static_if (WIDE) {
                keep = sqd_wide_sum(wsc, hp, sl, keep, PARTS == 4u)
            }
            var bm = 0.0
            var btot = 0.0
            let w = sqd_block_weights(keep, sl, nrows, c0, wstart, scale, ka.softcap, bm, btot)
            let bacc = sqd_vacc_blk_q(vqb, vsh, w, kv_ring_row(c0, ka.ring), nrows, nblk, blkoff, vb0b, vb0h, inb)
            sqd_merge(wacc, wo, bacc, bm, btot, mm, tot)
            ss += nsgs
        }
        if (ka.gated != 0u && sl < ld) {   //! the gate is linear, so the slice group's partial takes it
            sqd_gate(wacc, float4(q[(qoff + hs) / 4u + hp * 32u + slc]), wo)
        }
        sqd_store(part, wacc, sqd_part_base(bb, nheads, h, hp, gsg, nsgs, hs, WIDE, PARTS), wo, sl, WIDE ? 128u : hs, mm, tot)
    }
}

// q8_0 instance — K/V loads are 4 scalar int8s + block scale per position (34B blocks break
// half4 alignment); scale folds in before the simd_sum.
[metal_dispatch(name = "enc_attnd_q8_c", pso = "g_pso_attnd_q8", tgmem = "MetalSqAttnDQ8_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs/4, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDQ8 : MetalSqAttnDQuantT {
    typedef QB = int8
}

// tq4 instance — nibble reads: lane's 4 elements = 2 nibble bytes of block sl/8.
[metal_dispatch(name = "enc_attnd_tq4_c", pso = "g_pso_attnd_tq4", tgmem = "MetalSqAttnDTq4_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs/4, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDTq4 : MetalSqAttnDQuantT {
    typedef QB = uint8
    override LANE_BYTES = 2u
}

// the head-of-256 quant pair: a threadgroup a slice group, two simdgroups each half the head
[metal_dispatch(name = "enc_attndw_q8_c", pso = "g_pso_attndw_q8", tgmem = "MetalSqAttnDWQ8_metal_sq_attn_d_msl_tgmem", tg = 64, grid = "nheads, nsgs, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDWQ8 : MetalSqAttnDQuantT {
    typedef QB = int8
    override WIDE = true
    override PARTS = 2u
}

[metal_dispatch(name = "enc_attndw_tq4_c", pso = "g_pso_attndw_tq4", tgmem = "MetalSqAttnDWTq4_metal_sq_attn_d_msl_tgmem", tg = 64, grid = "nheads, nsgs, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDWTq4 : MetalSqAttnDQuantT {
    typedef QB = uint8
    override LANE_BYTES = 2u
    override WIDE = true
    override PARTS = 2u
}

// the head-of-512 quant pair: four simdgroups each a quarter of the head
[metal_dispatch(name = "enc_attndx_q8_c", pso = "g_pso_attndx_q8", tgmem = "MetalSqAttnDXQ8_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDXQ8 : MetalSqAttnDQuantT {
    typedef QB = int8
    override WIDE = true
    override PARTS = 4u
}

[metal_dispatch(name = "enc_attndx_tq4_c", pso = "g_pso_attndx_tq4", tgmem = "MetalSqAttnDXTq4_metal_sq_attn_d_msl_tgmem", tg = 128, grid = "nheads, nsgs, nrows", params = "nheads : int64, nsgs : int64, nrows : int64", requires = "ka.ring % 32")]
class MetalSqAttnDXTq4 : MetalSqAttnDQuantT {
    typedef QB = uint8
    override LANE_BYTES = 2u
    override WIDE = true
    override PARTS = 4u
}

// q8_0-mirror batched rope + KV-row store: the RopeStoreQ8 shape (a simdgroup a block) with the byte-offset row table
// (grid y = batch row; the new KV row lands at cnt-1). Same quantize_q8kv_row semantics.
[metal_dispatch(name = "enc_rpstb_q8_c", pso = "g_pso_rpstb_q8", tg = 64, grid = "nthreads/64, nrows", params = "nthreads : int64, nrows : int64")]
class MetalRopeStoreBQ8 : MetalRopeStoreQ8Base {
    @ssbo @binding = 0 @role = "read" qk : array<float>    //! [B x qstride] fused QKV rows (q ropes in `qr`)
    @ssbo @binding = 1 @role = "write" ksh : array<float16> // the whole mirror arena, half-scale view (k dest)
    @ssbo @binding = 2 @role = "write" kqb : array<int8>    // the SAME arena, byte view
    @ssbo @binding = 3 @role = "write" vsh : array<float16>
    @ssbo @binding = 4 @role = "write" vqb : array<int8>
    @ssbo @binding = 5 @role = "weight" tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 6 @role = "weight" tsin : array<float>
    @ssbo @binding = 7 @role = "read" @off = "rtoff" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @ssbo @binding = 8 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 9 ka : RopeStoreBQ8Args
    @ssbo @binding = 10 @role = "write" qr : array<float>   //! the q rows roped in place, `qrow` apart (0: `qk`'s `qstride`)

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
        let rot = ka.rot
        let gid = gl_GlobalInvocationID.x
        let bb = gl_WorkGroupID.y
        let rrow = rt[bb]
        let qbase = bb * qstride
        let half = head_size / 2u
        let tcb = bb * ((rot != 0u && rot != head_size) ? rot / 2u : half)
        if (gid < npairs_q) {
            q8_rope_q(qr, tcos, tsin, bias, gid, half, head_size, sq_qhs(ka.qhs, head_size), neox, has_bias, rot, bb * (ka.qrow == 0u ? qstride : ka.qrow), tcb)
            return
        }
        let rowb = nblk * 34u
        let kvd = nblk * 32u
        var inside = false
        var isV = false
        var ge = 0u
        q8_kv_lane(gid, npairs_q, kvd, inside, isV, ge)
        if (!inside) {   // whole simdgroups: a block's 32 lanes return together
            return
        }
        let kqoff = qbase + qd              // the fused row's k region (v = its third region)
        let vqoff = qbase + qd + kvd
        var v = 0.0
        q8_kv_elem(qk, qk, tcos, tsin, bias, kqoff, vqoff, tcb, ge, head_size, half, qd, kvd, neox, has_bias, rot, isV, v)
        if (isV) {
            let vb0b = rrow.y + lay * rrow.z + ka.layr + kv_ring_row(rrow.w - 1u, ka.ring) * rowb   // lay = the flat row-byte prefix, layr the rings'
            q8_store_lane(vsh, vqb, v, vb0b / 2u, vb0b, ge / 32u, ge % 32u)
        } else {
            let kb0b = rrow.x + lay * rrow.z + ka.layr + kv_ring_row(rrow.w - 1u, ka.ring) * rowb
            q8_store_lane(ksh, kqb, v, kb0b / 2u, kb0b, ge / 32u, ge % 32u)
        }
    }
}

// tq4-mirror batched rope + KV-row store: the RopeStoreTq4 shape (threadgroup per head) with the
// byte-offset row table — grid (heads, batch rows); the new KV row lands at cnt-1.
[metal_dispatch(name = "enc_rpstb_tq4_c", pso = "g_pso_rpstb_tq4", tgmem = "metal_rope_store_b_tq4_msl_tgmem", tg = "head_size", grid = "ntg, nrows", params = "ntg : int64, nrows : int64, head_size : int64")]
class MetalRopeStoreBTq4 : MetalRopeStoreTq4Base {
    @ssbo @binding = 0 @role = "read" qk : array<float>    //! [B x qstride] fused QKV rows (q ropes + rotates in `qr`)
    @ssbo @binding = 1 @role = "write" ksh : array<float16> // the whole mirror arena, half-scale view (k dest)
    @ssbo @binding = 2 @role = "write" kqb : array<uint8>   // the SAME arena, byte view
    @ssbo @binding = 3 @role = "write" vsh : array<float16>
    @ssbo @binding = 4 @role = "write" vqb : array<uint8>
    @ssbo @binding = 5 @role = "weight" tcos : array<float>  // [B x head_size/2] per-row rope rows
    @ssbo @binding = 6 @role = "weight" tsin : array<float>
    @ssbo @binding = 7 @role = "weight" signs : array<float>
    @ssbo @binding = 8 @role = "read" @off = "rtoff" rt : array<uint4>    // per-row (k base BYTES, v base BYTES, cap_rows, cnt)
    @ssbo @binding = 9 @role = "weight" bias : array<float> // [qd | kvd | kvd] QKV bias (dummy-bound when has_bias == 0)
    @uniform @binding = 10 ka : RopeStoreBTq4Args
    @ssbo @binding = 11 @role = "readwrite" qr : array<float>   //! the q rows roped and rotated in place, `qrow` apart (0: `qk`'s `qstride`)

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
        let rot = ka.rot
        let tgid = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let hs = head_size
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
        let tcb = bb * ((rot != 0u && rot != hs) ? rot / 2u : hs / 2u)
        let qb = bb * (ka.qrow == 0u ? qstride : ka.qrow) + tgid * sq_qhs(ka.qhs, hs)
        var v = 0.0
        if (isQ) {
            tq4_rope_elem(qr, tcos, tsin, bias, qb, bbase, tcb, lid, hs, neox, has_bias, rot, v)
        } elif (isK) {
            tq4_rope_elem(qk, tcos, tsin, bias, base, bbase, tcb, lid, hs, neox, has_bias, rot, v)
        } else {
            v = qk[base + lid] + (has_bias != 0u ? bias[bbase + lid] : 0.0)
        }
        tq4_fwht_signs_row(v, signs, lid, hs)
        if (isQ) {
            qr[qb + lid] = v
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
            let kb0b = rrow.x + lay * rrow.z + ka.layr + kv_ring_row(rrow.w - 1u, ka.ring) * rowb   // lay = the flat row-byte prefix, layr the rings'
            tq4_store(ksh, kqb, d, qv, qother, kb0b, blk, lid, sl)
        } else {
            let vb0b = rrow.y + lay * rrow.z + ka.layr + kv_ring_row(rrow.w - 1u, ka.ring) * rowb
            tq4_store(vsh, vqb, d, qv, qother, vb0b, blk, lid, sl)
        }
    }
}

struct KvRowsArgs {
    rows : uint    //!< the window rows a dispatch stores
    nblk : uint    //!< a K or V row's 32-blocks
    hs : uint      //!< a head's elements (the tq4 rotation's span)
}

//! A prefill window's roped K and V panel rows into a q8_0 mirror slab, a simdgroup a 32-block and a lane an element:
//! `quantize_q8kv_row`'s scale and rounding through the store shell the rope-store stamps share, the rope already done
[metal_dispatch(name = "enc_kv_rows_q8", pso = "g_pso_kv_rows_q8", tg = 64, grid = "nthreads/64", params = "nthreads : int64")]
class MetalKvRowsQ8 : MetalRopeStoreQ8Base {
    @ssbo @binding = 0 @role = "read" @off = "koff" ks : array<float>
    @ssbo @binding = 1 @role = "read" @off = "voff" vs : array<float>
    @ssbo @binding = 2 @role = "write" @off = "mkoff" ksh : array<float16>
    @ssbo @binding = 3 @role = "alias" @off = "mkoff" kqb : array<int8>
    @ssbo @binding = 4 @role = "write" @off = "mvoff" vsh : array<float16>
    @ssbo @binding = 5 @role = "alias" @off = "mvoff" vqb : array<int8>
    @uniform @binding = 6 ka : KvRowsArgs

    [metal_kernel(name = "metal_kv_rows_q8_msl"), arch(at = "../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-kv-mirror")]
    def metal_kv_rows_q8 {
        let nblk = ka.nblk
        let total = ka.rows * nblk * 32u   // the K elements; the V elements follow
        let gid = gl_GlobalInvocationID.x
        if (gid >= 2u * total) {   // whole simdgroups: a block's 32 lanes return together
            return
        }
        let isV = gid >= total
        let e = isV ? gid - total : gid
        let bi = e / 32u
        let row = bi / nblk
        let blk = bi % nblk
        let v = isV ? vs[e] : ks[e]
        if (isV) {
            q8_store_lane(vsh, vqb, v, row * nblk * 17u, row * nblk * 34u, blk, e % 32u)
        } else {
            q8_store_lane(ksh, kqb, v, row * nblk * 17u, row * nblk * 34u, blk, e % 32u)
        }
    }
}

//! A prefill window's roped K and V panel rows into a tq4 mirror slab, a threadgroup a (head, row): the head rotated by the
//! signed FWHT, then quantized - `fwht_signs_row` and `quantize_tq4kv_row` through the store shell the rope-store stamps share
[metal_dispatch(name = "enc_kv_rows_tq4", pso = "g_pso_kv_rows_tq4", tgmem = "metal_kv_rows_tq4_msl_tgmem", tg = "head_size", grid = "ntg, rows", params = "ntg : int64, rows : int64, head_size : int64")]
class MetalKvRowsTq4 : MetalRopeStoreTq4Base {
    @ssbo @binding = 0 @role = "read" @off = "koff" ks : array<float>
    @ssbo @binding = 1 @role = "read" @off = "voff" vs : array<float>
    @ssbo @binding = 2 @role = "write" @off = "mkoff" ksh : array<float16>
    @ssbo @binding = 3 @role = "alias" @off = "mkoff" kqb : array<uint8>
    @ssbo @binding = 4 @role = "write" @off = "mvoff" vsh : array<float16>
    @ssbo @binding = 5 @role = "alias" @off = "mvoff" vqb : array<uint8>
    @ssbo @binding = 6 @role = "weight" signs : array<float>
    @uniform @binding = 7 ka : KvRowsArgs

    [metal_kernel(name = "metal_kv_rows_tq4_msl"), arch(at = "../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-kv-mirror")]
    def metal_kv_rows_tq4 {
        let hs = ka.hs
        let kvd = ka.nblk * 32u
        let nkvh = kvd / hs
        let tgid = gl_WorkGroupID.x
        let row = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let isV = tgid >= nkvh
        let hh = isV ? tgid - nkvh : tgid
        let at = row * kvd + hh * hs + lid
        var v = isV ? vs[at] : ks[at]
        tq4_fwht_signs_row(v, signs, lid, hs)
        let sl = gl_SubgroupInvocationID
        var d = 0.0
        var qv = 0
        var qother = 0
        tq4_quant(v, d, qv, qother, sl)
        let blk = hh * (hs / 32u) + lid / 32u
        let b0b = row * ka.nblk * 18u
        if (isV) {
            tq4_store(vsh, vqb, d, qv, qother, b0b, blk, lid, sl)
        } else {
            tq4_store(ksh, kqb, d, qv, qother, b0b, blk, lid, sl)
        }
    }
}

// The decode PSO + resource globals moved to dasllama_metal_common (declared there so
// [metal_dispatch]-generated builders can see the pipeline they dispatch); the compile_pso()
// assignments below and the shutdown releases stay here.

def compile_pso(source, entry : string; fastmath : bool; var ok : bool&) : MetalComputePipeline? {
    var err : string
    var pso = pso_from_source(g_dev, source, entry, fastmath, err)
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
    if (!box_profile_runtime_applied() && !empty(box_profile_runtime_declined()) && empty(get_metal_tensor_crowns())) {
        to_log(LOG_WARNING, "dasLLAMA metal: no box profile applied ({box_profile_runtime_declined()}) - no metal_tensor crowns, every tensor mul_mm twin runs its base form; on a tensor-lane GPU prefill measures well under half its board cell. Mint: DAS_TUNE_MODE=tune DAS_TUNE_MANIFEST=<repo>/modules/dasLLAMA/performance/<box>.tune.json bin/daslang -jit modules/dasLLAMA/harness/dasllama_tuner.das -dasroot <repo>\n")
    }
    var ok = true
    // the GEMV / rms / add / swiglu MSL sources are the prefill module's, verbatim (own PSOs)
    g_pso_gemv = compile_stamp(MetalQ8Gemv_metal_q8_gemv_msl, ok)
    g_pso_gemv_sg = compile_stamp(MetalQ8GemvSg_metal_q8_gemv_msl, ok)
    g_gemv_rows = clamp(int64(g_env_metal.gemv_tg), 1l, 32l)
    g_pso_rms = compile_stamp(MetalRmsNorm_metal_rmsnorm_msl, ok)
    g_pso_rms_hx = compile_stamp(MetalRmsNormHX_metal_rmsnorm_msl, ok)
    g_pso_add = compile_stamp(MetalAdd_metal_ew2_msl, ok)
    g_pso_addrms = compile_stamp(metal_add_rms_msl, ok)
    g_pso_addrms_hx = compile_stamp(metal_add_rms_hx_msl, ok)
    g_pso_preaddrms = compile_stamp(metal_pre_add_rms_msl, ok)
    g_pso_preaddrms_hx = compile_stamp(metal_pre_add_rms_hx_msl, ok)
    g_pso_preadd = compile_stamp(metal_pre_add_msl, ok)
    g_pso_swiglu = compile_stamp(MetalSwiglu_metal_ew2_msl, ok)
    g_pso_swiglu_hx = compile_stamp(MetalSwigluHX_metal_ew2_msl, ok)
    g_pso_swiglu_hxb = compile_stamp(MetalSwigluHXB_metal_ew2_msl, ok)
    g_pso_swiglu_b = compile_stamp(MetalSwigluB_metal_ew2_msl, ok)
    g_pso_geglu_hxb = compile_stamp(MetalGegluHXB_metal_ew2_msl, ok)
    g_pso_geglu_b = compile_stamp(MetalGegluB_metal_ew2_msl, ok)
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
    g_pso_geglu = compile_stamp(MetalGeglu_metal_ew2_msl, ok)
    g_pso_geglu_side = compile_stamp(metal_geglu_side_msl, ok)
    g_pso_ple_proj_bf16 = compile_stamp(metal_ple_proj_bf16_msl, ok)
    g_pso_ple_proj_bf16_one = compile_stamp(metal_ple_proj_bf16_one_msl, ok)
    g_pso_ple_finish = compile_stamp(metal_ple_finish_msl, ok)
    g_pso_ple_proj_bf16_hw = compile_stamp(metal_ple_proj_bf16_hw_msl, ok)
    g_pso_ple_proj_bf16_hw_one = compile_stamp(metal_ple_proj_bf16_hw_one_msl, ok)
    g_pso_f32_rows_one = compile_stamp(metal_f32_rows_one_msl, ok)
    g_pso_f32_rows_b = compile_stamp(metal_f32_rows_b_msl, ok)
    g_pso_geglu_hx = compile_stamp(MetalGegluHX_metal_ew2_msl, ok)
    g_pso_ln = compile_stamp(metal_layernorm_msl, ok)
    g_pso_f32mm = compile_stamp(metal_f32_mulmm_msl, ok)
    g_pso_bias_act = compile_stamp(metal_bias_act_msl, ok)
    g_pso_act_rows = compile_stamp(metal_act_rows_msl, ok)
    g_pso_posadd = compile_stamp(metal_posadd2d_msl, ok)
    g_pso_q3v_stem = compile_stamp(metal_q3v_stem_msl, ok)
    g_pso_clamp = compile_stamp(metal_clamp_msl, ok)
    g_pso_clamp_copy = compile_stamp(metal_clamp_copy_msl, ok)
    g_pso_rope2d = compile_stamp(metal_rope2d_msl, ok)
    g_pso_head_pad = compile_stamp(metal_head_pad_msl, ok)
    g_pso_head_unpad = compile_stamp(metal_head_unpad_msl, ok)
    g_pso_geglu_quick = compile_stamp(metal_geglu_quick_msl, ok)
    g_pso_wdec_kv = compile_stamp(metal_wdec_kv_msl, ok)
    g_pso_wdec_kv_h = compile_stamp(metal_wdec_kv_h_msl, ok)
    g_pso_im2col2d = compile_stamp(metal_im2col2d_msl, ok)
    g_pso_wdec_attn_part = compile_stamp(metal_wdec_attn_part_msl, ok)
    g_pso_wdec_attn_comb = compile_stamp(metal_wdec_attn_comb_msl, ok)
    g_pso_bias_add_res = compile_stamp(metal_bias_add_res_msl, ok)
    g_pso_cls_epi = compile_stamp(metal_cls_epi_msl, ok)
    g_pso_cls_select = compile_stamp(metal_cls_select_msl, ok)
    g_pso_attn16 = compile_stamp(MetalSqAttnF16_metal_sq_attn_msl, ok)
    g_pso_attn32 = compile_stamp(MetalSqAttnF32_metal_sq_attn_msl, ok)
    g_pso_attnp16 = compile_stamp(MetalSqAttnPartF16_metal_sq_attn_part_msl, ok)
    g_pso_attnp32 = compile_stamp(MetalSqAttnPartF32_metal_sq_attn_part_msl, ok)
    g_pso_attncomb = compile_stamp(MetalSqAttnComb_metal_sq_attn_comb_msl, ok)
    g_pso_attn_q8 = compile_stamp(MetalSqAttnQ8_metal_sq_attn_msl, ok)
    g_pso_attnp_q8 = compile_stamp(MetalSqAttnPartQ8_metal_sq_attn_part_msl, ok)
    g_pso_attn_tq4 = compile_stamp(MetalSqAttnTq4_metal_sq_attn_msl, ok)
    g_pso_attnp_tq4 = compile_stamp(MetalSqAttnPartTq4_metal_sq_attn_part_msl, ok)
    g_pso_rpst16 = compile_stamp(MetalRopeStoreF16_metal_rope_store_msl, ok)
    g_pso_qkn16 = compile_stamp(metal_qkn_rope16_msl, ok)
    g_pso_qkn32 = compile_stamp(metal_qkn_rope32_msl, ok)
    g_pso_qknb16 = compile_stamp(metal_qkn_rope_b16_msl, ok)
    g_pso_qknb32 = compile_stamp(metal_qkn_rope_b32_msl, ok)
    g_pso_rpst32 = compile_stamp(MetalRopeStoreF32_metal_rope_store_msl, ok)
    g_pso_rpst_q8 = compile_stamp(metal_rope_store_q8_msl, ok)
    g_pso_rpst_tq4 = compile_stamp(metal_rope_store_tq4_msl, ok)
    g_pso_unsign = compile_stamp(metal_fwht_unsign_msl, ok)
    g_pso_qknorm = compile_stamp(metal_qk_norm_msl, ok)
    g_pso_argmax = compile_stamp(metal_argmax_msl, ok)
    g_pso_argmax_rows = compile_stamp(metal_argmax_rows_msl, ok)
    g_pso_moe_router = compile_stamp(metal_moe_router_msl, ok)
    g_pso_moe_router_b = compile_stamp(metal_moe_router_b_msl, ok)
    g_pso_dn_ba = compile_stamp(MetalDnBa_metal_q8_gemv_msl, ok)
    g_pso_moe_select = compile_stamp(MetalMoeSelect_metal_moe_select_msl, ok)
    g_pso_moe_select_512 = compile_stamp(MetalMoeSelect512_metal_moe_select_msl, ok)
    g_pso_moe_select_ds = compile_stamp(MetalMoeSelectDs_metal_moe_select_msl, ok)
    g_pso_moe_select_512_ds = compile_stamp(MetalMoeSelect512Ds_metal_moe_select_msl, ok)
    g_pso_moe_gemv_q8 = compile_stamp(metal_moe_gemv_q8_msl, ok)
    g_pso_moe_kq_k4 = compile_stamp(metal_moe_gemv_k4_msl, ok)
    g_pso_moe_kq_k3 = compile_stamp(metal_moe_gemv_k3_msl, ok)
    g_pso_moe_kq_iq3s = compile_stamp(metal_moe_gemv_iq3s_msl, ok)
    g_pso_moe_kq_iq3xxs = compile_stamp(metal_moe_gemv_iq3xxs_msl, ok)
    g_pso_moe_kq_iq2s = compile_stamp(metal_moe_gemv_iq2s_msl, ok)
    g_pso_moe_kq_iq2xs = compile_stamp(metal_moe_gemv_iq2xs_msl, ok)
    g_pso_moe_kq_iq2xxs = compile_stamp(metal_moe_gemv_iq2xxs_msl, ok)
    g_pso_moe_kq_iq4xs = compile_stamp(metal_moe_gemv_iq4xs_msl, ok)
    g_pso_moe_kq_iq3s4 = compile_stamp(metal_moe_gemv_iq3s4_msl, ok)
    g_pso_moe_kq_q40 = compile_stamp(metal_moe_gemv_q40_msl, ok)
    g_pso_moe_kq_nv4 = compile_stamp(metal_moe_gemv_nv4_msl, ok)
    g_pso_moe_kq_iq4nl = compile_stamp(metal_moe_gemv_iq4nl_msl, ok)
    g_pso_moe_kq_k2 = compile_stamp(metal_moe_gemv_k2_msl, ok)
    g_pso_moe_kq_k5 = compile_stamp(metal_moe_gemv_k5_msl, ok)
    g_pso_moe_kq_k6 = compile_stamp(metal_moe_gemv_k6_msl, ok)
    g_pso_moe_combine = compile_stamp(metal_moe_combine_msl, ok)
    g_pso_moe_combine_sh = compile_stamp(metal_moe_combine_sh_msl, ok)
    g_pso_moe_comb_g4 = compile_stamp(metal_moe_comb_g4_msl, ok)
    g_pso_moe_comb_g4_hx = compile_stamp(metal_moe_comb_g4_hx_msl, ok)
    g_pso_moe_comb_g4_pf = compile_stamp(metal_moe_comb_g4_pf_msl, ok)
    g_pso_g4rnorm = compile_stamp(metal_g4_router_norm_msl, ok)
    g_pso_swiglu_oai = compile_stamp(MetalSwigluOai_metal_swiglu_oai_msl, ok)
    g_pso_moe_gemv_mx4 = compile_stamp(metal_moe_gemv_mx4_msl, ok)
    g_pso_moe_gemv_q51 = compile_stamp(metal_moe_gemv_q51_msl, ok)
    g_pso_moe_gemv_iq4nl32 = compile_stamp(metal_moe_gemv_iq4nl32_msl, ok)
    g_pso_kq_q51 = compile_stamp(metal_kq_gemv_q51_msl, ok)
    g_pso_kq_iq4nl32 = compile_stamp(metal_kq_gemv_iq4nl32_msl, ok)
    g_pso_b32_mvb2_q51 = compile_stamp(MetalB32MvB2Q51_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb4_q51 = compile_stamp(MetalB32MvB4Q51_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb8_q51 = compile_stamp(MetalB32MvB8Q51_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb2_iq4nl32 = compile_stamp(MetalB32MvB2Iq4nl32_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb4_iq4nl32 = compile_stamp(MetalB32MvB4Iq4nl32_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb8_iq4nl32 = compile_stamp(MetalB32MvB8Iq4nl32_metal_b32_mvb_msl, ok)
    g_pso_kq_q51_ks = compile_stamp(metal_kq_gemv_q51_ks_msl, ok)
    g_pso_kq_iq4nl32_ks = compile_stamp(metal_kq_gemv_iq4nl32_ks_msl, ok)
    g_pso_b32_mvb2_q51_ks = compile_stamp(MetalB32MvB2Q51Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb4_q51_ks = compile_stamp(MetalB32MvB4Q51Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb8_q51_ks = compile_stamp(MetalB32MvB8Q51Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb2_iq4nl32_ks = compile_stamp(MetalB32MvB2Iq4nl32Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb4_iq4nl32_ks = compile_stamp(MetalB32MvB4Iq4nl32Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb8_iq4nl32_ks = compile_stamp(MetalB32MvB8Iq4nl32Ks_metal_b32_mvb_msl, ok)
    g_pso_moe_gemv_q4032 = compile_stamp(metal_moe_gemv_q4032_msl, ok)
    g_pso_kq_q4032 = compile_stamp(metal_kq_gemv_q4032_msl, ok)
    g_pso_kq_q4032_ks = compile_stamp(metal_kq_gemv_q4032_ks_msl, ok)
    g_pso_b32_mvb2_q4032 = compile_stamp(MetalB32MvB2Q4032_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb4_q4032 = compile_stamp(MetalB32MvB4Q4032_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb8_q4032 = compile_stamp(MetalB32MvB8Q4032_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb2_q4032_ks = compile_stamp(MetalB32MvB2Q4032Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb4_q4032_ks = compile_stamp(MetalB32MvB4Q4032Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb8_q4032_ks = compile_stamp(MetalB32MvB8Q4032Ks_metal_b32_mvb_msl, ok)
    g_pso_moe_gemv_q5032 = compile_stamp(metal_moe_gemv_q5032_msl, ok)
    g_pso_kq_q5032 = compile_stamp(metal_kq_gemv_q5032_msl, ok)
    g_pso_kq_q5032_ks = compile_stamp(metal_kq_gemv_q5032_ks_msl, ok)
    g_pso_b32_mvb2_q5032 = compile_stamp(MetalB32MvB2Q5032_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb4_q5032 = compile_stamp(MetalB32MvB4Q5032_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb8_q5032 = compile_stamp(MetalB32MvB8Q5032_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb2_q5032_ks = compile_stamp(MetalB32MvB2Q5032Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb4_q5032_ks = compile_stamp(MetalB32MvB4Q5032Ks_metal_b32_mvb_msl, ok)
    g_pso_b32_mvb8_q5032_ks = compile_stamp(MetalB32MvB8Q5032Ks_metal_b32_mvb_msl, ok)
    g_pso_dn_conv = compile_stamp(metal_dn_conv_msl, ok)
    g_pso_dn_conv_n = compile_stamp(metal_dn_conv_n_msl, ok)
    g_pso_dn_conv_n4 = compile_stamp(metal_dn_conv_n4_msl, ok)
    g_pso_dn_hist = compile_stamp(metal_dn_conv_hist_msl, ok)
    g_pso_dn_l2 = compile_stamp(metal_dn_l2norm_msl, ok)
    g_pso_dn_scan = compile_stamp(MetalDnScan_metal_dn_scan_msl, ok)
    g_pso_dn_scan_h = compile_stamp(MetalDnScanH_metal_dn_scan_msl, ok)
    g_pso_dn_prep = compile_stamp(metal_dn_prep_msl, ok)
    g_pso_st2_im2col = compile_stamp(metal_st2_im2col_msl, ok)
    g_pso_st2_conv_mm = compile_stamp(metal_st2_conv_mm_msl, ok)
    g_pso_st2_conv_tr_mm = compile_stamp(metal_st2_conv_tr_mm_msl, ok)
    g_pso_f32_exact_mm = compile_stamp(metal_f32_exact_mulmm_msl, ok)
    g_pso_st2_conv_exact_mm = compile_stamp(metal_st2_conv_exact_mm_msl, ok)
    g_pso_st2_conv_tr_exact_mm = compile_stamp(metal_st2_conv_tr_exact_mm_msl, ok)
    g_pso_st2_adain_leaky = compile_stamp(metal_st2_adain_leaky_msl, ok)
    g_pso_row_gather = compile_stamp(metal_row_gather_msl, ok)
    g_pso_st2_concat = compile_stamp(metal_st2_concat_msl, ok)
    g_pso_st2_pool_dw = compile_stamp(metal_st2_pool_dw_msl, ok)
    g_pso_st2_lstm = compile_stamp(metal_st2_lstm_msl, ok)
    g_pso_st2_sigsum = compile_stamp(metal_st2_sigsum_msl, ok)
    g_pso_st2_gelu_tanh = compile_stamp(metal_st2_gelu_tanh_msl, ok)
    g_pso_st2_attn = compile_stamp(metal_st2_attn_msl, ok)
    g_pso_st2_src_low_torch = compile_stamp(metal_st2_src_low_torch_msl, ok)
    g_pso_st2_src_low_onnx = compile_stamp(metal_st2_src_low_onnx_msl, ok)
    g_pso_st2_src_cumsum_torch = compile_stamp(metal_st2_src_cumsum_torch_msl, ok)
    g_pso_st2_src_cumsum_onnx = compile_stamp(metal_st2_src_cumsum_onnx_msl, ok)
    g_pso_st2_src_noise = compile_stamp(metal_st2_src_noise_msl, ok)
    g_pso_st2_src_sines_torch = compile_stamp(metal_st2_src_sines_torch_msl, ok)
    g_pso_st2_src_sines_onnx = compile_stamp(metal_st2_src_sines_onnx_msl, ok)
    g_pso_st2_stft_reflect = compile_stamp(metal_st2_stft_reflect_msl, ok)
    g_pso_st2_stft_edge = compile_stamp(metal_st2_stft_edge_msl, ok)
    g_pso_st2_istft = compile_stamp(metal_st2_istft_msl, ok)
    g_pso_pk_rows = compile_stamp(metal_pk_rows_msl, ok)
    g_pso_pk_rows_elu = compile_stamp(metal_pk_rows_elu_msl, ok)
    g_pso_pk_row_scale = compile_stamp(metal_pk_row_scale_msl, ok)
    g_pso_pk_attn = compile_stamp(metal_pk_attn_msl, ok)
    g_pso_pk_rope_tab = compile_stamp(metal_pk_rope_tab_msl, ok)
    g_pso_pk_add_ln = compile_stamp(metal_pk_add_ln_msl, ok)
    g_pso_tw_rms = compile_stamp(metal_tw_rms_msl, ok)
    g_pso_tw_patch_im2col = compile_stamp(metal_tw_patch_im2col_msl, ok)
    g_pso_tw_add_ln = compile_stamp(metal_tw_add_ln_msl, ok)
    g_pso_tw_add_rms = compile_stamp(metal_tw_add_rms_msl, ok)
    g_pso_tw_add_post_rms = compile_stamp(metal_tw_add_post_rms_msl, ok)
    g_pso_tw_rope_tab = compile_stamp(metal_tw_rope_tab_msl, ok)
    g_pso_tw_bias_gate = compile_stamp(metal_tw_bias_gate_msl, ok)
    g_pso_pk_gemv_f32 = compile_stamp(metal_pk_gemv_f32_msl, ok)
    g_pso_pk_gemv_f32_ln_silu = compile_stamp(metal_pk_gemv_f32_ln_silu_msl, ok)
    g_pso_pk_gemv_f32_gate = compile_stamp(metal_pk_gemv_f32_gate_msl, ok)
    g_pso_pk_gemv_f32_add_silu = compile_stamp(metal_pk_gemv_f32_add_silu_msl, ok)
    g_pso_pk_gemv_f32_tail = compile_stamp(metal_pk_gemv_f32_tail_msl, ok)
    g_pso_pk_gemv_q8_ln_silu = compile_stamp(metal_pk_gemv_q8_ln_silu_msl, ok)
    g_pso_pk_gemv_q8_gate = compile_stamp(metal_pk_gemv_q8_gate_msl, ok)
    g_pso_pk_gemv_q8_add_silu = compile_stamp(metal_pk_gemv_q8_add_silu_msl, ok)
    g_pso_pk_gemv_q8_tail = compile_stamp(metal_pk_gemv_q8_tail_msl, ok)
    g_pso_st2_colstats = compile_stamp(metal_st2_colstats_msl, ok)
    g_pso_st2_adain_snake = compile_stamp(metal_st2_adain_snake_msl, ok)
    g_pso_st2_leaky = compile_stamp(metal_st2_leaky_msl, ok)
    g_pso_st2_axpy = compile_stamp(metal_st2_axpy_msl, ok)
    g_pso_st2_add = compile_stamp(metal_st2_add_msl, ok)
    g_pso_st2_reflect1 = compile_stamp(metal_st2_reflect1_msl, ok)
    g_pso_dn_gate = compile_stamp(MetalDnGate_metal_dn_gate_msl, ok)
    g_pso_dn_gate_sig = compile_stamp(MetalDnGateSig_metal_dn_gate_msl, ok)
    g_pso_hc_norm = compile_stamp(metal_hc_norm_msl, ok)
    g_pso_hc_lo = compile_stamp(MetalHcLo_metal_ew1_msl, ok)
    g_pso_hc_init = compile_stamp(metal_hc_init_msl, ok)
    g_pso_cat2 = compile_stamp(metal_cat2_msl, ok)
    g_pso_hc_mix = compile_stamp(metal_hc_mix_msl, ok)
    g_pso_hc_combine = compile_stamp(metal_hc_combine_msl, ok)
    g_pso_ple_gate = compile_stamp(metal_ple_gate_msl, ok)
    g_pso_ple_conv = compile_stamp(metal_ple_conv_msl, ok)
    g_pso_dn_deint = compile_stamp(metal_dn_deint_msl, ok)
    g_pso_sigmul = compile_stamp(MetalSigmul_metal_ew2_msl, ok)
    g_mx4_vtab = metal_new_buffer(g_dev, 64ul)
    unsafe {
        var vp = reinterpret<float?>(metal_buffer_contents(g_mx4_vtab))
        let mags = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0)
        for (vi in range(16)) {
            vp[vi] = vi < 8 ? mags[vi] : -mags[vi - 8]
        }
    }
    g_pso_copy_row = compile_stamp(metal_copy_row_msl, ok)
    g_pso_emb = compile_stamp(metal_embed_q8_msl, ok)
    g_pso_emb_kq_k4 = compile_stamp(MetalEmbedK4_metal_embed_kq_msl, ok)
    g_pso_emb_kq_k5 = compile_stamp(MetalEmbedK5_metal_embed_kq_msl, ok)
    g_pso_emb_kq_k6 = compile_stamp(MetalEmbedK6_metal_embed_kq_msl, ok)
    g_pso_emb_kq_iq4xs = compile_stamp(MetalEmbedIq4xs_metal_embed_kq_msl, ok)
    g_pso_emb_kq_iq3s4 = compile_stamp(MetalEmbedIq3s4_metal_embed_kq_msl, ok)
    g_pso_emb_kq_iq4nl = compile_stamp(MetalEmbedIq4nl_metal_embed_kq_msl, ok)
    g_pso_emb_kq_q40 = compile_stamp(MetalEmbedQ40_metal_embed_kq_msl, ok)
    g_pso_emb_kq_nv4 = compile_stamp(MetalEmbedNv4_metal_embed_kq_msl, ok)
    g_pso_emb_kq_k3 = compile_stamp(MetalEmbedK3_metal_embed_kq_msl, ok)
    g_pso_emb_kq_k2 = compile_stamp(MetalEmbedK2_metal_embed_kq_msl, ok)
    g_pso_emb_kq_iq3s = compile_stamp(MetalEmbedIq3s_metal_embed_kq_msl, ok)
    g_pso_emb_kq_iq3xxs = compile_stamp(MetalEmbedIq3xxs_metal_embed_kq_msl, ok)
    g_pso_emb_kq_iq2s = compile_stamp(MetalEmbedIq2s_metal_embed_kq_msl, ok)
    g_pso_emb_kq_iq2xs = compile_stamp(MetalEmbedIq2xs_metal_embed_kq_msl, ok)
    g_pso_emb_kq_iq2xxs = compile_stamp(MetalEmbedIq2xxs_metal_embed_kq_msl, ok)
    g_pso_emb_b32_q51 = compile_stamp(MetalEmbedB32Q51_metal_embed_b32_msl, ok)
    g_pso_emb_b32_iq4nl32 = compile_stamp(MetalEmbedB32Iq4nl32_metal_embed_b32_msl, ok)
    g_pso_emb_b32_q4032 = compile_stamp(MetalEmbedB32Q4032_metal_embed_b32_msl, ok)
    g_pso_emb_b32_q5032 = compile_stamp(MetalEmbedB32Q5032_metal_embed_b32_msl, ok)
    g_pso_qkvrs16 = compile_stamp(MetalQ8GemvQkvRsF16_metal_q8_gemv_qkv_rs_msl, ok)
    g_pso_qkvrs32 = compile_stamp(MetalQ8GemvQkvRsF32_metal_q8_gemv_qkv_rs_msl, ok)
    g_pso_w13sw = compile_stamp(MetalQ8GemvW13Sw_metal_q8_gemv_w13sw16_msl, ok)
    g_pso_w13sw_sg = compile_stamp(MetalQ8GemvW13SwSg_metal_q8_gemv_w13sw16_msl, ok)
    g_pso_kq_k4 = compile_stamp(metal_kq_gemv_k4_msl, ok)
    g_pso_kq_k5 = compile_stamp(metal_kq_gemv_k5_msl, ok)
    g_pso_kq_k5c = compile_stamp(metal_kq_gemv_k5c_msl, ok)
    g_pso_kq_k6 = compile_stamp(metal_kq_gemv_k6_msl, ok)
    g_pso_kq_mvb2_k4 = compile_stamp(MetalKqMvB2K4_metal_kq_mv_k4_msl, ok)
    g_pso_kq_mvb2_k4r2 = compile_stamp(MetalKqMvB2K4R2_metal_kq_mv_k4_r2_msl, ok)
    g_pso_kq_mvw2h_k4 = compile_stamp(MetalKqMvW2K4H_metal_kq_mvw_k4_msl, ok)
    g_pso_kq_mvw4h_k4 = compile_stamp(MetalKqMvW4K4H_metal_kq_mvw_k4_msl, ok)
    g_pso_kq_mvw8h_k4 = compile_stamp(MetalKqMvW8K4H_metal_kq_mvw_k4_msl, ok)
    // every other format's half-x small-batch stamps (the per-format `kq_rows_half_<fmt>` crowns)
    g_pso_kq_mvb2h_k5 = compile_stamp(MetalKqMvB2K5H_metal_kq_mv_k5_msl, ok)
    g_pso_kq_mvb4h_k5 = compile_stamp(MetalKqMvB4K5H_metal_kq_mv_k5_msl, ok)
    g_pso_kq_mvb8h_k5 = compile_stamp(MetalKqMvB8K5H_metal_kq_mvb8_msl, ok)
    g_pso_kq_mvb2h_k6 = compile_stamp(MetalKqMvB2K6H_metal_kq_mv_k6_msl, ok)
    g_pso_kq_mvb4h_k6 = compile_stamp(MetalKqMvB4K6H_metal_kq_mv_k6_msl, ok)
    g_pso_kq_mvb8h_k6 = compile_stamp(MetalKqMvB8K6H_metal_kq_mvb8_k6_msl, ok)
    g_pso_kq_mvb2h_iq4xs = compile_stamp(MetalKqMvB2Iq4xsH_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4h_iq4xs = compile_stamp(MetalKqMvB4Iq4xsH_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8h_iq4xs = compile_stamp(MetalKqMvB8Iq4xsH_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_mvb2h_iq4nl = compile_stamp(MetalKqMvB2Iq4nlH_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4h_iq4nl = compile_stamp(MetalKqMvB4Iq4nlH_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8h_iq4nl = compile_stamp(MetalKqMvB8Iq4nlH_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_mvb2h_q40 = compile_stamp(MetalKqMvB2Q40H_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4h_q40 = compile_stamp(MetalKqMvB4Q40H_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8h_q40 = compile_stamp(MetalKqMvB8Q40H_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_mvb2h_nv4 = compile_stamp(MetalKqMvB2Nv4H_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4h_nv4 = compile_stamp(MetalKqMvB4Nv4H_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8h_nv4 = compile_stamp(MetalKqMvB8Nv4H_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_mvb2h_k3 = compile_stamp(MetalKqMvB2K3H_metal_kq_mv_k3_msl, ok)
    g_pso_kq_mvb4h_k3 = compile_stamp(MetalKqMvB4K3H_metal_kq_mv_k3_msl, ok)
    g_pso_kq_mvb8h_k3 = compile_stamp(MetalKqMvB8K3H_metal_kq_mvb8_k3_msl, ok)
    g_pso_kq_mvb2h_iq3s = compile_stamp(MetalKqMvB2Iq3sH_metal_kq_mv_iq3s_msl, ok)
    g_pso_kq_mvb4h_iq3s = compile_stamp(MetalKqMvB4Iq3sH_metal_kq_mv_iq3s_msl, ok)
    g_pso_kq_mvb8h_iq3s = compile_stamp(MetalKqMvB8Iq3sH_metal_kq_mvb8_iq3s_msl, ok)
    g_pso_kq_mvb2h_iq3xxs = compile_stamp(MetalKqMvB2Iq3xxsH_metal_kq_mv_iq3xxs_msl, ok)
    g_pso_kq_mvb4h_iq3xxs = compile_stamp(MetalKqMvB4Iq3xxsH_metal_kq_mv_iq3xxs_msl, ok)
    g_pso_kq_mvb8h_iq3xxs = compile_stamp(MetalKqMvB8Iq3xxsH_metal_kq_mvb8_iq3xxs_msl, ok)
    g_pso_kq_mvb2h_k2 = compile_stamp(MetalKqMvB2K2H_metal_kq_mv_k2_msl, ok)
    g_pso_kq_mvb4h_k2 = compile_stamp(MetalKqMvB4K2H_metal_kq_mv_k2_msl, ok)
    g_pso_kq_mvb8h_k2 = compile_stamp(MetalKqMvB8K2H_metal_kq_mvb8_k2_msl, ok)
    g_pso_kq_mvb2h_iq2s = compile_stamp(MetalKqMvB2Iq2sH_metal_kq_mv_iq2s_msl, ok)
    g_pso_kq_mvb4h_iq2s = compile_stamp(MetalKqMvB4Iq2sH_metal_kq_mv_iq2s_msl, ok)
    g_pso_kq_mvb8h_iq2s = compile_stamp(MetalKqMvB8Iq2sH_metal_kq_mvb8_iq2s_msl, ok)
    g_pso_kq_mvb2h_iq2xs = compile_stamp(MetalKqMvB2Iq2xsH_metal_kq_mv_iq2xs_msl, ok)
    g_pso_kq_mvb4h_iq2xs = compile_stamp(MetalKqMvB4Iq2xsH_metal_kq_mv_iq2xs_msl, ok)
    g_pso_kq_mvb8h_iq2xs = compile_stamp(MetalKqMvB8Iq2xsH_metal_kq_mvb8_iq2xs_msl, ok)
    g_pso_kq_mvb2h_iq2xxs = compile_stamp(MetalKqMvB2Iq2xxsH_metal_kq_mv_iq2xxs_msl, ok)
    g_pso_kq_mvb4h_iq2xxs = compile_stamp(MetalKqMvB4Iq2xxsH_metal_kq_mv_iq2xxs_msl, ok)
    g_pso_kq_mvb8h_iq2xxs = compile_stamp(MetalKqMvB8Iq2xxsH_metal_kq_mvb8_iq2xxs_msl, ok)
    g_pso_kq_mvb4_k4 = compile_stamp(MetalKqMvB4K4_metal_kq_mv_k4_msl, ok)
    g_pso_kq_mvb2_k5 = compile_stamp(MetalKqMvB2K5_metal_kq_mv_k5_msl, ok)
    g_pso_kq_mvb4_k5 = compile_stamp(MetalKqMvB4K5_metal_kq_mv_k5_msl, ok)
    g_pso_kq_mvb2_k6 = compile_stamp(MetalKqMvB2K6_metal_kq_mv_k6_msl, ok)
    g_pso_kq_mvb4_k6 = compile_stamp(MetalKqMvB4K6_metal_kq_mv_k6_msl, ok)
    g_pso_kq_mvb8_k4 = compile_stamp(MetalKqMvB8K4_metal_kq_mvb8_msl, ok)
    g_pso_kq_mvb8_k5 = compile_stamp(MetalKqMvB8K5_metal_kq_mvb8_msl, ok)
    g_pso_kq_mvb8_k6 = compile_stamp(MetalKqMvB8K6_metal_kq_mvb8_k6_msl, ok)
    g_pso_kq_mm_b4 = compile_stamp(MetalKqMulMmK4_metal_kq_mulmm_msl, ok)
    g_pso_kq_mm_b5 = compile_stamp(MetalKqMulMmK5_metal_kq_mulmm_msl, ok)
    g_pso_kq_mm_b6 = compile_stamp(MetalKqMulMmK6_metal_kq_mulmm_msl, ok)
    g_pso_kq_iq4xs = compile_stamp(MetalKqGemvIq4xs_metal_kq_gemv_iq4_msl, ok)
    g_pso_kq_mvb2_iq4xs = compile_stamp(MetalKqMvB2Iq4xs_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4_iq4xs = compile_stamp(MetalKqMvB4Iq4xs_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8_iq4xs = compile_stamp(MetalKqMvB8Iq4xs_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_mm_b44 = compile_stamp(MetalKqMulMmIq4xs_metal_kq_mulmm_msl, ok)
    g_pso_kq_iq3s4 = compile_stamp(MetalKqGemvIq3s4_metal_kq_gemv_iq4_msl, ok)
    g_pso_kq_mvb2_iq3s4 = compile_stamp(MetalKqMvB2Iq3s4_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4_iq3s4 = compile_stamp(MetalKqMvB4Iq3s4_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8_iq3s4 = compile_stamp(MetalKqMvB8Iq3s4_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_mm_b35 = compile_stamp(MetalKqMulMmIq3s4_metal_kq_mulmm_msl, ok)
    g_pso_kq_k3 = compile_stamp(metal_kq_gemv_k3_msl, ok)
    g_pso_kq_mvb2_k3 = compile_stamp(MetalKqMvB2K3_metal_kq_mv_k3_msl, ok)
    g_pso_kq_mvb4_k3 = compile_stamp(MetalKqMvB4K3_metal_kq_mv_k3_msl, ok)
    g_pso_kq_mvb8_k3 = compile_stamp(MetalKqMvB8K3_metal_kq_mvb8_k3_msl, ok)
    g_pso_kq_mm_b3 = compile_stamp(MetalKqMulMmK3_metal_kq_mulmm_msl, ok)
    g_pso_kq_iq3s = compile_stamp(metal_kq_gemv_iq3s_msl, ok)
    g_pso_kq_mvb2_iq3s = compile_stamp(MetalKqMvB2Iq3s_metal_kq_mv_iq3s_msl, ok)
    g_pso_kq_mvb4_iq3s = compile_stamp(MetalKqMvB4Iq3s_metal_kq_mv_iq3s_msl, ok)
    g_pso_kq_mvb8_iq3s = compile_stamp(MetalKqMvB8Iq3s_metal_kq_mvb8_iq3s_msl, ok)
    g_pso_kq_mm_b33 = compile_stamp(MetalKqMulMmIq3s_metal_kq_mulmm_msl, ok)
    g_pso_kq_iq3xxs = compile_stamp(metal_kq_gemv_iq3xxs_msl, ok)
    g_pso_kq_mvb2_iq3xxs = compile_stamp(MetalKqMvB2Iq3xxs_metal_kq_mv_iq3xxs_msl, ok)
    g_pso_kq_mvb4_iq3xxs = compile_stamp(MetalKqMvB4Iq3xxs_metal_kq_mv_iq3xxs_msl, ok)
    g_pso_kq_mvb8_iq3xxs = compile_stamp(MetalKqMvB8Iq3xxs_metal_kq_mvb8_iq3xxs_msl, ok)
    g_pso_kq_mm_b34 = compile_stamp(MetalKqMulMmIq3xxs_metal_kq_mulmm_msl, ok)
    g_pso_kq_iq4nl = compile_stamp(MetalKqGemvIq4nl_metal_kq_gemv_iq4_msl, ok)
    g_pso_kq_mvb2_iq4nl = compile_stamp(MetalKqMvB2Iq4nl_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4_iq4nl = compile_stamp(MetalKqMvB4Iq4nl_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8_iq4nl = compile_stamp(MetalKqMvB8Iq4nl_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_iq2s = compile_stamp(metal_kq_gemv_iq2s_msl, ok)
    g_pso_kq_mvb2_iq2s = compile_stamp(MetalKqMvB2Iq2s_metal_kq_mv_iq2s_msl, ok)
    g_pso_kq_mvb4_iq2s = compile_stamp(MetalKqMvB4Iq2s_metal_kq_mv_iq2s_msl, ok)
    g_pso_kq_mvb8_iq2s = compile_stamp(MetalKqMvB8Iq2s_metal_kq_mvb8_iq2s_msl, ok)
    g_pso_kq_mm_b23 = compile_stamp(MetalKqMulMmIq2s_metal_kq_mulmm_msl, ok)
    g_pso_kq_iq2xs = compile_stamp(metal_kq_gemv_iq2xs_msl, ok)
    g_pso_kq_mvb2_iq2xs = compile_stamp(MetalKqMvB2Iq2xs_metal_kq_mv_iq2xs_msl, ok)
    g_pso_kq_mvb4_iq2xs = compile_stamp(MetalKqMvB4Iq2xs_metal_kq_mv_iq2xs_msl, ok)
    g_pso_kq_mvb8_iq2xs = compile_stamp(MetalKqMvB8Iq2xs_metal_kq_mvb8_iq2xs_msl, ok)
    g_pso_kq_mm_b24 = compile_stamp(MetalKqMulMmIq2xs_metal_kq_mulmm_msl, ok)
    g_pso_kq_iq2xxs = compile_stamp(MetalKqGemvIq2xxs_metal_kq_gemv_iq2xxs_msl, ok)
    if (metal_tensor_crowned("kq_gemv_iq3s_f4")) {
        g_pso_kq_iq3s_f4 = compile_stamp(metal_kq_gemv_iq3s_f4_msl, ok)
        g_pso_moe_kq_iq3s_f4 = compile_stamp(metal_moe_gemv_iq3s_f4_msl, ok)
    }
    if (metal_tensor_crowned("kq_gemv_iq3xxs_f4")) {
        g_pso_kq_iq3xxs_f4 = compile_stamp(metal_kq_gemv_iq3xxs_f4_msl, ok)
    }
    if (metal_tensor_crowned("kq_gemv_iq2xxs_f4")) {
        g_pso_kq_iq2xxs_f4 = compile_stamp(MetalKqGemvIq2xxsF4_metal_kq_gemv_iq2xxs_msl, ok)
    }
    g_pso_kq_mvb2_iq2xxs = compile_stamp(MetalKqMvB2Iq2xxs_metal_kq_mv_iq2xxs_msl, ok)
    g_pso_kq_mvb4_iq2xxs = compile_stamp(MetalKqMvB4Iq2xxs_metal_kq_mv_iq2xxs_msl, ok)
    g_pso_kq_mvb8_iq2xxs = compile_stamp(MetalKqMvB8Iq2xxs_metal_kq_mvb8_iq2xxs_msl, ok)
    g_pso_kq_mm_b25 = compile_stamp(MetalKqMulMmIq2xxs_metal_kq_mulmm_msl, ok)
    g_pso_kq_mm_b45 = compile_stamp(MetalKqMulMmIq4nl_metal_kq_mulmm_msl, ok)
    g_pso_kq_q40 = compile_stamp(MetalKqGemvQ40_metal_kq_gemv_iq4_msl, ok)
    g_pso_kq_mvb2_q40 = compile_stamp(MetalKqMvB2Q40_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4_q40 = compile_stamp(MetalKqMvB4Q40_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8_q40 = compile_stamp(MetalKqMvB8Q40_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_mm_b40 = compile_stamp(MetalKqMulMmQ40_metal_kq_mulmm_msl, ok)
    g_pso_kq_nv4 = compile_stamp(MetalKqGemvNv4_metal_kq_gemv_iq4_msl, ok)
    g_pso_kq_mvb2_nv4 = compile_stamp(MetalKqMvB2Nv4_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb4_nv4 = compile_stamp(MetalKqMvB4Nv4_metal_kq_mv_iq4_msl, ok)
    g_pso_kq_mvb8_nv4 = compile_stamp(MetalKqMvB8Nv4_metal_kq_mvb8_iq4_msl, ok)
    g_pso_kq_mm_b49 = compile_stamp(MetalKqMulMmNv4_metal_kq_mulmm_msl, ok)
    g_pso_kq_mm_b51 = compile_stamp(MetalKqMulMmQ51_metal_kq_mulmm_msl, ok)
    g_pso_kq_mm_b4n32 = compile_stamp(MetalKqMulMmIq4nl32_metal_kq_mulmm_msl, ok)
    g_pso_kq_mm_b4032 = compile_stamp(MetalKqMulMmQ4032_metal_kq_mulmm_msl, ok)
    g_pso_kq_mm_b5032 = compile_stamp(MetalKqMulMmQ5032_metal_kq_mulmm_msl, ok)
    g_pso_kq_k2 = compile_stamp(metal_kq_gemv_k2_msl, ok)
    g_pso_kq_mvb2_k2 = compile_stamp(MetalKqMvB2K2_metal_kq_mv_k2_msl, ok)
    g_pso_kq_mvb4_k2 = compile_stamp(MetalKqMvB4K2_metal_kq_mv_k2_msl, ok)
    g_pso_kq_mvb8_k2 = compile_stamp(MetalKqMvB8K2_metal_kq_mvb8_k2_msl, ok)
    g_pso_kq_mm_b2 = compile_stamp(MetalKqMulMmK2_metal_kq_mulmm_msl, ok)
    // the batched-step set (P4)
    g_pso_gemv_b2 = compile_stamp(MetalGemvB2_metal_gemv_msl, ok)
    g_pso_gemv_b4 = compile_stamp(MetalGemvB4_metal_gemv_msl, ok)
    g_pso_mv_b2 = compile_stamp(metal_q8_mv_b2_msl, ok)
    g_pso_mv_b4 = compile_stamp(metal_q8_mv_b4_msl, ok)
    g_pso_w13sw_b2 = compile_stamp(MetalGemvW13SwB2_metal_gemv_w13sw_msl, ok)
    g_pso_w13sw_b4 = compile_stamp(MetalGemvW13SwB4_metal_gemv_w13sw_msl, ok)
    for (f in KQ_ROWS_RACE_FORMATS) {
        g_kq_rows_crowned[int(f.fmt)] = metal_tensor_crowned("kq_rows_{f.fmt}")
    }
    g_kq_k4_r2_crowned = metal_tensor_crowned("kq_mvb2_k4_r2")
    if (!g_kq_half_set) {   //! a setter's value outlives a shutdown and re-init; the environment's is read only until one speaks
        g_kq_half_env = g_env_metal.kq_half
    }
    if (!g_kq_half_env) {
        to_log(LOG_WARNING, "dasLLAMA metal decode: DASLLAMA_METAL_KQ_HALF=0 - the K-quant rows sites take the f32 forms under every half-x crown (numerics move with the form)\n")
    }
    for (f in KQ_ROWS_RACE_FORMATS) {
        g_kq_half_crowned[int(f.fmt)] = metal_tensor_crowned(kq_half_crown(f.fmt))
    }
    g_gemm_b_tensor = metal_tensor_crowned("gemmb_q8")
    g_pso_gemm_b = (g_gemm_b_tensor
        ? compile_stamp(MetalQ8GemmBT_metal_q8_gemm_b_t_msl, ok)
        : compile_stamp(MetalQ8GemmB_metal_q8_gemm_b_msl, ok))
    g_gemm_b_sk_tensor = metal_tensor_crowned("gemmb_sk_q8")
    g_pso_gemm_b_sk = (g_gemm_b_sk_tensor
        ? compile_stamp(MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl, ok)
        : compile_stamp(MetalQ8GemmBSk_metal_q8_gemm_b_msl, ok))
    g_pso_skred = compile_stamp(metal_sk_reduce_msl, ok)
    g_gemm64_b_tensor = metal_tensor_crowned("gemm64b_q8")
    g_pso_gemm64_b = (g_gemm64_b_tensor
        ? compile_stamp(MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl, ok)
        : compile_stamp(metal_q8_gemm64_b_msl, ok))
    g_pso_mm = compile_stamp(MetalQ8MulMm_metal_q8_mulmm_msl, ok)
    metal_base_split_init(ok)
    g_pso_attnb16 = compile_stamp(MetalSqAttnBF16_metal_sq_attn_b_msl, ok)
    g_pso_attnb32 = compile_stamp(MetalSqAttnBF32_metal_sq_attn_b_msl, ok)
    g_pso_attnb_q8 = compile_stamp(MetalSqAttnBQ8_metal_sq_attn_b_msl, ok)
    g_pso_attnb_tq4 = compile_stamp(MetalSqAttnBTq4_metal_sq_attn_b_msl, ok)
    g_pso_attnpb16 = compile_stamp(MetalSqAttnPartBF16_metal_sq_attn_part_b_msl, ok)
    g_pso_attnpb32 = compile_stamp(MetalSqAttnPartBF32_metal_sq_attn_part_b_msl, ok)
    g_pso_attnpb_q8 = compile_stamp(MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl, ok)
    g_pso_attnpb_tq4 = compile_stamp(MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl, ok)
    g_pso_attncombb = compile_stamp(MetalSqAttnCombB_metal_sq_attn_comb_msl, ok)
    g_pso_attnd16 = compile_stamp(MetalSqAttnDF16_metal_sq_attn_d_msl, ok)
    g_pso_attnd32 = compile_stamp(MetalSqAttnDF32_metal_sq_attn_d_msl, ok)
    g_pso_attndw16 = compile_stamp(MetalSqAttnDWF16_metal_sq_attn_d_msl, ok)
    g_pso_attndw32 = compile_stamp(MetalSqAttnDWF32_metal_sq_attn_d_msl, ok)
    g_pso_attndx16 = compile_stamp(MetalSqAttnDXF16_metal_sq_attn_d_msl, ok)
    g_pso_attndx32 = compile_stamp(MetalSqAttnDXF32_metal_sq_attn_d_msl, ok)
    g_pso_attnd_q8 = compile_stamp(MetalSqAttnDQ8_metal_sq_attn_d_msl, ok)
    g_pso_attnd_tq4 = compile_stamp(MetalSqAttnDTq4_metal_sq_attn_d_msl, ok)
    g_pso_attndw_q8 = compile_stamp(MetalSqAttnDWQ8_metal_sq_attn_d_msl, ok)
    g_pso_attndw_tq4 = compile_stamp(MetalSqAttnDWTq4_metal_sq_attn_d_msl, ok)
    g_pso_attndx_q8 = compile_stamp(MetalSqAttnDXQ8_metal_sq_attn_d_msl, ok)
    g_pso_attndx_tq4 = compile_stamp(MetalSqAttnDXTq4_metal_sq_attn_d_msl, ok)
    g_pso_attncombd = compile_stamp(metal_sq_attn_comb_d_msl, ok)
    g_pso_rpstb16 = compile_stamp(MetalRopeStoreBF16_metal_rope_store_b_msl, ok)
    g_pso_rpstb32 = compile_stamp(MetalRopeStoreBF32_metal_rope_store_b_msl, ok)
    g_pso_rpstb_q8 = compile_stamp(metal_rope_store_b_q8_msl, ok)
    g_pso_rpstb_tq4 = compile_stamp(metal_rope_store_b_tq4_msl, ok)
    g_pso_kv_rows_q8 = compile_stamp(metal_kv_rows_q8_msl, ok)
    g_pso_kv_rows_tq4 = compile_stamp(metal_kv_rows_tq4_msl, ok)
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

let GEMV_SG_MAX_N = 1536l

def enc_gemv_sg(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64; bx, by, bn, bd : MetalBuffer?; d : int64; yoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    enc_gemv_sg_c(enc, bw, boff, bw, bx, xoff, by, yoff, bn, bd, d)
}

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#metal-layer-enc")]
def enc_q8_site_gemv(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64; bx, by, bn, bd : MetalBuffer?; d, n : int64; yoff : uint64 = 0ul) {
    if (n <= GEMV_SG_MAX_N) {
        enc_gemv_sg(enc, bw, boff, bx, by, bn, bd, d, yoff)
    } else {
        enc_gemv(enc, bw, boff, bx, by, bn, bd, d, yoff)
    }
}

//! one row of a dense per-32 plane (q51, iq4nl32, q4032, q5032) through its GEMV over the verbatim plane pair
def enc_b32_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64; bx, by, bn, bd : MetalBuffer?; yoff, xoff : uint64) {
    let ks = b32_narrow(rows, n, 1l)
    if (ks) {
        g_b32_ks++
    }
    let qp = b32_planes_of(g_dev, t, fmt, woff)
    if (fmt == KqFmt.q51) {
        if (ks) {
            enc_kq_q51_ks_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, rows, n)
        } else {
            enc_kq_q51_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, rows, n)
        }
    } elif (fmt == KqFmt.iq4nl32) {
        if (ks) {
            enc_kq_iq4nl32_ks_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, rows, n)
        } else {
            enc_kq_iq4nl32_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, rows, n)
        }
    } elif (fmt == KqFmt.q5032) {
        if (ks) {
            enc_kq_q5032_ks_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, rows, n)
        } else {
            enc_kq_q5032_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, rows, n)
        }
    } else {
        if (ks) {
            enc_kq_q4032_ks_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, rows, n)
        } else {
            enc_kq_q4032_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, rows, n)
        }
    }
}

let B32_NARROW_ROWS = 512l        //! a plane of at most this many rows takes the K-split stamps at any row count
let B32_NARROW_WIDE_ROWS = 2048l  //! ... and one of at most this many at one or two x rows
let B32_NARROW_LONG_K = 2048l     //! ... and any plane at one x row whose K is at least this long

var private g_b32_ks = 0l   //! per-32 K-split (narrow-plane) dispatches since load

//! the small-batch stamps by `enc_b32_mvb`'s key: column width (2, 4, 8) major, then the K split, then the format (q51, iq4nl32,
//! q4032, q5032)
let B32_MVB_STEMS = "b32_mvb2_q51 b32_mvb2_iq4nl32 b32_mvb2_q4032 b32_mvb2_q5032 b32_mvb2_q51_ks b32_mvb2_iq4nl32_ks b32_mvb2_q4032_ks b32_mvb2_q5032_ks b32_mvb4_q51 b32_mvb4_iq4nl32 b32_mvb4_q4032 b32_mvb4_q5032 b32_mvb4_q51_ks b32_mvb4_iq4nl32_ks b32_mvb4_q4032_ks b32_mvb4_q5032_ks b32_mvb8_q51 b32_mvb8_iq4nl32 b32_mvb8_q4032 b32_mvb8_q5032 b32_mvb8_q51_ks b32_mvb8_iq4nl32_ks b32_mvb8_q4032_ks b32_mvb8_q5032_ks"

//! the per-32 K-split dispatches since load - the narrow-plane form's witness
def metal_b32_ks_count() : int64 => g_b32_ks

[arch(at="../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-b32-small-batch")]
def b32_narrow(rows, n, nrows : int64) : bool {
    return (rows <= B32_NARROW_ROWS || (rows <= B32_NARROW_WIDE_ROWS && nrows <= 2l)
        || (nrows == 1l && n >= B32_NARROW_LONG_K))
}

var private g_b32_mvb = 0l        //! per-32 small-batch dispatches since load
var private g_b32_mvb_rows = 0l   //! ... and the x rows they served

//! the per-32 small-batch dispatches and the rows they served since load - the batched sites' witness
def metal_b32_mvb_stats() : tuple<dispatches : int64; rows : int64> => (dispatches = g_b32_mvb, rows = g_b32_mvb_rows)

//! `nlive` x rows (`n` apart) of a dense per-32 plane in one dispatch: the two-column form at two, the four-column form to four,
//! else the eight-column form over ceil(nlive / 8) column groups; y row b at `yoff + b * ystride` floats
[arch(at="../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-b32-small-batch")]
def enc_b32_mvb(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64; bx, by : MetalBuffer?; yoff : uint64;
                bn, bd : MetalBuffer?; ystride, nlive : int64; xoff : uint64 = 0ul) {
    var mv = KqMvArgs(ndim = uint(n), ddim = uint(rows), ys = uint(ystride), nr = uint(nlive))
    g_b32_mvb++
    g_b32_mvb_rows += nlive
    let qp = b32_planes_of(g_dev, t, fmt, woff)
    let w = nlive == 2l ? 2l : (nlive <= 4l ? 4l : 8l)
    let gcols = (nlive + w - 1l) / w
    let ks = b32_narrow(rows, n, nlive)
    if (ks) {
        g_b32_ks++
    }
    let fi = fmt == KqFmt.q51 ? 0 : (fmt == KqFmt.iq4nl32 ? 1 : (fmt == KqFmt.q4032 ? 2 : 3))
    let key = (w == 2l ? 0 : (w == 4l ? 1 : 2)) * 8 + (ks ? 4 : 0) + fi
    enc_b32_mvb_stamp(key, enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, xoff, by, yoff, bn, bd, mv, rows, gcols)
}

//! the small-batch stamp `key` names in `B32_MVB_STEMS` (column width, K split, format)
def private enc_b32_mvb_stamp(key : int; enc : MetalComputeEncoder?; bq : MetalBuffer?; qoff : uint64; bs : MetalBuffer?; soff : uint64;   // nolint:STYLE037 — the stamp macro's one ladder arm per stem
                              bx : MetalBuffer ?; xoff : uint64; by : MetalBuffer?; yoff : uint64; bn, bd : MetalBuffer?; var mv : KqMvArgs; rows, gcols : int64) {
    stamp_ladder(key, B32_MVB_STEMS) <| $() {
        stamp_enc_c(enc, bq, qoff, bs, soff, bx, xoff, by, yoff, bn, bd, mv, rows, gcols)
    }
}

// K-quant site GEMV — the per-format twin of enc_gemv over the kq plane pair
// (kq_upload_quants + kq_upload_scales). k6 binds the scale buffer twice: the trailing packed
// f16 d plane at byte nsb*16, the 16B sub-scale blocks at 0. Geometry: 64-thread tg, 4 rows.
[arch(at="../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-iquant-gemv-grid")]
def enc_kq_gemv(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
                bs : MetalBuffer?; soff, doff : uint64; bx, by, bn, bd : MetalBuffer?;
                rows, n : int64; yoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    if (fmt == KqFmt.iq4xs) {
        enc_kq_iq4xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq3s4) {
        enc_kq_iq3s4_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.k3) {
        enc_kq_k3_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq3s) {
        if (g_pso_kq_iq3s_f4 != null) {
            enc_kq_iq3s_f4_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
        } else {
            enc_kq_iq3s_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
        }
    } elif (fmt == KqFmt.iq3xxs) {
        if (g_pso_kq_iq3xxs_f4 != null) {
            enc_kq_iq3xxs_f4_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
        } else {
            enc_kq_iq3xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
        }
    } elif (fmt == KqFmt.iq4nl) {
        enc_kq_iq4nl_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.q40) {
        enc_kq_q40_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.nv4) {
        enc_kq_nv4_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.k2) {
        enc_kq_k2_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq2s) {
        enc_kq_iq2s_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq2xs) {
        enc_kq_iq2xs_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
    } elif (fmt == KqFmt.iq2xxs) {
        if (g_pso_kq_iq2xxs_f4 != null) {
            enc_kq_iq2xxs_f4_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
        } else {
            enc_kq_iq2xxs_c(enc, bs, doff, bs, soff, bq, qoff, bx, xoff, by, yoff, bn, bd, rows, n)
        }
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

//! K-quant small-batch mv (B >= 2): R=4 column groups via grid.y, R=2 via the B2 PSO; B >= 5 rides
//! the staged B8 twin over 8-column groups via grid.y (DASLLAMA_METAL_KQ_B8=0 = the A/B rail).
def private kq_b8_on : bool => g_env_metal.kq_b8

typedef KqMvbBuilder = function<(enc : MetalComputeEncoder?; s0 : MetalBuffer?; s0off : uint64; s : MetalBuffer?; soff : uint64;
                                 q : MetalBuffer?; qoff : uint64; x : MetalBuffer?; xoff : uint64; y : MetalBuffer?; yoff : uint64; var ka : KqMvArgs; rows : int64) : void>
typedef KqMvbBuilder4 = function<(enc : MetalComputeEncoder?; s0 : MetalBuffer?; s0off : uint64; s : MetalBuffer?; soff : uint64;
                                  q : MetalBuffer?; qoff : uint64; x : MetalBuffer?; xoff : uint64; y : MetalBuffer?; yoff : uint64; var ka : KqMvArgs; rows, gcols : int64) : void>

//! a format's small-batch builders - the two-column, four-column tiled and eight-column staged forms (the last two over
//! gcols column groups), each in its f32 stamp and its half-x stamp (the f16 twin bound at the x slot); k4's half route is
//! its wide forms, its h stamps unset
struct private KqMvbForms {
    b2, b2h : KqMvbBuilder
    b4, b4h, b8, b8h : KqMvbBuilder4
}

//! the builders by format; `[init]` fills it, since a serialized exe restores a global as data and a function value
//! written by a declaration initializer arrives null
var private KQ_MVB_FORMS : table<KqFmt; KqMvbForms>

def private kq_mvb_forms_build : table<KqFmt; KqMvbForms> {
    return <- {
    KqFmt.k4 => KqMvbForms(b2 = @@enc_kq_mvb2_k4_c, b4 = @@enc_kq_mvb4_k4_c, b8 = @@enc_kq_mvb8_k4_c),
    KqFmt.k5 => KqMvbForms(b2 = @@enc_kq_mvb2_k5_c, b2h = @@enc_kq_mvb2h_k5_c, b4 = @@enc_kq_mvb4_k5_c, b4h = @@enc_kq_mvb4h_k5_c,
        b8 = @@enc_kq_mvb8_k5_c, b8h = @@enc_kq_mvb8h_k5_c),
    KqFmt.k6 => KqMvbForms(b2 = @@enc_kq_mvb2_k6_c, b2h = @@enc_kq_mvb2h_k6_c, b4 = @@enc_kq_mvb4_k6_c, b4h = @@enc_kq_mvb4h_k6_c,
        b8 = @@enc_kq_mvb8_k6_c, b8h = @@enc_kq_mvb8h_k6_c),
    KqFmt.iq4xs => KqMvbForms(b2 = @@enc_kq_mvb2_iq4xs_c, b2h = @@enc_kq_mvb2h_iq4xs_c, b4 = @@enc_kq_mvb4_iq4xs_c, b4h = @@enc_kq_mvb4h_iq4xs_c,
        b8 = @@enc_kq_mvb8_iq4xs_c, b8h = @@enc_kq_mvb8h_iq4xs_c),
    KqFmt.iq3s4 => KqMvbForms(b2 = @@enc_kq_mvb2_iq3s4_c, b4 = @@enc_kq_mvb4_iq3s4_c, b8 = @@enc_kq_mvb8_iq3s4_c),
    KqFmt.iq4nl => KqMvbForms(b2 = @@enc_kq_mvb2_iq4nl_c, b2h = @@enc_kq_mvb2h_iq4nl_c, b4 = @@enc_kq_mvb4_iq4nl_c, b4h = @@enc_kq_mvb4h_iq4nl_c,
        b8 = @@enc_kq_mvb8_iq4nl_c, b8h = @@enc_kq_mvb8h_iq4nl_c),
    KqFmt.q40 => KqMvbForms(b2 = @@enc_kq_mvb2_q40_c, b2h = @@enc_kq_mvb2h_q40_c, b4 = @@enc_kq_mvb4_q40_c, b4h = @@enc_kq_mvb4h_q40_c,
        b8 = @@enc_kq_mvb8_q40_c, b8h = @@enc_kq_mvb8h_q40_c),
    KqFmt.nv4 => KqMvbForms(b2 = @@enc_kq_mvb2_nv4_c, b2h = @@enc_kq_mvb2h_nv4_c, b4 = @@enc_kq_mvb4_nv4_c, b4h = @@enc_kq_mvb4h_nv4_c,
        b8 = @@enc_kq_mvb8_nv4_c, b8h = @@enc_kq_mvb8h_nv4_c),
    KqFmt.k3 => KqMvbForms(b2 = @@enc_kq_mvb2_k3_c, b2h = @@enc_kq_mvb2h_k3_c, b4 = @@enc_kq_mvb4_k3_c, b4h = @@enc_kq_mvb4h_k3_c,
        b8 = @@enc_kq_mvb8_k3_c, b8h = @@enc_kq_mvb8h_k3_c),
    KqFmt.iq3s => KqMvbForms(b2 = @@enc_kq_mvb2_iq3s_c, b2h = @@enc_kq_mvb2h_iq3s_c, b4 = @@enc_kq_mvb4_iq3s_c, b4h = @@enc_kq_mvb4h_iq3s_c,
        b8 = @@enc_kq_mvb8_iq3s_c, b8h = @@enc_kq_mvb8h_iq3s_c),
    KqFmt.iq3xxs => KqMvbForms(b2 = @@enc_kq_mvb2_iq3xxs_c, b2h = @@enc_kq_mvb2h_iq3xxs_c, b4 = @@enc_kq_mvb4_iq3xxs_c, b4h = @@enc_kq_mvb4h_iq3xxs_c,
        b8 = @@enc_kq_mvb8_iq3xxs_c, b8h = @@enc_kq_mvb8h_iq3xxs_c),
    KqFmt.k2 => KqMvbForms(b2 = @@enc_kq_mvb2_k2_c, b2h = @@enc_kq_mvb2h_k2_c, b4 = @@enc_kq_mvb4_k2_c, b4h = @@enc_kq_mvb4h_k2_c,
        b8 = @@enc_kq_mvb8_k2_c, b8h = @@enc_kq_mvb8h_k2_c),
    KqFmt.iq2s => KqMvbForms(b2 = @@enc_kq_mvb2_iq2s_c, b2h = @@enc_kq_mvb2h_iq2s_c, b4 = @@enc_kq_mvb4_iq2s_c, b4h = @@enc_kq_mvb4h_iq2s_c,
        b8 = @@enc_kq_mvb8_iq2s_c, b8h = @@enc_kq_mvb8h_iq2s_c),
    KqFmt.iq2xs => KqMvbForms(b2 = @@enc_kq_mvb2_iq2xs_c, b2h = @@enc_kq_mvb2h_iq2xs_c, b4 = @@enc_kq_mvb4_iq2xs_c, b4h = @@enc_kq_mvb4h_iq2xs_c,
        b8 = @@enc_kq_mvb8_iq2xs_c, b8h = @@enc_kq_mvb8h_iq2xs_c),
    KqFmt.iq2xxs => KqMvbForms(b2 = @@enc_kq_mvb2_iq2xxs_c, b2h = @@enc_kq_mvb2h_iq2xxs_c, b4 = @@enc_kq_mvb4_iq2xxs_c, b4h = @@enc_kq_mvb4h_iq2xxs_c,
        b8 = @@enc_kq_mvb8_iq2xxs_c, b8h = @@enc_kq_mvb8h_iq2xxs_c)
    }
}

[init]
def private kq_mvb_forms_boot {
    delete KQ_MVB_FORMS
    KQ_MVB_FORMS <- kq_mvb_forms_build()
}

var private g_kq_mvb_wide = 0l   //! small-batch dispatches past eight rows: one dispatch over every column group

//! the small-batch dispatches that served more than eight rows at once since load - the past-eight route's witness
def metal_kq_mvb_wide_count() : int64 => g_kq_mvb_wide

//! a format's small-batch route, one dispatch at any row count: the eight-column form (ceil(nlive / 8) groups) past four rows
//! where the box crowned it, the two-column form at two (k4's pair tile under its crown), else the four-column tiled form;
//! `half` takes each form's half-x stamp over the eight-row f16 twin bound as x
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#kq-rows-crown")]
def enc_kq_mvb(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
               bs : MetalBuffer?; soff, doff : uint64; bx, by : MetalBuffer?; yoff : uint64;
               var ka : KqMvArgs; rows, nlive : int64; half : bool = false; xoff : uint64 = 0ul) {
    assert(!half || nlive <= 8l, "the f16 twin holds eight rows")
    let forms = unsafe(KQ_MVB_FORMS?[fmt])
    assert(forms != null, "a format with no small-batch forms")
    assert(!half || forms.b2h != null, "a format with no half-x small-batch stamps (k4's half route is its wide forms)")
    let s0off = kq_scale_d_tail(fmt) != 0l ? doff : soff   //! a split-scale plane binds its d tail (the caller's doff) at slot 0, a compact one its scale plane twice
    if (nlive > 8l) {
        g_kq_mvb_wide++
    }
    if (nlive >= 5l && kq_b8_on()) {
        invoke(half ? forms.b8h : forms.b8, enc, bs, s0off, bs, soff, bq, qoff, bx, xoff, by, yoff, ka, rows, (nlive + 7l) / 8l)
    } elif (nlive == 2l) {
        if (fmt == KqFmt.k4 && !half && g_kq_k4_r2_crowned) {
            enc_kq_mvb2_k4r2_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff, by, yoff, ka, rows)
        } else {
            invoke(half ? forms.b2h : forms.b2, enc, bs, s0off, bs, soff, bq, qoff, bx, xoff, by, yoff, ka, rows)
        }
    } else {
        invoke(half ? forms.b4h : forms.b4, enc, bs, s0off, bs, soff, bq, qoff, bx, xoff, by, yoff, ka, rows, (nlive + 3l) / 4l)
    }
}

typedef KqMmBuilder = function<(enc : MetalComputeEncoder?; s0 : MetalBuffer?; s0off : uint64; s : MetalBuffer?; soff : uint64;
                                q : MetalBuffer?; qoff : uint64; x, y : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows : int64) : void>
typedef KqMmSkBuilder = function<(enc : MetalComputeEncoder?; s0 : MetalBuffer?; s0off : uint64; s : MetalBuffer?; soff : uint64;
                                  q : MetalBuffer?; qoff : uint64; x, y : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows, nsk : int64) : void>
typedef B32MmBuilder = function<(enc : MetalComputeEncoder?; s : MetalBuffer?; soff : uint64; q : MetalBuffer?; qoff : uint64;
                                 x, y : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows : int64) : void>
typedef B32MmSkBuilder = function<(enc : MetalComputeEncoder?; s : MetalBuffer?; soff : uint64; q : MetalBuffer?; qoff : uint64;
                                   x, y : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows, nsk : int64) : void>

//! a superblock format's simdgroup mul_mm tile and its split-K stamp (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`)
struct private KqMmForms {
    mm : KqMmBuilder
    sk : KqMmSkBuilder
}

//! a per-32 format's mul_mm tile and its split-K stamp, bound off the format's own planes (`b32_planes_of`)
struct private B32MmForms {
    mm : B32MmBuilder
    sk : B32MmSkBuilder
}

//! the base mul_mm builders by format; `[init]` fills them, since a serialized exe restores a global as data and a function value
//! written by a declaration initializer arrives null
var private KQ_MM_FORMS : table<KqFmt; KqMmForms>

//! whether `fmt` has a base mul_mm tile (the staged simdgroup form every dense K-quant site falls to): the tower's kq lane serves a member on it
def kq_mm_base_tile_on(fmt : KqFmt) : bool => key_exists(KQ_MM_FORMS, fmt)
var private B32_MM_FORMS : table<KqFmt; B32MmForms>
var private g_base_sk_on = false   //! the base tiles' split-K stamps compiled (`metal_base_split_init`)

def private kq_mm_forms_build : table<KqFmt; KqMmForms> {
    return <- {
    KqFmt.k4 => KqMmForms(mm = @@enc_kq_mm_k4_c, sk = @@enc_kq_mm_k4_sk_c),
    KqFmt.k5 => KqMmForms(mm = @@enc_kq_mm_k5_c, sk = @@enc_kq_mm_k5_sk_c),
    KqFmt.k6 => KqMmForms(mm = @@enc_kq_mm_k6_c, sk = @@enc_kq_mm_k6_sk_c),
    KqFmt.iq4xs => KqMmForms(mm = @@enc_kq_mm_iq4xs_c, sk = @@enc_kq_mm_iq4xs_sk_c),
    KqFmt.iq3s4 => KqMmForms(mm = @@enc_kq_mm_iq3s4_c, sk = @@enc_kq_mm_iq3s4_sk_c),
    KqFmt.iq4nl => KqMmForms(mm = @@enc_kq_mm_iq4nl_c, sk = @@enc_kq_mm_iq4nl_sk_c),
    KqFmt.q40 => KqMmForms(mm = @@enc_kq_mm_q40_c, sk = @@enc_kq_mm_q40_sk_c),
    KqFmt.nv4 => KqMmForms(mm = @@enc_kq_mm_nv4_c, sk = @@enc_kq_mm_nv4_sk_c),
    KqFmt.k3 => KqMmForms(mm = @@enc_kq_mm_k3_c, sk = @@enc_kq_mm_k3_sk_c),
    KqFmt.k2 => KqMmForms(mm = @@enc_kq_mm_k2_c, sk = @@enc_kq_mm_k2_sk_c),
    KqFmt.iq3s => KqMmForms(mm = @@enc_kq_mm_iq3s_c, sk = @@enc_kq_mm_iq3s_sk_c),
    KqFmt.iq3xxs => KqMmForms(mm = @@enc_kq_mm_iq3xxs_c, sk = @@enc_kq_mm_iq3xxs_sk_c),
    KqFmt.iq2s => KqMmForms(mm = @@enc_kq_mm_iq2s_c, sk = @@enc_kq_mm_iq2s_sk_c),
    KqFmt.iq2xs => KqMmForms(mm = @@enc_kq_mm_iq2xs_c, sk = @@enc_kq_mm_iq2xs_sk_c),
    KqFmt.iq2xxs => KqMmForms(mm = @@enc_kq_mm_iq2xxs_c, sk = @@enc_kq_mm_iq2xxs_sk_c)
    }
}

def private b32_mm_forms_build : table<KqFmt; B32MmForms> {
    return <- {
    KqFmt.q51 => B32MmForms(mm = @@enc_kq_mm_q51_c, sk = @@enc_kq_mm_q51_sk_c),
    KqFmt.iq4nl32 => B32MmForms(mm = @@enc_kq_mm_iq4nl32_c, sk = @@enc_kq_mm_iq4nl32_sk_c),
    KqFmt.q4032 => B32MmForms(mm = @@enc_kq_mm_q4032_c, sk = @@enc_kq_mm_q4032_sk_c),
    KqFmt.q5032 => B32MmForms(mm = @@enc_kq_mm_q5032_c, sk = @@enc_kq_mm_q5032_sk_c)
    }
}

[init]
def private kq_mm_forms_boot {
    delete KQ_MM_FORMS
    KQ_MM_FORMS <- kq_mm_forms_build()
    delete B32_MM_FORMS
    B32_MM_FORMS <- b32_mm_forms_build()
}

//! the batched kq GEMM (B >= 9): the format's simdgroup mul_mm tile over the padded x panel - enc_gemm_mm_b's shape (grid
//! mp/32 x rows/64, y bound at the site's column byte offset); a split-scale plane binds its d tail (`doff`) at slot 0
def enc_kq_gemm_mm_b(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
                     bs : MetalBuffer?; soff, doff : uint64; bx, by : MetalBuffer?; yoff : uint64;
                     bk, bn : MetalBuffer?; mp, rows : int64) {
    let forms = unsafe(KQ_MM_FORMS?[fmt])
    assert(forms != null, "a format with no base mul_mm tile")
    invoke(forms.mm, enc, bs, kq_scale_d_tail(fmt) != 0l ? doff : soff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows)
}

//! The base tile's split-K stamp of a superblock format, `nsk` partial planes into `by`; false where the base split stamps are not
//! compiled (`metal_base_split_init`)
def enc_kq_gemm_mm_sk(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64;
                      bs : MetalBuffer?; soff, doff : uint64; bx, by : MetalBuffer?; yoff : uint64;
                      bk, bn : MetalBuffer?; mp, rows, nsk : int64) : bool {
    let forms = unsafe(KQ_MM_FORMS?[fmt])
    return false if (!g_base_sk_on || forms == null)
    invoke(forms.sk, enc, bs, kq_scale_d_tail(fmt) != 0l ? doff : soff, bs, soff, bq, qoff, bx, by, yoff, bk, bn, mp, rows, nsk)
    return true
}

//! the per-32 planes' split-K stamp, as `enc_b32_gemm_mm` binds them; false where the base split stamps are not compiled
def enc_b32_gemm_mm_sk(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff : int64; bx, by : MetalBuffer?; yoff : uint64;
                       bk, bn : MetalBuffer?; mp, rows, nsk : int64) : bool {
    let forms = unsafe(B32_MM_FORMS?[fmt])
    return false if (!g_base_sk_on || forms == null)
    let qp = b32_planes_of(g_dev, t, fmt, woff)
    invoke(forms.sk, enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bx, by, yoff, bk, bn, mp, rows, nsk)
    return true
}

//! a dense per-32 plane (q51, iq4nl32, q4032, q5032) over `mp` padded rows through the simdgroup mul_mm tile - its one GEMM form
def enc_b32_gemm_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff : int64; bx, by : MetalBuffer?; yoff : uint64;
                    bk, bn : MetalBuffer?; mp, rows : int64) {
    let forms = unsafe(B32_MM_FORMS?[fmt])
    assert(forms != null, "a format with no per-32 mul_mm tile")
    let qp = b32_planes_of(g_dev, t, fmt, woff)
    invoke(forms.mm, enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bx, by, yoff, bk, bn, mp, rows)
}

//! The base tiles' split-K stamps (`MetalQ8MulMmSk`, `MetalKqMulMm<Fmt>Sk`), compiled under their knees - the q8 blob's
//! (`metal_split_k_tgs_base`) and the K-quant and per-32 ones' (`metal_split_k_tgs_kq_base`) - unless `DASLLAMA_METAL_SPLIT_K=0`
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def metal_base_split_init(var ok : bool&) {
    return if (!g_env_metal.split_k)
    if (metal_split_k_tgs_base() > 0l && g_pso_mm_sk == null) {
        g_pso_mm_sk = compile_stamp(MetalQ8MulMmSk_metal_q8_mulmm_msl, ok)
    }
    if (metal_split_k_tgs_kq_base() > 0l && !g_base_sk_on) {
        g_pso_kq_mm_b4_sk = compile_stamp(MetalKqMulMmK4Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b5_sk = compile_stamp(MetalKqMulMmK5Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b6_sk = compile_stamp(MetalKqMulMmK6Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b44_sk = compile_stamp(MetalKqMulMmIq4xsSk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b35_sk = compile_stamp(MetalKqMulMmIq3s4Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b45_sk = compile_stamp(MetalKqMulMmIq4nlSk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b40_sk = compile_stamp(MetalKqMulMmQ40Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b49_sk = compile_stamp(MetalKqMulMmNv4Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b3_sk = compile_stamp(MetalKqMulMmK3Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b2_sk = compile_stamp(MetalKqMulMmK2Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b33_sk = compile_stamp(MetalKqMulMmIq3sSk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b34_sk = compile_stamp(MetalKqMulMmIq3xxsSk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b23_sk = compile_stamp(MetalKqMulMmIq2sSk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b24_sk = compile_stamp(MetalKqMulMmIq2xsSk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b25_sk = compile_stamp(MetalKqMulMmIq2xxsSk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b51_sk = compile_stamp(MetalKqMulMmQ51Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b4n32_sk = compile_stamp(MetalKqMulMmIq4nl32Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b4032_sk = compile_stamp(MetalKqMulMmQ4032Sk_metal_kq_mulmm_msl, ok)
        g_pso_kq_mm_b5032_sk = compile_stamp(MetalKqMulMmQ5032Sk_metal_kq_mulmm_msl, ok)
        g_base_sk_on = ok
    }
}

def private metal_base_split_release {
    release_handles(g_pso_mm_sk, g_pso_kq_mm_b4_sk, g_pso_kq_mm_b5_sk, g_pso_kq_mm_b6_sk, g_pso_kq_mm_b44_sk, g_pso_kq_mm_b35_sk)
    release_handles(g_pso_kq_mm_b45_sk, g_pso_kq_mm_b40_sk, g_pso_kq_mm_b49_sk, g_pso_kq_mm_b3_sk, g_pso_kq_mm_b2_sk, g_pso_kq_mm_b33_sk, g_pso_kq_mm_b34_sk)
    release_handles(g_pso_kq_mm_b23_sk, g_pso_kq_mm_b24_sk, g_pso_kq_mm_b25_sk, g_pso_kq_mm_b51_sk, g_pso_kq_mm_b4n32_sk, g_pso_kq_mm_b4032_sk, g_pso_kq_mm_b5032_sk)
    g_base_sk_on = false
}

// One weight site's GEMV — a q8-tagged tensor rides the planar q8 kernels, a kq tensor the
// plane GEMVs (kq_quants_of/kq_scales_of + enc_kq_gemv). yoff places the k/v/w3 output segments.
def enc_site_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64; bx, by, bn, bd : MetalBuffer?; yoff : uint64 = 0ul) {
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, woff)
        enc_q8_site_gemv(enc, bw.buf, bw.boff, bx, by, bn, bd, rows, n, yoff)
    } elif (kq_dense_b32(fmt)) {
        enc_b32_gemv(enc, t, fmt, woff, rows, n, bx, by, bn, bd, yoff, 0ul)
    } else {
        let bq = kq_quants_of(g_dev, t, fmt, woff)
        let bs = kq_scales_of(g_dev, t, fmt, woff)
        enc_kq_gemv(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, bn, bd, rows, n, yoff)
    }
}

var private g_kq_rows_crowned : bool[KQ_FMT_COUNT]
var private g_kq_k4_r2_crowned : bool
var private g_kq_half_crowned : bool[KQ_FMT_COUNT]   //! per format: the box crowned its half-x forms (`kq_half_crown(fmt)`)
var private g_kq_half_env = true           //! `DASLLAMA_METAL_KQ_HALF` and its setter - 0 keeps the f32 forms under every crown (the A/B rail)
var private g_kq_half_set = false          //! the setter spoke: a decode re-init keeps its value instead of reading the environment again

let KQ_HALF_CROWN_PREFIX = "kq_rows_half_"

//! the half-x crown of one format: `kq_rows_half_<fmt>`
def kq_half_crown(fmt : KqFmt) : string => "{KQ_HALF_CROWN_PREFIX}{fmt}"

//! the half-x forms serve this format's rows sites on the box: its crown is minted and the rail is on
def metal_kq_half_on(fmt : KqFmt) : bool => g_kq_half_env && g_kq_half_crowned[int(fmt)]

//! the rows verdict the process holds for a format: its `kq_rows_<fmt>` crown, as the sidecar or the race left it
def metal_kq_rows_crowned(fmt : KqFmt) : bool => g_kq_rows_crowned[int(fmt)]

//! some format's half-x forms are on - the row producers store the f16 twin beside x
def metal_kq_half_any : bool {
    return false if (!g_kq_half_env)
    for (c in g_kq_half_crowned) {
        return true if (c)
    }
    return false
}

//! A/B seat: the half-x forms under their crowns vs the f32 forms - `DASLLAMA_METAL_KQ_HALF`'s setter twin. Default on.
def public set_metal_kq_half(v : bool) {
    g_kq_half_set = true
    if (v != g_kq_half_env) {
        to_log(LOG_WARNING, "dasLLAMA metal decode: set_metal_kq_half({v}) - the K-quant rows sites take the {v ? "half-x forms under their crowns" : "f32 forms under every half-x crown"} (numerics move with the form)\n")
    }
    g_kq_half_env = v
}

def public get_metal_kq_half : bool => g_kq_half_env

//! B >= 9 over a panel padded to the mul_mm's 32-row tile takes the kq mul_mm twin; any other row count
//! (the verify's nine-row panel) rides the f32 rows route, one small-batch dispatch over every row;
//! bxh = the f16 twin of x at the same row stride, where the producer wrote one (null = none)
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#kq-rows-crown")]
def enc_kq_site_b(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64;
                  bx : MetalBuffer?; xstride : int64; by : MetalBuffer?; ystride, ycol : int64;
                  bn, bd, bys : MetalBuffer?; nrows, mp : int64; bxh : MetalBuffer? = null; xrow : int64 = 0l) {
    //! the per-32 planes' forms: the mul_mm tile past eight rows, the
    //! small-batch form at two or more, the GEMV at one; `xrow` starts the rows forms' x at that row (a prefill remainder)
    let xoff = uint64(xrow * xstride * 4l)
    if (kq_dense_b32(fmt)) {
        if (nrows > 8l && mp % 32l == 0l && rows % 64l == 0l) {
            enc_b32_gemm_mm(enc, t, fmt, woff, bx, by, uint64(ycol * 4l), bn, bys, mp, rows)
        } elif (nrows >= 2l) {
            assert(xstride == n)
            enc_b32_mvb(enc, t, fmt, woff, rows, n, bx, by, uint64(ycol * 4l), bn, bd, ystride, nrows, xoff)
        } else {
            enc_b32_gemv(enc, t, fmt, woff, rows, n, bx, by, bn, bd, uint64(ycol * 4l), xoff)
        }
        return
    }
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    if (nrows > 8l) {
        if (mp % 32l == 0l && rows % 64l == 0l) {   //! the mul_mm grid is rows/64: a narrower plane (deltanet beta/alpha) takes the row forms
            enc_kq_gemm_mm_b(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, uint64(ycol * 4l), bn, bys, mp, rows)
            return
        }
    } elif (nrows >= 2l && bxh != null && metal_kq_half_on(fmt)) {
        assert(xstride == n)
        enc_kq_rows_half(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bxh, by, uint64(ycol * 4l), rows, n, ystride, nrows, uint64(xrow * n * 2l))
        return
    }
    enc_kq_rows_f32(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, uint64(ycol * 4l), bn, bd, rows, n, xstride, ystride, nrows, xoff)
}

//! the f32-x rows route of a format at any row count - the crowns' incumbent: B single passes where
//! the box crowned them (and at one row), the k4 pair walk under the tile crown, else the small-batch twin
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#kq-rows-crown")]
def enc_kq_rows_f32(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64; bs : MetalBuffer?; soff, doff : uint64;
                    bx, by : MetalBuffer?; yoff : uint64; bn, bd : MetalBuffer?; rows, n, xstride, ystride, nrows : int64; xoff : uint64 = 0ul) {
    if (nrows >= 2l && !g_kq_rows_crowned[int(fmt)]) {
        assert(xstride == n)
        if (nrows > 2l && fmt == KqFmt.k4 && g_kq_k4_r2_crowned) {
            enc_kq_mvb_k4_pairs(enc, bq, qoff, bs, soff, doff, bx, by, yoff, bn, bd, rows, n, ystride, nrows, xoff)
            return
        }
        var ka = KqMvArgs(ndim = uint(n), ddim = uint(rows), ys = uint(ystride), nr = uint(nrows))
        enc_kq_mvb(enc, fmt, bq, qoff, bs, soff, doff, bx, by, yoff, ka, rows, nrows, false, xoff)
        return
    }
    for (b in range64(nrows)) {
        enc_kq_gemv(enc, fmt, bq, qoff, bs, soff, doff, bx, by, bn, bd, rows, n,
            yoff + uint64(b * ystride * 4l), xoff + uint64(b * xstride * 4l))
    }
}

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#kq-rows-crown")]
def enc_kq_mvb_k4_pairs(enc : MetalComputeEncoder?; bq : MetalBuffer?; qoff : uint64; bs : MetalBuffer?; soff, doff : uint64;
                        bx, by : MetalBuffer?; yoff : uint64; bn, bd : MetalBuffer?; rows, n, ystride, nlive : int64; xoff : uint64 = 0ul) {
    var ka = KqMvArgs(ndim = uint(n), ddim = uint(rows), ys = uint(ystride), nr = 2u)
    var b = 0l
    while (b + 1l < nlive) {
        enc_kq_mvb2_k4r2_c(enc, bs, soff, bs, soff, bq, qoff, bx, xoff + uint64(b * n * 4l), by, yoff + uint64(b * ystride * 4l), ka, rows)
        b += 2l
    }
    if (b < nlive) {
        enc_kq_gemv(enc, KqFmt.k4, bq, qoff, bs, soff, doff, bx, by, bn, bd, rows, n, yoff + uint64(b * ystride * 4l), xoff + uint64(b * n * 4l))
    }
}

//! a format's half-x route over nlive (2..8) rows of the f16 twin bxh (row stride n): k4 rides its wide forms (two
//! live rows the two-column, three or four the four-column, five to eight the eight-column), every other format its
//! small-batch half stamps; a form reads every one of its columns, so the twin holds eight rows of n halves whatever nlive
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#kq-rows-crown")]
def enc_kq_rows_half(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64; bs : MetalBuffer?; soff, doff : uint64;
                     bxh, by : MetalBuffer?; yoff : uint64; rows, n, ystride, nlive : int64; xoff : uint64 = 0ul) {
    var ka = KqMvArgs(ndim = uint(n), ddim = uint(rows), ys = uint(ystride), nr = uint(nlive))
    if (fmt != KqFmt.k4) {
        enc_kq_mvb(enc, fmt, bq, qoff, bs, soff, doff, bxh, by, yoff, ka, rows, nlive, true, xoff)
    } elif (nlive <= 2l) {
        enc_kq_mvw2h_k4_c(enc, bs, soff, bs, soff, bq, qoff, bxh, xoff, by, yoff, ka, rows)
    } elif (nlive <= 4l) {
        enc_kq_mvw4h_k4_c(enc, bs, soff, bs, soff, bq, qoff, bxh, xoff, by, yoff, ka, rows)
    } else {
        enc_kq_mvw8h_k4_c(enc, bs, soff, bs, soff, bq, qoff, bxh, xoff, by, yoff, ka, rows)
    }
}

//! toff = the position's row in the rope tables (0 where the caller binds the row's own buffer); vsoff = v's byte offset in the one QKV buffer.
def enc_rope_store(enc : MetalComputeEncoder?; f16 : bool; bqkv : MetalBuffer?; vsoff : uint64;
                   kmb, vmb : MetalBuffer?; koff, voff : uint64;
                   bcos, bsin : MetalBuffer?; toff : uint64; bbias : MetalBuffer?; var ka : GkRopeKvArgs; npairs : int64) {
    if (f16) {
        enc_rpst16_c(enc, bqkv, bqkv, bqkv, vsoff, kmb, koff, vmb, voff, bbias, bcos, toff, bsin, ka, npairs)
    } else {
        enc_rpst32_c(enc, bqkv, bqkv, bqkv, vsoff, kmb, koff, vmb, voff, bbias, bcos, toff, bsin, ka, npairs)
    }
}

//! the fused q/k norm, rope and K/V store at one row (`GkQknRopeKv`): `nwg` head threadgroups over the [q | k | v] row of bqkv,
//! the mirror K and V rows bound at koff / voff, the position's cos and sin rows at toff
def enc_qkn_rope(enc : MetalComputeEncoder?; f16 : bool; bqkv, kmb, vmb : MetalBuffer?; koff, voff : uint64;
                 bcos, bsin : MetalBuffer?; toff : uint64; bqkw, bbias : MetalBuffer?; var ka : GkQknRopeArgs; nwg : int64; bq : MetalBuffer? = null) {
    let bqr = bq != null ? bq : bqkv
    if (f16) {
        enc_qkn_rope16(enc, bqr, bqkv, kmb, koff, vmb, voff, bcos, toff, bqkw, bsin, toff, bbias, bqkv, ka, nwg, int64(ka.hs / 2u))
    } else {
        enc_qkn_rope32(enc, bqr, bqkv, kmb, koff, vmb, voff, bcos, toff, bqkw, bsin, toff, bbias, bqkv, ka, nwg, int64(ka.hs / 2u))
    }
}

//! ... and its rows form over the route table at rtoff, the mirror planes bound whole
def enc_qkn_rope_b(enc : MetalComputeEncoder?; f16 : bool; bqkv, bcos, bsin, brt, bqkw, bbias : MetalBuffer?; var ka : GkQknRopeArgs;
                   nwg, nrows : int64; rtoff : uint64 = 0ul; bq : MetalBuffer? = null) {
    let bqr = bq != null ? bq : bqkv
    if (f16) {
        enc_qkn_rope_b16(enc, bqr, bqkv, g_arena, 0ul, g_arena, 0ul, bcos, 0ul, bqkw, bsin, 0ul, bbias, brt, rtoff, bqkv, ka, nwg, nrows, int64(ka.hs / 2u))
    } else {
        enc_qkn_rope_b32(enc, bqr, bqkv, g_arena, 0ul, g_arena, 0ul, bcos, 0ul, bqkw, bsin, 0ul, bbias, brt, rtoff, bqkv, ka, nwg, nrows, int64(ka.hs / 2u))
    }
}

// The tq4 rotation signs, uploaded once at max head size (smaller heads read a prefix — the
// deterministic-seed contract with the CPU codec).

def enc_rope_store_tq4(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                       kmb, vmb : MetalBuffer?; koff, voff : uint64;
                       bcos, bsin, bbias : MetalBuffer?; var ka : RopeStoreTq4Args; ntg, head_size : int64; bq : MetalBuffer? = null) {
    enc_rpst_tq4_c(enc, bqkv, bqkv, vsoff, kmb, koff, kmb, vmb, voff, vmb, bcos, bsin, signs_buffer(),
        bbias, ka, bq != null ? bq : bqkv, ntg, head_size)
}

// the tq4 attention-output un-rotate: one threadgroup per (head, row), row stride = qstride
def enc_fwht_unsign(enc : MetalComputeEncoder?; byo, bhs, bqs : MetalBuffer?; nheads, nrows, head_size : int64; bqg : MetalBuffer? = null;
                    bgstride : MetalBuffer? = null) {
    enc_unsign_c(enc, byo, signs_buffer(), bhs, bqs, bqg, bgstride, nheads, nrows, head_size)
}

// q8_0-mirror rope+store: the K/V mirror rows bind TWICE each (half-scale + byte views at the
// same row offset); grid covers the q pairs, then a lane an element of the K and V rows (q8_store_threads)
//! the q8_0 store's threads: a thread a q pair, then - from the next simdgroup boundary - a lane an element of the K and V rows
def q8_store_threads(qd, kv_dim : int64) : int64 => (qd / 2l + 31l) / 32l * 32l + 2l * kv_dim

def enc_rope_store_q8(enc : MetalComputeEncoder?; bqkv : MetalBuffer?; vsoff : uint64;
                      kmb, vmb : MetalBuffer?; koff, voff : uint64;
                      bcos, bsin, bbias : MetalBuffer?; var ka : RopeStoreQ8Args; nthreads : int64; bq : MetalBuffer? = null) {
    enc_rpst_q8_c(enc, bqkv, bqkv, vsoff, kmb, koff, kmb, vmb, voff, vmb, bcos, bsin,
        bbias, ka, bq != null ? bq : bqkv, nthreads)
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

// the fused W1|W3 + silu/gelu dispatch (gate + up rows per output element; bact picks the activation)
def enc_gemv_w13sw(enc : MetalComputeEncoder?; bw : MetalBuffer?; w1off, w3off : uint64; bx, bh : MetalBuffer?; bn, bhid : MetalBuffer?; n, hidden : int64; bact : MetalBuffer?) {
    if (n <= GEMV_SG_MAX_N) {
        enc_w13sw_sg_c(enc, bw, w1off, bw, bx, bh, bn, bhid, bw, w3off, bw, bact, hidden)
    } else {
        enc_w13sw_c(enc, bw, w1off, bw, bx, bh, bn, bhid, bw, w3off, bw, bact, hidden)
    }
}

def enc_embed(enc : MetalComputeEncoder?; bw : MetalBuffer?; eoff : uint64; btok, bx, bdim, bescale : MetalBuffer?; dim : int64; toff : uint64 = 0ul; xoff : uint64 = 0ul; nrows : int64 = 1l) {
    enc_embed_c(enc, bw, eoff, bw, btok, toff, bx, xoff, bdim, bescale != null ? bescale : g_one, dim, nrows)
}

//! A blob model's token table serves its rows to the device gather: the q8 linear copy, the fp32 table, or its own
//! K-quant plane in a format the gather has a stamp for.
def emb_dev_gather_ok(t : Model) : bool {
    return false if (!t.metal_blob || t.planes_trimmed)
    return t.emb_q8 || (emb_kq(t) ? emb_kq_gather_fmt(t.emb_fmt) : t.tok_emb_off >= 0l)
}

//! the gather reads its token ids at any offset of a token list: the q8 copy's and the K-quant plane's stamps (the fp32 table's reads from 0)
def emb_dev_gather_at_ok(t : Model) : bool => emb_dev_gather_ok(t) && (t.emb_q8 || emb_kq(t))

//! The rows of `nrows` token ids (btok at toff) into bx at xoff, row stride dim, off the token table under the model's
//! embed scale (bescale: its uploaded scalar, null = 1) - the device's embed_row. The fp32 table's gather reads its ids from 0.
def enc_embed_dev(enc : MetalComputeEncoder?; t : Model; btok : MetalBuffer?; toff : uint64; bx : MetalBuffer?; xoff : uint64; nrows : int64;
                  bdim, bescale : MetalBuffer?) {
    let dim = t.config.dim
    if (t.emb_q8) {
        enc_embed_table(enc, t, KqFmt.q8, t.emb_q8_off, dim, t.config.embed_scale, btok, toff, bx, xoff, nrows, bdim, bescale)
    } elif (emb_kq(t)) {
        enc_embed_table(enc, t, t.emb_fmt, emb_kq_plane_off(t), dim, t.config.embed_scale, btok, toff, bx, xoff, nrows, bdim, bescale)
    } else {
        assert(toff == 0ul, "the fp32 table's gather reads its token ids from the buffer's start")
        let bemb = plane_of(g_dev, t, t.fblob, t.tok_emb_off)
        var kg = GkRowGatherArgs(c = uint(dim), rows = uint(nrows), aoff = uint(bemb.boff / 4ul), boff = 0u, coff = 0u, out_off = uint(xoff / 4ul),
            add_b = 0u, add_c = 0u, scale = t.config.embed_scale)
        enc_row_gather(enc, bemb.buf, btok, g_one, g_one, bx, kg, nrows * dim)
    }
}

//! the per-32 blocks of format `fmt` that span a whole number of 16-byte steps in both its quant plane and its scale plane:
//! a bind every that many blocks sits on the 16-byte grid in both
def b32_bind_blocks(fmt : KqFmt) : int64 {
    var k = 1l
    while ((k * kq_qsb(fmt)) % 16l != 0l || (k * kq_ssb(fmt)) % 16l != 0l) {
        k++
    }
    return k
}

//! the blocks a per-32 table of format `fmt` at element `off` sits past the 16-byte bind below it (`b32_bind_blocks`): a table
//! off that grid binds there and its gather skips these
def b32_bind_rem(off : int64; fmt : KqFmt) : int64 => (off / KQ_B32_ELEMS) % b32_bind_blocks(fmt)

//! where the per-32 gather binds a table of format `fmt` at element `off`: the element on the 16-byte grid at or below it, and
//! the blocks the table sits past it (`EmbedB32Args.b0`)
def b32_table_bind(off : int64; fmt : KqFmt) : tuple<off : int64; b0 : int64> {
    let rem = b32_bind_rem(off, fmt)
    return (off = off - rem * KQ_B32_ELEMS, b0 = rem)
}

//! The rows of `nrows` token ids (btok at toff) off the table plane `fmt` at element `off`, `width` elements a row under `escale`, into
//! bx at xoff: the token table's gather and the E-series per-layer table's (layers x ple under sqrt(ple)); the q8 copy's stamp
//! reads the width and the scale off the uniforms `bwidth` / `bescale` (null = 1)
def enc_embed_table(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; off, width : int64; escale : float; btok : MetalBuffer?; toff : uint64;
                    bx : MetalBuffer?; xoff : uint64; nrows : int64; bwidth, bescale : MetalBuffer?) {
    if (fmt == KqFmt.q8) {
        let bemb = blob_of(g_dev, t, off)
        enc_embed(enc, bemb.buf, bemb.boff, btok, bx, bwidth, bescale, width, toff, xoff, nrows)
    } elif (kq_dense_b32(fmt)) {
        let bind = b32_table_bind(off, fmt)
        let qp = b32_planes_of(g_dev, t, fmt, bind.off)
        var ka = EmbedB32Args(dim = uint(width), escale = escale, b0 = uint(bind.b0))
        let units = width / 16l
        let slot = int(fmt)
        kq_tile_stamp(slot, "!b32", "embed") <| $() {   //! the per-32 table gather (`MetalEmbedB32T`) stamps the per-32 formats alone
            tile_enc_b32(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, btok, toff, bx, xoff, ka, units, nrows)
        }
    } else {
        let bq = kq_quants_of(g_dev, t, fmt, off)
        let bs = kq_scales_of(g_dev, t, fmt, off)
        enc_embed_kq_rows(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, btok, toff, bx, xoff, width, nrows, escale)
    }
}

//! the K-quant table rows of `nrows` token ids (btok at toff) into bx at xoff, row stride dim: the scale buffer binds as the
//! d halves (the compact block at soff, the split plane's d tail at doff) and as the sub-scale strips, the quant plane once
def enc_embed_kq_rows(enc : MetalComputeEncoder?; fmt : KqFmt; bq : MetalBuffer?; qoff : uint64; bs : MetalBuffer?; soff, doff : uint64;
                      btok : MetalBuffer?; toff : uint64; bx : MetalBuffer?; xoff : uint64; dim, nrows : int64; escale : float) {
    let d0 = kq_scale_d_tail(fmt) == 0l ? soff : doff
    var ka = EmbedKqArgs(dim = uint(dim), nsb = uint(dim / KQ_SB_ELEMS), escale = escale)
    let units = dim / 16l
    if (fmt == KqFmt.k4) {
        enc_embed_kq_k4(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.k5) {
        enc_embed_kq_k5(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.k6) {
        enc_embed_kq_k6(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.iq4xs) {
        enc_embed_kq_iq4xs(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.iq3s4) {
        enc_embed_kq_iq3s4(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.iq4nl) {
        enc_embed_kq_iq4nl(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.q40) {
        enc_embed_kq_q40(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.nv4) {
        enc_embed_kq_nv4(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.k3) {
        enc_embed_kq_k3(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.k2) {
        enc_embed_kq_k2(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.iq3s) {
        enc_embed_kq_iq3s(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.iq3xxs) {
        enc_embed_kq_iq3xxs(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.iq2s) {
        enc_embed_kq_iq2s(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.iq2xs) {
        enc_embed_kq_iq2xs(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } elif (fmt == KqFmt.iq2xxs) {
        enc_embed_kq_iq2xxs(enc, bs, d0, bs, soff, bq, qoff, btok, toff, bx, xoff, ka, units, nrows)
    } else {
        panic("dasLLAMA metal: no token-table gather stamp for {fmt} (emb_kq_gather_fmt admits the formats above)")
    }
}

// ===== MoE routing encoders (Wave C) =====

// enc_moe_router / enc_moe_select / enc_moe_combine are [metal_dispatch]-generated from their
// prefill classes (the selection buffer's idx/w views are the seli/selw fields — callers pass
// the same buffer twice)

//! a per-32 expert stack's routed GEMV (q51, iq4nl32, q4032, q5032) over its own plane pair
def private enc_moe_gemv_b32(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, k, nst : int64;
                             bx, by, bsel : MetalBuffer?; var ka : MoeGemvArgs; yoff : uint64) {
    let qp = b32_planes_of(g_dev, t, fmt, stack_off)
    if (fmt == KqFmt.q51) {
        enc_moe_gemv_q51_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, by, yoff, ka, bsel, rows, k, nst)
    } elif (fmt == KqFmt.iq4nl32) {
        enc_moe_gemv_iq4nl32_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, by, yoff, ka, bsel, rows, k, nst)
    } elif (fmt == KqFmt.q4032) {
        enc_moe_gemv_q4032_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, by, yoff, ka, bsel, rows, k, nst)
    } else {
        enc_moe_gemv_q5032_c(enc, qp.qbuf, qp.qoff, qp.sbuf, qp.soff, bx, by, yoff, ka, bsel, rows, k, nst)
    }
}

// one expert-indexed weight site (gate/up/down): weight binds at the layer's expert STACK base,
// grid.y = k slots, grid.z = streams; bes = blocks-per-expert (q8 ege/32, kq ege/256), bxs =
// per-slot x stride, bss/bbxs/bbys = per-stream sel/x/y strides (single decode binds zeros)
def enc_moe_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, k, nst : int64;
                 bx, by, bsel : MetalBuffer?; var ka : MoeGemvArgs; yoff : uint64 = 0ul) {
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, stack_off)
        enc_moe_gemv_q8_c(enc, bw.buf, bw.boff, bw.buf, bx, by, yoff, ka, bsel, rows, k, nst)
    } elif (kq_dense_b32(fmt)) {
        enc_moe_gemv_b32(enc, t, fmt, stack_off, rows, k, nst, bx, by, bsel, ka, yoff)
    } else {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        if (fmt == KqFmt.k6) {
            enc_moe_gemv_k6_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.k4) {
            enc_moe_gemv_k4_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.k5) {
            enc_moe_gemv_k5_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.k3) {
            enc_moe_gemv_k3_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.iq3s) {
            if (g_pso_moe_kq_iq3s_f4 != null) {
                enc_moe_gemv_iq3s_f4_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
            } else {
                enc_moe_gemv_iq3s_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
            }
        } elif (fmt == KqFmt.iq3xxs) {
            enc_moe_gemv_iq3xxs_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.iq2s) {
            enc_moe_gemv_iq2s_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.iq2xs) {
            enc_moe_gemv_iq2xs_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.iq2xxs) {
            enc_moe_gemv_iq2xxs_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.iq4xs) {
            enc_moe_gemv_iq4xs_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.iq3s4) {
            enc_moe_gemv_iq3s4_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.q40) {
            enc_moe_gemv_q40_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.nv4) {
            enc_moe_gemv_nv4_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.iq4nl) {
            enc_moe_gemv_iq4nl_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } elif (fmt == KqFmt.k2) {
            enc_moe_gemv_k2_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, ka, bsel, rows, k, nst)
        } else {
            panic("metal moe decode: expert plane format {fmt} has no Metal expert kernel - moe_metal_ok admitted it (moe_fmt_metal_served)")
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

//! layer `l`'s per-expert bias rows of an mx4 site - `n` floats an expert at `off` in the fblob - where the model carries
//! expert biases; a model without them binds the stand-in, which the kernels never read (their `hasb` is 0)
def mx4_bias_rows(t : Model; off, l, n : int64) : MetalBuffer? {
    return g_one if (!t.config.moe_exps_bias)
    return upload_region(unsafe(addr<void?>(t.fblob[off + l * t.config.n_expert * n])), uint64(t.config.n_expert * n * 4l))
}

//! one routed site of layer `l` over `nrows` rows' selections in `bsel`: the stack's plane by its format, an mx4 stack with its
//! bias rows at `boff`; `ka` is the site's shape, its plane stride set here from the stack's format
def private enc_moe_site(enc : MetalComputeEncoder?; t : Model; l, stack_off, boff, rows, nrows : int64; fmt : KqFmt;
                         bx, by, bsel : MetalBuffer?; ka : MoeGemvArgs; yoff : uint64) {
    var a = ka
    let ege = uint(t.config.dim * t.config.n_ff_exp)
    a.eplane = ege / (!t.experts_mx4 && kq_sb(fmt) ? 256u : 32u)
    if (t.experts_mx4) {
        enc_moe_gemv_mx4(enc, t, stack_off, rows, t.config.n_expert_used, nrows, bx, by, bsel, mx4_bias_rows(t, boff, l, rows), a, yoff)
    } else {
        a.hasb = 0u
        enc_moe_gemv(enc, t, fmt, stack_off, rows, t.config.n_expert_used, nrows, bx, by, bsel, a, yoff)
    }
}

//! layer `l`'s routed gate and up GEMVs over `nrows` rows' selections: gate into `bg`, up into `bu` at byte `uoff` - the
//! decode's routed block and the prefill window's gathered route both
def enc_moe_gate_up(enc : MetalComputeEncoder?; t : Model; l, nrows : int64; bx, bg, bu, bsel : MetalBuffer?; uoff : uint64; ka : MoeGemvArgs) {
    let nfe = t.config.n_ff_exp
    enc_moe_site(enc, t, l, t.we1_offs[l], t.web1_off, nfe, nrows, fmt_at(t.we1_fmt, l), bx, bg, bsel, ka, 0ul)
    enc_moe_site(enc, t, l, t.we3_offs[l], t.web3_off, nfe, nrows, fmt_at(t.we3_fmt, l), bx, bu, bsel, ka, uoff)
}

//! layer `l`'s routed down GEMV over `nrows` rows' selections, the activated rows in `bg` into `bdn`
def enc_moe_down(enc : MetalComputeEncoder?; t : Model; l, nrows : int64; bg, bdn, bsel : MetalBuffer?; ka : MoeGemvArgs) {
    enc_moe_site(enc, t, l, t.we2_offs[l], t.web2_off, t.config.dim, nrows, fmt_at(t.we2_fmt, l), bg, bdn, bsel, ka, 0ul)
}

//! the routed block's activation over `n` live elements (`btot` holds the count): gate rows in `bg`, up rows in `bu` at byte
//! `uoff` - gpt-oss's clamped swiglu, gemma-4's geglu, silu otherwise
def enc_moe_act(enc : MetalComputeEncoder?; c : Config; bg, bu : MetalBuffer?; uoff : uint64; btot : MetalBuffer?; n : int64) {
    if (c.ffn_act == FfnAct.swiglu_oai) {
        enc_swiglu_oai(enc, bg, 0ul, bu, uoff, btot, n)
    } elif (c.moe_dense_shexp) {
        enc_geglu(enc, bg, 0ul, bu, uoff, btot, n)
    } else {
        enc_swiglu(enc, bg, 0ul, bu, uoff, btot, n)
    }
}

//! the uniforms the residual step's split chain reads: the row width, the epsilon and the rows' element count
struct ResidualUniforms {
    bdim : MetalBuffer?
    beps : MetalBuffer?
    btot : MetalBuffer?
}

//! the residual step's dispatch shape: a decode row, a decode step's rows (1024 lanes a row), the prefill window's rows (256)
enum ResidualForm {
    one_row
    rows
    window
}

//! one layer seam of the residual step: the branch's post norm (a model-blob row, -1 none), the next norm the seam stores (-1 none)
//! and the layer's output scale (its model-blob element, -1 for one)
struct ResidualSeam {
    pre_off : int64
    norm_off : int64
    osc_off : int64
}

def seam(pre_off, norm_off : int64; osc_off : int64 = -1l) : ResidualSeam => ResidualSeam(pre_off = pre_off, norm_off = norm_off, osc_off = osc_off)

//! the seam's norm rows as one buffer, the post norm first: the args' `pwoff` 0 and `woff` past it
def private seam_norms(t : Model; sm : ResidualSeam) : MetalBuffer? {
    let rb = uint64(t.config.dim * 4l)
    unsafe {
        if (sm.pre_off >= 0l && sm.norm_off >= 0l) {
            return upload_region_cat(addr<void?>(t.fblob[sm.pre_off]), rb, addr<void?>(t.fblob[sm.norm_off]), rb)
        }
        return upload_region(addr<void?>(t.fblob[sm.pre_off >= 0l ? sm.pre_off : sm.norm_off]), rb)
    }
}

//! x = (x + a') . ascale over `nrows` rows, a' the partner under the seam's post norm, then x's next norm into `by` (`byh` its f16
//! twin) where the seam names one (`GkAddRmsRows`): one dispatch at any width while `split` is off, else the split chain
[arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-kernel-pairs")]
def enc_residual(enc : MetalComputeEncoder?; t : Model; bx, ba, by, byh : MetalBuffer?; sm : ResidualSeam; nrows : int64; form : ResidualForm;
                 split : bool; u : ResidualUniforms) {
    let c = t.config
    let dim = c.dim
    let pre = sm.pre_off >= 0l
    let out = sm.norm_off >= 0l
    if (split || (!pre && !out)) {
        let rb = uint64(dim * 4l)
        unsafe {
            var bwp = pre ? upload_region(addr<void?>(t.fblob[sm.pre_off]), rb) : null
            var bosc = sm.osc_off >= 0l ? upload_region(addr<void?>(t.fblob[sm.osc_off]), 4ul) : null
            var bw = out ? upload_region(addr<void?>(t.fblob[sm.norm_off]), rb) : null
            enc_residual_split(enc, form, bx, ba, ba, bwp, bosc, bw, by, byh, u, dim, nrows)
        }
        return
    }
    var bw = seam_norms(t, sm)
    var ka = GkAddRmsArgs(dim = uint(dim), add_on = 1u, eps = c.norm_eps, ascale = sm.osc_off >= 0l ? t.fblob[sm.osc_off] : 1.0)
    enc_residual_fused(enc, form, bx, ba, bw, by, byh, pre, out, ka, nrows, residual_wide(form, dim))
}

//! true where the step's one dispatch takes the 1024-lane `w` builders: a decode row past the slab
def residual_wide(form : ResidualForm; dim : int64) : bool => form != ResidualForm.window && dim > ADD_RMS_MAX_DIM

var private g_residual_narrow = 0l   //! the residual step's one dispatches on the 256-lane builders
var private g_residual_wide = 0l     //! ... and on the decode rows' 1024-lane ones

//! the residual step's one dispatches since load by lane width - which builders a seam past the slab took
def metal_residual_lanes() : tuple<narrow : int64; wide : int64> => (narrow = g_residual_narrow, wide = g_residual_wide)

//! the residual step's one dispatch in `form`: the plain step, the sandwich seam (`pre`), its no-norm form (`!out`), the f16 twin
//! beside where `byh` is bound; `wide` the decode rows' 1024-lane `w` builders, else 256 lanes
def enc_residual_fused(enc : MetalComputeEncoder?; form : ResidualForm; bx, ba, bw, by, byh : MetalBuffer?; pre, out : bool;
                       var ka : GkAddRmsArgs; nrows : int64; wide : bool) {
    if (wide) {
        g_residual_wide++
    } else {
        g_residual_narrow++
    }
    let one = !wide && form == ResidualForm.one_row
    let win = !wide && form == ResidualForm.window
    if (!out) {
        if (wide) {
            enc_preadd_bw_c(enc, bx, ba, bw, ka, nrows)
        } elif (one) {
            enc_preadd_c(enc, bx, ba, bw, ka)
        } elif (win) {
            pf_enc_preadd_b_c(enc, bx, ba, bw, ka, nrows)
        } else {
            enc_preadd_b_c(enc, bx, ba, bw, ka, nrows)
        }
    } elif (pre) {
        if (byh != null) {
            if (wide) {
                enc_preaddrms_bhxw_c(enc, bx, ba, bw, by, byh, ka, nrows)
            } elif (win) {
                pf_enc_preaddrms_bhx_c(enc, bx, ba, bw, by, byh, ka, nrows)
            } else {
                enc_preaddrms_bhx_c(enc, bx, ba, bw, by, byh, ka, nrows)
            }
        } elif (wide) {
            enc_preaddrms_bw_c(enc, bx, ba, bw, by, ka, nrows)
        } elif (one) {
            enc_preaddrms_c(enc, bx, ba, bw, by, ka)
        } elif (win) {
            pf_enc_preaddrms_b_c(enc, bx, ba, bw, by, ka, nrows)
        } else {
            enc_preaddrms_b_c(enc, bx, ba, bw, by, ka, nrows)
        }
    } elif (byh != null) {
        if (wide) {
            enc_add_rms_bhxw_c(enc, bx, ba, bw, by, byh, ka, nrows)
        } elif (win) {
            pf_enc_add_rms_bhx_c(enc, bx, ba, bw, by, byh, ka, nrows)
        } else {
            enc_add_rms_bhx_c(enc, bx, ba, bw, by, byh, ka, nrows)
        }
    } elif (wide) {
        enc_add_rms_bw_c(enc, bx, ba, bw, by, ka, nrows)
    } elif (one) {
        enc_addrms_c(enc, bx, ba, bw, by, ka)
    } elif (win) {
        pf_enc_add_rms_b_c(enc, bx, ba, bw, by, ka, nrows)
    } else {
        enc_add_rms_b_c(enc, bx, ba, bw, by, ka, nrows)
    }
}

//! the residual step's split chain over `nrows` rows of width `dim`: `ba` under `bwp` into `ban` (skipped where `bwp` is null), x +=
//! that row . `bosc` (one where null), then x's norm under `bw` into `by` (`byh` its f16 twin; skipped where `bw` is null)
def enc_residual_split(enc : MetalComputeEncoder?; form : ResidualForm; bx, ba, ban, bwp, bosc, bw, by, byh : MetalBuffer?;
                       u : ResidualUniforms; dim, nrows : int64) {
    if (bwp != null) {
        enc_rms_form(enc, form, ba, bwp, ban, null, u, nrows)
    }
    enc_add(enc, bx, 0ul, bwp != null ? ban : ba, 0ul, u.btot, nrows * dim, bosc)
    if (bw != null) {
        enc_rms_form(enc, form, bx, bw, by, byh, u, nrows)
    }
}

//! the split chain's norm: `bx`'s rows under `bw` into `by` (`byh` its f16 twin where bound), in the residual form's shape
def enc_rms_form(enc : MetalComputeEncoder?; form : ResidualForm; bx, bw, by, byh : MetalBuffer?; u : ResidualUniforms; nrows : int64) {
    if (form == ResidualForm.window) {
        if (byh != null) {
            pf_enc_rms_hx(enc, bx, 0ul, bw, by, u.bdim, u.beps, byh, nrows)
        } else {
            pf_enc_rms(enc, bx, 0ul, bw, by, u.bdim, u.beps, nrows)
        }
    } elif (byh != null) {
        enc_rms_bhx(enc, bx, 0ul, bw, by, u.bdim, u.beps, byh, nrows)
    } elif (form == ResidualForm.one_row) {
        enc_rms(enc, bx, 0ul, bw, by, u.bdim, u.beps)
    } else {
        enc_rms_b(enc, bx, 0ul, bw, by, u.bdim, u.beps, nrows)
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
               bx, by, bn, bd, bys : MetalBuffer?; d : int64; yoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    if (four) {
        enc_gemv_b4_c(enc, bw, boff, bw, bx, xoff, by, yoff, bn, bd, bys, d)
    } else {
        enc_gemv_b2_c(enc, bw, boff, bw, bx, xoff, by, yoff, bn, bd, bys, d)
    }
}

//! the tile block is handed its live row count and BYTE offsets off the site's x row width n and
//! y row stride ys - a form that grids over four rows ignores the count, an mv form binds it
def enc_rows_tiles(nrows, n, ys : int64; tile : block<(four : bool; live : int64; xoff, yoff : uint64) : void>) {
    assert(nrows <= 12l, "three B4 tiles cover twelve rows")
    invoke(tile, nrows > 2l, min(nrows, 4l), 0ul, 0ul)
    if (nrows > 4l) {
        invoke(tile, true, min(nrows - 4l, 4l), uint64(4l * n * 4l), uint64(4l * ys * 4l))
    }
    if (nrows > 8l) {
        invoke(tile, true, nrows - 8l, uint64(8l * n * 4l), uint64(8l * ys * 4l))
    }
}

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#verify-row-pad")]
def enc_gemv_rows(enc : MetalComputeEncoder?; nrows : int64; bw : MetalBuffer?; boff : uint64;
                  bx, by, bn, bd, bys : MetalBuffer?; n, d, ys : int64; yoff : uint64 = 0ul) {
    var base = 0l
    while (base < nrows) {   // the tile ladder repeats every twelve rows, each pass offset by its first row
        let xb = uint64(base * n * 4l)
        let yb = uint64(base * ys * 4l)
        enc_rows_tiles(min(nrows - base, 12l), n, ys) $(four; _live; xo, yo) {
            enc_gemv_b(enc, four, bw, boff, bx, by, bn, bd, bys, d, yoff + yb + yo, xb + xo)
        }
        base += 12l
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
// stay the fast skinny form). The tiles differ only in their live row count.
def enc_mv_site(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                bx, by : MetalBuffer?; n, xs4, d, nrows, ys : int64;
                yoff : uint64 = 0ul) {
    var ka = Q8MvArgs(ndim = uint(n), ddim = uint(d), nrows = 0u, xs4 = uint(xs4), ys = uint(ys))
    enc_rows_tiles(nrows, n, ys) $(four; live; xo, yo) {
        ka.nrows = uint(live)
        enc_mv_b(enc, four, bwblob, wboff, bx, by, ka, xo, yoff + yo)
    }
}

// fused fixed-B W1|W3 GEMV + swiglu — the w1 and w3 blob slices bind separately (kind-major
// layout), by is the [B x hidden] swiglu output. Replaces the w1 + w3 GEMVs + the swiglu ew2
// with one dispatch. Same 4-rows/tg geometry, gridded over the hidden output rows.
def enc_gemv_w13sw_b(enc : MetalComputeEncoder?; four : bool; bw : MetalBuffer?; w1off, w3off : uint64;
                     bx, by, bn, bhid, bact : MetalBuffer?; hidden : int64; xoff : uint64 = 0ul; yoff : uint64 = 0ul) {
    if (four) {
        enc_w13sw_b4_c(enc, bw, w1off, bw, bx, xoff, by, yoff, bn, bhid, bact, bw, w3off, bw, hidden)
    } else {
        enc_w13sw_b2_c(enc, bw, w1off, bw, bx, xoff, by, yoff, bn, bhid, bact, bw, w3off, bw, hidden)
    }
}

// the fused W1|W3 + activation site at nrows live rows (enc_gemv_rows's tiling; y stride = hidden), `bact` the act code (0 silu,
// 1 gelu)
def enc_gemv_w13sw_rows(enc : MetalComputeEncoder?; nrows : int64; bw : MetalBuffer?; w1off, w3off : uint64;
                        bx, by, bn, bhid, bact : MetalBuffer?; n, hidden : int64) {
    enc_rows_tiles(nrows, n, hidden) $(four; _live; xo, yo) {
        enc_gemv_w13sw_b(enc, four, bw, w1off, w3off, bx, by, bn, bhid, bact, hidden, xo, yo)
    }
}

// the production mul_mm dispatch (the prefill kernel: 34B-blob dual-view W, raw f32 X): y binds
// at a byte offset so per-tensor tiles land inside a wider fused row — the ndim uniform is the
// y ROW STRIDE while this tensor's output width d shapes the grid
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

//! sums `nsk` partial planes of `total` floats (a multiple of four) into `by` at byte `yoff`; `cols` floats a partial row land
//! on output rows `ld` floats apart (0: the output's own shape). The launch covers the writes' reach, `(rows - 1) x ld + cols`,
//! so the bound span stops where the writes do
def enc_sk_reduce_b(enc : MetalComputeEncoder?; bpart, by : MetalBuffer?; total, nsk : int64; yoff : uint64 = 0ul; cols : int64 = 0l; ld : int64 = 0l) {
    let strided = cols > 0l && ld > cols
    var ka = SkReduceArgs(total = uint(total / 4l), ksplit = uint(nsk), cols4 = strided ? uint(cols / 4l) : 0u, ld4 = uint(ld / 4l))
    enc_sk_reduce_c(enc, bpart, by, yoff, ka, strided ? ((total / cols - 1l) * ld + cols) / 4l : total / 4l)
}

// the 64-wide N tile (classifier site)
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
//! Whether the fused attention has a stamp for a head: 64, 96 and 128 on the narrow stamps (a lane a quad, the lanes past a
//! narrow head idle), the wide 256 and 512, on every mirror codec. A block codec needs a whole number of 32-element blocks a head.
def attn_d_head_ok(head_size : int64) : bool
    => head_size == 64l || head_size == 96l || head_size == 128l || head_size == 256l || head_size == 512l

//! the slice groups a row of `keys` keys splits over: a slice of 32 keys a group, sixteen groups at most
def attn_d_groups(keys : int64) : int64 => min((keys + 31l) / 32l, 16l)

//! the heads of 128 the fused attention's partials make of one head: two of a 256 head, four of a 512 head, the head itself otherwise
def attn_d_halves(head_size : int64) : int64 => head_size > 128l ? head_size / 128l : 1l

//! the fused attention's partial plane in floats: an entry a row, head half and slice group, each the half's sums plus its max and total
def attn_d_part_floats(nrows, n_heads, head_size, nsgs : int64) : int64 {
    let halves = attn_d_halves(head_size)
    return nrows * n_heads * halves * nsgs * (head_size / halves + 4l)
}

def enc_attn_d(enc : MetalComputeEncoder?; kdt : KVDtype; bq, bpart, brt : MetalBuffer?; var ka : SqAttnDArgs; nsgs, nrows, nheads : int64; rtoff : uint64 = 0ul) {
    if (ka.hs == 512u) {
        if (kdt == KVDtype.q8_0) {
            enc_attndx_q8_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
        } elif (kdt == KVDtype.tq4) {
            enc_attndx_tq4_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
        } elif (kdt == KVDtype.f16) {
            enc_attndx16_c(enc, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
        } else {
            enc_attndx32_c(enc, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
        }
    } elif (ka.hs == 256u) {
        if (kdt == KVDtype.q8_0) {
            enc_attndw_q8_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
        } elif (kdt == KVDtype.tq4) {
            enc_attndw_tq4_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
        } elif (kdt == KVDtype.f16) {
            enc_attndw16_c(enc, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
        } else {
            enc_attndw32_c(enc, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
        }
    } elif (kdt == KVDtype.q8_0) {
        enc_attnd_q8_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
    } elif (kdt == KVDtype.tq4) {
        enc_attnd_tq4_c(enc, g_arena, g_arena, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
    } elif (kdt == KVDtype.f16) {
        enc_attnd16_c(enc, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
    } else {
        enc_attnd32_c(enc, g_arena, g_arena, bq, bpart, brt, rtoff, ka, nheads, nsgs, nrows)
    }
}

def enc_rope_store_b(enc : MetalComputeEncoder?; f16 : bool; bqkv, bcos, bsin, brt, bbias : MetalBuffer?; var ka : GkRopeKvArgs; npairs, nrows : int64; rtoff : uint64 = 0ul) {
    if (f16) {
        enc_rpstb16_c(enc, bqkv, bqkv, bqkv, 0ul, g_arena, 0ul, g_arena, 0ul, bbias, bcos, 0ul, bsin, brt, rtoff, ka, npairs, nrows)
    } else {
        enc_rpstb32_c(enc, bqkv, bqkv, bqkv, 0ul, g_arena, 0ul, g_arena, 0ul, bbias, bcos, 0ul, bsin, brt, rtoff, ka, npairs, nrows)
    }
}

// tq4-mirror batched rope+store: one threadgroup per (head, batch row), arena bound four ways
def enc_rope_store_b_tq4(enc : MetalComputeEncoder?; bqkv, bcos, bsin, brt, bbias : MetalBuffer?; var ka : RopeStoreBTq4Args; ntg, nrows, head_size : int64; rtoff : uint64 = 0ul;
                         bq : MetalBuffer? = null) {
    enc_rpstb_tq4_c(enc, bqkv, g_arena, g_arena, g_arena, g_arena, bcos, bsin, signs_buffer(), brt, rtoff, bbias,
        ka, bq != null ? bq : bqkv, ntg, nrows, head_size)
}

// q8_0-mirror batched rope+store: arena bound four ways (K/V x scale/byte views); grid covers
// the q pairs, then a lane an element of the K and V rows (q8_store_threads), one grid row per batch row
def enc_rope_store_b_q8(enc : MetalComputeEncoder?; bqkv, bcos, bsin, brt, bbias : MetalBuffer?; var ka : RopeStoreBQ8Args; nthreads, nrows : int64; rtoff : uint64 = 0ul;
                        bq : MetalBuffer? = null) {
    enc_rpstb_q8_c(enc, bqkv, g_arena, g_arena, g_arena, g_arena, bcos, bsin, brt, rtoff, bbias, ka, bq != null ? bq : bqkv, nthreads, nrows)
}

// enc_rms_b (the batched prologue) and pf_enc_rms (the prefill 256/tg shape) are
// [metal_dispatch]-generated from MetalRmsNorm alongside enc_rms.

//! Release every PSO the registry compiled (the PSO half of metal_decode_shutdown — runs
//! before the shared device goes).
def metal_kernels_release {
    release_handles(g_pso_gemv, g_pso_gemv_sg, g_pso_kq_k4, g_pso_kq_k5, g_pso_kq_k5c, g_pso_kq_k6)
    release_handles(g_pso_kq_mvb2_k4, g_pso_kq_mvb2_k4r2, g_pso_kq_mvb4_k4, g_pso_kq_mvb2_k5, g_pso_kq_mvb4_k5, g_pso_kq_mvb2_k6)
    release_handles(g_pso_kq_mvb4_k6, g_pso_kq_mvb8_k4, g_pso_kq_mvb8_k5, g_pso_kq_mvb8_k6, g_pso_kq_mm_b4, g_pso_kq_mm_b5)
    release_handles(g_pso_kq_mm_b6, g_pso_kq_iq4xs, g_pso_kq_mvb2_iq4xs, g_pso_kq_mvb4_iq4xs, g_pso_kq_mvb8_iq4xs, g_pso_kq_mm_b44)
    release_handles(g_pso_kq_iq3s4, g_pso_kq_mvb2_iq3s4, g_pso_kq_mvb4_iq3s4, g_pso_kq_mvb8_iq3s4, g_pso_kq_mm_b35, g_pso_emb_kq_iq3s4)
    release_handles(g_pso_moe_kq_iq3s4)
    release_handles(g_pso_kq_k3, g_pso_kq_mvb2_k3, g_pso_kq_mvb4_k3, g_pso_kq_mvb8_k3, g_pso_kq_mm_b3, g_pso_kq_iq3s)
    release_handles(g_pso_kq_mvb2_iq3s, g_pso_kq_mvb4_iq3s, g_pso_kq_mvb8_iq3s, g_pso_kq_mm_b33, g_pso_kq_iq3xxs, g_pso_kq_mvb2_iq3xxs)
    release_handles(g_pso_kq_mvb4_iq3xxs, g_pso_kq_mvb8_iq3xxs, g_pso_kq_mm_b34, g_pso_kq_iq4nl, g_pso_kq_iq2s, g_pso_kq_mvb2_iq2s)
    release_handles(g_pso_kq_mvb4_iq2s, g_pso_kq_mvb8_iq2s, g_pso_kq_mm_b23, g_pso_kq_iq2xs, g_pso_kq_mvb2_iq2xs, g_pso_kq_mvb4_iq2xs)
    release_handles(g_pso_kq_mvb8_iq2xs, g_pso_kq_mm_b24, g_pso_kq_iq2xxs, g_pso_kq_iq2xxs_f4, g_pso_kq_iq3s_f4, g_pso_kq_iq3xxs_f4)
    release_handles(g_pso_kq_mvb2_iq2xxs, g_pso_kq_mvb4_iq2xxs, g_pso_kq_mvb8_iq2xxs, g_pso_kq_mm_b25, g_pso_kq_mvb2_iq4nl, g_pso_kq_mvb4_iq4nl)
    release_handles(g_pso_kq_mvb8_iq4nl, g_pso_kq_mm_b45, g_pso_kq_q40, g_pso_kq_mvb2_q40, g_pso_kq_mvb4_q40, g_pso_kq_mvb8_q40)
    release_handles(g_pso_kq_nv4, g_pso_kq_mvb2_nv4, g_pso_kq_mvb4_nv4, g_pso_kq_mvb8_nv4, g_pso_kq_mm_b49)
    release_handles(g_pso_kq_mvb2h_nv4, g_pso_kq_mvb4h_nv4, g_pso_kq_mvb8h_nv4)
    release_handles(g_pso_kq_mm_b40, g_pso_kq_k2, g_pso_kq_mvb2_k2, g_pso_kq_mvb4_k2, g_pso_kq_mvb8_k2, g_pso_kq_mm_b2)
    release_handles(g_pso_rms, g_pso_rms_hx, g_pso_dn_ba, g_pso_moe_router_b, g_pso_add, g_pso_addrms_hx)
    release_handles(g_pso_kq_mvw2h_k4, g_pso_kq_mvw4h_k4, g_pso_kq_mvw8h_k4)
    release_handles(g_pso_kq_mvb2h_k5, g_pso_kq_mvb4h_k5, g_pso_kq_mvb8h_k5, g_pso_kq_mvb2h_k6, g_pso_kq_mvb4h_k6, g_pso_kq_mvb8h_k6,
        g_pso_kq_mvb2h_iq4xs, g_pso_kq_mvb4h_iq4xs, g_pso_kq_mvb8h_iq4xs, g_pso_kq_mvb2h_iq4nl, g_pso_kq_mvb4h_iq4nl, g_pso_kq_mvb8h_iq4nl,
        g_pso_kq_mvb2h_q40, g_pso_kq_mvb4h_q40, g_pso_kq_mvb8h_q40, g_pso_kq_mvb2h_k3, g_pso_kq_mvb4h_k3, g_pso_kq_mvb8h_k3,
        g_pso_kq_mvb2h_iq3s, g_pso_kq_mvb4h_iq3s, g_pso_kq_mvb8h_iq3s, g_pso_kq_mvb2h_iq3xxs, g_pso_kq_mvb4h_iq3xxs, g_pso_kq_mvb8h_iq3xxs,
        g_pso_kq_mvb2h_k2, g_pso_kq_mvb4h_k2, g_pso_kq_mvb8h_k2, g_pso_kq_mvb2h_iq2s, g_pso_kq_mvb4h_iq2s, g_pso_kq_mvb8h_iq2s,
        g_pso_kq_mvb2h_iq2xs, g_pso_kq_mvb4h_iq2xs, g_pso_kq_mvb8h_iq2xs, g_pso_kq_mvb2h_iq2xxs, g_pso_kq_mvb4h_iq2xxs, g_pso_kq_mvb8h_iq2xxs)
    release_handles(g_pso_addrms, g_pso_preaddrms, g_pso_preaddrms_hx, g_pso_preadd, g_one, g_zero)
    release_handles(g_pso_geglu, g_pso_geglu_hx)
    release_handles(g_pso_geglu_side, g_pso_ple_proj_bf16, g_pso_ple_proj_bf16_one, g_pso_ple_finish, g_pso_f32_rows_one, g_pso_f32_rows_b)
    release_handles(g_pso_ple_proj_bf16_hw, g_pso_ple_proj_bf16_hw_one)
    release_handles(g_pso_ln, g_pso_bias_act, g_pso_act_rows, g_pso_posadd, g_pso_q3v_stem, g_pso_clamp, g_pso_clamp_copy)
    release_handles(g_pso_rope2d, g_pso_head_pad, g_pso_head_unpad, g_pso_geglu_quick, g_pso_wdec_kv, g_pso_wdec_kv_h)
    release_handles(g_pso_im2col2d, g_pso_wdec_attn_part, g_pso_wdec_attn_comb, g_pso_bias_add_res)
    release_handles(g_pso_f32mm, g_pso_cls_epi, g_pso_cls_select, g_pso_swiglu, g_pso_swiglu_hx, g_pso_swiglu_hxb)
    release_handles(g_pso_swiglu_b, g_pso_geglu_hxb, g_pso_geglu_b, g_pso_attn16, g_pso_attn32, g_pso_attnp16)
    release_handles(g_pso_attnp32, g_pso_attn_q8, g_pso_attnp_q8, g_pso_rpst_q8, g_pso_attn_tq4, g_pso_attnp_tq4)
    release_handles(g_pso_rpst_tq4, g_pso_unsign, g_pso_qknorm, g_pso_attncomb, g_pso_rpst16, g_pso_qkn16)
    release_handles(g_pso_qkn32, g_pso_qknb16, g_pso_qknb32)
    release_handles(g_pso_rpst32, g_pso_argmax_rows, g_pso_argmax, g_pso_copy_row, g_pso_emb, g_pso_emb_kq_k6)
    release_handles(g_pso_emb_kq_k4, g_pso_emb_kq_k5, g_pso_emb_kq_iq4xs, g_pso_emb_kq_iq4nl, g_pso_emb_kq_q40, g_pso_emb_kq_k3, g_pso_emb_kq_nv4)
    release_handles(g_pso_emb_kq_k2, g_pso_emb_kq_iq3s, g_pso_emb_kq_iq3xxs, g_pso_emb_kq_iq2s, g_pso_emb_kq_iq2xs, g_pso_emb_kq_iq2xxs)
    release_handles(g_pso_emb_b32_q51, g_pso_emb_b32_iq4nl32, g_pso_emb_b32_q4032, g_pso_emb_b32_q5032)
    release_handles(g_pso_moe_router, g_pso_moe_select, g_pso_moe_select_512, g_pso_moe_gemv_q8, g_pso_moe_kq_k4, g_pso_moe_kq_k3)
    release_handles(g_pso_moe_select_ds, g_pso_moe_select_512_ds)
    release_handles(g_pso_moe_kq_iq3s, g_pso_moe_kq_iq3xxs, g_pso_moe_kq_iq2s, g_pso_moe_kq_iq2xs, g_pso_moe_kq_iq2xxs, g_pso_moe_kq_iq4xs)
    release_handles(g_pso_moe_kq_iq3s_f4)
    release_handles(g_pso_moe_kq_q40, g_pso_moe_kq_nv4, g_pso_moe_kq_k5, g_pso_moe_kq_k6, g_pso_moe_combine, g_pso_g4rnorm)
    release_handles(g_pso_moe_combine_sh, g_pso_moe_comb_g4, g_pso_moe_comb_g4_hx, g_pso_moe_comb_g4_pf)
    release_handles(g_pso_swiglu_oai, g_pso_moe_gemv_mx4, g_pso_moe_gemv_q51, g_pso_moe_gemv_iq4nl32, g_pso_dn_conv, g_pso_dn_conv_n, g_pso_dn_conv_n4, g_pso_dn_hist, g_pso_dn_l2)
    release_handles(g_pso_moe_kq_iq4nl, g_pso_moe_kq_k2, g_pso_kq_q51, g_pso_kq_iq4nl32, g_pso_kq_mm_b51, g_pso_kq_mm_b4n32)
    release_handles(g_pso_b32_mvb2_q51, g_pso_b32_mvb4_q51, g_pso_b32_mvb8_q51, g_pso_b32_mvb2_iq4nl32, g_pso_b32_mvb4_iq4nl32, g_pso_b32_mvb8_iq4nl32)
    release_handles(g_pso_kq_q51_ks, g_pso_kq_iq4nl32_ks, g_pso_b32_mvb2_q51_ks, g_pso_b32_mvb4_q51_ks, g_pso_b32_mvb8_q51_ks)
    release_handles(g_pso_b32_mvb2_iq4nl32_ks, g_pso_b32_mvb4_iq4nl32_ks, g_pso_b32_mvb8_iq4nl32_ks)
    release_handles(g_pso_moe_gemv_q4032, g_pso_kq_q4032, g_pso_kq_q4032_ks, g_pso_kq_mm_b4032)
    release_handles(g_pso_b32_mvb2_q4032, g_pso_b32_mvb4_q4032, g_pso_b32_mvb8_q4032, g_pso_b32_mvb2_q4032_ks, g_pso_b32_mvb4_q4032_ks, g_pso_b32_mvb8_q4032_ks)
    release_handles(g_pso_moe_gemv_q5032, g_pso_kq_q5032, g_pso_kq_q5032_ks, g_pso_kq_mm_b5032)
    release_handles(g_pso_b32_mvb2_q5032, g_pso_b32_mvb4_q5032, g_pso_b32_mvb8_q5032, g_pso_b32_mvb2_q5032_ks, g_pso_b32_mvb4_q5032_ks, g_pso_b32_mvb8_q5032_ks)
    release_handles(g_pso_dn_scan, g_pso_dn_gate, g_pso_dn_deint, g_pso_sigmul, g_mx4_vtab)
    release_handles(g_pso_dn_scan_h, g_pso_dn_prep)
    release_handles(g_pso_dn_gate_sig, g_pso_hc_norm, g_pso_hc_lo, g_pso_hc_init, g_pso_cat2, g_pso_hc_mix, g_pso_hc_combine, g_pso_ple_gate, g_pso_ple_conv)
    release_handles(g_pso_st2_attn)
    release_handles(g_pso_pk_rows, g_pso_pk_rows_elu, g_pso_pk_row_scale, g_pso_pk_attn, g_pso_pk_rope_tab)
    release_handles(g_pso_tw_rms, g_pso_tw_rope_tab, g_pso_tw_bias_gate, g_pso_tw_add_ln, g_pso_tw_add_rms, g_pso_tw_add_post_rms)
    release_handle(g_pso_tw_patch_im2col)
    release_handles(g_pso_pk_add_ln, g_pso_pk_gemv_f32, g_pso_pk_gemv_f32_ln_silu, g_pso_pk_gemv_f32_gate, g_pso_pk_gemv_f32_add_silu)
    release_handles(g_pso_pk_gemv_f32_tail, g_pso_pk_gemv_q8_ln_silu, g_pso_pk_gemv_q8_gate, g_pso_pk_gemv_q8_add_silu, g_pso_pk_gemv_q8_tail)
    release_handles(g_pso_st2_adain_leaky, g_pso_row_gather, g_pso_st2_concat, g_pso_st2_pool_dw, g_pso_st2_lstm, g_pso_st2_sigsum, g_pso_st2_gelu_tanh,
        g_pso_st2_src_low_torch, g_pso_st2_src_low_onnx, g_pso_st2_src_cumsum_torch, g_pso_st2_src_cumsum_onnx, g_pso_st2_src_noise,
        g_pso_st2_src_sines_torch, g_pso_st2_src_sines_onnx, g_pso_st2_stft_reflect, g_pso_st2_stft_edge, g_pso_st2_istft)
    release_handles(g_pso_f32_exact_mm, g_pso_st2_conv_exact_mm, g_pso_st2_conv_tr_mm, g_pso_st2_conv_tr_exact_mm)
    release_handles(g_pso_st2_im2col, g_pso_st2_conv_mm, g_pso_st2_colstats, g_pso_st2_adain_snake, g_pso_st2_leaky, g_pso_st2_axpy, g_pso_st2_add, g_pso_st2_reflect1)
    release_handles(g_pso_qkvrs16, g_pso_qkvrs32, g_pso_w13sw, g_pso_w13sw_sg, g_pso_gemv_b2, g_pso_gemv_b4)
    release_handles(g_pso_mv_b2, g_pso_mv_b4, g_pso_w13sw_b2, g_pso_w13sw_b4, g_pso_gemm_b, g_pso_gemm_b_sk)
    release_handles(g_pso_skred, g_pso_gemm64_b, g_pso_mm, g_pso_attnb16, g_pso_attnb32, g_pso_attnb_q8)
    metal_base_split_release()
    release_handles(g_pso_attnb_tq4, g_pso_attnpb16, g_pso_attnpb32, g_pso_attnpb_q8, g_pso_attnpb_tq4, g_pso_attncombb)
    release_handles(g_pso_attnd16, g_pso_attnd32, g_pso_attnd_q8, g_pso_attnd_tq4, g_pso_attncombd, g_pso_rpstb16)
    release_handles(g_pso_attndw16, g_pso_attndw32, g_pso_attndx16, g_pso_attndx32)
    release_handles(g_pso_attndw_q8, g_pso_attndw_tq4, g_pso_attndx_q8, g_pso_attndx_tq4, g_pso_rpstb32, g_pso_rpstb_q8, g_pso_rpstb_tq4)
    release_handles(g_pso_kv_rows_q8, g_pso_kv_rows_tq4)
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
    var bw = metal_buffer_from(dev, uint64(ndim * nkb * 34), unsafe(addr<void?>(blob[0])))
    var bxa = metal_buffer_from(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = metal_buffer_from(dev, uint64(out_n * 4), null)
    var by_twin = metal_buffer_from(dev, uint64(out_n * 4), null)
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
    kq_race_verdict(res, by_base, by_twin, out_n)
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
    var p = race_pso_pair(dev, family, base_src, base_entry, base_fm, twin_src, twin_entry, twin_fm)
    return p.verdict if (!p.ok)
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
    let res = race_gemmb_family(dev, queue, family, m, kdim, ndim, out_n, grid, base_pso, twin_pso, base_tgmem, twin_tgmem, ka) $(enc, slot_y) {
        invoke(bind_extra, enc, slot_y)
    }
    metal_release(base_pso)
    metal_release(twin_pso)
    return res
}

//! the K-quant formats with a small-batch twin, each with its race fixture's plane parameters
//! (quant uints per superblock, scale-plane form) - the same table the prefill mul_mm race uses
let KQ_ROWS_RACE_FORMATS <- [
    (fmt = KqFmt.k4, qu = 32, sform = KqRaceScaleForm.compact),
    (fmt = KqFmt.k5, qu = 40, sform = KqRaceScaleForm.compact),
    (fmt = KqFmt.k6, qu = 48, sform = KqRaceScaleForm.split_d),
    (fmt = KqFmt.iq4xs, qu = 32, sform = KqRaceScaleForm.split_d),
    (fmt = KqFmt.iq4nl, qu = 32, sform = KqRaceScaleForm.pure_d),
    (fmt = KqFmt.q40, qu = 32, sform = KqRaceScaleForm.pure_d),
    (fmt = KqFmt.nv4, qu = int(kq_qsb(KqFmt.nv4) / 4l), sform = KqRaceScaleForm.ue4m3),
    (fmt = KqFmt.k3, qu = 24, sform = KqRaceScaleForm.split_d),
    (fmt = KqFmt.iq3s, qu = 26, sform = KqRaceScaleForm.split_d),
    (fmt = KqFmt.iq3xxs, qu = 24, sform = KqRaceScaleForm.split_d),
    (fmt = KqFmt.k2, qu = 16, sform = KqRaceScaleForm.split_dmin),
    (fmt = KqFmt.iq2s, qu = 18, sform = KqRaceScaleForm.split_d),
    (fmt = KqFmt.iq2xs, qu = 16, sform = KqRaceScaleForm.split_d),
    (fmt = KqFmt.iq2xxs, qu = 16, sform = KqRaceScaleForm.split_d)
]

def private race_kq_rig_pair(queue : MetalCommandQueue?; family : string; qu_per_sb : int; sform : KqRaceScaleForm;
                             n, rows : int;
                             base_arm, twin_arm : block<(enc : MetalComputeEncoder?; rig : KqRaceRig; var ka : KqMvArgs) : void>) : MetalTensorRaceResult {
    var res = MetalTensorRaceResult(family = family, winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    var rig <- kq_race_rig(g_dev, qu_per_sb, sform, n, rows)
    var ka = KqMvArgs(ndim = uint(n), ddim = uint(rows), ys = uint(rows), nr = 2u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        invoke(base_arm, enc, rig, ka)
    }, $(enc) {
        invoke(twin_arm, enc, rig, ka)
    })
    kq_race_verdict(res, rig.by_a, rig.by_b, 2 * rows)
    release_kq_race_rig(rig)
    return res
}

//! needs the decode PSOs - the caller runs `metal_decode_init` first
[arch(at="../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity")]
def private race_kq_rows(queue : MetalCommandQueue?; fmt : KqFmt; qu_per_sb : int; sform : KqRaceScaleForm) : MetalTensorRaceResult {
    let n = 2048
    let rows = 8192
    return race_kq_rig_pair(queue, "kq_rows_{fmt}", qu_per_sb, sform, n, rows, $(enc, rig, ka) {
        for (r in range(8)) {
            enc_kq_mvb(enc, fmt, rig.bq, 0ul, rig.bs, 0ul, rig.doff, rig.bx, (r & 1) == 0 ? rig.by_a : rig.by_a2, 0ul, ka, int64(rows), 2l)
        }
    }, $(enc, rig, _ka) {
        for (r in range(8)) {
            let by = (r & 1) == 0 ? rig.by_b : rig.by_b2
            enc_kq_gemv(enc, fmt, rig.bq, 0ul, rig.bs, 0ul, rig.doff, rig.bx, by, rig.bn, rig.bd, int64(rows), int64(n))
            enc_kq_gemv(enc, fmt, rig.bq, 0ul, rig.bs, 0ul, rig.doff, rig.bx, by, rig.bn, rig.bd, int64(rows), int64(n), uint64(rows * 4), uint64(n * 4))
        }
    })
}

[arch(at="../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity")]
def private race_kq_k4_form(queue : MetalCommandQueue?) : MetalTensorRaceResult {
    let n = 2048
    let rows = 8192
    return race_kq_rig_pair(queue, "kq_mvb2_k4_r2", 32, KqRaceScaleForm.compact, n, rows, $(enc, rig, ka) {
        for (r in range(8)) {
            enc_kq_mvb2_k4_c(enc, rig.bs, 0ul, rig.bs, 0ul, rig.bq, 0ul, rig.bx, 0ul, (r & 1) == 0 ? rig.by_a : rig.by_a2, 0ul, ka, int64(rows))
        }
    }, $(enc, rig, ka) {
        for (r in range(8)) {
            enc_kq_mvb2_k4r2_c(enc, rig.bs, 0ul, rig.bs, 0ul, rig.bq, 0ul, rig.bx, 0ul, (r & 1) == 0 ? rig.by_b : rig.by_b2, 0ul, ka, int64(rows))
        }
    })
}

//! a format's half-x crown race: the box's f32 rows route (under the crowns the races before it decided) against the
//! half-x route, at two, four and eight rows in every timed encoder - every form the dispatcher has - on a 27B-class
//! FFN plane past the last-level cache (5120 x 16384: 47 MB at k4, 70 MB at k6); the half forms are "tensor"
[arch(at="../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity")]
def private race_kq_half(queue : MetalCommandQueue?; fmt : KqFmt; qu_per_sb : int; sform : KqRaceScaleForm) : MetalTensorRaceResult {
    let n = 5120
    let rows = 16384
    var res = MetalTensorRaceResult(family = kq_half_crown(fmt), winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    var rig <- kq_race_rig(g_dev, qu_per_sb, sform, n, rows, 8)
    let widths <- [2l, 4l, 8l]
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        for (r in range(6)) {
            enc_kq_rows_f32(enc, fmt, rig.bq, 0ul, rig.bs, 0ul, rig.doff, rig.bx, (r & 1) == 0 ? rig.by_a : rig.by_a2, 0ul,
                rig.bn, rig.bd, int64(rows), int64(n), int64(n), int64(rows), widths[r % 3])
        }
    }, $(enc) {
        for (r in range(6)) {
            enc_kq_rows_half(enc, fmt, rig.bq, 0ul, rig.bs, 0ul, rig.doff, rig.bxh, (r & 1) == 0 ? rig.by_b : rig.by_b2, 0ul,
                int64(rows), int64(n), int64(rows), widths[r % 3])
        }
    })
    kq_race_verdict(res, rig.by_a, rig.by_b, 8 * rows)
    release_kq_race_rig(rig)
    return res
}

// the K-quant races' fixture: one format's planes at n x rows, `cols` x columns on the f16 lattice with
// their half twin, two y planes per arm, the n/d uniforms
struct private KqRaceRig {
    bq, bs, bx, bxh, by_a, by_a2, by_b, by_b2, bn, bd : MetalBuffer?
    doff : uint64
    xa : array<float>
    xh : array<float16>
}

def private kq_race_rig(dev : MetalDevice?; qu_per_sb : int; sform : KqRaceScaleForm; n, rows : int; cols : int = 2) : KqRaceRig {
    var rig : KqRaceRig
    var planes = race_kq_planes(dev, rows * (n / 256), qu_per_sb, sform)
    rig.bq = planes.bq
    rig.bs = planes.bs
    rig.doff = planes.doff
    rig.xa <- race_x_f32(cols * n)
    rig.xh <- [for (v in rig.xa); float16(v)]   //! race_x_f32 sits on the f16 lattice, so both x views read one value
    rig.bx = metal_buffer_from(dev, uint64(cols * n * 4), unsafe(addr<void?>(rig.xa[0])))
    rig.bxh = metal_buffer_from(dev, uint64(cols * n * 2), unsafe(addr<void?>(rig.xh[0])))
    rig.by_a = metal_buffer_from(dev, uint64(cols * rows * 4), null)
    rig.by_a2 = metal_buffer_from(dev, uint64(cols * rows * 4), null)
    rig.by_b = metal_buffer_from(dev, uint64(cols * rows * 4), null)
    rig.by_b2 = metal_buffer_from(dev, uint64(cols * rows * 4), null)
    rig.bn = race_uniform_u32(dev, uint(n))
    rig.bd = race_uniform_u32(dev, uint(rows))
    return <- rig
}

def private release_kq_race_rig(var rig : KqRaceRig) {
    metal_release(rig.bq)
    metal_release(rig.bs)
    metal_release(rig.bx)
    metal_release(rig.bxh)
    metal_release(rig.by_a)
    metal_release(rig.by_a2)
    metal_release(rig.by_b)
    metal_release(rig.by_b2)
    metal_release(rig.bn)
    metal_release(rig.bd)
    delete rig.xa
    delete rig.xh
}

//! Decode-side tuner entry (the runtime.metal_tensor crown): the batch q8 GEMM families and the
//! K-quant small-batch form per format.
[arch(at="../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity")]
def metal_tensor_race_decode : array<MetalTensorRaceResult> {
    var results : array<MetalTensorRaceResult>
    results |> reserve(8 + 2 * length(KQ_ROWS_RACE_FORMATS))
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
    results |> emplace(race_gemv_f4_twin(dev, queue, "kq_gemv_iq3s_f4",
        metal_kq_gemv_iq3s_msl, metal_kq_gemv_iq3s_msl_entry, metal_kq_gemv_iq3s_msl_fastmath,
        metal_kq_gemv_iq3s_f4_msl, metal_kq_gemv_iq3s_f4_msl_entry, metal_kq_gemv_iq3s_f4_msl_fastmath, metal_kq_gemv_iq3s_f4_msl_tgmem, 26, 4, 8))
    results |> emplace(race_gemv_f4_twin(dev, queue, "kq_gemv_iq3xxs_f4",
        metal_kq_gemv_iq3xxs_msl, metal_kq_gemv_iq3xxs_msl_entry, metal_kq_gemv_iq3xxs_msl_fastmath,
        metal_kq_gemv_iq3xxs_f4_msl, metal_kq_gemv_iq3xxs_f4_msl_entry, metal_kq_gemv_iq3xxs_f4_msl_fastmath, metal_kq_gemv_iq3xxs_f4_msl_tgmem, 24, 4, 8))
    metal_release(queue)
    metal_release(dev)
    // the small-batch form per K-quant format rides the production PSOs, so it races on g_dev
    if (metal_decode_init()) {
        var pq = metal_new_command_queue(g_dev)
        var form = race_kq_k4_form(pq)
        g_kq_k4_r2_crowned = form.winner == "tensor"
        results |> emplace(form)
        for (f in KQ_ROWS_RACE_FORMATS) {
            var rr = race_kq_rows(pq, f.fmt, f.qu, f.sform)
            g_kq_rows_crowned[int(f.fmt)] = rr.winner == "tensor"   //! every format's half race times against the rows route this verdict leaves
            results |> emplace(rr)
        }
        for (f in KQ_ROWS_RACE_FORMATS) {
            var half = race_kq_half(pq, f.fmt, f.qu, f.sform)
            g_kq_half_crowned[int(f.fmt)] = half.winner == "tensor"
            results |> emplace(half)
        }
        metal_release(pq)
    }
    return <- results
}

//! A GEMV f4-slab twin race over the split-scale fixture: base (constant-table, rows/4) vs
//! the f4-slab twin. Winner "tensor" = the twin takes the family crown.
[arch(at="../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity")]
def private race_gemv_f4_twin(dev : MetalDevice?; queue : MetalCommandQueue?; family : string;
                              base_src, base_entry : string; base_fm : bool;
                              twin_src, twin_entry : string; twin_fm : bool; twin_tgmem : uint64;
                              qu_per_sb : int; base_gdiv, twin_gdiv : int) : MetalTensorRaceResult {
    let kdim = 2048
    let ndim = 8192
    var p = race_pso_pair(dev, family, base_src, base_entry, base_fm, twin_src, twin_entry, twin_fm)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
    var rig <- kq_race_rig(dev, qu_per_sb, KqRaceScaleForm.split_d, kdim, ndim)
    let tg = uint3(64u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_buffer(enc, rig.bs, rig.doff, 0)
        kn_buffer(enc, rig.bs, 0ul, 1)
        kn_buffer(enc, rig.bq, 0ul, 2)
        kn_buffer(enc, rig.bx, 0ul, 3)
        kn_buffer(enc, rig.bn, 0ul, 5)
        kn_buffer(enc, rig.bd, 0ul, 6)
        for (r in range(32)) {
            kn_buffer(enc, (r & 1) == 0 ? rig.by_a : rig.by_a2, 0ul, 4)
            kn_dispatch(enc, uint3(uint(ndim / base_gdiv), 1u, 1u), tg)
        }
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_tgmem(enc, twin_tgmem, 0)
        kn_buffer(enc, rig.bs, rig.doff, 0)
        kn_buffer(enc, rig.bs, 0ul, 1)
        kn_buffer(enc, rig.bq, 0ul, 2)
        kn_buffer(enc, rig.bx, 0ul, 3)
        kn_buffer(enc, rig.bn, 0ul, 5)
        kn_buffer(enc, rig.bd, 0ul, 6)
        for (r in range(32)) {
            kn_buffer(enc, (r & 1) == 0 ? rig.by_b : rig.by_b2, 0ul, 4)
            kn_dispatch(enc, uint3(uint(ndim / twin_gdiv), 1u, 1u), tg)
        }
    })
    kq_race_verdict(res, rig.by_a, rig.by_b, ndim)
    release_kq_race_rig(rig)
    metal_release(base_pso)
    metal_release(twin_pso)
    return res
}

// ===== family-shared kernel classes (M1: moved from prefill) =====

// One threadgroup per row: cooperative sum of squares, lane 0 folds rsqrt(mean + eps), every
// thread scales its strided slice. HX = the dual-store stamp: the scale loop also writes the
// f16 twin panel the half-X GEMM stamps stream — the norm's consumers pay no convert dispatch.
[ |> template_struct_instance]
class template MetalRmsNormT : MetalTgReduceBase {
    @ssbo @binding = 0 @off = "xoff" x : array<float>     // [nrows x dim] source
    @ssbo @binding = 1 @role = "weight" w : array<float>     // [dim] weight
    @ssbo @binding = 2 y : array<float>     // [nrows x dim] out (x aliasing is safe: x is fully
                                            // read before the barrier that precedes any write)
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 eps : float
    @ssbo @binding = 5 @role = "write" @template_gate = HX yh : array<float16>   // [nrows x dim] f16 twin
    @template_constant HX : bool = false

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
        let inv = tg_rms_inv(ss, dim, eps)
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

[metal_dispatch(name = "pf_enc_rms_hx", pso = "g_pso_rms_hx", tgmem = "MetalRmsNormHX_metal_rmsnorm_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "enc_rms_bhx", pso = "g_pso_rms_hx", tgmem = "MetalRmsNormHX_metal_rmsnorm_msl_tgmem", tg = 1024, grid = "nrows", params = "nrows : int64")]
class MetalRmsNormHX : MetalRmsNormT {
    override HX = true
}

//! Row LayerNorm (w + b), POPULATION variance, on the seam-and-norm template both homes stamp: the norm alone, xb may alias x
[metal_dispatch(name = "enc_ln", pso = "g_pso_ln", tgmem = "metal_layernorm_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalLayerNorm : GkPostAddLn {
    override ADD = false

    [metal_kernel(name="metal_layernorm_msl")]
    def metal_layernorm {
        post_add_ln_body()
    }
}

// One elementwise map g[i] = map(g[i]) in place — the ew1 family (the 2-operand twin is MetalEw2T).
// A stamp needing its own entry name derives from the base; MetalEw1T adds the shared metal_ew1 entry.
[ |> template_struct_instance]
class template MetalEw1BaseT {
    @ssbo @binding = 0 @role = "readwrite" @off = "goff" @span = "total*4" g : array<float>
    @uniform @binding = 1 total : uint      // grid guard

    def abstract map(a : float) : float

    def ew1_body {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let vv = g[gid]
        g[gid] = map(vv)
    }
}

[ |> template_struct_instance]
class template MetalEw1T : MetalEw1BaseT {
    [metal_kernel]
    def metal_ew1 {
        ew1_body()
    }
}

//! a GEMM output's bias row and activation, the rows template both homes stamp: four elements a thread, `total` = the
//! element count over four (`tw_bias` / `tw_act` size the grid)
[metal_dispatch(name = "enc_bias_act", pso = "g_pso_bias_act", tg = 64, grid = "total/64", params = "total : int64")]
class MetalBiasAct : GkBiasAct {
    [metal_kernel(name = "metal_bias_act_msl")]
    def metal_bias_act {
        bias_act_body()
    }
}

//! the activation alone over the rows (the GELUs, silu), the same template with its bias row gated off
[metal_dispatch(name = "enc_act_rows", pso = "g_pso_act_rows", tg = 64, grid = "total/64", params = "total : int64")]
class MetalActRows : GkBiasAct {
    override BIAS = false

    [metal_kernel(name = "metal_act_rows_msl")]
    def metal_act_rows {
        bias_act_body()
    }
}

//! the bias row at `a.boff` of `bw` added to the rows at `a.xoff` of `bx`, then `a.act`, over `a.nelem` elements
def tw_bias(enc : MetalComputeEncoder?; bx, bw : MetalBuffer?; var a : GkBiasActArgs) {
    enc_bias_act(enc, bx, bw, a, (int64(a.nelem) + 3l) / 4l)
}

//! `a.act` alone over the rows at `a.xoff` of `bx`
def tw_act(enc : MetalComputeEncoder?; bx : MetalBuffer?; var a : GkBiasActArgs) {
    enc_act_rows(enc, bx, a, (int64(a.nelem) + 3l) / 4l)
}

//! the gemma4uv 2-axis position add on the template both homes stamp
[metal_dispatch(name = "enc_posadd2d", pso = "g_pso_posadd", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPosAdd2D : GkPosAdd2D {
    [metal_kernel(name = "metal_posadd2d_msl")]
    def metal_posadd2d {
        pos_add_2d_body()
    }
}

//! the qwen3v stem assemble on the template both homes stamp
[metal_dispatch(name = "enc_q3v_stem", pso = "g_pso_q3v_stem", tg = 64, grid = "total/64", params = "total : int64")]
class MetalQ3vStemAsm : GkStemAsm {
    [metal_kernel(name = "metal_q3v_stem_msl")]
    def metal_q3v_stem {
        stem_asm_body()
    }
}

//! rows clamped in place, the template both homes stamp (`tw_clamp` sizes the grid: the elements over four)
[metal_dispatch(name = "enc_clamp_rows", pso = "g_pso_clamp", tg = 64, grid = "total/64", params = "total : int64")]
class MetalClampRows : GkClampRows {
    [metal_kernel(name = "metal_clamp_msl")]
    def metal_clamp {
        clamp_rows_body()
    }
}

//! the clamped rows into another buffer: a clamped GEMM's input copy ahead of the f32 tile, the source left whole
[metal_dispatch(name = "enc_clamp_copy", pso = "g_pso_clamp_copy", tg = 64, grid = "total/64", params = "total : int64")]
class MetalClampCopy : GkClampRows {
    override COPY = true

    [metal_kernel(name = "metal_clamp_copy_msl")]
    def metal_clamp_copy {
        clamp_rows_body()
    }
}

//! `n` elements of `bx` clamped to [lo, hi] in place
def tw_clamp(enc : MetalComputeEncoder?; bx : MetalBuffer?; lo, hi : float; n : int64) {
    var a = GkClampArgs(nelem = uint(n), lo = lo, hi = hi, scale = 1.0)
    enc_clamp_rows(enc, bx, a, (n + 3l) / 4l)
}

//! `n` elements of `bx` clamped to [lo, hi] into `bdst`
def tw_clamp_copy(enc : MetalComputeEncoder?; bx, bdst : MetalBuffer?; lo, hi : float; n : int64) {
    var a = GkClampArgs(nelem = uint(n), lo = lo, hi = hi, scale = 1.0)
    enc_clamp_copy(enc, bx, bdst, a, (n + 3l) / 4l)
}

//! The head restride around the attention tiles (gemma3v, the qwen vision towers), on the template both homes stamp: the pad
//! writes the tile's wider head panel with zero lanes past hs, the unpad reads the compact rows back off it
[metal_dispatch(name = "enc_head_pad", pso = "g_pso_head_pad", tg = 64, grid = "total/64", params = "total : int64")]
class MetalHeadPad : GkRestride {
    [metal_kernel(name="metal_head_pad_msl")]
    def metal_head_pad {
        restride_body()
    }
}

//! the head restride's unpad, on the template both homes stamp (GkRestride)
[metal_dispatch(name = "enc_head_unpad", pso = "g_pso_head_unpad", tg = 64, grid = "total/64", params = "total : int64")]
class MetalHeadUnpad : GkRestride {
    override PAD = false

    [metal_kernel(name="metal_head_unpad_msl")]
    def metal_head_unpad {
        restride_body()
    }
}

//! The gemma4v two-axis NEOX rope over token-major rows, on the template both homes stamp
[metal_dispatch(name = "enc_rope2d", pso = "g_pso_rope2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalRope2D : GkRope2d {
    [metal_kernel(name="metal_rope2d_msl")]
    def metal_rope2d {
        rope2d_body()
    }
}

//! the whisper decoder's K/V stores, the template both homes stamp: the CPU chain's f32 layouts (the pre-scaled transposed keys,
//! the values), one thread an element
[metal_dispatch(name = "enc_wdec_kv", pso = "g_pso_wdec_kv", tg = 64, grid = "total/64", params = "total : int64")]
class MetalWdecKv : GkWdecKv {
    [metal_kernel(name = "metal_wdec_kv_msl")]
    def metal_wdec_kv {
        wdec_kv_body()
    }
}

//! ... the resident f16 K/V memory (the decode step's K/V append rides it too)
[metal_dispatch(name = "enc_wdec_kv_h", pso = "g_pso_wdec_kv_h", tg = 64, grid = "total/64", params = "total : int64")]
class MetalWdecKvH : GkWdecKv {
    override OUT16 = true

    [metal_kernel(name = "metal_wdec_kv_h_msl")]
    def metal_wdec_kv_h {
        wdec_kv_body()
    }
}

//! The chunked twin pair on the templates both homes stamp: Part computes one 256-key chunk's online-softmax partial per
//! (chunk, head, row) - local max m, exp-sum s, unnormalized weighted V - and Comb merges the chunks by log-sum-exp;
//! 6-8x the threadgroups of the single-pass form on a 1500-key cross
[metal_dispatch(name = "enc_wdec_attn_part", pso = "g_pso_wdec_attn_part", tgmem = "metal_wdec_attn_part_msl_tgmem", tg = 128, grid = "groups", params = "groups : int64")]
class MetalWdecAttnPart : GkWdecAttnPart {
    [metal_kernel(name="metal_wdec_attn_part_msl")]
    def metal_wdec_attn_part {
        wdec_attn_part_body()
    }
}

//! the chunked attention's combine, on the template both homes stamp (the f32 store; the shared reduce base's slots ride along unused)
[metal_dispatch(name = "enc_wdec_attn_comb", pso = "g_pso_wdec_attn_comb", tgmem = "metal_wdec_attn_comb_msl_tgmem", tg = 64, grid = "groups", params = "groups : int64")]
class MetalWdecAttnComb : GkWdecAttnComb {
    [metal_kernel(name="metal_wdec_attn_comb_msl")]
    def metal_wdec_attn_comb {
        wdec_attn_comb_body()
    }
}

//! The conv2d im2col (k3 s2 p1) over a block of channel rows, on the template both homes stamp - the f32 tile's feed here
[metal_dispatch(name = "enc_im2col2d", pso = "g_pso_im2col2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalIm2col2d : GkIm2col2d {
    typedef OT = float

    [metal_kernel(name="metal_im2col2d_msl")]
    def metal_im2col2d {
        im2col2d_body()
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

//! One chunk of a rows conv: output rows [r0, r0 + rows) padded to mp, kdim = k * cin padded to 32; xs the input rows' stride (cin, or the producer's padded width).
struct St2ConvArgs {
    cin : uint
    k : uint
    stride : uint
    pad_l : uint
    dil : uint
    t_in : uint
    r0 : uint
    rows : uint
    xs : uint
}

//! The float4 index of a forward conv's output row rr 8-run at column kk, or 0xFFFFFFFF where the run reads zeros.
def st2_conv_b_src_fwd(rr, kk, rows, k, cin, r0, pad_l, dil, stride, t_in, xs : uint) : uint {
    if (rr >= rows || kk >= k * cin) {
        return 0xFFFFFFFF
    }
    let st = conv_src_fwd_row(rr, kk / cin, r0, pad_l, dil, stride, t_in)
    return st >= 0 ? (uint(st) * xs + kk % cin) / 4u : 0xFFFFFFFF
}

//! The transposed conv's twin: output row rr reads the source row its tap lands on a stride, or zeros.
def st2_conv_b_src_tr(rr, kk, rows, k, cin, r0, pad_l, dil, stride, t_in, xs : uint) : uint {
    if (rr >= rows || kk >= k * cin) {
        return 0xFFFFFFFF
    }
    let st = conv_src_tr_row(rr, kk / cin, r0, pad_l, dil, stride, t_in)
    return st >= 0 ? (uint(st) * xs + kk % cin) / 4u : 0xFFFFFFFF
}

//! y[m x ndim] = x[m x kdim] . W^T, f32 accumulate; ndim % 64, kdim % 32, m padded to 32; CONV: cin % 8.
//! The stamps set the tile type: half tiles (TILET float16, SGT simdgroup_half8x8) stage the
//! operands at f16, float tiles (EXACT) keep every product the f32 one.
[ |> template_struct_instance]
class template MetalF32MulMmT : MetalMmTileBase {
    @template_constant CONV : bool = false
    @template_constant DENSE : bool = true
    @template_constant EXACT : bool = false
    @template_constant TRANSPOSED : bool = false
    @ssbo @binding = 0 @role = "weight" @off = "woff" w : array<float4>   // [ndim x kdim/4] weight rows
    @ssbo @binding = 1 @role = "read" xf : array<float4>                  // [m x kdim/4] activations (CONV: the [t_in x cin] rows)
    @ssbo @binding = 2 @role = "write" @off = "yoff" @span = "mp*d*4" y : array<float>
    @uniform @binding = 3 @template_gate = DENSE kdim_u : uint   //!< the dense stamps' K; the CONV stamps read it from their kargs
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 @template_gate = CONV ka : St2ConvArgs
    // inherited ta = W tiles (k row, wrow col), tb = X tiles (token row, k col)

    //! K padded to 32: the dense stamps' uniform, the CONV stamps' k*cin.
    def k_padded() : uint {
        static_if (CONV) {
            return (ka.k * ka.cin + 31u) / 32u * 32u
        } else {
            return kdim_u
        }
    }

    //! A W element at the tile type: the half stamps round it, the float stamps keep it.
    def tile(v : float) : TILET =>
        TILET(v)  // nolint:PERF020,LINT019 — the float stamps' identity cast

    //! One W tile: four float4 rows staged at the tile type.
    def stage_a(a0, a1, a2, a3 : float4; tA0 : uint) {
        var va : TILET[16]
        va[0] = tile(a0.x)
        va[1] = tile(a0.y)
        va[2] = tile(a0.z)
        va[3] = tile(a0.w)
        va[4] = tile(a1.x)
        va[5] = tile(a1.y)
        va[6] = tile(a1.z)
        va[7] = tile(a1.w)
        va[8] = tile(a2.x)
        va[9] = tile(a2.y)
        va[10] = tile(a2.z)
        va[11] = tile(a2.w)
        va[12] = tile(a3.x)
        va[13] = tile(a3.y)
        va[14] = tile(a3.z)
        va[15] = tile(a3.w)
        for [unroll_full] (i in range(16)) {
            ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
        }
    }

    def f32_mm_body {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var mc : simdgroup_float8x8[8]
        var ma : SGT[4]
        var mb : SGT[2]
        let kdim = k_padded()
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
            var b0 = float4(0.0)
            var b1 = float4(0.0)
            static_if (CONV) {
                var srcB = 0xFFFFFFFF
                static_if (TRANSPOSED) {
                    srcB = st2_conv_b_src_tr(mBase + lr1, kb * 32u + iy, ka.rows, ka.k, ka.cin, ka.r0, ka.pad_l, ka.dil, ka.stride, ka.t_in, ka.xs)
                } else {
                    srcB = st2_conv_b_src_fwd(mBase + lr1, kb * 32u + iy, ka.rows, ka.k, ka.cin, ka.r0, ka.pad_l, ka.dil, ka.stride, ka.t_in, ka.xs)
                }
                if (srcB != 0xFFFFFFFF) {
                    b0 = xf[srcB]
                    b1 = xf[srcB + 1u]
                }
            } else {
                let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
                b0 = xf[srcB]
                b1 = xf[srcB + 1u]
            }
            barrier()
            stage_a(a0, a1, a2, a3, tA0)
            static_if (EXACT) {
                tg_store_float4(tb, ibB, b0)
                tg_store_float4(tb, ibB + 4u, b1)
            } else {
                tg_store_half4(tb, ibB, b0)
                tg_store_half4(tb, ibB + 4u, b1)
            }
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

[metal_dispatch(name = "enc_f32_mm", pso = "g_pso_f32mm", tgmem = "metal_f32_mulmm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalF32MulMm : MetalF32MulMmT {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8

    [metal_kernel(name="metal_f32_mulmm_msl")]
    def metal_f32_mulmm {
        f32_mm_body()
    }
}

//! The StyleTTS2 chain's rows conv as the GEMM with the im2col folded into the X loader: a
//! forward conv's gather, and the transposed conv's on its own stamp.
[metal_dispatch(name = "enc_st2_conv_mm", pso = "g_pso_st2_conv_mm", tgmem = "metal_st2_conv_mm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64", requires = "ka.cin % 8, ka.xs % 4")]
class MetalSt2ConvMm : MetalF32MulMmT {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    override CONV = true
    override DENSE = false

    [metal_kernel(name="metal_st2_conv_mm_msl")]
    def metal_st2_conv_mm {
        f32_mm_body()
    }
}

[metal_dispatch(name = "enc_st2_conv_tr_mm", pso = "g_pso_st2_conv_tr_mm", tgmem = "metal_st2_conv_tr_mm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64", requires = "ka.cin % 8, ka.xs % 4")]
class MetalSt2ConvTrMm : MetalF32MulMmT {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    override CONV = true
    override DENSE = false
    override TRANSPOSED = true

    [metal_kernel(name="metal_st2_conv_tr_mm_msl")]
    def metal_st2_conv_tr_mm {
        f32_mm_body()
    }
}

//! The f32-exact stamps: float tiles, twice the threadgroup memory, for the sites whose outputs
//! round to integers or feed a phase (the StyleTTS2 front end).
[metal_dispatch(name = "enc_f32_exact_mm", pso = "g_pso_f32_exact_mm", tgmem = "metal_f32_exact_mulmm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalF32ExactMm : MetalF32MulMmT {
    typedef TILET = float
    typedef SGT = simdgroup_float8x8
    override EXACT = true

    [metal_kernel(name="metal_f32_exact_mulmm_msl")]
    def metal_f32_exact_mulmm {
        f32_mm_body()
    }
}

[metal_dispatch(name = "enc_st2_conv_exact_mm", pso = "g_pso_st2_conv_exact_mm", tgmem = "metal_st2_conv_exact_mm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64", requires = "ka.cin % 8, ka.xs % 4")]
class MetalSt2ConvExactMm : MetalF32MulMmT {
    typedef TILET = float
    typedef SGT = simdgroup_float8x8
    override CONV = true
    override DENSE = false
    override EXACT = true

    [metal_kernel(name="metal_st2_conv_exact_mm_msl")]
    def metal_st2_conv_exact_mm {
        f32_mm_body()
    }
}

[metal_dispatch(name = "enc_st2_conv_tr_exact_mm", pso = "g_pso_st2_conv_tr_exact_mm", tgmem = "metal_st2_conv_tr_exact_mm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64", requires = "ka.cin % 8, ka.xs % 4")]
class MetalSt2ConvTrExactMm : MetalF32MulMmT {
    typedef TILET = float
    typedef SGT = simdgroup_float8x8
    override CONV = true
    override DENSE = false
    override EXACT = true
    override TRANSPOSED = true

    [metal_kernel(name="metal_st2_conv_tr_exact_mm_msl")]
    def metal_st2_conv_tr_exact_mm {
        f32_mm_body()
    }
}

// The production GEMM: 64-output x 32-token C tiles (4 simdgroups, 8 accumulators), W as
// interleaved 34B q8 blocks, raw f32 activations. Rolled [unroll_full] math over simdgroup-matrix
// arrays. Beats the upstream equivalent 2-3% on M1 Max.
[ |> template_struct_instance]
class template MetalQ8MulMmBaseT : MetalMmTileBase {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> // 34B-block W blob, HALF-SCALE view: scale of block b at 17*b
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    // the SAME blob buffer, BYTE view: quants of block b at 34*b + 2
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // raw f32 activations, float4 view
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "mp*d*4" @template_gate = "!SK" y : array<float>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "nsk*mp*d*4" @template_gate = SK yk : array<float>   //!< `nsk` partial planes of mp x d
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @template_constant SK : bool = false   //!< split-K: the grid's z threadgroups share a tile's K, each into its own partial plane
    // inherited ta = W tiles (k row, wrow col), tb = X tiles (token row, k col)

    [metal_kernel]
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
        let kper = SK ? nkb / gl_NumWorkGroups.z : nkb
        let kbeg = SK ? gl_WorkGroupID.z * kper : 0u
        let blk0 = (nBase + lr0) * nkb + kbeg
        var scur = unsafe(addr(wsh[blk0 * 17u]))                    // half scale rides the quants' cacheline
        var qp = unsafe(addr(wqb[blk0 * 34u + 2u + il0 * 16u]))
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u + kbeg * 8u]))
        var kb = 0u
        while (kb < kper) {
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
        static_if (SK) {
            let plane = gl_WorkGroupID.z * gl_NumWorkGroups.x * 32u * ndim
            for [unroll_full] (i in range(8)) {
                simdgroup_store(mc[i], yk, plane + (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
            }
        } else {
            for [unroll_full] (i in range(8)) {
                simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
            }
        }
    }
}

[metal_dispatch(name = "enc_gemm_mm_c", pso = "g_pso_mm", tgmem = "MetalQ8MulMm_metal_q8_mulmm_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64", requires = "mp % 32, d % 64")]
class MetalQ8MulMm : MetalQ8MulMmBaseT {}

//! the base tile's split-K stamp (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`): a box with no q8 tensor crown splits here
[metal_dispatch(name = "enc_gemm_mm_sk_c", pso = "g_pso_mm_sk", tgmem = "MetalQ8MulMmSk_metal_q8_mulmm_msl_tgmem", tg = 128, grid = "mp/32, d/64, nsk", params = "mp : int64, d : int64, nsk : int64", requires = "mp % 32, d % 64")]
class MetalQ8MulMmSk : MetalQ8MulMmBaseT {
    override SK = true
}

//! The 32 x 64 simdgroup tile's W decode, one source for the dense mul_mm and the routed base form: a lane decodes 16
//! elements of a weight row's 32-block `kb`, half `il0`, into `va`. KQ keeps the scale and quant views (q8 and mx4 bind their
//! own), KQ_SC the superblock formats' sub-scale view; B32 is the per-32 plane pair (q51, iq4nl32 under IQ4XS).
[ |> template_struct_instance]
class template MetalKqTileDecodeT : MetalMmTileBase {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    //! s0off is the SLOT-0 view offset: k4/k5 callers pass soff again, k6 passes the d-plane doff; the per-32 formats bind their scale plane there
    @ssbo @binding = 0 @role = "weight" @off = "s0off" @template_gate = KQ ksh : array<float16> //! k4/k5: 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1); k6: the d plane (bound at byte nsb*16); q51: d, m at 2*blk; iq4nl32: d at blk
    @ssbo @binding = 1 @role = "weight" @off = "soff" @template_gate = KQ_SC ks4 : array<uint4>   // the same buffer, uint4 view (k6: 16 int8 sub-scales at blk, bound at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" @template_gate = KQ kqu : array<uint>    //! quant plane, uint view (qs at SB_UINTS*blk; k5 qh at SB_UINTS*blk+32; k6 ql at 48*blk, qh at 48*blk+32; q51 at 5*blk; iq4nl32 at 4*blk)
    @template_constant KQ : bool = false   //! the stamp decodes through this template's views
    @template_constant KQ_SC : bool = false   //! ... and reads the sub-scale uint4 view (every superblock format)
    @template_constant B32 : bool = false   //! the per-32 planes: a 32-block a block, no superblock
    @template_constant Q51 : bool = false   //! q5_1: 5-bit quants (nibble | qh bit), d*q + m
    @template_constant Q50 : bool = false   //!< q5032: q5_1's quant block under one d, d * (q - 16)
    @template_constant B32W : uint = 4u     //!< a per-32 quant block's words: KQ_B32_QWORDS, KQ_Q5032_QWORDS on q5032 (the qh word last)
    @template_constant B32S : uint = 1u     //!< a per-32 block's scale halves: 2 on q51 (d then m)
    @template_constant SB_UINTS : uint = 32u  // superblock quant-plane stride in uints (unused in the SIXBIT arm — k6 strides are hardcoded 48/16/8)
    @template_constant QH : bool = false // q5_K: the 5th-bit overlay plane rides after the nibbles
    @template_constant SIXBIT : bool = false // q6_K: int8 sub-scales x super-d, ql/qh compose
    @template_constant IQ4XS : bool = false //! iq4xs: the k6 split scale form, the q40 pairing, codebook decode (B32: iq4nl32's block d)
    @template_constant K3 : bool = false // q3_K: the k6 split scale form, 2-bit lane + hmask compose, offset 4
    @template_constant IQ3S : bool = false // iq3s: the k6 split scale form, threadgroup grid gather + sign flip
    @template_constant IQ3XXS : bool = false // iq3xxs: the same form over the halved grid + parity signs
    @template_constant IQ4NL : bool = false // iq4nl: the IQ4XS nibble walk with the per-32-block f16 d plane (no sub-scale; s0off = soff)
    @template_constant K2 : bool = false // q2_K: per-16 sc/min nibble pairs (one uint4 strip), the split 4B d+dmin tail at s0off
    @template_constant IQ2S : bool = false // iq2s: the u64-grid gather DIRECT off the hoisted constant table, per-16 UNSIGNED strips
    @template_constant IQ2XS : bool = false // iq2xs: iq2s's walk with ksigns-by-parity off each u16 qs word (no sign plane, no qh)
    @template_constant IQ2XXS : bool = false // iq2xxs: byte grid indices + aux32 parity ksigns, per-32 UNSIGNED strips
    @template_constant Q40 : bool = false // q40: iq4nl's walk and d plane, the linear nibble map (q - 8) in the codebook slot
    @template_constant IQODD : bool = false   //!< iq3s4: the IQ4XS walk and split scale form, the odd codebook (`iq3s4_val`) staged
    @template_constant NV4 : bool = false   //!< nv4: the IQ4XS walk over the compact 16-byte UE4M3 row (byte 2 js + il0 the half's scale), the doubled e2m1 codebook staged
    @workgroup @template_gate = IQ4XS iq4lut : float[16]   // kvalues_iq4nl as floats, staged once per threadgroup
    @workgroup @template_gate = IQ3S gridw3 : uint[512]   // the 2 KB iq3s grid, staged once per threadgroup
    @workgroup @template_gate = IQ3XXS gridw3x : uint[256]   // the 1 KB halved iq3xxs grid, staged once per threadgroup

    //! the decode unit a weight row is counted in: the 256-element superblock, or the per-32 formats' block
    def kq_unit() : uint {
        static_if (B32) {
            return 32u
        } else {
            return 256u
        }
    }

    //! the format's threadgroup tables (the iq4 codebook, the iq3 grids), staged once before the walk by all 128 lanes
    def kq_tables {
        let lid = gl_LocalInvocationID.x
        static_if (IQ4XS) {
            if (lid < 16u) {
                static_if (Q40) {
                    iq4lut[lid] = float(int(lid)) - 8.0
                } else {
                    static_if (IQODD) {
                        iq4lut[lid] = iq3s4_val(lid)
                    } else {
                        static_if (NV4) {
                            iq4lut[lid] = mx4_val(lid)
                        } else {
                            iq4lut[lid] = iq4_lut(lid)
                        }
                    }
                }
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
    }

    //! publishes a decoded A half into the shared W tile, fenced against the previous math loop
    def publish_a(var va : float16[16]; tA0 : uint) {
        barrier()
        for [unroll_full] (i in range(16)) {
            ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
        }
    }

    //! row origin `wb` (in `kq_unit()`s), 32-block `kb`, half `il0` -> the 16 decoded elements in `va`
    def kq_decode(wb, kb, il0 : uint; var va : float16[16]) {   // nolint:STYLE037,STYLE038 — one decode arm per format, each a constant's static_if
        static_if (KQ) {
            static_if (B32) {
                let blk32 = wb + kb
                static_if (Q51) {
                    let nsh32 = il0 * 4u
                    let hb32 = il0 * 16u
                    let d32 = float(ksh[blk32 * B32S])
                    let m32 = float(ksh[blk32 * B32S + 1u])
                    let qp32 = unsafe(addr(kqu[blk32 * B32W]))
                    let qh32 = unsafe(qp32[B32W - 1u])
                    for [unroll_full] (k in range(4)) {
                        let u32 = unsafe(qp32[k])
                        for [unroll_full] (c in range(4)) {
                            let el32 = uint(k) * 4u + uint(c)
                            let q32 = ((u32 >> (8u * uint(c) + nsh32)) & 15u) | (((qh32 >> (el32 + hb32)) & 1u) << 4u)
                            va[k * 4 + c] = float16(d32 * float(q32) + m32)
                        }
                    }
                } else {
                    static_if (Q50) {
                        let nsh32 = il0 * 4u
                        let hb32 = il0 * 16u
                        let d32 = float(ksh[blk32])
                        let qp32 = unsafe(addr(kqu[blk32 * B32W]))
                        let qh32 = unsafe(qp32[B32W - 1u])
                        for [unroll_full] (k in range(4)) {
                            let u32 = unsafe(qp32[k])
                            for [unroll_full] (c in range(4)) {
                                let el32 = uint(k) * 4u + uint(c)
                                let q32 = ((u32 >> (8u * uint(c) + nsh32)) & 15u) | (((qh32 >> (el32 + hb32)) & 1u) << 4u)
                                va[k * 4 + c] = float16(d32 * float(int(q32) - 16))
                            }
                        }
                    } else {
                        let nsh32 = il0 * 4u
                        let dsc32 = float(ksh[blk32])
                        let qp32 = unsafe(addr(kqu[blk32 * B32W]))
                        for [unroll_full] (k in range(4)) {
                            let u32 = unsafe(qp32[k])
                            for [unroll_full] (c in range(4)) {
                                va[k * 4 + c] = float16(dsc32 * iq4lut[(u32 >> (8u * uint(c) + nsh32)) & 15u])
                            }
                        }
                    }
                }
            } else {
                let blk = wb + kb / 8u
                let js = kb % 8u
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
                    let qp = unsafe(addr(kqu[blk * 48u + half6 * 16u + (gg & 1u) * 8u + il0 * 4u]))
                    let hp = unsafe(addr(kqu[blk * 48u + 32u + half6 * 8u + il0 * 4u]))
                    let nsh = (gg / 2u) * 4u
                    let hsh = gg * 2u
                    for [unroll_full] (k in range(4)) {
                        let u = unsafe(qp[k])
                        let hu = unsafe(hp[k])
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
                        let qp = unsafe(addr(kqu[blk * 24u + hf3 * 8u + il0 * 4u]))
                        let hp = unsafe(addr(kqu[blk * 24u + 16u + il0 * 4u]))
                        let nsh = gg3 * 2u
                        let hsh = hf3 * 4u + gg3
                        for [unroll_full] (k in range(4)) {
                            let u = unsafe(qp[k])
                            let hu = unsafe(hp[k])
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
                                let sgb2 = ksign7((aux2 >> (7u * q3)) & 127u)
                                let nib = uint(k) % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u
                                let sw = iq3s_signed(iq2xxs_gw(int(bidx * 2u + uint(k) % 2u)), nib)
                                for [unroll_full] (c in range(4)) {
                                    va[k * 4 + c] = float16(dsc * iq3s_sx(sw, uint(c)))
                                }
                            }
                        } else {
                        static_if (IQ2XS) {
                            // iq2xs: the iq2s strip/scale reads; each u16 qs word = 9-bit grid
                            // index + 7-bit ksigns index whose sign byte ksign7 recomputes
                            let sv = ks4[blk]
                            let si2 = js * 2u + il0
                            let scw = si2 < 8u ? (si2 < 4u ? sv.x : sv.y) : (si2 < 12u ? sv.z : sv.w)
                            let dsc = float(ksh[blk]) * float((scw >> ((si2 & 3u) * 8u)) & 255u)
                            let qw2 = kqu[blk * 16u + js * 2u + il0]
                            for [unroll_full] (k in range(4)) {
                                let w16 = (qw2 >> (16u * (uint(k) / 2u))) & 0xFFFFu
                                let idx2 = (w16 & 511u) * 2u + uint(k) % 2u
                                let sgb2 = ksign7(w16 >> 9u)
                                let nib = uint(k) % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u
                                let sw = iq3s_signed(iq2xs_gw(int(idx2)), nib)
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
                                let sw = iq3s_signed(iq2s_gw(int(idx2)), nib)
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
                                let sgb = ksign7((aux3 >> (7u * q3)) & 127u)
                                let nib = uint(k) % 2u == 0u ? sgb & 15u : sgb >> 4u
                                let sw = iq3s_signed(gridw3x[(qsw >> (8u * uint(k))) & 255u], nib)
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
                                    let sw = iq3s_signed(gridw3[idx], nib)
                                    for [unroll_full] (c in range(4)) {
                                        va[k * 4 + c] = float16(dsc * iq3s_sx(sw, uint(c)))
                                    }
                                }
                            } else {
                                static_if (IQ4XS) {
                                    // iq4xs: strip byte js is the SIGNED sub-scale, super-d off the d plane (the k6 split
                                    // form); block js = 4 uints of the q40 pairing, il0 picks the nibble half; the codebook
                                    // off threadgroup memory. iq4nl: same walk, dsc = block js's own f16 d
                                    var dsc : float
                                    static_if (NV4) {
                                        dsc = nv4_sub_scale(ks4[blk], js * 2u + il0)
                                    } else {
                                        static_if (IQ4NL) {
                                            dsc = float(ksh[blk * 8u + js])
                                        } else {
                                            let sv = ks4[blk]
                                            let scw = js < 4u ? sv.x : sv.y
                                            dsc = float(ksh[blk]) * float(((int(scw >> ((js & 3u) * 8u)) & 255) ^ 128) - 128)
                                        }
                                    }
                                    let qp = unsafe(addr(kqu[blk * 32u + js * 4u]))
                                    let nsh = il0 * 4u
                                    for [unroll_full] (k in range(4)) {
                                        let u = unsafe(qp[k])
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
                                        let qp = unsafe(addr(kqu[blk * 16u + hf3 * 8u + il0 * 4u]))
                                        let nsh = gg3 * 2u
                                        for [unroll_full] (k in range(4)) {
                                            let u = unsafe(qp[k])
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
                                    kmask_scmn(sv, js, sc, mn)
                                    let dsc = float(ksh[blk * 8u]) * float(sc)
                                    let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
                                    let qp = unsafe(addr(kqu[blk * SB_UINTS + (js / 2u) * 8u + il0 * 4u]))
                                    static_if (QH) {
                                        let hp = unsafe(addr(kqu[blk * SB_UINTS + 32u + il0 * 4u]))
                                        let nsh = (js & 1u) * 4u
                                        for [unroll_full] (k in range(4)) {
                                            let u = unsafe(qp[k])
                                            let hu = unsafe(hp[k])
                                            for [unroll_full] (c in range(4)) {
                                                let q = ((u >> (8u * uint(c) + nsh)) & 15u) | (((hu >> (8u * uint(c) + js)) & 1u) << 4u)
                                                va[k * 4 + c] = float16(dsc * float(q) - dmn)
                                            }
                                        }
                                    } else {
                                        let nsh = (js & 1u) * 4u
                                        for [unroll_full] (k in range(4)) {
                                            let u = unsafe(qp[k])
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
            }
        }
    }
}

//! The dense K-quant prefill GEMM over every superblock and per-32 format as one template: the 32 x 64 simdgroup tile
//! walk around MetalKqTileDecodeT's decode. Q8 (blob walk) stays its own class.
[ |> template_struct_instance]
class template MetalKqMulMmK45T : MetalKqTileDecodeT {
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "mp*rows*4" @template_gate = "!SK" y : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "nsk*mp*rows*4" @template_gate = SK yk : array<float>   //!< `nsk` partial planes of mp x rows
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @template_constant SK : bool = false   //!< split-K: the grid's z threadgroups share a tile's K, each into its own partial plane
    override KQ = true
    override KQ_SC = true

    [metal_kernel]
    def metal_kq_mulmm {
        let lid = gl_LocalInvocationID.x
        kq_tables()
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
        let wb = (nBase + lr0) * (kdim / kq_unit())
        let kper = SK ? nkb / gl_NumWorkGroups.z : nkb
        let kbeg = SK ? gl_WorkGroupID.z * kper : 0u
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u + kbeg * 8u]))
        var kb = kbeg
        while (kb < kbeg + kper) {
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            kq_decode(wb, kb, il0, va)
            publish_a(va, tA0)
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            acc_quad(aB, bB, ma, mb, mc)
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        static_if (SK) {
            let plane = gl_WorkGroupID.z * gl_NumWorkGroups.x * 32u * ndim
            for [unroll_full] (i in range(8)) {
                simdgroup_store(mc[i], yk, plane + (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
            }
        } else {
            for [unroll_full] (i in range(8)) {
                simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mm_k4_c", pso = "g_pso_kq_mm_b4", tgmem = "MetalKqMulMmK4_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmK4 : MetalKqMulMmK45T {
    override SB_UINTS = 32u
    override QH = false
}

[metal_dispatch(name = "enc_kq_mm_k4_sk_c", pso = "g_pso_kq_mm_b4_sk", tgmem = "MetalKqMulMmK4Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmK4Sk : MetalKqMulMmK45T {
    override SB_UINTS = 32u
    override QH = false
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_k5_c", pso = "g_pso_kq_mm_b5", tgmem = "MetalKqMulMmK5_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmK5 : MetalKqMulMmK45T {
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(name = "enc_kq_mm_k5_sk_c", pso = "g_pso_kq_mm_b5_sk", tgmem = "MetalKqMulMmK5Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmK5Sk : MetalKqMulMmK45T {
    override SB_UINTS = 40u
    override QH = true
    override SK = true
}

// q6_K rides the SIXBIT arm; scalars reload per kb — the cached form cost an occupancy tier
// (896 vs 1024 max_threads; bench_metal_kq_mm_lab measured -1.7..-10% per shape).
[metal_dispatch(name = "enc_kq_mm_k6_c", pso = "g_pso_kq_mm_b6", tgmem = "MetalKqMulMmK6_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmK6 : MetalKqMulMmK45T {
    override SIXBIT = true
}

[metal_dispatch(name = "enc_kq_mm_k6_sk_c", pso = "g_pso_kq_mm_b6_sk", tgmem = "MetalKqMulMmK6Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmK6Sk : MetalKqMulMmK45T {
    override SIXBIT = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_iq4xs_c", pso = "g_pso_kq_mm_b44", tgmem = "MetalKqMulMmIq4xs_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq4xs : MetalKqMulMmK45T {
    override IQ4XS = true
}

[metal_dispatch(name = "enc_kq_mm_iq4xs_sk_c", pso = "g_pso_kq_mm_b44_sk", tgmem = "MetalKqMulMmIq4xsSk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq4xsSk : MetalKqMulMmK45T {
    override IQ4XS = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_iq3s4_c", pso = "g_pso_kq_mm_b35", tgmem = "MetalKqMulMmIq3s4_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq3s4 : MetalKqMulMmK45T {
    override IQ4XS = true
    override IQODD = true
}

[metal_dispatch(name = "enc_kq_mm_iq3s4_sk_c", pso = "g_pso_kq_mm_b35_sk", tgmem = "MetalKqMulMmIq3s4Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq3s4Sk : MetalKqMulMmK45T {
    override IQ4XS = true
    override IQODD = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_k3_c", pso = "g_pso_kq_mm_b3", tgmem = "MetalKqMulMmK3_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmK3 : MetalKqMulMmK45T {
    override K3 = true
}

[metal_dispatch(name = "enc_kq_mm_k3_sk_c", pso = "g_pso_kq_mm_b3_sk", tgmem = "MetalKqMulMmK3Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmK3Sk : MetalKqMulMmK45T {
    override K3 = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_iq3s_c", pso = "g_pso_kq_mm_b33", tgmem = "MetalKqMulMmIq3s_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq3s : MetalKqMulMmK45T {
    override IQ3S = true
}

[metal_dispatch(name = "enc_kq_mm_iq3s_sk_c", pso = "g_pso_kq_mm_b33_sk", tgmem = "MetalKqMulMmIq3sSk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq3sSk : MetalKqMulMmK45T {
    override IQ3S = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_iq3xxs_c", pso = "g_pso_kq_mm_b34", tgmem = "MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq3xxs : MetalKqMulMmK45T {
    override IQ3XXS = true
}

[metal_dispatch(name = "enc_kq_mm_iq3xxs_sk_c", pso = "g_pso_kq_mm_b34_sk", tgmem = "MetalKqMulMmIq3xxsSk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq3xxsSk : MetalKqMulMmK45T {
    override IQ3XXS = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_k2_c", pso = "g_pso_kq_mm_b2", tgmem = "MetalKqMulMmK2_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmK2 : MetalKqMulMmK45T {
    override K2 = true
}

[metal_dispatch(name = "enc_kq_mm_k2_sk_c", pso = "g_pso_kq_mm_b2_sk", tgmem = "MetalKqMulMmK2Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmK2Sk : MetalKqMulMmK45T {
    override K2 = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_iq2s_c", pso = "g_pso_kq_mm_b23", tgmem = "MetalKqMulMmIq2s_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq2s : MetalKqMulMmK45T {
    override IQ2S = true
}

[metal_dispatch(name = "enc_kq_mm_iq2s_sk_c", pso = "g_pso_kq_mm_b23_sk", tgmem = "MetalKqMulMmIq2sSk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq2sSk : MetalKqMulMmK45T {
    override IQ2S = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_iq2xs_c", pso = "g_pso_kq_mm_b24", tgmem = "MetalKqMulMmIq2xs_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq2xs : MetalKqMulMmK45T {
    override IQ2XS = true
}

[metal_dispatch(name = "enc_kq_mm_iq2xs_sk_c", pso = "g_pso_kq_mm_b24_sk", tgmem = "MetalKqMulMmIq2xsSk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq2xsSk : MetalKqMulMmK45T {
    override IQ2XS = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_iq2xxs_c", pso = "g_pso_kq_mm_b25", tgmem = "MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq2xxs : MetalKqMulMmK45T {
    override IQ2XXS = true
}

[metal_dispatch(name = "enc_kq_mm_iq2xxs_sk_c", pso = "g_pso_kq_mm_b25_sk", tgmem = "MetalKqMulMmIq2xxsSk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq2xxsSk : MetalKqMulMmK45T {
    override IQ2XXS = true
    override SK = true
}

// IQ4NL flips the scale read to q40's per-32-block d plane
[metal_dispatch(name = "enc_kq_mm_iq4nl_c", pso = "g_pso_kq_mm_b45", tgmem = "MetalKqMulMmIq4nl_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq4nl : MetalKqMulMmK45T {
    override IQ4XS = true
    override IQ4NL = true
}

[metal_dispatch(name = "enc_kq_mm_iq4nl_sk_c", pso = "g_pso_kq_mm_b45_sk", tgmem = "MetalKqMulMmIq4nlSk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq4nlSk : MetalKqMulMmK45T {
    override IQ4XS = true
    override IQ4NL = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_q40_c", pso = "g_pso_kq_mm_b40", tgmem = "MetalKqMulMmQ40_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmQ40 : MetalKqMulMmK45T {
    override IQ4XS = true
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(name = "enc_kq_mm_q40_sk_c", pso = "g_pso_kq_mm_b40_sk", tgmem = "MetalKqMulMmQ40Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmQ40Sk : MetalKqMulMmK45T {
    override IQ4XS = true
    override IQ4NL = true
    override Q40 = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_nv4_c", pso = "g_pso_kq_mm_b49", tgmem = "MetalKqMulMmNv4_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmNv4 : MetalKqMulMmK45T {
    override IQ4XS = true
    override NV4 = true
}

[metal_dispatch(name = "enc_kq_mm_nv4_sk_c", pso = "g_pso_kq_mm_b49_sk", tgmem = "MetalKqMulMmNv4Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmNv4Sk : MetalKqMulMmK45T {
    override IQ4XS = true
    override NV4 = true
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_q51_c", pso = "g_pso_kq_mm_b51", tgmem = "MetalKqMulMmQ51_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmQ51 : MetalKqMulMmK45T {
    override KQ_SC = false
    override B32 = true
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
}

[metal_dispatch(name = "enc_kq_mm_q51_sk_c", pso = "g_pso_kq_mm_b51_sk", tgmem = "MetalKqMulMmQ51Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmQ51Sk : MetalKqMulMmK45T {
    override KQ_SC = false
    override B32 = true
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override SK = true
}

[metal_dispatch(name = "enc_kq_mm_iq4nl32_c", pso = "g_pso_kq_mm_b4n32", tgmem = "MetalKqMulMmIq4nl32_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmIq4nl32 : MetalKqMulMmK45T {
    override KQ_SC = false
    override B32 = true
    override IQ4XS = true
}

[metal_dispatch(name = "enc_kq_mm_iq4nl32_sk_c", pso = "g_pso_kq_mm_b4n32_sk", tgmem = "MetalKqMulMmIq4nl32Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmIq4nl32Sk : MetalKqMulMmK45T {
    override KQ_SC = false
    override B32 = true
    override IQ4XS = true
    override SK = true
}

//! q4032: iq4nl32's per-32 walk with the linear code in the codebook slot (`Q40`)
[metal_dispatch(name = "enc_kq_mm_q4032_c", pso = "g_pso_kq_mm_b4032", tgmem = "MetalKqMulMmQ4032_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmQ4032 : MetalKqMulMmK45T {
    override KQ_SC = false
    override B32 = true
    override IQ4XS = true
    override Q40 = true
}

[metal_dispatch(name = "enc_kq_mm_q4032_sk_c", pso = "g_pso_kq_mm_b4032_sk", tgmem = "MetalKqMulMmQ4032Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmQ4032Sk : MetalKqMulMmK45T {
    override KQ_SC = false
    override B32 = true
    override IQ4XS = true
    override Q40 = true
    override SK = true
}

//! q5032: q51's per-32 quant walk under one d and the code q - 16 (`Q50`)
[metal_dispatch(name = "enc_kq_mm_q5032_c", pso = "g_pso_kq_mm_b5032", tgmem = "MetalKqMulMmQ5032_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64", requires = "mp % 32")]
class MetalKqMulMmQ5032 : MetalKqMulMmK45T {
    override KQ_SC = false
    override B32 = true
    override Q50 = true
    override B32W = 5u
}

[metal_dispatch(name = "enc_kq_mm_q5032_sk_c", pso = "g_pso_kq_mm_b5032_sk", tgmem = "MetalKqMulMmQ5032Sk_metal_kq_mulmm_msl_tgmem", tg = 128, grid = "mp/32, rows/64, nsk", params = "mp : int64, rows : int64, nsk : int64", requires = "mp % 32")]
class MetalKqMulMmQ5032Sk : MetalKqMulMmK45T {
    override KQ_SC = false
    override B32 = true
    override Q50 = true
    override B32W = 5u
    override SK = true
}

//! The router GEMV's Metal shell on the template both homes stamp (`GkRouterGemv`): the stamp's grid onto its rows - a threadgroup
//! of 256 a row under `ROW_WG`, else a simdgroup a row of the threadgroup's four; a format's template sets the row words
[ |> template_struct_instance]
class template MetalRouterGemvT : GkRouterGemv {
    def grid_row {
        static_if (ROW_WG) {
            router_row(gl_WorkGroupID.x, gl_LocalInvocationID.x, 256u)
        } else {
            let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
            if (row < pa.ne) {
                router_row(row, gl_SubgroupInvocationID, gl_SubgroupSize)
            }
        }
    }
}

//! the f32 router rows, a lane stepping a float4, the bias row where `hasb` (gpt-oss)
[ |> template_struct_instance]
class template MetalMoeRouterT : MetalRouterGemvT {
    typedef KT = float4
    typedef XT = float4
    override VW = 4u
    override BIAS = true
}

//! the per-layer-embedding pre-step's projection off its native-BF16 rows: the bf16 word pairs (the finish applies the 1/sqrt(dim))
[ |> template_struct_instance]
class template MetalPleProjBf16T : MetalRouterGemvT {
    typedef KT = uint
    typedef XT = float
    override VW = 2u
    override BF16 = true
}

//! the same rows a halfword a lane, for an odd dim or base: a row then starts mid-word, off the pairs' word grid
[ |> template_struct_instance]
class template MetalPleProjBf16HwT : MetalRouterGemvT {
    typedef KT = uint16
    typedef XT = float
    override BF16 = true
}

//! the halfword walk's decode-column stamp of the template both homes stamp (`GkRouterGemv`, through `MetalPleProjBf16HwT`),
//! a threadgroup of 256 a row
[metal_dispatch(name = "enc_ple_proj_bf16_hw_one", pso = "g_pso_ple_proj_bf16_hw_one", tgmem = "metal_ple_proj_bf16_hw_one_msl_tgmem", tg = 256, grid = "ne, nst", params = "ne : int64, nst : int64")]
class MetalPleProjBf16HwOne : MetalPleProjBf16HwT {
    override NC = 1

    [metal_kernel(name="metal_ple_proj_bf16_hw_one_msl")]
    def metal_ple_proj_bf16_hw_one {
        grid_row()
    }
}

//! the halfword walk's position-blocked stamp of the template both homes stamp (`GkRouterGemv`, through `MetalPleProjBf16HwT`),
//! a simdgroup a row and eight columns a simdgroup
[metal_dispatch(name = "enc_ple_proj_bf16_hw", pso = "g_pso_ple_proj_bf16_hw", tg = 128, grid = "ne/4, nst/8", params = "ne : int64, nst : int64")]
class MetalPleProjBf16Hw : MetalPleProjBf16HwT {
    override ROW_WG = false
    override COL_CLAMP = true
    override NC = 8

    [metal_kernel(name="metal_ple_proj_bf16_hw_msl")]
    def metal_ple_proj_bf16_hw {
        grid_row()
    }
}

//! the projection off the bf16 rows over `nst` columns: the word pairs where the dim and the base step whole words, else the
//! halfword walk; `blocked` the position-blocked stamps, else a threadgroup a row
def enc_ple_proj_bf16_rows(enc : MetalComputeEncoder?; bw, bx, by : MetalBuffer?; var ka : GkRouterArgs; ne, nst : int64; blocked : bool) {
    let pairs = gk_bf16_pairs(int64(ka.dim), int64(ka.woff))
    if (pairs && blocked) {
        enc_ple_proj_bf16(enc, bw, bx, by, ka, ne, nst)
    } elif (pairs) {
        enc_ple_proj_bf16_one(enc, bw, bx, by, ka, ne, nst)
    } elif (blocked) {
        enc_ple_proj_bf16_hw(enc, bw, bx, by, ka, ne, nst)
    } else {
        enc_ple_proj_bf16_hw_one(enc, bw, bx, by, ka, ne, nst)
    }
}

//! the f32 rows a lane an element, for a dim, base or column stride off the float4 lattice the f32 router's words step
[ |> template_struct_instance]
class template MetalF32RowsT : MetalRouterGemvT {
    typedef KT = float
    typedef XT = float
}

//! the f32 rows' one-column stamp of the template both homes stamp (`GkRouterGemv`, through `MetalF32RowsT`), a threadgroup
//! of 256 a row as the f32 router's
[metal_dispatch(name = "enc_f32_rows_one", pso = "g_pso_f32_rows_one", tgmem = "metal_f32_rows_one_msl_tgmem", tg = 256, grid = "ne, nst", params = "ne : int64, nst : int64")]
class MetalF32RowsOne : MetalF32RowsT {
    override NC = 1

    [metal_kernel(name="metal_f32_rows_one_msl")]
    def metal_f32_rows_one {
        grid_row()
    }
}

//! the f32 rows' position-blocked stamp of the template both homes stamp (`GkRouterGemv`, through `MetalF32RowsT`), a
//! simdgroup a row and eight columns a simdgroup
[metal_dispatch(name = "enc_f32_rows_b", pso = "g_pso_f32_rows_b", tg = 128, grid = "ne/4, nst/8", params = "ne : int64, nst : int64")]
class MetalF32RowsB : MetalF32RowsT {
    override ROW_WG = false
    override COL_CLAMP = true
    override NC = 8

    [metal_kernel(name="metal_f32_rows_b_msl")]
    def metal_f32_rows_b {
        grid_row()
    }
}

//! the f32 rows' GEMV over `nst` columns `dim` apart: the float4 router words (`MetalMoeRouter` / `B`, the bias row `brb`) where the
//! dim and the base step whole float4s, else the element walk; `blocked` the position-blocked stamps
def enc_f32_rows_gemv(enc : MetalComputeEncoder?; bw, bx, by, brb : MetalBuffer?; var ka : GkRouterArgs; ne, nst : int64; blocked : bool) {
    if (ka.dim % 4u == 0u && ka.woff % 4u == 0u) {
        if (blocked) {
            enc_moe_router_b(enc, bw, bx, by, brb, ka, ne, nst)
        } else {
            enc_moe_router(enc, bw, bx, by, brb, ka, ne, nst)
        }
    } elif (blocked) {
        enc_f32_rows_b(enc, bw, bx, by, ka, ne, nst)
    } else {
        enc_f32_rows_one(enc, bw, bx, by, ka, ne, nst)
    }
}

//! the one-column stamp: a threadgroup of 256 an expert row, grid.y the streams of a batched decode step (one on the single row)
[metal_dispatch(name = "enc_moe_router", pso = "g_pso_moe_router", tgmem = "metal_moe_router_msl_tgmem", tg = 256, grid = "ne, nst", params = "ne : int64, nst : int64", requires = "pa.dim % 4, pa.woff % 4, pa.xs % 4")]
class MetalMoeRouter : MetalMoeRouterT {
    override NC = 1

    [metal_kernel(name="metal_moe_router_msl")]
    def metal_moe_router {
        grid_row()
    }
}

//! the position-blocked prefill stamp: a simdgroup an expert row, eight positions a simdgroup (the prefill router's cost is the
//! rows' traffic); also the prefill's f32 GEMV
[metal_dispatch(name = "enc_moe_router_b", pso = "g_pso_moe_router_b", tg = 128, grid = "ne/4, nst/8", params = "ne : int64, nst : int64", requires = "pa.dim % 4, pa.woff % 4, pa.xs % 4")]
class MetalMoeRouterB : MetalMoeRouterT {
    override ROW_WG = false
    override COL_CLAMP = true
    override NC = 8

    [metal_kernel(name="metal_moe_router_b_msl")]
    def metal_moe_router_b {
        grid_row()
    }
}

//! the projection's position-blocked stamp of the template both homes stamp (`GkRouterGemv`): a simdgroup a row, eight columns a simdgroup
[metal_dispatch(name = "enc_ple_proj_bf16", pso = "g_pso_ple_proj_bf16", tg = 128, grid = "ne/4, nst/8", params = "ne : int64, nst : int64", requires = "pa.dim % 2, pa.woff % 2")]
class MetalPleProjBf16 : MetalPleProjBf16T {
    override ROW_WG = false
    override COL_CLAMP = true
    override NC = 8

    [metal_kernel(name="metal_ple_proj_bf16_msl")]
    def metal_ple_proj_bf16 {
        grid_row()
    }
}

//! the projection's decode-column stamp of the template both homes stamp (`GkRouterGemv`): a threadgroup of 256 a row, as the f32
//! router's one-column stamp reads its rows
[metal_dispatch(name = "enc_ple_proj_bf16_one", pso = "g_pso_ple_proj_bf16_one", tgmem = "metal_ple_proj_bf16_one_msl_tgmem", tg = 256, grid = "ne, nst", params = "ne : int64, nst : int64", requires = "pa.dim % 2, pa.woff % 2")]
class MetalPleProjBf16One : MetalPleProjBf16T {
    override NC = 1

    [metal_kernel(name="metal_ple_proj_bf16_one_msl")]
    def metal_ple_proj_bf16_one {
        grid_row()
    }
}

//! the pre-step's finish args: slices of `ple`, the projection scaled by 1 / sqrt(dim) in the finish
def ple_fin_args(ple, dim : int64; eps : float) : GkPleFinArgs => GkPleFinArgs(ple = uint(ple), eps = eps, inv_sd = 1.0 / sqrt(float(dim)))

//! The pre-step's finish on the template both homes stamp (`GkPleFinish`), a threadgroup of 256 a (row, layer) slice: the
//! projection scaled here, the inverse norm `rsqrt`
[metal_dispatch(name = "enc_ple_finish", pso = "g_pso_ple_finish", tgmem = "metal_ple_finish_msl_tgmem", tg = 256, grid = "nslices", params = "nslices : int64")]
class MetalPleFinish : GkPleFinish {
    override XSCALE = true
    override RSQRT = true

    [metal_kernel(name="metal_ple_finish_msl")]
    def metal_ple_finish {
        finish_slice(gl_WorkGroupID.x)
    }
}

//! The E-series pre-step on the device over `nrows` rows - the CPU's `ple_gather_rows` + `ple_pre_finish`: the side rows off the
//! table at the token ids in `btok` into `bple`, the embedded rows projected into `bproj` (`project`: the driver's projection form),
//! the finish; `ball` / `bsq` the layers x ple and sqrt(ple) uniforms
def enc_ple_pre(enc : MetalComputeEncoder?; t : Model; btok : MetalBuffer?; toff : uint64; bple, bproj, ball, bsq : MetalBuffer?; nrows : int64;
                project : block<() : void>) {
    let c = t.config
    let ple = c.n_embd_per_layer
    enc_embed_table(enc, t, t.ple_emb_fmt, t.ple_emb_off, c.n_layers * ple, sqrt(float(ple)), btok, toff, bple, 0ul, nrows, ball, bsq)
    invoke(project)
    var bpn = upload_region(unsafe(addr < void? >(t.fblob[t.ple_pnorm_off])), uint64(ple * 4l))
    var kf = ple_fin_args(ple, c.dim, c.norm_eps)
    enc_ple_finish(enc, bproj, bpn, bple, kf, c.n_layers * nrows)
}

//! the router GEMV's args: `ne` rows of `dim` over `ncols` columns `xs` apart (0 = dim), the bias row where `hasb`
def moe_router_args(ne, dim, ncols : int64; xs : int64 = 0l; hasb : bool = false) : GkRouterArgs
    => GkRouterArgs(ne = uint(ne), dim = uint(dim), ncols = uint(ncols), xs = uint(xs), hasb = hasb ? 1u : 0u)

//! The router select on the template both homes stamp (`GkTopKRow`): one simdgroup a stream, DEPTH logits a lane; lane 0 writes the
//! picks (`seli`), then their weights (`selw`, the float view): renormed under `norm`, or under `raw` (gpt-oss) the softmax over
//! the picks, then under the global and the per-expert down scales (`scale_weight`)
[ |> template_struct_instance]
class template MetalMoeSelectT : GkTopKRow {
    @ssbo @binding = 0 logits : array<float>                 //! [streams x lstride] router logits
    @ssbo @binding = 1 @role = "write" seli : array<uint>    //! the selection rows, uint view: stream s's picks at s 2k
    @ssbo @binding = 2 @role = "alias" selw : array<float>   //! the same rows' float view: stream s's weights at s 2k + k
    @ssbo @binding = 3 @role = "weight" @template_gate = DSCALE ds : array<float>   //! the layer's per-expert down-scale row (gemma-4)
    override LANES = 32u
    override DEPTH = 8

    def override logit_at(p, e : uint) : float => logits[p * pa.lstride + e]

    def override down_scale(e : uint) : float {
        static_if (DSCALE) {
            return ds[e]
        } else {
            return 1.0
        }
    }

    [metal_kernel]
    def metal_moe_select {
        let st = gl_WorkGroupID.x
        select_row(st, pa.ne, pa.k, pa.raw != 0u)
        if (gl_LocalInvocationID.x == 0u) {
            let sel0 = st * 2u * pa.k
            var mxw = -3.0e38
            var jj = 0u
            while (jj < pa.k) {
                seli[sel0 + jj] = min(pick[jj], pa.ne - 1u)   //! a NaN row leaves the sentinel: never a wild expert
                mxw = max(mxw, pickw[jj])
                jj++
            }
            var sw = 0.0
            if (pa.raw != 0u) {
                jj = 0u
                while (jj < pa.k) {
                    sw += exp(max(pickw[jj] - mxw, -80.0))
                    jj++
                }
            } else {
                pick_wsum_into(pa.k, sw)
            }
            jj = 0u
            while (jj < pa.k) {
                var wj = 0.0
                let ex = min(pick[jj], pa.ne - 1u)
                if (pa.raw != 0u) {
                    wj = exp(max(pickw[jj] - mxw, -80.0)) / sw
                    scale_weight(ex, pa.wscale, wj)
                } else {
                    pick_weight_into(jj, ex, pa.norm, sw, pa.wscale, wj)
                }
                selw[sel0 + pa.k + jj] = wj
                jj++
            }
        }
    }
}

[metal_dispatch(name = "enc_moe_select", pso = "g_pso_moe_select", tgmem = "MetalMoeSelect_metal_moe_select_msl_tgmem", tg = 32, grid = "nst", params = "nst : int64")]
class MetalMoeSelect : MetalMoeSelectT {
}

[metal_dispatch(name = "enc_moe_select_512", pso = "g_pso_moe_select_512", tgmem = "MetalMoeSelect512_metal_moe_select_msl_tgmem", tg = 32, grid = "nst", params = "nst : int64")]
class MetalMoeSelect512 : MetalMoeSelectT {
    override DEPTH = 16
}

//! the gemma-4 stamps: the down scales in the select (`ARCHITECTURE_GPU_SHARED_ROWS.md#moe-wscale` says why they are stamps of their own)
[metal_dispatch(name = "enc_moe_select_ds", pso = "g_pso_moe_select_ds", tgmem = "MetalMoeSelectDs_metal_moe_select_msl_tgmem", tg = 32, grid = "nst", params = "nst : int64")]
class MetalMoeSelectDs : MetalMoeSelectT {
    override DSCALE = true
    override DSOFF_SWITCH = false
}

[metal_dispatch(name = "enc_moe_select_512_ds", pso = "g_pso_moe_select_512_ds", tgmem = "MetalMoeSelect512Ds_metal_moe_select_msl_tgmem", tg = 32, grid = "nst", params = "nst : int64")]
class MetalMoeSelect512Ds : MetalMoeSelectT {
    override DEPTH = 16
    override DSCALE = true
    override DSOFF_SWITCH = false
}

// q4_K/q5_K kmask scale decode for ONE 32-element sub-block js (0..7): the 6-bit sc and min -
// the low four sub-blocks read their bytes whole, the high four assemble w's nibble with the
// y/z byte's top bits. Reference outputs: a uint2 return leaves a vector phi + pack/unpack in the AIR.
def public kmask_scmn(sv : uint4; js : uint; var sc, mn : uint&) {
    if (js < 4u) {
        sc = (sv.y >> (8u * js)) & 63u
        mn = (sv.z >> (8u * js)) & 63u
    } else {
        let j2 = js - 4u
        let hi = sv.w >> (8u * j2)
        sc = (hi & 15u) | (((sv.y >> (8u * j2 + 6u)) & 3u) << 4u)
        mn = ((hi >> 4u) & 15u) | (((sv.z >> (8u * j2 + 6u)) & 3u) << 4u)
    }
}

//! q4_K/q5_K sc/min for ONE half (ch) of the lane's quadrant. It IS `kmask_scmn(sv, jl + 4*ch)`,
//! and stays its own body: forwarding leaves the sub-block index a run-time value, so the
//! quadrant's byte extracts stop folding and seven mv kernels' AIR changes
def private kmask_sc_mn_ch(sv : uint4; jl : uint; ch : int; var sc, mn : uint&) {
    let sy = (sv.y >> (8u * jl)) & 255u
    let szv = (sv.z >> (8u * jl)) & 255u
    let sw = (sv.w >> (8u * jl)) & 255u
    sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
    mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
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

[grid_words(of = "iq3s_grid")]
def public iq3s_gw(i : int) : uint {}


def public iq3s_sx(sw, t : uint) : float => float((int((sw >> (t * 8u)) & 255u) ^ 128) - 128)

def public u8x4_f4(w : uint) : float4 => float4(float(w & 255u), float((w >> 8u) & 255u),
    float((w >> 16u) & 255u), float(w >> 24u))

// the HALVED iq3xxs grid (iq3xxs_grid, every byte / 2 - the plane convention), one
// word per call; the GEMV and mul_mm stage it, the mv twins read it directly.
[grid_words(of = "iq3xxs_grid")]
def public iq3xxs_gw(i : int) : uint {}


def public iq_sign_apply(m : float4; nib : uint) : float4 => float4((nib & 1u) != 0u ? -m.x : m.x, (nib & 2u) != 0u ? -m.y : m.y,
    (nib & 4u) != 0u ? -m.z : m.z, (nib & 8u) != 0u ? -m.w : m.w)

// the four signs of a grid word from its sign nibble: bit b set flips element b
def public iq_sign4(nib : uint) : float4 => iq_sign_apply(float4(1.0), nib)

// iq2xxs_grid as low/high word pairs (word 2i = magnitudes 0..3 of u64 entry i, word
// 2i+1 = 4..7); the same all-literal program-scope-constant form as its grid siblings.
[grid_words(of = "iq2xxs_grid2")]
def public iq2xxs_gw(i : int) : uint {}

// iq2xs_grid as low/high word pairs (word 2i = magnitudes 0..3 of u64 entry i, word
// 2i+1 = 4..7); the same all-literal program-scope-constant form as iq2s_gw below.
[grid_words(of = "iq2xs_grid2")]
def public iq2xs_gw(i : int) : uint {}

// iq2s_grid as low/high word pairs (word 2i = magnitudes 0..3 of u64 entry i, word 2i+1
// = 4..7); an all-literal fixed_array local, so it lowers to a program-scope constant table -
// every kernel reads it DIRECT (an f4 slab of 1024 entries would be 32 KB, past the tg budget).
[grid_words(of = "iq2s_grid2")]
def public iq2s_gw(i : int) : uint {}

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
                let sc = e8m0_half_f32(uint(mxe[blk]))
                sumf[r] += sc * ((am.x + am.y) + (am.z + am.w))
            }
            ib += 16u
        }
        let bp = unsafe(addr(wb[e * ddim + first_row]))
        for [unroll_full] (r in range(2)) {
            let sr = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                var outv = sr
                if (hasb != 0u) {
                    outv += unsafe(bp[r])
                }
                y[y0 + first_row + uint(r)] = outv
            }
        }
    }
}

//! the routed combine on the template both homes stamp (`GkMoeCombine`), a thread an element of a stream's row: the slots
//! at p k + j, their weights the selection rows' float view (a row's 2k words, the weights past the k picks)
[metal_dispatch(name = "enc_moe_combine", pso = "g_pso_moe_combine", tgmem = "metal_moe_combine_msl_tgmem", tg = 256, grid = "dim/256, nst", params = "dim : int64, nst : int64")]
class MetalMoeCombine : GkMoeCombine {
    override GATHERED = false
    override WINDOW = false

    [metal_kernel(name="metal_moe_combine_msl")]
    def metal_moe_combine {
        if (gl_GlobalInvocationID.x < pa.n) {
            comb_elem(gl_WorkGroupID.y, gl_GlobalInvocationID.x)
        }
    }
}

//! ... and its shared-expert stamp: the shared expert's rows under their gate logit's sigmoid added in the same pass
//! derives GkMoeCombine, the template both homes stamp
[metal_dispatch(name = "enc_moe_combine_sh", pso = "g_pso_moe_combine_sh", tgmem = "metal_moe_combine_sh_msl_tgmem", tg = 256, grid = "dim/256, nst", params = "dim : int64, nst : int64")]
class MetalMoeCombineSh : GkMoeCombine {
    override GATHERED = false
    override WINDOW = false
    override SH = true

    [metal_kernel(name="metal_moe_combine_sh_msl")]
    def metal_moe_combine_sh {
        if (gl_GlobalInvocationID.x < pa.n) {
            comb_elem(gl_WorkGroupID.y, gl_GlobalInvocationID.x)
        }
    }
}

//! The gemma-4 MoE step on the template both homes stamp (`GkMoeCombG4`): a threadgroup of 1024 lanes a row over the add-rms slab's
//! stash; the decode stamps read the selection rows' slots and store the next norm in f32 (`Hx`: with its f16 twin), the window's
//! (`Pf`) reads them through the bucket inverse and stores none (`ARCHITECTURE_GPU_PREFILL_MOE.md#metal-g4-moe-step`)
[ |> template_struct_instance]
class template MetalMoeCombG4T : GkMoeCombG4 {
    @workgroup xrow : float[4096]
    override TG = 1024u
    override RSQRT = true
    override GATHERED = false
    override OUT_F32 = true

    def override stash(k : uint; v : float) : void {
        xrow[k] = v
    }
    def override stashed(k : uint) : float => xrow[k]

    //! `moe_g4_norms`' layout: the four rows a row width apart
    def override rpost_base : uint => 0u
    def override dpost_base : uint => pa.n
    def override spost_base : uint => 2u * pa.n
    def override next_base : uint => 3u * pa.n
}

[metal_dispatch(name = "enc_moe_comb_g4", pso = "g_pso_moe_comb_g4", tgmem = "metal_moe_comb_g4_msl_tgmem", tg = 1024, grid = "nst", params = "nst : int64")]
class MetalMoeCombG4 : MetalMoeCombG4T {
    override PRESUM_SWITCH = false

    [metal_kernel(name="metal_moe_comb_g4_msl")]
    def metal_moe_comb_g4 {
        comb_g4_row(gl_WorkGroupID.x)
    }
}

[metal_dispatch(name = "enc_moe_comb_g4_hx", pso = "g_pso_moe_comb_g4_hx", tgmem = "metal_moe_comb_g4_hx_msl_tgmem", tg = 1024, grid = "nst", params = "nst : int64")]
class MetalMoeCombG4Hx : MetalMoeCombG4T {
    override HX = true
    override PRESUM_SWITCH = false

    [metal_kernel(name="metal_moe_comb_g4_hx_msl")]
    def metal_moe_comb_g4_hx {
        comb_g4_row(gl_WorkGroupID.x)
    }
}

[metal_dispatch(name = "enc_moe_comb_g4_pf", pso = "g_pso_moe_comb_g4_pf", tgmem = "metal_moe_comb_g4_pf_msl_tgmem", tg = 1024, grid = "npos", params = "npos : int64")]
class MetalMoeCombG4Pf : MetalMoeCombG4T {
    override GATHERED = true
    override OUT_F32 = false
    override SLOT_X4 = false   //! 121.4 us off against 136.9 on at 512 rows, `debug-jit` (ARCHITECTURE_GPU_SHARED_KERNELS.md's race provenance)

    [metal_kernel(name="metal_moe_comb_g4_pf_msl")]
    def metal_moe_comb_g4_pf {
        comb_g4_row(gl_WorkGroupID.x)
    }
}

//! the gemma-4 step's args over `nk` slots a row at width `dim`, the weights the selection rows' (`moe_comb_args`); the stamps read
//! the norm rows at `moe_g4_norms`' layout
def moe_g4_args(dim, nk : int64; eps, ascale : float; presummed : bool) : GkMoeCombArgs {
    var ka = moe_comb_args(dim, nk)
    ka.eps = eps
    ka.ascale = ascale
    ka.presummed = presummed ? 1u : 0u
    return ka
}

//! the gemma-4 step's args for layer l: the model's norm epsilon and the layer's output scale, `presummed` as `moe_g4_args`
def moe_g4_layer_args(t : Model; l : int64; presummed : bool) : GkMoeCombArgs {
    let c = t.config
    let osc = c.layer_out_scale ? t.fblob[t.out_scale_off + l] : 1.0
    return moe_g4_args(c.dim, c.n_expert_used, c.norm_eps, osc, presummed)
}

//! the combine's args over `nk` slots a stream at row width `dim`, the weights the selection rows' (the Metal stamps sum every
//! slot from zero: no window)
def moe_comb_args(dim, nk : int64; gstride : int64 = 1l; goff : int64 = 0l) : GkMoeCombArgs
    => GkMoeCombArgs(n = uint(dim), k = uint(nk), wstride = uint(2l * nk), woff = uint(nk), gstride = uint(gstride), goff = uint(goff))

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

//! the router select over `nrows` streams' logits into `bsel`, every picked weight under the model's `expert_weights_scale` and,
//! where `bds` is the layer's per-expert down-scale row (gemma-4), that expert's scale - the select's own fold, no pass after it
[arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#moe-wscale")]
def enc_moe_select_rows(enc : MetalComputeEncoder?; c : Config; blg, bsel : MetalBuffer?; nrows : int64; lstride : int64 = 0l;
                        bds : MetalBuffer? = null) {
    let sw = c.moe_gate == MoeGate.softmax_weight
    var ka = GkTopKArgs(ne = uint(c.n_expert), k = uint(c.n_expert_used), norm = !sw && c.norm_topk_prob ? 1u : 0u,
        wscale = c.expert_weights_scale, lstride = uint(lstride > 0l ? lstride : c.n_expert), raw = sw ? 1u : 0u)
    if (bds != null) {
        if (c.n_expert > 256l) {
            enc_moe_select_512_ds(enc, blg, bsel, bsel, bds, ka, nrows)
        } else {
            enc_moe_select_ds(enc, blg, bsel, bsel, bds, ka, nrows)
        }
    } elif (c.n_expert > 256l) {
        enc_moe_select_512(enc, blg, bsel, bsel, ka, nrows)
    } else {
        enc_moe_select(enc, blg, bsel, bsel, ka, nrows)
    }
}

//! The q8 dot's inner forms (four quants against a float4, a block's eight against two), spelled once: this sum order is the one every q8 kernel's parity holds.
def private q8_quad_dot(qv : array<int8>; qb : uint; x0 : float4) : float =>
    float(qv[qb]) * x0.x + float(qv[qb + 1u]) * x0.y + float(qv[qb + 2u]) * x0.z + float(qv[qb + 3u]) * x0.w

def private q8_oct_dot(qv : array<int8>; qb : uint; x0, x1 : float4) : float {
    return (float(qv[qb]) * x0.x + float(qv[qb + 1u]) * x0.y + float(qv[qb + 2u]) * x0.z + float(qv[qb + 3u]) * x0.w +
        float(qv[qb + 4u]) * x1.x + float(qv[qb + 5u]) * x1.y + float(qv[qb + 6u]) * x1.z + float(qv[qb + 7u]) * x1.w)
}

[ |> template_struct_instance]
class template MetalGemvSiteT {
    @template_constant GATHERED : bool = false
    //! the gathered x row lands in float4 units (the q8 body indexes x through a float4 view; every K body scales g[2] itself)
    @template_constant XROW4 : bool = false

    //! g[0] = n, g[1] = the expert's superblock shift, g[2] = the x row in the unit the body's x view indexes (float4 for the q8 body, scalar for the K bodies), g[3] = the y row
    [arch(at="../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-gemv-site")]
    def site_setup(var g : uint[8]) {
        static_if (GATHERED) {
            g[0] = ka.ndim
            let slot = gl_WorkGroupID.y
            let st = gl_WorkGroupID.z
            g[1] = sel[st * ka.ss + slot] * ka.eplane
            g[2] = st * ka.bxs + slot * ka.xs
            g[3] = st * ka.bys + slot * ka.ddim
            static_if (XROW4) {
                g[2] /= 4u
            }
        } else {
            g[0] = ndim
        }
    }

    //! d, the y rows - read where the reduce guard reads it, so the uniform load stays there
    def y_rows : uint {
        static_if (GATHERED) {
            return ka.ddim
        } else {
            return ddim
        }
    }
    def abstract x_at(i : uint) : float
    def y_put(i : uint; v : float) {
        y[i] = v
    }

    def put_rows2(first_row, y0, lane : uint; s0, s1 : float) {
        if (lane == 0u && first_row < y_rows()) {
            y_put(y0 + first_row, s0)
        }
        if (lane == 0u && first_row + 1u < y_rows()) {
            y_put(y0 + first_row + 1u, s1)
        }
    }
}

//! a Q5_1 lane's four weights: d * (q + 16 h) + m, the nibbles of `u` at `nsh` with the fifth bits in `qh`'s low four
def public b32_q51_w4(u, qh, nsh : uint; dm : float2) : float4 {
    let q = float4(float(((u >> nsh) & 15u) | ((qh & 1u) << 4u)), float(((u >> (8u + nsh)) & 15u) | ((qh & 2u) << 3u)),
        float(((u >> (16u + nsh)) & 15u) | ((qh & 4u) << 2u)), float(((u >> (24u + nsh)) & 15u) | ((qh & 8u) << 1u)))
    return mad(float4(dm.x), q, float4(dm.y))
}

//! Q5_1 / IQ4_NL per-32 GEMV over the verbatim disk planes, an 8-lane group per 32-block, 2 rows a simdgroup; MVB stamps the
//! small-batch form over the same decode and dot.
[ |> template_struct_instance]
class template MetalBlk32GemvT : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "qoff" wqu : array<uint>    //! quant plane, uint view (QW words a block)
    @ssbo @binding = 1 @role = "weight" @off = "soff" wsh : array<float16> //! scale plane (q51: d then m a block; iq4nl32: d)
    @uniform @binding = 4 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 5 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 4 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 6 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 6 @template_gate = MVB mv : KqMvArgs   //! the small-batch form's y row stride and live column count
    @workgroup @template_gate = "!Q51" lut : float[64]   //! kvalues_iq4nl as floats, one slot per thread of the first 64
    @workgroup @template_gate = KSPLIT part : float[64]   //! the K-split stamps' per-simdgroup partial sums, KS x RS x NC
    @template_constant Q51 : bool = false
    @template_constant Q40L : bool = false     //!< a Q4_0 per-32 plane (q4032): iq4nl32's block form under the linear code q - 8
    @template_constant Q50 : bool = false      //!< a Q5_0 per-32 plane (q5032): q51's quant block under one d, the 32-entry code q - 16
    @template_constant MVB : bool = false
    @template_constant KSPLIT : bool = false   //!< the narrow-plane stamps: the threadgroup's KS simdgroups split one row group's K
    @template_constant KS : uint = 1u          //!< simdgroups splitting K: 2 at tg 64, 4 at tg 128
    @template_constant KSI : int = 1           //!< KS's int spelling - override both together
    @template_constant NC : int = 8      //!< the small-batch form's x columns a threadgroup
    @template_constant NCU : uint = 8u   //!< NC's uint spelling - override both together
    @template_constant BE : uint = 32u   //!< weights a block: KQ_B32_ELEMS
    @template_constant QW : uint = 4u    //!< quant-plane words a block: KQ_B32_QWORDS, KQ_Q51_QWORDS on q51 (the qh word after the four)
    @template_constant SH : uint = 1u    //!< scale-plane halves a block: KQ_IQ4NL32_SBYTES / 2, KQ_Q51_SBYTES / 2 on q51 (d then m)
    @template_constant RS : uint = 2u    //!< rows a simdgroup (a K-split group): 1 on the narrow-plane stamps
    @template_constant RSI : int = 2     //!< RS's int spelling - override both together

    def lut_stage {
        static_if (!Q51) {
            let lid = gl_LocalInvocationID.x
            if (lid < 64u) {
                static_if (Q40L) {
                    lut[lid] = float(int(lid & 15u) - 8)
                } else {
                    static_if (Q50) {
                        lut[lid] = float(int(lid & 31u) - 16)
                    } else {
                        lut[lid] = iq4_lut(lid & 15u)
                    }
                }
            }
            barrier()
        }
    }

    //! the threadgroup's first row: a K-split group shares its rows across its simdgroups, else each simdgroup owns RS rows
    def first_row_of : uint {
        static_if (KSPLIT) {
            return gl_WorkGroupID.x * RS
        } else {
            return (gl_WorkGroupID.x * 2u + gl_SubgroupID) * RS
        }
    }

    //! the lane's first block: a K-split simdgroup starts its own four-block stride
    def ib0_of(ix : uint) : uint {
        static_if (KSPLIT) {
            return ix + 4u * gl_SubgroupID
        } else {
            return ix
        }
    }

    //! a lane's four weights off its nibble word `u` (word `ku` of a block's QW) at shift `nsh`: Q5_1's d * (q + 16 h) + m,
    //! IQ4_NL's LUT[q] (its d applies to the dot, `blk_dot`) - decoded once a block, then dotted against every x column
    def blk_w(u, qh, nsh : uint; sc : float2) : float4 {
        static_if (Q51) {
            return b32_q51_w4(u, qh, nsh, sc)
        } else {
            static_if (Q50) {
                return float4(lut[((u >> nsh) & 15u) | ((qh & 1u) << 4u)], lut[((u >> (8u + nsh)) & 15u) | ((qh & 2u) << 3u)],
                    lut[((u >> (16u + nsh)) & 15u) | ((qh & 4u) << 2u)], lut[((u >> (24u + nsh)) & 15u) | ((qh & 8u) << 1u)])
            } else {
                return float4(lut[(u >> nsh) & 15u], lut[(u >> (8u + nsh)) & 15u], lut[(u >> (16u + nsh)) & 15u], lut[(u >> (24u + nsh)) & 15u])
            }
        }
    }

    //! `acc` plus a lane's decoded weights against its four x: the one
    //! text the GEMV and the small-batch form both sum
    def blk_dot(w : float4; sc : float2; xv : float4; acc : float) : float {
        static_if (Q51) {
            return acc + dot4_seq(w, xv)
        } else {
            return mad(sc.x, dot4_seq(w, xv), acc)
        }
    }

    //! Q5_1's and Q5_0's fifth bits of the lane's four codes in the low four bits (the qh word after the nibble words); 0 elsewhere
    def blk_qh(blk, hb : uint) : uint {
        static_if (Q51 || Q50) {
            return wqu[blk * QW + QW - 1u] >> hb
        } else {
            return 0u
        }
    }

    //! the block's scales: Q5_1 (d, m), IQ4_NL (d, -)
    def blk_scale(blk : uint) : float2 {
        static_if (Q51) {
            return float2(float(wsh[blk * SH]), float(wsh[blk * SH + 1u]))
        } else {
            return float2(float(wsh[blk * SH]), 0.0)
        }
    }

    def gemv_body {
        var g : uint[8]
        site_setup(g)
        lut_stage()
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u    // 4-way block stride
        let it = lane % 8u    // 8 threads per block, one float4 (4 elems) each
        let nb = g[0] / BE
        let eb = g[1]
        let y0 = g[3]
        let first_row = first_row_of()
        // this thread's 4 elems: half = it/4 selects the nibble half, ku = it%4 the quant uint
        let half = it / 4u
        let ku = it % 4u
        let nsh = half * 4u
        let hb = ku * 4u + half * 16u
        var sumf : float[RSI]
        var ib = ib0_of(ix)
        while (ib < nb) {
            let xv = xf[g[2] + ib * 8u + it]
            for [unroll_full] (r in range(RSI)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let u = wqu[blk * QW + ku]
                let qh = blk_qh(blk, hb)
                let sc = blk_scale(blk)
                let w = blk_w(u, qh, nsh, sc)
                sumf[r] = blk_dot(w, sc, xv, sumf[r])
            }
            ib += 4u * KS
        }
        static_if (KSPLIT) {
            for [unroll_full] (r in range(RSI)) {
                let sr = simd_sum(sumf[r])
                if (lane == 0u) {
                    part[gl_SubgroupID * RS + uint(r)] = sr
                }
            }
            barrier()
            if (gl_SubgroupID == 0u && lane == 0u) {
                let pp = unsafe(addr(part[0]))
                for [unroll_full] (r in range(RSI)) {
                    var tot = unsafe(pp[r])
                    for [unroll_full] (k in range(1, KSI)) {
                        tot += unsafe(pp[k * RSI + r])
                    }
                    if (first_row + uint(r) < y_rows()) {
                        y_put(y0 + first_row + uint(r), tot)
                    }
                }
            }
        } else {
            for [unroll_full] (r in range(RSI)) {
                let sr = simd_sum(sumf[r])
                if (lane == 0u && first_row + uint(r) < y_rows()) {
                    y_put(y0 + first_row + uint(r), sr)
                }
            }
        }
    }

    //! one row's block against the NC columns: its weights decoded once (`blk_w`), dotted against each column's x as the GEMV dots
    def mvb_row(blk, ku, nsh, hb, xi : uint; xr : uint[NC]; var s : float[NC]) {
        let u = wqu[blk * QW + ku]
        let qh = blk_qh(blk, hb)
        let sc = blk_scale(blk)
        let w = blk_w(u, qh, nsh, sc)
        for [unroll_full] (c in range(NC)) {
            let xv = xf[xr[c] + xi]
            s[c] = blk_dot(w, sc, xv, s[c])
        }
    }

    //! the small-batch uniform's live column count and y row stride (the GEMV stamps bind none: one column)
    def mv_nr : uint {
        static_if (MVB) {
            return mv.nr
        } else {
            return 1u
        }
    }

    def mv_ys : uint {
        static_if (MVB) {
            return mv.ys
        } else {
            return 0u
        }
    }

    //! the small-batch form: the GEMV's lane map and row pair, `NC` x columns a column group (`gl_WorkGroupID.y`); a group's
    //! columns past the live ones read the last live column and store nothing
    [arch(at="../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-b32-small-batch")]
    def mvb_body {
        var g : uint[8]
        site_setup(g)
        lut_stage()
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let nb = g[0] / BE
        let first_row = first_row_of()
        let colbase = gl_WorkGroupID.y * NCU
        let half = it / 4u
        let ku = it % 4u
        let nsh = half * 4u
        let hb = ku * 4u + half * 16u
        var xr : uint[NC]
        for [unroll_full] (c in range(NC)) {
            xr[c] = min(colbase + uint(c), mv_nr() - 1u) * (g[0] / 4u) + it
        }
        var s0, s1 : float[NC]
        var ib = ib0_of(ix)
        while (ib < nb) {
            mvb_row(first_row * nb + ib, ku, nsh, hb, ib * 8u, xr, s0)
            static_if (RSI == 2) {
                mvb_row((first_row + 1u) * nb + ib, ku, nsh, hb, ib * 8u, xr, s1)
            }
            ib += 4u * KS
        }
        static_if (KSPLIT) {
            for [unroll_full] (c in range(NC)) {
                let a0 = simd_sum(s0[c])
                if (lane == 0u) {
                    part[(gl_SubgroupID * RS) * NCU + uint(c)] = a0
                }
                static_if (RSI == 2) {
                    let a1 = simd_sum(s1[c])
                    if (lane == 0u) {
                        part[(gl_SubgroupID * RS + 1u) * NCU + uint(c)] = a1
                    }
                }
            }
            barrier()
            if (gl_SubgroupID == 0u && lane == 0u) {
                let pp = unsafe(addr(part[0]))
                for [unroll_full] (r in range(RSI)) {
                    for [unroll_full] (c in range(NC)) {
                        var tot = unsafe(pp[r * NC + c])
                        for [unroll_full] (k in range(1, KSI)) {
                            tot += unsafe(pp[(k * RSI + r) * NC + c])
                        }
                        let col = colbase + uint(c)
                        if (col < mv_nr() && first_row + uint(r) < y_rows()) {
                            y_put(col * mv_ys() + first_row + uint(r), tot)
                        }
                    }
                }
            }
        } else {
            for [unroll_full] (c in range(NC)) {
                let a0 = simd_sum(s0[c])
                let col = colbase + uint(c)
                if (lane == 0u && col < mv_nr() && first_row < y_rows()) {
                    y_put(col * mv_ys() + first_row, a0)
                }
                static_if (RSI == 2) {
                    let a1 = simd_sum(s1[c])
                    if (lane == 0u && col < mv_nr() && first_row + 1u < y_rows()) {
                        y_put(col * mv_ys() + first_row + 1u, a1)
                    }
                }
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_q51_c", pso = "g_pso_kq_q51", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvQ51 : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" @span = "n*4" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override Q51 = true
    override QW = 5u
    override SH = 2u

    [metal_kernel(name="metal_kq_gemv_q51_msl", fastmath=false)]
    def metal_kq_gemv_q51 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_iq4nl32_c", pso = "g_pso_kq_iq4nl32", tgmem = "metal_kq_gemv_iq4nl32_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq4nl32 : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" @span = "n*4" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_iq4nl32_msl", fastmath=false)]
    def metal_kq_gemv_iq4nl32 {
        gemv_body()
    }
}

//! the narrow-plane GEMV stamps: one row a threadgroup, its four simdgroups splitting the row's K
[metal_dispatch(name = "enc_kq_q51_ks_c", pso = "g_pso_kq_q51_ks", tgmem = "metal_kq_gemv_q51_ks_msl_tgmem", tg = 128, grid = "rows", params = "rows : int64, n : int64")]
class MetalKqGemvQ51Ks : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" @span = "n*4" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override Q51 = true
    override QW = 5u
    override SH = 2u
    override RS = 1u
    override RSI = 1
    override KSPLIT = true
    override KS = 4u
    override KSI = 4

    [metal_kernel(name="metal_kq_gemv_q51_ks_msl", fastmath=false)]
    def metal_kq_gemv_q51_ks {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_iq4nl32_ks_c", pso = "g_pso_kq_iq4nl32_ks", tgmem = "metal_kq_gemv_iq4nl32_ks_msl_tgmem", tg = 128, grid = "rows", params = "rows : int64, n : int64")]
class MetalKqGemvIq4nl32Ks : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" @span = "n*4" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override RS = 1u
    override RSI = 1
    override KSPLIT = true
    override KS = 4u
    override KSI = 4

    [metal_kernel(name="metal_kq_gemv_iq4nl32_ks_msl", fastmath=false)]
    def metal_kq_gemv_iq4nl32_ks {
        gemv_body()
    }
}

//! Expert-indexed per-32 GEMVs: MetalBlk32GemvT's dot with the expert block shift; x and y ride the slot strides
[metal_dispatch(name = "enc_moe_gemv_q51_c", pso = "g_pso_moe_gemv_q51", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvQ51 : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override XROW4 = true
    override Q51 = true
    override QW = 5u
    override SH = 2u

    [metal_kernel(name="metal_moe_gemv_q51_msl")]
    def metal_moe_gemv_q51 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq4nl32_c", pso = "g_pso_moe_gemv_iq4nl32", tgmem = "metal_moe_gemv_iq4nl32_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq4nl32 : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override XROW4 = true

    [metal_kernel(name="metal_moe_gemv_iq4nl32_msl")]
    def metal_moe_gemv_iq4nl32 {
        gemv_body()
    }
}

//! the per-32 small-batch stamps: two, four and eight x columns a column group, x the live rows at `ndim` a row
[ |> template_struct_instance]
class template MetalB32MvbT : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" y : array<float>
    override MVB = true

    [metal_kernel(fastmath=false)]   //! the GEMV's math mode: no reassociation, so each column's sum is the GEMV's bit for bit
    def metal_b32_mvb {
        mvb_body()
    }
}

[metal_dispatch(name = "enc_b32_mvb2_q51_c", pso = "g_pso_b32_mvb2_q51", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB2Q51 : MetalB32MvbT {
    override Q51 = true
    override QW = 5u
    override SH = 2u
    override NC = 2
    override NCU = 2u
}

[metal_dispatch(name = "enc_b32_mvb4_q51_c", pso = "g_pso_b32_mvb4_q51", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB4Q51 : MetalB32MvbT {
    override Q51 = true
    override QW = 5u
    override SH = 2u
    override NC = 4
    override NCU = 4u
}

[metal_dispatch(name = "enc_b32_mvb8_q51_c", pso = "g_pso_b32_mvb8_q51", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB8Q51 : MetalB32MvbT {
    override Q51 = true
    override QW = 5u
    override SH = 2u
}

[metal_dispatch(name = "enc_b32_mvb2_iq4nl32_c", pso = "g_pso_b32_mvb2_iq4nl32", tgmem = "MetalB32MvB2Iq4nl32_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB2Iq4nl32 : MetalB32MvbT {
    override NC = 2
    override NCU = 2u
}

[metal_dispatch(name = "enc_b32_mvb4_iq4nl32_c", pso = "g_pso_b32_mvb4_iq4nl32", tgmem = "MetalB32MvB4Iq4nl32_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB4Iq4nl32 : MetalB32MvbT {
    override NC = 4
    override NCU = 4u
}

[metal_dispatch(name = "enc_b32_mvb8_iq4nl32_c", pso = "g_pso_b32_mvb8_iq4nl32", tgmem = "MetalB32MvB8Iq4nl32_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB8Iq4nl32 : MetalB32MvbT {}

//! the narrow-plane small-batch stamps: one row a threadgroup, its four simdgroups splitting the row's K
[ |> template_struct_instance]
class template MetalB32MvbNarrowT : MetalB32MvbT {
    override RS = 1u
    override RSI = 1
    override KSPLIT = true
    override KS = 4u
    override KSI = 4
}

[metal_dispatch(name = "enc_b32_mvb2_q51_ks_c", pso = "g_pso_b32_mvb2_q51_ks", tgmem = "MetalB32MvB2Q51Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB2Q51Ks : MetalB32MvbNarrowT {
    override Q51 = true
    override QW = 5u
    override SH = 2u
    override NC = 2
    override NCU = 2u
}

[metal_dispatch(name = "enc_b32_mvb4_q51_ks_c", pso = "g_pso_b32_mvb4_q51_ks", tgmem = "MetalB32MvB4Q51Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB4Q51Ks : MetalB32MvbNarrowT {
    override Q51 = true
    override QW = 5u
    override SH = 2u
    override NC = 4
    override NCU = 4u
}

[metal_dispatch(name = "enc_b32_mvb8_q51_ks_c", pso = "g_pso_b32_mvb8_q51_ks", tgmem = "MetalB32MvB8Q51Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB8Q51Ks : MetalB32MvbNarrowT {
    override Q51 = true
    override QW = 5u
    override SH = 2u
}

[metal_dispatch(name = "enc_b32_mvb2_iq4nl32_ks_c", pso = "g_pso_b32_mvb2_iq4nl32_ks", tgmem = "MetalB32MvB2Iq4nl32Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB2Iq4nl32Ks : MetalB32MvbNarrowT {
    override NC = 2
    override NCU = 2u
}

[metal_dispatch(name = "enc_b32_mvb4_iq4nl32_ks_c", pso = "g_pso_b32_mvb4_iq4nl32_ks", tgmem = "MetalB32MvB4Iq4nl32Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB4Iq4nl32Ks : MetalB32MvbNarrowT {
    override NC = 4
    override NCU = 4u
}

[metal_dispatch(name = "enc_b32_mvb8_iq4nl32_ks_c", pso = "g_pso_b32_mvb8_iq4nl32_ks", tgmem = "MetalB32MvB8Iq4nl32Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB8Iq4nl32Ks : MetalB32MvbNarrowT {}

[metal_dispatch(name = "enc_kq_q4032_c", pso = "g_pso_kq_q4032", tgmem = "metal_kq_gemv_q4032_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvQ4032 : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" @span = "n*4" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override Q40L = true

    [metal_kernel(name="metal_kq_gemv_q4032_msl", fastmath=false)]
    def metal_kq_gemv_q4032 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_q4032_ks_c", pso = "g_pso_kq_q4032_ks", tgmem = "metal_kq_gemv_q4032_ks_msl_tgmem", tg = 128, grid = "rows", params = "rows : int64, n : int64")]
class MetalKqGemvQ4032Ks : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" @span = "n*4" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override Q40L = true
    override RS = 1u
    override RSI = 1
    override KSPLIT = true
    override KS = 4u
    override KSI = 4

    [metal_kernel(name="metal_kq_gemv_q4032_ks_msl", fastmath=false)]
    def metal_kq_gemv_q4032_ks {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_q4032_c", pso = "g_pso_moe_gemv_q4032", tgmem = "metal_moe_gemv_q4032_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvQ4032 : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override XROW4 = true
    override Q40L = true

    [metal_kernel(name="metal_moe_gemv_q4032_msl")]
    def metal_moe_gemv_q4032 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_b32_mvb2_q4032_c", pso = "g_pso_b32_mvb2_q4032", tgmem = "MetalB32MvB2Q4032_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB2Q4032 : MetalB32MvbT {
    override Q40L = true
    override NC = 2
    override NCU = 2u
}

[metal_dispatch(name = "enc_b32_mvb4_q4032_c", pso = "g_pso_b32_mvb4_q4032", tgmem = "MetalB32MvB4Q4032_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB4Q4032 : MetalB32MvbT {
    override Q40L = true
    override NC = 4
    override NCU = 4u
}

[metal_dispatch(name = "enc_b32_mvb8_q4032_c", pso = "g_pso_b32_mvb8_q4032", tgmem = "MetalB32MvB8Q4032_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB8Q4032 : MetalB32MvbT {
    override Q40L = true
}

[metal_dispatch(name = "enc_b32_mvb2_q4032_ks_c", pso = "g_pso_b32_mvb2_q4032_ks", tgmem = "MetalB32MvB2Q4032Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB2Q4032Ks : MetalB32MvbNarrowT {
    override Q40L = true
    override NC = 2
    override NCU = 2u
}

[metal_dispatch(name = "enc_b32_mvb4_q4032_ks_c", pso = "g_pso_b32_mvb4_q4032_ks", tgmem = "MetalB32MvB4Q4032Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB4Q4032Ks : MetalB32MvbNarrowT {
    override Q40L = true
    override NC = 4
    override NCU = 4u
}

[metal_dispatch(name = "enc_b32_mvb8_q4032_ks_c", pso = "g_pso_b32_mvb8_q4032_ks", tgmem = "MetalB32MvB8Q4032Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB8Q4032Ks : MetalB32MvbNarrowT {
    override Q40L = true
}

[metal_dispatch(name = "enc_kq_q5032_c", pso = "g_pso_kq_q5032", tgmem = "metal_kq_gemv_q5032_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvQ5032 : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" @span = "n*4" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override Q50 = true
    override QW = 5u

    [metal_kernel(name="metal_kq_gemv_q5032_msl", fastmath=false)]
    def metal_kq_gemv_q5032 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_q5032_ks_c", pso = "g_pso_kq_q5032_ks", tgmem = "metal_kq_gemv_q5032_ks_msl_tgmem", tg = 128, grid = "rows", params = "rows : int64, n : int64")]
class MetalKqGemvQ5032Ks : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" @span = "n*4" xf : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override Q50 = true
    override QW = 5u
    override RS = 1u
    override RSI = 1
    override KSPLIT = true
    override KS = 4u
    override KSI = 4

    [metal_kernel(name="metal_kq_gemv_q5032_ks_msl", fastmath=false)]
    def metal_kq_gemv_q5032_ks {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_q5032_c", pso = "g_pso_moe_gemv_q5032", tgmem = "metal_moe_gemv_q5032_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvQ5032 : MetalBlk32GemvT {
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override XROW4 = true
    override Q50 = true
    override QW = 5u

    [metal_kernel(name="metal_moe_gemv_q5032_msl")]
    def metal_moe_gemv_q5032 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_b32_mvb2_q5032_c", pso = "g_pso_b32_mvb2_q5032", tgmem = "MetalB32MvB2Q5032_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB2Q5032 : MetalB32MvbT {
    override Q50 = true
    override QW = 5u
    override NC = 2
    override NCU = 2u
}

[metal_dispatch(name = "enc_b32_mvb4_q5032_c", pso = "g_pso_b32_mvb4_q5032", tgmem = "MetalB32MvB4Q5032_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB4Q5032 : MetalB32MvbT {
    override Q50 = true
    override QW = 5u
    override NC = 4
    override NCU = 4u
}

[metal_dispatch(name = "enc_b32_mvb8_q5032_c", pso = "g_pso_b32_mvb8_q5032", tgmem = "MetalB32MvB8Q5032_metal_b32_mvb_msl_tgmem", tg = 64, grid = "rows/4, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB8Q5032 : MetalB32MvbT {
    override Q50 = true
    override QW = 5u
}

[metal_dispatch(name = "enc_b32_mvb2_q5032_ks_c", pso = "g_pso_b32_mvb2_q5032_ks", tgmem = "MetalB32MvB2Q5032Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB2Q5032Ks : MetalB32MvbNarrowT {
    override Q50 = true
    override QW = 5u
    override NC = 2
    override NCU = 2u
}

[metal_dispatch(name = "enc_b32_mvb4_q5032_ks_c", pso = "g_pso_b32_mvb4_q5032_ks", tgmem = "MetalB32MvB4Q5032Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB4Q5032Ks : MetalB32MvbNarrowT {
    override Q50 = true
    override QW = 5u
    override NC = 4
    override NCU = 4u
}

[metal_dispatch(name = "enc_b32_mvb8_q5032_ks_c", pso = "g_pso_b32_mvb8_q5032_ks", tgmem = "MetalB32MvB8Q5032Ks_metal_b32_mvb_msl_tgmem", tg = 128, grid = "rows, gcols", params = "rows : int64, gcols : int64")]
class MetalB32MvB8Q5032Ks : MetalB32MvbNarrowT {
    override Q50 = true
    override QW = 5u
}

// One simdgroup per output row over the 34B q8 blob (f16 scale + 32 int8 inline, dual-viewed),
// 4 blocks in flight: the DnBa narrow-projection form (d = dt_rank 16..64, where the 2-rows/tg
// MetalQ8Gemv would starve the grid) and the expert-indexed MoE form; the site's x row is float4s.
[ |> template_struct_instance]
class template MetalQ8GemvT : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" @off = "boff" wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @uniform @binding = 4 @template_gate = "!GATHERED" ndim : uint       // n (reduction dim, % 32)
    @uniform @binding = 5 @template_gate = "!GATHERED" ddim : uint       // d (output rows)
    @uniform @binding = 4 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 6 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    def x4_at(i : uint) : float4 => x[i]

    //! the dot over the site g; x0 = the x row's float4 at the lane (a pointer parameter, since
    //! the emitter roots a pointer local at an addr(member[..]) of the kernel's own class)
    def gemv_body(g : uint[8]; var x0 : float4 const?) {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= y_rows()) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb = g[0] / 32u
        let nb4 = g[0] / 4u
        let wrow = g[1] + row * nb
        var sp = unsafe(addr(wsh[(wrow + lane / 8u) * 17u]))
        var qp = unsafe(addr(wqb[(wrow + lane / 8u) * 34u + 2u + (lane % 8u) * 4u]))
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
            let bi = wrow + k / 8u
            let d = float(wsh[bi * 17u])
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let xv = x4_at(g[2] + k)
            acc += q8_quad_dot(wqb, qb, xv) * d
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y_put(g[3] + row, acc)
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
                sumf[r] += q8_oct_dot(wqb, qb, x0, x1) * d
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

// Batched narrow projection (DeltaNet β/α: d = dt_rank, 16..64 — under every GEMM N tile):
// one simdgroup per (output row, position); 256/tg = 8 rows per threadgroup.
[metal_dispatch(name = "pf_enc_dn_ba", pso = "g_pso_dn_ba", tg = 256, grid = "d/8, np", params = "d : int64, np : int64")]
class MetalDnBa : MetalQ8GemvT {
    @ssbo @binding = 2 x : array<float4>    // [npos x n] f32 rows, float4 view
    @ssbo @binding = 3 @off = "yoff" @span = "d*np*4" y : array<float>     // [np x d] outputs

    [metal_kernel]
    def metal_q8_gemv {
        var g : uint[8]
        site_setup(g)
        gemv_body(g, unsafe(addr(x[g[2] + gl_SubgroupInvocationID])))
    }

    def override site_setup(var g : uint[8]) {
        g[0] = ndim
        g[2] = gl_WorkGroupID.y * (ndim / 4u)
        g[3] = gl_WorkGroupID.y * ddim
    }
}

[metal_dispatch(name = "enc_gemv_sg_c", pso = "g_pso_gemv_sg", tg = 256, grid = "d/8", params = "d : int64")]
class MetalQ8GemvSg : MetalQ8GemvT {
    @ssbo @binding = 2 @role = "read" @off = "xoff" x : array<float4>    // [n] f32 vector, float4 view
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "d*4" y : array<float>     // [d] outputs

    [metal_kernel]
    def metal_q8_gemv {
        var g : uint[8]
        site_setup(g)
        gemv_body(g, unsafe(addr(x[g[2] + gl_SubgroupInvocationID])))
    }
}

// Expert-indexed q8 GEMV — MetalQ8GemvT's dot with the expert plane picked GPU-side: the blob
// binds at the layer's expert STACK base, the kernel adds sel[slot] * eblk to the block index,
// reads x at slot * xs (0 = shared activation), writes y at slot * ddim.
[metal_dispatch(name = "enc_moe_gemv_q8_c", pso = "g_pso_moe_gemv_q8", tg = "g_gemv_rows*32", grid = "rows/g_gemv_rows, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvQ8 : MetalQ8GemvT {
    @ssbo @binding = 2 @role = "read" x : array<float4>
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override XROW4 = true

    [metal_kernel(name="metal_moe_gemv_q8_msl")]
    def metal_moe_gemv_q8 {
        var g : uint[8]
        site_setup(g)
        gemv_body(g, unsafe(addr(x[g[2] + gl_SubgroupInvocationID])))
    }
}

// Q4_K: lane owns 32 elems of every 4th superblock; the kmask halfword scale decode +
// nibble-mask acc folds (x16/x256 groups folded by pow2 constants). X4 reads x through a float4
// view: the scalar form was load-issue-bound at the routed expert shapes, the vector view 2.25x.
[ |> template_struct_instance]
class template MetalKqGemvK4T : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @uniform @binding = 5 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 6 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 5 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @template_constant X4 : bool = false


    def abstract x4_at(i : uint) : float4

    def gemv_body {   // nolint:STYLE038 — MSL kernel body; the gathered GEMV phases share tgmem state
        var g : uint[8]
        site_setup(g)
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = g[0] / 256u
        let eb = g[1]
        let xoff = g[2]
        let y0 = g[3]
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl, yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            var sumy : float[4]
            static_if (X4) {
                let yb4 = (xoff + ib * 256u + iq * 64u + ir * 8u) / 4u
                for [unroll_full] (h in range(4)) {   // h: yl lo/hi, yh lo/hi — float4 pairs at 0/8/32/40
                    let xi = yb4 + (h < 2 ? uint(h) * 8u : 16u + uint(h) * 8u)
                    let va = x4_at(xi)
                    let vb = x4_at(xi + 1u)
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
            } else {
                let yb = xoff + ib * 256u + iq * 64u + ir * 8u
                for [unroll_full] (i in range(8)) {
                    yl[i] = x_at(yb + uint(i))
                    sumy[0] += yl[i]
                    yl[i + 8] = x_at(yb + 32u + uint(i))
                    sumy[1] += yl[i + 8]
                    yh[i] = x_at(yb + 128u + uint(i))
                    sumy[2] += yh[i]
                    yh[i + 8] = x_at(yb + 160u + uint(i))
                    sumy[3] += yh[i + 8]
                }
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let scs = kmask_scales(ks4[blk], iq)
                let qb = blk * 32u + iq * 8u + ir * 2u
                let q1a = kqu[qb]
                let q1b = kqu[qb + 1u]
                let q2a = kqu[qb + 16u]
                let q2b = kqu[qb + 17u]
                var acc1, acc2 : float[4]
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
        let s0 = simd_sum(sumf[0])
        let s1 = simd_sum(sumf[1])
        put_rows2(first_row, y0, lane, s0, s1)
    }
}

[metal_dispatch(name = "enc_kq_k4_c", pso = "g_pso_kq_k4", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK4 : MetalKqGemvK4T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_k4_msl")]
    def metal_kq_gemv_k4 {
        gemv_body()
    }

    def override x_at(i : uint) : float => x[i]
}

// Expert-indexed Q4_K GEMV — MetalKqGemvK4T's dot with the expert superblock shift, x through
// the float4 view.
[metal_dispatch(name = "enc_moe_gemv_k4_c", pso = "g_pso_moe_kq_k4", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvK4 : MetalKqGemvK4T {
    @ssbo @binding = 3 @role = "read" x4 : array<float4>   // activations, float4 view (every x stride is % 4)
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override X4 = true

    [metal_kernel(name="metal_moe_gemv_k4_msl")]
    def metal_moe_gemv_k4 {
        gemv_body()
    }

    def override x4_at(i : uint) : float4 => x4[i]
}

// Q5_K: int-composed q5 (nibble | qh bit << 4) — one fma per element; same kmask scale decode.
// The quant plane is [qs 128][qh 32] per superblock, read through the byte view.
[ |> template_struct_instance]
class template MetalKqGemvK5T : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @uniform @binding = 5 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 6 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 5 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    def override x_at(i : uint) : float => x[i]

    def gemv_body {
        var g : uint[8]
        site_setup(g)
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hb0 = 2u * iq
        let nb = g[0] / 256u
        let eb = g[1]
        let xoff = g[2]
        let y0 = g[3]
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl, yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = xoff + ib * 256u + iq * 64u + l0
            var sumy : float[4]
            for [unroll_full] (l in range(8)) {
                yl[l] = x_at(yb + uint(l))
                sumy[0] += yl[l]
                yl[l + 8] = x_at(yb + 32u + uint(l))
                sumy[1] += yl[l + 8]
                yh[l] = x_at(yb + 128u + uint(l))
                sumy[2] += yh[l]
                yh[l + 8] = x_at(yb + 160u + uint(l))
                sumy[3] += yh[l + 8]
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let scs = kmask_scales(ks4[blk], iq)
                let qsp = unsafe(addr(kqb[blk * 160u + iq * 32u + l0]))
                let qhp = unsafe(addr(kqb[blk * 160u + 128u + l0]))
                var acc : float[4]
                for [unroll_full] (l in range(8)) {
                    let q1 = uint(unsafe(qsp[l]))
                    let q2 = uint(unsafe(qsp[64 + l]))
                    let h = uint(unsafe(qhp[l])) >> hb0
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
        let s0 = simd_sum(sumf[0])
        let s1 = simd_sum(sumf[1])
        put_rows2(first_row, y0, lane, s0, s1)
    }
}

[metal_dispatch(name = "enc_kq_k5_c", pso = "g_pso_kq_k5", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK5 : MetalKqGemvK5T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_k5_msl")]
    def metal_kq_gemv_k5 {
        gemv_body()
    }
}

// Expert-indexed Q5_K GEMV — MetalKqGemvK5T's dot with the expert superblock shift.
[metal_dispatch(name = "enc_moe_gemv_k5_c", pso = "g_pso_moe_kq_k5", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvK5 : MetalKqGemvK5T {
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_k5_msl")]
    def metal_moe_gemv_k5 {
        gemv_body()
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
        var yl, yh : float[16]
        var sumf = 0.0
        var ib = ix
        while (ib < nb) {
            let xp = unsafe(addr(x[ib * 256u + iq * 64u + l0]))
            var sumy : float[4]
            for [unroll_full] (l in range(8)) {
                yl[l] = unsafe(xp[l])
                sumy[0] += yl[l]
                yl[l + 8] = unsafe(xp[32 + l])
                sumy[1] += yl[l + 8]
                yh[l] = unsafe(xp[128 + l])
                sumy[2] += yh[l]
                yh[l + 8] = unsafe(xp[160 + l])
                sumy[3] += yh[l + 8]
            }
            let blk = first_row * nb + ib
            let scs = kmask_scales(ks4[blk], iq)
            let qsp = unsafe(addr(kqb[blk * 160u + iq * 32u + l0]))
            let qhp = unsafe(addr(kqb[blk * 160u + 128u + l0]))
            var acc1, acc2 : float[4]
            for [unroll_full] (l in range(8)) {
                let q1 = uint(unsafe(qsp[l]))
                let q2 = uint(unsafe(qsp[64 + l]))
                let h = uint(unsafe(qhp[l]))
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
[ |> template_struct_instance]
class template MetalKqGemvK6T : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k6 quant plane, byte view (ql at 192*blk, qh at +128)
    @uniform @binding = 5 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 6 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 5 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    def override x_at(i : uint) : float => x[i]

    def gemv_body {
        var g : uint[8]
        site_setup(g)
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let is0 = ip * 8u + l0 / 16u
        let nb = g[0] / 256u
        let eb = g[1]
        let xoff = g[2]
        let y0 = g[3]
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = xoff + ib * 256u + ip * 128u + l0
            for [unroll_full] (l in range(4)) {
                yl[4 * l] = x_at(yb + uint(l))
                yl[4 * l + 1] = x_at(yb + 32u + uint(l))
                yl[4 * l + 2] = x_at(yb + 64u + uint(l))
                yl[4 * l + 3] = x_at(yb + 96u + uint(l))
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let qlp = unsafe(addr(kqb[blk * 192u + ip * 64u + l0]))
                let qhp = unsafe(addr(kqb[blk * 192u + 128u + ip * 32u + l0]))
                let sb = blk * 16u + is0
                var sums : float[4]
                for [unroll_full] (l in range(4)) {
                    let q1 = uint(unsafe(qlp[l]))
                    let q2 = uint(unsafe(qlp[32 + l]))
                    let h = uint(unsafe(qhp[l]))
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
        let s0 = simd_sum(sumf[0])
        let s1 = simd_sum(sumf[1])
        put_rows2(first_row, y0, lane, s0, s1)
    }
}

[metal_dispatch(name = "enc_kq_k6_c", pso = "g_pso_kq_k6", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK6 : MetalKqGemvK6T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_k6_msl")]
    def metal_kq_gemv_k6 {
        gemv_body()
    }
}

// Expert-indexed Q6_K GEMV — MetalKqGemvK6T's dot; the expert shift covers the sub-scales AND
// the trailing f16 d plane (both indexed by the same region-relative superblock).
[metal_dispatch(name = "enc_moe_gemv_k6_c", pso = "g_pso_moe_kq_k6", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvK6 : MetalKqGemvK6T {
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_k6_msl")]
    def metal_moe_gemv_k6 {
        gemv_body()
    }
}

// Q2_K: the k3 GEMV's lane map minus the hmask - unsigned 2-bit lanes, per-16 sc/min NIBBLE
// pairs off the split strips (sc scales the dot, mn folds on the lane's x sums against dmin),
// d + dmin off the 4B tail plane; 4 rows a simdgroup.
[ |> template_struct_instance]
class template MetalKqGemvK2T : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d+dmin tail - the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B pair-byte strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k2 quant plane, uint view (qs at 16*blk)
    @uniform @binding = 5 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 6 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 5 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    def gemv_body {
        var g : uint[8]
        site_setup(g)
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let isb = ir / 2u
        let nb = g[0] / 256u
        let eb = g[1]
        let y0 = g[3]
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 4u
        var yl : float[32]
        var sumf : float[4]
        var ib = ix
        while (ib < nb) {
            let xp = unsafe(addr(x[g[2] + ib * 256u + iq * 128u + ir * 8u]))
            var sumy : float[4]
            for [unroll_full] (i in range(8)) {
                yl[i] = unsafe(xp[i])
                sumy[0] += yl[i]
                yl[i + 8] = unsafe(xp[32 + i])
                sumy[1] += yl[i + 8]
                yl[i + 16] = unsafe(xp[64 + i])
                sumy[2] += yl[i + 16]
                yl[i + 24] = unsafe(xp[96 + i])
                sumy[3] += yl[i + 24]
            }
            for [unroll_full] (r in range(4)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let qb = blk * 16u + iq * 8u + ir * 2u
                let qa = kqu[qb]
                let qc = kqu[qb + 1u]
                var acc1, acc2 : float[4]
                for [unroll_full] (i in range(4)) {
                    let q16 = ((i < 2 ? qa : qc) >> (16u * (uint(i) & 1u))) & 0xFFFF
                    acc1[0] += yl[2 * i] * float(q16 & 0x0003)
                    acc2[0] += yl[2 * i + 1] * float(q16 & 0x0300)
                    acc1[1] += yl[2 * i + 8] * float(q16 & 0x000C)
                    acc2[1] += yl[2 * i + 9] * float(q16 & 0x0C00)
                    acc1[2] += yl[2 * i + 16] * float(q16 & 0x0030)
                    acc2[2] += yl[2 * i + 17] * float(q16 & 0x3000)
                    acc1[3] += yl[2 * i + 24] * float(q16 & 0x00C0)
                    acc2[3] += yl[2 * i + 25] * float(q16 & 0xC000)
                }
                let sb = blk * 16u + iq * 8u + isb
                let b0 = uint(kscb[sb])
                let b2 = uint(kscb[sb + 2u])
                let b4 = uint(kscb[sb + 4u])
                let b6 = uint(kscb[sb + 6u])
                let dall = float(kdh[blk * 2u])
                let dmn = float(kdh[blk * 2u + 1u]) * 0.0625
                sumf[r] += dall * ((acc1[0] + 0.00390625 * acc2[0]) * float(b0 & 15u)
                    + (acc1[1] + 0.00390625 * acc2[1]) * float(b2 & 15u) * 0.25
                    + (acc1[2] + 0.00390625 * acc2[2]) * float(b4 & 15u) * 0.0625
                    + (acc1[3] + 0.00390625 * acc2[3]) * float(b6 & 15u) * 0.015625)
                sumf[r] -= dmn * (sumy[0] * float(b0 & 0xF0) + sumy[1] * float(b2 & 0xF0)
                    + sumy[2] * float(b4 & 0xF0) + sumy[3] * float(b6 & 0xF0))
            }
            ib += 4u
        }
        for [unroll_full] (r in range(4)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < y_rows()) {
                y_put(y0 + first_row + uint(r), s)
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_k2_c", pso = "g_pso_kq_k2", tg = 64, grid = "rows/8", params = "rows : int64, n : int64")]
class MetalKqGemvK2 : MetalKqGemvK2T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_k2_msl")]
    def metal_kq_gemv_k2 {
        gemv_body()
    }
}

//! Expert-indexed Q2_K GEMV - MetalKqGemvK2T's dot with the expert superblock shift
[metal_dispatch(name = "enc_moe_gemv_k2_c", pso = "g_pso_moe_kq_k2", tg = 64, grid = "rows/8, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvK2 : MetalKqGemvK2T {
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_k2_msl")]
    def metal_moe_gemv_k2 {
        gemv_body()
    }
}


// LANES picks the map lane_setup lays into lc[] - 23 = k2/k3, 45 = k4/k5, 6 = k6, 0 = none; an mv_superblock reads the slots back by position
[ |> template_struct_instance]
class template MetalKqLaneMapsT {
    @template_constant LANES : int = 0

    def lane_setup(tx : uint; var lc : uint[8]) {
        static_if (LANES == 23) {
            lc[0] = tx / 2u             // s
            lc[1] = (tx % 2u) * 4u      // o4
            lc[2] = 2u * lc[0]          // shr
        }
        static_if (LANES == 45) {
            lc[0] = tx / 4u                        // q4i
            lc[1] = 2u * lc[0] + (tx % 4u) / 2u    // jl
            lc[2] = (tx % 2u) * 4u                 // lo
            lc[3] = (tx % 4u) < 2u ? 0u : 4u       // nsh
        }
        static_if (LANES == 6) {
            lc[0] = tx / 2u                     // s
            lc[1] = (tx % 2u) * 4u              // o4: 16B offset within the slot, in uints
            lc[2] = (lc[0] % 2u) * 8u + lc[1]   // qoff: ql uint offset within the 128-half's 16 uints
            lc[3] = lc[0] < 2u ? 0u : 4u        // shr
            lc[4] = 2u * lc[0]                  // hsh
        }
    }
}

//! The x panel every K-quant small-batch form reads: the f32 rows, or under XHALF the f16 twin the row producer
//! stores beside x (half the x bytes a weight costs - the bound on an L1-bound box). Both views bind at slot 3,
//! so a half stamp is its f32 form's class plus `override XHALF = true`, bound with the twin at that slot.
[ |> template_struct_instance]
class template MetalKqXT : MetalKqLaneMapsT {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @template_gate = "!XHALF" x : array<float4>
    @ssbo @binding = 3 @role = "read" @off = "xoff" @template_gate = XHALF xh : array<half4>
    @template_constant XHALF : bool = false
    @template_constant KQ_SB : uint = 256u   //! the superblock width: a kernel body reads no module constant, so the family's one home for it is here

    def x4_at(i : uint) : float4 {
        static_if (XHALF) {
            return float4(xh[i])
        } else {
            return x[i]
        }
    }
}

// The K-quant small-batch mv shell: a format's override decodes lane tx's weights of one superblock
// and accumulates its x columns into sumf; the shell walks the row and reduces the eight lanes. TILED
// branch-duplicates the writeback: a LIVE colbase on B2 costs +2% (k4) / +0.5% (k6) at cls (gemv bench).
[ |> template_struct_instance]
class template MetalKqMvShellT : MetalKqXT {
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NR : int = 2
    @template_constant NRU : uint = 2u  // NR's uint spelling (colbase math) — override BOTH together
    @template_constant TILED : bool = false

    //! the format's per-lane constants, computed once before the row walk (lc[] keeps them in the
    //! kernel's entry block, where a hand-written kernel has them)

    def abstract mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) : void

    def accum_cols32(cx4, colbase, nb4 : uint; w : float4[8]; var sumf : float[NR]) {
        for [unroll_full] (b in range(NR)) {
            var rowb = uint(b)    // nolint:LINT003,LINT019 — the TILED stamp alone reassigns it
            static_if (TILED) {
                rowb = colbase + uint(b)
            }
            let xi = rowb * nb4 + cx4
            var acc = 0.0
            static_if (XHALF) {
                let xp = unsafe(addr(xh[xi]))
                for [unroll_full] (u in range(8)) {
                    let v = float4(unsafe(xp[u]))
                    acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                }
            } else {
                let xp = unsafe(addr(x[xi]))
                for [unroll_full] (u in range(8)) {
                    let v = unsafe(xp[u])
                    acc += v.x * w[u].x + v.y * w[u].y + v.z * w[u].z + v.w * w[u].w
                }
            }
            sumf[b] += acc
        }
    }

    def accum_cols16(cx4, colbase, nb4 : uint; w : float[16]; var sumf : float[NR]) {
        for [unroll_full] (b in range(NR)) {
            var rowb = uint(b)    // nolint:LINT003,LINT019 — the TILED stamp alone reassigns it
            static_if (TILED) {
                rowb = colbase + uint(b)
            }
            let xi = rowb * nb4 + cx4
            var acc = 0.0
            static_if (XHALF) {
                let xp = unsafe(addr(xh[xi]))
                for [unroll_full] (u in range(4)) {
                    let v = float4(unsafe(xp[u]))
                    acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                }
            } else {
                let xp = unsafe(addr(x[xi]))
                for [unroll_full] (u in range(4)) {
                    let v = unsafe(xp[u])
                    acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                }
            }
            sumf[b] += acc
        }
    }

    def run_shell {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * NRU
        let nb = ka.ndim / 256u
        var lc : uint[8]
        lane_setup(tx, lc)
        let nb4 = ka.ndim / 4u
        var sumf : float[NR]
        var sb = 0u
        while (sb < nb) {
            mv_superblock(sb, row * nb + sb, tx, colbase, nb4, lc, sumf)
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

// The k2 B2/B4 pair: the K3T shell minus the hmask - w = q*d*sc - dmin*mn per element, the
// pair byte picked per (slot, half).
[ |> template_struct_instance]
class template MetalKqMvK2T : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d+dmin tail - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B pair-byte strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k2 quant plane, uint view (16 qs words per superblock)

    [metal_kernel]
    def metal_kq_mv_k2 {
        run_shell()
    }

    override LANES = 23

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let s = lc[0]
        let o4 = lc[1]
        let shr = lc[2]
        let dd = float(kdh[blk * 2u])
        let dmn = float(kdh[blk * 2u + 1u])
        for [unroll_full] (ch in range(2)) {
            let ip = uint(ch)
            let qp = unsafe(addr(kqu[blk * 16u + ip * 8u + o4]))
            let g = ip * 8u + s * 2u + (tx % 2u)
            let pb = uint(kscb[blk * 16u + g])
            let dl = dd * float(pb & 15u)
            let ml = dmn * float(pb >> 4u)
            var w : float[16]
            for [unroll_full] (u in range(4)) {
                let qu = unsafe(qp[u])
                let qv = (qu >> shr) & 0x03030303
                for [unroll_full] (e in range(4)) {
                    w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                }
            }
            let cx4 = sb * 64u + ip * 32u + s * 8u + o4
            accum_cols16(cx4, colbase, nb4, w, sumf)
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k2_c", pso = "g_pso_kq_mvb2_k2", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K2 : MetalKqMvK2T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_k2_c", pso = "g_pso_kq_mvb4_k2", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K2 : MetalKqMvK2T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// The K-quant eight-column mv shell: one superblock's X panel staged per threadgroup between two
// barriers, a format's override decodes lane tx's weights of the superblock and accumulates the
// eight staged columns into acc, and the shell reduces the eight lanes.
[ |> template_struct_instance]
class template MetalKqMvB8ShellT : MetalKqXT {
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel
    @template_constant PANEL_COLS : int = 8   //!< float4 columns one accum_panel call folds

    //! the format's per-lane constants, computed once before the row walk

    def abstract mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) : void

    //! the staged X panel's eight streams times one lane's decoded weights - the column run a
    //! lane decodes at once, which is also the weight array's extent
    def accum_panel(cxl : uint; w : float4[PANEL_COLS]; var acc : float4[8]) {
        for [unroll_full] (b in range(8)) {
            for [unroll_full] (u in range(PANEL_COLS)) {
                acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w[u]
            }
        }
    }

    def run_shell {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 8u
        let nb = ka.ndim / 256u
        var lc : uint[8]
        lane_setup(tx, lc)
        let nb4 = ka.ndim / 4u
        var acc : float4[8]
        var sb = 0u
        while (sb < nb) {
            barrier()
            for [unroll_full] (j in range(8)) {
                let slot = uint(j) * 64u + lid
                let xrow = min(colbase + uint(j), ka.nr - 1u)   //! a group's columns past the live rows restage the last live one
                txp[slot] = x4_at(xrow * nb4 + sb * 64u + (slot % 64u))
            }
            barrier()
            mvb8_superblock(row * nb + sb, tx, lc, acc)
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var sm = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                y[(colbase + uint(b)) * ka.ys + row] = sm
            }
        }
    }
}

// the k2 B8 twin: the k3 B8 shell minus the hmask, per-element w = q*d*sc - dmin*mn
[ |> template_struct_instance]
class template MetalKqMvB8K2T : MetalKqMvB8ShellT {
    override PANEL_COLS = 4
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d+dmin tail - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B pair-byte strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k2 quant plane, uint view

    [metal_kernel]
    def metal_kq_mvb8_k2 {
        run_shell()
    }

    override LANES = 23

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        let s = lc[0]
        let o4 = lc[1]
        let shr = lc[2]
        let dd = float(kdh[blk * 2u])
        let dmn = float(kdh[blk * 2u + 1u])
        for [unroll_full] (ch in range(2)) {
            let ip = uint(ch)
            let qp = unsafe(addr(kqu[blk * 16u + ip * 8u + o4]))
            let g = ip * 8u + s * 2u + (tx % 2u)
            let pb = uint(kscb[blk * 16u + g])
            let dl = dd * float(pb & 15u)
            let ml = dmn * float(pb >> 4u)
            var w4 : float4[4]
            for [unroll_full] (u in range(4)) {
                let qvc = (unsafe(qp[u]) >> shr) & 0x03030303
                w4[u] = u8x4_f4(qvc) * dl - float4(ml)
            }
            accum_panel(ip * 32u + s * 8u + o4, w4, acc)
        }
    }
}

// Q3_K: k6's lane map (a lane's 4 elements of each of a half's 4 blocks) over the k3 planes — the
// 2-bit lane of qs byte ip*32 + l at shift 2j | hmask bit 4ip + j of byte l, minus 4; per-16 SIGNED
// sub-scales off the split strips, d off the d plane.
[ |> template_struct_instance]
class template MetalKqGemvK3T : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqb : array<uint8>   // k3 quant plane, byte view (qs at 96*blk, hmask at +64)
    @uniform @binding = 5 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 6 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 5 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)

    def override x_at(i : uint) : float => x[i]

    def gemv_body {
        var g : uint[8]
        site_setup(g)
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let is0 = ip * 8u + l0 / 16u
        let hb0 = ip * 4u
        let nb = g[0] / 256u
        let eb = g[1]
        let xoff = g[2]
        let y0 = g[3]
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = xoff + ib * 256u + ip * 128u + l0
            for [unroll_full] (l in range(4)) {
                yl[4 * l] = x_at(yb + uint(l))
                yl[4 * l + 1] = x_at(yb + 32u + uint(l))
                yl[4 * l + 2] = x_at(yb + 64u + uint(l))
                yl[4 * l + 3] = x_at(yb + 96u + uint(l))
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let qsp = unsafe(addr(kqb[blk * 96u + ip * 32u + l0]))
                let hmp = unsafe(addr(kqb[blk * 96u + 64u + l0]))
                let sb = blk * 16u + is0
                var sums : float[4]
                for [unroll_full] (l in range(4)) {
                    let q = uint(unsafe(qsp[l]))
                    let h = uint(unsafe(hmp[l])) >> hb0
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
        let s0 = simd_sum(sumf[0])
        let s1 = simd_sum(sumf[1])
        put_rows2(first_row, y0, lane, s0, s1)
    }
}

[metal_dispatch(name = "enc_kq_k3_c", pso = "g_pso_kq_k3", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvK3 : MetalKqGemvK3T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_k3_msl")]
    def metal_kq_gemv_k3 {
        gemv_body()
    }
}

// Expert-indexed Q3_K GEMV — MetalKqGemvK3T's dot with the expert superblock shift
[metal_dispatch(name = "enc_moe_gemv_k3_c", pso = "g_pso_moe_kq_k3", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvK3 : MetalKqGemvK3T {
    @ssbo @binding = 3 @role = "read" x : array<float>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_k3_msl")]
    def metal_moe_gemv_k3 {
        gemv_body()
    }
}

// The k3 B2/B4 pair: the K6T shell (slot s = block of the half, o4 = its 16-half) with the
// lane + mask compose off the 24-uint superblock (qs 0..15, hmask 16..23), offset 4.
[ |> template_struct_instance]
class template MetalKqMvK3T : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k3 quant plane, uint view (qs at 24*blk, hmask at 24*blk+16)

    [metal_kernel]
    def metal_kq_mv_k3 {
        run_shell()
    }

    override LANES = 23

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let s = lc[0]
        let o4 = lc[1]
        let shr = lc[2]
        let dd = float(kdh[blk])
        for [unroll_full] (ch in range(2)) {
            let ip = uint(ch)
            let qp = unsafe(addr(kqu[blk * 24u + ip * 8u + o4]))
            let hp = unsafe(addr(kqu[blk * 24u + 16u + o4]))
            let hsh = ip * 4u + s
            let g = ip * 8u + s * 2u + (tx % 2u)
            let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
            let dl = dd * sc
            let ml = dl * 4.0
            var w : float[16]
            for [unroll_full] (u in range(4)) {
                let qu = unsafe(qp[u])
                let hu = unsafe(hp[u])
                let qv = ((qu >> shr) & 0x03030303) | (((hu >> hsh) & 0x01010101) << 2u)
                for [unroll_full] (e in range(4)) {
                    w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                }
            }
            let cx4 = sb * 64u + ip * 32u + s * 8u + o4
            accum_cols16(cx4, colbase, nb4, w, sumf)
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k3_c", pso = "g_pso_kq_mvb2_k3", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K3 : MetalKqMvK3T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_k3_c", pso = "g_pso_kq_mvb4_k3", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K3 : MetalKqMvK3T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the k3 B8 twin: the k6 B8 shell (one superblock's X panel staged per threadgroup) with the
// lane + mask compose
[ |> template_struct_instance]
class template MetalKqMvB8K3T : MetalKqMvB8ShellT {
    override PANEL_COLS = 4
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale strips, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k3 quant plane, uint view (qs at 24*blk, hmask at 24*blk+16)

    [metal_kernel]
    def metal_kq_mvb8_k3 {
        run_shell()
    }

    override LANES = 23

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        let s = lc[0]
        let o4 = lc[1]
        let shr = lc[2]
        let dd = float(kdh[blk])
        for [unroll_full] (ch in range(2)) {
            let ip = uint(ch)
            let qp = unsafe(addr(kqu[blk * 24u + ip * 8u + o4]))
            let hp = unsafe(addr(kqu[blk * 24u + 16u + o4]))
            let hsh = ip * 4u + s
            var qu, hu : uint[4]
            for [unroll_full] (u in range(4)) {
                qu[u] = unsafe(qp[u])
                hu[u] = unsafe(hp[u])
            }
            let g = ip * 8u + s * 2u + (tx % 2u)
            let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
            let dl = dd * sc
            let ml = dl * 4.0
            var w4 : float4[4]
            for [unroll_full] (u in range(4)) {
                let qvc = ((qu[u] >> shr) & 0x03030303) | (((hu[u] >> hsh) & 0x01010101) << 2u)
                w4[u] = u8x4_f4(qvc) * dl - float4(ml)
            }
            accum_panel(ip * 32u + s * 8u + o4, w4, acc)
        }
    }
}

// IQ4_XS / IQ4_NL: w = d*sc*LUT[q], the codebook in threadgroup floats, a 16-lane half-simdgroup
// per superblock (the q40 pairing). The one axis is the scale source: iq4xs the SIGNED strip byte
// times the split-plane super d, iq4nl a PER-32-BLOCK f16 d (no strip bytes; kscb keeps the shape).
[ |> template_struct_instance]
class template MetalKqGemvIq4T : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // iq4xs: the d plane (the scale buffer at byte nsb*16, s0off = the caller's doff); iq4nl: the 16B/sb d plane, block bu's d at blk*8+bu
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // quant plane, uint view (block bu of blk at 32*blk + 4*bu; q40's 128B/sb nibble tiling)
    @uniform @binding = 5 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 6 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 5 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @workgroup lut : float[64]   // kvalues_iq4nl as floats, one slot per thread of the 64-wide group
    @template_constant IQ4NL : bool = false
    @template_constant Q40 : bool = false   // q40: iq4nl's d plane, the linear nibble map (q - 8) in the codebook slot
    @template_constant IQODD : bool = false   //!< iq3s4: iq4xs's strip and d, the odd codebook (`iq3s4_val`) in the codebook slot
    @template_constant NV4 : bool = false   //!< nv4: the 16 UE4M3 bytes at soff (byte 2 bu + half the half's scale), the doubled e2m1 codebook

    def x4_at(i : uint) : float4 => x[i]

    def gemv_body {
        var g : uint[8]
        site_setup(g)
        let lid = gl_LocalInvocationID.x
        static_if (Q40) {
            lut[lid] = float(int(lid & 15u)) - 8.0
        } else {
            static_if (IQODD) {
                lut[lid] = iq3s4_val(lid & 15u)
            } else {
                static_if (NV4) {
                    lut[lid] = mx4_val(lid & 15u)
                } else {
                    lut[lid] = iq4_lut(lid & 15u)
                }
            }
        }
        barrier()
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = g[0] / 256u
        let eb = g[1]
        let xo4 = g[2] >> 2u
        let yrow0 = g[3]
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb4 = xo4 + ib * 64u + bu * 8u + il * 2u
            let y0 = x4_at(yb4)
            let y1 = x4_at(yb4 + 1u)
            let y2 = x4_at(yb4 + 4u)
            let y3 = x4_at(yb4 + 5u)
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let qb = blk * 32u + bu * 4u + il * 2u
                let q0 = kqu[qb]
                let q1 = kqu[qb + 1u]
                static_if (NV4) {
                    var alo = y0 * float4(lut[q0 & 15u], lut[(q0 >> 8u) & 15u], lut[(q0 >> 16u) & 15u], lut[(q0 >> 24u) & 15u])
                    alo += y1 * float4(lut[q1 & 15u], lut[(q1 >> 8u) & 15u], lut[(q1 >> 16u) & 15u], lut[(q1 >> 24u) & 15u])
                    var ahi = y2 * float4(lut[(q0 >> 4u) & 15u], lut[(q0 >> 12u) & 15u], lut[(q0 >> 20u) & 15u], lut[q0 >> 28u])
                    ahi += y3 * float4(lut[(q1 >> 4u) & 15u], lut[(q1 >> 12u) & 15u], lut[(q1 >> 20u) & 15u], lut[q1 >> 28u])
                    let slo = ue4m3_half_f32(uint(kscb[blk * 16u + bu * 2u]))
                    let shi = ue4m3_half_f32(uint(kscb[blk * 16u + bu * 2u + 1u]))
                    sumf[r] += slo * (alo.x + alo.y + alo.z + alo.w) + shi * (ahi.x + ahi.y + ahi.z + ahi.w)
                } else {
                    var acc = y0 * float4(lut[q0 & 15u], lut[(q0 >> 8u) & 15u], lut[(q0 >> 16u) & 15u], lut[(q0 >> 24u) & 15u])
                    acc += y2 * float4(lut[(q0 >> 4u) & 15u], lut[(q0 >> 12u) & 15u], lut[(q0 >> 20u) & 15u], lut[q0 >> 28u])
                    acc += y1 * float4(lut[q1 & 15u], lut[(q1 >> 8u) & 15u], lut[(q1 >> 16u) & 15u], lut[(q1 >> 24u) & 15u])
                    acc += y3 * float4(lut[(q1 >> 4u) & 15u], lut[(q1 >> 12u) & 15u], lut[(q1 >> 20u) & 15u], lut[q1 >> 28u])
                    static_if (IQ4NL) {
                        sumf[r] += float(kdh[blk * 8u + bu]) * (acc.x + acc.y + acc.z + acc.w)
                    } else {
                        let sc = float((int(kscb[blk * 16u + bu]) ^ 128) - 128)
                        sumf[r] += float(kdh[blk]) * sc * (acc.x + acc.y + acc.z + acc.w)
                    }
                }
            }
            ib += 2u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < y_rows()) {
                y_put(yrow0 + first_row + uint(r), s)
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_iq4xs_c", pso = "g_pso_kq_iq4xs", tgmem = "MetalKqGemvIq4xs_metal_kq_gemv_iq4_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq4xs : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override IQ4NL = false

    [metal_kernel]
    def metal_kq_gemv_iq4 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_iq3s4_c", pso = "g_pso_kq_iq3s4", tgmem = "MetalKqGemvIq3s4_metal_kq_gemv_iq4_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq3s4 : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override IQODD = true

    [metal_kernel]
    def metal_kq_gemv_iq4 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_iq4nl_c", pso = "g_pso_kq_iq4nl", tgmem = "MetalKqGemvIq4nl_metal_kq_gemv_iq4_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq4nl : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override IQ4NL = true

    [metal_kernel]
    def metal_kq_gemv_iq4 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_q40_c", pso = "g_pso_kq_q40", tgmem = "MetalKqGemvQ40_metal_kq_gemv_iq4_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvQ40 : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override IQ4NL = true
    override Q40 = true

    [metal_kernel]
    def metal_kq_gemv_iq4 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_nv4_c", pso = "g_pso_kq_nv4", tgmem = "MetalKqGemvNv4_metal_kq_gemv_iq4_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvNv4 : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override NV4 = true

    [metal_kernel]
    def metal_kq_gemv_iq4 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq4xs_c", pso = "g_pso_moe_kq_iq4xs", tgmem = "metal_moe_gemv_iq4xs_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq4xs : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_iq4xs_msl")]
    def metal_moe_gemv_iq4xs {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq3s4_c", pso = "g_pso_moe_kq_iq3s4", tgmem = "metal_moe_gemv_iq3s4_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq3s4 : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override IQODD = true

    [metal_kernel(name="metal_moe_gemv_iq3s4_msl")]
    def metal_moe_gemv_iq3s4 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_q40_c", pso = "g_pso_moe_kq_q40", tgmem = "metal_moe_gemv_q40_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvQ40 : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override IQ4NL = true
    override Q40 = true

    [metal_kernel(name="metal_moe_gemv_q40_msl")]
    def metal_moe_gemv_q40 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_nv4_c", pso = "g_pso_moe_kq_nv4", tgmem = "metal_moe_gemv_nv4_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvNv4 : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override NV4 = true

    [metal_kernel(name="metal_moe_gemv_nv4_msl")]
    def metal_moe_gemv_nv4 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq4nl_c", pso = "g_pso_moe_kq_iq4nl", tgmem = "metal_moe_gemv_iq4nl_msl_tgmem", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq4nl : MetalKqGemvIq4T {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override IQ4NL = true

    [metal_kernel(name="metal_moe_gemv_iq4nl_msl")]
    def metal_moe_gemv_iq4nl {
        gemv_body()
    }
}

//! The grid-codebook formats' decode GEMV shell (iq3s, iq3xxs, iq2s, iq2xs, iq2xxs; DM T3): two rows a simdgroup, lane `it` of
//! eight a superblock's 32-element strip, the strip's x as eight float4s; a format supplies its block's term (`blk_sum`) over the
//! d plane `kdh`, the strip bytes `kscb` and the quant words `kqu`, all three the one scale/quant buffer's views at their offsets
[ |> template_struct_instance]
class template MetalKqGridGemvT : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16>   //! the d plane - the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>     //! the 16-byte sub-scale strips, byte view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>       //! the quant plane, uint view
    @uniform @binding = 5 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 6 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 5 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" @template_gate = GATHERED sel : array<uint>    //! [streams x 2k] selection rows (idx section read)

    def x4_at(i : uint) : float4 => x[i]

    //! what a threadgroup stages ahead of the rows (the iq2xxs F4 stamp's expanded grid); nothing by default
    def grid_prologue {
        pass
    }

    //! superblock `blk`'s strip `it` against x `yv`, its term added into `s`
    def abstract blk_sum(blk, it : uint; yv : float4[8]; var s : float&) : void

    def gemv_body {
        grid_prologue()
        var g : uint[8]
        site_setup(g)
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let nb = g[0] / 256u
        let eb = g[1]
        let xo4 = g[2] >> 2u
        let y0 = g[3]
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb4 = xo4 + ib * 64u + it * 8u
            var yv : float4[8]
            for [unroll_full] (i in range(8)) {
                yv[i] = x4_at(yb4 + uint(i))
            }
            for [unroll_full] (r in range(2)) {
                blk_sum(eb + (first_row + uint(r)) * nb + ib, it, yv, sumf[r])
            }
            ib += 4u
        }
        let s0 = simd_sum(sumf[0])
        let s1 = simd_sum(sumf[1])
        put_rows2(first_row, y0, lane, s0, s1)
    }
}

// IQ3_S / IQ3_XXS GEMV: w = d*sc*(+-grid byte), 2 rows per simdgroup. XXS is the 24-word row -
// signs from the block's aux32 through the parity helper, no sign plane and no qh - against
// iq3s's 26-word row with its own sign plane; 8 signed sub-scales (iq3xxs: 2ls+1) a strip.
[ |> template_struct_instance]
class template MetalKqGemvIq3T : MetalKqGridGemvT {
    @template_constant XXS : bool = false

    def override blk_sum(blk, it : uint; yv : float4[8]; var s : float&) {
        var accv = float4(0.0)
        static_if (XXS) {
            let aux = kqu[blk * 24u + 16u + it]
            let qsp = unsafe(addr(kqu[blk * 24u + it * 2u]))
            for [unroll_full] (h in range(2)) {
                let qsw = unsafe(qsp[h])
                for [unroll_full] (k in range(4)) {
                    let e8 = uint(h) * 2u + uint(k) / 2u
                    let sgb = ksign7((aux >> (7u * e8)) & 127u)
                    let nib = uint(k) % 2u == 0u ? sgb & 15u : sgb >> 4u
                    let gw = iq3xxs_gw(int((qsw >> (8u * uint(k))) & 255u))
                    let fs = iq_sign4(nib)
                    accv += yv[h * 4 + k] * (u8x4_f4(gw) * fs)
                }
            }
        } else {
            let qh = (kqu[blk * 26u + 16u + it / 4u] >> ((it % 4u) * 8u)) & 255u
            let sgw = kqu[blk * 26u + 18u + it]
            let qsp = unsafe(addr(kqu[blk * 26u + it * 2u]))
            for [unroll_full] (h in range(2)) {
                let qsw = unsafe(qsp[h])
                for [unroll_full] (k in range(4)) {
                    let b3 = uint(h) * 4u + uint(k)
                    let idx = ((qsw >> (8u * uint(k))) & 255u) | ((qh << (8u - b3)) & 256u)
                    let nib = (sgw >> ((b3 / 2u) * 8u + (b3 % 2u) * 4u)) & 15u
                    let gw = iq3s_gw(int(idx))
                    let fs = iq_sign4(nib)
                    accv += yv[h * 4 + k] * (u8x4_f4(gw) * fs)
                }
            }
        }
        let sc = float(((int(kscb[blk * 16u + it]) & 255) ^ 128) - 128)
        s += float(kdh[blk]) * sc * (accv.x + accv.y + accv.z + accv.w)
    }
}

[metal_dispatch(name = "enc_kq_iq3s_c", pso = "g_pso_kq_iq3s", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq3s : MetalKqGemvIq3T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_iq3s_msl")]
    def metal_kq_gemv_iq3s {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq3s_c", pso = "g_pso_moe_kq_iq3s", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq3s : MetalKqGemvIq3T {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_iq3s_msl")]
    def metal_moe_gemv_iq3s {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_iq3xxs_c", pso = "g_pso_kq_iq3xxs", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq3xxs : MetalKqGemvIq3T {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override XXS = true

    [metal_kernel(name="metal_kq_gemv_iq3xxs_msl")]
    def metal_kq_gemv_iq3xxs {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq3xxs_c", pso = "g_pso_moe_kq_iq3xxs", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq3xxs : MetalKqGemvIq3T {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true
    override XXS = true

    [metal_kernel(name="metal_moe_gemv_iq3xxs_msl")]
    def metal_moe_gemv_iq3xxs {
        gemv_body()
    }
}

// The F4 twins: the grid's magnitudes pre-expanded into threadgroup memory at stage time, so the
// inner loop reads one float4 and flips signs (no byte extracts); 4 rows per simdgroup - y loads
// and grid gathers amortize x4. GRIDT sizes the stage to the format's grid (iq3s 512, iq3xxs 256).
[ |> template_struct_instance]
class template MetalKqGemvIq3F4T : MetalGemvSiteT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16 (s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales (iq3xxs: 2ls+1) + 8 pad, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // quant plane, uint view (iq3s: 16 qs, 2 qh, 8 signs per superblock; iq3xxs: 16 qs, 8 aux)
    @uniform @binding = 5 @template_gate = "!GATHERED" ndim : uint
    @uniform @binding = 6 @template_gate = "!GATHERED" ddim : uint
    @uniform @binding = 5 @template_gate = GATHERED ka : MoeGemvArgs
    @ssbo @binding = 7 @role = "read" @template_gate = GATHERED sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @workgroup gridf : GRIDT
    @template_constant XXS : bool = false

    def x4_at(i : uint) : float4 => x[i]

    def gemv_body {
        var g : uint[8]
        site_setup(g)
        let lid = gl_LocalInvocationID.x
        static_if (XXS) {
            for [unroll_full] (k in range(4)) {
                let gi = lid + uint(k) * 64u
                let gw = iq3xxs_gw(int(gi))
                gridf[gi] = u8x4_f4(gw)
            }
        } else {
            for [unroll_full] (k in range(8)) {
                let gi = lid + uint(k) * 64u
                let gw = iq3s_gw(int(gi))
                gridf[gi] = u8x4_f4(gw)
            }
        }
        barrier()
        let lane = gl_SubgroupInvocationID
        let ix = lane / 16u
        let it = lane % 16u
        let bu = it / 2u
        let il = it % 2u
        let nb = g[0] / 256u
        let eb = g[1]
        let xo4 = g[2] >> 2u
        let yrow0 = g[3]
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 4u
        var sumf : float[4]
        var ib = ix
        while (ib < nb) {
            let yb4 = xo4 + ib * 64u + bu * 8u + il * 4u
            let y0 = x4_at(yb4)
            let y1 = x4_at(yb4 + 1u)
            let y2 = x4_at(yb4 + 2u)
            let y3 = x4_at(yb4 + 3u)
            for [unroll_full] (r in range(4)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                var accv = float4(0.0)
                static_if (XXS) {
                    let qsw = kqu[blk * 24u + bu * 2u + il]
                    let aux = kqu[blk * 24u + 16u + bu]
                    for [unroll_full] (wi in range(4)) {
                        let q3 = il * 2u + uint(wi) / 2u
                        let sgb = ksign7((aux >> (7u * q3)) & 127u)
                        let nib = uint(wi) % 2u == 0u ? sgb & 15u : sgb >> 4u
                        let idx = (qsw >> (8u * uint(wi))) & 255u
                        let fs = iq_sign4(nib)
                        let yv = wi == 0 ? y0 : (wi == 1 ? y1 : (wi == 2 ? y2 : y3))
                        accv += yv * (gridf[idx] * fs)
                    }
                } else {
                    let qsw = kqu[blk * 26u + bu * 2u + il]
                    let qh = (kqu[blk * 26u + 16u + bu / 4u] >> ((bu % 4u) * 8u)) & 255u
                    let sgw = kqu[blk * 26u + 18u + bu]
                    for [unroll_full] (wi in range(4)) {
                        let b = il * 4u + uint(wi)
                        let idx = ((qsw >> (8u * uint(wi))) & 255u) | ((qh << (8u - b)) & 256u)
                        let nib = (sgw >> ((b / 2u) * 8u + (b % 2u) * 4u)) & 15u
                        let fs = iq_sign4(nib)
                        let yv = wi == 0 ? y0 : (wi == 1 ? y1 : (wi == 2 ? y2 : y3))
                        accv += yv * (gridf[idx] * fs)
                    }
                }
                let sc = float((int(kscb[blk * 16u + bu]) ^ 128) - 128)
                sumf[r] += float(kdh[blk]) * sc * (accv.x + accv.y + accv.z + accv.w)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(4)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < y_rows()) {
                y_put(yrow0 + first_row + uint(r), s)
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_iq3s_f4_c", pso = "g_pso_kq_iq3s_f4", tgmem = "metal_kq_gemv_iq3s_f4_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64, n : int64")]
class MetalKqGemvIq3sF4 : MetalKqGemvIq3F4T {
    typedef GRIDT = float4[512]
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_iq3s_f4_msl")]
    def metal_kq_gemv_iq3s_f4 {
        gemv_body()
    }
}

// the gathered site's twin: the staged grid amortizes over four rows of one expert a simdgroup, the dense twin's crown gates it
[metal_dispatch(name = "enc_moe_gemv_iq3s_f4_c", pso = "g_pso_moe_kq_iq3s_f4", tgmem = "metal_moe_gemv_iq3s_f4_msl_tgmem", tg = 64, grid = "rows/8, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq3sF4 : MetalKqGemvIq3F4T {
    typedef GRIDT = float4[512]
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_iq3s_f4_msl")]
    def metal_moe_gemv_iq3s_f4 {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_kq_iq3xxs_f4_c", pso = "g_pso_kq_iq3xxs_f4", tgmem = "metal_kq_gemv_iq3xxs_f4_msl_tgmem", tg = 64, grid = "rows/8", params = "rows : int64, n : int64")]
class MetalKqGemvIq3xxsF4 : MetalKqGemvIq3F4T {
    typedef GRIDT = float4[256]
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override XXS = true

    [metal_kernel(name="metal_kq_gemv_iq3xxs_f4_msl")]
    def metal_kq_gemv_iq3xxs_f4 {
        gemv_body()
    }
}

// IQ2_S: the iq3s GEMV geometry over the u64 grid pair; per-16 UNSIGNED strips scale each
// 16-element half and d is pre-eighth-ed at transcode, so the fold is
// d8 * strip * signed_grid_byte.
[ |> template_struct_instance]
class template MetalKqGemvIq2sT : MetalKqGridGemvT {
    //! 18 words a superblock: 8 idx, 8 signs, 2 qh; 16 unsigned per-16 sub-scales a strip on the eighth-ed d
    def override blk_sum(blk, it : uint; yv : float4[8]; var s : float&) {
        let qwi = kqu[blk * 18u + it]
        let sgw = kqu[blk * 18u + 8u + it]
        let qh = (kqu[blk * 18u + 16u + it / 4u] >> ((it % 4u) * 8u)) & 255u
        let scp = unsafe(addr(kscb[blk * 16u + it * 2u]))
        var half_sum : float[2]
        for [unroll_full] (h in range(2)) {
            var accv = float4(0.0)
            for [unroll_full] (li in range(2)) {
                let l = uint(h) * 2u + uint(li)
                let idx2 = (((qwi >> (8u * l)) & 255u) | ((qh << (8u - 2u * l)) & 0x300u)) * 2u
                let sgb = (sgw >> (8u * l)) & 255u
                let s0 = sgb & 15u
                let s1 = sgb >> 4u
                let w0 = iq2s_gw(int(idx2))
                let w1 = iq2s_gw(int(idx2 + 1u))
                let m0 = u8x4_f4(w0)
                let m1 = u8x4_f4(w1)
                let f0 = iq_sign_apply(m0, s0)
                let f1 = iq_sign_apply(m1, s1)
                accv += yv[l * 2u] * f0
                accv += yv[l * 2u + 1u] * f1
            }
            half_sum[h] = float(unsafe(scp[h])) * (accv.x + accv.y + accv.z + accv.w)
        }
        s += float(kdh[blk]) * (half_sum[0] + half_sum[1])
    }
}

[metal_dispatch(name = "enc_kq_iq2s_c", pso = "g_pso_kq_iq2s", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq2s : MetalKqGemvIq2sT {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_iq2s_msl")]
    def metal_kq_gemv_iq2s {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq2s_c", pso = "g_pso_moe_kq_iq2s", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq2s : MetalKqGemvIq2sT {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_iq2s_msl")]
    def metal_moe_gemv_iq2s {
        gemv_body()
    }
}

// IQ2_XS: the iq2s GEMV geometry over the parity-signed u16 words - one u32 = two u16 qs
// words, the 9-bit index doubles into the grid pair, and ksign7 recomputes the sign byte
// (no sign plane, no qh); per-16 UNSIGNED strips, d eighth-ed.
[ |> template_struct_instance]
class template MetalKqGemvIq2xsT : MetalKqGridGemvT {
    //! 16 words a superblock: 32 u16 qs words; 16 unsigned per-16 sub-scales a strip on the eighth-ed d
    def override blk_sum(blk, it : uint; yv : float4[8]; var s : float&) {
        var half_sum : float[2]
        let qwp = unsafe(addr(kqu[blk * 16u + it * 2u]))
        let scp = unsafe(addr(kscb[blk * 16u + it * 2u]))
        for [unroll_full] (h in range(2)) {
            let qw = unsafe(qwp[h])
            var accv = float4(0.0)
            for [unroll_full] (wi in range(2)) {
                let w16 = (qw >> (16u * uint(wi))) & 0xFFFFu
                let idx2 = (w16 & 511u) * 2u
                let sgn = ksign7(w16 >> 9u)
                let s0 = sgn & 15u
                let s1 = sgn >> 4u
                let w0 = iq2xs_gw(int(idx2))
                let w1 = iq2xs_gw(int(idx2 + 1u))
                let m0 = u8x4_f4(w0)
                let m1 = u8x4_f4(w1)
                let f0 = iq_sign_apply(m0, s0)
                let f1 = iq_sign_apply(m1, s1)
                accv += yv[h * 4 + wi * 2] * f0
                accv += yv[h * 4 + wi * 2 + 1] * f1
            }
            half_sum[h] = float(unsafe(scp[h])) * (accv.x + accv.y + accv.z + accv.w)
        }
        s += float(kdh[blk]) * (half_sum[0] + half_sum[1])
    }
}

[metal_dispatch(name = "enc_kq_iq2xs_c", pso = "g_pso_kq_iq2xs", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq2xs : MetalKqGemvIq2xsT {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel(name="metal_kq_gemv_iq2xs_msl")]
    def metal_kq_gemv_iq2xs {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq2xs_c", pso = "g_pso_moe_kq_iq2xs", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq2xs : MetalKqGemvIq2xsT {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_iq2xs_msl")]
    def metal_moe_gemv_iq2xs {
        gemv_body()
    }
}

// IQ2_XXS over the aux32 form: grid-word byte indices double into the u64 grid pair, aux32
// carries the parity ksigns, ONE per-32 UNSIGNED strip scales the block on the eighth-ed d.
// Two stamps on the F4 axis: in-place constant expand vs the crown-gated threadgroup slab.
[ |> template_struct_instance]
class template MetalKqGemvIq2xxsFam : MetalKqGridGemvT {
    @template_constant F4 : bool = false
    @workgroup @template_gate = F4 gridf : float4[512]   // pre-expanded magnitudes, one float4 per grid word (8 KB)

    def override grid_prologue {
        static_if (F4) {
            let lid = gl_LocalInvocationID.x
            for [unroll_full] (gk in range(8)) {
                let gi = lid + uint(gk) * 64u
                let w = iq2xxs_gw(int(gi))
                gridf[gi] = u8x4_f4(w)
            }
            barrier()
        }
    }

    //! 16 words a superblock, [grid bytes][aux32] a block; one unsigned per-32 sub-scale a strip on the eighth-ed d
    def override blk_sum(blk, it : uint; yv : float4[8]; var s : float&) {
        let gw = kqu[blk * 16u + it * 2u]
        let aux = kqu[blk * 16u + it * 2u + 1u]
        var accv = float4(0.0)
        for [unroll_full] (q3 in range(4)) {
            let bidx = (gw >> (8u * uint(q3))) & 255u
            let sgn = ksign7((aux >> (7u * uint(q3))) & 127u)
            let s0 = sgn & 15u
            let s1 = sgn >> 4u
            var m0, m1 : float4
            static_if (F4) {
                m0 = gridf[bidx * 2u]
                m1 = gridf[bidx * 2u + 1u]
            } else {
                let w0 = iq2xxs_gw(int(bidx * 2u))
                let w1 = iq2xxs_gw(int(bidx * 2u + 1u))
                m0 = u8x4_f4(w0)
                m1 = u8x4_f4(w1)
            }
            let f0 = iq_sign_apply(m0, s0)
            let f1 = iq_sign_apply(m1, s1)
            accv += yv[q3 * 2] * f0
            accv += yv[q3 * 2 + 1] * f1
        }
        let sc = float(kscb[blk * 16u + it])
        s += float(kdh[blk]) * sc * (accv.x + accv.y + accv.z + accv.w)
    }
}

[metal_dispatch(name = "enc_kq_iq2xxs_c", pso = "g_pso_kq_iq2xxs", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq2xxs : MetalKqGemvIq2xxsFam {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>

    [metal_kernel]
    def metal_kq_gemv_iq2xxs {
        gemv_body()
    }
}

[metal_dispatch(name = "enc_moe_gemv_iq2xxs_c", pso = "g_pso_moe_kq_iq2xxs", tg = 64, grid = "rows/4, k, nst", params = "rows : int64, k : int64, nst : int64")]
class MetalMoeGemvIq2xxs : MetalKqGemvIq2xxsFam {
    @ssbo @binding = 3 @role = "read" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*k*nst*4" y : array<float>
    override GATHERED = true

    [metal_kernel(name="metal_moe_gemv_iq2xxs_msl")]
    def metal_moe_gemv_iq2xxs {
        gemv_body()
    }
}

// the F4 twin serves only under the box's serving-confirm crown (ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity)
[metal_dispatch(name = "enc_kq_iq2xxs_f4_c", pso = "g_pso_kq_iq2xxs_f4", tgmem = "MetalKqGemvIq2xxsF4_metal_kq_gemv_iq2xxs_msl_tgmem", tg = 64, grid = "rows/4", params = "rows : int64, n : int64")]
class MetalKqGemvIq2xxsF4 : MetalKqGemvIq2xxsFam {
    @ssbo @binding = 3 @role = "read" @off = "xoff" @span = "n*4" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" @span = "rows*4" y : array<float>
    override F4 = true

    [metal_kernel]
    def metal_kq_gemv_iq2xxs {
        gemv_body()
    }
}

// The iq3xxs B2/B4 pair: the iq3s shells over the 24-word rows - direct halved-grid reads,
// parity signs.
[ |> template_struct_instance]
class template MetalKqMvIq3xxsT : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 sub-scale bytes (2ls+1) + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3xxs quant plane, uint view

    [metal_kernel]
    def metal_kq_mv_iq3xxs {
        run_shell()
    }

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
        let aux = kqu[blk * 24u + 16u + tx]
        var w : float4[8]
        let qsp = unsafe(addr(kqu[blk * 24u + tx * 2u]))
        for [unroll_full] (h in range(2)) {
            let qsw = unsafe(qsp[h])
            for [unroll_full] (wi in range(4)) {
                let q3 = uint(h) * 2u + uint(wi) / 2u
                let sgb = ksign7((aux >> (7u * q3)) & 127u)
                let nib = uint(wi) % 2u == 0u ? sgb & 15u : sgb >> 4u
                let gw = iq3xxs_gw(int((qsw >> (8u * uint(wi))) & 255u))
                let fsd = iq_sign_apply(float4(dl), nib)
                w[h * 4 + wi] = u8x4_f4(gw) * fsd
            }
        }
        accum_cols32(sb * 64u + tx * 8u, colbase, nb4, w, sumf)
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq3xxs_c", pso = "g_pso_kq_mvb2_iq3xxs", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq3xxs : MetalKqMvIq3xxsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq3xxs_c", pso = "g_pso_kq_mvb4_iq3xxs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq3xxs : MetalKqMvIq3xxsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq3xxs B8 twin: the iq3s B8 shell (one superblock's X panel staged per threadgroup)
// over the 24-word rows; the grid read direct.
[ |> template_struct_instance]
class template MetalKqMvB8Iq3xxsT : MetalKqMvB8ShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 sub-scale bytes (2ls+1) + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3xxs quant plane, uint view

    [metal_kernel]
    def metal_kq_mvb8_iq3xxs {
        run_shell()
    }

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
        let aux = kqu[blk * 24u + 16u + tx]
        var w : float4[8]
        let qsp = unsafe(addr(kqu[blk * 24u + tx * 2u]))
        for [unroll_full] (h in range(2)) {
            let qsw = unsafe(qsp[h])
            for [unroll_full] (wi in range(4)) {
                let q3 = uint(h) * 2u + uint(wi) / 2u
                let sgb = ksign7((aux >> (7u * q3)) & 127u)
                let nib = uint(wi) % 2u == 0u ? sgb & 15u : sgb >> 4u
                let gw = iq3xxs_gw(int((qsw >> (8u * uint(wi))) & 255u))
                let fsd = iq_sign_apply(float4(dl), nib)
                w[h * 4 + wi] = u8x4_f4(gw) * fsd
            }
        }
        accum_panel(tx * 8u, w, acc)
    }
}

// The iq3s B2/B4 pair: the iq4xs shell over the per-32-block lane map — lane tx owns block tx,
// 8 grid words = 32 consecutive elems as float4s.
[ |> template_struct_instance]
class template MetalKqMvIq3sT : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3s quant plane, uint view

    [metal_kernel]
    def metal_kq_mv_iq3s {
        run_shell()
    }

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
        let qh = (kqu[blk * 26u + 16u + tx / 4u] >> ((tx % 4u) * 8u)) & 255u
        let sgw = kqu[blk * 26u + 18u + tx]
        var w : float4[8]
        let qsp = unsafe(addr(kqu[blk * 26u + tx * 2u]))
        for [unroll_full] (h in range(2)) {
            let qsw = unsafe(qsp[h])
            for [unroll_full] (wi in range(4)) {
                let b = uint(h) * 4u + uint(wi)
                let idx = ((qsw >> (8u * uint(wi))) & 255u) | ((qh << (8u - b)) & 256u)
                let nib = (sgw >> ((b / 2u) * 8u + (b % 2u) * 4u)) & 15u
                let gw = iq3s_gw(int(idx))
                let fsd = iq_sign_apply(float4(dl), nib)
                w[h * 4 + wi] = u8x4_f4(gw) * fsd
            }
        }
        accum_cols32(sb * 64u + tx * 8u, colbase, nb4, w, sumf)
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq3s_c", pso = "g_pso_kq_mvb2_iq3s", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq3s : MetalKqMvIq3sT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq3s_c", pso = "g_pso_kq_mvb4_iq3s", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq3s : MetalKqMvIq3sT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq3s B8 twin: the iq4xs B8 shell (one superblock's X panel staged per threadgroup) over
// the per-32-block lane map; the grid read direct.
[ |> template_struct_instance]
class template MetalKqMvB8Iq3sT : MetalKqMvB8ShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 8 signed sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq3s quant plane, uint view

    [metal_kernel]
    def metal_kq_mvb8_iq3s {
        run_shell()
    }

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        let dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
        let qh = (kqu[blk * 26u + 16u + tx / 4u] >> ((tx % 4u) * 8u)) & 255u
        let sgw = kqu[blk * 26u + 18u + tx]
        var w : float4[8]
        let qsp = unsafe(addr(kqu[blk * 26u + tx * 2u]))
        for [unroll_full] (h in range(2)) {
            let qsw = unsafe(qsp[h])
            for [unroll_full] (wi in range(4)) {
                let b = uint(h) * 4u + uint(wi)
                let idx = ((qsw >> (8u * uint(wi))) & 255u) | ((qh << (8u - b)) & 256u)
                let nib = (sgw >> ((b / 2u) * 8u + (b % 2u) * 4u)) & 15u
                let gw = iq3s_gw(int(idx))
                let fsd = iq_sign_apply(float4(dl), nib)
                w[h * 4 + wi] = u8x4_f4(gw) * fsd
            }
        }
        accum_panel(tx * 8u, w, acc)
    }
}

// The iq2s B2/B4 pair: the iq3s shell over the u64 grid read direct - lane tx owns block tx,
// TWO grid words per 10-bit index; per-16 UNSIGNED strips give each half its own d8*strip.
[ |> template_struct_instance]
class template MetalKqMvIq2sT : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2s quant plane, uint view

    [metal_kernel]
    def metal_kq_mv_iq2s {
        run_shell()
    }

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
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
            let s0 = sgn & 15u
            let s1 = sgn >> 4u
            let w0 = iq2s_gw(int(idx2))
            let w1 = iq2s_gw(int(idx2 + 1u))
            let m0 = u8x4_f4(w0)
            let m1 = u8x4_f4(w1)
            w[l * 2] = iq_sign_apply(m0, s0) * dl
            w[l * 2 + 1] = iq_sign_apply(m1, s1) * dl
        }
        accum_cols32(sb * 64u + tx * 8u, colbase, nb4, w, sumf)
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq2s_c", pso = "g_pso_kq_mvb2_iq2s", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq2s : MetalKqMvIq2sT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq2s_c", pso = "g_pso_kq_mvb4_iq2s", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq2s : MetalKqMvIq2sT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq2s B8 twin: the iq3s B8 shell (one superblock's X panel staged per threadgroup) with
// the u64 two-word compose; the grid read direct.
[ |> template_struct_instance]
class template MetalKqMvB8Iq2sT : MetalKqMvB8ShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2s quant plane, uint view

    [metal_kernel]
    def metal_kq_mvb8_iq2s {
        run_shell()
    }

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
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
            let s0 = sgn & 15u
            let s1 = sgn >> 4u
            let w0 = iq2s_gw(int(idx2))
            let w1 = iq2s_gw(int(idx2 + 1u))
            let m0 = u8x4_f4(w0)
            let m1 = u8x4_f4(w1)
            w[l * 2] = iq_sign_apply(m0, s0) * dl
            w[l * 2 + 1] = iq_sign_apply(m1, s1) * dl
        }
        accum_panel(tx * 8u, w, acc)
    }
}

// The iq2xs B2/B4 pair: the iq2s shell with the parity-signed u16 walk - lane tx owns block
// tx; two u32 qs words per block, ksign7 per u16; per-16 UNSIGNED strips as dl0/dl1.
[ |> template_struct_instance]
class template MetalKqMvIq2xsT : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xs quant plane, uint view

    [metal_kernel]
    def metal_kq_mv_iq2xs {
        run_shell()
    }

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let dl0 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u])
        let dl1 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u + 1u])
        let qw0 = kqu[blk * 16u + tx * 2u]
        let qw1 = kqu[blk * 16u + tx * 2u + 1u]
        var w : float4[8]
        for [unroll_full] (l in range(4)) {
            let lu = uint(l)
            let w16 = ((l < 2 ? qw0 : qw1) >> (16u * (lu & 1u))) & 0xFFFFu
            let idx2 = (w16 & 511u) * 2u
            let sgn = ksign7(w16 >> 9u)
            let dl = l < 2 ? dl0 : dl1
            let s0 = sgn & 15u
            let s1 = sgn >> 4u
            let w0 = iq2xs_gw(int(idx2))
            let w1 = iq2xs_gw(int(idx2 + 1u))
            let m0 = u8x4_f4(w0)
            let m1 = u8x4_f4(w1)
            w[l * 2] = iq_sign_apply(m0, s0) * dl
            w[l * 2 + 1] = iq_sign_apply(m1, s1) * dl
        }
        accum_cols32(sb * 64u + tx * 8u, colbase, nb4, w, sumf)
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq2xs_c", pso = "g_pso_kq_mvb2_iq2xs", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq2xs : MetalKqMvIq2xsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq2xs_c", pso = "g_pso_kq_mvb4_iq2xs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq2xs : MetalKqMvIq2xsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq2xs B8 twin: the iq2s B8 shell (one superblock's X panel staged per threadgroup) with
// the parity-signed u16 compose; the grid read direct.
[ |> template_struct_instance]
class template MetalKqMvB8Iq2xsT : MetalKqMvB8ShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B strips: 16 UNSIGNED per-16 sub-scales (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xs quant plane, uint view

    [metal_kernel]
    def metal_kq_mvb8_iq2xs {
        run_shell()
    }

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        let dl0 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u])
        let dl1 = float(kdh[blk]) * float(kscb[blk * 16u + tx * 2u + 1u])
        let qw0 = kqu[blk * 16u + tx * 2u]
        let qw1 = kqu[blk * 16u + tx * 2u + 1u]
        var w : float4[8]
        for [unroll_full] (l in range(4)) {
            let lu = uint(l)
            let w16 = ((l < 2 ? qw0 : qw1) >> (16u * (lu & 1u))) & 0xFFFFu
            let idx2 = (w16 & 511u) * 2u
            let sgn = ksign7(w16 >> 9u)
            let dl = l < 2 ? dl0 : dl1
            let s0 = sgn & 15u
            let s1 = sgn >> 4u
            let w0 = iq2xs_gw(int(idx2))
            let w1 = iq2xs_gw(int(idx2 + 1u))
            let m0 = u8x4_f4(w0)
            let m1 = u8x4_f4(w1)
            w[l * 2] = iq_sign_apply(m0, s0) * dl
            w[l * 2 + 1] = iq_sign_apply(m1, s1) * dl
        }
        accum_panel(tx * 8u, w, acc)
    }
}

// The iq2xxs B2/B4 pair: the iq2xs shell over the aux32 walk - lane tx owns block tx; the
// block's grid word supplies four byte indices, ksign7 per index; ONE per-32 strip as dl.
[ |> template_struct_instance]
class template MetalKqMvIq2xxsT : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // strip region: 8 UNSIGNED per-32 sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xxs quant plane, uint view

    [metal_kernel]
    def metal_kq_mv_iq2xxs {
        run_shell()
    }

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let dl = float(kdh[blk]) * float(kscb[blk * 16u + tx])
        let gw = kqu[blk * 16u + tx * 2u]
        let aux = kqu[blk * 16u + tx * 2u + 1u]
        var w : float4[8]
        for [unroll_full] (l in range(4)) {
            let lu = uint(l)
            let bidx = (gw >> (8u * lu)) & 255u
            let sgn = ksign7((aux >> (7u * lu)) & 127u)
            let s0 = sgn & 15u
            let s1 = sgn >> 4u
            let w0 = iq2xxs_gw(int(bidx * 2u))
            let w1 = iq2xxs_gw(int(bidx * 2u + 1u))
            let m0 = u8x4_f4(w0)
            let m1 = u8x4_f4(w1)
            w[l * 2] = iq_sign_apply(m0, s0) * dl
            w[l * 2 + 1] = iq_sign_apply(m1, s1) * dl
        }
        accum_cols32(sb * 64u + tx * 8u, colbase, nb4, w, sumf)
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq2xxs_c", pso = "g_pso_kq_mvb2_iq2xxs", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq2xxs : MetalKqMvIq2xxsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq2xxs_c", pso = "g_pso_kq_mvb4_iq2xxs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq2xxs : MetalKqMvIq2xxsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// the iq2xxs B8 twin: the iq2xs B8 shell (one superblock's X panel staged per threadgroup)
// with the aux32 compose; the grid read direct.
[ |> template_struct_instance]
class template MetalKqMvB8Iq2xxsT : MetalKqMvB8ShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d8 plane - the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // strip region: 8 UNSIGNED per-32 sub-scales + 8 pad (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // iq2xxs quant plane, uint view

    [metal_kernel]
    def metal_kq_mvb8_iq2xxs {
        run_shell()
    }

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        let dl = float(kdh[blk]) * float(kscb[blk * 16u + tx])
        let gw = kqu[blk * 16u + tx * 2u]
        let aux = kqu[blk * 16u + tx * 2u + 1u]
        var w : float4[8]
        for [unroll_full] (l in range(4)) {
            let lu = uint(l)
            let bidx = (gw >> (8u * lu)) & 255u
            let sgn = ksign7((aux >> (7u * lu)) & 127u)
            let s0 = sgn & 15u
            let s1 = sgn >> 4u
            let w0 = iq2xxs_gw(int(bidx * 2u))
            let w1 = iq2xxs_gw(int(bidx * 2u + 1u))
            let m0 = u8x4_f4(w0)
            let m1 = u8x4_f4(w1)
            w[l * 2] = iq_sign_apply(m0, s0) * dl
            w[l * 2 + 1] = iq_sign_apply(m1, s1) * dl
        }
        accum_panel(tx * 8u, w, acc)
    }
}

// the iq4 B2/B4 pair on the GEMV's per-32-block lane map; the one axis is the scale source: iq4xs a signed strip byte times the super d, iq4nl a per-32-block f16 d
[ |> template_struct_instance]
class template MetalKqMvIq4T : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  //!< the iq4nl stamp binds it unread, so both stamps share one set layout
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @template_constant IQ4NL : bool = false
    @template_constant Q40 : bool = false   // q40: iq4nl's d plane, the linear nibble map (q - 8)
    @template_constant IQODD : bool = false   //!< iq3s4: iq4xs's strip and d, the odd codebook (`iq3s4_val`)
    @template_constant NV4 : bool = false   //!< nv4: the UE4M3 byte pair 2 tx, 2 tx + 1 scaling the block's low and high halves, the doubled e2m1 codebook

    [metal_kernel]
    def metal_kq_mv_iq4 {
        run_shell()
    }

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        var dl, dh : float
        static_if (NV4) {
            dl = ue4m3_half_f32(uint(kscb[blk * 16u + tx * 2u]))
            dh = ue4m3_half_f32(uint(kscb[blk * 16u + tx * 2u + 1u]))
        } else {
            static_if (IQ4NL) {
                dl = float(kdh[blk * 8u + tx])
            } else {
                dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
            }
            dh = dl
        }
        let qp = unsafe(addr(kqu[blk * 32u + tx * 4u]))
        var wl, wh : float4[4]
        for [unroll_full] (u in range(4)) {
            let qv = unsafe(qp[u])
            static_if (Q40) {
                wl[u] = (float4(float(qv & 15u), float((qv >> 8u) & 15u), float((qv >> 16u) & 15u), float((qv >> 24u) & 15u)) - float4(8.0)) * dl
                wh[u] = (float4(float((qv >> 4u) & 15u), float((qv >> 12u) & 15u), float((qv >> 20u) & 15u), float(qv >> 28u)) - float4(8.0)) * dl
            } else {
                static_if (IQODD) {
                    wl[u] = float4(iq3s4_val(qv & 15u), iq3s4_val((qv >> 8u) & 15u), iq3s4_val((qv >> 16u) & 15u), iq3s4_val((qv >> 24u) & 15u)) * dl
                    wh[u] = float4(iq3s4_val((qv >> 4u) & 15u), iq3s4_val((qv >> 12u) & 15u), iq3s4_val((qv >> 20u) & 15u), iq3s4_val(qv >> 28u)) * dl
                } else {
                    static_if (NV4) {
                        wl[u] = float4(mx4_val(qv & 15u), mx4_val((qv >> 8u) & 15u), mx4_val((qv >> 16u) & 15u), mx4_val((qv >> 24u) & 15u)) * dl
                        wh[u] = float4(mx4_val((qv >> 4u) & 15u), mx4_val((qv >> 12u) & 15u), mx4_val((qv >> 20u) & 15u), mx4_val(qv >> 28u)) * dh
                    } else {
                        wl[u] = float4(iq4_lut(qv & 15u), iq4_lut((qv >> 8u) & 15u), iq4_lut((qv >> 16u) & 15u), iq4_lut((qv >> 24u) & 15u)) * dl
                        wh[u] = float4(iq4_lut((qv >> 4u) & 15u), iq4_lut((qv >> 12u) & 15u), iq4_lut((qv >> 20u) & 15u), iq4_lut(qv >> 28u)) * dl
                    }
                }
            }
        }
        var w : float4[8]
        for [unroll_full] (u in range(4)) {
            w[u] = wl[u]
            w[4 + u] = wh[u]
        }
        accum_cols32(sb * 64u + tx * 8u, colbase, nb4, w, sumf)
    }
}

[metal_dispatch(name = "enc_kq_mvb2_iq4xs_c", pso = "g_pso_kq_mvb2_iq4xs", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq4xs : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_iq4xs_c", pso = "g_pso_kq_mvb4_iq4xs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq4xs : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

[metal_dispatch(name = "enc_kq_mvb2_iq3s4_c", pso = "g_pso_kq_mvb2_iq3s4", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq3s4 : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override IQODD = true
}

[metal_dispatch(name = "enc_kq_mvb4_iq3s4_c", pso = "g_pso_kq_mvb4_iq3s4", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq3s4 : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override IQODD = true
}

[metal_dispatch(name = "enc_kq_mvb2_iq4nl_c", pso = "g_pso_kq_mvb2_iq4nl", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq4nl : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override IQ4NL = true
}

[metal_dispatch(name = "enc_kq_mvb4_iq4nl_c", pso = "g_pso_kq_mvb4_iq4nl", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq4nl : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override IQ4NL = true
}

[metal_dispatch(name = "enc_kq_mvb2_q40_c", pso = "g_pso_kq_mvb2_q40", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Q40 : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(name = "enc_kq_mvb4_q40_c", pso = "g_pso_kq_mvb4_q40", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Q40 : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(name = "enc_kq_mvb2_nv4_c", pso = "g_pso_kq_mvb2_nv4", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Nv4 : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override NV4 = true
}

[metal_dispatch(name = "enc_kq_mvb4_nv4_c", pso = "g_pso_kq_mvb4_nv4", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Nv4 : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override NV4 = true
}

// the iq4 B8 twin on the K45 B8 shell, the same scale-source axis
[ |> template_struct_instance]
class template MetalKqMvB8Iq4T : MetalKqMvB8ShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  //!< the iq4nl stamp binds it unread, so both stamps share one set layout
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @template_constant IQ4NL : bool = false
    @template_constant Q40 : bool = false   // q40: iq4nl's d plane, the linear nibble map (q - 8)
    @template_constant IQODD : bool = false   //!< iq3s4: iq4xs's strip and d, the odd codebook (`iq3s4_val`)
    @template_constant NV4 : bool = false   //!< nv4: the UE4M3 byte pair 2 tx, 2 tx + 1 scaling the block's low and high halves, the doubled e2m1 codebook

    [metal_kernel]
    def metal_kq_mvb8_iq4 {
        run_shell()
    }

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        var dl, dh : float
        static_if (NV4) {
            dl = ue4m3_half_f32(uint(kscb[blk * 16u + tx * 2u]))
            dh = ue4m3_half_f32(uint(kscb[blk * 16u + tx * 2u + 1u]))
        } else {
            static_if (IQ4NL) {
                dl = float(kdh[blk * 8u + tx])
            } else {
                dl = float(kdh[blk]) * float((int(kscb[blk * 16u + tx]) ^ 128) - 128)
            }
            dh = dl
        }
        let qp = unsafe(addr(kqu[blk * 32u + tx * 4u]))
        var wl, wh : float4[4]
        for [unroll_full] (u in range(4)) {
            let qv = unsafe(qp[u])
            static_if (Q40) {
                wl[u] = (float4(float(qv & 15u), float((qv >> 8u) & 15u), float((qv >> 16u) & 15u), float((qv >> 24u) & 15u)) - float4(8.0)) * dl
                wh[u] = (float4(float((qv >> 4u) & 15u), float((qv >> 12u) & 15u), float((qv >> 20u) & 15u), float(qv >> 28u)) - float4(8.0)) * dl
            } else {
                static_if (IQODD) {
                    wl[u] = float4(iq3s4_val(qv & 15u), iq3s4_val((qv >> 8u) & 15u), iq3s4_val((qv >> 16u) & 15u), iq3s4_val((qv >> 24u) & 15u)) * dl
                    wh[u] = float4(iq3s4_val((qv >> 4u) & 15u), iq3s4_val((qv >> 12u) & 15u), iq3s4_val((qv >> 20u) & 15u), iq3s4_val(qv >> 28u)) * dl
                } else {
                    static_if (NV4) {
                        wl[u] = float4(mx4_val(qv & 15u), mx4_val((qv >> 8u) & 15u), mx4_val((qv >> 16u) & 15u), mx4_val((qv >> 24u) & 15u)) * dl
                        wh[u] = float4(mx4_val((qv >> 4u) & 15u), mx4_val((qv >> 12u) & 15u), mx4_val((qv >> 20u) & 15u), mx4_val(qv >> 28u)) * dh
                    } else {
                        wl[u] = float4(iq4_lut(qv & 15u), iq4_lut((qv >> 8u) & 15u), iq4_lut((qv >> 16u) & 15u), iq4_lut((qv >> 24u) & 15u)) * dl
                        wh[u] = float4(iq4_lut((qv >> 4u) & 15u), iq4_lut((qv >> 12u) & 15u), iq4_lut((qv >> 20u) & 15u), iq4_lut(qv >> 28u)) * dl
                    }
                }
            }
        }
        let cxl = tx * 8u
        for [unroll_full] (b in range(8)) {
            for [unroll_full] (u in range(4)) {
                acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * wl[u] + txp[uint(b) * 64u + cxl + 4u + uint(u)] * wh[u]
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb8_iq4xs_c", pso = "g_pso_kq_mvb8_iq4xs", tgmem = "MetalKqMvB8Iq4xs_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq4xs : MetalKqMvB8Iq4T {
}

[metal_dispatch(name = "enc_kq_mvb8_iq3s4_c", pso = "g_pso_kq_mvb8_iq3s4", tgmem = "MetalKqMvB8Iq3s4_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq3s4 : MetalKqMvB8Iq4T {
    override IQODD = true
}

[metal_dispatch(name = "enc_kq_mvb8_iq4nl_c", pso = "g_pso_kq_mvb8_iq4nl", tgmem = "MetalKqMvB8Iq4nl_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq4nl : MetalKqMvB8Iq4T {
    override IQ4NL = true
}

[metal_dispatch(name = "enc_kq_mvb8_q40_c", pso = "g_pso_kq_mvb8_q40", tgmem = "MetalKqMvB8Q40_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Q40 : MetalKqMvB8Iq4T {
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(name = "enc_kq_mvb8_nv4_c", pso = "g_pso_kq_mvb8_nv4", tgmem = "MetalKqMvB8Nv4_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Nv4 : MetalKqMvB8Iq4T {
    override NV4 = true
}

// The B2/B4 width pair as one template per format (the B8 trio below is a DIFFERENT algorithm and
// stays hand-written).
[ |> template_struct_instance]
class template MetalKqMvK4T : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)

    [metal_kernel]
    def metal_kq_mv_k4 {
        run_shell()
    }

    override LANES = 45

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let q4i = lc[0]
        let jl = lc[1]
        let lo = lc[2]
        let nsh = lc[3]
        let sv = ks4[blk]
        let dd = float(ksh[blk * 8u])
        let dm = float(ksh[blk * 8u + 1u])
        for [unroll_full] (ch in range(2)) {
            var sc = 0u
            var mn = 0u
            kmask_sc_mn_ch(sv, jl, ch, sc, mn)
            let dl = dd * float(sc)
            let ml = dm * float(mn)
            let qp = unsafe(addr(kqu[blk * 32u + (q4i + uint(ch) * 2u) * 8u + lo]))
            var w : float[16]
            for [unroll_full] (u in range(4)) {
                let qv = unsafe(qp[u])
                for [unroll_full] (e in range(4)) {
                    let byv = (qv >> (8u * uint(e))) & 255u
                    w[4 * u + e] = float((byv >> nsh) & 0x0Fu) * dl - ml
                }
            }
            let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + ((tx % 4u) < 2u ? 0u : 8u) + lo
            accum_cols16(cx4, colbase, nb4, w, sumf)
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k4_c", pso = "g_pso_kq_mvb2_k4", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K4 : MetalKqMvK4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

//! the k4 twin's two-row register tile: a thread owns TWO weight rows and both x columns, so every x float4 load feeds two rows
[metal_dispatch(name = "enc_kq_mvb2_k4r2_c", pso = "g_pso_kq_mvb2_k4r2", tg = 64, grid = "rows/16", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K4R2 {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @ssbo @binding = 3 @role = "read" @off = "xoff" x : array<float4>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel]
    def metal_kq_mv_k4_r2 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row0 = gl_WorkGroupID.x * 16u + gl_SubgroupID * 8u + ty * 2u
        let dd1 = ka.ddim - 1u
        let rowA = min(row0, dd1)          // a tail pair past ddim reads a live row, writes nothing
        let rowB = min(row0 + 1u, dd1)
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ka.ndim / 4u
        var sumf : float[4]                 // [b * 2 + r]
        var sb = 0u
        while (sb < nb) {
            let blkA = rowA * nb + sb
            let blkB = rowB * nb + sb
            let svA = ks4[blkA]
            let svB = ks4[blkB]
            let ddA = float(ksh[blkA * 8u])
            let dmA = float(ksh[blkA * 8u + 1u])
            let ddB = float(ksh[blkB * 8u])
            let dmB = float(ksh[blkB * 8u + 1u])
            for [unroll_full] (ch in range(2)) {
                var scA = 0u
                var mnA = 0u
                var scB = 0u
                var mnB = 0u
                kmask_sc_mn_ch(svA, jl, ch, scA, mnA)
                kmask_sc_mn_ch(svB, jl, ch, scB, mnB)
                let dlA = ddA * float(scA)
                let mlA = dmA * float(mnA)
                let dlB = ddB * float(scB)
                let mlB = dmB * float(mnB)
                let qoff = (q4i + uint(ch) * 2u) * 8u + lo
                let qpA = unsafe(addr(kqu[blkA * 32u + qoff]))
                let qpB = unsafe(addr(kqu[blkB * 32u + qoff]))
                var wa, wb : float[16]
                for [unroll_full] (u in range(4)) {
                    let qvA = unsafe(qpA[u])
                    let qvB = unsafe(qpB[u])
                    for [unroll_full] (e in range(4)) {
                        let byA = (qvA >> (8u * uint(e))) & 255u
                        let byB = (qvB >> (8u * uint(e))) & 255u
                        wa[4 * u + e] = float((byA >> nsh) & 0x0Fu) * dlA - mlA
                        wb[4 * u + e] = float((byB >> nsh) & 0x0Fu) * dlB - mlB
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(2)) {
                    let xp = unsafe(addr(x[uint(b) * nb4 + cx4]))
                    var accA = 0.0
                    var accB = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = unsafe(xp[u])
                        accA += v.x * wa[4 * u] + v.y * wa[4 * u + 1] + v.z * wa[4 * u + 2] + v.w * wa[4 * u + 3]
                        accB += v.x * wb[4 * u] + v.y * wb[4 * u + 1] + v.z * wb[4 * u + 2] + v.w * wb[4 * u + 3]
                    }
                    sumf[b * 2] += accA
                    sumf[b * 2 + 1] += accB
                }
            }
            sb++
        }
        for [unroll_full] (i in range(4)) {
            var s = sumf[i]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            let row = row0 + uint(i % 2)
            if (tx == 0u && row < ka.ddim && uint(i / 2) < ka.nr) {
                y[uint(i / 2) * ka.ys + row] = s
            }
        }
    }
}

//! the k4 wide form: the single-row GEMV's lane map (two rows a simdgroup, four superblocks in flight
//! through the lane/8 stride) over NCOL x columns - each lane decodes its 32 weights of both rows once
//! and dots them against every column, so the weight pass costs what one GEMV pass costs
[ |> template_struct_instance]
class template MetalKqMvWK4T : MetalKqXT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @ssbo @binding = 4 @role = "write" @off = "yoff" y : array<float>
    @uniform @binding = 5 ka : KqMvArgs
    @template_constant NCOL : int = 2

    //! one lane's 32 weights of one row: four regions of eight (the lo and hi nibbles of the eight
    //! bytes at qb, then of the eight at qb + 64), each under its own scale and min
    def decode32(blk, iq, ir : uint; var w : float[32]) {
        let scs = kmask_scales(ks4[blk], iq)
        let dd = float(ksh[blk * 8u])
        let dm = float(ksh[blk * 8u + 1u])
        let qb = blk * 32u + iq * 8u + ir * 2u
        let dl0 = dd * float(scs.x & 255u)
        let ml0 = dm * float(scs.y & 255u)
        let dl1 = dd * float(scs.x >> 8u)
        let ml1 = dm * float(scs.y >> 8u)
        let dl2 = dd * float(scs.z & 255u)
        let ml2 = dm * float(scs.w & 255u)
        let dl3 = dd * float(scs.z >> 8u)
        let ml3 = dm * float(scs.w >> 8u)
        let qp = unsafe(addr(kqu[qb]))
        for [unroll_full] (u in range(2)) {
            let q1 = unsafe(qp[u])
            let q2 = unsafe(qp[16 + u])
            for [unroll_full] (e in range(4)) {
                let b1 = (q1 >> (8u * uint(e))) & 255u
                let b2 = (q2 >> (8u * uint(e))) & 255u
                w[4 * u + e] = float(b1 & 15u) * dl0 - ml0
                w[8 + 4 * u + e] = float(b1 >> 4u) * dl1 - ml1
                w[16 + 4 * u + e] = float(b2 & 15u) * dl2 - ml2
                w[24 + 4 * u + e] = float(b2 >> 4u) * dl3 - ml3
            }
        }
    }

    [metal_kernel]
    def metal_kq_mvw_k4 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ka.ndim / KQ_SB
        let nb4 = ka.ndim / 4u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        let dd1 = ka.ddim - 1u
        let rowA = min(first_row, dd1)
        let rowB = min(first_row + 1u, dd1)
        var sumf : float[2 * NCOL]
        var ib = ix
        while (ib < nb) {
            var wa, wb : float[32]
            decode32(rowA * nb + ib, iq, ir, wa)
            decode32(rowB * nb + ib, iq, ir, wb)
            let xb4 = (ib * KQ_SB + iq * 64u + ir * 8u) / 4u
            for [unroll_full] (b in range(NCOL)) {
                var acca = 0.0
                var accb = 0.0
                for [unroll_full] (g in range(4)) {
                    let xi = uint(b) * nb4 + xb4 + (g < 2 ? uint(g) * 8u : 16u + uint(g) * 8u)
                    static_if (XHALF) {
                        let xp = unsafe(addr(xh[xi]))
                        for [unroll_full] (u in range(2)) {
                            let v = float4(unsafe(xp[u]))
                            let k = 8 * g + 4 * u
                            acca += v.x * wa[k] + v.y * wa[k + 1] + v.z * wa[k + 2] + v.w * wa[k + 3]
                            accb += v.x * wb[k] + v.y * wb[k + 1] + v.z * wb[k + 2] + v.w * wb[k + 3]
                        }
                    } else {
                        let xp = unsafe(addr(x[xi]))
                        for [unroll_full] (u in range(2)) {
                            let v = unsafe(xp[u])
                            let k = 8 * g + 4 * u
                            acca += v.x * wa[k] + v.y * wa[k + 1] + v.z * wa[k + 2] + v.w * wa[k + 3]
                            accb += v.x * wb[k] + v.y * wb[k + 1] + v.z * wb[k + 2] + v.w * wb[k + 3]
                        }
                    }
                }
                sumf[2 * b] += acca
                sumf[2 * b + 1] += accb
            }
            ib += 4u
        }
        for [unroll_full] (i in range(2 * NCOL)) {
            let s = simd_sum(sumf[i])
            let row = first_row + uint(i % 2)
            if (lane == 0u && row < ka.ddim && uint(i / 2) < ka.nr) {
                y[uint(i / 2) * ka.ys + row] = s
            }
        }
    }
}

//! the f32 wide k4 stamps are the race bench's arms alone (`bench_metal_kq_race.das`): production's wide route is the half one
class MetalKqMvW2K4 : MetalKqMvWK4T {
    override NCOL = 2
}

[metal_dispatch(name = "enc_kq_mvw2h_k4_c", pso = "g_pso_kq_mvw2h_k4", tg = 64, grid = "rows/4", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvW2K4H : MetalKqMvWK4T {
    override NCOL = 2
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvw4h_k4_c", pso = "g_pso_kq_mvw4h_k4", tg = 64, grid = "rows/4", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvW4K4H : MetalKqMvWK4T {
    override NCOL = 4
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvw8h_k4_c", pso = "g_pso_kq_mvw8h_k4", tg = 64, grid = "rows/4", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvW8K4H : MetalKqMvWK4T {
    override NCOL = 8
    override XHALF = true
}

class MetalKqMvW4K4 : MetalKqMvWK4T {
    override NCOL = 4
}

class MetalKqMvW8K4 : MetalKqMvWK4T {
    override NCOL = 8
}

[metal_dispatch(name = "enc_kq_mvb4_k4_c", pso = "g_pso_kq_mvb4_k4", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K4 : MetalKqMvK4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

[ |> template_struct_instance]
class template MetalKqMvK6T : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)

    [metal_kernel]
    def metal_kq_mv_k6 {
        run_shell()
    }

    override LANES = 6

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let s = lc[0]
        let o4 = lc[1]
        let qoff = lc[2]
        let shr = lc[3]
        let hsh = lc[4]
        let dd = float(kdh[blk])
        for [unroll_full] (ch in range(2)) {
            let ip = uint(ch)
            let qp = unsafe(addr(kqu[blk * 48u + ip * 16u + qoff]))
            let hp = unsafe(addr(kqu[blk * 48u + 32u + ip * 8u + o4]))
            let g = ip * 8u + s * 2u + (tx % 2u)
            let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
            let dl = dd * sc
            let ml = dl * 32.0
            var w : float[16]
            // 4+4 uint loads per chunk (the byte-view form was 32 scattered byte loads);
            // the slot's lsh/rsh dance is identically ((h >> 2s) & 3) << 4, vectorized
            for [unroll_full] (u in range(4)) {
                let qu = unsafe(qp[u])
                let hu = unsafe(hp[u])
                let qv = ((qu >> shr) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                for [unroll_full] (e in range(4)) {
                    w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                }
            }
            let cx4 = sb * 64u + ip * 32u + s * 8u + o4
            accum_cols16(cx4, colbase, nb4, w, sumf)
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k6_c", pso = "g_pso_kq_mvb2_k6", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K6 : MetalKqMvK6T {
    override NR = 2
    override NRU = 2u
    override TILED = false
}

[metal_dispatch(name = "enc_kq_mvb4_k6_c", pso = "g_pso_kq_mvb4_k6", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K6 : MetalKqMvK6T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// k5 twins: k4's chunk geometry and kmask scale decode verbatim (q5_K shares the 12-byte scale
// block) + the qh 5th-bit overlay. The 160B plane rides the SAME uint view (40 uints/blk: qs at
// 40*blk, qh at 40*blk+32) — the overlay bit equals its kmask pair index, so scale indexing serves it too.

// k5 rides the shell's TILED arm at BOTH widths: its live-colbase B2 measured neutral-to-BETTER
// (-1.4% cls, max_threads 448 -> 512; bench_metal_gemv_kernels KqMv round).
[ |> template_struct_instance]
class template MetalKqMvK5T : MetalKqMvShellT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k5 plane, uint view (qs at 40*blk, qh at 40*blk+32)

    [metal_kernel]
    def metal_kq_mv_k5 {
        run_shell()
    }

    override LANES = 45

    def override mv_superblock(sb, blk, tx, colbase, nb4 : uint; lc : uint[8]; var sumf : float[NR]) {
        let q4i = lc[0]
        let jl = lc[1]
        let lo = lc[2]
        let nsh = lc[3]
        let nib = (tx % 4u) < 2u
        let sv = ks4[blk]
        let dd = float(ksh[blk * 8u])
        let dm = float(ksh[blk * 8u + 1u])
        var hq : uint[4]
        let hp = unsafe(addr(kqu[blk * 40u + 32u + lo]))
        for [unroll_full] (u in range(4)) {
            hq[u] = unsafe(hp[u])
        }
        for [unroll_full] (ch in range(2)) {
            var sc = 0u
            var mn = 0u
            kmask_sc_mn_ch(sv, jl, ch, sc, mn)
            let dl = dd * float(sc)
            let ml = dm * float(mn)
            let qp = unsafe(addr(kqu[blk * 40u + (q4i + uint(ch) * 2u) * 8u + lo]))
            let hsh = jl + 4u * uint(ch)
            var w : float[16]
            for [unroll_full] (u in range(4)) {
                let qv = unsafe(qp[u])
                for [unroll_full] (e in range(4)) {
                    let byv = (qv >> (8u * uint(e))) & 255u
                    let hb = (hq[u] >> (8u * uint(e) + hsh)) & 1u
                    w[4 * u + e] = float(((byv >> nsh) & 0x0Fu) | (hb << 4u)) * dl - ml
                }
            }
            let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
            accum_cols16(cx4, colbase, nb4, w, sumf)
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb2_k5_c", pso = "g_pso_kq_mvb2_k5", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K5 : MetalKqMvK5T {
    override NR = 2
    override NRU = 2u
    override TILED = true
}

[metal_dispatch(name = "enc_kq_mvb4_k5_c", pso = "g_pso_kq_mvb4_k5", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K5 : MetalKqMvK5T {
    override NR = 4
    override NRU = 4u
    override TILED = true
}

// The B8 K4/K5 pair as one template — same X-panel algorithm, q5_K adds the qh overlay (w4 is
// arm-scoped; K4's stamp carries one dead hq decl, below the measured-free bar). K6 stays forked
// on the weight-decode axis (its 48B superblock walk shares no SB_UINTS/QH shape with these).
[ |> template_struct_instance]
class template MetalKqMvB8K45T : MetalKqMvB8ShellT {
    override PANEL_COLS = 4
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // quant plane, uint view (qs at SB_UINTS*blk; k5 qh at SB_UINTS*blk+32)
    @template_constant SB_UINTS : uint = 32u  // superblock quant-plane stride in uints
    @template_constant QH : bool = false // q5_K: the 5th-bit overlay plane rides after the nibbles

    [metal_kernel]
    def metal_kq_mvb8 {
        run_shell()
    }

    override LANES = 45

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        let q4i = lc[0]
        let jl = lc[1]
        let lo = lc[2]
        let nsh = lc[3]
        let nib = (tx % 4u) < 2u
        let sv = ks4[blk]
        let dd = float(ksh[blk * 8u])
        let dm = float(ksh[blk * 8u + 1u])
        let qp = unsafe(addr(kqu[blk * SB_UINTS + q4i * 8u + lo]))
        let hp = unsafe(addr(kqu[blk * SB_UINTS + 32u + lo]))
        var qv : uint[8]
        var hq : uint[4]   // nolint:LINT002,LINT019 dead in the QH=false stamp — no init, no dataflow
        for [unroll_full] (u in range(4)) {
            qv[u] = unsafe(qp[u])
            qv[4 + u] = unsafe(qp[16 + u])
            static_if (QH) {
                hq[u] = unsafe(hp[u])
            }
        }
        for [unroll_full] (ch in range(2)) {
            var sc = 0u
            var mn = 0u
            kmask_sc_mn_ch(sv, jl, ch, sc, mn)
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
                accum_panel((q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo, w4, acc)
            } else {
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvv = qv[4 * ch + u]
                    w4[u] = float4(float((qvv >> nsh) & 15u), float((qvv >> (8u + nsh)) & 15u),
                        float((qvv >> (16u + nsh)) & 15u), float((qvv >> (24u + nsh)) & 15u)) * dl - float4(ml)
                }
                accum_panel((q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo, w4, acc)
            }
        }
    }
}

[metal_dispatch(name = "enc_kq_mvb8_k4_c", pso = "g_pso_kq_mvb8_k4", tgmem = "MetalKqMvB8K4_metal_kq_mvb8_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K4 : MetalKqMvB8K45T {
    override SB_UINTS = 32u
    override QH = false
}

[metal_dispatch(name = "enc_kq_mvb8_k5_c", pso = "g_pso_kq_mvb8_k5", tgmem = "MetalKqMvB8K5_metal_kq_mvb8_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K5 : MetalKqMvB8K45T {
    override SB_UINTS = 40u
    override QH = true
}

[ |> template_struct_instance]
class template MetalKqMvB8K6T : MetalKqMvB8ShellT {
    override PANEL_COLS = 4
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 @role = "weight" @off = "soff" kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)

    [metal_kernel]
    def metal_kq_mvb8_k6 {
        run_shell()
    }

    override LANES = 6

    def override mvb8_superblock(blk, tx : uint; lc : uint[8]; var acc : float4[8]) {
        let s = lc[0]
        let o4 = lc[1]
        let qoff = lc[2]
        let shr = lc[3]
        let hsh = lc[4]
        let dd = float(kdh[blk])
        for [unroll_full] (ch in range(2)) {
            let ip = uint(ch)
            let qp = unsafe(addr(kqu[blk * 48u + ip * 16u + qoff]))
            let hp = unsafe(addr(kqu[blk * 48u + 32u + ip * 8u + o4]))
            var qu, hu : uint[4]
            for [unroll_full] (u in range(4)) {
                qu[u] = unsafe(qp[u])
                hu[u] = unsafe(hp[u])
            }
            let g = ip * 8u + s * 2u + (tx % 2u)
            let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
            let dl = dd * sc
            let ml = dl * 32.0
            var w4 : float4[4]
            for [unroll_full] (u in range(4)) {
                let qvc = ((qu[u] >> shr) & 0x0F0F0F0F) | (((hu[u] >> hsh) & 0x03030303) << 4u)
                w4[u] = u8x4_f4(qvc) * dl - float4(ml)
            }
            accum_panel(ip * 32u + s * 8u + o4, w4, acc)
        }
    }
}

// The small-batch forms' instances past the templates above: the eight-row f32 stamps of the hand-written B8
// templates, and every format's half-x stamps (`override XHALF = true` on its f32 form's class). k4's half
// route is its wide forms (MetalKqMvW*K4H), so it has none here.
[metal_dispatch(name = "enc_kq_mvb8_k2_c", pso = "g_pso_kq_mvb8_k2", tgmem = "MetalKqMvB8K2_metal_kq_mvb8_k2_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K2 : MetalKqMvB8K2T {
}

[metal_dispatch(name = "enc_kq_mvb8_k3_c", pso = "g_pso_kq_mvb8_k3", tgmem = "MetalKqMvB8K3_metal_kq_mvb8_k3_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K3 : MetalKqMvB8K3T {
}

[metal_dispatch(name = "enc_kq_mvb8_iq3xxs_c", pso = "g_pso_kq_mvb8_iq3xxs", tgmem = "MetalKqMvB8Iq3xxs_metal_kq_mvb8_iq3xxs_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq3xxs : MetalKqMvB8Iq3xxsT {
}

[metal_dispatch(name = "enc_kq_mvb8_iq3s_c", pso = "g_pso_kq_mvb8_iq3s", tgmem = "MetalKqMvB8Iq3s_metal_kq_mvb8_iq3s_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq3s : MetalKqMvB8Iq3sT {
}

[metal_dispatch(name = "enc_kq_mvb8_iq2s_c", pso = "g_pso_kq_mvb8_iq2s", tgmem = "MetalKqMvB8Iq2s_metal_kq_mvb8_iq2s_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq2s : MetalKqMvB8Iq2sT {
}

[metal_dispatch(name = "enc_kq_mvb8_iq2xs_c", pso = "g_pso_kq_mvb8_iq2xs", tgmem = "MetalKqMvB8Iq2xs_metal_kq_mvb8_iq2xs_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq2xs : MetalKqMvB8Iq2xsT {
}

[metal_dispatch(name = "enc_kq_mvb8_iq2xxs_c", pso = "g_pso_kq_mvb8_iq2xxs", tgmem = "MetalKqMvB8Iq2xxs_metal_kq_mvb8_iq2xxs_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq2xxs : MetalKqMvB8Iq2xxsT {
}

[metal_dispatch(name = "enc_kq_mvb8_k6_c", pso = "g_pso_kq_mvb8_k6", tgmem = "MetalKqMvB8K6_metal_kq_mvb8_k6_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K6 : MetalKqMvB8K6T {
}

[metal_dispatch(name = "enc_kq_mvb2h_k5_c", pso = "g_pso_kq_mvb2h_k5", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K5H : MetalKqMvK5T {
    override NR = 2
    override NRU = 2u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_k5_c", pso = "g_pso_kq_mvb4h_k5", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K5H : MetalKqMvK5T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_k5_c", pso = "g_pso_kq_mvb8h_k5", tgmem = "MetalKqMvB8K5H_metal_kq_mvb8_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K5H : MetalKqMvB8K45T {
    override SB_UINTS = 40u
    override QH = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_k6_c", pso = "g_pso_kq_mvb2h_k6", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K6H : MetalKqMvK6T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_k6_c", pso = "g_pso_kq_mvb4h_k6", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K6H : MetalKqMvK6T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_k6_c", pso = "g_pso_kq_mvb8h_k6", tgmem = "MetalKqMvB8K6H_metal_kq_mvb8_k6_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K6H : MetalKqMvB8K6T {
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_iq4xs_c", pso = "g_pso_kq_mvb2h_iq4xs", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq4xsH : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_iq4xs_c", pso = "g_pso_kq_mvb4h_iq4xs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq4xsH : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_iq4xs_c", pso = "g_pso_kq_mvb8h_iq4xs", tgmem = "MetalKqMvB8Iq4xsH_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq4xsH : MetalKqMvB8Iq4T {
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_iq4nl_c", pso = "g_pso_kq_mvb2h_iq4nl", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq4nlH : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override IQ4NL = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_iq4nl_c", pso = "g_pso_kq_mvb4h_iq4nl", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq4nlH : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override IQ4NL = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_iq4nl_c", pso = "g_pso_kq_mvb8h_iq4nl", tgmem = "MetalKqMvB8Iq4nlH_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq4nlH : MetalKqMvB8Iq4T {
    override IQ4NL = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_q40_c", pso = "g_pso_kq_mvb2h_q40", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Q40H : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override IQ4NL = true
    override Q40 = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_q40_c", pso = "g_pso_kq_mvb4h_q40", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Q40H : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override IQ4NL = true
    override Q40 = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_q40_c", pso = "g_pso_kq_mvb8h_q40", tgmem = "MetalKqMvB8Q40H_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Q40H : MetalKqMvB8Iq4T {
    override IQ4NL = true
    override Q40 = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_nv4_c", pso = "g_pso_kq_mvb2h_nv4", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Nv4H : MetalKqMvIq4T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override NV4 = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_nv4_c", pso = "g_pso_kq_mvb4h_nv4", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Nv4H : MetalKqMvIq4T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override NV4 = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_nv4_c", pso = "g_pso_kq_mvb8h_nv4", tgmem = "MetalKqMvB8Nv4H_metal_kq_mvb8_iq4_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Nv4H : MetalKqMvB8Iq4T {
    override NV4 = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_k3_c", pso = "g_pso_kq_mvb2h_k3", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K3H : MetalKqMvK3T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_k3_c", pso = "g_pso_kq_mvb4h_k3", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K3H : MetalKqMvK3T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_k3_c", pso = "g_pso_kq_mvb8h_k3", tgmem = "MetalKqMvB8K3H_metal_kq_mvb8_k3_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K3H : MetalKqMvB8K3T {
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_iq3s_c", pso = "g_pso_kq_mvb2h_iq3s", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq3sH : MetalKqMvIq3sT {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_iq3s_c", pso = "g_pso_kq_mvb4h_iq3s", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq3sH : MetalKqMvIq3sT {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_iq3s_c", pso = "g_pso_kq_mvb8h_iq3s", tgmem = "MetalKqMvB8Iq3sH_metal_kq_mvb8_iq3s_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq3sH : MetalKqMvB8Iq3sT {
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_iq3xxs_c", pso = "g_pso_kq_mvb2h_iq3xxs", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq3xxsH : MetalKqMvIq3xxsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_iq3xxs_c", pso = "g_pso_kq_mvb4h_iq3xxs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq3xxsH : MetalKqMvIq3xxsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_iq3xxs_c", pso = "g_pso_kq_mvb8h_iq3xxs", tgmem = "MetalKqMvB8Iq3xxsH_metal_kq_mvb8_iq3xxs_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq3xxsH : MetalKqMvB8Iq3xxsT {
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_k2_c", pso = "g_pso_kq_mvb2h_k2", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2K2H : MetalKqMvK2T {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_k2_c", pso = "g_pso_kq_mvb4h_k2", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4K2H : MetalKqMvK2T {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_k2_c", pso = "g_pso_kq_mvb8h_k2", tgmem = "MetalKqMvB8K2H_metal_kq_mvb8_k2_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8K2H : MetalKqMvB8K2T {
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_iq2s_c", pso = "g_pso_kq_mvb2h_iq2s", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq2sH : MetalKqMvIq2sT {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_iq2s_c", pso = "g_pso_kq_mvb4h_iq2s", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq2sH : MetalKqMvIq2sT {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_iq2s_c", pso = "g_pso_kq_mvb8h_iq2s", tgmem = "MetalKqMvB8Iq2sH_metal_kq_mvb8_iq2s_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq2sH : MetalKqMvB8Iq2sT {
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_iq2xs_c", pso = "g_pso_kq_mvb2h_iq2xs", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq2xsH : MetalKqMvIq2xsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_iq2xs_c", pso = "g_pso_kq_mvb4h_iq2xs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq2xsH : MetalKqMvIq2xsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_iq2xs_c", pso = "g_pso_kq_mvb8h_iq2xs", tgmem = "MetalKqMvB8Iq2xsH_metal_kq_mvb8_iq2xs_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq2xsH : MetalKqMvB8Iq2xsT {
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb2h_iq2xxs_c", pso = "g_pso_kq_mvb2h_iq2xxs", tg = 64, grid = "rows/8", params = "rows : int64", requires = "ka.ndim % 256")]
class MetalKqMvB2Iq2xxsH : MetalKqMvIq2xxsT {
    override NR = 2
    override NRU = 2u
    override TILED = false
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb4h_iq2xxs_c", pso = "g_pso_kq_mvb4h_iq2xxs", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB4Iq2xxsH : MetalKqMvIq2xxsT {
    override NR = 4
    override NRU = 4u
    override TILED = true
    override XHALF = true
}

[metal_dispatch(name = "enc_kq_mvb8h_iq2xxs_c", pso = "g_pso_kq_mvb8h_iq2xxs", tgmem = "MetalKqMvB8Iq2xxsH_metal_kq_mvb8_iq2xxs_msl_tgmem", tg = 64, grid = "rows/8, gcols", params = "rows : int64, gcols : int64", requires = "ka.ndim % 256")]
class MetalKqMvB8Iq2xxsH : MetalKqMvB8Iq2xxsT {
    override XHALF = true
}

//! Per-head RMSNorm on Q/K before rope (the qk-norm models) on the template both homes stamp (`GkQkRms`): head group x of row y,
//! the q heads then the k heads; decode covers q+k in ONE grid over the fused QKV buffer (bound as both row planes), prefill
//! dispatches separate q/k panels per-plane
[metal_dispatch(name = "enc_qk_norm", pso = "g_pso_qknorm", tgmem = "metal_qk_norm_msl_tgmem", tg = 256, grid = "ntg, nrows", params = "ntg : int64, nrows : int64")]
class MetalQkNorm : GkQkRms {
    [metal_kernel(name="metal_qk_norm_msl")]
    def metal_qk_norm {
        qk_rms_group(gl_WorkGroupID.y, gl_WorkGroupID.x)
    }
}

//! the per-head norm's args over Metal's one-buffer rows `rstride` apart: `nq` q heads at a row's start, then `nk` k heads read
//! at `soffe` and written at `koffe` past it (the no-wv V fill reads the pre-norm K slice and writes the v segment), the k
//! weight row at `woffe`; every base `xoff` bytes into the buffer
def qk_norm_args(nq, nk, hs, rstride, soffe, koffe, woffe : uint; eps : float; xoff : uint64 = 0ul) : GkQkRmsArgs {
    let e = uint(xoff / 4ul)
    return GkQkRmsArgs(hs = hs, nh = nq, nkvh = nk, qstride = rstride, kstride = rstride, qbase = e, kbase = e + soffe, kwd = koffe - soffe,
        qwoff = 0u, kwoff = woffe, eps = eps)
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
    @ssbo @binding = 5 @role = "read" @off = "toff" @template_gate = TOTB totb : array<uint>   // the device-side row count (the bucket rail's padded total)
    @uniform @binding = 6 @template_gate = TOTB row_elems : uint
    @ssbo @binding = 7 @role = "write" @off = "ooff" @template_gate = OUT o : array<float>   //!< the out-of-place result (the in-place stamps erase it)
    @template_constant HAS_ASCALE : bool = false
    @template_constant HX : bool = false
    @template_constant TOTB : bool = false
    @template_constant OUT : bool = false
    @template_constant ACT : int = 0   //!< the combine, spelled as its literal: 0 silu(a)*b, 1 gelu_tanh(a)*b, 2 (a+b)*ascale, 3 a*sigmoid(b)

    def combine(a, b : float) : float {
        static_if (ACT == 0) {
            return gk_act_mul(a, b, 0u)
        } static_elif (ACT == 1) {
            return gk_act_mul(a, b, 1u)
        } static_elif (ACT == 2) {
            return (a + b) * ascale
        } else {
            return sig_gate(a, b)
        }
    }

    [metal_kernel]
    def metal_ew2 {
        let gid = gl_GlobalInvocationID.x
        static_if (TOTB) {
            if (gid >= totb[0] * row_elems) {
                return
            }
        } else {
            if (gid >= total) {
                return
            }
        }
        let res = combine(g[gid], u[gid])
        static_if (OUT) {
            o[gid] = res
        } else {
            g[gid] = res
        }
        static_if (HX) {
            gh[gid] = float16(res)
        }
    }
}

[metal_dispatch(name = "enc_swiglu", pso = "g_pso_swiglu", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSwiglu : MetalEw2T {}

[metal_dispatch(name = "enc_swiglu_hx", pso = "g_pso_swiglu_hx", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSwigluHX : MetalEw2T {
    override HX = true
}

[metal_dispatch(name = "enc_swiglu_hxb", pso = "g_pso_swiglu_hxb", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSwigluHXB : MetalEw2T {
    override HX = true
    override TOTB = true
}

[metal_dispatch(name = "enc_swiglu_b", pso = "g_pso_swiglu_b", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSwigluB : MetalEw2T {
    override TOTB = true
}

[metal_dispatch(name = "enc_geglu", pso = "g_pso_geglu", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGeglu : MetalEw2T {
    override ACT = 1
}

//! the geglu over the E-series' side rows on the template both homes stamp (`GkPleAct`): in place over the gate rows
[metal_dispatch(name = "enc_geglu_side", pso = "g_pso_geglu_side", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGegluSide : GkPleAct {
    [metal_kernel(name = "metal_geglu_side_msl")]
    def metal_geglu_side {
        act_elem(gl_GlobalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_geglu_hxb", pso = "g_pso_geglu_hxb", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGegluHXB : MetalEw2T {
    override HX = true
    override TOTB = true
    override ACT = 1
}

[metal_dispatch(name = "enc_geglu_b", pso = "g_pso_geglu_b", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGegluB : MetalEw2T {
    override TOTB = true
    override ACT = 1
}

[metal_dispatch(name = "enc_geglu_hx", pso = "g_pso_geglu_hx", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGegluHX : MetalEw2T {
    override HX = true
    override ACT = 1
}

//! gemma4v's GEGLU-quick with its bounds fused, on the template both homes stamp (`total` = the elements over four)
[metal_dispatch(name = "enc_geglu_quick", pso = "g_pso_geglu_quick", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGegluQuick : GkGeglu {
    [metal_kernel(name = "metal_geglu_quick_msl")]
    def metal_geglu_quick {
        geglu_body()
    }
}

// residual add: x += y, scaled — gemma4 layer_out_scale scales the WHOLE layer output
[metal_dispatch(name = "enc_add_c", pso = "g_pso_add", tg = 64, grid = "total/64", params = "total : int64")]
class MetalAdd : MetalEw2T {
    override ACT = 2
    override HAS_ASCALE = true
}

// per-head sigmoid output gate (sigmoid_gate): x ⊙ σ(g) in place — the gated-attention
// epilogue between the attention output and the out projection
[metal_dispatch(name = "enc_sigmul", pso = "g_pso_sigmul", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSigmul : MetalEw2T {
    override ACT = 3
}

//! the classifier tail's selection on the template both homes stamp (`GkClsSelect`), a threadgroup of 1024 threads a row; no
//! fast math: the lane floor reads NaN lanes through the ordered compare the host's `sel_lane_val` makes
[metal_dispatch(name = "enc_cls_select", pso = "g_pso_cls_select", tgmem = "metal_cls_select_msl_tgmem", tg = 1024, grid = "rows", params = "rows : int64")]
class MetalClsSelect : GkClsSelect {
    [metal_kernel(name="metal_cls_select_msl", fastmath=false)]
    def metal_cls_select {
        select_row()
    }
}

//! the classifier tail's epilogue on the template both homes stamp (`GkClsEpilogue`), a thread a logit over every row
[metal_dispatch(name = "enc_cls_epi", pso = "g_pso_cls_epi", tg = 256, grid = "total/256", params = "total : int64")]
class MetalClsEpilogue : GkClsEpilogue {
    [metal_kernel(name="metal_cls_epi_msl")]
    def metal_cls_epi {
        epi_body()
    }
}

//! the epilogue over `rows` logits rows `vocab` wide in `by` from element 0: the final softcap (`cap` 0 = none), then the `nsupp`
//! suppressed ids in `bsupp` pinned - one dispatch; `bsupp` null carries no pin
def enc_cls_epi_rows(enc : MetalComputeEncoder?; by, bsupp : MetalBuffer?; vocab, rows : int64; cap : float; nsupp : int64) {
    var ka = GkClsEpiArgs(vocab = uint(vocab), cap = cap, nsupp = bsupp != null ? uint(nsupp) : 0u, rows = uint(rows))
    enc_cls_epi(enc, by, bsupp != null ? bsupp : by, ka, vocab * rows)
}

// Causal depthwise conv over the fused [q;k;v] rows + SiLU (dn_conv_step's batch form): one
// thread per (channel, position, group); positions reaching before a group's rows read its
// channel-major history taps (the CPU dn_conv_state layout). Never writes history — MetalDnConvHist advances.
[metal_dispatch(name = "enc_dn_conv", pso = "g_pso_dn_conv", tg = 256, grid = "cd/256, npos, ng", params = "cd : int64, npos : int64, ng : int64")]
class MetalDnConv {
    @ssbo @binding = 0 x : array<float>     //! [rows x cd] fused qkv projection rows
    @ssbo @binding = 1 hist : array<float>  //! the mirror pool: [cd x taps] channel-major history at each group's base
    @ssbo @binding = 2 @role = "weight" w : array<float>     // [cd x dconv] conv taps (raw fblob layout)
    @ssbo @binding = 3 y : array<float>     //! [rows x cd] conv output, silu'd
    @ssbo @binding = 4 grp : array<uint>    //! the group table, eight words a group (dn_grp_push)
    @uniform @binding = 5 ka : DnArgs

    [metal_kernel(name="metal_dn_conv_msl")]
    def metal_dn_conv {
        let cd = ka.cd
        let dconv = ka.dconv
        let ch = gl_GlobalInvocationID.x
        let p = gl_GlobalInvocationID.y
        let ge = gl_WorkGroupID.z * 8u
        if (ch >= cd || p >= grp[ge + 1u]) {
            return
        }
        let row0 = grp[ge]
        let hb = grp[ge + 4u] + ka.cslice
        let taps = dconv - 1u
        var acc = 0.0
        var k = 0u
        while (k < dconv) {
            let sp = int(p + k) - int(taps)
            let xv = sp >= 0 ? x[(row0 + uint(sp)) * cd + ch] : hist[hb + ch * taps + (p + k)]
            acc += xv * w[ch * dconv + k]
            k++
        }
        y[(row0 + p) * cd + ch] = silu_f32(acc)
    }
}

// Advance each group's channel-major conv history past its rows: the new taps are its last
// `taps` input rows, shifted in from the old history when the rows are fewer (straddle). Reads
// land in registers before any write, so the in-place buffer update is safe per channel.
[metal_dispatch(name = "enc_dn_conv_hist", pso = "g_pso_dn_hist", tg = 256, grid = "cd/256, ng", params = "cd : int64, ng : int64")]
class MetalDnConvHist {
    @ssbo @binding = 0 x : array<float>     //! [rows x cd] the fused qkv rows
    @ssbo @binding = 1 hist : array<float>  //! the mirror pool: each group's live history
    @ssbo @binding = 2 @role = "alias" histm : array<float> //! dual-store: the history one row short of the group's (verify shadow region)
    @ssbo @binding = 3 grp : array<uint>    //! the group table, eight words a group (dn_grp_push), then its capture records
    @uniform @binding = 4 ka : DnArgs
    @ssbo @binding = 5 @role = "alias" capb : array<float>   //! the capture arena: the taps rows before each capture's row land at the record's history base (the pool when no group captures)

    [metal_kernel(name="metal_dn_conv_hist_msl")]
    def metal_dn_conv_hist {
        let cd = ka.cd
        let dconv = ka.dconv
        let mid = ka.mid
        let ch = gl_GlobalInvocationID.x
        let ge = gl_WorkGroupID.y * 8u
        if (ch >= cd) {
            return
        }
        let row0 = grp[ge]
        let npos = grp[ge + 1u]
        let hb = grp[ge + 4u] + ka.cslice
        let hmb = grp[ge + 5u] + ka.cslice
        let taps = dconv - 1u
        var nh : float[8]                   // dconv <= 8 by the serve gate
        var k = 0u
        while (k < taps) {
            nh[k] = tap(row0, hb, ch, npos, k)
            k++
        }
        if (mid != 0u) {                    // every hist read precedes the in-place write below
            let short_row = npos - 1u
            k = 0u
            while (k < taps) {
                histm[hmb + ch * taps + k] = tap(row0, hb, ch, short_row, k)
                k++
            }
        }
        var ci = 0u
        while (ci < grp[ge + 6u]) {
            let cw = grp[ge + 7u] + ci * 3u
            let crow = grp[cw]
            let cb = grp[cw + 2u] + ka.cslice
            k = 0u
            while (k < taps) {
                capb[cb + ch * taps + k] = tap(row0, hb, ch, crow, k)
                k++
            }
            ci++
        }
        k = 0u
        while (k < taps) {
            if (ka.split != 0u) {
                histm[hmb + ch * taps + k] = nh[k]
            } else {
                hist[hb + ch * taps + k] = nh[k]
            }
            k++
        }
    }

    //! channel `ch`'s tap `k` of the history standing after the group's window row `row`: an input row, or the old history before the window
    def tap(row0, hb, ch, row, k : uint) : float => (int(row + k) >= int(ka.dconv - 1u)
        ? x[(row0 + row + k - (ka.dconv - 1u)) * ka.cd + ch]
        : hist[hb + ch * (ka.dconv - 1u) + row + k])
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
        let base = (ka.row + p) * cd + blk * ds + lane * 4u
        var v : float[4]
        var ss = 0.0
        let yp = unsafe(addr(y[base]))
        for [unroll_full] (c in range(4)) {
            v[c] = unsafe(yp[c])
            ss += v[c] * v[c]
        }
        ss = simd_sum(ss)
        let sc = (1.0 / max(sqrt(ss), eps)) * (blk < kd / ds ? qscale : 1.0)
        for [unroll_full] (c in range(4)) {
            y[base + uint(c)] = v[c] * sc
        }
    }
}

//! the conv front on the template both homes stamp (`GkDnConv`): every group's rows over its own channel-major history in the
//! mirror pool, the group table's eight words a group (`dn_grp_push`), up to eight taps
[ |> template_struct_instance]
class template MetalDnConvNT : GkDnConv {
    override RING = 8
    @ssbo @binding = 2 @role = "read" hist : array<float>     //! the mirror pool: [cd x taps] channel-major history at each group's base
    @ssbo @binding = 3 @role = "weight" w : array<float>      //! [cd x dconv] conv taps
    @ssbo @binding = 4 @role = "read" grp : array<uint>

    def override seg_row0 : uint => pa.row0 + grp[gl_WorkGroupID.z * 8u]
    def override seg_npos : uint => grp[gl_WorkGroupID.z * 8u + 1u]
    def override tap_at(ch, j : uint) : float => w[ch * pa.dconv + j]
    def override hist_at(ch, taps, hidx : uint) : float => hist[grp[gl_WorkGroupID.z * 8u + 4u] + pa.hist_off + ch * taps + hidx]
}

[metal_dispatch(name = "enc_dn_conv_n", pso = "g_pso_dn_conv_n", tgmem = "metal_dn_conv_n_msl_tgmem", tg = 256, grid = "wgs, 1, ng",
    params = "wgs : int64, ng : int64")]
class MetalDnConvN : MetalDnConvNT {
    override PB = 1

    [metal_kernel(name = "metal_dn_conv_n_msl")]
    def metal_dn_conv_n {
        dn_conv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_dn_conv_n4", pso = "g_pso_dn_conv_n4", tgmem = "metal_dn_conv_n4_msl_tgmem", tg = 256, grid = "wgs, 1, ng",
    params = "wgs : int64, ng : int64")]
class MetalDnConvN4 : MetalDnConvNT {
    override PB = 4

    [metal_kernel(name = "metal_dn_conv_n4_msl")]
    def metal_dn_conv_n4 {
        dn_conv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! the fused conv's args off the stage args: the channel-major history at the layer's slice
def dn_conv_args(ka : DnArgs) : GkDnConvArgs => GkDnConvArgs(cd = ka.cd, kd = ka.kd, ds = ka.ds, dconv = ka.dconv, hist_mode = 1u,
    hist_off = ka.cslice, eps = ka.eps, qscale = ka.qscale)

//! the fused conv's workgroups over `npmax` rows a group
def dn_conv_n_wgs(cd, npmax : int64; pb : int64 = 1l) : int64 => ((cd + 255l) / 256l) * ((npmax + pb - 1l) / pb)

[ |> template_struct_instance]
class template MetalDnScanT {
    @template_constant PREP : bool = false   //! braw / graw hold beta and the decay (MetalDnPrep wrote them), not the raw projections
    @ssbo @binding = 0 cv : array<CT>       //! [rows x cd] conv rows: q at 0 / k at kd / v at 2kd
    @ssbo @binding = 1 st : array<float>    //! the mirror pool: each group's state layer [nvh x ds x ds] row-major (i·ds + j)
    @ssbo @binding = 2 braw : array<float>  //! [rows x nvh] raw beta projections
    @ssbo @binding = 3 graw : array<float>  //! [rows x nvh] raw alpha projections
    @ssbo @binding = 4 @role = "weight" aa : array<float>    // [nvh] ssm_a = -exp(A_log) (fblob row)
    @ssbo @binding = 5 @role = "weight" dtb : array<float>   // [nvh] dt_bias (fblob row)
    @ssbo @binding = 6 o : array<float>     //! [rows x di] delta-rule output rows
    @ssbo @binding = 7 @role = "alias" stm : array<float>  //! dual-store: the state one row short of the group's (verify shadow region)
    @ssbo @binding = 8 grp : array<uint>    //! the group table, eight words a group (dn_grp_push), then its capture records
    @uniform @binding = 9 ka : DnArgs
    @ssbo @binding = 10 @role = "alias" capb : array<float>   //! the capture arena: a group's state after a capture's row lands at the record's state base (the pool when no group captures)

    [metal_kernel, arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-dn-scan")]
    def metal_dn_scan {   // nolint:STYLE038 — MSL kernel body; the walk's state is register-resident across the whole loop
        let cd = ka.cd
        let kd = ka.kd
        let ds = ka.ds
        let nkh = ka.nkh
        let di = ka.di
        let mid = ka.mid
        let sg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let h = gl_WorkGroupID.y
        let ge = gl_WorkGroupID.z * 8u
        if (sg * 8u >= ds) {
            return
        }
        let row0 = grp[ge]
        let npos = grp[ge + 1u]
        let sb = grp[ge + 2u] + ka.sslice
        let smb = grp[ge + 3u] + ka.sslice
        let lane = gl_SubgroupInvocationID
        let j = sg * 8u + lane / 4u
        let r = lane & 3u
        let kh = h % nkh
        let qoff = kh * ds + r * 32u
        let koff = kd + kh * ds + r * 32u
        let voff = 2u * kd + h * ds + j
        let a_h = aa[h]
        let dt_h = dtb[h]
        let s0 = h * ds * ds + (r * 32u) * ds + j
        var ls : float[32]
        for [unroll_full] (e in range(32)) {
            ls[e] = st[sb + s0 + uint(e) * ds]
        }
        var p = 0u
        while (p < npos) {
            var dec = graw[ka.goff + (row0 + p) * ka.bstr + h]
            var beta = braw[(row0 + p) * ka.bstr + h]
            static_if (!PREP) {
                dec = dn_decay(a_h, dec + dt_h)
                beta = sigmoid_f32(beta)
            }
            let cb = (row0 + p) * cd
            let kp = unsafe(addr(cv[cb + koff]))
            let qp = unsafe(addr(cv[cb + qoff]))
            var part : float[8]
            for [unroll_full] (pb in range(8)) {
                var acc = 0.0
                for [unroll_full] (c in range(4)) {
                    ls[pb * 4 + c] *= dec
                    acc += float(unsafe(kp[pb * 4 + c])) * ls[pb * 4 + c]   // nolint:PERF020 — the half stamp's row widens here; on the f32 stamp the cast is the identity
                }
                part[pb] = acc
            }
            var sk = ((part[0] + part[1]) + (part[2] + part[3])) + ((part[4] + part[5]) + (part[6] + part[7]))
            sk += simd_shuffle_xor(sk, 1u)
            sk += simd_shuffle_xor(sk, 2u)
            let d = (float(cv[cb + voff]) - sk) * beta   // nolint:PERF020 — same widening
            for [unroll_full] (pb in range(8)) {
                var acc = 0.0
                for [unroll_full] (c in range(4)) {
                    ls[pb * 4 + c] += float(unsafe(kp[pb * 4 + c])) * d   // nolint:PERF020 — same widening
                    acc += float(unsafe(qp[pb * 4 + c])) * ls[pb * 4 + c]   // nolint:PERF020 — same widening
                }
                part[pb] = acc
            }
            var yy = ((part[0] + part[1]) + (part[2] + part[3])) + ((part[4] + part[5]) + (part[6] + part[7]))
            yy += simd_shuffle_xor(yy, 1u)
            yy += simd_shuffle_xor(yy, 2u)
            if (r == 0u) {
                o[(row0 + p) * di + h * ds + j] = yy
            }
            if (mid != 0u && p + 2u == npos) {
                for [unroll_full] (e in range(32)) {
                    stm[smb + s0 + uint(e) * ds] = ls[e]
                }
            }
            var ci = 0u
            while (ci < grp[ge + 6u]) {
                let cw = grp[ge + 7u] + ci * 3u
                if (grp[cw] == p + 1u) {
                    let capbase = grp[cw + 1u] + ka.sslice + s0
                    for [unroll_full] (e in range(32)) {
                        capb[capbase + uint(e) * ds] = ls[e]
                    }
                }
                ci++
            }
            p++
        }
        for [unroll_full] (e in range(32)) {
            if (ka.split != 0u) {
                stm[smb + s0 + uint(e) * ds] = ls[e]
            } else {
                st[sb + s0 + uint(e) * ds] = ls[e]
            }
        }
    }
}

[metal_dispatch(name = "enc_dn_scan", pso = "g_pso_dn_scan", tg = 64, grid = "ds/16, nvh, ng", params = "ds : int64, nvh : int64, ng : int64", requires = "ds % 32")]
class MetalDnScan : MetalDnScanT {
    typedef CT = float
}

//! the prefill window's stamp: half conv rows, the gates prepared
[metal_dispatch(name = "enc_dn_scan_h", pso = "g_pso_dn_scan_h", tg = 64, grid = "ds/16, nvh, ng", params = "ds : int64, nvh : int64, ng : int64", requires = "ds % 32")]
class MetalDnScanH : MetalDnScanT {
    typedef CT = float16
    override PREP = true
}

[metal_dispatch(name = "enc_dn_prep", pso = "g_pso_dn_prep", tg = 64, grid = "total/64", params = "total : int64")]
class MetalDnPrep {
    @ssbo @binding = 0 braw : array<float>  // [npos x nvh] raw beta in, beta out
    @ssbo @binding = 1 graw : array<float>  // [npos x nvh] raw alpha in, the decay out
    @ssbo @binding = 2 @role = "weight" aa : array<float>    // [nvh] ssm_a
    @ssbo @binding = 3 @role = "weight" dtb : array<float>   // [nvh] dt_bias
    @uniform @binding = 4 ka : DnArgs

    [metal_kernel(name="metal_dn_prep_msl"), arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-dn-scan")]
    def metal_dn_prep {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.npos * ka.nvh) {
            return
        }
        let h = gid % ka.nvh
        let i = ka.row * ka.nvh + gid
        graw[i] = dn_decay(aa[h], graw[i] + dtb[h])
        braw[i] = sigmoid_f32(braw[i])
    }
}

//! The gated out-norm (dn_token_core's tail) on the template both homes stamp (`GkDnGate`): one simdgroup a (v-head, position),
//! eight a threadgroup; the inverse norm `rsqrt`
[ |> template_struct_instance]
class template MetalDnGateT : GkDnGate {
    override RSQRT = true

    def dn_gate_grid {
        let h = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (h < pa.nvh) {
            dn_gate_head(h, gl_WorkGroupID.y)
        }
    }
}

[metal_dispatch(name = "enc_dn_gate", pso = "g_pso_dn_gate", tg = 256, grid = "nvh/8, npos", params = "nvh : int64, npos : int64")]
class MetalDnGate : MetalDnGateT {
    [metal_kernel]
    def metal_dn_gate {
        dn_gate_grid()
    }
}

[metal_dispatch(name = "enc_dn_gate_sig", pso = "g_pso_dn_gate_sig", tg = 256, grid = "nvh/8, npos", params = "nvh : int64, npos : int64")]
class MetalDnGateSig : MetalDnGateT {
    override ZSIG = true

    [metal_kernel]
    def metal_dn_gate {
        dn_gate_grid()
    }
}

//! the deltanet output gate over `rows` rows from the stages' first row (`dka.row`): the family's z activation - sigmoid
//! (qwen4exp) or silu
def enc_dn_zgate(enc : MetalComputeEncoder?; c : Config; bo, bz, bwn : MetalBuffer?; dka : DnArgs; nvh, rows : int64) {
    var gka = GkDnGateArgs(nvh = uint(nvh), di = dka.di, ds = dka.ds, o_off = dka.row * dka.di, z_off = dka.row * dka.di, eps = dka.eps)
    if (c.dn_z_sigmoid) {
        enc_dn_gate_sig(enc, bz, bo, bwn, gka, nvh, rows)
    } else {
        enc_dn_gate(enc, bz, bo, bwn, gka, nvh, rows)
    }
}

//! One recurrent layer's planes over every group of rows - a group is one session's rows against its mirror's slot, rows
//! back to back in the planes - with the group table and the shapes the stages dispatch over
struct DnStages {
    qkv : MetalBuffer?   //! [rows x cd] the projections
    cv : MetalBuffer?    //! [rows x cd] the conv rows, normed in place
    cvh : MetalBuffer?   //! the conv rows as halves over prepared gates (the prefill window); null = the f32 scan
    b : MetalBuffer?
    g : MetalBuffer?
    o : MetalBuffer?
    z : MetalBuffer?     //! the gate rows; null = no gate (the replay)
    grp : MetalBuffer?
    capb : MetalBuffer?  //! the capture arena the group table's records index; null = no group captures (the stages bind the pool)
    ng : int64
    npmax : int64        //! the most rows one group carries
    rows : int64
    mid : bool
    split : bool
    ba_rows : bool       //! `b` holds each row's [beta; alpha] and `g` is `b` (one GEMV wrote both)
}

var private @scratch g_dn_grp_w : array<uint>

//! a fresh group table: the words `dn_grp_push` appends and `dn_grp_upload` hands to a buffer
def dn_grp_clear {
    g_dn_grp_w |> clear()
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#dn-group-stages")]
def dn_grp_push(row0, npos : int64; soff, salt, coff, calt : uint64; ncap : int64 = 0l; capw : int64 = 0l) {
    g_dn_grp_w |> push(uint(row0))
    g_dn_grp_w |> push(uint(npos))
    g_dn_grp_w |> push(uint(soff / 4ul))
    g_dn_grp_w |> push(uint(salt / 4ul))
    g_dn_grp_w |> push(uint(coff / 4ul))
    g_dn_grp_w |> push(uint(calt / 4ul))
    g_dn_grp_w |> push(uint(ncap))
    g_dn_grp_w |> push(uint(capw))
}

let DN_GRP_WORDS = 8l
let DN_CAP_WORDS = 3l   //! a capture's: the row the state stands after, its state base and its history base (floats into the capture arena)

[arch(at="../ARCHITECTURE_GPU_MTP.md#dn-group-stages")]
def dn_grp_cap(row : int64; soff, hoff : uint64) {
    g_dn_grp_w |> push(uint(row))
    g_dn_grp_w |> push(uint(soff / 4ul))
    g_dn_grp_w |> push(uint(hoff / 4ul))
}

//! the table as a pooled buffer; the caller hands it back to `g_upool` at its byte size once the step completes
def dn_grp_upload() : tuple<buf : MetalBuffer?; bytes : uint64> {
    let bytes = uint64(long_length(g_dn_grp_w) * 4l)
    var b = pool_acquire_untracked(g_upool, g_dev, bytes)
    unsafe {
        memcpy(metal_buffer_contents(b), addr<void?>(g_dn_grp_w[0]), int64(bytes))
    }
    return (buf = b, bytes = bytes)
}

def private dn_stage_args(c : Config; d : DnStages; dnli : int64) : DnArgs {
    var ka = dn_args(c, d.rows, d.mid)
    ka.split = d.split ? 1u : 0u
    ka.sslice = uint(dnli * c.ssm_dt_rank * c.ssm_d_state * c.ssm_d_state)
    ka.cslice = uint(dnli * (c.ssm_d_conv - 1l) * dn_conv_dim(c))
    if (d.ba_rows) {
        ka.bstr = uint(2l * c.ssm_dt_rank)
        ka.goff = uint(c.ssm_dt_rank)
    }
    return ka
}

//! the history advance of every group past its rows
def enc_dn_hist(enc : MetalComputeEncoder?; c : Config; dnli : int64; d : DnStages) {
    var ka = dn_stage_args(c, d, dnli)
    let pool = dn_pool_buffer()
    enc_dn_conv_hist(enc, d.qkv, pool, pool, d.grp, ka, d.capb != null ? d.capb : pool, dn_conv_dim(c), d.ng)
}

//! the most rows a group carries that the fused conv + L2 serves (`GkDnConv`, ARCHITECTURE_GPU_SHARED_KERNELS.md's reading); a window
//! past it takes the conv and the L2 pass apart
let DN_CONV_FUSED_ROWS = 16l

[arch(at="../ARCHITECTURE_GPU_MTP.md#dn-group-stages")]
def enc_dn_front(enc : MetalComputeEncoder?; t : Model; l, dnli : int64; d : DnStages) {
    let c = t.config
    let cd = dn_conv_dim(c)
    var ka = dn_stage_args(c, d, dnli)
    var bw_cv = upload_region(unsafe(addr < void? >(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * cd])), uint64(c.ssm_d_conv * cd * 4l))
    if (d.npmax > DN_CONV_FUSED_ROWS) {
        enc_dn_conv(enc, d.qkv, dn_pool_buffer(), bw_cv, d.cv, d.grp, ka, cd, d.npmax, d.ng)
        enc_dn_hist(enc, c, dnli, d)
        enc_dn_l2norm(enc, d.cv, ka, 2l * c.ssm_n_group, d.rows)
        return
    }
    var kn = dn_conv_args(ka)
    if (d.npmax == 1l) {
        enc_dn_conv_n(enc, d.qkv, d.cv, dn_pool_buffer(), bw_cv, d.grp, kn, dn_conv_n_wgs(cd, 1l), d.ng)
    } else {
        enc_dn_conv_n4(enc, d.qkv, d.cv, dn_pool_buffer(), bw_cv, d.grp, kn, dn_conv_n_wgs(cd, d.npmax, 4l), d.ng)
    }
    enc_dn_hist(enc, c, dnli, d)
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#dn-group-stages")]
def enc_dn_back(enc : MetalComputeEncoder?; t : Model; l, dnli : int64; d : DnStages) {
    let c = t.config
    let nvh = c.ssm_dt_rank
    var ka = dn_stage_args(c, d, dnli)
    let pool = dn_pool_buffer()
    unsafe {
        var baa = upload_region(addr < void? >(t.fblob[t.dn_a_off + l * nvh]), uint64(nvh * 4l))
        var bdt = upload_region(addr < void? >(t.fblob[t.dn_dt_off + l * nvh]), uint64(nvh * 4l))
        let capb = d.capb != null ? d.capb : pool
        if (d.cvh != null) {
            enc_dn_scan_h(enc, d.cvh, pool, d.b, d.g, baa, bdt, d.o, pool, d.grp, ka, capb, c.ssm_d_state, nvh, d.ng)
        } else {
            enc_dn_scan(enc, d.cv, pool, d.b, d.g, baa, bdt, d.o, pool, d.grp, ka, capb, c.ssm_d_state, nvh, d.ng)
        }
        if (d.z != null) {
            var bwn = upload_region(addr < void? >(t.fblob[t.dn_norm_off + l * c.ssm_d_state]), uint64(c.ssm_d_state * 4l))
            enc_dn_zgate(enc, c, d.o, d.z, bwn, ka, nvh, d.rows)
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

// ===== hyper-connections (qwen4exp): the wide residual's mixer and scatter =====

//! The mixer's pre-norm on the template both homes stamp (`GkHcNorm`): one threadgroup a stream row, rows = hc for the
//! single row, npos x hc for a window of wide rows (the converter folded (1 + w) into w); the inverse norm `rsqrt`
[metal_dispatch(name = "enc_hc_norm", pso = "g_pso_hc_norm", tgmem = "metal_hc_norm_msl_tgmem", tg = 1024, grid = "rows", params = "rows : int64")]
class MetalHcNorm : GkHcNorm {
    override TG = 1024u
    override RSQRT = true

    [metal_kernel(name="metal_hc_norm_msl")]
    def metal_hc_norm {
        hc_norm_body()
    }
}

//! the wide residual of nrows rows on the template both homes stamp (`GkHcInit`): every stream of row p starts as row p of x
[metal_dispatch(name = "enc_hc_init", pso = "g_pso_hc_init", tg = 64, grid = "total/64", params = "total : int64")]
class MetalHcInit : GkHcInit {
    [metal_kernel(name="metal_hc_init_msl")]
    def metal_hc_init {
        hc_init_body()
    }
}

//! the MTP head's eh_proj input rows on the template both homes stamp (`GkCat2`)
[metal_dispatch(name = "enc_cat2", pso = "g_pso_cat2", tg = 64, grid = "total/64", params = "total : int64")]
class MetalCat2 : GkCat2 {
    [metal_kernel(name="metal_cat2_msl")]
    def metal_cat2 {
        cat2_body()
    }
}

//! the cat args of `rows` rows of width d: one stream a position, a and b both read from their row 0
def cat2_args(d, rows : int64; rep : int64 = 1l) : GkCat2Args => GkCat2Args(d = uint(d), rows = uint(rows), rep = uint(rep))

//! the grouped norm's args over a model's streams, every gamma row bound at its own offset
def hc_norm_args(c : Config) : GkHcNormArgs => GkHcNormArgs(dim = uint(c.dim), hc = uint(c.hc_count), woff = 0u, row0 = 0u, eps = c.norm_eps)

//! the wide rows' args at `rows` rows - the opening copies' and the mixer tail's
def hc_init_args(c : Config; rows : int64) : GkHcInitArgs => GkHcInitArgs(dim = uint(c.dim), hc = uint(c.hc_count), rows = uint(rows))
def hc_mix_args(c : Config; rows : int64) : GkHcMixArgs => GkHcMixArgs(dim = uint(c.dim), hc = uint(c.hc_count), rows = uint(rows))

//! the scatter's args at `rows` rows from row 0, its MoE arm off
def hc_comb_args(c : Config; rows : int64) : GkHcCombArgs => GkHcCombArgs(dim = uint(c.dim), hc = uint(c.hc_count), rows = uint(rows))

// the mixer's gate input: lo = silu(lo / hc) in place, on the low-rank row
[metal_dispatch(name = "enc_hc_lo", pso = "g_pso_hc_lo", tg = 64, grid = "total/64", params = "total : int64")]
class MetalHcLo : MetalEw1T {
    @uniform @binding = 2 hc : uint

    def override map(a : float) : float => silu_f32(a / float(hc))
}

//! the mixer's collapse over nrows wide rows on the template both homes stamp (`GkHcMix`): the block's input rows
[metal_dispatch(name = "enc_hc_mix", pso = "g_pso_hc_mix", tg = 64, grid = "total/64", params = "total : int64")]
class MetalHcMix : GkHcMix {
    [metal_kernel(name="metal_hc_mix_msl")]
    def metal_hc_mix {
        hc_mix_body()
    }
}

//! the scatter over nrows wide rows on the template both homes stamp (`GkHcCombine`), its MoE arm off: the block output
//! row lands in every stream at 2 sigmoid(inject / hc) - a zero logit is a plain residual add
[metal_dispatch(name = "enc_hc_combine", pso = "g_pso_hc_combine", tg = 64, grid = "total/64", params = "total : int64")]
class MetalHcCombine : GkHcCombine {
    [metal_kernel(name="metal_hc_combine_msl")]
    def metal_hc_combine {
        hc_combine_body()
    }
}

// ===== the n-gram PLE side input (qwen4exp) =====

//! The side input's gate on the template both homes stamp (`GkPleGate`): the conv input row of row p into the window panel's
//! row p (`direct`) or the ring's slot of position pos0 + p; the inverse norms `rsqrt`
[metal_dispatch(name = "enc_ple_gate", pso = "g_pso_ple_gate", tgmem = "metal_ple_gate_msl_tgmem", tg = 1024, grid = "wgs", params = "wgs : int64")]
class MetalPleGate : GkPleGate {
    @ssbo @binding = 4 @role = "write" cin : array<float>   //! the conv input rows: the ring (rows x hc x dim) or the window panel
    override TG = 1024u
    override RSQRT = true

    def override cin_base(r, c : uint) : uint => ((pa.direct != 0u ? r : (pa.pos0 + r) % max(1u, pa.nrows)) * pa.hc + c) * pa.dim + pa.coff

    def override cin_store(e : uint; v : float) {
        cin[e] = v
    }

    [metal_kernel(name="metal_ple_gate_msl")]
    def metal_ple_gate {
        ple_gate_body()
    }
}

//! The dilated depthwise causal conv on the template both homes stamp (`GkPleConv`): tap k of row p reads position
//! pos0 + p - (kern - 1 - k) x dil, the window panel from pos0 on under `direct`, the ring before it
[metal_dispatch(name = "enc_ple_conv", pso = "g_pso_ple_conv", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPleConv : GkPleConv {
    @ssbo @binding = 2 @role = "read" ring : array<float>    //! the history ring (rows x hc x dim)
    @ssbo @binding = 5 @role = "read" panel : array<float>   //! the window panel (nrows x hc x dim); `direct` 0 leaves it unread

    def override row_pos(r : uint) : int => int(pa.pos0) + int(r)

    def override tap(e : uint; src_pos : int; nrows : uint) : float => ((pa.direct != 0u && src_pos >= int(pa.pos0))
        ? panel[pa.coff + uint(src_pos - int(pa.pos0)) * pa.hc * pa.dim + e]
        : ring[pa.ringoff + uint(src_pos % int(nrows)) * pa.hc * pa.dim + e])

    [metal_kernel(name="metal_ple_conv_msl")]
    def metal_ple_conv {
        ple_conv_body()
    }
}

//! The n-gram side input's gate and dilated conv over `nrows` rows from `pos0`, the decode's three row shapes' and the prefill
//! window's: the ring form (`direct` off, `bcin` the ring) writes each row's conv input into the ring and reads every tap there,
//! the panel form writes them into `bcin` and reads the window's own taps there, the earlier ones off the ring at `bring`
def enc_ple_side_rows(enc : MetalComputeEncoder?; t : Model; bhc : MetalBuffer?; hoff : uint64; bkey : MetalBuffer?; koff : uint64;
                      bval : MetalBuffer?; voff : uint64; bgated : MetalBuffer?; goff : uint64; bcin : MetalBuffer?; coff : uint64;
                      bring : MetalBuffer?; roff : uint64; pos0, nrows : int64; direct : bool) {
    let c = t.config
    let hcd = hc_dim(c)
    unsafe {
        var bwn = upload_region(addr < void? >(t.fblob[t.ple_nkey_off]), uint64((3l * hcd + hcd * c.ple_conv_kernel) * 4l))
        var gka = GkPleGateArgs(dim = uint(c.dim), hc = uint(c.hc_count), nrows = uint(ple_hist_rows(c)), pos0 = uint(pos0), eps = c.norm_eps,
            inv_sq = 1.0 / sqrt(float(c.dim)), direct = direct ? 1u : 0u, roff = uint(hoff / 4ul), koff = uint(koff / 4ul),
            voff = uint(voff / 4ul), goff = uint(goff / 4ul), coff = uint(coff / 4ul))
        enc_ple_gate(enc, bkey, bhc, bval, bgated, bcin, bwn, gka, nrows * c.hc_count)
        var cka = GkPleConvArgs(dim = uint(c.dim), hc = uint(c.hc_count), kern = uint(c.ple_conv_kernel), dil = uint(c.ple_ngram_size),
            rows = uint(nrows), pos0 = uint(pos0), direct = direct ? 1u : 0u, roff = uint(hoff / 4ul), goff = uint(goff / 4ul),
            coff = uint(coff / 4ul), ringoff = uint(roff / 4ul))
        enc_ple_conv(enc, bhc, bgated, bring, bwn, bcin, cka, nrows * hcd)
    }
}

//! The im2col of one chunk on the template both homes stamp: the whole [mp x kdim] panel, zeros past the taps, in the pad
//! columns and in the pad rows; the conv's direction and the source layout by push
[metal_dispatch(name = "enc_st2_im2col", pso = "g_pso_st2_im2col", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2Im2col : GkIm2col1d {
    [metal_kernel(name="metal_st2_im2col_msl")]
    def metal_st2_im2col {
        im2col1d_body()
    }
}

[metal_dispatch(name = "enc_st2_colstats", pso = "g_pso_st2_colstats", tgmem = "metal_st2_colstats_msl_tgmem", tg = 256, grid = "c", params = "c : int64")]
class MetalSt2ColStats : MetalTgReduceBase {
    @ssbo @binding = 0 @role = "read" x : array<float>       //!< [t x c]
    @ssbo @binding = 1 @role = "write" stats : array<float>  //!< [2 x c]: the means, then the squared deviations' sums
    @uniform @binding = 2 t : uint
    @uniform @binding = 3 c : uint

    [metal_kernel(name="metal_st2_colstats_msl")]
    def metal_st2_colstats {
        let ch = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        var s = 0.0
        var r = lid
        while (r < t) {
            s += x[r * c + ch]
            r += nt
        }
        let tot = tg_sum(s)
        let mean = tot / float(t)
        var q = 0.0
        r = lid
        while (r < t) {
            let dv = x[r * c + ch] - mean
            q += dv * dv
            r += nt
        }
        barrier()
        let qtot = tg_sum(q)
        if (lid == 0u) {
            stats[ch] = mean
            stats[c + ch] = qtot
        }
    }
}

[metal_dispatch(name = "enc_st2_adain_snake", pso = "g_pso_st2_adain_snake", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2AdainSnake : GkAdain {
    override SNAKE = true

    [metal_kernel(name="metal_st2_adain_snake_msl")]
    def metal_st2_adain_snake {
        adain_body()
    }
}

[metal_dispatch(name = "enc_st2_adain_leaky", pso = "g_pso_st2_adain_leaky", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2AdainLeaky : GkAdain {
    [metal_kernel(name="metal_st2_adain_leaky_msl")]
    def metal_st2_adain_leaky {
        adain_body()
    }
}

//! LeakyReLU in place at the stage's slope - an ew1 map with the slope bound.
[metal_dispatch(name = "enc_st2_leaky", pso = "g_pso_st2_leaky", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2Leaky : MetalEw1BaseT {
    @uniform @binding = 2 slope : float

    def override map(a : float) : float => a > 0.0 ? a : a * slope

    [metal_kernel(name="metal_st2_leaky_msl")]
    def metal_st2_leaky {
        ew1_body()
    }
}

//! out = (zero ? 0 : out) + scale * y - the stage's averaged residual-block sum.
[metal_dispatch(name = "enc_st2_axpy", pso = "g_pso_st2_axpy", tg = 64, grid = "nelem/64", params = "nelem : int64")]
class MetalSt2Axpy : GkAxpy {
    [metal_kernel(name="metal_st2_axpy_msl")]
    def metal_st2_axpy {
        axpy_body()
    }
}

//! the residual join's add-and-scale: the AdaIN blocks' join (1 / sqrt 2) and the generator's averaged stage sum, in place where o is a
//! - off the `MetalEw2T` family onto the template both homes stamp (`GkAddScale`), whose body is the Vulkan twin's
[metal_dispatch(name = "enc_st2_add", pso = "g_pso_st2_add", tg = 64, grid = "nelem/64", params = "nelem : int64")]
class MetalSt2Add : GkAddScale {
    [metal_kernel(name="metal_st2_add_msl")]
    def metal_st2_add {
        add_scale_body()
    }
}

//! y [t + 1][c] = x [t][c] behind one reflected row (row 0 = x's row 1): the last stage's left reflect pad.
[metal_dispatch(name = "enc_st2_reflect1", pso = "g_pso_st2_reflect1", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2Reflect1 {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @uniform @binding = 2 c : uint
    @uniform @binding = 3 total : uint      //!< (t + 1) * c

    [metal_kernel(name="metal_st2_reflect1_msl")]
    def metal_st2_reflect1 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let r = gid / c
        let ci = gid % c
        let src = r == 0u ? 1u : r - 1u
        y[gid] = x[src * c + ci]
    }
}

//! the row gather both homes stamp: a stem's window-sort reorder with the adds off, an embedding lookup with its position and
//! type rows on, the greedy chain's one-row gather from the fp32 token table under the model's embed scale
[metal_dispatch(name = "enc_row_gather", pso = "g_pso_row_gather", tg = 64, grid = "total/64", params = "total : int64")]
class MetalRowGather : GkRowGather {
    [metal_kernel(name="metal_row_gather_msl")]
    def metal_row_gather {
        row_gather_body()
    }
}

//! cat(x [t][c], res [t][rc] at stride rs when with_res, f0 at stride f0s, n at stride ns), zero-padded to width.
[metal_dispatch(name = "enc_st2_concat", pso = "g_pso_st2_concat", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2Concat : GkConcat {
    [metal_kernel(name="metal_st2_concat_msl")]
    def metal_st2_concat {
        concat_body()
    }
}

//! The depthwise transposed pool on rows - the transposed form of the depthwise conv both homes stamp.
[metal_dispatch(name = "enc_st2_pool_dw", pso = "g_pso_st2_pool_dw", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2PoolDw : GkDwConvRows {
    [metal_kernel(name="metal_st2_pool_dw_msl")]
    def metal_st2_pool_dw {
        dw_conv_rows_body()
    }
}

//! One LSTM direction on 4 x hidden lanes; xg holds the GEMM's input gates x . w_ih^T + b as [t x xgs].
struct St2LstmArgs {
    t : uint
    hidden : uint
    xgs : uint       //!< the input-gate rows' stride (4 * hidden padded to the GEMM lattice)
    ystride : uint
    yoff : uint
    reverse : uint
}

[metal_dispatch(name = "enc_st2_lstm", pso = "g_pso_st2_lstm", tgmem = "metal_st2_lstm_msl_tgmem", tg = "lanes", grid = "1", params = "lanes : int64")]
class MetalSt2Lstm {
    @ssbo @binding = 0 @role = "read" xg : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @ssbo @binding = 2 @role = "weight" @off = "woff" whh : array<float4>   //!< [4 hidden x hidden] row-major, four inputs a load
    @uniform @binding = 3 ka : St2LstmArgs
    @workgroup hsh : float[256]
    @workgroup gsh : float[1024]

    [metal_kernel(name="metal_st2_lstm_msl")]
    def metal_st2_lstm {
        let lid = gl_LocalInvocationID.x
        let hd = ka.hidden
        let g = lid / hd
        let j = lid % hd
        let hd4 = hd / 4u
        if (lid < hd) {
            hsh[lid] = 0.0
        }
        var cst = 0.0
        let t_first = ka.reverse != 0u ? ka.t - 1u : 0u   //! the direction, decided once: the step loop walks ti = t_first -/+ step
        var step = 0u
        while (step < ka.t) {
            barrier()
            var acc = 0.0
            let wrow = (g * hd + j) * hd4
            var i4 = 0u
            while (i4 < hd4) {
                let w4 = whh[wrow + i4]
                let i = i4 * 4u
                acc += w4.x * hsh[i] + w4.y * hsh[i + 1u] + w4.z * hsh[i + 2u] + w4.w * hsh[i + 3u]
                i4++
            }
            gsh[lid] = acc
            barrier()
            if (g == 0u) {
                let ti = ka.reverse != 0u ? t_first - step : t_first + step
                let gi = xg[ti * ka.xgs + j] + gsh[j]
                let gf = xg[ti * ka.xgs + hd + j] + gsh[hd + j]
                let gg = xg[ti * ka.xgs + 2u * hd + j] + gsh[2u * hd + j]
                let go = xg[ti * ka.xgs + 3u * hd + j] + gsh[3u * hd + j]
                let si = sigmoid_f32(gi)
                let sf = sigmoid_f32(gf)
                let so = sigmoid_f32(go)
                cst = sf * cst + si * tts_tanh(gg)
                let hn = so * tts_tanh(cst)
                hsh[j] = hn
                y[ti * ka.ystride + ka.yoff + j] = hn
            }
            step++
        }
    }
}

//! q, k, v and o as f32 rows [t][d], head h at column h * hs, hs at most 128.
struct St2AttnArgs {
    t : uint
    d : uint
    hs : uint
    heads : uint
}

[metal_dispatch(name = "enc_st2_attn", pso = "g_pso_st2_attn", tg = 64, grid = "total/2", params = "total : int64")]
class MetalSt2Attn {
    @ssbo @binding = 0 @role = "read" q : array<float>
    @ssbo @binding = 1 @role = "read" k : array<float>
    @ssbo @binding = 2 @role = "read" v : array<float>
    @ssbo @binding = 3 @role = "write" o : array<float>
    @uniform @binding = 4 ka : St2AttnArgs

    [metal_kernel(name="metal_st2_attn_msl")]
    def metal_st2_attn {
        let gid = gl_GlobalInvocationID.x
        let row = gid / 32u
        if (row >= ka.heads * ka.t) {   //! whole simdgroups: the rows align to them
            return
        }
        let lane = gid % 32u
        let h = row / ka.t
        let i = row % ka.t
        let qrow = i * ka.d + h * ka.hs
        var acc : float[128]
        var e0 = 0u
        while (e0 < ka.hs) {
            acc[e0] = 0.0
            e0++
        }
        var m = -1.0e30
        var l = 0.0
        var j = lane
        while (j < ka.t) {
            let krow = j * ka.d + h * ka.hs
            var sdot = 0.0
            var e = 0u
            while (e < ka.hs) {
                sdot += q[qrow + e] * k[krow + e]
                e++
            }
            let sc = sdot / sqrt(float(ka.hs))
            let mn = max(m, sc)
            let f = exp(m - mn)
            let p = exp(sc - mn)
            l = l * f + p
            e = 0u
            while (e < ka.hs) {
                acc[e] = acc[e] * f + p * v[krow + e]
                e++
            }
            m = mn
            j += 32u
        }
        let mall = simd_max(m)
        let f2 = exp(m - mall)
        let lall = simd_sum(l * f2)
        var e1 = 0u
        while (e1 < ka.hs) {
            let s = simd_sum(acc[e1] * f2)
            if (lane == 0u) {
                o[qrow + e1] = s / lall
            }
            e1++
        }
    }
}

//! The CPU gelu's tanh form at full precision: the LUT form's two f16 roundings would move the front end's durations.
[metal_dispatch(name = "enc_st2_gelu_tanh", pso = "g_pso_st2_gelu_tanh", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2GeluTanh : MetalEw1BaseT {
    def override map(a : float) : float => tts_gelu_tanh(a)

    [metal_kernel(name="metal_st2_gelu_tanh_msl")]
    def metal_st2_gelu_tanh {
        ew1_body()
    }
}

//! the duration sigmoid sums - the CPU `styletts2_durations`' raw durations
[metal_dispatch(name = "enc_st2_sigsum", pso = "g_pso_st2_sigsum", tg = 64, grid = "t/64", params = "t : int64")]
class MetalSt2SigSum : GkSigSum {
    [metal_kernel(name="metal_st2_sigsum_msl")]
    def metal_st2_sigsum {
        sigsum_body()
    }
}

[metal_dispatch(name = "enc_st2_src_low_torch", pso = "g_pso_st2_src_low_torch", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2SrcLowTorch : GkSrcLow {
    override TORCH = true

    [metal_kernel(name="metal_st2_src_low_torch_msl", fastmath=false)]
    def metal_st2_src_low_torch {
        src_low_body()
    }
}

[metal_dispatch(name = "enc_st2_src_low_onnx", pso = "g_pso_st2_src_low_onnx", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2SrcLowOnnx : GkSrcLow {
    [metal_kernel(name="metal_st2_src_low_onnx_msl", fastmath=false)]
    def metal_st2_src_low_onnx {
        src_low_body()
    }
}

[metal_dispatch(name = "enc_st2_src_cumsum_torch", pso = "g_pso_st2_src_cumsum_torch", tg = "nh", grid = "1", params = "nh : int64")]
class MetalSt2SrcCumsumTorch : GkSrcCumsum {
    override TORCH = true

    [metal_kernel(name="metal_st2_src_cumsum_torch_msl", fastmath=false)]   //! fast math folds the compensation away
    def metal_st2_src_cumsum_torch {
        src_cumsum_body()
    }
}

[metal_dispatch(name = "enc_st2_src_cumsum_onnx", pso = "g_pso_st2_src_cumsum_onnx", tg = "nh", grid = "1", params = "nh : int64")]
class MetalSt2SrcCumsumOnnx : GkSrcCumsum {
    [metal_kernel(name="metal_st2_src_cumsum_onnx_msl", fastmath=false)]
    def metal_st2_src_cumsum_onnx {
        src_cumsum_body()
    }
}

//! The source's own noise draw - the captured stream's seat when no stream was captured.
[metal_dispatch(name = "enc_st2_src_noise", pso = "g_pso_st2_src_noise", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2SrcNoise : GkSrcNoise {
    [metal_kernel(name="metal_st2_src_noise_msl")]
    def metal_st2_src_noise {
        src_noise_body()
    }
}

[metal_dispatch(name = "enc_st2_src_sines_torch", pso = "g_pso_st2_src_sines_torch", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2SrcSinesTorch : GkSrcSines {
    override TORCH = true

    [metal_kernel(name="metal_st2_src_sines_torch_msl", fastmath=false)]
    def metal_st2_src_sines_torch {
        src_sines_body()
    }
}

[metal_dispatch(name = "enc_st2_src_sines_onnx", pso = "g_pso_st2_src_sines_onnx", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2SrcSinesOnnx : GkSrcSines {
    [metal_kernel(name="metal_st2_src_sines_onnx_msl", fastmath=false)]
    def metal_st2_src_sines_onnx {
        src_sines_body()
    }
}

//! spec rows [frames][2 bins], the magnitudes then the phases; a zero imaginary part on the negative real axis reads +pi.
[metal_dispatch(name = "enc_st2_stft_reflect", pso = "g_pso_st2_stft_reflect", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2StftReflect : GkStft {
    override REFLECT = true

    [metal_kernel(name="metal_st2_stft_reflect_msl")]
    def metal_st2_stft_reflect {
        stft_body()
    }
}

[metal_dispatch(name = "enc_st2_stft_edge", pso = "g_pso_st2_stft_edge", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSt2StftEdge : GkStft {
    [metal_kernel(name="metal_st2_stft_edge_msl")]
    def metal_st2_stft_edge {
        stft_body()
    }
}

//! From conv_post's rows y [tt][stride] - a bin's log magnitude, then its phase; a non-negligible envelope divided out.
//! The inverse STFT from conv_post's rows.
[metal_dispatch(name = "enc_st2_istft", pso = "g_pso_st2_istft", tg = 64, grid = "n/64", params = "n : int64")]
class MetalSt2Istft : GkIstft {
    [metal_kernel(name="metal_st2_istft_msl")]
    def metal_st2_istft {
        istft_body()
    }
}

[metal_dispatch(name = "enc_pk_rows", pso = "g_pso_pk_rows", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkRows : GkPkRows {
    [metal_kernel(name="metal_pk_rows_msl")]
    def metal_pk_rows {
        pk_rows_body()
    }
}

[metal_dispatch(name = "enc_pk_rows_elu", pso = "g_pso_pk_rows_elu", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkRowsElu : GkPkRows {
    override ELU = true

    [metal_kernel(name="metal_pk_rows_elu_msl")]
    def metal_pk_rows_elu {
        pk_rows_body()
    }
}

//! x [t][c] *= s[c]: a transformer layer's per-channel layer scale, the rows template's SCALE stamp (`total` = the elements over four)
[metal_dispatch(name = "enc_pk_row_scale", pso = "g_pso_pk_row_scale", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkRowScale : GkBiasAct {
    override SCALE = true

    [metal_kernel(name = "metal_pk_row_scale_msl")]
    def metal_pk_row_scale {
        bias_act_body()
    }
}

//! the Pocket prompt's adjacent-pair rope over a span of rows from the voice slot's tables at an element base
//! (the position the rows start at): the Vulkan chain's rope, stamped here for the prompt stage
[metal_dispatch(name = "enc_pk_rope_tab", pso = "g_pso_pk_rope_tab", tg = 64, grid = "pairs/64", params = "pairs : int64")]
class MetalPkRopeTab : GkRopeTab {
    [metal_kernel(name="metal_pk_rope_tab_msl")]
    def metal_pk_rope_tab {
        rope_tab_body()
    }
}

//! the towers' rms rows on the layernorm template's RMS arm (ADD off): xb = rms(x) . w, a workgroup a row, xb may alias x
[metal_dispatch(name = "enc_tw_rms", pso = "g_pso_tw_rms", tgmem = "metal_tw_rms_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalTwRms : GkPostAddLn {
    override ADD = false
    override RMS = true

    [metal_kernel(name = "metal_tw_rms_msl")]
    def metal_tw_rms {
        post_add_ln_body()
    }
}

//! the towers' biased residual seam with the next layernorm in one pass: x += y + bias, xb = layernorm(x) under ln_on, a workgroup a row
[metal_dispatch(name = "enc_tw_add_ln", pso = "g_pso_tw_add_ln", tgmem = "metal_tw_add_ln_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalTwAddLn : GkPostAddLn {
    [metal_kernel(name = "metal_tw_add_ln_msl")]
    def metal_tw_add_ln {
        post_add_ln_body()
    }
}

//! the same seam with the next rms (the qwen25v blocks)
[metal_dispatch(name = "enc_tw_add_rms", pso = "g_pso_tw_add_rms", tgmem = "metal_tw_add_rms_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalTwAddRms : GkPostAddLn {
    override RMS = true

    [metal_kernel(name = "metal_tw_add_rms_msl")]
    def metal_tw_add_rms {
        post_add_ln_body()
    }
}

//! the post-normed seam of the gemma towers: x += rms(y) . w_post, then the next rms
[metal_dispatch(name = "enc_tw_add_post_rms", pso = "g_pso_tw_add_post_rms", tgmem = "metal_tw_add_post_rms_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalTwAddPostRms : GkPostAddLn {
    override RMS = true
    override POST_NORM = true

    [metal_kernel(name = "metal_tw_add_post_rms_msl")]
    def metal_tw_add_post_rms {
        post_add_ln_body()
    }
}

//! the vision stems' patch im2col on the template both homes stamp
[metal_dispatch(name = "enc_tw_patch_im2col", pso = "g_pso_tw_patch_im2col", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwPatchIm2col : GkPatchIm2col {
    [metal_kernel(name = "metal_tw_patch_im2col_msl")]
    def metal_tw_patch_im2col {
        patch_im2col_body()
    }
}

//! the vision towers' full-head NEOX rope over compact q or k rows from per-position tables, one table row a position shared by every head
[metal_dispatch(name = "enc_tw_rope_tab", pso = "g_pso_tw_rope_tab", tg = 64, grid = "pairs/64", params = "pairs : int64")]
class MetalTwRopeTab : GkRopeTab {
    override NEOX = true

    [metal_kernel(name = "metal_tw_rope_tab_msl")]
    def metal_tw_rope_tab {
        rope_tab_body()
    }
}

//! a gated hidden with its two bias rows in place over the gate rows (`total` = the elements over four)
[metal_dispatch(name = "enc_tw_bias_gate", pso = "g_pso_tw_bias_gate", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwBiasGate : GkBiasGate {
    [metal_kernel(name = "metal_tw_bias_gate_msl")]
    def metal_tw_bias_gate {
        bias_gate_body()
    }
}

let PK_ATTN_HS = int(GK_PK_ATTN_HS)   //!< the widest head the attention row takes; a narrower head shares a column's value sum across 256 / hs parts
let PK_ATTN_MAX_KEYS = int64(GK_PK_ATTN_MAX_KEYS)   //!< the scores the attention row stages in threadgroup memory - the body's literal

//! the subgroup primitives the shared reductions ride, Metal's spelling
def gk_subgroup_add(v : float) : float => simd_sum(v)
def gk_subgroup_max(v : float) : float => simd_max(v)
def gk_subgroup_xor(v : float; m : uint) : float => simd_shuffle_xor(v, m)
def gk_subgroup_xor_u(v : uint; m : uint) : uint => simd_shuffle_xor(v, m)
def gk_subgroup_bcast0_u(v : uint) : uint => simd_shuffle(v, 0u)
def gk_subgroup_min_u(v : uint) : uint {
    var m = v
    var b = 16u
    while (b >= 1u) {
        m = min(m, simd_shuffle_xor(m, b))
        b /= 2u
    }
    return m
}

//! The Pocket chains' causal cached attention row; a threadgroup a (head, row) over staged scores.
[metal_dispatch(name = "enc_pk_attn", pso = "g_pso_pk_attn", tgmem = "metal_pk_attn_msl_tgmem", tg = 256, grid = "total", params = "total : int64", requires = "pa.hs % 4")]
class MetalPkAttn : GkPkAttn {
    [metal_kernel(name="metal_pk_attn_msl")]
    def metal_pk_attn {
        pk_attn_body()
    }
}

//! Rows [nrows x dim]: x += a in place, then y = LayerNorm(x) * w + b at `yoff` - a layer's residual join
//! and the norm after it as one dispatch (a is read out before y is written, so y may alias it): the
//! ADD stamp of the LayerNorm body.
[metal_dispatch(name = "enc_pk_add_ln", pso = "g_pso_pk_add_ln", tgmem = "metal_pk_add_ln_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class MetalPkAddLn : GkPostAddLn {
    override BIAS = false

    [metal_kernel(name="metal_pk_add_ln_msl")]
    def metal_pk_add_ln {
        post_add_ln_body()
    }
}

//! The row GEMV's epilogues: the bare dot, the SiLU, the gated residual, the SiLU over the slab vector, the frame's tail.
let PK_GEMV_MAX_NIN = 4096l   //!< the x the row GEMV stages in threadgroup memory - the body's literal
let PK_GEMV_DOT = 0
let PK_GEMV_LN_SILU = 1
let PK_GEMV_GATE = 2
let PK_GEMV_ADD_SILU = 3
let PK_GEMV_TAIL = 4

//! The Pocket frame's row GEMV on the template both homes stamp (`GkPkGemv`): the f32 rows of the slab, the inverse norm `rsqrt`
[ |> template_struct_instance]
class template MetalPkGemvT : GkPkGemv {
    override RSQRT = true
    override QUAD = false

    def override row_dot_into(row, lane : uint; var acc : float&) {
        f32_row_dot_into(row, lane, acc)
    }
}

//! ... over a q8 region of the blob: the decode GEMV's lane map, a lane four quants of a block, the 32 lanes eight blocks a step
//! derives GkPkGemv, the template both homes stamp
[ |> template_struct_instance]
class template MetalPkGemvQ8T : GkPkGemv {
    @ssbo @binding = 9 @role = "weight" @off = "qoff" wsh : array<float16>
    @ssbo @binding = 10 @role = "alias" @off = "qoff" wqb : array<int8>
    override RSQRT = true
    override QUAD = false

    def override row_dot_into(row, lane : uint; var acc : float&) {
        let nb4 = pa.nin / 4u
        let wrow = row * (pa.nin / 32u)
        var k = lane
        while (k < nb4) {
            let bi = wrow + k / 8u
            let d = float(wsh[bi * 17u])
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let xv = float4(xs[k * 4u], xs[k * 4u + 1u], xs[k * 4u + 2u], xs[k * 4u + 3u])
            acc += q8_quad_dot(wqb, qb, xv) * d
            k += 32u
        }
    }
}

[metal_dispatch(name = "enc_pk_gemv_f32", pso = "g_pso_pk_gemv_f32", tgmem = "metal_pk_gemv_f32_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64")]
class MetalPkGemvF32 : MetalPkGemvT {
    [metal_kernel(name="metal_pk_gemv_f32_msl")]
    def metal_pk_gemv_f32 {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pk_gemv_f32_ln_silu", pso = "g_pso_pk_gemv_f32_ln_silu", tgmem = "metal_pk_gemv_f32_ln_silu_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64")]
class MetalPkGemvF32LnSilu : MetalPkGemvT {
    override MOD_IN = true
    override NORM_MOD_IN = true
    override EPI = 1

    [metal_kernel(name="metal_pk_gemv_f32_ln_silu_msl")]
    def metal_pk_gemv_f32_ln_silu {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pk_gemv_f32_gate", pso = "g_pso_pk_gemv_f32_gate", tgmem = "metal_pk_gemv_f32_gate_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64")]
class MetalPkGemvF32Gate : MetalPkGemvT {
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name="metal_pk_gemv_f32_gate_msl")]
    def metal_pk_gemv_f32_gate {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pk_gemv_f32_add_silu", pso = "g_pso_pk_gemv_f32_add_silu", tgmem = "metal_pk_gemv_f32_add_silu_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64")]
class MetalPkGemvF32AddSilu : MetalPkGemvT {
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name="metal_pk_gemv_f32_add_silu_msl")]
    def metal_pk_gemv_f32_add_silu {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pk_gemv_f32_tail", pso = "g_pso_pk_gemv_f32_tail", tgmem = "metal_pk_gemv_f32_tail_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64")]
class MetalPkGemvF32Tail : MetalPkGemvT {
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override NORM_MOD_IN = true
    override EPI = 4

    [metal_kernel(name="metal_pk_gemv_f32_tail_msl")]
    def metal_pk_gemv_f32_tail {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pk_gemv_q8_ln_silu", pso = "g_pso_pk_gemv_q8_ln_silu", tgmem = "metal_pk_gemv_q8_ln_silu_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64", requires = "pa.nin % 32")]
class MetalPkGemvQ8LnSilu : MetalPkGemvQ8T {
    override MOD_IN = true
    override NORM_MOD_IN = true
    override EPI = 1

    [metal_kernel(name="metal_pk_gemv_q8_ln_silu_msl")]
    def metal_pk_gemv_q8_ln_silu {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pk_gemv_q8_gate", pso = "g_pso_pk_gemv_q8_gate", tgmem = "metal_pk_gemv_q8_gate_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64", requires = "pa.nin % 32")]
class MetalPkGemvQ8Gate : MetalPkGemvQ8T {
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name="metal_pk_gemv_q8_gate_msl")]
    def metal_pk_gemv_q8_gate {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pk_gemv_q8_add_silu", pso = "g_pso_pk_gemv_q8_add_silu", tgmem = "metal_pk_gemv_q8_add_silu_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64", requires = "pa.nin % 32")]
class MetalPkGemvQ8AddSilu : MetalPkGemvQ8T {
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name="metal_pk_gemv_q8_add_silu_msl")]
    def metal_pk_gemv_q8_add_silu {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pk_gemv_q8_tail", pso = "g_pso_pk_gemv_q8_tail", tgmem = "metal_pk_gemv_q8_tail_msl_tgmem", tg = 128, grid = "rows/4", params = "rows : int64", requires = "pa.nin % 32")]
class MetalPkGemvQ8Tail : MetalPkGemvQ8T {
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override NORM_MOD_IN = true
    override EPI = 4

    [metal_kernel(name="metal_pk_gemv_q8_tail_msl")]
    def metal_pk_gemv_q8_tail {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

// ===== split-K knee races (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k): the scaffold both files' races time through, and
// the races of the base tiles this file declares =====

let KNEE_CHAIN = 12           //! dispatches a timed encoder issues back to back
let KNEE_RING_BYTES = 96l << 20l
let KNEE_ROWS = 32l           //! the window a knee rung times: one 32-row tile

//! a race site's planes: the quant plane, the scale plane (the q8 blob binds as both) and the scale plane's d tail
typedef KneePlanes = tuple<bq : MetalBuffer?; bs : MetalBuffer?; doff : uint64>

//! a race arm's pipeline from one kernel stem, through `race_stems`
[unused_argument(tgmem)]
def knee_pso(dev : MetalDevice?; src, entry : string; fastmath : bool; tgmem : uint64; var err : string&) : MetalComputePipeline? =>
    pso_from_source(dev, src, entry, fastmath, err)

//! one arm's chain on a concurrent encoder, as the window encodes a site pair: `step(i)` encodes two independent sites and the
//! barriers their own dependencies need; GPU ms
def knee_chain_ms(queue : MetalCommandQueue?; step : block<(enc : MetalComputeEncoder?; i : int) : void>) : double {
    var err = ""
    var gpu = 0.0lf
    var enc_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu, enc_ms, true) $(enc) {
        for (i in range(KNEE_CHAIN)) {
            invoke(step, enc, i)
            metal_memory_barrier(enc)
        }
    }
    return ok ? gpu : -1.0lf
}

//! base and twin chains interleaved, the GPU burned first, best of nine rounds each, in GPU ms. The chains run on concurrent
//! encoders because the window overlaps a layer's independent sites: a serial race prices a grid production never runs alone
//! (`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity`)
def knee_pair_ms(queue : MetalCommandQueue?; base_blk : block<(enc : MetalComputeEncoder?; i : int) : void>;
                 twin_blk : block<(enc : MetalComputeEncoder?; i : int) : void>) : tuple<b : double; t : double> {
    var burn = 0.0lf   // clock: control - ramps the governor before the first timed round
    var rounds = 0
    while (burn < 150.0lf && rounds < 512) {
        let bb = knee_chain_ms(queue, base_blk)
        let tt = knee_chain_ms(queue, twin_blk)
        return (b = -1.0lf, t = -1.0lf) if (bb < 0.0lf || tt < 0.0lf)
        burn += bb + tt
        rounds++
    }
    var tb = -1.0lf
    var tt = -1.0lf
    for (_r in range(9)) {
        let bb = knee_chain_ms(queue, base_blk)
        let t1 = knee_chain_ms(queue, twin_blk)
        tb = tb < 0.0lf ? bb : min(tb, bb)
        tt = tt < 0.0lf ? t1 : min(tt, t1)
    }
    return (b = tb, t = tt)
}

//! copies of an `n` x `k` site's planes in `fmt` past the SLC: the q8 blob (bound as both planes), else a format's quant and
//! scale planes (`race_kq_planes`)
def knee_ring(dev : MetalDevice?; fmt : KqFmt; n, k : int64) : array<KneePlanes> {
    let copies = int(max(2l, KNEE_RING_BYTES / race_weight_bytes(fmt, n, k) + 1l))
    if (fmt == KqFmt.q8) {
        var blob <- race_q8_blob(int(n * k / kq_elems(KqFmt.q8)))
        var ring <- [for (_c in range(copies)); knee_blob_planes(dev, blob)]
        delete blob
        return <- ring
    }
    return <- [for (_c in range(copies)); race_kq_planes(dev, int(n * k / KQ_SB_ELEMS), race_qu(fmt), race_scale_form(fmt))]
}

//! one blob copy bound as both race planes
def knee_blob_planes(dev : MetalDevice?; blob : array<int8>) : KneePlanes {
    let b = metal_buffer_from(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
    return (bq = b, bs = b, doff = 0ul)
}

def knee_ring_release(var ring : array<KneePlanes>) {
    for (pl in ring) {
        metal_release(pl.bq)
        if (pl.bs != pl.bq) {
            metal_release(pl.bs)
        }
    }
    ring |> clear()
}

//! a Metal knee rung's grid: the 32-row tile's threadgroups over a site `n` wide, 64 columns a threadgroup
def metal_knee_tiles(n : int64) : int64 => KNEE_ROWS / 32l * (n / 64l)

typedef KqMmPso = function<(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; s0 : MetalBuffer?; s0off : uint64; s : MetalBuffer?;
                            soff : uint64; q : MetalBuffer?; qoff : uint64; x, y : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows : int64) : void>
typedef KqMmSkPso = function<(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; s0 : MetalBuffer?; s0off : uint64; s : MetalBuffer?;
                              soff : uint64; q : MetalBuffer?; qoff : uint64; x, y : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows, nsk : int64) : void>
typedef B32MmPso = function<(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; s : MetalBuffer?; soff : uint64; q : MetalBuffer?; qoff : uint64;
                             x, y : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows : int64) : void>
typedef B32MmSkPso = function<(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; s : MetalBuffer?; soff : uint64; q : MetalBuffer?; qoff : uint64;
                               x, y : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; mp, rows, nsk : int64) : void>
typedef KneeTile = block<(enc : MetalComputeEncoder?; pl : KneePlanes; x, y, bk, bn : MetalBuffer?; n : int64) : void>
typedef KneeSplit = block<(enc : MetalComputeEncoder?; pl : KneePlanes; x, part, bk, bn : MetalBuffer?; n, nsk : int64) : void>

//! One split race's rungs (`knee_rungs`), a 32-row tile of `widths` x `kdim`: each step two independent sites issued
//! concurrently - their whole tiles (`tile`) against their split stamps (`split`), the barrier and the two reduces - weights
//! from `ring_of`'s copies past the SLC, outputs alternating; the activation panel half or f32 as the stamps read it
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def knee_split_rungs(dev : MetalDevice?; queue : MetalCommandQueue?; widths : int64[5]; kdim : int64; shape : SplitKnee; xhalf : bool;
                     ring_of : block<(n : int64) : array<KneePlanes>>; tile : KneeTile; split : KneeSplit) : array<KneeRung> {
    var rungs : array<KneeRung>
    var err = ""
    var p_red = race_stems(knee_pso, dev, metal_sk_reduce_msl, err)
    if (p_red == null) {
        to_log(LOG_INFO, "dasLLAMA metal: split-K knee race skipped - {err}\n")
        return <- rungs
    }
    let xn = int(KNEE_ROWS * kdim)
    var xh <- race_x_f16(xhalf ? xn : 1)
    var xf <- race_x_f32(xhalf ? 1 : xn)
    var bx = (xhalf ? metal_buffer_from(dev, uint64(xn * 2), unsafe(addr<void?>(xh[0])))
        : metal_buffer_from(dev, uint64(xn * 4), unsafe(addr<void?>(xf[0]))))
    let ybytes = uint64(KNEE_ROWS * widths[4] * 4l)
    var ys <- [for (_i in range(4)); metal_buffer_from(dev, ybytes, null)]
    var parts <- [for (_i in range(2)); metal_buffer_from(dev, ybytes * 8ul, null)]
    var bk = race_uniform_u32(dev, uint(kdim))
    var cur = -1l
    var ring : array<KneePlanes>
    var bn : MetalBuffer?
    rungs <- knee_rungs(widths, kdim, shape, $(n : int64) => metal_knee_tiles(n)) $(n, nsk : int64) : tuple<b : double; t : double> {
        if (n != cur) {
            knee_ring_release(ring)
            metal_release(bn)
            var fresh <- invoke(ring_of, n)
            ring |> push_from(fresh)
            fresh |> clear()
            bn = race_uniform_u32(dev, uint(n))
            cur = n
        }
        var ka = SkReduceArgs(total = uint(KNEE_ROWS * n / 4l), ksplit = uint(nsk))
        return knee_pair_ms(queue, $(enc; i) {
            for (j in range(2)) {
                invoke(tile, enc, ring[(2 * i + j) % length(ring)], bx, ys[(i & 1) * 2 + j], bk, bn, n)
            }
        }, $(enc; i) {
            for (j in range(2)) {
                invoke(split, enc, ring[(2 * i + j) % length(ring)], bx, parts[j], bk, bn, n, nsk)
            }
            metal_memory_barrier(enc)
            for (j in range(2)) {
                enc_sk_reduce_c_pso(enc, p_red, parts[j], ys[(i & 1) * 2 + j], 0ul, ka, KNEE_ROWS * n / 4l)
            }
        })
    }
    knee_ring_release(ring)
    for (b in [bx, bk, bn]) {
        metal_release(b)
    }
    for (b in ys) {
        metal_release(b)
    }
    for (b in parts) {
        metal_release(b)
    }
    release_handle(p_red)
    delete xh
    delete xf
    return <- rungs
}

//! the q8 base tile's knee (`metal_split_k_tgs_base`): `MetalQ8MulMm` against `MetalQ8MulMmSk` on the q8 rung table
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def metal_knee_race_q8_base(dev : MetalDevice?; queue : MetalCommandQueue?; shape : SplitKnee) : int64 {
    var err = ""
    var p_base = race_stems(knee_pso, dev, MetalQ8MulMm_metal_q8_mulmm_msl, err)
    var p_sk = race_stems(knee_pso, dev, MetalQ8MulMmSk_metal_q8_mulmm_msl, err)
    var tgs = metal_split_k_tgs_base()
    if (p_base != null && p_sk != null) {
        var rungs <- knee_split_rungs(dev, queue, KNEE_Q8_WIDTHS, KNEE_Q8_K, shape, false, $(n : int64) => knee_ring(dev, KqFmt.q8, n, KNEE_Q8_K), $(enc : MetalComputeEncoder?; pl : KneePlanes; x, y, bk, bn : MetalBuffer?; n : int64) {
            enc_gemm_mm_c_pso(enc, p_base, pl.bq, 0ul, pl.bq, x, y, 0ul, bk, bn, KNEE_ROWS, n)
        }, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, part, bk, bn : MetalBuffer?; n, nsk : int64) {
            enc_gemm_mm_sk_c_pso(enc, p_sk, pl.bq, 0ul, pl.bq, x, part, 0ul, bk, bn, KNEE_ROWS, n, nsk)
        })
        if (!empty(rungs)) {
            tgs = knee_pick(rungs, shape, "METAL_KNEE base q8")
        }
        delete rungs
    } else {
        to_log(LOG_INFO, "dasLLAMA metal: q8 base split-K knee race skipped - {err}\n")
    }
    release_handles(p_base, p_sk)
    return tgs
}

//! the K-quant and per-32 base tiles' knee (`metal_split_k_tgs_kq_base`): one arm a scale layout the base stamps decode - k4
//! (compact), k6 (split d), iq4xs (a grid format) and q51 (per-32) - each its base tile against its split stamp on the K-quant
//! rung table, every arm's rungs priced together
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def metal_knee_race_kq_base(dev : MetalDevice?; queue : MetalCommandQueue?; shape : SplitKnee) : int64 {
    var err = ""
    var all : array<KneeRung>
    var k4 = race_stems(knee_pso, dev, MetalKqMulMmK4_metal_kq_mulmm_msl, err)
    var k4s = race_stems(knee_pso, dev, MetalKqMulMmK4Sk_metal_kq_mulmm_msl, err)
    knee_base_arm(dev, queue, shape, KqFmt.k4, k4, k4s, @@enc_kq_mm_k4_c_pso, @@enc_kq_mm_k4_sk_c_pso, all)
    var k6 = race_stems(knee_pso, dev, MetalKqMulMmK6_metal_kq_mulmm_msl, err)
    var k6s = race_stems(knee_pso, dev, MetalKqMulMmK6Sk_metal_kq_mulmm_msl, err)
    knee_base_arm(dev, queue, shape, KqFmt.k6, k6, k6s, @@enc_kq_mm_k6_c_pso, @@enc_kq_mm_k6_sk_c_pso, all)
    var iq = race_stems(knee_pso, dev, MetalKqMulMmIq4xs_metal_kq_mulmm_msl, err)
    var iqs = race_stems(knee_pso, dev, MetalKqMulMmIq4xsSk_metal_kq_mulmm_msl, err)
    knee_base_arm(dev, queue, shape, KqFmt.iq4xs, iq, iqs, @@enc_kq_mm_iq4xs_c_pso, @@enc_kq_mm_iq4xs_sk_c_pso, all)
    var q51 = race_stems(knee_pso, dev, MetalKqMulMmQ51_metal_kq_mulmm_msl, err)
    var q51s = race_stems(knee_pso, dev, MetalKqMulMmQ51Sk_metal_kq_mulmm_msl, err)
    knee_b32_arm(dev, queue, shape, KqFmt.q51, q51, q51s, @@enc_kq_mm_q51_c_pso, @@enc_kq_mm_q51_sk_c_pso, all)
    let tgs = empty(all) ? metal_split_k_tgs_kq_base() : knee_pick(all, shape, "METAL_KNEE base kq")
    if (!empty(err)) {
        to_log(LOG_INFO, "dasLLAMA metal: a K-quant base split-K arm skipped - {err}\n")
    }
    delete all
    return tgs
}

//! one superblock format's base arm of the K-quant base race: its rungs appended to `all`, its pipelines released
def private knee_base_arm(dev : MetalDevice?; queue : MetalCommandQueue?; shape : SplitKnee; fmt : KqFmt; var p_base, p_sk : MetalComputePipeline?;
                          mm : KqMmPso; mm_sk : KqMmSkPso; var all : array<KneeRung>) {
    if (p_base != null && p_sk != null) {
        var rungs <- knee_split_rungs(dev, queue, KNEE_KQ_WIDTHS, KNEE_KQ_K, shape, false, $(n : int64) => knee_ring(dev, fmt, n, KNEE_KQ_K), $(enc : MetalComputeEncoder?; pl : KneePlanes; x, y, bk, bn : MetalBuffer?; n : int64) {
            invoke(mm, enc, p_base, pl.bs, pl.doff, pl.bs, 0ul, pl.bq, 0ul, x, y, 0ul, bk, bn, KNEE_ROWS, n)
        }, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, part, bk, bn : MetalBuffer?; n, nsk : int64) {
            invoke(mm_sk, enc, p_sk, pl.bs, pl.doff, pl.bs, 0ul, pl.bq, 0ul, x, part, 0ul, bk, bn, KNEE_ROWS, n, nsk)
        })
        all |> push_from(rungs)
        delete rungs
    }
    release_handles(p_base, p_sk)
}

//! the per-32 arm of the K-quant base race: its planes bind without a d tail
def private knee_b32_arm(dev : MetalDevice?; queue : MetalCommandQueue?; shape : SplitKnee; fmt : KqFmt; var p_base, p_sk : MetalComputePipeline?;
                         mm : B32MmPso; mm_sk : B32MmSkPso; var all : array<KneeRung>) {
    if (p_base != null && p_sk != null) {
        var rungs <- knee_split_rungs(dev, queue, KNEE_KQ_WIDTHS, KNEE_KQ_K, shape, false, $(n : int64) => knee_ring(dev, fmt, n, KNEE_KQ_K), $(enc : MetalComputeEncoder?; pl : KneePlanes; x, y, bk, bn : MetalBuffer?; n : int64) {
            invoke(mm, enc, p_base, pl.bs, 0ul, pl.bq, 0ul, x, y, 0ul, bk, bn, KNEE_ROWS, n)
        }, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, part, bk, bn : MetalBuffer?; n, nsk : int64) {
            invoke(mm_sk, enc, p_sk, pl.bs, 0ul, pl.bq, 0ul, x, part, 0ul, bk, bn, KNEE_ROWS, n, nsk)
        })
        all |> push_from(rungs)
        delete rungs
    }
    release_handles(p_base, p_sk)
}
