options gen2

module dasllama_math_aarch64_neon shared public

require dasllama/dasllama_math
require llvm/daslib/aarch64_neon  // sdot4: JIT-emitted ARM SDOT, scalar fallback elsewhere
require daslib/jobque_boost public  // kernels expand parallel_for; consumers need its symbols
require dasllama/dasllama_par  // maybe_parallel_for
require math  // min/max in the token-block bounds

// arm64 SDOT backend for the Q8·Q8 matmuls. dasllama_math owns the public matmul wrappers + the
// kernel-backend registry; dasllama_math_default supplies the portable fallback; this module supplies
// the NEON kernels and, on an arm64 JIT host, registers them at [init] (register_kernel_backend). The
// registration is gated on `jit_enabled()`: only under JIT does each sdot4 lower to one
// @llvm.aarch64.neon.sdot (see llvm/daslib/aarch64_neon + g_aarch64_intrin_lookup in llvm_jit_intrin).
// Off-JIT (interpreter or AOT) sdot4 hits its scalar fallback — slower than the portable dot_q8q8 — so
// the portable backend stays selected there, as it does on every non-arm64 host.
//
// The kernels mirror the portable ones in dasllama_math (same raw-pointer signatures = the
// MatmulQ8Q8*Fn typedefs, same 1N threading) and differ only in the inner dot. They are free to
// diverge in threading/tiling per-platform since dispatch is at the whole-matmul level.

// Batched-GEMM token-block width: the laneq batch kernel tiles tokens into TB-sized blocks (outer),
// looping weight groups inside, so a TB×n activation slice stays L2-resident and is reused across a
// thread's groups instead of re-streaming the whole activation per group. 128 is the tuned default
// (Llama-3.2-1B, M1 Max — pipeline best-of-5 @2048); exposed as a runtime knob for per-model tuning
// (larger n wants a smaller TB to stay in L2). Clamped to >= 1: the kernel advances `tb += TB`, so a
// non-positive block width would never make progress (infinite loop).
var g_q8_token_block = 128l
def public set_q8_token_block(tb : int64) { g_q8_token_block = max(tb, 1l) }
def public get_q8_token_block() : int64 { return g_q8_token_block }

//! ARM SDOT dot: 4 blocks/iter, 4 independent int4 accumulators (each block = 2 sdot4 of 16 int8),
//! hsum + dual-scale flushed per block into 4 separate float accumulators. Bit-for-bit close to
//! dot_q8q8 (~3.8e-6). Not `private`: it is called from inside the parallel_for worker lambdas, which
//! are lifted out of the module and can't see module-private functions.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_sdot4x4(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var f0 = 0.0
    var f1 = 0.0
    var f2 = 0.0
    var f3 = 0.0
    var bi = 0l
    unsafe {
        while (bi + 4l <= nb) {
            let bb = bi * 32l
            var a0 = int4(0, 0, 0, 0)
            var a1 = int4(0, 0, 0, 0)
            var a2 = int4(0, 0, 0, 0)
            var a3 = int4(0, 0, 0, 0)
            a0 = sdot4(a0, wq + bb, xq + bb)
            a0 = sdot4(a0, wq + bb + 16l, xq + bb + 16l)
            a1 = sdot4(a1, wq + bb + 32l, xq + bb + 32l)
            a1 = sdot4(a1, wq + bb + 48l, xq + bb + 48l)
            a2 = sdot4(a2, wq + bb + 64l, xq + bb + 64l)
            a2 = sdot4(a2, wq + bb + 80l, xq + bb + 80l)
            a3 = sdot4(a3, wq + bb + 96l, xq + bb + 96l)
            a3 = sdot4(a3, wq + bb + 112l, xq + bb + 112l)
            f0 += float(a0.x + a0.y + a0.z + a0.w) * ws[bi + 0l] * xs[bi + 0l]
            f1 += float(a1.x + a1.y + a1.z + a1.w) * ws[bi + 1l] * xs[bi + 1l]
            f2 += float(a2.x + a2.y + a2.z + a2.w) * ws[bi + 2l] * xs[bi + 2l]
            f3 += float(a3.x + a3.y + a3.z + a3.w) * ws[bi + 3l] * xs[bi + 3l]
            bi += 4l
        }
        while (bi < nb) {
            let base = bi * 32l
            var acc = int4(0, 0, 0, 0)
            acc = sdot4(acc, wq + base, xq + base)
            acc = sdot4(acc, wq + base + 16l, xq + base + 16l)
            f0 += float(acc.x + acc.y + acc.z + acc.w) * ws[bi] * xs[bi]
            bi++
        }
    }
    return f0 + f1 + f2 + f3
}

// ===== default backend (sdot4x4 on row-major weights) — direct callers (tests, benches) =====

def private q8q8_kernel_neon_sdot4x4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(n * d >= MATMUL_PAR_THRESHOLD, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                myp[i] = dot_q8q8_sdot4x4(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
            }
        }
    }
}

def private q8q8_batch_kernel_neon_sdot4x4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(n * d * ntok >= MATMUL_PAR_THRESHOLD, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let wrow = wp + int64(i) * n
                let srow = sp + int64(i) * nb
                for (tk in range64(ntok)) {
                    myp[tk * d + int64(i)] = dot_q8q8_sdot4x4(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                }
            }
        }
    }
}

def private q8q8_group3_kernel_neon_sdot4x4(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                                            woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let dtot = d0 + d1 + d2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    parallel_for(0, int(dtot), get_total_hw_jobs()) $(rb, re, wg) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                if (g < d0) {
                    w0[g] = dot_q8q8_sdot4x4(wp + woff0 + g * n, sp + woff0 / 32l + g * nb, xqp, xsp, n)
                } elif (g < d0 + d1) {
                    let r = g - d0
                    w1[r] = dot_q8q8_sdot4x4(wp + woff1 + r * n, sp + woff1 / 32l + r * nb, xqp, xsp, n)
                } else {
                    let r = g - d0 - d1
                    w2[r] = dot_q8q8_sdot4x4(wp + woff2 + r * n, sp + woff2 / 32l + r * nb, xqp, xsp, n)
                }
            }
        }
        wg |> notify_and_release
    }
}

// ===== laneq backend (lane-indexed SDOT over interleaved 4-row groups) — Q8 model load only =====
// Weights are repacked at load (repack_q8q8_neon) so each accumulator lane is a distinct output row:
// no per-block horizontal reduce. Activated by select_matmul_backend_for_load at Q8 model load, never
// by default (the row-major direct callers would read the interleaved layout as garbage). Group base + block stride match the
// repack: a 4-row group is 4*n int8, block bi at +bi*128 (32 k's × 4 rows). Tail rows (d % 4) are kept
// row-major after the groups and run the sdot4x4 fallback.

//! Lane-indexed SDOT over one 4-row interleaved group against one token. 8 sdot4_laneq per 32-block
//! (two 16-k chunks × lanes 0..3); the int4 accumulator's lane L is output row L, flushed to a float4
//! with the 4 rows' block scales × the token block scale — no horizontal reduce anywhere. Not `private`:
//! called from inside the lifted parallel_for worker lambdas.
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xq, noalias = xs)]
def dot_q8q8_laneq4(wg : int8 const?; sg : float const?; xq : int8 const?; xs : float const?; n : int64) : float4 {
    let nb = n / 32l
    var f = float4(0.0, 0.0, 0.0, 0.0)
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * 128l
            let xb = bi * 32l
            var acc = int4(0, 0, 0, 0)
            acc = sdot4_laneq(acc, wg + wb, xq + xb, 0)
            acc = sdot4_laneq(acc, wg + wb + 16l, xq + xb, 1)
            acc = sdot4_laneq(acc, wg + wb + 32l, xq + xb, 2)
            acc = sdot4_laneq(acc, wg + wb + 48l, xq + xb, 3)
            acc = sdot4_laneq(acc, wg + wb + 64l, xq + xb + 16l, 0)
            acc = sdot4_laneq(acc, wg + wb + 80l, xq + xb + 16l, 1)
            acc = sdot4_laneq(acc, wg + wb + 96l, xq + xb + 16l, 2)
            acc = sdot4_laneq(acc, wg + wb + 112l, xq + xb + 16l, 3)
            let s = float4(sg[bi * 4l], sg[bi * 4l + 1l], sg[bi * 4l + 2l], sg[bi * 4l + 3l])
            let xsb = xs[bi]
            f += float4(acc) * s * float4(xsb, xsb, xsb, xsb)
        }
    }
    return f
}

//! 4-row × 4-token tile (the llama.cpp ggml_gemm_q8_0_4x4 structure): one 4-row interleaved group
//! against 4 tokens at once, 4 independent int4 accumulators (one per token). Per chunk-lane the same
//! weight v16i8 feeds all 4 tokens — LLVM CSEs it to one load (4× fewer weight loads than 4× laneq4) —
//! and the 4 independent accumulator chains give the SDOT pipeline the ILP a single chain can't, so it
//! approaches throughput instead of being latency-bound. Writes 16 outputs token-major into yp. Not
//! `private`: called from inside the lifted parallel_for worker lambda. t0 % 1, ntok-t0 >= 4 assumed.
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp)]
def dot_q8q8_laneq4x4(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; n, d, g, t0 : int64) {
    let nb = n / 32l
    var f0 = float4(0.0, 0.0, 0.0, 0.0)
    var f1 = float4(0.0, 0.0, 0.0, 0.0)
    var f2 = float4(0.0, 0.0, 0.0, 0.0)
    var f3 = float4(0.0, 0.0, 0.0, 0.0)
    unsafe {
        let x0 = xqp + (t0 + 0l) * n
        let x1 = xqp + (t0 + 1l) * n
        let x2 = xqp + (t0 + 2l) * n
        let x3 = xqp + (t0 + 3l) * n
        let xs0 = xsp + (t0 + 0l) * nb
        let xs1 = xsp + (t0 + 1l) * nb
        let xs2 = xsp + (t0 + 2l) * nb
        let xs3 = xsp + (t0 + 3l) * nb
        for [unroll_count = 2] (bi in range64(nb)) {
            let wb = bi * 128l
            let xb = bi * 32l
            var a0 = int4(0, 0, 0, 0)
            var a1 = int4(0, 0, 0, 0)
            var a2 = int4(0, 0, 0, 0)
            var a3 = int4(0, 0, 0, 0)
            // chunk 0 (k 0..15): weight v16i8 per lane shared across the 4 tokens
            a0 = sdot4_laneq(a0, wg + wb, x0 + xb, 0); a1 = sdot4_laneq(a1, wg + wb, x1 + xb, 0); a2 = sdot4_laneq(a2, wg + wb, x2 + xb, 0); a3 = sdot4_laneq(a3, wg + wb, x3 + xb, 0)
            a0 = sdot4_laneq(a0, wg + wb + 16l, x0 + xb, 1); a1 = sdot4_laneq(a1, wg + wb + 16l, x1 + xb, 1); a2 = sdot4_laneq(a2, wg + wb + 16l, x2 + xb, 1); a3 = sdot4_laneq(a3, wg + wb + 16l, x3 + xb, 1)
            a0 = sdot4_laneq(a0, wg + wb + 32l, x0 + xb, 2); a1 = sdot4_laneq(a1, wg + wb + 32l, x1 + xb, 2); a2 = sdot4_laneq(a2, wg + wb + 32l, x2 + xb, 2); a3 = sdot4_laneq(a3, wg + wb + 32l, x3 + xb, 2)
            a0 = sdot4_laneq(a0, wg + wb + 48l, x0 + xb, 3); a1 = sdot4_laneq(a1, wg + wb + 48l, x1 + xb, 3); a2 = sdot4_laneq(a2, wg + wb + 48l, x2 + xb, 3); a3 = sdot4_laneq(a3, wg + wb + 48l, x3 + xb, 3)
            // chunk 1 (k 16..31)
            a0 = sdot4_laneq(a0, wg + wb + 64l, x0 + xb + 16l, 0); a1 = sdot4_laneq(a1, wg + wb + 64l, x1 + xb + 16l, 0); a2 = sdot4_laneq(a2, wg + wb + 64l, x2 + xb + 16l, 0); a3 = sdot4_laneq(a3, wg + wb + 64l, x3 + xb + 16l, 0)
            a0 = sdot4_laneq(a0, wg + wb + 80l, x0 + xb + 16l, 1); a1 = sdot4_laneq(a1, wg + wb + 80l, x1 + xb + 16l, 1); a2 = sdot4_laneq(a2, wg + wb + 80l, x2 + xb + 16l, 1); a3 = sdot4_laneq(a3, wg + wb + 80l, x3 + xb + 16l, 1)
            a0 = sdot4_laneq(a0, wg + wb + 96l, x0 + xb + 16l, 2); a1 = sdot4_laneq(a1, wg + wb + 96l, x1 + xb + 16l, 2); a2 = sdot4_laneq(a2, wg + wb + 96l, x2 + xb + 16l, 2); a3 = sdot4_laneq(a3, wg + wb + 96l, x3 + xb + 16l, 2)
            a0 = sdot4_laneq(a0, wg + wb + 112l, x0 + xb + 16l, 3); a1 = sdot4_laneq(a1, wg + wb + 112l, x1 + xb + 16l, 3); a2 = sdot4_laneq(a2, wg + wb + 112l, x2 + xb + 16l, 3); a3 = sdot4_laneq(a3, wg + wb + 112l, x3 + xb + 16l, 3)
            let s = float4(sg[bi * 4l], sg[bi * 4l + 1l], sg[bi * 4l + 2l], sg[bi * 4l + 3l])
            let q0 = xs0[bi]
            let q1 = xs1[bi]
            let q2 = xs2[bi]
            let q3 = xs3[bi]
            f0 += float4(a0) * s * float4(q0, q0, q0, q0)
            f1 += float4(a1) * s * float4(q1, q1, q1, q1)
            f2 += float4(a2) * s * float4(q2, q2, q2, q2)
            f3 += float4(a3) * s * float4(q3, q3, q3, q3)
        }
        let r = g * 4l
        yp[(t0 + 0l) * d + r + 0l] = f0.x
        yp[(t0 + 0l) * d + r + 1l] = f0.y
        yp[(t0 + 0l) * d + r + 2l] = f0.z
        yp[(t0 + 0l) * d + r + 3l] = f0.w
        yp[(t0 + 1l) * d + r + 0l] = f1.x
        yp[(t0 + 1l) * d + r + 1l] = f1.y
        yp[(t0 + 1l) * d + r + 2l] = f1.z
        yp[(t0 + 1l) * d + r + 3l] = f1.w
        yp[(t0 + 2l) * d + r + 0l] = f2.x
        yp[(t0 + 2l) * d + r + 1l] = f2.y
        yp[(t0 + 2l) * d + r + 2l] = f2.z
        yp[(t0 + 2l) * d + r + 3l] = f2.w
        yp[(t0 + 3l) * d + r + 0l] = f3.x
        yp[(t0 + 3l) * d + r + 1l] = f3.y
        yp[(t0 + 3l) * d + r + 2l] = f3.z
        yp[(t0 + 3l) * d + r + 3l] = f3.w
    }
}

// In-place repack of one Q8 weight region [d x n] into 4-row-interleaved groups (+ float4-per-block
// scales). Copies the region to a scratch, then writes the interleaved layout back; tail rows (d % 4)
// stay row-major. Runs once per weight tensor at load, on the main context.
def private repack_q8q8_neon(var wp : int8?; var sp : float?; n, d : int64) {
    let nb = n / 32l
    let ng = d / 4l
    let ncg = n / 4l
    var tw : array<int8>
    var ts : array<float>
    tw |> resize(d * n)
    ts |> resize(d * nb)
    unsafe {
        var twp = addr(tw[0])
        var tsp = addr(ts[0])
        for (i in range64(d * n)) {
            twp[i] = wp[i]
        }
        for (i in range64(d * nb)) {
            tsp[i] = sp[i]
        }
        for (g in range64(ng)) {
            let gbase = g * 4l * n
            for (kg in range64(ncg)) {
                let dst = gbase + kg * 16l
                for (r in range64(4l)) {
                    let src = (g * 4l + r) * n + kg * 4l
                    wp[dst + r * 4l + 0l] = twp[src + 0l]
                    wp[dst + r * 4l + 1l] = twp[src + 1l]
                    wp[dst + r * 4l + 2l] = twp[src + 2l]
                    wp[dst + r * 4l + 3l] = twp[src + 3l]
                }
            }
            let sgbase = g * 4l * nb
            for (bi in range64(nb)) {
                for (r in range64(4l)) {
                    sp[sgbase + bi * 4l + r] = tsp[(g * 4l + r) * nb + bi]
                }
            }
        }
    }
    delete tw
    delete ts
}

def private q8q8_kernel_neon_laneq(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    let ng = d / 4l
    var myp = yp
    maybe_parallel_for(n * d >= MATMUL_PAR_THRESHOLD, 0, int(ng), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                myp[g * 4l + 0l] = f.x
                myp[g * 4l + 1l] = f.y
                myp[g * 4l + 2l] = f.z
                myp[g * 4l + 3l] = f.w
            }
        }
    }
    unsafe {
        for (i in range64(ng * 4l, d)) {
            myp[i] = dot_q8q8_sdot4x4(wp + i * n, sp + i * nb, xqp, xsp, n)
        }
    }
}

def private q8q8_batch_kernel_neon_laneq(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    let ng = d / 4l
    var myp = yp
    // Token-blocked: TB tokens outer, groups inner — the TB×n activation slice stays L2-resident and is
    // reused across this thread's groups, instead of re-streaming the whole activation once per group
    // (which goes DRAM-bound at large ntok, e.g. the FFN-down n=8192, ntok=2048 = 16MB restreamed d/4×).
    // Clamp to keep TB×n within the per-box L2 budget (inert at the default TB=128): above ~L2/n the
    // slice spills and prefill cliffs (measured n=8192 ~TB 1024, n=14336 ~512 on M1 Max).
    let TB = effective_token_block(g_q8_token_block, n)
    maybe_parallel_for(n * d * ntok >= MATMUL_PAR_THRESHOLD, 0, int(ng), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            var tb = 0l
            while (tb < ntok) {
                let tbe = min(tb + TB, ntok)
                let tb4 = tb + ((tbe - tb) / 4l) * 4l
                for (gi in range(rb, re)) {
                    let g = int64(gi)
                    let wgp = wp + g * 4l * n
                    let sgp = sp + g * 4l * nb
                    var tk = tb
                    while (tk < tb4) {     // 4-row × 4-token tiles (the throughput path)
                        dot_q8q8_laneq4x4(myp, wgp, sgp, xqp, xsp, n, d, g, tk)
                        tk += 4l
                    }
                    while (tk < tbe) {    // token tail within this block: one token at a time
                        let f = dot_q8q8_laneq4(wgp, sgp, xqp + tk * n, xsp + tk * nb, n)
                        myp[tk * d + g * 4l + 0l] = f.x
                        myp[tk * d + g * 4l + 1l] = f.y
                        myp[tk * d + g * 4l + 2l] = f.z
                        myp[tk * d + g * 4l + 3l] = f.w
                        tk++
                    }
                }
                tb += TB
            }
        }
    }
    unsafe {
        for (i in range64(ng * 4l, d)) {   // row tail (d % 4): row-major sdot4x4
            let wrow = wp + i * n
            let srow = sp + i * nb
            for (tk in range64(ntok)) {
                myp[tk * d + i] = dot_q8q8_sdot4x4(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
            }
        }
    }
}

def private q8q8_group3_kernel_neon_laneq(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                                          woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let ng0 = d0 / 4l
    let ng1 = d1 / 4l
    let ng2 = d2 / 4l
    let ngtot = ng0 + ng1 + ng2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    parallel_for(0, int(ngtot), get_total_hw_jobs()) $(rb, re, wg) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                if (g < ng0) {
                    let f = dot_q8q8_laneq4(wp + woff0 + g * 4l * n, sp + woff0 / 32l + g * 4l * nb, xqp, xsp, n)
                    w0[g * 4l + 0l] = f.x
                    w0[g * 4l + 1l] = f.y
                    w0[g * 4l + 2l] = f.z
                    w0[g * 4l + 3l] = f.w
                } elif (g < ng0 + ng1) {
                    let r = g - ng0
                    let f = dot_q8q8_laneq4(wp + woff1 + r * 4l * n, sp + woff1 / 32l + r * 4l * nb, xqp, xsp, n)
                    w1[r * 4l + 0l] = f.x
                    w1[r * 4l + 1l] = f.y
                    w1[r * 4l + 2l] = f.z
                    w1[r * 4l + 3l] = f.w
                } else {
                    let r = g - ng0 - ng1
                    let f = dot_q8q8_laneq4(wp + woff2 + r * 4l * n, sp + woff2 / 32l + r * 4l * nb, xqp, xsp, n)
                    w2[r * 4l + 0l] = f.x
                    w2[r * 4l + 1l] = f.y
                    w2[r * 4l + 2l] = f.z
                    w2[r * 4l + 3l] = f.w
                }
            }
        }
        wg |> notify_and_release
    }
    unsafe {
        for (i in range64(ng0 * 4l, d0)) {
            w0[i] = dot_q8q8_sdot4x4(wp + woff0 + i * n, sp + woff0 / 32l + i * nb, xqp, xsp, n)
        }
        for (i in range64(ng1 * 4l, d1)) {
            w1[i] = dot_q8q8_sdot4x4(wp + woff1 + i * n, sp + woff1 / 32l + i * nb, xqp, xsp, n)
        }
        for (i in range64(ng2 * 4l, d2)) {
            w2[i] = dot_q8q8_sdot4x4(wp + woff2 + i * n, sp + woff2 / 32l + i * nb, xqp, xsp, n)
        }
    }
}

[init]
def dasllama_math_aarch64_neon_register() {
    // Only register the SDOT backends under JIT on arm64. Off-JIT (interpreter or AOT) sdot4/sdot4_laneq
    // run their SCALAR fallback body — slower than the portable dot_q8q8, which the vectorizer handles —
    // so the portable backend stays selected everywhere the hardware SDOT isn't emitted.
    if (get_architecture_name() == "arm64" && jit_enabled()) {
        // arm64-sdot: sdot4x4 on row-major weights — the auto-selected default for direct callers
        // (tests, benchmarks). priority 10 out-ranks portable (0).
        register_kernel_backend(KernelBackend(name = "arm64-sdot", mm = @@q8q8_kernel_neon_sdot4x4,
            batch = @@q8q8_batch_kernel_neon_sdot4x4, group3 = @@q8q8_group3_kernel_neon_sdot4x4,
            repack = @@repack_q8q8_identity, needs_repack = false, priority = 10))
        // arm64-laneq: laneq + interleaved repack — the highest priority (20), but needs_repack, so only
        // select_matmul_backend_for_load (at Q8 model load, which then repacks) activates it.
        register_kernel_backend(KernelBackend(name = "arm64-laneq", mm = @@q8q8_kernel_neon_laneq,
            batch = @@q8q8_batch_kernel_neon_laneq, group3 = @@q8q8_group3_kernel_neon_laneq,
            repack = @@repack_q8q8_neon, needs_repack = true, priority = 20))
    }
}
