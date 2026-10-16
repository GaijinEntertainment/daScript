options gen2
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require metal/msl_shader
require ?das_metal metal/das_metal_boost  // GPU driver half, Apple builds only
require dasllama/dasllama_metal_decode  // the production batch add+rms MSL (zero copy drift)
require dasllama/dasllama_metal_kernels  // the production swiglu MSL (zero copy drift)
require math
require strings

// Kernel 4 of the B=2 walkthrough: the elementwise pack (das-kernel lab).
// Slot A (2x per layer): OUR MetalAddRmsB — ONE dispatch fusing the residual add (in place)
// with the next rmsnorm (the upstream reference runs these as two dispatches; the residual
// add is not in its norm+weight fusion window).
// Slot B (1x per layer): our one-thread-per-element swiglu.
// Shapes: dim 3072, hidden 8192, B=2 rows.
// Each slot runs one arm, its baseline the CPU reference in float off the same inputs: the
// add+rms arm's compare region is x (in place, bound 1e-4) and y (bound 1e-3) over the B rows,
// the swiglu arm's g over the B rows (bound 1e-3). A slot past its bound prints the miss and no
// time; a slot whose timed rep fails to dispatch prints REFUSED and no time.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/ew/bench_metal_ew.das

def private upload_array(dev; arr; bytes : int64) {
    var buf = metal_new_buffer(dev, uint64(bytes))
    unsafe {
        memcpy(metal_buffer_contents(buf), addr < void? >(arr[0]), bytes)
    }
    return buf
}

def private upload_u32(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
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

//! the best per-dispatch ms over `rounds` timed reps, or -1 the moment a rep fails to dispatch
def private best_of(queue; psos; bufss; groupss; threadss; tgmems; rounds : int; est : double) : double {
    var best = 1.0e30lf
    for (_round in range(rounds)) {
        let reps = clamp(int(5.0lf / max(est, 0.002lf)) + 1, 1, 512)
        let ms = time_seq(queue, psos, bufss, groupss, threadss, tgmems, reps)
        if (ms <= 0.0lf) {
            return -1.0lf
        }
        best = min(best, ms)
    }
    return best
}

//! a slot's report line: the miss and its bound, REFUSED where a rep failed to dispatch, else the time
def private slot_row(name : string; ok : bool; bound : string; ms : double) {
    if (!ok) {
        to_log(LOG_INFO, "{name}: MISS - past the bound {bound} vs the CPU reference\n")
    } elif (ms < 0.0lf) {
        to_log(LOG_INFO, "{name}: REFUSED - a timed rep failed to dispatch (the error above)\n")
    } else {
        to_log(LOG_INFO, "{name}: {ms * 1000.0lf} us - within {bound} vs the CPU reference\n")
    }
}

def private run_ew_lab(b) {   // nolint:STYLE038 — flat lab driver: buffers bound once, then one timed slot per kernel
    let rounds = g_env_bench.lab_rounds
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
    var pso_addrms = pipeline_from_source(dev, metal_add_rms_msl, metal_add_rms_msl_entry, true, err)
    b |> success(pso_addrms != null, "production add+rms kernel compiles")
    var pso_swiglu = pipeline_from_source(dev, MetalSwiglu_metal_ew2_msl, MetalSwiglu_metal_ew2_msl_entry, true, err)
    b |> success(pso_swiglu != null, "production swiglu kernel compiles")
    if (pso_addrms == null || pso_swiglu == null) {
        metal_release(queue)
        metal_release(dev)
        return
    }
    var x0, aa, w : array<float>
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
    var gg, uu : array<float>
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
    var argbuf = metal_new_buffer(dev, uint64(typeinfo sizeof(type<GkAddRmsArgs>)))
    unsafe {
        *reinterpret<GkAddRmsArgs?>(metal_buffer_contents(argbuf)) = GkAddRmsArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = 1.0)
    }
    // swiglu: ours in place on g
    var gbuf = upload_array(dev, gg, nb * hidden * 4l)
    var ubuf = upload_array(dev, uu, nb * hidden * 4l)
    var totalbuf = upload_u32(dev, uint(nb * hidden))
    // arm descriptors
    var psos_oa <- [pso_addrms]
    var bufs_oa <- [xbuf, abuf, wbuf, ybuf, argbuf]   //! the lab binds by slot (its one timing loop over raw pipelines): the stamp's args ride the binding past the class's highest bound buffer, 4
    var bufss_oa <- [bufs_oa]
    var groups_oa <- [uint3(uint(nb), 1u, 1u)]
    var threads_oa <- [1024u]
    var tgm_oa <- [metal_add_rms_msl_tgmem]
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
    for (_round in range(max(rounds, 1))) {
        let t_oa = best_of(queue, psos_oa, bufss_oa, groups_oa, threads_oa, tgm_oa, 1, e_oa)
        let t_os = best_of(queue, psos_os, bufss_os, groups_os, threads_os, tgm_os, 1, e_os)
        b_oa = b_oa < 0.0lf || t_oa < 0.0lf ? -1.0lf : min(b_oa, t_oa)
        b_os = b_os < 0.0lf || t_os < 0.0lf ? -1.0lf : min(b_os, t_os)
    }
    b |> success(b_oa > 0.0lf && b_os > 0.0lf, "every timed rep dispatched")
    slot_row("ew B2 addrms: ours(1 disp)", ok_oa, "1e-4 x, 1e-3 y", b_oa)
    slot_row("ew B2 swiglu: ours", ok_os, "1e-3 g", b_os)
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
    metal_release(argbuf)
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
