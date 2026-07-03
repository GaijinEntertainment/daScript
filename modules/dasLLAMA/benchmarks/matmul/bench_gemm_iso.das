options gen2
options persistent_heap

require dastest/testing_boost
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_math_x64_avx  // self-registers the x64 backends on x86-64 (nothing off-x64) — without it BOTH halves run portable here
require dasllama/dasllama_quant
require daslib/jobque_boost
require daslib/fio
require math
require strings

// Isolated batched-GEMM throughput, ours (production matmul_q8q8_batch) on the REAL Llama matmul
// shapes at the prefill batch width (ntok = 512), THREADED, reported as GMAC/s aggregate AND per
// core. This is the apples-to-apples number against llama.cpp's NEON Q8 MUL_MAT (~207 GMAC/s/core,
// ~1.66 TMAC/s across 8 Firestorm cores). Sweep DAS_JOBQUE_THREADS=1,2,4,6,8,9 to read the
// parallel_for scaling curve: per-core GMAC/s at 1 thread is the raw kernel ceiling; how far the
// aggregate climbs as threads grow tells us whether parallel_for or the kernel is the wall.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/matmul/bench_gemm_iso.das
//   DAS_JOBQUE_THREADS=1 bin/daslang -jit dastest/dastest.das -- --bench --test .../bench_gemm_iso.das

let NTOK = 512l

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
        // backend pin for row-major A/Bs on one binary (e.g. x64-avx2 vs x64-avx2-acc8), same
        // env contract as llama3_perf; the repack half below still switches to the repack tier
        if (has_env_variable("DASLLAMA_PIN_BACKEND")) {
            pin_kernel_backend(get_env_variable("DASLLAMA_PIN_BACKEND"))
        }
        to_log(LOG_INFO, "=== {active_kernel_backend()} backend (row-major) ===\n")
        bench_one("kv  2048x512  ", 2048l, 512l, 100, false)
        bench_one("qo  2048x2048 ", 2048l, 2048l, 30, false)
        bench_one("w13 2048x5632 ", 2048l, 5632l, 20, false)
        bench_one("w2  5632x2048 ", 5632l, 2048l, 20, false)
        bench_one("cls 2048x32000", 2048l, 32000l, 6, false)

        // switch to the arch's repack tier for the rest of the run; each shape repacks its own weight
        let repack_backend = get_architecture_name() == "arm64" ? "arm64-laneq" : "x64-avx2-repack"
        select_kernel_backend(repack_backend)
        to_log(LOG_INFO, "=== {repack_backend} backend (tiled, interleaved repack) ===\n")
        bench_one("kv  2048x512  ", 2048l, 512l, 100, true)
        bench_one("qo  2048x2048 ", 2048l, 2048l, 30, true)
        bench_one("w13 2048x5632 ", 2048l, 5632l, 20, true)
        bench_one("w2  5632x2048 ", 5632l, 2048l, 20, true)
        bench_one("cls 2048x32000", 2048l, 32000l, 6, true)
    }
}
