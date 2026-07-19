options gen2
options _comment_hygiene = true

module dasllama_math_aarch64_neon shared public

require dasllama/dasllama_math
require dasllama/dasllama_tune  // [tuned]: reconstitute the laneq4x4 tile from its template with per-box loop hints
require llvm/daslib/aarch64_neon  // sdot4: JIT-emitted ARM SDOT, scalar fallback elsewhere
require llvm/daslib/f16_cvt  // f16_to_f32 — the wscale_f16 twins widen the binary16 scale in-loop
require daslib/jobque_boost public  // kernels expand parallel_for; consumers need its symbols
require dasllama/dasllama_par  // maybe_parallel_for

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
    maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), get_q8_batch_chunks_per_job(), n * d)) $(rb, re) {
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
    maybe_parallel_for(0, int(d), matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)) $(rb, re) {
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
    // oversplit ×4 costs the fifo path ~20% but wins ~19% under team dispatch (1B mm_qkv 86.6k->108k us)
    maybe_parallel_for(0, int(dtot), matmul_chunks_gemv(int(dtot), get_jobque_team_mode() ? get_q8_batch_chunks_per_job() : 1, n * dtot)) $(rb, re) {
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
    }
}

// ===== MXFP4·Q8 (native 4-bit expert weights) =====

[hint(unsafe_range_check, noalias = lut, noalias = wn, noalias = we, noalias = xq, noalias = xs)]
def template dot_mx4q8_template(lut : int8 const ?; wn : uint8 const ?; we : uint8 const ?; xq : int8 const ?; xs : float const ?; n : int64) : float {
    let nb = n / 32l
    var f = 0.0
    for [tune = 1] (bi in range64(nb)) {   // fallback u2 = ggml's ib += 2 shape
        let wp = unsafe(wn + bi * 16l)
        let xp = unsafe(xq + bi * 32l)
        var acc = int4(0, 0, 0, 0)
        acc = sdot4_w(acc, tbl16_lo(lut, wp), xp)
        acc = sdot4_w(acc, tbl16_hi(lut, wp), unsafe(xp + 16l))
        f += float(acc.x + acc.y + acc.z + acc.w) * e8m0_half(uint(unsafe(we[bi]))) * unsafe(xs[bi])
    }
    return f
}

//! ARM TBL+SDOT MXFP4·Q8 dot (NEON body of ggml_vec_dot_mxfp4_q8_0): one v16i8 nibble load feeds
//! two TBL lookups (low/high nibbles), each SDOT'd in-register against the Q8 activation, flushed
//! through e8m0_half(we) × xs. Not `private`: called from lifted parallel_for worker lambdas.
[hint(unsafe_range_check, noalias = lut, noalias = wn, noalias = we, noalias = xq, noalias = xs), tuned(fallback = "u2")]
def dot_mx4q8(lut : int8 const?; wn : uint8 const?; we : uint8 const?; xq : int8 const?; xs : float const?; n : int64) : float {}

def mx4q8_kernel_neon(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), get_q8_batch_chunks_per_job(), n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    myp[i] = dot_mx4q8(lutp, wn + int64(i) * (n / 2l), we + int64(i) * nb, xqp, xsp, n)
                }
            }
        }
    }
}

// ===== region-list (groupN) kernels — the MoE decode dispatch fuse =====
// One parallel_for over nregions*d rows fans out all routed experts in one fork/join; math is
// bit-exact vs the single-region kernels, only scheduling differs.

def private q8q8_groupn_kernel_neon_sdot4x4(var yp : float?; wp : int8 const?; sp : float const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(0, int(nregions * d), matmul_chunks_gemv(int(nregions * d), get_q8_batch_chunks_per_job(), n * d * nregions)) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let ii = int64(i)
                let r = ii / d
                let row = ii % d
                let woff = offs[r * 2l]
                let xoff = offs[r * 2l + 1l]
                var v = dot_q8q8_sdot4x4(wp + woff + row * n, sp + woff / 32l + row * nb, xqp + xoff, xsp + xoff / 32l, n)
                if (bp != null) {
                    v += bp[boffs[r] + row]
                }
                myp[ii] = v
            }
        }
    }
}

def mx4q8_groupn_kernel_neon(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(nregions * d), matmul_chunks_gemv(int(nregions * d), get_q8_batch_chunks_per_job(), n * d * nregions)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let ii = int64(i)
                    let r = ii / d
                    let row = ii % d
                    let woff = offs[r * 2l]
                    let xoff = offs[r * 2l + 1l]
                    var v = dot_mx4q8(lutp, wn + woff / 2l + row * (n / 2l), we + woff / 32l + row * nb,
                        xqp + xoff, xsp + xoff / 32l, n)
                    if (bp != null) {
                        v += bp[boffs[r] + row]
                    }
                    myp[ii] = v
                }
            }
        }
    }
}

// ===== MXFP4 laneq dot (interleaved 4-row groups) — gen composer =====
// Repack (repack_mx4_grp, mr=4) interleaves nibble bytes 4-rows-per-sub-chunk: one v16i8 load +
// tbl16_lo/hi yields both laneq vectors per sub-chunk. Tail rows (d % 4) fall back to row-major dot_mx4q8.

//! Interleaved MXFP4·Q8 dot over one 4-row group vs one token: 4 nibble loads feed 8 TBL + 8 indexed
//! SDOT (via sdot4_laneq_w); accumulator lane L is output row L, flushed via e8m0_half scales ×
//! token block scale, no horizontal reduce. Not `private`: called from lifted worker lambdas.
[hint(unsafe_range_check, noalias = lut, noalias = wg, noalias = eg, noalias = xq, noalias = xs)]
def dot_mx4q8_laneq4(lut : int8 const?; wg : uint8 const?; eg : uint8 const?; xq : int8 const?; xs : float const?; n : int64) : float4 {
    let nb = n / 32l
    var f = float4(0.0, 0.0, 0.0, 0.0)
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * 64l
            let xb = bi * 32l
            var acc = int4(0, 0, 0, 0)
            acc = sdot4_laneq_w(acc, tbl16_lo(lut, wg + wb), xq + xb, 0)
            acc = sdot4_laneq_w(acc, tbl16_hi(lut, wg + wb), xq + xb + 16l, 0)
            acc = sdot4_laneq_w(acc, tbl16_lo(lut, wg + wb + 16l), xq + xb, 1)
            acc = sdot4_laneq_w(acc, tbl16_hi(lut, wg + wb + 16l), xq + xb + 16l, 1)
            acc = sdot4_laneq_w(acc, tbl16_lo(lut, wg + wb + 32l), xq + xb, 2)
            acc = sdot4_laneq_w(acc, tbl16_hi(lut, wg + wb + 32l), xq + xb + 16l, 2)
            acc = sdot4_laneq_w(acc, tbl16_lo(lut, wg + wb + 48l), xq + xb, 3)
            acc = sdot4_laneq_w(acc, tbl16_hi(lut, wg + wb + 48l), xq + xb + 16l, 3)
            let s = float4(e8m0_half(uint(eg[bi * 4l])), e8m0_half(uint(eg[bi * 4l + 1l])),
                e8m0_half(uint(eg[bi * 4l + 2l])), e8m0_half(uint(eg[bi * 4l + 3l])))
            let xsb = xs[bi]
            f += float4(acc) * s * float4(xsb, xsb, xsb, xsb)
        }
    }
    return f
}

// ===== laneq dots (lane-indexed SDOT over interleaved 4-row groups) — gen composers =====
// Repack (repack_q8q8_grp, mr=4) makes each accumulator lane a distinct output row (no horizontal
// reduce); group = 4*n int8, block bi at +bi*128. Tail rows (d % 4) fall back to row-major sdot4x4.

//! Lane-indexed SDOT over one 4-row interleaved group vs one token: 8 sdot4_laneq per 32-block,
//! accumulator lane L is output row L, flushed to a float4 via block scales × token scale — no
//! horizontal reduce. Not `private`: called from lifted parallel_for worker lambdas.
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

//! wscale_f16 twin of dot_q8q8_laneq4: block scales are raw binary16 halfwords widened in-loop —
//! same fold order, bit-identical over the widened plane. Not `private`: called from lifted
//! parallel_for worker lambdas.
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xq, noalias = xs)]
def dot_q8q8_laneq4_f16s(wg : int8 const?; sg : uint16 const?; xq : int8 const?; xs : float const?; n : int64) : float4 {
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
            let s = float4(f16_to_f32(uint(sg[bi * 4l])), f16_to_f32(uint(sg[bi * 4l + 1l])),
                f16_to_f32(uint(sg[bi * 4l + 2l])), f16_to_f32(uint(sg[bi * 4l + 3l])))
            let xsb = xs[bi]
            f += float4(acc) * s * float4(xsb, xsb, xsb, xsb)
        }
    }
    return f
}

[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp)]
def template dot_q8q8_laneq4x4_template(var yp : float ?; wg : int8 const ?; sg : float const ?; xqp : int8 const ?; xsp : float const ?; n, d, g, t0 : int64) : void {
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
        for [tune = 1] (bi in range64(nb)) {   // fallback u2
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

//! 4-row × 4-token tile (llama.cpp ggml_gemm_q8_0_4x4 structure): shared weight v16i8 per chunk-lane
//! feeds all 4 tokens (LLVM CSEs to one load, 4x fewer than 4× laneq4); 4 independent accumulators
//! give SDOT the ILP a single chain can't. Not `private`, t0 % 4 == 0 and ntok-t0 >= 4 assumed.
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp), tuned(fallback = "u2")]
def dot_q8q8_laneq4x4(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; n, d, g, t0 : int64) {}

// Row-range core (mm_rows slot): rows [rb,re) of one region, no dispatch — fused decode chains
// thread these from their own stage lambdas. Not `private`: invoked through the kernel pointer
// from lifted worker lambdas. rb/re are multiples of 32 (slot contract).
def q8q8_rows_kernel_neon_sdot4x4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {
    let nb = n / 32l
    unsafe {
        for (i in range64(rb, re)) {
            yp[i] = dot_q8q8_sdot4x4(wp + i * n, sp + i * nb, xqp, xsp, n)
        }
    }
}

// ===== f16-scale twins (the wscale_f16 rail): the same SDOT loops over the binary16
// weight-scale plane, widened in-loop (the f16-KV codec pattern) — bit-identical to the f32
// forms over the plane the halfwords widen to =====

//! The wscale_f16 twin of dot_q8q8_sdot4x4: the per-block WEIGHT scale is a raw binary16
//! halfword. Not `private`: called from inside the lifted parallel_for worker lambdas.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_sdot4x4_f16s(wq : int8 const?; ws : uint16 const?; xq : int8 const?; xs : float const?; n : int64) : float {
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
            f0 += float(a0.x + a0.y + a0.z + a0.w) * f16_to_f32(uint(ws[bi + 0l])) * xs[bi + 0l]
            f1 += float(a1.x + a1.y + a1.z + a1.w) * f16_to_f32(uint(ws[bi + 1l])) * xs[bi + 1l]
            f2 += float(a2.x + a2.y + a2.z + a2.w) * f16_to_f32(uint(ws[bi + 2l])) * xs[bi + 2l]
            f3 += float(a3.x + a3.y + a3.z + a3.w) * f16_to_f32(uint(ws[bi + 3l])) * xs[bi + 3l]
            bi += 4l
        }
        while (bi < nb) {
            let base = bi * 32l
            var acc = int4(0, 0, 0, 0)
            acc = sdot4(acc, wq + base, xq + base)
            acc = sdot4(acc, wq + base + 16l, xq + base + 16l)
            f0 += float(acc.x + acc.y + acc.z + acc.w) * f16_to_f32(uint(ws[bi])) * xs[bi]
            bi++
        }
    }
    return f0 + f1 + f2 + f3
}

def private q8q8_kernel_neon_sdot4x4_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), get_q8_batch_chunks_per_job(), n * d)) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                myp[i] = dot_q8q8_sdot4x4_f16s(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
            }
        }
    }
}

def private q8q8_batch_kernel_neon_sdot4x4_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(0, int(d), matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let wrow = wp + int64(i) * n
                let srow = sp + int64(i) * nb
                for (tk in range64(ntok)) {
                    myp[tk * d + int64(i)] = dot_q8q8_sdot4x4_f16s(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                }
            }
        }
    }
}

def private q8q8_group3_kernel_neon_sdot4x4_s16(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : uint16 const?;
                                                woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let dtot = d0 + d1 + d2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    maybe_parallel_for(0, int(dtot), matmul_chunks_gemv(int(dtot), get_jobque_team_mode() ? get_q8_batch_chunks_per_job() : 1, n * dtot)) $(rb, re) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                if (g < d0) {
                    w0[g] = dot_q8q8_sdot4x4_f16s(wp + woff0 + g * n, sp + woff0 / 32l + g * nb, xqp, xsp, n)
                } elif (g < d0 + d1) {
                    let r = g - d0
                    w1[r] = dot_q8q8_sdot4x4_f16s(wp + woff1 + r * n, sp + woff1 / 32l + r * nb, xqp, xsp, n)
                } else {
                    let r = g - d0 - d1
                    w2[r] = dot_q8q8_sdot4x4_f16s(wp + woff2 + r * n, sp + woff2 / 32l + r * nb, xqp, xsp, n)
                }
            }
        }
    }
}

def private q8q8_groupn_kernel_neon_sdot4x4_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(0, int(nregions * d), matmul_chunks_gemv(int(nregions * d), get_q8_batch_chunks_per_job(), n * d * nregions)) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let ii = int64(i)
                let r = ii / d
                let row = ii % d
                let woff = offs[r * 2l]
                let xoff = offs[r * 2l + 1l]
                var v = dot_q8q8_sdot4x4_f16s(wp + woff + row * n, sp + woff / 32l + row * nb, xqp + xoff, xsp + xoff / 32l, n)
                if (bp != null) {
                    v += bp[boffs[r] + row]
                }
                myp[ii] = v
            }
        }
    }
}

// Not `private`: invoked through the kernel pointer from lifted worker lambdas.
def q8q8_rows_kernel_neon_sdot4x4_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {
    let nb = n / 32l
    unsafe {
        for (i in range64(rb, re)) {
            yp[i] = dot_q8q8_sdot4x4_f16s(wp + i * n, sp + i * nb, xqp, xsp, n)
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
            repack = @@repack_q8q8_identity, mm_mx4 = @@mx4q8_kernel_neon,
            groupn = @@q8q8_groupn_kernel_neon_sdot4x4, groupn_mx4 = @@mx4q8_groupn_kernel_neon,
            mm_rows = @@q8q8_rows_kernel_neon_sdot4x4,
            mm_s16 = @@q8q8_kernel_neon_sdot4x4_s16, batch_s16 = @@q8q8_batch_kernel_neon_sdot4x4_s16,
            group3_s16 = @@q8q8_group3_kernel_neon_sdot4x4_s16, groupn_s16 = @@q8q8_groupn_kernel_neon_sdot4x4_s16,
            mm_rows_s16 = @@q8q8_rows_kernel_neon_sdot4x4_s16,
            needs_repack = false, priority = 10))
        // The repack tier (interleaved groups, load-select only) is "arm64-gen" in
        // dasllama_math_gen — it superseded the hand arm64-laneq backend that lived here.
    }
}
