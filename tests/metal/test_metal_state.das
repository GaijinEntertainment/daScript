options gen2
options indenting = 4

// Behavioral gate for STATE members: a plain (unannotated) member on a [metal_kernel] class is a
// kernel-entry thread-local, so every GPU thread starts from the declared initializer — scalars
// from their constant init, fixed arrays zero, carried pointers unbound. That lifetime is the
// contract, and the CPU oracle has to be written to match it: a FRESH instance per simulated lane.
// One instance looped across lanes (the shape the rest of this suite uses, which is correct for
// stateless kernels) would carry lane i-1's final boost and tally into lane i, so it disagrees with
// the GPU from lane 1 onward. The text-level twin (the thread-local declarations) is tests/msl/.

require dastest/testing_boost public
require metal/msl_shader
require ?das_metal metal/das_metal_boost
require _metal_common   // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half

let N = 256
let STEP = 4        // per-lane internal loop length; the kernel spells it as the literal 4
let W = N * STEP    // input width: STEP elements per lane
let TPG = 64u

class StKern {
    @ssbo @binding = 0 sa : array<float>
    @ssbo @binding = 1 so : array<float>
    cur : float const?      // carried-pointer state: bound once per lane, advanced per step
    boost : float = 0.25    // scalar state with a constant initializer — read AND written per step
    tally : float[4]        // fixed-array state: zero per lane, accumulated into per step

    def seed(i : uint) {    // the stage_init shape: bind the carried pointer once
        cur = unsafe(addr(sa[i * 4u]))
    }

    def bump(k : uint) {    // reads its own state, then advances both riders
        tally[k] += unsafe(cur[0]) * boost
        cur = unsafe(cur + 1)
        boost += 0.5
    }

    def total : float => ((tally[0] + tally[1]) + tally[2]) + tally[3]

    [metal_kernel(name="stkern_msl"), marker(no_coverage)]
    def stkern {
        let i = gl_GlobalInvocationID.x
        seed(i)
        var k = 0u
        while (k < 4u) {
            bump(k)
            k++
        }
        so[i] = total()
    }
}

// One simulated lane, on its OWN instance — the per-thread state lifetime made executable. The
// declared initializers come from the class constructor, so the oracle never restates 0.25 here.
def private st_lane(src : array<float>; i : int) : float {
    var lane = new StKern()
    lane.sa := src
    lane.so |> resize(N)
    gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
    lane->stkern()
    let v = lane.so[i]
    unsafe {
        delete lane
    }
    return v
}

// Closed form, independent of both runs: lane i walks its own STEP inputs and weights step j by
// the boost its OWN state has reached (0.25 + 0.5 * j), starting from a zero tally.
def private st_want(src : array<float>) : array<float> {
    var res : array<float>
    res |> resize(N)
    for (i in range(N)) {
        var s = 0.0
        for (j in range(STEP)) {
            s += src[i * STEP + j] * (0.25 + 0.5 * float(j))
        }
        res[i] = s
    }
    return <- res
}

[test]
def test_state_members_gpu_vs_cpu(t : T?) {
    t |> run("state members: per-thread thread-locals on GPU == fresh CPU instance per lane") <| @(t : T?) {
        var src : array<float>
        src |> resize(W)
        for (k in range(W)) {
            src[k] = float(k % 9)
        }
        let want <- st_want(src)
        var cpu : array<float>
        cpu |> resize(N)
        for (i in range(N)) {
            cpu[i] = st_lane(src, i)
        }
        // this compare is the lifetime gate on every platform: a lane that inherited the previous
        // lane's boost and tally lands nowhere near the closed form (lane 1: 77.0 instead of 24.5)
        var cpu_bad = 0
        for (i in range(N)) {
            if (cpu[i] != want[i]) {
                cpu_bad++
            }
        }
        t |> equal(cpu_bad, 0)
        // i=1: inputs 4,5,6,7 -> 4*0.25 + 5*0.75 + 6*1.25 + 7*1.75 = 24.5
        t |> equal(cpu[1], 24.5)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, stkern_msl, stkern_msl_entry, stkern_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, src)
                var bo = buf_fill(dev, N, -1000.0)
                var bufs <- [ba, bo]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = 0
                    var go <- buf_download(bo, N, type<float>)
                    for (i in range(N)) {
                        if (go[i] != cpu[i]) {
                            gpu_bad++
                        }
                    }
                    delete go
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bo)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; the CPU lifetime gate asserted above\n")
        }
    }
}
