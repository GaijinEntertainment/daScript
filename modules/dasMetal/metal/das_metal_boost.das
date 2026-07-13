options gen2
options indenting = 4

// Host sugar over the das_metal binding (Apple-only — this file requires the C++ module, so
// consumers guard with `require ?das_metal metal/das_metal_boost` + static_if
// builtin_module_exists(das_metal)). v1 covers what the behavioral gate needs: scoped device
// access, the source->pipeline chain with intermediate handles released, a one-line 1D dispatch,
// and the leak assert.

module das_metal_boost shared public

require das_metal public

//! Scoped system-default device: creates, invokes, releases. `dev` is null when the box has no
//! Metal device — the block decides whether that is a skip or a failure.
def public with_metal_device(blk : block<(dev : MetalDevice?) : void>) {
    let dev = metal_create_system_default_device()
    invoke(blk, dev)
    if (dev != null) {
        metal_release(dev)
    }
}

//! MSL source -> compute pipeline (library and function handles released on the way; the pipeline
//! keeps what it needs). Returns null with `error` filled on any failure — including an entry
//! point missing from a successfully compiled library.
def public pipeline_from_source(dev : MetalDevice?; source, entry : string; fastmath : bool; var error : string&) : MetalComputePipeline? {
    let lib = metal_new_library_from_source(dev, source, fastmath, error)
    if (lib == null) {
        return null
    }
    let fn = metal_new_function(lib, entry)
    if (fn == null) {
        metal_release(lib)
        error = "kernel entry point `{entry}` not found in compiled library"
        return null
    }
    var pso = metal_new_compute_pipeline(dev, fn, error)
    metal_release(fn)
    metal_release(lib)
    return pso
}

//! One 1D dispatch: encode pipeline + buffers (bound to [[buffer(0..N-1)]] in array order),
//! dispatch ceil(threads/threads_per_group) threadgroups, commit, wait. Returns false with
//! `error` filled when the command buffer reports a GPU-side failure.
def public run_compute_1d(queue : MetalCommandQueue?; pso : MetalComputePipeline?; buffers : array<MetalBuffer?>; threads, threads_per_group : uint; var error : string&) : bool {
    let cb = metal_new_command_buffer(queue)
    let enc = metal_new_compute_encoder(cb)
    metal_set_pipeline(enc, pso)
    for (buf, index in buffers, range(length(buffers))) {
        metal_set_buffer(enc, buf, 0ul, index)
    }
    let groups = (threads + threads_per_group - 1u) / threads_per_group
    metal_dispatch_threadgroups(enc, uint3(groups, 1u, 1u), uint3(threads_per_group, 1u, 1u))
    metal_end_encoding(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    error = metal_command_buffer_error(cb)
    metal_release(enc)
    metal_release(cb)
    return empty(error)
}

//! The leak gate: call at the end of every tests/metal file — Metal objects are invisible to all
//! six das leak detectors, so the shim-side counter is the only witness.
def public assert_no_metal_leaks {
    let live = metal_live_object_count()
    if (live != 0l) {
        panic("metal_live_object_count() == {live} — every handle must be metal_release()d")
    }
}

// ===== host perf plumbing (Phase 5) =====

//! Pipeline cache: source-text compiles are the dominant per-pipeline cost, so a driver that
//! dispatches the same kernels every call keeps them here. Keyed by entry + fastmath + source
//! (the source participates so two kernels sharing an entry name cannot collide). Drain releases
//! everything — the cache owns its pipelines.
struct public MetalPipelineCache {
    cache : table<string; MetalComputePipeline?>
}

def public pipeline_cached(var pc : MetalPipelineCache; dev : MetalDevice?; source, entry : string; fastmath : bool; var error : string&) : MetalComputePipeline? {
    let key = "{entry}|{fastmath}|{source}"
    if (key_exists(pc.cache, key)) {
        return pc.cache[key]
    }
    var pso = pipeline_from_source(dev, source, entry, fastmath, error)
    if (pso != null) {
        pc.cache[key] = pso
    }
    return pso
}

def public cache_drain(var pc : MetalPipelineCache) {
    for (pso in values(pc.cache)) {
        if (pso != null) {
            metal_release(pso)
        }
        pso = null                  // handle values must not be finalized by the table delete
    }
    unsafe {
        delete pc.cache
    }
}

//! Buffer pool: size-bucketed free lists over metal_new_buffer (buckets are the next power of
//! two, so `bytes` at release must equal `bytes` at acquire for the buffer to re-file into its
//! bucket — deterministic for fixed-shape drivers). Acquired buffers stay owned by the caller
//! until released back; drain frees the free lists only.
struct public MetalBufferPool {
    free_bufs : table<uint64; array<MetalBuffer?>>
}

def private pool_bucket(bytes : uint64) : uint64 {
    var b = 256ul                   // floor keeps tiny uniforms from fragmenting buckets
    while (b < bytes) {
        b *= 2ul
    }
    return b
}

def public pool_acquire(var pool : MetalBufferPool; dev : MetalDevice?; bytes : uint64) : MetalBuffer? {
    let bucket = pool_bucket(bytes)
    var lst & = unsafe(pool.free_bufs[bucket])
    if (!empty(lst)) {
        var buf = lst[length(lst) - 1]
        lst |> pop()
        return buf
    }
    return metal_new_buffer(dev, bucket)
}

def public pool_release(var pool : MetalBufferPool; var buf : MetalBuffer?; bytes : uint64) {
    pool.free_bufs[pool_bucket(bytes)] |> push(buf)
}

def public pool_drain(var pool : MetalBufferPool) {
    for (lst in values(pool.free_bufs)) {
        for (buf in lst) {
            metal_release(buf)
        }
        lst |> clear()              // handles were released above — empty lists finalize safely
    }
    unsafe {
        delete pool.free_bufs
    }
}

//! One command buffer + one compute encoder around `blk`: encode any number of set_pipeline /
//! set_buffer / dispatch calls, then end + commit + wait once. Metal's default hazard tracking
//! orders same-buffer dispatches within the encoder. Returns false with `error` filled when the
//! command buffer reports a GPU-side failure.
def public with_compute_encoder(queue : MetalCommandQueue?; var error : string&; blk : block<(enc : MetalComputeEncoder?) : void>) : bool {
    var gpu_ms = 0.0lf
    return with_compute_encoder_timed(queue, error, gpu_ms, blk)
}

//! The timed twin: `gpu_ms` gets the command buffer's GPU execution time (GPUEndTime −
//! GPUStartTime, milliseconds) — the honest GPU-busy number, as opposed to the host wall time
//! around commit+wait which folds in scheduling and readback.
def public with_compute_encoder_timed(queue : MetalCommandQueue?; var error : string&; var gpu_ms : double&; blk : block<(enc : MetalComputeEncoder?) : void>) : bool {
    var encode_ms = 0.0lf
    return with_compute_encoder_timed(queue, error, gpu_ms, encode_ms, blk)
}

//! The 4-out twin: also reports the host-side ENCODE time (block entry to commit) — the
//! encode-CPU vs GPU-window split that attributes command-buffer overhead.
def public with_compute_encoder_timed(queue : MetalCommandQueue?; var error : string&; var gpu_ms : double&; var encode_ms : double&; blk : block<(enc : MetalComputeEncoder?) : void>) : bool {
    let cb = metal_new_command_buffer(queue)
    let enc = metal_new_compute_encoder(cb)
    let ts_enc = ref_time_ticks()
    invoke(blk, enc)
    metal_end_encoding(enc)
    encode_ms = double(get_time_usec(ts_enc)) / 1000.0lf
    metal_commit(cb)
    metal_wait_until_completed(cb)
    error = metal_command_buffer_error(cb)
    gpu_ms = (metal_command_buffer_gpu_end_time(cb) - metal_command_buffer_gpu_start_time(cb)) * 1000.0lf
    metal_release(enc)
    metal_release(cb)
    return empty(error)
}
