options gen2
options persistent_heap
options _dasllama_internal = true

// SHORT-WINDOW GEMM RACE - the q8 prefill tile at 9 to 128 rows, where the 32-row tile's grid is
// mp/32 x d/64 threadgroups and a 0.6B layer's sites launch 16 to 48 of them on a 40-core GPU.
// The baseline arm is the shipped double-buffered 32-row stamp (`MetalQ8MulMmTHDb`) over the padded
// rows; every other arm is the shipped split-K stamp (`MetalQ8MulMmTHSk`, 32 output columns a
// threadgroup) at a K-split count `skN`, the partial planes summed by `MetalSkReduce`. Each step issues two independent sites
// on a concurrent encoder, as the window issues a layer's co-running sites (`knee_pair_ms`), over a ring of weight copies past
// the SLC (`knee_ring`), alternating outputs; the times are best of nine rounds (burned first), in microseconds a GEMM. The
// compare region is every live output element, held against the baseline arm's output (f16 staging: bit-exact or within the
// race envelope).
//
//   bin/daslang -jit modules/dasLLAMA/harness/short_window_gemm_race.das [-- wide]

require daslib/strings_boost
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_prefill
require dasllama/dasllama_metal_kernels   // MetalSkReduce, the knee races' scaffold (knee_pair_ms, knee_ring, knee_pso)
require dasllama/dasllama_kqformat   // KqFmt.q8: the ring's planes
require _race_fixture


struct SkArm {
    name : string
    nsk : int
}



//! microseconds a GEMM from a chain's GPU ms: `KNEE_CHAIN` steps of two GEMMs
def private gemm_us(ms : double) : double => ms * 1000.0lf / double(KNEE_CHAIN * 2)

[export]
def main() : int {   // nolint:STYLE038 — one-shot race harness; buffers + PSOs stay live to the release tail
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var p_base = race_stems(knee_pso, dev, MetalQ8MulMmTHDb_metal_q8_mulmm_t_msl, err)
        var p_sk32 = race_stems(knee_pso, dev, MetalQ8MulMmTHSk_metal_q8_mulmm_t_msl, err)
        var p_red = race_stems(knee_pso, dev, metal_sk_reduce_msl, err)
        if (p_base == null || p_sk32 == null || p_red == null) {
            print("short_window_gemm_race: pso - {err}\n")
            return 1
        }
        //! `-- wide`: the one-way 32-column stamp against the tile past the split-K knee, on the wide sites (row 58's grid)
        let wide = has_value(get_command_line_arguments(), "wide")
        let shapes <- (wide ? [(k = 4096l, n = 4096l), (k = 4096l, n = 14336l), (k = 14336l, n = 4096l), (k = 5120l, n = 17408l),
                (k = 2048l, n = 8192l), (k = 1024l, n = 3072l), (k = 3072l, n = 8192l)]
            : [(k = 1024l, n = 1024l), (k = 1024l, n = 2048l), (k = 1024l, n = 3072l), (k = 3072l, n = 1024l),
                (k = 4096l, n = 4096l), (k = 4096l, n = 14336l)])
        let rows_set <- wide ? [32, 64, 96, 128, 160, 192, 256] : [9, 16, 25, 33, 48, 64, 96, 128]
        let arms <- wide ? [SkArm(name = "sk1", nsk = 1)] : [SkArm(name = "sk1", nsk = 1), SkArm(name = "sk2", nsk = 2), SkArm(name = "sk4", nsk = 4), SkArm(name = "sk8", nsk = 8)]
        var tally : RaceTally
        for (sh in shapes) {
            var ring <- knee_ring(dev, KqFmt.q8, sh.n, sh.k)
            let mpmax = 256l
            var xa <- race_x_f16(int(mpmax * sh.k))
            var bx = metal_buffer_from(dev, uint64(mpmax * sh.k * 2l), unsafe(addr<void?>(xa[0])))
            let ybytes = uint64(mpmax * sh.n * 4l)
            var ys <- [for (_i in range(4)); metal_buffer_from(dev, ybytes, null)]   //! the base arm's outputs, a pair a step, alternating
            var yt <- [for (_i in range(4)); metal_buffer_from(dev, ybytes, null)]   //! the split arm's
            var parts <- [for (_i in range(2)); metal_buffer_from(dev, ybytes * 8ul, null)]
            var bk = race_uniform_u32(dev, uint(sh.k))
            var bn = race_uniform_u32(dev, uint(sh.n))
            for (rows in rows_set) {
                let mp = int64((rows + 31) / 32 * 32)
                var cells : array<string>
                var base_best = -1.0lf
                for (a in arms) {
                    var ka = SkReduceArgs(total = uint(mp * sh.n / 4l), ksplit = uint(a.nsk))
                    let pr = knee_pair_ms(queue, $(enc; i) {
                        for (j in range(2)) {
                            let w = ring[(2 * i + j) % length(ring)].bq
                            enc_gemm_mm_thdb_c_pso(enc, p_base, w, 0ul, w, bx, 0ul, ys[(i & 1) * 2 + j], 0ul, bk, bn, mp, sh.n)
                        }
                    }, $(enc; i) {
                        for (j in range(2)) {
                            let w = ring[(2 * i + j) % length(ring)].bq
                            let out = a.nsk == 1 ? yt[(i & 1) * 2 + j] : parts[j]
                            enc_gemm_mm_thsk_c_pso(enc, p_sk32, w, 0ul, w, bx, 0ul, out, 0ul, bk, bn, mp, sh.n, int64(a.nsk))
                        }
                        if (a.nsk > 1) {
                            metal_memory_barrier(enc)
                            for (j in range(2)) {
                                enc_sk_reduce_c_pso(enc, p_red, parts[j], yt[(i & 1) * 2 + j], 0ul, ka, mp * sh.n / 4l)
                            }
                        }
                    })
                    var note = ""
                    let last = ((KNEE_CHAIN - 1) & 1) * 2 + 1   //! the pair the chain's last step wrote: its second site
                    let ok = pr.b > 0.0lf && pr.t > 0.0lf && race_envelope_ok(ys[last], yt[last], rows * int(sh.n), note)
                    if (!ok) {
                        print("  {a.name}: REFUSED ({note})\n")
                        tally |> tally(-1)
                        continue
                    }
                    if (base_best < 0.0lf || pr.b < base_best) {
                        base_best = pr.b
                    }
                    cells |> push("{a.name}={fmt(":.1f", gemm_us(pr.t))}")
                    tally |> tally(1)
                }
                print("k={sh.k} n={sh.n} rows={rows} (mp {mp}, ring {length(ring)}): {join(cells, " ")} base={fmt(":.1f", gemm_us(base_best))} us/GEMM\n")
                delete cells
            }
            knee_ring_release(ring)
            for (b in [bx, bk, bn]) {
                metal_release(b)
            }
            for (b in ys) {
                metal_release(b)
            }
            for (b in yt) {
                metal_release(b)
            }
            for (b in parts) {
                metal_release(b)
            }
            delete xa
        }
        for (p in [p_base, p_sk32, p_red]) {
            metal_release(p)
        }
        return race_exit(tally, "short_window_gemm_race")
    } else {
        print("short_window_gemm_race: no das_metal in this build\n")
        return 1
    }
}
