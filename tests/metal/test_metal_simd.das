options gen2
options indenting = 4

// Real-GPU behavioral gate for Phase-3 simdgroup intrinsics. Simd ops have no sequential CPU
// replay (the identity stubs are width-1 semantics), so the oracle is READBACK-DRIVEN: the kernel
// reports each thread's simdgroup width / lane / simdgroup id, and the CPU recomputes every
// expected value from those reported memberships — layout-agnostic (no assumption that lanes map
// to consecutive thread indices), uint bit-exact, float sums under the fastmath tolerance.
// Shuffles run in simdgroup-uniform flow; the undefined edge lanes of up/down are selected away
// on the VALUE (a shuffle inside divergent flow would read inactive lanes — undefined).

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

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

// the thread in [tg0, tg1) sitting on `want_lane` of simdgroup `want_sg`, or -1
def private find_lane(tg0, tg1 : int; sgs, lanes : array<uint>; want_sg, want_lane : uint) : int {
    for (j in range(tg0, tg1)) {
        if (sgs[j] == want_sg && lanes[j] == want_lane) {
            return j
        }
    }
    return -1
}

[test]
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
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, simd_gpu_msl, simd_gpu_msl_entry, simd_gpu_msl_fastmath, perr)
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
