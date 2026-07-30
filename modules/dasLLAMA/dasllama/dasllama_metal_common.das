options gen2
options _comment_hygiene = true
options _metal_manual_dispatch = true   // PERMANENT opt-out: this module IS the capture rail (kn_dispatch / graph replay dispatch here)

module dasllama_metal_common shared public

require math
require strings
require dasllama/dasllama_env  // env_int/env_flag: the shared typed readers + the knob registry
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_kv_codec  // tq4_fill_signs — GPU and CPU must share the exact sign vector
require dasllama/dasllama_metal_shapes public   // the extracted portable shape gates — re-exported so the driver family keeps its names

// Shared infrastructure for the dasMetal resident drivers (the format-matrix arc's W1 split):
// device/queue init, buffer pools, the weight-region + block_q8_0-blob + cat-blob caches, the
// per-session KV mirror arena (watermark/LRU/rewind contract — see dasllama_metal_decode's
// header), HazTrack, the pending-step (encode-ahead) machinery + metal_decode_flush, upload +
// uniform helpers, and the decline observability layer (all three GPU paths' reason enums,
// declines-by-reason tables, and the stage report). The kernel/PSO registry
// (dasllama_metal_kernels) and the family graph modules (dasllama_metal_decode) sit on top.
// Module-level state is deliberately non-private: this is an internal driver family, required
// only by dasLLAMA itself, and the family modules read/write it directly.

// ===== driver state =====

// chunk width of the split-K attention pair (64 rows per threadgroup — must match the part
// kernels' slab + hardcoded chunk math; 64 beats 128 on M1: more threadgroups in flight + half
// the serial per-lane pass length)
let ATTN_CHUNK_ROWS = 64l
// contexts up to this depth take the one-dispatch single-tg kernel (no comb dispatch), deeper
// ones the chunked pair — DASLLAMA_METAL_ATTN_SINGLE overrides for A/B (64 measured better than
// 128 in the 65-128 band: occupancy beats the comb overhead)
var g_attn_single_max = 64l
// the fused add+rms kernel's threadgroup row slab — wider models take the unfused pair
let ADD_RMS_MAX_DIM = 4096l

var g_dev : MetalDevice?
var g_queue : MetalCommandQueue?
var g_failed = false
var @scratch g_regions : table<uint64; MetalBuffer?>  // base address -> resident region; upload-once, steady-state [] is a hit
var g_pool : MetalBufferPool
var g_upool : MetalBufferPool                // UNTRACKED pool: uniforms + rope rows (GPU-read-only)
var g_decodes = 0l
var g_declines = 0l
var g_declines_by_reason : table<string; int64>
var g_logits_cpu = false
var g_skip = ""                              // DASLLAMA_METAL_DECODE_SKIP=gemv|attn|ew|moe_rt|moe_sh|qkv|wo knockout rail

var g_signs : MetalBuffer?                  // the tq4 rotation signs (128-float prefix rule)

// cumulative per-stage wall (us) since load — decode is per-token, so logging is on-demand
// (metal_decode_stage_report), never per step
var g_us_setup = 0l
var g_us_encode = 0l
var g_us_wait = 0l
var g_us_readback = 0l
var g_gpu_ms = 0.0lf
// the batched-step twins (per eval_batch step, not per token)
var g_batch_steps = 0l
var g_batch_rows = 0l
var g_batch_declines = 0l
var g_batch_declines_by_reason : table<string; int64>
var g_us_bsetup = 0l
var g_us_bencode = 0l
var g_us_bwait = 0l
var g_us_breadback = 0l
var g_gpu_b_ms = 0.0lf
var g_sched_b_ms = 0.0lf                     // driver scheduling span (kernelStart..kernelEnd)
var g_handoff_b_ms = 0.0lf                   // kernelEnd -> GPUStart gap

//! One retired single-stream decode step in the per-step trace (metal_decode_trace_arm) — host
//! stamps are us on the arm clock; the cb stamps are Metal host-time SECONDS on one shared
//! timeline across steps (gs - previous step's ge = GPU idle between the two cbs).
struct MetalDecodeStepTrace {
    pos : int64
    mode : int64        // 0 = slow (encode-now), 1 = poked pre-encode, 2 = spec chain
    commit_us : int64   // host stamp at this step's cb commit (spec: during the PREVIOUS step)
    finish_us : int64   // host stamp at step retire (wait + readback done)
    wait_us : int64     // the retire wait: commit-to-drain as the CPU saw it
    ks : double         // cb kernelStart/EndTime — the driver scheduling span
    ke : double
    gs : double         // cb GPUStart/EndTime — the execution window
    ge : double
}

var @scratch g_step_trace : array<MetalDecodeStepTrace>   // capacity-capped trace ring (g_step_trace_cap bounds it)
var g_step_trace_cap = 0l
var g_step_trace_t0 = 0l

// per-session logical KV mirror record (see module header). Mirrors are SLICES of one shared
// arena buffer (g_arena): the batched kernel reaches every row via a per-row offset table; the
// single-stream path binds at byte offsets. Fields copyable — slice returns to free list first.
struct KVMirror {
    uid : uint64
    kdt : KVDtype
    vdt : KVDtype
    cap_rows : int64      // logical rows allocated per layer (grows by rebuild + re-upload)
    watermark : int64     // logical rows [0, watermark) are mirrored; pos < watermark = rewind
    tick : int64          // LRU stamp
    koff : uint64         // arena byte offset of the K plane (n_layers slabs of cap_rows rows)
    voff : uint64         // ... and the V plane (koff + bytes/2 — one slice carries both)
    bytes : uint64        // slice size (k + v planes)
}

var @scratch g_mirrors : table<uint64; KVMirror>   // per-session mirrors; hot [] after key_exists is a hit
var g_mirror_bytes = 0ul
var g_tick = 0l
var g_mirror_cap_mb = -1   // <0 = unset: DASLLAMA_METAL_KV_MIRROR_MB (default 4096) reads at use
                           // — a CEILING (M2.7): the arena starts at 256MB and grows on demand

// the mirror arena: one buffer, first-fit free-list suballocation (slices are per-session and
// coarse, so fragmentation stays trivial; a failed fit evicts LRU mirrors and retries)
var g_arena : MetalBuffer?
var g_arena_bytes = 0ul
var g_arena_epoch = 0l   // bumps on every grow-rebuild — pre-encoded steps referencing the old buffer are stale
var @scratch g_arena_free : array<tuple<off : uint64; size : uint64>>   // sorted by off, coalesced

// ===== decode PSO + resource globals (declared here, not kernels.das, so [metal_dispatch]-generated
// builders — co-located with their kernel class, some in prefill.das — see the pipeline they
// dispatch; compile_pso() assignments + shutdown releases stay in kernels' metal_decode_init) =====
var g_pso_gemv : MetalComputePipeline?
var g_gemv_rows = 4l                       // rows (simdgroups) per GEMV threadgroup
var g_pso_rms : MetalComputePipeline?
var g_pso_add : MetalComputePipeline?
var g_pso_addrms : MetalComputePipeline?
var g_pso_preaddrms : MetalComputePipeline?
var g_pso_swiglu : MetalComputePipeline?
var g_pso_geglu : MetalComputePipeline?
var g_pso_softcap : MetalComputePipeline?
var g_pso_suppress : MetalComputePipeline?
var g_one : MetalBuffer?                    // persistent 1.0f — the rms oscale default bind
var g_zero : MetalBuffer?                   // persistent 0u — the sink/hass identity bind
var g_pso_attn16 : MetalComputePipeline?
var g_pso_attn32 : MetalComputePipeline?
var g_pso_attn_q8 : MetalComputePipeline?   // q8_0-mirror twin (dual-view block reads)
var g_pso_attn_tq4 : MetalComputePipeline?  // tq4-mirror twin (rotated-basis nibble reads)
var g_pso_attnp16 : MetalComputePipeline?
var g_pso_attnp32 : MetalComputePipeline?
var g_pso_attnp_q8 : MetalComputePipeline?
var g_pso_attnp_tq4 : MetalComputePipeline?
var g_pso_attncomb : MetalComputePipeline?
var g_pso_rpst16 : MetalComputePipeline?
var g_pso_rpst_h16 : MetalComputePipeline?  // head-cooperative fused bias+norm+rope+store (QK-norm models)
var g_pso_rpst32 : MetalComputePipeline?
var g_pso_rpst_q8 : MetalComputePipeline?   // per-32-block quantizing store
var g_pso_rpst_tq4 : MetalComputePipeline?  // threadgroup-per-head FWHT + quantizing store
var g_pso_unsign : MetalComputePipeline?    // the tq4 attention-output un-rotate
var g_pso_qknorm : MetalComputePipeline?    // per-head QK RMSNorm prepass (MSL owned by prefill)

var g_pso_argmax : MetalComputePipeline?
var g_pso_copy_row : MetalComputePipeline?   // zero-copy logits: staging row -> wrapped session storage
var g_pso_emb : MetalComputePipeline?
var g_pso_emb_k6 : MetalComputePipeline?     // tied-k6 spec chain's embed gather
var g_pso_moe_router : MetalComputePipeline?   // MoE (Wave C): router GEMV over the fp32 slab
var g_pso_moe_select : MetalComputePipeline?   // softmax + top-k + renorm (one simdgroup)
var g_pso_moe_gemv_q8 : MetalComputePipeline?  // expert-indexed GEMV twins (grid.y = slot)
var g_pso_moe_kq_k4 : MetalComputePipeline?
var g_pso_moe_kq_k5 : MetalComputePipeline?
var g_pso_moe_kq_k6 : MetalComputePipeline?
var g_pso_moe_combine : MetalComputePipeline?  // weighted top-k reduce into the w2 write shape
var g_pso_g4rnorm : MetalComputePipeline?      // gemma4 MoE router-input norm (stage 3)
var g_pso_moe_wscale : MetalComputePipeline?   // gemma4 per-expert down-scale fold
var g_pso_swiglu_oai : MetalComputePipeline?   // gpt-oss clamped swiglu (stage 4)
var g_pso_moe_gemv_mx4 : MetalComputePipeline?  // gpt-oss MXFP4 expert GEMV
var g_pso_moe_gemv_q51 : MetalComputePipeline?  // Q5_1 expert GEMV (per-32 planes)
var g_mx4_vtab : MetalBuffer?                   // [16] signed doubled-e2m1 values
var g_pso_dn_conv : MetalComputePipeline?      // deltanet (Wave D): causal conv + SiLU
var g_pso_dn_hist : MetalComputePipeline?      // conv-history advance
var g_pso_dn_l2 : MetalComputePipeline?        // q/k per-head L2 norm + q pre-scale
var g_pso_dn_scan : MetalComputePipeline?      // the fused sequential delta rule
var g_pso_dn_gate : MetalComputePipeline?      // z-gated out-norm
var g_pso_dn_deint : MetalComputePipeline?     // 2x-wide [q|gate] deinterleave
var g_pso_sigmul : MetalComputePipeline?       // x ⊙ σ(g) — the gated-attention out-gate
var g_pso_axpy_sig : MetalComputePipeline?     // y += x ⊙ σ(g[row]) — the shexp combine (stage 3)
var g_pso_qkvrs16 : MetalComputePipeline?   // fused QKV+rope+store (s16 path)
var g_pso_qkvrs32 : MetalComputePipeline?
var g_pso_w13sw : MetalComputePipeline?
var g_pso_kq_k4 : MetalComputePipeline?
var g_pso_kq_k5 : MetalComputePipeline?
var g_pso_kq_k5c : MetalComputePipeline?   // the select-form nr0=1 twin — n < 3072 / small-d sites
var g_pso_kq_k6 : MetalComputePipeline?
var g_pso_kq_mvb2_k4 : MetalComputePipeline?   // batch B=2..8: the kq ext small-batch mv twins
var g_pso_kq_mvb4_k4 : MetalComputePipeline?
var g_pso_kq_mvb2_k5 : MetalComputePipeline?
var g_pso_kq_mvb4_k5 : MetalComputePipeline?
var g_pso_kq_mvb2_k6 : MetalComputePipeline?
var g_pso_kq_mvb4_k6 : MetalComputePipeline?
var g_pso_kq_mvb8_k4 : MetalComputePipeline?   // B=5..8 single-pass twins (in-graph A/B vs 2 x B4 groups)
var g_pso_kq_mvb8_k5 : MetalComputePipeline?
var g_pso_kq_mvb8_k6 : MetalComputePipeline?
var g_pso_kq_mm_b4 : MetalComputePipeline?   // batch B>=9: the prefill-owned kq mul_mm twins
var g_pso_kq_mm_b5 : MetalComputePipeline?
var g_pso_kq_mm_b6 : MetalComputePipeline?     // fused W1|W3+swiglu (s16 path)
// the batched-step set (P4): fixed-B GEMVs + the M-pad-32 GEMM twin (f32 X) + row-table kernels
var g_pso_gemv_b2 : MetalComputePipeline?
var g_pso_gemv_b4 : MetalComputePipeline?
var g_pso_mv_b2 : MetalComputePipeline?
var g_pso_mv_b4 : MetalComputePipeline?
var g_pso_w13sw_b2 : MetalComputePipeline?
var g_pso_w13sw_b4 : MetalComputePipeline?
var g_pso_gemm_b : MetalComputePipeline?
var g_pso_gemm_b_sk : MetalComputePipeline? // split-K twin (per-site policy)
var g_pso_skred : MetalComputePipeline?     // the split-K plane reduce
var g_pso_gemm64_b : MetalComputePipeline?  // 64-wide N tile (classifier site)
var g_gemm_b_tensor : bool                  // crowned tensor twins selected (no tgmem bind)
var g_gemm_b_sk_tensor : bool
var g_gemm64_b_tensor : bool
var g_pso_mm : MetalComputePipeline?        // the prefill module's MetalQ8MulMm (34B blob)
var g_pso_attnb16 : MetalComputePipeline?
var g_pso_attnb32 : MetalComputePipeline?
var g_pso_attnb_q8 : MetalComputePipeline?
var g_pso_attnb_tq4 : MetalComputePipeline?
var g_pso_attnpb16 : MetalComputePipeline?
var g_pso_attnpb32 : MetalComputePipeline?
var g_pso_attnpb_q8 : MetalComputePipeline?
var g_pso_attnpb_tq4 : MetalComputePipeline?
var g_pso_attncombb : MetalComputePipeline?
var g_pso_attnd16 : MetalComputePipeline?   // fused single-pass batch attention (hs 128)
var g_pso_attnd32 : MetalComputePipeline?
var g_pso_attnd_q8 : MetalComputePipeline?
var g_pso_attnd_tq4 : MetalComputePipeline?
var g_pso_attncombd : MetalComputePipeline?
var g_pso_rpstb16 : MetalComputePipeline?
var g_pso_rpstb32 : MetalComputePipeline?
var g_pso_rpstb_q8 : MetalComputePipeline?
var g_pso_rpstb_tq4 : MetalComputePipeline?
var g_pso_addrmsb : MetalComputePipeline?
// uids a batch step is actively encoding against — eviction must never free a batch row's slice
// out from under the step being assembled (the single path's `keep` protects only one uid)
var g_pin : table<uint64>

def arena_alloc(size : uint64) : int64 {
    let sz = (size + 15ul) & ~15ul   // 16B-align every slice (buffer binds at byte offsets)
    for (i in range(length(g_arena_free))) {
        if (g_arena_free[i].size >= sz) {
            let off = g_arena_free[i].off
            if (g_arena_free[i].size == sz) {
                g_arena_free |> erase(i)
            } else {
                g_arena_free[i].off += sz
                g_arena_free[i].size -= sz
            }
            return int64(off)
        }
    }
    return -1l
}

def arena_free_slice(off : uint64; size : uint64) {
    let sz = (size + 15ul) & ~15ul
    var at = length(g_arena_free)
    for (i in range(length(g_arena_free))) {
        if (g_arena_free[i].off > off) {
            at = i
            break
        }
    }
    g_arena_free |> push((off = off, size = sz), at)
    // coalesce with the right neighbor, then the left
    if (at + 1 < length(g_arena_free) && g_arena_free[at].off + g_arena_free[at].size == g_arena_free[at + 1].off) {
        g_arena_free[at].size += g_arena_free[at + 1].size
        g_arena_free |> erase(at + 1)
    }
    if (at > 0 && g_arena_free[at - 1].off + g_arena_free[at - 1].size == g_arena_free[at].off) {
        g_arena_free[at - 1].size += g_arena_free[at].size
        g_arena_free |> erase(at)
    }
}

//! The KV-mirror cap (M2.7): the arena's grow-on-demand CEILING (clamps to 64..4096MB) AND the
//! RAW resident-bytes target for LRU eviction (tiny values = legitimate evict-aggressively policy).
//! Negative restores the env default (`DASLLAMA_METAL_KV_MIRROR_MB`, else 4096).
def public set_metal_decode_mirror_cap_mb(v : int) {
    g_mirror_cap_mb = v
}

//! Pin the CPU final-norm/classifier step (logits-on-GPU off) — the A/B and fallback rail.
//! `DASLLAMA_METAL_LOGITS=0` pins it process-wide (shared with the prefill driver).
def public set_metal_decode_logits_cpu(v : bool) {
    g_logits_cpu = v
}


// required-mode enforcement: a capability decline under MetalMode.required is a hard failure —
// the caller would silently fall back to the CPU path otherwise. Policy declines (the GPU
// could serve but chooses not to — prefill's npos_window) stay fallback-eligible in every mode.
def require_or_panic(kind : string; why : string) {
    if (get_metal_mode() == MetalMode.required) {
        panic("dasLLAMA: metal mode `required`, but {kind} declined: {why} (the GPU cannot serve this call)")
    }
}

// the failure leg's message fetch — hot code checks metal_command_buffer_failed (no alloc)
[cold_path]
def cb_error_text(cb : MetalCommandBuffer?) : string => metal_command_buffer_error(cb)

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def decline(why : MetalDecodeDecline) {
    g_declines++
    g_declines_by_reason["{why}"]++
    require_or_panic("decode", "{why}")
}

// the `feature` flavor: records a need_* key per missing bit, and the required-mode panic names them
[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def decline(why : MetalDecodeDecline; missing : MetalNeed) {
    g_declines++
    g_declines_by_reason["{why}"]++
    record_needs(g_declines_by_reason, missing)
    require_or_panic("decode", "{why} {missing}")
}

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def batch_decline(why : MetalDecodeDecline) {
    g_batch_declines++
    g_batch_declines_by_reason["{why}"]++
    require_or_panic("batch decode", "{why}")
}

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def batch_decline(why : MetalDecodeDecline; missing : MetalNeed) {
    g_batch_declines++
    g_batch_declines_by_reason["{why}"]++
    record_needs(g_batch_declines_by_reason, missing)
    require_or_panic("batch decode", "{why} {missing}")
}

//! (gpu decode steps served, declines to the CPU path) since load — diagnostics / tests.
def public metal_decode_stats() : tuple<decodes : int64; declines : int64> {
    return (decodes = g_decodes, declines = g_declines)
}

//! Single-stream declines-by-reason since load — reason name -> count (the first failing gate
//! clause per declined step). The support-matrix test asserts WHY a config declined.
def public metal_decode_declines() : table<string; int64> {
    return clone_to_move(g_declines_by_reason)
}

//! (batched eval_batch steps served, total rows across them, batch declines) since load.
def public metal_batch_decode_stats() : tuple<steps : int64; rows : int64; declines : int64> {
    return (steps = g_batch_steps, rows = g_batch_rows, declines = g_batch_declines)
}

//! Batched-step declines-by-reason since load — reason name -> count.
def public metal_batch_decode_declines() : table<string; int64> {
    return clone_to_move(g_batch_declines_by_reason)
}

// speculative-chain state: the pre-encoded next step may open with GPU argmax + embed gather off
// the current step's logits (no CPU poke). A greedy caller never misses; forced/non-greedy
// samplers miss every step, so adaptive mode backs off exponentially and re-arms on any hit.
var g_spec_mode = -1        // -1 adaptive (default), 0 off, 1 forced
var g_spec_cooldown = 0     // adaptive: served steps left before the next attempt
var g_spec_backoff = 1      // the next cooldown length
var g_spec_hits = 0l
var g_spec_misses = 0l

//! Speculative greedy chaining rail: -1 adaptive (default), 0 off, 1 forced-on (tests/A-B).
def public set_metal_decode_speculate(v : int) {
    g_spec_mode = v
    g_spec_cooldown = 0
    g_spec_backoff = 1
}

//! (speculative steps verified correct, mispredicted-and-rerun) since load.
def public metal_decode_spec_stats() : tuple<hits : int64; misses : int64> {
    return (hits = g_spec_hits, misses = g_spec_misses)
}

def spec_armed() : bool {
    if (g_spec_mode == 0) {
        return false
    }
    if (g_spec_mode == 1) {
        return true
    }
    if (g_spec_cooldown > 0) {
        g_spec_cooldown--
        return false
    }
    return true
}

//! The knockout rail as an in-process setter (timing attribution — served output is GARBAGE while
//! non-empty): `"gemv"` | `"attn"` | `"ew"`; `""` restores full math. Same rail as
//! `DASLLAMA_METAL_DECODE_SKIP`, switchable between measurement windows.
def public set_metal_decode_skip(v : string) {
    g_skip = v
}

//! Zero the cumulative per-stage timers + the served-step counter — windows a chase run's stage
//! stats to one measurement loop (declines keep accumulating; they gate support, not speed).
def public metal_decode_stage_reset {
    g_decodes = 0l
    g_us_setup = 0l
    g_us_encode = 0l
    g_us_wait = 0l
    g_us_readback = 0l
    g_gpu_ms = 0.0lf
    g_batch_steps = 0l
    g_batch_rows = 0l
    g_us_bsetup = 0l
    g_us_bencode = 0l
    g_us_bwait = 0l
    g_us_breadback = 0l
    g_gpu_b_ms = 0.0lf
    g_sched_b_ms = 0.0lf
    g_handoff_b_ms = 0.0lf
}

//! Cumulative batched-step stage wall since load/reset — the batch bench's numbers rail
//! (per STEP, one step = nrows tokens).
def public metal_batch_decode_stage_stats() : tuple<steps : int64; rows : int64; setup_us : int64; encode_us : int64; wait_us : int64; gpu_us : double; readback_us : int64> {
    return (steps = g_batch_steps, rows = g_batch_rows, setup_us = g_us_bsetup,
        encode_us = g_us_bencode, wait_us = g_us_bwait, gpu_us = g_gpu_b_ms * 1000.0lf,
        readback_us = g_us_breadback)
}

//! Lifetime dispatch-call count (dispatchThreadgroups + dispatchThreads, all rails) — read deltas
//! around a window; the dispatch-count instrument for the split-too-much question.
def public metal_dispatch_calls() : int64 => metal_dispatch_call_count()

//! Cumulative per-stage wall since load/reset — (served steps, CPU setup us, encode us, commit+wait
//! us, GPU-timestamp us within the wait, readback+writeback us). The chase harness's numbers rail.
def public metal_decode_stage_stats() : tuple<steps : int64; setup_us : int64; encode_us : int64; wait_us : int64; gpu_us : double; readback_us : int64> {
    return (steps = g_decodes, setup_us = g_us_setup, encode_us = g_us_encode,
        wait_us = g_us_wait, gpu_us = g_gpu_ms * 1000.0lf, readback_us = g_us_readback)
}

//! (live KV mirrors, resident mirror bytes) — the churn / LRU-cap gates read this.
def public metal_decode_mirror_stats() : tuple<mirrors : int64; bytes : uint64> {
    return (mirrors = long_length(g_mirrors), bytes = g_mirror_bytes)
}

//! Arm the per-step single-stream decode trace for the next `cap` retired steps (0 disarms) — the
//! bimodality diagnostic. Recording stops when full; batch/prefill steps are never recorded; a spec
//! mispredict retires TWICE at the same pos (the wrong-token spec entry, then the slow rerun).
def public metal_decode_trace_arm(cap : int64) {
    g_step_trace |> clear()
    g_step_trace |> reserve(int(cap))
    g_step_trace_cap = cap
    g_step_trace_t0 = ref_time_ticks()
}

//! Move out the recorded per-step trace and disarm.
def public metal_decode_trace_take() : array<MetalDecodeStepTrace> {
    g_step_trace_cap = 0l
    return <- g_step_trace
}

// " reason=count reason=count" — the declines-by-reason tail of a report line
def declines_line(tab : table<string; int64>) : string {
    return build_string() $(var w) {
        for (k, v in keys(tab), values(tab)) {
            w |> write(" ")
            w |> write(k)
            w |> write("=")
            w |> write(v)
        }
    }
}

//! One cumulative per-stage timing line for the steps served since load (decode is per-token, so
//! the driver never logs per step) + declines-by-reason for all three GPU paths — the decline
//! tables surface HERE even when zero steps served (the silent-decline case this report exists for).
def public metal_decode_stage_report {
    if (g_decodes == 0l && g_batch_steps == 0l) {
        to_log(LOG_INFO, "dasLLAMA metal decode: no steps served\n")
    }
    if (g_decodes > 0l) {
        let n = float(g_decodes)
        to_log(LOG_INFO, "dasLLAMA metal decode: {g_decodes} steps  per-step setup={float(g_us_setup) / n}us encode={float(g_us_encode) / n}us wait={float(g_us_wait) / n}us (gpu {g_gpu_ms * 1000.0lf / double(g_decodes)}us) readback+writeback={float(g_us_readback) / n}us\n")
    }
    if (g_batch_steps > 0l) {
        let n = float(g_batch_steps)
        to_log(LOG_INFO, "dasLLAMA metal batch decode: {g_batch_steps} steps ({g_batch_rows} rows)  per-step setup={float(g_us_bsetup) / n}us encode={float(g_us_bencode) / n}us wait={float(g_us_bwait) / n}us (sched {g_sched_b_ms * 1000.0lf / double(g_batch_steps)}us handoff {g_handoff_b_ms * 1000.0lf / double(g_batch_steps)}us gpu {g_gpu_b_ms * 1000.0lf / double(g_batch_steps)}us) readback+writeback={float(g_us_breadback) / n}us\n")
    }
    if (g_declines > 0l) {
        to_log(LOG_INFO, "dasLLAMA metal decode declines: {g_declines} —{declines_line(g_declines_by_reason)}\n")
    }
    if (g_batch_declines > 0l) {
        to_log(LOG_INFO, "dasLLAMA metal batch decode declines: {g_batch_declines} —{declines_line(g_batch_declines_by_reason)}\n")
    }
    if (g_prefill_declines > 0l) {
        to_log(LOG_INFO, "dasLLAMA metal prefill declines: {g_prefill_declines} —{declines_line(g_prefill_declines_by_reason)}\n")
    }
}


def mirror_release(var mr : KVMirror) {
    if (mr.bytes > 0ul) {
        arena_free_slice(mr.koff, mr.bytes)
    }
    g_mirror_bytes -= mr.bytes
    mr.bytes = 0ul
}

//! Release every GPU object the driver holds — mirrors, weight regions, pools, PSOs, queue,
//! device (the tests' leak-gate rail, same contract as metal_prefill_shutdown). Lazily
//! re-initializes on the next served step; all mirrors rebuild from the CPU caches on demand.


def upload_region(src : void?; bytes : uint64) : MetalBuffer? {
    let key = intptr(src)
    if (key_exists(g_regions, key)) {
        return g_regions[key]
    }
    var buf = metal_new_buffer_untracked(g_dev, bytes)
    unsafe {
        memcpy(metal_buffer_contents(buf), src, bytes)
    }
    g_regions[key] = buf
    return buf
}

// lazily upload up to three CONSECUTIVE-ROW regions as ONE resident buffer, keyed by the first
// piece's base address — the fused QKV / W1W3 GEMV streams (one dispatch, continuous row
// indexing). Load-once-stable like upload_region; b2 == 0 makes it a two-piece concat.
def upload_region_cat3(p0 : void?; b0 : uint64; p1 : void?; b1 : uint64; p2 : void?; b2 : uint64) : MetalBuffer? {
    let key = intptr(p0)
    if (key_exists(g_regions, key)) {
        return g_regions[key]
    }
    var buf = metal_new_buffer_untracked(g_dev, b0 + b1 + b2)
    unsafe {
        var d = reinterpret<uint8?>(metal_buffer_contents(buf))
        memcpy(reinterpret<void?>(d), p0, b0)
        memcpy(reinterpret<void?>(d + b0), p1, b1)
        if (b2 > 0ul) {
            memcpy(reinterpret<void?>(d + (b0 + b1)), p2, b2)
        }
    }
    g_regions[key] = buf
    return buf
}

// ===== the whole-plane resident buffers (blob-only models) =====
// Each GPU-ready plane becomes ONE resident buffer (zero-copy page wrap when image-mapped, else a
// one-time untracked copy); tensors bind at byte offsets, keeping in-kernel indices REGION-relative — a 13GB blob's absolute byte indices overflow the kernels' uint32 index math.

def private plane_buffer(dev : MetalDevice?; base : void?; bytes : uint64; mapped : bool) : MetalBuffer? {
    let key = intptr(base)
    if (key_exists(g_regions, key)) {
        return g_regions[key]
    }
    var buf : MetalBuffer?
    if (mapped) {
        // the rounded-up tail is only guaranteed mapped for image sections (page-aligned, file pads to a page multiple) — owned planes always copy
        buf = unsafe(metal_new_buffer_no_copy_untracked(dev, base, (bytes + 16383ul) & ~16383ul))
    } else {
        buf = metal_new_buffer_untracked(dev, bytes)
        unsafe {
            var d = reinterpret<uint8?>(metal_buffer_contents(buf))
            let s = reinterpret<uint8 const?>(base)
            var at = 0l
            while (at < int64(bytes)) {   // memcpy takes int — chunk the >2GB planes
                let chunk = min(int64(bytes) - at, 1073741824l)
                memcpy(reinterpret<void?>(d + at), reinterpret<void const?>(s + at), chunk)
                at += chunk
            }
        }
    }
    g_regions[key] = buf
    return buf
}

//! The whole-model 34B-block q8 blob buffer (metal_blob models) + one tensor's byte offset
//! within it — bind BOTH blob views (half-scale + byte) at this offset. Layout gate at the
//! transform keeps every offset 16B-aligned.
def blob_of(dev : MetalDevice?; t : Model; off : int64) : tuple<buf : MetalBuffer?; boff : uint64> {
    assert(t.metal_blob)   // planar models decline at the gates — they never reach an upload
    unsafe {
        return (buf = plane_buffer(dev, addr < void? >(t.mblob[0]), uint64(long_length(t.mblob)), t.image_map != null),
                boff = uint64((off / 32l) * 34l))
    }
}

//! The kept-bf16 plane (E-series model_proj) + one tensor's byte offset within it — zero-copy
//! page wrap on image-mapped models, one-time resident copy otherwise (cached per base).
def bf16_of(dev : MetalDevice?; t : Model; off : int64) : tuple<buf : MetalBuffer?; boff : uint64> {
    unsafe {
        return (buf = plane_buffer(dev, addr < void? >(t.bf16blob[0]), uint64(long_length(t.bf16blob) * 2l), t.image_map != null),
                boff = uint64(off * 2l))
    }
}

//! The native-MXFP4 expert planes (gpt-oss) + one stack's byte offsets within them: nibble
//! bytes at off/2, E8M0 scale bytes at off/32 (disk order — the mx4_repacked gate keeps the
//! CPU laneq repack out). Region-relative uint32 indices stay safe per layer stack.
def mx4_of(dev : MetalDevice?; t : Model; off : int64) : tuple<qbuf : MetalBuffer?; qoff : uint64; sbuf : MetalBuffer?; soff : uint64> {
    assert(t.metal_blob)
    unsafe {
        return (qbuf = plane_buffer(dev, addr < void? >(t.mxq[0]), uint64(long_length(t.mxq)), t.image_map != null),
                qoff = uint64(off / 2l),
                sbuf = plane_buffer(dev, addr < void? >(t.mxs[0]), uint64(long_length(t.mxs)), t.image_map != null),
                soff = uint64(off / 32l))
    }
}


// ===== zero-copy logits: bytesNoCopy wrappers over session-owned storage =====
// Writes land straight into s.logits (no landing memcpy); borrowed, no deallocator. Gated on
// 16KB page alignment + page-rounded capacity; the pointer is revalidated on every lookup.
var @scratch g_nc_logits : table<uint64; tuple<buf : MetalBuffer?; ptr : uint64>>

def nc_logits_for(dev : MetalDevice?; s : Session) : MetalBuffer? {
    let p : uint64 = intptr(unsafe(addr(s.logits[0])))
    if (key_exists(g_nc_logits, s.uid)) {
        if (g_nc_logits[s.uid].ptr == p) {
            return g_nc_logits[s.uid].buf
        }
        var stale = g_nc_logits[s.uid].buf
        if (stale != null) {
            metal_release(stale)
        }
        g_nc_logits |> erase(s.uid)
    }
    let bytes = (uint64(long_length(s.logits)) * 4ul + 16383ul) & ~16383ul
    var buf : MetalBuffer?
    if ((p & 16383ul) == 0ul && uint64(long_capacity(s.logits)) * 4ul >= bytes) {
        unsafe {
            buf = metal_new_buffer_no_copy(dev, reinterpret<void?>(p), bytes)
        }
    }
    g_nc_logits[s.uid] = (buf = buf, ptr = p)
    return buf
}

def nc_logits_release {
    for (e in values(g_nc_logits)) {
        if (e.buf != null) {
            metal_release(e.buf)
            e.buf = null
        }
    }
    unsafe {
        delete g_nc_logits
    }
}

//! One kq tensor's quant plane bind: the whole disk-layout plane as one resident buffer + the
//! tensor's byte offset (superblock payloads verbatim — the backend_repack decline excludes
//! grp-repacked loads). Serves BOTH resident drivers (prefill passes its own device handle).
def kq_quants_of(dev : MetalDevice?; t : Model; fmt : KqFmt; off : int64) : tuple<buf : MetalBuffer?; qoff : uint64> {
    assert(t.metal_blob)
    let sb0 = off / 256l
    unsafe {
        if (fmt == KqFmt.k4) {
            return (buf = plane_buffer(dev, addr < void? >(t.k4q[0]), uint64(long_length(t.k4q)), t.image_map != null),
                    qoff = uint64(sb0 * 128l))
        }
        if (fmt == KqFmt.k5) {
            return (buf = plane_buffer(dev, addr < void? >(t.k5q[0]), uint64(long_length(t.k5q)), t.image_map != null),
                    qoff = uint64(sb0 * 160l))
        }
        return (buf = plane_buffer(dev, addr < void? >(t.k6q[0]), uint64(long_length(t.k6q)), t.image_map != null),
                qoff = uint64(sb0 * 192l))
    }
}

//! One kq tensor's GPU-form scale bind (the gemv lab's layouts, pre-baked by the blob
//! transform): k4/k5 = 16B compact blocks at soff; k6 = the split plane — sub-scale strips at
//! soff, the packed f16 d plane at doff (the kernels bind the buffer twice, once per offset).
def kq_scales_of(dev : MetalDevice?; t : Model; fmt : KqFmt; off : int64) : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64> {
    assert(t.metal_blob)
    let sb0 = off / 256l
    unsafe {
        if (fmt == KqFmt.k6) {
            let plane_sb = long_length(t.k6s) / 18l   // split form keeps the 18B/sb total
            return (buf = plane_buffer(dev, addr < void? >(t.k6s[0]), uint64(long_length(t.k6s)), t.image_map != null),
                    soff = uint64(sb0 * 16l), doff = uint64(plane_sb * 16l + sb0 * 2l))
        }
        if (fmt == KqFmt.k4) {
            return (buf = plane_buffer(dev, addr < void? >(t.k4s[0]), uint64(long_length(t.k4s)), t.image_map != null),
                    soff = uint64(sb0 * 16l), doff = 0ul)
        }
        return (buf = plane_buffer(dev, addr < void? >(t.k5s[0]), uint64(long_length(t.k5s)), t.image_map != null),
                soff = uint64(sb0 * 16l), doff = 0ul)
    }
}

//! One q51 tensor's plane pair bind: quant plane (20B/block, uint-viewable) at (off/32)*20 and
//! the f16 d+m scale plane at (off/32)*4 — both verbatim disk splits, never rebaked (the blob
//! transform leaves q51 planes untouched). off % 128 keeps both binds 16B-aligned.
def q51_planes_of(dev : MetalDevice?; t : Model; off : int64) : tuple<qbuf : MetalBuffer?; qoff : uint64; sbuf : MetalBuffer?; soff : uint64> {
    assert(t.metal_blob)
    let b0 = off / 32l
    unsafe {
        return (qbuf = plane_buffer(dev, addr < void? >(t.q51q[0]), uint64(long_length(t.q51q)), t.image_map != null),
                qoff = uint64(b0 * 20l),
                sbuf = plane_buffer(dev, addr < void? >(t.q51s[0]), uint64(long_length(t.q51s)), t.image_map != null),
                soff = uint64(b0 * 4l))
    }
}

def uniform_u32(v : uint) : MetalBuffer? {
    var buf = pool_acquire_untracked(g_upool, g_dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def uniform_f32(v : float) : MetalBuffer? {
    var buf = pool_acquire_untracked(g_upool, g_dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

// only the llama-family std shape is wired — the same decline set as prefill_decline minus the
// npos window (the model/config half, shared by the single-stream and batched drivers). Returns
// the FIRST failing clause; .none = supported.

def mirror_evict_to_cap(cap_bytes : uint64; keep : uint64) {
    while (g_mirror_bytes > cap_bytes) {
        var victim = 0ul
        var vtick = 0l
        for (uid, mr in keys(g_mirrors), values(g_mirrors)) {
            if (uid != keep && !key_exists(g_pin, uid) && (victim == 0ul || mr.tick < vtick)) {
                victim = uid
                vtick = mr.tick
            }
        }
        if (victim == 0ul) {   // only the live session's mirror remains
            return
        }
        mirror_release(g_mirrors[victim])
        g_mirrors |> erase(victim)
    }
}


// Hetero-safe per-layer mirror addressing (gemma4's two kv-head classes): every KV codec is
// linear bytes-per-element for kvd % 32 == 0, so the layout_offsets element prefix scales
// straight to bytes. Uniform models reduce to the old l * cap * rowb addresses exactly.
def mirror_row_total(t : Model) : int64 {
    let c = t.config
    var tot = 0l
    for (l in range64(c.n_layers + c.n_layer_nextn)) {   // + the MTP draft slab (trunk-shaped attention)
        if (!kv_shared_layer(t, l)) {   // shared layers (E-series) alias their source's slab
            tot += layer_kv_dim(c, l)
        }
    }
    return tot
}

//! layer l's slab base (bytes) within a mirror K or V plane of cap rows
def mirror_lbase(t : Model; dt : KVDtype; cap, l : int64) : int64 => cap * kv_row_bytes(dt, t.kv_row_prefix[l])

// the slice mirror_prepare would build for (s, pos): chunked high-water cap (pow2 from 256,
// never past the session's own seq cap) and the K/V plane sizes
def private mirror_slice_shape(t : Model; s : Session; pos : int64) : tuple<cap : int64; kbytes : uint64; vbytes : uint64> {
    let needed = pos + 1l
    var cap = 256l
    while (cap < needed) {
        cap *= 2l
    }
    cap = clamp(cap, needed, t.config.seq_len)
    let total = mirror_row_total(t)
    return (cap = cap, kbytes = uint64(cap * kv_row_bytes(s.kv_dtype_k, total)),
            vbytes = uint64(cap * kv_row_bytes(s.kv_dtype_v, total)))
}

def private mirror_ceiling_bytes : uint64 {
    let ceil_mb = uint64(clamp(g_mirror_cap_mb >= 0 ? g_mirror_cap_mb : g_mirror_cap_env, 64, 4096))
    return ceil_mb * 1024ul * 1024ul
}

// release-all rebuild at `want` (clamped to the ceiling): every mirror re-uploads via its
// watermark, and the epoch bump invalidates any pre-encoded step whose kernels reference the
// OLD arena buffer. Callers must ensure nothing is pinned and no step is in flight.
[cold_path]   // arena (re)construction — the grow event, not the per-step path
def private arena_rebuild(want : uint64; ceil_bytes : uint64) {
    for (mr in values(g_mirrors)) {
        mirror_release(mr)
    }
    unsafe {
        delete g_mirrors
    }
    g_mirror_bytes = 0ul
    metal_release(g_arena)
    g_arena_bytes = min(want, ceil_bytes)
    g_arena = metal_new_buffer(g_dev, g_arena_bytes)
    g_arena_free |> clear()
    g_arena_free |> push((off = 0ul, size = g_arena_bytes))
    g_arena_epoch++
    to_log(LOG_INFO, "dasLLAMA metal decode: KV mirror arena grown to {int(g_arena_bytes / (1024ul * 1024ul))}MB (ceiling {int(ceil_bytes / (1024ul * 1024ul))}MB)\n")   // nolint:PERF028,PERF026 — arena growth is rare and the upload it announces dwarfs the log
}

// the arena bytes session s will occupy after mirror_prepare(t, s, pos): its existing slice
// when capacity + dtypes already fit, else the rebuilt slice's size
def mirror_arena_demand(t : Model; s : Session; pos : int64) : uint64 {
    if (key_exists(g_mirrors, s.uid)) {
        let mr0 = g_mirrors[s.uid]
        if (mr0.cap_rows >= pos + 1l && mr0.kdt == s.kv_dtype_k && mr0.vdt == s.kv_dtype_v) {
            return mr0.bytes
        }
    }
    let shp = mirror_slice_shape(t, s, pos)
    return shp.kbytes + shp.vbytes
}

// grow the arena until it can hold `total` bytes of slices — the batch driver reserves its
// WHOLE step's demand through this before pinning any row (growth is a release-all rebuild,
// forbidden under pins). false = total exceeds the ceiling (a genuine capability limit).
def mirror_reserve(total : uint64) : bool {
    let ceil_bytes = mirror_ceiling_bytes()
    if (total > ceil_bytes) {
        return false
    }
    if (g_arena == null) {
        g_arena_bytes = clamp(256ul * 1024ul * 1024ul, total, ceil_bytes)   // total <= ceiling here
        g_arena = metal_new_buffer(g_dev, g_arena_bytes)
        g_arena_free |> clear()
        g_arena_free |> push((off = 0ul, size = g_arena_bytes))
        return true
    }
    if (g_arena_bytes >= total) {
        return true
    }
    if (!empty(g_pin)) {
        return false
    }
    var want = g_arena_bytes
    while (want < total) {
        want *= 2ul
    }
    arena_rebuild(want, ceil_bytes)
    return true
}

// release-all rebuild sized for `total` — the batch driver's fragmentation fallback when the
// per-session prepares can't pack a step that DOES fit capacity-wise (exact-fit demand around
// older residents can strand contiguity). Callers unpin first; false = past the ceiling.
[cold_path]   // arena defrag = the occasional rebuild leg
def mirror_defrag(total : uint64) : bool {
    let ceil_bytes = mirror_ceiling_bytes()
    if (total > ceil_bytes || !empty(g_pin) || g_arena == null) {
        return false
    }
    arena_rebuild(max(g_arena_bytes, total), ceil_bytes)
    return true
}

// Ensure s.uid's mirror exists, is big enough, and holds logical rows [0, pos) — the watermark
// contract (module header). Returns arena byte offsets; caller writes row `pos` GPU-side and
// advances the watermark only after the step + writeback complete. ok=false ⇒ decline to CPU.
def mirror_prepare(t : Model; var s : Session; pos : int64) : tuple<ok : bool; koff : uint64; voff : uint64; cap : int64> {
    let c = t.config
    let needed = pos + 1l
    if (key_exists(g_mirrors, s.uid)) {
        // uid is never reused, so dtype can't legitimately change — a mismatch means a stale
        // record; capacity growth rebuilds too (watermark 0 → full re-upload from the CPU cache)
        let mr0 = g_mirrors[s.uid]   // cheap copy — fields tested off it, one lookup per statement
        if (mr0.cap_rows < needed || mr0.kdt != s.kv_dtype_k || mr0.vdt != s.kv_dtype_v) {
            mirror_release(g_mirrors[s.uid])
            g_mirrors |> erase(s.uid)
        }
    }
    if (!key_exists(g_mirrors, s.uid)) {
        // the mirror cap is a CEILING (M2.7): grows on demand to clamp(cap, 64, 4096)MB — 4096 is a
        // HARD max (the batch row table's uint32 arena offsets need arena size <= 2^32). Growth
        // releases every mirror and rebuilds (re-mirror cost only); never runs while pins are held.
        let ceil_bytes = mirror_ceiling_bytes()
        if (g_arena == null) {
            g_arena_bytes = min(256ul * 1024ul * 1024ul, ceil_bytes)
            g_arena = metal_new_buffer(g_dev, g_arena_bytes)
            g_arena_free |> clear()
            g_arena_free |> push((off = 0ul, size = g_arena_bytes))
        }
        let shp = mirror_slice_shape(t, s, pos)
        let cap = shp.cap
        let kbytes = shp.kbytes
        let vbytes = shp.vbytes
        var off = arena_alloc(kbytes + vbytes)
        while (off < 0l) {   // arena full — evict LRU until the slice fits (or nothing evictable)
            var victim = 0ul
            var vtick = 0l
            for (uid, mr in keys(g_mirrors), values(g_mirrors)) {
                if (uid != s.uid && !key_exists(g_pin, uid) && (victim == 0ul || mr.tick < vtick)) {
                    victim = uid
                    vtick = mr.tick
                }
            }
            if (victim == 0ul) {
                // nothing left to evict — grow toward the ceiling, unless a batch step holds
                // pinned rows (the batch driver's mirror_reserve pre-grows for that case)
                if (!empty(g_pin) || g_arena_bytes >= ceil_bytes || kbytes + vbytes > ceil_bytes) {
                    return (ok = false, koff = 0ul, voff = 0ul, cap = 0l)
                }
                arena_rebuild(max(g_arena_bytes * 2ul, kbytes + vbytes), ceil_bytes)
            } else {
                mirror_release(g_mirrors[victim])
                g_mirrors |> erase(victim)
            }
            off = arena_alloc(kbytes + vbytes)
        }
        let mr = KVMirror(uid = s.uid, kdt = s.kv_dtype_k, vdt = s.kv_dtype_v, cap_rows = cap,
            koff = uint64(off), voff = uint64(off) + kbytes, bytes = kbytes + vbytes)
        g_mirror_bytes += mr.bytes
        g_mirrors[s.uid] = mr
        // the evict target honors the RAW knob (a tiny cap is legitimate idle-eviction policy —
        // the LRU churn tests rely on it); only the arena GROWTH ceiling carries the 64MB floor
        let cap_mb = uint64(g_mirror_cap_mb >= 0 ? g_mirror_cap_mb : g_mirror_cap_env)
        mirror_evict_to_cap(cap_mb * 1024ul * 1024ul, s.uid)
    }
    g_tick++
    g_mirrors[s.uid].tick = g_tick
    if (pos < g_mirrors[s.uid].watermark) {   // rewind detected → re-mirror from scratch
        g_mirrors[s.uid].watermark = 0l
    }
    let wm = g_mirrors[s.uid].watermark
    let cap = g_mirrors[s.uid].cap_rows
    let koff = g_mirrors[s.uid].koff
    let voff = g_mirrors[s.uid].voff
    if (wm < pos) {
        // gather logical rows [wm, pos) from the CPU cache (source of truth) — flat sessions are
        // one run, paged sessions gather through kv_runs; bytes move verbatim (same dtype)
        let nrun = kv_runs(s, wm, pos, s.kv_runs)
        unsafe {
            var kdst = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + koff
            var vdst = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + voff
            for (l in range64(c.n_layers + c.n_layer_nextn)) {   // draft slab rides the watermark contract
                continue if (kv_shared_layer(t, l))   // aliases the source slab — its upload covers l
                let kvd_l = layer_kv_dim(c, l)
                let krow = kv_row_bytes(s.kv_dtype_k, kvd_l)
                let vrow = kv_row_bytes(s.kv_dtype_v, kvd_l)
                let klb = mirror_lbase(t, s.kv_dtype_k, cap, l)
                let vlb = mirror_lbase(t, s.kv_dtype_v, cap, l)
                let kbase = kv_layer_kbase(s, l, c.seq_len)
                let vbase = kv_layer_vbase(s, l, c.seq_len)
                for (r in range64(nrun)) {
                    let j0 = s.kv_runs[r].j0
                    let nrows = s.kv_runs[r].j1 - j0
                    let prow = s.kv_runs[r].row0
                    memcpy(reinterpret<void?>(kdst + klb + j0 * krow),
                        reinterpret<void?>(kbase + prow * krow), nrows * krow)
                    memcpy(reinterpret<void?>(vdst + vlb + j0 * vrow),
                        reinterpret<void?>(vbase + prow * vrow), nrows * vrow)
                }
            }
        }
        g_mirrors[s.uid].watermark = pos
    }
    return (ok = true, koff = koff, voff = voff, cap = cap)
}



// ===== deltanet recurrent-state mirrors (Wave D) — per-session GPU S + conv state =====
// Forward-only, NOT re-derivable: pos 0 zeroes, s.dn_pos == pos syncs from CPU, else decline;
// own buffers (KV-arena rebuild re-uploads from CPU — impossible for state); layout == Session's.

struct DnMirror {
    uid : uint64
    @do_not_delete buf : MetalBuffer?   // regions x [state planes f32][conv planes f32]
    conv_off : uint64                   // byte offset of the conv-history region within a region
    region_bytes : uint64
    regions : int64                     // 2 for MTP-verify dual-store (cur flips on reject), else 1
    cur : int64                         // live region index
    bytes : uint64                      // regions * region_bytes
    pos : int64                         // next position the GPU state expects (forward-only)
    tick : int64
}
var @scratch g_dn_mirrors : table<uint64; DnMirror>   // per-session; hot [] after key_exists is a hit
let DN_MIRRORS_MAX = 4      // per-session state is big (35B: ~60MB) — oldest evicts; a returning
                            // evicted session declines unless it restarts at pos 0 or off CPU state

// MTP-capable models verify at B=2 with dual-store — their mirrors carry the shadow region
def dn_mirror_regions(t : Model) : int64 => t.config.n_layer_nextn > 0l ? 2l : 1l

def dn_mirror_release(mr : DnMirror) {
    if (mr.buf != null) {
        metal_release(mr.buf)
    }
}

def dn_mirror_prepare(var s : Session; pos : int64; regions : int64) : tuple<ok : bool; buf : MetalBuffer?; state_off : uint64; conv_off : uint64; state_alt : uint64; conv_alt : uint64> {
    let sbytes = uint64(long_length(s.dn_state)) * 4ul
    let cbytes = uint64(long_length(s.dn_conv_state)) * 4ul
    let rb = sbytes + cbytes
    let total = rb * uint64(regions)
    if (sbytes == 0ul) {
        return (ok = false, buf = null, state_off = 0ul, conv_off = 0ul, state_alt = 0ul, conv_alt = 0ul)
    }
    if (key_exists(g_dn_mirrors, s.uid) && g_dn_mirrors[s.uid].bytes != total) {
        dn_mirror_release(g_dn_mirrors[s.uid])
        g_dn_mirrors |> erase(s.uid)
    }
    let have = key_exists(g_dn_mirrors, s.uid)
    let cpu_prefix = s.dn_pos == pos
    if (!have && pos != 0l && !cpu_prefix) {
        return (ok = false, buf = null, state_off = 0ul, conv_off = 0ul, state_alt = 0ul, conv_alt = 0ul)
    }
    if (!have) {
        while (length(g_dn_mirrors) >= DN_MIRRORS_MAX) {
            var victim = 0ul
            var vtick = 0l
            for (uid, mr in keys(g_dn_mirrors), values(g_dn_mirrors)) {
                if (victim == 0ul || mr.tick < vtick) {
                    victim = uid
                    vtick = mr.tick
                }
            }
            dn_mirror_release(g_dn_mirrors[victim])
            g_dn_mirrors |> erase(victim)
        }
        g_dn_mirrors[s.uid] = DnMirror(uid = s.uid, buf = metal_new_buffer(g_dev, total),
            conv_off = sbytes, region_bytes = rb, regions = regions, cur = 0l, bytes = total, pos = -1l)
    }
    if (pos == 0l) {
        // fresh conversation: zero every region (dn_reset's GPU half). Safe on unified memory —
        // no step of this session is in flight at pos 0 (spec is clipped for recurrent models)
        unsafe {
            memset8(metal_buffer_contents(g_dn_mirrors[s.uid].buf), uint8(0), int(total))
        }
        g_dn_mirrors[s.uid].cur = 0l
        g_dn_mirrors[s.uid].pos = 0l
    } elif (g_dn_mirrors[s.uid].pos != pos) {
        if (!cpu_prefix) {
            return (ok = false, buf = null, state_off = 0ul, conv_off = 0ul, state_alt = 0ul, conv_alt = 0ul)
        }
        // the CPU ran positions [0, pos) — its state is the source of truth; sync it into region 0
        unsafe {
            var dst = reinterpret<uint8?>(metal_buffer_contents(g_dn_mirrors[s.uid].buf))
            memcpy(reinterpret<void?>(dst), addr<void?>(s.dn_state[0]), sbytes)
            memcpy(reinterpret<void?>(dst + sbytes), addr<void?>(s.dn_conv_state[0]), cbytes)
        }
        g_dn_mirrors[s.uid].cur = 0l
        g_dn_mirrors[s.uid].pos = pos
    }
    g_tick++
    g_dn_mirrors[s.uid].tick = g_tick
    var mr = g_dn_mirrors[s.uid]   // cheap copy — one lookup per statement (table re-hash safety)
    let base = uint64(mr.cur) * mr.region_bytes
    let alt = mr.regions == 2l ? uint64(mr.cur ^ 1l) * mr.region_bytes : base
    return (ok = true, buf = mr.buf, state_off = base, conv_off = base + mr.conv_off,
        state_alt = alt, conv_alt = alt + mr.conv_off)
}

// the step served and its command buffer completed — the GPU state now holds position pos's update
def dn_mirror_advance(uid : uint64; pos : int64) {
    if (key_exists(g_dn_mirrors, uid)) {
        g_dn_mirrors[uid].pos = pos + 1l
    }
}

// MTP verify reject: the shadow region holds the post-row0 state — make it live (zero copies)
def dn_mirror_flip(uid : uint64; pos : int64) {
    if (key_exists(g_dn_mirrors, uid) && g_dn_mirrors[uid].regions == 2l) {
        g_dn_mirrors[uid].cur ^= 1l
        g_dn_mirrors[uid].pos = pos
    }
}

[cold_path]   // one-time: fills the 512B FWHT sign table on first touch
def private signs_buffer_build() {
    var signs : array<float>
    tq4_fill_signs(signs, 128l)
    g_signs = metal_new_buffer(g_dev, 512ul)
    unsafe {
        memcpy(metal_buffer_contents(g_signs), addr < void? >(signs[0]), 512)
    }
    delete signs
}

def signs_buffer() : MetalBuffer? {
    if (g_signs == null) {
        signs_buffer_build()
    }
    return g_signs
}

// tq4-mirror rope+store: one threadgroup per head (the FWHT spans it); mirror rows bind twice
// each (half-scale + byte views at the same row offset)

// one MoE weight stack's (fmt, offset) serve check: base offset = the format's BIND alignment
// (q8/k4/k5 %256, k6's d-plane pair %512); per-expert stride = the in-kernel index unit



// ===== the range tracker (the dispatch port; since P3 the batch rail's only hazard system) =====
// lcpp ggml_mem_ranges-shaped, finer: exact (buffer, off, len) ranges over mutable FRAME buffers
// only; armed only while the single-stream concurrent encoder builds (others no-op the staging)

struct MemRange {
    buf : uint64    // intptr of the MetalBuffer handle (arena/pool handles are load-stable)
    p0 : uint64
    p1 : uint64
    dst : bool
}

let HZ_WHOLE = 0xFFFFFFFFFFFFFFFFul   // whole-buffer p1 — for binds whose span lives in a uniform

var g_hz_on = false                 // gating armed for the encoder being built
var g_hz_paranoid = false           // DASLLAMA_METAL_HAZARD_PARANOID: barrier every dispatch
var private g_pipe_debug = false    // DASLLAMA_METAL_PIPE_DEBUG, resolved once — the read sits in the per-step path
var private g_mirror_cap_env = 4096   // DASLLAMA_METAL_KV_MIRROR_MB, resolved at init; the default stands until then
var g_hz_strict = false             // DASLLAMA_METAL_HAZARD_STRICT: undeclared dispatch panics
var g_hz_dirty = false              // an undeclared dispatch ran — the next gate must barrier
var @scratch g_hz_stage : array<MemRange>    // the next dispatch's declared ranges (clear()-recycled)
var @scratch g_hz_pend : array<MemRange>     // unbarriered ranges live on the encoder (clear()-recycled)
var g_hz_barriers = 0l              // lifetime barrier count (chase prints the per-step delta)
var g_hz_gated = 0l                 // lifetime gated dispatches — the coverage check's twin
var g_hz_undeclared = 0l            // conservative fallbacks taken (0 once migration completes)
var g_hz_cover_warned = false
var g_hz_g0 = 0l                    // g_hz_gated at the last hz_arm — per-encoder dispatch index

def hz_read(b : MetalBuffer?; off, len : uint64) {
    if ((g_hz_on || g_gr_on) && b != null) {
        g_hz_stage |> push(MemRange(buf = intptr(b), p0 = off, p1 = off + len, dst = false))
    }
}

def hz_read(b : MetalBuffer?) {
    if ((g_hz_on || g_gr_on) && b != null) {
        g_hz_stage |> push(MemRange(buf = intptr(b), p0 = 0ul, p1 = HZ_WHOLE, dst = false))
    }
}

def hz_write(b : MetalBuffer?; off, len : uint64) {
    if ((g_hz_on || g_gr_on) && b != null) {
        g_hz_stage |> push(MemRange(buf = intptr(b), p0 = off, p1 = off + len, dst = true))
    }
}

def hz_write(b : MetalBuffer?) {
    if ((g_hz_on || g_gr_on) && b != null) {
        g_hz_stage |> push(MemRange(buf = intptr(b), p0 = 0ul, p1 = HZ_WHOLE, dst = true))
    }
}

// the per-dispatch gate: barrier + reset only when this dispatch's staged ranges conflict with
// the live set (any overlap involving a write) — then the staged set joins the live set
def hz_gate(enc : MetalComputeEncoder?) {
    // capture mode outranks an armed oracle: kn_dispatch consumes the staged ranges into the
    // node — gating here would eat them (leaving every captured node "undeclared")
    if (g_gr_on || !g_hz_on) {
        return
    }
    g_hz_gated++
    if (empty(g_hz_stage)) {
        // undeclared dispatch: serial semantics — barrier before it, and dirty forces one after
        if (g_hz_strict) {
            panic("dasLLAMA metal: undeclared dispatch under DASLLAMA_METAL_HAZARD_STRICT (gated #{g_hz_gated - g_hz_g0} on this encoder)")
        }
        g_hz_undeclared++
        if (g_hz_dirty || !empty(g_hz_pend)) {
            metal_memory_barrier(enc)
            g_hz_barriers++
            g_hz_pend |> clear()
        }
        g_hz_dirty = true
        return
    }
    var bar = g_hz_dirty || (g_hz_paranoid && !empty(g_hz_pend))
    if (!bar) {
        for (s in g_hz_stage) {
            for (p in g_hz_pend) {
                if ((p.dst || s.dst) && s.buf == p.buf && s.p0 < p.p1 && p.p0 < s.p1) {
                    bar = true
                    break
                }
            }
            break if (bar)
        }
    }
    if (bar) {
        metal_memory_barrier(enc)
        g_hz_barriers++
        g_hz_pend |> clear()
    }
    g_hz_dirty = false
    g_hz_pend |> push_from(g_hz_stage)
    g_hz_stage |> clear()
}

// arm/disarm around one encoder's build; lifetime counters survive
def hz_arm(on : bool) {
    g_hz_on = on
    g_hz_stage |> clear()
    g_hz_pend |> clear()
    g_hz_dirty = false
    g_hz_g0 = g_hz_gated   // per-encoder base — the strict panic names the offender by index
}

// ===== the step graph (the dispatch port, session 2) =====
// Graph mode: the kn_* twins RECORD each dispatch as a KNode; graph_flush replays in schedule
// order with the hz oracle deriving barriers — a schedule can only change speed, not correctness.

struct KNode {
    pso : MetalComputePipeline?
    grid : uint3
    tg : uint3
    tgmem : uint64   // threadgroup memory bytes at index 0 (0 = none)
    bind0 : int      // g_gr_binds slice
    nbind : int
    range0 : int     // g_gr_ranges slice (the hz declarations)
    nrange : int
}

struct KBind {
    buf : MetalBuffer?
    off : uint64
    slot : int
}

var g_gr_on = false
var @scratch g_gr_nodes : array<KNode>      // capture-rail arrays: grown once, clear()-recycled per capture
var @scratch g_gr_binds : array<KBind>
var @scratch g_gr_ranges : array<MemRange>
var g_gr_open = false            // a node is being built (kn_pipeline .. kn_dispatch)

def kn_pipeline(enc : MetalComputeEncoder?; pso : MetalComputePipeline?) {
    if (!g_gr_on) {
        metal_set_pipeline(enc, pso)
        return
    }
    // the helpers hold handles as const params; the node stores for deferred encoding only —
    // nothing writes through these, the strip is capture-side plumbing
    g_gr_nodes |> push(KNode(pso = unsafe(reinterpret<MetalComputePipeline?>(pso)),
        bind0 = length(g_gr_binds), range0 = length(g_gr_ranges)))
    g_gr_open = true
}

def kn_buffer(enc : MetalComputeEncoder?; b : MetalBuffer?; off : uint64; slot : int) {
    if (!g_gr_on) {
        metal_set_buffer(enc, b, off, slot)
        return
    }
    g_gr_binds |> push(KBind(buf = unsafe(reinterpret<MetalBuffer?>(b)), off = off, slot = slot))
}

def kn_tgmem(enc : MetalComputeEncoder?; bytes : uint64; idx : int) {
    if (!g_gr_on) {
        metal_set_threadgroup_memory_length(enc, bytes, idx)
        return
    }
    g_gr_nodes[length(g_gr_nodes) - 1].tgmem = bytes
}

def kn_dispatch(enc : MetalComputeEncoder?; grid, tg : uint3) {
    if (!g_gr_on) {
        metal_dispatch_threadgroups(enc, grid, tg)
        return
    }
    let i = length(g_gr_nodes) - 1
    assert(g_gr_open)
    g_gr_nodes[i].grid = grid
    g_gr_nodes[i].tg = tg
    g_gr_nodes[i].nbind = length(g_gr_binds) - g_gr_nodes[i].bind0
    // the helper's hz declarations become the node's ranges (hz_gate no-ops in graph mode)
    g_gr_ranges |> push_from(g_hz_stage)
    g_hz_stage |> clear()
    g_gr_nodes[i].nrange = length(g_gr_ranges) - g_gr_nodes[i].range0
    g_gr_open = false
}

def gr_arm(on : bool) {
    g_gr_on = on
    g_gr_nodes |> clear()
    g_gr_binds |> clear()
    g_gr_ranges |> clear()
    g_gr_open = false
    g_hz_stage |> clear()
}

// encode one captured node with the hz oracle deciding the barrier in front of it
def private gr_encode_node(enc : MetalComputeEncoder?; n : KNode) {
    g_hz_stage |> reserve(n.nrange)
    for (r in range(n.nrange)) {
        g_hz_stage |> push(g_gr_ranges[n.range0 + r])
    }
    metal_set_pipeline(enc, n.pso)
    if (n.tgmem > 0ul) {
        metal_set_threadgroup_memory_length(enc, n.tgmem, 0)
    }
    for (b in range(n.nbind)) {
        let kb = g_gr_binds[n.bind0 + b]
        metal_set_buffer(enc, kb.buf, kb.off, kb.slot)
    }
    hz_gate(enc)
    metal_dispatch_threadgroups(enc, n.grid, n.tg)
}

def graph_flush(enc : MetalComputeEncoder?) {
    let ident : array<int>
    graph_flush(enc, ident)
}

// ===== the load-time schedule (compile once per shape class, replay per token) =====
// ASAP leveling over the conflict DAG (write-involved overlap = ORDERING edge, capture-order
// direction) — levels are antichains, and the level count is the barrier floor.

struct KSched {
    order : array<int>
    depth : int
    nodes : int
}

var @scratch g_gr_scheds : table<uint64; KSched>
var g_gr_sched_mode = -1   // DASLLAMA_METAL_SCHED: 1 = leveled reorder (default), 0 = capture order

[cold_path]   // one-time env resolution; sched_enabled is a global test after the first call
def private load_sched_mode() {
    g_gr_sched_mode = env_int("DASLLAMA_METAL_SCHED", 1)
}

def private sched_enabled : bool {
    if (g_gr_sched_mode < 0) {
        load_sched_mode()
    }
    return g_gr_sched_mode != 0
}

// the shape-class key: pso sequence + per-node bind/range counts (pooled buffer identities and
// arena offsets vary per step, but the conflict STRUCTURE is role-determined within a class —
// grid values never enter, the oracle re-derives barriers per step anyway)
def private gr_shape_hash : uint64 {
    var h = 14695981039346656037ul
    for (n in g_gr_nodes) {
        h = (h ^ intptr(n.pso)) * 1099511628211ul
        h = (h ^ uint64(n.nbind)) * 1099511628211ul
        h = (h ^ uint64(n.nrange)) * 1099511628211ul
    }
    return h
}

// level[j] = 1 + max level of any conflicting predecessor; scan partitioned per buffer id.
// An UNDECLARED node has no edges and would float freely — refuse and keep capture order.
[cold_path]   // schedule compile — once per shape class, inside the g_gr_scheds miss
def private gr_schedule(var sched : KSched) : bool {
    let n = length(g_gr_nodes)
    sched.nodes = n
    for (j in range(n)) {
        if (g_gr_nodes[j].nrange == 0) {
            to_log(LOG_WARNING, "dasLLAMA metal: node #{j}/{n} is undeclared (nbind {g_gr_nodes[j].nbind}) — schedule refused\n")   // nolint:PERF028,PERF026 — inside gr_schedule, which runs once per shape class
            return false
        }
    }
    var level : array<int>
    level |> resize(n)
    // per-buffer event lists: indices into g_gr_ranges, with a parallel range->node map
    var rnode : array<int>
    rnode |> resize(length(g_gr_ranges))
    for (j in range(n)) {
        let nd & = unsafe(g_gr_nodes[j])
        for (r in range(nd.range0, nd.range0 + nd.nrange)) {
            rnode[r] = j
        }
    }
    var by_buf : table<uint64; array<int>>
    for (j in range(n)) {
        let nd & = unsafe(g_gr_nodes[j])
        var lvl = 0
        for (r in range(nd.range0, nd.range0 + nd.nrange)) {
            let rj = g_gr_ranges[r]
            if (key_exists(by_buf, rj.buf)) {
                for (k in by_buf[rj.buf]) {
                    let ri = g_gr_ranges[k]
                    if ((ri.dst || rj.dst) && rj.p0 < ri.p1 && ri.p0 < rj.p1) {
                        lvl = max(lvl, level[rnode[k]] + 1)
                    }
                }
            }
        }
        level[j] = lvl
        sched.depth = max(sched.depth, lvl)
        for (r in range(nd.range0, nd.range0 + nd.nrange)) {
            by_buf[g_gr_ranges[r].buf] |> push(r)   // nolint:PERF006 — per-buffer lists grow by ones across the whole walk
        }
    }
    // stable counting emit: capture order within a level keeps locality + determinism
    sched.order |> reserve(n)
    for (lvl in range(sched.depth + 1)) {
        for (j in range(n)) {
            if (level[j] == lvl) {
                sched.order |> push(j)
            }
        }
    }
    delete level
    delete rnode
    delete by_buf
    return true
}

//! Replay the captured graph under the shape class's cached schedule (compiled on first sight,
//! ~sub-ms one-time); DASLLAMA_METAL_SCHED=0 or an undeclared node fall back to capture order.
def graph_flush_sched(enc : MetalComputeEncoder?) {
    if (!sched_enabled()) {
        graph_flush(enc)
        return
    }
    let h = gr_shape_hash()
    if (!key_exists(g_gr_scheds, h)) {
        var s : KSched
        if (!gr_schedule(s)) {
            to_log(LOG_WARNING, "dasLLAMA metal decode: step graph holds undeclared dispatches — schedule stays capture-order\n")   // nolint:PERF028 — once per shape class, inside the schedule-compile miss
        }
        to_log(LOG_INFO, "dasLLAMA metal decode: schedule compiled — {s.nodes} nodes, depth {s.depth} (class {h})\n")   // nolint:PERF028,PERF026 — once per shape class, inside the schedule-compile miss
        g_gr_scheds[h] <- s
    }
    graph_flush(enc, g_gr_scheds[h].order)
}

//! Replay the captured step graph onto `enc` in `order` (empty = capture order), the hz oracle
//! deriving barriers per step — a schedule permutation can only change speed, never correctness.
def graph_flush(enc : MetalComputeEncoder?; order : array<int>) {
    g_gr_on = false
    hz_arm(true)
    // index loops, not for-in: a strict-mode panic must be able to unwind without leaving the
    // node array LOCKED (a recovered test would then die on the next gr_arm's clear)
    if (empty(order)) {
        for (i in range(length(g_gr_nodes))) {   // nolint:PERF018 — no for-in lock, see above
            gr_encode_node(enc, g_gr_nodes[i])
        }
    } else {
        assert(length(order) == length(g_gr_nodes))
        for (i in range(length(order))) {   // nolint:PERF018 — no for-in lock, see above
            gr_encode_node(enc, g_gr_nodes[order[i]])
        }
    }
    hz_arm(false)
}

// the coverage check: every dispatch on a gated encoder must have passed hz_gate — a raw
// metal_dispatch_* in a decode-path helper would run UNBARRIERED (silent race), so a count
// mismatch is loud: strict panics, otherwise one LOG_ERROR per process
def hz_coverage_check(ndisp, ngated : int64) {
    if (ndisp == ngated) {
        return
    }
    if (g_hz_strict) {
        panic("dasLLAMA metal decode: {ndisp - ngated} dispatch(es) bypassed hz_gate on a gated encoder")
    }
    if (!g_hz_cover_warned) {
        g_hz_cover_warned = true
        to_log(LOG_ERROR, "dasLLAMA metal decode: {ndisp - ngated} dispatch(es) bypassed hz_gate on a gated encoder — ungated dispatches are unbarriered\n")   // nolint:PERF028,PERF026 — correctness tripwire; firing at all means the encoder is already wrong
    }
}

//! (barriers issued, gated dispatches, conservative undeclared fallbacks) since load — read
//! deltas around a window; the chase prints barriers/step next to disp/step.
def public metal_hazard_stats() : tuple<barriers : int64; gated : int64; undeclared : int64> {
    return (barriers = g_hz_barriers, gated = g_hz_gated, undeclared = g_hz_undeclared)
}

//! Hazard-mode rails: `paranoid` barriers every dispatch on the concurrent encoder (isolates
//! encoder-flip bugs from barrier-elision bugs); `strict` panics on an undeclared dispatch
//! (the migration-coverage gate). Env twins: `DASLLAMA_METAL_HAZARD_PARANOID` / `_STRICT`.
def public set_metal_decode_hazard(paranoid, strict : bool) {
    g_hz_paranoid = paranoid
    g_hz_strict = strict
}


// ===== the pending-step (encode-ahead) machinery =====

// the pending pipelined step (one deep): everything finish_pending_step needs to wait, read
// back, and release AFTER the next step is committed behind it. Sessions are borrowed pointers
// (caller-owned) — always cleared, never deleted.

var g_lp_active = false
var g_lp_cbs : array<MetalCommandBuffer?>
var @scratch g_lp_pooled : array<tuple<buf : MetalBuffer?; size : uint64>>   // pending-step stash; clear()-recycled per finish
var @scratch g_lp_upooled : array<tuple<buf : MetalBuffer?; size : uint64>>
var g_lp_blog : MetalBuffer?
var g_lp_xb : MetalBuffer?      // production sync + CPU-classifier path: x_b writeback
var g_lp_xb_dst : void?         // ... into the caller's workspace (borrowed, sync-only)
var g_lp_xb_bytes = 0l
var g_lp_err = false            // sticky per-step dispatch-error signal for sync callers
var @scratch g_lp_sessions : array<Session?>
var @scratch g_lp_positions : array<int64>
var @scratch g_lp_koffs : array<uint64>
var @scratch g_lp_voffs : array<uint64>
var @scratch g_lp_caps : array<int64>
var g_lp_nrows = 0l
var g_lp_layers = 0l
var g_lp_seq = 0l
var g_lp_vocab = 0l
// hetero (gemma4): per-layer row bytes + slab bases within a mirror plane (uniform models:
// lbase_l == l * cap * rowb_l — captured at encode, one array per step)
var @scratch g_lp_lrowb : array<int64>
var @scratch g_lp_lkbase : array<int64>   // per (row i, layer l) flattened i * n_layers + l (caps differ per row)
var @scratch g_lp_lvbase : array<int64>
var g_lp_prev_gpu_end = 0.0lf   // last finished step's gpu_end — the handoff stat's anchor

// wait + read back + release the pending step; x_b writes back only on the sync CPU-classifier
// path (pipe mode skips it — stale). A dispatch error sets g_lp_err (sync reruns on CPU; a pipelined step is lost).
def finish_pending_step {
    if (!g_lp_active) {
        return
    }
    let ts_wait = ref_time_ticks()
    // in-order queue: the last cb completing implies all of them did
    metal_wait_until_completed(g_lp_cbs[length(g_lp_cbs) - 1])
    var err = ""
    for (c_i in g_lp_cbs) {
        if (empty(err) && metal_command_buffer_failed(c_i)) {
            err = cb_error_text(c_i)
        }
        g_sched_b_ms += (metal_command_buffer_kernel_end_time(c_i) - metal_command_buffer_kernel_start_time(c_i)) * 1000.0lf
    }
    // gpu = first-start .. last-end envelope; handoff = the TRUE inter-step GPU idle — this step's
    // start vs whichever bound it last (the previous step's gpu end, or its own sched end)
    let gs0 = metal_command_buffer_gpu_start_time(g_lp_cbs[0])
    let ge_last = metal_command_buffer_gpu_end_time(g_lp_cbs[length(g_lp_cbs) - 1])
    g_gpu_b_ms += (ge_last - gs0) * 1000.0lf
    g_handoff_b_ms += max(0.0lf, (gs0 - max(g_lp_prev_gpu_end, metal_command_buffer_kernel_end_time(g_lp_cbs[0]))) * 1000.0lf)
    if (g_pipe_debug) {   // per-step pipeline trace (first steps + outliers)
        let gap_ms = (gs0 - g_lp_prev_gpu_end) * 1000.0lf
        let env_ms = (ge_last - gs0) * 1000.0lf
        if (g_batch_steps < 8l || gap_ms > 1.0lf) {
            to_log(LOG_INFO, "metal pipe step {g_batch_steps}: gap={gap_ms}ms env={env_ms}ms\n")   // nolint:PERF028,PERF026 — behind the g_pipe_debug gate above
        }
    }
    g_lp_prev_gpu_end = ge_last
    g_us_bwait += int64(get_time_usec(ts_wait))
    for (c_i in g_lp_cbs) {
        metal_release(c_i)
    }
    g_lp_cbs |> clear()   // released above — delete of array<T?> would double-free the pointees
    if (empty(err)) {
        let ts_rb = ref_time_ticks()
        unsafe {
            if (g_lp_xb != null) {   // sync CPU-classifier path only — pipe skips it (stale)
                memcpy(g_lp_xb_dst, metal_buffer_contents(g_lp_xb), g_lp_xb_bytes)
            }
            let ab = reinterpret<uint8?>(metal_buffer_contents(g_arena))
            let lgp = g_lp_blog != null ? reinterpret<uint8?>(metal_buffer_contents(g_lp_blog)) : null
            for (i in range64(g_lp_nrows)) {
                var sp = g_lp_sessions[i]
                let pos = g_lp_positions[i]
                let prow = kv_phys_row(*sp, pos)
                for (l in range64(g_lp_layers)) {
                    let rowb_l = g_lp_lrowb[l]
                    let fb = i * g_lp_layers + l
                    memcpy(reinterpret<void?>(kv_layer_kbase(*sp, l, g_lp_seq) + prow * rowb_l),
                        reinterpret<void?>(ab + (g_lp_koffs[i] + uint64(g_lp_lkbase[fb] + pos * rowb_l))), rowb_l)
                    memcpy(reinterpret<void?>(kv_layer_vbase(*sp, l, g_lp_seq) + prow * rowb_l),
                        reinterpret<void?>(ab + (g_lp_voffs[i] + uint64(g_lp_lvbase[fb] + pos * rowb_l))), rowb_l)
                }
                if (lgp != null) {
                    memcpy(addr < void? >(sp.logits[0]),
                        reinterpret<void?>(lgp + i * g_lp_vocab * 4l), g_lp_vocab * 4l)
                }
                if (key_exists(g_mirrors, sp.uid)) {   // never regress the optimistic commit-time mark
                    let wm0 = g_mirrors[sp.uid].watermark
                    g_mirrors[sp.uid].watermark = max(wm0, pos + 1l)
                }
            }
        }
        g_us_breadback += int64(get_time_usec(ts_rb))
        g_batch_steps++
        g_batch_rows += g_lp_nrows
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal batch decode: dispatch failed ({err}) — pipelined step lost\n")   // nolint:PERF028,PERF026 — error path; the re-upload it triggers dwarfs the log
        // the optimistic commit-time watermark now overstates the mirror (its rows are garbage at
        // and above this step's positions) — force a full re-upload from the CPU truth
        for (i in range64(g_lp_nrows)) {
            var sp = g_lp_sessions[i]
            if (key_exists(g_mirrors, sp.uid)) {
                g_mirrors[sp.uid].watermark = 0l
            }
        }
        g_lp_err = true
        batch_decline(MetalDecodeDecline.gpu_error)
    }
    for (pb in g_lp_pooled) {
        var b = pb.buf
        pool_release(g_pool, b, pb.size)
    }
    g_lp_pooled |> clear()
    for (pb in g_lp_upooled) {
        var b = pb.buf
        pool_release(g_upool, b, pb.size)
    }
    g_lp_upooled |> clear()
    g_lp_blog = null
    g_lp_xb = null
    g_lp_xb_dst = null
    g_lp_sessions |> clear()   // borrowed handles — clear, never delete
    g_lp_active = false
}

//! Complete any in-flight pipelined GPU decode step (the encode-ahead rails), landing its logits
//! and KV writeback. Benches call this before closing a timing window; no-op when nothing pends.
def public metal_decode_flush {
    finish_pending_step()
}


// ===== the device half of driver init + the state half of shutdown =====

//! Bring up the shared device + queue (the PSO half lives in dasllama_metal_kernels'
//! metal_decode_init, which calls this first).
[cold_path]   // one-time device/queue bring-up, guarded by its own inited flag
def metal_common_init : bool {
    if (g_dev != null) {
        return true
    }
    if (g_failed) {
        return false
    }
    g_dev = metal_create_system_default_device()
    if (g_dev == null) {
        to_log(LOG_WARNING, "dasLLAMA metal decode: no Metal device — decode stays on the CPU\n")
        g_failed = true
        return false
    }
    g_queue = metal_new_command_queue(g_dev)
    g_attn_single_max = clamp(int64(env_int("DASLLAMA_METAL_ATTN_SINGLE", 64)), ATTN_CHUNK_ROWS, 128l)
    g_pipe_debug = env_flag("DASLLAMA_METAL_PIPE_DEBUG", false)
    g_mirror_cap_env = env_int("DASLLAMA_METAL_KV_MIRROR_MB", 4096)
    g_hz_paranoid = env_int("DASLLAMA_METAL_HAZARD_PARANOID", 0) != 0
    g_hz_strict = env_int("DASLLAMA_METAL_HAZARD_STRICT", 0) != 0
    return true
}

// ===== the weights-epoch guard =====
// g_regions/g_cat_blobs key by host address; a model reload makes those keys lie (see
// weights_epoch). Drivers flush at entry when the epoch moved, after quiescing in-flight work.
var g_weight_cache_epoch = -1l

def weight_caches_stale() : bool {
    return weights_epoch() != g_weight_cache_epoch
}

[cold_path]   // live-reload only
def weight_caches_flush_on_reload {
    if (!weight_caches_stale()) {
        return
    }
    finish_pending_step()
    for (buf in values(g_regions)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_regions
    }
    g_weight_cache_epoch = weights_epoch()
}

def metal_common_release {
    // schedules key on PSO pointers — a re-init reallocates PSOs, so cached classes would lie
    unsafe {
        delete g_gr_scheds
    }
    nc_logits_release()
    for (mr in values(g_mirrors)) {
        mirror_release(mr)
    }
    unsafe {
        delete g_mirrors
    }
    for (dmr in values(g_dn_mirrors)) {
        dn_mirror_release(dmr)
    }
    unsafe {
        delete g_dn_mirrors
    }
    g_mirror_bytes = 0ul
    if (g_arena != null) {
        metal_release(g_arena)
        g_arena = null
    }
    g_arena_bytes = 0ul
    g_arena_free |> clear()
    for (buf in values(g_regions)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_regions
    }
    pool_drain(g_pool)
    pool_drain(g_upool)
    if (g_signs != null) {
        metal_release(g_signs)
        g_signs = null
    }
    if (g_queue != null) {
        metal_release(g_queue)
        g_queue = null
    }
    if (g_dev != null) {
        metal_release(g_dev)
        g_dev = null
    }
    g_failed = false
}

// ===== the prefill path's decline machinery + the shared block_q8_0 blob cache =====
// (moved from dasllama_metal_prefill in the W1 split: the stage report above reads these, and
// prefill requires THIS module for its decline enum — the reverse call would be a cycle)

// why a prefill declined to the CPU path — the FIRST failing gate clause (the decode enum's
// twin; the support-matrix test asserts these names as strings)
enum MetalPrefillDecline {
    none                // supported — the prefill runs on the GPU
    start_pos           // RETIRED (M2.5): a continuation chunk gathers the session's existing
                        // rows into the K/V panels and shifts the causal diagonal by start_pos
    npos_window         // npos below g_min_npos or above g_max_npos
    quant_mode          // weights not loaded under QuantMode.q8
    kquant_native       // per-tensor K-quant planes: not readable as qblob offsets (kernels = W4)
    wscale_f16          // f16 scale plane (the load DEFAULT) — GPU prefill needs the s16 path (W2)
    arch                // RETIRED (family waves A): the arch-name check is gone — non-std block
                        // sets decline `graph`, feature-flag configs decline `feature` per bit
    backend_repack      // repack backend: interleaved weight layout unreadable by the GEMM kernel
    config              // RETIRED (family waves A): split into `feature` + the per-need
                        // `need_*` keys in the declines-by-reason table
    graph               // block set is not the std dense stack (MoE / recurrent / custom attn)
    feature             // the model needs layer-stack features this path's kernels don't
                        // implement — each missing bit records a `need_*` key alongside
    shape               // head/dim alignment outside the kernel grids
    layers              // per-layer non-uniformity (kv_dim/head_size/hidden/sliding/missing wv)
    rope_rows           // the window's rope table not built
    device              // metal_prefill_init failed (no device / PSO build failure)
    gpu_error           // dispatch failed at execution time
    planar              // weights not in the blob-image form (the drivers are blob-only)
    dn_state            // recurrent state not GPU-resident and no CPU prefix to sync from
}

var g_prefill_declines = 0l
var g_prefill_declines_by_reason : table<string; int64>

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def decline(why : MetalPrefillDecline) {
    g_prefill_declines++
    g_prefill_declines_by_reason["{why}"]++
    if (why != MetalPrefillDecline.npos_window) {   // the one POLICY decline — see require_or_panic
        require_or_panic("prefill", "{why}")
    }
}

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def decline(why : MetalPrefillDecline; missing : MetalNeed) {
    g_prefill_declines++
    g_prefill_declines_by_reason["{why}"]++
    record_needs(g_prefill_declines_by_reason, missing)
    require_or_panic("prefill", "{why} {missing}")
}

//! Declines-by-reason since load — reason name -> count (the first failing gate clause per
//! declined prefill). The support-matrix test asserts WHY a config declined; the stage report
//! prints it alongside the decode tables.
def public metal_prefill_declines() : table<string; int64> {
    return clone_to_move(g_prefill_declines_by_reason)
}

// ===== Metal-4 tensor twin race scaffolding (shared by the per-module race sections) =====

struct public MetalTensorRaceResult {
    family : string
    winner : string   // "tensor" | "simdgroup" | "" when the family could not race
    base_ms : double
    twin_ms : double
    note : string
}

def race_buf(dev : MetalDevice?; bytes : uint64; src : void?) : MetalBuffer? {
    var b = metal_new_buffer(dev, bytes)
    if (src != null) {
        unsafe {
            memcpy(metal_buffer_contents(b), src, bytes)
        }
    }
    return b
}

def race_uniform_u32(dev : MetalDevice?; v : uint) : MetalBuffer? {
    var b = metal_new_buffer(dev, 4ul)
    unsafe {
        var p = reinterpret<uint?>(metal_buffer_contents(b))
        p[0] = v
    }
    return b
}

// deterministic 34B-interleaved q8_0 W blob for the races: int quants, pow2 f16 scales
def race_q8_blob(nblk : int) : array<int8> {
    var blob : array<int8>
    blob |> resize(nblk * 34)
    for (blk in range(nblk)) {
        for (k in range(32)) {
            blob[blk * 34 + 2 + k] = int8((blk * 7 + k * 3) % 19 - 9)
        }
    }
    unsafe {
        var p8 = addr(blob[0])
        for (blk in range(nblk)) {
            var ph = reinterpret<float16?>(p8 + blk * 34)
            ph[0] = float16(0.5 * float(1 << (blk % 3)))
        }
    }
    return <- blob
}

// deterministic f32 activation panel for the races
def race_x_f32(count : int) : array<float> {
    var xa : array<float>
    xa |> resize(count)
    for (i in range(count)) {
        xa[i] = 0.25 * float(i % 17 - 8)
    }
    return <- xa
}

// eligibility envelope: f16-staging noise passes, a garbage twin lands at O(max)
def race_envelope_ok(ba, bb : MetalBuffer?; count : int; var note : string&) : bool {
    var maxref = 0.0lf
    var maxdiff = 0.0lf
    unsafe {
        let pb = reinterpret<float?>(metal_buffer_contents(ba))
        let pt = reinterpret<float?>(metal_buffer_contents(bb))
        for (i in range(count)) {
            maxref = max(maxref, abs(double(pb[i])))
            maxdiff = max(maxdiff, abs(double(pt[i]) - double(pb[i])))
        }
    }
    if (maxdiff > 0.05lf * maxref + 1e-6lf) {
        note = "twin OUTPUT MISMATCH maxdiff={maxdiff} maxref={maxref}"
        return false
    }
    return true
}

// interleaved adjacent A/B pair — separate per-side blocks read the box's thermal/clock ramp
// as a phantom winner (reds-dig2; sequential blocks INVERTED m4's kq verdicts). Best-of/side.
def race_pair_ms(queue : MetalCommandQueue?; reps : int; var base_ms, twin_ms : double&;
                 base_blk : block<(enc : MetalComputeEncoder?) : void>;
                 twin_blk : block<(enc : MetalComputeEncoder?) : void>) {
    base_ms = -1.0lf
    twin_ms = -1.0lf
    var err = ""
    var best_b = -1.0lf
    var best_t = -1.0lf
    for (r in range(reps + 1)) {
        var g1 = 0.0lf
        var g2 = 0.0lf
        let okb = with_compute_encoder_timed(queue, err, g1) $(enc) {
            invoke(base_blk, enc)
        }
        let okt = okb && with_compute_encoder_timed(queue, err, g2) $(enc) {
            invoke(twin_blk, enc)
        }
        return if (!okb || !okt)
        if (r > 0) {
            if (best_b < 0.0lf || g1 < best_b) {
                best_b = g1
            }
            if (best_t < 0.0lf || g2 < best_t) {
                best_t = g2
            }
        }
    }
    base_ms = best_b
    twin_ms = best_t
}

// best GPU time over `reps` dispatches after one warmup; <0 on dispatch failure
def race_time_ms(queue : MetalCommandQueue?; reps : int; blk : block<(enc : MetalComputeEncoder?) : void>) : double {
    var best = -1.0lf
    var err = ""
    for (r in range(reps + 1)) {
        var gpu_ms = 0.0lf
        var okr = false
        okr = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
            invoke(blk, enc)
        }
        if (!okr) {
            return -1.0lf
        }
        if (r > 0 && (best < 0.0lf || gpu_ms < best)) {
            best = gpu_ms
        }
    }
    return best
}
