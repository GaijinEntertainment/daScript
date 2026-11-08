options gen2

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require metal/metal_builtins
require spirv/spirv_builtins  // nolint:STYLE029 — see above

// Real-GPU behavioral gate for Phase-3 simdgroup intrinsics. Simd ops
// have no sequential CPU replay (the identity stubs are width-1
// semantics), so the oracle is READBACK-DRIVEN: the kernel reports
// each thread's simdgroup width / lane / simdgroup id, and the CPU
// recomputes every expected value from those reported memberships —
// layout-agnostic (no assumption that lanes map to consecutive thread
// indices), uint bit-exact, float sums under the fastmath tolerance.
// Shuffles run in simdgroup-uniform flow; the undefined edge lanes of
// up/down are selected away on the VALUE (a shuffle inside divergent
// flow would read inactive lanes — undefined).
let N = 256             // exact grid: 4 threadgroups of 64
let TPG = 64u

class SimdG {
    @ssbo @binding = 0 v : array<uint>
    @ssbo @binding = 1 widtho : array<uint>
    @ssbo @binding = 2 laneo : array<uint>
    @ssbo @binding = 3 sgo : array<uint>
    @ssbo @binding = 4 osum : array<uint>
    @ssbo @binding = 5 orev : array<uint>
    @ssbo @binding = 6 oxor : array<uint>
    @ssbo @binding = 7 oup : array<uint>
    @ssbo @binding = 8 odn : array<uint>
    @ssbo @binding = 9 fv : array<float>
    @ssbo @binding = 10 fsum : array<float>

    [metal_kernel(name="simd_gpu_msl"), marker(no_coverage)]
    def simd_gpu {
        let i = gl_GlobalInvocationID.x
        let ln = gl_SubgroupInvocationID
        let w = gl_SubgroupSize
        widtho[i] = w
        laneo[i] = ln
        sgo[i] = gl_SubgroupID
        let x = v[i]
        osum[i] = simd_sum(x)
        orev[i] = simd_shuffle(x, (w - 1u) - ln)
        oxor[i] = simd_shuffle_xor(x, 1u)
        let up = simd_shuffle_up(x, 1u)
        let dn = simd_shuffle_down(x, 1u)
        oup[i] = (ln >= 1u) ? up : x
        odn[i] = ((ln + 1u) < w) ? dn : x
        fsum[i] = simd_sum(fv[i])
    }
}

// the thread in [tg0, tg1) sitting on `want_lane` of simdgroup
// `want_sg`, or -1
def private find_lane(tg0, tg1 : int; sgs, lanes : array<uint>; want_sg, want_lane : uint) : int {
    for (j in range(tg0, tg1)) {
        if (sgs[j] == want_sg && lanes[j] == want_lane) {
            return j
        }
    }
    return -1
}

[test(description = "A simdgroup sum and shuffles on the GPU equal values recomputed from the reported simdgroup membership.")]
def test_simd_gpu_vs_reported_membership(t : T?) { // nolint:STYLE038 - linear GPU-vs-CPU comparison body
    t |> run("simdgroup sum + shuffles: GPU == membership-recomputed expected") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var vs : array<uint>
            var fvs : array<float>
            vs |> resize(N)
            fvs |> resize(N)
            for (k in range(N)) {
                vs[k] = uint(k) * 2246822519u + 777u
                fvs[k] = float(k % 17) * 0.25 + 0.5
            }
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, simd_gpu_msl, simd_gpu_msl_entry,
                                               simd_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bv = buf_upload(dev, vs)
                var bw = buf_fill(dev, N, 0u)
                var bl = buf_fill(dev, N, 0u)
                var bsg = buf_fill(dev, N, 0u)
                var bsum = buf_fill(dev, N, 0u)
                var brev = buf_fill(dev, N, 0u)
                var bxor = buf_fill(dev, N, 0u)
                var bup = buf_fill(dev, N, 0u)
                var bdn = buf_fill(dev, N, 0u)
                var bfv = buf_upload(dev, fvs)
                var bfsum = buf_fill(dev, N, -1.0)
                var bufs <- [bv, bw, bl, bsg, bsum, brev, bxor, bup, bdn, bfv, bfsum]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    var widths <- buf_download(bw, N, type<uint>)
                    var lanes <- buf_download(bl, N, type<uint>)
                    var sgs <- buf_download(bsg, N, type<uint>)
                    var osums <- buf_download(bsum, N, type<uint>)
                    var orevs <- buf_download(brev, N, type<uint>)
                    var oxors <- buf_download(bxor, N, type<uint>)
                    var oups <- buf_download(bup, N, type<uint>)
                    var odns <- buf_download(bdn, N, type<uint>)
                    var fsums <- buf_download(bfsum, N, type<float>)
                    var wbad = 0
                    for (i in range(N)) {
                        if (widths[i] != widths[0]) {
                            wbad++
                        }
                    }
                    t |> success(widths[0] > 0u, "reported simdgroup width {widths[0]} is positive")
                    t |> equal(wbad, 0)
                    to_log(LOG_INFO, "simdgroup width on this device: {int(widths[0])}\n")
                    var bad = 0
                    for (i in range(N)) {
                        let tg0 = (i / int(TPG)) * int(TPG)
                        let tg1 = tg0 + int(TPG)
                        let w = widths[i]
                        let ln = lanes[i]
                        var su = 0u
                        var sf = 0.0
                        for (j in range(tg0, tg1)) {
                            if (sgs[j] == sgs[i]) {
                                su += vs[j]
                                sf += fvs[j]
                            }
                        }
                        bad += osums[i] != su ? 1 : 0
                        bad += !fapprox(fsums[i], sf) ? 1 : 0
                        let jr = find_lane(tg0, tg1, sgs, lanes, sgs[i], (w - 1u) - ln)
                        bad += (jr < 0 || orevs[i] != vs[jr]) ? 1 : 0
                        let jx = find_lane(tg0, tg1, sgs, lanes, sgs[i], ln ^ 1u)
                        bad += (jx < 0 || oxors[i] != vs[jx]) ? 1 : 0
                        if (ln >= 1u) {
                            let ju = find_lane(tg0, tg1, sgs, lanes, sgs[i], ln - 1u)
                            bad += (ju < 0 || oups[i] != vs[ju]) ? 1 : 0
                        } else {
                            bad += oups[i] != vs[i] ? 1 : 0
                        }
                        if (ln + 1u < w) {
                            let jd = find_lane(tg0, tg1, sgs, lanes, sgs[i], ln + 1u)
                            bad += (jd < 0 || odns[i] != vs[jd]) ? 1 : 0
                        } else {
                            bad += odns[i] != vs[i] ? 1 : 0
                        }
                    }
                    t |> equal(bad, 0)
                    delete widths
                    delete lanes
                    delete sgs
                    delete osums
                    delete orevs
                    delete oxors
                    delete oups
                    delete odns
                    delete fsums
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bv)
                metal_release(bw)
                metal_release(bl)
                metal_release(bsg)
                metal_release(bsum)
                metal_release(brev)
                metal_release(bxor)
                metal_release(bup)
                metal_release(bdn)
                metal_release(bfv)
                metal_release(bfsum)
                metal_release(pso)
                metal_release(queue)
            }
            t |> equal(metal_live_object_count(), 0l)
            delete vs
            delete fvs
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}

// Real-GPU behavioral gate vs the CPU-reference oracle for the
// atomic add: a threadgroup histogram over the keys of one
// 64-thread group and a device counter every thread bumps. The
// CPU half repeats the kernel's three barrier phases over one
// instance for every thread id, since the histogram and the
// counter are the group's shared state (the kernel has no
// per-thread member) and `atomicAdd`'s CPU body adds as the
// device's fetch-add does; the GPU half is static_if-gated on
// das_metal and skips cleanly where there is no Metal device.
let N_atomics = 64
let KEY_BINS = 11   //! the bins of the 64 the keys hit

class AtomicHist {
    @ssbo @binding = 0 keys : array<uint>
    @ssbo @binding = 1 bins : array<uint>
    @ssbo @binding = 2 total : array<int>
    @ssbo @binding = 3 ibins : array<int>
    @ssbo @binding = 4 utotal : array<uint>
    @ssbo @binding = 5 olds : array<int>
    @workgroup hist : uint[64]
    @workgroup ihist : int[64]

    [metal_kernel(name="atomic_hist_gpu_msl"), marker(no_coverage)]
    def atomic_hist {
        let i = gl_GlobalInvocationID.x
        let lid = gl_LocalInvocationID.x
        hist[lid] = 0u
        ihist[lid] = 0
        barrier()
        let _o = atomicAdd(hist[keys[i] & 63u], 1u)
        let _p = atomicAdd(ihist[keys[i] & 63u], 1)
        barrier()
        bins[i] = hist[lid]
        ibins[i] = ihist[lid]
        olds[i] = atomicAdd(total[0], 1)
        let _u = atomicAdd(utotal[0], 1u)
    }
}

//! one group's phases in the device's barrier order: every clear,
//! then every add, then every read
def private run_oracle(var oracle : AtomicHist?) {
    for (k in range(N_atomics)) {
        gl_LocalInvocationID = uint3(uint(k), 0u, 0u)
        oracle.hist[k] = 0u
        oracle.ihist[k] = 0
    }
    for (k in range(N_atomics)) {
        gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
        let _o = atomicAdd(oracle.hist[oracle.keys[k] & 63u], 1u)
        let _p = atomicAdd(oracle.ihist[oracle.keys[k] & 63u], 1)
    }
    for (k in range(N_atomics)) {
        oracle.bins[k] = oracle.hist[k]
        oracle.ibins[k] = oracle.ihist[k]
        oracle.olds[k] = atomicAdd(oracle.total[0], 1)
        let _u = atomicAdd(oracle.utotal[0], 1u)
    }
}

//! how many of the counts 0 .. N_atomics - 1 the device's returned
//! old values hold exactly once (its adds land in any order)
def private olds_seen_once(bo) : int {
    var seen : array<int>
    seen |> resize(N_atomics)
    let po = unsafe(reinterpret<int?>(metal_buffer_contents(bo)))
    for (k in range(N_atomics)) {
        let v = unsafe(po[k])
        if (v >= 0 && v < N_atomics) {
            seen[v]++
        }
    }
    var once = 0
    for (c in seen) {
        once += c == 1 ? 1 : 0
    }
    delete seen
    return once
}

//! the device half: the kernel over the oracle's keys, its bins,
//! counters and returned olds compared; -1 with no Metal device
def private gpu_mismatches(t : T?; oracle : AtomicHist?) : int {
    var gpu_bad = -1
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_metal_device() $(dev : MetalDevice?) {
            if (dev == null) {
                feint("no Metal device on this box; GPU compare skipped\n")
                return
            }
            var perr : string
            var pso = pipeline_from_source(dev, atomic_hist_gpu_msl,
                atomic_hist_gpu_msl_entry, atomic_hist_gpu_msl_fastmath, perr)
            t |> success(pso != null, "pipeline_from_source: {perr}")
            if (pso == null) {
                return
            }
            var queue = metal_new_command_queue(dev)
            var bk = buf_upload(dev, oracle.keys)
            var bb = buf_fill(dev, N_atomics, 0u)
            var bt = buf_fill(dev, 1, 0)
            var bi = buf_fill(dev, N_atomics, 0)
            var bu = buf_fill(dev, 1, 0u)
            var bo = buf_fill(dev, N_atomics, -1)
            var bufs <- [bk, bb, bt, bi, bu, bo]
            var rerr : string
            let ran = run_compute_1d(queue, pso, bufs, uint(N_atomics), TPG, rerr,
                atomic_hist_gpu_msl_tgmem)
            t |> success(ran, "run_compute_1d: {rerr}")
            if (ran) {
                gpu_bad = (buf_mismatch_exact(bb, oracle.bins)
                    + buf_mismatch_exact(bt, oracle.total)
                    + buf_mismatch_exact(bi, oracle.ibins)
                    + buf_mismatch_exact(bu, oracle.utotal))
                t |> equal(olds_seen_once(bo), N_atomics,
                    "the device's returned old values are every count from 0 to N_atomics - 1 once")
            }
            bufs |> clear()             // non-owning handles — clear() skips pointee finalize
            unsafe {
                delete bufs
            }
            metal_release(bk)
            metal_release(bb)
            metal_release(bt)
            metal_release(bi)
            metal_release(bu)
            metal_release(bo)
            metal_release(pso)
            metal_release(queue)
        }
    }
    return gpu_bad
}

[test(description = "Atomic adds into a threadgroup histogram and a device counter give the CPU-computed totals.")]
def test_atomic_add_gpu_vs_cpu(t : T?) {
    t |> run("atomic add: a threadgroup histogram and a device counter,"
        + " GPU == CPU-reference") <| @(t : T?) {
        var oracle = new AtomicHist()
        oracle.keys |> resize(N_atomics)
        oracle.bins |> resize(N_atomics)
        oracle.total |> resize(1)
        oracle.ibins |> resize(N_atomics)
        oracle.utotal |> resize(1)
        oracle.olds |> resize(N_atomics)
        for (k in range(N_atomics)) {
            oracle.keys[k] = uint((k * 7) % KEY_BINS)
        }
        run_oracle(oracle)
        var summed = 0u
        for (k in range(N_atomics)) {
            summed += oracle.bins[k]
        }
        t |> equal(summed, uint(N_atomics), "the oracle's bins hold every key")
        t |> equal(oracle.bins[0], 6u, "bin 0 holds the six keys that land there")
        t |> equal(oracle.total[0], N_atomics, "the oracle's counter counts every thread")
        t |> equal(oracle.ibins[0], 6, "the int histogram's bin 0 holds the same six")
        t |> equal(oracle.utotal[0], uint(N_atomics), "the uint counter counts every thread")
        t |> equal(oracle.olds[0], 0, "the CPU add returns the old value: the first thread read 0")
        t |> equal(oracle.olds[N_atomics - 1], N_atomics - 1,
                   "... and the last thread read N_atomics - 1")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let gpu_bad = gpu_mismatches(t, oracle)
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0, "the device's bins and counter are the oracle's")
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

// Real-GPU behavioral gate for Phase-3 threadgroup memory + barriers:
// the classic tree reduction — each threadgroup loads 64 elements
// into a @workgroup tile, barriers, halves, and thread 0 writes the
// partial. A barrier kernel cannot be replayed by the sequential
// same-body driver (thread 0 would reduce before the other lanes ever
// load), so the CPU reference here is the directly computed per-group
// sum: uint wrap-exact, float under the fastmath tolerance. A missing
// or misplaced barrier shows up as partials built from stale tile
// lanes.
let N_reduce = 4096            // exact grid: 64 threadgroups of 64
let GROUPS = 64

class Reduce {
    @ssbo @binding = 0 usrc : array<uint>
    @ssbo @binding = 1 udst : array<uint>
    @ssbo @binding = 2 fsrc : array<float>
    @ssbo @binding = 3 fdst : array<float>
    @workgroup utile : uint[64]
    @workgroup ftile : float[64]

    [metal_kernel(name="reduce_u_gpu_msl"), marker(no_coverage)]
    def reduce_u {
        let gid = gl_GlobalInvocationID.x
        let lid = gl_LocalInvocationID.x
        utile[lid] = usrc[gid]
        barrier()
        var s = gl_WorkGroupSize.x / 2u
        while (s > 0u) {
            if (lid < s) {
                utile[lid] += utile[lid + s]
            }
            barrier()
            s /= 2u
        }
        if (lid == 0u) {
            udst[gl_WorkGroupID.x] = utile[0]
        }
    }

    [metal_kernel(name="reduce_f_gpu_msl"), marker(no_coverage)]
    def reduce_f {
        let gid = gl_GlobalInvocationID.x
        let lid = gl_LocalInvocationID.x
        ftile[lid] = fsrc[gid]
        barrier()
        var s = gl_WorkGroupSize.x / 2u
        while (s > 0u) {
            if (lid < s) {
                ftile[lid] += ftile[lid + s]
            }
            barrier()
            s /= 2u
        }
        if (lid == 0u) {
            fdst[gl_WorkGroupID.x] = ftile[0]
        }
    }
}

[test(description = "A threadgroup tree reduction yields partials equal to the directly computed sums.")]
def test_reduce_gpu_vs_cpu(t : T?) { // nolint:STYLE038 - linear GPU-vs-CPU comparison body
    t |> run("threadgroup tree reduction: GPU partials == directly computed sums") <| @(t : T?) {
        var usrc : array<uint>
        var fsrc : array<float>
        usrc |> resize(N_reduce)
        fsrc |> resize(N_reduce)
        for (k in range(N_reduce)) {
            usrc[k] = uint(k) * 2654435761u + 12345u    // wraps — bit-exact on both sides
            fsrc[k] = float(k % 31) * 0.125 + 0.5
        }
        var wantU : array<uint>
        var wantF : array<float>
        wantU |> resize(GROUPS)
        wantF |> resize(GROUPS)
        for (g in range(GROUPS)) {
            var su = 0u
            var sf = 0.0
            for (k in range(int(TPG))) {
                su += usrc[g * int(TPG) + k]
                sf += fsrc[g * int(TPG) + k]
            }
            wantU[g] = su
            wantF[g] = sf
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var psoU = pipeline_from_source(dev, reduce_u_gpu_msl, reduce_u_gpu_msl_entry,
                                                reduce_u_gpu_msl_fastmath, perr)
                t |> success(psoU != null, "pipeline_from_source(u): {perr}")
                var psoF = pipeline_from_source(dev, reduce_f_gpu_msl, reduce_f_gpu_msl_entry,
                                                reduce_f_gpu_msl_fastmath, perr)
                t |> success(psoF != null, "pipeline_from_source(f): {perr}")
                if (psoU == null || psoF == null) {
                    if (psoU != null) {
                        metal_release(psoU)
                    }
                    if (psoF != null) {
                        metal_release(psoF)
                    }
                    return
                }
                var queue = metal_new_command_queue(dev)
                var busrc = buf_upload(dev, usrc)
                var budst = buf_fill(dev, GROUPS, 0xABAD1DEA)
                var bfsrc = buf_upload(dev, fsrc)
                var bfdst = buf_fill(dev, GROUPS, -1.0)
                var bufs <- [busrc, budst, bfsrc, bfdst]
                var rerr : string
                let ranU = run_compute_1d(queue, psoU, bufs, uint(N_reduce), TPG, rerr,
                                          reduce_u_gpu_msl_tgmem)
                t |> success(ranU, "run_compute_1d(u): {rerr}")
                let ranF = run_compute_1d(queue, psoF, bufs, uint(N_reduce), TPG, rerr,
                                          reduce_f_gpu_msl_tgmem)
                t |> success(ranF, "run_compute_1d(f): {rerr}")
                if (ranU && ranF) {
                    gpu_bad = buf_mismatch_exact(budst, wantU) + buf_mismatch_approx(bfdst, wantF)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(busrc)
                metal_release(budst)
                metal_release(bfsrc)
                metal_release(bfdst)
                metal_release(psoU)
                metal_release(psoF)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
        delete usrc
        delete fsrc
        delete wantU
        delete wantF
    }
}

// The four gl_Subgroup* IDs are ONE declaration in
// shader_lingua_franca shared by both rails — requiring both builtins
// modules in one TU must stay unambiguous (each carrying its own copy
// was error[30615]). Reading and writing all four pins the
// single-declaration property; the width-1 defaults are what a CPU
// replay relies on.
[test(description = "The gl_Subgroup* builtins resolve once across the metal and spirv builtin modules.")]
def test_subgroup_ids_shared(t : T?) {
    t |> run("gl_Subgroup* resolve once across metal+spirv builtins") <| @(t : T?) {
        t |> equal(int(gl_SubgroupSize), 1)         // the width-1 CPU-replay default
        t |> equal(int(gl_SubgroupInvocationID), 0)
        gl_SubgroupSize = 4u
        gl_SubgroupInvocationID = 1u
        gl_NumSubgroups = 2u
        gl_SubgroupID = 1u
        t |> equal(int(gl_SubgroupSize + gl_SubgroupInvocationID + gl_NumSubgroups + gl_SubgroupID),
                   8)
        gl_SubgroupSize = 1u
        gl_SubgroupInvocationID = 0u
        gl_NumSubgroups = 1u
        gl_SubgroupID = 0u
    }
}
