options gen2
options indenting = 4

// Phase-5 host plumbing gate: the pipeline cache compiles a source once (second lookup allocates
// no new Metal objects), the buffer pool reuses released buffers (re-acquire allocates nothing),
// and with_compute_encoder batches several dispatches into one synchronous submit (default hazard
// tracking orders same-buffer dispatches). The live-object counter is the observable for both
// caching claims — Metal objects are invisible to every das leak detector.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let N = 64
let TPG = 32u

class IncKernel {
    @ssbo @binding = 0 dat : array<uint>

    [metal_kernel(name="plumb_inc_msl"), marker(no_coverage)]
    def plumb_inc {
        let i = gl_GlobalInvocationID.x
        dat[i]++
    }
}

[test]
def test_host_plumbing(t : T?) {
    t |> run("pipeline cache + buffer pool + multi-dispatch encoder") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; plumbing checks skipped\n")
                    return
                }
                let base = metal_live_object_count()    // the device handle itself

                // pipeline cache: two lookups, one compile — the second adds no objects
                var pc : MetalPipelineCache
                var perr : string
                var pso1 = pipeline_cached(pc, dev, plumb_inc_msl, plumb_inc_msl_entry, plumb_inc_msl_fastmath, perr)
                t |> success(pso1 != null, "pipeline_cached compiles: {perr}")
                let after_first = metal_live_object_count()
                var pso2 = pipeline_cached(pc, dev, plumb_inc_msl, plumb_inc_msl_entry, plumb_inc_msl_fastmath, perr)
                t |> success(pso2 != null, "pipeline_cached hits")
                t |> equal(metal_live_object_count(), after_first)      // hit allocated nothing

                // buffer pool: acquire two, release both, re-acquire — no new allocations
                var pool : MetalBufferPool
                var ba = pool_acquire(pool, dev, uint64(N * 4))
                var bb = pool_acquire(pool, dev, uint64(N * 4))
                let after_bufs = metal_live_object_count()
                pool_release(pool, ba, uint64(N * 4))
                pool_release(pool, bb, uint64(N * 4))
                ba = pool_acquire(pool, dev, uint64(N * 4))
                bb = pool_acquire(pool, dev, uint64(N * 4))
                t |> equal(metal_live_object_count(), after_bufs)       // both reused from the pool

                // multi-dispatch: three increments in ONE submit — hazard tracking orders them
                var pu = unsafe(reinterpret<uint?>(metal_buffer_contents(ba)))
                for (k in range(N)) {
                    unsafe(pu[k]) = uint(k)
                }
                var queue = metal_new_command_queue(dev)
                var derr : string
                var gpu_ms = 0.0lf
                let ran = with_compute_encoder_timed(queue, derr, gpu_ms) $(enc : MetalComputeEncoder?) {
                    metal_set_pipeline(enc, pso1)
                    metal_set_buffer(enc, ba, 0ul, 0)
                    metal_dispatch_threadgroups(enc, uint3(uint(N) / TPG, 1u, 1u), uint3(TPG, 1u, 1u))
                    metal_dispatch_threadgroups(enc, uint3(uint(N) / TPG, 1u, 1u), uint3(TPG, 1u, 1u))
                    metal_dispatch_threadgroups(enc, uint3(uint(N) / TPG, 1u, 1u), uint3(TPG, 1u, 1u))
                }
                t |> success(ran, "with_compute_encoder_timed: {derr}")
                t |> success(gpu_ms > 0.0lf, "GPUStartTime/GPUEndTime bracket the completed work")
                var bad = 0
                for (k in range(N)) {
                    if (unsafe(pu[k]) != uint(k) + 3u) {
                        bad++
                    }
                }
                t |> equal(bad, 0)

                // teardown: caller releases what it holds, drains give the rest back
                pool_release(pool, ba, uint64(N * 4))
                pool_release(pool, bb, uint64(N * 4))
                pool_drain(pool)
                cache_drain(pc)
                metal_release(queue)
                t |> equal(metal_live_object_count(), base)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; nothing to check\n")
        }
    }
}

[test]
def test_residency_heartbeat(t : T?) {
    t |> run("residency heartbeat: arm, witness, unregister-on-release; submit trace branch") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; heartbeat checks skipped\n")
                    return
                }
                var rset = metal_new_residency_set(dev)
                if (rset == null) {
                    feint("MTLResidencySet unavailable (macOS < 15); heartbeat checks skipped\n")
                    return
                }
                metal_residency_set_commit(rset)
                metal_residency_set_request(rset)
                let sets0 = metal_residency_heartbeat_sets()
                let ticks0 = metal_residency_heartbeat_ticks()
                metal_residency_set_heartbeat(rset, 60)
                t |> success(metal_residency_heartbeat_live(), "the heartbeat is armed after a kick")
                t |> equal(metal_residency_heartbeat_sets(), sets0 + 1l, "the kick registered the set")
                metal_residency_set_heartbeat(rset, 60)
                t |> equal(metal_residency_heartbeat_sets(), sets0 + 1l, "a second kick does not register a duplicate")
                let ts = ref_time_ticks()
                while (get_time_usec(ts) < 30000) {}   // clock: control - bounds the tick window, never a reported figure
                t |> success(metal_residency_heartbeat_ticks() > ticks0, "the loop requested residency ({metal_residency_heartbeat_ticks() - ticks0} passes in the window)")
                metal_release(rset)
                t |> equal(metal_residency_heartbeat_sets(), sets0, "release unregistered the set from the heartbeat collection")
                // the loop keeps ticking on what remains — surviving 5 ms ticks past the
                // release is the no-use-after-free witness
                let ts2 = ref_time_ticks()
                while (get_time_usec(ts2) < 30000) {}   // clock: control
                t |> success(metal_residency_heartbeat_live(), "the loop survives ticks past the release (countdown intact)")
                // the submit-split trace branch: one traced empty submit through the chokepoint
                var queue = metal_new_command_queue(dev)
                var terr : string
                var tms = 0.0lf
                metal_submit_trace = true
                let tran = with_compute_encoder_timed(queue, terr, tms) $(enc : MetalComputeEncoder?) {}
                metal_submit_trace = false
                t |> success(tran, "a traced submit completes: {terr}")
                metal_release(queue)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; nothing to check\n")
        }
    }
}
