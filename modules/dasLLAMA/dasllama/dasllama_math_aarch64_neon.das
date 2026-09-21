options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_math_aarch64_neon shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_math
require dasllama/dasllama_math_default  // the idot4x4 kernel stamps this backend registers + dot_mx4q8
require dasllama/dasllama_tune  // [tuned]: reconstitute the laneq4x4 tile from its template with per-box loop hints
require dasllama/dasllama_tune_scope  // nolint:STYLE030 — the dasllama [tune_scope] must be on the AST before this module's [tuned] kernels pick their perms
require daslib/aarch64_neon  // tbl16/sdot4_w/sdot4_laneq: the mx4 + laneq tiles' JIT intrinsics
require daslib/f16_cvt  // f16_to_f32 — the wscale_f16 laneq stamp widens the binary16 scale in-loop
require daslib/jobque_boost public  // kernels expand parallel_for; consumers need its symbols

// arm64 SDOT backend for the Q8·Q8 matmuls. dasllama_math owns the public matmul wrappers + the
// Registration stays gated on `jit_enabled()` + arm64: off-JIT the idot externs are correct but not
// fast, so the portable backend stays selected there, as it does on every non-arm64 host.

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
// reduce); group = 4*n int8, block bi at +bi*128. Tail rows (d % 4) fall back to row-major idot4x4.

//! The 4-row group's block scales as one float4: the f32 plane's four, or the binary16 plane's four
//! widened in-loop (the wscale_f16 rail; bit-identical to the f32 stamp over the widened plane).
def wscale4_f32(sg : float const?; bi : int64) : float4 {
    unsafe {
        return float4(sg[bi * 4l], sg[bi * 4l + 1l], sg[bi * 4l + 2l], sg[bi * 4l + 3l])
    }
}

def wscale4_f16(sg : uint16 const?; bi : int64) : float4 {
    unsafe {
        return float4(f16_to_f32(uint(sg[bi * 4l])), f16_to_f32(uint(sg[bi * 4l + 1l])),
            f16_to_f32(uint(sg[bi * 4l + 2l])), f16_to_f32(uint(sg[bi * 4l + 3l])))
    }
}

[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xq, noalias = xs)]
def template dot_q8q8_laneq4_template(wg : int8 const ?; sg : float const ?; xq : int8 const ?; xs : float const ?; n : int64) : float4 {
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
            let s = WSCALE4(sg, bi)
            let xsb = xs[bi]
            f += float4(acc) * s * float4(xsb, xsb, xsb, xsb)
        }
    }
    return f
}

//! Lane-indexed SDOT over one 4-row interleaved group vs one token: 8 sdot4_laneq per 32-block,
//! accumulator lane L is output row L, flushed to a float4 via block scales × token scale — no
//! horizontal reduce.
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xq, noalias = xs), from_template(src = "dot_q8q8_laneq4_template", WSCALE4 = "wscale4_f32")]
def dot_q8q8_laneq4(wg : int8 const?; sg : float const?; xq : int8 const?; xs : float const?; n : int64) : float4 {}

[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xq, noalias = xs), from_template(src = "dot_q8q8_laneq4_template", WSCALE4 = "wscale4_f16")]
def dot_q8q8_laneq4_f16s(wg : int8 const?; sg : uint16 const?; xq : int8 const?; xs : float const?; n : int64) : float4 {}

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

//! 4-row × 4-token tile: shared weight v16i8 per chunk-lane
//! feeds all 4 tokens (LLVM CSEs to one load, 4x fewer than 4× laneq4); 4 independent accumulators
//! give SDOT the ILP a single chain can't. Not `private`, t0 % 4 == 0 and ntok-t0 >= 4 assumed.
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp), tuned(fallback = "u2")]
def dot_q8q8_laneq4x4(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; n, d, g, t0 : int64) {}

[init, arch(at = "../ARCHITECTURE_CPU_KERNELS.md#cpu-tier-target-select")]
def dasllama_math_aarch64_neon_register() {
    if (get_target_architecture_name() == "arm64" && jit_enabled()) {
        // arm64-sdot: idot4x4 on row-major weights (idot4 → SDOT) — the auto-selected default for direct callers
        // (tests, benchmarks). priority 10 out-ranks portable (0). rb/re of the rows cores are multiples of 32 (slot contract).
        register_kernel_backend(KernelBackend(name = "arm64-sdot", mm = @@q8q8_kernel_idot4x4,
            batch = @@q8q8_batch_kernel_idot4x4, group3 = @@q8q8_group3_kernel_idot4x4,
            repack = @@repack_q8q8_identity, mm_mx4 = @@mx4q8_kernel_idot4x4,
            groupn = @@q8q8_groupn_kernel_idot4x4, groupn_mx4 = @@mx4q8_groupn_kernel_idot4x4,
            mm_rows = @@q8q8_rows_kernel_idot4x4, mx4_rows = @@mx4q8_rows_kernel_idot4x4,
            mm_s16 = @@q8q8_kernel_idot4x4_s16, batch_s16 = @@q8q8_batch_kernel_idot4x4_s16,
            group3_s16 = @@q8q8_group3_kernel_idot4x4_s16, groupn_s16 = @@q8q8_groupn_kernel_idot4x4_s16,
            mm_rows_s16 = @@q8q8_rows_kernel_idot4x4_s16,
            needs_repack = false, priority = 10))
        // The repack tier (interleaved groups, load-select only) is "arm64-gen" in
        // dasllama_math_gen — it superseded the hand arm64-laneq backend that lived here.
    }
}
