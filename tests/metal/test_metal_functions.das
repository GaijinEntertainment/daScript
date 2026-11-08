options gen2

require dastest/testing_boost public
require metal/msl_shader
require ?das_metal metal/das_metal_boost
require _metal_common  // nolint:STYLE030,LINT019 - used in the das_metal static_if half
require math

// Phase 0 behavioral gate (user-function lowering): the kernel body
// is a chain of ordinary das functions — nested calls, a
// hidden-builtin reader, a thread-ref accumulator, generic twin
// instances (float + int), and write-through var array params. The
// CPU run of the same method is the oracle; the GPU result must match
// exactly (integer-valued floats). The text-level twin (signatures,
// address spaces, prototypes) is tests/msl/test_msl_functions.das.
let N = 256
let TPG = 64u

def private fk_lane : uint => gl_GlobalInvocationID.x

def private fk_mix(x, y : float) : float => x * 2.0 + y

def private fk_pick(src : array<auto(TT)>; i : uint) : TT => src[i] + src[i]

def private fk_accum(var acc : float&; v : float) {
    acc += v
}

def private fk_store(var dst : array<float>; i : uint; v : float) {
    dst[i] = v
}

// the staged shape: a helper fills the caller's @workgroup slab,
// rendezvouses inside, and a second helper reads it back const — what
// dasLLAMA's attention softmax halves do
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

// pointer parameters: `T?` names no address space in das, so the
// parameter declares it (unmarked = device, `@threadgroup` opts in)
// and the emitter checks it against the provenance of every pointer
// handed over. This is the split-K attention shape — advance, don't
// index.
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

// The threadgroup-slab scenario gets its own kernel: a group-wide
// slab does not replay through the sequential CPU oracle (a barrier
// has no width-1 meaning), so its expectation is computed directly —
// lane i sums its OWN group's inputs, in the helper's ascending
// order.
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

[test(description = "A chain of user functions emitted to MSL gives on the GPU the same results as the CPU run of the same method.")]
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
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, fnkern_msl, fnkern_msl_entry,
                                               fnkern_msl_fastmath, perr)
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

[test(description = "A writable threadgroup-slab parameter with a barrier inside a helper gives the right group sums.")]
def test_threadgroup_param_gpu(t : T?) {
    t |> run("writable threadgroup-slab parameter + barrier inside a helper: group sums match") <| @(t : T?) {
        var src : array<float>
        src |> resize(N)
        for (i in range(N)) {
            src[i] = float(i % 5 + 1)
        }
        // lane i sums its own group's inputs, summed ascending — the helper's
        // exact order
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
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, fntg_msl, fntg_msl_entry, fntg_msl_fastmath,
                                               perr)
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

[test(description = "Device and threadgroup pointer parameters give the right advancing dot product and slab maximum.")]
def test_pointer_param_gpu(t : T?) {
    t |> run("device + threadgroup pointer parameters: advancing dot and slab max match") <| @(t : T?) {
        let W = N * 4
        var pa, pb : array<float>
        pa |> resize(W)
        pb |> resize(W)
        for (k in range(W)) {
            pa[k] = float(k % 9)
            pb[k] = float(k % 5 + 1)
        }
        // the group-wide slab has no width-1 CPU replay (a barrier means
        // nothing at width 1), so the expectation is computed directly:
        // per-lane dot over its own 4 elements, plus the max of pa over the
        // lane's whole threadgroup
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
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, fnptr_msl, fnptr_msl_entry, fnptr_msl_fastmath,
                                               perr)
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

[test(description = "Class methods with inheritance devirtualized on the GPU equal the CPU vtable run.")]
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
        // i=9: pick = 9+1 = 10 (the OVERRIDE); combo = 30; fk_mix = 18+1 =
        // 19; fc = 49
        t |> equal(oracle.fc[9], 49.0)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, fncls_msl, fncls_msl_entry, fncls_msl_fastmath,
                                               perr)
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

// Behavioral gate for the kargs form: a @uniform member whose type is
// a STRUCT costs ONE bind for every scalar it carries. The whole
// feature rests on the das struct and the MSL struct being the same
// bytes, and only a GPU run can prove that — a layout slip reads the
// fields shifted and still compiles on both sides. The CPU run of the
// same method is the oracle. The text-level twin (struct definition,
// one constant& parameter) is tests/msl/.

struct UkArgs {
    n : uint
    gain : float
    bias : int
}

class UkKern {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 ys : array<float>
    @uniform @binding = 2 ka : UkArgs

    [metal_kernel(name="ukk_msl"), marker(no_coverage)]
    def ukk {
        let i = gl_GlobalInvocationID.x
        if (i >= ka.n) {
            return
        }
        // every field is read, and each one differently — a shifted layout
        // cannot pass all three
        ys[i] = xs[i] * ka.gain + float(ka.bias)
    }
}

[test(description = "A kargs uniform struct passed to the GPU gives the same results as the CPU run of the same method.")]
def test_uniform_struct_gpu_vs_cpu(t : T?) {
    t |> run("kargs struct on GPU == CPU-reference run of the same method") <| @(t : T?) {
        var oracle = new UkKern()
        oracle.xs |> resize(N)
        oracle.ys |> resize(N)
        // n < N proves the guard reads n
        oracle.ka = UkArgs(n = uint(N) - 17u, gain = 3.0, bias = -5)
        for (i in range(N)) {
            oracle.xs[i] = float(i % 23)
            oracle.ys[i] = -1000.0
        }
        for (i in range(N)) {
            gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
            oracle->ukk()
        }
        t |> equal(oracle.ys[9], float(9 % 23) * 3.0 - 5.0)
        t |> equal(oracle.ys[N - 1], -1000.0)      // past n: untouched
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, ukk_msl, ukk_msl_entry, ukk_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bx = buf_upload(dev, oracle.xs)
                var by = buf_fill(dev, N, -1000.0)
                var bufs <- [bx, by]
                var ka = UkArgs(n = uint(N) - 17u, gain = 3.0, bias = -5)
                var rerr : string
                let ran = run_compute_1d_kargs(queue, pso, bufs, ka, 2, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d_kargs: {rerr}")
                if (ran) {
                    gpu_bad = 0
                    var gy <- buf_download(by, N, type<float>)
                    for (i in range(N)) {
                        if (gy[i] != oracle.ys[i]) {
                            gpu_bad++
                        }
                    }
                    delete gy
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bx)
                metal_release(by)
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

// Behavioral gate for STATE members: a plain (unannotated) member on
// a [metal_kernel] class is a kernel-entry thread-local, so every GPU
// thread starts from the declared initializer — scalars from their
// constant init, fixed arrays zero, carried pointers unbound. That
// lifetime is the contract, and the CPU oracle has to be written to
// match it: a FRESH instance per simulated lane. One instance looped
// across lanes (the shape the rest of this suite uses, which is
// correct for stateless kernels) would carry lane i-1's final boost
// and tally into lane i, so it disagrees with the GPU from lane 1
// onward. The text-level twin (the thread-local declarations) is
// tests/msl/.
// per-lane internal loop length; the kernel spells it as the literal
// 4
let STEP = 4
let W = N * STEP    // input width: STEP elements per lane

class StKern {
    @ssbo @binding = 0 sa : array<float>
    @ssbo @binding = 1 so : array<float>
    cur : float const?      // carried-pointer state: bound once per lane, advanced per step
    // scalar state with a constant initializer — read AND written per
    // step
    boost : float = 0.25
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

// One simulated lane, on its OWN instance — the per-thread state
// lifetime made executable. The declared initializers come from the
// class constructor, so the oracle never restates 0.25 here.
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

// Closed form, independent of both runs: lane i walks its own STEP
// inputs and weights step j by the boost its OWN state has reached
// (0.25 + 0.5 * j), starting from a zero tally.
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

[test(description = "Per-thread state members on the GPU equal a fresh CPU instance per lane.")]
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
        // this compare is the lifetime gate on every platform: a lane that
        // inherited the previous lane's boost and tally lands nowhere near
        // the closed form (lane 1: 77.0 instead of 24.5)
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
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, stkern_msl, stkern_msl_entry,
                                               stkern_msl_fastmath, perr)
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

// Real-GPU behavioral gate for the hoisted constant tables: a `let`
// fixed-array local of literals lowers to a program-scope `constant`
// array (renamed when a helper reuses the name with different
// content), and the kernel's reads through it must return the staged
// values. The CPU run of the same body is the oracle; the GPU half is
// static_if-gated on das_metal and skips without a device.

def private ct_pick(i : uint) : uint {
    let tbl = fixed_array<uint>(0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x66u, 0x77u, 0x88u)
    return tbl[i & 7u]
}

// its own table carries the name the rename would generate
def private ct_pick0(i : uint) : uint {
    let tbl_c0 = fixed_array<uint>(0x100u, 0x200u, 0x300u, 0x400u, 0x500u, 0x600u, 0x700u, 0x800u)
    return tbl_c0[i & 7u]
}

// the same local name with different content: the emitter renames the
// hoisted twin
def private ct_pick2(i : uint) : uint {
    let tbl = fixed_array<uint>(0x1u, 0x2u, 0x4u, 0x8u, 0x10u, 0x20u, 0x40u, 0x80u)
    return tbl[i & 7u]
}

class ConstTabGpu {
    @ssbo @binding = 0 src : array<uint>
    @ssbo @binding = 1 dst : array<uint>
    @ssbo @binding = 2 fdst : array<float>

    [metal_kernel(name="consttab_gpu_msl"), marker(no_coverage)]
    def consttab {
        let i = gl_GlobalInvocationID.x
        let sgn = fixed_array<float>(1.0, 0.5)
        dst[i] = src[i] + ct_pick0(i) + ct_pick(i) + ct_pick2(i)
        fdst[i] = sgn[i & 1u] * 2.0
    }
}

[test(description = "Hoisted constant tables give the same results on the GPU as the CPU reference.")]
def test_consttab_gpu_vs_cpu(t : T?) {
    t |> run("hoisted constant tables: GPU == CPU-reference") <| @(t : T?) {
        var oracle = new ConstTabGpu()
        oracle.src |> resize(N)
        oracle.dst |> resize(N)
        oracle.fdst |> resize(N)
        for (k in range(N)) {
            oracle.src[k] = uint(k * 3)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->consttab()
        }
        // the oracle itself: src 3, the helper's own tbl_c0[1], tbl[1], the
        // renamed tbl_c1[1]
        t |> equal(oracle.dst[1], 3u + 0x200u + 0x22u + 0x2u)
        t |> equal(oracle.fdst[1], 1.0)                 // sgn[1] * 2
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, consttab_gpu_msl, consttab_gpu_msl_entry,
                                               consttab_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bsrc = buf_upload(dev, oracle.src)
                var bdst = buf_fill(dev, N, 0u)
                var bfdst = buf_fill(dev, N, 0.0)
                var bufs <- [bsrc, bdst, bfdst]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_exact(bdst, oracle.dst) + buf_mismatch_exact(bfdst, oracle.fdst)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bsrc)
                metal_release(bdst)
                metal_release(bfdst)
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

[export]
def main {
    return
}
