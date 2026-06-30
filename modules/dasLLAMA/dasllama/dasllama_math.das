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
        acc = mad(unsafe(a[j]), unsafe(b[j]), acc)   // fused multiply-add (vectorizes to fmla)
    }
    return acc
}

//! AXPY: d[j] += scale * s[j] over n elements, auto-vectorized. Used single-threaded AND as the
//! V-accumulate in threaded prefill attention — `d` (output) and `s` (value-cache) never overlap, so
//! the noalias holds inside fork workers too (a hoisted/captured `var d : float?` writes straight
//! through: capture freezes the binding, not the pointee, and `d + off` stays a mutable `float?`).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def axpy(var d : float?; s : float const?; scale : float; n : int64) {
    for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
        unsafe {
            d[j] = mad(scale, s[j], d[j])
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

//! d[j] = s[j], width-8 vectorized contiguous copy (the KV-cache store). Same reason as add_inplace:
//! a bounds-checked array copy does NOT vectorize under the JIT; this pointer form does. d (cache) and
//! s (the k_b/v_b scratch) are always distinct arrays, so noalias is correct and free.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def copy_floats(var d : float?; s : float const?; n : int64) {
    for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
        unsafe {
            d[j] = s[j]
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

// ===== fp32 micro-GEMM: C[M x N] += A[M x K] * B[K x N] (B's N contiguous = SIMD dim) =====
// The broadcast-A FMA register-tile form (zero horizontal reduce) that makes flash attention's
// QKᵀ and P·V fast — mirrors llama.cpp ggml simd-gemm.h. Single-thread (callers thread above it,
// e.g. over heads); pointers so it is fork/thread-callable. Experiment ledger + A/B bench live in
// benchmarks/attn/. Tolerance-equal to a scalar triple loop (FMA contraction reorders low bits).

// Scalar C += A·B over rows [i0,i1) × cols [j0,j1) — the L-shaped remainder the 4×16 tile leaves.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def private gemm_f32_region(var cp : float?; ap, bp : float const?; i0, i1, j0, j1, K, N : int64) {
    unsafe {
        for (i in range64(i0, i1)) {
            let crow = i * N
            for (kk in range64(K)) {
                let a = ap[i * K + kk]
                let brow = kk * N
                for (j in range64(j0, j1)) {
                    cp[crow + j] += a * bp[brow + j]
                }
            }
        }
    }
}

// 4-row × 16-col tile: 16 float4 accumulators, broadcast-A FMA, no horizontal reduce. C += (cols
// [j0,j0+16) of rows [i0,i0+4)). N is the row stride of C/B; K the row stride / inner dim of A/B.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def private gemm_f32_uk_4x16(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {
    unsafe {
        let o0 = (i0 + 0l) * N + j0; let o1 = (i0 + 1l) * N + j0
        let o2 = (i0 + 2l) * N + j0; let o3 = (i0 + 3l) * N + j0
        var r0a = *reinterpret<float4?>(cp + o0);       var r0b = *reinterpret<float4?>(cp + o0 + 4l)
        var r0c = *reinterpret<float4?>(cp + o0 + 8l);  var r0d = *reinterpret<float4?>(cp + o0 + 12l)
        var r1a = *reinterpret<float4?>(cp + o1);       var r1b = *reinterpret<float4?>(cp + o1 + 4l)
        var r1c = *reinterpret<float4?>(cp + o1 + 8l);  var r1d = *reinterpret<float4?>(cp + o1 + 12l)
        var r2a = *reinterpret<float4?>(cp + o2);       var r2b = *reinterpret<float4?>(cp + o2 + 4l)
        var r2c = *reinterpret<float4?>(cp + o2 + 8l);  var r2d = *reinterpret<float4?>(cp + o2 + 12l)
        var r3a = *reinterpret<float4?>(cp + o3);       var r3b = *reinterpret<float4?>(cp + o3 + 4l)
        var r3c = *reinterpret<float4?>(cp + o3 + 8l);  var r3d = *reinterpret<float4?>(cp + o3 + 12l)
        for [unroll_count = 2] (kk in range64(K)) {     // ~3% over the rolled loop; same accumulation order
            let bo = kk * N + j0
            let b0 = *reinterpret<float4 const?>(bp + bo);      let b1 = *reinterpret<float4 const?>(bp + bo + 4l)
            let b2 = *reinterpret<float4 const?>(bp + bo + 8l); let b3 = *reinterpret<float4 const?>(bp + bo + 12l)
            let a0 = ap[(i0 + 0l) * K + kk]; let v0 = float4(a0, a0, a0, a0)
            r0a = mad(b0, v0, r0a); r0b = mad(b1, v0, r0b); r0c = mad(b2, v0, r0c); r0d = mad(b3, v0, r0d)
            let a1 = ap[(i0 + 1l) * K + kk]; let v1 = float4(a1, a1, a1, a1)
            r1a = mad(b0, v1, r1a); r1b = mad(b1, v1, r1b); r1c = mad(b2, v1, r1c); r1d = mad(b3, v1, r1d)
            let a2 = ap[(i0 + 2l) * K + kk]; let v2 = float4(a2, a2, a2, a2)
            r2a = mad(b0, v2, r2a); r2b = mad(b1, v2, r2b); r2c = mad(b2, v2, r2c); r2d = mad(b3, v2, r2d)
            let a3 = ap[(i0 + 3l) * K + kk]; let v3 = float4(a3, a3, a3, a3)
            r3a = mad(b0, v3, r3a); r3b = mad(b1, v3, r3b); r3c = mad(b2, v3, r3c); r3d = mad(b3, v3, r3d)
        }
        *reinterpret<float4?>(cp + o0) = r0a;       *reinterpret<float4?>(cp + o0 + 4l) = r0b
        *reinterpret<float4?>(cp + o0 + 8l) = r0c;  *reinterpret<float4?>(cp + o0 + 12l) = r0d
        *reinterpret<float4?>(cp + o1) = r1a;       *reinterpret<float4?>(cp + o1 + 4l) = r1b
        *reinterpret<float4?>(cp + o1 + 8l) = r1c;  *reinterpret<float4?>(cp + o1 + 12l) = r1d
        *reinterpret<float4?>(cp + o2) = r2a;       *reinterpret<float4?>(cp + o2 + 4l) = r2b
        *reinterpret<float4?>(cp + o2 + 8l) = r2c;  *reinterpret<float4?>(cp + o2 + 12l) = r2d
        *reinterpret<float4?>(cp + o3) = r3a;       *reinterpret<float4?>(cp + o3 + 4l) = r3b
        *reinterpret<float4?>(cp + o3 + 8l) = r3c;  *reinterpret<float4?>(cp + o3 + 12l) = r3d
    }
}

//! Register-tiled fp32 GEMM, C[M x N] += A[M x K] · B[K x N]. 4×16 tile over the bulk; the
//! L-shaped remainder (cols N%16, rows M%4) falls back to scalar. General over M/K/N. C must be
//! pre-initialized (the kernel accumulates). 2-3.5× the scalar form on the attention tile shapes.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def gemm_f32(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let m4 = M - M % 4l
    let n16 = N - N % 16l
    var ii = 0l
    while (ii < m4) {
        var jj = 0l
        while (jj < n16) {
            gemm_f32_uk_4x16(cp, ap, bp, ii, jj, K, N)
            jj += 16l
        }
        ii += 4l
    }
    if (n16 < N) {                                   // right strip: rows [0,m4) × cols [n16,N)
        gemm_f32_region(cp, ap, bp, 0l, m4, n16, N, K, N)
    }
    if (m4 < M) {                                    // bottom strip: rows [m4,M) × all cols
        gemm_f32_region(cp, ap, bp, m4, M, 0l, N, K, N)
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
//! Pointer core — the batched prefill norm threads over positions in fork contexts (raw addresses
//! only). `o`/`x` may alias (in-place post-norms): the sum over x completes before any write to o.
// noalias only on `weight` — `o` and `x` alias in-place for the Gemma2 post-norms (rms_at(buf,…,buf)).
[hint(noalias = weight)]
def rmsnorm(var o : float?; x : float const?; weight : float const?; size : int64; eps : float = 1.0e-5) {
    unsafe {
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
}
def rmsnorm(var o : array<float> | #; x : array<float> | #; weight : array<float> | #; size : int64; eps : float = 1.0e-5) {
    // reinterpret strips the temporary (#) qualifier — rms_at passes array_view (#) slices. Output `o`
    // keeps a non-const pointer (it's written); the read-only x/weight add const.
    unsafe {
        rmsnorm(reinterpret<float?>(addr(o[0])), reinterpret<float const?>(addr(x[0])),
            reinterpret<float const?>(addr(weight[0])), size, eps)
    }
}

//! softmax — turns a score vector into a probability distribution (sums to 1).
//! In place over the first `size` elements. Subtracts the max first so exp() can't
//! overflow (exp(x-max)/sum(exp(x-max)) is identical math, numerically safe).
//! Pointer core — the threaded prefill attention runs in fork contexts and can only touch raw
//! addresses; the array overload forwards here, so both are bit-identical. Output `x` is a `var
//! float?` written in place (a hoisted/captured fork pointer binds directly — no const-strip).
def softmax(var x : float?; size : int64) {
    unsafe {
        // Hand-vectorized max reduction (replaces a [vectorize] hint that LLVM ignored): max() lowers to
        // fcmp+select, which LLVM won't auto-vectorize as a reduction without nnan/nsz fast-math (we don't
        // emit it — bit-exactness). max IS associative+exact, so two float4 lane accumulators (width-8, the
        // original hint's intent — 2 accs hide the fmax latency, ~9x over scalar) + a horizontal max is
        // bit-identical to the scalar fold. The broadcast seed folds x[0] in (idempotent) and never reads
        // past the buffer, so it stays safe for size < 8 (early decode).
        let pf4 = reinterpret<float4 const?>(x)
        let n8 = size / 8l
        var acc0 = float4(x[0], x[0], x[0], x[0])
        var acc1 = acc0
        for (k in 0l..n8) {
            acc0 = max(acc0, pf4[2l * k])
            acc1 = max(acc1, pf4[2l * k + 1l])
        }
        let acc = max(acc0, acc1)
        var maxv = max(max(acc.x, acc.y), max(acc.z, acc.w))
        for (i in n8 * 8l .. size) {
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

//! Vectorized expf, 4 lanes — ggml_v_expf (ARM optimized-routines): a degree-4 poly on the
//! ln2-reduced argument times the 2^n exponent rebuilt by the round-to-int magic-number trick.
//! Max rel err ~2 ulp vs libm, ~7x faster than scalar exp() (which the JIT lowers to libm expf —
//! no vector libm on Darwin-ARM, so exp(float4) scalarizes). FAST PATH ONLY (no |n|>126 guard):
//! valid for x in ~[-87, 88]; outside that it returns garbage, so callers must clamp. The flash
//! softmax fold clamps masked/underflow entries to -87 (exp -> ~0) before calling.
//! NOTE: the JIT now also emits this same ggml_v_expf as the lowering for exp(floatN) directly
//! (build_vector_expf in modules/dasLLVM/daslib/llvm_jit_intrin.das) — but full-range (always pays the
//! |n|>126 guard, ~2.9x) and gated to aarch64 pending x64 hardware validation. This local exp4 is kept
//! separate because it's the cheaper fast-path-only form. Once x64 is verified, add a fast-path branch
//! to the intrinsic and replace this with exp(float4); until then they coexist deliberately.
def exp4(x : float4) : float4 {
    let z = mad(x, float4(1.4426950216293335), float4(12582912.0))   // x*log2e + 1.5*2^23
    let n = z - float4(12582912.0)                                   // round(x*log2e)
    let b = mad(n, float4(-1.428606765330187e-06), mad(n, float4(-0.693145751953125), x))  // x - n*ln2
    let u = b * b
    let p = mad(mad(mad(float4(0.008247390389442444), b, float4(0.04189976677298546)), u,
        mad(float4(0.16668395698070526), b, float4(0.4999912679195404))), u, float4(0.9999994039535522) * b)
    unsafe {
        let zbits = reinterpret<uint4>(z)
        let k = reinterpret<float4>((zbits << 23) + uint4(0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u))
        return mad(p, k, k)                                          // 2^n * (1 + poly(b))
    }
}

//! SiLU (a.k.a. swish) activation: silu(x) = x * sigmoid(x) = x / (1 + exp(-x)).
//! In place over the first `size` elements. The nonlinearity in the SwiGLU FFN gate;
//! smooth, ~identity for large x, dips slightly negative just below zero.
//! Pointer core (see softmax) — the threaded prefill FFN runs act_batch in fork contexts on raw addresses.
def silu(var x : float?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            let v = x[i]
            x[i] = v / (1.0 + exp(-v))
        }
    }
}
def silu(var x : array<float> | #; size : int64) {
    silu(unsafe(addr(x[0])), size)
}

//! GELU, tanh approximation (HF's gelu_pytorch_tanh): the GeGLU gate nonlinearity (Gemma2).
//! gelu(x) = 0.5x(1 + tanh(sqrt(2/pi)(x + 0.044715 x^3))). In place over the first `size`
//! elements. ggml's LLM_FFN_GELU approximates this same form via an fp16 lookup table; we
//! compute it directly (more accurate — near-ties against the LUT are expected, not bugs).
//! Pointer core (see softmax) — threaded prefill act_batch calls this on raw fork addresses.
def gelu(var x : float?; size : int64) {
    let k = 0.7978845608028654  // sqrt(2/pi)
    unsafe {
        for (i in range64(size)) {
            let v = x[i]
            x[i] = 0.5 * v * (1.0 + tanh(k * (v + 0.044715 * v * v * v)))
        }
    }
}
def gelu(var x : array<float> | #; size : int64) {
    gelu(unsafe(addr(x[0])), size)
}

//! Logit soft-capping (Gemma2): x = cap * tanh(x / cap), elementwise over the first `size`.
//! Smoothly bounds values to (-cap, cap). Applied to attention scores (cap 50) after the
//! 1/sqrt(head_size) scale and before softmax, and to the final classifier logits (cap 30).
//! Pointer core (see softmax) — threaded prefill attention calls this on raw fork addresses.
def softcap(var x : float?; size : int64; cap : float) {
    let inv = 1.0 / cap
    unsafe {
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
//! Pointer core (see softmax) — the threaded prefill ropes q_b/k_b in fork contexts on raw addresses.
//! `has_ff` gates the `ff` freq-factors pointer (empty() can't test a pointer); pass null when false.
[hint(noalias = v, noalias = ff)]
def rope_scaled(var v : float?; pos, head_size, n : int64; theta : float; has_ff : bool; ff : float const?) {
    unsafe {
        for (p in range64(n / 2l)) {
            let i = p * 2l
            let head_dim = i % head_size
            var freq = 1.0 / pow(theta, float(head_dim) / float(head_size))
            if (has_ff) {
                freq /= ff[head_dim / 2l]
            }
            let angle = float(pos) * freq
            let fcr = cos(angle)
            let fci = sin(angle)
            let v0 = v[i]
            let v1 = v[i + 1]
            v[i] = v0 * fcr - v1 * fci
            v[i + 1] = v0 * fci + v1 * fcr
        }
    }
}
def rope_scaled(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #) {
    let has_ff = !empty(freq_factors)
    unsafe {
        let ff = has_ff ? reinterpret<float const?>(addr(freq_factors[0])) : null
        rope_scaled(reinterpret<float?>(addr(vec[0])), pos, head_size, n, theta, has_ff, ff)
    }
}

//! NEOX-style RoPE (Qwen2 / Phi3 / Gemma2): the rotated pair is offset by head_size/2 — vec[base+j]
//! with vec[base+j+half] — instead of the adjacent (2j, 2j+1) pair the Llama-family NORM rope uses.
//! Same per-pair frequencies; only the pairing differs.
//! Pointer core (see rope_scaled) — fork-callable NEOX rope on a raw address.
[hint(noalias = v, noalias = ff)]
def rope_scaled_neox(var v : float?; pos, head_size, n : int64; theta : float; has_ff : bool; ff : float const?; mscale : float = 1.0) {
    unsafe {
        let half = head_size / 2l
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                var freq = 1.0 / pow(theta, float(2l * j) / float(head_size))
                if (has_ff) {
                    freq /= ff[j]
                }
                let angle = float(pos) * freq
                // mscale (LongRoPE attn_factor) scales cos/sin — i.e. the rotated magnitude (Phi3 = 1.19)
                let fcr = cos(angle) * mscale
                let fci = sin(angle) * mscale
                let v0 = v[base + j]
                let v1 = v[base + j + half]
                v[base + j] = v0 * fcr - v1 * fci
                v[base + j + half] = v0 * fci + v1 * fcr
            }
        }
    }
}
def rope_scaled_neox(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #; mscale : float = 1.0) {
    let has_ff = !empty(freq_factors)
    unsafe {
        let ff = has_ff ? reinterpret<float const?>(addr(freq_factors[0])) : null
        rope_scaled_neox(reinterpret<float?>(addr(vec[0])), pos, head_size, n, theta, has_ff, ff, mscale)
    }
}

//! Table-driven RoPE (NORM, adjacent pairs). cosrow/sinrow are this position's row of the cos/sin
//! table (head_size/2 entries) precomputed once by build_rope_table. Bit-identical to rope_scaled —
//! the angle pos*freq is identical across every head and layer, so its cos/sin is hoisted out of the
//! per-head/per-layer loop into the table and just looked up here.
//! Looped per head (frequency-pair j is the inner index, 0..half-1, the same for every head) rather
//! than over a flat p with j = p % half: the per-pair integer modulo dominated the rotation (it's all
//! cheap arithmetic) — dropping it is ~2.3x on the kernel. (NEON [vectorize] does NOT help: the
//! interleaved stride-2 in-place rotation is bandwidth-bound, measured slightly slower vectorized.)
[hint(noalias = v, noalias = cosrow, noalias = sinrow)]
def rope_scaled_tab(var v : float?; head_size, n : int64; cosrow : float const?; sinrow : float const?) {
    unsafe {
        let half = head_size / 2l
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                let i = base + j * 2l
                let fcr = cosrow[j]
                let fci = sinrow[j]
                let v0 = v[i]
                let v1 = v[i + 1]
                v[i] = v0 * fcr - v1 * fci
                v[i + 1] = v0 * fci + v1 * fcr
            }
        }
    }
}

//! Table-driven NEOX RoPE (pair offset by head_size/2). Same table as rope_scaled_tab; the NEOX
//! mscale magnitude factor is baked into cosrow/sinrow by build_rope_table. Bit-identical to rope_scaled_neox.
//! The NEOX split layout (v0 in [base..base+half), v1 in [base+half..)) is four contiguous arrays, so
//! the inner loop vectorizes width-8 natively (~5-10% over scalar) — measured Δ=0, i.e. plain mul/sub
//! under [vectorize] does NOT contract to fma, so it stays bit-exact (no `mad`). The NORM leaf above
//! can't do this: its interleaved (2j, 2j+1) layout is stride-2 and the JIT won't emit vld2/vst2 for it.
[hint(noalias = v, noalias = cosrow, noalias = sinrow)]
def rope_scaled_neox_tab(var v : float?; head_size, n : int64; cosrow : float const?; sinrow : float const?) {
    unsafe {
        let half = head_size / 2l
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(half)) {
                let fcr = cosrow[j]
                let fci = sinrow[j]
                let v0 = v[base + j]
                let v1 = v[base + j + half]
                v[base + j] = v0 * fcr - v1 * fci
                v[base + j + half] = v0 * fci + v1 * fcr
            }
        }
    }
}

//! Dispatch RoPE by the model's convention: NEOX (Qwen2/Phi3/Gemma2) or the default NORM (Llama).
//! mscale is the LongRoPE magnitude factor (1.0 for everything but Phi3); only the NEOX path uses it.
//! Pointer core (see rope_scaled) — fork-callable rope dispatch on a raw address.
def rope_apply(var vec : float?; pos, head_size, n : int64; theta : float; has_ff : bool; ff : float const?; neox : bool; mscale : float = 1.0) {
    if (neox) {
        rope_scaled_neox(vec, pos, head_size, n, theta, has_ff, ff, mscale)
    } else {
        rope_scaled(vec, pos, head_size, n, theta, has_ff, ff)
    }
}
def rope_apply(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #; neox : bool; mscale : float = 1.0) {
    let has_ff = !empty(freq_factors)
    unsafe {
        let ff = has_ff ? reinterpret<float const?>(addr(freq_factors[0])) : null
        rope_apply(reinterpret<float?>(addr(vec[0])), pos, head_size, n, theta, has_ff, ff, neox, mscale)
    }
}
