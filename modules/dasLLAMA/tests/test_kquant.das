//! The K-quant and grid formats: plane dequants, dots, the grp repack, the bf16 panel, tiles and GEMVs against in-test references, and the model-gated load cells. Skips: an interpreted run and a missing stocked model (the model cells), no gen backend registered (batch groupn), a backend with no kq slots (the repacked-plane serve cell), and a format whose stamp is a bf16 tile (the 4-token tile cells).
options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // the layout arm loads a multi-GB model; explicit delete below
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemm_schema   // vbmi_pack_word / vbmi_alphabet / pm1_of - the lattice's pure table builders
require dasllama/dasllama_layout   // moe_gpu_gather_stack_kq - the device-plane gather oracle
require _model_tier
require _kq_fixtures   // the synthetic superblocks and plane regions; dot_kq and the kernel stamps on the run-time format
require dasllama/dasllama_gguf
require dasllama/dasllama_common   // dequant_kq_plane_sb - the carrier-form dispatcher
require dasllama/dasllama_convert
require dasllama/dasllama_math   // with_kernel_backend - the layout arm needs disk-order planes
require dasllama/dasllama_math_default   // dot_kq on the format tag — the kernel arms gate it vs the plane dequant
require dasllama/dasllama_math_gen   // dequant_kq_row_grp, kq_groupn_gen, the bf16 panels
require dasllama/dasllama_repack     // repack_kq_grp
require dasllama/dasllama_gemm_schema   // kq_qsb/kq_ssb — the fmt-id plane strides
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration for load_gguf
require daslib/jobque_boost   // team_parallel_for - the lane-context arm of the iq4nl dequant
require daslib/algorithm   // fill - the NaN sentinel over a reused output
require math

// K-quant (Q4_K / Q6_K) superblock dequant gates. The synthetic arms hand-pack one superblock
// with the on-disk bit layout written in the PACK direction (independent of the reader's unpack),
// so a layout misread fails loudly with no model files needed. The dequant arithmetic itself is
// proven bit-exact against gguf-py's dequantize (harness/q4k_dequant_probe.das); the real-file
// arm cross-checks against the Q8_0 twin of the same weights at known quantization-error levels.
// The repacked-plane cell is JIT-only, needs the large-tier gemma-4-26B_q4_0-it.gguf
// (DASLLAMA_PARITY_FULL=1) and a backend carrying kq slots - each absence is a loud skip.

//! the superblock format ids the kq gates walk
let private KQ_IDS = fixed_array(4, 5, 6, 40, 44, 3, 33, 34, 45, 2, 23, 24, 25, 35)

// the codebook value of element k of synthetic block `blk` (k < 16 low nibble, else high)
def private iq4nl32_w(blk, k : int) : float {
    let q = iq4nl_qs_pat(blk * 16 + (k % 16))
    let nib = k < 16 ? q & 15 : q >> 4
    return iq4nl32_d(blk) * float(int(IQ4NL_LUT[nib]))
}

//! `nw` values of `f` - the element-by-element formula a synthetic superblock's dequant must reproduce
def private formula_row(nw : int; f : block<(k : int) : float>) : array<float> {
    var w : array<float>
    w |> resize(nw)
    for (k in range(nw)) {
        w[k] = invoke(f, k)
    }
    return <- w
}

[test]
def test_q4k_synthetic(t : T?) {
    t |> run("Q4_K superblock unpacks the 6-bit scale/min packing") @(t : T?) {
        let blkb <- build_q4k_block()
        var dst : array<float>
        dst |> resize(256)
        dequant_q4k_superblock(blkb, 0l, dst, 0l)
        let want <- formula_row(256) $(k) {
            let sub = k / 32
            return SYNTH_D * float(SYNTH_SC[sub]) * float(q4_pat(k)) - SYNTH_DMIN * float(SYNTH_MN[sub])
        }
        check(t, "Q4_K elements against the hand-packed values", dst, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("Q5_K superblock unpacks the split high-bit plane") @(t : T?) {
        let blkb <- build_q5k_block()
        var dst : array<float>
        dst |> resize(256)
        dequant_q5k_superblock(blkb, 0l, dst, 0l)
        let want <- formula_row(256) $(k) {
            let sub = k / 32
            return SYNTH_D * float(SYNTH_SC[sub]) * float(q5_pat(k)) - SYNTH_DMIN * float(SYNTH_MN[sub])
        }
        check(t, "Q5_K elements against the hand-packed values", dst, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("Q6_K superblock unpacks the ql/qh split and signed sub-scales") @(t : T?) {
        let blkb <- build_q6k_block()
        var dst : array<float>
        dst |> resize(256)
        dequant_q6k_superblock(blkb, 0l, dst, 0l)
        let want <- formula_row(256) $(k) => 0.25 * float(SYNTH_SCQ[k / 16]) * float(q6_pat(k) - 32)
        check(t, "Q6_K elements against the hand-packed values", dst, want, BAR_EXACT, FLAT_SHAPE)
    }
}

//! One synthetic block's planes as format `kf` writes them and their dequant: a superblock's 256 values, or a per-32
//! block's 32
struct PlaneRow {
    kq : array<uint8>
    ks : array<uint8>
    w : array<float>
}

//! `blkb` transcoded into format `kf`'s plane pair at the format's own strides, and that pair dequanted
def private plane_row(kf : KqFmt; blkb : array<uint8>) : PlaneRow {
    var p : PlaneRow
    p.kq |> resize(kq_qsb(kf))
    p.ks |> resize(kq_ssb(kf))
    kq_transcode_superblock(kf, blkb, 0l, p.kq, 0l, p.ks, 0l)
    let per32 = kf == KqFmt.q51 || kf == KqFmt.iq4nl32
    p.w <- nan_plane(per32 ? 32 : 256)
    if (kf == KqFmt.q51) {
        dequant_q51_plane_block(p.kq, 0l, p.ks, 0l, p.w, 0l)
    } elif (kf == KqFmt.iq4nl32) {
        dequant_iq4nl32_plane_block(p.kq, 0l, p.ks, 0l, p.w, 0l)
    } else {
        dequant_kq_plane_sb(kf, p.kq, p.ks, 0l, false, p.w, 0l)
    }
    return <- p
}

//! the file dequant of `blkb` through `dequant` - a K-quant plane's bit-exact reference
def private file_dequant(blkb : array<uint8>; dequant : block<(b : array<uint8>; var dst : array<float>) : void>) : array<float> {
    var want : array<float>
    want |> resize(256)
    invoke(dequant, blkb, want)
    return <- want
}

[test]
def test_kq_transcode_planes(t : T?) {
    kquant_plane_cells(t)
    grid_plane_cells(t)
    lattice_plane_cells(t)
}

//! the K-quant and q40 / iq4xs / k3 plane cells
def private kquant_plane_cells(t : T?) {
    // The transcoded planes ARE the kernel-facing format, and they are verbatim disk splits for
    // all three formats (kq v2) — every plane dequant must equal the FILE dequant bit-exactly.
    t |> run("k4 planes reproduce the Q4_K dequant bit-exactly") @(t : T?) {
        let blkb <- build_q4k_block()
        var inscope p <- plane_row(KqFmt.k4, blkb)
        let want <- file_dequant(blkb) $(b; var dst) {
            dequant_q4k_superblock(b, 0l, dst, 0l)
        }
        check(t, "k4 plane against the file dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("k5 planes reproduce the Q5_K dequant bit-exactly") @(t : T?) {
        let blkb <- build_q5k_block()
        var inscope p <- plane_row(KqFmt.k5, blkb)
        let want <- file_dequant(blkb) $(b; var dst) {
            dequant_q5k_superblock(b, 0l, dst, 0l)
        }
        check(t, "k5 plane against the file dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("k6 planes reproduce the Q6_K dequant bit-exactly") @(t : T?) {
        let blkb <- build_q6k_block()
        var inscope p <- plane_row(KqFmt.k6, blkb)
        let want <- file_dequant(blkb) $(b; var dst) {
            dequant_q6k_superblock(b, 0l, dst, 0l)
        }
        check(t, "k6 plane against the file dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("q40 planes unpack the hand-packed Q4_0 blocks exactly") @(t : T?) {
        var inscope p <- plane_row(KqFmt.q40, build_q40_block())
        let want <- formula_row(256) $(k) => q40_d(k / 32) * float(q4_pat(k) - 8)
        check(t, "q40 plane against the hand-packed values", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("iq4xs planes decode the 6-bit sub-scales and the codebook exactly") @(t : T?) {
        var inscope p <- plane_row(KqFmt.iq4xs, build_iq4xs_block())
        for (blk in range(8)) {
            t |> equal(iq4xs_sc(p.ks, 0l, int64(blk)), SYNTH_SC[blk] - 32, "decoded sub-scale is ls - 32, signed")
        }
        let want <- formula_row(256) $(k) => (SYNTH_D * float(SYNTH_SC[k / 32] - 32)) * float(IQ4NL_LUT[q4_pat(k)])
        check(t, "iq4xs plane against the upstream dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("k3 planes decode the packed 6-bit sub-scales and compose the 3-bit quants exactly") @(t : T?) {
        var inscope p <- plane_row(KqFmt.k3, build_q3k_block())
        for (i in range(16)) {
            t |> equal(int(int8(p.ks[i])), SYNTH_SC[i % 8] - 32, "decoded sub-scale is scale - 32, signed")
        }
        let want <- formula_row(256) $(k) => (SYNTH_D * float(SYNTH_SC[(k / 16) % 8] - 32)) * float(q3_pat(k) - 4)
        check(t, "k3 plane against the upstream dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
}

//! the iq3s, iq3s4 and k2 plane cells
def private grid_plane_cells(t : T?) {
    t |> run("iq3s planes decode the 4-bit sub-scales and gather the grid exactly") @(t : T?) {
        var inscope p <- plane_row(KqFmt.iq3s, build_iq3s_block())
        for (b in range(8)) {
            t |> equal(int(int8(p.ks[4 + b])), 1 + 2 * (SYNTH_SC[b] & 15), "decoded sub-scale is 1 + 2s")
        }
        let want <- formula_row(256) $(k) {
            let b = k / 32
            let l = (k % 32) / 8
            let hj = k % 8
            let qi = iq3s_qs_pat(b * 8 + 2 * l + hj / 4) | ((iq3s_qh_pat(b) << (hj < 4 ? 8 - 2 * l : 7 - 2 * l)) & 256)
            let g = int((IQ3S_GRID[qi] >> uint(8 * (hj % 4))) & 0xFFu)
            let sgn = ((iq3s_sg_pat(b * 4 + l) >> hj) & 1) != 0 ? -1.0 : 1.0
            return (SYNTH_D * float(1 + 2 * (SYNTH_SC[b] & 15))) * float(g) * sgn
        }
        check(t, "iq3s plane against the upstream dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("iq3s4 planes are the iq3s superblock's exact served twin: 16-value codebook, iq3s's scale row") @(t : T?) {
        var blkb <- build_iq3s_block()
        var st = 0x2545F491u
        for (rep in range(8)) {   // rep 0 = the synthetic block; then LCG-perturbed grid/qh/sign/scale bytes (every IQ3_S byte is legal)
            if (rep > 0) {
                for (i in range(2, 110)) {
                    st = st * 1664525u + 1013904223u
                    blkb[i] = uint8(st >> 24u)
                }
            }
            var inscope p3 <- plane_row(KqFmt.iq3s, blkb)
            var inscope p4 <- plane_row(KqFmt.iq3s4, blkb)
            check_exact(t, "rep {rep}: the iq3s4 scale row against iq3s's", p4.ks, p3.ks)
            check(t, "rep {rep}: iq3s4 weights through the codebook against iq3s's through the grid", p4.w, p3.w, BAR_EXACT, FLAT_SHAPE)
        }
        for (v in [-15, -1, 1, 15]) {
            t |> equal(int(IQ3S4_LUT[iq3s4_index(v)]), v, "the codebook round-trips its own index")
        }
    }
    t |> run("k2 planes reorder the header and compose the 2-bit quants exactly") @(t : T?) {
        var inscope p <- plane_row(KqFmt.k2, build_q2k_block())
        let want <- formula_row(256) $(k) {
            let g = k / 16
            return SYNTH_D * float(SYNTH_SC[g % 8] & 15) * float(q2_pat(k)) - SYNTH_DMIN * float(SYNTH_MN[g % 8] & 15)
        }
        check(t, "k2 plane against the upstream dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
}

//! the iq2 family's and the per-32 formats' plane cells
def private lattice_plane_cells(t : T?) {
    t |> run("iq2xs planes eighth the d, decode the strips, and gather the 512-entry grid + ksigns exactly") @(t : T?) {
        var inscope p <- plane_row(KqFmt.iq2xs, build_iq2xs_block())
        for (g in range(16)) {
            t |> equal(int(p.ks[4 + g]), 1 + 2 * (SYNTH_SC[g % 8] & 15), "decoded strip is 1 + 2s")
        }
        let d8 = SYNTH_D * 0.125
        let want <- formula_row(256) $(k) {
            let b = k / 32
            let l = (k % 32) / 8
            let j = k % 8
            let idx = iq2xs_ix_pat(b * 4 + l)
            let gw = j < 4 ? IQ2XS_GRID[idx * 2] : IQ2XS_GRID[idx * 2 + 1]
            let g = int((gw >> uint(8 * (j % 4))) & 0xFFu)
            let sgn = ((int(KSIGNS_IQ2XS[iq2xs_sg7_pat(b * 4 + l)]) >> j) & 1) != 0 ? -1.0 : 1.0
            return (d8 * float(1 + 2 * (SYNTH_SC[(2 * b + l / 2) % 8] & 15))) * float(g) * sgn
        }
        check(t, "iq2xs plane against the upstream dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("iq2xxs planes eighth the d, decode the per-32 strips, and gather the 256-entry grid + ksigns exactly") @(t : T?) {
        var inscope p <- plane_row(KqFmt.iq2xxs, build_iq2xxs_block())
        for (b in range(8)) {
            t |> equal(int(p.ks[4 + b]), 1 + 2 * (SYNTH_SC[b % 8] & 15), "decoded per-32 strip is 1 + 2s")
        }
        let d8 = SYNTH_D * 0.125
        let want <- formula_row(256) $(k) {
            let b = k / 32
            let l = (k % 32) / 8
            let j = k % 8
            let idx = iq2xxs_ix_pat(b * 4 + l)
            let gw = j < 4 ? IQ2XXS_GRID[idx * 2] : IQ2XXS_GRID[idx * 2 + 1]
            let g = int((gw >> uint(8 * (j % 4))) & 0xFFu)
            let sgn = ((int(KSIGNS_IQ2XS[iq2xxs_sg7_pat(b * 4 + l)]) >> j) & 1) != 0 ? -1.0 : 1.0
            return (d8 * float(1 + 2 * (SYNTH_SC[b % 8] & 15))) * float(g) * sgn
        }
        check(t, "iq2xxs plane against the upstream dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("iq2s planes eighth the d, decode the strips, and gather the u64 grid exactly") @(t : T?) {
        var inscope p <- plane_row(KqFmt.iq2s, build_iq2s_block())
        for (g in range(16)) {
            t |> equal(int(p.ks[4 + g]), 1 + 2 * (SYNTH_SC[g % 8] & 15), "decoded strip is 1 + 2s")
        }
        let d8 = SYNTH_D * 0.125
        let want <- formula_row(256) $(k) {
            let b = k / 32
            let l = (k % 32) / 8
            let j = k % 8
            let idx = iq2s_ix_pat(b * 4 + l) | ((iq2s_qh_pat(b) << (8 - 2 * l)) & 0x300)
            let gw = j < 4 ? IQ2S_GRID[idx * 2] : IQ2S_GRID[idx * 2 + 1]
            let g = int((gw >> uint(8 * (j % 4))) & 0xFFu)
            let sgn = ((iq2s_sg_pat(b * 4 + l) >> j) & 1) != 0 ? -1.0 : 1.0
            return (d8 * float(1 + 2 * (SYNTH_SC[(2 * b + l / 2) % 8] & 15))) * float(g) * sgn
        }
        check(t, "iq2s plane against the upstream dequant", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("q51 planes unpack the hand-packed Q5_1 blocks exactly") @(t : T?) {
        for (blk in range(4)) {
            var inscope p <- plane_row(KqFmt.q51, build_q51_block(blk))
            let want <- formula_row(32) $(k) => q51_d(blk) * float(q5_pat(k + blk)) + q51_m(blk)
            check(t, "q51 block {blk} plane against the hand-packed values", p.w, want, BAR_EXACT, FLAT_SHAPE)
        }
    }
    t |> run("iq4nl32 planes unpack the hand-packed IQ4_NL blocks through the codebook exactly") @(t : T?) {
        for (blk in range(4)) {
            var inscope p <- plane_row(KqFmt.iq4nl32, build_iq4nl32_block(blk))
            let want <- formula_row(32) $(k) => iq4nl32_w(blk, k)
            check(t, "iq4nl32 block {blk} plane against the hand-packed values", p.w, want, BAR_EXACT, FLAT_SHAPE)
        }
    }
}

// the per-32 gates (q51, iq4nl32 - not the superblock lattice): one gate a stage over the format's tag, the overloads below its per-format calls

def private p32_fmt(_f : KqTag_q51) : KqFmt => KqFmt.q51
def private p32_fmt(_f : KqTag_iq4nl32) : KqFmt => KqFmt.iq4nl32
def private p32_n(_f : KqTag_q51) : int64 => 704l   // the real 26B-A4B expert row: % 32, NOT % 256 — why the tier exists
def private p32_n(_f : KqTag_iq4nl32) : int64 => 2560l   // the qwen4exp expert row: % 32, NOT % 256 - why the format exists
def private p32_block(blk : int; _f : KqTag_q51) : array<uint8> => build_q51_block(blk)
def private p32_block(blk : int; _f : KqTag_iq4nl32) : array<uint8> => build_iq4nl32_block(blk)
def private p32_dequant(kq : array<uint8>; ko : int64; ks : array<uint8>; so : int64; var w : array<float>; wo : int64; _f : KqTag_q51) => dequant_q51_plane_block(kq, ko, ks, so, w, wo)
def private p32_dequant(kq : array<uint8>; ko : int64; ks : array<uint8>; so : int64; var w : array<float>; wo : int64; _f : KqTag_iq4nl32) => dequant_iq4nl32_plane_block(kq, ko, ks, so, w, wo)
def private p32_dot(wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?; n : int64; _f : KqTag_q51) : float => dot_q51q8_scalar(wq, ws, xq, xs, n)
def private p32_dot(wq : uint8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?; n : int64; _f : KqTag_iq4nl32) : float => dot_iq4nl32q8_scalar(wq, ws, xq, xs, n)
def private p32_groupn(var y : array<float>; kq, ks : array<uint8>; offs : array<int64>; nreg : int64; xq : array<int8>; xs : array<float>; n, d : int64; _f : KqTag_q51) => matmul_q51q8_groupn(y, kq, ks, offs, nreg, xq, xs, n, d)
def private p32_groupn(var y : array<float>; kq, ks : array<uint8>; offs : array<int64>; nreg : int64; xq : array<int8>; xs : array<float>; n, d : int64; _f : KqTag_iq4nl32) => matmul_iq4nl32q8_groupn(y, kq, ks, offs, nreg, xq, xs, n, d)
def private p32_batch(var y : array<float>; kq, ks : array<uint8>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64; _f : KqTag_q51) => matmul_q51q8_batch(y, kq, ks, woff, xq, xs, n, d, ntok)
def private p32_batch(var y : array<float>; kq, ks : array<uint8>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64; _f : KqTag_iq4nl32) => matmul_iq4nl32q8_batch(y, kq, ks, woff, xq, xs, n, d, ntok)
def private p32_batch_groupn(var y : array<float>; kq, ks : array<uint8>; offs : array<int64>; nreg : int64; xq : array<int8>; xs : array<float>; n, d : int64; _f : KqTag_q51) => matmul_q51q8_batch_groupn(y, kq, ks, offs, nreg, xq, xs, n, d)
def private p32_batch_groupn(var y : array<float>; kq, ks : array<uint8>; offs : array<int64>; nreg : int64; xq : array<int8>; xs : array<float>; n, d : int64; _f : KqTag_iq4nl32) => matmul_iq4nl32q8_batch_groupn(y, kq, ks, offs, nreg, xq, xs, n, d)
def private p32_grp_row_dot(qg : uint8 const?; sg : uint8 const?; r, mr : int64; xq : int8 const?; xs : float const?; xbs : int const?; n : int64; _f : KqTag_q51) : float => q51_grp_row_dot(qg, sg, r, mr, xq, xs, xbs, n)
def private p32_grp_row_dot(qg : uint8 const?; sg : uint8 const?; r, mr : int64; xq : int8 const?; xs : float const?; _xbs : int const?; n : int64; _f : KqTag_iq4nl32) : float => iq4nl32_grp_row_dot(qg, sg, r, mr, xq, xs, n)
def private p32_expand_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64; _f : KqTag_q51) => expand_q51_grp_row(gq, r, mr, qe, nb)
def private p32_expand_grp_row(gq : uint8 const?; r, mr : int64; var qe : int8?; nb : int64; _f : KqTag_iq4nl32) => expand_iq4nl32_grp_row(gq, r, mr, qe, nb)
def private p32_grp_scales(gs : uint8 const?; r, mr : int64; var ss : uint8?; nb : int64; _f : KqTag_q51) => gather_q51_grp_scales(gs, r, mr, ss, nb)
def private p32_grp_scales(gs : uint8 const?; r, mr : int64; var ss : uint8?; nb : int64; _f : KqTag_iq4nl32) => gather_iq4nl32_grp_scales(gs, r, mr, ss, nb)
def private p32_expand_row(wq : uint8 const?; var qe : int8?; nb : int64; _f : KqTag_q51) => expand_q51_row(wq, qe, nb)
def private p32_expand_row(wq : uint8 const?; var qe : int8?; nb : int64; _f : KqTag_iq4nl32) => expand_iq4nl32_row(wq, qe, nb)
def private p32_dot_e(qe : int8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?; n : int64; _f : KqTag_q51) : float => dot_q51e(qe, ws, xq, xs, n)
def private p32_dot_e(qe : int8 const?; ws : uint8 const?; xq : int8 const?; xs : float const?; n : int64; _f : KqTag_iq4nl32) : float => dot_iq4nl32e(qe, ws, xq, xs, n)
def private p32_gather(m : Model; n, d : int64; repacked : bool; mr : int64; var wq, ws : array<uint8>; _f : KqTag_q51) => moe_gpu_gather_stack_q51(m, 0l, n, d, d, repacked, mr, wq, ws)
def private p32_gather(m : Model; n, d : int64; repacked : bool; mr : int64; var wq, ws : array<uint8>; _f : KqTag_iq4nl32) => moe_gpu_gather_stack_iq4nl32(m, 0l, n, d, d, repacked, mr, wq, ws)

//! `m`'s per-32 plane pair of the tag's format set to `kq` / `ks`
def private p32_put_planes(var m : Model; kq, ks : array<uint8>; _f : KqTag_q51) {
    m.q51q := kq
    m.q51s := ks
}

def private p32_put_planes(var m : Model; kq, ks : array<uint8>; _f : KqTag_iq4nl32) {
    m.iq4nl32q := kq
    m.iq4nl32s := ks
}

//! `m`'s per-32 plane pair of the tag's format repacked grp<mr> in place
def private p32_repack_model(var m : Model; n, d, mr : int64; _f : KqTag_q51) {
    unsafe {
        repack_kq_grp(KqFmt.q51, addr(m.q51q[0]), addr(m.q51s[0]), n, d, mr)
    }
}

def private p32_repack_model(var m : Model; n, d, mr : int64; _f : KqTag_iq4nl32) {
    unsafe {
        repack_kq_grp(KqFmt.iq4nl32, addr(m.iq4nl32q[0]), addr(m.iq4nl32s[0]), n, d, mr)
    }
}

//! `rows` per-32 plane rows of n; row r block b packs synthetic block r*31 + b, so no two blocks share data and offset bugs can't cancel
def private p32_fill_planes(var kq : array<uint8>; var ks : array<uint8>; rows, n : int64; f : auto(TT)) {
    let kf = p32_fmt(f)
    let qb = kq_qsb(kf)
    let sb = kq_ssb(kf)
    let nb = n / 32l
    kq |> resize(rows * nb * qb)
    ks |> resize(rows * nb * sb)
    for (r in range64(rows)) {
        for (b in range64(nb)) {
            let blkb <- p32_block(int(r * 31l + b), f)
            kq_transcode_superblock(kf, blkb, 0l, kq, (r * nb + b) * qb, ks, (r * nb + b) * sb)
        }
    }
}

// smooth per-row-shifted activation rows -> the Q8_0-form image (per-32 scales, no block sums)
def private q51_fill_acts(var xq : array<int8>; var xs : array<float>; rows, n : int64) {
    var src : array<float>
    src |> resize(n)
    xq |> resize(rows * n)
    xs |> resize(rows * (n / 32l))
    for (r in range64(rows)) {
        for (i in range64(n)) {
            src[i] = 1.7 * sin(float(i + r * 37l) * 0.23) + 0.3 * cos(float(i) * 0.071)
        }
        quantize_q8_0_into(src, n, xq, xs, r * n, r * (n / 32l))
    }
    delete src
}

def private p32_dot_gate(t : T?; f : auto(TT)) {
    let n = p32_n(f)
    let kf = p32_fmt(f)
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    p32_fill_planes(kq, ks, 1l, n, f)
    var inscope w : array<float>
    w |> resize(n)
    for (b in range64(n / 32l)) {
        p32_dequant(kq, b * kq_qsb(kf), ks, b * kq_ssb(kf), w, b * 32l, f)
    }
    var inscope xq : array<int8>
    var inscope xs : array<float>
    q51_fill_acts(xq, xs, 1l, n)
    var ref = 0.0lf
    for (i in range64(n)) {
        ref += double(w[i]) * double(xq[i]) * double(xs[i / 32l])
    }
    let got = p32_dot(unsafe(addr(kq[0])), unsafe(addr(ks[0])), unsafe(addr(xq[0])), unsafe(addr(xs[0])), n, f)
    let denom = max(abs(float(ref)), 1.0e-6)
    t |> success(abs(got - float(ref)) / denom < 1.0e-4, "the {kf} scalar dot must match the fp64 plane-dequant reference")
    t |> success(abs(got - float(ref) + 0.5) / denom >= 1.0e-4, "the dot bar's control: an added 0.5 must land outside it")
}

//! the scalar row dots of rows [r0, r0 + d) of a per-32 plane against activation row `xr`, for every (region, row) pair
//! `rows` names: element i is the dot of plane row `wrow(i)` with activation row `xrow(i)`
def private p32_row_dots(kq, ks : array<uint8>; xq : array<int8>; xs : array<float>; n : int64; cnt : int64;
                         wrow : block<(i : int64) : int64>; xrow : block<(i : int64) : int64>; f : auto(TT)) : array<float> {
    let kf = p32_fmt(f)
    let rq = n / 32l * kq_qsb(kf)
    let rs = n / 32l * kq_ssb(kf)
    var want <- nan_plane(int(cnt))
    unsafe {
        for (i in range64(cnt)) {
            let wr = invoke(wrow, i)
            let xr = invoke(xrow, i)
            want[i] = p32_dot(addr(kq[wr * rq]), addr(ks[wr * rs]), addr(xq[xr * n]), addr(xs[xr * (n / 32l)]), n, f)
        }
    }
    return <- want
}

def private p32_groupn_gate(t : T?; f : auto(TT)) {
    let n = p32_n(f)
    let kf = p32_fmt(f)
    let d = 8l
    let nreg = 3l
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    p32_fill_planes(kq, ks, nreg * d, n, f)
    var inscope xq : array<int8>
    var inscope xs : array<float>
    q51_fill_acts(xq, xs, nreg, n)
    var inscope offs : array<int64>
    offs |> reserve(nreg * 2l)
    for (r in range64(nreg)) {   // (weight element off, activation element off) pairs
        offs |> push(r * d * n)
        offs |> push(r * n)
    }
    var inscope y <- nan_plane(int(nreg * d))
    p32_groupn(y, kq, ks, offs, nreg, xq, xs, n, d, f)
    var inscope want <- p32_row_dots(kq, ks, xq, xs, n, nreg * d, $(i) => i, $(i) => i / d, f)
    check(t, "{kf} groupn rows against the scalar row dots", y, want, BAR_EXACT, Shape(row_width = int(d)))
}

def private p32_batch_gate(t : T?; f : auto(TT)) {
    let n = p32_n(f)
    let kf = p32_fmt(f)
    let d = 8l
    let ntok = 5l
    let nreg = 2l
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    p32_fill_planes(kq, ks, nreg * d, n, f)
    var inscope xq : array<int8>
    var inscope xs : array<float>
    q51_fill_acts(xq, xs, ntok, n)
    // batch off region 1's rows — the non-zero element woff pins the wrapper's /32 plane math
    var inscope y <- nan_plane(int(ntok * d))
    p32_batch(y, kq, ks, d * n, xq, xs, n, d, ntok, f)
    var inscope want <- p32_row_dots(kq, ks, xq, xs, n, ntok * d, $(i) => d + i % d, $(i) => i / d, f)
    check(t, "{kf} batch cells against the scalar row dots", y, want, BAR_EXACT, Shape(row_width = int(d)))
    // batch groupn: (woff, row0, cnt) triples — tokens 0..2 hit region 0, 2..5 region 1
    let offs <- [0l, 0l, 2l, d * n, 2l, 3l]
    var inscope yg <- nan_plane(int(ntok * d))
    p32_batch_groupn(yg, kq, ks, offs, nreg, xq, xs, n, d, f)
    var inscope wantg <- p32_row_dots(kq, ks, xq, xs, n, ntok * d, $(i) => (i / d < 2l ? 0l : d) + i % d, $(i) => i / d, f)
    check(t, "{kf} batch-groupn cells against the scalar row dots", yg, wantg, BAR_EXACT, Shape(row_width = int(d)))
}

// the grp<mr> per-32 layout (the emitter family's plane): repack round-trip, the grp scalar walk,
// the grp expand twins, and the untouched disk-order tail rows — all bit-exact vs disk
def private p32_grp_gate(t : T?; f : auto(TT)) {
    let n = p32_n(f)
    let kf = p32_fmt(f)
    let rq = n / 32l * kq_qsb(kf)
    let rs = n / 32l * kq_ssb(kf)
    let nb = n / 32l
    let d = 26l   // 3 grp8 (6 grp4) groups + a 2-row disk-order tail
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    p32_fill_planes(kq, ks, d, n, f)
    var inscope xq : array<int8>
    var inscope xs : array<float>
    q51_fill_acts(xq, xs, 1l, n)
    var inscope xbs : array<int>
    xbs |> resize(nb)
    for (bi in range64(nb)) {
        var s = 0
        for (j in range64(32l)) {
            s += int(xq[bi * 32l + j])
        }
        xbs[bi] = s
    }
    var inscope yref <- p32_row_dots(kq, ks, xq, xs, n, d, $(i) => i, $(_i) => 0l, f)
    unsafe {
        var inscope qe : array<int8>
        qe |> resize(n)
        var inscope qe2 : array<int8>
        qe2 |> resize(n)
        var inscope sc : array<uint8>
        sc |> resize(nb * kq_ssb(kf))
        for (mr in fixed_array(4l, 8l)) {
            var inscope gq := kq
            var inscope gs := ks
            repack_kq_grp(kf, addr(gq[0]), addr(gs[0]), n, d, mr)
            let grpRows = (d / mr) * mr
            t |> success(mismatch_exact(gq, kq) > 0, "grp{mr}: the repack must move the quant bytes (the round-trip is not vacuous)")
            var inscope got <- nan_plane(int(d))
            var inscope ge <- nan_plane(int(grpRows))
            var inscope de <- nan_plane(int(grpRows))
            for (r in range64(d)) {
                if (r < grpRows) {
                    let gbase = (r / mr) * mr * nb
                    got[r] = p32_grp_row_dot(addr(gq[0]) + gbase * kq_qsb(kf), addr(gs[0]) + gbase * kq_ssb(kf), r % mr, mr,
                                             addr(xq[0]), addr(xs[0]), addr(xbs[0]), n, f)
                    p32_expand_grp_row(addr(gq[0]) + gbase * kq_qsb(kf), r % mr, mr, addr(qe[0]), nb, f)
                    p32_grp_scales(addr(gs[0]) + gbase * kq_ssb(kf), r % mr, mr, addr(sc[0]), nb, f)
                    p32_expand_row(addr(kq[0]) + r * rq, addr(qe2[0]), nb, f)
                    ge[r] = p32_dot_e(addr(qe[0]), addr(sc[0]), addr(xq[0]), addr(xs[0]), n, f)
                    de[r] = p32_dot_e(addr(qe2[0]), addr(ks[0]) + r * rs, addr(xq[0]), addr(xs[0]), n, f)
                } else {
                    got[r] = p32_dot(addr(gq[0]) + r * rq, addr(gs[0]) + r * rs, addr(xq[0]), addr(xs[0]), n, f)
                }
            }
            check(t, "grp{mr}: the {kf} grp row dots (and the disk-order tail rows) against the disk scalar dots", got, yref, BAR_EXACT, FLAT_SHAPE)
            check(t, "grp{mr}: the {kf} grp expand twins against the disk expand", ge, de, BAR_EXACT, FLAT_SHAPE)
            var inscope ydisk <- copy_span(yref, 0l, grpRows)
            check(t, "grp{mr}: the {kf} disk expand's dot against the scalar dot", de, ydisk, BAR_EXACT, FLAT_SHAPE)
        }
    }
}

//! the GPU tier's per-32 expert gather: the device planes (disk order, row-major) gathered off the grp<mr> plane must
//! equal the ones gathered off the disk-order plane, byte for byte, groups and the disk-order tail alike
def private p32_gather_gate(t : T?; f : auto(TT)) {
    let n = p32_n(f)
    let kf = p32_fmt(f)
    let nb = n / 32l
    let d = 26l   // 3 grp8 groups + a 2-row disk-order tail
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    p32_fill_planes(kq, ks, d, n, f)
    var m = Model()
    p32_put_planes(m, kq, ks, f)
    var inscope wqDisk : array<uint8>
    var inscope wsDisk : array<uint8>
    p32_gather(m, n, d, false, 1l, wqDisk, wsDisk, f)
    check_exact(t, "the {kf} disk-order gather against the plane itself", wqDisk, kq)
    t |> equal(long_length(wqDisk), d * nb * kq_qsb(kf))
    t |> equal(long_length(wsDisk), d * nb * kq_ssb(kf))
    for (mr in fixed_array(4l, 8l)) {
        var g = Model()
        p32_put_planes(g, kq, ks, f)
        p32_repack_model(g, n, d, mr, f)
        var inscope wqGrp : array<uint8>
        var inscope wsGrp : array<uint8>
        p32_gather(g, n, d, true, mr, wqGrp, wsGrp, f)
        check_exact(t, "grp{mr}: the gathered {kf} quant bytes against the disk-order gather", wqGrp, wqDisk)
        check_exact(t, "grp{mr}: the gathered {kf} scale words against the disk-order gather", wsGrp, wsDisk)
        delete g
    }
    delete m
}

//! the five per-32 cells of the tag's format, named for it
def private p32_cells(t : T?; f : auto(TT)) {
    let kf = p32_fmt(f)
    let n = p32_n(f)
    t |> run("dot_{kf}q8_scalar matches the fp64 plane-dequant reference at n={int(n)}") @(t : T?) {
        p32_dot_gate(t, f)
    }
    t |> run("the GPU tier's {kf} gather off grp4 and grp8 planes matches the disk-order gather byte for byte") @(t : T?) {
        p32_gather_gate(t, f)
    }
    t |> run("{kf} groupn bit-matches scalar row dots") @(t : T?) {
        p32_groupn_gate(t, f)
    }
    t |> run("{kf} grp<mr> planes bit-match disk (repack + grp walk + expand twins + tails)") @(t : T?) {
        p32_grp_gate(t, f)
    }
    t |> run("{kf} batch + batch-groupn bit-match scalar row dots") @(t : T?) {
        p32_batch_gate(t, f)
    }
}

[test]
def test_q51_kernels(t : T?) {
    p32_cells(t, KqTag_q51())
}

[test]
def test_iq4nl32_kernels(t : T?) {
    p32_cells(t, KqTag_iq4nl32())
}

//! the disk-order row dequant of `rows` rows of `n` elements off format-`kf` planes: the gates' reference weights
def private plane_dequant(kf : KqFmt; kq, ks : array<uint8>; rows, n : int64) : array<float> {
    var w <- nan_plane(int(rows * n))
    for (sb in range64(rows * n / 256l)) {
        dequant_kq_plane_sb(kf, kq, ks, sb, false, w, sb * 256l)
    }
    return <- w
}

//! the integer-domain dot over LCG-distinct superblocks (no offset bug cancels) against the fp64 dot over the plane dequant
def private kq_dot_gate(t : T?; fmt : int; n : int64 = 512l) {
    let kf = kq_fmt_of_id(fmt, "kq_dot_gate")
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    synth_kq_region(fmt, kq, ks, 1l, n, 0x12345u, 0u)
    var inscope w <- plane_dequant(kf, kq, ks, 1l, n)   // the reference weights
    var inscope x <- build_q8k_acts(1l, n, Wave(a = 1.7, fa = 0.23, b = 0.3, fb = 0.071))   // kq v2: Q8_K-form activations, all formats
    // fp64 elementwise reference over the SAME quantized activation (Q8_K form: per-256 scale)
    var ref = 0.0lf
    for (i in range64(n)) {
        ref += double(w[i]) * double(x.xq[i]) * double(x.xs[i / 256l])
    }
    var got : float
    unsafe {
        got = dot_kq(addr(kq[0]), addr(ks[0]), addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n, kf)
    }
    let denom = max(abs(float(ref)), 1.0e-6)
    t |> success(abs(got - float(ref)) / denom < 1.0e-4, "kernel dot must match the fp64 plane-dequant reference")
}

[test]
def test_kq_dots(t : T?) {
    // beside 512, the 122B-class widths: 3072 (qwen35moe-122B gate/up/cls) and 1024 (its k5 down_exps)
    for (fmt in KQ_IDS) {
        let kf = kq_fmt_of_id(fmt, "test_kq_dots")
        for (nn in fixed_array(512l, 1024l, 3072l)) {
            t |> run("dot_{kf}q8 matches the fp64 reference at n={int(nn)}") @(t : T?) {
                kq_dot_gate(t, fmt, nn)
            }
        }
    }
}

// Portable multi-row GEMV gate: matmul_kq (the disk-order kq_gemv_kernel rail — what a
// kq-tagged model runs when the backend has no kq stamps) must bit-match the per-row disk
// dots. Guards the k*_rows_kernel PER-ROW plane strides, which the single-superblock dot
// gate above never exercises (a k4 scale-row stride miss shipped when K4_SSB went 32 -> 20:
// row 0 dotted clean, every later row read the wrong scale rows).
def private kq_gemv_rows_gate(t : T?; fmt : int; n : int64 = 512l) {
    let d = 8l
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let kf = kq_fmt_of_id(fmt, "kq_gemv_gate")
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    synth_kq_region(fmt, kq, ks, d, n, 0x9E3779Bu, 0x85EBCA6Bu)   // per-row LCG over the quant payload only (scale planes stay valid f16/int8)
    var inscope x <- build_q8k_acts(1l, n, Wave(a = 1.1, fa = 0.31, b = 0.5, fb = 0.043))
    var inscope y <- nan_plane(int(d))
    matmul_kq(fmt, y, kq, ks, 0l, x.xq, x.xs, x.xbs, n, d)
    var inscope want <- nan_plane(int(d))
    unsafe {
        for (r in range64(d)) {
            want[r] = dot_kq(addr(kq[r * nsb * qsb]), addr(ks[r * nsb * ssb]), addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n, kf)
        }
    }
    check(t, "portable {kf} multi-row GEMV against the per-row disk dots", y, want, BAR_EXACT, FLAT_SHAPE)
}

[test]
def test_kq_gemv_rows(t : T?) {
    for (fmt in KQ_IDS) {
        let kf = kq_fmt_of_id(fmt, "test_kq_gemv_rows")
        for (nn in fixed_array(512l, 1024l, 3072l)) {
            t |> run("portable {kf} GEMV rows bit-match per-row disk dots at n={int(nn)}") @(t : T?) {
                kq_gemv_rows_gate(t, fmt, nn)
            }
        }
    }
}

// A bf16 panel holds every weight of `wrow` at its pair-layout slot within `rel` of the weight
// plus `slack`; the same slots against their pair neighbors' weights must miss. A slot the
// builder never wrote keeps its NaN and counts as off.
def private bf16_panel_holds(t : T?; panel : array<uint16>; wrow : array<float>; n, d : int64; rel, slack : float; what : string) {
    var bad = 0
    var worst = 0.0
    var worstRow = 0l
    var worstCol = 0l
    var neighborSeen = 0
    for (r in range64(d)) {
        for (k in range64(n)) {
            let slot = (r / 16l) * 16l * n + (k / 32l) * 512l + ((k % 32l) / 2l) * 32l + (r % 16l) * 2l + (k & 1l)
            let w = wrow[r * n + k]
            let df = abs(bf16_value(panel[slot]) - w)
            if (!(df <= abs(w) * rel + slack)) {
                bad++
            }
            if (!(df <= worst)) {
                worst = df
                worstRow = r
                worstCol = k
            }
            let wn = wrow[r * n + (k ^ 1l)]
            if (!(abs(bf16_value(panel[slot]) - wn) <= abs(wn) * rel + slack)) {
                neighborSeen++
            }
        }
    }
    to_log(LOG_INFO, "{what}: max err {worst} at row {int(worstRow)} weight {int(worstCol)}\n")
    t |> success(bad == 0, "{what}: {bad} of {int(d * n)} slots off the weight, max err {worst} at row {int(worstRow)} weight {int(worstCol)}")
    t |> success(neighborSeen > 0, "{what}: a slot compared against its pair neighbor's weight must be seen off, and was not")
}

// The bf16 panel over grp16 planes: the row-dequant panel rounds to nearest, half a bf16 step
// (2^-8 of the weight); the panel a bf16 stamp builds (a tile form above 0 - off one the
// companion is an empty body) sits within a whole step plus the f32 slack of its fused scale fold.
def private bf16_panel_cell(t : T?; fmt : int; kq : array<uint8>; ks : array<uint8>; wrow : array<float>; n, d : int64) {
    let kf = kq_fmt_of_id(fmt, "bf16_panel_cell")
    let ngr = d / 16l
    var mag = 0.0
    for (w in wrow) {
        mag = max(mag, abs(w))
    }
    var panel : array<uint16>
    panel |> resize(ngr * 16l * n)
    for (h in panel) {
        h = uint16(0x7FC0)
    }
    unsafe {
        kq_panel_rows_bf16(fmt, addr<uint8 const?>(kq[0]), addr<uint8 const?>(ks[0]), 16l, n / 256l, n, ngr, addr(panel[0]))
    }
    bf16_panel_holds(t, panel, wrow, n, d, 0.00390625, 0.0, "{kf} row-dequant bf16 panel")
    if (kq_tileform_of(fmt) > 0) {
        for (h in panel) {
            h = uint16(0x7FC0)
        }
        unsafe {
            kq_panel_bf16(fmt, addr<uint8 const?>(kq[0]), addr<uint8 const?>(ks[0]), 16l, n / 256l, n, ngr, addr(panel[0]))
        }
        bf16_panel_holds(t, panel, wrow, n, d, 0.0078125, mag * 1.0e-5, "{kf} stamped bf16 panel")
    }
    delete panel
}

// Stage-4 repack oracle core: build a d-row disk plane of `fmt` (transcoded synthetic
// superblocks, per-row LCG-perturbed quant payloads — every quant bit pattern is valid),
// capture the portable disk dots + row dequants, repack in place at `mr`, and require the
// grp<mr> scalar row dot (the stamped cores' decline body, the kernel under test) and the grp
// row dequant to reproduce them BIT-EXACTLY (same block order, exact integer sub-sums — only the
// plane layout moved).
def private kq_repack_gate(t : T?; fmt : int; mr : int64; n : int64 = 512l) {
    let d = 32l
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let kf = kq_fmt_of_id(fmt, "kq_rows_gate")
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    synth_kq_region(fmt, kq, ks, d, n, 0x9E3779Bu, 0x85EBCA6Bu)
    var inscope x <- build_q8k_acts(1l, n, Wave(a = 1.3, fa = 0.19, b = -0.4, fb = 0.057))
    var inscope want <- nan_plane(int(d))
    var inscope wrow <- plane_dequant(kf, kq, ks, d, n)
    var inscope got <- nan_plane(int(d))
    var inscope row <- nan_plane(int(d * n))
    unsafe {
        for (r in range64(d)) {
            want[r] = dot_kq(addr(kq[r * nsb * qsb]), addr(ks[r * nsb * ssb]), addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n, kf)
        }
        repack_kq_grp(kf, addr(kq[0]), addr(ks[0]), n, d, mr)
        for (r in range64(d)) {
            let g = r / mr
            let kqg = addr(kq[g * mr * nsb * qsb])
            let ksg = addr(ks[g * mr * nsb * ssb])
            got[r] = kq_grp_row_dot_rt(kf, kqg, ksg, r % mr, mr, addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n)
            dequant_kq_row_grp(int64(fmt), kqg, ksg, r % mr, mr, n, addr(row[r * n]))
        }
    }
    check(t, "grp<{mr}> {kf} row dots against the disk dots after repack", got, want, BAR_EXACT, FLAT_SHAPE)
    check(t, "grp<{mr}> {kf} row dequant against the disk row dequant after repack", row, wrow, BAR_EXACT, Shape(row_width = int(n)))
    if (mr == 16l) {
        bf16_panel_cell(t, fmt, kq, ks, wrow, n, d)
    }
}

[test]
def test_kq_repack_grp(t : T?) {
    for (fmt in KQ_IDS) {
        let kf = kq_fmt_of_id(fmt, "test_kq_repack_grp")
        for (mr in fixed_array(4l, 8l, 16l)) {
            t |> run("repack_{kf}_grp mr={mr} preserves dots and row dequants") @(t : T?) {
                kq_repack_gate(t, fmt, mr)
            }
        }
        for (nn in fixed_array(1024l, 3072l)) {
            t |> run("repack_{kf}_grp mr=8 preserves dots and row dequants at n={int(nn)}") @(t : T?) {
                kq_repack_gate(t, fmt, 8l, nn)
            }
        }
    }
}

// Tile-vs-GEMV gate: the kq 4-token tile must be BIT-exact vs 4 per-token GEMVs over the same
// repacked planes (the tile's per-token fold order IS the GEMV's). Repack at the layout
// companion's mr — the same value the stubs/kernels read, so this holds on the reference
// bodies (interp) AND the stamped kernels (-jit) alike.
def private kq_tile_gate(t : T?; fmt : int; n : int64 = 512l) {
    if (kq_tileform_of(fmt) > 0) {
        t |> skip("the stamped tile is the bf16 tile - it reads a bf16 panel over 32 tokens, not the quant planes over 4; test_prefill_cpu_kernels reaches it through the batch kernel")
        return
    }
    let d = 32l
    let ntok = 7l   // 4-token tile + a 3-token gemv tail
    let nsb = n / 256l
    let mr = kq_layout_of(fmt)   // the format's OWN layout companion (per-format tune families)
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let kf = kq_fmt_of_id(fmt, "kq_tile_gate")
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    synth_kq_region(fmt, kq, ks, d, n, 0xDEADBEFu, 0x9E3779Bu)
    unsafe {
        repack_kq_grp(kf, addr(kq[0]), addr(ks[0]), n, d, mr)
    }
    var inscope x <- build_q8k_acts(ntok, n, Wave(a = 0.9, fa = 0.31, b = 0.5, fb = 0.043))   // ntok-row activation image, bs-quantized per row
    var inscope ytile <- nan_plane(int(ntok * d))
    var inscope ygemv <- nan_plane(int(ntok * d))
    // k5/k6 tiles read the byte-expanded panel the batch cell unpacks per (group, token-block)
    // — the gate covers unpack_kq_panel_grp too (tile-over-panel vs gemv-over-packed planes);
    // k4/q40 tiles read the packed planes directly
    let packed = kq_reads_packed_planes(int64(fmt))
    var inscope panel : array<uint8>
    if (!packed) {
        panel |> resize(mr * nsb * 256l)
    }
    unsafe {
        for (g in range64(d / mr)) {
            let kqg = addr(kq[g * mr * nsb * qsb])
            let ksg = addr(ks[g * mr * nsb * ssb])
            if (!packed) {
                unpack_kq_panel_grp(int64(fmt), kqg, addr(panel[0]), mr, nsb)
            }
            let tsrc = packed ? kqg : addr(panel[0])
            var tk = 0l
            while (tk + 4l <= ntok) {   // whole tiles
                kq_tile_rt(kf, addr(ytile[0]), tsrc, ksg, addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n, d, g, tk)
                tk += 4l
            }
            while (tk < ntok) {   // token tail via the rows core (the batch cell's shape)
                kq_gemv_rt(kf, addr(ytile[tk * d]), addr(kq[0]), addr(ks[0]), addr(x.xq[tk * n]), addr(x.xs[tk * nsb]), addr(x.xbs[tk * (n / 16l)]), n, g * mr, (g + 1l) * mr)
                tk++
            }
        }
        for (p in range64(ntok)) {   // reference: one full GEMV per token
            kq_gemv_rt(kf, addr(ygemv[p * d]), addr(kq[0]), addr(ks[0]), addr(x.xq[p * n]), addr(x.xs[p * nsb]), addr(x.xbs[p * (n / 16l)]), n, 0l, d)
        }
    }
    check(t, "{kf} 4-token tile against per-token GEMVs", ytile, ygemv, BAR_EXACT, Shape(row_width = int(d)))
}

[test]
def test_kq_tile(t : T?) {
    for (fmt in KQ_IDS) {
        let kf = kq_fmt_of_id(fmt, "test_kq_tile")
        t |> run("{kf} 4-token tile bit-matches per-token GEMVs") @(t : T?) {
            kq_tile_gate(t, fmt)
        }
        for (nn in fixed_array(1024l, 3072l)) {
            t |> run("{kf} 4-token tile bit-matches per-token GEMVs at n={int(nn)}") @(t : T?) {
                kq_tile_gate(t, fmt, nn)
            }
        }
    }
}

// Groupn gate: the region-list kq GEMV (the MoE expert dispatch) must be BIT-exact vs per-row
// disk dots — disk-order (the portable matmul_kq_groupn) AND repacked (kq_groupn_gen over
// per-region grp slices, the per-expert-slice repack shape). Regions carry distinct LCG
// payloads; both offs forms run — the shared activation image (gate/up, xoff 0) and
// per-region activation rows (down, xoff = r*n).
def private kq_groupn_gate(t : T?; fmt : int; n : int64 = 512l; d : int64 = 32l; nreg : int64 = 3l) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let kf = kq_fmt_of_id(fmt, "kq_groupn_gate")
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    synth_kq_region(fmt, kq, ks, nreg * d, n, 0x51ED270Bu, 0x85EBCA6Bu)
    var inscope x <- build_q8k_acts(nreg, n, Wave(a = 1.1, fa = 0.23, b = -0.6, fb = 0.071))   // nreg activation rows, bs-quantized per row
    var inscope offs_sh : array<int64>
    var inscope offs_pr : array<int64>
    offs_sh |> reserve(nreg * 2l)
    offs_pr |> reserve(nreg * 2l)
    for (r in range64(nreg)) {
        offs_sh |> push(r * d * n)
        offs_sh |> push(0l)
        offs_pr |> push(r * d * n)
        offs_pr |> push(r * n)
    }
    // references: per-row disk dots for both activation bindings
    var inscope want_sh <- nan_plane(int(nreg * d))
    var inscope want_pr <- nan_plane(int(nreg * d))
    unsafe {
        for (r in range64(nreg)) {
            for (row in range64(d)) {
                let sb = (r * d + row) * nsb
                let kqr = addr(kq[sb * qsb])
                let ksr = addr(ks[sb * ssb])
                want_sh[r * d + row] = dot_kq(kqr, ksr, addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n, kf)
                want_pr[r * d + row] = dot_kq(kqr, ksr, addr(x.xq[r * n]), addr(x.xs[r * nsb]), addr(x.xbs[r * (n / 16l)]), n, kf)
            }
        }
    }
    let rows = Shape(row_width = int(d))
    var inscope y <- nan_plane(int(nreg * d))
    matmul_kq_groupn(fmt, y, kq, ks, offs_sh, nreg, x.xq, x.xs, x.xbs, n, d)
    check(t, "{kf} disk groupn against per-row dots (shared image)", y, want_sh, BAR_EXACT, rows)
    y |> fill(NAN_SENTINEL)
    matmul_kq_groupn(fmt, y, kq, ks, offs_pr, nreg, x.xq, x.xs, x.xbs, n, d)
    check(t, "{kf} disk groupn against per-row dots (per-region rows)", y, want_pr, BAR_EXACT, rows)
    // per-region grp repack (the per-expert-slice shape), then the gen driver over the slices.
    // The grp reference is the ROWS CORE itself (one full-region call per region): under -jit
    // that's the stamped kernel, whose float fold order differs from the portable dots at the
    // last bit — the groupn driver's job is the region walk, so THAT must be bit-exact vs the
    // per-region core calls (interp and stamped alike). Rows-core-vs-disk exactness for the
    // das reference body is kq_repack_gate's gate.
    let mr = kq_layout_of(fmt)   // the format's OWN layout companion (per-format tune families)
    unsafe {
        for (r in range64(nreg)) {
            repack_kq_grp(kf, addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
        }
        want_sh |> fill(NAN_SENTINEL)
        want_pr |> fill(NAN_SENTINEL)
        for (r in range64(nreg)) {   // grp references: one direct rows-core call per region, both bindings
            let kqr = addr(kq[r * d * nsb * qsb])
            let ksr = addr(ks[r * d * nsb * ssb])
            kq_gemv_rt(kf, addr(want_sh[r * d]), kqr, ksr, addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n, 0l, d)
            kq_gemv_rt(kf, addr(want_pr[r * d]), kqr, ksr, addr(x.xq[r * n]), addr(x.xs[r * nsb]), addr(x.xbs[r * (n / 16l)]), n, 0l, d)
        }
        y |> fill(NAN_SENTINEL)
        kq_groupn_gen(fmt, addr(y[0]), addr(kq[0]), addr(ks[0]), addr(offs_sh[0]), nreg, addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n, d)
    }
    check(t, "{kf} grp groupn against per-region rows-core calls (shared image)", y, want_sh, BAR_EXACT, rows)
    unsafe {
        y |> fill(NAN_SENTINEL)
        kq_groupn_gen(fmt, addr(y[0]), addr(kq[0]), addr(ks[0]), addr(offs_pr[0]), nreg, addr(x.xq[0]), addr(x.xs[0]), addr(x.xbs[0]), n, d)
    }
    check(t, "{kf} grp groupn against per-region rows-core calls (per-region rows)", y, want_pr, BAR_EXACT, rows)
}

[test]
def test_kq_groupn(t : T?) {
    for (fmt in KQ_IDS) {
        let kf = kq_fmt_of_id(fmt, "test_kq_groupn")
        t |> run("{kf} region-list GEMV bit-matches per-row dots (disk + grp slices)") @(t : T?) {
            kq_groupn_gate(t, fmt)
        }
        for (nn in fixed_array(1024l, 3072l)) {
            t |> run("{kf} region-list GEMV bit-matches per-row dots at n={int(nn)}") @(t : T?) {
                kq_groupn_gate(t, fmt, nn)
            }
        }
    }
    // the qwen35moe-122B expert slice shapes: gate/up k4 n=3072 d=1024, down k5 n=1024 d=3072,
    // top-8 regions — the small-d arms above never walk >4 mr-groups per region
    t |> run("k4 region-list GEMV at the 122B gate/up slice shape (n=3072 d=1024 nreg=8)") @(t : T?) {
        kq_groupn_gate(t, 4, 3072l, 1024l, 8l)
    }
    t |> run("k5 region-list GEMV at the 122B down slice shape (n=1024 d=3072 nreg=8)") @(t : T?) {
        kq_groupn_gate(t, 5, 1024l, 3072l, 8l)
    }
}

// Batch-groupn gate: the fused MoE prefill's kq batch GEMM (matmul_kq_batch_groupn — triple offs
// (woff, row0, cnt), token-major y over the gathered image) must bit-match per-(region, token)
// rows-core calls over the same repacked planes. This drives kq_batch_cell_gen's TB/tile/tail
// walk over bucket-shaped token runs — the rail no per-region gate above reaches.
def private kq_batch_groupn_gate(t : T?; fmt : int; n : int64 = 512l; d : int64 = 32l) {
    // matmul_kq_batch_groupn dispatches through the active backend and only the gen tier carries
    // the slot; a pin that does not take (interp, no avx2, declined stamp) runs no cell
    let ran = with_kernel_backend(get_architecture_name() == "arm64" ? "arm64-gen" : "x64-gen") $ {
        if (!kernel_backend_has_kq_batch_groupn()) {
            t |> skip("no gen backend registered (kq_batch_groupn is JIT-only)")
            return
        }
        kq_batch_groupn_cell(t, fmt, n, d)
    }
    if (!ran) {
        t |> skip("no gen backend registered (kq_batch_groupn is JIT-only)")
    }
}

def private kq_batch_groupn_cell(t : T?; fmt : int; n, d : int64) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let mr = kq_layout_of(fmt)
    let kf = kq_fmt_of_id(fmt, "kq_batch_groupn_gate")
    // bucket mix: a 1-row run (pure gemv tail), whole tiles, tile+tail, and a TB-spanning run;
    // every run is under a bf16 tile's 32 tokens, so on that stamp the rows core serves them all
    // and the compare stays exact (test_prefill_cpu_kernels holds the longer runs' envelope)
    let cnts = fixed_array(1l, 4l, 6l, 13l)
    let nreg = 4l
    var nk = 0l
    for (c in cnts) {
        nk += c
    }
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    synth_kq_region(fmt, kq, ks, nreg * d, n, 0x7F4A7C15u, 0x85EBCA6Bu)
    unsafe {
        for (r in range64(nreg)) {   // per-region grp repack — the per-expert-slice shape
            repack_kq_grp(kf, addr(kq[r * d * nsb * qsb]), addr(ks[r * d * nsb * ssb]), n, d, mr)
        }
    }
    var inscope x <- build_q8k_acts(nk, n, Wave(a = 0.8, fa = 0.29, b = -0.4, fb = 0.057))   // gathered activation image: nk rows, Q8_K-quantized per row
    // region triples + per-(region, token) rows-core references
    var inscope offs : array<int64>
    offs |> reserve(nreg * 3l)
    var inscope want <- nan_plane(int(nk * d))
    var inscope y <- nan_plane(int(nk * d))
    var row0 = 0l
    unsafe {
        for (r in range64(nreg)) {
            offs |> push(r * d * n)   // triple stream, not a collectable comprehension
            offs |> push(row0)
            offs |> push(cnts[r])
            let kqr = addr(kq[r * d * nsb * qsb])
            let ksr = addr(ks[r * d * nsb * ssb])
            for (tk in range64(row0, row0 + cnts[r])) {
                kq_gemv_rt(kf, addr(want[tk * d]), kqr, ksr, addr(x.xq[tk * n]), addr(x.xs[tk * nsb]), addr(x.xbs[tk * (n / 16l)]), n, 0l, d)
            }
            row0 += cnts[r]
        }
    }
    matmul_kq_batch_groupn(fmt, y, kq, ks, offs, nreg, x.xq, x.xs, x.xbs, n, d)
    check(t, "{kf} batch groupn against per-(region, token) rows-core calls", y, want, BAR_EXACT, Shape(row_width = int(d)))
}

[test]
def test_kq_batch_groupn(t : T?) {
    for (fmt in KQ_IDS) {
        let kf = kq_fmt_of_id(fmt, "test_kq_batch_groupn")
        t |> run("{kf} batch groupn bit-matches per-token rows-core calls") @(t : T?) {
            kq_batch_groupn_gate(t, fmt)
        }
    }
    // the qwen35moe-122B fused-prefill slice shapes
    t |> run("k4 batch groupn at the 122B gate/up slice shape (n=3072 d=1024)") @(t : T?) {
        kq_batch_groupn_gate(t, 4, 3072l, 1024l)
    }
    t |> run("k5 batch groupn at the 122B down slice shape (n=1024 d=3072)") @(t : T?) {
        kq_batch_groupn_gate(t, 5, 1024l, 3072l)
    }
}

[test]
def test_quantize_bs(t : T?) {
    t |> run("bs quantizer matches quantize_q8_0 byte-for-byte plus exact per-16 sums") @(t : T?) {
        let n = 4096l
        var src : array<float>
        src |> resize(n)
        for (i in range64(n)) {
            src[i] = 2.0 * sin(float(i) * 0.37) - 0.11 * float(i % 13l)
        }
        let want <- quantize_q8_0(src, n)
        var qdst : array<int8>
        var sdst : array<float>
        var bdst : array<int>
        qdst |> resize(n)
        sdst |> resize(n / 32l)
        bdst |> resize(n / 16l)
        quantize_q8_0_bs_into(src, n, qdst, sdst, bdst, 0l, 0l, 0l)
        var bs : array<int>
        bs |> resize(n / 16l)
        for (h in range64(n / 16l)) {
            for (i in range64(16l)) {
                bs[h] += int(want.q[h * 16l + i])
            }
        }
        check_exact(t, "quants against quantize_q8_0", qdst, want.q)
        check_exact(t, "scales against quantize_q8_0", sdst, want.scales)
        check_exact(t, "per-16 sums", bdst, bs)
    }
}

// Dequant `n` elements from a Model kq plane starting at plane-local element offset `eloff`.
def private dequant_plane_prefix(tr : Model; fmt : KqFmt; eloff, n : int64; var dst : array<float>) {
    for (blk in range64(n / 256l)) {
        let sb = (eloff + blk * 256l) / 256l
        dequant_kq_plane_sb(fmt, tr.kq[int(fmt)].q, tr.kq[int(fmt)].s, sb, false, dst, blk * 256l)
    }
}

//! `path` loaded in the q8 mode with the native K-quant planes on (and the native Q4_0 ones with `q40`), each flag put back after
def private kq_native_load(path : string; q40 : bool) : Model {
    let prev_kq = get_kquant_native()
    let prev_q40 = get_kq_q40_native()
    set_kquant_native(true)
    if (q40) {
        set_kq_q40_native(true)
    }
    var tr <- load_gguf(path, QuantMode.q8)
    set_kquant_native(prev_kq)
    set_kq_q40_native(prev_q40)
    return <- tr
}

//! `blk` over stocked model `file` loaded kq-native (`kq_native_load`) - under the portable backend's pin when `pin_portable`,
//! so no kq-capable backend repacks the planes at load and they stay disk-order. JIT-only; a missing model skips.
def private load_kq_native(t : T?; file : string; pin_portable, q40 : bool; blk : block<(tr : Model; path : string) : void>) {
    return if (!jit_only(t))
    let path = path_join(models_dir(), file)
    return if (!model_available(t, path))
    var tr = Model()
    if (!pin_portable) {
        delete tr
        tr <- kq_native_load(path, q40)
    } else {
        let pinned = with_kernel_backend("portable") $ {
            delete tr
            tr <- kq_native_load(path, q40)
        }
        if (!pinned) {
            t |> success(false, "{file}: the portable pin did not take - the placement oracle reads disk-order planes")
            return
        }
    }
    invoke(blk, tr, path)
    delete tr
}

[test]
def test_kq_load_layout(t : T?) {
    // End-to-end stage-2 oracle: a kquant_native load must place every tagged tensor at its
    // cursor-assigned plane offset. Plane-dequant of tensor prefixes vs the file dequant —
    // exact for k6 (verbatim planes), quant-noise-free but f16-pair-rounded for k4 (rel RMS
    // bound catches any misplaced superblock loudly; the synthetic arms above pin the bit level).
    t |> run("kquant_native load places tensors at their cursor offsets (Qwen3-4B Q4_K_M)") @(t : T?) {
        // this arm is the stage-2 CURSOR-PLACEMENT oracle over disk-order planes — pin the
        // portable backend so a kq-capable gen backend can't repack them at load (stage 4)
        load_kq_native(t, "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", true, false) $(tr; path) {
            t |> success(!tr.kq_repacked, "portable-pinned load keeps the kq planes disk-order")
            t |> success(tr.kquant_native, "load detected native K-quant tensors")
            t |> success(cls_kq(tr), "tied Q6_K classifier takes the kq plane path")
            t |> success(tr.wq_fmt[0] == KqFmt.k4, "blk.0.attn_q tagged k4")
            t |> success(tr.wv_fmt[0] == KqFmt.k6, "blk.0.attn_v tagged k6")
            let n = 262144l
            var have : array<float>
            have |> resize(n)
            // deep-layer k4 tensor: catches cursor drift accumulated across every earlier tensor
            let l = tr.config.n_layers - 1l
            dequant_plane_prefix(tr, tr.wq_fmt[l], tr.wq_offs[l], n, have)
            var gt = 0
            var fdq <- read_prefix(path, "blk.{l}.attn_q.weight", n, gt)
            let r4 = rel_l2(have, fdq)
            t |> success(r4 < 1.0e-3, "deep-layer k4 plane matches the file dequant (f16-pair rounding only)")
            delete fdq
            // k6 (verbatim planes): the tied classifier row block — must be bit-exact
            var fdq6 <- read_prefix(path, "token_embd.weight", n, gt)
            dequant_plane_prefix(tr, tr.emb_fmt, tr.wcls_off, n, have)
            check(t, "k6 classifier plane against the file dequant", have, fdq6, BAR_EXACT, FLAT_SHAPE)
            delete fdq6
            delete have
        }
    }
    // the q40 tier's end-to-end placement oracle: the QAT gemma file is 265 x Q4_0 + F32
    // norms/router + one Q6_K embd — its q40 planes are VERBATIM disk splits, so every
    // plane-dequant prefix must bit-match the file dequant (the k6 exactness class).
    t |> run("q40 load places Q4_0 tensors at their cursor offsets (gemma-4-26B QAT q4_0)") @(t : T?) {
        load_kq_native(t, "gemma-4-26B_q4_0-it.gguf", true, true) $(tr; path) {   // disk-order planes (the placement oracle)
            t |> success(!tr.kq_repacked, "portable-pinned load keeps the q40 planes disk-order")
            t |> success(tr.kquant_native, "load detected native Q4_0 tensors")
            t |> success(tr.wq_fmt[0] == KqFmt.q40, "blk.0.attn_q tagged q40")
            let n = 262144l
            var have : array<float>
            have |> resize(n)
            var gt = 0
            // deep-layer q40 tensor: catches cursor drift accumulated across every earlier tensor
            let l = tr.config.n_layers - 1l
            dequant_plane_prefix(tr, tr.wq_fmt[l], tr.wq_offs[l], n, have)
            var fdq <- read_prefix(path, "blk.{l}.attn_q.weight", n, gt)
            t |> equal(gt, GGML_TYPE_Q4_0)
            check(t, "deep-layer q40 plane against the file dequant (verbatim split)", have, fdq, BAR_EXACT, FLAT_SHAPE)
            delete fdq
            delete have
        }
    }
}

// An activation whose Q8_K image is EXACT: integers in [-127, 127] with a +-127 peak in every
// 256-block, so the per-256 scale is 1 and the quants are the values. A GEMV over it therefore
// carries no activation-quantization error, and any deviation from the double dot of the file's
// own dequant is a plane-placement or kernel defect, never rounding.
def private exact_q8k_row(var x : array<float>; n, seed : int64) {
    for (i in range64(n)) {
        x[i] = float(((i * 37l + 11l + seed * 13l) % 255l) - 127l)
    }
}

// Largest |y[r] - double-dot(w row r, x)| over `rows`, relative to the dot's own magnitude; a NaN reads as infinite.
def private rows_dot_dev(w, x, y : array<float>; n, rows : int64) : float {
    var worst = 0.0
    for (r in range64(rows)) {
        var acc = 0.0lf
        for (i in range64(n)) {
            acc += double(w[r * n + i]) * double(x[i])
        }
        let dev = float(abs(double(y[r]) - acc) / (abs(acc) + 1.0lf))
        worst = max(worst, is_nan(dev) ? INFINITE_DIFF : dev)
    }
    return worst
}

[test]
def test_kq_q40_repacked_planes(t : T?) {
    // The REPACKED twin of test_kq_load_layout's q40 arm: that arm pins the portable backend, so
    // it proves cursor placement over disk-order planes only. This one runs the box's own backend,
    // so the grp<mr> repack, the per-expert offsets of the fused ffn_gate_up_exps split, the
    // tied-k6 classifier plane and the region-list expert dispatch are all held to the file's own
    // dequant at the 26B's production widths (dim 2816, n_ff_exp 704, 128 experts).
    t |> run("repacked q40 expert planes and the tied k6 classifier serve exactly (gemma-4-26B QAT q4_0)") @(t : T?) {
        load_kq_native(t, "gemma-4-26B_q4_0-it.gguf", false, true) $(tr; path) {
            q40_repacked_cell(t, tr, path)
        }
    }
}

def private q40_repacked_cell(t : T?; tr : Model; path : string) {
    let c = tr.config
    let dim = c.dim
    let nfe = c.n_ff_exp
    let ege = dim * nfe
    t |> equal(dim, 2816l, "the 26B's dim")
    t |> equal(nfe, 704l, "the 26B's n_ff_exp")
    t |> equal(c.n_expert, 128l, "the 26B's expert count")
    t |> success(tr.kquant_native, "load detected native Q4_0 tensors")
    t |> success(tr.emb_fmt == KqFmt.k6 && cls_kq(tr), "the Q6_K token_embd serves as the tied kq classifier")
    t |> success(fmt_at(tr.we1_fmt, 0l) == KqFmt.q40 && fmt_at(tr.we3_fmt, 0l) == KqFmt.q40,
        "the fused ffn_gate_up_exps split keeps both halves on the q40 plane")
    t |> success(fmt_at(tr.we2_fmt, 0l) == KqFmt.q8, "the 704-wide down stacks demote to the q8 rail")
    if (!tr.kq_repacked) {
        t |> skip("this box's backend carries no kq slots - the planes stay disk-order (test_kq_load_layout covers that)")
        return
    }
    var gt = 0
    let rows = 64l
    // the tied classifier's rows read through the grp<mr> gather: bit-exact vs the file dequant
    var femb <- read_prefix(path, "token_embd.weight", rows * dim, gt)
    var emb <- nan_plane(int(rows * dim))
    for (r in range64(rows)) {
        embed_row(tr, r, unsafe(addr(emb[r * dim])))
    }
    check(t, "embedding rows off the repacked k6 plane against the file dequant", emb, femb, BAR_EXACT, Shape(row_width = int(dim)))
    delete emb
    delete femb
    var x : array<float>
    x |> resize(dim)
    exact_q8k_row(x, dim, 0l)
    var xq : array<int8>
    xq |> resize(dim)
    var xs : array<float>
    xs |> resize(dim / 32l + 8l)
    var xbs : array<int>
    xbs |> resize(dim / 16l + 8l)
    var y : array<float>
    y |> resize(nfe)
    // expert e of the fused stack sits at file rows e*2*nfe (gate) and e*2*nfe + nfe (up);
    // the plane splits them into two regions at we1_offs/we3_offs + e*ege
    var gatebad = 0.0
    var upbad = 0.0
    var mustred = 0.0
    for (e in range64(4l)) {
        var wg <- read_prefix(path, "blk.0.ffn_gate_up_exps.weight", rows * dim, gt, e * 2l * nfe * dim)
        var wu <- read_prefix(path, "blk.0.ffn_gate_up_exps.weight", rows * dim, gt, (e * 2l * nfe + nfe) * dim)
        mm_f(y, tr, fmt_at(tr.we1_fmt, 0l), tr.we1_offs[0] + e * ege, x, xq, xs, xbs, dim, nfe)
        gatebad = max(gatebad, rows_dot_dev(wg, x, y, dim, rows))
        mustred = max(mustred, rows_dot_dev(wu, x, y, dim, rows))
        mm_f(y, tr, fmt_at(tr.we3_fmt, 0l), tr.we3_offs[0] + e * ege, x, xq, xs, xbs, dim, nfe)
        upbad = max(upbad, rows_dot_dev(wu, x, y, dim, rows))
        delete wu
        delete wg
    }
    t |> success(gatebad == 0.0, "every expert's gate region dots the fused tensor's gate rows exactly")
    t |> success(upbad == 0.0, "every expert's up region dots the fused tensor's up rows exactly")
    t |> success(mustred > 0.1, "the control: the gate region must NOT read the up half's rows")
    routed_dispatch_cell(t, tr, x, xq, xs, xbs, y)
    delete y
    delete xbs
    delete xs
    delete xq
    delete x
}

//! the routed dispatch reads the same planes through a region list - scattered experts, one call
def private routed_dispatch_cell(t : T?; tr : Model; x : array<float>; var xq : array<int8>; var xs : array<float>; var xbs : array<int>; var y : array<float>) {
    let dim = tr.config.dim
    let nfe = tr.config.n_ff_exp
    let ege = dim * nfe
    let k = 4l
    var offs : array<int64>
    offs |> resize(k * 2l)
    for (j in range64(k)) {
        offs[j * 2l] = tr.we1_offs[0] + (j * 7l + 3l) * ege
        offs[j * 2l + 1l] = 0l
    }
    var gu : array<float>
    gu |> resize(k * nfe)
    quantize_q8_k_into(x, dim, xq, xs, xbs, 0l, 0l, 0l)
    mm_at_kq_groupn(gu, tr, fmt_at(tr.we1_fmt, 0l), offs, k, xq, xs, xbs, dim, nfe)
    var want <- nan_plane(int(k * nfe))
    for (j in range64(k)) {
        mm_f(y, tr, fmt_at(tr.we1_fmt, 0l), offs[j * 2l], x, xq, xs, xbs, dim, nfe)
        for (r in range64(nfe)) {
            want[j * nfe + r] = y[r]
        }
    }
    check(t, "the region-list expert dispatch against the per-region GEMV", gu, want, BAR_EXACT, Shape(row_width = int(nfe)))
    delete want
    delete gu
    delete offs
}

[test]
def test_kq_fused_qkv_slices(t : T?) {
    // Fused-projection (Phi3) gate-lift oracle: attn_qkv / ffn_up split at load with dim-multiple
    // src_offs into the kq planes — a slice landing off by one superblock shows up as noise vs
    // the file dequant. The K slice (src_off = dim*qd) is the non-trivial placement.
    t |> run("phi3 fused attn_qkv K-quant slices land at their plane offsets (Phi-3.5 Q4_K_M)") @(t : T?) {
        load_kq_native(t, "Phi-3.5-mini-instruct-Q4_K_M.gguf", true, false) $(tr; path) {   // disk-order planes (the placement oracle, like test_kq_load_layout)
            t |> success(tr.kquant_native, "load detected native K-quant tensors")
            t |> success(tr.wq_fmt[0] == KqFmt.k5 && tr.wk_fmt[0] == KqFmt.k5 && tr.wv_fmt[0] == KqFmt.k5,
                "fused attn_qkv slices share the Q5_K tag")
            t |> success(tr.w1_fmt[0] == KqFmt.k4 && tr.w3_fmt[0] == KqFmt.k4, "fused ffn_up gate/up slices tagged k4")
            t |> success(tr.wcls_fmt == KqFmt.k6 && !cls_kq(tr), "untied Q6_K classifier tagged (no tied-cls path)")
            let dim = tr.config.dim
            let qd = layer_qd(tr.config, 0l)
            let n = 262144l
            var have : array<float>
            have |> resize(n)
            var gt = 0
            // q slice (src_off 0): the trivial placement
            dequant_plane_prefix(tr, tr.wq_fmt[0], tr.wq_offs[0], n, have)
            var fq <- read_prefix(path, "blk.0.attn_qkv.weight", dim * qd + n, gt)
            var want : array<float>
            want |> resize(n)
            for (i in range64(n)) {
                want[i] = fq[i]
            }
            t |> success(rel_l2(have, want) < 1.0e-3, "q slice matches the file dequant (f16-pair rounding only)")
            // k slice (src_off = dim*qd): the fused-split placement this arm exists for
            dequant_plane_prefix(tr, tr.wk_fmt[0], tr.wk_offs[0], n, have)
            for (i in range64(n)) {
                want[i] = fq[dim * qd + i]
            }
            t |> success(rel_l2(have, want) < 1.0e-3, "k slice at src_off dim*qd matches the file dequant")
            delete fq
            delete want
            delete have
        }
    }
}

[test]
def test_kq_ple_table(t : T?) {
    // E-series PLE token table (family c): gather-only tensor rides native kq planes — the tag
    // must take (a silent q8 fallback also passes parity, so pin it here) and whole-row plane
    // dequant must match the file dequant at its offset.
    t |> run("E-series PLE token table takes the kq plane path (gemma-4-E2B Q4_K_M)") @(t : T?) {
        load_kq_native(t, "gemma-4-E2B-it-Q4_K_M.gguf", true, false) $(tr; path) {   // disk-order planes (the placement oracle)
            t |> success(tr.kquant_native, "load detected native K-quant tensors")
            t |> success(tr.ple_emb_fmt == KqFmt.k5, "per_layer_token_embd tagged k5")
            t |> success(tr.emb_fmt == KqFmt.k4 && cls_kq(tr), "tied k4 classifier takes the kq plane path")
            // whole-row placement: rows 0..R of the table vs the file dequant (f16-pair rounding only)
            let all = tr.config.n_layers * tr.config.n_embd_per_layer
            let n = all * 16l
            var have : array<float>
            have |> resize(n)
            dequant_plane_prefix(tr, tr.ple_emb_fmt, tr.ple_emb_off, n, have)
            var gt = 0
            var fdq <- read_prefix(path, "per_layer_token_embd.weight", n, gt)
            t |> success(rel_l2(have, fdq) < 1.0e-3, "PLE table plane rows match the file dequant")
            delete fdq
            delete have
        }
    }
}

// Dequant `n` elements of `name` from element `first` into fp32; reports the on-disk type via `gt`.
def private read_prefix(path, name : string; n : int64; var gt : int&; first : int64 = 0l) : array<float> {
    var dst : array<float>
    dst |> reserve(n)
    dst |> resize(n)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("cannot open {path}")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var m <- parse_gguf_meta(bytes)
        gt = gguf_tensor_type(m, name)
        gguf_read_tensor_f32(m, bytes, name, dst, 0l, n, first)
        delete m
    }
    fclose(f)
    return <- dst
}


[test]
def test_iq3xxs_planes(t : T?) {
    t |> run("iq3xxs planes fold the 0.25 into d and the grid, and decode exactly") @(t : T?) {
        let blkb <- build_iq3xxs_block()
        var inscope p <- plane_row(KqFmt.iq3xxs, blkb)
        // independent recomputation off the DISK bytes through the identity tables
        let dh = f16_to_f32(f16_half_bits(uint(blkb[0]) | (uint(blkb[1]) << 8u)))
        let grid = iq3xxs_grid()
        let signs = ksigns_iq2xs()
        let want <- formula_row(256) $(k) {
            let b = k / 32
            let l = (k % 32) / 8
            let hj = k % 8
            var aux = 0u
            for (tt in range(4)) {
                aux |= uint(iq3xxs_aux_pat(b * 4 + tt)) << uint(8 * tt)
            }
            let db = dh * float(1 + 2 * int(aux >> 28u))
            let sgnb = int(signs[int((aux >> uint(7 * l)) & 127u)])
            let g = grid[iq3xxs_qs_pat(b * 8 + 2 * l + hj / 4)]
            let mag = float((g >> uint(8 * (hj % 4))) & 0xFFu)
            let sgn = ((sgnb >> hj) & 1) != 0 ? -1.0 : 1.0
            return db * mag * sgn
        }
        check(t, "iq3xxs plane against the table recomputation", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
}

[test]
def test_iq4nl_planes(t : T?) {
    t |> run("iq4nl planes split q40's bytes and decode through the codebook exactly") @(t : T?) {
        let blkb <- build_iq4nl_block()
        var inscope p <- plane_row(KqFmt.iq4nl, blkb)
        let want <- formula_row(256) $(k) {
            let b = k / 32
            let l = k % 32
            let d = f16_to_f32(uint(blkb[b * 18]) | (uint(blkb[b * 18 + 1]) << 8u))
            let q = iq4nl_qs_pat(b * 16 + (l % 16))
            let nib = l < 16 ? q & 15 : q >> 4
            return d * float(int(IQ4NL_LUT[nib]))
        }
        check(t, "iq4nl plane against the codebook recomputation", p.w, want, BAR_EXACT, FLAT_SHAPE)
    }
    t |> run("the per-32 formats refuse the lattice codecs by name: q8 has no plane transcode and no grp repack") @(t : T?) {
        var blkb : array<uint8>
        blkb |> resize(34)
        var kq : array<uint8>
        var ks : array<uint8>
        kq |> resize(128)
        ks |> resize(16)
        var tripped = false
        try {
            kq_transcode_superblock(KqFmt.q8, blkb, 0l, kq, 0l, ks, 0l)
        } recover {
            tripped = true
        }
        t |> success(tripped, "q8 plane transcode panics")
        tripped = false
        try {
            unsafe {
                repack_kq_grp(KqFmt.q8, addr(kq[0]), addr(ks[0]), 32l, 1l, 4l)
            }
        } recover {
            tripped = true
        }
        t |> success(tripped, "q8 grp repack panics")
        delete blkb
        delete kq
        delete ks
    }
    t |> run("iq4nl plane dequant reads its codebook on a team lane: lane values == main-thread values") @(t : T?) {
        iq4nl_lane_cell(t)
    }
}

def private iq4nl_lane_cell(t : T?) {
    let blkb <- build_iq4nl_block()
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(128)
    ks |> resize(16)
    kq_transcode_superblock(KqFmt.iq4nl, blkb, 0l, kq, 0l, ks, 0l)
    var main_dst : array<float>
    main_dst |> resize(256)
    dequant_iq4nl_plane_superblock(kq, 0l, ks, 0l, main_dst, 0l)
    var lane_dst : array<float>
    lane_dst |> resize(256)
    var ran = 0
    with_job_que() {
        set_jobque_fork_pool(true, true)
        set_jobque_team_mode(true)
        unsafe {
            let kqp = addr(kq)
            let ksp = addr(ks)
            var dp = addr(lane_dst)
            var rp = addr(ran)
            team_parallel_for(0, 1, 1) <| @(rb, re : int) {
                feint("{rb} {re}")
                dequant_iq4nl_plane_superblock(*kqp, 0l, *ksp, 0l, *dp, 0l)
                unsafe(rp[0]) = 1
            }
        }
        set_jobque_team_mode(false)
    }
    t |> equal(ran, 1, "the lane body ran")
    var nonzero = 0
    for (a in main_dst) {
        if (a != 0.0) {
            nonzero++
        }
    }
    t |> success(nonzero > 200, "the fixture's expected weights are not zero ({nonzero} nonzero of 256)")
    check(t, "the lane's dequant against the main thread's", lane_dst, main_dst, BAR_EXACT, FLAT_SHAPE)
    delete kq
    delete ks
    delete main_dst
    delete lane_dst
}

// the Metal-blob carrier splits a format's scale plane into [16B strips of every superblock]
// [the d tails] (ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane); this rebuilds that form from the
// CPU row form the way metal_blob_scale_plane does for the 20B rows: [f16 d][2 pad][16B strips]
def private split_scale_plane(ks : array<uint8>; ssb : int64) : array<uint8> {
    let nsb = long_length(ks) / ssb
    var out : array<uint8>
    out |> resize(nsb * 18l)
    for (sb in range64(nsb)) {
        for (i in range64(16l)) {
            out[sb * 16l + i] = ks[sb * ssb + 4l + i]
        }
        out[nsb * 16l + sb * 2l] = ks[sb * ssb]
        out[nsb * 16l + sb * 2l + 1l] = ks[sb * ssb + 1l]
    }
    return <- out
}

[test]
def test_kq_plane_sb_metal_blob_form(t : T?) {
    t |> run("dequant_kq_plane_sb reads the Metal-blob split scale plane to the same values as the row form") <| @(t : T?) {
        // one superblock per format from random disk bytes: transcode, dequant the row form, then
        // dequant the split form through the metal_blob arm - the values must agree exactly
        let gtypes <- [GGML_TYPE_IQ2_XXS, GGML_TYPE_IQ2_XS, GGML_TYPE_IQ2_S, GGML_TYPE_IQ3_XXS, GGML_TYPE_IQ3_S, GGML_TYPE_IQ4_XS]
        var seed = 11
        for (gtype in gtypes) {
            let fmt = gguf_kq_fmt_of_type(gtype)
            let sbb = ggml_type_bytes(gtype, 256l)
            var bytes : array<uint8>
            bytes |> reserve(sbb * 3l)
            var x = uint(seed) * 2654435761u
            for (_i in range64(sbb * 3l)) {
                x = x * 1664525u + 1013904223u
                bytes |> push(uint8((x >> 24u) & 0xFFu))
            }
            let qsb = kq_qsb(fmt)
            let ssb = kq_ssb(fmt)
            var kq : array<uint8>
            var ks : array<uint8>
            kq |> resize(int(qsb * 3l))
            ks |> resize(int(ssb * 3l))
            for (sb in range64(3l)) {
                kq_transcode_superblock(fmt, bytes, sb * sbb, kq, sb * qsb, ks, sb * ssb)
            }
            var rowform : array<float>
            var blobform : array<float>
            rowform |> resize(768)
            blobform |> resize(768)
            let split <- split_scale_plane(ks, ssb)
            for (sb in range64(3l)) {
                dequant_kq_plane_sb(fmt, kq, ks, sb, false, rowform, sb * 256l)
                dequant_kq_plane_sb(fmt, kq, split, sb, true, blobform, sb * 256l)
            }
            var bad = 0
            for (i in range(768)) {
                // random f16 scale bits can be NaN on both sides - a NaN pair is agreement
                if (rowform[i] != blobform[i] && !(rowform[i] != rowform[i] && blobform[i] != blobform[i])) {
                    bad++
                }
            }
            t |> equal(bad, 0, "{fmt}: split form == row form ({bad} of 768 differ)")
            delete bytes
            delete kq
            delete ks
            delete rowform
            delete blobform
            seed++
        }
    }
}

// The VBMI lattice's tables are pure host-side data - prove them on any box, no VBMI needed:
// every grid entry's packed code decodes back to the grid's own bytes through the alphabet,
// and the ksigns +-1 expansion matches a scalar re-derivation.
def private grid_words(kq : int) : array<uint> {
    if (kq == 34) {
        return <- [for (w in iq3xxs_grid()); w]
    } elif (kq == 33) {
        return <- [for (w in iq3s_grid()); w]
    } elif (kq == 25) {
        return <- [for (w in iq2xxs_grid2()); w]
    } elif (kq == 24) {
        return <- [for (w in iq2xs_grid2()); w]
    }
    return <- [for (w in iq2s_grid2()); w]
}

def private vbmi_plane_gate(t : T?; kq : int) {
    let alpha <- vbmi_alphabet(kq)
    let iq3 = kq == 33 || kq == 34
    let bits = iq3 ? 3 : 2
    let nbytes = iq3 ? 2 : 4
    let mask = iq3 ? 7 : 3
    let words <- grid_words(kq)
    var bad = 0
    for (word in words) {
        for (half in range(iq3 ? 2 : 1)) {   // iq3: one word = two plane bytes (low/high pair); iq2: one word = one plane byte
            let w = half == 0 ? word : (word >> 16u)
            let code = vbmi_pack_word(w, alpha, bits, nbytes)
            for (tb in range(nbytes)) {
                let sym = (code >> (bits * tb)) & mask
                let back = alpha[sym]
                let orig = int((w >> uint(8 * tb)) & 0xFFu)
                if (back != orig) {
                    bad++
                }
            }
        }
    }
    t |> equal(bad, 0, "kq {kq}: every grid byte round-trips through the code plane and alphabet")
}

[test]
def test_vbmi_lattice_tables(t : T?) {
    for (kq in [25, 24, 23, 34, 33]) {
        t |> run("vbmi code planes round-trip the kq {kq} grid") @(t : T?) {
            vbmi_plane_gate(t, kq)
        }
    }
    t |> run("pm1_of matches the scalar +-1 expansion of every ksigns byte") @(tt : T?) {
        var bad = 0
        for (sb in range(256)) {
            let packed = pm1_of(sb)
            for (k in range(8)) {
                let lane = int8(int(packed >> uint64(8 * k)) & 0xFF)
                let want = int8((sb & (1 << k)) != 0 ? -1 : 1)
                if (lane != want) {
                    bad++
                }
            }
        }
        tt |> equal(bad, 0)
    }
}

// The GPU device-plane gather's own invariant ("device bytes are identical either way"): gathering
// from disk-order planes and from the grp<mr>-repacked planes must produce byte-identical wq/ws.
// This is the only k3/k6 sub-block-plane reader with no dot-product oracle - the byte equality IS its gate.
def private kq_gpu_gather_gate(t : T?; fmt : int) {
    let n = 512l
    let d = 32l
    let mr = kq_layout_of(fmt)
    let kf = kq_fmt_of_id(fmt, "kq_gpu_gather_gate")
    var inscope kq : array<uint8>
    var inscope ks : array<uint8>
    synth_kq_region(fmt, kq, ks, d, n, 0xC0FFEEu, 0x9E3779Bu)
    var m = Model()
    m.kq[int(kf)].q := kq
    m.kq[int(kf)].s := ks
    var inscope wqDisk : array<uint8>
    var inscope wsDisk : array<uint8>
    moe_gpu_gather_stack_kq(m, kf, 0l, n, d, d, false, mr, wqDisk, wsDisk)
    unsafe {
        repack_kq_grp(kf, addr(m.kq[int(kf)].q[0]), addr(m.kq[int(kf)].s[0]), n, d, mr)
    }
    var inscope wqGrp : array<uint8>
    var inscope wsGrp : array<uint8>
    moe_gpu_gather_stack_kq(m, kf, 0l, n, d, d, true, mr, wqGrp, wsGrp)
    check_exact(t, "kq {fmt}: quant device bytes from the grp planes against the disk-order ones", wqGrp, wqDisk)
    check_exact(t, "kq {fmt}: scale device bytes from the grp planes against the disk-order ones", wsGrp, wsDisk)
    delete m
}

//! the decoded sub-scale byte a synthetic plane row carries at (row, blk): iq4xs's ls - 32, the iq3 family's 1 + 2s
def private iq_plane_sc(fmt : KqFmt; r, blk : int64) : int {
    return fmt == KqFmt.iq4xs ? int((r * 5l + blk * 7l) % 64l) - 32 : 1 + 2 * int((r * 3l + blk * 5l) % 16l)
}

def private iq_plane_d(r : int64) : float => 0.25 + 0.5 * float(r)

def private kq_gpu_gather_iq_gate(t : T?; fmt : KqFmt) {
    let n = 512l
    let d = 16l
    let nsb = n / 256l
    let mr = kq_layout_of(int(fmt))
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(d * nsb * qsb)
    ks |> resize(d * nsb * ssb)
    var st = 0xC0FFEEu
    for (i in range64(d * nsb * qsb)) {
        st = st * 1664525u + 1013904223u
        kq[i] = uint8(st >> 24u)
    }
    for (r in range64(d)) {
        for (sbi in range64(nsb)) {
            let ro = (r * nsb + sbi) * ssb
            let dh = f32_to_f16(iq_plane_d(r + sbi))
            ks[ro] = uint8(dh & 0xFFu)
            ks[ro + 1l] = uint8((dh >> 8u) & 0xFFu)
            for (blk in range64(8l)) {
                ks[ro + 4l + blk] = uint8(iq_plane_sc(fmt, r + sbi, blk) & 0xFF)
            }
        }
    }
    var m = Model()
    m.kq[int(fmt)].q := kq
    m.kq[int(fmt)].s := ks
    var wqDisk : array<uint8>
    var wsDisk : array<uint8>
    moe_gpu_gather_stack_kq(m, fmt, 0l, n, d, d, false, mr, wqDisk, wsDisk)
    // the device row is two words: word 0 = the f16 d with iq4xs's high scale bits above it, word 1 = eight 4-bit fields
    var bad = 0
    for (r in range64(d)) {
        for (sbi in range64(nsb)) {
            let wo = (r * nsb + sbi) * 8l
            let w0 = uint(wsDisk[wo]) | (uint(wsDisk[wo + 1l]) << 8u) | (uint(wsDisk[wo + 2l]) << 16u) | (uint(wsDisk[wo + 3l]) << 24u)
            let w1 = uint(wsDisk[wo + 4l]) | (uint(wsDisk[wo + 5l]) << 8u) | (uint(wsDisk[wo + 6l]) << 16u) | (uint(wsDisk[wo + 7l]) << 24u)
            if (f16_to_f32(w0 & 0xFFFFu) != iq_plane_d(r + sbi)) {
                bad++
            }
            for (blk in range64(8l)) {
                let f = int((w1 >> uint(4l * blk)) & 15u) | (int((w0 >> uint(16l + 2l * blk)) & 3u) << 4)
                let got = fmt == KqFmt.iq4xs ? f - 32 : 1 + 2 * f
                if (got != iq_plane_sc(fmt, r + sbi, blk)) {
                    bad++
                }
            }
        }
    }
    t |> equal(bad, 0, "kq {fmt}: the two-word device scale rows decode back to the plane's d and sub-scales")
    t |> equal(long_length(wqDisk), d * nsb * qsb, "kq {fmt}: the quant plane gathers whole")
    if (fmt == KqFmt.iq4xs || fmt == KqFmt.iq3s || fmt == KqFmt.iq3s4) {
        unsafe {
            repack_kq_grp(fmt, addr(m.kq[int(fmt)].q[0]), addr(m.kq[int(fmt)].s[0]), n, d, mr)
        }
        var wqGrp : array<uint8>
        var wsGrp : array<uint8>
        moe_gpu_gather_stack_kq(m, fmt, 0l, n, d, d, true, mr, wqGrp, wsGrp)
        check_exact(t, "kq {fmt}: quant device bytes from the grp planes against the disk-order ones", wqGrp, wqDisk)
        check_exact(t, "kq {fmt}: scale device bytes from the grp planes against the disk-order ones", wsGrp, wsDisk)
        delete wqGrp
        delete wsGrp
    }
    delete wqDisk
    delete wsDisk
    delete m
}

[test]
def test_kq_gpu_gather_iq(t : T?) {
    for (fmt in [KqFmt.iq4xs, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq2xxs, KqFmt.iq3s4]) {
        t |> run("moe_gpu_gather_stack_kq {fmt}: the codebook format's two-word device scale rows") @(t : T?) {
            kq_gpu_gather_iq_gate(t, fmt)
        }
    }
}

[test]
def test_kq_gpu_gather_grp(t : T?) {
    for (fmt in [3, 6]) {
        t |> run("moe_gpu_gather_stack_kq kq {fmt}: disk-order and grp planes gather identically") @(t : T?) {
            kq_gpu_gather_gate(t, fmt)
        }
    }
}

let private Q8P_NIN = 96l   //! three Q8_0 blocks a row

//! `nout` rows of 96 quantized by `quantize_q8_0_into` (amaxes whose /127 scales sit off the f16 lattice), repacked by
//! `repack_q8q8_grp` at `mr` / `kgroup` / `wbias`, every row read back by `q8_plane_row_f32` against q x its block's f32 scale
def private q8_plane_row_arm(t : T?; mr, kgroup, wbias, nout, tail : int64) {
    let tag = "q8_plane_row_f32 mr {mr} kgroup {kgroup} wbias {wbias} nout {nout}"
    t |> equal(nout % mr, tail, "{tag}: the rows past the last group of {mr}")
    let n = nout * Q8P_NIN
    let nb = Q8P_NIN / 32l
    var src <- [for (i in range64(n)); float((i * 37l + 11l) % 101l) * 0.0123457 - 0.61]
    var q : array<int8>
    var s : array<float>
    q |> resize(n)
    s |> resize(nout * nb)
    quantize_q8_0_into(src, n, q, s, 0l, 0l)
    var qr := q
    var sr := s
    unsafe {
        repack_q8q8_grp(addr(qr[0]), addr(sr[0]), Q8P_NIN, nout, mr, wbias, kgroup)
    }
    let lay : Q8RepackLayout = (repacked = true, mr = mr, kgroup = kgroup, wbias = wbias)
    var row : array<float>
    row |> resize(Q8P_NIN)
    var off_f32 = 0
    var off_f16 = 0
    for (r in range64(nout)) {
        unsafe {
            q8_plane_row_f32(addr<int8 const?>(qr[0]), addr<float const?>(sr[0]), Q8P_NIN, nout, lay, r, addr(row[0]))
        }
        for (j in range64(Q8P_NIN)) {
            let qv = float(q[r * Q8P_NIN + j])
            let d = s[r * nb + j / 32l]
            off_f32 += row[j] == qv * d ? 0 : 1
            off_f16 += row[j] == qv * float(float16(d)) ? 0 : 1
        }
    }
    t |> equal(off_f32, 0, "{tag}: every element is q x its block's f32 scale, bit for bit")
    t |> success(off_f16 > 0, "{tag}: the f16-rounded scale's product misses {off_f16} of {n} elements (the control: the compare sees a rounded scale)")
    delete src
    delete q
    delete s
    delete qr
    delete sr
    delete row
}

[test]
def test_q8_plane_row_f32(t : T?) {
    t |> run("q8_plane_row_f32: grp4 kgroup 8 bias128, 12 group rows and 15 with a tail of 3, q x the f32 scale bit for bit") @(t : T?) {
        q8_plane_row_arm(t, 4l, 8l, 128l, 12l, 0l)
        q8_plane_row_arm(t, 4l, 8l, 128l, 15l, 3l)
    }
    t |> run("q8_plane_row_f32: grp16 kgroup 8 unbiased, 32 group rows and 37 with a tail of 5, q x the f32 scale bit for bit") @(t : T?) {
        q8_plane_row_arm(t, 16l, 8l, 0l, 32l, 0l)
        q8_plane_row_arm(t, 16l, 8l, 0l, 37l, 5l)
    }
}
