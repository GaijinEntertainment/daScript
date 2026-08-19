options gen2
options indenting = 4

// Real-GPU behavioral gate vs the CPU-reference oracle for the Phase-2 control-flow kernel:
// mutable/zero-init locals, compound assignment, ++/-- (statement + value postfix), every
// comparison flavor per scalar class, logical ops, if/elif/else, ternary. Fills are chosen so
// every branch arm executes for some thread. uint/int outputs compare bit-exact; float with
// fastmath tolerance.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common

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

[test]
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
        // k=0: s==z (both 0) -> facc = ((1 *= 2) += 8) += 16 = 26 — the oracle itself computed
        t |> success(fapprox(oracle.out3[0], 26.0), "float branch anchor (k=0 -> 26.0)")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, control_gpu_msl, control_gpu_msl_entry, control_gpu_msl_fastmath, perr)
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
