options gen2
options persistent_heap

require dastest/testing_boost
require dasllama/dasllama_math
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_quant
require daslib/jobque_boost
require math

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
    with_job_que() {
        set_jobque_fork_pool(true, true)
        to_log(LOG_INFO, "=== sdot4x4 backend (row-major) ===\n")
        bench_one("qo  2048x2048 ", 2048l, 2048l, 30, false)
        bench_one("w13 2048x5632 ", 2048l, 5632l, 20, false)
        bench_one("w2  5632x2048 ", 5632l, 2048l, 20, false)
        bench_one("cls 2048x32000", 2048l, 32000l, 6, false)

        // enable laneq for the rest of the run; each shape repacks its own weight
        enable_q8q8_laneq()
        to_log(LOG_INFO, "=== laneq backend (4x4 tile, interleaved repack) ===\n")
        bench_one("qo  2048x2048 ", 2048l, 2048l, 30, true)
        bench_one("w13 2048x5632 ", 2048l, 5632l, 20, true)
        bench_one("w2  5632x2048 ", 5632l, 2048l, 20, true)
        bench_one("cls 2048x32000", 2048l, 32000l, 6, true)
    }
}
