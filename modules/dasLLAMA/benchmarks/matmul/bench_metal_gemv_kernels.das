options gen2
options persistent_heap

require dastest/testing_boost
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require metal/msl_shader
require ?das_metal metal/das_metal_boost  // GPU driver half, Apple builds only
require ?das_metal dasllama/dasllama_metal_gemm  // the production 32x32 GEMM MSL (the batch-crossover row)
require dasllama/dasllama_metal_kernels  // the production GEMV MSL (v0 baseline)
require llvm/daslib/f16_cvt  // f16 bit converts for the K-quant superblock fills
require math  // used only inside the Apple static_if half
require strings

// The Metal Q8 GEMV kernel lab — the emission (decode) arc's weight-stream detector. Decode is
// GEMV-bandwidth-bound: one token reads every weight once (3B q8 ~3.4GB/token), so the metric
// that matters is the WEIGHT GB/s each variant sustains, not GMAC/s. The single-stream done gate
// (llama.cpp Metal tg128 = 87 t/s on the 3B) implies ~340 GB/s effective on M1 Max.
//
// Batched decode (B independent streams, one weight pass) turns the GEMV into a skinny GEMM:
// variants here stream the weight row ONCE and dot it against B activation vectors — the
// arithmetic-intensity amplifier the eval_batch override will ride. B in {1,2,4,8,16}.
//
// Correctness is bit-exact by construction (same contract as bench_metal_gemm_kernels): quants
// confined to [-8,7], scales to powers of two, so every product and partial sum is an exactly
// representable integer times a power of two — any accumulation order yields identical bits.
// v0 = the production metal_q8_gemv MSL text (zero copy drift); every variant must match the
// CPU reference exactly on the sampled region, and same-B variants must match each other.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/matmul/bench_metal_gemv_kernels.das
//
// DASMETAL_LAB_ROUNDS: interleaved timing rounds (default 3, best-of)
// DASMETAL_LAB_SHAPES / DASMETAL_LAB_VARIANTS: substring filters for fast iteration
//
// READING THE NUMBERS — SLC recycling: back-to-back reps re-read the same weight buffer, so
// shapes whose weights fit the system-level cache (q3b 9.4MB, kv3b 3.1MB, w13/w2 25MB partly)
// report SLC bandwidth, not DRAM (q3b can exceed the 400GB/s DRAM peak). Real decode streams
// all ~3.6GB once per token — cls3b (394MB) is the DRAM-honest proxy shape.
//
// Round-2 verdicts (M1 Max, 2026-07-14): v0/B1 ~352 wGB/s on cls3b (the bar is ~340) — the
// single-stream GEMV is at target already; gemv_b1's plain loop matches it (the 4-in-flight
// unroll is neutral at decode shapes). Stream-major gemv_bN saturates ALU ~340-540 GMAC/s (a
// B=8 step costs 8 single GEMVs — no batch win); the transposed-X forms (b4t/b8t/b16t) are
// PATHOLOGICAL (6-24 wGB/s, 64B-strided lane loads wreck coalescing) — kept as negative
// controls. gemm32 (production 32x32 GEMM, M-pad-32) is B-independent: it beats B sequential
// GEMVs from B~4-6 on big shapes and cls, but only from B~10 on kv3b (d=1024 = 32 tgs,
// under-occupied) — the batched driver picks per (B, shape), like gemm_use64.
//
// Round-3 verdicts (M1 Max, 2026-07-14 late): the X-STAGED forms (bNx — tgmem panel chunk,
// cooperative 128-thread load) are the fixed-B winners: b2x +40-88% over b2 (326-362 wGB/s;
// kv3b neutral), b4x ~2x over b4 (188-217 — ABOVE lcpp's ~186 effective at ne11=4); the
// per-lane device x loads were the limiter, NOT fma. b8x/b16x double their baselines but stay
// UNDER the per-site GEMM policy (96-107 vs 109-124 wGB/s at B=8) — the B>=5 GEMM boundary
// holds. The chunked-prefetch forms (bNc/bNc8 — per-lane-CONSECUTIVE byte4 chunks, one scale
// load per chunk) are 30-45% WORSE everywhere: inter-lane coalescing beats per-lane locality
// on AGX (lcpp's mul_mv_ext chunks are strided per-thread too — its prefetch win is the load
// BATCHING, which [unroll_count=4] already banks). Kept as negative controls.
//
// K-quant round verdicts (M1 Max, 2026-07-15): k4_b1c (16B compact scale blocks, whole block in
// ONE uint4 load) beats the lcpp mul_mv_q4_K wall at every shape — parity on the SLC-resident
// small shapes (q3b/kv3b), -16..-25% on qkv/w13/w2/cls (cls3b 0.677 vs 0.872 ms, 327 wGB/s,
// 1.87x the q8 GEMV row; the 144/288-byte ceiling is 2.0x). k4_b1 (reading the production 20B
// scale blocks) is the negative control: the 20B stride's varying mod-16 phase keeps LLVM from
// merging the scale-triple into a vector load (3 scalar loads/row, +4 issues/iter) — 7-8%
// behind lcpp everywhere. Production uploads therefore strip the pads GPU-side: quant plane
// byte-identical (zero-copy-able), scale plane repacked 20B->16B at upload (2.7% of tensor bytes).
//
// K-quant ext round 3 (M1 Max, 2026-07-18, the gemma4-12B shapes): ALL three fused-B levers
// REFUTED at B=4 — extb4 (load batching) dead neutral, extv4 (float4 accumulators) -20..-26%
// (register pressure: max_threads 448 -> 384), extx4 (tgmem x-stage) -12..-21%. GMAC/s is flat
// ~550-830 across every form = instruction-issue-bound; the production ext twins already sit at
// the reachable wall, and the compiler already extracts the ILP the v forms spell by hand. The
// production B=4-8 board reds are therefore NOT in this kernel. B=8 single-pass caveat: extv8
// loses to 2 x ext4 passes outright, extx8 ties them — but every 12B shape is SLC-resident here
// (<= 33MB vs 48MB SLC), so a single pass's halved DRAM weight traffic is INVISIBLE in this lab;
// only an in-graph A/B can resolve the B=8 form. Arms kept as negative controls.
//
// k5/k6 round (M1 Max, 2026-07-15): k6_b1d beats the lcpp q6_K wall at EVERY shape (-11..-19%,
// 321-370 wGB/s; the uint4+select sub-scale read of k6_b1c cost 2 resident sgs — lcpp-shape
// scattered byte loads of the 16B block win). k5 is ALU-bound on the per-element 5th-bit
// extract (~6 ops/elem vs k4's ~2.5 uint16-mask trick): k5_b2r (int-composed q5, 2 rows/sg)
// takes 5/6 shapes at -5..-27% vs the wall (243 wGB/s cls3b — well under k4/k6's ~330-370;
// further forms = ledger: uint16-mask nibble hybrid ~+5%, upload-time qh bit-plane transpose
// ~+10% but kills k5q zero-copy); the small-d kv3b shape prefers nr0=1 k5_b1c (wall -5% there,
// b2r +22% — under-occupied at 256 tgs) — production picks per site like the gemm policies.
//
// KqMv B2 colbase round (M1 Max, 2026-08-08, the reification followup; 3 launches per arm,
// interleaved, cls3b = the DRAM-honest cell): a LIVE colbase term on the B2 kernels costs real
// percent — k4 +1.9/+2.0/+2.0% cls3b (naive form) and +1.9/+1.8/+2.0% (hoisted form; NOT the
// per-b mul — same occupancy, and both hoisted B4s measure dead-neutral vs production), k6
// +0.5% cls3b both forms; k5 is the exception, its colbase form measures neutral-to-BETTER
// (-1.4% cls3b, max_threads 448 -> 512). The dead-decl preview (kq_mvb2s_*: production text +
// an unused colbase line — the static_if template stamp's only residue) is FREE: +-0.1% cls3b
// all launches. Template verdict: k5 = plain colbase form; k4/k6 = TILED static_if
// branch-duplicating only the b-loop and writeback tail (B4 byte-identical, B2 + dead decl).

// ===== kernel variants =====
// Shared contract (f32-X planar): b0 wq [d x n] Q8 byte4 view, b1 ws [d x n/32] f32 scales,
// b2 x [B x n] f32 float4 view, b3 y [B x d] f32, b4 ndim = n, b5 ddim = d. One simdgroup per
// output row, 128 threads = 4 rows per threadgroup (the production gemv geometry). The MSL
// emitter is pure das, so the classes compile on every platform — only the driver is Apple-gated.

// Fixed-B batched gemv: the weight row (and its scale) is loaded ONCE per k step and dotted
// against B activation vectors held in registers — B accumulators per lane, unrolled compile-time.
// The scale folds into the weight ONCE (w4 * s), saving B-1 scale multiplies per byte4 — exact
// under the lab fill (int x pow2).
class MetalGemvLabB2 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b2_msl")]
    def lab_gemv_b2 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[2]
        var k = lane
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
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

class MetalGemvLabB4 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b4_msl")]
    def lab_gemv_b4 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[4]
        // unroll_count=4 mirrors the production MetalGemvB4 (landed 3a1c1b6e9: +3.7% at B=4)
        var k = lane
        while [unroll_count=4] (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
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

class MetalGemvLabB8 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b8_msl")]
    def lab_gemv_b8 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[8]
        var k = lane
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
            for [unroll_full] (b in range(8)) {
                let xv = x[uint(b) * nb4 + k]
                acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(8)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalGemvLabB16 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b16_msl")]
    def lab_gemv_b16 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[16]
        var k = lane
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
            for [unroll_full] (b in range(16)) {
                let xv = x[uint(b) * nb4 + k]
                acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (b in range(16)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// B=1 control with the batched loop structure (single block in flight, no 4-deep ILP): isolates
// how much of v0's rate is the 4-blocks-in-flight unroll vs the plain stream.
class MetalGemvLabB1 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b1_msl")]
    def lab_gemv_b1 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc = 0.0
        var k = lane
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
            let xv = x[k]
            acc += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[row] = acc
        }
    }
}

// ===== production-B1 addressing A/B (index vs pointer-stream) — same buffers, same geometry =====
// q8_b1i = the EXACT production MetalQ8Gemv body (4 blocks in flight, array-index addressing);
// q8_b1p = the identical structure spelled as unsafe(addr()) pointer streams walked once per
// 128-stride iteration with immediate offsets inside the body (the MvB2 discipline). The single
// variable is the addressing shape the compiler sees — the A/B for whether the production
// single-stream GEMV leaves load-batching on the table.
// VERDICT (2026-07-17, M1 Max, clean disasm + timing): a WASH — identical ISA load structure
// (16 loads / 4 waits / 20 ffma both; the pointer form is 11 instrs LONGER on walk iadds),
// timing within noise both directions on all 6 shapes, occupancy identical (0x400). The
// compiler canonicalizes both spellings at this shape; don't re-litigate without new silicon.
// Both forms stream 327-388 wGB/s = at the effective wall — the e2e q8 B=1 deficit is
// BETWEEN dispatches (driver cadence), not in the kernel.
class MetalGemvLabQ8B1I {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_q8_b1i_msl")]
    def lab_q8_b1i {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc = 0.0
        var k = lane
        while (k + 96u < nb4) {
            let wa = float4(int4(wq[wbase + k]))
            let xa = x[k]
            let wb = float4(int4(wq[wbase + (k + 32u)]))
            let xb = x[k + 32u]
            let wc = float4(int4(wq[wbase + (k + 64u)]))
            let xc = x[k + 64u]
            let wd = float4(int4(wq[wbase + (k + 96u)]))
            let xd = x[k + 96u]
            acc += (wa.x * xa.x + wa.y * xa.y + wa.z * xa.z + wa.w * xa.w) * ws[sbase + k / 8u]
            acc += (wb.x * xb.x + wb.y * xb.y + wb.z * xb.z + wb.w * xb.w) * ws[sbase + (k + 32u) / 8u]
            acc += (wc.x * xc.x + wc.y * xc.y + wc.z * xc.z + wc.w * xc.w) * ws[sbase + (k + 64u) / 8u]
            acc += (wd.x * xd.x + wd.y * xd.y + wd.z * xd.z + wd.w * xd.w) * ws[sbase + (k + 96u) / 8u]
            k += 128u
        }
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k]))
            let x4 = x[k]
            acc += (w4.x * x4.x + w4.y * x4.y + w4.z * x4.z + w4.w * x4.w) * ws[sbase + k / 8u]
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[row] = acc
        }
    }
}

class MetalGemvLabQ8B1P {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_q8_b1p_msl")]
    def lab_q8_b1p {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        // pointer streams bound ONCE; walked per 128-stride iteration, immediate offsets inside
        var wp = unsafe(addr(wq[wbase + lane]))
        var xp = unsafe(addr(x[lane]))
        var sp = unsafe(addr(ws[sbase + lane / 8u]))
        var acc = 0.0
        var k = lane
        while (k + 96u < nb4) {
            let wa = float4(int4(unsafe(wp[0])))
            let xa = unsafe(xp[0])
            let wb = float4(int4(unsafe(wp[32])))
            let xb = unsafe(xp[32])
            let wc = float4(int4(unsafe(wp[64])))
            let xc = unsafe(xp[64])
            let wd = float4(int4(unsafe(wp[96])))
            let xd = unsafe(xp[96])
            acc += (wa.x * xa.x + wa.y * xa.y + wa.z * xa.z + wa.w * xa.w) * unsafe(sp[0])
            acc += (wb.x * xb.x + wb.y * xb.y + wb.z * xb.z + wb.w * xb.w) * unsafe(sp[4])
            acc += (wc.x * xc.x + wc.y * xc.y + wc.z * xc.z + wc.w * xc.w) * unsafe(sp[8])
            acc += (wd.x * xd.x + wd.y * xd.y + wd.z * xd.z + wd.w * xd.w) * unsafe(sp[12])
            wp = unsafe(wp + 128)
            xp = unsafe(xp + 128)
            sp = unsafe(sp + 16)
            k += 128u
        }
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k]))
            let x4 = x[k]
            acc += (w4.x * x4.x + w4.y * x4.y + w4.z * x4.z + w4.w * x4.w) * ws[sbase + k / 8u]
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[row] = acc
        }
    }
}

// Chunked-prefetch twins (lcpp mul_mv_ext's chpt=4 discipline, plus per-lane-CONSECUTIVE chunks):
// each lane owns 4 adjacent byte4s (16 elements) per k step, so the whole chunk sits inside ONE
// scale block — 1 scale load per 16 elements instead of 4 — and the quant loads share a base
// register (base+immediate addressing, merge-friendly). W dequants into registers up front, the
// dot chains follow: ~4 waits per 16 elements where the strided forms wait every 4. Requires
// n % 512 == 0 (the driver adds a strided tail; every lab shape tiles exactly).
class MetalGemvLabB2C {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b2c_msl")]
    def lab_gemv_b2c {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[2]
        let step = gl_SubgroupSize * 4u
        var c = lane * 4u
        while (c < nb4) {
            let sv = ws[sbase + c / 8u]
            var wf : float4[4]
            for [unroll_full] (g in range(4)) {
                wf[g] = float4(int4(wq[wbase + c + uint(g)])) * sv
            }
            for [unroll_full] (b in range(2)) {
                let xb = uint(b) * nb4 + c
                for [unroll_full] (g in range(4)) {
                    let xv = x[xb + uint(g)]
                    acc[b] += wf[g].x * xv.x + wf[g].y * xv.y + wf[g].z * xv.z + wf[g].w * xv.w
                }
            }
            c += step
        }
        for [unroll_full] (b in range(2)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalGemvLabB4C {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b4c_msl")]
    def lab_gemv_b4c {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[4]
        let step = gl_SubgroupSize * 4u
        var c = lane * 4u
        while (c < nb4) {
            let sv = ws[sbase + c / 8u]
            var wf : float4[4]
            for [unroll_full] (g in range(4)) {
                wf[g] = float4(int4(wq[wbase + c + uint(g)])) * sv
            }
            for [unroll_full] (b in range(4)) {
                let xb = uint(b) * nb4 + c
                for [unroll_full] (g in range(4)) {
                    let xv = x[xb + uint(g)]
                    acc[b] += wf[g].x * xv.x + wf[g].y * xv.y + wf[g].z * xv.z + wf[g].w * xv.w
                }
            }
            c += step
        }
        for [unroll_full] (b in range(4)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// chunk-depth probes: 8 byte4s per lane (a FULL scale block per chunk — one scale, one select);
// doubles the staged registers, requires n % 1024 == 0
class MetalGemvLabB2C8 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b2c8_msl")]
    def lab_gemv_b2c8 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[2]
        let step = gl_SubgroupSize * 8u
        var c = lane * 8u
        while (c < nb4) {
            let sv = ws[sbase + c / 8u]
            var wf : float4[8]
            for [unroll_full] (g in range(8)) {
                wf[g] = float4(int4(wq[wbase + c + uint(g)])) * sv
            }
            for [unroll_full] (b in range(2)) {
                let xb = uint(b) * nb4 + c
                for [unroll_full] (g in range(8)) {
                    let xv = x[xb + uint(g)]
                    acc[b] += wf[g].x * xv.x + wf[g].y * xv.y + wf[g].z * xv.z + wf[g].w * xv.w
                }
            }
            c += step
        }
        for [unroll_full] (b in range(2)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalGemvLabB4C8 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b4c8_msl")]
    def lab_gemv_b4c8 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[4]
        let step = gl_SubgroupSize * 8u
        var c = lane * 8u
        while (c < nb4) {
            let sv = ws[sbase + c / 8u]
            var wf : float4[8]
            for [unroll_full] (g in range(8)) {
                wf[g] = float4(int4(wq[wbase + c + uint(g)])) * sv
            }
            for [unroll_full] (b in range(4)) {
                let xb = uint(b) * nb4 + c
                for [unroll_full] (g in range(8)) {
                    let xv = x[xb + uint(g)]
                    acc[b] += wf[g].x * xv.x + wf[g].y * xv.y + wf[g].z * xv.z + wf[g].w * xv.w
                }
            }
            c += step
        }
        for [unroll_full] (b in range(4)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// X-staged batched gemv (the PERF_LEDGER candidate (a)): the tg's 4 rows all dot the SAME
// B x-vectors, so stage a k-chunk of the X panel in threadgroup memory ONCE per tg (cooperative
// 128-thread load, 4x fewer device x loads) and the per-lane inner-loop x reads become tgmem hits.
// W stays strided/coalesced. Chunk = 64 float4s per stream (4KB tgmem at B=4). Barrier-safe by
// construction only when every simdgroup owns a real row (d % 4 == 0 — all decode shapes) and
// every thread iterates the same chunk count (n % 1024 == 0... n=3072/8192 both tile: nb4 % 64).
class MetalGemvLabB4X {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @workgroup txp : float4[256]            // [4 streams x 64 float4] X panel chunk

    [metal_kernel(name="lab_gemv_b4x_msl")]
    def lab_gemv_b4x {
        let lid = gl_LocalInvocationID.x
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[4]
        var cb = 0u
        while (cb < nb4) {
            barrier()                       // previous round done reading the panel
            for [unroll_full] (j in range(2)) {
                let slot = uint(j) * 128u + lid
                txp[slot] = x[(slot / 64u) * nb4 + cb + slot % 64u]
            }
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(4)) {
                    let xv = txp[uint(b) * 64u + kt]
                    acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
                }
            }
            cb += 64u
        }
        for [unroll_full] (b in range(4)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalGemvLabB2X {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @workgroup txp : float4[128]            // [2 streams x 64 float4] X panel chunk

    [metal_kernel(name="lab_gemv_b2x_msl")]
    def lab_gemv_b2x {
        let lid = gl_LocalInvocationID.x
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[2]
        var cb = 0u
        while (cb < nb4) {
            barrier()
            let slot = lid
            txp[slot] = x[(slot / 64u) * nb4 + cb + slot % 64u]
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(2)) {
                    let xv = txp[uint(b) * 64u + kt]
                    acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
                }
            }
            cb += 64u
        }
        for [unroll_full] (b in range(2)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalGemvLabB8X {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @workgroup txp : float4[512]            // [8 streams x 64 float4] X panel chunk (8KB)

    [metal_kernel(name="lab_gemv_b8x_msl")]
    def lab_gemv_b8x {
        let lid = gl_LocalInvocationID.x
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[8]
        var cb = 0u
        while (cb < nb4) {
            barrier()
            for [unroll_full] (j in range(4)) {
                let slot = uint(j) * 128u + lid
                txp[slot] = x[(slot / 64u) * nb4 + cb + slot % 64u]
            }
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(8)) {
                    let xv = txp[uint(b) * 64u + kt]
                    acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
                }
            }
            cb += 64u
        }
        for [unroll_full] (b in range(8)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalGemvLabB16X {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @workgroup txp : float4[1024]           // [16 streams x 64 float4] X panel chunk (16KB)

    [metal_kernel(name="lab_gemv_b16x_msl")]
    def lab_gemv_b16x {
        let lid = gl_LocalInvocationID.x
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[16]
        var cb = 0u
        while (cb < nb4) {
            barrier()
            for [unroll_full] (j in range(8)) {
                let slot = uint(j) * 128u + lid
                txp[slot] = x[(slot / 64u) * nb4 + cb + slot % 64u]
            }
            barrier()
            for [unroll_full] (i in range(2)) {
                let k = cb + uint(i) * 32u + lane
                let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
                let kt = uint(i) * 32u + lane
                for [unroll_full] (b in range(16)) {
                    let xv = txp[uint(b) * 64u + kt]
                    acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
                }
            }
            cb += 64u
        }
        for [unroll_full] (b in range(16)) {
            let s = simd_sum(acc[b])
            if (lane == 0u) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// Transposed-X batched gemv: X stored stream-minor [n][16] so ONE float4 load covers 4 streams
// at one k — the B accumulator update vectorizes to B/4 vec4-fmas per k-element instead of B
// scalar chains (4x fewer ALU ops than the stream-major forms). Stride is baked at 16 streams;
// unused tail streams are zero-filled.
class MetalGemvLabB4T {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xt : array<float4>   // [n][16] stream-minor, float4 = 4 adjacent streams
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b4t_msl")]
    def lab_gemv_b4t {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc = float4(0.0)
        var k = lane
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
            let kk = k * 16u                // float4 slots: 4 per k-element, 16 per byte4
            acc += w4.x * xt[kk]
            acc += w4.y * xt[kk + 4u]
            acc += w4.z * xt[kk + 8u]
            acc += w4.w * xt[kk + 12u]
            k += gl_SubgroupSize
        }
        let s0 = simd_sum(acc.x)
        let s1 = simd_sum(acc.y)
        let s2 = simd_sum(acc.z)
        let s3 = simd_sum(acc.w)
        if (lane == 0u) {
            y[row] = s0
            y[ddim + row] = s1
            y[2u * ddim + row] = s2
            y[3u * ddim + row] = s3
        }
    }
}

class MetalGemvLabB8T {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xt : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b8t_msl")]
    def lab_gemv_b8t {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc0 = float4(0.0)
        var acc1 = float4(0.0)
        var k = lane
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
            let kk = k * 16u
            acc0 += w4.x * xt[kk]
            acc1 += w4.x * xt[kk + 1u]
            acc0 += w4.y * xt[kk + 4u]
            acc1 += w4.y * xt[kk + 5u]
            acc0 += w4.z * xt[kk + 8u]
            acc1 += w4.z * xt[kk + 9u]
            acc0 += w4.w * xt[kk + 12u]
            acc1 += w4.w * xt[kk + 13u]
            k += gl_SubgroupSize
        }
        for [unroll_full] (c in range(4)) {
            let s0 = simd_sum(acc0[c])
            let s1 = simd_sum(acc1[c])
            if (lane == 0u) {
                y[uint(c) * ddim + row] = s0
                y[uint(c + 4) * ddim + row] = s1
            }
        }
    }
}

class MetalGemvLabB16T {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xt : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_gemv_b16t_msl")]
    def lab_gemv_b16t {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float4[4]
        var k = lane
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
            let kk = k * 16u
            for [unroll_full] (g in range(4)) {
                acc[g] += w4.x * xt[kk + uint(g)]
                acc[g] += w4.y * xt[kk + 4u + uint(g)]
                acc[g] += w4.z * xt[kk + 8u + uint(g)]
                acc[g] += w4.w * xt[kk + 12u + uint(g)]
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (g in range(4)) {
            for [unroll_full] (c in range(4)) {
                let s = simd_sum(acc[g][c])
                if (lane == 0u) {
                    y[uint(g * 4 + c) * ddim + row] = s
                }
            }
        }
    }
}

// Runtime-B probe: same math with B as a uniform and a dynamic accumulator loop — measures what
// ONE PSO serving every batch size costs vs the compile-time-B set (the dynamic index likely
// spills acc[] to thread memory).
class MetalGemvLabBDyn {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @uniform @binding = 6 bcount : uint

    [metal_kernel(name="lab_gemv_bdyn_msl")]
    def lab_gemv_bdyn {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc : float[16]
        var k = lane
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k])) * ws[sbase + k / 8u]
            var b = 0u
            while (b < bcount) {
                let xv = x[b * nb4 + k]
                acc[b] += w4.x * xv.x + w4.y * xv.y + w4.z * xv.z + w4.w * xv.w
                b++
            }
            k += gl_SubgroupSize
        }
        var bo = 0u
        while (bo < bcount) {
            let s = simd_sum(acc[bo])
            if (lane == 0u) {
                y[bo * ddim + row] = s
            }
            bo++
        }
    }
}

// q8-X probe (B=1): the activation arrives PRE-QUANTIZED (the CPU decode contract — xq/xs planes);
// integer byte4 dot, one combined scale per byte4. Reads n bytes of X instead of 4n — X is noise
// either way; this measures whether the int-dot pipeline beats the float one on the weight stream.
class MetalGemvLabQ8X {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_gemv_q8x_msl")]
    def lab_gemv_q8x {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc = 0.0
        var k = lane
        while (k < nb4) {
            let w4 = int4(wq[wbase + k])
            let x4 = int4(xq[k])
            let idot = w4.x * x4.x + w4.y * x4.y + w4.z * x4.z + w4.w * x4.w
            acc += float(idot) * (ws[sbase + k / 8u] * xs[k / 8u])
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[row] = acc
        }
    }
}

// The 64-wide twin — the SAME v2 discipline on MulMm's 32(M) x 64(N) tile (its k-loop measured
// 156 instr vs lcpp's 165, per-element 29% under the 32-wide v2). Half the threadgroups of v2
// (d/64), so the per-shape winner is an occupancy-vs-ISA trade: wide-d sites (w13, cls) keep a
// healthy grid and should capture the per-element edge; 48/16-tg sites should stay 32-wide.
class MetalGemm64V2Lab {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>    // [32 x n] f32 rows (M-pad-32)
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint       // n (reduction dim)
    @uniform @binding = 5 ddim : uint       // d (output rows)
    @workgroup tw : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, (k row, wrow col)
    @workgroup tx : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, (token row, k col)

    [metal_kernel(name="lab_gemm64_v2_msl")]
    def lab_gemm64_v2 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ddim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        var mc : simdgroup_float8x8[8]      // this simdgroup's 16x32 quadrant: 2 token x 4 wrow tiles
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]                // dequantized W staging regs (this thread's k-half)
        let kdim4 = ndim / 4u
        let nkb = ndim / 32u
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
        var sp = unsafe(addr(ws[(nBase + lr0) * nkb]))
        var xp = unsafe(addr(x[(mBase + lr1) * kdim4 + il1 * 2u]))
        var kb = 0u
        while (kb < nkb) {
            let sA = unsafe(sp[0])
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
            sp = unsafe(sp + 1u)
            xp = unsafe(xp + 8u)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ddim + col0 + uint(i % 4) * 8u, ddim)
        }
    }
}

// The GemmB rework candidate — MulMm's staging discipline on the occupancy-friendly 32x32 tile:
// planar W + raw f32 X (the quant tails disappear), per-thread staging indexes hoisted out of the
// k-loop, device reads + dequant into registers BEFORE the barrier (they fly while the previous
// math drains), pointer-walked streams, k-major 8x8 W tiles so the mma reads straight.
class MetalGemmV2Lab {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>    // [32 x n] f32 rows (M-pad-32)
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint       // n (reduction dim)
    @uniform @binding = 5 ddim : uint       // d (output rows)
    @workgroup tw : float16[1024]           // W: 4 k-tiles x 4 row-tiles of 8x8, (k row, wrow col)
    @workgroup tx : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, (token row, k col)

    [metal_kernel(name="lab_gemm_v2_msl")]
    def lab_gemm_v2 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ddim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        var mc : simdgroup_float8x8[4]      // this simdgroup's 16x16 quadrant: 2 token x 2 wrow tiles
        var ma : simdgroup_half8x8[2]
        var mb : simdgroup_half8x8[2]
        let kdim4 = ndim / 4u
        let nkb = ndim / 32u
        // staging split: thread handles row lr (0..31) x k-eighth il (0..3) — 8 elems of W AND X
        let lr = lid / 4u
        let il = lid % 4u
        let tW0 = (il * 4u + lr / 8u) * 64u + (lr % 8u)
        let tX0 = (il * 4u + lr / 8u) * 64u + (lr % 8u) * 8u
        let aQ = (sg % 2u) * 128u           // wrow tile-pair base within a k-tile slab
        let bQ = (sg / 2u) * 128u           // token tile-pair base
        var wp = unsafe(addr(wq[(nBase + lr) * kdim4 + il * 2u]))
        var sp = unsafe(addr(ws[(nBase + lr) * nkb]))
        var xp = unsafe(addr(x[(mBase + lr) * kdim4 + il * 2u]))
        var kb = 0u
        while (kb < nkb) {
            let sA = unsafe(sp[0])
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
            sp = unsafe(sp + 1u)
            xp = unsafe(xp + 8u)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 16u
        simdgroup_store(mc[0], y, row0 * ddim + col0, ddim)
        simdgroup_store(mc[1], y, row0 * ddim + (col0 + 8u), ddim)
        simdgroup_store(mc[2], y, (row0 + 8u) * ddim + col0, ddim)
        simdgroup_store(mc[3], y, (row0 + 8u) * ddim + (col0 + 8u), ddim)
    }
}

// The split-K (vertical) twin of gemm_v2: grid gains a k-slice axis (grid.y = slice), each
// threadgroup walks nkb/ksplit k-blocks into its own partial-C plane, and a tiny reduce pass
// sums the planes — deterministic (no float atomics). At mp=32 the M axis is one tile, so
// k-slicing is the only way to grow the grid past d/32 threadgroups on the starved sites.
class MetalGemmV2SkLab {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 x : array<float4>    // [32 x n] f32 rows (M-pad-32)
    @ssbo @binding = 3 part : array<float>  // [ksplit x 32 x d] partial-C planes
    @uniform @binding = 4 ndim : uint       // n (reduction dim)
    @uniform @binding = 5 ddim : uint       // d (output rows)
    @uniform @binding = 6 ksplit : uint
    @workgroup tw : float16[1024]
    @workgroup tx : float16[1024]

    [metal_kernel(name="lab_gemm_sk_msl")]
    def lab_gemm_sk {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ddim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let sl = gl_WorkGroupID.y
        var mc : simdgroup_float8x8[4]
        var ma : simdgroup_half8x8[2]
        var mb : simdgroup_half8x8[2]
        let kdim4 = ndim / 4u
        let kbn = (ndim / 32u) / ksplit
        let kb0 = sl * kbn
        let lr = lid / 4u
        let il = lid % 4u
        let tW0 = (il * 4u + lr / 8u) * 64u + (lr % 8u)
        let tX0 = (il * 4u + lr / 8u) * 64u + (lr % 8u) * 8u
        let aQ = (sg % 2u) * 128u
        let bQ = (sg / 2u) * 128u
        var wp = unsafe(addr(wq[(nBase + lr) * kdim4 + kb0 * 8u + il * 2u]))
        var sp = unsafe(addr(ws[(nBase + lr) * (ndim / 32u) + kb0]))
        var xp = unsafe(addr(x[(mBase + lr) * kdim4 + kb0 * 8u + il * 2u]))
        var kb = 0u
        while (kb < kbn) {
            let sA = unsafe(sp[0])
            let q0 = unsafe(wp[0])
            let q1 = unsafe(wp[1])
            let b0 = unsafe(xp[0])
            let b1 = unsafe(xp[1])
            let w40 = float4(int4(q0)) * sA
            let w41 = float4(int4(q1)) * sA
            barrier()
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
            sp = unsafe(sp + 1u)
            xp = unsafe(xp + 8u)
            kb++
        }
        let pbase = sl * 32u * ddim
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 16u
        simdgroup_store(mc[0], part, pbase + row0 * ddim + col0, ddim)
        simdgroup_store(mc[1], part, pbase + row0 * ddim + (col0 + 8u), ddim)
        simdgroup_store(mc[2], part, pbase + (row0 + 8u) * ddim + col0, ddim)
        simdgroup_store(mc[3], part, pbase + (row0 + 8u) * ddim + (col0 + 8u), ddim)
    }
}

// the split-K reduce: y = sum of the ksplit partial planes (one thread per output element)
class MetalSkReduceLab {
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 y : array<float>
    @uniform @binding = 2 total : uint      // 32 * d
    @uniform @binding = 3 ksplit : uint

    [metal_kernel(name="lab_sk_reduce_msl")]
    def lab_sk_reduce {
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

// ===== K-quant (Q4_K) arms — the format-matrix W4 lab gate =====
// Weight contract: f32-X, in-kernel superblock dequant. Two weight layouts per format:
// the ggml interleaved block_q4_K stream (144B/sb: d f16, dmin f16, scales[12], qs[128]) for the
// lcpp wall arm, and the production plane pair (k4q 128B qs + k4s 20B [d][dmin][scales[12]][pad4],
// gguf_transcode_q4k's exact output) for ours. Geometry both: the lcpp mul_mv shape — 64-thread
// tg, 2 simdgroups x 2 rows (N_SG_Q4_K=2, N_R0_Q4_K=2), lane owns 32 elems of every 4th
// superblock (ix = lane/8 block-stream, iq/ir sub-position), y loaded once per superblock and
// reused across both rows. K-quant traffic/row: 148B (planes) / 144B (ggml) per 256 elems vs
// q8's 288B — the ~1.95x decode payoff W4 is after.

// the lcpp kernel_mul_mv_q4_K_f32 wall: their lane map, scale-halfword decode (kmask trick) and
// nibble-mask acc folds (x16/x256 groups folded by pow2 constants), on the ggml interleaved
// stream. Verbatim math; only the kargs plumbing is baked (dense strides from ndim/ddim).
class MetalKqLabLcppK4 {
    @ssbo @binding = 0 gsh : array<float16> // interleaved block_q4_K stream, half view (d at 72*blk)
    @ssbo @binding = 1 gqu : array<uint>    // the same stream, uint view (scales at 36*blk+1, qs at 36*blk+4)
    @ssbo @binding = 2 x : array<float>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_k4_lcpp_msl")]
    def lab_k4_lcpp {
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
                let so = blk * 36u + 1u
                let hw0 = (gqu[so] >> (16u * iq)) & 0xFFFF
                let hw2 = (gqu[so + 1u] >> (16u * iq)) & 0xFFFF
                let hw4 = (gqu[so + 2u] >> (16u * iq)) & 0xFFFF
                let sc0 = hw0 & 0x3F3F
                let sc1 = hw2 & 0x3F3F
                let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
                let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
                let qb = blk * 36u + 4u + iq * 8u + ir * 2u
                let q1a = gqu[qb]
                let q1b = gqu[qb + 1u]
                let q2a = gqu[qb + 16u]
                let q2b = gqu[qb + 17u]
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
                let dd = float(gsh[blk * 72u])
                let dm = float(gsh[blk * 72u + 1u])
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

// ours v1 — NEGATIVE CONTROL: the same lcpp shape reading the production 20B scale blocks
// (k4s dual f16/uint views). The 20B stride's varying mod-16 phase blocks LLVM's vector-load
// merge, so the scale triple compiles to 3 scalar loads per row — 7-8% behind the lcpp arm.
class MetalKqLabK4B1 {
    @ssbo @binding = 0 ksh : array<float16> // k4s plane, half view (d at 10*blk, dmin at 10*blk+1)
    @ssbo @binding = 1 ksu : array<uint>    // the same plane, uint view (scales at 5*blk+1)
    @ssbo @binding = 2 kqu : array<uint>    // k4q plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_b1_msl")]
    def lab_k4_b1 {
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
                let so = blk * 5u + 1u
                let hw0 = (ksu[so] >> (16u * iq)) & 0xFFFF
                let hw2 = (ksu[so + 1u] >> (16u * iq)) & 0xFFFF
                let hw4 = (ksu[so + 2u] >> (16u * iq)) & 0xFFFF
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
                let dd = float(ksh[blk * 10u])
                let dm = float(ksh[blk * 10u + 1u])
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

// ours v2: 16B scale blocks — the 20B transcode block minus its 4 pad bytes, built at upload.
// The 20B stride's varying mod-16 phase blocks LLVM's vector-load merge (the v1 scale triple
// compiles to 3 scalar loads; lcpp's 144B-stride triple gets one xyz), and 16B blocks land the
// whole [d|dmin][s0][s1][s2] block in ONE uint4 load at phase 0 — fewer issues than lcpp's
// xyz+halfpair AND 148->144B per 256 elems, byte-parity with ggml. Quant plane unchanged.
class MetalKqLabK4B1C {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same plane, uint4 view (whole block at blk)
    @ssbo @binding = 2 kqu : array<uint>    // k4q plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_b1c_msl")]
    def lab_k4_b1c {
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

// the lcpp kernel_mul_mv_q5_K_f32 wall (N_R0_Q5_K = 1 — one output row per simdgroup, rtg 2):
// same kmask scale decode as q4_K; nibble bytes read directly, the 5th bit lands as a +16
// select off the qh byte (bit s of qh[l] = sub-block s at position l).
class MetalKqLabLcppK5 {
    @ssbo @binding = 0 gsh : array<float16> // interleaved block_q5_K stream, half view (d at 88*blk)
    @ssbo @binding = 1 gsu : array<uint>    // the same stream, uint view (scales at 44*blk+1)
    @ssbo @binding = 2 gqb : array<uint8>   // the same stream, byte view (qh at 176*blk+16, qs at +48)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_lcpp_msl")]
    def lab_k5_lcpp {
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
            let so = blk * 44u + 1u
            let hw0 = (gsu[so] >> (16u * iq)) & 0xFFFF
            let hw2 = (gsu[so + 1u] >> (16u * iq)) & 0xFFFF
            let hw4 = (gsu[so + 2u] >> (16u * iq)) & 0xFFFF
            let sc0 = hw0 & 0x3F3F
            let sc1 = hw2 & 0x3F3F
            let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
            let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
            let qs0 = blk * 176u + 48u + iq * 32u + l0
            let qh0 = blk * 176u + 16u + l0
            var acc1 : float[4]
            var acc2 : float[4]
            for [unroll_full] (l in range(8)) {
                let q1 = uint(gqb[qs0 + uint(l)])
                let q2 = uint(gqb[qs0 + 64u + uint(l)])
                let h = uint(gqb[qh0 + uint(l)])
                acc1[0] += yl[l] * float(q1 & 0x0F)
                acc1[1] += yl[l + 8] * float(q1 & 0xF0)
                acc1[2] += yh[l] * float(q2 & 0x0F)
                acc1[3] += yh[l + 8] * float(q2 & 0xF0)
                acc2[0] += (h & hm1) != 0u ? yl[l] : 0.0
                acc2[1] += (h & hm2) != 0u ? yl[l + 8] : 0.0
                acc2[2] += (h & hm3) != 0u ? yh[l] : 0.0
                acc2[3] += (h & hm4) != 0u ? yh[l + 8] : 0.0
            }
            let dd = float(gsh[blk * 88u])
            let dm = float(gsh[blk * 88u + 1u])
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

// ours v1 — the SMALL-d pick (kv-shape wins, big shapes lose to b2r): k5 plane pair (k5q 160B
// [qs][qh] + 16B compact scale blocks), lcpp's q5 lane map and select-form math, nr0=1.
class MetalKqLabK5B1C {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same plane, uint4 view (whole block at blk)
    @ssbo @binding = 2 kqb : array<uint8>   // k5q plane, byte view (qs at 160*blk, qh at 160*blk+128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_b1c_msl")]
    def lab_k5_b1c {
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

// ours, take 2 (superseded by b2r — kept as the nr0 isolation control): q5 composed in INT
// (nibble | qh bit << 4, the k6 discipline) — ONE fma per element instead of lcpp's
// mask-fma + select-add pair, halving the float-accumulate chains.
class MetalKqLabK5B1D {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same plane, uint4 view (whole block at blk)
    @ssbo @binding = 2 kqb : array<uint8>   // k5q plane, byte view (qs at 160*blk, qh at 160*blk+128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_b1d_msl")]
    def lab_k5_b1d {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hb0 = 2u * iq
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
            sumf += dd * (float(sc0 & 255u) * acc[0] + float(sc0 >> 8u) * acc[1]
                + float(sc2 & 255u) * acc[2] + float(sc2 >> 8u) * acc[3])
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

// ours, take 3 — THE big-shape k5 winner (5/6 shapes, -5..-27% vs the wall; small-d kv shape
// prefers b1c): b1d's int-composed q5 x TWO rows per simdgroup (the k4/k6 shape) — the 32
// y-loads and the sumy build amortize across both rows.
class MetalKqLabK5B2R {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqb : array<uint8>
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_b2r_msl")]
    def lab_k5_b2r {
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

// the lcpp kernel_mul_mv_q6_K_f32 wall (N_R0_Q6_K = 2): 6-bit q = nibble | 2 qh bits, minus 32,
// per-16-element SIGNED int8 sub-scales, no min side. Lane owns 4x4-elem slices of every 2nd
// superblock; the 4 scattered sub-scale byte loads are their shape.
class MetalKqLabLcppK6 {
    @ssbo @binding = 0 gsh : array<float16> // interleaved block_q6_K stream, half view (d at 105*blk+104)
    @ssbo @binding = 1 gqb : array<uint8>   // the same stream, byte view (ql at 210*blk, qh at +128, scales at +192)
    @ssbo @binding = 2 x : array<float>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint

    [metal_kernel(name="lab_k6_lcpp_msl")]
    def lab_k6_lcpp {
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
                let ql0 = blk * 210u + ip * 64u + l0
                let qh0 = blk * 210u + 128u + ip * 32u + l0
                let sb = blk * 210u + 192u + is0
                var sums : float[4]
                for [unroll_full] (l in range(4)) {
                    let q1 = uint(gqb[ql0 + uint(l)])
                    let q2 = uint(gqb[ql0 + 32u + uint(l)])
                    let h = uint(gqb[qh0 + uint(l)])
                    sums[0] += yl[4 * l] * (float((q1 & 0x0F) | ((h & 0x03) << 4u)) - 32.0)
                    sums[1] += yl[4 * l + 1] * (float((q2 & 0x0F) | ((h & 0x0C) << 2u)) - 32.0)
                    sums[2] += yl[4 * l + 2] * (float((q1 >> 4u) | (h & 0x30)) - 32.0)
                    sums[3] += yl[4 * l + 3] * (float((q2 >> 4u) | ((h & 0xC0) >> 2u)) - 32.0)
                }
                let s0 = float(((int(gqb[sb]) ^ 128) - 128))
                let s2 = float(((int(gqb[sb + 2u]) ^ 128) - 128))
                let s4 = float(((int(gqb[sb + 4u]) ^ 128) - 128))
                let s6 = float(((int(gqb[sb + 6u]) ^ 128) - 128))
                let dd = float(gsh[blk * 105u + 104u])
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

// ours v1 — NEGATIVE CONTROL (the uint4 + ip-selects cost 2 resident sgs; b1d wins every
// shape): k6q plane (192B [ql][qh]) + SPLIT scale planes — the 18B production scale block is
// not even uint-addressable per block (18B stride), so the GPU side takes 16B sub-scale blocks
// (whole block in one uint4, lane extracts its 4 signed bytes by shift) + a packed f16 d plane.
class MetalKqLabK6B1C {
    @ssbo @binding = 0 kdh : array<float16> // d plane (d of blk at blk)
    @ssbo @binding = 1 ksc : array<uint4>   // 16B sub-scale blocks (16 int8 at blk)
    @ssbo @binding = 2 kqb : array<uint8>   // k6q plane, byte view (ql at 192*blk, qh at 192*blk+128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_b1c_msl")]
    def lab_k6_b1c {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let o = (l0 / 16u) * 8u   // bit offset of sub-scale is0 within its uint (is0 % 4 = l0/16)
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
                let sv = ksc[blk]
                let ulo = ip != 0u ? sv.z : sv.x   // sub-scales is0, is0+2 (block bytes 0..3 / 8..11)
                let uhi = ip != 0u ? sv.w : sv.y   // sub-scales is0+4, is0+6
                let s0 = float(((int(ulo >> o) & 255) ^ 128) - 128)
                let s2 = float(((int(ulo >> (o + 16u)) & 255) ^ 128) - 128)
                let s4 = float(((int(uhi >> o) & 255) ^ 128) - 128)
                let s6 = float(((int(uhi >> (o + 16u)) & 255) ^ 128) - 128)
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

// k6 take 2 — THE k6 winner (beats the lcpp wall at every shape, -11..-19%, 321-370 wGB/s):
// the sub-scale block read as 4 scattered byte loads (lcpp's own shape — fewer live registers
// than b1c's uint4+select form, 28 vs 26 resident sgs; the 16B block still spans one line).
class MetalKqLabK6B1D {
    @ssbo @binding = 0 kdh : array<float16> // d plane (d of blk at blk)
    @ssbo @binding = 1 kscb : array<uint8>  // 16B sub-scale blocks, byte view
    @ssbo @binding = 2 kqb : array<uint8>   // k6q plane, byte view (ql at 192*blk, qh at 192*blk+128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_b1d_msl")]
    def lab_k6_b1d {
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

// ===== K-quant batched mv arms (M2.1) — B x-streams through ONE weight pass =====
// STATUS (2026-07-15, round 1 verdict): NEGATIVE so far — the weight-amortization thesis is
// wrong for these shapes. The kq GEMV is ALU/ISSUE-bound, not BW-bound (b1c cls3b executes
// ~94 G sg-instr/s at ~500 instrs per row-token; DRAM had headroom at 327 of ~400 GB/s), so
// sharing the weight pass across streams saves bytes that were not the bottleneck while the
// per-stream accumulator sets + w[32] block cost occupancy (0x200 vs 0x2c0) and drop the
// execution rate ~1.75x. v4 (float4 x views, registered pre-folded weights, batch-load runs)
// is ISA-clean — 362 instrs / 17 vector loads / 4 waits vs b1c's 393/17/3 — and still loses:
// cls3b 2.37 ms vs 2 x b1c = 1.35. The stream-0-only control (k4_mvb2ctl, B=1) pins the split:
// the 1-row/sg + w-register structure costs 0.976 vs b1c 0.677; the second stream adds 1.39.
// Iteration levers, in order: (a) hoist the row-invariant min-side sums (sy = per-quarter
// x-sums — every ROW recomputes identical values; a d-independent prepass kills ~25% of the
// per-token ALU), (b) inline per-quarter dequant (8 live w regs, not 32) for occupancy,
// (c) fp16 x panels (2x fma rate) — accuracy call. Production interim: B=2..8 as B separate
// B1 dispatches per site (= the lcpp wall exactly; honest, GPU-only, no CPU fallback).
// Address-alignment lesson (cost v1-v3 their 3-7x): a runtime-uniform term (b*ndim) in a
// scalar-view index defeats LLVM's vector-load merge — it cannot prove 16B alignment. Bind a
// float4 VIEW and index in float4 units; alignment holds by construction (the production q8
// mv kernels' xs4 discipline).

class MetalKqLabK4MvB2 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>  // float4 view: stream-strided reads stay provably 16B-aligned
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_mvb2_msl")]
    def lab_k4_mvb2 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let row = gl_WorkGroupID.x * 2u + gl_SubgroupID
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + iq * 64u + ir * 8u
            let blk = row * nb + ib
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
            // dequant ONCE into quarter-major registers, mask folds pre-applied — the per-elem
            // converts amortize across the B streams; the x side then reads consecutive runs
            var w : float[32]
            for [unroll_full] (i in range(4)) {
                let sh = 16u * (uint(i) & 1u)
                let q1 = ((i < 2 ? q1a : q1b) >> sh) & 0xFFFF
                let q2 = ((i < 2 ? q2a : q2b) >> sh) & 0xFFFF
                w[2 * i] = float(q1 & 0x000F)
                w[2 * i + 1] = float(q1 & 0x0F00) * 0.00390625
                w[8 + 2 * i] = float(q1 & 0x00F0) * 0.0625
                w[8 + 2 * i + 1] = float(q1 & 0xF000) * 0.000244140625
                w[16 + 2 * i] = float(q2 & 0x000F)
                w[16 + 2 * i + 1] = float(q2 & 0x0F00) * 0.00390625
                w[24 + 2 * i] = float(q2 & 0x00F0) * 0.0625
                w[24 + 2 * i + 1] = float(q2 & 0xF000) * 0.000244140625
            }
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            for [unroll_full] (b in range(2)) {
                let xb4 = uint(b) * (ndim / 4u) + yb / 4u
                var acc : float[4]
                var sy : float[4]
                for [unroll_full] (q4 in range(4)) {
                    let qo4 = q4 == 0 ? 0u : (q4 == 1 ? 8u : (q4 == 2 ? 32u : 40u))
                    let va = x[xb4 + qo4]
                    let vb = x[xb4 + qo4 + 1u]
                    acc[q4] += va.x * w[q4 * 8] + va.y * w[q4 * 8 + 1] + va.z * w[q4 * 8 + 2] + va.w * w[q4 * 8 + 3]
                    acc[q4] += vb.x * w[q4 * 8 + 4] + vb.y * w[q4 * 8 + 5] + vb.z * w[q4 * 8 + 6] + vb.w * w[q4 * 8 + 7]
                    sy[q4] += va.x + va.y + va.z + va.w + vb.x + vb.y + vb.z + vb.w
                }
                sumf[b] += dd * (acc[0] * float(sc0 & 255u) + acc[1] * float(sc0 >> 8u)
                    + acc[2] * float(sc2 & 255u) + acc[3] * float(sc2 >> 8u))
                sumf[b] -= dm * (sy[0] * float(sc1 & 255u) + sy[1] * float(sc1 >> 8u)
                    + sy[2] * float(sc3 & 255u) + sy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (b in range(2)) {
            let sm = simd_sum(sumf[b])
            if (lane == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

class MetalKqLabK4MvB2Ctl {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>  // float4 view: stream-strided reads stay provably 16B-aligned
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_mvb2ctl_msl")]
    def lab_k4_mvb2ctl {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let row = gl_WorkGroupID.x * 2u + gl_SubgroupID
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + iq * 64u + ir * 8u
            let blk = row * nb + ib
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
            // dequant ONCE into quarter-major registers, mask folds pre-applied — the per-elem
            // converts amortize across the B streams; the x side then reads consecutive runs
            var w : float[32]
            for [unroll_full] (i in range(4)) {
                let sh = 16u * (uint(i) & 1u)
                let q1 = ((i < 2 ? q1a : q1b) >> sh) & 0xFFFF
                let q2 = ((i < 2 ? q2a : q2b) >> sh) & 0xFFFF
                w[2 * i] = float(q1 & 0x000F)
                w[2 * i + 1] = float(q1 & 0x0F00) * 0.00390625
                w[8 + 2 * i] = float(q1 & 0x00F0) * 0.0625
                w[8 + 2 * i + 1] = float(q1 & 0xF000) * 0.000244140625
                w[16 + 2 * i] = float(q2 & 0x000F)
                w[16 + 2 * i + 1] = float(q2 & 0x0F00) * 0.00390625
                w[24 + 2 * i] = float(q2 & 0x00F0) * 0.0625
                w[24 + 2 * i + 1] = float(q2 & 0xF000) * 0.000244140625
            }
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            for [unroll_full] (b in range(1)) {
                let xb4 = uint(b) * (ndim / 4u) + yb / 4u
                var acc : float[4]
                var sy : float[4]
                for [unroll_full] (q4 in range(4)) {
                    let qo4 = q4 == 0 ? 0u : (q4 == 1 ? 8u : (q4 == 2 ? 32u : 40u))
                    let va = x[xb4 + qo4]
                    let vb = x[xb4 + qo4 + 1u]
                    acc[q4] += va.x * w[q4 * 8] + va.y * w[q4 * 8 + 1] + va.z * w[q4 * 8 + 2] + va.w * w[q4 * 8 + 3]
                    acc[q4] += vb.x * w[q4 * 8 + 4] + vb.y * w[q4 * 8 + 5] + vb.z * w[q4 * 8 + 6] + vb.w * w[q4 * 8 + 7]
                    sy[q4] += va.x + va.y + va.z + va.w + vb.x + vb.y + vb.z + vb.w
                }
                sumf[b] += dd * (acc[0] * float(sc0 & 255u) + acc[1] * float(sc0 >> 8u)
                    + acc[2] * float(sc2 & 255u) + acc[3] * float(sc2 >> 8u))
                sumf[b] -= dm * (sy[0] * float(sc1 & 255u) + sy[1] * float(sc1 >> 8u)
                    + sy[2] * float(sc3 & 255u) + sy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (b in range(2)) {
            let sm = simd_sum(sumf[b])
            if (lane == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

class MetalKqLabK5MvB2 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqb : array<uint8>
    @ssbo @binding = 3 x : array<float4>  // float4 view: stream-strided reads stay provably 16B-aligned
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_mvb2_msl")]
    def lab_k5_mvb2 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hb0 = 2u * iq
        let nb = ndim / 256u
        let row = gl_WorkGroupID.x * 2u + gl_SubgroupID
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + iq * 64u + l0
            let blk = row * nb + ib
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
            var w : float[32]
            for [unroll_full] (l in range(8)) {
                let q1 = uint(kqb[qs0 + uint(l)])
                let q2 = uint(kqb[qs0 + 64u + uint(l)])
                let h = uint(kqb[qh0 + uint(l)]) >> hb0
                w[l] = float((q1 & 0x0F) | ((h & 1u) << 4u))
                w[8 + l] = float((q1 >> 4u) | ((h & 2u) << 3u))
                w[16 + l] = float((q2 & 0x0F) | ((h & 16u) << 0u))
                w[24 + l] = float((q2 >> 4u) | ((h & 32u) >> 1u))
            }
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            for [unroll_full] (b in range(2)) {
                let xb4 = uint(b) * (ndim / 4u) + yb / 4u
                var acc : float[4]
                var sy : float[4]
                for [unroll_full] (q4 in range(4)) {
                    let qo4 = q4 == 0 ? 0u : (q4 == 1 ? 8u : (q4 == 2 ? 32u : 40u))
                    let va = x[xb4 + qo4]
                    let vb = x[xb4 + qo4 + 1u]
                    acc[q4] += va.x * w[q4 * 8] + va.y * w[q4 * 8 + 1] + va.z * w[q4 * 8 + 2] + va.w * w[q4 * 8 + 3]
                    acc[q4] += vb.x * w[q4 * 8 + 4] + vb.y * w[q4 * 8 + 5] + vb.z * w[q4 * 8 + 6] + vb.w * w[q4 * 8 + 7]
                    sy[q4] += va.x + va.y + va.z + va.w + vb.x + vb.y + vb.z + vb.w
                }
                sumf[b] += dd * (float(sc0 & 255u) * acc[0] + float(sc0 >> 8u) * acc[1]
                    + float(sc2 & 255u) * acc[2] + float(sc2 >> 8u) * acc[3])
                sumf[b] -= dm * (sy[0] * float(sc1 & 255u) + sy[1] * float(sc1 >> 8u)
                    + sy[2] * float(sc3 & 255u) + sy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (b in range(2)) {
            let sm = simd_sum(sumf[b])
            if (lane == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

class MetalKqLabK6MvB2 {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 ksc : array<uint4>
    @ssbo @binding = 2 kqb : array<uint8>
    @ssbo @binding = 3 x : array<float4>  // float4 view: stream-strided reads stay provably 16B-aligned
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_mvb2_msl")]
    def lab_k6_mvb2 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let o = (l0 / 16u) * 8u
        let nb = ndim / 256u
        let row = gl_WorkGroupID.x * 2u + gl_SubgroupID
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + ip * 128u + l0
            let blk = row * nb + ib
            let ql0 = blk * 192u + ip * 64u + l0
            let qh0 = blk * 192u + 128u + ip * 32u + l0
            var w : float[16]
            for [unroll_full] (l in range(4)) {
                let q1 = uint(kqb[ql0 + uint(l)])
                let q2 = uint(kqb[ql0 + 32u + uint(l)])
                let h = uint(kqb[qh0 + uint(l)])
                w[l] = float((q1 & 0x0F) | ((h & 0x03) << 4u)) - 32.0
                w[4 + l] = float((q2 & 0x0F) | ((h & 0x0C) << 2u)) - 32.0
                w[8 + l] = float((q1 >> 4u) | (h & 0x30)) - 32.0
                w[12 + l] = float((q2 >> 4u) | ((h & 0xC0) >> 2u)) - 32.0
            }
            let sv = ksc[blk]
            let ulo = ip != 0u ? sv.z : sv.x
            let uhi = ip != 0u ? sv.w : sv.y
            let s0 = float(((int(ulo >> o) & 255) ^ 128) - 128)
            let s2 = float(((int(ulo >> (o + 16u)) & 255) ^ 128) - 128)
            let s4 = float(((int(uhi >> o) & 255) ^ 128) - 128)
            let s6 = float(((int(uhi >> (o + 16u)) & 255) ^ 128) - 128)
            let dd = float(kdh[blk])
            for [unroll_full] (b in range(2)) {
                let xb4 = uint(b) * (ndim / 4u) + yb / 4u
                var acc : float[4]
                for [unroll_full] (q4 in range(4)) {
                    let v = x[xb4 + uint(q4) * 8u]
                    acc[q4] += v.x * w[q4 * 4] + v.y * w[q4 * 4 + 1] + v.z * w[q4 * 4 + 2] + v.w * w[q4 * 4 + 3]
                }
                sumf[b] += dd * (acc[0] * s0 + acc[1] * s2 + acc[2] * s4 + acc[3] * s6)
            }
            ib += 2u
        }
        for [unroll_full] (b in range(2)) {
            let sm = simd_sum(sumf[b])
            if (lane == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

// ===== K-quant ext arms (round 2) — the lcpp mul_mv_ext SHAPE on our planes =====
// Round 1 failed on occupancy (w[32] + per-stream acc/sy sets in the b1c lane geometry). The
// lcpp ext form keeps registers tiny and STILL amortizes the weight pass: one thread owns one
// output row (8 K-lanes x 4 rows per simdgroup, 8 rows/tg, rtg=8), streams 16-elem chunks
// (thread tx owns chunks tx and tx+8 of every superblock), materializes the chunk dequant into
// 16 regs with d*sc AND dmin*m folded in (no per-column min-side sums at all — the round-1 sy
// lever dissolves), then R column dot-folds off INDEXED float4 bases (their walked pointers
// traded for independent addresses per the AGX lesson). Scale decode = the ggml scalar pair
// formula per chunk: chunk tx is always a LOW pair (j<4), chunk tx+8 always HIGH — both baked.

class MetalKqLabK4Ext2 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_ext2_msl")]
    def lab_k4_ext2 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let q4i = tx / 4u                   // quarter of chunk tx (low pair j = 2*q4i + hp)
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u             // uint offset of the 16-byte half inside the quarter
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u            // low-nibble phase for BOTH of this thread's chunks
        let nsh = nib ? 0u : 4u             // hoisted: the nibble select must NOT sit in the elem loop
        let nb4 = ndim / 4u
        var sumf : float[2]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            for [unroll_full] (ch in range(2)) {
                // chunk tx (+0) decodes pair jl; chunk tx+8 (+2 quarters) decodes pair jl+4
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
                // x float4 base of this chunk: quarter elems 64*q + nibble half (0/32) + 16-byte half
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
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalKqLabK4Ext4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_ext4_msl")]
    def lab_k4_ext4 {
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
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(4)) {
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
        for [unroll_full] (b in range(4)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// k6 ext twins: chunk tx sits in half 0, chunk tx+8 in half 1 — same 32-slot and 16-byte offset
// both (s = tx/2, o = (tx%2)*16), so the 6-bit assembly is baked per thread. One SIGNED scattered
// byte sub-scale per chunk (the b1d discipline — the uint4+select form costs 2 resident sgs);
// -32 folds into ml = 32*dl.
class MetalKqLabK6Ext2 {
    @ssbo @binding = 0 kdh : array<float16> // d plane (the scale buffer bound at byte nsb*16)
    @ssbo @binding = 1 kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 kqb : array<uint8>   // quant plane (ql at 192*blk, qh at +128)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_ext2_msl")]
    def lab_k6_ext2 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let s = tx / 2u                     // 32-elem slot within the 128-half
        let o = (tx % 2u) * 16u             // 16-byte offset within the slot
        // hoisted slot params — the 6-bit assembly must be branch-free in the elem loop
        let bsel = (s % 2u) * 32u           // ql byte half: slots 1/3 read ql[32+j]
        let shr = s < 2u ? 0u : 4u          // nibble: slots 0/1 low, 2/3 high
        let hmask = 0x03u << (2u * s)       // qh bits of this slot
        let lsh = s == 0u ? 4u : (s == 1u ? 2u : 0u)
        let rsh = s == 3u ? 2u : 0u
        let nb4 = ndim / 4u
        var sumf : float[2]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {   // ch = the 128-half (chunk tx / tx+8)
                let ip = uint(ch)
                let ql0 = blk * 192u + ip * 64u + o
                let qh0 = blk * 192u + 128u + ip * 32u + o
                let g = ip * 8u + s * 2u + o / 16u
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 32.0
                var w : float[16]
                for [unroll_full] (l in range(16)) {
                    let qj = uint(l)
                    let h = uint(kqb[qh0 + qj])
                    let hv = ((h & hmask) << lsh) >> rsh
                    let q6 = ((uint(kqb[ql0 + bsel + qj]) >> shr) & 0x0Fu) | hv
                    w[l] = float(q6) * dl - ml
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o / 4u
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
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

class MetalKqLabK6Ext4 {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqb : array<uint8>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_ext4_msl")]
    def lab_k6_ext4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let s = tx / 2u
        let o = (tx % 2u) * 16u
        let bsel = (s % 2u) * 32u
        let shr = s < 2u ? 0u : 4u
        let hmask = 0x03u << (2u * s)
        let lsh = s == 0u ? 4u : (s == 1u ? 2u : 0u)
        let rsh = s == 3u ? 2u : 0u
        let nb4 = ndim / 4u
        var sumf : float[4]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let ql0 = blk * 192u + ip * 64u + o
                let qh0 = blk * 192u + 128u + ip * 32u + o
                let g = ip * 8u + s * 2u + o / 16u
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 32.0
                var w : float[16]
                for [unroll_full] (l in range(16)) {
                    let qj = uint(l)
                    let h = uint(kqb[qh0 + qj])
                    let hv = ((h & hmask) << lsh) >> rsh
                    let q6 = ((uint(kqb[ql0 + bsel + qj]) >> shr) & 0x0Fu) | hv
                    w[l] = float(q6) * dl - ml
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o / 4u
                for [unroll_full] (b in range(4)) {
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
        for [unroll_full] (b in range(4)) {
            var sm = sumf[b]
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

// k5 ext twins: k4's chunk geometry and kmask scale decode verbatim (q5_K shares the 12-byte
// scale block), plus the qh 5th-bit overlay. The 160B plane is uint-viewed (40 uints/blk: qs at
// 40*blk, qh at 40*blk+32) so the qh side is 4 hoisted uints per superblock, not per-element
// byte reads. The overlay bit of chunk (group, nibble half) = the chunk's kmask pair index —
// jl for chunk tx, jl+4 for chunk tx+8 — so the baked scale indexing serves the qh shift too.
class MetalKqLabK5Ext2 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 kqu : array<uint>    // k5 plane, uint view (qs at 40*blk, qh at 40*blk+32)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_ext2_msl")]
    def lab_k5_ext2 {
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
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalKqLabK5Ext4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_ext4_msl")]
    def lab_k5_ext4 {
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
        for [unroll_full] (b in range(4)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// k6 ext twins, uint-view quant loads (the mulmm round-1 pattern): the 32 scattered byte loads
// per chunk become 4+4 uint loads, and the slot's lsh/rsh dance is identically
// ((h >> 2s) & 3) << 4, so the 6-bit compose vectorizes to one masked-or per uint.
class MetalKqLabK6Ext2U {
    @ssbo @binding = 0 kdh : array<float16> // d plane (the scale buffer bound at byte nsb*16)
    @ssbo @binding = 1 kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 kqu : array<uint>    // quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_ext2u_msl")]
    def lab_k6_ext2u {
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
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

class MetalKqLabK6Ext4U {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_ext4u_msl")]
    def lab_k6_ext4u {
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
        for [unroll_full] (b in range(4)) {
            var sm = sumf[b]
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

// ===== K-quant ext arms (round 3) — the fused-B rework levers, isolated =====
// The knockout said the ext twins' GEMV share scales ~linearly in B and is FORMAT-INDEPENDENT
// (~14ms/row at the 12B shapes) — that fingerprints the per-column dot block, not the dequant:
// 16 SERIAL adds into one scalar accumulator per chunk per column. Round-3 levers, one arm each:
//   extb4 = quant loads for BOTH chunks issued ahead of any decode (the q8 mv load-batching
//           lesson), scalar accumulators unchanged — isolates the load-latency half.
//   extv4 = extb4 + float4 accumulators (4 independent fma chains per column, one horizontal
//           fold per row after the loop) — the fma-ILP half. Reassociates the add order (exact
//           under the lab fill; production compares within tolerance).
//   extv8 = the v form at 8 columns in ONE weight pass (production B=8 today = 2 x B4 groups =
//           2 weight passes) — trades register pressure for halved weight traffic.

class MetalKqLabK4ExtB4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_extb4_msl")]
    def lab_k4_extb4 {
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
        var sumf : float[4]
        var sb = 0u
        while (sb < nb) {
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
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qvv = qv[4 * ch + u]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qvv >> (8u * uint(e))) & 255u
                        w[4 * u + e] = float((byv >> nsh) & 0x0Fu) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(4)) {
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
        for [unroll_full] (b in range(4)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalKqLabK4ExtV4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_extv4_msl")]
    def lab_k4_extv4 {
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
        var acc : float4[4]
        var sb = 0u
        while (sb < nb) {
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
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(4)) {
                    let xb4 = uint(b) * nb4 + cx4
                    for [unroll_full] (u in range(4)) {
                        acc[b] += x[xb4 + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalKqLabK4ExtV8 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k4_extv8_msl")]
    def lab_k4_extv8 {
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
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(8)) {
                    let xb4 = uint(b) * nb4 + cx4
                    for [unroll_full] (u in range(4)) {
                        acc[b] += x[xb4 + uint(u)] * w4[u]
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
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// k6 v-form: the 8 quant-plane loads of each chunk (ql x4 + qh x4) batch ahead of the chunk's
// decode; the nibble/high-bit composition and scale math are the ext twins' verbatim.
class MetalKqLabK6ExtV4 {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_extv4_msl")]
    def lab_k6_extv4 {
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
        var acc : float4[4]
        var sb = 0u
        while (sb < nb) {
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
                    let qv = ((qu[u] >> shr) & 0x0F0F0F0F) | (((hu[u] >> hsh) & 0x03030303) << 4u)
                    w4[u] = float4(float(qv & 255u), float((qv >> 8u) & 255u),
                        float((qv >> 16u) & 255u), float((qv >> 24u) & 255u)) * dl - float4(ml)
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                for [unroll_full] (b in range(4)) {
                    let xb4 = uint(b) * nb4 + cx4
                    for [unroll_full] (u in range(4)) {
                        acc[b] += x[xb4 + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var sm = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

class MetalKqLabK6ExtV8 {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k6_extv8_msl")]
    def lab_k6_extv8 {
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
                    let qv = ((qu[u] >> shr) & 0x0F0F0F0F) | (((hu[u] >> hsh) & 0x03030303) << 4u)
                    w4[u] = float4(float(qv & 255u), float((qv >> 8u) & 255u),
                        float((qv >> 16u) & 255u), float((qv >> 24u) & 255u)) * dl - float4(ml)
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                for [unroll_full] (b in range(8)) {
                    let xb4 = uint(b) * nb4 + cx4
                    for [unroll_full] (u in range(4)) {
                        acc[b] += x[xb4 + uint(u)] * w4[u]
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
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = sm
            }
        }
    }
}

// k5 v-form: the hoisted qh uints (shared by both chunks) + both chunks' qs uints batch ahead;
// the 5th-bit overlay folds into the uint domain before the float4 conversion.
class MetalKqLabK5ExtV4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_extv4_msl")]
    def lab_k5_extv4 {
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
        var acc : float4[4]
        var sb = 0u
        while (sb < nb) {
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
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(4)) {
                    let xb4 = uint(b) * nb4 + cx4
                    for [unroll_full] (u in range(4)) {
                        acc[b] += x[xb4 + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalKqLabK5ExtV8 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="lab_k5_extv8_msl")]
    def lab_k5_extv8 {
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
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(8)) {
                    let xb4 = uint(b) * nb4 + cx4
                    for [unroll_full] (u in range(4)) {
                        acc[b] += x[xb4 + uint(u)] * w4[u]
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
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// extx = the v form with the x panel STAGED through tgmem per superblock (cooperative 64-thread
// load, 2 barriers/round): the kq lane geometry's x reads are SCATTERED within the superblock
// (unlike q8's clean stripes, where the in-graph verdict kept streaming) — staging turns 16
// scattered device loads per chunk-column into tgmem reads. B=4 panel = 4KB, B=8 = 8KB.
class MetalKqLabK4ExtX4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @workgroup txp : float4[256]            // [4 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="lab_k4_extx4_msl")]
    def lab_k4_extx4 {
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
        var acc : float4[4]
        var sb = 0u
        while (sb < nb) {
            barrier()
            for [unroll_full] (j in range(4)) {
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
                for [unroll_full] (b in range(4)) {
                    for [unroll_full] (u in range(4)) {
                        acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

class MetalKqLabK4ExtX8 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @workgroup txp : float4[512]            // [8 streams x 64 float4]

    [metal_kernel(name="lab_k4_extx8_msl")]
    def lab_k4_extx8 {
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
            if (tx == 0u && row < ddim) {
                y[uint(b) * ddim + row] = s
            }
        }
    }
}

// ===== the KqMv B2 colbase A/B (reification followup) =====
// The reification sweep left KqMv B2/B4 unmerged: B2's body omits B4's column-tiling term (its
// dispatch pins gcols = 1), so a width-stamped template puts always-zero colbase math on the
// B=2 decode GEMV. These arms measure exactly that: kq_mvb2_* = the production B2 MSL verbatim
// (required, zero copy drift); kq_mvb2u_* = the text the template would stamp at NR=2 — B4's
// colbase form at width 2. Production binding contract (KqMvArgs uniform at 5), tg 64,
// grid ceil(d/8) x 1 — wgid.y is always 0, so colbase folds to 0 at runtime, never in the ISA.
// The *h twins are the HOISTED shared form (colrow = colbase * nb4 once per thread, cb4 =
// colrow + cx4 once per chunk, xb4 = cb4 + b * nb4): production folds uint(b) * nb4 at unroll
// time, so the naive colbase form leaves a runtime mul per b-iteration — the hoist reduces the
// delta to one add per chunk. It rewrites BOTH widths, so B4 gets its own prod/hoisted arms
// (kq_mvb4_* / kq_mvb4h_*, nr = 4 args, gcols still 1 — the nlive == 4 dispatch).
// The *s twins are the STATIC_IF-stamp preview: a TILED template constant branch-duplicating
// only the b-loop and writeback tail emits production text for both widths EXCEPT one dead
// `colbase` decl at B2 (the emitter keeps unused lets; the tail's use site forces a body-top
// decl). These arms = production B2 text + that dead line — the only delta the route leaves.

class MetalKqMvLabB2K4U {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb2u_k4_msl")]
    def lab_kq_mvb2u_k4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 2u
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ka.ndim / 4u
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
        for [unroll_full] (b in range(2)) {
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

class MetalKqMvLabB2K5U {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb2u_k5_msl")]
    def lab_kq_mvb2u_k5 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 2u
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ka.ndim / 4u
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
        for [unroll_full] (b in range(2)) {
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

class MetalKqMvLabB2K6U {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb2u_k6_msl")]
    def lab_kq_mvb2u_k6 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 2u
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ka.ndim / 4u
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
        for [unroll_full] (b in range(2)) {
            var sm = sumf[b]
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                y[(colbase + uint(b)) * ka.ys + row] = sm
            }
        }
    }
}

class MetalKqMvLabB2K4H {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb2h_k4_msl")]
    def lab_kq_mvb2h_k4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 2u
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ka.ndim / 4u
        let colrow = colbase * nb4
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
                let cb4 = colrow + cx4
                for [unroll_full] (b in range(2)) {
                    let xb4 = cb4 + uint(b) * nb4
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
            if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                y[(colbase + uint(b)) * ka.ys + row] = s
            }
        }
    }
}

class MetalKqMvLabB4K4H {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb4h_k4_msl")]
    def lab_kq_mvb4h_k4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 4u
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ka.ndim / 4u
        let colrow = colbase * nb4
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
                let cb4 = colrow + cx4
                for [unroll_full] (b in range(4)) {
                    let xb4 = cb4 + uint(b) * nb4
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
            if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                y[(colbase + uint(b)) * ka.ys + row] = s
            }
        }
    }
}

class MetalKqMvLabB2K6H {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb2h_k6_msl")]
    def lab_kq_mvb2h_k6 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 2u
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ka.ndim / 4u
        let colrow = colbase * nb4
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
                for [unroll_full] (u in range(4)) {
                    let qu = kqu[qb + uint(u)]
                    let hu = kqu[hb + uint(u)]
                    let qv = ((qu >> shr) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                    for [unroll_full] (e in range(4)) {
                        w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                    }
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                let cb4 = colrow + cx4
                for [unroll_full] (b in range(2)) {
                    let xb4 = cb4 + uint(b) * nb4
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
            if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                y[(colbase + uint(b)) * ka.ys + row] = sm
            }
        }
    }
}

class MetalKqMvLabB4K6H {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb4h_k6_msl")]
    def lab_kq_mvb4h_k6 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 4u
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ka.ndim / 4u
        let colrow = colbase * nb4
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
                let cb4 = colrow + cx4
                for [unroll_full] (b in range(4)) {
                    let xb4 = cb4 + uint(b) * nb4
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
            if (tx == 0u && row < ka.ddim && colbase + uint(b) < ka.nr) {
                y[(colbase + uint(b)) * ka.ys + row] = sm
            }
        }
    }
}

class MetalKqMvLabB2K4S {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb2s_k4_msl")]
    def lab_kq_mvb2s_k4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 2u   // nolint:LINT002 deliberately DEAD — the static_if stamp's residue
        let nb = ka.ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ka.ndim / 4u
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
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = s
            }
        }
    }
}

class MetalKqMvLabB2K6S {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : KqMvArgs

    [metal_kernel(name="lab_kq_mvb2s_k6_msl")]
    def lab_kq_mvb2s_k6 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 2u   // nolint:LINT002 deliberately DEAD — the static_if stamp's residue
        let nb = ka.ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ka.ndim / 4u
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
            if (tx == 0u && row < ka.ddim && uint(b) < ka.nr) {
                y[uint(b) * ka.ys + row] = sm
            }
        }
    }
}

// ===== driver (Apple-only; every Metal-typed helper is a generic so non-Apple never compiles it) =====

struct private LabVariant {
    name : string
    src : string
    entry : string
    b_list : array<int>                     // batch sizes this kernel serves
    q8x : bool = false                      // b2/b3 get the quantized activation planes
    dyn_b : bool = false                    // b6 gets the bcount uniform
    xt : bool = false                       // b2 gets the stream-minor [n][16] f32 panel
    gemm : bool = false                     // the production 32x32 GEMM, M padded to 32 (q8 X)
    gemm_fx : bool = false                  // f32-X GEMM (default buffer list, gemm dispatch)
    gemm64_fx : bool = false                // f32-X GEMM, 64-wide N tile (d/64 grid)
    gemm_sk : bool = false                  // split-K f32-X GEMM (partial planes + reduce pass)
    sk : int = 0                            // slice count for gemm_sk
    tgmem : uint64 = 0ul                    // dynamic [[threadgroup(0)]] bytes (@workgroup kernels)
    kq : int = 0                            // K-quant plane arm: 4 = k4 (b0/b1 k4s dual view, b2 k4q)
    kq_ggml : bool = false                  // K-quant lcpp wall arm (b0/b1 = ggml interleaved stream)
    kq16 : bool = false                     // kq arm reads the 16B compact scale blocks instead
    kq_mvb : bool = false                   // production small-batch KqMv contract (KqMvArgs uniform at 5)
    tgsz : int = 128                        // threads per threadgroup (kq arms ride lcpp's 64)
    rtg : int = 4                           // output rows per threadgroup (grid.x = ceil(d/rtg))
}

// exact-arithmetic Q8 fill: quants in [-8, 7], pow2 scales (same contract as the gemm lab)
def private fill_exact_q8(var q : array<int8>; var s : array<float>; rows, k : int64; seed : int64) {
    let nb = k / 32l
    q |> resize(rows * k)
    s |> resize(rows * nb)
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    for (i in range64(rows * k)) {
        q[i] = int8(int((i * seed + i / 31l) % 16l) - 8)
    }
    for (i in range64(rows * nb)) {
        s[i] = lut[int((i / 7l) % 4l)]
    }
}

// CPU reference, sampled: first `refd` output rows of every stream — every partial sum exact
def private cpu_ref_sample(wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>;
                           n, nb_streams, refd : int64) : array<float> {
    var y : array<float>
    y |> resize(nb_streams * refd)
    let nb = n / 32l
    for (b in range64(nb_streams)) {
        for (j in range64(refd)) {
            var acc = 0.0
            for (kb in range64(nb)) {
                var blk = 0.0
                for (c in range64(32l)) {
                    blk += float(wq[j * n + kb * 32l + c]) * float(xq[b * n + kb * 32l + c])
                }
                acc += blk * (ws[j * nb + kb] * xs[b * nb + kb])
            }
            y[b * refd + j] = acc
        }
    }
    return <- y
}

// exact-arithmetic Q4_K fill: sub-scales/mins in [0,7], pow2 d/dmin, nibbles 0..15 — every
// product is an exact int x pow2 (the fill_exact_q8 contract). Emits BOTH weight layouts from
// one logical stream: the production plane pair (kq 128B qs + ks 20B scale blocks) and the ggml
// interleaved block_q4_K stream (gg 144B) for the lcpp wall arm.
def private fill_exact_k4(var kq : array<uint8>; var ks : array<uint8>; var gg : array<uint8>; rows, k : int64; seed : int64) {
    let nsb = k / 256l
    let total = rows * nsb
    kq |> resize(total * 128l)
    ks |> resize(total * 20l)
    gg |> resize(total * 144l)
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    for (sb in range64(total)) {
        let db = f32_to_f16(lut[int((sb / 5l) % 4l)])
        let mb = f32_to_f16(lut[int((sb / 3l + 1l) % 4l)])
        let ko = sb * 20l
        let go = sb * 144l
        ks[ko] = uint8(db & 255u)
        ks[ko + 1l] = uint8(db >> 8u)
        ks[ko + 2l] = uint8(mb & 255u)
        ks[ko + 3l] = uint8(mb >> 8u)
        gg[go] = ks[ko]
        gg[go + 1l] = ks[ko + 1l]
        gg[go + 2l] = ks[ko + 2l]
        gg[go + 3l] = ks[ko + 3l]
        for (j in range64(4l)) {   // ggml scales[12]: [0,7] values keep the packed high bits zero
            let scl = int((sb + j * 5l + seed) % 8l)
            let mnl = int((sb * 3l + j * 7l + 1l) % 8l)
            let sch = int((sb + (j + 4l) * 5l + seed) % 8l)
            let mnh = int((sb * 3l + (j + 4l) * 7l + 1l) % 8l)
            ks[ko + 4l + j] = uint8(scl)
            ks[ko + 8l + j] = uint8(mnl)
            ks[ko + 12l + j] = uint8(sch | (mnh << 4))
        }
        for (j in range64(4l)) {
            gg[go + 4l + j] = ks[ko + 4l + j]
            gg[go + 8l + j] = ks[ko + 8l + j]
            gg[go + 12l + j] = ks[ko + 12l + j]
        }
        ks[ko + 16l] = uint8(0)   // the transcode zeroes the 4 pad bytes
        ks[ko + 17l] = uint8(0)
        ks[ko + 18l] = uint8(0)
        ks[ko + 19l] = uint8(0)
        let qo = sb * 128l
        for (b in range64(128l)) {
            let lo = int((b * 7l + sb * 13l + seed) % 16l)
            let hi = int((b * 11l + sb * 5l + 3l) % 16l)
            kq[qo + b] = uint8(lo | (hi << 4))
            gg[go + 16l + b] = kq[qo + b]
        }
    }
}

// 6-bit sub-scale/min j of a 12-byte packed Q4_K scale block (ggml get_scale_min_k4, array form)
def private k4_ref_sc_mn(ks : array<uint8>; base, j : int64) : int2 {
    if (j < 4l) {
        return int2(int(ks[base + j]) & 63, int(ks[base + j + 4l]) & 63)
    }
    let sc = (int(ks[base + j + 4l]) & 15) | ((int(ks[base + j - 4l]) >> 6) << 4)
    let mn = (int(ks[base + j + 4l]) >> 4) | ((int(ks[base + j]) >> 6) << 4)
    return int2(sc, mn)
}

// CPU reference for the k4 arms, sampled like cpu_ref_sample: per-element w = d*sc*q - dmin*mn
// against the f32 panel — algebraically the kernels' grouped folds, identical bits since every
// intermediate is exact under the fill
def private cpu_ref_k4(kq : array<uint8>; ks : array<uint8>; xf : array<float>; n, nb_streams, refd : int64) : array<float> {
    var y : array<float>
    y |> resize(nb_streams * refd)
    let nsb = n / 256l
    for (b in range64(nb_streams)) {
        for (j in range64(refd)) {
            var acc = 0.0
            for (sb in range64(nsb)) {
                let ko = (j * nsb + sb) * 20l
                let qo = (j * nsb + sb) * 128l
                let dd = f16_to_f32(uint(ks[ko]) | (uint(ks[ko + 1l]) << 8u))
                let dm = f16_to_f32(uint(ks[ko + 2l]) | (uint(ks[ko + 3l]) << 8u))
                for (js in range64(8l)) {
                    let s = k4_ref_sc_mn(ks, ko + 4l, js)
                    let ds = dd * float(s.x)
                    let dmn = dm * float(s.y)
                    for (l in range64(32l)) {
                        let qv = int(kq[qo + (js / 2l) * 32l + l])
                        let q = (js & 1l) == 0l ? (qv & 15) : (qv >> 4)
                        acc += (ds * float(q) - dmn) * xf[b * n + sb * 256l + js * 32l + l]
                    }
                }
            }
            y[b * refd + j] = acc
        }
    }
    return <- y
}

// exact-arithmetic Q5_K fill: the Q4_K scale/min construction + full-random qh bytes (bit s of
// qh[l] = the +16 bit of sub-block s at position l). Emits the production plane ([qs 128][qh 32])
// with 16B COMPACT scale blocks (the upload form) and the ggml interleaved block_q5_K stream
// ([d][dmin][scales 12][qh 32][qs 128], 176B).
def private fill_exact_k5(var kq : array<uint8>; var kc : array<uint8>; var gg : array<uint8>; rows, k : int64; seed : int64) {
    let nsb = k / 256l
    let total = rows * nsb
    kq |> resize(total * 160l)
    kc |> resize(total * 16l)
    gg |> resize(total * 176l)
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    for (sb in range64(total)) {
        let db = f32_to_f16(lut[int((sb / 5l) % 4l)])
        let mb = f32_to_f16(lut[int((sb / 3l + 1l) % 4l)])
        let co = sb * 16l
        let go = sb * 176l
        kc[co] = uint8(db & 255u)
        kc[co + 1l] = uint8(db >> 8u)
        kc[co + 2l] = uint8(mb & 255u)
        kc[co + 3l] = uint8(mb >> 8u)
        for (j in range64(4l)) {
            let scl = int((sb + j * 5l + seed) % 8l)
            let mnl = int((sb * 3l + j * 7l + 1l) % 8l)
            let sch = int((sb + (j + 4l) * 5l + seed) % 8l)
            let mnh = int((sb * 3l + (j + 4l) * 7l + 1l) % 8l)
            kc[co + 4l + j] = uint8(scl)
            kc[co + 8l + j] = uint8(mnl)
            kc[co + 12l + j] = uint8(sch | (mnh << 4))
        }
        for (j in range64(16l)) {
            gg[go + j] = kc[co + j]
        }
        let qo = sb * 160l
        for (b in range64(128l)) {   // qs nibbles
            let lo = int((b * 7l + sb * 13l + seed) % 16l)
            let hi = int((b * 11l + sb * 5l + 3l) % 16l)
            kq[qo + b] = uint8(lo | (hi << 4))
            gg[go + 48l + b] = kq[qo + b]
        }
        for (b in range64(32l)) {   // qh bytes
            let h = int((b * 29l + sb * 17l + seed) % 256l)
            kq[qo + 128l + b] = uint8(h)
            gg[go + 16l + b] = uint8(h)
        }
    }
}

// exact-arithmetic Q6_K fill: signed sub-scales in [-7,7], pow2 d, random ql/qh streams. Emits
// the production quant plane ([ql 128][qh 64]) with the SPLIT GPU scale planes (16B sub-scale
// blocks + packed f16 d plane) and the ggml interleaved block_q6_K stream
// ([ql 128][qh 64][scales 16][d], 210B).
def private fill_exact_k6(var kq : array<uint8>; var ksc : array<uint8>; var kd : array<uint8>; var gg : array<uint8>; rows, k : int64; seed : int64) {
    let nsb = k / 256l
    let total = rows * nsb
    kq |> resize(total * 192l)
    ksc |> resize(total * 16l)
    kd |> resize(total * 2l)
    gg |> resize(total * 210l)
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    for (sb in range64(total)) {
        let db = f32_to_f16(lut[int((sb / 5l) % 4l)])
        kd[sb * 2l] = uint8(db & 255u)
        kd[sb * 2l + 1l] = uint8(db >> 8u)
        let go = sb * 210l
        gg[go + 208l] = kd[sb * 2l]
        gg[go + 209l] = kd[sb * 2l + 1l]
        for (j in range64(16l)) {
            let sc = int((sb * 3l + j * 5l + seed) % 15l) - 7   // [-7, 7]
            ksc[sb * 16l + j] = uint8(sc & 255)
            gg[go + 192l + j] = ksc[sb * 16l + j]
        }
        let qo = sb * 192l
        for (b in range64(192l)) {   // ql then qh, disk order
            let v = int((b * 7l + sb * 13l + seed) % 256l)
            kq[qo + b] = uint8(v)
            gg[go + b] = uint8(v)
        }
    }
}

// CPU reference for the k5 arms: q = nibble | (qh bit s << 4), w = d*sc*q - dmin*mn
def private cpu_ref_k5(kq : array<uint8>; kc : array<uint8>; xf : array<float>; n, nb_streams, refd : int64) : array<float> {
    var y : array<float>
    y |> resize(nb_streams * refd)
    let nsb = n / 256l
    for (b in range64(nb_streams)) {
        for (j in range64(refd)) {
            var acc = 0.0
            for (sb in range64(nsb)) {
                let co = (j * nsb + sb) * 16l
                let qo = (j * nsb + sb) * 160l
                let dd = f16_to_f32(uint(kc[co]) | (uint(kc[co + 1l]) << 8u))
                let dm = f16_to_f32(uint(kc[co + 2l]) | (uint(kc[co + 3l]) << 8u))
                for (js in range64(8l)) {
                    let s = k4_ref_sc_mn(kc, co + 4l, js)
                    let ds = dd * float(s.x)
                    let dmn = dm * float(s.y)
                    for (l in range64(32l)) {
                        let qv = int(kq[qo + (js / 2l) * 32l + l])
                        let nib = (js & 1l) == 0l ? (qv & 15) : (qv >> 4)
                        let hb = (int(kq[qo + 128l + l]) >> int(js)) & 1
                        acc += (ds * float(nib | (hb << 4)) - dmn) * xf[b * n + sb * 256l + js * 32l + l]
                    }
                }
            }
            y[b * refd + j] = acc
        }
    }
    return <- y
}

// CPU reference for the k6 arms: q = ql nibble | 2 qh bits, w = d * sc16 * (q - 32)
def private cpu_ref_k6(kq : array<uint8>; ksc : array<uint8>; kd : array<uint8>; xf : array<float>; n, nb_streams, refd : int64) : array<float> {
    var y : array<float>
    y |> resize(nb_streams * refd)
    let nsb = n / 256l
    for (b in range64(nb_streams)) {
        for (j in range64(refd)) {
            var acc = 0.0
            for (sb in range64(nsb)) {
                let so = (j * nsb + sb) * 16l
                let qo = (j * nsb + sb) * 192l
                let dd = f16_to_f32(uint(kd[(j * nsb + sb) * 2l]) | (uint(kd[(j * nsb + sb) * 2l + 1l]) << 8u))
                for (g in range64(8l)) {   // 32-elem group g: half = g/4, within-half group g%4
                    let half = g / 4l
                    let gg = g % 4l
                    let qlb = qo + half * 64l + (gg & 1l) * 32l
                    let qhb = qo + 128l + half * 32l
                    let sh = gg < 2l ? 0 : 4
                    let hb = int(gg) * 2
                    for (l in range64(32l)) {
                        let q6 = ((int(kq[qlb + l]) >> sh) & 15) | (((int(kq[qhb + l]) >> hb) & 3) << 4)
                        let sci = so + half * 8l + gg * 2l + l / 16l
                        let sc = (int(ksc[sci]) ^ 128) - 128
                        acc += dd * float(sc) * float(q6 - 32) * xf[b * n + sb * 256l + half * 128l + gg * 32l + l]
                    }
                }
            }
            y[b * refd + j] = acc
        }
    }
    return <- y
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

// KqMvArgs uniform for the kq_mvb arms: ys = ddim (the lab's y layout), nr = the arm's batch width
def private upload_kqmv_args(dev; n, d, nr : uint) {
    var buf = metal_new_buffer(dev, 16ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = n
    unsafe(p[1]) = d
    unsafe(p[2]) = d
    unsafe(p[3]) = nr
    return buf
}

def private download_floats(buf; var dst : array<float>; count : int64) {
    dst |> resize(count)
    unsafe {
        memcpy(addr < void? >(dst[0]), metal_buffer_contents(buf), count * 4l)
    }
}

def private compile_lab_pso(dev; v : LabVariant) {
    var err : string
    var pso = pipeline_from_source(dev, v.src, v.entry, true, err)
    if (pso == null) {
        to_log(LOG_ERROR, "{v.name}: kernel compile failed — {err}\n")
    }
    return pso
}

// one timed encoder: set state once, dispatch `reps` back-to-back (hazard tracking on the shared
// y buffer serializes them); returns per-dispatch GPU milliseconds, or -1 on failure
def private time_reps(queue; pso; bufs; groups : uint3; reps : int; tgmem : uint64; tgsz : int = 128) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        for (bi in range(length(bufs))) {
            metal_set_buffer(enc, bufs[bi], 0ul, bi)
        }
        if (tgmem > 0ul) {
            metal_set_threadgroup_memory_length(enc, tgmem, 0)
        }
        for (_i in range(reps)) {
            metal_dispatch_threadgroups(enc, groups, uint3(uint(tgsz), 1u, 1u))
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms / double(reps)
}

// split-K timing: each rep = the partial GEMM then the reduce, same serial encoder
def private time_reps_sk(queue; pso_g; bufs_g; groups_g : uint3; tgmem : uint64;
                         pso_r; bufs_r; groups_r : uint3; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        for (_i in range(reps)) {
            metal_set_pipeline(enc, pso_g)
            for (bi in range(length(bufs_g))) {
                metal_set_buffer(enc, bufs_g[bi], 0ul, bi)
            }
            if (tgmem > 0ul) {
                metal_set_threadgroup_memory_length(enc, tgmem, 0)
            }
            metal_dispatch_threadgroups(enc, groups_g, uint3(128u, 1u, 1u))
            metal_set_pipeline(enc, pso_r)
            for (bi in range(length(bufs_r))) {
                metal_set_buffer(enc, bufs_r[bi], 0ul, bi)
            }
            metal_dispatch_threadgroups(enc, groups_r, uint3(128u, 1u, 1u))
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "sk dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms / double(reps)
}

def private want(filter, name : string) : bool {
    return empty(filter) || find(filter, name) >= 0
}

def private sk_idx(sv : int) : int => sv == 2 ? 0 : (sv == 4 ? 1 : 2)

// per-variant buffer list in binding order — generic (untyped params) so non-Apple never compiles it
// per-output-row weight bytes for the wGB/s report: q8 planar = n + n/8; kq = superblocks x
// (quant + scale block) — k4 144B compact / 148B with the padded 20B plane, k5 176B, k6 210B
def private kq_wrow_bytes(v : LabVariant; n : int64) : int64 {
    if (v.kq == 5) {
        return (n / 256l) * 176l
    }
    if (v.kq == 6) {
        return (n / 256l) * 210l
    }
    if (v.kq == 4) {
        return (n / 256l) * (v.kq16 || v.kq_ggml ? 144l : 148l)
    }
    return n + n / 8l
}

// kq buffer-array slots (built per shape in run_lab, indexed by the make_bufs kq branches)
let KQA_Q4 = 0
let KQA_S4 = 1
let KQA_G4 = 2
let KQA_C4 = 3
let KQA_Q5 = 4
let KQA_C5 = 5
let KQA_G5 = 6
let KQA_Q6 = 7
let KQA_SC6 = 8
let KQA_D6 = 9
let KQA_G6 = 10
let KQA_MVARGS = 11
let KQA_MVARGS4 = 12

def private make_bufs(v : LabVariant; var wbuf; var sbuf; var xfbuf; var xtbuf; var xqbuf; var xsbuf; var ybuf; var ndimbuf; var ddimbuf; var bbuf; var partbuf; var skbuf; var kqarr) {
    if (v.kq_mvb) {    // production KqMv contract: plane views 0-2, x, y, KqMvArgs uniform at 5
        var argsbuf = v.b_list[0] == 4 ? kqarr[KQA_MVARGS4] : kqarr[KQA_MVARGS]
        if (v.kq == 5) {
            return <- [kqarr[KQA_C5], kqarr[KQA_C5], kqarr[KQA_Q5], xfbuf, ybuf, argsbuf]
        }
        if (v.kq == 6) {
            return <- [kqarr[KQA_D6], kqarr[KQA_SC6], kqarr[KQA_Q6], xfbuf, ybuf, argsbuf]
        }
        return <- [kqarr[KQA_C4], kqarr[KQA_C4], kqarr[KQA_Q4], xfbuf, ybuf, argsbuf]
    }
    if (v.kq_ggml) {   // interleaved stream bound per view (f16 / uint / byte where the arm needs it)
        if (v.kq == 5) {
            return <- [kqarr[KQA_G5], kqarr[KQA_G5], kqarr[KQA_G5], xfbuf, ybuf, ndimbuf, ddimbuf]
        }
        if (v.kq == 6) {
            return <- [kqarr[KQA_G6], kqarr[KQA_G6], xfbuf, ybuf, ndimbuf, ddimbuf]
        }
        return <- [kqarr[KQA_G4], kqarr[KQA_G4], xfbuf, ybuf, ndimbuf, ddimbuf]
    }
    if (v.kq == 5) {   // 16B compact scale blocks (f16 + uint4 views) + the quant plane
        return <- [kqarr[KQA_C5], kqarr[KQA_C5], kqarr[KQA_Q5], xfbuf, ybuf, ndimbuf, ddimbuf]
    }
    if (v.kq == 6) {   // split scale planes: f16 d plane + 16B sub-scale blocks + the quant plane
        return <- [kqarr[KQA_D6], kqarr[KQA_SC6], kqarr[KQA_Q6], xfbuf, ybuf, ndimbuf, ddimbuf]
    }
    if (v.kq16) {      // k4: compact scale blocks bound twice (f16 + uint4 views) + the quant plane
        return <- [kqarr[KQA_C4], kqarr[KQA_C4], kqarr[KQA_Q4], xfbuf, ybuf, ndimbuf, ddimbuf]
    }
    if (v.kq != 0) {   // k4 negative control: the production 20B scale plane
        return <- [kqarr[KQA_S4], kqarr[KQA_S4], kqarr[KQA_Q4], xfbuf, ybuf, ndimbuf, ddimbuf]
    }
    if (v.gemm_sk) {
        return <- [wbuf, sbuf, xfbuf, partbuf, ndimbuf, ddimbuf, skbuf]
    }
    if (v.q8x || v.gemm) {
        return <- [wbuf, sbuf, xqbuf, xsbuf, ybuf, ndimbuf, ddimbuf]
    }
    if (v.dyn_b) {
        return <- [wbuf, sbuf, xfbuf, ybuf, ndimbuf, ddimbuf, bbuf]
    }
    if (v.xt) {
        return <- [wbuf, sbuf, xtbuf, ybuf, ndimbuf, ddimbuf]
    }
    return <- [wbuf, sbuf, xfbuf, ybuf, ndimbuf, ddimbuf]
}

def private count_mismatch_sampled(got : array<float>; ref_v : array<float>; d, nb_streams, refd : int64;
                                   var firstBad : int64&) : int64 {
    var bad = 0l
    firstBad = -1l
    for (b in range64(nb_streams)) {
        for (j in range64(refd)) {
            if (got[b * d + j] != ref_v[b * refd + j]) {
                if (firstBad < 0l) {
                    firstBad = b * d + j
                }
                bad++
            }
        }
    }
    return bad
}

def private run_lab(b : B?; variants) {   // nolint:STYLE037,STYLE038 — the lab A/B ladder: per-variant compile/dispatch/measure arms are one flow
    let rounds = g_env_bench.lab_rounds
    let refd = 128l
    let bmax = 16l
    var dev = metal_create_system_default_device()
    if (dev == null) {
        to_log(LOG_WARNING, "skipped: no Metal device\n")
        return
    }
    to_log(LOG_INFO, "device={metal_device_name(dev)} rounds={rounds}\n")
    var queue = metal_new_command_queue(dev)
    var psos <- [for (v in variants); compile_lab_pso(dev, v)]
    var err_red : string
    var pso_red = pipeline_from_source(dev, lab_sk_reduce_msl, lab_sk_reduce_msl_entry, lab_sk_reduce_msl_fastmath, err_red)
    for (v, pso in variants, psos) {
        b |> success(pso != null, "pipeline compiles: {v.name}")
        if (pso != null && want(g_env_bench.lab_variants, v.name)) {
            to_log(LOG_INFO, "{v.name}: max_threads={metal_pipeline_max_total_threads(pso)} simd_width={metal_pipeline_thread_execution_width(pso)}\n")
        }
    }
    // pre-built B uniforms for the dynamic-B variant
    var bbufs <- [for (bi in range(5)); upload_u32(dev, uint(1 << bi))]
    // the llama-3B decode GEMV geometries (n = reduction dim, d = output rows) + the qwen3-4B
    // set (dim 2560 / qd 4096 / hidden 9728 — qd != dim splits the q and wo shapes) + the
    // gemma4-12B set (dim 3840 / hidden 15360; qkv = the sliding class 4096+2048+2048, wogl =
    // the global class's qd-8192 reduction — the fattest n the batch loop dispatches)
    var shapes <- [("q3b", 3072l, 3072l), ("kv3b", 3072l, 1024l), ("qkv3b", 3072l, 5120l), ("w13_3b", 3072l, 8192l),
                   ("w2_3b", 8192l, 3072l), ("cls3b", 3072l, 128256l),
                   ("qkv4b", 2560l, 6144l), ("wo4b", 4096l, 2560l), ("w13_4b", 2560l, 9728l),
                   ("w2_4b", 9728l, 2560l), ("cls4b", 2560l, 151936l),
                   ("qkv12b", 3840l, 8192l), ("wogl12b", 8192l, 3840l), ("w13_12b", 3840l, 15360l),
                   ("w2_12b", 15360l, 3840l)]
    for (sh in shapes) {
        if (!want(g_env_bench.lab_shapes, sh._0)) {
            continue
        }
        let n = sh._1
        let d = sh._2
        let nb = n / 32l
        let xrows = 32l                      // the GEMM row is M-padded to 32; streams 16-31 are pad
        let t_shape0 = ref_time_ticks()
        var wq : array<int8>
        var ws : array<float>
        var xq : array<int8>
        var xs : array<float>
        fill_exact_q8(wq, ws, d, n, 7l)
        fill_exact_q8(xq, xs, xrows, n, 11l)
        var yref <- cpu_ref_sample(wq, ws, xq, xs, n, bmax, refd)
        // the f32 activation panel — exact dequant of xq/xs (int x pow2); full 32 M-pad rows
        // (the f32-X GEMM reads all of them — pad outputs land in unchecked rows)
        var xf : array<float>
        xf |> resize(int(xrows * n))
        for (i in range64(xrows * n)) {
            xf[i] = float(xq[i]) * xs[(i / n) * nb + (i % n) / 32l]
        }
        // the stream-minor transpose (stride 16) for the xt variants
        var xft : array<float>
        xft |> resize(int(n * bmax))
        for (i in range64(bmax * n)) {
            xft[(i % n) * bmax + i / n] = xf[i]
        }
        // the k4 arms' weight streams (built only when a kq arm survives the filter — the cls
        // shape's three extra layouts are ~450MB of host fill otherwise wasted)
        var anyk4 = false
        var anyk5 = false
        var anyk6 = false
        var anymvb = false
        for (v in variants) {
            let on = want(g_env_bench.lab_variants, v.name)
            anyk4 ||= v.kq == 4 && on
            anyk5 ||= v.kq == 5 && on
            anyk6 ||= v.kq == 6 && on
            anymvb ||= v.kq_mvb && on
        }
        var k4q_h : array<uint8>
        var k4s_h : array<uint8>
        var k4g_h : array<uint8>
        var k4c_h : array<uint8>
        var yref_k4 : array<float>
        if (anyk4) {
            fill_exact_k4(k4q_h, k4s_h, k4g_h, d, n, 7l)
            yref_k4 <- cpu_ref_k4(k4q_h, k4s_h, xf, n, bmax, refd)   // nolint:PERF030 — first write to the empty decl above
            // the 16B compact scale plane = the 20B blocks with the pads stripped
            let nsb_all = (d * n) / 256l
            k4c_h |> resize(nsb_all * 16l)
            for (sb in range64(nsb_all)) {
                for (i in range64(16l)) {
                    k4c_h[sb * 16l + i] = k4s_h[sb * 20l + i]
                }
            }
        }
        var k5q_h : array<uint8>
        var k5c_h : array<uint8>
        var k5g_h : array<uint8>
        var yref_k5 : array<float>
        if (anyk5) {
            fill_exact_k5(k5q_h, k5c_h, k5g_h, d, n, 9l)
            yref_k5 <- cpu_ref_k5(k5q_h, k5c_h, xf, n, bmax, refd)   // nolint:PERF030 — first write to the empty decl above
        }
        var k6q_h : array<uint8>
        var k6sc_h : array<uint8>
        var k6d_h : array<uint8>
        var k6g_h : array<uint8>
        var yref_k6 : array<float>
        if (anyk6) {
            fill_exact_k6(k6q_h, k6sc_h, k6d_h, k6g_h, d, n, 11l)
            yref_k6 <- cpu_ref_k6(k6q_h, k6sc_h, k6d_h, xf, n, bmax, refd)   // nolint:PERF030 — first write to the empty decl above
        }
        var wbuf = upload_array(dev, wq, d * n)
        var sbuf = upload_array(dev, ws, d * nb * 4l)
        let nsball = (d * n) / 256l
        var kq4buf = anyk4 ? upload_array(dev, k4q_h, nsball * 128l) : null
        var ks4buf = anyk4 ? upload_array(dev, k4s_h, nsball * 20l) : null
        var kg4buf = anyk4 ? upload_array(dev, k4g_h, nsball * 144l) : null
        var kc4buf = anyk4 ? upload_array(dev, k4c_h, nsball * 16l) : null
        var kq5buf = anyk5 ? upload_array(dev, k5q_h, nsball * 160l) : null
        var kc5buf = anyk5 ? upload_array(dev, k5c_h, nsball * 16l) : null
        var kg5buf = anyk5 ? upload_array(dev, k5g_h, nsball * 176l) : null
        var kq6buf = anyk6 ? upload_array(dev, k6q_h, nsball * 192l) : null
        var ksc6buf = anyk6 ? upload_array(dev, k6sc_h, nsball * 16l) : null
        var kd6buf = anyk6 ? upload_array(dev, k6d_h, nsball * 2l) : null
        var kg6buf = anyk6 ? upload_array(dev, k6g_h, nsball * 210l) : null
        var kqmvargs = anymvb ? upload_kqmv_args(dev, uint(n), uint(d), 2u) : null
        var kqmvargs4 = anymvb ? upload_kqmv_args(dev, uint(n), uint(d), 4u) : null
        var kqarr <- [kq4buf, ks4buf, kg4buf, kc4buf, kq5buf, kc5buf, kg5buf, kq6buf, ksc6buf, kd6buf, kg6buf, kqmvargs, kqmvargs4]
        var xfbuf = upload_array(dev, xf, xrows * n * 4l)
        var xtbuf = upload_array(dev, xft, n * bmax * 4l)
        var xqbuf = upload_array(dev, xq, xrows * n)
        var xsbuf = upload_array(dev, xs, xrows * nb * 4l)
        var ybuf = metal_new_buffer(dev, uint64(xrows * d * 4l))
        var partbuf = metal_new_buffer(dev, uint64(8l * 32l * d * 4l))   // up to 8 split-K planes
        var totalbuf = upload_u32(dev, uint(32l * d))
        var skbufs <- [for (sv in [2, 4, 8]); upload_u32(dev, uint(sv))]
        var ndimbuf = upload_u32(dev, uint(n))
        var ddimbuf = upload_u32(dev, uint(d))
        let setup_ms = double(ref_time_ticks() - t_shape0) / 1000000.0lf
        to_log(LOG_INFO, "=== {sh._0} n={n} d={d} === (setup {setup_ms} ms)\n")
        let groups_gemm = uint3(uint(d / 32l), 1u, 1u)   // (32/32 token tiles) x (d/32 output tiles)
        let groups_gemm64 = uint3(uint(d / 64l), 1u, 1u)
        // flattened (variant, B) runs — interleaved timing across ALL runs neutralizes drift
        var run_v : array<int>
        var run_b : array<int>
        run_v |> reserve(length(variants) * 5)
        run_b |> reserve(length(variants) * 5)
        for (v, idx in variants, count()) {
            if (psos[idx] == null || !want(g_env_bench.lab_variants, v.name)) {
                continue
            }
            for (bn in v.b_list) {
                run_v |> push(idx)
                run_b |> push(bn)
            }
        }
        var alive : array<bool>
        var est_ms : array<double>
        var best : array<double>
        alive |> resize(length(run_v))
        est_ms |> resize(length(run_v))
        best |> resize(length(run_v))
        var ygot : array<float>
        for (vi, nbatch, alv, est, bst in run_v, run_b, alive, est_ms, best) {
            alv = false
            bst = 1.0e30lf
            var bufs <- make_bufs(variants[vi], wbuf, sbuf, xfbuf, xtbuf, xqbuf, xsbuf, ybuf, ndimbuf, ddimbuf, bbufs[find_bbuf(nbatch)], partbuf, skbufs[sk_idx(variants[vi].sk)], kqarr)
            var ms = 0.0lf   // warmup + correctness dispatch
            if (variants[vi].gemm_sk) {
                var rbufs <- [partbuf, ybuf, totalbuf, skbufs[sk_idx(variants[vi].sk)]]
                ms = time_reps_sk(queue, psos[vi], bufs, uint3(uint(d / 32l), uint(variants[vi].sk), 1u), variants[vi].tgmem,
                    pso_red, rbufs, uint3(uint((32l * d + 127l) / 128l), 1u, 1u), 1)
                rbufs |> clear()
                unsafe {
                    delete rbufs
                }
            } else {
                let vgroups = uint3(uint((d + int64(variants[vi].rtg) - 1l) / int64(variants[vi].rtg)), 1u, 1u)
                ms = time_reps(queue, psos[vi], bufs, variants[vi].gemm64_fx ? groups_gemm64 : ((variants[vi].gemm || variants[vi].gemm_fx) ? groups_gemm : vgroups), 1, variants[vi].tgmem, variants[vi].tgsz)
            }
            bufs |> clear()   // borrowed handles — delete of array<T?> would free the pointees
            unsafe {
                delete bufs
            }
            if (ms < 0.0lf) {
                b |> success(false, "{variants[vi].name}/B{nbatch} {sh._0}: dispatch")
                continue
            }
            est = ms
            download_floats(ybuf, ygot, int64(nbatch) * d)
            var firstBad = -1l
            var bad = 0l
            if (variants[vi].kq == 4) {
                bad = count_mismatch_sampled(ygot, yref_k4, d, int64(nbatch), refd, firstBad)
            } elif (variants[vi].kq == 5) {
                bad = count_mismatch_sampled(ygot, yref_k5, d, int64(nbatch), refd, firstBad)
            } elif (variants[vi].kq == 6) {
                bad = count_mismatch_sampled(ygot, yref_k6, d, int64(nbatch), refd, firstBad)
            } else {
                bad = count_mismatch_sampled(ygot, yref, d, int64(nbatch), refd, firstBad)
            }
            b |> success(bad == 0l, "{variants[vi].name}/B{nbatch} {sh._0}: bit-exact vs CPU reference")
            if (bad != 0l) {
                to_log(LOG_ERROR, "{variants[vi].name}/B{nbatch} {sh._0}: {bad} mismatches, first at {firstBad}: got {ygot[firstBad]}\n")
            } else {
                alv = true
            }
        }
        // interleaved best-of-N timing rounds
        for (_round in range(rounds)) {
            for (vi, nbatch, alv, est, bst in run_v, run_b, alive, est_ms, best) {
                if (!alv) {
                    continue
                }
                var bufs <- make_bufs(variants[vi], wbuf, sbuf, xfbuf, xtbuf, xqbuf, xsbuf, ybuf, ndimbuf, ddimbuf, bbufs[find_bbuf(nbatch)], partbuf, skbufs[sk_idx(variants[vi].sk)], kqarr)
                let reps = clamp(int(25.0lf / max(est, 0.01lf)) + 1, 1, 128)
                var ms = 0.0lf
                if (variants[vi].gemm_sk) {
                    var rbufs <- [partbuf, ybuf, totalbuf, skbufs[sk_idx(variants[vi].sk)]]
                    ms = time_reps_sk(queue, psos[vi], bufs, uint3(uint(d / 32l), uint(variants[vi].sk), 1u), variants[vi].tgmem,
                        pso_red, rbufs, uint3(uint((32l * d + 127l) / 128l), 1u, 1u), reps)
                    rbufs |> clear()
                    unsafe {
                        delete rbufs
                    }
                } else {
                    let vgroups = uint3(uint((d + int64(variants[vi].rtg) - 1l) / int64(variants[vi].rtg)), 1u, 1u)
                    ms = time_reps(queue, psos[vi], bufs, variants[vi].gemm64_fx ? groups_gemm64 : ((variants[vi].gemm || variants[vi].gemm_fx) ? groups_gemm : vgroups), reps, variants[vi].tgmem, variants[vi].tgsz)
                }
                if (ms > 0.0lf && ms < bst) {
                    bst = ms
                }
                bufs |> clear()
                unsafe {
                    delete bufs
                }
            }
        }
        // weight traffic: q8 planar = n quant + n/8 scale bytes per output row; k4 = 148B
        // (planes, 128+20) or 144B (ggml interleaved) per 256 elems
        for (vi, nbatch, alv, bst in run_v, run_b, alive, best) {
            if (!alv) {
                continue
            }
            let wbytes = double(d * kq_wrow_bytes(variants[vi], n))
            let gbs = wbytes / (bst * 1.0e6lf)
            let gmacs = double(n * d * int64(nbatch)) / (bst * 1.0e6lf)
            to_log(LOG_INFO, "{variants[vi].name}/B{nbatch}  {sh._0} {bst} ms  {gbs} wGB/s  {gmacs} GMAC/s\n")
        }
        metal_release(wbuf)
        metal_release(sbuf)
        if (anyk4) {
            metal_release(kq4buf)
            metal_release(ks4buf)
            metal_release(kg4buf)
            metal_release(kc4buf)
        }
        if (anyk5) {
            metal_release(kq5buf)
            metal_release(kc5buf)
            metal_release(kg5buf)
        }
        if (anyk6) {
            metal_release(kq6buf)
            metal_release(ksc6buf)
            metal_release(kd6buf)
            metal_release(kg6buf)
        }
        if (anymvb) {
            metal_release(kqmvargs)
            metal_release(kqmvargs4)
        }
        kqarr |> clear()   // borrowed handles — released above
        unsafe {
            delete kqarr
        }
        metal_release(partbuf)
        metal_release(totalbuf)
        for (skb in skbufs) {
            metal_release(skb)
        }
        skbufs |> clear()   // released above — delete of array<T?> would double-free the pointees
        unsafe {
            delete skbufs
        }
        metal_release(xfbuf)
        metal_release(xtbuf)
        metal_release(xqbuf)
        metal_release(xsbuf)
        metal_release(ybuf)
        metal_release(ndimbuf)
        metal_release(ddimbuf)
        delete run_v
        delete run_b
        delete alive
        delete est_ms
        delete best
        delete ygot
        delete wq
        delete ws
        delete xq
        delete xs
        delete xf
        delete xft
        delete yref
        delete k4q_h
        delete k4s_h
        delete k4g_h
        delete k4c_h
        delete yref_k4
        delete k5q_h
        delete k5c_h
        delete k5g_h
        delete yref_k5
        delete k6q_h
        delete k6sc_h
        delete k6d_h
        delete k6g_h
        delete yref_k6
    }
    delete shapes
    for (bbuf in bbufs) {
        metal_release(bbuf)
    }
    bbufs |> clear()
    unsafe {
        delete bbufs
    }
    for (pso in psos) {
        if (pso != null) {
            metal_release(pso)
        }
    }
    if (pso_red != null) {
        metal_release(pso_red)
    }
    psos |> clear()
    unsafe {
        delete psos
    }
    metal_release(queue)
    metal_release(dev)
    assert_no_metal_leaks()
}

def private find_bbuf(nbatch : int) : int {
    var bi = 0
    var v = nbatch
    while (v > 1) {
        v /= 2
        bi++
    }
    return bi
}

[benchmark, unused_argument(b)]   // b is consumed only inside the Apple static_if half
def bench_metal_gemv_kernels(b : B?) {   // nolint:STYLE038 — flat variant registration table + the driver call
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var variants <- [
            LabVariant(name = "v0_gemv", src = metal_q8_gemv_msl, entry = metal_q8_gemv_msl_entry,
                       b_list <- [1]),
            LabVariant(name = "gemv_b2", src = lab_gemv_b2_msl, entry = lab_gemv_b2_msl_entry,
                       b_list <- [2]),
            LabVariant(name = "gemv_b4", src = lab_gemv_b4_msl, entry = lab_gemv_b4_msl_entry,
                       b_list <- [4]),
            LabVariant(name = "gemv_b8", src = lab_gemv_b8_msl, entry = lab_gemv_b8_msl_entry,
                       b_list <- [8]),
            LabVariant(name = "gemv_b16", src = lab_gemv_b16_msl, entry = lab_gemv_b16_msl_entry,
                       b_list <- [16]),
            LabVariant(name = "gemv_b2c", src = lab_gemv_b2c_msl, entry = lab_gemv_b2c_msl_entry,
                       b_list <- [2]),
            LabVariant(name = "gemv_b4c", src = lab_gemv_b4c_msl, entry = lab_gemv_b4c_msl_entry,
                       b_list <- [4]),
            LabVariant(name = "gemv_b2c8", src = lab_gemv_b2c8_msl, entry = lab_gemv_b2c8_msl_entry,
                       b_list <- [2]),
            LabVariant(name = "gemv_b4c8", src = lab_gemv_b4c8_msl, entry = lab_gemv_b4c8_msl_entry,
                       b_list <- [4]),
            LabVariant(name = "gemv_b4x", src = lab_gemv_b4x_msl, entry = lab_gemv_b4x_msl_entry,
                       b_list <- [4], tgmem = lab_gemv_b4x_msl_tgmem),
            LabVariant(name = "gemv_b2x", src = lab_gemv_b2x_msl, entry = lab_gemv_b2x_msl_entry,
                       b_list <- [2], tgmem = lab_gemv_b2x_msl_tgmem),
            LabVariant(name = "gemv_b8x", src = lab_gemv_b8x_msl, entry = lab_gemv_b8x_msl_entry,
                       b_list <- [8], tgmem = lab_gemv_b8x_msl_tgmem),
            LabVariant(name = "gemv_b16x", src = lab_gemv_b16x_msl, entry = lab_gemv_b16x_msl_entry,
                       b_list <- [16], tgmem = lab_gemv_b16x_msl_tgmem),
            LabVariant(name = "gemv_bdyn", src = lab_gemv_bdyn_msl, entry = lab_gemv_bdyn_msl_entry,
                       b_list <- [1, 2, 4, 8, 16], dyn_b = true),
            LabVariant(name = "gemv_q8x", src = lab_gemv_q8x_msl, entry = lab_gemv_q8x_msl_entry,
                       b_list <- [1], q8x = true),
            LabVariant(name = "gemv_b1", src = lab_gemv_b1_msl, entry = lab_gemv_b1_msl_entry,
                       b_list <- [1]),
            LabVariant(name = "q8_b1i", src = lab_q8_b1i_msl, entry = lab_q8_b1i_msl_entry,
                       b_list <- [1]),
            LabVariant(name = "q8_b1p", src = lab_q8_b1p_msl, entry = lab_q8_b1p_msl_entry,
                       b_list <- [1]),
            LabVariant(name = "gemv_b4t", src = lab_gemv_b4t_msl, entry = lab_gemv_b4t_msl_entry,
                       b_list <- [4], xt = true),
            LabVariant(name = "gemv_b8t", src = lab_gemv_b8t_msl, entry = lab_gemv_b8t_msl_entry,
                       b_list <- [8], xt = true),
            LabVariant(name = "gemv_b16t", src = lab_gemv_b16t_msl, entry = lab_gemv_b16t_msl_entry,
                       b_list <- [16], xt = true),
            LabVariant(name = "gemm32", src = metal_q8_gemm_msl, entry = metal_q8_gemm_msl_entry,
                       b_list <- [1, 2, 4, 8, 16], gemm = true, tgmem = metal_q8_gemm_msl_tgmem),
            LabVariant(name = "gemm_v2", src = lab_gemm_v2_msl, entry = lab_gemm_v2_msl_entry,
                       b_list <- [8, 16], gemm_fx = true, tgmem = lab_gemm_v2_msl_tgmem),
            LabVariant(name = "gemm64_v2", src = lab_gemm64_v2_msl, entry = lab_gemm64_v2_msl_entry,
                       b_list <- [8, 16], gemm64_fx = true, tgmem = lab_gemm64_v2_msl_tgmem),
            LabVariant(name = "gemm_sk2", src = lab_gemm_sk_msl, entry = lab_gemm_sk_msl_entry,
                       b_list <- [8], gemm_sk = true, sk = 2, tgmem = lab_gemm_sk_msl_tgmem),
            LabVariant(name = "gemm_sk4", src = lab_gemm_sk_msl, entry = lab_gemm_sk_msl_entry,
                       b_list <- [8], gemm_sk = true, sk = 4, tgmem = lab_gemm_sk_msl_tgmem),
            LabVariant(name = "gemm_sk8", src = lab_gemm_sk_msl, entry = lab_gemm_sk_msl_entry,
                       b_list <- [8], gemm_sk = true, sk = 8, tgmem = lab_gemm_sk_msl_tgmem),
            LabVariant(name = "k4_lcpp", src = lab_k4_lcpp_msl, entry = lab_k4_lcpp_msl_entry,
                       b_list <- [1], kq = 4, kq_ggml = true, tgsz = 64),
            LabVariant(name = "k4_b1", src = lab_k4_b1_msl, entry = lab_k4_b1_msl_entry,
                       b_list <- [1], kq = 4, tgsz = 64),
            LabVariant(name = "k4_b1c", src = lab_k4_b1c_msl, entry = lab_k4_b1c_msl_entry,
                       b_list <- [1], kq = 4, kq16 = true, tgsz = 64),
            LabVariant(name = "k5_lcpp", src = lab_k5_lcpp_msl, entry = lab_k5_lcpp_msl_entry,
                       b_list <- [1], kq = 5, kq_ggml = true, tgsz = 64, rtg = 2),
            LabVariant(name = "k5_b1c", src = lab_k5_b1c_msl, entry = lab_k5_b1c_msl_entry,
                       b_list <- [1], kq = 5, kq16 = true, tgsz = 64, rtg = 2),
            LabVariant(name = "k5_b1d", src = lab_k5_b1d_msl, entry = lab_k5_b1d_msl_entry,
                       b_list <- [1], kq = 5, kq16 = true, tgsz = 64, rtg = 2),
            LabVariant(name = "k5_b2r", src = lab_k5_b2r_msl, entry = lab_k5_b2r_msl_entry,
                       b_list <- [1], kq = 5, kq16 = true, tgsz = 64),
            LabVariant(name = "k6_lcpp", src = lab_k6_lcpp_msl, entry = lab_k6_lcpp_msl_entry,
                       b_list <- [1], kq = 6, kq_ggml = true, tgsz = 64),
            LabVariant(name = "k6_b1c", src = lab_k6_b1c_msl, entry = lab_k6_b1c_msl_entry,
                       b_list <- [1], kq = 6, kq16 = true, tgsz = 64),
            LabVariant(name = "k6_b1d", src = lab_k6_b1d_msl, entry = lab_k6_b1d_msl_entry,
                       b_list <- [1], kq = 6, kq16 = true, tgsz = 64),
            LabVariant(name = "k4_mvb2", src = lab_k4_mvb2_msl, entry = lab_k4_mvb2_msl_entry,
                       b_list <- [2], kq = 4, kq16 = true, tgsz = 64, rtg = 2),
            LabVariant(name = "k4_mvb2ctl", src = lab_k4_mvb2ctl_msl, entry = lab_k4_mvb2ctl_msl_entry,
                       b_list <- [1], kq = 4, kq16 = true, tgsz = 64, rtg = 2),
            LabVariant(name = "k5_mvb2", src = lab_k5_mvb2_msl, entry = lab_k5_mvb2_msl_entry,
                       b_list <- [2], kq = 5, kq16 = true, tgsz = 64, rtg = 2),
            LabVariant(name = "k6_mvb2", src = lab_k6_mvb2_msl, entry = lab_k6_mvb2_msl_entry,
                       b_list <- [2], kq = 6, kq16 = true, tgsz = 64, rtg = 2),
            LabVariant(name = "k4_ext2", src = lab_k4_ext2_msl, entry = lab_k4_ext2_msl_entry,
                       b_list <- [2], kq = 4, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k4_ext4", src = lab_k4_ext4_msl, entry = lab_k4_ext4_msl_entry,
                       b_list <- [4], kq = 4, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k6_ext2", src = lab_k6_ext2_msl, entry = lab_k6_ext2_msl_entry,
                       b_list <- [2], kq = 6, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k6_ext4", src = lab_k6_ext4_msl, entry = lab_k6_ext4_msl_entry,
                       b_list <- [4], kq = 6, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k5_ext2", src = lab_k5_ext2_msl, entry = lab_k5_ext2_msl_entry,
                       b_list <- [2], kq = 5, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k5_ext4", src = lab_k5_ext4_msl, entry = lab_k5_ext4_msl_entry,
                       b_list <- [4], kq = 5, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k6_ext2u", src = lab_k6_ext2u_msl, entry = lab_k6_ext2u_msl_entry,
                       b_list <- [2], kq = 6, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k6_ext4u", src = lab_k6_ext4u_msl, entry = lab_k6_ext4u_msl_entry,
                       b_list <- [4], kq = 6, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k4_extb4", src = lab_k4_extb4_msl, entry = lab_k4_extb4_msl_entry,
                       b_list <- [4], kq = 4, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k4_extv4", src = lab_k4_extv4_msl, entry = lab_k4_extv4_msl_entry,
                       b_list <- [4], kq = 4, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k4_extv8", src = lab_k4_extv8_msl, entry = lab_k4_extv8_msl_entry,
                       b_list <- [8], kq = 4, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k6_extv4", src = lab_k6_extv4_msl, entry = lab_k6_extv4_msl_entry,
                       b_list <- [4], kq = 6, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k6_extv8", src = lab_k6_extv8_msl, entry = lab_k6_extv8_msl_entry,
                       b_list <- [8], kq = 6, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k5_extv4", src = lab_k5_extv4_msl, entry = lab_k5_extv4_msl_entry,
                       b_list <- [4], kq = 5, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k5_extv8", src = lab_k5_extv8_msl, entry = lab_k5_extv8_msl_entry,
                       b_list <- [8], kq = 5, kq16 = true, tgsz = 64, rtg = 8),
            LabVariant(name = "k4_extx4", src = lab_k4_extx4_msl, entry = lab_k4_extx4_msl_entry,
                       b_list <- [4], kq = 4, kq16 = true, tgsz = 64, rtg = 8, tgmem = lab_k4_extx4_msl_tgmem),
            LabVariant(name = "k4_extx8", src = lab_k4_extx8_msl, entry = lab_k4_extx8_msl_entry,
                       b_list <- [8], kq = 4, kq16 = true, tgsz = 64, rtg = 8, tgmem = lab_k4_extx8_msl_tgmem),
            LabVariant(name = "kq_mvb2_k4", src = metal_kq_mvb2_k4_msl, entry = metal_kq_mvb2_k4_msl_entry,
                       b_list <- [2], kq = 4, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2u_k4", src = lab_kq_mvb2u_k4_msl, entry = lab_kq_mvb2u_k4_msl_entry,
                       b_list <- [2], kq = 4, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2_k5", src = metal_kq_mvb2_k5_msl, entry = metal_kq_mvb2_k5_msl_entry,
                       b_list <- [2], kq = 5, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2u_k5", src = lab_kq_mvb2u_k5_msl, entry = lab_kq_mvb2u_k5_msl_entry,
                       b_list <- [2], kq = 5, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2_k6", src = metal_kq_mvb2_k6_msl, entry = metal_kq_mvb2_k6_msl_entry,
                       b_list <- [2], kq = 6, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2u_k6", src = lab_kq_mvb2u_k6_msl, entry = lab_kq_mvb2u_k6_msl_entry,
                       b_list <- [2], kq = 6, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2h_k4", src = lab_kq_mvb2h_k4_msl, entry = lab_kq_mvb2h_k4_msl_entry,
                       b_list <- [2], kq = 4, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb4_k4", src = metal_kq_mvb4_k4_msl, entry = metal_kq_mvb4_k4_msl_entry,
                       b_list <- [4], kq = 4, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb4h_k4", src = lab_kq_mvb4h_k4_msl, entry = lab_kq_mvb4h_k4_msl_entry,
                       b_list <- [4], kq = 4, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2h_k6", src = lab_kq_mvb2h_k6_msl, entry = lab_kq_mvb2h_k6_msl_entry,
                       b_list <- [2], kq = 6, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb4_k6", src = metal_kq_mvb4_k6_msl, entry = metal_kq_mvb4_k6_msl_entry,
                       b_list <- [4], kq = 6, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb4h_k6", src = lab_kq_mvb4h_k6_msl, entry = lab_kq_mvb4h_k6_msl_entry,
                       b_list <- [4], kq = 6, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2s_k4", src = lab_kq_mvb2s_k4_msl, entry = lab_kq_mvb2s_k4_msl_entry,
                       b_list <- [2], kq = 4, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8),
            LabVariant(name = "kq_mvb2s_k6", src = lab_kq_mvb2s_k6_msl, entry = lab_kq_mvb2s_k6_msl_entry,
                       b_list <- [2], kq = 6, kq16 = true, kq_mvb = true, tgsz = 64, rtg = 8)
        ]
        run_lab(b, variants)
        delete variants
    }
}
