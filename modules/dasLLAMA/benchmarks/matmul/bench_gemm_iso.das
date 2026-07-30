options gen2
options persistent_heap

require dastest/testing_boost
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_math_gen  // registers the arm64-gen/x64-gen repack tier — without it the repack half runs portable
require ?das_metal dasllama/dasllama_metal_gemm  // nolint:STYLE030 — registers the "metal" GPU batch donor (DASLLAMA_PIN_BATCH_BACKEND=metal) on Apple builds
require dasllama/dasllama_convert
require daslib/jobque_boost
require daslib/fio
require math
require strings

// Isolated batched-GEMM throughput, ours (production matmul_q8q8_batch) on REAL model matmul
// shapes at the prefill batch width (ntok = 512), THREADED, reported as GMAC/s aggregate AND per
// core. This is the das half of the pure-GEMM ladder (harness/pure_gemm_ladder.sh) — the lcpp
// half is `test-backend-ops perf -o MUL_MAT` with the same shapes (harness/backend_ops_shapes.patch);
// lcpp reports GFLOPS = 2·GMAC/s. Sweep DAS_JOBQUE_THREADS=2,4,8,… to read the parallel_for
// scaling curve (values <= 1 are a fatal error — N means N total lanes incl. the computing main,
// so there is no honest 1-lane jobque run); per-lane GMAC/s at 2 lanes is the near-kernel ceiling;
// how far the aggregate climbs as lanes grow tells us whether parallel_for or the kernel is the wall.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/matmul/bench_gemm_iso.das
//   DASLLAMA_BENCH_MODEL=llama8b DAS_JOBQUE_THREADS=8 bin/daslang -jit dastest/dastest.das -- --bench --test .../bench_gemm_iso.das
//
// DASLLAMA_BENCH_MODEL: tinyllama (default) | smol135m | qwen06b | gemma1b | llama1b | qwen4b | llama8b
// DASLLAMA_BENCH_NTOK: batch width override (default 512; 32 = one amx token block)

var NTOK = 512l

// Fleet GEMM geometries (from GGUF metadata, 2026-07-09), (name, n, d) triples: n = reduction
// (input dim), d = output rows. kv (dim→kv_dim), q (dim→qd), wo (qd→dim), w13 (dim→ffn),
// w2 (ffn→dim), cls (dim→vocab). tinyllama keeps the original row set for comparability.
def private shapes_for(tag : string) : array<tuple<string; int64; int64>> {
    if (tag == "smol135m") {
        return <- [("kv ", 576l, 192l), ("q  ", 576l, 576l), ("w13", 576l, 1536l),
                   ("w2 ", 1536l, 576l), ("cls", 576l, 49152l)]
    }
    if (tag == "qwen06b") {
        return <- [("kv ", 1024l, 1024l), ("q  ", 1024l, 2048l), ("wo ", 2048l, 1024l),
                   ("w13", 1024l, 3072l), ("w2 ", 3072l, 1024l), ("cls", 1024l, 151936l)]
    }
    if (tag == "gemma1b") {
        return <- [("kv ", 1152l, 256l), ("q  ", 1152l, 1024l), ("wo ", 1024l, 1152l),
                   ("w13", 1152l, 6912l), ("w2 ", 6912l, 1152l), ("cls", 1152l, 262144l)]
    }
    if (tag == "llama1b") {
        return <- [("kv ", 2048l, 512l), ("q  ", 2048l, 2048l), ("w13", 2048l, 8192l),
                   ("w2 ", 8192l, 2048l), ("cls", 2048l, 128256l)]
    }
    if (tag == "qwen4b") {
        return <- [("kv ", 2560l, 1024l), ("q  ", 2560l, 4096l), ("wo ", 4096l, 2560l),
                   ("w13", 2560l, 9728l), ("w2 ", 9728l, 2560l), ("cls", 2560l, 151936l)]
    }
    if (tag == "llama8b") {
        return <- [("kv ", 4096l, 1024l), ("q  ", 4096l, 4096l), ("w13", 4096l, 14336l),
                   ("w2 ", 14336l, 4096l), ("cls", 4096l, 128256l)]
    }
    // tinyllama — the original set
    return <- [("kv ", 2048l, 512l), ("qo ", 2048l, 2048l), ("w13", 2048l, 5632l),
               ("w2 ", 5632l, 2048l), ("cls", 2048l, 32000l)]
}

// reps from a fixed MAC budget so every shape runs comparably long; floor 3 (best-of needs
// signal), cap 200 (tiny shapes would spin forever)
def private reps_for(n, d : int64) : int => clamp(int(100000000000l / (n * d * NTOK)), 3, 200)

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = float(i % 13l) * 0.1 - 0.6
    }
}

// Time `reps` calls of matmul_q8q8_batch on (q,scales) weights against (xqb,xsb) activations,
// return GMAC/s aggregate. Caller has already repacked the weight for the active backend.
def time_gemm(var y : array<float>; q : array<int8>; scales : array<float>;
              xqb : array<int8>; xsb : array<float>; n, d, ntok : int64; reps : int) : float {
    // warmup (page-in + JIT first-touch)
    matmul_q8q8_batch(y, q, scales, xqb, xsb, n, d, ntok)
    let t0 = ref_time_ticks()
    for (_r in range(reps)) {
        matmul_q8q8_batch(y, q, scales, xqb, xsb, n, d, ntok)
    }
    let dt_ns = ref_time_ticks() - t0
    // GMAC/s = macs / seconds / 1e9 = (n*d*ntok*reps) / dt_ns  (the 1e9s cancel; ref_time_ticks is ns)
    let macs = double(n * d * ntok) * double(reps)
    return float(macs / double(dt_ns))
}

def bench_one(name : string; n, d : int64; reps : int; laneq : bool) {
    let nb = n / 32l
    var w : array<float>
    fillf(w, n * d)
    let qt8 <- quantize_q8_0(w, n * d)
    // copy quants out of the tensor so we can repack in place without touching qt8
    var q : array<int8>
    var scales : array<float>
    q |> resize(n * d)
    scales |> resize(d * nb)
    for (i in range64(n * d)) { q[i] = qt8.q[i] }
    for (i in range64(d * nb)) { scales[i] = qt8.scales[i] }

    // one base activation, broadcast across NTOK tokens (timing is data-independent)
    var x : array<float>
    fillf(x, n)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(nb)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    var xqb : array<int8>
    var xsb : array<float>
    xqb |> resize(NTOK * n)
    xsb |> resize(NTOK * nb)
    for (tk in range64(NTOK)) {
        for (i in range64(n)) { xqb[tk * n + i] = xq[i] }
        for (i in range64(nb)) { xsb[tk * nb + i] = xs[i] }
    }
    var y : array<float>
    y |> resize(NTOK * d)

    if (laneq) {
        repack_q8q8_weight(q, scales, 0l, n, d)
    }
    let nthreads = get_total_hw_jobs()
    let gmacs = time_gemm(y, q, scales, xqb, xsb, n, d, NTOK, reps)
    let percore = gmacs / float(nthreads)
    let kind = laneq ? "laneq " : "sdot4x"
    to_log(LOG_INFO, "{kind} {name}  threads={nthreads}  {gmacs} GMAC/s total  {percore} GMAC/s/core\n")

    delete w
    delete q
    delete scales
    delete x
    delete xq
    delete xs
    delete xqb
    delete xsb
    delete y
}

[benchmark, unused_argument(b)]
def bench_gemm_iso(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        // A/B the fork-pool restartHeaps opt-out: set DAS_FORK_SKIP_HEAP_RESET to skip it
        let skip_reset = has_env_variable("DAS_FORK_SKIP_HEAP_RESET")
        if (skip_reset) {
            set_jobque_fork_skip_heap_reset(true)
        }
        to_log(LOG_INFO, "fork_skip_heap_reset = {skip_reset}\n")
        // A/B the worker spin window (0 disables — suspect on thermally-bound boxes: 32 spinners
        // through the serial gaps depress package clocks for the WHOLE process)
        if (has_env_variable("DAS_WORKER_SPIN_US")) {
            let spin_us = to_int(get_env_variable("DAS_WORKER_SPIN_US"))
            set_jobque_worker_spin(spin_us)
            to_log(LOG_INFO, "worker spin override: {spin_us}us\n")
        }
        // A/B the batch-oversplit knob (chunks per hw job) across alternating runs
        if (has_env_variable("DASLLAMA_BATCH_CHUNKS")) {
            let bc = to_int(get_env_variable("DASLLAMA_BATCH_CHUNKS"))
            set_q8_batch_chunks_per_job(bc)
            to_log(LOG_INFO, "batch chunks override: {bc}\n")
        }
        // A/B the matmul min-grain (rows per chunk floor); 0 = clamp off (pre-grain behavior)
        if (has_env_variable("DASLLAMA_MIN_CHUNK_ROWS")) {
            let mg = to_int(get_env_variable("DASLLAMA_MIN_CHUNK_ROWS"))
            set_matmul_min_chunk_rows(mg)
            to_log(LOG_INFO, "min chunk rows override: {mg}\n")
        }
        // backend pin for row-major A/Bs on one binary (e.g. portable vs arm64-sdot), same
        // env contract as llama3_perf; the repack half below still switches to the repack tier
        if (has_env_variable("DASLLAMA_PIN_BACKEND")) {
            pin_kernel_backend(get_env_variable("DASLLAMA_PIN_BACKEND"))
        }
        if (has_env_variable("DASLLAMA_PIN_BATCH_BACKEND")) {
            select_batch_backend(get_env_variable("DASLLAMA_PIN_BATCH_BACKEND"))
        }
        if (has_env_variable("DASLLAMA_BENCH_NTOK")) {
            NTOK = int64(to_int(get_env_variable("DASLLAMA_BENCH_NTOK")))
        }
        let model = has_env_variable("DASLLAMA_BENCH_MODEL") ? get_env_variable("DASLLAMA_BENCH_MODEL") : "tinyllama"
        var shapes <- shapes_for(model)
        to_log(LOG_INFO, "model={model} ntok={NTOK}\n")
        // DASLLAMA_BENCH_SKIP_ROWMAJOR skips the row-major half — a pin-lattice sweep re-runs
        // the repack half per manifest and the row-major rows would repeat identically
        if (!has_env_variable("DASLLAMA_BENCH_SKIP_ROWMAJOR")) {
            to_log(LOG_INFO, "=== {active_kernel_backend()} backend (row-major) ===\n")
            for (s in shapes) {
                bench_one("{s._0} {model} {s._1}x{s._2}", s._1, s._2, reps_for(s._1, s._2), false)
            }
        }

        // switch to the arch's repack tier for the rest of the run; each shape repacks its own
        // weight. pin (not select): pin runs the backend's available() witness — on amx stamps
        // that witness IS the per-process XTILEDATA enable, skipping it SIGILLs the first tile op
        let repack_backend = get_architecture_name() == "arm64" ? "arm64-gen" : "x64-gen"
        pin_kernel_backend(repack_backend)
        to_log(LOG_INFO, "=== {repack_backend} backend (tiled, interleaved repack) ===\n")
        for (s in shapes) {
            bench_one("{s._0} {model} {s._1}x{s._2}", s._1, s._2, reps_for(s._1, s._2), true)
        }
        clear_kernel_backend_pin()   // a standing pin would leak into later files in a dastest --bench process
    }
}
