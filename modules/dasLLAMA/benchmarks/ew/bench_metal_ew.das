options gen2
options persistent_heap

require dastest/testing_boost
require dasllama/dasllama_env  // env_int: the shared typed readers + the knob registry
require metal/msl_shader
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — GPU driver half, Apple builds only
require dasllama/dasllama_metal_llama  // nolint:STYLE030 — the production batch add+rms MSL (zero copy drift)
require dasllama/dasllama_metal_prefill  // the production swiglu MSL (zero copy drift)
require daslib/fio
require math
require strings

// Kernel 4 of the B=2 walkthrough: the elementwise pack (das-kernel lab).
// Slot A (2x per layer): OUR MetalAddRmsB — ONE dispatch fusing the residual add (in place)
// with the next rmsnorm (llama.cpp runs these as two dispatches; the residual add is not
// in its norm+weight fusion window).
// Slot B (1x per layer): our one-thread-per-element swiglu.
// Shapes: dim 3072, hidden 8192, B=2 rows.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/ew/bench_metal_ew.das

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

def private upload_f32(dev; v : float) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

// N back-to-back dispatches per rep with per-dispatch tgmem
def private time_seq(queue; psos; bufss; groupss; threadss; tgmems; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        for (_i in range(reps)) {
            for (di in range(length(psos))) {
                metal_set_pipeline(enc, psos[di])
                for (bi in range(length(bufss[di]))) {
                    metal_set_buffer(enc, bufss[di][bi], 0ul, bi)
                }
                if (tgmems[di] > 0ul) {
                    metal_set_threadgroup_memory_length(enc, tgmems[di], 0)
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

def private best_of(queue; psos; bufss; groupss; threadss; tgmems; rounds : int; est : double) : double {
    var best = 1.0e30lf
    for (_round in range(rounds)) {
        let reps = clamp(int(5.0lf / max(est, 0.002lf)) + 1, 1, 512)
        let ms = time_seq(queue, psos, bufss, groupss, threadss, tgmems, reps)
        if (ms > 0.0lf && ms < best) {
            best = ms
        }
    }
    return best
}

def private run_ew_lab(b) {
    let rounds = env_int("DASMETAL_LAB_ROUNDS", 3)
    let dim = 3072l
    let hidden = 8192l
    let nb = 2l
    let eps = 1.0e-5
    var dev = metal_create_system_default_device()
    if (dev == null) {
        to_log(LOG_WARNING, "skipped: no Metal device\n")
        return
    }
    var queue = metal_new_command_queue(dev)
    var err : string
    var pso_addrms = pipeline_from_source(dev, metal_add_rms_b_msl, metal_add_rms_b_msl_entry, true, err)
    b |> success(pso_addrms != null, "production add+rms kernel compiles")
    var pso_swiglu = pipeline_from_source(dev, metal_swiglu_msl, metal_swiglu_msl_entry, true, err)
    b |> success(pso_swiglu != null, "production swiglu kernel compiles")
    if (pso_addrms == null || pso_swiglu == null) {
        metal_release(queue)
        metal_release(dev)
        return
    }
    var x0 : array<float>
    var aa : array<float>
    var w : array<float>
    x0 |> resize(int(nb * dim))
    aa |> resize(int(nb * dim))
    w |> resize(int(dim))
    for (i in range64(nb * dim)) {
        x0[i] = float(int((i * 13l + i / 61l) % 9l) - 4) * 0.125
        aa[i] = float(int((i * 7l + i / 43l) % 9l) - 4) * 0.125
    }
    for (i in range64(dim)) {
        w[i] = 1.0 + float(int(i % 5l) - 2) * 0.1
    }
    var gg : array<float>
    var uu : array<float>
    gg |> resize(int(nb * hidden))
    uu |> resize(int(nb * hidden))
    for (i in range64(nb * hidden)) {
        gg[i] = float(int((i * 11l + i / 29l) % 9l) - 4) * 0.25
        uu[i] = float(int((i * 5l + i / 71l) % 9l) - 4) * 0.25
    }
    // ours addrms: x (in place), a, w, y
    var xbuf = upload_array(dev, x0, nb * dim * 4l)
    var abuf = upload_array(dev, aa, nb * dim * 4l)
    var wbuf = upload_array(dev, w, dim * 4l)
    var ybuf = metal_new_buffer(dev, uint64(nb * dim * 4l))
    var dimbuf = upload_u32(dev, uint(dim))
    var epsbuf = upload_f32(dev, eps)
    // swiglu: ours in place on g
    var gbuf = upload_array(dev, gg, nb * hidden * 4l)
    var ubuf = upload_array(dev, uu, nb * hidden * 4l)
    var totalbuf = upload_u32(dev, uint(nb * hidden))
    // arm descriptors
    var psos_oa <- [pso_addrms]
    var bufs_oa <- [xbuf, abuf, wbuf, ybuf, dimbuf, epsbuf]
    var bufss_oa <- [bufs_oa]
    var groups_oa <- [uint3(uint(nb), 1u, 1u)]
    var threads_oa <- [1024u]
    var tgm_oa <- [metal_add_rms_b_msl_tgmem]
    var psos_os <- [pso_swiglu]
    var bufs_os <- [gbuf, ubuf, totalbuf]
    var bufss_os <- [bufs_os]
    var groups_os <- [uint3(uint((nb * hidden + 127l) / 128l), 1u, 1u)]
    var threads_os <- [128u]
    var tgm_os <- [0ul]
    // correctness
    let e_oa = time_seq(queue, psos_oa, bufss_oa, groups_oa, threads_oa, tgm_oa, 1)
    let e_os = time_seq(queue, psos_os, bufss_os, groups_os, threads_os, tgm_os, 1)
    var ok_oa = true
    var ok_os = true
    unsafe {
        var px = reinterpret<float?>(metal_buffer_contents(xbuf))
        var py = reinterpret<float?>(metal_buffer_contents(ybuf))
        for (bb in range64(nb)) {
            var ssum = 0.0
            for (k in range64(dim)) {
                let vv = x0[bb * dim + k] + aa[bb * dim + k]
                ssum += vv * vv
            }
            let inv = 1.0 / sqrt(ssum / float(dim) + eps)
            for (k in range64(dim)) {
                let vv = x0[bb * dim + k] + aa[bb * dim + k]
                ok_oa &&= abs(px[bb * dim + k] - vv) < 1.0e-4
                ok_oa &&= abs(py[bb * dim + k] - w[k] * (vv * inv)) < 1.0e-3
            }
        }
        var pg = reinterpret<float?>(metal_buffer_contents(gbuf))
        for (i in range64(nb * hidden)) {
            let sw = (gg[i] / (1.0 + exp(-gg[i]))) * uu[i]
            ok_os &&= abs(pg[i] - sw) < 1.0e-3
        }
    }
    b |> success(ok_oa, "ours add+rms matches the twin")
    b |> success(ok_os, "ours swiglu matches the twin")
    // interleaved best-of
    var b_oa = 1.0e30lf
    var b_os = 1.0e30lf
    for (_round in range(rounds)) {
        b_oa = min(b_oa, best_of(queue, psos_oa, bufss_oa, groups_oa, threads_oa, tgm_oa, 1, e_oa))
        b_os = min(b_os, best_of(queue, psos_os, bufss_os, groups_os, threads_os, tgm_os, 1, e_os))
    }
    to_log(LOG_INFO, "ew B2 addrms: ours(1 disp) {b_oa * 1000.0lf} us\n")
    to_log(LOG_INFO, "ew B2 swiglu: ours {b_os * 1000.0lf} us\n")
    psos_oa |> clear()
    psos_os |> clear()
    bufs_oa |> clear()
    bufs_os |> clear()
    bufss_oa |> clear()
    bufss_os |> clear()
    unsafe {
        delete psos_oa
        delete psos_os
        delete bufs_oa
        delete bufs_os
        delete bufss_oa
        delete bufss_os
    }
    delete groups_oa
    delete threads_oa
    delete tgm_oa
    delete groups_os
    delete threads_os
    delete tgm_os
    metal_release(xbuf)
    metal_release(abuf)
    metal_release(wbuf)
    metal_release(ybuf)
    metal_release(dimbuf)
    metal_release(epsbuf)
    metal_release(gbuf)
    metal_release(ubuf)
    metal_release(totalbuf)
    metal_release(pso_addrms)
    metal_release(pso_swiglu)
    metal_release(queue)
    metal_release(dev)
    delete x0
    delete aa
    delete w
    delete gg
    delete uu
    assert_no_metal_leaks()
}

[benchmark, unused_argument(b)]   // b is consumed only inside the Apple static_if half
def bench_metal_ew(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        run_ew_lab(b)
    }
}
