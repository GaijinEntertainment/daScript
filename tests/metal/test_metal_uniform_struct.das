options gen2
options indenting = 4

// Behavioral gate for the kargs form: a @uniform member whose type is a STRUCT costs ONE bind
// for every scalar it carries. The whole feature rests on the das struct and the MSL struct
// being the same bytes, and only a GPU run can prove that — a layout slip reads the fields
// shifted and still compiles on both sides. The CPU run of the same method is the oracle.
// The text-level twin (struct definition, one constant& parameter) is tests/msl/.

require dastest/testing_boost public
require metal/msl_shader
require ?das_metal metal/das_metal_boost
require _metal_common   // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half

let N = 256
let TPG = 64u

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
        // every field is read, and each one differently — a shifted layout cannot pass all three
        ys[i] = xs[i] * ka.gain + float(ka.bias)
    }
}

[test]
def test_uniform_struct_gpu_vs_cpu(t : T?) {
    t |> run("kargs struct on GPU == CPU-reference run of the same method") <| @(t : T?) {
        var oracle = new UkKern()
        oracle.xs |> resize(N)
        oracle.ys |> resize(N)
        oracle.ka = UkArgs(n = uint(N) - 17u, gain = 3.0, bias = -5)   // n < N proves the guard reads n
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
