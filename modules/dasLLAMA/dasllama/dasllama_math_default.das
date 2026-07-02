options gen2

module dasllama_math_default shared public

require dasllama/dasllama_math       // KernelBackend + register_kernel_backend + MATMUL_PAR_THRESHOLD + repack_q8q8_identity
require daslib/jobque_boost public   // parallel_for / notify_and_release (the kernels expand parallel_for)
require dasllama/dasllama_par        // maybe_parallel_for / get_total_hw_jobs
require math

// The PORTABLE Q8·Q8 kernel backend — auto-vectorized dot_q8q8, no repack (row-major weights). It is
// the default/fallback backend on every host, registered at [init] with the lowest priority; a platform
// module (dasllama_math_aarch64_neon, dasllama_math_x64_avx) registers a higher-priority backend that
// wins the auto-select where its ISA kernels are available. dasllama_math owns the registry + the public
// matmul wrappers; this module supplies the portable kernels and registers them.

//! Q8·Q8 dot: BOTH the weight (wq/ws) and the activation (xq/xs) are Q8 — int8 quants + one fp32
//! scale per 32-block. The inner block loop is a pure int8·int8 multiply-accumulate, which the LLVM
//! JIT lowers to ARM SDOT (vdotq_s32: 16 int8 MACs/instr, exact int32 accumulate). The two block
//! scales multiply in once per 32-wide block. ~3.8x the dequant-in-dot dot_q8 on the kernel bench.
//! n must be a multiple of 32.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
    }
    return acc
}

// ----- parallel_for dispatch profiler (off by default) -----
// When on, the batch kernel records each worker's start time (relative to the parallel_for start) into
// g_pf_ws[rb], then aggregates the first/last worker start and the wall time per call. A large gap
// between pf-start and last-worker-start = workers trickling in (the job pool waking from sleep), which
// is the suspected prefill stall. The recorder lives with the portable batch kernel so the A/B harness
// can pin the portable backend and measure the same parallel_for behavior in one module.
var g_pf_prof = false
var g_pf_ws : array<int64>   // per-call worker start (ns, relative to pf-start), indexed by chunk start row
var g_pf_first_sum = 0l      // Σ first-worker start (us) over calls
var g_pf_last_sum = 0l       // Σ last-worker start (us) over calls
var g_pf_wall_sum = 0l       // Σ parallel_for wall (us) over calls
var g_pf_n = 0l

//! Enable/disable the parallel_for dispatch profiler. max_rows must be >= the largest matmul output d.
def pf_prof_enable(on : bool; max_rows : int64) {
    g_pf_prof = on
    g_pf_ws |> resize(max_rows)
    g_pf_first_sum = 0l
    g_pf_last_sum = 0l
    g_pf_wall_sum = 0l
    g_pf_n = 0l
}

def pf_prof_report() {
    if (g_pf_n == 0l) {
        return
    }
    to_log(LOG_INFO, "=== parallel_for dispatch ({g_pf_n} batch-matmul calls) ===\n")
    to_log(LOG_INFO, "  avg first-worker start = {g_pf_first_sum / g_pf_n} us\n")
    to_log(LOG_INFO, "  avg last-worker start  = {g_pf_last_sum / g_pf_n} us  (time to full parallelism)\n")
    to_log(LOG_INFO, "  avg parallel_for wall  = {g_pf_wall_sum / g_pf_n} us\n")
    to_log(LOG_INFO, "  last-start as % of wall = {100l * g_pf_last_sum / max(g_pf_wall_sum, 1l)}% (high = workers trickle in / pool wake-up stall)\n")
}

// Portable kernels (auto-vectorized dot_q8q8). 1N threading (one chunk per hw job) measured fastest
// for this compute-light kernel (~52 vs ~51 tok/s at 2N end-to-end).
def private q8q8_kernel(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(n * d >= MATMUL_PAR_THRESHOLD, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                myp[i] = dot_q8q8(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
            }
        }
    }
}

def private q8q8_batch_kernel(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    var myp = yp
    if (g_pf_prof) {
        // instrumented path: raw parallel_for + per-worker start capture (see pf_prof_report)
        unsafe {
            var wsp0 = reinterpret<int64?>(addr(g_pf_ws[0]))
            for (k in range(int(d))) {
                wsp0[k] = 0l
            }
        }
        let pf0 = ref_time_ticks()
        unsafe {
            let wsbase = addr(g_pf_ws[0])
            parallel_for(0, int(d), get_total_hw_jobs()) $(rb, re, wg) {
                unsafe {
                    var wsw = reinterpret<int64?>(wsbase)
                    wsw[rb] = ref_time_ticks() - pf0
                    for (i in range(rb, re)) {
                        let wrow = wp + int64(i) * n
                        let srow = sp + int64(i) * nb
                        for (tk in range64(ntok)) {
                            myp[tk * d + int64(i)] = dot_q8q8(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                        }
                    }
                }
                wg |> notify_and_release
            }
        }
        let wall = (ref_time_ticks() - pf0) / 1000l
        var fs = 1l << 62l
        var ls = 0l
        for (k in range(int(d))) {
            let v = g_pf_ws[k]
            if (v != 0l) {
                fs = min(fs, v)
                ls = max(ls, v)
            }
        }
        g_pf_first_sum += fs / 1000l
        g_pf_last_sum += ls / 1000l
        g_pf_wall_sum += wall
        g_pf_n ++
        return
    }
    maybe_parallel_for(n * d * ntok >= MATMUL_PAR_THRESHOLD, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let wrow = wp + int64(i) * n
                let srow = sp + int64(i) * nb
                for (tk in range64(ntok)) {
                    myp[tk * d + int64(i)] = dot_q8q8(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                }
            }
        }
    }
}

def private q8q8_group3_kernel(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
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
                    w0[g] = dot_q8q8(wp + woff0 + g * n, sp + woff0 / 32l + g * nb, xqp, xsp, n)
                } elif (g < d0 + d1) {
                    let r = g - d0
                    w1[r] = dot_q8q8(wp + woff1 + r * n, sp + woff1 / 32l + r * nb, xqp, xsp, n)
                } else {
                    let r = g - d0 - d1
                    w2[r] = dot_q8q8(wp + woff2 + r * n, sp + woff2 / 32l + r * nb, xqp, xsp, n)
                }
            }
        }
        wg |> notify_and_release
    }
}

//! Register the portable Q8·Q8 backend (row-major, no repack). Lowest priority — a platform ISA backend
//! out-ranks it in the auto-select where available; it stays the fallback everywhere else.
[init]
def dasllama_math_default_register() {
    register_kernel_backend(KernelBackend(name = "portable", mm = @@q8q8_kernel, batch = @@q8q8_batch_kernel,
        group3 = @@q8q8_group3_kernel, repack = @@repack_q8q8_identity, needs_repack = false, priority = 0))
}
