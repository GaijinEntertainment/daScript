options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_math_default shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_math       // KernelBackend + register_kernel_backend + the matmul_chunks shapers + repack_q8q8_identity
require dasllama/dasllama_tune       // [tuned]: reconstitute dot_q8q8 from its template with per-box loop hints
require dasllama/dasllama_tune_scope  // nolint:STYLE030 — the dasllama [tune_scope] must be on the AST before this module's [tuned] kernels pick their perms
require dasllama/dasllama_gemm_schema  // kq_qsb/kq_ssb — the fmt-id kq plane strides
require dasllama/dasllama_gpu_math    // ue4m3_half_f32 - the nv4 sub-block scale's one decode
require daslib/f16_cvt               // wscale_f16 - the binary16 weight-scale plane widens in-loop
require daslib/jobque_boost public   // parallel_for / notify_and_release (the kernels expand parallel_for)
require dasllama/dasllama_par        // maybe_parallel_for
require math

// The PORTABLE Q8·Q8 kernel backend — auto-vectorized dot_q8q8, no repack (row-major weights). It is
// the default/fallback backend on every host, registered at [init] with the lowest priority; a platform
// module (dasllama_math_aarch64_neon, dasllama_math_gen) registers a higher-priority backend that
// wins the auto-select where its ISA kernels are available. dasllama_math owns the registry + the public
// matmul wrappers; this module supplies the portable kernels and registers them.

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def template dot_q8q8_template(wq : int8 const ?; ws : float const ?; xq : int8 const ?; xs : float const ?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var iacc = 0
        for [tune = 1] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
    }
    return acc
}

//! Q8·Q8 dot: BOTH the weight (wq/ws) and the activation (xq/xs) are Q8 — int8 quants + one fp32
//! scale per 32-block. The inner block loop is a pure int8·int8 multiply-accumulate, which the LLVM
//! JIT lowers to ARM SDOT (vdotq_s32: 16 int8 MACs/instr, exact int32 accumulate) under the
//! fallback vec16. The two block scales multiply in once per 32-wide block. ~3.8x the
//! dequant-in-dot dot_q8 on the kernel bench. n must be a multiple of 32.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs), tuned(fallback = "vec16")]
def dot_q8q8(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def template dot_q8q8_f16s_template(wq : int8 const ?; ws : uint16 const ?; xq : int8 const ?; xs : float const ?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var iacc = 0
        for [tune = 1] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * f16_to_f32(uint(unsafe(ws[bi]))) * unsafe(xs[bi])
    }
    return acc
}

//! The wscale_f16 twin of dot_q8q8: the per-block WEIGHT scale is a raw binary16 halfword,
//! widened in-loop (vcvtph2ps/fcvtl under the JIT — the f16-KV codec lesson). Bit-identical to
//! dot_q8q8 over the f32 plane those halfwords widen to. n must be a multiple of 32.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs), tuned(fallback = "vec16")]
def dot_q8q8_f16s(wq : int8 const?; ws : uint16 const?; xq : int8 const?; xs : float const?; n : int64) : float {}

//! a Q8 block's weight scale off either scale plane: the f32 plane as is, the wscale_f16 halfword plane widened
def wscale(ws : float const?; bi : int64) : float {
    unsafe {
        return ws[bi]
    }
}

def wscale(ws : uint16 const?; bi : int64) : float {
    unsafe {
        return f16_to_f32(uint(ws[bi]))
    }
}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def template dot_q8q8_idot4x4_template(wq : int8 const ?; ws : float const ?; xq : int8 const ?; xs : float const ?; n : int64) : float {
    let nb = n / 32l
    var f0 = 0.0
    var f1 = 0.0
    var f2 = 0.0
    var f3 = 0.0
    var bi = 0l
    unsafe {
        let wv = reinterpret<byte16 const?>(wq)
        let xv = reinterpret<byte16 const?>(xq)
        while (bi + 4l <= nb) {
            let v = bi * 2l
            var a0 = idot4(wv[v], xv[v])
            a0 = idot4(a0, wv[v + 1l], xv[v + 1l])
            var a1 = idot4(wv[v + 2l], xv[v + 2l])
            a1 = idot4(a1, wv[v + 3l], xv[v + 3l])
            var a2 = idot4(wv[v + 4l], xv[v + 4l])
            a2 = idot4(a2, wv[v + 5l], xv[v + 5l])
            var a3 = idot4(wv[v + 6l], xv[v + 6l])
            a3 = idot4(a3, wv[v + 7l], xv[v + 7l])
            f0 += float(a0.x + a0.y + a0.z + a0.w) * wscale(ws, bi + 0l) * xs[bi + 0l]
            f1 += float(a1.x + a1.y + a1.z + a1.w) * wscale(ws, bi + 1l) * xs[bi + 1l]
            f2 += float(a2.x + a2.y + a2.z + a2.w) * wscale(ws, bi + 2l) * xs[bi + 2l]
            f3 += float(a3.x + a3.y + a3.z + a3.w) * wscale(ws, bi + 3l) * xs[bi + 3l]
            bi += 4l
        }
        while (bi < nb) {
            let v = bi * 2l
            var a0 = idot4(wv[v], xv[v])
            a0 = idot4(a0, wv[v + 1l], xv[v + 1l])
            f0 += float(a0.x + a0.y + a0.z + a0.w) * wscale(ws, bi) * xs[bi]
            bi++
        }
    }
    return f0 + f1 + f2 + f3
}

//! The 4-accumulator idot4 GEMV dot (4 blocks/iter, 2 idot4/block, per-block hsum into 4 rotating
//! float accs): plain-das byte16 loads + the idot4 builtin — one SDOT per call on arm64 JIT, ~1.35x the
//! autovectorized dot_q8q8. noinline: inlined into its kernel stamps' batch row loop it runs 9% slower on the M5.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs, noinline), from_template(src = "dot_q8q8_idot4x4_template")]
def dot_q8q8_idot4x4(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs, noinline), from_template(src = "dot_q8q8_idot4x4_template")]
def dot_q8q8_idot4x4_f16s(wq : int8 const?; ws : uint16 const?; xq : int8 const?; xs : float const?; n : int64) : float {}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def template dot_q8q8_idot4_ps_template(wq : int8 const ?; ws : float const ?; xq : int8 const ?; xs : float const ?; n : int64) : float {
    let nb = n / 32l
    var f0 = float4(0.0, 0.0, 0.0, 0.0)
    var f1 = float4(0.0, 0.0, 0.0, 0.0)
    var bi = 0l
    unsafe {
        let wv = reinterpret<byte16 const?>(wq)
        let xv = reinterpret<byte16 const?>(xq)
        while (bi + 2l <= nb) {
            let v = bi * 2l
            var a0 = idot4(wv[v], xv[v])
            a0 = idot4(a0, wv[v + 1l], xv[v + 1l])
            var a1 = idot4(wv[v + 2l], xv[v + 2l])
            a1 = idot4(a1, wv[v + 3l], xv[v + 3l])
            let s0 = wscale(ws, bi) * xs[bi]
            let s1 = wscale(ws, bi + 1l) * xs[bi + 1l]
            f0 += float4(a0) * float4(s0, s0, s0, s0)
            f1 += float4(a1) * float4(s1, s1, s1, s1)
            bi += 2l
        }
        while (bi < nb) {
            let v = bi * 2l
            var a0 = idot4(wv[v], xv[v])
            a0 = idot4(a0, wv[v + 1l], xv[v + 1l])
            let s0 = wscale(ws, bi) * xs[bi]
            f0 += float4(a0) * float4(s0, s0, s0, s0)
            bi++
        }
    }
    let f = f0 + f1
    return f.x + f.y + f.z + f.w
}

//! The vector-epilogue idot4 dot: per 32-block one idot4 pair, then float4 accumulate under the block's
//! scale - no horizontal sum until the row ends, two rotating float4 accumulators. The wasm SIMD128 form
//! (dot_q8q8_idot4x4's per-block hsum is four lane extracts there).
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs), from_template(src = "dot_q8q8_idot4_ps_template")]
def dot_q8q8_idot4_ps(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs), from_template(src = "dot_q8q8_idot4_ps_template")]
def dot_q8q8_idot4_ps_f16s(wq : int8 const?; ws : uint16 const?; xq : int8 const?; xs : float const?; n : int64) : float {}

//! a def, not a module global: team lanes run no global initializers, and the fold below is a
//! compile-time constant either way
def private wasm_target() : bool => get_target_architecture_name() == "wasm64" || get_target_architecture_name() == "wasm32"

[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#cpu-tier-target-select")]
def private dot_q8q8_p(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float =>
    wasm_target() ? dot_q8q8_idot4_ps(wq, ws, xq, xs, n) : dot_q8q8(wq, ws, xq, xs, n)

def private dot_q8q8_f16s_p(wq : int8 const?; ws : uint16 const?; xq : int8 const?; xs : float const?; n : int64) : float =>
    wasm_target() ? dot_q8q8_idot4_ps_f16s(wq, ws, xq, xs, n) : dot_q8q8_f16s(wq, ws, xq, xs, n)

// ----- parallel_for dispatch profiler (off by default) -----
// Records each worker's start time (rel. to pf-start) into g_pf_ws[rb]; a large first/last-worker gap
// = workers trickling in (job pool waking from sleep) — the suspected prefill stall.
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

//! The chunk oversplit the idot4 stamps bind: the batch-chunks knob, group3 only under team
//! dispatch (x4 costs the fifo path ~20% but wins ~19% under team: 1B mm_qkv 86.6k -> 108k us).
def q8_split_idot4x4(team_only : bool) : int => (!team_only || get_jobque_team_mode()) ? get_q8_batch_chunks_per_job() : 1

def q8_split_portable(_team_only : bool) : int => 1

def template q8q8_kernel_template(var yp : float ?; wp : int8 const ?; sp : float const ?; xqp : int8 const ?; xsp : float const ?; n, d : int64) : void {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), Q8SPLIT(false), n * d)) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                myp[i] = Q8DOT(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
            }
        }
    }
}

def template q8q8_batch_kernel_template(var yp : float ?; wp : int8 const ?; sp : float const ?; xqp : int8 const ?; xsp : float const ?; n, d, ntok : int64) : void {
    let nb = n / 32l
    var myp = yp
    // the raw parallel_for below is a dispatch single-thread mode admits no exception for
    if (g_pf_prof && !get_single_thread_()) {
        // instrumented path: raw parallel_for + per-worker start capture (see pf_prof_report)
        var wsp0 = unsafe(addr<int64?>(g_pf_ws[0]))
        for (k in range64(d)) {
            unsafe(wsp0[k]) = 0l
        }
        let pf0 = ref_time_ticks()
        unsafe {
            let wsbase = addr(g_pf_ws[0])
            parallel_for(0, int(d), matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)) $(rb, re, wg) {
                unsafe {
                    var wsw = reinterpret<int64?>(wsbase)
                    wsw[rb] = ref_time_ticks() - pf0
                    for (i in range(rb, re)) {
                        let wrow = wp + int64(i) * n
                        let srow = sp + int64(i) * nb
                        for (tk in range64(ntok)) {
                            myp[tk * d + int64(i)] = Q8DOT(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                        }
                    }
                }
                wg |> notify_and_release
            }
        }
        let wall = (ref_time_ticks() - pf0) / 1000l
        var fs = 1l << 62l
        var ls = 0l
        for (k in range64(d)) {
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
    maybe_parallel_for(0, int(d), matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let wrow = wp + int64(i) * n
                let srow = sp + int64(i) * nb
                for (tk in range64(ntok)) {
                    myp[tk * d + int64(i)] = Q8DOT(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                }
            }
        }
    }
}

// Row-range core (mm_rows slot): rows [rb,re) of one region, no dispatch — the fused decode
// chains thread it from their own stage lambdas.
def template q8q8_rows_kernel_template(var yp : float ?; wp : int8 const ?; sp : float const ?; xqp : int8 const ?; xsp : float const ?; n, rb, re : int64) : void {
    let nb = n / 32l
    unsafe {
        for (i in range64(rb, re)) {
            yp[i] = Q8DOT(wp + i * n, sp + i * nb, xqp, xsp, n)
        }
    }
}

def template q8q8_group3_kernel_template(var y0p : float ?; var y1p : float ?; var y2p : float ?; wp : int8 const ?; sp : float const ?;
                                         woff0, woff1, woff2 : int64; xqp : int8 const ?; xsp : float const ?; n, d0, d1, d2 : int64) : void {
    let nb = n / 32l
    let dtot = d0 + d1 + d2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    maybe_parallel_for(0, int(dtot), matmul_chunks_gemv(int(dtot), Q8SPLIT(true), n * dtot)) $(rb, re) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                if (g < d0) {
                    w0[g] = Q8DOT(wp + woff0 + g * n, sp + woff0 / 32l + g * nb, xqp, xsp, n)
                } elif (g < d0 + d1) {
                    let r = g - d0
                    w1[r] = Q8DOT(wp + woff1 + r * n, sp + woff1 / 32l + r * nb, xqp, xsp, n)
                } else {
                    let r = g - d0 - d1
                    w2[r] = Q8DOT(wp + woff2 + r * n, sp + woff2 / 32l + r * nb, xqp, xsp, n)
                }
            }
        }
    }
}

// region-list (groupN) kernel - the MoE decode dispatch fuse: one parallel_for over nregions*d
// rows fans out all routed experts in one fork/join, bit-exact vs the single-region kernel.
def template q8q8_groupn_kernel_template(var yp : float ?; wp : int8 const ?; sp : float const ?; offs : int64 const ?; nregions : int64; xqp : int8 const ?; xsp : float const ?; bp : float const ?; boffs : int64 const ?; n, d : int64) : void {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(0, int(nregions * d), matmul_chunks_gemv(int(nregions * d), Q8SPLIT(false), n * d * nregions)) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let ii = int64(i)
                let r = ii / d
                let row = ii % d
                let woff = offs[r * 2l]
                let xoff = offs[r * 2l + 1l]
                var v = Q8DOT(wp + woff + row * n, sp + woff / 32l + row * nb, xqp + xoff, xsp + xoff / 32l, n)
                if (bp != null) {
                    v += bp[boffs[r] + row]
                }
                myp[ii] = v
            }
        }
    }
}

[from_template(src = "q8q8_kernel_template", Q8DOT = "dot_q8q8_p", Q8SPLIT = "q8_split_portable")]
def private q8q8_kernel(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {}

[from_template(src = "q8q8_batch_kernel_template", Q8DOT = "dot_q8q8_p")]
def private q8q8_batch_kernel(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {}

[from_template(src = "q8q8_rows_kernel_template", Q8DOT = "dot_q8q8_p")]
def q8q8_rows_kernel(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {}

[from_template(src = "q8q8_group3_kernel_template", Q8DOT = "dot_q8q8_p", Q8SPLIT = "q8_split_portable")]
def private q8q8_group3_kernel(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                               woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {}

[from_template(src = "q8q8_groupn_kernel_template", Q8DOT = "dot_q8q8_p", Q8SPLIT = "q8_split_portable"), arch(at = "../ARCHITECTURE_CPU_KERNELS.md#kernel-visibility-lifted-workers")]
def q8q8_groupn_kernel(var yp : float?; wp : int8 const?; sp : float const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {}

[from_template(src = "q8q8_kernel_template", Q8DOT = "dot_q8q8_f16s_p", Q8SPLIT = "q8_split_portable")]
def private q8q8_kernel_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {}

[from_template(src = "q8q8_batch_kernel_template", Q8DOT = "dot_q8q8_f16s_p")]
def private q8q8_batch_kernel_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {}

[from_template(src = "q8q8_rows_kernel_template", Q8DOT = "dot_q8q8_f16s_p")]
def q8q8_rows_kernel_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {}

[from_template(src = "q8q8_group3_kernel_template", Q8DOT = "dot_q8q8_f16s_p", Q8SPLIT = "q8_split_portable")]
def private q8q8_group3_kernel_s16(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : uint16 const?;
                                   woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {}

[from_template(src = "q8q8_groupn_kernel_template", Q8DOT = "dot_q8q8_f16s_p", Q8SPLIT = "q8_split_portable")]
def q8q8_groupn_kernel_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {}

[from_template(src = "q8q8_kernel_template", Q8DOT = "dot_q8q8_idot4x4", Q8SPLIT = "q8_split_idot4x4")]
def q8q8_kernel_idot4x4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {}

[from_template(src = "q8q8_batch_kernel_template", Q8DOT = "dot_q8q8_idot4x4")]
def q8q8_batch_kernel_idot4x4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {}

[from_template(src = "q8q8_rows_kernel_template", Q8DOT = "dot_q8q8_idot4x4")]
def q8q8_rows_kernel_idot4x4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {}

[from_template(src = "q8q8_group3_kernel_template", Q8DOT = "dot_q8q8_idot4x4", Q8SPLIT = "q8_split_idot4x4")]
def q8q8_group3_kernel_idot4x4(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                               woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {}

[from_template(src = "q8q8_groupn_kernel_template", Q8DOT = "dot_q8q8_idot4x4", Q8SPLIT = "q8_split_idot4x4")]
def q8q8_groupn_kernel_idot4x4(var yp : float?; wp : int8 const?; sp : float const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {}

[from_template(src = "q8q8_kernel_template", Q8DOT = "dot_q8q8_idot4x4_f16s", Q8SPLIT = "q8_split_idot4x4")]
def q8q8_kernel_idot4x4_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {}

[from_template(src = "q8q8_batch_kernel_template", Q8DOT = "dot_q8q8_idot4x4_f16s")]
def q8q8_batch_kernel_idot4x4_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {}

[from_template(src = "q8q8_rows_kernel_template", Q8DOT = "dot_q8q8_idot4x4_f16s")]
def q8q8_rows_kernel_idot4x4_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {}

[from_template(src = "q8q8_group3_kernel_template", Q8DOT = "dot_q8q8_idot4x4_f16s", Q8SPLIT = "q8_split_idot4x4")]
def q8q8_group3_kernel_idot4x4_s16(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : uint16 const?;
                                   woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {}

[from_template(src = "q8q8_groupn_kernel_template", Q8DOT = "dot_q8q8_idot4x4_f16s", Q8SPLIT = "q8_split_idot4x4")]
def q8q8_groupn_kernel_idot4x4_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {}

// ===== Native K-quant (Q4_K/Q5_K/Q6_K) dots + rows kernels =====
// Plane layouts per 256-weight superblock: k4 128B nibbles+32B f16(s,o); k5 adds 32B qh; k6
// [128B ql][64B qh]+[16 sub-scales][f16 d]. Dot: s*(Σq·a) − o*(Σa); disk-order — backend-independent.

// kq v2 (all three formats): Q8_K-form activations — xsp is the PER-SUPERBLOCK scale plane
// (n/256 per token); integer sub-scale / bsums-x-mins sums fold to ONE float per superblock.
// Int sums are exact, so this is bit-exact vs the grp walk regardless of sub-block order.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#cpu-tier-target-select"),
 hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_k4) : float {
    var acc = 0.0
    let nibble_mask = byte16(15)
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 128l
            let kso = sb * 20l
            let dd = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            let dm = f16_to_f32(uint(ksrow[kso + 2l]) | (uint(ksrow[kso + 3l]) << 8u))
            var isum = 0
            var bsum = 0
            for (js in range64(4l)) {   // 32 quant bytes: low nibbles = sub-block 2js, high = 2js+1
                let b0 = sb * 8l + 2l * js   // activation 32-block of the low-nibble sub-block
                let qv = reinterpret<byte16 const?>(kqrow + kqo + 32l * js)
                let av = reinterpret<byte16 const?>(xqp + b0 * 32l)
                let q0 = qv[0]
                let q1 = qv[1]
                let ilo = idot(q0 & nibble_mask, av[0]) + idot(q1 & nibble_mask, av[1])
                let ihi = idot((q0 >> 4) & nibble_mask, av[2]) + idot((q1 >> 4) & nibble_mask, av[3])
                let s0 = k4_sc_mn(ksrow + kso + 4l, 2l * js)
                let s1 = k4_sc_mn(ksrow + kso + 4l, 2l * js + 1l)
                isum += s0.x * ilo + s1.x * ihi
                bsum += s0.y * (xbsp[b0 * 2l] + xbsp[b0 * 2l + 1l])
                bsum += s1.y * (xbsp[b0 * 2l + 2l] + xbsp[b0 * 2l + 3l])
            }
            let d8 = xsp[sb]
            acc += float(isum) * (dd * d8)
            acc -= float(bsum) * (dm * d8)
        }
    }
    return acc
}

// kq v2: same Q8_K-form contract and integer folds as the k4 dot_kq (k5 shares the Q4_K scale-block
// form) — only the quant reconstruction differs (nibble | high-bit << 4).
[hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_k5) : float {
    var acc = 0.0
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 160l
            let kso = sb * 20l
            let dd = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            let dm = f16_to_f32(uint(ksrow[kso + 2l]) | (uint(ksrow[kso + 3l]) << 8u))
            let qhb = kqo + 128l   // 32 high-bit bytes; chunk js owns bits 2js (low nibbles) / 2js+1
            var isum = 0
            var bsum = 0
            for (js in range64(4l)) {
                let qb = kqo + 32l * js
                let b0 = sb * 8l + 2l * js
                let ab = b0 * 32l
                let sh = int(js) * 2
                var ilo = 0
                var ihi = 0
                for (l in range64(32l)) {
                    let q = int(kqrow[qb + l])
                    let h = int(kqrow[qhb + l]) >> sh
                    ilo += ((q & 15) | ((h & 1) << 4)) * int(xqp[ab + l])
                    ihi += ((q >> 4) | ((h & 2) << 3)) * int(xqp[ab + 32l + l])
                }
                let s0 = k4_sc_mn(ksrow + kso + 4l, 2l * js)
                let s1 = k4_sc_mn(ksrow + kso + 4l, 2l * js + 1l)
                isum += s0.x * ilo + s1.x * ihi
                bsum += s0.y * (xbsp[b0 * 2l] + xbsp[b0 * 2l + 1l])
                bsum += s1.y * (xbsp[b0 * 2l + 2l] + xbsp[b0 * 2l + 3l])
            }
            let d8 = xsp[sb]
            acc += float(isum) * (dd * d8)
            acc -= float(bsum) * (dm * d8)
        }
    }
    return acc
}

// kq v2: Q8_K-form activations; k6's per-16 SIGNED sub-scales fold as two superblock-wide int
// accumulators — isum = Σ sc·dot, bsum = Σ sc·Σa16 (the -32 offset term) — combined exactly in
// int32 (|isum| < 2^28, |32·bsum| < 2^28), then ONE d·d8 float fold per superblock.
[hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_k6) : float {
    var acc = 0.0
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 192l
            let kso = sb * 18l
            let d = f16_to_f32(uint(ksrow[kso + 16l]) | (uint(ksrow[kso + 17l]) << 8u))
            var isum = 0
            var bsum = 0
            for (half in range64(2l)) {
                let qlb = kqo + 64l * half
                let qhb = kqo + 128l + 32l * half
                for (g in range64(4l)) {   // group g: ql byte (g&1)*32+l, shift 4*(g/2), qh bits 2g
                    let bb = (g & 1l) * 32l
                    let sh = g < 2l ? 0 : 4
                    let hb = int(g) * 2
                    let b = sb * 8l + half * 4l + g   // activation 32-block
                    let ab = b * 32l
                    var i0 = 0
                    var i1 = 0
                    for (l in range64(16l)) {
                        let q0 = ((int(kqrow[qlb + bb + l]) >> sh) & 15) | (((int(kqrow[qhb + l]) >> hb) & 3) << 4)
                        let q1 = ((int(kqrow[qlb + bb + 16l + l]) >> sh) & 15) | (((int(kqrow[qhb + 16l + l]) >> hb) & 3) << 4)
                        i0 += q0 * int(xqp[ab + l])
                        i1 += q1 * int(xqp[ab + 16l + l])
                    }
                    let si = kso + half * 8l + g * 2l
                    let u0 = int(ksrow[si])
                    let u1 = int(ksrow[si + 1l])
                    let s0 = u0 < 128 ? u0 : u0 - 256
                    let s1 = u1 < 128 ? u1 : u1 - 256
                    isum += s0 * i0 + s1 * i1
                    bsum += s0 * xbsp[b * 2l] + s1 * xbsp[b * 2l + 1l]
                }
            }
            acc += float(isum - 32 * bsum) * (d * xsp[sb])
        }
    }
    return acc
}

// q40 (native Q4_0): w = d·(q − 8) with a PER-32 f16 d, so the fold is per BLOCK — facc
// accumulates float(ilo+ihi−8·Σa32)·d over the superblock's 8 blocks, closed by ONE ·d8.
// This per-block/per-superblock fma order is the contract every q40 kernel must reproduce.
[hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_q40) : float {
    var acc = 0.0
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 128l
            let kso = sb * 16l
            var facc = 0.0
            for (blk in range64(8l)) {
                let qb = kqo + 16l * blk
                let b = sb * 8l + blk
                let ab = b * 32l
                var ilo = 0
                var ihi = 0
                for (l in range64(16l)) {
                    let q = int(kqrow[qb + l])
                    ilo += (q & 15) * int(xqp[ab + l])
                    ihi += (q >> 4) * int(xqp[ab + 16l + l])
                }
                let isub = ilo + ihi - 8 * (xbsp[b * 2l] + xbsp[b * 2l + 1l])
                let d = f16_to_f32(uint(ksrow[kso + 2l * blk]) | (uint(ksrow[kso + 2l * blk + 1l]) << 8u))
                facc += float(isub) * d
            }
            acc += facc * xsp[sb]
        }
    }
    return acc
}

//! IQ4_XS row dot off the DISK-ORDER planes: per 32-block idot = sum IQ4NL_LUT[q] * x (signed,
//! exact int), folded as sum sc_blk * idot_blk over the superblock, then * (d * xs). No bsum
//! term — the codebook carries the sign, so xbsp is unused.
[unused_argument(xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_iq4xs) : float => dot_kq_lut(kqrow, ksrow, xqp, xsp, n, iq4nl_lut())

//! The iq3s4 row dot: iq4xs's walk over the iq3s4 codebook (the sub-scale byte is 1 + 2s).
[unused_argument(xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_iq3s4) : float => dot_kq_lut(kqrow, ksrow, xqp, xsp, n, iq3s4_lut())

[hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp), unused_argument(xbsp), arch(at="../ARCHITECTURE_ENGINE_FORMATS_NV4.md#nv4-planes")]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_nv4) : float {
    let lut = mx4_lut()
    var acc = 0.0
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 128l
            let kso = sb * 16l
            var facc = 0.0
            for (blk in range64(8l)) {
                let qb = kqo + 16l * blk
                let ab = (sb * 8l + blk) * 32l
                var ilo = 0
                var ihi = 0
                for (l in range64(16l)) {
                    let q = int(kqrow[qb + l])
                    ilo += int(lut[q & 15]) * int(xqp[ab + l])
                    ihi += int(lut[q >> 4]) * int(xqp[ab + 16l + l])
                }
                facc += float(ilo) * ue4m3_half_f32(uint(ksrow[kso + 2l * blk])) + float(ihi) * ue4m3_half_f32(uint(ksrow[kso + 2l * blk + 1l]))
            }
            acc += facc * xsp[sb]
        }
    }
    return acc
}

//! The LUT-plane row dot the iq4xs and iq3s4 forms share (the disk-order planes, `lut` the format's codebook).
def private dot_kq_lut(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; n : int64; lut : int8[16]) : float {
    var acc = 0.0
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 128l
            let kso = sb * 20l
            let dd = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            var isum = 0
            for (blk in range64(8l)) {
                let ab = (sb * 8l + blk) * 32l
                var idot = 0
                for (l in range64(16l)) {
                    let q = int(kqrow[kqo + blk * 16l + l])
                    idot += int(lut[q & 15]) * int(xqp[ab + l])
                    idot += int(lut[q >> 4]) * int(xqp[ab + 16l + l])
                }
                let u = int(ksrow[kso + 4l + blk])
                isum += (u < 128 ? u : u - 256) * idot
            }
            acc += float(isum) * (dd * xsp[sb])
        }
    }
    return acc
}

//! Q3_K row dot off the DISK-ORDER planes: block b = 4h + j takes the 2-bit lane of qs byte
//! h*32 + l at shift 2j plus hmask bit b of byte l (q in 0..7); k6's fold with the -4 offset:
//! isum = sum sc16 * dot, bsum = sum sc16 * sum a16, then (isum - 4 * bsum) * d * xs per superblock.
[hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_k3) : float {
    var acc = 0.0
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 96l
            let kso = sb * 18l
            let d = f16_to_f32(uint(ksrow[kso + 16l]) | (uint(ksrow[kso + 17l]) << 8u))
            var isum = 0
            var bsum = 0
            for (blk in range64(8l)) {
                let qb = kqo + (blk / 4l) * 32l
                let hb = kqo + 64l
                let sh = int((blk % 4l) * 2l)
                let b = sb * 8l + blk
                let ab = b * 32l
                var i0 = 0
                var i1 = 0
                for (l in range64(16l)) {
                    let q0 = ((int(kqrow[qb + l]) >> sh) & 3) | (((int(kqrow[hb + l]) >> int(blk)) & 1) << 2)
                    let q1 = ((int(kqrow[qb + 16l + l]) >> sh) & 3) | (((int(kqrow[hb + 16l + l]) >> int(blk)) & 1) << 2)
                    i0 += q0 * int(xqp[ab + l])
                    i1 += q1 * int(xqp[ab + 16l + l])
                }
                let u0 = int(ksrow[kso + 2l * blk])
                let u1 = int(ksrow[kso + 2l * blk + 1l])
                let s0 = u0 < 128 ? u0 : u0 - 256
                let s1 = u1 < 128 ? u1 : u1 - 256
                isum += s0 * i0 + s1 * i1
                bsum += s0 * xbsp[b * 2l] + s1 * xbsp[b * 2l + 1l]
            }
            acc += float(isum - 4 * bsum) * (d * xsp[sb])
        }
    }
    return acc
}

//! IQ3_S row dot off the DISK-ORDER planes: per 32-block, grid entry qs[b*8 + 2l + half] |
//! (qh[b] bit << 8) yields four magnitudes, the block's sign bytes flip them, sub-scale
//! (1 + 2s) folds per block; signed weights, so no activation block-sum term.
[unused_argument(xbsp), hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_iq3s) : float {
    var acc = 0.0
    let grid = iq3s_grid()
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 104l
            let kso = sb * 20l
            let d = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            var isum = 0
            for (blk in range64(8l)) {
                let qh = int(kqrow[kqo + 64l + blk])
                let ab = (sb * 8l + blk) * 32l
                var ib = 0
                for (l in range64(4l)) {
                    let sgn = int(kqrow[kqo + 72l + blk * 4l + l])
                    let g1 = grid[int(kqrow[kqo + blk * 8l + 2l * l]) | ((qh << int(8l - 2l * l)) & 256)]
                    let g2 = grid[int(kqrow[kqo + blk * 8l + 2l * l + 1l]) | ((qh << int(7l - 2l * l)) & 256)]
                    ib += grid_octet_dot(g1, g2, sgn, xqp, ab + l * 8l)
                }
                isum += int(int8(ksrow[kso + 4l + blk])) * ib
            }
            acc += float(isum) * (d * xsp[sb])
        }
    }
    return acc
}

//! IQ2_S row dot off the DISK-ORDER planes: per 32-block, grid entry idx | qh<<8 bits yields
//! EIGHT magnitudes (two uint words), the block's sign bytes flip them, per-16 strips (1 + 2s)
//! fold on the eighth-ed d; signed weights, so no activation block-sum term.
[unused_argument(xbsp), hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_iq2s) : float {
    var acc = 0.0
    let grid = iq2s_grid2()
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 72l
            let kso = sb * 20l
            let d8 = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            var isum = 0
            for (b in range64(8l)) {
                let qh = int(kqrow[kqo + 64l + b])
                let ab = (sb * 8l + b) * 32l
                for (l in range64(4l)) {
                    let sgn = int(kqrow[kqo + 32l + b * 4l + l])
                    let idx = int(kqrow[kqo + b * 4l + l]) | ((qh << int(8l - 2l * l)) & 0x300)
                    let g1 = grid[idx * 2]
                    let g2 = grid[idx * 2 + 1]
                    let ig = grid_octet_dot(g1, g2, sgn, xqp, ab + l * 8l)
                    isum += int(ksrow[kso + 4l + 2l * b + l / 2l]) * ig
                }
            }
            acc += float(isum) * (d8 * xsp[sb])
        }
    }
    return acc
}

//! IQ2_XS row dot off the DISK-ORDER planes: each u16 qs word's low 9 bits pick a u64 grid
//! entry (EIGHT magnitudes, two uint words), its high 7 bits pick the ksigns sign byte; per-16
//! strips (1 + 2s) fold on the eighth-ed d; signed weights, so no activation block-sum term.
[unused_argument(xbsp), hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_iq2xs) : float {
    var acc = 0.0
    let grid = iq2xs_grid2()
    let ksg = ksigns_iq2xs()
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 64l
            let kso = sb * 20l
            let d8 = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            var isum = 0
            for (b in range64(8l)) {
                let ab = (sb * 8l + b) * 32l
                for (l in range64(4l)) {
                    let w16 = uint(kqrow[kqo + (b * 4l + l) * 2l]) | (uint(kqrow[kqo + (b * 4l + l) * 2l + 1l]) << 8u)
                    let sgn = int(ksg[int(w16 >> 9u)])
                    let idx = int(w16 & 511u)
                    let g1 = grid[idx * 2]
                    let g2 = grid[idx * 2 + 1]
                    let ig = grid_octet_dot(g1, g2, sgn, xqp, ab + l * 8l)
                    isum += int(ksrow[kso + 4l + 2l * b + l / 2l]) * ig
                }
            }
            acc += float(isum) * (d8 * xsp[sb])
        }
    }
    return acc
}

//! IQ2_XXS row dot off the DISK-ORDER planes: per block the aux32[0] bytes pick u64 grid
//! entries (two words each), the aux32[1]'s 7-bit ksigns indices flip signs, its top nibble
//! is the per-32 strip (1 + 2s) on the eighth-ed d - decoded to the strip byte at transcode.
[unused_argument(xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_iq2xxs) : float {
    var acc = 0.0
    let grid = iq2xxs_grid2()
    let signs = ksigns_iq2xs()
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 64l
            let kso = sb * 20l
            let d8 = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            var isum = 0
            for (blk in range64(8l)) {
                let aux = uint(kqrow[kqo + blk * 8l + 4l]) | (uint(kqrow[kqo + blk * 8l + 5l]) << 8u) | (uint(kqrow[kqo + blk * 8l + 6l]) << 16u) | (uint(kqrow[kqo + blk * 8l + 7l]) << 24u)
                let ab = (sb * 8l + blk) * 32l
                var ib = 0
                for (l in range64(4l)) {
                    let sgn = int(signs[int((aux >> uint(7l * l)) & 127u)])
                    let idx = int(kqrow[kqo + blk * 8l + l])
                    let g1 = grid[idx * 2]
                    let g2 = grid[idx * 2 + 1]
                    ib += grid_octet_dot(g1, g2, sgn, xqp, ab + l * 8l)
                }
                isum += int(ksrow[kso + 4l + blk]) * ib
            }
            acc += float(isum) * (d8 * xsp[sb])
        }
    }
    return acc
}

def dot_kq(_kqrow : uint8 const?; _ksrow : uint8 const?; _xqp : int8 const?; _xsp : float const?; _xbsp : int const?; _n : int64; _f : KqTag_q8) : float {
    panic("q8 has no kq plane dot")
    return 0.0
}

def dot_kq(_kqrow : uint8 const?; _ksrow : uint8 const?; _xqp : int8 const?; _xsp : float const?; _xbsp : int const?; _n : int64; _f : KqTag_q51) : float {
    panic("q51 rides the per-32 planes, not the kq lattice dot")
    return 0.0
}

def dot_kq(_kqrow : uint8 const?; _ksrow : uint8 const?; _xqp : int8 const?; _xsp : float const?; _xbsp : int const?; _n : int64; _f : KqTag_mx4) : float {
    panic("mx4 rides the expert nibble planes, not the kq lattice dot")
    return 0.0
}

def dot_kq(_kqrow : uint8 const?; _ksrow : uint8 const?; _xqp : int8 const?; _xsp : float const?; _xbsp : int const?; _n : int64; _f : KqTag_iq4nl32) : float {
    panic("iq4nl32 rides the per-32 planes, not the kq lattice dot")
    return 0.0
}

def dot_kq(_kqrow : uint8 const?; _ksrow : uint8 const?; _xqp : int8 const?; _xsp : float const?; _xbsp : int const?; _n : int64; _f : KqTag_q4032) : float {
    panic("q4032 rides the per-32 planes, not the kq lattice dot")
    return 0.0
}

def dot_kq(_kqrow : uint8 const?; _ksrow : uint8 const?; _xqp : int8 const?; _xsp : float const?; _xbsp : int const?; _n : int64; _f : KqTag_q5032) : float {
    panic("q5032 rides the per-32 planes, not the kq lattice dot")
    return 0.0
}

//! Rows [rb, re) of a kq region off the DISK-ORDER planes through the format's dot: y[i] = the
//! dot of row i, the row strides off the descriptor. Generic in the tag; the gen tier's row tails
//! and the fused chains thread it.
def kq_rows_kernel(f : auto(TT); fmt : KqFmt; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        for (i in range64(rb, re)) {
            yp[i] = dot_kq(kqp + i * nsb * qsb, ksp + i * nsb * ssb, xqp, xsp, xbsp, n, f)
        }
    }
}

[unused_argument(xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_iq3xxs) : float {
    var acc = 0.0
    let grid = iq3xxs_grid()
    let signs = ksigns_iq2xs()
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 96l
            let kso = sb * 20l
            let d = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            var isum = 0
            for (blk in range64(8l)) {
                let aux = uint(kqrow[kqo + 64l + blk * 4l]) | (uint(kqrow[kqo + 64l + blk * 4l + 1l]) << 8u) | (uint(kqrow[kqo + 64l + blk * 4l + 2l]) << 16u) | (uint(kqrow[kqo + 64l + blk * 4l + 3l]) << 24u)
                let ab = (sb * 8l + blk) * 32l
                var ib = 0
                for (l in range64(4l)) {
                    let sgn = int(signs[int((aux >> uint(7l * l)) & 127u)])
                    let g1 = grid[int(kqrow[kqo + blk * 8l + 2l * l])]
                    let g2 = grid[int(kqrow[kqo + blk * 8l + 2l * l + 1l])]
                    ib += grid_octet_dot(g1, g2, sgn, xqp, ab + l * 8l)
                }
                isum += int(int8(ksrow[kso + 4l + blk])) * ib
            }
            acc += float(isum) * (d * xsp[sb])
        }
    }
    return acc
}

[unused_argument(xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_iq4nl) : float {
    var acc = 0.0
    let lut = iq4nl_lut()
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 128l
            let kso = sb * 16l
            var facc = 0.0
            for (blk in range64(8l)) {
                let ab = (sb * 8l + blk) * 32l
                var idot = 0
                for (l in range64(16l)) {
                    let q = int(kqrow[kqo + blk * 16l + l])
                    idot += int(lut[q & 15]) * int(xqp[ab + l])
                    idot += int(lut[q >> 4]) * int(xqp[ab + 16l + l])
                }
                let d = f16_to_f32(uint(ksrow[kso + 2l * blk]) | (uint(ksrow[kso + 2l * blk + 1l]) << 8u))
                facc += float(idot) * d
            }
            acc += facc * xsp[sb]
        }
    }
    return acc
}

//! Q2_K row dot off the DISK-ORDER planes: per 16-elem group g, sc/mn = the group's nibble
//! pair, q = the unsigned 2-bit lane; the k4/k5 fold - isum on d*sc, the min term through the
//! activation 16-sums (bsum on dmin*mn).
[hint(unsafe_range_check, noalias = kqrow, noalias = ksrow, noalias = xqp, noalias = xsp, noalias = xbsp)]
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; _f : KqTag_k2) : float {
    var acc = 0.0
    unsafe {
        for (sb in range64(n / 256l)) {
            let kqo = sb * 64l
            let kso = sb * 20l
            let dd = f16_to_f32(uint(ksrow[kso]) | (uint(ksrow[kso + 1l]) << 8u))
            let dm = f16_to_f32(uint(ksrow[kso + 2l]) | (uint(ksrow[kso + 3l]) << 8u))
            var isum = 0
            var bsum = 0
            for (g in range64(16l)) {
                let scb = int(ksrow[kso + 4l + g])
                let qb = kqo + (g / 8l) * 32l + (g % 2l) * 16l
                let sh = int(((g % 8l) / 2l) * 2l)
                let ab = sb * 256l + g * 16l
                var ig = 0
                for (l in range64(16l)) {
                    ig += ((int(kqrow[qb + l]) >> sh) & 3) * int(xqp[ab + l])
                }
                isum += (scb & 15) * ig
                bsum += (scb >> 4) * xbsp[sb * 16l + g]
            }
            let d8 = xsp[sb]
            acc += float(isum) * (dd * d8)
            acc -= float(bsum) * (dm * d8)
        }
    }
    return acc
}

def private kq_gemv_kernel(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {   // nolint:STYLE037 - the stamp's ladder, one arm per KqFmt member
    let kf = kq_fmt_of_id(fmt, "kq_gemv_kernel")
    var myp = yp
    kq_fmt_stamp(kf) $(F) {
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(rb, re) {
            kq_rows_kernel(F, kf, myp, kqp, ksp, xqp, xsp, xbsp, n, int64(rb), int64(re))
        }
    }
}

//! q40 grp row dequant: the k4 nibble tiling with a per-block f16 d plane — w = d·(q − 8).
def private dequant_q40_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 128l * mr
            let sb = sbi * 16l * mr
            for (blk in range64(8l)) {
                let kb = sbi * 256l + blk * 32l
                let d = f16_to_f32(uint(ksg[sb + blk * 2l * mr + 2l * r]) | (uint(ksg[sb + blk * 2l * mr + 2l * r + 1l]) << 8u))
                for (j in range64(4l)) {
                    for (t in range64(4l)) {
                        let nib = uint(kqg[qb + ((blk * 4l + j) * mr + r) * 4l + t])
                        dst[kb + j * 4l + t] = d * float(int(nib & 15u) - 8)
                        dst[kb + 16l + j * 4l + t] = d * float(int(nib >> 4u) - 8)
                    }
                }
            }
        }
    }
}

//! iq4xs / iq3s4 grp row dequant: the k4 tiling + the k4 scale-row interleave (d, signed int8 sc, no
//! min) — w = (d·sc)·lut[q] over the format's codebook.
def private dequant_lut_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?; lut : int8[16]) {
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 128l * mr
            let sb = sbi * 20l * mr
            let d = f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u))
            for (blk in range64(8l)) {
                let kb = sbi * 256l + blk * 32l
                let u = int(ksg[sb + 4l * mr + blk * mr + r])
                let dl = d * float(u < 128 ? u : u - 256)
                for (j in range64(4l)) {
                    for (t in range64(4l)) {
                        let nib = int(kqg[qb + ((blk * 4l + j) * mr + r) * 4l + t])
                        dst[kb + j * 4l + t] = dl * float(lut[nib & 15])
                        dst[kb + 16l + j * 4l + t] = dl * float(lut[nib >> 4])
                    }
                }
            }
        }
    }
}

def private dequant_nv4_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    let lut = mx4_lut()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 128l * mr
            let sb = sbi * 16l * mr
            for (blk in range64(8l)) {
                let kb = sbi * 256l + blk * 32l
                let so = sb + blk * 2l * mr + 2l * r
                let slo = ue4m3_half_f32(uint(ksg[so]))
                let shi = ue4m3_half_f32(uint(ksg[so + 1l]))
                for (j in range64(4l)) {
                    for (t in range64(4l)) {
                        let nib = int(kqg[qb + ((blk * 4l + j) * mr + r) * 4l + t])
                        dst[kb + j * 4l + t] = float(int(lut[nib & 15])) * slo
                        dst[kb + 16l + j * 4l + t] = float(int(lut[nib >> 4])) * shi
                    }
                }
            }
        }
    }
}

//! w = (d * sc16) * (q - 4)
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#kq-subblock-planes")]
def private dequant_k3_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 96l * mr
            let sb = sbi * 18l * mr
            let d = f16_to_f32(uint(ksg[sb + 16l * mr + 2l * r]) | (uint(ksg[sb + 16l * mr + 2l * r + 1l]) << 8u))
            for (blk in range64(8l)) {
                let kb = sbi * 256l + blk * 32l
                let s0 = d * float(int(int8(ksg[sb + 2l * blk * mr + r])))
                let s1 = d * float(int(int8(ksg[sb + (2l * blk + 1l) * mr + r])))
                for (l4 in range64(8l)) {
                    for (t in range64(4l)) {
                        let q2 = (int(kqg[qb + ((blk * 2l + l4 / 4l) * mr + r) * 4l + t]) >> int((l4 % 4l) * 2l)) & 3
                        let hb = (int(kqg[qb + 64l * mr + (blk * mr + r) * 4l + t]) >> int(l4)) & 1
                        dst[kb + l4 * 4l + t] = (l4 < 4l ? s0 : s1) * float((q2 | (hb << 2)) - 4)
                    }
                }
            }
        }
    }
}

//! iq3s grp row dequant: 26 uniform 4-byte columns per superblock row ([qs][qh][signs] in
//! place), the k4-shaped scale interleave — w = (d * (1 + 2s)) * grid_byte * sign.
def private dequant_iq3s_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    let grid = iq3s_grid()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 104l * mr
            let sb = sbi * 20l * mr
            let d = f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u))
            for (blk in range64(8l)) {
                let db = d * float(int(int8(ksg[sb + 4l * mr + blk * mr + r])))
                let hp = 64l + blk
                let qh = int(kqg[qb + ((hp / 4l) * mr + r) * 4l + hp % 4l])
                for (l in range64(4l)) {
                    let sp = 72l + blk * 4l + l
                    let sgn = int(kqg[qb + ((sp / 4l) * mr + r) * 4l + sp % 4l])
                    let q1p = blk * 8l + 2l * l
                    let q2p = q1p + 1l
                    let g1 = grid[int(kqg[qb + ((q1p / 4l) * mr + r) * 4l + q1p % 4l]) | ((qh << int(8l - 2l * l)) & 256)]
                    let g2 = grid[int(kqg[qb + ((q2p / 4l) * mr + r) * 4l + q2p % 4l]) | ((qh << int(7l - 2l * l)) & 256)]
                    store_grid_pair(dst, sbi * 256l + blk * 32l + l * 8l, db, g1, g2, sgn)
                }
            }
        }
    }
}

//! iq3xxs grp row dequant: 24 uniform 4-byte columns per superblock row ([qs][aux] in place),
//! the iq3s scale interleave — w = (d_halved * (2ls + 1)) * halved_grid_byte * sign.
def private dequant_iq3xxs_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    let grid = iq3xxs_grid()
    let signs = ksigns_iq2xs()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 96l * mr
            let sb = sbi * 20l * mr
            let d = f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u))
            for (blk in range64(8l)) {
                let db = d * float(int(int8(ksg[sb + 4l * mr + blk * mr + r])))
                var aux = 0u
                for (t in range64(4l)) {
                    let ap = 64l + blk * 4l + t
                    aux |= uint(kqg[qb + ((ap / 4l) * mr + r) * 4l + ap % 4l]) << uint(8l * t)
                }
                for (l in range64(4l)) {
                    let sgn = int(signs[int((aux >> uint(7l * l)) & 127u)])
                    let q1p = blk * 8l + 2l * l
                    let q2p = q1p + 1l
                    let g1 = grid[int(kqg[qb + ((q1p / 4l) * mr + r) * 4l + q1p % 4l])]
                    let g2 = grid[int(kqg[qb + ((q2p / 4l) * mr + r) * 4l + q2p % 4l])]
                    store_grid_pair(dst, sbi * 256l + blk * 32l + l * 8l, db, g1, g2, sgn)
                }
            }
        }
    }
}

//! iq4nl grp row dequant: q40's tiling and scale interleave, the codebook in place of (q - 8).
def private dequant_iq4nl_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    let lut = iq4nl_lut()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 128l * mr
            let sb = sbi * 16l * mr
            for (blk in range64(8l)) {
                let kb = sbi * 256l + blk * 32l
                let d = f16_to_f32(uint(ksg[sb + blk * 2l * mr + 2l * r]) | (uint(ksg[sb + blk * 2l * mr + 2l * r + 1l]) << 8u))
                for (j in range64(4l)) {
                    for (t in range64(4l)) {
                        let nib = uint(kqg[qb + ((blk * 4l + j) * mr + r) * 4l + t])
                        dst[kb + j * 4l + t] = d * float(int(lut[int(nib & 15u)]))
                        dst[kb + 16l + j * 4l + t] = d * float(int(lut[int(nib >> 4u)]))
                    }
                }
            }
        }
    }
}

//! k2 grp row dequant: the 2-bit lanes stay in place as 16 four-byte columns x mr; scale row =
//! [16 sc/min bytes interleaved x mr][mr x f16 d][mr x f16 dmin]. w = (d*sc)*q - (dmin*mn).
def private dequant_k2_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 64l * mr
            let sb = sbi * 20l * mr
            let hd = sb + 16l * mr + 2l * r
            let d = f16_to_f32(uint(ksg[hd]) | (uint(ksg[hd + 1l]) << 8u))
            let hm = sb + 18l * mr + 2l * r
            let dmin = f16_to_f32(uint(ksg[hm]) | (uint(ksg[hm + 1l]) << 8u))
            for (g in range64(16l)) {
                let scb = int(ksg[sb + g * mr + r])
                let dl = d * float(scb & 15)
                let ml = dmin * float(scb >> 4)
                let base = (g / 8l) * 8l + (g % 2l) * 4l
                let sh = int(((g % 8l) / 2l) * 2l)
                for (c4 in range64(4l)) {
                    for (t in range64(4l)) {
                        let q = (int(kqg[qb + ((base + c4) * mr + r) * 4l + t]) >> sh) & 3
                        dst[sbi * 256l + g * 16l + c4 * 4l + t] = dl * float(q) - ml
                    }
                }
            }
        }
    }
}

//! iq2s grp row dequant: 18 uniform 4-byte columns per superblock row ([idx][signs][qh] in
//! place), the k4-shaped scale interleave with 16 strips - w = (d8 * (1 + 2s)) * grid * sign.
def private dequant_iq2s_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    let grid = iq2s_grid2()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 72l * mr
            let sb = sbi * 20l * mr
            let d8 = f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u))
            for (b in range64(8l)) {
                let hp = 64l + b
                let qh = int(kqg[qb + ((hp / 4l) * mr + r) * 4l + hp % 4l])
                for (l in range64(4l)) {
                    let db = d8 * float(int(ksg[sb + 4l * mr + (2l * b + l / 2l) * mr + r]))
                    let sp = 32l + b * 4l + l
                    let sgn = int(kqg[qb + ((sp / 4l) * mr + r) * 4l + sp % 4l])
                    let ip = b * 4l + l
                    let idx = int(kqg[qb + ((ip / 4l) * mr + r) * 4l + ip % 4l]) | ((qh << int(8l - 2l * l)) & 0x300)
                    let g1 = grid[idx * 2]
                    let g2 = grid[idx * 2 + 1]
                    store_grid_pair(dst, sbi * 256l + b * 32l + l * 8l, db, g1, g2, sgn)
                }
            }
        }
    }
}

//! iq2xs grp row dequant: 16 uniform 4-byte columns per superblock row (the u16 qs words in
//! place), the k4-shaped scale interleave with 16 strips - w = (d8 * (1 + 2s)) * grid * sign.
def private dequant_iq2xs_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    let grid = iq2xs_grid2()
    let ksgn = ksigns_iq2xs()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 64l * mr
            let sb = sbi * 20l * mr
            let d8 = f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u))
            for (b in range64(8l)) {
                for (l in range64(4l)) {
                    let db = d8 * float(int(ksg[sb + 4l * mr + (2l * b + l / 2l) * mr + r]))
                    let lp = (b * 4l + l) * 2l
                    let w16 = uint(kqg[qb + ((lp / 4l) * mr + r) * 4l + lp % 4l]) | (uint(kqg[qb + ((lp / 4l) * mr + r) * 4l + lp % 4l + 1l]) << 8u)
                    let sgn = int(ksgn[int(w16 >> 9u)])
                    let idx = int(w16 & 511u)
                    let g1 = grid[idx * 2]
                    let g2 = grid[idx * 2 + 1]
                    store_grid_pair(dst, sbi * 256l + b * 32l + l * 8l, db, g1, g2, sgn)
                }
            }
        }
    }
}

//! iq2xxs grp row dequant: 16 uniform 4-byte columns per superblock row - column 2b carries
//! block b's four grid bytes, column 2b+1 its aux32 (signs + the per-32 scale nibble, whose
//! strip byte the scale row already carries).
def private dequant_iq2xxs_row_grp(kqg : uint8 const?; ksg : uint8 const?; r, mr, nsb : int64; var dst : float?) {
    let grid = iq2xxs_grid2()
    let ksgn = ksigns_iq2xs()
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 64l * mr
            let sb = sbi * 20l * mr
            let d8 = f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u))
            for (b in range64(8l)) {
                let db = d8 * float(int(ksg[sb + 4l * mr + b * mr + r]))
                let ac = (2l * b + 1l) * mr + r
                let aux = uint(kqg[qb + ac * 4l]) | (uint(kqg[qb + ac * 4l + 1l]) << 8u) | (uint(kqg[qb + ac * 4l + 2l]) << 16u) | (uint(kqg[qb + ac * 4l + 3l]) << 24u)
                let gc = (2l * b) * mr + r
                for (l in range64(4l)) {
                    let sgn = int(ksgn[int((aux >> uint(7l * l)) & 127u)])
                    let idx = int(kqg[qb + gc * 4l + l])
                    let g1 = grid[idx * 2]
                    let g2 = grid[idx * 2 + 1]
                    store_grid_pair(dst, sbi * 256l + b * 32l + l * 8l, db, g1, g2, sgn)
                }
            }
        }
    }
}

//! Dequant one row off the grp<mr>-REPACKED K-quant planes (repack_k4/k5/k6_grp layout).
//! kqg/ksg = row's GROUP plane base (group g = row/mr); r = row % mr.
//! embed_row's kq_repacked path — per-token, so scalar is fine.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#kq-subblock-planes")]
def dequant_kq_row_grp(fmt : int64; kqg : uint8 const?; ksg : uint8 const?; r, mr, n : int64; var dst : float?) {   // nolint:STYLE037,STYLE038 — the flat per-format dispatch (one arm per kq format) is the honest shape
    let nsb = n / 256l
    let qsb = kq_qsb(int(fmt))
    let ssb = kq_ssb(int(fmt))
    if (fmt == 40l) {
        dequant_q40_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 44l) {
        dequant_lut_row_grp(kqg, ksg, r, mr, nsb, dst, iq4nl_lut())
        return
    }
    if (fmt == 35l) {
        dequant_lut_row_grp(kqg, ksg, r, mr, nsb, dst, iq3s4_lut())
        return
    }
    if (fmt == 3l) {
        dequant_k3_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 33l) {
        dequant_iq3s_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 34l) {
        dequant_iq3xxs_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 45l) {
        dequant_iq4nl_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 49l) {
        dequant_nv4_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 2l) {
        dequant_k2_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 23l) {
        dequant_iq2s_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 24l) {
        dequant_iq2xs_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    if (fmt == 25l) {
        dequant_iq2xxs_row_grp(kqg, ksg, r, mr, nsb, dst)
        return
    }
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * qsb * mr
            let sb = sbi * ssb * mr
            let dk6 = fmt == 6l ? f16_to_f32(uint(ksg[sb + 16l * mr + 2l * r]) | (uint(ksg[sb + 16l * mr + 2l * r + 1l]) << 8u)) : 0.0
            let dk4d = fmt != 6l ? f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u)) : 0.0
            let dk4m = fmt != 6l ? f16_to_f32(uint(ksg[sb + 2l * mr + 2l * r]) | (uint(ksg[sb + 2l * mr + 2l * r + 1l]) << 8u)) : 0.0
            for (blk in range64(8l)) {
                let kb = sbi * 256l + blk * 32l
                var s0 = 0.0
                var s1 = 0.0
                var o = 0.0
                if (fmt == 6l) {
                    s0 = dk6 * float(int(int8(ksg[sb + 2l * blk * mr + r])))
                    s1 = dk6 * float(int(int8(ksg[sb + (2l * blk + 1l) * mr + r])))
                } else {
                    // kq v2 (k4/k5): decoded u8 sc/mn planes — w = (d*sc)*q - (dmin*mn), disk-exact
                    s0 = dk4d * float(int(ksg[sb + 4l * mr + blk * mr + r]))
                    s1 = s0
                    o = dk4m * float(int(ksg[sb + 12l * mr + blk * mr + r]))
                }
                for (j in range64(4l)) {
                    let hb = fmt == 5l ? int(kqg[qb + 128l * mr + (blk * 4l + j) * mr + r]) : 0
                    let sj = uint(2l * j)
                    for (t in range64(4l)) {
                        let nib = uint(kqg[qb + ((blk * 4l + j) * mr + r) * 4l + t])
                        var qlo = int(nib & 15u)
                        var qhi = int(nib >> 4u)
                        if (fmt == 5l) {
                            qlo |= ((hb >> int(t)) & 1) << 4
                            qhi |= ((hb >> int(4l + t)) & 1) << 4
                        } elif (fmt == 6l) {
                            let hbase = qb + 128l * mr + blk * 2l * mr * 4l
                            let h0 = uint(kqg[hbase + r * 4l + t])
                            let h1 = uint(kqg[hbase + (mr + r) * 4l + t])
                            qlo |= int(((h0 >> sj) & 3u) << 4u)
                            qhi |= int(((h1 >> sj) & 3u) << 4u)
                        }
                        if (fmt == 6l) {
                            dst[kb + j * 4l + t] = s0 * (float(qlo) - 32.0)
                            dst[kb + 16l + j * 4l + t] = s1 * (float(qhi) - 32.0)
                        } else {
                            dst[kb + j * 4l + t] = s0 * float(qlo) - o
                            dst[kb + 16l + j * 4l + t] = s1 * float(qhi) - o
                        }
                    }
                }
            }
        }
    }
}

//! K-quant GEMV over a plane pair at plane-local element offset `woff` (blob+offset form, the
//! dispatch shape every call site uses). fmt: 4/5/6 selects the sub-format; rows land at
//! y[yoff .. yoff+d) (batched callers pass the token row base).
def matmul_kq(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; woff : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d : int64; yoff : int64 = 0l) {
    let sb = woff / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        kq_gemv_kernel(fmt, addr(y[yoff]), addr(kq[sb * qsb]), addr(ks[sb * ssb]),
                       addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, d)
    }
}

//! `matmul_kq` over DISK-ORDER planes handed as the region's own base pointers (a borrowed plane's region at a byte offset).
def matmul_kq_at(fmt : int; var y : float?; kq, ks : uint8 const?; xq : int8 const?; xs : float const?; xbs : int const?; n, d : int64) {
    kq_gemv_kernel(fmt, y, kq, ks, xq, xs, xbs, n, d)
}

//! Region-list K-quant GEMV over DISK-ORDER planes (groupn twin of matmul_kq): `offs` packs
//! (weight, activation) element-offset pairs; region r's rows land at y[r*d .. r*d+d).
//! No bias form — the only biased-experts arch (gpt-oss) is mx4.
def matmul_kq_groupn(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; offs : array<int64> | #; nregions : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d : int64) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        let kqp = addr<uint8 const?>(kq[0])
        let ksp = addr<uint8 const?>(ks[0])
        let offp = addr<int64 const?>(offs[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let xbsp = addr<int const?>(xbs[0])
        var myp = addr(y[0])
        let kf = kq_fmt_of_id(fmt, "matmul_kq_groupn")
        kq_fmt_stamp(kf) $(F) {
            maybe_parallel_for(0, int(nregions * d), matmul_chunks_gemv(int(nregions * d), 1, n * d * nregions)) $(rb, re) {
                unsafe {
                    for (i in range(rb, re)) {
                        let ii = int64(i)
                        let r = ii / d
                        let row = ii % d
                        let sb = offp[r * 2l] / 256l + row * nsb
                        let xoff = offp[r * 2l + 1l]
                        myp[ii] = dot_kq(kqp + sb * qsb, ksp + sb * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n, F)
                    }
                }
            }
        }
    }
}

//! MXFP4·Q8 dot, scalar reference form: each nibble
//! pairs low nibble w/ xq[j], high w/ xq[j+16] via doubled-e2m1 `lut`; flush order matches NEON dot_mx4q8 (agrees to reassociation).
//! Not `private`: the mx4 unit-test reference, callable from lifted worker lambdas.
[hint(unsafe_range_check, noalias = lut, noalias = wn, noalias = we, noalias = xq, noalias = xs)]
def dot_mx4q8_scalar(lut : int8 const?; wn : uint8 const?; we : uint8 const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var f = 0.0
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * 16l
            let xb = bi * 32l
            var s = 0
            for (j in range64(16l)) {
                let b = int(wn[wb + j])
                s += int(lut[b & 15]) * int(xq[xb + j])
                s += int(lut[b >> 4]) * int(xq[xb + j + 16l])
            }
            f += float(s) * e8m0_half(uint(we[bi])) * xs[bi]
        }
    }
    return f
}

[hint(unsafe_range_check, noalias = lut, noalias = wn, noalias = we, noalias = xq, noalias = xs)]
def template dot_mx4q8_template(lut : int8 const ?; wn : uint8 const ?; we : uint8 const ?; xq : int8 const ?; xs : float const ?; n : int64) : float {
    let nb = n / 32l
    var f = 0.0
    unsafe {
        let lutv = reinterpret<byte16 const?>(lut)[0]
        let wv = reinterpret<ubyte16 const?>(wn)
        let xv = reinterpret<byte16 const?>(xq)
        for [tune = 1] (bi in range64(nb)) {   // fallback u2 = two blocks per iteration
            let nib = wv[bi]
            var acc = idot4(shuffle(lutv, nib), xv[bi * 2l])
            acc = idot4(acc, shuffle(lutv, nib >> 4), xv[bi * 2l + 1l])
            f += float(acc.x + acc.y + acc.z + acc.w) * e8m0_half(uint(we[bi])) * xs[bi]
        }
    }
    return f
}

//! MXFP4·Q8 dot in plain das: one nibble load, two shuffle LUT
//! lookups (low via the &15 mask, high via the per-byte >> 4), each idot4'd against the Q8
//! activation (one TBL + one SDOT each on arm64 JIT).
[hint(unsafe_range_check, noalias = lut, noalias = wn, noalias = we, noalias = xq, noalias = xs), tuned(fallback = "u2")]
def dot_mx4q8(lut : int8 const?; wn : uint8 const?; we : uint8 const?; xq : int8 const?; xs : float const?; n : int64) : float {}

def template mx4q8_kernel_template(var yp : float ?; wn : uint8 const ?; we : uint8 const ?; xqp : int8 const ?; xsp : float const ?; n, d : int64) : void {
    let nb = n / 32l
    var myp = yp
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), Q8SPLIT(false), n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    myp[i] = MX4DOT(lutp, wn + int64(i) * (n / 2l), we + int64(i) * nb, xqp, xsp, n)
                }
            }
        }
    }
}

[from_template(src = "mx4q8_kernel_template", MX4DOT = "dot_mx4q8_scalar", Q8SPLIT = "q8_split_portable")]
def mx4q8_kernel(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {}

[from_template(src = "mx4q8_kernel_template", MX4DOT = "dot_mx4q8", Q8SPLIT = "q8_split_idot4x4")]
def mx4q8_kernel_idot4x4(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {}

//! The idot4 mx4 row-range core (mx4_rows slot): rows [rb, re) over dot_mx4q8, no dispatch - the
//! fused MoE expert chain threads it.
def mx4q8_rows_kernel_idot4x4(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {
    let nb = n / 32l
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])
        for (i in range64(rb, re)) {
            yp[i] = dot_mx4q8(lutp, wn + i * (n / 2l), we + i * nb, xqp, xsp, n)
        }
    }
}

// Portable mx4 batch: rows-outer/tokens-inner over the scalar row dot. NOT flagged mx4_native_batch
// — grouped prefill keeps the expand-to-Q8 + vectorized batch path instead; this is the probe/A-B
// reference (bit-identical to ntok × mx4q8_kernel).
def mx4q8_batch_kernel(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    var myp = yp
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(d), matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let wrow = wn + int64(i) * (n / 2l)
                    let erow = we + int64(i) * nb
                    for (tk in range64(ntok)) {
                        myp[tk * d + int64(i)] = dot_mx4q8_scalar(lutp, wrow, erow, xqp + tk * n, xsp + tk * nb, n)
                    }
                }
            }
        }
    }
}

// Portable mx4 batch groupN: per (region, row) scalar dots over each region's token-row range —
// the probe/A-B reference twin of mx4q8_batch_kernel (NOT flagged; see that kernel's note).
def mx4q8_batch_groupn_kernel(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(nregions * d), matmul_chunks(int(nregions * d), get_q8_batch_chunks_per_job(), n * d * nregions)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let ii = int64(i)
                    let r = ii / d
                    let row = ii % d
                    let woff = offs[r * 3l]
                    let row0 = offs[r * 3l + 1l]
                    let cnt = offs[r * 3l + 2l]
                    let wrow = wn + woff / 2l + row * (n / 2l)
                    let erow = we + woff / 32l + row * nb
                    for (tk in range64(cnt)) {
                        var v = dot_mx4q8_scalar(lutp, wrow, erow, xqp + (row0 + tk) * n, xsp + (row0 + tk) * nb, n)
                        if (bp != null) {
                            v += bp[boffs[r] + row]
                        }
                        myp[(row0 + tk) * d + row] = v
                    }
                }
            }
        }
    }
}

def template mx4q8_groupn_kernel_template(var yp : float ?; wn : uint8 const ?; we : uint8 const ?; offs : int64 const ?; nregions : int64; xqp : int8 const ?; xsp : float const ?; bp : float const ?; boffs : int64 const ?; n, d : int64) : void {
    let nb = n / 32l
    var myp = yp
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(nregions * d), matmul_chunks_gemv(int(nregions * d), Q8SPLIT(false), n * d * nregions)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let ii = int64(i)
                    let r = ii / d
                    let row = ii % d
                    let woff = offs[r * 2l]
                    let xoff = offs[r * 2l + 1l]
                    var v = MX4DOT(lutp, wn + woff / 2l + row * (n / 2l), we + woff / 32l + row * nb,
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

[from_template(src = "mx4q8_groupn_kernel_template", MX4DOT = "dot_mx4q8_scalar", Q8SPLIT = "q8_split_portable")]
def mx4q8_groupn_kernel(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {}

[from_template(src = "mx4q8_groupn_kernel_template", MX4DOT = "dot_mx4q8", Q8SPLIT = "q8_split_idot4x4")]
def mx4q8_groupn_kernel_idot4x4(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {}

// ===== the per-32 rail: scalar leaves, shapes over the tag =====

//! IQ4_NL·Q8 dot, scalar reference — THE iq4nl32 float-order
//! contract: per 32-block isum = Σ lut[q]·a (elem k off byte k's low
//! nibble, k + 16 off its high); f += d·isum·xs[b], no asum.
def dot_iq4nl32q8_scalar(wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?;
                         n : int64) : float => dot_nl32q8_scalar(wq, ws, xq, xs, n, iq4nl_lut())

//! the q4032 twin: the contract under the linear table (q - 8)
def dot_q4032q8_scalar(wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?;
                       n : int64) : float => dot_nl32q8_scalar(wq, ws, xq, xs, n, q40_lut())

//! a per-32 codebook plane's scalar dot under the 16-entry `lut`
[hint(unsafe_range_check, noalias = wq, noalias = ws, noalias = xq, noalias = xs)]
def private dot_nl32q8_scalar(wq : uint8 const?; ws : uint8 const?; xq : int8 const?;
                              xs : float const?; n : int64; lut : int8[16]) : float {
    let nb = n / KQ_B32_ELEMS
    var f = 0.0
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * IQ4NL32_QB
            let xb = bi * KQ_B32_ELEMS
            var isum = 0
            for (j in range64(B32_NIB_BYTES)) {
                let b = int(wq[wb + j])
                isum += (int(lut[b & 15]) * int(xq[xb + j])
                    + int(lut[b >> 4]) * int(xq[xb + j + B32_NIB_BYTES]))
            }
            let sp = ws + bi * IQ4NL32_SB
            let d = f16_to_f32(uint(sp[0]) | (uint(sp[1]) << 8u))
            f += d * float(isum) * xs[bi]
        }
    }
    return f
}

//! a 5-bit level off its code `q`: q51's q, q5032's q - 16
def b5_level(q : int; _f : KqTag_q51) : int => q
def b5_level(q : int; _f : KqTag_q5032) : int => q - 16

//! a 5-bit block's float fold off its scales at `sp`: d·isum + m·asum
def b5_fold(sp : uint8 const?; isum, asum : int; _f : KqTag_q51) : float {
    let d = f16_to_f32(uint(unsafe(sp[0])) | (uint(unsafe(sp[1])) << 8u))
    let m = f16_to_f32(uint(unsafe(sp[2])) | (uint(unsafe(sp[3])) << 8u))
    return d * float(isum) + m * float(asum)
}

//! ... and q5032's d·isum: the signed level reads no activation sum
[unused_argument(asum)]
def b5_fold(sp : uint8 const?; isum, asum : int; _f : KqTag_q5032) : float {
    let d = f16_to_f32(uint(unsafe(sp[0])) | (uint(unsafe(sp[1])) << 8u))
    return d * float(isum)
}

//! eight expanded bytes off nibbles `lo` and qh bits `s`: q51's code
def b5_bytes(lo, s : uint64; _f : KqTag_q51) : uint64 => lo | s
//! ... and q5032's code - 16: the sign byte where the qh bit is clear
def b5_bytes(lo, s : uint64; _f : KqTag_q5032) : uint64 => lo | ((s ^ 0x1010101010101010ul) * 15ul)

//! Q5_1·Q8 dot, scalar reference — THE q51 float-order contract: per
//! 32-block isum = Σ q·a (q = nibble | qh_bit << 4), asum = Σ a
//! inline (Q8_0 rows carry no sums); f += (d·isum + m·asum)·xs[b].
def dot_q51q8_scalar(wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?;
                     n : int64) : float => dot_b5q8_scalar(wq, ws, xq, xs, n, KqTag_q51())

//! Q5_0·Q8 dot, scalar reference — THE q5032 float-order contract:
//! q51's walk over the level q - 16; f += d·isum·xs[b], no asum.
def dot_q5032q8_scalar(wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?;
                       n : int64) : float => dot_b5q8_scalar(wq, ws, xq, xs, n, KqTag_q5032())

//! the qh word at `hp`, little-endian
def b5_qh(hp : uint8 const?) : uint {
    unsafe {
        return uint(hp[0]) | (uint(hp[1]) << 8u) | (uint(hp[2]) << 16u) | (uint(hp[3]) << 24u)
    }
}

//! a 5-bit per-32 plane's scalar dot under its level and fold
[hint(unsafe_range_check, noalias = wq, noalias = ws, noalias = xq, noalias = xs)]
def private dot_b5q8_scalar(wq : uint8 const?; ws : uint8 const?; xq : int8 const?;
                            xs : float const?; n : int64; f : auto(TT)) : float {
    let nb = n / KQ_B32_ELEMS
    var acc = 0.0
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * b32_qb(f)
            let xb = bi * KQ_B32_ELEMS
            let qh = b5_qh(wq + wb + B32_NIB_BYTES)
            var isum = 0
            var asum = 0
            for (j in range64(B32_NIB_BYTES)) {
                let b = int(wq[wb + j])
                let h0 = (int(qh >> uint(j)) & 1) << 4
                let h1 = (int(qh >> uint(j + B32_NIB_BYTES)) & 1) << 4
                let a0 = int(xq[xb + j])
                let a1 = int(xq[xb + j + B32_NIB_BYTES])
                isum += b5_level((b & 15) | h0, f) * a0 + b5_level((b >> 4) | h1, f) * a1
                asum += a0 + a1
            }
            acc += b5_fold(ws + bi * b32_sb(f), isum, asum, f) * xs[bi]
        }
    }
    return acc
}

//! Expand one iq4nl32 row through the codebook to int8 bytes in `qe`
//! (n bytes) — the row the fast kernels MAC over, exact.
def expand_iq4nl32_row(wq : uint8 const?; var qe : int8?; nb : int64) {
    expand_nl32_row(wq, qe, nb, iq4nl_lut())
}

//! the q4032 twin: the row through the linear table
def expand_q4032_row(wq : uint8 const?; var qe : int8?; nb : int64) {
    expand_nl32_row(wq, qe, nb, q40_lut())
}

//! one per-32 codebook plane row through the table `lut` to int8
[hint(unsafe_range_check, noalias = wq, noalias = qe)]
def private expand_nl32_row(wq : uint8 const?; var qe : int8?; nb : int64; lut : int8[16]) {
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * IQ4NL32_QB
            let xb = bi * KQ_B32_ELEMS
            for (j in range64(B32_NIB_BYTES)) {
                let b = int(wq[wb + j])
                qe[xb + j] = lut[b & 15]
                qe[xb + j + B32_NIB_BYTES] = lut[b >> 4]
            }
        }
    }
}

//! Expand one q51 row's 5-bit quants to bytes in `qe`: u64 nibble
//! masks + the qh bits spread in at 0x10. Exact; the MAC runs over it
//! (over just-stored STACK bytes it won't vectorize: 1.95/8.1 GB/s).
def expand_q51_row(wq : uint8 const?; var qe : int8?; nb : int64) {
    expand_b5_row(wq, qe, nb, KqTag_q51())
}

//! the q5032 twin: the row's signed q - 16 levels
def expand_q5032_row(wq : uint8 const?; var qe : int8?; nb : int64) {
    expand_b5_row(wq, qe, nb, KqTag_q5032())
}

//! one qh byte's eight bits spread to bit 4 of eight bytes
def private b5_spread(h : uint8) : uint64 {
    let m = (0x0101010101010101ul * uint64(h)) & 0x8040201008040201ul
    return ((m + 0x7f7f7f7f7f7f7f7ful) >> 3ul) & 0x1010101010101010ul
}

//! one 5-bit per-32 plane row to int8 bytes under the format's level
[hint(unsafe_range_check, noalias = wq, noalias = qe)]
def private expand_b5_row(wq : uint8 const?; var qe : int8?; nb : int64; f : auto(TT)) {
    unsafe {
        var qp64 = reinterpret<uint64?>(qe)
        for (bi in range64(nb)) {
            let wb = bi * b32_qb(f)
            let hb = wb + B32_NIB_BYTES
            let wp64 = reinterpret<uint64 const?>(wq + wb)
            let w0 = wp64[0]
            let w1 = wp64[1]
            let lo = 0x0f0f0f0f0f0f0f0ful
            qp64[bi * 4l] = b5_bytes(w0 & lo, b5_spread(wq[hb]), f)
            qp64[bi * 4l + 1l] = b5_bytes(w1 & lo, b5_spread(wq[hb + 1l]), f)
            qp64[bi * 4l + 2l] = b5_bytes((w0 >> 4ul) & lo, b5_spread(wq[hb + 2l]), f)
            qp64[bi * 4l + 3l] = b5_bytes((w1 >> 4ul) & lo, b5_spread(wq[hb + 3l]), f)
        }
    }
}

[hint(unsafe_range_check, noalias = qe, noalias = ws, noalias = xq, noalias = xs)]
def template dot_iq4nl32e_template(qe : int8 const ?; ws : uint8 const ?; xq : int8 const ?;
                                   xs : float const ?; n : int64) : float {
    let nb = n / 32l
    var f = 0.0
    for (bi in range64(nb)) {
        let xb = bi * 32l
        var isum = 0
        for [tune = 1] (k in range64(32l)) {
            isum += int(unsafe(qe[xb + k])) * int(unsafe(xq[xb + k]))
        }
        let d = f16_to_f32(uint(unsafe(ws[bi * 2l])) | (uint(unsafe(ws[bi * 2l + 1l])) << 8u))
        f += d * float(isum) * unsafe(xs[bi])
    }
    return f
}

//! The iq4nl32 MAC over an expanded row — the fast kernels' core,
//! bit-identical to dot_iq4nl32q8_scalar (same per-block fold order).
[hint(unsafe_range_check, noalias = qe, noalias = ws, noalias = xq, noalias = xs),
 tuned(fallback = "vec16")]
def dot_iq4nl32e(qe : int8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?;
                 n : int64) : float {}

[hint(unsafe_range_check, noalias = qe, noalias = ws, noalias = xq, noalias = xs)]
def template dot_q51e_template(qe : int8 const ?; ws : uint8 const ?; xq : int8 const ?;
                               xs : float const ?; n : int64) : float {
    let nb = n / 32l
    var f = 0.0
    for (bi in range64(nb)) {
        let xb = bi * 32l
        var isum = 0
        var asum = 0
        for [tune = 1] (k in range64(32l)) {
            let a = int(unsafe(xq[xb + k]))
            isum += int(unsafe(qe[xb + k])) * a
            asum += a
        }
        let d = f16_to_f32(uint(unsafe(ws[bi * 4l])) | (uint(unsafe(ws[bi * 4l + 1l])) << 8u))
        let m = f16_to_f32(uint(unsafe(ws[bi * 4l + 2l])) | (uint(unsafe(ws[bi * 4l + 3l])) << 8u))
        f += (d * float(isum) + m * float(asum)) * unsafe(xs[bi])
    }
    return f
}

//! The q51 MAC over an expanded row — the fast kernels' core,
//! bit-identical to dot_q51q8_scalar; vectorizes like dot_q8q8.
[hint(unsafe_range_check, noalias = qe, noalias = ws, noalias = xq, noalias = xs),
 tuned(fallback = "vec16")]
def dot_q51e(qe : int8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?;
             n : int64) : float {}

//! expand_iq4nl32_row's grp<mr> twin: row r of an mr-row group (the
//! nibble dwords at j·4·mr + r·4, mx4's column order). Bit-identical.
def expand_iq4nl32_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64) {
    expand_nl32_grp_row(gq, r, mr, qe, nb, iq4nl_lut())
}

//! expand_iq4nl32_grp_row's q4032 twin, under the linear q - 8 code
def expand_q4032_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64) {
    expand_nl32_grp_row(gq, r, mr, qe, nb, q40_lut())
}

//! one grp<mr> row of a per-32 codebook plane through the table `lut`
def private expand_nl32_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64;
                                lut : int8[16]) {
    unsafe {
        for (bi in range64(nb)) {
            let base = bi * IQ4NL32_QB * mr
            let xb = bi * KQ_B32_ELEMS
            for (j in range64(B32_NIB_BYTES)) {
                let b = int(gq[base + (j / 4l) * 4l * mr + r * 4l + j % 4l])
                qe[xb + j] = lut[b & 15]
                qe[xb + j + B32_NIB_BYTES] = lut[b >> 4]
            }
        }
    }
}

//! expand_q51_row's grp<mr> twin: row r of an mr-row group (nibble
//! dwords at j·4·mr + r·4, qh at the 16·mr tail). Bit-identical.
def expand_q51_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64) {
    expand_b5_grp_row(gq, r, mr, qe, nb, KqTag_q51())
}

//! expand_q5032_row's grp<mr> twin over q51's interleave
def expand_q5032_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64) {
    expand_b5_grp_row(gq, r, mr, qe, nb, KqTag_q5032())
}

//! one grp<mr> row of a 5-bit per-32 plane under the format's level
def private expand_b5_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64;
                              f : auto(TT)) {
    unsafe {
        for (bi in range64(nb)) {
            let base = bi * b32_qb(f) * mr
            let qh = b5_qh(gq + base + B32_NIB_BYTES * mr + r * B5_QH_BYTES)
            let xb = bi * KQ_B32_ELEMS
            for (j in range64(B32_NIB_BYTES)) {
                let b = int(gq[base + (j / 4l) * 4l * mr + r * 4l + j % 4l])
                let h0 = (int(qh >> uint(j)) & 1) << 4
                let h1 = (int(qh >> uint(j + B32_NIB_BYTES)) & 1) << 4
                qe[xb + j] = int8(b5_level((b & 15) | h0, f))
                qe[xb + j + B32_NIB_BYTES] = int8(b5_level((b >> 4) | h1, f))
            }
        }
    }
}

//! One row's f16 d bytes gathered contiguous off the grp<mr> scale
//! plane (dot_iq4nl32e reads a packed 2B-per-block row).
def gather_iq4nl32_grp_scales(gs : uint8 const?; r, mr : int64; var ss : uint8?; nb : int64) {
    unsafe {
        for (bi in range64(nb)) {
            let src = bi * IQ4NL32_SB * mr + r * IQ4NL32_SB
            ss[bi * IQ4NL32_SB] = gs[src]
            ss[bi * IQ4NL32_SB + 1l] = gs[src + 1l]
        }
    }
}

//! One row's scale bytes gathered contiguous off the grp<mr> scale
//! plane (dot_q51e reads a packed 4B-per-block row).
def gather_q51_grp_scales(gs : uint8 const?; r, mr : int64; var ss : uint8?; nb : int64) {
    unsafe {
        for (bi in range64(nb)) {
            let src = bi * 4l * mr + r * 4l
            ss[bi * 4l] = gs[src]
            ss[bi * 4l + 1l] = gs[src + 1l]
            ss[bi * 4l + 2l] = gs[src + 2l]
            ss[bi * 4l + 3l] = gs[src + 3l]
        }
    }
}

// A per-32 format expands a row to int8 once and MACs over it; the
// tag picks strides, expand and fold - the three shapes are one body.

def b32_qb(_f : KqTag_q51) : int64 => Q51_QB
def b32_qb(_f : KqTag_iq4nl32) : int64 => IQ4NL32_QB
def b32_qb(_f : KqTag_q4032) : int64 => Q4032_QB
def b32_qb(_f : KqTag_q5032) : int64 => Q5032_QB
def b32_sb(_f : KqTag_q51) : int64 => Q51_SB
def b32_sb(_f : KqTag_iq4nl32) : int64 => IQ4NL32_SB
def b32_sb(_f : KqTag_q4032) : int64 => Q4032_SB
def b32_sb(_f : KqTag_q5032) : int64 => Q5032_SB
def private b32_layout_mr(_f : KqTag_q51) : int64 => active_b32_layout_mr(KqFmt.q51)
def private b32_layout_mr(_f : KqTag_iq4nl32) : int64 => active_b32_layout_mr(KqFmt.iq4nl32)
def private b32_layout_mr(_f : KqTag_q4032) : int64 => active_b32_layout_mr(KqFmt.q4032)
def private b32_layout_mr(_f : KqTag_q5032) : int64 => active_b32_layout_mr(KqFmt.q5032)

def private b32_expand_row(wq : uint8 const?; var qe : int8?; nb : int64; _f : KqTag_q51) {
    expand_q51_row(wq, qe, nb)
}
def private b32_expand_row(wq : uint8 const?; var qe : int8?; nb : int64; _f : KqTag_iq4nl32) {
    expand_iq4nl32_row(wq, qe, nb)
}
def private b32_expand_row(wq : uint8 const?; var qe : int8?; nb : int64; _f : KqTag_q4032) {
    expand_q4032_row(wq, qe, nb)
}
def private b32_expand_row(wq : uint8 const?; var qe : int8?; nb : int64; _f : KqTag_q5032) {
    expand_q5032_row(wq, qe, nb)
}
def private b32_expand_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64;
                               _f : KqTag_q51) {
    expand_q51_grp_row(gq, r, mr, qe, nb)
}
def private b32_expand_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64;
                               _f : KqTag_iq4nl32) {
    expand_iq4nl32_grp_row(gq, r, mr, qe, nb)
}
def private b32_expand_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64;
                               _f : KqTag_q4032) {
    expand_q4032_grp_row(gq, r, mr, qe, nb)
}
def private b32_expand_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64;
                               _f : KqTag_q5032) {
    expand_q5032_grp_row(gq, r, mr, qe, nb)
}
def private b32_gather_grp_scales(gs : uint8 const?; r, mr : int64; var ss : uint8?; nb : int64;
                                  _f : KqTag_q51) {
    gather_q51_grp_scales(gs, r, mr, ss, nb)
}
//! the d-half formats gather iq4nl32's 2-byte d interleave
def private b32_gather_grp_scales(gs : uint8 const?; r, mr : int64; var ss : uint8?; nb : int64;
                                  _f : KqTag_iq4nl32 | KqTag_q4032 | KqTag_q5032) {
    gather_iq4nl32_grp_scales(gs, r, mr, ss, nb)
}
def private b32_dot_e(qe : int8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?;
                      n : int64; _f : KqTag_q51) : float => dot_q51e(qe, ws, xq, xs, n)
//! the d-half formats' MAC (iq4nl32, q4032, q5032) is iq4nl32's:
//! the expanded row's int8 levels under the block's f16 d
def private b32_dot_e(qe : int8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?;
                      n : int64; _f : auto(TT)) : float => dot_iq4nl32e(qe, ws, xq, xs, n)

//! One (region, row) expanded into the worker's scratch: the disk
//! row, or the grp<mr> group's row with its scales gathered
def private b32_expand_at(wq : uint8 const?; ws : uint8 const?; blk0, row, nb, mr, grpRows : int64;
                          var qep : int8?; var scp : uint8?; var srow : uint8 const?&;
                          f : auto(TT)) {
    unsafe {
        srow = ws + (blk0 + row * nb) * b32_sb(f)
        if (mr == 1l || row >= grpRows) {
            b32_expand_row(wq + (blk0 + row * nb) * b32_qb(f), qep, nb, f)
        } else {
            let gbase = blk0 + (row / mr) * mr * nb
            b32_expand_grp_row(wq + gbase * b32_qb(f), row % mr, mr, qep, nb, f)
            b32_gather_grp_scales(ws + gbase * b32_sb(f), row % mr, mr, scp, nb, f)
            srow = scp
        }
    }
}

// Portable per-32 batch (the batch slot default): rows-outer — each
// row expands ONCE into the worker's scratch, then ntok MAC passes.
def private b32_batch_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                             xqp : int8 const?; xsp : float const?; n, d, ntok : int64;
                             f : auto(TT)) {
    let nb = n / KQ_B32_ELEMS
    let mr = b32_layout_mr(f)   // grp<mr> planes on the gen backend; tail rows stay disk-order
    let grpRows = (d / mr) * mr
    let sb = b32_sb(f)
    var myp = yp
    unsafe {
        let chunks = matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)
        maybe_parallel_for(0, int(d), chunks) $(rb, re) {
            var qe : array<int8>
            qe |> resize(n)
            var sc : array<uint8>
            sc |> resize(nb * sb)
            unsafe {
                var qep = addr(qe[0])
                var scp = addr(sc[0])
                for (i in range(rb, re)) {
                    let row = int64(i)
                    var srow : uint8 const?
                    b32_expand_at(wq, ws, 0l, row, nb, mr, grpRows, qep, scp, srow, f)
                    for (tk in range64(ntok)) {
                        myp[tk * d + row] = b32_dot_e(qep, srow, xqp + tk * n, xsp + tk * nb, n, f)
                    }
                }
            }
            delete qe
            delete sc
        }
    }
}

// Portable per-32 groupN (the groupn default, MoE decode): (woff,
// xoff) element pairs, expand-then-MAC a (region, row); no bias form.
def private b32_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                              offs : int64 const?; nregions : int64; xqp : int8 const?;
                              xsp : float const?; n, d : int64; f : auto(TT)) {
    let nb = n / KQ_B32_ELEMS
    let mr = b32_layout_mr(f)   // grp<mr> planes on the gen backend; tail rows stay disk-order
    let grpRows = (d / mr) * mr
    let sb = b32_sb(f)
    var myp = yp
    unsafe {
        let chunks = matmul_chunks_gemv(int(nregions * d), 1, n * d * nregions)
        maybe_parallel_for(0, int(nregions * d), chunks) $(rb, re) {
            var qe : array<int8>
            qe |> resize(n)
            var sc : array<uint8>
            sc |> resize(nb * sb)
            unsafe {
                var qep = addr(qe[0])
                var scp = addr(sc[0])
                for (i in range(rb, re)) {
                    let ii = int64(i)
                    let r = ii / d
                    let row = ii % d
                    let woff = offs[r * 2l]
                    let xoff = offs[r * 2l + 1l]
                    var srow : uint8 const?
                    let blk0 = woff / KQ_B32_ELEMS
                    b32_expand_at(wq, ws, blk0, row, nb, mr, grpRows, qep, scp, srow, f)
                    myp[ii] = b32_dot_e(qep, srow, xqp + xoff, xsp + xoff / KQ_B32_ELEMS, n, f)
                }
            }
            delete qe
            delete sc
        }
    }
}

// Portable per-32 batch groupN (MoE prefill): (woff, row0, cnt)
// triples; each (region, row) expands once, then cnt token MACs.
def private b32_batch_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                    offs : int64 const?; nregions : int64; xqp : int8 const?;
                                    xsp : float const?; n, d : int64; f : auto(TT)) {
    let nb = n / KQ_B32_ELEMS
    let mr = b32_layout_mr(f)   // grp<mr> planes on the gen backend; tail rows stay disk-order
    let grpRows = (d / mr) * mr
    let sb = b32_sb(f)
    var myp = yp
    unsafe {
        let nrows = int(nregions * d)
        let chunks = matmul_chunks(nrows, get_q8_batch_chunks_per_job(), n * d * nregions)
        maybe_parallel_for(0, nrows, chunks) $(rb, re) {
            var qe : array<int8>
            qe |> resize(n)
            var sc : array<uint8>
            sc |> resize(nb * sb)
            unsafe {
                var qep = addr(qe[0])
                var scp = addr(sc[0])
                for (i in range(rb, re)) {
                    let ii = int64(i)
                    let r = ii / d
                    let row = ii % d
                    let woff = offs[r * 3l]
                    let row0 = offs[r * 3l + 1l]
                    let cnt = offs[r * 3l + 2l]
                    var srow : uint8 const?
                    let blk0 = woff / KQ_B32_ELEMS
                    b32_expand_at(wq, ws, blk0, row, nb, mr, grpRows, qep, scp, srow, f)
                    for (tk in range64(cnt)) {
                        let t = row0 + tk
                        myp[t * d + row] = b32_dot_e(qep, srow, xqp + t * n, xsp + t * nb, n, f)
                    }
                }
            }
            delete qe
            delete sc
        }
    }
}

// the per-32 slots' portable kernels: the shapes stamped per tag
def private q51q8_batch_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                               xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    b32_batch_kernel(yp, wq, ws, xqp, xsp, n, d, ntok, KqTag_q51())
}
def private q51q8_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                offs : int64 const?; nregions : int64; xqp : int8 const?;
                                xsp : float const?; n, d : int64) {
    b32_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, KqTag_q51())
}
def private q51q8_batch_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                      offs : int64 const?; nregions : int64; xqp : int8 const?;
                                      xsp : float const?; n, d : int64) {
    b32_batch_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, KqTag_q51())
}
def private iq4nl32q8_batch_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                   xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    b32_batch_kernel(yp, wq, ws, xqp, xsp, n, d, ntok, KqTag_iq4nl32())
}
def private iq4nl32q8_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                    offs : int64 const?; nregions : int64; xqp : int8 const?;
                                    xsp : float const?; n, d : int64) {
    b32_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, KqTag_iq4nl32())
}
def private iq4nl32q8_batch_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                          offs : int64 const?; nregions : int64; xqp : int8 const?;
                                          xsp : float const?; n, d : int64) {
    b32_batch_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, KqTag_iq4nl32())
}
def private q4032q8_batch_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                 xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    b32_batch_kernel(yp, wq, ws, xqp, xsp, n, d, ntok, KqTag_q4032())
}
def private q4032q8_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                  offs : int64 const?; nregions : int64; xqp : int8 const?;
                                  xsp : float const?; n, d : int64) {
    b32_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, KqTag_q4032())
}
def private q4032q8_batch_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                        offs : int64 const?; nregions : int64; xqp : int8 const?;
                                        xsp : float const?; n, d : int64) {
    b32_batch_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, KqTag_q4032())
}
def private q5032q8_batch_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                 xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    b32_batch_kernel(yp, wq, ws, xqp, xsp, n, d, ntok, KqTag_q5032())
}
def private q5032q8_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                  offs : int64 const?; nregions : int64; xqp : int8 const?;
                                  xsp : float const?; n, d : int64) {
    b32_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, KqTag_q5032())
}
def private q5032q8_batch_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?;
                                        offs : int64 const?; nregions : int64; xqp : int8 const?;
                                        xsp : float const?; n, d : int64) {
    b32_batch_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, KqTag_q5032())
}

//! Register the portable Q8·Q8 backend (row-major, no repack). Lowest priority — a platform ISA backend
//! out-ranks it in the auto-select where available; it stays the fallback everywhere else.
//! The per-32 portable kernels register first: unset slots take them.
[init]
def dasllama_math_default_register() {
    register_b32_portable(KqFmt.q51, B32Slots(batch = @@q51q8_batch_kernel,
        groupn = @@q51q8_groupn_kernel, batch_groupn = @@q51q8_batch_groupn_kernel))
    register_b32_portable(KqFmt.iq4nl32, B32Slots(batch = @@iq4nl32q8_batch_kernel,
        groupn = @@iq4nl32q8_groupn_kernel, batch_groupn = @@iq4nl32q8_batch_groupn_kernel))
    register_b32_portable(KqFmt.q4032, B32Slots(batch = @@q4032q8_batch_kernel,
        groupn = @@q4032q8_groupn_kernel, batch_groupn = @@q4032q8_batch_groupn_kernel))
    register_b32_portable(KqFmt.q5032, B32Slots(batch = @@q5032q8_batch_kernel,
        groupn = @@q5032q8_groupn_kernel, batch_groupn = @@q5032q8_batch_groupn_kernel))
    register_kernel_backend(KernelBackend(name = "portable", mm = @@q8q8_kernel, batch = @@q8q8_batch_kernel,
        group3 = @@q8q8_group3_kernel, repack = @@repack_q8q8_identity, mm_mx4 = @@mx4q8_kernel,
        mx4_batch = @@mx4q8_batch_kernel, mx4_batch_groupn = @@mx4q8_batch_groupn_kernel,
        groupn = @@q8q8_groupn_kernel, groupn_mx4 = @@mx4q8_groupn_kernel,
        mm_rows = @@q8q8_rows_kernel,
        mm_s16 = @@q8q8_kernel_s16, batch_s16 = @@q8q8_batch_kernel_s16,
        group3_s16 = @@q8q8_group3_kernel_s16, groupn_s16 = @@q8q8_groupn_kernel_s16,
        mm_rows_s16 = @@q8q8_rows_kernel_s16,
        needs_repack = false, priority = 0))
}
