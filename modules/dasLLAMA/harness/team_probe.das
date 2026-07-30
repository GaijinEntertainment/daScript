options gen2
options persistent_heap = true

// team_probe — the dispatch-overhaul constants probe (gemm_generator_plan.md, dispatch arc).
// Three measurements, all under -jit, sweeping T EXTERNALLY via DAS_JOBQUE_THREADS:
//   1. overhead(T): empty-body team_parallel_for at a chunk ladder {L, 2L, 4L, 8L} —
//      team-prof phase averages (publish / caller-serve / join-tail) + wall ns/dispatch.
//   2. invoke ns/chunk: wall-difference between the 8L and L ladder points divides out the
//      per-dispatch fixed cost; the quotient is the per-chunk claim+invoke cost. This is the
//      JIT-fastpath A/B rail: run the SAME probe on pre/post binaries, the delta is the evidence.
//   3. streaming GEMV GB/s vs lanes: the active backend rows core over real-shape Q8 copies
//      (rotated to defeat LLC). The flattening knee = gemv_lane_cap for this box.
// Ends with a paste-ready box_profile.json "runtime" fragment.

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_convert
require daslib/jobque_boost
require math

let ROUNDS_EMPTY = 2000     // empty dispatches per ladder point (per rep)
let REPS = 5                // best-of reps per measurement
let GEMV_N = 4096l          // cols (one row = 4096 B q + 512 B scales)
let GEMV_D = 4096l          // rows
let GEMV_COPIES = 8         // 8 x ~18.7 MB > any LLC here
let GEMV_TARGET_MS = 60.0lf // per lane point, per rep

var private g_per_dispatch_ns = 0.0lf   // K=L ladder point, filled by empty_ladder

def best_of_us(reps : int; blk : block<() : void>) : double {
    var best = 1.0e18lf
    for (_r in range(reps)) {
        let t0 = ref_time_ticks()
        invoke(blk)
        best = min(best, double(get_time_usec(t0)))
    }
    return best
}

def empty_ladder(lanes : int) : double {
    // returns per-chunk ns (the slope between the L and 8L points); per-dispatch ns goes
    // to g_per_dispatch_ns
    let ladder = fixed_array(lanes, 2 * lanes, 4 * lanes, 8 * lanes)
    var wall_us : float[4]
    var idx = 0
    for (k in ladder) {
        set_jobque_team_prof(true)
        reset_jobque_team_prof()
        let us = best_of_us(REPS) $ {
            for (_r in range(ROUNDS_EMPTY)) {
                team_parallel_for(0, k, k) <| @(rb, re : int) {}
            }
        }
        wall_us[idx] = float(us)
        let prof = get_jobque_team_prof()
        let counts = get_jobque_team_prof_counts()
        let react = get_jobque_team_prof_react()
        set_jobque_team_prof(false)
        let perDispatchNs = us * 1000.0lf / double(ROUNDS_EMPTY)
        let callerShare = counts.y > 0 ? 100 * counts.z / counts.y : 0
        let soloShare = counts.x > 0 ? 100 * counts.w / counts.x : 0
        print("empty K={k}: {perDispatchNs} ns/dispatch | prof avg ns/op: publish {prof.x} serve {prof.y} join-tail {prof.z} total {prof.w} | ops {counts.x} caller-share {callerShare}% solo {soloShare}% | claims first +{react.x}ns last +{react.y}ns\n")
        idx++
    }
    g_per_dispatch_ns = double(wall_us[0]) * 1000.0lf / double(ROUNDS_EMPTY)
    let dChunks = double(ladder[3] - ladder[0]) * double(ROUNDS_EMPTY)
    return (double(wall_us[3]) - double(wall_us[0])) * 1000.0lf / dChunks
}

struct GemvFixture {
    q : array<int8>         // GEMV_COPIES weight planes, active-backend layout
    s : array<float>
    xq : array<int8>
    xs : array<float>
    y : array<float>
}

def build_gemv_fixture() : GemvFixture {
    var fx = GemvFixture()
    let planeQ = GEMV_N * GEMV_D
    let planeS = planeQ / 32l
    var w : array<float>
    w |> resize(int(planeQ))
    for (i in range64(planeQ)) {
        w[i] = float(i % 17l) * 0.13 - 1.1
    }
    var qt <- quantize_q8_0(w, planeQ)
    delete w
    fx.q |> resize(int(planeQ) * GEMV_COPIES)
    fx.s |> resize(int(planeS) * GEMV_COPIES)
    for (c in range(GEMV_COPIES)) {
        for (i in range64(planeQ)) {
            fx.q[int64(c) * planeQ + i] = qt.q[i]
        }
        for (i in range64(planeS)) {
            fx.s[int64(c) * planeS + i] = qt.scales[i]
        }
        // repack THIS copy into the active backend's layout (no-op on row-major backends)
        repack_q8q8_weight(fx.q, fx.s, int64(c) * planeQ, GEMV_N, GEMV_D)
    }
    delete qt
    var x : array<float>
    x |> resize(int(GEMV_N))
    for (i in range64(GEMV_N)) {
        x[i] = float(i % 23l) * 0.07 - 0.8
    }
    var xt <- quantize_q8_0(x, GEMV_N)
    delete x
    fx.xq := xt.q
    fx.xs := xt.scales
    delete xt
    fx.y |> resize(int(GEMV_D))
    return <- fx
}

def gemv_ladder(lanes : int) : int {
    // returns the measured bandwidth knee (last lane count that improved GB/s by >10%)
    if (!kernel_backend_has_rows()) {
        print("gemv ladder SKIPPED: active backend '{active_kernel_backend()}' has no rows core\n")
        return 0
    }
    var fx <- build_gemv_fixture()
    let core = mm_q8q8_rows_fn()
    let planeQ = GEMV_N * GEMV_D
    let planeS = planeQ / 32l
    let bytesPerPass = planeQ + planeS * 4l                     // weight bytes streamed per full matrix
    var lastGBs = 0.0lf
    var knee = 0
    var lp = 1
    while (lp <= lanes) {
        let lanesNow = lp
        let estPassUs = double(bytesPerPass) / 100.0lf / 1000.0lf   // size reps assuming ~100 GB/s
        let reps = max(int(GEMV_TARGET_MS * 1000.0lf / max(estPassUs, 1.0lf)), 8)
        unsafe {
            let xqp = addr(fx.xq[0])
            let xsp = addr(fx.xs[0])
            var yp = addr(fx.y[0])
            let us = best_of_us(REPS) $ {
                for (r in range(reps)) {
                    let copy = int64((r * lanesNow) % GEMV_COPIES)
                    let wq = addr(fx.q[0]) + copy * planeQ
                    let ws = addr(fx.s[0]) + copy * planeS
                    team_parallel_for(0, lanesNow, lanesNow) <| @(cb, ce : int) {
                        for (c in range(cb, ce)) {
                            let rb = int64(c) * GEMV_D / int64(lanesNow)
                            let re = int64(c + 1) * GEMV_D / int64(lanesNow)
                            invoke(core, yp, wq, ws, xqp, xsp, GEMV_N, rb, re)
                        }
                    }
                }
            }
            let gbs = double(bytesPerPass) * double(reps) / us / 1000.0lf
            print("gemv lanes={lp}: {gbs} GB/s ({reps} passes, best-of-{REPS})\n")
            if (gbs > lastGBs * 1.10lf) {
                knee = lp
            }
            lastGBs = max(lastGBs, gbs)
        }
        lp = lp >= lanes ? lanes + 1 : min(lp * 2, lanes)
    }
    delete fx
    return knee
}

[export]
def main {
    if (!jit_enabled()) {
        print("WARNING: interpreted run — numbers are meaningless for the fastpath A/B; use -jit\n")
    }
    with_job_que() {
        setup_dasllama_jobque_()
        let lanes = get_dispatch_lanes()
        print("team_probe: {lanes} lanes (workers + caller), backend '{active_kernel_backend()}'\n")
        print("=== 1+2: empty-body team dispatch ===\n")
        let perChunkNs = empty_ladder(lanes)
        print("per-dispatch (K=L) {g_per_dispatch_ns} ns; per-chunk (slope L..8L) {perChunkNs} ns\n")
        print("=== 3: streaming GEMV bandwidth ladder ===\n")
        let knee = gemv_ladder(lanes)
        // target_chunk_work: MACs whose compute is ~8x the per-chunk overhead at a
        // conservative 10 MACs/ns single-lane floor
        let targetWork = int64(max(perChunkNs * 8.0lf * 10.0lf, 65536.0lf))
        print("=== box_profile runtime fragment (paste under \"runtime\") ===\n")
        print("    \"target_chunk_work\": {targetWork},\n")
        print("    \"gemv_lane_cap\": {knee},\n")
        print("    \"batch_lane_cap\": 0\n")
        print("(derivation: target = perChunk {perChunkNs} ns x 8 amortization x 10 MACs/ns floor; gemv cap = bandwidth knee)\n")
    }
}
