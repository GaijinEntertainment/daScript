options gen2

module dasllama_math shared public

require math
require daslib/jobque_boost public  // matmul's threaded path expands parallel_for; consumers need its symbols
require dasllama/dasllama_par  // maybe_parallel_for: thread-or-inline a kernel body on a runtime size flag

// dasLLAMA numeric primitives — naive fp32 baseline (curriculum step 1).
// Correctness first; optimization (blocking, float4, threading) comes later.
//
// Every array param is `array<float> | #` (owning array OR borrowed view), so weights
// can be passed zero-copy as views into one big model blob and outputs can be written
// straight into a view (e.g. the KV cache). Dims/indices are int64 for large models.

//! Dot product of two n-length float buffers, auto-vectorized by the LLVM JIT
//! (width-8, no bounds checks, a/b asserted non-aliasing). Beats hand-rolled float4 ~1.8x.
[hint(unsafe_range_check, noalias = a, noalias = b)]
def dot(a, b : float const?; n : int64) : float {
    var acc = 0.0
    for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
        acc += unsafe(a[j]) * unsafe(b[j])
    }
    return acc
}

//! AXPY: d[j] += scale * s[j] over n elements, same auto-vectorization.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def axpy(var d : float?; s : float const?; scale : float; n : int64) {
    for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
        unsafe {
            d[j] += scale * s[j]
        }
    }
}

//! Fork-callable AXPY for the threaded prefill attention. Same width-8 kernel as axpy, but takes
//! `d` as `const?` so a capture-const fork pointer binds (add-const is allowed) — un-consts inside
//! to write. NO `noalias` hint on purpose: `const?`+reinterpret under noalias miscompiles. Output
//! and the value-cache source never overlap, so the vectorizer's runtime alias check always passes.
[hint(unsafe_range_check)]
def vaxpy(d : float const?; s : float const?; scale : float; n : int64) {
    unsafe {
        var dm = reinterpret<float?>(d)
        for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
            dm[j] += scale * s[j]
        }
    }
}

//! d[j] += s[j], width-8 vectorized (the residual-stream update). The plain `for k; a[k]+=b[k]`
//! over bounds-checked arrays does NOT vectorize under the JIT — this pointer form is ~5x faster.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def add_inplace(var d : float?; s : float const?; n : int64) {
    for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
        unsafe {
            d[j] += s[j]
        }
    }
}

//! d[j] *= s[j], width-8 vectorized — the SwiGLU/GeGLU gate multiply (act(W1·x) * (W3·x)).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def mul_inplace(var d : float?; s : float const?; n : int64) {
    for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
        unsafe {
            d[j] *= s[j]
        }
    }
}

// n*d above this -> split the output rows across worker threads. Below it, the per-call
// job-dispatch overhead (~90us) dwarfs the gain, so stay single-thread. (Measured on M1 Max:
// classifier 32000x288 = 9.2M threads to ~2x; ffn 768x288 = 0.22M would be ~5x slower.)
let MATMUL_PAR_THRESHOLD = 2_000_000l

//! y = W·x — matrix-vector product, the dominant LLM compute kernel.
//! W is row-major [d rows x n cols], x is [n], y is [d]; each y[i] is one dot
//! product of W's i-th row with x. Shapes match llama2.c (n = input dim, d = output dim).
//! Large matmuls thread the output rows across workers (requires an enclosing with_job_que);
//! small ones stay single-thread. Workers read y/w/x via raw pointers (unsafe cross-context).
def matmul(var y : array<float>; w : array<float> | #; x : array<float>; n, d : int64) {
    unsafe {
        var yp = addr(y[0])                              // var: stays a writable pointer when captured
        let wp = reinterpret<float const?>(addr(w[0]))   // strip the `#` if w is a view
        let xp = addr(x[0])
        maybe_parallel_for(n * d >= MATMUL_PAR_THRESHOLD, 0, int(d), get_total_hw_jobs()) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot(wp + int64(i) * n, xp, n)
                }
            }
        }
    }
}

//! Batched fp32 GEMM for prefill: Y [ntok x d] = X [ntok x n] · Wᵀ, W row-major [d x n].
//! X is token-major (token t at X[t*n..]); Y is token-major (token t at Y[t*d..]). Weight-
//! stationary nest (rows outer) — each weight row streams from DRAM once and is reused across
//! all ntok tokens, vs ntok× matmul re-reading W every call. Bit-for-bit ntok× matmul.
def matmul_batch(var y : array<float>; w : array<float> | #; x : array<float>; n, d, ntok : int64) {
    unsafe {
        var yp = addr(y[0])
        let wp = reinterpret<float const?>(addr(w[0]))
        let xp = reinterpret<float const?>(addr(x[0]))
        maybe_parallel_for(n * d * ntok >= MATMUL_PAR_THRESHOLD, 0, int(d), get_total_hw_jobs()) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let wrow = wp + int64(i) * n
                    for (tk in range64(ntok)) {
                        yp[tk * d + int64(i)] = dot(wrow, xp + tk * n, n)
                    }
                }
            }
        }
    }
}

//! Q8_0 dequant-in-dot: weights are int8 quants `wq` + one fp32 scale `ws` per 32-block,
//! activations `x` stay fp32. A float4 accumulator is carried across the blocks (scale
//! applied per block as a broadcast fma), with a single horizontal reduce at the end —
//! this layout won the kernel bench (see matmul_variants). n must be a multiple of 32.
def dot_q8(wq : int8 const?; ws : float const?; x : float const?; n : int64) : float {
    let nb = n / 32l
    var acc = float4(0.0, 0.0, 0.0, 0.0)
    for (bi in range64(nb)) {
        let base = bi * 32l
        let s = unsafe(ws[bi])
        var bacc = float4(0.0, 0.0, 0.0, 0.0)
        var c = 0l
        while (c < 32l) {
            let off = base + c
            unsafe {
                let q4 = float4(float(wq[off]), float(wq[off + 1l]), float(wq[off + 2l]), float(wq[off + 3l]))
                let x4 = *reinterpret<float4?>(addr(x[off]))
                bacc += q4 * x4
            }
            c += 4l
        }
        acc += float4(s, s, s, s) * bacc
    }
    return acc.x + acc.y + acc.z + acc.w
}

//! Q8_0 matmul: y = W·x with W stored as int8 quants `wq` [d x n] + per-block scales
//! `ws` [d x n/32]. Same auto-dispatch as the fp32 matmul; large matmuls thread the output
//! rows (2x the worker count — Q8 is compute-bound and threads better than fp32). n % 32 == 0.
def matmul_q8(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; x : array<float>; n, d : int64) {
    let nb = n / 32l
    unsafe {
        var yp = addr(y[0])
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        // 2N (2 chunks per hw job): Q8 dequant-in-dot is compute-bound and threads better than fp32
        maybe_parallel_for(n * d >= MATMUL_PAR_THRESHOLD, 0, int(d), 2 * get_total_hw_jobs()) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q8(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

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

// ===== Q8·Q8 matmul: platform backend dispatched via function pointers =====
// The three public matmuls extract raw pointers ONCE in the main context and call through a function
// pointer. dasllama_math owns the pointers + the portable kernels (the default, set at the var-init
// below); dasllama_math_aarch64_neon overrides them at [init] on arm64 with SDOT kernels (via
// use_q8q8_kernels). Dispatch is at the MATMUL level — a platform owns its whole matmul, threading
// and tiling included — so the wrapper itself is backend-agnostic and the pointer is read in the main
// context, never inside a worker. Kernels take raw pointers (a function pointer can't carry the
// `array<T> | #` owning-or-view union), so the array->pointer extraction lives in the wrappers.
//
// TODO (deferred — NOT before the prefill profiler results + more refactoring): move the portable
// kernels below out into their own dasllama_math_default module, symmetric with
// dasllama_math_aarch64_neon, so dasllama_math holds only the typedefs + dispatch pointers + wrappers
// + setter. Holding off until we know where prefill time actually goes and the surrounding code settles.
typedef MatmulQ8Q8Fn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
typedef MatmulQ8Q8BatchFn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64; ntok : int64) : void>
typedef MatmulQ8Q8Group3Fn = function<(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0 : int64; woff1 : int64; woff2 : int64; xqp : int8 const?; xsp : float const?; n : int64; d0 : int64; d1 : int64; d2 : int64) : void>
// In-place repack of one Q8 weight region [d rows x n cols] (raw pointers at the region start) into
// the active backend's preferred layout. Default = no-op (row-major). n % 32 == 0.
typedef RepackQ8Q8Fn = function<(var wp : int8?; var sp : float?; n : int64; d : int64) : void>

var g_mm_q8q8 = @@q8q8_kernel
var g_mm_q8q8_batch = @@q8q8_batch_kernel
var g_mm_q8q8_group3 = @@q8q8_group3_kernel
var g_repack_q8q8 = @@repack_q8q8_identity

// Registered alternate backend (the arm64 laneq + interleaved-repack pair). Registered at [init] but
// NOT active by default — direct matmul callers (tests, benches) keep the row-major sdot4x4/portable
// backend, since the laneq kernels read an interleaved layout that only exists after the model's blob
// is repacked at load. enable_q8q8_laneq swaps it in; load_gguf calls that on the Q8 path, then repacks.
var g_q8q8_laneq_available = false
var g_q8q8_laneq_enabled = true   // user/bench toggle: set false before load_gguf to force the sdot4x4 baseline
var g_lq_mm = @@q8q8_kernel
var g_lq_batch = @@q8q8_batch_kernel
var g_lq_group3 = @@q8q8_group3_kernel
var g_lq_repack = @@repack_q8q8_identity

//! Default repack: no-op — weights stay row-major for the portable / sdot4x4 kernels.
[unused_argument(wp, sp, n, d)]
def repack_q8q8_identity(var wp : int8?; var sp : float?; n, d : int64) {}

//! Override the active Q8·Q8 matmul kernels + repack. Called by dasllama_math_aarch64_neon at [init] on
//! arm64 to set the default (sdot4x4 + no-op repack); the portable kernels are the default elsewhere.
def use_q8q8_kernels(mm : MatmulQ8Q8Fn; batch : MatmulQ8Q8BatchFn; group3 : MatmulQ8Q8Group3Fn; repack : RepackQ8Q8Fn) {
    g_mm_q8q8 = mm
    g_mm_q8q8_batch = batch
    g_mm_q8q8_group3 = group3
    g_repack_q8q8 = repack
}

//! Restore the portable kernels (+ no-op repack) as the active backend. For benchmarks that A/B the
//! default against a platform backend on the same binary.
def use_q8q8_default_kernels() {
    use_q8q8_kernels(@@q8q8_kernel, @@q8q8_batch_kernel, @@q8q8_group3_kernel, @@repack_q8q8_identity)
}

//! Register the alternate laneq backend (arm64 SDOT lane-indexed kernels + interleaved repack). Called
//! by dasllama_math_aarch64_neon at [init] on arm64. Does NOT activate it — enable_q8q8_laneq does, at
//! model load, because the laneq kernels require the interleaved weight layout the repack produces.
def register_q8q8_laneq(mm : MatmulQ8Q8Fn; batch : MatmulQ8Q8BatchFn; group3 : MatmulQ8Q8Group3Fn; repack : RepackQ8Q8Fn) {
    g_lq_mm = mm
    g_lq_batch = batch
    g_lq_group3 = group3
    g_lq_repack = repack
    g_q8q8_laneq_available = true
}

//! Activate the registered laneq backend if one exists (arm64). Returns true when activated — the
//! caller (load_gguf, Q8 path) MUST then repack every weight tensor via repack_q8q8_weight before any
//! matmul runs. Returns false where there is no laneq backend (portable stays active, weights row-major).
def enable_q8q8_laneq() : bool {
    if (!g_q8q8_laneq_available || !g_q8q8_laneq_enabled) {
        return false
    }
    use_q8q8_kernels(g_lq_mm, g_lq_batch, g_lq_group3, g_lq_repack)
    return true
}

//! Toggle the laneq backend on/off (default on). Set false BEFORE load_gguf to load a Q8 model on the
//! row-major sdot4x4 backend (the pre-laneq baseline) — for A/B benchmarking on one binary.
def set_q8q8_laneq_enabled(on : bool) {
    g_q8q8_laneq_enabled = on
}

//! Repack one Q8 weight tensor (region at element offset woff in qblob, shape [d rows x n cols]) into
//! the active backend's layout, IN PLACE. No-op unless the laneq backend is enabled. n % 32 == 0.
def repack_q8q8_weight(var qblob : array<int8>; var qscales : array<float>; woff, n, d : int64) {
    unsafe {
        invoke(g_repack_q8q8, addr(qblob[woff]), addr(qscales[woff / 32l]), n, d)
    }
}

// ----- parallel_for dispatch profiler (off by default) -----
// When on, the batch kernel records each worker's start time (relative to the parallel_for start) into
// g_pf_ws[rb], then aggregates the first/last worker start and the wall time per call. A large gap
// between pf-start and last-worker-start = workers trickling in (the job pool waking from sleep), which
// is the suspected prefill stall. The recorder lives here (not the neon module) so the A/B harness can
// force the portable kernels and measure the same parallel_for behavior in one module.
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

// Portable kernels (auto-vectorized dot_q8q8) — the default backend. 1N threading (one chunk per hw
// job) measured fastest for this compute-light kernel (~52 vs ~51 tok/s at 2N end-to-end).
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

//! Q8·Q8 matmul: y = W·x with W as int8 quants `wq` [d x n] + scales `ws` [d x n/32] and the
//! activation PRE-quantized to (xq [n], xs [n/32]) by the caller (it's shared across all d rows, so
//! quantizing once amortizes). Dispatches to the active backend (g_mm_q8q8). n % 32 == 0.
def matmul_q8q8(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8, addr(y[0]),
            reinterpret<int8 const?>(addr(wq[0])), reinterpret<float const?>(addr(ws[0])),
            reinterpret<int8 const?>(addr(xq[0])), reinterpret<float const?>(addr(xs[0])), n, d)
    }
}

//! Batched Q8·Q8 GEMM for prefill: same weight-stationary nest as matmul_batch over the SDOT kernel.
//! Activations PRE-quantized per token into (xq [ntok x n], xs [ntok x n/32]). Y token-major
//! [ntok x d]. Bit-for-bit ntok× matmul_q8q8. Dispatches to the active backend. n % 32 == 0.
def matmul_q8q8_batch(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q8q8_batch, addr(y[0]),
            reinterpret<int8 const?>(addr(wq[0])), reinterpret<float const?>(addr(ws[0])),
            reinterpret<int8 const?>(addr(xq[0])), reinterpret<float const?>(addr(xs[0])), n, d, ntok)
    }
}

//! Fused Q8·Q8 for three independent row-groups that share the activation (attention Q/K/V): one
//! parallel_for over the combined d0+d1+d2 output rows, so idle workers fill in on the small K/V rows
//! that would otherwise run single-thread while the pool sits idle (~23% faster than the sequential
//! Q-parallel + K,V-single on the real shapes). wq/ws is the whole quantized blob; woff{0,1,2} are the
//! per-group weight-element offsets; the activation is pre-quantized once into (xq, xs). Dispatches to
//! the active backend.
def matmul_q8q8_group3(var y0 : array<float>; var y1 : array<float>; var y2 : array<float>;
                       wq : array<int8>; ws : array<float>; woff0, woff1, woff2 : int64;
                       xq : array<int8>; xs : array<float>; n, d0, d1, d2 : int64) {
    unsafe {
        invoke(g_mm_q8q8_group3, addr(y0[0]), addr(y1[0]), addr(y2[0]),
            reinterpret<int8 const?>(addr(wq[0])), reinterpret<float const?>(addr(ws[0])),
            woff0, woff1, woff2,
            reinterpret<int8 const?>(addr(xq[0])), reinterpret<float const?>(addr(xs[0])), n, d0, d1, d2)
    }
}

//! Q4_0 dequant-in-dot: weights are 4-bit quants `wq` in GGML split-half packing (per 32-block,
//! byte j holds element j low / j+16 high) + one fp32 scale `ws` per block; activations `x` fp32.
//! Per block: the inner for[vectorize] widens nibbles to float at width 4 (M1 native NEON lane —
//! the heavy Q4 unpack loses ~7% at width 8 from cross-lane shuffles), then one scaled reduce per
//! block. This won the kernel bench (see matmul_variants). n must be a multiple of 32.
[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def dot_q4(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        let bbyte = bi * 16l
        let s = unsafe(ws[bi])
        var bs = 0.0
        for [vectorize, vectorize_width = 4, unroll_count = 4] (j in range64(16l)) {
            let byte = int(unsafe(wq[bbyte + j]))
            bs += float((byte & 0x0F) - 8) * unsafe(x[base + j]) + float((byte >> 4) - 8) * unsafe(x[base + 16l + j])
        }
        acc += s * bs
    }
    return acc
}

//! Q4_0 matmul: y = W·x with W as 4-bit quants `wq` [d x n/2 bytes] + per-block scales `ws`
//! [d x n/32]. Large matmuls thread the output rows at 4 chunks per hw job (4N — the heavy nibble
//! unpack is compute-bound, so finer splitting balances better); small ones stay single. n % 32 == 0.
def matmul_q4(var y : array<float>; wq : array<uint8> | #; ws : array<float> | #; x : array<float>; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        var yp = addr(y[0])
        let wp = reinterpret<uint8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        // 4N (4 chunks per hw job): the heavy nibble unpack makes each row compute-bound, so finer
        // splitting balances better (esp. the classifier) — more than the compute-light Q8 path.
        maybe_parallel_for(n * d >= MATMUL_PAR_THRESHOLD, 0, int(d), 4 * get_total_hw_jobs()) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q4(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

//! RMSNorm — root-mean-square normalization (no mean subtraction, no bias).
//! o[i] = weight[i] * x[i] / sqrt(mean(x^2) + eps). eps defaults to 1e-5 (Llama-2/3);
//! Qwen2 / Gemma2 pass 1e-6 from the model's metadata.
def rmsnorm(var o : array<float> | #; x : array<float> | #; weight : array<float> | #; size : int64; eps : float = 1.0e-5) {
    var ss = 0.0
    for (j in range64(size)) {
        ss += x[j] * x[j]
    }
    ss /= float(size)
    ss += eps
    ss = 1.0 / sqrt(ss)
    for (j in range64(size)) {
        o[j] = weight[j] * (x[j] * ss)
    }
}

//! softmax — turns a score vector into a probability distribution (sums to 1).
//! In place over the first `size` elements. Subtracts the max first so exp() can't
//! overflow (exp(x-max)/sum(exp(x-max)) is identical math, numerically safe).
//! Pointer core — the threaded prefill attention runs in fork contexts and can only touch raw
//! addresses; the array overload forwards here, so both are bit-identical. Takes `const?` so a
//! capture-const fork pointer binds (add-const is allowed); un-consts internally to write in place.
def softmax(xc : float const?; size : int64) {
    unsafe {
        var x = reinterpret<float?>(xc)
        // max is exact + idempotent, so the vectorized reduce (any order, x[0] folded in twice) is bit-exact
        var maxv = x[0]
        for [vectorize, vectorize_width = 8, unroll_count = 2] (i in range64(size)) {
            maxv = max(maxv, x[i])
        }
        var sum = 0.0
        for [vectorize, vectorize_width = 8, unroll_count = 2] (i in range64(size)) {
            let e = exp(x[i] - maxv)
            x[i] = e
            sum += e
        }
        let inv_sum = 1.0 / sum   // scale by the reciprocal (matches ggml_vec_scale_f32), not a per-element /=
        for [vectorize, vectorize_width = 8, unroll_count = 2] (i in range64(size)) {
            x[i] *= inv_sum
        }
    }
}
def softmax(var x : array<float> | #; size : int64) {
    softmax(unsafe(addr(x[0])), size)
}

//! SiLU (a.k.a. swish) activation: silu(x) = x * sigmoid(x) = x / (1 + exp(-x)).
//! In place over the first `size` elements. The nonlinearity in the SwiGLU FFN gate;
//! smooth, ~identity for large x, dips slightly negative just below zero.
def silu(var x : array<float> | #; size : int64) {
    for (i in range64(size)) {
        let v = x[i]
        x[i] = v / (1.0 + exp(-v))
    }
}

//! GELU, tanh approximation (HF's gelu_pytorch_tanh): the GeGLU gate nonlinearity (Gemma2).
//! gelu(x) = 0.5x(1 + tanh(sqrt(2/pi)(x + 0.044715 x^3))). In place over the first `size`
//! elements. ggml's LLM_FFN_GELU approximates this same form via an fp16 lookup table; we
//! compute it directly (more accurate — near-ties against the LUT are expected, not bugs).
def gelu(var x : array<float> | #; size : int64) {
    let k = 0.7978845608028654  // sqrt(2/pi)
    for (i in range64(size)) {
        let v = x[i]
        x[i] = 0.5 * v * (1.0 + tanh(k * (v + 0.044715 * v * v * v)))
    }
}

//! Logit soft-capping (Gemma2): x = cap * tanh(x / cap), elementwise over the first `size`.
//! Smoothly bounds values to (-cap, cap). Applied to attention scores (cap 50) after the
//! 1/sqrt(head_size) scale and before softmax, and to the final classifier logits (cap 30).
//! Pointer core (see softmax) — threaded prefill attention calls this on raw fork addresses.
def softcap(xc : float const?; size : int64; cap : float) {
    let inv = 1.0 / cap
    unsafe {
        var x = reinterpret<float?>(xc)
        for (i in range64(size)) {
            x[i] = cap * tanh(x[i] * inv)
        }
    }
}
def softcap(var x : array<float> | #; size : int64; cap : float) {
    softcap(unsafe(addr(x[0])), size, cap)
}

//! RoPE — rotary position embedding. Rotates each adjacent coordinate pair (2j, 2j+1)
//! within a head by angle pos*freq, freq = 1/10000^(2j/head_size) (low pairs spin fast,
//! high pairs slow). This encodes position as rotation, so a later Q·K dot product
//! depends only on the relative offset between the two tokens. In place over the first
//! `n` elements (n = dim for Q, kv_dim for K); matches llama2.c's adjacent-pairs scheme.
def rope(var vec : array<float> | #; pos, head_size, n : int64) {
    for (p in range64(n / 2l)) {
        let i = p * 2l
        let head_dim = i % head_size
        let freq = 1.0 / pow(10000.0, float(head_dim) / float(head_size))
        let angle = float(pos) * freq
        let fcr = cos(angle)
        let fci = sin(angle)
        let v0 = vec[i]
        let v1 = vec[i + 1]
        vec[i] = v0 * fcr - v1 * fci
        vec[i + 1] = v0 * fci + v1 * fcr
    }
}

//! RoPE with an explicit base frequency θ and optional per-pair frequency factors
//! (the GGUF `rope_freqs` tensor — Llama-3.1's NTK-by-parts scaling). Empty factors =>
//! plain RoPE at base θ, so (θ=10000, empty) reproduces rope() above bit-for-bit.
//! freq_factors[j] divides the j-th pair's frequency, matching ggml (theta/ff).
def rope_scaled(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #) {
    let has_ff = !empty(freq_factors)
    for (p in range64(n / 2l)) {
        let i = p * 2l
        let head_dim = i % head_size
        var freq = 1.0 / pow(theta, float(head_dim) / float(head_size))
        if (has_ff) {
            freq /= freq_factors[head_dim / 2l]
        }
        let angle = float(pos) * freq
        let fcr = cos(angle)
        let fci = sin(angle)
        let v0 = vec[i]
        let v1 = vec[i + 1]
        vec[i] = v0 * fcr - v1 * fci
        vec[i + 1] = v0 * fci + v1 * fcr
    }
}

//! NEOX-style RoPE (Qwen2 / Phi3 / Gemma2): the rotated pair is offset by head_size/2 — vec[base+j]
//! with vec[base+j+half] — instead of the adjacent (2j, 2j+1) pair the Llama-family NORM rope uses.
//! Same per-pair frequencies; only the pairing differs.
def rope_scaled_neox(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #; mscale : float = 1.0) {
    let has_ff = !empty(freq_factors)
    let half = head_size / 2l
    for (h in range64(n / head_size)) {
        let base = h * head_size
        for (j in range64(half)) {
            var freq = 1.0 / pow(theta, float(2l * j) / float(head_size))
            if (has_ff) {
                freq /= freq_factors[j]
            }
            let angle = float(pos) * freq
            // mscale (LongRoPE attn_factor) scales cos/sin — i.e. the rotated magnitude (Phi3 = 1.19)
            let fcr = cos(angle) * mscale
            let fci = sin(angle) * mscale
            let v0 = vec[base + j]
            let v1 = vec[base + j + half]
            vec[base + j] = v0 * fcr - v1 * fci
            vec[base + j + half] = v0 * fci + v1 * fcr
        }
    }
}

//! Dispatch RoPE by the model's convention: NEOX (Qwen2/Phi3/Gemma2) or the default NORM (Llama).
//! mscale is the LongRoPE magnitude factor (1.0 for everything but Phi3); only the NEOX path uses it.
def rope_apply(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #; neox : bool; mscale : float = 1.0) {
    if (neox) {
        rope_scaled_neox(vec, pos, head_size, n, theta, freq_factors, mscale)
    } else {
        rope_scaled(vec, pos, head_size, n, theta, freq_factors)
    }
}
