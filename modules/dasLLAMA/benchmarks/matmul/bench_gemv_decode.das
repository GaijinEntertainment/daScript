options gen2
options persistent_heap

require dastest/testing_boost
require matmul_variants
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_par
require daslib/jobque_boost
require math
require strings

// Decode-GEMV variant race at the real Llama-3.2-1B decode shapes. Decode is pure weight
// streaming (1 token, zero reuse) — llama.cpp's same-shape ggml_gemv_q8_0_4x4_q8_0 sustains
// ~119 GB/s in-situ vs our ~101, and this bench races the matmul_variants decode-GEMV cells
// (unroll / loads-in-flight, fma flush, split sdot chain, dual-group streams, inline-scale
// single-stream layout) to find which delta pays. Method: correctness-gated, then round-robin
// interleaved cells (the only trustworthy comparison on this box), weights rotated across
// enough DRAM-resident copies to defeat the SLC, single-thread AND team-threaded sections.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/matmul/bench_gemv_decode.das

let NV = 12
let ROUNDS = 5

let VNAMES = fixed_array("base", "u2h", "mad", "u2h_mad", "ilp2", "ilp2_mad",
    "u2", "u2_mad", "u4_mad", "2g", "inline", "inline_u2m")

def fill_q8(var a : array<int8>; cnt, phase : int64) {
    a |> reserve(cnt)
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = int8(int((i * 31l + phase) % 251l) - 125)
    }
}

def fill_scales(var a : array<float>; cnt, phase : int64) {
    a |> reserve(cnt)
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = 0.001 + float((i * 7l + phase) % 13l) * 0.0004
    }
}

def blit_i8(src : array<int8>; var dst : array<int8>; at : int64) {
    let cnt = long_length(src)
    unsafe {
        let sp = addr<int8 const?>(src[0])
        var dp = addr(dst[at])
        for [vectorize] (i in range64(cnt)) {
            dp[i] = sp[i]
        }
    }
}

def blit_f32(src : array<float>; var dst : array<float>; at : int64) {
    let cnt = long_length(src)
    unsafe {
        let sp = addr<float const?>(src[0])
        var dp = addr(dst[at])
        for [vectorize] (i in range64(cnt)) {
            dp[i] = sp[i]
        }
    }
}

// One variant over group range [gb, ge) — shared by the single-thread race (full range) and the
// threaded runner (per chunk). Not `private`: called from inside the lifted parallel_for workers.
def gemv_range(vsel, gb, ge : int; var yp : float?; wp : int8 const?; sp : float const?;
               ip : int8 const?; xqp : int8 const?; xsp : float const?; n : int64) {
    let nb = n / 32l
    unsafe {
        if (vsel == 0) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = matmul_variants::dot_q8q8_laneq4(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 1) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4_u2h(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 2) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4_mad(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 3) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4_u2h_mad(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 4) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4_ilp2(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 5) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4_ilp2_mad(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 6) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4_u2(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 7) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4_u2_mad(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 8) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_laneq4_u4_mad(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 9) {
            // dual-group cells: pairs are global (even g), chunk edges fall back to single-group
            var gi = gb
            if (gi % 2 == 1 && gi < ge) {
                let g = int64(gi)
                let f = matmul_variants::dot_q8q8_laneq4(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
                gi++
            }
            while (gi + 1 < ge) {
                let g = int64(gi)
                dot_q8q8_laneq4_2g(yp + g * 4l, wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                gi += 2
            }
            if (gi < ge) {
                let g = int64(gi)
                let f = matmul_variants::dot_q8q8_laneq4(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } elif (vsel == 10) {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_inline4(ip + g * nb * 144l, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        } else {
            for (gi in range(gb, ge)) {
                let g = int64(gi)
                let f = dot_q8q8_inline4_u2_mad(ip + g * nb * 144l, xqp, xsp, n)
                store_f4(yp, g, f)
            }
        }
    }
}

[hint(unsafe_range_check, noalias = yp)]
def store_f4(var yp : float?; g : int64; f : float4) {
    unsafe {
        yp[g * 4l] = f.x
        yp[g * 4l + 1l] = f.y
        yp[g * 4l + 2l] = f.z
        yp[g * 4l + 3l] = f.w
    }
}

// Time `reps` full GEMVs of one variant, rotating the weight copy per call (DRAM-resident
// streaming, no SLC reuse). Returns ns/call.
def race_cell(vsel : int; reps : int; threaded, align : bool; var y : array<float>;
              wgq : array<int8>; wgs : array<float>; inl : array<int8>;
              xq : array<int8>; xs : array<float>; n, d, ncopies : int64) : double {
    let ng = int(d / 4l)
    let nb = n / 32l
    let wstride = d * n
    let sstride = d * nb
    let istride = (d / 4l) * nb * 144l
    // the chunk-alignment axis: production emits workers*chunks (28) onto workers+caller
    // participants (8) -> 3.5 chunks each, last wave runs half-empty. align sizes to participants.
    let base_jobs = align ? get_total_hw_jobs() + 1 : get_total_hw_jobs()
    let njobs = base_jobs * get_q8_batch_chunks_per_job()
    var total : int64
    unsafe {
        var yp = addr(y[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let t0 = ref_time_ticks()
        for (r in range(reps)) {
            let c = int64(r) % ncopies
            let wp = addr<int8 const?>(wgq[c * wstride])
            let sp = addr<float const?>(wgs[c * sstride])
            let ip = addr<int8 const?>(inl[c * istride])
            var myp = yp
            if (threaded) {
                maybe_parallel_for(true, 0, ng, njobs) $(rb, re) {
                    gemv_range(vsel, rb, re, myp, wp, sp, ip, xqp, xsp, n)
                }
            } else {
                gemv_range(vsel, 0, ng, myp, wp, sp, ip, xqp, xsp, n)
            }
        }
        total = ref_time_ticks() - t0
    }
    return double(total) / double(reps)
}

def r1(x : double) : double => double(int(x * 10.0lf)) / 10.0lf

def pad(s : string; w : int) : string {
    let k = w - length(s)
    return k > 0 ? s + repeat(" ", k) : s
}

def bench_shape(name : string; n, d : int64; reps_st, reps_thr : int) {
    let nb = n / 32l
    verify(d % 4l == 0l)

    // synthetic q8 weights + activation (timing is data-independent; gate needs consistency only)
    var q : array<int8>
    var s : array<float>
    fill_q8(q, n * d, 0l)
    fill_scales(s, d * nb, 0l)
    var xq : array<int8>
    var xs : array<float>
    fill_q8(xq, n, 37l)
    fill_scales(xs, nb, 5l)

    // laneq repack (copy 0) + inline layout (copy 0)
    var wgq0 : array<int8>
    var wgs0 : array<float>
    repack_q8q8_4(q, s, n, d, wgq0, wgs0)
    var inl0 : array<int8>
    repack_q8q8_inline(wgq0, wgs0, n, d, inl0)

    // correctness gate: every variant vs the row-major sdot4x4 reference
    var yref : array<float>
    yref |> reserve(d)
    yref |> resize(d)
    matmul_q8q8_sdot4x4(yref, q, s, xq, xs, n, d)
    var mxref = 0.0
    for (i in range64(d)) {
        mxref = max(mxref, abs(yref[i]))
    }
    var yv : array<float>
    yv |> reserve(d)
    yv |> resize(d)
    for (v in range(NV)) {
        unsafe {
            var yp = addr(yv[0])
            let wp = addr<int8 const?>(wgq0[0])
            let sp = addr<float const?>(wgs0[0])
            let ip = addr<int8 const?>(inl0[0])
            let xqp = addr<int8 const?>(xq[0])
            let xsp = addr<float const?>(xs[0])
            gemv_range(v, 0, int(d / 4l), yp, wp, sp, ip, xqp, xsp, n)
        }
        var mx = 0.0
        for (i in range64(d)) {
            mx = max(mx, abs(yv[i] - yref[i]))
        }
        if (mx > 1.0e-4 * mxref + 1.0e-6) {
            to_log(LOG_ERROR, "{name} variant {VNAMES[v]} FAILS gate: max|diff| = {mx} (maxref {mxref})\n")
            verify(false)
        }
    }
    delete q
    delete s

    // replicate weight copies until the rotating set is DRAM-resident (>~128MB or 1 copy if huge)
    let wbytes = n * d + d * nb * 4l
    let ibytes = (d / 4l) * nb * 144l
    let ncopies = max(1l, 128l * 1024l * 1024l / wbytes + 1l)
    var wgq : array<int8>
    var wgs : array<float>
    var inl : array<int8>
    wgq |> reserve(ncopies * n * d)
    wgq |> resize(ncopies * n * d)
    wgs |> reserve(ncopies * d * nb)
    wgs |> resize(ncopies * d * nb)
    inl |> reserve(ncopies * ibytes)
    inl |> resize(ncopies * ibytes)
    for (c in range64(ncopies)) {
        blit_i8(wgq0, wgq, c * n * d)
        blit_f32(wgs0, wgs, c * d * nb)
        blit_i8(inl0, inl, c * ibytes)
    }
    delete wgq0
    delete wgs0
    delete inl0

    let wmb = r1(double(wbytes) / (1024.0lf * 1024.0lf))
    let imb = r1(double(ibytes) / (1024.0lf * 1024.0lf))
    to_log(LOG_INFO, "\n=== {name}  n={n} d={d}  laneq {wmb} MB / inline {imb} MB  x{ncopies} copies ===\n")

    // interleaved race: ROUNDS passes, every variant timed once per pass, ST then THR sections
    var st_sum : array<double>
    var thr_sum : array<double>
    var thr_min : array<double>
    st_sum |> resize(NV)
    thr_sum |> resize(NV)
    thr_min |> resize(NV)
    var thr_sum_al : array<double>
    var thr_min_al : array<double>
    thr_sum_al |> resize(NV)
    thr_min_al |> resize(NV)
    for (_round in range(ROUNDS)) {
        for (v in range(NV)) {
            st_sum[v] += race_cell(v, reps_st, false, false, yv, wgq, wgs, inl, xq, xs, n, d, ncopies)
        }
    }
    for (_round in range(ROUNDS)) {
        for (v in range(NV)) {
            let ns = race_cell(v, reps_thr, true, false, yv, wgq, wgs, inl, xq, xs, n, d, ncopies)
            thr_sum[v] += ns
            if (thr_min[v] == 0.0lf || ns < thr_min[v]) {
                thr_min[v] = ns
            }
            let ns2 = race_cell(v, reps_thr, true, true, yv, wgq, wgs, inl, xq, xs, n, d, ncopies)
            thr_sum_al[v] += ns2
            if (thr_min_al[v] == 0.0lf || ns2 < thr_min_al[v]) {
                thr_min_al[v] = ns2
            }
        }
    }

    to_log(LOG_INFO, "variant      st us     st GB/s   t28 us    t28 GB/s  t32 us    t32 GB/s  t28min    t32min\n")
    for (v in range(NV)) {
        let bytes = v >= 10 ? ibytes : wbytes
        let st_ns = st_sum[v] / double(ROUNDS)
        let thr_ns = thr_sum[v] / double(ROUNDS)
        let thr_ns_al = thr_sum_al[v] / double(ROUNDS)
        let st_us = r1(st_ns / 1000.0lf)
        let thr_us = r1(thr_ns / 1000.0lf)
        let thr_us_al = r1(thr_ns_al / 1000.0lf)
        let st_gbs = r1(double(bytes) / st_ns)
        let thr_gbs = r1(double(bytes) / thr_ns)
        let thr_gbs_al = r1(double(bytes) / thr_ns_al)
        let tmin = r1(thr_min[v] / 1000.0lf)
        let tmin_al = r1(thr_min_al[v] / 1000.0lf)
        let c0 = pad(VNAMES[v], 12)
        let c1 = pad("{st_us}", 9)
        let c2 = pad("{st_gbs}", 9)
        let c3 = pad("{thr_us}", 9)
        let c4 = pad("{thr_gbs}", 9)
        let c5 = pad("{thr_us_al}", 9)
        let c6 = pad("{thr_gbs_al}", 9)
        let c7 = pad("{tmin}", 9)
        to_log(LOG_INFO, "{c0} {c1} {c2} {c3} {c4} {c5} {c6} {c7} {tmin_al}\n")
    }

    delete wgq
    delete wgs
    delete inl
    delete xq
    delete xs
    delete yref
    delete yv
    delete st_sum
    delete thr_sum
    delete thr_min
    delete thr_sum_al
    delete thr_min_al
}

[benchmark, unused_argument(b)]
def bench_gemv_decode(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        set_jobque_team_mode(true)
        to_log(LOG_INFO, "threads={get_total_hw_jobs()} chunks/job={get_q8_batch_chunks_per_job()} team=on rounds={ROUNDS}\n")
        // Llama-3.2-1B decode shapes: dim=2048, hidden=8192, vocab=128256
        bench_shape("wo  ", 2048l, 2048l, 12, 48)
        bench_shape("ffn_gate", 2048l, 8192l, 6, 20)
        bench_shape("ffn_down", 8192l, 2048l, 6, 20)
        bench_shape("cls ", 2048l, 128256l, 2, 4)
        set_jobque_team_mode(false)
    }
}
