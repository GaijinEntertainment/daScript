options gen2
options indenting = 4

// Real-GPU behavioral gate for the hoisted constant tables: a `let` fixed-array local of literals
// lowers to a program-scope `constant` array (renamed when a helper reuses the name with different
// content), and the kernel's reads through it must return the staged values. The CPU run of the
// same body is the oracle; the GPU half is static_if-gated on das_metal and skips without a device.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common   // nolint:STYLE030 — used inside the das_metal static_if arm only

let N = 256
let TPG = 64u

def private ct_pick(i : uint) : uint {
    let tbl = fixed_array<uint>(0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x66u, 0x77u, 0x88u)
    return tbl[i & 7u]
}

def private ct_pick0(i : uint) : uint {   // its own table carries the name the rename would generate
    let tbl_c0 = fixed_array<uint>(0x100u, 0x200u, 0x300u, 0x400u, 0x500u, 0x600u, 0x700u, 0x800u)
    return tbl_c0[i & 7u]
}

// the same local name with different content: the emitter renames the hoisted twin
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

[test]
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
        t |> equal(oracle.dst[1], 3u + 0x200u + 0x22u + 0x2u)   // the oracle itself: src 3, the helper's own tbl_c0[1], tbl[1], the renamed tbl_c1[1]
        t |> equal(oracle.fdst[1], 1.0)                 // sgn[1] * 2
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, consttab_gpu_msl, consttab_gpu_msl_entry, consttab_gpu_msl_fastmath, perr)
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
