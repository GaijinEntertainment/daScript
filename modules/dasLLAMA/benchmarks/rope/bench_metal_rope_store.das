options gen2
options persistent_heap

require dastest/testing_boost
require dasllama/dasllama_env  // env_int: the shared typed readers + the knob registry
require metal/msl_shader
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — GPU driver half, Apple builds only
require dasllama/dasllama_metal_llama  // nolint:STYLE030 — the production rope+KV-store MSL (zero copy drift)
require daslib/fio
require math
require strings

// Kernel 3 of the B=2 walkthrough: rope + KV-store. OURS is ONE fused dispatch per layer
// (MetalRopeStoreBF16: q ropes in place in the fused QKV row, k ropes with the f16 clamp
// straight into the mirror, v copies alongside; cos/sin PRECOMPUTED per row). LLAMA.CPP runs
// FOUR dispatches (kernel_rope_neox on Q, on K — theta via pow() per pair — then
// kernel_cpy_f32_f16 for K and V into the cache). Same logical work, wildly different
// dispatch/transcendental budgets; per-layer cost is launch-overhead territory, which is
// exactly the point: the step runs 28 of these.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/rope/bench_metal_rope_store.das

def private upload_array(dev; arr; bytes : int64) {
    var buf = metal_new_buffer(dev, uint64(bytes))
    unsafe {
        memcpy(metal_buffer_contents(buf), addr < void? >(arr[0]), int(bytes))
    }
    return buf
}

def private upload_u32(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

// N back-to-back dispatches per rep, per-dispatch state re-set
def private time_seq(queue; psos; bufss; groupss; threadss; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        for (_i in range(reps)) {
            for (di in range(length(psos))) {
                metal_set_pipeline(enc, psos[di])
                for (bi in range(length(bufss[di]))) {
                    metal_set_buffer(enc, bufss[di][bi], 0ul, bi)
                }
                metal_dispatch_threadgroups(enc, groupss[di], uint3(threadss[di], 1u, 1u))
            }
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "seq dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms / double(reps)
}

def private run_rope_sidebyside(b) {
    let rounds = env_int("DASMETAL_LAB_ROUNDS", 3)
    let nheads = 24l
    let hs = 128l
    let kv_dim = 1024l
    let qd = nheads * hs
    let qkvd = qd + 2l * kv_dim
    let nb = 2l
    let cap = 2048l
    let pos = 511l
    let freq_base = 500000.0
    var dev = metal_create_system_default_device()
    if (dev == null) {
        to_log(LOG_WARNING, "skipped: no Metal device\n")
        return
    }
    var queue = metal_new_command_queue(dev)
    var err : string
    var pso_ours = pipeline_from_source(dev, metal_rope_store_b16_msl, metal_rope_store_b16_msl_entry, true, err)
    b |> success(pso_ours != null, "production rope-store kernel compiles")
    if (pso_ours == null) {
        metal_release(queue)
        metal_release(dev)
        return
    }
    // the fused QKV rows (ours ropes in place)
    var qk : array<float>
    qk |> resize(int(nb * qkvd))
    for (i in range64(nb * qkvd)) {
        qk[i] = float(int((i * 13l + i / 61l) % 9l) - 4) * 0.25
    }
    // per-row rope tables (adjacent-pair convention), positions pos and pos+1
    let half_hs = hs / 2l
    var tcos : array<float>
    var tsin : array<float>
    tcos |> resize(int(nb * half_hs))
    tsin |> resize(int(nb * half_hs))
    for (bb in range64(nb)) {
        for (j in range64(half_hs)) {
            let f = pow(freq_base, -2.0 * float(j) / float(hs))
            tcos[bb * half_hs + j] = cos(float(pos + bb) * f)
            tsin[bb * half_hs + j] = sin(float(pos + bb) * f)
        }
    }
    var rte : array<uint>
    rte <- [0u, 0u, uint(cap), uint(pos + 1l), uint(cap * 4l), uint(cap * 4l), uint(cap), uint(pos + 2l)]
    // row 1 gets its own arena region (element offsets cap*kv_dim x4 fits one arena of 8*cap*kv)
    var qkbuf = upload_array(dev, qk, nb * qkvd * 4l)
    var tcosbuf = upload_array(dev, tcos, nb * half_hs * 4l)
    var tsinbuf = upload_array(dev, tsin, nb * half_hs * 4l)
    var rtbuf = upload_array(dev, rte, nb * 16l)
    var karena = metal_new_buffer(dev, uint64(8l * cap * kv_dim * 2l))
    var varena = metal_new_buffer(dev, uint64(8l * cap * kv_dim * 2l))
    var laybuf = upload_u32(dev, 0u)
    var kvdimbuf = upload_u32(dev, uint(kv_dim))
    var qdbuf = upload_u32(dev, uint(qd))
    var hsbuf = upload_u32(dev, uint(hs))
    let npairs_q = qd / 2l
    let npairs = (qd + kv_dim) / 2l
    var npqbuf = upload_u32(dev, uint(npairs_q))
    var npbuf = upload_u32(dev, uint(npairs))
    var qstridebuf = upload_u32(dev, uint(qkvd))
    // arm descriptors
    var bufs_ours <- [qkbuf, karena, varena, tcosbuf, tsinbuf, rtbuf, laybuf, kvdimbuf, qdbuf, hsbuf, npqbuf, npbuf, qstridebuf]
    var psos_ours <- [pso_ours]
    var bufss_ours <- [bufs_ours]
    var groups_ours <- [uint3(uint((npairs + 63l) / 64l), uint(nb), 1u)]
    var threads_ours <- [64u]
    // correctness: ours vs the adjacent-pair CPU twin (q in place + mirror rows)
    let ms0_ours = time_seq(queue, psos_ours, bufss_ours, groups_ours, threads_ours, 1)
    var okq = true
    var okk = true
    var okv = true
    unsafe {
        var pq = reinterpret<float?>(metal_buffer_contents(qkbuf))
        var pk = reinterpret<float16?>(metal_buffer_contents(karena))
        var pv = reinterpret<float16?>(metal_buffer_contents(varena))
        for (bb in range64(nb)) {
            let qbase = bb * qkvd
            let kdst = int64(rte[int(bb * 4l)]) + (pos + 1l + bb - 1l + 0l) * kv_dim   // (cnt-1)*kv_dim + base
            for (lgid in range64(npairs)) {
                let isK = lgid >= npairs_q
                let lg = isK ? lgid - npairs_q : lgid
                let j = lg % half_hs
                let i = (lg / half_hs) * hs + 2l * j
                let fcr = tcos[bb * half_hs + j]
                let fci = tsin[bb * half_hs + j]
                if (isK) {
                    let v0 = qk[qbase + qd + i]
                    let v1 = qk[qbase + qd + i + 1l]
                    okk &&= abs(float(pk[kdst + i]) - (v0 * fcr - v1 * fci)) < 2.0e-3
                    okk &&= abs(float(pk[kdst + i + 1l]) - (v0 * fci + v1 * fcr)) < 2.0e-3
                    okv &&= abs(float(pv[kdst + i]) - qk[qbase + qd + kv_dim + i]) < 2.0e-3
                } else {
                    let v0 = qk[qbase + i]
                    let v1 = qk[qbase + i + 1l]
                    okq &&= abs(pq[qbase + i] - (v0 * fcr - v1 * fci)) < 2.0e-3
                    okq &&= abs(pq[qbase + i + 1l] - (v0 * fci + v1 * fcr)) < 2.0e-3
                }
            }
        }
    }
    b |> success(okq, "ours: q ropes in place")
    b |> success(okk, "ours: k ropes into the mirror")
    b |> success(okv, "ours: v copies into the mirror")
    // best-of timing
    var best_ours = 1.0e30lf
    for (_round in range(rounds)) {
        let reps_o = clamp(int(5.0lf / max(ms0_ours, 0.002lf)) + 1, 1, 512)
        let mo = time_seq(queue, psos_ours, bufss_ours, groups_ours, threads_ours, reps_o)
        if (mo > 0.0lf && mo < best_ours) {
            best_ours = mo
        }
    }
    to_log(LOG_INFO, "rope_store B2: ours(1 dispatch) {best_ours * 1000.0lf} us\n")
    bufs_ours |> clear()
    psos_ours |> clear()
    bufss_ours |> clear()
    unsafe {
        delete bufs_ours
        delete psos_ours
        delete bufss_ours
    }
    delete groups_ours
    delete threads_ours
    metal_release(qkbuf)
    metal_release(tcosbuf)
    metal_release(tsinbuf)
    metal_release(rtbuf)
    metal_release(karena)
    metal_release(varena)
    metal_release(laybuf)
    metal_release(kvdimbuf)
    metal_release(qdbuf)
    metal_release(hsbuf)
    metal_release(npqbuf)
    metal_release(npbuf)
    metal_release(qstridebuf)
    metal_release(pso_ours)
    metal_release(queue)
    metal_release(dev)
    delete qk
    delete tcos
    delete tsin
    delete rte
    assert_no_metal_leaks()
}

[benchmark, unused_argument(b)]   // b is consumed only inside the Apple static_if half
def bench_metal_rope_store(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        run_rope_sidebyside(b)
    }
}
