options gen2
options persistent_heap
options _jit_fast_math = true   // match the end-to-end config AND ggml's -O3 FP laxity (kernel-to-kernel fairness)

// RAW single-core kernel throughput — no jobque, no threading, no pipeline. The production
// matmul_q8q8_batch runs INLINE on the calling thread: matmul_par_threshold is pushed to infinity,
// so maybe_parallel_for compiles to a plain invoke (no with_job_que anywhere in this file). This is
// the kernel-to-kernel denominator against llama.cpp's ggml_vec_dot_q8_0_q8_0 measured the same way
// (examples/kernel-bench in the llama.cpp tree). Interleaved best-of across backends per shape,
// tolerance-gated vs portable. Run: daslang -jit gemm_1core_probe.das

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_x64_avx
require dasllama/dasllama_quant
require daslib/jobque_boost
require daslib/fio
require math
require strings
require daslib/strings_boost

let NTOK = 128l
let ROUNDS = 6

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(int(cnt))
    for (i in range64(cnt)) {
        a[i] = float(i % 13l) * 0.1 - 0.6
    }
}

def maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (i in range(length(a))) {
        m = max(m, abs(a[i] - b[i]))
    }
    return m
}

struct Cand {
    name : string
    repacked : bool
}

def bench_shape(name : string; n, d : int64; reps : int) {
    let nb = n / 32l
    var w : array<float>
    fillf(w, n * d)
    let qt <- quantize_q8_0(w, n * d)
    var q : array<int8>
    var s : array<float>
    q |> resize(int(n * d))
    s |> resize(int(d * nb))
    for (i in range64(n * d)) {
        q[i] = qt.q[i]
    }
    for (i in range64(d * nb)) {
        s[i] = qt.scales[i]
    }
    // separate weight copy for the repack tier (repacks in place)
    var qr := q
    var sr := s
    select_kernel_backend("x64-avx2-repack")
    repack_q8q8_weight(qr, sr, 0l, n, d)

    var x : array<float>
    fillf(x, n)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(int(n))
    xs |> resize(int(nb))
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    var xqb : array<int8>
    var xsb : array<float>
    xqb |> resize(int(NTOK * n))
    xsb |> resize(int(NTOK * nb))
    for (tk in range64(NTOK)) {
        for (i in range64(n)) {
            xqb[int(tk * n + i)] = xq[int(i)]
        }
        for (i in range64(nb)) {
            xsb[int(tk * nb + i)] = xs[int(i)]
        }
    }

    let cands <- [Cand(name = "portable", repacked = false),
        Cand(name = "x64-avx2", repacked = false),
        Cand(name = "x64-avx2-acc8", repacked = false),
        Cand(name = "x64-avx2-repack", repacked = true)]
    let nc = length(cands)

    var y : array<float>
    var yref : array<float>
    y |> resize(int(NTOK * d))
    yref |> resize(int(NTOK * d))

    // correctness gate: every backend within 1e-3 of portable on the same data
    select_kernel_backend("portable")
    matmul_q8q8_batch(yref, q, s, xqb, xsb, n, d, NTOK)
    var ok : array<bool>
    ok |> resize(nc)
    for (ci in range(nc)) {
        select_kernel_backend(cands[ci].name)
        if (cands[ci].repacked) {
            matmul_q8q8_batch(y, qr, sr, xqb, xsb, n, d, NTOK)
        } else {
            matmul_q8q8_batch(y, q, s, xqb, xsb, n, d, NTOK)
        }
        let md = maxdiff(yref, y)
        ok[ci] = md < 1.0e-3
        if (!ok[ci]) {
            print("  !! {cands[ci].name} maxdiff {md} vs portable — REJECTED\n")
        }
    }

    var best : array<double>
    best |> resize(nc)
    for (ci in range(nc)) {
        best[ci] = 1.0e30lf
    }
    for (_round in range(ROUNDS)) {
        for (ci in range(nc)) {
            if (!ok[ci]) {
                continue
            }
            select_kernel_backend(cands[ci].name)
            let t0 = ref_time_ticks()
            for (_rep in range(reps)) {
                if (cands[ci].repacked) {
                    matmul_q8q8_batch(y, qr, sr, xqb, xsb, n, d, NTOK)
                } else {
                    matmul_q8q8_batch(y, q, s, xqb, xsb, n, d, NTOK)
                }
            }
            let us = double(get_time_usec(t0))
            if (us < best[ci]) {
                best[ci] = us
            }
        }
    }

    print("--- {name}  (n={n} d={d} ntok={NTOK}, x{reps}) ---\n")
    let macs = double(n * d * NTOK) * double(reps)
    for (ci in range(nc)) {
        if (!ok[ci]) {
            continue
        }
        let gmacs = macs / (best[ci] * 1000.0lf)
        print("  {cands[ci].name}: {gmacs} GMAC/s  ({best[ci]} us best-of-{ROUNDS})\n")
    }

    delete w
    delete q
    delete s
    delete qr
    delete sr
    delete x
    delete xq
    delete xs
    delete xqb
    delete xsb
    delete y
    delete yref
}

// ===== focus mode (the scaling-ladder worker) =====
// DASLLAMA_FOCUS_BACKEND=<name> selects one backend + one shape (DASLLAMA_FOCUS_SHAPE=kv|qo|w13|w2)
// and times ONLY that, calibrated to a ~6s window, one GMAC/s line. Two modes:
//   inline (default)              — rung A: no jobque at all; spawn K processes to measure what the
//                                   HARDWARE gives K copies of the loop (zero scheduler)
//   DASLLAMA_FOCUS_FORKED=1       — rung B: one process, threshold 0 (always parallel_for), workers
//                                   from DAS_JOBQUE_THREADS; B minus A at each K = the scheduler's bill

def focus_bench(be : string; shape : string; n, d, ntok : int64; forked : bool) {
    let nb = n / 32l
    var w : array<float>
    fillf(w, n * d)
    let qt <- quantize_q8_0(w, n * d)
    var q : array<int8>
    var s : array<float>
    q |> resize(int(n * d))
    s |> resize(int(d * nb))
    for (i in range64(n * d)) {
        q[i] = qt.q[i]
    }
    for (i in range64(d * nb)) {
        s[i] = qt.scales[i]
    }
    select_kernel_backend(be)
    if (be == "x64-avx2-repack") {
        repack_q8q8_weight(q, s, 0l, n, d)
    }
    var x : array<float>
    fillf(x, n)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(int(n))
    xs |> resize(int(nb))
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    var xqb : array<int8>
    var xsb : array<float>
    xqb |> resize(int(ntok * n))
    xsb |> resize(int(ntok * nb))
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xqb[int(tk * n + i)] = xq[int(i)]
        }
        for (i in range64(nb)) {
            xsb[int(tk * nb + i)] = xs[int(i)]
        }
    }
    var y : array<float>
    y |> resize(int(ntok * d))

    // DASLLAMA_BATCH_CHUNKS="1,4" A/Bs the batch-oversplit knob INTERLEAVED within this process
    // (best-of-6 per value, adjacent in time — immune to box drift); absent = current knob, one shot
    var chunk_cands : array<int>
    if (has_env_variable("DASLLAMA_BATCH_CHUNKS")) {
        for (part in split(get_env_variable("DASLLAMA_BATCH_CHUNKS"), ",")) {
            if (!empty(part)) {
                chunk_cands |> push(to_int(part))
            }
        }
    }
    if (empty(chunk_cands)) {
        chunk_cands |> push(get_q8_batch_chunks_per_job())
    }
    let ncand = length(chunk_cands)

    // calibrate the measurement window: ~6s single-config so K concurrent processes overlap (rung A);
    // ~1.5s per window when interleaving configs (dispatch tax still ≪ window)
    let t0 = ref_time_ticks()
    matmul_q8q8_batch(y, q, s, xqb, xsb, n, d, ntok)
    let one_us = double(get_time_usec(t0))
    let window_us = ncand > 1 ? 1.5e6lf : 6.0e6lf
    var reps = int(window_us / max(one_us, 100.0lf))
    if (reps < 1) {
        reps = 1
    }
    let rounds = ncand > 1 ? 6 : 1
    var bestv : array<double>
    bestv |> resize(ncand)
    for (ci in range(ncand)) {
        bestv[ci] = 1.0e30lf
    }
    for (_round in range(rounds)) {
        for (ci in range(ncand)) {
            set_q8_batch_chunks_per_job(chunk_cands[ci])
            let t1 = ref_time_ticks()
            for (_rep in range(reps)) {
                matmul_q8q8_batch(y, q, s, xqb, xsb, n, d, ntok)
            }
            let us = double(get_time_usec(t1))
            if (us < bestv[ci]) {
                bestv[ci] = us
            }
        }
    }
    let mode = forked ? "forked x{get_total_hw_jobs()}" : "inline"
    for (ci in range(ncand)) {
        let gmacs = double(n * d * ntok) * double(reps) / (bestv[ci] * 1000.0lf)
        let ctag = ncand > 1 ? " chunks_x{chunk_cands[ci]}" : ""
        print("FOCUS {be} {shape} ntok={ntok} {mode}{ctag}: {gmacs} GMAC/s  ({bestv[ci]} us best-of-{rounds}, {reps} calls)\n")
    }
}

def focus_main {
    let be = get_env_variable("DASLLAMA_FOCUS_BACKEND")
    var shape = "qo"
    if (has_env_variable("DASLLAMA_FOCUS_SHAPE")) {
        shape = get_env_variable("DASLLAMA_FOCUS_SHAPE")
    }
    var n = 2048l
    var d = 2048l
    if (shape == "kv") {
        d = 512l
    } elif (shape == "w13") {
        d = 5632l
    } elif (shape == "w2") {
        n = 5632l
    }
    var ntok = 128l
    if (has_env_variable("DASLLAMA_FOCUS_NTOK")) {
        ntok = int64(to_int(get_env_variable("DASLLAMA_FOCUS_NTOK")))
    }
    if (has_env_variable("DASLLAMA_FOCUS_FORKED")) {
        set_matmul_par_threshold(0l)   // always parallel — every call goes through parallel_for
        with_job_que() {
            setup_dasllama_jobque_()
            if (has_env_variable("DAS_JOBQUE_JOIN_SPIN")) {
                let join_spin = to_int(get_env_variable("DAS_JOBQUE_JOIN_SPIN"))   // poll LEVEL 0..100 (ggml denomination), not µs
                set_jobque_join_spin(join_spin)
                print("join spin override: level {join_spin}\n")
            }
            focus_bench(be, shape, n, d, ntok, true)
        }
    } else {
        set_matmul_par_threshold(1l << 62l)
        focus_bench(be, shape, n, d, ntok, false)
    }
}

[export]
def main {
    if (!jit_enabled()) {
        print("run with -jit — off-JIT the x64 backends do not register and dot32 runs its scalar fallback\n")
        return
    }
    if (has_env_variable("DASLLAMA_FOCUS_BACKEND")) {
        focus_main()
        return
    }
    set_matmul_par_threshold(1l << 62l)   // never parallel: maybe_parallel_for runs the kernel inline, single core
    print("single-core raw kernel throughput (inline, no jobque), ntok={NTOK}, best-of-{ROUNDS} interleaved\n\n")
    bench_shape("kv  2048x512  ", 2048l, 512l, 8)
    bench_shape("qo  2048x2048 ", 2048l, 2048l, 3)
    bench_shape("w13 2048x5632 ", 2048l, 5632l, 1)
    bench_shape("w2  5632x2048 ", 5632l, 2048l, 1)
}
