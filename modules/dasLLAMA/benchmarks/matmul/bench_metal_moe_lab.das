options gen2
options persistent_heap
options stack = 65536

require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — GPU driver half, Apple builds only
require dasllama/dasllama_env  // env_int: the shared typed readers + the knob registry
require metal/msl_shader
require dasllama/dasllama_metal_prefill   // the production MoE mul_mm MSL (zero copy drift)
require dasllama/dasllama_metal_kernels   // the production MoE GEMV + geglu MSL (zero copy drift)
require llvm/daslib/f16_cvt
require daslib/fio   // nolint:STYLE030 — used only inside the Apple static_if half
require math   // nolint:STYLE030 — used only inside the Apple static_if half
require daslib/math_bits   // nolint:STYLE030 — uint_bits_to_float in the lcppe kernel
require strings

// The routed-expert GEMV lab — the 26B tg dig's barrier/dispatch attribution rig. The knockout
// chase convicted the routed k4/q51 expert GEMVs (5.48ms/step at ~163 wGB/s vs the ~300-340 the
// same k4 dot sustains in bench_metal_gemv_kernels), with the dense-shared branch NEAR ceiling in
// the same graph — so the suspect is the DISPATCH STRUCTURE, not the dot: the single-decode
// driver encodes on a SERIAL encoder (implicit barrier after every dispatch, ~330/step) while
// only the batch rail has the concurrent-encoder + explicit-hazard form.
//
// This rig re-creates the routed chain at the EXACT 26B shapes (E=128, nfe=704, dim=2816, k=8,
// 30 layers: w1+w3 k4, geglu ew, w2 q51) on a CONCURRENT encoder and prices each structure:
//   pipe      no barriers, per-layer disjoint buffers — the kernels' pipelined ceiling
//   pipe_c    pipe with CONTIGUOUS sel — expert-scatter locality control (vs pipe's scattered)
//   serial    barrier after EVERY dispatch — today's serial-encoder semantics
//   hazard    barriers only at real hazards (w1 ∥ w3, then ew, then w2) — the port candidate
//   pair13    the fused W1+W3 lab kernel (one dispatch streams both planes per slot) + hazard
//   true_ser  a genuine serial encoder, no explicit barriers — sanity: must match `serial`
// Weight stacks total ~476MB (>> 48MB SLC) and sel rotates per layer, so passes stay DRAM-honest.
// Correctness: exact-arithmetic fills (lab contract: pow2 scales, small ints) check the
// standalone w1 (k4) and w2 (q51) sites against CPU refs — the chain arms are timing-only
// (geglu is transcendental; the production kernels carry their own in-graph parity elsewhere).
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/bench_metal_moe_lab.das
//   DASMETAL_LAB_ROUNDS (default 3, best-of), DASMETAL_LAB_PASSES (default 4, per cb),
//   DASMETAL_LAB_ARMS (substring filter)

let E = 128l          // experts per logical stack (w1 planes 0..127, w3 planes 128..255)
let NFE = 704l        // expert FFN width (gemma4.expert_feed_forward_length)
let DIM = 2816l       // model dim
let K = 8l            // experts used per token
let LAYERS = 30l
let KNFE = K * NFE    // one h12 half, floats

let NSB_ROW = DIM / 256l           // k4 superblocks per w1/w3 row
let ESB = NFE * NSB_ROW            // k4 superblocks per expert plane
let NBLK_ROW = NFE / 32l           // q51 blocks per w2 row
let EBLK = DIM * NBLK_ROW          // q51 blocks per expert plane

// ===== the paired W1+W3 lab kernel =====
// MetalMoeGemvK4's dot with a SECOND weight stream: the w3 plane rides +E expert slots in the
// same stack, sharing the x loads (yl/yh) — one dispatch per layer instead of two, and double
// the weight ILP per x fetch. y = [gate: k x nfe][up: k x nfe], the production h12 layout.
class MoeGemvK4Pair {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 sel : array<uint>
    @uniform @binding = 8 esb : uint
    @uniform @binding = 9 eshift : uint     // w3 plane offset, expert slots (= E)
    @uniform @binding = 10 knfe : uint      // up-half y offset, elements

    [metal_kernel(name="lab_moe_k4_pair_msl")]
    def lab_moe_k4_pair {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let eb1 = sel[slot] * esb
        let eb3 = (sel[slot] + eshift) * esb
        let y0 = slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[4]
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
            for [unroll_full] (p in range(4)) {   // p/2 = plane (w1/w3), p%2 = row of the pair
                let blk = (p < 2 ? eb1 : eb3) + (first_row + uint(p % 2)) * nb + ib
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
                sumf[p] += dd * ((acc1[0] + 0.00390625 * acc1[1]) * float(sc0 & 255u)
                    + (acc1[2] + 0.00390625 * acc1[3]) * float(sc0 >> 8u) * 0.0625
                    + (acc2[0] + 0.00390625 * acc2[1]) * float(sc2 & 255u)
                    + (acc2[2] + 0.00390625 * acc2[3]) * float(sc2 >> 8u) * 0.0625)
                sumf[p] -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                    + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (p in range(4)) {
            let s = simd_sum(sumf[p])
            if (lane == 0u && first_row + uint(p % 2) < ddim) {
                y[(p < 2 ? 0u : knfe) + y0 + first_row + uint(p % 2)] = s
            }
        }
    }
}

// x-vector variant: the checklist's "vector-width ssbo views" lever — the production kernel's 32
// scalar x loads per block become 8 float4 loads (same bytes, quarter the load issues).
class MoeGemvK4X4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x4 : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 sel : array<uint>
    @uniform @binding = 8 esb : uint
    @uniform @binding = 9 xs : uint

    [metal_kernel(name="lab_moe_k4_x4_msl")]
    def lab_moe_k4_x4 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let eb = sel[slot] * esb
        let xoff = slot * xs
        let y0 = slot * ddim
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

// 4-rows-per-simdgroup variant: the checklist's "fewer long-running sgs" lever — one x window
// feeds 4 weight rows (half the x issues per weight byte of the production 2-row form).
class MoeGemvK4R4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 sel : array<uint>
    @uniform @binding = 8 esb : uint
    @uniform @binding = 9 xs : uint

    [metal_kernel(name="lab_moe_k4_r4_msl")]
    def lab_moe_k4_r4 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let eb = sel[slot] * esb
        let xoff = slot * xs
        let y0 = slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 4u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[4]
        var ib = ix
        while (ib < nb) {
            let yb = xoff + ib * 256u + iq * 64u + ir * 8u
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
            for [unroll_full] (r in range(4)) {
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
        for [unroll_full] (r in range(4)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[y0 + first_row + uint(r)] = s
            }
        }
    }
}

// q51 integer-compose variant: the production kernel's per-element 5th-bit SELECT chain (4 cmps +
// 4 selects + 3 adds + the 16*hx mad per float4) becomes an integer OR into the nibble before the
// convert — q | (hbit << 4) == q + 16*hbit exactly, one dot chain instead of two.
class MoeGemvQ51Ic {
    @ssbo @binding = 0 wqu : array<uint>
    @ssbo @binding = 1 wsh : array<float16>
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>
    @uniform @binding = 7 eblk : uint
    @uniform @binding = 8 xs : uint

    [metal_kernel(name="lab_moe_q51_ic_msl")]
    def lab_moe_q51_ic {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let nb = ndim / 32u
        let slot = gl_WorkGroupID.y
        let eb = sel[slot] * eblk
        let xoff = slot * xs
        let y0 = slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
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
                let q0 = ((u >> nsh) & 15u) | ((qh & 1u) << 4u)
                let q1 = ((u >> (8u + nsh)) & 15u) | ((qh & 2u) << 3u)
                let q2 = ((u >> (16u + nsh)) & 15u) | ((qh & 4u) << 2u)
                let q3 = ((u >> (24u + nsh)) & 15u) | ((qh & 8u) << 1u)
                sumf[r] += float(wsh[blk * 2u]) * (float(q0) * xv.x + float(q1) * xv.y
                    + float(q2) * xv.z + float(q3) * xv.w) + float(wsh[blk * 2u + 1u]) * sx
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

// ===== exact-arithmetic fills (bench_metal_gemv_kernels contract, gg stream dropped) =====

def private fill_k4_stack(var kq : array<uint8>; var kc : array<uint8>; rows, k, seed : int64) {
    let nsb = k / 256l
    let total = rows * nsb
    kq |> resize(total * 128l)
    kc |> resize(total * 16l)
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    for (sb in range64(total)) {
        let db = f32_to_f16(lut[int((sb / 5l) % 4l)])
        let mb = f32_to_f16(lut[int((sb / 3l + 1l) % 4l)])
        let co = sb * 16l
        kc[co] = uint8(db & 255u)
        kc[co + 1l] = uint8(db >> 8u)
        kc[co + 2l] = uint8(mb & 255u)
        kc[co + 3l] = uint8(mb >> 8u)
        for (j in range64(4l)) {   // ggml scales[12]: [0,7] values keep the packed high bits zero
            kc[co + 4l + j] = uint8(int((sb + j * 5l + seed) % 8l))
            kc[co + 8l + j] = uint8(int((sb * 3l + j * 7l + 1l) % 8l))
            kc[co + 12l + j] = uint8(int((sb + (j + 4l) * 5l + seed) % 8l) | (int((sb * 3l + (j + 4l) * 7l + 1l) % 8l) << 4))
        }
        let qo = sb * 128l
        for (b in range64(128l)) {
            let lo = int((b * 7l + sb * 13l + seed) % 16l)
            let hi = int((b * 11l + sb * 5l + 3l) % 16l)
            kq[qo + b] = uint8(lo | (hi << 4))
        }
    }
}

// 6-bit sub-scale/min j of a 12-byte packed Q4_K scale block (ggml get_scale_min_k4)
def private k4_sc_mn(kc : array<uint8>; base, j : int64) : int2 {
    if (j < 4l) {
        return int2(int(kc[base + j]) & 63, int(kc[base + j + 4l]) & 63)
    }
    let sc = (int(kc[base + j + 4l]) & 15) | ((int(kc[base + j - 4l]) >> 6) << 4)
    let mn = (int(kc[base + j + 4l]) >> 4) | ((int(kc[base + j]) >> 6) << 4)
    return int2(sc, mn)
}

// CPU k4 ref, one row of one expert plane against xf
def private ref_k4_row(kq : array<uint8>; kc : array<uint8>; xf : array<float>; plane_sb0, row, n : int64) : float {
    var acc = 0.0
    let nsb = n / 256l
    for (sb in range64(nsb)) {
        let g = plane_sb0 + row * nsb + sb
        let co = g * 16l
        let qo = g * 128l
        let dd = f16_to_f32(uint(kc[co]) | (uint(kc[co + 1l]) << 8u))
        let dm = f16_to_f32(uint(kc[co + 2l]) | (uint(kc[co + 3l]) << 8u))
        for (js in range64(8l)) {
            let s = k4_sc_mn(kc, co + 4l, js)
            for (l in range64(32l)) {
                let qv = int(kq[qo + (js / 2l) * 32l + l])
                let q = (js & 1l) == 0l ? (qv & 15) : (qv >> 4)
                acc += (dd * float(s.x) * float(q) - dm * float(s.y)) * xf[sb * 256l + js * 32l + l]
            }
        }
    }
    return acc
}

// q51 production planes: quant 20B blocks ([16B qs][4B qh]) + scale plane (d,m halves per block)
def private fill_q51_stack(var wq : array<uint8>; var ws : array<uint8>; rows, k, seed : int64) {
    let nblk = k / 32l
    let total = rows * nblk
    wq |> resize(total * 20l)
    ws |> resize(total * 4l)
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    for (b in range64(total)) {
        let db = f32_to_f16(lut[int((b / 7l) % 4l)])
        let mb = f32_to_f16(lut[int((b / 3l + 2l) % 4l)])
        let so = b * 4l
        ws[so] = uint8(db & 255u)
        ws[so + 1l] = uint8(db >> 8u)
        ws[so + 2l] = uint8(mb & 255u)
        ws[so + 3l] = uint8(mb >> 8u)
        let qo = b * 20l
        for (j in range64(16l)) {
            let lo = int((j * 7l + b * 13l + seed) % 16l)
            let hi = int((j * 11l + b * 5l + 3l) % 16l)
            wq[qo + j] = uint8(lo | (hi << 4))
        }
        for (j in range64(4l)) {
            wq[qo + 16l + j] = uint8(int((b * 29l + j * 17l + seed) % 256l))
        }
    }
}

// CPU q51 ref, one row of one expert plane: w = d*(q + 16*qh_bit) + m per element
def private ref_q51_row(wq : array<uint8>; ws : array<uint8>; xf : array<float>; plane_b0, row, n : int64) : float {
    var acc = 0.0
    let nblk = n / 32l
    for (blk in range64(nblk)) {
        let g = plane_b0 + row * nblk + blk
        let so = g * 4l
        let dd = f16_to_f32(uint(ws[so]) | (uint(ws[so + 1l]) << 8u))
        let dm = f16_to_f32(uint(ws[so + 2l]) | (uint(ws[so + 3l]) << 8u))
        let qo = g * 20l
        for (l in range64(32l)) {
            let qv = int(wq[qo + (l % 16l)])
            let q = l < 16l ? (qv & 15) : (qv >> 4)
            let hbit = (int(wq[qo + 16l + l / 8l]) >> int(l % 8l)) & 1
            acc += (dd * float(q + 16 * hbit) + dm) * xf[blk * 32l + l]
        }
    }
    return acc
}

// ===== host plumbing =====

def private want(filter_env, name : string) : bool {
    return !has_env_variable(filter_env) || find(get_env_variable(filter_env), name) >= 0
}

// generic (untyped param) — non-Apple builds never compile the Metal-typed body
def private upload_bytes(dev; arr : array<uint8>) {
    var buf = metal_new_buffer_untracked(dev, uint64(long_length(arr)))
    unsafe {
        memcpy(metal_buffer_contents(buf), addr < void? >(arr[0]), long_length(arr))
    }
    return buf
}

def private upload_floats(dev; arr : array<float>) {
    var buf = metal_new_buffer_untracked(dev, uint64(length(arr) * 4))
    unsafe {
        memcpy(metal_buffer_contents(buf), addr < void? >(arr[0]), length(arr) * 4)
    }
    return buf
}

def private upload_u32v(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    unsafe {
        var p = reinterpret<uint?>(metal_buffer_contents(buf))
        p[0] = v
    }
    return buf
}

// one command buffer: encoder (serial or concurrent), caller encodes, gpu window returned in us
def private time_arm(queue; concurrent : bool; blk) : double {
    var cb = metal_new_command_buffer(queue)
    var enc = concurrent ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
    blk |> invoke(enc)
    metal_end_encoding(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    let err = metal_command_buffer_error(cb)
    if (!empty(err)) {
        panic("command buffer error: {err}")
    }
    let us = (metal_command_buffer_gpu_end_time(cb) - metal_command_buffer_gpu_start_time(cb)) * 1e6lf
    metal_release(enc)
    metal_release(cb)
    return us
}

// bufs slots (array<MetalBuffer?> local to the generic body — Metal types never at module level)
let RB_K4Q = 0     // k4 quant stack, 2E planes (w1 = 0..E-1, w3 = E..2E-1)
let RB_K4S = 1     // k4 16B compact scale stack
let RB_Q51Q = 2    // q51 quant stack, E planes
let RB_Q51S = 3    // q51 d/m scale stack
let RB_X = 4       // shared activation vector [DIM]
let RB_H12 = 5     // per-layer [gate: K x NFE][up: K x NFE]
let RB_DN = 6      // per-layer [K x DIM]
let RB_SEL = 7     // sel rows: [scatter w1 | scatter w3 | contig w1 | contig w3] x LAYERS
let RB_UDIM = 8
let RB_UNFE = 9
let RB_UESB = 10
let RB_UEBLK = 11
let RB_UZERO = 12
let RB_UXSNFE = 13
let RB_UKNFE = 14
let RB_UESHIFT = 15
// psos slots
let RP_K4 = 0      // production expert-indexed k4 GEMV
let RP_Q51 = 1     // production expert-indexed q51 GEMV
let RP_GLU = 2     // production geglu ew
let RP_PAIR = 3    // the lab W1+W3 paired kernel
let RP_X4 = 4      // float4-x variant
let RP_R4 = 5      // 4-rows-per-simdgroup variant
let RP_Q51IC = 6   // q51 integer-compose variant

let SEL_ROW_BYTES = 32ul                          // 8 idx uints per row
let SEL_W3_BASE = uint64(LAYERS) * SEL_ROW_BYTES  // block starts in bsel
let SEL_CW1_BASE = SEL_W3_BASE * 2ul
let SEL_CW3_BASE = SEL_W3_BASE * 3ul

def private h12_off(l : int64) : uint64 => uint64(l * 2l * KNFE * 4l)
def private dn_off(l : int64) : uint64 => uint64(l * K * DIM * 4l)

def private enc_lab_w13(bufs; psos; enc; l : int64; sel_base : uint64; up_half : bool) {
    metal_set_pipeline(enc, psos[RP_K4])
    metal_set_buffer(enc, bufs[RB_K4S], 0ul, 0)
    metal_set_buffer(enc, bufs[RB_K4S], 0ul, 1)
    metal_set_buffer(enc, bufs[RB_K4Q], 0ul, 2)
    metal_set_buffer(enc, bufs[RB_X], 0ul, 3)
    metal_set_buffer(enc, bufs[RB_H12], h12_off(l) + (up_half ? uint64(KNFE * 4l) : 0ul), 4)
    metal_set_buffer(enc, bufs[RB_UDIM], 0ul, 5)
    metal_set_buffer(enc, bufs[RB_UNFE], 0ul, 6)
    metal_set_buffer(enc, bufs[RB_SEL], sel_base + uint64(l) * SEL_ROW_BYTES, 7)
    metal_set_buffer(enc, bufs[RB_UESB], 0ul, 8)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 9)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 10)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 11)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 12)
    metal_dispatch_threadgroups(enc, uint3(uint((NFE + 3l) / 4l), uint(K), 1u), uint3(64u, 1u, 1u))
}

// variant dispatch: same superset binding table (extra binds past a pipeline's table are ignored),
// rows_tg = output rows per threadgroup (4 for the 2-row forms, 8 for r4)
def private enc_lab_w13v(bufs; psos; enc; l : int64; sel_base : uint64; up_half : bool; pslot : int; rows_tg : int64) {
    metal_set_pipeline(enc, psos[pslot])
    metal_set_buffer(enc, bufs[RB_K4S], 0ul, 0)
    metal_set_buffer(enc, bufs[RB_K4S], 0ul, 1)
    metal_set_buffer(enc, bufs[RB_K4Q], 0ul, 2)
    metal_set_buffer(enc, bufs[RB_X], 0ul, 3)
    metal_set_buffer(enc, bufs[RB_H12], h12_off(l) + (up_half ? uint64(KNFE * 4l) : 0ul), 4)
    metal_set_buffer(enc, bufs[RB_UDIM], 0ul, 5)
    metal_set_buffer(enc, bufs[RB_UNFE], 0ul, 6)
    metal_set_buffer(enc, bufs[RB_SEL], sel_base + uint64(l) * SEL_ROW_BYTES, 7)
    metal_set_buffer(enc, bufs[RB_UESB], 0ul, 8)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 9)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 10)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 11)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 12)
    metal_dispatch_threadgroups(enc, uint3(uint((NFE + rows_tg - 1l) / rows_tg), uint(K), 1u), uint3(64u, 1u, 1u))
}

def private enc_lab_pair(bufs; psos; enc; l : int64; sel_base : uint64) {
    metal_set_pipeline(enc, psos[RP_PAIR])
    metal_set_buffer(enc, bufs[RB_K4S], 0ul, 0)
    metal_set_buffer(enc, bufs[RB_K4S], 0ul, 1)
    metal_set_buffer(enc, bufs[RB_K4Q], 0ul, 2)
    metal_set_buffer(enc, bufs[RB_X], 0ul, 3)
    metal_set_buffer(enc, bufs[RB_H12], h12_off(l), 4)
    metal_set_buffer(enc, bufs[RB_UDIM], 0ul, 5)
    metal_set_buffer(enc, bufs[RB_UNFE], 0ul, 6)
    metal_set_buffer(enc, bufs[RB_SEL], sel_base + uint64(l) * SEL_ROW_BYTES, 7)
    metal_set_buffer(enc, bufs[RB_UESB], 0ul, 8)
    metal_set_buffer(enc, bufs[RB_UESHIFT], 0ul, 9)
    metal_set_buffer(enc, bufs[RB_UKNFE], 0ul, 10)
    metal_dispatch_threadgroups(enc, uint3(uint((NFE + 3l) / 4l), uint(K), 1u), uint3(64u, 1u, 1u))
}

def private enc_lab_glu(bufs; psos; enc; l : int64) {
    metal_set_pipeline(enc, psos[RP_GLU])
    metal_set_buffer(enc, bufs[RB_H12], h12_off(l), 0)
    metal_set_buffer(enc, bufs[RB_H12], h12_off(l) + uint64(KNFE * 4l), 1)
    metal_set_buffer(enc, bufs[RB_UKNFE], 0ul, 2)
    metal_dispatch_threadgroups(enc, uint3(uint((KNFE + 255l) / 256l), 1u, 1u), uint3(256u, 1u, 1u))
}

def private enc_lab_w2(bufs; psos; enc; l : int64; sel_base : uint64; pslot : int = RP_Q51) {
    metal_set_pipeline(enc, psos[pslot])
    metal_set_buffer(enc, bufs[RB_Q51Q], 0ul, 0)
    metal_set_buffer(enc, bufs[RB_Q51S], 0ul, 1)
    metal_set_buffer(enc, bufs[RB_H12], h12_off(l), 2)
    metal_set_buffer(enc, bufs[RB_DN], dn_off(l), 3)
    metal_set_buffer(enc, bufs[RB_UNFE], 0ul, 4)
    metal_set_buffer(enc, bufs[RB_UDIM], 0ul, 5)
    metal_set_buffer(enc, bufs[RB_SEL], sel_base + uint64(l) * SEL_ROW_BYTES, 6)
    metal_set_buffer(enc, bufs[RB_UEBLK], 0ul, 7)
    metal_set_buffer(enc, bufs[RB_UXSNFE], 0ul, 8)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 9)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 10)
    metal_set_buffer(enc, bufs[RB_UZERO], 0ul, 11)
    metal_dispatch_threadgroups(enc, uint3(uint((DIM + 3l) / 4l), uint(K), 1u), uint3(64u, 1u, 1u))
}

// one pass of the routed chain under a barrier pattern: 0 = none, 1 = every dispatch (the
// serial-encoder semantics), 2 = real hazards only (w1 ∥ w3), 3 = paired W1+W3 + real hazards
def private enc_chain_pass(bufs; psos; enc; pattern : int; contig : bool) {
    let w1b = contig ? SEL_CW1_BASE : 0ul
    let w3b = contig ? SEL_CW3_BASE : SEL_W3_BASE
    for (l in range64(LAYERS)) {
        if (pattern == 3) {
            enc_lab_pair(bufs, psos, enc, l, w1b)
        } else {
            enc_lab_w13(bufs, psos, enc, l, w1b, false)
            if (pattern == 1) {
                metal_memory_barrier(enc)
            }
            enc_lab_w13(bufs, psos, enc, l, w3b, true)
        }
        if (pattern != 0) {
            metal_memory_barrier(enc)
        }
        enc_lab_glu(bufs, psos, enc, l)
        if (pattern != 0) {
            metal_memory_barrier(enc)
        }
        enc_lab_w2(bufs, psos, enc, l, w1b)
        if (pattern != 0) {
            metal_memory_barrier(enc)
        }
    }
}

def private compile_or_die(dev; src, entry : string; fastmath : bool) {
    var err : string
    var pso = pipeline_from_source(dev, src, entry, fastmath, err)
    if (pso == null) {
        panic("PSO '{entry}' failed: {err}")
    }
    return pso
}

// ===== gpt-oss mx4 rate section (reds-dig2) =====
// The chase convicted the routed mx4 expert GEMVs at 216 wGB/s IN-GRAPH (48% of every decode
// step) vs the 321 the k4 sibling sustains standalone. Same discriminator as the 26B dig: price
// the PRODUCTION MetalMoeGemvMx4 at the exact gpt-oss shapes on pipe (no barriers — kernel
// ceiling) vs serial (barrier per dispatch — the structure tax). pipe ~320 ⇒ structure; pipe
// ~216 ⇒ the kernel (vtab/etab device-table issue count is the disasm suspect).
let GE = 32l           // experts
let GNFE = 2880l       // expert ffn width
let GDIM = 2880l       // model dim
let GK = 4l            // experts used per token
let GLAYERS = 24l
let GNB_ROW = GDIM / 32l          // mx4 blocks per row
let GEBLK = GNFE * GNB_ROW        // blocks per expert plane

// ===== the arithmetic-decode variant =====
// The disasm indicts the tables: 15 device loads per block iteration, 8 of them vtab lookups
// serialized behind the nibble extracts. This form computes the doubled-e2m1 value in ALU
// (magnitude = m<2 ? m : (2+(m&1)) << ((m>>1)-1), sign select) — the e8m0 scale keeps its etab
// lookup (no exp2/bitcast in the MSL whitelist; 2 broadcast loads/block vs vtab's 8 divergent
// ones). Bindings = the production table's prefix + etab at 14 (extra binds are ignored).
class LabMoeGemvMx4A {
    @ssbo @binding = 0 mxu : array<uint>
    @ssbo @binding = 1 mxe : array<uint8>
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>
    @uniform @binding = 7 eblk : uint
    @uniform @binding = 8 xs : uint
    @uniform @binding = 9 ss : uint
    @uniform @binding = 10 bxs : uint
    @uniform @binding = 11 bys : uint
    @ssbo @binding = 14 etab : array<float>   // [256] exact e8m0_half scales (host-built)

    [metal_kernel(name="lab_moe_mx4_a_msl")]
    def lab_moe_mx4_a {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let nb = ndim / 32u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let e = sel[st * ss + slot]
        let eb = e * eblk
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        let half = it / 4u
        let ku = it % 4u
        let nsh = half * 4u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let xv = xf[(xoff + ib * 32u) / 4u + it]
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let u = mxu[blk * 4u + ku]
                let n0 = (u >> nsh) & 15u
                let m0 = n0 & 7u
                let a0 = float(m0 < 2u ? m0 : (2u + (m0 & 1u)) << ((m0 >> 1u) - 1u))
                var qx = (n0 < 8u ? a0 : -a0) * xv.x
                let n1 = (u >> (8u + nsh)) & 15u
                let m1 = n1 & 7u
                let a1 = float(m1 < 2u ? m1 : (2u + (m1 & 1u)) << ((m1 >> 1u) - 1u))
                qx += (n1 < 8u ? a1 : -a1) * xv.y
                let n2 = (u >> (16u + nsh)) & 15u
                let m2 = n2 & 7u
                let a2 = float(m2 < 2u ? m2 : (2u + (m2 & 1u)) << ((m2 >> 1u) - 1u))
                qx += (n2 < 8u ? a2 : -a2) * xv.z
                let n3 = (u >> (24u + nsh)) & 15u
                let m3 = n3 & 7u
                let a3 = float(m3 < 2u ? m3 : (2u + (m3 & 1u)) << ((m3 >> 1u) - 1u))
                qx += (n3 < 8u ? a3 : -a3) * xv.w
                sumf[r] += etab[uint(mxe[blk])] * qx
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

// ===== the tgmem-LUT variant (the lcpp shape) =====
// llama.cpp's kernel_mul_mv_mxfp4_f32 copies its 16-value LUT into THREADGROUP memory at kernel
// start (one load + one barrier) and the hot loop reads tgmem — zero device loads per lookup.
// Same change here, production kernel otherwise untouched: the 8 vtab device loads per block
// iteration become tgmem reads. etab stays a device table (2 broadcast loads/block, secondary).
class LabMoeGemvMx4T {
    @ssbo @binding = 0 mxu : array<uint>
    @ssbo @binding = 1 mxe : array<uint8>
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>
    @uniform @binding = 7 eblk : uint
    @uniform @binding = 8 xs : uint
    @uniform @binding = 9 ss : uint
    @uniform @binding = 10 bxs : uint
    @uniform @binding = 11 bys : uint
    @ssbo @binding = 14 etab : array<float>   // [256] exact e8m0_half scales (host-built)
    @ssbo @binding = 15 vtab : array<float>   // [16] signed doubled-e2m1 values (host-built)
    @workgroup vt : float[16]   // etab stays a device table: its reads are same-address
                                // broadcasts (uniform path) — the tgmem copy REGRESSED (253 vs 260)

    [metal_kernel(name="lab_moe_mx4_t_msl")]
    def lab_moe_mx4_t {
        let lane = gl_SubgroupInvocationID
        let tid = gl_SubgroupID * 32u + lane
        if (tid < 16u) {
            vt[tid] = vtab[tid]
        }
        barrier()
        let ix = lane / 8u
        let it = lane % 8u
        let nb = ndim / 32u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let e = sel[st * ss + slot]
        let eb = e * eblk
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        let half = it / 4u
        let ku = it % 4u
        let nsh = half * 4u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let xv = xf[(xoff + ib * 32u) / 4u + it]
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let u = mxu[blk * 4u + ku]
                var qx = vt[(u >> nsh) & 15u] * xv.x
                qx += vt[(u >> (8u + nsh)) & 15u] * xv.y
                qx += vt[(u >> (16u + nsh)) & 15u] * xv.z
                qx += vt[(u >> (24u + nsh)) & 15u] * xv.w
                sumf[r] += etab[uint(mxe[blk])] * qx
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

// ===== the lcpp-shape variant (round 3: their whole thread mapping, verbatim) =====
// llama.cpp's kernel_mul_mv_mxfp4_f32 monomorphized (NR0=2, NSG=2) at our MoE bindings:
// 16 blocks in flight x 2 threads/block (ix=tiisg/2, it=tiisg%2), each thread owns 8 qs bytes;
// 4 float4 x prefetch (yl[4]) reused across both rows; float4 accumulators folded to one
// horizontal add per row; 32-slot tgmem LUT fill (their branch-free shape — reads touch 0..15).
// vs the tgv arm: block stride 16 not 4 (6 loop trips at nb=90, not 23) and each weight byte
// loads ONCE (tgv's 8-thread/block map loads every uint twice, one per nibble half).
class LabMoeGemvMx4L {
    @ssbo @binding = 0 mxu : array<uint>
    @ssbo @binding = 1 mxe : array<uint8>
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>
    @uniform @binding = 7 eblk : uint
    @uniform @binding = 8 xs : uint
    @uniform @binding = 9 ss : uint
    @uniform @binding = 10 bxs : uint
    @uniform @binding = 11 bys : uint
    @ssbo @binding = 14 etab : array<float>   // [256] exact e8m0_half scales (host-built)
    @ssbo @binding = 15 vtab : array<float>   // [16] signed doubled-e2m1 values (host-built)
    @ssbo @binding = 16 mxb : array<uint8>    // nibble plane, byte view (lcpp's q2[0..7] loads)
    @workgroup vt : float[32]

    [metal_kernel(name="lab_moe_mx4_l_msl")]
    def lab_moe_mx4_l {
        let lane = gl_SubgroupInvocationID
        vt[lane] = vtab[lane % 16u]   // both sgs write the same 32 slots (benign, lcpp shape)
        barrier()
        let ix = lane / 2u   // 0..15
        let it = lane % 2u
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
                let q2 = blk * 16u + it * 8u
                let b0 = uint(mxb[q2])
                let b1 = uint(mxb[q2 + 1u])
                let b2 = uint(mxb[q2 + 2u])
                let b3 = uint(mxb[q2 + 3u])
                let b4 = uint(mxb[q2 + 4u])
                let b5 = uint(mxb[q2 + 5u])
                let b6 = uint(mxb[q2 + 6u])
                let b7 = uint(mxb[q2 + 7u])
                let acc1 = yl0 * float4(vt[b0 & 15u], vt[b1 & 15u], vt[b2 & 15u], vt[b3 & 15u])
                let acc2 = yl1 * float4(vt[b0 >> 4u], vt[b1 >> 4u], vt[b2 >> 4u], vt[b3 >> 4u])
                let acc3 = yl2 * float4(vt[b4 & 15u], vt[b5 & 15u], vt[b6 & 15u], vt[b7 & 15u])
                let acc4 = yl3 * float4(vt[b4 >> 4u], vt[b5 >> 4u], vt[b6 >> 4u], vt[b7 >> 4u])
                let am = (acc1 + acc3) + (acc2 + acc4)
                sumf[r] += etab[uint(mxe[blk])] * ((am.x + am.y) + (am.z + am.w))
            }
            ib += 16u
        }
        for [unroll_full] (r in range(2)) {
            let sr = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[y0 + first_row + uint(r)] = sr
            }
        }
    }
}

// ===== lcpp shape + wide weight loads =====
// Same mapping, but the thread's 8 qs bytes arrive as ONE uint2 (8B) instead of 8 byte loads —
// nibbles shift-extracted. The k4 sibling's 321 streams 16B uint4; this is the mx4 equivalent
// (2 threads/block caps a thread's share of the 16B block at 8B).
class LabMoeGemvMx4W {
    @ssbo @binding = 0 mxu : array<uint>
    @ssbo @binding = 1 mxe : array<uint8>
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>
    @uniform @binding = 7 eblk : uint
    @uniform @binding = 8 xs : uint
    @uniform @binding = 9 ss : uint
    @uniform @binding = 10 bxs : uint
    @uniform @binding = 11 bys : uint
    @ssbo @binding = 14 etab : array<float>   // [256] exact e8m0_half scales (host-built)
    @ssbo @binding = 15 vtab : array<float>   // [16] signed doubled-e2m1 values (host-built)
    @ssbo @binding = 17 mxu2 : array<uint2>   // nibble plane, uint2 view (8B per row-visit)
    @workgroup vt : float[32]

    [metal_kernel(name="lab_moe_mx4_w_msl")]
    def lab_moe_mx4_w {
        let lane = gl_SubgroupInvocationID
        vt[lane] = vtab[lane % 16u]
        barrier()
        let ix = lane / 2u   // 0..15
        let it = lane % 2u
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
                sumf[r] += etab[uint(mxe[blk])] * ((am.x + am.y) + (am.z + am.w))
            }
            ib += 16u
        }
        for [unroll_full] (r in range(2)) {
            let sr = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[y0 + first_row + uint(r)] = sr
            }
        }
    }
}

// ===== lcpp shape + wide loads + COMPUTED e8m0 =====
// The lcppw disasm's remaining serial chain: mxe byte load -> wait -> etab device load at that
// index, twice per iteration. lcpp computes the scale in ALU (as_type<float>(e << 23)) — same
// here via uint_bits_to_float (msl_emit bitcast), bit-exact vs the etab formula for all 256
// bytes (e<2 subnormal arm included). The mxe byte load stays; its dependent load is gone.
class LabMoeGemvMx4E {
    @ssbo @binding = 0 mxu : array<uint>
    @ssbo @binding = 1 mxe : array<uint8>
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>
    @uniform @binding = 7 eblk : uint
    @uniform @binding = 8 xs : uint
    @uniform @binding = 9 ss : uint
    @uniform @binding = 10 bxs : uint
    @uniform @binding = 11 bys : uint
    @ssbo @binding = 15 vtab : array<float>   // [16] signed doubled-e2m1 values (host-built)
    @ssbo @binding = 17 mxu2 : array<uint2>   // nibble plane, uint2 view (8B per row-visit)
    @workgroup vt : float[32]

    [metal_kernel(name="lab_moe_mx4_e_msl")]
    def lab_moe_mx4_e {
        let lane = gl_SubgroupInvocationID
        vt[lane] = vtab[lane % 16u]
        barrier()
        let ix = lane / 2u   // 0..15
        let it = lane % 2u
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
                y[y0 + first_row + uint(r)] = sr
            }
        }
    }
}

// the production host tables, bit-exact (dasllama_metal_kernels.das mx4 install)
def private build_mx4_tabs(var etab : array<float>; var vtab : array<float>) {
    etab |> resize(256)
    for (ei in range(256)) {
        let bits = ei < 2 ? (0x00200000u << uint(ei)) : (uint(ei - 1) << 23u)
        etab[ei] = unsafe(reinterpret<float>(bits))
    }
    vtab |> resize(16)
    let mags = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0)
    for (vi in range(16)) {
        vtab[vi] = vi < 8 ? mags[vi] : -mags[vi - 8]
    }
}

// CPU ref mirroring the kernel's u32 nibble extraction exactly (two quads share one u32,
// low/high nibble halves) — exact under the lab fill contract (pow2 scales, small-int x)
def private ref_mx4_row(gq, ge : array<uint8>; gx, etab, vtab : array<float>; plane_blk : int64; row, ndim : int64) : float {
    let nb = ndim / 32l
    var acc = 0.0
    for (ib in range64(nb)) {
        let blk = plane_blk + row * nb + ib
        let scale = etab[int(ge[blk])]
        for (q in range64(8l)) {
            let b0 = blk * 16l + 4l * (q % 4l)
            let u = uint(gq[b0]) | (uint(gq[b0 + 1l]) << 8u) | (uint(gq[b0 + 2l]) << 16u) | (uint(gq[b0 + 3l]) << 24u)
            let nsh = uint(q / 4l) * 4u
            for (c in range64(4l)) {
                let nib = int((u >> (uint(c) * 8u + nsh)) & 15u)
                acc += scale * vtab[nib] * gx[ib * 32l + q * 4l + c]
            }
        }
    }
    return acc
}

// production binding table (MetalMoeGemvMx4): single-stream form (ss/bxs/bys = 0), no bias
def private enc_gmx4(gbufs; pso; enc; l : int64) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, gbufs[0], 0ul, 0)                                  // mxu (uint view)
    metal_set_buffer(enc, gbufs[1], 0ul, 1)                                  // mxe
    metal_set_buffer(enc, gbufs[2], 0ul, 2)                                  // x (float4 view)
    metal_set_buffer(enc, gbufs[3], uint64(l * GK * GNFE * 4l), 3)           // y, per-layer block
    metal_set_buffer(enc, gbufs[4], 0ul, 4)                                  // ndim
    metal_set_buffer(enc, gbufs[5], 0ul, 5)                                  // ddim
    metal_set_buffer(enc, gbufs[6], uint64(l * GK * 4l), 6)                  // sel row
    metal_set_buffer(enc, gbufs[7], 0ul, 7)                                  // eblk
    metal_set_buffer(enc, gbufs[8], 0ul, 8)                                  // xs = 0
    metal_set_buffer(enc, gbufs[8], 0ul, 9)                                  // ss = 0
    metal_set_buffer(enc, gbufs[8], 0ul, 10)                                 // bxs = 0
    metal_set_buffer(enc, gbufs[8], 0ul, 11)                                 // bys = 0
    metal_set_buffer(enc, gbufs[2], 0ul, 12)                                 // wb dummy (hasb = 0)
    metal_set_buffer(enc, gbufs[8], 0ul, 13)                                 // hasb = 0
    metal_set_buffer(enc, gbufs[9], 0ul, 14)                                 // etab
    metal_set_buffer(enc, gbufs[10], 0ul, 15)                                // vtab
    metal_set_buffer(enc, gbufs[0], 0ul, 16)                                 // mxb (byte view of mxu)
    metal_set_buffer(enc, gbufs[0], 0ul, 17)                                 // mxu2 (uint2 view of mxu)
    metal_set_threadgroup_memory_length(enc, 128ul, 0)                       // LUT tgmem, 32-float worst case (@workgroup = dynamic shmem)
    metal_dispatch_threadgroups(enc, uint3(uint((GNFE + 3l) / 4l), uint(GK), 1u), uint3(64u, 1u, 1u))
}

def private check_gmx4(queue; gbufs : array<MetalBuffer?>; cpso; name : string;
                       gq, ge : array<uint8>; gx, etab, vtab : array<float>; gsel : array<uint>) {
    let t = time_arm(queue, false) $(enc : MetalComputeEncoder?) {
        enc_gmx4(gbufs, cpso, enc, 0l)
    }
    var worst = 0.0
    unsafe {
        var y = reinterpret<float const?>(metal_buffer_contents(gbufs[3]))
        for (s in range64(GK)) {
            for (row in [0l, 1l, 1234l, GNFE - 1l]) {
                let e = int64(gsel[s])
                let want_v = ref_mx4_row(gq, ge, gx, etab, vtab, e * GEBLK, row, GDIM)
                worst = max(worst, abs(y[s * GNFE + row] - want_v) / max(1.0, abs(want_v)))
            }
        }
    }
    if (worst > 1e-4) {
        panic("{name} correctness: worst rel diff {worst}")
    }
    print("{name} correctness: worst rel {worst}\n")
}

// applegpu disasm rail: DASMETAL_LAB_DUMP_MSL=<dir> writes the gmx4 kernel sources — checked at
// the top of main (no device, no fills: the dump needs only the compile-time MSL globals)
def private dump_gmx4_msl : bool {
    if (!has_env_variable("DASMETAL_LAB_DUMP_MSL")) {
        return false
    }
    let dd = get_env_variable("DASMETAL_LAB_DUMP_MSL")
    fwrite("{dd}/gmx4_prod.metal", metal_moe_gemv_mx4_msl)
    fwrite("{dd}/gmx4_arith.metal", lab_moe_mx4_a_msl)
    fwrite("{dd}/gmx4_tgv.metal", lab_moe_mx4_t_msl)
    fwrite("{dd}/gmx4_lcpp.metal", lab_moe_mx4_l_msl)
    fwrite("{dd}/gmx4_lcppw.metal", lab_moe_mx4_w_msl)
    fwrite("{dd}/gmx4_lcppe.metal", lab_moe_mx4_e_msl)
    fwrite("{dd}/gmm4_prod.metal", metal_moe_mulmm_mx4_msl)
    fwrite("{dd}/gmm4_bf32.metal", lab_moe_mm_mx4_bf32_msl)
    fwrite("{dd}/gmm4_lcpp.metal", lab_moe_mm_mx4_lcpp_msl)
    print("gmx4+gmm4 MSL dumped to {dd}\n")
    return true
}

def private run_gmx4_lab(dev; queue; rounds : int; passes : int64) {
    var pso = compile_or_die(dev, metal_moe_gemv_mx4_msl, metal_moe_gemv_mx4_msl_entry, metal_moe_gemv_mx4_msl_fastmath)
    var pso_a = compile_or_die(dev, lab_moe_mx4_a_msl, lab_moe_mx4_a_msl_entry, lab_moe_mx4_a_msl_fastmath)
    var pso_t = compile_or_die(dev, lab_moe_mx4_t_msl, lab_moe_mx4_t_msl_entry, lab_moe_mx4_t_msl_fastmath)
    var pso_l = compile_or_die(dev, lab_moe_mx4_l_msl, lab_moe_mx4_l_msl_entry, lab_moe_mx4_l_msl_fastmath)
    var pso_w = compile_or_die(dev, lab_moe_mx4_w_msl, lab_moe_mx4_w_msl_entry, lab_moe_mx4_w_msl_fastmath)
    var pso_e = compile_or_die(dev, lab_moe_mx4_e_msl, lab_moe_mx4_e_msl_entry, lab_moe_mx4_e_msl_fastmath)
    var etab : array<float>
    var vtab : array<float>
    build_mx4_tabs(etab, vtab)
    let t_fill = ref_time_ticks()
    var gq : array<uint8>
    gq |> resize(int(GE * GEBLK * 16l))
    for (i in range(length(gq))) {
        gq[i] = uint8((i * 37) % 251)
    }
    var ge : array<uint8>
    ge |> resize(int(GE * GEBLK))
    for (i in range(length(ge))) {
        ge[i] = uint8(120 + (i * 13) % 15)   // pow2 scales near 1 — exact-arithmetic contract
    }
    var gx : array<float>
    gx |> resize(int(GDIM))
    for (i in range64(GDIM)) {
        gx[i] = float(int((i * 5l) % 13l) - 6)
    }
    var gsel : array<uint>
    gsel |> resize(int(GLAYERS * GK))
    for (l in range64(GLAYERS)) {
        for (s in range64(GK)) {
            gsel[l * GK + s] = uint((l * 13l + s * 7l + 3l) % GE)
        }
    }
    var gselb : array<uint8>
    gselb |> resize(length(gsel) * 4)
    unsafe {
        memcpy(addr < void? >(gselb[0]), addr < void? >(gsel[0]), length(gsel) * 4)
    }
    print("gmx4 host fill: {get_time_usec(t_fill) / 1000} ms ({length(gq) / 1048576} + {length(ge) / 1048576} MB planes)\n")

    var gbufs : array<MetalBuffer?>
    gbufs |> push(upload_bytes(dev, gq))                                        // 0 mxu
    gbufs |> push(upload_bytes(dev, ge))                                        // 1 mxe
    gbufs |> push(upload_floats(dev, gx))                                       // 2 x
    gbufs |> push(metal_new_buffer_untracked(dev, uint64(GLAYERS * GK * GNFE * 4l)))  // 3 y
    gbufs |> push(upload_u32v(dev, uint(GDIM)))                                 // 4 ndim
    gbufs |> push(upload_u32v(dev, uint(GNFE)))                                 // 5 ddim
    gbufs |> push(upload_bytes(dev, gselb))                                     // 6 sel
    gbufs |> push(upload_u32v(dev, uint(GEBLK)))                                // 7 eblk
    gbufs |> push(upload_u32v(dev, 0u))                                         // 8 zero
    gbufs |> push(upload_floats(dev, etab))                                     // 9 etab
    gbufs |> push(upload_floats(dev, vtab))                                     // 10 vtab

    // correctness: layer 0, all 4 slots x 4 rows vs the CPU ref (exact under the fill contract)
    check_gmx4(queue, gbufs, pso, "gmx4", gq, ge, gx, etab, vtab, gsel)
    check_gmx4(queue, gbufs, pso_a, "gmx4_arith", gq, ge, gx, etab, vtab, gsel)
    check_gmx4(queue, gbufs, pso_t, "gmx4_tgv", gq, ge, gx, etab, vtab, gsel)
    check_gmx4(queue, gbufs, pso_l, "gmx4_lcpp", gq, ge, gx, etab, vtab, gsel)
    check_gmx4(queue, gbufs, pso_w, "gmx4_lcppw", gq, ge, gx, etab, vtab, gsel)
    check_gmx4(queue, gbufs, pso_e, "gmx4_lcppe", gq, ge, gx, etab, vtab, gsel)

    // the discriminator: pipe (kernel ceiling) vs serial (per-dispatch barrier tax), and the
    // challengers on the same pipe schedule
    let bytes_pass = double(GLAYERS * GK * GEBLK * 17l)
    var psov <- [pso, pso_a, pso_t, pso_l, pso_w, pso_e]
    var garms <- [("gmx4_pipe", 0, false), ("gmx4_serial", 0, true), ("gmx4_arith", 1, false), ("gmx4_tgv", 2, false), ("gmx4_lcpp", 3, false), ("gmx4_lcppw", 4, false), ("gmx4_lcppe", 5, false)]
    var gbest : table<string; double>
    for (a in garms) {
        gbest[a._0] = 1e18lf
    }
    for (_rd in range(rounds)) {
        for (a in garms) {
            if (!want("DASMETAL_LAB_ARMS", a._0)) {
                continue
            }
            let kind = a._1
            let barrier = a._2
            let us = time_arm(queue, true) $(enc : MetalComputeEncoder?) {
                for (_p in range64(passes)) {
                    for (l in range64(GLAYERS)) {
                        enc_gmx4(gbufs, psov[kind], enc, l)
                        if (barrier) {
                            metal_memory_barrier(enc)
                        }
                    }
                }
            }
            let per = us / double(passes)
            if (per < gbest[a._0]) {
                gbest[a._0] = per
            }
        }
    }
    print("--- gmx4 (gpt-oss shapes: E={GE} nfe={GNFE} dim={GDIM} k={GK}, {int(bytes_pass) / 1048576} MB routed/pass) ---\n")
    for (a in garms) {
        if (!want("DASMETAL_LAB_ARMS", a._0)) {
            continue
        }
        let per = gbest[a._0]
        print("  {a._0}: {per} us/pass  {per / double(GLAYERS)} us/layer  {bytes_pass / per / 1000.0lf} wGB/s\n")
    }

    delete gq
    delete ge
    delete gx
    delete gsel
    delete gselb
    delete etab
    delete vtab
    delete garms
    delete gbest
    for (b in gbufs) {
        metal_release(b)
    }
    gbufs |> clear()   // handles are metal_release-owned — delete on a ptr array would free pointees
    unsafe {
        delete gbufs
    }
    psov |> clear()    // aliases of the named psos below
    unsafe {
        delete psov
    }
    metal_release(pso)
    metal_release(pso_a)
    metal_release(pso_t)
    metal_release(pso_l)
    metal_release(pso_w)
    metal_release(pso_e)
}

// ===== gmm4: gathered mx4 MUL_MM section (gptoss pp512 shapes — the Wave-1 pp chase) =====
// The pp knockout chase convicted the gathered expert mul_mms: 397ms of the 516ms pp512 GPU
// window (77%) while lcpp's whole wall is ~496ms. Same discriminator as gmx4: price the
// PRODUCTION MetalMoeMulMmMx4 at the exact shapes (E=32 nk=4 ntok=512, 2880x2880 planes, padded
// 32-row buckets) on pipe vs serial, against (a) lcpp's kernel_mul_mm_id classic-simdgroup path
// monomorphized VERBATIM for mxfp4 (their 17-byte interleaved blocks, byte loads, f32 B tile,
// mixed float x half MMAs) and (b) the production kernel with the one structural delta lcpp
// holds: an f32 B tile (no f32->f16 activation convert).

let GNTOK = 512l                  // pp512 token rows
let GROWS = GNTOK * GK            // routed (token, slot) rows

// production body, B tile widened to f32 (tg_store_float4 + mixed float x half MMAs)
class LabMoeMmMx4Bf32 {
    @ssbo @binding = 0 mxq4 : array<uint4>
    @ssbo @binding = 1 mxe : array<uint8>
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>
    @ssbo @binding = 8 bkt : array<uint>
    @uniform @binding = 9 eblk : uint
    @uniform @binding = 10 nk : uint
    @uniform @binding = 11 gather : uint
    @ssbo @binding = 13 vtab : array<float>
    @workgroup ta : float16[2048]
    @workgroup tb : float[1024]
    @workgroup vt : float[32]

    [metal_kernel(name="lab_moe_mm_mx4_bf32_msl")]
    def lab_moe_mm_mx4_bf32 {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {
            return
        }
        let rbase = basep[e]
        let lid = gl_LocalInvocationID.x
        if (lid < 32u) {
            vt[lid] = vtab[lid % 16u]
        }
        barrier()
        let sg = gl_SubgroupID
        let nBase = gl_WorkGroupID.y * 64u
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_float8x8[2]
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
        let eb = e * eblk + (nBase + lr0) * nkb
        var srcrow = rbase + mBase + lr1
        if (gather != 0u) {
            srcrow = bkt[rbase + min(mBase + lr1, ce - 1u)] / nk
        }
        var xcur = unsafe(addr(xf[srcrow * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let blk = eb + kb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let sc = uint(mxe[blk])
            let d = uint_bits_to_float(sc < 2u ? (0x00200000u << sc) : ((sc - 1u) << 23u))   // exact e8m0_half, denormal e < 2 included
            let u4 = mxq4[blk]
            for [unroll_full] (w in range(4)) {
                let uw = w == 0 ? u4.x : (w == 1 ? u4.y : (w == 2 ? u4.z : u4.w))
                for [unroll_full] (c in range(4)) {
                    let nby = (uw >> (uint(c) * 8u)) & 255u
                    va[w * 4 + c] = float16(vt[il0 == 0u ? (nby & 15u) : (nby >> 4u)] * d)
                }
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_float4(tb, ibB, b0)
            tg_store_float4(tb, ibB + 4u, b1)
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
        let row0 = rbase + mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// lcpp kernel_mul_mm_id monomorphized (mxfp4, classic simdgroup branch — the M1 path):
// 17-byte interleaved block_mxfp4 A plane read with their 16 byte-loads through the halved LUT,
// half A tile in their swizzled-transposed layout, f32 B rows through the per-expert id list
// (their map0 output), mixed float x half MMAs. Deviations, both noted at the measurement: the
// LUT rides tgmem (their constant address space has no msl_emit spelling — same on-chip class),
// and the epilogue stores C tiles STRAIGHT to the padded bucket panel (their tgmem-restage +
// ids-scatter epilogue is the production reduce's job in our graph; the K-loop engine races
// here). ndim % 64 == 0 at these shapes, so their nr0 partial-tile clamp drops out.
class LabMoeMmMx4Lcpp {
    @ssbo @binding = 0 ab : array<uint8>    // interleaved block_mxfp4 plane: e byte + 16 nibble bytes
    @ssbo @binding = 1 xr : array<float4>   // token rows [ntok x kdim] f32
    @ssbo @binding = 2 tpe : array<uint>    // [E] per-expert routed-row counts (their htpe)
    @ssbo @binding = 3 ids : array<int>     // [E x ntok] per-expert id lists (their hids; id = tok * nk + slot)
    @ssbo @binding = 4 y : array<float>     // padded bucket panel (prod layout — shared CPU check)
    @ssbo @binding = 5 basep : array<uint>  // padded panel bases
    @uniform @binding = 6 kdim : uint       // ne00
    @uniform @binding = 7 ndim : uint       // ne0 (weight rows)
    @uniform @binding = 8 ntok : uint       // ne21
    @uniform @binding = 9 nk : uint         // ne20
    @ssbo @binding = 10 vtab : array<float> // [16] their halved LUT (kvalues_mxfp4_f)
    @workgroup sa : float16[2048]           // A tile: 64 rows x 32 k, 8x8-block swizzled
    @workgroup sb : float[1024]             // B tile: 32 rows x 32 k, 8x8-block swizzled
    @workgroup vt : float[16]

    [metal_kernel(name="lab_moe_mm_mx4_lcpp_msl")]
    def lab_moe_mm_mx4_lcpp {
        let lid = gl_LocalInvocationID.x
        if (lid < 16u) {
            vt[lid] = vtab[lid]
        }
        barrier()
        let im = gl_WorkGroupID.z
        let r1 = gl_WorkGroupID.x * 32u
        let neh1 = tpe[im]
        if (r1 >= neh1) {
            return
        }
        let r0 = gl_WorkGroupID.y * 64u
        let nr1 = min(neh1 - r1, 32u)
        let lr0 = lid / 2u
        let lr1 = min(lid / 4u, nr1 - 1u)
        let il0 = lid % 2u
        let shr = il0 * 4u
        let id = uint(ids[im * ntok + r1 + lr1])
        let idt = id / nk
        let nkb = kdim / 32u
        let ablk = (im * ndim + r0 + lr0) * nkb
        let iy = (lid % 4u) * 8u
        var xcur = idt * (kdim / 4u) + iy / 4u
        let sg = gl_SubgroupID
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_float8x8[2]
        var mc : simdgroup_float8x8[8]
        var va : float16[16]
        // their A/B swizzles, hoisted: A ib = 8*(2*il0 + i/8) + (lid/2)/8, addr 64*ib + 8*(i%8) + (lid/2)%8;
        // B ib = 4*(lid%4) + (lid/4)/8, addr 64*ib + 8*((lid/4)%8)
        let tA0 = (il0 * 2u * 8u + lr0 / 8u) * 64u + lr0 % 8u
        let ibB = ((lid % 4u) * 4u + (lid / 4u) / 8u) * 64u + ((lid / 4u) % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        var kb = 0u
        while (kb < nkb) {
            // dequantize_mxfp4(x, il, temp_a): d = e8m0_to_fp32(e); 16 byte loads through the LUT
            let abase = (ablk + kb) * 17u
            let sc = uint(ab[abase])
            let d = uint_bits_to_float(sc == 0u ? 0x00400000u : (sc << 23u))
            for [unroll_full] (i in range(16)) {
                let q = uint(ab[abase + 1u + uint(i)])
                va[i] = float16(d * vt[(q >> shr) & 15u])
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                sa[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            let b0 = xr[xcur]
            let b1 = xr[xcur + 1u]
            tg_store_float4(sb, ibB, b0)
            tg_store_float4(sb, ibB + 4u, b1)
            barrier()
            var acur = unsafe(addr(sa[aB]))
            var bcur = unsafe(addr(sb[bB]))
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
            xcur += 8u
            kb++
        }
        let row0 = basep[im] + r1 + (sg / 2u) * 16u
        let col0 = r0 + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// gmm4 buffer slots
let GM_GQ = 0       // nibble plane (prod layout, uint4 view in-kernel)
let GM_GE = 1       // scale bytes (prod layout)
let GM_AB = 2       // interleaved block_mxfp4 plane (lcpp layout)
let GM_X = 3        // token rows [GNTOK x GDIM] f32
let GM_CNT = 4      // per-expert routed-row counts
let GM_BASE = 5     // padded panel bases
let GM_BKT = 6      // padded bucket -> (tok*nk + slot)
let GM_IDS = 7      // lcpp hids: [E x GNTOK] id lists
let GM_Y1 = 8       // gate dst panel
let GM_Y2 = 9       // up dst panel
let GM_UK = 10      // kdim = GDIM
let GM_UN = 11      // ndim = GNFE
let GM_UEBLK = 12
let GM_UNK = 13
let GM_UG1 = 14     // gather = 1
let GM_UNTOK = 15
let GM_VT = 16      // prod doubled-e2m1 LUT
let GM_LVT = 17     // lcpp halved LUT
let GM_U0 = 18      // hasb = 0 (lab arms run the prod kernel without the bias fold)

def private enc_gmm4_prod(gb; pso; tgmem : uint64; enc; yslot : int) {
    metal_set_pipeline(enc, pso)
    metal_set_threadgroup_memory_length(enc, tgmem, 0)
    metal_set_buffer(enc, gb[GM_GQ], 0ul, 0)
    metal_set_buffer(enc, gb[GM_GE], 0ul, 1)
    metal_set_buffer(enc, gb[GM_X], 0ul, 2)
    metal_set_buffer(enc, gb[yslot], 0ul, 3)
    metal_set_buffer(enc, gb[GM_UK], 0ul, 4)
    metal_set_buffer(enc, gb[GM_UN], 0ul, 5)
    metal_set_buffer(enc, gb[GM_CNT], 0ul, 6)
    metal_set_buffer(enc, gb[GM_BASE], 0ul, 7)
    metal_set_buffer(enc, gb[GM_BKT], 0ul, 8)
    metal_set_buffer(enc, gb[GM_UEBLK], 0ul, 9)
    metal_set_buffer(enc, gb[GM_UNK], 0ul, 10)
    metal_set_buffer(enc, gb[GM_UG1], 0ul, 11)
    metal_set_buffer(enc, gb[GM_VT], 0ul, 12)   // wb slot — never read at hasb=0, any float buffer satisfies binding validation
    metal_set_buffer(enc, gb[GM_VT], 0ul, 13)
    metal_set_buffer(enc, gb[GM_U0], 0ul, 14)
    metal_dispatch_threadgroups(enc, uint3(uint((GNTOK + 31l) / 32l), uint(GNFE / 64l), uint(GE)), uint3(128u, 1u, 1u))
}

def private enc_gmm4_lcpp(gb; pso; tgmem : uint64; enc; yslot : int) {
    metal_set_pipeline(enc, pso)
    metal_set_threadgroup_memory_length(enc, tgmem, 0)
    metal_set_buffer(enc, gb[GM_AB], 0ul, 0)
    metal_set_buffer(enc, gb[GM_X], 0ul, 1)
    metal_set_buffer(enc, gb[GM_CNT], 0ul, 2)
    metal_set_buffer(enc, gb[GM_IDS], 0ul, 3)
    metal_set_buffer(enc, gb[yslot], 0ul, 4)
    metal_set_buffer(enc, gb[GM_BASE], 0ul, 5)
    metal_set_buffer(enc, gb[GM_UK], 0ul, 6)
    metal_set_buffer(enc, gb[GM_UN], 0ul, 7)
    metal_set_buffer(enc, gb[GM_UNTOK], 0ul, 8)
    metal_set_buffer(enc, gb[GM_UNK], 0ul, 9)
    metal_set_buffer(enc, gb[GM_LVT], 0ul, 10)
    metal_dispatch_threadgroups(enc, uint3(uint((GNTOK + 31l) / 32l), uint(GNFE / 64l), uint(GE)), uint3(128u, 1u, 1u))
}

// exact under the fill contract (pow2 scales, small-int x -> f16-exact A and B, f32-exact sums):
// sample rows/cols per expert against the gathered CPU ref
def private check_gmm4(queue; gb : array<MetalBuffer?>; pso; tgmem : uint64; name : string; kind : int;
                       gq, ge : array<uint8>; gx, etab, vtab : array<float>;
                       cnt, basep, bkt : array<uint>) {
    let t = time_arm(queue, false) $(enc : MetalComputeEncoder?) {
        if (kind == 0) {
            enc_gmm4_prod(gb, pso, tgmem, enc, GM_Y1)
        } else {
            enc_gmm4_lcpp(gb, pso, tgmem, enc, GM_Y1)
        }
    }
    var xrow : array<float>
    xrow |> resize(int(GDIM))
    var worst = 0.0
    unsafe {
        var y = reinterpret<float const?>(metal_buffer_contents(gb[GM_Y1]))
        for (e in [0l, 5l, 31l]) {
            for (r in [0l, int64(cnt[e]) - 1l]) {
                let tok = int64(bkt[int64(basep[e]) + r] / uint(GK))
                for (i in range64(GDIM)) {
                    xrow[i] = gx[tok * GDIM + i]
                }
                for (col in [0l, 1l, 1234l, GNFE - 1l]) {
                    let want_v = ref_mx4_row(gq, ge, xrow, etab, vtab, e * GEBLK, col, GDIM)
                    let got = y[(int64(basep[e]) + r) * GNFE + col]
                    worst = max(worst, abs(got - want_v) / max(1.0, abs(want_v)))
                }
            }
        }
    }
    delete xrow
    if (worst > 1e-4) {
        panic("{name} correctness: worst rel diff {worst}")
    }
    print("{name} correctness: worst rel {worst}  (single mm {t / 1000.0lf} ms)\n")
}

def private run_gmm4_lab(dev; queue; rounds : int; passes : int64) {
    var pso_p = compile_or_die(dev, metal_moe_mulmm_mx4_msl, metal_moe_mulmm_mx4_msl_entry, metal_moe_mulmm_mx4_msl_fastmath)
    var pso_b = compile_or_die(dev, lab_moe_mm_mx4_bf32_msl, lab_moe_mm_mx4_bf32_msl_entry, lab_moe_mm_mx4_bf32_msl_fastmath)
    var pso_l = compile_or_die(dev, lab_moe_mm_mx4_lcpp_msl, lab_moe_mm_mx4_lcpp_msl_entry, lab_moe_mm_mx4_lcpp_msl_fastmath)
    var etab : array<float>
    var vtab : array<float>
    build_mx4_tabs(etab, vtab)
    var lvt : array<float>
    lvt |> resize(16)
    for (i in range(16)) {
        lvt[i] = vtab[i] * 0.5   // their halved LUT x their full e8m0 == our doubled LUT x e8m0_half
    }
    let t_fill = ref_time_ticks()
    var gq : array<uint8>
    gq |> resize(int(GE * GEBLK * 16l))
    for (i in range(length(gq))) {
        gq[i] = uint8((i * 37) % 251)
    }
    var ge : array<uint8>
    ge |> resize(int(GE * GEBLK))
    for (i in range(length(ge))) {
        ge[i] = uint8(120 + (i * 13) % 15)   // pow2 scales near 1 — exact-arithmetic contract
    }
    var ab : array<uint8>
    ab |> resize(int(GE * GEBLK * 17l))
    for (blk in range64(GE * GEBLK)) {
        ab[blk * 17l] = ge[blk]
        for (i in range64(16l)) {
            ab[blk * 17l + 1l + i] = gq[blk * 16l + i]
        }
    }
    var gx : array<float>
    gx |> resize(int(GNTOK * GDIM))
    for (i in range(length(gx))) {
        gx[i] = float((i * 5) % 13 - 6)
    }
    // routing: 4 distinct experts per token (odd stride mod 32), hash jitter on the base for
    // realistic per-expert count spread (the padded-tile waste production pays)
    var cnt : array<uint>
    cnt |> resize(int(GE))
    var selE : array<int>
    selE |> resize(int(GROWS))
    for (tk in range64(GNTOK)) {
        let base = int64((uint(int(tk)) * 2654435761u) >> 27u)   // multiplicative hash — realistic count spread
        for (s in range64(GK)) {
            let e = int((base + s * 9l) % GE)
            selE[tk * GK + s] = e
            cnt[e]++
        }
    }
    var basep : array<uint>
    basep |> resize(int(GE))
    var ppad = 0l
    for (e in range64(GE)) {
        basep[e] = uint(ppad)
        ppad += (int64(cnt[e]) + 31l) / 32l * 32l
    }
    var bkt : array<uint>
    bkt |> resize(int(ppad))
    var hids : array<int>
    hids |> resize(int(GE * GNTOK))
    var fill : array<uint>
    fill |> resize(int(GE))
    for (tk in range64(GNTOK)) {
        for (s in range64(GK)) {
            let e = selE[tk * GK + s]
            let idv = uint(tk * GK + s)
            bkt[int64(basep[e]) + int64(fill[e])] = idv
            hids[int64(e) * GNTOK + int64(fill[e])] = int(idv)
            fill[e]++
        }
    }
    var hidsb : array<uint8>
    hidsb |> resize(length(hids) * 4)
    var cntb : array<uint8>
    cntb |> resize(length(cnt) * 4)
    var basepb : array<uint8>
    basepb |> resize(length(basep) * 4)
    var bktb : array<uint8>
    bktb |> resize(length(bkt) * 4)
    unsafe {
        memcpy(addr < void? >(hidsb[0]), addr < void? >(hids[0]), length(hids) * 4)
        memcpy(addr < void? >(cntb[0]), addr < void? >(cnt[0]), length(cnt) * 4)
        memcpy(addr < void? >(basepb[0]), addr < void? >(basep[0]), length(basep) * 4)
        memcpy(addr < void? >(bktb[0]), addr < void? >(bkt[0]), length(bkt) * 4)
    }
    var cmin = cnt[0]
    var cmax = cnt[0]
    for (e in range64(GE)) {
        cmin = min(cmin, cnt[e])
        cmax = max(cmax, cnt[e])
    }
    print("gmm4 host fill: {get_time_usec(t_fill) / 1000} ms ({length(gq) / 1048576} + {length(ab) / 1048576} MB planes; rows {GROWS} padded {ppad}, cnt {int(cmin)}..{int(cmax)})\n")

    var gb : array<MetalBuffer?>
    gb |> push(upload_bytes(dev, gq))                                          // GM_GQ
    gb |> push(upload_bytes(dev, ge))                                          // GM_GE
    gb |> push(upload_bytes(dev, ab))                                          // GM_AB
    gb |> push(upload_floats(dev, gx))                                         // GM_X
    gb |> push(upload_bytes(dev, cntb))                                        // GM_CNT
    gb |> push(upload_bytes(dev, basepb))                                      // GM_BASE
    gb |> push(upload_bytes(dev, bktb))                                        // GM_BKT
    gb |> push(upload_bytes(dev, hidsb))                                       // GM_IDS
    gb |> push(metal_new_buffer_untracked(dev, uint64(ppad * GNFE * 4l)))      // GM_Y1
    gb |> push(metal_new_buffer_untracked(dev, uint64(ppad * GNFE * 4l)))      // GM_Y2
    gb |> push(upload_u32v(dev, uint(GDIM)))                                   // GM_UK
    gb |> push(upload_u32v(dev, uint(GNFE)))                                   // GM_UN
    gb |> push(upload_u32v(dev, uint(GEBLK)))                                  // GM_UEBLK
    gb |> push(upload_u32v(dev, uint(GK)))                                     // GM_UNK
    gb |> push(upload_u32v(dev, 1u))                                           // GM_UG1
    gb |> push(upload_u32v(dev, uint(GNTOK)))                                  // GM_UNTOK
    gb |> push(upload_floats(dev, vtab))                                       // GM_VT
    gb |> push(upload_floats(dev, lvt))                                        // GM_LVT
    gb |> push(upload_u32v(dev, 0u))                                           // GM_U0

    check_gmm4(queue, gb, pso_p, metal_moe_mulmm_mx4_msl_tgmem, "gmm4_prod", 0, gq, ge, gx, etab, vtab, cnt, basep, bkt)
    check_gmm4(queue, gb, pso_b, lab_moe_mm_mx4_bf32_msl_tgmem, "gmm4_bf32", 0, gq, ge, gx, etab, vtab, cnt, basep, bkt)
    check_gmm4(queue, gb, pso_l, lab_moe_mm_mx4_lcpp_msl_tgmem, "gmm4_lcpp", 1, gq, ge, gx, etab, vtab, cnt, basep, bkt)

    // per pass: GLAYERS layers of the gathered 2880x2880 class that owns the pp window.
    // pipe = 2 mms/layer, concurrent, no barriers (ceiling); serial = barrier per dispatch;
    // graph = the production layer shape — (gate ∥ up), barrier (the act), down, barrier (the
    // reduce) — 3 mms/layer, the partial-overlap regime the in-graph 5.51 ms/mm lives in
    let mac_mm = double(GROWS * GNFE * GDIM)
    let wbytes_mm = double((ppad / 32l) * GNFE * (GDIM / 32l) * 17l)   // A plane re-read per row tile
    var garms <- [("gmm4_prod", 0, 0), ("gmm4_prod_serial", 0, 1), ("gmm4_prod_graph", 0, 2),
                  ("gmm4_bf32", 1, 0), ("gmm4_bf32_graph", 1, 2),
                  ("gmm4_lcpp", 2, 0), ("gmm4_lcpp_serial", 2, 1), ("gmm4_lcpp_graph", 2, 2)]
    var gbest : table<string; double>
    for (a in garms) {
        gbest[a._0] = 1e18lf
    }
    for (_rd in range(rounds)) {
        for (a in garms) {
            if (!want("DASMETAL_LAB_ARMS", a._0)) {
                continue
            }
            let kind = a._1
            let mode = a._2
            let nmm = mode == 2 ? 3 : 2
            let us = time_arm(queue, true) $(enc : MetalComputeEncoder?) {
                for (_p in range64(passes)) {
                    for (_l in range64(GLAYERS)) {
                        for (half in range(nmm)) {
                            if (kind == 2) {
                                enc_gmm4_lcpp(gb, pso_l, lab_moe_mm_mx4_lcpp_msl_tgmem, enc, half == 1 ? GM_Y2 : GM_Y1)
                            } elif (kind == 1) {
                                enc_gmm4_prod(gb, pso_b, lab_moe_mm_mx4_bf32_msl_tgmem, enc, half == 1 ? GM_Y2 : GM_Y1)
                            } else {
                                enc_gmm4_prod(gb, pso_p, metal_moe_mulmm_mx4_msl_tgmem, enc, half == 1 ? GM_Y2 : GM_Y1)
                            }
                            if (mode == 1 || (mode == 2 && half >= 1)) {
                                metal_memory_barrier(enc)   // graph: after (gate ∥ up) and after down
                            }
                        }
                    }
                }
            }
            let per = us / double(passes)
            if (per < gbest[a._0]) {
                gbest[a._0] = per
            }
        }
    }
    print("--- gmm4 (gptoss pp512 shapes: E={GE} nk={GK} ntok={GNTOK}, {int(GNFE)}x{int(GDIM)} planes, {int(mac_mm) / 1000000000} GMAC/mm; in-graph ref 5.51 ms/mm) ---\n")
    for (a in garms) {
        if (!want("DASMETAL_LAB_ARMS", a._0)) {
            continue
        }
        let per = gbest[a._0]
        let per_mm = per / double(GLAYERS * int64(a._2 == 2 ? 3 : 2))
        print("  {a._0}: {per / 1000.0lf} ms/pass  {per_mm / 1000.0lf} ms/mm  {mac_mm / per_mm / 1000.0lf} GMAC/s  {wbytes_mm / per_mm / 1000.0lf} wGB/s\n")
    }

    delete etab
    delete vtab
    delete lvt
    delete gq
    delete ge
    delete ab
    delete gx
    delete cnt
    delete selE
    delete basep
    delete bkt
    delete hids
    delete fill
    delete hidsb
    delete cntb
    delete basepb
    delete bktb
    delete garms
    delete gbest
    for (b in gb) {
        metal_release(b)
    }
    gb |> clear()   // handles are metal_release-owned — delete on a ptr array would free pointees
    unsafe {
        delete gb
    }
    metal_release(pso_p)
    metal_release(pso_b)
    metal_release(pso_l)
}

def private main_apple(dummy) {
    if (dump_gmx4_msl()) {
        return
    }
    let rounds = env_int("DASMETAL_LAB_ROUNDS", 3)
    let passes = int64(env_int("DASMETAL_LAB_PASSES", 4))
    var dev = metal_create_system_default_device()
    if (dev == null) {
        print("no Metal device\n")
        return
    }
    print("device={metal_device_name(dev)} rounds={rounds} passes={passes}\n")
    var queue = metal_new_command_queue(dev)
    var psos : array<MetalComputePipeline?>
    psos |> push(compile_or_die(dev, metal_moe_gemv_k4_msl, metal_moe_gemv_k4_msl_entry, metal_moe_gemv_k4_msl_fastmath))
    psos |> push(compile_or_die(dev, metal_moe_gemv_q51_msl, metal_moe_gemv_q51_msl_entry, metal_moe_gemv_q51_msl_fastmath))
    psos |> push(compile_or_die(dev, metal_geglu_msl, metal_geglu_msl_entry, metal_geglu_msl_fastmath))
    psos |> push(compile_or_die(dev, lab_moe_k4_pair_msl, lab_moe_k4_pair_msl_entry, lab_moe_k4_pair_msl_fastmath))
    psos |> push(compile_or_die(dev, lab_moe_k4_x4_msl, lab_moe_k4_x4_msl_entry, lab_moe_k4_x4_msl_fastmath))
    psos |> push(compile_or_die(dev, lab_moe_k4_r4_msl, lab_moe_k4_r4_msl_entry, lab_moe_k4_r4_msl_fastmath))
    psos |> push(compile_or_die(dev, lab_moe_q51_ic_msl, lab_moe_q51_ic_msl_entry, lab_moe_q51_ic_msl_fastmath))

    // host fills (exact-arithmetic contract)
    let t_fill = ref_time_ticks()
    var k4q : array<uint8>
    var k4c : array<uint8>
    fill_k4_stack(k4q, k4c, 2l * E * NFE, DIM, 7l)
    var q51q : array<uint8>
    var q51s : array<uint8>
    fill_q51_stack(q51q, q51s, E * DIM, NFE, 11l)
    var xf : array<float>
    xf |> resize(int(DIM))
    for (i in range64(DIM)) {
        xf[i] = float(int((i * 7l) % 15l) - 7)
    }
    print("host fill: {get_time_usec(t_fill) / 1000} ms (k4 {length(k4q) / 1048576} + {length(k4c) / 1048576} MB, q51 {length(q51q) / 1048576} + {length(q51s) / 1048576} MB)\n")

    // sel rows: scattered = 8 distinct experts spread across the plane, rotated per layer;
    // contig = 8 consecutive experts (the locality control). w3 rows ride the +E plane block.
    var sel : array<uint>
    sel |> resize(int(4l * LAYERS * 8l))
    for (l in range64(LAYERS)) {
        for (s in range64(8l)) {
            let e = uint(s * 16l + (l * 7l + s * 3l) % 16l)
            sel[l * 8l + s] = e
            sel[LAYERS * 8l + l * 8l + s] = e + uint(E)
            sel[2l * LAYERS * 8l + l * 8l + s] = uint((l * 8l + s) % E)
            sel[3l * LAYERS * 8l + l * 8l + s] = uint((l * 8l + s) % E) + uint(E)
        }
    }
    var selb : array<uint8>
    selb |> resize(length(sel) * 4)
    unsafe {
        memcpy(addr < void? >(selb[0]), addr < void? >(sel[0]), length(sel) * 4)
    }

    var bufs : array<MetalBuffer?>
    bufs |> push(upload_bytes(dev, k4q))
    bufs |> push(upload_bytes(dev, k4c))
    bufs |> push(upload_bytes(dev, q51q))
    bufs |> push(upload_bytes(dev, q51s))
    bufs |> push(upload_floats(dev, xf))
    bufs |> push(metal_new_buffer_untracked(dev, uint64(LAYERS * 2l * KNFE * 4l)))
    bufs |> push(metal_new_buffer_untracked(dev, uint64(LAYERS * K * DIM * 4l)))
    bufs |> push(upload_bytes(dev, selb))
    bufs |> push(upload_u32v(dev, uint(DIM)))
    bufs |> push(upload_u32v(dev, uint(NFE)))
    bufs |> push(upload_u32v(dev, uint(ESB)))
    bufs |> push(upload_u32v(dev, uint(EBLK)))
    bufs |> push(upload_u32v(dev, 0u))
    bufs |> push(upload_u32v(dev, uint(NFE)))
    bufs |> push(upload_u32v(dev, uint(KNFE)))
    bufs |> push(upload_u32v(dev, uint(E)))

    // ===== correctness: standalone w1 (k4), w2 (q51), and the pair kernel vs CPU refs =====
    var t = time_arm(queue, false) $(enc : MetalComputeEncoder?) {
        enc_lab_w13(bufs, psos, enc, 0l, 0ul, false)
    }
    var worst = 0.0
    unsafe {
        var y = reinterpret<float const?>(metal_buffer_contents(bufs[RB_H12]))
        for (s in range64(8l)) {
            for (row in [0l, 1l, 350l, NFE - 1l]) {
                let e = int64(sel[s])
                let want_v = ref_k4_row(k4q, k4c, xf, e * ESB, row, DIM)
                worst = max(worst, abs(y[s * NFE + row] - want_v) / max(1.0, abs(want_v)))
            }
        }
    }
    if (worst > 1e-4) {
        panic("w1 k4 correctness: worst rel diff {worst}")
    }
    print("w1 k4 correctness: worst rel {worst}\n")

    t = time_arm(queue, false) $(enc : MetalComputeEncoder?) {
        enc_lab_pair(bufs, psos, enc, 0l, 0ul)
    }
    worst = 0.0
    unsafe {
        var y = reinterpret<float const?>(metal_buffer_contents(bufs[RB_H12]))
        for (s in range64(8l)) {
            for (row in [0l, 1l, 350l, NFE - 1l]) {
                let e1 = int64(sel[s])
                let w1v = ref_k4_row(k4q, k4c, xf, e1 * ESB, row, DIM)
                let w3v = ref_k4_row(k4q, k4c, xf, (e1 + E) * ESB, row, DIM)
                worst = max(worst, abs(y[s * NFE + row] - w1v) / max(1.0, abs(w1v)))
                worst = max(worst, abs(y[KNFE + s * NFE + row] - w3v) / max(1.0, abs(w3v)))
            }
        }
    }
    if (worst > 1e-4) {
        panic("pair k4 correctness: worst rel diff {worst}")
    }
    print("pair k4 correctness: worst rel {worst}\n")

    for (vv in [(RP_X4, 4l, "x4"), (RP_R4, 8l, "r4")]) {
        let pslot = vv._0
        let rtg = vv._1
        t = time_arm(queue, false) $(enc : MetalComputeEncoder?) {
            enc_lab_w13v(bufs, psos, enc, 0l, 0ul, false, pslot, rtg)
        }
        worst = 0.0
        unsafe {
            var y = reinterpret<float const?>(metal_buffer_contents(bufs[RB_H12]))
            for (s in range64(8l)) {
                for (row in [0l, 1l, 350l, NFE - 1l]) {
                    let e = int64(sel[s])
                    let want_v = ref_k4_row(k4q, k4c, xf, e * ESB, row, DIM)
                    worst = max(worst, abs(y[s * NFE + row] - want_v) / max(1.0, abs(want_v)))
                }
            }
        }
        if (worst > 1e-4) {
            panic("{vv._2} k4 correctness: worst rel diff {worst}")
        }
        print("{vv._2} k4 correctness: worst rel {worst}\n")
    }

    // w2 standalone: fill h12 layer 0 gate half with exact ints, then one q51 dispatch
    var h0 : array<float>
    h0 |> resize(int(KNFE))
    for (i in range64(KNFE)) {
        h0[i] = float(int((i * 11l) % 13l) - 6)
    }
    unsafe {
        memcpy(metal_buffer_contents(bufs[RB_H12]), addr < void? >(h0[0]), int(KNFE) * 4)
    }
    t = time_arm(queue, false) $(enc : MetalComputeEncoder?) {
        enc_lab_w2(bufs, psos, enc, 0l, 0ul)
    }
    worst = 0.0
    unsafe {
        var y = reinterpret<float const?>(metal_buffer_contents(bufs[RB_DN]))
        var hslot : array<float>
        hslot |> resize(int(NFE))
        for (s in range64(8l)) {
            for (i in range64(NFE)) {
                hslot[i] = h0[s * NFE + i]
            }
            for (row in [0l, 1l, 1500l, DIM - 1l]) {
                let e = int64(sel[s])
                let want_v = ref_q51_row(q51q, q51s, hslot, e * EBLK, row, NFE)
                worst = max(worst, abs(y[s * DIM + row] - want_v) / max(1.0, abs(want_v)))
            }
        }
        delete hslot
    }
    if (worst > 1e-4) {
        panic("w2 q51 correctness: worst rel diff {worst}")
    }
    print("w2 q51 correctness: worst rel {worst}\n")

    // the ic variant against the same refs (h0 still resident in the gate half)
    t = time_arm(queue, false) $(enc : MetalComputeEncoder?) {
        enc_lab_w2(bufs, psos, enc, 0l, 0ul, RP_Q51IC)
    }
    worst = 0.0
    unsafe {
        var y = reinterpret<float const?>(metal_buffer_contents(bufs[RB_DN]))
        var hslot : array<float>
        hslot |> resize(int(NFE))
        for (s in range64(8l)) {
            for (i in range64(NFE)) {
                hslot[i] = h0[s * NFE + i]
            }
            for (row in [0l, 1l, 1500l, DIM - 1l]) {
                let e = int64(sel[s])
                let want_v = ref_q51_row(q51q, q51s, hslot, e * EBLK, row, NFE)
                worst = max(worst, abs(y[s * DIM + row] - want_v) / max(1.0, abs(want_v)))
            }
        }
        delete hslot
    }
    if (worst > 1e-4) {
        panic("w2_ic q51 correctness: worst rel diff {worst}")
    }
    print("w2_ic q51 correctness: worst rel {worst}\n")

    // ===== the timing matrix =====
    // per-pass routed weight bytes: K slots x (2 k4 planes + 1 q51 plane) x LAYERS
    let bytes_pass = double(LAYERS * K * (2l * NFE * NSB_ROW * 144l + DIM * NBLK_ROW * 24l))
    var arms <- [("pipe", 0, false, true), ("pipe_c", 0, true, true), ("serial", 1, false, true),
                 ("hazard", 2, false, true), ("pair13", 3, false, true), ("true_ser", 1, false, false)]
    var best : table<string; double>
    for (a in arms) {
        best[a._0] = 1e18lf
    }
    // true_ser runs pattern-free on a serial encoder — pattern 1 barriers would double-count
    for (_rd in range(rounds)) {
        for (a in arms) {
            if (!want("DASMETAL_LAB_ARMS", a._0)) {
                continue
            }
            let name = a._0
            let pattern = a._3 ? a._1 : 0
            let contig = a._2
            let conc = a._3
            let us = time_arm(queue, conc) $(enc : MetalComputeEncoder?) {
                for (_p in range64(passes)) {
                    enc_chain_pass(bufs, psos, enc, pattern, contig)
                }
            }
            let per = us / double(passes)
            if (per < best[name]) {
                best[name] = per
            }
        }
    }
    print("--- best per arm ({int(bytes_pass) / 1048576} MB routed weights/pass) ---\n")
    for (a in arms) {
        if (!want("DASMETAL_LAB_ARMS", a._0)) {
            continue
        }
        let per = best[a._0]
        let gbs = bytes_pass / per / 1000.0lf
        print("  {a._0}: {per} us/pass  {per / double(LAYERS)} us/layer  {gbs} wGB/s\n")
    }

    // ===== per-site kernel matrix (pipelined, no barriers — kernel throughput only) =====
    // psel >= 0: w1+w3 via that variant; -1: the pair kernel; -2: the q51 w2 site
    let bytes_w13 = double(LAYERS * K * 2l * NFE * NSB_ROW * 144l)
    let bytes_w2 = double(LAYERS * K * DIM * NBLK_ROW * 24l)
    var sarms <- [("w13", RP_K4, 4l), ("w13_x4", RP_X4, 4l), ("w13_r4", RP_R4, 8l),
                  ("w13_pair", -1, 4l), ("w2", -2, 4l), ("w2_ic", -3, 4l)]
    var sbest : table<string; double>
    for (a in sarms) {
        sbest[a._0] = 1e18lf
    }
    for (_rd in range(rounds)) {
        for (a in sarms) {
            if (!want("DASMETAL_LAB_ARMS", a._0)) {
                continue
            }
            let name = a._0
            let pslot = a._1
            let rtg = a._2
            let us = time_arm(queue, true) $(enc : MetalComputeEncoder?) {
                for (_p in range64(passes)) {
                    for (l in range64(LAYERS)) {
                        if (pslot == -1) {
                            enc_lab_pair(bufs, psos, enc, l, 0ul)
                        } elif (pslot == -2) {
                            enc_lab_w2(bufs, psos, enc, l, 0ul)
                        } elif (pslot == -3) {
                            enc_lab_w2(bufs, psos, enc, l, 0ul, RP_Q51IC)
                        } else {
                            enc_lab_w13v(bufs, psos, enc, l, 0ul, false, pslot, rtg)
                            enc_lab_w13v(bufs, psos, enc, l, SEL_W3_BASE, true, pslot, rtg)
                        }
                    }
                }
            }
            let per = us / double(passes)
            if (per < sbest[name]) {
                sbest[name] = per
            }
        }
    }
    print("--- per-site kernel matrix (w13 {int(bytes_w13) / 1048576} MB, w2 {int(bytes_w2) / 1048576} MB per pass) ---\n")
    for (a in sarms) {
        if (!want("DASMETAL_LAB_ARMS", a._0)) {
            continue
        }
        let per = sbest[a._0]
        let bytes_arm = a._1 <= -2 ? bytes_w2 : bytes_w13
        print("  {a._0}: {per} us/pass  {bytes_arm / per / 1000.0lf} wGB/s\n")
    }
    delete sarms
    delete sbest

    run_gmx4_lab(dev, queue, rounds, passes)
    run_gmm4_lab(dev, queue, rounds, passes)

    delete k4q
    delete k4c
    delete q51q
    delete q51s
    delete xf
    delete sel
    delete selb
    delete h0
    delete arms
    delete best
    for (b in bufs) {
        metal_release(b)
    }
    bufs |> clear()   // handles are metal_release-owned — delete on a ptr array would free pointees
    unsafe {
        delete bufs
    }
    for (p in psos) {
        metal_release(p)
    }
    psos |> clear()
    unsafe {
        delete psos
    }
    metal_release(queue)
    metal_release(dev)
    assert_no_metal_leaks()
}

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        main_apple(0)
    } else {
        print("das_metal is not built on this platform\n")
    }
}
