options gen2
options _comment_hygiene = true

module dasllama_math shared public

require math
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require dasllama/dasllama_plane  // PlaneF/PlaneI8 — the borrowed-plane weight forms below
require dasllama/dasllama_config  // DlimVulkanConfig — the dry-bake seam's config argument
require daslib/jobque_boost public  // matmul's threaded path expands parallel_for; consumers need its symbols
require daslib/jobque_profile  // named trace categories + unit markers (set_trace_tags registers them)
require dasllama/dasllama_par  // maybe_parallel_for: thread-or-inline a kernel body on a runtime size flag
require dasllama/dasllama_tune  // [tuned]: reconstitute dot from dot_template with per-box loop hints
require dasllama/dasllama_gemm_schema  // kq_qsb/kq_ssb — the fmt-id kq plane strides
require dasllama/dasllama_gpu_tier public  // the device-cooperation seam (split out 2026-07-29) — re-exported for the 112 requirers
require llvm/daslib/f16_cvt  // f16<->f32 converts: JIT half intrinsics on aarch64 / x64+F16C, exact bit-twiddle elsewhere

// dasLLAMA numeric primitives — naive fp32 baseline (curriculum step 1).
// Correctness first; optimization (blocking, float4, threading) comes later.
//
// Every array param is `array<float> | #` (owning array OR borrowed view), so weights
// can be passed zero-copy as views into one big model blob and outputs can be written
// straight into a view (e.g. the KV cache). Dims/indices are int64 for large models.

//! The library-seam ``-jit`` stop: dasLLAMA inference NEVER runs interpreted (kernels 10-40x
//! slower). Inference seams call it; scaffolding that only parses or peeks never crosses it.
def guard_interp_inference(what : string) {
    if (!jit_enabled() && !is_standalone_exe()) {
        panic("dasLLAMA: {what} requires -jit (or a compiled exe) — interpreted inference runs the kernels 10-40x slower")
    }
}

//! Dot product of two n-length float buffers, auto-vectorized (width-8, no bounds checks,
//! non-aliasing). `dot_template` is the single source (hot loop `[tune=1]`); the emitted `dot`
//! reconstitutes it via `[tuned]` — default `vec8_u2` is the original hints, bit-identical.
[hint(unsafe_range_check, noalias = a, noalias = b)]
def template dot_template(a, b : float const ?; n : int64) : float {
    var acc = 0.0
    for [tune = 1] (j in range64(n)) {
        acc = mad(unsafe(a[j]), unsafe(b[j]), acc)   // fused multiply-add (vectorizes to fmla)
    }
    return acc
}

[hint(unsafe_range_check, noalias = a, noalias = b), tuned]
def dot(a, b : float const?; n : int64) : float {}

// The elementwise kernels below share dot's tuning rig: each `<name>_template` holds the `[tune=1]`
// hot loop and `[tuned]` reconstitutes `<name>` with the per-box permutation from the app tune
// sidecar — default `vec8_u2` is bit-identical until the profiler picks a different perm.

//! AXPY: d[j] += scale * s[j] over n elements, auto-vectorized. Used single-threaded AND as the
//! V-accumulate in threaded prefill attention — `d`/`s` never overlap, so noalias holds inside
//! fork workers too (a captured `var d : float?` stays a writable pointer — capture freezes the binding, not the pointee).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template axpy_template(var d : float ?; s : float const ?; scale : float; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = mad(scale, s[j], d[j])
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def axpy(var d : float?; s : float const?; scale : float; n : int64) {}

//! Dot of an f32 row against a bfloat16 (bf16-in-uint16) row. The widen is an exact high-half
//! bit shift, so dots are bit-identical to pre-expanding the weight to fp32 at half the bandwidth
//! (llama.cpp's AVX2 ggml_vec_dot_bf16 also truncates the ACTIVATION to bf16; ours stays f32).
[hint(unsafe_range_check, noalias = a, noalias = b)]
def template dot_bf16_template(a : float const ?; b : uint16 const ?; n : int64) : float {
    var acc = 0.0
    for [tune = 1] (j in range64(n)) {
        acc = mad(unsafe(a[j]), unsafe(reinterpret<float>(uint(unsafe(b[j])) << 16u)), acc)
    }
    return acc
}

[hint(unsafe_range_check, noalias = a, noalias = b), tuned]
def dot_bf16(a : float const?; b : uint16 const?; n : int64) : float {}

// The KV-cache codec families (f16/q8_0/tq4 store/read/dot/axpy + the tq4 FWHT rotation) live in
// dasllama_kv_codec; the generic f16 row converts (cvt_f32_to_f16/cvt_f16_to_f32) in dasllama_convert.

//! d[j] += s[j], width-8 vectorized (the residual-stream update). The plain `for k; a[k]+=b[k]`
//! over bounds-checked arrays does NOT vectorize under the JIT — this pointer form is ~5x faster.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template add_inplace_template(var d : float ?; s : float const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] += s[j]
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def add_inplace(var d : float?; s : float const?; n : int64) {}

//! d[j] = (d[j] + s[j]) * scale in ONE pass — the residual add fused with the per-layer output
//! scale (gemma4 layer_out_scale, the PLE 1/sqrt(2) fold). Bit-identical to add_inplace followed
//! by scale_inplace (same two fp ops per element, same order) at one RMW walk instead of two.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template add_scale_inplace_template(var d : float ?; s : float const ?; scale : float; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = (d[j] + s[j]) * scale
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def add_scale_inplace(var d : float?; s : float const?; scale : float; n : int64) {}

//! d[j] *= s[j], width-8 vectorized — the SwiGLU/GeGLU gate multiply (act(W1·x) * (W3·x)).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template mul_inplace_template(var d : float ?; s : float const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] *= s[j]
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def mul_inplace(var d : float?; s : float const?; n : int64) {}

//! d[j] *= scale (broadcast scalar), width-8 vectorized — ggml_vec_scale_f32. Flash attention
//! multiplies the whole Q×KV score tile by 1/sqrt(head_size) in one sweep, replacing the old scalar
//! per-element multiply buried in the mask branch.
[hint(unsafe_range_check, noalias = d)]
def template scale_inplace_template(var d : float ?; scale : float; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] *= scale
        }
    }
}
[hint(unsafe_range_check, noalias = d), tuned]
def scale_inplace(var d : float?; scale : float; n : int64) {}

//! d[j] = s[j], width-8 vectorized contiguous copy (the KV-cache store). Same reason as add_inplace:
//! a bounds-checked array copy does NOT vectorize under the JIT; this pointer form does. d (cache) and
//! s (the k_b/v_b scratch) are always distinct arrays, so noalias is correct and free.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template copy_floats_template(var d : float ?; s : float const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = s[j]
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def copy_floats(var d : float?; s : float const?; n : int64) {}

// Spin-before-park window for the jobque workers (see setup_dasllama_jobque). Runtime knob: the
// useful window tracks the box's serial-gap lengths, not the kernel. 30ms covers the between-call
// gaps of a decode/prefill loop, keeping workers hot through a whole token instead of parking.
var g_jobque_spin_us = 30000l
def public set_jobque_spin_us(v : int64) { g_jobque_spin_us = max(v, 0l) }
def public get_jobque_spin_us() : int64 { return g_jobque_spin_us }

// Join-poll level for fork/join joins (see setup_dasllama_jobque) — ggml's --poll denomination:
// level*1024*128 relax-rounds on the joining thread before it parks (0 = park immediately).
// 50 is ggml's shipped default; the poll burns only the calling thread and is bounded (~100ms).
var g_jobque_join_poll = 50l
def public set_jobque_join_poll(v : int64) { g_jobque_join_poll = max(v, 0l) }
def public get_jobque_join_poll() : int64 { return g_jobque_join_poll }

// Worker-pool limit (JobQue::setWorkerLimit — workers above the limit sleep dormant): the
// tiny-model dial. An inline-dominated decode token runs FASTER with fewer awake workers (zen2
// 135M: T=8 176 t/s vs T=48 149 — T-1 pollers interfere with main-thread weight streaming). 0 = no limit.
var g_dispatch_worker_limit = 0
def public set_dispatch_worker_limit(v : int) { g_dispatch_worker_limit = max(v, 0) }
def public get_dispatch_worker_limit() : int { return g_dispatch_worker_limit }

// Per-op team rank gate (JobQue::setTeamRankGate — each team op admits only as many workers as
// its widest stage published chunks). Server-core boxes want it ON (135M T=48: +10% zen2/+48% SPR,
// >=1B neutral); heterogeneous P/E (M1) keep it off. Tri-state: -1 = que default, 0/1 = force.
var g_team_rank_gate = -1
def public set_team_rank_gate(v : int) { g_team_rank_gate = clamp(v, -1, 1) }
def public get_team_rank_gate() : int { return g_team_rank_gate }

// Trace op tags (jobque_trace_tag): stamped before dispatches so the lane-timeline viewer colors
// by OP rather than stage-count. Gated — set_trace_tags(true) rides with jobque_trace_start in
// the profiling harness; production dispatches never pay the extern call.
var g_trace_tags = false
def public set_trace_tags(on : bool) {   // nolint:LINT012,LINT019 — on is only read when JOBQUE_PROFILING_ENABLED compiles the body in
    static_if (JOBQUE_PROFILING_ENABLED) {
        g_trace_tags = on
        if (on && is_job_que_available()) {   // registries live on the que; stay que-independent like trace_tag
            register_trace_categories()
        }
    }
}
let public TRACE_TAG_ATTN_CHAIN = 1
let public TRACE_TAG_ATTN_STD = 2
let public TRACE_TAG_FFN_CHAIN = 3
let public TRACE_TAG_FFN_STD = 4
let public TRACE_TAG_CLS = 5
let public TRACE_TAG_MOE = 6
let public TRACE_TAG_SHEXP = 7
let public TRACE_TAG_PLE = 8
let public TRACE_TAG_ROUTE = 9   // MoE router GEMM + select/bucket (prefill section split)
let public TRACE_TAG_PARK = 10   // MoE park/reduce bookkeeping around the expert GEMMs
let public TRACE_TAG_DN = 11   // Gated DeltaNet layers (qwen35 hybrid) — separates dn from gated attn
// ASR encoder stages (dasllama_audio's shared whisper-core frontend + block loop + projector)
let public TRACE_TAG_ENC_CONV = 12
let public TRACE_TAG_ENC_ATTN_PROJ = 13
let public TRACE_TAG_ENC_ATTN = 14
let public TRACE_TAG_ENC_ATTN_OUT = 15
let public TRACE_TAG_ENC_FC1 = 16
let public TRACE_TAG_ENC_FC2 = 17
let public TRACE_TAG_ENC_PROJ = 18
def public trace_tag(t : int) {   // nolint:LINT012,LINT019 — t is only read when JOBQUE_PROFILING_ENABLED compiles the body in
    static_if (JOBQUE_PROFILING_ENABLED) {
        if (g_trace_tags) {
            jobque_trace_tag(t)
        }
    }
}
def public trace_marker(name : string; arg : int) {   // nolint:LINT012,LINT019 — args only read when JOBQUE_PROFILING_ENABLED compiles the body in
    // instant unit boundary ("token", "layer", "step") on the caller lane; same gate as trace_tag
    static_if (JOBQUE_PROFILING_ENABLED) {
        if (g_trace_tags && is_job_que_available()) {
            profile_marker(name, arg)
        }
    }
}
// names/colors match the lane-timeline viewer's dark-theme palette, so saved traces carry
// the exact legend the artifact hardcoded
def private register_trace_categories() {
    profile_category(TRACE_TAG_ATTN_CHAIN, "attn chain", 0x5E96DEu)
    profile_category(TRACE_TAG_ATTN_STD, "attn std", 0x5E96DEu)
    profile_category(TRACE_TAG_FFN_CHAIN, "ffn chain", 0x43B08Au)
    profile_category(TRACE_TAG_FFN_STD, "ffn per-op", 0x43B08Au)
    profile_category(TRACE_TAG_CLS, "classifier", 0xAC82E4u)
    profile_category(TRACE_TAG_MOE, "moe experts", 0xE08563u)
    profile_category(TRACE_TAG_SHEXP, "shexp", 0xC39536u)
    profile_category(TRACE_TAG_PLE, "ple", 0xC39536u)
    profile_category(TRACE_TAG_ROUTE, "router", 0x4FC3D0u)
    profile_category(TRACE_TAG_PARK, "park/reduce", 0xD678B4u)
    profile_category(TRACE_TAG_DN, "deltanet", 0x8385E8u)
    profile_category(TRACE_TAG_ENC_CONV, "enc conv", 0x4FA8A0u)
    profile_category(TRACE_TAG_ENC_ATTN_PROJ, "enc attn proj", 0x7FA8E8u)
    profile_category(TRACE_TAG_ENC_ATTN, "enc attn core", 0x4C7FC4u)
    profile_category(TRACE_TAG_ENC_ATTN_OUT, "enc attn out", 0x91B8ECu)
    profile_category(TRACE_TAG_ENC_FC1, "enc fc1", 0x3FA378u)
    profile_category(TRACE_TAG_ENC_FC2, "enc fc2", 0x6BC49Eu)
    profile_category(TRACE_TAG_ENC_PROJ, "enc proj", 0x9B7BD0u)
}

//! Configure the job queue for dasLLAMA's pure fork/join matmul dispatch: pooled fork contexts,
//! batched dispatch, spin-before-park window, join-poll level. Call INSIDE `with_job_que` (or
//! after `create_job_que`) — the queue must exist. Engine spelling of `setup_dasllama_jobque`.
def setup_dasllama_jobque_() {
    set_jobque_fork_pool(true, true)
    set_jobque_batch_dispatch(true)
    // team dispatch (#3368) as the runner default: fifo fork/join costs ~10us PER JOB, so a
    // 96-lane dispatch is ~1ms x ~160 dispatches/decoded token — EPYC 9654 measured Llama-1B
    // decode 7.6 -> 43 t/s (5.7x) and prefill-512 2x from this one switch (history/dasLLAMA/epyc9654_measurements.md)
    set_jobque_team_mode(true)
    set_jobque_worker_spin(int(min(g_jobque_spin_us, 2147483647l)))
    set_jobque_join_spin(int(min(g_jobque_join_poll, 2147483647l)))
    set_jobque_worker_limit(g_dispatch_worker_limit > 0 ? g_dispatch_worker_limit : -1)   // -1 = unlimited
    if (g_team_rank_gate >= 0 && is_none(g_env_core.jobque_team_rank_gate)) {
        set_jobque_team_rank_gate(g_team_rank_gate != 0)
    }
    // The dispatch caller is load-bearing in team mode — only it publishes chains, so a
    // descheduled caller idles every lane (4B Q4 32L trace: 13 all-lane holes/token, 1.7% worst).
    // Que creation ran this thread at High; claim the top notch. DASLLAMA_CALLER_PRIO=<-2..2> is the A/B rail.
    set_current_thread_priority(clamp(g_env_engine.caller_prio, -2, 2))
}

// dasLLAMA matmuls are lockstep linear algebra: SMT siblings share the FMA/load ports, so the
// default jobque gets physical cores - 1 workers (ggml's -t default; EPYC 9654 confirmed 192 SMT
// lanes lose to 96). Cap min()s with the stock rule; DAS_JOBQUE_THREADS overrides.
[init]
def dasllama_jobque_threads_cap() {
    set_jobque_threads_cap(max(get_total_hw_cores() - 1, 1))
}

// Chunks-per-hw-job for the quantized matmuls' row split (the fp32 kernels use 1). Q8's
// dequant-in-dot is compute-bound and balances better at 2 chunks/job; Q4's heavy nibble
// unpack at 4 (both measured M1 Max). Per-box knobs; clamped to >= 1.
var g_q8_chunks_per_job = 2
var g_q4_chunks_per_job = 4
def public set_q8_chunks_per_job(v : int) { g_q8_chunks_per_job = max(v, 1) }
def public get_q8_chunks_per_job() : int { return g_q8_chunks_per_job }
def public set_q4_chunks_per_job(v : int) { g_q4_chunks_per_job = max(v, 1) }
def public get_q4_chunks_per_job() : int { return g_q4_chunks_per_job }

// Oversplit factor for the quantized BATCH matmuls (prefill GEMM): chunks = hw_jobs * this.
// More chunks than workers lets awake workers pop a late-woken worker's remaining chunks off the
// fifo (3990X @32 workers: fork/join bill -41% -> -15% at x4; x8 is past the per-chunk-overhead knee).
var g_q8_batch_chunks_per_job = 4
def public set_q8_batch_chunks_per_job(v : int) { g_q8_batch_chunks_per_job = max(v, 1) }
def public get_q8_batch_chunks_per_job() : int { return g_q8_batch_chunks_per_job }

// Dispatch lanes for a fork/join chunk split: worker threads PLUS the calling thread (team mode
// by design). Sizing chunk counts to workers alone leaves the last wave short (7 workers x4 = 28
// chunks/8 lanes = 87.5%); sizing to lanes took decode GEMV ~109 -> ~118 GB/s on M1 Max.
var g_dispatch_caller_lane = true
def public set_dispatch_caller_lane(v : bool) { g_dispatch_caller_lane = v }
def public get_dispatch_caller_lane() : bool { return g_dispatch_caller_lane }
def public get_dispatch_lanes() : int {
    // Que-less process: the calling thread is the only lane, so every shaper answers "inline".
    // (The work-proportional shapers evaluate eagerly at the dispatch sites — the old bool-gated
    // form only read this in its parallel arm, so que-less small-matmul callers never noticed.)
    if (!is_job_que_available()) {
        return 1
    }
    // the live pool: dormant over-limit workers (set_jobque_worker_limit) never serve chunks,
    // so the shapers must size waves to the awake workers only
    let live = min(get_total_hw_jobs(), get_jobque_worker_limit())
    return live + (g_dispatch_caller_lane ? 1 : 0)
}

def public get_dispatch_slot_bound() : int {
    // exclusive upper bound on the slot maybe_parallel_for_indexed hands its block — size
    // per-slot scratch to this. Caller participates as the LAST slot; a worker limit only shrinks
    // the live set, so dormant workers keep their high slot numbers (bound stays pool-sized).
    return is_job_que_available() ? get_total_hw_jobs() + 1 : 1
}

// Min grain for matmul row splits (ggml-cpu.c mul_mat chunk_size = 16): without it a fixed
// lanes×oversplit chunk count shatters a small matmul into sub-grain work items on many-core
// boxes (EPYC 9654: kv 2048x512 GEMM 2.5x slower at 192 lanes than at 48). 0 disables.
var g_matmul_min_chunk_rows = 16
def public set_matmul_min_chunk_rows(v : int) { g_matmul_min_chunk_rows = max(v, 0) }
def public get_matmul_min_chunk_rows() : int { return g_matmul_min_chunk_rows }

// The dispatch invariant that killed the prefill regression: a chunk count > lanes MUST be a whole
// multiple of dispatch lanes, so no wave is ragged (a partial final wave makes the join wait on
// stragglers). Fires in debug/test builds so a future chunk-math change can't silently reintroduce it.
def private assert_wave_aligned(chunks, lanes : int) {
    assert(chunks <= lanes || chunks % lanes == 0, "matmul dispatch chunk count must be a whole number of dispatch-lane waves")
}

// ===== work-proportional dispatch (lanes sized to the matmul, not the box) =====

// MACs of work that justify one dispatch chunk: lanes_for_work invites one lane per this much
// work (grain floors still keep tiny matmuls inline). Default 1 = dispatch everything the grains
// admit — the old 1M default left every per-layer attn matmul of a <=4B model single-lane (-5..-17% tok/s).
var g_target_chunk_work = 1l
def public set_target_chunk_work(v : int64) { g_target_chunk_work = max(v, 1l) }
def public get_target_chunk_work() : int64 { return g_target_chunk_work }

// Per-regime lane caps (0 = uncapped). GEMV rides memory bandwidth — past the box's DRAM knee
// extra lanes just queue at the memory door (team_probe's streaming ladder finds the knee).
// Batch (GEMM) is compute-bound and caps at the overhead knee instead.
var g_gemv_lane_cap = 0
def public set_gemv_lane_cap(v : int) { g_gemv_lane_cap = max(v, 0) }
def public get_gemv_lane_cap() : int { return g_gemv_lane_cap }

var g_batch_lane_cap = 0
def public set_batch_lane_cap(v : int) { g_batch_lane_cap = max(v, 0) }
def public get_batch_lane_cap() : int { return g_batch_lane_cap }

//! Dispatch lanes proportional to work: clamp(work / target_chunk_work, 1,
//! min(get_dispatch_lanes(), lane_cap)). 1 means "not worth a dispatch" — the matmul_chunks
//! shapers return 1 and the num_jobs form of maybe_parallel_for runs inline. lane_cap 0 = uncapped.
def public lanes_for_work(work : int64; lane_cap : int) : int {
    let lanes = max(get_dispatch_lanes(), 1)
    let cap = lane_cap > 0 ? min(lanes, lane_cap) : lanes
    return int(clamp(work / g_target_chunk_work, 1l, int64(cap)))
}

//! Work-aware chunk count for a batch (GEMM) row split: L = lanes_for_work(work, batch cap);
//! 1 => inline; else chunks rounded DOWN to a whole number of L-waves so no ragged final wave
//! makes the join wait on stragglers. `rows` is the split range's total row count; `work` = MAC count.
def public matmul_chunks(rows : int; oversplit : int; work : int64) : int {
    let L = lanes_for_work(work, g_batch_lane_cap)
    if (L <= 1) {
        return 1
    }
    let want = L * max(oversplit, 1)
    if (g_matmul_min_chunk_rows <= 0) {
        return want
    }
    let cap = clamp(rows / g_matmul_min_chunk_rows, 1, want)   // grain-limited ceiling (<= want = L × oversplit)
    // the grain may only limit OVERSPLIT — never cut chunks below the admitted lanes (underfilling
    // L idles lanes for the whole dispatch, always worse than sub-grain chunks; rows < L is the
    // only natural floor)
    if (cap < L) {
        let chunks = clamp(rows, 1, L)
        assert_wave_aligned(chunks, L)
        return chunks
    }
    let chunks = (cap / L) * L                                 // whole L-waves within the grain cap
    assert_wave_aligned(chunks, L)
    return chunks
}

// ===== 2-D batch chunk grid (row-units × token-blocks) — the chunk-starved-shape remedy =====

// The per-box 2-D grid pin (box_profile "batch_grid_2d"): 0 = off (1-D always); 1/2 arm the
// per-dispatch auto-gate — 2-D engages only when the 1-D grain-capped chunk count can't fill one
// wave (a starved shape idles lanes); big GEMMs keep the weight-stationary 1-D walk regardless.
var g_batch_grid_2d = 0
def public set_batch_grid_2d(v : int) { g_batch_grid_2d = clamp(v, 0, 2) }
def public get_batch_grid_2d() : int { return g_batch_grid_2d }

def private gcd_int(a, b : int) : int {
    var x = max(a, 1)
    var y = max(b, 1)
    while (y != 0) {
        let t = y
        y = x % y
        x = t
    }
    return x
}

//! Chunk grid for the token-blocked batch GEMM: (row chunks, token chunks). y == 1 => stay 1-D.
//! With batch_grid_2d armed, a starved shape (1-D chunks < admitted lanes) goes 2-D: the token
//! axis supplies missing parallelism at the same row grain (cells decode rows-fastest, sharing LLC).
def public matmul_grid(units : int; unit_rows : int64; oversplit : int; work : int64; ntok : int64; tokq : int) : int2 {
    let rows = units * int(unit_rows)
    let c1 = matmul_chunks(rows, oversplit, work)
    if (g_batch_grid_2d == 0 || c1 <= 1) {
        return int2(c1, 1)
    }
    let L = lanes_for_work(work, g_batch_lane_cap)
    let ntile = int(ntok / int64(tokq))
    // grain-starved = the 1-D count only reaches L by going sub-grain (matmul_chunks fills to L);
    // the token axis supplies the missing parallelism at FULL row grain instead — weight rows
    // stream in grain-sized spans, re-read once per token cell
    let gcap = g_matmul_min_chunk_rows > 0 ? clamp(rows / g_matmul_min_chunk_rows, 1, c1) : c1
    if (gcap >= L || ntile < 2) {   // not grain-starved, or no token axis to split
        return int2(c1, 1)
    }
    let rc = clamp(gcap, 1, max(units, 1))   // row cells finer than one unit are empty claims
    var ntc = 1
    if (g_batch_grid_2d == 1) {
        ntc = min((int(ntok) + 15) / 16, ntile)
    } else {
        // smallest whole-wave ntc covering the oversplit target; alignment that can't fit the
        // available tiles stays 1-D (a clamped ntc would be unaligned = strategy 1 in disguise)
        let step = L / gcd_int(rc, L)
        let need = (L * max(oversplit, 1) + rc - 1) / rc
        ntc = ((need + step - 1) / step) * step
        if (ntc > ntile) {
            return int2(c1, 1)
        }
    }
    return ntc > 1 ? int2(rc, ntc) : int2(c1, 1)
}

// Coarser grain for GEMV-shaped splits (ggml mul_mat: chunk_size 16 -> 64 when nr0==1||nr1==1):
// a decode matvec row is far less work than a GEMM row reused across ntok tokens (EPYC 9654:
// grain 64 = +13-15% decode at default threads). 0 disables.
var g_matmul_min_chunk_rows_gemv = 64
def public set_matmul_min_chunk_rows_gemv(v : int) { g_matmul_min_chunk_rows_gemv = max(v, 0) }
def public get_matmul_min_chunk_rows_gemv() : int { return g_matmul_min_chunk_rows_gemv }

// GEMV tail: the one-chunk-per-lane equal split leaves the join waiting out the SLOWEST lane's
// whole chunk (median lane idles 14-24% of every GEMV stage, per-lane trace 3990X). >1 publishes
// wave-aligned chunks per lane so finished lanes keep serving; default 8; 1 = historical equal split.
var g_gemv_chunks_per_lane = 8
def public set_gemv_chunks_per_lane(v : int) { g_gemv_chunks_per_lane = max(v, 1) }
def public get_gemv_chunks_per_lane() : int { return g_gemv_chunks_per_lane }

// Wave fill: when the grain-derived count lands below admitted lanes, round UP to one full wave —
// underfilling always idles more than going sub-grain (30B decode router: 128 rows/grain 64 = 2
// chunks on 16 lanes, 14 idle, ~3.3% of token). rows < lanes is the only natural floor.
var g_gemv_wave_fill = true
def public set_gemv_wave_fill(v : bool) { g_gemv_wave_fill = v }
def public get_gemv_wave_fill() : bool { return g_gemv_wave_fill }

//! matmul_chunks for GEMV-shaped dispatches (single-token mm/group3/groupn): L = lanes_for_work
//! (work, gemv cap); 1 => inline; else the coarser GEMV grain, capped at ONE chunk per lane (a
//! decode chunk is one bandwidth stream — EPYC T=24: last lane's +1 chunk = 37% of op wall).
def public matmul_chunks_gemv(rows : int; oversplit : int; work : int64) : int {
    let L = lanes_for_work(work, g_gemv_lane_cap)
    if (L <= 1) {
        return 1
    }
    if (g_matmul_min_chunk_rows_gemv <= 0) {
        return L * max(oversplit, 1)
    }
    if (g_gemv_chunks_per_lane > 1) {
        let waves = min(rows / (g_matmul_min_chunk_rows_gemv * L), g_gemv_chunks_per_lane)
        if (waves >= 1) {
            let chunks = waves * L
            assert_wave_aligned(chunks, L)
            return chunks
        }
        // fewer than one grain-sized chunk per lane — the grain-limited split below applies
    }
    let n = clamp(rows / g_matmul_min_chunk_rows_gemv, 1, L)
    let chunks = (g_gemv_wave_fill && n < L) ? clamp(rows, 1, L) : n
    assert_wave_aligned(chunks, L)
    return chunks
}

//! LPT (longest-processing-time-first) claim order for a region-list dispatch with known unequal
//! per-region costs: sort (id, cost) biggest-cost first (ties: lowest id) so the self-serve chunk
//! queue front-loads expensive regions and drains cheap ones last. Use whenever cost is known at build time.
def public lpt_order(var ord : array<int2>) {
    ord |> sort() $(a, b) => a.y != b.y ? a.y > b.y : a.x < b.x
}

//! Chunk count for a fused-chain stage over `groups` 32-row quant groups: the GEMV shaper's count,
//! except fine-grain mode (gemv_chunks_per_lane > 1) splits at GROUP granularity (capped at
//! lanes×fine) so a ragged last wave costs one 32-row chunk, not a whole lane's share (~25% join tail fix, 3990X).
def public chain_stage_chunks(groups : int; rows : int; work : int64) : int {
    if (g_gemv_chunks_per_lane > 1) {
        let L = lanes_for_work(work, g_gemv_lane_cap)
        if (L > 1) {
            return min(groups, L * g_gemv_chunks_per_lane)
        }
    }
    return matmul_chunks_gemv(rows, 1, work)
}

//! y = W·x — matrix-vector product, the dominant LLM compute kernel. W is row-major [d x n],
//! x is [n], y is [d]; each y[i] is one dot of W's row i with x (matches llama2.c: n=input, d=output).
//! Large matmuls thread output rows (requires an enclosing with_job_que); small stay single-thread.
def matmul(var y : array<float>; w : array<float> | #; x : array<float>; n, d : int64) {
    unsafe {
        var yp = addr(y[0])                              // var: stays a writable pointer when captured
        let wp = addr<float const?>(w[0])   // strip the `#` if w is a view
        let xp = addr(x[0])
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot(wp + int64(i) * n, xp, n)
                }
            }
        }
    }
}

//! Borrowed-plane form of the blob+offset matvec.
def matmul(var y : array<float>; w : PlaneF; woff : int64; x : array<float>; n, d : int64) {
    matmul_at(y, plane_at(w, woff), x, n, d)
}

//! Blob+offset form: the weight is rows [d x n] at element offset `woff` inside `w` — no
//! array_view (the view's block invoke is an LLVM optimization boundary; the kernel only ever
//! wanted the base pointer).
def matmul(var y : array<float>; w : array<float>; woff : int64; x : array<float>; n, d : int64) {
    matmul_at(y, unsafe(addr<float const?>(w[woff])), x, n, d)
}

def private matmul_at(var y : array<float>; wp : float const?; x : array<float>; n, d : int64) {
    unsafe {
        var yp = addr(y[0])
        let xp = addr(x[0])
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot(wp + int64(i) * n, xp, n)
                }
            }
        }
    }
}

//! Batched fp32 GEMM for prefill: Y [ntok x d] = X [ntok x n]·Wᵀ, W row-major [d x n]. Both
//! token-major. Weight-stationary nest (rows outer) — each weight row streams from DRAM once,
//! reused across all ntok tokens (vs ntok× matmul re-reading W). Bit-for-bit ntok× matmul.
def matmul_batch(var y : array<float>; w : array<float> | #; x : array<float>; n, d, ntok : int64) {
    unsafe {
        matmul_batch_core(addr(y[0]), addr<float const?>(w[0]), addr<float const?>(x[0]), n, d, ntok)
    }
}

// The fp32 batch GEMM core: 1-D row split, except ROW-STARVED shapes (MoE router: d=n_expert=128,
// grain 16 => 8 chunks idling half a 16-lane box) split tokens too — a flat (row-chunk × token-block)
// domain. Disjoint stores, same dot per (row, token) — bit-identical either way.
def private matmul_batch_core(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) {
    var myp = yp
    let rch = matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)
    let L = lanes_for_work(n * d * ntok, g_batch_lane_cap)
    let tb = (rch < L && ntok >= 128l) ? min(int64((2 * L + rch - 1) / rch), ntok / 64l) : 1l
    if (tb <= 1l) {
        maybe_parallel_for(0, int(d), rch) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let wrow = wp + int64(i) * n
                    for (tk in range64(ntok)) {
                        myp[tk * d + int64(i)] = dot(wrow, xp + tk * n, n)
                    }
                }
            }
        }
        return
    }
    let rows_per = (d + int64(rch) - 1l) / int64(rch)
    let tok_per = (ntok + tb - 1l) / tb
    maybe_parallel_for(0, int(int64(rch) * tb), int(int64(rch) * tb)) $(cb, ce) {
        unsafe {
            for (c in range(cb, ce)) {
                let rc = int64(c) / tb
                let tc = int64(c) % tb
                let r1 = min(rc * rows_per + rows_per, d)
                let t1 = min(tc * tok_per + tok_per, ntok)
                for (i in range64(rc * rows_per, r1)) {
                    let wrow = wp + i * n
                    for (tk in range64(tc * tok_per, t1)) {
                        myp[tk * d + i] = dot(wrow, xp + tk * n, n)
                    }
                }
            }
        }
    }
}

// ===== float-plane batch override (the "+AMX"/accelerate tier) =====
// fp32/bf16 BATCH slots only (quant stays NEON — crossover verdict in the accelerate driver
// header). false = fall through to the portable body; off by default (raw tier bit-unchanged).
typedef FpBatchOverrideFn = function<(var yp : float?; wp : float const?; xp : float const?; n : int64; d : int64; ntok : int64) : bool>
typedef Bf16BatchOverrideFn = function<(var yp : float?; wp : uint16 const?; xp : float const?; n : int64; d : int64; ntok : int64) : bool>

[unused_argument(yp, wp, xp, n, d, ntok)]
def private fp_batch_override_unset(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) : bool => false

[unused_argument(yp, wp, xp, n, d, ntok)]
def private bf16_batch_override_unset(var yp : float?; wp : uint16 const?; xp : float const?; n, d, ntok : int64) : bool => false

var private g_mm_fp_batch_override : FpBatchOverrideFn = @@fp_batch_override_unset
var private g_mm_bf16_batch_override : Bf16BatchOverrideFn = @@bf16_batch_override_unset
var private g_fp_override_active = false
var private g_fp_override_min_ntok = 32l

//! Install the float-plane batch override (the accelerate driver). min_ntok gates the donor to
//! batch shapes; 1 arms it for decode GEMV too (the M4/M5-SME applicability probe lane).
def set_float_batch_override(fp : FpBatchOverrideFn; bf16 : Bf16BatchOverrideFn; min_ntok : int64) {
    g_mm_fp_batch_override = fp
    g_mm_bf16_batch_override = bf16
    g_fp_override_min_ntok = min_ntok
    g_fp_override_active = true
}

def clear_float_batch_override() {
    g_mm_fp_batch_override = @@fp_batch_override_unset
    g_mm_bf16_batch_override = @@bf16_batch_override_unset
    g_fp_override_active = false
}

def float_batch_override_active() : bool => g_fp_override_active

// the one body: the float-override seam then the core. Both weight forms below reach it with a
// base pointer, so a borrowed plane and an owned blob take the identical route.
def private matmul_batch_at(var y : array<float>; wp : float const?; x : array<float>; n, d, ntok : int64) {
    unsafe {
        var yp = addr(y[0])
        let xp = addr<float const?>(x[0])
        if (g_fp_override_active && ntok >= g_fp_override_min_ntok && invoke(g_mm_fp_batch_override, yp, wp, xp, n, d, ntok)) {
            return
        }
        matmul_batch_core(yp, wp, xp, n, d, ntok)
    }
}

//! Blob+offset form of matmul_batch (see the matmul woff overload).
def matmul_batch(var y : array<float>; w : array<float>; woff : int64; x : array<float>; n, d, ntok : int64) {
    matmul_batch_at(y, unsafe(addr<float const?>(w[woff])), x, n, d, ntok)
}

//! Borrowed-plane form: the weights live in a prepared image, indexed from `woff`.
def matmul_batch(var y : array<float>; w : PlaneF; woff : int64; x : array<float>; n, d, ntok : int64) {
    matmul_batch_at(y, plane_at(w, woff), x, n, d, ntok)
}

//! Both sides borrowed — a baked table (gemma4a's positional embedding) driving a baked weight.
def matmul_batch(var y : array<float>; w : PlaneF; woff : int64; x : PlaneF; n, d, ntok : int64) {
    unsafe {
        var yp = addr(y[0])
        let wp = plane_at(w, woff)
        let xp = plane_at(x, 0l)
        if (g_fp_override_active && ntok >= g_fp_override_min_ntok && invoke(g_mm_fp_batch_override, yp, wp, xp, n, d, ntok)) {
            return
        }
        matmul_batch_core(yp, wp, xp, n, d, ntok)
    }
}

//! Batched matvec against a native-BF16 weight [d x n] at halfword offset woff: Y [ntok x d]
//! token-major. Bit-for-bit matmul_batch's fp32 expand at half the weight read (dot_bf16's widen
//! is exact). Decode (ntok == 1) takes the GEMV chunk shaper.
def matmul_bf16_batch(var y : array<float>; w : array<uint16>; woff : int64; x : array<float>; n, d, ntok : int64) {
    unsafe {
        var yp = addr(y[0])
        let wp = addr<uint16 const?>(w[woff])
        let xp = addr<float const?>(x[0])
        if (g_fp_override_active && ntok >= g_fp_override_min_ntok && invoke(g_mm_bf16_batch_override, yp, wp, xp, n, d, ntok)) {
            return
        }
        let nch = ntok == 1l ? matmul_chunks_gemv(int(d), 1, n * d) : matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)
        maybe_parallel_for(0, int(d), nch) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let wrow = wp + int64(i) * n
                    for (tk in range64(ntok)) {
                        yp[tk * d + int64(i)] = dot_bf16(xp + tk * n, wrow, n)
                    }
                }
            }
        }
    }
}

// ===== fp32 micro-GEMM: C[M x N] += A[M x K] * B[K x N] (B's N contiguous = SIMD dim) =====
// Broadcast-A FMA register-tile form (zero horizontal reduce) that makes flash attention's QKᵀ
// and P·V fast; single-thread, pointer-based (fork-callable). Tolerance-equal to scalar (FMA reorders low bits).

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
// Template public (not private): the tuner harness's cross-module [dasllama_grid] scan must see it.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def template gemm_f32_uk_4x16_template(var cp : float ?; ap, bp : float const ?; i0, j0, K, N : int64) : void {
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
        for [tune = 1] (kk in range64(K)) {             // fallback u2: ~3% over the rolled loop; same accumulation order
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

[hint(noalias = cp, noalias = ap, noalias = bp), tuned(fallback = "u2")]
def private gemm_f32_uk_4x16(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {}

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

//! gemm_f32 with the tile walk COLUMN-BLOCKS OUTER (jj outer, ii inner): the K×16 B strip stays
//! L1-hot across row blocks, so big-M streams B once instead of M/4 times — numerically identical
//! to gemm_f32. Wins on big-B multi-thread shapes (parakeet attn −20%); small/reused B keeps gemm_f32.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def gemm_f32_jo(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let m4 = M - M % 4l
    let n16 = N - N % 16l
    var jj = 0l
    while (jj < n16) {
        var ii = 0l
        while (ii < m4) {
            gemm_f32_uk_4x16(cp, ap, bp, ii, jj, K, N)
            ii += 4l
        }
        jj += 16l
    }
    if (n16 < N) {                                   // right strip: rows [0,m4) × cols [n16,N)
        gemm_f32_region(cp, ap, bp, 0l, m4, n16, N, K, N)
    }
    if (m4 < M) {                                    // bottom strip: rows [m4,M) × all cols
        gemm_f32_region(cp, ap, bp, m4, M, 0l, N, K, N)
    }
}

//! Q8_0 dequant-in-dot: `wq` int8 quants + fp32 scale `ws` per 32-block, `x` fp32. float4
//! accumulator carried across blocks (scale applied per block as broadcast fma), single
//! horizontal reduce at the end — won the kernel bench (see matmul_variants). n % 32 == 0.
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
                let x4 = *addr<float4?>(x[off])
                bacc += q4 * x4
            }
            c += 4l
        }
        acc += float4(s, s, s, s) * bacc
    }
    return acc.x + acc.y + acc.z + acc.w
}

//! Q8_0 matmul: y = W·x, W as int8 quants `wq` [d x n] + per-block scales `ws` [d x n/32]. Large
//! matmuls thread output rows at 2x worker count (Q8 is compute-bound, threads better than fp32).
//! n % 32 == 0. Blob+offset: woff in elements (scales at woff/32).
def matmul_q8(var y : array<float>; wq : array<int8>; ws : array<float>; woff : int64; x : array<float>; n, d : int64) {
    let nb = n / 32l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<int8 const?>(wq[woff])
        let sp = addr<float const?>(ws[woff / 32l])
        let xp = addr(x[0])
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), g_q8_chunks_per_job, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q8(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

def matmul_q8(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; x : array<float>; n, d : int64) {
    let nb = n / 32l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<int8 const?>(wq[0])
        let sp = addr<float const?>(ws[0])
        let xp = addr(x[0])
        // default 2 chunks per hw job (knob): Q8 dequant-in-dot is compute-bound and threads better than fp32
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), g_q8_chunks_per_job, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q8(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

// ===== Token-block width (shared, ISA-agnostic) =====
// A token-blocked batch kernel (arm64 laneq, x64 grp4) tiles tokens into TB-sized blocks so a
// TB×n activation slice stays L2-resident. Tuned default 128 (Llama-3.2-1B, M1 Max); clamped >= 1.
var g_q8_token_block = 128l
def public set_q8_token_block(tb : int64) { g_q8_token_block = max(tb, 1l) }
def public get_q8_token_block() : int64 { return g_q8_token_block }

// ===== Token-block L2 budget (shared, ISA-agnostic) =====
// A token-blocked Q8·Q8 GEMM keeps TB×n L2-resident; past L2 the slice spills and prefill falls
// off a cliff (M1 Max: knee at TB ≈ L2/n). TB clamped to this budget; default 4MB is PROVISIONAL.
var g_q8_l2_budget = 4l * 1024l * 1024l
def public set_q8_l2_budget(bytes : int64) { g_q8_l2_budget = max(bytes, 1l) }
def public get_q8_l2_budget() : int64 { return g_q8_l2_budget }

//! Clamp a requested token-block width so the TB×n int8 activation slice stays within the per-box L2
//! budget. n = contraction dim (one int8 per activation element). Returns >= 1.
def public effective_token_block(tb, n : int64) : int64 {
    return clamp(tb, 1l, max(1l, g_q8_l2_budget / max(n, 1l)))   // floor 1 (a 0 block width infinite-loops the kernel)
}

// ===== Q8·Q8 matmul: pluggable kernel-backend registry =====
// A *backend* (KernelBackend) is a named kernel-set + weight repack, registered per ISA at [init];
// row-major backends auto-activate, repack backends (arm64 laneq) activate only at load (pin_kernel_backend forces one for A/B).
typedef MatmulQ8Q8Fn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
typedef MatmulQ8Q8BatchFn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64; ntok : int64) : void>
typedef MatmulQ8Q8Group3Fn = function<(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0 : int64; woff1 : int64; woff2 : int64; xqp : int8 const?; xsp : float const?; n : int64; d0 : int64; d1 : int64; d2 : int64) : void>
// In-place repack of one Q8 weight region [d rows x n cols] (raw pointers at the region start) into
// the active backend's preferred layout. Default = no-op (row-major). n % 32 == 0.
typedef RepackQ8Q8Fn = function<(var wp : int8?; var sp : float?; n : int64; d : int64) : void>
// MXFP4·Q8 GEMV: W kept in its native 4-bit form as two planes — `wn` packed nibbles (16 bytes per
// 32-block, row stride n/2) + `we` RAW E8M0 scale bytes (1 per block, row stride n/32) — against a
// pre-quantized Q8 activation. Native half-traffic path for MoE expert stacks (gpt-oss decode).
typedef MatmulMx4Q8Fn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
// Batched MXFP4·Q8 GEMM — the batch analog of MatmulMx4Q8Fn (Y token-major [ntok x d], activations
// pre-quantized per token like MatmulQ8Q8BatchFn). Grouped MoE prefill dispatches here INSTEAD of
// expanding the expert to Q8 scratch when the backend sets mx4_native_batch.
typedef MatmulMx4Q8BatchFn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64; ntok : int64) : void>
// In-place repack of one MXFP4 region [d rows x n cols] (raw plane pointers at the region start)
// into the active backend's preferred layout. Default = no-op (row-major). n % 32 == 0.
typedef RepackMx4Fn = function<(var np : uint8?; var ep : uint8?; n : int64; d : int64) : void>
// In-place repack of one q51 region [d rows x n cols] (quant + scale plane pointers at the
// region start) into the active backend's grp<mr> layout. Default = no-op (disk order).
typedef RepackQ51Fn = function<(var qp : uint8?; var sp : uint8?; n : int64; d : int64) : void>
// Region-list GEMV, ONE dispatch for N same-shape [d x n] projections (MoE decode fuse: all k
// routed experts' gates/ups/downs in one fork/join). `offs` packs PAIRS [weight offset, activation
// offset] per region; region r -> yp+r*d. bp folds a per-region bias at the store (~36us/layer saved).
typedef MatmulQ8Q8GroupNFn = function<(var yp : float?; wp : int8 const?; sp : float const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n : int64; d : int64) : void>
// Row-range GEMV core: compute y rows [rb,re) of ONE [d x n] region with NO internal dispatch —
// the caller owns threading, letting a fused multi-stage decode chain serve matmul rows from
// inside its own stage lambdas. rb/re must be multiples of 32; bit-identical to the mm slot.
typedef MatmulQ8Q8RowsFn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n : int64; rb : int64; re : int64) : void>
typedef MatmulMx4GroupNFn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n : int64; d : int64) : void>
// Row-range MXFP4·Q8 GEMV core — the mx4 twin of MatmulQ8Q8RowsFn (wn/we are the REGION's
// nibble/exponent plane bases; same rb/re-multiples-of-32, no-internal-dispatch contract).
typedef MatmulMx4RowsFn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n : int64; rb : int64; re : int64) : void>
// f16-weight-scale twins (the wscale_f16 rail): identical contracts to the Fn above them, except
// the per-32-block WEIGHT scale plane is raw binary16 halfwords (Model.qscales16) instead of f32 —
// llama.cpp byte parity, 34 vs 36 B per 32 weights. Activation scales (xsp) stay f32 everywhere.
typedef MatmulQ8Q8FnS16 = function<(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
typedef MatmulQ8Q8BatchFnS16 = function<(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64; ntok : int64) : void>
typedef MatmulQ8Q8Group3FnS16 = function<(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : uint16 const?; woff0 : int64; woff1 : int64; woff2 : int64; xqp : int8 const?; xsp : float const?; n : int64; d0 : int64; d1 : int64; d2 : int64) : void>
typedef MatmulQ8Q8GroupNFnS16 = function<(var yp : float?; wp : int8 const?; sp : uint16 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n : int64; d : int64) : void>
typedef MatmulQ8Q8RowsFnS16 = function<(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n : int64; rb : int64; re : int64) : void>
// Is this backend selectable RIGHT NOW? Evaluated at (load-)select/pin time — NOT at registration
// ([init]s run before the JIT installs generated code, so a predicate reading a JIT-stamped value
// answers with its reference tier there). Lets a module register several runtime-dependent flavors.
typedef BackendAvailableFn = function<() : bool>
// Region-list batched MXFP4·Q8 GEMM — batch analog of MatmulMx4GroupNFn, llama.cpp's mul_mat_id
// shape: ONE dispatch runs every routed expert's [d x n] GEMM over its own row range of a shared
// token-row image. `offs` packs TRIPLES [weight offset, row0, cnt]; bit-exact vs a per-region loop.
typedef MatmulMx4Q8BatchGroupNFn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n : int64; d : int64) : void>
// q51 (native Q5_1) kernels — mx4's shape with per-32-BLOCK planes: wq = 20B/block nibbles+qh
// (row stride (n/32)*20), ws = 4B/block f16 d+m (row stride (n/32)*4), Q8_0-form activations.
// No bias forms (no biased-q51 arch); offs conventions match the mx4/q8 groupn twins.
typedef MatmulQ51Q8BatchFn = function<(var yp : float?; wq : uint8 const?; ws : uint8 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64; ntok : int64) : void>
typedef MatmulQ51GroupNFn = function<(var yp : float?; wq : uint8 const?; ws : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
typedef MatmulQ51Q8BatchGroupNFn = function<(var yp : float?; wq : uint8 const?; ws : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
// Native K-quant rows-range GEMV core (kq stage 4): rows [rb, re) of one plane-region pair
// (kqp/ksp = the format's quant/scale plane bases). One typedef per format; a backend's cores
// read ITS repacked kq layout — mixing backends across a load is as illegal as for the Q8 plane.
typedef MatmulKqRowsFn = function<(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; rb : int64; re : int64) : void>
// The fmt-branched full K-quant GEMV (fmt 4/5/6; internal fork over d rows) and plane repack.
typedef MatmulKqFn = function<(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64) : void>
typedef RepackKqFn = function<(fmt : int; var kq : uint8?; var ks : uint8?; n : int64; d : int64) : void>
// Batched K-quant GEMM (the kq prefill tile): activations an [ntok x n] image with per-token
// scale (n/32) and bsum (n/16) rows; Y token-major [ntok x d]. Bit-for-bit ntok x the fmt's
// per-position GEMV over the same repacked planes.
typedef MatmulKqBatchFn = function<(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64; ntok : int64) : void>
// Batched region-list K-quant GEMM (kq batch groupn — fused MoE prefill, llama.cpp's mul_mat_id
// shape): `offs` packs TRIPLES [weight offset, row0, cnt]; region r consumes/produces activation
// rows row0..row0+cnt (token-major). Plane bases pass whole; no bias form (biased arches are mx4).
typedef MatmulKqBatchGroupNFn = function<(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64) : void>
// Region-list K-quant GEMV (kq groupn — MoE routed-expert decode dispatch): `offs` packs
// (weight, activation) element-offset PAIRS, region r's rows land at y[r*d .. r*d+d); kqp/ksp are
// plane bases (each region computes its own superblock offset). No bias form: gpt-oss carries mx4, never kq.
typedef MatmulKqGroupNFn = function<(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64) : void>

//! One kernel backend: the three Q8·Q8 matmul kernels + the weight repack, plus selection metadata.
//! needs_repack = kernels read an interleaved layout, so the loader repacks first (mm_mx4/repack_mx4
//! follow the same flag). priority: higher wins auto-select (portable 0; arm64 sdot 10/laneq 20).
struct KernelBackend {
    name : string
    mm : MatmulQ8Q8Fn
    batch : MatmulQ8Q8BatchFn
    group3 : MatmulQ8Q8Group3Fn
    repack : RepackQ8Q8Fn
    mm_mx4 : MatmulMx4Q8Fn = @@mx4q8_unset_kernel
    // The mx4 BATCH kernels are optional (unlike mm_mx4): only dispatched when mx4_native_batch is
    // set, so backends without them keep the panic stubs (a row-major fallback over an interleaved
    // plane would read garbage, not just run slow) and use the expansion path instead.
    mx4_batch : MatmulMx4Q8BatchFn = @@mx4q8_unset_batch
    mx4_batch_groupn : MatmulMx4Q8BatchGroupNFn = @@mx4q8_unset_batch_groupn
    repack_mx4 : RepackMx4Fn = @@repack_mx4_identity
    groupn : MatmulQ8Q8GroupNFn = @@q8q8_unset_groupn
    groupn_mx4 : MatmulMx4GroupNFn = @@mx4q8_unset_groupn
    // q51 slots DEFAULT to the portable kernels (not panic stubs): the q51 planes are never
    // repacked, so the portable disk-order forms are correct on every backend — a native/tuned
    // backend overrides them explicitly.
    q51_batch : MatmulQ51Q8BatchFn = @@q51q8_batch_kernel
    groupn_q51 : MatmulQ51GroupNFn = @@q51q8_groupn_kernel
    q51_batch_groupn : MatmulQ51Q8BatchGroupNFn = @@q51q8_batch_groupn_kernel
    repack_q51 : RepackQ51Fn = @@repack_q51_identity
    q51_layout : function<() : int> = @@q51_layout_grp1
    // Optional row-range GEMV core (see MatmulQ8Q8RowsFn) — backends without one keep the panic
    // stub, and the fused multi-stage decode falls back to the per-op dispatch path.
    mm_rows : MatmulQ8Q8RowsFn = @@q8q8_unset_rows
    // Optional mx4 row-range GEMV core (see MatmulMx4RowsFn) — the fused MoE expert chain gates
    // on it the same way the dense chains gate on mm_rows.
    mx4_rows : MatmulMx4RowsFn = @@mx4q8_unset_rows
    // Optional f16-scale twins (the wscale_f16 rail). All-or-nothing per backend: the loader's
    // plane convert gates on kernel_backend_has_wscale16(), keyed off mm_s16 alone.
    mm_s16 : MatmulQ8Q8FnS16 = @@q8q8_unset_kernel_s16
    batch_s16 : MatmulQ8Q8BatchFnS16 = @@q8q8_unset_batch_s16
    group3_s16 : MatmulQ8Q8Group3FnS16 = @@q8q8_unset_group3_s16
    groupn_s16 : MatmulQ8Q8GroupNFnS16 = @@q8q8_unset_groupn_s16
    mm_rows_s16 : MatmulQ8Q8RowsFnS16 = @@q8q8_unset_rows_s16
    // Optional native K-quant slots (kq stage 4), all-or-nothing per backend, keyed off mm_kq
    // (kernel_backend_has_kq): once the loader repacks kq planes via repack_kq, EVERY kq matmul
    // must run these cores — the portable disk-order kernels would read garbage off the interleaved planes.
    mm_kq : MatmulKqFn = @@kq_unset_kernel
    kq_rows_k4 : MatmulKqRowsFn = @@kq_unset_rows
    kq_rows_k5 : MatmulKqRowsFn = @@kq_unset_rows
    kq_rows_k6 : MatmulKqRowsFn = @@kq_unset_rows
    kq_rows_q40 : MatmulKqRowsFn = @@kq_unset_rows
    kq_batch : MatmulKqBatchFn = @@kq_unset_batch
    kq_groupn : MatmulKqGroupNFn = @@kq_unset_groupn
    kq_batch_groupn : MatmulKqBatchGroupNFn = @@kq_unset_batch_groupn
    repack_kq : RepackKqFn = @@repack_kq_identity
    needs_repack : bool
    // mx4_expand_interleaved: true when repack_mx4/repack Q8 layouts satisfy the byte-for-byte
    // interleave map (expand_mx4_region_q8's direct branch) — nibble byte `off` of group bg maps
    // positionally onto Q8 bytes. False on a row-major mx4 backend: prefill expands + repacks via `repack`.
    mx4_expand_interleaved : bool = false
    // mx4_native_batch: the backend's mx4_batch beats expand-to-Q8 + Q8 batch (x64: the expansion's
    // 3·d·n Q8 scratch round-trip through DRAM was 87% of gpt-oss prefill wall), so grouped MoE
    // prefill dispatches matmul_mx4q8_batch instead. Slot may be set without the flag (probe/A-B ref).
    mx4_native_batch : bool = false
    priority : int
    // see BackendAvailableFn — selection-time availability; default = always selectable
    available : BackendAvailableFn = @@backend_always_available
    // The Q8 interleave (mr) the backend's repack writes and its kernels read. Evaluated at
    // ACTIVATION time, never [init] (a JIT-stamped layout still answers with its reference tier then).
    // The mx4 expand's positional map and probes read the cached value via active_q8_layout_mr().
    q8_layout : function<() : int> = @@q8_layout_grp4
    // per-FORMAT kq plane interleave (kq v2: the k4/k5/k6 families tune separately, so their
    // grp layouts can differ from the q8 family's and from each other)
    kq_layout : function<(fmt : int) : int> = @@kq_layout_grp4_fmt
    // The additive bias the backend's repack bakes into GROUP-row weight bytes (128 for a
    // bias128 gen stamp, 0 everywhere else). Same activation-time contract as q8_layout;
    // the mx4->Q8 expand reads the cached value via active_q8_wbias().
    q8_wbias : function<() : int> = @@q8_wbias_zero
    // K-bytes per row per interleave group of the backend's Q8 plane (4 everywhere; 8 on an
    // smmla gen stamp — the row-pair × 8-k layout). Same activation-time contract; the mx4->Q8
    // expand map and the token-tail generic read the cached value via active_q8_kgroup().
    q8_kgroup : function<() : int> = @@q8_kgroup_four
}

// Active kernels — the hot path reads these directly. A backend must register before any Q8·Q8 matmul
// runs (require dasllama/dasllama_math_default for the portable fallback); until then the stubs panic.
var g_mm_q8q8 = @@q8q8_unset_kernel
var g_mm_q8q8_batch = @@q8q8_unset_batch
var g_mm_q8q8_group3 = @@q8q8_unset_group3
var g_mm_mx4q8 = @@mx4q8_unset_kernel
var g_mm_mx4q8_batch = @@mx4q8_unset_batch
var g_mm_mx4q8_batch_groupn = @@mx4q8_unset_batch_groupn
var g_mm_q8q8_groupn = @@q8q8_unset_groupn
var g_mm_mx4q8_groupn = @@mx4q8_unset_groupn
var g_mm_q51q8_batch = @@q51q8_batch_kernel
var g_mm_q51q8_groupn = @@q51q8_groupn_kernel
var g_mm_q51q8_batch_groupn = @@q51q8_batch_groupn_kernel
var g_mm_q8q8_rows = @@q8q8_unset_rows
var g_active_has_rows = false       // does the active backend carry a row-range GEMV core?
var g_mm_mx4q8_rows = @@mx4q8_unset_rows
var g_active_has_mx4_rows = false   // ... and the mx4 row-range core (the fused MoE chain's gate)?
var g_mm_q8q8_s16 = @@q8q8_unset_kernel_s16
var g_mm_q8q8_batch_s16 = @@q8q8_unset_batch_s16
var g_mm_q8q8_group3_s16 = @@q8q8_unset_group3_s16
var g_mm_q8q8_groupn_s16 = @@q8q8_unset_groupn_s16
var g_mm_q8q8_rows_s16 = @@q8q8_unset_rows_s16
var g_active_has_wscale16 = false   // does the active backend carry the f16-scale twins?
var g_active_has_rows16 = false     // ... including the f16-scale row-range GEMV core?
var g_mm_kq = @@kq_unset_kernel
var g_kq_rows_k4 = @@kq_unset_rows
var g_kq_rows_k5 = @@kq_unset_rows
var g_kq_rows_k6 = @@kq_unset_rows
var g_kq_rows_q40 = @@kq_unset_rows
var g_mm_kq_batch = @@kq_unset_batch
var g_mm_kq_groupn = @@kq_unset_groupn
var g_mm_kq_batch_groupn = @@kq_unset_batch_groupn
var g_active_has_kq = false         // does the active backend carry the native K-quant slots?
var g_active_has_kq_batch = false   // ... including the batched kq GEMM (the prefill tile)?
var g_active_has_kq_groupn = false  // ... and the region-list kq GEMV (MoE expert dispatch)?
var g_active_has_kq_batch_groupn = false   // ... and the fused batched region-list GEMM (MoE prefill)?
var g_repack_q8q8 = @@repack_q8q8_identity   // no-op until a repack backend is selected
var g_repack_mx4 = @@repack_mx4_identity     // no-op until a repack backend is selected
var g_repack_q51 = @@repack_q51_identity     // no-op until a repack backend is selected
var g_repack_kq = @@repack_kq_identity       // no-op until a kq-capable backend is selected

var g_kernel_backends : table<string; KernelBackend>
var g_active_backend = ""           // name of the active backend ("" = the unset stub)
var g_active_priority = -1          // priority of the active row-major default (auto-activate compare)
var g_active_needs_repack = false   // does the active backend need repacked weights?
var g_active_mx4_expand_interleaved = false   // see KernelBackend.mx4_expand_interleaved
var g_active_mx4_native_batch = false         // see KernelBackend.mx4_native_batch
var g_active_q8_layout_mr = 4l                // see KernelBackend.q8_layout (activation-time cache)
var g_active_kq_layout = @@kq_layout_grp4_fmt // see KernelBackend.kq_layout (activation-time cache)
var g_active_q51_layout_mr = 1l               // see KernelBackend.q51_layout (activation-time cache)
var g_active_q8_wbias = 0l                    // see KernelBackend.q8_wbias (activation-time cache)
var g_active_q8_kgroup = 4l                   // see KernelBackend.q8_kgroup (activation-time cache)
var g_backend_pin = ""              // A/B: force this backend by name; "" = auto-select
var g_batch_pin = ""                // hybrid: donor backend for the BATCH-shaped slots; "" = whole-backend
var g_active_batch_backend = ""     // == g_active_backend unless a batch pin overrode the batch slots

//! Default repack: no-op — weights stay row-major for the portable / sdot4x4 kernels.
[unused_argument(wp, sp, n, d)]
def repack_q8q8_identity(var wp : int8?; var sp : float?; n, d : int64) {}

//! Default MXFP4 repack: no-op — the planes stay row-major for the portable / row-major kernels.
[unused_argument(np, ep, n, d)]
def repack_mx4_identity(var np : uint8?; var ep : uint8?; n, d : int64) {}

//! Default K-quant repack: no-op — the kq planes stay disk-order for the portable kernels.
[unused_argument(fmt, kq, ks, n, d)]
def repack_kq_identity(fmt : int; var kq : uint8?; var ks : uint8?; n, d : int64) {}

//! Default q51 repack: no-op — the q51 planes stay disk-order for the portable kernels.
[unused_argument(qp, sp, n, d)]
def repack_q51_identity(var qp : uint8?; var sp : uint8?; n, d : int64) {}

//! Q5_1·Q8 dot, scalar reference form — THE q51 float-order contract every q51 kernel
//! reproduces: per 32-block isum = Σ q·a (q = nibble | qh_bit << 4, unsigned [0,31]), asum = Σ a
//! computed INLINE (Q8_0-form activations carry no block sums); f += (d·isum + m·asum)·xs[b].
[hint(unsafe_range_check, noalias = wq, noalias = ws, noalias = xq, noalias = xs)]
def dot_q51q8_scalar(wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var f = 0.0
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * 20l
            let xb = bi * 32l
            let qh = uint(wq[wb + 16l]) | (uint(wq[wb + 17l]) << 8u) | (uint(wq[wb + 18l]) << 16u) | (uint(wq[wb + 19l]) << 24u)
            var isum = 0
            var asum = 0
            for (j in range64(16l)) {
                let b = int(wq[wb + j])
                let h0 = (int(qh >> uint(j)) & 1) << 4
                let h1 = (int(qh >> uint(j + 16l)) & 1) << 4
                let a0 = int(xq[xb + j])
                let a1 = int(xq[xb + j + 16l])
                isum += ((b & 15) | h0) * a0 + ((b >> 4) | h1) * a1
                asum += a0 + a1
            }
            let d = f16_to_f32(uint(ws[bi * 4l]) | (uint(ws[bi * 4l + 1l]) << 8u))
            let m = f16_to_f32(uint(ws[bi * 4l + 2l]) | (uint(ws[bi * 4l + 3l]) << 8u))
            f += (d * float(isum) + m * float(asum)) * xs[bi]
        }
    }
    return f
}

//! Expand one q51 plane row's 5-bit quants to full bytes in `qe` (n bytes): nibbles via u64
//! masks + the qh bits ORd in at 0x10 via the u64 bit-spread. Exact. The fast kernels MAC over
//! the expanded row — a MAC over just-stored STACK bytes defeats the vectorizer (1.95 vs 8.1 GB/s).
[hint(unsafe_range_check, noalias = wq, noalias = qe)]
def expand_q51_row(wq : uint8 const?; var qe : int8?; nb : int64) {
    unsafe {
        var qp64 = reinterpret<uint64?>(qe)
        for (bi in range64(nb)) {
            let wb = bi * 20l
            let wp64 = reinterpret<uint64 const?>(wq + wb)
            let w0 = wp64[0]
            let w1 = wp64[1]
            let s0 = ((((0x0101010101010101ul * uint64(wq[wb + 16l])) & 0x8040201008040201ul) + 0x7f7f7f7f7f7f7f7ful) >> 3ul) & 0x1010101010101010ul
            let s1 = ((((0x0101010101010101ul * uint64(wq[wb + 17l])) & 0x8040201008040201ul) + 0x7f7f7f7f7f7f7f7ful) >> 3ul) & 0x1010101010101010ul
            let s2 = ((((0x0101010101010101ul * uint64(wq[wb + 18l])) & 0x8040201008040201ul) + 0x7f7f7f7f7f7f7f7ful) >> 3ul) & 0x1010101010101010ul
            let s3 = ((((0x0101010101010101ul * uint64(wq[wb + 19l])) & 0x8040201008040201ul) + 0x7f7f7f7f7f7f7f7ful) >> 3ul) & 0x1010101010101010ul
            qp64[bi * 4l] = (w0 & 0x0f0f0f0f0f0f0f0ful) | s0
            qp64[bi * 4l + 1l] = (w1 & 0x0f0f0f0f0f0f0f0ful) | s1
            qp64[bi * 4l + 2l] = ((w0 >> 4ul) & 0x0f0f0f0f0f0f0f0ful) | s2
            qp64[bi * 4l + 3l] = ((w1 >> 4ul) & 0x0f0f0f0f0f0f0f0ful) | s3
        }
    }
}

[hint(unsafe_range_check, noalias = qe, noalias = ws, noalias = xq, noalias = xs)]
def template dot_q51e_template(qe : int8 const ?; ws : uint8 const ?; xq : int8 const ?; xs : float const ?; n : int64) : float {
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

//! The q51 MAC over an expand_q51_row-expanded row — the fast kernels' core. Bit-identical to
//! dot_q51q8_scalar (exact int sums, same per-block fold order); vectorizes like dot_q8q8.
[hint(unsafe_range_check, noalias = qe, noalias = ws, noalias = xq, noalias = xs), tuned(fallback = "vec16")]
def dot_q51e(qe : int8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?; n : int64) : float {}

//! expand_q51_row's grp<mr> twin: row r of one mr-row group off the interleaved plane (nibble
//! dwords at j·4·mr + r·4, qh at the 16·mr tail section). Bit-identical expansion.
def expand_q51_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64) {
    unsafe {
        for (bi in range64(nb)) {
            let base = bi * 20l * mr
            let hb = base + 16l * mr + r * 4l
            let qh = uint(gq[hb]) | (uint(gq[hb + 1l]) << 8u) | (uint(gq[hb + 2l]) << 16u) | (uint(gq[hb + 3l]) << 24u)
            for (j in range64(16l)) {
                let b = int(gq[base + (j / 4l) * 4l * mr + r * 4l + j % 4l])
                let h0 = (int(qh >> uint(j)) & 1) << 4
                let h1 = (int(qh >> uint(j + 16l)) & 1) << 4
                qe[bi * 32l + j] = int8((b & 15) | h0)
                qe[bi * 32l + j + 16l] = int8((b >> 4) | h1)
            }
        }
    }
}

//! One row's scale bytes gathered contiguous off the grp<mr> scale plane (dot_q51e reads a
//! packed 4B-per-block row).
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

// Portable q51 batch (the KernelBackend.q51_batch default): rows-outer/tokens-inner — each row
// expands ONCE into the worker's scratch, then ntok MAC passes (the mm_b_q51_pre core).
def q51q8_batch_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    let mr = active_q51_layout_mr()   // grp<mr> planes on the gen backend; tail rows stay disk-order
    let grpRows = (d / mr) * mr
    var myp = yp
    unsafe {
        maybe_parallel_for(0, int(d), matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)) $(rb, re) {
            var qe : array<int8>
            qe |> resize(n)
            var sc : array<uint8>
            sc |> resize(nb * 4l)
            unsafe {
                var qep = addr(qe[0])
                var scp = addr(sc[0])
                for (i in range(rb, re)) {
                    let row = int64(i)
                    var srow = ws + row * nb * 4l
                    if (mr == 1l || row >= grpRows) {
                        expand_q51_row(wq + row * nb * 20l, qep, nb)
                    } else {
                        let gbase = (row / mr) * mr * nb
                        expand_q51_grp_row(wq + gbase * 20l, row % mr, mr, qep, nb)
                        gather_q51_grp_scales(ws + gbase * 4l, row % mr, mr, scp, nb)
                        srow = scp
                    }
                    for (tk in range64(ntok)) {
                        myp[tk * d + row] = dot_q51e(qep, srow, xqp + tk * n, xsp + tk * nb, n)
                    }
                }
            }
            delete qe
            delete sc
        }
    }
}

// Portable q51 groupN (the groupn_q51 default — MoE decode dispatch): (element woff, activation
// xoff) pairs, expand-then-MAC per (region, row). No bias form — no biased-q51 arch exists.
def q51q8_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    let mr = active_q51_layout_mr()   // grp<mr> planes on the gen backend; tail rows stay disk-order
    let grpRows = (d / mr) * mr
    var myp = yp
    unsafe {
        maybe_parallel_for(0, int(nregions * d), matmul_chunks_gemv(int(nregions * d), 1, n * d * nregions)) $(rb, re) {
            var qe : array<int8>
            qe |> resize(n)
            var sc : array<uint8>
            sc |> resize(nb * 4l)
            unsafe {
                var qep = addr(qe[0])
                var scp = addr(sc[0])
                for (i in range(rb, re)) {
                    let ii = int64(i)
                    let r = ii / d
                    let row = ii % d
                    let woff = offs[r * 2l]
                    let xoff = offs[r * 2l + 1l]
                    var srow = ws + (woff / 32l + row * nb) * 4l
                    if (mr == 1l || row >= grpRows) {
                        expand_q51_row(wq + (woff / 32l + row * nb) * 20l, qep, nb)
                    } else {
                        let gbase = woff / 32l + (row / mr) * mr * nb
                        expand_q51_grp_row(wq + gbase * 20l, row % mr, mr, qep, nb)
                        gather_q51_grp_scales(ws + gbase * 4l, row % mr, mr, scp, nb)
                        srow = scp
                    }
                    myp[ii] = dot_q51e(qep, srow, xqp + xoff, xsp + xoff / 32l, n)
                }
            }
            delete qe
            delete sc
        }
    }
}

// Portable q51 batch groupN (the q51_batch_groupn default — fused MoE prefill): (element woff,
// row0, cnt) triples; each (region, row) expands once, then cnt token MACs.
def q51q8_batch_groupn_kernel(var yp : float?; wq : uint8 const?; ws : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    let mr = active_q51_layout_mr()   // grp<mr> planes on the gen backend; tail rows stay disk-order
    let grpRows = (d / mr) * mr
    var myp = yp
    unsafe {
        maybe_parallel_for(0, int(nregions * d), matmul_chunks(int(nregions * d), get_q8_batch_chunks_per_job(), n * d * nregions)) $(rb, re) {
            var qe : array<int8>
            qe |> resize(n)
            var sc : array<uint8>
            sc |> resize(nb * 4l)
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
                    var srow = ws + (woff / 32l + row * nb) * 4l
                    if (mr == 1l || row >= grpRows) {
                        expand_q51_row(wq + (woff / 32l + row * nb) * 20l, qep, nb)
                    } else {
                        let gbase = woff / 32l + (row / mr) * mr * nb
                        expand_q51_grp_row(wq + gbase * 20l, row % mr, mr, qep, nb)
                        gather_q51_grp_scales(ws + gbase * 4l, row % mr, mr, scp, nb)
                        srow = scp
                    }
                    for (tk in range64(cnt)) {
                        myp[(row0 + tk) * d + row] = dot_q51e(qep, srow, xqp + (row0 + tk) * n, xsp + (row0 + tk) * nb, n)
                    }
                }
            }
            delete qe
            delete sc
        }
    }
}

def private no_backend() {
    panic("dasLLAMA: no Q8·Q8 kernel backend registered — require dasllama/dasllama_math_default")
}
def private backend_always_available() : bool => true
def private q8_layout_grp4() : int => 4
def private q51_layout_grp1() : int => 1

[unused_argument(fmt)]
def private kq_layout_grp4_fmt(fmt : int) : int => 4
def private q8_wbias_zero() : int => 0
def private q8_kgroup_four() : int => 4

// ===== cross-box bake: repack by NUMBERS =====
// The getters below are the one layout authority; a foreign-backend bake overrides them with the target's numbers — the generic grp<mr> repacks produce the exact bytes, and the local kernels never serve them (the foreign identity cannot load here)
var private g_bake_cpu_override = false
var private g_bake_cpu = DlimCpuConfig()

//! The backend name the dlim identity reports — the bake alias when a cross-box bake is armed.
def dlim_backend_name() : string => g_bake_cpu_override ? g_bake_cpu.backend : active_kernel_backend()

//! Pin the backend a bake targets: a registered name pins normally; a FOREIGN name arms
//! repack-by-numbers (the gen tier's generic grp<mr> primitives produce the target layout,
//! the identity reports the target backend). false = no route — the caller fail-closes.
def pin_backend_for_bake(cpu : DlimCpuConfig) : bool {
    if (key_exists(g_kernel_backends, cpu.backend)) {
        pin_kernel_backend(cpu.backend)
        return active_kernel_backend() == cpu.backend
    }
    select_matmul_backend_for_load()   // the load enters repack stages only under a repack backend
    if (!kernel_backend_needs_repack(active_kernel_backend())) {
        to_log(LOG_WARNING, "dasLLAMA: cross-box bake for '{cpu.backend}' needs the generated repack family — none is active here\n")
        return false
    }
    g_bake_cpu = cpu
    g_bake_cpu_override = true
    return true
}

def clear_bake_backend_override() {
    g_bake_cpu_override = false
}

//! The Q8 interleave (mr) of the ACTIVE backend's weight layout — cached at activation from
//! KernelBackend.q8_layout (4 for every hand tier; the stamped mr on the gen backend).
def active_q8_layout_mr() : int64 => g_bake_cpu_override ? g_bake_cpu.q8_mr : g_active_q8_layout_mr

//! The active backend's kq plane interleave for `fmt` (4/5/6) — per-format since the kq v2
//! family split; the loader freezes these onto the Model at repack time.
def active_kq_layout_mr(fmt : int) : int64 {
    if (g_bake_cpu_override) {
        return fmt == 4 ? g_bake_cpu.kq_mr4 : (fmt == 5 ? g_bake_cpu.kq_mr5 : (fmt == 40 ? g_bake_cpu.kq_mr40 : g_bake_cpu.kq_mr6))
    }
    return int64(invoke(g_active_kq_layout, fmt))
}

//! The active backend's q51 plane interleave — 1 (disk order) on every hand tier; the stamped
//! mr on the arm64-gen backend (the self-anchored q51 GEMV family).
def active_q51_layout_mr() : int64 => g_bake_cpu_override ? g_bake_cpu.q51_mr : g_active_q51_layout_mr

//! The weight-plane bias of the ACTIVE backend's Q8 layout — cached at activation from
//! KernelBackend.q8_wbias (0 for every hand tier; 128 on a bias128 gen stamp). The mx4->Q8
//! expand biases its LUT writes to match.
def active_q8_wbias() : int64 => g_bake_cpu_override ? g_bake_cpu.q8_wbias : g_active_q8_wbias

//! The plane kgroup of the ACTIVE backend's Q8 layout — cached at activation from
//! KernelBackend.q8_kgroup (4 for every hand tier; 8 on an smmla gen stamp). The mx4->Q8
//! expand's positional map and the batch token-tail generic read it.
def active_q8_kgroup() : int64 => g_bake_cpu_override ? g_bake_cpu.q8_kgroup : g_active_q8_kgroup

// the q51 repack for a bake whose target has grp<mr> q51 planes: some LOCAL registrations
// leave repack_q51 unset (x64-gen keeps disk-order q51 slots), so the gen module registers its
// generic q51 repack here for the override path to reach
var private g_bake_q51_repack = @@repack_q51_identity

def set_bake_q51_repack(f : RepackQ51Fn) {
    g_bake_q51_repack = f
}

//! The q51 repack the LOAD path invokes — the gen generic under a cross-box override with a
//! grp<mr> q51 target, else the active backend's slot.
def active_repack_q51() : RepackQ51Fn {
    return g_bake_cpu_override && g_bake_cpu.q51_mr > 1l ? g_bake_q51_repack : g_repack_q51
}

// The bake twins (registered by math_gen like set_bake_q51_repack): override-getter bodies that
// only ever run inline on the converter main thread. The REGISTERED slots stay global-free —
// the eager loader fans them across pool lanes, whose contexts can't read this module's globals.
var private g_bake_q8q8_repack = @@repack_q8q8_identity
var private g_bake_mx4_repack = @@repack_mx4_identity
var private g_bake_kq_repack = @@repack_kq_identity

def set_bake_q8q8_repack(f : RepackQ8Q8Fn) {
    g_bake_q8q8_repack = f
}

def set_bake_mx4_repack(f : RepackMx4Fn) {
    g_bake_mx4_repack = f
}

def set_bake_kq_repack(f : RepackKqFn) {
    g_bake_kq_repack = f
}

//! The repack to HOIST as a value (repack_regions) or call inline (stream_repack_one): the
//! registered slot normally, the bake twin while a cross-box override is armed.
def active_repack_q8q8() : RepackQ8Q8Fn {
    return g_bake_cpu_override ? g_bake_q8q8_repack : g_repack_q8q8
}

def active_repack_mx4() : RepackMx4Fn {
    return g_bake_cpu_override ? g_bake_mx4_repack : g_repack_mx4
}

def active_repack_kq() : RepackKqFn {
    return g_bake_cpu_override ? g_bake_kq_repack : g_repack_kq
}

//! True while a cross-box bake override is armed (converter main thread only — the streaming
//! bake repacks inline, never on lanes).
def bake_cpu_override_active() : bool {
    return g_bake_cpu_override
}
[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private q8q8_unset_kernel(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) { no_backend() }
[unused_argument(yp, wp, sp, xqp, xsp, n, d, ntok)]
def private q8q8_unset_batch(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) { no_backend() }
[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private q8q8_unset_group3(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) { no_backend() }
[unused_argument(yp, wn, we, xqp, xsp, n, d)]
def private mx4q8_unset_kernel(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d : int64) { no_backend() }
[unused_argument(yp, wn, we, xqp, xsp, n, d, ntok)]
def private mx4q8_unset_batch(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    // reachable with a registered backend (slot is optional — see KernelBackend.mx4_batch)
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' provides no MXFP4·Q8 batch kernel (mx4_batch)")
}
[unused_argument(yp, wn, we, offs, nregions, xqp, xsp, bp, boffs, n, d)]
def private mx4q8_unset_batch_groupn(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' provides no MXFP4·Q8 batch groupN kernel (mx4_batch_groupn)")
}
[unused_argument(yp, wp, sp, offs, nregions, xqp, xsp, bp, boffs, n, d)]
def private q8q8_unset_groupn(var yp : float?; wp : int8 const?; sp : float const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) { no_backend() }
[unused_argument(yp, wp, sp, xqp, xsp, n, rb, re)]
def private q8q8_unset_rows(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {
    // reachable with a registered backend: the slot is optional — gate on kernel_backend_has_rows()
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' provides no row-range GEMV core (mm_rows)")
}
[unused_argument(yp, wn, we, offs, nregions, xqp, xsp, bp, boffs, n, d)]
def private mx4q8_unset_groupn(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) { no_backend() }
[unused_argument(yp, wn, we, xqp, xsp, n, rb, re)]
def private mx4q8_unset_rows(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {
    // reachable with a registered backend: the slot is optional — gate on kernel_backend_has_mx4_rows()
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' provides no mx4 row-range GEMV core (mx4_rows)")
}
// The f16-scale stubs are reachable with a registered backend (the twins are optional) — a
// wscale_f16 model must never activate a backend without them; the loader gates the convert.
[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private q8q8_unset_kernel_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale kernels (mm_s16)")
}
[unused_argument(yp, wp, sp, xqp, xsp, n, d, ntok)]
def private q8q8_unset_batch_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale batch kernel (batch_s16)")
}
[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private q8q8_unset_group3_s16(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : uint16 const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale group3 kernel (group3_s16)")
}
[unused_argument(yp, wp, sp, offs, nregions, xqp, xsp, bp, boffs, n, d)]
def private q8q8_unset_groupn_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale groupN kernel (groupn_s16)")
}
[unused_argument(yp, wp, sp, xqp, xsp, n, rb, re)]
def private q8q8_unset_rows_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale row-range GEMV core (mm_rows_s16)")
}
// The kq stubs are reachable with a registered backend (the slots are optional) — dispatch
// gates on kernel_backend_has_kq(); when it is down the disk-order portable kernels serve.
[unused_argument(fmt, yp, kqp, ksp, xqp, xsp, xbsp, n, d)]
def private kq_unset_kernel(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no native K-quant kernels (mm_kq)")
}
[unused_argument(yp, kqp, ksp, xqp, xsp, xbsp, n, rb, re)]
def private kq_unset_rows(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no K-quant row-range GEMV cores (kq_rows_*)")
}
[unused_argument(fmt, yp, kqp, ksp, offs, nregions, xqp, xsp, xbsp, n, d)]
def private kq_unset_groupn(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no K-quant region-list kernel (kq_groupn)")
}
[unused_argument(fmt, yp, kqp, ksp, offs, nregions, xqp, xsp, xbsp, n, d)]
def private kq_unset_batch_groupn(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no K-quant batched region-list kernel (kq_batch_groupn)")
}
[unused_argument(fmt, yp, kqp, ksp, xqp, xsp, xbsp, n, d, ntok)]
def private kq_unset_batch(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, ntok : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no K-quant batch kernel (kq_batch)")
}

def private activate(be : KernelBackend) {
    g_mm_q8q8 = be.mm
    g_mm_q8q8_batch = be.batch
    g_mm_q8q8_group3 = be.group3
    g_mm_mx4q8 = be.mm_mx4
    g_mm_mx4q8_batch = be.mx4_batch
    g_mm_mx4q8_batch_groupn = be.mx4_batch_groupn
    g_mm_q8q8_groupn = be.groupn
    g_mm_mx4q8_groupn = be.groupn_mx4
    g_mm_q51q8_batch = be.q51_batch
    g_mm_q51q8_groupn = be.groupn_q51
    g_mm_q51q8_batch_groupn = be.q51_batch_groupn
    g_mm_q8q8_rows = be.mm_rows
    g_active_has_rows = !(be.mm_rows == @@q8q8_unset_rows)
    g_mm_mx4q8_rows = be.mx4_rows
    g_active_has_mx4_rows = !(be.mx4_rows == @@mx4q8_unset_rows)
    g_mm_q8q8_s16 = be.mm_s16
    g_mm_q8q8_batch_s16 = be.batch_s16
    g_mm_q8q8_group3_s16 = be.group3_s16
    g_mm_q8q8_groupn_s16 = be.groupn_s16
    g_mm_q8q8_rows_s16 = be.mm_rows_s16
    g_active_has_wscale16 = !(be.mm_s16 == @@q8q8_unset_kernel_s16)
    g_active_has_rows16 = !(be.mm_rows_s16 == @@q8q8_unset_rows_s16)
    g_mm_kq = be.mm_kq
    g_kq_rows_k4 = be.kq_rows_k4
    g_kq_rows_k5 = be.kq_rows_k5
    g_kq_rows_k6 = be.kq_rows_k6
    g_kq_rows_q40 = be.kq_rows_q40
    g_mm_kq_batch = be.kq_batch
    g_mm_kq_groupn = be.kq_groupn
    g_mm_kq_batch_groupn = be.kq_batch_groupn
    g_active_has_kq = !(be.mm_kq == @@kq_unset_kernel)
    g_active_has_kq_batch = !(be.kq_batch == @@kq_unset_batch)
    g_active_has_kq_groupn = !(be.kq_groupn == @@kq_unset_groupn)
    g_active_has_kq_batch_groupn = !(be.kq_batch_groupn == @@kq_unset_batch_groupn)
    g_repack_q8q8 = be.repack
    g_repack_mx4 = be.repack_mx4
    g_repack_q51 = be.repack_q51
    g_repack_kq = be.repack_kq
    g_active_backend = be.name
    g_active_needs_repack = be.needs_repack
    g_active_mx4_expand_interleaved = be.mx4_expand_interleaved
    g_active_mx4_native_batch = be.mx4_native_batch
    g_active_q8_layout_mr = int64(invoke(be.q8_layout))
    g_active_kq_layout = be.kq_layout
    g_active_q51_layout_mr = int64(invoke(be.q51_layout))
    g_active_q8_wbias = int64(invoke(be.q8_wbias))
    g_active_q8_kgroup = int64(invoke(be.q8_kgroup))
    g_active_batch_backend = be.name
    // a standing batch pin survives re-activation (auto-select, load-time select, explicit select)
    if (g_batch_pin != "") {
        apply_batch_override(g_batch_pin)
    }
}

// The batch half of a hybrid: overwrites only BATCH-shaped slots (prefill GEMM + mx4 batch) from
// the donor; GEMV-shaped slots stay with the active backend. Donor must read the same weight layout
// (same needs_repack, and for repack backends the same repack fn — laneq/grp4 are mutually unreadable).
def private apply_batch_override(name : string) {
    if (!key_exists(g_kernel_backends, name)) {
        to_log(LOG_WARNING, "dasLLAMA: batch backend '{name}' — no such backend registered; keeping '{g_active_backend}' batch kernels\n")
        return
    }
    let be = g_kernel_backends[name]
    if (be.needs_repack != g_active_needs_repack || (g_active_needs_repack && !(be.repack == g_repack_q8q8))) {
        to_log(LOG_WARNING, "dasLLAMA: batch backend '{name}' reads a different weight layout than active '{g_active_backend}'; keeping '{g_active_backend}' batch kernels\n")
        return
    }
    g_mm_q8q8_batch = be.batch
    g_mm_mx4q8_batch = be.mx4_batch
    g_mm_mx4q8_batch_groupn = be.mx4_batch_groupn
    g_mm_q51q8_batch = be.q51_batch
    g_mm_q51q8_batch_groupn = be.q51_batch_groupn
    g_active_mx4_native_batch = be.mx4_native_batch
    g_active_batch_backend = be.name
}

//! Register a kernel backend under its name. Auto-activates it as the row-major default when it is the
//! highest-priority backend that needs no repack — so direct callers get the best un-repacked kernel with
//! no load step. Repack backends are activated only by select_matmul_backend_for_load (at model load).
def register_kernel_backend(be : KernelBackend) {
    g_kernel_backends[be.name] = be
    if (g_backend_pin == "" && !be.needs_repack && be.priority > g_active_priority) {
        g_active_priority = be.priority
        activate(be)
    }
}

//! Activate a registered backend by name (explicit control / A/B benchmarking). No-op if unknown.
//! Runs the availability witness first — for some backends it doubles as the runtime enable
//! (amx = XTILEDATA arch_prctl); activating past it would SIGILL on the first tile op.
def select_kernel_backend(name : string) {
    if (!key_exists(g_kernel_backends, name)) {
        return
    }
    if (!invoke(g_kernel_backends[name].available)) {
        to_log(LOG_WARNING, "dasLLAMA: select_kernel_backend('{name}') — backend reports itself unavailable in this configuration; keeping '{g_active_backend}'\n")
        return
    }
    activate(g_kernel_backends[name])
}

//! Pin the active backend to `name` (forces it; suppresses auto-select on later registrations). For A/B
//! benchmarking on one binary — e.g. pin "arm64-sdot" to measure the row-major baseline against the gen tier.
def pin_kernel_backend(name : string) {
    // Reject an unknown name rather than sticking an invalid pin: a bad pin would suppress
    // auto-activation AND keep select_matmul_backend_for_load from picking the best (repack) backend,
    // silently degrading to whatever is currently active. Keep the current backend and warn instead.
    if (!key_exists(g_kernel_backends, name)) {
        to_log(LOG_WARNING, "dasLLAMA: pin_kernel_backend('{name}') — no such backend registered; keeping '{g_active_backend}'\n")
        return
    }
    if (!invoke(g_kernel_backends[name].available)) {
        to_log(LOG_WARNING, "dasLLAMA: pin_kernel_backend('{name}') — backend reports itself unavailable in this configuration (stamped layout mismatch?); keeping '{g_active_backend}'\n")
        return
    }
    g_backend_pin = name
    select_kernel_backend(name)
}
def clear_kernel_backend_pin() {
    g_backend_pin = ""
}

//! Pin the BATCH-shaped kernel slots (prefill GEMM + mx4 batch) to backend `name`, keeping the
//! active backend's GEMV slots — the hybrid for boxes where decode/prefill prefer different backends.
//! Survives later reselection; `""` clears it. Layout-incompatible/unknown donor warns and no-ops.
def select_batch_backend(name : string) {
    g_batch_pin = name
    if (name != "") {
        apply_batch_override(name)
    }
}

//! The backend serving the BATCH-shaped slots (== active_kernel_backend() unless a batch pin
//! overrode them — diagnostics / tests).
def active_batch_backend() : string {
    return g_active_batch_backend
}

//! Select the best backend for a model load — INCLUDING repack backends (arm64 laneq). Honors a pin.
//! Returns true if the selected backend needs the loader to repack every weight tensor (via
//! repack_q8q8_weight) into its interleaved layout before any matmul runs.
def select_matmul_backend_for_load() : bool {
    if (g_backend_pin != "") {
        select_kernel_backend(g_backend_pin)
        return g_active_needs_repack
    }
    var best = ""
    var bestpri = -1
    for (be in values(g_kernel_backends)) {
        if (be.priority > bestpri && invoke(be.available)) {
            bestpri = be.priority
            best = be.name
        }
    }
    if (best != "") {
        select_kernel_backend(best)
    }
    return g_active_needs_repack
}

//! The active backend's name (diagnostics / tests). "" until a backend registers.
def active_kernel_backend() : string {
    return g_active_backend
}

//! Does the active backend carry a row-range GEMV core (mm_rows)? The fused multi-stage decode
//! chains gate on this and fall back to the per-op dispatch path when false.
def kernel_backend_has_rows() : bool {
    return g_active_has_rows
}

//! The active backend's row-range GEMV core (see MatmulQ8Q8RowsFn). Read it into a local in the
//! MAIN context and capture that in worker lambdas — same rule as every kernel-pointer read.
def mm_q8q8_rows_fn() : MatmulQ8Q8RowsFn {
    return g_mm_q8q8_rows
}

//! Does the active backend carry the mx4 row-range GEMV core (mx4_rows)? The fused MoE expert
//! chain gates on this and falls back to the per-op groupn dispatch path when false.
def kernel_backend_has_mx4_rows() : bool {
    return g_active_has_mx4_rows
}

//! The active backend's mx4 row-range GEMV core (see MatmulMx4RowsFn) — the MoE chain's twin of
//! mm_q8q8_rows_fn. Same read-into-a-local-then-capture rule.
def mx4_rows_fn() : MatmulMx4RowsFn {
    return g_mm_mx4q8_rows
}

//! Does the active backend carry the f16-scale kernel twins (the wscale_f16 rail)? The loader's
//! scale-plane convert gates on this — without them the model keeps the f32 plane (logged).
def kernel_backend_has_wscale16() : bool {
    return g_active_has_wscale16
}

//! The active backend's f16-scale row-range GEMV core (see MatmulQ8Q8RowsFnS16) — the chains'
//! twin of mm_q8q8_rows_fn. Same read-into-a-local rule.
def mm_q8q8_rows16_fn() : MatmulQ8Q8RowsFnS16 {
    return g_mm_q8q8_rows_s16
}

//! Does the active backend carry the f16-scale row-range GEMV core? The chains' twin of
//! kernel_backend_has_rows — a wscale_f16 model's fused chains gate on it.
def kernel_backend_has_rows16() : bool {
    return g_active_has_rows16
}

//! Does the active backend carry the native K-quant slots (mm_kq + kq_rows_* + repack_kq)?
//! Decides at LOAD whether the kq planes get repacked; a Model records the answer
//! (kq_repacked) and every later dispatch routes off that record, not this live query.
def kernel_backend_has_kq() : bool {
    return g_active_has_kq
}

//! The active backend's K-quant rows core for `fmt` (4/5/6/40) — the chains' kq twin of
//! mm_q8q8_rows_fn. Same read-into-a-local-then-capture rule. Any other id (kq_fi's q8
//! poison 0 included) gets the panic stub — never a silently-wrong format's core.
def kq_rows_fn(fmt : int) : MatmulKqRowsFn {
    if (fmt == 4) return g_kq_rows_k4
    if (fmt == 5) return g_kq_rows_k5
    if (fmt == 6) return g_kq_rows_k6
    if (fmt == 40) return g_kq_rows_q40
    return @@kq_unset_rows
}

//! Does the active backend carry the batched kq GEMM (kq_batch — the prefill tile)? The
//! batched dispatch gates on it and falls back to serial per-position GEMVs when down.
def kernel_backend_has_kq_batch() : bool {
    return g_active_has_kq_batch
}

//! A/B override for the grouped-MoE-prefill mx4 route: native mx4 batch GEMMs vs expand-to-Q8 +
//! Q8 batch. Safe to flip after load (both routes read the same repacked planes); forcing `true`
//! on a backend without mx4 batch slots hits their panic stubs — benchmark knob only.
def set_mx4_native_batch(on : bool) {
    g_active_mx4_native_batch = on
}

//! Does the active backend carry the region-list kq GEMV (kq_groupn — MoE expert dispatch)?
//! The MoE dispatch gates on it and falls back to per-region GEMVs when down.
def kernel_backend_has_kq_groupn() : bool {
    return g_active_has_kq_groupn
}

//! Does the active backend carry the fused batched region-list kq GEMM (kq_batch_groupn — the
//! fused MoE prefill)? The grouped prefill gates on it and keeps per-expert tiles when down.
def kernel_backend_has_kq_batch_groupn() : bool {
    return g_active_has_kq_batch_groupn
}

//! Batched region-list K-quant GEMM over REPACKED planes (triple offs — see the typedef).
//! Callers guard with kq_repacked && kernel_backend_has_kq_batch_groupn().
def matmul_kq_batch_groupn(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; offs : array<int64> | #; nregions : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d : int64) {
    unsafe {
        invoke(g_mm_kq_batch_groupn, fmt, addr(y[0]),
            addr<uint8 const?>(kq[0]), addr<uint8 const?>(ks[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<int const?>(xbs[0]), n, d)
    }
}

//! Region-list K-quant GEMV over REPACKED planes (kq twin of mm_at_q8_groupn): `offs` packs
//! (weight, activation) element-offset pairs, region r's rows land at y[r*d .. r*d+d). Plane bases
//! pass whole; callers guard with kq_repacked && kernel_backend_has_kq_groupn().
def matmul_kq_groupn_active(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; offs : array<int64> | #; nregions : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d : int64) {
    unsafe {
        invoke(g_mm_kq_groupn, fmt, addr(y[0]),
            addr<uint8 const?>(kq[0]), addr<uint8 const?>(ks[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<int const?>(xbs[0]), n, d)
    }
}

//! Batched K-quant GEMM over a REPACKED plane pair at plane-local element offset `woff`
//! (blob+offset form): ntok activation rows -> token-major Y [ntok x d]. Bit-for-bit ntok x
//! matmul_kq_active. Callers guard with kq_repacked && kernel_backend_has_kq_batch().
def matmul_kq_batch(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; woff : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d, ntok : int64) {
    let sb = woff / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        invoke(g_mm_kq_batch, fmt, addr(y[0]),
            addr<uint8 const?>(kq[sb * qsb]), addr<uint8 const?>(ks[sb * ssb]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<int const?>(xbs[0]), n, d, ntok)
    }
}

//! Native K-quant GEMV over a REPACKED plane pair at plane-local element offset `woff`
//! (blob+offset form — the repacked twin of dasllama_math_default's matmul_kq). Dispatches
//! to the active backend's mm_kq; callers guard with the model's kq_repacked record.
def matmul_kq_active(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; woff : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d : int64; yoff : int64 = 0l) {
    let sb = woff / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        invoke(g_mm_kq, fmt, addr(y[yoff]),
            addr<uint8 const?>(kq[sb * qsb]), addr<uint8 const?>(ks[sb * ssb]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<int const?>(xbs[0]), n, d)
    }
}

//! Repack one K-quant weight tensor (region at element offset woff in the fmt's planes, shape
//! [d rows x n cols]) into the active backend's kq layout, IN PLACE. No-op unless a kq-capable
//! backend is active. n % 256 == 0 (the ggml K-quant row invariant).
def repack_kq_weight(fmt : int; var kq : array<uint8>; var ks : array<uint8>; woff, n, d : int64) {
    let sb = woff / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        invoke(g_repack_kq, fmt, addr(kq[sb * qsb]), addr(ks[sb * ssb]), n, d)
    }
}

//! Registered backend names (diagnostics / tests).
def kernel_backend_names() : array<string> {
    return <- [for (k in keys(g_kernel_backends)); k]
}

//! Backend `name`'s f32 batch kernel (a batch-only donor's small-ntok / unsupported-shape
//! fallback rail — the metal backend delegates to "portable" through this). The panic stub
//! when unknown.
def kernel_backend_batch_fn(name : string) : MatmulQ8Q8BatchFn {
    var r = @@q8q8_unset_batch
    g_kernel_backends |> get(name) $(be) {
        r = be.batch
    }
    return r
}

//! Repack one Q8 weight tensor (region at element offset woff in qblob, shape [d rows x n cols]) into
//! the active backend's layout, IN PLACE. No-op unless the laneq backend is enabled. n % 32 == 0.
def repack_q8q8_weight(var qblob : array<int8>; var qscales : array<float>; woff, n, d : int64) {
    unsafe {
        invoke(g_repack_q8q8, addr(qblob[woff]), addr(qscales[woff / 32l]), n, d)
    }
}

//! Repack one native-MXFP4 weight tensor (region at weight-element offset woff in the mxq/mxs
//! planes, shape [d rows x n cols]) into the active backend's layout, IN PLACE. No-op unless a
//! repack backend is enabled. n % 32 == 0.
def repack_mx4_weight(var mxq : array<uint8>; var mxs : array<uint8>; woff, n, d : int64) {
    unsafe {
        invoke(g_repack_mx4, addr(mxq[woff / 2l]), addr(mxs[woff / 32l]), n, d)
    }
}

//! Does backend `name` read an interleaved (repacked) weight layout? false for unknown names.
//! Lets tests / direct callers skip layout-mismatched A/Bs (a repack backend cannot run on
//! row-major buffers).
def kernel_backend_needs_repack(name : string) : bool {
    var r = false
    g_kernel_backends |> get(name) $(be) {
        r = be.needs_repack
    }
    return r
}

//! Q8·Q8 matmul: y = W·x with W as int8 quants `wq` [d x n] + scales `ws` [d x n/32] and the
//! activation PRE-quantized to (xq [n], xs [n/32]) by the caller (it's shared across all d rows, so
//! quantizing once amortizes). Dispatches to the active backend (g_mm_q8q8). n % 32 == 0.
def matmul_q8q8(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Blob+offset form: the weight region at element offset `woff` in the whole quantized blob
//! (scales at woff/32) — no array_view (see the matmul woff overload).
def matmul_q8q8(var y : array<float>; wq : array<int8>; ws : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8, addr(y[0]),
            addr<int8 const?>(wq[woff]), addr<float const?>(ws[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Batched Q8·Q8 GEMM for prefill: same weight-stationary nest as matmul_batch over the SDOT kernel.
//! Activations PRE-quantized per token into (xq [ntok x n], xs [ntok x n/32]). Y token-major
//! [ntok x d]. Bit-for-bit ntok× matmul_q8q8. Dispatches to the active backend. n % 32 == 0.
def matmul_q8q8_batch(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q8q8_batch, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Blob+offset form (weights at woff, scales at woff/32) — no array_view.
def matmul_q8q8_batch(var y : array<float>; wq : array<int8>; ws : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q8q8_batch, addr(y[0]),
            addr<int8 const?>(wq[woff]), addr<float const?>(ws[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Borrowed-plane form: the Q8 quant and scale planes live in a prepared image. Scales index
//! at woff/32, exactly as the array form does.
def matmul_q8q8_batch(var y : array<float>; wq : PlaneI8; ws : PlaneF; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q8q8_batch, addr(y[0]),
            plane_at(wq, woff), plane_at(ws, woff / 32l),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Fused Q8·Q8 for three independent row-groups sharing one activation (attention Q/K/V): one
//! parallel_for over combined d0+d1+d2 rows so idle workers fill in on small K/V rows instead of
//! running single-thread (~23% faster than sequential Q-parallel + K,V-single on real shapes).
def matmul_q8q8_group3(var y0 : array<float>; var y1 : array<float>; var y2 : array<float>;
                       wq : array<int8>; ws : array<float>; woff0, woff1, woff2 : int64;
                       xq : array<int8>; xs : array<float>; n, d0, d1, d2 : int64) {
    unsafe {
        invoke(g_mm_q8q8_group3, addr(y0[0]), addr(y1[0]), addr(y2[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            woff0, woff1, woff2,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d0, d1, d2)
    }
}

// ===== f16-weight-scale overloads (the wscale_f16 rail) — same contracts, ws is the raw
// binary16 plane (Model.qscales16); overload resolution keys on the scale array type =====

//! Blob+offset form over the f16 scale plane — dispatches to the backend's mm_s16 twin.
def matmul_q8q8(var y : array<float>; wq : array<int8>; ws : array<uint16>; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_s16, addr(y[0]),
            addr<int8 const?>(wq[woff]), addr<uint16 const?>(ws[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Blob+offset batch form over the f16 scale plane (batch_s16).
def matmul_q8q8_batch(var y : array<float>; wq : array<int8>; ws : array<uint16>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q8q8_batch_s16, addr(y[0]),
            addr<int8 const?>(wq[woff]), addr<uint16 const?>(ws[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Q/K/V group form over the f16 scale plane (group3_s16).
def matmul_q8q8_group3(var y0 : array<float>; var y1 : array<float>; var y2 : array<float>;
                       wq : array<int8>; ws : array<uint16>; woff0, woff1, woff2 : int64;
                       xq : array<int8>; xs : array<float>; n, d0, d1, d2 : int64) {
    unsafe {
        invoke(g_mm_q8q8_group3_s16, addr(y0[0]), addr(y1[0]), addr(y2[0]),
            addr<int8 const?>(wq[0]), addr<uint16 const?>(ws[0]),
            woff0, woff1, woff2,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d0, d1, d2)
    }
}

//! Region-list form over the f16 scale plane (groupn_s16).
def matmul_q8q8_groupn(var y : array<float>; wq : array<int8> | #; ws : array<uint16> | #; offs : array<int64>; nregions : int64;
                       xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_groupn_s16, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<uint16 const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            null, null, n, d)
    }
}

//! Biased region-list form over the f16 scale plane (groupn_s16).
def matmul_q8q8_groupn(var y : array<float>; wq : array<int8> | #; ws : array<uint16> | #; offs : array<int64>; nregions : int64;
                       xq : array<int8>; xs : array<float>; bias : array<float> | #; boffs : array<int64>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_groupn_s16, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<uint16 const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<float const?>(bias[0]), addr<int64 const?>(boffs[0]), n, d)
    }
}

// ===== MXFP4·Q8: native 4-bit weights (MoE expert stacks) =====
// 32-elem block: 16 packed nibble bytes (low=elem j, high=j+16) + one E8M0 scale byte, in two planes
// (16-byte-regular for TBL); nibbles index the DOUBLED e2m1 table, e8m0_half compensates the doubling.

//! E8M0 scale byte -> fp32 * 0.5 = 2^(e-128), pairing with the DOUBLED e2m1 nibble values in
//! mxfp4_fill_lut. e < 2 lands in fp32-denormal territory -> precomputed bit patterns (mirrors
//! ggml_e8m0_to_fp32_half).
def e8m0_half(e : uint) : float {
    let bits = e < 2u ? (0x00200000u << e) : ((e - 1u) << 23u)
    return unsafe(reinterpret<float>(bits))
}

//! Fill a 16-byte LUT with the MXFP4 e2m1 magnitudes DOUBLED (nibble -> ±{0,.5,1,1.5,2,3,4,6}×2,
//! OCP MX spec via ggml kvalues_mxfp4) as int8 — the table `tbl16_lo`/`tbl16_hi` and the scalar
//! kernels index; the halved E8M0 scale (e8m0_half) compensates.
def mxfp4_fill_lut(var lut : int8[16]) {
    let kv = fixed_array(0, 1, 2, 3, 4, 6, 8, 12, 0, -1, -2, -3, -4, -6, -8, -12)
    for (i in range(16)) {
        lut[i] = int8(kv[i])
    }
}

//! MXFP4·Q8 GEMV: y = W·x with W native 4-bit — `wn` packed nibbles [d x n/2 bytes] + `we` raw E8M0
//! scale bytes [d x n/32] — and the activation PRE-quantized to (xq [n], xs [n/32]) by the caller.
//! Dispatches to the active backend (g_mm_mx4q8). n % 32 == 0.
def matmul_mx4q8(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Blob+offset form: weight region at ELEMENT offset `woff` in the whole nibble/exponent planes
//! (nibbles at woff/2, E8M0 at woff/32) — no array_view.
def matmul_mx4q8(var y : array<float>; wn : array<uint8>; we : array<uint8>; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8, addr(y[0]),
            addr<uint8 const?>(wn[woff / 2l]), addr<uint8 const?>(we[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Batched MXFP4·Q8 GEMM for prefill — batch analog of matmul_mx4q8 (Y token-major [ntok x d],
//! activations pre-quantized per token). Only backends flagged mx4_native_batch provide one (the
//! stub panics — grouped prefill routes through the Q8 expansion elsewhere). n % 32 == 0.
def matmul_mx4q8_batch(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_mx4q8_batch, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Blob+offset form (nibbles at woff/2, E8M0 at woff/32) — no array_view.
def matmul_mx4q8_batch(var y : array<float>; wn : array<uint8>; we : array<uint8>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_mx4q8_batch, addr(y[0]),
            addr<uint8 const?>(wn[woff / 2l]), addr<uint8 const?>(we[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Region-list batched MXFP4·Q8 GEMM — every routed expert's [d x n] GEMM over its own row range
//! of one token-row image, in ONE dispatch (see the MatmulMx4Q8BatchGroupNFn doc for the packed
//! `offs` triples). Only backends flagged mx4_native_batch provide one. n % 32 == 0.
def matmul_mx4q8_batch_groupn(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; offs : array<int64>; nregions : int64;
                              xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8_batch_groupn, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            null, null, n, d)
    }
}

//! Biased form: folds bias[boffs[r] + row] into every store of region r (per-expert bias vectors).
def matmul_mx4q8_batch_groupn(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; offs : array<int64>; nregions : int64;
                              xq : array<int8>; xs : array<float>; bias : array<float> | #; boffs : array<int64>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8_batch_groupn, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<float const?>(bias[0]), addr<int64 const?>(boffs[0]), n, d)
    }
}

//! Region-list Q8·Q8 GEMV: ONE dispatch for `nregions` same-shape [d x n] projections (see the
//! MatmulQ8Q8GroupNFn doc for the packed `offs` pairs). wq/ws are the WHOLE blob (offsets are
//! absolute); xq/xs hold the activation row(s) the offs pairs index. Region r -> y[r*d .. r*d+d).
def matmul_q8q8_groupn(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; offs : array<int64>; nregions : int64;
                       xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_groupn, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            null, null, n, d)
    }
}

//! Biased form: adds bias[boffs[r] + row] at the store (see the MatmulQ8Q8GroupNFn doc).
def matmul_q8q8_groupn(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; offs : array<int64>; nregions : int64;
                       xq : array<int8>; xs : array<float>; bias : array<float> | #; boffs : array<int64>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_groupn, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<float const?>(bias[0]), addr<int64 const?>(boffs[0]), n, d)
    }
}

//! Region-list MXFP4·Q8 GEMV — the native-4-bit twin of matmul_q8q8_groupn (offsets in weight
//! elements into the whole nibble/scale planes; the kernels derive the /2 and /32 byte positions).
def matmul_mx4q8_groupn(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; offs : array<int64>; nregions : int64;
                        xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8_groupn, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            null, null, n, d)
    }
}

//! Biased form: adds bias[boffs[r] + row] at the store (see the MatmulQ8Q8GroupNFn doc).
def matmul_mx4q8_groupn(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; offs : array<int64>; nregions : int64;
                        xq : array<int8>; xs : array<float>; bias : array<float> | #; boffs : array<int64>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8_groupn, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<float const?>(bias[0]), addr<int64 const?>(boffs[0]), n, d)
    }
}

//! Batched Q5_1·Q8 GEMM (blob+offset form): weight region at ELEMENT offset `woff` in the whole
//! q51 planes (quants at (woff/32)*20, d+m at (woff/32)*4) — the mm_b_q51_pre core. n % 32 == 0.
def matmul_q51q8_batch(var y : array<float>; wq : array<uint8>; ws : array<uint8>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q51q8_batch, addr(y[0]),
            addr<uint8 const?>(wq[(woff / 32l) * 20l]), addr<uint8 const?>(ws[(woff / 32l) * 4l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Region-list Q5_1·Q8 GEMV — the q51 twin of matmul_mx4q8_groupn (offs pack (weight element
//! offset, activation element offset) PAIRS; kernels derive the /32*20 and /32*4 byte positions).
def matmul_q51q8_groupn(var y : array<float>; wq : array<uint8> | #; ws : array<uint8> | #; offs : array<int64>; nregions : int64;
                        xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q51q8_groupn, addr(y[0]),
            addr<uint8 const?>(wq[0]), addr<uint8 const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Region-list batched Q5_1·Q8 GEMM — the q51 twin of matmul_mx4q8_batch_groupn (offs pack
//! (weight element offset, row0, cnt) TRIPLES over one token-row image). n % 32 == 0.
def matmul_q51q8_batch_groupn(var y : array<float>; wq : array<uint8> | #; ws : array<uint8> | #; offs : array<int64>; nregions : int64;
                              xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q51q8_batch_groupn, addr(y[0]),
            addr<uint8 const?>(wq[0]), addr<uint8 const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def template dot_q4_template(wq : uint8 const ?; ws : float const ?; x : float const ?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        let bbyte = bi * 16l
        let s = unsafe(ws[bi])
        var bs = 0.0
        for [tune = 1] (j in range64(16l)) {
            let byte = int(unsafe(wq[bbyte + j]))
            bs += float((byte & 0x0F) - 8) * unsafe(x[base + j]) + float((byte >> 4) - 8) * unsafe(x[base + 16l + j])
        }
        acc += s * bs
    }
    return acc
}

//! Q4_0 dequant-in-dot: `wq` is GGML split-half nibble packing (byte j = elem j low / j+16 high) +
//! fp32 scale `ws` per 32-block. Widens nibbles at NEON lane width 4 (width 8 loses ~7% to cross-lane
//! shuffles, hence fallback vec4_u4) — won the kernel bench (see matmul_variants). n % 32 == 0.
[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws), tuned(fallback = "vec4_u4")]
def dot_q4(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {}

//! Q4_0 matmul: y = W·x, W as 4-bit quants `wq` [d x n/2 bytes] + per-block scales `ws` [d x n/32].
//! Output rows thread at 4 chunks/job (nibble unpack is compute-bound, so finer splitting balances
//! better); small matmuls stay single-threaded. n % 32 == 0. Blob+offset: woff in elements.
def matmul_q4(var y : array<float>; wq : array<uint8>; ws : array<float>; woff : int64; x : array<float>; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<uint8 const?>(wq[woff / 2l])
        let sp = addr<float const?>(ws[woff / 32l])
        let xp = addr(x[0])
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), g_q4_chunks_per_job, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q4(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

def matmul_q4(var y : array<float>; wq : array<uint8> | #; ws : array<float> | #; x : array<float>; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<uint8 const?>(wq[0])
        let sp = addr<float const?>(ws[0])
        let xp = addr(x[0])
        // default 4 chunks per hw job (knob): the heavy nibble unpack makes each row compute-bound, so finer
        // splitting balances better (esp. the classifier) — more than the compute-light Q8 path.
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), g_q4_chunks_per_job, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q4(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

// noalias only on `weight` — `o` and `x` alias in-place for the Gemma2 post-norms (rms_at(buf,…,buf)),
// so a vectorize perm pays a runtime alias check and falls back to scalar exactly when they alias.
[hint(noalias = weight)]
def template rmsnorm_template(var o : float ?; x : float const ?; weight : float const ?; size : int64; eps : float = 1.0e-5) : void {
    unsafe {
        var ss = 0.0
        for [tune = 1] (j in range64(size)) {
            ss += x[j] * x[j]
        }
        ss /= float(size)
        ss += eps
        ss = 1.0 / sqrt(ss)
        for [tune = 1] (j in range64(size)) {
            o[j] = weight[j] * (x[j] * ss)
        }
    }
}

//! RMSNorm: o[i] = weight[i] * x[i] / sqrt(mean(x^2) + eps). eps defaults to 1e-5 (Llama-2/3);
//! Qwen2/Gemma2 pass 1e-6. `o`/`x` may alias (in-place post-norms) — the sum over x completes
//! before any write to o. Tuned fallback "plain" is byte-identical; a vectorized perm reorders low bits.
[hint(noalias = weight), tuned(fallback = "plain")]
def rmsnorm(var o : float?; x : float const?; weight : float const?; size : int64; eps : float = 1.0e-5) {}
def rmsnorm(var o : array<float> | #; x : array<float> | #; weight : array<float> | #; size : int64; eps : float = 1.0e-5) {
    // reinterpret strips the temporary (#) qualifier — rms_at passes array_view (#) slices. Output `o`
    // keeps a non-const pointer (it's written); the read-only x/weight add const.
    unsafe {
        rmsnorm(addr<float?>(o[0]), addr<float const?>(x[0]),
            addr<float const?>(weight[0]), size, eps)
    }
}

//! Horizontal max of the first `n` floats, hand-vectorized (two float4 accumulators; ~9x over scalar).
//! Reassociated by hand for bit-exactness — LLVM won't auto-vectorize a max-reduction without
//! nnan/nsz fast-math. Safe for n < 8; returns -FLT_MAX for n <= 0.
def hmax(x : float const?; n : int64) : float {
    if (n <= 0l) return -FLT_MAX
    unsafe {
        let pf4 = reinterpret<float4 const?>(x)
        let n8 = n / 8l
        var acc0 = float4(x[0], x[0], x[0], x[0])
        var acc1 = acc0
        for (k in 0l..n8) {
            acc0 = max(acc0, pf4[2l * k])
            acc1 = max(acc1, pf4[2l * k + 1l])
        }
        let acc = max(acc0, acc1)
        var maxv = max(max(acc.x, acc.y), max(acc.z, acc.w))
        for (i in n8 * 8l .. n) {
            maxv = max(maxv, x[i])
        }
        return maxv
    }
}

def template softmax_template(var x : float ?; size : int64) : void {
    if (size <= 0) return   // seeds the reduction with x[0]; size<=0 must be a no-op (same contract as the array overload)
    unsafe {
        let maxv = hmax(x, size)   // hand-vectorized max reduction (see hmax)
        // exp4 fold (like the flash softmax folds): clamp underflow to -80 (exp -> ~0, negligible)
        let m4 = float4(maxv, maxv, maxv, maxv)
        var sum4 = float4(0.0)
        var i = 0l
        while (i + 4l <= size) {
            let e = exp4(max(float4(x[i], x[i + 1l], x[i + 2l], x[i + 3l]) - m4, EXP4_LO))
            x[i] = e.x
            x[i + 1l] = e.y
            x[i + 2l] = e.z
            x[i + 3l] = e.w
            sum4 += e
            i += 4l
        }
        var sum = sum4.x + sum4.y + sum4.z + sum4.w
        while (i < size) {
            let e = exp(max(x[i] - maxv, -80.0))
            x[i] = e
            sum += e
            i++
        }
        let inv_sum = 1.0 / sum   // scale by the reciprocal (matches ggml_vec_scale_f32), not a per-element /=
        for [tune = 1] (j in range64(size)) {
            x[j] *= inv_sum
        }
    }
}

//! Pointer core — the threaded prefill attention runs in fork contexts and can only touch raw
//! addresses; the array overload forwards here, so both are bit-identical. Output `x` is a `var
//! float?` written in place (a hoisted/captured fork pointer binds directly — no const-strip).
[tuned]
def softmax(var x : float?; size : int64) {}
def softmax(var x : array<float> | #; size : int64) {
    if (size <= 0) return   // empty array: addr(x[0]) would index out of range; old loop form no-op'd
    softmax(unsafe(addr(x[0])), size)
}

def template softmax_sink_template(var x : float ?; size : int64; sink : float) : void {
    if (size <= 0) return
    unsafe {
        let maxv = max(hmax(x, size), sink)
        // exp4 fold — same shape (and clamp convention) as softmax_template
        let m4 = float4(maxv, maxv, maxv, maxv)
        var sum4 = float4(0.0)
        var i = 0l
        while (i + 4l <= size) {
            let e = exp4(max(float4(x[i], x[i + 1l], x[i + 2l], x[i + 3l]) - m4, EXP4_LO))
            x[i] = e.x
            x[i + 1l] = e.y
            x[i + 2l] = e.z
            x[i + 3l] = e.w
            sum4 += e
            i += 4l
        }
        var sum = sum4.x + sum4.y + sum4.z + sum4.w
        while (i < size) {
            let e = exp(max(x[i] - maxv, -80.0))
            x[i] = e
            sum += e
            i++
        }
        sum += exp(max(sink - maxv, -80.0))
        let inv_sum = 1.0 / sum
        for [tune = 1] (j in range64(size)) {
            x[j] *= inv_sum
        }
    }
}

//! Softmax with an attention-sink logit (gpt-oss): `sink` joins the max and denominator as one
//! extra virtual score with NO value contribution — x sums to sum/(sum+e^(sink-max)) < 1 by design.
//! Shares softmax's loop shape, so the tuner mirrors softmax's winning perm onto its profile key.
[tuned]
def softmax_sink(var x : float?; size : int64; sink : float) {}
def softmax_sink(var x : array<float> | #; size : int64; sink : float) {
    if (size <= 0) return
    softmax_sink(unsafe(addr(x[0])), size, sink)
}

//! Vectorized expf, 4 lanes (ggml_v_expf, ARM optimized-routines): ~2 ulp vs libm, ~7x scalar exp()
//! (which scalarizes on Darwin-ARM, no vector libm). FAST PATH ONLY (no |n|>126 guard) — valid for
//! x in ~[-87, 88]; callers must clamp (the flash softmax fold clamps to -80 before calling).
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

//! Index of the max element, first-index-wins on ties — the greedy-decode argmax. Chunked team
//! scan (serial 151-262k-vocab scan idled every lane 210-284us/token); combine folds candidates
//! in index order with strict >, so the result is bit-identical to the serial loop for ANY split.
let ARGMAX_MAX_CHUNKS = 64   // partials cap: enough for any current box's lanes*2

def public parallel_argmax(x : float const?; n : int64) : int64 {
    if (n <= 1l) {
        return 0l
    }
    let nch = int(min(min(int64(get_dispatch_lanes()) * 2l, n / 4096l), int64(ARGMAX_MAX_CHUNKS)))
    unsafe {
        if (nch <= 1) {
            var best = 0l
            for (i in range64(1l, n)) {
                if (x[i] > x[best]) {
                    best = i
                }
            }
            return best
        }
        // fixed-size worker partials: no heap on the hot path (fork-pool context clones forbid
        // a global scratch); ARGMAX_MAX_CHUNKS caps chunking, not correctness
        var cm : float[ARGMAX_MAX_CHUNKS]
        var ci : int64[ARGMAX_MAX_CHUNKS]
        var cmp = addr(cm[0])
        var cip = addr(ci[0])
        let nn = n
        let nc = int64(nch)
        maybe_parallel_for(0, nch, nch) $(cb, ce) {
            unsafe {
                for (c in range(cb, ce)) {
                    let r0 = int64(c) * nn / nc
                    let r1 = (int64(c) + 1l) * nn / nc
                    var b = r0
                    for (i in range64(r0 + 1l, r1)) {
                        if (x[i] > x[b]) {
                            b = i
                        }
                    }
                    cmp[c] = x[b]
                    cip[c] = b
                }
            }
        }
        var best = ci[0]
        var bm = cm[0]
        for (c in range(1, nch)) {
            if (cm[c] > bm) {
                bm = cm[c]
                best = ci[c]
            }
        }
        return best
    }
}

//! Array overload — forwards to the pointer core.
def public parallel_argmax(x : array<float> | #; n : int64) : int64 {
    if (n <= 0l) {
        return 0l
    }
    return parallel_argmax(unsafe(addr(x[0])), n)
}

//! SiLU (a.k.a. swish) activation: silu(x) = x * sigmoid(x) = x / (1 + exp(-x)), in place.
//! The nonlinearity in the SwiGLU FFN gate; smooth, ~identity for large x, dips slightly negative near zero.
def silu(var x : float?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            let v = x[i]
            x[i] = v / (1.0 + exp(-v))
        }
    }
}
def silu(var x : array<float> | #; size : int64) {
    if (size <= 0) return   // empty array: addr(x[0]) would index out of range; old loop form no-op'd
    silu(unsafe(addr(x[0])), size)
}

//! Softplus, matching ggml's op_softplus exactly: x > 20 → x, else log(1 + exp(x)).
def softplus(x : float) : float => x > 20.0 ? x : log(1.0 + exp(x))

//! Row-wise L2 normalization in place (ggml_l2_norm): scale = 1 / max(sqrt(Σx²), eps), the
//! square-sum accumulated in double like ggml's ggml_float. Used on the deltanet q/k heads.
[hint(noalias = v)]
def l2_norm_rows(var v : float?; rows, row_size : int64; eps : float) {
    unsafe {
        for (r in range64(rows)) {
            let base = r * row_size
            var sum = 0.0lf
            for (i in range64(row_size)) {
                let x = v[base + i]
                sum += double(x * x)
            }
            let scale = 1.0 / max(sqrt(float(sum)), eps)
            for (i in range64(row_size)) {
                v[base + i] *= scale
            }
        }
    }
}

//! GELU, tanh approximation (HF's gelu_pytorch_tanh): the GeGLU gate nonlinearity (Gemma2).
//! gelu(x) = 0.5x(1 + tanh(sqrt(2/pi)(x + 0.044715 x^3))), computed directly rather than via
//! ggml's fp16 LUT approximation — near-ties against the LUT are expected, not bugs.
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
    if (size <= 0) return   // empty array: addr(x[0]) would index out of range; old loop form no-op'd
    gelu(unsafe(addr(x[0])), size)
}

//! Fused SwiGLU gate: g[i] = silu(g[i]) * u[i] in place (g = W1·x gate, u = W3·x up). One pass
//! instead of silu-then-mul avoids round-tripping the intermediate through memory; bit-exact vs the split path.
def swiglu(var g : float?; u : float const?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            let v = g[i]
            g[i] = (v / (1.0 + exp(-v))) * u[i]
        }
    }
}
def swiglu(var g : array<float> | #; u : array<float> | #; size : int64) {
    if (size <= 0) return
    swiglu(unsafe(addr(g[0])), unsafe(addr(u[0])), size)
}

//! Fused GeGLU gate (Gemma2): g[i] = gelu(g[i]) * u[i] in place, tanh-approx gelu. Same fusion
//! rationale and bit-exactness as swiglu.
def geglu(var g : float?; u : float const?; size : int64) {
    let k = 0.7978845608028654  // sqrt(2/pi)
    unsafe {
        for (i in range64(size)) {
            let v = g[i]
            g[i] = (0.5 * v * (1.0 + tanh(k * (v + 0.044715 * v * v * v)))) * u[i]
        }
    }
}
def geglu(var g : array<float> | #; u : array<float> | #; size : int64) {
    if (size <= 0) return
    geglu(unsafe(addr(g[0])), unsafe(addr(u[0])), size)
}

// gpt-oss clamped-SwiGLU constants (llama.cpp hardcodes both in build_moe_ffn's SWIGLU_OAI arm)
let SWIGLU_OAI_ALPHA = 1.702
let SWIGLU_OAI_LIMIT = 7.0

//! Fused clamped SwiGLU (gpt-oss, ggml_swiglu_oai): x = min(g, 7), y = clamp(u, -7, 7),
//! g = (x · sigmoid(1.702·x)) · (y + 1). The clamps and the +1 on the up branch are part of the
//! trained activation, not an optimization. Same fusion contract as swiglu.
def swiglu_oai(var g : float?; u : float const?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            let x = min(g[i], SWIGLU_OAI_LIMIT)
            let y = clamp(u[i], -SWIGLU_OAI_LIMIT, SWIGLU_OAI_LIMIT)
            g[i] = (x / (1.0 + exp(-SWIGLU_OAI_ALPHA * x))) * (y + 1.0)
        }
    }
}
def swiglu_oai(var g : array<float> | #; u : array<float> | #; size : int64) {
    if (size <= 0) return
    swiglu_oai(unsafe(addr(g[0])), unsafe(addr(u[0])), size)
}

// exp4 valid range (fast-path, no overflow guard) — clamp the exp argument here so an outlier
// gate value can never feed exp4 out of range (silu saturates to 0/v, gelu to ±1 there anyway).
let EXP4_LO = float4(-80.0, -80.0, -80.0, -80.0)
let EXP4_HI = float4(80.0, 80.0, 80.0, 80.0)

//! Vectorized fused SwiGLU gate (default): g[i] = silu(g[i]) * u[i], float4 with exp4. sigmoid =
//! 1/(1+exp4(-v)); ~4.5x the scalar swiglu (which pays a libm expf per element). ~2 ulp, NOT bit-exact
//! (that's the scalar swiglu, kept as the opt-in reference). Tail (<4 elems) runs the scalar form.
def swiglu4(var g : float?; u : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var g4 = reinterpret<float4?>(g)
        var u4 = reinterpret<float4 const?>(u)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let v = g4[k]
            let arg = clamp(-v, EXP4_LO, EXP4_HI)
            g4[k] = (v / (one + exp4(arg))) * u4[k]
        }
        for (i in range64(n4 * 4l, size)) {
            let v = g[i]
            g[i] = (v / (1.0 + exp(-v))) * u[i]
        }
    }
}

//! Vectorized fused GeGLU gate (default, Gemma2): g[i] = gelu(g[i]) * u[i], float4 with exp4 via the
//! identity tanh(a) = 1 - 2/(1+exp(2a)); ~4.8x the scalar geglu (which pays a libm tanhf per element —
//! the slowest transcendental, no vector path). ~2 ulp, NOT bit-exact. Tail runs the scalar form.
def geglu4(var g : float?; u : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    let two = float4(2.0, 2.0, 2.0, 2.0)
    let half = float4(0.5, 0.5, 0.5, 0.5)
    let gk = float4(0.7978845608028654)   // sqrt(2/pi)
    let cc = float4(0.044715)
    unsafe {
        var g4 = reinterpret<float4?>(g)
        var u4 = reinterpret<float4 const?>(u)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let v = g4[k]
            let a = gk * (v + cc * v * v * v)
            let arg = clamp(two * a, EXP4_LO, EXP4_HI)
            let th = one - two / (one + exp4(arg))   // tanh(a)
            g4[k] = (half * v * (one + th)) * u4[k]
        }
        let kk = 0.7978845608028654
        for (i in range64(n4 * 4l, size)) {
            let v = g[i]
            g[i] = (0.5 * v * (1.0 + tanh(kk * (v + 0.044715 * v * v * v)))) * u[i]
        }
    }
}

//! Vectorized fused clamped SwiGLU (gpt-oss), float4 with exp4. A very negative gate makes -alpha·x
//! large-positive; the EXP4_HI clamp keeps exp4 finite (exp4(80)≈5.5e34), driving sigmoid to ~0.
//! ~2 ulp, NOT bit-exact (scalar swiglu_oai is the opt-in reference); scalar tail for <4 elems.
def swiglu_oai4(var g : float?; u : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    let lim = float4(SWIGLU_OAI_LIMIT)
    let nalpha = float4(-SWIGLU_OAI_ALPHA)
    unsafe {
        var g4 = reinterpret<float4?>(g)
        var u4 = reinterpret<float4 const?>(u)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let x = min(g4[k], lim)
            let y = clamp(u4[k], -lim, lim)
            let arg = clamp(nalpha * x, EXP4_LO, EXP4_HI)
            g4[k] = (x / (one + exp4(arg))) * (y + one)
        }
        for (i in range64(n4 * 4l, size)) {
            let x = min(g[i], SWIGLU_OAI_LIMIT)
            let y = clamp(u[i], -SWIGLU_OAI_LIMIT, SWIGLU_OAI_LIMIT)
            g[i] = (x / (1.0 + exp(-SWIGLU_OAI_ALPHA * x))) * (y + 1.0)
        }
    }
}

//! Vectorized SiLU: x[i] = x[i] · σ(x[i]) in place, float4 with exp4; ~4x the scalar silu
//! (which pays a libm expf per element). ~2 ulp, NOT bit-exact — scalar silu stays the
//! reference the chat-model oracles pin; ungated conformer FFNs (parakeet) call this form.
def silu4(var x : float?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var x4 = reinterpret<float4?>(x)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let v = x4[k]
            let arg = clamp(-v, EXP4_LO, EXP4_HI)
            x4[k] = v / (one + exp4(arg))
        }
        for (i in range64(n4 * 4l, size)) {
            let v = x[i]
            x[i] = v / (1.0 + exp(-v))
        }
    }
}
def silu4(var x : array<float> | #; size : int64) {
    if (size <= 0) return
    silu4(unsafe(addr(x[0])), size)
}

//! Threaded silu4 over a token-major image (npos rows x d columns): rows split across lanes.
//! Row length must be a multiple of 4 (keeps chunk starts 4-aligned so results stay bit-identical
//! at any lane count); measured M1 8 lanes: 3.6x on parakeet jfk ffn, 6.9x on gb1.
def silu4_batch(var x : array<float> | #; d, npos : int64) {
    if (d % 4l != 0l || npos <= 1l) {   // odd row length would shift the float4 grouping per chunk
        silu4(x, d * npos)
        return
    }
    unsafe {
        var xp = addr(x[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d * 16l, 0)) $(rb, re) {
            silu4(unsafe(xp + int64(rb) * d), int64(re - rb) * d)
        }
    }
}

//! Sigmoid gate: x[i] *= σ(g[i]) over the first `size` elements — the gated-attention out-gate
//! shape (qwen35). Scalar reference form; the exp4 twin below is the production path.
def sigmoid_gate(var x : float?; g : float const?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            x[i] *= 1.0 / (1.0 + exp(-g[i]))
        }
    }
}

//! Vectorized sigmoid gate: float4 with exp4, scalar tail. ~2 ulp, NOT bit-exact vs the scalar form.
def sigmoid_gate4(var x : float?; g : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var x4 = reinterpret<float4?>(x)
        let g4 = reinterpret<float4 const?>(g)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let arg = clamp(-g4[k], EXP4_LO, EXP4_HI)
            x4[k] = x4[k] / (one + exp4(arg))
        }
        for (i in range64(n4 * 4l, size)) {
            x[i] *= 1.0 / (1.0 + exp(-g[i]))
        }
    }
}

//! Fused SiLU gate into a destination: o[i] = a[i] · silu(z[i]), float4 with exp4, scalar tail.
//! ~2 ulp, NOT bit-exact. The deltanet chunked z-gated out-norm shape (o must not alias a/z).
def silu_mul4(var o : float?; a, z : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var o4 = reinterpret<float4?>(o)
        let a4 = reinterpret<float4 const?>(a)
        let z4 = reinterpret<float4 const?>(z)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let zv = z4[k]
            let arg = clamp(-zv, EXP4_LO, EXP4_HI)
            o4[k] = a4[k] * (zv / (one + exp4(arg)))
        }
        for (i in range64(n4 * 4l, size)) {
            let zv = z[i]
            o[i] = a[i] * (zv / (1.0 + exp(-zv)))
        }
    }
}

//! Logit soft-capping (Gemma2): x = cap*tanh(x/cap), bounding values to (-cap, cap). Applied to
//! attention scores (cap 50, after the 1/sqrt(head_size) scale) and final classifier logits (cap 30).
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

//! Vectorized logit soft-capping: x = cap*tanh(x/cap), float4 via the tanh(a)=1-2/(1+exp(2a)) identity.
//! ~2 ulp, NOT bit-exact vs scalar softcap. A 256k-vocab scalar-tanh pass cost 2-4ms/token; this
//! is the production path (scalar tail for the remainder).
def softcap4(var x : float?; size : int64; cap : float) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    let two = float4(2.0, 2.0, 2.0, 2.0)
    let cap4 = float4(cap)
    let invcap4 = float4(1.0 / cap)
    unsafe {
        var x4 = reinterpret<float4?>(x)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let arg = clamp(two * (x4[k] * invcap4), EXP4_LO, EXP4_HI)   // 2 * (x/cap)
            let th = one - two / (one + exp4(arg))                       // tanh(x/cap)
            x4[k] = cap4 * th
        }
        let inv = 1.0 / cap
        for (i in range64(n4 * 4l, size)) {
            x[i] = cap * tanh(x[i] * inv)
        }
    }
}
def softcap4(var x : array<float> | #; size : int64; cap : float) {
    softcap4(unsafe(addr(x[0])), size, cap)
}

//! RoPE — rotary position embedding. Rotates each adjacent coordinate pair (2j, 2j+1) within a
//! head by angle pos*freq, freq = 1/10000^(2j/head_size); encodes position as rotation so a later
//! Q·K dot product depends only on relative token offset. In place; matches llama2.c's scheme.
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

//! RoPE with explicit base frequency θ and optional per-pair frequency factors (GGUF `rope_freqs`,
//! Llama-3.1 NTK-by-parts). Empty factors => plain RoPE at θ; (θ=10000, empty) matches rope() bit-for-bit.
//! fscale is linear position scaling (angle = pos*fscale*freq); `has_ff` gates the `ff` pointer (pass null when false).
[hint(noalias = v, noalias = ff)]
def rope_scaled(var v : float?; pos, head_size, n : int64; theta, fscale : float; has_ff : bool; ff : float const?; mscale : float = 1.0) {
    unsafe {
        let posf = float(pos) * fscale
        for (p in range64(n / 2l)) {
            let i = p * 2l
            let head_dim = i % head_size
            var freq = 1.0 / pow(theta, float(head_dim) / float(head_size))
            if (has_ff) {
                freq /= ff[head_dim / 2l]
            }
            let angle = posf * freq
            // mscale (YaRN/LongRoPE attn_factor) scales cos/sin — ggml applies it to NORM and NEOX
            // alike (mistral3 YaRN = 1.416); the 1.0 default multiplies out bit-exactly elsewhere
            let fcr = cos(angle) * mscale
            let fci = sin(angle) * mscale
            let v0 = v[i]
            let v1 = v[i + 1]
            v[i] = v0 * fcr - v1 * fci
            v[i + 1] = v0 * fci + v1 * fcr
        }
    }
}
def rope_scaled(var vec : array<float> | #; pos, head_size, n : int64; theta, fscale : float; freq_factors : array<float> | #; mscale : float = 1.0) {
    let has_ff = !empty(freq_factors)
    unsafe {
        let ff = has_ff ? addr<float const?>(freq_factors[0]) : null
        rope_scaled(addr<float?>(vec[0]), pos, head_size, n, theta, fscale, has_ff, ff, mscale)
    }
}

//! NEOX-style RoPE (Qwen2/Phi3/Gemma2): rotated pair is offset by head_size/2 (vec[base+j] with
//! vec[base+j+half]) instead of the adjacent (2j, 2j+1) pair NORM rope uses; same per-pair frequencies.
[hint(noalias = v, noalias = ff)]
def rope_scaled_neox(var v : float?; pos, head_size, n : int64; theta, fscale : float; has_ff : bool; ff : float const?; mscale : float = 1.0) {
    unsafe {
        let half = head_size / 2l
        let posf = float(pos) * fscale
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                var freq = 1.0 / pow(theta, float(2l * j) / float(head_size))
                if (has_ff) {
                    freq /= ff[j]
                }
                let angle = posf * freq
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
def rope_scaled_neox(var vec : array<float> | #; pos, head_size, n : int64; theta, fscale : float; freq_factors : array<float> | #; mscale : float = 1.0) {
    let has_ff = !empty(freq_factors)
    unsafe {
        let ff = has_ff ? addr<float const?>(freq_factors[0]) : null
        rope_scaled_neox(addr<float?>(vec[0]), pos, head_size, n, theta, fscale, has_ff, ff, mscale)
    }
}

//! Table-driven RoPE (NORM, adjacent pairs); bit-identical to rope_scaled (cos/sin hoisted into a
//! precomputed table since angle pos*freq is identical across heads/layers).
//! Looped per head, not flat p%half — dropping the per-pair modulo is ~2.3x; NEON vectorize doesn't help (bandwidth-bound).
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

[hint(noalias = v, noalias = cosrow, noalias = sinrow)]
def template rope_scaled_neox_tab_template(var v : float ?; head_size, n : int64; cosrow : float const ?; sinrow : float const ?) : void {
    unsafe {
        let half = head_size / 2l
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for [tune = 1] (j in range64(half)) {
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

//! Table-driven NEOX RoPE; bit-identical to rope_scaled_neox (mscale baked into cosrow/sinrow).
//! Contiguous v0/v1 split layout vectorizes width-8 (~5-10% over scalar, bit-exact — no fma contract);
//! the NORM leaf's interleaved stride-2 layout can't do that.
[hint(noalias = v, noalias = cosrow, noalias = sinrow), tuned]
def rope_scaled_neox_tab(var v : float?; head_size, n : int64; cosrow : float const?; sinrow : float const?) {}

//! NEOX RoPE over only the FIRST `rot` dims of each head (partial rotary — qwen35: 64 of 256).
//! Pairs are (base+j, base+j+rot/2), frequencies run over `rot`; heads stride by full head_size,
//! dims >= rot pass through untouched (text-only M-RoPE degenerates to this).
[hint(noalias = v, noalias = ff)]
def rope_scaled_neox_part(var v : float?; pos, head_size, rot, n : int64; theta, fscale : float; has_ff : bool; ff : float const?; mscale : float = 1.0) {
    unsafe {
        let half = rot / 2l
        let posf = float(pos) * fscale
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                var freq = 1.0 / pow(theta, float(2l * j) / float(rot))
                if (has_ff) {
                    freq /= ff[j]
                }
                let angle = posf * freq
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

//! Table form of rope_scaled_neox_part: cosrow/sinrow are one position's rot/2-wide row of a
//! table built by build_rope_table with head_size = rot. Bit-identical to the direct form.
[hint(noalias = v, noalias = cosrow, noalias = sinrow)]
def rope_scaled_neox_tab_part(var v : float?; head_size, rot, n : int64; cosrow : float const?; sinrow : float const?) {
    unsafe {
        let half = rot / 2l
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
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

//! Dispatch RoPE by the model's convention: NEOX (Qwen2/Phi3/Gemma2) or default NORM (Llama).
//! mscale is the YaRN/LongRoPE magnitude factor; both paths apply it (mistral3 = NORM + YaRN).
def rope_apply(var vec : float?; pos, head_size, n : int64; theta, fscale : float; has_ff : bool; ff : float const?; neox : bool; mscale : float = 1.0) {
    if (neox) {
        rope_scaled_neox(vec, pos, head_size, n, theta, fscale, has_ff, ff, mscale)
    } else {
        rope_scaled(vec, pos, head_size, n, theta, fscale, has_ff, ff, mscale)
    }
}
//! `use_ff = false` drops the freq-factors for this call even when they're loaded — p-RoPE arches
//! (gemma4) apply them to global layers only.
def rope_apply(var vec : array<float> | #; pos, head_size, n : int64; theta, fscale : float; freq_factors : array<float> | #; neox : bool; mscale : float = 1.0; use_ff : bool = true) {
    let has_ff = use_ff && !empty(freq_factors)
    unsafe {
        let ff = has_ff ? addr<float const?>(freq_factors[0]) : null
        rope_apply(addr<float?>(vec[0]), pos, head_size, n, theta, fscale, has_ff, ff, neox, mscale)
    }
}

// Function values do not survive the -exe bake's global serialization (the
// dasllama_config restore names the defect) — every @@-defaulted seam in this
// module boot-restores here; a real registrar wins via the null guards.
[init]
def private restore_math_fn_defaults { // nolint:STYLE037,STYLE038 — flat seam list, one guard per global
    if (g_repack_q8q8 == null) {
        g_repack_q8q8 = @@repack_q8q8_identity
    }
    if (g_repack_mx4 == null) {
        g_repack_mx4 = @@repack_mx4_identity
    }
    if (g_mm_q8q8 == null) {
        g_mm_q8q8 = @@q8q8_unset_kernel
    }
    if (g_mm_q8q8_batch == null) {
        g_mm_q8q8_batch = @@q8q8_unset_batch
    }
    if (g_mm_q8q8_group3 == null) {
        g_mm_q8q8_group3 = @@q8q8_unset_group3
    }
    if (g_mm_mx4q8 == null) {
        g_mm_mx4q8 = @@mx4q8_unset_kernel
    }
    if (g_mm_mx4q8_batch == null) {
        g_mm_mx4q8_batch = @@mx4q8_unset_batch
    }
    if (g_mm_mx4q8_batch_groupn == null) {
        g_mm_mx4q8_batch_groupn = @@mx4q8_unset_batch_groupn
    }
    if (g_mm_q8q8_groupn == null) {
        g_mm_q8q8_groupn = @@q8q8_unset_groupn
    }
    if (g_mm_mx4q8_groupn == null) {
        g_mm_mx4q8_groupn = @@mx4q8_unset_groupn
    }
    if (g_mm_q51q8_batch == null) {
        g_mm_q51q8_batch = @@q51q8_batch_kernel
    }
    if (g_mm_q51q8_groupn == null) {
        g_mm_q51q8_groupn = @@q51q8_groupn_kernel
    }
    if (g_mm_q51q8_batch_groupn == null) {
        g_mm_q51q8_batch_groupn = @@q51q8_batch_groupn_kernel
    }
    if (g_mm_q8q8_rows == null) {
        g_mm_q8q8_rows = @@q8q8_unset_rows
    }
    if (g_mm_mx4q8_rows == null) {
        g_mm_mx4q8_rows = @@mx4q8_unset_rows
    }
    if (g_mm_q8q8_s16 == null) {
        g_mm_q8q8_s16 = @@q8q8_unset_kernel_s16
    }
    if (g_mm_q8q8_batch_s16 == null) {
        g_mm_q8q8_batch_s16 = @@q8q8_unset_batch_s16
    }
    if (g_mm_q8q8_group3_s16 == null) {
        g_mm_q8q8_group3_s16 = @@q8q8_unset_group3_s16
    }
    if (g_mm_q8q8_groupn_s16 == null) {
        g_mm_q8q8_groupn_s16 = @@q8q8_unset_groupn_s16
    }
    if (g_mm_q8q8_rows_s16 == null) {
        g_mm_q8q8_rows_s16 = @@q8q8_unset_rows_s16
    }
    if (g_mm_kq == null) {
        g_mm_kq = @@kq_unset_kernel
    }
    if (g_kq_rows_k4 == null) {
        g_kq_rows_k4 = @@kq_unset_rows
    }
    if (g_kq_rows_k5 == null) {
        g_kq_rows_k5 = @@kq_unset_rows
    }
    if (g_kq_rows_k6 == null) {
        g_kq_rows_k6 = @@kq_unset_rows
    }
    if (g_kq_rows_q40 == null) {
        g_kq_rows_q40 = @@kq_unset_rows
    }
    if (g_mm_kq_batch == null) {
        g_mm_kq_batch = @@kq_unset_batch
    }
    if (g_mm_kq_groupn == null) {
        g_mm_kq_groupn = @@kq_unset_groupn
    }
    if (g_mm_kq_batch_groupn == null) {
        g_mm_kq_batch_groupn = @@kq_unset_batch_groupn
    }
    if (g_bake_q51_repack == null) {
        g_bake_q51_repack = @@repack_q51_identity
    }
    if (g_bake_q8q8_repack == null) {
        g_bake_q8q8_repack = @@repack_q8q8_identity
    }
    if (g_bake_mx4_repack == null) {
        g_bake_mx4_repack = @@repack_mx4_identity
    }
    if (g_bake_kq_repack == null) {
        g_bake_kq_repack = @@repack_kq_identity
    }
    if (g_repack_q51 == null) {
        g_repack_q51 = @@repack_q51_identity
    }
    if (g_repack_kq == null) {
        g_repack_kq = @@repack_kq_identity
    }
    if (g_active_kq_layout == null) {
        g_active_kq_layout = @@kq_layout_grp4_fmt
    }
}
