options gen2
options indenting = 4

// Phase 0 behavioral gate (user-function lowering): the kernel body is a chain of ordinary
// das functions — nested calls, a hidden-builtin reader, a thread-ref accumulator, generic
// twin instances (float + int), and write-through var array params. The CPU run of the same
// method is the oracle; the GPU result must match exactly (integer-valued floats). The
// text-level twin (signatures, address spaces, prototypes) is tests/msl/test_msl_functions.das.

require dastest/testing_boost public
require metal/msl_shader
require ?das_metal metal/das_metal_boost
require _metal_common
require math

let N = 256
let TPG = 64u

def private fk_lane : uint => gl_GlobalInvocationID.x

def private fk_mix(x, y : float) : float => x * 2.0 + y

def private fk_pick(src : array<auto(TT)>; i : uint) : TT {
    return src[i] + src[i]
}

def private fk_accum(var acc : float&; v : float) {
    acc += v
}

def private fk_store(var dst : array<float>; i : uint; v : float) {
    dst[i] = v
}

// the staged shape: a helper fills the caller's @workgroup slab, rendezvouses inside, and a
// second helper reads it back const — what dasLLAMA's attention softmax halves do
def private fk_fill_tg(var tile : float[64]; v : float; lid : uint) {
    tile[lid] = v
    barrier()
}

def private fk_sum_tg(tile : float[64]; n : uint) : float {
    var s = 0.0
    var k = 0u
    while (k < n) {
        s += tile[k]
        k++
    }
    return s
}

// pointer parameters: `T?` names no address space in das, so the parameter declares it
// (unmarked = device, `@threadgroup` opts in) and the emitter checks it against the provenance
// of every pointer handed over. This is the split-K attention shape — advance, don't index.
def private fk_ptr_dot(var p : float const?; var q : float const?; n : uint) : float {
    var s = 0.0
    var k = 0u
    while (k < n) {
        s += unsafe(p[0]) * unsafe(q[0])
        p = unsafe(p + 1)
        q = unsafe(q + 1)
        k++
    }
    return s
}

def private fk_ptr_tg_max(@threadgroup p : float const?; n : uint) : float {
    var m = -1.0e30
    var k = 0u
    while (k < n) {
        m = max(m, unsafe(p[k]))
        k++
    }
    return m
}

def private fk_ptr_store(var o : float?; i : uint; v : float) {
    unsafe(o[i]) = v
}

class FnPtr {
    @ssbo @binding = 0 pa : array<float>
    @ssbo @binding = 1 pb : array<float>
    @ssbo @binding = 2 pc : array<float>
    @workgroup ptile : float[64]

    [metal_kernel(name="fnptr_msl"), marker(no_coverage)]
    def fnptr {
        let i = gl_GlobalInvocationID.x
        ptile[gl_LocalInvocationID.x] = pa[i]
        barrier()
        let d = fk_ptr_dot(unsafe(addr(pa[i * 4u])), unsafe(addr(pb[i * 4u])), 4u)
        let m = fk_ptr_tg_max(unsafe(addr(ptile[0])), gl_WorkGroupSize.x)
        fk_ptr_store(unsafe(addr(pc[0])), i, d + m)
    }
}

class FnBase {
    @ssbo @binding = 0 fa : array<float>
    @ssbo @binding = 1 fc : array<float>
    @uniform @binding = 2 gain : float

    def pick(i : uint) : float => fa[i]
    def combo(i : uint) : float => pick(i) * gain   // base skeleton over the overridable stage
}

class FnKid : FnBase {
    @ssbo @binding = 3 fb : array<float>

    def override pick(i : uint) : float => fa[i] + fb[i]

    def store(i : uint; v : float) {
        fc[i] = v
    }

    [metal_kernel(name="fncls_msl"), marker(no_coverage)]
    def fncls {
        let i = gl_GlobalInvocationID.x
        store(i, combo(i) + fk_mix(fa[i], fb[i]))
    }
}

class FnKern {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>
    @ssbo @binding = 3 d : array<int>
    @ssbo @binding = 4 e : array<int>

    [metal_kernel(name="fnkern_msl"), marker(no_coverage)]
    def fnkern {
        let i = fk_lane()
        var acc = 0.0
        fk_accum(acc, fk_mix(a[i], b[i]))
        fk_store(c, i, acc + fk_pick(a, i))
        e[i] = fk_pick(d, i) + 1
    }
}

// The threadgroup-slab scenario gets its own kernel: a group-wide slab does not replay through
// the sequential CPU oracle (a barrier has no width-1 meaning), so its expectation is computed
// directly — lane i sums its OWN group's inputs, in the helper's ascending order.
class FnTg {
    @ssbo @binding = 0 ta : array<float>
    @ssbo @binding = 1 to_ : array<float>
    @workgroup tile : float[64]

    [metal_kernel(name="fntg_msl"), marker(no_coverage)]
    def fntg {
        let i = gl_GlobalInvocationID.x
        fk_fill_tg(tile, ta[i], gl_LocalInvocationID.x)   // writes the slab, barriers inside
        to_[i] = fk_sum_tg(tile, gl_WorkGroupSize.x)      // reads it back const
    }
}

[test]
def test_functions_gpu_vs_cpu(t : T?) {
    t |> run("user-function chain: emitted MSL on GPU == CPU-reference run of the same method") <| @(t : T?) {
        var oracle = new FnKern()
        oracle.a |> resize(N)
        oracle.b |> resize(N)
        oracle.c |> resize(N)
        oracle.d |> resize(N)
        oracle.e |> resize(N)
        for (i in range(N)) {
            oracle.a[i] = float(i % 31)
            oracle.b[i] = float(i % 7 + 1)
            oracle.d[i] = i % 13 - 6
        }
        for (i in range(N)) {
            gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
            oracle->fnkern()
        }
        // i=9: acc = 9*2+3 = 21; c = 21 + 18 = 39; e = 2*3 + 1 = 7
        t |> equal(oracle.c[9], 39.0)
        t |> equal(oracle.e[9], 7)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, fnkern_msl, fnkern_msl_entry, fnkern_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, oracle.a)
                var bb = buf_upload(dev, oracle.b)
                var bc = buf_fill(dev, N, -1000.0)
                var bd = buf_upload(dev, oracle.d)
                var be = buf_fill(dev, N, -1000)
                var bufs <- [ba, bb, bc, bd, be]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = 0
                    var gc <- buf_download(bc, N, type<float>)
                    var ge <- buf_download(be, N, type<int>)
                    for (i in range(N)) {
                        if (gc[i] != oracle.c[i] || ge[i] != oracle.e[i]) {
                            gpu_bad++
                        }
                    }
                    delete gc
                    delete ge
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bb)
                metal_release(bc)
                metal_release(bd)
                metal_release(be)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; CPU oracle asserted above\n")
        }
        unsafe {
            delete oracle
        }
    }
}

[test]
def test_threadgroup_param_gpu(t : T?) {
    t |> run("writable threadgroup-slab parameter + barrier inside a helper: group sums match") <| @(t : T?) {
        var src : array<float>
        src |> resize(N)
        for (i in range(N)) {
            src[i] = float(i % 5 + 1)
        }
        // lane i sums its own group's inputs, summed ascending — the helper's exact order
        var want : array<float>
        want |> resize(N)
        for (i in range(N)) {
            let g0 = (i / int(TPG)) * int(TPG)
            var s = 0.0
            for (k in range(g0, g0 + int(TPG))) {
                s += src[k]
            }
            want[i] = s
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, fntg_msl, fntg_msl_entry, fntg_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, src)
                var bo = buf_fill(dev, N, -1000.0)
                var bufs <- [ba, bo]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr, fntg_msl_tgmem)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = 0
                    var go <- buf_download(bo, N, type<float>)
                    for (i in range(N)) {
                        if (go[i] != want[i]) {
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
            feint("das_metal is not built on this platform; the MSL text twin covers the emission\n")
        }
    }
}

[test]
def test_pointer_param_gpu(t : T?) {
    t |> run("device + threadgroup pointer parameters: advancing dot and slab max match") <| @(t : T?) {
        let W = N * 4
        var pa : array<float>
        var pb : array<float>
        pa |> resize(W)
        pb |> resize(W)
        for (k in range(W)) {
            pa[k] = float(k % 9)
            pb[k] = float(k % 5 + 1)
        }
        // the group-wide slab has no width-1 CPU replay (a barrier means nothing at width 1),
        // so the expectation is computed directly: per-lane dot over its own 4 elements, plus
        // the max of pa over the lane's whole threadgroup
        var want : array<float>
        want |> resize(N)
        for (i in range(N)) {
            var d = 0.0
            for (k in range(i * 4, i * 4 + 4)) {
                d += pa[k] * pb[k]
            }
            let g0 = (i / int(TPG)) * int(TPG)
            var m = -1.0e30
            for (k in range(g0, g0 + int(TPG))) {
                m = max(m, pa[k])
            }
            want[i] = d + m
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, fnptr_msl, fnptr_msl_entry, fnptr_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, pa)
                var bb = buf_upload(dev, pb)
                var bc = buf_fill(dev, N, -1000.0)
                var bufs <- [ba, bb, bc]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr, fnptr_msl_tgmem)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = 0
                    var gc <- buf_download(bc, N, type<float>)
                    for (i in range(N)) {
                        if (gc[i] != want[i]) {
                            gpu_bad++
                        }
                    }
                    delete gc
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bb)
                metal_release(bc)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; the MSL text twin covers the emission\n")
        }
    }
}

[test]
def test_class_gpu_vs_cpu(t : T?) {
    t |> run("class methods + inheritance: devirtualized struct on GPU == CPU vtable run") <| @(t : T?) {
        var oracle = new FnKid()
        oracle.fa |> resize(N)
        oracle.fb |> resize(N)
        oracle.fc |> resize(N)
        oracle.gain = 3.0
        for (i in range(N)) {
            oracle.fa[i] = float(i % 16)
            oracle.fb[i] = float(i % 8)
        }
        for (i in range(N)) {
            gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
            oracle->fncls()
        }
        // i=9: pick = 9+1 = 10 (the OVERRIDE); combo = 30; fk_mix = 18+1 = 19; fc = 49
        t |> equal(oracle.fc[9], 49.0)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, fncls_msl, fncls_msl_entry, fncls_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, oracle.fa)
                var bc = buf_fill(dev, N, -1000.0)
                var bg = buf_fill(dev, 1, 3.0)
                var bb = buf_upload(dev, oracle.fb)
                var bufs <- [ba, bc, bg, bb]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = 0
                    var gc <- buf_download(bc, N, type<float>)
                    for (i in range(N)) {
                        if (gc[i] != oracle.fc[i]) {
                            gpu_bad++
                        }
                    }
                    delete gc
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bc)
                metal_release(bg)
                metal_release(bb)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; CPU oracle asserted above\n")
        }
        unsafe {
            delete oracle
        }
    }
}
