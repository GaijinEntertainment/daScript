options gen2
options persistent_heap
options _jit_fast_math = true   // match the end-to-end config AND ggml's -O3 FP laxity (kernel-to-kernel fairness)

// RAW single-core kernel throughput — no threading, no pipeline. The production
// matmul_q8q8_batch runs INLINE on the calling thread: target_chunk_work is pushed to infinity,
// so maybe_parallel_for compiles to a plain invoke; the que merely exists, idle (the dispatch
// sites' chunk-count args read get_dispatch_lanes() eagerly, que-less boxes throw). This is
// the kernel-to-kernel denominator against llama.cpp's Q8_0 MUL_MAT via `test-backend-ops perf`
// (harness/pure_gemm_ladder.sh + backend_ops_shapes.patch = the shape-matched, thread-pinned
// ladder). Interleaved best-of across backends per shape, tolerance-gated vs portable.
// Run: daslang -jit gemm_1core_probe.das

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_math_gen
require dasllama/dasllama_convert
require daslib/jobque_boost
require math
require strings
require daslib/strings_boost
require dasllama/dasllama_env

let NTOK = 128l
let ROUNDS = 6

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = float(i % 13l) * 0.1 - 0.6
    }
}

def maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (av, bv in a, b) {
        m = max(m, abs(av - bv))
    }
    return m
}

struct Cand {
    name : string
    repacked : bool
}

def bench_shape(name : string; n, d : int64; reps : int) {   // nolint:STYLE037,STYLE038 — one rig: fixtures, the correctness gate and the interleaved timing rounds share every buffer
    let nb = n / 32l
    var w : array<float>
    fillf(w, n * d)
    let qt <- quantize_q8_0(w, n * d)
    var q : array<int8>
    var s : array<float>
    q |> resize(n * d)
    s |> resize(d * nb)
    for (i in range64(n * d)) {
        q[i] = qt.q[i]
    }
    for (i in range64(d * nb)) {
        s[i] = qt.scales[i]
    }
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
        for (i in range64(n)) {
            xqb[int(tk * n + i)] = xq[int(i)]
        }
        for (i in range64(nb)) {
            xsb[int(tk * nb + i)] = xs[int(i)]
        }
    }

    // every backend registered on THIS host (x64 and arm64 tiers register platform-gated),
    // portable first (it is the correctness reference below)
    var names <- kernel_backend_names()
    names |> sort()
    var cands : array<Cand>
    cands |> push(Cand(name = "portable", repacked = false))
    for (nm in names) {
        if (nm != "portable") {
            cands |> push(Cand(name = nm, repacked = kernel_backend_needs_repack(nm)))
        }
    }
    let nc = length(cands)

    // per-backend repacked weight copies (repack layouts differ across backends; repack is in place
    // under the SELECTED backend's repack fn)
    var qr : array<array<int8>>
    var sr : array<array<float>>
    qr |> resize(nc)
    sr |> resize(nc)
    for (ci in range(nc)) {
        if (cands[ci].repacked) {
            qr[ci] := q
            sr[ci] := s
            select_kernel_backend(cands[ci].name)
            repack_q8q8_weight(qr[ci], sr[ci], 0l, n, d)
        }
    }

    var y : array<float>
    var yref : array<float>
    y |> resize(NTOK * d)
    yref |> resize(NTOK * d)

    // correctness gate: every backend within 1e-3 of portable on the same data
    select_kernel_backend("portable")
    matmul_q8q8_batch(yref, q, s, xqb, xsb, n, d, NTOK)
    var ok : array<bool>
    ok |> resize(nc)
    for (ci in range(nc)) {
        select_kernel_backend(cands[ci].name)
        if (cands[ci].repacked) {
            matmul_q8q8_batch(y, qr[ci], sr[ci], xqb, xsb, n, d, NTOK)
        } else {
            matmul_q8q8_batch(y, q, s, xqb, xsb, n, d, NTOK)
        }
        let md = maxdiff(yref, y)
        ok[ci] = md < 1.0e-3
        if (!ok[ci]) {
            print("  !! {cands[ci].name} maxdiff {md} vs portable — REJECTED\n")
        }
    }

    let gemv_reps = reps * int(NTOK) / 4
    var best : array<double>
    var bestv : array<double>
    best |> resize(nc)
    bestv |> resize(nc)
    for (ci in range(nc)) {
        best[ci] = 1.0e30lf
        bestv[ci] = 1.0e30lf
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
                    matmul_q8q8_batch(y, qr[ci], sr[ci], xqb, xsb, n, d, NTOK)
                } else {
                    matmul_q8q8_batch(y, q, s, xqb, xsb, n, d, NTOK)
                }
            }
            let us = double(get_time_usec(t0))
            if (us < best[ci]) {
                best[ci] = us
            }
            let t1 = ref_time_ticks()
            for (_rep in range(gemv_reps)) {
                if (cands[ci].repacked) {
                    matmul_q8q8(y, qr[ci], sr[ci], xq, xs, n, d)
                } else {
                    matmul_q8q8(y, q, s, xq, xs, n, d)
                }
            }
            let usv = double(get_time_usec(t1))
            if (usv < bestv[ci]) {
                bestv[ci] = usv
            }
        }
    }

    print("--- {name}  (n={n} d={d} ntok={NTOK}, x{reps}) ---\n")
    let macs = double(n * d * NTOK) * double(reps)
    let macsv = double(n * d) * double(gemv_reps)
    for (ci in range(nc)) {
        if (!ok[ci]) {
            continue
        }
        let gmacs = macs / (best[ci] * 1000.0lf)
        let gmacsv = macsv / (bestv[ci] * 1000.0lf)
        print("  {cands[ci].name}: batch {gmacs} GMAC/s  ({best[ci]} us best-of-{ROUNDS})   gemv {gmacsv} GMAC/s  ({bestv[ci]} us, x{gemv_reps})\n")
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

def focus_bench(be : string; shape : string; n, d, ntok : int64; forked : bool) {   // nolint:STYLE037,STYLE038 — one calibrated window: splitting it separates the calibration from the loop it sizes
    let nb = n / 32l
    var w : array<float>
    fillf(w, n * d)
    let qt <- quantize_q8_0(w, n * d)
    var q : array<int8>
    var s : array<float>
    q |> resize(n * d)
    s |> resize(d * nb)
    for (i in range64(n * d)) {
        q[i] = qt.q[i]
    }
    for (i in range64(d * nb)) {
        s[i] = qt.scales[i]
    }
    select_kernel_backend(be)
    if (kernel_backend_needs_repack(be)) {
        repack_q8q8_weight(q, s, 0l, n, d)
    }
    var x : array<float>
    fillf(x, n)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(nb)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    var xqb : array<int8>
    var xsb : array<float>
    xqb |> resize(ntok * n)
    xsb |> resize(ntok * nb)
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xqb[int(tk * n + i)] = xq[int(i)]
        }
        for (i in range64(nb)) {
            xsb[int(tk * nb + i)] = xs[int(i)]
        }
    }
    var y : array<float>
    y |> resize(ntok * d)

    // DASLLAMA_BATCH_CHUNKS="1,4" A/Bs the batch-oversplit knob INTERLEAVED within this process
    // (best-of-6 per value, adjacent in time — immune to box drift); absent = current knob, one shot
    var chunk_cands : array<int>
    if (!empty(g_env_harness.batch_chunks)) {
        for (part in split(g_env_harness.batch_chunks, ",")) {
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
    let be = g_env_harness.focus_backend
    var shape = "qo"
    if (!empty(g_env_harness.focus_shape)) {
        shape = g_env_harness.focus_shape
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
    ntok = g_env_harness.focus_ntok ?? ntok
    if (g_env_harness.focus_forked) {
        set_target_chunk_work(1l)   // always parallel — every call goes through parallel_for
        with_job_que() {
            setup_dasllama_jobque_()
            if (is_some(g_env_harness.das_jobque_join_spin)) {
                let join_spin = unwrap(g_env_harness.das_jobque_join_spin)   // poll LEVEL 0..100 (ggml denomination), not µs
                set_jobque_join_spin(join_spin)
                print("join spin override: level {join_spin}\n")
            }
            focus_bench(be, shape, n, d, ntok, true)
        }
    } else {
        set_target_chunk_work(1l << 62l)
        // the que must still EXIST (see main) — nothing dispatches at this target
        with_job_que() {
            focus_bench(be, shape, n, d, ntok, false)
        }
    }
}

[export]
def main {
    if (!jit_enabled()) {
        print("run with -jit — off-JIT the x64 backends do not register and dot32 runs its scalar fallback\n")
        return
    }
    if (!empty(g_env_harness.focus_backend)) {
        focus_main()
        return
    }
    set_target_chunk_work(1l << 62l)   // never parallel: maybe_parallel_for runs the kernel inline, single core
    // a que must still EXIST: the chunk shapers read get_dispatch_lanes() eagerly even when
    // maybe_parallel_for stays inline — production always has a que.
    // Workers sit idle (nothing dispatches at this target), so the measurement stays 1-core.
    with_job_que() {
        print("single-core raw kernel throughput (inline, idle jobque), ntok={NTOK}, best-of-{ROUNDS} interleaved\n\n")
        bench_shape("kv  2048x512  ", 2048l, 512l, 8)
        bench_shape("qo  2048x2048 ", 2048l, 2048l, 3)
        bench_shape("w13 2048x5632 ", 2048l, 5632l, 1)
        bench_shape("w2  5632x2048 ", 5632l, 2048l, 1)
    }
}
