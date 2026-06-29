options gen2

module dasllama_math_aarch64_neon shared public

require dasllama/dasllama_math
require llvm/daslib/aarch64_neon  // sdot4: JIT-emitted ARM SDOT, scalar fallback elsewhere
require daslib/jobque_boost public  // kernels expand parallel_for; consumers need its symbols
require dasllama/dasllama_par  // maybe_parallel_for

// arm64 SDOT backend for the Q8·Q8 matmuls. dasllama_math owns the public matmul wrappers + the
// portable kernels (the default); this module supplies the NEON kernels and, on an arm64 host,
// registers them as the active backend at [init] (use_q8q8_kernels). On every other host
// get_architecture_name() != "arm64" so the portable kernels stay selected — and dot_q8q8_sdot4x4's
// sdot4 calls would hit their scalar fallback anyway. Under JIT each sdot4 lowers to one
// @llvm.aarch64.neon.sdot (see llvm/daslib/aarch64_neon + g_aarch64_intrin_lookup in llvm_jit_intrin).
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

def private q8q8_kernel_neon(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
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

def private q8q8_batch_kernel_neon(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
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

def private q8q8_group3_kernel_neon(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
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

[init]
def dasllama_math_aarch64_neon_register() {
    if (get_architecture_name() == "arm64") {
        use_q8q8_kernels(@@q8q8_kernel_neon, @@q8q8_batch_kernel_neon, @@q8q8_group3_kernel_neon)
    }
}
