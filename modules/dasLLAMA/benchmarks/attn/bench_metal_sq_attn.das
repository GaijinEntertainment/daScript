options gen2
options persistent_heap
options _function_length = 0        // one MSL kernel body + its hand-bound dispatch per lab — barrier/register coupled, no seam
options _cyclomatic_complexity = 0  // same — the side-by-side runners are flat arm ladders

require dastest/testing_boost
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require metal/msl_shader
require ?das_metal metal/das_metal_boost  // GPU driver half, Apple builds only
require dasllama/dasllama_metal_decode  // the production batch attention MSL (zero copy drift)
require dasllama/dasllama_math
require dasllama/dasllama_kv_codec   // the q8_0/tq4 KV codecs (quantize_q8kv_row / cvt_q8kv_to_f32 / FWHT) — the store oracle
require math
require strings

// Single-query decode attention microbench — the emission arc's P1c detector and the one
// genuinely NEW piece of GPU math (prefill's attention kernels are batch-tiled and read f32
// pre-codec K/V; decode reads the f16 KV mirror one query at a time). Shape: one threadgroup
// per (stream, head), 128 threads; pass 1 scores the whole context into a threadgroup slab
// (depth <= 2048 fits 8KB), pass 2 softmax (simd butterfly + cross-simdgroup reduce), pass 3
// per-lane V column accumulate. GQA: head h reads kv head h/kv_mul.
//
// The oracle is a CPU twin function (NOT the method's width-1 replay — barrier reductions
// don't replay sequentially), compared under a small absolute tolerance: exp() and the
// accumulation order differ legitimately between CPU and GPU.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/attn/bench_metal_sq_attn.das

class MetalSqAttnLab {
    @ssbo @binding = 0 kc : array<float16>  // [rows x kv_dim] K, f16 (the mirror contract)
    @ssbo @binding = 1 vc : array<float16>  // [rows x kv_dim] V, f16
    @ssbo @binding = 2 q : array<float>     // [B x qd] roped queries (qd = nheads * head_size)
    @ssbo @binding = 3 yo : array<float>    // [B x qd] attention out
    @uniform @binding = 4 cnt : uint        // context depth (pos + 1)
    @uniform @binding = 5 kv_dim : uint
    @uniform @binding = 6 nheads : uint
    @uniform @binding = 7 kv_mul : uint
    @uniform @binding = 8 scale : float
    @workgroup scores : float[2048]
    @workgroup red : float[4]

    [metal_kernel(name="metal_sq_attn_msl")]
    def metal_sq_attn {
        let head_size = 128u                // == threads per group; lane owns one V column
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let khoff = (h / kv_mul) * head_size
        let qoff = bb * (nheads * head_size) + h * head_size
        // pass 1: strided score fill — lane j dots q[head] against K row j
        var j = lid
        while (j < cnt) {
            var s = 0.0
            var e = 0u
            while (e < head_size) {
                s += q[qoff + e] * float(kc[j * kv_dim + khoff + e])
                e++
            }
            scores[j] = s * scale
            j += 128u
        }
        barrier()
        // pass 2a: max — strided local, simd butterfly, cross-simdgroup
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += 128u
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
        m = max(max(red[0], red[1]), max(red[2], red[3]))
        barrier()                           // red[] reused for the sum below
        // pass 2b: exp in place + sum
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - m)
            scores[j] = w
            l += w
            j += 128u
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
        l = red[0] + red[1] + red[2] + red[3]
        // pass 3: lane owns V column `lid` — weighted sum over the whole context
        var acc = 0.0
        j = 0u
        while (j < cnt) {
            acc += scores[j] * float(vc[j * kv_dim + khoff + lid])
            j++
        }
        yo[qoff + lid] = acc / l
    }
}

// q8_0-cache twin of MetalSqAttnLab (format-matrix W3): K/V arrive as ggml block_q8_0 rows
// (34B per 32 elements — {f16 d; int8 qs[32]}), read through the MetalQ8MulMm dual-view trick
// (one blob buffer bound twice: half view for scales at 17*b, byte view for quants at 34*b+2)
// and dequantized in-registers while streaming. Same grid contract as the f16 form: one
// threadgroup per (stream, head), 128 threads, hs == 128. Score pass: simdgroup sg owns row j,
// lane sl covers 4 elements — all inside ONE block (sl/8), so the block scale factors out of
// the lane dot. V pass: lane owns column lid — element (khoff + lid) sits in block lid/32 of
// the row at in-block offset lid%32. The BW bar vs f16: 34B vs 64B per 32 elements = 0.53x
// cache traffic where the K walk is coalesced.
class MetalSqAttnLabQ8 {
    @ssbo @binding = 0 ksh : array<float16> // K blob, HALF-SCALE view: scale of block b at 17*b
    @ssbo @binding = 1 kqb : array<int8>    // the SAME K blob, BYTE view: quants of block b at 34*b + 2
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<int8>
    @ssbo @binding = 4 q : array<float>     // [B x qd] roped queries
    @ssbo @binding = 5 yo : array<float>    // [B x qd] attention out
    @uniform @binding = 6 cnt : uint        // context depth (pos + 1)
    @uniform @binding = 7 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 scale : float
    @workgroup scores : float[2048]
    @workgroup red : float[4]

    [metal_kernel(name="lab_sq_attn_q8_msl")]
    def lab_sq_attn_q8 {
        let head_size = 128u                // == threads per group; lane owns one V column
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let khblk = (h / kv_mul) * (head_size / 32u)   // kv head offset in BLOCKS
        let qoff = bb * (nheads * head_size) + h * head_size
        // pass 1: simdgroup-cooperative score fill — sg owns row j; lane sl's 4 elements share
        // block sl/8, so d multiplies the lane dot once
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let qb = qoff + sl * 4u
        let q0 = q[qb]
        let q1 = q[qb + 1u]
        let q2 = q[qb + 2u]
        let q3 = q[qb + 3u]
        let inblk = (sl % 8u) * 4u          // this lane's byte offset inside its block's quants
        var j = sg
        while (j < cnt) {
            let blk = j * nblk + khblk + sl / 8u
            let d = float(ksh[blk * 17u])
            let kb = blk * 34u + 2u + inblk
            var s = q0 * float(kqb[kb])
            s += q1 * float(kqb[kb + 1u])
            s += q2 * float(kqb[kb + 2u])
            s += q3 * float(kqb[kb + 3u])
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
            j += 128u
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
        m = max(max(red[0], red[1]), max(red[2], red[3]))
        barrier()                           // red[] reused for the sum below
        // pass 2b: exp in place + sum
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - m)
            scores[j] = w
            l += w
            j += 128u
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
        l = red[0] + red[1] + red[2] + red[3]
        // pass 3: lane owns V column lid — dequant rides the weighted sum (scale + quant land
        // on the same cacheline, so the column walk still touches ~one line per row)
        let vblk0 = khblk + lid / 32u
        let vin = lid % 32u
        var acc = 0.0
        j = 0u
        while (j < cnt) {
            let blk = j * nblk + vblk0
            acc += scores[j] * float(vqb[blk * 34u + 2u + vin]) * float(vsh[blk * 17u])
            j++
        }
        yo[qoff + lid] = acc / l
    }
}

// tq4-cache twin (format-matrix W3): K/V arrive as TurboQuant rotated 4-bit blocks (18B per 32
// elements — {f16 d; uint8 qs[16]}, element 2i in the low nibble of byte i, d = signed-max/-8).
// The rows AND the query are already FWHT-rotated (⟨Hx, Hy⟩ = ⟨x, y⟩, so the whole attention
// runs in the rotated basis; the yo it produces is rotated too — the driver un-rotates once
// per head downstream). Same dual-view read: scale at halfword 9*b, nibbles at byte 18*b + 2.
class MetalSqAttnLabTq4 {
    @ssbo @binding = 0 ksh : array<float16> // K blob, HALF-SCALE view: scale of block b at 9*b
    @ssbo @binding = 1 kqb : array<uint8>   // the SAME K blob, BYTE view: nibbles at 18*b + 2
    @ssbo @binding = 2 vsh : array<float16>
    @ssbo @binding = 3 vqb : array<uint8>
    @ssbo @binding = 4 q : array<float>     // [B x qd] roped + ROTATED queries
    @ssbo @binding = 5 yo : array<float>    // [B x qd] attention out (rotated basis)
    @uniform @binding = 6 cnt : uint
    @uniform @binding = 7 nblk : uint       // blocks per row = kv_dim / 32
    @uniform @binding = 8 nheads : uint
    @uniform @binding = 9 kv_mul : uint
    @uniform @binding = 10 scale : float
    @workgroup scores : float[2048]
    @workgroup red : float[4]

    [metal_kernel(name="lab_sq_attn_tq4_msl")]
    def lab_sq_attn_tq4 {
        let head_size = 128u                // == threads per group; lane owns one V column
        let wg = gl_WorkGroupID.x
        let h = wg % nheads
        let bb = wg / nheads
        let lid = gl_LocalInvocationID.x
        let khblk = (h / kv_mul) * (head_size / 32u)
        let qoff = bb * (nheads * head_size) + h * head_size
        // pass 1: sg owns row j; lane sl's 4 elements = 2 nibble bytes of block sl/8
        let sg = gl_SubgroupID
        let sl = gl_SubgroupInvocationID
        let qb = qoff + sl * 4u
        let q0 = q[qb]
        let q1 = q[qb + 1u]
        let q2 = q[qb + 2u]
        let q3 = q[qb + 3u]
        let inb = (sl % 8u) * 2u            // this lane's byte offset inside its block's nibbles
        var j = sg
        while (j < cnt) {
            let blk = j * nblk + khblk + sl / 8u
            let d = float(ksh[blk * 9u])
            let kb = blk * 18u + 2u + inb
            let by0 = uint(kqb[kb])
            let by1 = uint(kqb[kb + 1u])
            var s = q0 * float(int(by0 & 15u) - 8)
            s += q1 * float(int(by0 >> 4u) - 8)
            s += q2 * float(int(by1 & 15u) - 8)
            s += q3 * float(int(by1 >> 4u) - 8)
            s = simd_sum(s * d)
            if (sl == 0u) {
                scores[j] = s * scale
            }
            j += gl_NumSubgroups
        }
        barrier()
        // pass 2a: max
        var m = -3.0e38
        j = lid
        while (j < cnt) {
            m = max(m, scores[j])
            j += 128u
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
        m = max(max(red[0], red[1]), max(red[2], red[3]))
        barrier()                           // red[] reused for the sum below
        // pass 2b: exp in place + sum
        var l = 0.0
        j = lid
        while (j < cnt) {
            let w = exp(scores[j] - m)
            scores[j] = w
            l += w
            j += 128u
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
        l = red[0] + red[1] + red[2] + red[3]
        // pass 3: lane owns V column lid — nibble parity picks the half-byte
        let vblk0 = khblk + lid / 32u
        let vbyte = (lid % 32u) / 2u
        let hi = (lid % 2u) == 1u
        var acc = 0.0
        j = 0u
        while (j < cnt) {
            let blk = j * nblk + vblk0
            let by = uint(vqb[blk * 18u + 2u + vbyte])
            let nib = hi ? (by >> 4u) : (by & 15u)
            acc += scores[j] * float(int(nib) - 8) * float(vsh[blk * 9u])
            j++
        }
        yo[qoff + lid] = acc / l
    }
}

// Pointer-stream rework candidate for the production part kernel (hs == 128 specialization,
// epl == 4): K reads ONE half4 per lane per position through a walked device pointer (the
// production form issues 4 scalar half loads + the epl guard chain), q hoists as ONE float4,
// the V column walk becomes a pointer stream (base + uniform-scaled offsets, one bump per 4
// positions). Same grid, same tgmem, same part/comb contract.
// NEGATIVE (2026-07-14): the fully-fused lcpp-shape variant — one sg per 32-position slice,
// score/softmax/weights all register-resident (keep-select + butterfly max + simd_shuffle
// broadcast), zero barriers, part entries per-32-slice — reached ISA parity with fa_vec
// (~15 issues/position vs their 14.7) but measured 15-20% SLOWER than THIS two-phase form at
// every depth (d512 28.5 vs 23.5us; d2048 83.2 vs 74.5), both as 32-thread tgs and packed 4
// slices per 128-thread tg. The per-slice prologue (~270 instr re-run per 32 positions vs per
// 64 here) eats the softmax-pass savings. Issue count is not the limiter at this size.
class MetalSqAttnPartLabP {
    @ssbo @binding = 0 kc : array<half4>    // the same f16 mirror buffer, half4 view
    @ssbo @binding = 1 vc : array<half4>    // ... V too — lane owns a half4 COLUMN GROUP
    @ssbo @binding = 2 q : array<float4>    // the same q buffer, float4 view
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
    @workgroup vout : float[512]            // 4 per-sg partial planes x 128 columns

    [metal_kernel(name="lab_sq_attn_part_p_msl")]
    def lab_sq_attn_part_p {
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
        let kv4 = kv_dim / 4u
        let q4 = float4(q[qoff / 4u + sl])
        // K: each sg owns a contiguous 16-position slice, straight-line unrolled; lane sl KEEPS
        // position sl's reduced score via a select (no divergent guard, no per-position store —
        // the lcpp fa_vec ownership shape), then ONE batch store writes the whole slice.
        // NEGATIVE (2026-07-14): [unroll_count=4] on the per-position guarded-store form was
        // neutral — the simd_sum -> lane-0 store chain serialized the back-edge regardless.
        let base16 = sg * 16u
        if (base16 + 16u <= nrows) {
            var kp = unsafe(addr(kc[(kb0 + (c0 + base16) * kv_dim + khoff) / 4u + sl]))
            var keep = 0.0
            for [unroll_full] (i in range(16)) {
                let p = q4 * float4(unsafe(kp[0]))
                let s = simd_sum(p.x + p.y + p.z + p.w)
                keep = uint(i) == sl ? s : keep
                kp = unsafe(kp + kv4)
            }
            if (sl < 16u) {
                scores[base16 + sl] = keep * scale
            }
        } elif (base16 < nrows) {
            // partial tail slice (cnt %64 rows) — the guarded per-position form
            var kp = unsafe(addr(kc[(kb0 + (c0 + base16) * kv_dim + khoff) / 4u + sl]))
            var j = base16
            while (j < nrows) {
                let p = q4 * float4(unsafe(kp[0]))
                let s = simd_sum(p.x + p.y + p.z + p.w)
                if (sl == 0u) {
                    scores[j] = s * scale
                }
                kp = unsafe(kp + kv4)
                j++
            }
        }
        barrier()
        var m = -3.0e38
        var j = lid
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
        // V: same ownership shape as K — each sg walks its contiguous 16-position slice, lane sl
        // owns half4 column group sl (32 x 4 = 128 columns), ONE vector load per position per sg;
        // the 4 per-sg float4 partials combine through the vout slab once per chunk
        var acc4 = float4(0.0)
        if (base16 + 16u <= nrows) {
            var vp = unsafe(addr(vc[(vb0 + (c0 + base16) * kv_dim + khoff) / 4u + sl]))
            for [unroll_full] (i in range(16)) {
                acc4 += scores[base16 + uint(i)] * float4(unsafe(vp[0]))
                vp = unsafe(vp + kv4)
            }
        } elif (base16 < nrows) {
            var vp = unsafe(addr(vc[(vb0 + (c0 + base16) * kv_dim + khoff) / 4u + sl]))
            j = base16
            while (j < nrows) {
                acc4 += scores[j] * float4(unsafe(vp[0]))
                vp = unsafe(vp + kv4)
                j++
            }
        }
        let vo = sg * 128u + sl * 4u
        vout[vo] = acc4.x
        vout[vo + 1u] = acc4.y
        vout[vo + 2u] = acc4.z
        vout[vo + 3u] = acc4.w
        barrier()
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 2u)
        part[pbase + lid] = vout[lid] + vout[128u + lid] + vout[256u + lid] + vout[384u + lid]
        if (lid == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// Fused single-pass form, take 2 (the lcpp fa_vec shape on our rails): one sg per (row, head,
// 32-position slice), 4 slices packed per 128-thread tg (no barriers — divergent per-sg exit is
// safe), score/softmax/weights register-resident. UNLIKE the first fused attempt (recorded
// negative above), K/V loads use INDEPENDENT indexed offsets (kp[i * kv4]) instead of a walked
// pointer — the walk built a 32-deep serial address chain every load depended on; lcpp's
// unrolled body has base+immediate offsets (NS10 is an FC constant), all loads independent.
class MetalSqAttnPartLabF {
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
    @uniform @binding = 12 nchmax : uint

    [metal_kernel(name="lab_sq_attn_part_f_msl")]
    def lab_sq_attn_part_f {
        // 3D grid (head, slice-group, row), HEAD in x: consecutive tgs share the same position
        // rows (positions are kv_dim-contiguous across heads) — SLC locality at depth
        let h = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.z
        let ci = gl_WorkGroupID.y * 4u + gl_SubgroupID
        let rrow = rt[bb]
        let cnt = rrow.w
        let c0 = ci * 32u
        if (c0 >= cnt || ci >= nchmax) {
            return
        }
        let sl = gl_SubgroupInvocationID
        let kb0 = rrow.x + lay * rrow.z * kv_dim
        let vb0 = rrow.y + lay * rrow.z * kv_dim
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let nrows = min(c0 + 32u, cnt) - c0
        let kv4 = kv_dim / 4u
        let q4 = float4(q[qoff / 4u + sl])
        var keep = -3.0e38
        var kp = unsafe(addr(kc[(kb0 + c0 * kv_dim + khoff) / 4u + sl]))
        if (nrows == 32u) {
            for [unroll_full] (i in range(32)) {
                let p = q4 * float4(unsafe(kp[uint(i) * kv4]))
                let s = simd_sum(p.x + p.y + p.z + p.w)
                keep = uint(i) == sl ? s : keep
            }
        } else {
            var j = 0u
            while (j < nrows) {
                let p = q4 * float4(unsafe(kp[0]))
                let s = simd_sum(p.x + p.y + p.z + p.w)
                keep = j == sl ? s : keep
                kp = unsafe(kp + kv4)
                j++
            }
        }
        keep *= scale   // invalid tail lanes stay hugely negative -> weight 0
        var mm = keep
        mm = max(mm, simd_shuffle_xor(mm, 16u))
        mm = max(mm, simd_shuffle_xor(mm, 8u))
        mm = max(mm, simd_shuffle_xor(mm, 4u))
        mm = max(mm, simd_shuffle_xor(mm, 2u))
        mm = max(mm, simd_shuffle_xor(mm, 1u))
        let w = exp(keep - mm)
        let tot = simd_sum(w)
        var acc4 = float4(0.0)
        var vp = unsafe(addr(vc[(vb0 + c0 * kv_dim + khoff) / 4u + sl]))
        if (nrows == 32u) {
            for [unroll_full] (i in range(32)) {
                acc4 += simd_shuffle(w, uint(i)) * float4(unsafe(vp[uint(i) * kv4]))
            }
        } else {
            var j = 0u
            while (j < nrows) {
                acc4 += simd_shuffle(w, j) * float4(unsafe(vp[0]))
                vp = unsafe(vp + kv4)
                j++
            }
        }
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 4u)   // 132: float4-aligned entries
        let po = pbase + sl * 4u
        part[po] = acc4.x
        part[po + 1u] = acc4.y
        part[po + 2u] = acc4.z
        part[po + 3u] = acc4.w
        if (sl == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// 64-position slice twin: TWO inlined 32-batches with one branch-free register merge — the
// prologue amortizes over 64 positions with NO in-sg loop. Entries are 64-wide (comb's
// min((cnt+31)/32, nchmax) clamp counts them right when nchmax = ceil(cnt/64)).
// NEGATIVES recorded here (2026-07-14): (a) full-bake twin (kv_dim/hs/kv_mul hardcoded) —
// neutral, the per-offset imadds are free; (b) slice-group WHILE-loop twin (lcpp nwg
// discipline, running online merge across strided slices) — 15-30% SLOWER at depth than the
// loop-free partF even at 1 slice/sg: the loop-carried acc/m/tot state wrecks scheduling and
// register pressure; amortization must be straight-line. Both removed; see git history.
class MetalSqAttnPartLabF64 {
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
    @uniform @binding = 12 nchmax : uint

    [metal_kernel(name="lab_sq_attn_part_f64_msl")]
    def lab_sq_attn_part_f64 {
        let h = gl_WorkGroupID.x
        let bb = gl_WorkGroupID.z
        let ci = gl_WorkGroupID.y * 4u + gl_SubgroupID
        let rrow = rt[bb]
        let cnt = rrow.w
        let c0 = ci * 64u
        if (ci >= nchmax || c0 >= cnt) {
            return
        }
        let sl = gl_SubgroupInvocationID
        let kb0 = rrow.x + lay * rrow.z * kv_dim
        let vb0 = rrow.y + lay * rrow.z * kv_dim
        let khoff = (h / kv_mul) * hs
        let qoff = bb * qstride + h * hs
        let kv4 = kv_dim / 4u
        let q4 = float4(q[qoff / 4u + sl])
        // batch A: positions [c0, c0+32)
        let nrowsA = min(c0 + 32u, cnt) - c0
        var keepA = -3.0e38
        var kpA = unsafe(addr(kc[(kb0 + c0 * kv_dim + khoff) / 4u + sl]))
        var vpA = unsafe(addr(vc[(vb0 + c0 * kv_dim + khoff) / 4u + sl]))
        var accA = float4(0.0)
        if (nrowsA == 32u) {
            for [unroll_full] (i in range(32)) {
                let p = q4 * float4(unsafe(kpA[uint(i) * kv4]))
                let sc = simd_sum(p.x + p.y + p.z + p.w)
                keepA = uint(i) == sl ? sc : keepA
            }
        } else {
            var j = 0u
            while (j < nrowsA) {
                let p = q4 * float4(unsafe(kpA[0]))
                let sc = simd_sum(p.x + p.y + p.z + p.w)
                keepA = j == sl ? sc : keepA
                kpA = unsafe(kpA + kv4)
                j++
            }
        }
        keepA *= scale
        var mA = keepA
        mA = max(mA, simd_shuffle_xor(mA, 16u))
        mA = max(mA, simd_shuffle_xor(mA, 8u))
        mA = max(mA, simd_shuffle_xor(mA, 4u))
        mA = max(mA, simd_shuffle_xor(mA, 2u))
        mA = max(mA, simd_shuffle_xor(mA, 1u))
        let wA = exp(keepA - mA)
        let totA = simd_sum(wA)
        if (nrowsA == 32u) {
            for [unroll_full] (i in range(32)) {
                accA += simd_shuffle(wA, uint(i)) * float4(unsafe(vpA[uint(i) * kv4]))
            }
        } else {
            var j = 0u
            while (j < nrowsA) {
                accA += simd_shuffle(wA, j) * float4(unsafe(vpA[0]))
                vpA = unsafe(vpA + kv4)
                j++
            }
        }
        var mm = mA
        var tot = totA
        var acc4 = accA
        // batch B: positions [c0+32, c0+64), absent on the shallow tail
        let cB = c0 + 32u
        if (cB < cnt) {
            let nrowsB = min(cB + 32u, cnt) - cB
            var keepB = -3.0e38
            var kpB = unsafe(addr(kc[(kb0 + cB * kv_dim + khoff) / 4u + sl]))
            var vpB = unsafe(addr(vc[(vb0 + cB * kv_dim + khoff) / 4u + sl]))
            var accB = float4(0.0)
            if (nrowsB == 32u) {
                for [unroll_full] (i in range(32)) {
                    let p = q4 * float4(unsafe(kpB[uint(i) * kv4]))
                    let sc = simd_sum(p.x + p.y + p.z + p.w)
                    keepB = uint(i) == sl ? sc : keepB
                }
            } else {
                var j = 0u
                while (j < nrowsB) {
                    let p = q4 * float4(unsafe(kpB[0]))
                    let sc = simd_sum(p.x + p.y + p.z + p.w)
                    keepB = j == sl ? sc : keepB
                    kpB = unsafe(kpB + kv4)
                    j++
                }
            }
            keepB *= scale
            var mB = keepB
            mB = max(mB, simd_shuffle_xor(mB, 16u))
            mB = max(mB, simd_shuffle_xor(mB, 8u))
            mB = max(mB, simd_shuffle_xor(mB, 4u))
            mB = max(mB, simd_shuffle_xor(mB, 2u))
            mB = max(mB, simd_shuffle_xor(mB, 1u))
            let wB = exp(keepB - mB)
            let totB = simd_sum(wB)
            if (nrowsB == 32u) {
                for [unroll_full] (i in range(32)) {
                    accB += simd_shuffle(wB, uint(i)) * float4(unsafe(vpB[uint(i) * kv4]))
                }
            } else {
                var j = 0u
                while (j < nrowsB) {
                    accB += simd_shuffle(wB, j) * float4(unsafe(vpB[0]))
                    vpB = unsafe(vpB + kv4)
                    j++
                }
            }
            let nm = max(mA, mB)
            let r1 = exp(mA - nm)
            let r2 = exp(mB - nm)
            acc4 = accA * r1 + accB * r2
            tot = totA * r1 + totB * r2
            mm = nm
        }
        let pbase = ((bb * nheads + h) * nchmax + ci) * (hs + 4u)   // 132: float4-aligned entries
        let po = pbase + sl * 4u
        part[po] = acc4.x
        part[po + 1u] = acc4.y
        part[po + 2u] = acc4.z
        part[po + 3u] = acc4.w
        if (sl == 0u) {
            part[pbase + hs] = mm
            part[pbase + hs + 1u] = tot
        }
    }
}

// Deep-context twin: the lcpp nsg-loop shape done RIGHT — a capped budget of min(ceil(cnt/32),
// 32) sgs per (row, head), each looping strided 32-slices, but the O accumulator lives in a
// LANE-PRIVATE tgmem slot (their so4 discipline: loop-carried state through memory, registers
// stay light) with scalar m/tot registers and a branch-free rescale-merge per slice. No
// barriers — each lane touches only its own 4 floats. The register-resident loop form was the
// recorded partG negative; tgmem residency is the variable it did not isolate.
class MetalSqAttnPartLabD {
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

    [metal_kernel(name="lab_sq_attn_part_d_msl")]
    def lab_sq_attn_part_d {
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

// Comb twin for the fused-arm part entries (132-float, float4-aligned). PARALLEL over entries
// (the production comb walks them serially — at d2048 that is 64 dependent iterations): the 4
// sgs each online-merge their entry subset with float4 loads (lane owns a 4-column group; m and
// l arrive in ONE float4), then a single tgmem cross-sg merge. Mirrors lcpp reduce parallelism.
class MetalSqAttnCombLabP {
    @ssbo @binding = 0 part : array<float4>
    @ssbo @binding = 1 yo : array<float>
    @ssbo @binding = 2 rt : array<uint4>
    @uniform @binding = 3 nchmax : uint
    @uniform @binding = 4 nheads : uint
    @uniform @binding = 5 hs : uint
    @workgroup wacc : float[512]
    @workgroup wml : float[8]

    [metal_kernel(name="lab_sq_attn_comb_p_msl")]
    def lab_sq_attn_comb_p {
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

// CPU twin — same math, f32 throughout, natural order
def private at_f(a : array<float16>; i : int64) : float => float(a[i])
def private at_f(a : array<float>; i : int64) : float => a[i]

// generic over the K/V element type (f16 mirror or a dequantized-f32 codec oracle)
def private cpu_sq_attn(kc, vc; q : array<float>; nb, cnt, nheads, head_size, kv_dim, kv_mul : int64;
                        scale : float) : array<float> {
    var yo : array<float>
    yo |> resize(int(nb * nheads * head_size))
    var sc : array<float>
    sc |> resize(int(cnt))
    for (bb in range64(nb)) {
        for (h in range64(nheads)) {
            let khoff = (h / kv_mul) * head_size
            let qoff = bb * nheads * head_size + h * head_size
            var m = -3.0e38
            for (j in range64(cnt)) {
                var s = 0.0
                for (e in range64(head_size)) {
                    s += q[qoff + e] * at_f(kc, j * kv_dim + khoff + e)
                }
                sc[j] = s * scale
                m = max(m, sc[j])
            }
            var l = 0.0
            for (j in range64(cnt)) {
                sc[j] = exp(sc[j] - m)
                l += sc[j]
            }
            for (e in range64(head_size)) {
                var acc = 0.0
                for (j in range64(cnt)) {
                    acc += sc[j] * at_f(vc, j * kv_dim + khoff + e)
                }
                yo[qoff + e] = acc / l
            }
        }
    }
    delete sc
    return <- yo
}

def private upload_array(dev; arr; bytes : int64) {
    var buf = metal_new_buffer(dev, uint64(bytes))
    unsafe {
        memcpy(metal_buffer_contents(buf), addr < void? >(arr[0]), bytes)
    }
    return buf
}

def private upload_u32(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def private upload_f32(dev; v : float) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

// a kargs struct uploaded whole — the production kernels bind ONE constant& per slot
def private upload_kargs(dev; ka) {
    let sz = uint64(typeinfo sizeof(ka))
    var buf = metal_new_buffer(dev, sz)
    unsafe {
        memcpy(metal_buffer_contents(buf), addr(ka), sz)
    }
    return buf
}

def private time_reps(queue; pso; bufs; groups : uint3; reps : int; tgmem : uint64) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        for (bi in range(length(bufs))) {
            metal_set_buffer(enc, bufs[bi], 0ul, bi)
        }
        metal_set_threadgroup_memory_length(enc, tgmem, 0)
        for (_i in range(reps)) {
            metal_dispatch_threadgroups(enc, groups, uint3(128u, 1u, 1u))
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms / double(reps)
}

// two-dispatch rep (every arm is a kernel PAIR: part->comb); the shared
// intermediate-buffer hazard serializes the pair and the reps, same contract as time_reps
def private time_pair(queue; pso_a; bufs_a; groups_a : uint3; threads_a : uint; tgmem_a : uint64;
                      pso_b; bufs_b; groups_b : uint3; threads_b : uint; tgmem_b : uint64; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        for (_i in range(reps)) {
            metal_set_pipeline(enc, pso_a)
            for (bi in range(length(bufs_a))) {
                metal_set_buffer(enc, bufs_a[bi], 0ul, bi)
            }
            if (tgmem_a > 0ul) {
                metal_set_threadgroup_memory_length(enc, tgmem_a, 0)
            }
            metal_dispatch_threadgroups(enc, groups_a, uint3(threads_a, 1u, 1u))
            metal_set_pipeline(enc, pso_b)
            for (bi in range(length(bufs_b))) {
                metal_set_buffer(enc, bufs_b[bi], 0ul, bi)
            }
            if (tgmem_b > 0ul) {
                metal_set_threadgroup_memory_length(enc, tgmem_b, 0)
            }
            metal_dispatch_threadgroups(enc, groups_b, uint3(threads_b, 1u, 1u))
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "pair dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms / double(reps)
}

// B=2 side-by-side at the real decode roster slot: the production chunked pair (the
// metal_sq_attn_part_b16 + metal_sq_attn_comb_b MSL, verbatim from the decode driver, rt table
// built flat) vs the lab part/comb variants. Same k/v/q buffers, same CPU oracle,
// interleaved best-of timing.
def private run_attn_b2_sidebyside(b) {
    let rounds = g_env_bench.lab_rounds
    let nheads = 24l
    let head_size = 128l
    let kv_dim = 1024l
    let kv_mul = 3l
    let qd = nheads * head_size
    let nb = 2l
    let maxrows = 16384l
    let scale = 1.0 / sqrt(float(head_size))
    var dev = metal_create_system_default_device()
    if (dev == null) {
        to_log(LOG_WARNING, "skipped: no Metal device\n")
        return
    }
    var queue = metal_new_command_queue(dev)
    var err : string
    var pso_part = pipeline_from_source(dev, MetalSqAttnPartBF16_metal_sq_attn_part_b_msl, MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_entry, true, err)
    b |> success(pso_part != null, "production part kernel compiles")
    var pso_comb = pipeline_from_source(dev, MetalSqAttnCombB_metal_sq_attn_comb_msl, MetalSqAttnCombB_metal_sq_attn_comb_msl_entry, true, err)
    b |> success(pso_comb != null, "production comb kernel compiles")
    var pso_partp = pipeline_from_source(dev, lab_sq_attn_part_p_msl, lab_sq_attn_part_p_msl_entry, true, err)
    b |> success(pso_partp != null, "pointer-stream part kernel compiles")
    if (pso_partp == null) {
        to_log(LOG_ERROR, "pointer-stream part compile failed: {err}\n")
    }
    var pso_partf = pipeline_from_source(dev, lab_sq_attn_part_f_msl, lab_sq_attn_part_f_msl_entry, true, err)
    b |> success(pso_partf != null, "fused part kernel compiles")
    if (pso_partf == null) {
        to_log(LOG_ERROR, "fused part compile failed: {err}\n")
    }
    var pso_partg = pipeline_from_source(dev, lab_sq_attn_part_f64_msl, lab_sq_attn_part_f64_msl_entry, true, err)
    b |> success(pso_partg != null, "64-slice fused part kernel compiles")
    var pso_partd = pipeline_from_source(dev, lab_sq_attn_part_d_msl, lab_sq_attn_part_d_msl_entry, true, err)
    b |> success(pso_partd != null, "deep-context part kernel compiles")
    var pso_combp = pipeline_from_source(dev, lab_sq_attn_comb_p_msl, lab_sq_attn_comb_p_msl_entry, true, err)
    b |> success(pso_combp != null, "32-slice comb twin compiles")
    if (pso_part == null || pso_comb == null || pso_partp == null || pso_partf == null ||
            pso_partg == null || pso_partd == null || pso_combp == null) {
        metal_release(queue)
        metal_release(dev)
        return
    }
    // same exact-in-f16 fills as the single-kernel bench
    var kc : array<float16>
    var vc : array<float16>
    kc |> resize(int(maxrows * kv_dim))
    vc |> resize(int(maxrows * kv_dim))
    for (i in range64(maxrows * kv_dim)) {
        kc[i] = float16(float(int((i * 7l + i / 129l) % 9l) - 4))
        vc[i] = float16(float(int((i * 11l + i / 257l) % 9l) - 4))
    }
    var q : array<float>
    q |> resize(int(nb * qd))
    for (i in range64(nb * qd)) {
        q[i] = float(int((i * 13l + i / 61l) % 9l) - 4)
    }
    var kbuf = upload_array(dev, kc, maxrows * kv_dim * 2l)
    var vbuf = upload_array(dev, vc, maxrows * kv_dim * 2l)
    var qbuf = upload_array(dev, q, nb * qd * 4l)
    var ybuf = metal_new_buffer(dev, uint64(nb * qd * 4l))
    // part: [B x nheads x nchmax x entry] — sized at the deepest depth (32-position entries for the fused arms)
    let nchmax_deep = (maxrows + 31l) / 32l
    var partbuf = metal_new_buffer(dev, uint64(nb * nheads * nchmax_deep * (head_size + 4l) * 4l))
    var laybuf = upload_u32(dev, 0u)
    var kvdimbuf = upload_u32(dev, uint(kv_dim))
    var nheadsbuf = upload_u32(dev, uint(nheads))
    var kvmulbuf = upload_u32(dev, uint(kv_mul))
    var hsbuf = upload_u32(dev, uint(head_size))
    var scalebuf = upload_f32(dev, scale)
    var qstridebuf = upload_u32(dev, uint(qd))
    var ygot : array<float>
    for (depth in [512l, 640l, 1024l, 2048l, 4096l, 16384l]) {
        let nch = (depth + 63l) / 64l
        let nch32 = (depth + 31l) / 32l
        var nchmax32buf = upload_u32(dev, uint(nch32))
        let nch64 = (depth + 63l) / 64l
        var nsgsbuf = upload_u32(dev, uint(nch64))
        // fixed sg budget of 16: beats 32 (16k 1.00 vs 1.05) and the depth-doubling heuristic
        // (64/128 = more entry traffic, shorter loops) — FEWER long-running workers win because
        // the tgmem-resident accumulator loop amortizes; the knob is for re-sweeps
        let nsgsd = min(nch32, int64(g_env_bench.lab_attn_nsgs))
        var nsgsdbuf = upload_u32(dev, uint(nsgsd))
        // rt row: (kbase, vbase, capacity, cnt) — flat single-layer mirror at base 0
        var rte <- [0u, 0u, uint(depth), uint(depth), 0u, 0u, uint(depth), uint(depth)]
        var rtbuf = upload_array(dev, rte, nb * 16l)
        var yref <- cpu_sq_attn(kc, vc, q, nb, depth, nheads, head_size, kv_dim, kv_mul, scale)
        // the production pair binds ONE kargs (llama-shape: no soft cap, no window, no sinks)
        var kapbuf = upload_kargs(dev, SqAttnPartBArgs(lay = 0u, kstride = uint(kv_dim),
            nheads = uint(nheads), kv_mul = uint(kv_mul), hs = uint(head_size), scale = scale,
            qstride = uint(qd), nchmax = uint(nch), softcap = 0.0, window = 0u))
        var kacbuf = upload_kargs(dev, SqAttnCombArgs(nch = uint(nch), nheads = uint(nheads),
            hs = uint(head_size), hass = 0u))
        var bufs_part <- [kbuf, vbuf, qbuf, partbuf, rtbuf, kapbuf]
        var bufs_partf <- [kbuf, vbuf, qbuf, partbuf, rtbuf, laybuf, kvdimbuf, nheadsbuf, kvmulbuf, hsbuf, scalebuf, qstridebuf, nchmax32buf]
        var bufs_partg <- [kbuf, vbuf, qbuf, partbuf, rtbuf, laybuf, kvdimbuf, nheadsbuf, kvmulbuf, hsbuf, scalebuf, qstridebuf, nsgsbuf]
        var bufs_partd <- [kbuf, vbuf, qbuf, partbuf, rtbuf, laybuf, kvdimbuf, nheadsbuf, kvmulbuf, hsbuf, scalebuf, qstridebuf, nsgsdbuf]
        var bufs_comb <- [partbuf, ybuf, kacbuf, rtbuf]   // production layout ka@2, rt@3; sink@20 unbound (hass=0, never read)
        var bufs_combp <- [partbuf, ybuf, rtbuf, nchmax32buf, nheadsbuf, hsbuf]
        var bufs_combg <- [partbuf, ybuf, rtbuf, nsgsbuf, nheadsbuf, hsbuf]
        var bufs_combd <- [partbuf, ybuf, rtbuf, nsgsdbuf, nheadsbuf, hsbuf]
        let groups_part = uint3(uint(nheads * nch * nb), 1u, 1u)
        let groups_partf = uint3(uint(nheads), uint((nch32 + 3l) / 4l), uint(nb))
        let groups_partg = uint3(uint(nheads), uint((nch64 + 3l) / 4l), uint(nb))
        let groups_partd = uint3(uint(nheads), uint((nsgsd + 3l) / 4l), uint(nb))
        let groups_comb = uint3(uint(nheads * nb), 1u, 1u)
        let groups_combp = uint3(uint(nheads), uint(nb), 1u)
        var best <- [1.0e30lf, 1.0e30lf, 1.0e30lf, 1.0e30lf]
        var est <- [0.0lf, 0.0lf, 0.0lf, 0.0lf]
        var names <- ["part+comb", "partD+combG", "partF+comb32", "partF64+combG"]
        // warmup + correctness, each arm against the CPU oracle
        for (arm in range(4)) {
            var ms0 = 0.0lf
            if (arm == 0) {
                ms0 = time_pair(queue, pso_part, bufs_part, groups_part, 128u, MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_tgmem,
                    pso_comb, bufs_comb, groups_comb, 128u, 0ul, 1)
            } elif (arm == 1) {
                ms0 = time_pair(queue, pso_partd, bufs_partd, groups_partd, 128u, lab_sq_attn_part_d_msl_tgmem,
                    pso_combp, bufs_combd, groups_combp, 128u, lab_sq_attn_comb_p_msl_tgmem, 1)
            } elif (arm == 2) {
                ms0 = time_pair(queue, pso_partf, bufs_partf, groups_partf, 128u, 0ul,
                    pso_combp, bufs_combp, groups_combp, 128u, lab_sq_attn_comb_p_msl_tgmem, 1)
            } else {
                ms0 = time_pair(queue, pso_partg, bufs_partg, groups_partg, 128u, 0ul,
                    pso_combp, bufs_combg, groups_combp, 128u, lab_sq_attn_comb_p_msl_tgmem, 1)
            }
            if (ms0 < 0.0lf) {
                b |> success(false, "{names[arm]} d{depth}: dispatch")
                continue
            }
            est[arm] = ms0
            ygot |> resize(int(nb * qd))
            unsafe {
                memcpy(addr < void? >(ygot[0]), metal_buffer_contents(ybuf), nb * qd * 4l)
            }
            var maxd = 0.0
            for (i in range64(nb * qd)) {
                maxd = max(maxd, abs(ygot[i] - yref[i]))
            }
            b |> success(maxd < 5.0e-3, "{names[arm]} d{depth}: matches CPU twin (maxd {maxd})")
        }
        // interleaved best-of rounds
        for (_round in range(rounds)) {
            for (arm in range(4)) {
                let reps = clamp(int(5.0lf / max(est[arm], 0.005lf)) + 1, 1, 256)
                var ms = 0.0lf
                if (arm == 0) {
                    ms = time_pair(queue, pso_part, bufs_part, groups_part, 128u, MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_tgmem,
                        pso_comb, bufs_comb, groups_comb, 128u, 0ul, reps)
                } elif (arm == 1) {
                    ms = time_pair(queue, pso_partd, bufs_partd, groups_partd, 128u, lab_sq_attn_part_d_msl_tgmem,
                        pso_combp, bufs_combd, groups_combp, 128u, lab_sq_attn_comb_p_msl_tgmem, reps)
                } elif (arm == 2) {
                    ms = time_pair(queue, pso_partf, bufs_partf, groups_partf, 128u, 0ul,
                        pso_combp, bufs_combp, groups_combp, 128u, lab_sq_attn_comb_p_msl_tgmem, reps)
                } else {
                    ms = time_pair(queue, pso_partg, bufs_partg, groups_partg, 128u, 0ul,
                        pso_combp, bufs_combg, groups_combp, 128u, lab_sq_attn_comb_p_msl_tgmem, reps)
                }
                if (ms > 0.0lf && ms < best[arm]) {
                    best[arm] = ms
                }
            }
        }
        let kvbytes = double(nb * nheads * depth * head_size * 2l * 2l)
        to_log(LOG_INFO, "attn_b2 d{depth}: old {best[0]} | partD {best[1]} | partF {best[2]} | partF64 {best[3]} ms ({kvbytes / (best[1] * 1.0e6lf)} KV GB/s) | D/old {best[1] / best[0]} | F/old {best[2] / best[0]} | F64/old {best[3] / best[0]}\n")
        delete best
        delete est
        delete names
        bufs_part |> clear()
        bufs_partf |> clear()
        bufs_partg |> clear()
        bufs_partd |> clear()
        bufs_comb |> clear()
        bufs_combp |> clear()
        bufs_combg |> clear()
        bufs_combd |> clear()
        unsafe {
            delete bufs_part
            delete bufs_partf
            delete bufs_partg
            delete bufs_partd
            delete bufs_comb
            delete bufs_combp
            delete bufs_combg
            delete bufs_combd
        }
        metal_release(kapbuf)
        metal_release(kacbuf)
        metal_release(nchmax32buf)
        metal_release(nsgsbuf)
        metal_release(nsgsdbuf)
        metal_release(rtbuf)
        delete rte
        delete yref
    }
    metal_release(kbuf)
    metal_release(vbuf)
    metal_release(qbuf)
    metal_release(ybuf)
    metal_release(partbuf)
    metal_release(laybuf)
    metal_release(kvdimbuf)
    metal_release(nheadsbuf)
    metal_release(kvmulbuf)
    metal_release(hsbuf)
    metal_release(scalebuf)
    metal_release(qstridebuf)
    metal_release(pso_part)
    metal_release(pso_comb)
    metal_release(pso_partp)
    metal_release(pso_partf)
    metal_release(pso_partg)
    metal_release(pso_partd)
    metal_release(pso_combp)
    metal_release(queue)
    metal_release(dev)
    delete kc
    delete vc
    delete q
    delete ygot
    assert_no_metal_leaks()
}

def private run_sq_attn(b) {   // untyped param = generic — non-Apple builds never compile the Metal-typed body
    let rounds = g_env_bench.lab_rounds
    // llama-3B geometry
    let nheads = 24l
    let head_size = 128l
    let kv_dim = 1024l
    let kv_mul = 3l
    let qd = nheads * head_size
    let maxrows = 2048l
    let scale = 1.0 / sqrt(float(head_size))
    var dev = metal_create_system_default_device()
    if (dev == null) {
        to_log(LOG_WARNING, "skipped: no Metal device\n")
        return
    }
    to_log(LOG_INFO, "device={metal_device_name(dev)} rounds={rounds}\n")
    var queue = metal_new_command_queue(dev)
    var err : string
    var pso = pipeline_from_source(dev, metal_sq_attn_msl, metal_sq_attn_msl_entry, true, err)
    b |> success(pso != null, "sq attention kernel compiles")
    var pso8 = pipeline_from_source(dev, lab_sq_attn_q8_msl, lab_sq_attn_q8_msl_entry, true, err)
    b |> success(pso8 != null, "q8_0-cache sq attention kernel compiles")
    var pso4 = pipeline_from_source(dev, lab_sq_attn_tq4_msl, lab_sq_attn_tq4_msl_entry, true, err)
    b |> success(pso4 != null, "tq4-cache sq attention kernel compiles")
    if (pso == null || pso8 == null || pso4 == null) {
        to_log(LOG_ERROR, "compile failed: {err}\n")
        metal_release(queue)
        metal_release(dev)
        return
    }
    // exact-in-f16 fills: small ints for K/V, small ints for q
    var kf : array<float>
    var vf : array<float>
    kf |> resize(int(maxrows * kv_dim))
    vf |> resize(int(maxrows * kv_dim))
    var kc : array<float16>
    var vc : array<float16>
    kc |> resize(int(maxrows * kv_dim))
    vc |> resize(int(maxrows * kv_dim))
    for (i in range64(maxrows * kv_dim)) {
        kf[i] = float(int((i * 7l + i / 129l) % 9l) - 4)
        vf[i] = float(int((i * 11l + i / 257l) % 9l) - 4)
        kc[i] = float16(kf[i])
        vc[i] = float16(vf[i])
    }
    var q : array<float>
    q |> resize(int(16l * qd))
    for (i in range64(16l * qd)) {
        q[i] = float(int((i * 13l + i / 61l) % 9l) - 4)
    }
    // the q8_0 arm's cache: encode the SAME rows through the CPU codec (the store-side oracle),
    // and give the correctness gate a dequantized-f32 oracle so codec error stays out of it
    let nblk_row = kv_dim / 32l
    let rowb = nblk_row * 34l
    var k8 : array<uint8>
    var v8 : array<uint8>
    k8 |> resize(int(maxrows * rowb))
    v8 |> resize(int(maxrows * rowb))
    var kdq : array<float>
    var vdq : array<float>
    kdq |> resize(int(maxrows * kv_dim))
    vdq |> resize(int(maxrows * kv_dim))
    unsafe {
        quantize_q8kv_row(addr(k8[0]), addr(kf[0]), maxrows * kv_dim)
        quantize_q8kv_row(addr(v8[0]), addr(vf[0]), maxrows * kv_dim)
        cvt_q8kv_to_f32(addr(kdq[0]), addr(k8[0]), maxrows * kv_dim)
        cvt_q8kv_to_f32(addr(vdq[0]), addr(v8[0]), maxrows * kv_dim)
    }
    // the tq4 arm's cache: rotate every K/V/q head (the store-seam basis change), encode through
    // the CPU codec, dequantized-f32 (still-rotated) oracle — the whole arm runs in the rotated
    // basis, exactly as production attention does
    let rowb4 = nblk_row * 18l
    var signs : array<float>
    tq4_fill_signs(signs, head_size)
    var kr := kf
    var vr := vf
    var qr := q
    unsafe {
        for (r in range64(maxrows)) {
            for (hh in range64(kv_dim / head_size)) {
                fwht_signs_row(addr(kr[r * kv_dim + hh * head_size]), addr(signs[0]), head_size)
                fwht_signs_row(addr(vr[r * kv_dim + hh * head_size]), addr(signs[0]), head_size)
            }
        }
        for (r in range64(16l)) {
            for (hh in range64(nheads)) {
                fwht_signs_row(addr(qr[r * qd + hh * head_size]), addr(signs[0]), head_size)
            }
        }
    }
    var k4 : array<uint8>
    var v4 : array<uint8>
    k4 |> resize(int(maxrows * rowb4))
    v4 |> resize(int(maxrows * rowb4))
    var kdq4 : array<float>
    var vdq4 : array<float>
    kdq4 |> resize(int(maxrows * kv_dim))
    vdq4 |> resize(int(maxrows * kv_dim))
    unsafe {
        quantize_tq4kv_row(addr(k4[0]), addr(kr[0]), maxrows * kv_dim)
        quantize_tq4kv_row(addr(v4[0]), addr(vr[0]), maxrows * kv_dim)
        cvt_tq4kv_to_f32(addr(kdq4[0]), addr(k4[0]), maxrows * kv_dim)
        cvt_tq4kv_to_f32(addr(vdq4[0]), addr(v4[0]), maxrows * kv_dim)
    }
    var kbuf = upload_array(dev, kc, maxrows * kv_dim * 2l)
    var vbuf = upload_array(dev, vc, maxrows * kv_dim * 2l)
    var k8buf = upload_array(dev, k8, maxrows * rowb)
    var v8buf = upload_array(dev, v8, maxrows * rowb)
    var k4buf = upload_array(dev, k4, maxrows * rowb4)
    var v4buf = upload_array(dev, v4, maxrows * rowb4)
    var qrbuf = upload_array(dev, qr, 16l * qd * 4l)
    var nblkbuf = upload_u32(dev, uint(nblk_row))
    var qbuf = upload_array(dev, q, 16l * qd * 4l)
    var ybuf = metal_new_buffer(dev, uint64(16l * qd * 4l))
    var kvdimbuf = upload_u32(dev, uint(kv_dim))
    var nheadsbuf = upload_u32(dev, uint(nheads))
    var kvmulbuf = upload_u32(dev, uint(kv_mul))
    var scalebuf = upload_f32(dev, scale)
    var ygot : array<float>
    for (depth in [512l, 2048l]) {
        var cntbuf = upload_u32(dev, uint(depth))
        var yref <- cpu_sq_attn(kc, vc, q, 8l, depth, nheads, head_size, kv_dim, kv_mul, scale)
        var yref8 <- cpu_sq_attn(kdq, vdq, q, 8l, depth, nheads, head_size, kv_dim, kv_mul, scale)
        var yref4 <- cpu_sq_attn(kdq4, vdq4, qr, 8l, depth, nheads, head_size, kv_dim, kv_mul, scale)
        for (nb in [1, 8]) {
            var bufs <- [kbuf, vbuf, qbuf, ybuf, cntbuf, kvdimbuf, nheadsbuf, kvmulbuf, scalebuf]
            let groups = uint3(uint(nb * int(nheads)), 1u, 1u)
            let ms0 = time_reps(queue, pso, bufs, groups, 1, metal_sq_attn_msl_tgmem)
            if (ms0 < 0.0lf) {
                b |> success(false, "sq_attn d{depth} B{nb}: dispatch")
                bufs |> clear()
                unsafe {
                    delete bufs
                }
                continue
            }
            ygot |> resize(int(16l * qd))
            unsafe {
                memcpy(addr < void? >(ygot[0]), metal_buffer_contents(ybuf), int64(nb) * qd * 4l)
            }
            var maxd = 0.0
            for (i in range64(int64(nb) * qd)) {
                maxd = max(maxd, abs(ygot[i] - yref[i]))
            }
            b |> success(maxd < 5.0e-3, "sq_attn d{depth} B{nb}: matches CPU twin (maxd {maxd})")
            var best = 1.0e30lf
            for (_round in range(rounds)) {
                let reps = clamp(int(5.0lf / max(ms0, 0.005lf)) + 1, 1, 256)
                let ms = time_reps(queue, pso, bufs, groups, reps, metal_sq_attn_msl_tgmem)
                if (ms > 0.0lf && ms < best) {
                    best = ms
                }
            }
            // logical KV traffic: every head reads its kv head's K+V over the context (GQA re-reads counted)
            let kvbytes = double(int64(nb) * nheads * depth * head_size * 2l * 2l)
            let gbs = kvbytes / (best * 1.0e6lf)
            to_log(LOG_INFO, "sq_attn  d{depth} B{nb}  {best} ms  {gbs} KV GB/s (logical)\n")
            bufs |> clear()
            unsafe {
                delete bufs
            }

            // the q8_0-cache arm: same q/y, dual-view K/V blobs (each blob bound twice)
            var bufs8 <- [k8buf, k8buf, v8buf, v8buf, qbuf, ybuf, cntbuf, nblkbuf, nheadsbuf, kvmulbuf, scalebuf]
            let ms08 = time_reps(queue, pso8, bufs8, groups, 1, lab_sq_attn_q8_msl_tgmem)
            if (ms08 < 0.0lf) {
                b |> success(false, "sq_attn_q8 d{depth} B{nb}: dispatch")
                bufs8 |> clear()
                unsafe {
                    delete bufs8
                }
                continue
            }
            unsafe {
                memcpy(addr < void? >(ygot[0]), metal_buffer_contents(ybuf), int64(nb) * qd * 4l)
            }
            var maxd8 = 0.0
            for (i in range64(int64(nb) * qd)) {
                maxd8 = max(maxd8, abs(ygot[i] - yref8[i]))
            }
            b |> success(maxd8 < 5.0e-3, "sq_attn_q8 d{depth} B{nb}: matches dequantized CPU twin (maxd {maxd8})")
            var best8 = 1.0e30lf
            for (_round in range(rounds)) {
                let reps = clamp(int(5.0lf / max(ms08, 0.005lf)) + 1, 1, 256)
                let ms = time_reps(queue, pso8, bufs8, groups, reps, lab_sq_attn_q8_msl_tgmem)
                if (ms > 0.0lf && ms < best8) {
                    best8 = ms
                }
            }
            // physical traffic: 34B per 32 elements, K+V (the BW bar vs f16 is 0.53x bytes)
            let kvbytes8 = double(int64(nb) * nheads * depth * (head_size / 32l * 34l) * 2l)
            let gbs8 = kvbytes8 / (best8 * 1.0e6lf)
            to_log(LOG_INFO, "sq_attn_q8 d{depth} B{nb}  {best8} ms  {gbs8} KV GB/s (physical)  f16/q8 speedup {best / best8}x\n")
            bufs8 |> clear()
            unsafe {
                delete bufs8
            }

            // the tq4-cache arm: rotated q, rotated-basis oracle, dual-view nibble blobs
            var bufs4 <- [k4buf, k4buf, v4buf, v4buf, qrbuf, ybuf, cntbuf, nblkbuf, nheadsbuf, kvmulbuf, scalebuf]
            let ms04 = time_reps(queue, pso4, bufs4, groups, 1, lab_sq_attn_tq4_msl_tgmem)
            if (ms04 < 0.0lf) {
                b |> success(false, "sq_attn_tq4 d{depth} B{nb}: dispatch")
                bufs4 |> clear()
                unsafe {
                    delete bufs4
                }
                continue
            }
            unsafe {
                memcpy(addr < void? >(ygot[0]), metal_buffer_contents(ybuf), int64(nb) * qd * 4l)
            }
            var maxd4 = 0.0
            for (i in range64(int64(nb) * qd)) {
                maxd4 = max(maxd4, abs(ygot[i] - yref4[i]))
            }
            b |> success(maxd4 < 5.0e-3, "sq_attn_tq4 d{depth} B{nb}: matches dequantized rotated CPU twin (maxd {maxd4})")
            var best4 = 1.0e30lf
            for (_round in range(rounds)) {
                let reps = clamp(int(5.0lf / max(ms04, 0.005lf)) + 1, 1, 256)
                let ms = time_reps(queue, pso4, bufs4, groups, reps, lab_sq_attn_tq4_msl_tgmem)
                if (ms > 0.0lf && ms < best4) {
                    best4 = ms
                }
            }
            // physical traffic: 18B per 32 elements, K+V (0.28x the f16 bytes)
            let kvbytes4 = double(int64(nb) * nheads * depth * (head_size / 32l * 18l) * 2l)
            let gbs4 = kvbytes4 / (best4 * 1.0e6lf)
            to_log(LOG_INFO, "sq_attn_tq4 d{depth} B{nb}  {best4} ms  {gbs4} KV GB/s (physical)  f16/tq4 speedup {best / best4}x\n")
            bufs4 |> clear()
            unsafe {
                delete bufs4
            }
        }
        metal_release(cntbuf)
        delete yref
        delete yref8
        delete yref4
    }
    metal_release(kbuf)
    metal_release(vbuf)
    metal_release(k8buf)
    metal_release(v8buf)
    metal_release(k4buf)
    metal_release(v4buf)
    metal_release(qrbuf)
    metal_release(nblkbuf)
    metal_release(qbuf)
    metal_release(ybuf)
    metal_release(kvdimbuf)
    metal_release(nheadsbuf)
    metal_release(kvmulbuf)
    metal_release(scalebuf)
    metal_release(pso)
    metal_release(pso8)
    metal_release(pso4)
    metal_release(queue)
    metal_release(dev)
    delete kf
    delete vf
    delete kc
    delete vc
    delete k8
    delete v8
    delete kdq
    delete vdq
    delete signs
    delete kr
    delete vr
    delete qr
    delete k4
    delete v4
    delete kdq4
    delete vdq4
    delete q
    delete ygot
    assert_no_metal_leaks()
}

[benchmark, unused_argument(b)]   // b is consumed only inside the Apple static_if half
def bench_metal_sq_attn(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        run_sq_attn(b)
        run_attn_b2_sidebyside(b)
    }
}
