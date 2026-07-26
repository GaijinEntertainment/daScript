options gen2
options persistent_heap

require dastest/testing_boost
require metal/msl_shader
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — GPU driver half, Apple builds only
require daslib/fio
require math  // nolint:STYLE030 — used only inside the Apple static_if half
require strings

// The Metal decode FRAMING lab — the emission arc's orchestration detector. A decode token at
// the 87 t/s bar has an 11.5ms budget, and the GEMV weight stream alone costs ~10.3ms (see
// bench_metal_gemv_kernels) — so everything else (encode, hazard analysis, launch gaps between
// ~340 tiny dispatches, commit+wait wake latency, logits readback, CPU sampling) must fit the
// remaining ~1.2ms. This lab replays a 28-layer-shaped command buffer with ZERO math per
// "token" and measures that orchestration ceiling directly.
//
// Modes:
//   serial   — per token: encode D dispatches into one cb, commit, wait. The floor.
//   tail     — serial + the per-token CPU tail: 513KB logits readback + a serial argmax over
//              the 3B vocab (production uses parallel_argmax — this overestimates a little).
//   overlap  — encode-ahead: token N+1's cb is encoded (and committed) while the GPU runs
//              token N — the P3 driver experiment, measured here without the driver.
//
// Dispatches alternate write targets across two tracked activation buffers (the residual-stream
// hazard chain that orders a real layer stack) and read three untracked weight stand-ins each
// (binding churn). Grid = 768 threadgroups x 128 (a q3b-GEMV-shaped launch).
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/bench_metal_framing.das
//
// DASLLAMA_FRAME_TOKENS (default 32) / DASLLAMA_FRAME_DISPATCHES (default 340)

// Minimal grid-guarded kernel: one read from each weight stand-in + one write to the chained
// activation buffer — enough for hazard tracking to see the same dependency shape as real ops.
class MetalFrameLab {
    @ssbo @binding = 0 y : array<float>
    @ssbo @binding = 1 wa : array<float>
    @ssbo @binding = 2 wb : array<float>
    @ssbo @binding = 3 wc : array<float>
    @uniform @binding = 4 n : uint

    [metal_kernel(name="metal_frame_lab_msl")]
    def metal_frame_lab {
        let gid = gl_GlobalInvocationID.x
        if (gid >= n) {
            return
        }
        y[gid] += wa[0u] + wb[0u] + wc[0u]
    }
}

def private env_or(name : string; dflt : int) : int {
    return has_env_variable(name) ? to_int(get_env_variable(name)) : dflt
}

// encode one token's worth of dispatches into a fresh cb; returns the committed cb.
// barriers=true swaps the serial encoder for a CONCURRENT one with an explicit buffer-scope
// memory barrier between every dispatch — the ggml-metal encode model, worst-case barrier count
// (a real chain elides barriers between independent neighbors)
def private encode_token(queue; pso; acts; weights; nbuf; ndis : int; barriers : bool = false) {
    var cb = metal_new_command_buffer(queue)
    var enc = barriers ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
    metal_set_pipeline(enc, pso)
    for (dis in range(ndis)) {
        if (barriers && dis > 0) {
            metal_memory_barrier(enc)
        }
        metal_set_buffer(enc, acts[dis % length(acts)], 0ul, 0)
        metal_set_buffer(enc, weights[dis % length(weights)], 0ul, 1)
        metal_set_buffer(enc, weights[(dis + 1) % length(weights)], 0ul, 2)
        metal_set_buffer(enc, weights[(dis + 2) % length(weights)], 0ul, 3)
        metal_set_buffer(enc, nbuf, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(768u, 1u, 1u), uint3(128u, 1u, 1u))
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    return cb
}

// the per-token CPU tail: logits readback + greedy argmax (serial — production parallel_argmax
// is faster; this is the conservative bound)
def private token_tail(logits_buf; var logits : array<float>; vocab : int64) : int64 {
    unsafe {
        memcpy(addr<void?>(logits[0]), metal_buffer_contents(logits_buf), int(vocab * 4l))
    }
    var best = 0l
    var bv = logits[0]
    for (i in range64(1l, vocab)) {
        if (logits[i] > bv) {
            bv = logits[i]
            best = i
        }
    }
    return best
}

def private report(mode : string; ndis, ntok : int; total_us : int; enc_us, gpu_ms : double) {
    let ms_tok = double(total_us) / 1000.0lf / double(ntok)
    let tps = 1000.0lf / ms_tok
    let enc_tok = enc_us / 1000.0lf / double(ntok)
    let gpu_tok = gpu_ms / double(ntok)
    to_log(LOG_INFO, "{mode}  D={ndis}  {ms_tok} ms/token  ceiling {tps} t/s  (encode {enc_tok} ms, gpu {gpu_tok} ms)\n")
}

def private run_framing(b) {   // untyped param = generic — non-Apple builds never compile the Metal-typed body
    let ntok = env_or("DASLLAMA_FRAME_TOKENS", 32)
    let ndis = env_or("DASLLAMA_FRAME_DISPATCHES", 340)
    let vocab = 128256l
    var dev = metal_create_system_default_device()
    if (dev == null) {
        to_log(LOG_WARNING, "skipped: no Metal device\n")
        return
    }
    to_log(LOG_INFO, "device={metal_device_name(dev)} tokens={ntok} dispatches/token={ndis}\n")
    var queue = metal_new_command_queue(dev)
    var err : string
    var pso = pipeline_from_source(dev, metal_frame_lab_msl, metal_frame_lab_msl_entry, true, err)
    b |> success(pso != null, "framing kernel compiles")
    if (pso == null) {
        to_log(LOG_ERROR, "compile failed: {err}\n")
        metal_release(queue)
        metal_release(dev)
        return
    }
    // two tracked activation buffers (the hazard chain) + four untracked weight stand-ins
    var acts <- [for (_i in range(2)); metal_new_buffer(dev, uint64(3072l * 4l))]
    var weights <- [for (_i in range(4)); metal_new_buffer_untracked(dev, uint64(256l * 1024l))]
    var logits_buf = metal_new_buffer(dev, uint64(vocab * 4l))
    var nbuf = metal_new_buffer(dev, 4ul)
    unsafe {
        var p = reinterpret<uint?>(metal_buffer_contents(nbuf))
        p[0] = uint(3072)
    }
    var logits : array<float>
    logits |> resize(int(vocab))
    var sink = 0l

    // warmup (PSO caches, first-cb costs)
    var wcb = encode_token(queue, pso, acts, weights, nbuf, ndis)
    metal_wait_until_completed(wcb)
    metal_release(wcb)

    // ===== serial: encode, commit, wait =====
    var enc_us = 0.0lf
    var gpu_ms = 0.0lf
    var t0 = ref_time_ticks()
    for (_tok in range(ntok)) {
        let e0 = ref_time_ticks()
        var cb = encode_token(queue, pso, acts, weights, nbuf, ndis)
        enc_us += double(get_time_usec(e0))
        metal_wait_until_completed(cb)
        gpu_ms += (metal_command_buffer_gpu_end_time(cb) - metal_command_buffer_gpu_start_time(cb)) * 1000.0lf
        metal_release(cb)
    }
    report("serial ", ndis, ntok, get_time_usec(t0), enc_us, gpu_ms)

    // ===== tail: serial + logits readback + argmax =====
    enc_us = 0.0lf
    gpu_ms = 0.0lf
    t0 = ref_time_ticks()
    for (_tok in range(ntok)) {
        let e0 = ref_time_ticks()
        var cb = encode_token(queue, pso, acts, weights, nbuf, ndis)
        enc_us += double(get_time_usec(e0))
        metal_wait_until_completed(cb)
        gpu_ms += (metal_command_buffer_gpu_end_time(cb) - metal_command_buffer_gpu_start_time(cb)) * 1000.0lf
        metal_release(cb)
        sink += token_tail(logits_buf, logits, vocab)
    }
    report("tail   ", ndis, ntok, get_time_usec(t0), enc_us, gpu_ms)

    // ===== barrier: concurrent encoder + explicit barrier between EVERY dispatch =====
    enc_us = 0.0lf
    gpu_ms = 0.0lf
    t0 = ref_time_ticks()
    for (_tok in range(ntok)) {
        let e0 = ref_time_ticks()
        var cb = encode_token(queue, pso, acts, weights, nbuf, ndis, true)
        enc_us += double(get_time_usec(e0))
        metal_wait_until_completed(cb)
        gpu_ms += (metal_command_buffer_gpu_end_time(cb) - metal_command_buffer_gpu_start_time(cb)) * 1000.0lf
        metal_release(cb)
    }
    report("barrier", ndis, ntok, get_time_usec(t0), enc_us, gpu_ms)

    // ===== overlap: encode token N+1 while the GPU runs token N =====
    enc_us = 0.0lf
    gpu_ms = 0.0lf
    t0 = ref_time_ticks()
    var e0 = ref_time_ticks()
    var cur = encode_token(queue, pso, acts, weights, nbuf, ndis)
    enc_us += double(get_time_usec(e0))
    for (tok in range(ntok)) {
        var nxt : MetalCommandBuffer?
        if (tok + 1 < ntok) {
            e0 = ref_time_ticks()
            nxt = encode_token(queue, pso, acts, weights, nbuf, ndis)   // GPU is running `cur`
            enc_us += double(get_time_usec(e0))
        }
        metal_wait_until_completed(cur)
        gpu_ms += (metal_command_buffer_gpu_end_time(cur) - metal_command_buffer_gpu_start_time(cur)) * 1000.0lf
        metal_release(cur)
        sink += token_tail(logits_buf, logits, vocab)
        cur = nxt
    }
    report("overlap", ndis, ntok, get_time_usec(t0), enc_us, gpu_ms)
    to_log(LOG_INFO, "argmax sink {sink}\n")

    for (a in acts) {
        metal_release(a)
    }
    acts |> clear()
    unsafe {
        delete acts
    }
    for (w in weights) {
        metal_release(w)
    }
    weights |> clear()
    unsafe {
        delete weights
    }
    metal_release(logits_buf)
    metal_release(nbuf)
    metal_release(pso)
    metal_release(queue)
    metal_release(dev)
    delete logits
    assert_no_metal_leaks()
}

[benchmark, unused_argument(b)]   // b is consumed only inside the Apple static_if half
def bench_metal_framing(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        run_framing(b)
    }
}
