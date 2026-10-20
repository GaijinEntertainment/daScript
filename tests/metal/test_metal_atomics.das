options gen2
options indenting = 4

// Real-GPU behavioral gate vs the CPU-reference oracle for the
// atomic add: a threadgroup histogram over the keys of one
// 64-thread group and a device counter every thread bumps. The
// CPU half repeats the kernel's three barrier phases over one
// instance for every thread id, since the histogram and the
// counter are the group's shared state (the kernel has no
// per-thread member) and `atomicAdd`'s CPU body adds as the
// device's fetch-add does; the GPU half is static_if-gated on
// das_metal and skips cleanly where there is no Metal device.


require dastest/testing_boost public
require metal/msl_shader
require _metal_common   // nolint:STYLE030,LINT019 — the das_metal half alone

let N = 64
let KEY_BINS = 11   //! the bins of the 64 the keys hit
let TPG = 64u

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
    for (k in range(N)) {
        gl_LocalInvocationID = uint3(uint(k), 0u, 0u)
        oracle.hist[k] = 0u
        oracle.ihist[k] = 0
    }
    for (k in range(N)) {
        gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
        let _o = atomicAdd(oracle.hist[oracle.keys[k] & 63u], 1u)
        let _p = atomicAdd(oracle.ihist[oracle.keys[k] & 63u], 1)
    }
    for (k in range(N)) {
        oracle.bins[k] = oracle.hist[k]
        oracle.ibins[k] = oracle.ihist[k]
        oracle.olds[k] = atomicAdd(oracle.total[0], 1)
        let _u = atomicAdd(oracle.utotal[0], 1u)
    }
}

//! how many of the counts 0 .. N - 1 the device's returned old values
//! hold exactly once (its adds land in any order)
def private olds_seen_once(bo) : int {
    var seen : array<int>
    seen |> resize(N)
    let po = unsafe(reinterpret<int?>(metal_buffer_contents(bo)))
    for (k in range(N)) {
        let v = unsafe(po[k])
        if (v >= 0 && v < N) {
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
            var bb = buf_fill(dev, N, 0u)
            var bt = buf_fill(dev, 1, 0)
            var bi = buf_fill(dev, N, 0)
            var bu = buf_fill(dev, 1, 0u)
            var bo = buf_fill(dev, N, -1)
            var bufs <- [bk, bb, bt, bi, bu, bo]
            var rerr : string
            let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr,
                atomic_hist_gpu_msl_tgmem)
            t |> success(ran, "run_compute_1d: {rerr}")
            if (ran) {
                gpu_bad = (buf_mismatch_exact(bb, oracle.bins)
                    + buf_mismatch_exact(bt, oracle.total)
                    + buf_mismatch_exact(bi, oracle.ibins)
                    + buf_mismatch_exact(bu, oracle.utotal))
                t |> equal(olds_seen_once(bo), N,
                    "the device's returned old values are every count from 0 to N - 1 once")
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

[test]
def test_atomic_add_gpu_vs_cpu(t : T?) {
    t |> run("atomic add: a threadgroup histogram and a device counter,"
        + " GPU == CPU-reference") <| @(t : T?) {
        var oracle = new AtomicHist()
        oracle.keys |> resize(N)
        oracle.bins |> resize(N)
        oracle.total |> resize(1)
        oracle.ibins |> resize(N)
        oracle.utotal |> resize(1)
        oracle.olds |> resize(N)
        for (k in range(N)) {
            oracle.keys[k] = uint((k * 7) % KEY_BINS)
        }
        run_oracle(oracle)
        var summed = 0u
        for (k in range(N)) {
            summed += oracle.bins[k]
        }
        t |> equal(summed, uint(N), "the oracle's bins hold every key")
        t |> equal(oracle.bins[0], 6u, "bin 0 holds the six keys that land there")
        t |> equal(oracle.total[0], N, "the oracle's counter counts every thread")
        t |> equal(oracle.ibins[0], 6, "the int histogram's bin 0 holds the same six")
        t |> equal(oracle.utotal[0], uint(N), "the uint counter counts every thread")
        t |> equal(oracle.olds[0], 0, "the CPU add returns the old value: the first thread read 0")
        t |> equal(oracle.olds[N - 1], N - 1, "... and the last thread read N - 1")
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
