options gen2
options _comment_hygiene = true

module dasllama_metal_common shared public

require math
require strings
require daslib/fio
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math

// Shared infrastructure for the dasMetal resident drivers (the format-matrix arc's W1 split):
// device/queue init, buffer pools, the weight-region + block_q8_0-blob + cat-blob caches, the
// per-session KV mirror arena (watermark/LRU/rewind contract — see dasllama_metal_llama's
// header), HazTrack, the pending-step (encode-ahead) machinery + metal_decode_flush, upload +
// uniform helpers, and the decline observability layer (all three GPU paths' reason enums,
// declines-by-reason tables, and the stage report). The kernel/PSO registry
// (dasllama_metal_kernels) and the family graph modules (dasllama_metal_llama) sit on top.
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

// why a step declined to the CPU path — every gate reports its FIRST failing clause, and the
// declines-by-reason tables make silent coverage regressions observable (the support-matrix test
// asserts these names as strings; renaming a value is a test-visible change)
enum MetalDecodeDecline {
    none                // supported — the step runs on the GPU
    quant_mode          // weights not loaded under QuantMode.q8
    kquant_native       // per-tensor K-quant planes: not readable as qblob offsets (kernels = W4)
    arch                // not the llama family graph
    backend_repack      // repack backend: interleaved weight layout unreadable by the GPU kernels
    rope_mode           // rope tables off
    config              // feature-flag config the kernels don't implement (neox/bias/qk-norm/softcap/...)
    shape               // head/dim alignment outside the kernel grids
    layers              // per-layer non-uniformity (kv_dim/head_size/hidden/sliding/missing wv)
    kv_dtype            // session KV codec not uniform f16/f32 (the mirror moves raw row bytes)
    depth               // RETIRED (M2.4): the chunked/partD attention scans any depth — capacity
                        // limits surface as `mirror` (the arena cannot hold the session's slice)
    no_uid              // session without a real uid (mirror keying)
    rope_rows           // the position's rope row not built
    batch_dim_cap       // RETIRED (M2.6): the batch add+rms falls back to the unfused pair past
                        // the fused slab, and the GEMM's fixed K-tiles serve any dim at B > 4
    device              // metal_decode_init failed (no device / PSO build failure)
    mirror              // the mirror arena cannot hold the KV slice
    gpu_error           // dispatch failed at execution time
}

var g_dev : MetalDevice?
var g_queue : MetalCommandQueue?
var g_failed = false
var g_regions : table<uint64; MetalBuffer?>  // base address -> resident weight/scale/norm region
var g_pool : MetalBufferPool
var g_upool : MetalBufferPool                // UNTRACKED pool: uniforms + rope rows (GPU-read-only)
var g_decodes = 0l
var g_declines = 0l
var g_declines_by_reason : table<string; int64>
var g_logits_cpu = false
var g_skip = ""                              // DASLLAMA_METAL_DECODE_SKIP=gemv|attn|ew knockout rail

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

var g_mirrors : table<uint64; KVMirror>
var g_mirror_bytes = 0ul
var g_tick = 0l
var g_mirror_cap_mb = -1   // <0 = unset: DASLLAMA_METAL_KV_MIRROR_MB (default 4096) reads at use
                           // — a CEILING (M2.7): the arena starts at 256MB and grows on demand

// the mirror arena: one buffer, first-fit free-list suballocation (slices are per-session and
// coarse, so fragmentation stays trivial; a failed fit evicts LRU mirrors and retries)
var g_arena : MetalBuffer?
var g_arena_bytes = 0ul
var g_arena_epoch = 0l   // bumps on every grow-rebuild — pre-encoded steps referencing the old buffer are stale
var g_arena_free : array<tuple<off : uint64; size : uint64>>   // sorted by off, coalesced
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

def decline(why : MetalDecodeDecline) {
    g_declines++
    g_declines_by_reason["{why}"]++
    require_or_panic("decode", "{why}")
}

def batch_decline(why : MetalDecodeDecline) {
    g_batch_declines++
    g_batch_declines_by_reason["{why}"]++
    require_or_panic("batch decode", "{why}")
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

//! Cumulative per-stage wall since load/reset — (served steps, CPU setup us, encode us, commit+wait
//! us, GPU-timestamp us within the wait, readback+writeback us). The chase harness's numbers rail.
def public metal_decode_stage_stats() : tuple<steps : int64; setup_us : int64; encode_us : int64; wait_us : int64; gpu_us : double; readback_us : int64> {
    return (steps = g_decodes, setup_us = g_us_setup, encode_us = g_us_encode,
        wait_us = g_us_wait, gpu_us = g_gpu_ms * 1000.0lf, readback_us = g_us_readback)
}

//! (live KV mirrors, resident mirror bytes) — the churn / LRU-cap gates read this.
def public metal_decode_mirror_stats() : tuple<mirrors : int64; bytes : uint64> {
    return (mirrors = int64(length(g_mirrors)), bytes = g_mirror_bytes)
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

def env_int(name : string; dflt : int) : int {
    return has_env_variable(name) ? to_int(get_env_variable(name)) : dflt
}


def upload_region(src : void?; bytes : uint64) : MetalBuffer? {
    let key = intptr(src)
    if (key_exists(g_regions, key)) {
        return g_regions[key]
    }
    var buf = metal_new_buffer_untracked(g_dev, bytes)
    unsafe {
        memcpy(metal_buffer_contents(buf), src, int(bytes))
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
        memcpy(reinterpret<void?>(d), p0, int(b0))
        memcpy(reinterpret<void?>(d + b0), p1, int(b1))
        if (b2 > 0ul) {
            memcpy(reinterpret<void?>(d + (b0 + b1)), p2, int(b2))
        }
    }
    g_regions[key] = buf
    return buf
}

// one tensor's scale-plane region — the f32 plane or the wscale_f16 halfword plane, whichever
// the model carries (the GEMV PSO twin is picked to match)
def upload_scales(t : Model; off, rows, n : int64) : MetalBuffer? {
    let cnt = rows * (n / 32l)
    unsafe {
        if (t.wscale_f16) {
            return upload_region(addr < void? >(t.qscales16[off / 32l]), uint64(cnt * 2l))
        }
        return upload_region(addr < void? >(t.qscales[off / 32l]), uint64(cnt * 4l))
    }
}

// one tensor's resident 34B q8-block blob (the prefill module's cache — shared, repacked once)
def blob_of(t : Model; off, rows, n : int64) : MetalBuffer? {
    unsafe {
        let q = addr < void? >(t.qblob[off])
        let s = t.wscale_f16 ? addr < void? >(t.qscales16[off / 32l]) : addr < void? >(t.qscales[off / 32l])
        return metal_blob_region(g_dev, q, s, rows, n, t.wscale_f16)
    }
}

// the concat twin for the fused QKV / W1|W3 scale planes (rows2 == 0 = two-piece)
def upload_scales_cat3(t : Model; off0, rows0, off1, rows1, off2, rows2, n : int64) : MetalBuffer? {
    let ssz = t.wscale_f16 ? 2l : 4l
    let nb = n / 32l
    unsafe {
        let p0 = t.wscale_f16 ? addr < void? >(t.qscales16[off0 / 32l]) : addr < void? >(t.qscales[off0 / 32l])
        let p1 = t.wscale_f16 ? addr < void? >(t.qscales16[off1 / 32l]) : addr < void? >(t.qscales[off1 / 32l])
        var p2 : void?
        if (rows2 > 0l) {
            p2 = t.wscale_f16 ? addr < void? >(t.qscales16[off2 / 32l]) : addr < void? >(t.qscales[off2 / 32l])
        }
        return upload_region_cat3(p0, uint64(rows0 * nb * ssz), p1, uint64(rows1 * nb * ssz), p2, uint64(rows2 * nb * ssz))
    }
}

// the kq formats the Metal kernels implement — anything else (the q4_0 QAT tier, future
// formats) must DECLINE kquant_native instead of falling into a wrong-layout kernel branch
// (the dispatchers below treat "not k4/k6" as k5)
def kq_fmt_gpu_supported(f : KqFmt) : bool {
    return f == KqFmt.q8 || f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6
}

def private kq_fmts_gpu_supported(a : array<KqFmt>) : bool {
    for (f in a) {
        if (!kq_fmt_gpu_supported(f)) {
            return false
        }
    }
    return true
}

// every tensor fmt a kq load can route to a GPU kernel — the decode/batch/prefill gates all
// clause on this (the MoE expert planes never reach the llama-family GPU graph, so we1/2/3
// stay out of scope)
def kq_load_gpu_supported(t : Model) : bool {
    if (!t.kquant_native) {
        return true
    }
    return ((kq_fmts_gpu_supported(t.wq_fmt) && kq_fmts_gpu_supported(t.wk_fmt) &&
             kq_fmts_gpu_supported(t.wv_fmt) && kq_fmts_gpu_supported(t.wo_fmt)) &&
        (kq_fmts_gpu_supported(t.w1_fmt) && kq_fmts_gpu_supported(t.w2_fmt) &&
             kq_fmts_gpu_supported(t.w3_fmt)) &&
        (kq_fmt_gpu_supported(t.wcls_fmt) && kq_fmt_gpu_supported(t.emb_fmt)))
}

// ===== zero-copy logits: bytesNoCopy wrappers over session-owned storage =====
// Writes land straight into s.logits (no landing memcpy); borrowed, no deallocator. Gated on
// 16KB page alignment + page-rounded capacity; the pointer is revalidated on every lookup.
var g_nc_logits : table<uint64; tuple<buf : MetalBuffer?; ptr : uint64>>

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
    let bytes = (uint64(length(s.logits)) * 4ul + 16383ul) & ~16383ul
    var buf : MetalBuffer?
    if ((p & 16383ul) == 0ul && uint64(capacity(s.logits)) * 4ul >= bytes) {
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

// One kq tensor's quant plane slice, uploaded BYTE-IDENTICAL (disk-order superblock payloads —
// the backend_repack decline excludes grp-repacked loads, and the non-repack backends leave the
// planes disk-order). Cached in g_regions by source address like every weight region.
def kq_upload_quants(dev : MetalDevice?; t : Model; fmt : KqFmt; off, rows, n : int64) : MetalBuffer? {
    let sb0 = off / 256l
    let nsb = (rows * n) / 256l
    unsafe {
        if (fmt == KqFmt.k4) {
            return kq_region(dev, addr < void? >(t.k4q[sb0 * 128l]), uint64(nsb * 128l))
        }
        if (fmt == KqFmt.k5) {
            return kq_region(dev, addr < void? >(t.k5q[sb0 * 160l]), uint64(nsb * 160l))
        }
        return kq_region(dev, addr < void? >(t.k6q[sb0 * 192l]), uint64(nsb * 192l))
    }
}

// upload_region's device-parameterized twin (the kq helpers serve BOTH resident drivers — the
// prefill driver owns its own device handle; same shared address-keyed cache)
def private kq_region(dev : MetalDevice?; src : void?; bytes : uint64) : MetalBuffer? {
    let key = intptr(src)
    if (key_exists(g_regions, key)) {
        return g_regions[key]
    }
    var buf = metal_new_buffer_untracked(dev, bytes)
    unsafe {
        memcpy(metal_buffer_contents(buf), src, int(bytes))
    }
    g_regions[key] = buf
    return buf
}

// One kq tensor's GPU-form scale buffer: k4/k5 = 16B compact blocks (20B production block minus
// pad — its varying mod-16 phase defeats vector-load merge); k6 = [16B sub-scale][packed f16 d]
// in ONE buffer, d plane at byte nsb*16 (kernel binds the buffer twice with offsets).
def kq_upload_scales(dev : MetalDevice?; t : Model; fmt : KqFmt; off, rows, n : int64) : MetalBuffer? {
    let sb0 = off / 256l
    let nsb = (rows * n) / 256l
    unsafe {
        if (fmt == KqFmt.k6) {
            let key = intptr(addr(t.k6s[sb0 * 18l]))
            if (key_exists(g_regions, key)) {
                return g_regions[key]
            }
            var buf = metal_new_buffer_untracked(dev, uint64(nsb * 18l))
            var d = reinterpret<uint8?>(metal_buffer_contents(buf))
            let src = addr < uint8 const? >(t.k6s[sb0 * 18l])
            for (sb in range64(nsb)) {
                memcpy(reinterpret<void?>(d + sb * 16l), reinterpret<void const?>(src + sb * 18l), 16)
                d[nsb * 16l + sb * 2l] = src[sb * 18l + 16l]
                d[nsb * 16l + sb * 2l + 1l] = src[sb * 18l + 17l]
            }
            g_regions[key] = buf
            return buf
        }
        let src = fmt == KqFmt.k4 ? addr < uint8 const? >(t.k4s[sb0 * 20l]) : addr < uint8 const? >(t.k5s[sb0 * 20l])
        let key = intptr(src)
        if (key_exists(g_regions, key)) {
            return g_regions[key]
        }
        var buf = metal_new_buffer_untracked(dev, uint64(nsb * 16l))
        var d = reinterpret<uint8?>(metal_buffer_contents(buf))
        for (sb in range64(nsb)) {
            memcpy(reinterpret<void?>(d + sb * 16l), reinterpret<void const?>(src + sb * 20l), 16)
        }
        g_regions[key] = buf
        return buf
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

// the slice mirror_prepare would build for (s, pos): chunked high-water cap (pow2 from 256,
// never past the session's own seq cap) and the K/V plane sizes
def private mirror_slice_shape(t : Model; s : Session; pos : int64) : tuple<cap : int64; kbytes : uint64; vbytes : uint64> {
    let c = t.config
    let kv_dim = layer_kv_dim(c, 0l)
    let needed = pos + 1l
    var cap = 256l
    while (cap < needed) {
        cap *= 2l
    }
    cap = clamp(cap, needed, c.seq_len)
    return (cap = cap, kbytes = uint64(c.n_layers * cap * kv_row_bytes(s.kv_dtype_k, kv_dim)),
            vbytes = uint64(c.n_layers * cap * kv_row_bytes(s.kv_dtype_v, kv_dim)))
}

def private mirror_ceiling_bytes : uint64 {
    let ceil_mb = uint64(clamp(g_mirror_cap_mb >= 0 ? g_mirror_cap_mb : env_int("DASLLAMA_METAL_KV_MIRROR_MB", 4096), 64, 4096))
    return ceil_mb * 1024ul * 1024ul
}

// release-all rebuild at `want` (clamped to the ceiling): every mirror re-uploads via its
// watermark, and the epoch bump invalidates any pre-encoded step whose kernels reference the
// OLD arena buffer. Callers must ensure nothing is pinned and no step is in flight.
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
    to_log(LOG_INFO, "dasLLAMA metal decode: KV mirror arena grown to {int(g_arena_bytes / (1024ul * 1024ul))}MB (ceiling {int(ceil_bytes / (1024ul * 1024ul))}MB)\n")
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
    let kv_dim = layer_kv_dim(c, 0l)
    let krow = kv_row_bytes(s.kv_dtype_k, kv_dim)
    let vrow = kv_row_bytes(s.kv_dtype_v, kv_dim)
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
        let cap_mb = uint64(g_mirror_cap_mb >= 0 ? g_mirror_cap_mb : env_int("DASLLAMA_METAL_KV_MIRROR_MB", 4096))
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
            for (l in range64(c.n_layers)) {
                let kbase = kv_layer_kbase(s, l, c.seq_len)
                let vbase = kv_layer_vbase(s, l, c.seq_len)
                for (r in range64(nrun)) {
                    let j0 = s.kv_runs[r].j0
                    let nrows = s.kv_runs[r].j1 - j0
                    let prow = s.kv_runs[r].row0
                    memcpy(reinterpret<void?>(kdst + (l * cap + j0) * krow),
                        reinterpret<void?>(kbase + prow * krow), int(nrows * krow))
                    memcpy(reinterpret<void?>(vdst + (l * cap + j0) * vrow),
                        reinterpret<void?>(vbase + prow * vrow), int(nrows * vrow))
                }
            }
        }
        g_mirrors[s.uid].watermark = pos
    }
    return (ok = true, koff = koff, voff = voff, cap = cap)
}


def signs_buffer() : MetalBuffer? {
    if (g_signs == null) {
        var signs : array<float>
        tq4_fill_signs(signs, 128l)
        g_signs = metal_new_buffer(g_dev, 512ul)
        unsafe {
            memcpy(metal_buffer_contents(g_signs), addr < void? >(signs[0]), 512)
        }
        delete signs
    }
    return g_signs
}

// tq4-mirror rope+store: one threadgroup per head (the FWHT spans it); mirror rows bind twice
// each (half-scale + byte views at the same row offset)

let HZ_BX     = 1u
let HZ_BXB    = 2u
let HZ_BXB2   = 4u
let HZ_BQKV   = 8u
let HZ_BHB    = 16u
let HZ_BHB2   = 32u
let HZ_BPART  = 64u
let HZ_ARENA  = 128u
let HZ_BPSK   = 256u
let HZ_BXF    = 512u
let HZ_BLOG   = 1024u

struct HazTrack {
    on : bool
    w : uint    // buffers with a pending (unbarriered) write
    r : uint    // buffers with a pending read
}

def bar_dep(enc : MetalComputeEncoder?; var h : HazTrack; rmask, wmask : uint) {
    if (!h.on) {
        return
    }
    if ((rmask & h.w) != 0u || (wmask & h.w) != 0u || (wmask & h.r) != 0u) {
        metal_memory_barrier(enc)
        h.w = 0u
        h.r = 0u
    }
    h.r |= rmask
    h.w |= wmask
}


// ===== fused-GEMV cat blobs + the pending-step (encode-ahead) machinery =====

// fused-GEMV weight blobs: 2-3 tensors' q8 rows repacked into ONE contiguous block_q8_0 stream.
// Own cache (NOT the per-tensor blob cache — different content under a colliding base address);
// released in metal_decode_shutdown.
var g_cat_blobs : table<uint64; MetalBuffer?>

// one q8 region (rows x n) interleaved into `base` at block offset blk0 — the same 34B layout
// as metal_blob_region, targeting a slice of a cat blob
def repack_blocks(base : int8?; blk0 : int64; t : Model; off, rows, n : int64) {
    let nblocks = rows * (n / 32l)
    unsafe {
        var qp = addr < int8? >(t.qblob[off])
        if (t.wscale_f16) {
            var sp16 = addr < uint16? >(t.qscales16[off / 32l])
            for (b in range64(nblocks)) {
                let o = (blk0 + b) * 34l
                var pb = reinterpret<uint16?>(base + o)
                pb[0] = sp16[b]
                memcpy(reinterpret<void?>(base + o + 2l), reinterpret<void?>(qp + b * 32l), 32)
            }
        } else {
            var sp = addr < float? >(t.qscales[off / 32l])
            for (b in range64(nblocks)) {
                let o = (blk0 + b) * 34l
                var ph = reinterpret<float16?>(base + o)
                ph[0] = float16(sp[b])
                memcpy(reinterpret<void?>(base + o + 2l), reinterpret<void?>(qp + b * 32l), 32)
            }
        }
    }
}

def blob_cat3(t : Model; off0, rows0, off1, rows1, off2, rows2, n : int64) : MetalBuffer? {
    let key = intptr(unsafe(addr(t.qblob[off0])))
    if (key_exists(g_cat_blobs, key)) {
        return g_cat_blobs[key]
    }
    let nb = n / 32l
    var buf = metal_new_buffer_untracked(g_dev, uint64((rows0 + rows1 + rows2) * nb * 34l))
    var base = unsafe(reinterpret<int8?>(metal_buffer_contents(buf)))
    repack_blocks(base, 0l, t, off0, rows0, n)
    repack_blocks(base, rows0 * nb, t, off1, rows1, n)
    if (rows2 > 0l) {
        repack_blocks(base, (rows0 + rows1) * nb, t, off2, rows2, n)
    }
    g_cat_blobs[key] = buf
    return buf
}
// the pending pipelined step (one deep): everything finish_pending_step needs to wait, read
// back, and release AFTER the next step is committed behind it. Sessions are borrowed pointers
// (caller-owned) — always cleared, never deleted.

var g_lp_active = false
var g_lp_cbs : array<MetalCommandBuffer?>
var g_lp_pooled : array<tuple<buf : MetalBuffer?; size : uint64>>
var g_lp_upooled : array<tuple<buf : MetalBuffer?; size : uint64>>
var g_lp_blog : MetalBuffer?
var g_lp_xb : MetalBuffer?      // production sync + CPU-classifier path: x_b writeback
var g_lp_xb_dst : void?         // ... into the caller's workspace (borrowed, sync-only)
var g_lp_xb_bytes = 0l
var g_lp_err = false            // sticky per-step dispatch-error signal for sync callers
var g_lp_sessions : array<Session?>
var g_lp_positions : array<int64>
var g_lp_koffs : array<uint64>
var g_lp_voffs : array<uint64>
var g_lp_caps : array<int64>
var g_lp_nrows = 0l
var g_lp_layers = 0l
var g_lp_seq = 0l
var g_lp_vocab = 0l
var g_lp_rowb = 0l
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
        if (empty(err)) {
            err = metal_command_buffer_error(c_i)
        }
        g_sched_b_ms += (metal_command_buffer_kernel_end_time(c_i) - metal_command_buffer_kernel_start_time(c_i)) * 1000.0lf
    }
    // gpu = first-start .. last-end envelope; handoff = the TRUE inter-step GPU idle — this step's
    // start vs whichever bound it last (the previous step's gpu end, or its own sched end)
    let gs0 = metal_command_buffer_gpu_start_time(g_lp_cbs[0])
    let ge_last = metal_command_buffer_gpu_end_time(g_lp_cbs[length(g_lp_cbs) - 1])
    g_gpu_b_ms += (ge_last - gs0) * 1000.0lf
    g_handoff_b_ms += max(0.0lf, (gs0 - max(g_lp_prev_gpu_end, metal_command_buffer_kernel_end_time(g_lp_cbs[0]))) * 1000.0lf)
    if (has_env_variable("DASLLAMA_METAL_PIPE_DEBUG")) {   // per-step pipeline trace (first steps + outliers)
        let gap_ms = (gs0 - g_lp_prev_gpu_end) * 1000.0lf
        let env_ms = (ge_last - gs0) * 1000.0lf
        if (g_batch_steps < 8l || gap_ms > 1.0lf) {
            to_log(LOG_INFO, "metal pipe step {g_batch_steps}: gap={gap_ms}ms env={env_ms}ms\n")
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
                memcpy(g_lp_xb_dst, metal_buffer_contents(g_lp_xb), int(g_lp_xb_bytes))
            }
            let ab = reinterpret<uint8?>(metal_buffer_contents(g_arena))
            let lgp = g_lp_blog != null ? reinterpret<uint8?>(metal_buffer_contents(g_lp_blog)) : null
            for (i in range64(g_lp_nrows)) {
                var sp = g_lp_sessions[i]
                let pos = g_lp_positions[i]
                let prow = kv_phys_row(*sp, pos)
                let cap = g_lp_caps[i]
                for (l in range64(g_lp_layers)) {
                    memcpy(reinterpret<void?>(kv_layer_kbase(*sp, l, g_lp_seq) + prow * g_lp_rowb),
                        reinterpret<void?>(ab + (g_lp_koffs[i] + uint64((l * cap + pos) * g_lp_rowb))), int(g_lp_rowb))
                    memcpy(reinterpret<void?>(kv_layer_vbase(*sp, l, g_lp_seq) + prow * g_lp_rowb),
                        reinterpret<void?>(ab + (g_lp_voffs[i] + uint64((l * cap + pos) * g_lp_rowb))), int(g_lp_rowb))
                }
                if (lgp != null) {
                    memcpy(addr < void? >(sp.logits[0]),
                        reinterpret<void?>(lgp + i * g_lp_vocab * 4l), int(g_lp_vocab * 4l))
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
        to_log(LOG_ERROR, "dasLLAMA metal batch decode: dispatch failed ({err}) — pipelined step lost\n")
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
    return true
}

// ===== the weights-epoch guard =====
// g_regions/g_cat_blobs key by host address; a model reload makes those keys lie (see
// weights_epoch). Drivers flush at entry when the epoch moved, after quiescing in-flight work.
var g_weight_cache_epoch = -1l

def weight_caches_stale() : bool {
    return weights_epoch() != g_weight_cache_epoch
}

def weight_caches_flush_on_reload {
    if (!weight_caches_stale()) {
        return
    }
    finish_pending_step()
    metal_blob_regions_release()
    for (buf in values(g_cat_blobs)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_cat_blobs
    }
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
    nc_logits_release()
    metal_blob_regions_release()   // the batch mm path creates blobs in the prefill module's cache
    for (buf in values(g_cat_blobs)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_cat_blobs
    }
    for (mr in values(g_mirrors)) {
        mirror_release(mr)
    }
    unsafe {
        delete g_mirrors
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
    arch                // not the llama family graph
    backend_repack      // repack backend: interleaved weight layout unreadable by the GEMM kernel
    config              // feature-flag config the kernels don't implement (neox/bias/qk-norm/softcap/...)
    shape               // head/dim alignment outside the kernel grids
    layers              // per-layer non-uniformity (kv_dim/head_size/hidden/sliding/missing wv)
    rope_rows           // the window's rope table not built
    device              // metal_prefill_init failed (no device / PSO build failure)
    gpu_error           // dispatch failed at execution time
}

var g_prefill_declines = 0l
var g_prefill_declines_by_reason : table<string; int64>

def decline(why : MetalPrefillDecline) {
    g_prefill_declines++
    g_prefill_declines_by_reason["{why}"]++
    if (why != MetalPrefillDecline.npos_window) {   // the one POLICY decline — see require_or_panic
        require_or_panic("prefill", "{why}")
    }
}

//! Declines-by-reason since load — reason name -> count (the first failing gate clause per
//! declined prefill). The support-matrix test asserts WHY a config declined; the stage report
//! prints it alongside the decode tables.
def public metal_prefill_declines() : table<string; int64> {
    return clone_to_move(g_prefill_declines_by_reason)
}

var g_blob_regions : table<uint64; MetalBuffer?>    // quant base address -> resident block_q8_0 blob (lcpp mul_mm path)
var g_repack_us = 0l

//! Release the shared block_q8_0 blob cache — BOTH drivers' shutdowns call this (the prefill
//! and batch-decode drivers create blobs here), and each shutdown must leave the count at zero
//! for the leak gates. Blobs lazily rebuild on the next use.
def public metal_blob_regions_release {
    for (buf in values(g_blob_regions)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_blob_regions
    }
    g_repack_us = 0l
}

//! Resident block_q8_0 blob for one weight region — llama.cpp's mul_mm layout (34B blocks: half
//! scale + 32 int8 quants), cached by quant base address, shared across prefill/batch-decode.
//! `s16` reads raw f16 bits; f32->f16 narrowing is exact for GGUF-q8_0-sourced weights.
def public metal_blob_region(dev : MetalDevice?; qsrc, ssrc : void?; rows, k : int64; s16 : bool) : MetalBuffer? {
    // the scale encoding is part of the identity: the same quant region blobbed under f32 vs raw
    // f16 scales must not share a cache slot (the top bit is never set in a user-space address)
    let key = intptr(qsrc) | (s16 ? 0x8000000000000000ul : 0ul)
    if (key_exists(g_blob_regions, key)) {
        return g_blob_regions[key]
    }
    let ts = ref_time_ticks()
    let nblocks = int(rows * (k / 32l))
    var buf = metal_new_buffer_untracked(dev, uint64(nblocks) * 34ul)
    var base = unsafe(reinterpret<int8?>(metal_buffer_contents(buf)))
    var qp = unsafe(reinterpret<int8?>(qsrc))
    var sp = unsafe(reinterpret<float?>(ssrc))
    var sp16 = unsafe(reinterpret<uint16?>(ssrc))
    for (b in range(nblocks)) {
        let off = b * 34
        unsafe {
            if (s16) {
                var pb = reinterpret<uint16?>(base + off)
                pb[0] = sp16[b]
            } else {
                var ph = reinterpret<float16?>(base + off)
                ph[0] = float16(sp[b])
            }
            let src = b * 32
            for (c in range(32)) {
                base[off + 2 + c] = qp[src + c]
            }
        }
    }
    g_blob_regions[key] = buf
    g_repack_us += int64(get_time_usec(ts))
    return buf
}
