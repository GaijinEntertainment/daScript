options gen2

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 - used in the das_metal static_if half

// Real-GPU behavioral gate vs the CPU-reference oracle for the
// Phase-2 control-flow kernel: mutable/zero-init locals, compound
// assignment, ++/-- (statement + value postfix), every comparison
// flavor per scalar class, logical ops, if/elif/else, ternary. Fills
// are chosen so every branch arm executes for some thread. uint/int
// outputs compare bit-exact; float with fastmath tolerance.
let N = 256
let TPG = 64u

class Control {
    @ssbo @binding = 0 a : array<uint>
    @ssbo @binding = 1 b : array<uint>
    @ssbo @binding = 2 p : array<int>
    @ssbo @binding = 3 q : array<int>
    @ssbo @binding = 4 f : array<float>
    @ssbo @binding = 5 g : array<float>
    @ssbo @binding = 6 out1 : array<uint>
    @ssbo @binding = 7 out2 : array<int>
    @ssbo @binding = 8 out3 : array<float>

    [metal_kernel(name="control_gpu_msl"), marker(no_coverage)]
    def control {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i]
        var acc = 0u
        var zed : uint
        var hot = false
        if (x > y && x != 0u) {
            acc += x
            hot = true
        } elif (x < y || y >= 8u) {
            acc -= x
        } else {
            acc *= 3u
        }
        if (x <= y) {
            acc /= 2u
            acc %= 101u
        }
        if (x == y) {
            acc |= 1u
            acc &= 255u
            acc ^= 129u
            acc <<= 1u
            acc >>= 1u
        }
        acc++
        ++acc
        acc--
        --acc
        let snap = acc++
        let dnap = acc--
        zed += (acc + snap) + dnap
        out1[i] = hot ? zed : (zed + 7u)
        let m = p[i]
        let w = q[i]
        var iacc = 0
        if (m > w) {
            iacc += 9
        }
        if (m < w) {
            iacc += 2
        }
        if (m >= w) {
            iacc += 3
        }
        if (m <= w) {
            iacc += 4
        }
        if (m == w) {
            iacc += 5
        }
        if (m != w) {
            iacc += 6
        }
        out2[i] = (!(m > w)) ? iacc : (-iacc)
        let s = f[i]
        let z = g[i]
        var facc = 1.0
        if (s > z) {
            facc += 2.0
        }
        if (s < z) {
            facc += 4.0
        }
        if (s >= z) {
            facc *= 2.0
        }
        if (s <= z) {
            facc += 8.0
        }
        if (s == z) {
            facc += 16.0
        }
        if (s != z) {
            facc += 32.0
        }
        out3[i] = facc
    }
}

[test(description = "Control-flow kernels run on the GPU give the same results as the CPU reference.")]
def test_control_gpu_vs_cpu(t : T?) { // nolint:STYLE038 - linear GPU-vs-CPU comparison body
    t |> run("control flow: GPU == CPU-reference") <| @(t : T?) {
        var oracle = new Control()
        oracle.a |> resize(N)
        oracle.b |> resize(N)
        oracle.p |> resize(N)
        oracle.q |> resize(N)
        oracle.f |> resize(N)
        oracle.g |> resize(N)
        oracle.out1 |> resize(N)
        oracle.out2 |> resize(N)
        oracle.out3 |> resize(N)
        for (k in range(N)) {
            oracle.a[k] = uint(k % 11)
            oracle.b[k] = uint(k % 7)
            oracle.p[k] = k % 9 - 4
            oracle.q[k] = k % 5 - 2
            oracle.f[k] = float(k % 6)
            oracle.g[k] = float(k % 4)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->control()
        }
        // k=0: s==z (both 0) -> facc = ((1 *= 2) += 8) += 16 = 26 — the
        // oracle itself computed
        t |> success(fapprox(oracle.out3[0], 26.0), "float branch anchor (k=0 -> 26.0)")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, control_gpu_msl, control_gpu_msl_entry,
                                               control_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, oracle.a)
                var bb = buf_upload(dev, oracle.b)
                var bp = buf_upload(dev, oracle.p)
                var bq = buf_upload(dev, oracle.q)
                var bf = buf_upload(dev, oracle.f)
                var bg = buf_upload(dev, oracle.g)
                var bo1 = buf_fill(dev, N, 0u)
                var bo2 = buf_fill(dev, N, 0)
                var bo3 = buf_fill(dev, N, 0.0)
                var bufs <- [ba, bb, bp, bq, bf, bg, bo1, bo2, bo3]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = ((buf_mismatch_exact(bo1, oracle.out1) + buf_mismatch_exact(bo2, oracle.out2)) +
                        buf_mismatch_approx(bo3, oracle.out3))
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bb)
                metal_release(bp)
                metal_release(bq)
                metal_release(bf)
                metal_release(bg)
                metal_release(bo1)
                metal_release(bo2)
                metal_release(bo3)
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

// Real-GPU behavioral gate for the Phase-2 loop kernel — deliberately
// dispatched on a NON-exact grid: 1000 threads at 64/threadgroup
// rounds up to 1024 launched threads, and the in-kernel bounds guard
// (`if (i >= n) return`) must keep the extra 24 from writing. Both
// output buffers are sentinel-filled and compared over the FULL
// padded range, so a broken guard shows up as clobbered sentinels.
// Also exercises while + break/continue, constant range-for, the
// dynamic-end range-for hoist (the body mutates `s`, which the range
// end was computed from — das evaluates endpoints once, and the
// emitted MSL must too), and a urange loop over the @uniform.
let NREAL = 1000        // guarded thread count (the uniform n)
let PAD = 1024          // ceil(1000/64)*64 — what the GPU actually launches
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

[test(description = "Loops with a bounds guard on a non-exact grid match the CPU reference and leave the sentinel padding untouched.")]
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
        // the CPU driver mimics the padded GPU grid: all 1024 threads run,
        // the guard returns for i >= n
        for (k in range(PAD)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->loops()
        }
        // k=0: while gives 1+2+4+5+6=18, range(4) adds (0+1+2+3)*src[0]=
        // 6*(-3) -> s=0, dynamic loop runs 0 times -> dst[0]=0; usum =
        // sum(0..999) = 499500 — the oracle itself computed
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
                var pso = pipeline_from_source(dev, loops_gpu_msl, loops_gpu_msl_entry,
                                               loops_gpu_msl_fastmath, perr)
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
                // dispatch asks for NREAL threads; run_compute_1d rounds up to 16
                // groups = PAD threads
                let ran = run_compute_1d(queue, pso, bufs, uint(NREAL), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    // full padded range: [0,n) must match the oracle, [n,PAD) must still
                    // be sentinels
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

// Real-GPU behavioral gate for the Phase-3 workgroup-grid builtins
// (thread_position_in_threadgroup, threadgroup_position_in_grid,
// threadgroups_per_grid, threads_per_threadgroup,
// thread_index_in_threadgroup). The kernel has no barrier and no simd
// ops, so the same-body CPU-reference replay stays valid: the driver
// walks the exact grid setting all six builtin globals per thread,
// and the GPU result must match bit-exactly.
let N_ids = 512             // exact grid: 8 threadgroups of 64
let GROUPS = 8u

class GridIds {
    @ssbo @binding = 0 code : array<uint>
    @ssbo @binding = 1 dims : array<uint>

    [metal_kernel(name="grid_ids_msl"), marker(no_coverage)]
    def grid_ids {
        let gid = gl_GlobalInvocationID.x
        code[gid] = ((gl_WorkGroupID.x * 1000u) + gl_LocalInvocationID.x) * 10000u + gl_LocalInvocationIndex
        dims[gid] = (gl_WorkGroupSize.x * 100000u) + gl_NumWorkGroups.x
    }
}

[test(description = "The workgroup-grid builtins read on the GPU equal a CPU replay of the grid.")]
def test_grid_ids_gpu_vs_cpu(t : T?) {
    t |> run("workgroup-grid builtins: GPU == CPU-reference replay") <| @(t : T?) {
        var oracle = new GridIds()
        oracle.code |> resize(N_ids)
        oracle.dims |> resize(N_ids)
        for (k in range(N_ids)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            gl_LocalInvocationID = uint3(uint(k) % TPG, 0u, 0u)
            gl_LocalInvocationIndex = uint(k) % TPG
            gl_WorkGroupID = uint3(uint(k) / TPG, 0u, 0u)
            gl_WorkGroupSize = uint3(TPG, 1u, 1u)
            gl_NumWorkGroups = uint3(GROUPS, 1u, 1u)
            oracle->grid_ids()
        }
        // spot-check the replay itself: k=65 -> workgroup 1, local id/index 1
        t |> equal(oracle.code[0], 0u)
        t |> equal(oracle.code[65], 10010001u)
        t |> equal(oracle.dims[0], 6400008u)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, grid_ids_msl, grid_ids_msl_entry,
                                               grid_ids_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bcode = buf_fill(dev, N_ids, 0u)
                var bdims = buf_fill(dev, N_ids, 0u)
                var bufs <- [bcode, bdims]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N_ids), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_exact(bcode, oracle.code) + buf_mismatch_exact(bdims, oracle.dims)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bcode)
                metal_release(bdims)
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
