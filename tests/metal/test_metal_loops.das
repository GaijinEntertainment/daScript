options gen2
options indenting = 4

// Real-GPU behavioral gate for the Phase-2 loop kernel — deliberately dispatched on a NON-exact
// grid: 1000 threads at 64/threadgroup rounds up to 1024 launched threads, and the in-kernel
// bounds guard (`if (i >= n) return`) must keep the extra 24 from writing. Both output buffers
// are sentinel-filled and compared over the FULL padded range, so a broken guard shows up as
// clobbered sentinels. Also exercises while + break/continue, constant range-for, the dynamic-end
// range-for hoist (the body mutates `s`, which the range end was computed from — das evaluates
// endpoints once, and the emitted MSL must too), and a urange loop over the @uniform.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let NREAL = 1000        // guarded thread count (the uniform n)
let PAD = 1024          // ceil(1000/64)*64 — what the GPU actually launches
let TPG = 64u
let SENT_I = int(0x5EED5EED)
let SENT_U = 0xDEADBEEF

class Loops {
    @ssbo @binding = 0 src : array<int>
    @ssbo @binding = 1 dst : array<int>
    @uniform @binding = 2 n : uint
    @ssbo @binding = 3 udst : array<uint>

    [metal_kernel(name="loops_gpu_msl"), marker(no_coverage)]
    def loops {
        let i = gl_GlobalInvocationID.x
        if (i >= n) {
            return
        }
        var s = 0
        var j = 0
        while (j < 100) {
            j++
            if (j == 3) {
                continue
            }
            if (j >= 7) {
                break
            }
            s += j
        }
        for (k in range(4)) {
            s += k * src[i]
        }
        for (_k in range(0, s)) {
            s++
        }
        dst[i] = s
        var usum = 0u
        for (ku in urange(n)) {
            usum += ku
        }
        udst[i] = usum
    }
}

[test]
def test_loops_gpu_vs_cpu(t : T?) {
    t |> run("loops + bounds guard on a non-exact grid: GPU == CPU-reference") <| @(t : T?) {
        var oracle = new Loops()
        oracle.src |> resize(PAD)
        oracle.dst |> resize(PAD)
        oracle.udst |> resize(PAD)
        oracle.n = uint(NREAL)
        for (k in range(PAD)) {
            oracle.src[k] = k % 7 - 3
            oracle.dst[k] = SENT_I
            oracle.udst[k] = SENT_U
        }
        // the CPU driver mimics the padded GPU grid: all 1024 threads run, the guard returns for i >= n
        for (k in range(PAD)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->loops()
        }
        // k=0: while gives 1+2+4+5+6=18, range(4) adds (0+1+2+3)*src[0]= 6*(-3) -> s=0, dynamic loop
        // runs 0 times -> dst[0]=0; usum = sum(0..999) = 499500 — the oracle itself computed
        t |> equal(oracle.dst[0], 0)
        t |> equal(oracle.udst[0], 499500u)
        t |> equal(oracle.dst[NREAL], SENT_I)       // oracle's own guard left the pad untouched
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, loops_gpu_msl, loops_gpu_msl_entry, loops_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bsrc = buf_upload(dev, oracle.src)
                var bdst = buf_fill(dev, PAD, SENT_I)
                var bn = buf_fill(dev, 1, uint(NREAL))
                var budst = buf_fill(dev, PAD, SENT_U)
                var bufs <- [bsrc, bdst, bn, budst]
                var rerr : string
                // dispatch asks for NREAL threads; run_compute_1d rounds up to 16 groups = PAD threads
                let ran = run_compute_1d(queue, pso, bufs, uint(NREAL), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    // full padded range: [0,n) must match the oracle, [n,PAD) must still be sentinels
                    gpu_bad = buf_mismatch_exact(bdst, oracle.dst) + buf_mismatch_exact(budst, oracle.udst)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bsrc)
                metal_release(bdst)
                metal_release(bn)
                metal_release(budst)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; CPU-reference half only\n")
        }
        unsafe {
            delete oracle
        }
    }
}
