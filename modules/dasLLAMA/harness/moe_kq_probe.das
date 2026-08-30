options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require llvm/daslib/llvm_tune
require ../performance/profile_common.das   // tune_gate — the standing pre-measure stack
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_gen
require dasllama/dasllama_convert
require dasllama/dasllama_repack
require dasllama/dasllama_gemm_schema
require daslib/jobque_boost
require llvm/daslib/f16_cvt
require math

// ATTRIBUTION SWEEP: attributes the MoE CPU pp gap across kernel shape (dense vs narrow D),
// per-expert M, the groupn walk, routing skew, and the 32-row sub-region split - the cells
// behind ARCHITECTURE_RUNTIME.md's moe-region-split numbers (620 vs 3929 GFLOP/s on zipf k4).
// The MoE k4 batch-GEMM discriminator, four cells at equal weight footprint (~127 MB each side):
//   A dense-flat:   32 sets  d=2816, M=512  (the dense pp regime)
//   B narrow-flat: 128 sets  d= 704, M=512  (A->B isolates the narrow expert D)
//   C narrow-flat: 128 sets  d= 704, M= 32  (B->C isolates the small per-expert M)
//   D groupn:      128 regions, cnt=32      (C->D isolates the groupn walk vs flat dispatch)
let N = 2816l
let DE = 704l
let DD = 2816l
let NREG = 128l
let CNT = 32l
let MB = 512l
let NSETS_D = 32l
let REPS = 5

let SYNTH_SC <- [63, 21, 37, 5, 48, 17, 60, 33]
let SYNTH_MN <- [1, 62, 19, 44, 7, 58, 25, 36]
let SYNTH_D = 0.5
let SYNTH_DMIN = 0.25

def private q4_pat(k : int) : int => (k * 7 + 3) % 16

def private build_q4k_block() : array<uint8> {
    var blkb : array<uint8>
    blkb |> resize(144)
    let dbits = f32_to_f16(SYNTH_D)
    let mbits = f32_to_f16(SYNTH_DMIN)
    blkb[0] = uint8(dbits & 0xFF)
    blkb[1] = uint8(dbits >> 8u)
    blkb[2] = uint8(mbits & 0xFF)
    blkb[3] = uint8(mbits >> 8u)
    for (j in range(4)) {
        blkb[4 + j] = uint8((SYNTH_SC[j] & 63) | (((SYNTH_SC[j + 4] >> 4) & 3) << 6))
        blkb[8 + j] = uint8((SYNTH_MN[j] & 63) | (((SYNTH_MN[j + 4] >> 4) & 3) << 6))
        blkb[12 + j] = uint8((SYNTH_SC[j + 4] & 15) | ((SYNTH_MN[j + 4] & 15) << 4))
    }
    for (js in range(4)) {
        for (l in range(32)) {
            blkb[16 + 32 * js + l] = uint8(q4_pat(64 * js + l) | (q4_pat(64 * js + 32 + l) << 4))
        }
    }
    return <- blkb
}

// per-region planes: transcode the synthetic superblock everywhere, xor-noise the quants,
// then the per-region grp repack (the per-expert-slice shape the load path produces)
def private build_pool(nregions, d : int64; var kq : array<uint8>; var ks : array<uint8>) {
    let nsb = N / 256l
    let qsb = kq_qsb(4)
    let ssb = kq_ssb(4)
    let mr = kq_layout_of(4)
    let blk1 <- build_q4k_block()
    kq |> reserve(nregions * d * nsb * qsb)
    kq |> resize(nregions * d * nsb * qsb)
    ks |> reserve(nregions * d * nsb * ssb)
    ks |> resize(nregions * d * nsb * ssb)
    for (r in range64(nregions * d)) {
        for (s in range64(nsb)) {
            transcode_q4k_superblock(blk1, 0l, kq, (r * nsb + s) * qsb, ks, (r * nsb + s) * ssb)
        }
    }
    var st = 0x7F4A7C15u
    for (c in kq) {
        st = st * 1664525u + 1013904223u
        c = uint8(uint(c) ^ (st >> 16u))
    }
    unsafe {
        for (r in range64(nregions)) {
            repack_k4_grp(addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), N, d, mr)
        }
    }
}

def private time_cell(name : string; gflop : double; blk : block<() : void>) {
    invoke(blk)   // warmup
    var best = 1e30lf
    var sum = 0.0lf
    for (_rep in range(REPS)) {
        let t0 = ref_time_ticks()
        invoke(blk)
        let ms = double(get_time_usec(t0)) / 1000.0lf
        sum += ms
        best = min(best, ms)
    }
    let mean = sum / double(REPS)
    print("{name}: mean {mean} ms  best {best} ms  {gflop / (mean / 1000.0lf)} GFLOP/s (best {gflop / (best / 1000.0lf)})\n")
}

[export]
def main {   // nolint:STYLE038 — one flat cell ladder A->G; the cells share pools and read in sequence
    with_job_que() {
        setup_dasllama_jobque_()
        pin_kernel_backend("arm64-gen")
        if (!kernel_backend_has_kq_batch() || !kernel_backend_has_kq_batch_groupn()) {
            panic("moe_kq_probe: no kq batch slots on this backend - zero result rows")
            return
        }
        if (!tune_gate()) {
            panic("moe_kq_probe: tune_gate refused - tune first (or point DAS_TUNE_MANIFEST at a complete sidecar)")
        }
        print("lanes {get_dispatch_lanes()}  backend {active_kernel_backend()}\n")
        var kqE : array<uint8>
        var ksE : array<uint8>
        var kqD : array<uint8>
        var ksD : array<uint8>
        build_pool(NREG, DE, kqE, ksE)
        build_pool(NSETS_D, DD, kqD, ksD)
        print("pools built: expert {long_length(kqE) / 1048576l} MB  dense {long_length(kqD) / 1048576l} MB\n")

        let nk = NREG * CNT
        var src : array<float>
        src |> resize(nk * N)
        for (i in range64(nk * N)) {
            src[i] = 0.8 * sin(float(i) * 0.29) - 0.4 * cos(float(i) * 0.057)
        }
        var xq : array<int8>
        var xs : array<float>
        var xbs : array<int>
        xq |> resize(nk * N)
        xs |> resize(nk * N / 256l)
        xbs |> resize(nk * N / 16l)
        unsafe {
            for (p in range64(nk)) {
                quantize_q8_k_into_ptr(addr(src[p * N]), N, addr(xq[0]), addr(xs[0]), addr(xbs[0]), p * N, p * (N / 256l), p * (N / 16l))
            }
        }
        var y : array<float>
        y |> resize(nk * DD)

        let gflopBig = double(NSETS_D) * 2.0lf * double(DD * N * MB) / 1e9lf
        let gflopSmall = double(NREG) * 2.0lf * double(DE * N * CNT) / 1e9lf

        time_cell("A dense-flat  d=2816 M=512 x32 ", gflopBig) $ {
            for (s in range64(NSETS_D)) {
                matmul_kq_batch(4, y, kqD, ksD, s * DD * N, xq, xs, xbs, N, DD, MB)
            }
        }
        time_cell("B narrow-flat d= 704 M=512 x128", gflopBig) $ {
            for (r in range64(NREG)) {
                matmul_kq_batch(4, y, kqE, ksE, r * DE * N, xq, xs, xbs, N, DE, MB)
            }
        }
        time_cell("C narrow-flat d= 704 M= 32 x128", gflopSmall) $ {
            for (r in range64(NREG)) {
                matmul_kq_batch(4, y, kqE, ksE, r * DE * N, xq, xs, xbs, N, DE, CNT)
            }
        }
        var offs : array<int64>
        offs |> reserve(NREG * 3l)
        for (r in range64(NREG)) {
            offs |> push(r * DE * N)
            offs |> push(r * CNT)
            offs |> push(CNT)
        }
        time_cell("D groupn      d= 704 cnt=32x128", gflopSmall) $ {
            matmul_kq_batch_groupn(4, y, kqE, ksE, offs, NREG, xq, xs, xbs, N, DE)
        }
        // E: the same total rows under a zipf routing skew (cnt_r ~ 1/(r+1)) — real top-8
        // routing is imbalanced, and groupn chunks cost units by count, not by rows
        var cnts : array<int64>
        cnts |> reserve(NREG)
        var acc = 0l
        for (r in range64(NREG)) {
            let c = max(nk / ((r + 1l) * 6l), 1l)
            cnts |> push(c)
            acc += c
        }
        cnts[0] += nk - acc   // remainder onto the heaviest expert (the skew's point)
        var offsE : array<int64>
        offsE |> reserve(NREG * 3l)
        var row0 = 0l
        var cmax = 0l
        for (r in range64(NREG)) {
            offsE |> push(r * DE * N)
            offsE |> push(row0)
            offsE |> push(cnts[r])
            row0 += cnts[r]
            cmax = max(cmax, cnts[r])
        }
        print("skew: rows {row0} of {nk}, cnt max {cmax} min {cnts[NREG - 1l]}\n")
        time_cell("E groupn-skew d= 704 zipf x128  ", gflopSmall) $ {
            matmul_kq_batch_groupn(4, y, kqE, ksE, offsE, NREG, xq, xs, xbs, N, DE)
        }
        time_cell("F flat-skew   d= 704 zipf x128  ", gflopSmall) $ {
            for (r in range64(NREG)) {
                matmul_kq_batch(4, y, kqE, ksE, r * DE * N, xq, xs, xbs, N, DE, cnts[r])
            }
        }
        // G: the fix shape — heavy regions pre-split into <=32-row sub-regions so groupn's
        // count-chunking sees equal-cost units
        var offsG : array<int64>
        offsG |> reserve(NREG * 3l * 8l)
        var nregG = 0l
        var rowG = 0l
        for (r in range64(NREG)) {
            var left = cnts[r]
            while (left > 0l) {
                let c = min(left, 32l)
                offsG |> push(r * DE * N)
                offsG |> push(rowG)
                offsG |> push(c)
                rowG += c
                left -= c
                nregG++
            }
        }
        print("split: {nregG} sub-regions\n")
        time_cell("G groupn-split d=704 zipf/32    ", gflopSmall) $ {
            matmul_kq_batch_groupn(4, y, kqE, ksE, offsG, nregG, xq, xs, xbs, N, DE)
        }
        delete offsG
        delete offsE
        delete cnts
        delete offs
        delete y
        delete xq
        delete xs
        delete xbs
        delete src
        delete kqE
        delete ksE
        delete kqD
        delete ksD
    }
}
