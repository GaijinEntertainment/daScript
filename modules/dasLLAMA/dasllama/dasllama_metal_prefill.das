options gen2

module dasllama_metal_prefill shared public

require math
require strings
require daslib/fio
require metal/msl_shader
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_math_metal
require dasllama/dasllama_lcpp_mulmm

// Phase-6 dasMetal: the full-GPU-resident prefill kernel set — everything between the Q8 GEMMs
// that Phase 5c left on the CPU: activation requant, rmsnorm, table-driven NORM RoPE, fused
// causal-softmax attention scores, the P·V gather, fused SwiGLU, and the residual add. Each
// kernel mirrors its dasLLAMA CPU twin's math exactly (same formula, same rounding ops):
//   metal_q8_quant      ~ quantize_q8_0_into_ptr   (amax/127 scale, round-half-away quants)
//   metal_rmsnorm       ~ rmsnorm                  (w[i] * x[i] * rsqrt(mean(x^2) + eps))
//   metal_rope          ~ rope_scaled_tab          (NORM adjacent pairs; cos/sin table rows)
//   metal_attn_qk/_softmax/_av ~ the classic per-head causal scores + softmax + P·V, as a
//                         tiled sgmat GEMM trio over np32-padded per-head score slabs
//   metal_swiglu        ~ swiglu                   (g = g/(1+exp(-g)) * u)
//   metal_add           ~ add_inplace
// Reductions ride simd_sum / a max butterfly over simd_shuffle_xor with hardcoded masks 16..1 —
// Apple-GPU simdgroups are width 32 (asserted at driver init). Kernels with barriers/simd ops are
// NOT sequentially CPU-replayable; their oracles are the dasLLAMA CPU functions themselves
// (tests/dasLLAMA/test_metal_prefill_kernels.das).

// One thread per 32-element block of one activation row: amax -> d = amax/127 -> q = round(x*id).
// Grid covers ntok*(n/32) blocks (uniform nblk guards the ceil-padded dispatch); pad rows of the
// resident xq/xs image are never written here — the GEMM's C-block row independence keeps stale
// pad rows out of every live output row.
class MetalQ8Quant {
    @ssbo @binding = 0 x : array<float>     // [ntok x n] activations
    @ssbo @binding = 1 xq : array<int8>     // [mp x n] quants (live rows only)
    @ssbo @binding = 2 xs : array<float>    // [mp x n/32] scales (live rows only)
    @uniform @binding = 3 n : uint          // row width, % 32
    @uniform @binding = 4 nblk : uint       // ntok * n/32 — grid guard

    [metal_kernel(name="metal_q8_quant_msl")]
    def metal_q8_quant {
        let gid = gl_GlobalInvocationID.x
        if (gid >= nblk) {
            return
        }
        let nb = n / 32u
        let row = gid / nb
        let cb = gid % nb
        let base = row * n + cb * 32u
        var amax = 0.0
        var k = 0u
        while (k < 32u) {
            amax = max(amax, abs(x[base + k]))
            k++
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        xs[row * nb + cb] = d
        k = 0u
        while (k < 32u) {
            xq[base + k] = int8(round(x[base + k] * id))
            k++
        }
    }
}

// One threadgroup per row: cooperative sum of squares (simd_sum + per-simdgroup partials),
// lane 0 folds partials into rsqrt(mean + eps), every thread scales its strided slice.
class MetalRmsNorm {
    @ssbo @binding = 0 x : array<float>     // [nrows x dim] source
    @ssbo @binding = 1 w : array<float>     // [dim] weight
    @ssbo @binding = 2 y : array<float>     // [nrows x dim] out (x aliasing is safe: x is fully
                                            // read before the barrier that precedes any write)
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 eps : float
    @workgroup partial : float[8]           // one per simdgroup (256 threads / width 32)
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

// One thread per rotation pair, NORM layout (adjacent elements 2j, 2j+1 within each head):
// v0*c - v1*s / v0*s + v1*c with this position's precomputed table row — rope_scaled_tab exactly.
// One kernel serves q (n = qd) and k (n = kv_dim) as two dispatches.
class MetalRope {
    @ssbo @binding = 0 v : array<float>     // [ntok x n], roped in place
    @ssbo @binding = 1 tcos : array<float>  // [ntok x head_size/2]
    @ssbo @binding = 2 tsin : array<float>
    @uniform @binding = 3 n : uint          // row width (qd or kv_dim), % head_size
    @uniform @binding = 4 head_size : uint
    @uniform @binding = 5 npairs : uint     // ntok * n/2 — grid guard

    [metal_kernel(name="metal_rope_msl")]
    def metal_rope {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let pairs_per_row = n / 2u
        let p = gid / pairs_per_row
        let rem = gid % pairs_per_row
        let half = head_size / 2u
        let h = rem / half
        let j = rem % half
        let i = p * n + h * head_size + 2u * j
        let fcr = tcos[p * half + j]
        let fci = tsin[p * half + j]
        let v0 = v[i]
        let v1 = v[i + 1u]
        v[i] = v0 * fcr - v1 * fci
        v[i + 1u] = v0 * fci + v1 * fcr
    }
}

// The dispatch-fusion set (the 21 -> 15 dispatches/layer collapse — ew cost on M1 is launch
// gaps, not bandwidth):
//   metal_add_rms_quant  ~ add_inplace? + rmsnorm + quantize_q8_0 in ONE pass — the summed row
//                          is staged in threadgroup memory (device writeback for the residual
//                          stream happens alongside), so the normalized values never round-trip
//                          device memory and the standalone add dispatch disappears. One
//                          threadgroup per row; dim <= 3072 (the tg stage) gates the fused path.
//   metal_swiglu_quant   ~ swiglu + quantize_q8_0 per 32-block, silu recomputed on the quant
//                          pass instead of buffered — the gated activation is dead after the
//                          quant, so it is never written back at all.
//   metal_rope_qk        ~ both rope dispatches in one grid (q pairs first, then k).
class MetalAddRmsQuant {
    @ssbo @binding = 0 x : array<float>     // [nrows x dim] residual stream, updated in place
    @ssbo @binding = 1 r : array<float>     // [nrows x dim] delta to fold in (addv != 0)
    @ssbo @binding = 2 w : array<float>     // [dim] rms weight
    @ssbo @binding = 3 xq : array<int8>     // [mp x dim] quants out
    @ssbo @binding = 4 xs : array<float>    // [mp x dim/32] scales out
    @uniform @binding = 5 dim : uint
    @uniform @binding = 6 eps : float
    @uniform @binding = 7 addv : uint       // 0 = plain rms+quant (layer 0 / no pending add)
    @workgroup xrow : float[3072]           // the summed row (pre-norm) — llama-1B/3B dims fit
    @workgroup partial : float[8]
    @workgroup inv_sh : float

    [metal_kernel(name="metal_add_rms_quant_msl")]
    def metal_add_rms_quant {
        let row = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        var ss = 0.0
        var k = lid
        while (k < dim) {
            var xv = x[row * dim + k]
            if (addv != 0u) {
                xv += r[row * dim + k]
                x[row * dim + k] = xv          // the residual stream keeps the sum
            }
            xrow[k] = xv
            ss += xv * xv
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
        let nb = dim / 32u
        var cb = lid
        while (cb < nb) {
            let base = cb * 32u
            var amax = 0.0
            var e = 0u
            while (e < 32u) {
                amax = max(amax, abs(w[base + e] * (xrow[base + e] * inv)))
                e++
            }
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            xs[row * nb + cb] = d
            e = 0u
            while (e < 32u) {
                xq[row * dim + base + e] = int8(round(w[base + e] * (xrow[base + e] * inv) * id))
                e++
            }
            cb += gl_WorkGroupSize.x
        }
    }
}

class MetalSwigluQuant {
    @ssbo @binding = 0 g : array<float>     // [ntok x hidden] W1 gate (read-only — dead after)
    @ssbo @binding = 1 u : array<float>     // [ntok x hidden] W3 up
    @ssbo @binding = 2 xq : array<int8>     // [mp x hidden] quants out
    @ssbo @binding = 3 xs : array<float>    // [mp x hidden/32] scales out
    @uniform @binding = 4 n : uint          // hidden, % 32
    @uniform @binding = 5 nblk : uint       // ntok * hidden/32 — grid guard

    [metal_kernel(name="metal_swiglu_quant_msl")]
    def metal_swiglu_quant {
        let gid = gl_GlobalInvocationID.x
        if (gid >= nblk) {
            return
        }
        let nb = n / 32u
        let row = gid / nb
        let cb = gid % nb
        let base = row * n + cb * 32u
        var amax = 0.0
        var k = 0u
        while (k < 32u) {
            let gv = g[base + k]
            let sv = (gv / (1.0 + exp(-gv))) * u[base + k]
            amax = max(amax, abs(sv))
            k++
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        xs[row * nb + cb] = d
        k = 0u
        while (k < 32u) {
            let gv = g[base + k]
            let sv = (gv / (1.0 + exp(-gv))) * u[base + k]
            xq[base + k] = int8(round(sv * id))
            k++
        }
    }
}

class MetalRopeQK {
    @ssbo @binding = 0 q : array<float>     // [ntok x qd], roped in place
    @ssbo @binding = 1 k : array<float>     // [ntok x kv_dim], roped in place
    @ssbo @binding = 2 tcos : array<float>  // [ntok x head_size/2]
    @ssbo @binding = 3 tsin : array<float>
    @uniform @binding = 4 qd : uint
    @uniform @binding = 5 kvd : uint
    @uniform @binding = 6 head_size : uint
    @uniform @binding = 7 npairs_q : uint   // ntok * qd/2 — the q half of the grid
    @uniform @binding = 8 npairs : uint     // + ntok * kv_dim/2 — grid guard

    [metal_kernel(name="metal_rope_qk_msl")]
    def metal_rope_qk {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let isK = gid >= npairs_q
        let n = isK ? kvd : qd
        let lgid = isK ? gid - npairs_q : gid
        let pairs_per_row = n / 2u
        let p = lgid / pairs_per_row
        let rem = lgid % pairs_per_row
        let half = head_size / 2u
        let h = rem / half
        let j = rem % half
        let i = p * n + h * head_size + 2u * j
        let fcr = tcos[p * half + j]
        let fci = tsin[p * half + j]
        if (isK) {
            let v0 = k[i]
            let v1 = k[i + 1u]
            k[i] = v0 * fcr - v1 * fci
            k[i + 1u] = v0 * fci + v1 * fcr
        } else {
            let v0 = q[i]
            let v1 = q[i + 1u]
            q[i] = v0 * fcr - v1 * fci
            q[i + 1u] = v0 * fci + v1 * fcr
        }
    }
}

// Attention runs as a tiled-GEMM trio over a per-head score slab PADDED to 32-row blocks
// (np32 = ceil32(npos) — sgmat stores are whole 8x8 tiles, so the slab must absorb edge tiles):
//   metal_attn_qk       S = scale * Q_h . K_hT   (32x32 C blocks, mixed... all-f32 sgmat macs)
//   metal_attn_softmax  causal in-row softmax: P[0..qi] normalized, P(qi..np32) ZEROED
//   metal_attn_av       O = P_h . V_h            (the zeroed tail makes full-block macs exact)
// The naive fused per-row pair this replaced ran at ~80 GMAC/s and owned 44% of the prefill's
// GPU time (knockout-attributed); the tiled trio is the same math at GEMM rates.

// One threadgroup (128 threads = 4 simdgroups) per 32x32 score block (grid: qi-blocks x j-blocks
// x heads; blocks fully above the causal diagonal exit early — softmax zeroes them). Q staging
// folds `scale` in; K stages transposed with rows >= npos zeroed (V-GEMM pads can carry NaN and
// 0*NaN would poison the tile). head_size walks in 64-wide K-SLABS through the statically-sized
// 32x64 staging tiles (accumulators persist across slabs), so any hs % 8 fits in the same
// threadgroup footprint — hs 64 (llama-1B) is one slab, 128 (llama-3B) two.
class MetalAttnQK {
    @ssbo @binding = 0 q : array<float>     // [mp x qd], roped
    @ssbo @binding = 1 k : array<float>     // [mp x kv_dim], roped
    @ssbo @binding = 2 att : array<float>   // [n_heads x np32 x np32] raw scores out
    @uniform @binding = 3 qd : uint
    @uniform @binding = 4 kv_dim : uint
    @uniform @binding = 5 head_size : uint
    @uniform @binding = 6 kv_mul : uint
    @uniform @binding = 7 npos : uint
    @uniform @binding = 8 np32 : uint
    @uniform @binding = 9 scale : float
    @workgroup ta : float[2048]             // Q block: 32 qi-rows x 64 (row stride 64)
    @workgroup tb : float[2048]             // KT block: 64 k-rows x 32 j-cols (row stride 32)

    [metal_kernel(name="metal_attn_qk_msl")]
    def metal_attn_qk {
        let qi0 = gl_WorkGroupID.x * 32u
        let j0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        if (j0 > qi0 + 31u) {
            return                          // fully above the causal diagonal
        }
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let kvhoff = (h / kv_mul) * head_size
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var ks = 0u
        while (ks < head_size) {
            let kslab = min(head_size - ks, 64u)
            var e = lid
            while (e < 2048u) {
                let r = e / 64u
                let c = e % 64u
                ta[e] = c < kslab ? q[(qi0 + r) * qd + h * head_size + ks + c] * scale : 0.0
                let jr = e % 32u
                let kc = e / 32u
                tb[e] = (kc < kslab && j0 + jr < npos) ? k[(j0 + jr) * kv_dim + kvhoff + ks + kc] : 0.0
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < kslab) {
                var a0 : simdgroup_float8x8
                var a1 : simdgroup_float8x8
                var b0 : simdgroup_float8x8
                var b1 : simdgroup_float8x8
                simdgroup_load(a0, ta, qm * 64u + k8, 64u)
                simdgroup_load(a1, ta, (qm + 8u) * 64u + k8, 64u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            ks += 64u
        }
        let row0 = (h * np32 + qi0 + qm) * np32 + (j0 + qn)
        simdgroup_store(c00, att, row0, np32)
        simdgroup_store(c01, att, row0 + 8u, np32)
        simdgroup_store(c10, att, row0 + 8u * np32, np32)
        simdgroup_store(c11, att, row0 + 8u * np32 + 8u, np32)
    }
}

// One threadgroup (128 threads = 4 simdgroups) per (query row < npos, head): causal in-row
// softmax over the raw scores — max via a width-32 simd_shuffle_xor butterfly + threadgroup
// partials, exp and sum likewise; the tail (qi, np32) is ZEROED so the AV GEMM can run whole
// blocks. start_pos = 0 by driver contract, so cnt = qi + 1.
class MetalAttnSoftmax {
    @ssbo @binding = 0 att : array<float>   // [n_heads x np32 x np32], normalized in place
    @uniform @binding = 1 np32 : uint
    @workgroup red : float[4]               // one per simdgroup
    @workgroup bcast : float

    [metal_kernel(name="metal_attn_softmax_msl")]
    def metal_attn_softmax {
        let qi = gl_WorkGroupID.x
        let h = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let nthreads = gl_WorkGroupSize.x
        let cnt = qi + 1u
        let rowbase = (h * np32 + qi) * np32
        var m = -1.0e30
        var j = lid
        while (j < cnt) {
            m = max(m, att[rowbase + j])
            j += nthreads
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
        if (lid == 0u) {
            var mm = red[0]
            var sgi = 1u
            while (sgi < gl_NumSubgroups) {
                mm = max(mm, red[sgi])
                sgi++
            }
            bcast = mm
        }
        barrier()
        let rowmax = bcast
        var sum = 0.0
        j = lid
        while (j < cnt) {
            let ev = exp(att[rowbase + j] - rowmax)
            att[rowbase + j] = ev
            sum += ev
            j += nthreads
        }
        sum = simd_sum(sum)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = sum
        }
        barrier()
        if (lid == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += red[sgi]
                sgi++
            }
            bcast = 1.0 / tot
        }
        barrier()
        let inv = bcast
        j = lid
        while (j < cnt) {
            att[rowbase + j] *= inv
            j += nthreads
        }
        j = cnt + lid
        while (j < np32) {
            att[rowbase + j] = 0.0          // the AV GEMM runs whole 32-blocks over this tail
            j += nthreads
        }
    }
}

// One threadgroup (128 threads = 4 simdgroups) per 32x32 output block (grid: qi-blocks x
// d-blocks(hs/32) x heads): O = P.V walking j in 32-blocks up to the causal limit qi0+32 (P's
// zeroed tail keeps whole blocks exact). V rows >= npos stage as 0 (GEMM pad rows can carry
// NaN; P's tail zeros there would still produce 0*NaN). hs % 32 == 0 (the d-block grid).
class MetalAttnAV {
    @ssbo @binding = 0 att : array<float>   // [n_heads x np32 x np32] normalized P
    @ssbo @binding = 1 v : array<float>     // [mp x kv_dim] raw V
    @ssbo @binding = 2 xb : array<float>    // [mp x qd] attention output
    @uniform @binding = 3 qd : uint
    @uniform @binding = 4 kv_dim : uint
    @uniform @binding = 5 head_size : uint
    @uniform @binding = 6 kv_mul : uint
    @uniform @binding = 7 npos : uint
    @uniform @binding = 8 np32 : uint
    @workgroup ta : float[1024]             // P block: 32 qi-rows x 32 j (stride 32)
    @workgroup tb : float[1024]             // V block: 32 j-rows x 32 d (stride 32)

    [metal_kernel(name="metal_attn_av_msl")]
    def metal_attn_av {
        let qi0 = gl_WorkGroupID.x * 32u
        let d0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let kvhoff = (h / kv_mul) * head_size
        let jlimit = min(qi0 + 32u, np32)   // causal: P rows here are zero past qi0+31
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var jb = 0u
        while (jb < jlimit) {
            var e = lid
            while (e < 1024u) {
                let r = e / 32u
                let c = e % 32u
                ta[e] = att[(h * np32 + qi0 + r) * np32 + jb + c]
                tb[e] = jb + r < npos ? v[(jb + r) * kv_dim + kvhoff + d0 + c] : 0.0
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_float8x8
                var a1 : simdgroup_float8x8
                var b0 : simdgroup_float8x8
                var b1 : simdgroup_float8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            jb += 32u
        }
        let row0 = (qi0 + qm) * qd + h * head_size + d0 + qn
        simdgroup_store(c00, xb, row0, qd)
        simdgroup_store(c01, xb, row0 + 8u, qd)
        simdgroup_store(c10, xb, row0 + 8u * qd, qd)
        simdgroup_store(c11, xb, row0 + 8u * qd + 8u, qd)
    }
}

// Fused SwiGLU gate, one thread per element: g = silu(g) * u — swiglu exactly (the CPU side's
// exp4 polynomial vs the GPU exp differ inside the shared tolerance envelope).
class MetalSwiglu {
    @ssbo @binding = 0 g : array<float>     // [ntok x hidden] W1 gate, in place
    @ssbo @binding = 1 u : array<float>     // [ntok x hidden] W3 up
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

// Residual add, one thread per element: x += y (add_inplace).
class MetalAdd {
    @ssbo @binding = 0 x : array<float>
    @ssbo @binding = 1 y : array<float>
    @uniform @binding = 2 total : uint      // grid guard

    [metal_kernel(name="metal_add_msl")]
    def metal_add {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] += y[gid]
    }
}

// ===== the whole-prefill driver =====
// Registered as prefill override "metal" (select_prefill_override; env rail DASLLAMA_PIN_PREFILL).
// One command buffer per prefill: 20 dispatches per layer (norm, requant, 7 GEMMs across the
// llama block, rope x2, scores, P.V, swiglu, residual adds), default hazard tracking orders them,
// ONE commit+wait — then the unified-memory readback hands x_b and the per-layer roped-K/raw-V
// back to the CPU (kv_store_batch keeps the session KV codec CPU-side, so any KV dtype works).
// Weights ride the same lazy region cache contract as the Phase-5c GEMM donor: uploaded once,
// keyed by base address, load-once-stable (refilling the same arrays needs metal_prefill_shutdown).

var private g_dev : MetalDevice?
var private g_queue : MetalCommandQueue?
var private g_failed = false
var private g_regions : table<uint64; MetalBuffer?>    // base address -> resident weight/scale/norm region
var private g_blob_regions : table<uint64; MetalBuffer?>    // quant base address -> resident block_q8_0 blob (lcpp mul_mm path)
var private g_repack_us = 0l
var private g_pool : MetalBufferPool
var private g_upool : MetalBufferPool     // UNTRACKED pool: uniforms + rope tables (GPU-read-only)
var private g_min_npos = 64l
var private g_max_npos = 2048l          // naive-attention P footprint cap: heads x npos^2 floats
var private g_prefills = 0l
var private g_declines = 0l
var private g_pso_gemm : MetalComputePipeline?
var private g_pso_gemm64 : MetalComputePipeline?
var private g_pso_quant : MetalComputePipeline?
var private g_pso_rms : MetalComputePipeline?
var private g_pso_rope : MetalComputePipeline?
var private g_pso_qk : MetalComputePipeline?
var private g_pso_softmax : MetalComputePipeline?
var private g_pso_av : MetalComputePipeline?
var private g_pso_swiglu : MetalComputePipeline?
var private g_pso_add : MetalComputePipeline?
var private g_pso_addrmsq : MetalComputePipeline?
var private g_pso_swigluq : MetalComputePipeline?
var private g_pso_ropeqk : MetalComputePipeline?
var private g_pso_mulmm : MetalComputePipeline?
var private g_mm_legacy = false             // set_metal_prefill_mulmm_legacy — tests' exact-parity rail
// timing-attribution knockout (DASLLAMA_METAL_PREFILL_SKIP=attn|gemm|ew|nongemm): skips encoding
// that stage family (nongemm = attn AND ew, leaving the pure GEMM chain) — output is GARBAGE,
// gpu-time deltas attribute the budget. Debug rail only.
var private g_skip = ""

//! Prefills below this npos decline to the CPU path (submit+readback overhead outweighs the win).
//! Set 1 to force every prefill onto the GPU (tests).
def public set_metal_prefill_min_npos(v : int64) {
    g_min_npos = v
}

//! Pin the legacy quantized-activation GEMM path (q8 X + planar weights) instead of the default
//! llama.cpp mul_mm path (f32 X + block_q8_0 blobs). The legacy path is bit-identical to the CPU
//! forward — the tests' exact-parity rail. `DASLLAMA_METAL_MULMM=0` pins it process-wide.
def public set_metal_prefill_mulmm_legacy(v : bool) {
    g_mm_legacy = v
}

//! (gpu prefills served, declines to the CPU path) since load — diagnostics / tests.
def public metal_prefill_stats() : tuple<prefills : int64; declines : int64> {
    return (prefills = g_prefills, declines = g_declines)
}

//! Release every GPU object the driver holds — the tests' leak-gate rail (and the reset for
//! refilled weight arrays, same contract as metal_gemm_shutdown). Lazily re-initializes on the
//! next served prefill.
def public metal_prefill_shutdown {
    for (buf in values(g_regions)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_regions
    }
    for (buf in values(g_blob_regions)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_blob_regions
    }
    g_repack_us = 0l
    pool_drain(g_pool)
    pool_drain(g_upool)
    if (g_pso_gemm != null) {
        metal_release(g_pso_gemm)
        g_pso_gemm = null
    }
    if (g_pso_gemm64 != null) {
        metal_release(g_pso_gemm64)
        g_pso_gemm64 = null
    }
    if (g_pso_quant != null) {
        metal_release(g_pso_quant)
        g_pso_quant = null
    }
    if (g_pso_rms != null) {
        metal_release(g_pso_rms)
        g_pso_rms = null
    }
    if (g_pso_rope != null) {
        metal_release(g_pso_rope)
        g_pso_rope = null
    }
    if (g_pso_qk != null) {
        metal_release(g_pso_qk)
        g_pso_qk = null
    }
    if (g_pso_softmax != null) {
        metal_release(g_pso_softmax)
        g_pso_softmax = null
    }
    if (g_pso_av != null) {
        metal_release(g_pso_av)
        g_pso_av = null
    }
    if (g_pso_swiglu != null) {
        metal_release(g_pso_swiglu)
        g_pso_swiglu = null
    }
    if (g_pso_add != null) {
        metal_release(g_pso_add)
        g_pso_add = null
    }
    if (g_pso_addrmsq != null) {
        metal_release(g_pso_addrmsq)
        g_pso_addrmsq = null
    }
    if (g_pso_swigluq != null) {
        metal_release(g_pso_swigluq)
        g_pso_swigluq = null
    }
    if (g_pso_ropeqk != null) {
        metal_release(g_pso_ropeqk)
        g_pso_ropeqk = null
    }
    if (g_pso_mulmm != null) {
        metal_release(g_pso_mulmm)
        g_pso_mulmm = null
    }
    if (g_queue != null) {
        metal_release(g_queue)
        g_queue = null
    }
    if (g_dev != null) {
        metal_release(g_dev)
        g_dev = null
    }
    g_failed = false
}

def private env_int(name : string; dflt : int) : int {
    return has_env_variable(name) ? to_int(get_env_variable(name)) : dflt
}

def private compile_pso(source, entry : string; fastmath : bool; var ok : bool&) : MetalComputePipeline? {
    var err : string
    var pso = pipeline_from_source(g_dev, source, entry, fastmath, err)
    if (pso == null) {
        to_log(LOG_ERROR, "dasLLAMA metal prefill: `{entry}` compile failed — {err}\n")
        ok = false
    }
    return pso
}

def private metal_prefill_init : bool {
    if (g_pso_add != null) {
        return true
    }
    if (g_failed) {
        return false
    }
    g_dev = metal_create_system_default_device()
    if (g_dev == null) {
        to_log(LOG_WARNING, "dasLLAMA metal prefill: no Metal device — prefill stays on the CPU\n")
        g_failed = true
        return false
    }
    g_queue = metal_new_command_queue(g_dev)
    var ok = true
    g_pso_gemm = compile_pso(metal_q8_gemm_msl, metal_q8_gemm_msl_entry, metal_q8_gemm_msl_fastmath, ok)
    g_pso_gemm64 = compile_pso(metal_q8_gemm64_msl, metal_q8_gemm64_msl_entry, metal_q8_gemm64_msl_fastmath, ok)
    g_pso_quant = compile_pso(metal_q8_quant_msl, metal_q8_quant_msl_entry, metal_q8_quant_msl_fastmath, ok)
    g_pso_rms = compile_pso(metal_rmsnorm_msl, metal_rmsnorm_msl_entry, metal_rmsnorm_msl_fastmath, ok)
    g_pso_rope = compile_pso(metal_rope_msl, metal_rope_msl_entry, metal_rope_msl_fastmath, ok)
    g_pso_qk = compile_pso(metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath, ok)
    g_pso_softmax = compile_pso(metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath, ok)
    g_pso_av = compile_pso(metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath, ok)
    g_pso_swiglu = compile_pso(metal_swiglu_msl, metal_swiglu_msl_entry, metal_swiglu_msl_fastmath, ok)
    g_pso_add = compile_pso(metal_add_msl, metal_add_msl_entry, metal_add_msl_fastmath, ok)
    g_pso_addrmsq = compile_pso(metal_add_rms_quant_msl, metal_add_rms_quant_msl_entry, metal_add_rms_quant_msl_fastmath, ok)
    g_pso_swigluq = compile_pso(metal_swiglu_quant_msl, metal_swiglu_quant_msl_entry, metal_swiglu_quant_msl_fastmath, ok)
    g_pso_ropeqk = compile_pso(metal_rope_qk_msl, metal_rope_qk_msl_entry, metal_rope_qk_msl_fastmath, ok)
    g_pso_mulmm = compile_pso(LCPP_MUL_MM_Q8_MSL, LCPP_MUL_MM_Q8_ENTRY, true, ok)   // ggml compiles fast-math ON
    if (!ok) {
        metal_prefill_shutdown()
        g_failed = true
        return false
    }
    to_log(LOG_INFO, "dasLLAMA metal prefill: resident path live on {metal_device_name(g_dev)}\n")
    return true
}

// lazily upload one stable region (weight quants / scales / norm vectors), keyed by base address
def private upload_region(src : void?; bytes : uint64) : MetalBuffer? {
    let key = intptr(src)
    if (key_exists(g_regions, key)) {
        return g_regions[key]
    }
    // UNTRACKED: weights are GPU-read-only (written once here, before any commit), so they
    // carry no hazards — keeping ~450 of them tracked made the command-buffer scheduler's
    // dependency analysis the dominant non-GPU cost (~70ms/prefill on the 3B)
    var buf = metal_new_buffer_untracked(g_dev, bytes)
    unsafe {
        memcpy(metal_buffer_contents(buf), src, int(bytes))
    }
    g_regions[key] = buf
    return buf
}

// lazily repack one weight region (planar quants + f32 scale plane) into a resident block_q8_0
// blob — llama.cpp's mul_mm layout: 34-byte blocks of half scale + 32 int8 quants. One-time CPU
// pass per region, keyed by the quant base address; f32 -> f16 scale narrowing is exact for
// GGUF-q8_0-sourced weights (the loader widened the file's native f16 scales)
def private upload_blob_region(qsrc : void?; ssrc : void?; rows, k : int64) : MetalBuffer? {
    let key = intptr(qsrc)
    if (key_exists(g_blob_regions, key)) {
        return g_blob_regions[key]
    }
    let ts = ref_time_ticks()
    let nblocks = int(rows * (k / 32l))
    var buf = metal_new_buffer_untracked(g_dev, uint64(nblocks) * 34ul)
    var base = unsafe(reinterpret<int8?>(metal_buffer_contents(buf)))
    var qp = unsafe(reinterpret<int8?>(qsrc))
    var sp = unsafe(reinterpret<float?>(ssrc))
    for (b in range(nblocks)) {
        let off = b * 34
        unsafe {
            var ph = reinterpret<float16?>(base + off)
            ph[0] = float16(sp[b])
            let src = b * 32
            for (c in range(32)) {
                base[off + 2 + c] = qp[src + c]
            }
        }
    }
    g_blob_regions[key] = buf
    g_repack_us += int64(get_time_usec(ts))
    return buf
}

// ggml_metal_kargs_mul_mm image for a plain 2D GEMM (88 bytes; padding after ne12 zeroed):
// dst[m x d] = W[d x k] . X[m x k]^T with W as block_q8_0 blobs and X as raw f32 rows
def private uniform_mm_args(k, d, m : int64) : MetalBuffer? {
    var buf = pool_acquire_untracked(g_upool, g_dev, 88ul)
    let pc = metal_buffer_contents(buf)
    var pi = unsafe(reinterpret<int?>(pc))
    var pu = unsafe(reinterpret<uint64?>(pc))
    var ps = unsafe(reinterpret<int16?>(pc))
    let nb01 = (k / 32l) * 34l
    unsafe {
        pi[0] = int(k)                      // ne00
        pi[1] = 1                           // ne02
        pu[1] = uint64(nb01)                // nb01
        pu[2] = uint64(nb01 * d)            // nb02
        pu[3] = uint64(nb01 * d)            // nb03
        pi[8] = 1                           // ne12
        pi[9] = 0                           // struct padding — keep deterministic
        pu[5] = 4ul                         // nb10
        pu[6] = uint64(k * 4l)              // nb11
        pu[7] = uint64(k * 4l * m)          // nb12
        pu[8] = uint64(k * 4l * m)          // nb13
        pi[18] = int(d)                     // ne0
        pi[19] = int(m)                     // ne1
        ps[40] = int16(1)                   // r2
        ps[41] = int16(1)                   // r3
    }
    return buf
}

def private uniform_u32(v : uint) : MetalBuffer? {
    var buf = pool_acquire_untracked(g_upool, g_dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def private uniform_f32(v : float) : MetalBuffer? {
    var buf = pool_acquire_untracked(g_upool, g_dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

// only the llama-family std shape is wired (Llama-3.2 class — 1B hs=64 and 3B hs=128 both
// serve) — anything else declines to the CPU loop
def private prefill_supported(t : Model; s : Session; npos, start_pos : int64) : bool {
    // the GEMM kernel reads ROW-MAJOR Q8 weights — a repack backend's interleaved layout is
    // unreadable here (the same donor contract as the Phase-5c batch offload: load under
    // DASLLAMA_PIN_BACKEND=arm64-sdot or another row-major backend)
    if ((start_pos != 0l || npos < g_min_npos || npos > g_max_npos) ||
            (t.quant != QuantMode.q8 || t.wscale_f16 || t.arch != "llama") ||
            kernel_backend_needs_repack(active_kernel_backend())) {
        return false
    }
    let c = t.config
    if ((c.rope_neox || c.attn_qkv_bias || c.qk_norm || c.v_norm || c.pre_post_norm) ||
            (c.layer_out_scale || c.attn_sinks || c.attn_logit_softcap > 0.0 || c.sliding_window > 0l) ||
            (c.ffn_act != FfnAct.silu || c.attn_scale > 0.0)) {
        return false
    }
    let head_size = c.head_size
    let qd = c.n_heads * head_size
    let kv_dim = layer_kv_dim(c, 0l)
    let hidden = layer_hidden(t, 0l)
    // head_size % 32: the AV kernel grids d in 32-blocks; the QK kernel k-slabs any size
    if ((qd != c.dim || (head_size % 32l) != 0l) ||
            ((c.dim % 32l) != 0l || (kv_dim % 32l) != 0l || (hidden % 32l) != 0l)) {
        return false
    }
    for (l in range64(c.n_layers)) {
        if ((t.kv_src[l] != l || layer_kv_dim(c, l) != kv_dim || layer_head_size(c, l) != head_size) ||
                (layer_hidden(t, l) != hidden || layer_is_sliding(c, l) || t.wv_offs[l] < 0l)) {
            return false
        }
    }
    // the rope table is the kernel's input — forward_prefill_body builds it right before the hook
    return int64(length(s.rope_cos)) >= npos * (head_size / 2l)
}

def private enc_gemm(enc : MetalComputeEncoder?; bwq, bws, bxq, bxs, by, bkdim, bndim : MetalBuffer?; mp, d : int64) {
    let use64 = gemm_use64(mp, d)
    metal_set_pipeline(enc, use64 ? g_pso_gemm64 : g_pso_gemm)
    metal_set_buffer(enc, bwq, 0ul, 0)
    metal_set_buffer(enc, bws, 0ul, 1)
    metal_set_buffer(enc, bxq, 0ul, 2)
    metal_set_buffer(enc, bxs, 0ul, 3)
    metal_set_buffer(enc, by, 0ul, 4)
    metal_set_buffer(enc, bkdim, 0ul, 5)
    metal_set_buffer(enc, bndim, 0ul, 6)
    let groups = uint(use64 ? (mp / 64l) * (d / 64l) : (mp / 32l) * (d / 32l))
    metal_dispatch_threadgroups(enc, uint3(groups, 1u, 1u), uint3(128u, 1u, 1u))
}

// the llama.cpp mul_mm dispatch: kargs + blob W + raw f32 X + f32 dst, 6144 B dynamic threadgroup
// scratch, grid (mp/32 token tiles, d/64 output tiles) x (32,4,1). Requires d % 64 == 0 and
// mp % 32 == 0 (the pipeline is specialized bc_out=false)
def private enc_gemm_lcpp(enc : MetalComputeEncoder?; bargs, bwblob, bx, by : MetalBuffer?; mp, d : int64) {
    metal_set_pipeline(enc, g_pso_mulmm)
    metal_set_buffer(enc, bargs, 0ul, 0)
    metal_set_buffer(enc, bwblob, 0ul, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_threadgroup_memory_length(enc, 6144ul, 0)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(d / 64l), 1u), uint3(32u, 4u, 1u))
}

def private enc_quant(enc : MetalComputeEncoder?; bx, bxq, bxs, bn, bnblk : MetalBuffer?; nblk : int64) {
    metal_set_pipeline(enc, g_pso_quant)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, bxq, 0ul, 1)
    metal_set_buffer(enc, bxs, 0ul, 2)
    metal_set_buffer(enc, bn, 0ul, 3)
    metal_set_buffer(enc, bnblk, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint((nblk + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

def private enc_rms(enc : MetalComputeEncoder?; bx, bw, by, bdim, beps : MetalBuffer?; nrows : int64) {
    metal_set_pipeline(enc, g_pso_rms)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, bw, 0ul, 1)
    metal_set_buffer(enc, by, 0ul, 2)
    metal_set_buffer(enc, bdim, 0ul, 3)
    metal_set_buffer(enc, beps, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(256u, 1u, 1u))
}

def private enc_rope(enc : MetalComputeEncoder?; bv, bcos, bsin, bn, bhs, bnpairs : MetalBuffer?; npairs : int64) {
    metal_set_pipeline(enc, g_pso_rope)
    metal_set_buffer(enc, bv, 0ul, 0)
    metal_set_buffer(enc, bcos, 0ul, 1)
    metal_set_buffer(enc, bsin, 0ul, 2)
    metal_set_buffer(enc, bn, 0ul, 3)
    metal_set_buffer(enc, bhs, 0ul, 4)
    metal_set_buffer(enc, bnpairs, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// the fused add?+rms+quant: one threadgroup per row, r folded in when baddv reads 1
def private enc_add_rms_quant(enc : MetalComputeEncoder?; bx, br, bw, bxq, bxs, bdim, beps, baddv : MetalBuffer?; nrows : int64) {
    metal_set_pipeline(enc, g_pso_addrmsq)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, br, 0ul, 1)
    metal_set_buffer(enc, bw, 0ul, 2)
    metal_set_buffer(enc, bxq, 0ul, 3)
    metal_set_buffer(enc, bxs, 0ul, 4)
    metal_set_buffer(enc, bdim, 0ul, 5)
    metal_set_buffer(enc, beps, 0ul, 6)
    metal_set_buffer(enc, baddv, 0ul, 7)
    metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(256u, 1u, 1u))
}

def private enc_swiglu_quant(enc : MetalComputeEncoder?; bg, bu, bxq, bxs, bn, bnblk : MetalBuffer?; nblk : int64) {
    metal_set_pipeline(enc, g_pso_swigluq)
    metal_set_buffer(enc, bg, 0ul, 0)
    metal_set_buffer(enc, bu, 0ul, 1)
    metal_set_buffer(enc, bxq, 0ul, 2)
    metal_set_buffer(enc, bxs, 0ul, 3)
    metal_set_buffer(enc, bn, 0ul, 4)
    metal_set_buffer(enc, bnblk, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((nblk + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

def private enc_rope_qk(enc : MetalComputeEncoder?; bq, bk, bcos, bsin, bqd, bkvd, bhs, bnpq, bnpall : MetalBuffer?; npairs : int64) {
    metal_set_pipeline(enc, g_pso_ropeqk)
    metal_set_buffer(enc, bq, 0ul, 0)
    metal_set_buffer(enc, bk, 0ul, 1)
    metal_set_buffer(enc, bcos, 0ul, 2)
    metal_set_buffer(enc, bsin, 0ul, 3)
    metal_set_buffer(enc, bqd, 0ul, 4)
    metal_set_buffer(enc, bkvd, 0ul, 5)
    metal_set_buffer(enc, bhs, 0ul, 6)
    metal_set_buffer(enc, bnpq, 0ul, 7)
    metal_set_buffer(enc, bnpall, 0ul, 8)
    metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

def private enc_qk(enc : MetalComputeEncoder?; bq, bk, batt, bqd, bkvd, bhs, bkvm, bnp, bnp32, bscale : MetalBuffer?; mp, heads : int64) {
    metal_set_pipeline(enc, g_pso_qk)
    metal_set_buffer(enc, bq, 0ul, 0)
    metal_set_buffer(enc, bk, 0ul, 1)
    metal_set_buffer(enc, batt, 0ul, 2)
    metal_set_buffer(enc, bqd, 0ul, 3)
    metal_set_buffer(enc, bkvd, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_set_buffer(enc, bkvm, 0ul, 6)
    metal_set_buffer(enc, bnp, 0ul, 7)
    metal_set_buffer(enc, bnp32, 0ul, 8)
    metal_set_buffer(enc, bscale, 0ul, 9)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(mp / 32l), uint(heads)), uint3(128u, 1u, 1u))
}

def private enc_softmax(enc : MetalComputeEncoder?; batt, bnp32 : MetalBuffer?; npos, heads : int64) {
    metal_set_pipeline(enc, g_pso_softmax)
    metal_set_buffer(enc, batt, 0ul, 0)
    metal_set_buffer(enc, bnp32, 0ul, 1)
    metal_dispatch_threadgroups(enc, uint3(uint(npos), uint(heads), 1u), uint3(128u, 1u, 1u))
}

def private enc_av(enc : MetalComputeEncoder?; batt, bv, bxb, bqd, bkvd, bhs, bkvm, bnp, bnp32 : MetalBuffer?; mp, heads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_av)
    metal_set_buffer(enc, batt, 0ul, 0)
    metal_set_buffer(enc, bv, 0ul, 1)
    metal_set_buffer(enc, bxb, 0ul, 2)
    metal_set_buffer(enc, bqd, 0ul, 3)
    metal_set_buffer(enc, bkvd, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_set_buffer(enc, bkvm, 0ul, 6)
    metal_set_buffer(enc, bnp, 0ul, 7)
    metal_set_buffer(enc, bnp32, 0ul, 8)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(head_size / 32l), uint(heads)), uint3(128u, 1u, 1u))
}

// swiglu and add share the (a, b, total) elementwise shape
def private enc_ew2(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; ba, bb, btot : MetalBuffer?; total : int64) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, ba, 0ul, 0)
    metal_set_buffer(enc, bb, 0ul, 1)
    metal_set_buffer(enc, btot, 0ul, 2)
    metal_dispatch_threadgroups(enc, uint3(uint((total + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

//! The PrefillOverrideFn: runs the whole llama layer stack GPU-resident. true = done (KV cache
//! filled, x_b final); false = declined, the caller runs the CPU loop (x_b untouched, so the
//! fallback recomputes from clean state).
def metal_prefill_forward(t : Model; var s : Session; npos, start_pos : int64) : bool {
    if (!prefill_supported(t, s, npos, start_pos) || !metal_prefill_init()) {
        g_declines++
        return false
    }
    let ts_all = ref_time_ticks()
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = layer_kv_dim(c, 0l)
    let hidden = layer_hidden(t, 0l)
    let kv_mul = n_heads / layer_n_kv_heads(c, 0l)
    let scale = 1.0 / sqrt(float(head_size))
    let half = head_size / 2l
    let mp = ((npos + 63l) / 64l) * 64l    // 64 = the big GEMM kernel's M tile; attention kernels only assume %32
    let maxn = max(qd, hidden)
    // pooled per-prefill buffers (activations sized mp rows — the GEMM has no edge masking; rows
    // past npos are never read by the norm/rope/attention/elementwise kernels, and stale bytes in
    // the requant image only ever land in pad output rows: C-block rows are independent)
    let bytes_x = uint64(mp * dim * 4l)
    let bytes_q = uint64(mp * qd * 4l)
    let bytes_kv = uint64(mp * kv_dim * 4l)
    let bytes_h = uint64(mp * hidden * 4l)
    let bytes_xq = uint64(mp * maxn)
    let bytes_xs = uint64(mp * (maxn / 32l) * 4l)
    let bytes_att = uint64(n_heads * mp * mp * 4l)   // np32-padded per-head score slabs
    let bytes_tab = uint64(npos * half * 4l)
    var bx = pool_acquire(g_pool, g_dev, bytes_x)
    var bxb = pool_acquire(g_pool, g_dev, bytes_x)
    var bxb2 = pool_acquire(g_pool, g_dev, bytes_x)
    var bq = pool_acquire(g_pool, g_dev, bytes_q)
    var bhb = pool_acquire(g_pool, g_dev, bytes_h)
    var bhb2 = pool_acquire(g_pool, g_dev, bytes_h)
    var bxq = pool_acquire(g_pool, g_dev, bytes_xq)
    var bxs = pool_acquire(g_pool, g_dev, bytes_xs)
    var batt = pool_acquire(g_pool, g_dev, bytes_att)
    var bcos = pool_acquire_untracked(g_upool, g_dev, bytes_tab)
    var bsin = pool_acquire_untracked(g_upool, g_dev, bytes_tab)
    var bks : array<MetalBuffer?>
    var bvs : array<MetalBuffer?>
    bks |> reserve(int(c.n_layers))
    bvs |> reserve(int(c.n_layers))
    for (_l in range64(c.n_layers)) {
        bks |> push(pool_acquire(g_pool, g_dev, bytes_kv))
        bvs |> push(pool_acquire(g_pool, g_dev, bytes_kv))
    }
    // upload the embedded rows + this prefill's rope table
    unsafe {
        memcpy(metal_buffer_contents(bx), reinterpret<void?>(addr(s.x_b[0])), int(npos * dim * 4l))
        memcpy(metal_buffer_contents(bcos), reinterpret<void?>(addr(s.rope_cos[0])), int(npos * half * 4l))
        memcpy(metal_buffer_contents(bsin), reinterpret<void?>(addr(s.rope_sin[0])), int(npos * half * 4l))
    }
    // layer-invariant uniforms (weight offsets ride the region cache, so nothing varies per layer)
    var u_dim = uniform_u32(uint(dim))
    var u_qd = uniform_u32(uint(qd))
    var u_kvd = uniform_u32(uint(kv_dim))
    var u_hidden = uniform_u32(uint(hidden))
    var u_hs = uniform_u32(uint(head_size))
    var u_kvm = uniform_u32(uint(kv_mul))
    var u_np = uniform_u32(uint(npos))
    var u_np32 = uniform_u32(uint(mp))
    var u_nblk_dim = uniform_u32(uint(npos * (dim / 32l)))
    var u_nblk_qd = uniform_u32(uint(npos * (qd / 32l)))
    var u_nblk_h = uniform_u32(uint(npos * (hidden / 32l)))
    var u_npairs_q = uniform_u32(uint(npos * qd / 2l))
    var u_npairs_k = uniform_u32(uint(npos * kv_dim / 2l))
    var u_total_dim = uniform_u32(uint(npos * dim))
    var u_total_h = uniform_u32(uint(npos * hidden))
    var u_scale = uniform_f32(scale)
    var u_eps = uniform_f32(c.norm_eps)
    var u_addv0 = uniform_u32(0u)
    var u_addv1 = uniform_u32(1u)
    var u_npairs_all = uniform_u32(uint(npos * (qd + kv_dim) / 2l))
    // llama.cpp mul_mm GEMM path (the default): W as resident block_q8_0 blobs, raw f32
    // activations — the X-quant dispatches disappear and the GEMMs run ~19% faster (lab).
    // Needs every GEMM output dim % 64 (the pipeline is specialized bc_out=false);
    // DASLLAMA_METAL_MULMM=0 (or the setter) pins the legacy quantized path, which is
    // bit-identical to the CPU forward.
    let lcppmm = (!g_mm_legacy && get_env_variable("DASLLAMA_METAL_MULMM") != "0" &&
        (dim % 64l) == 0l && (qd % 64l) == 0l && (kv_dim % 64l) == 0l && (hidden % 64l) == 0l)
    var u_mm_q = uniform_mm_args(dim, qd, mp)
    var u_mm_kv = uniform_mm_args(dim, kv_dim, mp)
    var u_mm_wo = uniform_mm_args(qd, dim, mp)
    var u_mm_w13 = uniform_mm_args(dim, hidden, mp)
    var u_mm_w2 = uniform_mm_args(hidden, dim, mp)
    // the fused add+rms+quant stages one row in threadgroup memory — dim must fit its 3072 cap.
    // DASLLAMA_METAL_FUSE=0 pins the unfused dispatch set (the A/B rail). The lcpp path runs the
    // unfused elementwise set (no quant tails to fuse).
    let fused = !lcppmm && dim <= 3072l && get_env_variable("DASLLAMA_METAL_FUSE") != "0"
    let ts_enc = ref_time_ticks()
    let setup_us = get_time_usec(ts_all)
    var err : string
    var gpu_ms : double
    var encode_ms : double
    // command-buffer split (DASLLAMA_METAL_NCB chunks): each chunk commits as soon as it is
    // encoded, so the scheduler analyzes chunk k while chunk k-1 executes — same-queue commit
    // order plus hazard tracking on the activation buffers keeps cross-chunk ordering. With
    // DASLLAMA_METAL_UNRETAINED=1 the chunks also skip the per-dispatch resource retain/release
    // (safe here: pooled buffers release back only after the wait, weight regions are resident).
    // default = ~4 layers per chunk (3B measured: slack 57ms -> 9ms, +10% pp512; finer splits
    // are neutral). DASLLAMA_METAL_NCB=1 restores the single-buffer path.
    let ncb = clamp(int64(env_int("DASLLAMA_METAL_NCB", int((c.n_layers + 3l) / 4l))), 1l, c.n_layers)
    let unret = get_env_variable("DASLLAMA_METAL_UNRETAINED") == "1"
    let layers_per_cb = (c.n_layers + ncb - 1l) / ncb
    var cbs : array<MetalCommandBuffer?>
    cbs |> reserve(int(ncb))
    var cb : MetalCommandBuffer?
    var enc : MetalComputeEncoder?
    {
        for (l in range64(c.n_layers)) {
            if (l % layers_per_cb == 0l) {
                if (enc != null) {
                    metal_end_encoding(enc)
                    metal_release(enc)
                    metal_commit(cb)
                    cbs |> push(cb)
                }
                cb = unret ? metal_new_command_buffer_unretained(g_queue) : metal_new_command_buffer(g_queue)
                enc = metal_new_compute_encoder(cb)
            }
            unsafe {
                var bw_att = upload_region(reinterpret<void?>(addr(t.fblob[t.rms_att_off + l * dim])), uint64(dim * 4l))
                var bw_ffn = upload_region(reinterpret<void?>(addr(t.fblob[t.rms_ffn_off + l * dim])), uint64(dim * 4l))
                // attention: [pending W2 residual +] norm -> [requant ->] Q/K/V -> rope ->
                // scores+softmax -> P.V -> out-proj (fused path folds each residual add into
                // the FOLLOWING add+rms+quant; the last layer's pending add lands after the loop;
                // the lcpp path feeds the normed f32 rows straight to mul_mm — no requant)
                if (g_skip != "ew" && g_skip != "nongemm") {
                    if (fused) {
                        enc_add_rms_quant(enc, bx, bxb, bw_att, bxq, bxs, u_dim, u_eps, l > 0l ? u_addv1 : u_addv0, npos)
                    } else {
                        enc_rms(enc, bx, bw_att, bxb, u_dim, u_eps, npos)
                        if (!lcppmm) {
                            enc_quant(enc, bxb, bxq, bxs, u_dim, u_nblk_dim, npos * (dim / 32l))
                        }
                    }
                }
                if (g_skip != "gemm") {
                    if (lcppmm) {
                        var bwqb = upload_blob_region(reinterpret<void?>(addr(t.qblob[t.wq_offs[l]])), reinterpret<void?>(addr(t.qscales[t.wq_offs[l] / 32l])), qd, dim)
                        var bwkb = upload_blob_region(reinterpret<void?>(addr(t.qblob[t.wk_offs[l]])), reinterpret<void?>(addr(t.qscales[t.wk_offs[l] / 32l])), kv_dim, dim)
                        var bwvb = upload_blob_region(reinterpret<void?>(addr(t.qblob[t.wv_offs[l]])), reinterpret<void?>(addr(t.qscales[t.wv_offs[l] / 32l])), kv_dim, dim)
                        enc_gemm_lcpp(enc, u_mm_q, bwqb, bxb, bq, mp, qd)
                        enc_gemm_lcpp(enc, u_mm_kv, bwkb, bxb, bks[l], mp, kv_dim)
                        enc_gemm_lcpp(enc, u_mm_kv, bwvb, bxb, bvs[l], mp, kv_dim)
                    } else {
                        var bwq = upload_region(reinterpret<void?>(addr(t.qblob[t.wq_offs[l]])), uint64(qd * dim))
                        var bwq_s = upload_region(reinterpret<void?>(addr(t.qscales[t.wq_offs[l] / 32l])), uint64(qd * (dim / 32l) * 4l))
                        var bwk = upload_region(reinterpret<void?>(addr(t.qblob[t.wk_offs[l]])), uint64(kv_dim * dim))
                        var bwk_s = upload_region(reinterpret<void?>(addr(t.qscales[t.wk_offs[l] / 32l])), uint64(kv_dim * (dim / 32l) * 4l))
                        var bwv = upload_region(reinterpret<void?>(addr(t.qblob[t.wv_offs[l]])), uint64(kv_dim * dim))
                        var bwv_s = upload_region(reinterpret<void?>(addr(t.qscales[t.wv_offs[l] / 32l])), uint64(kv_dim * (dim / 32l) * 4l))
                        enc_gemm(enc, bwq, bwq_s, bxq, bxs, bq, u_dim, u_qd, mp, qd)
                        enc_gemm(enc, bwk, bwk_s, bxq, bxs, bks[l], u_dim, u_kvd, mp, kv_dim)
                        enc_gemm(enc, bwv, bwv_s, bxq, bxs, bvs[l], u_dim, u_kvd, mp, kv_dim)
                    }
                }
                if (g_skip != "ew" && g_skip != "nongemm") {
                    if (fused) {
                        enc_rope_qk(enc, bq, bks[l], bcos, bsin, u_qd, u_kvd, u_hs, u_npairs_q, u_npairs_all, npos * (qd + kv_dim) / 2l)
                    } else {
                        enc_rope(enc, bq, bcos, bsin, u_qd, u_hs, u_npairs_q, npos * qd / 2l)
                        enc_rope(enc, bks[l], bcos, bsin, u_kvd, u_hs, u_npairs_k, npos * kv_dim / 2l)
                    }
                }
                if (g_skip != "attn" && g_skip != "nongemm") {
                    enc_qk(enc, bq, bks[l], batt, u_qd, u_kvd, u_hs, u_kvm, u_np, u_np32, u_scale, mp, n_heads)
                    enc_softmax(enc, batt, u_np32, npos, n_heads)
                    enc_av(enc, batt, bvs[l], bxb, u_qd, u_kvd, u_hs, u_kvm, u_np, u_np32, mp, n_heads, head_size)
                }
                if (!lcppmm && g_skip != "ew" && g_skip != "nongemm") {
                    enc_quant(enc, bxb, bxq, bxs, u_qd, u_nblk_qd, npos * (qd / 32l))
                }
                if (g_skip != "gemm") {
                    if (lcppmm) {
                        var bwob = upload_blob_region(reinterpret<void?>(addr(t.qblob[t.wo_offs[l]])), reinterpret<void?>(addr(t.qscales[t.wo_offs[l] / 32l])), dim, qd)
                        enc_gemm_lcpp(enc, u_mm_wo, bwob, bxb, bxb2, mp, dim)
                    } else {
                        var bwo = upload_region(reinterpret<void?>(addr(t.qblob[t.wo_offs[l]])), uint64(dim * qd))
                        var bwo_s = upload_region(reinterpret<void?>(addr(t.qscales[t.wo_offs[l] / 32l])), uint64(dim * (qd / 32l) * 4l))
                        enc_gemm(enc, bwo, bwo_s, bxq, bxs, bxb2, u_qd, u_dim, mp, dim)
                    }
                }
                // ffn: [WO residual +] norm -> [requant ->] W1/W3 -> swiglu[+requant] -> W2
                if (g_skip != "ew" && g_skip != "nongemm") {
                    if (fused) {
                        enc_add_rms_quant(enc, bx, bxb2, bw_ffn, bxq, bxs, u_dim, u_eps, u_addv1, npos)
                    } else {
                        enc_ew2(enc, g_pso_add, bx, bxb2, u_total_dim, npos * dim)
                        enc_rms(enc, bx, bw_ffn, bxb, u_dim, u_eps, npos)
                        if (!lcppmm) {
                            enc_quant(enc, bxb, bxq, bxs, u_dim, u_nblk_dim, npos * (dim / 32l))
                        }
                    }
                }
                if (g_skip != "gemm") {
                    if (lcppmm) {
                        var bw1b = upload_blob_region(reinterpret<void?>(addr(t.qblob[t.w1_offs[l]])), reinterpret<void?>(addr(t.qscales[t.w1_offs[l] / 32l])), hidden, dim)
                        var bw3b = upload_blob_region(reinterpret<void?>(addr(t.qblob[t.w3_offs[l]])), reinterpret<void?>(addr(t.qscales[t.w3_offs[l] / 32l])), hidden, dim)
                        enc_gemm_lcpp(enc, u_mm_w13, bw1b, bxb, bhb, mp, hidden)
                        enc_gemm_lcpp(enc, u_mm_w13, bw3b, bxb, bhb2, mp, hidden)
                    } else {
                        var bw1 = upload_region(reinterpret<void?>(addr(t.qblob[t.w1_offs[l]])), uint64(hidden * dim))
                        var bw1_s = upload_region(reinterpret<void?>(addr(t.qscales[t.w1_offs[l] / 32l])), uint64(hidden * (dim / 32l) * 4l))
                        var bw3 = upload_region(reinterpret<void?>(addr(t.qblob[t.w3_offs[l]])), uint64(hidden * dim))
                        var bw3_s = upload_region(reinterpret<void?>(addr(t.qscales[t.w3_offs[l] / 32l])), uint64(hidden * (dim / 32l) * 4l))
                        enc_gemm(enc, bw1, bw1_s, bxq, bxs, bhb, u_dim, u_hidden, mp, hidden)
                        enc_gemm(enc, bw3, bw3_s, bxq, bxs, bhb2, u_dim, u_hidden, mp, hidden)
                    }
                }
                if (g_skip != "ew" && g_skip != "nongemm") {
                    if (fused) {
                        enc_swiglu_quant(enc, bhb, bhb2, bxq, bxs, u_hidden, u_nblk_h, npos * (hidden / 32l))
                    } else {
                        enc_ew2(enc, g_pso_swiglu, bhb, bhb2, u_total_h, npos * hidden)
                        if (!lcppmm) {
                            enc_quant(enc, bhb, bxq, bxs, u_hidden, u_nblk_h, npos * (hidden / 32l))
                        }
                    }
                }
                if (g_skip != "gemm") {
                    if (lcppmm) {
                        var bw2b = upload_blob_region(reinterpret<void?>(addr(t.qblob[t.w2_offs[l]])), reinterpret<void?>(addr(t.qscales[t.w2_offs[l] / 32l])), dim, hidden)
                        enc_gemm_lcpp(enc, u_mm_w2, bw2b, bhb, bxb, mp, dim)
                    } else {
                        var bw2 = upload_region(reinterpret<void?>(addr(t.qblob[t.w2_offs[l]])), uint64(dim * hidden))
                        var bw2_s = upload_region(reinterpret<void?>(addr(t.qscales[t.w2_offs[l] / 32l])), uint64(dim * (hidden / 32l) * 4l))
                        enc_gemm(enc, bw2, bw2_s, bxq, bxs, bxb, u_hidden, u_dim, mp, dim)
                    }
                }
                if (!fused && g_skip != "ew" && g_skip != "nongemm") {
                    enc_ew2(enc, g_pso_add, bx, bxb, u_total_dim, npos * dim)
                }
            }
        }
        if (fused && g_skip != "ew" && g_skip != "nongemm") {
            enc_ew2(enc, g_pso_add, bx, bxb, u_total_dim, npos * dim)   // the last layer's pending W2 add
        }
        metal_end_encoding(enc)
        metal_release(enc)
        encode_ms = double(get_time_usec(ts_enc)) / 1000.0lf
        metal_commit(cb)
        cbs |> push(cb)
    }
    // completion + readback INTERLEAVE: chunks complete in commit order, and each completed
    // chunk's roped-K/raw-V rows stream into the CPU KV codec while later chunks keep the GPU
    // busy (unified memory — the memcpy is safe the moment the chunk reports complete). The
    // residual-stream copy needs the LAST chunk and lands after the loop.
    var ran = true
    var gpu_t0 = 0.0lf
    var gpu_t1 = 0.0lf
    var readback_us = 0l
    unsafe {
        let store_nrun = kv_runs(s, start_pos, start_pos + npos, s.kv_runs)
        let runsp = reinterpret<KVRun const?>(addr(s.kv_runs[0]))
        for (cbi, i in cbs, count()) {
            metal_wait_until_completed(cbi)
            let e = metal_command_buffer_error(cbi)
            if (!empty(e)) {
                ran = false
                err = e
            }
            let t0 = metal_command_buffer_gpu_start_time(cbi)
            let t1 = metal_command_buffer_gpu_end_time(cbi)
            if (i == 0 || t0 < gpu_t0) {
                gpu_t0 = t0
            }
            if (i == 0 || t1 > gpu_t1) {
                gpu_t1 = t1
            }
            metal_release(cbi)
            if (ran) {
                let ts_rb = ref_time_ticks()
                let l1 = min(int64(i + 1) * layers_per_cb, c.n_layers)
                for (l in range64(int64(i) * layers_per_cb, l1)) {
                    memcpy(reinterpret<void?>(addr(s.k_b[0])), metal_buffer_contents(bks[l]), int(npos * kv_dim * 4l))
                    memcpy(reinterpret<void?>(addr(s.v_b[0])), metal_buffer_contents(bvs[l]), int(npos * kv_dim * 4l))
                    kv_store_batch(kv_layer_kbase(s, l, c.seq_len), kv_layer_vbase(s, l, c.seq_len),
                        s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b, runsp, store_nrun, start_pos, npos, kv_dim)
                }
                readback_us += int64(get_time_usec(ts_rb))
            }
        }
    }
    cbs |> clear()
    unsafe {
        delete cbs
    }
    gpu_ms = (gpu_t1 - gpu_t0) * 1000.0lf   // whole-GPU window: first chunk start -> last chunk end
    let encwait_us = get_time_usec(ts_enc)
    let ok = ran
    if (ran) {
        let ts_rb = ref_time_ticks()
        unsafe {
            memcpy(reinterpret<void?>(addr(s.x_b[0])), metal_buffer_contents(bx), int(npos * dim * 4l))
        }
        readback_us += int64(get_time_usec(ts_rb))
        g_prefills++
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal prefill: dispatch failed ({err}) — falling back to the CPU layer loop\n")
        g_declines++
    }
    // return the pooled buffers (uniforms too); the weight regions stay resident
    pool_release(g_pool, bx, bytes_x)
    pool_release(g_pool, bxb, bytes_x)
    pool_release(g_pool, bxb2, bytes_x)
    pool_release(g_pool, bq, bytes_q)
    pool_release(g_pool, bhb, bytes_h)
    pool_release(g_pool, bhb2, bytes_h)
    pool_release(g_pool, bxq, bytes_xq)
    pool_release(g_pool, bxs, bytes_xs)
    pool_release(g_pool, batt, bytes_att)
    pool_release(g_upool, bcos, bytes_tab)
    pool_release(g_upool, bsin, bytes_tab)
    for (l in range64(c.n_layers)) {
        pool_release(g_pool, bks[l], bytes_kv)
        pool_release(g_pool, bvs[l], bytes_kv)
    }
    bks |> clear()      // non-owning handles — the pool owns them now
    bvs |> clear()
    unsafe {
        delete bks
        delete bvs
    }
    pool_release(g_upool, u_dim, 4ul)
    pool_release(g_upool, u_qd, 4ul)
    pool_release(g_upool, u_kvd, 4ul)
    pool_release(g_upool, u_hidden, 4ul)
    pool_release(g_upool, u_hs, 4ul)
    pool_release(g_upool, u_kvm, 4ul)
    pool_release(g_upool, u_np, 4ul)
    pool_release(g_upool, u_np32, 4ul)
    pool_release(g_upool, u_nblk_dim, 4ul)
    pool_release(g_upool, u_nblk_qd, 4ul)
    pool_release(g_upool, u_nblk_h, 4ul)
    pool_release(g_upool, u_npairs_q, 4ul)
    pool_release(g_upool, u_npairs_k, 4ul)
    pool_release(g_upool, u_total_dim, 4ul)
    pool_release(g_upool, u_total_h, 4ul)
    pool_release(g_upool, u_scale, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_addv0, 4ul)
    pool_release(g_upool, u_addv1, 4ul)
    pool_release(g_upool, u_npairs_all, 4ul)
    pool_release(g_upool, u_mm_q, 88ul)
    pool_release(g_upool, u_mm_kv, 88ul)
    pool_release(g_upool, u_mm_wo, 88ul)
    pool_release(g_upool, u_mm_w13, 88ul)
    pool_release(g_upool, u_mm_w2, 88ul)
    if (ok) {
        let total_us = get_time_usec(ts_all)
        let repack_note = g_repack_us > 0l ? " weight-repack={float(g_repack_us) / 1000.0}ms(one-time)" : ""
        g_repack_us = 0l
        to_log(LOG_INFO, "dasLLAMA metal prefill: npos={npos} gemm={lcppmm ? "lcpp-mulmm" : "q8"} total={float(total_us) / 1000.0}ms setup={float(setup_us) / 1000.0}ms encode+wait={float(encwait_us) / 1000.0}ms (encode {encode_ms}ms, gpu {gpu_ms}ms) kv+readback={float(readback_us) / 1000.0}ms dispatches={(fused ? 15l : lcppmm ? 17l : 21l) * c.n_layers + (fused ? 1l : 0l)}{repack_note}\n")
    }
    return ok
}

[init]
def dasllama_metal_prefill_register() {
    // registered but dormant: only select_prefill_override("metal") (env rail
    // DASLLAMA_PIN_PREFILL) activates it, and unsupported shapes decline per call
    register_prefill_override("metal", @@metal_prefill_forward)
    if (has_env_variable("DASLLAMA_METAL_PREFILL_SKIP")) {
        g_skip = get_env_variable("DASLLAMA_METAL_PREFILL_SKIP")
        to_log(LOG_WARNING, "dasLLAMA metal prefill: SKIP={g_skip} — timing-attribution mode, output is GARBAGE\n")
    }
}
