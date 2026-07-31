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
